# `-fdata-sections` retest at 4496 + data-layout flag sweep (w3562)

Status: research note, 2026-11-01. Worktree `wt/w3562`, branch `wave87-w3562`,
HEAD `113737ee`. Every number below is from a command run against
`../_work/bins/android_2.0.16_libgof2hdaa.so` or from a controlled A/B rebuild of
this tree. This note supersedes the `-fdata-sections` rows in
`fdata_sections_retest.md` (measured at byte 4182) and `fdata_sections_w3439.md`
(measured at byte 4250) for the current tree state.

## 0. Bottom line

At the current baseline (**byte 4496 / linked 4496 / size 4524/4524 / data
843/843 / lint CLEAN / drift 0**) **`-fdata-sections` is still a regression and
no data-layout flag is a strict improvement**:

| variant | byte | linked | size | `.rodata` | `.data` | `.bss` |
|---|---:|---:|---:|---:|---:|---:|
| baseline (no `-fdata-sections`) | **4496** | 4496 | 4524/4524 | `0x41b67` (exact) | `0x296c` (exact) | `0x216970`/`0x69ac` (exact) |
| app TUs only | **4333** (−163) | 4496 | 4524/4524 | `0x41a07` (−0x160) | `0x2970` (+4) | `0x216970`/`0x69ac` |
| vendored zlib/libzip only | **4493** (−3) | 4496 | 4524/4524 | `0x41b67` | `0x296e` (+2) | `0x216970`/`0x69ac` |
| app + vendored | **4315** (−181) | 4496 | 4524/4524 | `0x41a07` (−0x160) | `0x295e` (−0xe) | `0x216960`/`0x69ac` |
| `-fno-data-sections` control | **4496** | 4496 | 4524/4524 | `0x41b67` | `0x296c` | `0x216970`/`0x69ac` |

The original's `.rodata`/`.data`/`.data.rel.ro` are the **non-`-fdata-sections`**
layout: the baseline is 100 % byte-identical in all three sections, while the app
`-fdata` build differs in 42.3 % of `.rodata` bytes (first divergence at VMA
`0x1e79a3`, the `AEFile` RTTI block).

No flag or source change is landed. This note is the only commit.

## 1. Baseline

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
cmake --build cmake-build-match --target lint  -j8
```

```text
verify: byte 4496  linked 4496  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
=== DRIFT: 0 field(s) across 0 class(es) ===
CLEAN ✓ (no new soft violations; hard invariants 0)
```

`report.json`: 4524/4524 size-exact. Baseline `.so` sha256:

```text
a992e12ff5b675912516185b2a9952211f15b394a76ec5282df24f398cbfec87  cmake-build-match/libgof2hdaa.so
d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483  ../_work/bins/android_2.0.16_libgof2hdaa.so
```

Baseline section table (all equal to the original except `.ARM.extab`, a
pre-existing 8-byte shortfall unrelated to this pass):

| section | addr | size | original |
|---|---:|---:|---:|
| `.text` | `0x69440` | `0x1337c8` | same |
| `.ARM.exidx` | `0x19cc08` | `0x6f70` | same |
| `.ARM.extab` | `0x1a3b78` | `0x26290` | `0x26298` |
| `.rodata` | `0x1c9e10` | `0x41b67` | same |
| `.data.rel.ro` | `0x20cbb0` | `0x2fe8` | same |
| `.got` | `0x20fcf4` | `0x430c` | same |
| `.dynamic` | `0x20fba4` | `0x148` | same |
| `.data` | `0x214000` | `0x296c` | same |
| `.bss` | `0x216970` | `0x69ac` | same |

## 2. A/B method

`cmake/toolchain.cmake` `FORCE`s `CMAKE_CXX_FLAGS`/`CMAKE_C_FLAGS`, so the extra
flag is appended by a wrapper toolchain:

```sh
cat > /tmp/w3562/toolchain_extra.cmake <<'TOOLCHAIN_EOF'
include("/home/debian/GalaxyOnFire2/wt/w3562/cmake/toolchain.cmake")
set(GOF2_EXTRA_FLAGS "" CACHE STRING "extra matching flags")
if(GOF2_EXTRA_FLAGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GOF2_EXTRA_FLAGS}" CACHE STRING "matching ARM compile flags" FORCE)
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${GOF2_EXTRA_FLAGS}" CACHE STRING "matching ARM compile flags" FORCE)
endif()
TOOLCHAIN_EOF
```

Each app-only variant (scratch build dir, then verify + ratchet):

```sh
cmake -S . -B /tmp/w3562/build-NAME -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/tmp/w3562/toolchain_extra.cmake \
  -DGOF2_MATCH_OPT=-Oz -DGOF2_EXTRA_FLAGS='<flags>'
cmake --build /tmp/w3562/build-NAME --target link_so -j8
python3 tools/verify/verify.py \
  --build-dir /tmp/w3562/build-NAME/verify \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3562/build-NAME/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 tools/ratchet.py --build-dir /tmp/w3562/build-NAME --no-rebuild
```

Vendored zlib/libzip are compiled by `third_party/build_match_lib.cmake` with its
own `CC_BASE`, not the CMake flags. For vendored-only rows, the builder was
temporarily edited:

```sh
cp third_party/build_match_lib.cmake /tmp/w3562/build_match_lib.cmake.pre
python3 - <<'PY'
p = 'third_party/build_match_lib.cmake'
s = open(p).read()
old = '    -fPIC -ffunction-sections -funwind-tables -Wno-everything)'
new = '    -fPIC -ffunction-sections <flags> -funwind-tables -Wno-everything)'
assert old in s
open(p, 'w').write(s.replace(old, new, 1))
PY
# build the vendored variant, then restore:
cp /tmp/w3562/build_match_lib.cmake.pre third_party/build_match_lib.cmake
cmake --build cmake-build-match --target gof2_libs -j8   # restore baseline archives
cmake --build cmake-build-match --target check -j8       # back to 4496 / 4496
```

The vendored builder writes `third_party/<lib>/lib*_gof2.a` into the source tree,
so all builds were run **sequentially**. The baseline was re-checked after the
vendored rows and returned to `byte 4496 / linked 4496 / RATCHET PASS`.

Link-flag A/B used the exact gold command from the baseline build
(`ninja -C cmake-build-match -t commands libgof2hdaa.so`), retargeted with
`-o "$OUT"` and `--build-id "$@"`, then stripped. A no-extra-flag relink
reproduced the baseline sha256 `a992e12f…` exactly.

## 3. `-fdata-sections` results

### 3.1 Gate and section table

| variant | sha256 | `.text` | `.rodata` | `.data.rel.ro` | `.data` | `.bss` addr/size |
|---|---|---|---:|---:|---:|---:|---:|
| original | `d4c17be6…` | `0x1337c8` | `0x41b67` | `0x2fe8` | `0x296c` | `0x216970`/`0x69ac` |
| baseline | `a992e12f…` | `0x1337c8` | `0x41b67` | `0x2fe8` | `0x296c` | `0x216970`/`0x69ac` |
| app only | `edea731f…` | `0x1337c8` | `0x41a07` | `0x2fe8` | `0x2970` | `0x216970`/`0x69ac` |
| vendored only | `848f77c5…` | `0x1337c8` | `0x41b67` | `0x2fe8` | `0x296e` | `0x216970`/`0x69ac` |
| app + vendored | `c8ec1a7e…` | `0x1337c8` | `0x41a07` | `0x2fe8` | `0x295e` | `0x216960`/`0x69ac` |

`.text` never changes (the flag is data-only). `linked_exact` stays 4496 and
size stays 4524/4524 in every row; stubs/missing/extra stay 0; data stays
843/843. The scratch-build sha256 values differ from the baseline only in the
20-byte `.note.gnu.build-id` (`cmp -l` reports exactly 20 differing bytes for
the neutral variants); the section contents are otherwise directly comparable.

### 3.2 Which functions lose byte-exactness

All **163** app-only losses are rows that are `pct == 100.0` in both reports:
**0 instruction-stream changes, 0 gains, 0 size changes**. The only changed
bytes are literal-pool words. The committed classifier
`_work/reconstructions/fdata_sections_w3439.py` (same method as the earlier
notes: disassemble each lost function, resolve every PIC `ldr/add pc` pair to
its destination, classify the differing words) gives:

```sh
GOF2_ROOT=$PWD python3 _work/reconstructions/fdata_sections_w3439.py \
  --fdata /tmp/w3562/so/fdata-app.so \
  --fdata-report /tmp/w3562/verify-fdata-app-final/report.json
```

```text
losses: 163 pct_changed: 0
pool words by destination section: {('.data', '.data'): 31, ('.rodata', '.rodata'): 362}
functions by destination section: {('.data', '.data'): 14, ('.rodata', '.rodata'): 151}
total pool words: 393
```

Every word resolves to the **same logical object** in both binaries; only its
address moved. The 151 `.rodata` functions are dominated by the shader `Init`
cluster (each loads its vertex/fragment source strings), plus `GameText`,
`ModStation`, `RepairBeam`, `SpaceLounge`, `DialogueWindow`, `Generator`,
`Station`, `Status`, `PlayerEgo`, zlib and a handful of others. The 14 `.data`
functions are the input globals (`SwitchToOtherMouseConifguration`,
`ActualizeMouseVisibilty`, `setValuesForGamepad`, …) and zlib's `_tr_init`.

### 3.3 Content comparison against the original

Section payload scan (baseline and app `-fdata`):

| section | baseline | app `-fdata-sections` |
|---|---|---|
| `.rodata` | **269159 / 269159** same (100 %), first diff `+0x41b67` | **154872 / 268807** same (57.6 %), first diff `+0x1db93` (VMA `0x1e79a3`) |
| `.data` | **10604 / 10604** same (100 %), first diff `+0x296c` | **7331 / 10604** same (69.1 %), first diff `+0x4` (VMA `0x214004`) |
| `.data.rel.ro` | **12264 / 12264** same (100 %), first diff `+0x2fe8` | **11859 / 12264** same (96.7 %), first diff `+0x68` (VMA `0x20cc18`) |

Over the whole `.rodata` region where the baseline equals the original
(269,159 bytes), the app `-fdata` build differs in **113,935 bytes (42.3 %)**.
The original agrees with the non-`-fdata` bytes, not the `-fdata` bytes.

### 3.4 Structural proof: `AEFile.cpp` RTTI

The first `.rodata` divergence at VMA `0x1e79a3` is the `AEFile` RTTI block.
Original / baseline:

```text
0x1e7998  "9AEPakFile\0"
0x1e79a3  "12AENormalFile\0"
0x1e79c0  "14AELowLevelFile\0"     (16-aligned)
```

App `-fdata-sections`:

```text
0x1e7998  "9AEPakFile\0"
0x1e79b0  "14AELowLevelFile\0"     (16-aligned)
0x1e79c1  "12AENormalFile\0"
```

The object sections show why. Compiling only `AEFile.cpp` with the exact
`compile_commands.json` line, with and without `-fdata-sections`:

```text
baseline object:
  .rodata                       0x41a  crc32Table + _ZTS9AEPakFile + _ZTS12AENormalFile
  .rodata.str1.1                0x007
  .rodata._ZTS14AELowLevelFile  0x011  (group, align 16)

-fdata-sections object:
  .rodata._ZN12_GLOBAL__N_110crc32TableE  0x400
  .rodata.str1.1                          0x007
  .rodata._ZTS9AEPakFile                  0x00b
  .rodata._ZTS14AELowLevelFile            0x011  (group, align 16)
  .rodata._ZTS12AENormalFile              0x00f  <-- after the group
```

The original has the two plain RTTI strings packed contiguously before the
16-aligned group, i.e. exactly clang's non-`-fdata` shared-`.rodata` emission.
This is compiler-emission order, not string tuning: the source order of the
class definitions is unchanged between the two builds.

### 3.5 The section-ordering file is not the dominant cause

The app `-fdata` objects were relinked with the literal `.data*` line removed
from `section_order.txt` (there is no `.bss*` line at this HEAD; only
`.bss.anchor`):

```sh
grep -v -x -F '.data*' /tmp/w3562/build-fdata-app-keep/section_order.txt \
  > /tmp/w3562/order-fdata-nodata.txt
# exact gold command from: ninja -C /tmp/w3562/build-fdata-app-keep -t commands libgof2hdaa.so
# only --section-ordering-file and -o changed
```

Result: **byte 4334 / linked 4496** (vs 4333 with the unmodified ordering file),
`.data` back to `0x296c` (exact), `.bss` file offset back to `0x21596c`, but
`.rodata` still `0x41a07`. Classifier on the relinked `.so`:

```text
losses: 162 pct_changed: 0
pool words by destination section: {('.data', '.data'): 28, ('.rodata', '.rodata'): 362}
functions by destination section: {('.data', '.data'): 13, ('.rodata', '.rodata'): 151}
total pool words: 390
```

So the `.data*` grouping accounts for **1 net loss and +4 `.data` bytes**; the
remaining **162 losses are intrinsic to the per-symbol `.rodata` reorder**.
The negative result is not an artifact of the ordering file.

## 4. Data-layout compile-flag sweep

All rows are app-only unless marked “vendored”. Baseline is
`4496 / 4496 / 4524 / 843`. “neutral” means the linked content is identical to
the baseline except the 20-byte build-id (`cmp -l` reports 20 differing bytes).

| extra flag | byte | linked | size | data | `.text` | `.rodata` | `.data` | `.bss` | verdict |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---|
| *(baseline)* | 4496 | 4496 | 4524/4524 | 843/843 | `0x1337c8` | `0x41b67` | `0x296c` | `0x69ac` | — |
| `-fdata-sections` | **4333** | 4496 | 4524/4524 | 843/843 | `0x1337c8` | `0x41a07` | `0x2970` | `0x69ac` | −163 |
| `-fno-data-sections` | 4496 | 4496 | 4524/4524 | 843/843 | `0x1337c8` | `0x41b67` | `0x296c` | `0x69ac` | neutral |
| `-fmerge-all-constants` | 4496 | 4496 | 4524/4524 | 843/843 | `0x1337c8` | `0x41b67` | `0x296c` | `0x69ac` | neutral |
| `-fno-merge-all-constants` | 4496 | 4496 | 4524/4524 | 843/843 | `0x1337c8` | `0x41b67` | `0x296c` | `0x69ac` | neutral |
| `-fcommon` | 4496 | 4496 | 4524/4524 | 843/843 | `0x1337c8` | `0x41b67` | `0x296c` | `0x69ac` | neutral |
| `-fno-common` | — | — | — | — | — | — | — | — | **link error** (see below) |
| `-mno-global-merge` | **1397** | 4486 | 4517/4524 | 843/843 | `0x133928` | `0x41b67` | `0x294c` | `0x699c` | −3099 |
| `-fno-zero-initialized-in-bss` | **1522** | 4486 | 4516/4524 | 557/843 | `0x133768` | `0x41b67` | `0x800c` | `0x132c` | −2974 |
| `-fshort-wchar` | 4496 | 4496 | 4524/4524 | 843/843 | `0x1337c8` | `0x41b67` | `0x296c` | `0x69ac` | neutral |
| `-fno-threadsafe-statics` | **1368** | 4492 | 4520/4521 | 843/843 | `0x133684` | `0x419b7` | `0x296c` | `0x69ac` | −3128, missing 3 |
| `-fno-use-cxa-atexit` | **1356** | 4495 | 4523/4524 | 843/843 | `0x133818` | `0x41b67` | `0x296c` | `0x69ac` | −3140 |
| `-fno-rtti` | **1725** | 4496 | 4524/4524 | 593/673 | `0x1337c8` | `0x4104e` | `0x296c` | `0x69ac` | −2771, missing 170 |
| `-fno-unwind-tables` | 4496 | 4496 | 4524/4524 | 843/843 | `0x1337c8` | `0x41b67` | `0x296c` | `0x69ac` | neutral (overridden) |
| `-fno-asynchronous-unwind-tables` | 4496 | 4496 | 4524/4524 | 843/843 | `0x1337c8` | `0x41b67` | `0x296c` | `0x69ac` | neutral (overridden) |
| `-fno-use-init-array` | 4496 | 4496 | 4524/4524 | 843/843 | `0x1337c8` | `0x41b67` | `0x296c` | `0x69ac` | neutral |
| `-fwritable-strings` | **1425** | 4487 | 4515/4524 | 838/843 | `0x133828` | `0x256e7` | `0x2310c` | `0x6aac` | −3071 |
| `-fdata-sections -fmerge-all-constants` | **4333** | 4496 | 4524/4524 | 843/843 | `0x1337c8` | `0x41a07` | `0x2970` | `0x69ac` | same as `-fdata` |
| vendored `-fdata-sections` | **4493** | 4496 | 4524/4524 | 843/843 | `0x1337c8` | `0x41b67` | `0x296e` | `0x69ac` | −3 |
| vendored `-fno-common` | 4496 | 4496 | 4524/4524 | 843/843 | `0x1337c8` | `0x41b67` | `0x296c` | `0x69ac` | neutral |
| vendored `-fmerge-all-constants` | 4496 | 4496 | 4524/4524 | 843/843 | `0x1337c8` | `0x41b67` | `0x296c` | `0x69ac` | neutral |

Notes:

* `-fno-common` on the app fails to link at this tree state:

  ```text
  ld.gold: error: .../GOF2HD2012.c.obj: multiple definition of 'g_android_back_button_pressed'
  ld.gold: .../BackButton.c.obj: previous definition here
  ```

  The duplicate is a tentative definition in both C TUs. Removing the
  `GOF2HD2012.c` copy temporarily (then reverting) makes the link succeed:
  **byte 4465 / linked 4496**, all section sizes/bases exact, data 843/843.
  That is still −31, so `-fno-common` is not a candidate. The temporary edit was
  reverted; the worktree is clean.
* `-fno-threadsafe-statics` drops the three `__cxa_guard_*` exports and 3
  function rows (`missing 3`), in addition to the byte loss.
* `-fno-rtti` drops 170 `_ZTI*`/`_ZTS*` exports and the `.data.rel.ro` vtable
  block (`0x2fe8 -> 0x2c1c`), so its data comparison is over 673 common objects.
* `-fshort-wchar` is gate-neutral because the tree has no `wchar_t` data; the
  linked `.ARM.attributes` still says `Tag_ABI_PCS_wchar_t: 4` (the original's
  value). It is not evidence for the flag.
* `-fno-unwind-tables` and `-fno-asynchronous-unwind-tables` are neutral because
  the target appends `-funwind-tables` after the extra flags.
* clang 7.0.2 does not support `-fmerge-constants`/`-fno-merge-constants`
  (`warning: optimization flag ... is not supported`) or `-fno-const-strings`
  (`error: unknown argument`). `-fwritable-strings` is accepted and is
  catastrophic.

## 5. Data-layout link-flag sweep

Baseline relink (no extra flag) reproduces sha256 `a992e12f…` exactly. Each row
is a relink of the baseline objects with the extra gold flag:

| extra gold flag | byte | linked | `.text` | `.rodata` | `.data.rel.ro` | `.data` | `.bss` | verdict |
|---|---:|---:|---:|---:|---:|---:|---:|---|
| *(baseline relink)* | 4496 | 4496 | `0x1337c8` | `0x41b67` | `0x2fe8` | `0x296c` | `0x69ac` | exact |
| `--sort-common` | 4496 | 4496 | `0x1337c8` | `0x41b67` | `0x2fe8` | `0x296c` | `0x69ac` | exact no-op (same sha) |
| `--sort-section=name` | **1342** | 4494 | `0x13381c` | `0x41b50` | `0x2fe0` | `0x296c` | `0x69b4` | −3154 |
| `--gc-sections` | **2176** | 4496 | `0x133794` | `0x41b57` | `0x2fe8` | `0x296c` | `0x699c` | −2320 |
| `-z norelro` | **2932** | 4496 | `0x1337c8` | `0x41b67` | `0x2fe8` | `0x296c` | `0x69ac` | −1564; `.data.rel.ro` moves after `.data` |
| `--no-merge-exidx-entries` | **2878** | 4496 | `0x1337c8` | `0x41b67` | `0x2fe8` | `0x296c` | `0x69ac` | −1618; `.ARM.exidx` `0x95c0`, everything shifts |

`--sort-common` is an exact no-op even with the Blowfish C `COMMON` block (the
COMMON symbols are already in gold's sorted order). No link flag improves the
gate.

## 6. Proven vs hypothesis

**Proven by controlled rebuilds at `113737ee`:**

1. `-fdata-sections` is a regression: app-only **4496 → 4333** (−163),
   app+vendored **4496 → 4315** (−181), vendored-only **4496 → 4493** (−3).
   `linked_exact` stays 4496; size stays 4524/4524; stubs/missing/extra stay 0;
   data stays 843/843; lint is CLEAN and drift is 0 in every variant.
2. All 163 app-only losses are `pct == 100.0` rows: the instruction stream is
   unchanged. The 393 differing words are literal-pool words: 362 to `.rodata`
   (151 functions) and 31 to `.data` (14 functions), all resolving to the same
   logical object at a different address.
3. App `-fdata` shrinks `.rodata` by 0x160 and grows `.data` by 4; vendored
   `-fdata` grows `.data` by 2; both together shrink `.data` by 0xe and move
   `.bss` from 0x216970 to 0x216960. The original's sizes/addresses are the
   non-`-fdata` ones.
4. The baseline `.rodata`, `.data` and `.data.rel.ro` are 100 % byte-identical
   to the original; the app `-fdata` build differs in 42.3 % of `.rodata`,
   30.9 % of `.data` and 3.3 % of `.data.rel.ro`. The first `.rodata` divergence
   is the `AEFile` RTTI block at VMA 0x1e79a3, and the object-level section
   order proves the original matches the non-`-fdata` emission.
5. The loss is not caused by the section-ordering file's `.data*` grouping:
   removing that line gives 4334 (one function recovered, `.data` size exact),
   still −162. The remaining loss is intrinsic to the per-symbol `.rodata`
   reorder.
6. `-fno-data-sections` is neutral, confirming the flag is off by default and
   that the baseline is not accidentally relying on an implicit
   `-fdata-sections`.
7. No tested data-layout compile flag or link flag is a strict improvement.
   `-mno-global-merge`, `-fno-zero-initialized-in-bss`, `-fno-threadsafe-statics`,
   `-fno-use-cxa-atexit`, `-fno-rtti` and `-fwritable-strings` are catastrophic;
   `-fmerge-all-constants`, `-fno-merge-all-constants`, `-fcommon`,
   `-fno-common` (vendored), `-fno-unwind-tables`,
   `-fno-asynchronous-unwind-tables`, `-fno-use-init-array`, `-fshort-wchar`
   and `--sort-common` are neutral.

**Still hypothesis / not settled:**

1. *Why* the original lacks the flag. The binary is consistent with a
   non-`-fdata-sections` compile, but the original build system is not
   recovered. Candidates: the app was not built by stock ndk-build
   `GLOBAL_CFLAGS`; it passed `-fno-data-sections`; or its TU/source order
   happens to re-interleave the per-symbol sections exactly as the shared
   `.rodata`/`.data` emission. The last is possible but not parsimonious, and
   the `AEFile` RTTI group order is a direct counterexample for our TU.
2. The content comparison is partly by construction: earlier data-layout
   passes tuned the tree's `.rodata` against the original under the
   non-`-fdata` flag. The gate result (§3.1) is independent of that tuning; the
   structural `AEFile`/`.data` evidence (§3.4–3.5) is the part that does not
   rely on tuned string order.
3. `-fdata-sections` could become positive again if a future TU/data-layout
   change makes the tree's data content match the original's *per-symbol*
   order. Re-test then. Today it is a regression, not an authenticity win.

## 7. Reproduction summary

```sh
# baseline
cmake --preset default
cmake --build cmake-build-match --target check -j8       # 4496 / 4496
cmake --build cmake-build-match --target drift -j8       # 0 fields
cmake --build cmake-build-match --target lint  -j8       # CLEAN

# app-only -fdata-sections
cat > /tmp/w3562/toolchain_extra.cmake <<'TOOLCHAIN_EOF'
include("/home/debian/GalaxyOnFire2/wt/w3562/cmake/toolchain.cmake")
set(GOF2_EXTRA_FLAGS "" CACHE STRING "extra matching flags")
if(GOF2_EXTRA_FLAGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GOF2_EXTRA_FLAGS}" CACHE STRING "" FORCE)
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${GOF2_EXTRA_FLAGS}" CACHE STRING "" FORCE)
endif()
TOOLCHAIN_EOF
cmake -S . -B /tmp/w3562/build-fdata-app -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/tmp/w3562/toolchain_extra.cmake \
  -DGOF2_MATCH_OPT=-Oz -DGOF2_EXTRA_FLAGS='-fdata-sections'
cmake --build /tmp/w3562/build-fdata-app --target link_so -j8   # 4333 / 4496

# vendored-only: append -fdata-sections to CC_BASE in third_party/build_match_lib.cmake
# app+vendored: both of the above

# classification of the 163 losses
python3 tools/verify/verify.py --build-dir /tmp/w3562/verify-fdata-app-final \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3562/build-fdata-app/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
GOF2_ROOT=$PWD python3 _work/reconstructions/fdata_sections_w3439.py \
  --fdata /tmp/w3562/build-fdata-app/libgof2hdaa.so \
  --fdata-report /tmp/w3562/verify-fdata-app-final/report.json
```

Section-content scan used in §3.3:

```python
import struct
def sec(path, name):
    d = open(path, 'rb').read()
    shoff, = struct.unpack_from('<I', d, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from('<HHH', d, 0x2e)
    hdrs = [struct.unpack_from('<IIIIIIIIII', d, shoff + i*shentsize) for i in range(shnum)]
    stroff = hdrs[shstrndx][4]
    for h in hdrs:
        n = d[stroff+h[0]:d.index(b'\0', stroff+h[0])].decode()
        if n == name:
            return d[h[4]:h[4]+h[5]], h[3]
for name in ('.rodata', '.data', '.data.rel.ro'):
    o, oa = sec('../_work/bins/android_2.0.16_libgof2hdaa.so', name)
    for label, path in (('base', 'cmake-build-match/libgof2hdaa.so'),
                        ('fdata', '/tmp/w3562/build-fdata-app/libgof2hdaa.so')):
        s, sa = sec(path, name)
        n = min(len(o), len(s))
        same = sum(1 for i in range(n) if o[i] == s[i])
        i = 0
        while i < n and o[i] == s[i]:
            i += 1
        print(name, label, f'size {len(s):#x} same {same}/{n} firstdiff +{i:#x}')
```
