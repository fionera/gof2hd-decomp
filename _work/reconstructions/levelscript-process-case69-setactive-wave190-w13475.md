# LevelScript::process — wave 190 / w13475: case-69 setActive / getPosition order

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13475`, branch
`wave190-w13475`, base master `7db176c4d` (raw **51,691**, aligned **21,967**,
`.text` 71,956, `.rel.plt` md5 `289b2a069f8504d1e8a3919e43915e04`).

A fresh full `d=1` adjacent-statement sweep of the current body (1,763
candidates, exact NDK r18b `-Oz` compile + gold link, 4 workers) found two
size-exact `-1` cells.  One is the already-documented
`switchSkyboxForIntro`/`particleRenderBoolPtr->reset` swap (source 829/830),
which still changes `.rel.plt` (`d997d11f…`) and remains rejected.  The other
is new and `.rel.plt`-clean.

## Landed change

Source lines 2442-2443 (case 69, cutscene-exit state 0):

```cpp
-                    m_pCamera->setActive(true);
-                    Vector camPos = (*m_pLevel->getEnemies())[0]->getPosition();
+                    Vector camPos = (*m_pLevel->getEnemies())[0]->getPosition();
+                    m_pCamera->setActive(true);
```

`TargetFollowCamera::setActive` only stores the camera's active flag;
`KIPlayer::getPosition` is a pure read of the enemy transform.  The two
receivers are unrelated, so the reorder is semantics-preserving.  (The
original emits `setActive` at `0x139798` before the virtual `getPosition` at
`0x1397c6`, so this is a local encoding ripple like the previously landed
adjacent cells, not a source-order correction.)

Measured on the real build:

```
raw        51,691 -> 51,690  (-1)
aligned    21,967 -> 21,967
size       57,232 / 57,232   (exact)
.text      71,956 -> 71,955  (-1)
.ARM.extab 578 (unchanged; size/base exact 156,312 B @ 0x1a3b78)
.rel.plt   md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical)
```

`check` -> RATCHET PASS (byte_exact 4507, linked 4507, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370).

## Rejected

* `down 224,225` (source 829/830, `switchSkyboxForIntro` across
  `particleRenderBoolPtr->reset`): raw 51,690, size-exact, but `.rel.plt`
  becomes `d997d11f85b4fdc0454d71c45f061bfe` (first-reference reorder) —
  gate-fatal.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13475
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 51690 aligned=21967 size=57232/57232
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=51690 size=57232/57232 text=71955 relplt=289b2a069f8504d1e8a3919e43915e04
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
```
