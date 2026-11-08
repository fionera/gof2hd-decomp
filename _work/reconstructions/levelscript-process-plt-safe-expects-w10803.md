# LevelScript::process — wave 162 / w10803: PLT-safe `__builtin_expect` table

Target `_ZN11LevelScript7processEi` (`LevelScript::process`), original `0x137d18`,
**57,232 B (0xdf90)**, size-exact.  Owned TU `src/game/world/LevelScript.cpp`.
Worktree `wt/w10803`, base master `29248c1d0` (wave 161).  Scratch `/dev/shm/w10803/`.

## Baseline (wave-161 tree, unchanged)

```
raw diffbytes   55,013
aligned insns   17,308
orig/ours size  57,232 / 57,232
.text diff      85,374
.ARM.extab      725
.rel.plt        0 (BYTE-IDENTICAL)
byte_exact      4507/4524   linked_exact 4507/4524   size 4524/4524
data_exact      843/843     imports 207              lint CLEAN   drift 0
```

## Why w10705's L1526:0 no longer applies

w10705 (wave-160 base) reported `expect(mission==51 && messages!=nullptr, 0)` as a
size-exact raw win (55,048 -> 54,959) that rotated 16 `.rel.plt` bytes.  On the
wave-161 base (after the mission-89 fix) the same hint gives **55,318 / size
57,248 (+16)** and the same 16-byte `.rel.plt` rotation, so it is now gate-fatal.
Its counterpart `expect(..., 1)` is the PLT-safe size-exact cell (see below).

## Full current-base sweep (one guard at a time, full link)

Every cell below was built and linked; the `.rel.plt` byte diff was measured each
time.  `plt` is the `.rel.plt` differing-byte count (0 = byte-identical).
Baseline row: raw 55,013 / aligned 17,308 / size 57,232 / plt 0.

| guard | e | raw | aligned | fn size | plt |
|---|---:|---:|---:|---:|---:|
| mission0 (L615) | 0 | 55,532 | 22,004 | 57,444 | 56 |
| mission0 | 1 | 55,662 | 22,002 | 57,436 | 76 |
| intro (L620) | 0 | 55,489 | 22,314 | 57,384 | 52 |
| intro | 1 | 55,581 | 19,434 | 57,480 | 65 |
| msg6 (L652) | 0 | 55,192 | 17,349 | 57,240 | 0 |
| msg6 | 1 | **55,013** | 17,308 | 57,232 | 0 |
| msg7 (L669) | 0 | 55,480 | 17,317 | 57,236 | 0 |
| msg7 | 1 | **55,013** | 17,308 | 57,232 | 0 |
| msg10 (L686) | 0 | **55,013** | 17,308 | 57,232 | 0 |
| msg10 | 1 | 55,426 | 19,911 | 57,200 | 52 |
| msg13 (L727) | 0 | 55,209 | 17,303 | 57,240 | 0 |
| msg13 | 1 | **55,013** | 17,308 | 57,232 | 0 |
| msg14 (L730) | 0 | **55,013** | 17,308 | 57,232 | 0 |
| msg14 | 1 | 55,422 | 19,911 | 57,200 | 52 |
| msg8 (L739) | 0/1 | **55,013** | 17,308 | 57,232 | 0 |
| msg17 (L882) | 0 | 55,021 | 18,344 | 57,252 | 0 |
| msg17 | 1 | 55,441 | 21,465 | 57,192 | 52 |
| msg22 (L919) | 0 | 55,104 | 18,828 | 57,248 | 8 |
| msg22 | 1 | 55,061 | 18,342 | 57,252 | 0 |
| state16 (L929) | 0 | 55,406 | 18,814 | 57,244 | 0 |
| state16 | 1 | 55,084 | 18,790 | 57,244 | 8 |
| !isFading (L930) | 0 | 55,442 | 17,544 | 57,240 | 0 |
| !isFading | 1 | 54,872 | 18,427 | 57,264 | 0 |
| msg2 (L961) | 0/1 | **55,013** | 17,308 | 57,232 | 0 |
| state1&&!fading (L965) | 0 | 55,149 | 18,701 | 57,452 | 18 |
| state1&&!fading | 1 | 55,289 | 18,701 | 57,552 | 38 |
| msg3 (L1023) | 0 | 55,160 | 17,305 | 57,240 | 0 |
| msg3 | 1 | 55,049 | 17,365 | 57,244 | 0 |
| !isFading (L1030) | 0 | **55,013** | 17,308 | 57,232 | 0 |
| !isFading | 1 | **55,010** | 17,308 | 57,232 | 0 |
| msg2 (L1356) | 0/1 | **55,013** | 17,308 | 57,232 | 0 |
| state3&&msg4trig (L1380) | 0 | **55,013** | 17,308 | 57,232 | 0 |
| state3&&msg4trig | 1 | 55,109 | 17,556 | 57,216 | 0 |
| state4&&msg4over (L1383) | 0 | 55,013 | 17,420 | 57,240 | 0 |
| state4&&msg4over | 1 | 55,021 | 17,325 | 57,244 | 0 |
| state6&&msg5over (L1395) | 0 | 55,113 | 17,399 | 57,236 | 0 |
| state6&&msg5over | 1 | **55,013** | 17,308 | 57,232 | 0 |
| state0 (L1425) | 0 | 55,065 | 17,318 | 57,224 | 0 |
| state0 | 1 | 55,119 | 17,346 | 57,224 | 0 |
| msg0 (L1430) | 0 | 55,141 | 17,294 | 57,232 | 0 |
| **msg0 (L1430)** | **1** | **54,989** | 17,312 | **57,232** | **0** |
| m_bStartSequence (L1455) | 0 | 55,391 | 16,940 | 57,268 | 9 |
| m_bStartSequence | 1 | 55,514 | 18,411 | 57,436 | 11 |
| state0 (L1507) | 0 | 55,399 | 17,372 | 57,224 | 16 |
| state0 | 1 | **55,013** | 17,308 | 57,232 | 0 |
| msg1trig (L1517) | 0/1 | **55,013** | 17,308 | 57,232 | 0 |
| **mission50 (L1522)** | **0** | **55,007** | 17,338 | **57,232** | **0** |
| mission50 | 1 | 55,397 | 17,344 | 57,252 | 0 |
| **mission51 (L1526)** | 0 | 55,318 | 17,486 | 57,248 | 16 |
| **mission51** | **1** | **54,994** | 17,347 | **57,232** | **0** |
| count3 (L1532) | 0 | 55,275 | 17,378 | 57,232 | 0 |
| count3 | 1 | 55,399 | 17,372 | 57,224 | 16 |
| state4..6 (L4026) | 0 | 55,088 | 21,170 | 57,264 | 0 |
| state4..6 | 1 | 55,016 | 17,315 | 57,232 | 0 |

**PLT-safe, size-exact, raw-improving cells (single):** `L1030:1` (55,010),
`L1430:1` (54,989), `L1522:0` (55,007), `L1526:1` (54,994).
Everything else is either neutral (hint ignored, identical code) or
size-changing / PLT-rotating.

## Extra flippers found outside the w10705 table

The wave-161 base changed the block layout enough that several guards the old
table did not list now flip.  Swept with the four-cell base combo applied:

| guard | e | raw | aligned | fn size | plt |
|---|---:|---:|---:|---:|---:|
| m_nScriptTimerA>=3001 (L1367) | 0 | 55,147 alone | 17,415 | 57,228 | 0 |
| state5 (L1389) | 1 | 54,848 | 17,433 | 57,224 | 0 |
| state>=2&&<=4 (L1410) | 1 | 54,948 | 17,352 | 57,232 | 0 |
| msg0 (L1430) | 1 | 54,989 | 17,312 | 57,232 | 0 |
| !isFading (L1030) | 1 | 55,010 | 17,308 | 57,232 | 0 |

`L1367:0` is the biggest single new lever but is **-4 B alone** (57,228);
combined with `L1430:1` it is 57,224, with `L1526:1` 57,220, and only the
three together restore **57,232**.

## Landed combination

```
L1030  if (__builtin_expect(!Globals::layout->isFading(), 1))            // mission-78 state 3
L1367  if (__builtin_expect(m_nScriptTimerA >= 3001, 0))                  // mission-106 state 2
L1430  if (__builtin_expect(((*messages)[0])->isOver(), 1))               // mission-95/99/... state 1
L1526  } else if (__builtin_expect(getCurrentCampaignMission() == 51 && messages != nullptr, 1))
```

Measured (full link):

| | raw | aligned | fn size | .text | .rel.plt |
|---|---:|---:|---:|---:|---:|
| base | 55,013 | 17,308 | 57,232 | 85,374 | 0 |
| **landed** | **54,698** | **17,456** | **57,232** | **85,059** | **0** |

Raw **-315**, aligned **+148** (indicative only; the raw metric is the
byte-identicality ground truth).  `.rel.plt` stays byte-identical, every
section size is exact, `check` green: byte_exact 4507/4524, linked_exact
4507/4524, size 4524/4524, data 843/843, imports 207, lint CLEAN.

### Combination search (all PLT-safe, size-exact)

| combo | raw | aligned |
|---|---:|---:|
| L1430:1 | 54,989 | 17,312 |
| L1526:1 | 54,994 | 17,347 |
| L1522:0 | 55,007 | 17,338 |
| L1030:1 | 55,010 | 17,308 |
| L1430:1+L1526:1 | 54,956 | 17,351 |
| L1430:1+L1526:1+L1030:1 | 54,953 | 17,351 |
| +L1367:0 (landed) | **54,698** | **17,456** |
| +L1410:1 on landed | 54,699 | 17,456 |
| +L1389:1 on landed | 54,848 | 17,433 (size 57,224) |

## Reproduction

```sh
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10803/fdiff.py _ZN11LevelScript7processEi        # 54,698 / 57,232
python3 /dev/shm/w10803/align_diff.py _ZN11LevelScript7processEi  # 17,456
python3 /dev/shm/w10803/relplt.py                                  # diffbytes 0
cmake --build cmake-build-match --target check -j6                 # RATCHET PASS
```

## Open questions

1. `L1367:0` is the strongest new lever (-315 raw) but is size-unstable on its
   own (-4 B); it needs the L1430/L1526 pair to land at 57,232.  A real
   code-shape change that absorbs the 4-byte swing would decouple them.
2. `L1389:1` (54,848 at 57,224) and `L1364:0` (54,546 at 57,208 with
   L1389:1) are the best raw values found but are 8/24 B short of size-exact;
   the mission-106 state-5/state-2 region is the next target.
3. The `m_bStartSequence` tie flip (w10705) still costs +20 B and rotates
   `.rel.plt`; it remains blocked on a real -20 B shape change.
