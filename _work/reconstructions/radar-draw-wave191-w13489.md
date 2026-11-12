# Radar::draw wave 191 (w13489) — enemy-loop guard: original CAB order
# (`isAutoPilot` -> `isDockedToDockingPoint` -> exclude-flag -> enemyHit/field_0x24 ->
# wreck/isDead`); aligned 2,051 -> 2,027

**Status:** landed, 2026-11-12. Worktree `wt/w13489`, branch `wave191-w13489`,
base `master` `f325e2d87`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave190-w13493.md` (asteroid-lock tail, base 6,058) and
`radar-draw-wave190-w13404-music.md` (music-tail operand order).

## 0. The change

The per-enemy guard (`Radar.cpp:833`) evaluated its five operands in the source order

```
A  !isAutoPilot()
B  (!enemyHit || field_0x24)
C  (!wreck || isDead())
D  !isDockedToDockingPoint()
E  !(countsAsEnemyExcludeFlag != 0 && !wreck)
```

i.e. `A && B && C && D && E`.  The original binary evaluates them **A -> D -> E ->
B -> C**:

```
12f02e  getPlayer / 12f030 isAutoPilot       ; A
12f038  bne 12f15c
12f03e  getPlayer / 12f040 isDockedToDockingPoint ; D
12f04a  bne 12f15c
12f04e  ldrb r0,[r8,#61]   ; countsAsEnemyExcludeFlag
12f052  cmp r0,#0
12f054  it ne / 12f056 movne r0,#1
12f058  ldr r1,[sp,#152]   ; wreck
12f05a  mvns r1,r1
12f05c  tst r1,r0          ; E: excludeFlag & !wreck
12f05e  bne 12f15c
12f060  ldr r0,[sp,#88]    ; enemyHit
12f062  lsls r0,#31
12f064  beq 12f06e
12f066  ldrb r0,[r8,#36]   ; field_0x24
12f06a  cmp r0,#0
12f06c  beq 12f15c        ; B: !enemyHit || field_0x24
12f06e  ldr r0,[sp,#152]   ; wreck
12f070  cbz r0,12f086
12f072  mov r0,r8
12f074  blx isDead
12f078  cmp r0,#0
12f07a  beq 12f15c        ; C: !wreck || isDead
```

The `&&` short-circuit order is semantically irrelevant (all five predicates are pure
getters/field reads), so the source is reordered to the original's evaluation order:

```cpp
-            if (!this->level->getPlayer()->isAutoPilot() &&
-                (!enemyHit || __builtin_expect((enemy->field_0x24), 0)) &&  // lint: arm_builtin
-                (!wreck || enemy->isDead()) &&
-                __builtin_expect(!this->level->getPlayer()->isDockedToDockingPoint(), 1) && // lint: arm_builtin anchor: Radar::draw
-                !(enemy->countsAsEnemyExcludeFlag != 0 && !wreck)) {
+            if (!this->level->getPlayer()->isAutoPilot() &&
+                __builtin_expect(!this->level->getPlayer()->isDockedToDockingPoint(), 1) && // lint: arm_builtin anchor: Radar::draw
+                !(enemy->countsAsEnemyExcludeFlag != 0 && !wreck) &&
+                (!enemyHit || __builtin_expect((enemy->field_0x24), 0)) &&  // lint: arm_builtin
+                (!wreck || enemy->isDead())) {
```

The `__builtin_expect` hints stay attached to their own operands (`D` is the `,1` anchor,
`B` is the `,0` hint), matching the original's branch polarity.

| metric | base | after |
|---|---:|---:|
| raw same-address diff | 6,058 | 6,060 |
| instruction-index alignment | 2,051 | **2,027 (-24)** |
| function size | 9,908 / 9,908 | 9,908 / 9,908 |
| instruction count | 3,542 / 3,532 | 3,542 / 3,532 |
| `.text` differing bytes (whole `.so`) | 71,811 | 71,813 (+2) |
| `.ARM.extab` differing bytes | 575 | 575 (no growth) |
| `.ARM.exidx` differing bytes | 0 | 0 |
| `.note.gnu.build-id` | 20 | 20 |
| `.rel.plt` | byte-identical | **byte-identical** |

`RATCHET PASS`: byte_exact 4,507 / linked_exact 4,507 / size 4,524 / data 843 /
imports 207 / lint CLEAN / drift 0 / parity 5,370.

The +2 raw is a same-address shift artefact: the reordered guard now emits in the
original's order but the surrounding enemy block still sits at a different address, so
same-address bytes cannot line up. The shift-invariant aligned metric — the progress
measure for this function — improves by 24 instructions. This is the first pass to touch
the enemy-guard operand order (the documented cells cover the music tail, the asteroid
lock, the gas-cloud box, the planet loop and the blip call orders).

## 1. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6060 aligned= 2027 size=9908/9908 insn=3542/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

## 2. Negative results measured this pass (reverted)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | 6,058 | 2,051 | 9,908 |
| enemy-lock blip: `blip->setFrame` before `setRefPixelPosition` | 6,062 | 2,027 | 9,908 |
| planet-blip frame `(timer / 0x1b8) * frames` reassociation | 6,063 | 2,053 | 9,908 |
| gas-cloud 24000-box `egoPos - targetPos` sign flip | 6,061 | 2,051 | 9,908 |
| gas-cloud block reordered to the original (targetPos before DrawImage2D) | 6,148 | 2,037 | 9,908 |
| `(current != 151 && expect(alwaysEnemySeen,0))` | 6,062 | 2,051 | 9,908 |
| enemy blip order + enemy guard order | 6,064 | **2,003** | 9,908 |
| enemy blip order + guard order + planet frame | 6,069 | 2,003 | 9,908 |

The blip-order fix (`blip->setFrame` first, as in every other blip block of the original)
is a real shape win on aligned (-24) but costs +4 raw and one extra instruction
(3,543/3,532), so it is held for a later pass rather than landed here.

Also measured and neutral: `int`/`unsigned`/`long` `campaign`, `bool` `alienOrbit`,
`int` `planetHit`/`landmarkHit`/`enemyHit`/`gasHit` (all of the latter change size),
`int hull`/`int hullInt` for the hull bar (clang rematerializes the `vcvt` either way),
`barWidth` before `DrawRegion2D` (9,912 B, gate-fatal), and all 120 permutations of the
enemy-guard operand order (none beats 6,058 raw; `A D E B C` is the aligned-best).
