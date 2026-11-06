# Build-flag sensitivity matrix B, wave 123 (w7320)

Status: research note, 2026-11-06. Worktree `/home/debian/GalaxyOnFire2/wt/w7320`,
HEAD `1c8c9b20e` (`report: wave-122 final snapshot`). Scratch work is under
`/dev/shm/w7320`. **No `src/`, `tools/`, baseline, allowlist, `CMakeLists.txt` or
`cmake/` change is landed.** This note is the only commit.

The assignment was to answer one question: *are the remaining non-exact functions
explained by build flags we have not discovered, or by source shape?* The answer
from 258 full-link global variants and 140 per-TU cells is unambiguous: **source
shape**. No tested flag raises a single target's official fuzzy percentage, and no
tested flag is a plausible missing global original flag.

Canonical gate, measured before and after this pass (unchanged):

```text
verify: byte 4506  linked 4506  size 4524  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS   lint CLEAN ✓   drift 0
```

Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`,
sha256 `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`.

## 0. Bottom line

* **258 global full-link variants** were built and scored (257 completed; only
  `-fno-common` is a real link failure, see §6). **132 are gate-neutral at
  4506/4506/4524.** **Zero improve
  any of the 18 targets** — the maximum target delta across all 257 scored
  variants is exactly **+0.0**.
* **140 per-TU `(TU, flag)` cells** were built, linked and fully verified across
  the 13 TUs that own the 18 targets. **Zero improve any target.** 28 cells are
  gate-green (13 baseline controls + 15 real flag cells); of those, **7 are
  target-neutral** (same rounded percentage, same byte count) and the rest lower a
  target.
* The best *sub-rounding* movement is **−2 diff bytes** on
  `Radar::draw` (`-mllvm -likely-branch-weight=500` on `Radar.cpp`), i.e. 0.02 %,
  invisible at the official one-decimal precision. The best global sub-rounding
  movement is **−1 diff byte** on `MGame::OnTouchEnd`
  (`-mllvm -tail-dup-placement-penalty=0`), also invisible.
* Several flags are **gate-neutral but unobservable** on matched code:
  `-fno-vectorize`, `-fno-omit-frame-pointer`, `-fstrict-aliasing`, `-fmath-errno`,
  `-ffp-contract=off`, `-ffp-contract=fast`, `-fno-delete-null-pointer-checks`,
  `-fno-data-sections`, `-ffunction-sections`, `-fno-function-sections`,
  `-fvisibility=default`, `-fno-sized-deallocation`, `-faligned-allocation`,
  `-fshort-wchar`, `-fno-plt`, `-fno-rtti-data`, and ~100 `-mllvm` knobs. They
  cannot be distinguished from the current recipe and therefore cannot be
  "discovered" from the binary.
* Flags that *do* move code are almost all strongly negative. The recipe's
  explicit choices (`-Oz -g -fPIC -frtti -fstack-protector-strong
  -ffunction-sections -funwind-tables`) are confirmed as the unique best
  combination in the tested space, and the clang-default frame-pointer,
  math-errno and fp-contract settings are consistent with the binary.
* **Conclusion:** the 18 residuals are source-shape (and, for the low scorers,
  TU-shape) residuals, not build-flag artifacts. The remaining flag work is
  archaeology on unobservable defaults, not a route to byte-exactness.

## 1. Baseline and targets

The scratch baseline `/dev/shm/w7320/base-build` was configured from this
worktree with the canonical recipe and reproduced the canonical report exactly
(`byte_exact 4506`, `linked_exact 4506`, `size_exact 4524`, `avg 99.94`).

The 18 non-exact functions, their baseline fuzzy score, size, and owning TU:

| function | baseline % | size | TU |
|---|---:|---:|---|
| `_ZN6LayoutC1Ev` | 48.7 | 7072 | `src/game/ui/Layout.cpp` |
| `_ZN6LayoutC2Ev` | 48.7 | 7072 | `src/game/ui/Layout.cpp` |
| `_ZN11LevelScript7processEi` | 60.3 | 57232 | `src/game/world/LevelScript.cpp` |
| `_ZN6Status16missionCompletedEbbx` | 71.0 | 1432 | `src/game/mission/Status.cpp` |
| `_ZN5Level10createShipEiiiP8Waypointbb` | 73.9 | 2764 | `src/game/world/Level.cpp` |
| `_ZN5Radar4drawEP6PlayerP3Hudi` | 74.5 | 9908 | `src/game/weapons/Radar.cpp` |
| `_ZN5MGame10OnTouchEndEiiPv` | 81.7 | 8080 | `src/game/menu/MGame.cpp` |
| `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 89.9 | 1996 | `src/game/mission/Generator.cpp` |
| `_ZN5Level13createMissionEv` | 92.3 | 17172 | `src/game/world/Level.cpp` |
| `_ZN5Level11createSceneEv` | 92.8 | 2252 | `src/game/world/Level.cpp` |
| `_ZN3Gun22calcCharacterCollisionEv` | 93.4 | 1984 | `src/game/weapons/Gun.cpp` |
| `_ZN12HangarWindow6renderEv` | 94.0 | 9700 | `src/game/ui/HangarWindow.cpp` |
| `_ZN3Hud4drawExxP9PlayerEgobjj` | 95.3 | 9332 | `src/game/ui/Hud.cpp` |
| `_ZN11SpaceLounge10drawLoungeEv` | 95.7 | 1948 | `src/game/menu/SpaceLounge.cpp` |
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 98.1 | 1896 | `src/game/ui/MenuTouchWindow.cpp` |
| `_ZN10ModStationC1Ev` | 99.2 | 1008 | `src/game/menu/ModStation.cpp` |
| `_ZN10ModStationC2Ev` | 99.2 | 1008 | `src/game/menu/ModStation.cpp` |
| `_ZN5MGame8OnUpdateEv` | 99.9 | 11440 | `src/game/menu/MGame.cpp` |

All 18 are size-exact. 13 TUs own them (Level.cpp owns three, MGame.cpp and
ModStation.cpp two each). All 13 are `exact(HD)` original game TUs per
`_work/original_layout/tu_mapping.md`; no invented or split TU is involved.

## 2. Method

Two independent matrices were run.

### 2.1 Global full-link matrix (258 variants)

For each candidate flag set:

1. Configure a fresh build from this worktree with
   `-DGOF2_ORIGINAL_SO=... -DGOF2_FMOD_LIBDIR=... -DGOF2_MATCH_OPT=-Oz` and the
   candidate flags appended to both `CMAKE_CXX_FLAGS` and `CMAKE_C_FLAGS`
   (`/dev/shm/w7320/toolchain_extra.cmake`). The canonical per-file
   `-mllvm -likely-branch-weight=7 -mllvm -unlikely-branch-weight=1` overrides on
   `StarMap.cpp`/`SpaceLounge.cpp` stay in place unless the variant is a global
   branch-weight test (then `-DGOF2_NO_PER_FILE_WEIGHTS=ON` is used, because clang
   7 rejects a repeated option).
2. Build `link_so` (full gold link + GNU strip + shstrtab reorder).
3. Score all 18 targets with `_work/reconstructions`-independent
   `/dev/shm/w7320/score_targets.py` (same ELF/dynsym extraction and difflib
   normalization as `tools/verify/verify.py`).
4. Full-verify 155 selected variants with the official
   `tools/verify/verify.py` to confirm gate counts.

### 2.2 Per-TU matrix (140 cells)

For each `(TU, flag)` cell:

1. Configure a fresh build with
   `-DCMAKE_PROJECT_INCLUDE=/dev/shm/w7320/per_tu_flags.cmake`, which sets
   `COMPILE_FLAGS` on exactly one source file
   (`set_source_files_properties("${CMAKE_SOURCE_DIR}/${GOF2_AB_TU}"
   PROPERTIES COMPILE_FLAGS "${GOF2_AB_FLAGS}")`). Every other TU keeps the
   canonical recipe.
2. Build `link_so`, score all 18 targets, and full-verify every cell with the
   official verifier (140/140 completed).

The 140 cells are: for each TU, the flags from the 261-flag screen that changed
that TU's target bytes while leaving every exact function in the TU exact, plus a
curated set of extra candidates for TUs with few such flags, plus one empty-flag
baseline control per TU. Cell counts per TU: Layout 14, LevelScript 22, Radar 17,
Generator 12, MGame 9, Status 9, HangarWindow 9, MenuTouchWindow 9, Gun 9,
Level 9, SpaceLounge 7, ModStation 6, Hud 8.

### 2.3 Candidate space

`/dev/shm/w7320/candidates.txt` has 261 entries covering:

* optimization level: `-O0 -O1 -O2 -O3 -Os -Oz`;
* inlining: `-fno-inline -finline-functions`, `-mllvm -inline-threshold=N`
  (0/1/5/10/25/50/100/225/500/1000), `-inlinehint-threshold`,
  `-inline-cold-callsite-threshold`, `-max-num-inline-blocks`,
  `-inline-cost-full`, `-disable-preinline`;
* SLP/loop vectorization: `-fno-slp-vectorize -fno-vectorize`,
  `-mllvm -slp-threshold=N`, `-slp-min-tree-size`, `-slp-max-reg-size`,
  `-slp-min-reg-size`, `-slp-schedule-budget`, `-run-slp-after-loop-vectorization`,
  `-extra-vectorizer-passes`, `-enable-cond-stores-vec=false`;
* unrolling: `-fno-unroll-loops`, `-mllvm -unroll-threshold=N`,
  `-unroll-count=N`, `-unroll-partial-threshold=N`, `-unroll-max-count=N`,
  `-unroll-full-max-count=N`, `-unroll-allow-{partial,runtime,peeling,remainder}=false`;
* block placement / tail duplication: `-mllvm -disable-block-placement`,
  `-branch-fold-placement=false`, `-tail-dup-placement=false`,
  `-tail-dup-placement-penalty=N`, `-tail-dup-placement-threshold=N`,
  `-tail-dup-placement-aggressive-threshold=N`, `-block-placement-exit-block-bias=N`,
  `-tail-dup-size=N`, `-tail-merge-size=N`, `-enable-tail-merge=false`,
  `-disable-early-taildup`, `-disable-tail-duplicate`, `-enable-shrink-wrap=false`;
* register allocator: `-mllvm -regalloc=greedy/basic/fast/pbqp`,
  `-optimize-regalloc=false`, `-pbqp-coalescing`, `-stress-regalloc=N`,
  `-regalloc-csr-first-time-cost=N`, `-disable-spill-hoist`;
* scheduling: `-enable-misched=false`, `-enable-post-misched=false`,
  `-misched=ilpmax/ilpmin/bottomup/topdown`, `-disable-sched-*`,
  `-pre-RA-sched=list-*`, `-max-sched-reorder=N`, `-misched-limit=N`,
  `-enable-aa-sched-mi`, `-disable-dfa-sched`, `-disable-2addr-hack`,
  `-twoaddr-reschedule=false`;
* frame pointer / stack protector: `-fomit-frame-pointer`,
  `-fno-omit-frame-pointer`, `-fstack-protector`, `-fstack-protector-all`,
  `-fno-stack-protector`;
* exceptions/RTTI: `-fno-exceptions`, `-fno-rtti`, `-fno-rtti-data`;
* aliasing/overflow: `-fno-strict-aliasing`, `-fstrict-aliasing`,
  `-fno-strict-overflow`, `-fwrapv`, `-fno-delete-null-pointer-checks`;
* math: `-fno-math-errno`, `-fmath-errno`, `-ffp-contract=off/on/fast`,
  `-fno-honor-nans`, `-fno-honor-infinities`, `-fno-signed-zeros`,
  `-fno-trapping-math`, `-fno-rounding-math`, `-funsafe-math-optimizations`,
  `-ffast-math`, `-freciprocal-math`, `-fno-associative-math`,
  `-fno-finite-math-only`;
* data/section layout: `-fdata-sections`, `-fno-data-sections`,
  `-ffunction-sections`, `-fno-function-sections`, `-fno-common`;
* visibility/ABI: `-fvisibility=default/hidden`, `-fno-threadsafe-statics`,
  `-fno-use-cxa-atexit`, `-fno-sized-deallocation`, `-faligned-allocation`,
  `-fno-aligned-allocation`, `-fshort-wchar`, `-fno-short-wchar`, `-fno-builtin`,
  `-fno-plt`;
* branch weights / predictability: `-mllvm -likely-branch-weight=N`,
  `-unlikely-branch-weight=N`, `-min-predictable-branch=N`,
  `-cold-branch-ratio=N`, `-enable-block-placement-stats`, `-jump-is-expensive`,
  `-arm-adjust-jump-tables=false`, `-enable-gvn-hoist/sink`, `-enable-ipra`,
  `-enable-andcmp-sinking=false`, `-disable-cgp-{branch-opts,select2branch}`,
  `-disable-branch-fold`, `-simplifycfg-*`.

Prior waves already covered parts of this space; this note extends
`build-flag-differential-matrix-w5020.md` (wave 100, 192-TU `-O` sweep),
`worst_function_flag_sensitivity_w3751.md`,
`untested_compile_flags_w3695.md`, `mllvm_backend_screen_w5210.md`,
`per-tu-flag-sensitivity-wave120-w7005.md` (wave 120, 450 cells / 14 TUs) and
`optimization_level_per_tu_w5215.md` with the current 18-function set, the
full-link global matrix, and the `-mllvm` backend knobs.

## 3. Global matrix results

257 of 258 variants completed and scored (all six original build failures were
re-run; only `-fno-common` is a real link failure). Distribution of full-link
`byte_exact`/`linked_exact`:

| band | variants |
|---|---:|
| 4506 / 4506 (gate-neutral) | 132 |
| 4500–4505 | 1 |
| 4000–4499 | 3 |
| 1000–3999 | 116 |
| < 1000 | 5 |
| build failure | 1 |

**Target deltas:** `maxd > 0` for **0/257** variants. 101 variants changed at
least one target's fuzzy percentage, all in the negative direction
(`maxd = 0.0`, `mind < 0`). The best target movement is therefore `+0.0`.

Representative results (full official `verify.py` unless marked †; † = fast screen,
which uses the same ELF/difflib scoring code):

| variant | flags | byte | linked | target effect |
|---|---|---:|---:|---|
| `Oz` | `-Oz` (baseline) | 4506 | 4506 | none |
| `O0` † | `-O0` | 61 | 253 | every target −48…−96 |
| `O1` † | `-O1` | 1033 | 1781 | every target −37…−81 |
| `O2` † | `-O2` | 1168 | 2157 | every target −5…−79 |
| `O3` † | `-O3` | 1167 | 2117 | every target −12…−71 |
| `Os` † | `-Os` | 1263 | 2650 | every target −2…−73 |
| `fno_inline` | `-fno-inline` | 1219 | 3513 | targets −0…−54 |
| `fno_slp_vectorize` † | `-fno-slp-vectorize` | 1484 | 4385 | targets −0…−31 |
| `fno_vectorize` | `-fno-vectorize` | 4506 | 4506 | **neutral** |
| `fno_stack_protector` | `-fno-stack-protector` | 1369 | 3783 | targets −0…−32 |
| `fstack_protector_all` | `-fstack-protector-all` | 83 | 968 | massive loss |
| `fomit_frame_pointer` † | `-fomit-frame-pointer` | 1298 | 1920 | targets −6…−51 |
| `fno_omit_frame_pointer` | `-fno-omit-frame-pointer` | 4506 | 4506 | **neutral** |
| `fno_rtti` | `-fno-rtti` | 1701 | 4506 | 2823 functions change; **no target change** |
| `fno_rtti_data` | `-fno-rtti-data` | 4506 | 4506 | **neutral** |
| `fno_math_errno` | `-fno-math-errno` | 1356 | 4495 | 3168 functions change; **no target change** |
| `fmath_errno` | `-fmath-errno` | 4506 | 4506 | **neutral** |
| `ffp_contract_off` | `-ffp-contract=off` | 4506 | 4506 | **neutral** |
| `ffp_contract_fast` | `-ffp-contract=fast` | 4506 | 4506 | **neutral** |
| `ffp_contract_on` | `-ffp-contract=on` | 2076 | 4474 | targets −0…−0.9 |
| `fdata_sections` | `-fdata-sections` | 4315 | 4506 | 199 functions byte-shift, targets unchanged |
| `fno_data_sections` | `-fno-data-sections` | 4506 | 4506 | **neutral** |
| `ffunction_sections` | `-ffunction-sections` | 4506 | 4506 | **neutral** |
| `fno_function_sections` | `-fno-function-sections` | 4506 | 4506 | **neutral** |
| `fvisibility_hidden` | `-fvisibility=hidden` | 57 | 243 | massive loss |
| `fvisibility_default` | `-fvisibility=default` | 4506 | 4506 | **neutral** |
| `fno_common` | `-fno-common` | — | — | link fail (duplicate `g_android_back_button_pressed`) |
| `fno_jump_tables` | `-fno-jump-tables` | 1363 | 4417 | targets −0…−23 (LevelScript 60.3→37.6, Generator 89.9→68.0) |
| `fno_unroll_loops` | `-fno-unroll-loops` | 2886 | 4499 | `Level::createScene` 92.8→74.4 |
| `mllvm_disable_cgp_branch_opts` | `-mllvm -disable-cgp-branch-opts` | 4505 | 4505 | only `AbyssEngine::Engine::SetUVMatrix` newly inexact |
| `mllvm_tail_dup_placement_penalty_0` | `-mllvm -tail-dup-placement-penalty=0` | 4506 | 4506 | only `MGame::OnTouchEnd` bytes change; 5550→5549 diff bytes, pct 81.7 unchanged |

The 132 gate-neutral variants are listed in §7. The important point is not the
list length; it is that **none of them changes a target percentage**, and the
ones that change target bytes at all do so by ≤2 diff bytes.

### 3.1 The one near-miss gate count

`-mllvm -disable-cgp-branch-opts` is the only variant that lands at 4505/4505
rather than 4506/4506. It does **not** touch any of the 18 targets; it makes
`_ZN11AbyssEngine6Engine11SetUVMatrixERKNS_6AEMath6MatrixE` newly inexact. So it
is not a candidate original flag either — it trades one exact function for
nothing.

### 3.2 Branch-weight variants

Global `-likely-branch-weight`/`-unlikely-branch-weight` tests must use
`-DGOF2_NO_PER_FILE_WEIGHTS=ON`, which removes the existing
`StarMap.cpp`/`SpaceLounge.cpp` 7:1 overrides. That alone drops `byte_exact` to
2405–4000, so the global variants are *not* gate-neutral. The per-TU matrix shows
the same knobs applied to a single TU (Radar) are gate-neutral and move
`Radar::draw` by −1/−2 diff bytes with no rounded change. The existing 7:1
overrides are matching knobs, not evidence for a global default.

## 4. Per-TU matrix results

140/140 cells built, linked, scored and full-verified. **`maxd > 0` for 0/140
cells.** 28 cells are gate-green at 4506/4506; 7 of those are target-neutral
(`maxd = mind = 0`):

| TU | flag | gate | target diff-byte movement |
|---|---|---|---|
| `MGame.cpp` | `-mllvm -tail-dup-placement-penalty=0` | 4506/4506 | `OnTouchEnd` 5550→5549 (−1), pct 81.7 unchanged |
| `HangarWindow.cpp` | `-mllvm -disable-postra-machine-licm` | 4506/4506 | `render` 2071→2089 (+18), pct 94.0 unchanged |
| `Radar.cpp` | `-mllvm -likely-branch-weight=500` | 4506/4506 | `draw` 8028→8026 (−2), pct 74.5 unchanged |
| `Radar.cpp` | `-mllvm -likely-branch-weight=1000` | 4506/4506 | `draw` 8028→8027 (−1), pct 74.5 unchanged |
| `Radar.cpp` | `-mllvm -unlikely-branch-weight=2` | 4506/4506 | `draw` 8028→8027 (−1), pct 74.5 unchanged |
| `Level.cpp` | `-mllvm -disable-postra-machine-sink` | 4506/4506 | none (0 diff bytes) |
| `LevelScript.cpp` | `-mllvm -disable-postra-machine-sink` | 4506/4506 | none (0 diff bytes) |

The remaining 15 gate-green flag cells change target bytes but *lower* a target.
Examples:

* `Generator.cpp + -fno-jump-tables` is gate-green 4506/4506 but drops
  `Generator::createMission` 89.9→68.0.
* `MGame.cpp + -mllvm -disable-postra-machine-sink` is 4506/4506 but drops
  `OnTouchEnd` 81.7→81.6 (and `OnUpdate` 99.9→99.9, +11 diff bytes).
* `Layout.cpp + -mllvm -tail-dup-placement=false` is 4506/4506 but drops the
  `Layout` ctors by 0.1.

Per-TU summary:

| TU | cells | gate-green | target-neutral | best `maxd` |
|---|---:|---:|---:|---:|
| `src/game/menu/MGame.cpp` | 9 | 3 | 2 | 0.0 |
| `src/game/menu/ModStation.cpp` | 6 | 1 | 1 | 0.0 |
| `src/game/menu/SpaceLounge.cpp` | 7 | 1 | 1 | 0.0 |
| `src/game/mission/Generator.cpp` | 12 | 4 | 1 | 0.0 |
| `src/game/mission/Status.cpp` | 9 | 2 | 1 | 0.0 |
| `src/game/ui/HangarWindow.cpp` | 9 | 2 | 2 | 0.0 |
| `src/game/ui/Hud.cpp` | 8 | 1 | 1 | 0.0 |
| `src/game/ui/Layout.cpp` | 14 | 3 | 1 | 0.0 |
| `src/game/ui/MenuTouchWindow.cpp` | 9 | 1 | 1 | 0.0 |
| `src/game/weapons/Gun.cpp` | 9 | 1 | 1 | 0.0 |
| `src/game/weapons/Radar.cpp` | 17 | 5 | 4 | 0.0 |
| `src/game/world/Level.cpp` | 9 | 1 | 2 | 0.0 |
| `src/game/world/LevelScript.cpp` | 22 | 3 | 2 | 0.0 |

## 5. Flag-family findings

* **Optimization level.** `-Oz` is uniquely best. `-O2`/`-Os`/`-O3`/`-O1`/`-O0`
  change every target and lower every target. This is the strongest single
  confirmation of the recipe and matches the wave-100 192-TU sweep.
* **Inlining.** `-fno-inline` is catastrophic (1219/3513). Every
  `-inline-threshold` value tested changes exact functions; none helps a target.
  The default `-Oz` inliner is correct.
* **SLP/vectorization.** `-fno-slp-vectorize` is strongly negative (1484/4385);
  `-fno-vectorize` is neutral (loop vectorizer is already off at `-Oz`). SLP
  thresholds 0/1/2/5/10/100 and tree/register-size knobs either change exact
  functions or lower a target. No positive cell.
* **Unrolling.** `-fno-unroll-loops` is negative (2886/4499, `Level::createScene`
  92.8→74.4). `-unroll-threshold=0`, `-unroll-count=N`,
  `-unroll-partial-threshold`, `-unroll-max-count`, `-unroll-full-max-count` are
  all neutral or negative. The default unroller is correct.
* **Block placement / tail duplication.** The most interesting family, because
  it produces the only sub-rounding improvements (`-tail-dup-placement-penalty=0`
  on MGame, `-likely-branch-weight=500` on Radar). But every variant that
  improves one target by 1–2 bytes either leaves the rounded percentage unchanged
  or worsens another target. `-disable-block-placement` is very negative
  (block placement is load-bearing).
* **Register allocator.** `-regalloc=greedy` is neutral (it is the default).
  `basic`/`fast`/`pbqp`, `-optimize-regalloc=false`, `-stress-regalloc` are all
  strongly negative. No positive cell.
* **Frame pointer / stack protector.** `-fno-omit-frame-pointer` is neutral
  (baseline); `-fomit-frame-pointer` is negative (1298/1920).
  `-fstack-protector-strong` is load-bearing: `-fno-stack-protector` is
  1369/3783 and `-fstack-protector-all` is 83/968. No positive cell.
* **Exceptions/RTTI.** `-fno-rtti` is negative (1701/4506, 2823 functions
  change); `-fno-rtti-data` is neutral. `-fno-exceptions` does not compile
  (`Level.cpp` has `try`). `-frtti` is confirmed.
* **Aliasing/overflow.** `-fstrict-aliasing` is neutral (default);
  `-fno-strict-aliasing` is negative. `-fwrapv`/`-fno-strict-overflow` are
  negative. No positive cell.
* **Math.** `-fmath-errno` is neutral (baseline); `-fno-math-errno` is strongly
  negative (1356/4495). `-ffp-contract=off` and `=fast` are neutral;
  `=on` is negative. `-ffast-math`, `-funsafe-math-optimizations`,
  `-freciprocal-math`, `-fno-honor-nans`, `-fno-signed-zeros` are all negative.
  The default math model is correct.
* **Data/section layout.** `-fdata-sections` keeps `linked_exact` at 4506 but
  drops `byte_exact` to 4315 (199 functions byte-shift); `-fno-data-sections` is
  neutral. Function sections are neutral either way. No target moves. Link/TU
  order was held fixed at `tools/source_order.txt`; reordering it is a TU-shape
  experiment, not a compiler flag, and is outside this matrix.
* **Visibility/ABI.** `-fvisibility=default` is neutral (baseline);
  `-fvisibility=hidden` is catastrophic (57/243). `-fno-sized-deallocation`,
  `-faligned-allocation`, `-fshort-wchar`, `-fno-plt`, `-fno-threadsafe-statics`
  are neutral or negative. No positive cell.
* **`-mllvm` backend knobs.** ~100 knobs are exactly neutral; the rest are
  negative. The only knob that changes a target without changing its rounded
  percentage is `-tail-dup-placement-penalty=0` (MGame, −1 diff byte) and the
  Radar branch weights (−1/−2 diff bytes). None is a gate improvement.

## 6. Build failures, caveats, and measurement notes

* **6 global variants failed to build on the first run:** `-fno-unroll-loops`,
  `-fno-jump-tables`, `-fno-common`, `-mllvm -disable-sched-reg-pressure`,
  `-mllvm -unroll-max-count=4`, `-mllvm -tail-dup-size=4`. Five were
  **concurrency artifacts** in the vendored-archive build (rebuilding the same
  variant serially succeeds). `-fno-common` is a real link failure: duplicate
  `g_android_back_button_pressed` in `BackButton.c` and `GOF2HD2012.c`. All five
  were re-run and full-verified: `-fno-jump-tables` 1363/4417,
  `-fno-unroll-loops` 2886/4499, `-mllvm -tail-dup-size=4` 1392/4288,
  `-mllvm -disable-sched-reg-pressure` 4506/4506 (neutral),
  `-mllvm -unroll-max-count=4` 4506/4506 (neutral).
* **3 full-verify variants were killed for time:** `O2`, `Os`,
  `mllvm_regalloc_fast`. Their fast-screen counts were already far below 4506, so
  the missing full verify does not affect any conclusion.
* **`-fno-exceptions` does not compile** (`try` in `Level.cpp`).
* **`-mllvm -likely-branch-weight` global variants** need
  `-DGOF2_NO_PER_FILE_WEIGHTS=ON` (clang 7 rejects a repeated option); this
  confounds the global branch-weight test with the removal of the per-file
  overrides, which is why the per-TU Radar cells are the clean evidence.
* **Unknown in clang 7.0.2:** `-mllvm -enable-unsafe-fp-math`,
  `-enable-no-nans-fp-math`, `-enable-no-infs-fp-math`,
  `-enable-finite-only-fp-math`. The driver spelling
  `-Xclang -menable-unsafe-fp-math` is accepted and was already disproven in
  wave 100 (byte_exact 1426).
* **Timeouts/crashes:** `-mllvm -slp-min-tree-size=0/1` timed out;
  `-mllvm -stress-regalloc=4` crashed the compiler. Neither is a plausible
  original flag.
* The fuzzy percentage is rounded to one decimal. A 1-byte change in a 10 KB
  function is ~0.01 %, so the "target-neutral" cells are neutral *at the official
  precision*, not necessarily byte-identical. The diff-byte column in §4 gives
  the unrounded movement.
* `-g`/`-g0` were covered by wave 120: `-g` is load-bearing (`-g0` changes 7
  target functions and drops `MenuTouchWindow::OnTouchBegin` 98.1→83.2). This
  wave kept `-g` fixed.

## 7. The 132 gate-neutral global variants

These are full-link `4506/4506/4524` with no target change. They are
**unobservable** on matched code: any subset could be added to the recipe without
changing the gate, so the binary cannot tell us whether the original used them.

```
Oz, finline_functions, fno_vectorize, fno_omit_frame_pointer, fno_rtti_data,
fstrict_aliasing, fmath_errno, ffp_contract_off, ffp_contract_fast,
fno_honor_infinities, fno_trapping_math, fno_rounding_math, fno_associative_math,
fno_finite_math_only, fno_delete_null_pointer_checks, fno_data_sections,
ffunction_sections, fno_function_sections, fvisibility_default,
fno_sized_deallocation, faligned_allocation, fno_aligned_allocation, fshort_wchar,
fno_short_wchar, fno_plt, mllvm_tail_dup_placement, mllvm_enable_block_placement_stats,
mllvm_disable_sched_reg_pressure, mllvm_unroll_max_count_4,
mllvm_regalloc_greedy, mllvm_pbqp_coalescing, mllvm_enable_misched_false,
mllvm_enable_post_misched_false, mllvm_pre_RA_sched_list_hybrid, mllvm_misched_converge,
mllvm_misched_ilpmax, mllvm_misched_ilpmin, mllvm_misched_bottomup, mllvm_misched_topdown,
mllvm_misched_regpressure_false, mllvm_disable_sched_critical_path,
mllvm_disable_sched_height, mllvm_disable_sched_live_uses, mllvm_disable_sched_stalls,
mllvm_disable_sched_vrcycle, mllvm_disable_early_ifcvt, mllvm_old_thumb2_ifcvt,
mllvm_disable_libcalls_shrinkwrap, mllvm_inline_cost_full, mllvm_disable_preinline,
mllvm_run_slp_after_loop_vectorization, mllvm_enable_cond_stores_vec_false,
mllvm_enable_if_conversion_false, mllvm_unroll_allow_partial_false,
mllvm_unroll_runtime_false, mllvm_unroll_allow_peeling_false,
mllvm_unroll_allow_remainder_false, mllvm_simplifycfg_dup_ret_false,
mllvm_simplifycfg_merge_cond_stores_aggressively_false, mllvm_disable_2addr_hack,
mllvm_disable_dfa_sched, mllvm_misched_cluster_false, mllvm_misched_fusion_false,
mllvm_misched_cyclicpath_false, mllvm_enable_andcmp_sinking_false,
mllvm_disable_cgp_select2branch, mllvm_disable_spill_hoist, mllvm_inline_threshold_1,
mllvm_inline_threshold_5, mllvm_inlinehint_threshold_0, mllvm_inlinehint_threshold_50,
mllvm_inlinehint_threshold_225, mllvm_inlinehint_threshold_500,
mllvm_inline_cold_callsite_threshold_0, mllvm_inline_cold_callsite_threshold_50,
mllvm_inline_cold_callsite_threshold_225, mllvm_max_num_inline_blocks_0,
mllvm_max_num_inline_blocks_1, mllvm_max_num_inline_blocks_2,
mllvm_max_num_inline_blocks_10, mllvm_unroll_threshold_0, mllvm_unroll_count_1,
mllvm_unroll_count_2, mllvm_unroll_count_4, mllvm_unroll_count_8,
mllvm_unroll_partial_threshold_0, mllvm_unroll_partial_threshold_50,
mllvm_unroll_partial_threshold_100, mllvm_unroll_partial_threshold_200,
mllvm_unroll_max_count_0, mllvm_unroll_max_count_1, mllvm_unroll_max_count_2,
mllvm_unroll_full_max_count_0, mllvm_unroll_full_max_count_1,
mllvm_unroll_full_max_count_2, mllvm_unroll_full_max_count_4, mllvm_slp_threshold_0,
mllvm_slp_min_tree_size_2, mllvm_slp_max_reg_size_128, mllvm_slp_min_reg_size_128,
mllvm_slp_schedule_budget_1000, mllvm_tail_dup_size_1,
mllvm_tail_dup_placement_threshold_0, mllvm_tail_dup_placement_threshold_1,
mllvm_tail_dup_placement_threshold_2, mllvm_tail_dup_placement_threshold_4,
mllvm_tail_dup_placement_threshold_8, mllvm_tail_dup_placement_aggressive_threshold_0,
mllvm_tail_dup_placement_aggressive_threshold_1,
mllvm_tail_dup_placement_aggressive_threshold_2,
mllvm_tail_dup_placement_aggressive_threshold_4,
mllvm_tail_dup_placement_aggressive_threshold_8, mllvm_tail_dup_placement_penalty_0,
mllvm_tail_dup_placement_penalty_1, mllvm_tail_dup_placement_penalty_10,
mllvm_block_placement_exit_block_bias_0, mllvm_regalloc_csr_first_time_cost_0,
mllvm_misched_limit_0, mllvm_misched_limit_1, mllvm_misched_limit_10,
mllvm_misched_limit_100, mllvm_max_sched_reorder_0, mllvm_max_sched_reorder_1,
mllvm_max_sched_reorder_10, mllvm_max_sched_reorder_100, mllvm_min_predictable_branch_0,
mllvm_min_predictable_branch_50, mllvm_min_predictable_branch_100,
mllvm_cold_branch_ratio_0, mllvm_cold_branch_ratio_1, mllvm_cold_branch_ratio_50,
mllvm_cold_branch_ratio_100
```

## 8. Conclusion and remaining root cause

The current recipe is not a guess that happens to work: it sits at the best gate
count (4506/4506) in a 258-variant global matrix, and 132 variants tie it only by
changing no matched code (and are therefore unobservable). Every alternative that
actually moves code makes the gate worse. No flag
raises a single one of the 18 targets, and the best sub-rounding movement is 2
bytes out of 9908 on one function.

Therefore:

* **Do not look for a missing global build flag as the route to the remaining 18
  functions.** The residual is source shape: expression form, statement order,
  live ranges, local-variable reuse, inlined-helper shape, and — for the low
  scorers (`Layout` ctors 48.7, `LevelScript::process` 60.3,
  `Status::missionCompleted` 71.0, `Level::createShip` 73.9,
  `Radar::draw` 74.5) — likely TU shape / missing inlined callees.
* **The only flag-shaped lever that survives is per-TU branch-weight /
  tail-duplication tuning**, and it is worth ≤2 diff bytes with no rounded
  change. It is a matching knob, not a gate win.
* **The unobservable flags in §7 are not worth chasing.** They cannot be
  confirmed or refuted from the binary; adding them would be unfalsifiable
  archaeology.

## 9. Artifacts

All under `/dev/shm/w7320` (scratch, not committed):

| file | contents |
|---|---|
| `base-build/verify/report.json` | canonical baseline report |
| `base-score.json` | baseline per-target fuzzy/diff-byte scores |
| `targets.json` | 18 targets + TU map + baseline counts |
| `candidates.txt` | 261 candidate flags |
| `screen.json` | 261×13 per-TU byte screen (3393 rows) |
| `global_results.json` | 258 global variants (build dirs, verify, changed lists) |
| `global_summary.json` | 258 global variants with target deltas |
| `global_fullverify.json` | 155 official full-verify reports |
| `fv_jt/`, `fv_ul/`, `fv_mllvm_*/` | full-verify reports for the 5 re-run build-failure variants |
| `pt_cells.json` | 140 `(TU, flag)` cells |
| `pt_results.json` | 140 cell builds/scores |
| `pt_summary.json` | 140 cells with target deltas |
| `pt_gate_summary.json` | 140 cells with gate counts |
| `pt_fullverify.json` | 140 official full-verify reports |
| `pt_scores/*.json` | per-cell per-target diff-byte scores |
| `run_pt.py`, `score_global.py`, `full_verify_global.py`, `full_verify_pt.py` | harness |
| `toolchain_extra.cmake`, `per_tu_flags.cmake` | global / per-TU flag injection |

No tracked file outside this note was modified. `git status --short` is empty
after the commit.
