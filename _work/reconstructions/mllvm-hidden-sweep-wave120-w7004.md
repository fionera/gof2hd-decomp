# Hidden `-mllvm` sweep, wave 120 (w7004)

Status: research note, 2026-11-06. Worktree `/home/debian/GalaxyOnFire2/wt/w7004`,
branch `wave120-w7004`, base `ef7fb9cde`. Scratch is `/dev/shm/w7004`.
**No `src/`, build-flag, baseline or allowlist change is landed; this note is the only commit.**

## 0. Bottom line

* Baseline is **byte_exact 4505/4524, linked_exact 4505/4524, size-exact 4524/4524,
  data 843/843, imports 207, lint CLEAN, drift 0, RATCHET PASS**.
* A fresh `clang++ -mllvm -help-hidden` in NDK r18b clang 7.0.2 prints **1,035 lines /
  823 options** (505 plain bool, 288 value-taking, 30 enums). The fresh dump is byte-identical
  to the archived `mllvm_help_hidden_clang702_r18b.txt` (`cmp` clean).
* **150 distinct hidden option payloads** (139 value-taking, 10 enum, 1 bool control)
  were screened over the **14 TUs owning the 19 non-exact functions**. 22 distinct options change at least one non-exact function in the
  object screen; the other 128 are code-neutral on those TUs.
* Every one of the 22 was full-linked with the real gold/strip/`reorder_shstrtab` recipe and
  scored with the official verifier. **No option improves `byte_exact` or `linked_exact`;
  no option creates a new exact function.** The best changed-target gate is
  `-mllvm -break-anti-dependencies=critical` at **4354 / 4419 / 4521**; most target-changing
  knobs collapse to 1.3k–4.4k byte-exact.
* **No option improves any of the 19 per-function fuzzy percentages.** Every changed target
  cell is lower; the closest near-misses are `break-anti-dependencies=critical`
  (`LevelScript::process` 60.0→59.9, `HangarWindow::render` 89.7→89.6,
  `Hud::draw` 96.0→95.9, `MenuTouchWindow::OnTouchBegin` 98.1→97.8) and
  `alias-set-saturation-threshold=1` (LevelScript 60.0→59.8, MGame::OnTouchEnd 81.6→81.4).
* A focused follow-up value sweep (79 variants) over the target-changing knobs and the
  branch-weight controls found **no positive pct and no gate improvement**. Several values
  are exact no-ops: `break-anti-dependencies=none`, `alias-set-saturation-threshold=100`,
  `instcombine-maxarray-size>=16`, `scalar-evolution-max-arith-depth>=4`,
  `scev-mulops-inline-threshold>=2`, `static-likely-prob=80`.
* **Verdict:** no hidden `-mllvm` value option tested here explains the residual. The
  remaining 19 functions are still source/TU-shape driven, not a missing compiler flag.

## 1. Baseline and target TUs

```text
verify: byte 4505  linked 4505  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint CLEAN ✓   drift 0
```

The 19 non-exact functions are owned by 14 TUs:

| TU | non-exact functions |
|---|---|
| `src/game/ui/Layout.cpp` | `Layout::Layout` C1/C2 |
| `src/game/world/LevelScript.cpp` | `LevelScript::process` |
| `src/game/world/Level.cpp` | `Level::createShip`, `Level::createScene`, `Level::createMission` |
| `src/game/weapons/Radar.cpp` | `Radar::draw` |
| `src/game/menu/MGame.cpp` | `MGame::OnTouchEnd`, `MGame::OnUpdate` |
| `src/game/mission/Generator.cpp` | `Generator::createMission` |
| `src/game/ui/HangarWindow.cpp` | `HangarWindow::render` |
| `src/game/mission/Status.cpp` | `Status::missionCompleted` |
| `src/game/weapons/Gun.cpp` | `Gun::calcCharacterCollision` |
| `src/game/ship/PlayerEgo.cpp` | `PlayerEgo::calcCollision` |
| `src/game/menu/SpaceLounge.cpp` | `SpaceLounge::drawLounge` |
| `src/game/ui/Hud.cpp` | `Hud::draw` |
| `src/game/ui/MenuTouchWindow.cpp` | `MenuTouchWindow::OnTouchBegin` |
| `src/game/menu/ModStation.cpp` | `ModStation::ModStation` C1/C2 |

## 2. Method

### 2.1 Fresh option extraction

```sh
CXX=.cache/ndk/android-ndk-r18b/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++
printf 'int f(int x){return x+1;}\n' > /dev/shm/w7004/help/t.cpp
$CXX -target armv7-none-linux-androideabi16 -mthumb -Oz \
     -c /dev/shm/w7004/help/t.cpp -o /dev/shm/w7004/help/t.o \
     -mllvm -help-hidden > /dev/shm/w7004/help/help-hidden-fresh.txt 2>&1
```

Parsed: **823 options** = 505 plain bool + 288 value-taking + 30 enums. The archived dump
`_work/reconstructions/mllvm_help_hidden_clang702_r18b.txt` is identical.

### 2.2 Candidate selection

Wave 106 (`mllvm_residual_screen-wave106-w5424.md`) already exhaustively screened all 643
bool payloads and a targeted value set. This sweep therefore concentrates on **value-taking
options that the earlier notes did not cover or covered only at one value**, plus a few
controls. The 150 candidates cover:

* ARM/Thumb codegen, constant islands, literal pools, jump tables, `t2-reduce-limit*`,
  `arm-constant-island-*`, `arm-promote-constant-*`, `align-all-*`;
* block placement, tail duplication/merging, jump threading, switch peeling,
  `block-placement-exit-block-bias`, `tail-dup-*`, `tail-merge-*`, `triangle-chain-count`;
* scheduling, machine sinking, anti-dependencies, register allocation/spill/remat,
  `misched-limit`, `max-sched-reorder`, `dfa-*`, `regalloc-csr-*`, `stress-regalloc`,
  `spp-rematerialization-threshold`, `lcr-*`, `ssc-dce-limit`;
* inlining, vectorization, SLP, unrolling, loop rotation/unswitching,
  `inline-*`, `unroll-*`, `slp-*`, `force-vector-*`, `vectorize-*`;
* IR/alias/global/GVN/ICP/loop analysis, `alias-set-saturation-threshold`,
  `instcombine-maxarray-size`, `float2int-max-integer-bw`, `gvn-*`, `icp-*`,
  `memdep-block-*`, `memssa-check-limit`, `scalar-evolution-*`, `scev-*`, `use-cfl-aa*`;
* branch weights/profile/cold-block heuristics, `likely-branch-weight`,
  `unlikely-branch-weight`, `static-likely-prob`, `min-predictable-branch`,
  `cold-*`, `hot-*`, `profile-likely-prob`, `guards-predicate-pass-branch-weight`.

The complete 150-option list is in Appendix A. Previously tested bool/enum options were
excluded from the main 150 except where they serve as controls; the wave-106 note is the
justification for not re-running all 505 bools.

### 2.3 Object screen

```sh
TMPDIR=/dev/shm/w7004/tmp python3 tools/flag_fingerprint.py \
  --mode llvm-screen --workdir /dev/shm/w7004/screen \
  --baseline-build cmake-build-match \
  --llvm-candidates /dev/shm/w7004/candidates-150.txt \
  --jobs 32 --json /dev/shm/w7004/screen/results-150.json
```

This compiles only the 14 target TUs with each candidate appended to the canonical command
and compares every defined `STT_FUNC` object blob against the canonical `-Oz` object. A
target function whose object bytes do not change cannot change in the linked `.so` (no LTO,
`-ffunction-sections`; cross-function references are compared by symbol name). The screen
also records changes to exact functions in the same TUs as collateral.

### 2.4 Full link and scoring

Every target-changing option was rebuilt with the real `link_so` recipe:

```sh
TMPDIR=/dev/shm/w7004/tmp python3 tools/flag_fingerprint.py \
  --mode variants --workdir /dev/shm/w7004/full \
  --baseline-build cmake-build-match \
  --variants-file /dev/shm/w7004/variants-target-changing.json \
  --jobs 4 --json /dev/shm/w7004/full/results-target-changing.json
```

`flag_fingerprint` runs the official `tools/verify/verify.py` for gate counts. Its fast-fuzzy
path only short-circuits the `difflib` percentage for non-identical streams; `byte_exact`,
`linked_exact`, `size_exact`, `missing`, `extra` and `wrong_type` are the official values.
Per-function percentages for the 19 targets were then computed with the official verifier's
own `disassemble()` / `reconcile()` / `score()` (`autojunk=False`) against the original, with
the same `Resolver` setup as `verify.py`. The baseline percentages in the tables are the
canonical `cmake-build-match/verify/report.json` values.

For `likely-branch-weight` / `unlikely-branch-weight`, the full link used
`-DGOF2_NO_PER_FILE_WEIGHTS=ON` because StarMap.cpp and SpaceLounge.cpp carry per-file
7:1 overrides and clang 7 rejects a repeated `-likely-branch-weight`. The no-per-file
baseline is **2404 / 4501 / 4523**, so those two rows are not directly comparable to the
canonical 4505 baseline; they are included as branch-weight controls.

## 3. Results

### 3.1 First-pass 150-option screen: every option that changed a non-exact function

22 distinct options changed at least one target. All full-link gate counts are below the
4505 baseline; `new exact` is empty for every row. Δ is official fuzzy percentage points.

| option | byte | linked | size | all fns changed | changed target functions (Δ official pct) |
|---|---:|---:|---:|---:|---|
| `-mllvm -optsize-jump-table-density=3` | 1383 | 4465 | 4480/4524 | 3141 | LevelScript::process -18.9; MenuTouchWindow::OnTouchBegin -27.6; Level::createMission -15.4; MGame::OnTouchEnd -5.4; MGame::OnUpdate -7.8; Status::missionCompleted -42.4; Generator::createMission -11.8 |
| `-mllvm -min-jump-table-entries=3` | 1367 | 4461 | 4490/4524 | 3157 | LevelScript::process -17.7; MGame::OnTouchEnd -0.3; MGame::OnUpdate -7.9 |
| `-mllvm -jump-threading-threshold=1` | 1395 | 4458 | 4474/4524 | 3129 | HangarWindow::render -5.5; Gun::calcCharacterCollision -41.3; Level::createMission -3.6; MGame::OnTouchEnd -2.0; MGame::OnUpdate -7.8; Radar::draw -2.7; Layout::Layout C1/C2 -46.2; Generator::createMission -18.7 |
| `-mllvm -tail-dup-size=3` | 1378 | 4344 | 4380/4524 | 3146 | ModStation C1/C2 -0.4; LevelScript::process -6.6; HangarWindow::render -1.1; Level::createScene -5.6; Level::createMission -12.0; MGame::OnTouchEnd -1.2; MGame::OnUpdate -1.1; Radar::draw -2.1 |
| `-mllvm -tail-merge-threshold=3` | 1358 | 4264 | 4285/4524 | 3166 | ModStation C1/C2 -2.4; LevelScript::process -1.7; SpaceLounge::drawLounge -6.3; HangarWindow::render -1.5; MenuTouchWindow::OnTouchBegin -18.2; Hud::draw -5.4; Level::createShip -6.5; Level::createScene -4.1; Level::createMission -3.3; MGame::OnTouchEnd -15.1; MGame::OnUpdate -14.7; Radar::draw -3.8; Status::missionCompleted -26.8; Generator::createMission -6.7; PlayerEgo::calcCollision -8.5 |
| `-mllvm -break-anti-dependencies=critical` | 4354 | 4419 | 4521/4524 | 157 | LevelScript::process -0.1; HangarWindow::render -0.1; MenuTouchWindow::OnTouchBegin -0.3; Hud::draw -0.1 |
| `-mllvm -available-load-scan-limit=1` | 1721 | 4468 | 4501/4524 | 2802 | LevelScript::process -4.6; HangarWindow::render -27.1; Hud::draw -0.1; MGame::OnUpdate -15.8; Radar::draw -2.3; Layout::Layout C1/C2 -15.4 |
| `-mllvm -max-speculation-depth=1` | 1388 | 4414 | 4451/4524 | 3136 | LevelScript::process -0.9; HangarWindow::render -0.1; Gun::calcCharacterCollision -11.2; Hud::draw -29.8; Level::createScene -0.6; Level::createMission -3.8; MGame::OnUpdate -11.0; Radar::draw -2.5; Layout::Layout C1/C2 -27.7 |
| `-mllvm -regalloc=pbqp` | 957 | 1717 | 2557/4524 | 3567 | ModStation C1/C2 -60.5; LevelScript::process -37.8; SpaceLounge::drawLounge -61.7; HangarWindow::render -54.2; MenuTouchWindow::OnTouchBegin -82.8; Gun::calcCharacterCollision -63.5; Hud::draw -69.8; Level::createShip -41.1; Level::createScene -52.6; Level::createMission -60.5; MGame::OnTouchEnd -57.9; MGame::OnUpdate -58.6; Radar::draw -54.0; Layout::Layout C1/C2 -23.4; Status::missionCompleted -62.9; Generator::createMission -46.5; PlayerEgo::calcCollision -61.0 |
| `-mllvm -split-spill-mode=default` | 1460 | 4329 | 4387/4524 | 3064 | ModStation C1/C2 -0.8; LevelScript::process -6.8; SpaceLounge::drawLounge -9.2; HangarWindow::render -33.3; Gun::calcCharacterCollision -16.5; Hud::draw -34.6; Level::createShip -5.4; Level::createScene -21.5; Level::createMission -23.4; MGame::OnTouchEnd -1.5; MGame::OnUpdate -19.4; Radar::draw -26.8; Status::missionCompleted -28.6; Generator::createMission -8.7; PlayerEgo::calcCollision -10.4 |
| `-mllvm -slp-recursion-max-depth=1` | 1377 | 4475 | 4493/4524 | 3147 | LevelScript::process -0.9; MGame::OnUpdate -12.3; Layout::Layout C1/C2 -9.1 |
| `-mllvm -slp-schedule-budget=1` | 1877 | 4393 | 4479/4524 | 2647 | ModStation C1/C2 -1.9; LevelScript::process -0.9; Level::createMission -1.3; MGame::OnUpdate -12.3; Layout::Layout C1/C2 -17.6 |
| `-mllvm -alias-set-saturation-threshold=1` | 1407 | 4460 | 4516/4524 | 3117 | LevelScript::process -0.2; MGame::OnTouchEnd -0.2 |
| `-mllvm -instcombine-maxarray-size=1` | 2389 | 4500 | 4522/4524 | 2131 | Level::createScene -3.3 |
| `-mllvm -phi-node-folding-threshold=1` | 1456 | 4473 | 4498/4524 | 3068 | LevelScript::process -0.9; HangarWindow::render -0.1; Gun::calcCharacterCollision -11.2; Level::createScene -0.6; Level::createMission -3.8; Layout::Layout C1/C2 -27.9 |
| `-mllvm -memdep-block-number-limit=3` | 1384 | 4387 | 4453/4524 | 3140 | LevelScript::process -2.6; MenuTouchWindow::OnTouchBegin -31.5; Gun::calcCharacterCollision -5.6; Hud::draw -4.8; Level::createMission -26.1; MGame::OnTouchEnd -0.1; MGame::OnUpdate -0.2; Radar::draw -3.2; Layout::Layout C1/C2 -32.2 |
| `-mllvm -memdep-block-scan-limit=3` | 1353 | 4100 | 4239/4524 | 3171 | LevelScript::process -12.7; HangarWindow::render -5.3; MenuTouchWindow::OnTouchBegin -31.5; Gun::calcCharacterCollision -5.6; Hud::draw -7.5; Level::createScene -4.6; Level::createMission -26.2; MGame::OnTouchEnd -10.3; MGame::OnUpdate -23.1; Radar::draw -6.2; Layout::Layout C1/C2 -32.9; Status::missionCompleted -0.2 |
| `-mllvm -scalar-evolution-max-arith-depth=1` | 1649 | 4477 | 4502/4524 | 2875 | LevelScript::process -0.9 |
| `-mllvm -scev-mulops-inline-threshold=1` | 1827 | 4492 | 4515/4524 | 2697 | LevelScript::process -0.8 |
| `-mllvm -static-likely-prob=1` | 1328 | 3343 | 4063/4524 | 3196 | ModStation C1/C2 -9.5; LevelScript::process -29.7; SpaceLounge::drawLounge -48.6; HangarWindow::render -52.8; MenuTouchWindow::OnTouchBegin -22.1; Gun::calcCharacterCollision -33.2; Hud::draw -75.6; Level::createShip -17.3; Level::createScene -32.1; Level::createMission -35.7; MGame::OnTouchEnd -32.1; MGame::OnUpdate -62.7; Radar::draw -42.8; Layout::Layout C1/C2 -32.2; Status::missionCompleted -30.6; Generator::createMission -36.7; PlayerEgo::calcCollision -49.9 |
| `-mllvm -likely-branch-weight=3` | 3052 | 4498 | 4518/4524 | 1467 | Radar::draw -1.2; Generator::createMission -32.7 |
| `-mllvm -unlikely-branch-weight=3` | 3999 | 4503 | 4522/4524 | 513 | SpaceLounge::drawLounge -0.2; Radar::draw -3.2 |

### 3.2 Focused value sweep (79 variants)

The target-changing knobs were re-run at neighboring values, plus the branch-weight and
`static-likely-prob` controls. **74 full-link rows (including the no-per-file baseline)
change at least one non-exact function; every one has max Δ ≤ 0 and min Δ < 0, and no
variant has a positive target delta.** The table
below is the complete changed-target set (gate counts from the official verifier; Δ from the
official per-function scorer). Rows with `targets changed = 0` are listed in §3.3.

| option | byte | linked | size | all fns changed | targets changed | max Δ | min Δ |
|---|---:|---:|---:|---:|---:|---:|---:|
| `(no-per-file baseline)` | 2404 | 4501 | 4523/4524 | 2116 | 1 | -0.2 | -0.2 |
| `-mllvm -alias-set-saturation-threshold=0` | 1407 | 4460 | 4516/4524 | 3117 | 2 | -0.2 | -0.2 |
| `-mllvm -alias-set-saturation-threshold=1` | 1407 | 4460 | 4516/4524 | 3117 | 2 | -0.2 | -0.2 |
| `-mllvm -available-load-scan-limit=1` | 1721 | 4468 | 4501/4524 | 2802 | 7 | -0.1 | -27.1 |
| `-mllvm -break-anti-dependencies=all` | 1502 | 3568 | 4499/4524 | 3022 | 17 | -0.4 | -6.8 |
| `-mllvm -break-anti-dependencies=critical` | 4354 | 4419 | 4521/4524 | 157 | 4 | -0.1 | -0.3 |
| `-mllvm -instcombine-maxarray-size=0` | 2389 | 4500 | 4522/4524 | 2131 | 1 | -3.3 | -3.3 |
| `-mllvm -instcombine-maxarray-size=1` | 2389 | 4500 | 4522/4524 | 2131 | 1 | -3.3 | -3.3 |
| `-mllvm -instcombine-maxarray-size=2` | 2389 | 4500 | 4522/4524 | 2131 | 1 | -3.3 | -3.3 |
| `-mllvm -instcombine-maxarray-size=3` | 2389 | 4500 | 4522/4524 | 2131 | 1 | -3.3 | -3.3 |
| `-mllvm -instcombine-maxarray-size=4` | 2389 | 4500 | 4522/4524 | 2131 | 1 | -3.3 | -3.3 |
| `-mllvm -instcombine-maxarray-size=8` | 2693 | 4501 | 4523/4524 | 1824 | 1 | -3.3 | -3.3 |
| `-mllvm -jump-threading-threshold=1` | 1395 | 4458 | 4474/4524 | 3129 | 9 | -2.0 | -46.2 |
| `-mllvm -likely-branch-weight=0` | 2735 | 4495 | 4514/4524 | 1789 | 4 | -16.1 | -44.1 |
| `-mllvm -likely-branch-weight=1` | 2190 | 4495 | 4519/4524 | 2334 | 4 | -0.6 | -31.1 |
| `-mllvm -likely-branch-weight=10` | 3154 | 4499 | 4521/4524 | 1365 | 2 | -1.2 | -1.7 |
| `-mllvm -likely-branch-weight=100` | 3999 | 4503 | 4522/4524 | 513 | 2 | -1.6 | -7.2 |
| `-mllvm -likely-branch-weight=1000` | 2404 | 4501 | 4523/4524 | 2116 | 1 | -0.2 | -0.2 |
| `-mllvm -likely-branch-weight=12` | 3475 | 4499 | 4520/4524 | 1041 | 3 | -1.7 | -7.1 |
| `-mllvm -likely-branch-weight=16` | 3475 | 4499 | 4520/4524 | 1041 | 3 | -0.2 | -3.4 |
| `-mllvm -likely-branch-weight=2` | 2404 | 4497 | 4516/4524 | 2117 | 4 | -7.1 | -32.7 |
| `-mllvm -likely-branch-weight=20` | 3475 | 4499 | 4520/4524 | 1040 | 2 | -3.4 | -7.2 |
| `-mllvm -likely-branch-weight=200` | 3999 | 4503 | 4522/4524 | 513 | 2 | -0.2 | -3.2 |
| `-mllvm -likely-branch-weight=2000` | 2404 | 4501 | 4523/4524 | 2116 | 1 | -0.2 | -0.2 |
| `-mllvm -likely-branch-weight=3` | 3052 | 4498 | 4518/4524 | 1467 | 2 | -1.2 | -32.7 |
| `-mllvm -likely-branch-weight=32` | 4018 | 4501 | 4522/4524 | 494 | 2 | -0.2 | -3.4 |
| `-mllvm -likely-branch-weight=4` | 2110 | 4499 | 4519/4524 | 2414 | 3 | -0.9 | -32.7 |
| `-mllvm -likely-branch-weight=5` | 2110 | 4499 | 4519/4524 | 2414 | 3 | -1.1 | -32.7 |
| `-mllvm -likely-branch-weight=500` | 2404 | 4501 | 4523/4524 | 2116 | 1 | -0.2 | -0.2 |
| `-mllvm -likely-branch-weight=6` | 2469 | 4500 | 4520/4524 | 2052 | 3 | -1.1 | -7.1 |
| `-mllvm -likely-branch-weight=64` | 3999 | 4503 | 4522/4524 | 513 | 2 | -1.6 | -7.2 |
| `-mllvm -likely-branch-weight=7` | 2769 | 4501 | 4521/4524 | 1749 | 2 | -1.7 | -3.4 |
| `-mllvm -likely-branch-weight=8` | 3154 | 4499 | 4520/4524 | 1365 | 3 | -1.7 | -7.1 |
| `-mllvm -max-speculation-depth=1` | 1388 | 4414 | 4451/4524 | 3136 | 10 | -0.1 | -29.8 |
| `-mllvm -memdep-block-number-limit=3` | 1384 | 4387 | 4453/4524 | 3140 | 10 | -0.1 | -32.2 |
| `-mllvm -memdep-block-scan-limit=3` | 1353 | 4100 | 4239/4524 | 3171 | 13 | -0.2 | -32.9 |
| `-mllvm -min-jump-table-entries=3` | 1367 | 4461 | 4490/4524 | 3157 | 3 | -0.3 | -17.7 |
| `-mllvm -optsize-jump-table-density=3` | 1383 | 4465 | 4480/4524 | 3141 | 7 | -5.4 | -42.4 |
| `-mllvm -phi-node-folding-threshold=1` | 1456 | 4473 | 4498/4524 | 3068 | 7 | -0.1 | -27.9 |
| `-mllvm -regalloc=pbqp` | 957 | 1717 | 2557/4524 | 3567 | 19 | -23.4 | -82.8 |
| `-mllvm -scalar-evolution-max-arith-depth=1` | 1649 | 4477 | 4502/4524 | 2875 | 1 | -0.9 | -0.9 |
| `-mllvm -scev-mulops-inline-threshold=0` | 1827 | 4492 | 4515/4524 | 2697 | 1 | -0.8 | -0.8 |
| `-mllvm -scev-mulops-inline-threshold=1` | 1827 | 4492 | 4515/4524 | 2697 | 1 | -0.8 | -0.8 |
| `-mllvm -slp-recursion-max-depth=1` | 1377 | 4475 | 4493/4524 | 3147 | 4 | -0.9 | -12.3 |
| `-mllvm -slp-schedule-budget=1` | 1877 | 4393 | 4479/4524 | 2647 | 7 | -0.9 | -17.6 |
| `-mllvm -split-spill-mode=default` | 1460 | 4329 | 4387/4524 | 3064 | 16 | -0.8 | -34.6 |
| `-mllvm -static-likely-prob=0` | 1312 | 3335 | 4064/4524 | 3212 | 19 | -9.5 | -75.6 |
| `-mllvm -static-likely-prob=1` | 1328 | 3343 | 4063/4524 | 3196 | 19 | -9.5 | -75.6 |
| `-mllvm -static-likely-prob=100` | 1427 | 4274 | 4389/4524 | 3097 | 16 | -2.2 | -49.2 |
| `-mllvm -static-likely-prob=16` | 1312 | 3352 | 4065/4524 | 3212 | 19 | -9.5 | -75.7 |
| `-mllvm -static-likely-prob=2` | 1312 | 3343 | 4063/4524 | 3212 | 19 | -9.5 | -75.6 |
| `-mllvm -static-likely-prob=32` | 1312 | 3382 | 4069/4524 | 3212 | 19 | -7.3 | -62.6 |
| `-mllvm -static-likely-prob=4` | 1312 | 3344 | 4063/4524 | 3212 | 19 | -9.5 | -75.6 |
| `-mllvm -static-likely-prob=64` | 1350 | 4077 | 4296/4524 | 3174 | 19 | -3.7 | -56.6 |
| `-mllvm -static-likely-prob=8` | 1312 | 3347 | 4064/4524 | 3212 | 19 | -9.5 | -75.6 |
| `-mllvm -static-likely-prob=90` | 1502 | 4307 | 4403/4524 | 3022 | 16 | -2.2 | -31.3 |
| `-mllvm -tail-dup-size=3` | 1378 | 4344 | 4380/4524 | 3146 | 9 | -0.4 | -12.0 |
| `-mllvm -tail-merge-threshold=3` | 1358 | 4264 | 4285/4524 | 3166 | 16 | -1.5 | -26.8 |
| `-mllvm -unlikely-branch-weight=0` | 2094 | 4502 | 4519/4524 | 2430 | 4 | -3.2 | -10.3 |
| `-mllvm -unlikely-branch-weight=1` | 2404 | 4501 | 4523/4524 | 2116 | 1 | -0.2 | -0.2 |
| `-mllvm -unlikely-branch-weight=10` | 3999 | 4503 | 4522/4524 | 513 | 2 | -0.2 | -3.2 |
| `-mllvm -unlikely-branch-weight=100` | 3475 | 4499 | 4520/4524 | 1040 | 2 | -3.4 | -7.2 |
| `-mllvm -unlikely-branch-weight=12` | 3999 | 4503 | 4522/4524 | 513 | 2 | -3.2 | -7.2 |
| `-mllvm -unlikely-branch-weight=16` | 2404 | 4501 | 4523/4524 | 2116 | 2 | -0.1 | -7.2 |
| `-mllvm -unlikely-branch-weight=2` | 2404 | 4501 | 4523/4524 | 2116 | 1 | -0.2 | -0.2 |
| `-mllvm -unlikely-branch-weight=20` | 3999 | 4503 | 4522/4524 | 513 | 2 | -1.6 | -7.2 |
| `-mllvm -unlikely-branch-weight=3` | 3999 | 4503 | 4522/4524 | 513 | 2 | -0.2 | -3.2 |
| `-mllvm -unlikely-branch-weight=32` | 4020 | 4503 | 4522/4524 | 492 | 2 | -3.4 | -7.2 |
| `-mllvm -unlikely-branch-weight=4` | 2404 | 4501 | 4523/4524 | 2116 | 1 | -0.2 | -0.2 |
| `-mllvm -unlikely-branch-weight=5` | 2404 | 4501 | 4523/4524 | 2116 | 1 | -0.2 | -0.2 |
| `-mllvm -unlikely-branch-weight=6` | 2404 | 4501 | 4523/4524 | 2116 | 1 | -0.2 | -0.2 |
| `-mllvm -unlikely-branch-weight=64` | 3997 | 4501 | 4522/4524 | 515 | 2 | -0.2 | -1.6 |
| `-mllvm -unlikely-branch-weight=7` | 2404 | 4501 | 4523/4524 | 2116 | 1 | -0.2 | -0.2 |
| `-mllvm -unlikely-branch-weight=8` | 3999 | 4503 | 4522/4524 | 513 | 2 | -0.2 | -3.2 |

### 3.3 Exact no-ops / unobservable values

These full-link variants change no target percentage. Some are exact gate-neutral no-ops
(4505 / 4505 / 4524); others change exact functions but not the 19. They are neutral on this
tree, not improvements:

| option | byte | linked | size |
|---|---:|---:|---:|
| `-mllvm -alias-set-saturation-threshold=100` | 4505 | 4505 | 4524/4524 |
| `-mllvm -alias-set-saturation-threshold=16` | 4452 | 4504 | 4523/4524 |
| `-mllvm -alias-set-saturation-threshold=2` | 2273 | 4481 | 4519/4524 |
| `-mllvm -alias-set-saturation-threshold=32` | 4452 | 4504 | 4523/4524 |
| `-mllvm -alias-set-saturation-threshold=4` | 2271 | 4494 | 4521/4524 |
| `-mllvm -alias-set-saturation-threshold=64` | 4452 | 4504 | 4523/4524 |
| `-mllvm -alias-set-saturation-threshold=8` | 4452 | 4504 | 4523/4524 |
| `-mllvm -break-anti-dependencies=none` | 4505 | 4505 | 4524/4524 |
| `-mllvm -instcombine-maxarray-size=100` | 4505 | 4505 | 4524/4524 |
| `-mllvm -instcombine-maxarray-size=16` | 4505 | 4505 | 4524/4524 |
| `-mllvm -instcombine-maxarray-size=32` | 4505 | 4505 | 4524/4524 |
| `-mllvm -instcombine-maxarray-size=64` | 4505 | 4505 | 4524/4524 |
| `-mllvm -scalar-evolution-max-arith-depth=16` | 4505 | 4505 | 4524/4524 |
| `-mllvm -scalar-evolution-max-arith-depth=2` | 1830 | 4493 | 4517/4524 |
| `-mllvm -scalar-evolution-max-arith-depth=3` | 1830 | 4498 | 4522/4524 |
| `-mllvm -scalar-evolution-max-arith-depth=32` | 4505 | 4505 | 4524/4524 |
| `-mllvm -scalar-evolution-max-arith-depth=4` | 4505 | 4505 | 4524/4524 |
| `-mllvm -scalar-evolution-max-arith-depth=64` | 4505 | 4505 | 4524/4524 |
| `-mllvm -scalar-evolution-max-arith-depth=8` | 4505 | 4505 | 4524/4524 |
| `-mllvm -scev-mulops-inline-threshold=16` | 4505 | 4505 | 4524/4524 |
| `-mllvm -scev-mulops-inline-threshold=2` | 4505 | 4505 | 4524/4524 |
| `-mllvm -scev-mulops-inline-threshold=3` | 4505 | 4505 | 4524/4524 |
| `-mllvm -scev-mulops-inline-threshold=32` | 4505 | 4505 | 4524/4524 |
| `-mllvm -scev-mulops-inline-threshold=4` | 4505 | 4505 | 4524/4524 |
| `-mllvm -scev-mulops-inline-threshold=64` | 4505 | 4505 | 4524/4524 |
| `-mllvm -scev-mulops-inline-threshold=8` | 4505 | 4505 | 4524/4524 |
| `-mllvm -static-likely-prob=80` | 4505 | 4505 | 4524/4524 |

## 4. Build failures, crashes, and harness caveats

| candidate | result |
|---|---|
| `-mllvm -arm-constant-island-max-iteration=0` | **compile failure**: `fatal error: error in backend: Constant Island pass failed to converge!` (MGame.cpp; 0/14 TUs built) |
| `-mllvm -stress-regalloc=1` | **compile failure**: `fatal error: error in backend: ran out of registers during register allocation` (MGame.cpp; 0/14) |
| `-mllvm -pre-RA-sched=vliw-td` | **clang crash**: SIGSEGV in ARM Instruction Selection on `MGame::MGame` (0/14) |
| `-mllvm -scalar-evolution-max-arith-depth=0` | **clang crash**: SIGSEGV in Loop Strength Reduction on `LevelScript::process` and `Level::createSpace` (full link fails) |
| `-mllvm -slp-min-tree-size=1` | **compile-time blow-up**: 9/14 object-screen TUs hit the 20 s timeout; MGame.cpp still exceeds 120 s. Not fully characterized. |
| `-mllvm -likely-branch-weight=3`, `-mllvm -unlikely-branch-weight=3` | duplicate-option error on TUs with per-file weights (`may only occur zero or one times!`); 13/14 object-screen TUs built. Full link works with `GOF2_NO_PER_FILE_WEIGHTS=ON`. |
| `-mllvm -alias-set-saturation-threshold=8`, `-mllvm -likely-branch-weight=1` | first parallel full-build attempt hit an OpenSSL `sha256-armv4.S` “unterminated #if” race in the shared `_obj_gof2` directory; both reran clean sequentially and are included in §3.2. |

No other candidate crashed. The `-mllvm -enable-machine-outliner=true` crash already recorded
in `mllvm_flag_audit.md` was not re-tested.

## 5. Proven, disproven, unobservable

### Proven

1. Baseline is 4505 / 4505 / 4524, data 843/843, imports 207, lint CLEAN, drift 0.
2. `-mllvm -help-hidden` exposes 823 options (505 bool, 288 value, 30 enum); the fresh dump
   is identical to the archived one.
3. 150 distinct hidden option payloads (139 value, 10 enum, 1 bool) were screened over the
   14 target TUs. 22 change a non-exact function; 128 are code-neutral on the target TUs.
4. All 22 target-changing options were full-linked and official-verified. None improves
   `byte_exact` or `linked_exact`; none creates a new exact function; the best changed-target
   gate is 4354 / 4419 / 4521 (`break-anti-dependencies=critical`).
5. No tested option improves any of the 19 per-function fuzzy percentages. Every changed
   target cell is lower; the largest single improvements are still negative.
6. A 79-variant focused value sweep confirms the same: no positive target delta, no gate
   improvement. Several values are exact no-ops (`break-anti-dependencies=none`,
   `alias-set-saturation-threshold=100`, `instcombine-maxarray-size>=16`,
   `scalar-evolution-max-arith-depth>=4`, `scev-mulops-inline-threshold>=2`,
   `static-likely-prob=80`).

### Disproven

1. A hidden value-taking `-mllvm` option is not the explanation for any of the 19 residual
   functions at this tree state.
2. No tested option can improve a target percentage without also changing exact functions
   in its TU; every target-changing option lowers the gate.
3. `break-anti-dependencies=critical` is not the original setting (4354/4419/4521).
   `regalloc=pbqp`, `split-spill-mode=default`, `static-likely-prob=1`, the `memdep-*`
   limits, `tail-merge-threshold=3`, `tail-dup-size=3`, `jump-threading-threshold=1`,
   `max-speculation-depth=1`, `available-load-scan-limit=1`, `slp-*`, `phi-node-folding-*`
   and the branch-weight values are all worse.
4. `-mllvm -break-anti-dependencies=none`, `-mllvm -alias-set-saturation-threshold=100`,
   `-mllvm -instcombine-maxarray-size>=16`, `-mllvm -scalar-evolution-max-arith-depth>=4`,
   `-mllvm -scev-mulops-inline-threshold>=2` and `-mllvm -static-likely-prob=80` are
   code-neutral no-ops, not evidence of an original flag.

### Unobservable / still hypothesis

1. A flag that is code-neutral on these 14 TUs cannot be excluded as *used but neutral*.
   There is no positive evidence for or against it.
2. Options that only change exact functions (not the 19) were not pursued; they cannot
   improve the residual directly.
3. `-mllvm -slp-min-tree-size=1` is a compile-time blow-up, not a codegen result; it was
   not full-linked.
4. The branch-weight rows use the no-per-file baseline (2404/4501/4523), so they do not
   prove a canonical-build improvement even if a target cell had moved up (none did).
5. The exact original build flag list beyond the observable set remains unrecoverable;
   this pass only rules out the tested value-option space.

## 6. Reproduction

```sh
# baseline
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6

# 150-option object screen
TMPDIR=/dev/shm/w7004/tmp python3 tools/flag_fingerprint.py \
  --mode llvm-screen --workdir /dev/shm/w7004/screen \
  --baseline-build cmake-build-match \
  --llvm-candidates /dev/shm/w7004/candidates-150.txt \
  --jobs 32 --json /dev/shm/w7004/screen/results-150.json

# full link of every target-changing option
TMPDIR=/dev/shm/w7004/tmp python3 tools/flag_fingerprint.py \
  --mode variants --workdir /dev/shm/w7004/full \
  --baseline-build cmake-build-match \
  --variants-file /dev/shm/w7004/variants-target-changing.json \
  --jobs 4 --json /dev/shm/w7004/full/results-target-changing.json

# focused value sweep
TMPDIR=/dev/shm/w7004/tmp python3 tools/flag_fingerprint.py \
  --mode variants --workdir /dev/shm/w7004/focused \
  --baseline-build cmake-build-match \
  --variants-file /dev/shm/w7004/variants-focused.json \
  --jobs 6 --json /dev/shm/w7004/focused/results-focused.json
```

Scratch artifacts (not tracked): `/dev/shm/w7004/candidates-150.txt`,
`/dev/shm/w7004/variants-target-changing.json`, `/dev/shm/w7004/variants-focused.json`,
`/dev/shm/w7004/screen/results-150.json`, `/dev/shm/w7004/full/target-pct.json`,
`/dev/shm/w7004/focused/target-pct.json`, `/dev/shm/w7004/rerun/target-pct.json`.

## Appendix A: 150-option candidate list

- `-mllvm -arm-constant-island-max-iteration=0`
- `-mllvm -arm-promote-constant-max-size=3`
- `-mllvm -arm-promote-constant-max-total=3`
- `-mllvm -t2-reduce-limit=3`
- `-mllvm -t2-reduce-limit2=3`
- `-mllvm -t2-reduce-limit3=3`
- `-mllvm -jump-table-density=3`
- `-mllvm -optsize-jump-table-density=3`
- `-mllvm -max-jump-table-size=3`
- `-mllvm -min-jump-table-entries=3`
- `-mllvm -switch-peel-threshold=1`
- `-mllvm -jump-threading-threshold=1`
- `-mllvm -jump-threading-implication-search-threshold=1`
- `-mllvm -align-all-blocks=1`
- `-mllvm -align-all-nofallthru-blocks=1`
- `-mllvm -align-all-functions=1`
- `-mllvm -block-placement-exit-block-bias=3`
- `-mllvm -loop-to-cold-block-ratio=3`
- `-mllvm -misfetch-cost=100`
- `-mllvm -jump-inst-cost=1`
- `-mllvm -force-target-instruction-cost=1`
- `-mllvm -tail-dup-placement-threshold=3`
- `-mllvm -tail-dup-placement-aggressive-threshold=3`
- `-mllvm -tail-dup-placement-penalty=1`
- `-mllvm -tail-dup-indirect-size=3`
- `-mllvm -tail-dup-limit=3`
- `-mllvm -tail-dup-size=3`
- `-mllvm -tail-merge-size=3`
- `-mllvm -tail-merge-threshold=3`
- `-mllvm -triangle-chain-count=1`
- `-mllvm -misched-limit=3`
- `-mllvm -max-sched-reorder=3`
- `-mllvm -dfa-sched-reg-pressure-threshold=3`
- `-mllvm -dfa-instr-limit=3`
- `-mllvm -sched-high-latency-cycles=3`
- `-mllvm -sched-avg-ipc=3`
- `-mllvm -max-uses-for-sinking=1`
- `-mllvm -sink-freq-percent-threshold=1`
- `-mllvm -break-anti-dependencies=critical`
- `-mllvm -regalloc-csr-first-time-cost=3`
- `-mllvm -stress-regalloc=1`
- `-mllvm -spp-rematerialization-threshold=3`
- `-mllvm -lcr-max-depth=1`
- `-mllvm -lcr-max-interf=1`
- `-mllvm -ssc-dce-limit=1`
- `-mllvm -available-load-scan-limit=1`
- `-mllvm -dataflow-edge-limit=1`
- `-mllvm -recurrence-chain-limit=1`
- `-mllvm -machine-combiner-inc-threshold=1`
- `-mllvm -postra-sched-debugdiv=1`
- `-mllvm -postra-sched-debugmod=1`
- `-mllvm -max-speculation-depth=1`
- `-mllvm -spec-exec-max-not-hoisted=1`
- `-mllvm -spec-exec-max-speculation-cost=1`
- `-mllvm -pre-RA-sched=vliw-td`
- `-mllvm -misched=ilpmin`
- `-mllvm -regalloc=pbqp`
- `-mllvm -split-spill-mode=default`
- `-mllvm -reverse-csr-restore-seq`
- `-mllvm -disable-postra-machine-sink=false`
- `-mllvm -inline-threshold=2`
- `-mllvm -inlinehint-threshold=3`
- `-mllvm -preinline-threshold=1`
- `-mllvm -inline-cold-callsite-threshold=1`
- `-mllvm -max-num-inline-blocks=3`
- `-mllvm -partial-inlining-extra-penalty=1`
- `-mllvm -max-partial-inlining=1`
- `-mllvm -unroll-threshold=1`
- `-mllvm -unroll-count=1`
- `-mllvm -unroll-max-count=1`
- `-mllvm -unroll-full-max-count=1`
- `-mllvm -unroll-partial-threshold=1`
- `-mllvm -unroll-max-upperbound=1`
- `-mllvm -unroll-peel-count=1`
- `-mllvm -unroll-peel-max-count=1`
- `-mllvm -unroll-force-peel-count=1`
- `-mllvm -unroll-max-iteration-count-to-analyze=1`
- `-mllvm -unroll-max-percent-threshold-boost=1`
- `-mllvm -partial-unrolling-threshold=1`
- `-mllvm -pragma-unroll-threshold=1`
- `-mllvm -flat-loop-tripcount-threshold=1`
- `-mllvm -rotation-max-header-size=1`
- `-mllvm -unswitch-threshold=1`
- `-mllvm -loop-unswitch-threshold=1`
- `-mllvm -slp-min-tree-size=1`
- `-mllvm -slp-recursion-max-depth=1`
- `-mllvm -slp-schedule-budget=1`
- `-mllvm -slp-threshold=1`
- `-mllvm -force-vector-width=1`
- `-mllvm -force-vector-interleave=1`
- `-mllvm -force-target-max-vector-interleave=1`
- `-mllvm -force-target-max-scalar-interleave=1`
- `-mllvm -force-target-num-vector-regs=1`
- `-mllvm -vectorize-num-stores-pred=1`
- `-mllvm -vectorize-scev-check-threshold=1`
- `-mllvm -vectorizer-min-trip-count=1`
- `-mllvm -vector-library=none`
- `-mllvm -cost-kind=code-size`
- `-mllvm -alias-set-saturation-threshold=1`
- `-mllvm -instcombine-maxarray-size=1`
- `-mllvm -float2int-max-integer-bw=1`
- `-mllvm -gvn-hoist-max-bbs=1`
- `-mllvm -gvn-hoist-max-chain-length=1`
- `-mllvm -gvn-hoist-max-depth=1`
- `-mllvm -gvn-max-hoisted=1`
- `-mllvm -icp-cutoff=1`
- `-mllvm -icp-max-annotations=1`
- `-mllvm -icp-max-prom=1`
- `-mllvm -icp-remaining-percent-threshold=1`
- `-mllvm -icp-total-percent-threshold=1`
- `-mllvm -memssa-check-limit=1`
- `-mllvm -rewrite-phi-limit=1`
- `-mllvm -phi-node-folding-threshold=1`
- `-mllvm -dom-conditions-max-uses=1`
- `-mllvm -runtime-memory-check-threshold=1`
- `-mllvm -min-region-size-ratio=1`
- `-mllvm -outline-region-freq-percent=1`
- `-mllvm -cgp-freq-ratio-to-skip-merge=1`
- `-mllvm -global-merge-max-offset=3`
- `-mllvm -memdep-block-number-limit=3`
- `-mllvm -memdep-block-scan-limit=3`
- `-mllvm -licm-max-num-uses-traversed=1`
- `-mllvm -licm-versioning-invariant-threshold=1`
- `-mllvm -licm-versioning-max-depth-threshold=1`
- `-mllvm -loop-distribute-scev-check-threshold=1`
- `-mllvm -loop-interchange-threshold=1`
- `-mllvm -loop-load-elimination-scev-check-threshold=1`
- `-mllvm -scalar-evolution-max-add-rec-size=1`
- `-mllvm -scalar-evolution-max-arith-depth=1`
- `-mllvm -scalar-evolution-max-ext-depth=1`
- `-mllvm -scalar-evolution-max-scev-compare-depth=1`
- `-mllvm -scalar-evolution-max-value-compare-depth=1`
- `-mllvm -scev-addops-inline-threshold=1`
- `-mllvm -scev-mulops-inline-threshold=1`
- `-mllvm -use-cfl-aa=steens`
- `-mllvm -use-cfl-aa-in-codegen=steens`
- `-mllvm -replexitval=cheap`
- `-mllvm -cold-branch-ratio=1`
- `-mllvm -cold-callsite-rel-freq=1`
- `-mllvm -hot-callsite-rel-freq=1`
- `-mllvm -hot-callsite-threshold=1`
- `-mllvm -locally-hot-callsite-threshold=1`
- `-mllvm -profile-likely-prob=1`
- `-mllvm -static-likely-prob=1`
- `-mllvm -min-predictable-branch=1`
- `-mllvm -guards-predicate-pass-branch-weight=1`
- `-mllvm -likely-branch-weight=3`
- `-mllvm -unlikely-branch-weight=3`
- `-mllvm -min-block-execution=3`
- `-mllvm -callsite-splitting-duplication-threshold=3`
