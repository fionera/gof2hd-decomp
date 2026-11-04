# Visibility / symbol-export / RTTI / vtable audit (w5216)

Status: research note, 2026-11-04. Worktree `/home/debian/GalaxyOnFire2/wt/w5216`,
branch `wave104-w5216`, base `8c5fd3646`. Scratch is `/dev/shm/w5216`.
**No source or build-flag change is landed.** This commit contains this note only.

## 0. Bottom line

* Baseline is green: **byte_exact 4500/4524, linked_exact 4500/4524, size-exact
  4524/4524, data_exact 843/843, imports 207, lint CLEAN, drift 0, sodiff
  5370/5370 (0 extra / 0 missing)**.
* The original's exported surface is **all DEFAULT visibility**:
  `5370 = 4524 FUNC + 843 OBJECT + 3 NOTYPE`, `4895 GLOBAL + 475 WEAK`,
  `DEFAULT 5370` (no HIDDEN/PROTECTED/INTERNAL in `.dynsym`).
* The original exports **99 `_ZTV` (5,364 B), 176 `_ZTI` (2,156 B), 176 `_ZTS`
  (2,820 B)**. All are DEFAULT. The vtable split is 98 GLOBAL / 1 WEAK; the
  typeinfo and type-string split is 170 GLOBAL / 6 WEAK each. Our baseline
  reproduces every one of these symbols, sizes, bindings and visibilities
  exactly: `.dynsym`, `.dynstr` and `.hash` are byte-identical to the original.
* `-fvisibility=default` is an exact no-op. `-fvisibility=hidden`,
  `-fvisibility=protected`, `-fvisibility-inlines-hidden` and
  `-fvisibility-ms-compat` are all much worse. `-fno-rtti` is worse.
  `-fno-rtti-data` is an exact no-op on this source. `-fno-weak` is not
  supported by clang 7.0.2. `-fno-common` fails to link the C TUs; with the
  duplicate tentative definition removed it is still 31 byte_exact worse.
* **No strict improvement exists.** No tested variant exceeds 4500/4500.
* **Irreducible export deltas: none.** `tools/sodiff_allow.txt` is empty and
  all 5,370 defined exports, including all vtables/typeinfo, match exactly.

## 1. Baseline commands and raw results

Commands (nested worktree needs the explicit original/FMOD paths):

```sh
export TMPDIR=/dev/shm/w5216/tmp
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
cmake --build cmake-build-match --target diff -j6
cmake --build cmake-build-match --target drift -j6
python3 tools/goal_lint.py src
```

Raw output:

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
original: 5370  ours: 5370  extra: 0  missing: 0 (wrong_type: 0  absent: 0)
SYMBOL PARITY ✓
=== DRIFT: 0 field(s) across 0 class(es) ===
CLEAN ✓ (no new soft violations; hard invariants 0)
```

Artifact hashes:

| artifact | sha256 |
|---|---|
| original `android_2.0.16_libgof2hdaa.so` | `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483` |
| canonical `cmake-build-match/libgof2hdaa.so` | `cba9529fe5db32aafa7bafe78e5b4552e948346039efefeb6cb66337ddb79ab0` |

Original vs canonical section bytes (the dynamic tables are exact):

| section | size | original sha256 | canonical sha256 | exact |
|---|---:|---|---|---|
| `.dynsym` | 89,248 | `5a3b59e8436c8dc5b2ef63ac5ab0196eb41cde2ed6314b354e7a9fbd37a82d6c` | same | yes |
| `.dynstr` | 189,208 | `a42e036792f61aaa4c07ac027495052050ab7ddacbdc5204cc2aadefb69955b7` | same | yes |
| `.hash` | 38,716 | `fdc6fe8d74f1d4e2882daef8057dbea632a25ee5da3dfd8df2f500911c65cb1e` | same | yes |
| `.text` | 1,259,464 | `d49e32c1330e5d94e7628b9a40e05500af963a22816a93f2c738dbd366f813ed` | `144a3c3799cddbe2a02e3eef72cf37b213fd24b9a2eccaac866e75640d0a1a2f` | no (known 24 bodies) |
| `.rodata` | 269,159 | `73c737319958f462dfea24f59883dea97853195cbb5607f50243f417389deb31` | same | yes |
| `.data.rel.ro` | 12,264 | `34ad551db3f4b09ae3e40f5107d6666f922a7de32bd0f0900e5d5669443a9025` | same | yes |
| `.data` | 10,604 | `bf3a87118411e0a3883bfe8db2c4e2d48f9dc38346016de7fd1409d7003398d4` | same | yes |
| `.bss` | 27,052 | `74e30e0e954e913a5e77191ab1e370bcd40486a203b329f7f6f9450337742063` | same | yes |

`.dynsym` is 5,578 entries (5,370 defined + 207 undefined imports + the null
entry). The original has no `.symtab`; only exported dynamic symbols survive
the strip.

## 2. Harness

A scratch wrapper appends the tested flag after the canonical toolchain flags:

```cmake
# /dev/shm/w5216/toolchain_extra.cmake
include("/home/debian/GalaxyOnFire2/wt/w5216/cmake/toolchain.cmake")
if(GOF2_EXTRA_CXX_FLAGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GOF2_EXTRA_CXX_FLAGS}" CACHE STRING "" FORCE)
endif()
if(GOF2_EXTRA_C_FLAGS)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GOF2_EXTRA_C_FLAGS}" CACHE STRING "" FORCE)
endif()
```

Each variant was configured into the same scratch build directory (so the
vendored libs are reused), linked, copied to `/dev/shm/w5216/so/<tag>.so`, and
measured with the official verifier and `sodiff.py`:

```sh
cmake -S /home/debian/GalaxyOnFire2/wt/w5216 -B /dev/shm/w5216/build-sweep -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/dev/shm/w5216/toolchain_extra.cmake \
  -DGOF2_MATCH_OPT=-Oz \
  -DGOF2_EXTRA_CXX_FLAGS='<flag>' -DGOF2_EXTRA_C_FLAGS='<flag>' \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build /dev/shm/w5216/build-sweep --target link_so -j6
python3 tools/verify/verify.py \
  --build-dir /dev/shm/w5216/verify/<tag> \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /dev/shm/w5216/so/<tag>.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 tools/sodiff.py \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /dev/shm/w5216/so/<tag>.so --allow tools/sodiff_allow.txt
```

The scratch build directory is different from `cmake-build-match`, so the
20-byte `.note.gnu.build-id` normally differs even for code-identical variants
(gold hashes the unstripped output, which includes the DWARF compilation
directory). For the explicit-default check, `cmp -l` found **exactly 20
differing bytes**, all in `.note.gnu.build-id` (file offsets 477..496); every
section hash was identical.

## 3. Original export / visibility / typeinfo / vtable model

Direct counts from the shipped binary:

```sh
readelf --dyn-syms -W "$O" | awk '$7!="UND"{v[$6]++;t[$4]++;b[$5]++} END{...}'
```

```text
defined: 5370 = FUNC 4524 + OBJECT 843 + NOTYPE 3
bind:    GLOBAL 4895 + WEAK 475
         GLOBAL: FUNC 4062, OBJECT 830, NOTYPE 3
         WEAK:   FUNC 462,  OBJECT 13
vis:     DEFAULT 5370   (HIDDEN 0, PROTECTED 0, INTERNAL 0)
```

RTTI/vtable exports:

| prefix | count | total size | bind | visibility |
|---|---:|---:|---|---|
| `_ZTV` | 99 | 5,364 B | 98 GLOBAL, 1 WEAK | DEFAULT 99 |
| `_ZTI` | 176 | 2,156 B | 170 GLOBAL, 6 WEAK | DEFAULT 176 |
| `_ZTS` | 176 | 2,820 B | 170 GLOBAL, 6 WEAK | DEFAULT 176 |
| `_ZTT` / `_ZTC` / `_ZThn` / `_ZTv` / `_ZGV` | 0 | 0 | — | — |

The single weak vtable is `_ZTVN11AbyssEngine7AESoundE`. The six weak
typeinfo/type-string pairs are `FileInterface`, `AELowLevelFile`,
`AESoundInterface`, `AESound`, `IApplicationModule`, `AbstractGun`. Our
baseline has exactly the same sets, sizes, bindings and visibility.

`g_android_back_button_pressed` is a 4-byte `GLOBAL DEFAULT` object at
`0x21d2fc` in `.bss` in both binaries. The reconstructed tree has tentative
`int` definitions in both `src/platform/android/GOF2HD2012.c` and
`src/platform/android/BackButton.c`; the original link therefore used the
r18b C default `-fcommon`, not `-fno-common`.

## 4. Variant matrix

All variants are one flag on top of the canonical `-Oz -g -fPIC -frtti
-fstack-protector-strong -ffunction-sections -funwind-tables` build. “Size”
is `sum(osize == usize)` over the verifier rows. `data` is
`tools/datadiff.Side` exact/common exported objects. `imports` is the
undefined `.dynsym` set size.

| variant | flag(s) | byte | linked | size | avg | verify missing | sodiff ours | sodiff missing | data | imports |
|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| baseline | *(none)* | **4500** | **4500** | **4524/4524** | 99.9 | 0 | 5370 | 0 | **843/843** | 207 |
| `vis-default` | `-fvisibility=default` (C+CXX) | **4500** | **4500** | **4524/4524** | 99.9 | 0 | 5370 | 0 | **843/843** | 207 |
| `vis-hidden` | `-fvisibility=hidden` (C+CXX) | 57 | 243 | 244/244 | 99.92 | 4280 | 578 | 4792 | 297/331 | 207 |
| `vis-protected` | `-fvisibility=protected` (C+CXX) | 1345 | 2382 | 4170/4524 | 92.6 | 0 | 5370 | 0 | 643/843 | 207 |
| `vis-inlines-hidden` | `-fvisibility-inlines-hidden` (CXX) | 1278 | 4203 | 4381/4383 | 99.84 | 141 | 5229 | 141 | 772/843 | 207 |
| `vis-ms-compat` | `-fvisibility-ms-compat` (C+CXX) | 57 | 243 | 244/244 | 99.93 | 4280 | 828 | 4542 | 467/581 | 207 |
| `no-rtti` | `-fno-rtti` (CXX) | 1701 | 4500 | 4524/4524 | 99.9 | 0 | 5200 | 170 | 593/673 | 207 |
| `no-rtti-data` | `-fno-rtti-data` (CXX) | **4500** | **4500** | **4524/4524** | 99.9 | 0 | 5370 | 0 | **843/843** | 207 |
| `no-common-cxx` | `-fno-common` (CXX only) | **4500** | **4500** | **4524/4524** | 99.9 | 0 | 5370 | 0 | **843/843** | 207 |
| `no-common-dedup` | `-fno-common` (C+CXX), one C tentative removed | 4469 | 4500 | 4524/4524 | 99.9 | 0 | 5370 | 0 | **843/843** | 207 |

`-fno-common` with both C and CXX (or C only) **fails to link**:

```text
ld.gold: error: .../GOF2HD2012.c.obj: multiple definition of 'g_android_back_button_pressed'
ld.gold: .../BackButton.c.obj: previous definition here
```

`-fno-weak` is rejected by clang 7.0.2:

```text
clang++: error: unknown argument: '-fno-weak'
```

`clang++ -cc1 --help` has no weak-symbol option; the original nevertheless
exports 462 WEAK functions and 13 WEAK objects, so weak support is required.

## 5. Section and symbol effects

### 5.1 `.dynsym` / `.dynstr` / `.hash`

`=` means byte-identical to the original; `≠` means different. `.dynsym`
entries include the null and undefined entries.

| variant | `.dynsym` entries | `.dynsym` size | `.dynsym` sha256 | `.dynstr` | `.hash` |
|---|---:|---:|---|---|---|
| original / baseline / `vis-default` / `no-rtti-data` / `no-common-cxx` | 5,578 | 89,248 | `5a3b59e8436c8dc5b2ef63ac5ab0196eb41cde2ed6314b354e7a9fbd37a82d6c` | 189,208 `=` | 38,716 `=` |
| `vis-hidden` | 786 | 12,576 | `ff48f439871022de99e532ded317ef28ced4049fdfa41857625115aed0909649` | 14,311 `≠` | 5,236 `≠` |
| `vis-protected` | 5,578 | 89,248 | `15d1dac3c1ea4a47bf876b03e5350496700d8f2ec34348bb3b937fe0c4372072` | 189,208 `=` | 38,716 `=` |
| `vis-inlines-hidden` | 5,437 | 86,992 | `f121062c6bf17c62cbd8b722088c8bc6d8b1c51e3fe92b3a899154f26eac71db` | 183,640 `≠` | 38,152 `≠` |
| `vis-ms-compat` | 1,036 | 16,576 | `103f296498c12c27034b7625ea00186763a0943f99a13e057cc49f6c1e7b078f` | 21,374 `≠` | 8,276 `≠` |
| `no-rtti` | 5,408 | 86,528 | `119d1e54992f5c86aa2f5d22315ff743c512864c9845beafed0b79548f8bb1da` | 184,408 `≠` | 38,036 `≠` |
| `no-common-dedup` | 5,578 | 89,248 | `b3092660c1ce3e7678900700797035bb5912b0cf51dd9ef1a88cf0cbcf737a7c` | 189,208 `=` | 38,716 `=` |

The original `.dynstr` hash is
`a42e036792f61aaa4c07ac027495052050ab7ddacbdc5204cc2aadefb69955b7`; the
original `.hash` hash is
`fdc6fe8d74f1d4e2882daef8057dbea632a25ee5da3dfd8df2f500911c65cb1e`.

`-fvisibility=protected` keeps the same names in the same order, so
`.dynstr` and `.hash` are unchanged; only `.dynsym`'s `st_other` bytes change.
Hidden/inlines/ms-compat/no-rtti remove names, so all three tables change.

### 5.2 Defined-symbol counts

| variant | defined | FUNC | OBJECT | NOTYPE | GLOBAL | WEAK | DEFAULT | PROTECTED |
|---|---:|---:|---:|---:|---:|---:|---:|---:|
| original / baseline | 5370 | 4524 | 843 | 3 | 4895 | 475 | 5370 | 0 |
| `vis-default` | 5370 | 4524 | 843 | 3 | 4895 | 475 | 5370 | 0 |
| `vis-hidden` | 578 | 244 | 331 | 3 | 558 | 20 | 578 | 0 |
| `vis-protected` | 5370 | 4524 | 843 | 3 | 4895 | 475 | 578 | 4792 |
| `vis-inlines-hidden` | 5229 | 4383 | 843 | 3 | 4895 | 334 | 5229 | 0 |
| `vis-ms-compat` | 828 | 244 | 581 | 3 | 795 | 33 | 828 | 0 |
| `no-rtti` | 5200 | 4524 | 673 | 3 | 4737 | 463 | 5200 | 0 |
| `no-rtti-data` | 5370 | 4524 | 843 | 3 | 4895 | 475 | 5370 | 0 |
| `no-common-cxx` | 5370 | 4524 | 843 | 3 | 4895 | 475 | 5370 | 0 |
| `no-common-dedup` | 5370 | 4524 | 843 | 3 | 4895 | 475 | 5370 | 0 |

The 578 symbols left by `-fvisibility=hidden` are the non-app symbols from
libc++abi, zlib, libzip and libcrypto: **244 FUNC + 331 OBJECT + 3 NOTYPE**.
`-fvisibility-ms-compat` leaves the same 578 plus **250 app typeinfo/vtable
objects** (80 `_ZTV` + 85 `_ZTI` + 85 `_ZTS`), for 828. `-fvisibility=protected`
marks all 4,792 app exports PROTECTED and leaves the 578 non-app exports
DEFAULT.

### 5.3 Typeinfo / vtable counts

`n/B` is count / total exported object size.

| variant | `_ZTV` | `_ZTI` | `_ZTS` | visibility |
|---|---|---|---|---|
| original / baseline | 99 / 5,364 B | 176 / 2,156 B | 176 / 2,820 B | all DEFAULT |
| `vis-default` | 99 / 5,364 B | 176 / 2,156 B | 176 / 2,820 B | all DEFAULT |
| `vis-hidden` | 19 / 500 B | 91 / 1,180 B | 91 / 760 B | all DEFAULT (non-app only) |
| `vis-protected` | 99 / 5,364 B | 176 / 2,156 B | 176 / 2,820 B | 80/85/85 PROTECTED; 19/91/91 DEFAULT |
| `vis-inlines-hidden` | 99 / 5,364 B | 176 / 2,156 B | 176 / 2,820 B | all DEFAULT |
| `vis-ms-compat` | 99 / 5,364 B | 176 / 2,156 B | 176 / 2,820 B | all DEFAULT |
| `no-rtti` | 99 / 5,364 B | 91 / 1,180 B | 91 / 760 B | all DEFAULT |
| `no-rtti-data` | 99 / 5,364 B | 176 / 2,156 B | 176 / 2,820 B | all DEFAULT |
| `no-common-cxx` | 99 / 5,364 B | 176 / 2,156 B | 176 / 2,820 B | all DEFAULT |
| `no-common-dedup` | 99 / 5,364 B | 176 / 2,156 B | 176 / 2,820 B | all DEFAULT |

`-fno-rtti` leaves all 99 vtables (same sizes) but removes 85 app `_ZTI` and
85 app `_ZTS` objects; the remaining 91/91 are the libc++abi runtime's
typeinfo. `-fno-rtti-data` changes nothing at all.

### 5.4 Code/data section sizes

| variant | `.text` | `.rodata` | `.data.rel.ro` | `.data` | `.bss` |
|---|---:|---:|---:|---:|---:|
| original / baseline | 1,259,464 | 269,159 | 12,264 | 10,604 | 27,052 |
| `vis-default` | 1,259,464 | 269,159 | 12,264 | 10,604 | 27,052 |
| `vis-hidden` | 1,249,080 | 269,159 | 12,264 | 10,796 | 26,924 |
| `vis-protected` | 1,249,080 | 269,159 | 12,264 | 10,796 | 26,924 |
| `vis-inlines-hidden` | 1,259,432 | 269,159 | 12,264 | 10,604 | 27,052 |
| `vis-ms-compat` | 1,249,240 | 269,159 | 12,264 | 10,796 | 26,924 |
| `no-rtti` | 1,259,464 | 266,318 | 11,292 | 10,604 | 27,052 |
| `no-rtti-data` | 1,259,464 | 269,159 | 12,264 | 10,604 | 27,052 |
| `no-common-cxx` | 1,259,464 | 269,159 | 12,264 | 10,604 | 27,052 |
| `no-common-dedup` | 1,259,464 | 269,159 | 12,264 | 10,604 | 27,052 |

`-fno-rtti` shrinks `.rodata` by 2,841 B and `.data.rel.ro` by 972 B while
leaving `.text` size unchanged. `-fvisibility=hidden` and `-protected` both
shrink `.text` by 10,384 B and change the app `.data`/`.bss` layout; hidden
and protected have the same `.text` size but different content.

## 6. Function-level effects

`cmp_elf.py` compared every common defined symbol's size and raw bytes against
the baseline.

| variant | common symbols | only in baseline | size changes | raw-byte changes |
|---|---:|---:|---:|---:|
| `vis-default` | 5370 | 0 | 0 | 0 |
| `vis-hidden` | 578 | 4792 | 0 | 237 |
| `vis-protected` | 5370 | 0 | 354 | 3,045 |
| `vis-inlines-hidden` | 5229 | 141 | 2 | 3,194 |
| `vis-ms-compat` | 828 | 4542 | 0 | 317 |
| `no-rtti` | 5200 | 170 | 0 | 2,847 |
| `no-rtti-data` | 5370 | 0 | 0 | 0 |
| `no-common-dedup` | 5370 | 0 | 0 | 33 |

Representative effects:

* `-fvisibility=protected` changes 354 function sizes. The largest are
  `Globals::getAgentMissionText(Agent*)` 9518 -> 8948 (-570),
  `Globals::Globals()` 612 -> 300 (-312, both C1/C2),
  `ApplicationManager::OnUpdate(long long)` 4576 -> 4348 (-228),
  `Globals::init(...)` 800 -> 588 (-212), `Globals::loadFont(int)`
  536 -> 360 (-176), `Globals::~Globals()` 468 -> 304 (-164, both D1/D2),
  `Level::createSpace()` 3076 -> 2976 (-100), `simulateTouch(Engine*)`
  1928 -> 1832 (-96). Small functions also lose their PLT/GOT sequence:
  `Blowfish_decipher` 80 -> 76, `InitializeBlowfish` 416 -> 404,
  `JNI_OnLoad` 100 -> 96 (raw bytes only), several `Java_*` wrappers
  16 -> 12.
* `-fvisibility=hidden` and `-fvisibility-ms-compat` hide the 4,280 app
  functions; only the 244 non-app functions remain comparable. Their common
  non-app functions still differ in raw bytes because the link layout moved.
* `-fvisibility-inlines-hidden` hides exactly 141 weak inline functions
  (`ModStation::OnTouch*`, `AbstractGun::is*Gun`, etc.). It also shrinks
  `Level::Level(int)` C1/C2 from 264 to 260 bytes; all typeinfo/vtables stay
  exported.
* `-fno-rtti` keeps all 4,524 functions and their sizes, but 2,847 raw-byte
  comparisons change because typeinfo/vtable data moved; `linked_exact`
  stays 4500. This is why `byte_exact` drops to 1701 while `linked_exact`
  does not move.
* `-fno-common` with the duplicate removed changes 33 raw function bodies and
  302 symbol values but no sizes; byte_exact drops 4500 -> 4469.

## 7. Object-level RTTI evidence

`src/engine/file/FileInterfaceAndroid.cpp` was compiled directly with the
canonical command plus one flag:

```sh
$CLANGXX $CANONICAL_FLAGS -o /dev/shm/w5216/FileInterfaceAndroid.<tag>.o \
  -c src/engine/file/FileInterfaceAndroid.cpp
```

| tag | extra flags | object sha256 | `.data.rel.ro` | `_ZTI` / `_ZTS` defined |
|---|---|---|---:|---:|
| `base` | *(none)* | `7f842f1033f8bc2aa5559a5ccc13de50b234814c729b28d93281e60f9b11cd5f` | 0x6c | 2 / 2 |
| `norttidata` | `-fno-rtti-data` | `7f842f1033f8bc2aa5559a5ccc13de50b234814c729b28d93281e60f9b11cd5f` | 0x6c | 2 / 2 |
| `nortti` | `-fno-rtti` | `9f22e5fa2f29b05dc1172a49df0030a95d5080a3cf0214317044a07ad2846dcc` | 0x60 | 0 / 0 |
| `nortti_norttidata` | `-fno-rtti -fno-rtti-data` | `9f22e5fa2f29b05dc1172a49df0030a95d5080a3cf0214317044a07ad2846dcc` | 0x60 | 0 / 0 |

`-fno-rtti-data` is a **byte-for-byte no-op** on this TU. `-fno-rtti`
removes the TU's typeinfo/type-string objects and shrinks `.data.rel.ro`
0x6c -> 0x60, while the vtable object remains. Adding `-fno-rtti-data` to
`-fno-rtti` changes nothing.

## 8. `-fno-common` evidence

The original exports exactly one 4-byte `g_android_back_button_pressed`
object:

```text
5: 0021d2fc 4 OBJECT GLOBAL DEFAULT 22 g_android_back_button_pressed
```

Our baseline has the same value/size/visibility. Both
`src/platform/android/GOF2HD2012.c` and `src/platform/android/BackButton.c`
contain a tentative `int g_android_back_button_pressed;`. With `-fno-common`
on C (or C+CXX), gold reports the duplicate and fails. With `-fno-common` on
C++ only, the build is byte-identical to the explicit-default build
(`f5b390c0...`), because C++ already treats these as definitions. Removing
the `GOF2HD2012.c` tentative definition lets the C+CXX `-fno-common` link
succeed, but then:

```text
byte_exact 4469  linked_exact 4500  size 4524/4524  data 843/843
sodiff 5370/5370 (0 extra / 0 missing)
.dynsym b3092660... (different)
.dynstr a42e0367... (same)
.hash   fdc6fe8d... (same)
.text   8d2b8631... (different)
.data.rel.ro 5a49de40... (different)
.rodata / .data / .bss exact
```

So `-fno-common` is not an improvement even after making it linkable.

## 9. Proven / disproven / unobservable

### Positively proven

1. The original's 5,370 exported symbols are all `DEFAULT` visibility.
2. The original exports 99 `_ZTV`, 176 `_ZTI`, 176 `_ZTS`; our baseline
   matches all names, sizes, bindings, visibilities and bytes.
3. `.dynsym`, `.dynstr` and `.hash` are byte-identical to the original.
4. The original uses RTTI: `-fno-rtti` removes 170 app typeinfo/type-string
   exports and drops byte_exact to 1701.
5. The original uses weak symbols: 462 WEAK functions and 13 WEAK objects.
6. The original uses the C default `-fcommon` for the reconstructed C TUs;
   `-fno-common` fails on the duplicate tentative definition.
7. `-fvisibility=default` is an exact no-op.
8. `-fvisibility=hidden`, `-fvisibility=protected`,
   `-fvisibility-inlines-hidden` and `-fvisibility-ms-compat` are all
   strictly worse.
9. `-fno-rtti-data` is an exact no-op on this source (object and full link).
10. `-fno-weak` is not accepted by clang 7.0.2.

### Disproven

* `-fvisibility=hidden`, `-fvisibility=protected`,
  `-fvisibility-inlines-hidden`, `-fvisibility-ms-compat`.
* `-fno-rtti`.
* `-fno-common` for the C TUs (link failure); and as a gate improvement
  even after deduplication (4469 vs 4500).
* `-fno-weak` as a clang 7.0.2 flag.

### Unobservable / not settled

* Whether the original spelled `-fvisibility=default` explicitly or relied on
  the compiler default. The two are byte-identical; the exact spelling is
  unobservable.
* Whether the original also passed `-fno-rtti-data`. It is a no-op on this
  source, so the binary cannot distinguish it from the default.
* Visibility of **local, stripped** symbols (file-static functions/objects).
  The original has no `.symtab`; only exported `.dynsym` survives. A
  selective per-symbol hidden attribute on a local definition would be
  invisible. There is no evidence for one, and the global hidden/protected
  variants are disproven.
* Whether the original source had exactly the same two tentative
  `g_android_back_button_pressed` definitions. The reconstructed C TUs do;
  under that faithful source, `-fcommon` is required. If the original source
  had only one definition, `-fno-common` could not be ruled out from the
  binary alone, but it would still be a worse match for this tree.

## 10. Confidence table

| claim | confidence | hard evidence |
|---|---|---|
| Original exports are all DEFAULT visibility | **very high** | `readelf --dyn-syms`: 5370/5370 DEFAULT; `.dynsym` byte-identical to ours |
| Original exports 99 vtables, 176 typeinfo, 176 type strings | **very high** | direct `.dynsym` counts; exact sizes/binds/visibility; baseline `.dynsym` byte-identical |
| Our baseline reproduces the typeinfo/vtable surface exactly | **very high** | `.dynsym` sha256 `5a3b...` both; sodiff 0/0 |
| Original uses RTTI | **very high** | 99/176/176 exports; `-fno-rtti` removes 170 exports and drops byte_exact 4500 -> 1701 |
| Original uses default visibility, not hidden/protected/ms-compat/inlines-hidden | **high** | full-link A/B: 57/1345/1278/57 byte_exact; 4280/0/141/4280 verify-missing; `.dynsym`/`.dynstr`/`.hash` deltas |
| Original uses weak symbols | **high** | 462 WEAK FUNC + 13 WEAK OBJECT; clang 7.0.2 has no `-fno-weak` |
| Original C TUs use `-fcommon` | **high** (given faithful duplicate tentative definitions) | duplicate `g_android_back_button_pressed` in two C TUs; `-fno-common` link failure; r18b default is `-fcommon` |
| `-fno-rtti-data` was not codegen-relevant | **high** | identical object sha256 and full `.so` sha256 to explicit-default build |
| Exact `-fvisibility=default` spelling | **unobservable** | explicit default and no flag are byte-identical (20-byte build-id only due scratch dir) |
| Local stripped symbol visibility | **unobservable** | no `.symtab` in original |

## 11. Strict improvement

**No strict improvement exists.** The baseline is 4500/4500; no tested
variant exceeds it. The only neutral variants are:

* `-fvisibility=default` (exact no-op; 4500/4500),
* `-fno-rtti-data` (exact no-op; 4500/4500),
* `-fno-common` on C++ only (exact no-op; 4500/4500).

All other variants are worse, fail to link, or are unsupported. No minimal
code/build change is warranted. The current canonical flags already encode
the correct visibility/RTTI/weak/common model.

## 12. Reproduction summary

```sh
# baseline
export TMPDIR=/dev/shm/w5216/tmp
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
cmake --build cmake-build-match --target diff -j6
cmake --build cmake-build-match --target drift -j6

# one variant (substitute <flag>; use GOF2_EXTRA_C_FLAGS='' for CXX-only flags)
cmake -S /home/debian/GalaxyOnFire2/wt/w5216 -B /dev/shm/w5216/build-sweep -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/dev/shm/w5216/toolchain_extra.cmake \
  -DGOF2_MATCH_OPT=-Oz \
  -DGOF2_EXTRA_CXX_FLAGS='<flag>' -DGOF2_EXTRA_C_FLAGS='<flag>' \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build /dev/shm/w5216/build-sweep --target link_so -j6
python3 tools/verify/verify.py \
  --build-dir /dev/shm/w5216/verify/<tag> \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /dev/shm/w5216/so/<tag>.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 tools/sodiff.py \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /dev/shm/w5216/so/<tag>.so --allow tools/sodiff_allow.txt
```

Scratch artifacts (not tracked): `/dev/shm/w5216/so/*.so`,
`/dev/shm/w5216/verify/*/report.json`, `/dev/shm/w5216/logs/*.log`,
`/dev/shm/w5216/audit_all.json`, `/dev/shm/w5216/cmp_*.json`.
