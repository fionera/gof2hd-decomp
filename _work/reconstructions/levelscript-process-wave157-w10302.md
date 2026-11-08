# LevelScript::process — wave 157 / w10302: case-154 graded expect +
# case-14 increment, `.ARM.extab` byte-neutral (1,292)

Target `_ZN11LevelScript7processEi` (`LevelScript::process`), original
`0x137d18`, **57,232 B (0xdf90)**, size-exact.  Owned TU
`src/game/world/LevelScript.cpp`.  Base master `08575efff` (wave 156,
w10203 landed).  Scratch `/dev/shm/w10302/`.

## Result (landed)

```
                         before (HEAD)     after (landed)
process raw diffbytes    55,095            55,088            (-7)
process aligned insns    17,131            17,135            (+4)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
.text diffbytes          85,831            85,824            (-7)
.ARM.extab diffbytes     1,292             1,292             (neutral)
.ARM.extab size          156,312           156,312           (exact)
.ARM.exidx               0                 0                 **BYTE-IDENTICAL**
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

## 1. The landed source change (2 edits)

| # | line | edit | region |
|---|------|------|--------|
| H9b | 4673 | `if (m_nScriptTimerA >= 2001)` -> `__builtin_expect(..., 1)` | case-154 state-9 completion |
| E14b | 1728 | `m_nState = 5` -> `m_nState++` (state is 4) | case-14 state-4 arm |

Both are semantically identical: H9b is a hint-only branch weight, and E14b
increments a known state value.  H9b is the "c154->c157 strong-arm adjacency
with a graded expect that adds no LSDA rows" candidate; E14b is the cheapest
extab-neutral raw companion found in the same row-17 span.

## 2. LSDA row table (deliverable)

Decoded with `_work/reconstructions/extab_lsda_decode.py` (process exidx
record `0x1c2af8`).  The landed record is **byte-for-byte identical to the
pre-change record**: 46 rows, call-site length 324, **182 differing bytes**
in both builds.  The whole-image `.ARM.extab` differing bytes stay at
**1,292** (section size exact 156,312; `.ARM.exidx` byte-identical).

| field | original | base (w10203) | landed |
|---|---:|---:|---:|
| record length | 348 B | 348 B | 348 B |
| `lpStart` enc | `0xff` | `0xff` | `0xff` |
| `ttype` enc | `0xff` | `0xff` | `0xff` |
| call-site enc | `0x01` | `0x01` | `0x01` |
| call-site length | 324 | 324 | 324 |
| call-site rows | 46 | 46 | 46 |
| record differing bytes | — | 182 | **182** |

Row table (orig -> ours; identical in base and landed):

```
 0 (0,5832,0,0)          (0,5654,0,0)
 1 (5832,6,57116,0)      (5654,6,57092,0)
 2 (5838,748,0,0)        (5660,786,0,0)
 3 (6586,8,57080,0)      (6446,8,57074,0)
 4 (6594,1632,0,0)       (6454,1196,0,0)
 5 (8226,8,57106,0)      (7650,8,57094,0)
 6 (8234,664,0,0)        (7658,624,0,0)
 7 (8898,8,57104,0)      (8282,8,57104,0)
 8 (8906,718,0,0)        (8290,132,0,0)
 9 (9624,8,57074,0)      (8422,10,57162,0)
10 (9632,310,0,0)        (8436,10,57152,0)
11 (9942,6,57114,0)      (8446,10,57142,0)
12 (9948,2214,0,0)       (8456,8,57132,0)
13 (12162,8,57098,0)     (8464,1502,0,0)
14 (12170,2054,0,0)      (9966,8,57066,0)
15 (14224,14,57162,0)    (9974,242,0,0)
16 (14242,8,57152,0)     (10216,6,57106,0)
17 (14250,12,57142,0)    (10222,1846,0,0)
18 (14262,8,57132,0)     (12068,8,57096,0)
19 (14270,5718,0,0)      (12076,12430,0,0)
20 (19988,6,57086,0)     (24506,12,57102,0)
21 (19994,4984,0,0)      (24518,2140,0,0)
22 (24978,8,57058,0)     (26658,8,57114,0)
23 (24986,1132,0,0)      (26666,1018,0,0)
24 (26118,12,57096,0)    (27684,8,57116,0)
25 (26130,2794,0,0)      (27692,1784,0,0)
26 (28924,8,57108,0)     (29476,10,57086,0)
27 (28932,328,0,0)       (29486,360,0,0)
28 (29260,8,57064,0)     (29846,6,57072,0)
29 (29268,552,0,0)       (29852,1338,0,0)
30 (29820,10,57090,0)    (31190,8,57108,0)
31 (29830,6658,0,0)      (31198,5178,0,0)
32 (36488,4,57122,0)     (36376,4,57122,0)
33 (36492,2626,0,0)      (36380,1704,0,0)
34 (39118,6,57088,0)     (38084,6,57080,0)
35 (39124,2500,0,0)      (38090,3852,0,0)
36 (41624,8,57068,0)     (41942,8,57060,0)
37 (41632,8,0,0)         (41950,8,0,0)
38 (41640,8,57066,0)     (41958,8,57058,0)
39 (41648,5556,0,0)      (41966,4064,0,0)
40 (47204,8,57056,0)     (46030,8,57056,0)
41 (47212,2470,0,0)      (46038,3994,0,0)
42 (49682,16,57050,0)    (50032,16,57054,0)
43 (49698,4756,0,0)      (50048,4368,0,0)
44 (54454,8,57048,0)     (54416,8,57052,0)
45 (54462,2770,0,0)      (54424,2808,0,0)
```

## 3. The w10103 composition re-tested on the wave-156 base

Edits: `A` = case-125 state-0/1 `else if`; `B` = case-42 arm order
`6,0,7` -> `0,6,7`; `C` = case-42 state-0 `m_nState++`; `D` = case-142
state-0 `m_nState++`; `F` = case-24 wormhole state-1 `m_nState++`.
`E` (case-157 s7 timer expect) is already landed by w10202.

| variant | size | raw | aligned | .ARM.extab |
|---|---:|---:|---:|---:|
| HEAD (w10203) | 57,232 | 55,095 | 17,131 | 1,292 |
| **H9b+E14b (landed)** | **57,232** | **55,088** | **17,135** | **1,292** |
| H9b (c154 expect only) | 57,232 | 55,093 | 17,132 | 1,292 |
| E14b (case-14 ++ only) | 57,232 | 55,090 | 17,134 | 1,292 |
| A (case-125 else-if) | 57,256 | 55,221 | 17,340 | 2,079 |
| A2 (faithful `expect(state==0,1)`+else-if) | 57,340 | 55,412 | 17,429 | 29,521 (+4 B size) |
| A3 (`expect(trig,1)`+else-if) | 57,392 | 55,396 | 16,386 | 2,376 |
| B | 57,224 | 55,153 | 17,110 | 1,297 |
| C | 57,224 | 55,167 | 17,132 | 1,297 |
| D | 57,224 | 55,218 | 17,127 | 1,296 |
| F | 57,240 | 55,205 | 16,982 | 1,975 |
| BC | 57,224 | 55,147 | 17,108 | 1,297 |
| BCD | 57,208 | 55,130 | 17,094 | 1,301 |
| ABC | 57,232 | 55,085 | 17,302 | 1,396 |
| ABCD | 57,216 | 55,215 | 17,289 | 1,399 |
| ABCDEF (w10103) | 57,224 | 55,140 | 17,141 | 1,397 |
| CF | **57,232** | **55,071** | **16,983** | **1,293** |
| BF | 57,232 | 55,105 | 16,956 | 1,293 |
| BCF | 57,232 | 55,108 | 16,954 | 1,293 |
| H9b+CF | 57,232 | 55,069 | 16,984 | 1,293 |
| H9b+BCDF | 57,216 | 55,088 | 16,942 | 1,297 |

### Why the case-125 edit cannot be extab-neutral

* `A` (plain `else-if`) re-scopes the case-125 cleanup cascade and grows the
  process call-site table from 324 to 326 bytes, costing ~787 whole-image
  extab bytes.
* `A2` (faithful `expect(m_nState == 0, 1)` + `else-if`, span 96) grows the
  call-site table to **329 bytes** and the record by 4 bytes, so
  `.ARM.extab` becomes `0x2629c` (+4).  An exhaustive subset sweep of the
  available order-preserving levers (`B C D F H9 H9b`, 64 builds) found **no
  size-exact companion**: the closest is `A2+B+C+D` at 57,240 (+8), and every
  A2 variant keeps `.ARM.extab` at `0x2629c`.  The +4 section growth shifts
  `.rodata` and drops `byte_exact` 4507 -> 4044, so A2 is not landable.
* `F` alone costs +683 extab bytes: its +8 B sits early (case 24) and the
  matching -8 B `E` sits late (case 157), so every landing-pad row between
  them shifts by +8.  Pairing it with `C` (`CF`) cancels the shift after
  case 42 and recovers all but **1 byte** (1,293).  The residual +1 is a
  single coincidence match at record offset 134: row 17's length is 1846
  (`0xb6 0x0e`) in the original and base, but 1850 (`0xba 0x0e`) under `CF`.
  `E21` (-2 in row 17) brings it to 1848, not 1846; no -4 lever was found.

## 4. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10302/fdiff.py _ZN11LevelScript7processEi        # 55,088 / 57,232
python3 /dev/shm/w10302/align_diff.py _ZN11LevelScript7processEi  # 17,135
python3 _work/reconstructions/extab_lsda_decode.py <orig> cmake-build-match/libgof2hdaa.so
cmake --build cmake-build-match --target check -j6                 # RATCHET PASS
```

## 5. Open questions

1. `CF` (57,232 / 55,071 / 16,983) is the best raw/aligned size-exact shape
   measured but is +1 extab.  A -4 B edit inside row 17 (cases 154/24/14/21/42)
   would make row 17's length 1846 again and land it extab-neutral.
2. The faithful case-125 entry needs a companion that shrinks the call-site
   table by 5 bytes (row merge), not just a size companion; none of the
   `m_nState++`/expect levers does that.
3. The case-154 state-0 body is still 290 B shorter than the original
   (`0x13a76e..0x13a930`, 450 B): the original calls `isInRocketControl`,
   `setRumblePercentage`, `autoTurretIsEnabled`/`setAutoTurret` and
   `setComputerControlled` there, which our block omits.  Reconstructing it
   would need a matching -290 B elsewhere to stay size-exact.
