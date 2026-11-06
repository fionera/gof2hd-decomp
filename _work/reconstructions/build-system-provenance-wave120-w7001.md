# Build-system provenance, wave 120 (w7001)

Status: research note, 2026-11-06. Worktree `/home/debian/GalaxyOnFire2/wt/w7001`,
branch `wave120-w7001`, base `ef7fb9cde`. Scratch: `/dev/shm/w7001`.
**No `src/` or `tools/` change is landed.** This note is the deliverable, plus one
stale build-id byte-count correction in `docs/IDENTICALITY.md` (19 -> 20; the
build-id descriptor is 20 B, and the number of *differing* bytes is 19 or 20 depending on
whether one hash byte happens to coincide -- see §3.1).

Canonical gate measured before and after this pass (unchanged):

```text
verify: byte 4505  linked 4505  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
lint: CLEAN   drift: 0   RATCHET PASS
```

Original: `sha256 d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`,
2,187,328 B. Canonical build: `sha256 8d0fd2bbd55640a0a45a32d5d2d863760bd642ef49850a295166add2a1aa1e39`,
same size.

## 0. Bottom line

* The shipped `libgof2hdaa.so` is an **NDK r18b / clang 7.0.2 / gold 1.12** build:
  API 16, armeabi-v7a, Thumb-2, VFPv3, NEONv1, softfp, static libc++
  (`libc++abi.a` + `libandroid_support.a` + `libgcc.a`), `-Oz -g`, RTTI + exceptions,
  `-fstack-protector-strong`, `-ffunction-sections`, **no `-fdata-sections`**, default
  visibility.
* The **compile-flag set is exactly the stock r18b `android.toolchain.cmake` defaults plus
  three non-default settings**: `ANDROID_ARM_NEON=ON`,
  `ANDROID_CPP_FEATURES="rtti exceptions"`, `CMAKE_BUILD_TYPE=Release`. A full-tree build
  using the stock r18b CMake toolchain (same link recipe) reproduces the gate
  **4505/4505/4524**, `RATCHET PASS`, and differs from the canonical build in **exactly
  20 bytes, all inside `.note.gnu.build-id`**.
* The **link line is not the vanilla CMake `c++_static` standard-library line either**. The
  original places FMOD before the STL archives, so FMOD's exported `_Unwind_*` satisfy
  `libc++abi.a`'s references and `libgcc.a`'s `libunwind.o` is *not* pulled; it includes
  `libgcc.a` (pulling `_aeabi_ldivmod.o`/`_dvmd_lnx.o`, leaving `__gnu_ldivmod_helper`
  and `raise@LIBC` undefined), and omits `libc++_static.a`/`libunwind.a`. This is the
  repo's custom gold link. The vanilla CMake `c++_static` group gives a different binary
  (`.text` +32 B, `byte_exact` 2404).
* **Plain ndk-build defaults are refuted**: 1345/3846/4523, imports **189** (not 207),
  `_Unwind_*` defined locally, `DT_NEEDED` tail `libdl libc libm`. A heavily coerced
  ndk-build (`APP_STL=none`, `-fno-data-sections`, explicit `libc++abi`/`android_support`/
  `libgcc`, `--exclude-libs=libgcc.a`, custom `LOCAL_LDLIBS` order) reaches
  **2404/4504/4523** with 207 imports — close, but still not byte-exact.
* **Confidence: CMake (stock r18b toolchain + custom link) ~85%; ndk-build with many
  overrides ~10%; a hand-written custom link script ~5%.** The build-system *file* is not
  recoverable from the stripped ELF; the effective flags and link order are.

## 1. Primary evidence: artifact metadata

Commands:

```sh
O=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
N=/home/debian/GalaxyOnFire2/wt/w7001/.cache/ndk/android-ndk-r18b
RE=$N/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-readelf
$RE -hW $O; $RE -nW $O; $RE -p .comment $O; $RE -AW $O; $RE -dW $O; $RE -SW $O
```

| observable | value | conclusion |
|---|---|---|
| ELF header | `ELF32 DYN ARM EABI5`, `e_flags 0x5000200` | armeabi-v7a **softfp** |
| `.note.android.ident` | API **16**, `r18b`, build `5063045`; **byte-identical** (152/152) to `$N/platforms/android-16/arch-arm/usr/lib/crtbegin_so.o` | NDK **r18b**, API 16 |
| `.comment` | clang 7.0.2 (`4751641 based on r328903`) + `GCC: (GNU) 4.9.x 20150123` | clang app TUs + GCC 4.9 libcrypto |
| `.note.gnu.gold-version` | `gold 1.12` | NDK gold linker |
| `.ARM.attributes` | 56 B: ARM v7-A, Thumb-2, VFPv3, NEONv1, GOT-indirect, wchar_t 4, enum int, 8-byte align, unaligned v6 | `-march=armv7-a -mthumb -mfpu=neon -mfloat-abi=softfp -fPIC`; no `-fshort-enums` |
| `DT_NEEDED` | `liblog libGLESv2 libGLESv1_CM libEGL libandroid libfmodex libfmodevent libm libdl libc` | link-line order; FMOD before `m/dl/c` |
| `DT_FLAGS` / `DT_FLAGS_1` | `BIND_NOW` / `NOW` | `-z now` |
| `.hash` only, no `.gnu.hash` | SYSV hash | gold Android default |
| `.init_array` / `.fini_array` | 12 B (3 entries) / 8 B (2 entries) | 3 static initializers |
| defined / undefined dynsym | 5370 / 207 | symbol parity 5370/5370, 0 extra / 0 missing |

The `.note.android.ident` byte-identity is the decisive NDK-r18b fingerprint. It is emitted
by `crtbegin_so.o`, which both CMake and ndk-build link, so it proves the toolchain/API but
not the build driver.

### 1.1 APK-level evidence

```sh
APK="/home/debian/GalaxyOnFire2/Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk"
unzip -p "$APK" META-INF/MANIFEST.MF | head -3
unzip -p "$APK" assets/crashlytics-build.properties
unzip -l "$APK" | grep -iE "Android.mk|Application.mk|CMakeLists|\.cmake|\.gradle|\.ninja"
```

| observable | value |
|---|---|
| `META-INF/MANIFEST.MF` | `Created-By: Android Gradle 3.1.2` |
| Crashlytics build timestamp | `Mon Oct 22 10:18:17 CEST 2018` |
| `BuildConfig.BUILD_TYPE` / `DEBUG` / `FLAVOR` | `release` / `false` / `""` |
| manifest `minSdkVersion` / `targetSdkVersion` | **16** / 27 |
| native libs | `libcrashlytics.so`, `libfmodevent.so`, `libfmodex.so`, `libgof2hdaa.so` |
| build-system leftovers | **none** (no `Android.mk`, `Application.mk`, `CMakeLists.txt`, `*.cmake`, `*.gradle`, `*.ninja`) |

AGP 3.1.2 supports both `externalNativeBuild.cmake` and `externalNativeBuild.ndkBuild`,
so the APK metadata alone does not choose between them; it only proves the build was
Gradle-driven.

The APK's `lib/armeabi-v7a/libgof2hdaa.so` is
`sha256 6d38fd767ee3b4f8174c371bce371bfc6397cf76e13c1756938b2a94658e3b58`, the same size
and the same build-id as the ground truth; it differs in **288 bytes**, all in `.shstrtab`
(259) and the section-header `sh_name` fields (29). The ground truth is the APK lib after
`tools/reorder_shstrtab.py` (bfd section-header order -> gold Stringpool order), not a
different build. This is a packaging/post-link detail, not a build-system discriminator.
Fresh confirmation: copying the APK lib and running
`tools/reorder_shstrtab.py <ground-truth> reordered-apk.so` (the ground truth supplies the
gold Stringpool order) yields
`sha256 d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483` and
`cmp -l` reports **0** differing bytes -- the APK lib is exactly the ground-truth build
before the post-link shstrtab rewrite.

## 2. Fresh minimal probes: CMake vs ndk-build command lines

Two minimal `probe.cpp` projects (same source: virtual/RTTI, static init, exception,
stack buffer, NEON loop, `std::string`) were built in `/dev/shm/w7001` with the r18b NDK.

### 2.1 Stock r18b CMake toolchain

```sh
N=/home/debian/GalaxyOnFire2/wt/w7001/.cache/ndk/android-ndk-r18b
cmake -S /dev/shm/w7001/cmakeprobe -B /dev/shm/w7001/cmakeprobe/build -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=$N/build/cmake/android.toolchain.cmake \
  -DANDROID_ABI=armeabi-v7a -DANDROID_PLATFORM=android-16 -DANDROID_STL=c++_static \
  -DANDROID_ARM_NEON=ON '-DANDROID_CPP_FEATURES=rtti exceptions' \
  -DCMAKE_BUILD_TYPE=Release
ninja -C /dev/shm/w7001/cmakeprobe/build -v
```

Compile command (codegen-relevant tokens):

```text
clang++ --target=armv7-none-linux-androideabi16 --gcc-toolchain=... --sysroot=...
  -g -DANDROID -ffunction-sections -funwind-tables -fstack-protector-strong
  -no-canonical-prefixes -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16
  -mthumb -mfpu=neon -Wa,--noexecstack -Wformat -Werror=format-security
  -std=c++11 -frtti -fexceptions -Oz -DNDEBUG -fPIC
```

Link command (library order):

```text
clang++ ... -Wl,--exclude-libs,libgcc.a -Wl,--exclude-libs,libatomic.a -nostdlib++
  --sysroot=.../platforms/android-16/arch-arm -Wl,--build-id -Wl,--warn-shared-textrel
  -Wl,--fatal-warnings -Wl,--fix-cortex-a8 -Wl,--exclude-libs,libunwind.a
  -L.../libs/armeabi-v7a -Wl,--no-undefined -Wl,-z,noexecstack -Qunused-arguments
  -Wl,-z,relro -Wl,-z,now -shared -Wl,-soname,libprobe.so -o libprobe.so probe.o
  -latomic -lm libc++_static.a libc++abi.a libandroid_support.a libunwind.a -ldl
```

Measured probe `DT_NEEDED` tail: **`libm.so libdl.so libc.so`** — the same tail order as
the original.

### 2.2 Stock r18b ndk-build

```sh
$N/ndk-build NDK_PROJECT_PATH=/dev/shm/w7001/ndkprobe \
  APP_BUILD_SCRIPT=/dev/shm/w7001/ndkprobe/jni/Android.mk \
  NDK_APPLICATION_MK=/dev/shm/w7001/ndkprobe/jni/Application.mk V=1 -j4
```

`Application.mk`: `APP_ABI := armeabi-v7a`, `APP_PLATFORM := android-16`,
`APP_STL := c++_static`, `APP_OPTIM := release`.
`Android.mk`: `LOCAL_CPP_FEATURES := rtti exceptions`, `LOCAL_ARM_NEON := true`.

Compile command (codegen-relevant tokens):

```text
clang++ -target armv7-none-linux-androideabi16 -fdata-sections -ffunction-sections
  -fstack-protector-strong -funwind-tables -no-canonical-prefixes -g
  -Wno-invalid-command-line-argument -Wno-unused-command-line-argument
  -fno-exceptions -fno-rtti -gcc-toolchain ... -fpic -march=armv7-a
  -mfloat-abi=softfp -mfpu=vfpv3-d16 -mthumb -Oz -DNDEBUG -mfpu=neon
  -I.../llvm-libc++/include -I.../llvm-libc++abi/include -I.../android/support/include
  -std=c++11 -DANDROID -Wa,--noexecstack -Wformat -Werror=format-security
  -frtti -fexceptions --sysroot ... -isystem .../sysroot/usr/include/arm-linux-androideabi
```

Link command (library order):

```text
clang++ -Wl,-soname,libprobe.so -shared --sysroot=.../platforms/android-16/arch-arm
  probe.o libc++_static.a libc++abi.a libandroid_support.a libunwind.a
  -lgcc -Wl,--exclude-libs,libgcc.a -latomic -Wl,--exclude-libs,libatomic.a
  -target armv7-none-linux-androideabi16 -no-canonical-prefixes -gcc-toolchain ...
  -Wl,--fix-cortex-a8 -Wl,--exclude-libs,libunwind.a -Wl,--build-id -nostdlib++
  -Wl,--no-undefined -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now
  -Wl,--warn-shared-textrel -Wl,--fatal-warnings
  -L.../platforms/android-16/arch-arm/usr/lib -ldl -lc -lm -o libprobe.so
```

Measured probe `DT_NEEDED` tail: **`libdl.so libc.so libm.so`** — the reverse of the
original's `libm libdl libc`. The ndk-build link also places the STL static archives and
`libgcc` *before* the shared libraries; CMake places the user/shared libraries first.

### 2.3 Token-by-token comparison

| token | original (derived) | CMake r18b default | ndk-build r18b default | codegen-visible? |
|---|---|---|---|---|
| `-target` / API | `armv7-none-linux-androideabi16` | same | same | yes (API 16) |
| `-march/-mthumb/-mfpu/-mfloat-abi` | armv7-a / thumb / neon / softfp | same (`vfpv3-d16` then `neon`) | same (`vfpv3-d16` then `neon`) | yes |
| `-Oz` | yes | yes (Release) | yes (release) | yes |
| `-g` | yes | yes | yes | yes (schedule at -Oz) |
| `-fPIC` / `-fpic` | either (byte-identical) | `-fPIC` | `-fpic` | no |
| `-std=c++11` / `gnu++11` | either (byte-identical) | `-std=c++11` | `-std=c++11` | no |
| `-frtti -fexceptions` | yes | explicit via `ANDROID_CPP_FEATURES` | explicit via `LOCAL_CPP_FEATURES` (after `-fno-*`) | yes |
| `-fstack-protector-strong` | yes | yes | yes | yes |
| `-ffunction-sections` | yes | yes | yes | yes |
| `-funwind-tables` | yes | yes | yes | yes |
| **`-fdata-sections`** | **no** | **no** | **yes** | **yes (data layout)** |
| `-DANDROID` / `-DNDEBUG` | yes (r18b Release) | yes | yes | no |
| `-D__ANDROID_API__=16` | yes (implicit via target) | implicit | implicit | no |
| STL include dirs | libc++/libc++abi/android_support | same | same | no |
| link: user libs before STL | yes (FMOD before `libc++abi`) | **yes** | **no** (static before shared) | **yes** |
| link: `-lm` before `-ldl -lc` | yes | **yes** | **no** (`-ldl -lc -lm`) | **yes** |
| link: `libunwind.a` pulled | **no** (`_Unwind_*` UND) | yes on the line, not pulled (FMOD satisfies) | **yes pulled** (`_Unwind_*` local) | **yes** |
| link: `libgcc.a` pulled | **yes** (`__gnu_ldivmod_helper`, `raise`) | no (vanilla standard group) | yes (`-lgcc`) | **yes** |

The repo's canonical `build-canonical/compile_commands.json` line (e.g. `StarMap.cpp`) is
the same codegen token set as the CMake probe: `-target armv7-none-linux-androideabi16
-march=armv7-a -mthumb -mfpu=neon -mfloat-abi=softfp -std=c++11 -fPIC -frtti
-fstack-protector-strong -Oz -g -stdlib=libc++ ... -D__ANDROID_API__=16 -DANDROID
-DNDEBUG -ffunction-sections -funwind-tables`, plus the per-file
`-mllvm -likely-branch-weight=7 -mllvm -unlikely-branch-weight=1` override on
`StarMap.cpp`/`SpaceLounge.cpp`. The differences from the stock CMake probe line are
spelling (`--target=` vs `-target`), include-path order, the explicit
`-D__ANDROID_API__=16`, and the two per-file `-mllvm` options; §4 shows the resulting
objects are byte-identical.

## 3. Full-tree builds and gate counts

All builds use the same 192 app TUs (`tools/source_order.txt`), the vendored
zlib/libzip/libcrypto archives, and the APK's FMOD shared libraries.

### 3.1 Canonical (repo `cmake/toolchain.cmake` + repo gold link) — reference

```sh
cmake -S . -B /dev/shm/w7001/build-canonical -G Ninja \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build /dev/shm/w7001/build-canonical --target link_so -j8
cmake --build /dev/shm/w7001/build-canonical --target check
```

```text
verify: byte 4505  linked 4505  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
lint: CLEAN   drift: 0   RATCHET PASS
```

Section diff vs the original (fresh `secdiff.py`): `.text` 1,259,464 B / 88,216 differing
bytes; `.ARM.exidx` 28,528 / 863; `.ARM.extab` 156,312 / 109,364 (size exact); `.rodata`
269,351 / 192 (the appended mission rank table); `.rel.plt` 29,392 / **6** (three adjacent
`JUMP_SLOT` entries permuted); `.note.gnu.build-id` 36 / **20**. Every other section is
byte-identical, including `.dynsym`, `.dynstr`, `.hash`, `.got`, `.data`, `.data.rel.ro`,
`.init_array`, `.fini_array`, `.comment`, `.note.android.ident`, `.note.gnu.gold-version`,
`.ARM.attributes`, `.shstrtab`. File size exact.

The `.note.gnu.build-id` descriptor is exactly 20 B, so at most 20 bytes can differ. The
current canonical and stock-CMake builds each differ from the original in all 20. Wave 111
measured 19 on its own build, where one of the 20 hash bytes happened to match the
original -- that is a hash coincidence, not a different note layout. The doc correction
above therefore records the current build's count, not a universal constant.

### 3.2 Stock r18b CMake toolchain (only the 3 non-default settings) — reproduces the gate

A wrapper toolchain resolves the repo's `GOF2_NDK_*` paths, then includes the **stock**
`$N/build/cmake/android.toolchain.cmake` with only:

```cmake
set(ANDROID_ABI armeabi-v7a)          # r18b default anyway
set(ANDROID_PLATFORM android-16)      # r18b minimum/default
set(ANDROID_STL c++_static)           # r18b default
set(ANDROID_ARM_NEON ON)              # NON-DEFAULT (API < 23 -> FALSE)
set(ANDROID_CPP_FEATURES "rtti exceptions")  # NON-DEFAULT (empty)
set(CMAKE_BUILD_TYPE Release)         # NON-DEFAULT (empty)
```

The repo `CMakeLists.txt` link recipe is unchanged. Result:

```text
verify: byte 4505  linked 4505  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`cmp -l build-canonical/libgof2hdaa.so build-stockcmake2/libgof2hdaa.so` = **20 differing
bytes, all at file offsets 477..496 (the `.note.gnu.build-id` descriptor)**. Every code,
data, EH and dynamic-table section is byte-identical. This proves the canonical hand-written
compile flags are codegen-equivalent to the stock r18b CMake toolchain defaults + the three
settings.

### 3.3 Plain ndk-build defaults — refuted

`/dev/shm/w7001/ndkfull`: `APP_ABI := armeabi-v7a`, `APP_PLATFORM := android-16`,
`APP_STL := c++_static`, `APP_OPTIM := release`, `LOCAL_CPP_FEATURES := rtti exceptions`,
`LOCAL_ARM_NEON := true`, `LOCAL_CFLAGS := -Wno-int-to-pointer-cast
-Wno-int-to-void-pointer-cast`, `LOCAL_CPPFLAGS := -std=c++11`, FMOD in `LOCAL_LDLIBS`,
zlib/libzip whole-static, crypto static. `LOCAL_SRC_FILES` is
`tools/source_order.txt` verbatim (192 paths); the prebuilt zlib/libzip modules use
`PREBUILT_STATIC_LIBRARY` and are linked with `-Wl,--whole-archive`.

```sh
$N/ndk-build NDK_PROJECT_PATH=/dev/shm/w7001/ndkfull \
  APP_BUILD_SCRIPT=/dev/shm/w7001/ndkfull/jni/Android.mk \
  NDK_APPLICATION_MK=/dev/shm/w7001/ndkfull/jni/Application.mk V=1 -j8
```

```text
verify: byte 1345  linked 3846  size 4523/4524  avg 99.74
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 189
  FAIL  byte_exact 4505 -> 1345
  FAIL  linked_exact 4505 -> 3846 (beyond the +-1 band)
  FAIL  NEW undefined imports: ['__gnu_Unwind_Find_exidx', 'dladdr', 'fflush']
RATCHET FAIL
```

Hard differences: `.dynsym` 88,960 vs 89,248 (-18 symbols), `.dynstr` 188,870 vs 189,208,
`.text` 1,269,632 vs 1,259,464 (+10,168), `.ARM.exidx` 28,944 vs 28,528, `.rodata` 270,167
vs 269,159 (+1,008), `DT_NEEDED` tail `libdl libc libm`, and `_Unwind_*` are **defined
locally** (not in dynsym) because ndk-build's static-before-shared order pulls
`libgcc.a`'s `libunwind.o` before FMOD is seen.

### 3.4 Coerced ndk-build — close, not byte-exact

`/dev/shm/w7001/ndkcoerced` adds the non-default overrides: `APP_STL := none`,
`-fno-data-sections`, explicit `libc++abi.a`/`libandroid_support.a`/`libgcc.a` after FMOD
in `LOCAL_LDLIBS`, `-lm` before `-ldl`, `-nostdlib++`, `TARGET_LIBGCC :=` /
`TARGET_LIBATOMIC :=` emptied, `-Wl,--exclude-libs=libgcc.a`.

```text
verify: byte 2404  linked 4504  size 4523/4524  avg 99.93
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
  WARN  linked_exact 4505 -> 4504 (transient band)
  FAIL  byte_exact 4505 -> 2404
RATCHET FAIL
```

The coerced build has the correct `DT_NEEDED` tail (`libm libdl libc`), the correct 207
imports (`_Unwind_*` UND, `__gnu_ldivmod_helper` UND), symbol parity 5370/5370, and the
same file size (2,187,328 B). Its `.text` is **+32 B** (1,259,496) and `.rodata` is the
same +192 mission table. The only named size mismatch is
`StarMap::drawOnScreenInfo` 3488 -> 3500 (+12, the repo's per-file
`-mllvm -likely-branch-weight=7` override is not applied); the remaining ~20 B is extra
local code, most likely from the clang-driver `-lgcc` group pulling a different libgcc
member set than the canonical gold link. So ndk-build *can* be coerced to the right
imports/NEEDED/size, but not to the exact `.text` layout without further work.

### 3.5 `-fdata-sections` A/B (canonical toolchain + repo link)

```text
verify: byte 4314  linked 4505  size 4524/4524  avg 99.94
```

`.rodata` 268,999 (-160), `.data` 10,588 (-16), `.data.rel.ro` 12,264 (365 differing
bytes), file size 2,187,312 (-16). This independently confirms the original has **no
`-fdata-sections`** (CMake default; ndk-build adds it).

## 4. Object-level compile-flag identity

Compiled the same 192 TUs with the canonical CMake command and with the ndk-build command
(`-fno-data-sections`), then compared every `.text.*`, `.rodata*`, `.data*`, `.bss*`,
`.ARM.exidx*`, `.ARM.extab*` section:

```sh
python3 /dev/shm/w7001/cmp_objs2.py \
  /dev/shm/w7001/build-canonical/CMakeFiles/gof2.dir \
  /dev/shm/w7001/ndkfull_nodata/obj/local/armeabi-v7a/objs/gof2hdaa
```

Result: **192/192 TUs compared; 42 diffs, all in the two TUs that carry the repo's
per-file `-mllvm -likely-branch-weight=7 -mllvm -unlikely-branch-weight=1` override**
(`StarMap.cpp`, `SpaceLounge.cpp`). Recompiling those two with the ndk-build flags **plus**
the same `-mllvm` overrides gives **0 diffs** against the canonical objects. Therefore:

* The ndk-build compile line with `-fno-data-sections` is **byte-identical** to the
  canonical CMake compile line for all 192 TUs.
* `-std=c++11` vs `-std=gnu++11` is byte-identical (checked on `Generator.cpp`,
  `Array.cpp`, `BumpShader.cpp`).
* `-fPIC` vs `-fpic` is byte-identical (same three TUs).
* `-fdata-sections` changes only `.rodata`/`.data`/`.bss`/`.data.rel.ro` sectioning;
  every `.text.*` section is byte-identical (0 diffs across 192 TUs).

## 5. Link-order evidence (the strongest CMake-vs-ndk-build discriminator)

1. **`DT_NEEDED` tail.** Original `libm libdl libc`; CMake probe `libm libdl libc`;
   ndk-build probe `libdl libc libm`. CMake's standard-library order is `-latomic -lm
   ... -ldl` (libc implicit last); ndk-build appends `TARGET_LDLIBS := -lc -lm` after
   `LOCAL_LDLIBS`, so `libc` precedes `libm`.
2. **`_Unwind_*` are UND in the original and canonical, local in ndk-build.** FMOD
   (`libfmodex.so`) exports `_Unwind_Resume`, `_Unwind_RaiseException`, `_Unwind_VRS_Get/
   Set`, `_Unwind_GetLanguageSpecificData`, `_Unwind_DeleteException`,
   `_Unwind_GetRegionStart`. CMake places FMOD before `libc++abi.a`/`libgcc.a`, so those
   references bind to FMOD's shared definitions and `libgcc.a`'s `libunwind.o` is not
   pulled. ndk-build places `libgcc.a` (and `libunwind.a`) before FMOD, so `libunwind.o`
   is pulled and `_Unwind_*` become local (hidden by `--exclude-libs`). The original's
   207-import set includes the seven `_Unwind_*`; the ndk-build default's 189-import set
   does not.
3. **`libgcc.a` is pulled, but `libunwind.a` is not.** The original imports
   `__gnu_ldivmod_helper` and `raise@LIBC` (UND) and does not import `__aeabi_ldivmod`.
   That is the signature of `libgcc.a`'s `_aeabi_ldivmod.o` + `_dvmd_lnx.o` being pulled
   (and hidden), while `libunwind.o` is not. A relink of the canonical objects with the
   vanilla CMake `c++_static` standard group (`libc++_static.a libc++abi.a
   libandroid_support.a libunwind.a`, no `libgcc.a`) instead leaves `__aeabi_ldivmod` UND
   and drops `__gnu_ldivmod_helper`/`raise` — not the original.
4. **No `libc++_static.a` code.** The original has no `std::__ndk1` libc++ symbols. The
   only `_ZSt*`/`_ZNSt*` symbols it defines are libc++abi's (`_ZSt9terminatev`,
   `_ZTVN10__cxxabiv1...`, `std::bad_exception`, `std::type_info`, etc.). Therefore
   `libc++_static.a` is not pulled.
5. **The canonical link line** (exact, from `ninja -t commands`):

```text
ld.gold -shared -soname libgof2hdaa.so --build-id -z now --exclude-libs=libgcc.a
  -o libgof2hdaa.unstripped.so
  crtbegin_so.o -L .../platforms/android-16/arch-arm/usr/lib
  <192 app objects in tools/source_order.txt order>
  --whole-archive libz_gof2.a libzip_gof2.a --no-whole-archive
  -l log -l GLESv2 -l GLESv1_CM -l EGL -l android
  libfmodex.so libfmodevent.so
  libcrypto_gof2.a
  -l m
  libc++abi.a
  libandroid_support.a
  libgcc.a                      # armv7-a/thumb multilib
  mission_case_table.o
  -l dl -l c
  crtend_so.o
```

This is a **custom/trimmed** link line: it matches CMake's *ordering* (user/shared libs
before the C++ runtime; `-lm` before `-ldl -lc`) but not CMake's *vanilla standard-library
group* (no `libc++_static.a`, no `libunwind.a`, and explicit `libgcc.a`).

## 6. Minimum non-default settings

### 6.1 CMake (stock r18b `android.toolchain.cmake`)

Only **three** non-default settings are needed for the compile flags:

| setting | r18b default | original needs | non-default? |
|---|---|---|---|
| `ANDROID_ABI` | `armeabi-v7a` | `armeabi-v7a` | no |
| `ANDROID_PLATFORM` | `android-16` (minimum) | `android-16` | no |
| `ANDROID_STL` | `c++_static` | `c++_static` | no |
| **`ANDROID_ARM_NEON`** | `FALSE` for API < 23 | `ON` | **yes** |
| **`ANDROID_CPP_FEATURES`** | empty | `rtti exceptions` | **yes** |
| **`CMAKE_BUILD_TYPE`** | empty | `Release` | **yes** |
| `-fdata-sections` | absent | absent | no |
| link order | user libs before standard libs | FMOD before STL archives | no (CMake default) |
| standard libs | `libc++_static.a libc++abi.a libandroid_support.a libunwind.a` | `libc++abi.a libandroid_support.a libgcc.a` | **yes** (custom link) |

### 6.2 ndk-build

| setting | r18b default | original needs | non-default? |
|---|---|---|---|
| `APP_ABI` | all ABIs | `armeabi-v7a` | **yes** |
| `APP_PLATFORM` | `android-16` (minimum) | `android-16` | no |
| `APP_STL` | `system` | `c++_static` (or `none` + explicit) | **yes** |
| `APP_OPTIM` | `release` | `release` | no |
| `LOCAL_ARM_NEON` | false | true | **yes** |
| `LOCAL_CPP_FEATURES` | none (`-fno-exceptions -fno-rtti`) | `rtti exceptions` | **yes** |
| `-fdata-sections` | present | absent | **yes** (`-fno-data-sections`) |
| link order | static libs before shared libs | FMOD before STL archives | **yes** (custom `LOCAL_LDLIBS`) |
| libgcc | full `libgcc.a` before shared libs | mini `libgcc.a` or reorder | **yes** |

Even with all of these, the coerced ndk-build build is 2404/4504/4523, not 4505/4505/4524.

## 7. Proven / disproven / unobservable

### Proven (direct artifact or controlled A/B)

* NDK **r18b**, clang **7.0.2**, gold **1.12**, API **16**, armeabi-v7a, Thumb-2, VFPv3,
  NEONv1, softfp, static libc++ (`libc++abi.a` + `libandroid_support.a` + `libgcc.a`),
  `-Oz -g`, RTTI + exceptions, `-fstack-protector-strong`, `-ffunction-sections`,
  **no `-fdata-sections`**, default visibility.
* `.note.android.ident` is byte-identical to r18b's API-16 `crtbegin_so.o` note.
* The compile-flag set is exactly the stock r18b CMake toolchain defaults +
  `ANDROID_ARM_NEON=ON`, `ANDROID_CPP_FEATURES="rtti exceptions"`,
  `CMAKE_BUILD_TYPE=Release`: full-tree stock-CMake build = **4505/4505/4524**,
  `RATCHET PASS`, 20-byte build-id-only difference from canonical.
* `-fdata-sections` is absent: adding it gives **4314/4505/4524** and changes
  `.rodata`/`.data`/`.data.rel.ro`.
* The link line places FMOD before the STL archives: `_Unwind_*` stay UND (resolved from
  FMOD), `libgcc.a`'s `libunwind.o` is not pulled.
* The link line includes `libgcc.a` (pulls `_aeabi_ldivmod.o`/`_dvmd_lnx.o`; leaves
  `__gnu_ldivmod_helper`/`raise@LIBC` UND) and omits `libc++_static.a`/`libunwind.a`.
* `DT_NEEDED` tail is `libm libdl libc` (CMake order), not `libdl libc libm`
  (ndk-build order).
* The APK is an AGP 3.1.2 release build (2018-10-22), with no surviving build files.
* The APK lib and ground truth are the same build; the 288-byte difference is only
  `.shstrtab`/`sh_name` (gold Stringpool order vs bfd section-header order). Re-running
  `tools/reorder_shstrtab.py` on the APK lib reproduces the ground-truth sha256 exactly
  (`d4c17be6...`, `cmp -l` 0).

### Disproven

* **Plain ndk-build defaults reproduce the original**: no — 1345/3846/4523, imports 189,
  `_Unwind_*` local, `DT_NEEDED` tail wrong, `RATCHET FAIL`.
* **ndk-build's default compile line reproduces the original's data layout**: no —
  `-fdata-sections` costs 191 byte_exact (4505 -> 4314).
* **The vanilla CMake `c++_static` standard-library line reproduces the original**: no —
  a full build with that group gives 2404/4504 (`.text` +32 B), and relinking the
  canonical objects with it leaves `__aeabi_ldivmod` UND instead of
  `__gnu_ldivmod_helper`/`raise`.
* **`-std=gnu++11` or `-fpic` are discriminators**: no — both are byte-identical on this
  source.

### Unobservable (no surviving evidence)

* The exact build-system file/driver: `CMakeLists.txt` vs `Android.mk`/`Application.mk`
  vs a hand-written link script. The ELF proves the effective flags and link order, not
  the file that produced them.
* The exact CMake version and generator (Ninja vs Make); CMake version is codegen-neutral.
* The host OS (the libc++abi path suggests the darwin NDK package, but the build host is
  not directly observable).
* The exact link-line text (only the effective order is recoverable from `DT_NEEDED`,
  imports, and section layout).
* The native link timestamp (the Crashlytics `2018-10-22 10:18:17 CEST` is the closest
  witness).
* Whether `-DANDROID`/`-DNDEBUG` were explicit or implied (codegen-neutral here).

## 8. Confidence estimate

| hypothesis | confidence | why |
|---|---:|---|
| **CMake + stock r18b `android.toolchain.cmake` defaults + 3 settings, with a custom/trimmed gold link** | **~85%** | Both CMake discriminators (no `-fdata-sections`, user libs before STL archives) match the defaults; the stock toolchain reproduces the gate byte-for-byte except build-id; the custom link is the only non-default part. |
| ndk-build with `APP_STL=none`, `-fno-data-sections`, explicit STL libs, mini libgcc, custom link order | ~10% | Possible (coerced build reaches 207 imports / exact size) but requires 6+ overrides and still misses the exact `.text`. |
| Hand-written custom link script | ~5% | The effective link line is custom, but the compile line is exactly a CMake toolchain default set. |

The build-system *file* is genuinely unobservable from the stripped ELF. What is proven is
the **effective build recipe**: NDK r18b CMake-default compile flags + the repo's custom
gold link order.

## 9. Exact commands (reproduction)

```sh
# baseline
cd /home/debian/GalaxyOnFire2/wt/w7001
cmake -S . -B /dev/shm/w7001/build-canonical -G Ninja \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build /dev/shm/w7001/build-canonical --target link_so -j8
cmake --build /dev/shm/w7001/build-canonical --target check

# APK lib == ground truth after the post-link shstrtab rewrite
unzip -o "/home/debian/GalaxyOnFire2/Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk" \
  lib/armeabi-v7a/libgof2hdaa.so -d /dev/shm/w7001/apklib
cp /dev/shm/w7001/apklib/lib/armeabi-v7a/libgof2hdaa.so /dev/shm/w7001/reordered-apk.so
python3 tools/reorder_shstrtab.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  /dev/shm/w7001/reordered-apk.so
sha256sum /dev/shm/w7001/reordered-apk.so   # d4c17be6... (== ground truth)

# stock r18b CMake toolchain (wrapper: DownloadNDK.cmake + stock android.toolchain.cmake
# + the 3 settings + project include dirs)
cmake -S . -B /dev/shm/w7001/build-stockcmake2 -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/dev/shm/w7001/toolchain-stock-r18b.cmake \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build /dev/shm/w7001/build-stockcmake2 --target link_so -j8
cmake --build /dev/shm/w7001/build-stockcmake2 --target check
cmp -l /dev/shm/w7001/build-canonical/libgof2hdaa.so \
       /dev/shm/w7001/build-stockcmake2/libgof2hdaa.so | wc -l   # 20

# plain ndk-build full tree
N=/home/debian/GalaxyOnFire2/wt/w7001/.cache/ndk/android-ndk-r18b
$N/ndk-build NDK_PROJECT_PATH=/dev/shm/w7001/ndkfull \
  APP_BUILD_SCRIPT=/dev/shm/w7001/ndkfull/jni/Android.mk \
  NDK_APPLICATION_MK=/dev/shm/w7001/ndkfull/jni/Application.mk V=1 -j8
python3 tools/verify/verify.py --build-dir /dev/shm/w7001/verify-ndkfull \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /dev/shm/w7001/ndkfull/libs/armeabi-v7a/libgof2hdaa.so \
  --objdump $N/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0

# -fdata-sections A/B
cmake -S . -B /dev/shm/w7001/build-fdata -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/dev/shm/w7001/toolchain-fdata.cmake \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build /dev/shm/w7001/build-fdata --target link_so -j8
python3 tools/verify/verify.py --build-dir /dev/shm/w7001/verify-fdata \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /dev/shm/w7001/build-fdata/libgof2hdaa.so \
  --objdump $N/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0

# object-level compile-flag identity
python3 /dev/shm/w7001/cmp_objs2.py \
  /dev/shm/w7001/build-canonical/CMakeFiles/gof2.dir \
  /dev/shm/w7001/ndkfull_nodata/obj/local/armeabi-v7a/objs/gof2hdaa

# probes
cmake -S /dev/shm/w7001/cmakeprobe -B /dev/shm/w7001/cmakeprobe/build -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=$N/build/cmake/android.toolchain.cmake \
  -DANDROID_ABI=armeabi-v7a -DANDROID_PLATFORM=android-16 -DANDROID_STL=c++_static \
  -DANDROID_ARM_NEON=ON '-DANDROID_CPP_FEATURES=rtti exceptions' \
  -DCMAKE_BUILD_TYPE=Release
ninja -C /dev/shm/w7001/cmakeprobe/build -v
$N/ndk-build NDK_PROJECT_PATH=/dev/shm/w7001/ndkprobe \
  APP_BUILD_SCRIPT=/dev/shm/w7001/ndkprobe/jni/Android.mk \
  NDK_APPLICATION_MK=/dev/shm/w7001/ndkprobe/jni/Application.mk V=1 -j4
```

Scratch helpers: `/dev/shm/w7001/cmp_objs2.py`, `/dev/shm/w7001/secdiff.py`,
`/dev/shm/w7001/toolchain-stock-r18b.cmake`, `/dev/shm/w7001/toolchain-fdata.cmake`,
`/dev/shm/w7001/cmakeprobe/`, `/dev/shm/w7001/ndkprobe/`,
`/dev/shm/w7001/ndkfull/`, `/dev/shm/w7001/ndkfull_nodata/`,
`/dev/shm/w7001/ndkcoerced/`, `/dev/shm/w7001/stocklink/`.

## 10. Gate state / what changed

Only this note and the one stale byte count in `docs/IDENTICALITY.md` are landed. No
`src/`, `tools/`, baseline or allowlist change. The canonical gate is unchanged:
`byte 4505 / linked 4505 / size 4524 / data 843 / imports 207 / lint CLEAN / drift 0 /
RATCHET PASS`.
