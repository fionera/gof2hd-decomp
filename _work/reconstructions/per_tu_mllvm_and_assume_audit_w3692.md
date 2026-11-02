# Per-TU `-mllvm` flags and `__builtin_assume` audit (w3692)

Status: research note (2026-11-02). Worktree `wt/w3692`, branch `wave87-w3692`,
base `88aaf59f`. No `src/` or build-flag change landed: **no tested per-TU flag is a
strict gate improvement**, and the three `__builtin_assume` sites remain load-bearing.
Every number below is from a command run against
`../_work/bins/android_2.0.16_libgof2hdaa.so` or from a controlled rebuild of this tree.

## TL;DR

* Baseline (`cmake --build cmake-build-match --target check -j8`):
  **byte 4496 / linked 4496 / stubs 0 / missing 0 / extra 0 / avg 99.9 / lint CLEAN**,
  `.text 0x1337c8`; `drift` reports **0 fields across 0 classes**.
* 28 functions across 17 TUs are not `linked_exact`. Those are the only functions a
  per-TU flag can improve (byte_exact == linked_exact == 4496, so every linked-exact
  function is also byte-exact).
* **206 per-TU flag-set rows (204 unique; two driver flags were duplicated) x 17 TUs =
  3502 object compiles.** 0 functions became
  `linked_exact` under the oracle. The best fuzzy gains are either gate-neutral or change the function
  size (and therefore shift `.text` and regress downstream functions).
* Full-gate confirmation of the three most promising variants:
  `MenuTouchWindow 20:12` is **4496/4496** (neutral); `Level.cpp
  -sink-insts-to-avoid-spills` is **2499/4482**; `LevelScript.cpp
  -disable-peephole` is **2736/4491**.
* Exactly three `__builtin_assume` sites exist. Delete-only full checks:
  `String::Compare` **4495/4495** (fail), `TouchButton::draw` **2973/4495** (fail),
  `StarMap::draw` **4496/4496** (pass, but the function drops 94.8 -> 94.7%).
  A 206-flag sweep with the assume disabled on each TU finds **no flag that restores
  the assume-free codegen**.
* The one remaining scoped flag (`Radar.cpp`, `-likely-branch-weight=80
  -unlikely-branch-weight=1`) is load-bearing: removing it gives **3632/4496** (fail).

## 1. Baseline

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
readelf -SW cmake-build-match/libgof2hdaa.so | grep -E '\.text|\.rodata|\.data|\.bss'
```

```
verify: byte 4496  linked 4496  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
=== DRIFT: 0 field(s) across 0 class(es) ===
[12] .text    PROGBITS 00069440 069440 1337c8
[15] .rodata  PROGBITS 001c9e10 1c9e10 041b67
[21] .data    PROGBITS 00214000 213000 00296c
[22] .bss     NOBITS   00216970 21596c 0069ac
```

## 2. Method: a fast object-level oracle

A full `check` takes ~35 s; a single-TU compile takes 0.3-10 s. For screening, the real
TU was compiled with the exact command from `cmake-build-match/compile_commands.json`,
replacing only `-o` and appending the candidate flags. The function's
`.text.<mangled>` section was extracted with `objcopy`, and its **instruction stream**
was compared to the original function's instruction stream after:

* masking external-call immediate words (relocation targets) and resolving them by
  symbol name (the original's `blx <sym>` and the object's `bl <reloc-sym>` both
  normalize to `call <sym>`);
* masking literal-pool and `tbb`/`tbh` jump-table bytes;
* comparing intra-function branch targets as function-relative offsets.

The oracle and the sweep driver are committed alongside this note as
`w3692_oracle.py` and `w3692_sweep.py`. The oracle is a **screening tool**, not the
gate. It was validated against
`tools/verify/verify.py` on the current tree:

| function | verify pct | oracle pct | note |
|---|---:|---:|---|
| `_ZN7StarMap6updateEi` | 100.0 | 100.0 | byte-exact control |
| `_ZN7StarMap4drawEv` | 94.8 | 94.6 | non-linked control |
| `_ZN5MGame8OnUpdateEv` | 99.9 | 99.8 | non-linked control |
| `_ZN6LayoutC1Ev` | 46.8 | 46.6 | non-linked control |
| `_ZN11LevelScript7processEi` | 54.3 | 55.5 | largest deviation |

The oracle's `exact` flag is equivalent to the gate's `linked_exact` for the screened
function (same instruction stream and size); every candidate that the oracle called
exact was confirmed with a full `check`. The oracle deliberately ignores literal-pool
*values*, so it can miss a pool-only difference; it cannot invent an exact function.

The temporary A/B harness added to `CMakeLists.txt` (reverted before commit):

```cmake
if(GOF2_AB_TU AND GOF2_AB_FLAGS)
    set_property(SOURCE "${CMAKE_SOURCE_DIR}/src/${GOF2_AB_TU}"
                 APPEND PROPERTY COMPILE_OPTIONS ${GOF2_AB_FLAGS})
endif()
```

and the variant was built with:

```sh
cmake -S . -B cmake-build-match \
  -DGOF2_AB_TU='game/ui/MenuTouchWindow.cpp' \
  -DGOF2_AB_FLAGS='-mllvm;-likely-branch-weight=20;-mllvm;-unlikely-branch-weight=12'
cmake --build cmake-build-match --target link_so -j8
cmake --build cmake-build-match --target check -j8
```

`check` does not depend on `link_so`; after a reconfigure the `.so` must be rebuilt
explicitly before `check`, otherwise the ratchet sees the previous link.

## 3. Per-TU `-mllvm` sweep

The 17 TUs containing the 28 non-linked functions:

| TU | non-linked functions |
|---|---:|
| `game/ui/Layout.cpp` | 2 (`Layout::Layout` C1/C2) |
| `game/world/LevelScript.cpp` | 1 (`LevelScript::process`) |
| `game/mission/Status.cpp` | 1 (`Status::missionCompleted`) |
| `game/weapons/Radar.cpp` | 1 (`Radar::draw`) |
| `game/world/Level.cpp` | 3 (`createShip`, `createMission`, `createScene`) |
| `game/mission/Generator.cpp` | 1 (`Generator::createMission`) |
| `game/menu/MGame.cpp` | 2 (`OnTouchEnd`, `OnUpdate`) |
| `game/ui/HangarWindow.cpp` | 1 (`HangarWindow::render`) |
| `game/core/Globals.cpp` | 2 (`Globals::Globals` C1/C2) |
| `game/weapons/Gun.cpp` | 1 (`Gun::calcCharacterCollision`) |
| `game/weapons/RocketGun.cpp` | 1 (`RocketGun::update`) |
| `game/ship/PlayerEgo.cpp` | 1 (`PlayerEgo::calcCollision`) |
| `game/ui/Hud.cpp` | 2 (`Hud::draw`, `drawChallengeModeScore`) |
| `game/ui/MenuTouchWindow.cpp` | 4 (C1/C2, `OnTouchEnd`, `OnTouchBegin`) |
| `game/world/StarMap.cpp` | 2 (`draw`, `drawOnScreenInfo`) |
| `game/menu/SpaceLounge.cpp` | 1 (`SpaceLounge::drawLounge`) |
| `game/menu/ModStation.cpp` | 2 (`ModStation::ModStation` C1/C2) |

Flag sets (206 rows, 204 unique):

* 125 unique `-mllvm` options transcribed from
  `_work/reconstructions/mllvm_flag_audit.md` (scheduling/regalloc, block placement,
  IR optimisation, ARM backend, and the default-toggle controls);
* 37 added `-mllvm` combinations/values (branch-weight pairs, `-stress-regalloc=4/8/16`,
  `-regalloc-csr-first-time-cost=1/3`, `-tail-dup-size=8/16/32`,
  `-tail-merge-size=8/16`, `-jump-table-density=5/10`,
  `-block-placement-exit-block-bias=10/50`, `-inline-threshold=50/200`,
  `-unroll-threshold=50/100`, `-sink-freq-percent-threshold=10/50`, and the
  `=true/false/default` forms of the default toggles);
* 44 non-`-mllvm` flags (`-O0..-O3`, `-Os`, `-fno-inline-functions-called-once`,
  `-fno-delete-null-pointer-checks`, `-fno-threadsafe-statics`,
  `-fomit-frame-pointer`, `-fno-stack-protector`, `-fno-unroll-loops`,
  `-fno-vectorize`, `-fno-slp-vectorize`, `-fno-jump-tables`, `-mno-movt`,
  `-fno-sized-deallocation`, `-fno-rtti`, `-fno-strict-aliasing`, `-fwrapv`,
  `-fno-strict-overflow`, `-mno-unaligned-access`, `-fno-omit-frame-pointer`,
  `-fno-common`, `-fno-builtin`, `-mcpu=cortex-a8/a9/a15`, `-mthumb`, `-fno-inline`,
  `-finline-functions`, `-fmerge-all-constants`, `-fno-optimize-sibling-calls`,
  `-fno-ipa-cp`, `-fno-exceptions`, `-fno-unwind-tables`, ...).

50 of the 3502 compiles failed to build (counted as no result, not as a regression):

| failure | count |
|---|---:|
| `-mllvm -enable-machine-outliner=true` (clang 7.0.2 crash) | 17 |
| `-fno-ipa-sra` (unknown argument in clang 7.0.2) | 17 |
| `-mllvm -stress-regalloc=4` (clang 7.0.2 crash) | 11 |
| Radar branch-weight variants (duplicate `-likely-branch-weight`; Radar already has 80:1) | 4 |
| `-fno-exceptions` (source uses exceptions) | 1 |

**Result: 0 of the 28 functions became instruction-exact (the oracle's `linked_exact`
equivalent) under any of the 206 flag-set rows.**
The best fuzzy improvements are:

| function | flag | oracle pct | function size |
|---|---|---:|---:|
| `Level::createScene` | `-mllvm -sink-insts-to-avoid-spills` | 87.8 -> 89.0 | 2252 -> 2256 |
| `MenuTouchWindow::OnTouchEnd` | `-mllvm -likely-branch-weight=20 -mllvm -unlikely-branch-weight=12` | 93.5 -> 94.1 | 8132 (same) |
| `LevelScript::process` | `-mllvm -disable-peephole` | 55.5 -> 55.9 | 57232 -> 57248 |
| `SpaceLounge::drawLounge` | `-mllvm -likely-branch-weight=20 -mllvm -unlikely-branch-weight=12` | 95.3 -> 95.5 | 1948 (same) |
| `Layout::Layout` C1/C2 | `-mllvm -branch-fold-placement=false` | 46.6 -> 46.7 | 7072 (same) |
| `MenuTouchWindow` C1/C2 | `-mllvm -disable-postra-machine-sink` | 91.7 -> 91.8 | 9600 (same) |

A pairwise sweep of 40 scheduling/regalloc/IR flags (780 pairs) on `MGame.cpp` and
`StarMap.cpp` also produced **no exact flip** for `MGame::OnUpdate`,
`MGame::OnTouchEnd`, `StarMap::drawOnScreenInfo` or `StarMap::draw`.

### Full-gate confirmation of the size-neutral and size-changing leaders

| variant | byte | linked | verdict |
|---|---:|---:|---|
| baseline | **4496** | **4496** | PASS |
| `MenuTouchWindow.cpp` + `20:12` | **4496** | **4496** | PASS (gate-neutral) |
| `Level.cpp` + `-sink-insts-to-avoid-spills` | 2499 | 4482 | FAIL (+4 B function size shifts `.text`) |
| `LevelScript.cpp` + `-disable-peephole` | 2736 | 4491 | FAIL (+16 B function size shifts `.text`) |

The only gate-neutral variant (`MenuTouchWindow 20:12`) does not change
`byte_exact`/`linked_exact`; the fuzzy gain is real but the gate counts are identical.
No per-TU flag is a strict improvement.

## 4. `__builtin_assume` audit

There are exactly three `__builtin_assume` uses in `src/`:

| # | site | assume |
|---|---|---|
| 1 | `src/game/core/String.cpp:566` (`String::Compare(const char*)`) | `!reachedEnd \|\| d == -1` |
| 2 | `src/game/ui/TouchButton.cpp:544` (`TouchButton::draw`) | `canvasTop != 0` |
| 3 | `src/game/world/StarMap.cpp:876` (`StarMap::draw`) | `!(transitionIn && transitionOut)` |

The delete-only A/B used the preprocessor, so the source was not edited:

```sh
cmake -S . -B cmake-build-match \
  -DGOF2_AB_TU='game/core/String.cpp' \
  -DGOF2_AB_FLAGS='-D__builtin_assume(x)='
cmake --build cmake-build-match --target link_so -j8
cmake --build cmake-build-match --target check -j8
```

Full-gate results at the current HEAD:

| site | delete-only gate | function |
|---|---|---|
| `String::Compare` | **byte 4495 / linked 4495** (FAIL) | 68 B, 100.0 -> 96.7% |
| `TouchButton::draw` | **byte 2973 / linked 4495** (FAIL) | 1376 -> 1380 B, 100.0 -> 93.9% |
| `StarMap::draw` | **byte 4496 / linked 4496** (PASS) | 2644 B, 94.8 -> 94.7% |

These reproduce the w3523 conclusion at the current baseline: sites 1 and 2 are
required for byte-exactness; site 3 is gate-neutral but is a 0.1% fuzzy regression, so
deleting it is not an improvement.

### Can a per-TU flag replace an assume?

The same 206 flag-set rows were swept with `-D__builtin_assume(x)=` on each of the three
TUs. The best result for each function was the baseline assume-off result itself:

| TU | assume-off baseline | best flag result |
|---|---|---|
| `String.cpp` | 68 B, 96.7% | 68 B, 96.7% (all flags) |
| `TouchButton.cpp` | 1380 B, 93.9% | 1380 B, 93.9% (all flags) |
| `StarMap.cpp` | 2644 B, 94.5% (oracle) | 2644 B, 94.5% (all flags) |

No tested `-mllvm` or driver flag recovers the assume-free codegen. The three assumes
stay, each with its `// lint: arm_builtin` waiver.

The mechanisms (from the w3523 note, re-confirmed by the size/pct changes here) are:
site 1 keeps the subtraction in i32 so the sign extension stays after the select;
site 2 keeps the `canvasTop` pointer live without emitting an instruction (a 4-byte
register-allocation change otherwise shifts `.text`); site 3 lets clang simplify the
second guard using the mutual-exclusion fact.

## 5. The remaining scoped `-mllvm` flag

`CMakeLists.txt` still scopes `-mllvm -likely-branch-weight=80
-mllvm -unlikely-branch-weight=1` to `Radar.cpp`. Removing that line and rebuilding:

```
verify: byte 3632  linked 4496  stubs 0  missing 0  extra 0  avg 99.9
  FAIL  byte_exact 4496 -> 3632
```

`linked_exact` is unchanged, so the flag changes instruction selection/pool placement
inside Radar functions, not their instruction streams. It is a real compensation for a
source/TU-shape mismatch, not a neutral build flag.

## 6. Proven vs hypothesis

**Proven by measurement**

* Baseline is byte 4496 / linked 4496 / `.text 0x1337c8` / lint CLEAN / drift 0.
* 28 functions in 17 TUs are not linked-exact; all other 4496 compared functions are
  both linked- and byte-exact.
* 206 per-TU flag-set rows (204 unique) on all 17 TUs produce 0 linked-exact flips. The four best
  fuzzy improvements are either gate-neutral or size-changing, and the size-changing
  ones fail the gate (2499/4482 and 2736/4491).
* The three `__builtin_assume` sites are still load-bearing: delete-only full checks
  give 4495/4495, 2973/4495 and 4496/4496 (the last with a 94.8 -> 94.7% fuzzy drop).
* No tested per-TU flag recovers the assume-free codegen on any of the three TUs.
* Radar's 80:1 flag is load-bearing: removing it gives 3632/4496.

**Hypothesis / limits**

* The original app TUs are consistent with LLVM 7.0.2's default codegen; the remaining
  28 functions are source/TU shape, not a missing compiler flag. This is the same
  conclusion as `mllvm_flag_audit.md` and `per_tu_flags_research.md`, now re-measured
  at the current baseline.
* A flag outside the 204 unique flags tested (or a combination outside the 780 pairs tested) could
  in principle flip a function; the search is exhaustive over the listed space, not
  over all LLVM options. The zero-exact result is a strong negative, not a proof.
* The oracle ignores literal-pool values; a pool-only difference is invisible to it.
  Every exact candidate would still need the full `check`, and none was found.

## 7. Reproduction

```sh
# baseline
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8

# single-TU A/B (add the CMake block from section 2, then)
cmake -S . -B cmake-build-match \
  -DGOF2_AB_TU='game/ui/MenuTouchWindow.cpp' \
  -DGOF2_AB_FLAGS='-mllvm;-likely-branch-weight=20;-mllvm;-unlikely-branch-weight=12'
cmake --build cmake-build-match --target link_so -j8
cmake --build cmake-build-match --target check -j8

# assume-off A/B (same harness)
cmake -S . -B cmake-build-match \
  -DGOF2_AB_TU='game/core/String.cpp' \
  -DGOF2_AB_FLAGS='-D__builtin_assume(x)='
cmake --build cmake-build-match --target link_so -j8
cmake --build cmake-build-match --target check -j8

# restore the baseline
git checkout -- CMakeLists.txt
cmake -S . -B cmake-build-match -DGOF2_AB_TU= -DGOF2_AB_FLAGS=
cmake --build cmake-build-match --target link_so -j8
cmake --build cmake-build-match --target check -j8
```
