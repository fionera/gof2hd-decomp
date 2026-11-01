# `-fdata-sections` is negative: data-layout fidelity (w3439)

Status: research note, 2026-11-01. Worktree `wt/w3439`, branch `wave87-w3439`,
HEAD `4de520d5`. Every number below is from a command run against
`../_work/bins/android_2.0.16_libgof2hdaa.so` or from a controlled A/B rebuild of
this tree. This note extends `_work/reconstructions/fdata_sections_retest.md`
(measured at `d00e0030`, byte 4182 / linked 4495) to the current tree.

## 0. Bottom line

At HEAD the baseline is **byte 4250 / linked 4496**, with every ELF section
size- and base-exact. Adding `-fdata-sections` is a regression:

| variant | byte | linked | size | `.rodata` | `.data` | `.bss` |
|---|---:|---:|---:|---:|---:|---:|
| baseline (no `-fdata-sections`) | **4250** | 4496 | 4523/4524 | `0x41b67` (exact) | `0x296c` (exact) | `0x216970`/`0x69ac` (exact) |
| app TUs only | **4150** (−100) | 4496 | 4523/4524 | `0x41a27` (−0x140) | `0x2962` (−0xa) | `0x216970`/`0x69ac` |
| vendored zlib/libzip only | **4249** (−1) | 4496 | 4523/4524 | `0x41b67` | `0x295e` (−0xe) | `0x216960`/`0x69ac` |
| app + vendored | **4153** (−97) | 4496 | 4523/4524 | `0x41a27` (−0x140) | `0x295e` (−0xe) | `0x216960`/`0x69ac` |
| `-fno-data-sections` control | **4250** | 4496 | 4523/4524 | `0x41b67` | `0x296c` | `0x216970`/`0x69ac` |

No variant is a strict improvement, so no flag change is committed. The
original's `.rodata`/`.data` layout is the **non-`-fdata-sections`** layout.

The coordinator's stated baseline (byte 4182 / linked 4495) is `d00e0030`, the
base of the earlier note. The current branch has advanced by +68 byte_exact /
+1 linked. The negative effect grew with the tree: app-only was −71 at
`d00e0030` and is −100 at HEAD; vendored-only was neutral and is now −1. That is
consistent with the mechanism in §4: more functions now have byte-exact
literal-pool words under the non-`-fdata` layout, and `-fdata-sections` moves
those data addresses away.

## 1. Baseline at HEAD

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
cmake --build cmake-build-match --target lint  -j8
```

```text
verify: byte 4250  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
=== DRIFT: 0 field(s) across 0 class(es) ===
CLEAN ✓ (no new soft violations; hard invariants 0)
```

`report.json`: 4523/4524 size-exact (the known `Generator::createMission` −4 B
row). Baseline `.so` sha256:

```text
530e70b7da6f7d1e340c7c9e3276dff761f3952647618af6d3e2c9941ce29d4f  cmake-build-match/libgof2hdaa.so
```

Baseline section table (all equal to the original):

| section | addr | size |
|---|---:|---:|
| `.text` | `0x69440` | `0x1337c8` |
| `.rodata` | `0x1c9e10` | `0x41b67` |
| `.data.rel.ro` | `0x20cbb0` | `0x2fe8` |
| `.data` | `0x214000` | `0x296c` |
| `.bss` | `0x216970` | `0x69ac` |

## 2. A/B method and gate numbers

`cmake/toolchain.cmake` `FORCE`s `CMAKE_CXX_FLAGS`/`CMAKE_C_FLAGS`, so the
extra flag is appended by a wrapper toolchain:

```sh
cat > /tmp/w3439/extra_toolchain.cmake <<'TOOLCHAIN_EOF'
include("/home/debian/GalaxyOnFire2/wt/w3439/cmake/toolchain.cmake")
set(GOF2_EXTRA_FLAGS "" CACHE STRING "extra matching flags")
if(GOF2_EXTRA_FLAGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GOF2_EXTRA_FLAGS}" CACHE STRING "matching ARM compile flags" FORCE)
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${GOF2_EXTRA_FLAGS}" CACHE STRING "matching ARM compile flags" FORCE)
endif()
TOOLCHAIN_EOF
```

App TUs only:

```sh
cmake -S . -B /tmp/w3439/fdata-app2 -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/tmp/w3439/extra_toolchain.cmake \
  -DGOF2_MATCH_OPT=-Oz -DGOF2_EXTRA_FLAGS='-fdata-sections'
cmake --build /tmp/w3439/fdata-app2 --target check -j8
```

Vendored zlib/libzip are compiled by `third_party/build_match_lib.cmake` with
its own `CC_BASE`, not the CMake flags. For the vendored-only row, add
`-fdata-sections` to `CC_BASE` (line 27) and build:

```sh
cp third_party/build_match_lib.cmake /tmp/w3439/build_match_lib.cmake.pre
python3 - <<'PY'
p = 'third_party/build_match_lib.cmake'
s = open(p).read()
s = s.replace('    -fPIC -ffunction-sections -funwind-tables -Wno-everything)',
              '    -fPIC -ffunction-sections -fdata-sections -funwind-tables -Wno-everything)')
open(p, 'w').write(s)
PY
cmake -S . -B /tmp/w3439/fdata-vend2 -G Ninja -DGOF2_MATCH_OPT=-Oz
cmake --build /tmp/w3439/fdata-vend2 --target check -j8
```

The app+vendored row combines both:

```sh
cmake -S . -B /tmp/w3439/fdata-both2 -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/tmp/w3439/extra_toolchain.cmake \
  -DGOF2_MATCH_OPT=-Oz -DGOF2_EXTRA_FLAGS='-fdata-sections'
cmake --build /tmp/w3439/fdata-both2 --target check -j8
```

The control row:

```sh
cmake -S . -B /tmp/w3439/fnodata -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/tmp/w3439/extra_toolchain.cmake \
  -DGOF2_MATCH_OPT=-Oz -DGOF2_EXTRA_FLAGS='-fno-data-sections'
cmake --build /tmp/w3439/fnodata --target check -j8
```

The vendored builder writes `third_party/<lib>/lib*_gof2.a` into the source
tree. Build the rows sequentially. Restore the baseline afterwards:

```sh
cp /tmp/w3439/build_match_lib.cmake.pre third_party/build_match_lib.cmake
cmake --build cmake-build-match --target link_so -j8
cmake --build cmake-build-match --target check -j8   # back to 4250 / 4496
```

Every variant prints the same gate fields except `byte_exact`:

```text
stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
size:   4523/4524
drift:  0 fields
lint:   CLEAN ✓
```

The `check` target reports `RATCHET FAIL` for the `-fdata` rows because the
ratchet compares against the 4250 baseline; that is the expected measurement,
not a build failure.

## 3. What loses byte_exactness

All 100 app-only losses are rows that are `pct == 100.0` in both reports:
**0 instruction-stream changes, 0 gains**. The only changed bytes are
literal-pool words. `_work/reconstructions/fdata_sections_w3439.py` (committed
alongside this note) disassembles each lost function, resolves every PIC
`ldr rX, [pc, #N]` / `add rX, pc` pair to its destination, and classifies the
differing words:

```text
losses: 100  pct_changed: 0
pool words by destination section: {('.data', '.data'): 7, ('.rodata', '.rodata'): 194}
functions by destination section:   {('.data', '.data'): 4, ('.rodata', '.rodata'): 96}
total pool words: 201
```

Every one of the 201 words resolves to the **same logical object** in both
binaries (`pic-same`): the same shader source string, the same global, the same
table. Only its address moved. Examples:

* `_Z31SwitchToOtherMouseConifgurationi` (16 B, `.text` 0x6a03c). The
  instruction stream is identical; the pool word at 0x6a048 is
  `0x001ab05e` in the original/baseline and `0x001ab0ae` in the app
  `-fdata` build. Both resolve to `keys + 0x90`: 0x2140a0 in the original
  and 0x2140f0 in `-fdata`.
* `_ZN11AbyssEngine10BlurShader4InitEPNS_6EngineE` (188 B, `.text` 0x8a68c).
  The instruction stream is identical; the two pool words at 0x8a71c/0x8a720
  are `0x00161dd6`/`0x00161f0c` in the original/baseline and
  `0x00161d84`/`0x00161eba` in `-fdata`. They resolve to the same shader
  source strings at 0x1ec470/0x1ec4a0 vs 0x1ec41e/0x1ec44e.

The 96 `.rodata` functions are dominated by the shader `Init` cluster (each
`Init` loads its vertex/fragment source strings from `.rodata`), plus
`GameText`, `ModStation`, `RepairBeam`, `SpaceLounge`, `DialogueWindow`,
`Generator`, `Station`, `Status`, `PlayerEgo`, zlib and a handful of others.
The 4 `.data` functions are the three input globals
(`SwitchToOtherMouseConifguration`, `ActualizeMouseVisibilty`,
`setValuesForGamepad`) and zlib's `_tr_init`.

## 4. Why: `-fdata-sections` changes the data order

`-fdata-sections` moves every data symbol into its own input section
(`.rodata.<sym>`, `.data.<sym>`, `.bss.<sym>`). Gold then concatenates the
per-symbol sections by its orphan-section placement instead of concatenating
the one shared `.rodata`/`.data` section clang emits without the flag. The
section-ordering file does not list `.rodata` at all (it is a gold orphan at
index 0), and its `.data*`/`.bss*` trailing entries group the per-symbol
`.data.*` sections after the plain `.data` block. The result is a different
global data order, so every PC-relative pool word pointing into `.rodata` or
`.data` changes.

### 4.1 Structural example: `AEFile.cpp` RTTI

The original and the baseline have the same RTTI string order in `.rodata`:

```text
orig / baseline:
  0x1e7998  "9AEPakFile\0"
  0x1e79a3  "12AENormalFile\0"
  0x1e79c0  "14AELowLevelFile\0"     (16-aligned)
```

The app `-fdata` build moves `12AENormalFile` after the 16-aligned
`14AELowLevelFile` group:

```text
app -fdata-sections:
  0x1e7998  "9AEPakFile\0"
  0x1e79b0  "14AELowLevelFile\0"     (16-aligned)
  0x1e79c1  "12AENormalFile\0"
```

The object sections show why (`readelf -SW` on
`CMakeFiles/gof2.dir/src/engine/file/AEFile.cpp.obj`):

```text
baseline:
  .rodata                       0x41a  crc32Table + _ZTS9AEPakFile + _ZTS12AENormalFile
  .rodata.str1.1                0x007
  .rodata._ZTS14AELowLevelFile  0x011  (group, align 16)

-fdata-sections:
  .rodata._ZN12_GLOBAL__N_110crc32TableE  0x400
  .rodata.str1.1                          0x007
  .rodata._ZTS9AEPakFile                  0x00b
  .rodata._ZTS14AELowLevelFile            0x011  (group, align 16)
  .rodata._ZTS12AENormalFile              0x00f  <-- after the group
```

The original has the two plain RTTI strings packed contiguously before the
16-aligned group, i.e. exactly clang's non-`-fdata` shared-`.rodata` emission.
This is a compiler-emission-order observable, not a string-tuning artifact:
the source order of the class definitions is unchanged between the two builds.

### 4.2 Structural example: `.data` globals

`NFC::interface_path` and `VirtualInput`'s `keys` array are in `.data` in the
original and baseline:

```text
symbol                         original / baseline   app -fdata-sections
_ZN3NFC14interface_pathE       0x214004              0x21405c
keys (VirtualInput.cpp)        0x214010 (4224 B)     0x214060 (4224 B)
```

`NFC.cpp`'s object has one `.data` section in the baseline and a
`.data._ZN3NFC14interface_pathE` section in `-fdata`; `VirtualInput.cpp`'s
`.data` becomes `.data.keys` plus several `.data.*` sections. The ordering
file's `.data*` entry groups those per-symbol sections after the plain `.data`
block, moving both globals. The original's `.data` bytes match the non-`-fdata`
order for the first 0x8d0 bytes; the `-fdata` build differs from byte 4.

### 4.3 Content comparison

Matching bytes against the original (section payload scan; “first diff” is
section-relative, with the VMA in parentheses):

| section | baseline | app `-fdata-sections` |
|---|---|---|
| `.rodata` | **255,851 / 269,159** same, first diff `+0x32580` (VMA 0x1fc390) | **154,723 / 268,839** same, first diff `+0x1db93` (VMA 0x1e79a3) |
| `.data` | **8,978 / 10,604** same, first diff `+0x8d0` (VMA 0x2148d0) | **7,167 / 10,594** same, first diff `+0x4` (VMA 0x214004) |
| `.data.rel.ro` | **11,364 / 12,264** same, first diff `+0xe10` (VMA 0x20d9c0) | **11,168 / 12,264** same, first diff `+0x68` (VMA 0x20cc18) |

The decisive measurement is the `.rodata` window where the baseline is
byte-identical to the original, `[0x1db93, 0x32580)` section-relative
(84,461 bytes). The app `-fdata` build differs in **77,360 bytes (91.6%)** of
that window. Over the whole 206,208-byte baseline==original prefix it differs
in 77,360 bytes (37.5%). In other words, the original agrees with the
non-`-fdata` bytes across the region where `-fdata` moves everything.

Vendored-only `-fdata` changes `.rodata` only at the zlib/libzip block
(`+0x402b7`), lowers matching bytes 255,851 → 254,999 (the first diff is
unchanged at `+0x32580`; the vendored block is after it), and changes `.data`
from 8,978 to 8,959 matching bytes. It is gate-neutral-to-−1 and structurally
worse.

### 4.4 The section-ordering file is not the cause

The app `-fdata` objects were relinked with the literal `.data*` and `.bss*`
lines removed from `section_order.txt`:

```sh
grep -v -x -F -e '.data*' -e '.bss*' \
  /tmp/w3439/fdata-app2/section_order.txt > /tmp/w3439/order-app2-nodata.txt
# exact gold command from: ninja -C /tmp/w3439/fdata-app2 -t commands libgof2hdaa.so
# only --section-ordering-file and -o changed
python3 tools/verify/verify.py \
  --build-dir /tmp/w3439/verify-app2-nodata \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3439/fdata-app2-nodata.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
```

Result: **byte 4150 / linked 4496**, exactly the same 100 losses as the
unmodified ordering file. The `.data`/`.bss` grouping accounts for none of the
loss; the `.rodata` per-symbol reorder is intrinsic to the flag.

## 5. Proven vs hypothesis

**Proven by controlled rebuilds at HEAD `4de520d5`:**

1. `-fdata-sections` is a regression: app-only **4250 → 4150** (−100),
   app+vendored **4250 → 4153** (−97), vendored-only **4250 → 4249** (−1).
   `linked_exact` stays 4496; `size` stays 4523/4524; stubs/missing/extra stay
   0; lint is CLEAN and drift is 0 in every variant.
2. All 100 app-only losses are `pct == 100.0` rows: the instruction stream is
   unchanged. The 201 differing words are literal-pool words: 194 to
   `.rodata` (96 functions) and 7 to `.data` (4 functions), all resolving to
   the same logical object at a different address.
3. App `-fdata` shrinks `.rodata` by 0x140 and `.data` by 0xa; vendored
   `-fdata` shrinks `.data` by 0xe and moves `.bss` from 0x216970 to
   0x216960. The original's sizes/addresses are the non-`-fdata` ones.
4. The first app `-fdata` divergence is the `AEFile.cpp` RTTI block at VMA
   0x1e79a3; the original equals the non-`-fdata` bytes there. In the 84,461-
   byte `.rodata` window where the baseline equals the original, `-fdata`
   differs in 91.6% of bytes.
5. The loss is not caused by the section-ordering file's `.data`/`.data*`
   split: removing those entries still gives 4150 and the same 100 losses.
6. `-fno-data-sections` is neutral (4250/4496), confirming the flag is off by
   default and that the baseline is not accidentally relying on an implicit
   `-fdata-sections`.

**Still hypothesis / not settled:**

1. *Why* the original lacks the flag. The binary is consistent with a
   non-`-fdata-sections` compile, but the original build system is not
   recovered. Candidates: the app was not built by stock ndk-build
   `GLOBAL_CFLAGS`; it passed `-fno-data-sections`; or its TU/source order
   happens to re-interleave the per-symbol sections exactly as the shared
   `.rodata`/`.data` emission. The last is possible but not parsimonious, and
   the AEFile RTTI group order is a direct counterexample for our TU.
2. The content comparison is partly by construction: earlier data-layout
   passes tuned the tree's `.rodata` against the original under the
   non-`-fdata` flag. The gate result (§2) is independent of that tuning; the
   structural AEFile/.data evidence (§4.1–4.2) is the part that does not rely
   on tuned string order.
3. `-fdata-sections` could become positive again if a future TU/data-layout
   change makes the tree's data content match the original's *per-symbol*
   order. Re-test then. Today it is a regression, not an authenticity win.

## 6. Reproduction summary

```sh
# baseline
cmake --preset default
cmake --build cmake-build-match --target check -j8       # 4250 / 4496
cmake --build cmake-build-match --target drift -j8       # 0 fields
cmake --build cmake-build-match --target lint  -j8       # CLEAN

# app-only -fdata-sections
cat > /tmp/w3439/extra_toolchain.cmake <<'TOOLCHAIN_EOF'
include("/home/debian/GalaxyOnFire2/wt/w3439/cmake/toolchain.cmake")
set(GOF2_EXTRA_FLAGS "" CACHE STRING "extra matching flags")
if(GOF2_EXTRA_FLAGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GOF2_EXTRA_FLAGS}" CACHE STRING "" FORCE)
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${GOF2_EXTRA_FLAGS}" CACHE STRING "" FORCE)
endif()
TOOLCHAIN_EOF
cmake -S . -B /tmp/w3439/fdata-app2 -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/tmp/w3439/extra_toolchain.cmake \
  -DGOF2_MATCH_OPT=-Oz -DGOF2_EXTRA_FLAGS='-fdata-sections'
cmake --build /tmp/w3439/fdata-app2 --target check -j8   # 4150 / 4496

# vendored-only: append -fdata-sections to CC_BASE in third_party/build_match_lib.cmake
cmake -S . -B /tmp/w3439/fdata-vend2 -G Ninja -DGOF2_MATCH_OPT=-Oz
cmake --build /tmp/w3439/fdata-vend2 --target check -j8  # 4249 / 4496

# app + vendored
cmake -S . -B /tmp/w3439/fdata-both2 -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/tmp/w3439/extra_toolchain.cmake \
  -DGOF2_MATCH_OPT=-Oz -DGOF2_EXTRA_FLAGS='-fdata-sections'
cmake --build /tmp/w3439/fdata-both2 --target check -j8  # 4153 / 4496

# control
cmake -S . -B /tmp/w3439/fnodata -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/tmp/w3439/extra_toolchain.cmake \
  -DGOF2_MATCH_OPT=-Oz -DGOF2_EXTRA_FLAGS='-fno-data-sections'
cmake --build /tmp/w3439/fnodata --target check -j8      # 4250 / 4496

# classification (committed alongside this note)
python3 _work/reconstructions/fdata_sections_w3439.py \
  --fdata /tmp/w3439/fdata-app2/libgof2hdaa.so \
  --fdata-report /tmp/w3439/fdata-app2/verify/report.json
```

```text
losses: 100  pct_changed: 0
pool words by destination section: {('.data', '.data'): 7, ('.rodata', '.rodata'): 194}
functions by destination section:   {('.data', '.data'): 4, ('.rodata', '.rodata'): 96}
total pool words: 201
```

Section/content scan used in §4.3 (run from the repo root):

```python
import struct
def sec(path, name):
    d = open(path, 'rb').read()
    shoff, = struct.unpack_from('<I', d, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from('<HHH', d, 0x2e)
    hdrs = [struct.unpack_from('<IIIIIIIIII', d, shoff + i * shentsize) for i in range(shnum)]
    stroff = hdrs[shstrndx][4]
    for h in hdrs:
        n = d[stroff + h[0]:d.index(b'\0', stroff + h[0])].decode()
        if n == name:
            return d[h[4]:h[4] + h[5]]
for name in ('.rodata', '.data', '.data.rel.ro'):
    o = sec('../_work/bins/android_2.0.16_libgof2hdaa.so', name)
    b = sec('cmake-build-match/libgof2hdaa.so', name)
    f = sec('/tmp/w3439/fdata-app2/libgof2hdaa.so', name)
    for lbl, s in (('base', b), ('fdata', f)):
        n = min(len(o), len(s))
        same = sum(1 for i in range(n) if o[i] == s[i])
        i = 0
        while i < n and o[i] == s[i]:
            i += 1
        print(f'{name} {lbl}: size {len(s):#x} same {same}/{n} firstdiff +{i:#x}')
```
