# LevelScript::process — wave 155 / w10202: case-157 completion-increment
# composition, extab-neutral (1,292), raw 55,123 -> 55,096, aligned 17,180 -> 17,131

Target `_ZN11LevelScript7processEi` (`LevelScript::process`), original
`0x137d18`, **57,232 B (0xdf90)**, size-exact.  Owned TU
`src/game/world/LevelScript.cpp`.  Base master `cf74f4ee6` (wave 155, w10102
landed).  Scratch `/dev/shm/w10202/`.

## Result (landed)

```
                         before (HEAD)     after (landed)
process raw diffbytes    55,123            55,096            (-27)
process aligned insns    17,180            17,131            (-49)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
.text diffbytes          86,044            86,017            (-27)
.ARM.extab diffbytes     1,292             1,292             (neutral)
.ARM.extab size          156,312           156,312           (exact)
.ARM.exidx               0                 0
.rel.plt                 0                 0                 **BYTE-IDENTICAL**
.rodata / .data / .bss   exact             exact
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
RATCHET                  PASS              PASS
```

## 1. The landed composition (5 edits, all case 157)

| # | line | edit | state guard |
|---|------|------|-------------|
| K | 4870 | `if (m_nScriptTimerB >= 25001)` -> `__builtin_expect(..., 1)` | c157 s5 |
| M | 4904 | `m_nState = 6` -> `m_nState++` (state is 5) | c157 s5 |
| L | 4924 | `m_nState = 7` -> `m_nState++` (state is 6) | c157 s6 |
| E | 4931 | `if (m_nScriptTimerA <= 9999)` -> `__builtin_expect(..., 1)` | c157 s7 |
| H | 4975 | `m_nState = 9` -> `m_nState++` (state is 8) | c157 s8 |

All are semantically identical: the `++` increments a known state value, and
the expects are hint-only.  Each `__builtin_expect` line carries a same-line
`// lint: arm_builtin` waiver.

## 2. LSDA row table (deliverable)

Decoded with `_work/reconstructions/extab_lsda_decode.py` (process exidx
entry `0x1a12a0`, extab record `0x1c2af8`):

| field | original | ours (landed) |
|---|---:|---:|
| record length | 348 B | 348 B |
| `lpStart` enc | `0xff` | `0xff` |
| `ttype` enc | `0xff` | `0xff` |
| call-site enc | `0x01` | `0x01` |
| call-site length | 324 | 324 |
| call-site rows | **46** | **46** |
| rows with landing pad | 24 | 24 |
| record differing bytes | — | **182** |

The process record diff is **182 bytes, exactly the same as the pre-change
baseline** — the composition shifts no landing-pad row's ULEB encoding.  The
whole-image `.ARM.extab` differing bytes therefore stay at **1,292** (section
size exact 156,312).  This is the "extab-neutral companion" the mission asked
for.

## 3. Why the w10103 ABCDEF composition was rejected

Measured on the current master (wave-155 base, w10102 landed):

| variant | size | raw | aligned | .ARM.extab diff |
|---|---:|---:|---:|---:|
| HEAD | 57,232 | 55,123 | 17,180 | 1,292 |
| **EKMLH (landed)** | **57,232** | **55,096** | **17,131** | **1,292** |
| EF (w10103 core) | 57,232 | 55,076 | 16,982 | 1,293 |
| EFBH | 57,232 | 55,061 | 16,952 | 1,293 |
| EFMBH | 57,232 | 55,064 | 16,952 | 1,293 |
| ABCDEF (w10103) | 57,232 | 55,088 | 17,170 | 1,396 |
| ABDEF (ABCDEF-C) | 57,232 | 55,086 | 17,172 | 1,396 |
| EK | 57,232 | 55,122 | 17,128 | 1,292 |
| EKM | 57,232 | 55,119 | 17,128 | 1,292 |
| EKC | 57,232 | 55,125 | 17,126 | 1,292 |
| EKBD | 57,232 | 55,085 | 17,116 | 1,293 |

`E` = c157 s7 timer expect, `F` = wormhole state-1 `m_nState++`,
`B` = case-42 arm reorder, `C` = case-42 state-0 `++`, `D` = case-142
state-0 `++`, `H`/`K`/`L`/`M` = c157 s8/s5/s6/s5 increments/expects.

The w10103 `F` lever (wormhole state-1 `m_nState++`) is what pushes
`.ARM.extab` from 1,292 to 1,293: it adds +8 B early (case 42) while the
matching -8 B `E` edit sits late (case 157), so every landing-pad row between
them shifts by +8 and its ULEB bytes change.  Dropping `F` and using the
case-157-only increment set keeps the row table byte-neutral.

`EFBH` is the best raw/aligned pair measured (55,061 / 16,952) but costs
`.ARM.extab` +1 (1,293).  It is available if the coordinator prefers the
larger `.text` win over strict extab neutrality.

## 4. Faithful case-125 entry (measured, not landed)

The `expect(m_nState == 0, 1)` + `else-if` case-125 shape (`GA`) is
**57,344 B (+112)**, raw 55,430, aligned 17,427 on this base.  It needs a
-112 B size companion; no order-preserving increment/expect combination
reaches -112 (the available levers are -36 `J`, -8 `E`, -4 `B`/`L`,
+4 `D`/`H`, +8 `F`/`K`/`G`, +16 `I`, +24 `N`).  The `else-if` alone (`A`) is
+24 B and pushes `.ARM.extab` to 2,079 because it re-scopes the case-125
cleanup cascade.  So the case-125 fix remains open.

## 5. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10202/fdiff.py _ZN11LevelScript7processEi        # 55,096 / 57,232
python3 /dev/shm/w10202/align_diff.py _ZN11LevelScript7processEi  # 17,131
python3 /dev/shm/w10202/sect.py <orig> cmake-build-match/libgof2hdaa.so  # extab 1,292
python3 _work/reconstructions/extab_lsda_decode.py <orig> cmake-build-match/libgof2hdaa.so
cmake --build cmake-build-match --target check -j6                 # RATCHET PASS
```

## 6. Open questions

1. The case-125 `else-if`/faithful entry is still the largest aligned
   residual; it needs a -112 B (faithful) or -24 B (else-if) companion that
   also keeps the LSDA row table byte-neutral.  The `else-if` re-scope adds
   ~787 extab diff bytes on its own.
2. `EFBH` (55,061 / 16,952) is the best raw/aligned pair but +1 extab; a
   shape that moves the `F` +8 and `E` -8 edits adjacent in code order would
   recover the extab neutrality and land it.
3. The mission-0 body placement (case 0) remains the only case-order
   difference and still drives the first LSDA row's length (5,654 vs 5,832).
