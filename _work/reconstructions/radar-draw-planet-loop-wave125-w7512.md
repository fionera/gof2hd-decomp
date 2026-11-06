# Radar::draw pass (wave 125, w7512) — planet-loop encoding sweep, no source win

**Status:** research note, 2026-11-06. Worktree `wt/w7512`, based on current
`master` (`02513de33`, wave-124 final). **No source change landed**; this
commit contains this note only. The target remains the pristine baseline
`src/game/weapons/Radar.cpp`.

Target `_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).
Original 9908 B / 3492 normalised instructions; ours 9908 B / 3483.
Baseline unrounded `difflib.SequenceMatcher(None, on, un, autojunk=False).ratio()`
= **75.268817 %** (official `verify.py` display 75.3), `M = 2625` matching
instructions. Gate at start and end: `RATCHET PASS`, byte **4506**, linked
**4506**, data 843/843, imports 207, lint CLEAN, drift 0, symbol parity
5370/5370, avg 99.93.

Read with `radar-draw-wave124-w7413.md`, `radar-draw-wave124-w7412.md`,
`radar-draw-wave123-w7307.md`, `radar-draw-wave122-w7209.md`,
`radar-draw-body-wave122-w7210.md`, `radar-draw-wave119-w6967.md`. This wave
re-measures the planet-loop residual at instruction level and tests ~350
source variants aimed at the four missing instructions. The conclusion is
unchanged: the loop is a tuned local optimum, and every spelling that adds the
missing instructions re-rolls the loop's register allocation.

## 1. Commands

```sh
cd /home/debian/GalaxyOnFire2/wt/w7512
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6        # RATCHET PASS
cmake --build cmake-build-match --target diff -j6         # SYMBOL PARITY
```

Scratch harness `/dev/shm/w7512/score.py` imports `tools/verify/verify.py` and
reproduces the official unrounded ratio exactly. Per-variant loop: edit
`Radar.cpp`, `cmake --build cmake-build-match --target link_so -j6` (≈1 s),
re-score. Section deltas measured with `tools/verify/elf.py`.

## 2. What "196 vs 192" actually is

The assignment's planet-loop body is the region from the loop head to the
`DrawString` block:

| region | original | ours |
|---|---:|---:|
| planet-loop head → DrawString (`12e3d4`→`12e5dc` / `12e5c6`) | 196 insns / 0x208 B | **192 insns / 0x1f2 B** |
| planet-loop head → recompute (`12e3d4`→`12e6ee` / `12e6d2`) | 297 insns / 794 B | 289 insns / 766 B |
| recompute (`12e6ee`→`12e746` / `12e6d2`→`12e72a`) | 88 B | 88 B (identical code) |
| planet-lock tail (`12e748`→`12e798` / `12e72c`→`12e7c2`) | 80 B | 76 B |
| landmark setup (`12e7a8`→`12e7d4` / `12e7c4`→`12e7f4`) | 44 B | 48 B |
| landmark body (`12e7d4`→`12eb42` / `12e800`→`12eb44`) | 878 B | 836 B |

So the loop body is 4 instructions / 0x16 B shorter in the head→DrawString
window and 8 instructions / 0x1c B shorter head→recompute. The literal pools:

| pool | original | ours |
|---|---:|---:|
| first (5 words) | `12e340` | `12e310` |
| second (5 words) | `12e79c` (before landmark setup) | `12e7f4` (after landmark setup) |

The second pool is after the setup because **ours' landmark setup ends in a
water block** (`12e7f2 b.n 12eb48`, the rotated loop condition), while the
original's setup falls straight through into the body (`12e7d4`). The pool is
therefore placed after the setup's `b.n`, not after the planet-lock tail's
`b.n` (`12e798`). Adding bytes to the planet loop pushes the setup later but
does not un-rotate the landmark loop, so the pool stays after the setup.

## 3. The four missing instructions (instruction-level diff)

Aligned normalised diff of `12e3d4`→`12e5dc` (original) vs `12e3d4`→`12e5c6`
(ours):

1. **Campaign `isTarget` rematerialises `stationId`** (2 insns, 8 B).
   Original `12e474`:
   ```
   ldr.w r0, [sl, #4]        ; stations->data()
   ldr.w sl, [r0, fp, lsl #2] ; sl = stations->data()[i]
   ...
   cmp   sl, r0              ; stationId == getTargetStation()
   ldr.w sl, [sp, #116]      ; restore sl = stations
   ```
   Ours keeps `stationId` in `r8` and compares `r8` directly (no reload).
   The compiler chose to rematerialise the load in the original because
   `stations` lives in `sl` and is spilled to `[sp,#116]`; ours keeps a second
   copy of `stations` in `r6` and never spills it.

2. **`marked` bool canonicalisation** (4 insns, 8 B).
   Original `12e67a`, on the `==120`-fail path just before the `==125` test:
   ```
   ldr   r1, [sp, #132]      ; marked
   cmp   r1, #0
   it    ne
   movne r1, #1
   cmp   r0, #125
   str   r1, [sp, #132]      ; marked = marked != 0
   ```
   Ours has no reload/normalisation: `marked` stays in a register across the
   `getCurrentCampaignMission()` call. The store is a word store in both
   binaries, so `marked` is `int`; the normalisation is a bool-canonicalisation
   artifact of the original's higher register pressure.

3. **Three separate bool loads vs `ldrd`** (1 insn, 2 B).
   Original `12e4de`: `ldr r0,[sp,#144]; ldr r1,[sp,#152]; orrs; ldr
   r1,[sp,#148]; orrs; lsls; beq hot`.
   Ours `12e4da`: `ldrd r0,r1,[sp,#144]; orrs; ldr r1,[sp,#152]; orrs; lsls;
   bne cold`. Ours' `planetHit`/`turretMode` stack slots are swapped relative
   to the original (`[sp,#148]`/`[sp,#152]`), so the compiler pairs
   `otherLock`+`planetHit` in one `ldrd`.

4. **Cold `sl`-restore tails** (3 insns, 6 B).
   Original has `12e5fa ldr.w sl,[sp,#116]; b.n 12e4de` and
   `12e600 ldr.w sl,[sp,#116]; b.n 12e620` (and `12e6e4`/`12e6ea` variants)
   after the campaign/freelance reloads. Ours has no reload, hence no restore
   tails.

Ours has compensating extras: six `mov r6, sl` copies of the `stations`
pointer, a `b.n` before the cold hit block, and `mov`/`b.n` shuffles around
the freelance compare. Net: −4 instructions / −0x16 B.

## 4. Variants measured this wave

All rows are single or combined edits on the pristine baseline. `usize` is
emitted function size; `raw %` is the unrounded ratio.

| variant | usize | raw % |
|---|---:|---:|
| **baseline** | 9908 | **75.268817** |
| campaign `isTarget` → `stations->data()[i]` (`camp_reload`) | 9904 | 73.4442 |
| campaign + freelance both `stations->data()[i]` | 9904 | 73.4442 |
| freelance `isTarget` → `stationId` (`free_local`) | 9892 | 73.7627 |
| `stationId` removed, `stations->data()[i]` everywhere | 9948 | 72.2850 |
| `camp_reload` + `bool marked` | 9932 | 72.9017 |
| `camp_reload` + `int stationId` last | 9908 | 74.8961 |
| `camp_reload` + `currentStation` first | 9904 | 73.4729 |
| `camp_reload` + `const Array<int> *stations` | 9904 | 73.4442 |
| `camp_reload` + `const int stationId` | 9904 | 73.4442 |
| `camp_reload` + `__builtin_expect(...,1/0)` | 9904 | 73.4442 |
| `camp_reload` + `KIPlayer *planet` local | 9904 | 73.4442 |
| `camp_reload` + `Status *status` loop local | 9904 | 73.4442 |
| `bool marked` alone | 9916 | 72.6647 |
| `bool marked` + `KIPlayer *planet` / `Status *status` | 9916 | 72.6647 |
| `bool marked` + `camp_reload` | 9932 | 72.9017 |
| `int marked` → `char`/`short`/`unsigned`/`long`/`int32_t`/`uint32_t` | 9908 | 75.2688 (tie) |
| explicit `marked = marked != 0` (nested else) | 9908 | 75.2688 (folds) |
| explicit `marked = marked ? 1 : 0` / `(int)(bool)marked` / `!!marked` / `&1` | 9908 | 75.2688 (folds) |
| `marked == 0` / `!marked` added to the `==125` condition | 9976 | 71.3509 |
| `int planetHit` | 9896 | 72.8446 |
| `bool otherLock` | 9812 | 69.0778 |
| `otherLock` → `unsigned`/`char`/`short`/`long`/`int32_t`/`uint32_t`/`uint8_t` | 9908 | 75.2688 (tie) |
| `notOtherLock`/`notTurretMode` → `unsigned`/`char`/`uint8_t` | 9908 | 75.2688 (tie) |
| `notOtherLock`/`notTurretMode` → `bool` | 9920 | 71.9346 |
| `Status *status` in landmark guard | 9912 | 73.3266 |
| `Status *status` in planet loop only | 9908 | 75.2688 (optimised away) |
| `KIPlayer *planet` loop local | 9908 | 75.2688 (tie) |
| `Array<KIPlayer *> *targets` loop local | 9904 | 72.6673 |
| planet loop `int i` / `unsigned i` | 9908 | 75.2688 (tie) |
| planet loop `size` cached in the `for` init | 9908 | 74.1080 |
| `warpGate` inlined into the condition | 9900 | 73.3974 |
| `currentStation` inlined | 10220 | 51.2319 |
| `!planetHit && !otherLock && !turretMode` / `!(a||b||c)` forms | 9908 | 75.2688 (tie) |
| `!otherLock && !turretMode && !planetHit` (and 3 more orders) | 9844 | 58.5443 |
| `bool canHit` / `int canHit` local | 9836 | 60.9647 |
| `(otherLock \| planetHit \| turretMode) == 0` | 9908 | 75.2688 (tie) |
| nested `if`/`else` mission chain | 9908 | 75.2688 (tie) |
| `(int)i == warpGate` → `i == (uint32_t)warpGate` | 9908 | 75.2688 (tie) |
| `(int)i == orbitPlanetIndex` → `i == (uint32_t)…` | 9908 | 75.2688 (tie) |

Systematic searches:

* **All 24 declaration permutations** of `{stationId, currentStation,
  warpGate, textOffset}`, each with and without `camp_reload` (48 builds).
  Best 9908-B result is the baseline order at **75.268817**; every
  `camp_reload` permutation is ≤ 74.8961 and/or 9904 B.
* **128-combination grid** over `{camp_reload, freelance reload, bool marked,
  stationId position, warpGate cast, orbit-index cast}`. Best 9908-B result is
  **75.268817** (baseline); all `camp_reload` combinations ≤ 74.8961.
* **120 random 2–6-edit combinations** of the above plus `Status *status`,
  `int planetHit`, condition orders. No combination exceeded 75.268817.

## 5. Root cause: the campaign reload re-rolls the loop's register split

The original's loop register split is:

```
sl = stations (spilled to [sp,#116])
r8 = stationId (live across the iteration)
r9 = currentStation
r6 = campaignMission
r5 = freelance
fp = i
r1 = textOffset
```

Ours is:

```
r6 = stations, sl = stations (duplicate, never spilled)
r8 = stationId
r9 = currentStation
r6 = campaignMission (temporarily; reused for stations in the freelance block)
r5 = freelance
fp = i
r1 = textOffset
```

Adding the campaign reload (`camp_reload`) makes clang spill `stations` to
`[sp,#148]`, move `stationId` to `r9`, `currentStation` to `r6`, and the
reload to `r8`. That is a different allocation from the original in every
register, so the whole loop (and the landmark setup/body after it) re-rolls:
the size drops to 9904 B and the score to 73.44 %. The same happens for every
declaration order and type spelling tried. The `marked` normalisation is a
second-order symptom of the same pressure: with `stations` in `sl` and
`stationId` in `r8`, the original spills `marked` across the
`getCurrentCampaignMission()` call and canonicalises it on reload; ours keeps
it in a register.

The second literal pool is a third-order symptom: because the loop is shorter
and the landmark loop is rotated, the setup ends in a water block and the pool
lands after it. Adding the missing 0x16–0x26 B to the loop does not un-rotate
the landmark loop (wave-123 tried every loop form), so the pool stays after
the setup.

## 6. Gate / section deltas (baseline, unchanged)

```
compared 4524  avg 99.93%  byte_exact 4506  linked_exact 4506  fuzzy_perfect 4506
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Section deltas vs the original: `.text` 85,975 differing bytes (size/base
exact, `0x1337c8`), `.ARM.extab` 1,474 differing bytes (size/base exact,
`0x26298` @ `0x1a3b78`), `.note.gnu.build-id` 20. `.rodata`, `.data`, `.bss`,
`.got`, `.ARM.exidx`, `.rel.plt`, `.rel.dyn`, `.dynsym`, `.dynstr`, `.hash`
are byte-identical.

## 7. Artifacts

Scratch (not committed, `/dev/shm/w7512`): `Radar.base.cpp` (pristine source),
`score.py`, `diffnorm.py`, `diffregion.py`, `diffany.py`, `batch.py`–`batch23.py`,
`orig.fn`, `ours.fn`, `ours1.fn`–`ours8.fn`, `show.txt`. The analysis is
self-contained in this note; the scratch files are reproducible from the
commands in §1.
