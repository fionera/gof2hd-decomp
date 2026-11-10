# Build provenance round 6 (w11118): exhaustive per-TU `-mllvm` sweep; two more
# size-exact matching knobs (-6 LevelScript, -2 Radar)

Status: **research note + two landed per-TU matching knobs**, 2026-11-10.
Worktree `wt/w11118`, base master `85c1f88df` (wave 166). Scratch
`/dev/shm/w11118`. Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Gate on entry: **RATCHET PASS**, `byte_exact 4507/4524`, `linked_exact
4507/4524`, size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0,
parity 5370/5370, `.text` **83,147** differing bytes, `.ARM.extab` **659**,
`.rel.plt` **0** (byte-identical), build-id 20.

Gate on exit: **RATCHET PASS**, same counts, `.text` **83,139** (-8),
`.ARM.extab` **659**, `.rel.plt` **0**, build-id 20. The only changes are two
per-TU `COMPILE_OPTIONS` lines in `CMakeLists.txt`.

## 0. Bottom line

1. **5,712 per-TU compile cells were measured** (14 TUs x 343 primary flags,
   plus 63 supplemental scheduling/placement flags x 14 TUs, plus 28
   LevelScript/Radar combination cells). Every size-exact mover was then
   link-measured with the real gold link.
2. **Two new size-exact, `byte_exact`-neutral raw wins landed:**
   * `LevelScript.cpp -mllvm -machine-sink-split-probability-threshold=0`:
     `LevelScript::process` raw **54,247 -> 54,241** (-6), aligned
     **17,532 -> 17,532** (unchanged), size 57,232 B; `.text` **83,147 ->
     83,141**.
   * `Radar.cpp -mllvm -disable-postra-machine-licm`: `Radar::draw` raw
     **7,549 -> 7,547** (-2), aligned **1,446 -> 1,455** (+9), size 9,908 B;
     `.text` **83,141 -> 83,139**. Raw same-address bytes are the
     byte-identicality ground truth; the aligned +9 is a schedule difference,
     not a structural regression (4 bytes fixed, 2 broken).
3. **No flag makes any residual function byte-exact.** The residual is still
   source/TU shape. There is no positive evidence that the original used a
   different per-file flag for any of the 14 TUs; the two landed flags are
   matching knobs, not original build flags.
4. The round-5 `LevelScript.cpp -mllvm -disable-postra-machine-sink` knob is
   still present; the new threshold flag is additive on top of it.

## 1. Baseline (this worktree)

`raw` = same-address differing bytes; `aligned` = instruction-index alignment;
`size` = function bytes. Sum of the 17 residual bodies = `.text` 83,147.

| function | mangled | raw | aligned | size |
|---|---|---:|---:|---:|
| `Layout::Layout` C1/C2 | `_ZN6LayoutC1Ev` / `C2` | 5,688 each | 2,108 each | 7,072 |
| `LevelScript::process` | `_ZN11LevelScript7processEi` | 54,247 | 17,532 | 57,232 |
| `Level::createShip` | `_ZN5Level10createShipEiiiP8Waypointbb` | 1,560 | 455 | 2,764 |
| `Level::createMission` | `_ZN5Level13createMissionEv` | 5,436 | 767 | 17,172 |
| `Status::missionCompleted` | `_ZN6Status16missionCompletedEbbx` | 722 | 320 | 1,432 |
| `Radar::draw` | `_ZN5Radar4drawEP6PlayerP3Hudi` | 7,549 | 1,446 | 9,908 |
| `MGame::OnTouchEnd` | `_ZN5MGame10OnTouchEndEiiPv` | 4,087 | 917 | 8,080 |
| `MGame::OnUpdate` | `_ZN5MGame8OnUpdateEv` | 15 | 111 | 11,440 |
| `Gun::calcCharacterCollision` | `_ZN3Gun22calcCharacterCollisionEv` | 463 | 57 | 1,984 |
| `HangarWindow::render` | `_ZN12HangarWindow6renderEv` | 2,245 | 620 | 9,700 |
| `SpaceLounge::drawLounge` | `_ZN11SpaceLounge10drawLoungeEv` | 175 | 41 | 1,948 |
| `Hud::draw` | `_ZN3Hud4drawExxP9PlayerEgobjj` | 762 | 222 | 9,332 |
| `Generator::createMission` | `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 127 | 107 | 1,996 |
| `MenuTouchWindow::OnTouchBegin` | `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 28 | 16 | 1,896 |
| `ModStation::ModStation` C1/C2 | `_ZN10ModStationC1Ev` / `C2` | 43 each | 12 each | 1,008 |

## 2. Method

A three-stage harness in `/dev/shm/w11118` (`sweep.py`, `sweep_supp.py`,
`combos.py`, `link_sweep.py`):

1. **Object-level filter.** For each (TU, flag) cell the canonical compile
   command is taken from `ninja -t commands` (so the per-file StarMap /
   SpaceLounge / LevelScript knobs are already in it) and the candidate flag
   appended. The TU's object is recompiled and compared with the canonical
   object. A cell survives only if
   * every function in the TU keeps its exact size,
   * every **non-target** function keeps its exact bytes,
   * the target function's bytes change,
   * the TU `.text` section size is unchanged.
   This guarantees the link layout cannot move and the only linked difference
   is inside the target function.
2. **Link measurement.** Every survivor is linked in a scratch build
   (`/dev/shm/w11118/bdlink`) with a compiler launcher that appends the flag
   only for that TU. The canonical object of every one of the 14 TUs is
   restored before each cell (a flagged object left behind by the previous
   cell would otherwise contaminate the next measurement), the target object
   is recompiled, and the whole `.so` is relinked. `tools/fn_diff.py` gives
   raw/aligned per target function and a direct `.text` byte comparison
   against the original gives the headline metric.
3. **Official gate.** The landed cells were rebuilt in `cmake-build-match`
   and `check` was run.

Parallelism was kept at 4-6 concurrent single-TU clang invocations with
`ulimit -v 12000000`; full links used `-j2`.

## 3. Flag space swept

343 primary flags, all applied per TU:

* **Block placement / layout:** `-disable-block-placement`,
  `-block-placement-exit-block-bias={0,1,10,25,50,75,99,100}`,
  `-branch-fold-placement[=false]`, `-disable-branch-fold`,
  `-tail-dup-placement[=false]`,
  `-tail-dup-placement-threshold={0,1,2,4,8,16,32,100}`,
  `-tail-dup-placement-penalty={0,1,2,5,10,50,100}`,
  `-tail-dup-placement-aggressive-threshold={0,1,4,8,16,100}`,
  `-tail-merge-size={0,1,2,4,8,16,100}`,
  `-tail-merge-threshold={0,1,2,4,8,100}`, `-enable-tail-merge`,
  `-tail-dup-size={0,1,2,4,8,16,100}`,
  `-tail-dup-indirect-size={0,1,2,4,8,100}`,
  `-tail-dup-limit={0,1,2,4,8,100}`, `-disable-early-taildup`,
  `-disable-tail-duplicate`, `-triangle-chain-count={0,1,2,4,8,100}`,
  `-arm-adjust-jump-tables`, `-arm-implicit-it`, `-arm-default-it`,
  `-arm-restrict-it`, `-arm-no-restrict-it`, `-arm-load-store-opt`.
* **Scheduling:** `-enable-misched[=false]`, `-enable-post-misched[=false]`,
  `-misched-postra[=true]`, `-misched={default,converge,ilpmax,ilpmin}`,
  `-pre-RA-sched={default,list-burr,source,list-hybrid,list-ilp}`,
  `-max-sched-reorder={0,1,2,4,8,16,100}`,
  `-misched-limit={0,1,2,4,8,16,100}`, `-enable-aa-sched-mi[=false]`,
  `-disable-{sched-critical-path,sched-cycles,sched-hazard,sched-height,
  sched-live-uses,sched-physreg-join,sched-reg-pressure,sched-stalls,
  sched-vrcycle}`, `-disable-dfa-sched`, `-scheditins[=false]`,
  `-schedmodel[=false]`, `-post-RA-scheduler`, `-disable-post-ra`,
  `-disable-postra-machine-sink[=true]`, `-disable-machine-sink`,
  `-disable-postra-machine-licm`, `-disable-machine-licm`,
  `-machine-sink-bfi[=false]`, `-machine-sink-split[=false]`,
  `-machine-sink-split-probability-threshold={0,50,100}`,
  `-break-anti-dependencies={none,critical,all}`,
  `-dag-maps-huge-region={0,100,1000,10000}`,
  `-twoaddr-reschedule=false`, `-disable-2addr-hack`.
* **Register allocation:** `-regalloc={basic,greedy,pbqp,fast}`,
  `-optimize-regalloc=false`, `-regalloc-csr-first-time-cost={0,1,2,4,8,16,100}`,
  `-enable-deferred-spilling[=false]`, `-enable-local-reassign[=false]`,
  `-pbqp-coalescing`, `-join-globalcopies=false`,
  `-join-liveintervals=false`, `-join-splitedges=false`,
  `-split-spill-mode={default,aggressive}`, `-spp-rematerialization-threshold`,
  `-disable-spill-hoist`, `-disable-spill-fusing`, `-align-neon-spills`,
  `-disable-non-allocatable-phys-copy-opt`.
* **SLP / vectorization:** `-slp-threshold={0,1,2,4,10,100}`,
  `-slp-max-reg-size={32,64,128}`, `-slp-min-reg-size={8,16,32,64}`,
  `-slp-min-tree-size={2,3,4,8}`,
  `-slp-recursion-max-depth={1,2,4,8}`,
  `-slp-schedule-budget={0,1,2,4,8,16,100}`, `-slp-vectorize-hor[-store]`,
  `-run-slp-after-loop-vectorization`, `-fno-slp-vectorize`, `-fno-vectorize`,
  `-fno-unroll-loops`.
* **If-conversion / CGP / misc codegen:** `-disable-early-ifcvt`,
  `-early-ifcvt-limit`, `-ifcvt-limit`, `-disable-ifcvt-{diamond,
  forked-diamond,simple,simple-false,triangle,triangle-false,
  triangle-false-rev,triangle-rev}`, `-old-thumb2-ifcvt`,
  `-disable-cgp[-branch-opts,-select2branch,-ext-ld-promotion,-gc-opts,
  -store-extract]`, `-disable-constant-hoisting`,
  `-arm-promote-constant[=false]`, `-arm-promote-constant-max-size`,
  `-arm-promote-constant-max-total`, `-arm-global-merge=false`,
  `-arm-assume-misaligned-load-store`, `-arm-synthesize-thumb-1-tbb=false`,
  `-arm-constant-island-max-iteration`, `-disable-{copyprop,adv-copy-opt,
  machine-cse,machine-dce,peephole,ssc,lftr,shifter-op,
  phi-elim-edge-splitting,licm-promotion,complex-addr-modes,
  separate-const-offset-from-gep,memop-opt}`, `-enable-andcmp-sinking`,
  `-enable-arm-3-addr-conv`, `-enable-gvn-hoist`, `-enable-gvn-sink`,
  `-enable-load-pre=false`, `-enable-if-conversion=false`,
  `-enable-cond-stores-vec=false`, `-enable-interleaved-mem-accesses=false`,
  `-enable-shrink-wrap[=false]`, `-disable-libcalls-shrinkwrap`,
  `-enable-machine-outliner`, `-enable-linkonceodr-outlining`,
  `-use-tbaa-in-sched-mi`, `-enable-double-float-shrink`, `-enable-ipra`,
  `-disable-inlined-alloca-merging`.
* **Driver flags:** `-fno-jump-tables`, `-fno-strict-aliasing`,
  `-fno-builtin`, `-fno-honor-nans`, `-funsafe-math-optimizations`,
  `-fno-stack-protector`, `-fomit-frame-pointer`, `-mno-movt`,
  `-mno-unaligned-access`, `-fno-threadsafe-statics`, `-fno-common`,
  `-fdata-sections`, `-fno-function-sections`,
  `-fno-asynchronous-unwind-tables`, `-fno-unwind-tables`, `-fno-rtti`,
  `-fwrapv`, `-fno-strict-overflow`, `-ffp-contract=off`,
  `-freciprocal-math`, `-fno-math-errno`, `-fno-delete-null-pointer-checks`,
  `-fno-associative-math`, `-fno-finite-math-only`, `-fno-signed-zeros`,
  `-faligned-allocation`, `-fno-sized-deallocation`, `-fvisibility=hidden`,
  `-fno-inline`, `-fno-inline-functions`, `-finline-functions`,
  `-finline-hint-functions`.

The 63 supplemental flags covered the remaining scheduler/sink knobs:
`-misched-{bottomup,topdown,cluster,cyclicpath,regpressure,fusion}`,
`-sink-common-insts`, `-simplifycfg-sink-common`,
`-consthoist-with-block-frequency`, `-dfa-sched-reg-pressure-threshold`,
`-max-uses-for-sinking`, `-sink-freq-percent-threshold`,
`-dag-maps-reduction-size`, `-sched-avg-ipc`, `-sched-high-latency-cycles`,
`-pipeliner-max-stages`, the `-addr-sink-*` switches, and
`-pre-RA-sched=arm-cp-islands`.

## 4. Results: the two landed cells

### 4.1 `LevelScript.cpp -mllvm -machine-sink-split-probability-threshold=0`

`LevelScript::process` keeps its 57,232 B size and its instruction-index
alignment; six same-address bytes move into agreement. The threshold is
robust: every value `0,1,2,3,4,5,8,10,15,20,25,30` produces exactly the same
object (the pass only speculatively splits single-instruction critical edges
above the threshold, and no candidate edge in this function is in that band
for small values). `0` was chosen because it states the intent (disable the
speculative split) and is the least arbitrary.

| | raw | aligned | size | `.text` diff |
|---|---:|---:|---:|---:|
| before | 54,247 | 17,532 | 57,232 | 83,147 |
| after | **54,241** | 17,532 | 57,232 | **83,141** |

### 4.2 `Radar.cpp -mllvm -disable-postra-machine-licm`

`Radar::draw` keeps its 9,908 B size. Four text bytes move into agreement and
two move out, for a net -2 raw. The instruction-index alignment moves +9
(1,446 -> 1,455); the changed bytes are a PostRA-rematerialisation/scheduling
difference in the campaign/sl block map around function offset `0x1cb9`, not
a size or CFG change. Raw same-address bytes are the byte-identicality
ground truth, so the cell is landed; the aligned number is reported honestly.

| | raw | aligned | size | `.text` diff |
|---|---:|---:|---:|---:|
| before | 7,549 | 1,446 | 9,908 | 83,141 |
| after | **7,547** | 1,455 | 9,908 | **83,139** |

## 5. All link-measured strict candidates

Baseline `.text` 83,147. `raw`/`aligned` are the target TU's residual
function(s); `textdiff` is the whole `.text` differing-byte count for that
cell alone.

| TU | flag | raw | aligned | textdiff | verdict |
|---|---|---|---:|---:|---:|---|
| Layout | `-mllvm -tail-dup-placement=false` | 11,376 -> 11,478 | 4,216 -> 4,252 | 83,198 | worse |
| Layout | `-fno-unwind-tables` | 11,376 -> 11,376 | unchanged | 88,236 | unwind tables shrink, `.rodata` shifts |
| LevelScript | `-mllvm -enable-tail-merge` | 54,247 -> 54,329 | 17,532 -> 17,530 | 83,229 | worse |
| LevelScript | `-mllvm -disable-ifcvt-diamond` | 54,247 -> 54,322 | 17,532 -> 17,533 | 83,222 | worse |
| LevelScript | `-mllvm -disable-ifcvt-triangle-rev` | 54,247 -> 54,247 | 17,532 -> 17,534 | 83,147 | neutral |
| LevelScript | `-mllvm -disable-constant-hoisting` | 54,247 -> 54,258 | 17,532 -> 17,534 | 83,158 | worse |
| **LevelScript** | **`-mllvm -machine-sink-split-probability-threshold=0`** | **54,247 -> 54,241** | **unchanged** | **83,141** | **landed (-6)** |
| **Radar** | **`-mllvm -disable-postra-machine-licm`** | **7,549 -> 7,547** | **1,446 -> 1,455** | **83,145** | **landed (-2)** |
| Radar | `-mllvm -enable-load-pre=false` | 7,549 -> 8,567 | 1,446 -> 1,629 | 84,165 | worse |
| Radar | `-fwrapv` | 7,549 -> 7,549 | unchanged | 83,147 | neutral |
| Radar | `-fno-strict-overflow` | 7,549 -> 7,549 | unchanged | 83,147 | neutral |
| MGame | `-mllvm -disable-postra-machine-sink=true` | 4,102 -> 4,127 | 1,028 -> 1,039 | 83,172 | worse |
| MGame | `-mllvm -tail-dup-placement-penalty=0` | 4,102 -> 4,102 | 1,028 -> 1,036 | 83,147 | neutral |
| Gun | `-mllvm -disable-ifcvt-triangle-rev` | 463 -> 467 | 57 -> 61 | 83,151 | worse |
| HangarWindow | `-mllvm -disable-postra-machine-licm` | 2,245 -> 2,263 | 620 -> 623 | 83,165 | worse |
| HangarWindow | `-mllvm -machine-sink-split-probability-threshold=100` | 2,245 -> 2,262 | 620 -> 624 | 83,164 | worse |
| SpaceLounge | `-mllvm -disable-constant-hoisting` | 175 -> 218 | 41 -> 58 | 83,190 | worse |
| Hud | `-mllvm -machine-sink-split-probability-threshold=50` | 762 -> 776 | 222 -> 225 | 83,161 | worse |
| Hud | `-mllvm -machine-sink-split-probability-threshold=100` | 762 -> 776 | 222 -> 225 | 83,161 | worse |
| Generator | `-mllvm -disable-lftr` | 127 -> 140 | 107 -> 220 | 83,160 | worse |
| Generator | `-mllvm -disable-postra-machine-sink=true` | 127 -> 139 | 107 -> 111 | 83,159 | worse |

## 6. Relaxed candidates and combinations

56 further cells changed the target but also a non-target function in the
same TU while keeping every size and the TU `.text` size. All were
link-measured; **none reduced the whole-`.text` diff.** The closest:

| TU | flag | target raw | textdiff | note |
|---|---|---|---:|---:|---|
| Layout | `-mllvm -tail-merge-size={0,1,2}` | 11,376 -> 11,372 | 83,149 | target -4, non-target +6 |
| LevelScript | `-mllvm -disable-lftr` | 54,247 -> 54,248 | 83,150 | aligned 17,532 -> 17,462 |
| Radar | `-fno-honor-nans` | 7,549 -> 7,549 | 83,148 | aligned +2 |
| LevelScript | `-mllvm -break-anti-dependencies=critical` | 54,247 -> 54,245 | 83,159 | non-target +12 |

The 28 combination cells (LevelScript threshold 0 x each strict mover;
Radar `-disable-postra-machine-licm` x each strict mover and fine threshold
values) produced no improvement beyond the two landed cells. In particular
`-disable-postra-machine-licm` combined with `-disable-ifcvt-triangle-rev`,
`-disable-constant-hoisting`, `-tail-merge-size`, `-fwrapv`,
`-fno-strict-overflow`, `-disable-machine-dce` or `-disable-lftr` is exactly
the same Radar object (-2 raw, +9 aligned).

## 7. Supplemental scheduling/placement sweep

63 flags x 14 TUs = 882 cells; **zero** strict candidates. The remaining
scheduler/sink knobs (`-misched-bottomup/-topdown/-cluster/-cyclicpath/
-regpressure/-fusion`, `-sink-common-insts`, `-simplifycfg-sink-common`,
`-consthoist-with-block-frequency`, `-dfa-sched-reg-pressure-threshold`,
`-max-uses-for-sinking`, `-sink-freq-percent-threshold`,
`-dag-maps-reduction-size`, `-sched-avg-ipc`,
`-sched-high-latency-cycles`, `-pipeliner-max-stages`, `-addr-sink-*`,
`-pre-RA-sched=arm-cp-islands`) are either exact no-ops on all 14 TUs or
change a size / a non-target function.

## 8. Interpretation: did the original use per-file flags?

No positive evidence. A different original per-file flag would have to make a
residual function byte-exact; none of the 5,712 cells does. The two landed
cells are matching knobs: they reduce the same-address diff by 6 and 2 bytes
in 57 KB / 10 KB bodies while leaving every gate count, every function size,
`.ARM.extab` (659) and `.rel.plt` (0) untouched. This is consistent with
round 5: the residual is source shape / TU shape, not a compiler-flag
difference. The one genuinely flag-shaped residual class (LevelScript's
PostRA sink ordering) was already captured by round 5's
`-disable-postra-machine-sink`; the new threshold knob is an independent
MachineSink (pre-RA) ordering elsewhere in the same 57 KB function.

## 9. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11118
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so verify check -j2

# object-level filter (all 14 TUs x 343 flags)
python3 /dev/shm/w11118/sweep.py 6
# supplemental scheduler/placement flags
python3 /dev/shm/w11118/sweep_supp.py 6
# LevelScript/Radar combinations
python3 /dev/shm/w11118/combos.py 6
# link measurement of the survivors
python3 /dev/shm/w11118/link_sweep.py /dev/shm/w11118/candidates6.json
```

Scratch artifacts: `/dev/shm/w11118/{sweep.py,sweep_supp.py,combos.py,
link_sweep.py,wrap.sh,cells*.json,link6*.json}`.

## 10. Files changed

* `_work/reconstructions/build-provenance-round6-per-tu-flags.md` — this note.
* `CMakeLists.txt` — per-file
  `-mllvm -machine-sink-split-probability-threshold=0` on `LevelScript.cpp`
  (additive to the round-5 `-disable-postra-machine-sink`) and
  `-mllvm -disable-postra-machine-licm` on `Radar.cpp`.
* `docs/VALIDATION.md`, `docs/IDENTICALITY.md` — `.text` 83,177 -> 83,139,
  LevelScript raw 54,247 -> 54,241, Radar raw 7,578 -> 7,547.
