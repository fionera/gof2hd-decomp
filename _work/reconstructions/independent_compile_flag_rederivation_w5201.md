# Independent compile-flag re-derivation (w5201)

Status: research note, 2026-11-04. Worktree `/home/debian/GalaxyOnFire2/wt/w5201`,
branch `wave104-w5201`, base commit `8c5fd3646`. Every number below was produced in this
worktree from the shipped ELF and from controlled rebuilds, not by reading
`cmake/toolchain.cmake` first. No `src/`, `tools/`, baseline or allowlist change is landed;
this commit contains only this note.

Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(sha256 `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`,
2,187,328 bytes).

Scratch area: `/dev/shm/w5201/` (`tmp/`, `scratch/`, two repo copies for the
`-fno-function-sections` / `-fno-unwind-tables` tests). `TMPDIR=/dev/shm/w5201/tmp`.

## 0. Bottom line

* The original was built by **NDK r18b clang 7.0.2 + gold 1.12**, target
  `armv7-none-linux-androideabi16`, **armeabi-v7a, Thumb-2, NEON, softfp, libc++**, with
  **`-Oz -g`**, **exceptions and RTTI on**, **`-fstack-protector-strong`**,
  **`-funwind-tables`**, **`-ffunction-sections`**, **no `-fdata-sections`**, and
  **default visibility**.
* Every compile flag in the current canonical recipe is confirmed. No flag is disproven.
  `-DANDROID`, `-DNDEBUG` and the app-side `-D__ANDROID_API__=16` are codegen-neutral on
  this source (the API-16 link is separately visible in `.note.android.ident`).
* **No strict improvement exists** from the compile flags tested. The canonical build is
  already at the maximum `byte_exact 4500 / linked_exact 4500 / size-exact 4524/4524`.
  Every non-neutral variant loses byte-exact functions; every neutral variant is
  byte-identical to the canonical `.so` except `.note.gnu.build-id`.

Baseline (`cmake --build cmake-build-match --target check -j6`):

```text
compared 4524  avg 99.9%  byte_exact 4500  linked_exact 4500  fuzzy_perfect 4500
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`lint`: `CLEAN ✓ (no new soft violations; hard invariants 0)`.
`drift`: `=== DRIFT: 0 field(s) across 0 class(es) ===`.

Because this is a nested worktree, the canonical configure used:

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
```

## 1. Direct ELF fingerprints (artifact first)

### 1.1 Toolchain / target identity

```sh
ORIG=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
NDK=.cache/ndk/android-ndk-r18b
READELF=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-readelf
$READELF -hW  "$ORIG"
$READELF -AW  "$ORIG"
$READELF -nW  "$ORIG"
$READELF -p .comment "$ORIG"
$READELF -dW  "$ORIG"
```

Raw observations:

| observable | value |
|---|---|
| ELF header | `ELF32`, `DYN`, `ARM`, `Version5 EABI`, flags `0x5000200` (`EF_ARM_ABI_FLOAT_SOFT`) |
| `.comment` | `Android (4751641 based on r328903) clang version 7.0.2` and `GCC: (GNU) 4.9.x 20150123 (prerelease)` |
| `.note.android.ident` | Android note, API 16, NDK `r18b`, build `5063045` |
| `.note.gnu.gold-version` | `gold 1.12` |
| `.ARM.attributes` | `Tag_CPU_name: "ARM v7"`, `Tag_CPU_arch: v7`, `Tag_CPU_arch_profile: Application`, `Tag_THUMB_ISA_use: Thumb-2`, `Tag_FP_arch: VFPv3`, `Tag_Advanced_SIMD_arch: NEONv1`, `Tag_ABI_PCS_GOT_use: GOT-indirect`, `Tag_ABI_PCS_wchar_t: 4`, `Tag_ABI_enum_size: int`, `Tag_ABI_align_needed: 8-byte`, `Tag_CPU_unaligned_access: v6`, `Tag_ABI_HardFP_use: Deprecated` (no `Tag_ABI_VFP_args`) |
| `DT_NEEDED` | `liblog, libGLESv2, libGLESv1_CM, libEGL, libandroid, libfmodex, libfmodevent, libm, libdl, libc` |
| `DT_FLAGS` / `DT_FLAGS_1` | `BIND_NOW` / `NOW` |
| `DT_RELCOUNT` | `1162` |
| `.init_array` / `.fini_array` | 3 / 2 entries |

Section table (27 sections; all headers match the canonical build):

```text
.text       0x1337c8 @ 0x69440
.ARM.exidx  0x006f70 @ 0x19cc08
.ARM.extab  0x026298 @ 0x1a3b78
.rodata     0x041b67 @ 0x1c9e10
.data.rel.ro 0x002fe8 @ 0x20cbb0
.data       0x00296c @ 0x214000
.bss        0x0069ac @ 0x216970
```

The canonical build has the same sizes and bases, and `.rodata`, `.data`,
`.data.rel.ro`, `.dynsym`, `.dynstr`, `.hash`, `.rel.dyn`, `.rel.plt`, `.plt`, `.got`,
`.dynamic`, `.comment`, `.ARM.attributes` and `.shstrtab` are byte-identical to the
original (only `.text`, `.ARM.exidx`, `.ARM.extab` content and `.note.gnu.build-id`
differ).

### 1.2 ISA / ABI: `-march=armv7-a`, `-mthumb`, `-mfpu=neon`, `-mfloat-abi=softfp`

The `.ARM.attributes` above are the direct fingerprint:

* `Tag_CPU_arch: v7` + `Tag_CPU_arch_profile: Application` + `Tag_CPU_name: "ARM v7"`
  ⇒ generic ARMv7-A, not a named CPU. A tiny object built with
  `-mcpu=cortex-a8` emits `Tag_CPU_name: "cortex-a8"`; `-mcpu=cortex-a9` emits
  `"cortex-a9"`; `-march=armv7-a` (and `-march=armv7-a -mtune=cortex-a8`) emits no
  CPU name. The original's `"ARM v7"` is the generic ARMv7-A tag.
* `Tag_THUMB_ISA_use: Thumb-2` and the disassembly: the app `.text` is Thumb-2
  (`objdump -d` reports 160,463 32-bit Thumb encodings). The `.plt` is ARM, as usual.
* `Tag_FP_arch: VFPv3` + `Tag_Advanced_SIMD_arch: NEONv1` ⇒ `-mfpu=neon`
  (`-mfpu=vfpv3-d16` alone emits `Tag_FP_arch: VFPv3-D16` and no NEON tag; it also
  fails to compile this source because `Engine.cpp` uses NEON intrinsics).
* ELF flags `0x5000200` (`EF_ARM_ABI_FLOAT_SOFT`), no `Tag_ABI_VFP_args`, and
  `Tag_ABI_HardFP_use: Deprecated` ⇒ **softfp**. A `-mfloat-abi=hard` full build fails
  at link with `uses VFP register arguments, output does not`.

Instruction counts from `objdump -d "$ORIG"`:

```text
NEON/VFP mnemonics (vld/vst/vadd/vmul/vmov/...): 20304
32-bit Thumb encodings:                         160463
```

### 1.3 PIC: `-fPIC` / `-fpic`

```sh
$READELF -rW "$ORIG" | awk '/R_ARM_/{print $3}' | sort | uniq -c
$READELF -dW "$ORIG" | grep TEXTREL
```

```text
1817 R_ARM_ABS32
 614 R_ARM_GLOB_DAT
3674 R_ARM_JUMP_SLOT
1162 R_ARM_RELATIVE
(no R_ARM_REL32; no DT_TEXTREL)
```

`Tag_ABI_PCS_GOT_use: GOT-indirect` confirms GOT-indirect global access. The binary is
PIC. `-fPIC`, `-fpic` and `-fPIE` produce byte-identical objects on this source (see
§3), so the exact spelling is unobservable.

### 1.4 Exceptions and RTTI: `-fexceptions`, `-frtti`

```sh
$READELF --dyn-syms -W "$ORIG" | grep -E '_Unwind_Resume|__gxx_personality_v0|__cxa_begin_catch'
$READELF --dyn-syms -W "$ORIG" | awk '$7!="UND" && ($8 ~ /^_ZTI/ || $8 ~ /^_ZTV/ || $8 ~ /^_ZTS/){n++} END{print n}'
```

```text
259: UND _Unwind_Resume
310: 0x1924e1 348 FUNC __gxx_personality_v0
456: 0x191ecd 112 FUNC __cxa_begin_catch
451 defined _ZTI/_ZTV/_ZTS objects
```

`.ARM.extab` is `0x26298` (156,312 bytes). The original has full EHABI LSDA data and
typeinfo/vtables, so both exceptions and RTTI are on. `-fno-rtti` drops the typeinfo
objects to 281 and the gate to 1701; `-fno-exceptions` shrinks `.ARM.extab` to
`0x780c` and the gate to 1344 (§3).

### 1.5 Stack protector: `-fstack-protector-strong`

```sh
$READELF --dyn-syms -W "$ORIG" | grep -E '__stack_chk_fail|__stack_chk_guard'
OBJDUMP=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OBJDUMP -d "$ORIG" | grep -c '__stack_chk_fail@plt'
```

```text
36: UND __stack_chk_fail@LIBC
37: UND __stack_chk_guard@LIBC
788 calls to __stack_chk_fail@plt
```

The canonical build has exactly 788 canary sites. `-fstack-protector` (basic) and
`-fno-stack-protector` both produce 89/85 canary calls and lose the gate (1369/1369),
so the **strong** variant is the one the binary shows.

### 1.6 Function sections and unwind tables: `-ffunction-sections`, `-funwind-tables`

`readelf -uW` on the original:

```text
.ARM.exidx contains 3566 entries
272 entries are [cantunwind]
194 target addresses appear twice (one real entry + one empty-section sentinel)
```

The 194 duplicate-address entries are gold's `fix_exidx_coverage()` sentinel for the
empty plain `.text` section every `-ffunction-sections` object carries.  Parsing the
`readelf -uW` entries by address gives exactly **193 CANTUNWIND+real pairs and 1
real+real pair** (the zero-size `AbyssEngine::getAppVersion`). A proper no-`-ffunction-sections`
rebuild (repo copy with the target option removed) has only 52 duplicate addresses,
83 cantunwind entries, `.text 0x133158`, and `byte_exact 1345` (§3).

`-funwind-tables` is visible on the C TUs. `InitializeBlowfish` (0x6b3b0),
`Blowfish_encipher` (0x6b310) and `Blowfish_decipher` (0x6b360) are C functions with
real `.ARM.extab` records in the original:

```text
0x6b310: @0x1a3d50  compact model 1
0x6b360: @0x1a3d5c  compact model 1
0x6b3b0: @0x1a3d68  compact model 1
```

Object-level A/B on `BlowfishSubkeyFile.c`: `-funwind-tables` emits
`.ARM.extab.text.Blowfish_encipher` / `.ARM.extab.text.Blowfish_decipher`;
`-fno-unwind-tables` emits only CANTUNWIND exidx records and no `.ARM.extab`. A proper
no-`-funwind-tables` full rebuild has `.ARM.extab 0x2556c`, `.ARM.exidx 0x6678`, and
`byte_exact 2878` (§3).

### 1.7 Data sections: `-fno-data-sections`

The original's data layout is the **no-`-fdata-sections`** layout. Full-link comparison
against the original:

| build | `.rodata` size | differing bytes | `.data` size | differing bytes | `.data.rel.ro` differing bytes |
|---|---:|---:|---:|---:|---:|
| canonical (no `-fdata-sections`) | `0x41b67` | **0** | `0x296c` | **0** | **0** |
| `-fdata-sections` | `0x41a07` | **113,935** | `0x295c` | **3,023** | **365** |

This is direct data-layout evidence, not a section-ordering artifact: the canonical
build's `.rodata`, `.data` and `.data.rel.ro` are byte-identical to the original.

### 1.8 C++ dialect: `-std=c++11`

The original contains the C++11 signed-`new[]` guard
`orr.w rX, rY, rY, asr #31` in 10 app functions:

```text
0x7a456  Material::Material
0xcdc54  RecordHandler::readRecordAsByteArray
0xcdd84  RecordHandler::readOptionsFileAsByteArray
0xe0a06  NewsItem::clone
0x15202a Gun::Gun
0x173ace MeshMerger::MeshMerger (two sites)
0x181274 LodMeshMerger::LodMeshMerger (two sites)
0x181a88 IParticleSystem::IParticleSystem
```

Object-level A/B on `NewsItem.cpp` (same command, only `-std` changed):

```text
c++11     guard=1
gnu++11   guard=1
c++14     guard=0
gnu++14   guard=0
```

So the dialect is C++11 (or GNU++11), not the clang default gnu++14. A full
`-std=gnu++14` build gives `byte_exact 1512`, `linked_exact 4487` (§3).

### 1.9 Debug info: `-g` / `-g2`, not `-g0`

The ELF is stripped, so DWARF cannot be read directly. The codegen fingerprint is the
entry-block argument copy in `InitializeBlowfish`:

```text
original 0x6b3cc:
  blx opensubkeyfile@plt
  4606  mov r6, r0
  b128  cbz r0, ...
```

Object-level `BlowfishSubkeyFile.c`:

```text
-g :  mov r6, r0 ; cbz r0, ...   (matches original)
-g0:  cbz r0, ... ; mov r6, r0   (copy sunk into the error branch)
```

`-g` and `-g2` are the same clang 7.0.2 setting (`-debug-info-kind=limited`); `-g3`
is also identical here. Full-build A/B: `-g2` and `-g3` are `4500/4500` and
byte-identical to the canonical `.so` except `.note.gnu.build-id`; `-g0` is
`2069/4247` (§3).

### 1.10 Standard library: `-stdlib=libc++`

The original's libc++abi provenance is visible in `.rodata`:

```text
/Volumes/Android/buildbot/src/android/ndk-release-r18/external/libcxx/../../external/libcxxabi/src/abort_message.cpp
```

and the dynsym contains the libc++abi typeinfo classes
(`_ZTVN10__cxxabiv117__class_type_infoE`, `_ZTVN10__cxxabiv120__si_class_type_infoE`,
`__cxa_*`, `_ZSt9terminatev`, ...). NDK r18b on this host ships only
`llvm-libc++`/`llvm-libc++abi` headers (`sources/cxx-stl/` has no
`gnu-libstdc++`), and the build uses `-stdlib=libc++`. This is positive evidence for
libc++/libc++abi, not libstdc++.

### 1.11 Visibility: default

The original exports 5,370 defined dynamic symbols (4,524 functions + objects).
`-fvisibility=hidden` hides 4,280 of the 4,524 compared functions and drops the gate to
`byte_exact 57`; the canonical default visibility is correct.

## 2. Controlled A/B matrix

Method: a scratch wrapper included the repo toolchain and appended extra flags to
`CMAKE_CXX_FLAGS` / `CMAKE_C_FLAGS`, then `link_so` was built and `verify.py` run
directly against the original. For `-fno-function-sections` and `-fno-unwind-tables`,
the repo was copied to `/dev/shm/w5201/repo_nofs2` / `repo_nounwind` and the
`target_compile_options(gof2 PRIVATE ...)` line was edited, because the target option
is appended after `CMAKE_CXX_FLAGS` and would otherwise override the extra flag.

Baseline: `.text 0x1337c8`, `.rodata 0x41b67`, `.ARM.exidx 0x6f70`,
`.ARM.extab 0x26298`, sha256 `54c6106c...`.

| variant | byte | linked | size | `.text` | `.rodata` | `.ARM.exidx` | `.ARM.extab` | sha256 prefix |
|---|---:|---:|---:|---:|---:|---:|---:|---|
| baseline (`-Oz -g -fPIC -frtti -fstack-protector-strong -ffunction-sections -funwind-tables`) | **4500** | **4500** | **4524** | `0x1337c8` | `0x41b67` | `0x6f70` | `0x26298` | `54c6106c` |
| `-g2` | 4500 | 4500 | 4524 | `0x1337c8` | `0x41b67` | `0x6f70` | `0x26298` | `567d0eac` |
| `-g3` | 4500 | 4500 | 4524 | `0x1337c8` | `0x41b67` | `0x6f70` | `0x26298` | `33b8537c` |
| `-g0` | 2069 | 4247 | 4484 | `0x1337c8` | `0x41b67` | `0x6f70` | `0x26298` | `c16d1c19` |
| `-std=gnu++14` | 1512 | 4487 | 4514 | `0x133788` | `0x41b67` | `0x6f70` | `0x26298` | `127ec0fd` |
| `-fpic` | 4500 | 4500 | 4524 | `0x1337c8` | `0x41b67` | `0x6f70` | `0x26298` | `e21405ad` |
| `-fno-rtti` | 1701 | 4500 | 4524 | `0x1337c8` | `0x4104e` | `0x6f70` | `0x26298` | `0ed06eee` |
| `-fno-exceptions` | 1344 | 3757 | 3772 | `0x11d464` | `0x41b57` | `0x6f68` | `0x780c` | `e48aeaa5` |
| `-fno-stack-protector` | 1369 | 3780 | 3789 | `0x12e108` | `0x41b67` | `0x6f18` | `0x2607c` | `0a4d4c1f` |
| `-fstack-protector` | 1369 | 3785 | 3793 | `0x12e188` | `0x41b67` | `0x6f18` | `0x2607c` | `8909a91d` |
| `-fno-function-sections` (proper copy) | 1345 | 4357 | 4399 | `0x133158` | `0x41b67` | `0x6930` | `0x26298` | `0e2efe22` |
| `-fno-unwind-tables` (proper copy) | 2878 | 4500 | 4524 | `0x1337c8` | `0x41b67` | `0x6678` | `0x2556c` | `a0be5b61` |
| `-fdata-sections` | 4311 | 4500 | 4524 | `0x1337c8` | `0x41a07` | `0x6f70` | `0x26298` | `36570936` |
| `-fno-data-sections` | 4500 | 4500 | 4524 | `0x1337c8` | `0x41b67` | `0x6f70` | `0x26298` | `94eea4cf` |
| `-D__ANDROID_API__=21` (overrides 16) | 4500 | 4500 | 4524 | `0x1337c8` | `0x41b67` | `0x6f70` | `0x26298` | `d75c10b0` |
| `-UANDROID -UNDEBUG` | 4500 | 4500 | 4524 | `0x1337c8` | `0x41b67` | `0x6f70` | `0x26298` | `144bb94a` |
| `-fvisibility=hidden` | 57 | 243 | 244* | `0x130f38` | `0x41b67` | `0x6f70` | `0x26280` | `e0e0594e` |
| `-marm` | 57 | 237 | 342 | `0x1a38b8` | `0x41b67` | `0x6eb8` | `0x25b18` | `1091495e` |
| `-mcpu=cortex-a8` | 1330 | 4226 | 4367 | `0x133fa8` | `0x41b67` | `0x6f90` | `0x262a4` | `b91abd26` |
| `-mtune=cortex-a8` | 4500 | 4500 | 4524 | `0x1337c8` | `0x41b67` | `0x6f70` | `0x26298` | `31177f9f` |
| `-O2` | 1168 | 2159 | 2411 | `0x1631e8` | `0x42d77` | `0x6880` | `0x250b4` | `673ab3ce` |
| `-fno-sized-deallocation` | 4500 | 4500 | 4524 | `0x1337c8` | `0x41b67` | `0x6f70` | `0x26298` | `84012bc5` |
| `-fno-delete-null-pointer-checks` | 4500 | 4500 | 4524 | `0x1337c8` | `0x41b67` | `0x6f70` | `0x26298` | `13c4e684` |
| `-fmerge-all-constants` | 4500 | 4500 | 4524 | `0x1337c8` | `0x41b67` | `0x6f70` | `0x26298` | `74af8e9f` |
| `-fno-omit-frame-pointer` | 4500 | 4500 | 4524 | `0x1337c8` | `0x41b67` | `0x6f70` | `0x26298` | `385f44b8` |

`*` `-fvisibility=hidden` is not a like-for-like row: only 244 functions are compared
(4,280 app exports disappear), so its `size` count is over the surviving set.

Every neutral row above (`-g2`, `-g3`, `-fpic`, `-fno-data-sections`, `-D__ANDROID_API__=21`,
`-UANDROID -UNDEBUG`, `-mtune=cortex-a8`, `-fno-sized-deallocation`,
`-fno-delete-null-pointer-checks`, `-fmerge-all-constants`, `-fno-omit-frame-pointer`)
was also compared section-by-section with the canonical `.so`: the **only** differing
section is `.note.gnu.build-id` (19–20 bytes of its 20-byte descriptor). They are
codegen-identical on this source.

`-mfloat-abi=hard` does not link (`uses VFP register arguments, output does not`);
`-mfpu=vfpv3-d16` alone does not compile (`NEON support not enabled`).

## 3. Confidence table

| flag | verdict | hard evidence | confidence |
|---|---|---|---|
| `-Oz` | **proven** | `InitializeBlowfish` is 0x1a0 B at `-Oz`, 0x2f0 at `-O2`, 0x5fc at `-O3`, 0x1b8 at `-Os`, 0x198 at `-O1`, 0x2cc at `-O0`; original is 0x1a0. Full `-O2` build: 1168/2159, `.text 0x1631e8`. | high |
| `-g` / `-g2` | **proven (at least `-g2`-level)** | `InitializeBlowfish` entry-block copy order matches `-g`/`-g2`, not `-g0`; `-g2`/`-g3` full builds are 4500/4500 and code-identical except build-id; `-g0` is 2069/4247. | high |
| exact `-g` vs `-g2` vs `-g3` | **consistent, unobservable** | all three lower to `-debug-info-kind=limited` in clang 7.0.2 and produce identical code here. | high (for codegen equality) |
| `-std=c++11` | **proven** | signed-`new[]` guard `orr ..., asr #31` in 10 app functions; `NewsItem.cpp` guard present at c++11/gnu++11, absent at c++14/gnu++14; full `-std=gnu++14` is 1512/4487. | high |
| exact `-std=c++11` vs `gnu++11` | **consistent, unobservable** | same guard and same object for the tested TU; no GNU-extension witness found. | medium |
| `-fPIC` / `-fpic` | **PIC proven; spelling unobservable** | no `R_ARM_REL32`, no `DT_TEXTREL`, 614 `R_ARM_GLOB_DAT`, `Tag_ABI_PCS_GOT_use: GOT-indirect`; `-fPIC`/`-fpic`/`-fPIE` objects are byte-identical. | high (PIC), high (spelling unobservable) |
| `-frtti` | **proven** | 451 defined `_ZTI/_ZTV/_ZTS` objects, `__gxx_personality_v0`, libc++abi typeinfo; `-fno-rtti` drops to 281 objects and 1701/4500. | high |
| `-fexceptions` | **proven** | `.ARM.extab 0x26298`, `_Unwind_Resume` UND, `__gxx_personality_v0`, `__cxa_begin_catch`; `-fno-exceptions` shrinks extab to `0x780c` and gives 1344/3757. | high |
| `-fstack-protector-strong` | **proven** | 788 `__stack_chk_fail@plt` calls and `__stack_chk_guard`/`__stack_chk_fail` imports; `-fstack-protector` and `-fno-stack-protector` both give 89/85 canaries and 1369. | high |
| `-funwind-tables` | **proven** | C functions `Blowfish_encipher`/`decipher`/`InitializeBlowfish` have real `.ARM.extab` records; `-fno-unwind-tables` removes them, shrinks extab to `0x2556c`, and gives 2878/4500. | high |
| `-ffunction-sections` | **proven** | 194 duplicate-address `.ARM.exidx` entries (193 empty-`.text` sentinels + zero-size `getAppVersion`), 272 cantunwind; proper no-`-ffunction-sections` build has 52 duplicates, 83 cantunwind, `.text 0x133158`, 1345/4357. | high |
| `-fdata-sections` | **disproven** | `-fdata-sections` changes `.rodata` 0x41b67→0x41a07 (113,935 differing bytes), `.data` 0x296c→0x295c (3,023), `.data.rel.ro` (365), and drops the gate to 4311. | high |
| `-fno-data-sections` | **proven (default)** | canonical no-`-fdata-sections` build has `.rodata`/`.data`/`.data.rel.ro` byte-identical to the original; explicit `-fno-data-sections` is neutral. | high |
| `-DANDROID` | **consistent, unobservable** | `-UANDROID` is neutral (4500/4500, only build-id differs); `src/` has no `#ifdef ANDROID` code path. | medium |
| `-DNDEBUG` | **consistent, unobservable** | `-UNDEBUG` is neutral; the only `__assert2` call site is in libc++abi (`0x193eb2`), not app code. | medium |
| `-D__ANDROID_API__=16` | **API 16 proven at link; app macro unobservable** | `.note.android.ident` says API 16; `-D__ANDROID_API__=21` is neutral (4500/4500, only build-id differs). | high (link API), medium (macro) |
| `-march=armv7-a` | **proven** | `.ARM.attributes` `Tag_CPU_arch: v7`, `Tag_CPU_arch_profile: Application`, `Tag_CPU_name: "ARM v7"`; `-mcpu=cortex-a8` emits `"cortex-a8"` and gives 1330/4226. | high |
| `-mthumb` | **proven** | `Tag_THUMB_ISA_use: Thumb-2`, 160,463 32-bit Thumb encodings; `-marm` gives 57/237. | high |
| `-mfpu=neon` | **proven** | `Tag_FP_arch: VFPv3`, `Tag_Advanced_SIMD_arch: NEONv1`, 20,304 NEON/VFP instructions; `-mfpu=vfpv3-d16` alone emits VFPv3-D16 and fails to compile NEON intrinsics. | high |
| `-mfloat-abi=softfp` | **proven** | ELF flags `0x5000200` (`EF_ARM_ABI_FLOAT_SOFT`), no `Tag_ABI_VFP_args`, `Tag_ABI_HardFP_use: Deprecated`; `-mfloat-abi=hard` fails to link. | high |
| `-stdlib=libc++` | **proven** | libc++abi `abort_message.cpp` path, `__cxxabiv1` typeinfo, `_ZSt9terminatev`, `__cxa_*`; NDK r18b ships only libc++/libc++abi headers. | high |
| default visibility | **proven** | 5,370 defined dynamic symbols; `-fvisibility=hidden` hides 4,280 functions and gives 57/243. | high |
| `-mtune=cortex-a8` | **consistent, unobservable** | neutral (4500/4500; only build-id differs); does not change `.ARM.attributes` CPU name. | high (neutral) |
| `-fno-sized-deallocation`, `-fno-delete-null-pointer-checks`, `-fmerge-all-constants`, `-fno-omit-frame-pointer` | **consistent, unobservable** | all neutral (4500/4500; only build-id differs). | high (neutral) |

## 4. Proven vs disproven vs unobservable

**Positively proven by the binary (direct fingerprint and/or controlled A/B):**

* `-Oz`, `-g`/`-g2` (at least limited debug info), `-std=c++11`, PIC (`-fPIC`/`-fpic`
  family), `-frtti`, `-fexceptions`, `-fstack-protector-strong`, `-funwind-tables`,
  `-ffunction-sections`, `-fno-data-sections`, `-march=armv7-a`, `-mthumb`,
  `-mfpu=neon`, `-mfloat-abi=softfp`, `-stdlib=libc++`, default visibility.
* Link-level API 16 (`.note.android.ident`), gold 1.12, NDK r18b, clang 7.0.2.

**Disproven (a full build is worse than the canonical 4500/4500):**

* `-g0` (2069/4247), `-std=gnu++14` (1512/4487), `-fno-rtti` (1701/4500),
  `-fno-exceptions` (1344/3757), `-fno-stack-protector` (1369/3780),
  `-fstack-protector` (1369/3785), `-fno-function-sections` (1345/4357),
  `-fno-unwind-tables` (2878/4500), `-fdata-sections` (4311/4500),
  `-fvisibility=hidden` (57/243), `-marm` (57/237), `-mcpu=cortex-a8` (1330/4226),
  `-O2` (1168/2159).
* `-mfloat-abi=hard` (link failure) and `-mfpu=vfpv3-d16` alone (compile failure) are
  incompatible with the original's source/ABI.

**Consistent but unobservable (codegen-identical on this source):**

* `-fPIC` vs `-fpic` vs `-fPIE`; `-g` vs `-g2` vs `-g3`; `-std=c++11` vs `-std=gnu++11`;
  `-DANDROID`, `-DNDEBUG`, `-D__ANDROID_API__=16` vs `=21`; `-mtune=cortex-a8`;
  `-fno-sized-deallocation`, `-fno-delete-null-pointer-checks`, `-fmerge-all-constants`,
  `-fno-omit-frame-pointer`.
* The exact build driver (ndk-build, CMake, Gradle, custom script) is not recoverable
  from the stripped `.so`; only the effective flags are.

## 5. Strict improvement

**No strict improvement exists from the compile flags tested.** The canonical build is
already at the maximum gate:

```text
byte_exact 4500/4524, linked_exact 4500/4524, size-exact 4524/4524,
data_exact 843/843, imports 207, lint CLEAN, drift 0.
```

Every non-neutral variant above loses byte-exact functions. Every neutral variant is
byte-identical to the canonical `.so` except `.note.gnu.build-id`, so none can raise the
count. No `src/`, `tools/`, baseline or allowlist change is warranted; this commit
contains the note only.

## 6. Reproduction commands

```sh
# baseline
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
cmake --build cmake-build-match --target lint  -j6
cmake --build cmake-build-match --target drift -j6

# direct ELF fingerprints
ORIG=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
NDK=.cache/ndk/android-ndk-r18b
READELF=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-readelf
OBJDUMP=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$READELF -hW "$ORIG"; $READELF -AW "$ORIG"; $READELF -nW "$ORIG"
$READELF -p .comment "$ORIG"; $READELF -dW "$ORIG"
$READELF -rW "$ORIG" | awk '/R_ARM_/{print $3}' | sort | uniq -c
$READELF -uW "$ORIG" | grep -c '\[cantunwind\]'
$READELF -uW "$ORIG" | grep -E '^0x[0-9a-f]+:' | sed 's/:.*//' | sort | uniq -d | wc -l
$OBJDUMP -d "$ORIG" | grep -c '__stack_chk_fail@plt'
$OBJDUMP -d "$ORIG" | grep -cE '\b(vld|vst|vadd|vmul|vmov|vsub|vdiv|vpush|vpop|vldr|vstr|vcvt|vabs|vneg|vmax|vmin|vsqrt|vdup|vext|vzip|vuzp|vrev|vshl|vshr|vand|vorr|veor|vbic|vmvn|vceq|vcgt|vclt|vcle|vcge|vmla|vmls|vnmla|vnmls|vrecpe|vrsqrte|vtbl|vswp|vtrn|vcombine|vget|vset)'
$READELF --dyn-syms -W "$ORIG" | awk '$7!="UND" && ($8 ~ /^_ZTI/ || $8 ~ /^_ZTV/ || $8 ~ /^_ZTS/){n++} END{print n}'

# scratch wrapper (append flags after the canonical set)
cat > /dev/shm/w5201/tmp/toolchain_extra.cmake <<'X'
set(GOF2_EXTRA_FLAGS "" CACHE STRING "")
set(GOF2_EXTRA_CXX_FLAGS "" CACHE STRING "")
set(GOF2_EXTRA_C_FLAGS "" CACHE STRING "")
include("/home/debian/GalaxyOnFire2/wt/w5201/cmake/toolchain.cmake")
if(GOF2_EXTRA_FLAGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GOF2_EXTRA_FLAGS}" CACHE STRING "" FORCE)
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${GOF2_EXTRA_FLAGS}" CACHE STRING "" FORCE)
endif()
if(GOF2_EXTRA_CXX_FLAGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GOF2_EXTRA_CXX_FLAGS}" CACHE STRING "" FORCE)
endif()
if(GOF2_EXTRA_C_FLAGS)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GOF2_EXTRA_C_FLAGS}" CACHE STRING "" FORCE)
endif()
X

# one variant
BD=/dev/shm/w5201/scratch/build_<tag>
cmake -S . -B "$BD" -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/dev/shm/w5201/tmp/toolchain_extra.cmake \
  -DGOF2_MATCH_OPT=-Oz -DGOF2_EXTRA_CXX_FLAGS='<flags>' \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build "$BD" --target link_so -j6
python3 tools/verify/verify.py --build-dir "$BD/verify" \
  --original "$ORIG" --ours "$BD/libgof2hdaa.so" --objdump "$OBJDUMP" \
  --fail-on-wrong-type --top 0

# proper -fno-function-sections / -fno-unwind-tables: copy the repo and edit
# target_compile_options(gof2 PRIVATE -ffunction-sections -funwind-tables)
# in the copy's CMakeLists.txt, then configure/build/verify the copy.

# object-level checks
CLANGXX=$NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++
# NewsItem.cpp: -std=c++11/gnu++11 -> 1 signed-new[] guard; c++14/gnu++14 -> 0
# BlowfishSubkeyFile.c: InitializeBlowfish size 0x1a0 (Oz), 0x2f0 (O2), 0x5fc (O3),
#   0x1b8 (Os), 0x198 (O1), 0x2cc (O0); entry-block copy order matches -g, not -g0
# BlowfishSubkeyFile.c: -funwind-tables emits .ARM.extab.text.Blowfish_encipher/decipher;
#   -fno-unwind-tables emits only CANTUNWIND exidx records
# NewsItem.cpp: -ffunction-sections -> 14 .text.* sections; -fno-function-sections -> 0
```

## 7. Artifacts

* Scratch builds: `/dev/shm/w5201/scratch/build_<tag>/`.
* Proper no-function-section / no-unwind-table repo copies:
  `/dev/shm/w5201/repo_nofs2/`, `/dev/shm/w5201/repo_nounwind/`.
* No tracked file other than this note changed.
