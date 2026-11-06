# Build flags: stock NDK r18b CMake equivalence (wave 120, main)

Status: research note, 2026-11-06. Base `ef7fb9cde`. No `src/`, `tools/`, baseline or
allowlist change. This note records a fresh, independent equivalence test of the
current hand-written `cmake/toolchain.cmake` against the **stock NDK r18b CMake
toolchain defaults**.

Canonical gate at the base:

```text
verify: byte 4505  linked 4505  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
size-exact 4524/4524; lint CLEAN; drift 0
```

## 0. Bottom line

The current compile recipe is **byte-for-byte equivalent to the stock NDK r18b
CMake toolchain defaults** for this source tree:

* A stock CMake project configured with
  `ANDROID_ABI=armeabi-v7a`, `ANDROID_PLATFORM=android-16`,
  `ANDROID_STL=c++_static`, `ANDROID_ARM_NEON=ON`, `CMAKE_BUILD_TYPE=Release`
  emits the same codegen-visible flags as `cmake/toolchain.cmake`.
* Compiling **all 192 app TUs** with that stock toolchain (plus the two
  existing per-file branch-weight overrides) produces **byte-identical object
  code**: 192/192 TUs, 0 differing functions, 0 symbol-set differences.
* Relinking the canonical objects with the **full CMake/ndk-build default link
  flag set** produces the **same `.so` hash** as the current canonical link
  (`2e167e84053ff7ecda46968c2e346fedf720ec0d7366c372bfbd44af47f43330`).

Therefore the remaining 19 non-byte-exact functions are not explained by a
missing/incorrect compiler or linker flag. The only non-default build inputs
are the two per-file branch-weight matching knobs and the checked-in
`tools/mission_case_table.cpp`; both are source/TU matching aids, not evidence
of an original global flag.

## 1. Stock CMake compile line

Probe:

```sh
NDK=$PWD/.cache/ndk/android-ndk-r18b
cmake -S probe -B probe/build -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake \
  -DANDROID_ABI=armeabi-v7a -DANDROID_PLATFORM=android-16 \
  -DANDROID_STL=c++_static -DANDROID_ARM_NEON=ON \
  -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```

The emitted C++ command is:

```text
clang++ --target=armv7-none-linux-androideabi16
  --gcc-toolchain=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64
  --sysroot=$NDK/sysroot
  -isystem $NDK/sources/cxx-stl/llvm-libc++/include
  -isystem $NDK/sources/android/support/include
  -isystem $NDK/sources/cxx-stl/llvm-libc++abi/include
  -isystem $NDK/sysroot/usr/include/arm-linux-androideabi
  -g -DANDROID -ffunction-sections -funwind-tables -fstack-protector-strong
  -no-canonical-prefixes -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16
  -mthumb -mfpu=neon -Wa,--noexecstack -Wformat -Werror=format-security
  -std=c++11 -Oz -DNDEBUG -fPIC
```

The current `cmake/toolchain.cmake` line is the same effective set:
`-target armv7-none-linux-androideabi16 -march=armv7-a -mthumb -mfpu=neon
-mfloat-abi=softfp -std=c++11 -fPIC -frtti -fstack-protector-strong -Oz -g
-stdlib=libc++ -D__ANDROID_API__=16 -DANDROID -DNDEBUG -ffunction-sections
-funwind-tables`. The textual differences (`-target` vs `--target=`,
`--gcc-toolchain`, `-no-canonical-prefixes`, `-Wa,--noexecstack`, `-Wformat`,
`-Werror=format-security`, `-mfpu=vfpv3-d16` before `-mfpu=neon`, flag order)
are codegen-neutral on this tree.

## 2. All-TU object equivalence

A scratch stock-CMake project listed exactly the 192 paths in
`tools/source_order.txt`, added the same include directories and
`-ffunction-sections -funwind-tables`, and applied the existing per-file
branch-weight overrides to `StarMap.cpp` and `SpaceLounge.cpp`. It was
configured with the stock toolchain settings above and built with `-j64`.

Comparison used `tools/flag_fingerprint.py`'s `object_functions()` (defined
`STT_FUNC` bytes per ET_REL object) against the canonical
`cmake-build-match/CMakeFiles/gof2.dir/<TU>.obj` objects:

```text
TUs compared 192
TUs with differences 0
```

This covers every app TU, not just the 19 non-exact functions' TUs. The result
is stronger than a gate count: it proves the current hand-written toolchain and
the stock CMake Release toolchain generate identical function bodies.

## 3. Full default link-flag equivalence

The canonical gold command was re-run with the full CMake/ndk-build default
link flag set added:

```text
--no-undefined --fix-cortex-a8
--exclude-libs=libatomic.a --exclude-libs=libunwind.a
-z noexecstack -z relro --warn-shared-textrel --fatal-warnings
--hash-style=sysv --no-as-needed
-z max-page-size=4096 -z common-page-size=4096 --spare-dynamic-tags=5
```

The result was stripped and `.shstrtab`-normalized exactly like the canonical
link. Its SHA-256 is identical to the canonical build:

```text
variant  2e167e84053ff7ecda46968c2e346fedf720ec0d7366c372bfbd44af47f43330
current  2e167e84053ff7ecda46968c2e346fedf720ec0d7366c372bfbd44af47f43330
```

Section digests are identical too. The only sections that differ from the
original are the known residuals: `.text`, `.ARM.exidx`, `.ARM.extab`,
`.rodata` (+192 mission table), `.rel.plt` (source-order transient) and
`.note.gnu.build-id`.

## 4. What is proven / not proven

**Proven by this pass**

* The current compile flags are equivalent to stock NDK r18b CMake Release
  defaults for every app TU.
* The current link is byte-identical to the full CMake/ndk-build default link
  flag set.
* No tested global compile or link flag can explain the 19 remaining
  non-exact functions; they are source-shape residuals.

**Not proven / still unobservable**

* The exact original build driver (CMake vs ndk-build vs a custom script) is
  not recoverable from the stripped `.so`. The stock CMake defaults are the
  closest single explanation because they need no `-fdata-sections` override
  and put FMOD before the STL archives.
* The exact CMake version, original build directory, and original unstripped
  link output are not recoverable; `.note.gnu.build-id` remains a hash of that
  unknown output.
* The two per-file branch-weight overrides are matching knobs, not proven
  original global flags.

## 5. Reproduction

```sh
# stock compile-line probe
NDK=$PWD/.cache/ndk/android-ndk-r18b
cmake -S probe -B probe/build -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake \
  -DANDROID_ABI=armeabi-v7a -DANDROID_PLATFORM=android-16 \
  -DANDROID_STL=c++_static -DANDROID_ARM_NEON=ON \
  -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# full object comparison
python3 - <<'PY'
import sys
sys.path.insert(0, 'tools')
import flag_fingerprint as ff
# compare ff.object_functions(stock_obj) with ff.object_functions(canonical_obj)
PY
```
