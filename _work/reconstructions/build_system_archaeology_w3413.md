# Original build-system archaeology (w3413)

Status: research note, 2026-11-01. Worktree `wt/w3413`, branch `wave87-w3413`,
base `103bcc42`. This note is self-contained: every number below is from a command run
against `../_work/bins/android_2.0.16_libgof2hdaa.so` or from a controlled rebuild of this
tree. It extends, and where noted corrects, `build_system_evidence.md`,
`build_flags_research.md`, and `link_flags_research.md`.

## 0. Bottom line

* The original is an NDK r18b / clang 7.0.2 / gold 1.12, armeabi-v7a, API-16,
  `c++_static` build. The compile flags are the r18b ndk-build defaults, including
  `-fdata-sections`, `-DNDEBUG`, `-fpic`, `-fno-exceptions -fno-rtti` followed by
  `-frtti -fexceptions`, `-mfpu=vfpv3-d16` followed by `-mfpu=neon`, `-mthumb -Oz`,
  `-g`, `-fstack-protector-strong`, `-funwind-tables`, `-ffunction-sections`.
* At the current tree state, the only authentic compile flag that still changes the
  gate is `-fdata-sections`, and it is **negative**: app TUs `-fdata-sections` gives
  **byte 4108 / linked 4495** vs baseline **4115 / 4495**; adding it to the vendored
  zlib/libzip too gives **4110 / 4494**. Do not add it until the data/TU layout is
  faithful.
* `-DNDEBUG`, `-fpic` (vs `-fPIC`), the ndk-build `-fno-exceptions -fno-rtti -frtti
  -fexceptions` ordering, and `-fno-sized-deallocation` are codegen-neutral here
  (4115 / 4495 / 4524). `-fno-threadsafe-statics`, `-fno-use-cxa-atexit`,
  `-fno-slp-vectorize`, and `-fno-optimize-sibling-calls` are strongly negative.
  `-fno-common` does not even link against the current C TUs.
* The current link line had two non-original switches that are now redundant:
  `-Tdata 0x20cb80` and `--unresolved-symbols=ignore-all`. Relinking without either
  produces a **byte-identical** `.so` (same sha256) and the same gate. They were removed
  from `CMakeLists.txt` in this pass. `--exclude-libs=libgcc.a` is still required.
* The current `.init_array` is already exactly the original's three entries and the
  three initializer functions are byte-identical. The old "orig 3 vs our 1" note is
  stale. The only extra ELF section is `.aezdata` (8 bytes, source artifact), which
  shifts `.bss` by `+0x10` and the RW `LOAD` by `+0x8/+0x10`.

## 1. Baseline

Commands:

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
```

Baseline output:

```text
verify: byte 4115  linked 4495  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
=== DRIFT: 0 field(s) across 0 class(es) ===
```

`report.json` rows: **4524/4524 size-exact**, `lint` CLEAN. Baseline `.so` sha256:

```text
895d5e63b207945eacb0675031d82c42c2d6f651c8156106607ae91c0fe5ec1d  cmake-build-match/libgof2hdaa.so
d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483  ../_work/bins/android_2.0.16_libgof2hdaa.so
```

## 2. Primary build-system evidence

The original ELF fixes the toolchain and ABI:

```sh
readelf -p .comment ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -n         ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -A         ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -dW        ../_work/bins/android_2.0.16_libgof2hdaa.so
```

* `.comment`: `Android (4751641 based on r328903) clang version 7.0.2`
  and `GCC: (GNU) 4.9.x 20150123 (prerelease)`.
* `.note.android.ident`: API 16, `r18b`, build `5063045`.
* `.note.gnu.gold-version`: `gold 1.12`.
* `.ARM.attributes`: ARMv7-A, Thumb-2, VFPv3, NEONv1, GOT-indirect, 4-byte
  `wchar_t`, int enums, 8-byte alignment, unaligned v6.
* `DT_NEEDED` order: `liblog, libGLESv2, libGLESv1_CM, libEGL, libandroid,
  libfmodex, libfmodevent, libm, libdl, libc`; no `libc++_shared.so`.
* `DT_FLAGS = BIND_NOW`, `DT_FLAGS_1 = NOW`, `DT_SONAME = libgof2hdaa.so`,
  `.hash` (no `.gnu.hash`), `.dynamic` size `0x148`.

These are exactly the r18b ndk-build outputs. The r18b `build/core` files on this
machine say so explicitly:

```sh
sed -n '130,165p' .cache/ndk/android-ndk-r18b/build/core/default-build-commands.mk
sed -n '35,115p' .cache/ndk/android-ndk-r18b/build/core/toolchains/arm-linux-androideabi-clang/setup.mk
sed -n '180,230p' .cache/ndk/android-ndk-r18b/build/core/build-binary.mk
```

Relevant defaults: `GLOBAL_CFLAGS` has `-fdata-sections -ffunction-sections
-fstack-protector-strong -funwind-tables -g`; `GLOBAL_CXXFLAGS` appends
`-fno-exceptions -fno-rtti`; `TARGET_CFLAGS` has `-fpic -march=armv7-a
-mfloat-abi=softfp -mfpu=vfpv3-d16`; `TARGET_CFLAGS.neon` is `-mfpu=neon`;
`TARGET_thumb_release_CFLAGS` is `-mthumb -Oz -DNDEBUG`; `TARGET_RELRO_LDFLAGS`
is `-Wl,-z,relro -Wl,-z,now`; `TARGET_NO_UNDEFINED_LDFLAGS` is
`-Wl,--no-undefined`; `TARGET_LDLIBS` is `-lc -lm`.

The clang driver's own `-###` output for the current target also shows the
defaults that matter for code shape:

```sh
.cache/ndk/android-ndk-r18b/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++ \
  -target armv7-none-linux-androideabi16 -march=armv7-a -mthumb -mfpu=neon \
  -mfloat-abi=softfp -std=c++11 -fPIC -frtti -fstack-protector-strong -Oz -g \
  -### -c /tmp/empty.cpp 2>&1 | tail -20
```

It contains `-mdisable-fp-elim`, `-fmath-errno`, `-vectorize-slp`,
`-fuse-init-array`, `-fcxx-exceptions`, `-fexceptions`, `-stack-protector 2`.
So frame pointers, math errno, SLP vectorization, `.init_array`, RTTI, and
exceptions are all expected defaults, not extra flags.

## 3. Current ELF equality vs the original

Commands:

```sh
readelf -SW ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -SW cmake-build-match/libgof2hdaa.so
readelf -lW ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -lW cmake-build-match/libgof2hdaa.so
readelf -dW ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -dW cmake-build-match/libgof2hdaa.so
```

Current section table (address / size, original vs ours):

| section | original | ours | byte diff |
|---|---:|---:|---:|
| `.text` | `0x69440` / `0x1337c8` | same | 197423 |
| `.ARM.exidx` | `0x19cc08` / `0x6f70` | same | 14216 |
| `.ARM.extab` | `0x1a3b78` / `0x26298` | same | 114142 |
| `.rodata` | `0x1c9e10` / `0x41b67` | same | 101025 |
| `.fini_array` | `0x20cba0` / `0x8` | same | **0** |
| `.data.rel.ro` | `0x20cbb0` / `0x2fe8` | same | 910 |
| `.init_array` | `0x20fb98` / `0xc` | same | **0** |
| `.dynamic` | `0x20fba4` / `0x148` | same | 3 |
| `.got` | `0x20fcf4` / `0x430c` | same | **0** |
| `.data` | `0x214000` / `0x296c` | same | 1626 |
| `.bss` | `0x216970` / `0x69ac` | `0x216980` / `0x69ac` | n/a |
| `.plt` | `0x5e7ec` / `0xac4c` | same | **0** |
| `.comment` | — / `0x12f` | same | **0** |
| `.note.android.ident` | `0x134` / `0x98` | same | **0** |
| `.note.gnu.gold-version` | — / `0x1c` | same | **0** |
| `.ARM.attributes` | — / `0x38` | same | **0** |

The only extra section is `.aezdata` at `0x21696c`, size 8, from
`src/engine/file/AEFile.cpp` (`#pragma clang section data=".aezdata"`). It is
the sole cause of `.bss` being `+0x10` and the RW `LOAD` filesz/memsz being
`+0x8/+0x10`; it is a source artifact, not a link flag.

`.dynamic` differs in exactly one field: `RELCOUNT` is **1161** ours vs
**1162** original. Correspondingly the relocation counts are
`R_ARM_ABS32 1818 vs 1817` and `R_ARM_RELATIVE 1161 vs 1162`; `R_ARM_GLOB_DAT`
(614) and `R_ARM_JUMP_SLOT` (3674) are exact. This is a source/layout delta,
not a link-flag delta.

`.dynsym` has the same 5578 entries and size `0x15ca0`; 1409 names are at the
same index and `difflib.SequenceMatcher` finds 115 matching blocks. The order
is a TU/object-order residue, not a flag.

### 3.1 `.init_array` / `.fini_array` are exact

```sh
readelf -x .init_array ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -x .init_array cmake-build-match/libgof2hdaa.so
readelf -x .fini_array ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -x .fini_array cmake-build-match/libgof2hdaa.so
```

Both `.init_array` dumps are `41940600 c1940600 dd940600`; both `.fini_array`
dumps are `48950600 34950600`. To name the entries, relink the same objects
without the final `strip` and run `nm`:

```sh
# use the ld.gold line from `ninja -C cmake-build-match -t commands libgof2hdaa.so`,
# replace the output path with /tmp/w3413-unstripped.so, and omit the strip command
.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm \
  -n /tmp/w3413-unstripped.so | grep -E '00069440|000694c0|000694dc|GLOBAL__sub_I'
```

Result:

```text
00069440 t _GLOBAL__sub_I_Engine.cpp
000694c0 t _GLOBAL__sub_I_LODManager.cpp
000694dc t _GLOBAL__sub_I_ParticleSettings.cpp
```

The bytes at `0x69440..0x694c0`, `0x694c0..0x694dc`, and `0x694dc..0x694fc`
are all identical to the original (diff 0). The `.fini_array` entries are
`__on_dlclose` (`0x69534`) and `__on_dlclose_late` (`0x69548`), also byte-identical.
This supersedes the stale `IDENTICALITY.md` / `AGENTS.md` lines that say the
tree has only one `.init_array` entry; both are corrected in this commit.

## 4. Controlled compile-flag A/B

Scratch toolchain used for every variant (it appends `GOF2_EXTRA_FLAGS` after the
repo's forced `CMAKE_CXX_FLAGS`/`CMAKE_C_FLAGS`):

```sh
cat > /tmp/w3413_flags_toolchain.cmake <<'EOF'
include("/home/debian/GalaxyOnFire2/wt/w3413/cmake/toolchain.cmake")
set(GOF2_EXTRA_FLAGS "" CACHE STRING "extra matching flags")
if(GOF2_EXTRA_FLAGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GOF2_EXTRA_FLAGS}" CACHE STRING "matching ARM compile flags" FORCE)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GOF2_EXTRA_FLAGS}" CACHE STRING "matching ARM compile flags" FORCE)
endif()
EOF
```

Per variant:

```sh
cmake -S . -B /tmp/w3413-<variant> -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/tmp/w3413_flags_toolchain.cmake \
  -DGOF2_MATCH_OPT=-Oz \
  -DGOF2_EXTRA_FLAGS='<flags>'
cmake --build /tmp/w3413-<variant> --target check -j8
```

`check` runs the full ratchet (link, verify, data, sodiff, lint). Measured table
(all at base `103bcc42`; `size` is `osize == usize` rows):

| variant | byte | linked | size | missing | verdict |
|---|---:|---:|---:|---:|---|
| baseline (no extra flags) | **4115** | **4495** | 4524/4524 | 0 | reference |
| `-DNDEBUG` | 4115 | 4495 | 4524/4524 | 0 | neutral |
| `-DNDEBUG -fpic -fno-exceptions -fno-rtti -frtti -fexceptions` | 4115 | 4495 | 4524/4524 | 0 | neutral |
| `-fdata-sections` (app TUs only) | 4108 | 4495 | 4524/4524 | 0 | **-7 byte** |
| `-fdata-sections` (vendored zlib/libzip only) | 4115 | 4494 | 4524/4524 | 0 | neutral byte, -1 linked |
| `-fdata-sections` (app + vendored) | 4110 | 4494 | 4524/4524 | 0 | -5 byte |
| `-fno-sized-deallocation` | 4115 | 4495 | 4524/4524 | 0 | neutral |
| `-fno-threadsafe-statics` | 1344 | 4491 | 4520/4521 | 3 | catastrophic; loses `__cxa_guard_*` |
| `-fno-use-cxa-atexit` | 1348 | 4494 | 4523/4524 | 0 | catastrophic |
| `-fno-slp-vectorize` | 1411 | 4381 | 4408/4524 | 0 | catastrophic |
| `-fno-optimize-sibling-calls` | 1316 | 3585 | 3686/4524 | 0 | catastrophic |
| `-fno-common` | — | — | — | — | link error: duplicate `g_android_back_button_pressed` |

The vendored-lib rows are not reachable through `GOF2_EXTRA_FLAGS`: zlib/libzip
are compiled by `third_party/build_match_lib.cmake` with its own `CC_BASE`, not
the CMake C flags. For those rows the exact edit was to append
`-fdata-sections` to `CC_BASE` (line 27 of that file), rebuild, and revert;
the app+vendored row combines that edit with `GOF2_EXTRA_FLAGS=-fdata-sections`.

The `-fno-common` link failure is direct evidence that the current C TUs rely on
tentative/common definitions; the original clang 7 default is `-fcommon`, so do
not add `-fno-common` without reshaping those definitions.

Interpretation:

* `-fdata-sections` is authentic (r18b `GLOBAL_CFLAGS`) but currently negative.
  The app-only and vendored-only results are not additive: app-only `-7` byte,
  vendored-only `0` byte/-1 linked, both `-5` byte/-1 linked. This is a data
  layout/section-ordering interaction, not evidence against ndk-build.
* `-DNDEBUG`, `-fpic`, and the r18b RTTI/exception flag ordering are codegen-neutral
  at this tree state. They are safe to add for authenticity, but not a gate lever.
* `-fno-threadsafe-statics` and `-fno-use-cxa-atexit` are strongly refuted: the
  original imports `__cxa_atexit` and exports `__cxa_guard_acquire/release/abort`.
* `-fno-sized-deallocation` is neutral; the binary cannot distinguish it. The
  original's weak `_ZdlPvj`/`_ZdaPvj` exports come from libc++abi regardless.
* `-fno-slp-vectorize` and `-fno-optimize-sibling-calls` are refuted; clang 7's
  defaults (`-vectorize-slp`, sibling calls) are correct.

## 5. Controlled link-flag A/B

The exact gold command is the `ld.gold -shared` line from:

```sh
ninja -C cmake-build-match -t commands libgof2hdaa.so | grep 'ld.gold -shared' | tail -1
```

Relink variants were made by substituting the output path and deleting/replacing
one switch, then running the same `tools/verify/verify.py` used by `check`.

| relink | sha256 | byte | linked | size | notes |
|---|---|---:|---:|---:|---|
| baseline (`-Tdata 0x20cb80`, `--unresolved-symbols=ignore-all`) | `895d5e63…` | 4115 | 4495 | 4524/4524 | reference |
| remove `-Tdata 0x20cb80` | `895d5e63…` | 4115 | 4495 | 4524/4524 | **byte-identical** |
| remove `--unresolved-symbols=ignore-all` | `895d5e63…` | 4115 | 4495 | 4524/4524 | **byte-identical** |
| remove both | `895d5e63…` | 4115 | 4495 | 4524/4524 | **byte-identical** |
| remove `-Tdata`, replace ignore-all with `--no-undefined` | `895d5e63…` | 4115 | 4495 | 4524/4524 | **byte-identical** |
| remove `--exclude-libs=libgcc.a` | `526f88a5…` | — | — | — | `.dynsym` 0x15cd0, `.text` 0x1337d8, `.got` 0x4314; required |

Commands used:

```sh
python3 - <<'PY'
import subprocess
lines = subprocess.check_output(
    ['ninja','-C','cmake-build-match','-t','commands','libgof2hdaa.so'],
    text=True).splitlines()
ld = [l for l in lines if 'ld.gold -shared' in l][-1]
cmd = ld.replace('cmake-build-match/libgof2hdaa.so', '/tmp/w3413-no-tdata.so')
cmd = cmd.replace(' -Tdata 0x20cb80', '')
# other rows: also delete ' --unresolved-symbols=ignore-all', or replace it with
# ' --no-undefined' (the --no-undefined row also deletes -Tdata)
subprocess.check_call(cmd, shell=True)
PY
python3 tools/verify/verify.py \
  --build-dir /tmp/w3413-no-tdata-verify \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3413-no-tdata.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
```

Conclusion: at the current tree state both `-Tdata 0x20cb80` and
`--unresolved-symbols=ignore-all` are redundant. They were removed from
`CMakeLists.txt`; the resulting link is byte-identical and the full `check` gate
is still **byte 4115 / linked 4495 / size 4524/4524 / lint CLEAN / drift 0**.
`--exclude-libs=libgcc.a` remains required.

## 6. What is proven, and what is still hypothesis

Proven by the binary and the controlled rebuilds:

* NDK r18b, clang 7.0.2, gold 1.12; armeabi-v7a softfp; API 16; `c++_static`;
  RTTI and exceptions are on; NEON is on; Thumb-2; `-Oz`; `-g`;
  `-fstack-protector-strong`; `-funwind-tables`; `-ffunction-sections`.
* The original compile command is the r18b ndk-build default set, including
  `-fdata-sections`, `-DNDEBUG`, `-fpic`, `-fno-exceptions -fno-rtti` then
  `-frtti -fexceptions`, `-mfpu=vfpv3-d16` then `-mfpu=neon`, `-mthumb -Oz`.
  The current tree omits or reorders several of these, but only
  `-fdata-sections` changes the gate today (negatively).
* The original link has `-z now`, `--build-id`, `--exclude-libs=libgcc.a`,
  `-z relro`, `-z noexecstack`, `--hash-style=sysv`, `--spare-dynamic-tags=5`,
  `-z combreloc`, and `--no-as-needed`; the current link reproduces the
  `.dynamic`, `.plt`, `.got`, `.init_array`, `.fini_array`, notes, and ARM
  attributes exactly (except the one `RELCOUNT`/relocation swap noted above).
* `.init_array` is exactly three entries and all three initializer functions are
  byte-identical; `.fini_array` is exactly two entries and both are byte-identical.
* `-Tdata 0x20cb80` and `--unresolved-symbols=ignore-all` are not needed at this
  tree state; removing them is byte-identical.
* `.aezdata` is the only extra ELF section; it is a source artifact from
  `AEFile.cpp`, not a build flag.
* `-fno-threadsafe-statics`, `-fno-use-cxa-atexit`, `-fno-slp-vectorize`, and
  `-fno-optimize-sibling-calls` are refuted by large gate drops.
* `-fno-common` is incompatible with the current C TUs (duplicate tentative
  definitions).

Still hypothesis (not recoverable from the stripped `.so`):

* The original build system was **ndk-build** from NDK r18b, not CMake; no
  original `Android.mk`/`Application.mk` survives. The flag set, `.note.android.ident`,
  and the r18b `build/core` defaults are the evidence.
* The exact `Application.mk`/`Android.mk` text (`APP_STL := c++_static`,
  `LOCAL_ARM_NEON := true`, `LOCAL_CPP_FEATURES := rtti exceptions`,
  `LOCAL_WHOLE_STATIC_LIBRARIES := zlib libzip`, crypto static, FMOD in
  `LOCAL_LDLIBS` vs `LOCAL_SHARED_LIBRARIES`) is a reconstruction, not recovered text.
* The exact `LOCAL_SRC_FILES` list and object order are not recoverable from the
  stripped `.so`; `_work/original_layout/` and `.dynsym` constrain them.
* Whether the original explicitly wrote `-fdata-sections`, `-DNDEBUG`, `-fpic`, or
  the RTTI/exception flags, or got them from ndk-build defaults, cannot be
  distinguished. The binary is consistent with the defaults.
* `-fno-sized-deallocation` is indistinguishable (neutral).
* The remaining `.dynamic` `RELCOUNT` swap and the `.dynsym`/`.rel.plt` order are
  source/TU/link-order residues, not link flags.

## 7. Reproduction summary

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8

# compile-flag A/B
cat > /tmp/w3413_flags_toolchain.cmake <<'EOF'
include("/home/debian/GalaxyOnFire2/wt/w3413/cmake/toolchain.cmake")
set(GOF2_EXTRA_FLAGS "" CACHE STRING "extra matching flags")
if(GOF2_EXTRA_FLAGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GOF2_EXTRA_FLAGS}" CACHE STRING "matching ARM compile flags" FORCE)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GOF2_EXTRA_FLAGS}" CACHE STRING "matching ARM compile flags" FORCE)
endif()
EOF
cmake -S . -B /tmp/w3413-fdata -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/tmp/w3413_flags_toolchain.cmake \
  -DGOF2_MATCH_OPT=-Oz -DGOF2_EXTRA_FLAGS=-fdata-sections
cmake --build /tmp/w3413-fdata --target check -j8

# link-flag A/B: extract the exact gold line, substitute output, delete one switch
ninja -C cmake-build-match -t commands libgof2hdaa.so | grep 'ld.gold -shared' | tail -1
```
