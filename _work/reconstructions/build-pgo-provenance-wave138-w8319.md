# Build-flag / PGO provenance, wave 138 (w8319)

Status: **note-only research commit**, 2026-11-07. Worktree
`/home/debian/GalaxyOnFire2/wt/w8319`, base `96b069792` ("Level::createMission
wave 137: keep w8213 note"). Scratch: `/dev/shm/w8319`. No `src/`, `tools/`,
`CMakeLists.txt`, baseline, allowlist or doc change is landed. This note is the
only commit.

Gate at the base (fresh `cmake --build cmake-build-match --target check`):

```text
verify: byte 4507/4524  linked 4507/4524  size 4524/4524  avg 99.94
data:   exact 843/843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0  imports 207
symbol parity 5370/5370 (0 extra / 0 missing)
lint CLEAN ✓   drift 0
RATCHET PASS
```

Section deltas (fresh measurement, canonical build):

| section | size | differing bytes |
|---|---:|---:|
| `.text` | 1,259,464 | **85,535** |
| `.ARM.extab` | 156,312 | **1,395** |
| `.note.gnu.build-id` | 36 | **20** |
| `.rodata`, `.data`, `.bss`, `.ARM.exidx`, `.rel.plt` | exact | **0** |

Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`,
2,187,328 B.

---

## 0. Bottom line

1. **The `-fprofile-use` knob is now tested with a *valid* IR-level profile,
   not a synthetic text profile whose hash is rejected.** w8119 used a
   frontend-instrumented profile (and a synthetic text profile). This pass
   builds the whole tree with `-fprofile-generate` (IR-level), reconstructs a
   real `.profraw` from the object files' own `__llvm_prf_data` /
   `__llvm_prf_cnts` / `__llvm_prf_names` sections, and merges it. The result
   is a large regression: **byte_exact 1249/4524**, `.text` 1,266,252 B
   (+6,788 B), `.ARM.extab` 156,624 B. This matches w8119's FE uniform result
   (1250) and confirms the original is not PGO-built.
2. **The IR `FuncHash` is 64-bit, not 32-bit.** `__llvm_prf_data` is
   `NameRef` (u64) + `FuncHash` (u64) + `CounterPtr`/`FunctionPointer`/`Values`
   (u32 each) + `NumCounters` (u32) + `NumValueSites[3]` (u16) = 40 B on ARM.
   A text profile that carries only the low 32 bits is silently rejected with
   `Function control flow change detected (hash mismatch)` and the profile is
   **not applied** — which is why a "uniform" text profile looked neutral in
   the first pass. The correct hash for `Status::missionCompleted` is
   `1224980072979520487`, not `3672136679`.
3. **`-fprofile-use` and `-fprofile-instr-use` are the same driver flag in
   clang 7** (`-fprofile-instrument-use-path=...`). The only difference is the
   profile *format* (IR vs frontend); the reader auto-detects it. So the
   w8119 FE result and this IR result are two encodings of the same experiment.
4. **No build flag raises `byte_exact`.** The re-opened knobs are all neutral
   or negative on the full gate (matrix below). The canonical flag set is the
   original's.
5. **`-mvectorize-with-neon-quad` is a GCC-only flag**; clang 7 rejects it
   (`unknown argument`). `-mllvm -enable-machine-outliner` crashes clang 7 on
   this tree (`VirtualInput.cpp`), and `-mllvm -enable-merge-functions` is not
   a clang 7 backend option.

**Verdict: the original is a plain `-Oz -g` (== `-g2`) build with heuristic
branch weights. The 17 residual functions remain source-shape, not build
configuration.**

---

## 1. Method

A scratch build (`/dev/shm/w8319/build`) uses a compiler launcher that appends
`$GOF2_EXTRA_FLAGS` after the canonical flags:

```sh
#!/bin/sh
exec "$@" $GOF2_EXTRA_FLAGS
```

Each cell deletes all `CMakeFiles/gof2.dir` objects, relinks the whole tree
(`link_so -j6`, ~10 s), and scores with a fast byte-only scorer
(`/dev/shm/w8319/score.py`) that reproduces `verify.py`'s `byte_exact` exactly
(canonical 4507, 17 bad). Section diffs are computed against the original with
the same `Elf32` reader.

Per-TU cells use a second launcher that appends `$GOF2_TU_FLAGS` only when the
source basename equals `$GOF2_TU`.

### 1.1 Building a valid IR-level `.profdata` without running the game

`-fprofile-generate` is IR-level (`-fprofile-instrument=llvm`); the instrumented
objects carry one `__llvm_prf_data` + one `__llvm_prf_cnts` COMDAT section per
function and one `__llvm_prf_names` section per TU. The raw profile format
(`InstrProfData.inc`, version 4) is:

```
header:  magic, version|(1<<56), DataSize(records), CountersSize(u64s),
         NamesSize, CountersDelta, NamesDelta, ValueKindLast
data:    DataSize * 40-byte ProfileData records
counters:CountersSize * u64
names:   the concatenated __llvm_prf_names sections (each [uleb usize][uleb csize][data])
```

`mkraw.py` pairs each `__llvm_prf_data` section with its `__llvm_prf_cnts`
section, patches each record's `CounterPtr` to the global counter byte offset
(`CountersDelta = 0`), zeroes `Values`/`NumValueSites` (no value profile), and
writes all counters as 1. `llvm-profdata merge` then produces a valid
`.profdata` with 4,285 functions and the exact 64-bit `FuncHash` of every
function. `-fprofile-use=<that file>` compiles with **zero hash-mismatch
warnings**.

---

## 2. PGO measurements

| build | byte_exact | `.text` size | `.text` diff | `.ARM.extab` size |
|---|---:|---:|---:|---:|
| canonical (no profile) | **4507** | 1,259,464 | **85,535** | 156,312 |
| IR uniform whole-tree profile (all counters 1) | 1249 | 1,266,252 | 1,223,236 | 156,624 |
| IR partial profile (Status only, uniform) | 1378 | 1,259,432 | — | 156,312 (content diff 2,060) |
| FE uniform whole-tree profile (w8119) | 1250 | — | 1,220,251 | 137,436 |

The partial profile is **not** scoped to one function: `-fprofile-use` treats
every function missing from the profile as cold, so the whole tree re-rolls
(`.text` shrinks by 32 B and 3,146 functions change). This is the same
limitation w8119 found for the FE Status-only profile (1421).

**Interpretation.** A whole-program profile sets branch weights on every
branch; the original's block layout is the static heuristic layout in 1,957 of
1,973 branch-containing functions (w8119 census). A uniform profile is a
regression because clang's static heuristic is *not* uniform (loops, `if`
bias, `__builtin_expect`). The original is not PGO-built.

---

## 3. Full-gate flag matrix

All cells are a full link of the whole tree with only the named flag appended
to every compile. Canonical is `byte_exact 4507`, `.text` diff 85,535.

### 3.1 Neutral (`byte_exact 4507`, all sections byte-identical)

| flag | note |
|---|---|
| `-g2` | `-g` is already `-g2` |
| `-gdwarf-2` / `-gdwarf-3` / `-gdwarf-4` | debug info is stripped; `-g3` was already neutral in w8119 |
| `-fmerge-all-constants` | `.rodata` still exact |
| `-ffp-contract=fast` | VFPv3 has no FMA |
| `-fno-delete-null-pointer-checks` | no effect |
| `-fno-inline-functions-called-once` | no effect at `-Oz` |
| `-falign-functions=4` / `-falign-functions=16` | ignored with `-ffunction-sections` |
| `-fno-vectorize` | loop vectorizer already off at `-Oz` |

### 3.2 Negative (`byte_exact < 4507`)

| flag | byte_exact | `.text` size (canonical 1,259,464) |
|---|---:|---:|
| `-g0` | 2071 | 1,259,464 (content diff 804,580) |
| `-gmlt` / `-gline-tables-only` | 2071 | 1,259,464 (content diff 804,580) |
| `-g1` (w8119) | 2071 | 1,259,464 |
| `-fno-math-errno` | 1356 | 1,259,064 |
| `-fvisibility=hidden` | 57 / 244 common | 1,249,080 |
| `-fno-unwind-tables` | 2878 | 1,259,464 (content diff 101,478) |
| `-fno-exceptions` | 1351 | 1,168,548 |
| `-fvisibility-inlines-hidden` | 1278 / 4383 common | 1,259,432 |
| `-fno-strict-overflow` | 1418 | 1,259,688 |
| `-fno-jump-tables` | 1363 | 1,263,816 |
| `-mvectorize-with-neon-quad` | build error (GCC flag) | — |
| `-mllvm -enable-machine-outliner` | clang 7 crash (`VirtualInput.cpp`) | — |
| `-mllvm -enable-merge-functions` | unknown backend option | — |

`-fvisibility=hidden` and `-fvisibility-inlines-hidden` also collapse the
exported `.dynsym` (the verify "common" set shrinks to 244 / 4,383), so they are
hard symbol-parity failures, not just codegen regressions.

### 3.3 Per-TU `-fno-exceptions` (13 residual TUs)

Every cell is negative; the best are `LevelScript.cpp` (2049) and
`HangarWindow.cpp` (2058). The cleanup TUs lose their LSDA/landing pads and
`.ARM.extab` shrinks (e.g. `Level.cpp` 156,312 -> 145,860), so the original
needs exceptions on in every residual TU.

### 3.4 Per-TU `-fno-unwind-tables` (13 residual TUs)

Most cells are `byte_exact 4044` with `.ARM.exidx` 8 B short. Four cells are
`byte_exact 4507` but still regress sections:

| TU | byte_exact | `.ARM.exidx` | `.ARM.extab` diff |
|---|---:|---|---:|
| `MGame.cpp` | 4507 | size -8 | 2,004 |
| `Gun.cpp` | 4507 | content diff 794 | size -12 |
| `HangarWindow.cpp` | 4507 | content diff 894 | size -12 |
| `Generator.cpp` | 4507 | size -8 | 2,004 |

Since `.ARM.exidx` is currently byte-identical, no per-TU `-fno-unwind-tables`
cell can land.

---

## 4. Reproduction

```sh
# canonical gate
cd /home/debian/GalaxyOnFire2/wt/w8319
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6

# scratch build with the flag launcher
cmake -S . -B /dev/shm/w8319/build -G Ninja \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs \
  -DGOF2_MATCH_OPT=-Oz \
  -DCMAKE_C_COMPILER_LAUNCHER=/dev/shm/w8319/launch.sh \
  -DCMAKE_CXX_COMPILER_LAUNCHER=/dev/shm/w8319/launch.sh

# one cell
GOF2_EXTRA_FLAGS=-fmerge-all-constants \
  cmake --build /dev/shm/w8319/build --target link_so -j6
python3 /dev/shm/w8319/score.py /dev/shm/w8319/build/libgof2hdaa.so

# valid IR-level PGO profile (does not run the game)
cmake -S . -B /dev/shm/w8319/build_irgen -G Ninja \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs \
  -DGOF2_MATCH_OPT=-Oz \
  -DCMAKE_C_COMPILER_LAUNCHER=/dev/shm/w8319/launch.sh \
  -DCMAKE_CXX_COMPILER_LAUNCHER=/dev/shm/w8319/launch.sh
GOF2_EXTRA_FLAGS="-fprofile-generate -mllvm -enable-name-compression=false" \
  cmake --build /dev/shm/w8319/build_irgen --target gof2 -j6
python3 /dev/shm/w8319/mkraw.py /dev/shm/w8319/build_irgen /dev/shm/w8319/all_ir.profraw
llvm-profdata merge /dev/shm/w8319/all_ir.profraw -o /dev/shm/w8319/all_ir.profdata
GOF2_EXTRA_FLAGS="-fprofile-use=/dev/shm/w8319/all_ir.profdata" \
  cmake --build /dev/shm/w8319/build --target link_so -j6
```

Scratch (not committed): `/dev/shm/w8319/` — `launch.sh`, `launch_tu.sh`,
`cell.sh`, `cell_tu.sh`, `score.py`, `sec.py`, `mkraw.py`, `build/`,
`build_tu/`, `build_irgen/`, `all_ir.profraw`, `all_ir.profdata`,
`status_partial.profdata`, `matrix1.txt`, `matrix2.txt`, `matrix_tu_exc.txt`,
`matrix_tu_unwind.txt`.

---

## 5. What would falsify this note

* A whole-program `-fprofile-use` build whose `.text` matches the original
  better than 4507 while keeping the 1,957 branch-function matches. The valid
  IR uniform profile drops to 1249.
* A flag that raises `byte_exact` above 4507 without changing `.text` size.
  None of the ~40 cells in this pass (plus w8119's 60) does.
* Evidence that the original's `!prof` weights are empirical rather than
  heuristic. The 1,957/1,973 heuristic-layout match is the current evidence
  against it.
