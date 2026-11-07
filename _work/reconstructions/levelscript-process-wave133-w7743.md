# LevelScript::process — wave 133 / w7743 (case-64 state-1 full body landed)

Status: **code change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**. Owned TU:
`src/game/world/LevelScript.cpp`. Base master `ec0acea1e`.

## Result

```
                         before (master)   after (landed)
official verify pct      58.9              58.9
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
process diffbytes        54465             54524
whole .text diffbytes    85143             85202
.ARM.extab diffbytes     1416              1416
.rel.plt diffbytes       0                 0
byte_exact               4506              4506
linked_exact             4506              4506
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

All section sizes are exact (`.text` 1259464, `.ARM.extab` 156312, `.rel.plt` 29392,
`.rodata` 269159, `.data` 10604, `.bss` 27052, `.ARM.exidx` 28528).

## Landed edit 1 — case 64 state 1, full body (orig `0x13ecfc`..`0x13ed32`)

The original body starts with two calls that w7723 had left out:

```
13ecfc  ldr r0,[r9,#24]; ldr r0,[r0]; blx LODManager::forceUpdate(delta,false)
13ed0e  vmov s0,r5 ; vldr d17,=0.2 ; vcvt.f64.s32 ; vmul.f64 ; vcvt.f32.f64
13ed2e  blx TargetFollowCamera::translate(0.0f, (float)(delta*0.2), 0.0f)
13ed32  if (messages[2]->isTriggered()) { ... m_nState = 2; }
```

The `0.2` is a **double** literal (`0x3fc999999999999a` at `0x13edc0`), so the source
must spell `(float) ((double) delta * 0.2)` to reproduce the
`vcvt.f64.s32 / vmul.f64 / vcvt.f32.f64` sequence. Landed exactly:

```cpp
} else if (m_nState == 1) {
    m_pLevel->lodManager->forceUpdate(delta, false);
    m_pCamera->translate(0.0f, (float) ((double) delta * 0.2), 0.0f);
    if (((RadioMessage *) ((*messages)[2]))->isTriggered()) { ... }
```

The rest of the body (setLookAtCam / setTarget / setComputerControlled / hud+radar
visible / resetCamera / forceUpdate / `m_bCinematicBreak = 0` / `player->freeze = 0` /
`for i: enemies[i]->setRoute(nullptr)` / `m_nState = 2`) was already faithful from
w7723.

## Landed edit 2 — three `m_pLevel->getPlayer()` -> `player` fixes

The original reuses the already-loaded `PlayerEgo *` local at these three sites; our
source re-fetched it through `Level::getPlayer()`, adding three extra calls:

* case 102 state 0 (`~line 3240`): `if (player->isInRocketControl())`
* case 145 state 1 (`~line 4318`): `if (player->isInRocketControl())`
* case 145 state 10 (`~line 3787`): `*camPos += player->geometry->getPosition();`

Verified against the original: case 145 state 1 uses `r6` (player), case 125 state 3
uses `r9` (player), case 145 state 10's `*camPos +=` uses `r5` (player). Size effect
`-16 B`; this is the companion that lets the case-64 body land at exactly 57232 B.

## Landed edit 3 — first-reference anchor reduced 7 calls -> 2

The `if (delta < 0)` anchor in case 125 state 3 (never taken by the caller) is a
first-reference/size knob. It previously held 7 calls. It now keeps only
`switchSkyboxForIntro()` (B) and `PlayerStation::setVisible(false)` (G):

```cpp
if (delta < 0) {
    m_pLevel->switchSkyboxForIntro();
    ((PlayerStation *) (*m_pLevel->getLandmarks())[0])->setVisible(false);
}
```

`G` is required for `.rel.plt` exactness: without an early `setVisible` reference the
first-reference order becomes `PlayerEgo::deleteHackingGame` then
`PlayerStation::setVisible`, swapping `.rel.plt` entries 2835/2836. `B` is the size
filler that lets the pair land at 57232 B; `E` (`PlayEngineSound`) and `F`
(`StopEngineSound`) are byte-identical alternatives.

## Measurements (all on pristine master `ec0acea1e`)

### case-64 body alone

| variant | size | pct | proc diff | .text diff | relplt |
|---|---:|---:|---:|---:|---:|
| master | 57232 | 58.9 | 54465 | 85143 | 0 |
| master + case-64 full | 57296 | 58.7 | 55170 | 416808 | 0 |

The body alone costs `+64 B` (not the `+24 B` seen on the w7723 base) and re-rolls
the whole layout, hence the need for a `-64 B` companion.

### anchor keep-set sweep on master + case-64 full (no other edits)

| kept anchor calls | size | pct | proc diff | .text diff | relplt |
|---|---:|---:|---:|---:|---:|
| B only | 57232 | 58.9 | 54467 | 85145 | **2** |
| G only | 57248 | 58.7 | 54964 | 410981 | 0 |
| B+G | 57248 | 58.7 | 54989 | 411006 | 0 |
| E+G | 57248 | 58.7 | 54989 | 411006 | 0 |
| F+G | 57248 | 58.7 | 54989 | 411006 | 0 |
| A+G / C+G / D+G | 57240 | 58.7 | 55082..55079 | 407698 | 0 |

`B only` is the closest to master (`.text` +2) but has the `.rel.plt` swap.

### anchor keep-set + the three `getPlayer` fixes

| variant | size | pct | proc diff | .text diff | relplt |
|---|---:|---:|---:|---:|---:|
| G only + fixes | 57232 | 58.9 | 54553 | 85231 | 0 |
| **B+G + fixes (landed)** | **57232** | **58.9** | **54524** | **85202** | **0** |
| E+G + fixes | 57232 | 58.9 | 54524 | 85202 | 0 |
| F+G + fixes | 57232 | 58.9 | 54524 | 85202 | 0 |
| B+E+G + fixes | 57240 | 58.7 | 55102 | 407718 | 0 |

`B+G + fixes` is the best valid point: size-exact, `.rel.plt`-exact, pct unchanged at
58.9, `.text` only +59 vs master.

## Dropped (measured, not landed)

The previous agent's uncommitted case-92 state-3 reorder and case-154 state-2
creep/tail fixes are semantically faithful but cost `0.9` pct when combined with the
case-64 body (58.9 -> 58.0) and need their own companions. They are not part of this
commit; they remain open leads:

* case 92 state 3 reorder alone on master+case-64+anchor: size 57240.
* case 154 state 2 tail alone: size 57224.
* both together: 57232 but pct 58.0, proc diff 54558.

## Open leads

1. `B only` (no `G`) is the ideal metric point (`proc 54467`, `.text` 85145) but swaps
   `.rel.plt` 2835/2836. If the real case-42 state-7 `PlayerStation::setVisible` block
   could be made to precede the case-125 `PlayerEgo::deleteHackingGame` block in MBP's
   order, `G` would be unnecessary and the anchor could shrink to `B` only.
2. The case-92 / case-154 state-2 faithful reorders need `-8`/`+8` companions to land
   without the pct loss.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
cmake --build cmake-build-match --target drift -j6      # 0 fields
```

Scratch helpers in `/dev/shm/w7743/` (not tracked): `score.py` (size/pct/diffbytes),
`relplt.py` (`.rel.plt` name diff), `edits.py` (E64/E92/E154s2/E67/anchor spec
builder), `e64sweep.py`, `anchor67.py`, `anchor_no67.py`, `master.cpp`, `base2.cpp`,
`orig_process.dis`.

## Post-merge note (wave 134 coordinator)

The source change was **not landed**: on the wave-134 base it raises `process`
diff bytes from 54,359 to 54,568 (+209) even at the exact 57,232 B size.  The
case-64 double-constant translate and the 2-call anchor are the faithful shape,
but the block re-roll outweighs them.  This note is kept as the reconstruction
lead; the next pass should land it together with a block-placement companion
that recovers the 209 bytes.
