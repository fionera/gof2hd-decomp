# LevelScript::process — wave 190 / w13384: fresh d1 sweep, two more reorders

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13384`, branch
`wave190-w13384`.  Scratch `/home/debian/GalaxyOnFire2/scratch/w13384/h`.

A fresh full `d=1` adjacent-statement sweep (1,530 unique independent pairs,
exact NDK r18b `-Oz` compile + gold link, 4 workers) on the post-harvest body
(raw 51,785) found seven size-exact `-1` cells.  Five were the known
read-after-write / `.rel.plt`-moving cells; the two new independent ones are
landed here.

## Landed changes

| # | source lines | swap | independence |
|---:|---|---|---|
| 1 | 3883/3884 | `wcamera->setTarget(marker)` after `Vector zero{0,0,0}` | call vs local init |
| 2 | 4219/4220 | `m_pCamera->setLookAtCam(false)` after `m_pRadar->enabled = 1` | disjoint receivers |

Each is `-1` raw alone on the current body; together `-2`.

## Measured result

```
                         base (HEAD)      this pass
process raw diffbytes    51,785           51,783     (-2)
process aligned insns    22,208           22,210
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
.text diffbytes          73,282           73,280     (-2)
.ARM.extab diffbytes     603              603        (size/base exact 156,312 B @ 0x1a3b78)
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical, 29,392 B)
file size                2,187,328        2,187,328  (exact)
```

## Sweep notes (for future passes)

The other five size-exact `-1` cells on this body:
* 829/830 `switchPlanetForIntro` / `switchSkyboxForIntro` — moves `.rel.plt`
  (PLT first-reference order), rejected;
* 890/891 `m_tempVec = player->getPosition()` / `m_tempVec += …` — RAW;
* 3735/3736 `*tmp = ref2->getUpVector()` / `*camPos += *tmp * 400` — RAW;
* 4542/4543 `m_tempVec += …` / `setPosition(m_tempVec)` — RAW;
* 4963/4964 `m_tempVec += …` / `chaser->setPosition(...)` — RAW.

Note the harness prints 0-based line indices; source lines are +1.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13384
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 51783 aligned=22210 size= 57232/57232
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=51783 size=57232/57232 text=73280 relplt=289b2a069f8504d1e8a3919e43915e04
```
