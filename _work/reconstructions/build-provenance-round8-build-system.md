# Build provenance round 8: per-TU object layout in the original binary

Status: **research note only**, 2026-11-11. Worktree `wt/w11320`, branch
`wave168-w11320`, base master `631c26f0b`. Scratch `/dev/shm/w11320`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(2,187,328 B, sha256 `d4c17be6…bed9483`).

Gate on entry and exit (fresh `cmake-build-match`, `check -j2` + `diff`):
**RATCHET PASS**, `byte_exact 4507/4524`, `linked_exact 4507/4524`,
size-exact 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0,
symbol parity 5370/5370 (0 extra / 0 missing). `.text` **82,526** differing
bytes, `.ARM.extab` **670**, `.ARM.exidx` **0**, `.rel.plt` **0**
(byte-identical), `.note.gnu.build-id` 20. All 27 section sizes exact; file
size 2,187,328 B.

This round verifies round 7's build-system conclusion from a **new angle**:
the original's **per-TU object layout** — function order, `.rodata` order and
`.data` order — reconstructed from the stripped binary and compared with
`tools/source_order.txt` and the source-layout dumps. It also audits the
per-TU include-path / `-D` / `-include` question. No `src/`, CMake, baseline,
allowlist or doc change is landed; this note (and its audit script) is the
only commit.

---

## 0. Bottom line

1. **The original's per-TU function order is exactly `tools/source_order.txt`.**
   All **4,524** exported functions have the same name at the same address as
   our gold-map build (4,524/4,524 name match). The 190 TUs with code form a
   **monotonic subsequence** of `source_order.txt` with **0 breaks**; the only
   interleaving is the three `.text.startup` sections (Engine, NdkAutosave,
   ParticleSettings), which gold places at the front of `.text`. All **9**
   local (non-exported) function sections are byte-identical; excluding the
   three startup sections, their TU order is non-decreasing.
2. **The original's per-TU `.rodata` order is exactly `source_order.txt`.**
   The 114 non-mergeable `.rodata` contributions (109 TUs, **128,047 B**) are
   contiguous per TU, monotonic in `source_order.txt` with **0 breaks**, and
   **0 bytes differ** from the original. The 120,665-byte merged string pool
   is also byte-identical, but its internal order is gold's string-merge
   order, not TU order (see §3.2); it is not used as an order witness.
3. **The original's per-TU `.data` / `.data.rel.ro` / `.bss` order is exactly
   `source_order.txt`.** `.data.rel.ro` 90 contributions / 84 TUs / **6,684 B**,
   `.data` 64 / 64 / **10,495 B**, `.bss` 30 / 30 / **22,128 B**; all
   contiguous per TU, all monotonic with **0 breaks**, and `.data`/
   `.data.rel.ro` are **0 bytes different**. Exported data objects confirm
   this from the original's own `.dynsym`: every mapped object's TU sequence
   is monotonic (`.data.rel.ro` 165 mapped, `.data` 80, `.bss` 286,
   `.rodata` 85; 0 breaks).
4. **No TU was compiled with a different include path, `-D` macro or
   `-include` that is observable.** The source has no `#if` on `ANDROID`,
   `NDEBUG` or `DEBUG`; the only conditionals are `GOF2_MATCH` (not defined),
   `GOF2_ARC_NOEXCEPT` (source-defined), `__SIZEOF_POINTER__` (compiler-
   defined) and `__ANDROID__` (target-defined in `fmod.h`). `.init_array` has
   exactly **3** entries, all accounted for by the three
   `_GLOBAL__sub_I_*` functions; no forced include added a fourth. There is no
   app `__FILE__` full path in `.rodata` (only `main.c`'s hard-coded
   `"… in main.c: %s"` strings and libc++abi's `abort_message.cpp`), so the
   include-path spelling is unobservable — but any path/header difference that
   changed code or data would have broken the per-TU byte identity above.
5. **The source dumps witness TU membership, not emission order.** For the 133
   TUs with a dump witness, 2,751 of 2,800 dump function symbols are present
   in the Android binary; 2,704 are in the same TU. The 47 cross-TU cases are
   all inline/weak/template symbols (35 `Array*` helpers, 5 `KIPlayer`, 4
   `AbstractGun`, 3 `Matrix`/`operator*`). The dump symbol list is not a
   binary-order witness: e.g. the `ApplicationManager.cpp` dump starts with
   `VibrateEnable`, while the binary starts with the ctor and puts
   `VibrateEnable` near the end.

---

## 1. Method: reconstructing per-TU layout from a stripped `.so`

The original is stripped (no `.symtab`/`.strtab`/`.debug_*`), so per-TU
attribution cannot come from local symbol names. The layout was reconstructed
from four independent binary observables:

| observable | what it proves |
|---|---|
| `.dynsym` (4,524 defined FUNC, 843 defined OBJECT) | exported names and addresses |
| `.ARM.exidx` (3,566 entries, 28,528 B) | function-section start addresses and unwind data, in link order |
| section contents (`.text`, `.rodata`, `.data`, `.data.rel.ro`, `.init_array`) | the bytes at each address |
| a gold `-Map` of the canonical build | which object/TU owns each address |

The gold map was produced by re-running the exact canonical link command with
`-Map=/dev/shm/w11320/gold.map` and an unstripped output
(`/dev/shm/w11320/map.so`). It lists every input section, its output address,
size and owning object, plus every symbol address. The map's section
addresses are identical to the shipped `.so` (same link inputs, same order);
the map's bytes are used only to compare against the original at the same
virtual address.

The audit script is committed as
`_work/reconstructions/round8_layout_audit.py`. It reproduces every table
below from `android_2.0.16_libgof2hdaa.so`, the gold map and
`tools/source_order.txt`.

### 1.1 Why the map attribution is safe

- For exported functions, the original `.dynsym` itself gives the name at
  each address. All **4,524/4,524** names match the map's `.symtab` at the
  same address (0 misses), so the map's TU attribution for exported functions
  is direct, not inferred.
- For local functions, there are only **9** non-exported function sections
  (988 B). All 9 are byte-identical to the map at the same address, so the
  function at that address is the same body. `.ARM.exidx` is byte-identical
  (3,566 entries, 0 diff), so the function-section address sequence and
  unwind data match exactly.
- For data, every per-TU chunk's bytes at the map address are compared with
  the original; the result is 0 diff for `.rodata`, `.data.rel.ro` and
  `.data` (§3, §4). The chunk at that address is therefore the same data, not
  merely the same size.

---

## 2. Function order

### 2.1 Exported functions

`readelf -sW` on the original gives 4,524 defined `FUNC` symbols. Sorting by
address and looking up each address in the map:

```
exported functions                         4524
name matches map .symtab at same address    4524 / 4524
app TUs with exported functions             190
TU sequence monotonic in source_order       yes, 0 breaks
```

The 242 exported functions not in `src/` are the statically linked zlib /
libzip / libc++abi / libgcc functions; they are not part of the app TU order.

### 2.2 All function sections (including local functions)

The map has **3,946** non-empty `.text` function sections from the 192 app
TUs (1,167,952 B). Of these:

```
function sections                           3946
sections byte-identical to original         3931
sections with any diff                      15
diff bytes                                  82526
local (non-exported) function sections      9  (988 B, 0 diff)
```

The 15 differing sections are exactly the known residual functions (17
exported symbols, C1/C2 aliases share a section), in 13 TUs:

| diff | addr | size | TU | function |
|---:|---:|---:|---|---|
| 127 | 0x971b8 | 1,996 | `src/game/mission/Generator.cpp` | `Generator::createMission` |
| 722 | 0xabf38 | 1,432 | `src/game/mission/Status.cpp` | `Status::missionCompleted` |
| 5,435 | 0xaffd8 | 17,172 | `src/game/world/Level.cpp` | `Level::createMission` |
| 1,560 | 0xc0214 | 2,764 | `src/game/world/Level.cpp` | `Level::createShip` |
| 5,658 | 0xd1028 | 7,072 | `src/game/ui/Layout.cpp` | `Layout::Layout` |
| 43 | 0xd52d0 | 1,008 | `src/game/menu/ModStation.cpp` | `ModStation::ModStation` |
| 28 | 0x129bd4 | 1,896 | `src/game/ui/MenuTouchWindow.cpp` | `MenuTouchWindow::OnTouchBegin` |
| 7,487 | 0x12e050 | 9,908 | `src/game/weapons/Radar.cpp` | `Radar::draw` |
| 54,031 | 0x137d18 | 57,232 | `src/game/world/LevelScript.cpp` | `LevelScript::process` |
| 2,245 | 0x1490d8 | 9,700 | `src/game/ui/HangarWindow.cpp` | `HangarWindow::render` |
| 463 | 0x152f90 | 1,984 | `src/game/weapons/Gun.cpp` | `Gun::calcCharacterCollision` |
| 762 | 0x163b90 | 9,332 | `src/game/ui/Hud.cpp` | `Hud::draw` |
| 175 | 0x170d38 | 1,948 | `src/game/menu/SpaceLounge.cpp` | `SpaceLounge::drawLounge` |
| 3,775 | 0x17a144 | 8,080 | `src/game/menu/MGame.cpp` | `MGame::OnTouchEnd` |
| 15 | 0x17c8d8 | 11,440 | `src/game/menu/MGame.cpp` | `MGame::OnUpdate` |

Every other function body — including all 9 local functions — is
byte-identical at its original address. That is the strongest possible
statement that the original's function layout (TU order and intra-TU order)
is the map's layout, and the map's layout is `source_order.txt`.

### 2.3 TU sequence

Excluding the three `.text.startup` sections (gold places them at the front of
`.text`), the 190 TUs with normal code form a monotonic subsequence of
`source_order.txt` with **0 breaks**:

```
src/platform/android/BackButton.c           0x069578
src/engine/core/NFC.cpp                     0x069588
src/engine/input/VirtualInput.cpp           0x069e5c
...
src/engine/render/ParticleSystemManager.cpp 0x18357c
src/engine/render/ParticleSettings.cpp      0x183e68
src/engine/render/ParticleSystemMesh.cpp    0x185490
```

The three startup sections are:

```
0x69440  128 B  Engine.cpp          _GLOBAL__sub_I_Engine.cpp
0x694c0   28 B  NdkAutosave.cpp     _GLOBAL__sub_I_NdkAutosave.cpp
0x694dc   88 B  ParticleSettings.cpp _GLOBAL__sub_I_ParticleSettings.cpp
```

They are the only interleaving in `.text` (the same three TUs also have
normal code at their `source_order` positions). `.ARM.exidx` is byte-identical
(3,566 entries, 0 diff), which independently fixes the function-section
address sequence and unwind data.

---

## 3. `.rodata` order

### 3.1 Non-mergeable per-TU chunks

Gold splits `.rodata` into a merged string pool (`** merge strings`, the
first 0x1d759 = 120,665 B) followed by the per-object `.rodata` chunks. The
per-TU chunks are the reliable TU-order witness:

```
contributions                    114
TUs                              109
bytes                            128047
bytes different from original    0
TUs contiguous per TU            yes (0 interleaved)
TU sequence monotonic            yes (0 breaks)
```

First/last examples:

```
src/engine/file/FileInterfaceAndroid.cpp   0x1e7570..0x1e7597   39 B
src/engine/file/AEFile.cpp                 0x1e7598..0x1e79d1 1067 B
src/game/core/String.cpp                   0x1e79e0..0x1e79f7   23 B
...
src/engine/render/IParticleSystem.cpp      0x206dc0..0x206dd2   18 B
src/engine/render/ParticleSystemSprite.cpp 0x206de0..0x206df7   23 B
src/engine/render/ParticleSystemMesh.cpp   0x206e00..0x206e15   21 B
```

The original's bytes at every one of these addresses equal the map's bytes
(0 diff), so the original's per-TU `.rodata` chunks are exactly the chunks
emitted by the TUs in `source_order.txt`.

### 3.2 The merged string pool is not a TU-order witness

The 120,665-byte `** merge strings` block is byte-identical to the original,
but gold's string merging is not a simple input-order concatenation. Of the
98 TUs with a `.rodata.str1.1` section, 61 have their full section bytes contiguous
in the pool and 37 are not found as a contiguous block (deduplicated/split); the first-string positions are
non-monotonic for 7 pairs (mostly the shader zoo). This is expected: gold
merges `.rodata.str1.1` by content, not by TU. The pool is still a useful
byte-identity check, but it is **not** used to infer TU order. The
non-mergeable chunks in §3.1 are.

---

## 4. `.data`, `.data.rel.ro`, `.bss` order

```
section        contributions  TUs  bytes   diff  contiguous  monotonic
.data.rel.ro   90             84   6684    0     yes         0 breaks
.data          64             64   10495   0     yes         0 breaks
.bss           30             30   22128   n/a   yes         0 breaks
```

`.bss` is `NOBITS` (no file bytes); its layout is witnessed by the map's
addresses/sizes and by the original's exported `.bss` objects (§4.1).

First/last examples:

```
.data.rel.ro:
  FileInterfaceAndroid.cpp   0x20cbb0..0x20cc24   116 B
  AEFile.cpp                 0x20cc24..0x20cc8c   104 B
  String.cpp                 0x20cc8c..0x20cca4    24 B
  ...
  IParticleSystem.cpp        0x20e54c..0x20e578    44 B
  ParticleSystemSprite.cpp   0x20e578..0x20e5a8    48 B
  ParticleSystemMesh.cpp     0x20e5a8..0x20e5d8    48 B

.data:
  NFC.cpp                    0x214004..0x214008     4 B
  VirtualInput.cpp           0x214010..0x2150bc  4268 B
  Mesh.cpp                   0x2150bc..0x2150c0     4 B
  ...
  Radio.cpp                  0x215f24..0x2163fc  1240 B
  DialogueWindow.cpp         0x2163fc..0x2168d4  1240 B
  MGame.cpp                  0x2168e0..0x21691c    60 B
  ParticleSettings.cpp       0x21691c..0x216920     4 B
```

`.data.rel.ro` is especially strong: it is the vtable/typeinfo stream, which
is emitted per TU in source order. 90 contributions from 84 TUs, all
contiguous, all monotonic, 0 bytes different. A different TU order would
permute the vtables.

### 4.1 Exported data objects (original `.dynsym`)

The original exports 843 data objects. Mapping each object's address to the
map's owning TU gives a direct original-binary TU sequence:

```
section        exported objects  mapped  TU sequence monotonic  name match at same addr
.rodata        182               85      yes, 0 breaks          134/182
.data.rel.ro   277               165     yes, 0 breaks          277/277
.data          82                80      yes, 0 breaks          80/82
.bss           302               286     yes, 0 breaks          285/302
```

The unmapped `.rodata`/`.data.rel.ro` objects are merged typeinfo names and
zlib/libc++abi data (no per-TU map ownership); the unmapped `.data`/`.bss`
objects are archive data (`__cxa_terminate_handler`, `__cxa_unexpected_handler`,
zlib/Android globals) outside the app TU map. Every mapped object is
monotonic. The `.data` name mismatches are bitfields (`UseAdvancedShader`,
`showNewCreditsMenu`) whose address is inside a larger object; the `.bss`
mismatches are bitfields likewise. These are not layout differences.

---

## 5. `.init_array` and the `-include` question

The original `.init_array` is 12 bytes, exactly 3 pointers, byte-identical to
the map:

```
0x20fb98 -> 0x69440  _GLOBAL__sub_I_Engine.cpp
0x20fb9c -> 0x694c0  _GLOBAL__sub_I_NdkAutosave.cpp
0x20fba0 -> 0x694dc  _GLOBAL__sub_I_ParticleSettings.cpp
```

The map has exactly these three `_GLOBAL__sub_I_*` functions and no others.
A forced include (`-include`) that contains a static initializer would add a
fourth entry; one that contains a string literal or data object would change
`.rodata`/`.data`. Neither is present. A no-op forced include is
unobservable, but no observable forced include exists.

`.fini_array` is 8 bytes, entirely from `crtbegin_so.o` (no app TU).

---

## 6. Include path and `-D` audit

### 6.1 Source conditionals

`grep` over `src/` for `#if`/`#ifdef`/`#ifndef`/`#elif` finds:

- include guards (`GOF2_*_H`, `GALAXYONFIRE2_*_H`) — no codegen effect;
- `__SIZEOF_POINTER__ == 4` — compiler-defined, identical for every TU;
- `#ifdef GOF2_MATCH` in `Transform.h`/`KeyFrame.h` — **not defined** in the
  build (the drift tool notes it would activate a known-failing assert);
- `#ifdef GOF2_ARC_NOEXCEPT` in `Array.h`/`ArrayReleaseClasses.h` — defined
  by `PaintCanvas.cpp` itself, not a build flag;
- `__ANDROID__` in `third_party/fmod/inc/fmod.h` — target-defined by
  `-target armv7-none-linux-androideabi16`, identical for every TU.

There is **no** `#if` on `ANDROID`, `NDEBUG`, `DEBUG` or any other candidate
per-TU macro. A per-TU `-D` would therefore have to be codegen- and
data-neutral on all 4,507 exact functions and 843 exact data objects. None is
witnessed.

### 6.2 `__FILE__` / `__DATE__` / `__TIME__`

The original `.rodata` contains only three app `__FILE__`-like strings:

```
"dataDirectory in main.c: %s"
"rootPath in main.c: %s"
"zipDirectory in main.c: %s"
```

These are hard-coded strings in `main.c`, not `__FILE__` expansions; they
witness the file name `main.c`, not the include path. The only other source
path in `.rodata` is libc++abi's
`/Volumes/Android/buildbot/.../abort_message.cpp`, from the prebuilt archive.
There are no `__DATE__`/`__TIME__` strings. The include-path spelling is
therefore not directly observable.

The dumps' `headers_seen` roots are the expected original roots
(`AE2/Classes`, `Classes/Game`, `Classes/Game/ParticleSystems`,
`Classes/Game/MeshMerger`, `Classes/Shader`, `Classes/zlib`,
`FMOD/v44442/inc`). Our tree uses the same header set under `src/` and
`third_party/`; only the path spelling differs. Since no app `__FILE__` path
survives, the spelling cannot be pinned from the binary — but the header
*content* is pinned by the byte identity in §3–§5.

### 6.3 What the byte identity rules out

Any per-TU include-path, `-D` or `-include` difference that changed a header
choice, a macro expansion, a string literal, a static initializer or a data
object would show up in at least one of:

- the per-TU `.rodata` chunks (128,047 B, 0 diff);
- the per-TU `.data.rel.ro` vtables/typeinfo (6,684 B, 0 diff);
- the per-TU `.data` statics (10,495 B, 0 diff);
- the `.init_array` (12 B, 0 diff, exactly 3 entries);
- the 3,931 byte-identical function sections (including all 9 local
  functions).

All are clean. The only `.text` differences are the 15 known residual
function sections. So the effective per-TU compile environment is the same
for every TU; any remaining difference is unobservable by construction.

### 6.4 Our build's per-TU flags are uniform

For completeness, `cmake-build-match/compile_commands.json` shows that the
canonical build itself applies the same flags to every TU:

```
192/192  -DANDROID -DNDEBUG -D__ANDROID_API__=16
188/188  C++ TUs: -std=c++11
  4/4    C TUs: clang default (gnu11), no -std
192/192  same sysroot / android-support include paths
188/188  C++ TUs: same libc++ / libc++abi include paths
```

The only per-file deviations are the documented matching knobs (StarMap and
SpaceLounge branch weights, LevelScript post-RA machine sink, Radar post-RA
machine LICM, Level EarlyCSE), which are not original build flags. The
original's per-TU layout matching this build therefore also matches a uniform
compile line.

---

## 7. Source dumps: membership, not order

The dumps are iOS builds, not the Android link. They are authoritative for
**which functions belong to which TU**, but their symbol-list order is not a
binary emission-order witness. Example: the `ApplicationManager.cpp` dump
starts with `VibrateEnable`, `GetCurrentApplicationModule`,
`SetApplicationModule`, … while the Android binary starts with the ctor and
the `Array<...>` helpers and puts `VibrateEnable` near the end.

Membership check over the 133 TUs with a dump witness:

```
dump function symbols checked                 2800
found in the Android binary                   2751
found in the same TU                          2704
found in a different TU                       47
not found (iOS-only / inlined / renamed)      49
```

The 47 cross-TU cases are all inline/weak/template symbols:

```
35  Array* helpers (ArrayAdd/ArraySetLength/ArrayReleaseClasses/Array<T>::...)
 5  KIPlayer inline methods
 4  AbstractGun inline methods
 3  Matrix::Matrix / operator*
```

These are exactly the symbols a compiler may emit weakly in several TUs and
the linker may keep from a different TU; they do not contradict the
`source_order` layout. For non-inline functions, the dump TU membership and
the map TU membership agree.

---

## 8. Conclusion

Round 7 concluded that the original was a stock NDK r18b CMake-toolchain build
with one explicit source list in the original Android link order, default
visibility, no version script, C++11, and uniform per-TU flags. Round 8
confirms that conclusion from the object-layout side:

- The original's **function order** (4,524 exported names, 3,946 function
  sections, 3,566 `.ARM.exidx` entries) is exactly the checked-in
  `tools/source_order.txt`; the only `.text` differences are the 15 known
  residual function sections (17 exported functions).
- The original's **`.rodata`**, **`.data.rel.ro`**, **`.data`** and **`.bss`**
  per-TU chunks are contiguous, monotonic in `source_order.txt`, and (for the
  file-backed sections) byte-identical to the canonical build.
- The original's **`.init_array`** has exactly the three expected
  `_GLOBAL__sub_I_*` entries; no forced include added a fourth.
- No per-TU include-path, `-D` or `-include` difference is observable; any
  such difference would have to be codegen- and data-neutral on every exact
  function and data object.
- The source dumps confirm TU membership for non-inline functions; their
  symbol order is not a link-order witness.

Confidence: the layout conclusion is **high** (direct byte identity at every
per-TU chunk plus 4,524/4,524 exported name/address matches). The exact
build-file text remains unrecoverable (as in round 7); what is recovered is
the effective compile/link layout, and it is exactly the checked-in
`source_order.txt`.

---

## 9. Reproduction

```bash
# 1. Build the canonical match tree and keep the gold map.
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2

# 2. Re-run the link with -Map and an unstripped output (scratch).
cd cmake-build-match
ninja -t commands libgof2hdaa.so | tail -1 > /dev/shm/w11320/linkcmd.txt
# (script in the note's audit tool edits the -o target and inserts -Map=...)
python3 _work/reconstructions/round8_layout_audit.py \
    --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
    --map /dev/shm/w11320/gold.map \
    --map-so /dev/shm/w11320/map.so \
    --source-order tools/source_order.txt

# 3. Gate.
cmake --build cmake-build-match --target check -j2
```
