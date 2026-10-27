# Jump-table, data-table, and function-pointer audit

Analysis-only report for branch `wave39a-w916` (HEAD `969f4608`). No `src/` or build files were
changed. The only tracked artifact is this report.

## Scope and method

Targets audited in `src/`:

- manual jump-table emulation (`Status::missionCompleted`);
- file-static lookup/data tables, especially zero-initialised placeholders;
- `reinterpret_cast` function-pointer / member-pointer calls;
- dead decompiler-era callback globals and fake vtable views;
- layout anchors and other "keep the section size" constructs;
- large hand-maintained tables that are probably original source data.

Ground truth used:

1. `../_work/bins/android_2.0.16_libgof2hdaa.so` (ELF32 ARM, image base 0x10000).
2. `llvm-objdump -d --triple=thumbv7-none-linux-gnueabi --start-address=... --stop-address=...`.
3. `readelf -sW` / `readelf -SW` for symbol values, sizes and section sizes.
4. Direct byte reads from the ELF file for `.rodata` tables. In this binary `.rodata` has
   `vaddr == file offset` (`0x1c9e10`), so `struct.unpack_from("<i", data, address)` is valid.
5. `report.json` for the current per-function verify rows.
6. `_work/original_layout/tu_mapping.md` and `_work/original_layout/original_layout_notes.md` for
   original TU ownership.
7. `docs/IDENTICALITY.md` and `docs/VALIDATION.md` for the current gate state and known verifier
   limitations.

Convention: ELF `st_value` has the Thumb bit set; disassembly starts at `st_value & ~1`.
"Observation" means read from source/binary. "Inference" means a proposed source shape.

Current gate context from `report.json` and `docs/IDENTICALITY.md`:

- compared 4523, `byte_exact` 1352, `linked_exact` 4468, avg 99.78%, `stub_zero_size` 0.
- section sizes: `.text` orig 1,259,464 / ours 1,262,528 (+3,064); `.rodata` 269,159 / 268,007
  (-1,152); `.data` 10,604 / 10,616 (+12); `.bss` 27,052 / 27,280 (+228).

## Executive summary

The highest-value findings are correctness bugs hidden behind 100% `linked_exact` rows:

1. **`Status::missionCompleted` is semantically broken by a zero `.bss` "mission-order" table.**
   `kMissionOrder[190]` is all zero, so every mission type reaches `case 0`. The original binary
   uses a direct `switch (m->getType())` with two Thumb TBH tables, not a table lookup.
2. **Three hand-maintained lookup tables are wrong.** `Agent::kModPriceTable`,
   `SolarSystem::kAttackRaceTable`, and `Generator::kaamoSpecialItems` are all zero, but the
   original `.rodata` tables are `{20,30,40,20}`, `{1,0,3,2}`, and
   `{200,220,208,213,216,228,229,230,231}` respectively. All three functions currently score
   100% `linked_exact`, because the table is TU-local and the verifier does not compare its bytes.
3. **A large amount of decompiler scaffolding is still present.** `RefreshThunk` /
   `ModuleTransitionThunk` / `RefreshFn`, 24 unused `StarMap` globals, unused `LevelScript` and
   `SpaceLounge` function-pointer typedefs, a fake `PlayerEgoVtable`, and a `Sparks` function
   pointer that only exists to model an indirect GOT call are all dead or non-source-like.

The member-pointer casts in `AEFile`, `ModStation`, `MissionsWindow`, `Hud`, and
`MenuTouchWindow` are a separate, lower-risk cleanup class: the original binary calls the real
methods directly (or through the normal vtable), and the casts exist only to force a bool return
type. They should be changed one site at a time under `verify-fn`.

## F1. `Status::missionCompleted`: manual jump-table emulation is wrong

### Location

- `src/game/mission/Status.cpp:2048-2051`:

```cpp
// Layout anchor: the original 190-byte mission-order table lives in .bss so
// the .rodata/.data page boundary stays fixed. TODO(decomp): initialize with
// the original values; the zero table preserves the original codegen only.
static volatile unsigned char kMissionOrder[190];
```

- `src/game/mission/Status.cpp:2072`:

```cpp
switch (kMissionOrder[m->getType()]) {
```

- `src/engine/render/shaders/CubeMapping.cpp:71-75` explicitly says the 265-byte
  `kCubeMappingLayoutAnchor` compensates for the 190-byte Status table.

### Binary observation

- `_ZN6Status16missionCompletedEbbx`: ELF `st_value 0xabf39`, size 1432; disassembly starts
  `0xabf38`. `report.json` row: `pct 85.4`, `osize 1432`, `usize 1432`, `linked false`.
- The function contains **two TBH jump tables**:

```text
high table: TBH @ 0xabfe2, 40 halfwords @ 0xabfe6
            index = type - 0x96 (150), valid 0..39
low table:  TBH @ 0xac04e, 7 halfwords @ 0xac052
            index = type - 8, valid 0..6
```

The high-table halfwords are:

```text
0x0095 0x0098 0x009b 0x00b5 0x00b8 0x00c4 0x01a2 0x01b7 0x00cd 0x023a
0x01d1 0x023a 0x00fb 0x01e8 0x01f6 0x01f8 0x012e 0x0028 0x01fc 0x020b
0x0160 0x020d 0x021a 0x023a 0x0028 0x023a 0x023a 0x023a 0x023a 0x023a
0x023a 0x023a 0x023a 0x023a 0x0167 0x023a 0x023a 0x023a 0x023a 0x0184
```

Because Thumb TBH targets are `table_base + 2 * entry`, the first entries are
`0xac110`, `0xac116`, `0xac11c`, `0xac150`, ... and the default/continue target is
`0xac45a`. The low-table entries are `0x0007, 0x0204, 0x0204, 0x0039, 0x0204, 0x003e,
0x0045`, i.e. targets `0xac060`, `0xac45a`, `0xac45a`, `0xac0c4`, `0xac45a`, `0xac0ce`,
`0xac0dc`.

The original body computes `type - 0x96` and `type - 8` directly and dispatches through these
tables. There is no load from a 190-byte global table in this function.

### Observation vs inference

- **Observation:** the original function is a direct switch on `m->getType()` with two TBH tables.
- **Observation:** the current `kMissionOrder` is a file-static `volatile` zero array in `.bss`;
  every entry is zero.
- **Inference:** the 190-byte array is a source-shaping/codegen device introduced to force clang's
  jump-table block order, not original game data. The original source shape is the direct switch.
- **Correctness bug:** current `switch (kMissionOrder[m->getType()])` always sees `0`, so every
  mission type takes `case 0`/`case 11` (the `atStation && station index == target` block) and the
  real per-type cases are unreachable.

### Intended ranking values (if the table is kept)

Commit `93d31479` introduced the following non-zero ranking table before `792160d8` moved it to
zero `.bss`. It is a useful recovery key, but it is still a compiler-shaping table, not the original
source:

```text
type -> rank
0x00 -> 0     0x08 -> 8     0x0b -> 11    0x0d -> 13    0x0e -> 14
0x96 -> 152   0x97 -> 154   0x98 -> 155   0x99 -> 156   0x9a -> 157
0x9b -> 159   0x9c -> 167   0x9d -> 168   0x9e -> 160   0xa0 -> 169
0xa2 -> 161   0xa3 -> 171   0xa4 -> 172   0xa5 -> 173   0xa6 -> 162
0xa7 -> 150   0xa8 -> 174   0xa9 -> 176   0xaa -> 163   0xab -> 177
0xac -> 178   0xae -> 151   0xb8 -> 165   0xbd -> 166
all other types -> 190 (default)
```

### Proposed human C++

Preferred, original-shaped replacement:

```cpp
switch (m->getType()) {
    case 0xa7:
    case 0xae:
        ...
    case 0x96:
        ...
    case 0x97:
        ...
    /* ... original case values ... */
}
```

If the 85.4% block-order result must be preserved while fixing semantics, keep a clearly documented
`static const unsigned char kMissionOrder[190]` with the non-zero mapping above and keep the
`switch (kMissionOrder[m->getType()])`; do **not** leave it zero. A `constexpr` array or a small
`switch` helper is preferable to `volatile`.

### Risk and next step

- High gate risk: the 190-byte `.bss` table, the `CubeMapping` `.rodata` anchor at
  `src/engine/render/shaders/CubeMapping.cpp:75`, and the `Status::moveWanted` anchor at
  `src/game/mission/Status.cpp:2330` are coupled. Removing or initialising the table changes
  `.bss`/`.rodata` and may shift `.data`/`.got`.
- Bounded assignment: one worker, one file plus the `CubeMapping` anchor. First fix semantics with
  the non-zero ranking table (smallest diff), run `FN=_ZN6Status16missionCompletedEbbx
  cmake --build cmake-build-match --target verify-fn`, then decide whether to replace the table
  with the direct switch. Re-tune `kCubeMappingLayoutAnchor` only after the function is correct.

## F2. Three zero lookup tables are objectively wrong

All three functions currently score 100% `linked_exact` in `report.json`. The table is file-static,
so the verifier compares the relocation/address and not the table bytes; this is the dangerous
"local data is invisible" class described in `docs/VALIDATION.md`.

### F2a. `Agent::getModPricePercentage`

- Source: `src/game/ship/Agent.cpp:53-59`.
- Current table: `static const int kModPriceTable[4] = {0, 0, 0, 0};`.
- Binary: `_ZN5Agent21getModPricePercentageEv`, ELF `0x177739`, size 28, disassembly `0x177738`.
- Original code:

```text
177738: ldr.w r0,[r0,#0x94]       ; sellModIndex
17773c: cmp   r0,#3
17773e: itt   hi
177740: movhi r0,#0x28            ; fallback 40
177742: bxhi  lr
177744: ldr   r1,[pc,#8]          ; literal -> table
177746: add   r1,pc
177748: ldr.w r0,[r1,r0,lsl #2]
17774c: bx    lr
```

- Table address: `0x1fb8e8` in `.rodata`.
- Table bytes: `14 00 00 00 1e 00 00 00 28 00 00 00 14 00 00 00`
  = `{20, 30, 40, 20}`.
- Proposed: `static const int kModPriceTable[4] = {20, 30, 40, 20};` (or `constexpr` /
  `std::array`), keeping the `i < 4 ? table[i] : 0x28` shape.

### F2b. `SolarSystem::getAttackRace`

- Source: `src/game/world/SolarSystem.cpp:10`.
- Current table: `static const int kAttackRaceTable[4] = {0, 0, 0, 0};`.
- Binary: `_ZN11SolarSystem13getAttackRaceEv`, ELF `0x155709`, size 24, disassembly `0x155708`.
- Original code loads a table at `0x1fb878` and falls back to `8` for index > 3.
- Table bytes: `01 00 00 00 00 00 00 00 03 00 00 00 02 00 00 00`
  = `{1, 0, 3, 2}`.
- Cross-check: `src/game/world/Standing.cpp:48` and `:134` already use `{1, 0, 3, 2}` for the
  same race permutation.
- Proposed: `static const int kAttackRaceTable[4] = {1, 0, 3, 2};`.

### F2c. `Generator::isKaamoSpecialItem`

- Source: `src/game/mission/Generator.cpp:48-49,77-87`.
- Current table: `static const int kaamoSpecialItems[9] = {};`.
- Binary: `_ZN9Generator18isKaamoSpecialItemEi`, ELF `0x95405`, size 36, disassembly `0x95404`.
- Original code loads a 9-entry table at `0x1fb590` and scans it for the item.
- Table bytes: `c8 00 00 00 dc 00 00 00 d0 00 00 00 d5 00 00 00 d8 00 00 00
  e4 00 00 00 e5 00 00 00 e6 00 00 00 e7 00 00 00`
  = `{200, 220, 208, 213, 216, 228, 229, 230, 231}`.
- Note: this is the same value set as `kNeverUnlockedItems[9]` at
  `src/game/mission/Generator.cpp:41`, but the original has a separate table at `0x1fb590`;
  keep a separate `kaamoSpecialItems` array to preserve data layout.
- Proposed: `static const int kaamoSpecialItems[9] = {200, 220, 208, 213, 216, 228, 229,
  230, 231};`.

### Risk and next step

- These are correctness fixes, not code-shape fixes. The functions already match codegen, so the
  risk is section layout (`.rodata` bytes and possibly `.data`/`.bss` placement), not control flow.
- Bounded assignment: one worker, three files, one table at a time. For each:
  `FN=<mangled> cmake --build cmake-build-match --target verify-fn`, then `diff`/section-size check.
  Do not combine with F1 or the anchor cleanup; the tables are in different TUs but the link layout
  is global.
- A useful follow-up tool is a local-table extractor: locate each file-static table's PC-relative
  literal, read its original bytes, and compare with the current initialiser. The three above show
  the verifier alone is not sufficient.

## F3. Dead decompiler-era thunk structs and typedefs

### F3a. `RefreshThunk` / `ModuleTransitionThunk` / `RefreshFn`

- `src/game/ui/RefreshThunk.h:8-10` defines `struct RefreshThunk { void (*refreshFn)(); };`.
- `src/game/ui/ModuleTransitionThunk.h:7-13` defines `struct ModuleTransitionThunk { void
  (*transitionFn)(void *app, int mode); };`.
- `src/game/ui/MenuTouchWindow.h:6,8` includes both headers.
- `src/game/ui/MenuTouchWindow.cpp:307` defines `typedef void (*RefreshFn)();`.
- `rg -n 'RefreshThunk|ModuleTransitionThunk|refreshFn|transitionFn|RefreshFn' src` finds only
  those declarations/includes; there is no use of either struct, either member, or the typedef.

**Observation:** dead. **Inference:** decompiler-era placeholders; the original `MenuTouchWindow`
source did not need these types. **Proposed:** delete both headers, remove the two includes, and
delete the unused `RefreshFn` typedef. If a real callback is discovered later, use a typed
`using RefreshFn = void (*)();` at the actual call site.

**Risk:** no codegen if truly unused; header/include graph and lint only. Confirm with a build and
`report.json` (the dead types should not change any function row).

### F3b. `StarMap` 24 unused static globals

- `src/game/world/StarMap.cpp:38-41,45-47,69-71,77-79,81,83,85,87,89,95,97-101` declare 24
  file-static pointers/pointer-to-pointers: `g_StarMap_draw_*`, `g_StarMap_end_*`,
  `g_StarMap_touch_*`, `g_StarMap_drawKey_*`, `g_StarMap_init_*`.
- `rg -c` for each name in `StarMap.cpp` returns `1` (the declaration only); no assignment or read
  exists. The real code uses `Globals::Canvas`, `Globals::layout`, `Globals::gameText`, etc.
- On 32-bit ARM these are 24 * 4 = 96 bytes of `.bss`.

**Observation:** dead. **Inference:** decompiler scaffolding from an earlier reconstruction.
**Proposed:** delete all 24 globals. **Risk:** `.bss` shrinks by 96 bytes; current `.bss` is +228 vs
original, so this may improve or may expose a layout anchor. Verify `StarMap::draw` (current 89.7%,
2644/2636) and the `.bss` size.

### F3c. `LevelScript`, `SpaceLounge`, and `Level` dead function-pointer views

- `src/game/world/LevelScript.cpp:55-63`: `LevelListProc`, `ReadWaypointProc`, `SetVectorProc`,
  `VirtualCommandProc`, and `static LevelListProc gLevelListProc;` — no use after the declaration.
- `src/game/menu/SpaceLounge.cpp:59-60,109`: `ScreenProjectFn` and
  `static ScreenProjectFn SpaceLounge_screen_projector;` — no assignment/use. The real projection
  calls are direct `Globals::Canvas->GetScreenPosition(...)` at `SpaceLounge.cpp:1455,1458`.
- `src/game/world/Level.cpp:172-187`: `PlayerEgoVtable` / `PlayerEgoPolymorphic` with
  `placeAtStation` at offset `0x1c`; only a `static_assert` uses it. `PlayerEgo` has no virtual
  methods in `src/game/ship/PlayerEgo.h`, so this is a fake vtable view.
- `src/game/world/Level.cpp:189,7220-7227`: `levelSpawnFar` is declared/defined but never called.

**Proposed:** delete the unused typedefs/globals and the fake vtable; if a real call site is found,
replace with a typed direct call (`Globals::Canvas->GetScreenPosition`, a real `PlayerEgo` method,
etc.). **Risk:** low codegen; `gLevelListProc` and `SpaceLounge_screen_projector` are 4 bytes each
in `.bss`, and the vtable/static_assert is compile-time only.

### F3d. Unused one-off arrays

- `src/game/ui/MenuTouchWindow.cpp:310`: `static const int g_mtw_upTextIds[16] = {};` — no use.
- `src/game/ui/Hud.cpp:1019-1022,1111-1112,1211`: `g_Hud_ccHashX/N`, `g_Hud_ccUnit/Unit2`,
  `g_Hud_swSep/swEnd`, `g_Hud_initMsg` are all one-byte zero arrays and have no uses.
- `src/game/menu/SpaceLounge.cpp:75`: `static int SpaceLounge_touch_race_vectors[64];` — no use.

**Observation:** dead. **Inference:** layout/string anchors or leftover decompiler data.
**Proposed:** delete if the section-size delta permits; otherwise document them explicitly as
anchors. `SpaceLounge_silhouetteChar[8]` at `SpaceLounge.cpp:172` is different: it is a real
zero-initialised character descriptor used at line 289, so keep it.

## F4. `Sparks::explode` function-pointer indirection

- `src/engine/render/Sparks.cpp:99`:
  `static int (*g_Sparks_nextInt_explode)(AERandom *rng, int bound) = Sparks_nextInt;`
- Used at `Sparks.cpp:113`: `int(*nextInt)(AERandom *, int) = g_Sparks_nextInt_explode;`
- `Sparks_nextInt` is a one-line wrapper at `Sparks.cpp:7-9`.
- Binary: `_ZN6Sparks7explodeEiii`, ELF `0x15e48d`, size 264, disassembly `0x15e48c`. The original
  does an indirect `blx r9` at `0x15e4de`, `0x15e4f2`, `0x15e50c`; `r9` is loaded from a GOT
  literal, not from a user-visible function-pointer table.
- `report.json`: `_ZN6Sparks7explodeEiii` is 100.0% `linked_exact`, 264/264.

**Observation:** the original has an indirect call through the GOT (normal for a shared-library
call), not a 4-byte static function-pointer global. **Inference:** `g_Sparks_nextInt_explode` is a
decompiler artifact. **Proposed:** replace the table/pointer with a direct
`Sparks_nextInt(*rng, 0x190)` (or `(*rng)->nextInt(0x190)` if `AERandom` is modeled as an object).
**Risk:** the current 100% row depends on the indirect-call shape; changing it may alter
scheduling/register allocation. Verify `FN=_ZN6Sparks7explodeEiii` before keeping.

## F5. Member-pointer casts used as bool-return type puns

These are not jump tables, but they are non-source-like control constructs. The original methods
return a byte/int and are called directly; the casts force a bool-returning member-pointer call to
make clang emit the expected branch.

| Site | Current cast | Original evidence | Proposed human C++ |
|---|---|---|---|
| `src/engine/file/AEFile.cpp:437-439` | `bool (FileInterface::*)(char*,bool)` around `&FileInterface::FileEnumInit` | `_ZN6AEFile15collectPakFiles...` at `0x70669`/size 244; original does `ldr r3,[r1,#0x3c]; blx r3` (normal virtual call) | declare `FileInterface::FileEnumInit` and override as `bool`; call `fileInterface->FileEnumInit(pathChars,false)` directly |
| `src/game/menu/ModStation.cpp:921-923` | `void (Status::*)(Station*)` around `&Status::departStation` | `_ZN10ModStation12enterStationEv` at `0xd7201`/size 172; original has a direct `blx 0x645fc` to `Status::departStation` | `Globals::status->departStation(Globals::status->getStation());` |
| `src/game/menu/ModStation.cpp:2217-2218,2643-2644` | `bool (Radio::*)()` around `&Radio::lastMessageShown` | `_ZN5Radio16lastMessageShownEv` at `0x15505b`/size 6: `ldrb.w r0,[r0,#0x2c]; bx lr` | declare `Radio::lastMessageShown()` as `bool`; call directly |
| `src/game/ui/MissionsWindow.cpp:542-543` | `bool (Layout::*)()` around `&Layout::helpPressed` | `_ZN6Layout11helpPressedEv` at `0xd4bc9`/size 6: `ldrb.w r0,[r0,#0x3c0]; bx lr` | declare `Layout::helpPressed()` as `bool`; call directly |
| `src/game/ui/Hud.cpp:851-852` | `(bool (SolarSystem::*)())&SolarSystem::hasNoOwner` | `_ZN11SolarSystem10hasNoOwnerEv` at `0x1558e3`/size 30; returns 0/1 | declare `SolarSystem::hasNoOwner()` as `bool`; call directly |
| `src/game/ui/MenuTouchWindow.cpp:2540-2543` | `bool (FModSound::*MtwBoolFn)()` around `&FModSound::tryToStopMusicForBGMusic` | `_ZN9FModSound24tryToStopMusicForBGMusicEv` at `0x92da9`/size 4: `movs r0,#0; bx lr` | declare it `bool`; call `Globals::sound->tryToStopMusicForBGMusic()` directly |

**Observation:** all of these functions already match 100% (or near-100%) in `report.json`, so the
casts are purely type/codegen shims. **Inference:** the original headers used `bool` for these
methods. **Risk:** Itanium does not mangle a non-template function's return type, so the symbol
names stay stable, but callers' branch lowering and register allocation can change. Change one site
per pass and verify the caller (`AEFile::collectPakFiles` 100%, `ModStation::enterStation` 100%,
`ModStation::OnUpdate` 99.7%, `MissionsWindow::OnTouchEnd` 100%, `Hud::drawOrbitInformation` 100%,
`MenuTouchWindow::OnTouchMove` 100%). Do not batch all six.

## F6. Layout anchors and fake control/data

These are intentionally gate-sensitive. They are not necessarily bugs, but they are the main
"non-source-like" constructs left in `src/`.

| Site | Construct | Why it exists | Risk / next step |
|---|---|---|---|
| `src/engine/math/Transform.cpp:8-13` | 72-byte `g_transform_matrix_flag` in natural `.data` (was 80 B in `.aezdata`) | keeps pre-`.got` data distance stable for libc++ exception ctors | `2d3a2a9b`; `.got` unchanged |
| `src/engine/file/AEFile.cpp:7-9` | `.aezdata` 8-byte `pakExtension` | same class of pre-`.got` anchor | keep; document |
| `src/engine/core/GameText.cpp:51-55` | **removed** (was `[[gnu::used]] arabicFormRow`, 16-byte `.text`) | real hidden `arabicFormOffset` now used by both lookups | `19e43d05`; `.text` unchanged |
| `src/game/weapons/ObjectGun.cpp:35-40` | **removed** (was `[[gnu::used]] ObjectGunProjectileMesh`, 20-byte `.text`) | real hidden helper now used by ctor/update | `c902617a`; `.text` unchanged |
| `src/game/world/StarSystem.cpp:130-135` | 14-byte `starSystemLayoutAnchor` | absorbs section-alignment slack | keep; `StarSystem::StarSystem` is 93.9% |
| `src/game/mission/Generator.cpp:51-55` | 8-byte `generatorShipListLayoutAnchor` | keeps Generator TU `.text` size | keep; separate from F2c table fix |
| `src/game/ui/StatusWindow.cpp:16-23` | **removed** (was `statusWindowClamp`) | real hidden medal-row height helper now used by both ctor branches | `6c78ad36`; `.text` unchanged |
| `src/game/mission/RecordHandler.cpp:1415-1424` | **removed** (was `mixRecordStoreSlot`) | real hidden libc++abi `is_equal` helper | `0a8cdbe6`; `.text` unchanged |
| `src/game/world/StarMap.cpp:55-57` | `[[gnu::used]] StarMap_GetScreenPosition` | `.text` anchor | keep |
| `src/game/ui/MenuTouchWindow.cpp:328-334,3898+` | never-taken `anchorFn` hook + huge `mtwTextLayoutAnchor` store body | keeps TU on `0x1333c0 + k*0x1000`; libstdc++ exception ctors depend on it | high risk; re-tune with `tools/verify/verify.py --show _ZNSt9bad_allocC1Ev` |
| `src/game/world/Level.cpp:2598-2599,7247-7255` | never-taken `anchorFn` + `levelTextAnchor` volatile double stores | keeps Level TU `.text` size | keep |
| `src/game/menu/HackingGame.cpp:236-238` | `volatile int *anchor = &offset[0][0]; *anchor += 1;` | local `.text` padding; `offset` is a local array, so the increment is dead | keep or replace with a smaller real body; harmless semantically |
| `src/game/world/LevelScript.cpp:4805-4812` | `volatile char layoutPad` | `.text` size | keep |
| `src/game/ui/HangarWindow.cpp:898-905` | repeated `description = description;` | `.text` size; no-op on rendered text | keep |
| `src/game/mission/Status.cpp:2330-2333` | `if (reinterpret_cast<intptr_t>(this) == 1) return;` | unreachable `.text` anchor in `moveWanted` | keep until F1 is settled; it is coupled to the TU size |
| `src/engine/render/shaders/CubeMapping.cpp:71-75` | **removed** (was 75-byte `kCubeMappingLayoutAnchor`) | `.rodata` page boundary / RW segment address | removed by `1ed51c3c`; gate unchanged |

**Observation:** these are all documented in-source and/or in `docs/IDENTICALITY.md`.
**Inference:** they are temporary scaffolding, not original source. **Risk:** removing any one can
change `.text`/`.rodata`/`.data`/`.got` and break unrelated byte-exact functions. Keep them until
the corresponding real code is recovered; do not clean them up in a data-table pass.

## F7. Large hand-maintained tables (probably original data; audit, do not rewrite)

The following are large static tables in `src/`. They look like real original data, not decompiler
artifacts. No value errors were proven in this pass; they are listed so a future worker knows where
the data-table risk is.

| File:line | Table(s) | Notes |
|---|---|---|
| `src/game/world/Level.cpp:93-94` | `g_cg_meshTable[233]`, `g_cg_soundTable[233]` | per-item mesh/sound tables; high value, verify against original `.rodata` if touched |
| `src/game/world/Level.cpp:1130-1141` | `stageEntries`, `stageCounts`, `bountyCounts`, `bountyEntries`, `wantedCounts`, `wantedEntries` | local mission tables |
| `src/game/world/Level.cpp:1556-1599` | `g_tommyFace`, `kStationTurrets`, `kAlienStationTurrets`, `kAlienOutpostTurrets`, `kBaseTurrets`, `kPirateStationCoords`, `kBlueprintRaces`, `kBlueprintPositions`, `kBlueprintSpacePoints` | station/blueprint data |
| `src/game/world/Level.cpp:6691-6697` | `g_ips_skyColors[16]`, `g_ips_shipTypeMap[32]` | particle/ship mapping |
| `src/game/world/Level.cpp:6922-6960` | hangar spot tables, bar child/mesh tables, ship hangar heights, capacity/seats | large hangar data block |
| `src/game/core/Globals.cpp:363-441` | ship geometry/mesh/LOD tables, race tables, music-by-race, `gDialogueSoundPairs[3008]` | global resource data |
| `src/game/menu/ModStation.cpp:106-260` | `gStoryFaces[62][5]`, `gStoryFaceTable[63]`, medal/camera/station tables | story/face data; `gPirateGreetingFaceB[5]` is zero but used as a face |
| `src/game/ui/DialogueWindow.cpp:184-380` | `g_dw_briefingScript[120]`, `g_dw_successScript[2056]`, station-108 scripts, face tables | dialogue scripts |
| `src/game/world/StarSystem.cpp:35-103` | `kSunTextures[19]`, `kSystemTints[34]`, `kCurrentStationTextures[27]`, `kStationTextures[27]`, `kSunLightColors[57]`, `kStationLightColors[81]`, `kRimLightColors[57]` | rendering data |
| `src/game/world/StarMap.cpp:197-203` | planet textures, key text ids, station scales, race images, security colors, ship races | map data |
| `src/game/mission/Generator.cpp:19-49` | `kBlockedMissionStations[52]`, agent offer items/ships/enemy races, `kShipRaces[64]`, `kKaamoStationItems`, `kNeverUnlockedItems`, `kLootTypeChance` | `kaamoSpecialItems` is the wrong one (F2c) |
| `src/game/mission/Achievements.cpp:9-57` | `gAchievementValues[135]`, `gCFN_req[45][3]` | achievement data |
| `src/game/ui/TouchButton.cpp:164-187` | medal tint/image/icon tables, `g_TB_frameImagesTablet[21][9]`, `g_TB_frameImagesPhone[21][9]` | UI data |
| `src/game/ui/ChoiceWindow.cpp:133-269` | medal tables | UI data |
| `src/game/ui/ListItemWindow.cpp:27-140,636` | unit/mesh/hidden-attribute/key-text/angle tables | UI data |
| `src/game/ui/MissionsWindow.cpp:43` | `CAMPAIGN_GOAL_TEXT[164]` | text data |
| `src/game/menu/SpaceLounge.cpp:152-172,489` | camera positions/rotations, `SpaceLounge_touch_race_vectors[64]` (unused), `SpaceLounge_silhouetteChar[8]` (used) | camera/character data |
| `src/game/menu/MGame.cpp:88-100` | `gMGameFaces[15]`, `gShipRace[64]`, `gWingmenRadioMissions[3]` | menu data |
| `src/game/core/CharacterImages.h:10-90` | 63 `CHAR_*[5]` tables + `CHAR_IMAGES[63]` pointer table | header-static per TU by design; values from original `.data` |
| `src/engine/render/ImageFactory.cpp:9-114` | `IMAGE_OFFSETS_IPAD_HD/LARGE/IPAD`, `IMAGE_OFFSETS[13][4][2]`, `kPartImageIds`, `kRetinaOffsets`, `gCreateChar2Table` | image layout data |
| `src/engine/render/LensFlare.cpp:6-8` | `kFlareRed/Green/Blue[5]` | rendering data |
| `src/engine/render/AEFile.cpp:625` | `crc32Table[256]` | standard CRC table |
| `src/engine/core/GameText.cpp:7,58` | `kArabicForms[41][5]`, `kNonJoiningAfter[10]` | Arabic shaping data |
| `src/engine/audio/FModSound.cpp:91-127` | suffix/category/language tables | audio data |
| `src/engine/input/VirtualInput.cpp:41` | `VirtualKey keys[48]` with typed `condition` function pointers | real typed callback table; likely original |
| `src/engine/render/BuildResourceList.cpp:20-2510` | generated `Resource *resources[]` (2488 entries) and 152 image resources | generated from the binary by `_work/reconstructions/gameresource_gen/gen_brl.py`; original TU is `Classes/Game/GameResource.cpp` per `tu_mapping.md:45` |

**Proposed:** leave these as arrays. If a table is touched, extract the original bytes from
`.rodata`/`.data` and compare, not just the function's `linked_exact` row. The `kaamoSpecialItems`
case proves that a 100% row can hide wrong table bytes.

## F8. Real typed callback tables (keep)

These are source-like and should not be "cleaned up":

- `src/engine/input/VirtualInput.cpp:34,41`: `VirtualKey::condition` and `keys[48]` — typed
  `int (*)(int)` callbacks.
- `src/engine/file/TokenStruct.h:9-16`: `ConfigTokenReadFunction` callback + context.
- `src/engine/render/Engine.h:30-32,186,261,321`: destroy/initialize callbacks.
- `src/engine/core/ApplicationManager.h:30-38,51-55`: loading/resume/quit callbacks.
- `src/game/core/CheatHandler.h:12-19`: cheat callback + context.
- `src/engine/render/Image.cpp:22,29` and `PaintCanvas.h:34-35`: image-load callbacks.
- `src/game/core/CharacterImages.h:73-90`: `CHAR_IMAGES[63]` pointer table — real data, not a
  function-pointer table.

## F9. `BuildResourceList` generated table

- `src/engine/render/BuildResourceList.cpp` is 2955 lines, generated by
  `_work/reconstructions/gameresource_gen/gen_brl.py` from the original binary.
- `Resource *resources[]` starts at line 20 and contains 2488 `new Resource(...)` entries; the
  `SetResourceList(resources, 2488)` call is at line 2510; a 152-entry image loop is at line 2513.
- `_work/original_layout/tu_mapping.md:45` says the original TU is
  `Classes/Game/GameResource.cpp`, and the original `GameResource.cpp` also owns
  `loadPortraits` and `loadLowTexturesAndMaterials`.
- **Observation:** this is generated, not hand-maintained. **Inference:** it is a valid
  reconstruction of a large original data table, but its TU split is not original.
- **Proposed:** keep the generated table; if TU membership is the goal, move it into a
  `GameResource.cpp`-shaped TU together with `loadPortraits`/`loadLowTexturesAndMaterials`, one
  merge per pass under the full gate. Do not rewrite the 2488 entries by hand.

## Bounded work packages

| Priority | Package | Files | Expected risk |
|---|---|---|---|
| P0 | Fix `Status::missionCompleted` semantics (non-zero ranking table or direct switch) | `Status.cpp` | high: `.bss`/`.rodata` coupling; CubeMapping anchor already removed (`1ed51c3c`) |
| P0 | Fix the three wrong lookup tables | `Agent.cpp`, `SolarSystem.cpp`, `Generator.cpp` | medium: `.rodata` bytes, local data not gate-checked |
| P1 | Remove dead thunk structs / `RefreshFn` | `RefreshThunk.h`, `ModuleTransitionThunk.h`, `MenuTouchWindow.h/.cpp` | low: no codegen if unused |
| P1 | Remove dead `StarMap` globals and unused typedefs/arrays | `StarMap.cpp`, `LevelScript.cpp`, `SpaceLounge.cpp`, `Level.cpp`, `MenuTouchWindow.cpp`, `Hud.cpp` | low/medium: `.bss` shrink, layout |
| P2 | Replace member-pointer bool casts with real `bool` methods | `AEFile.cpp/.h`, `FileInterface*.h`, `ModStation.cpp`, `MissionsWindow.cpp`, `Hud.cpp`, `MenuTouchWindow.cpp` | medium: one site per verify pass |
| P2 | Replace `Sparks` function-pointer indirection | `Sparks.cpp` | medium: current 100% row |
| P2 | Build a local-table extractor and compare all file-static tables | tool + `_work/reconstructions/` | low code risk, high audit value |
| P3 | Merge generated `BuildResourceList` into `GameResource.cpp` shape | `BuildResourceList.cpp`, `LowResourceTable.cpp`, `ResourceMaterial.cpp` | high: TU/link layout |

## Commands used (reproducible)

```sh
readelf -sW ../_work/bins/android_2.0.16_libgof2hdaa.so | rg '_ZN6Status16missionCompletedEbbx|_ZN5Agent21getModPricePercentageEv|_ZN11SolarSystem13getAttackRaceEv|_ZN9Generator18isKaamoSpecialItemEi'
llvm-objdump -d --triple=thumbv7-none-linux-gnueabi --start-address=0xabf38 --stop-address=0xac4e8 \
  ../_work/bins/android_2.0.16_libgof2hdaa.so
llvm-objdump -d --triple=thumbv7-none-linux-gnueabi --start-address=0x177738 --stop-address=0x177760 \
  ../_work/bins/android_2.0.16_libgof2hdaa.so
llvm-objdump -d --triple=thumbv7-none-linux-gnueabi --start-address=0x155708 --stop-address=0x155724 \
  ../_work/bins/android_2.0.16_libgof2hdaa.so
llvm-objdump -d --triple=thumbv7-none-linux-gnueabi --start-address=0x95404 --stop-address=0x9542a \
  ../_work/bins/android_2.0.16_libgof2hdaa.so
python3 - <<'PY'
import struct
b = open('../_work/bins/android_2.0.16_libgof2hdaa.so', 'rb').read()
for addr, n in [(0x1fb8e8, 4), (0x1fb878, 4), (0x1fb590, 9)]:
    print(hex(addr), struct.unpack_from('<%di' % n, b, addr))
PY
```

Note: in this environment `llvm-objdump -s --section=.rodata --start-address=...` ignored the
start/stop range and dumped the whole section. The direct Python reads above are the reliable
table extraction path.

## Bottom line

The decomp is no longer mainly missing code; it is now vulnerable to **silent data errors**.
`Status::missionCompleted` and the three zero lookup tables are concrete, independently fixable
bugs. The remaining function-pointer and anchor constructs are mostly scaffolding: keep the anchors
until the real code is recovered, but delete the dead thunk/typedef/global layer and replace the
member-pointer bool casts with real typed methods one site at a time.
