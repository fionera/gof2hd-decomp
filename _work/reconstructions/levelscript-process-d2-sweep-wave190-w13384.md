# LevelScript::process — wave 190 / w13384: d2 rotation sweep, seven wins

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13384`, branch
`wave190-w13384`.  Scratch `/home/debian/GalaxyOnFire2/scratch/w13384/h`.

A fresh full `d=2` rotation sweep (2,040 candidates, exact NDK r18b `-Oz`
compile + gold link, 4 workers) on the post-d1 body (raw 51,783) found 14
size-exact `-1..-2` cells.  Seven are independent and `.rel.plt`-clean; they
compose to `-8`.

## Landed changes

Source lines are 1-based; the sweep prints 0-based indices (source = +1).

| # | move (0-based) | source lines | statement order after | raw |
|---:|---|---|---|---:|
| 1 | down 2371,2373 | 2372-2374 | `resetCamera` / `m_bCinematicBreak = 0` / `forceUpdate` | -2 |
| 2 | down 2295,2297 | 2296-2298 | `routePoints[3]` init / `new Route` / `field_0x24 = 0` | -1 |
| 3 | up 2365,2367 | 2366-2368 | camera `setTarget` / `setLookAtCam` / enemy `setSpeed` | -1 |
| 4 | down 3551,3553 | 3552-3554 | `m_nScriptTimerA += delta` / `Vector zero` / camera `translate` | -1 |
| 5 | up 3880,3882 | 3881-3883 | `Vector zero` / marker `setPosition` / `sound->play(14)` | -1 |
| 6 | up 4836,4838 | 4837-4839 | camera `translate` / camera `setTarget` / `setVulnerable` | -1 |
| 7 | down 4853,4855 | 4854-4856 | `m_tempVec = getPosition` / `m_tempVec +=` / `m_pGeometry4->setPosition` | -1 |

All are reorders of statements with disjoint receivers / independent local
initialisers; each is `-1`/`-2` raw alone and size-exact.

## Rejected cells

* up 827,829 (`particleRenderBoolPtr->reset()` before `switchPlanetForIntro`)
  is `-1` but moves `.rel.plt` (md5 `75ad6587…`), rejected.
* up 888,890 / up 2694,2696 / up 4540,4542 / up 4602,4604 / down 4962,4964
  are read-after-write rotations of `m_tempVec`; up 3157,3159 reorders the
  `v_dirVecA` producer/consumer — all semantically unsafe.

## Measured result

```
                         base (HEAD)      this pass
process raw diffbytes    51,783           51,775     (-8)
process aligned insns    22,210           22,413
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
.text diffbytes          73,280           73,272     (-8)
.ARM.extab diffbytes     603              603        (size/base exact 156,312 B @ 0x1a3b78)
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical, 29,392 B)
file size                2,187,328        2,187,328  (exact)
```

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13384
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 51775 aligned=22413 size= 57232/57232
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=51775 size=57232/57232 text=73272 relplt=289b2a069f8504d1e8a3919e43915e04
```

## Current-master harvest (wave 190)

`807205a3c` applies the seven d2 rotations on master `a59477829`. Most were
already present from the w13369 follow-up harvest, so the net is raw
**51,748 -> 51,747** (-1), aligned 22,483 unchanged, size exact 57,232 B.
`.text` **73,006 -> 73,003** (-3, combined with the w13396 Status win);
`.ARM.extab` 603 and build-id 20 unchanged. `check` -> RATCHET PASS.
