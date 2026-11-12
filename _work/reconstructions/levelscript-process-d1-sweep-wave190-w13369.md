# LevelScript::process — wave 190 / w13369: fresh distance-1 sweep

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13369`, branch
`wave190-w13369`, base master `0f54626b3` (raw **51,820**, `.text` 73,516,
`.ARM.extab` 610).  Scratch `/home/debian/GalaxyOnFire2/scratch/w13369`.

Task: state-machine dispatch / register-allocation residual.  The wave-189
d1/d2/d3/d4 sweeps were run on older bodies (51,845 / 51,874 / 51,844); the
wave-189 follow-ups (w13355 d3/d4, w13357, w13363) moved the body to 51,820.
This pass re-ran the full adjacent-statement (`d=1`) sweep on the current
51,820 body with the w13352 exact compile + gold-link harness.

## Result (code win landed)

```
                         base (HEAD)      this pass
process raw diffbytes    51,820           51,818     (-2)
process aligned insns    20,981           20,981
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,726  20,830 / 20,726
.text diffbytes          73,516           73,514     (-2)
.ARM.extab diffbytes     610              610        (size/base exact 156,312 B @ 0x1a3b78)
.ARM.exidx diffbytes     0                0          (byte-identical)
.rodata / .data / .bss   0 / 0 / 0        0 / 0 / 0
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical, 29,392 B)
.note.gnu.build-id       20               20
total differing bytes    74,146           74,144     (-2)
file size                2,187,328        2,187,328  (exact)
byte_exact               4507             4507
linked_exact             4507             4507
size-exact               4524/4524        4524/4524
data_exact               843/843          843/843
imports                  207              207
lint                     CLEAN ✓          CLEAN ✓
drift                    0                0
symbol parity            5370/5370        5370/5370
RATCHET                  PASS             PASS
```

## Landed change

Source lines 2496/2497 (mission-0 state-4 teardown block):

```cpp
-                    player->setFreeLookMode(false);
-                    m_pLevel->lodManager->forceUpdate(delta, false);
+                    m_pLevel->lodManager->forceUpdate(delta, false);
+                    player->setFreeLookMode(false);
```

The two statements have disjoint receivers (`PlayerEgo` free-look flag vs
`LODManager` update) and neither reads state written by the other, so the
reorder is semantics-preserving.  Measured alone on the wave-190 base: raw
**51,820 -> 51,818** (-2), size 57,232/57,232 exact, `.text` 73,516 ->
73,514, `.ARM.extab` unchanged (610), `.rel.plt` md5
`289b2a069f8504d1e8a3919e43915e04` unchanged.

## Landed change 2

Source lines 3538/3539 (mission-0 state-4 teardown block):

```cpp
-                        wcamera->setLookAtCam(true);
-                        player->player->setVulnerable(false);
+                        player->player->setVulnerable(false);
+                        wcamera->setLookAtCam(true);
```

Disjoint receivers (`TargetFollowCamera` look-at flag vs `Player` vulnerable
flag); semantics-preserving.  Measured on top of change 1: raw **51,818 ->
51,816** (-2), size 57,232/57,232 exact, `.text` 73,514 -> 73,512,
`.ARM.extab` 610 unchanged, `.rel.plt` md5 unchanged.

## Landed change 3

Source lines 1491/1492 (mission-0 state-3 teardown):

```cpp
-            lvl->getPlayer()->setCollide(true);
-            m_bStartSequenceOver = 1; // startSequenceIsOver = true
+            m_bStartSequenceOver = 1; // startSequenceIsOver = true
+            lvl->getPlayer()->setCollide(true);
```

Disjoint receivers (player collide flag vs `m_bStartSequenceOver` store).
Measured on top of changes 1-2: raw **51,816 -> 51,815** (-1), size exact,
`.text` 73,512 -> 73,511, `.ARM.extab` 610 unchanged, `.rel.plt` unchanged.

## Landed change 4

Source lines 1609/1610:

```cpp
-                    m_pRadar->enabled = 0;
-                    player->stopShooting(0);
+                    player->stopShooting(0);
+                    m_pRadar->enabled = 0;
```

Disjoint receivers (`Radar` enabled flag vs `PlayerEgo` shooting state).
Measured on top of changes 1-3: raw **51,815 -> 51,814** (-1), size exact,
`.text` 73,511 -> 73,510, `.ARM.extab` 610 unchanged, `.rel.plt` unchanged.

## Landed change 5

Source lines 1941/1942:

```cpp
-                    player->setFreeLookMode(false);
-                    player->player->setVulnerable(false);
+                    player->player->setVulnerable(false);
+                    player->setFreeLookMode(false);
```

Disjoint player flags (`freeLookMode` vs `vulnerable`).  Measured on top of
changes 1-4: raw **51,814 -> 51,813** (-1), size exact, `.text` 73,510 ->
73,509, `.ARM.extab` 610 unchanged, `.rel.plt` unchanged.

## Landed change 6

Source lines 3049/3050:

```cpp
-                    wcamera->setTarget(leader);
-                    AbyssEngine::PaintCanvas *canvas = Globals::Canvas;
+                    AbyssEngine::PaintCanvas *canvas = Globals::Canvas;
+                    wcamera->setTarget(leader);
```

`Globals::Canvas` is a pure read and `setTarget` does not write it.
Measured on top of changes 1-5: raw **51,813 -> 51,812** (-1), size exact,
`.text` 73,509 -> 73,508, `.ARM.extab` 610 unchanged, `.rel.plt` unchanged.

## Landed change 7

Source lines 3536/3537:

```cpp
-                        m_pRadar->enabled = 0;
-                        m_pHud->visible = 0;
+                        m_pHud->visible = 0;
+                        m_pRadar->enabled = 0;
```

Disjoint receivers (`Hud` visibility vs `Radar` enabled flag).  Measured on
top of changes 1-6: raw **51,812 -> 51,811** (-1), size exact, `.text`
73,508 -> 73,507, `.ARM.extab` 610 unchanged, `.rel.plt` unchanged.

## Landed change 8

Source lines 3882/3883:

```cpp
-                            wcamera->setTarget(marker);
-                            Globals::sound->play(14, nullptr, nullptr, 0.0f);
+                            Globals::sound->play(14, nullptr, nullptr, 0.0f);
+                            wcamera->setTarget(marker);
```

Disjoint receivers (camera target pointer vs `FModSound`).  Measured on top
of changes 1-7: raw **51,811 -> 51,810** (-1), size exact, `.text` 73,507 ->
73,506, `.ARM.extab` 610 unchanged, `.rel.plt` unchanged.

## Landed change 9

Source lines 3883/3884 (second d1 sweep on the 51,810 body):

```cpp
-                            wcamera->setTarget(marker);
-                            Vector zero{0.0f, 0.0f, 0.0f};
+                            Vector zero{0.0f, 0.0f, 0.0f};
+                            wcamera->setTarget(marker);
```

The local `zero` construction is independent of the camera target write.
Measured on top of changes 1-8: raw **51,810 -> 51,809** (-1), size exact,
`.text` 73,506 -> 73,505, `.ARM.extab` 610 unchanged, `.rel.plt` unchanged.

## Sweep

The w13352 harness (`stmts.py`/`model.py`/`gen.py`/`eval.py`/`sweep.py`,
copied into `scratch/w13369/`) was re-pointed at this worktree.  A full
`d=1` sweep of all **1,529** unique independent adjacent statement pairs on
the current 51,820 body (4 workers, exact NDK r18b `-Oz` compile + gold
link) found **14** size-exact raw wins:

| 0-based lines | pair | raw | safe? |
|---|---|---:|---|
| **2495/2496** | `setFreeLookMode(false)` / `forceUpdate` | **-2** | **independent — landed** |
| 3537/3538 | `setLookAtCam(true)` / `setVulnerable(false)` | -2 | independent — follow-up candidate |
| 828/829 | `switchPlanetForIntro()` / `switchSkyboxForIntro()` | -1 | `.rel.plt` moves — rejected |
| 889/890 | `m_tempVec = getPosition()` / `m_tempVec +=` | -1 | read-after-write — unsafe |
| 1490/1491 | `setCollide(true)` / `m_bStartSequenceOver = 1` | -1 | independent — follow-up candidate |
| 1608/1609 | `m_pHud->visible = 0` / `m_pRadar->enabled = 0` | -1 | independent — follow-up candidate |
| 1940/1941 | `shipGroup = 1` / `setFreeLookMode(false)` | -1 | independent — follow-up candidate |
| 2531/2532 | `camPos.x +=` / `camPos.y +=` | -1 | disjoint members — follow-up candidate |
| 3048/3049 | `wcamera->setTarget(leader)` / `canvas = Globals::Canvas` | -1 | independent — follow-up candidate |
| 3535/3536 | `m_pRadar->enabled = 0` / `m_pHud->visible = 0` | -1 | independent — follow-up candidate |
| 3734/3735 | `*tmp = getUpVector()` / `*camPos += *tmp * 400` | -1 | read-after-write — unsafe |
| 3881/3882 | `wcamera->setTarget(marker)` / `sound->play(14)` | -1 | independent — follow-up candidate |
| 4541/4542 | `m_tempVec +=` / `setPosition(m_tempVec)` | -1 | read-after-write — unsafe |
| 4962/4963 | `m_tempVec +=` / `chaser->setPosition(...)` | -1 | read-after-write — unsafe |

Line numbers are 0-based (the landed pair is source lines 2496/2497).
The remaining independent candidates are left for follow-up passes; each is
a size-exact -1/-2 on this body.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13369
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 51818 aligned= 20981 size= 57232/57232 insn= 20726/20830
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=51818 size=57232/57232 text=73514 relplt=289b2a069f8504d1e8a3919e43915e04
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
```

Scratch harnesses (`stmts.py`, `model.py`, `gen.py`, `eval.py`, `elfread.py`,
`sweep.py`, `linkonly.txt`, `res_d1.jsonl`) live in
`/home/debian/GalaxyOnFire2/scratch/w13369/`; scratch, not tracked.

## Current-master harvest (wave 190, zero-vector reorder)

Re-applied the new `Vector zero` before `wcamera->setTarget(marker)` reorder on
current master (`5ff423480`). LevelScript raw **51,785 -> 51,784** (-1), aligned
22,210. It composes with the w13368 d5/d6 group to raw **51,769** (-16 from
51,785). `.text` 73,273 -> 73,254 (-19 across LevelScript + Radar), `.ARM.extab`
606 unchanged, total 73,899 -> 73,880 (-19). `check` green, RATCHET PASS.
