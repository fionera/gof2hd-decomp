# LevelScript::process — wave 158 / w10402: w10302 landed, -4 aligned companion not found

Target `_ZN11LevelScript7processEi` (`LevelScript::process`), original
`0x137d18`, **57,232 B (0xdf90)**, size-exact. Base master `9e4795206`
(wave 157). Scratch `/dev/shm/w10402/`.

## Result (landed)

```
                         before (HEAD)     after (landed)
process raw diffbytes    55,094            55,087            (-7)
process aligned insns    17,129            17,133            (+4)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
.ARM.extab diffbytes     1,292             1,292             (neutral)
.ARM.extab size          156,312           156,312           (exact)
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
RATCHET                  PASS              PASS
```

## 1. Landed source change (w10302, 2 edits)

| # | line | edit | region |
|---|------|------|--------|
| H9b | 4675 | `if (m_nScriptTimerA >= 2001)` -> `__builtin_expect(..., 1)` | case-154 state-9 completion |
| E14b | 1730 | `m_nState = 5` -> `m_nState++` (state is 4) | case-16 state-4 arm (note calls it "case-14"; line 1728 in the 08575efff base) |

H9b changes exactly one instruction: at `0x144af0` `ldr r0,[sp,#84]` becomes
`mov r0,r5` (a global register-allocation change in the state-9 tail), worth
raw -2.  E14b is +4 B in the case-16 state-4 block but repacks the literal
pool (two `nop`s at `0x1408b4` disappear), keeping the function size exact;
it is worth raw -5.

## 2. LSDA row table (deliverable)

Decoded with `_work/reconstructions/extab_lsda_decode.py` (process exidx
record `0x1c2af8`).  The landed record is **byte-for-byte identical to the
pre-change record**: 46 rows, call-site length 324, **182 differing bytes**
in both builds.  `.ARM.extab` stays 1,292 differing bytes, size exact.

| field | original | landed |
|---|---:|---:|
| record length | 348 B | 348 B |
| `lpStart` enc | `0xff` | `0xff` |
| `ttype` enc | `0xff` | `0xff` |
| call-site enc | `0x01` | `0x01` |
| call-site length | 324 | 324 |
| call-site rows | 46 | 46 |
| record differing bytes | — | **182** |

Rows (orig -> ours) are unchanged from the wave-157 note's table.

## 3. The -4 aligned companion search (all measured, full link)

All variants start from the landed w10302 build.  None reached
aligned <= 17,129 while keeping raw <= 55,087, size 57,232 and extab 1,292.

| variant | size | raw | aligned | extab |
|---|---:|---:|---:|---:|
| **w10302 landed** | **57,232** | **55,087** | **17,133** | **1,292** |
| H9b only (E14b reverted) | 57,232 | 55,092 | 17,130 | 1,292 |
| E14b only (H9b reverted) | 57,232 | 55,089 | 17,132 | 1,292 |
| c154 state-8 expect too | 57,232 | 55,087 | 17,139 | 1,292 |
| c154 `shipGroup==9` expect(1) | 57,232 | 55,087 | 17,133 | 1,292 |
| c154 `field_0xaa` expect(1) | 57,232 | 55,087 | 17,133 | 1,292 |
| c16 `shipGroup==9` expect(1) | 57,232 | 55,087 | 17,133 | 1,292 |
| c16 isOver expect(1) | 57,232 | 55,087 | 17,133 | 1,292 |
| c16 translate `0` vs `0.0f` | 57,232 | 55,087 | 17,133 | 1,292 |
| c16 `m_nState++` after loop | 57,240 | 55,396 | 17,252 | — |
| c16 `m_nState++` after reset | 57,248 | 55,377 | 17,181 | — |
| c154 `m_nState = 10` | 57,228 | 55,165 | 17,144 | — |
| c154 swap timer/state | 57,232 | 55,087 | 17,135 | 1,292 |
| c154 `m_bCinematicBreak` after turret | 57,232 | 55,092 | 17,135 | 1,292 |
| c154 timer before cinematic | 57,232 | 55,091 | 17,135 | 1,292 |
| c154 `shipGroup==9` expect(0) | 57,200 | 55,213 | 17,177 | — |
| c16 `shipGroup==9` expect(0) | 57,236 | 55,209 | 17,167 | — |
| c16 isOver expect(0) | 57,232 | 55,405 | 17,279 | — |
| c154 isOver expect(0/1) | 57,248 | 55,200/55,164 | 19,019/19,011 | — |
| c154 state-9 expect(0) | 57,196 | 55,122 | 17,458 | — |
| c14 (real case 14, line 1642) `m_nState++` | 57,224 | 55,160 | **17,128** | — |
| both case-14 + case-16 `++` | 57,224 | 55,155 | 17,131 | — |

The real case-14 state-4 `m_nState++` (line 1642) is the only variant that
beats baseline aligned (17,128), but it shrinks the function by 8 B and costs
raw +73, so it is not landable without a +8 size companion and a raw recovery.

## 4. Why the +4 aligned is hard to remove

The aligned diff change is exactly 3 original instructions becoming
unmatched: `0x14056a ldr r1,[sp,#88]`, `0x14056c movs r0,#5` (E14b) and
`0x1449fc ldr r0,[sp,#84]` (H9b), plus one greedy shift.  These are the very
instructions the two edits rewrite, so any companion must recover 4 matches
elsewhere; no order-preserving statement move in the two blocks did.

## 5. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10402/fdiff.py _ZN11LevelScript7processEi        # 55,087 / 57,232
python3 /dev/shm/w10402/align_diff.py _ZN11LevelScript7processEi  # 17,133
python3 _work/reconstructions/extab_lsda_decode.py <orig> cmake-build-match/libgof2hdaa.so
cmake --build cmake-build-match --target check -j6                 # RATCHET PASS
```

## 6. Open questions

1. The real case-14 state-4 `m_nState++` (line 1642) gives aligned 17,128 but
   size 57,224 and raw 55,160; a +8 size companion that also recovers raw is
   the most promising remaining lever.
2. The +4 aligned is localised to the two rewritten instructions; a faithful
   reconstruction of the case-16 state-4 register allocation (delta in `sl`,
   `this` at `[sp,#88]`) would recover it but is a whole-function RA problem.
