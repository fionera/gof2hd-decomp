# LevelScript::process — operator-overload + fresh d1/d2 pass (wave 189, w13330)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13330`, branch
`wave189-w13330`, base master `cbcdc0bc4` (wave-189 harvest, raw **51,884**,
`.text` 74,009, `.ARM.extab` 639, `.rel.plt` md5 `289b2a06…`).

Scratch `/home/debian/GalaxyOnFire2/scratch/w13330`.

## Baseline

```
process raw diffbytes    51,884
process aligned insns    21,008
orig / ours size         57,232 / 57,232   (size-exact)
orig / ours insns        20,830 / 20,726
.text diffbytes          74,009
.ARM.extab diffbytes     639  (size/base exact @0x1a3b78, 156,312 B)
.rel.plt                 md5 289b2a06… (byte-identical, 29,392 B)
file size                2,187,328 / 2,187,328
```

## Operator-overload census (task brief)

The w13309 note identified three sites that should be `Vector * float` rather
than `float * Vector`:

* line 2504 `camPos += 3000.0f * dir;` → `camPos += dir * 3000.0f;`
* line 3821 `geo->translate(-30000.0f * right);` → `geo->translate(right * -30000.0f);`
* line 3181 `player->getPosition() - 2400.0f * player->GetDirVector()` →
  `player->getPosition() - player->GetDirVector() * 2400.0f`

The single correct `float * Vector` site (line 2153
`m_tempVec = 5000.0f * VectorNormalize(...)`) is left alone.

All 8 combinations were re-measured on the current body.  On the old
(wave-188) body they were metric-neutral; on the current body every
combination is a raw **regression**:

| A | B | C | raw |
|---|---|---|---|
| 0 | 0 | 0 | **51,884** (base) |
| 0 | 0 | 1 | 51,885 |
| 0 | 1 | 0 | 51,885 |
| 0 | 1 | 1 | 51,886 |
| 1 | 0 | 0 | 51,886 |
| 1 | 0 | 1 | 51,887 |
| 1 | 1 | 0 | 51,887 |
| 1 | 1 | 1 | 51,888 |

All are size-exact with `.rel.plt` unchanged.  The corrections are structurally
right but not raw wins, so the base overload spellings are kept.

## Fresh distance-1 adjacent-statement sweep

A fresh d1 sweep was run over all **2,079** adjacent same-indent single-line
statement pairs on the current body (4-way sharded, NDK r18b `-Oz` compile +
gold link, raw byte metric).  96 pairs fail to compile (declaration/use
reorders); the rest are size-exact or size-changing.  Size-exact raw wins:

| line | swapped order (now) | raw alone |
|---:|---|---:|
| 1960 | `player->setComputerControlled(false)` before `m_pCamera->translate(...)` | **-6** |
| 5229 | `float approach = ...` before `m_nScriptTimerA += delta` | -1 (semantic: reads timer) |
| 5402 | `m_pParticleGeom1->render()` before `m_pParticleGeom0->render()` | -1 |
| 2674 | `CameraSetLocal(...)` before `frame->setPosition(...)` | -1 (semantic: reads matrix) |
| 5461 | `m_nState = 2` before `enemies[0]->setActive(true)` | -1 |
| 829 | `particleRenderBoolPtr->reset()` before `switchSkyboxForIntro()` | -1 |
| 1238 | `m_tempVec.y += 500.0f` before the `m_tempVec = ...` assignment | -1 (semantic) |
| 2532 | `m_pCamera->setPosition(camPos)` before `camPos.y += 800.0f` | -1 (semantic) |
| 721 | `Globals::sound->play(158, ...)` before `updateEvent3DAttributes(158, ...)` | -1 |
| 1912 | `m_bCinematicBreak = 0` before `player->setComputerControlled(false)` | -1 |

## Landed changes

### 1. line 1960 swap (-6)

`player->setComputerControlled(false);` moved before
`m_pCamera->translate(0.0f, 0.0f, (float) (-2 * delta));`.  Independent
receivers (player vs camera); semantics-preserving.

Measured: raw **51,884 → 51,878** (-6), aligned 21,008 (unchanged), size
57,232/57,232 exact, `.text` 74,009 → 74,003, `.ARM.extab` 639 (unchanged),
`.rel.plt` md5 unchanged, file size exact.

### 2. line 1912 swap (-1)

`m_bCinematicBreak = 0;` moved before `player->setComputerControlled(false);`
(and after `m_pCamera->setLookAtCam(false)`).  This matches the original's
relative order at `0x13e710` (`strb ... #17`) / `0x13e71a`
(`setComputerControlled`).  Independent stores/calls.

Measured: raw **51,878 → 51,877** (-1), aligned 21,008 → 21,028 (+20, not a
gate metric), size exact, `.text` 74,003 → 74,002, `.ARM.extab` 639 unchanged,
`.rel.plt` unchanged.

### 3. line 721 swap (-1)

`Globals::sound->play(158, nullptr, nullptr, 0.0f);` moved before
`Globals::sound->updateEvent3DAttributes(158, &pos, &zero158, false);`.
The original at `0x13904e` (`play(158)`) / `0x139060` (`update(158)`) has
exactly this order.  `.rel.plt` is unchanged because both callees were already
referenced earlier in the body.

Measured: raw **51,877 → 51,876** (-1), aligned 21,028 → 21,020 (-8), size
exact, `.text` 74,002 → 74,001, `.ARM.extab` 639 unchanged, `.rel.plt`
unchanged.

## Fresh distance-2 sweep

A fresh d2 sweep (swap statements `i` and `i+2`, leaving `i+1` between them)
was run over all **1,579** same-indent triples on the post-d1 body
(raw 51,876).  138 triples fail to compile.  Size-exact raw wins are listed
below; semantic read-after-write pairs are marked.

| line | swapped with | raw alone | note |
|---:|---:|---:|---|
| 694 | 696 | **-6** | independent player calls |
| 1238 | 1240 | -3 | `z +=` before assignment (semantic) |
| 5229 | 5231 | -3 | `approach` reads timer (semantic) |
| 2364 | 2366 | -2 | independent |
| 3344 | 3346 | -2 | independent |
| 3737 | 3739 | -2 | independent stores |
| 4411 | 4413 | -2 | `setPosition` reads `m_tempVec` (semantic) |
| 4834 | 4836 | -2 | camera calls |
| 654 | 656 | -1 | camera calls |
| 834 | 836 | -1 | independent |
| 1656 | 1658 | -1 | independent |
| 2326 | 2328 | -1 | independent player modes |
| 2327 | 2329 | -1 | camera calls |
| 2441 | 2443 | -1 | camera calls |
| 2695 | 2697 | -1 | `MatrixSetTranslation` reads `m_tempVec` (semantic) |
| 3085 | 3087 | -1 | `docked` local read |
| 3234 | 3236 | -1 | `m_dirVec` read-after-write (semantic) |
| 3571 | 3573 | -1 | two `setPosition` calls (semantic) |
| 3736 | 3738 | -1 | camera calls |
| 4458 | 4460 | -1 | independent boss flags |
| 4527 | 4529 | -1 | independent player calls |
| 4629 | 4631 | -1 | independent |
| 5402 | 5404 | -1 | render order |

### 4. lines 694/696 d2 swap (-6)

`self->getPlayer()->setCollide(false);` moved before
`player->hideShipForFirstPersonCameraView(false);` (the intervening
`m_bCinematicBreak = 1;` stays put).  Independent player calls.

Measured: raw **51,876 → 51,870** (-6), aligned 21,020 (unchanged), size
exact, `.text` 74,001 → 73,995, `.ARM.extab` 639 unchanged, `.rel.plt`
unchanged.

### 5. lines 2364/2366 d2 swap (-2)

`self->lodManager->forceUpdate(delta, false);` moved before
`(*enemies)[10]->setSpeed(8.0f);` (the intervening
`m_pCamera->setLookAtCam(false);` stays put).  Independent receivers.

Measured: raw **51,870 → 51,868** (-2), aligned unchanged, size exact,
`.text` 73,995 → 73,993, `.ARM.extab` 639 unchanged, `.rel.plt` unchanged.

### 6. lines 3344/3346 d2 swap (-2)

`camera->setLookAtCam(false);` moved before `player->setFreeze(false);` (the
intervening `player->player->setVulnerable(true);` stays put).  Independent
receivers.

Measured: raw **51,868 → 51,866** (-2), aligned 21,020 → 21,021 (+1), size
exact, `.text` 73,993 → 73,991, `.ARM.extab` 639 unchanged, `.rel.plt`
unchanged.

### 7. lines 3737/3739 d2 swap (-2)

`m_nScriptTimerB = 0;` moved before `m_nScriptTimerA = 0;` (the intervening
`m_pCamera->setLookAtCam(true);` stays put).  Independent stores.

Measured: raw **51,866 → 51,864** (-2), aligned 21,021 → 21,073 (+52 fuzzy,
not a gate metric), size exact, `.text` 73,991 → 73,989, `.ARM.extab` 639
unchanged, `.rel.plt` unchanged.

### 8. lines 654/656 d2 swap (-1)

`wcamera->setPosition(-5000.0f, 300.0f, -5000.0f);` moved before
`wcamera->setTarget((*enemies)[0]->geometry);` (the intervening
`m_nState = 2;` stays put).  Independent camera state.

Measured: raw **51,864 → 51,863** (-1), aligned 21,073 → 21,075 (+2), size
exact, `.text` 73,989 → 73,988, `.ARM.extab` 639 unchanged, `.rel.plt`
unchanged.

### 9. lines 834/836 d2 swap (-1)

`m_pGeometry5->setPosition(player->getPosition());` moved before
`m_pCamera->setPosition(5000.0f, 500.0f, -10000.0f);` (the intervening
`m_nState = 10;` stays put).  Independent receivers.

Measured: raw **51,863 → 51,862** (-1), aligned unchanged, size exact,
`.text` 73,988 → 73,987, `.ARM.extab` 639 unchanged, `.rel.plt` unchanged.

### 10. lines 1656/1658 d2 swap (-1)

`Globals::appManager->SetCurrentApplicationModule(5);` moved before
`Globals::status->nextCampaignMission(true);` (the intervening
`Globals::switch_to_target_setting = 0;` stays put).  Independent globals.

Measured: raw **51,862 → 51,861** (-1), aligned 21,075 → 21,050 (-25), size
exact, `.text` 73,987 → 73,986, `.ARM.extab` 639 unchanged, `.rel.plt`
unchanged.

### 11. lines 2326/2328 d2 swap (-1)

`player->setFreeLookMode(false);` moved before `player->setTurretMode(false);`
(the intervening `resetCamera(m_pLevel);` stays put).  Independent player
mode flags.

Measured: raw **51,861 → 51,860** (-1), aligned unchanged, size exact,
`.text` 73,986 → 73,985, `.ARM.extab` 639 unchanged, `.rel.plt` unchanged.

### 12. lines 2327/2329 d2 swap (-1)

On the post-2326 body, `player->setTurretMode(false);` moved before
`resetCamera(m_pLevel);` (the intervening
`m_pCamera->enableFirstPersonCam(false);` stays put).  Independent
player/camera state.

Measured: raw **51,860 → 51,859** (-1), aligned 21,050 → 21,057 (+7), size
exact, `.text` 73,985 → 73,984, `.ARM.extab` 639 unchanged, `.rel.plt`
unchanged.

### 13. lines 3085/3087 d2 swap (-1)

`player->setFreeLookMode(true);` moved before
`bool docked = player->isDockedToDockingPoint();` (the intervening
`m_pCamera->setLookAtCam(false);` stays put).  `setFreeLookMode` only writes
`freeLookMode`; `isDockedToDockingPoint` reads `dockedFlag` /
`dockingPointIndex`, so the two are independent.

Measured: raw **51,859 → 51,858** (-1), aligned 21,057 → 21,016 (-41), size
exact, `.text` 73,984 → 73,983, `.ARM.extab` 639 unchanged, `.rel.plt`
unchanged.

### 14. lines 4458/4460 d2 swap (-1)

`boss->setActive(false);` moved before `boss->setVisible(false);` (the
intervening `wreck->setActive(true);` stays put).  Independent flags on the
same receiver.

Measured: raw **51,858 → 51,857** (-1), aligned unchanged, size exact,
`.text` 73,983 → 73,982, `.ARM.extab` 639 unchanged, `.rel.plt` unchanged.

### 15. lines 4527/4529 d2 swap (-1)

`player->setSpeed(0.0f);` moved before
`player->hideShipForFirstPersonCameraView(false);` (the intervening
`player->player->setVulnerable(false);` stays put).  Independent player state.

Measured: raw **51,857 → 51,856** (-1), aligned unchanged, size exact,
`.text` 73,982 → 73,981, `.ARM.extab` 639 unchanged, `.rel.plt` unchanged.

### 16. lines 4629/4631 d2 swap (-1)

`m_pCamera->setTarget(player->geometry);` moved before
`tvA = player->getPosition();` (the intervening `player->setSpeed(0.0f);`
stays put).  Independent.

Measured: raw **51,856 → 51,855** (-1), aligned 21,016 → 21,006 (-10), size
exact, `.text` 73,981 → 73,980, `.ARM.extab` 639 unchanged, `.rel.plt`
unchanged.

### 17. lines 3571/3573 d2 swap (-1)

`player->player->setVulnerable(true);` moved before
`m_pCamera->setPosition(m_dirVec);` (the intervening `m_pHud->visible = 1;`
stays put).  Independent player/camera state.

Measured: raw **51,855 → 51,854** (-1), aligned unchanged, size exact,
`.text` 73,980 → 73,979, `.ARM.extab` 639 unchanged, `.rel.plt` unchanged.

## Second d1 sweep (post-d2 body)

A second full d1 sweep was run on the post-d2 body (raw 51,854).  New
size-exact raw wins:

| line | swapped order (now) | raw alone |
|---:|---|---:|
| 5229 | `approach` before `m_nScriptTimerA += delta` | -3 (semantic) |
| 829 | `particleRenderBoolPtr->reset()` before `switchSkyboxForIntro()` | -1 (`.rel.plt` changes) |
| 1238 | `m_tempVec.y += 500` before assignment | -1 (semantic) |
| 2532 | `setPosition(camPos)` before `camPos.y += 800` | -1 (semantic) |
| 2674 | `CameraSetLocal` before `frame->setPosition` | -1 (semantic) |
| 4629 | `player->setSpeed(0.0f)` before `m_pCamera->setTarget(...)` | -1 |
| 4630 | `tvA = player->getPosition()` before `player->setSpeed(0.0f)` | -1 |
| 5402 | `render1` before `render0` | -1 (`.text` +32) |
| 5461 | `m_nState = 2` before `setActive(true)` | -1 (`.text` +32) |

### 18. lines 4629/4630 d1 swap (-1)

`player->setSpeed(0.0f);` moved before
`m_pCamera->setTarget(player->geometry);`.  Independent receiver.

Measured: raw **51,854 → 51,853** (-1), aligned 21,006 → 21,001 (-5), size
exact, `.text` 73,979 → 73,978, `.ARM.extab` 639 unchanged, `.rel.plt`
unchanged.

## Rejected candidates

All of the following are size-exact and `.rel.plt`-identical but were rejected
for the stated reason:

* **lines 5402/5404 and 5461/5462 (d1)**: raw -1 each, but `.text` grows
  1,259,464 → 1,259,496 (+32) and `.ARM.extab` diff grows 639 → 1,324.  The
  +32-byte `.text` shift demotes every function after `process` (byte_exact
  4507 → 2826), so it is a net loss.
* **line 829 (d1)**: raw -1, but swapping `switchSkyboxForIntro()` and
  `particleRenderBoolPtr->reset()` changes gold's first-reference order, so
  `.rel.plt` md5 changes (`289b2a06…` → `d997d11f…`).
* **line 2441 (d2)**: raw -1 but aligned 21,057 → 21,761 (+704); the original
  at `0x139790`/`0x139798`/`0x1397b0` has `setLookAtCam`, `setActive`,
  `setTarget`, i.e. the unswapped order.
* **lines 4834/4836 (d2)**: raw -2, but the original at
  `0x13a48c`/`0x13a496`/`0x13a4ac` has `setLookAtCam`, `setTarget`,
  `setPosition`, i.e. the unswapped order.
* **lines 4411/4413 (d2)**: raw -2, but `setPosition(m_tempVec)` must follow
  both `m_tempVec +=` updates (original `0x1394ec`…`0x13951a`).
* **lines 1238/1240, 2532/2534, 2674/2676, 2695/2697, 3234/3236, 5229/5231
  (d2)**: read-after-write pairs; the swap changes the value consumed.
  2695 also changes the instruction count (20,726 → 20,727).
* **line 3736 (d1, second sweep)**: raw +1 on the post-d2 body.
* **line 4630 (d1, second sweep)**: raw +2 on the post-d2 body.

## Operator-overload re-measurement on the final body

Re-ran all 8 combinations of the three `Vector * float` corrections on the
post-d2 body (raw 51,854).  Every combination is still a raw regression
(51,855 … 51,860), so the base spellings are kept.
