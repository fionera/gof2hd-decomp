# `Level::createScene` — wave-119 w6820: no source win over 92.7573 %

**Status:** dead-end sweep, 2026-11-05. Worktree `wt/w6820`, branch
`wave119-w6820`. Target `_ZN5Level11createSceneEv`. No source change landed;
this note records the measurements so the next pass does not repeat them.

Continuation of `level-createscene-wave119-w6709.md` (landed the `-g`
Vector-address trick, y/z/x store order and the 78 `rnd` mask, 90.7243 ->
92.7573 %) and `level-createscene-wave119-w6735.md` (dead-end sweep, same
ceiling). The wave-119 landed shape is still the unique maximum found.

## TL;DR

* Baseline and best: **92.7573 % / 2252 B**, 783/791 instructions, first
  diff at instruction 478 (parts-loop store schedule).
* The function is instruction-exact up to instruction 477. The only two
  differing regions are the parts-loop store schedule (478-486) and the
  ship-loop register allocation (548 onward).
* ~110 source variants were measured this session (all-global `rnd`,
  every 5-site `rnd` mask with the plain and constructor Vector forms,
  branch-order permutations, `Status`/`Globals`/`Canvas` locals, address
  trick spellings, type/qualifier/declaration-order sweeps, switch vs
  if-chain, redundant-store and dependency tricks). **None beat
  92.7573 %**; the closest non-landed shape is the `mesh` address trick at
  91.4975 % (same ship loop, earlier parts-loop diff).
* Gate re-measured green: **byte_exact 4505/4524, linked_exact 4505/4524,
  stub_zero_size 0, missing 0, wrong_type 0, extra 0, data_exact 843/843,
  imports 207, lint CLEAN, drift 0, RATCHET PASS.** 27 sections, file size
  2,187,328 B. **`.ARM.extab` is 0x26294** on this branch (wave-118 note
  said 0x26290; the wave-119 createShip work on master moved it +4, not a
  regression from this pass).

## 1. The two remaining diff regions (unchanged from w6735)

### 1.1 Parts loop (instructions 478-486)

Original (`g->setRotation(rot)` — one-instruction address, three separate
stores, zero materialised after the y store):

```
ldr   r0, [pc, #808]      ; pi
add   r1, sp, #56         ; &rot
str   r0, [sp, #60]       ; rot.y = pi
mov.w sl, #0              ; zero
mov   r0, r5              ; g
str.w sl, [sp, #56]       ; rot.x = 0
str.w sl, [sp, #64]       ; rot.z = 0
blx   setRotation
```

Ours (landed `-g` trick — two-instruction address, `strd` merge):

```
ldr   r0, [pc, #808]      ; pi
mov.w sl, #0              ; zero
str.w sl, [sp, #64]       ; rot.z = 0
strd  sl, r0, [sp, #56]   ; rot.x = 0, rot.y = pi (merged)
add   r0, sp, #56
subs  r1, r0, r5
mov   r0, r5
blx   setRotation
```

Both regions are 26 bytes. The `-g` trick is the only lever found that
forces the original's Basin-B prologue/parts-loop allocation; without it
the allocator moves the zero to `r4`, swaps Canvas/table-base (`r8`/`r9`)
and spills the parts-loop counter `k`.

### 1.2 Ship loop (instructions 548 onward)

Original:

```
ldr   r0, [pc, #632]      ; GOT
ldr   r1, [pc, #636]
add   r0, pc
add   r1, pc
ldr.w r8, [r0]            ; r8 = &Globals::rnd
ldr   r0, [sp, #52]       ; sceneRace
ldr.w r6, [r1, r0, lsl#2] ; capacity -> r6
ldr.w r0, [r8]            ; Globals::rnd
adds  r1, r6, #1
blx   nextInt
```

Ours:

```
ldr   r0, [pc, #632]
ldr   r1, [pc, #636]
add   r0, pc
add   r1, pc
ldr   r2, [r0, #0]        ; r2 = &Globals::rnd
ldr   r0, [sp, #52]
str   r2, [sp, #44]       ; spill &Globals::rnd
ldr.w fp, [r1, r0, lsl#2] ; capacity -> fp
ldr   r0, [r2, #0]        ; Globals::rnd
add.w r1, fp, #1
blx   nextInt
```

Register map in the ship loop:

| value | original | ours |
|---|---|---|
| `spotUsed` | r4 | r4 |
| `race` | r5 | r6 |
| `capacity` | r6 | fp |
| `&Globals::rnd` | **r8** | spilled `[sp,#44]` |
| `i` | r9 | r9 |
| `ship` / `status` | sl | sl |
| `type` | fp | r8 |
| `Globals::rnd` value | reloaded `[r8]` at every site | cached in r5 |

The original uses `Globals::rnd->` at **all eight** sites (the GOT-derived
address is loop-invariant and hoisted into r8; the pointer value is
reloaded at each use). Our source uses a local
`AERandom *rnd = Globals::rnd;` for three sites and `Globals::rnd->` for
the other five, so the compiler caches the pointer value in r5 and spills
the address. That is one extra live value, and the allocator resolves it by
spilling the address and giving r8 to `type`.

The central basin conflict (confirmed again this session):

* **Local-value basin (landed, 92.7573 %):** `Globals::status` in `sl`,
  Canvas in `r8`, table base in `r9`, parts-loop zero in `sl`; ship loop
  caches `rnd` in r5 and spills the address.
* **All-global basin (75.4286 %, 2232 B):** removing the local `rnd`
  entirely gives the original's `ldr rX,[addr]` ship-loop shape, but the
  whole function collapses to a different allocation: `Globals::status` in
  `r4`, `this` kept in `sl`, `&Globals::rnd` in `r9`, `capacity` in `r8`,
  `i` spilled. The parts-loop preheader (`r8`/`r9` swap) also changes.

No source shape found this session bridges the two: the local value must
exist to pin the prologue/parts-loop basin, but its existence is exactly
what pushes the address out of r8.

## 2. Measurements (all 2252 B unless noted)

Scored with the official single-function verifier (unrounded difflib
ratio). `pct` is the `--show` structural ratio; `firstdiff` is the first
differing normalized instruction index.

### 2.1 `rnd` mask sweeps

The five variable sites are B = `if (rnd->nextInt(100) <= 29)`,
C = `race = rnd->nextInt(4)`, D = `if (Globals::rnd->nextInt(100) < 30)`,
E = `int pick = Globals::rnd->nextInt(3)`,
G = `spot = rnd->nextInt(capacity)` (retry loop). The landed mask is
B,C,G local (5-site mask 19). Full 32-mask sweep with the landed
y/z/x + `-g` shape:

| mask | sites local | pct |
|---|---|---|
| 19, 23, 27, 31 | B,C,G / +D / +E / all five | **92.7573** (identical code) |
| 18, 22, 26, 30 | C,G ... | 88.3100 |
| 17, 21, 25, 29 | B,G ... | 86.7302 |
| 16, 20, 24, 28 | G only | 66.5404 (2236 B) |
| 0–15 | G global | 66.19–75.43 (2232 B) |

The retry-loop site G **must** be local; masks without it collapse to the
all-global basin. Masks 19/23/27/31 compile to byte-identical code.

Plain `g->setRotation(rot)` (no `-g` trick) sweep: best 85.4061 % (mask
18/22/26/30), landed mask 19 = 84.8793 % (2248 B). Constructor
`Vector(0, pi, 0)` sweep: best 85.4788 % (mask 18/22/26/30), landed mask
19 = 84.9524 % (2248 B).

### 2.2 Vector construction with the landed `-g` trick

| variant | pct | firstdiff |
|---|---|---|
| **y, z, x field assignment (landed)** | **92.7573** | 478 |
| y, x, z | 92.6302 | 477 |
| x, z, y | 92.6302 | 477 |
| z, x, y | 92.6302 | 477 |
| `rot.y = pi; rot.x = rot.z = 0;` | 92.7573 | 478 |
| `rot.y = pi; rot.z = 0; rot.x = rot.z;` | 92.7573 | 478 |
| `Vector rot(0, pi, 0); rot.x = 0;` (redundant store) | 92.7573 | 478 |
| x, y, z | 88.3100 | 28 |
| z, y, x | 88.1830 | 28 |
| `Vector rot; rot.y = pi;` (default ctor zeroes) | 87.9288 | 28 |
| `Vector rot(0, pi, 0); g->setRotation(rot)` | 88.3100 | 28 |
| `g->setRotation(Vector(0, pi, 0))` temporary | 84.9524 | 28 |
| `float *fp = (float *)&rot; fp[1]=pi; ...` | 92.7573 | 478 |
| `char *cp = (char *)&rot; *(float*)(cp+4)=pi; ...` | 92.7573 | 478 |
| `float rot[3]` | 84.8793 | 28 |
| `rot` declared before the `for` loop | 84.2839 (2264 B) | — |
| `rot` declared at function scope | 78.9677 (2180 B) | — |

Dependency tricks to force the original store order
(`rot.x = rot.y - pi`, `rot.x = rot.y * 0`, `float zero = 0; rot.x = zero`,
`const float zero`, `rot.x = rot.z`) all score 92.6302 (firstdiff 477);
`volatile float zero` collapses to 80.2790.

### 2.3 Address-trick spellings

| trick | pct | firstdiff |
|---|---|---|
| **`- (size_t) g` (landed)** | **92.7573** | 478 |
| `- (size_t) mesh` | 91.4975 | 456 |
| `- ((size_t) g >> 1)` | 88.1119 | 28 |
| `- ((size_t) g * 2)` | 88.1119 | 28 |
| `- ((size_t) g + 1)` | 88.0559 | 28 |
| `- ((size_t) g & 1)` / `| 1` | 87.7460 | 28 |
| `- (size_t) k` | 86.4590 | 28 |
| `- (size_t) this` | 86.8020 | 28 |
| `- (size_t) sceneRace` | 86.5482 | 28 |
| `- 1` / `- 4` / `- 12` / `+ 4` | 86.5482 | 28 |
| `+ 0` / `- (g == nullptr)` / `- g*0` | 84.8793 | 28 |
| `- (size_t) this->enemies` | 78.2719 | 5 |

Only the exact `- (size_t) g` form keeps the Basin-B allocation.

### 2.4 Ship-loop `rnd` local-vs-global and declaration position

| variant | pct |
|---|---|
| **landed mask (B,C,G local, after fill loop)** | **92.7573** |
| all global (no local `rnd`) | 75.4286 (2232 B) |
| `AERandom *&rnd = Globals::rnd` (reference) | 75.4286 (2232 B) |
| `AERandom *const &rnd` | 75.4286 (2232 B) |
| `AERandom **rndp = &Globals::rnd` (mixed) | 92.7573 |
| `volatile AERandom *const rnd` | 77.5949 (2240 B) |
| `static AERandom *rnd` | 72.1908 (2272 B) |
| `rnd` at branch top | 63.6652 (2204 B) |
| `rnd` before parts loop | 69.9177 (2224 B) |
| `rnd` before ownHangar | 71.2746 (2220 B) |
| `rnd` before capacity | 70.7595 (2236 B) |
| `rnd` before `new unsigned char[capacity]` | 76.6011 (2228 B) |
| `rnd` at loop top (inside body) | 91.4867 |
| `rnd` at function scope | 60.6369 (2204 B) |
| `rnd` at branch scope | 63.6652 (2204 B) |
| all 8 sites local (declaration moved before shipCount) | 74.3475 (2240 B) |
| `const` / `auto` / `register` local | 92.7573 |
| `Globals::rnd = rnd;` redundant store before loop | 92.7573 |

### 2.5 Variable types

| variant | pct |
|---|---|
| `unsigned`/`long` `shipCount`, `i`, `spot`, `tries`, `race`, `type` | 92.7573 |
| `long` `capacity` | 92.7573 |
| `unsigned int capacity` | 92.6302 |
| `unsigned int sceneRace` | 92.5032 (firstdiff 452, `it hi` vs `it gt`) |
| `short sceneRace` | 84.2640 |
| `bool *spotUsed` | 92.7573 |
| `int ownHangar` / `int plainHangar` | 92.7573 |

### 2.6 Control flow / statement order

| variant | pct |
|---|---|
| branch order 23,4,2 (source) / 23,2,4 | 92.7573 |
| branch order 2,4,23 | 90.3431 |
| branch order 2,23,4 | 70.6776 (2248 B) |
| branch order 4,23,2 / 4,2,23 | 61.8413 (2256 B) |
| `switch (missionPtr)` instead of if-chain | 92.7573 (identical code) |
| `pick` if/else instead of ternary | 92.7573 |
| `race` if/else instead of `race = sceneRace` | 92.7573 |
| retry loop `spotUsed[spot]` / `tries > 0` | 92.7573 |
| `spot` declared then assigned | 92.7573 |
| `ownHangar` ternary → if/else | 92.7573 |
| `plainHangar = !(sceneRace < 4 && sceneRace != 1)` / `>= 4` | 92.7573 |
| `if (mesh >= 0) { ... }` instead of `if (mesh < 0) continue` | 92.7573 |
| switch default first | 92.7573 |
| switch → if/else chain | 75.7269 (2268 B) |
| `spotUsed[spot] = 1` after the switch | 92.3761 |
| switch before the retry loop | 74.6835 (2268 B) |
| `type = pick == 0 ? 37 : ...` → if/else | 92.7573 |
| `height = kShipHangarHeights[type]` hoisted | 92.2490 |

### 2.7 Other locals / hoists

| variant | pct |
|---|---|
| `Status *status = Globals::status` for whole branch | 74.4275 (2228 B) |
| `Status *status` only before the parts loop | 81.4204 (2252 B) |
| `Status *status` only in the ship loop | 74.9206 / 71.1027 |
| `Globals *globals = Globals::globals` | 78.0178 (2248 B) |
| `PaintCanvas *canvas` hoisted (base / ctor Vector) | 64.88 / 64.97 (2212/2208 B) |
| `PaintCanvas *canvas` after plainHangar | 84.0635 / 82.7192 |
| `AERandom *rnd4` in the mission-4 branch | 67.0910 (2224 B) |

## 3. MIR evidence

`-mllvm -print-machineinstrs` on the landed source (dump in
`/dev/shm/w6820/mir_base.txt`, function section 1108473-1209800; the
pre-RA section is 1156207-1158051, post-RA 1158051+):

* Instruction selection already orders the parts-loop stores **z, y, x**
  (source order is y, z, x). The y/x merge into `strd` happens later in
  the ARM load/store optimizer, after the zero is materialised early.
* The pre-scheduler address computation is `t2ADDri %stack.1, 0`
  followed by `t2SUBrr %545, %10` (the `-g` trick). The post-RA sink moves
  the `add` down to just before the `subs`.
* The greedy allocator assigns `capacity -> fp`, `rnd -> r5`,
  `race -> r6`, `type -> r8` in the landed source. The original's
  `capacity -> r6`, `&rnd -> r8`, `race -> r5`, `type -> fp` is a cyclic
  permutation of the same four registers.
* The all-global source assigns `&rnd -> r9`, `capacity -> r8`,
  `i -> r6`, `this -> sl`, and spills `i` across the retry loop. The
  original spills `this` and keeps `i` in r9. The `this`-vs-`i` spill
  choice is the first divergence: the original's `sl` holds
  `Globals::status` at the ship-loop entry (restored at `b48f2`), while
  the all-global build loads `this` into `sl` at the parts-loop epilogue.
* A per-file `-mllvm -sink-insts-to-avoid-spills` override was also tried
  (temporarily, then reverted): it is neutral for the landed source
  (92.7573) and does not fix the all-global ship loop (75.9365, 2232 B).

## 4. Conclusion

The landed wave-119 shape is a genuine local maximum for this compiler at
`-Oz`. The `-g` trick is the only lever found that forces the original's
Basin-B prologue/parts-loop allocation, and the local `rnd` value is the
only lever found that keeps it. The original's ship loop needs
`&Globals::rnd` in r8, which the compiler only produces in the all-global
basin where `Globals::status` moves to r4 and `this` takes sl.

The next pass should look for the source feature that pins the prologue
basin **independently of the `rnd` local** — a witnessed original local
that keeps `Globals::status` in `sl` and `this` spilled without adding a
ship-loop live value. The all-global build is the semantically correct
source; the only missing piece is that early-allocation pin.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6820
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 tools/verify/verify.py --original \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so --show _ZN5Level11createSceneEv
```

Scratch harness (not committed): `/dev/shm/w6820/` — `Level.base.cpp`
(the landed source), `score.py` / `score2.py` (official verifier import,
unrounded ratio + first-diff index), `try.py`, `sweep5.py`,
`sweep_ctor.py`, `sweep_plain.py`, `v_*.py` (this session's variant
batches), `mir_base.txt` (full `-print-machineinstrs` dump),
`orig.asm` / `ours.asm` / `show*.txt` (resolved disassembly).
