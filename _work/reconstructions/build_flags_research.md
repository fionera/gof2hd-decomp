# Original build reconstruction — toolchain and flags

Status: research note (2026-11-01). Every claim below is backed by a command run against
`../_work/bins/android_2.0.16_libgof2hdaa.so` or by a controlled rebuild of the tree.

## 1. Toolchain identity (hard evidence)

| evidence | value | source |
|---|---|---|
| `.comment` | `Android (4751641 based on r328903) clang version 7.0.2` | `readelf -p .comment` |
| `.comment` (second) | `GCC: (GNU) 4.9.x 20150123 (prerelease)` | same (NDK r18b `libgcc`/crt objects) |
| `.note.android.ident` | `r18b`, build `5063045` | `readelf -n` |
| `.note.gnu.gold-version` | `gold 1.12` | `readelf -n` |
| ELF flags | `Version5 EABI, soft-float ABI` | `readelf -h` |
| `.ARM.attributes` | ARM v7, Thumb-2, VFPv3, NEONv1, GOT-indirect, wchar_t 4, enum int, 8-byte align, unaligned v6 | `readelf -A` |
| `DT_NEEDED` | liblog, libGLESv2, libGLESv1_CM, libEGL, libandroid, libfmodex, libfmodevent, libm, libdl, libc | `readelf -d` |
| libc++ | no `libc++_shared.so` NEEDED → statically linked (`c++_static`) | `readelf -d` |
| APK build date | `Mon Oct 22 10:18:17 CEST 2018` | `assets/crashlytics-build.properties` |
| APK min/target SDK | min 16, target 27 | parsed `AndroidManifest.xml` |

The app's C++ code is clang 7.0.2 from NDK r18b; the GCC 4.9.x string is from the NDK's
`libgcc`/crt objects, not the app TUs.

## 2. Global flags — evidence and controlled A/B results

The tree's toolchain (`cmake/toolchain.cmake`) uses:

```
-target armv7-none-linux-androideabi21 -march=armv7-a -mthumb -mfpu=neon -mfloat-abi=softfp
-std=c++11 -fPIC -frtti -fstack-protector-strong -Oz -g -stdlib=libc++
```

Each was tested by rebuilding the whole tree in a scratch worktree with the flag changed and
counting `byte_exact` / `linked_exact` / `size_match` against the original. Baseline with the
four scoped `-mllvm` flags removed: **byte 1407, linked 4492, size 4520** (the scoped flags add
+8 byte / +1 linked; see §3).

| variant | byte | linked | size | verdict |
|---|---:|---:|---:|---|
| `-Oz` (baseline) | 1407 | 4492 | 4520 | correct |
| `-Os` | 1252 | 2649 | 2882 | much worse |
| `-O2` | 1157 | 2161 | 2413 | much worse |
| `-O1` | 1031 | 1783 | 2675 | much worse |
| `-O3` | 1156 | 2121 | 2249 | much worse |
| `-fno-strict-aliasing` | 1335 | 4346 | — | worse → strict aliasing is on |
| `-fomit-frame-pointer` | 1273 | 1921 | — | catastrophic → frame pointers are kept |
| `-fno-omit-frame-pointer` | 1407 | 4492 | 4520 | neutral (default already keeps them) |
| `-fstack-protector` | 1343 | 3784 | — | worse → `-strong` is correct |
| `-fstack-protector-all` | 57 | 956 | — | catastrophic |
| `-fno-stack-protector` | 1343 | 3780 | 3788 | worse |
| `-fno-rtti` | 1375 | 4492 | 4520 | worse byte → RTTI is on |
| `-fno-exceptions` | — | — | — | build fails (source uses exceptions) |
| `-fno-unwind-tables -fno-asynchronous-unwind-tables` | 1407 | 4492 | 4520 | neutral for codegen; `.ARM.extab` only |
| `-fno-common` | 1407 | 4492 | 4520 | neutral |
| `-fno-builtin` | 1337 | 3981 | 4118 | much worse |
| `-fno-use-cxa-atexit` | 1404 | 4491 | 4519 | slightly worse |
| `-fno-sized-deallocation` | 1407 | 4492 | 4520 | neutral |
| `-fno-delete-null-pointer-checks` | 1407 | 4492 | 4520 | neutral |
| `-fwrapv` | 1346 | 4458 | 4498 | worse |
| `-fno-strict-overflow` | 1346 | 4458 | 4498 | worse |
| `-fno-lax-vector-conversions` | 1407 | 4492 | 4520 | neutral |
| `-mno-unaligned-access` | 1342 | 4437 | — | worse → unaligned access is on |
| `-fno-vectorize -fno-slp-vectorize` | 1340 | 4380 | — | worse → vectorization is on |
| `-fno-inline-functions` | 1217 | 3509 | — | much worse |
| `-fno-threadsafe-statics` | 1351 | 4488 | — | worse |
| `-fvisibility=hidden` | 56 | 243 | — | wrong (only 244 symbols compared) |
| `-mcpu=cortex-a8` | 1304 | 4223 | 4364 | worse → plain `armv7-a` |
| `-marm` | 55 | 235 | 341 | catastrophic → Thumb-2 |
| `-std=gnu++11` | 1407 | 4492 | 4520 | neutral vs `-std=c++11` |
| `-mfloat-abi=hard` | — | — | — | build fails (ABI/libs) |
| `-mfpu=vfpv3-d16` | — | — | — | build fails (NEON uses) |
| `-fno-integrated-as` | — | — | — | build fails |

**Conclusion:** the global flag set is not a guess; it is the unique combination that reproduces
the original's codegen among the tested candidates. `-Oz`, `-g`, `-std=c++11`, `-fPIC`,
`-frtti`, `-fstack-protector-strong`, `-march=armv7-a -mthumb -mfpu=neon -mfloat-abi=softfp`,
default strict aliasing, frame pointers, vectorization, inlining, and threadsafe statics are all
supported by the A/B counts.

## 3. Scoped `-mllvm` flags — local compensations, not original build flags

Current scoped flags in `CMakeLists.txt`:

| TU | flags | purpose |
|---|---|---|
| `LevelCreateScene.cpp` | `-mllvm -sink-insts-to-avoid-spills` | keep `&Globals::rnd` in r8 |
| `LevelScript.cpp` | `-mllvm -condsider-local-interval-cost` | split-candidate accounting |
| `Radar.cpp` | `-mllvm -likely-branch-weight=80 -mllvm -unlikely-branch-weight=1` | flatter expect weights |
| `ModStation.cpp` | `-mllvm -likely-branch-weight=20 -mllvm -unlikely-branch-weight=12` | EH landing-pad order |

With the four flags removed, the tree drops from **1415/4493/4494** to **1407/4492/4493**
(byte/linked/fuzzy). Applying any one of them **globally** is much worse:

| global flag | byte | linked | size |
|---|---:|---:|---:|
| `-mllvm -sink-insts-to-avoid-spills` | 1349 | 4343 | — |
| `-mllvm -condsider-local-interval-cost` | 1367 | 4471 | — |
| `-mllvm -likely-branch-weight=80:1` | 1378 | 4492 | — |
| `-mllvm -likely-branch-weight=20:12` | 1382 | 4488 | — |

So these are **not** original global build flags. They are per-TU compensations for a source or
TU-membership mismatch. The original almost certainly used LLVM 7.0.2's defaults. The right
long-term fix is to reshape the source/TU so the flags become unnecessary, not to add more flags.

## 4. What this means for the remaining 30 functions

The remaining gap is not a missing compiler flag. It is:

1. **Source shape** — the original's exact expression trees, local lifetimes, and statement order.
2. **TU membership / link order** — `.rodata`, literal pools, `__GLOBAL__sub_I_*`, and -Oz
   same-TU inlining follow TU ownership. The known divergences (AbyssEngine mega-TU, etc.) are
   still open.
3. **Link layout** — `.rodata` -1328 B, `.bss` +12, `.data.rel.ro` +32, `.init_array` 3 vs 1,
   dynsym order, build-id.

The scoped flags should be treated as a temporary measurement aid. When a function's source/TU
is corrected, re-test with the flag removed; if the function stays exact, delete the flag.

## 5. Reproduction commands

```
# toolchain identity
readelf -p .comment ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -n ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -A ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -d ../_work/bins/android_2.0.16_libgof2hdaa.so

# APK metadata
unzip -p "../Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk" assets/crashlytics-build.properties

# controlled flag A/B (scratch worktree, scoped flags removed)
cmake --preset default -DGOF2_EXTRA_FLAGS="<flags>"
cmake --build cmake-build-match --target link_so -j16
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
```
