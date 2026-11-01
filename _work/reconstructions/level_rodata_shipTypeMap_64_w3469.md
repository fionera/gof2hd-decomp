# Level.cpp `.rodata` block: `shipTypeMap[64]` and the last `-0x10` (w3469)

Status: research note only. Branch `wave87-w3469`, based on master `b0f4540f`.
Original binary: `../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a,
NDK r18b clang 7.0.2, `-Oz`, image base 0x10000; real VMA = Ghidra - 0x10000).

All numbers below were measured in this worktree. Scratch files are in `/tmp`.

## TL;DR

* Baseline is **byte_exact 4408 / linked_exact 4496**, `.rodata` size/base
  exact (`0x41b67 @ 0x1c9e10`). The task brief's `4182/4495` is stale.
* The original `Level.cpp` plain `.rodata` block is **`0x18a4` bytes at
  `0x1fc3d0`** (`0x1fc3d0..0x1fdc74`). Our baseline block is `0x1904` bytes at
  the same start. The `0x60` delta decomposes exactly as
  `-0xc0` (invented `kMissionCase[190]` + its 2-byte pad)
  `+0x80` (`g_ips_shipTypeMap` is 64 ints, not 32)
  `-0x20` (`jumpgateMeshes` is 16 `uint16`, not 32).
* Applying all three Level fixes plus removing `kMissionCase` makes the Level
  block **start- and size-exact** (`0x18a4 @ 0x1fc3d0`) and `.rodata` shrinks
  by exactly `0x60` (`0x41b07`).
* The remaining `-0x10` is **not** Level data. It is the Hud
  `_RACE_IMAGES_BIG` table: the original is a **10-int** plain `.rodata` array
  `{1190,1187,1189,1188,-1,-1,-1,-1,-1,-1}` at `0x203758`, while our tree has
  a 4-int `alignas(16)` `.rodata.cst16` array. Adding the six `-1` entries and
  dropping `alignas(16)` makes the Hud region byte-identical and the whole
  `.rodata` exactly `0x41b67` again.
* With `kMissionCase` removed, `Status::missionCompleted` becomes **1428 B**
  (original 1432 B). The 4-byte `.text` shrink shifts every later function and
  costs 1376 byte-exact functions. That is the only blocker to landing the
  Level block; no experiment below is a strict gate improvement, so this pass
  lands the note only.

## 1. Baseline

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
```

```
verify: byte 4408  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Section sizes/bases (identical in both binaries):

```
.text        0x1337c8 @ 0x69440
.rodata      0x41b67  @ 0x1c9e10
.data.rel.ro 0x2fe8   @ 0x20cbb0
.got         0x430c   @ 0x20fcf4
.data        0x296c   @ 0x214000
.bss         0x69ac   @ 0x216970
```

## 2. Original Level block

The original Level plain `.rodata` is `0x1fc3d0..0x1fdc74` = **6308 B =
`0x18a4`**. The content map is in `rodata_content_map_round3.md` §3 and
`level_rodata_reorder_w3454.md` §3; the head of the block is:

```
0x1fc3d0  g_ips_shipTypeMap[64]        0x100
0x1fc4d0  g_ips_skyColors[19]          0x4c + 4 pad
0x1fc520  createAsteroids distances[3] 0x0c
0x1fc530  createAsteroids distances[3] 0x0c
0x1fc540  createAsteroids distances[3] 0x0c
0x1fc54c  g_cg_meshTable               0x3a4
0x1fc8f0  g_cg_soundTable              0x3a4
0x1fcc94  kStationTurrets              0xe0
...
0x1fdc54  jumpgateMeshes[16]           0x20
0x1fdc74  cst32 merge group            (kAgentMeshes, stageCounts, wantedEntries, ...)
```

The 64-int `g_ips_shipTypeMap` was dumped with:

```sh
od -Ax -tx4 -v -j 0x1fc3d0 -N 0x100 ../_work/bins/android_2.0.16_libgof2hdaa.so
```

First 32 words are already in `src/game/world/Level.cpp`; the second 32 are:

```
8,0,0,2,0,0,0,1,0,1,1,2,1,3,3,3,
3,1,1,0,8,1,1,0,3,2,0,0,8,1,3,1
```

`_work/reconstructions/g_ips_shipTypeMap_64.py` prints `match: False` on the
current 32-entry source and `match: True` once the table is widened.

The 16 `uint16` at `0x1fdc54` are the first 16 values of the current 32-entry
`jumpgateMeshes`; the second 16 values (`14725,0,14726,0,...`) are the separate
`kAgentMeshes[8]` merge constant (`0x1fdc74`), not part of `jumpgateMeshes`:

```sh
od -Ax -tx2 -v -j 0x1fdc54 -N 0x20 ../_work/bins/android_2.0.16_libgof2hdaa.so
# 3a98 3a9a 3a99 3a9b 3a9c 3a9e 3a9d 3a9f
# 3aa0 3aa2 3aa1 3aa3 3aa4 3aa6 3aa5 3aa7
```

## 3. The `0x60` arithmetic (proven)

Baseline `o102_Level.cpp.obj` `.rodata` is `0x1904` at linked `0x1fc3d0`:

```
0x000  g_ips_shipTypeMap[32]   0x80
0x080  g_ips_skyColors[19]     0x4c
0x0d0  distances[3]            0x0c
0x0e0  distances[3]            0x0c
0x0f0  distances[3]            0x0c
0x0fc  jumpgateMeshes[32]      0x40
...
0x1780  kMissionCase[190]      0xbe
0x1840  switch tables          0x9c
0x1904  end
```

Removing `kMissionCase` also removes the 2-byte alignment pad
(`0x1780 + 0xbe -> 0x1840`), so:

```
-0xc0  kMissionCase[190] + pad
+0x80  shipTypeMap 32 -> 64 ints
-0x20  jumpgateMeshes 32 -> 16 uint16
= -0x60
0x1904 - 0x60 = 0x18a4  == original Level plain .rodata
```

This is the central proof that the three Level changes are exactly right and
that there is no missing Level data.

## 4. The last `-0x10` is the Hud `_RACE_IMAGES_BIG` table (proven)

With the three Level fixes + `kMissionCase` removal, `.rodata` is `0x41b07`
(`-0x60`). The `gCreateChar2Table` 13-row fix (`+0x20`) and the LensFlare
`alignas(16)` fix (`+0x30`) recover `0x50`, leaving `-0x10`.

A full `difflib` diff of the two `.rodata` images (after the Level block) shows
exactly one remaining size-bearing difference:

```
insert  ours 0x203758..0x203760 (0x8)   zero padding
delete  orig 0x203768..0x203780 (0x18)  ff ff ff ff ff ff ff ff ff ff ff ff
                                        ff ff ff ff ff ff ff ff ff ff ff ff
```

The original bytes at `0x203758` are:

```sh
od -Ax -tx4 -v -j 0x203758 -N 0x28 ../_work/bins/android_2.0.16_libgof2hdaa.so
# 203758 000004a6 000004a3 000004a5 000004a4
# 203768 ffffffff ffffffff ffffffff ffffffff
# 203778 ffffffff ffffffff
```

i.e. a **10-int** array `{1190,1187,1189,1188,-1,-1,-1,-1,-1,-1}`. Our
`src/game/ui/Hud.cpp` currently has:

```cpp
alignas(16) static const int g_Hud_raceBadge[4] = {1190, 1187, 1189, 1188};
```

The iOS source dump names this symbol `_RACE_IMAGES_BIG` (the "BIG" suffix is
consistent with a table larger than four), and `Hud::Hud` indexes it with
`SolarSystem::getRace()`. The `alignas(16)` makes ours a `.rodata.cst16` merge
constant placed at `0x203760` (16-aligned), leaving 8 bytes of pad; the
original is a plain `.rodata` array at `0x203758` (8-aligned) directly before
`Hud`'s `_SECURITY_COLORS_RGB` at `0x203780`.

Controlled change:

```cpp
static const int g_Hud_raceBadge[10] = {1190, 1187, 1189, 1188, -1, -1, -1, -1, -1, -1};
```

Measured on the B3 tree (`kMissionCase` removed, Level/ImageFactory/LensFlare
fixes in place):

| tree | byte | linked | `.rodata` |
|---|---:|---:|---:|
| baseline | 4408 | 4496 | `0x41b67` |
| B3 (Level+ImageFactory+LensFlare) | 3032 | 4495 | `0x41b57` |
| B3 + Hud 10-int table | 3032 | 4495 | **`0x41b67`** |

With the Hud fix the entire `.rodata` is size-exact and the Hud region
(`0x203758..0x203780`) is byte-identical to the original. The Hud fix alone on
the baseline is **not** a win (`.rodata` grows to `0x41b77`, byte 4408 -> 4388);
it is only the compensating `+0x10` for the Level `-0x60`.

## 5. The blocker: `Status::missionCompleted` is 4 bytes short

`kMissionCase` lives in `Level.cpp`, so removing it requires
`Status::missionCompleted` to switch on `m->getType()` directly (the original
does `sub.w r1, r0, #150` / `tbh` / `sub.w r1, r0, #8` / `tbh`; there is no
byte-table load). The direct switch is structurally right but emits **1428 B**
instead of the original **1432 B**:

```sh
nm /tmp/w3469_b3hud_unstripped.so | grep missionCompleted   # 0xabf39
```

`report.json` row: `osize 1432`, `usize 1428`. Because `.text` is
`-ffunction-sections` + section-ordered, the 4-byte shrink shifts every later
function by `-4` and costs **1376** byte-exact functions (the `lost` set starts
with the early `Java_*`/SHA/Array helpers and includes every later TU).

The 4 bytes are not missing data: the two `tbh` tables are the same size
(40 entries for types 150..189, 7 entries for types 8..14). The difference is
case-body order. The original lays the bodies out in source order:

```
158,162,166,170,184,189,156,157,160,163,164,165,168,169,171,172
```

(`od`-verified from the `tbh` targets at `0xabfe6`). Clang 7 `-Oz` in this tree
emits them in **ascending case-value order**:

```
156,157,158,160,162,163,164,165,166,168,169,170,171,172,184,189
```

Reversing the source case order does not change the emitted order (tested), so
the backend is sorting; the different body positions then change `b.n` vs
`b.w` encodings and account for the 4 bytes. Making `missionCompleted` 1432 B
again (without adding a padding anchor) is the remaining work for a landable
Level pass.

## 6. Measured experiment table

All rows are full `cmake --build cmake-build-match --target check -j8` runs; no
`--update-baseline`.

| # | change | byte | linked | `.rodata` | first diff |
|---|---|---:|---:|---:|---:|
| 0 | baseline | **4408** | 4496 | `0x41b67` | `0x1fb7f8` |
| A | `g_ips_shipTypeMap[64]` alone | 4388 | 4496 | `0x41be7` | `0x1fb7f8` |
| B1 | direct `missionCompleted` switch + `shipTypeMap[64]` + `jumpgateMeshes[16] volatile` + remove `kMissionCase` | 3032 | 4495 | `0x41b07` | `0x1fb7f8` |
| B2 | B1 + `gCreateChar2Table` 13 rows | 3032 | 4495 | `0x41b27` | `0x1fb7f8` |
| B3 | B2 + LensFlare `alignas(16)` | 3032 | 4495 | `0x41b57` | `0x1fb7f8` |
| B3H | B3 + Hud `_RACE_IMAGES_BIG[10]` | 3032 | 4495 | **`0x41b67`** | `0x1fb7f8` |
| H | baseline + Hud `_RACE_IMAGES_BIG[10]` | 4388 | 4496 | `0x41b77` | `0x1fb7f8` |

`B3H` has every ELF section size/base exact and the Level block
start/size-exact, but the `.text` shift from `missionCompleted` keeps the gate
below baseline.

## 7. Level block content order (still wrong)

Even at `0x18a4 @ 0x1fc3d0`, the Level block's contents are reordered relative
to the original. A `difflib` diff of `0x1fc3d0..0x1fdc74` on the B3H tree gives
several large moves (`ORIG_EXTRA`/`OURS_EXTRA` pairs) and no size difference, e.g.
`g_cg_meshTable`/`g_cg_soundTable` are emitted after the `createCampaignMission`
locals in our tree but immediately after the asteroid distances in the
original. Fixing that order is a separate pass; it changes the literal-pool
addresses used by Level functions but not the block size.

## 8. Proven vs hypothesis

**Proven**

* Original Level plain `.rodata` is `0x18a4` at `0x1fc3d0`; the `0x60` delta is
  exactly `-0xc0 + 0x80 - 0x20` (kMissionCase, shipTypeMap 64, jumpgate 16).
* `g_ips_shipTypeMap` is 64 ints; the second 32 are the values quoted above.
* `jumpgateMeshes` is 16 `uint16`; the following 16 `uint16` are `kAgentMeshes`.
* The original Hud `_RACE_IMAGES_BIG` is a 10-int plain `.rodata` array
  `{1190,1187,1189,1188,-1,-1,-1,-1,-1,-1}`; ours is a 4-int `.rodata.cst16`
  with `alignas(16)`. The 10-int/no-alignas form makes `.rodata` size-exact and
  the Hud region byte-identical.
* `Status::missionCompleted` must switch on `m->getType()` directly; the direct
  switch is 1428 B vs the original 1432 B because the case bodies are emitted
  in ascending value order instead of the original source order.

**Hypothesis / next pass**

* Reordering the `missionCompleted` case bodies to the original order (or
  otherwise recovering the 4 bytes) should make `B3H` a large strict gain: the
  1376 `.text`-shift casualties return and `.rodata` is already exact.
* The Level block's internal table order is the next correctness item after
  that; the target order is the table in `level_rodata_reorder_w3454.md` §3.
