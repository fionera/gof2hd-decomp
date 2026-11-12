# `Level::createMission` — wave 191 / w13502 (pass 2)

Follow-up to `level-createmission-wave191-w13502.md` (case-2 convoy loot
order + non-volatile `offset`, raw 2964 -> 2725).  Branch
`wave191b-w13502`, base `692599753`.

## Result — landed

```text
                                     size            raw    aligned  insn
base 692599753                       17172/17172     2725    871     6206/6197
landed                               17172/17172     2669    865     6206/6197
```

Raw **2,725 -> 2,669** (-56), aligned **871 -> 865** (-6), size exact
17,172 B.  `.text` diff **71,564 -> 71,508** (-56); `.ARM.extab` 577
unchanged; `.rodata` 485 unchanged; build-id 20 unchanged.  `.rel.plt`
byte-identical, 27/27 headers.  `check`: **RATCHET PASS** (byte 4507,
linked 4507, imports 207).

## The change: case-2 escort-loop call order

The original's first (escort) loop, after `setPosition`, calls
(`0xb183a..0xb187e`):

```
0xb1848  blx  Player::setAlwaysEnemy(bool)
0xb185a  blx  Route::clone()
0xb1862  blx  KIPlayer::setRoute(Route*)
0xb1870  blx  KIPlayer::getRoute()
0xb1876  blx  Route::reachWaypoint(int)
```

i.e. `setAlwaysEnemy`, `setRoute(clone)`, `getRoute()->reachWaypoint(0)`.
The source had `reachWaypoint` first, then `setRoute`, then
`setAlwaysEnemy`.  Reordering the three statements to the binary's order is
the whole patch (semantically the clone is now the route that reaches
waypoint 0, which is what the binary does).

## Measurements (size 17,172 unless noted)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | 2,725 | 871 | 17,172 |
| **escort order setAlwaysEnemy/setRoute/reachWaypoint (landed)** | **2,669** | **865** | **17,172** |
| landed + drop `enemies` local in loot loop | 10,000 | 889 | 17,180 |
| drop `enemies` local in loot loop alone | 10,056 | 895 | 17,180 |
| landed + `convoyShip` local in loot loop | 10,312 | 860 | 17,164 |

## Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13502
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
```

Expected: `raw=2669 aligned=865 size=17172/17172`.
