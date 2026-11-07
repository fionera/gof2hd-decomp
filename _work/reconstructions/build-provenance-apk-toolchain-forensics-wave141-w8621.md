# Build provenance: APK / toolchain / sibling-binary forensics (wave 141, w8621)

Status: **note-only research commit**, 2026-11-07. Worktree
`/home/debian/GalaxyOnFire2/wt/w8621`, base `e18e0c2a4` ("Wave 140 docs: .text
86,472, .ARM.extab 1,411, semantic wins"). Scratch: `/dev/shm/w8621`. No
`src/`, `tools/`, `CMakeLists.txt`, baseline, allowlist or doc change is landed.
This note is the only commit.

Gate at the base (fresh `cmake --build cmake-build-match --target check -j6`):

```text
verify: byte 4507  linked 4507  stubs 0  missing 0  extra 0  avg 99.93
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Section deltas (fresh measurement of the canonical build):

| section | size | differing bytes |
|---|---:|---:|
| `.text` | 1,259,464 | **86,472** |
| `.ARM.extab` | 156,312 | **1,411** |
| `.ARM.exidx` | 28,528 | **0** |
| `.note.gnu.build-id` | 36 | **20** |
| `.rodata`, `.data`, `.bss`, `.rel.plt`, `.dynsym`, `.dynstr`, `.hash`, `.got` | exact | **0** |

Original: `_work/bins/android_2.0.16_libgof2hdaa.so`, 2,187,328 B,
sha256 `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`.
Our canonical build: 2,187,328 B,
sha256 `68430eb30502ac92ad6fa68389fd8b81de44dbef292bba62aaeae4415c625844`.

---

## 0. Bottom line

1. **The original is exactly NDK r18b (18.1.5063045), clang 7.0.2 build
   4751641 / r328903, GNU gold 1.12 (binutils 2.27.0.20170315), API 16,
   armeabi-v7a Thumb-2 NEON softfp, libc++ static.** The `.comment`,
   `.note.android.ident`, `.note.gnu.gold-version` and `.ARM.attributes`
   sections are **byte-identical** to our canonical build (same SHA-256 for
   all four sections), and the NDK r18b package's own `source.properties`,
   `ndk-version.h`, `AndroidVersion.txt`, `CHANGELOG.md`, `clang --version`,
   `ld.gold --version` and `strip --version` pin every string.
2. **The original was compiled with `-g2`-level full DWARF, then stripped.**
   The shipped file's own SHA-1 is not its build-id, so the build-id was
   computed over an unstripped link output. The shipped `.dynsym` sizes of
   all 40 functions whose size changes between `-g2` and `-g0` match the
   `-g2` sizes (40/40), not the `-g0`/`-gline-tables-only` sizes.
3. **The build system is most likely the NDK r18b CMake toolchain, not
   default ndk-build.** The r18b ndk-build `GLOBAL_CFLAGS` includes
   `-fdata-sections`; the r18b CMake toolchain does not. A fresh
   `-fdata-sections` A/B drops `byte_exact` 4507 -> **4315**, shrinks
   `.rodata` 269,159 -> 268,807 and `.data` 10,604 -> 10,588, and shifts
   `.bss` by 16 bytes. The original matches the no-`-fdata-sections` build
   exactly. The rest of the original's compile line is exactly the r18b
   CMake toolchain Release defaults for `armeabi-v7a` + `c++_static`
   (`-g -DANDROID -ffunction-sections -funwind-tables
   -fstack-protector-strong -Oz -DNDEBUG -march=armv7-a -mfloat-abi=softfp
   -mfpu=neon -std=c++11 -fPIC -frtti -fexceptions`).
4. **No app build path survives.** The only path-like string in the shipped
   `.so` is the NDK libc++abi buildbot path
   `/Volumes/Android/buildbot/src/android/ndk-release-r18/external/libcxx/../../external/libcxxabi/src/abort_message.cpp`,
   which is identical in our build. `DW_AT_comp_dir`, `DW_AT_name`,
   `-fdebug-prefix-map` and the original build directory were all in the
   stripped `.debug_*`/`.symtab` and are unrecoverable.
5. **The APK contains four native libraries from at least three different
   toolchains.** Only `libgof2hdaa.so` is r18b/clang 7.0.2. `libcrashlytics.so`
   is a GCC 4.9 / gold 1.11 (r13b/r14-era) build; `libfmodex.so` and
   `libfmodevent.so` are prebuilt GCC 4.4.3/4.6 ARMv5TE objects. The Unity
   Remake reference `libgof2hdaa.so` is a different NDK r14 / clang 3.8 /
   gold 1.11 / gnustl build of the same game.
6. **No build flag improves the gate.** `-fpic` is byte-identical to `-fPIC`
   on this tree; `--fix-cortex-a8` is byte-identical on the unstripped link;
   `-fdata-sections` is a regression. The 17 residual functions remain
   source-shape, not build configuration.

---

## 1. APK inventory and per-library toolchain fingerprints

`Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk` (6,389,308 B,
sha256 `9cfe4484a475c9a31a336df9dea6408fbb129d88aa19083afe808ef6bca9eb19`)
contains exactly four `.so` files, all in `lib/armeabi-v7a/`:

| library | size | sha256 | compiler / linker | ISA / FP | notes |
|---|---:|---|---|---|---|
| `libgof2hdaa.so` | 2,187,328 | `6d38fd76…e3b58` (APK copy) | clang 7.0.2 r328903 / gold 1.12 | ARMv7-A Thumb-2 VFPv3+NEONv1 softfp | `.note.android.ident` r18b / 5063045 / API 16; 27 sections |
| `libcrashlytics.so` | 202,580 | `afbe76e6…c604b` | GCC 4.9.x 20150123 / gold 1.11 | ARMv7-A Thumb-2 VFPv3-D16, no NEON | `Tag_ABI_optimization_goals: Aggressive Speed`; `DT_FLAGS=SYMBOLIC|BIND_NOW`; no `.note.android.ident` |
| `libfmodex.so` | 920,204 | `60ec4de0…c3fa81` | GCC 4.4.3 / GCC 4.6 | ARMv5TE Thumb-1 VFPv1 | no build-id, no gold note; prebuilt third-party |
| `libfmodevent.so` | 370,660 | `b92cd6ff…9c97d` | GCC 4.4.3 | ARMv5TE, no Thumb flag, VFPv2 | no build-id, no gold note; prebuilt third-party |

The ground-truth `_work/bins/android_2.0.16_libgof2hdaa.so` differs from the
APK copy by exactly 288 bytes (the raw-bfd `.shstrtab`/`sh_name` difference;
the ground truth is the same library after `tools/reorder_shstrtab.py`).

### 1.1 `libgof2hdaa.so`: exact toolchain strings

`.comment` is 303 bytes, two NUL-terminated strings:

```text
offset 0x000: ""
offset 0x001 (262 B):
Android (4751641 based on r328903) clang version 7.0.2
(https://android.googlesource.com/toolchain/clang 003100370607242ddd5815e4a043907ea9004281)
(https://android.googlesource.com/toolchain/llvm 1d739ffb0366421d383e04ff80ec2ee591315116)
(based on LLVM 7.0.2svn)
offset 0x108 (38 B):
GCC: (GNU) 4.9.x 20150123 (prerelease)
```

The local NDK r18b `clang --version` prints the same string verbatim. The
local `arm-linux-androideabi-gcc --version` prints
`arm-linux-androideabi-gcc (GCC) 4.9.x 20150123 (prerelease)`. The r18b
`CHANGELOG.md` says "Updated Clang to build 4751641, based on r328903";
`toolchains/llvm/prebuilt/linux-x86_64/AndroidVersion.txt` says
`7.0.2 / based on r328903`.

`.note.android.ident` (152 B) is:

```text
0000: 08 00 00 00 84 00 00 00 01 00 00 00 41 6e 64 72  ............Andr
0010: 6f 69 64 00 10 00 00 00 72 31 38 62 00 00 00 00  oid.....r18b....
...
0050: 00 00 00 00 00 00 00 00 35 30 36 33 30 34 35 00  ........5063045.
```

i.e. API level `0x10` = 16, NDK name `r18b`, build number `5063045`.
The r18b `source.properties` is `Pkg.Revision = 18.1.5063045`, and
`sysroot/usr/include/android/ndk-version.h` has
`__NDK_MAJOR__ 18`, `__NDK_MINOR__ 1`, `__NDK_BUILD__ 5063045`.

`.note.gnu.gold-version` (28 B) is:

```text
0000: 04 00 00 00 09 00 00 00 04 00 00 00 47 4e 55 00  ............GNU.
0010: 67 6f 6c 64 20 31 2e 31 32 00 00 00              gold 1.12...
```

The r18b `arm-linux-androideabi-ld.gold --version` is
`GNU gold (GNU Binutils 2.27.0.20170315) 1.12`; the matching
`arm-linux-androideabi-strip --version` is
`GNU strip (GNU Binutils) 2.27.0.20170315`. The note alone says "gold 1.12";
the r18b package pins the exact binutils revision.

`.ARM.attributes` (56 B) decodes to:

```text
Tag_CPU_name: "ARM v7"          Tag_CPU_arch: v7
Tag_CPU_arch_profile: Application
Tag_ARM_ISA_use: Yes            Tag_THUMB_ISA_use: Thumb-2
Tag_FP_arch: VFPv3              Tag_Advanced_SIMD_arch: NEONv1
Tag_ABI_PCS_GOT_use: GOT-indirect
Tag_ABI_PCS_wchar_t: 4          Tag_ABI_FP_denormal: Needed
Tag_ABI_FP_exceptions: Needed   Tag_ABI_FP_number_model: IEEE 754
Tag_ABI_align_needed: 8-byte    Tag_ABI_enum_size: int
Tag_ABI_HardFP_use: Deprecated  Tag_CPU_unaligned_access: v6
Tag_ABI_FP_16bit_format: IEEE 754
```

There is **no** `Tag_ABI_optimization_goals` (clang 7 does not emit it) and
**no** `Tag_Virtualization_use` (clang 3.8 does). The GCC-built
`libcrashlytics.so` has `Tag_ABI_optimization_goals: Aggressive Speed`
(`-O2`); the r14 reference game lib has both `Aggressive Speed` and
`TrustZone`.

### 1.2 Byte-identical toolchain fingerprint sections

The four metadata sections are byte-identical between the original and our
canonical build:

| section | size | SHA-256 (original = ours) |
|---|---:|---|
| `.note.android.ident` | 152 | `522bf4171215935bd062d824e7c00f3cbc0b5312942192199f3747e6cc11115b` |
| `.note.gnu.gold-version` | 28 | `47f758e9dbe910453f5b562efab608e6b2289e178fa76940332cfcf1ca1fd40e` |
| `.comment` | 303 | `d4f577f46aa5f1329f9898849122325b00c135130f431950d9600f5cb73c7fee` |
| `.ARM.attributes` | 56 | `b03afea6aa56c9ca191d5385abb498e64c7ecdf8e249a80e68704050256b1bc2` |

This is the strongest possible toolchain fingerprint: the exact compiler,
linker and assembler version strings and the exact ARM build attributes are
reproduced byte for byte.

### 1.3 Dynamic metadata

| observable | original | ours |
|---|---|---|
| `DT_SONAME` | `libgof2hdaa.so` | same |
| `DT_NEEDED` order | `liblog`, `libGLESv2`, `libGLESv1_CM`, `libEGL`, `libandroid`, `libfmodex`, `libfmodevent`, `libm`, `libdl`, `libc` | same |
| `DT_FLAGS` | `BIND_NOW` (`0x8`) | same |
| `DT_FLAGS_1` | `NOW` (`0x1`) | same |
| `DT_RELCOUNT` | 1162 | same |
| `PT_GNU_RELRO` | `0x20cba0`, size `0x7460`, align `0x10` | same |
| `PT_GNU_STACK` | `RW`, align `0x10` | same |
| `.gnu.version_d` | 1 BASE entry `libgof2hdaa.so` | same |
| `.gnu.version_r` | `LIBC` v2 from `libc.so`, `LIBC` v3 from `libm.so` | same |
| `.text` sh_addralign | 32 | same |
| `.rodata` / `.data` / `.bss` sh_addralign | 16 / 16 / 16 | same |

The `DT_NEEDED` order is exactly a hand-written `-l` list order
(`-llog -lGLESv2 -lGLESv1_CM -lEGL -landroid -lfmodex -lfmodevent -lm -ldl
-lc`), which is why the original can resolve `__aeabi_uldivmod` from
`libfmodex.so` at run time.

---

## 2. Sibling native libraries in the APK

### 2.1 `libcrashlytics.so` — GCC / gold 1.11, not the game toolchain

| observable | value |
|---|---|
| `.comment` | `GCC: (GNU) 4.9.x 20150123 (prerelease)` only |
| `.note.gnu.gold-version` | `gold 1.11` |
| `.note.android.ident` | absent |
| `.ARM.attributes` | ARMv7-A, Thumb-2, `VFPv3-D16`, **no NEON**, `Tag_ABI_optimization_goals: Aggressive Speed` |
| `DT_FLAGS` | `SYMBOLIC | BIND_NOW` (`0xa`) |
| sections | 28 (incl. `.data.rel.ro.local` and `.data.rel.ro`) |
| `__stack_chk_fail` call sites | 82 |
| `__aeabi_*` imports/defs | none exported; GCC/libgcc statically linked |
| build-id | `e3c978ad09d1fab2d6c86de96df95c6442868a1a` |

The local NDK r13b `arm-linux-androideabi-ld.gold --version` is
`GNU gold (binutils-2.25-0666073 2.25.51.20141117) 1.11`, and its
`arm-linux-androideabi-gcc --version` is the same GCC 4.9.x 20150123
string. So `libcrashlytics.so` is an NDK **r13b/r14-era GCC build**, not
the r18b clang build. Its `Tag_ABI_optimization_goals: Aggressive Speed`
means `-O2`, not `-Oz`.

### 2.2 `libfmodex.so` / `libfmodevent.so` — prebuilt third-party

Both are GCC 4.4.3 (libfmodex also has a GCC 4.6.20120106 object), ARMv5TE
(`Tag_CPU_name: "5TE"`, `Tag_CPU_arch: v5TE`), VFPv1/VFPv2, no NEON, no
build-id, no gold-version note. They export a large `__aeabi_*` helper set
(including `__aeabi_uldivmod`) and are the reason the game lib's
`__aeabi_uldivmod` import resolves at run time. They are not built by the
same NDK and are not codegen oracles for the game.

### 2.3 Unity Remake reference `libgof2hdaa.so` — NDK r14 / clang 3.8

`_work/oracles/Galaxy-on-Fire-2-Unity-Remake/Reference/binaries/libgof2hdaa.so`
is a Git-LFS pointer in the checkout; fetching the LFS object gives a
2,469,656-byte stripped library
(sha256 `125dfdb218470b76d5c41ee2eb00b55ce379963df8869ba5c6bece0fd040a50e`).
It is a different Android build of the same game:

| observable | r14 reference | original r18b |
|---|---|---|
| `.comment` | `Android clang version 3.8.275480 (based on LLVM 3.8.275480)` + GCC 4.9.x | clang 7.0.2 r328903 + GCC 4.9.x |
| linker | gold 1.11 | gold 1.12 |
| `.note.android.ident` | absent | present, r18b / 5063045 / API 16 |
| sections | 26 | 27 |
| `DT_NEEDED` | includes `libstdc++.so` (gnustl shared) | no libstdc++/libc++_shared (libc++ static) |
| `.ARM.attributes` | `Aggressive Speed`, `TrustZone`, VFPv3+NEONv1 | no optimization/TrustZone tags, VFPv3+NEONv1 |
| `.text` | 1,476,296 B | 1,259,464 B |
| `.rodata` | 272,340 B | 269,159 B |
| build-id | `1dbb7d451b7ca0ff0cfccf2f2d2fae0e8f9c5a1c` | `c011810da45777b83ed721b2a87d517efed0d310` |
| `__aeabi_ldivmod` | **defined** at `0x1c80a5` (exported) | not exported |
| `__aeabi_idiv0` / `__aeabi_ldiv0` | weak defined at `0x1c80dd` | not exported |
| `__aeabi_memset` / `__aeabi_memset8` | imported | not imported |
| `__aeabi_memmove4` | not imported | imported |
| `__stack_chk_fail` call sites | 706 | 787 |

The r14 library is a useful **contrast**, not a byte oracle: it confirms the
same source family and the same NEEDED order, but its codegen and link
visibility differ. In particular, the r14 link did **not** hide libgcc
symbols (`__aeabi_ldivmod` is exported), while the r18b original did
(`--exclude-libs=libgcc.a`), which is one reason the r18b `.dynsym` has
5,370 symbols and no exported libgcc helper.

---

## 3. Codegen fingerprints

### 3.1 Stack-canary density

Counted as `bl <__stack_chk_fail@plt>` call sites in the disassembly:

| library | `.text` size | call sites | per KiB |
|---|---:|---:|---:|
| original r18b | 1,259,464 | **787** | 0.640 |
| our canonical build | 1,259,464 | **787** | 0.640 |
| r14 reference | 1,476,296 | 706 | 0.489 |
| `libcrashlytics.so` | 140,964 | 82 | 0.595 |

The original and our build match exactly. The density difference between
the two game builds is source/version, not a different protector mode: both
use `-fstack-protector-strong`.

### 3.2 `__aeabi_*` helper imports

The original's undefined `__aeabi_*` set is exactly:

```text
__aeabi_f2lz __aeabi_f2ulz __aeabi_idiv __aeabi_idivmod __aeabi_l2d
__aeabi_l2f __aeabi_memclr __aeabi_memclr4 __aeabi_memclr8 __aeabi_memcpy
__aeabi_memcpy4 __aeabi_memcpy8 __aeabi_memmove __aeabi_memmove4
__aeabi_memset4 __aeabi_uidiv __aeabi_uidivmod __aeabi_ul2f
__aeabi_uldivmod __aeabi_unwind_cpp_pr0 __aeabi_unwind_cpp_pr1
```

Our canonical build has the **same 21 names**. The r14 reference differs:
it imports `__aeabi_memset` and `__aeabi_memset8` in addition, does not
import `__aeabi_memmove4`, and exports `__aeabi_ldivmod` plus weak
`__aeabi_idiv0`/`__aeabi_ldiv0`. This is a clang 3.8 vs clang 7.0.2 codegen
and libgcc-visibility fingerprint; the r18b original matches our build.

### 3.3 Literal-pool alignment

Parsing every Thumb `ldr`/`ldr.w` with `[pc, #imm]` and computing
`target = Align(addr + 4, 4) + imm`:

| library | PC-relative literal loads | target `& 15` distribution | `.text` sh_addralign |
|---|---:|---|---:|
| original r18b | 14,579 | `{0:3709, 4:3659, 8:3653, 12:3558}` | 32 |
| our canonical | 14,582 | `{0:3705, 4:3664, 8:3668, 12:3545}` | 32 |
| r14 reference | 19,004 | `{0:4789, 4:4742, 8:4755, 12:4718}` | 32 |

Every literal target is 4-byte aligned; the distribution is uniform modulo
16. The original does **not** 8- or 16-byte-align literal pools, and the
`.text` section alignment of 32 is a section-header property, not a pool
property. Our build matches the original's pool alignment.

---

## 4. Build-system inference: NDK r18b CMake toolchain, not default ndk-build

### 4.1 The two r18b default flag sets

The NDK r18b package contains both build systems:

**ndk-build** (`build/core/default-build-commands.mk` and
`build/core/toolchains/arm-linux-androideabi-clang/setup.mk`):

```text
GLOBAL_CFLAGS = -target armv7-none-linux-androideabi16
                -fdata-sections -ffunction-sections
                -fstack-protector-strong -funwind-tables
                -no-canonical-prefixes
GLOBAL_CFLAGS += -g
TARGET_CFLAGS = -gcc-toolchain ... -fpic -march=armv7-a
                -mfloat-abi=softfp -mfpu=vfpv3-d16
TARGET_thumb_release_CFLAGS = -mthumb -Oz -DNDEBUG
TARGET_CFLAGS.neon = -mfpu=neon
```

**CMake toolchain** (`build/cmake/android.toolchain.cmake`):

```text
-g -DANDROID -ffunction-sections -funwind-tables
-fstack-protector-strong -no-canonical-prefixes
-Oz -DNDEBUG
-march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16
-mfpu=neon            (when ANDROID_ARM_NEON=ON)
-fPIC                 (CMAKE_POSITION_INDEPENDENT_CODE)
-frtti -fexceptions   (ANDROID_CPP_FEATURES)
-std=c++11            (ANDROID_STL=c++_static)
```

The only compile-flag difference that reaches the output is
**`-fdata-sections`**: ndk-build has it, the CMake toolchain does not.

### 4.2 `-fdata-sections` A/B (new)

Scratch build `/dev/shm/w8621/fdata` with the canonical toolchain plus
`-fdata-sections`, then the official verifier:

```text
compared 4524  avg 99.93%  byte_exact 4315  linked_exact 4507
stub_zero_size 0  missing 0  wrong_type 0  extra 0
```

Section comparison against the original:

| section | original | canonical (no `-fdata-sections`) | `-fdata-sections` |
|---|---:|---:|---:|
| `.text` size | 1,259,464 | 1,259,464 | 1,259,464 |
| `.text` differing bytes | — | **86,472** | **87,255** |
| `.rodata` size | 269,159 (`0x41b67`) | 269,159 | 268,807 (`0x41a07`) |
| `.data` size | 10,604 (`0x296c`) | 10,604 | 10,588 (`0x295c`) |
| `.bss` address | `0x216970` | `0x216970` | `0x216960` |
| `.ARM.exidx` differing bytes | — | 0 | 0 |
| `.ARM.extab` differing bytes | — | 1,411 | 1,411 |

The original's `.rodata`, `.data` and `.bss` sizes/addresses match the
**no-`-fdata-sections`** build exactly. Default ndk-build would therefore
not reproduce the original's data layout. This is the first concrete
discriminator between the two r18b build systems.

The remaining compile flags are identical between the two systems, so the
original is consistent with the **NDK r18b CMake toolchain** Release build
for `ANDROID_ABI=armeabi-v7a`, `ANDROID_PLATFORM=android-16`,
`ANDROID_STL=c++_static`, `ANDROID_ARM_NEON=ON`,
`ANDROID_CPP_FEATURES="rtti exceptions"`. (ndk-build with an explicit
`-fno-data-sections` is a less likely alternative; a hand-written build
script that copies the CMake defaults is also possible.)

### 4.3 Neutral build-system probes

| probe | result |
|---|---|
| `-fpic` instead of `-fPIC` | `byte_exact 4507`; every section byte-identical to canonical (`.text`/`.rodata`/`.data`/`.bss`/`.ARM.exidx`/`.ARM.extab`/`.rel.plt`/`.dynamic`/`.got`/`.data.rel.ro` diff 0). `-fPIC` vs `-fpic` cannot distinguish CMake from ndk-build. |
| `--fix-cortex-a8` on the gold link | unstripped output byte-identical (sha256 `791e561c…` both). The flag is in both r18b default link lines but is a no-op on this code. |
| `-Wl,-z,relro -Wl,-z,now` | original has `PT_GNU_RELRO` and `DT_FLAGS=BIND_NOW`; our build matches. |
| `-Wl,--build-id` | original has a build-id; our build matches. |
| `-Wl,--exclude-libs,libgcc.a` | original exports no libgcc helper; r14 reference (without the exclusion) exports `__aeabi_ldivmod`. |
| `-Wl,-soname,libgof2hdaa.so` | `DT_SONAME` matches. |

### 4.4 Link flags and NEEDED order

Both r18b default link lines contain `-Wl,--build-id`,
`-Wl,-z,relro -Wl,-z,now`, `-Wl,--exclude-libs,libgcc.a`, `-lgcc`,
`-Wl,-soname`, and (for armeabi-v7a) `-Wl,--fix-cortex-a8`. The original's
`.dynamic`, program headers and section table match our build exactly. The
`DT_NEEDED` order is the order of the app's `-l` list, not a linker default;
it is reproduced by our link command.

---

## 5. `-g` was full DWARF (`-g2`), stripped after the link

### 5.1 Build-id proves the link output was unstripped

Gold's `--build-id` is SHA-1 over the complete output file with the 20-byte
build-id descriptor zeroed. The shipped file's own SHA-1 (descriptor zeroed)
is **not** the stored build-id:

```text
size                          2,187,328
stored build-id               c011810da45777b83ed721b2a87d517efed0d310
SHA-1 of shipped file, zeroed d7c05dc5e9804023d8786147addc48537b0bee6d
SHA-1 of shipped file, as-is  39528e8a46f5d75adfd851f71bdeb97c02b060f0
```

Therefore the build-id was computed over a larger, unstripped link output
(our own unstripped link is ~19.35 MB). The `.debug_*` and `.symtab` data
were present at link time and removed by the post-link GNU `strip
--strip-all`.

### 5.2 Shipped `.dynsym` sizes prove `-g2`, not `-g0`/line-tables-only

The prior `debug_info_build_id_w5211.md` measured 40 functions whose size
changes between `-g2` and `-g0` (the `-g0`/`-g1`/`-gline-tables-only`/`-gmlt`
codegen is identical). Reading the **shipped original's** `.dynsym` sizes
and comparing them to those two columns gives 40/40 matches to `-g2`:

| function | original size | `-g2` | `-g0` |
|---|---:|---:|---:|
| `_ZN5MGame10OnTouchEndEiiPv` | 8080 | 8080 | 8084 |
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 1896 | 1896 | 1892 |
| `_Z10keyPressedPN11AbyssEngine6EngineEi` | 708 | 708 | 704 |
| `_ZN10AEGeometry9updateLodERKN11AbyssEngine6AEMath6VectorEf` | 532 | 532 | 528 |
| `_ZN10GameRecord4loadEv` | 2696 | 2696 | 2692 |
| `_ZN10ModStation10OnTouchEndEiiPv` | 5404 | 5404 | 5416 |
| `_ZN10ModStation11OnTouchMoveEiiPv` | 384 | 384 | 388 |
| `_ZN10RepairBeam6updateEiP5RadarP5LevelP3Hud` | 1872 | 1872 | 1868 |
| `_ZN11AbyssEngine11PaintCanvas11DrawImage2DEjiih` | 340 | 340 | 348 |
| `_ZN11AbyssEngine11PaintCanvas13Image2DCreateEtRj` | 192 | 192 | 188 |
| `_ZN11AbyssEngine11PaintCanvas20TransformRemoveChildEjj` | 60 | 60 | 62 |
| `_ZN11AbyssEngine11PaintCanvas24MeshChangeMaterialInternEPNS_9TransformEPNS_8MaterialE` | 80 | 80 | 76 |
| `_ZN11AbyssEngine11PaintCanvas29MaterialResourceChangeTextureEjji` | 60 | 60 | 58 |
| `_ZN11AbyssEngine12CurveReleaseEPPNS_5CurveE` | 86 | 86 | 90 |
| `_ZN11AbyssEngine16ShaderBaseStruct14ES2LoadProgramEPKcS2_` | 240 | 240 | 236 |
| `_ZN11AbyssEngine18ApplicationManager25RegisterApplicationModuleEjPNS_18IApplicationModuleE` | 58 | 58 | 54 |
| `_ZN11AbyssEngine18ApplicationManager8SoundSetEPKNS_11AESoundInfoEi` | 52 | 52 | 50 |
| `_ZN11AbyssEngine6Engine11SetTexturesEjj` | 312 | 312 | 316 |
| `_ZN11AbyssEngine6Engine14ShaderRegisterEPNS_16ShaderBaseStructE` | 128 | 128 | 124 |
| `_ZN11AbyssEngine6String5SplitES0_` | 196 | 196 | 194 |
| `_ZN11AbyssEngine6String9SplitTagsES0_` | 456 | 456 | 452 |
| `_ZN12Achievements28initCheckEquipmentAndWeaponsEv` | 164 | 164 | 172 |
| `_ZN12HangarWindow6updateEi` | 488 | 488 | 486 |
| `_ZN14DialogueWindow3setEP7Missionii` | 120 | 120 | 116 |
| `_ZN16SystemPathFinder15getJumpDistanceEP5ArrayIP11SolarSystemEii` | 38 | 38 | 36 |
| `_ZN4Item17combineDuplicatesEP5ArrayIPS_E` | 130 | 130 | 128 |
| `_ZN4Ship10getSlotPosEP4Item` | 84 | 84 | 82 |
| `_ZN4Ship13getCargoValueEv` | 52 | 52 | 56 |
| `_ZN5Level16almostKillWantedEi` | 252 | 252 | 248 |
| `_ZN5MGame12OnInitializeEv` | 2080 | 2080 | 2076 |
| `_ZN6Player15StopEngineSoundEv` | 40 | 40 | 44 |
| `_ZN6Player16PauseEngineSoundEv` | 36 | 36 | 40 |
| `_ZN6Player4healEf` | 94 | 94 | 100 |
| `_ZN6Player6addGunEP5ArrayIP3GunEi` | 118 | 118 | 116 |
| `_ZN6Player6damageEibi` | 1236 | 1236 | 1232 |
| `_ZN8CutScene14checkForTurretEv` | 1048 | 1048 | 1044 |
| `_ZN9BluePrint19getIngredientsValueEv` | 88 | 88 | 92 |
| `_ZN9FModSound23playMusicFadeOutCurrentEi` | 44 | 44 | 48 |
| `ndk_setNativeItemInformationList` | 1504 | 1504 | 1496 |
| `pConstToNonConst` | 40 | 40 | 36 |

(The prior note's `_ZN9RepairBeam…` spelling is a typo for
`_ZN10RepairBeam…`; the shipped symbol is the latter and matches `-g2`.)

This is a direct, shipped-file proof that the original was compiled with
`-g2`-level full DWARF, not `-g0`, `-g1`, `-gline-tables-only` or `-gmlt`.
`-g2` and `-g3` are byte-identical in clang 7.0.2, so the exact level
between them is unobservable. The DWARF version is also unobservable from
the stripped file; clang 7.0.2's default is DWARF 4, `-gdwarf-2/3/4` are
codegen-neutral, and `-gdwarf-5` segfaults on 45 TUs of this source.

### 5.3 What `-g` does at `-Oz`

The mechanism is the one recorded in `debug_info_flags_w3564.md`:
`-g2` emits `DBG_VALUE` users that pin entry-block argument copies against
PostRA machine sinking. At `-g0`/`-g1` the copy sinks into the error branch.
Example (`InitializeBlowfish`, 416 B in both):

```text
# -g / -g2 (and original)
blx   opensubkeyfile@plt
mov   r6, r0
cbz   r0, error

# -g0 / -g1 / line-tables-only
blx   opensubkeyfile@plt
cbz   r0, error
mov   r6, r0
```

The shipped `.dynsym` size table above is the direct witness that the
original has the `-g2` schedule.

---

## 6. Build paths and `-fdebug-prefix-map`

The only path-like string in the shipped original is the NDK libc++abi
buildbot path:

```text
/Volumes/Android/buildbot/src/android/ndk-release-r18/external/libcxx/../../external/libcxxabi/src/abort_message.cpp
```

Our canonical build contains the same string. It comes from the r18b
prebuilt `libc++abi.a`, not from the app. The r14 reference contains the
corresponding `ndk-r14-release` path.

There is **no** app source path, no `DW_AT_comp_dir`, no `DW_AT_name`, no
`-fdebug-prefix-map` value and no build-directory string in the shipped
`.so`. A byte scan of `.comment`, `.rodata`, `.dynstr` and the whole file
finds only the libc++abi path and the three `main.c` log strings
(`dataDirectory in main.c: %s`, `rootPath in main.c: %s`,
`zipDirectory in main.c: %s`). The app's debug/symbol data was entirely in
the stripped `.debug_*`/`.symtab`.

`-fdebug-prefix-map` and `-fdebug-compilation-dir` are codegen-neutral but
change the build-id (prior note, same-directory experiment). Because the
build-id is a one-way SHA-1 over the whole unstripped output, the original
build directory cannot be recovered from the shipped file, and the original
build-id cannot be reproduced until every other unstripped byte matches.

---

## 7. Ranked remaining uncertainties

1. **Exact build system (high confidence, not proof).** The NDK r18b CMake
   toolchain Release defaults match every observable compile flag and the
   absence of `-fdata-sections`; default ndk-build is excluded by the
   `.rodata`/`.data`/`.bss` layout. The remaining alternatives are
   ndk-build with an explicit `-fno-data-sections`, or a hand-written build
   script copying the CMake defaults. No `Android.mk`, `Application.mk`,
   `CMakeLists.txt`, `.ninja`, `.d` or build log survives in the APK.
2. **`-g2` vs `-g3`.** Byte-identical in clang 7.0.2; the shipped `.dynsym`
   proves the `-g2`/`-g3` codegen class, not the exact spelling.
3. **DWARF version.** `-gdwarf-2/3/4` are codegen-neutral; clang 7.0.2
   defaults to 4; `-gdwarf-5` crashes. Unobservable from the stripped file.
4. **Original build directory / `DW_AT_comp_dir` / `-fdebug-prefix-map`
   value.** Stripped; only the one-way build-id remains.
5. **Original unstripped size and build-id preimage.** Not shipped; our
   unstripped link is ~19.35 MB, but the original's exact size is unknown.
6. **Exact NDK of `libcrashlytics.so`.** GCC 4.9.x 20150123 + gold 1.11
   points to r13b or r14; the file does not encode the package revision.
7. **`-fPIC` vs `-fpic`.** Byte-identical on this source; CMake uses
   `-fPIC`, ndk-build uses `-fpic`; no witness.
8. **`-DANDROID` / `-DNDEBUG`.** Both are r18b default flags and both are
   codegen-neutral here; no independent witness.
9. **`-mfpu=neon` global vs per-source.** The merged `.ARM.attributes`
   shows VFPv3 + NEONv1, which requires every object to have seen
   `-mfpu=neon`; whether that was CMake `ANDROID_ARM_NEON=ON` or
   ndk-build `LOCAL_ARM_NEON := true` is not distinguishable.
10. **Whether the original used `-Wl,--fix-cortex-a8`.** It is in both
    r18b default link lines but is a byte-level no-op on this code.

---

## 8. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w8621
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6

# APK inventory
mkdir -p /dev/shm/w8621/apk
cd /dev/shm/w8621/apk
unzip -o -q "/home/debian/GalaxyOnFire2/Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk" 'lib/*'
for f in lib/armeabi-v7a/*.so; do
  readelf -p .comment "$f"; readelf -nW "$f"; readelf -dW "$f" | grep -E 'SONAME|NEEDED|FLAGS'
  readelf -AW "$f" | grep -E 'Tag_'
done

# toolchain identity
NDK=/home/debian/GalaxyOnFire2/wt/w8621/.cache/ndk/android-ndk-r18b
cat "$NDK/source.properties"
cat "$NDK/toolchains/llvm/prebuilt/linux-x86_64/AndroidVersion.txt"
grep -n 'Updated Clang' "$NDK/CHANGELOG.md"
"$NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/clang" --version
"$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-ld.gold" --version
"$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-strip" --version

# -fdata-sections A/B
cat > /dev/shm/w8621/toolchain_extra.cmake <<'WRAP'
include("/home/debian/GalaxyOnFire2/wt/w8621/cmake/toolchain.cmake")
set(GOF2_EXTRA_FLAGS "" CACHE STRING "extra flags")
if(GOF2_EXTRA_FLAGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GOF2_EXTRA_FLAGS}" CACHE STRING "" FORCE)
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${GOF2_EXTRA_FLAGS}" CACHE STRING "" FORCE)
endif()
WRAP
cmake -S . -B /dev/shm/w8621/fdata -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/dev/shm/w8621/toolchain_extra.cmake \
  -DGOF2_EXTRA_FLAGS='-fdata-sections' -DGOF2_MATCH_OPT=-Oz \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build /dev/shm/w8621/fdata --target link_so -j6
python3 tools/verify/verify.py \
  --build-dir /dev/shm/w8621/fdata/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /dev/shm/w8621/fdata/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0

# -fpic A/B: same wrapper, -DGOF2_EXTRA_FLAGS='-fpic'
# --fix-cortex-a8: relink the canonical objects with gold and add --fix-cortex-a8
#   (the unstripped outputs are byte-identical on this tree).
```

Scratch (not committed): `/dev/shm/w8621/` — `apk/`, `unity_libgof2hdaa.so`,
`unity_price.apk`, `toolchain_extra.cmake`, `fdata/`, `fpic/`,
`relink_base.so`, `relink_fixcortex.so`, `litpool2.py`, logs.

---

## 9. Files touched

* `_work/reconstructions/build-provenance-apk-toolchain-forensics-wave141-w8621.md`
  (this note).
* No source, tool, CMake, baseline, allowlist or doc change.
