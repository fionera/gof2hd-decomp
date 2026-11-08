# Build provenance round 5 (w11019): the residual is not a compile flag; the
# r18-beta1/r18/r18b clang binaries are byte-identical; one 2-byte matching knob

Status: **research note + one landed per-TU matching knob**, 2026-11-08.
Worktree `wt/w11019`, base master `b3d3a1f68` (wave 163). Scratch
`/dev/shm/w11019`. Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Gate on entry: **RATCHET PASS**, `byte_exact 4507`, `linked_exact 4507`,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0, parity
5370/5370. `.text` **84,652** differing bytes, `.ARM.extab` **725**,
`.rel.plt` **0** (byte-identical), `.note.gnu.build-id` 20. All 27 section
sizes exact; file size 2,187,328 B.

Gate on exit (this commit): **RATCHET PASS**, same counts, `.text`
**84,650** (-2), `.ARM.extab` **725**, `.rel.plt` **0**, build-id 19.
The only landed change is a one-line per-TU `-mllvm
-disable-postra-machine-sink` on `LevelScript.cpp` (see §7).

## 0. Bottom line

1. **The 17 residuals are not a build-flag difference.** About 2,400 per-TU
   compile cells were measured at object level (every residual TU × every
   hypothesis flag) and every size-exact mover was then link-measured. The
   only size-exact, `byte_exact`-neutral raw improvement is **-2 bytes**
   (`LevelScript.cpp -mllvm -disable-postra-machine-sink`, 54,541 -> 54,539
   raw, aligned unchanged). It is landed as a matching knob.
2. **The r18 family is one compiler binary.** `clang` in NDK
   **r18-beta1**, **r18** and **r18b** is the same 53,695,608-byte file,
   sha256 `af8268675ca343698d3fae4bca21b0563762442204cbbc34555c8ddcf362376a`.
   The r18/r18b CHANGELOG confirms r18b only changed ndk-build scripts; the
   clang build `4751641 / r328903` was introduced in r18. So the
   "different clang minor / different r18 package" hypothesis is closed.
3. **LTO (full and Thin) is disproven by section sizes, freshly measured.**
   Canonical `.text`/`.rodata`/`.ARM.exidx`/`.ARM.extab` are exactly the
   original's; ThinLTO is -324/-320/-1,160/-11,160 B and drops 277 exported
   symbols, full LTO is -24,996/-5,872/-3,112/-13,468 B and drops 355.
4. **The specific residual classes are not flag-controlled.**
   * `and` operand order: `-dataflow-edge-limit` (0/1/2/4/8/16),
     `-recurrence-chain-limit` (0/1/2/4/8), `-disable-2addr-hack` and
     `-twoaddr-reschedule=false` are **exactly neutral on all 13 TUs**.
   * callee-saved ties: `-regalloc-csr-first-time-cost` (0/1/2/4/8/16/100),
     `-join-globalcopies/-liveintervals/-splitedges`, `-pbqp-coalescing`,
     `-reverse-csr-restore-seq`, `-regalloc=basic`,
     `-optimize-regalloc=false` are neutral on every target function or
     size-changing.
   * SLP group bases: `-fno-slp-vectorize`, `-slp-threshold`,
     `-slp-max-reg-size`, `-slp-min-reg-size`, `-slp-recursion-max-depth`,
     `-slp-schedule-budget` are neutral or size-changing; the only
     size-neutral mover (`ModStation -slp-schedule-budget=0/1/2`) is +10 raw.
   * LSDA rows: `.ARM.extab` is downstream of `.text`; `-fno-unwind-tables`
     and `-fno-asynchronous-unwind-tables` are neutral, `-fno-exceptions` is
     size-changing.
   * block placement: `-disable-block-placement`,
     `-block-placement-exit-block-bias`, `-branch-fold-placement`,
     `-tail-dup-placement`, `-tail-merge-size`, `-misched-postra` are all
     worse; only the `-disable-postra-machine-sink` cell is a (2-byte) win.
5. **`-fno-reorder-blocks` is not a clang flag.** clang 7.0.2 prints
   `warning: optimization flag '-fno-reorder-blocks' is not supported
   [-Wignored-optimization-argument]` and emits the canonical object. The
   clang spelling of the hypothesis is `-mllvm -disable-block-placement`,
   which is strongly negative (e.g. `Radar::draw` -16 B but raw +1,445).
6. **PGO/sample-profile is still the only profile route and still worse.**
   The round-4 result reproduces exactly on this base: the committed
   `status_sample_w10117.prof` gives 1,400/607/1,464 (vs 722/320/1,432).
   See `build-provenance-round4-w10920.md`.
7. **No other flag or compiler version makes a residual byte-exact.** The
   canonical `-Oz` build remains the best measured configuration; the
   residual is source shape / TU shape.

## 1. Baseline

`raw` = same-address differing bytes; `aligned` = instruction-index
alignment; `size` = function bytes. Pre-landing baseline (this worktree):

| function | mangled | raw | aligned | size |
|---|---|---:|---:|---:|
| `Layout::Layout` (C1/C2) | `_ZN6LayoutC1Ev` | 5,705 | 2,082 | 7,072 |
| `LevelScript::process` | `_ZN11LevelScript7processEi` | 54,541 | 17,427 | 57,232 |
| `Level::createShip` | `_ZN5Level10createShipEiiiP8Waypointbb` | 1,563 | 449 | 2,764 |
| `Level::createMission` | `_ZN5Level13createMissionEv` | 5,454 | 786 | 17,172 |
| `Status::missionCompleted` | `_ZN6Status16missionCompletedEbbx` | 722 | 320 | 1,432 |
| `Radar::draw` | `_ZN5Radar4drawEP6PlayerP3Hudi` | 8,124 | 1,320 | 9,908 |
| `MGame::OnTouchEnd` | `_ZN5MGame10OnTouchEndEiiPv` | 4,127 | 906 | 8,080 |
| `MGame::OnUpdate` | `_ZN5MGame8OnUpdateEv` | 16 | 6 | 11,440 |
| `Gun::calcCharacterCollision` | `_ZN3Gun22calcCharacterCollisionEv` | 463 | 57 | 1,984 |
| `HangarWindow::render` | `_ZN12HangarWindow6renderEv` | 2,514 | 485 | 9,700 |
| `SpaceLounge::drawLounge` | `_ZN11SpaceLounge10drawLoungeEv` | 175 | 41 | 1,948 |
| `Hud::draw` | `_ZN3Hud4drawExxP9PlayerEgobjj` | 1,015 | 229 | 9,332 |
| `Generator::createMission` | `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 162 | 64 | 1,996 |
| `MenuTouchWindow::OnTouchBegin` | `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 28 | 16 | 1,896 |
| `ModStation::ModStation` (C1/C2) | `_ZN10ModStationC1Ev` | 43 | 12 | 1,008 |
| **unique-body sum** | | **84,652** | **24,200** | |

The unique raw sum is exactly the `.text` differing-byte count.

## 2. Method

Two stages, both in `/dev/shm/w11019`:

1. **Object-level filter** (`filter_par.py`). For each (TU, flag) cell the
   canonical compile command is reproduced from `build.ninja` and the flag
   appended; the target function's `.text.<mangled>` section bytes and size
   are compared with the canonical object. `-ffunction-sections` makes this
   exact. A cell is a *candidate* only if the target function changes, its
   size is unchanged, and the whole TU's `.text` size is unchanged (so the
   link layout cannot shift).
2. **Link measurement** (`sweep2.py`). Every candidate is built in a
   scratch link directory with a compiler launcher that appends the flag only
   for that TU, then scored with `tools/fn_diff.py` (raw + aligned) and the
   official `tools/verify/verify.py` gate. The neutral build is restored and
   re-scored between cells.

The TUs are the 13 residual TUs (`Layout`, `LevelScript`, `Level`, `Status`,
`Radar`, `MGame`, `Gun`, `HangarWindow`, `SpaceLounge`, `Hud`, `Generator`,
`MenuTouchWindow`, `ModStation`). Cells per matrix: 1,430 (inlining / SLP /
unroll / block placement / commutativity / regalloc / scheduling / EH), 520
(2addr / coalescing / spill / ARM passes), 468 (math / aliasing / sections /
RTTI / visibility). 125 cells timed out or were rejected by clang; they are
listed in §11.

### 2.1 Per-TU `-O` levels (re-confirmed)

The wave-157/158 13-TU x 5-level matrix reproduces on this tree: every
non-`-Oz` level changes the target function size or the TU `.text` size, so
every cell is gate-fatal. Examples (target size, TU `.text` size; canonical
`-Oz` in parentheses):

| TU | `-Oz` | `-O1` | `-O2` | `-Os` | `-O3` |
|---|---|---|---|---|---|
| `Layout` | 7,072 / 17,146 | 8,012 / 18,314 | 7,120 / 17,674 | 7,072 / 17,276 | 7,276 / 17,950 |
| `LevelScript` | 57,232 / 67,120 | 61,620 / 71,990 | 63,460 / 73,690 | 57,728 / 67,358 | 66,460 / 77,070 |
| `Level` (`createShip`) | 2,764 / 102,458 | 3,060 / 110,976 | 3,064 / 114,108 | 2,988 / 106,188 | 3,080 / 126,092 |
| `Status` | 1,432 / 20,858 | 1,940 / 23,242 | 1,760 / 27,616 | 1,456 / 21,156 | 1,840 / 31,320 |
| `Radar` | 9,908 / 15,610 | 10,520 / 16,594 | 10,392 / 16,238 | 10,276 / 16,018 | 10,844 / 16,826 |
| `MGame` (`OnTouchEnd`) | 8,080 / 37,884 | 8,448 / 39,742 | 9,940 / 44,746 | 8,288 / 38,312 | 10,136 / 45,542 |
| `ModStation` | 1,008 / 28,884 | 908 / 30,186 | 1,016 / 31,692 | 1,008 / 29,230 | 1,088 / 33,820 |

`-Oz` is the unique size-exact level for all 13 TUs (the three `-Os` cells
that keep the target size still change the TU `.text` size). This is the
same conclusion as `build-provenance-wave157-w10420.md` /
`build-provenance-wave158-w10520.md`.

## 3. Inlining

`-fno-inline` and `-fno-inline-functions` are catastrophic: 11 of 13 TUs
change, 10 of those change size (the TU `.text` size changes in all 13).
`-fno-inline -finline-functions` is **exactly neutral on all 13 TUs** (the
`-Oz` inliner only does the always-inline work that `-finline-functions`
re-enables). `-finline-hint-functions` is also exactly neutral.

`-mllvm -inline-threshold`:

| value | TUs changed | size-neutral target | TU size unchanged | note |
|---:|---:|---:|---:|---|
| 0 | 1 | 0 | 11 | one size change |
| 1, 5 | 0 | 0 | 13 | **neutral** |
| 10 | 1 | 1 | 13 | `HangarWindow::render`, raw +8,710 |
| 50, 500, 1000 | 6-7 | 0 | 0 | size-changing |

`-inlinehint-threshold=0/50/500`, `-inline-cold-callsite-threshold=0/50`,
`-inlinecold-threshold=0/50`, `-max-num-inline-blocks=0/1/2`,
`-inline-cost-full` and `-disable-preinline` are **exactly neutral on all
13 TUs**. The inliner is not the residual.

## 4. SLP / loop vectorization / unrolling

`-fno-vectorize` is neutral on all 13 TUs (the loop vectorizer is already
off at `-Oz`). `-fno-unroll-loops`, `-mllvm -unroll-threshold=0/1/100`,
`-unroll-count=1` and the `-unroll-allow-*` off-switches are neutral on all
target functions (one TU text-size change for `-fno-unroll-loops`).

`-fno-slp-vectorize` changes 5 of 13 TUs and is size-changing on all 5
(`Layout::Layout` 7,072 -> 6,000). The SLP knobs:

| flag | TUs changed | size-neutral target | TU size unchanged |
|---|---:|---:|---:|
| `-slp-threshold=0/1` | 0 | 0 | 13 |
| `-slp-threshold=10/100` | 5 | 0 | 4 |
| `-slp-max-reg-size=32/64` | 5 | 0 | 4 |
| `-slp-max-reg-size=128` | 0 | 0 | 13 |
| `-slp-min-reg-size=8/16/32` | 6 | 0 | 1 |
| `-slp-min-tree-size=2/4` | 0 | 0 | 13 |
| `-slp-recursion-max-depth=2` | 3 | 0 | 9 |
| `-slp-recursion-max-depth=4/8` | 0 | 0 | 13 |
| `-slp-schedule-budget=0/1/2` | 5 | 2 | 7 |
| `-slp-vectorize-hor[-store]=false` | 0 | 0 | 13 |
| `-run-slp-after-loop-vectorization` | 0 | 0 | 13 |

The only size-neutral SLP mover is `ModStation.cpp -slp-schedule-budget=0/1/2`
(10 object bytes), which is **+10 raw** at link time. `-mllvm
-vectorize-slp=false` is a no-op because the driver emits `-vectorize-slp`
after the `-mllvm` argument; `-fno-slp-vectorize` is the working spelling.

## 5. Block placement, scheduling, register allocation, commutativity

The full matrix is in `/dev/shm/w11019/cells_all_results.json` etc.; the
link-measured candidates are:

| TU | flag | byte | raw | aligned | verdict |
|---|---|---|---:|---:|---|
| baseline | — | 5,348 | 84,652 | 24,200 | |
| Layout | `-mllvm -tail-dup-placement=false` | 5,348 | 84,693 | 24,218 | worse |
| Layout | `-mllvm -tail-merge-size=0` | 5,347 | 84,650 | 24,200 | raw -2 but one exact fn changed |
| **LevelScript** | **`-mllvm -disable-postra-machine-sink`** | **5,348** | **84,650** | **24,200** | **win (-2)** |
| Level | `-mllvm -block-placement-exit-block-bias=50` | 5,347 | 86,987 | 24,334 | worse |
| Level | `-mllvm -disable-postra-machine-sink` | 5,347 | 84,662 | 24,201 | worse |
| Level | `-mllvm -enable-aa-sched-mi` | 5,332 | 84,735 | 24,221 | worse |
| Radar | `-mllvm -enable-aa-sched-mi` | 5,345 | 84,658 | 24,204 | worse |
| Radar | `-mllvm -disable-block-placement` | 5,332 | 86,097 | 27,305 | worse |
| MGame | `-mllvm -tail-dup-placement-penalty=0` | 5,348 | 84,652 | 24,208 | aligned +8 |
| MGame | `-mllvm -enable-deferred-spilling` | 5,347 | 84,955 | 24,572 | worse |
| MGame | `-mllvm -disable-postra-machine-sink` | 5,348 | 84,677 | 24,211 | worse |
| Gun | `-mllvm -enable-aa-sched-mi` | 5,341 | 84,660 | 24,202 | worse |
| HangarWindow | `-mllvm -inline-threshold=10` | 2,600 | 93,362 | 25,545 | worse |
| HangarWindow | `-mllvm -enable-deferred-spilling` | 5,346 | 86,553 | 25,081 | worse |
| HangarWindow | `-mllvm -disable-postra-machine-sink` | 5,347 | 84,709 | 24,222 | worse |
| SpaceLounge | `-mllvm -enable-aa-sched-mi` | 5,339 | 84,662 | 24,204 | worse |
| Generator | `-mllvm -disable-postra-machine-sink` | 5,348 | 84,664 | 24,204 | worse |
| ModStation | `-mllvm -slp-schedule-budget=0` | 5,347 | 84,662 | 24,208 | worse |
| ModStation | `-mllvm -enable-aa-sched-mi` | 5,340 | 84,696 | 24,207 | worse |

(`byte` is the fast scorer's 5,348-of-5,365 common symbols, i.e. the official
4,507-of-4,524; the two zero-size exports and the 17 residuals are the
difference.)

Other exactly-neutral groups (0 target bytes on all 13 TUs):
`-mllvm -dataflow-edge-limit=0/1/2/4/8/16`,
`-mllvm -recurrence-chain-limit=0/1/2/4/8`,
`-mllvm -disable-2addr-hack`, `-mllvm -twoaddr-reschedule=false`,
`-mllvm -regalloc-csr-first-time-cost=0/1/2/4/8/16/100` (target functions;
some TUs' other functions move), `-mllvm -join-globalcopies=false`,
`-mllvm -join-liveintervals=false`, `-mllvm -join-splitedges=false`,
`-mllvm -pbqp-coalescing`, `-mllvm -reverse-csr-restore-seq`,
`-mllvm -disable-spill-hoist`, `-mllvm -disable-spill-fusing`,
`-mllvm -align-neon-spills`, `-mllvm -enable-local-reassign`,
`-mllvm -enable-misched=false`, `-mllvm -enable-post-misched=false`,
`-mllvm -misched=ilpmax/ilpmin`, `-mllvm -max-sched-reorder=0/1/100`,
`-mllvm -block-placement-exit-block-bias=0`,
`-mllvm -tail-dup-placement-threshold=0/100`,
`-mllvm -tail-dup-placement-penalty=100`,
`-mllvm -enable-gvn-hoist=false`, `-mllvm -enable-gvn-sink=false`,
`-mllvm -disable-licm-promotion`, `-mllvm -enable-load-pre=true`,
`-mllvm -arm-promote-constant=false`, `-mllvm -arm-global-merge=false`,
`-mllvm -arm-synthesize-thumb-1-tbb=false`,
`-mllvm -disable-cgp-branch-opts`, `-mllvm -disable-early-ifcvt`,
`-mllvm -enable-if-conversion=false`, `-mllvm -enable-cond-stores-vec=false`,
`-mllvm -enable-interleaved-mem-accesses=false`,
`-mllvm -vectorizer-maximize-bandwidth`, `-mllvm -use-gvn-after-vectorization`,
`-mllvm -enable-shrink-wrap=false`, `-fno-reorder-blocks`.

`-mllvm -misched-postra=true` changes 11 TUs (10 size-neutral) but is worse
on every one (e.g. `Layout` byte 3,144, `Hud` raw +5,633). `-mllvm
-sink-insts-to-avoid-spills` (MGame, raw +3,444), `-mllvm
-tail-dup-size=0` and `-mllvm -disable-early-taildup` (LevelScript, raw +709)
are also worse.

## 6. Math / aliasing / sections / RTTI

`-fno-honor-nans` changes 5 TUs (3 size-neutral): `Radar` raw unchanged
(aligned +2), `Gun` raw +2, `Level` raw +2, `Layout` and `Status`
catastrophic (`byte_exact` 4,097 / 3,891). `-funsafe-math-optimizations` and
`-freciprocal-math` change 5 TUs (2 size-neutral) but are worse (`Radar`
aligned +4, `MGame` raw +582). `-fno-strict-overflow` and `-fwrapv` change
`Radar::draw` by one object byte but are net-zero at link time (raw and
aligned unchanged).

Exactly neutral on all 13 TUs: `-ffp-contract=off/fast`, `-fno-math-errno`,
`-fmath-errno`, `-fno-delete-null-pointer-checks`, `-fno-associative-math`,
`-fno-finite-math-only`, `-fno-signed-zeros` (one size-neutral target byte on
Radar), `-fno-threadsafe-statics`, `-fno-rtti`, `-fno-use-cxa-atexit`,
`-faligned-allocation`, `-fno-sized-deallocation`, `-fno-common`,
`-fdata-sections`, `-mno-movt`, `-mno-unaligned-access` (one Radar byte).

Size-changing (gate-fatal): `-fno-stack-protector`, `-fstack-protector`,
`-fno-builtin`, `-fno-strict-aliasing`, `-fno-jump-tables`,
`-fno-function-sections`, `-fvisibility=hidden`, `-fno-exceptions`,
`-fno-unwind-tables` (neutral here, but known to regress the launcher cell),
`-fno-asynchronous-unwind-tables`.

## 7. The one landed win: `LevelScript.cpp -mllvm -disable-postra-machine-sink`

`LevelScript::process` has an 8-byte difference at function offset `0x8d46`:
the canonical object emits `mov r4,r3; ...` in one order, the
`-disable-postra-machine-sink` object in another. Both are 57,232 B; the
whole `LevelScript.cpp` `.text` size is unchanged, so the link layout does
not move. At link time the reordering happens to match 2 more bytes of the
original:

| | raw | aligned | size | `.text` diff | `.ARM.extab` |
|---|---:|---:|---:|---:|---:|
| before | 54,541 | 17,427 | 57,232 | 84,652 | 725 |
| after | **54,539** | 17,427 | 57,232 | **84,650** | 725 |

`check` stays green: byte_exact **4507**, linked_exact **4507**, size
**4524/4524**, data **843/843**, imports **207**, lint CLEAN, drift 0,
parity **5370/5370**; `.rel.plt` stays byte-identical (0). The flag is added
to `CMakeLists.txt` as a per-file `COMPILE_OPTIONS` next to the existing
StarMap/SpaceLounge branch-weight knobs. It is a matching knob, not an
original build flag: the original almost certainly had the pass on (the
default), and the -2 is a coincidence in a 57 KB function. It is landed
because it is a real, gate-neutral reduction of the headline `.text` metric.

## 8. LTO / ThinLTO (fresh, this tree)

Built with `-flto`/`-flto=thin` and the NDK's `LLVMgold.so` plugin through
gold; same `-Oz` flags otherwise.

| build | `.text` | `.rodata` | `.ARM.exidx` | `.ARM.extab` | dynsym |
|---|---:|---:|---:|---:|---:|
| canonical | `0x1337c8` | `0x41b67` | `0x6f70` | `0x26298` | 5,370 |
| original | `0x1337c8` | `0x41b67` | `0x6f70` | `0x26298` | 5,370 |
| ThinLTO | `0x133684` (-324) | `0x41a27` (-320) | `0x6ae8` (-1,160) | `0x23700` (-11,160) | 5,093 (-277) |
| full LTO | `0x12d3a4` (-24,996) | `0x40477` (-5,872) | `0x6348` (-3,112) | `0x22dfc` (-13,468) | 5,015 (-355) |

LTO changes every section size and drops exported template/ctor bodies, so it
cannot be the original's build (the canonical section sizes are exact). This
reproduces `lto_w4307.md` on the wave-163 tree.

## 9. Clang minor / patched r18b

The three r18 packages were downloaded from `dl.google.com` and their
`toolchains/llvm/prebuilt/linux-x86_64/bin/clang` compared:

| package | Pkg.Revision | clang size | sha256 |
|---|---|---:|---|
| r18-beta1 | 18.0.4912477-beta1 | 53,695,608 | `af8268675ca343698d3fae4bca21b0563762442204cbbc34555c8ddcf362376a` |
| r18 | 18.0.5002713 | 53,695,608 | same |
| r18b | 18.1.5063045 | 53,695,608 | same |

The r18b `CHANGELOG.md` confirms r18b only changed ndk-build scripts; the
clang build `4751641 / r328903` was introduced in r18. The original's
`.comment` names exactly that build. **The r18 package choice is
codegen-irrelevant.**

A *locally patched* clang is unfalsifiable from metadata (the `.comment`
string is baked in), but it is unnecessary: 4,507 of 4,524 functions are
byte-identical to the stock r18 clang's output, and the 17 residuals differ
in ways that look like source shape (register names, block order, SLP group
bases), not like a compiler patch. A patch would have to be a no-op on 4,507
functions and change exactly the 17 residuals while preserving all 17 sizes.

## 10. PGO / sample profile (reproduced)

The round-4 harness reproduces on this base:
`status_sample_w10117.prof` on `Status::missionCompleted` gives
**1,400 / 607 / 1,464** (baseline 722/320/1,432). The sample-profile loader
ignores `BranchInst`, so the empty-`br` switch successors cannot be
weighted; the original's 4:3:2 plateau is only reachable by post-optimisation
IR `!prof` injection. Instrumented profiles also weight every other branch
and move the non-switch layout away. See
`build-provenance-round3-w10820.md` and `build-provenance-round4-w10920.md`.

## 11. Cells that failed

125 object cells did not produce an object (60 in the first matrix, 39 in the
second, 26 in the third):

* `-mllvm -misched=bottomup` / `-mllvm -misched=topdown` (26): not valid
  enum values in clang 7.0.2; the accepted spellings are `ilpmax`/`ilpmin`.
* `-mllvm -stress-regalloc=1/2` (26): clang crashes on these TUs.
* `-mllvm -slp-min-tree-size=0/1` (8): does not finish within 120 s on
  `Layout`/`LevelScript`/`Level`/`MGame` (the known wave-123 timeout).
* `-mllvm -enable-cgp=false` / `-mllvm -enable-cgp-branch-opts=true` (26):
  unknown; the options are negative (`-disable-cgp[=false]`,
  `-disable-cgp-branch-opts[=false]`).
* `-mllvm -enable-early-ifcvt` (13): unknown; only `-disable-early-ifcvt`
  exists.
* `-mllvm -arm-implicit-it=false` (13): it is a flag, not a boolean; the
  bare `-arm-implicit-it` is the correct spelling.

The valid-spelling cells are already covered by the neutral list in §5
(`-disable-cgp-branch-opts`, `-disable-early-ifcvt`).

## 12. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11019
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so verify check -j32

# object-level filter (scratch build with the per-TU launcher)
python3 /dev/shm/w11019/filter_par.py /dev/shm/w11019/cells_all.json 16
# link measurement of the candidates
python3 /dev/shm/w11019/sweep2.py /dev/shm/w11019/cells_link2.json
```

Scratch artifacts: `/dev/shm/w11019/{filter_par.py,sweep2.py,score.py,cells_*.json,*_results.json}`.

## 13. Files changed

* `_work/reconstructions/build-provenance-round5-flags.md` — this note.
* `CMakeLists.txt` — per-file `-mllvm -disable-postra-machine-sink` on
  `LevelScript.cpp` (the -2 matching knob).
* `docs/VALIDATION.md`, `docs/IDENTICALITY.md` — `.text` 84,652 -> 84,650,
  build-id 20 -> 19.
