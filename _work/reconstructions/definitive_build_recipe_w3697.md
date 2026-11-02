# Definitive build recipe: how the original `libgof2hdaa.so` was built (w3697)

Status: research note, 2026-11-02. Worktree `wt/w3697`, branch `wave87-w3697`,
HEAD `9dbaf4df`. Every number below is from a command run against
`../_work/bins/android_2.0.16_libgof2hdaa.so` or from a controlled A/B rebuild of this
tree. No `src/` or build-flag change is landed: this commit contains this note plus a
stale-number correction in `docs/IDENTICALITY.md` (§8).

Baseline (unchanged, measured before and after the experiments):

```text
verify: byte 4496  linked 4496  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
=== DRIFT: 0 field(s) across 0 class(es) ===
CLEAN ✓ (no new soft violations; hard invariants 0)
```

`.text` = `0x1337c8` at `0x69440` (exact); `report.json` = 4524/4524 size-exact.
Our `.so` sha256 `84b28fe629284c3b95df009910fc8990e3fbaff148826b23eb28d9f3bf1bfef6`;
original `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`.

## 0. Bottom line

The original was built with **NDK r18b clang 7.0.2 + gold 1.12**, target
`armv7-none-linux-androideabi16`, armeabi-v7a, Thumb-2, NEON, softfp, static libc++.
Its **effective compile flags are exactly the ndk-build r18b default set minus
`-fdata-sections`** (equivalently, with `-fno-data-sections`). Its **effective link
flags are exactly the ndk-build r18b default set**; the tree's shorter gold line is
byte-equivalent to the full ndk-build line. The remaining non-default part is the
**link input order**: the original places the FMOD shared libraries before
`libc++abi.a` and does not link `libc++_static.a`/`libunwind.a` (or links them only
where no member is pulled). This is why the original's `_Unwind_*` symbols stay
`UND` and `__gnu_Unwind_Find_exidx` is absent. Vanilla ndk-build `c++_static` order
(static archives before shared libraries) defines `_Unwind_*` locally and adds
`__gnu_Unwind_Find_exidx`; a controlled rebuild with that order drops the gate from
4496 to 1345 byte-exact and grows `.text` from `0x1337c8` to `0x135d0c`.

The original was linked **without** a section-ordering file. The tree's
`--section-ordering-file` is an approved reconstruction aid (it reproduces the
original `.text` order while our TU membership diverges), not an original build flag.

## 1. Toolchain identity (hard evidence from the original)

```sh
readelf -hW  ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -AW  ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -nW  ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -p .comment ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -dW  ../_work/bins/android_2.0.16_libgof2hdaa.so
```

| observable | value |
|---|---|
| ELF | `ELF32`, `DYN`, `ARM`, `Version5 EABI, soft-float ABI` |
| `.comment` | `Android (4751641 based on r328903) clang version 7.0.2` + `GCC: (GNU) 4.9.x 20150123 (prerelease)` |
| `.note.android.ident` | API `16`, `r18b`, build `5063045` |
| `.note.gnu.gold-version` | `gold 1.12` |
| `.ARM.attributes` | ARM v7, Thumb-2, VFPv3, NEONv1, GOT-indirect, wchar_t 4, enum int, 8-byte align, unaligned v6 |
| `DT_SONAME` | `libgof2hdaa.so` |
| `DT_NEEDED` | `liblog, libGLESv2, libGLESv1_CM, libEGL, libandroid, libfmodex, libfmodevent, libm, libdl, libc` |
| `DT_FLAGS` / `DT_FLAGS_1` | `BIND_NOW` / `NOW` |
| `DT_RELCOUNT` | `1162` |
| `.init_array` / `.fini_array` | 3 entries / 2 entries |
| defined / undefined dynsym | 5373 / 208 |
| `.rel.dyn` / `.rel.plt` | 3593 / 3674 entries |

`.note.android.ident`, `.comment`, `.note.gnu.gold-version`, `.ARM.attributes`,
`.init_array`, `.fini_array`, `.dynamic`, `.got` and `.plt` are byte-identical between
the tree's build and the original. The GCC 4.9.x `.comment` string comes from the NDK's
crt/libgcc objects, not from the app TUs.

## 2. The build system: ndk-build r18b defaults, with two deviations

### 2.1 Exact ndk-build r18b command lines (controlled sample)

A minimal `ndk-build` project (`/tmp/ndktest`) with `APP_ABI := armeabi-v7a`,
`APP_STL := c++_static`, `APP_PLATFORM := android-16`, `APP_OPTIM := release`,
`LOCAL_CPP_FEATURES := rtti exceptions`, `LOCAL_ARM_NEON := true` and
`LOCAL_LDLIBS := -llog` was built with the repo's NDK r18b and `V=1`. The emitted
commands are the authoritative r18b defaults:

```text
$NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++ \
  -target armv7-none-linux-androideabi16 \
  -fdata-sections -ffunction-sections -fstack-protector-strong -funwind-tables \
  -no-canonical-prefixes -g \
  -Wno-invalid-command-line-argument -Wno-unused-command-line-argument \
  -fno-exceptions -fno-rtti \
  -gcc-toolchain $NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64 \
  -fpic -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16 \
  -mthumb -Oz -DNDEBUG -mfpu=neon \
  -I$NDK/sources/cxx-stl/llvm-libc++/include \
  -I$NDK/sources/cxx-stl/llvm-libc++abi/include \
  -I$NDK/sources/android/support/include \
  -std=c++11 -DANDROID -Wa,--noexecstack -Wformat -Werror=format-security \
  -frtti -fexceptions \
  --sysroot=$NDK/sysroot \
  -isystem $NDK/sysroot/usr/include/arm-linux-androideabi \
  -c file.cpp -o file.o
```

```text
$NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++ \
  -Wl,-soname,libt.so -shared \
  --sysroot=$NDK/platforms/android-16/arch-arm \
  <objects> \
  $NDK/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libc++_static.a \
  $NDK/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libc++abi.a \
  $NDK/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libandroid_support.a \
  $NDK/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libunwind.a \
  -lgcc -Wl,--exclude-libs,libgcc.a \
  -latomic -Wl,--exclude-libs,libatomic.a \
  -target armv7-none-linux-androideabi16 -no-canonical-prefixes \
  -gcc-toolchain $NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64 \
  -Wl,--fix-cortex-a8 -Wl,--exclude-libs,libunwind.a \
  -Wl,--build-id -nostdlib++ \
  -Wl,--no-undefined -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now \
  -Wl,--warn-shared-textrel -Wl,--fatal-warnings \
  -L$NDK/platforms/android-16/arch-arm/usr/lib \
  -llog -ldl -lc -lm \
  -o libt.so
```

The original's link is the same line with `-Wl,-soname,libgof2hdaa.so` and
`-o libgof2hdaa.so`.

The sample's `DT_NEEDED` order is `liblog, libdl, libc, libm` (the `-ldl` is
`libunwind`'s exported `LOCAL_EXPORT_LDLIBS`), and its `_Unwind_*` symbols are
**defined locally and hidden** by `--exclude-libs,libunwind.a`; its dynsym contains
`__gnu_Unwind_Find_exidx` as `UND`.

### 2.2 The tree's canonical compile command

For comparison, the tree's `Level.cpp` compile command is:

```text
clang++ -target armv7-none-linux-androideabi16 -march=armv7-a -mthumb -mfpu=neon \
  -mfloat-abi=softfp -std=c++11 -fPIC -frtti -fstack-protector-strong -Oz -g \
  -stdlib=libc++ -isystem .../llvm-libc++/include -isystem .../llvm-libc++abi/include \
  -isystem .../android/support/include --sysroot=... -D__ANDROID_API__=16 \
  -Wno-int-to-pointer-cast -Wno-int-to-void-pointer-cast -I... \
  -ffunction-sections -funwind-tables -c file.cpp -o file.o
```

The two sets are the same codegen flags in a different order, with these textual
differences: `-fPIC` vs `-fpic`, `-mfpu=neon` vs `-mfpu=vfpv3-d16 -mfpu=neon`,
`-std=c++11`/`-frtti`/exceptions on vs `-fno-exceptions -fno-rtti -frtti -fexceptions`,
and the ndk-build-only `-fdata-sections`, `-DNDEBUG`, `-DANDROID`,
`-Wa,--noexecstack`, `-Wformat -Werror=format-security`, `-gcc-toolchain`,
`-no-canonical-prefixes` and `-Wno-*`. Every one of those textual differences was
measured at the current baseline (§3.2): all are byte-identical **except
`-fdata-sections`**.

## 3. Controlled A/B at the current baseline

Method: scratch build dirs under `/tmp/w3697`, a toolchain wrapper that appends
`GOF2_EXTRA_FLAGS` to both `CMAKE_CXX_FLAGS` and `CMAKE_C_FLAGS`, then
`cmake --build <dir> --target link_so -j8` and the repo verifier. The canonical
`cmake-build-match` build was not modified; `--update-baseline` was never used.

```sh
cmake -S . -B /tmp/w3697/build_<tag> -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/tmp/w3697/toolchain_extra.cmake \
  -DGOF2_MATCH_OPT=-Oz -DGOF2_EXTRA_FLAGS='<flags>'
cmake --build /tmp/w3697/build_<tag> --target link_so -j8
python3 tools/verify/verify.py --build-dir /tmp/w3697/verify_<tag> \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3697/build_<tag>/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
```

### 3.1 Compile-flag matrix (new measurements at byte 4496)

`sha` is the `.so` sha256; "identical" means exactly the baseline
`84b28f…fef6`. `.rodata`/`.data` are section sizes.

| variant | byte | linked | `.rodata` | `.data` | `.so` |
|---|---:|---:|---:|---:|---|
| canonical (`-Oz -g -fPIC -frtti -fstack-protector-strong -ffunction-sections -funwind-tables`) | **4496** | **4496** | `0x41b67` | `0x296c` | base |
| `-fpic` | 4496 | 4496 | `0x41b67` | `0x296c` | identical |
| `-DNDEBUG` | 4496 | 4496 | `0x41b67` | `0x296c` | identical |
| `-fno-exceptions -fno-rtti -frtti -fexceptions` | 4496 | 4496 | `0x41b67` | `0x296c` | identical |
| `-mfpu=vfpv3-d16 -mfpu=neon` | 4496 | 4496 | `0x41b67` | `0x296c` | identical |
| `-gcc-toolchain … -no-canonical-prefixes -Wno-invalid-command-line-argument -Wno-unused-command-line-argument` | 4496 | 4496 | `0x41b67` | `0x296c` | identical |
| `-DANDROID -Wa,--noexecstack -Wformat -Werror=format-security` | 4496 | 4496 | `0x41b67` | `0x296c` | identical |
| `-fno-data-sections` | 4496 | 4496 | `0x41b67` | `0x296c` | identical |
| `-fdata-sections -fno-data-sections` | 4496 | 4496 | `0x41b67` | `0x296c` | identical |
| **`-fdata-sections`** | **4333** | 4496 | `0x41a07` | `0x2970` | differs |
| full ndk-build compile replica **without** `-fdata-sections` | 4496 | 4496 | `0x41b67` | `0x296c` | identical |
| full ndk-build compile replica **with** `-fdata-sections` | 4333 | 4496 | `0x41a07` | `0x2970` | same as `-fdata-sections` |
| `-g0` | 2069 | 4245 | — | — | differs |
| `-gline-tables-only` | 2069 | 4245 | — | — | same as `-g0` |
| `-g2` | 4496 | 4496 | `0x41b67` | `0x296c` | identical |
| `-g3` | 4496 | 4496 | `0x41b67` | `0x296c` | identical |

The full ndk-build replica is the exact sample command's flag set (API 16, `-fpic`,
`-fdata-sections`, `-DNDEBUG`, `-fno-exceptions -fno-rtti -frtti -fexceptions`,
`-mfpu=vfpv3-d16 -mfpu=neon`, `-gcc-toolchain`, `-no-canonical-prefixes`,
`-Wno-*`, `-DANDROID`, `-Wa,--noexecstack`, `-Wformat -Werror=format-security`).
Its `.so` sha is exactly the `-fdata-sections`-alone sha (`a9787a05…2942b`), proving
every other ndk-build compile flag is codegen-neutral here.

The rest of the compile-flag evidence is unchanged from
`_work/reconstructions/compile_flag_matrix_w3690.md` (74 variants at the same
4496 baseline): `-Oz` is uniquely best; `-Os` 1263, `-O1` 1033, `-O2` 1168,
`-O3` 1167; `-fomit-frame-pointer` 1298; `-fno-stack-protector` 1369;
`-fstack-protector` 1369; `-fstack-protector-all` 83; `-fno-rtti` 1725;
`-fno-builtin` 1350; `-fno-use-cxa-atexit` 1356; `-fwrapv` 1451;
`-mno-unaligned-access` 1376; `-fno-vectorize -fno-slp-vectorize` 1484;
`-fno-inline-functions` 1219; `-fno-threadsafe-statics` 1368;
`-fvisibility=hidden` 57; `-mcpu=cortex-a8` 1330; `-marm` 57;
`-fno-zero-initialized-in-bss` 1522; `-mno-global-merge` 1397;
`-fwritable-strings` 1425; `-fno-jump-tables` 1363;
`-fno-optimize-sibling-calls` 1316; `-fno-unroll-loops` 2883;
`-ffast-math` 1319; `-fno-math-errno` 1356; `-fno-signed-zeros` 2355;
`-freciprocal-math` 1539; `-fno-common` and `-fno-exceptions` fail to build;
`-mfloat-abi=hard`, `-mfpu=vfpv3-d16` (alone), `-fno-integrated-as` fail to build.
`-fno-omit-frame-pointer`, `-fno-unwind-tables` (appended after the canonical
`-funwind-tables`), `-fno-sized-deallocation`, `-fno-delete-null-pointer-checks`,
`-fmerge-all-constants`, `-fno-short-wchar`, `-DNDEBUG`, `-fno-plt`,
`-fno-use-init-array`, `-mno-restrict-it`, `-fno-inline-functions-called-once`,
`-fno-trapping-math`, `-fno-finite-math-only`, `-faligned-allocation`,
`-fno-aligned-allocation`, `-mno-movt` and `-mllvm -unroll-threshold=0` are
byte-identical.

### 3.2 Link-flag matrix (new measurements at byte 4496)

All variants are relinks of the canonical objects with the exact gold command from
`cmake-build-match/build.ninja`, plus the listed flag(s). "identical" means the same
sha256 as the baseline.

| variant | byte | linked | `.so` |
|---|---:|---:|---|
| canonical (`-shared -soname … --build-id -s -z now --exclude-libs=libgcc.a`) | **4496** | **4496** | base |
| `--no-undefined` | 4496 | 4496 | identical |
| `--fix-cortex-a8` | 4496 | 4496 | identical |
| `--no-fix-cortex-a8` | 4496 | 4496 | identical |
| `-z noexecstack` | 4496 | 4496 | identical |
| `-z relro` | 4496 | 4496 | identical |
| `--warn-shared-textrel --fatal-warnings` | 4496 | 4496 | identical |
| `--build-id=sha1` | 4496 | 4496 | identical |
| `--hash-style=sysv -z combreloc --no-as-needed -z max-page-size=4096 -z common-page-size=4096 --spare-dynamic-tags=5` | 4496 | 4496 | identical |
| full ndk-build link flag set (`--no-undefined -z noexecstack -z relro -z now --warn-shared-textrel --fatal-warnings --fix-cortex-a8 --build-id=sha1 --exclude-libs=libgcc.a`) | 4496 | 4496 | identical |
| `-z lazy` (with `-z now` still present) | 4496 | 4496 | identical |
| **`-z norelro`** | **2932** | 4496 | differs |
| **`-z now` removed** | **2932** | 4496 | differs (`.dynamic` `0x138`) |

The full ndk-build link flag set produces a **byte-identical** `.so` to the tree's
shorter gold line. The only load-bearing link flag is `-z now` (with its implied
RELRO): removing it costs **1564 byte-exact** (4496 -> 2932) and shrinks `.dynamic`
from `0x148` to `0x138`. `--fix-cortex-a8`/`--no-fix-cortex-a8` are indistinguishable
(no Cortex-A8 veneers are needed in this output).

The `-s` (gold's own strip), `--build-id` (SHA-1), `--exclude-libs=libgcc.a`,
`--whole-archive` around zlib/libzip and the `-l log -l GLESv2 -l GLESv1_CM -l EGL
-l android` NEEDED order are established in
`_work/reconstructions/link_flags_research.md` and
`_work/reconstructions/nontext_relplt_shstrtab_w3695.md`; they are unchanged here.

## 4. The one compile-flag deviation: the original did not use `-fdata-sections`

ndk-build r18b always passes `-fdata-sections`. The original's data layout, however,
is the **no-`-fdata-sections` layout**, byte-for-byte:

```sh
# canonical (no -fdata-sections) build
python3 - <<'PY'   # section-by-section byte comparison against the original
...
PY
```

| build | `.rodata` size | differing bytes vs original | `.data` size | differing bytes vs original | `.data.rel.ro` differing bytes |
|---|---:|---:|---:|---:|---:|
| canonical (no `-fdata-sections`) | `0x41b67` | **0** | `0x296c` | **0** | **0** |
| `-fdata-sections` | `0x41a07` | **113,935** | `0x2970` | **3,273** | **405** |

The canonical build's `.rodata`, `.data` and `.data.rel.ro` are all byte-identical to
the original. The `-fdata-sections` build reorders the string/constant sections and
changes the `.data` padding, so its data bytes diverge massively. This is not a
section-ordering-file artifact: a relink of the same objects **without**
`--section-ordering-file` still gives `.rodata` `0x41b67` / 0 differing bytes for the
canonical build and `0x41a07` / 113,935 differing bytes for the `-fdata-sections`
build.

Therefore the original's effective compile flags are the ndk-build r18b defaults
**with `-fdata-sections` disabled** (`-fno-data-sections`). The tree's canonical set
(the clang default `-fno-data-sections`) is correct; adding `-fdata-sections` costs
**163 byte-exact** (4496 -> 4333).

This is a real deviation from vanilla ndk-build. It is consistent with the link-order
deviation in §5: the original was built either by a custom build script that copied
ndk-build's flags but omitted `-fdata-sections`, or by ndk-build with
`LOCAL_CFLAGS := -fno-data-sections` (or `APP_CFLAGS`) and a non-default link order.
The stripped `.so` cannot distinguish those two.

## 5. Link inputs and order: not vanilla ndk-build `c++_static`

The original's dynamic symbol table pins the link order:

```sh
readelf --dyn-syms -W ../_work/bins/android_2.0.16_libgof2hdaa.so \
  | grep -E '_Unwind_|__gnu_Unwind_Find_exidx|_ZNSt3__1'
readelf -dW ../_work/bins/android_2.0.16_libgof2hdaa.so | grep NEEDED
```

| observable | original | vanilla ndk-build `c++_static` sample |
|---|---|---|
| `_Unwind_Resume`, `_Unwind_VRS_Get`, `_Unwind_RaiseException`, `_Unwind_VRS_Set`, `_Unwind_GetLanguageSpecificData`, `_Unwind_DeleteException`, `_Unwind_GetRegionStart` | **`UND`** | defined locally (hidden by `--exclude-libs,libunwind.a`), absent from dynsym |
| `__gnu_Unwind_Find_exidx` | **absent** | `UND` |
| `_ZNSt3__1*` (libc++_static members) | **absent** | absent (no members pulled in the sample either) |
| `DT_NEEDED` order | `log, GLESv2, GLESv1_CM, EGL, android, fmodex, fmodevent, m, dl, c` | `log, dl, c, m` for the sample (`LOCAL_LDLIBS` before libunwind's `-ldl` before `-lc -lm`) |

The original's `_Unwind_*` are resolved from **`libfmodex.so`** at runtime, not from a
linked `libunwind.a`/libgcc unwind member. That requires the FMOD shared libs to be
processed **before** `libc++abi.a` and `libgcc.a` on the link line. Vanilla ndk-build
puts static archives before shared libraries, so it would pull `libunwind.a` and
define `_Unwind_*` locally. A controlled rebuild with the vanilla order confirms it:

| link variant | byte | linked | extra | `.text` | `.dynsym` | `_Unwind_*` | `__gnu_Unwind_Find_exidx` |
|---|---:|---:|---:|---:|---:|---|---|
| canonical (FMOD before `libc++abi.a`) | **4496** | **4496** | 0 | `0x1337c8` | `0x15ca0` | `UND` | absent |
| +`libc++_static.a` before `libc++abi.a` | 4496 | 4496 | 0 | `0x1337c8` | `0x15ca0` | `UND` | absent |
| +`libunwind.a` after `libandroid_support.a` | 4496 | 4496 | 0 | `0x1337c8` | `0x15ca0` | `UND` | absent |
| +`libatomic.a` after `libgcc.a` | 4496 | 4496 | 0 | `0x1337c8` | `0x15ca0` | `UND` | absent |
| +all three (same positions) | 4496 | 4496 | 0 | `0x1337c8` | `0x15ca0` | `UND` | absent |
| +all three appended at the end | 4496 | 4496 | 0 | `0x1337c8` | `0x15ca0` | `UND` | absent |
| FMOD moved after `libgcc.a`; STL archives in ndk order | 1345 | 4402 | 29 | `0x135d0c` | `0x15d60` | defined | `UND` |
| faithful ndk-build order (STL archives before FMOD, `--exclude-libs=libunwind.a`) | 1345 | 3796 | 0 | `0x135d0c` | `0x15b80` | hidden | `UND` |

The "faithful" variant reproduces the original's `DT_NEEDED` order exactly
(`log, GLESv2, GLESv1_CM, EGL, android, fmodex, fmodevent, m, dl, c`) but still
defines `_Unwind_*` (hidden) and adds `__gnu_Unwind_Find_exidx`, and its `.text` is
`0x135d0c` (the original is `0x1337c8`). The original therefore did **not** link
`libunwind.a` (and, by the same ordering evidence, did not link `libc++_static.a` in a
way that pulls members). The tree's current link line — FMOD before `libc++abi.a`,
then `libandroid_support.a`, `libgcc.a`, `-l dl -l c`, no `libc++_static.a` /
`libunwind.a` / `libatomic.a` — is the matching input order.

Adding `libc++_static.a`, `libunwind.a` or `libatomic.a` **after** the FMOD libs is
unobservable (no members are pulled), so their presence cannot be excluded from the
binary alone; what is excluded is the vanilla ndk-build order that puts the STL
archives before FMOD and therefore pulls libunwind.

The original also imports `memalign` (not `posix_memalign`) and has no
`__aeabi_ldivmod` export but does import `__gnu_ldivmod_helper`, consistent with
`libandroid_support.a` and `libgcc.a` being linked with `--exclude-libs=libgcc.a`:

```text
$ readelf --dyn-syms -W ../_work/bins/android_2.0.16_libgof2hdaa.so | grep -E 'memalign|ldivmod'
  1547: 00000000 0 FUNC GLOBAL DEFAULT UND memalign@LIBC (2)
  5267: 00000000 0 FUNC GLOBAL DEFAULT UND __gnu_ldivmod_helper
```

## 6. The definitive recipe

### 6.1 Compile (per C++ TU)

The original's effective command is the ndk-build r18b default command of §2.1 with
`-fdata-sections` replaced by `-fno-data-sections` (or simply omitted). In the tree's
shorter spelling, the equivalent is:

```sh
$NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++ \
  -target armv7-none-linux-androideabi16 \
  -march=armv7-a -mthumb -mfpu=neon -mfloat-abi=softfp \
  -std=c++11 -fPIC -frtti -fstack-protector-strong -Oz -g -stdlib=libc++ \
  -fno-data-sections -ffunction-sections -funwind-tables \
  -isystem $NDK/sources/cxx-stl/llvm-libc++/include \
  -isystem $NDK/sources/cxx-stl/llvm-libc++abi/include \
  -isystem $NDK/sources/android/support/include \
  --sysroot=$NDK/sysroot \
  -isystem $NDK/sysroot/usr/include \
  -isystem $NDK/sysroot/usr/include/arm-linux-androideabi \
  -D__ANDROID_API__=16 \
  -c file.cpp -o file.o
```

Every flag in this command is either the tree's canonical flag or one of the
ndk-build defaults measured neutral in §3.1. The only *effective* deviation from
ndk-build is the absence of `-fdata-sections`.

### 6.2 Link (gold)

The original's link is a single plain gold link. The tree's exact line (from
`cmake-build-match/build.ninja`) is:

```sh
$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/arm-linux-androideabi/bin/ld.gold \
  -shared -soname libgof2hdaa.so --build-id -s -z now \
  --exclude-libs=libgcc.a \
  [--section-ordering-file=<reconstruction aid>] \
  -o libgof2hdaa.so \
  crtbegin_so_renamed.o -L $NDK/platforms/android-16/arch-arm/usr/lib \
  @link_order.rsp \
  --whole-archive libz_gof2.a libzip_gof2.a --no-whole-archive \
  -l log -l GLESv2 -l GLESv1_CM -l EGL -l android \
  $APK/lib/armeabi-v7a/libfmodex.so $APK/lib/armeabi-v7a/libfmodevent.so \
  libcrypto_gof2.a -l m \
  libc++abi.a libandroid_support.a libgcc.a \
  -l dl -l c \
  crtend_so.o
```

`crtbegin_so_renamed.o` is the API-16 `crtbegin_so.o` with `.text` renamed to
`.text.crtbegin` so the section-ordering file can place it; the original used the
plain `crtbegin_so.o` and was linked without an ordering file. The ordering file is
the only reconstruction aid in the line; it is not an original flag.

The link flags are the ndk-build r18b defaults: `-shared -soname … --build-id`,
`-z relro -z now`, `-z noexecstack`, `--no-undefined`, `--warn-shared-textrel`,
`--fatal-warnings`, `--fix-cortex-a8`, `--exclude-libs=libgcc.a`, `-lc -lm`
(and `-ldl` from the STL). The tree's line omits the neutral ones because gold's
defaults already produce the same bytes; §3.2 proves the full ndk-build set is
byte-identical. `-s` is gold's own strip (the original's `.shstrtab` is in gold's
Stringpool order; a post-link GNU `strip --strip-all` rewrites it).

The link input order is the non-default part: FMOD before `libc++abi.a`, no
`libc++_static.a`/`libunwind.a`/`libatomic.a` members pulled. The tree's
`@link_order.rsp` (from `tools/gen_link_order.py`) reproduces the original's object
order; the `.dynsym`/`.dynstr`/`.hash` order follows gold's first-mention order over
that input sequence (see `_work/reconstructions/dynsym_dynstr_hash_order_w3567.md`).

## 7. Proven vs hypothesis

**Proven by measurement (this note or the cited notes):**

* Toolchain: NDK r18b clang 7.0.2, gold 1.12, API 16, armeabi-v7a, Thumb-2, NEON,
  softfp, static libc++ (`.comment`, `.note.android.ident`, `.note.gnu.gold-version`,
  `.ARM.attributes`, `DT_NEEDED`).
* Compile flags: the ndk-build r18b default set is codegen-equivalent to the tree's
  canonical set **except `-fdata-sections`**; all other ndk-build defaults are
  byte-identical (§3.1).
* `-Oz` is uniquely best; `-g` is codegen-relevant (`-g0`/`-gline-tables-only` 2069,
  `-g2`/`-g3` 4496); `-fstack-protector-strong`, `-frtti`, frame pointers, strict
  aliasing, vectorization, threadsafe statics and unaligned access are all on
  (w3690 + §3.1).
* Link flags: the full ndk-build r18b link flag set is **byte-identical** to the
  tree's shorter gold line (§3.2). `-z now` is load-bearing (+1564 byte-exact).
* The original's data layout is the **no-`-fdata-sections`** layout: `.rodata`,
  `.data` and `.data.rel.ro` are byte-identical without it and differ by
  113,935 / 3,273 / 405 bytes with it (§4).
* The original's link order is **not** vanilla ndk-build `c++_static`: FMOD precedes
  `libc++abi.a`; `_Unwind_*` stay `UND`, `__gnu_Unwind_Find_exidx` is absent, and a
  vanilla-order rebuild drops to 1345 byte-exact and grows `.text` to `0x135d0c`
  (§5).
* `-ffunction-sections` is present (193 CANTUNWIND sentinels in `.ARM.exidx`;
  `_work/reconstructions/ffunction_sections_and_order_file.md`).
* The original was linked without a section-ordering file; the tree's file is a
  reconstruction aid (`_work/reconstructions/ffunction_sections_and_order_file.md`).

**Hypothesis / not recoverable from the stripped `.so`:**

* The exact build-system file (`Android.mk`/`Application.mk` vs a custom Makefile)
  is not recoverable. The compile flags are ndk-build r18b's; the `-fdata-sections`
  omission and the link order are not vanilla ndk-build defaults, so the original
  either used a custom build script or overrode ndk-build's defaults.
* `APP_PLATFORM` explicit vs the r18b default 16 cannot be distinguished (both give
  16). `APP_OPTIM` explicit vs default release cannot be distinguished (both give
  `-Oz`). FMOD in `LOCAL_LDLIBS` vs `LOCAL_SHARED_LIBRARIES` cannot be distinguished
  from the NEEDED order alone; the order requires FMOD before the STL archives.
* `libc++_static.a`/`libatomic.a` may have been on the link line; their members are
  not pulled, so their presence is unobservable. `libunwind.a` is excluded by the
  `_Unwind_*`/`__gnu_Unwind_Find_exidx` observables.
* The remaining byte deltas (`.text` 113,935; `.ARM.exidx` 2,010; `.ARM.extab`
  115,871 with size `-0xc`; `.dynsym` 324; `.dynstr` 1,268; `.hash` 133;
  `.rel.dyn` 74; `.rel.plt` 21; build-id 20) are source/TU/function-body and
  link-order residue, not missing build flags. `.rodata`, `.data`,
  `.data.rel.ro`, `.plt`, `.got`, `.dynamic`, `.init_array`, `.fini_array`,
  `.comment`, `.note.*`, `.ARM.attributes` and `.shstrtab` are byte-identical.

## 8. Doc correction landed with this note

`docs/IDENTICALITY.md`'s "Current differing bytes" paragraph and `.ARM.extab`
section-size row were stale. The current measurements are:

| section | orig | ours | delta | differing bytes |
|---|---:|---:|---:|---:|
| `.text` | `0x1337c8` | `0x1337c8` | 0 | 113,935 |
| `.ARM.exidx` | `0x6f70` | `0x6f70` | 0 | 2,010 |
| `.ARM.extab` | `0x26298` | `0x2628c` | **-0xc** | 115,871 |
| `.rodata` | `0x41b67` | `0x41b67` | 0 | 0 |
| `.data.rel.ro` | `0x2fe8` | `0x2fe8` | 0 | 0 |
| `.data` | `0x296c` | `0x296c` | 0 | 0 |
| `.dynsym` | `0x15ca0` | `0x15ca0` | 0 | 324 |
| `.dynstr` | `0x2e318` | `0x2e318` | 0 | 1,268 |
| `.hash` | `0x973c` | `0x973c` | 0 | 133 |
| `.rel.dyn` | `0x7048` | `0x7048` | 0 | 74 |
| `.rel.plt` | `0x72d0` | `0x72d0` | 0 | 21 |
| `.plt`, `.got`, `.dynamic`, `.init_array`, `.fini_array`, `.shstrtab` | — | — | 0 | 0 |

The `.ARM.extab` delta is `-0xc` (156,300 vs 156,312), not the previously stated
`-8` (156,304). The `docs/IDENTICALITY.md` numbers are corrected in this commit.

## 9. Reproduction commands

```sh
# baseline
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
cmake --build cmake-build-match --target lint  -j8

# toolchain / link metadata
readelf -hW  ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -AW  ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -nW  ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -p .comment ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -dW  ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf --dyn-syms -W ../_work/bins/android_2.0.16_libgof2hdaa.so \
  | grep -E '_Unwind_|__gnu_Unwind_Find_exidx|_ZNSt3__1|memalign|ldivmod'

# ndk-build r18b defaults (exact command lines)
NDK=.cache/ndk/android-ndk-r18b
mkdir -p /tmp/ndktest/jni
cat > /tmp/ndktest/jni/Application.mk <<'X'
APP_ABI := armeabi-v7a
APP_STL := c++_static
APP_PLATFORM := android-16
APP_OPTIM := release
X
cat > /tmp/ndktest/jni/Android.mk <<'X'
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := t
LOCAL_SRC_FILES := t.cpp
LOCAL_CPP_FEATURES := rtti exceptions
LOCAL_ARM_NEON := true
LOCAL_LDLIBS := -llog
include $(BUILD_SHARED_LIBRARY)
X
printf '#include <stdexcept>\nextern "C" int f(int x){ if(x) throw std::runtime_error("x"); return x; }\n' \
  > /tmp/ndktest/jni/t.cpp
$NDK/ndk-build NDK_PROJECT_PATH=/tmp/ndktest \
  APP_BUILD_SCRIPT=/tmp/ndktest/jni/Android.mk \
  NDK_APPLICATION_MK=/tmp/ndktest/jni/Application.mk V=1 -B 2>&1 | grep 'clang++'

# compile-flag A/B (scratch; never --update-baseline)
cat > /tmp/w3697/toolchain_extra.cmake <<'X'
include("/home/debian/GalaxyOnFire2/wt/w3697/cmake/toolchain.cmake")
set(GOF2_EXTRA_FLAGS "" CACHE STRING "extra matching flags")
if(GOF2_EXTRA_FLAGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GOF2_EXTRA_FLAGS}" CACHE STRING "" FORCE)
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${GOF2_EXTRA_FLAGS}" CACHE STRING "" FORCE)
endif()
X
cmake -S . -B /tmp/w3697/build_fdata -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/tmp/w3697/toolchain_extra.cmake \
  -DGOF2_MATCH_OPT=-Oz -DGOF2_EXTRA_FLAGS='-fdata-sections'
cmake --build /tmp/w3697/build_fdata --target link_so -j8

# link-flag A/B: relink the canonical objects with extra gold args
ninja -C cmake-build-match -t commands libgof2hdaa.so | grep 'ld.gold -shared' | tail -1
# (replace -o .../libgof2hdaa.so and insert the extra flag after -shared)

# section-by-section byte comparison
python3 - <<'PY'
import struct
def secs(path):
    b=open(path,'rb').read()
    e_shoff=struct.unpack_from('<I',b,0x20)[0]; e_shentsize=struct.unpack_from('<H',b,0x2e)[0]
    e_shnum=struct.unpack_from('<H',b,0x30)[0]; e_shstrndx=struct.unpack_from('<H',b,0x32)[0]
    sh=[struct.unpack_from('<IIIIIIIIII',b,e_shoff+i*e_shentsize) for i in range(e_shnum)]
    st=sh[e_shstrndx]; stb=b[st[4]:st[4]+st[5]]
    return b,{stb[s[0]:stb.find(b'\0',s[0])].decode():s for s in sh}
ob,os_=secs('../_work/bins/android_2.0.16_libgof2hdaa.so')
b,sh=secs('cmake-build-match/libgof2hdaa.so')
for n in ['.text','.ARM.exidx','.ARM.extab','.rodata','.data.rel.ro','.data',
          '.dynsym','.dynstr','.hash','.rel.dyn','.rel.plt','.plt','.got','.dynamic']:
    s,o=sh[n],os_[n]; m=min(s[5],o[5])
    d=sum(1 for x,y in zip(b[s[4]:s[4]+m],ob[o[4]:o[4]+m]) if x!=y)
    print(f'{n:16s} size {s[5]:#8x} orig {o[5]:#8x} delta {s[5]-o[5]:+#x} diff {d}')
PY
```
