# LevelScript::process — wave 190 / w13384: seven d1 follow-up reorders

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13384`, branch
`wave190-w13384`, base master `dd8cf7a29` (raw **51,792**, `.text` 73,289,
`.ARM.extab` 603).  Scratch `/home/debian/GalaxyOnFire2/scratch/w13384`.

Task: largest remaining residual.  This pass first landed the seven
independent adjacent-statement (`d=1`) candidates that the w13369 sweep had
left as "follow-up candidates" (it landed only two of its fourteen).  All
seven are still present at the same source lines on the wave-190 harvest and
each measures **-1 raw** alone on the current body.

## Landed changes (all seven compose)

| # | source lines | swap | receiver independence |
|---:|---|---|---|
| 1 | 1491/1492 | `lvl->getPlayer()->setCollide(true)` before `m_bStartSequenceOver = 1` | flag store vs collide call |
| 2 | 1609/1610 | `m_pHud->visible = 0` before `m_pRadar->enabled = 0` | disjoint objects |
| 3 | 1941/1942 | `(*enemies)[0]->shipGroup = 1` before `player->setFreeLookMode(false)` | field store vs call |
| 4 | 2532/2533 | `camPos.x += -600` before `camPos.y += 800` | disjoint members |
| 5 | 3049/3050 | `wcamera->setTarget(leader)` before `Canvas = Globals::Canvas` | call vs global load |
| 6 | 3536/3537 | `m_pRadar->enabled = 0` before `m_pHud->visible = 0` | disjoint objects |
| 7 | 3882/3883 | `wcamera->setTarget(marker)` before `sound->play(14, …)` | disjoint receivers |

None of the moved statements reads state written by its partner, so each
reorder is semantics-preserving.

## Measured result

```
                         base (HEAD)      this pass
process raw diffbytes    51,792           51,785     (-7)
process aligned insns    22,211           22,208
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
.text diffbytes          73,289           73,282     (-7)
.ARM.extab diffbytes     603              603        (size/base exact 156,312 B @ 0x1a3b78)
.ARM.exidx diffbytes     0                0          (byte-identical)
.rodata / .data / .bss   0 / 0 / 0        0 / 0 / 0
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical, 29,392 B)
.note.gnu.build-id       20               20
file size                2,187,328        2,187,328  (exact)
```

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13384
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 51785 aligned=22208 size= 57232/57232
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=51785 size=57232/57232 text=73282 relplt=289b2a069f8504d1e8a3919e43915e04
```

## Current-master harvest (wave 190)

The w13384 branch was based on `dd8cf7a29` (raw 51,792). Six of its seven reorders
were already landed via w13369; only the new `camPos.y += 800.0f` after
`m_pCamera->setPosition(camPos)` hunk was still missing. Re-applied that hunk on
current master (`01f5edc5d`): LevelScript raw **51,786 -> 51,785** (-1), aligned
22,208, size 57,232/57,232. `.text` 73,274 -> 73,273 (-1), `.ARM.extab` 606
unchanged, total 73,900 -> 73,899 (-1). `check` green, RATCHET PASS.
