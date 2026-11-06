# Layout::Layout C1/C2 — wave 121 / worker w7102
## Full-body reconstruction proposal (note only; no source win landed)

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (identical bodies, aliased at
`0xd1028`), original size **7072 B**, source `src/game/ui/Layout.cpp`.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(sha256 `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`).

This pass re-decompiles the whole constructor from the original disassembly:
the exact branch tree, every `this` store, every literal, and the complete
literal-pool census.  It also records a corrected config-major body that is
semantically exact for all 32 flag combinations.  The body is **not** a score
win over the current field-major baseline, so no `src/` change is landed.

## Result — note only, no source change

| metric | before (base) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| unrounded `difflib` ratio | 48.963317384370 % | **48.963317384370 %** |
| `report.json` pct (1 dp) | 49.0 | 49.0 |
| `byte_exact` / `linked_exact` | 4505 / 4505 | **4505 / 4505** |
| `size_exact` / `data_exact` | 4524 / 843 | 4524 / 843 |
| imports / lint / drift | 207 / CLEAN / 0 | 207 / CLEAN / 0 |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
No source change landed, so the file size, section headers and
`tools/verify_baseline.json` are unchanged.  `git status --short` is clean
apart from the pre-existing untracked `third_party/*` directories.

## 1. Original function shape

The function is a pure field initializer: there is **no `bl`/`blx`** in the
whole 7072-byte body.  It is compiled config-major:

| region | address range | notes |
|---|---|---|
| prologue | `0xd1028..0xd1041` | `push {r4,r5,r6,r7,lr}` / `add r7,sp,#12` / `stmdb sp!,{r6,r7,r8,r9,sl,fp}` |
| `iPadHD` arm | `0xd1042..0xd1268` | ends `b.w 0xd21e2` |
| `n9` test | `0xd126c..0xd127a` | `beq.w 0xd1570` |
| `n9` arm | `0xd127e..0xd12fc` | ends `b.n 0xd1860` |
| `else` arm (retina/iPadLarge/iPad/default) | `0xd1570..0xd1830` | ends `b.n 0xd1860` |
| common tail 1 | `0xd1860..0xd21e0` | per-field retina/iPad/default selects |
| common tail 2 | `0xd21e2..0xd240c` | second half of the common selects |
| default part 2 | `0xd240e..` | continues common tail 2 |
| epilogue | `0xd2800` | `pop {r4,r5,r6,r7,pc}` |
| outlined cold tails | `0xd2802..0xd2852` | 80 B, four `iPad` arms (see §1.1) |
| literal pool | `0xd2854..0xd2bc8` | 221 words (see §3) |

The top-level tree is therefore:

```cpp
if (Globals::iPadHD) {
    /* 137 stores, 0xd1042..0xd1268 */
} else if (Globals::n9) {
    /* 34 stores, 0xd127e..0xd12fc */
} else {
    /* 34 stores, 0xd1570..0xd1830 */
}
/* common tail 1: 101..103 stores, 0xd1860.. */
/* common tail 2: 25..26 stores, 0xd21e2.. */
```

Store counts per configuration (unique `this` offsets, from the emulator):

| config | first block | common1 | part2 | common2 | cold | total |
|---|---:|---:|---:|---:|---:|---:|
| `iPadHD=1` | 137 | 0 | 41 | 26 | 0 | **204** |
| `n9=1` | 34 | 103 | 42 | 25 | 0 | **204** |
| default/retina/iPad/iPadLarge | 34 | 101 | 41 | 26 | 2 | **204** |

All three configurations write exactly the same 204 field offsets; the
`Blk16` member at `0x238` is four separate 4-byte stores
(`0x238/0x23c/0x240/0x244`).

### 1.1 Outlined cold tails (`0xd2802..0xd2852`)

These are the final `Globals::iPad ? A : B` arms that clang outlined after the
`pop`:

| addr | bytes | construct |
|---|---:|---|
| `0xd2802..0xd2827` | 38 | `field_0x130` / `field_0x134` / `v138` final `iPad` arm (`56:40`, `108:80`, `47:35`) |
| `0xd2828..0xd2835` | 14 | `t_field_0x2c8` final `iPad` arm (`45:30`) |
| `0xd2836..0xd2843` | 14 | `field_0x2d0` final `iPad` arm (`45:30`) |
| `0xd2844..0xd2851` | 14 | `field_0x60` final `iPad` arm (`64:44`) |
| `0xd2852` | 2 | `nop` alignment pad |

The current field-major baseline has only one 16-byte outlined tail
(`field_0x1ac` retina/default arm at `0xd28be`); reproducing the four original
tails is a direct consequence of the config-major source shape.

## 2. Complete store inventory

The store order below is the **actual** order of the first write to each
`this` offset in the original, recovered by emulating the original ctor for
each configuration.  The values are the bytes the original stores; they are
also the values used by the proposed body in §4.


### HD store order

`14 18 1c 20 4 8 c 10 24 28 2c 30 34 38 3c 40 44 48 4c 50 54 58 5c 60 64 68 6c 70 2c8 2cc 2d0 2d4 7c 80 84 88 74 78 2c0 2c4 2d8 ac 8c 90 94 98 9c a0 a4 a8 c0 c4 c8 cc b0 b4 b8 bc ec f0 f4 f8 294 298 29c 2a0 2a4 2a8 2ac 2b0 108 10c 110 114 118 11c 120 124 d0 d4 d8 dc e0 e4 e8 fc 100 104 2b4 2b8 128 12c 130 134 140 144 148 14c 150 154 158 15c 160 164 168 16c 170 174 178 17c 180 184 188 18c 190 194 198 19c 1a0 1a4 1a8 1ac 1b0 1b4 1b8 1bc 1c0 1c4 1c8 1cc 1d0 1d4 1d8 1dc 138 13c 1e0 1e8 1e4 1fc 1ec 234 238 27c 280 3e0 3e4 3e8 3f4 3f8 204 208 20c 210 214 218 21c 220 224 228 22c 230 23c 240 244 248 24c 250 254 258 25c 260 264 268 26c 270 274 278 3ec 3f0 2bc 3fc 200 1f0 1f4 1f8 284 285 286 288 28c 290 2f4 2f8 2fc 300 304 308 30c 310 314 318 31c 320`


### n9 store order

`74 78 2c0 2c4 2c8 2cc 14 18 1c 20 4 8 c 10 24 28 2c 30 34 38 3c 40 44 48 4c 50 54 58 5c 60 64 68 6c 70 2d0 2d4 2d8 7c 80 84 88 8c 90 94 98 9c ac a8 a0 a4 d0 d4 d8 dc e0 e4 e8 fc 100 104 2b4 2b8 b0 b4 b8 bc c0 c4 c8 cc ec f0 f4 f8 294 298 29c 2a0 2a4 2a8 2ac 2b0 118 11c 120 124 108 10c 110 114 128 12c 130 134 140 144 148 14c 138 13c 150 154 158 15c 160 164 168 16c 170 174 178 17c 180 184 188 18c 190 194 198 19c 1a0 1a4 1a8 1ac 1e0 1c0 1c4 1c8 1cc 1b0 1b4 1b8 1bc 1d0 1d4 1d8 1dc 1e8 1e4 1fc 1ec 234 238 27c 204 208 20c 210 214 218 21c 220 224 228 22c 230 23c 240 244 248 24c 250 254 258 25c 260 264 268 26c 270 274 278 280 3e4 3e0 3e8 3f4 3f8 3ec 3f0 2bc 3fc 200 1f0 1f4 1f8 284 285 286 288 28c 290 2f4 2f8 2fc 300 304 308 30c 310 314 318 31c 320`


### default store order

`4 8 c 10 14 18 1c 20 24 28 30 2c 38 34 3c 44 40 4c 48 54 50 58 5c 60 64 68 6c 2c0 70 74 78 2c4 2c8 2cc 2d0 2d4 2d8 7c 80 84 88 8c 90 94 98 9c ac a8 d4 d0 a0 a4 d8 fc 100 104 298 dc e0 e4 e8 294 29c b0 b4 b8 bc c0 c4 c8 cc ec f0 f4 f8 2a0 2a4 2a8 2ac 108 10c 110 114 2b0 2b4 2b8 118 11c 120 124 128 12c 130 134 14c 138 13c 140 144 148 150 154 158 15c 160 164 168 16c 170 174 178 17c 180 184 188 18c 190 194 198 19c 1a0 1a4 1a8 1ac 1e0 1c0 1c4 1c8 1cc 1b0 1b4 1b8 1bc 1d0 1d4 1d8 1dc 1e8 1e4 1fc 1ec 224 228 22c 230 234 238 24c 250 20c 218 220 204 208 210 214 21c 23c 240 244 248 278 264 268 26c 270 254 258 25c 260 274 27c 280 3e4 3e0 3e8 3f4 3f8 3ec 3f0 2bc 3fc 200 1f0 1f4 1f8 284 285 286 288 28c 290 2f4 2f8 2fc 300 304 308 30c 310 314 318 31c 320`


### Field values by configuration

| off | member | HD | n9 | default |
|---:|---|---:|---:|---:|
| `0x000` | `choiceWindowOpen` | - | - | - |
| `0x001` | `_pad_0x1` | - | - | - |
| `0x004` | `field_0x4` | 21 | 22 | 15 |
| `0x008` | `windowTopInset` | 42 | 45 | 30 |
| `0x00c` | `field_0xc_leftMargin` | 40 | 51 | 29 |
| `0x010` | `field_0x10_rightMargin` | 52 | 58 | 37 |
| `0x014` | `footerTextInset` | 30 | 37 | 22 |
| `0x018` | `headerTitleY` | 12 | 11 | 9 |
| `0x01c` | `field_0x1c` | 28 | 30 | 20 |
| `0x020` | `field_0x20_top` | 7 | 7 | 5 |
| `0x024` | `field_0x24` | 7 | 7 | 5 |
| `0x028` | `buttonInsetX` | 7 | 9 | 5 |
| `0x02c` | `field_0x2c_rowHeight` | 4 | 6 | 3 |
| `0x030` | `field_0x30_rowHeight` | 42 | 51 | 30 |
| `0x034` | `field_0x34` | 8 | 4 | 6 |
| `0x038` | `field_0x38` | 9 | 12 | 7 |
| `0x03c` | `field_0x3c` | 82 | 106 | 59 |
| `0x040` | `field_0x40` | 140 | 180 | 100 |
| `0x044` | `field_0x44` | 7 | 9 | 5 |
| `0x048` | `field_0x48` | 15 | 16 | 11 |
| `0x04c` | `field_0x4c` | 7 | 10 | 5 |
| `0x050` | `field_0x50` | 125 | 160 | 89 |
| `0x054` | `field_0x54` | 487 | 520 | 347 |
| `0x058` | `field_0x58` | 341 | 364 | 243 |
| `0x05c` | `field_0x5c` | 63 | 51 | 30 |
| `0x060` | `field_0x60` | 90 | 79 | 44 |
| `0x064` | `field_0x64` | 112 | 108 | 60 |
| `0x068` | `field_0x68` | 566 | 545 | 303 |
| `0x06c` | `field_0x6c` | 191 | 170 | 131 |
| `0x070` | `field_0x70_rowHeight` | 63 | 51 | 30 |
| `0x074` | `field_0x74` | 161 | 207 | 115 |
| `0x078` | `field_0x78` | 369 | 473 | 263 |
| `0x07c` | `field_0x7c` | 12 | 13 | 9 |
| `0x080` | `field_0x80_touchMargin` | 14 | 15 | 10 |
| `0x084` | `field_0x84` | 112 | 120 | 80 |
| `0x088` | `field_0x88` | 42 | 45 | 30 |
| `0x08c` | `field_0x8c` | 45 | 57 | 32 |
| `0x090` | `field_0x90` | 0.462222219 | 0.324999988 | 0.649999976 |
| `0x094` | `field_0x94` | 14 | 15 | 10 |
| `0x098` | `field_0x98` | 590 | 630 | 420 |
| `0x09c` | `field_0x9c` | 70 | 75 | 50 |
| `0x0a0` | `field_0xa0` | 35 | 45 | 25 |
| `0x0a4` | `field_0xa4` | 7 | 9 | 5 |
| `0x0a8` | `field_0xa8` | 85 | 61 | 41 |
| `0x0ac` | `field_0xac` | 85 | 61 | 41 |
| `0x0b0` | `field_0xb0` | 4 | 4 | 3 |
| `0x0b4` | `field_0xb4` | 20 | 22 | 15 |
| `0x0b8` | `field_0xb8` | 25 | 25 | 18 |
| `0x0bc` | `field_0xbc` | 11 | 12 | 8 |
| `0x0c0` | `field_0xc0` | 9 | 11 | 6 |
| `0x0c4` | `field_0xc4` | 11 | 11 | 8 |
| `0x0c8` | `field_0xc8` | 5 | 10 | 4 |
| `0x0cc` | `field_0xcc` | 253 | 270 | 180 |
| `0x0d0` | `centerYBase` | 21 | 22 | 15 |
| `0x0d4` | `marqueeWidth` | 28 | 36 | 20 |
| `0x0d8` | `progressBarYBase` | 52 | 55 | 37 |
| `0x0dc` | `field_0xdc` | 14.0625 | 15 | 10 |
| `0x0e0` | `animSpeedScale` | 1.40625 | 1.5 | 1 |
| `0x0e4` | `driftSpeedDivisor` | 28.125 | 12 | 20 |
| `0x0e8` | `ringRadiusScale` | 1.40625 | 1.5 | 1 |
| `0x0ec` | `ringNearMidThreshold` | 56 | 60 | 40 |
| `0x0f0` | `field_0xf0` | 112 | 120 | 80 |
| `0x0f4` | `ringMidFarThreshold` | 281 | 300 | 200 |
| `0x0f8` | `field_0xf8` | 562 | 600 | 400 |
| `0x0fc` | `hudInset` | 7 | 0 | 5 |
| `0x100` | `oreLabelYOffset` | 2 | 5 | 2 |
| `0x104` | `oreTextYOffset` | 5 | 4 | 3 |
| `0x108` | `field_0x108` | -2 | -1 | 0 |
| `0x10c` | `field_0x10c_rowBaseY` | 1 | 2 | 1 |
| `0x110` | `field_0x110_strip58` | 2 | 1 | 2 |
| `0x114` | `field_0x114_strip5c` | 1 | -1 | 1 |
| `0x118` | `field_0x118` | 9 | 13 | 7 |
| `0x11c` | `field_0x11c` | 35 | 37 | 25 |
| `0x120` | `field_0x120` | 21 | 22 | 15 |
| `0x124` | `field_0x124` | 1 | 2 | 1 |
| `0x128` | `field_0x128` | 1 | 2 | 1 |
| `0x12c` | `field_0x12c` | 21 | 22 | 15 |
| `0x130` | `field_0x130` | 78 | 60 | 40 |
| `0x134` | `field_0x134` | 151 | 120 | 80 |
| `0x138` | `field_0x138` | 66 | 52 | 35 |
| `0x13c` | `field_0x13c` | 23 | 25 | 17 |
| `0x140` | `field_0x140` | 15 | 19 | 11 |
| `0x144` | `field_0x144` | 18 | 19 | 13 |
| `0x148` | `field_0x148` | 2 | 19 | 2 |
| `0x14c` | `field_0x14c` | 189 | 243 | 135 |
| `0x150` | `field_0x150` | 5 | 6 | 4 |
| `0x154` | `field_0x154` | 70 | 14 | 15 |
| `0x158` | `field_0x158` | 70 | 13 | 14 |
| `0x15c` | `field_0x15c` | 206 | 136 | 79 |
| `0x160` | `field_0x160` | 68 | 13 | 13 |
| `0x164` | `field_0x164` | 0 | 23 | 13 |
| `0x168` | `field_0x168` | 35 | 37 | 25 |
| `0x16c` | `field_0x16c` | 184 | 115 | 69 |
| `0x170` | `field_0x170` | 174 | 106 | 64 |
| `0x174` | `field_0x174` | 84 | 45 | 30 |
| `0x178` | `field_0x178` | 84 | 27 | 22 |
| `0x17c` | `field_0x17c` | 217 | 146 | 85 |
| `0x180` | `field_0x180` | 23 | 30 | 17 |
| `0x184` | `field_0x184` | 163 | 210 | 116 |
| `0x188` | `field_0x188` | 23 | 30 | 17 |
| `0x18c` | `field_0x18c` | 244 | 320 | 174 |
| `0x190` | `field_0x190` | -6 | -4 | -1 |
| `0x194` | `field_0x194` | 8 | 10 | 6 |
| `0x198` | `field_0x198` | 16 | 9 | 6 |
| `0x19c` | `field_0x19c` | 90 | 54 | 45 |
| `0x1a0` | `field_0x1a0` | 18 | 10 | 7 |
| `0x1a4` | `field_0x1a4` | 22 | 28 | 16 |
| `0x1a8` | `field_0x1a8` | 22 | 24 | 16 |
| `0x1ac` | `field_0x1ac` | 243 | 208 | 116 |
| `0x1b0` | `field_0x1b0` | 18 | 19 | 13 |
| `0x1b4` | `field_0x1b4` | 42 | 45 | 30 |
| `0x1b8` | `field_0x1b8` | 46 | 40 | 33 |
| `0x1bc` | `field_0x1bc` | 1 | 2 | 1 |
| `0x1c0` | `field_0x1c0` | 92 | 80 | 66 |
| `0x1c4` | `field_0x1c4` | 26 | 34 | 19 |
| `0x1c8` | `field_0x1c8` | 40 | 43 | 29 |
| `0x1cc` | `field_0x1cc` | 50 | 54 | 36 |
| `0x1d0` | `field_0x1d0` | 7 | 7 | 5 |
| `0x1d4` | `field_0x1d4` | 17 | 18 | 12 |
| `0x1d8` | `field_0x1d8` | 5 | 6 | 4 |
| `0x1dc` | `field_0x1dc` | 14 | 14 | 10 |
| `0x1e0` | `eventBannerScale` | 1.40625 | 2 | 1 |
| `0x1e4` | `field_0x1e4` | 5 | 6 | 4 |
| `0x1e8` | `field_0x1e8` | 1 | 1 | 0 |
| `0x1ec` | `field_0x1ec` | 26 | 34 | 19 |
| `0x1f0` | `field_0x1f0` | 26 | 34 | 19 |
| `0x1f4` | `field_0x1f4` | 22 | 2 | 2 |
| `0x1f8` | `field_0x1f8` | 2 | 2 | 2 |
| `0x1fc` | `field_0x1fc` | 2 | 4 | 2 |
| `0x200` | `field_0x200` | -18 | -26 | -13 |
| `0x204` | `field_0x204` | 12 | 16 | 9 |
| `0x208` | `field_0x208` | 5 | 7 | 4 |
| `0x20c` | `field_0x20c` | 7 | 7 | 5 |
| `0x210` | `field_0x210` | 5 | 6 | 4 |
| `0x214` | `field_0x214` | 1 | -2 | 1 |
| `0x218` | `field_0x218` | 7 | 7 | 5 |
| `0x21c` | `orbitInfoTextX` | 8 | 10 | 6 |
| `0x220` | `orbitInfoStationY` | 7 | 7 | 5 |
| `0x224` | `orbitInfoSystemY` | 28 | 30 | 20 |
| `0x228` | `orbitInfoSecurityY` | 49 | 52 | 35 |
| `0x22c` | `quickMenuHeaderLift` | 2 | 4 | 2 |
| `0x230` | `quickMenuIconGap` | 1 | 2 | 1 |
| `0x234` | `field_0x234` | 1 | 2 | 1 |
| `0x238` | `field_0x238_blk16.a` | 70 | 90 | 50 |
| `0x23c` | `field_0x238_blk16.b` | 21 | 27 | 15 |
| `0x240` | `field_0x238_blk16.c` | 7 | 9 | 5 |
| `0x244` | `field_0x238_blk16.d` | 4 | 5 | 3 |
| `0x248` | `field_0x248` | 239 | 306 | 170 |
| `0x24c` | `field_0x24c` | -2 | -1 | 0 |
| `0x250` | `field_0x250` | 1 | 2 | 1 |
| `0x254` | `field_0x254` | 5 | 7 | 4 |
| `0x258` | `field_0x258` | 2 | 4 | 2 |
| `0x25c` | `field_0x25c` | 22 | 28 | 16 |
| `0x260` | `field_0x260` | 1 | 0 | 1 |
| `0x264` | `field_0x264` | 421 | 450 | 300 |
| `0x268` | `field_0x268` | 11 | 12 | 8 |
| `0x26c` | `field_0x26c` | 92 | 99 | 66 |
| `0x270` | `field_0x270` | 98 | 105 | 70 |
| `0x274` | `field_0x274` | 136 | 145 | 97 |
| `0x278` | `field_0x278` | 421 | 450 | 300 |
| `0x27c` | `field_0x27c` | 28 | 30 | 20 |
| `0x280` | `field_0x280` | 0 | 4 | 0 |
| `0x284` | `field_0x284_sliderSlot5Enabled` | 1 | 0 | 1 |
| `0x285` | `field_0x285` | 1 | 1 | 1 |
| `0x286` | `field_0x286` | 1 | 0 | 1 |
| `0x287` | `_pad_0x287` | - | - | - |
| `0x288` | `field_0x288` | 0 | 20 | 0 |
| `0x28c` | `field_0x28c` | 0 | -19 | 0 |
| `0x290` | `field_0x290` | 3 | 4 | 3 |
| `0x294` | `field_0x294_buttonWidth` | 281 | 360 | 200 |
| `0x298` | `field_0x298_buttonYBias` | 118 | 146 | 84 |
| `0x29c` | `field_0x29c_buttonRowGap` | 16 | 8 | 12 |
| `0x2a0` | `field_0x2a0_listRowGap` | 16 | 6 | 12 |
| `0x2a4` | `field_0x2a4_metricA` | 154 | 198 | 110 |
| `0x2a8` | `field_0x2a8_metricB` | 140 | 180 | 100 |
| `0x2ac` | `field_0x2ac_metricC` | 147 | 189 | 105 |
| `0x2b0` | `field_0x2b0` | 42 | 54 | 30 |
| `0x2b4` | `field_0x2b4` | 35 | 37 | 25 |
| `0x2b8` | `field_0x2b8` | 9 | 14 | 7 |
| `0x2bc` | `field_0x2bc` | 2 | 3 | 2 |
| `0x2c0` | `field_0x2c0` | -2 | -3 | -2 |
| `0x2c4` | `field_0x2c4` | 126 | 108 | 60 |
| `0x2c8` | `field_0x2c8` | 63 | 45 | 30 |
| `0x2cc` | `field_0x2cc` | 126 | 106 | 60 |
| `0x2d0` | `field_0x2d0` | 63 | 53 | 30 |
| `0x2d4` | `field_0x2d4` | 112 | 100 | 60 |
| `0x2d8` | `field_0x2d8` | 141 | 125 | 75 |
| `0x2dc` | `windowX` | - | - | - |
| `0x2e0` | `windowY` | - | - | - |
| `0x2e4` | `windowWidth` | - | - | - |
| `0x2e8` | `windowHeight` | - | - | - |
| `0x2ec` | `rewardMessageActive` | - | - | - |
| `0x2ed` | `rewardMessageFlag` | - | - | - |
| `0x2ee` | `_pad_0x2ee` | - | - | - |
| `0x2f0` | `backButtonWidth` | - | - | - |
| `0x2f4` | `field_0x2f4` | 112 | 45 | 30 |
| `0x2f8` | `field_0x2f8` | 281.25 | 150 | 100 |
| `0x2fc` | `field_0x2fc` | 50 | 36 | 36 |
| `0x300` | `field_0x300` | 281 | 200 | 200 |
| `0x304` | `field_0x304` | 492 | 350 | 350 |
| `0x308` | `choiceWindowTitleHeight` | 351 | 250 | 250 |
| `0x30c` | `field_0x30c` | 3 | 4 | 3 |
| `0x310` | `field_0x310` | -5 | -8 | -5 |
| `0x314` | `field_0x314` | -5 | -8 | -5 |
| `0x318` | `field_0x318` | 0 | 0 | 0 |
| `0x31c` | `helpWindowX` | 331 | 236 | 118 |
| `0x320` | `helpWindowY` | 202 | 150 | 76 |
| `0x324` | `bgPatternImage` | - | - | - |
| `0x328` | `headerPatternImage` | - | - | - |
| `0x32c` | `headerIconImage` | - | - | - |
| `0x330` | `headerCapImage` | - | - | - |
| `0x334` | `footerImageLeft` | - | - | - |
| `0x338` | `footerFillImage` | - | - | - |
| `0x33c` | `footerImageRight` | - | - | - |
| `0x340` | `field_0x340` | - | - | - |
| `0x344` | `footerPatternImage` | - | - | - |
| `0x348` | `field_0x348` | - | - | - |
| `0x34c` | `field_0x34c` | - | - | - |
| `0x350` | `field_0x350` | - | - | - |
| `0x354` | `field_0x354` | - | - | - |
| `0x358` | `field_0x358` | - | - | - |
| `0x35c` | `field_0x35c` | - | - | - |
| `0x360` | `field_0x360` | - | - | - |
| `0x364` | `field_0x364` | - | - | - |
| `0x368` | `field_0x368` | - | - | - |
| `0x36c` | `field_0x36c` | - | - | - |
| `0x370` | `field_0x370` | - | - | - |
| `0x374` | `scrollBarImage` | - | - | - |
| `0x378` | `scrollBarFillImage` | - | - | - |
| `0x37c` | `field_0x37c` | - | - | - |
| `0x380` | `field_0x380` | - | - | - |
| `0x384` | `field_0x384` | - | - | - |
| `0x388` | `field_0x388` | - | - | - |
| `0x38c` | `field_0x38c` | - | - | - |
| `0x390` | `field_0x390` | - | - | - |
| `0x394` | `field_0x394` | - | - | - |
| `0x398` | `tipBoxImage` | - | - | - |
| `0x39c` | `field_0x39c` | - | - | - |
| `0x3a0` | `field_0x3a0` | - | - | - |
| `0x3a4` | `rewardIconImage` | - | - | - |
| `0x3a8` | `field_0x3a8` | - | - | - |
| `0x3ac` | `textBaselineAdjust` | - | - | - |
| `0x3b0` | `drawColor` | - | - | - |
| `0x3b4` | `backButton` | - | - | - |
| `0x3b8` | `secondaryButton` | - | - | - |
| `0x3bc` | `helpButton` | - | - | - |
| `0x3c0` | `helpPressedFlag` | - | - | - |
| `0x3c1` | `_pad_0x3c1` | - | - | - |
| `0x3c4` | `choiceWindow` | - | - | - |
| `0x3c8` | `tipLines` | - | - | - |
| `0x3cc` | `helpButtonEnabled` | - | - | - |
| `0x3cd` | `_pad_0x3cd` | - | - | - |
| `0x3d0` | `rewardMessageTimer` | - | - | - |
| `0x3d4` | `rewardCredits` | - | - | - |
| `0x3d8` | `field_0x3d8` | - | - | - |
| `0x3dc` | `field_0x3dc` | - | - | - |
| `0x3dd` | `_pad_0x3dd` | - | - | - |
| `0x3e0` | `scrollBarInset` | 1 | 2 | 1 |
| `0x3e4` | `scrollBarHandle` | 0 | 1 | 0 |
| `0x3e8` | `rewardBoxWidth` | 140 | 112 | 75 |
| `0x3ec` | `rewardBoxHeight` | 196 | 352 | 196 |
| `0x3f0` | `rewardBoxX` | 50 | 75 | 50 |
| `0x3f4` | `rewardBoxY` | 77 | 57 | 38 |
| `0x3f8` | `rewardBoxY2` | 132 | 111 | 55 |
| `0x3fc` | `footerButtonOffset` | 4 | 5 | 3 |
| `0x400` | `fading` | - | - | - |
| `0x401` | `fadeOut` | - | - | - |
| `0x402` | `_pad_0x402` | - | - | - |
| `0x404` | `fadeColor` | - | - | - |
| `0x408` | `fadeProgress` | - | - | - |
| `0x40c` | `fadeDuration` | - | - | - |
| `0x410` | `fillScreen` | - | - | - |


Notes:

* The `0x238/0x23c/0x240/0x244` entries are the four `Blk16` sub-fields
  `field_0x238_blk16.a/.b/.c/.d`.
* `0x284/0x285/0x286` are byte stores; all other entries are 4-byte stores.
* The `0x3e0..0x3fc` entries are the image-id fields (`bgPatternImage`,
  `headerPatternImage`, ...); they are written once each in the original
  constructor, not by `reload()`.
* The store order is config-major: the first block is entirely
  `iPadHD`-specific, then the `n9` block, then the `else` block, then the two
  common tails.

## 3. Literal-pool census

The post-`pop` region is `966 B` = `20` cold-block instruction words + `2 B`
pad + `221` literal-pool words.  The 221 words are:

* **200 words** = 50 `vld1.64` groups of four consecutive field values;
* **14 words** = scalar `float` constants;
* **7 words** = GOT-relative `Globals::*` deltas.

### 3.1 The 50 end-pool `vld1.64` groups (200 words)

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

### 3.2 The 7 GOT deltas and 14 scalar floats (21 words)

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

`200 + 7 + 14 = 221`.

The current baseline has 42 end-pool `vld1.64` groups (168 words), 14 scalar
floats and **8** GOT deltas (190 words total).  The net deficit is 8 groups =
32 words: 15 original-only groups minus 7 baseline-only groups.  The extra
baseline GOT delta is a second `Globals::iPadLarge` test in the
`field_0x2f4`/`field_0x2f8` pair.  The only genuine source-type mismatch is
`field_0xa8`: the original uses an integer chain
(`movs r2,#82` / `moveq r2,#41` / `vmov` / `vcvt`), while the baseline uses
float arms (`82.0f`/`41.0f`).

## 4. Concrete reconstruction proposal

The body below is generated directly from the original store table, with the
original iOS `Layout::Layout` source order inside each configuration block
(`_work/reconstructions/layout_ctor/ios_source_order.md`).  It is
**semantically exact for all 32 flag combinations**: the emulator comparison
reports `bad combos 0 of 32`, 807 stored bytes, 202 stored words.

It compiles to **6672 B** and scores **27.729913844921 %** on the official
fuzzy metric (versus the current field-major baseline at **7072 B /
48.963317384370 %**).  It is therefore a structural reconstruction, not a
score win.  The implementer can land it as the readable config-major body, or
keep the current best-scoring field-major body; either way `byte_exact` and
`linked_exact` stay at 4505 because `Layout::Layout` is already non-exact.

```cpp
Layout::Layout() {
    if (Globals::iPadHD) {
        this->field_0x4 = 21;
        this->windowTopInset = 42;
        this->field_0xc_leftMargin = 40;
        this->field_0x10_rightMargin = 52;
        this->footerTextInset = 30;
        this->headerTitleY = 12;
        this->field_0x1c = 28;
        this->field_0x20_top = 7;
        this->field_0x24 = 7;
        this->buttonInsetX = 7;
        this->field_0x2c_rowHeight = 4;
        this->field_0x30_rowHeight = 42;
        this->field_0x34 = 8;
        this->field_0x38 = 9;
        this->field_0x3c = 82;
        this->field_0x40 = 140;
        this->field_0x44 = 7;
        this->field_0x48 = 15;
        this->field_0x4c = 7;
        this->field_0x50 = 125;
        this->field_0x54 = 487;
        this->field_0x58 = 341;
        this->field_0x5c = 63;
        this->field_0x60 = 90;
        this->field_0x64 = 112;
        this->field_0x68 = 566;
        this->field_0x6c = 191;
        this->field_0x2c0 = -2;
        this->field_0x70_rowHeight = 63;
        this->field_0x74 = 161;
        this->field_0x78 = 369;
        this->field_0x2c4 = 126;
        this->field_0x2c8 = 63;
        this->field_0x2cc = 126;
        this->field_0x2d0 = 63;
        this->field_0x2d4 = 112;
        this->field_0x2d8 = 141;
        this->field_0x7c = 12;
        this->field_0x80_touchMargin = 14;
        this->field_0x84 = 112;
        this->field_0x88 = 42;
        this->field_0x8c = 45;
        this->field_0x90 = 0.46222221851348877f;
        this->field_0x94 = 14;
        this->field_0x98 = 590;
        this->field_0x9c = 70;
        this->field_0xac = 85;
        this->field_0xa8 = 85;
        this->field_0xa0 = 35;
        this->field_0xa4 = 7;
        this->field_0xb0 = 4;
        this->field_0xb8 = 25;
        this->field_0xb4 = 20;
        this->field_0xbc = 11;
        this->field_0xc0 = 9;
        this->field_0xc4 = 11;
        this->field_0xc8 = 5;
        this->field_0xcc = 253;
        this->centerYBase = 21;
        this->marqueeWidth = 28;
        this->progressBarYBase = 52;
        this->field_0xdc = 14.0625f;
        this->animSpeedScale = 1.40625f;
        this->driftSpeedDivisor = 28.125f;
        this->ringRadiusScale = 1.40625f;
        this->ringNearMidThreshold = 56;
        this->field_0xf0 = 112;
        this->ringMidFarThreshold = 281;
        this->field_0xf8 = 562;
        this->hudInset = 7;
        this->oreLabelYOffset = 2;
        this->oreTextYOffset = 5;
        this->field_0x294_buttonWidth = 281;
        this->field_0x298_buttonYBias = 118;
        this->field_0x29c_buttonRowGap = 16;
        this->field_0x2a0_listRowGap = 16;
        this->field_0x2a4_metricA = 154;
        this->field_0x2a8_metricB = 140;
        this->field_0x2ac_metricC = 147;
        this->field_0x2b0 = 42;
        this->field_0x2b4 = 35;
        this->field_0x2b8 = 9;
        this->field_0x108 = -2;
        this->field_0x10c_rowBaseY = 1;
        this->field_0x110_strip58 = 2;
        this->field_0x114_strip5c = 1;
        this->field_0x118 = 9;
        this->field_0x11c = 35;
        this->field_0x120 = 21;
        this->field_0x124 = 1;
        this->field_0x128 = 1;
        this->field_0x12c = 21;
        this->field_0x130 = 78;
        this->field_0x134 = 151;
        this->field_0x138 = 66;
        this->field_0x13c = 23;
        this->field_0x140 = 15;
        this->field_0x144 = 18;
        this->field_0x148 = 2;
        this->field_0x14c = 189;
        this->field_0x150 = 5;
        this->field_0x154 = 70;
        this->field_0x158 = 70;
        this->field_0x15c = 206;
        this->field_0x160 = 68;
        this->field_0x164 = 0;
        this->field_0x168 = 35;
        this->field_0x16c = 184;
        this->field_0x170 = 174;
        this->field_0x174 = 84;
        this->field_0x178 = 84;
        this->field_0x17c = 217;
        this->field_0x180 = 23;
        this->field_0x184 = 163;
        this->field_0x188 = 23;
        this->field_0x18c = 244;
        this->field_0x190 = -6;
        this->field_0x194 = 8;
        this->field_0x198 = 16;
        this->field_0x19c = 90;
        this->field_0x1a0 = 18;
        this->field_0x1a4 = 22;
        this->field_0x1a8 = 22;
        this->field_0x1ac = 243;
        this->field_0x1b0 = 18;
        this->field_0x1b4 = 42;
        this->field_0x1b8 = 46;
        this->field_0x1bc = 1;
        this->field_0x1c0 = 92;
        this->field_0x1c4 = 26;
        this->field_0x1c8 = 40;
        this->field_0x1cc = 50;
        this->field_0x1d0 = 7;
        this->field_0x1d4 = 17;
        this->field_0x1d8 = 5;
        this->field_0x1dc = 14;
        this->eventBannerScale = 1.40625f;
    } else {
        if (Globals::n9) {
            this->field_0x4 = 22;
            this->windowTopInset = 45;
            this->field_0xc_leftMargin = 51;
            this->field_0x10_rightMargin = 58;
            this->footerTextInset = 37;
            this->headerTitleY = 11;
            this->field_0x1c = 30;
            this->field_0x20_top = 7;
            this->field_0x24 = 7;
            this->buttonInsetX = 9;
            this->field_0x2c_rowHeight = 6;
            this->field_0x30_rowHeight = 51;
            this->field_0x34 = 4;
            this->field_0x38 = 12;
            this->field_0x3c = 106;
            this->field_0x40 = 180;
            this->field_0x44 = 9;
            this->field_0x48 = 16;
            this->field_0x4c = 10;
            this->field_0x50 = 160;
            this->field_0x54 = 520;
            this->field_0x58 = 364;
            this->field_0x5c = 51;
            this->field_0x60 = 79;
            this->field_0x64 = 108;
            this->field_0x68 = 545;
            this->field_0x6c = 170;
            this->field_0x2c0 = -3;
            this->field_0x70_rowHeight = 51;
            this->field_0x74 = 207;
            this->field_0x78 = 473;
            this->field_0x2c4 = 108;
            this->field_0x2c8 = 45;
            this->field_0x2cc = 106;
        } else {
            this->field_0x4 = Globals::retinaDisplay ? 30 : 15;
            this->windowTopInset = Globals::retinaDisplay ? 60 : 30;
            this->field_0xc_leftMargin = Globals::retinaDisplay ? 58 : 29;
            this->field_0x10_rightMargin = Globals::retinaDisplay ? 74 : 37;
            this->footerTextInset = Globals::retinaDisplay ? 50 : 22;
            this->headerTitleY = Globals::retinaDisplay ? 14 : 9;
            this->field_0x1c = Globals::retinaDisplay ? 40 : 20;
            this->field_0x20_top = Globals::retinaDisplay ? 10 : 5;
            this->field_0x24 = Globals::retinaDisplay ? 10 : 5;
            this->buttonInsetX = Globals::retinaDisplay ? 10 : 5;
            this->field_0x2c_rowHeight = Globals::retinaDisplay ? 6 : 3;
            this->field_0x30_rowHeight = Globals::retinaDisplay ? 60 : 30;
            this->field_0x34 = Globals::retinaDisplay ? 12 : 6;
            this->field_0x38 = Globals::retinaDisplay ? 14 : 7;
            this->field_0x3c = Globals::retinaDisplay ? 118 : 59;
            this->field_0x40 = Globals::retinaDisplay ? 200 : 100;
            this->field_0x44 = Globals::retinaDisplay ? 10 : 5;
            this->field_0x48 = Globals::retinaDisplay ? 22 : 11;
            this->field_0x4c = Globals::retinaDisplay ? 10 : 5;
            this->field_0x50 = Globals::retinaDisplay ? 178 : 89;
            this->field_0x54 = Globals::retinaDisplay ? 694 : 347;
            this->field_0x58 = Globals::retinaDisplay ? 486 : 243;
            this->field_0x5c = Globals::iPadLarge ? 90 : (Globals::retinaDisplay ? 60 : (Globals::iPad ? 45 : 30));
            this->field_0x60 = Globals::iPadLarge ? 128 : (Globals::retinaDisplay ? 88 : (Globals::iPad ? 64 : 44));
            this->field_0x64 = Globals::iPadLarge ? 160 : (Globals::retinaDisplay ? 120 : (Globals::iPad ? 80 : 60));
            this->field_0x68 = Globals::iPadLarge ? 806 : (Globals::retinaDisplay ? 606 : (Globals::iPad ? 403 : 303));
            this->field_0x6c = Globals::iPadLarge ? 272 : (Globals::retinaDisplay ? 262 : (Globals::iPad ? 136 : 131));
            this->field_0x2c0 = Globals::retinaDisplay ? -4 : -2;
            this->field_0x70_rowHeight = Globals::iPadLarge ? 90 : (Globals::retinaDisplay ? 60 : (Globals::iPad ? 45 : 30));
            this->field_0x74 = Globals::retinaDisplay ? 230 : 115;
            this->field_0x78 = Globals::retinaDisplay ? 526 : 263;
            this->field_0x2c4 = Globals::iPadLarge ? 180 : (Globals::retinaDisplay ? 120 : (Globals::iPad ? 90 : 60));
            this->field_0x2c8 = Globals::iPadLarge ? 90 : (Globals::retinaDisplay ? 60 : (Globals::iPad ? 45 : 30));
            this->field_0x2cc = Globals::iPadLarge ? 180 : (Globals::retinaDisplay ? 120 : (Globals::iPad ? 90 : 60));
        }
        this->field_0x2d0 = Globals::n9 ? 53 : (Globals::iPadLarge ? 90 : (Globals::retinaDisplay ? 60 : (Globals::iPad ? 45 : 30)));
        this->field_0x2d4 = (Globals::n9 ? (Globals::iPad ? 160 : 100) : (Globals::iPad ? (Globals::retinaDisplay ? 160 : 100) : (Globals::retinaDisplay ? 100 : 60)));
        this->field_0x2d8 = (Globals::n9 ? (Globals::iPad ? 200 : 125) : (Globals::iPad ? (Globals::retinaDisplay ? 200 : 125) : (Globals::retinaDisplay ? 125 : 75)));
        this->field_0x7c = Globals::n9 ? 13 : (Globals::retinaDisplay ? 18 : 9);
        this->field_0x80_touchMargin = Globals::n9 ? 15 : (Globals::retinaDisplay ? 20 : 10);
        this->field_0x84 = Globals::n9 ? 120 : (Globals::retinaDisplay ? 160 : 80);
        this->field_0x88 = Globals::n9 ? 45 : (Globals::retinaDisplay ? 60 : 30);
        this->field_0x8c = Globals::n9 ? 57 : (Globals::retinaDisplay ? 64 : 32);
        this->field_0x90 = Globals::n9 ? 0.32499998807907104f : (Globals::iPad ? 0.32499998807907104f : (Globals::retinaDisplay ? 0.32499998807907104f : 0.6499999761581421f));
        this->field_0x94 = Globals::n9 ? 15 : (Globals::retinaDisplay ? 20 : 10);
        this->field_0x98 = Globals::n9 ? 630 : (Globals::retinaDisplay ? 840 : 420);
        this->field_0x9c = Globals::n9 ? 75 : (Globals::retinaDisplay ? 100 : 50);
        this->field_0xac = Globals::iPadLarge ? 122 : (Globals::n9 ? 61 : (Globals::iPad ? 61 : (Globals::retinaDisplay ? 82 : 41)));
        this->field_0xa8 = Globals::iPadLarge ? 122 : (Globals::n9 ? 61 : (Globals::iPad ? 61 : (Globals::retinaDisplay ? 82 : 41)));
        this->field_0xa0 = Globals::n9 ? 45 : (Globals::retinaDisplay ? 50 : 25);
        this->field_0xa4 = Globals::n9 ? 9 : (Globals::retinaDisplay ? 10 : 5);
        this->field_0xb0 = Globals::n9 ? 4 : (Globals::retinaDisplay ? 6 : 3);
        this->field_0xb8 = Globals::n9 ? 25 : (Globals::retinaDisplay ? 36 : 18);
        this->field_0xb4 = Globals::n9 ? 22 : (Globals::retinaDisplay ? 29 : 15);
        this->field_0xbc = Globals::n9 ? 12 : (Globals::retinaDisplay ? 16 : 8);
        this->field_0xc0 = Globals::n9 ? 11 : (Globals::retinaDisplay ? 13 : 6);
        this->field_0xc4 = Globals::n9 ? 11 : (Globals::retinaDisplay ? 14 : 8);
        this->field_0xc8 = Globals::n9 ? 10 : (Globals::retinaDisplay ? 10 : 4);
        this->field_0xcc = Globals::n9 ? 270 : (Globals::retinaDisplay ? 360 : 180);
        this->centerYBase = Globals::n9 ? 22 : (Globals::retinaDisplay ? 30 : 15);
        this->marqueeWidth = Globals::n9 ? 36 : (Globals::retinaDisplay ? 40 : 20);
        this->progressBarYBase = Globals::n9 ? 55 : (Globals::retinaDisplay ? 74 : 37);
        this->field_0xdc = Globals::n9 ? 15.0f : (Globals::retinaDisplay ? 20.0f : 10.0f);
        this->animSpeedScale = Globals::n9 ? 1.5f : (Globals::retinaDisplay ? 2.0f : 1.0f);
        this->driftSpeedDivisor = Globals::n9 ? 12.0f : (Globals::retinaDisplay ? 10.0f : 20.0f);
        this->ringRadiusScale = Globals::n9 ? 1.5f : (Globals::retinaDisplay ? 2.0f : 1.0f);
        this->ringNearMidThreshold = Globals::n9 ? 60 : (Globals::retinaDisplay ? 80 : 40);
        this->field_0xf0 = Globals::n9 ? 120 : (Globals::retinaDisplay ? 160 : 80);
        this->ringMidFarThreshold = Globals::n9 ? 300 : (Globals::retinaDisplay ? 400 : 200);
        this->field_0xf8 = Globals::n9 ? 600 : (Globals::retinaDisplay ? 800 : 400);
        this->hudInset = Globals::n9 ? 0 : (Globals::retinaDisplay ? 10 : 5);
        this->oreLabelYOffset = Globals::n9 ? 5 : (Globals::retinaDisplay ? 4 : 2);
        this->oreTextYOffset = Globals::n9 ? 4 : (Globals::retinaDisplay ? 5 : 3);
        this->field_0x294_buttonWidth = Globals::n9 ? 360 : (Globals::retinaDisplay ? 400 : 200);
        this->field_0x298_buttonYBias = Globals::n9 ? 146 : (Globals::retinaDisplay ? 168 : 84);
        this->field_0x29c_buttonRowGap = Globals::n9 ? 8 : (Globals::retinaDisplay ? 24 : 12);
        this->field_0x2a0_listRowGap = Globals::n9 ? 6 : (Globals::retinaDisplay ? 24 : 12);
        this->field_0x2a4_metricA = Globals::n9 ? 198 : (Globals::retinaDisplay ? 220 : 110);
        this->field_0x2a8_metricB = Globals::n9 ? 180 : (Globals::retinaDisplay ? 200 : 100);
        this->field_0x2ac_metricC = Globals::n9 ? 189 : (Globals::retinaDisplay ? 210 : 105);
        this->field_0x2b0 = Globals::n9 ? 54 : (Globals::retinaDisplay ? 60 : 30);
        this->field_0x2b4 = Globals::n9 ? 37 : (Globals::retinaDisplay ? 50 : 25);
        this->field_0x2b8 = Globals::n9 ? 14 : (Globals::retinaDisplay ? 16 : 7);
        this->field_0x108 = Globals::n9 ? -1 : (Globals::retinaDisplay ? -2 : 0);
        this->field_0x10c_rowBaseY = Globals::n9 ? 2 : (Globals::retinaDisplay ? 2 : 1);
        this->field_0x110_strip58 = Globals::n9 ? 1 : (Globals::retinaDisplay ? 1 : 2);
        this->field_0x114_strip5c = Globals::n9 ? -1 : (Globals::retinaDisplay ? -1 : 1);
        this->field_0x118 = Globals::n9 ? 13 : (Globals::retinaDisplay ? 14 : 7);
        this->field_0x11c = Globals::n9 ? 37 : (Globals::retinaDisplay ? 50 : 25);
        this->field_0x120 = Globals::n9 ? 22 : (Globals::retinaDisplay ? 28 : 15);
        this->field_0x124 = Globals::n9 ? 2 : (Globals::retinaDisplay ? 2 : 1);
        this->field_0x128 = Globals::n9 ? 2 : (Globals::retinaDisplay ? 2 : 1);
        this->field_0x12c = Globals::n9 ? 22 : (Globals::retinaDisplay ? 30 : 15);
        this->field_0x130 = Globals::n9 ? 60 : (Globals::iPadLarge ? 112 : (Globals::retinaDisplay ? 80 : (Globals::iPad ? 56 : 40)));
        this->field_0x134 = Globals::n9 ? 120 : (Globals::iPadLarge ? 216 : (Globals::retinaDisplay ? 160 : (Globals::iPad ? 108 : 80)));
        this->field_0x138 = Globals::n9 ? 52 : (Globals::iPadLarge ? 94 : (Globals::retinaDisplay ? 70 : (Globals::iPad ? 47 : 35)));
        this->field_0x13c = Globals::n9 ? 25 : (Globals::retinaDisplay ? 34 : 17);
        this->field_0x140 = Globals::n9 ? 19 : (Globals::retinaDisplay ? 22 : 11);
        this->field_0x144 = Globals::n9 ? 19 : (Globals::retinaDisplay ? 26 : 13);
        this->field_0x148 = Globals::n9 ? 19 : (Globals::retinaDisplay ? 4 : 2);
        this->field_0x14c = Globals::n9 ? 243 : (Globals::retinaDisplay ? 270 : 135);
        this->field_0x150 = Globals::n9 ? 6 : (Globals::retinaDisplay ? 8 : 4);
        this->field_0x154 = Globals::iPadLarge ? 100 : (Globals::iPad ? 50 : (Globals::n9 ? 14 : (Globals::retinaDisplay ? 30 : 15)));
        this->field_0x158 = Globals::iPadLarge ? 100 : (Globals::iPad ? 50 : (Globals::n9 ? 13 : (Globals::retinaDisplay ? 28 : 14)));
        this->field_0x15c = Globals::iPadLarge ? 294 : (Globals::iPad ? 147 : (Globals::n9 ? 136 : (Globals::retinaDisplay ? 158 : 79)));
        this->field_0x160 = Globals::iPadLarge ? 98 : (Globals::iPad ? 49 : (Globals::n9 ? 13 : (Globals::retinaDisplay ? 26 : 13)));
        this->field_0x164 = Globals::iPadLarge ? 0 : (Globals::iPad ? 0 : (Globals::n9 ? 23 : (Globals::retinaDisplay ? 25 : 13)));
        this->field_0x168 = Globals::n9 ? 37 : (Globals::retinaDisplay ? 50 : 25);
        this->field_0x16c = Globals::iPadLarge ? 262 : (Globals::iPad ? 131 : (Globals::n9 ? 115 : (Globals::retinaDisplay ? 138 : 69)));
        this->field_0x170 = Globals::iPadLarge ? 248 : (Globals::iPad ? 124 : (Globals::n9 ? 106 : (Globals::retinaDisplay ? 128 : 64)));
        this->field_0x174 = Globals::iPadLarge ? 84 : (Globals::iPad ? 42 : (Globals::n9 ? 45 : (Globals::retinaDisplay ? 60 : 30)));
        this->field_0x178 = Globals::iPadLarge ? 120 : (Globals::iPad ? 60 : (Globals::n9 ? 27 : (Globals::retinaDisplay ? 44 : 22)));
        this->field_0x17c = Globals::iPadLarge ? 310 : (Globals::iPad ? 155 : (Globals::n9 ? 146 : (Globals::retinaDisplay ? 170 : 85)));
        this->field_0x180 = Globals::n9 ? 30 : (Globals::retinaDisplay ? 34 : 17);
        this->field_0x184 = Globals::iPadLarge ? 346 : (Globals::iPad ? 173 : (Globals::n9 ? 210 : (Globals::retinaDisplay ? 232 : 116)));
        this->field_0x188 = Globals::n9 ? 30 : (Globals::retinaDisplay ? 34 : 17);
        this->field_0x18c = Globals::iPadLarge ? 446 : (Globals::iPad ? 223 : (Globals::n9 ? 320 : (Globals::retinaDisplay ? 348 : 174)));
        this->field_0x190 = Globals::iPadLarge ? -8 : (Globals::n9 ? -4 : (Globals::iPad ? -4 : (Globals::retinaDisplay ? -2 : -1)));
        this->field_0x194 = Globals::n9 ? 10 : (Globals::retinaDisplay ? 12 : 6);
        this->field_0x198 = Globals::iPadLarge ? 24 : (Globals::iPad ? 12 : (Globals::n9 ? 9 : (Globals::retinaDisplay ? 12 : 6)));
        this->field_0x19c = Globals::iPadLarge ? 128 : (Globals::iPad ? 64 : (Globals::n9 ? 54 : (Globals::retinaDisplay ? 90 : 45)));
        this->field_0x1a0 = Globals::iPadLarge ? 26 : (Globals::iPad ? 13 : (Globals::n9 ? 10 : (Globals::retinaDisplay ? 14 : 7)));
        this->field_0x1a4 = Globals::n9 ? 28 : (Globals::retinaDisplay ? 32 : 16);
        this->field_0x1a8 = Globals::n9 ? 24 : (Globals::retinaDisplay ? 32 : 16);
        this->field_0x1ac = Globals::iPadLarge ? 346 : (Globals::iPad ? 173 : (Globals::n9 ? 208 : (Globals::retinaDisplay ? 232 : 116)));
        this->field_0x1b0 = Globals::n9 ? 19 : (Globals::retinaDisplay ? 26 : 13);
        this->field_0x1b4 = Globals::n9 ? 45 : (Globals::retinaDisplay ? 60 : 30);
        this->field_0x1b8 = Globals::n9 ? 40 : (Globals::retinaDisplay ? 66 : 33);
        this->field_0x1bc = Globals::n9 ? 2 : (Globals::retinaDisplay ? 2 : 1);
        this->field_0x1c0 = Globals::n9 ? 80 : (Globals::retinaDisplay ? 132 : 66);
        this->field_0x1c4 = Globals::n9 ? 34 : (Globals::retinaDisplay ? 38 : 19);
        this->field_0x1c8 = Globals::n9 ? 43 : (Globals::retinaDisplay ? 58 : 29);
        this->field_0x1cc = Globals::n9 ? 54 : (Globals::retinaDisplay ? 72 : 36);
        this->field_0x1d0 = Globals::n9 ? 7 : (Globals::retinaDisplay ? 10 : 5);
        this->field_0x1d4 = Globals::n9 ? 18 : (Globals::retinaDisplay ? 24 : 12);
        this->field_0x1d8 = Globals::n9 ? 6 : (Globals::retinaDisplay ? 8 : 4);
        this->field_0x1dc = Globals::n9 ? 14 : (Globals::retinaDisplay ? 20 : 10);
        this->eventBannerScale = Globals::n9 ? 2.0f : (Globals::retinaDisplay ? 2.0f : 1.0f);
    }
    if (Globals::iPadHD) {
        this->field_0x1e4 = 5;
        this->field_0x1e8 = 1;
        this->field_0x1ec = 26;
        this->field_0x1fc = 2;
        this->field_0x204 = 12;
        this->field_0x208 = 5;
        this->field_0x20c = 7;
        this->field_0x210 = 5;
        this->field_0x214 = 1;
        this->field_0x218 = 7;
        this->orbitInfoTextX = 8;
        this->orbitInfoStationY = 7;
        this->orbitInfoSystemY = 28;
        this->orbitInfoSecurityY = 49;
        this->quickMenuHeaderLift = 2;
        this->quickMenuIconGap = 1;
        this->field_0x234 = 1;
        this->field_0x238_blk16.a = 70;
        this->field_0x238_blk16.b = 21;
        this->field_0x238_blk16.c = 7;
        this->field_0x238_blk16.d = 4;
        this->field_0x248 = 239;
        this->field_0x24c = -2;
        this->field_0x250 = 1;
        this->field_0x254 = 5;
        this->field_0x258 = 2;
        this->field_0x25c = 22;
        this->field_0x260 = 1;
        this->field_0x264 = 421;
        this->field_0x268 = 11;
        this->field_0x26c = 92;
        this->field_0x270 = 98;
        this->field_0x274 = 136;
        this->field_0x278 = 421;
        this->field_0x27c = 28;
        this->field_0x280 = 0;
        this->scrollBarInset = 1;
        this->scrollBarHandle = 0;
        this->rewardBoxWidth = 140;
        this->rewardBoxY = 77;
        this->rewardBoxY2 = 132;
    } else if (Globals::n9) {
        this->field_0x1e4 = 6;
        this->field_0x1e8 = 1;
        this->field_0x1ec = 34;
        this->field_0x1fc = 4;
        this->field_0x204 = 16;
        this->field_0x208 = 7;
        this->field_0x20c = 7;
        this->field_0x210 = 6;
        this->field_0x214 = -2;
        this->field_0x218 = 7;
        this->orbitInfoTextX = 10;
        this->orbitInfoStationY = 7;
        this->orbitInfoSystemY = 30;
        this->orbitInfoSecurityY = 52;
        this->quickMenuHeaderLift = 4;
        this->quickMenuIconGap = 2;
        this->field_0x234 = 2;
        this->field_0x238_blk16.a = 90;
        this->field_0x238_blk16.b = 27;
        this->field_0x238_blk16.c = 9;
        this->field_0x238_blk16.d = 5;
        this->field_0x248 = 306;
        this->field_0x24c = -1;
        this->field_0x250 = 2;
        this->field_0x254 = 7;
        this->field_0x258 = 4;
        this->field_0x25c = 28;
        this->field_0x260 = 0;
        this->field_0x264 = 450;
        this->field_0x268 = 12;
        this->field_0x26c = 99;
        this->field_0x270 = 105;
        this->field_0x274 = 145;
        this->field_0x278 = 450;
        this->field_0x27c = 30;
        this->field_0x280 = 4;
        this->scrollBarInset = 2;
        this->scrollBarHandle = 1;
        this->rewardBoxWidth = 112;
        this->rewardBoxY = 57;
        this->rewardBoxY2 = 111;
        this->rewardBoxHeight = 352;
    } else {
        this->field_0x1e4 = Globals::retinaDisplay ? 8 : 4;
        this->field_0x1e8 = Globals::retinaDisplay ? 1 : 0;
        this->field_0x1ec = Globals::retinaDisplay ? 38 : 19;
        this->field_0x1fc = Globals::retinaDisplay ? 4 : 2;
        this->field_0x204 = Globals::retinaDisplay ? 18 : 9;
        this->field_0x208 = Globals::retinaDisplay ? 8 : 4;
        this->field_0x20c = Globals::retinaDisplay ? 10 : 5;
        this->field_0x210 = Globals::retinaDisplay ? 8 : 4;
        this->field_0x214 = Globals::retinaDisplay ? -2 : 1;
        this->field_0x218 = Globals::retinaDisplay ? 10 : 5;
        this->orbitInfoTextX = Globals::retinaDisplay ? 12 : 6;
        this->orbitInfoStationY = Globals::retinaDisplay ? 10 : 5;
        this->orbitInfoSystemY = Globals::retinaDisplay ? 40 : 20;
        this->orbitInfoSecurityY = Globals::retinaDisplay ? 70 : 35;
        this->quickMenuHeaderLift = Globals::retinaDisplay ? 4 : 2;
        this->quickMenuIconGap = Globals::retinaDisplay ? 2 : 1;
        this->field_0x234 = Globals::retinaDisplay ? 2 : 1;
        this->field_0x238_blk16.a = Globals::retinaDisplay ? 100 : 50;
        this->field_0x238_blk16.b = Globals::retinaDisplay ? 30 : 15;
        this->field_0x238_blk16.c = Globals::retinaDisplay ? 10 : 5;
        this->field_0x238_blk16.d = Globals::retinaDisplay ? 6 : 3;
        this->field_0x248 = Globals::retinaDisplay ? 340 : 170;
        this->field_0x24c = Globals::retinaDisplay ? -2 : 0;
        this->field_0x250 = Globals::retinaDisplay ? 2 : 1;
        this->field_0x254 = Globals::retinaDisplay ? 8 : 4;
        this->field_0x258 = Globals::retinaDisplay ? 4 : 2;
        this->field_0x25c = Globals::retinaDisplay ? 32 : 16;
        this->field_0x260 = Globals::retinaDisplay ? 0 : 1;
        this->field_0x264 = Globals::retinaDisplay ? 600 : 300;
        this->field_0x268 = Globals::retinaDisplay ? 16 : 8;
        this->field_0x26c = Globals::retinaDisplay ? 132 : 66;
        this->field_0x270 = Globals::retinaDisplay ? 140 : 70;
        this->field_0x274 = Globals::retinaDisplay ? 194 : 97;
        this->field_0x278 = Globals::retinaDisplay ? 600 : 300;
        this->field_0x27c = Globals::retinaDisplay ? 40 : 20;
        this->field_0x280 = Globals::retinaDisplay ? 5 : 0;
        this->scrollBarInset = Globals::retinaDisplay ? 2 : 1;
        this->scrollBarHandle = Globals::retinaDisplay ? 1 : 0;
        this->rewardBoxWidth = Globals::iPadLarge ? 200 : (Globals::iPad ? 100 : (Globals::retinaDisplay ? 150 : 75));
        this->rewardBoxY = Globals::iPadLarge ? 110 : (Globals::iPad ? 55 : (Globals::retinaDisplay ? 76 : 38));
        this->rewardBoxY2 = (Globals::iPadLarge ? (Globals::retinaDisplay ? 254 : 127) : (Globals::iPad ? (Globals::retinaDisplay ? 144 : 72) : (Globals::retinaDisplay ? 186 : 55)));
    }
    this->rewardBoxHeight = Globals::n9 ? 352 : (Globals::retinaDisplay ? 392 : 196);
    this->rewardBoxX = Globals::n9 ? 75 : (Globals::retinaDisplay ? 100 : 50);
    this->footerButtonOffset = Globals::iPadHD ? 4 : (Globals::n9 ? 5 : (Globals::retinaDisplay ? 6 : 3));
    this->field_0x2bc = Globals::iPadHD ? 2 : (Globals::n9 ? 3 : 2);
    this->field_0x1f0 = Globals::iPadHD ? 26 : (Globals::n9 ? 34 : (Globals::retinaDisplay ? 38 : 19));
    this->field_0x1f4 = (Globals::iPadHD ? 22 : (Globals::iPad ? (Globals::retinaDisplay ? 32 : 16) : (Globals::retinaDisplay ? 4 : 2)));
    this->field_0x200 = Globals::iPadHD ? -18 : (Globals::n9 ? -26 : (Globals::retinaDisplay ? -26 : -13));
    this->field_0x1f8 = (Globals::iPadHD ? 2 : (Globals::iPad ? (Globals::retinaDisplay ? 4 : 2) : 2));
    this->field_0x284_sliderSlot5Enabled = Globals::n9 ? 0 : 1;
    this->field_0x285 = 1;
    this->field_0x286 = Globals::n9 ? 0 : 1;
    this->field_0x288 = Globals::n9 ? 20 : 0;
    this->field_0x28c = Globals::n9 ? -19 : 0;
    this->field_0x290 = Globals::n9 ? 4 : (Globals::retinaDisplay ? 7 : 3);
    this->field_0x2f4 = Globals::iPadHD ? 112 : (Globals::iPadLarge ? 160 : (Globals::iPad ? 80 : (Globals::n9 ? 45 : (Globals::retinaDisplay ? 60 : 30))));
    this->field_0x2f8 = Globals::iPadHD ? 281.25f : (Globals::iPadLarge ? 400.0f : (Globals::iPad ? 200.0f : (Globals::n9 ? 150.0f : (Globals::retinaDisplay ? 200.0f : 100.0f))));
    this->field_0x2fc = Globals::iPadHD ? 50 : (Globals::iPadLarge ? 72 : 36);
    this->field_0x300 = Globals::iPadHD ? 281 : (Globals::iPadLarge ? 400 : 200);
    this->field_0x304 = Globals::iPadHD ? 492 : (Globals::iPadLarge ? 700 : 350);
    this->choiceWindowTitleHeight = Globals::iPadHD ? 351 : (Globals::iPadLarge ? 500 : 250);
    this->field_0x30c = Globals::iPadLarge ? 6 : (Globals::iPad ? 3 : (Globals::n9 ? 4 : (Globals::retinaDisplay ? 7 : 3)));
    this->field_0x310 = Globals::iPadLarge ? -24 : (Globals::iPad ? -12 : (Globals::n9 ? -8 : (Globals::retinaDisplay ? -9 : -5)));
    this->field_0x314 = Globals::iPadLarge ? -10 : (Globals::iPad ? -5 : (Globals::n9 ? -8 : (Globals::retinaDisplay ? -8 : -5)));
    this->field_0x318 = Globals::iPadLarge ? 8 : (Globals::iPad ? 4 : 0);
    this->helpWindowX = Globals::iPadHD ? 331 : (Globals::iPadLarge ? 472 : (Globals::n9 ? 236 : (Globals::iPad ? 236 : (Globals::retinaDisplay ? 236 : 118))));
    this->helpWindowY = Globals::iPadHD ? 202 : (Globals::iPadLarge ? 288 : (Globals::iPad ? 144 : (Globals::n9 ? 150 : (Globals::retinaDisplay ? 150 : 76))));
}
```

### 4.1 Why the config-major body scores lower

The original is config-major, but its code generation is extremely sensitive
to the exact expression types and to clang's SLP/scheduling decisions.  The
generated body uses the final integer/float values, so clang vectorises more
aggressively (100 `vld1.64` groups versus the original 86) and schedules the
literal loads differently.  The current field-major baseline preserves the
original `* 1.40625f`, `(int)(float)` and `(float)` expression shapes, which
happens to reproduce more of the original instruction stream even though the
source shape is less faithful.  The baseline is a local optimum: 5,500 random
single moves/swaps and all targeted ascending/move variants measured in the
wave-119 pass failed to beat 48.963317384370 % at the mandatory 7072 B size.

Measured config-major variants (all semantically exact, all `bad combos 0`):

| order | size | ratio % |
|---|---:|---:|
| iOS source order (`ios`) | 6672 | **27.729913844921** |
| current baseline order (`cur`) | 6608 | 21.217674608023 |
| original trace order (`trace`) | 6564 | 19.643580396922 |

The 7072-B size is mandatory: 7068 B drops `byte_exact` from 4505 to 4462;
7076 B collapses to 2454.  The baseline is the only variant found that keeps
7072 B and the full gate.

## 5. Generator corrections discovered in this pass

The previous config-major generator (`_work/reconstructions/layout_ctor/`)
had three bugs that made its body semantically incomplete:

1. **`Blk16` alias.**  The header parser overwrote `names[0x238]` with
   `field_0x238_blk16.a`, so the baseline-source lookup for
   `field_0x238_blk16` failed and the four `Blk16` stores were omitted.
   Fix: keep the base alias for lookup while emitting the `.a/.b/.c/.d`
   names.
2. **`DEF['COLD']`.**  The default/iPad `field_0x130`/`field_0x134` stores
   live in the outlined cold block (`0xd2802..0xd2827`), not in `ELSE`.
   They must be emitted in the non-HD common path.
3. **`n9` common1.**  For `n9=1`, `field_0x130`/`field_0x134` are in
   `COMMON1` (values `60`/`120`), not in the `N91` block.  Emitting them only
   in the `else` arm leaves the `n9` arm with uninitialised fields.

After these fixes the generated body matches the original for all 32 flag
combinations.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7102
OBJ=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OBJ -d --start-address=0xd1028 --stop-address=0xd2bd0 \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so > /dev/shm/w7102/orig.dis
$OBJ -d --start-address=0xd1028 --stop-address=0xd2bd0 \
  cmake-build-match/libgof2hdaa.so > /dev/shm/w7102/ours.dis

python3 /dev/shm/w7102/score_exact.py          # unrounded C1/C2 ratio
python3 /dev/shm/w7102/compare_combos.py cmake-build-match/libgof2hdaa.so
```

Scratch artifacts (not committed): `/dev/shm/w7102/gen_hybrid2.py` (corrected
config-major generator), `/dev/shm/w7102/gen_ios4.cpp` (the proposed body),
`/dev/shm/w7102/Layout.hybrid_ios4.full.cpp` (full file), and the disassembly
dumps `orig.dis` / `ours_ios4.dis`.
