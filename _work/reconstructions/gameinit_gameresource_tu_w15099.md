# GameInit.cpp / GameResource.cpp TU recreation audit (w15099)

Scope: verify that the original `Classes/Game/GameInit.cpp` and `Classes/Game/GameResource.cpp`
translation units are recreated in the tree under the full gate, using the source-layout dumps and
binary `.text`/`.rodata`/string-adjacency evidence. Baseline branch `wave193-w15099`, HEAD
`bac641cc9`, original binary `../_work/bins/android_2.0.16_libgof2hdaa.so` (Ghidra image base
`0x10000`).

## 1. Baseline gate

```bash
cd /home/debian/GalaxyOnFire2/wt/w15099
cmake --build cmake-build-match --target check -j4
```

Result (matches the documented baseline exactly):

```
verify: byte 4508  linked 4508  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`byte_exact 4508/4524`, `linked_exact 4508/4524`, size `4524/4524`, data `843/843`,
imports `207`, lint CLEAN, drift 0.

## 2. TU membership from the authoritative dumps

`_work/original_layout/original_layout.json` -> `tu_symbols["gof2hd_1.1.19_ios"]`:

```text
Classes/Game/GameInit.cpp  (object GameInit-5d326cd789d7550198c2c5758579608c.o)
  _OnDestroyApplication(AbyssEngine::Engine*)
  _OnCreateApplication(AbyssEngine::Engine*)
  _loadingScreen(AbyssEngine::PaintCanvas*, int, void*)
  _cheatFunction(int, void*)
  _GameData::~GameData()
  _GameData::GameData()
  headers: Classes/Game/GameData.h

Classes/Game/GameResource.cpp  (object GameResource-d6dee39462cc8dc5a2ac265e80401c60.o)
  _BuildResourceList(AbyssEngine::Engine*)
  _loadPortraits(AbyssEngine::Engine*)
  _loadLowTexturesAndMaterials(AbyssEngine::Engine*)
  _AbyssEngine::ResourceTexture::ResourceTexture(AbyssEngine::String const&, float)
  headers: AE2/Classes/PaintCanvas.h
```

The SD 1.0.1 dump corroborates the same file ownership and additionally names
`ResourceMesh::ResourceMesh(char const*, unsigned short, bool)` and
`ResourceTexture::ResourceTexture(char const*, float)`; the Android HD build contains the latter
too (address/size table below). `loadLowTexturesAndMaterials` is HD-only.

Existing source (no duplicate owner; verified by grep across `src/`):

- `src/game/core/GameInit.cpp` defines `loadingScreen`, `OnDestroyApplication`,
  `OnCreateApplication`, file-static `cheatFunction`, and (via the implicit `GameData` methods)
  `GameData::GameData()` / `GameData::~GameData()`.
- `src/engine/render/GameResource.cpp` defines `BuildResourceList`, `loadPortraits`,
  `loadLowTexturesAndMaterials`, and both `ResourceTexture` constructors.
- `tools/source_order.txt`: line 116 `src/game/core/GameInit.cpp`, line 127
  `src/engine/render/GameResource.cpp` (original TU link order preserved).

## 3. Per-symbol proof (original vs ours)

```bash
O=../_work/bins/android_2.0.16_libgof2hdaa.so
N=cmake-build-match/libgof2hdaa.so
readelf -sW "$O" | grep -E '<the 10 symbols>'
readelf -sW "$N" | grep -E '<the 10 symbols>'
```

Addresses and sizes are identical in both binaries:

| symbol | addr | size |
|---|---:|---:|
| `_Z13loadingScreen...` | 0xd03e5 | 736 |
| `_Z20OnDestroyApplication...` | 0xd06c5 | 48 |
| `_ZN8GameDataD2Ev` (WEAK) | 0xd06f5 | 68 |
| `_Z19OnCreateApplication...` | 0xd0739 | 1332 |
| `_ZN8GameDataC2Ev` (WEAK) | 0xd0c6d | 168 |
| `_Z17BuildResourceList...` | 0xe8f55 | 0x29d34 (171316) |
| `_ZN11AbyssEngine15ResourceTextureC2EPKcf` | 0x112c89 | 50 |
| `_Z13loadPortraits...` | 0x112d4d | 21562 |
| `_Z27loadLowTexturesAndMaterials...` | 0x118189 | 13642 |
| `_ZN11AbyssEngine15ResourceTextureC2ERKNS_6StringEf` | 0x11b6d3 | 60 |

`cmake-build-match/verify/report.json` rows for all ten are `pct=100.0`, `byte=true`,
`linked=true`, `osize==usize`.

## 4. Whole-TU region proof (not just symbols)

The `.text` regions between the neighbouring TUs are byte-identical, including intra-TU padding.
`GameInit` starts one byte before `loadingScreen` (its Thumb `push` alignment byte at `0xd03e4`) and
ends where the first `Sprite` function starts (`0xd0d19`). `GameResource` starts at `0xe8f54` and
ends before `Node::Node` (`0x11b711`).

```bash
O=../_work/bins/android_2.0.16_libgof2hdaa.so; N=cmake-build-match/libgof2hdaa.so
cmp -n $((0xd0d18-0xd03e4)) -i $((0xd03e4)):$((0xd03e4)) "$O" "$N"   # -> IDENTICAL
cmp -n $((0x11b70e-0xe8f54)) -i $((0xe8f54)):$((0xe8f54)) "$O" "$N" # -> IDENTICAL
```

## 5. `.rodata` / string adjacency proof

Both `.rodata` sections are `PROGBITS 001c9e10 1c9e10 041b67`, and the full section content is
identical:

```bash
cmp -n $((0x41b67)) -i $((0x1c9e10)):$((0x1c9e10)) "$O" "$N"  # -> IDENTICAL
```

`grep -aboF` on both binaries returns the same file offsets for the TU-adjacent literals:

| literal | original file offset | ours |
|---|---:|---:|
| `data/textures/pow_texture.aei` | 1880132 | 1880132 |
| `data/assets/main/3d/textures/low/etc/fx/cloak_map.aei` | 1880553 | 1880553 |

The GameInit cheat/`pow_texture` literals end immediately before GameResource's first literal,
which confirms the original source-file adjacency in `.rodata` is preserved.

## 6. Proven vs hypothesis

**PROVEN**

- The `GameInit.cpp` and `GameResource.cpp` TU membership matches the authoritative HD iOS dump.
- All ten original symbols have identical addresses, sizes, and 100% byte/linked match.
- The complete `.text` byte ranges covering both TUs are byte-identical to the original,
  including alignment/padding.
- Full `.rodata` is byte-identical and the GameInit -> GameResource literal order matches.
- `tools/source_order.txt` keeps the original TU link order.
- No source change is required for this topic; the baseline gate is unchanged.

**HYPOTHESIS / residuals (not codegen-affecting here)**

- `cheatFunction(int, void*)` is a file-local, empty helper in the Android build (absent from
  `.dynsym`); the iOS dump names it. Its body contributes no bytes and is not independently
  visible, so this is inferred from the dump + the empty-body source.
- `ModStationAlloc` placement `operator new` exists only to make the original 336-byte
  `ModStation` allocation reproducible while `ModStation.h` models a larger object; the 336-byte
  size is taken from the original call site, not from the dump.
- Stale comments, not gate-relevant: `_work/original_layout/tu_mapping.md` lines 16 and 210 still
  say JniBridge owns the GameInit callbacks; `src/game/core/GameInit.cpp` line 21 says
  `.text 0xd03e4..0xd0d14`, but the GameData ctor actually ends at `0xd0d15` and the next TU
  starts at `0xd0d19`. The range `cmp` above nevertheless shows byte identity. These are doc
  nits only and were intentionally not "fixed" in this pass because a source touch would force a
  full rebuild for no gate gain.

## 7. Conclusion

`GameInit.cpp` and `GameResource.cpp` are already fully recreated and byte-identical to the
original in the current baseline build. No source, flag, or baseline change was made; this commit
adds only this research note. Gate counts before == after: `byte_exact 4508/4524`,
`linked_exact 4508/4524`, size `4524/4524`, data `843/843`, imports `207`, lint CLEAN, drift 0.
