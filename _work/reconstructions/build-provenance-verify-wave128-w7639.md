# Build provenance verification, wave 128 (w7639)

Status: note-only research commit, 2026-11-07. Worktree
`/home/debian/GalaxyOnFire2/wt/w7640`, HEAD `4bbe88776` ("Generator::createMission
pass: amount/padA slot-form study (no source win)"). Scratch is `/dev/shm/w7640`.
No `src/`, `tools/`, `CMakeLists.txt`, baseline, allowlist or doc change is landed.

This note independently re-derives the original `libgof2hdaa.so` build recipe from
the shipped ELF and from the NDK r18b sources in `.cache/ndk/android-ndk-r18b/`,
rather than restating the wave-127 synthesis. Every number below was measured in
this worktree unless explicitly attributed to a prior note.

Gate at the base (fresh `cmake --build cmake-build-match --target check`):

```text
verify: byte 4506/4524  linked 4506/4524  size 4524/4524  avg 99.94
data:   exact 843/843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0  imports 207
symbol parity 5370/5370 (0 extra / 0 missing)
lint CLEAN ✓   drift 0
RATCHET PASS
```

Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`,
2,187,328 B, `sha256 d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`.
Our fresh canonical link: 2,187,328 B,
`sha256 597d36aac326e3891eae9ebf41a0b2dfc73632a947d6584b89a582caa4343c69`.

---

## 0. Bottom line

1. **The toolchain is proven from the ELF, not inferred.** `.comment` is exactly
   the NDK r18b clang 7.0.2 string plus the OpenSSL GCC 4.9.x string;
   `.note.android.ident` decodes to API 16, `r18b`, `5063045`;
   `.note.gnu.gold-version` is exactly `gold 1.12`; `e_flags = 0x5000200`
   (EABI5 + soft-float); `.ARM.attributes` is exactly Thumb-2, ARMv7-A,
   VFPv3, NEONv1, GOT-indirect, wchar_t 4.
2. **The original was built with the CMake toolchain's compile flags, not
   vanilla ndk-build.** The NDK r18b CMake toolchain adds
   `-ffunction-sections -funwind-tables -fstack-protector-strong` and **no**
   `-fdata-sections`; ndk-build's `GLOBAL_CFLAGS` adds `-fdata-sections`.
   A full controlled A/B (same objects, only `-fdata-sections` added) gives:
   original `.rodata`/`.data`/`.data.rel.ro` **byte-identical to the
   no-`-fdata-sections` build**, and `-fdata-sections` changes `.rodata` by
   113,935 bytes, `.data` by 3,023 bytes, `.data.rel.ro` by 365 bytes and costs
   **191 byte_exact** (4506 → 4315).
3. **The link/strip recipe is proven.** A gold link with the explicit default
   set (`--hash-style=sysv --no-as-needed -z combreloc --spare-dynamic-tags=5
   -z relro -z noexecstack -z max-page-size=4096 -z common-page-size=4096`)
   is byte-identical to the repo link. The build-id is the SHA-1 of the
   **unstripped** gold output with the build-id descriptor zeroed. GNU
   `strip --strip-all` preserves that build-id; gold `-s` does not. The
   canonical `.so` is exactly GNU-strip + `tools/reorder_shstrtab.py`.
4. **No tested flag improves any of the 18 residual functions.** `-fpic`,
   `-fdata-sections` and `-fno-rtti` leave the residual percentages unchanged;
   `-g0` changes four of them and makes all four worse. The residual gaps are
   source shape, not build configuration.
5. **New contradictions / corrections to the prior synthesis:**
   - `.ARM.exidx` has **272** CANTUNWIND entries, not 194. (Total 3,566 entries:
     2,438 extab references, 856 compact, 272 CANTUNWIND.)
   - `objcopy --strip-all` is **byte-identical** to GNU `strip --strip-all` in
     this NDK r18b (same sha256, same build-id), contrary to the synthesis's
     claim that objcopy's build-id differs.
   - The docs' `.text`/`.ARM.extab` differing-byte counts are stale at this
     HEAD: the fresh measurement is `.text` **85,520** and `.ARM.extab`
     **1,436** (not 85,496/1,438 or 85,552/1,438).

---

## 1. Artifacts and exact commands

```sh
O=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
M=/home/debian/GalaxyOnFire2/wt/w7640/cmake-build-match/libgof2hdaa.so
R=/home/debian/GalaxyOnFire2/wt/w7640/.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-readelf
sha256sum "$O" "$M"
```

```text
d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483  original
597d36aac326e3891eae9ebf41a0b2dfc73632a947d6584b89a582caa4343c69  ours
2187328  original
2187328  ours
```

Build commands used:

```sh
cmake --preset default
cmake -S . -B cmake-build-match \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs \
  -DGOF2_MATCH_OPT=-Oz
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6
```

(The nested worktree needs the explicit `GOF2_ORIGINAL_SO`/`GOF2_FMOD_LIBDIR`
override; the preset's default `../_work/...` resolves to `wt/_work` here.)

---

## 2. ELF header, notes, comment, attributes

### 2.1 ELF header

```sh
"$R" -hW "$O"
```

```text
Class:                             ELF32
Data:                              2's complement, little endian
Type:                              DYN (Shared object file)
Machine:                           ARM
Flags:                             0x5000200, Version5 EABI, soft-float ABI
Number of program headers:         8
Number of section headers:         27
Section header string table index: 26
```

`0x5000200` = `EF_ARM_EABI_VER5 (0x05000000) | EF_ARM_ABI_FLOAT_SOFT (0x200)`.
The 8 program headers are `PHDR, LOAD(R E), LOAD(RW), DYNAMIC, NOTE,
GNU_STACK(RW), EXIDX, GNU_RELRO`; `GNU_STACK` has no `E` bit (noexecstack), and
`GNU_RELRO` covers `.fini_array .data.rel.ro .init_array .dynamic .got`.

### 2.2 `.comment` — exact

```sh
"$R" -p .comment "$O"
```

```text
[     1]  Android (4751641 based on r328903) clang version 7.0.2
          (https://android.googlesource.com/toolchain/clang 003100370607242ddd5815e4a043907ea9004281)
          (https://android.googlesource.com/toolchain/llvm 1d739ffb0366421d383e04ff80ec2ee591315116)
          (based on LLVM 7.0.2svn)
[   108]  GCC: (GNU) 4.9.x 20150123 (prerelease)
```

The clang string is byte-identical to the r18b package's clang:
`.cache/ndk/android-ndk-r18b/toolchains/llvm/prebuilt/linux-x86_64/bin/clang
--version` prints the same `Android (4751641 based on r328903) clang version
7.0.2 ...` line. The GCC string is the vendored OpenSSL 1.1.0l build, not the
app compiler.

### 2.3 `.note.android.ident` — exact

```sh
"$R" -x .note.android.ident "$O"
```

```text
0x00000134 08000000 84000000 01000000 416e6472 ............Andr
0x00000144 6f696400 10000000 72313862 00000000 oid.....r18b....
...
0x00000184 00000000 00000000 35303633 30343500 ........5063045.
```

Decoding the 132-byte descriptor:

```text
word[0] = 0x00000010              -> API level 16
bytes[4:8] = "r18b"               -> NDK release r18b
bytes[64:72] = "5063045"          -> Pkg.Revision 18.1.5063045
```

`source.properties` in the cached NDK says `Pkg.Revision = 18.1.5063045`.

### 2.4 `.note.gnu.gold-version` — exact

```text
Owner GNU, NT_GNU_GOLD_VERSION
Version: gold 1.12
```

The cached linker prints `GNU gold (GNU Binutils 2.27.0.20170315) 1.12`.

### 2.5 `.ARM.attributes` — exact

```sh
"$R" -AW "$O"
```

```text
Tag_CPU_name: "ARM v7"
Tag_CPU_arch: v7
Tag_CPU_arch_profile: Application
Tag_ARM_ISA_use: Yes
Tag_THUMB_ISA_use: Thumb-2
Tag_FP_arch: VFPv3
Tag_Advanced_SIMD_arch: NEONv1
Tag_ABI_PCS_GOT_use: GOT-indirect
Tag_ABI_PCS_wchar_t: 4
Tag_ABI_FP_denormal: Needed
Tag_ABI_FP_exceptions: Needed
Tag_ABI_FP_number_model: IEEE 754
Tag_ABI_align_needed: 8-byte
Tag_ABI_enum_size: int
Tag_ABI_HardFP_use: Deprecated
Tag_CPU_unaligned_access: v6
Tag_ABI_FP_16bit_format: IEEE 754
```

This is exactly the canonical build's `.ARM.attributes` (byte-identical,
0 differing bytes), and it pins `-march=armv7-a -mthumb -mfpu=neon
-mfloat-abi=softfp` independently of the compile command.

### 2.6 Section headers — all 27 exact

A Python parser of the section-header table compared every field
(`sh_name, sh_type, sh_flags, sh_addr, sh_offset, sh_size, sh_link, sh_info,
sh_addralign, sh_entsize`) of all 27 sections. Result: **27/27 headers
identical**, no extra section, file size exact (2,187,328 B). The only
differing payloads are `.text`, `.ARM.extab` and `.note.gnu.build-id`.

| section | addr | size | differing bytes |
|---|---:|---:|---:|
| `.note.android.ident` | 0x134 | 0x98 | 0 |
| `.note.gnu.build-id` | 0x1cc | 0x24 | **20** |
| `.dynsym` | 0x1f0 | 0x15ca0 | 0 |
| `.dynstr` | 0x15e90 | 0x2e318 | 0 |
| `.hash` | 0x441a8 | 0x973c | 0 |
| `.gnu.version` | 0x4d8e4 | 0x2b94 | 0 |
| `.gnu.version_d` | 0x50478 | 0x1c | 0 |
| `.gnu.version_r` | 0x50494 | 0x40 | 0 |
| `.rel.dyn` | 0x504d4 | 0x7048 | 0 |
| `.rel.plt` | 0x5751c | 0x72d0 | 0 |
| `.plt` | 0x5e7ec | 0xac4c | 0 |
| `.text` | 0x69440 | 0x1337c8 | **85,520** |
| `.ARM.exidx` | 0x19cc08 | 0x6f70 | 0 |
| `.ARM.extab` | 0x1a3b78 | 0x26298 | **1,436** |
| `.rodata` | 0x1c9e10 | 0x41b67 | 0 |
| `.fini_array` | 0x20cba0 | 0x8 | 0 |
| `.data.rel.ro` | 0x20cbb0 | 0x2fe8 | 0 |
| `.init_array` | 0x20fb98 | 0xc | 0 |
| `.dynamic` | 0x20fba4 | 0x148 | 0 |
| `.got` | 0x20fcf4 | 0x430c | 0 |
| `.data` | 0x214000 | 0x296c | 0 |
| `.bss` | 0x216970 | 0x69ac | NOBITS, size exact |
| `.comment` | 0 | 0x12f | 0 |
| `.note.gnu.gold-version` | 0 | 0x1c | 0 |
| `.ARM.attributes` | 0 | 0x38 | 0 |
| `.shstrtab` | 0 | 0x117 | 0 |

---

## 3. Dynamic section, `.init_array`, `.ARM.exidx`/`.ARM.extab`

### 3.1 `DT_NEEDED`, `DT_FLAGS`, hash style, spare tags

```sh
"$R" -dW "$O"
```

```text
NEEDED  liblog.so
NEEDED  libGLESv2.so
NEEDED  libGLESv1_CM.so
NEEDED  libEGL.so
NEEDED  libandroid.so
NEEDED  libfmodex.so
NEEDED  libfmodevent.so
NEEDED  libm.so
NEEDED  libdl.so
NEEDED  libc.so
SONAME  libgof2hdaa.so
FINI_ARRAY 0x20cba0 / 8
INIT_ARRAY 0x20fb98 / 12
FLAGS      BIND_NOW
FLAGS_1    NOW
RELCOUNT   1162
VERDEF     1
VERNEED    2
```

The original has `.hash` and **no `.gnu.hash`** (sysv hash). The dynamic
section is 0x148 = 41 entries; 35 are real tags, leaving **5 spare `DT_NULL`
tags** after the terminator, i.e. `--spare-dynamic-tags=5`. `DT_FLAGS=BIND_NOW`
and `DT_FLAGS_1=NOW` plus `.got` inside `PT_GNU_RELRO` prove `-z now -z relro`.
Our canonical link prints exactly the same `readelf -d` block.

### 3.2 `.init_array` — 3 entries, same targets

```sh
python3 - <<'PY'
import struct
d=open('/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so','rb').read()
print([hex(x) for x in struct.unpack_from('<3I',d,0x20eb98)])
PY
```

```text
['0x69441', '0x694c1', '0x694dd']
```

The low Thumb bit is set; the real entry addresses are 0x69440, 0x694c0,
0x694dc. `nm -n` on our unstripped link maps them to:

```text
0x69440  t _GLOBAL__sub_I_Engine.cpp
0x694c0  t _GLOBAL__sub_I_NdkAutosave.cpp
0x694dc  t _GLOBAL__sub_I_ParticleSettings.cpp
```

The original's `.init_array` bytes are byte-identical to ours (0 differing
bytes), so the same three initializers run in the same order.

### 3.3 `.ARM.exidx` — byte-identical

`.ARM.exidx` is 0x6f70 = **3,566 entries**, and the whole section is
byte-identical to the original (0 differing bytes). Parsing the entries:

```text
extab references : 2,438
compact entries  :   856
CANTUNWIND (=1)  :   272
total            : 3,566
```

The wave-127 synthesis says "194 empty-`.text` CANTUNWIND sentinels". That is
**not the total CANTUNWIND count** in the shipped ELF; the independent count is
272. (194 may be a subset defined some other way; the total is 272.)

### 3.4 `.ARM.extab` — size/base exact, 23 records / 1,436 bytes differ

`.ARM.extab` is 0x26298 = 156,312 B at 0x1a3b78, same as the original. Using the
byte-identical `.ARM.exidx` pointers as record boundaries, the section has
**2,438 records**. Only **23 records** differ, totalling **1,436 bytes**:

```text
extab off  len  diff   function
0x14d0     60    2    PaintCanvas::DrawStringColor(...)
0x150c     12    1    PaintCanvas::GetTextWidth(...)
0x3d48    172   26    Generator::createMission(...)
0x3df4     28    1    MTitle::~MTitle()
0x499c     64   15    PlayerJumpgate::PlayerJumpgate(...)
0x5f48    956  138    Level::createMission()
0x6304    228    1    Level::createScene()
0x7520    336   69    Level::createShip(...)
0x7670     32    1    Level::createRoute(int)
0x8d50   1244    3    RecordHandler::recordStoreRead(int)
0x9334     36    3    RecordHandler::readOptionsFileAsByteArray(...)
0x9ef0    112    2    ModStation::ModStation()
0x1e38c   256    7    Radar::Radar(Level*)
0x1e4c0   132   28    Radar::draw(Player*, Hud*, int)
0x1e544   132    1    Radar::calcDistance(...)
0x1ef7c   348  291    LevelScript::process(int)
0x1f0d8    12    1    LevelScript::resetCamera(Level*)
0x1f580   948  760    HangarWindow::render()
0x1f934   128    2    HangarWindow::OnTouchBegin(int,int)
0x21c50   476   39    Hud::draw(...)
0x21e2c    68    1    Hud::drawOrbitInformation()
0x235bc   168   14    SpaceLounge::drawLounge()
0x244b8   280   30    MGame::OnTouchEnd(int,int,void*)
```

This is a useful localization: 11 of the 18 residual functions (12 of the 18
residual symbols, counting `ModStation` C1/C2 separately) are in this list,
plus a handful of functions whose LSDA shape differs because of the same
source-shape work. No build-flag change is needed to explain the extab delta.

The record parser used the byte-identical `.ARM.exidx` as the authority:
`word1 == 1` is CANTUNWIND, `word1 & 0x80000000` is a compact exidx entry,
otherwise `target = exidx_entry_addr + prel31(word1)` is an `.ARM.extab`
record start. Sorting the unique targets gives the record boundaries; comparing
the record bytes original vs ours gives the 23/1,436 result above.

---

## 4. CMake vs ndk-build: the `-fdata-sections` discriminator

### 4.1 NDK r18b sources

`build/cmake/android.toolchain.cmake` lines 520-525:

```cmake
list(APPEND ANDROID_COMPILER_FLAGS
  -g
  -DANDROID
  -ffunction-sections
  -funwind-tables
  -fstack-protector-strong
  -no-canonical-prefixes)
```

There is **no `-fdata-sections`** anywhere in the CMake toolchain's compiler
flags. `build/core/default-build-commands.mk` lines 136-141:

```make
GLOBAL_CFLAGS = \
    -target $(LLVM_TRIPLE)$(TARGET_PLATFORM_LEVEL) \
    -fdata-sections \
    -ffunction-sections \
    -fstack-protector-strong \
    -funwind-tables \
    -no-canonical-prefixes \
```

So vanilla ndk-build always adds `-fdata-sections`; the CMake toolchain does
not. This is the cleanest build-system discriminator.

### 4.2 Full controlled A/B

I built the same tree twice with the same compiler/objects, once canonical and
once with `-fdata-sections` appended to every C/C++ compile via a compiler
launcher (the launcher only appends the flag; it does not change anything else):

```sh
cat > /dev/shm/w7640/add_fdata.sh <<'SH'
#!/bin/sh
exec "$@" -fdata-sections
SH
cmake -S . -B /dev/shm/w7640/build-fdata -G Ninja \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs \
  -DGOF2_MATCH_OPT=-Oz \
  -DCMAKE_C_COMPILER_LAUNCHER=/dev/shm/w7640/add_fdata.sh \
  -DCMAKE_CXX_COMPILER_LAUNCHER=/dev/shm/w7640/add_fdata.sh
cmake --build /dev/shm/w7640/build-fdata --target link_so -j6
```

Section comparison (`sha256` of payloads; `diff` is byte count):

| section | original | canonical (no `-fdata`) | `-fdata-sections` |
|---|---:|---:|---:|
| `.rodata` size | 269,159 | **269,159** | 268,807 |
| `.rodata` sha | `73c73731…` | **`73c73731…`** | `9b2a9a42…` |
| `.rodata` diff vs canonical | — | **0** | 113,935 (+352 length) |
| `.data` size | 10,604 | **10,604** | 10,588 |
| `.data` sha | `bf3a8711…` | **`bf3a8711…`** | `d779524c…` |
| `.data` diff vs canonical | — | **0** | 3,023 (+16 length) |
| `.data.rel.ro` size | 12,264 | **12,264** | 12,264 |
| `.data.rel.ro` sha | `34ad551d…` | **`34ad551d…`** | `7de91b42…` |
| `.data.rel.ro` diff | — | **0** | 365 |
| `.text` diff vs canonical | 85,520 | 0 | 801 |
| `.dynsym` diff | 0 | 0 | 753 |
| `.rel.dyn` diff | 0 | 0 | 180 |

The original's `.rodata`, `.data` and `.data.rel.ro` are **byte-identical to the
no-`-fdata-sections` build**, and differ from the `-fdata-sections` build in
exactly the way the prior notes describe (113,935 / 3,023 / 365 bytes). The
full-link verify score for the `-fdata-sections` build is:

```text
compared 4524  avg 99.94%  byte_exact 4315  linked_exact 4506  fuzzy_perfect 4506
```

i.e. **4506 → 4315 byte_exact** (191 functions), with linked_exact unchanged.
This independently confirms both the discriminator and the direction of the
cost.

### 4.3 Object-level confirmation

`Status.cpp` compiled without `-fdata-sections` has one `.rodata` (0x94 B) plus
`.rodata.str1.1`; with `-fdata-sections` the same object splits into:

```text
.rodata._ZL22kMedalCampaignMissions
.rodata._ZL14g_levelXPTable
.rodata..Lswitch.table._ZN6Status26getGammaRayDamagePerSecondEii
.rodata..Lswitch.table._ZN6Status26getGammaRayDamagePerSecondEii.1
```

so the original's single `.rodata` block can only come from the no-`-fdata`
compile.

### 4.4 Link-order corroboration

The original's dynamic imports also match the CMake order, not ndk-build's:

```text
DT_NEEDED tail: libm.so libdl.so libc.so
_Unwind_*:      7 UND imports (DeleteException, GetLanguageSpecificData,
                GetRegionStart, RaiseException, Resume, VRS_Get, VRS_Set)
__gnu_Unwind_Find_exidx: absent
memalign:       UND (not posix_memalign)
```

FMOD (`libfmodex.so`) is before `libc++abi.a`/`libgcc.a` on the link line, so
FMOD's `_Unwind_*` satisfy the C++ runtime and `libgcc.a`'s `libunwind.o` is not
pulled. ndk-build puts the static STL archives before the shared libraries,
which defines `_Unwind_*` locally and pulls `libunwind.o`. Our canonical link
prints exactly the original's 10-entry `DT_NEEDED` list and the same 7 UND
`_Unwind_*` imports.

`--exclude-libs=libgcc.a` is visible in the symbol table: `__aeabi_ldivmod` is a
local `t` at `0x19ae54` in the unstripped link and is absent from `.dynsym`,
while `__gnu_ldivmod_helper` stays UND. `--whole-archive` for zlib/libzip is
visible in the exported set: `adler32`, `crc32`, `deflate`, `inflate`,
`zip_open`, `zip_close` and `zip_fopen` are all `T` at the same addresses in
both binaries.

---

## 5. Compile-flag controlled A/B

### 5.1 Object-level screen (5 TUs)

I compiled `Status.cpp`, `Engine.cpp`, `Level.cpp`, `SimpleShader.cpp` and
`main.c` with the canonical command and with one flag changed at a time, then
compared every non-debug section of the object. Summary (`IDENTICAL` means all
non-debug sections byte-identical; `CHANGED` lists the first affected
code/data/attribute sections):

| variant | result |
|---|---|
| `-O2`, `-Os`, `-O1` (replace `-Oz`) | CHANGED on every TU (`.text.*`, `.ARM.exidx`, `.ARM.extab`) |
| `-g0` / `-gline-tables-only` (replace `-g`) | CHANGED (`.ARM.extab`, `.rel.ARM.exidx`; some TUs also `.text`) |
| `-fpic` (replace `-fPIC`) | **IDENTICAL on all 5 TUs** |
| `-fno-rtti` (replace `-frtti`) | IDENTICAL on most; `SimpleShader.cpp` `.data.rel.ro` 64 → 52 B |
| `-fno-exceptions` | CHANGED on all C++ TUs (`.ARM.extab` LSDA sizes, `__clang_call_terminate`); it compiles and links (there is no `try`/`catch` in `src/` at this HEAD), but full-link `.text` 1,259,464 → 1,168,676 B and `.ARM.extab` 156,312 → 30,732 B |
| `-fno-stack-protector` / `-fstack-protector` (replace `-fstack-protector-strong`) | CHANGED (`.text.*`, `.ARM.extab`, `.rel.text`) |
| remove `-ffunction-sections` | CHANGED (per-function `.text.*`/`.ARM.exidx.*` collapse to one section) |
| remove `-funwind-tables` | CHANGED (`.ARM.exidx` entries lose unwind info) |
| `-std=gnu++14` (replace `-std=c++11`) | CHANGED on `Engine.cpp` (relocations); `main.c` is C, so the replacement is not applicable |
| `-march=armv7ve` | CHANGED (`.ARM.attributes` 60 → 66 B, code) |
| `-marm` (replace `-mthumb`) | CHANGED (code, `.ARM.exidx`/`.ARM.extab`) |
| `-mfpu=vfpv3-d16` (replace `-mfpu=neon`) | CHANGED (`.ARM.attributes` 60 → 58 B); `Engine.cpp` fails to compile (NEON intrinsics) |
| `-mfloat-abi=soft` (replace `softfp`) | CHANGED (`.ARM.attributes` 60 → 56 B, code) |

This confirms every load-bearing flag is observable on real TUs. `-fPIC` vs
`-fpic` is the one neutral pair, exactly as the synthesis says.

### 5.2 Full-link A/B scores

Using the same launcher technique on the whole tree:

| build | byte_exact | linked_exact | avg | note |
|---|---:|---:|---:|---|
| canonical (`-Oz -g -fPIC -frtti -fstack-protector-strong …`) | **4506** | **4506** | 99.94 | gate |
| `-fpic` instead of `-fPIC` | **4506** | **4506** | 99.94 | identical |
| `-fdata-sections` added | **4315** | **4506** | 99.94 | −191 byte_exact |
| `-g0` (no `-g`) | **2071** | **4252** | 99.59 | −2435 byte_exact; linked −254 |
| `-fno-rtti` | **1701** | **4506** | 99.94 | −2805 byte_exact |

The `-g0` and `-fno-rtti` numbers reproduce the prior notes' magnitudes
(2071 and 1701 byte_exact). The `-g0` linked_exact here is 4252 (prior note:
4247) because the source has advanced; the direction and scale are unchanged.

### 5.3 Residual-function sensitivity

The 18 non-byte-exact functions at this HEAD and their percentages under the
tested full-link variants:

| function | canonical | `-g0` | `-fdata` | `-fno-rtti` | `-fpic` |
|---|---:|---:|---:|---:|---:|
| `Layout::Layout` C1 | 48.9 | 48.9 | 48.9 | 48.9 | 48.9 |
| `Layout::Layout` C2 | 48.9 | 48.9 | 48.9 | 48.9 | 48.9 |
| `LevelScript::process` | 60.2 | 60.2 | 60.2 | 60.2 | 60.2 |
| `Level::createShip` | 70.5 | 70.5 | 70.5 | 70.5 | 70.5 |
| `Status::missionCompleted` | 71.1 | 71.1 | 71.1 | 71.1 | 71.1 |
| `Radar::draw` | 75.3 | **75.2** | 75.3 | 75.3 | 75.3 |
| `MGame::OnTouchEnd` | 81.9 | **81.7** | 81.9 | 81.9 | 81.9 |
| `Generator::createMission` | 90.0 | **89.7** | 90.0 | 90.0 | 90.0 |
| `Level::createMission` | 92.3 | 92.3 | 92.3 | 92.3 | 92.3 |
| `Gun::calcCharacterCollision` | 93.4 | 93.4 | 93.4 | 93.4 | 93.4 |
| `Level::createScene` | 93.6 | 93.6 | 93.6 | 93.6 | 93.6 |
| `HangarWindow::render` | 94.4 | 94.4 | 94.4 | 94.4 | 94.4 |
| `SpaceLounge::drawLounge` | 95.7 | 95.7 | 95.7 | 95.7 | 95.7 |
| `Hud::draw` | 96.6 | 96.6 | 96.6 | 96.6 | 96.6 |
| `MenuTouchWindow::OnTouchBegin` | 98.1 | **83.2** | 98.1 | 98.1 | 98.1 |
| `ModStation::ModStation` C1 | 99.2 | 99.2 | 99.2 | 99.2 | 99.2 |
| `ModStation::ModStation` C2 | 99.2 | 99.2 | 99.2 | 99.2 | 99.2 |
| `MGame::OnUpdate` | 99.9 | 99.9 | 99.9 | 99.9 | 99.9 |

**No tested flag improves any residual function.** `-g0` changes four of them
and makes all four worse (Radar −0.1, OnTouchEnd −0.2, Generator −0.3,
MenuTouchWindow −14.9). `-fdata-sections`, `-fno-rtti` and `-fpic` leave every
residual percentage unchanged. This is direct evidence that the residual gaps
are source shape, not build configuration.

---

## 6. Link and strip recipe

### 6.1 Gold defaults are exactly the expected set

I extracted the gold part of the canonical link command from
`ninja -t commands libgof2hdaa.so` and relinked the same objects with explicit
flags. The baseline and the explicit-default link are byte-identical:

```text
baseline (repo link):        sha256 21c5ddcfefbe2751b82ca208e48db5c69a48ecea962dd21f1084907ebeb6f6ae
+ --hash-style=sysv
+ --no-as-needed
+ -z combreloc
+ --spare-dynamic-tags=5
+ -z relro
+ -z noexecstack
+ -z max-page-size=4096
+ -z common-page-size=4096:  sha256 21c5ddcfefbe2751b82ca208e48db5c69a48ecea962dd21f1084907ebeb6f6ae  (identical)
```

So those are gold 1.12 defaults, not extra repo flags. Changing any one of them
changes the output:

| variant | unstripped sha256 (first 16) | size |
|---|---|---:|
| baseline | `21c5ddcfefbe2751` | 19,357,736 |
| explicit defaults | `21c5ddcfefbe2751` | 19,357,736 |
| `--hash-style=gnu` | `b421d328b26c872a` | 19,361,836 |
| `--spare-dynamic-tags=0` | `2b2f91ac6137b433` | 19,357,736 |
| `--spare-dynamic-tags=10` | `a1d9fa916db5eb73` | 19,357,736 |
| `-z max-page-size=65536` | `3c1d647c3c637770` | 19,410,984 |
| `-z norelro` | `8cd811840c0a366b` | 19,358,864 |
| `-z lazy` | `6250b527bd38947f` | 19,358,512 |
| `--as-needed` | `0f41e07fcbf61eae` | 19,357,736 |
| `-z nocombreloc` | `e053e7b893fd5b56` | 19,357,736 |

`--hash-style=gnu` adds `.gnu.hash`; `--spare-dynamic-tags=0/10` changes the
dynamic-section padding; `-z max-page-size=65536` changes segment alignment;
`-z norelro`/`-z lazy` change the RELRO/BIND_NOW shape; `--as-needed` drops
unused `DT_NEEDED` entries; `-z nocombreloc` changes `.rel.dyn`. The original
has sysv hash, 5 spare tags, 4096 page size, RELRO/BIND_NOW and combreloc, so
the default set is the one that matches.

### 6.2 Build-id is the SHA-1 of the unstripped link output

The build-id note descriptor in the baseline unstripped output is
`7de0196f59b1ac9e74cde340a7cb984b22c4bf9f`. Zeroing that 20-byte descriptor in
the unstripped file and hashing the whole file with SHA-1 gives exactly the same
value:

```text
build-id note off 0x1cc, descriptor off 0x1dc
build-id descriptor: 7de0196f59b1ac9e74cde340a7cb984b22c4bf9f
sha1(file with descriptor zeroed): 7de0196f59b1ac9e74cde340a7cb984b22c4bf9f
sha1(whole unstripped file):       c499efff22febee75d13132d17b5866b5334e99c
```

This is the direct proof that the original link was **gold without `-s`**,
followed by a strip that preserves the build-id. Gold `-s` computes a different
build-id (`86f089bfa3136be78418bcc123c8eb55b67eb0ca` in our test), because it
hashes the stripped file.

### 6.3 Strip audit

Using the baseline unstripped link:

| artifact | size | sha256 (first 16) | build-id |
|---|---:|---|---|
| unstripped gold | 19,357,736 | `21c5ddcfefbe2751` | `7de0196f…` |
| GNU `strip --strip-all` | 2,187,328 | `38e4c7351b38c430` | `7de0196f…` |
| GNU strip + `reorder_shstrtab.py` | 2,187,328 | `597d36aac326e389` | `7de0196f…` |
| gold `-s` | 2,187,328 | `44e5aee401e00d65` | `86f089bf…` |
| `objcopy --strip-all` | 2,187,328 | `38e4c7351b38c430` | `7de0196f…` |
| `strip --strip-debug` | 2,794,840 | `dbad96707a134512` | `7de0196f…` |
| canonical `libgof2hdaa.so` | 2,187,328 | `597d36aac326e389` | `7de0196f…` |
| original | 2,187,328 | `d4c17be61af96272` | `c011810d…` |

`canonical == gnu_strip_reorder` is byte-for-byte true. The gold `-s` output
differs in the bfd-normalized headers:

```text
GNU strip:  .rel.plt  sh_info=20, .init_array/.fini_array sh_entsize=4,
            .bss sh_offset=0x21596c
gold -s:    .rel.plt  sh_info=0,  .init_array/.fini_array sh_entsize=0,
            .bss sh_offset=0x215970
```

and in `.shstrtab` order (`7d089864…` before reorder, `c34e875e…` after
reorder; the original is `c34e875e…`). The reorder script restores gold's
Stringpool order from the unstripped file, which is why the canonical `.shstrtab`
is byte-identical to the original.

**Contradiction with the prior synthesis:** in this NDK r18b,
`objcopy --strip-all` is byte-identical to GNU `strip --strip-all` (same sha256
and same build-id), not a different-build-id variant. The real discriminator is
gold `-s` vs a post-link bfd strip, not GNU strip vs objcopy. The canonical
build uses GNU strip, so this does not affect the recipe.

---

## 7. Confidence table

| claim | confidence | evidence |
|---|---:|---|
| NDK r18b (`Pkg.Revision 18.1.5063045`) | **proven** | `.note.android.ident` bytes; `source.properties` |
| clang 7.0.2 | **proven** | `.comment` exact; `clang --version` |
| gold 1.12 | **proven** | `.note.gnu.gold-version` exact; `ld.gold --version` |
| API 16 | **proven** | `.note.android.ident` word[0] = 0x10 |
| armeabi-v7a, Thumb-2, VFPv3, NEONv1, softfp | **proven** | `e_flags 0x5000200`; `.ARM.attributes` exact |
| static libc++abi (`c++_static`) | **proven** | no `libc++_shared.so` NEEDED; `__cxa_*` defined; `memalign` UND; `libc++abi.a` on the link line |
| CMake compile flags, **no `-fdata-sections`** | **proven** | CMake/ndk-build source lines; `.rodata`/`.data`/`.data.rel.ro` byte-identical A/B |
| `-Oz`, `-g`, `-fPIC`, `-frtti`, exceptions, `-fstack-protector-strong`, `-ffunction-sections`, `-funwind-tables`, `-std=c++11`, `-march/-mthumb/-mfpu/-mfloat-abi` | **proven** | object-level A/B on 5 TUs + full-link A/B for `-Oz`/`-g`/`-fPIC`/`-frtti`/`-fdata`; `-fstack-protector-strong` object-level here and prior full-link 1369/3783 |
| `-fPIC` vs `-fpic` neutral | **proven** | object-level and full-link byte-identical |
| gold `-z now`, `--build-id`, `--exclude-libs=libgcc.a`, whole-archive zlib/libzip, FMOD-before-STL, `libgcc.a` | **proven** | `DT_NEEDED`, `DT_FLAGS`, `_Unwind_*` UND, `__aeabi_ldivmod` local, exported zlib/libzip symbols |
| gold defaults `--hash-style=sysv --no-as-needed -z combreloc --spare-dynamic-tags=5 -z relro -z noexecstack -z max-page-size=4096` | **proven** | explicit-default link is byte-identical to baseline; each variant changes the output |
| build-id = SHA-1 of unstripped output with descriptor zeroed | **proven** | direct SHA-1 computation |
| GNU `strip --strip-all` + `reorder_shstrtab.py` | **proven** | canonical == gnu_strip_reorder; gold `-s` differs |
| macOS/darwin NDK package | **likely** | only surviving source path is `/Volumes/Android/buildbot/...`; prior note's linux-archive A/B |
| exact CMake version / generator | **unknown** | codegen-neutral, not embedded |
| exact build host OS/CPU | **unknown** | only the darwin path string survives |
| exact build file (`CMakeLists.txt` vs `externalNativeBuild`) | **unknown** | stripped ELF; effective recipe proven |
| link timestamp | **unknown** | Crashlytics `2018-10-22 10:18:17 CEST` is the closest witness |

---

## 8. New evidence, contradictions and doc notes

1. **`.ARM.exidx` CANTUNWIND count is 272, not 194.** The total is 3,566
   entries: 2,438 extab references, 856 compact, 272 CANTUNWIND. The 194 figure
   in the wave-127 synthesis is not the total; it may be a subset defined some
   other way. The section itself is byte-identical to the original, so this is
   a note-accuracy issue, not a matching issue.
2. **`objcopy --strip-all` == GNU `strip --strip-all` here.** Both produce
   sha256 `38e4c7351b38c430…` and preserve build-id `7de0196f…`. The synthesis's
   "objcopy build-id differs" is not reproduced with the NDK r18b objcopy.
3. **Docs' `.text`/`.ARM.extab` counts are stale at this HEAD.** The fresh
   measurement is `.text` 85,520 and `.ARM.extab` 1,436 (23 records). The docs
   currently say 85,496/1,438 (wave 126) and the wave-127 synthesis measured
   85,552/1,438 at its base. This is source drift, not a regression; the gate
   counts are unchanged. A future doc-only pass should refresh those numbers.
4. **`.ARM.extab` differences are localized.** Only 23 of 2,438 records differ,
   and 11 of the 18 residual functions (12 symbols counting `ModStation` C1/C2)
   are among them. This is consistent with source-shape work on those
   functions, not a global flag.
5. **`-g0` changes four residual functions, all worse.** Radar 75.3 → 75.2,
   MGame::OnTouchEnd 81.9 → 81.7, Generator::createMission 90.0 → 89.7,
   MenuTouchWindow::OnTouchBegin 98.1 → 83.2. This is new quantitative evidence
   that `-g` is load-bearing and that the residual gap is not a missing flag.
6. **`-fdata-sections` costs 191 byte_exact but changes no residual
   percentage.** This cleanly separates the build-system discriminator from the
   residual-function problem.
7. **The repo-root `report.json` is stale for several residual percentages.**
   The fresh verify gives `LevelScript::process` 60.2, `MGame::OnTouchEnd` 81.9,
   `HangarWindow::render` 94.4 and `Hud::draw` 96.6; the checked-in
   `report.json` still says 60.3, 81.3, 94.1 and 96.4. The gate counts
   (4506/4506/4524, 843/843, 207 imports) are identical, so this is a snapshot
   staleness issue, not a regression. This note uses the fresh verify values.
8. **`-fno-exceptions` compiles and links at this HEAD.** The synthesis says
   `Level.cpp` has try/catch and `-fno-exceptions` fails; there is no
   `try`/`catch` in `src/` now. The flag is still load-bearing: full-link
   `.text` shrinks 1,259,464 → 1,168,676 B and `.ARM.extab` 156,312 → 30,732 B,
   so exceptions are on in the original. The EH/LSDA records are cleanup-only,
   not user try/catch.

---

## 9. What would falsify this verification

* A build with `-fdata-sections` whose `.rodata`/`.data`/`.data.rel.ro` are
  byte-identical to the original. It is not: the no-`-fdata` build is exact.
* A gold link with `--hash-style=gnu`, `--spare-dynamic-tags=0/10`,
  `-z max-page-size=65536`, `-z norelro`, `--as-needed` or `-z nocombreloc`
  producing the original's dynamic section. All change the output.
* A gold `-s` link reproducing the original build-id. It cannot: gold `-s`
  hashes the stripped file, and the original build-id is the SHA-1 of the
  unstripped output with the descriptor zeroed.
* A flag that raises any of the 18 residual percentages without changing the
  source. The tested set (`-fpic`, `-fdata-sections`, `-fno-rtti`) is neutral;
  `-g0` is negative.

Until one of those happens, the correct conclusion is: **the build recipe is
proven, and the remaining 18 functions are source reconstruction.**
