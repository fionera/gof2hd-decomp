# Level.cpp `.rodata` reorder and the `kMissionCase` blocker (w3454)

Status: research note + one landed source change. Branch `wave87-w3454`, based on
master `9b45fb42` (`ModStation data pass`, byte 4255 / linked 4496).
Original binary: `../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a,
NDK r18b clang 7.0.2, `-Oz`, image base 0x10000; real VMA = Ghidra - 0x10000).

All numbers below were measured in this worktree. Scratch files live in `/tmp/w3454`.

## TL;DR

`Status.cpp`'s invented `kMissionCase[190]` table sat between `Status` and `Level`
in `.rodata`, pushing `Level.cpp`'s block from the original `0x1fc3d0` to
`0x1fc490` (+0xc0).  Moving the table to the **end of `Level.cpp`'s `.rodata`**
(the original Level block is exactly `0xc0` larger than our reconstruction) puts
`Status` back at `0xa0` bytes, `Level` back at `0x1fc3d0`, and leaves the merge
constants and every later section at their existing addresses.

Measured gate: **byte_exact 4255 -> 4256, linked_exact 4496 -> 4496**.  The only
changed report row is `Status::getGammaRayDamagePerSecond` (`byte False -> True`).
Every ELF section stays size- and base-exact (`.rodata 0x41b67 @ 0x1c9e10`,
`.got 0x430c @ 0x20fcf4`, `.text 0x1337c8 @ 0x69440`).

## 1. Baseline

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
```

At `9b45fb42`:

```
verify: byte 4255  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
DRIFT: 0 field(s) across 0 class(es)
```

Section sizes/bases (both binaries):

```
.text        0x1337c8 @ 0x69440
.rodata      0x41b67  @ 0x1c9e10
.data.rel.ro 0x2fe8   @ 0x20cbb0
.got         0x430c   @ 0x20fcf4
.data        0x296c   @ 0x214000
.bss         0x69ac   @ 0x216970
```

Baseline `.rodata` content diff (scratch relink, see §2):

```
differing bytes 13266 / 269159 (4.93%)
first VMA 0x1fc390
NOT-EXACT _ZL12kMissionCase  ours 0x1fc390 size 190
```

## 2. Scratch relink with a gold `-Map`

The shipped `.so` is stripped, so the input-section owner is recovered from a
scratch gold relink with `-Map` and without the final `strip`:

```sh
ninja -C cmake-build-match -t commands libgof2hdaa.so | tail -1 > /tmp/w3454/link_cmd_raw.txt
# replace '-o .../cmake-build-match/libgof2hdaa.so'
# with '-Map=/tmp/w3454/rodata.map -o /tmp/w3454/unstripped.so'
# and remove the trailing '&& ... arm-linux-androideabi-strip --strip-all ...'
/tmp/w3454/relink_map.sh

python3 _work/reconstructions/rodata_content_diff_map.py \
  --unstripped /tmp/w3454/unstripped.so --map /tmp/w3454/rodata.map

python3 _work/reconstructions/rodata_content_map_round2.py \
  --map /tmp/w3454/rodata.map \
  --orig ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so
```

For a candidate link that is not the CMake output, verify directly:

```sh
python3 tools/verify/verify.py \
  --build-dir /tmp/w3454/verify_candidate \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3454/unstripped.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
```

## 3. Original `Level.cpp` `.rodata` map

The original `Level.cpp` block starts at `0x1fc3d0` and ends at `0x1fdcd4`, where
the merge constants (Level's `.rodata.cst32` content) begin.  That is exactly
`0x1904` bytes.  Searching the original for each reconstructed table gives:

| Level offset | original VMA | symbol | size |
|---:|---:|---|---:|
| 0x000 | 0x1fc3d0 | `g_ips_shipTypeMap[64]` | 0x100 |
| 0x100 | 0x1fc4d0 | `g_ips_skyColors[19]` | 0x4c |
| 0x14c | 0x1fc51c | padding to 16 | 0x04 |
| 0x150 | 0x1fc520 | `createAsteroids` `distances[3]` | 0x2c |
| 0x17c | 0x1fc54c | `g_cg_meshTable` | 0x3a4 |
| 0x520 | 0x1fc8f0 | `g_cg_soundTable` | 0x3a4 |
| 0x8c4 | 0x1fcc94 | `kStationTurrets` | 0xe0 |
| 0x9a4 | 0x1fcd74 | `kAlienStationTurrets` | 0xc0 |
| 0xa64 | 0x1fce34 | `kAlienOutpostTurrets` | 0xa8 |
| 0xb0c | 0x1fcedc | `kBaseTurrets` | 0x78 |
| 0xb84 | 0x1fcf54 | `kPirateStationCoords` | 0x30 |
| 0xbb4 | 0x1fcf84 | `kBlueprintRaces` | 0x14 |
| 0xbc8 | 0x1fcf98 | `kBlueprintPositions` | 0x3c |
| 0xc04 | 0x1fcfd4 | `kBlueprintSpacePoints` | 0x14 |
| 0xc18 | 0x1fcfe8 | `createMission` local coords | 0x90 |
| 0xca8 | 0x1fd078 | `createCampaignMission` local coords | 0x188 |
| 0xe30 | 0x1fd200 | `kCarrierTurrets` | 0x378 |
| 0x11a8 | 0x1fd578 | `kShipHangarHeights` | 0x100 |
| 0x12a8 | 0x1fd678 | `kHangarMeshes` | 0xa0 |
| 0x1348 | 0x1fd718 | `kHangarCapacity` | 0x28 |
| 0x1370 | 0x1fd740 | `kHangarSpotsTerran` | 0x18 |
| 0x1388 | 0x1fd758 | `kHangarSpotsVossk` | 0x6c |
| 0x13f4 | 0x1fd7c4 | `kHangarSpotsNivelian` | 0x24 |
| 0x1418 | 0x1fd7e8 | `kHangarSpotsVoid` | 0x0c |
| 0x1424 | 0x1fd7f4 | `kHangarSpotsKhador` | 0x24 |
| 0x1448 | 0x1fd818 | `kHangarSpotsDefault` | 0x18 |
| 0x1460 | 0x1fd830 | `kBarSeats` | 0x150 |
| 0x15b0 | 0x1fd980 | `kBarMeshes` | 0x78 |
| 0x1628 | 0x1fd9f8 | `stageEntries` | 0xa8 |
| 0x16d0 | 0x1fdaa0 | `bountyCounts` | 0x64 |
| 0x1734 | 0x1fdb04 | `bountyEntries` | 0x28 |
| 0x175c | 0x1fdb2c | `wantedCounts` | 0x128 |
| 0x1884 | 0x1fdc54 | `jumpgateMeshes` | 0x20 |
| 0x18a4 | 0x1fdc74 | `kAgentMeshes` | 0x20 |
| 0x18c4 | 0x1fdc94 | `stageCounts` | 0x20 |
| 0x18e4 | 0x1fdcb4 | `wantedEntries` | 0x20 |
| 0x1904 | 0x1fdcd4 | end / merge constants | |

The 64-word `g_ips_shipTypeMap` was dumped with:

```sh
od -Ax -tx4 -v -j 0x1fc3d0 -N 0x100 ../_work/bins/android_2.0.16_libgof2hdaa.so
```

## 4. The `kMissionCase` blocker

`Status.cpp` emits `kMissionCase` into its `.rodata`:

```
Status .rodata:
  0x00 kMedalCampaignMissions  12
  0x0c g_levelXPTable          84
  0x60 kMissionCase           190   <- invented, not in the original
  0x120 getGammaRay table      20
  0x140 getGammaRay table      20
  size 0x154
```

The original `Status` block is only `0xa0`:

```
0x1fc330 kMedalCampaignMissions  12
0x1fc33c g_levelXPTable          84
0x1fc390 getGammaRay table       32
0x1fc3b0 getGammaRay table       32
0x1fc3d0 Level.cpp .rodata
```

`Status::missionCompleted` in the original uses two in-text `tbh` tables
(`0xabfda`, 40 entries, types 150..189; `0xac046`, 7 entries, types 8..14) and
contains no byte-table load.  The `kMissionCase` table is a reconstruction device
that has no verbatim copy anywhere in the original `.rodata`.

Because `Status.cpp` is linked before `Level.cpp`, the table pushes `Level` to
`0x1fc490` (+0xc0) and every later `.rodata` owner with it.

## 5. Experiments

All `check` rows are full `cmake --build cmake-build-match --target check -j8`
runs.  `linked_exact` is 4496 in every row.  "Level @" is the linked VMA of
`o102_Level.cpp.obj`'s `.rodata`.

| experiment | byte | `.rodata` | Level @ | verdict |
|---|---:|---:|---:|---|
| baseline (`9b45fb42`) | 4255 | 0x41b67 | 0x1fc490 | — |
| neutral reorder: `skyColors[19]`, `shipTypeMap` before sky, `initParticleSystems`/`createAsteroids` before `createSpace` | 4255 | 0x41b67 | 0x1fc490 | neutral; correct source order |
| `kMissionCase` in its own `.rodata.kMissionCase` section, appended to `section_order.txt` | 4236 | **0x41b65** | 0x1fc3d0 | size mismatch (-2), tail shifts; worse |
| `kMissionCase` in a separate TU linked after `Level.cpp` | 1713 | 0x41b67 | 0x1fc3d0 | new object changes PLT/GOT input order; worse |
| separate TU + `#pragma GCC visibility push(hidden)` on both declarations | (not re-run) | — | — | GOT slot is removed by the visibility pragma; see next row |
| `kMissionCase` moved to the end of `Level.cpp`'s `.rodata`, hidden `extern const` | **4256** | **0x41b67** | **0x1fc3d0** | **strict gain** |
| full reorder (`shipTypeMap[64]`, `createGun` before `createMission`, `createScene` before `createRadioMessage`, `createSpace` to EOF) + `kMissionCase` at end of Level | 4238 | 0x41be7 | 0x1fc3d0 | +0x80 tail shift; worse |
| full reorder but `shipTypeMap[32]` + `kMissionCase` at end of Level | 4256 | 0x41b67 | 0x1fc3d0 | neutral vs the landed change |
| full reorder + `shipTypeMap[64]` + `kMissionCase` in `.data` (w3448) | 4236 | 0x41b27 | 0x1fc3d0 | `.data` +0xc0; worse |

### 5.1 Why the separate-TU experiment collapsed to 1713

A new object adds a `.text` input section and changes the linker's input order,
which changes gold's PLT slot assignment for every later object.  The first
attempt also declared the symbol without hidden visibility in `Status.cpp`; the
compiler then emitted a GOT-relative load and `.got` grew from `0x430c` to
`0x4310`, shifting every GOT-relative delta.  Both effects are visible in the
report: 2542 functions with `pct == 100.0` but `byte == False`.

The landed solution avoids both: the table stays in the existing `Level.cpp`
object (no new input section / PLT renumbering), and both the definition and the
extern declaration are wrapped in `#pragma GCC visibility push(hidden)` so the
compiler uses a direct PC-relative reference and the symbol stays local
(`kMissionCase` is `LOCAL HIDDEN` in the final `.so`, not in `.dynsym`).

### 5.2 Landed layout

```
Status .rodata   0x1fc330  0x94   (kMedal 12, g_levelXPTable 84, gamma tables 40)
Level  .rodata   0x1fc3d0  0x1904 (32-entry shipTypeMap + ... + kMissionCase at 0x1780)
merge constants  0x1fdcd4  0xa0   (unchanged)
StarMap .rodata  0x1fdd74  0x22c  (unchanged)
Globals .rodata  0x1fe310  0x37d0 (unchanged)
```

`Status` is now byte-identical to the original through `0x1fc3d0`, and
`getGammaRayDamagePerSecond` becomes byte-exact because its two switch tables are
back at `0x1fc390` / `0x1fc3b0`.  The only report row that changes is that
function; no function regresses.

## 6. Proven vs hypothesis

**Proven**

* The original `Level.cpp` `.rodata` is `0x1904` bytes (`0x1fc3d0..0x1fdcd4`),
  and the original `Status.cpp` block is `0xa0` bytes (`0x1fc330..0x1fc3d0`).
* `kMissionCase` is the only `.rodata` symbol with no verbatim copy in the
  original; the original `missionCompleted` uses in-text `tbh` tables.
* Moving `kMissionCase` to the end of `Level.cpp`'s `.rodata` is a strict
  improvement (4255 -> 4256) with every section size/base unchanged.
* Hidden visibility on both sides is required: without it, `.got` grows by 4
  bytes and the whole PLT/GOT layout shifts (1713 byte-exact).
* The 64-entry `g_ips_shipTypeMap` is real, but appending it now makes Level
  `.rodata` `0x1984` and shifts the tail by `+0x80`; that loses more functions
  than it gains on both the landed neutral tree (4236 vs 4256) and the
  full-reorder tree (4238 vs 4256).

**Hypothesis / next pass**

* The remaining Level `.rodata` differences are table order and table size, not
  missing bytes at the front.  The table order matches the original through
  `g_cg_soundTable` (Level offset `0x8c4`) once the 64-entry table and the
  `createGun`-before-`createMission` move are combined; the tail shift is what
  costs the gate.
* To land the 64-entry table without the `+0x80` shift, the missing `0x40` bytes
  of original Level data (or a smaller `kMissionCase` placement) must be
  reconstructed.  Candidate gaps from the map: `kCarrierTurrets` is `0x150` in
  our tree but `0x378` in the original, and our `createMission` locals are
  emitted before `kStationTurrets` while the original emits them at `0xc18`.
* The clean long-term fix is to replace `kMissionCase` with the original in-text
  `tbh` switch in `Status::missionCompleted`; that removes the 190-byte table
  entirely and lets the 64-entry table occupy its natural `0x100` bytes.
