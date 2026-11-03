# Radar::draw — block-layout and cold-chain analysis (w4202)

**Status:** research note, 2026-11-03. Worktree `wt/w4202`, branch `wave92-w4202`,
base `b379d00d`. No source change landed: the semantically-correct source shape
(the w4105 `dead + inSight` patch plus removal of the duplicate autopilot
block) is size-*inexact* (9784 B vs the original 9908 B), so it would regress
the `size-exact 4524/4524` gate. This commit contains this note only.

## Baseline (confirmed before and after)

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
```

```
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`Radar::draw` = `osize 9908 usize 9908 pct 73.9 byte False linked False`.
`Radar.cpp` is compiled with the scoped
`-mllvm -likely-branch-weight=80 -mllvm -unlikely-branch-weight=1`
(`CMakeLists.txt:50`). That flag is load-bearing for the fuzzy score: removing
it gives 9908 B / **72.5 %** (see §4).

## 1. The semantically-correct patch (A+B+C)

The three locally-correct source shapes are the w4105 patch:

```diff
@@ -609,17 +609,6 @@
-        if (!alienOrbit) {
-            int autoPilotTarget = this->level->getPlayer()->getAutoPilotTarget();
-            SolarSystem *system = (SolarSystem *) (intptr_t) Globals::status->getSystem();
-            int stationIndex = system->getStationEnumIndex(
-                    ((Array<int> *) ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getStations())
-                            ->data()[this->planetDockIndex]);
-            this->field_0x1a8 = this->level->getPlayer()->goingToPlanet() &&
-                                !this->level->getPlayer()->isDockingToPlanet() &&
-                                autoPilotTarget == ((Array<int> *) this->planetTargets)->data()[stationIndex];
-        }
-
@@ -790,9 +779,9 @@
-                    if (this->screenX > cx - this->field_0x124 && this->screenY > cy - this->field_0x124 &&
-                        this->screenX < cx + this->field_0x124 && this->screenY < cy + this->field_0x124 &&
-                        AbyssEngine::AEMath::VectorLength(this->egoPos - this->targetPos) < 60000.0f) {
+                    if (AbyssEngine::AEMath::VectorLength(this->egoPos - this->targetPos) < 60000.0f &&
+                        this->screenX > cx - this->field_0x124 && this->screenX < cx + this->field_0x124 &&
+                        this->screenY > cy - this->field_0x124 && this->screenY < cy + this->field_0x124) {
                         inSight = true;
                     }
@@ -1093,8 +1082,8 @@
-                bool dead = asteroid->isDead() || asteroid->isDying();
-                if (dead && !asteroid->hasCargo) {
+                if (!(asteroid->hasCargo && (asteroid->isDead() || asteroid->isDying())) &&
+                    (asteroid->isDead() || asteroid->isDying())) {
                     continue;
                 }
```

(A) removes the duplicate `if (!alienOrbit)` autopilot block after the planet
loop; (B) restores the asteroid `!(hasCargo && dead) && dead` predicate; (C)
restores the enemy `inSight` distance-first / x-pair / y-pair order.

## 2. Measured variants (official verifier, `autojunk=False`)

All rows are the baseline `Radar.cpp` plus the listed edits, with the scoped
80:1 branch weights unless stated.

| variant | usize | pct | byte_exact | note |
|---|---:|---:|---:|---|
| baseline | 9908 | **73.9** | 4498 | size-exact |
| A only (drop duplicate) | 9740 | 62.3 | 2701 | size shift |
| A+B | 9760 | 61.8 | 2701 | size shift |
| A+B+C | 9784 | 61.0 | 2794 | size shift |
| A+B+C + enemy `__builtin_expect(...,1)` | 9732 | 56.6 | 2701 | size shift |
| A+B+C, no 80:1 flag | 9772 | 66.2 | 2701 | size shift |
| A+B+C, W=40 | 9772 | 66.2 | 2701 | size shift |
| A+B+C, W=60 | 9796 | 61.0 | 2701 | size shift |
| A+B+C, W=80..1000 | 9784 | 61.0 | 2794 | size shift |

`byte_exact` falls because `Radar::draw` changes size; every later function's
PC-relative literal-pool load shifts. The correct source shape is therefore
**not gate-landable** until it is also size-exact.

## 3. The call multiset proves the semantic body is complete

A+B+C has **344** `bl`/`blx` calls, exactly the original's **344** (the
baseline has 351). The multiset difference is:

```
orig  ours  callee
  9     8    PaintCanvas::DrawString(uint, String const&, int, int, bool)
  7     6    FModSound::play(int, Vector*, Vector*, float)
  0     2    __aeabi_f2lz   (compiler-generated; inside the literal pool)
```

The two missing calls are **tail-merge decisions**, not missing source:

* **Music play tail.** The original has two separate `play` tails:
  `0x1302d2` (the `field_0x1bc >= 1` block) and `0x130698` (the final
  `field_0x1bc < 1` block). A+B+C merges them into one tail at `0x130286`.
  The baseline (with the duplicate autopilot block) already has the original's
  seven `play` calls at `0x12f87a/0x12ff7c/0x13009a/0x1302d4/0x1303ea/0x13046c/0x13069c`
  — the last three addresses are *identical* to the original's
  `0x1303ea/0x13046c/0x13069c`.
* **Landmark distance `DrawString`.** The original emits the `i <= 1` else
  branch's distance call at `0x12e9e0` and the `i == 0 && !alienOrbit` special
  branch's second call at `0x12eb36`. A+B+C merges both into `0x12f7c2`
  (the else branch does `b.n 12f7c2` at `0x12f6a6`).

The baseline's remaining call delta is exactly the duplicate autopilot block
(`getStations +1`, `getStationEnumIndex +1`, `getPlayer +3`, `getSystem +2`,
`goingToPlanet +1`, `isDockingToPlanet +1`, `getAutoPilotTarget +1`) minus the
missing asteroid `isDead/isDying` pair and the missing landmark `DrawString`.
So A+B+C is semantically exact.

## 4. Branch-weight sweep

The 80:1 flag is a plateau, not a single magic value. Baseline (duplicate block
present):

| likely:unlikely | usize | pct |
|---|---:|---:|
| no flag (default heuristic) | 9908 | 72.5 |
| 10:1 | 9904 | 70.9 |
| 20:1 | 9904 | 70.9 |
| 40:1 | 9908 | 72.3 |
| 60:1 | 9908 | 73.8 |
| 80:1 | 9908 | **73.9** |
| 100:1 | 9908 | 73.9 |
| 200:1 | 9908 | 73.9 |
| 1000:1 | 9908 | 73.9 |

The 60:1..1000:1 plateau is stable; the 80:1 value is not uniquely special.
For A+B+C the same sweep gives 9772 (no flag), 9772 (40:1), 9796 (60:1),
9784 (80:1..1000:1) — i.e. the correct source shape cannot be rescued by a
branch weight.

## 5. Where the missing 124 bytes live

Original vs A+B+C block sizes (call-anchored, non-overlapping):

| region | original | A+B+C | delta |
|---|---:|---:|---:|
| prologue/init | 708 | 748 | +40 |
| landmark condition + planet block | 1172 | 1128 | -44 |
| landmark loop | 1076 | 1088 | +12 |
| cold tail (`missionBlocksPlanets` + `field_0x14 == 0`) | 72 | 0 | **-72** |
| enemy loop | 2108 | 2096 | -12 |
| gas clouds | 1758 | 1790 | +32 |
| asteroids | 2780 | 2718 | -62 |
| music | 234 | 216 | -18 |
| **total** | **9908** | **9784** | **-124** |

The single largest missing item is the **72-byte cold tail** between the
landmark loop and the `enemies:` label. The original places two cold blocks
there:

```
12ebda  mov r0,r5
12ebdc  blx Mission::getType          ; missionBlocksPlanets = getType() != 172
12ebe0  subs.w fp,r0,#172
12ebe4  it ne
12ebe6  movne.w fp,#1
12ebea  b.w 12e0dc                    ; back to the main body
...
12ec1c  movs r0,#0                    ; field_0x14 == 0 branch
12ec1e  str r0,[r4,#416]              ; planetLockTimer = 0
12ec22  str r0,[r4,#24]               ; field_0x18 = 0
12ec24  <enemies:>
```

A+B+C places the `missionBlocksPlanets` cold block at `0x12f8d8` (after the
enemy/gas region) and merges the `field_0x14 == 0` block into the planet tail
(`12e79c str; 12e7a0 cmp; 12e7a2 beq`), so the whole 72-byte tail disappears.

The baseline (duplicate block present) already has the cold block at
`0x12eb98`, only `0x44` bytes before the original's `0x12ebdc`, and the
landmark loop at `0x12e7c4` (original `0x12e7f4`, -0x30). That is why the
baseline, despite being semantically wrong, is the best-scoring shape: its
block layout is within ~0x44 bytes of the original throughout, while A+B+C
re-rolls the layout by hundreds of bytes.

## 6. Cold-chain placement evidence

Key call anchors (original / baseline / A+B+C):

| anchor | original | baseline | A+B+C |
|---|---:|---:|---:|
| planet `getStations` | 0x12e3c2 | 0x12e3c4 | 0x12e3ce |
| top `getAutoPilotTarget` | 0x12e6f4 | 0x12e6d8 | 0x12e6f4 |
| duplicate `getAutoPilotTarget` | — | 0x12e732 | — |
| landmark `isVisible` | 0x12e7f4 | 0x12e7c4 | 0x12f498 |
| cold `Mission::getType` | 0x12ebdc | 0x12eb98 | 0x12f8d8 |
| enemy `KIPlayer::getType` | 0x12ee90 | 0x12eeac | 0x12ea20 |
| gas `getSparks` | 0x12f460 | 0x12f454 | 0x12efd4 |
| asteroid `isMinable` | 0x12fb3e | 0x12fb5a | 0x12fb12 |
| music `play` tail | 0x1302d2 / 0x130698 | 0x1302d4 / 0x13069c | 0x130286 (merged) |

With the 80:1 flag removed, A+B+C's landmark loop moves to `0x12e728`
(original `0x12e7f4`) and the enemy loop to `0x12ee14` (original `0x12ee90`) —
closer than the 80:1 variant, but still not the original order, and the cold
`Mission::getType` block stays at `0x12f8d4`.

## 7. Conclusion

The remaining 26 % is **not missing code and not a wrong constant/callee**:

* A+B+C is semantically exact: the `bl`/`blx` multiset matches the original's
  344 calls, with only two tail-merge differences.
* The baseline's 9908 B is a local optimum: its block layout is within
  ~0x44 bytes of the original throughout, and its music `play` tail already
  matches the original's last three call addresses exactly.
* The correct source shape re-rolls the block layout: the landmark loop moves
  from `0x12e7c4` to `0x12f498` (80:1) or `0x12e728` (no flag), the cold
  `missionBlocksPlanets` block moves to `0x12f8d8`, and the 72-byte cold tail
  disappears. That is the 124-byte size deficit.
* The first instruction-level divergence is the prologue: original
  `vpush {d8-d13}` and `str r3,[sp,#100]`; baseline `vpush {d8-d11}` and
  `str r3,[sp,#112]`. A+B+C restores `vpush {d8-d13}` (the `inSight` reorder
  defers the crosshair `vcvt`), but the stack-slot colouring still differs.

Next pass: reproduce the original's cold chain *without* the duplicate
autopilot block — force the `missionBlocksPlanets` computation and the
`field_0x14 == 0` block to stay in the `0x12ebda..0x12ec24` range, then re-apply
A+B+C under the full gate. The `field_0x14 == 0` block can be kept out-of-line
with a faithful `else if` shape only if the surrounding block placement is
already right; `__builtin_expect(field_0x14 == 0, 0)` alone gives 9788 B
(not 9908) and 61.0 %.

## 8. Evidence locations

Scratch (not committed): `/dev/shm/w4202/` — `Radar.base.cpp`,
`Radar.abc.cpp`, `abc.diff`, `orig.txt`, `oursBase.txt`, `oursABC.txt`,
`oursABCNoFlag.txt`, `oursNoFlag.txt`, `decomp_radar.txt`,
`radar_decomp_body.txt`, `blockdiff.py`, `variant.py`, `sweep_weights.sh`,
`sweep_abc.sh`.
