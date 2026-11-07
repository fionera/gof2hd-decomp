# Radar::draw wave 134 (w7773) — enemy-tail block split: loop-exit trunc placement flips the branch, but costs 32 B (no source win)

**Status:** research note, 2026-11-07. Worktree `wt/w7773`, based on `master`
(`bbffef482e`). **No source change landed**; this commit contains this note only.
The target remains the pristine baseline `src/game/weapons/Radar.cpp`
(`sha256 9b315de8bf294d1f5bbc303ef37551b6d828598d57ea6c4a121882bba684fbd3`).

Target `_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).
Original 9908 B / 3492 normalised instructions; ours 9908 B / 3483.
Baseline unrounded `difflib.SequenceMatcher(None, on, un, autojunk=False).ratio()`
= **75.354839 %** (official display 75.4). Gate at start and end:
`RATCHET PASS`, byte **4506**, linked **4506**, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, avg 99.94.

Read with `radar-draw-wave134-w7754.md`, `radar-draw-wave134-w7753.md`,
`radar-draw-wave131-w7712.md`, `radar-draw-body-wave122-w7210.md`.

This wave attacked the assignment's hypothesis directly: **reconstruct the
exact enemy-tail block layout from Ghidra and test source shapes that split it
without changing the 9908 B size**. It adds one new, positive result the
earlier notes did not have: the enemy-tail branch direction *can* be flipped to
the original's (`cbnz` to the post-if block, `checkB` fall-through) by moving
the `bossSeen`/`enemyHit` truncations into the loop-exit block, but the only
source shape found that does it costs **+32 B** and drops the whole-function
score to 71.6. The baseline is still the best local optimum.

## 1. Method / scratch

```sh
cd /home/debian/GalaxyOnFire2/wt/w7773
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6     # ~1 s per variant
cmake --build cmake-build-match --target check -j6        # RATCHET PASS
```

Ghidra decompile (provider reachable with
`GHIDRA_INSTALL_DIR=/tmp/ghidra_12.1.2_PUBLIC`):

```sh
rea decompile /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --procedure 0x13e050 --format md > ghidra_draw.md
```

Scratch in `/dev/shm/w7773` (not committed): `Radar.base.cpp` (pristine
source), `score.py` (single-symbol difflib harness reproducing 75.354839
exactly), `run.py` (variant driver), `dump_tail.py` (normalised instruction
alignment around the enemy tail), `ghidra_draw.md` (2019-line decompile),
`oilb.diff` (exact source diff of the layout-flipping variant).

## 2. The exact enemy-tail layout (original vs baseline)

The tail is `if (!A && !(B || turretMode || otherLock)) { body }` where
`A = isDockingToAsteroid()`, `B = isDockingToDockingPoint()`. The original
places the whole condition contiguously in the loop-exit block, then the body,
then the post-if `otherLock` recompute:

```
ORIG 0x12f3a6  ldr r0,[r4,#0]              ; this->level
      0x12f3a8  blx Level::getPlayer
      0x12f3ac  blx PlayerEgo::isDockingToAsteroid      ; A
      0x12f3b0  and.w r1,r6,#1               ; bossSeen  (loop value in r6)
      0x12f3b4  str r1,[sp,#92]              ; bossSeen  (outer)
      0x12f3b6  ldr r1,[sp,#80]              ; alwaysEnemy (loop)
      0x12f3b8  cmp r0,#0
      0x12f3ba  and.w r1,r1,#1
      0x12f3be  str r1,[sp,#104]             ; alwaysEnemySeen (outer)
      0x12f3c0  ldr r1,[sp,#88]              ; enemyHit (loop)
      0x12f3c2  and.w r1,r1,#1
      0x12f3c6  str r1,[sp,#132]             ; enemyHit (outer)
      0x12f3c8  bne.n 0x12f40e               ; A -> post-if
      0x12f3ca  blx Level::getPlayer
      0x12f3d0  blx PlayerEgo::isDockingToDockingPoint  ; B
      0x12f3d4  ldr r1,[sp,#148]             ; turretMode
      0x12f3d6  orrs r0,r1
      0x12f3d8  ldr r1,[sp,#144]             ; otherLock
      0x12f3da  orrs r0,r1
      0x12f3dc  bne.n 0x12f40e
      0x12f3de  ldr r0,[r4,#8]               ; navWreck body ...
```

The baseline splits the same condition and puts the post-if block first:

```
OURS 0x12f3cc  ldr r0,[r4,#0]
      0x12f3ce  blx Level::getPlayer
      0x12f3d2  ldr r1,[sp,#80]
      0x12f3d4  and.w r1,r1,#1
      0x12f3d8  str r1,[sp,#92]              ; alwaysEnemySeen only
      0x12f3da  blx PlayerEgo::isDockingToAsteroid
      0x12f3de  cmp r0,#0
      0x12f3e0  beq.w 0x12f888               ; !A -> checkB (outlined)
      0x12f3e4  ldr r5,[sp,#88]              ; else: bossSeen/enemyHit truncs
      0x12f3e6  ldr r0,[sp,#108]
      0x12f3e8  and.w r0,r0,#1
      0x12f3ec  str r0,[sp,#108]
      0x12f3ee  and.w r0,r5,#1
      0x12f3f2  str r0,[sp,#132]
      0x12f3f4  b.n 0x12f402                 ; jump over the zero block
      0x12f402  ...                          ; otherLock recompute
      ...
      0x12f888  ldr r2,[sp,#144]             ; checkB (outlined)
      0x12f88a  cmp r2,#0
      0x12f88c  it ne
      0x12f88e  movne r2,#1                  ; otherLock != 0 normalization
      0x12f890  ldr r0,[r4,#0]
      0x12f892  ldr r1,[sp,#152]             ; turretMode
      0x12f894  orr.w r5,r1,r2
      0x12f898  blx Level::getPlayer
      0x12f89c  blx PlayerEgo::isDockingToDockingPoint
      0x12f8a0  cmp r5,#0
      0x12f8a2  bne.w 0x12f3e4
      0x12f8a6  ...                          ; body
```

So the delta is exactly **which successor of the loop-exit block is the
fall-through**: the original keeps `checkB` inline and branches to the post-if
block; the baseline keeps the post-if block inline and outlines `checkB` +
body.

## 3. New result: the branch direction can be flipped (at a price)

The IR at `-Oz` has `bossSeen`/`enemyHit` as i8 SSA phis, and the `and #1`
truncations sit in the merge block `%1808` (the post-if merge), not in the
loop-exit block `%1156`. Because the post-if block is then tiny (two truncs +
a branch), it becomes the preferred fall-through.

The one source shape found this wave that moves the truncations into the
loop-exit block is to keep the **loop locals `bool`** and make the **outer
variables `int`**, with an explicit copy at the loop exit:

```c++
int bossSeen = 0;
int enemyHit = 0;
if (this->scannerAvailable && this->enemies != nullptr) {
    bool alwaysEnemy = false;
    bool bossSeenLoop = false;
    bool enemyHitLoop = false;
    for (...) { ... bossSeenLoop = true; ... enemyHitLoop = true/false; ... }
    alwaysEnemySeen = alwaysEnemy;
    bossSeen = bossSeenLoop;      // bool -> int zext at the loop exit
    enemyHit = enemyHitLoop;      // bool -> int zext at the loop exit
    if (!A && !(B || turretMode || otherLock)) { ... }
}
```

The exact diff is `oilb.diff` in the scratch dir (reproduced in §6). This
variant produces the original's branch direction and block order:

```
OILB 0x...  ldr r1,[sp,#108] ; and #1 ; str [sp,#88]   ; bossSeen trunc
            ldr r1,[sp,#80]  ; and #1 ; str [sp,#108]  ; alwaysEnemySeen trunc
            blx PlayerEgo::isDockingToAsteroid
            cbnz r0, <post-if>                          ; A -> post-if
            ldr r2,[sp,#144] ; cmp ; it ne ; movne r2,#1
            ldr r0,[r4,#0] ; ldr r1,[sp,#152] ; orr.w r5,r1,r2
            blx Level::getPlayer
            blx PlayerEgo::isDockingToDockingPoint
            cbnz r5, <post-if>
            cbnz r0, <post-if>
            ldr r0,[r4,#8]                              ; body fall-through
```

i.e. **entry -> checkB -> body -> post-if**, the original's layout. But the
variant is **9940 B** (32 B larger) and the whole-function ratio falls to
**71.565678 %** (official 71.6): the extra loop-local bools and the copy-back
re-roll the loop and the rest of the function. It is not a net win.

## 4. Why the baseline wins: block-size / fall-through coupling

The observed correlation is consistent with `MachineBlockPlacement` preferring
the **smaller** successor as the fall-through when the branch probability is
50/50:

| | original | baseline |
|---|---|---|
| loop-exit block | 3 truncs + call + branch | 1 trunc + call + branch |
| post-if block (else target) | `otherLock` recompute, ~20 insns | 2 truncs + branch, ~7 insns |
| `checkB` block | 8 insns (`ldr/blx/blx/ldr/orrs/ldr/orrs/bne`) | 14 insns (adds `cmp/it/movne` + `orr.w` before the call) |
| MBP fall-through | `checkB` (8 < 20) | post-if (7 < 14) |

The baseline's `checkB` is larger because `otherLock` is `int`: the
`turretMode || otherLock` bool conversion needs `cmp/it/movne` and the OR is
hoisted before the `getPlayer` call. The original's `checkB` uses two raw
`orrs` and no normalization, which is why it is small enough to be chosen.
Making `otherLock` `bool` does shrink `checkB` to the original's 8-instruction
shape (`orrs` twice), but it also changes the first `otherLock` guard to the
`cbnz` chain and costs 96 B overall (9812 B / 69.07 %, re-confirmed this wave).

## 5. Variant matrix (all single edits on the pristine baseline)

Unless noted, every variant below is **9908 B** and **75.354839 %** (an exact
tie, i.e. byte-identical output):

### 5.1 Condition / control-flow shapes (all ties)

| variant | what |
|---|---|
| `nested_if` | `if (!A) { if (!(B\|\|t\|\|o)) { body } }` |
| `if_else_if` | `if (A) {} else if (!(B\|\|t\|\|o)) { body }` |
| `goto_done` | `if (A) goto done; if (B\|\|t\|\|o) goto done; { body } done:;` |
| `or_chain_not` | `!A && !B && !t && !o` |
| `notA_and_cond` | `!(A \|\| B \|\| t \|\| o)` (B evaluated even when A) |
| `hoist_dockast` | `bool dockingToAsteroid = A; if (!dockingToAsteroid && ...)` |
| `neg_and` | `!A && !B && !t && !o` |
| `cond_bool` | `bool canTail = ...; if (canTail)` |
| `if_not_else` | positive `if (A \|\| B \|\| t \|\| o) {} else { body }` |
| `explicit_ne0` | `!(B \|\| t \|\| otherLock != 0)` |
| `double_not` | `!(B \|\| t \|\| !!otherLock)` |
| `old_otherlock` | `int oldOtherLock = otherLock;` used in the condition |
| `tailcond_sep` | split `tailAllowed` into two assignments |
| `recompute_else` / `recompute_dup` | move/duplicate the post-if recompute |
| `recomp_ternary` / `recomp_split` | post-if recompute spelling |
| `move_aes_down` | move `int alwaysEnemySeen` next to `bossSeen`/`enemyHit` |
| `nested_scanner` | `if (scannerAvailable) { if (enemies) { ... } }` |

### 5.2 Type / truncation shapes (worse)

| variant | usize | raw % |
|---|---:|---:|
| **baseline** | **9908** | **75.354839** |
| `outer_int_loop_bool` (both) | 9940 | 71.565678 |
| `outer_int_eh_only` | 9940 | 71.790468 |
| `tail_int_copy` (both) | 9920 | 72.982657 |
| `tail_int_eh` | 9916 | 73.230152 |
| `plus0` / `cast_int` / `ternary01` / `and1` | 9916 | 73.939220 |
| `selfassign` / `selfassign_bool` | 9916 | 73.939220 |
| `bool_otherlock` | 9812 | 69.067858 |
| `oilb_bool_otherlock` | 9876 | 71.766901 |
| `oieh_bool_otherlock` | 9844 | 67.710878 |
| `enemyhit_int` / `bossseen_int` / `both_int` | 9924 / 9904 / 9924 | 71.42 / 73.49 / 71.03 |
| `always_enemy_int` | 9900 | 72.557739 |
| `aes_bool` | 9904 | 72.555205 |
| `volatile_bs` / `volatile_eh` / `volatile_both` | 9844 / 9848 / 9852 | 68.67 / 62.37 / 63.04 |
| `move_aes_down2` | 9876 | 70.468256 |
| `reorder_eh_bs` | 9896 | 66.350711 |

### 5.3 Condition-ordering / scheduling (worse)

| variant | usize | raw % |
|---|---:|---:|
| `cache_player` | 9904 | 68.578804 |
| `hoist_both` | 9884 | 68.532874 |
| `otherlockb_local` / `otherlockb_local2` / `bothb_local` | 9888 | 68.686289 |
| `notB_local` | 9884 | 68.475452 |
| `or_int` | 9888 | 65.508329 |
| `or_int2` | 9864 | 67.835585 |
| `swap_scanner_cond` / `scanner_notnull` | 9908 | 75.211470 |
| `expect_cond1` (whole condition `expect(...,1)`) | 9892 | 68.390062 |
| `expect_notA1` / `expect_A0` | 9872 | 62.818483 |
| `expect_cond0` | 9932 | 59.685714 |
| `expect_inner` | 9876 | 62.992806 |

The `__builtin_expect` variants are new this wave (the earlier notes only
tried `expect` on the first `otherLock` guard). They change the branch
probability but re-roll the whole function; none helps.

## 6. Reproducible layout-flipping source diff (`outer_int_loop_bool`)

```diff
-    bool bossSeen = false;
-    bool enemyHit = false;
+    int bossSeen = 0;
+    int enemyHit = 0;
     if (this->scannerAvailable && this->enemies != nullptr) {
         bool alwaysEnemy = false;
+        bool bossSeenLoop = false;
+        bool enemyHitLoop = false;
         for (uint32_t i = 0; i < this->enemies->size(); ++i) {
...
-                    bossSeen = true;
+                    bossSeenLoop = true;
...
-                !(enemy->countsAsEnemyExcludeFlag != 0 && !wreck) && (!enemyHit || enemy->field_0x24) &&
+                !(enemy->countsAsEnemyExcludeFlag != 0 && !wreck) && (!enemyHitLoop || enemy->field_0x24) &&
...
-                    enemyHit = true;
+                    enemyHitLoop = true;
...
         alwaysEnemySeen = alwaysEnemy;
+        bossSeen = bossSeenLoop;
+        enemyHit = enemyHitLoop;
 
         if (!this->level->getPlayer()->isDockingToAsteroid() &&
```

## 7. Root cause (unchanged, with the new block-size evidence)

The residual is the coupled `MachineBlockPlacement` / register-allocation fixed
point:

1. the first `otherLock` guard's `cbz`+`itt` shape (ours) vs the original's
   `cbnz` chain, and the resulting first literal pool at `0x12e310` vs
   `0x12e340`;
2. the invented post-loop `onPlanetCourse` recompute at `0x12e72c` (72 B);
3. **the enemy-tail block split (§2–§4): the truncations are in the merge
   block, making the post-if block the small fall-through; moving them to the
   loop exit flips the branch to the original's direction but costs 32 B;**
4. `stations` in `r6` instead of `sl`+spill, and the `turretMode` slot swap;
5. the bottom-placed landmark loop header (`0x12eb48` vs `0x12e7d4`);
6. the ship `inSight` `s24`/`s26` live range (`vpush {d8-d13}` vs `{d8-d11}`).

The new evidence pins the enemy-tail delta to a **block-size-driven
fall-through choice**, not to the condition spelling (every spelling that
preserves the IR is byte-identical). A future wave would need a source shape
that moves the `bossSeen`/`enemyHit` truncations into the loop-exit block
**without** adding the 32 B of loop-local bool/copy-back overhead — e.g. a
shape that keeps the loop locals as `bool` in registers and only widens at the
exit, or a change to the neighbouring `stations`/`otherLock` allocation that
frees the registers needed to keep `bossSeen` in `r6`.

## 8. Gate / section deltas (baseline, unchanged)

```
compared 4524  avg 99.94%  byte_exact 4506  linked_exact 4506  fuzzy_perfect 4506
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Section deltas vs the original: `.text` **85,022** differing bytes (size/base
exact), `.ARM.extab` **1,416** (size exact 156,312), `.note.gnu.build-id`
**19**. `.rodata`, `.data`, `.bss`, `.got`, `.ARM.exidx`, `.rel.plt`,
`.rel.dyn`, `.dynsym`, `.dynstr`, `.hash` are byte-identical.
