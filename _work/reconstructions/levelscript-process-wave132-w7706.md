# LevelScript::process — wave 132 / w7706 (four semantic body fixes, size-neutral)

Status: **code change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, 20169 normalised
instructions. Owned TU: `src/game/world/LevelScript.cpp`. Base master `70e451ffe`.
Scratch helpers in `/dev/shm/w7706/` (not tracked).

## Result

```
                         before (master)   after (landed)
raw ratio (difflib)      59.879643905      59.635947680
official verify pct      59.9              59.6
orig / ours insns        20169 / 20045     20169 / 20045
orig / ours size         57232 / 57232     57232 / 57232    (size-exact)
process .text diffbytes  54651             54591            (-60)
byte_exact               4506              4506
linked_exact             4506              4506
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
.rel.plt/.rodata/.data/.ARM.exidx  all 0 diff bytes
```

The headline fuzzy pct dips 0.24 because the four corrected bodies are cold blocks whose
statement order changes the local block layout; the **real** differing-byte count falls by 60.
The gate only checks `byte_exact`/`linked_exact`/size/imports, all unchanged.

Metric = `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100` on
`tools/verify/verify.py`'s reconciled streams (`/dev/shm/w7706/score.py`).
`process .text diffbytes` is the byte-diff count over the function's own 57232-byte range
(`/dev/shm/w7706/bytediff.py`).

## Landed fix 1 — case 70 state 2: `setRotate(5)` was called every frame

Original state-2 body `0x13ec3c`:

```
13ec3c  forceUpdate(delta,false)             <-- at the TOP of the state body
13ec4c  if (messages[2]->isOver()) {
13ec5c    setLookAtCam(false) ... resetCamera ... forceUpdate
13eca0    m_bCinematicBreak = 0
13eca8    m_nState = 3
13ecaa    lead = enemies[0]->player
13ecb2    lead->setMaxHitpoints(enemies[0]->player->getMaxHitpoints()*3)
13ecd2    enemies[0]->setSpeed(10.0f)
13ece2    enemies[0]->setRotate(5)          <-- LAST statement, inside the if
13ece8  }
```

Our source had `setRotate(5)` **before** the `isOver()` early-out (so it ran every frame while
state 2 was active) and a `forceUpdate` at the end. The faithful body moves the `forceUpdate` to
the top and `setRotate(5)` to after `setSpeed`. Size-neutral (57232 B); the local body is now
instruction-for-instruction identical to the original apart from register colouring.

## Landed fix 2 — case 131 state 2: dead `GetDirVector` result (w7687 bug 1)

Original `0x13c942..0x13c9de` applies `dir*delta*2.01`, then `right*delta*0.03`, then
`up*delta*0.01`. Our source overwrote `scratch` with `getRightVector()` before using the
`GetDirVector` result, so the right vector was applied twice and the dir vector never. Fixed to
the same order as state 1 (and the original). Size -8 B on its own; landed with the +8 fix 4.

## Landed fix 3 — case 154 state 2: dead `GetDirVector` result

Original `0x142f34`: `m_dirVec = player->GetDirVector();` **then**
`m_tempVec += m_dirVec * (step*0.008f)`, then right `*-0.05`, then up `*0.0051`. Our source
used the stale `m_dirVec` for the first term and then assigned the dir vector (dead). Fixed.

## Landed fix 4 — case 154 state 6: same dead-assignment pattern

Original `0x14330e`: dir `*0.008`, right `*0.05`, up `*0.005`. Our source used the stale
`m_dirVec` for the first two terms and applied the up vector twice. Fixed. Size +12 B on its own;
landed with the -8/-4 fixes above (net 0).

## Measured but NOT landed (need size companions)

| fix | size delta | note |
|---|---:|---|
| case 64 state 1 | **+24 B** | biggest semantic bug found this pass — see below |
| case 157 `m_pGeometry2->setVisible(false)` | -4 B | w7687 bug 2, still present |
| case 154 state 4 (same dead-assignment pattern) | +8 B | original `0x143138` |

### Case 64 state 1 is a whole wrong statement sequence

Original `0x13ecfc` (state 1):

```
13ecfc  forceUpdate(delta,false)
13ed0e  translate(0.0f, delta * 0.2, 0.0f)     ; double math: vcvt.f64/ vmul.f64 /vcvt.f32
13ed32  if (messages[2]->isTriggered()) {
13ed42    setLookAtCam(false) ... resetCamera ... forceUpdate
13ed82    m_bCinematicBreak = 0
13ed84    player->freeze = 0
13ed88    for (i) (*enemies)[i]->setRoute(nullptr)
13eddc    m_nState = 2
13ede0  }
```

Our source has `player->freeze = 0; m_pCamera->setRumblePercentage(100.0f, 30);` at the top and
`Globals::sound->stop(156);` where the original has `m_bCinematicBreak = 0`. This is confirmed by
the global call multiset: original `setRumblePercentage` 39 / `stop` 10 / `translate` 57, ours
40 / 11 / 55. The faithful body would bring all three counts to 39 / 10 / 56 (one `translate`
still missing elsewhere). Cost: **+24 B**, so it needs a -24 B companion; not landable this pass.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
python3 /dev/shm/w7706/score.py                         # 59.635947680
python3 /dev/shm/w7706/bytediff.py                      # process 54591
```

Scratch helpers (`common.py`, `score.py`, `bytediff.py`, `fixes.py`, `orig.dis`,
`ours_final.dis`) live in `/dev/shm/w7706/` and are not part of the tree.

## Post-merge note (wave 132 coordinator)

This branch's `LevelScript.cpp` edits were **reverted** in favour of the parallel
w7705 semantic set (case 131 `dir * 2.01`, case 157 `m_pGeometry2`, the missing
8th `setPosition(f,f,f)`, mission-0 state-6/10 sound fixes, case-40 state 4).
The two branches overlap in case 131/154; applying both grew `process` to
57,264 B (+32 section bytes).  w7705 alone keeps the exact 57,232 B and lowers
the function's diff bytes to 54,507.  The per-case measurements above remain
valid as a map of the residual.
