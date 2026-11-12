# LevelScript::process — wave 191 / w13479: rebase of w13338's eight d2 rotations

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree
`/home/debian/GalaxyOnFire2/wt/w13479`, branch `wave191-w13479`, base master
`f325e2d87`.  Scratch `/home/debian/GalaxyOnFire2/scratch/w13479`.

Task: rebase and measure, one at a time, the eight 3-statement rotations from
w13338's candidate commit `2b1318a65` (note
`levelscript-process-literal-branch-wave189-w13338.md`, section G) onto current
master.  That commit is **not** an ancestor of master (`git merge-base
--is-ancestor 2b1318a65 HEAD` fails), but seven of the eight rotations have
since been landed independently by other waves.

## Baseline on current master

```
process raw diffbytes    51,679
process aligned insns    22,003
orig / ours size         57,232 / 57,232   (size-exact)
orig / ours insns        20,830 / 20,726
.text diffbytes          71,811
.ARM.extab diffbytes     575
.ARM.exidx diffbytes     0
.rel.plt                 md5 289b2a069f8504d1e8a3919e43915e04 (29,392 B, byte-identical)
.note.gnu.build-id       20
total differing bytes    72,406
byte_exact / linked      4507 / 4507
RATCHET                  PASS
```

## Landing status of the eight rotations

The three statements of each rotation were located in the current source and
their relative order compared with w13338's "after" order.  Seven are already
in the after order; only the case-70 rotation (line 2492 in w13338's base) is
not.

| w13338 line | rotation (after order) | current master | status |
|---:|---|---|---|
| 2281 | `setTarget` + `setComputerControlled` before `setLookAtCam(false)` | 2281-2283: target, CC, LA | landed |
| 2326 | `setFreeLookMode` before `setTurretMode` + `resetCamera` | 2326-2329: FL, enableFP, TM, RC | landed |
| 2441 | `setTarget` before `setLookAtCam(true)` + `setActive` | 2440-2443: target, LA, camPos, active | landed |
| 2492 | `setLookAtCam(true)` before `setFreeLookMode`/`setComputerControlled` | 2492-2505: **active, CC, LA, radar, ..., FL** | **not landed** |
| 3188 | `setVisible(false)` + `setVulnerable(true)` before `setActive(false)` | 3189-3191: visible, vulnerable, active | landed |
| 3343 | `resetCamera` + `setFreeze(false)` before `resetGunDelay()` | 3344-3347: RC, freeze, vulnerable, resetGunDelay | landed |
| 4752 | enemy0 `setVulnerable(true)` before enemy1 `setAlwaysEnemy` + enemy0 `setEnemies(nullptr)` | 4753-4755: vulnerable, alwaysEnemy, enemies | landed |
| 5103 | timerA/timerB before `setVisible(false)` | 5105-5107: timerA, timerB, visible | landed |

The case-70 block was subsequently rearranged by `e70dc09ce5` ("d4 pass: nine
independent pairs"), which moved `setActive(true)` to the front of the group and
`setFreeLookMode(false)` to the end.  The w13338 rotation therefore has to be
re-adapted, not re-applied literally.  (This is the "landed d4 pair" that must
not be duplicated.)

## Site-4 rebase measurements (case 70, `0x1399a2`)

Original order (from the binary, `0x1399c6`..`0x139aba`): `setTurretMode`,
`resetCamera`, `setFreeLookMode`, `enableFirstPersonCam`,
`hideShipForFirstPersonCameraView`, `stopShooting`, `cinematicBreak=1`,
`hud->visible=0`, `radar->enabled=0`, `setComputerControlled`, `setLookAtCam`,
`setActive`, `setTarget`, `getPosition`, `getDirection`, `camPos += 3000*dir`,
`camPos.x/y/z` adds, `setPosition`, `forceUpdate`, `freeze=1`, `m_nState=1`.

Current master order: `resetCamera`, `setTurretMode`, `enableFirstPersonCam`,
`hideShip`, `stopShooting`, `cinematicBreak=1`, `hud->visible=0`, `setActive`,
`setComputerControlled`, `setLookAtCam`, `radar->enabled=0`, `forceUpdate`,
`setTarget`, `getPosition`, `camPos.x`, `camPos.y`, `getDirection`,
`camPos.z`, `camPos += 3000*dir`, `setFreeLookMode`, `setPosition`, `freeze=1`,
`m_nState=1`.

All measurements are `measure_w11901.py` (raw) with size and `.rel.plt` md5
checked.  Base raw **51,679**, size 57,232/57,232, `.rel.plt`
`289b2a06…`.

### Exact in-place permutation of the three rotation statements

Slots: `CC` @2492, `LA` @2493, `FL` @2503 (current order `CC, LA, FL`).

| order | raw | size |
|---|---:|---|
| `CC, LA, FL` (base) | **51,679** | 57,232 |
| `LA, CC, FL` (literal w13338 move) | 51,683 | 57,232 |
| `LA, FL, CC` (full after order) | 51,686 | 57,232 |
| `CC, FL, LA` | 51,689 | 57,232 |
| `FL, CC, LA` | 51,688 | 57,232 |
| `FL, LA, CC` | 51,681 | 57,232 |

No permutation beats the base.  Moving the three statements as a block to the
`CC` slot (`LA, FL, CC` together) gives 51,695.

Two of the other seven triples have a **tie** (not a win) in the same
in-place permutation sweep: site 3 (`setTarget`, `setActive`, `setLookAtCam`
around the `camPos` declaration) and site 8 (`timerB`, `timerA`,
`setVisible`).  Applying both ties together still measures 51,679, so neither
is a stepping stone.

### Neighbouring statements

The four-statement group `[setActive, setComputerControlled, setLookAtCam,
radar->enabled=0]` was exhaustively permuted (24 orders).  The base order
`active, CC, LA, radar` is the unique best at 51,679; the next best is
`LA, CC, active, radar` at 51,682.  Any order with `radar` before `setActive`
breaks size-exact (57,224, `.text` diff 352,445), so `radar` must stay after
the `active/CC/LA` group.

`setFreeLookMode(false)` can be inserted at any of the five positions around
`[active, CC, LA, radar]` with **no** raw change (all 51,679) — the compiler
schedules it independently of that group.

Faithful-original single moves, each measured alone:

| move | raw | size |
|---|---:|---|
| `setFreeLookMode` after `resetCamera` (original position) | 51,707 | 57,232 |
| `radar->enabled=0` before `setComputerControlled` | 54,047 | **57,224** |
| `setActive` after `setLookAtCam` | 51,687 | 57,232 |
| `forceUpdate` after `setPosition` | 51,700 | 57,232 |
| swap `resetCamera` / `setTurretMode` | 51,680 | 57,232 |

All are >= base or break size-exact.

## Result

No size-exact, `.rel.plt`-identical raw win from rebasing the eight rotations
onto current master: seven are already landed and the eighth (case 70) is
locally optimal in every tested permutation and neighbour placement.  The
source is unchanged; baseline stays **51,679 / 22,003 / 57,232**, `.text`
71,811, `.ARM.extab` 575, `.rel.plt` `289b2a06…`, RATCHET PASS.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13479
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=51679 size=57232/57232 text=71811 relplt=289b2a069f8504d1e8a3919e43915e04
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 51679 aligned=22003 size= 57232/57232 insn=20726/20830
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
```

Scratch harnesses (`sweep_perm.py`, `sweep_triple.py`, `sweep4.py`,
`sweep_fl.py`, `sweep_radar.py`, `perm_res.jsonl`, `orig.dis`) live in
`/home/debian/GalaxyOnFire2/scratch/w13479/`; they are scratch, not tracked.
