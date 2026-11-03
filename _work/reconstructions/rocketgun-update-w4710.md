# `RocketGun::update(int)` — pass 10 (w4710, wave 97)

**Status:** research note, 2026-11-03. Worktree `wt/w4710`, branch `wave97-w4710`,
base `9b58a39d5`. **No source change landed**: every source shape tried this pass
scores <= 86.9 %. Tree clean, gate unchanged (byte 4500, linked 4500, size
4524/4524, data 843/843, imports 207, lint CLEAN, drift 0).

Target `_ZN9RocketGun6updateEi`: original **1908 B / 635 insns**, ours
**1908 B / 636 insns**, official verify score **86.9**.

Earlier notes: `rocketgun-update-w4512.md` (pass 9, aligned diff + spill-weight
sweep), `rocketgun-update-w4412.md` (pass 8, the most detailed),
`rocketgun-update-w4265.md`, `-w4248`, `-w4216`, `-w4111`, `-w4011`, `-w3831`,
`rocketgun-update-86-9-1908-b_w3811.md`.

This pass adds one new piece of evidence that the earlier notes did not have:
the **exact frame-slot map of every variant**, which localises the residual to
*spill-slot creation order*, not to register assignment. It also shows that the
one source shape that fixes the slots (the faithful form, no `trailMatrices`
local) breaks the frame bottom by 4 bytes and swaps `dt`/`this`.

---

## 1. Baseline (NDK r18b clang 7.0.2, this worktree)

```
cmake --build cmake-build-match --target check -j8
```
```
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`_ZN9RocketGun6updateEi`: 1908 B both sides, **635 vs 636** instructions,
**86.9 %**. All other `RocketGun` functions are byte-exact.

The current source carries four anchors (all documented inline):

| anchor | removal |
|---|---|
| `__builtin_expect(hasGeometry && delayActive, 1)` on the launch `if` | 84.6 |
| `volatile int padx = 0;` in the second launch branch | 86.7 |
| `volatile int unused = elapsed;` before the loop | 85.8 |
| `Array<Matrix> *trailMatrices = this->trailMatrices;` before the loop | 57.6 |
| all four removed (faithful form) | 61.6 (657 insns) |

---

## 2. Frame-slot map (new)

Every spill slot in the function, read off the disassembly. This is the single
most useful table in this note.

| slot | original | ours (baseline) | faithful (no anchors) |
|---|---|---|---|
| `[sp,#4]` | stack-guard address | stack-guard address | — |
| `[sp,#8]` | **`s`** | `c` | stack-guard address |
| `[sp,#12]` | **`c`** | **`elapsed`** | `c` |
| `[sp,#16]` | **`elapsed`** | `trailMatrices` local | **`elapsed`** |
| `[sp,#20]` | **`this`** | **`this`** | `dt` |
| `[sp,#24]` | `dt` | `dt` | **`this`** |
| `[sp,#28]` | — | `unused` / `padx` (shared) | — |
| `[sp,#156]` | stack-guard value | stack-guard value | stack-guard value |

Consequences:

* The original can merge the two adjacent stores into
  `strd r4, sl, [sp,#16]` (`elapsed`@16, `this`@20). Ours emits
  `str r5,[sp,#12]` + `str.w sl,[sp,#20]` because 12 and 20 are not adjacent.
  That is the whole 636-vs-635 instruction-count delta.
* The original spills **`s`** to `[sp,#8]`; ours keeps `s` in `r6`. Everything
  else in the table is a downstream shift of that one missing spill.
* The **faithful form already has the right slots for `c` (12) and `elapsed`
  (16)** — removing the `trailMatrices` local is exactly what moves them into
  place. It fails for two *different* reasons: (a) `s` is still in a register,
  so nothing takes `[sp,#8]`, and the 4-byte stack-guard address moves from
  `[sp,#4]` to `[sp,#8]` (the frame bottom shifts by 4); (b) `dt` and `this`
  swap slots (20/24 instead of 24/20).

So the target is precisely:

> faithful form **+ `s` forced to spill** (which restores the guard to `[sp,#4]`
> and puts `s` at 8) **+ `this` spilled before `dt`** (so `this` gets 20 and
> `dt` 24).

---

## 3. Why `s` does not spill (register-allocator trace)

`ra_b.log` (self-built clang 7 with `-mllvm -debug-only=regalloc`) shows the
greedy allocation of the baseline:

```
selectOrSplit rGPR:%29 [16r,9648B:0)[9840B,15104B:0) 0@16r weight:9.400839e-04
  missed hint $r1 ; CEI: hard interference
  assigning %29 to $r4                       <-- elapsed first takes r4
selectOrSplit GPR:%28 [32r,9648B:0)[9840B,15104B:0) 0@32r weight:1.479491e-02
  assigning %28 to $r5                       <-- this first takes r5
selectOrSplit GPR:%247 ... weight:8.627993e-03
  assigning %247 to $r6                      <-- dt
selectOrSplit GPRnopc:%251 ... weight:8.566814e-03
  should evict: %29 ... w= 9.400839e-04
  evicting $r4 interference: Cascade 1
  unassigning %29 from $r4 ; assigning %251 to $r4
selectOrSplit rGPR:%29 ...
  assigning %29 to $r8
selectOrSplit rGPR:%386 ... weight:2.805497e-02   <-- loop cursor i*60
  evicting $r8 interference: Cascade 4
  unassigning %29 from $r8 ; assigning %386 to $r8
selectOrSplit rGPR:%29 ... RS_Split Cascade 4
  $r4 static = 0.0, total = 96.73  vs  $r10 static = 0.0, total = 8.52
  Split for $r10 ...
```

New identification this pass: **`%251` is `t2ADDri %stack.4.ref.tmp, 0`**, and
`%stack.4.ref.tmp` is the *shared* Vector temporary of
`velocities.data()[i] * dt * 0.5f`, used at source lines 300 (loop head),
319/320 (cluster `+=`) and 326 (trail block). Because stack-slot colouring
merged all four temporaries into one object, its address is materialised once in
the preheader (`bb.34.if.end236`, slot 9312) and stays live to slot 15104 — i.e.
across the entire loop. That is what evicts `elapsed` from `r4`:

```
9200B  %239:spr = COPY %29:rgpr          ; elapsed
9216B  %12:spr  = VSITOS %239:spr        ; dt
9232B  t2STRi12 %29:rgpr, %stack.12.ref.tmp47, 0   ; volatile `unused`
9248B  %13 = t2LDRi12 %28:gpr, 216        ; trailMatrices local
9296B  %247:gpr = VMOVRS %12:spr          ; dt
9312B  %251 = t2ADDri %stack.4.ref.tmp, 0 ; <-- live to 15104
9328B  %252 = t2ADDri %stack.3.barrel, 0
9360B  %255 = t2ADDri %stack.7.tmp, 0
9376B  %256 = t2ADDri %stack.1.ahead, 0
9440B  %302 = t2ADDri %stack.6.up, 0
9488B  %386 = t2MOVi 0                     ; loop cursor
```

The final code rematerialises these addresses (`add r6, sp, #116` etc. at every
use), so the long pre-RA live range is purely a machine-IR artefact — but it is
the artefact that decides the colouring.

`s` itself is `%284` (`COPY $r0` after the `Sinf` call), live from just after
`Sinf` to the first `+=`; the only thing that crosses that range is the `Cosf`
call. `r6` is dead there in our IR, so the greedy allocator takes `r6`. In the
original the same range must be blocked, which is consistent with the original
having a *different* `%stack.4` sharing pattern (the original recomputes every
temporary address inside the loop rather than hoisting one shared address).

---

## 4. Forcing registers does not fix the slots

Using the debug clang's `FORCEVREG` / `FORCEEVICT` hooks (vreg numbers from
`RA_NOHINT_CALL` lines; ARM physreg numbers R0=66 … R4=70, R5=71, R6=72,
R7=73, R8=74, R9=75, R10=76, R11=77, R12=78):

| experiment | score | result |
|---|---|---|
| `FORCEVREG=2147483677:70` (elapsed→r4) | 86.9 | entry still `mov r5,r1` |
| `FORCEVREG=2147483676:76` (this→r10) | 64.2 | — |
| `FORCEVREG=2147483895:70` (dt→r4) | 86.9 | — |
| `FORCEVREG=2147484037:70` (`%389`→r4) | 86.9 | — |
| `FORCEVREG=2147484074:70` (`%426`→r4) | 86.9 | — |
| **`FORCEEVICT=1 FORCEANYFN=1 FORCEVREG=2147484074:70`** | **88.3** | elapsed→r4, player→r5, but slots still 12/20 and cursor still `r9` |
| `VRW=2147483677:15` (elapsed weight ×15) | 76.6 | elapsed→r4, slot still `[sp,#12]` |
| `VRW=2147483677:10` | 78.6 | **635 insns**, elapsed still `r5` |

The 88.3 % run is the closest anything has come: it fixes `elapsed`→`r4` and
`player`→`r5` (the launch-block permutation) but leaves the frame slots and the
loop cursor untouched. This is the proof that register colouring and spill-slot
assignment are **independent levers** here, and that the slot layout is decided
by *spill-slot creation order*, which is in turn decided by which values the
inline spiller spills first.

Scheduler flags (all measured, none better): default 86.9;
`-pre-RA-sched=list-ilp` 66.5 (635 insns, 1912 B); `-pre-RA-sched=source` 50.3;
`-enable-misched=false` 86.9; `list-hybrid` / `list-burr` /
`misched-postra=false` 86.9.

---

## 5. Source variants tried this pass (all <= 86.9)

All measured with the official verify machinery against the pristine baseline
(scratch `/dev/shm/w4710`, `rebuild.sh` = touch + `ninja link_so` + `score.py`).

### 5.1 `s` / `c` / trig block

| variant | pct |
|---|---|
| baseline | **86.9** (636) |
| `float s` / `float c` / both non-const | 86.9 / 86.9 / 86.9 |
| `double s` / `double c` | 86.9 / 86.9 |
| `volatile float s` | 76.1 (637) |
| `volatile float c` | 83.8 (637) |
| both volatile | 76.4 (638) |
| `const float argS = ...; s = Sinf(argS)` | 86.9 |
| `const float argC = ...; c = Cosf(argC)` | 86.9 |
| both args hoisted before both calls | 53.2 (621) |
| `c` declared before `s` | 45.2 (630) |
| `float phase` (non-const) | 86.9 |

### 5.2 trail block / `trailMatrices` local

| variant | pct |
|---|---|
| local declared inside the loop body | 59.7 (631) |
| local declared inside the trail `if` | 55.5 (629) |
| local before loop, referenced only in trail block | 64.2 (633) |
| local = `Matrix *` (data pointer) in trail block | 55.9 (631) |
| local as `Array<Matrix> *const &` | 57.6 (626) |
| local as `Array<Matrix> *volatile` | 57.0 (628) |
| local as `Array<Matrix> &` (deref) | 64.2 (633) |
| local after `wasFiring` / before `hitSmall` / after `gun=` | 81.1 (635) each |
| local before the fade block | 70.0 (643) |
| `MatrixSetTranslation(trailMatrices->data()[i], ...)` | 85.3 |
| same, via `this->trailMatrices` | 63.5 (631) |
| `(*this->trailMatrices)[cp] = frame` → `data()[cp]` | 86.9 |
| `(*this->trailTimers)[i]` → `data()[i]` | 86.9 |
| `(*this->trailSystems)[i]` → `data()[i]` | 86.9 |
| all three `data()` rewrites | 85.3 |
| `Matrix &trail` hoisted above the first `MatrixSetTranslation` | 84.0 (637) |

### 5.3 `unused` / `padx` anchors

| variant | pct |
|---|---|
| `volatile unsigned unused` / `volatile long unused` | 86.9 / 86.9 |
| `volatile long long unused` | 75.9 (638) |
| `volatile double unused` | 75.4 (638) |
| `volatile float unused` | 84.6 (637) |
| `volatile char` / `volatile short` | 84.6 (637) |
| `volatile int *unused = &elapsed` | 85.8 |
| second `volatile int unused2 = elapsed` | 75.6 (638) |
| `unused` before `wasFiring` | 85.3 |
| `unused` at function top | 84.8 |
| `unused` inside the loop | 70.0 (633) |
| `unused` before the fade block | 85.0 |
| `(void) unused;` | 84.5 (638) |
| `if (unused == 0x7ffffff) return;` | 66.8 (640) |
| `padx` removed | 86.7 |

### 5.4 launch block

| variant | pct |
|---|---|
| `up` declared before `forward` | 85.9 |
| `scale` computed before `setPosition` | 85.8 |
| `scale = (1 - delayTimer/200)*255/255` | 86.9 |
| `barrel.z = barrel.z - 100.0f` | 86.9 |
| `muzzle = muzzle + MatrixRotateVector(...)` | 80.3 (640) |
| `const Matrix ship` (by value) | 71.7 (640) |
| inline `reinterpret_cast` at the call (no `ship` local) | 86.9 |
| `Player *pl = player->player;` | 86.9 |
| `if (hasGeometry) if (delayActive)` (nested) | 84.6 (**635**) |
| plain `&&` | 84.6 (635) |
| `__builtin_expect` on inner / outer condition | 84.6 (635) each |
| `__builtin_expect(..., 0)` | 70.3 |
| `__builtin_expect(hasGeometry, 0) && ...` | 70.0 (637) |

### 5.5 loop form

| variant | pct |
|---|---|
| `while (i < count) { ... ++i; }` | 86.9 |
| `for (size_t i ...)` / `for (int i ...)` / `for (unsigned i ...)` | 86.9 |
| `for (uint32_t i = 0; count > i; ++i)` | 86.7 |
| `for (uint32_t i = 0; i != count; ++i)` | 86.9 |

---

## 6. iOS 1.1.4 oracle (unchanged from pass 9, restated for completeness)

`_work/bins/ios_1.1.4_HD` (Mach-O armv7) has no symbol table, but the
`RocketGun` vtable is recoverable: name string at `0x3ee5f0`, typeinfo at
`0x445d48`, vtable at `0x445d14` with entries `0x1abc49` (dtor), `0x1abba5`
(deleting dtor), `0x183d4d`, `0x183d59`, `0x1ac0d9` (**update**), `0x1ac0cd`
(render), `0x1ac9cd` (translate), `0x1849ed` (setRadar), `0x106f9`
(isRocketGun). `RocketGun::update` is at `0x1ac0d8`. It is an older build
(different struct offsets, `wasFiring = gun->delayActive & 1`, `tst`-tested
bools, no `goto itemEmit`/`goto launchDone`); it is useful as a shape hint only,
and every plain/nested/bool form tested on Android regresses.

---

## 7. What remains

The residual is entirely a **spill-slot creation-order** problem, not a
register-colouring problem:

1. `s` must be spilled to `[sp,#8]`. Ours keeps it in `r6` because `r6` is free
   between the `Sinf` and `Cosf` calls.
2. That spill would push `c`→12, `elapsed`→16 and restore the stack-guard
   address to `[sp,#4]`.
3. `this` must be spilled before `dt` so it takes `[sp,#20]` and `dt` takes
   `[sp,#24]`; then `str r4,[sp,#16]` + `str sl,[sp,#20]` merge into
   `strd r4, sl, [sp,#16]` and the instruction count becomes 635.

The faithful form already achieves (2) — `c`→12, `elapsed`→16 — which is why it
is the right *source*, but it leaves `s` in `r4` and swaps `dt`/`this`
(20/24), so it scores 61.6.

The most plausible remaining lever is the **`%stack.4.ref.tmp` sharing**: the
original recomputes every `Vector` temporary address inside the loop, while our
IR hoists one shared address to the preheader and keeps it live across the whole
loop, evicting `elapsed` from `r4` and blocking the `s` spill. A source shape
that gives the three `velocities[i] * dt * 0.5f` temporaries distinct,
non-mergeable stack objects (so the address is not hoisted) would change the
`$r4` interference bundle set and is the one untried lever left.

---

## 8. Reproduction

```
# gate
cmake --build cmake-build-match --target check -j8

# side-by-side
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN9RocketGun6updateEi

# regalloc trace (scratch clang 7 with the debug hooks)
CCERR=/dev/shm/w4710/ra.log /dev/shm/w4710/ccenv3.sh -mllvm -debug-only=regalloc

# force a vreg into a physical register
FORCEEVICT=1 FORCEANYFN=1 FORCEVREG=2147484074:70 /dev/shm/w4710/ccenv3.sh
```

Scratch lives in `/dev/shm/w4710`: `base.cpp`, `faithful.cpp`, `rebuild.sh`,
`score.py`, `diffn.py`, `dump.py`, `ccenv3.sh`, `linkcmd.sh`, `batch.py`,
`vs*.py`, `muts.py`, `search.py`, `orig_fn.asm`, `ours_fn.asm`, `ra_b.log`,
`ios.asm`; none of it is in the repo.
