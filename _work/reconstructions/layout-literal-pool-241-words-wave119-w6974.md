# Layout::Layout C1/C2 — wave 119 / worker w6974
## The 241-word post-`pop` region: full census, and the missing / duplicated constants

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (identical bodies, aliased at
`0xd1028`), original size **7072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave119-w6974`, base = wave-119 master `be3034dc1`.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.

This pass independently re-derives the complete post-`pop` census, matches all
**241** words to source constants / source constructs, and states exactly which
constants are *missing* from our pool and which are *duplicated* relative to the
original.  It also records a set of new measured source variants (ascending-order
rewrites, targeted statement moves, and 5,500 random single swaps) that all fail
to beat the base at 7072 B.

## Result — note only, no source change landed

| metric | before (base) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| unrounded `difflib` ratio | 48.963317384370 % | **48.963317384370 %** |
| `report.json` pct (1 dp) | 49.0 | 49.0 |
| `byte_exact` / `linked_exact` | 4505 / 4505 | **4505 / 4505** |
| `size_exact` / `data_exact` | 4524 / 843 | 4524 / 843 |
| imports / lint / drift | 207 / CLEAN / 0 | 207 / CLEAN / 0 |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
No source change landed, so the 27 section headers, the file size
(**2,187,328 B**), `.text` (`0x1337c8`), `.ARM.exidx` (`0x6f70`),
`.ARM.extab` (`0x26298`, exact), `.data` (`0x296c`) and `.rel.plt` (`0x72d0`)
are unchanged.  `tools/verify_baseline.json` was **not** touched.

## 1. The 241 words are 20 cold-block words + 221 literal-pool words

The last hot instruction is `pop {r4,r5,r6,r7,pc}`.  Everything after it is the
outlined cold region:

| | `pop` | region after `pop` | cold blocks | pad | literal pool | pool words |
|---|---:|---:|---:|---:|---:|---:|
| **original** | `0xd2800` | **966 B** (`0xd2802..0xd2bc8`) | `0xd2802..0xd2852` = **80 B** | 2 B | `0xd2854..0xd2bc8` = **884 B** | **221** |
| HEAD | `0xd28bc` | **778 B** (`0xd28be..0xd2bc8`) | `0xd28be..0xd28ce` = **16 B** | 2 B | `0xd28d0..0xd2bc8` = **760 B** | **190** |
| delta | | **+188 B** | **+64 B** (16 words) | 0 | **+124 B** | **+31** |

`966/4 = 241.5`, `778/4 = 194.5`; the brief's "241 words" is the whole post-`pop`
region (`20` cold-block instruction words + `221` pool words).  The literal pool
itself is 221 vs 190 words; the other 16 words are the four outlined `iPad` tails
in the original vs one outlined `retinaDisplay` tail in HEAD.

## 2. The 20 cold-block words (original `0xd2802..0xd2852`)

These are instructions, not constants; each is the final `Globals::iPad ? A : B`
arm of one source construct:

| addr | encoding | instruction | source construct |
|---|---:|---|---|
| `0xd2802` | `0x9e01` | `ldr r6,[sp,#4]` | block A: `field_0x130`/`field_0x134`/`v138` final `iPad` arm |
| `0xd2804` | `0x226c` | `movs r2,#108` | block A |
| `0xd2806` | `0x7831` | `ldrb r1,[r6,#0]` | block A |
| `0xd2808` | `0x2900` | `cmp r1,#0` | block A |
| `0xd280a` | `0xf04f0138` | `mov.w r1,#56` | block A |
| `0xd280e` | `0xbf08` | `it eq` | block A |
| `0xd2810` | `0x2250` | `moveq r2,#80` | block A |
| `0xd2812` | `0xbf08` | `it eq` | block A |
| `0xd2814` | `0x2128` | `moveq r1,#40` | block A |
| `0xd2816` | `0xe9c0124c` | `strd r1,r2,[r0,#304]` | block A: `0x130=56:40`, `0x134=108:80` |
| `0xd281a` | `0x212f` | `movs r1,#47` | block A |
| `0xd281c` | `0x7832` | `ldrb r2,[r6,#0]` | block A |
| `0xd281e` | `0x2a00` | `cmp r2,#0` | block A |
| `0xd2820` | `0xbf08` | `it eq` | block A |
| `0xd2822` | `0x2123` | `moveq r1,#35` | block A: `v138=47:35` |
| `0xd2824` | `0xf7ffba49` | `b.w 0xd1cba` | block A join |
| `0xd2828` | `0x7831` | `ldrb r1,[r6,#0]` | block B: `t_field_0x2c8` final `iPad` arm |
| `0xd282a` | `0x262d` | `movs r6,#45` | block B |
| `0xd282c` | `0x2900` | `cmp r1,#0` | block B |
| `0xd282e` | `0xbf08` | `it eq` | block B |
| `0xd2830` | `0x261e` | `moveq r6,#30` | block B: `45:30` |
| `0xd2832` | `0xf7febfef` | `b.w 0xd1814` | block B join |
| `0xd2836` | `0x7819` | `ldrb r1,[r3,#0]` | block C: `field_0x2d0` final `iPad` arm |
| `0xd2838` | `0x222d` | `movs r2,#45` | block C |
| `0xd283a` | `0x2900` | `cmp r1,#0` | block C |
| `0xd283c` | `0xbf08` | `it eq` | block C |
| `0xd283e` | `0x221e` | `moveq r2,#30` | block C: `45:30` |
| `0xd2840` | `0xf7ffb80e` | `b.w 0xd1860` | block C join |
| `0xd2844` | `0x7831` | `ldrb r1,[r6,#0]` | block D: `field_0x60` final `iPad` arm |
| `0xd2846` | `0x2640` | `movs r6,#64` | block D |
| `0xd2848` | `0x2900` | `cmp r1,#0` | block D |
| `0xd284a` | `0xbf08` | `it eq` | block D |
| `0xd284c` | `0x262c` | `moveq r6,#44` | block D: `64:44` |
| `0xd284e` | `0xf7febf42` | `b.w 0xd16d6` | block D join |
| `0xd2852` | `0xbf00` | `nop` | 2-byte alignment pad |

Block A is 38 B; blocks B/C/D are 14 B each → 80 B.  HEAD has only one 16-B
outlined tail (the `field_0x1ac` retina/default arm at `0xd28be`).

## 3. All 221 original literal-pool words

`vld1.64` groups are one `add/addw rX,pc,#imm` + `vld1.64 {dA-dB}`; the four
words are the four consecutive field values of the named store window.  Scalar
`vldr` words are single float constants; `ldr` words are GOT-relative deltas.

### 3a. The 50 end-pool `vld1.64` groups (200 words)

| pool addr | store dst | 4 words |
|---|---|---|
| `0xd2890` | `0xb0` | `[4, 22, 25, 12]` |
| `0xd28a0` | `0xc0` | `[11, 11, 10, 270]` |
| `0xd28b0` | `0xec` | `[60, 120, 300, 600]` |
| `0xd28c0` | `0x294` | `[360, 146, 8, 6]` |
| `0xd28d0` | `0x2a4` | `[198, 180, 189, 54]` |
| `0xd28e0` | `0x118` | `[13, 37, 22, 2]` |
| `0xd28f0` | `0x108` | `[4294967295, 2, 1, 4294967295]` |
| `0xd2900` | `0x128` | `[2, 22, 60, 120]` |
| `0xd2910` | `0x140` | `[19, 19, 19, 243]` |
| `0xd2920` | `0xc0` | `[13, 14, 10, 360]` |
| `0xd2930` | `0xc0` | `[6, 8, 4, 180]` |
| `0xd2940` | `0xb0` | `[6, 29, 36, 16]` |
| `0xd2950` | `0xb0` | `[3, 15, 18, 8]` |
| `0xd2960` | `0x108` | `[4294967294, 2, 1, 4294967295]` |
| `0xd2970` | `0x108` | `[0, 1, 2, 1]` |
| `0xd2980` | `0x2a0` | `[24, 220, 200, 210]` |
| `0xd2990` | `0xec` | `[80, 160, 400, 800]` |
| `0xd29a0` | `0xec` | `[40, 80, 200, 400]` |
| `0xd29b0` | `0x2a0` | `[12, 110, 100, 105]` |
| `0xd29c0` | `0x13c` | `[34, 22, 26, 4]` |
| `0xd29d0` | `0x13c` | `[17, 11, 13, 2]` |
| `0xd29e0` | `0x16c` | `[262, 248, 84, 120]` |
| `0xd29f0` | `0x180` | `[30, 346, 30, 446]` |
| `0xd2a30` | `0x1d0` | `[10, 24, 8, 20]` |
| `0xd2a40` | `0x1d0` | `[5, 12, 4, 10]` |
| `0xd2a50` | `0x1c0` | `[132, 38, 58, 72]` |
| `0xd2a60` | `0x1c0` | `[66, 19, 29, 36]` |
| `0xd2a70` | `0x1b0` | `[26, 60, 66, 2]` |
| `0xd2a80` | `0x1b0` | `[13, 30, 33, 1]` |
| `0xd2a00` | `0x1b0` | `[19, 45, 40, 2]` |
| `0xd2a10` | `0x1d0` | `[7, 18, 6, 14]` |
| `0xd2a20` | `0x1c0` | `[80, 34, 43, 54]` |
| `0xd2a90` | `0x204` | `[12, 5, 7, 5]` |
| `0xd2aa0` | `0x214` | `[1, 7, 8, 7]` |
| `0xd2ab0` | `0x224` | `[28, 49, 2, 1]` |
| `0xd2ac0` | `0x23c` | `[21, 7, 4, 239]` |
| `0xd2ad0` | `0x24c` | `[4294967294, 1, 5, 2]` |
| `0xd2ae0` | `0x25c` | `[22, 1, 421, 11]` |
| `0xd2af0` | `0x26c` | `[92, 98, 136, 421]` |
| `0xd2b70` | `0x264` | `[600, 16, 132, 140]` |
| `0xd2b80` | `0x264` | `[300, 8, 66, 70]` |
| `0xd2b90` | `0x23c` | `[30, 10, 6, 340]` |
| `0xd2ba0` | `0x23c` | `[15, 5, 3, 170]` |
| `0xd2b00` | `0x204` | `[16, 7, 7, 6]` |
| `0xd2b10` | `0x214` | `[4294967294, 7, 10, 7]` |
| `0xd2b20` | `0x224` | `[30, 52, 4, 2]` |
| `0xd2b30` | `0x23c` | `[27, 9, 5, 306]` |
| `0xd2b40` | `0x24c` | `[4294967295, 2, 7, 4]` |
| `0xd2b50` | `0x25c` | `[28, 0, 450, 12]` |
| `0xd2b60` | `0x26c` | `[99, 105, 145, 450]` |

The `dst` column is the `vst1.32` destination offset.  Every group is a
`(config, 4-field window)` pair; the values match the current source's constants
for that config/field exactly (see §5).

### 3b. The 7 GOT deltas and 14 scalar floats (21 words)

| pool addr | word | resolved symbol / value | source |
|---|---:|---|---|
| `0xd2864` | `0x0013df2e` | `_ZN7Globals9iPadLargeE` | `Globals::iPadLarge` |
| `0xd2868` | `0x0013de3c` | `_ZN7Globals4iPadE` | `Globals::iPad` |
| `0xd286c` | `0x0013ddb4` | `_ZN7Globals4iPadE` | `Globals::iPad` |
| `0xd2870` | `0x0013dd96` | `_ZN7Globals9iPadLargeE` | `Globals::iPadLarge` |
| `0xd287c` | `0x0013dc9a` | `_ZN7Globals4iPadE` | `Globals::iPad` |
| `0xd2888` | `0x0013dc04` | `_ZN7Globals4iPadE` | `Globals::iPad` |
| `0xd288c` | `0x0013db84` | `_ZN7Globals4iPadE` | `Globals::iPad` |
| `0xd2854` | `0x3ea66666` | `0.325f` | `field_0x90` retina/iPad/n9 arm |
| `0xd2858` | `0x3f266666` | `0.65f` | `field_0x90` default arm |
| `0xd285c` | `0x42c80000` | `100.0f` | `field_0x9c` retina arm |
| `0xd2860` | `0x42480000` | `50.0f` | `field_0x9c` default arm |
| `0xd2874` | `0x438ca000` | `281.25f` | `field_0x2f8` iPadHD arm |
| `0xd2878` | `0x43c80000` | `400.0f` | `field_0x2f8` iPadLarge arm |
| `0xd2880` | `0x43480000` | `200.0f` | `field_0x2f8` iPad arm |
| `0xd2884` | `0x43160000` | `150.0f` | `field_0x2f8` n9 arm |
| `0xd2bb0` | `0x43480000` | `200.0f` | `field_0x2f8` retina arm |
| `0xd2bb4` | `0x42c80000` | `100.0f` | `field_0x2f8` default arm |
| `0xd2bb8` | `0x43160000` | `150.0f` | `helpWindowY` n9 arm |
| `0xd2bbc` | `0x43100000` | `144.0f` | `helpWindowY` iPad arm |
| `0xd2bc0` | `0x43160000` | `150.0f` | `helpWindowY` retina arm |
| `0xd2bc4` | `0x42980000` | `76.0f` | `helpWindowY` default arm |

`200 + 7 + 14 = 221`.  HEAD has the same 14 scalar-float *count* but two of the
values differ: HEAD's end pool holds `82.0f` and `41.0f` (`field_0xa8` float
arms) where the original holds `0.325f`/`0.65f`; HEAD's `0.325f`/`0.65f` live in
a `.text` island at `0xd2548/0xd254c`.

## 4. What is missing and what is duplicated

### 4a. Group-level multiset diff

| | original | HEAD |
|---|---:|---:|
| end-pool `vld1.64` groups | **50** | **42** |
| end-pool group words | 200 | 168 |
| end-pool scalar floats | 14 | 14 |
| end-pool GOT deltas | **7** | **8** |
| total end-pool words | **221** | **190** |

The 8-group (32-word) deficit is **15 original-only groups minus 7 HEAD-only
groups**.  The 7 HEAD-only groups cancel against 7 of the 15, leaving the net 8.
The remaining word is one extra `Globals::iPadLarge` GOT delta in HEAD.

**15 original-only groups (missing from our pool):**

| config | window | 4 values | source fields |
|---|---|---|---|
| n9 | `0xec` | 60,120,300,600 | `ringNearMidThreshold`,`field_0xf0`,`ringMidFarThreshold`,`field_0xf8` |
| n9 | `0x140` | 19,19,19,243 | `field_0x140`,`0x144`,`0x148`,`0x14c` |
| retina | `0x2a0` | 24,220,200,210 | `field_0x2a0`,`0x2a4`,`0x2a8`,`0x2ac` |
| retina | `0x108` | −2,2,1,−1 | `field_0x108`,`0x10c`,`0x110`,`0x114` |
| retina | `0xec` | 80,160,400,800 | `ringNearMidThreshold`,`0xf0`,`ringMidFarThreshold`,`0xf8` |
| retina | `0xb0` | 6,29,36,16 | `field_0xb0`,`0xb4`,`0xb8`,`0xbc` |
| retina | `0x13c` | 34,22,26,4 | `field_0x13c`,`0x140`,`0x144`,`0x148` |
| retina | `0x1d0` | 10,24,8,20 | `field_0x1d0`,`0x1d4`,`0x1d8`,`0x1dc` |
| iPadLarge | `0x180` | 30,346,30,446 | `field_0x180`,`0x184`,`0x188`,`0x18c` |
| default | `0x2a0` | 12,110,100,105 | `field_0x2a0`,`0x2a4`,`0x2a8`,`0x2ac` |
| default | `0xb0` | 3,15,18,8 | `field_0xb0`,`0xb4`,`0xb8`,`0xbc` |
| default | `0x108` | 0,1,2,1 | `field_0x108`,`0x10c`,`0x110`,`0x114` |
| default | `0xec` | 40,80,200,400 | `ringNearMidThreshold`,`0xf0`,`ringMidFarThreshold`,`0xf8` |
| default | `0x13c` | 17,11,13,2 | `field_0x13c`,`0x140`,`0x144`,`0x148` |
| default | `0x1d0` | 5,12,4,10 | `field_0x1d0`,`0x1d4`,`0x1d8`,`0x1dc` |

**7 HEAD-only groups (duplicated relative to the original):**

| config | window | 4 values | source fields |
|---|---|---|---|
| n9 | `0xdc` | 15.0f,1.5f,12.0f,1.5f (float) | `field_0xdc`,`animSpeedScale`,`driftSpeedDivisor`,`ringRadiusScale` |
| n9 | `0x16c` | 115,106,45,27 | `field_0x16c`,`0x170`,`0x174`,`0x178` |
| retina | `0x294` | 400,168,24,24 | `field_0x294`,`0x298`,`0x29c`,`0x2a0` |
| retina | `0x16c` | 138,128,60,44 | `field_0x16c`,`0x170`,`0x174`,`0x178` |
| iPad | `0x16c` | 131,124,42,60 | `field_0x16c`,`0x170`,`0x174`,`0x178` |
| default | `0x294` | 200,84,12,12 | `field_0x294`,`0x298`,`0x29c`,`0x2a0` |
| default | `0x16c` | 69,64,30,22 | `field_0x16c`,`0x170`,`0x174`,`0x178` |

The 16-byte window-shift pairs (`original 0x204/0x214/0x224/0x23c/0x24c/0x25c/0x26c`
vs HEAD `0x208/0x218/0x228/0x240/0x250/0x260/0x270`) are value-identical and
cancel in the net count; they are not part of the 8.

### 4b. Scalar constants: values correct, placement/type differs

* `0.325f`/`0.65f` (`field_0x90`): same values, original in end pool, HEAD in a
  `.text` island.
* `1.5f` (`animSpeedScale`,`ringRadiusScale`): original uses the
  `mov.w r3,#0x3fc00000` immediate; HEAD puts `1.5f` in the end pool twice.
* `15.0f` (`field_0xdc`) and `12.0f` (`driftSpeedDivisor`): original in `.text`
  islands; HEAD in the end pool (the float SLP group above).
* `41.0f`/`82.0f` (`field_0xa8`): a genuine source-type mismatch.  The original's
  `field_0xa8` expression is an **int** chain
  (`movs r2,#82` / `moveq r2,#41` / `vmov s0,r2` / `vcvt.f32.s32` /
  `vcvt.s32.f32` / `vstr`).  HEAD uses float arms (`82.0f`,`41.0f`), so clang
  materialises them in the pool and emits an extra `vcvt`.  A faithful shape is

  ```cpp
  int v = Globals::iPadHD ? (int)(61 * 1.40625f)
        : (Globals::iPadLarge ? 122 : (Globals::n9 ? 61 :
           (Globals::iPad ? 61 : (Globals::retinaDisplay ? 82 : 41))));
  this->field_0xac = v;
  this->field_0xa8 = (int)(float)v;
  ```

  but this shape measures 7024 B (see §6), so it is not landable at 7072 B.

### 4c. GOT: HEAD has one extra end-pool `iPadLarge` delta

| | original | HEAD |
|---|---:|---:|
| `_ZN7Globals9iPadLargeE` | 2 | **3** |
| `_ZN7Globals4iPadE` | 5 | 5 |
| total | **7** | **8** |

The extra HEAD site is the `field_0x2f4`/`field_0x2f8` pair: HEAD tests
`Globals::iPadLarge` twice, the original once.  Sharing the test changes the body
size (see §6).

## 5. Every group maps to a real source constant — no wrong values

The multiset diff of *values* between the two pools contains only the extra
group values listed in §4a plus the scalar placement/type differences of §4b.
In particular every original-only group value is present in the current source
for the stated config/field, e.g.:

* `0xb0..0xbc` n9 = `4,22,25,12` (source lines 71/78/77/83);
* `0xc0..0xcc` n9 = `11,11,10,270`;
* `0xec..0xf8` retina = `80,160,400,800` (lines 100/85/94/89);
* `0x13c..0x148` retina = `34,22,26,4` (lines 139/297/287/143);
* `0x108..0x114` default = `0,1,2,1` (lines 115/111/104/114);
* `0x1d0..0x1dc` retina = `10,24,8,20` (lines 188/190/186/191);
* `0x2a0..0x2ac` retina = `24,220,200,210` (lines 92/101/97/99);
* `0x16c..0x178` iPadLarge = `262,248,84,120` (lines 152/155/153/151);
* `0x180..0x18c` iPadLarge = `30,346,30,446` (lines 173/157/172/175).

So the "missing constants" are **not missing source values**: they are the
original's extra SLP-vectorised arms.  Conversely the "duplicated constants" are
the HEAD-only SLP groups, not duplicated source values.  The only real
source-level defect is the `field_0xa8` int-vs-float type (§4b).

## 6. Measured variants (all real single-TU link+strip builds)

Scorer: `/dev/shm/w6974/score_raw.py` — the `tools/verify/verify.py`
`Resolver` + `reconcile` + `difflib(autojunk=False)` pipeline on
`_ZN6LayoutC1Ev`, unrounded.  It reproduces the full-link number bit-for-bit
(base `7072 / 48.963317384370`).

### 6a. Ascending-order rewrites (all shrink the body)

| variant | source change | size | ratio % |
|---|---|---:|---:|
| base | — | **7072** | **48.963317384370** |
| `asc22` | sort lines 14–35 (fields `0x4..0x58`) by offset | 7068 | 24.775225 |
| `asc60` | sort lines 60–115 (fields `0x7c..0x2b8`) by offset | 7068 | 34.9 |
| `ascall` | sort all 203 top-level ctor statements by offset | 6888 | 20.3 |
| `grp90` | regroup `0x90,0x94,0x98,0x9c,0xa0,0xa4,0xa8,0xac` ascending | 7008 | 40.9 |

### 6b. Targeted statement moves (single line, full sweep of insertion points)

| moved line | field | 7072-B insertion points | best 7072-B ratio % |
|---|---|---:|---:|
| 187 | `field_0x2c0` | 7 | 47.3 |
| 284 | `field_0x94` | 15 | 48.8 |
| 93 | `field_0xdc` | 1 | 38.7 |

Moving `field_0x2c0` next to `field_0x2c4` (line 44) gives 7032 B / 45.3 %;
moving `field_0x94` next to `0x98/0x9c` gives 7040 B / 46.2 %; moving
`field_0xdc` next to `centerYBase` gives 6968 B / 40.2 %.  All three are the
source-order changes the original's `strd 0x2c0`, `stmia 0x94` and `stmia 0xd0`
imply, but each shrinks the body.

### 6c. Random single swaps (5,500 builds)

| search | variants | 7072-B hits | best 7072-B ratio % |
|---|---:|---:|---:|
| random move (all `this->` lines) | 2,500 | 65 | 48.953558 |
| random swap (all `this->` lines) | 2,500 | 120 | 48.973081 |
| random swap (unconditional lines only) | 3,000 | 153 | 48.953558 |

The single 48.973081 hit (`rswap/s1065.cpp`) is the swap of
`field_0x134 = iPad ? 108 : 80` (line 135, inside the `0x130` chain's final
`else`) with `field_0x190 = -4` (line 166, inside the `0x190` chain's `n9` arm).
It is **semantically wrong** (it moves the `0x134` assignment into the `n9` arm
of the `0x190` chain), so it is not landable as faithful C++; it is recorded only
as the best raw 7072-B score seen.  No semantically neutral swap beats the base.

### 6d. `field_0xa8` int-shape variants

| variant | source change | size | ratio % |
|---|---|---:|---:|
| A | one `int v`; `0xac=v`; `0xa8=(int)(float)v` | 7024 | 48.8 |
| B | one `int v`; `0xa8` first; `0xac=v` | 7016 | 48.0 |
| C | one `int v`; `0xac=v`; `0xa8=v` | 7016 | 49.0 |
| D | two int expressions; `0xac` first | 7032 | 48.8 |
| E | two int expressions; `0xa8` first | 7008 | 47.9 |
| F | one `int v`; both `(int)(float)v` | 7016 | 48.0 |
| G | one `int v`; both `(int)(float)v`, `0xac` first | 7024 | 48.8 |

All remove the `82.0f`/`41.0f` pool words but shrink the body below 7072 B.

## 7. Proven vs hypothesis

**Proven (measured):**

* The original post-`pop` region is 966 B = 20 cold-block words + 2 B pad +
  221 literal-pool words; HEAD is 778 B = 4 cold words + 2 B pad + 190 words.
* The 221 original pool words are fully enumerated: 50 `vld1.64` groups
  (200 words), 14 scalar floats, 7 GOT deltas.
* HEAD has 42 groups (168 words), 14 scalar floats, 8 GOT deltas.
* The net group deficit is **8 groups = 32 words** = 15 original-only groups
  minus 7 HEAD-only groups; the end-pool deficit is **31 words** after the
  extra `iPadLarge` GOT delta.
* Every original-only group value is a real value of the current source for the
  stated config/field; every HEAD-only group is a real current-source value.
  There are no wrong constants.
* The only genuine source-type mismatch is `field_0xa8`: original uses an int
  chain, HEAD uses float arms (`82.0f`/`41.0f` in the pool).
* The base is a local optimum for the official fuzzy metric among 5,500 random
  single moves/swaps and all targeted ascending/move variants measured here.

**Hypothesis (not proven):**

* The original source is config-major (`if (iPadHD) … else if (n9) … else …`)
  with the `else` arm keeping per-field retina/iPad/default selects.  That is
  why the original vectorises the retina/default arms of `0xb0`, `0x108`,
  `0xec`, `0x13c`, `0x1d0`, `0x2a0` and the n9/iPadLarge arms of `0x140`/`0x180`,
  while HEAD vectorises the n9/iPad arms of `0x16c` and the retina/default arms
  of `0x294`.  Reproducing that shape needs the original statement order *and*
  the original expression types; every reorder measured here shrinks the body.
* The original's four outlined `iPad` tails are a consequence of that
  config-major layout: the `iPad ? A : B` arms are the last `else` of the
  per-field selects and clang places them after `pop`.  HEAD outlines only the
  `field_0x1ac` retina/default tail because its field-major source gives the
  block placer a different hot/cold profile.

## 8. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6974
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6     # RATCHET PASS, byte 4505

OBJ=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OBJ -d --start-address=0xd1028 --stop-address=0xd2bc8 \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so > /dev/shm/w6974/orig.dis
$OBJ -d --start-address=0xd1028 --stop-address=0xd2bc8 \
  cmake-build-match/libgof2hdaa.so > /dev/shm/w6974/ours.dis

python3 /dev/shm/w6974/extract.py <so> 0xd2854 0xd2bc8   # raw pool words
python3 /dev/shm/w6974/groups.py orig.dis <so>            # 50 end-pool groups
python3 /dev/shm/w6974/groups.py ours.dis <so>            # 42 end-pool groups
python3 /dev/shm/w6974/stores2.py orig.dis <so>           # store order trace
python3 /dev/shm/w6974/diffmult.py                        # value multiset diff
bash    /dev/shm/w6974/fast.sh <variant.cpp>              # single-TU build + score
```

Scratch harnesses (not committed, `/dev/shm/w6974/`): `extract.py`,
`extractlib.py`, `groups.py`, `stores.py`, `stores2.py`, `diffmult.py`,
`score.py`, `score_raw.py`, `fast.sh`, `fast2.sh`, `mkvar.py`,
`split_ctor.py`, `gen_asc.py`, `gen_sort.py`, `move.py`, `regrp.py`,
`mk_a8.py`, `sweep_move.py`, plus the variant trees `variants*`, `rand`,
`rswap`, `uswap` and the disassemblies `orig.dis`, `ours.dis`.
