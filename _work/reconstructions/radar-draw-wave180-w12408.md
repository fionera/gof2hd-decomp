# Radar::draw wave 180 (w12408) — music-tail select + alwaysEnemySeen expect removal

**Status:** landed, 2026-11-11. Worktree `wt/w12408`, branch `wave180-w12408`,
base `master` `99e341e9b` (wave-179 harvest). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave179-w12307.md` (the current base shape), and
`radar-draw-wave178-w12208.md` (the earlier music-tail select work, whose
if/else form was reverted by the w12207 recovery commit `5cc04f337`).

## 0. Baseline and landed result

| metric | base | landed |
|---|---:|---:|
| function size | 9,908 | **9,908** |
| raw same-address diff | 6,407 | **6,399** (-8) |
| instruction-index alignment | 1,806 | **1,801** (-5) |
| instruction count | 3,542 ours / 3,532 orig | 3,542 / 3,532 |
| whole-`.so` `.text` differing bytes | 76,870 | **76,862** (-8) |
| `.ARM.exidx` differing bytes | 0 | 0 |
| `.ARM.extab` differing bytes | 709 (size/base exact) | **709** |
| `.rel.plt` | byte-identical (0) | byte-identical (0) |
| `.note.gnu.build-id` differing bytes | 20 | 19 |
| total differing bytes | 77,599 | **77,590** |
| `.rodata` / `.data` / `.dynsym` / `.dynstr` / `.hash` | 0 | 0 |

Gate after the change: byte_exact **4,507/4,524**, linked_exact
**4,507/4,524**, size-exact **4,524/4,524**, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift **0**, parity **5,370/5,370**. `RATCHET PASS`.

## 1. Landed change (two cells, net size 0)

```diff
@@ music tail, supernova mission-station jingle (line ~1331)
-                            Globals::sound->play(Globals::status->getCurrentCampaignMission() < 106 ? 2241 : 2242,
-                                                 nullptr, nullptr, 0.0f);
+                            if (Globals::status->getCurrentCampaignMission() > 105) {
+                                Globals::sound->play(2242, nullptr, nullptr, 0.0f);
+                            } else {
+                                Globals::sound->play(2241, nullptr, nullptr, 0.0f);
+                            }

@@ enemy music tail, last branch (line ~1291)
-                if (__builtin_expect((__builtin_expect(alwaysEnemySeen, 1)), 0)) {  // lint: arm_builtin
+                if (__builtin_expect(alwaysEnemySeen, 1)) {  // lint: arm_builtin
```

Both are semantics-preserving. The music-tail if/else is the same predicate
as `< 106 ? 2241 : 2242` (`> 105` selects 2242, else 2241); the original
source evidently used this form. The second cell only drops a redundant
outer branch-weight hint.

### 1.1 Size accounting

The if/else spelling is **+4 B** (9,912) on its own; the outer-expect removal
is **-4 B**. Together they are exactly 9,908. The `if_gt105` form alone is
the best raw shape measured (6,366) but is size-fatal at 9,912; the `rm31`
compensation gives 6,399 at 9,908.

## 2. Why the music tail matters

The base ternary emits (ours `13064c`):

```
ldr r0,[sp,#156] ; ldr r0,[r0,#0] ; blx getCurrentCampaignMission
movs r1,#0 ; cmp r0,#106 ; str r1,[sp,#0]
movw r1,#2242 ; it lt ; movwlt r1,#2241
```

The original (`13064a`) emits the exact `ite` select:

```
ldr r0,[sp,#156] ; ldr r0,[r0,#0] ; blx getCurrentCampaignMission
mov r1,r0 ; cmp r1,#106 ; mov.w r1,#0 ; ldr.w r0,[r9] ; str r1,[sp,#0]
ite lt ; movwlt r1,#2241 ; movwge r1,#2242
```

The explicit if/else makes clang emit the original `ite lt; movwlt 2241;
movwge 2242` at `130660`; the base ternary emits `it lt; movwlt 2241` with a
pre-set default of 2242. The landed shape also aligns the whole tail block at
`130648` (the base had it at `13064c`).

## 3. Variant tables (all measured on this worktree)

### 3.1 Music-tail spelling (size 9,908 unless noted)

| spelling | raw | size |
|---|---:|---:|
| ternary `< 106 ? 2241 : 2242` (base) | 6,407 | 9,908 |
| `if (> 105) 2242 else 2241` | **6,366** | **9,912** |
| `if (<= 105) 2241 else 2242` | 6,370 | **9,912** |
| `if (< 106) 2241 else 2242` | 6,370 | **9,912** |
| `if (>= 106) 2242 else 2241` | 6,366 | **9,912** |
| ternary `> 105 ? 2242 : 2241` | 6,408 | 9,908 |
| ternary `>= 106 ? 2242 : 2241` | 6,408 | 9,908 |
| `int musicId = < 106 ? 2241 : 2242; play(musicId)` | 6,400 | 9,908 |
| `int musicId; if/else; play(musicId)` | 6,401 | 9,908 |

### 3.2 Compensating the +4 B if/else

| compensation | raw | size |
|---|---:|---:|
| none | 6,366 | 9,912 |
| remove outer `expect(alwaysEnemySeen, 0)` (landed) | **6,399** | **9,908** |
| flip `expect(alwaysEnemySeen, 0->1)` at the same site | 6,464 | 9,908 |
| remove `expect(field_0x18 != 0, 1)` (planet frame) | 6,480 | 9,908 |
| remove `expect(current != 151 && alwaysEnemySeen, 0)` | 7,283 | 9,908 |

All 33 single expect removals and flips were swept on top of the if/else;
`rm31` is the only size-exact compensation and the best raw one. On top of
`if/else + rm31`, all 33 further expect removals/flips are neutral or worse
(best neutral 6,399), and all 197 plain `if` conditions wrapped in
`expect(...,0)`/`expect(...,1)` are neutral or worse (best 6,399).

### 3.3 Dock guard / `+8 B` site (the brief's primary angle)

The current `expect(field_0x1a8, 0)` is the +8 B landing-pad site. Making the
dock guard plain restores the original inline body but is **size-fatal**:

| shape | raw | size |
|---|---:|---:|
| `expect(field_0x1a8, 0)` (base) | 6,407 | 9,908 |
| plain `if (field_0x1a8)` | 6,523 | **9,900** |
| plain + planet sound `expect(!lockConfirmed, 0)` (+8) | 6,478 | 9,908 |
| plain + station sound `expect(!lockConfirmed, 0)` (+8) | 6,459 | 9,908 |
| plain + best plain-if `expect` (+8) | 6,459 | 9,908 |

A 432-variant sweep of the whole planet-lock block (planet-guard spelling ×
`missionBlocksPlanets` form × dock-guard spelling × sound-guard spelling ×
all six statement orders of store/dock/sound) found **no shape below 6,407**;
the base is a local optimum for that block. `bool missionBlocksPlanets`
restores the original `orrs` guard but costs +8 B (9,916) and is raw-worse
(6,874). The `else`-arm form of the rare `missionBlocksPlanets` guard compiles
to exactly the base bytes (6,407 / 9,908).

### 3.4 shipGroup / wreck branches

| shape | raw | size |
|---|---:|---:|
| base (`wreck; field_0x75=0; update`) | **6,407** | 9,908 |
| `wreck; update; field_0x75=0` | 6,408 | 9,908 |
| `update; wreck; field_0x75=0` | 6,409 | 9,908 |
| `bossSeen = shipGroup == 10 ? true : false` | 6,407 | 9,908 |
| `if/else` bossSeen | 6,407 | 9,908 |
| `if (shipGroup == 10) bossSeen = true` | 9,200 | **9,980** |
| `bossSeen = true` | 8,511 | **9,900** |

The base `bossSeen = (shipGroup == 10)` compiles to a `subs/clz/lsrs`
branchless sequence in `r5`; the original uses `it eq; moveq r6,#1`. Every
source form that forces the `ite` form changes size, so the base stays.

### 3.5 Other cells re-confirmed neutral

Planet guard `!(timer <= x)`, `campaign > 1`, `!(campaign < 2)`, the
`!(turretMode || landmarkHit)` and `!(otherLock || planetHit || turretMode)`
forms, `field_0x14` truthiness, `otherLock` operand orders, and the
`isDockingToAsteroid`/`isDockingToDockingPoint` `||` fold are all exactly
6,407 / 9,908 (same bytes) or worse.

## 4. Remaining residual

Unchanged from wave 174/178/179: the original keeps `campaign` in `sl`
(`12e0f8 mov sl,r0`, `12e38a cmp.w sl,#2`) and later reuses `sl` for the
stations array, with the planet-loop index in `fp`; ours spills `campaign`
to `sp+0x90` and uses `sl` for the zero constant. That cascade is the
dominant remaining residual (the enemy bar block, the planet tail and the
stack-slot rotation all follow from it). The dock body is still out of line
(`bne.w 1305bc`) vs the original's inline body, held there by the +8 B size
requirement.

## 5. Reproduction

```sh
ulimit -v 12000000
cp /home/debian/GalaxyOnFire2/scratch/w12408/Radar.final_clean.cpp src/game/weapons/Radar.cpp
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
cmake --build cmake-build-match --target check -j2
cmake --build cmake-build-match --target drift -j2
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w12408/` —
`Radar.base.cpp` (base source), `Radar.final_clean.cpp` (landed source),
`harness.py` (build+measure), `gen_*.py` (variant generators),
`vpl.log` (432 planet-block variants), `vexp.log` / `vif.log`
(expect/if sweeps), `vmsw.log` / `vifgt2.log` / `vifgtif.log`
(music-tail sweeps), `ven.log` (enemy block), `secdiff.py` (section deltas).
