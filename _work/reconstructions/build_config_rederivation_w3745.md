# Independent build-configuration re-derivation (w3745)

Status: research note, 2026-11-02. Worktree `wt/w3745`, branch `wave87-w3745`,
based on current `master` (`b55d3518`, "docs: lock Level::createMission 87.3%").
Every number below is from a command run against
`../_work/bins/android_2.0.16_libgof2hdaa.so` or from a controlled A/B rebuild/relink
of this tree. No build flag or source change is landed: the tree's current flags are
already the best measured set. This commit contains this note only.

## 0. Bottom line

The original `libgof2hdaa.so` was built with **NDK r18b clang 7.0.2 + gold 1.12**,
target `armv7-none-linux-androideabi16`, armeabi-v7a, Thumb-2, NEON, softfp,
static libc++. Its **effective compile flags are the ndk-build r18b default set
with `-fdata-sections` disabled**. Its **effective link flags are the ndk-build r18b
default set**. The non-default part is the **link input order**: FMOD precedes
`libc++abi.a`, and no `libunwind.a`/`libc++_static.a`/`libatomic.a` member is pulled.

The independent measurements confirm every load-bearing flag in the tree and add one
correction to the earlier research record:

* **`-funwind-tables` is load-bearing, not neutral.** A proper removal (filtering the
  flag out of the command line, not appending `-fno-unwind-tables` before the later
  `-funwind-tables`) drops `byte_exact` **4497 -> 2878**. The earlier "neutral"
  result in `compile_flag_matrix_w3690.md` was a flag-order artifact: the wrapper
  appended `GOF2_EXTRA_FLAGS` to `CMAKE_CXX_FLAGS`, but `-funwind-tables` is a
  `target_compile_options` flag that appears later and therefore won.
* **`-ffunction-sections` is load-bearing**: a proper removal drops `byte_exact`
  **4497 -> 1345** (`linked_exact` 4356) and shrinks `.text` from `0x1337c8` to
  `0x133154`. The original's 194 duplicate-address `.ARM.exidx` entries are the
  independent structural witness for it.
* **`-fdata-sections` must be off**: with it, `.rodata`/`.data`/`.data.rel.ro` differ
  from the original by **113,935 / 3,273 / 405 bytes** and `byte_exact` drops
  **4497 -> 4334**. Without it, all three sections are byte-identical, even when the
  section-ordering file is removed.
* No tested compile or link flag is a strict improvement over the current tree.
  `-Oz`, `-g`, `-fPIC`, `-frtti`, `-fstack-protector-strong`, `-ffunction-sections`,
  `-funwind-tables`, `-march=armv7-a -mthumb -mfpu=neon -mfloat-abi=softfp`,
  `-std=c++11`, `-z now`, `--build-id` (SHA-1), `--hash-style=sysv`,
  `-z combreloc`, `--spare-dynamic-tags=5`, `--no-as-needed`, `-z max-page-size=4096`,
  `--exclude-libs=libgcc.a`, `--whole-archive` for zlib/libzip, and `-s` are all
  confirmed.

## 1. Baseline (before and after all experiments)

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
cmake --build cmake-build-match --target lint  -j8
```

```text
verify: byte 4497  linked 4497  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
=== DRIFT: 0 field(s) across 0 class(es) ===
CLEAN ✓ (no new soft violations; hard invariants 0)
```

`report.json`: 4524/4524 size-exact. `.text` = `0x1337c8` at `0x69440` (exact).
The current tree is at **byte 4497 / linked 4497**, not the 4496/4496 quoted in the
task text; the extra function is the `Level::createMission` pass already on `master`.

```text
ours     8aa715fb116d346da79e78514e77d0b250117aa84e6058bcaae058e8546203eb
original d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483
```

## 2. Toolchain identity (independent `readelf`)

```sh
O=../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -hW  $O
readelf -AW  $O
readelf -nW  $O
readelf -p .comment $O
readelf -dW  $O
```

| observable | original | ours |
|---|---|---|
| ELF | ELF32 DYN ARM, `Version5 EABI, soft-float ABI` | same |
| `.comment` | `Android (4751641 based on r328903) clang version 7.0.2` + `GCC: (GNU) 4.9.x 20150123 (prerelease)` | **byte-identical** |
| `.note.android.ident` | API `16`, `r18b`, build `5063045` | **byte-identical** |
| `.note.gnu.gold-version` | `gold 1.12` | **byte-identical** |
| `.ARM.attributes` | ARM v7, Thumb-2, VFPv3, NEONv1, GOT-indirect, wchar_t 4, enum int, 8-byte align, unaligned v6 | **byte-identical** |
| `DT_SONAME` | `libgof2hdaa.so` | same |
| `DT_NEEDED` | `log, GLESv2, GLESv1_CM, EGL, android, fmodex, fmodevent, m, dl, c` | **same order** |
| `DT_FLAGS` / `DT_FLAGS_1` | `BIND_NOW` / `NOW` | same |
| `DT_RELCOUNT` | `1162` | same |
| `.init_array` / `.fini_array` | 3 entries / 2 entries | **byte-identical** |
| program headers | 8: 2 `LOAD`, `DYNAMIC`, `NOTE`, `GNU_STACK`, `ARM_EXIDX`, `GNU_RELRO` | **all types/order/flags/alignments/addresses identical** |

The ELF header, program headers, `.ARM.attributes`, `.comment` and
`.note.android.ident` are byte-identical between the original and the current build.
This pins the toolchain and ABI independently of any source reconstruction.

## 3. The ndk-build r18b default command lines (controlled sample)

A minimal `ndk-build` project (`/tmp/w3745/ndktest`) with `APP_ABI := armeabi-v7a`,
`APP_STL := c++_static`, `APP_PLATFORM := android-16`, `APP_OPTIM := release`,
`LOCAL_CPP_FEATURES := rtti exceptions`, `LOCAL_ARM_NEON := true`,
`LOCAL_LDLIBS := -llog` was built with the repo's NDK r18b and `V=1`:

```sh
NDK=.cache/ndk/android-ndk-r18b
$NDK/ndk-build NDK_PROJECT_PATH=/tmp/w3745/ndktest \
  APP_BUILD_SCRIPT=/tmp/w3745/ndktest/jni/Android.mk \
  NDK_APPLICATION_MK=/tmp/w3745/ndktest/jni/Application.mk V=1 -B
```

The emitted compile command is (paths abbreviated):

```text
clang++ -target armv7-none-linux-androideabi16 \
  -fdata-sections -ffunction-sections -fstack-protector-strong -funwind-tables \
  -no-canonical-prefixes -g \
  -Wno-invalid-command-line-argument -Wno-unused-command-line-argument \
  -fno-exceptions -fno-rtti -gcc-toolchain $NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64 \
  -fpic -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16 -mthumb -Oz -DNDEBUG \
  -mfpu=neon -I$NDK/sources/cxx-stl/llvm-libc++/include \
  -I$NDK/sources/cxx-stl/llvm-libc++abi/include \
  -I$NDK/sources/android/support/include -std=c++11 -DANDROID \
  -Wa,--noexecstack -Wformat -Werror=format-security -frtti -fexceptions \
  --sysroot=$NDK/sysroot \
  -isystem $NDK/sysroot/usr/include/arm-linux-androideabi \
  -c file.cpp -o file.o
```

The emitted link command is:

```text
clang++ -Wl,-soname,libt.so -shared --sysroot=$NDK/platforms/android-16/arch-arm \
  <objects> \
  $NDK/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libc++_static.a \
  $NDK/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libc++abi.a \
  $NDK/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libandroid_support.a \
  $NDK/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libunwind.a \
  -lgcc -Wl,--exclude-libs,libgcc.a -latomic -Wl,--exclude-libs,libatomic.a \
  -target armv7-none-linux-androideabi16 -no-canonical-prefixes \
  -gcc-toolchain $NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64 \
  -Wl,--fix-cortex-a8 -Wl,--exclude-libs,libunwind.a -Wl,--build-id \
  -nostdlib++ -Wl,--no-undefined -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now \
  -Wl,--warn-shared-textrel -Wl,--fatal-warnings \
  -L$NDK/platforms/android-16/arch-arm/usr/lib -llog -ldl -lc -lm -o libt.so
```

The original's link is the same line with `-Wl,-soname,libgof2hdaa.so` and
`-o libgof2hdaa.so`, plus the link-input-order deviation in §9.

## 4. Compile-flag A/B matrix

Method: a scratch toolchain wrapper appends `GOF2_EXTRA_FLAGS` to both
`CMAKE_CXX_FLAGS` and `CMAKE_C_FLAGS`:

```cmake
# /tmp/w3745/toolchain_extra.cmake
include("/home/debian/GalaxyOnFire2/wt/w3745/cmake/toolchain.cmake")
set(GOF2_EXTRA_FLAGS "" CACHE STRING "extra matching flags")
if(GOF2_EXTRA_FLAGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GOF2_EXTRA_FLAGS}" CACHE STRING "" FORCE)
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${GOF2_EXTRA_FLAGS}" CACHE STRING "" FORCE)
endif()
```

Per variant:

```sh
cmake -S . -B /tmp/w3745/build_<tag> -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/tmp/w3745/toolchain_extra.cmake \
  -DGOF2_MATCH_OPT=-Oz -DGOF2_EXTRA_FLAGS='<flags>'
cmake --build /tmp/w3745/build_<tag> --target link_so -j8
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/verify_<tag> \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/build_<tag>/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
```

The scratch baseline `.so` is byte-identical to the canonical
`cmake-build-match/libgof2hdaa.so` (sha256 `8aa715fb…6203eb`), so "identical" below
means the same sha256 as the canonical build.

### 4.1 Built variants (full measured table)

| variant | flags | byte | linked | missing | `.so` vs canonical |
|---|---|---:|---:|---:|---|
| `DANDROID` | `-DANDROID` | 4497 | 4497 | 0 | identical |
| `DNDEBUG` | `-DNDEBUG` | 4497 | 4497 | 0 | identical |
| `api21` | `-target armv7-none-linux-androideabi21 -D__ANDROID_API__=21` | 4497 | 4497 | 0 | identical |
| `base` | `(canonical)` | 4497 | 4497 | 0 | identical |
| `faligned` | `-faligned-allocation` | 4497 | 4497 | 0 | identical |
| `fnodata` | `-fno-data-sections` | 4497 | 4497 | 0 | identical |
| `format_werror` | `-Wformat -Werror=format-security` | 4497 | 4497 | 0 | identical |
| `fpic` | `-fpic` | 4497 | 4497 | 0 | identical |
| `g2` | `-g2` | 4497 | 4497 | 0 | identical |
| `g3` | `-g3` | 4497 | 4497 | 0 | identical |
| `gnu11` | `-std=gnu++11` | 4497 | 4497 | 0 | identical |
| `mergeallconst` | `-fmerge-all-constants` | 4497 | 4497 | 0 | identical |
| `ndk_exc_rtti` | `-fno-exceptions -fno-rtti -frtti -fexceptions` | 4497 | 4497 | 0 | identical |
| `no_canonical` | `-no-canonical-prefixes` | 4497 | 4497 | 0 | identical |
| `noaligned` | `-fno-aligned-allocation` | 4497 | 4497 | 0 | identical |
| `noasynunwind` | `-fno-asynchronous-unwind-tables` | 4497 | 4497 | 0 | identical |
| `nodeletenull` | `-fno-delete-null-pointer-checks` | 4497 | 4497 | 0 | identical |
| `noexecstack_asm` | `-Wa,--noexecstack` | 4497 | 4497 | 0 | identical |
| `nofinitemath` | `-fno-finite-math-only` | 4497 | 4497 | 0 | identical |
| `noinitarray` | `-fno-use-init-array` | 4497 | 4497 | 0 | identical |
| `noinlineonce` | `-fno-inline-functions-called-once` | 4497 | 4497 | 0 | identical |
| `nomovt` | `-mno-movt` | 4497 | 4497 | 0 | identical |
| `noomitfp` | `-fno-omit-frame-pointer` | 4497 | 4497 | 0 | identical |
| `norestrictit` | `-mno-restrict-it` | 4497 | 4497 | 0 | identical |
| `noshortwchar` | `-fno-short-wchar` | 4497 | 4497 | 0 | identical |
| `nosizeddealloc` | `-fno-sized-deallocation` | 4497 | 4497 | 0 | identical |
| `notrapping` | `-fno-trapping-math` | 4497 | 4497 | 0 | identical |
| `nounwind_appended` | `-fno-unwind-tables` appended to `CMAKE_CXX_FLAGS` | 4497 | 4497 | 0 | identical (flag-order trap) |
| `noplt` | `-fno-plt` | 4497 | 4497 | 0 | identical |
| `shortwchar` | `-fshort-wchar` | 4497 | 4497 | 0 | identical |
| `unroll0` | `-mllvm -unroll-threshold=0` | 4497 | 4497 | 0 | identical |
| `vfp_then_neon` | `-mfpu=vfpv3-d16 -mfpu=neon` | 4497 | 4497 | 0 | identical |
| `neonvfpv4` | `-mfpu=neon-vfpv4` | 4497 | 4497 | 0 | differs |
| `fdata` | `-fdata-sections` | 4334 | 4497 | 0 | differs |
| `nounroll` | `-fno-unroll-loops` | 2892 | 4490 | 0 | differs |
| `nounwind` | `-fno-unwind-tables` (with `-funwind-tables` removed) | 2878 | 4497 | 0 | differs |
| `nosignedzeros` | `-fno-signed-zeros` | 2371 | 4488 | 0 | differs |
| `gline` | `-gline-tables-only` | 2069 | 4246 | 0 | differs |
| `g1` | `-g1` | 2069 | 4246 | 0 | differs |
| `g0` | `-g0` | 2069 | 4246 | 0 | differs |
| `fnortti` | `-fno-rtti` | 1725 | 4497 | 0 | differs |
| `reciprocal` | `-freciprocal-math` | 1539 | 4402 | 0 | differs |
| `nozerobss` | `-fno-zero-initialized-in-bss` | 1522 | 4487 | 0 | differs |
| `gnu14` | `-std=gnu++14` | 1512 | 4484 | 0 | differs |
| `novec` | `-fno-vectorize -fno-slp-vectorize` | 1484 | 4383 | 0 | differs |
| `nostrictoverflow` | `-fno-strict-overflow` | 1460 | 4461 | 0 | differs |
| `fwrapv` | `-fwrapv` | 1460 | 4461 | 0 | differs |
| `writablestrings` | `-fwritable-strings` | 1425 | 4488 | 0 | differs |
| `nounaligned` | `-mno-unaligned-access` | 1404 | 4442 | 0 | differs |
| `noglobalmerge` | `-mno-global-merge` | 1397 | 4487 | 0 | differs |
| `nostrictalias` | `-fno-strict-aliasing` | 1371 | 4349 | 0 | differs |
| `nostackprot` | `-fno-stack-protector` | 1369 | 3781 | 0 | differs |
| `stackprot` | `-fstack-protector` | 1369 | 3785 | 0 | differs |
| `nothreadsafe` | `-fno-threadsafe-statics` | 1368 | 4493 | 3 | differs |
| `nojumptables` | `-fno-jump-tables` | 1363 | 4409 | 0 | differs |
| `nomatherrno` | `-fno-math-errno` | 1356 | 4486 | 0 | differs |
| `nocxaatexit` | `-fno-use-cxa-atexit` | 1356 | 4496 | 0 | differs |
| `nobuiltin` | `-fno-builtin` | 1350 | 3985 | 0 | differs |
| `nofuncsec` | `-fno-function-sections` (with `-ffunction-sections` removed) | 1345 | 4356 | 0 | differs |
| `cortexa8` | `-mcpu=cortex-a8` | 1330 | 4225 | 0 | differs |
| `fastmath` | `-ffast-math` | 1319 | 4225 | 0 | differs |
| `nosiblingcalls` | `-fno-optimize-sibling-calls` | 1316 | 3587 | 0 | differs |
| `omitfp` | `-fomit-frame-pointer` | 1298 | 1922 | 0 | differs |
| `visinlineshidden` | `-fvisibility-inlines-hidden` | 1278 | 4203 | 141 | differs |
| `Os` | `-Os` | 1263 | 2650 | 168 | differs |
| `noinline` | `-fno-inline-functions` | 1219 | 3511 | 0 | differs |
| `O2` | `-O2` | 1168 | 2159 | 228 | differs |
| `O3` | `-O3` | 1167 | 2119 | 229 | differs |
| `O1` | `-O1` | 1033 | 1783 | 0 | differs |
| `stackprotall` | `-fstack-protector-all` | 91 | 961 | 0 | differs |
| `hidden` | `-fvisibility=hidden` | 57 | 243 | 4280 | differs |
| `marm` | `-marm` | 57 | 237 | 0 | differs |

### 4.2 Build failures

| variant | flags | failure |
|---|---|---|
| `hardfp` | `-mfloat-abi=hard` | gold: objects use VFP register arguments, output does not |
| `nocommon` | `-fno-common` | gold: multiple definition of `g_android_back_button_pressed` (GOF2HD2012.c vs BackButton.c) |
| `noexceptions` | `-fno-exceptions` | `Level.cpp:5203: error: cannot use 'try' with exceptions disabled` |
| `nointegratedas` | `-fno-integrated-as` | `as: unrecognized option '-mfpu=neon'` |
| `softfp_soft` | `-mfloat-abi=soft` | `arm_neon.h:28: error: "NEON support not enabled"` |
| `vfpv3d16` | `-mfpu=vfpv3-d16` | `arm_neon.h:28: error: "NEON support not enabled"` |

### 4.3 Compile-flag conclusions

* `-Oz` is uniquely best. The next-best optimization level (`-O1`) loses 3,464
  byte-exact functions; `-Os`/`-O2`/`-O3` are also far worse.
* `-g` is codegen-relevant at `-Oz`: `-g0`, `-g1` and `-gline-tables-only` drop to
  2069; `-g2`/`-g3` are identical to `-g`.
* `-fPIC` and `-fpic` are byte-identical; the original could have used either
  (ndk-build's default is `-fpic`). The tree's `-fPIC` is consistent.
* `-frtti` is required (`-fno-rtti` 1725); exceptions are required (the source uses
  `try`, and `-fno-exceptions` fails to build).
* `-fstack-protector-strong` is required; `-fstack-protector` and
  `-fno-stack-protector` both drop to 1369; `-all` is catastrophic (91).
* Frame pointers are kept: `-fomit-frame-pointer` drops to 1298;
  `-fno-omit-frame-pointer` is identical.
* Strict aliasing, strict overflow, unaligned access, vectorization, inlining,
  jump tables, threadsafe statics and zero-initialized-BSS are all on.
* `-std=c++11` and `-std=gnu++11` are byte-identical; `-std=gnu++14` is much worse
  (1512). The exact `c++11` vs `gnu++11` spelling is underdetermined.
* The ndk-build textual differences (`-fpic`, `-DNDEBUG`, `-DANDROID`,
  `-Wa,--noexecstack`, `-Wformat -Werror=format-security`, `-no-canonical-prefixes`,
  `-fno-exceptions -fno-rtti -frtti -fexceptions`,
  `-mfpu=vfpv3-d16 -mfpu=neon`) are all codegen-neutral.
* The target API level is **not** distinguished by codegen: an
  `armv7-none-linux-androideabi21` / `__ANDROID_API__=21` rebuild is byte-identical
  to the API-16 build. The `.note.android.ident` (API 16, from the NDK's API-16
  `crtbegin_so.o`) is the evidence that pins the platform to 16.
* `-mfpu=neon-vfpv4` is gate-neutral (4497/4497) but produces a different `.so`
  (sha `02fa0dba84ef…`); the original's `.ARM.attributes` says VFPv3, so
  `-mfpu=neon` is the correct choice.
* `-fshort-wchar` and `-fno-short-wchar` are byte-identical in the linked `.so`
  (the linker merges `Tag_ABI_PCS_wchar_t` back to 4); the object-level attribute is
  the only witness, and the original's linked attribute is 4.

## 5. Correction: `-funwind-tables` is load-bearing

The earlier `compile_flag_matrix_w3690.md` lists `-fno-unwind-tables` as neutral.
That measurement appended `-fno-unwind-tables` to `CMAKE_CXX_FLAGS`, but the
canonical `-funwind-tables` is a `target_compile_options` flag that appears **after**
`CMAKE_CXX_FLAGS` in the compile command, so the later `-funwind-tables` won. The
"neutral" result was a no-op.

A proper removal uses a compiler wrapper that filters `-funwind-tables` out and
appends `-fno-unwind-tables` at the end:

```sh
#!/bin/bash
# /tmp/w3745/clangxx_nounwind
args=()
for a in "$@"; do
  if [ "$a" != "-funwind-tables" ]; then args+=("$a"); fi
done
exec .../clang++ "${args[@]}" -fno-unwind-tables
```

With that wrapper (and the matching C wrapper), the build gives:

| build | byte | linked | `.text` | `.ARM.exidx` | `.ARM.extab` | `.rodata`@ | `.data`@ | `.bss`@ |
|---|---:|---:|---:|---:|---:|---:|---:|---:|
| canonical (`-funwind-tables`) | **4497** | 4497 | `0x1337c8`@`0x69440` | `0x6f70` | `0x2628c` | `0x1c9e10` | `0x214000` | `0x216970` |
| proper `-fno-unwind-tables` | **2878** | 4497 | `0x1337c8`@`0x69440` | `0x6678` | `0x25560` | `0x1c87e0` | `0x213000` | `0x215970` |

The `.text` size is unchanged, but the `.ARM.exidx`/`.ARM.extab` shrink, which moves
`.rodata` and every later section by `-0x1630`; every PC-relative literal-pool word
to `.rodata`/`.data`/`.got` then changes. The `.text` differing-byte count between
the two builds is 16,382, while `.rodata` content is identical (0 differing bytes) —
i.e. the code is the same, only the addresses moved. The original's
`.ARM.exidx = 0x6f70` and `.ARM.extab = 0x26298` (ours `0x2628c`, the known `-0xc`
source residue) match the **with-unwind-tables** build, so the original used
`-funwind-tables`.

## 6. `-ffunction-sections` is load-bearing and structurally witnessed

The original's `.ARM.exidx` has **3566 entries**, of which **272 are CANTUNWIND**,
**856 are inline compact**, **2438 point into `.ARM.extab`**, and **194 share their
target address with another entry**. The duplicate-address entries are gold's
`fix_exidx_coverage()` sentinels for the empty plain `.text` section that every
`-ffunction-sections` object carries. Our build reproduces the same counts
address-for-address:

```text
orig: entries=3566 cantunwind=272 inline=856 extab=2438 duplicate_addresses=194
ours: entries=3566 cantunwind=272 inline=856 extab=2438 duplicate_addresses=194
```

A proper removal of `-ffunction-sections` (compiler wrapper filtering the flag)
gives `byte_exact 1345 / linked_exact 4356`, `.text 0x133154` (vs `0x1337c8`),
`.ARM.exidx 0x67f0` (vs `0x6f70`). The flag is required both structurally and for
the section-ordering reconstruction.

## 7. `-fdata-sections` must be off (data-layout evidence)

The original's data layout is the **no-`-fdata-sections`** layout, byte-for-byte:

| build | `.rodata` size | differing bytes | `.data` size | differing bytes | `.data.rel.ro` differing bytes |
|---|---:|---:|---:|---:|---:|
| canonical (no `-fdata-sections`) | `0x41b67` | **0** | `0x296c` | **0** | **0** |
| `-fdata-sections` | `0x41a07` | **113,935** | `0x2970` | **3,273** | **405** |

This is not a section-ordering-file artifact. Relinking the same objects **without**
`--section-ordering-file` still gives:

```text
canonical + no ordering file: .rodata 0x41b67 (0 differing bytes),
                              .data 0x296c (0), .data.rel.ro 0x2fe8 (0)
-fdata-sections + no ordering: .rodata 0x41a07 (113,935),
                              .data 0x295c (3,041), .data.rel.ro 0x2fe8 (1,189)
```

The gate confirms it: `-fdata-sections` drops `byte_exact` **4497 -> 4334**.
The tree's canonical set relies on clang's default `-fno-data-sections`; that is
correct. (`-fno-data-sections` explicitly is byte-identical to the canonical build.)

## 8. Link-flag A/B matrix

Method: relink the canonical objects with the exact gold command from
`cmake-build-match/build.ninja`, replacing only the output path and inserting the
listed extra gold arguments. The base command (without `-z now`) is:

```sh
GOLD=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/arm-linux-androideabi/bin/ld.gold
$GOLD -shared -soname libgof2hdaa.so --build-id "$@" -s \
  --exclude-libs=libgcc.a \
  --section-ordering-file=$B/section_order.txt \
  -o OUT \
  $B/crtbegin_so_renamed.o -L $NDK/platforms/android-16/arch-arm/usr/lib \
  @$B/exidx_markers/link_order.rsp \
  --whole-archive $B/exidx_markers/archives/libz_gof2.a \
                  $B/exidx_markers/archives/libzip_gof2.a --no-whole-archive \
  -l log -l GLESv2 -l GLESv1_CM -l EGL -l android \
  ../_work/bins/apk_libs/libfmodex.so ../_work/bins/apk_libs/libfmodevent.so \
  $B/exidx_markers/archives/libcrypto_gof2.a -l m \
  $B/exidx_markers/archives/libc++abi.a \
  $B/exidx_markers/archives/libandroid_support.a \
  $B/exidx_markers/archives/libgcc.a \
  -l dl -l c \
  $NDK/platforms/android-16/arch-arm/usr/lib/crtend_so.o
```

`relink.sh OUT -z now` reproduces the canonical `.so` byte-for-byte
(sha256 `8aa715fb…6203eb`), which validates the method.

| variant | extra gold args | byte | linked | `.text` | `.ARM.exidx` | `.rodata` | `.got`@ | `.bss`@ | `.dynamic` | DT_NEEDED | DT_FLAGS | RELCOUNT |
|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| `now` | canonical `-z now` | 4497 | 4497 | `0x1337c8` | `0x6f70` | `0x41b67` | `0x20fcf4` | `0x216970` | `0x148` | 10 | 8 | 1162 |
| `lazy` | no `-z now` | 2932 | 4497 | `0x1337c8` | `0x6f70` | `0x41b67` | `0x21296c` | `0x216c80` | `0x138` | 10 | — | 1162 |
| `now_relro` | `-z relro` | 4497 | 4497 | `0x1337c8` | `0x6f70` | `0x41b67` | `0x20fcf4` | `0x216970` | `0x148` | 10 | 8 | 1162 |
| `now_norelro` | `-z norelro` | 2932 | 4497 | `0x1337c8` | `0x6f70` | `0x41b67` | `0x20c148` | `0x215de0` | `0x148` | 10 | 8 | 1162 |
| `now_bid_sha1` | `--build-id=sha1` | 4497 | 4497 | `0x1337c8` | `0x6f70` | `0x41b67` | `0x20fcf4` | `0x216970` | `0x148` | 10 | 8 | 1162 |
| `now_bid_md5` | `--build-id=md5` | 1969 | 4497 | `0x1337c8` | `0x6f70` | `0x41b67` | `0x20fcf4` | `0x216970` | `0x148` | 10 | 8 | 1162 |
| `now_bid_none` | `--build-id=none` | 1750 | 4497 | `0x1337c8` | `0x6f70` | `0x41b67` | `0x20fcf4` | `0x216970` | `0x148` | 10 | 8 | 1162 |
| `now_hash_sysv` | `--hash-style=sysv` | 4497 | 4497 | `0x1337c8` | `0x6f70` | `0x41b67` | `0x20fcf4` | `0x216970` | `0x148` | 10 | 8 | 1162 |
| `now_hash_gnu` | `--hash-style=gnu` | 1750 | 4497 | `0x1337c8` | `0x6f70` | `0x41b67` | `0x210cf4` | `0x217970` | `0x148` | 10 | 8 | 1162 |
| `now_hash_both` | `--hash-style=both` | 1750 | 4497 | `0x1337c8` | `0x6f70` | `0x41b67` | `0x21acf4` | `0x221970` | `0x150` | 10 | 8 | 1162 |
| `now_spare0` | `--spare-dynamic-tags=0` | 4486 | 4497 | `0x1337c8` | `0x6f70` | `0x41b67` | `0x20fcf4` | `0x216970` | `0x120` | 10 | 8 | 1162 |
| `now_spare5` | `--spare-dynamic-tags=5` | 4497 | 4497 | `0x1337c8` | `0x6f70` | `0x41b67` | `0x20fcf4` | `0x216970` | `0x148` | 10 | 8 | 1162 |
| `now_spare10` | `--spare-dynamic-tags=10` | 4486 | 4497 | `0x1337c8` | `0x6f70` | `0x41b67` | `0x20fcf4` | `0x216970` | `0x170` | 10 | 8 | 1162 |
| `now_noasneeded` | `--no-as-needed` | 4497 | 4497 | `0x1337c8` | `0x6f70` | `0x41b67` | `0x20fcf4` | `0x216970` | `0x148` | 10 | 8 | 1162 |
| `now_asneeded` | `--as-needed` | 1750 | 4497 | `0x1337c8` | `0x6f70` | `0x41b67` | `0x20fcf4` | `0x216970` | `0x128` | 6 | 8 | 1162 |
| `now_nocomb` | `-z nocombreloc` | 4486 | 4497 | `0x1337c8` | `0x6f70` | `0x41b67` | `0x20fcf4` | `0x216970` | `0x140` | 10 | 8 | — |
| `now_maxpage4096` | `-z max-page-size=4096` | 4497 | 4497 | `0x1337c8` | `0x6f70` | `0x41b67` | `0x20fcf4` | `0x216970` | `0x148` | 10 | 8 | 1162 |
| `now_maxpage65536` | `-z max-page-size=65536` | 2932 | 4497 | `0x1337c8` | `0x6f70` | `0x41b67` | `0x22bcf4` | `0x232970` | `0x148` | 10 | 8 | 1162 |
| `now_commonpage4096` | `-z common-page-size=4096` | 4497 | 4497 | `0x1337c8` | `0x6f70` | `0x41b67` | `0x20fcf4` | `0x216970` | `0x148` | 10 | 8 | 1162 |
| `now_noexecstack` | `-z noexecstack` | 4497 | 4497 | `0x1337c8` | `0x6f70` | `0x41b67` | `0x20fcf4` | `0x216970` | `0x148` | 10 | 8 | 1162 |
| `now_execstack` | `-z execstack` | 4497 | 4497 | `0x1337c8` | `0x6f70` | `0x41b67` | `0x20fcf4` | `0x216970` | `0x148` | 10 | 8 | 1162 |
| `now_noundef` | `--no-undefined` | 4497 | 4497 | `0x1337c8` | `0x6f70` | `0x41b67` | `0x20fcf4` | `0x216970` | `0x148` | 10 | 8 | 1162 |
| `now_fixcortex` | `--fix-cortex-a8` | 4497 | 4497 | `0x1337c8` | `0x6f70` | `0x41b67` | `0x20fcf4` | `0x216970` | `0x148` | 10 | 8 | 1162 |
| `now_warnfatal` | `--warn-shared-textrel --fatal-warnings` | 4497 | 4497 | `0x1337c8` | `0x6f70` | `0x41b67` | `0x20fcf4` | `0x216970` | `0x148` | 10 | 8 | 1162 |
| `now_exclall` | `--exclude-libs=ALL` | 1324 | 3430 | `0x133628` | `0x6f70` | `0x41b67` | `0x20af00` | `0x211970` | `0x148` | 10 | 8 | 1653 |
| `now_gc` | `--gc-sections` | 2176 | 4497 | `0x133794` | `0x6690` | `0x41b57` | `0x20fcf4` | `0x216970` | `0x148` | 10 | 8 | 1162 |
| `now_icf` | `--icf=all` | 1347 | 4496 | `0x12f99c` | `0x5770` | `0x41b67` | `0x20acf4` | `0x211970` | `0x148` | 10 | 8 | 1162 |
| `now_sortname` | `--sort-section=name` | 1342 | 4495 | `0x13381c` | `0x68b8` | `0x41b50` | `0x20fcf4` | `0x216970` | `0x148` | 10 | 8 | 1162 |
| `now_sortnone` | `--sort-section=none` | 4497 | 4497 | `0x1337c8` | `0x6f70` | `0x41b67` | `0x20fcf4` | `0x216970` | `0x148` | 10 | 8 | 1162 |
| `now_nomergeexidx` | `--no-merge-exidx-entries` | 2878 | 4497 | `0x1337c8` | `0x95c0` | `0x41b67` | `0x212cf4` | `0x219970` | `0x148` | 10 | 8 | 1162 |
| `now_mergeexidx` | `--merge-exidx-entries` | 4497 | 4497 | `0x1337c8` | `0x6f70` | `0x41b67` | `0x20fcf4` | `0x216970` | `0x148` | 10 | 8 | 1162 |
| `now_rosegment` | `--rosegment` | 1720 | 4497 | `0x1337c8` | `0x6f70` | `0x41b67` | `0x210cf4` | `0x217970` | `0x148` | 10 | 8 | 1162 |
| `now_noorder` | no section-ordering-file | 2723 | 4497 | `0x1337e8` | `0x6f48` | `0x41b67` | `0x20fcf4` | `0x216970` | `0x148` | 10 | 8 | 1162 |
| `now_notwhole` | zlib/libzip not `--whole-archive` | 1338 | 4413 | `0x12c208` | `0x6af0` | `0x3dc17` | `0x202dd8` | `0x209930` | `0x148` | 10 | 8 | 1137 |
| `now_fmod_after_gcc` | FMOD after `libgcc.a` | 1345 | 3796 | `0x135510` | `0x7098` | `0x41b67` | `0x211d1c` | `0x218970` | `0x148` | 10 | 8 | 1165 |
| `now_libunwind` | `+libunwind.a` after `libgcc.a` | 4497 | 4497 | `0x1337c8` | `0x6f70` | `0x41b67` | `0x20fcf4` | `0x216970` | `0x148` | 10 | 8 | 1162 |
| `now_libcxxstatic` | `+libc++_static.a` before `libc++abi.a` | 4497 | 4497 | `0x1337c8` | `0x6f70` | `0x41b67` | `0x20fcf4` | `0x216970` | `0x148` | 10 | 8 | 1162 |
| `now_libatomic` | `+libatomic.a` after `libgcc.a` | 4497 | 4497 | `0x1337c8` | `0x6f70` | `0x41b67` | `0x20fcf4` | `0x216970` | `0x148` | 10 | 8 | 1162 |
| `now_ndkfull` | full ndk-build link flag set | 4497 | 4497 | `0x1337c8` | `0x6f70` | `0x41b67` | `0x20fcf4` | `0x216970` | `0x148` | 10 | 8 | 1162 |
| `now_goldO0` | gold `-O0` | 4497 | 4497 | `0x1337c8` | `0x6f70` | `0x41b67` | `0x20fcf4` | `0x216970` | `0x148` | 10 | 8 | 1162 |
| `now_goldO1` | gold `-O1` | 4497 | 4497 | `0x1337c8` | `0x6f70` | `0x41b67` | `0x20fcf4` | `0x216970` | `0x148` | 10 | 8 | 1162 |
| `now_goldO2` | gold `-O2` | 1720 | 4497 | `0x1337c8` | `0x6f70` | `0x41b67` | `0x20fcf4` | `0x216970` | `0x148` | 10 | 8 | 1162 |
| `now_goldO3` | gold `-O3` | 1720 | 4497 | `0x1337c8` | `0x6f70` | `0x41b67` | `0x20fcf4` | `0x216970` | `0x148` | 10 | 8 | 1162 |
| `now_notextreorder` | `--no-text-reorder` | 4497 | 4497 | `0x1337c8` | `0x6f70` | `0x41b67` | `0x20fcf4` | `0x216970` | `0x148` | 10 | 8 | 1162 |
| `now_threads` | `--threads` | 4497 | 4497 | `0x1337c8` | `0x6f70` | `0x41b67` | `0x20fcf4` | `0x216970` | `0x148` | 10 | 8 | 1162 |
| `now_nothreads` | `--no-threads` | 4497 | 4497 | `0x1337c8` | `0x6f70` | `0x41b67` | `0x20fcf4` | `0x216970` | `0x148` | 10 | 8 | 1162 |
| `now_nokeepmemory` | `--no-keep-memory` | 4497 | 4497 | `0x1337c8` | `0x6f70` | `0x41b67` | `0x20fcf4` | `0x216970` | `0x148` | 10 | 8 | 1162 |
| `now_discardlocals` | `--discard-locals` | 4497 | 4497 | `0x1337c8` | `0x6f70` | `0x41b67` | `0x20fcf4` | `0x216970` | `0x148` | 10 | 8 | 1162 |
| `now_discardall` | `--discard-all` | 4497 | 4497 | `0x1337c8` | `0x6f70` | `0x41b67` | `0x20fcf4` | `0x216970` | `0x148` | 10 | 8 | 1162 |

`--emit-relocs` fails with a gold 1.12 internal error
(`set_no_output_symtab_entry`, `object.h:1809`); `--no-rosegment` is not accepted by
gold 1.12.

### 8.1 Link-flag conclusions

* **`-z now` is the only load-bearing link flag** among the tested set:
  4497 -> 2932 without it. It reproduces `DT_FLAGS=BIND_NOW`,
  `DT_FLAGS_1=NOW`, `.got` at `0x20fcf4`, `.data` at `0x214000`, `.dynamic` `0x148`
  at `0x20fba4`, and the RELRO-covered `.got`.
* `--build-id` must be SHA-1 (md5 1969, none 1750); `--hash-style=sysv` (gnu/both
  1750); `-z combreloc` (`DT_RELCOUNT=1162`; nocombreloc 4486);
  `--spare-dynamic-tags=5` (0/10 4486); `--no-as-needed` (as-needed 1750);
  `-z max-page-size=4096` (65536 2932); `--exclude-libs=libgcc.a` (ALL 1324);
  `--whole-archive` for zlib/libzip (without 1338).
* `-z relro`, `-z noexecstack`, `--no-undefined`, `--fix-cortex-a8`,
  `--warn-shared-textrel --fatal-warnings`, `-z common-page-size=4096`,
  `--sort-section=none`, `--merge-exidx-entries`, `--no-text-reorder`, `--threads`,
  `--no-threads`, `--no-keep-memory`, `--discard-locals`, `--discard-all` and gold
  `-O0`/`-O1` are all neutral (byte-identical `.so`).
* `--gc-sections` (2176), `--icf=all` (1347), `--sort-section=name` (1342),
  `--no-merge-exidx-entries` (2878), `--rosegment` (1720), gold `-O2`/`-O3` (1720)
  are all worse.
* The full ndk-build r18b link flag set is **byte-identical** to the tree's shorter
  gold line. The tree's line is a correct minimal spelling of the same link.

## 9. Link input order: not vanilla ndk-build `c++_static`

The original's dynamic symbol table pins the link order:

```sh
readelf --dyn-syms -W ../_work/bins/android_2.0.16_libgof2hdaa.so \
  | grep -E '_Unwind_|__gnu_Unwind_Find_exidx|memalign|ldivmod'
```

```text
259: 00000000 0 FUNC GLOBAL DEFAULT UND _Unwind_Resume
722: 00000000 0 FUNC GLOBAL DEFAULT UND __aeabi_uldivmod
1547: 00000000 0 FUNC GLOBAL DEFAULT UND memalign@LIBC (2)
5259: 00000000 0 FUNC GLOBAL DEFAULT UND _Unwind_VRS_Get
5261: 00000000 0 FUNC GLOBAL DEFAULT UND _Unwind_RaiseException
5264: 00000000 0 FUNC GLOBAL DEFAULT UND _Unwind_VRS_Set
5265: 00000000 0 FUNC GLOBAL DEFAULT UND _Unwind_GetLanguageSpecificData
5266: 00000000 0 FUNC GLOBAL DEFAULT UND _Unwind_DeleteException
5267: 00000000 0 FUNC GLOBAL DEFAULT UND __gnu_ldivmod_helper
5268: 00000000 0 FUNC GLOBAL DEFAULT UND _Unwind_GetRegionStart
```

`_Unwind_*` are `UND`, and `__gnu_Unwind_Find_exidx` is absent. That means the
original did **not** pull a `libunwind.a` member. Vanilla ndk-build puts the STL
archives before the shared libraries, so it would pull `libunwind.a` and define
`_Unwind_*` locally. The original's link order must therefore place FMOD before
`libc++abi.a` (so the FMOD shared libraries satisfy the `_Unwind_*` references first).

Controlled relink evidence:

| link order | byte | linked | `.text` | `_Unwind_*` |
|---|---:|---:|---:|---|
| canonical (FMOD before `libc++abi.a`) | **4497** | **4497** | `0x1337c8` | `UND` |
| FMOD after `libgcc.a` | 1345 | 3796 | `0x135510` | defined locally |
| `+libunwind.a` after `libgcc.a` | 4497 | 4497 | `0x1337c8` | `UND` (no member pulled) |
| `+libc++_static.a` before `libc++abi.a` | 4497 | 4497 | `0x1337c8` | `UND` (no member pulled) |
| `+libatomic.a` after `libgcc.a` | 4497 | 4497 | `0x1337c8` | `UND` (no member pulled) |

Adding `libc++_static.a`, `libunwind.a` or `libatomic.a` **after** the FMOD libs is
unobservable (no members are pulled), so their presence cannot be excluded from the
binary alone. What is excluded is the vanilla ndk-build order that puts the STL
archives before FMOD and therefore pulls libunwind.

The original's `DT_NEEDED` order is exactly
`liblog, libGLESv2, libGLESv1_CM, libEGL, libandroid, libfmodex, libfmodevent,
libm, libdl, libc`; our build matches it exactly.

## 10. `-s` (gold strip), not GNU `strip`

The original's `.shstrtab` is in gold's Stringpool order. Gold's `-s` reproduces it
byte-for-byte; a post-link GNU `arm-linux-androideabi-strip --strip-all` rewrites it
in section-header order:

```sh
cp cmake-build-match/libgof2hdaa.so /tmp/w3745/gnu_strip.so
.../arm-linux-androideabi-strip --strip-all /tmp/w3745/gnu_strip.so
```

```text
orig   279 bytes  .init_array .fini_array .ARM.exidx .text .got .comment ...
gold-s 279 bytes  identical to orig
gnustr 279 bytes  .shstrtab .note.android.ident .note.gnu.build-id .dynsym ...
```

The tree's `-s` is correct.

## 11. Section/dynamic fingerprint at the current baseline

Measured with a byte-by-byte section comparison (the script in §12):

| section | ours | original | delta | differing bytes |
|---|---:|---:|---:|---:|
| `.text` | `0x1337c8`@`0x69440` | `0x1337c8`@`0x69440` | 0 | 114,947 |
| `.ARM.exidx` | `0x6f70`@`0x19cc08` | `0x6f70`@`0x19cc08` | 0 | 1,752 |
| `.ARM.extab` | `0x2628c`@`0x1a3b78` | `0x26298`@`0x1a3b78` | **-0xc** | 114,260 |
| `.rodata` | `0x41b67`@`0x1c9e10` | `0x41b67`@`0x1c9e10` | 0 | **0** |
| `.data.rel.ro` | `0x2fe8`@`0x20cbb0` | `0x2fe8`@`0x20cbb0` | 0 | **0** |
| `.data` | `0x296c`@`0x214000` | `0x296c`@`0x214000` | 0 | **0** |
| `.bss` | `0x69ac`@`0x216970` | `0x69ac`@`0x216970` | 0 | (NOBITS) |
| `.dynsym` | `0x15ca0`@`0x1f0` | `0x15ca0`@`0x1f0` | 0 | 324 |
| `.dynstr` | `0x2e318`@`0x15e90` | `0x2e318`@`0x15e90` | 0 | 1,268 |
| `.hash` | `0x973c`@`0x441a8` | `0x973c`@`0x441a8` | 0 | 133 |
| `.gnu.version` | `0x2b94`@`0x4d8e4` | `0x2b94`@`0x4d8e4` | 0 | **0** |
| `.rel.dyn` | `0x7048`@`0x504d4` | `0x7048`@`0x504d4` | 0 | 74 |
| `.rel.plt` | `0x72d0`@`0x5751c` | `0x72d0`@`0x5751c` | 0 | 21 |
| `.plt` | `0xac4c`@`0x5e7ec` | `0xac4c`@`0x5e7ec` | 0 | **0** |
| `.got` | `0x430c`@`0x20fcf4` | `0x430c`@`0x20fcf4` | 0 | **0** |
| `.dynamic` | `0x148`@`0x20fba4` | `0x148`@`0x20fba4` | 0 | **0** |
| `.init_array` | `0xc`@`0x20fb98` | `0xc`@`0x20fb98` | 0 | **0** |
| `.fini_array` | `0x8`@`0x20cba0` | `0x8`@`0x20cba0` | 0 | **0** |
| `.shstrtab` | `0x117` | `0x117` | 0 | **0** |
| `.comment` | `0x12f` | `0x12f` | 0 | **0** |
| `.note.android.ident` | `0x98`@`0x134` | `0x98`@`0x134` | 0 | **0** |
| `.note.gnu.build-id` | `0x24`@`0x1cc` | `0x24`@`0x1cc` | 0 | 20 |
| `.ARM.attributes` | `0x38` | `0x38` | 0 | **0** |

Every ELF section is size- and base-exact except `.ARM.extab` (`-0xc`). The
`.dynsym`/`.dynstr`/`.hash` residue is a symbol-order issue, not a flag: 5,522 of
5,578 dynsym entries are at the same index, first differing at index 1995
(`_ZN7Globals14cItemListID_00E` vs `_ZN7Globals5soundE`). The remaining
`.text`/`.ARM.extab` bytes belong to the known mismatched function bodies. No tested
link flag improves them.

### 11.1 Residual section-header delta: `.rel.plt` `sh_flags`/`sh_info`

The `.rel.plt` **size and address** are exact (content differs by 21 bytes from the
symbol-order residue), but its section header is not:

```text
orig: [10] .rel.plt REL 0005751c 05751c 0072d0 08  AI  3  20  4
ours: [10] .rel.plt REL 0005751c 05751c 0072d0 08   A  3   0  4
```

The original sets `SHF_INFO_LINK` (`I`) and `sh_info = 20` (the `.got` section
index); our NDK r18b gold 1.12 (`GNU gold (GNU Binutils 2.27.0.20170315) 1.12`)
sets `sh_info = 0` and no `I` flag. This is not a compile/link flag: a minimal
`gold -shared -z now` link reproduces `sh_info = 0`, and none of the tested gold
flags (`-z now`, `-z relro`, `--sort-section=name`, `--rosegment`, `--icf=all`,
`--threads`, `--no-threads`, `--discard-all`, `--discard-locals`) changes
`sh_info` or the `I` flag. It is either a different gold 1.12
snapshot (the `.note.gnu.gold-version` only records the version string, not the
binutils snapshot) or a post-link step. It is invisible to `byte_exact` (which
compares `.text` and dynsym, not section-header `sh_info`). The earlier
`link_flags_research.md` statement that `.rel.plt`'s `sh_info` is 20 in both is not
reproducible with the current shared NDK r18b gold. This is a real remaining delta
for whole-file identity, independent of the function-body work.

## 12. Reproduction commands

```sh
# baseline
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
cmake --build cmake-build-match --target lint  -j8

# toolchain identity
readelf -hW  ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -AW  ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -nW  ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -p .comment ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -dW  ../_work/bins/android_2.0.16_libgof2hdaa.so

# ndk-build r18b defaults
NDK=.cache/ndk/android-ndk-r18b
$NDK/ndk-build NDK_PROJECT_PATH=/tmp/w3745/ndktest \
  APP_BUILD_SCRIPT=/tmp/w3745/ndktest/jni/Android.mk \
  NDK_APPLICATION_MK=/tmp/w3745/ndktest/jni/Application.mk V=1 -B

# compile A/B (scratch; never --update-baseline)
cmake -S . -B /tmp/w3745/build_<tag> -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/tmp/w3745/toolchain_extra.cmake \
  -DGOF2_MATCH_OPT=-Oz -DGOF2_EXTRA_FLAGS='<flags>'
cmake --build /tmp/w3745/build_<tag> --target link_so -j8
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/verify_<tag> \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/build_<tag>/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0

# link A/B: relink the canonical objects with extra gold args
/tmp/w3745/relink.sh /tmp/w3745/out.so -z now <extra gold args>
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/verify_link \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/out.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0

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

## 13. Proven vs hypothesis

**Proven by measurement (this note):**

* Toolchain: NDK r18b clang 7.0.2, gold 1.12, API 16, armeabi-v7a, Thumb-2, NEON,
  softfp, static libc++ (ELF header, `.comment`, `.note.android.ident`,
  `.note.gnu.gold-version`, `.ARM.attributes`, program headers, `DT_NEEDED`).
* `-Oz` uniquely best; `-g` codegen-relevant; `-fstack-protector-strong`, `-frtti`,
  exceptions, frame pointers, strict aliasing, strict overflow, unaligned access,
  vectorization, inlining, jump tables, threadsafe statics, zero-initialized-BSS on.
* `-fPIC`/`-fpic` are byte-identical; `-std=c++11`/`-std=gnu++11` are byte-identical;
  API 16/21 target triples are byte-identical; `-fshort-wchar` is byte-identical in
  the linked `.so`.
* `-ffunction-sections` is present and load-bearing (proper removal 4497 -> 1345;
  194 duplicate-address `.ARM.exidx` sentinels reproduced).
* `-funwind-tables` is present and load-bearing (proper removal 4497 -> 2878);
  the earlier "neutral" result was a flag-order artifact.
* `-fdata-sections` is absent: data layout is byte-identical without it and differs
  by 113,935 / 3,273 / 405 bytes with it, even without the ordering file.
* Link flags: `-z now` is load-bearing (4497 -> 2932); SHA-1 build-id,
  `--hash-style=sysv`, `-z combreloc`, `--spare-dynamic-tags=5`, `--no-as-needed`,
  `-z max-page-size=4096`, `--exclude-libs=libgcc.a`, zlib/libzip `--whole-archive`
  are required; the full ndk-build link flag set is byte-identical.
* Link order: FMOD before `libc++abi.a`; `_Unwind_*` stay `UND`,
  `__gnu_Unwind_Find_exidx` absent. FMOD after `libgcc.a` drops to 1345/3796.
* Gold `-s` reproduces the original `.shstrtab`; GNU `strip` does not.
* All ELF sections are size- and base-exact except `.ARM.extab` (`-0xc`); `.rodata`,
  `.data`, `.data.rel.ro`, `.plt`, `.got`, `.dynamic`, `.init_array`, `.fini_array`,
  `.shstrtab`, `.comment`, `.note.android.ident`, `.gnu.version` and
  `.ARM.attributes` are byte-identical.
* `.rel.plt` size/address are exact (21 content bytes differ from symbol order), but
  its section header differs:
  original `sh_flags=AI`, `sh_info=20` (`.got` index); ours `A`, `sh_info=0`.
  The current NDK r18b gold 1.12 produces `A`/`0` even in a minimal link, and no
  tested gold flag changes it.

**Hypothesis / not recoverable from the stripped `.so`:**

* The exact build-system file (`Android.mk`/`Application.mk` vs a custom Makefile)
  is not recoverable. The compile flags are ndk-build r18b's; the `-fdata-sections`
  omission and the link order are not vanilla ndk-build defaults, so the original
  either used a custom build script or overrode ndk-build's defaults.
* The exact target API (16 vs 21) is not distinguished by codegen; the API-16
  `.note.android.ident` is the evidence that pins it.
* `-fPIC` vs `-fpic`, `-std=c++11` vs `-std=gnu++11`, and
  `-fshort-wchar` vs `-fno-short-wchar` are not distinguished by the linked artifact.
* `libc++_static.a`/`libatomic.a`/`libunwind.a` may have been on the link line; no
  members are pulled, so their presence is unobservable. `libunwind.a` is excluded
  as the source of `_Unwind_*` by the `UND`/`__gnu_Unwind_Find_exidx` observables.
* The remaining byte deltas (`.text` 114,947; `.ARM.exidx` 1,752; `.ARM.extab`
  114,260 with size `-0xc`; `.dynsym` 324; `.dynstr` 1,268; `.hash` 133;
  `.rel.dyn` 74; `.rel.plt` 21; build-id 20) are source/TU/function-body and
  link-order residue, not missing build flags.

## 14. Scratch artifacts

The raw per-variant records, logs, wrappers and `relink.sh` are under
`/tmp/w3745/` (`compile/rec_*.json`, `link/matrix.json`, `toolchain_extra.cmake`,
`toolchain_extra2.cmake`, `toolchain_nounwind.cmake`, `toolchain_nofuncsections.cmake`,
`relink.sh`, `ndktest/`). They are not part of the repository.
