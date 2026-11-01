# `-fdata-sections` retest (w3423)

Status: research note, 2026-11-01. Worktree `wt/w3423`, branch `wave87-w3423`,
base `d00e0030`. Every number below is from a command run against
`../_work/bins/android_2.0.16_libgof2hdaa.so` or from a controlled A/B rebuild of
this tree. It supersedes the `-fdata-sections` rows in `build_system_evidence.md`
§6.5, `global_flags_sweep.md` §4 and `build_system_archaeology_w3413.md` §4, which
were measured before the data-layout passes made every ELF section size/base exact.

## 0. Bottom line

At the current tree state (`byte 4182 / linked 4495`, every section size+base exact)
**`-fdata-sections` is negative and must not be added**:

| variant | byte | linked | `.rodata` | `.data` | `.bss` |
|---|---:|---:|---:|---:|---:|
| baseline (no `-fdata-sections`) | **4182** | 4495 | `0x41b67` (exact) | `0x296c` (exact) | `0x216970` (exact) |
| app TUs only | **4111** (−71) | 4495 | `0x41a27` (−0x140) | `0x2962` (−0xa) | `0x216970` |
| vendored zlib/libzip only | **4182** (neutral) | 4495 | `0x41b67` | `0x295e` (−0xe) | `0x216960` (−0x10) |
| app + vendored | **4113** (−69) | 4495 | `0x41a27` (−0x140) | `0x295e` (−0xe) | `0x216960` (−0x10) |

No variant is a strict improvement, so this pass commits only this note.

The old hypothesis "the original is stock r18b ndk-build, whose `GLOBAL_CFLAGS`
always contains `-fdata-sections`" is **contradicted by the binary's data layout**
(§4). The original's `.rodata`/`.data`/`.data.rel.ro` byte content is the
non-`-fdata-sections` layout, not the `-fdata-sections` one.

## 1. Baseline

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
cmake --build cmake-build-match --target lint  -j8
```

```text
verify: byte 4182  linked 4495  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
=== DRIFT: 0 field(s) across 0 class(es) ===
CLEAN ✓ (no new soft violations; hard invariants 0)
```

`report.json`: 4523/4524 size-exact (the known `Generator::createMission` −4 B row).
Baseline `.so` sha256:

```text
c3342b6dc4b1fdaa85d9282d7832432544655ac9bd6f29a608772d80e7995470  cmake-build-match/libgof2hdaa.so
```

Baseline section table (all equal to the original):

| section | addr | size |
|---|---:|---:|
| `.text` | `0x69440` | `0x1337c8` |
| `.rodata` | `0x1c9e10` | `0x41b67` |
| `.data.rel.ro` | `0x20cbb0` | `0x2fe8` |
| `.data` | `0x214000` | `0x296c` |
| `.bss` | `0x216970` | `0x69ac` |

## 2. A/B method

`cmake/toolchain.cmake` `FORCE`s `CMAKE_CXX_FLAGS`/`CMAKE_C_FLAGS`, so the extra flag
is appended by a wrapper toolchain (same method as `global_flags_sweep.md`):

```sh
cat > /tmp/w3423/extra_toolchain.cmake <<'TOOLCHAIN_EOF'
include("/home/debian/GalaxyOnFire2/wt/w3423/cmake/toolchain.cmake")
set(GOF2_EXTRA_FLAGS "" CACHE STRING "extra matching flags")
if(GOF2_EXTRA_FLAGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GOF2_EXTRA_FLAGS}" CACHE STRING "matching ARM compile flags" FORCE)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GOF2_EXTRA_FLAGS}" CACHE STRING "matching ARM compile flags" FORCE)
endif()
TOOLCHAIN_EOF
```

App TUs:

```sh
cmake -S . -B /tmp/w3423/fdata-app -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/tmp/w3423/extra_toolchain.cmake \
  -DGOF2_MATCH_OPT=-Oz -DGOF2_EXTRA_FLAGS='-fdata-sections'
cmake --build /tmp/w3423/fdata-app --target check -j8
```

Vendored zlib/libzip are compiled by `third_party/build_match_lib.cmake` with its own
`CC_BASE`, not the CMake flags. For that row, append `-fdata-sections` to `CC_BASE`
(line 27) and rebuild; libcrypto uses `OSSL_FLAGS` (GCC 4.9) and is unaffected:

```sh
cmake -S . -B /tmp/w3423/fdata-vendored -G Ninja -DGOF2_MATCH_OPT=-Oz
cmake --build /tmp/w3423/fdata-vendored --target check -j8
```

The app+vendored row combines both. Builds must be run **sequentially** (the vendored
archives are written into the source tree). The `CC_BASE` edit was reverted and the
baseline relinked afterwards; `cmake-build-match/libgof2hdaa.so` is back to sha256
`c3342b6d…` and `check` still prints `byte 4182 / linked 4495 / RATCHET PASS`.

## 3. Gate and section table

`.so` sha256 and section sizes (`readelf -SW`), original for comparison:

| variant | sha256 | `.text` | `.rodata` | `.data.rel.ro` | `.data` | `.bss` addr/size |
|---|---|---|---:|---:|---:|---:|---:|
| original | `d4c17be6…` | `0x1337c8` | `0x41b67` | `0x2fe8` | `0x296c` | `0x216970`/`0x69ac` |
| baseline | `c3342b6d…` | `0x1337c8` | `0x41b67` | `0x2fe8` | `0x296c` | `0x216970`/`0x69ac` |
| app only | `61664a27…` | `0x1337c8` | `0x41a27` | `0x2fe8` | `0x2962` | `0x216970`/`0x69ac` |
| vendored only | `b02a5ec7…` | `0x1337c8` | `0x41b67` | `0x2fe8` | `0x295e` | `0x216960`/`0x69ac` |
| app + vendored | `04674297…` | `0x1337c8` | `0x41a27` | `0x2fe8` | `0x295e` | `0x216960`/`0x69ac` |

`.text` never changes (function code is unaffected; the flag is data-only). The
`-fdata-sections` runs break the previously exact `.rodata`/`.data` sizes; the
vendored-only run is gate-neutral but moves `.bss` 0x10 away from the original.

### 3.1 Which functions lose byte_exactness

All 71 losses in the app-only run are rows that were `pct == 100.0` but not `byte`
(the literal-pool set of `literal_pool_audit.md`); there are **0 gains**. The flag
shifts `.rodata`/`.data` addresses, so every PIC pool word that points into those
sections changes. Examples: `_ZN11AbyssEngine10BlurShader4InitEPNS_6EngineE`,
`_ZN10ModStation11checkMedalsEv`, `_Z19setValuesForGamepadff`.

### 3.2 Ordering-file confound is ruled out

The section-ordering file lists `.data` (early) and `.data*`/`.bss*` (trailing). With
`-fdata-sections` every app data symbol becomes `.data.<sym>` and therefore moves
behind the archive `.data` block (`_ZN3NFC14interface_pathE` moves
`0x214004 → 0x21405c`). To check whether that interaction causes the loss, the app-only
objects were relinked with `.data*`/`.bss*` deleted from `section_order.txt`:

```sh
grep -v -x -e '.data*' -e '.bss*' /tmp/w3423/fdata-app/section_order.txt > /tmp/w3423/order_nodata.txt
# relink the fdata-app gold command with --section-ordering-file=/tmp/w3423/order_nodata.txt
python3 tools/verify/verify.py --build-dir /tmp/w3423/verify-diag \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so --ours /tmp/w3423/fdata-app-diag.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
```

Result: **byte 4111 / linked 4495**, identical to the unmodified-ordering-file run.
The negative result is not an artifact of the ordering file's data grouping.

## 4. Why: the original's data layout is the non-`-fdata-sections` layout

`-fdata-sections` moves every data symbol into its own input section. Gold then
concatenates the per-symbol sections in the object's section-header order instead of
the order clang writes them into a shared `.rodata`. This changes the interleaving with
COMDAT/group sections (RTTI/typeinfo), which are emitted at their own positions.

Concrete first divergence, `AEFile.cpp` (vma `0x1e7998`):

```text
original / baseline:
  ... 9AEPakFile\0 12AENormalFile\0 <padding> 14AELowLevelFile\0 ...
  (0x1e7998)      (0x1e79a3)                  (0x1e79c0, 16-aligned)

app -fdata-sections:
  ... 9AEPakFile\0 <padding> 14AELowLevelFile\0 12AENormalFile\0 ...
  (0x1e7998)                 (0x1e79b0, 16-aligned) (0x1e79c1)
```

The object sections that produce this (`readelf -SW .../AEFile.cpp.obj`):

```text
baseline:        .rodata (0x41a: crc32Table + _ZTS9AEPakFile + _ZTS12AENormalFile)
                 .rodata.str1.1
                 .rodata._ZTS14AELowLevelFile (align 16, group)

-fdata-sections: .rodata._ZN12_GLOBAL__N_110crc32TableE
                 .rodata.str1.1
                 .rodata._ZTS9AEPakFile
                 .rodata._ZTS14AELowLevelFile (align 16, group)
                 .rodata._ZTS12AENormalFile        <-- now after the group
```

The original has the two plain RTTI strings packed contiguously before the 16-aligned
`AELowLevelFile` group section — i.e. exactly the non-`-fdata-sections` emission. The
order is fixed by the source order in `AEFile.cpp` (`AEPakFile`'s key function is
defined before `AENormalFile`'s), not by string-literal tuning.

### 4.1 Controlled content comparison

Same source, same link, only the compile flag differs. Matching bytes against the
original (payload scan of the section):

| section | baseline | app `-fdata-sections` |
|---|---|---|
| `.rodata` | **254519 / 269159** same, first diff `+0x31780` | **154759 / 268839** same, first diff `+0x1db93` |
| `.data` | **8978 / 10604** same, first diff `+0x8d0` | **7167 / 10594** same, first diff `+0x4` |
| `.data.rel.ro` | **11354 / 12264** same, first diff `+0xe10` | **11158 / 12264** same, first diff `+0x68` |

The decisive measurement: in the **80,877-byte region `[0x1db93, 0x31780)`** where the
original is byte-identical to the baseline (non-`-fdata`) output, the
`-fdata-sections` output differs in **75,307 bytes (93%)**. The first byte where
`-fdata-sections` departs from the non-`-fdata` output is `0x1db93`, and the original
agrees with the non-`-fdata` byte there.

Vendored-only `-fdata-sections` also changes `.rodata` (only at `+0x402b7`, the
zlib/libzip block) and lowers matching bytes 254519 → 253667; `.data` 8978 → 8959.
That is why it is gate-neutral but structurally worse.

## 5. Proven vs hypothesis

**Proven by controlled rebuilds at `d00e0030`:**

1. `-fdata-sections` is not a gate improvement: app-only **4182 → 4111** (−71),
   app+vendored **4182 → 4113** (−69), vendored-only **4182 / 4495** (neutral).
   `linked_exact` stays 4495 in all three; `size` stays 4523/4524.
2. App `-fdata-sections` shrinks `.rodata` by `0x140` and `.data` by `0xa`; vendored
   `-fdata-sections` shrinks `.data` by `0xe` and moves `.bss` from `0x216970` to
   `0x216960`. The original's sizes/addresses are the non-`-fdata` ones.
3. The first content divergence is the `AEFile.cpp` RTTI block at vma `0x1e79a3`;
   the original equals the non-`-fdata` bytes, not the `-fdata` bytes. Across the
   80,877-byte region where the original equals the baseline, the `-fdata` build
   differs in 75,307 bytes.
4. The loss is not caused by the section-ordering file's `.data`/`.data*` split:
   relinking the `-fdata` objects with `.data*`/`.bss*` removed still gives 4111.

**Still hypothesis / not settled:**

1. *Why* the original lacks the ndk-build default. The binary is consistent with a
   non-`-fdata-sections` compile, but the build system is not recovered. Candidates:
   the app was not built by stock ndk-build `GLOBAL_CFLAGS`; or it passed
   `-fno-data-sections`; or its source/TU layout differs in a way that would
   re-interleave the per-symbol sections exactly as the non-`-fdata` build does.
   The last is possible but not parsimonious.
2. The content comparison is partly by construction: earlier data-layout passes tuned
   the tree's `.rodata` against the original under the non-`-fdata` flag. The gate
   result (§3) is independent of that tuning; the byte-content evidence (§4) is
   strongest for the `AEFile` RTTI block, which follows class declaration order.
3. `-fdata-sections` could become positive again if a future TU/data-layout change
   makes the tree's data content match the original's *per-symbol* order. Re-test then.
   Today it is a regression, not an authenticity win.

## 6. Reproduction summary

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8          # 4182 / 4495

cat > /tmp/w3423/extra_toolchain.cmake <<'TOOLCHAIN_EOF'
include("/home/debian/GalaxyOnFire2/wt/w3423/cmake/toolchain.cmake")
set(GOF2_EXTRA_FLAGS "" CACHE STRING "extra matching flags")
if(GOF2_EXTRA_FLAGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GOF2_EXTRA_FLAGS}" CACHE STRING "" FORCE)
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${GOF2_EXTRA_FLAGS}" CACHE STRING "" FORCE)
endif()
TOOLCHAIN_EOF

cmake -S . -B /tmp/w3423/fdata-app -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/tmp/w3423/extra_toolchain.cmake \
  -DGOF2_MATCH_OPT=-Oz -DGOF2_EXTRA_FLAGS='-fdata-sections'
cmake --build /tmp/w3423/fdata-app --target check -j8       # 4111 / 4495

# vendored-only: append -fdata-sections to CC_BASE in third_party/build_match_lib.cmake
cmake -S . -B /tmp/w3423/fdata-vendored -G Ninja -DGOF2_MATCH_OPT=-Oz
cmake --build /tmp/w3423/fdata-vendored --target check -j8  # 4182 / 4495 (neutral)

# section/content comparison (payload scan of .rodata/.data/.data.rel.ro)
python3 - <<'SCAN_EOF'
import struct
def sec(path,name):
    d=open(path,'rb').read()
    shoff,=struct.unpack_from('<I',d,0x20)
    shentsize,shnum,shstrndx=struct.unpack_from('<HHH',d,0x2e)
    hdrs=[struct.unpack_from('<IIIIIIIIII',d,shoff+i*shentsize) for i in range(shnum)]
    stroff=hdrs[shstrndx][4]
    for h in hdrs:
        n=d[stroff+h[0]:d.index(b'\0',stroff+h[0])].decode()
        if n==name: return d[h[4]:h[4]+h[5]], h[3], h[5]
for name in ('.rodata','.data','.data.rel.ro'):
    o,oa,_=sec('../_work/bins/android_2.0.16_libgof2hdaa.so',name)
    b,ba,_=sec('cmake-build-match/libgof2hdaa.so',name)
    f,fa,_=sec('/tmp/w3423/fdata-app/libgof2hdaa.so',name)
    for nm,s in (('base',b),('fdata',f)):
        n=min(len(o),len(s)); same=sum(1 for i in range(n) if o[i]==s[i])
        i=0
        while i<n and o[i]==s[i]: i+=1
        print(f'{name} {nm}: size {len(s):#x} same {same}/{n} firstdiff +{i:#x}')
SCAN_EOF
```
