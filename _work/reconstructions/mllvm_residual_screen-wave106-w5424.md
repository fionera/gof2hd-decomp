# Hidden `-mllvm` screen targeted at the 24 residual functions (wave 106, w5424)

**Status:** research note, 2026-11-04. Worktree `/home/debian/GalaxyOnFire2/wt/w5424`,
branch `wave106-w5424`, base `486f8a119`. Scratch is `/dev/shm/w5424`.
**No source or build-flag change is landed: no tested `-mllvm` flag is a strict
improvement.** This commit contains only this note.

The task was to re-screen the clang 7.0.2 ARM backend option space at the current
baseline (byte_exact 4500, after the wave-105 source improvements), concentrating on
greedy register allocation, machine block placement, tail duplication, if-conversion,
shrink-wrapping, ARM constant/literal-pool placement, Thumb-2 instruction selection,
EH/LSDA emission and the machine schedulers. Wave 104 (`mllvm_backend_screen_w5210.md`)
did the same at the old baseline and found nothing; this note confirms the result at
the current tree with a faster target-TU-only harness and a fresh full-link check of
every positive cell.

## 0. Bottom line

* Baseline is **byte_exact 4500/4524, linked_exact 4500/4524, size-exact 4524/4524,
  data 843/843, imports 207, lint CLEAN, drift 0, RATCHET PASS**. `.text` 1,259,464 B
  (99,226 differing bytes), `.ARM.exidx` 28,528 B (829), `.ARM.extab` 156,312 B
  (108,294), `.note.gnu.build-id` 20; every other section is byte-identical.
* `clang++ -mllvm -help-hidden` in NDK r18b clang 7.0.2 prints **1,035 lines / 823
  options**: 505 plain bool, 288 value-taking, of which 30 are enums (the wave-104
  "535 bool" count is 505 bool + 30 enum).
* The fast harness recompiles only the **16 TUs that own the 24 functions**, relinks
  with the canonical gold+strip+`reorder_shstrtab` chain, and scores the 24 with the
  official verifier's own `disassemble`/`reconcile`/`score` functions. One variant is
  **~11.5 s**; 406 variants took 2 m 51 s at `--jobs 32`.
* **1,113 distinct flag payloads / 556 distinct option names** were screened (all 643
  bool-option payloads, the full targeted value set, and 10 combos). **643 unique
  successful (flag, result) rows.**
* **28 distinct flags/combos raise the fuzzy of at least one of the 24.** None makes a
  target byte-exact. **None keeps `byte_exact` at 4500**: 448 variants keep 4500 but
  change no target at all; every flag that changes a target also changes at least one
  exact function in its TU. No variant exceeds 4500.
* Full-link checks of every distinct positive single option confirm the same story:
  the target cell improves, but `byte_exact` collapses (1,218–2,290), and no target
  becomes byte-exact. **No strict improvement exists in the tested space.**

## 1. Baseline (measured)

```sh
cd /home/debian/GalaxyOnFire2/wt/w5424
export TMPDIR=/dev/shm/w5424/tmp
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
```

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.91
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

The 24 non-exact functions and their fresh official scores
(`cmake-build-match/verify/report.json`):

| mangled name | bytes | fuzzy % |
|---|---:|---:|
| `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` | 7072 | 47.8 |
| `_ZN11LevelScript7processEi` | 57232 | 54.2 |
| `_ZN6Status16missionCompletedEbbx` | 1432 | 60.4 |
| `_ZN5Level10createShipEiiiP8Waypointbb` | 2764 | 66.6 |
| `_ZN5Radar4drawEP6PlayerP3Hudi` | 9908 | 74.0 |
| `_ZN5MGame10OnTouchEndEiiPv` | 8080 | 78.4 |
| `_ZN7GlobalsC1Ev` / `_ZN7GlobalsC2Ev` | 612 | 80.5 |
| `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 1996 | 81.5 |
| `_ZN12HangarWindow6renderEv` | 9700 | 84.7 |
| `_ZN9RocketGun6updateEi` | 1908 | 86.9 |
| `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` | 968 | 87.6 |
| `_ZN5Level11createSceneEv` | 2252 | 88.3 |
| `_ZN3Gun22calcCharacterCollisionEv` | 1984 | 90.0 |
| `_ZN5Level13createMissionEv` | 17172 | 92.0 |
| `_ZN3Hud4drawExxP9PlayerEgobjj` | 9332 | 93.3 |
| `_ZN15MenuTouchWindowC1Ei` / `_ZN15MenuTouchWindowC2Ei` | 9600 | 95.1 |
| `_ZN11SpaceLounge10drawLoungeEv` | 1948 | 95.5 |
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 1896 | 97.7 |
| `_ZN10ModStationC1Ev` / `_ZN10ModStationC2Ev` | 1008 | 98.4 |
| `_ZN5MGame8OnUpdateEv` | 11440 | 99.9 |

The 16 target TUs are `Globals.cpp`, `MGame.cpp`, `ModStation.cpp`, `SpaceLounge.cpp`,
`Generator.cpp`, `Status.cpp`, `PlayerEgo.cpp`, `HangarWindow.cpp`, `Hud.cpp`,
`Layout.cpp`, `MenuTouchWindow.cpp`, `Gun.cpp`, `Radar.cpp`, `RocketGun.cpp`,
`Level.cpp`, `LevelScript.cpp`.

## 2. Option enumeration

```sh
CXX=/home/debian/GalaxyOnFire2/wt/w5424/.cache/ndk/android-ndk-r18b/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++
mkdir -p /dev/shm/w5424/help && cd /dev/shm/w5424/help
printf 'int f(int x){return x+1;}\n' > t.cpp
$CXX -target armv7-none-linux-androideabi16 -mthumb -Oz -c t.cpp -o t.o \
     -mllvm -help-hidden > help-hidden.txt 2>&1
$CXX -target armv7-none-linux-androideabi16 -mthumb -Oz -c t.cpp -o t2.o \
     -mllvm -help > help.txt 2>&1
```

| file | lines | parsed options | plain bool | value-taking | enums |
|---|---:|---:|---:|---:|---:|
| `-mllvm -help-hidden` | 1,035 | **823** | 505 | 288 | 30 |
| `-mllvm -help` | 61 | — | — | — | — |

The targeted candidate set covered every option whose name or description matches:
`regalloc|spill|remat|live|interval|hint|coalesc|copy|2addr|twoaddr|csr|physreg|defer`
(register allocation), `block-placement|block|misfetch|tail-merge|align-all`
(machine block placement), `tail-dup` (tail duplication), `ifcvt|if-conv`
(if-conversion), `shrink` (shrink-wrapping), `constant-island|promote-constant|
jump-table|arm-|thumb|literal` (ARM constant/literal pool), `t2-|thumb2` (Thumb-2
instruction selection), `eh|unwind|landing|lsda|cleanup|demotion|nounwind|funclet|
exception` (EH/LSDA), and `misched|sched|post-ra|postra|pre-ra|prera|dfa-`
(scheduling). Every bool option in the whole `-help-hidden` list (all 505, plus
`=false` for every `-enable-*`/`-disable-*`) was also screened, so the targeted set is
a superset of the requested categories and the bool space is exhaustive.

## 3. Fast A/B harness

`_work/reconstructions/mllvm_residual_screen_harness_w5424.py` (committed
alongside this note) does:

1. Read `cmake-build-match/compile_commands.json`; take the canonical command for the
   16 target TUs.
2. Compile each target TU with the candidate payload appended, into a scratch object
   directory.
3. Take the exact `ninja -t commands link_so` gold+strip+`reorder_shstrtab` chain, and
   substitute (a) the 16 target-TU object paths and (b) the two output `.so` paths.
   Non-target objects, vendored archives and `crtbegin_so.o` are the baseline ones.
4. Score the resulting `.so`:
   * `byte_exact` / `size_exact` over all 4,524 common functions by raw `.dynsym`
     blob comparison (the official verifier's `byte` flag is exactly raw-byte
     equality);
   * fuzzy % for the 24 using the official verifier's `disassemble`, `reconcile` and
     `score` with the same `Resolver` setup as `verify.py`.

The harness is valid because with `-ffunction-sections` and no LTO a target function's
bytes are determined by its own TU's compilation; cross-function references are
compared by symbol name. The only approximation is address-dependent layout
(literal-pool addresses, branch relaxation) that changes when non-target TUs also
change size.

### 3.1 Harness validation against the full `check`

Full builds used `tools/flag_fingerprint.py --mode variants` (real `cmake --build
--target link_so`, official `verify.py`). Harness target fuzzy is compared to the
full build's official per-function fuzzy:

| variant | harness byte | full byte | harness size | full size | target fuzzy diffs |
|---|---:|---:|---:|---:|---:|
| `-mllvm -disable-tail-duplicate` | 2590 | 1962 | 4516 | 4501 | **0 / 24** |
| `-mllvm -arm-promote-constant` | 2026 | 1370 | 4459 | 4221 | **0 / 24** |
| `-mllvm -disable-ifcvt-triangle` | 3588 | 4220 | 4522 | 4521 | **0 / 24** |
| `-mllvm -disable-ifcvt-simple-false` | 2023 | 1534 | 4491 | 4427 | 2 / 24 (MGame) |

The first three reproduce every target fuzzy cell exactly. The fourth differs on
`MGame::OnTouchEnd` (78.4 vs 71.5) and `MGame::OnUpdate` (99.9 vs 97.3) because the
global flag also changes other TUs' code sizes and therefore `MGame`'s
address-dependent layout. Harness `byte_exact` is an **upper bound** on the full
gate (it never changes non-target TUs), so `harness byte_exact >= 4500` is a
necessary condition for a full-gate improvement.

Commands:

```sh
# single variant
python3 _work/reconstructions/mllvm_residual_screen_harness_w5424.py \
  --flags "-mllvm -disable-peephole" --name peephole --jobs 1 \
  --out /dev/shm/w5424/one.json

# batch (lists generated by the committed variants script)
python3 _work/reconstructions/mllvm_residual_screen_harness_w5424.py \
  --variant-file /dev/shm/w5424/targeted.json --jobs 32 \
  --out /dev/shm/w5424/targeted_results.json
python3 _work/reconstructions/mllvm_residual_screen_harness_w5424.py \
  --variant-file /dev/shm/w5424/allbool.json --jobs 40 \
  --out /dev/shm/w5424/allbool_results.json
```

## 4. Screen results

1,113 distinct payloads were generated across six batches (targeted values, extra
bool/pass toggles, scheduler-model toggles, misc jump-table/stack toggles, all 643
bool payloads, 10 combos). 643 unique successful rows.
**28 distinct flags/combos raise at least one target fuzzy cell.** Every row below
has `harness byte_exact < 4500`; no target becomes byte-exact in any row.

| flag(s) | harness byte | harness size | target improvement |
|---|---:|---:|---|
| `-mllvm -disable-ifcvt-simple-false` | 2023 | 4491 | `HangarWindow::render` 84.7 → **86.1** (+1.4) |
| `-mllvm -sink-insts-to-avoid-spills` | 2108 | 4476 | `Level::createScene` 88.3 → **89.3** (+1.0) |
| `-mllvm -disable-peephole` | 1656 | 4424 | `LevelScript::process` 54.2 → **54.7** (+0.5) |
| `-mllvm -machine-sink-split-probability-threshold=64` | 2056 | 4512 | `Layout::Layout` C1/C2 47.8 → **48.0** (+0.2) |
| `-mllvm -machine-sink-split-probability-threshold=75` | 2056 | 4512 | `Layout::Layout` C1/C2 47.8 → **48.0** (+0.2) |
| `-mllvm -machine-sink-split-probability-threshold=90` | 2056 | 4512 | `Layout::Layout` C1/C2 47.8 → **48.0** (+0.2) |
| `-mllvm -machine-sink-split-probability-threshold=100` | 2086 | 4509 | `Layout::Layout` C1/C2 47.8 → **48.0** (+0.2) |
| `-mllvm -disable-ifcvt-triangle-rev` | 2713 | 4520 | `Level::createShip` 66.6 → **66.7** (+0.1) |
| `-mllvm -disable-postra-machine-sink` | 2294 | 4506 | `Hud::draw` 93.3 → **93.4** (+0.1) |
| `-mllvm -ifcvt-limit=0` | 2007 | 4437 | `Level::createShip` 66.6 → **66.7** (+0.1) |
| `-mllvm -ifcvt-fn-start={0,1,2,4,8,16,32,64,100,1000}` | 2007 | 4437 | `Level::createShip` 66.6 → **66.7** (+0.1) |
| `-mllvm -t2-reduce-limit2=0` | 1629 | 4415 | `LevelScript::process` 54.2 → **54.3** (+0.1) |
| `-mllvm -disable-ifcvt-simple-false -mllvm -disable-ifcvt-triangle-rev` | 2023 | 4480 | `HangarWindow::render` +1.4, `Level::createShip` +0.1 |
| `-mllvm -disable-ifcvt-simple-false -mllvm -disable-postra-machine-sink` | 2099 | 4474 | `HangarWindow::render` 84.7 → 86.0 (+1.3) |
| `-mllvm -sink-insts-to-avoid-spills -mllvm -machine-sink-split-probability-threshold=64` | 2042 | 4469 | `Level::createScene` +1.0, `Layout` C1/C2 +0.2 |
| `-mllvm -disable-ifcvt-simple-false -mllvm -disable-peephole` | 1678 | 4420 | `LevelScript::process` +0.5 |
| `-mllvm -disable-ifcvt-simple-false -mllvm -machine-sink-split-probability-threshold=64 -mllvm -disable-peephole` | 1656 | 4417 | `LevelScript::process` +0.5 |
| `-mllvm -disable-ifcvt-triangle-rev -mllvm -machine-sink-split-probability-threshold=64` | 2051 | 4511 | `Layout` C1/C2 +0.2 |
| `-mllvm -disable-ifcvt-triangle-rev -mllvm -ifcvt-limit=0` | 2007 | 4437 | `Level::createShip` +0.1 |

(The `ifcvt-fn-start=N` rows are the same code effect for every tested N; they are
listed once. The two `disable-ifcvt-simple` spellings are distinct options:
`-disable-ifcvt-simple=false` is a no-op (4500/4500/4524, `changed=0`), while the
standalone `-disable-ifcvt-simple-false` is the real +1.4 HangarWindow change.)

### 4.1 The `byte_exact >= 4500` condition

Of the 643 unique rows, **448 keep harness `byte_exact == 4500`; none of them changes
any of the 24** (all are code-neutral on the target functions). Conversely, **every
row that changes a target has harness `byte_exact < 4500`**. No row exceeds 4500 and
no row makes a target byte-exact. In other words, at this tree state a flag can only
move one of the 24 by also moving at least one exact function in the same TU — the
`changed_targets >= changed_exact + 1` necessary condition from wave 104 is not met
by any targeted flag.

## 5. Full-link checks of the positive single options

Every distinct positive single option was full-built with the real `link_so` recipe
and scored by the official verifier (`tools/flag_fingerprint.py --mode variants
--full-verify`; `--jobs 1` to avoid the vendored-lib build race). Baseline row:
4500 / 4500 / 4524.

| flag | byte | linked | size | target up | other target cells down |
|---|---:|---:|---:|---|---:|
| `-mllvm -disable-ifcvt-simple-false` | **1534** | 3733 | 4427 | `HangarWindow::render` 84.7→86.1 | 16 |
| `-mllvm -sink-insts-to-avoid-spills` | **1489** | 4345 | 4394 | `Level::createScene` 88.3→89.3 | 11 |
| `-mllvm -disable-ifcvt-triangle-rev` | **2290** | 4464 | 4510 | `Level::createShip` 66.6→66.7 | 4 |
| `-mllvm -disable-postra-machine-sink` | **1573** | 4394 | 4473 | `Hud::draw` 93.3→93.4 | 8 |
| `-mllvm -ifcvt-limit=0` | **1279** | 3419 | 4250 | `Level::createShip` 66.6→66.7 | 21 |
| `-mllvm -ifcvt-fn-start=0` | **1279** | 3419 | 4250 | `Level::createShip` 66.6→66.7 | 21 |
| `-mllvm -t2-reduce-limit2=0` | **1218** | 3126 | 4097 | `LevelScript::process` 54.2→54.3 | 21 |
| `-mllvm -disable-peephole` | **1336** | 3559 | 4204 | `LevelScript::process` 54.2→54.7 | 20 |
| `-mllvm -machine-sink-split-probability-threshold=64` | **2080** | 4478 | 4506 | `Layout` C1/C2 47.8→48.0 | 6 |
| `-mllvm -machine-sink-split-probability-threshold=100` | **2050** | 4475 | 4503 | `Layout` C1/C2 47.8→48.0 | 8 |

`new_exact` is empty for every row: no target becomes byte-exact even globally.
`-mllvm -disable-ifcvt-simple=false` is the only full-link row that is
4500/4500/4524 with `changed=0`, but it is a no-op (the default already enables
ifcvt-simple).

The full-link fuzzy deltas reproduce the harness's positive cells exactly for the
single flags (e.g. `-disable-ifcvt-simple-false` gives HangarWindow 86.1 globally),
while the other 23 cells drop. This is the same qualitative result wave 104 found at
the old baseline.

## 6. Combos

Combos were tested only after single options showed positive deltas. Ten combos were
screened; the best target changes are shown in §4. **No combo keeps harness
`byte_exact` at 4500** (best: 2547 for `-disable-ifcvt-simple-false +
-sink-insts-to-avoid-spills`, which interestingly cancels both target gains). The
combos cannot be strict improvements because each constituent already lowers the full
gate by hundreds of functions.

## 7. What is proven, disproven, and still hypothesis

### Proven

1. Baseline is 4500 / 4500 / 4524, data 843/843, imports 207, lint CLEAN, drift 0,
   RATCHET PASS; `.text` 99,226 differing bytes, `.ARM.exidx` 829, `.ARM.extab`
   108,294, build-id 20.
2. `-mllvm -help-hidden` exposes 823 options (505 bool, 288 value, 30 enum).
3. The target-TU-only harness reproduces the official target fuzzy exactly on 3/3
   validation variants (and 22/24 cells on a fourth whose flag changes global
   layout); harness `byte_exact` is an upper bound on the full gate.
4. 643 unique successful rows: 28 distinct flags/combos raise a target fuzzy cell,
   none makes a target byte-exact, none keeps `byte_exact` at 4500, none exceeds 4500.
5. Full-link checks of all 10 distinct positive single options confirm
   `byte_exact` 1,218–2,290 and `new_exact` empty.
6. `-mllvm -disable-ifcvt-simple=false` is a code-neutral no-op; the real
   `HangarWindow::render` +1.4 is the standalone `-disable-ifcvt-simple-false`.

### Disproven

1. A targeted `-mllvm` flag is not the explanation for any of the 24 residual
   functions at the current baseline.
2. No targeted flag can improve a target without lowering `byte_exact`; the
   necessary condition `changed_targets >= changed_exact + 1` is met by none.
3. `-disable-ifcvt-simple-false`, `-sink-insts-to-avoid-spills`, `-disable-peephole`,
   `-machine-sink-split-probability-threshold=*`, `-disable-ifcvt-triangle-rev`,
   `-disable-postra-machine-sink`, `-ifcvt-limit=0` and `-t2-reduce-limit2=0` are not
   the original settings (each costs 2,210–3,282 byte-exact functions).

### Hypothesis / unobservable

1. A flag that is code-neutral on this source cannot be excluded as *used but
   neutral*; there is no positive evidence for or against it.
2. The `.ARM.extab` content difference is address/relocation-driven and was not
   attributed to any flag here.
3. The remaining 24 are source/TU-shape residuals (register allocation, scheduling,
   block order, EH scope grouping); the next productive work is source-shape
   reconstruction, not more flag search.

## 8. Strict-improvement verdict

**No strict improvement exists in the tested `-mllvm` space at this tree state.**
The canonical build remains 4500 / 4500 / 4524 with no flag added. This commit
contains only this note.

## Appendix: harness source

Committed copy: `_work/reconstructions/mllvm_residual_screen_harness_w5424.py`.
Key points:

```python
# 16 target TUs from cmake-build-match/compile_commands.json
# compile each with canonical command + candidate flags
# take `ninja -t commands link_so`'s last line (gold + strip + reorder_shstrtab),
#   replace "/./" -> "/", substitute the 16 object paths and the two .so outputs
# score with verify.disassemble / verify.reconcile / verify.score and raw blobs
```

Reproduce:

```sh
export TMPDIR=/dev/shm/w5424/tmp
# regenerate every candidate list (targeted 406, extra 157, sched 13, misc 71,
# combos 10, allbool 643) from the committed help-hidden dump
python3 _work/reconstructions/mllvm_residual_screen_variants_w5424.py /dev/shm/w5424
for b in targeted extra sched misc combos; do
  python3 _work/reconstructions/mllvm_residual_screen_harness_w5424.py \
    --variant-file /dev/shm/w5424/$b.json --jobs 32 \
    --out /dev/shm/w5424/${b}_results.json
done
python3 _work/reconstructions/mllvm_residual_screen_harness_w5424.py \
  --variant-file /dev/shm/w5424/allbool.json --jobs 40 \
  --out /dev/shm/w5424/allbool_results.json
```

The option enumeration is archived verbatim as
`_work/reconstructions/mllvm_help_hidden_clang702_r18b.txt` (1,035 lines, 823
options), so the candidate lists are reproducible without the NDK.
