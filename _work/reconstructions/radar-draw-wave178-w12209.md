# Radar::draw wave 178 (w12209) — five size-exact operand-order cells

**Status:** source change landed, 2026-11-11. Worktree `wt/w12209`, branch
`wave178-w12209`, base `master` `1b48f990f` (wave-177 merge). Target
`src/game/weapons/Radar.cpp`, `_ZN5Radar4drawEP6PlayerP3Hudi`
(`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave177-w12107.md` / `radar-draw-wave177-w12108.md`
(the landed shape this pass starts from) and `radar-draw-wave174-w11907.md`
(the `campaign`/`sl` residual analysis).

## 0. Baseline and result

| metric | base | landed |
|---|---:|---:|
| function size | 9,908 | **9,908** |
| raw same-address diff | 6,636 | **6,626** (-10) |
| instruction-index alignment | 1,813 | **1,827** (+14) |
| instruction count | 3,539 ours / 3,532 orig | 3,539 / 3,532 |
| whole-`.so` `.text` differing bytes | 79,251 | **79,241** (-10) |
| `.ARM.exidx` differing bytes | 0 | 0 |
| `.ARM.extab` differing bytes | 731 (size/base exact) | 731 |
| `.rel.plt` | byte-identical (0) | byte-identical (0) |
| `.rodata` / `.data` / `.bss` | 0 / 0 / 0 | 0 / 0 / 0 |

Gate after the change: byte_exact **4,507/4,524**, linked_exact
**4,507/4,524**, size-exact **4,524/4,524**, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact **843/843**, imports **207**, lint
**CLEAN**, drift **0**, parity **5,370/5,370**. `RATCHET PASS`.

## 1. Landed change (five `&&`/`||` operand-order cells, net size 0)

All five are pure operand reorderings of short-circuit expressions. The
operands are side-effect-free field reads / null tests, so evaluation order
is unobservable; only the emitted branch shape changes.

```diff
@@ otherLock recompute before the gas-cloud block (line ~949)
-    otherLock = (this->dockNavPtr != nullptr && this->lockedEnemy == nullptr) || this->field_0x1c != nullptr ||
-                this->lockedStation != nullptr || this->field_0x14 != 0;
+    otherLock = this->lockedStation != nullptr || (this->dockNavPtr != nullptr && this->lockedEnemy == nullptr) ||
+                this->field_0x1c != nullptr || this->field_0x14 != 0;

@@ enemy inSight test (line ~795)
                         inSight = this->screenX > cx - this->field_0x124 && this->screenX < cx + this->field_0x124 &&
-                                  this->screenY > cy - this->field_0x124 && this->screenY < cy + this->field_0x124;
+                                  this->screenY < cy + this->field_0x124 && this->screenY > cy - this->field_0x124;

@@ asteroid crosshair bounds (line ~844; the FIRST of the two identical
@@ `if (screenX > left && ...)` blocks — line 1014 gas bounds is unchanged)
-                    if (this->screenX > left && this->screenX < left + size * 2 && this->screenY > top &&
-                        this->screenY < top + size * 2) {
+                    if (this->screenY < top + size * 2 && this->screenY > top && this->screenX > left &&
+                        this->screenX < left + size * 2) {

@@ enemy tractor label (line ~859)
-            if (this->tractorMode2 && this->field_0x1c == nullptr && wreck && enemy->isDead()) {
+            if (this->tractorMode2 && wreck && enemy->isDead() && this->field_0x1c == nullptr) {

@@ asteroid tractor label (line ~1160)
-                if (this->tractorMode2 && this->field_0x1c == nullptr && wreck) {
+                if (this->field_0x1c == nullptr && wreck && this->tractorMode2) {
```

Individual contributions on the wave-177 base (all size 9,908):

| cell | raw | aligned |
|---|---:|---:|
| base | 6,636 | 1,813 |
| 1 `otherLock` lockedStation-first | 6,635 | 1,816 |
| 2 `inSight` y-operands swapped | 6,635 | 1,816 |
| 3 asteroid bounds y-first | 6,635 | 1,820 |
| 4 enemy tractor `wreck`/`isDead` before `field_0x1c` | 6,631 | 1,804 |
| 5 asteroid tractor `field_0x1c` first | 6,634 | 1,823 |
| **1+2+3+4+5 (landed)** | **6,626** | **1,827** |

The five cells interact through branch layout: the combination is 5 bytes
better than the sum of the individual deltas would suggest.

## 2. Search performed (all size-exact cells only)

Baseline for every cell below is the landed 6,626 / 1,827 / 9,908. All
builds used `cmake --build cmake-build-match --target link_so -j2` under
`ulimit -v 12000000`; scratch is
`/home/debian/GalaxyOnFire2/scratch/w12209/`.

### 2.1 Rejected reorderings

| variant | raw | aligned | size |
|---|---:|---:|---:|
| gas-cloud bounds (line 1014) y-first, on top of landed | 6,627 | 1,834 | 9,908 |
| `candidateWreck` 4-op: `hasCargo` first | 6,629 | 1,830 | 9,908 |
| `candidateWreck` 4-op: `field_0x10 != nullptr` first (base) | 6,626 | 1,827 | 9,908 |
| `candidateWreck` 4-op: `isDead\|\|isDying` before `hasCargo` | 7,296-7,300 | 1,847+ | **9,912** |
| asteroid `otherLock` (line ~531) `hasDockNav` first (base) | 6,626 | 1,827 | 9,908 |
| asteroid `otherLock` `field_0x1c` first | 6,637 | 1,831 | 9,908 |
| asteroid `otherLock` `lockedStation` first | 6,638 | 1,832 | 9,908 |
| asteroid `otherLock` `field_0x14` first | 6,639 | 1,830 | 9,908 |
| asteroid `otherLock` `hasDockNav` group last | 7,311-8,087 | — | 9,908/**9,912+** |
| lock-image chain enemy-first | 6,628 | 1,823 | 9,908 |
| lock-image chain via the `Player *target` alias | 6,878 | 1,968 | 9,908 |
| drop the `Player *target` alias (use `enemy->player` directly) | 6,626 | 1,827 | 9,908 |
| `KIPlayer *locked` load moved after the steal check | 8,458 | 1,946 | **9,900** |
| `bool wreck` decl moved after `update(enemy)` | 8,540 | 1,843 | **9,904** |
| `Player *target` hoisted before the 24000-unit distance chain | 6,649 | 1,833 | 9,908 |
| `int zero = 0;` feeding `plasmaInRange`/`lockConfirmed` | 6,626 | 1,827 | 9,908 |
| `int missionBlocksPlanets = 0;` (vs `false`) | 6,626 | 1,827 | 9,908 |
| `bool notTurretMode` / `bool notOtherLock` | 8,853 | 2,116 | **9,892** |

### 2.2 Prior-session cells re-confirmed on the landed base

* Declaration permutations of `turretMode`/`alienOrbit`/`campaign`: base
  order is best (6,636); other orders 6,643-6,661 or size-fatal.
* Wreck-block statement order `WZU` is optimal (`UWZ` 6,638, `WUZ` 6,637,
  others size-fatal).
* Broad 2-4-operand permutation sweeps over ~20 further conditions
  (landmark guard, planet guard, scanner, steal, `navWreck`, sparks,
  distance, gas guard, lock sounds, turret guards, `dock_guard_inner`,
  `enemy_lock_guard`, `ast_lock_guard`, `tractorAuto`, `ast_tractor3`,
  `tractor_has13`): all neutral or worse on top of 6,626.
* `alwaysEnemy = isAlwaysEnemy()` size-fatal (9,980/9,976); function-scope
  `Status*`/`Canvas*`/`FModSound*` aliases size-fatal (9,704/10,004/9,416);
  block-scope `Player *target` / `shipGroup` / `hasCargo` hoists size-fatal;
  `lockConfirmed`/`plasmaInRange` zero-store moves size-fatal;
  `campaignMission` guard expect/plain size-fatal.

## 3. Remaining residual

Unchanged from wave 177: the original keeps `campaign` in `sl`
(`12e0f8 mov sl,r0`, `12e38a cmp.w sl,#2`) and reuses `sl` for the planet
loop index (`12e4be ldr.w r0,[sl,#4]`), while ours spills `campaign` to
`sp+0x90` and uses `sl` for the zero constant / planet loop index
(`U[17] mov.w sl,#0` vs `O[17] mov.w fp,#0`). The original's zero constant
and landmark/planet loop counter live in `fp`; ours uses `fp` as a scratch
for the `Status *` reload (`U[363] ldr.w fp,[sp,#156]`). Every
size-exact `campaign`-in-`sl` source shape measured in waves 174-175 is
raw-worse, so the rotation is still open.

The tail exception-pad site (`sp+220` twice vs the original's `220`/`208`)
is unchanged and wave 164 showed it is not source-reachable.

## 4. Reproduction

```sh
ulimit -v 12000000
cp /home/debian/GalaxyOnFire2/scratch/w12209/Radar.base.cpp src/game/weapons/Radar.cpp
# apply the five edits from section 1, then:
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
cmake --build cmake-build-match --target check -j2
cmake --build cmake-build-match --target drift -j2
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w12209/` —
`Radar.base.cpp` (wave-177 base source), `eval.py` (build+measure harness),
`combo2.py` (the five landed cells), `combo5.py` / `combo7.py` / `combo8.py`
(the rejected-cell sweeps), `dump.py` (side-by-side disasm),
`catdiff.py` (instruction diff classifier), `catdiff_new.txt` (landed-state
diff census).
