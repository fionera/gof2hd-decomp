# Untested compile-flag sweep (w3695)

Status: research note, 2026-11-02. Worktree `wt/w3695`, branch `wave87-w3695`,
base `88aaf59f`. No source or build-flag change was landed: **no previously
untested compile flag is a strict improvement at this tree state.** This commit
contains only this note.

## TL;DR

* Baseline (`cmake --build cmake-build-match --target check -j8`):
  **byte 4496, linked 4496, size-exact 4524/4524, lint `CLEAN ✓`, drift 0**,
  `.text` `0x1337c8` at `0x69440`.
* I measured **59 flag variants** in this pass, most not covered by the existing
  full-gate sweeps (`build_flags_research.md`, `global_flags_sweep.md`,
  `flag_matrix_2.md`, `mllvm_flag_audit.md`, `per_tu_flags_research.md`,
  `debug_info_flags_w3564.md`, `fdata_sections_retest_4496_w3562.md`).
* **No variant beats 4496 / 4496 / 4524.** 38 are gate-neutral, 16 are worse or
  not comparable, and 5 fail to build/link.
* The neutral flags are useful negatives: they show the original's codegen is
  still consistent with clang 7.0.2's default `-Oz` pipeline. The remaining 28
  non-byte-exact functions are source/TU shape, not a missing compile flag.

## 1. Baseline

Commands:

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
cmake --build cmake-build-match --target lint -j8
```

Measured at the start and again after the sweep:

```text
verify: byte 4496  linked 4496  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
=== DRIFT: 0 field(s) across 0 class(es) ===
CLEAN ✓ (no new soft violations; hard invariants 0)
```

`size-exact` from `cmake-build-match/verify/report.json` is **4524 / 4524**.
Section sizes/bases are the known exact set: `.text 0x1337c8 @ 0x69440`,
`.rodata 0x41b67 @ 0x1c9e10`, `.data 0x296c @ 0x214000`,
`.bss 0x69ac @ 0x216970`.

## 2. Method

`GOF2_EXTRA_FLAGS` is not a native knob in `cmake/toolchain.cmake`, so a scratch
wrapper appends it to both app C and C++ flags after the canonical set:

```cmake
# /dev/shm/w3695/toolchain_extra.cmake
include("/home/debian/GalaxyOnFire2/wt/w3695/cmake/toolchain.cmake")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GOF2_EXTRA_FLAGS}" CACHE STRING "matching ARM compile flags" FORCE)
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${GOF2_EXTRA_FLAGS}" CACHE STRING "matching ARM C compile flags" FORCE)
```

The host `/tmp` tmpfs was inode-full (`1,048,576 / 1,048,576`), so the scratch
build and logs live under `/dev/shm/w3695`; no repo files were touched. Each
variant was built sequentially in the same scratch build directory (the flag
change rewrites every app compile command, so Ninja recompiles all app TUs):

```sh
cmake -S /home/debian/GalaxyOnFire2/wt/w3695 -B /dev/shm/w3695/build-sweep -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/dev/shm/w3695/toolchain_extra.cmake \
  -DGOF2_MATCH_OPT=-Oz -DGOF2_EXTRA_FLAGS='<flag>'
cmake --build /dev/shm/w3695/build-sweep --target link_so -j8
python3 tools/verify/verify.py \
  --build-dir /dev/shm/w3695/build-sweep/verify \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /dev/shm/w3695/build-sweep/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
```

`size-exact` was computed from the generated report:

```sh
python3 - <<'PY'
import json
r = json.load(open('/dev/shm/w3695/build-sweep/verify/report.json'))
rows = r['rows']
print(sum(1 for x in rows if x.get('osize') == x.get('usize')), '/', len(rows))
PY
```

`lint` and `drift` read the source/headers, not the compile flag, and were
re-run on the scratch build after the sweep: `CLEAN ✓`, `0 field(s) across 0
class(es)`. The repo `check` was re-run at the end: 4496 / 4496, `RATCHET PASS`.

## 3. Results

Baseline is **4496 / 4496 / 4524/4524, avg 99.90**. “Neutral” below means the
gate counts are exactly baseline; it does not claim the section table or
`.ARM.attributes` is identical (two explicit exceptions are called out).

### 3.1 Gate-neutral variants

| extra flag | byte | linked | size | avg | verdict |
|---|---:|---:|---:|---:|---|
| `-fno-signed-char` | 4496 | 4496 | 4524/4524 | 99.90 | neutral |
| `-fno-short-enums` | 4496 | 4496 | 4524/4524 | 99.90 | neutral |
| `-fno-strict-enums` | 4496 | 4496 | 4524/4524 | 99.90 | neutral |
| `-fno-fine-grained-bitfield-accesses` | 4496 | 4496 | 4524/4524 | 99.90 | neutral |
| `-fno-assume-sane-operator-new` | 4496 | 4496 | 4524/4524 | 99.90 | neutral |
| `-fno-reroll-loops` | 4496 | 4496 | 4524/4524 | 99.90 | neutral |
| `-fno-strict-return` | 4496 | 4496 | 4524/4524 | 99.90 | neutral |
| `-fno-rtti-data` | 4496 | 4496 | 4524/4524 | 99.90 | neutral |
| `-funique-section-names` | 4496 | 4496 | 4524/4524 | 99.90 | neutral |
| `-fno-stack-size-section` | 4496 | 4496 | 4524/4524 | 99.90 | neutral |
| `-fstack-size-section` | 4496 | 4496 | 4524/4524 | 99.90 | neutral* |
| `-ffp-contract=off` | 4496 | 4496 | 4524/4524 | 99.90 | neutral |
| `-ffp-contract=fast` | 4496 | 4496 | 4524/4524 | 99.90 | neutral |
| `-fno-associative-math` | 4496 | 4496 | 4524/4524 | 99.90 | neutral |
| `-fno-rounding-math` | 4496 | 4496 | 4524/4524 | 99.90 | neutral |
| `-fno-unsafe-math-optimizations` | 4496 | 4496 | 4524/4524 | 99.90 | neutral |
| `-mno-neg-immediates` | 4496 | 4496 | 4524/4524 | 99.90 | neutral |
| `-mno-madd4` | 4496 | 4496 | 4524/4524 | 99.90 | neutral |
| `-mcrc` | 4496 | 4496 | 4524/4524 | 99.90 | neutral |
| `-mnocrc` | 4496 | 4496 | 4524/4524 | 99.90 | neutral |
| `-mstack-alignment=8` | 4496 | 4496 | 4524/4524 | 99.90 | neutral |
| `-mstack-alignment=4` | 4496 | 4496 | 4524/4524 | 99.90 | neutral |
| `-mstack-alignment=16` | 4496 | 4496 | 4524/4524 | 99.90 | neutral |
| `-mrelax-all` | 4496 | 4496 | 4524/4524 | 99.90 | neutral |
| `-mincremental-linker-compatible` | 4496 | 4496 | 4524/4524 | 99.90 | neutral |
| `-mno-incremental-linker-compatible` | 4496 | 4496 | 4524/4524 | 99.90 | neutral |
| `-mno-stack-arg-probe` | 4496 | 4496 | 4524/4524 | 99.90 | neutral |
| `-fno-access-control` | 4496 | 4496 | 4524/4524 | 99.90 | neutral |
| `-fno-gnu-inline-asm` | 4496 | 4496 | 4524/4524 | 99.90 | neutral |
| `-fno-operator-names` | 4496 | 4496 | 4524/4524 | 99.90 | neutral |
| `-fno-delayed-template-parsing` | 4496 | 4496 | 4524/4524 | 99.90 | neutral |
| `-mtune=cortex-a9` | 4496 | 4496 | 4524/4524 | 99.90 | neutral |
| `-mfpu=neon-vfpv4` | 4496 | 4496 | 4524/4524 | 99.90 | neutral* |
| `-mno-execute-only` | 4496 | 4496 | 4524/4524 | 99.90 | neutral |
| `-fno-honor-infinities` | 4496 | 4496 | 4524/4524 | 99.90 | neutral |
| `-falign-functions=4` | 4496 | 4496 | 4524/4524 | 99.90 | neutral |
| `-fno-tree-vectorize` | 4496 | 4496 | 4524/4524 | 99.90 | neutral |
| `-fno-vectorize` | 4496 | 4496 | 4524/4524 | 99.90 | neutral |

`*` side effects that make the flag non-authentic even though the gate is neutral:

* `-fstack-size-section` emits `.stack_sizes` and `.rel.stack_sizes` in the
  object (verified with one TU); the original `.so` has no `.stack_sizes`.
* `-mfpu=neon-vfpv4` changes `.ARM.attributes` to `Tag_FP_arch: VFPv4` and
  `Tag_Advanced_SIMD_arch: NEONv1 with Fused-MAC`; the original is `VFPv3` /
  `NEONv1`.

### 3.2 Worse variants

| extra flag | byte | linked | size | avg | verdict |
|---|---:|---:|---:|---:|---|
| `-fsigned-char` | 4438 | 4492 | 4522/4524 | 99.88 | worse |
| `-fno-elide-constructors` | 1370 | 4211 | 4234/4524 | 97.14 | much worse |
| `-fvisibility=protected` | 1345 | 2386 | 4173/4524 | 92.64 | much worse |
| `-mexecute-only` | 1299 | 3996 | 4081/4524 | 97.44 | much worse |
| `-mstackrealign` | 80 | 245 | 245/4523 | 62.49 | catastrophic |
| `-momit-leaf-frame-pointer` | 1390 | 4412 | 4449/4524 | 99.49 | worse |
| `-mthread-model single` | 2085 | 4493 | 4523/4524 | 99.89 | worse |
| `-mlong-calls` | 1345 | 1664 | 1673/4524 | 75.45 | much worse |
| `-fno-inline` | 1219 | 3511 | 3571/4524 | 89.02 | much worse |
| `-mno-implicit-float` | 1355 | 4262 | 4286/4524 | 98.14 | worse |
| `-fshort-enums` | 2869 | 4474 | 4511/4524 | 99.83 | worse |
| `-fstrict-vtable-pointers` | 1395 | 4439 | 4500/4524 | 99.76 | worse |
| `-mcpu=cortex-a9` | 1230 | 2944 | 4052/4524 | 92.76 | much worse |
| `-fno-honor-nans` | 1430 | 4373 | 4480/4524 | 99.65 | worse |
| `-fno-tree-slp-vectorize` | 1484 | 4382 | 4408/4524 | 99.26 | worse |
| `-fvisibility-ms-compat` | 57 | 243 | 244/244 | 99.93 | not comparable (missing 4280) |

### 3.3 Build/link failures and unsupported flags

| flag | result |
|---|---|
| `-fno-unique-section-names` | build failure: `gen_exidx_markers.py` runs `objcopy --remove-section .text` and objcopy dies with `SIGSEGV` |
| `-mms-bitfields` | compile failure: 8 × `ms_struct may not produce Microsoft-compatible layouts for classes with base classes or virtual functions` |
| `-msoft-float` | compile failure: `neon_vector_type` attribute is not supported for this target |
| `-mdisable-fp-elim` | clang 7.0.2: `unknown argument: '-mdisable-fp-elim'` |
| `-fno-common` | link failure: multiple definition of `g_android_back_button_pressed` (`GOF2HD2012.c.obj` vs `BackButton.c.obj`) |
| `-fno-semantic-interposition`, `-fno-const-strings`, `-mno-ldrd` | clang 7.0.2: unknown argument |
| `-fno-merge-constants`, `-fno-ipa-cp`, `-fno-dsp` | accepted but ignored/unused by clang 7.0.2 |

`-fvisibility-ms-compat` hides 4280 of the 4524 compared symbols, so its
`57 / 243 / 244` row is not a like-for-like gate comparison.

## 4. What the new negatives show

* **Character signedness:** `-fsigned-char` is worse (4438/4492), while
  `-fno-signed-char` is neutral. The original is consistent with ARM's default
  unsigned `char`, not with an explicit signed-char build.
* **Enum layout:** `-fshort-enums` is worse (2869/4474); `-fno-short-enums` is
  neutral. The original's `.ARM.attributes` says `Tag_ABI_enum_size: int`, so
  the default int-sized enum is correct.
* **Inlining / C++ shape:** `-fno-inline` is much worse (1219/3511) and
  `-fno-elide-constructors` is catastrophic (1370/4211). The original uses
  normal `-Oz` inlining and copy elision.
* **Vectorization:** `-fno-vectorize` and `-fno-tree-vectorize` are neutral,
  but `-fno-tree-slp-vectorize` is worse (1484/4382). The effective vectorizer
  at this tree state is SLP, not the loop vectorizer.
* **ARM CPU/FPU:** `-mcpu=cortex-a9` is much worse (1230/2944), while
  `-mtune=cortex-a9` is neutral; plain `armv7-a` remains the correct CPU.
  `-mfpu=neon-vfpv4` is gate-neutral but changes `.ARM.attributes`, so it is
  not authentic.
* **Frame pointers / stack:** `-momit-leaf-frame-pointer` is worse
  (1390/4412), `-mstackrealign` is catastrophic (80/245), and all tested
  `-mstack-alignment` values are neutral. The original keeps frame pointers and
  needs no stack realignment.
* **Visibility:** `-fvisibility=protected` is much worse (1345/2386);
  `-fvisibility-ms-compat` hides most symbols. Default visibility is correct.
* **FP semantics:** `-ffp-contract=off/fast`, `-fno-associative-math`,
  `-fno-rounding-math`, `-fno-unsafe-math-optimizations`, and
  `-fno-honor-infinities` are neutral; `-fno-honor-nans` is worse
  (1430/4373). The original uses the default NaN semantics and no FMA
  contraction beyond what VFPv3 can express.
* **ARM backend switches:** `-mcrc`, `-mnocrc`, `-mno-madd4`,
  `-mno-neg-immediates`, `-mno-execute-only`, `-mno-stack-arg-probe`,
  `-mrelax-all`, and the incremental-linker flags are neutral; `-mexecute-only`
  and `-mlong-calls` are worse. None is a missing original flag.

## 5. Proven vs still hypothesis

**Proven by the measurements above:**

1. Baseline is byte 4496 / linked 4496 / size 4524/4524, lint `CLEAN`, drift 0,
   `.text 0x1337c8`.
2. No tested flag raises any of the three gate counts above baseline.
3. 38 previously untested flag variants are gate-neutral; 16 are worse or not
   comparable; 5 are build/link failures.
4. `-fsigned-char`, `-fshort-enums`, `-fno-inline`, `-fno-elide-constructors`,
   `-fvisibility=protected`, `-mcpu=cortex-a9`, `-mexecute-only`,
   `-mlong-calls`, `-mstackrealign`, and `-momit-leaf-frame-pointer` are all
   real regressions, not neutral choices.
5. `-fstack-size-section` adds a `.stack_sizes` section and
   `-mfpu=neon-vfpv4` changes `.ARM.attributes`; both are gate-neutral but not
   section/attribute-authentic.

**Still hypothesis / not settled:**

1. The original build command line is not recoverable from the stripped `.so`.
   The neutral flags cannot be excluded as *used but codegen-neutral*; there is
   simply no positive gate or attribute evidence for them.
2. The 28 remaining non-byte-exact functions are source/TU shape. This sweep
   does not change that: every flag that moves codegen enough to matter makes
   the gate worse.
3. `-fno-common` remains a source-cleanup question, not a flag improvement: the
   current C TUs still have duplicate tentative `g_android_back_button_pressed`
   definitions.

## 6. Reproduction summary

```sh
# baseline
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
cmake --build cmake-build-match --target lint -j8

# scratch wrapper (see §2)
cat > /dev/shm/w3695/toolchain_extra.cmake <<'CMAKE'
include("/home/debian/GalaxyOnFire2/wt/w3695/cmake/toolchain.cmake")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GOF2_EXTRA_FLAGS}" CACHE STRING "matching ARM compile flags" FORCE)
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${GOF2_EXTRA_FLAGS}" CACHE STRING "matching ARM C compile flags" FORCE)
CMAKE

# one variant
cmake -S /home/debian/GalaxyOnFire2/wt/w3695 -B /dev/shm/w3695/build-sweep -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/dev/shm/w3695/toolchain_extra.cmake \
  -DGOF2_MATCH_OPT=-Oz -DGOF2_EXTRA_FLAGS='-fno-signed-char'
cmake --build /dev/shm/w3695/build-sweep --target link_so -j8
python3 tools/verify/verify.py \
  --build-dir /dev/shm/w3695/build-sweep/verify \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /dev/shm/w3695/build-sweep/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
```

No `--update-baseline` was run; `tools/verify_baseline.json` is unchanged.
