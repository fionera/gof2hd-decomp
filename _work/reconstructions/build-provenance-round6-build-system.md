# Build provenance round 6: the original build system, per-TU flags, and the
# per-file link order

Status: **research note only**, 2026-11-10. Worktree `wt/w11119`, branch
`wave166-w11119`, base master `85c1f88df`. Scratch `/dev/shm/w11119`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(2,187,328 B, sha256 `d4c17be6…bed9483`, build-id `c011810d…0d310`).

Gate on entry and exit (fresh `cmake --build cmake-build-match --target check -j2`):
**RATCHET PASS**, `byte_exact 4507/4524`, `linked_exact 4507/4524`, size-exact
4524/4524, data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
`.text` **83,147** differing bytes, `.ARM.extab` **659**, `.rel.plt` **0**
(byte-identical), `.note.gnu.build-id` 20. All 27 section sizes exact; file size
2,187,328 B.

This note is an independent round-6 re-derivation. It confirms the round-5
toolchain result and adds three new measurements:

1. a **per-TU `-g` sweep** that proves every TU was compiled with full DWARF
   (`-g2`/`-g3`), not `-g1`/`-gline-tables-only`/`-gmlt`;
2. a **per-TU `-O` sweep** that proves the uniform `-Oz` level;
3. a **per-TU call-count / frame-pointer / RTTI fingerprint** that shows every
   TU has the same codegen flags, plus the **full 192-TU link order** and the
   **host = macOS** evidence.

No `src/`, `CMakeLists.txt`, baseline, allowlist, or doc change is landed. This
note and `build_provenance_round6.py` are the only commits.

---

## 0. Bottom line

1. **Build system: the r18b CMake toolchain is the best fit, not default
   ndk-build.** The decisive observable remains **no `-fdata-sections`**: the
   r18b CMake toolchain does not add it, stock r18b ndk-build does. The
   original's `.rodata` (269,159 B), `.data` (10,604 B), `.data.rel.ro`
   (12,264 B), and `.bss` (27,052 B) are byte-identical to the no-
   `-fdata-sections` build. The link-line tail `libm, libdl, libc` and the
   user-library-before-STL order are also the CMake defaults; stock ndk-build
   gives `libdl, libc, libm` and puts the STL archives before the shared
   libraries. CMake confidence ~85%; ndk-build with explicit
   `-fno-data-sections` + custom `LOCAL_LDLIBS` order ~10%; a hand-written
   makefile copying the CMake defaults ~5%.
2. **Host: macOS.** The only path-like string in the shipped `.so` is
   `/Volumes/Android/buildbot/src/android/ndk-release-r18/external/libcxx/../../external/libcxxabi/src/abort_message.cpp`,
   which is the **darwin** NDK r18b `libc++abi.a` member, not the linux
   package's `/buildbot/...` string. The link therefore used the darwin NDK
   package. This is consistent with the Xcode/iOS/mac source dumps and with
   `Created-By: Android Gradle 3.1.2`.
3. **Every TU used the same compile flags.** All 4,507 byte-exact exported
   functions and every local function are identical; the 83,147 `.text`
   differing bytes are exactly the 17 residual exported functions (15 unique
   bodies; the C1/C2 aliases share an address) in 13 TUs.
   There is no evidence of a per-TU `-O`, `-fno-*`, `-std=`, `-D`, or
   `-include` deviation.
4. **`-g` is full DWARF (`-g2` or `-g3`), not limited.** A new per-TU sweep
   compiles all 192 TUs with `-g0`, `-g1`, `-g2`, `-g3`,
   `-gline-tables-only`, and `-gmlt`. `-g2` and `-g3` reproduce the canonical
   `.text` for **all 192 TUs**; the limited forms change **48 TUs**. Two
   independent size witnesses: `VirtualInput.cpp::keyPressed` is **708 B** in
   the original and under `-g2`/`-g3`, but **704 B** under `-g1`/`-g0`;
   `Level.cpp::almostKillWanted` is **252 B** in the original and under
   `-g2`/`-g3`, but **248 B** under `-g1`/`-g0`. `-g2` and `-g3` are
   byte-identical in `.text`; the shipped `.so` is stripped, so the two cannot
   be distinguished.
5. **`-Oz` is the uniform optimization level.** A new per-TU sweep compiles all
   192 TUs at `-O1`, `-O2`, `-Os`, and `-O3`. Relative to the canonical `-Oz`
   object, `-O1` changes **190/192** TUs, `-O2` **182/192**, `-Os`
   **174/192**, and `-O3` **184/192**. The remaining TUs are level-insensitive
   (their non-`-Oz` output is byte-identical), so `-Oz` is consistent with all
   192. Three independent size witnesses: `Mesh::MeshIntersect` is 680 B in
   the original and `-Oz`, 684 B at `-Os`; `VirtualInput::simulateTouch` is
   1928 B and `-Oz`, 2076 B at `-Os`; `CameraIsSphereinViewFrustum` is 436 B
   and `-Oz`, 460 B at `-Os`.
6. **`-DNDEBUG` is on.** The original has exactly one `__assert2` call, in
   libc++abi. The app assert strings listed by the MeeGo/mac dumps
   (`meshResourceIds.Length == objCount`, `lodLevels > 0`,
   `ParticleSettingsRef::assertInit == 42`, `duration[i] != -1`,
   `setIndex < particleSets.Length`, …) are **absent** from the Android
   `.so`. `-DDEBUG` is therefore not defined (or its asserts are compiled
   out).
7. **Per-file compile order is not encoded; the object/link order is.** The
   original `.text` is the concatenation of the 192 TUs in
   `tools/source_order.txt` order. The first strong symbol of each TU is
   monotonically increasing in that order (0 violations); the first TU is
   `src/platform/android/BackButton.c`, the last code-bearing TU is
   `src/engine/render/ParticleSystemMesh.cpp`, followed by the weak-only
   `Array.cpp`/`ArrayInstantiations.cpp`. A parallel build's actual compile
   order is nondeterministic and cannot be recovered; the link order can, and
   is reproduced exactly.

---

## 1. Evidence sources

| source | what it witnesses |
|---|---|
| `_work/bins/android_2.0.16_libgof2hdaa.so` | `.comment`, `.note.android.ident`, `.note.gnu.gold-version`, `.ARM.attributes`, `.dynsym`, `.text`, `.rodata`, `.data`, `.bss`, `.ARM.exidx`, `.ARM.extab`, build-id |
| `Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk` | packaging tool, build date, versionName/Code, package, minSdk/targetSdk, ABI set, APK `.so` copy |
| `source file list dump/` | original source layout, TU membership, per-TU symbol lists, MeeGo/mac assert strings |
| `_work/original_layout/` | parsed TU mapping and original layout notes |
| `.cache/ndk/android-ndk-r18b/` | exact r18b package, CMake toolchain, ndk-build defaults, crtbegin, libc++abi |
| `cmake-build-match/` | canonical match build for per-TU A/B |
| `/dev/shm/w11119/ndkprobe`, `/dev/shm/w11119/cmakeprobe` | fresh stock r18b ndk-build and CMake probe compile/link lines |

`raw`/`aligned`/`size` are the official verifier's counts. `diff bytes` is the
sum of differing bytes over the differing sections (current baseline
**83,826** = `.text` 83,147 + `.ARM.extab` 659 + `.note.gnu.build-id` 20).

---

## 2. Toolchain identity (confirmed, not re-derived)

The four non-code fingerprint sections are byte-identical to the canonical
build:

| section | size | SHA-256 |
|---|---:|---|
| `.comment` | 303 | `d4f577f46aa5f1329f9898849122325b00c135130f431950d9600f5cb73c7fee` |
| `.note.android.ident` | 152 | `522bf4171215935bd062d824e7c00f3cbc0b5312942192199f3747e6cc11115b` |
| `.note.gnu.gold-version` | 28 | `47f758e9dbe910453f5b562efab608e6b2289e178fa76940332cfcf1ca1fd40e` |
| `.ARM.attributes` | 56 | `b03afea6aa56c9ca191d5385abb498e64c7ecdf8e249a80e68704050256b1bc2` |

Exact strings:

```text
.comment:
  Android (4751641 based on r328903) clang version 7.0.2
  (https://android.googlesource.com/toolchain/clang 003100370607242ddd5815e4a043907ea9004281)
  (https://android.googlesource.com/toolchain/llvm 1d739ffb0366421d383e04ff80ec2ee591315116)
  (based on LLVM 7.0.2svn)
  GCC: (GNU) 4.9.x 20150123 (prerelease)

.note.android.ident: namesz=8 descsz=0x84 type=1 "Android\0"
  api=0x10 (16), ndk="r18b", build="5063045"

.note.gnu.gold-version: namesz=4 descsz=9 type=4 "GNU\0" "gold 1.12"
```

`.ARM.attributes` decoded: ARM v7-A, Thumb-2, VFPv3, NEONv1, GOT-indirect,
wchar_t 4, enum int, 8-byte alignment, unaligned v6, no `Tag_ABI_VFP_args`
(softfp). This is the uniform `-march=armv7-a -mthumb -mfpu=neon
-mfloat-abi=softfp -fPIC` signature. Because the linker merges per-object
`.ARM.attributes`, a single 56-byte attribute set also proves that **no TU was
compiled with a different ARM/FPU/ABI flag** (`-marm`, `-mfpu=vfpv3-d16`,
`-mfloat-abi=hard`, `-march=armv7ve`, etc. would merge different tags).

The `.comment` is exactly the r18b `crtbegin_so.o` clang string plus the
`libcrypto_gof2.a(sha256.o)` GCC string; there is no second compiler identity.
The app TUs are all clang 7.0.2. The GCC string is from the vendored OpenSSL
archive, not from an app TU.

---

## 3. Build system: CMake vs ndk-build

### 3.1 APK metadata

```text
META-INF/MANIFEST.MF:
  Manifest-Version: 1.0
  Built-By: Generated-by-ADT
  Created-By: Android Gradle 3.1.2

assets/crashlytics-build.properties:
  #Mon Oct 22 10:18:17 CEST 2018
  version_name=2.0.16
  package_name=net.fishlabs.gof2hdallandroid2012
  build_id=f527605b-309f-404d-8afc-b68027edc49e
  version_code=47947032
  app_name=GOF2

AndroidManifest.xml:
  minSdkVersion = 16
  targetSdkVersion = 27
  package = net.fishlabs.gof2hdallandroid2012
```

The APK ships only `lib/armeabi-v7a/` (`libgof2hdaa.so`, `libcrashlytics.so`,
`libfmodex.so`, `libfmodevent.so`). AGP 3.1.2 supports both
`externalNativeBuild.cmake` and `externalNativeBuild.ndkBuild`, so the APK
metadata proves the packaging tool and the 2018-10-22 era, not the native
build driver. The native `.so` could also have been built separately and
checked in as a prebuilt.

### 3.2 Stock r18b compile lines (freshly measured, 2026-11-10)

A tiny `probe.cpp` (virtual/RTTI, static init, exception, stack buffer, NEON
loop, `std::string`) built with stock **ndk-build**
(`APP_ABI := armeabi-v7a`, `APP_PLATFORM := android-16`,
`APP_STL := c++_static`, `APP_OPTIM := release`,
`LOCAL_CPP_FEATURES := rtti exceptions`, `LOCAL_ARM_NEON := true`) compiles
with:

```text
clang++ -target armv7-none-linux-androideabi16
  -fdata-sections -ffunction-sections -fstack-protector-strong -funwind-tables
  -no-canonical-prefixes -g
  -Wno-invalid-command-line-argument -Wno-unused-command-line-argument
  -fno-exceptions -fno-rtti
  -gcc-toolchain <NDK>/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64
  -fpic -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16
  -mthumb -Oz -DNDEBUG
  -I<libc++>/include -I<libc++abi>/include -I<android_support>/include
  -std=c++11 -fexceptions -frtti -DANDROID
  -Wa,--noexecstack -Wformat -Werror=format-security
  -fexceptions -frtti -frtti -fexceptions
  --sysroot <NDK>/sysroot -isystem <sysroot>/usr/include/arm-linux-androideabi
```

The same probe built with the stock **r18b CMake toolchain**
(`ANDROID_ABI=armeabi-v7a`, `ANDROID_PLATFORM=android-16`,
`ANDROID_STL=c++_static`, `ANDROID_ARM_NEON=ON`,
`ANDROID_CPP_FEATURES="rtti exceptions"`, `CMAKE_BUILD_TYPE=Release`)
compiles with:

```text
clang++ --target=armv7-none-linux-androideabi16
  --gcc-toolchain=<NDK>/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64
  --sysroot=<NDK>/sysroot
  -g -DANDROID -ffunction-sections -funwind-tables -fstack-protector-strong
  -no-canonical-prefixes
  -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16 -mthumb -mfpu=neon
  -Wa,--noexecstack -Wformat -Werror=format-security
  -std=c++11 -frtti -fexceptions -Oz -DNDEBUG -fPIC
```

The codegen-affecting differences are:

| observable | ndk-build default | CMake toolchain | original |
|---|---|---|---|
| `-fdata-sections` | **yes** | no | **no** (`.rodata` 269,159 / `.data` 10,604 / `.bss` `0x216970`) |
| `-fPIC` vs `-fpic` | `-fpic` | `-fPIC` | unobservable (`-fpic` byte-identical) |
| `-mfpu` | `vfpv3-d16` (+ per-source `neon`) | `neon` | `neon` (VFPv3+NEONv1) |
| `-frtti -fexceptions` | needs `APP_CPPFLAGS`/`LOCAL_CPP_FEATURES` | `ANDROID_CPP_FEATURES` | on |
| `-std=c++11` | from `APP_STL := c++_static` | from `ANDROID_STL=c++_static` | on |
| `-DANDROID` position | late | early | unobservable |
| `-Wno-invalid-command-line-argument` / `-Wno-unused-command-line-argument` | yes | no | unobservable |
| `-gcc-toolchain` vs `--gcc-toolchain=` | `-gcc-toolchain` | `--gcc-toolchain=` | unobservable |

### 3.3 The decisive `-fdata-sections` A/B

| build | `.rodata` | `.data` | `.data.rel.ro` | `.bss` addr / size | `byte_exact` |
|---|---:|---:|---:|---|---:|
| original | 269,159 (`0x41b67`) | 10,604 (`0x296c`) | 12,264 | `0x216970` / 27,052 (`0x69ac`) | — |
| canonical (no `-fdata-sections`) | 269,159 | 10,604 | 12,264 | `0x216970` / 27,052 | **4507** |
| `-fdata-sections` | 268,807 (`0x41a07`) | 10,588 (`0x295c`) | — | `0x216960` / 27,052 | **4315** |

The original matches the no-`-fdata-sections` layout **exactly**. Default r18b
ndk-build always adds `-fdata-sections`; the r18b CMake toolchain does not.
That is the single strongest build-system discriminator. The CMake line also
matches every other observable without an explicit override, whereas
ndk-build would need `APP_CFLAGS := -fno-data-sections` (and
`LOCAL_ARM_NEON := true`, and `APP_CPPFLAGS := -fexceptions -frtti`).

### 3.4 Link-line discriminators

Fresh probe `DT_NEEDED` tails:

```text
CMake:     liblog.so, libm.so, libdl.so, libc.so
ndk-build: liblog.so, libdl.so, libc.so, libm.so
original:  liblog, libGLESv2, libGLESv1_CM, libEGL, libandroid,
           libfmodex, libfmodevent, libm, libdl, libc
```

The original's tail is `libm, libdl, libc`, matching CMake. The CMake link
also puts the user/shared libraries before the STL archives; ndk-build puts
`libc++_static.a`, `libc++abi.a`, `libandroid_support.a`, `libunwind.a`
before the shared libraries. The original omits `libc++_static.a` and
`libunwind.a` entirely (FMOD supplies `_Unwind_*`; `libgcc.a` supplies
`__aeabi_ldivmod`), which is the repo's custom gold link, not the vanilla
CMake `c++_static` group. The build *driver* is CMake-shaped; the link line is
hand-curated.

### 3.5 Host = macOS

The only path-like string in the shipped `.so` is:

```text
/Volumes/Android/buildbot/src/android/ndk-release-r18/external/libcxx/../../external/libcxxabi/src/abort_message.cpp
```

That is the **darwin** NDK r18b `libc++abi.a` member. The linux r18b package's
`libc++abi.a` carries `/buildbot/...` instead. The link therefore used the
darwin package, i.e. a macOS build host. This is consistent with the Xcode/iOS
and mac source dumps and with AGP 3.1.2 on macOS. It does not by itself select
CMake vs ndk-build, but it rules out a pure Linux build.

### 3.6 Verdict

**Most likely:** NDK r18b **CMake toolchain** with
`ANDROID_ABI=armeabi-v7a`, `ANDROID_PLATFORM=android-16`,
`ANDROID_STL=c++_static`, `ANDROID_ARM_NEON=ON`,
`ANDROID_CPP_FEATURES="rtti exceptions"`, `CMAKE_BUILD_TYPE=Release`, plus a
custom `target_link_libraries` list that links only `libc++abi.a` (plus
`libandroid_support.a`, `libgcc.a`) and the FMOD prebuilts in the witnessed
order.

**Alternatives, in order of likelihood:**

1. ndk-build with `APP_CFLAGS := -fno-data-sections`, `LOCAL_ARM_NEON := true`,
   `APP_CPPFLAGS := -fexceptions -frtti`, and a custom `LOCAL_LDLIBS` order
   that puts `-lm` before `-ldl -lc` and the FMOD prebuilts before the STL
   archives.
2. A hand-written makefile that copies the CMake defaults (no
   `-fdata-sections`, `-fPIC`, `-mfpu=neon`, `-std=c++11`, `-frtti
   -fexceptions`, `-Oz -DNDEBUG -g`) and the custom gold link.
3. A Gradle `externalNativeBuild` block (AGP 3.1.2 supports both CMake and
   ndk-build); the APK's `Created-By: Android Gradle 3.1.2` proves packaging,
   not the native driver.

---

## 4. Per-file flags

### 4.1 Uniform flags proven by the artifact

| flag | value | witness |
|---|---|---|
| NDK | r18b 18.1.5063045 | `.note.android.ident` `r18b`/`5063045` |
| compiler | clang 7.0.2 (4751641/r328903) | `.comment` |
| target API | 16 | `.note.android.ident` `0x10`; `platforms/android-16` crtbegin |
| `-target` | `armv7-none-linux-androideabi16` | API 16 + ARMv7 |
| `-march` | `armv7-a` | `.ARM.attributes` v7/Application |
| `-mthumb` | yes | `Tag_THUMB_ISA_use: Thumb-2` |
| `-mfpu` | `neon` | `Tag_FP_arch: VFPv3`, `Tag_Advanced_SIMD_arch: NEONv1` |
| `-mfloat-abi` | `softfp` | no `Tag_ABI_VFP_args`, `Tag_ABI_HardFP_use: Deprecated` |
| `-std` | **C++11** | signed `new[]` guard; `gnu++11` byte-identical; C++14/17 change codegen |
| `-Oz` | yes | per-TU sweep below; `-O1/-O2/-Os/-O3` change 174–190 TUs |
| `-g` | **full DWARF (`-g2`/`-g3`)** | per-TU sweep below; 48 TUs change under `-g1` |
| `-fPIC` | yes | `Tag_ABI_PCS_GOT_use: GOT-indirect`; no `R_ARM_REL32`/`DT_TEXTREL` |
| `-frtti` | yes | 176 `_ZTI`, 176 `_ZTS`, 99 `_ZTV`; identical symbol set |
| `-fexceptions` | yes | 540 `_Unwind_Resume`, 3 `__cxa_throw`; identical call counts |
| `-fstack-protector-strong` | yes | 787 `__stack_chk_fail` calls; identical per-TU |
| `-funwind-tables` | yes | 3,566 `.ARM.exidx` entries; `.ARM.extab` 156,312 B |
| `-ffunction-sections` | yes | `.text` is input order; 272 CANTUNWIND exidx sentinels |
| `-fno-data-sections` | yes (absence) | `.rodata`/`.data`/`.bss` layout |
| `-fvisibility=default` | yes | 5,370 defined exports |
| `-fcommon` | yes (default) | duplicate `g_android_back_button_pressed` link error on our tree |
| `-fno-threadsafe-statics` | no (on) | `__cxa_guard_acquire/release` imports |
| `-fno-use-cxa-atexit` | no (on) | `__cxa_atexit` import |
| `-DANDROID` | yes | both build systems; codegen-neutral |
| `-DNDEBUG` | yes | one `__assert2` (libc++abi); app assert strings absent |
| `-D__ANDROID_API__=16` | yes (driver triple) | API 16 note; macro itself unobservable |

### 4.2 New per-TU `-g` sweep: full DWARF, not limited

Method: for every one of the 192 TUs, compile the canonical command with
`-g` replaced by `-g0`, `-g1`, `-g2`, `-g3`, `-gline-tables-only`, or `-gmlt`;
compare the concatenated `.text.*` section bytes (the exact codegen, excluding
debug sections) with the canonical `-g2` object.

| variant | TUs whose `.text.*` equals canonical | TUs that change |
|---|---:|---:|
| `-g0` | 144 | **48** |
| `-g1` | 144 | **48** |
| `-g2` | **192** | 0 |
| `-g3` | **192** | 0 |
| `-gline-tables-only` | 144 | **48** |
| `-gmlt` | 144 | **48** |

Because the original's `.text` is byte-identical to the canonical build for
every function except the 17 residual exported functions (15 unique bodies), and the 48 `-g`-sensitive TUs
include non-residual TUs, the original cannot be `-g0`/`-g1`/`-gline-tables-
only`/`-gmlt`. It must be `-g2` or `-g3`. The two are byte-identical in
`.text`; the shipped `.so` is stripped, so they cannot be distinguished.

Two independent size witnesses:

| function | TU | original size | `-g2`/`-g3` | `-g1`/`-g0` |
|---|---|---:|---:|---:|
| `_Z10keyPressedPN11AbyssEngine6EngineEi` | `src/engine/input/VirtualInput.cpp` | **708** | 708 | 704 |
| `_ZN5Level16almostKillWantedEi` | `src/game/world/Level.cpp` | **252** | 252 | 248 |

The mechanism is not frame-pointer elimination: `-g0 -fno-omit-frame-pointer`
still gives 704/248, so the 4-byte difference is a debug-info-driven
literal-pool/alignment effect. `-g2` is genuinely load-bearing at `-Oz`.

### 4.3 New per-TU `-O` sweep: uniform `-Oz`

Method: for every one of the 192 TUs, compile the canonical command with `-Oz`
replaced by `-O1`, `-O2`, `-Os`, or `-O3`; compare the concatenated `.text.*`
section bytes with the canonical `-Oz` object.

| variant | TUs whose `.text.*` equals canonical `-Oz` | TUs that change |
|---|---:|---:|
| `-O1` | 2 | **190** |
| `-O2` | 10 | **182** |
| `-Os` | 18 | **174** |
| `-O3` | 8 | **184** |

The remaining TUs are level-insensitive: their non-`-Oz` output is
byte-identical to `-Oz`, so `-Oz` is consistent with all 192. There is no TU
that requires a different level. Three independent size witnesses in
non-residual TUs:

| function | original size | `-Oz` | `-Os` |
|---|---:|---:|---:|
| `_ZN11AbyssEngine13MeshIntersectEffPNS_4MeshE` | **680** | 680 | 684 |
| `_Z13simulateTouchPN11AbyssEngine6EngineE` | **1928** | 1928 | 2076 |
| `_ZN11AbyssEngine27CameraIsSphereinViewFrustumERKNS_6AEMath6VectorEfPNS0_6MatrixEPNS_6CameraE` | **436** | 436 | 460 |

### 4.4 C++ standard

`-std=c++11` is proven by the signed `new T[n]` guard: the original emits
`orr rX, n, n, asr #31` before `_Znwj`, which clang 7 emits for C++98/C++11,
not for its default `gnu++14`. `-std=gnu++11` is byte-identical to
`-std=c++11` on the C++ TUs. `-std=c++14`/`c++17`/`gnu++14` change codegen and
drop the gate to 1512 in the round-5 whole-build A/B. `-std=c++98` fails to
compile (forward-declared scoped enum). So the standard is C++11 (the
`c++11` vs `gnu++11` spelling is unobservable).

### 4.5 `-DNDEBUG` and the absent app asserts

The original has exactly **one** `__assert2` call, in libc++abi. The MeeGo
1.2.7 and mac fhd dumps list the app's assert expressions and file paths:

```text
meshResourceIds.Length == objCount
lodLevels > 0
lodLevel < lodLevels
objIndex < objCount
ParticleSettingsRef::assertInit == 42
duration[i] != -1
setIndex < particleSets.Length
...
```

None of these strings is present in the Android `.so` (checked with
`strings -a`). The app asserts are compiled out. That is the expected effect
of `-DNDEBUG` (or an undefined `DEBUG`). The original's only `__assert2` call
is the libc++abi `abort_message` path, which is independent of `NDEBUG`.

### 4.6 Per-TU codegen fingerprints: no per-file flag deviation

For every exported function, the original and the canonical build have
identical call-site counts to:

```text
__stack_chk_fail, _Unwind_Resume, __cxa_throw, __cxa_guard_acquire,
__cxa_atexit, _ZdlPv, _ZdaPv, __assert2
```

Aggregated by TU, **0 of 188 TUs differ** in any of these counts. The same
holds for the frame-pointer fingerprint:

```text
mov r7, sp        220 occurrences (all functions)
add r7, sp, #N   2473 occurrences
push {..., r7}   2706 occurrences
```

0 TUs differ. The RTTI/typeinfo symbol set is identical (`_ZTI` 176,
`_ZTS` 176, `_ZTV` 99; 5,574 object/function symbols, 0 set difference). The
`.ARM.attributes` is a single uniform 56-byte set. Therefore:

* no TU was compiled with `-fno-stack-protector`, `-fno-exceptions`,
  `-fno-rtti`, `-fno-unwind-tables`, `-fno-threadsafe-statics`, or
  `-fno-use-cxa-atexit`;
* no TU was compiled with a different `-march`/`-mfpu`/`-mfloat-abi`/`-mthumb`;
* no TU has a different frame-pointer policy.

The 83,147 `.text` differing bytes are exactly the 17 residual exported functions
(15 unique bodies; the C1/C2 aliases share an address) in 13 TUs; every local
function is byte-identical. A per-TU flag difference
would have to be codegen-neutral on 4,507 of 4,524 exported functions and on
every local function.

### 4.7 What is not observable

The following cannot be distinguished from the shipped stripped `.so`:

* `-g2` vs `-g3` (identical `.text`; debug sections stripped);
* `-fPIC` vs `-fpic` (byte-identical on this source);
* `-std=c++11` vs `-std=gnu++11` (byte-identical on this source);
* `-DANDROID` position, `-D__ANDROID_API__=16` spelling, include-path order;
* `-fno-sized-deallocation`, `-fno-aligned-new`, `-fno-plt`,
  `-fno-inline-functions-called-once`, `-funsigned-char`,
  `-fno-merge-constants`, `-ffp-contract=off`, `-fno-trapping-math`,
  `-fno-delete-null-pointer-checks`, `-fno-asynchronous-unwind-tables`,
  `-mno-movt`, `-fno-omit-frame-pointer` (already the default), `-UNDEBUG`,
  `-UANDROID`, `-D__ANDROID_API__=21` — all codegen-neutral on this tree;
* `-include <header>`: no evidence; any forced include would have to be
  codegen-neutral on all 4,507 exact functions;
* per-file `-D` macros that are not used by the source;
* the exact CMake version (>= 3.6.0; AGP 3.1.2's bundled CMake is
  3.6.4111459) and the exact Gradle version;
* the actual parallel compile order (nondeterministic); only the link order is
  encoded.

---

## 5. Per-file compile order

### 5.1 The link order is `tools/source_order.txt`

The original `.text` is the concatenation of the 192 TUs in
`tools/source_order.txt` order, with each TU's functions in source order. The
first strong symbol of each TU is monotonically increasing in that order:
**0 violations** across the 190 TUs that emit at least one strong symbol. The
last two TUs, `src/engine/core/Array.cpp` and
`src/engine/core/ArrayInstantiations.cpp`, emit only weak template
instantiations; their bodies are placed in earlier TUs by the linker's COMDAT
selection, so they have no strong-symbol address of their own.

The first TUs are:

```text
0  src/platform/android/BackButton.c
1  src/engine/core/NFC.cpp
2  src/engine/input/VirtualInput.cpp
3  src/engine/file/BlowfishSubkeyFile.c
4  src/platform/android/main.c
5  src/platform/android/GOF2HD2012.c
6  src/engine/render/Mesh.cpp
7  src/engine/render/GLES1Compat.cpp
8  src/engine/file/FileInterfaceAndroid.cpp
9  src/engine/math/EaseInOutMatrix.cpp
```

The last code-bearing TUs are:

```text
185 src/engine/render/IParticleSystem.cpp
186 src/engine/render/ParticleSystemSprite.cpp
187 src/engine/render/ParticleSystemManager.cpp
188 src/engine/render/ParticleSettings.cpp
189 src/engine/render/ParticleSystemMesh.cpp
190 src/engine/core/Array.cpp              (weak only)
191 src/engine/core/ArrayInstantiations.cpp (weak only)
```

The full 192-row table is in §5.2. The order is not alphabetical and is not
the iOS dump order: it is the original Android source-list order, recovered
from the original `.dynsym`/`.text` placement and frozen in
`tools/source_order.txt`.

### 5.2 Full TU table (original link order)

Columns: `#` = link index; `strong funcs` = functions with a strong (`T`/`t`)
definition in that TU; `strong bytes` = sum of their sizes; `exported` =
functions compared by the verifier; `exact` = byte-exact exported functions;
`residual` = non-byte-exact exported functions; `first strong addr` = lowest
linked address of a strong symbol from that TU.

| # | TU (repo path) | strong funcs | strong bytes | exported | exact | residual functions | first strong addr |
|---:|---|---:|---:|---:|---:|---|---:|
| 0 | `src/platform/android/BackButton.c` | 1 | 16 | 1 | 1 |  | 0x69579 |
| 1 | `src/engine/core/NFC.cpp` | 25 | 2060 | 28 | 28 |  | 0x69589 |
| 2 | `src/engine/input/VirtualInput.cpp` | 34 | 5184 | 34 | 34 |  | 0x69e5d |
| 3 | `src/engine/file/BlowfishSubkeyFile.c` | 5 | 688 | 5 | 5 |  | 0x6b2a1 |
| 4 | `src/platform/android/main.c` | 40 | 1156 | 40 | 40 |  | 0x6b551 |
| 5 | `src/platform/android/GOF2HD2012.c` | 11 | 184 | 11 | 11 |  | 0x6b9d9 |
| 6 | `src/engine/render/Mesh.cpp` | 14 | 8488 | 14 | 14 |  | 0x6ba91 |
| 7 | `src/engine/render/GLES1Compat.cpp` | 46 | 814 | 46 | 46 |  | 0x6dac1 |
| 8 | `src/engine/file/FileInterfaceAndroid.cpp` | 34 | 2882 | 38 | 38 |  | 0x6ddf1 |
| 9 | `src/engine/math/EaseInOutMatrix.cpp` | 15 | 1784 | 17 | 17 |  | 0x6e821 |
| 10 | `src/engine/render/Image.cpp` | 7 | 4438 | 7 | 7 |  | 0x6ee95 |
| 11 | `src/engine/file/AEFile.cpp` | 73 | 4730 | 81 | 81 |  | 0x70011 |
| 12 | `src/engine/render/Camera.cpp` | 4 | 1140 | 4 | 4 |  | 0x7130d |
| 13 | `src/engine/math/EaseInOut.cpp` | 14 | 642 | 14 | 14 |  | 0x71781 |
| 14 | `src/engine/core/AERandom.cpp` | 11 | 444 | 11 | 11 |  | 0x719e1 |
| 15 | `src/engine/render/Image2D.cpp` | 1 | 28 | 1 | 1 |  | 0x71b65 |
| 16 | `src/engine/render/ImageFont.cpp` | 11 | 1890 | 11 | 11 |  | 0x71b81 |
| 17 | `src/game/core/String.cpp` | 58 | 5700 | 61 | 61 |  | 0x722e9 |
| 18 | `src/engine/math/Vector.cpp` | 36 | 2036 | 36 | 36 |  | 0x7395d |
| 19 | `src/engine/core/IApplicationModule.cpp` | 1 | 8 | 1 | 1 |  | 0x74155 |
| 20 | `src/engine/render/Curve.cpp` | 3 | 540 | 3 | 3 |  | 0x7415d |
| 21 | `src/game/core/CheatCode.cpp` | 12 | 822 | 19 | 19 |  | 0x74379 |
| 22 | `src/engine/math/Transform.cpp` | 28 | 12902 | 41 | 41 |  | 0x746a1 |
| 23 | `src/engine/render/PaintCanvas.cpp` | 167 | 27946 | 192 | 192 |  | 0x77675 |
| 24 | `src/engine/core/GameTextiPhone.cpp` | 6 | 2088 | 6 | 6 |  | 0x7efcd |
| 25 | `src/engine/render/FBOContainer.cpp` | 9 | 688 | 9 | 9 |  | 0x7f7f5 |
| 26 | `src/engine/audio/AESoundRessource.cpp` | 28 | 1730 | 47 | 47 |  | 0x7fa31 |
| 27 | `src/engine/math/Matrix.cpp` | 30 | 3796 | 30 | 30 |  | 0x8012d |
| 28 | `src/engine/math/Quaternion.cpp` | 26 | 1662 | 27 | 27 |  | 0x81009 |
| 29 | `src/engine/core/GameText.cpp` | 8 | 806 | 9 | 9 |  | 0x8165d |
| 30 | `src/engine/core/ApplicationManager.cpp` | 70 | 11310 | 77 | 77 |  | 0x8193d |
| 31 | `src/engine/render/Engine.cpp` | 72 | 11690 | 75 | 75 |  | 0x83ee9 |
| 32 | `src/engine/math/AEMath.cpp` | 10 | 180 | 10 | 10 |  | 0x86a29 |
| 33 | `src/engine/render/SpriteSystem.cpp` | 7 | 1496 | 7 | 7 |  | 0x86add |
| 34 | `src/engine/render/Material.cpp` | 1 | 416 | 1 | 1 |  | 0x870b9 |
| 35 | `src/engine/file/ConfigReader.cpp` | 7 | 1216 | 14 | 14 |  | 0x87259 |
| 36 | `src/engine/render/shaders/SpecCubeAlphaMapping.cpp` | 5 | 1060 | 6 | 6 |  | 0x87985 |
| 37 | `src/game/core/BumpShaderParticle.cpp` | 5 | 1372 | 6 | 6 |  | 0x87d35 |
| 38 | `src/engine/render/shaders/GreenShader.cpp` | 5 | 758 | 6 | 6 |  | 0x88215 |
| 39 | `src/engine/render/shaders/DrawFBOShader.cpp` | 9 | 962 | 9 | 9 |  | 0x88491 |
| 40 | `src/game/core/BumpRimCubeShader_new.cpp` | 5 | 1962 | 6 | 6 |  | 0x88821 |
| 41 | `src/engine/render/shaders/SimpleShader.cpp` | 5 | 492 | 6 | 6 |  | 0x88f51 |
| 42 | `src/engine/render/shaders/TextureAlphaTestShader.cpp` | 7 | 918 | 8 | 8 |  | 0x890c1 |
| 43 | `src/engine/render/shaders/BloomShader.cpp` | 11 | 3778 | 11 | 11 |  | 0x893dd |
| 44 | `src/engine/render/shaders/EnergyShield.cpp` | 5 | 1114 | 6 | 6 |  | 0x8a201 |
| 45 | `src/engine/render/shaders/BlurShader.cpp` | 10 | 1706 | 10 | 10 |  | 0x8a5e1 |
| 46 | `src/game/core/BumpShaderCloak.cpp` | 5 | 1690 | 6 | 6 |  | 0x8abf1 |
| 47 | `src/game/core/BumpShaderV4.cpp` | 5 | 1182 | 6 | 6 |  | 0x8b211 |
| 48 | `src/engine/render/shaders/TexOnlyShader.cpp` | 5 | 570 | 6 | 6 |  | 0x8b635 |
| 49 | `src/engine/render/shaders/BumpShader.cpp` | 5 | 1442 | 6 | 6 |  | 0x8b7f5 |
| 50 | `src/engine/render/shaders/TextureVtxColorShader.cpp` | 7 | 1148 | 8 | 8 |  | 0x8bd1d |
| 51 | `src/game/core/BumpShaderV3.cpp` | 5 | 1566 | 6 | 6 |  | 0x8c11d |
| 52 | `src/engine/render/shaders/SimpleRefractionShader.cpp` | 5 | 1218 | 6 | 6 |  | 0x8c6c1 |
| 53 | `src/game/core/TextureConference.cpp` | 5 | 658 | 10 | 10 |  | 0x8cb09 |
| 54 | `src/engine/render/shaders/GlowPPShader.cpp` | 11 | 2596 | 11 | 11 |  | 0x8cd1d |
| 55 | `src/game/core/BumpShaderV2.cpp` | 5 | 1182 | 6 | 6 |  | 0x8d6b9 |
| 56 | `src/engine/render/shaders/TextureLightShader.cpp` | 5 | 1228 | 6 | 6 |  | 0x8dadd |
| 57 | `src/engine/render/shaders/NoTexVtxColorShader.cpp` | 5 | 542 | 6 | 6 |  | 0x8df2d |
| 58 | `src/engine/render/shaders/GenericShader2.cpp` | 5 | 974 | 6 | 6 |  | 0x8e0d1 |
| 59 | `src/engine/render/shaders/BaseShader.cpp` | 10 | 848 | 10 | 10 |  | 0x8e425 |
| 60 | `src/engine/render/shaders/GenericShader.cpp` | 5 | 1226 | 6 | 6 |  | 0x8e751 |
| 61 | `src/engine/render/shaders/GenericShader1.cpp` | 5 | 974 | 6 | 6 |  | 0x8eba1 |
| 62 | `src/engine/render/shaders/PostBWShader.cpp` | 9 | 1150 | 9 | 9 |  | 0x8eef5 |
| 63 | `src/engine/render/shaders/DNSShader.cpp` | 5 | 1286 | 6 | 6 |  | 0x8f2d5 |
| 64 | `src/game/core/BumpShaderRefract.cpp` | 5 | 1006 | 6 | 6 |  | 0x8f761 |
| 65 | `src/engine/render/shaders/BumpRimCubeShader.cpp` | 5 | 1814 | 6 | 6 |  | 0x8fad5 |
| 66 | `src/engine/render/shaders/VertexColorShader.cpp` | 5 | 1236 | 6 | 6 |  | 0x90171 |
| 67 | `src/engine/render/shaders/NoTexShader.cpp` | 5 | 510 | 6 | 6 |  | 0x905c9 |
| 68 | `src/engine/render/shaders/CubeMapping.cpp` | 5 | 1008 | 6 | 6 |  | 0x9074d |
| 69 | `src/engine/render/shaders/CubeNormalMapping.cpp` | 5 | 1188 | 6 | 6 |  | 0x90ac1 |
| 70 | `src/engine/render/shaders/TextureShader.cpp` | 7 | 1018 | 8 | 8 |  | 0x90ee9 |
| 71 | `src/engine/render/shaders/BumpMapping.cpp` | 5 | 756 | 6 | 6 |  | 0x91269 |
| 72 | `src/engine/render/shaders/MaskShader.cpp` | 5 | 808 | 6 | 6 |  | 0x914e1 |
| 73 | `src/engine/render/shaders/SandboxShader.cpp` | 5 | 976 | 6 | 6 |  | 0x9178d |
| 74 | `src/engine/render/shaders/VertexColorAlphaTextureShader.cpp` | 5 | 1236 | 6 | 6 |  | 0x91ae1 |
| 75 | `src/engine/render/shaders/GlowShader.cpp` | 5 | 638 | 6 | 6 |  | 0x91f39 |
| 76 | `src/engine/render/shaders/ColorMixAdd.cpp` | 5 | 728 | 6 | 6 |  | 0x9213d |
| 77 | `src/engine/render/shaders/PulseShader.cpp` | 5 | 1218 | 6 | 6 |  | 0x92399 |
| 78 | `src/engine/render/shaders/SpecCubeMapping.cpp` | 5 | 1036 | 6 | 6 |  | 0x927e1 |
| 79 | `src/engine/audio/FModSound.cpp` | 46 | 4484 | 46 | 46 |  | 0x92b71 |
| 80 | `src/platform/android/NdkHooks.cpp` | 41 | 5200 | 41 | 41 |  | 0x93c89 |
| 81 | `src/platform/android/NdkAutosave.cpp` | 4 | 176 | 4 | 4 |  | 0x950dd |
| 82 | `src/engine/render/LODManager.cpp` | 8 | 450 | 9 | 9 |  | 0x9518d |
| 83 | `src/game/mission/Generator.cpp` | 13 | 9344 | 15 | 14 | `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 0x9539d |
| 84 | `src/game/menu/MTitle.cpp` | 14 | 672 | 21 | 21 |  | 0x97985 |
| 85 | `src/game/mission/Objective.cpp` | 12 | 1214 | 16 | 16 |  | 0x97bfd |
| 86 | `src/engine/math/BoundingAAB.cpp` | 9 | 886 | 9 | 9 |  | 0x98105 |
| 87 | `src/game/core/CutScene.cpp` | 15 | 5298 | 15 | 15 |  | 0x983e5 |
| 88 | `src/game/ship/PlayerEgo.cpp` | 168 | 37348 | 174 | 174 |  | 0x998cd |
| 89 | `src/game/ui/TouchSlider.cpp` | 15 | 900 | 15 | 15 |  | 0xa228d |
| 90 | `src/game/ship/Player.cpp` | 101 | 7738 | 115 | 115 |  | 0xa2571 |
| 91 | `src/engine/render/AEGeometry.cpp` | 45 | 3500 | 45 | 45 |  | 0xa4399 |
| 92 | `src/game/ship/PlayerJumpgate.cpp` | 11 | 820 | 11 | 11 |  | 0xa4fa5 |
| 93 | `src/game/ship/PlayerWormHole.cpp` | 13 | 1548 | 13 | 13 |  | 0xa5255 |
| 94 | `src/game/ship/KIPlayer.cpp` | 54 | 3698 | 60 | 60 |  | 0xa57c9 |
| 95 | `src/game/ui/ListItem.cpp` | 55 | 2212 | 55 | 55 |  | 0xa63e1 |
| 96 | `src/game/world/Station.cpp` | 35 | 2068 | 44 | 44 |  | 0xa692d |
| 97 | `src/game/weapons/RepairBeam.cpp` | 6 | 2612 | 7 | 7 |  | 0xa7101 |
| 98 | `src/engine/render/ImagePart.cpp` | 5 | 180 | 5 | 5 |  | 0xa7a75 |
| 99 | `src/game/mission/Explosion.cpp` | 17 | 5034 | 17 | 17 |  | 0xa7af5 |
| 100 | `src/game/mission/Status.cpp` | 116 | 20174 | 131 | 130 | `_ZN6Status16missionCompletedEbbx` | 0xa8ac1 |
| 101 | `src/game/world/Level.cpp` | 92 | 101728 | 107 | 105 | `_ZN5Level10createShipEiiiP8Waypointbb`, `_ZN5Level13createMissionEv` | 0xadac9 |
| 102 | `src/game/world/StarMap.cpp` | 22 | 22262 | 33 | 33 |  | 0xc67a5 |
| 103 | `src/game/mission/RecordHandler.cpp` | 34 | 18684 | 44 | 44 |  | 0xcba2d |
| 104 | `src/game/core/GameInit.cpp` | 3 | 2116 | 5 | 5 |  | 0xd03e5 |
| 105 | `src/engine/render/Sprite.cpp` | 18 | 1014 | 18 | 18 |  | 0xd0d19 |
| 106 | `src/game/ui/Layout.cpp` | 50 | 24214 | 50 | 48 | `_ZN6LayoutC1Ev`, `_ZN6LayoutC2Ev` | 0xd1029 |
| 107 | `src/game/menu/ModStation.cpp` | 29 | 29498 | 40 | 38 | `_ZN10ModStationC1Ev`, `_ZN10ModStationC2Ev` | 0xd52d1 |
| 108 | `src/game/ship/PlayerFighter.cpp` | 37 | 17094 | 38 | 38 |  | 0xdc2f9 |
| 109 | `src/game/mission/Item.cpp` | 57 | 2058 | 58 | 58 |  | 0xe0091 |
| 110 | `src/game/ship/PlayerStatic.cpp` | 11 | 416 | 19 | 19 |  | 0xe08cd |
| 111 | `src/game/world/NewsItem.cpp` | 5 | 174 | 5 | 5 |  | 0xe09c9 |
| 112 | `src/game/ui/WantedWindow.cpp` | 17 | 7502 | 18 | 18 |  | 0xe0a45 |
| 113 | `src/game/ship/PlayerAsteroid.cpp` | 24 | 3838 | 24 | 24 |  | 0xe2649 |
| 114 | `src/game/core/Globals.cpp` | 40 | 24948 | 41 | 41 |  | 0xe31e1 |
| 115 | `src/engine/render/GameResource.cpp` | 2 | 35204 | 8 | 8 |  | 0x112d4d |
| 116 | `src/game/world/SystemPathFinder.cpp` | 13 | 880 | 20 | 20 |  | 0x11b711 |
| 117 | `src/game/world/Route.cpp` | 25 | 1982 | 29 | 29 |  | 0x11bb75 |
| 118 | `src/game/ship/PlayerStaticFar.cpp` | 14 | 874 | 14 | 14 |  | 0x11c255 |
| 119 | `src/engine/render/ImageFactory.cpp` | 14 | 1628 | 15 | 15 |  | 0x11c54d |
| 120 | `src/game/ship/PlayerCreature.cpp` | 20 | 1400 | 21 | 21 |  | 0x11cb5d |
| 121 | `src/engine/render/LensFlare.cpp` | 6 | 1810 | 6 | 6 |  | 0x11d00d |
| 122 | `src/game/world/Standing.cpp` | 23 | 892 | 23 | 23 |  | 0x11d6a9 |
| 123 | `src/game/core/HangarList.cpp` | 20 | 3134 | 26 | 26 |  | 0x11d9f5 |
| 124 | `src/game/menu/MiningGame.cpp` | 16 | 4892 | 16 | 16 |  | 0x11e7a9 |
| 125 | `src/engine/math/BoundingVolume.cpp` | 11 | 666 | 11 | 11 |  | 0x11f6dd |
| 126 | `src/engine/file/FileRead.cpp` | 22 | 7502 | 30 | 30 |  | 0x11f945 |
| 127 | `src/game/ship/PlayerStation.cpp` | 17 | 7754 | 23 | 23 |  | 0x121819 |
| 128 | `src/game/world/Wanted.cpp` | 31 | 412 | 31 | 31 |  | 0x122891 |
| 129 | `src/game/ui/MenuTouchWindow.cpp` | 31 | 53292 | 41 | 40 | `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 0x12299d |
| 130 | `src/game/mission/PendingProduct.cpp` | 6 | 376 | 6 | 6 |  | 0x12d3c9 |
| 131 | `src/game/weapons/Radar.cpp` | 19 | 17846 | 19 | 18 | `_ZN5Radar4drawEP6PlayerP3Hudi` | 0x12d485 |
| 132 | `src/game/core/AutoPilotList.cpp` | 10 | 2012 | 10 | 10 |  | 0x131065 |
| 133 | `src/game/ui/ListItemWindow.cpp` | 12 | 10086 | 12 | 12 |  | 0x131529 |
| 134 | `src/game/world/StarSystem.cpp` | 20 | 9548 | 32 | 32 |  | 0x133b69 |
| 135 | `src/game/ui/ScrollTouchBox.cpp` | 17 | 1442 | 17 | 17 |  | 0x135599 |
| 136 | `src/game/world/LevelScript.cpp` | 17 | 75482 | 17 | 16 | `_ZN11LevelScript7processEi` | 0x135add |
| 137 | `src/engine/render/MovingStars.cpp` | 7 | 2320 | 7 | 7 |  | 0x146051 |
| 138 | `src/game/ui/ChoiceWindow.cpp` | 23 | 3764 | 23 | 23 |  | 0x1467ed |
| 139 | `src/game/weapons/BombGun.cpp` | 8 | 2510 | 10 | 10 |  | 0x1475cd |
| 140 | `src/game/ui/HangarWindow.cpp` | 32 | 28840 | 32 | 31 | `_ZN12HangarWindow6renderEv` | 0x147d21 |
| 141 | `src/game/menu/HackingGame.cpp` | 18 | 3608 | 18 | 18 |  | 0x14ecc9 |
| 142 | `src/game/ui/MissionsWindow.cpp` | 13 | 6304 | 13 | 13 |  | 0x14fa11 |
| 143 | `src/game/weapons/TractorBeam.cpp` | 6 | 1100 | 6 | 6 |  | 0x1511b5 |
| 144 | `src/game/core/RadioMessage.cpp` | 18 | 2156 | 18 | 18 |  | 0x15158d |
| 145 | `src/engine/math/BoundingSphere.cpp` | 9 | 566 | 9 | 9 |  | 0x151d29 |
| 146 | `src/game/weapons/Gun.cpp` | 28 | 7652 | 28 | 27 | `_ZN3Gun22calcCharacterCollisionEv` | 0x151f21 |
| 147 | `src/game/ship/PlayerFixedObject.cpp` | 31 | 6292 | 32 | 32 |  | 0x153a7d |
| 148 | `src/game/core/Radio.cpp` | 11 | 1790 | 11 | 11 |  | 0x154f7d |
| 149 | `src/game/world/SolarSystem.cpp` | 30 | 1070 | 30 | 30 |  | 0x1555dd |
| 150 | `src/game/mission/GameRecord.cpp` | 5 | 3172 | 5 | 5 |  | 0x155901 |
| 151 | `src/game/weapons/MineGun.cpp` | 8 | 1292 | 9 | 9 |  | 0x156479 |
| 152 | `src/game/mission/Achievements.cpp` | 28 | 1966 | 28 | 28 |  | 0x1568d1 |
| 153 | `src/game/ship/PlayerTurret.cpp` | 21 | 3966 | 23 | 23 |  | 0x157041 |
| 154 | `src/game/world/Waypoint.cpp` | 9 | 510 | 9 | 9 |  | 0x157c55 |
| 155 | `src/game/ui/StatusWindow.cpp` | 13 | 10862 | 13 | 13 |  | 0x157d81 |
| 156 | `src/game/ship/TargetFollowCamera.cpp` | 42 | 4912 | 42 | 42 |  | 0x15a4c9 |
| 157 | `src/game/mission/Mission.cpp` | 55 | 2900 | 55 | 55 |  | 0x15b64d |
| 158 | `src/engine/render/Trail.cpp` | 12 | 2018 | 12 | 12 |  | 0x15bd59 |
| 159 | `src/game/weapons/SentryGun.cpp` | 7 | 294 | 7 | 7 |  | 0x15c381 |
| 160 | `src/game/world/NewsTicker.cpp` | 11 | 8818 | 15 | 15 |  | 0x15c469 |
| 161 | `src/engine/render/Sparks.cpp` | 10 | 1152 | 10 | 10 |  | 0x15e38d |
| 162 | `src/game/ship/PlayerJunk.cpp` | 10 | 548 | 10 | 10 |  | 0x15e721 |
| 163 | `src/game/weapons/RocketGun.cpp` | 10 | 3478 | 13 | 13 |  | 0x15e8f9 |
| 164 | `src/engine/render/MarqueeImage.cpp` | 9 | 438 | 9 | 9 |  | 0x15f59d |
| 165 | `src/game/weapons/ObjectGun.cpp` | 12 | 3832 | 20 | 20 |  | 0x15f701 |
| 166 | `src/game/world/SpacePoint.cpp` | 8 | 140 | 8 | 8 |  | 0x1603ad |
| 167 | `src/game/ui/Hud.cpp` | 49 | 26988 | 54 | 53 | `_ZN3Hud4drawExxP9PlayerEgobjj` | 0x1603fd |
| 168 | `src/game/ui/DialogueWindow.cpp` | 27 | 7074 | 27 | 27 |  | 0x166bfd |
| 169 | `src/game/ui/TouchButton.cpp` | 44 | 6998 | 44 | 44 |  | 0x1684f1 |
| 170 | `src/game/weapons/SpriteGun.cpp` | 8 | 46 | 8 | 8 |  | 0x169aa5 |
| 171 | `src/game/menu/SpaceLounge.cpp` | 26 | 41374 | 34 | 33 | `_ZN11SpaceLounge10drawLoungeEv` | 0x169ad1 |
| 172 | `src/engine/render/MeshMerger.cpp` | 14 | 3718 | 14 | 14 |  | 0x1737a1 |
| 173 | `src/game/ui/ScrollTouchWindow.cpp` | 16 | 1420 | 16 | 16 |  | 0x174129 |
| 174 | `src/game/ship/Ship.cpp` | 93 | 4872 | 97 | 97 |  | 0x17457d |
| 175 | `src/game/menu/ModMainMenu.cpp` | 15 | 1696 | 21 | 21 |  | 0x1757a1 |
| 176 | `src/game/world/Galaxy.cpp` | 16 | 1596 | 17 | 17 |  | 0x175e09 |
| 177 | `src/game/ship/PlayerGasCloud.cpp` | 15 | 3366 | 26 | 26 |  | 0x1763cd |
| 178 | `src/game/mission/BluePrint.cpp` | 27 | 1416 | 27 | 27 |  | 0x176f21 |
| 179 | `src/game/ship/Agent.cpp` | 48 | 1224 | 51 | 51 |  | 0x1773c5 |
| 180 | `src/game/weapons/BeamGun.cpp` | 9 | 1264 | 11 | 11 |  | 0x177755 |
| 181 | `src/game/menu/MGame.cpp` | 40 | 38044 | 45 | 43 | `_ZN5MGame10OnTouchEndEiiPv`, `_ZN5MGame8OnUpdateEv` | 0x177b31 |
| 182 | `src/engine/render/SimpleMeshMerger.cpp` | 2 | 1480 | 2 | 2 |  | 0x180ecd |
| 183 | `src/engine/render/AMeshMerger.cpp` | 3 | 44 | 3 | 3 |  | 0x1811e5 |
| 184 | `src/engine/render/LodMeshMerger.cpp` | 11 | 2582 | 11 | 11 |  | 0x181201 |
| 185 | `src/engine/render/IParticleSystem.cpp` | 14 | 4286 | 15 | 15 |  | 0x1819c1 |
| 186 | `src/engine/render/ParticleSystemSprite.cpp` | 14 | 2546 | 18 | 18 |  | 0x182b89 |
| 187 | `src/engine/render/ParticleSystemManager.cpp` | 31 | 2166 | 37 | 37 |  | 0x18357d |
| 188 | `src/engine/render/ParticleSettings.cpp` | 5 | 5700 | 6 | 6 |  | 0x183e69 |
| 189 | `src/engine/render/ParticleSystemMesh.cpp` | 24 | 5798 | 24 | 24 |  | 0x185491 |
| 190 | `src/engine/core/Array.cpp` | 0 | 0 | 31 | 31 |  | — |
| 191 | `src/engine/core/ArrayInstantiations.cpp` | 0 | 0 | 3 | 3 |  | — |

---

## 6. Residual distribution (current tree)

17 exported functions are not byte-exact, in 13 TUs. Every other TU is 100%
exact. The C1/C2 rows below represent two aliases each. The residual set is the same as round 5 (no new residual, no residual
fixed by a flag):

| TU | function | pct | size |
|---|---|---:|---:|
| `src/game/menu/MGame.cpp` | `MGame::OnTouchEnd(int, int, void*)` | 82.0 | 8080 |
| `src/game/menu/MGame.cpp` | `MGame::OnUpdate()` | 99.9 | 11440 |
| `src/game/menu/ModStation.cpp` | `ModStation::ModStation()` (C1/C2) | 99.2 | 1008 |
| `src/game/menu/SpaceLounge.cpp` | `SpaceLounge::drawLounge()` | 95.7 | 1948 |
| `src/game/mission/Generator.cpp` | `Generator::createMission(Agent*, Array<SolarSystem*>*)` | 97.2 | 1996 |
| `src/game/mission/Status.cpp` | `Status::missionCompleted(bool, bool, long long)` | 71.0 | 1432 |
| `src/game/ui/HangarWindow.cpp` | `HangarWindow::render()` | 94.7 | 9700 |
| `src/game/ui/Hud.cpp` | `Hud::draw(long long, long long, PlayerEgo*, bool, unsigned int, unsigned int)` | 96.7 | 9332 |
| `src/game/ui/Layout.cpp` | `Layout::Layout()` (C1/C2) | 45.9 | 7072 |
| `src/game/ui/MenuTouchWindow.cpp` | `MenuTouchWindow::OnTouchBegin(int, int, void*)` | 98.1 | 1896 |
| `src/game/weapons/Gun.cpp` | `Gun::calcCharacterCollision()` | 93.4 | 1984 |
| `src/game/weapons/Radar.cpp` | `Radar::draw(Player*, Hud*, int)` | 75.4 | 9908 |
| `src/game/world/Level.cpp` | `Level::createMission()` | 92.5 | 17172 |
| `src/game/world/Level.cpp` | `Level::createShip(int, int, int, Waypoint*, bool, bool)` | 70.6 | 2764 |
| `src/game/world/LevelScript.cpp` | `LevelScript::process(int)` | 57.4 | 57232 |

The `.ARM.extab` residual (659 B) is the LSDA of these bodies plus
`Radar::Radar(Level*)`; it is downstream of `.text`/cold-block placement, not
a separate flag. `.rel.plt` is byte-identical (0 differing bytes).

---

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11119
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
ulimit -v 12000000
cmake --build cmake-build-match --target check -j2

# per-TU table, -g sweep, -O sweep, call-count fingerprints
python3 _work/reconstructions/build_provenance_round6.py \
  --repo /home/debian/GalaxyOnFire2/wt/w11119 \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --build cmake-build-match
```

Stock build-system probes are in `/dev/shm/w11119/ndkprobe` and
`/dev/shm/w11119/cmakeprobe` (scratch; not committed). The exact commands are
in §3.2.

---

## 8. Files changed

* `_work/reconstructions/build-provenance-round6-build-system.md` — this note.
* `_work/reconstructions/build_provenance_round6.py` — the per-TU table,
  `-g`/`-O` sweeps, and fingerprint checker.
* No `src/`, `CMakeLists.txt`, baseline, allowlist, or doc change.
