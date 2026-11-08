# LevelScript::process — wave 148 / w9404: case-157 state-5 strong-arm
# placement, and the real block that precedes it

Status: **landed a small source fix; the 0x6ae block-placement shift is not
reachable from `__builtin_expect`**.  Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**,
size-exact.  Owned TU `src/game/world/LevelScript.cpp`.  Worktree `wt/w9404`,
base master `cbe3b5f7f` (wave 147).  Scratch in `/dev/shm/w9404/`.

```
                         baseline (HEAD)   this pass
process raw diffbytes    54,913            54,912
align_diff TOTAL         15,740            15,740
orig / ours size         57232 / 57232     57232 / 57232
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
RATCHET                  PASS              PASS
```

## 1. The block before the strong arm is NOT the state-4 completion block

w9303 identified `orig 0x144960..0x144a12` as the "case-157 state-4 completion
block".  That is wrong.  The block is the **case-154 state-9 completion**
(source lines 4686-4712, `} else if (m_nState == 9)` of mission 154):

```
orig 0x144900: setAlwaysFriend(false), setAlwaysEnemy(true), setEnemies(nullptr),
               setVulnerable(true), setFixed(false), resetGunDelay(),
               setLookAtCam(false), setTarget(player->geometry),
               setFreeze(false), setSpeed(2.0f), setComputerControlled(false),
               setVulnerable(true), setPosition(0,0,90000),
               m_pHud->visible=1, m_pRadar->enabled=1, resetCamera(),
               forceUpdate(delta,false), m_bCinematicBreak=0,
               if (field_0xaa) setAutoTurret(true),
               m_nScriptTimerA=0, m_nState++
```

The case-157 state-4 completion block is elsewhere: `orig 0x1419ee..0x141b56`
(ends `movs r0,#5; b.w 0x1438d4`), `ours 0x14177e..0x1418d6`.

So the layout question is the relative order of the **case-154 state-9
completion block** and the **case-157 state-5 strong arm**:

```
orig: [case-154 s9 loop][case-154 s9 completion 0x144900..0x144a12]
      [strong arm 0x144a14..0x144a2c][state-5 timer 0x144a2e..]
ours: [case-12 s12 completion 0x144212..0x144362]
      [strong arm 0x144366..0x144384][state-5 timer 0x144388..]
      ... [case-154 s9 loop 0x1449dc..][case-154 s9 completion 0x144a38..0x144b44]
```

i.e. the strong-arm chain and the case-154 chain are swapped.  The shift is
`0x6ac` at the strong arm, `0x130` at the case-154 completion.

## 2. Block-placement dump: the strong arm is placed right after the case-12 block

`clang++ -mllvm -print-after-all -mllvm -filter-print-funcs=_ZN11LevelScript7processEi`
(the `Branch Probability Basic Block Placement` dump) gives the MBB order.
Relevant blocks (post-placement index, `bb.N`, `%ir-block.N`):

```
1189 bb.1197 ir.7289   case-12 state-12 completion (getLandmarks, SetAnimationState(3)/(1), play(160))
1190 bb.1198 ir.14291  strong arm init (m_bCinematicBreak=1 store in baseline)
1191 bb.1199 ir.14297  strong-arm loop body (isDead)
1192 bb.1200 ir.14293  strong-arm loop header (cmp r5,#22)
...
1238 bb.1246 ir.13691  case-154 state-9 completion (setAlwaysFriend+setSpeed)
```

Pre-placement (`Post RA top-down list latency scheduler`) the case-154 block is
at index 1137 and the strong arm at 1204, i.e. case-154 is *before* the strong
arm, as the IR order (case 154 < case 157) would suggest.  The block-placement
pass moves the strong-arm chain in front of the case-154 chain.  This is a
branch-weight decision: with PGO the original keeps the case-154 chain first.

## 3. `__builtin_expect` sweeps (all size-exact unless noted)

Applied on top of the pristine source (or on top of the landed `B--T`
variant where noted).  `sa` = strong-arm address, `c154` = case-154 s9
completion address.

| variant | size | raw | align | sa | c154 |
|---|---:|---:|---:|---|---|
| baseline | 57232 | 54913 | 15740 | 0x14436c | 0x144a6a |
| `expect(isRunningHDonWeakDevice,1)` on state 5 | 57220 | 55436 | 15966 | 0x145bc4 | 0x144a1e |
| `expect(isRunningHDonWeakDevice,0)` on state 5 | 57224 | 55426 | 15950 | — | — |
| `expect(m_nScriptTimerA>=2001,1)` case-154 s9 | 57232 | 54913 | 15740 | 0x14436c | 0x144a6a |
| `expect(m_nScriptTimerA>=2001,0)` case-154 s9 | 57240 | 55325 | 15744 | 0x14434a | 0x144a46 |
| `expect(messages[7]->isOver(),0)` case-154 s9 | 57232 | 55033 | 15775 | 0x14436e | 0x144a6a |
| `expect(m_nScriptTimerA>=6500,1)` case-157 s12 | 57228 | 55210 | 15751 | 0x144362 | 0x144a5e |
| `expect(m_nScriptTimerA>=6500,0)` case-157 s12 | 57228 | 54833 | 15738 | 0x14436a | 0x144a66 |
| `expect(messages[3]->isOver() && ...>=18001,1)` s4 | 57228 | 54833 | 15738 | 0x14436a | 0x144a66 |
| `expect(messages[3]->isOver() && ...>=18001,0)` s4 | 57240 | 55447 | 15880 | 0x144292 | 0x14498e |

No `__builtin_expect` shape reproduces the original's chain order at 57232 B.
`expect(isRunningHDonWeakDevice,1)` makes the strong arm *too* cold: it moves
to `0x145bc4`, past the post-switch tail.  The original's PGO weight is
intermediate and cannot be expressed by clang 7's 1:2000 `__builtin_expect`
weight.

## 4. Two real source differences found and (partly) landed

The original's state-5 chain differs from ours in two places:

1. **The strong arm has no `m_bCinematicBreak = 1`.**  Orig `0x144a14` is
   `ldr r6,[sp,#96]; movs r5,#11; movs r4,#0; b.n ...` — no store.  The
   original sets `m_bCinematicBreak = 1` in the **state-5 destroyed block**
   (`0x144ada: strb r1,[r4,#17]` with `r1=1`, immediately before
   `m_pHud->visible = 0`).
2. **The state-5 timer block resets the timer.**  Orig `0x144a4e` is
   `movs r1,#0; strd r1,r1,[r0]` (`m_nScriptTimerB = 0`) before the
   `field_0x140` toggle.  The source had no reset.

Landed (`B--T`): move `m_bCinematicBreak = 1` from the strong arm to the
destroyed block, and add `m_nScriptTimerB = 0` to the timer block.  This keeps
the function at exactly 57232 B and is a genuine semantic match with the
original.

```
                         baseline   landed (B--T)
process raw diffbytes    54,913     54,912
align_diff TOTAL         15,740     15,740
size                     57,232     57,232
```

The other two edits of the fully-correct shape (`m_pHud->visible = 0` belongs
in the destroyed block, not the timer block; `int destroyed = 0;`) were
measured but are **not** landed because they move the size off 57232 or make
the raw metric worse:

| variant | size | raw | align |
|---|---:|---:|---:|
| `B---` (move m_bCinematicBreak only) | 57228 | 54830 | 15736 |
| `B-D-` (+ `int destroyed = 0;`) | 57232 | 54913 | 15741 |
| `BH--` (also move m_pHud->visible) | 57228 | 54833 | 15738 |
| `BHD-` (+ `int destroyed = 0;`) | 57232 | 54910 | 15743 |
| `B--T` (landed) | 57232 | 54912 | 15740 |
| `BHDT` (fully correct) | 57224 | 55279 | 15746 |

`B---` is the best raw (54,830, −83) but its −4 B size change breaks the
global gate (`byte_exact` drops 4507 → 4329 because every following function
shifts).  The `+4` companion that would keep `B---` at 57232 B is the
`movs r4,#0` (`int destroyed = 0;`), but that edit re-misaligns the state-5
chain and gives back the whole raw win.

## 5. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9404/fdiff.py _ZN11LevelScript7processEi
python3 /dev/shm/w9404/align_diff.py _ZN11LevelScript7processEi
cmake --build cmake-build-match --target check -j6
```

`/dev/shm/w9404/probe.py` locates the weak arm, strong arm, case-154 s9 loop
and case-154 s9 completion in a built `.so` by instruction pattern.

## 6. Open questions

1. The chain-order swap (strong-arm chain vs case-154 chain) is a PGO
   branch-weight effect.  clang 7's `__builtin_expect` only produces 1:2000
   weights, which overshoots.  A source shape that gives an intermediate
   weight (e.g. `__builtin_expect` on a *different* branch that dominates the
   same chain) has not been found.
2. The strong arm is 30 B in ours vs 28 B in the original: ours reloads
   `[sp,#96]` (enemies) inside the loop and keeps `this` in `r8`, the original
   loads `r6=[sp,#96]` once and reloads `this` after the loop.  This is a
   register-allocation difference (+2 B) and is not reachable from the source
   edits tried.
3. The `autoTurret` PLT anchor (16 B) and the `m_pHud->visible` placement are
   still open; see the wave-141/147 notes.
