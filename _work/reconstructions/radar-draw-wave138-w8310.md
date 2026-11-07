# Radar::draw wave 138 (w8310) — status-reload sink: the else-block-6B target needs a bool guard (no source win)

**Status:** research note, 2026-11-07. Worktree `wt/w8310`, based on `master`
(`96b069792`). **No source change landed**; this commit contains this note only.
The target remains the pristine baseline `src/game/weapons/Radar.cpp`
(`sha256 9b315de8bf294d1f5bbc303ef37551b6d828598d57ea6c4a121882bba684fbd3`).

Target `_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).
Original 9908 B / 3492 normalised instructions; ours 9908 B / 3483.
Baseline official fuzzy **75.4** (`tools/verify/verify.py`, exact `75.4000`).
Gate at start and end: `RATCHET PASS`, byte **4507**, linked **4507**,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0.

Read with `radar-draw-wave137-w8212.md`, `radar-draw-wave136-w8110.md`,
`radar-draw-wave135-w7774.md`.

This wave attacked the wave-137 open question directly: *can a source shape make
the guard else block 6 B (the status GOT reload sunk out of it) while keeping
`int otherLock` in `r9` and the function at 9908 B?* The answer is **not by a
size-neutral source edit**: the reload placement is a `SpillPlacement` /
Greedy-RA fixed point, and every shape that moves the reload to `bb.37` also
changes the register allocation of the route section (or the tail) and loses
60–96 B. Two new data points pin the mechanism.

## 1. Method / scratch

```sh
cd /home/debian/GalaxyOnFire2/wt/w8310
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6     # ~1 s per variant
cmake --build cmake-build-match --target check -j6        # RATCHET PASS
```

Scratch in `/dev/shm/w8310` (not committed): `Radar.base.cpp` (pristine source),
`score.py` (single-symbol scorer importing `tools/verify/verify.py`, reproduces
75.4000 exactly), `reg.py` (side-by-side normalised diff), `cc.sh` (direct
clang-7 compile with the build flags), `all.txt` / `all_bool.txt` /
`all_exp1.txt` (`-mllvm -print-after-all -mllvm -filter-print-funcs=...`,
1.0 M lines each), `gen*.py` (variant generators), `ours.dis`/`orig.dis`.

## 2. Baseline MIR: the reload is in bb.29 (else) and bb.30 (check B)

`-print-after-all` on the pristine baseline, Greedy Register Allocator dump.
The spilled `Globals::status` GOT pointer is `%2600` / `%stack.31`. The guard:

```
bb.28:  %568 = t2LDR_PRE %this, 12        ; lockedAsteroid
        t2CMPri; t2Bcc bb.30, 0           ; cbz -> check B
bb.29:  %3028 = t2MOVi 1                  ; else: otherLock = 1
        %2600 = t2LDRi12 %stack.31, 0     ; <-- status GOT reload
        t2B bb.35
bb.30:  %571 = t2LDRi12 %this, 56         ; lockedGasCloud
        t2CMPri
        %2600 = t2LDRi12 %stack.31, 0     ; <-- status GOT reload
        t2Bcc bb.32, 0
bb.31:  %3028 = t2MOVi 1
        t2B bb.35
bb.32:  %574 = t2LDRi12 %this, 28         ; field_0x1c
        t2CMPri; t2Bcc bb.34, 0
bb.33:  %3028 = t2MOVi 1
        t2B bb.35
bb.34:  %3028 = t2LDRi12 %this, 20        ; otherLock = field_0x14
        t2CMPri; t2MOVCCi %3028, 1, 1
bb.35:  %582 = t2LDRi12 %this, 312        ; landmarks
        t2CMPri; t2Bcc bb.37, 1
bb.37:  %19 = t2LDRi12 %2600, 0           ; *status
```

So `%2600` is live-in to the join `bb.35`; `SpillPlacement` picks the two
dominator blocks `bb.29` and `bb.30` as reload points. The else block is
`mov.w r9,#1` + `ldr r5,[sp,#156]` + `b.n` = **10 B**. `otherLock` is `r9`,
the GOT is `r5`.

The original instead reloads in `bb.37` (`ldr r6,[sp,#156]` at `0x12e31a`,
after the `landmarks` `cbz`), keeps `otherLock` in `r5` (`movs r5,#1`, 2 B),
and materialises `notOtherLock` (`eor r0,r5,#1; str r0,[sp,#116]`) because the
GOT load clobbers `r5`. The three facts are one chain.

## 3. NEW: the exact original guard is reachable, but only with a bool guard

`bool otherLock` (wave-137 `bool_ol`) reproduces the original guard byte-for-byte
(`cbnz` chain, `movs r5,#1`, `ldr r6,[sp,#156]` after the join, `eor` for
`notOtherLock`) but is **9812 B / 69.1** — the bool type re-rolls the planet
and enemy tails (96 B loss, wave 131/137).

**New this wave:** a *separate* bool for the guard while the rest of the
function keeps `int otherLock`:

```cpp
bool otherLockGuard;
if (__builtin_expect(...)) otherLockGuard = this->field_0x14 != 0;
else                       otherLockGuard = 1;
...
int notOtherLock = !otherLockGuard;
...
int otherLock;                       // original int, assigned later as before
otherLock = this->lockedAsteroid != nullptr || ...;
```

This (`v_guardbool`) produces the **exact original guard region**:

```
12e310 mov r1, r4
12e312 ldr.w r0, [r1, #12]!
12e316 str r1, [sp, #124]
12e318 cbnz r0, 12e326
12e31a ldr r0, [r4, #56]
12e31c cbnz r0, 12e326
12e31e ldr r0, [r4, #28]
12e320 cmp r0, #0
12e322 beq.w 1305c6
12e326 movs r5, #1
12e328 ldr.w r0, [r4, #312]
12e32c cbz r0, 12e36a
12e32e ldr r6, [sp, #156]
12e330 ldr r0, [r6, #0]
```

but the function is **9848 B / 72.6** (60 B short). The 60 B is *before* the
guard: with `r9` freed (the guard's `otherLock` is now `r5`), the route
section's `DrawString` argument moves from `r6` to `r9`
(`strd r5,r9,[sp]` vs the original's `mov r2,r6`), a 38-instruction diff at
orig index 864. Keeping a dead `int otherLock` around does not re-occupy `r9`
(`v_guardbool2`, `v_guardbool3` are identical 9848 B).

## 4. NEW: `Globals::status != nullptr` before the landmark check moves the GOT to r6

Adding a status null test *before* the landmark check:

```cpp
if (Globals::status != nullptr && this->landmarks != nullptr &&
    !(Globals::status->...)) { ... }
```

(`v_gsfirst`) is the only probe that reproduces the original's **register
assignment** with `int otherLock`: `otherLock` stays in `r9` and the status GOT
moves to `r6` (`ldr r6,[sp,#156]` in both guard predecessors). But the status
check is then evaluated before the landmark check, so the layout is wrong
(`ldr r0,[r6,#0]; cbz r0, skip; ldr landmarks; cbz`) and the score is
**9888 B / 70.2**. The reload is still in `bb.29`/`bb.30`, so the else block is
still 10 B.

This is the new lever: the GOT register (`r5` vs `r6`) and the reload block
(`bb.29/30` vs `bb.37`) are *independent*. `v_gsfirst` shows `r6` is reachable
with `r9`; the missing piece is still the reload block.

## 5. Variant matrix (all single edits on the pristine baseline)

| variant | what | usize | raw % |
|---|---|---:|---:|
| **baseline** | — | **9908** | **75.4000** |
| `bool_ol` | `bool otherLock` (wave 137) | 9812 | 69.1000 |
| `v_guardbool` | separate `bool otherLockGuard` + int `otherLock` for the rest | 9848 | 72.6000 |
| `v_guardbool2` | same, `int otherLock` declared before the guard | 9848 | 72.6000 |
| `v_guardbool3` | same, int `otherLock` also assigned in the guard | 9848 | 72.6000 |
| `v_gsfirst` | `Globals::status != nullptr` before the landmark check | 9888 | 70.2000 |
| `v_gsnull` | `landmarks != nullptr && Globals::status != nullptr && ...` | 9908 | 72.4000 |
| `v_stnull` | `landmarks != nullptr && Globals::status->getStation() != nullptr && ...` | 9860 | 71.6000 |
| `v_statusok` | `bool statusOk = Globals::status != nullptr;` before the guard | 9908 | 72.0000 |
| `v_statusok2` | `Status *status` + `bool statusOk` before the guard | 9912 | 71.2000 |
| `v_comma` | `(Globals::status, landmarks != nullptr)` | 9908 | 75.4000 (tie) |
| `v_comma2` | `landmarks != nullptr && (Globals::status != nullptr, true)` | 9908 | 75.4000 (tie) |
| `v_sbefore` | `Status *status` after the guard, used in the landmark cond | 9908 | 75.1000 |
| `v_sboth` | `Status *status` assigned in both guard branches | 9908 | 75.1000 |
| `v_lmlast` | status checks before the landmark null check | 9916 | 71.0000 |
| `v_exp1` | outer `expect(..., 1)` | 9872 | 73.4000 |
| `v_pos0` | positive `\|\|`, `expect(..., 0)`, swapped bodies | 9872 | 73.4000 |
| `v_neg0` | negated `&&`, `expect(..., 0)`, swapped bodies | 9872 | 73.4000 |
| `v_noexp` | no expect at all | 9908 | 74.2000 |
| `v_innernone` | drop the inner `expect(lockedGasCloud, 1)` | 9908 | 75.3000 |
| `v_inner0` | inner `expect(lockedGasCloud, 0)` | 9904 | 71.9000 |
| `v_init1` | `int otherLock = 1; if (cond) otherLock = ...` | 9908 | 75.4000 (tie) |
| `v_lmlocal` | `Array<KIPlayer*> *lm = this->landmarks` used everywhere | 9892 | 70.9000 |
| `v_lmlocal2` | same, `this->landmarks` kept in the loop | 9908 | 75.4000 (tie) |
| `v_enlocal` | `enemies` local before the guard | 9908 | 75.4000 (tie) |
| `v_nolbool` | `bool notOtherLock` | 9920 | 72.0000 |
| `v_nol_xor/sub/cmp` | `(otherLock^1)&1` / `1-otherLock` / `otherLock==0` | 9908 | 75.4000 (tie) |
| `v_ol_tern` | `int otherLock = cond ? ... : 1` | 9908 | 75.4000 (tie) |
| `v_decl_nol_in` | `notOtherLock` declared inside the landmark block | 9900 | 73.6000 |
| `v_uint/long/short/schar/uchar` | otherLock type variants | 9908 | 75.4000 (tie) |

All int-like types (`unsigned`, `long`, `short`, `signed char`, `unsigned char`)
compile identically to `int` (integer promotion); only `bool` changes the RA.

## 6. Root cause

The residual is the same coupled `SpillPlacement` / Greedy-RA / `StackColoring`
fixed point as waves 134–137, now with the exact decoupling:

1. `%2600` (status GOT) is live-in to the guard join `bb.35`, so
   `SpillPlacement` chooses `bb.29` (else) and `bb.30` (check B) as reload
   points. The else block is 10 B. Moving the reload to `bb.37` requires the
   value to be live-in to `bb.37` only, which the source cannot force.
2. The only shape that moves the reload to `bb.37` is a bool guard
   (`bool otherLock` / `bool otherLockGuard`), which puts `otherLock` in `r5`
   and frees `r9`. The freed `r9` is immediately reused by the route section's
   `DrawString` argument, costing 60 B (or 96 B for the full bool type).
3. `Globals::status != nullptr` before the landmark check is a new, independent
   lever that moves the GOT to `r6` while keeping `otherLock` in `r9`, but it
   also moves the *evaluation* of the status check before the landmark check
   (wrong layout, 70.2).

The mission's target (else block 6 B = `mov.w r9,#1` + `b.n`, status reload at
the join, 9908 B) is therefore not reachable by a size-neutral source edit on
this baseline: the reload block and the `r9` occupancy are one fixed point.

## 7. Gate / section deltas (baseline, unchanged)

```
compared 4524  avg 99.94%  byte_exact 4507  linked_exact 4507  fuzzy_perfect 4507
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Section deltas vs the original: `.text` **85,535** differing bytes (size/base
exact), `.ARM.extab` **1,395** (size exact 156,312), `.rel.plt` 0
(byte-identical), `.note.gnu.build-id` 20. `.rodata`, `.data`, `.bss`, `.got`,
`.ARM.exidx`, `.rel.dyn`, `.dynsym`, `.dynstr`, `.hash` are byte-identical.
