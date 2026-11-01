# `g_ips_shipTypeMap` is 64 entries, not 32 (w3448)

Status: research note. Branch `wave87-w3448`, based on master `78e994de`.
All numbers were measured in this worktree against
`../_work/bins/android_2.0.16_libgof2hdaa.so`. No source change is landed by this note:
every A/B below is a controlled rebuild that was reverted before the commit.

## 1. Baseline

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
```

| metric | value |
|---|---|
| byte_exact | **4253** |
| linked_exact | 4496 |
| compared | 4524 |
| stub_zero_size / missing / wrong_type / extra | 0 / 0 / 0 / 0 |
| data exact | 843 / 843 |
| imports | 207 |
| lint | CLEAN |
| drift | 0 field(s) across 0 class(es) |
| `.text` | 0x1337c8 @ 0x69440 |
| `.rodata` | 0x41b67 @ 0x1c9e10 |
| `.data.rel.ro` | 0x2fe8 @ 0x20cbb0 |
| `.data` | 0x296c @ 0x214000 |
| `.bss` | 0x69ac @ 0x216970 |

The current tree declares the table as `static const uint32_t g_ips_shipTypeMap[32]`
at `src/game/world/Level.cpp:751`.

## 2. The original table is 64 ints

The table is at VMA `0x1fc3d0` in the original `.rodata` and occupies exactly `0x100`
bytes, ending where `g_ips_skyColors` begins at `0x1fc4d0`:

```sh
od -Ax -tx4 -v -j 0x1fc3d0 -N 0x100 ../_work/bins/android_2.0.16_libgof2hdaa.so
```

The 64 words are:

```
3,0,8,3,2,0,3,0,9,1,0,8,2,0,0,0,
2,0,2,3,3,2,0,8,8,8,0,0,0,8,3,2,
8,0,0,2,0,0,0,1,0,1,1,2,1,3,3,3,
3,1,1,0,8,1,1,0,3,2,0,0,8,1,3,1
```

The first 32 words are exactly the current source table. The second 32 words are not
padding: they contain live switch values (`1`, `2`, `3`, `8`) that map to different
particle UV rectangles. The switch in `Level::initParticleSystems()` has cases
`1, 2, 3, 8, 9` and a default; the second half exercises `1, 2, 3, 8`.

A direct content check against the source table is committed as
`_work/reconstructions/g_ips_shipTypeMap_64.py`:

```sh
python3 _work/reconstructions/g_ips_shipTypeMap_64.py
```

It prints the original 64 words, the current source table, and `match: False` at
HEAD (the source is still 32 entries). It prints `match: True` when the source is
temporarily changed to the 64-entry form.

## 3. Why appending the 32 entries alone is worse

The table is emitted in `Level.cpp`'s `.rodata`, but not at the original address.
In the baseline build the `Level.cpp` `.rodata` block starts at `0x1fc490` and its
first items are:

| offset in `Level.cpp` `.rodata` | symbol | linked VMA |
|---:|---|---:|
| 0x000 | `jumpgateMeshes` | 0x1fc490 |
| 0x040 | `g_ips_shipTypeMap[32]` | 0x1fc4d0 |
| 0x0c0 | `distances` | 0x1fc550 |

The original has `g_ips_shipTypeMap` at `0x1fc3d0` and `g_ips_skyColors` at
`0x1fc4d0`. So the table is `0x100` bytes too late in the baseline, and appending
32 entries shifts every later `.rodata` item by another `0x80`.

Controlled A/B (only the table size changed, then rebuilt and checked):

| experiment | byte | linked | `.rodata` | `.data` | verdict |
|---|---:|---:|---|---|---|
| baseline | 4253 | 4496 | 0x41b67 | 0x296c | — |
| `g_ips_shipTypeMap[64]` (32 appended) | **4234** | 4496 | 0x41be7 (+0x80) | 0x296c | worse |

The table itself is correct; the placement is not.

## 4. `g_ips_skyColors` is 19 entries, not 20

The original `g_ips_skyColors` starts at `0x1fc4d0` and the next 16-byte-aligned
table (`distances`) starts at `0x1fc520`. The 20th word of the current source table
is `0x00000000`, but it is alignment padding, not a colour: a 19-entry array ends at
`0x1fc51c`, and `0x1fc520` is the next 16-byte boundary.

This matters because clang puts a flat `static const uint32_t[20]` whose last element
is zero into `.rodata.str4.4` (mergeable string pool), not regular `.rodata`. With 19
entries it moves to regular `.rodata` and lands at the original address `0x1fc4d0`.

Controlled A/B (only `g_ips_skyColors` changed from `[20]` to `[19]`, trailing zero
removed):

| experiment | byte | linked | `.rodata` | `.data` | verdict |
|---|---:|---:|---|---|---|
| baseline | 4253 | 4496 | 0x41b67 | 0x296c | — |
| `g_ips_skyColors[19]` | 4253 | 4496 | 0x41b67 | 0x296c | neutral (address now exact) |

With 19 entries, `g_ips_skyColors` is at `0x1fc4d0` exactly, but `g_ips_shipTypeMap`
is still at `0x1fc51c` (after `jumpgateMeshes`) and still 32 entries.

## 5. Full `Level.cpp` `.rodata` reorder experiment

The original `Level.cpp` `.rodata` order was reconstructed by searching the original
`.rodata` for each of our table contents. The first tables are:

```
0x1fc3d0  g_ips_shipTypeMap[64]
0x1fc4d0  g_ips_skyColors[19]
0x1fc520  distances[3]
0x1fc54c  g_cg_meshTable
0x1fc8f0  g_cg_soundTable
0x1fcc94  kStationTurrets
0x1fcd74  kAlienStationTurrets
0x1fce34  kAlienOutpostTurrets
0x1fcedc  kBaseTurrets
0x1fcf54  kPirateStationCoords
0x1fcf84  kBlueprintRaces
0x1fcf98  kBlueprintPositions
0x1fcfd4  kBlueprintSpacePoints
0x1fcfe8  createMission local coords
0x1fd078  createCampaignMission local coords
0x1fd200  kCarrierTurrets
...
```

A controlled rebuild was made with:
* `g_ips_shipTypeMap[64]` and `g_ips_skyColors[19]`;
* `initParticleSystems()` moved before `createSpace()` so its statics are first;
* the two declarations swapped so `g_ips_shipTypeMap` precedes `g_ips_skyColors`;
* `createGun()` moved before `createMission()`;
* `createScene()`, `createRadioMessage()`, `createSpace()` moved to the end in that
  order.

With `kMissionCase` moved out of `Status.cpp` `.rodata` into `.data` (as
`static volatile unsigned char`), the first two tables land exactly:

```
ours:     0x1fc3d0  g_ips_shipTypeMap[64]
          0x1fc4d0  g_ips_skyColors[19]
original: 0x1fc3d0  g_ips_shipTypeMap[64]
          0x1fc4d0  g_ips_skyColors[19]
```

Measured results:

| experiment | byte | linked | `.rodata` | `.data` | gains / losses |
|---|---:|---:|---|---|---|
| baseline | 4253 | 4496 | 0x41b67 | 0x296c | — |
| full reorder + `kMissionCase` in `.data` | 4236 | 4496 | 0x41b27 (-0x40) | 0x2a2c (+0xc0) | +3 / -20 |
| full reorder + `kMissionCase` appended to `.rodata` via `#pragma clang section` + section-order entry | 4237 | 4496 | 0x41be5 (+0x7e) | 0x296c | +3 / -19 |
| full reorder + `kMissionCase` removed via inline string literal | 4234 | 4496 | 0x41be7 (+0x80) | 0x296c | +0 / -19 |

The three gains are `Level::assignGuns`, `Level::createGun`, and
`Status::getGammaRayDamagePerSecond` (all `pct==100` before, byte-exact after).
The losses are 19–20 functions in other TUs (`SpaceLounge`, `DialogueWindow`,
`TouchButton`, `Hud`, `Ship`, `BeamGun`, and zlib) whose `.rodata`/`.data` literals
shift when `Status.cpp`'s 190-byte `kMissionCase` table is moved.

The first remaining content difference in `Level.cpp` `.rodata` after the reorder is
`0x1fcc98` (inside `kStationTurrets`), not an ordering difference: everything from
`0x1fc3d0` through `0x1fcc97` matches the original byte-for-byte. The next ordering
mismatch is that our `createMission` local coord tables are emitted before
`kStationTurrets` (offset `0x8c8` vs the original `0xc18`), because they are declared
at the top of `createMission()` before the first `kStationTurrets` use.

## 6. Proven vs hypothesis

Proven (measured):

* The original `g_ips_shipTypeMap` is 64 ints at `0x1fc3d0`, ending exactly at
  `g_ips_skyColors` at `0x1fc4d0`.
* The first 32 words equal the current source table; the second 32 contain live
  switch values (`1,2,3,8`), so the current `[32]` declaration is incomplete.
* Appending the 32 entries alone is a regression: byte 4253 -> 4234, `.rodata`
  +0x80.
* `g_ips_skyColors` is 19 entries + 4 bytes of alignment padding; changing it to
  `[19]` moves it from `.rodata.str4.4` to regular `.rodata` at the original
  address and is gate-neutral.
* The original `Level.cpp` `.rodata` order is `shipTypeMap, skyColors, distances,
  g_cg_meshTable, g_cg_soundTable, kStationTurrets, ...`.
* A full function/declaration reorder reproduces the first two tables exactly and
  gains 3 byte-exact functions, but the 190-byte `kMissionCase` table in
  `Status.cpp` shifts the rest of `.rodata`/`.data` and costs 19–20 functions.
* With the reorder, `Level.cpp` `.rodata` matches the original from `0x1fc3d0`
  through `0x1fcc97`; the first content difference is inside `kStationTurrets`.

Hypothesis (supported, not closed):

* Eliminating `kMissionCase` from the binary (or placing it in a section that does
  not shift the rest of `.rodata`/`.data`) would turn the +3 reorder gain into a
  strict improvement. The 190-byte table is a decompilation aid; the original
  `Status::missionCompleted` uses in-text `tbh` jump tables and has no such
  `.rodata` table.
* The remaining `.rodata` ordering differences (`createMission` coords vs
  `kStationTurrets`, hangar-spot order, `jumpgateMeshes` after the switch tables)
  are source-shape/declaration-order issues, not table-size issues. Matching them
  should make `createSpace`, `createRadioMessage`, `initParticleSystems`, and
  `createCampaignMission` (all `pct==100` non-byte) byte-exact.

## 7. Reproduction commands

```sh
# baseline
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8

# original 64-entry table
od -Ax -tx4 -v -j 0x1fc3d0 -N 0x100 ../_work/bins/android_2.0.16_libgof2hdaa.so

# original table locations for our Level.cpp tables
# (the addresses are listed in §5; the one-off helper parsed readelf -sW and
#  searched each object .rodata* section's bytes in the original .rodata)

# `.rodata` content diff after a relink with -Map
/tmp/w3448_relink_map.sh
python3 _work/reconstructions/rodata_content_diff_map.py \
  --unstripped /tmp/w3448_unstripped.so --map /tmp/w3448_rodata.map
```

## 8. Suggested next pass

1. Land `g_ips_skyColors[19]` (gate-neutral, moves it to the original address).
2. Land `g_ips_shipTypeMap[64]` together with a `Level.cpp` declaration/function
   reorder that puts it first in `Level.cpp` `.rodata`.
3. In the same pass, eliminate or relocate `kMissionCase` so it does not shift the
   rest of `.rodata`/`.data`; the 190-byte table is the only thing standing between
   the +3 reorder gain and a strict improvement.
4. Re-check `kStationTurrets` content at `0x1fcc98`; it is the first remaining
   `.rodata` content mismatch after the reorder.
