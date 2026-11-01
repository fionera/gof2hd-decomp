# `.rodata` content map, round 3 (w3450)

Status: research note only (no source/flag change landed; no experiment was a strict
gate improvement). Branch `wave87-w3450`, based on master `cfbd140e`.
Original binary: `../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a,
NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

## TL;DR

* Baseline is **byte_exact 4254 / linked_exact 4496**, `.rodata` size/base exact
  (`0x41b67 @ 0x1c9e10`). The task brief's `4182/4495` is stale.
* The first `.rodata` difference is still VMA `0x1fc390` (the 190-byte
  `Status::kMissionCase` table). **13,266** of 269,159 bytes differ (4.93%),
  not the 13,308 quoted by `docs/IDENTICALITY.md` and round 2 (see §8).
* The original `Status` `.rodata` block is exactly `0x1fc330..0x1fc3d0` (`0xa0`):
  `kMedalCampaignMissions` (12 B), `g_levelXPTable` (0x54 B), then the two
  `getGammaRayDamagePerSecond` switch tables at `0x1fc390`/`0x1fc3b0`.
  `Level.cpp` plain `.rodata` starts at `0x1fc3d0`.
* The original `Level.cpp` plain `.rodata` is `0x1fc3d0..0x1fdc74` = **6308 B**
  (ours is 6132 B at `0x1fc490`). The 176-byte delta decomposes exactly as
  `+128` (`shipTypeMap` is 64 ints, not 32) `+80` (`skyColors` is in plain
  `.rodata`, not `.rodata.str4.4`) `-32` (`jumpgateMeshes` is 16 uint16, not 32).
* A controlled rebuild with exactly those three Level fixes plus the
  `kMissionCase` removal makes the `Status` block byte-identical and moves
  `Level` to `0x1fc3d0`, `StarMap` to `0x1fdd14`, `ModStation` to `0x1fdf40`
  — but the total `.rodata` shrinks to `0x41b07` (-0x60) and byte_exact falls
  to 2942, so it is **not landable alone**. The remaining front-to-tail shift
  is a separate `MenuTouchWindow`-region issue (§7).
* `kPirateStationLoot` is **not** Level data: the original `PlayerFixedObject.cpp`
  TU owns `_PIRATE_STATION_ITEMS_AND_AMOUNTS`; the original `Level.cpp` owns
  `_PIRATE_STATION_POSITIONS` (= our `kPirateStationCoords`). This corrects the
  round-2 handoff hypothesis.

## 1. Baseline

```
cmake --preset default
cmake --build cmake-build-match --target check -j8
```

```
compared 4524  avg 99.89%  byte_exact 4254  linked_exact 4496  fuzzy_perfect 4496
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
verify: byte 4254  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`lint`: `CLEAN ✓`; `drift`: `0 field(s) across 0 class(es)`.

Section sizes/bases (identical in both binaries):

```
.text        0x1337c8 @ 0x69440
.ARM.exidx   0x6f70   @ 0x19cc08
.ARM.extab   0x26298  @ 0x1a3b78
.rodata      0x41b67  @ 0x1c9e10
.data.rel.ro 0x2fe8   @ 0x20cbb0
.got         0x430c   @ 0x20fcf4
.data        0x296c   @ 0x214000
.bss         0x69ac   @ 0x216970
```

## 2. Method

The shipped `.so` is stripped, so input-section ownership is recovered from a
scratch gold relink with `-Map` and without the final `strip`:

```sh
ninja -C cmake-build-match -t commands libgof2hdaa.so | grep 'ld.gold -shared' \
  > /tmp/w3450_linkcmd.txt
# replace the output path with
#   -Map=/tmp/w3450_base.map -o /tmp/w3450_base_unstripped.so
# and drop the trailing '&& ... arm-linux-androideabi-strip --strip-all'
# (the scratch script used for this note is /tmp/w3450_relink_base.sh)
```

Then:

```sh
python3 _work/reconstructions/rodata_content_diff_map.py \
  --orig ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --unstripped /tmp/w3450_base_unstripped.so \
  --map /tmp/w3450_base.map

python3 _work/reconstructions/rodata_content_map_round2.py \
  --map /tmp/w3450_base.map \
  --orig ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so
```

Baseline result:

```
differing bytes 13266 / 269159 (4.93%)
runs 5150  first VMA 0x1fc390  last VMA 0x20377f
region 0x1fc390..0x203780  words 7420
common 7364  only-orig 56  only-ours 56
align: equal 6602  delete 554  insert 708  replace 45 (unequal 45); moved 762
differing symbols 173; no verbatim copy in original 1
  NOT-EXACT _ZL12kMissionCase  ours 0x1fc390 size 190
```

Top owners (full table is printed by the script):

| differing bytes | section size | owner | ours | first original occurrence |
|---:|---:|---|---:|---:|
| 4684 | 6132 | `o102_Level.cpp.obj` | `0x1fc490` | NONE |
| 4519 | 14288 | `o115_Globals.cpp.obj` | `0x1fe310` | `0x1fe2a0` |
| 808 | 1272 | `o137_StarSystem.cpp.obj` | `0x202270` | `0x202260` |
| 454 | 1532 | `o149_Gun.cpp.obj` | `0x202b14` | `0x202b04` |
| 343 | 800 | `o122_ImageFactory.cpp.obj` | `0x201af4` | NONE |
| 302 | 413 | `o108_ModStation.cpp.obj` | `0x1fdfd0` | `0x1fdf40` |
| 276 | 804 | `o136_ListItemWindow.cpp.obj` | `0x201f44` | `0x201f34` |
| 268 | 540 | `o155_Achievements.cpp.obj` | `0x203144` | `0x203134` |
| 247 | 556 | `o103_StarMap.cpp.obj` | `0x1fdda4` | NONE |
| 224 | 340 | `o101_Status.cpp.obj` | `0x1fc330` | NONE |
| 167 | 320 | `o109_PlayerFighter.cpp.obj` | `0x1fe170` | `0x1fe100` |
| 156 | 943 | `o168_ObjectGun.cpp.obj` | `0x2033a8` | `0x203398` |
| 156 | 656 | `o145_MissionsWindow.cpp.obj` | `0x202868` | `0x202858` |
| 96 | 128 | `** merge strings` | `0x1fdc84` | NONE |
| 67 | 160 | `** merge constants` | `0x1fdd04` | NONE |

## 3. Original `.rodata` content map: `0x1fc3d0..0x1fdd14`

The region is a single contiguous run of Level/PlayerFixedObject/SpaceLounge
data. Offsets are absolute VMAs; content was matched byte-for-byte against our
object files (script `/tmp/w3450_fullmap.py`, reproduced in §10). The `size`
column is the exact byte length found in the original.

```
0x1fc3d0  256  Level   g_ips_shipTypeMap[64]            (first 32 = our table)
0x1fc4d0   80  Level   g_ips_skyColors[20]
0x1fc520   12  Level   asteroid distances (100000,120000,150000)
0x1fc530   12  Level   asteroid distances (60000,100000,120000)
0x1fc540   12  Level   asteroid distances (40000,70000,90000)
0x1fc54c  932  Level   g_cg_meshTable[233]
0x1fc8f0  932  Level   g_cg_soundTable[233]
0x1fcc94  224  Level   kStationTurrets[4][2][7]
0x1fcd74  192  Level   kAlienStationTurrets[8][6]
0x1fce34  168  Level   kAlienOutpostTurrets[7][6]
0x1fcedc  120  Level   kBaseTurrets[5][6]
0x1fcf54   48  Level   kPirateStationCoords[4][3]   (_PIRATE_STATION_POSITIONS)
0x1fcf84   20  Level   kBlueprintRaces[5]
0x1fcf98   60  Level   kBlueprintPositions[5][3]
0x1fcfd4   20  Level   kBlueprintSpacePoints[5]
0x1fcfe8   24  Level   createMission coords
0x1fd000   36  Level   createMission convoyCoords
0x1fd028   24  Level   createMission shuttleCoords
0x1fd040   36  Level   createMission convoyCoords_0
0x1fd068   12  Level   createMission waitTimes_1
0x1fd078  ...  Level   createCampaignMission coords (through 0x1fd200)
0x1fd200  336  Level   kCarrierTurrets[12][7]
0x1fd350  ...  Level   createCampaignMission coords (through 0x1fd578)
0x1fd578  256  Level   kShipHangarHeights[64]
0x1fd678  160  Level   kHangarMeshes[10][4]
0x1fd718   40  Level   kHangarCapacity[10]
0x1fd740   24  Level   kHangarSpotsTerran[2][3]
0x1fd758  108  Level   kHangarSpotsVossk[9][3]
0x1fd7c4   36  Level   kHangarSpotsNivelian[3][3]
0x1fd7e8   12  Level   kHangarSpotsVoid[1][3]
0x1fd7f4   36  Level   kHangarSpotsKhador[3][3]
0x1fd818   24  Level   kHangarSpotsDefault[2][3]
0x1fd830  336  Level   kBarSeats[4][7][3]
0x1fd980  120  Level   kBarMeshes[10][3]
0x1fd9f8  168  Level   stageEntries[]
0x1fdaa0  100  Level   bountyCounts[25]
0x1fdb04   40  Level   bountyEntries[]
0x1fdb2c  100  Level   wantedCounts[25]
0x1fdb90   12  Level   createMission switch table
0x1fdba0   36  Level   initParticleSystems switch table 1
0x1fdbd0   36  Level   initParticleSystems switch table 2
0x1fdc00   36  Level   initParticleSystems switch table 3
0x1fdc30   36  Level   initParticleSystems switch table 4
0x1fdc54   32  Level   jumpgateMeshes[16]           (uint16; see §5)
0x1fdc74   32  Level   kAgentMeshes[8]              (cst32 merge group)
0x1fdc94   32  Level   stageCounts[8]               (cst32 merge group)
0x1fdcb4   32  Level   wantedEntries[8]             (cst32 merge group)
0x1fdcd4   32  PlayerFixedObject  kPirateStationLoot[4][2]  (cst32)
0x1fdcf4   32  SpaceLounge        SpaceLounge_modBonuses[4][2] (cst32)
0x1fdd14  ...  StarMap  StarMap_planetTextures (first StarMap table)
```

Evidence for the cst32 group: the five 32-byte tables at
`0x1fdc74/0x1fdc94/0x1fdcb4/0x1fdcd4/0x1fdcf4` are contiguous and each
content is unique in the original (`grep`-style byte search finds exactly one
occurrence). They are emitted from three different TUs (`Level.cpp`,
`PlayerFixedObject.cpp`, `SpaceLounge.cpp`), so this is the linker's
`.rodata.cst32` merge group, not Level plain `.rodata`.

## 4. `Status` block: `kMissionCase` and `getGammaRayDamagePerSecond`

### 4.1 Original has no `kMissionCase`

Original `Status::missionCompleted` (`0xabf39`) uses two in-text `tbh` tables:

```
abfda: sub.w r1, r0, #150
abfde: cmp   r1, #39
abfe2: tbh   [pc, r1, lsl #1]      ; 40-entry table, types 150..189
...
ac046: sub.w r1, r0, #8
ac04a: cmp   r1, #6
ac04e: tbh   [pc, r1, lsl #1]      ; 7-entry table, types 8..14
```

Our baseline instead loads a 190-byte byte table:

```
ldr r1,[pc]; add r1,pc; ldrb r0,[r1,r0]
sub.w r1,r0,#150; cmp r1,#32; tbh ...
```

The direct-switch type-to-case mapping (inverse of `kMissionCase`) is:

```
163->167 151->174   8->8   0->0  11->11  13->13  14->14
152->150 155->151 154->152 156->153 157->154 159->155
160->158 161->162 162->166 150->170 165->184 166->189
167->156 168->157 169->160 171->163 172->164 173->165
174->168 176->169 177->171 182->172
```

### 4.2 Original `getGammaRayDamagePerSecond` tables are 5 entries + padding

Original `Status::getGammaRayDamagePerSecond` (`0xacdc1`) is instruction-for-
instruction identical to ours (`cmp r3,#4` / `cmp r3,#5` / `ldr.w` from a
switch table). The original `.rodata` at `0x1fc390`/`0x1fc3b0` is:

```
0x1fc390: 0.7 0.4 0.4 0.3 0.2 0 0 0     (8 floats = 32 B)
0x1fc3b0: 3   2   1   0.5 0.3 0 0 0     (8 floats = 32 B)
```

The last three zeros of each are the 12-byte alignment padding after the
5-entry (20-byte) switch table. The original `Status` `.rodata` section is
`0xa0` (inference: clang includes that trailing padding in the section); ours
is `0x94` and the linker supplies the 12 bytes as inter-section padding. The byte content is
therefore already identical once `kMissionCase` is removed; no
`getGammaRay` source change is needed.

## 5. `Level` block target order and the 176-byte decomposition

Original `Level.cpp` plain `.rodata` = `0x1fdc74 - 0x1fc3d0` = **6308 B**.
Our baseline `Level.cpp.obj` `.rodata` = **6132 B** at `0x1fc490`.

The 176-byte delta is exactly:

| item | baseline | original | delta |
|---|---:|---:|---:|
| `g_ips_shipTypeMap` | 32 ints = 128 B | 64 ints = 256 B | **+128** |
| `g_ips_skyColors` | `.rodata.str4.4` (80 B, outside plain) | plain `.rodata` (80 B) | **+80** |
| `jumpgateMeshes` | 32 uint16 = 64 B | 16 uint16 = 32 B | **-32** |
| **net** | | | **+176** |

The original `shipTypeMap` second 32 ints are:

```
8,0,0,2,0,0,0,1,0,1,1,2,1,3,3,3,3,1,1,0,8,1,1,0,3,2,0,0,8,1,3,1
```

The original `jumpgateMeshes` is 16 uint16 (`15000,15002,15001,15003, ...,
15012,15014,15013,15015`); the bytes our source currently puts in
`jumpgateMeshes[16..31]` (`14725,0,14726,0,...`) are byte-identical to the
`kAgentMeshes[8]` int array at `0x1fdc74`. That is why a byte search for
`kAgentMeshes` also "finds" it inside the 64-byte `jumpgateMeshes`; it is a
coincidental overlap, not a second copy. `Level::createSpace` is linked-exact
with either array length (no bounds check), so shrinking the array does not
change codegen.

## 6. Section placement: namespace-scope vs function-local, `volatile`

The original `Level.cpp` TU symbol dump
(`_work/original_layout/original_layout.json`,
`tu_symbols.gof2hd_1.1.19_ios["Classes/Game/Level.cpp"]`) names the tables as
namespace-scope statics: `_SHIP_RACES`, `_SYSTEM_FOG_COLORS`,
`_JUMPGATE_MESHES`, `_JUMPGATE_LOD_MESHES`, `_ASTEROID_MESHES`,
`_TYPE_WEAPONS`, `_WEAPON_SOUNDS`, `_PIRATE_STATION_POSITIONS`,
`_HANGAR_MESHES`, `_BAR_MESHES`, etc. Our source declares most of them as
function-local `static const` arrays, so their emission order follows function
definition order, not the original declaration order.

Controlled clang 7.0.2 section-placement tests (scratch files under `/tmp`,
same flags as `compile_commands.json`):

| source construct | section |
|---|---|
| `static const int a[8]` (last value nonzero) | `.rodata.cst32` |
| `static const int a[8]` (last value 0) | `.rodata.str4.4` |
| `static const uint32_t a[20]` (last value 0) | `.rodata.str4.4` |
| `static const uint32_t a[20]` (last value nonzero) | plain `.rodata` |
| `static const int a[233]` | plain `.rodata` |
| `static const int a[3]` (12 B) | plain `.rodata` |
| `static const int a[4]` (16 B) | `.rodata.cst16` |
| `static const volatile int a[8]` | plain `.rodata` |
| `static const volatile uint16_t a[8]` | plain `.rodata` |
| class static member `S::m[20]` | plain `.rodata` |
| `-fno-merge-all-constants` | no change |
| `-fno-merge-constants` | unsupported warning, no change |
| `-fdata-sections` | per-symbol `.rodata._ZL...`, no change |

So `volatile` is the only source-only way to force a small (<=32 B, power-of-2)
constant array into plain `.rodata`; it may change codegen and must be
gate-tested. Namespace-scope alone does **not** move a 32-byte array out of
`.rodata.cst32`.

## 7. Controlled experiments

All variants are full `cmake --build cmake-build-match --target check -j8`
builds; no `--update-baseline`. A–G were run earlier in this worktree; the
baseline and experiment H were re-run and re-measured for this note.

| # | experiment | byte | linked | `.rodata` | first diff | verdict |
|---|---|---:|---:|---:|---:|---|
| 0 | baseline | 4254 | 4496 | `0x41b67` | `0x1fc390` | — |
| A | `Level.cpp` function reorder (initParticleSystems first, createScene before createRadioMessage, createSpace late) | 4254 | 4496 | `0x41b67` | `0x1fb7f8` | diff worsened (13296 B) |
| B | move `createSpace` after `initParticleSystems` | 4254 | 4496 | `0x41b67` | `0x1fc390` | diff 13284 B |
| C | `g_ips_shipTypeMap[64]` alone | 4234 | 4496 | — | — | RATCHET FAIL |
| D | `jumpgateLodMeshes` swap `15017/15019` | 4254 | 4496 | `0x41b67` | `0x1fb7f8` | diff 13296 B |
| E | direct `switch (m->getType())`, remove `kMissionCase` | 2942 | 4496 | `0x41aa7` (-0xc0) | `0x1fc3d0` | not landable alone |
| F | E + `g_ips_shipTypeMap[64]` | 2942 | 4496 | `0x41b27` (-0x40) | — | still 2942 |
| G | F + Level reorder + `skyColors` volatile/plain + `assignGuns` declaration order | 2942 | 4495 | `0x41b27` (-0x40) | — | linked transient |
| **H** | **E + `shipTypeMap[64]` + `jumpgateMeshes[16]` volatile + `skyColors` volatile + swap ship/sky declaration order** | **2942** | **4495** | **`0x41b07` (-0x60)** | **`0x1fc3d0`** | **front aligned, tail short** |

### Experiment H detail (the most informative one)

Changes:

1. `Status.cpp`: delete `kMissionCase`, replace
   `switch (kMissionCase[m->getType()])` with `switch (m->getType())` and
   relabel cases with the §4.1 mapping.
2. `Level.cpp`: `g_ips_shipTypeMap[64]` with the §5 second half.
3. `Level.cpp`: `jumpgateMeshes[4*4]` (16 uint16) and `volatile`.
4. `Level.cpp`: `g_ips_skyColors` `volatile` (plain `.rodata`).
5. `Level.cpp`: declare `g_ips_shipTypeMap` before `g_ips_skyColors`.

Measured:

```
verify: byte 2942  linked 4495  stubs 0  missing 0  extra 0  avg 99.89
.rodata 0x41b07 (-0x60)
differing bytes 30528 / 269159 (11.34%)
first VMA 0x1fc3d0
align: equal 14963  delete 510  insert 656  replace 30; moved 747
```

Object-level effect:

| section | baseline | H | original |
|---|---:|---:|---:|
| `Status.cpp.obj` `.rodata` | 340 (`0x154`) | 148 (`0x94`) | 160 (`0xa0`) |
| `Level.cpp.obj` `.rodata` | 6132 (`0x17f4`) | 6308 (`0x18a4`) | 6308 (`0x18a4`) |
| `Level.cpp.obj` `.rodata.str4.4` | 80 (`0x50`) | 0 | 0 |
| `Level.cpp.obj` `.rodata.cst32` | 96 (`0x60`) | 96 (`0x60`) | 96 (`0x60`) |

Link-level effect (H):

```
Status block   0x1fc330..0x1fc3d0   byte-identical
Level plain    0x1fc3d0..0x1fdc74   correct start/size (content order still differs)
cst32 group    0x1fdc74..0x1fdd14   correct position
StarMap        0x1fdd14             correct (original 0x1fdd14)
ModStation     0x1fdf40             correct (original 0x1fdf40)
Globals        0x1fe2b0             original 0x1fe2a0 (+0x10)
DialogueWindow 0x203750             original 0x2037b0 (-0x60)
```

So H fixes the whole front of the file but leaves a **0x60 total-size
shortfall** and a **0x50 tail shift** that starts in the
`MenuTouchWindow`/`ListItemWindow` region. The `Level` content order is also
still wrong: H emits `jumpgateMeshes` first (createSpace is the first function
with statics), while the original has it last. Fixing the order requires
namespace-scope declarations in the original declaration order (or moving the
owning functions), not just array-size changes.

## 8. Discrepancy: 13,266 vs 13,308

`docs/IDENTICALITY.md` (update 2026-10-26) and
`_work/reconstructions/rodata_content_map_round2.md` both state **13,308**
differing `.rodata` bytes. The measurement at `cfbd140e` with the committed
scripts is **13,266** (4.93%, 5,150 runs, first `0x1fc390`). The 42-byte
difference is most likely a later commit between round 2's `8bf6a675` base and
`cfbd140e` (the `dynsym app-region`/`LevelScript`/`docs` commits) changing a
few bytes; it is not a script change. Quote 13,266 for this HEAD.

## 9. TU-membership corrections

From `_work/original_layout/original_layout.json`:

* `Classes/Game/Level.cpp` owns `_PIRATE_STATION_POSITIONS` (= our
  `kPirateStationCoords`, 48 B at `0x1fcf54`), not `kPirateStationLoot`.
* `Classes/Game/PlayerFixedObject.cpp` owns `_PIRATE_STATION_ORBITS` and
  `_PIRATE_STATION_ITEMS_AND_AMOUNTS` (= our `kPirateStationIds` and
  `kPirateStationLoot`). So `kPirateStationLoot` is correctly in
  `PlayerFixedObject.cpp`; do **not** move it to `Level.cpp`.
* `SpaceLounge_modBonuses` (32 B at `0x1fdcf4`) is from `SpaceLounge.cpp`, not
  Level.
* The original `Level.cpp` table symbols are namespace-scope
  (`_SHIP_RACES`, `_SYSTEM_FOG_COLORS`, `_JUMPGATE_MESHES`, `_TYPE_WEAPONS`,
  `_WEAPON_SOUNDS`, `_HANGAR_MESHES`, `_BAR_MESHES`, ...), whereas ours are
  function-local statics. This is the root cause of the `.rodata` order
  divergence.

## 10. Proven vs hypothesis

Proven (commands above):

1. Baseline is byte 4254 / linked 4496; `.rodata` is size/base exact
   (`0x41b67 @ 0x1c9e10`); lint clean; drift 0.
2. 13,266/269,159 `.rodata` bytes differ at `cfbd140e`; first diff `0x1fc390`;
   `kMissionCase` is the only symbol with no verbatim copy.
3. The original `Status` block is `0xa0` and ends at `0x1fc3d0`; the original
   `getGammaRayDamagePerSecond` tables are the 5-entry switch tables plus
   12-byte alignment padding.
4. The original `Level` plain `.rodata` is `0x1fc3d0..0x1fdc74` = 6308 B and
   its 176-byte delta vs ours decomposes exactly as `+128` shipTypeMap
   `+80` skyColors `-32` jumpgateMeshes.
5. The original cst32 merge group is contiguous at
   `0x1fdc74..0x1fdd14` and contains Level, PlayerFixedObject and SpaceLounge
   tables.
6. `kPirateStationLoot` belongs to `PlayerFixedObject.cpp` (TU dump).
7. `volatile` is the only tested source-only way to force <=32-byte constant
   arrays into plain `.rodata`; namespace-scope and
   `-fno-merge-all-constants` do not.
8. Experiment H makes the Status block byte-identical and puts Level at
   `0x1fc3d0`, StarMap at `0x1fdd14`, ModStation at `0x1fdf40`, but the total
   `.rodata` is `0x41b07` (-0x60) and byte_exact is 2942, so it is not
   landable alone.

Hypothesis (consistent with the evidence, not yet proven):

1. The original `jumpgateMeshes` is 16 uint16 (32 B) and `createSpace`'s
   `race*4+k` indexing over-reads into `kAgentMeshes` for race >= 4 (or the
   game only calls it for races 0..3). The alternative — 32 uint16 and a
   linker-merged duplicate — would require overlapping sections.
2. The remaining 0x60 shortfall in H is a separate `MenuTouchWindow`-region
   content/size issue (its `.rodata` has no verbatim copy in the original),
   not a Level issue; fixing it plus the Level declaration order should make H
   a large strict improvement.
3. The correct long-term fix is to make the Level tables namespace-scope
   statics in the original declaration order (shipTypeMap, skyColors,
   asteroid distances, meshTable, soundTable, turrets, ..., jumpgateMeshes),
   with `volatile` only where a <=32-byte array must stay in plain `.rodata`.

## 11. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w3450

cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8

# scratch -Map relink (see §2)
ninja -C cmake-build-match -t commands libgof2hdaa.so | grep 'ld.gold -shared' \
  > /tmp/w3450_linkcmd.txt
# edit: -Map=/tmp/w3450_base.map -o /tmp/w3450_base_unstripped.so, remove strip
bash /tmp/w3450_relink_base.sh

python3 _work/reconstructions/rodata_content_diff_map.py \
  --orig ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --unstripped /tmp/w3450_base_unstripped.so \
  --map /tmp/w3450_base.map
python3 _work/reconstructions/rodata_content_map_round2.py \
  --map /tmp/w3450_base.map \
  --orig ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so
```

The content-map helper used for §3 is committed alongside this note as
`_work/reconstructions/rodata_content_map_round3.py`:

```sh
python3 _work/reconstructions/rodata_content_map_round3.py \
  --orig ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --build cmake-build-match --lo 0x1fc3d0 --hi 0x1fdd14
```

It reads every `.rodata`/`.data` symbol from every gof2 object, searches the
original `.rodata` for the exact bytes, and prints VMA, size, symbol, owner and
input section.  The first-occurrence caveat is documented in the script.

No source or flag change was landed by this pass; the worktree is clean at
`cfbd140e` with baseline 4254/4496.
