# `-fdata-sections` contradiction: the binary's effective flag is off (w3691)

Status: research note, 2026-11-02. Worktree `wt/w3691`, branch `wave87-w3691`,
based on master `88aaf59f`. Every number below is from the original
`../_work/bins/android_2.0.16_libgof2hdaa.so` or from a controlled A/B rebuild
of this tree with the repo's NDK r18b clang 7.0.2 / gold 1.12.

## 0. Bottom line

The apparent contradiction is real but one-sided:

* r18b ndk-build's `GLOBAL_CFLAGS` contains `-fdata-sections`
  (`build/core/default-build-commands.mk:135-138`), and older notes read that
  as "the original definitely had `-fdata-sections`".
* The **binary** says the effective data layout was the shared-section
  (`-fno-data-sections`) layout. The original's `.rodata`, `.data` and
  `.data.rel.ro` are byte-identical to the tree's non-`-fdata-sections` build;
  adding `-fdata-sections` changes 42.3 % of `.rodata`, 30.9 % of `.data` and
  3.3 % of `.data.rel.ro`, and costs **163 byte_exact** at the current baseline.

Two independent, fully byte-exact TUs prove the mechanism, one engine TU and
one game TU:

| TU | original order | non-`-fdata` order | `-fdata-sections` order |
|---|---|---|---|
| `AEFile.cpp` (engine) | `_ZTV9AEPakFile`, `_ZTV12AENormalFile`, `_ZTI9AEPakFile`, `_ZTI12AENormalFile`, `_ZTI14AELowLevelFile` | same | `_ZTI14AELowLevelFile` inserted between the vtables and the other typeinfos |
| `MTitle.cpp` (game) | `_ZTV6MTitle`, `_ZTI6MTitle`, `_ZTIN11AbyssEngine18IApplicationModuleE` | same | `_ZTIN11AbyssEngine18IApplicationModuleE` inserted before `_ZTI6MTitle` |

The original therefore matches the **shared-section** (`-fno-data-sections`)
layout, not the per-symbol-section layout. This settles the binary side of the
"why does the original lack the flag" question left open by
`fdata_sections_retest_4496_w3562.md`; the build-system reason is still not
recovered. The ndk-build default is a
build-system inference; it can be overridden by `LOCAL_CFLAGS` /
`NDK_APP_CFLAGS` (both are appended after `GLOBAL_CFLAGS` in
`build/core/definitions.mk`). No original `Android.mk`/`Application.mk`
survives, so the exact override is not recoverable, but the effective flag is.

No source or flag change is landed: `-fdata-sections` is a regression. This
note is the only commit.

## 1. Baseline

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8
```

```text
verify: byte 4496  linked 4496  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
CLEAN ✓ (no new soft violations; hard invariants 0)
=== DRIFT: 0 field(s) across 0 class(es) ===
```

`report.json`: 4524/4524 size-exact. Baseline `.so` sha256:

```text
112311d2c345718bcf8194a5213267839a97c9cc6f83bbe8c12589b1845db152  cmake-build-match/libgof2hdaa.so
d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483  ../_work/bins/android_2.0.16_libgof2hdaa.so
```

Section table (baseline vs original; `.ARM.extab` is the pre-existing -8 B
delta, unrelated to this note):

| section | original | baseline |
|---|---:|---:|
| `.text` | `0x69440` / `0x1337c8` | same |
| `.rodata` | `0x1c9e10` / `0x41b67` | same |
| `.data.rel.ro` | `0x20cbb0` / `0x2fe8` | same |
| `.data` | `0x214000` / `0x296c` | same |
| `.bss` | `0x216970` / `0x69ac` | same |
| `.ARM.extab` | `0x1a3b78` / `0x26298` | `0x1a3b78` / `0x26290` |

## 2. Controlled full-tree A/B: `-fdata-sections`

`cmake/toolchain.cmake` `FORCE`s `CMAKE_CXX_FLAGS`/`CMAKE_C_FLAGS`, so the
extra flag is appended by a wrapper toolchain:

```sh
mkdir -p /tmp/w3691
cat > /tmp/w3691/toolchain_extra.cmake <<'TOOLCHAIN_EOF'
include("/home/debian/GalaxyOnFire2/wt/w3691/cmake/toolchain.cmake")
set(GOF2_EXTRA_FLAGS "" CACHE STRING "extra matching flags")
if(GOF2_EXTRA_FLAGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GOF2_EXTRA_FLAGS}" CACHE STRING "matching ARM compile flags" FORCE)
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${GOF2_EXTRA_FLAGS}" CACHE STRING "matching ARM compile flags" FORCE)
endif()
TOOLCHAIN_EOF

cmake -S . -B /tmp/w3691/build-fdata -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/tmp/w3691/toolchain_extra.cmake \
  -DGOF2_MATCH_OPT=-Oz -DGOF2_EXTRA_FLAGS='-fdata-sections'
cmake --build /tmp/w3691/build-fdata --target link_so -j8

python3 tools/verify/verify.py \
  --build-dir /tmp/w3691/verify-fdata \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3691/build-fdata/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
```

Measured:

```text
compared 4524  avg 99.9%  byte_exact 4333  linked_exact 4496  fuzzy_perfect 4496
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
```

Ratchet on the same report (expected `FAIL` because the baseline is 4496):

```text
verify: byte 4333  linked 4496  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
  FAIL  byte_exact 4496 -> 4333
RATCHET FAIL
```

| variant | byte | linked | size | data | `.text` | `.rodata` | `.data` | `.bss` |
|---|---:|---:|---:|---:|---:|---:|---:|---:|
| baseline | **4496** | 4496 | 4524/4524 | 843/843 | `0x1337c8` | `0x41b67` | `0x296c` | `0x216970`/`0x69ac` |
| app `-fdata-sections` | **4333** | 4496 | 4524/4524 | 843/843 | `0x1337c8` | `0x41a07` | `0x2970` | `0x216970`/`0x69ac` |
| app `-fdata-sections`, no `.data*` ordering line | **4334** | 4496 | 4524/4524 | 843/843 | `0x1337c8` | `0x41a07` | `0x296c` | `0x216970`/`0x69ac` |

The `.data*` line removal test relinked the same `-fdata` objects with
`grep -v -x -F '.data*' /tmp/w3691/build-fdata/section_order.txt >
/tmp/w3691/section_order_nodata.txt` and the exact gold command from
`ninja -C /tmp/w3691/build-fdata -t commands libgof2hdaa.so`. It recovers one
function and the `.data` size, but not the `.rodata` order: the remaining
**162** losses are intrinsic to per-symbol `.rodata` sections.

Section payload comparison against the original:

| section | baseline | app `-fdata-sections` |
|---|---|---|
| `.rodata` | **269159 / 269159** (100 %), first diff past end | **154872 / 268807** (57.61 %), first diff `+0x1db93` (VMA `0x1e79a3`) |
| `.data` | **10604 / 10604** (100 %) | **7331 / 10604** (69.13 %), first diff `+0x4` (VMA `0x214004`) |
| `.data.rel.ro` | **12264 / 12264** (100 %) | **11859 / 12264** (96.70 %), first diff `+0x68` (VMA `0x20cc18`) |

The first `.rodata` divergence is exactly the `AEFile.cpp` RTTI block; the
first `.data` divergence is `NFC::interface_path`. Both are explained below.

## 3. Object-level proof: `AEFile.cpp` (engine TU)

`AEFile.cpp` is fully byte-exact in the gate (all `AEFile`, `AEPakFile`,
`AENormalFile` functions are `pct == 100.0`, `byte == true`). Its data layout
is therefore a clean witness.

Compile the exact `compile_commands.json` line twice, once with
`-fdata-sections`:

```sh
python3 - <<'PY' > /tmp/w3691/ae_cmd.sh
import json
cc = json.load(open('cmake-build-match/compile_commands.json'))
cmd = next(e['command'] for e in cc if e['file'].endswith('/src/engine/file/AEFile.cpp'))
cmd = cmd.replace('-o CMakeFiles/gof2.dir/src/engine/file/AEFile.cpp.obj',
                  '-o /tmp/w3691/AEFile_no.o')
print(cmd)
PY
bash /tmp/w3691/ae_cmd.sh

python3 - <<'PY' > /tmp/w3691/ae_cmd_f.sh
import json
cc = json.load(open('cmake-build-match/compile_commands.json'))
cmd = next(e['command'] for e in cc if e['file'].endswith('/src/engine/file/AEFile.cpp'))
cmd = cmd.replace('-o CMakeFiles/gof2.dir/src/engine/file/AEFile.cpp.obj',
                  '-o /tmp/w3691/AEFile_fdata.o')
cmd = cmd.replace(' -ffunction-sections', ' -fdata-sections -ffunction-sections')
print(cmd)
PY
bash /tmp/w3691/ae_cmd_f.sh

RE=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-readelf
$RE -SW /tmp/w3691/AEFile_no.o    | grep -E '\.rodata|\.data.rel.ro'
$RE -SW /tmp/w3691/AEFile_fdata.o | grep -E '\.rodata|\.data.rel.ro'
```

The relevant object sections are:

```text
no -fdata:
  .rodata                         0x41a  crc32Table + _ZTS9AEPakFile + _ZTS12AENormalFile
  .rodata.str1.1                  0x007
  .rodata._ZTS14AELowLevelFile    0x011  (COMDAT group, align 16)

-fdata:
  .rodata._ZN12_GLOBAL__N_110crc32TableE  0x400
  .rodata.str1.1                          0x007
  .rodata._ZTS9AEPakFile                  0x00b
  .rodata._ZTS14AELowLevelFile            0x011  (COMDAT group, align 16)
  .rodata._ZTS12AENormalFile              0x00f  <-- now after the group
```

Gold concatenates these input sections in section-header order. The original
and the non-`-fdata` build put both plain RTTI strings in one `.rodata`
section before the 16-aligned group; the `-fdata` build inserts the group
between them.

The same effect is visible in `.data.rel.ro`, with exact original symbol
addresses (`readelf -sW`):

| symbol | original | baseline | app `-fdata-sections` |
|---|---:|---:|---:|
| `_ZTV9AEPakFile` | `0x20cc24` | `0x20cc24` | `0x20cc24` |
| `_ZTV12AENormalFile` | `0x20cc48` | `0x20cc48` | `0x20cc48` |
| `_ZTI9AEPakFile` | `0x20cc6c` | `0x20cc6c` | `0x20cc74` |
| `_ZTI12AENormalFile` | `0x20cc78` | `0x20cc78` | `0x20cc80` |
| `_ZTI14AELowLevelFile` | `0x20cc84` | `0x20cc84` | `0x20cc6c` |

The original has the two derived-class vtables, then the two derived-class
typeinfos, then the base-class group. `-fdata-sections` moves the base-class
group to immediately after the vtables. The original matches the shared
section, not the per-symbol section.

The `.rodata` addresses are equally direct:

| symbol | original | baseline | app `-fdata-sections` |
|---|---:|---:|---:|
| `_ZTS9AEPakFile` | `0x1e7998` | `0x1e7998` | `0x1e7998` |
| `_ZTS12AENormalFile` | `0x1e79a3` | `0x1e79a3` | `0x1e79c1` |
| `_ZTS14AELowLevelFile` | `0x1e79c0` | `0x1e79c0` | `0x1e79b0` |

## 4. Object-level proof: `MTitle.cpp` (game TU)

`MTitle.cpp` is also fully byte-exact in the gate. Repeating the same
compile/readelf experiment:

```sh
# same extraction as above, replacing AEFile.cpp with MTitle.cpp and the
# output names with /tmp/w3691/MTitle_{no,fdata}.o
$RE -SW /tmp/w3691/MTitle_no.o    | grep -E '\.rodata|\.data.rel.ro'
$RE -SW /tmp/w3691/MTitle_fdata.o | grep -E '\.rodata|\.data.rel.ro'
```

Object sections:

```text
no -fdata:
  .data.rel.ro                    0x05c  _ZTV6MTitle + _ZTI6MTitle
  .rodata                         0x008  _ZTS6MTitle
  .rodata._ZTSN11AbyssEngine18IApplicationModuleE  0x024  (COMDAT group)
  .data.rel.ro._ZTIN11AbyssEngine18IApplicationModuleE  0x008 (group)

-fdata:
  .data.rel.ro._ZTV6MTitle        0x050
  .rodata._ZTS6MTitle             0x008
  .rodata._ZTSN11AbyssEngine18IApplicationModuleE  0x024 (group)
  .data.rel.ro._ZTIN11AbyssEngine18IApplicationModuleE  0x008 (group)
  .data.rel.ro._ZTI6MTitle        0x00c  <-- now after the group
```

Exact original addresses:

| symbol | original | baseline | app `-fdata-sections` |
|---|---:|---:|---:|
| `_ZTV6MTitle` | `0x20d7d0` | `0x20d7d0` | `0x20d7d0` |
| `_ZTI6MTitle` | `0x20d820` | `0x20d820` | `0x20d828` |
| `_ZTIN11AbyssEngine18IApplicationModuleE` | `0x20d82c` | `0x20d82c` | `0x20d820` |

The original again matches the shared-section order. This is a game TU, so
the result is not an engine-library-only artifact.

## 5. `.data` witness: `NFC::interface_path` and `VirtualInput::keys`

The first `.data` divergence is also structural, not a global shift:

| symbol | original | baseline | app `-fdata-sections` | app `-fdata`, no `.data*` line |
|---|---:|---:|---:|---:|
| `_ZN3NFC14interface_pathE` | `0x214004` | `0x214004` | `0x21405c` | `0x214000` |
| `keys` (`VirtualInput.cpp`) | `0x214010` | `0x214010` | `0x214060` | `0x214004` |

Removing the section-ordering file's `.data*` line recovers the `.data`
section size (`0x296c`) but not the order: the two symbols still move to
`0x214000`/`0x214004` instead of `0x214004`/`0x214010`. The remaining byte
difference is intrinsic to per-symbol data sections.

## 6. Why the ndk-build default is not a contradiction

The r18b ndk-build default is real:

```text
.cache/ndk/android-ndk-r18b/build/core/default-build-commands.mk:135
GLOBAL_CFLAGS = \
    -target $(LLVM_TRIPLE)$(TARGET_PLATFORM_LEVEL) \
    -fdata-sections \
    -ffunction-sections \
    ...
```

But it is only the **default**. In `definitions.mk`, the compile command is:

```text
_FLAGS := \
    $(GLOBAL_CFLAGS) \
    $(TARGET_CFLAGS) \
    ...
    $(NDK_APP_CFLAGS) \
    ...
    $(LOCAL_CFLAGS) \
    ...
```

`NDK_APP_CFLAGS` and `LOCAL_CFLAGS` come **after** `GLOBAL_CFLAGS`, so a
`-fno-data-sections` in either one overrides the default. A controlled
compile with both flags confirms the override is exact:

```sh
# same AEFile command, with both flags
... -fdata-sections -fno-data-sections -ffunction-sections ...
cmp /tmp/w3691/AEFile_no.o /tmp/w3691/AEFile_both.o
# exit 0: byte-identical objects
```

No original `Android.mk`/`Application.mk` survives, so we cannot prove
*which* override the original used. The binary proves the **effective** flag
was off; the surviving build-system default does not override the binary.

The older note `build_system_evidence.md` §6.5 says "the original definitely
had `-fdata-sections`". That sentence should be read as "stock r18b ndk-build
would pass `-fdata-sections` by default", not as a binary observation. The
binary observation is the opposite, and it is the one that matters for
byte-matching.

## 7. Proven vs hypothesis

**Proven by controlled rebuilds at `88aaf59f`:**

1. `-fdata-sections` is a regression: app-only **4496 -> 4333** (-163),
   with `linked_exact` unchanged at 4496, size 4524/4524, data 843/843,
   stubs/missing/extra 0, imports 207.
2. Removing the section-ordering file's `.data*` line gives **4334** (one
   function recovered), so 162 of the 163 losses are intrinsic to per-symbol
   `.rodata` sections.
3. The original's `.rodata`, `.data` and `.data.rel.ro` are 100 % byte-identical
   to the non-`-fdata` baseline; the `-fdata` build differs in 42.3 %, 30.9 %
   and 3.3 % respectively.
4. `AEFile.cpp` (engine) and `MTitle.cpp` (game) both have exact original
   symbol addresses matching the shared-section order. In both TUs,
   `-fdata-sections` moves a COMDAT group between the derived class's vtable
   and typeinfo; the original does not have that move.
5. `-fno-data-sections` after `-fdata-sections` produces a byte-identical
   object to the no-`-fdata` compile (`cmp` exit 0), so the ndk-build default
   can be overridden exactly as the binary requires.

**Still hypothesis / not settled:**

1. *Why* the original's effective flag was off. Candidates: an explicit
   `-fno-data-sections` in `LOCAL_CFLAGS`/`NDK_APP_CFLAGS`; a non-ndk-build
   build system; or a prebuilt engine library built without the flag plus game
   code built with it. The AEFile and MTitle witnesses show both engine and
   game TUs match the shared-section layout, so a mixed per-library build is
   not required by the data.
2. Whether the original used a section-ordering file that happened to undo
   the per-symbol order. The binary alone cannot distinguish "flag off" from
   "flag on plus a data-ordering file"; the flag-off explanation is the
   parsimonious one and is consistent with every section and symbol address
   checked here.
3. The exact original `Android.mk`/`Application.mk` is still not recovered.

## 8. Reproduction summary

```sh
# baseline
cmake --preset default
cmake --build cmake-build-match --target check -j8       # 4496 / 4496
cmake --build cmake-build-match --target lint  -j8       # CLEAN
cmake --build cmake-build-match --target drift -j8       # 0 fields

# app-only -fdata-sections
cat > /tmp/w3691/toolchain_extra.cmake <<'TOOLCHAIN_EOF'
include("/home/debian/GalaxyOnFire2/wt/w3691/cmake/toolchain.cmake")
set(GOF2_EXTRA_FLAGS "" CACHE STRING "extra matching flags")
if(GOF2_EXTRA_FLAGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GOF2_EXTRA_FLAGS}" CACHE STRING "" FORCE)
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${GOF2_EXTRA_FLAGS}" CACHE STRING "" FORCE)
endif()
TOOLCHAIN_EOF
cmake -S . -B /tmp/w3691/build-fdata -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/tmp/w3691/toolchain_extra.cmake \
  -DGOF2_MATCH_OPT=-Oz -DGOF2_EXTRA_FLAGS='-fdata-sections'
cmake --build /tmp/w3691/build-fdata --target link_so -j8
python3 tools/verify/verify.py \
  --build-dir /tmp/w3691/verify-fdata \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3691/build-fdata/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
# 4333 / 4496

# object-level AEFile / MTitle experiments: see §3 and §4
```

Section payload scan used in §2:

```python
import struct

def sec(path, name):
    d = open(path, 'rb').read()
    shoff, = struct.unpack_from('<I', d, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from('<HHH', d, 0x2e)
    hdrs = [struct.unpack_from('<IIIIIIIIII', d, shoff + i * shentsize)
            for i in range(shnum)]
    stroff = hdrs[shstrndx][4]
    for h in hdrs:
        n = d[stroff + h[0]:d.index(b'\0', stroff + h[0])].decode()
        if n == name:
            return d[h[4]:h[4] + h[5]], h[3]

for name in ('.rodata', '.data', '.data.rel.ro'):
    o, oa = sec('../_work/bins/android_2.0.16_libgof2hdaa.so', name)
    for label, path in (
        ('base', 'cmake-build-match/libgof2hdaa.so'),
        ('fdata', '/tmp/w3691/build-fdata/libgof2hdaa.so'),
    ):
        s, sa = sec(path, name)
        n = min(len(o), len(s))
        same = sum(1 for i in range(n) if o[i] == s[i])
        i = 0
        while i < n and o[i] == s[i]:
            i += 1
        print(f'{name} {label}: size {len(s):#x} same {same}/{n} firstdiff +{i:#x}')
```
