# LevelScript::process — wave 139 / w8404 (case-40 nest landed at 57232 B via the tail-store reorder)

Status: **code change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**. Owned TU:
`src/game/world/LevelScript.cpp`. Base master `bf123bd47` (wave-138 head).

## Result

```
                         before (master)   after (landed)
official verify pct      57.4              57.4
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
process diffbytes        55,072            55,171           (+99)
whole .text diffbytes    85,476            85,575           (+99)
.ARM.extab diffbytes     1,396             1,396            (size exact 156,312)
.ARM.exidx               0                 0
.rodata / .data / .bss   exact             exact
.rel.plt                 0                 0   (byte-identical)
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

The raw diff is 103 B lower than the wave-137 w8308 landing (55,274) while the
function stays exactly 57,232 B.  It is 99 B above the pre-w8308 master because
the case-40 cold-block re-roll still costs more than the local fix gains; the
official fuzzy pct is unchanged.

## 1. The w8308 case-40 cold-chain placement (the mission target)

w8308's nested test is re-landed verbatim (see
`levelscript-process-wave137-w8308.md`):

```cpp
if (m_nState == 0) {
    if (((RadioMessage *) ((*messages)[3]))->isTriggered()) {
        ... state 0 ...
    }
} else if (m_nState == 1) { ... }
```

On the current (wave-138) base this is **-8 B** and makes the original's
`cmp #0; bne.w cold` hot polarity with the state-0 body inline at `0x13a4d8`.
The original's cold dispatch at `0x13e6b4` then checks state 2, 1, 3, 4 in that
order — exactly what the source already emits.

## 2. The new -8 B companion: `tailorder` (case-157 tail store order)

The w8201 note's `rgd1`/`restore_rgd` levers are not used.  The new lever is a
genuine statement-order fix at the very end of the function.  Original
`0x145bd0..0x145bec`:

```
145bd0  ldr r0,[pc]   ; &Globals::appManager
145bd2  ldr r1,[pc]   ; &Globals::switch_to_target_setting
145bd4  ldr r2,[pc]   ; &Level::initStreamOutPosition
145bdc  ldr r0,[r0]
145bde  ldr r1,[r1]
145be0  ldr r2,[r2]
145be2  ldr r0,[r0]
145be4  str r8,[r1]   ; switch_to_target_setting = 1
145be8  strb r8,[r2]  ; initStreamOutPosition = 1
145bec  b.w 13d6b6    ; shared tail: movs r1,#2 ; blx SetCurrentApplicationModule
```

Both globals are stored **before** the `SetCurrentApplicationModule(2)` call.
The source had the call between them:

```cpp
Globals::switch_to_target_setting = 1;
Globals::appManager->SetCurrentApplicationModule(2);   // moved down
Level::initStreamOutPosition = 1;
```

Reordering to the original order is **-8 B** on this base (the compiler can
then share the `SetCurrentApplicationModule` tail block) and is `.rel.plt`-safe
(both symbols already have earlier first references).

## 3. Two additional faithful fixes (net 0 B, -22 raw diff)

These are genuine missing/duplicate statements, both verified against objdump:

* **`rgd` (+12 B)** — the case-154 state-9 teardown is missing
  `m_pLevel->getPlayer()->resetGunDelay();` between `m_pCamera->setFixed(false)`
  and `setLookAtCam(false)` (original `0x144976..0x14498c`).  This is w8201's
  open lead; it lowers the raw diff by 271 B.
* **`dupreset` (-12 B)** — the case-154 state-7 block calls `resetCamera(m_pLevel)`
  twice; the original has exactly one (`0x1408c6`).  Removing the duplicate is
  a faithfulness fix.

The two cancel in size and leave the raw diff 22 B lower than the `tailorder`
landing alone.

## Size accounting (measured on the wave-138 master)

| edit | delta |
|---|---:|
| case-40 nested `if` (w8308) | -8 |
| docking `resetGunDelay` fresh `getPlayer()` (w8308 `rgd2`) | +8 |
| `routePoints` faithful float fold (w8308 `routefold`) | +8 |
| `tailorder` (both globals before the call) | -8 |
| `rgd` (missing resetGunDelay @0x144982) | +12 |
| `dupreset` (duplicate resetCamera @0x1408c6) | -12 |
| **net** | **0** (measured 57232) |

## Experiments (all measured on the wave-138 master; 57232 B unless noted)

| variant | size | raw diff |
|---|---:|---:|
| master | 57232 | 55,072 |
| w8308 set (nest40 + rgd2 + routefold) | 57240 | 55,274 |
| w8308 + `fixgetplayer` (case-154 state-12 cached `player`) | 57232 | 55,272 |
| w8308 + `tailorder` | 57232 | 55,193 |
| **w8308 + `tailorder` + `rgd` + `dupreset` (landed)** | **57232** | **55,171** |
| w8308 + `tailorder` + `rgd` + `dupreset` + `nonull` | 57216 | 54,637 |
| w8308 + `tailorder` + `rgd` + `dupreset` + `loops` | 57248 | 54,925 |
| w8308 + `tailorder` + `nstate` + `rgd1` + `loops` + `turret` | 57232 | 54,856 |

`nonull` (drop the `ship != nullptr` checks in the two case-16 wormhole loops;
the original has no `cbz` there) is a faithful -16 B lever but overshoots to
57216.  `loops` (re-fetch `m_pLevel->getEnemies()` in the three loops) is a
faithful +32 B lever.  The combination `nonull + loops` is +40, not +16, so the
57232 landing with `nonull` needs a +16 B companion that has not been found.

The 54,856 B exact-size combination uses `turret` (removes the PLT-order
auto-turret anchor), which re-orders `.rel.plt` and drops `byte_exact` to 4506,
plus the unfaithful `rgd1`/`nstate` levers; it is not landable.

## Open leads

1. `tailorder + rgd + dupreset + nonull` is 57216 B / 54,637 raw diff.  A
   faithful **+16 B** companion would land it at 57232 with a raw diff 435 B
   below master.  `loops` is +32 on this base; `ncm` is already present
   (10/10 calls).
2. The case-154 state-4 teardown call order (w8306 `move4b`) is still unfixed.
3. The mission-0 state-0 double-index loop (w8306) still costs +60 B because it
   evicts `this` from `sl`.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
cmake --build cmake-build-match --target drift -j6      # 0 fields
python3 /dev/shm/w8404/fdiff.py _ZN11LevelScript7processEi   # 55171
python3 /dev/shm/w8404/secdiff.py                           # .text 85575, extab 1396
```

Scratch helpers in `/dev/shm/w8404/` (not tracked): `apply.py` (named-edit
harness), `sweep.sh` (combination runner), `search.py`/`search2.py` (random
subset search), `fdiff.py`, `secdiff.py`, `census.py`.
