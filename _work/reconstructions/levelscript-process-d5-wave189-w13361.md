# LevelScript::process — wave 189 / w13361: distance-5..8 adjacent-statement sweep

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13361`, branch
`wave189-w13361`, base master `aafeff78d` (raw **51,842**, `.text` 73,565,
`.ARM.extab` 610).  Scratch `/home/debian/GalaxyOnFire2/scratch/w13361`.

Task: an independent LevelScript pass.  The wave-189 fleet had already swept
distance-1 (`w13330`/`w13352`), distance-2 rotations (`w13338`/`w13367`),
distance-3 (`w13355`), distance-4 (`w13355`) and distance-9..12 (`w13332`) on
the current body.  This pass fills the **distance-5..8** gap with a model-free
same-indent adjacent-statement swap sweep over the whole `process` body
(3,721 candidates, exact NDK r18b `-Oz` compile + gold link, 4 workers).

One size-exact, `.rel.plt`-identical, semantics-preserving raw win landed.

```
                         base (HEAD)      this pass
process raw diffbytes    51,842           51,835     (-7)
process aligned insns    20,998           21,022     (+24, not a gate metric)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,726  20,830 / 20,725
.text diffbytes          73,565           73,558     (-7)
.ARM.extab diffbytes     610              610        (size/base exact @0x1a3b78, 156,312 B)
.ARM.exidx diffbytes     0                0          (byte-identical)
.rodata / .data / .got   0 / 0 / 0        0 / 0 / 0
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical, 29,392 B)
.note.gnu.build-id       20               20
file size                2,187,328        2,187,328  (exact)
byte_exact               4507             4507
linked_exact             4507             4507
size-exact               4524/4524        4524/4524
data_exact               843/843          843/843
imports                  207              207
lint                     CLEAN ✓          CLEAN ✓
drift                    0                0
symbol parity            5370/5370        5370/5370
stub_zero_size           0                0
missing / wrong_type     0 / 0            0 / 0
extra                    0                0
RATCHET                  PASS             PASS
```

## Landed change

Mission-89 supernova intro (`Globals::status->getCurrentCampaignMission() == 89`),
state-1 arm, source lines 1271 and 1279 of the base:

```cpp
// before
(*m_pLevel->getEnemies())[0]->setPosition(m_tempVec);
m_nState = 2;
((AbyssEngine::Transform *) ...)->SetAnimationState(3, nullptr);
((AbyssEngine::Transform *) ...)->SetAnimationState(1, nullptr);
(*m_pLevel->getEnemies())[0]->geometry->moveForward(-10000.0f);
m_pGeometry6->setDirection(-(*enemies)[0]->geometry->getDirection(),
                           Vector{0.0f, 1.0f, 0.0f});
m_pGeometry6->setPosition((*m_pLevel->getEnemies())[0]->geometry->getPosition());
return 1;
// after
(*m_pLevel->getEnemies())[0]->setPosition(m_tempVec);
m_pGeometry6->setPosition((*m_pLevel->getEnemies())[0]->geometry->getPosition());
((AbyssEngine::Transform *) ...)->SetAnimationState(3, nullptr);
((AbyssEngine::Transform *) ...)->SetAnimationState(1, nullptr);
(*m_pLevel->getEnemies())[0]->geometry->moveForward(-10000.0f);
m_pGeometry6->setDirection(-(*enemies)[0]->geometry->getDirection(),
                           Vector{0.0f, 1.0f, 0.0f});
m_nState = 2;
return 1;
```

The two moved statements are independent of the intervening calls: the
`m_pGeometry6->setPosition` reads enemy-0's geometry position (not touched by
the transform/moveForward/setDirection calls), and `m_nState = 2` is a store
with no reader before `return 1`.  Measured alone on the wave-189 base: raw
**51,842 -> 51,835** (-7), size 57,232/57,232 exact, `.text` 73,565 -> 73,558,
`.ARM.extab` 610 unchanged, `.rel.plt` md5 unchanged.

## Sweep results (for future passes)

The model-free distance-5..8 sweep found **43** size-exact,
`.rel.plt`-identical raw wins (39 at -1, and the following larger cells):

| lines | distance | raw | statements |
|---|---:|---:|---|
| 969/974 | 5 | 51,821 | `startFade(true,255,5000)` / `SetCurrentApplicationModule(5)` — semantic (different `if` arms) |
| **1271/1279** | **8** | **51,835** | **landed** |
| 858/864 | 6 | 51,836 | `m_nScriptTimerA += delta` / `PaintCanvas *canvas = Globals::Canvas` |
| 2256/2261 | 5 | 51,837 | `player->freeze = 1` / `player->setFreeLookMode(false)` |
| 3835/3842 | 7 | 51,837 | `player->setTurretMode(false)` / `m_pCamera->enableFirstPersonCam(false)` |
| 3267/3272 | 5 | 51,838 | `m_pHud->visible = 1` / `Globals::sound->stop(...)` |
| 3533/3539 | 6 | 51,838 | `player->setVisible(false)` / `m_pRadar->enabled = 0` |
| 3406/3412 | 6 | 51,839 | `player->setFreeze(true)` / `m_pRadar->enabled = 0` |
| 4759/4765 | 6 | 51,839 | `m_pCamera->setLookAtCam(false)` / `resetCamera(m_pLevel)` |

The 969/974 cell is a **-21** raw win but swaps statements between two
different `if` arms (the original at `0x137e40` calls `startFade` inside the
`msg2->isOver() && m_nState == 0` arm), so it is not semantics-preserving.

An independent-model distance-5..8 sweep (`gen.py`/`sweep.py`, 3,012
candidates) found only the four state-9 cells at 828/833..836, all of which
move `switchSkyboxForIntro()` past the particle-reset calls and therefore
change `.rel.plt` (md5 `d997d11f…`); they are rejected under the
byte-identical `.rel.plt` rule.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13361
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 51835 aligned=21022 size= 57232/57232 insn=20725/20830
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
```

Scratch harnesses (`fast.py`, `sweep_range.py`, `sweep_range_multi.py`,
`eval.py`, `elfread.py`, `variant.py`, `linkonly.txt`, `range58.jsonl`,
`res_d58.jsonl`) live in `/home/debian/GalaxyOnFire2/scratch/w13361/h`;
scratch, not tracked.

## Current-master rebase (wave 190 harvest)

Re-applied the mission-89 state-1 reorder on current master (`454bc033e`) together
with the two w13369 d1 reorders. Combined LevelScript raw 51,803 -> 51,792 (-11),
aligned 22,193 -> 22,211, size 57,232/57,232. `.text` 73,302 -> 73,289 (-13 across
LevelScript + Radar + Gun), `.ARM.extab` 603 unchanged, total 73,925 -> 73,912.
`check` green, RATCHET PASS.
