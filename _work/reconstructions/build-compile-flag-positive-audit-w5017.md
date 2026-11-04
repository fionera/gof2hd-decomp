# Build compile-flag positive audit (w5017, wave 100)

Status: research note, 2026-11-04. Worktree `/home/debian/GalaxyOnFire2/wt/w5017`,
branch `wave100-w5017`, base `44225192c`. Scratch is `/dev/shm/w5017`.

This is an **independent** audit: it starts from the shipped APK and the shipped
`libgof2hdaa.so`, not from `cmake/toolchain.cmake`, and re-derives each compile flag from
binary or controlled-rebuild evidence.  The current recipe is confirmed.  No flag is
disproven.  One build-file comment was corrected (the `-fPIC`/`-fpic` rationale); no
compile or link flag changed.

Canonical gate, measured before and after this pass:

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
size-exact 4524/4524; lint CLEAN; drift 0
```

## 0. Bottom line

* The APK's `lib/armeabi-v7a/libgof2hdaa.so` is the **raw bfd-strip artifact**
  (sha256 `6d38fd76...e3b58`).  The repo's ground-truth file is the **same ELF after
  `.shstrtab`/`sh_name` normalization to gold Stringpool order** (sha256
  `d4c17be6...9483`).  All section contents other than `.shstrtab` are identical; the
  difference is 259 bytes of `.shstrtab` plus 29 bytes across 26 section-header `sh_name`
  fields.  This is a correction to wave 93's §2.7 claim that the APK lib is byte-identical
  to the current ground truth.
* The shipped ELF proves **NDK r18b clang 7.0.2, gold 1.12, API 16, armeabi-v7a,
  Thumb-2, NEON, softfp, libc++**, and the current compile recipe.
* New positive fingerprints not in wave 93:
  * `-fstack-protector-strong` is separated from **both** `-fstack-protector` and
    `-fstack-protector-all` by two exact functions: `InitializeBlowfish` (416 B strong/all,
    372 B basic) and `opensubkeyfile` (52 B strong/basic, canary-bearing under `-all`).
  * `-std=c++14` changes `AbyssEngine::Material::Material` from 276 B (original) to 272 B;
    `-std=gnu++11` is byte-identical to `-std=c++11`.
  * `-fno-strict-aliasing` changes `Engine::RenderMesh` from 1040 B (original) to 1032 B.
  * `-fno-rtti` leaves `.text` unchanged but removes typeinfo from `.rodata`/`.data.rel.ro`
    (e.g. `FileInterfaceAndroid.cpp`: `.rodata` 155 -> 116, `.data` 116 -> 96).
  * `-DNDEBUG`, `-DANDROID`, `-D__ANDROID_API__=21` and `-fno-merge-constants` are
    **byte-identical except `.note.gnu.build-id`** on the full build; they are unobservable
    on this source, not disproven.
  * `-fPIC` and `-fpic` are byte-identical on this source; only the exact spelling is
    unobservable.
  * `-stdlib=libc++` is proven by code-byte identity of `__cxa_allocate_exception` /
    `__cxa_guard_acquire` with NDK r18b `libc++abi.a` (not merely by symbol names).
  * `__*_chk`: only `__stack_chk_fail` / `__stack_chk_guard` exist; no `__memcpy_chk`
    family, so `_FORTIFY_SOURCE` left no trace.
* `-fvisibility-inlines-hidden` is **disproven** if it were used: full-build
  `byte_exact` drops 4500 -> 1278 and 141 exported functions disappear.  The current
  recipe correctly uses default visibility.

## 1. Shipped artifacts and provenance

### 1.1 APK inventory

```sh
APK='/home/debian/GalaxyOnFire2/Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk'
sha256sum "$APK"
unzip -l "$APK" | grep -E 'lib/armeabi-v7a|AndroidManifest.xml'
unzip -l "$APK" | grep -Ei 'Android\.mk|Application\.mk|CMakeLists|\.cmake|\.gradle|\.ninja|\.o$|\.a$'
```

Results:

* APK sha256 `9cfe4484...9eb19`; 143 entries.
* Native libs: only `lib/armeabi-v7a/` (`libgof2hdaa.so`, `libfmodex.so`,
  `libfmodevent.so`, `libcrashlytics.so`).
* No `Android.mk`, `Application.mk`, `CMakeLists.txt`, `*.cmake`, `*.gradle`, `*.ninja`,
  `*.o` or `*.a` in the APK.
* `META-INF/MANIFEST.MF` starts with `Created-By: Android Gradle 3.1.2`.
* `assets/crashlytics-build.properties` says `#Mon Oct 22 10:18:17 CEST 2018`,
  `version_name=2.0.16`, `version_code=47947032`.

REA corroboration (artifact inventory only; no build files):

```sh
rea inspect-artifact "$APK" --detail summary \
  --evidence_output /dev/shm/w5017/apk-evidence.json --format json
```

The summary reports `143` file entries, `144` occurrences, `0` integrity contradictions,
and no build-file entries.

Binary AXML parse of `AndroidManifest.xml` (small local parser; see §8):

```text
manifest: versionCode=47947032 versionName=2.0.16 package=net.fishlabs.gof2hdallandroid2012
uses-sdk: minSdkVersion=16 targetSdkVersion=27
uses-feature: glEsVersion=0x00020000 required
```

`minSdkVersion=16` is the compile/link API and matches `.note.android.ident`; `targetSdk=27`
is Java-only and does not change the native build.

### 1.2 APK lib vs ground truth: the `.shstrtab` normalization

```sh
O=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
unzip -p "$APK" lib/armeabi-v7a/libgof2hdaa.so > /dev/shm/w5017/apk-libgof2hdaa.so
sha256sum "$O" /dev/shm/w5017/apk-libgof2hdaa.so
cmp -l /dev/shm/w5017/apk-libgof2hdaa.so "$O" | head
```

| file | sha256 | size |
|---|---|---:|
| APK `lib/armeabi-v7a/libgof2hdaa.so` | `6d38fd767ee3b4f8174c371bce371bfc6397cf76e13c1756938b2a94658e3b58` | 2,187,328 |
| ground truth `_work/bins/android_2.0.16_libgof2hdaa.so` | `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483` | 2,187,328 |

`cmp -l` reports 288 differing bytes, all in file offsets `0x215af2..0x216018`
(`cmp -l` prints 1-based byte positions, so its first/last are `0x215af3`/`0x216019`):

* 259 differing bytes inside `.shstrtab` (section range `0x215af0..0x215c07`);
* 29 differing bytes in 26 section-header `sh_name` fields (table starts at `0x215c08`);
  entries 15, 21 and 25 have a second differing byte in the `sh_name` word.

The `.shstrtab` has the **same multiset of section names** in a different order: the APK
lib is bfd strip's section-header order, the ground truth is gold's Stringpool order.
Every non-`.shstrtab` section's content is identical; every section-header field other
than `sh_name` is identical.  This is exactly what `tools/reorder_shstrtab.py` does.

Implication: for compile-flag work, the APK lib and ground truth are equivalent; the
ground-truth file is the normalized copy used by the verifier.  The raw APK lib is the
true shipped artifact.

### 1.3 Toolchain identity in the ELF

```sh
readelf -hW  "$O"
readelf -AW  "$O"
readelf -nW  "$O"
readelf -p .comment "$O"
readelf -dW  "$O"
readelf -rW  "$O" | awk '/R_ARM_/{print $3}' | sort | uniq -c
```

| observable | value | conclusion |
|---|---|---|
| ELF | `ELF32`, `DYN`, `ARM`, `EABI5`, flags `0x5000200` (`EF_ARM_ABI_FLOAT_SOFT`) | ARMv7 EABI, **softfp** |
| `.comment` | `Android (4751641 based on r328903) clang version 7.0.2` + `GCC: (GNU) 4.9.x 20150123` | **clang 7.0.2** app TUs; GCC 4.9 vendored libcrypto/libgcc |
| `.note.android.ident` | `api=16`, `ndk="r18b"`, `build="5063045"` | **NDK r18b, API 16** |
| `.note.gnu.gold-version` | `gold 1.12` | **gold 1.12** |
| `.ARM.attributes` | CPU v7/A, Thumb-2, VFPv3, NEONv1, GOT-indirect, wchar_t 4, enum int, 8-byte align, unaligned v6 | **ARMv7-A, Thumb-2, NEON, softfp** |
| `DT_NEEDED` | `liblog libGLESv2 libGLESv1_CM libEGL libandroid libfmodex libfmodevent libm libdl libc` | link-input order |
| `DT_FLAGS` / `DT_FLAGS_1` | `BIND_NOW` / `NOW` | `-z now` |
| relocations | 1817 `ABS32`, 614 `GLOB_DAT`, 3674 `JUMP_SLOT`, 1162 `RELATIVE`, **0 `REL32`** | PIC, no text relocations |
| dynamic tags | `DT_RELCOUNT=1162`, no `DT_TEXTREL` | `-z combreloc`, non-textrel PIC |

`.ARM.attributes`, `.comment`, `.note.android.ident`, `.note.gnu.gold-version`,
`.init_array` and `.fini_array` are byte-identical between the current build and ground
truth.  `.gcc_except_table` is **absent**; ARM EHABI LSDA data lives in `.ARM.extab`.

## 2. Method

Two independent measurement layers:

1. **Direct binary fingerprints** on the ground truth (counts, symbols, section sizes,
   disassembly).  These are per-TU-independent: they do not depend on the current source
   split or link order.
2. **Controlled rebuilds** of representative TUs and of the full link, using the exact
   compile command from `cmake-build-match/compile_commands.json` with one flag changed.
   The object-level harness is in §8.1; the full-link harness is in §8.2.

For full-link variants, the fast counter in §8.3 compares every common `.dynsym` function's
raw bytes and size.  It reproduces the official verifier's `byte_exact` and `size-exact`
on the baseline (4500/4524), on `-fvisibility-inlines-hidden` (1278/4381), and on
`-fstack-protector` / `-fstack-protector-all` (1373/3785, 83/964); the official verifier was
run for those variants.  `linked_exact` is quoted only where the official verifier was run.

No `--update-baseline` was run.  The canonical `cmake-build-match` was not used for
variants.

## 3. Positive fingerprints

### 3.1 `-Oz` vs `-Os` / `-O2` / `-O3`

The original is `-Oz`, not `-Os`/`-O2`/`-O3`.  Full-link A/B (same tree, same link order):

| variant | byte_exact | size-exact | `.text` | `.rodata` | `.ARM.exidx` | `.ARM.extab` |
|---|---:|---:|---:|---:|---:|---:|
| `-Oz` (base) | **4500** | **4524/4524** | `0x1337c8` | `0x41b67` | `0x6f70` | `0x26294` |
| `-Os` | 1263 | 2883/4356 | `0x14bdc8` | `0x43057` | `0x6a30` | `0x24770` |
| `-O2` | 1168 | 2411/4296 | `0x1632e8` | `0x42d77` | `0x6880` | `0x250b4` |
| `-O3` | 1167 | 2247/4295 | `0x171638` | `0x42c97` | `0x6838` | `0x25470` |

Exact object-level anchor:

```sh
SRC=src/engine/render/Engine.cpp
python3 /dev/shm/w5017/compile_tu.py "$SRC" /dev/shm/w5017/obj/Engine.base.o '' ''
python3 /dev/shm/w5017/compile_tu.py "$SRC" /dev/shm/w5017/obj/Engine.os.o   '-Os' ''
python3 /dev/shm/w5017/compile_tu.py "$SRC" /dev/shm/w5017/obj/Engine.o2.o   '-O2' ''
python3 /dev/shm/w5017/objstat.py /dev/shm/w5017/obj/Engine.base.o /dev/shm/w5017/obj/Engine.os.o
```

`Engine::SetTextures(unsigned, unsigned)` is **312 B** in the original and under `-Oz`,
**328 B** under `-Os`, and **336 B** under `-O2`.  This is a positive per-TU fingerprint,
not just a whole-build score.  (`Engine::GetGravValue` happens to be 80 B and identical
under all three, so it is *not* a discriminator.)

### 3.2 `-g` (debug info is codegen-relevant)

Full-link A/B:

| variant | byte_exact | size-exact | `.text` |
|---|---:|---:|---:|
| `-g` (base) | **4500** | **4524/4524** | `0x1337c8` |
| `-g0` | 2069 | 4485/4524 | `0x1337c8` |
| `-g1` | 2069 | 4485/4524 | `0x1337c8` |

Exact object-level anchor (`BlowfishSubkeyFile.c`, `InitializeBlowfish`, 416 B):

```sh
python3 /dev/shm/w5017/compile_tu.py src/engine/file/BlowfishSubkeyFile.c \
  /dev/shm/w5017/obj/Blowfish.base.o '' ''
python3 /dev/shm/w5017/compile_tu.py src/engine/file/BlowfishSubkeyFile.c \
  /dev/shm/w5017/obj/Blowfish.g0.o '-g0' '-g'
$OBJ -dr /dev/shm/w5017/obj/Blowfish.base.o --section=.text.InitializeBlowfish | sed -n '1,30p'
$OBJ -dr /dev/shm/w5017/obj/Blowfish.g0.o   --section=.text.InitializeBlowfish | sed -n '1,30p'
```

The original and `-g` have:

```text
blx opensubkeyfile@plt
mov r6, r0
cbz r0, error
```

`-g0` sinks the copy into the error branch:

```text
blx opensubkeyfile@plt
cbz r0, error
mov r6, r0          @ error branch
```

This is the PostRA machine-sink / `DBG_VALUE` mechanism.  The original's stripped `.so`
therefore still proves `-g`-level variable debug info, not `-g0`/line-tables.

### 3.3 `-fstack-protector-strong` vs basic/all

Direct counts on the ground truth and full-link variants:

```sh
$OBJ -d "$O" | grep -cE '(bl|blx|b\.w).*<__stack_chk_fail@plt>'
```

| build | `__stack_chk_fail` call sites |
|---|---:|
| ground truth | **787** |
| current base | **787** |
| `-fstack-protector` (basic) | 88 |
| `-fstack-protector-all` | 4421 |

Official verifier for the two alternatives (same command as §8.3):

```text
-fstack-protector:      compared 4524  avg 93.5%  byte_exact 1373  linked_exact 3785
-fstack-protector-all:  compared 4524  avg 45.97% byte_exact 83    linked_exact 964
```

Two object-level anchors separate all three:

```sh
# strong protects this local-array/address-taken function; basic does not
python3 /dev/shm/w5017/compile_tu.py src/engine/file/BlowfishSubkeyFile.c \
  /dev/shm/w5017/obj/Blowfish.stackbasic.o '-fstack-protector' ''
$OBJ -dr /dev/shm/w5017/obj/Blowfish.stackbasic.o --section=.text.InitializeBlowfish | head -12
# all protects this scalar-only function; strong/basic do not
$OBJ -dr /dev/shm/w5017/obj/Blowfish.stackall.o --section=.text.opensubkeyfile | head -12
```

* `InitializeBlowfish`: 416 B under strong and all, 372 B under basic.
* `opensubkeyfile`: 52 B under strong and basic, canary-bearing and larger under all.

So the original is neither basic nor all; it is **strong**.

### 3.4 Frame pointers kept (`-fomit-frame-pointer` off)

Direct count:

```sh
$OBJ -d "$O" | grep -cE '\badd\s+r7, sp'
```

| build | `add r7, sp` prologues |
|---|---:|
| ground truth | **2473** |
| current base | **2473** |
| `-fomit-frame-pointer` | 736 |

Exact object anchor (`opensubkeyfile`, 52 B):

```sh
$OBJ -dr /dev/shm/w5017/obj/Blowfish.base.o --section=.text.opensubkeyfile | head -8
# 0: push {r7, lr}
# 2: mov  r7, sp
$OBJ -dr /dev/shm/w5017/obj/Blowfish.omitfp.o --section=.text.opensubkeyfile | head -8
# no push {r7,lr}; no mov r7,sp
```

`-fno-omit-frame-pointer` is the default here; adding it explicitly is byte-identical
except build-id.  `-fomit-frame-pointer` is a real regression (full build 1298/2163).

### 3.5 `-funwind-tables`

Direct `.ARM.exidx` / `.ARM.extab` evidence:

```sh
readelf -SW "$O" | grep -E 'ARM.exidx|ARM.extab'
python3 - <<'PY'
# count entries, CANTUNWIND (w1==1), inline compact (w1 bit31), extab records
# (script in §8.4)
PY
```

| observable | original | base |
|---|---:|---:|
| `.ARM.exidx` entries | **3566** | 3566 |
| CANTUNWIND (`w1==1`) | **272** | 272 |
| CANT entries sharing a target with a real entry | **193** | 193 |
| duplicate target addresses | 194 | 194 |
| `.ARM.extab` size | `0x26298` | `0x26294` (known -4) |

Object-level `-fno-unwind-tables` (appended after the target's `-funwind-tables`):

```sh
python3 /dev/shm/w5017/compile_tu.py src/engine/render/Engine.cpp \
  /dev/shm/w5017/obj/Engine.nounwind.o '-fno-unwind-tables' ''
python3 /dev/shm/w5017/objstat.py /dev/shm/w5017/obj/Engine.base.o /dev/shm/w5017/obj/Engine.nounwind.o
```

`Engine.cpp` `.ARM.exidx` changes (640 B, 72 differing bytes) and `.ARM.extab` shrinks
1168 -> 1120; `.text` is unchanged.  The original's 3566-entry EXIDX table therefore
requires `-funwind-tables`.

### 3.6 `-ffunction-sections`

Direct evidence: the 193 CANTUNWIND entries that share an address with a real entry are
gold's `fix_exidx_coverage()` sentinel for the empty plain `.text` section every
`-ffunction-sections` object carries.

Object-level evidence:

```sh
python3 /dev/shm/w5017/compile_tu.py src/engine/render/Engine.cpp \
  /dev/shm/w5017/obj/Engine.nofuncsections.o '' '-ffunction-sections'
# section headers excluding the null section, and the plain .text size
readelf -SW /dev/shm/w5017/obj/Engine.base.o | grep -c '^\s*\['
readelf -SW /dev/shm/w5017/obj/Engine.nofuncsections.o | grep -c '^\s*\['
readelf -SW /dev/shm/w5017/obj/Engine.base.o | grep -E '^\s*\[\s*[0-9]+\] \.text\s'
readelf -SW /dev/shm/w5017/obj/Engine.nofuncsections.o | grep -E '^\s*\[\s*[0-9]+\] \.text\s'
```

| object | section headers (excl. null) | plain `.text` |
|---|---:|---:|
| base (`-ffunction-sections`) | 394 | 0 B (empty) |
| no `-ffunction-sections` | 92 | 10896 B (`0x2a90`) |

The original's 193 CANT+real pairs and its per-function `.text` order match the
`-ffunction-sections` form, not the monolithic form.

### 3.7 `-fdata-sections` absent

Full-link evidence:

| build | `.rodata` | `.data` | byte_exact |
|---|---:|---:|---:|
| base (no `-fdata-sections`) | **`0x41b67`** | **`0x296c`** | **4500** |
| `-fdata-sections` | `0x41a07` | `0x295c` | 4311 |

Object-level evidence:

```sh
python3 /dev/shm/w5017/compile_tu.py src/game/world/Level.cpp \
  /dev/shm/w5017/obj/Level.fdata.o '-fdata-sections' ''
python3 /dev/shm/w5017/objstat.py /dev/shm/w5017/obj/Level.base.o /dev/shm/w5017/obj/Level.fdata.o
```

`Level.cpp` section headers 664 -> 762 (excluding null); the sum of `.rodata*` sections
6490 -> 6334; `.data*` stays 68 B in both, while `.bss*` 64 -> 56 (per-symbol alignment).
The original's `.rodata` is byte-identical to the no-`-fdata-sections` layout, so the flag
was **not** used.

### 3.8 `-std=c++11` vs `gnu++11` / `c++14`

Full-link A/B:

| variant | byte_exact | size-exact | `.text` |
|---|---:|---:|---:|
| `-std=c++11` (base) | **4500** | **4524/4524** | `0x1337c8` |
| `-std=gnu++11` | **4500** | **4524/4524** | `0x1337c8` |
| `-std=c++14` | 1512 | 4514/4524 | `0x133788` |

`-std=gnu++11` is byte-identical except build-id; the exact spelling is unobservable.
`-std=c++14` is disproven by an exact function:

```sh
python3 /dev/shm/w5017/compile_tu.py src/engine/render/PaintCanvas.cpp \
  /dev/shm/w5017/obj/PaintCanvas.cxx14.o '-std=c++14' ''
python3 /dev/shm/w5017/objstat.py /dev/shm/w5017/obj/PaintCanvas.base.o /dev/shm/w5017/obj/PaintCanvas.cxx14.o
```

`AbyssEngine::Material::Material` is **276 B in the original and in `-std=c++11`**, and
**272 B under `-std=c++14`**.  This is a positive per-TU fingerprint.

### 3.9 `-frtti` vs `-fno-rtti`

Direct counts on the ground truth:

```sh
readelf --dyn-syms -W "$O" | grep -c '_ZTV'   # 99
readelf --dyn-syms -W "$O" | grep -c '_ZTI'   # 176
readelf --dyn-syms -W "$O" | grep -c '_ZTS'   # 176
```

Full-link `-fno-rtti`: byte_exact 1701, `.rodata` `0x41b67 -> 0x4104e`, `.data.rel.ro`
`0x2fe8 -> 0x2c1c`.  Object-level anchor:

```sh
python3 /dev/shm/w5017/compile_tu.py src/engine/file/FileInterfaceAndroid.cpp \
  /dev/shm/w5017/obj/FileInterfaceAndroid.nortti.o '-fno-rtti' ''
readelf -sW /dev/shm/w5017/obj/FileInterfaceAndroid.base.o | grep -cE '_ZTI|_ZTS|_ZTV'  # 8
readelf -sW /dev/shm/w5017/obj/FileInterfaceAndroid.nortti.o | grep -cE '_ZTI|_ZTS|_ZTV' # 2
```

`FileInterfaceAndroid.cpp` `.rodata` 155 -> 116 and `.data` 116 -> 96 under `-fno-rtti`.
The original's 176 `_ZTI`/`_ZTS` and 99 `_ZTV` objects prove **RTTI on**.

### 3.10 `-fexceptions` vs `-fno-exceptions`

Direct symbols/calls:

```sh
readelf --dyn-syms -W "$O" | grep -E '__cxa_throw|__cxa_begin_catch|__gxx_personality|_Unwind_'
$OBJ -d "$O" | grep -c '_Unwind_Resume'   # 540 calls
```

The original has `__cxa_throw` (3 calls), `__cxa_begin_catch` (12), `__cxa_end_catch` (10),
`__gxx_personality_v0`, `_Unwind_Resume` (540), and a 156,312-byte `.ARM.extab`.
`-fno-exceptions` fails to compile the real source:

```sh
cmake --build /dev/shm/w5017/build-fno-exceptions --target link_so -j16
# src/game/world/Level.cpp:5215:5: error: cannot use 'try' with exceptions disabled
```

### 3.11 `-fno-sized-deallocation` (C++11 default)

Direct call-site count:

```sh
$OBJ -d "$O" | grep -E '(bl|blx|b\.w).*<_ZdlPvj@plt>' | wc -l   # 0
$OBJ -d "$O" | grep -E '(bl|blx|b\.w).*<_ZdaPvj@plt>' | wc -l   # 0
$OBJ -d "$O" | grep -E '(bl|blx|b\.w).*<_ZdlPv@plt>'  | wc -l   # 1058
$OBJ -d "$O" | grep -E '(bl|blx|b\.w).*<_ZdaPv@plt>'  | wc -l   # 252
```

`_ZdlPvj`/`_ZdaPvj` are defined weak by libc++abi but have **zero call sites**.  Object
anchor:

```sh
python3 /dev/shm/w5017/compile_tu.py src/engine/render/Engine.cpp \
  /dev/shm/w5017/obj/Engine.sizeddealloc.o '-fsized-deallocation' ''
readelf -rW /dev/shm/w5017/obj/Engine.sizeddealloc.o | grep -c '_ZdlPvj'  # >0
readelf -rW /dev/shm/w5017/obj/Engine.base.o          | grep -c '_ZdlPvj'  # 0
```

Full `-fsized-deallocation`: byte_exact 1347, `.text` `0x134c78`.  Full explicit
`-fno-sized-deallocation`: 4500, byte-identical except build-id.  The original is the
C++11 default **no sized delete**.

### 3.12 Thread-safe statics (`-fno-threadsafe-statics` absent)

Direct calls:

```sh
$OBJ -d "$O" | grep -E '<__cxa_guard_acquire@plt>' | wc -l   # 2 calls + PLT label
$OBJ -d "$O" | grep -E '<__cxa_guard_release@plt>' | wc -l   # 1 call + PLT label
```

`GameText::getText(int)` (292 B, byte-exact) contains the full guard fast path
(`ldrb; dmb ish; tst #1; bne` -> `__cxa_guard_acquire` -> construct -> `__cxa_atexit` ->
`__cxa_guard_release`).  Object anchor:

```sh
python3 /dev/shm/w5017/compile_tu.py src/engine/core/GameText.cpp \
  /dev/shm/w5017/obj/GameText.nothreadsafe.o '-fno-threadsafe-statics' ''
readelf -rW /dev/shm/w5017/obj/GameText.base.o        | grep -c '__cxa_guard'  # 4
readelf -rW /dev/shm/w5017/obj/GameText.nothreadsafe.o | grep -c '__cxa_guard' # 0
```

Full `-fno-threadsafe-statics`: byte_exact 1368, `.text` `0x133684`, `.rodata` `0x419b7`.
The original has the guards, so the flag was **not** used.

### 3.13 `__cxa_atexit` (`-fno-use-cxa-atexit` absent)

Direct calls:

```sh
$OBJ -d "$O" | grep -E '<__cxa_atexit@plt>' | wc -l   # 2 call sites + PLT label
$OBJ -d "$O" | grep -E '<atexit@plt>' | wc -l         # 0
```

Call sites are `Engine.cpp`'s `__GLOBAL__sub_I_Engine.cpp` (tail `b __cxa_atexit@plt`)
and `GameText::getText` (direct `blx`).  Object anchor:

```sh
python3 /dev/shm/w5017/compile_tu.py src/engine/core/GameText.cpp \
  /dev/shm/w5017/obj/GameText.nocxaatexit.o '-fno-use-cxa-atexit' ''
readelf -rW /dev/shm/w5017/obj/GameText.nocxaatexit.o | grep -E 'atexit'  # atexit, not __cxa_atexit
```

Full `-fno-use-cxa-atexit`: byte_exact 1356, `.text` `0x133818`.  The original uses
`__cxa_atexit`, so the flag was **not** used.

### 3.14 Strict aliasing (`-fno-strict-aliasing` absent)

Full-link `-fno-strict-aliasing`: byte_exact 1371, `.text` `0x133d08`, `.ARM.exidx`
`0x6f80`, `.ARM.extab` `0x2629c`.  Exact object anchor:

```sh
python3 /dev/shm/w5017/compile_tu.py src/engine/render/Engine.cpp \
  /dev/shm/w5017/obj/Engine.nostrict.o '-fno-strict-aliasing' ''
python3 /dev/shm/w5017/objstat.py /dev/shm/w5017/obj/Engine.base.o /dev/shm/w5017/obj/Engine.nostrict.o
```

`Engine::RenderMesh(Mesh*)` is **1040 B in the original and in the default build**, and
**1032 B under `-fno-strict-aliasing`**.  `Level.cpp` shows the same effect on
`createCampaignMission` (29676 -> 29820), `createMission` (17172 -> 17280), etc.  The
original is **strict aliasing on**; `-fno-strict-aliasing` is disproven.

### 3.15 Visibility

Direct visibility count:

```sh
readelf --dyn-syms -W "$O" | awk '$4=="FUNC" && $7!="UND"{v[$6]++} END{for(k in v) print k,v[k]}'
# DEFAULT 4524
```

Full-link A/B:

| variant | byte_exact | compared | `.text` |
|---|---:|---:|---:|
| default | **4500** | 4524 | `0x1337c8` |
| `-fvisibility=hidden` | 57 | 244 | `0x130f38` |
| `-fvisibility-inlines-hidden` | 1278 | 4383 | `0x1337a8` |

`-fvisibility-inlines-hidden` is **disproven** if it were used: it hides 141 exported
functions and drops `byte_exact` to 1278.  The current recipe correctly leaves default
visibility.

### 3.16 PIC (`-fPIC` vs `-fpic`)

Direct evidence: 614 `R_ARM_GLOB_DAT`, 1817 `R_ARM_ABS32`, 1162 `R_ARM_RELATIVE`,
**0 `R_ARM_REL32`**, no `DT_TEXTREL`; globals are reached GOT-indirect
(`ldr rX,[pc,#N]; add rX,pc; ldr rX,[rX]`).  This proves PIC codegen.

Exact spelling is unobservable:

```sh
python3 /dev/shm/w5017/compile_tu.py src/engine/render/Engine.cpp \
  /dev/shm/w5017/obj/Engine.fpic.o '-fpic' ''
python3 /dev/shm/w5017/objstat.py /dev/shm/w5017/obj/Engine.base.o /dev/shm/w5017/obj/Engine.fpic.o
# .text/.rodata/.data/.ARM.exidx/.ARM.extab all 0 diff
```

Full `-fpic`: 4500/4524, every section byte-identical except `.note.gnu.build-id`.
`-fPIC` is CMake's spelling and the conservative choice; the old comment's claim that
`-fpic` emits `R_ARM_REL32` is false on this source and was corrected.

### 3.17 Defines: `-DNDEBUG`, `-DANDROID`, `-D__ANDROID_API__=16`

Source scan:

```sh
grep -RInE '#if(n?def)?[[:space:]]+ANDROID|defined\(ANDROID\)' src   # none
grep -RInE 'NDEBUG' src                                             # none
grep -RInE 'assert[[:space:]]*\(' src                               # 744 lines, all static_assert
grep -RInE '__ANDROID_API__' src                                    # none
```

Object and full-link A/B:

```sh
# remove -DNDEBUG / -DANDROID; replace API 16 with 21
python3 /dev/shm/w5017/compile_tu.py src/engine/render/Engine.cpp \
  /dev/shm/w5017/obj/Engine.nondEBUG.o '' '-DNDEBUG'
python3 /dev/shm/w5017/compile_tu.py src/engine/render/Engine.cpp \
  /dev/shm/w5017/obj/Engine.noandroid.o '' '-DANDROID'
python3 /dev/shm/w5017/compile_tu.py src/engine/render/Engine.cpp \
  /dev/shm/w5017/obj/Engine.api21.o '-D__ANDROID_API__=21' '-D__ANDROID_API__=16'
```

`-DNDEBUG` and `-DANDROID` produce byte-identical objects.  `-D__ANDROID_API__=21`
produces identical `.text`/`.rodata`/`.data`/`.ARM.exidx`/`.ARM.extab` on `Engine.cpp`,
`Level.cpp`, `PaintCanvas.cpp`, `PlayerEgo.cpp`, `Globals.cpp`, `GameText.cpp`,
`ParticleSettings.cpp`, `Mesh.cpp`, `NFC.cpp`, `FileInterfaceAndroid.cpp`, `main.c`,
`GOF2HD2012.c`, `BackButton.c` and `BlowfishSubkeyFile.c`; only DWARF differs (the macro
value is recorded in `.debug_str`/`.debug_line`).  Full-link variants are 4500/4524 and
byte-identical except `.note.gnu.build-id`.

Conclusion: these are **build-fidelity defaults, not matching knobs** on this source.
`-D__ANDROID_API__=16` is unobservable as a macro, but API 16 itself is **proven** by
`.note.android.ident` (`0x10`) and the r18b `crtbegin_so.o`/sysroot.

### 3.18 `-fmerge-constants`

Object and full-link A/B:

```sh
python3 /dev/shm/w5017/compile_tu.py src/game/world/Level.cpp \
  /dev/shm/w5017/obj/Level.fmergeoff.o '-fno-merge-constants' ''
python3 /dev/shm/w5017/objstat.py /dev/shm/w5017/obj/Level.base.o /dev/shm/w5017/obj/Level.fmergeoff.o
```

`-fno-merge-constants` is byte-identical on `Engine.cpp`, `Level.cpp`, `Globals.cpp`,
`GameText.cpp`, `NFC.cpp`, `main.c`, `BlowfishSubkeyFile.c`; full-link 4500/4524 and
byte-identical except build-id.  `-fmerge-constants` is accepted but codegen-neutral in
clang 7.0.2 on this source; **UNOBSERVABLE**.

### 3.19 ISA flags

| flag | positive fingerprint | command |
|---|---|---|
| `-march=armv7-a` | `.ARM.attributes`: `Tag_CPU_arch: v7`, `Tag_CPU_arch_profile: Application`; `movw`/`movt`, Thumb-2 | `readelf -AW "$O"` |
| `-mthumb` | `Tag_THUMB_ISA_use: Thumb-2`; dynsym entries are odd (`InitializeBlowfish` value `0x6b3b1`) | `readelf -AW "$O"`; `readelf --dyn-syms -W "$O"` |
| `-mfpu=neon` | `Tag_Advanced_SIMD_arch: NEONv1`; 348 `vld1`, 880 `vst1`; `-mfpu=vfpv3-d16` fails on `arm_neon.h` | `readelf -AW "$O"`; `$OBJ -d "$O" \| grep -c vld1`; compile Engine.cpp with `-mfpu=vfpv3-d16` |
| `-mfloat-abi=softfp` | ELF flags `0x5000200` = `EF_ARM_ABI_FLOAT_SOFT`; no `Tag_ABI_VFP_args`; `-mfloat-abi=hard` link fails against softfp FMOD | `readelf -hW "$O"`; `readelf -AW "$O"`; full hard-float link |

`-marm` is disproven: full build byte_exact 57, `.text` `0x1a3818` vs `0x1337c8`.
`-march=armv6` also fails NEON (`arm_neon.h: NEON support not enabled`).

### 3.20 `-stdlib=libc++`

The original is statically linked against NDK r18b's libc++/libc++abi, not libstdc++/gnustl.
The positive proof is a code-byte match against the toolchain archive, not a symbol name
(no `std::__ndk1` type appears in an exported signature, so the mangled-name route is empty):

```sh
NDK=.cache/ndk/android-ndk-r18b
ar x $NDK/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libc++abi.a cxa_exception.o cxa_guard.o
$OBJ -d cxa_exception.o | sed -n '/<__cxa_allocate_exception>:/,/^$/p'
$OBJ -d --start-address=0x191d30 --stop-address=0x191d5a "$O"
$OBJ -d cxa_guard.o | sed -n '/<__cxa_guard_acquire>:/,/^$/p'
$OBJ -d --start-address=0x1922a8 --stop-address=0x19231c "$O"
```

`__cxa_allocate_exception` (42 B) and `__cxa_guard_acquire` (116 B) in the original are
instruction-for-instruction identical to `libc++abi.a`'s `cxa_exception.o` /
`cxa_guard.o`; only the relocated `bl` targets differ (`bl` in the object, `blx ...@plt`
in the linked `.so`).  The original also defines the libc++abi set
`__cxa_begin_cleanup`, `__cxa_call_unexpected`, `__cxa_current_primary_exception`,
`__cxa_increment_exception_refcount`, `__cxa_decrement_exception_refcount`,
`__cxa_allocate_dependent_exception`, `__cxa_free_dependent_exception`,
`__cxa_get_globals{,_fast}`, and `__gxx_personality_v0` (348 B), all from
`libc++abi.a`.  There are **0** `std::__ndk1` / `std::__cxx11` / `std::__1` symbols in
`.dynsym`; libc++ template code is inlined and stripped.

`-stdlib=libc++` is therefore **PROVEN** as the STL implementation.  The exact header
spelling (`-stdlib=libc++` vs `-DANDROID_STL=c++_static`) is a build-system detail and is
unobservable in the ELF.

### 3.21 `__*_chk` / `_FORTIFY_SOURCE`

```sh
readelf --dyn-syms -W "$O" | grep -E '_chk'
$OBJ -d "$O" | grep -oE '<__[a-z_]+_chk(@plt)?>' | sort | uniq -c
```

The only `_chk` names are `__stack_chk_fail` and `__stack_chk_guard` (the stack protector).
There is **no** `__memcpy_chk`, `__memset_chk`, `__strcpy_chk`, `__strlen_chk`, `__sprintf_chk`
or `__vsnprintf_chk`, so `_FORTIFY_SOURCE` was not active (or was neutral on this source).
The current recipe does not pass `-D_FORTIFY_SOURCE`; this is consistent and **UNOBSERVABLE**
on the shipped binary (it neither proves nor disproves a define, only that no fortified
call survived).

## 4. Full A/B matrix

All variants are one flag changed on the canonical build (`-Oz -g -std=c++11 -fPIC
-frtti -fstack-protector-strong -ffunction-sections -funwind-tables -DANDROID -DNDEBUG`).
`byte` and `size` are the fast exact counter (§8.3), cross-checked against the official
verifier on the baseline, `-fvisibility-inlines-hidden`, `-fstack-protector` and
`-fstack-protector-all`.  `.text`/`.rodata` are
section sizes of the linked `.so`.

| variant | byte | size | `.text` | `.rodata` | `.ARM.exidx` | `.ARM.extab` | verdict |
|---|---:|---:|---:|---:|---:|---:|---|
| base `-Oz` | **4500** | **4524/4524** | `0x1337c8` | `0x41b67` | `0x6f70` | `0x26294` | correct |
| `-Os` | 1263 | 2883/4356 | `0x14bdc8` | `0x43057` | `0x6a30` | `0x24770` | worse |
| `-O2` | 1168 | 2411/4296 | `0x1632e8` | `0x42d77` | `0x6880` | `0x250b4` | worse |
| `-O3` | 1167 | 2247/4295 | `0x171638` | `0x42c97` | `0x6838` | `0x25470` | worse |
| `-std=gnu++11` | **4500** | **4524/4524** | `0x1337c8` | `0x41b67` | `0x6f70` | `0x26294` | neutral |
| `-std=c++14` | 1512 | 4514/4524 | `0x133788` | `0x41b67` | `0x6f70` | `0x26294` | worse |
| `-fpic` | **4500** | **4524/4524** | `0x1337c8` | `0x41b67` | `0x6f70` | `0x26294` | neutral |
| `-fvisibility=hidden` | 57 | 244/244 | `0x130f38` | `0x41b67` | `0x6f70` | `0x2627c` | worse |
| `-fvisibility-inlines-hidden` | 1278 | 4381/4383 | `0x1337a8` | `0x41b67` | `0x6f70` | `0x26290` | worse |
| `-fstack-protector` | 1373 | 3794/4524 | `0x12e188` | `0x41b67` | `0x6f18` | `0x26078` | worse |
| `-fstack-protector-all` | 83 | 981/4524 | `0x14f728` | `0x41b67` | `0x7230` | `0x270b8` | worse |
| `-fomit-frame-pointer` | 1298 | 2163/4524 | `0x12df48` | `0x41b67` | `0x6868` | `0x214f0` | worse |
| `-fno-omit-frame-pointer` | **4500** | **4524/4524** | `0x1337c8` | `0x41b67` | `0x6f70` | `0x26294` | neutral |
| `-g0` | 2069 | 4485/4524 | `0x1337c8` | `0x41b67` | `0x6f70` | `0x26294` | worse |
| `-g1` | 2069 | 4485/4524 | `0x1337c8` | `0x41b67` | `0x6f70` | `0x26294` | worse |
| `-fno-rtti` | 1701 | 4524/4524 | `0x1337c8` | `0x4104e` | `0x6f70` | `0x26294` | worse |
| `-fno-exceptions` | build fail | — | — | — | — | — | `Level.cpp:5215 try` |
| `-fno-sized-deallocation` | **4500** | **4524/4524** | `0x1337c8` | `0x41b67` | `0x6f70` | `0x26294` | neutral/default |
| `-fsized-deallocation` | 1347 | 3942/4524 | `0x134c78` | `0x41b67` | `0x6f70` | `0x2629c` | worse |
| `-fno-threadsafe-statics` | 1368 | 4520/4521 | `0x133684` | `0x419b7` | `0x6f50` | `0x26258` | worse |
| `-fno-use-cxa-atexit` | 1356 | 4523/4524 | `0x133818` | `0x41b67` | `0x6f80` | `0x26288` | worse |
| `-fno-strict-aliasing` | 1371 | 4411/4524 | `0x133d08` | `0x41b67` | `0x6f80` | `0x2629c` | worse |
| `-fstrict-aliasing` | **4500** | **4524/4524** | `0x1337c8` | `0x41b67` | `0x6f70` | `0x26294` | neutral/default |
| `-fdata-sections` | 4311 | 4524/4524 | `0x1337c8` | `0x41a07` | `0x6f70` | `0x26294` | worse |
| `-fno-merge-constants` | **4500** | **4524/4524** | `0x1337c8` | `0x41b67` | `0x6f70` | `0x26294` | neutral |
| `-DNDEBUG` removed | **4500** | **4524/4524** | `0x1337c8` | `0x41b67` | `0x6f70` | `0x26294` | neutral |
| `-DANDROID` removed | **4500** | **4524/4524** | `0x1337c8` | `0x41b67` | `0x6f70` | `0x26294` | neutral |
| `-D__ANDROID_API__=21` | **4500** | **4524/4524** | `0x1337c8` | `0x41b67` | `0x6f70` | `0x26294` | neutral |
| `-marm` | 57 | 342/4524 | `0x1a3818` | `0x41b67` | `0x6eb8` | `0x25b1c` | worse |
| `-mfpu=vfpv3-d16` | build fail | — | — | — | — | — | `arm_neon.h` |
| `-mfloat-abi=hard` | link fail | — | — | — | — | — | VFP args vs softfp FMOD |

For the neutral rows (`-fpic`, `-std=gnu++11`, `-fno-sized-deallocation`,
`-fno-omit-frame-pointer`, `-fstrict-aliasing`, `-fno-merge-constants`, the three
define variants), a section-by-section comparison shows **only `.note.gnu.build-id`
differs** (20 bytes).  They are byte-identical code/data, not merely equal counts.

## 5. Classification table

`PROVEN` = positive fingerprint in the shipped binary; `UNOBSERVABLE` = no code/data
difference on this source, cannot be excluded; `DISPROVEN` = a variant moves away from
the original.

| flag | status | evidence |
|---|---|---|
| `-target armv7-none-linux-androideabi16` | **PROVEN** (ABI/API; exact triple spelling unobservable) | ELF ARM EABI5, `.note.android.ident` api 16, r18b sysroot |
| `-march=armv7-a` | **PROVEN** | `.ARM.attributes` v7/A; `movw`/`movt`; Thumb-2 |
| `-mthumb` | **PROVEN** | `Tag_THUMB_ISA_use: Thumb-2`; odd dynsym; `-marm` 57 |
| `-mfpu=neon` | **PROVEN** | NEONv1; `vld1`/`vst1`; `vfpv3-d16` fails NEON intrinsics |
| `-mfloat-abi=softfp` | **PROVEN** | ELF `0x5000200`; no `Tag_ABI_VFP_args`; hard link fails |
| `-Oz` | **PROVEN** | 4500 vs 1263/1168/1167 for `-Os`/`-O2`/`-O3` |
| `-std=c++11` | **PROVEN** (vs c++14) | `Material::Material` 276 vs 272; full c++14 1512 |
| `-std=gnu++11` | **UNOBSERVABLE** | full 4500, only build-id |
| `-fPIC` | **PROVEN** (PIC); exact spelling **UNOBSERVABLE** | GOT/`GLOB_DAT`, no `REL32`/`DT_TEXTREL`; `-fpic` identical |
| default visibility | **PROVEN** | 4524 DEFAULT; `-fvisibility=hidden` 57 |
| `-fvisibility-inlines-hidden` | **DISPROVEN** | full 1278, 141 functions hidden |
| `-fstack-protector-strong` | **PROVEN** | 787 canaries; basic 88; all 4421; two exact anchors |
| `-fomit-frame-pointer` off | **PROVEN** | 2473 frame prologues; omit 736; full 1298 |
| `-fno-omit-frame-pointer` explicit | **PROVEN** (neutral) | full 4500, only build-id |
| `-funwind-tables` | **PROVEN** | 3566 EXIDX/272 CANT/156312 extab; object nounwind changes |
| `-frtti` | **PROVEN** | 176 `_ZTI`/`_ZTS`, 99 `_ZTV`; `-fno-rtti` 1701 |
| `-fexceptions` | **PROVEN** | `_Unwind_Resume` 540, `__gxx_personality_v0`; no-exceptions build fail |
| `-g` | **PROVEN** | g0/g1 2069; `InitializeBlowfish` entry copy |
| `-ffunction-sections` | **PROVEN** | 193 CANT+real; object 394 vs 92 sections |
| `-fdata-sections` | **DISPROVEN** (absent) | `.rodata` 0x41b67 vs 0x41a07; full 4311 |
| `-DNDEBUG` | **UNOBSERVABLE** | no source use; full 4500, only build-id |
| `-DANDROID` | **UNOBSERVABLE** | no source use; full 4500, only build-id |
| `-D__ANDROID_API__=16` | **UNOBSERVABLE** as macro; API 16 **PROVEN** | full 4500 with 21; `.note.android.ident` 16 |
| `-fno-sized-deallocation` | **PROVEN** (default) | 0 `_ZdlPvj`/`_ZdaPvj` calls; sized variant 1347 |
| `-fno-threadsafe-statics` | **DISPROVEN** (absent) | 2 `__cxa_guard_acquire`; variant 1368 |
| `-fno-use-cxa-atexit` | **DISPROVEN** (absent) | 2 `__cxa_atexit`, no `atexit`; variant 1356 |
| `-fno-strict-aliasing` | **DISPROVEN** (absent) | `RenderMesh` 1040 vs 1032; variant 1371 |
| `-fmerge-constants` | **UNOBSERVABLE** | full 4500, only build-id; object identical |
| `-stdlib=libc++` | **PROVEN** | `__cxa_*`/`__cxa_guard_acquire` bytes match `libc++abi.a`; no libstdc++ symbols |
| `_FORTIFY_SOURCE` | **UNOBSERVABLE** (absent) | only `__stack_chk_*`; no `__memcpy_chk`/`__strcpy_chk` |
| `-marm` | **DISPROVEN** | full 57, `.text` 0x1a3818 |
| `-mfpu=vfpv3-d16` | **DISPROVEN** | compile fails `arm_neon.h` |
| `-mfloat-abi=hard` | **DISPROVEN** | link fails VFP args vs softfp FMOD |
| `-Os` / `-O2` / `-O3` | **DISPROVEN** | 1263 / 1168 / 1167 |
| `-g0` / `-g1` | **DISPROVEN** | 2069 |
| `-fstack-protector` / `-all` | **DISPROVEN** | 1373 / 83 |
| `-fno-rtti` | **DISPROVEN** | 1701; typeinfo removed |
| `-fno-exceptions` | **DISPROVEN** | build fail |
| `-fsized-deallocation` | **DISPROVEN** | 1347; `_ZdlPvj` relocs |
| `-fvisibility=hidden` | **DISPROVEN** | 57; 4280 functions hidden |

## 6. Build-file change landed

Only a comment correction in `cmake/toolchain.cmake`; no compile/link flag changed.
The old comment claimed `-fpic` emits `R_ARM_REL32` that the original lacks.  On this
source `-fPIC` and `-fpic` are byte-identical (full build 4500, only build-id), so the
comment now says the exact spelling is unobservable and `-fPIC` is CMake's conservative
spelling.

```diff
-# the original .so's .comment names. -fPIC (not -fpic): the original .so has no R_ARM_REL32 text
-# relocs / DT_TEXTREL and accesses globals GOT-indirect (R_ARM_GLOB_DAT), which is -fPIC codegen;
-# -fpic emits R_ARM_REL32 the original lacks. API 16 is the original's: the first word of its
-# .note.android.ident is 0x10 (16), and NDK r18b's ndk-build default is android-16. All include
-# paths are absolute (CMake compiles from the build dir).
+# the original .so's .comment names. -fPIC: the original .so has no R_ARM_REL32 text relocs /
+# DT_TEXTREL and accesses globals GOT-indirect (R_ARM_GLOB_DAT). Both -fPIC and -fpic are
+# byte-identical on this source (the GOT fits the small model); -fPIC is CMake's spelling and the
+# conservative choice. API 16 is the original's: the first word of its .note.android.ident is
+# 0x10 (16), and NDK r18b's ndk-build default is android-16. All include paths are absolute
+# (CMake compiles from the build dir).
```

Gate before and after: **byte 4500, linked 4500, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, RATCHET PASS**.  No `--update-baseline`.

## 7. What remains unobservable

* Exact `-fPIC` vs `-fpic` spelling; exact `-std=c++11` vs `-std=gnu++11` spelling.
* `-DNDEBUG`, `-DANDROID`, `-D__ANDROID_API__=16` as macros (the API level itself is
  proven by the crt note).
* `-fmerge-constants` (clang 7.0.2 accepts it but emits the same code/data here).
* Exact `-target` vendor/OS spelling; the ABI/API level is proven.
* The build system file (`Android.mk`/`Application.mk` vs a custom Makefile/CMake) is not
  in the APK.  The flags are consistent with ndk-build r18b defaults except the known
  no-`-fdata-sections` and FMOD-before-libc++abi link order, so a custom or overridden
  build is still the simplest explanation.

## 8. Reproduction harness

### 8.1 Object-level compile with the exact TU command

`compile_tu.py` reads `cmake-build-match/compile_commands.json`, removes the requested
flag tokens, replaces `-o`, and appends the variant flags:

```python
#!/usr/bin/env python3
import json, shlex, subprocess, sys, os
src, out, extra, remove = sys.argv[1:5]
cc = json.load(open('/home/debian/GalaxyOnFire2/wt/w5017/cmake-build-match/compile_commands.json'))
ent = next(x for x in cc if os.path.realpath(x['file']) == os.path.realpath(src))
args = shlex.split(ent['command'])
rm = set(remove.split()) if remove else set()
args = [a for a in args if a not in rm]
i = args.index('-o'); args[i+1] = out
args += (extra.split() if extra else [])
os.makedirs(os.path.dirname(out), exist_ok=True)
subprocess.run(args, check=True)
```

Example:

```sh
python3 /dev/shm/w5017/compile_tu.py \
  /home/debian/GalaxyOnFire2/wt/w5017/src/engine/render/Engine.cpp \
  /dev/shm/w5017/obj/Engine.nostrict.o '-fno-strict-aliasing' ''
```

### 8.2 Full-link variant

`toolchain_extra.cmake` includes the real toolchain and appends/removes flags:

```cmake
include("/home/debian/GalaxyOnFire2/wt/w5017/cmake/toolchain.cmake")
set(GOF2_EXTRA_FLAGS "" CACHE STRING "extra matching flags (both C and C++)")
set(GOF2_EXTRA_CXX_FLAGS "" CACHE STRING "extra matching flags (C++ only)")
set(GOF2_EXTRA_C_FLAGS "" CACHE STRING "extra matching flags (C only)")
set(GOF2_REMOVE_FLAGS "" CACHE STRING "space-separated flags to remove")
foreach(_var CMAKE_CXX_FLAGS CMAKE_C_FLAGS)
  foreach(_rm ${GOF2_REMOVE_FLAGS})
    string(REPLACE " ${_rm} " " " _tmp "${${_var}}")
    set(${_var} "${_tmp}")
  endforeach()
endforeach()
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
```

```sh
cmake -S . -B /dev/shm/w5017/build-nostrict -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/dev/shm/w5017/toolchain_extra.cmake \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs \
  -DGOF2_MATCH_OPT=-Oz \
  -DGOF2_EXTRA_FLAGS='-fno-strict-aliasing'
cmake --build /dev/shm/w5017/build-nostrict --target link_so -j16
```

### 8.3 Fast exact byte/size counter

`fast_verify.py` compares every common `.dynsym` function's raw bytes and size; it
reproduces the official `byte_exact`/`size-exact` on the baseline and on
`-fvisibility-inlines-hidden`.

```python
#!/usr/bin/env python3
import sys
sys.path.insert(0, '/home/debian/GalaxyOnFire2/wt/w5017/tools/verify')
from elf import Elf32
orig = Elf32('/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so')
o = orig.functions()
for path in sys.argv[1:]:
    e = Elf32(path); f = e.functions(); common = set(o) & set(f)
    byte = size = 0
    for n in common:
        a, b = o[n], f[n]
        if a['size'] == b['size']:
            size += 1
        if orig.bytes_at(a['value'], a['size']) == e.bytes_at(b['value'], b['size']):
            byte += 1
    print(f'{path}: compared {len(common)} byte {byte} size {size} '
          f'missing {len(set(o)-set(f))} extra {len(set(f)-set(o))}')
```

Official verifier command used for the baseline and visibility variant:

```sh
python3 tools/verify/verify.py \
  --build-dir /dev/shm/w5017/verify-base \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /dev/shm/w5017/build-base/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
```

### 8.4 EXIDX / CANTUNWIND count

```python
import struct
from collections import Counter
p = '/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so'
b = open(p, 'rb').read()
shoff = struct.unpack_from('<I', b, 0x20)[0]
ent = struct.unpack_from('<H', b, 0x2e)[0]
n = struct.unpack_from('<H', b, 0x30)[0]
si = struct.unpack_from('<H', b, 0x32)[0]
sh = [struct.unpack_from('<IIIIIIIIII', b, shoff + i*ent) for i in range(n)]
st = sh[si]; names = b[st[4]:st[4]+st[5]]
def prel31(x, place):            # ARM EHABI prel31 -> absolute address
    v = x & 0x7fffffff
    if x & 0x80000000: v -= 0x80000000
    return place + v
for s in sh:
    o = s[0]; e = names.find(b'\0', o)
    if names[o:e] == b'.ARM.exidx':
        off, size, addr = s[4], s[5], s[2]
        rows = [struct.unpack_from('<II', b, off + i*8) for i in range(size//8)]
        targets = [prel31(w0, addr + i*8) for i,(w0,w1) in enumerate(rows)]
        real = set(t for (w0,w1),t in zip(rows,targets) if w1 != 1)
        print('entries', len(rows), 'cant', sum(1 for w0,w1 in rows if w1 == 1),
              'dup_targets', sum(1 for k,v in Counter(targets).items() if v > 1),
              'cant_sharing_real', sum(1 for (w0,w1),t in zip(rows,targets)
                                       if w1 == 1 and t in real))
# entries 3566 cant 272 dup_targets 194 cant_sharing_real 193
```

### 8.5 AXML manifest parse

A small parser walks the AXML string pool and `RES_XML_START_ELEMENT` chunks and prints
attribute name/typed values:

```python
import struct, sys, zipfile
p = sys.argv[1]
d = zipfile.ZipFile(p).read('AndroidManifest.xml') if p.endswith('.apk') else open(p, 'rb').read()
assert struct.unpack_from('<I', d, 0)[0] == 0x00080003
pos = 8; strings = []
while pos < len(d):
    typ, hs, sz = struct.unpack_from('<HHI', d, pos)
    if typ == 0x0001:                       # RES_STRING_POOL_TYPE
        cnt, stycnt, flags, strstart, stystart = struct.unpack_from('<IIIII', d, pos + 8)
        utf8 = bool(flags & 0x100)
        offs = struct.unpack_from('<%dI' % cnt, d, pos + hs)
        base = pos + strstart
        for o in offs:
            q = base + o
            if utf8:
                n = struct.unpack_from('<H', d, q)[0]; q += 2
                if n & 0x8000: q += 2
                n2 = d[q]; q += 1
                strings.append(d[q:q+n2].decode('utf-8', 'replace'))
            else:
                n = struct.unpack_from('<H', d, q)[0]; q += 2
                strings.append(d[q:q+n*2].decode('utf-16le', 'replace'))
    elif typ == 0x0102:                     # RES_XML_START_ELEMENT_TYPE
        ns, name, attrStart, attrSize, attrCount = struct.unpack_from('<IIHHH', d, pos + 16)
        attrs = []; apos = pos + 16 + attrStart
        for i in range(attrCount):
            ans, an, raw, tsize, res0, dtype, data = struct.unpack_from('<IIIHBBI', d, apos + i*attrSize)
            attrs.append((strings[an] if an < len(strings) else an,
                          strings[raw] if raw < len(strings) else raw, dtype, data))
        print('ELEMENT', strings[name] if name < len(strings) else name, attrs)
    pos += sz
```

Relevant output:

```text
ELEMENT uses-sdk [('minSdkVersion', 4294967295, 16, 16), ('targetSdkVersion', 4294967295, 16, 27)]
```

## 9. Files touched

* `_work/reconstructions/build-compile-flag-positive-audit-w5017.md` (this note).
* `cmake/toolchain.cmake` (comment only; `-fPIC`/`-fpic` rationale corrected).

No `src/` change; no flag change; no `--update-baseline`.
