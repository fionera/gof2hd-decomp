# Layout::Layout C1/C2 — wave 119 / worker w6958
## Full literal-pool census: all 241 post-`pop` words of the original, matched to source constants

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (identical bodies, aliased at
`0xd1028`), original size **7072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave119-w6958`, base = wave-119 master `be3034dc1`.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.

This pass answers the literal-pool question directly: every word in the
original's post-`pop` region (`0xd2802..0xd2bc8`, 966 B = 241.5 words) is
enumerated and matched to the source constant/field that produces it.  The
current HEAD has 778 B = 194.5 words in the same region.  The 188-B (47-word)
gap is **32 SLP group words + 16 cold-block words − 1 GOT word**, not the
older "8 groups + 100.0f + 50.0f − 1 GOT" decomposition (see §4).

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
No source change landed, so the 27 section headers, file size
(**2,187,328 B**), `.text` (`0x1337c8`), `.ARM.exidx` (`0x6f70`),
`.ARM.extab` (`0x26298`, exact), `.rodata` (`0x41c27`), `.data` (`0x296c`)
and `.rel.plt` (`0x72d0`) are unchanged.  `tools/verify_baseline.json` was
**not** touched.

## 1. Exact census of the post-`pop` region

The last hot instruction is `pop {r4,r5,r6,r7,pc}`.  Everything after it is
the cold region (outlined tails + alignment pad + literal pool):

| | `pop` | region after `pop` | cold blocks | pad | end pool | end-pool words |
|---|---:|---:|---:|---:|---:|---:|
| **original** | `0xd2800` | **966 B** | `0xd2802..0xd2852` = **80 B** (20 words) | 2 B | `0xd2854..0xd2bc8` = **884 B** | **221** |
| HEAD | `0xd28bc` | **778 B** | `0xd28be..0xd28ce` = **16 B** (4 words) | 2 B | `0xd28d0..0xd2bc8` = **760 B** | **190** |
| delta | | **+188 B = 47 words** | **+64 B = 16 words** | 0 | **+124 B = 31 words** | **+31** |

`966/4 = 241.5` and `778/4 = 194.5`, which is the task brief's "241 vs 195"
(after rounding).  The literal pool itself is 221 vs 190 words; the rest of
the difference is the four outlined `iPad` tails in the original vs one
outlined `retinaDisplay` tail in HEAD.

## 2. All 221 original literal-pool words

Every word in `0xd2854..0xd2bc8` is listed below.  `vld1.64` groups are four
consecutive words loaded by one `add rX,pc` + `vld1.64 {dA-dB}`; the four
words are the four field values of the named store window.  Scalar `vldr`
words are the single float constants; `ldr` words are GOT-relative deltas
(the delta is link-layout dependent, the resolved symbol is not).

| addr | word | value | source constant / field |
|---|---:|---:|---|
| `0xd2854` | `0x3ea66666` | 0.325f | field_0x90 0.325f arm (array[0]; taken when retinaDisplay \| iPad \| n9) |
| `0xd2858` | `0x3f266666` | 0.65f | field_0x90 0.65f default arm (array[1]) |
| `0xd285c` | `0x42c80000` | 100f | field_0x9c retina arm (array[0]) |
| `0xd2860` | `0x42480000` | 50f | field_0x9c default arm (array[1]) |
| `0xd2864` | `0x0013df2e` | GOT delta | _ZN7Globals9iPadLargeE |
| `0xd2868` | `0x0013de3c` | GOT delta | _ZN7Globals4iPadE |
| `0xd286c` | `0x0013ddb4` | GOT delta | _ZN7Globals4iPadE |
| `0xd2870` | `0x0013dd96` | GOT delta | _ZN7Globals9iPadLargeE |
| `0xd2874` | `0x438ca000` | 281.25f | field_0x2f8 iPadHD arm |
| `0xd2878` | `0x43c80000` | 400f | field_0x2f8 iPadLarge arm |
| `0xd287c` | `0x0013dc9a` | GOT delta | _ZN7Globals4iPadE |
| `0xd2880` | `0x43480000` | 200f | field_0x2f8 iPad arm |
| `0xd2884` | `0x43160000` | 150f | field_0x2f8 n9 arm |
| `0xd2888` | `0x0013dc04` | GOT delta | _ZN7Globals4iPadE |
| `0xd288c` | `0x0013db84` | GOT delta | _ZN7Globals4iPadE |
| `0xd2890` | `0x00000004` | 4 | n9 arm, `field_0xb0` |
| `0xd2894` | `0x00000016` | 22 | n9 arm, `field_0xb4` |
| `0xd2898` | `0x00000019` | 25 | n9 arm, `field_0xb8` |
| `0xd289c` | `0x0000000c` | 12 | n9 arm, `field_0xbc` |
| `0xd28a0` | `0x0000000b` | 11 | n9 arm, `field_0xc0` |
| `0xd28a4` | `0x0000000b` | 11 | n9 arm, `field_0xc4` |
| `0xd28a8` | `0x0000000a` | 10 | n9 arm, `field_0xc8` |
| `0xd28ac` | `0x0000010e` | 270 | n9 arm, `field_0xcc` |
| `0xd28b0` | `0x0000003c` | 60 | n9 arm, `ringNearMidThreshold` |
| `0xd28b4` | `0x00000078` | 120 | n9 arm, `field_0xf0` |
| `0xd28b8` | `0x0000012c` | 300 | n9 arm, `ringMidFarThreshold` |
| `0xd28bc` | `0x00000258` | 600 | n9 arm, `field_0xf8` |
| `0xd28c0` | `0x00000168` | 360 | n9 arm, `field_0x294_buttonWidth` |
| `0xd28c4` | `0x00000092` | 146 | n9 arm, `field_0x298_buttonYBias` |
| `0xd28c8` | `0x00000008` | 8 | n9 arm, `field_0x29c_buttonRowGap` |
| `0xd28cc` | `0x00000006` | 6 | n9 arm, `field_0x2a0_listRowGap` |
| `0xd28d0` | `0x000000c6` | 198 | n9 arm, `field_0x2a4_metricA` |
| `0xd28d4` | `0x000000b4` | 180 | n9 arm, `field_0x2a8_metricB` |
| `0xd28d8` | `0x000000bd` | 189 | n9 arm, `field_0x2ac_metricC` |
| `0xd28dc` | `0x00000036` | 54 | n9 arm, `field_0x2b0` |
| `0xd28e0` | `0x0000000d` | 13 | n9 arm, `field_0x118` |
| `0xd28e4` | `0x00000025` | 37 | n9 arm, `field_0x11c` |
| `0xd28e8` | `0x00000016` | 22 | n9 arm, `field_0x120` |
| `0xd28ec` | `0x00000002` | 2 | n9 arm, `field_0x124` |
| `0xd28f0` | `0xffffffff` | -1 | n9 arm, `field_0x108` |
| `0xd28f4` | `0x00000002` | 2 | n9 arm, `field_0x10c_rowBaseY` |
| `0xd28f8` | `0x00000001` | 1 | n9 arm, `field_0x110_strip58` |
| `0xd28fc` | `0xffffffff` | -1 | n9 arm, `field_0x114_strip5c` |
| `0xd2900` | `0x00000002` | 2 | n9 arm, `field_0x128` |
| `0xd2904` | `0x00000016` | 22 | n9 arm, `field_0x12c` |
| `0xd2908` | `0x0000003c` | 60 | n9 arm, `field_0x130` |
| `0xd290c` | `0x00000078` | 120 | n9 arm, `field_0x134` |
| `0xd2910` | `0x00000013` | 19 | n9 arm, `field_0x140` |
| `0xd2914` | `0x00000013` | 19 | n9 arm, `field_0x144` |
| `0xd2918` | `0x00000013` | 19 | n9 arm, `field_0x148` |
| `0xd291c` | `0x000000f3` | 243 | n9 arm, `field_0x14c` |
| `0xd2920` | `0x0000000d` | 13 | retina arm, `field_0xc0` |
| `0xd2924` | `0x0000000e` | 14 | retina arm, `field_0xc4` |
| `0xd2928` | `0x0000000a` | 10 | retina arm, `field_0xc8` |
| `0xd292c` | `0x00000168` | 360 | retina arm, `field_0xcc` |
| `0xd2930` | `0x00000006` | 6 | default arm, `field_0xc0` |
| `0xd2934` | `0x00000008` | 8 | default arm, `field_0xc4` |
| `0xd2938` | `0x00000004` | 4 | default arm, `field_0xc8` |
| `0xd293c` | `0x000000b4` | 180 | default arm, `field_0xcc` |
| `0xd2940` | `0x00000006` | 6 | retina arm, `field_0xb0` |
| `0xd2944` | `0x0000001d` | 29 | retina arm, `field_0xb4` |
| `0xd2948` | `0x00000024` | 36 | retina arm, `field_0xb8` |
| `0xd294c` | `0x00000010` | 16 | retina arm, `field_0xbc` |
| `0xd2950` | `0x00000003` | 3 | default arm, `field_0xb0` |
| `0xd2954` | `0x0000000f` | 15 | default arm, `field_0xb4` |
| `0xd2958` | `0x00000012` | 18 | default arm, `field_0xb8` |
| `0xd295c` | `0x00000008` | 8 | default arm, `field_0xbc` |
| `0xd2960` | `0xfffffffe` | -2 | retina arm, `field_0x108` |
| `0xd2964` | `0x00000002` | 2 | retina arm, `field_0x10c_rowBaseY` |
| `0xd2968` | `0x00000001` | 1 | retina arm, `field_0x110_strip58` |
| `0xd296c` | `0xffffffff` | -1 | retina arm, `field_0x114_strip5c` |
| `0xd2970` | `0x00000000` | 0 | default arm, `field_0x108` |
| `0xd2974` | `0x00000001` | 1 | default arm, `field_0x10c_rowBaseY` |
| `0xd2978` | `0x00000002` | 2 | default arm, `field_0x110_strip58` |
| `0xd297c` | `0x00000001` | 1 | default arm, `field_0x114_strip5c` |
| `0xd2980` | `0x00000018` | 24 | retina arm, `field_0x2a0_listRowGap` |
| `0xd2984` | `0x000000dc` | 220 | retina arm, `field_0x2a4_metricA` |
| `0xd2988` | `0x000000c8` | 200 | retina arm, `field_0x2a8_metricB` |
| `0xd298c` | `0x000000d2` | 210 | retina arm, `field_0x2ac_metricC` |
| `0xd2990` | `0x00000050` | 80 | retina arm, `ringNearMidThreshold` |
| `0xd2994` | `0x000000a0` | 160 | retina arm, `field_0xf0` |
| `0xd2998` | `0x00000190` | 400 | retina arm, `ringMidFarThreshold` |
| `0xd299c` | `0x00000320` | 800 | retina arm, `field_0xf8` |
| `0xd29a0` | `0x00000028` | 40 | default arm, `ringNearMidThreshold` |
| `0xd29a4` | `0x00000050` | 80 | default arm, `field_0xf0` |
| `0xd29a8` | `0x000000c8` | 200 | default arm, `ringMidFarThreshold` |
| `0xd29ac` | `0x00000190` | 400 | default arm, `field_0xf8` |
| `0xd29b0` | `0x0000000c` | 12 | default arm, `field_0x2a0_listRowGap` |
| `0xd29b4` | `0x0000006e` | 110 | default arm, `field_0x2a4_metricA` |
| `0xd29b8` | `0x00000064` | 100 | default arm, `field_0x2a8_metricB` |
| `0xd29bc` | `0x00000069` | 105 | default arm, `field_0x2ac_metricC` |
| `0xd29c0` | `0x00000022` | 34 | retina arm, `field_0x13c` |
| `0xd29c4` | `0x00000016` | 22 | retina arm, `field_0x140` |
| `0xd29c8` | `0x0000001a` | 26 | retina arm, `field_0x144` |
| `0xd29cc` | `0x00000004` | 4 | retina arm, `field_0x148` |
| `0xd29d0` | `0x00000011` | 17 | default arm, `field_0x13c` |
| `0xd29d4` | `0x0000000b` | 11 | default arm, `field_0x140` |
| `0xd29d8` | `0x0000000d` | 13 | default arm, `field_0x144` |
| `0xd29dc` | `0x00000002` | 2 | default arm, `field_0x148` |
| `0xd29e0` | `0x00000106` | 262 | iPadLarge arm, `field_0x16c` |
| `0xd29e4` | `0x000000f8` | 248 | iPadLarge arm, `field_0x170` |
| `0xd29e8` | `0x00000054` | 84 | iPadLarge arm, `field_0x174` |
| `0xd29ec` | `0x00000078` | 120 | iPadLarge arm, `field_0x178` |
| `0xd29f0` | `0x0000001e` | 30 | iPadLarge arm, `field_0x180` |
| `0xd29f4` | `0x0000015a` | 346 | iPadLarge arm, `field_0x184` |
| `0xd29f8` | `0x0000001e` | 30 | iPadLarge arm, `field_0x188` |
| `0xd29fc` | `0x000001be` | 446 | iPadLarge arm, `field_0x18c` |
| `0xd2a00` | `0x00000013` | 19 | n9 arm, `field_0x1b0` |
| `0xd2a04` | `0x0000002d` | 45 | n9 arm, `field_0x1b4` |
| `0xd2a08` | `0x00000028` | 40 | n9 arm, `field_0x1b8` |
| `0xd2a0c` | `0x00000002` | 2 | n9 arm, `field_0x1bc` |
| `0xd2a10` | `0x00000007` | 7 | n9 arm, `field_0x1d0` |
| `0xd2a14` | `0x00000012` | 18 | n9 arm, `field_0x1d4` |
| `0xd2a18` | `0x00000006` | 6 | n9 arm, `field_0x1d8` |
| `0xd2a1c` | `0x0000000e` | 14 | n9 arm, `field_0x1dc` |
| `0xd2a20` | `0x00000050` | 80 | n9 arm, `field_0x1c0` |
| `0xd2a24` | `0x00000022` | 34 | n9 arm, `field_0x1c4` |
| `0xd2a28` | `0x0000002b` | 43 | n9 arm, `field_0x1c8` |
| `0xd2a2c` | `0x00000036` | 54 | n9 arm, `field_0x1cc` |
| `0xd2a30` | `0x0000000a` | 10 | retina arm, `field_0x1d0` |
| `0xd2a34` | `0x00000018` | 24 | retina arm, `field_0x1d4` |
| `0xd2a38` | `0x00000008` | 8 | retina arm, `field_0x1d8` |
| `0xd2a3c` | `0x00000014` | 20 | retina arm, `field_0x1dc` |
| `0xd2a40` | `0x00000005` | 5 | default arm, `field_0x1d0` |
| `0xd2a44` | `0x0000000c` | 12 | default arm, `field_0x1d4` |
| `0xd2a48` | `0x00000004` | 4 | default arm, `field_0x1d8` |
| `0xd2a4c` | `0x0000000a` | 10 | default arm, `field_0x1dc` |
| `0xd2a50` | `0x00000084` | 132 | retina arm, `field_0x1c0` |
| `0xd2a54` | `0x00000026` | 38 | retina arm, `field_0x1c4` |
| `0xd2a58` | `0x0000003a` | 58 | retina arm, `field_0x1c8` |
| `0xd2a5c` | `0x00000048` | 72 | retina arm, `field_0x1cc` |
| `0xd2a60` | `0x00000042` | 66 | default arm, `field_0x1c0` |
| `0xd2a64` | `0x00000013` | 19 | default arm, `field_0x1c4` |
| `0xd2a68` | `0x0000001d` | 29 | default arm, `field_0x1c8` |
| `0xd2a6c` | `0x00000024` | 36 | default arm, `field_0x1cc` |
| `0xd2a70` | `0x0000001a` | 26 | retina arm, `field_0x1b0` |
| `0xd2a74` | `0x0000003c` | 60 | retina arm, `field_0x1b4` |
| `0xd2a78` | `0x00000042` | 66 | retina arm, `field_0x1b8` |
| `0xd2a7c` | `0x00000002` | 2 | retina arm, `field_0x1bc` |
| `0xd2a80` | `0x0000000d` | 13 | default arm, `field_0x1b0` |
| `0xd2a84` | `0x0000001e` | 30 | default arm, `field_0x1b4` |
| `0xd2a88` | `0x00000021` | 33 | default arm, `field_0x1b8` |
| `0xd2a8c` | `0x00000001` | 1 | default arm, `field_0x1bc` |
| `0xd2a90` | `0x0000000c` | 12 | iPadHD arm, `field_0x204` |
| `0xd2a94` | `0x00000005` | 5 | iPadHD arm, `field_0x208` |
| `0xd2a98` | `0x00000007` | 7 | iPadHD arm, `field_0x20c` |
| `0xd2a9c` | `0x00000005` | 5 | iPadHD arm, `field_0x210` |
| `0xd2aa0` | `0x00000001` | 1 | iPadHD arm, `field_0x214` |
| `0xd2aa4` | `0x00000007` | 7 | iPadHD arm, `field_0x218` |
| `0xd2aa8` | `0x00000008` | 8 | iPadHD arm, `field_0x21c` |
| `0xd2aac` | `0x00000007` | 7 | iPadHD arm, `field_0x220` |
| `0xd2ab0` | `0x0000001c` | 28 | iPadHD arm, `field_0x224` |
| `0xd2ab4` | `0x00000031` | 49 | iPadHD arm, `field_0x228` |
| `0xd2ab8` | `0x00000002` | 2 | iPadHD arm, `field_0x22c` |
| `0xd2abc` | `0x00000001` | 1 | iPadHD arm, `field_0x230` |
| `0xd2ac0` | `0x00000015` | 21 | iPadHD arm, `field_0x23c` |
| `0xd2ac4` | `0x00000007` | 7 | iPadHD arm, `field_0x240` |
| `0xd2ac8` | `0x00000004` | 4 | iPadHD arm, `field_0x244` |
| `0xd2acc` | `0x000000ef` | 239 | iPadHD arm, `field_0x248` |
| `0xd2ad0` | `0xfffffffe` | -2 | iPadHD arm, `field_0x24c` |
| `0xd2ad4` | `0x00000001` | 1 | iPadHD arm, `field_0x250` |
| `0xd2ad8` | `0x00000005` | 5 | iPadHD arm, `field_0x254` |
| `0xd2adc` | `0x00000002` | 2 | iPadHD arm, `field_0x258` |
| `0xd2ae0` | `0x00000016` | 22 | iPadHD arm, `field_0x25c` |
| `0xd2ae4` | `0x00000001` | 1 | iPadHD arm, `field_0x260` |
| `0xd2ae8` | `0x000001a5` | 421 | iPadHD arm, `field_0x264` |
| `0xd2aec` | `0x0000000b` | 11 | iPadHD arm, `field_0x268` |
| `0xd2af0` | `0x0000005c` | 92 | iPadHD arm, `field_0x26c` |
| `0xd2af4` | `0x00000062` | 98 | iPadHD arm, `field_0x270` |
| `0xd2af8` | `0x00000088` | 136 | iPadHD arm, `field_0x274` |
| `0xd2afc` | `0x000001a5` | 421 | iPadHD arm, `field_0x278` |
| `0xd2b00` | `0x00000010` | 16 | n9 arm, `field_0x204` |
| `0xd2b04` | `0x00000007` | 7 | n9 arm, `field_0x208` |
| `0xd2b08` | `0x00000007` | 7 | n9 arm, `field_0x20c` |
| `0xd2b0c` | `0x00000006` | 6 | n9 arm, `field_0x210` |
| `0xd2b10` | `0xfffffffe` | -2 | n9 arm, `field_0x214` |
| `0xd2b14` | `0x00000007` | 7 | n9 arm, `field_0x218` |
| `0xd2b18` | `0x0000000a` | 10 | n9 arm, `field_0x21c` |
| `0xd2b1c` | `0x00000007` | 7 | n9 arm, `field_0x220` |
| `0xd2b20` | `0x0000001e` | 30 | n9 arm, `field_0x224` |
| `0xd2b24` | `0x00000034` | 52 | n9 arm, `field_0x228` |
| `0xd2b28` | `0x00000004` | 4 | n9 arm, `field_0x22c` |
| `0xd2b2c` | `0x00000002` | 2 | n9 arm, `field_0x230` |
| `0xd2b30` | `0x0000001b` | 27 | n9 arm, `field_0x23c` |
| `0xd2b34` | `0x00000009` | 9 | n9 arm, `field_0x240` |
| `0xd2b38` | `0x00000005` | 5 | n9 arm, `field_0x244` |
| `0xd2b3c` | `0x00000132` | 306 | n9 arm, `field_0x248` |
| `0xd2b40` | `0xffffffff` | -1 | n9 arm, `field_0x24c` |
| `0xd2b44` | `0x00000002` | 2 | n9 arm, `field_0x250` |
| `0xd2b48` | `0x00000007` | 7 | n9 arm, `field_0x254` |
| `0xd2b4c` | `0x00000004` | 4 | n9 arm, `field_0x258` |
| `0xd2b50` | `0x0000001c` | 28 | n9 arm, `field_0x25c` |
| `0xd2b54` | `0x00000000` | 0 | n9 arm, `field_0x260` |
| `0xd2b58` | `0x000001c2` | 450 | n9 arm, `field_0x264` |
| `0xd2b5c` | `0x0000000c` | 12 | n9 arm, `field_0x268` |
| `0xd2b60` | `0x00000063` | 99 | n9 arm, `field_0x26c` |
| `0xd2b64` | `0x00000069` | 105 | n9 arm, `field_0x270` |
| `0xd2b68` | `0x00000091` | 145 | n9 arm, `field_0x274` |
| `0xd2b6c` | `0x000001c2` | 450 | n9 arm, `field_0x278` |
| `0xd2b70` | `0x00000258` | 600 | retina arm, `field_0x264` |
| `0xd2b74` | `0x00000010` | 16 | retina arm, `field_0x268` |
| `0xd2b78` | `0x00000084` | 132 | retina arm, `field_0x26c` |
| `0xd2b7c` | `0x0000008c` | 140 | retina arm, `field_0x270` |
| `0xd2b80` | `0x0000012c` | 300 | default arm, `field_0x264` |
| `0xd2b84` | `0x00000008` | 8 | default arm, `field_0x268` |
| `0xd2b88` | `0x00000042` | 66 | default arm, `field_0x26c` |
| `0xd2b8c` | `0x00000046` | 70 | default arm, `field_0x270` |
| `0xd2b90` | `0x0000001e` | 30 | retina arm, `field_0x23c` |
| `0xd2b94` | `0x0000000a` | 10 | retina arm, `field_0x240` |
| `0xd2b98` | `0x00000006` | 6 | retina arm, `field_0x244` |
| `0xd2b9c` | `0x00000154` | 340 | retina arm, `field_0x248` |
| `0xd2ba0` | `0x0000000f` | 15 | default arm, `field_0x23c` |
| `0xd2ba4` | `0x00000005` | 5 | default arm, `field_0x240` |
| `0xd2ba8` | `0x00000003` | 3 | default arm, `field_0x244` |
| `0xd2bac` | `0x000000aa` | 170 | default arm, `field_0x248` |
| `0xd2bb0` | `0x43480000` | 200f | field_0x2f8 retina arm (array[0]) |
| `0xd2bb4` | `0x42c80000` | 100f | field_0x2f8 default arm (array[1]) |
| `0xd2bb8` | `0x43160000` | 150f | helpWindowY n9 arm (array[0]) |
| `0xd2bbc` | `0x43100000` | 144f | helpWindowY iPad arm (array[1]) |
| `0xd2bc0` | `0x43160000` | 150f | helpWindowY retina arm (array[0]) |
| `0xd2bc4` | `0x42980000` | 76f | helpWindowY default arm (array[1]) |

### Scalar-word notes

* `0xd2854/0xd2858` = `{0.325f, 0.65f}` for `field_0x90`.  The original
  selects the pair with `addw r1,pc,#3848` + `it eq; addeq r1,#4` +
  `vldr s2,[r1]`; the pair is in the **end pool**.  Our build has the same
  two values but in a `.text` island at `0xd2548/0xd254c`.
* `0xd285c/0xd2860` = `{100.0f, 50.0f}` for `field_0x9c`, selected the same
  way.  Both binaries have these two words; the task brief's "extra 100.0f
  + 50.0f" is stale (see §4).
* `0xd2874` = `281.25f` for `field_0x2f8` (iPadHD), loaded with a plain
  `ldr r2,[pc,#536]` and stored as the raw word via `strd`.
* `0xd2878/0xd2880/0xd2884` = `400.0f/200.0f/150.0f` for `field_0x2f8`
  (iPadLarge/iPad/n9).
* `0xd2bb0/0xd2bb4` = `{200.0f,100.0f}` for `field_0x2f8`
  (retina/default); selected by `addw r2,pc,#1296` + `it eq; addeq r2,#4`.
* `0xd2bb8..0xd2bc4` = `{150.0f,144.0f,150.0f,76.0f}` for `helpWindowY`
  (n9/iPad/retina/default); two `addw` + `addeq #4` array selections.

## 3. The 20 cold-block words (original `0xd2802..0xd2852`)

These are instructions, not constants.  They are the four outlined final
`Globals::iPad ? A : B` arms of the `0x130/0x134/v138`, `t_field_0x2c8`,
`field_0x2d0` and `field_0x60` chains (see
`layout-layout-c1-coldblocks-wave119-w6932.md`):

| addr | encoding | instruction | source construct |
|---|---:|---|---|
| `0xd2802` | `0x9e01` | `ldr r6,[sp,#4]` | `0x130/0x134/v138` chain, final `iPad` arm (block A) |
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
| `0xd2828` | `0x7831` | `ldrb r1,[r6,#0]` | `t_field_0x2c8`, final `iPad` arm (block B) |
| `0xd282a` | `0x262d` | `movs r6,#45` | block B |
| `0xd282c` | `0x2900` | `cmp r1,#0` | block B |
| `0xd282e` | `0xbf08` | `it eq` | block B |
| `0xd2830` | `0x261e` | `moveq r6,#30` | block B: `45:30` |
| `0xd2832` | `0xf7febfef` | `b.w 0xd1814` | block B join |
| `0xd2836` | `0x7819` | `ldrb r1,[r3,#0]` | `field_0x2d0`, final `iPad` arm (block C) |
| `0xd2838` | `0x222d` | `movs r2,#45` | block C |
| `0xd283a` | `0x2900` | `cmp r1,#0` | block C |
| `0xd283c` | `0xbf08` | `it eq` | block C |
| `0xd283e` | `0x221e` | `moveq r2,#30` | block C: `45:30` |
| `0xd2840` | `0xf7ffb80e` | `b.w 0xd1860` | block C join |
| `0xd2844` | `0x7831` | `ldrb r1,[r6,#0]` | `field_0x60`, final `iPad` arm (block D) |
| `0xd2846` | `0x2640` | `movs r6,#64` | block D |
| `0xd2848` | `0x2900` | `cmp r1,#0` | block D |
| `0xd284a` | `0xbf08` | `it eq` | block D |
| `0xd284c` | `0x262c` | `moveq r6,#44` | block D: `64:44` |
| `0xd284e` | `0xf7febf42` | `b.w 0xd16d6` | block D join |
| `0xd2852` | `0xbf00` | `nop` | 2-byte alignment pad |

The 80-byte cold block is 20 4-byte words; the table lists the 35
instructions that occupy them (some are 16-bit).  The `nop` is the 2-byte
alignment pad, not a word.

## 4. What is actually missing / duplicated

### 4a. Group-level diff: 50 original vs 42 HEAD

The end-pool `vld1.64` count is **50 original / 42 HEAD** (200 vs 168 words).
Matching each group to a `(config, 4-field window)` via the original store
trace gives **15 original-only groups** and **7 HEAD-only groups**, net
**8 groups = 32 words**.  The remaining end-pool delta is **−1 GOT word**
(HEAD has 8 end-pool GOT deltas, original has 7), so the end-pool deficit is
exactly **31 words**.

**15 original-only groups** (the source constants the original vectorises
but HEAD does not):

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

**7 HEAD-only groups** (the source constants HEAD vectorises but the
original does not — these are the "duplicated" groups):

| config | window | 4 values | source fields |
|---|---|---|---|
| n9 | `0xdc` | 15.0f,1.5f,12.0f,1.5f | `field_0xdc`,`animSpeedScale`,`driftSpeedDivisor`,`ringRadiusScale` |
| n9 | `0x16c` | 115,106,45,27 | `field_0x16c`,`0x170`,`0x174`,`0x178` |
| retina | `0x294` | 400,168,24,24 | `field_0x294`,`0x298`,`0x29c`,`0x2a0` |
| retina | `0x16c` | 138,128,60,44 | `field_0x16c`,`0x170`,`0x174`,`0x178` |
| iPad | `0x16c` | 131,124,42,60 | `field_0x16c`,`0x170`,`0x174`,`0x178` |
| default | `0x294` | 200,84,12,12 | `field_0x294`,`0x298`,`0x29c`,`0x2a0` |
| default | `0x16c` | 69,64,30,22 | `field_0x16c`,`0x170`,`0x174`,`0x178` |

The seven 16-byte window-shift pairs (`original 0x204/0x214/0x224/0x23c/0x24c/0x25c/0x26c`
vs HEAD `0x208/0x218/0x228/0x240/0x250/0x260/0x270`) are value-identical
and cancel in the net count; they are not part of the 8.

### 4b. Scalar constants: values are present, placement/type differs

The end-pool scalar float count is **14 original / 14 HEAD**.  The task
brief's "100.0f + 50.0f" is therefore stale: both binaries have exactly one
`100.0f` and one `50.0f` in the end pool.  The real scalar differences are:

| constant | original | HEAD | source field | note |
|---|---|---|---|---|
| `0.325f` (`0x3ea66666`) | end pool `0xd2854` | `.text` island `0xd2548` | `field_0x90` | same value, different pool |
| `0.65f` (`0x3f266666`) | end pool `0xd2858` | `.text` island `0xd254c` | `field_0x90` | same value, different pool |
| `1.5f` (`0x3fc00000`) | `mov.w r3,#0x3fc00000` immediate | end pool ×2 (`0xd2944`,`0xd294c`) | `animSpeedScale`,`ringRadiusScale` | HEAD SLP group `0xdc` |
| `15.0f` (`0x41700000`) | `.text` island `0xd1d80` | end pool `0xd2940` | `field_0xdc` | HEAD SLP group `0xdc` |
| `12.0f` (`0x41400000`) | `.text` island `0xd1d84` | end pool `0xd2948` | `driftSpeedDivisor` | HEAD SLP group `0xdc` |
| `41.0f` (`0x42240000`) | absent (int `41` immediate) | end pool `0xd2ba4` | `field_0xa8` | HEAD uses float arms |
| `82.0f` (`0x42a40000`) | absent (int `82` immediate) | end pool `0xd2ba0` | `field_0xa8` | HEAD uses float arms |

The `41.0f`/`82.0f` pair is a genuine source-type mismatch, not just
placement.  The original's `field_0xa8` expression is an **int** chain:
`movs r2,#82` / `moveq r2,#41` / `vmov s0,r2` / `vcvt.f32.s32` /
`vcvt.s32.f32` / `vstr s0,[r0,#0xa8]`.  HEAD's source uses float arms
(`82.0f`, `41.0f`, ...), so clang materialises `41.0f`/`82.0f` in the
literal pool and emits an extra `vcvt`.  A faithful source shape is:

```cpp
int v = Globals::iPadHD ? (int)(61 * 1.40625f)
      : (Globals::iPadLarge ? 122 : (Globals::n9 ? 61 :
         (Globals::iPad ? 61 : (Globals::retinaDisplay ? 82 : 41))));
this->field_0xac = v;
this->field_0xa8 = (int)(float)v;
```

This shape was measured (§5, variant A): it removes the two pool words but
shrinks the body to 7024 B and lowers the fuzzy ratio, so it is not landable
at the mandatory 7072 B.

### 4c. GOT: HEAD has one extra end-pool `iPadLarge` delta

Resolving the end-pool `ldr rX,[pc,#N]` / `add rX,pc` deltas against
`.got` gives:

| | original | HEAD |
|---|---:|---:|
| `_ZN7Globals9iPadLargeE` | 2 | **3** |
| `_ZN7Globals4iPadE` | 5 | 5 |
| total end-pool GOT deltas | **7** | **8** |

The extra HEAD site is the `field_0x2f4` / `field_0x2f8` pair: HEAD tests
`Globals::iPadLarge` twice (two separate `ldr` pool words at `0xd28e4` and
`0xd28e8`), while the original tests it once (`0xd2870`) and selects both
fields from the same flag read.  This is the "−1 GOT" in the 31-word
decomposition.  Sharing the test with a local `bool` or an explicit
`if/else` chain was measured (§5, variants E/F) and changes the body size
and score, so it is not landable as-is.

## 5. Measured variants (all real `link_so` builds, exact scorer)

Scorer: `/dev/shm/w6958/score.py` — the `tools/verify/verify.py`
`Resolver` + `reconcile` + `difflib(autojunk=False)` pipeline on
`_ZN6LayoutC1Ev`.  It reproduces the full-link number bit-for-bit.

| variant | source change | size | ratio % |
|---|---|---:|---:|
| **base** | — | **7072** | **48.963317384370** |
| A | single int `v`, `field_0xac=v`, `field_0xa8=(int)(float)v` | 7024 | 48.800959232614 |
| B | `field_0xac` unchanged; int-arm `v`; `field_0xa8=(int)(float)v` | 7032 | 48.831168831169 |
| C | int arms except HD `61.0f*1.40625f` | 7032 | 48.621654015182 |
| D | `field_0x90` as `(retinaDisplay \|\| iPad \|\| n9) ? 0.325f : 0.65f` | 7072 | 48.963317384370 (codegen no-op) |
| E | `field_0x2f4`/`0x2f8` as explicit `if/else if` chain | 7048 | 48.532055122828 |
| F | local `bool isiPadLarge = Globals::iPadLarge;` shared by `0x2f4`/`0x2f8` | 7108 | 24.608057154197 |

The base is still the best measured 7072-B variant.  Every variant that
actually removes a pool word also changes the body size, and any size change
from 7072 B collapses whole-`.so` `byte_exact`, so none is landable.

## 6. Proven vs hypothesis

**Proven (measured):**

* The original post-`pop` region is 966 B: 80 B of four cold `iPad` tails,
  2 B pad, 884 B (221 words) of literal pool.  HEAD is 778 B: 16 B of one
  cold `retinaDisplay` tail, 2 B pad, 760 B (190 words).
* The 221 original literal words are fully enumerated above: 50 `vld1.64`
  groups (200 words), 14 scalar floats, 7 GOT deltas.
* HEAD has 42 groups (168 words), 14 scalar floats, 8 GOT deltas.
* The net group deficit is **8 groups = 32 words**, made of **15
  original-only groups minus 7 HEAD-only groups** (both lists above).
* The end-pool deficit is **32 − 1 = 31 words**; the extra word is one
  `Globals::iPadLarge` GOT delta on the `field_0x2f4`/`field_0x2f8` pair.
* The scalar float count is equal (14 vs 14); the task brief's
  "100.0f + 50.0f" is stale.
* `41.0f`/`82.0f` in HEAD's end pool are a real source-type mismatch:
  the original uses int arms for `field_0xa8`'s `v` and folds the HD arm.
* The base is a local optimum for the official fuzzy metric among the
  measured variants; every pool-word-removing variant changes the body
  size and scores lower.

**Hypothesis (not proven):**

* The original source is config-major for the `0x16c..0x178`,
  `0x294..0x2ac`, `0xb0..0xbc`, `0xec..0xf8`, `0x13c..0x148` and
  `0x1d0..0x1dc` runs, which is why it vectorises the retina/default arms
  and not the iPad/n9 arms that HEAD vectorises.  Reproducing that shape
  from HEAD requires the original statement order *and* the original
  expression types; every reorder measured in the cited notes lands below
  7072 B.
* The original's `field_0x2f4`/`field_0x2f8` pair shares a single
  `Globals::iPadLarge` read; the exact C++ that makes clang 7.0.2 reuse the
  pool delta without changing the body size is not yet found.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6958
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6     # RATCHET PASS, byte 4505

OBJ=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OBJ -d --start-address=0xd1028 --stop-address=0xd2bc8 \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so > /dev/shm/w6958/orig.dis
$OBJ -d --start-address=0xd1028 --stop-address=0xd2bc8 \
  cmake-build-match/libgof2hdaa.so > /dev/shm/w6958/ours.dis

python3 /dev/shm/w6958/fullcensus.py /dev/shm/w6958/orig.dis \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  0xd1028 0xd2bc8 0xd2854
python3 /dev/shm/w6958/fullcensus.py /dev/shm/w6958/ours.dis \
  cmake-build-match/libgof2hdaa.so 0xd1028 0xd2bc8 0xd28d0
python3 /dev/shm/w6958/groupmap2.py /dev/shm/w6958/orig.dis \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  0xd1028 0xd2bc8 0xd2854
python3 /dev/shm/w6958/groupmap2.py /dev/shm/w6958/ours.dis \
  cmake-build-match/libgof2hdaa.so 0xd1028 0xd2bc8 0xd28d0
python3 /dev/shm/w6958/score.py        # base 7072 B / 48.963317384370 %
```

Scratch harnesses (not committed, `/dev/shm/w6958/`): `census.py`,
`fullcensus.py`, `groupmap2.py`, `match_groups.py`, `mk_table2.py`,
`score.py`, `emu.py` (path-adapted `_work/reconstructions/layout_ctor/emu.py`),
`try_variants.py`, `Layout.base.cpp`, `orig.dis`, `ours.dis`,
`trace_*.txt`, `table2.md`.
