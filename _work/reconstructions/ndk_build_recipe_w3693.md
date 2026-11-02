# Definitive ndk-build recipe (w3693)

Status: research note, 2026-11-02. Worktree `wt/w3693`, branch `wave87-w3693`,
base `88aaf59f`. Every claim below is backed by a command run against
`../_work/bins/android_2.0.16_libgof2hdaa.so` or by a controlled A/B rebuild of
this tree with the gate counts recorded.

## 0. Bottom line

The original is an **NDK r18b toolchain** build: clang 7.0.2, gold 1.12,
armeabi-v7a softfp, API 16, `c++_static`, release (`-Oz -DNDEBUG`), Thumb-2,
NEON, RTTI + exceptions. Its effective flags are the r18b ndk-build defaults
with one material exception (next paragraph).

The effective compile recipe is the r18b ndk-build command **with one material
exception**: the original's data layout is the **no-`-fdata-sections`** layout.
The r18b `GLOBAL_CFLAGS` default includes `-fdata-sections`, but the ELF proves
it was not effective in the original (or the original `Android.mk` set
`-fno-data-sections`): adding `-fdata-sections` to this tree changes `.rodata`
by `-0x160` and `.data` by `+4` and costs **163 byte_exact** (4496 -> 4333).
`-ffunction-sections` *is* effective (the original's `.ARM.exidx` has 194
duplicate target addresses: 193 empty-`.text` CANTUNWIND sentinels plus the
zero-size `AbyssEngine::getAppVersion`).

The link recipe is the r18b ndk-build default line: `-z now`, `--build-id`,
`--no-undefined`, `-z relro`, `-z noexecstack`, `--fix-cortex-a8`,
`--exclude-libs=libgcc.a/libatomic.a/libunwind.a`, `--warn-shared-textrel`,
`--fatal-warnings`, `--whole-archive zlib libzip`, crypto static,
`libc++_static.a`, `libc++abi.a`, `libandroid_support.a`, `libunwind.a`,
`-lgcc`, `-latomic`, and `LOCAL_LDLIBS` in the order below. Adding the full
ndk-build link set to the tree's current gold line is **byte-identical**
(sha256 `ee87486d...`).

## 1. Baseline (this tree, before any experiment)

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
```

```text
verify: byte 4496  linked 4496  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
=== DRIFT: 0 field(s) across 0 class(es) ===
```

`report.json`: `compared 4524`, `size_match 4524/4524`. Every ELF section is
size- and base-exact except `.ARM.extab` (`0x26290` vs original `0x26298`, -8 B;
the seven known body/LSDA residues). The original `.ARM.exidx` has 3566 entries
and 194 duplicate target addresses (193 empty-`.text` CANTUNWIND sentinels +
the zero-size `AbyssEngine::getAppVersion`); the count command is in §7.
Baseline `.so` sha256:
`ee87486de17d19e4b6cfa2b480681845d4b6e61a6af86121a2cae6996ce8e688`.

Original ELF facts (commands in §7):

| observable | value |
|---|---|
| `.comment` | `Android (4751641 based on r328903) clang version 7.0.2`; `GCC: (GNU) 4.9.x 20150123 (prerelease)` |
| `.note.android.ident` | API `16`, `r18b`, build `5063045` |
| `.note.gnu.gold-version` | `gold 1.12` |
| `.ARM.attributes` | ARMv7-A, Thumb-2, VFPv3, NEONv1, GOT-indirect, wchar_t 4, int enums, 8-byte align, unaligned v6 |
| `DT_NEEDED` order | `liblog, libGLESv2, libGLESv1_CM, libEGL, libandroid, libfmodex, libfmodevent, libm, libdl, libc` |
| `DT_FLAGS` / `DT_FLAGS_1` | `BIND_NOW` / `NOW` |
| `SONAME` | `libgof2hdaa.so` |
| `.dynamic` | `0x148` at `0x20fba4`; `RELCOUNT 1162` |
| `.init_array` / `.fini_array` | `0xc` / `0x8` |

## 2. Exact r18b ndk-build probe

A scratch project under `/tmp/w3693/ndkprobe` used the reconstructed module
variables and was built with the repo's NDK r18b:

```sh
.cache/ndk/android-ndk-r18b/ndk-build \
  NDK_PROJECT_PATH=/tmp/w3693/ndkprobe \
  APP_BUILD_SCRIPT=/tmp/w3693/ndkprobe/jni/Android.mk \
  NDK_APPLICATION_MK=/tmp/w3693/ndkprobe/jni/Application.mk V=1 -j8
```

The emitted compile command is the canonical r18b command:

```text
clang++ -target armv7-none-linux-androideabi16
  -fdata-sections -ffunction-sections -fstack-protector-strong -funwind-tables
  -no-canonical-prefixes -g
  -Wno-invalid-command-line-argument -Wno-unused-command-line-argument
  -fno-exceptions -fno-rtti
  -gcc-toolchain .../arm-linux-androideabi-4.9/prebuilt/linux-x86_64
  -fpic -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16
  -mthumb -Oz -DNDEBUG -mfpu=neon
  ... -std=c++11 -DANDROID -Wno-int-to-pointer-cast -Wno-int-to-void-pointer-cast
  -Wa,--noexecstack -Wformat -Werror=format-security
  -frtti -fexceptions
  --sysroot=.../sysroot -isystem .../sysroot/usr/include/arm-linux-androideabi
```

The emitted shared-library link command is:

```text
clang++ -Wl,-soname,libgof2hdaa.so -shared --sysroot=.../platforms/android-16/arch-arm
  <objects>
  -Wl,--whole-archive libz_gof2.a libzip_gof2.a -Wl,--no-whole-archive libcrypto_gof2.a
  libc++_static.a libc++abi.a libandroid_support.a libunwind.a
  -lgcc -Wl,--exclude-libs,libgcc.a
  -latomic -Wl,--exclude-libs,libatomic.a
  -target armv7-none-linux-androideabi16 -no-canonical-prefixes
  -gcc-toolchain .../arm-linux-androideabi-4.9/prebuilt/linux-x86_64
  -Wl,--fix-cortex-a8 -Wl,--exclude-libs,libunwind.a
  -Wl,--build-id -nostdlib++ -Wl,--no-undefined
  -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now
  -Wl,--warn-shared-textrel -Wl,--fatal-warnings
  -L.../platforms/android-16/arch-arm/usr/lib
  -llog -lGLESv2 -lGLESv1_CM -lEGL -landroid
  -L.../apk_libs -lfmodex -lfmodevent -lm -ldl
  -ldl -lc -lm
  -o libgof2hdaa.so
```

The probe's `readelf -d` `NEEDED` order is exactly the original's:

```text
liblog.so libGLESv2.so libGLESv1_CM.so libEGL.so libandroid.so
libfmodex.so libfmodevent.so libm.so libdl.so libc.so
```

### 2.1 FMOD must be in `LOCAL_LDLIBS`, not `LOCAL_SHARED_LIBRARIES`

A second probe used

```make
LOCAL_LDLIBS := -llog -lGLESv2 -lGLESv1_CM -lEGL -landroid -lm -ldl
LOCAL_SHARED_LIBRARIES := fmodex fmodevent
```

and produced

```text
libfmodex.so libfmodevent.so liblog.so libGLESv2.so libGLESv1_CM.so
libEGL.so libandroid.so libm.so libdl.so libc.so
```

which does **not** match the original. Therefore the original listed FMOD in
`LOCAL_LDLIBS` after `-landroid` and before `-lm -ldl` (or in an equivalent
`LOCAL_LDFLAGS` position after the system libs). This resolves the prior
"cannot distinguish" caveat.

## 3. Controlled compile-flag A/B at the current tree

Scratch toolchain wrapper (appends `GOF2_EXTRA_FLAGS` after the repo's forced
flags):

```sh
cat > /tmp/w3693/toolchain_extra.cmake <<'WRAPEOF'
include("/home/debian/GalaxyOnFire2/wt/w3693/cmake/toolchain.cmake")
set(GOF2_EXTRA_FLAGS "" CACHE STRING "extra matching flags")
if(GOF2_EXTRA_FLAGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GOF2_EXTRA_FLAGS}" CACHE STRING "matching ARM compile flags" FORCE)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GOF2_EXTRA_FLAGS}" CACHE STRING "matching ARM compile flags" FORCE)
endif()
WRAPEOF
```

Per variant:

```sh
cmake -S . -B /tmp/w3693/<variant> -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/tmp/w3693/toolchain_extra.cmake \
  -DGOF2_MATCH_OPT=-Oz \
  "-DGOF2_EXTRA_FLAGS=<flags>"
cmake --build /tmp/w3693/<variant> --target link_so -j8
cmake --build /tmp/w3693/<variant> --target check -j8
```

| variant | byte | linked | size | `.rodata` | `.data` | verdict |
|---|---:|---:|---:|---:|---:|---|
| baseline (no extra flags) | **4496** | **4496** | 4524/4524 | `0x41b67` | `0x296c` | reference |
| full r18b compile set **minus** `-fdata-sections` | **4496** | **4496** | 4524/4524 | `0x41b67` | `0x296c` | neutral; `.so` differs only in `.note.gnu.build-id` |
| `-fdata-sections` (app TUs only) | **4333** | 4496 | 4524/4524 | `0x41a07` | `0x2970` | **-163 byte** |
| `-fdata-sections` (vendored zlib/libzip only) | **4493** | 4496 | 4524/4524 | — | — | **-3 byte** |
| `-fdata-sections` (app + vendored) | **4315** | 4496 | 4524/4524 | — | — | **-181 byte** |
| full r18b compile set **plus** `-fdata-sections` | **4333** | 4496 | 4524/4524 | `0x41a07` | `0x2970` | **-163 byte** |
| full r18b compile set + `-fdata-sections -fno-data-sections` | **4496** | **4496** | 4524/4524 | `0x41b67` | `0x296c` | neutral; `.so` differs only in `.note.gnu.build-id` |
| `-g0` | **2069** | 4245 | 4524/4524 | — | — | catastrophic; `-g` is codegen-relevant |

The "full r18b compile set minus `-fdata-sections`" flags were:

```text
-DNDEBUG -fpic -fno-exceptions -fno-rtti -frtti -fexceptions
-no-canonical-prefixes -Wno-invalid-command-line-argument
-Wno-unused-command-line-argument -gcc-toolchain <NDK gcc dir>
-mfpu=vfpv3-d16 -mfpu=neon -Wa,--noexecstack -Wformat
-Werror=format-security -DANDROID
```

This is neutral: the resulting `.so` is byte-identical to the baseline except
for the 20-byte `.note.gnu.build-id` (offsets 477..496). In particular
`-fpic` (the ndk-build default) is neutral vs the tree's `-fPIC` at this tree
state, and the `-fno-exceptions -fno-rtti` -> `-frtti -fexceptions` ordering is
neutral. The same is true when the full r18b set is followed by
`-fdata-sections -fno-data-sections`: byte 4496 / linked 4496, `.so` differing
only in the build-id. That is the gate-neutral encoding of the recipe in §5.

### 3.1 The `-fdata-sections` finding

The original's `.rodata` and `.data` are **byte-identical** to the tree's
no-`-fdata-sections` build:

```text
.rodata orig VMA 0x1c9e10 size 0x41b67  base same  fdata size 0x41a07
.data   orig VMA 0x214000 size 0x296c   base same  fdata size 0x2970
```

```text
base  vs orig .rodata differing bytes: 0
fdata vs orig .rodata differing bytes: 113935
base  vs orig .data   differing bytes: 0
fdata vs orig .data   differing bytes: 3273
```

The `.rodata` difference is a padding/alignment shift, not different content:
the first 121,747 bytes are identical, then the fdata layout has 12 zero bytes
before `12AELowLevelFile` where the original has none. The fdata section is
`0x160` bytes shorter overall. The original's data layout is therefore the
no-fdata layout.

This is the one place where the r18b ndk-build *default* and the original ELF
disagree. Either the original `Android.mk` set `-fno-data-sections` (or
`LOCAL_CFLAGS := -fno-data-sections`), or the original was not stock ndk-build.
The binary cannot distinguish those two; it can only prove that
`-fdata-sections` was **not effective**. The tree is correct to omit it today.

## 4. Controlled link-flag A/B at the current tree

The exact gold command was extracted with:

```sh
ninja -C cmake-build-match -t commands libgof2hdaa.so | grep 'ld.gold -shared' | tail -1
```

It was re-run with the output path substituted and the ndk-build link set
inserted after `--build-id`. Every variant below produced the **same sha256**
as the baseline (`ee87486d...`), i.e. the full ndk-build link line is
byte-identical to the tree's minimal line:

| relink variant | sha256 | byte | linked |
|---|---|---:|---:|
| baseline | `ee87486d...` | 4496 | 4496 |
| `-z relro -z noexecstack --warn-shared-textrel --fatal-warnings --fix-cortex-a8` | `ee87486d...` | 4496 | 4496 |
| `--no-undefined` | `ee87486d...` | 4496 | 4496 |
| `libatomic.a --exclude-libs=libatomic.a` | `ee87486d...` | 4496 | 4496 |
| `libunwind.a --exclude-libs=libunwind.a` | `ee87486d...` | 4496 | 4496 |
| `libc++_static.a` + `libunwind.a` + `libatomic.a` | `ee87486d...` | 4496 | 4496 |
| full ndk-build link set (above + `--no-undefined`) | `ee87486d...` | 4496 | 4496 |
| `strip --strip-unneeded` instead of `--strip-all` | `ee87486d...` | 4496 | 4496 |

`--no-undefined` succeeding also proves the original did **not** need
`LOCAL_ALLOW_UNDEFINED_SYMBOLS := true`: all 207 undefined imports are
resolved by the NEEDED libraries at link time.

## 5. Reconstructed `Application.mk` / `Android.mk`

```make
# Application.mk
APP_ABI := armeabi-v7a
APP_PLATFORM := android-16
APP_STL := c++_static
APP_OPTIM := release
```

```make
# Android.mk
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := zlib
LOCAL_SRC_FILES := third_party/zlib/libz_gof2.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libzip
LOCAL_SRC_FILES := third_party/libzip/libzip_gof2.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := crypto
LOCAL_SRC_FILES := third_party/openssl/libcrypto_gof2.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := gof2hdaa
LOCAL_SRC_FILES := <original link order; see _work/original_layout/ and tools/gen_link_order.py>
LOCAL_CPP_FEATURES := rtti exceptions
LOCAL_ARM_NEON := true
LOCAL_C_INCLUDES := $(LOCAL_PATH)/src \
    $(LOCAL_PATH)/third_party/fmod/inc \
    $(LOCAL_PATH)/third_party/gl \
    $(LOCAL_PATH)/third_party/jni \
    $(LOCAL_PATH)/third_party/libzip \
    $(LOCAL_PATH)/third_party/crypto
LOCAL_CFLAGS := -Wno-int-to-pointer-cast -Wno-int-to-void-pointer-cast
# The original's effective data layout is the no-fdata-sections layout (see §3.1).
LOCAL_CFLAGS += -fno-data-sections
LOCAL_LDLIBS := -llog -lGLESv2 -lGLESv1_CM -lEGL -landroid \
                -L<apk-libs> -lfmodex -lfmodevent -lm -ldl
LOCAL_WHOLE_STATIC_LIBRARIES := zlib libzip
LOCAL_STATIC_LIBRARIES := crypto
include $(BUILD_SHARED_LIBRARY)
```

Notes:

* `LOCAL_MODULE := gof2hdaa` gives `libgof2hdaa.so` and the original SONAME.
* `LOCAL_CPP_FEATURES := rtti exceptions` appends `-frtti -fexceptions` after
  ndk-build's default `-fno-exceptions -fno-rtti`; the final state is RTTI +
  exceptions on, matching the original.
* `LOCAL_ARM_NEON := true` appends `-mfpu=neon` after the default
  `-mfpu=vfpv3-d16`; the original `.ARM.attributes` has NEONv1.
* `LOCAL_WHOLE_STATIC_LIBRARIES := zlib libzip` + `LOCAL_STATIC_LIBRARIES := crypto`
  reproduces the original's `--whole-archive zlib libzip --no-whole-archive crypto`
  order and exported symbol set.
* The exact original `LOCAL_SRC_FILES` list is not recoverable from the stripped
  `.so`; the original was linked without a section-ordering file, so its `.text`
  is input-object order. The recovered order is in
  `_work/original_layout/original_layout.json` / `tu_mapping.md` and the tree's
  `tools/gen_link_order.py` (`link_order.rsp`). The tree's
  `--section-ordering-file` is a reconstruction aid for its different TU split,
  not an original build flag.

## 6. What is proven vs still hypothesis

**Proven by the binary and the controlled rebuilds**

* NDK r18b, clang 7.0.2, gold 1.12; armeabi-v7a softfp; API 16; `c++_static`;
  Thumb-2; NEON; RTTI + exceptions; `-Oz`, `-g`, `-fstack-protector-strong`,
  `-funwind-tables`, `-ffunction-sections` (194 duplicate `.ARM.exidx` target
  addresses: 193 empty-`.text` sentinels + the zero-size
  `AbyssEngine::getAppVersion`).
* The original's effective data layout is the **no-`-fdata-sections`** layout:
  `.rodata`/`.data` are byte-identical to the tree's no-fdata build; adding
  `-fdata-sections` changes them and costs 163 byte_exact.
* `-g` is codegen-relevant: `-g0` drops byte 4496 -> 2069 and linked -> 4245.
* `-fpic` (ndk-build default) is neutral vs the tree's `-fPIC` at this tree
  state; the full r18b compile set minus `-fdata-sections` is neutral.
* The full r18b compile set with `-fdata-sections -fno-data-sections` is also
  neutral (byte 4496 / linked 4496), proving the `Android.mk` override in §5.
* The full r18b ndk-build link set is byte-identical to the tree's minimal line;
  `--no-undefined` succeeds and is neutral; `libc++_static.a`, `libunwind.a`,
  `libatomic.a`, `-z relro`, `-z noexecstack`, `--fix-cortex-a8`,
  `--warn-shared-textrel`, `--fatal-warnings` are all neutral.
* FMOD must be in `LOCAL_LDLIBS` after `-landroid` and before `-lm -ldl`;
  `LOCAL_SHARED_LIBRARIES` puts FMOD first and does not match the original
  `DT_NEEDED` order.
* `strip --strip-unneeded` and `strip --strip-all` produce the same bytes here.

**Still hypothesis / not recoverable**

* The exact original `Android.mk`/`Application.mk` text. No original build
  script survives in the APK or the source dumps.
* Whether the original explicitly wrote `-fno-data-sections` or used a
  non-ndk-build system that omitted it. The ELF proves only that
  `-fdata-sections` was not effective.
* The exact original `LOCAL_SRC_FILES` list; only its order is recovered.
* `APP_PLATFORM := android-16` explicit vs omitted (r18b's default is 16).
* `APP_OPTIM := release` explicit vs omitted (release is the default).
* The exact original `LOCAL_CFLAGS` warning suppressions; they are
  codegen-neutral.

## 7. Reproduction commands

```sh
# baseline
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8

# original ELF observables
readelf -p .comment ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -n         ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -AW        ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -dW        ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -SW        ../_work/bins/android_2.0.16_libgof2hdaa.so

# .ARM.exidx duplicate-target count (194 = 193 empty-.text sentinels + 1)
python3 - <<'PY'
import struct
from collections import Counter
d = open('../_work/bins/android_2.0.16_libgof2hdaa.so', 'rb').read()
shoff, = struct.unpack_from('<I', d, 0x20)
shentsize, shnum, shstrndx = struct.unpack_from('<HHH', d, 0x2e)
h = [struct.unpack_from('<IIIIIIIIII', d, shoff + i*shentsize) for i in range(shnum)]
stroff = h[shstrndx][4]
for e in h:
    n = d[stroff+e[0]:d.index(b'\0', stroff+e[0])].decode()
    if n == '.ARM.exidx':
        addr, off, size = e[3], e[4], e[5]
        t = []
        for i in range(0, size, 8):
            w, = struct.unpack_from('<I', d, off+i)
            v = w & 0x7fffffff
            if v & 0x40000000: v -= 0x80000000
            t.append((addr+i+v) & 0xffffffff)
        c = Counter(t)
        print('entries', size//8, 'duplicate targets',
              sum(1 for k, v in c.items() if v > 1))
PY

# actual ndk-build probe (see §2 for the Android.mk/Application.mk)
.cache/ndk/android-ndk-r18b/ndk-build \
  NDK_PROJECT_PATH=/tmp/w3693/ndkprobe \
  APP_BUILD_SCRIPT=/tmp/w3693/ndkprobe/jni/Android.mk \
  NDK_APPLICATION_MK=/tmp/w3693/ndkprobe/jni/Application.mk V=1 -j8

# compile-flag A/B
cat > /tmp/w3693/toolchain_extra.cmake <<'WRAPEOF'
include("/home/debian/GalaxyOnFire2/wt/w3693/cmake/toolchain.cmake")
set(GOF2_EXTRA_FLAGS "" CACHE STRING "extra matching flags")
if(GOF2_EXTRA_FLAGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GOF2_EXTRA_FLAGS}" CACHE STRING "matching ARM compile flags" FORCE)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GOF2_EXTRA_FLAGS}" CACHE STRING "matching ARM compile flags" FORCE)
endif()
WRAPEOF
cmake -S . -B /tmp/w3693/build-fdata -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/tmp/w3693/toolchain_extra.cmake \
  -DGOF2_MATCH_OPT=-Oz -DGOF2_EXTRA_FLAGS=-fdata-sections
cmake --build /tmp/w3693/build-fdata --target link_so -j8
cmake --build /tmp/w3693/build-fdata --target check -j8

# link-flag A/B: extract the exact gold line, substitute output, insert flags
ninja -C cmake-build-match -t commands libgof2hdaa.so | grep 'ld.gold -shared' | tail -1
```
