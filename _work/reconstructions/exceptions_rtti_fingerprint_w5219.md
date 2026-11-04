# Exceptions / RTTI / unwind-table flag fingerprint (w5219)

Status: build-flag archaeology, 2026-11-04. Worktree `wt/w5219`, branch
`wave104-w5219`, base `8c5fd3646`. No source or build-flag change is landed:
the canonical build already reproduces the original's exception/RTTI
fingerprint exactly, and no tested flag is a strict gate improvement. This
commit contains this note only.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(sha256 `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`).
Toolchain: NDK r18b clang 7.0.2, `armv7-none-linux-androideabi16`, Thumb-2,
softfp, `-Oz`.

## 0. Bottom line

The original was compiled with **exceptions ON**, **RTTI ON**, and
**`-funwind-tables` ON**, using the ARM **DWARF EHABI** model (not SJLJ).
These are clang's C++ defaults plus the NDK CMake toolchain's
`-funwind-tables`; the current tree already reproduces them.

* `-fexceptions` / `-frtti` passed explicitly produce a **byte-identical
  `.so`** to the current build (same sha256), so the binary cannot distinguish
  "explicit flag" from "default".
* `-fno-exceptions` is disproven: byte_exact 4500 -> **1344**, `.ARM.extab`
  156,312 -> **30,732** B, personality LSDA records 597 -> **28**.
* `-fno-rtti` is disproven: **170** defined dynsym symbols disappear
  (`_ZTI*`/`_ZTS*`), 80 of 99 vtables lose their RTTI slot, byte_exact
  4500 -> **1701**.
* `-fno-unwind-tables` is disproven: `.ARM.exidx` 28,528 -> **26,232** B,
  3566 -> **3279** entries, byte_exact 4500 -> **2878**.
* `-fno-sjlj-exceptions` is **not accepted by clang 7.0.2** at all
  (`error: unknown argument: '-fno-sjlj-exceptions'`), so it cannot have been
  on the original command line. `-fsjlj-exceptions` is a catastrophic
  regression (`.text` 1,259,464 -> **1,901,032** B, byte_exact 1345).
* `-fno-asynchronous-unwind-tables` is a **no-op on ARM** (byte-identical
  `.so`), so it is unobservable.
* No TU differs in any observable way: every one of the 192 TUs has at least
  one exidx-covered function; 148 TUs have a personality LSDA; all 79 TUs
  that emit a vtable emit RTTI for it.

**Strict improvement: none.** No flag change raises any gate count. The note
is the only deliverable.

## 1. Commands and raw results

### 1.1 Canonical baseline

```sh
cd /home/debian/GalaxyOnFire2/wt/w5219
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
```

Raw result:

```text
compared 4524  avg 99.9%  byte_exact 4500  linked_exact 4500  fuzzy_perfect 4500
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Canonical `.so` sections (identical to the original in size and address;
measured differing bytes vs the original: `.text` 100,113, `.ARM.exidx` 829,
`.ARM.extab` 108,295, `.note.gnu.build-id` 20; `.rodata`, `.data`,
`.data.rel.ro`, `.bss`, `.got`, `.plt`, `.rel.plt`, `.rel.dyn`, `.dynsym`,
`.dynstr` and `.hash` are 0):

```text
.text         0x1337c8 = 1,259,464 @ 0x69440
.ARM.exidx    0x006f70 =    28,528 @ 0x19cc08
.ARM.extab    0x026298 =   156,312 @ 0x1a3b78
.rodata       0x041b67 =   269,159 @ 0x1c9e10
.data         0x00296c =    10,604 @ 0x214000
.data.rel.ro  0x002fe8 =    12,264 @ 0x20cbb0
.bss          0x0069ac =    27,052 @ 0x216970
```

### 1.2 Scratch A/B harness

A wrapper toolchain appends the tested flag to both C and C++ flags:

```cmake
# /dev/shm/w5219/toolchain_extra.cmake
include("/home/debian/GalaxyOnFire2/wt/w5219/cmake/toolchain.cmake")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GOF2_EXTRA_FLAGS}" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${GOF2_EXTRA_FLAGS}" CACHE STRING "" FORCE)
```

```sh
cmake -S . -B /dev/shm/w5219/build -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/dev/shm/w5219/toolchain_extra.cmake \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs \
  -DGOF2_MATCH_OPT=-Oz -DGOF2_EXTRA_FLAGS='<flags>'
cmake --build /dev/shm/w5219/build --target link_so -j6
```

The scratch baseline is byte-identical to the canonical build except for the
20-byte `.note.gnu.build-id` (`cmp -l` reports exactly 20 differing bytes);
scratch sha256 `47c6f5cb16de2ecb495b9d94d441ea679ba7e03c1568cba2e01202857935befb`.
Gate counts were measured two ways:

* `tools/verify/verify.py ... --top 0` (the canonical metric), and
* a direct byte comparison with `tools/verify/elf.py`: for every function
  defined in both `.so`s, `same size && bytes_at(orig) == bytes_at(ours)`.
  This reproduces verify's `byte_exact` exactly (4500 on the baseline) and is
  much faster; it was used for the flag matrix.

`size_exact` is `osize == usize` over the same 4524 functions.

### 1.3 Flag matrix

All variants use the canonical flags plus the listed extra flags. `byte` is
`byte_exact`, `size` is `size_exact`; section sizes are bytes. `pers` is the
number of `Personality routine:` records in `readelf --unwind` (generic-model
LSDA records pointing at `__gxx_personality_v0`).

| extra flags | byte | size | .text | .ARM.exidx | .ARM.extab | .rodata | .data.rel.ro | pers | sha256[:16] |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---|
| *(baseline)* | 4500 | 4524 | 1,259,464 | 28,528 | 156,312 | 269,159 | 12,264 | 597 | 47c6f5cb16de2ecb |
| `-fexceptions` | 4500 | 4524 | 1,259,464 | 28,528 | 156,312 | 269,159 | 12,264 | 597 | 47c6f5cb16de2ecb |
| `-frtti` | 4500 | 4524 | 1,259,464 | 28,528 | 156,312 | 269,159 | 12,264 | 597 | 47c6f5cb16de2ecb |
| `-fno-exceptions` | **1344** | **3772** | 1,168,484 | 28,520 | **30,732** | 269,143 | 12,264 | **28** | c07a1984420e0539 |
| `-fno-rtti` | **1701** | 4524 | 1,259,464 | 28,528 | 156,312 | **266,318** | **11,292** | 597 | f4e90dc337dd4a06 |
| `-fno-unwind-tables` (1) | **2878** | 4524 | 1,259,464 | **26,232** | **152,940** | 269,159 | 12,264 | 597 | 76ab895b140df7f8 |
| `-fno-asynchronous-unwind-tables` | 4500 | 4524 | 1,259,464 | 28,528 | 156,312 | 269,159 | 12,264 | 597 | 47c6f5cb16de2ecb |
| `-fno-sjlj-exceptions` | build fail | — | — | — | — | — | — | — | — |
| `-fsjlj-exceptions` | **1345** | **3773** | **1,901,032** | **4,448** | **4,512** | 269,159 | 12,264 | **28** | 0eaa8d74b41d7dc1 |
| `-fno-exceptions -fno-rtti` | **1344** | **3772** | 1,168,484 | 28,520 | 30,732 | 266,318 | 11,292 | 28 | d86c611e9ffd408e |
| `-fno-exceptions -fno-unwind-tables` (1) | 1344 | 3772 | 1,168,484 | 28,520 | 30,732 | 269,143 | 12,264 | 28 | c07a1984420e0539 |
| `-fno-rtti -fno-unwind-tables` (1) | 1701 | 4524 | 1,259,464 | 28,528 | 156,312 | 266,318 | 11,292 | 597 | f4e90dc337dd4a06 |
| `-fno-exceptions -fno-rtti -fno-unwind-tables -fno-asynchronous-unwind-tables` (1) | 1344 | 3772 | 1,168,484 | 28,520 | 30,732 | 266,318 | 11,292 | 28 | d86c611e9ffd408e |

(1) `CMakeLists.txt` adds `-funwind-tables` at target level
(`target_compile_options(gof2 PRIVATE -ffunction-sections -funwind-tables)`),
which appears **after** `CMAKE_CXX_FLAGS` on the command line and therefore
overrides a `-fno-unwind-tables` appended by the wrapper. The rows marked (1)
are consequently identical to the same build without `-fno-unwind-tables`.
The real `-fno-unwind-tables` test was made by temporarily deleting
`-funwind-tables` from that target line (then restoring it with
`git checkout -- CMakeLists.txt`):

* with the target flag removed and no extra flag: sha
  `76ab895b140df7f8219bfb66a70e8073010e2a2342ddef61d744e85f7fe0601d`,
  byte 2878, `.ARM.exidx` 26,232, `.ARM.extab` 152,940, exidx
  `(3279 entries: 698 cant, 422 inline, 2159 extab-pointer)`;
* with the target flag removed and `-fno-unwind-tables` added: **identical**
  sha/sections. So on this target clang's default (without
  `-funwind-tables`) is also the no-unwind-tables behaviour, and the original
  needs the explicit `-funwind-tables`.

### 1.4 `.ARM.exidx` / `.ARM.extab` classification

`readelf --unwind` over the original and the baseline:

```text
original: 3566 entries, 272 cantunwind, 856 compact-inline, 2438 extab-pointer
          -> 597 "Personality routine: 0x5eb60" (__gxx_personality_v0)
          -> 2697 "Compact model" records, 272 cantunwind
baseline: 3566 entries, 272 cantunwind, 856 compact-inline, 2438 extab-pointer
          -> 597 personality, 2697 compact, 272 cantunwind
```

The 597 personality records are the functions with a real LSDA (cleanup
landing pads). The remaining 2438 extab-pointer entries are compact-model
unwind records (personality 0/1 with no LSDA); they still need
`-funwind-tables` but not `-fexceptions`.

Per-function unwind text (personality marker normalised, extab addresses
normalised, compact instruction bytes kept) is **identical for all 3372
named exidx entries**: `unwind_text.py baseline.so original.so` prints
`entries 3372; differing unwind records 0`. This is the strongest single
piece of evidence: the original and the baseline have the same set of
landing pads and the same compact unwind instructions.

### 1.5 Exceptions / RTTI symbol evidence

Original and baseline both contain:

```text
_Unwind_Resume            541 references (540 bl + 1 blx)
_Unwind_RaiseException      3
__gxx_personality_v0      597 personality records (all to 0x5eb60)
__cxa_throw                 4 call sites
__cxa_begin_catch          12
__cxa_end_catch            11
__cxa_allocate_exception    3
__cxa_rethrow               1
__cxa_call_unexpected      22
```

`-fno-exceptions` leaves only 16 `_Unwind_Resume` references (libc++abi's own
landing pads) and 28 personality records; `.ARM.extab` falls to 30,732 B.
The 569-record difference is app-code cleanup LSDA. A full
`tools/verify/verify.py` run on the `-fno-exceptions` build reports
`byte_exact 1344  linked_exact 3757  avg 93.22%` (vs the baseline's
`4500 / 4500 / 99.9%`).

RTTI in the original:

```text
176 _ZTI* typeinfo objects
176 _ZTS* type-name objects
 99 _ZTV* vtables
 99/99 vtables have a non-null RTTI slot resolving (via R_ARM_ABS32 /
       R_ARM_RELATIVE) to a _ZTI* symbol
```

`-fno-rtti` removes 170 defined dynsym symbols (`defined orig 5370 ours 5200
missing 170 extra 0`), leaves only 91 `_ZTI`/`_ZTS` (the libc++abi archive's
own 19 vtables/typeinfos still carry RTTI because the archive was built with
it), and gives 80/99 vtables a null RTTI slot. Note that the `byte_exact`
drops for `-fno-exceptions` and `-fno-rtti` include data-address shifts
(`.rodata`/`.data.rel.ro` shrink), so the decisive evidence is the
section/personality/symbol counts above, not `byte_exact` alone.

## 2. Per-TU analysis

Using the checked-in `tools/source_order.txt` (192 TUs) and each object's
`.symtab`, every TU was mapped to the linked function addresses:

| observable | result |
|---|---|
| TUs with at least one exidx-covered function | **192 / 192** |
| TUs with at least one personality LSDA (generic `__gxx_personality_v0` record) | **148 / 192** |
| TUs with zero personality LSDA (exceptions unobservable) | 44 / 192 |
| TUs emitting at least one app vtable | **79 / 192** |
| app vtables with RTTI | **80 / 80** (79 TUs; one TU emits two) |
| TUs with no vtable (RTTI unobservable) | 113 / 192 |

The 44 exception-unobservable TUs are mostly leaf math/render helpers with no
local objects that need destruction (e.g. `Vector.cpp`, `Matrix.cpp`,
`AEMath.cpp`, `Camera.cpp`, `Image2D.cpp`, `SpriteGun.cpp`, the C TUs
`BackButton.c`/`main.c`/`GOF2HD2012.c`/`BlowfishSubkeyFile.c`). They are
consistent with `-fexceptions` on, but a per-TU `-fno-exceptions` there would
be invisible. No TU shows positive evidence of `-fno-exceptions` or
`-fno-rtti`; the 148 and 79 observable TUs prove the flags on.

## 3. Confidence table

| claim | confidence | hard evidence |
|---|---:|---|
| `-fexceptions` ON (DWARF EHABI) | **99%** | 597 personality LSDA records, 541 `_Unwind_Resume` refs, 3372/3372 per-function unwind texts identical; `-fno-exceptions` -> 28 records, 1344 byte_exact |
| `-frtti` ON | **99%** | 176 `_ZTI` + 176 `_ZTS`; 99/99 vtable RTTI slots resolve to `_ZTI`; `-fno-rtti` -> 170 missing symbols, 80/99 null slots, 1701 byte_exact |
| `-funwind-tables` ON | **99%** | 3566 exidx entries / 3372 unique addresses; removing it -> 3279 entries, 26,232 B, 2878 byte_exact |
| not SJLJ (`-fsjlj-exceptions`) | **99%** | no `_Unwind_SjLj_*`/`setjmp`/`longjmp` imports; `-fsjlj-exceptions` -> `.text` 1,901,032, exidx 4,448, 1345 byte_exact |
| `-fno-sjlj-exceptions` not on the command line | **99%** | clang 7.0.2: `error: unknown argument: '-fno-sjlj-exceptions'` (C and C++) |
| `-fno-asynchronous-unwind-tables` unobservable | **99%** | byte-identical `.so` (sha `47c6f5cb...`) on ARM; the flag is x86-64-specific |
| explicit vs default `-fexceptions`/`-frtti` | unobservable | both explicit flags give the same sha256 as the default build |
| no per-TU exception/RTTI difference | **high, bounded** | 148/192 TUs prove exceptions on; 79/79 vtable TUs prove RTTI on; 192/192 prove unwind tables on; the remaining TUs are unobservable, not contrary |
| current tree already faithful | **99%** | canonical `check` 4500/4500, 4524/4524, lint CLEAN, drift 0; all section sizes/addresses exact |

## 4. What is proven / disproven / unobservable

**Positively proven by the binary (and reproduced by the tree):**

1. Exceptions are enabled, using DWARF EHABI (`__gxx_personality_v0` LSDA +
   `_Unwind_Resume`), not SJLJ and not `-fno-exceptions`.
2. RTTI is enabled: every vtable has a `_ZTI` RTTI slot and the typeinfo/name
   objects are emitted.
3. `-funwind-tables` is enabled: the original has 3566 exidx entries
   (including 2697 compact records and 272 `cantunwind` sentinels), not the
   3279-entry/26,232-byte shape of a build without it.
4. `-fno-sjlj-exceptions` cannot have been passed to clang 7.0.2 (unknown
   argument); the ARM default is the DWARF EHABI model anyway.

**Disproven:**

* `-fno-exceptions` (byte 1344, extab 30,732, 28 personality records).
* `-fno-rtti` (170 missing symbols, 80/99 null RTTI slots, byte 1701).
* `-fno-unwind-tables` (exidx 26,232, byte 2878).
* `-fsjlj-exceptions` (`.text` +50%, byte 1345, no unwind sections).

**Unobservable / unknown:**

* Whether `-fexceptions` / `-frtti` were written explicitly or left to clang's
  C++ defaults: both produce a byte-identical `.so`.
* Whether `-fno-asynchronous-unwind-tables` was passed: no effect on ARM.
* The exact flag spelling of `-funwind-tables` (CMake toolchain default vs
  explicit `target_compile_options`): same codegen.
* Per-TU `-fno-exceptions` for the 44 TUs with no cleanup landing pad, and
  per-TU `-fno-rtti` for the 113 TUs with no vtable: no observable difference.
* The original command line itself is not recoverable; only its codegen
  fingerprint is.

## 5. Strict improvement?

**No.** The canonical build already has exceptions and RTTI on (clang C++
defaults; `-frtti` is explicit in `cmake/toolchain.cmake`) and
`-funwind-tables` on (`CMakeLists.txt` target option). Every tested change
either leaves the binary byte-identical (`-fexceptions`, `-frtti`,
`-fno-asynchronous-unwind-tables`) or degrades the gate. No baseline update
was made; `tools/verify_baseline.json`, `tools/sodiff_allow.txt` and `tools/`
are untouched.

An optional, byte-neutral cleanup would be to spell `-fexceptions` explicitly
next to `-frtti` in `cmake/toolchain.cmake` for documentation/faithfulness.
It changes no byte and no gate count, so it is not a strict improvement and
was not committed.

## 6. Reproduction summary

```sh
# canonical
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6

# one variant (wrapper in /dev/shm/w5219/toolchain_extra.cmake)
cmake -S . -B /dev/shm/w5219/build -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/dev/shm/w5219/toolchain_extra.cmake \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs \
  -DGOF2_MATCH_OPT=-Oz -DGOF2_EXTRA_FLAGS='-fno-exceptions'
cmake --build /dev/shm/w5219/build --target link_so -j6

# fast byte_exact / size_exact (same definition as tools/verify)
python3 /dev/shm/w5219/scripts/fast_gate.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  /dev/shm/w5219/so/<variant>.so

# EH / RTTI fingerprint
python3 /dev/shm/w5219/scripts/secstats.py /dev/shm/w5219/so/<variant>.so \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
python3 /dev/shm/w5219/scripts/unwind_text.py /dev/shm/w5219/so/<variant>.so \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
python3 /dev/shm/w5219/scripts/rtti_check2.py /dev/shm/w5219/so/<variant>.so
```

The scripts are analysis-only; they live under `/dev/shm/w5219/scripts/` and
are reproduced by the snippets above. No repo file other than this note was
changed.
