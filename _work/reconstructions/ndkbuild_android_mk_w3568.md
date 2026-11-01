# Working ndk-build Android.mk reproduction (w3568)

Status: research note, 2026-11-01. Worktree `wt/w3568`, branch `wave87-w3568`,
based on current master. Every number below is from a command run against
`../_work/bins/android_2.0.16_libgof2hdaa.so` or from a controlled rebuild of this tree.

## 0. Bottom line

The original native build is an **NDK r18b ndk-build** build. This note supplies a
working `Android.mk`/`Application.mk` for the current 203-source tree and measures it.

* A **plain ndk-build build** (r18b defaults, `APP_STL := c++_static`, no reconstruction
  aids) compiles and links the tree: **byte 1345 / linked 3843**.
* An **ndk-build match-mode link** (same ndk-build-compiled objects, plus the repo's
  existing `gen_exidx_markers.py`/`gen_section_order.py` aids and the two libgcc members
  the CMake link actually pulls) reproduces the current gate exactly:
  **byte 4496 / linked 4496**, every ELF section size/base-exact (except the known
  `.ARM.extab` −8 bytes that the CMake baseline also has). Its `.text`, `.rodata`,
  `.data`, `.data.rel.ro`, `.got`, `.ARM.exidx` and `.ARM.extab` are **byte-identical to
  the CMake baseline**; only `.dynsym`/`.dynstr`/reloc order and the build-id differ.

The reconstructed module shape is unchanged from the w3420 note:

```make
APP_ABI := armeabi-v7a
APP_PLATFORM := android-16
APP_STL := c++_static
APP_OPTIM := release

LOCAL_MODULE := gof2hdaa
LOCAL_CPP_FEATURES := rtti exceptions
LOCAL_ARM_NEON := true
LOCAL_LDLIBS := -llog -lGLESv2 -lGLESv1_CM -lEGL -landroid \
                -L<apk-libs> -lfmodex -lfmodevent -lm -ldl
LOCAL_WHOLE_STATIC_LIBRARIES := zlib libzip
LOCAL_STATIC_LIBRARIES := crypto
```

For the match-mode link two changes are required: `APP_STL := none` with explicit
`libc++abi.a`/`libandroid_support.a` (so ndk-build does not add `libc++_static.a` and
`libunwind.a`), and `-std=c++11` must be passed explicitly when `APP_STL := none`
(ndk-build only adds it for `c++_static`).

## 1. Baseline

Commands:

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
cmake --build cmake-build-match --target lint -j8
```

Measured:

```text
verify: byte 4496  linked 4496  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
=== DRIFT: 0 field(s) across 0 class(es) ===
CLEAN ✓
```

Baseline `.so` sha256: `2f4cc093cd350687f8eb27280a5493107fdc4fb1295337d23af6c7381755c03e`.
ELF sections (all size- and base-exact against the original; `.ARM.extab` is the known
−8-byte residue):

| section | address | size | original |
|---|---:|---:|---:|
| `.text` | `0x69440` | `0x1337c8` | `0x1337c8` |
| `.ARM.exidx` | `0x19cc08` | `0x6f70` | `0x6f70` |
| `.ARM.extab` | `0x1a3b78` | `0x26290` | `0x26298` |
| `.rodata` | `0x1c9e10` | `0x41b67` | `0x41b67` |
| `.data.rel.ro` | `0x20cbb0` | `0x2fe8` | `0x2fe8` |
| `.dynamic` | `0x20fba4` | `0x148` | `0x148` |
| `.got` | `0x20fcf4` | `0x430c` | `0x430c` |
| `.data` | `0x214000` | `0x296c` | `0x296c` |
| `.bss` | `0x216970` | `0x69ac` | `0x69ac` |

## 2. Compile-flag A/B at the current tree

### 2.1 The authentic r18b compile flags (minus `-fdata-sections`) are neutral

A scratch wrapper toolchain appended the r18b ndk-build compile flags after the CMake set:

```sh
cmake -S . -B /tmp/w3568/build-exp -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/tmp/w3568/toolchain_extra.cmake \
  -DGOF2_MATCH_OPT=-Oz \
  '-DGOF2_EXTRA_FLAGS=-DNDEBUG -fpic -fexceptions -no-canonical-prefixes \
   -Wno-invalid-command-line-argument -Wno-unused-command-line-argument \
   -gcc-toolchain <ndk>/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64 \
   -mfpu=vfpv3-d16 -mfpu=neon'
cmake --build /tmp/w3568/build-exp --target verify -j8
```

Result: **byte 4496, linked 4496**, `.text 0x1337c8`, `.rodata 0x41b67`,
`.data 0x296c`, `.bss 0x69ac`; only the build-id differs from the baseline. So
`-DNDEBUG`, `-fpic`, `-fexceptions`, `-no-canonical-prefixes`, the two `-Wno-*` flags,
`-gcc-toolchain`, and the `-mfpu=vfpv3-d16 -mfpu=neon` ordering are gate-neutral here.

### 2.2 `-fdata-sections` is authentic but still negative

```sh
cmake -S . -B /tmp/w3568/build-exp -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/tmp/w3568/toolchain_extra.cmake \
  -DGOF2_MATCH_OPT=-Oz -DGOF2_EXTRA_FLAGS=-fdata-sections
cmake --build /tmp/w3568/build-exp --target verify -j8
```

| build | byte | linked | `.text` | `.rodata` | `.data` |
|---|---:|---:|---:|---:|---:|
| baseline | **4496** | **4496** | `0x1337c8` | `0x41b67` | `0x296c` |
| `-fdata-sections` | **4333** | 4496 | `0x1337c8` | `0x41a07` | `0x2970` |

`-fdata-sections` is an r18b `GLOBAL_CFLAGS` default, but at this tree state it moves
`.rodata` by `-0x160`, `.data` by `+4`, and costs **163 byte_exact**. It is not landed.

### 2.3 `-std=c++11` is a real codegen flag when `APP_STL := none`

This is the one compile-flag difference that is easy to miss. With `APP_STL := c++_static`
ndk-build adds `-std=c++11` automatically; with `APP_STL := none` it does not, and clang 7
falls back to `gnu++14`. The difference is visible in `AEFile::collectFilesInPakFiles`:

```text
CMake / -std=c++11:
  %24 = add nsw i32 %23, 1
  %25 = icmp slt i8 %22, -1
  %26 = select i1 %25, i32 -1, i32 %24
  %27 = call i8* @_Znaj(i32 %26)

ndk-build / APP_STL=none, no -std=c++11:
  %24 = add nsw i32 %23, 1
  %25 = call i8* @_Znaj(i32 %24)
```

Measured object size of `AEFile::collectFilesInPakFiles`: **0x174** with `-std=c++11`,
**0x168** without. Adding `-std=c++11` to `LOCAL_CPPFLAGS` restores 0x174 and makes the
whole object match the CMake object. This is why the match-mode `Android.mk` below passes
`-std=c++11` explicitly.

## 3. Plain ndk-build build

The project is a scratch `jni/` directory with symlinks to `src/` and `third_party/`.
`LOCAL_SRC_FILES` is generated from the recovered link order
(`cmake-build-match/link_order.rsp`):

```sh
python3 - <<'PY'
import re
rsp = open('cmake-build-match/link_order.rsp').read().splitlines()
srcs = []
for line in rsp:
    m = re.search(r'gof2\.dir/\./(src/.*)\.obj$', line)
    assert m, line
    srcs.append(m.group(1))
open('/tmp/w3568/srcs.txt', 'w').write('\n'.join(srcs) + '\n')
PY
```

`Android.mk` (plain build; full source list is `/tmp/w3568/srcs.txt`):

```make
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
LOCAL_SRC_FILES := $(shell cat /tmp/w3568/srcs.txt)
LOCAL_CPP_FEATURES := rtti exceptions
LOCAL_ARM_NEON := true
LOCAL_C_INCLUDES := $(LOCAL_PATH)/src \
    $(LOCAL_PATH)/third_party/fmod/inc \
    $(LOCAL_PATH)/third_party/gl \
    $(LOCAL_PATH)/third_party/jni \
    $(LOCAL_PATH)/third_party/libzip \
    $(LOCAL_PATH)/third_party/crypto
LOCAL_CFLAGS := -Wno-int-to-pointer-cast -Wno-int-to-void-pointer-cast
LOCAL_LDLIBS := -llog -lGLESv2 -lGLESv1_CM -lEGL -landroid \
                -L/home/debian/GalaxyOnFire2/wt/w3568/../_work/bins/apk_libs \
                -lfmodex -lfmodevent -lm -ldl
LOCAL_WHOLE_STATIC_LIBRARIES := zlib libzip
LOCAL_STATIC_LIBRARIES := crypto
include $(BUILD_SHARED_LIBRARY)
```

`Application.mk`:

```make
APP_ABI := armeabi-v7a
APP_PLATFORM := android-16
APP_STL := c++_static
APP_OPTIM := release
```

Build and verify:

```sh
.cache/ndk/android-ndk-r18b/ndk-build \
  NDK_PROJECT_PATH=/tmp/w3568/ndkpure \
  APP_BUILD_SCRIPT=/tmp/w3568/ndkpure/jni/Android.mk \
  NDK_APPLICATION_MK=/tmp/w3568/ndkpure/jni/Application.mk -j8

python3 tools/verify/verify.py \
  --build-dir /tmp/w3568/ndkpure-verify \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3568/ndkpure/libs/armeabi-v7a/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
```

Result:

```text
compared 4524  avg 99.7%  byte_exact 1345  linked_exact 3843  fuzzy_perfect 3863
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
```

Sections: `.text 0x135f60`, `.ARM.exidx 0x7100`, `.ARM.extab 0x263f8`,
`.rodata 0x41e97`, `.data.rel.ro 0x3030`, `.data 0x2960`, `.bss 0x69b0`,
`.dynsym 0x15b80`, `.plt 0xab8c`. `DT_NEEDED` order is exact
(`liblog libGLESv2 libGLESv1_CM libEGL libandroid libfmodex libfmodevent libm libdl libc`).

The plain build is a valid ndk-build build of the tree, but it is not a gate match:
it has no `--section-ordering-file`, and ndk-build adds `libc++_static.a`/`libunwind.a`
and places `-lgcc` before the shared libraries, which pulls the `__aeabi_*`/libunwind
helpers into `.text`.

## 4. Match-mode ndk-build link

The match-mode link keeps ndk-build as the compiler and linker but supplies the same
reconstruction aids the CMake build uses. Steps:

1. Compile the 203 sources with the plain-build project, but with `APP_STL := none`,
   `LOCAL_CFLAGS += -fno-data-sections`, and `LOCAL_CPPFLAGS := -std=c++11 -stdlib=libc++`.
   The 203 objects in `ndkpure/obj/local/armeabi-v7a/objs/gof2hdaa/` are the result.
   (`libc++abi.a` and `libandroid_support.a` are link inputs, not compile inputs.)
2. Run the repo's `tools/gen_exidx_markers.py` on the ndk-build objects and the
   zlib/libzip/libcrypto/libc++abi/libandroid_support/libgcc archives. This renames each
   empty `.text` to a unique `.text.tuend.*` marker (the `.ARM.exidx` sentinels) and
   renames libcrypto's three plain `.text` sections.
3. Run `tools/gen_section_order.py` on the marker objects and marker archives. The
   resulting section-ordering file is **byte-identical** to
   `cmake-build-match/section_order.txt` (5885 names, 1361 local).
4. Archive the marker app objects in link order and link that archive whole.
5. Replace `-lgcc` with a two-member `libgcc.a` containing only the members the CMake
   link actually pulls: `__aeabi_ldivmod.o` (`0x38` `.text`) and `__dvmd_lnx.o`
   (`0xc` `.text`), total `0x44`. Put that archive in a `-L` directory; ndk-build's
   `-lgcc` resolves to it.
6. Rename `crtbegin_so.o`'s `.text` to `.text.crtbegin` and put it in a `-B` directory,
   so the driver's startfile is placed at the original's first `.text` slot.
7. Link with `--section-ordering-file`, `--unresolved-symbols=ignore-all`, and
   `LOCAL_ALLOW_UNDEFINED_SYMBOLS := true`.

`gen_exidx_markers.py` keys its drop-list on the CMake object basenames
(`Vector.cpp.obj`, ...), so first make a response file of symlinks with those names
pointing at the ndk-build objects:

```python
import os, re
rsp = open('cmake-build-match/link_order.rsp').read().splitlines()
outdir = '/tmp/w3568/ndkobj-links'
os.makedirs(outdir, exist_ok=True)
objs = []
for line in rsp:
    m = re.search(r'gof2\.dir/\./(src/.*)\.obj$', line)
    assert m, line
    src = m.group(1)
    base = os.path.splitext(src)[0]
    ndkobj = '/tmp/w3568/ndkpure/obj/local/armeabi-v7a/objs/gof2hdaa/' + base + '.o'
    link = os.path.join(outdir, os.path.basename(line))
    if os.path.lexists(link):
        os.unlink(link)
    os.symlink(ndkobj, link)
    objs.append(link)
open('/tmp/w3568/ndkpure-objs.rsp', 'w').write('\n'.join(objs) + '\n')
```

Commands (from the repo root; `NDK` is the r18b root):

```sh
REPO=/home/debian/GalaxyOnFire2/wt/w3568
SCRATCH=/tmp/w3568
NDK=$REPO/.cache/ndk/android-ndk-r18b
AR=$NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/llvm-ar
OBJCOPY=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objcopy
NM=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm

# 1. ndk-build compile of the 203 sources (APP_STL=none, -fno-data-sections,
#    -std=c++11); this is the plain-build project with those three changes.
$NDK/ndk-build NDK_PROJECT_PATH=$SCRATCH/ndkpure \
  APP_BUILD_SCRIPT=$SCRATCH/ndkpure/jni/Android.mk \
  NDK_APPLICATION_MK=$SCRATCH/ndkpure/jni/Application.mk -j8

# 2. marker copies of the ndk-build objects and archives
python3 tools/gen_exidx_markers.py $SCRATCH/ndk-marker $OBJCOPY $AR \
  $SCRATCH/ndkpure-objs.rsp \
  third_party/openssl/libcrypto_gof2.a \
  third_party/zlib/libz_gof2.a \
  third_party/libzip/libzip_gof2.a \
  $NDK/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libc++abi.a \
  $NDK/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libandroid_support.a \
  $NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/lib/gcc/arm-linux-androideabi/4.9.x/armv7-a/thumb/libgcc.a

# 3. section ordering file
python3 tools/gen_section_order.py ../_work/bins/android_2.0.16_libgof2hdaa.so \
  $NM $SCRATCH/ndk-marker/section_order.txt \
  @$SCRATCH/ndk-marker/link_order.rsp @$SCRATCH/ndk-marker/archives.rsp

# 4. archive the marker app objects in link order
$AR rcs $SCRATCH/ndk-marker-objs.a $(cat $SCRATCH/ndk-marker/link_order.rsp)

# 5. two-member libgcc
mkdir -p $SCRATCH/ndk-minigcc
(cd $SCRATCH/ndk-minigcc && \
  $AR x $SCRATCH/ndk-marker/archives/libgcc.a \
      m039__aeabi_ldivmod.o.marked m041__dvmd_lnx.o.marked && \
  $AR rcs libgcc.a m039__aeabi_ldivmod.o.marked m041__dvmd_lnx.o.marked)

# 6. renamed crtbegin
mkdir -p $SCRATCH/crt
cp $NDK/platforms/android-16/arch-arm/usr/lib/crtbegin_so.o $SCRATCH/crt/crtbegin_so.o
cp $NDK/platforms/android-16/arch-arm/usr/lib/crtend_so.o $SCRATCH/crt/crtend_so.o
$OBJCOPY --rename-section .text=.text.crtbegin $SCRATCH/crt/crtbegin_so.o

# 7. link
$NDK/ndk-build NDK_PROJECT_PATH=$SCRATCH/ndkfullmatch \
  APP_BUILD_SCRIPT=$SCRATCH/ndkfullmatch/jni/Android.mk \
  NDK_APPLICATION_MK=$SCRATCH/ndkfullmatch/jni/Application.mk -j8
```

Result:

```text
compared 4524  avg 99.9%  byte_exact 4496  linked_exact 4496  fuzzy_perfect 4496
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
```

Sections (ndk-build match vs original vs CMake baseline):

| section | original | CMake baseline | ndk-build match |
|---|---:|---:|---:|
| `.text` | `0x1337c8` @ `0x69440` | same | same |
| `.ARM.exidx` | `0x6f70` @ `0x19cc08` | same | same |
| `.ARM.extab` | `0x26298` @ `0x1a3b78` | `0x26290` @ `0x1a3b78` | `0x26290` @ `0x1a3b78` |
| `.rodata` | `0x41b67` @ `0x1c9e10` | same | same |
| `.data.rel.ro` | `0x2fe8` @ `0x20cbb0` | same | same |
| `.dynamic` | `0x148` @ `0x20fba4` | same | same |
| `.got` | `0x430c` @ `0x20fcf4` | same | same |
| `.data` | `0x296c` @ `0x214000` | same | same |
| `.bss` | `0x69ac` @ `0x216970` | same | same |

Byte-level comparison of the match-mode `.so` against the CMake baseline and the original
(differing bytes per section):

| section | ndk-vs-original | ndk-vs-CMake |
|---|---:|---:|
| `.text` | 119732 | **0** |
| `.rodata` | **0** | **0** |
| `.data` | **0** | **0** |
| `.data.rel.ro` | **0** | **0** |
| `.got` | **0** | **0** |
| `.ARM.exidx` | 2710 | **0** |
| `.ARM.extab` | 119161 | **0** |
| `.dynsym` | 3294 | 2125 |
| `.dynstr` | 21751 | 18748 |
| `.rel.dyn` | 156 | 100 |
| `.rel.plt` | 371 | 307 |

The `.text` difference against the original is the 28 non-linked functions (the gate's
4496 byte-exact functions are exactly the 4496 linked-exact functions). The important
result is the middle column: the ndk-build link is **byte-identical to the CMake baseline
for every code/data/EH section**. Only the dynamic-symbol/relocation order and the
build-id differ, and the gate counts are identical.

If the system/FMOD shared libraries are placed in the link before `libcrypto` (as
`PREBUILT_STATIC_LIBRARY` modules whose `LOCAL_SRC_FILES` are `.a` symlinks to the `.so`
files), the `.dynsym`/`.dynstr`/reloc differences against the CMake baseline shrink to
2125 / 18748 / 100 / 307 bytes (from 22308 / 18748 / ... in the first match-mode link);
the gate stays byte 4496 / linked 4496. This is optional and does not affect the gate.

## 5. Why each piece is needed

| piece | evidence |
|---|---|
| `-fno-data-sections` | `-fdata-sections` costs 163 byte (4496 → 4333), `.rodata -0x160`, `.data +4`. |
| `-std=c++11` with `APP_STL := none` | without it clang 7 uses gnu++14; `AEFile::collectFilesInPakFiles` is 0x168 instead of 0x174. |
| `APP_STL := none` + explicit `libc++abi.a`/`libandroid_support.a` | avoids ndk-build's `libc++_static.a` and `libunwind.a`; the undefined-import count becomes 207 (original/CMake) instead of 189. |
| `--section-ordering-file` + `gen_exidx_markers.py` | without markers `.ARM.exidx` is `0x6690`; with markers it is exactly `0x6f70`. The generated order file is byte-identical to the CMake one. |
| renamed `crtbegin_so.o` (`-B`) | crtbegin's `0x44`-byte `.text` must sit at the first `.text` slot; otherwise every app function is `0x44` early. |
| two-member `libgcc.a` | ndk-build puts `-lgcc` before the shared libraries, so the full `libgcc.a` pulls `__aeabi_*`/libunwind code (`+0x2658` `.text`). CMake's link order lets the shared libraries satisfy those symbols and pulls only `__aeabi_ldivmod.o` + `__dvmd_lnx.o` (`0x44`). |
| `--unresolved-symbols=ignore-all` + `LOCAL_ALLOW_UNDEFINED_SYMBOLS := true` | the original leaves `_Unwind_*`/`__aeabi_*` undefined (207 imports); ndk-build's default `--no-undefined` resolves them. |

## 6. Proven vs still hypothesis

**Proven by commands in this note**

* The current tree's CMake baseline is byte 4496 / linked 4496, size-exact, lint CLEAN,
  drift 0, `.text 0x1337c8`.
* The authentic r18b ndk-build compile flags (minus `-fdata-sections`) are gate-neutral:
  byte 4496 / linked 4496 with every code/data section byte-identical.
* `-fdata-sections` is authentic but currently costs 163 byte_exact.
* `APP_STL := none` makes ndk-build omit `-std=c++11`; clang 7 then uses gnu++14 and
  `AEFile::collectFilesInPakFiles` shrinks from 0x174 to 0x168. Passing `-std=c++11`
  restores it.
* A plain ndk-build build of the current 203-source tree compiles and links
  (byte 1345 / linked 3843).
* The match-mode ndk-build pipeline (same ndk-build-compiled objects + the repo's marker
  and section-order aids + the two-member libgcc) reproduces the gate exactly:
  byte 4496 / linked 4496, and its `.text`/`.rodata`/`.data`/`.data.rel.ro`/`.got`/
  `.ARM.exidx`/`.ARM.extab` are byte-identical to the CMake baseline.
* `DT_NEEDED` order is exact in both the plain and match-mode ndk-build links.

**Hypothesis / limits**

* The exact original `Android.mk`/`Application.mk` text is not in the APK or the source
  dumps. The module block is a reconstruction.
* The exact original `LOCAL_SRC_FILES` list cannot be recovered verbatim because the
  original TU split differs from this tree. The list used here is the recovered link
  order from `cmake-build-match/link_order.rsp`.
* `APP_PLATFORM := android-16` is indistinguishable from omitting it (r18b's default).
  `APP_OPTIM := release` is indistinguishable from omitting it (release is default).
* FMOD in `LOCAL_LDLIBS` vs `LOCAL_SHARED_LIBRARIES` cannot be distinguished from the
  binary; both produce the same `DT_NEEDED` order.
* The remaining `.dynsym`/`.dynstr`/reloc-order difference between the ndk-build match
  and the CMake baseline is an input-order effect (archive-vs-individual app objects,
  the dummy object, and the shared-library position). It does not affect the gate.
* The vendored zlib/libzip/libcrypto archives are built by this repo's
  `third_party/build_match_lib.cmake`, not by ndk-build. The original's archive build
  recipe is still not recovered.

## Appendix A — plain-build `Android.mk`

The full `LOCAL_SRC_FILES` list is the 203 paths in `cmake-build-match/link_order.rsp`,
converted by the Python snippet in §3. The rest of the file is the plain-build
`Android.mk` shown in §3.

## Appendix B — match-mode `Android.mk`

This is the exact **link project** used for the 4496/4496 result (it links the marker
archive; it does not compile the 203 sources). It expects the marker directory
`$SCRATCH/ndk-marker`, the marker archive `$SCRATCH/ndk-marker-objs.a`, the minigcc
directory `$SCRATCH/ndk-minigcc`, and the renamed crtbegin directory `$SCRATCH/crt`
from §4.

```make
LOCAL_PATH := $(call my-dir)
NDKROOT := /home/debian/GalaxyOnFire2/gof2-decomp/.cache/ndk/android-ndk-r18b
M := /tmp/w3568/ndk-marker

include $(CLEAR_VARS)
LOCAL_MODULE := gof2objs
LOCAL_SRC_FILES := /tmp/w3568/ndk-marker-objs.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := zlib
LOCAL_SRC_FILES := $(M)/archives/libz_gof2.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libzip
LOCAL_SRC_FILES := $(M)/archives/libzip_gof2.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := crypto
LOCAL_SRC_FILES := $(M)/archives/libcrypto_gof2.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := cxxabi
LOCAL_SRC_FILES := $(M)/archives/libc++abi.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := android_support
LOCAL_SRC_FILES := $(M)/archives/libandroid_support.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := gof2hdaa
LOCAL_SRC_FILES := dummy.c
LOCAL_CPP_FEATURES := rtti exceptions
LOCAL_ARM_NEON := true
LOCAL_C_INCLUDES := $(LOCAL_PATH)/src \
    $(LOCAL_PATH)/third_party/fmod/inc \
    $(LOCAL_PATH)/third_party/gl \
    $(LOCAL_PATH)/third_party/jni \
    $(LOCAL_PATH)/third_party/libzip \
    $(LOCAL_PATH)/third_party/crypto \
    $(NDKROOT)/sources/cxx-stl/llvm-libc++/include \
    $(NDKROOT)/sources/cxx-stl/llvm-libc++abi/include \
    $(NDKROOT)/sources/android/support/include
LOCAL_CFLAGS := -Wno-int-to-pointer-cast -Wno-int-to-void-pointer-cast -fno-data-sections
LOCAL_CPPFLAGS := -std=c++11 -stdlib=libc++
LOCAL_LDFLAGS := -B/tmp/w3568/crt -L/tmp/w3568/ndk-minigcc \
    -Wl,--section-ordering-file=$(M)/section_order.txt \
    -Wl,--unresolved-symbols=ignore-all
LOCAL_ALLOW_UNDEFINED_SYMBOLS := true
LOCAL_LDLIBS := -llog -lGLESv2 -lGLESv1_CM -lEGL -landroid \
                -L/home/debian/GalaxyOnFire2/wt/w3568/../_work/bins/apk_libs \
                -lfmodex -lfmodevent -lm -ldl
LOCAL_WHOLE_STATIC_LIBRARIES := gof2objs zlib libzip
LOCAL_STATIC_LIBRARIES := crypto cxxabi android_support
include $(BUILD_SHARED_LIBRARY)
```

`Application.mk`:

```make
APP_ABI := armeabi-v7a
APP_PLATFORM := android-16
APP_STL := none
APP_OPTIM := release
```

`dummy.c` is an empty C file. It contributes no code; its only purpose is to satisfy
ndk-build's requirement that a module have at least one source file.

## Appendix C — measured progression

| build | byte | linked | `.text` | `.rodata` | `.data` | `.bss` | `.ARM.exidx` |
|---|---:|---:|---:|---:|---:|---:|---:|
| CMake baseline | **4496** | **4496** | `0x1337c8` | `0x41b67` | `0x296c` | `0x69ac` | `0x6f70` |
| CMake + `-fdata-sections` | 4333 | 4496 | `0x1337c8` | `0x41a07` | `0x2970` | `0x69ac` | — |
| CMake + full ndk flags (no fdata) | **4496** | **4496** | `0x1337c8` | `0x41b67` | `0x296c` | `0x69ac` | `0x6f70` |
| plain ndk-build (`c++_static`, default fdata) | 1345 | 3843 | `0x135f60` | `0x41e97` | `0x2960` | `0x69b0` | `0x7100` |
| ndk-build + `-fno-data-sections` | 1345 | 3843 | `0x135f60` | `0x41ff7` | `0x2970` | `0x69b0` | `0x7100` |
| + section order | 1345 | 3843 | `0x135f48` | `0x41ff7` | `0x2970` | `0x69b0` | `0x6828` |
| + `APP_STL=none` + explicit cxxabi/android_support | 1345 | 4476 | `0x135e20` | `0x41b67` | `0x296c` | `0x69ac` | `0x67b8` |
| + renamed crtbegin | 1345 | 4476 | `0x135e20` | `0x41b67` | `0x296c` | `0x69ac` | `0x67c0` |
| + empty libgcc | 1345 | 4476 | `0x13370c` | `0x41b67` | `0x296c` | `0x69ac` | `0x6690` |
| + marker objects + two-member libgcc | **4496** | **4496** | `0x1337c8` | `0x41b67` | `0x296c` | `0x69ac` | `0x6f70` |
