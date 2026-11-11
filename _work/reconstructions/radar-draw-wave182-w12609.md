# Radar::draw wave 182 (w12609) — planet-lock body: plain dock guard + `expect` sound guard

**Status:** landed, 2026-11-11. Worktree `wt/w12609`, branch `wave182-w12609`,
base `master` `57e085059` (wave-181 harvest 5). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave181-w12508-pass2.md` (the landed 6,356 shape that
became 6,339 after the wave-181 harvest) and `radar-draw-wave180-w12408.md`
§3.3 (the +8 B dock-guard site). This note is self-contained: every number
below was measured on `57e085059` in this worktree.

## 0. Baseline and landed result

| metric | base | landed |
|---|---:|---:|
| function size | 9,908 | **9,908** |
| raw same-address diff | 6,339 | **6,329** (-10) |
| instruction-index alignment | 1,757 | 1,776 |
| instruction count | 3,542 ours / 3,532 orig | 3,543 / 3,532 |
| whole-`.so` `.text` differing bytes | 75,365 | **75,355** (-10) |
| `.ARM.exidx` differing bytes | 0 | 0 |
| `.ARM.extab` differing bytes | 655 (size/base exact at `0x1a3b78`, 156,312 B) | **655** (unchanged) |
| `.rel.plt` | byte-identical | **byte-identical** |
| `.rodata` / `.data` / `.dynsym` / `.dynstr` / `.hash` | 0 | 0 |
| `.note.gnu.build-id` differing bytes | 20 | 20 |
| total differing bytes | 76,040 | **76,030** |
| file size | 2,187,328 | 2,187,328 |

Gate after the change: byte_exact **4,507/4,524**, linked_exact
**4,507/4,524**, size-exact **4,524/4,524**, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift **0**, parity **5,370/5,370**. `RATCHET PASS`.

## 1. Landed changes (net size 0)

```diff
@@ planet-lock body (line ~635)
-                    if (__builtin_expect(this->field_0x1a8, 0)) {  // lint: arm_builtin
-                        this->level->getPlayer()->dockToPlanet();
-                    }
-                    if (!this->lockConfirmed) {
-                        Globals::sound->play(26, nullptr, nullptr, 0.0f);
-                    }
                     this->field_0x14 = this->field_0x18;
+                    if (this->field_0x1a8) {
+                        this->level->getPlayer()->dockToPlanet();
+                    }
+                    if (__builtin_expect(!this->lockConfirmed, 0)) {  // lint: arm_builtin
+                        Globals::sound->play(26, nullptr, nullptr, 0.0f);
+                    }
```

Three independent edits on the same block:
1. The dock guard loses its `__builtin_expect(..., 0)` (plain `if`). This is
   the "+8 B site": alone it shrinks the function by 8 B (9,900), so a
   compensating `expect` is required.
2. The sound guard gains the `__builtin_expect(!lockConfirmed, 0)` hint. This
   restores the +8 B and is the compensation.
3. The statement order becomes `store; dock; sound` (the store before the two
   guards instead of after them).

All three are semantics-preserving: `field_0x14 = field_0x18` is a plain member
write that neither `dockToPlanet()` nor `FModSound::play` reads; the dock and
sound guards remain independent `if`s (the sound still fires when the dock
guard is taken, as in the original); the `expect` is a pure branch hint.

### 1.2 Second cell: drop the campaign-145 `expect`

```diff
@@ music tail, campaign-145 gate (line ~1303)
-        if (__builtin_expect((Globals::status->getCurrentCampaignMission() != 145), 1)) {  // lint: arm_builtin
+        if (Globals::status->getCurrentCampaignMission() != 145) {
```

This is a pure branch-hint removal; the predicate is unchanged. On the
wave-181 shape this site was neutral/worse; on the new planet-lock shape it is
the unique single-`expect` improvement (-5 raw, 6,335 -> 6,330, size exact).
It is the last `__builtin_expect` site in the function (index 32 of 33).

### 1.3 Third cell: enemy-loop `lockedEnemy` dead test order

```diff
@@ enemy-loop tail, lockedEnemy cleanup (line ~870)
-            if (this->lockedEnemy != nullptr && (this->lockedEnemy->isDying() || this->lockedEnemy->isDead())) {
+            if (this->lockedEnemy != nullptr && (this->lockedEnemy->isDead() || this->lockedEnemy->isDying())) {
```

The two predicates are side-effect-free and the OR is commutative. This is the
secondary `lockedEnemy` cleanup inside the `enemyHit` tail (the first one at
line 673, before the enemy loop, keeps its original `isDying`-first order and
is neutral). Swapping only this occurrence is -1 raw, size exact.

## 2. Why this moves bytes

The original emits the whole planet-lock body as one inline cold block
(`130454`-`130482`): the `lockConfirmed`/`play(26)` test first, then the
`field_0x1a8` dock guard with `str field_0x14` scheduled into the delay slot
before the branch. The wave-181 shape kept the dock body out of line
(`bne.w` to a separate cold block) because the `expect(field_0x1a8, 0)` hint
forced it there. Dropping that hint makes clang emit the dock body inline;
the `expect(!lockConfirmed, 0)` on the sound guard then re-establishes the
9,908 B size, and the `store; dock; sound` order lets the store be scheduled
next to the `field_0x18` load.

## 3. Search evidence (all size-exact unless noted)

Every measurement is a full `link_so` + `tools/fn_diff.py` on this worktree
(`-j2`, `ulimit -v 12000000`).

### 3.1 Planet-lock block sweep (144 variants)

Cross product of 6 dock-guard spellings × 4 sound-guard spellings × 6
statement orders of `store`/`dock`/`sound`, all on the base source. Only the
`plain dock + expect(!lockConfirmed)` family wins:

| dock guard | sound guard | order | raw | size |
|---|---|---|---:|---:|
| `expect(field_0x1a8,0)` (base) | plain | dock,sound,store | 6,339 | 9,908 |
| plain | `expect(!lockConfirmed,0)` | **store,dock,sound (landed)** | **6,335** | **9,908** |
| plain | `expect(!lockConfirmed,0)` | sound,dock,store | 6,337 | 9,908 |
| plain | any `expect` form | dock,sound,store | 6,340 | 9,908 |
| plain | plain | sound,store,dock | 6,383 | **9,904** |

All other 138 variants are >= 6,339, size-fatal, or byte-identical to the
base. The three `expect(!lockConfirmed,0)` spellings (`!x` hint, `!x` with
`expect(x,1)`, and the `if/else` form) compile identically.

### 3.2 shipGroup / wreck branches (14 variants)

Statement orders of `wreck` / `field_0x75 = 0` / `update`, and four
`bossSeen` spellings, on the base source:

| variant | raw | size |
|---|---:|---:|
| base (`wreck; field_0x75=0; update`, `bossSeen = shipGroup == 10`) | **6,339** | 9,908 |
| `update; wreck; field_0x75=0` | 6,341 | 9,908 |
| `wreck; update; field_0x75=0` | 6,340 | 9,908 |
| `bossSeen = shipGroup != 10 ? false : true` | 6,339 | 9,908 |
| `bossSeen = shipGroup == 10 ? true : false` | 6,339 | 9,908 |
| `if (shipGroup == 10) bossSeen = true` | 9,223 | **9,988** |
| `bossSeen = true` | 8,496 | **9,900** |

The base stays: the original's `it eq; moveq r6,#1` in the skip branch is
reached only by the branchless `subs/clz/lsrs` sequence, and every form that
forces the `ite` changes size. (The original's loop-carried `bossSeen` keeps
its value when the skip branch's re-test is false; the source assignment form
is the shape that reproduces it at 9,908 B.)

## 4. Remaining residual

Unchanged from waves 174-181: the original keeps `campaign` in `sl` and later
reuses `sl` for the stations array, with the planet-loop index in `fp`; ours
spills `campaign` to `sp+0x90` and uses `sl` for the loop index. That cascade
is still the dominant residual. The dock body is now inline like the
original's, but at `+0x1a8` addresses because of the earlier global shift.

## 5. Reproduction

```sh
ulimit -v 12000000
cp /home/debian/GalaxyOnFire2/scratch/w12609/BR/locked_second_only.cpp src/game/weapons/Radar.cpp
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6329 aligned= 1776 size=9908/9908 insn=3543/3532
cmake --build cmake-build-match --target check -j2    # RATCHET PASS
cmake --build cmake-build-match --target drift -j2    # 0 field(s)
```

Section-level diff (scratch helper `secdiff.py`, ELF parser):

```sh
python3 /home/debian/GalaxyOnFire2/scratch/w12609/secdiff.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
# .text 75,355 ; .ARM.extab 655 ; .note.gnu.build-id 19 ; .rel.plt identical
```

## 6. Scratch inventory (`/home/debian/GalaxyOnFire2/scratch/w12609`)

* `Radar.base.cpp` — exact base source (HEAD `Radar.cpp`).
* `PL/D1_S1_sDS.cpp` — first-cell source (raw 6,335).
* `R_rm32.cpp` — second-cell source (raw 6,330).
* `BR/locked_second_only.cpp` — landed source (raw 6,329).
* `measure.sh <cpp>` — copy to `src/`, `link_so -j2`, `fn_diff`.
* `gen_plsweep.py` / `PL/` — 144-variant planet-lock block sweep.
* `gen_ship.py` / `S_*.cpp` — 14-variant shipGroup/wreck sweep.
* `gen_planet.py` / `P_*.cpp` — 11-variant first planet-body sweep.
* `gen_expect.py` / `EX/` — 33-site remove/flip sweep (66 variants).
* `gen_expect2.py` / `EX2/` — 32-site re-sweep on the 6,330 shape.
* `gen_plsweep2.py` / `PL2/` — 144-variant planet-block re-sweep.
* `gen_ship2.py` / `S2_*.cpp` — shipGroup/wreck re-sweep.
* `heat.py` — per-address differing-run census.
* `dump.py` / `full.py` — side-by-side and full disassembly dumps.
* `secdiff.py` — per-section differing-byte report.
