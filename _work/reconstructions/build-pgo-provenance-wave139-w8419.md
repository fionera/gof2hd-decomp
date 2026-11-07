# Build-flag / PGO provenance, wave 139 (w8419)

Status: **note-only research commit**, 2026-11-07. Worktree
`/home/debian/GalaxyOnFire2/wt/w8419`, base `bf123bd47` ("Level::createMission
wave 138: case-183 default-first switch..."). Scratch: `/dev/shm/w8419`.
No `src/`, `tools/`, `CMakeLists.txt`, baseline, allowlist or doc change is
landed. This note is the only commit.

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
| `.text` | 1,259,464 | **85,476** |
| `.ARM.extab` | 156,312 | **1,396** |
| `.note.gnu.build-id` | 36 | **20** |
| `.rodata`, `.data`, `.bss`, `.ARM.exidx`, `.rel.plt` | exact | **0** |

Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`,
2,187,328 B.

---

## 0. Bottom line

1. **No build flag raises `byte_exact` above 4507.** A ~100-cell full-gate
   matrix (this pass) plus the ~120 cells of w8119/w8319 leaves the canonical
   flag set as the unique best. Every codegen-relevant flag is negative; every
   neutral flag leaves `.text` byte-identical.
2. **The target is pinned by `.ARM.attributes`, and `-mtune` is the only free
   target knob.** The original's `.ARM.attributes` carries `Tag_CPU_name =
   "ARM v7"` (generic `-march=armv7-a`), not a `-mcpu=` name; `-mcpu=cortex-a8`
   / `-mcpu=cortex-a9` change the attribute blob. `-mtune=` is *not* encoded,
   but all of `generic/cortex-a5/a7/a8/a9/a15` are codegen-neutral at `-Oz`
   (the pre-RA scheduler is `list-hybrid`, and the MachineScheduler is off).
3. **The scheduler configuration is the clang 7 default and it matches:**
   `-pre-RA-sched=list-hybrid` (default; `source`/`list-burr`/`list-ilp` are
   regressions), `-enable-misched` **off** by default (turning it on is a
   huge regression), `-enable-post-misched` **on** (neutral), `-misched-postra`
   **off** (turning it on is negative), `-post-RA-scheduler` **on** (neutral),
   `-break-anti-dependencies=none` (neutral).
4. **`-disable-cgp-branch-opts` is the closest near-miss: 4506/4524.** It
   breaks exactly one previously-exact function,
   `AbyssEngine::Engine::SetUVMatrix(Matrix const&)` (148 B, 1 byte), so CGP
   branch opts are on in the original. No flag gets 4508+.
5. **Per-TU flags do not help the residual functions.** Per-TU
   `-fno-math-errno` is neutral on all 13 residual TUs (they make no libm
   calls); per-TU `-fno-exceptions` worsens every residual function's raw
   diff (new per-function measurement below, confirming w8319's whole-gate
   result); per-TU `-mllvm -pre-RA-sched=source`, `-enable-misched`,
   `-misched-postra`, `-disable-postra-machine-sink`, `-disable-machine-sink`
   all worsen the target. `-disable-postra-machine-sink` on `Generator.cpp`
   keeps `byte_exact 4507` but worsens `createMission` (445 -> 447 B).
6. **`-fno-integrated-as` (GNU as 4.9) is a regression (2767).** The original
   used clang's integrated assembler. This was previously untested because the
   NDK driver needs `--gcc-toolchain=` to find the ARM `as` (otherwise it
   invokes host `/bin/as` and fails on `-mfpu=neon`).
7. **`-mvectorize-with-neon-quad` is GCC-only**; clang 7.0.2 rejects it
   (`unknown argument`). `-enable-machine-outliner` still crashes clang 7 on
   `VirtualInput.cpp`; `-enable-linkonceodr-outlining` alone is neutral.

**Verdict (unchanged): the original is a plain `-Oz -g2` build with clang 7's
default scheduler, default visibility, default FP semantics, integrated
assembler, and heuristic branch weights. The 17 residual functions are source
shape, not build configuration.**

---

## 1. Method

A scratch build (`/dev/shm/w8419/build`) uses a compiler launcher that appends
`$GOF2_EXTRA_FLAGS` after the canonical flags:

```sh
#!/bin/sh
exec "$@" $GOF2_EXTRA_FLAGS
```

Each cell deletes all `CMakeFiles/gof2.dir` objects, relinks the whole tree
(`link_so -j6`, ~10 s), and scores with a fast byte-only scorer
(`/dev/shm/w8419/score.py`) that reproduces `verify.py`'s `byte_exact` exactly
(canonical 4507, 17 bad). Section diffs are computed against the original with
the same `Elf32` reader (`/dev/shm/w8419/sec.py`).

Per-TU cells use a second launcher that appends `$GOF2_TU_FLAGS` only when the
source basename equals `$GOF2_TU`, and delete **all** objects before each cell
(so no stale flagged object survives). The per-function raw diff is measured
at each symbol's own `st_value` in both files, so it is valid even when a TU
changes size; it is still link-layout sensitive for literal-pool words, which
is why the target TU's own row is the meaningful one.

The canonical scratch build reproduces the gate exactly: `byte_exact 4507`,
`.text` 85,476, `.ARM.extab` 1,396.

---

## 2. Full-gate flag matrix (new cells in this pass)

All cells are a full link of the whole tree with only the named flag appended
to every compile. Canonical is `byte_exact 4507`, `.text` diff 85,476.

### 2.1 Neutral (`byte_exact 4507`, `.text` and `.ARM.extab` byte-identical)

| flag | note |
|---|---|
| `-mtune=generic` / `cortex-a5` / `cortex-a7` / `cortex-a8` / `cortex-a9` / `cortex-a15` | tune is not in `.ARM.attributes`; neutral at `-Oz` |
| `-mno-dsp` / `-mno-restrict-it` / `-fjump-tables` | already the defaults |
| `-g2` / `-g3` | `-g` is `-g2`; debug info is stripped |
| `-fmerge-all-constants` / `-ffp-contract=fast` | `.rodata` still exact |
| `-fvisibility=default` | the canonical visibility |
| `-fdebug-info-for-profiling` / `-fno-standalone-debug` | stripped debug |
| `-g -fno-eliminate-unused-debug-types` / `-g -feliminate-unused-debug-symbols` | stripped debug |
| `-mno-omit-leaf-frame-pointer` | `-g -Oz` already keeps the frame |
| `-mllvm -branch-fold-placement` | default on |
| `-mllvm -enable-post-misched` | default on |
| `-mllvm -post-RA-scheduler` | default on |
| `-mllvm -break-anti-dependencies=none` | default |
| `-mllvm -arm-adjust-jump-tables` / `-arm-global-merge` / `-enable-global-merge` | default on |
| `-mllvm -enable-andcmp-sinking` / `-old-thumb2-ifcvt` / `-ifcvt-branch-fold` | no effect at `-Oz` |
| `-mllvm -disable-early-ifcvt` | early ifcvt is off at `-Oz` |
| `-mllvm -disable-ifcvt-forked-diamond` | no effect |
| `-mllvm -block-placement-exit-block-bias=0` | default 0 |
| `-mllvm -tail-dup-placement` / `-tail-dup-placement-threshold=0` / `=20` | default on; threshold not binding |
| `-mllvm -tail-dup-size=1` | no tail-dup candidates below the default |
| `-mllvm -loop-to-cold-block-ratio=10` | no effect |
| `-mllvm -switch-peel-threshold=0/50/101` | no switch has profile weights |
| `-mllvm -forward-switch-cond` | no effect |
| `-mllvm -enable-implicit-null-checks` | default on |
| `-mllvm -enable-npm-newgvn` | legacy pass manager, no effect |
| `-mllvm -enable-loop-versioning-licm` / `-enable-nontrivial-unswitch` / `-enable-simple-loop-unswitch` | no effect at `-Oz` |
| `-mllvm -enable-cond-stores-vec` / `-simplifycfg-merge-cond-stores` | no effect |
| `-mllvm -enable-shrink-wrap` / `-disable-libcalls-shrinkwrap` | default on |
| `-mllvm -aggressive-ext-opt` / `-enable-nonnull-arg-prop` / `-enable-double-float-shrink` | no effect |
| `-mllvm -t2-reduce-limit=100` | default limit already >= 100 |
| `-mllvm -arm-constant-island-max-iteration=10` | default converges in <= 10 |
| `-mllvm -pre-RA-sched=default` / `=list-hybrid` | default is `list-hybrid` |
| `-mllvm -misched-bottomup` | no effect while misched is off |
| `-mllvm -slp-threshold=0` / `-unroll-threshold=0` | no effect |
| `-mllvm -enable-linkonceodr-outlining` | outliner not enabled |
| `-mllvm -disable-cgp-select2branch` | no effect |
| `-mllvm -enable-post-misched=false` | post-misched is already off |

### 2.2 Negative (`byte_exact < 4507`)

| flag | byte_exact | `.text` size (canonical 1,259,464) |
|---|---:|---:|
| `-mllvm -disable-cgp-branch-opts` | **4506** | 1,259,464 (1 B) |
| `-mllvm -disable-postra-machine-sink` | 1573 | 1,265,000 |
| `-mllvm -misched-postra` | 1575 | 1,259,464 |
| `-mllvm -enable-misched` | 1219 | 1,268,680 |
| `-mllvm -disable-post-ra` | 1226 | 1,259,656 |
| `-mllvm -disable-machine-sink` | 1235 | 1,263,208 |
| `-mllvm -pre-RA-sched=source` | 1199 | 1,256,136 |
| `-mllvm -pre-RA-sched=list-burr` | 1257 | 1,257,320 |
| `-mllvm -pre-RA-sched=list-ilp` | 1266 | 1,259,080 |
| `-mllvm -disable-block-placement` | 1262 | 1,268,072 |
| `-mllvm -disable-branch-fold` | 1355 | 1,263,144 |
| `-mllvm -disable-tail-duplicate` | 1963 | 1,259,496 |
| `-mllvm -disable-early-taildup` | 3626 | 1,259,464 |
| `-mllvm -enable-tail-merge` | 2677 | 1,259,688 |
| `-mllvm -tail-merge-threshold=0` | 1324 | 1,357,736 |
| `-mllvm -tail-merge-size=0` | 1328 | 1,261,928 |
| `-mllvm -tail-dup-size=100` | 1378 | 1,259,432 |
| `-mllvm -align-all-nofallthru-blocks=4` | 1237 | 1,463,688 |
| `-mllvm -enable-gvn-sink` | 1349 | 1,259,896 |
| `-mllvm -enable-gvn-hoist` | 1429 | 1,258,312 |
| `-mllvm -enable-newgvn` | 1400 | 1,257,224 |
| `-mllvm -arm-promote-constant` | 1386 | 1,264,616 |
| `-mllvm -disable-ifcvt-diamond` | 1428 | 1,259,720 |
| `-mllvm -enable-aa-sched-mi` | 1408 | 1,259,496 |
| `-mllvm -enable-ipra` | 1370 | 1,261,032 |
| `-mllvm -force-loop-cold-block` | 2025 | 1,259,848 |
| `-mllvm -block-placement-exit-block-bias=100` | 1616 | 1,259,400 |
| `-mllvm -switch-to-lookup` | 2614 | 1,259,464 |
| `-mllvm -enable-shrink-wrap=false` | 1457 | 1,259,400 |
| `-mllvm -break-anti-dependencies=all` | 1707 | 1,259,560 |
| `-mllvm -disable-sched-cycles` | 1379 | 1,258,760 |
| `-mllvm -disable-phi-elim-edge-splitting` | 1367 | 1,258,984 |
| `-mllvm -phi-elim-split-all-critical-edges` | 1740 | 1,259,592 |
| `-mllvm -t2-reduce-limit=0` | 947 | 1,383,476 |
| `-mllvm -t2-reduce-limit2=0` | 1218 | 1,264,584 |
| `-mllvm -t2-reduce-limit3=0` | 838 | 1,394,632 |
| `-mllvm -slp-threshold=100` | 1484 | 1,257,096 |
| `-mllvm -inline-threshold=0` | 1356 | 1,259,112 |
| `-mllvm -inline-threshold=100` | 1350 | 1,368,584 |
| `-fno-integrated-as --gcc-toolchain=...` | 2767 | 1,259,464 |
| `-fexperimental-new-pass-manager` | 1300 | 1,265,096 |
| `-ffast-math` | 1335 | 1,255,836 |
| `-funsafe-math-optimizations` | 1719 | 1,259,432 |
| `-ffinite-math-only` | 1437 | 1,258,696 |
| `-fno-signed-zeros` | 2356 | 1,259,400 |
| `-fno-math-errno` | 1356 | 1,259,064 |
| `-fno-exceptions` | 1344 | 1,168,580 |
| `-fno-unwind-tables` | 2878 | 1,259,464 |
| `-fvisibility=hidden` | 57 / 244 common | 1,249,080 |
| `-fvisibility=protected` | 1345 | 1,249,080 |
| `-fvisibility-inlines-hidden` | 1278 / 4383 common | 1,259,432 |
| `-fomit-leaf-frame-pointer` | 1379 | 1,259,124 |
| `-fno-jump-tables` | 1363 | 1,263,816 |
| `-Os` / `-O2` / `-O3` / `-O1` / `-O0` | 1263 / 1168 / 1167 / 1033 / 61 | 1,359,464 / 1,455,112 / 1,513,208 / 1,326,792 / 2,519,320 |
| `-Oz -fno-inline-functions` | 1219 | 1,294,152 |

`-mvectorize-with-neon-quad`: clang 7.0.2 rejects it (`unknown argument`);
`-enable-machine-outliner`: clang 7 crash in `VirtualInput.cpp`;
`-fno-semantic-interposition`: unknown argument in clang 7;
`-fPIE` / `-fpie`: link error (`R_ARM_REL32`, not `-fPIC`).

### 2.3 `.ARM.attributes` target pinning

The original's `.ARM.attributes` (byte-identical to ours) is:

```text
A7 00 00 00 00 'aeabi' 01 2d 00 00 00
05 'ARM v7\0'  06 0a  07 41  08 01  09 02
0a 03  0c 01  11 02  12 04  14 01  15 01
17 03  18 01  1a 02  22 01  26 01
```

`Tag_CPU_name` (tag 5) is `"ARM v7"`, the generic `-march=armv7-a` name.
`-mcpu=cortex-a8` / `-mcpu=cortex-a9` append a CPU-name tag and change the
blob (verified with a one-file compile); `-march=armv7-a -mtune=cortex-a8`
does **not** change it. So `-mtune` is the only free target knob, and it is
codegen-neutral here.

---

## 3. Per-TU matrix (residual TUs)

Each cell recompiles only the named TU with the named flag (all objects are
deleted first) and reports the whole-gate `byte_exact` plus the target
function's raw same-address diff bytes. Baseline diffs:
`Layout` 5,775; `LevelScript::process` 55,072; `Level::createShip` 1,411;
`Status::missionCompleted` 722; `Radar::draw` 7,724; `MGame::OnTouchEnd`
4,363; `Gun::calcCharacterCollision` 463; `HangarWindow::render` 2,062;
`SpaceLounge::drawLounge` 175; `Hud::draw` 1,417;
`Generator::createMission` 445; `MenuTouchWindow::OnTouchBegin` 29;
`ModStation` 11.

### 3.1 `-fno-math-errno` — neutral on every residual TU

All 13 cells keep `byte_exact 4507` and the baseline target diff (none of the
residual TUs call a libm function). Example: `Radar.cpp` -> `Radar::draw`
7,724; `Hud.cpp` -> `Hud::draw` 1,417.

### 3.2 `-fno-exceptions` — worse on every residual TU

| TU | target raw diff (baseline -> `-fno-exceptions`) | target size |
|---|---|---|
| Layout.cpp | 5,775 -> 5,775 | 7,072 -> 7,072 |
| LevelScript.cpp | 55,072 -> 55,227 | 57,232 -> 57,148 |
| Level.cpp (`createShip`) | 1,411 -> 2,468 | 2,764 -> 2,588 |
| Status.cpp | 722 -> 809 | 1,432 |
| Radar.cpp | 7,724 -> 9,624 | 9,908 -> 10,368 |
| MGame.cpp (`OnTouchEnd`) | 4,363 -> 7,453 | 8,080 -> 7,932 |
| Gun.cpp | 463 -> 522 | 1,984 |
| HangarWindow.cpp | 2,062 -> 8,573 | 9,700 -> 9,296 |
| SpaceLounge.cpp | 175 -> 1,745 | 1,948 -> 1,880 |
| Hud.cpp | 1,417 -> 8,866 | 9,332 -> 9,228 |
| Generator.cpp | 445 -> 1,505 | 1,996 -> 1,928 |
| MenuTouchWindow.cpp | 29 -> 144 | 1,896 |
| ModStation.cpp | 11 -> 834 | 1,008 -> 960 |

### 3.3 Per-TU scheduler flags — worse on every target

| TU | flag | target raw diff (baseline -> cell) |
|---|---|---|
| ModStation.cpp | `-mllvm -pre-RA-sched=source` | 11 -> 877 |
| MenuTouchWindow.cpp | `-mllvm -pre-RA-sched=source` | 29 -> 595 |
| Gun.cpp | `-mllvm -pre-RA-sched=source` | 463 -> 1,380 |
| SpaceLounge.cpp | `-mllvm -pre-RA-sched=source` | 175 -> 1,659 |
| Status.cpp | `-mllvm -pre-RA-sched=source` | 722 -> 802 |
| Generator.cpp | `-mllvm -pre-RA-sched=source` | 445 -> 1,157 |
| ModStation.cpp | `-mllvm -enable-misched` | 11 -> 783 |
| MenuTouchWindow.cpp | `-mllvm -enable-misched` | 29 -> 1,462 |
| Gun.cpp | `-mllvm -enable-misched` | 463 -> 1,796 |
| ModStation.cpp | `-mllvm -disable-postra-machine-sink` | 11 -> 149 |
| MenuTouchWindow.cpp | `-mllvm -disable-postra-machine-sink` | 29 -> 91 |
| Gun.cpp | `-mllvm -disable-postra-machine-sink` | 463 -> 518 |
| Generator.cpp | `-mllvm -disable-postra-machine-sink` | 445 -> 447 (keeps 4507) |
| ModStation.cpp | `-mllvm -misched-postra` | 11 -> 27 |
| MenuTouchWindow.cpp | `-mllvm -misched-postra` | 29 -> 47 |
| Gun.cpp | `-mllvm -misched-postra` | 463 -> 467 |
| ModStation.cpp | `-mllvm -disable-machine-sink` | 11 -> 921 |
| MenuTouchWindow.cpp | `-mllvm -disable-machine-sink` | 29 -> 519 |
| Gun.cpp | `-mllvm -disable-machine-sink` | 463 -> 1,505 |

`-mllvm -enable-post-misched=false` is neutral on all three tested TUs.

---

## 4. What the residual diffs actually are

The 17 residual functions are not one phenomenon:

* **Block/switch-body order** (`Status::missionCompleted` TBH entries,
  `LevelScript::process`, `Level::createMission`): the original's switch body
  layout is source order; ours is the `MachineBlockPlacement` result. The
  `-mllvm` placement knobs above either do nothing (`branch-fold-placement`,
  `tail-dup-placement`, `switch-peel-threshold`) or regress globally
  (`disable-block-placement`, `tail-merge-*`).
* **Register allocation / receiver caching** (`MGame::OnUpdate` 41 B,
  `MGame::OnTouchEnd`, `MenuTouchWindow::OnTouchBegin`, `ModStation` 11 B):
  the original reloads `[r9]` before each call; our source caches the receiver
  in a callee-saved register. This is source shape, not scheduling: the
  `source`/`list-ilp`/misched cells move it further away.
* **Literal-pool rotation** (`Level::createMission`, `Radar::draw`): pool
  entries follow first-use order, which follows body order.

No scheduler flag reproduces the original's body order while keeping the other
4,507 functions exact.

---

## 5. Reproduction

```sh
# canonical gate
cd /home/debian/GalaxyOnFire2/wt/w8419
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6

# scratch build with the flag launcher
cmake -S . -B /dev/shm/w8419/build -G Ninja \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs \
  -DGOF2_MATCH_OPT=-Oz \
  -DCMAKE_C_COMPILER_LAUNCHER=/dev/shm/w8419/launch.sh \
  -DCMAKE_CXX_COMPILER_LAUNCHER=/dev/shm/w8419/launch.sh

# one cell
GOF2_EXTRA_FLAGS="-mllvm -pre-RA-sched=source" \
  cmake --build /dev/shm/w8419/build --target link_so -j6
python3 /dev/shm/w8419/score.py /dev/shm/w8419/build/libgof2hdaa.so

# per-TU cell (build_tu uses launch_tu.sh; delete all objects first)
find /dev/shm/w8419/build_tu/CMakeFiles/gof2.dir -name '*.obj' -delete
GOF2_TU=Generator.cpp GOF2_TU_FLAGS="-mllvm -disable-postra-machine-sink" \
  cmake --build /dev/shm/w8419/build_tu --target link_so -j6
python3 /dev/shm/w8419/score.py /dev/shm/w8419/build_tu/libgof2hdaa.so \
  _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE

# external assembler (needs --gcc-toolchain; otherwise the driver uses host /bin/as)
GOF2_EXTRA_FLAGS="-fno-integrated-as --gcc-toolchain=/home/debian/GalaxyOnFire2/wt/w8419/.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64" \
  cmake --build /dev/shm/w8419/build --target link_so -j6
```

Scratch (not committed): `/dev/shm/w8419/` — `launch.sh`, `launch_tu.sh`,
`cell.sh`, `cell_tu.sh`, `cell_tu2.sh`, `score.py`, `sec.py`, `build/`,
`build_tu/`, `m_*.txt`, `mtu_*.txt`.

---

## 6. What would falsify this note

* A full-gate flag cell with `byte_exact >= 4508` and no size regression.
  None of the ~100 cells in this pass (plus w8119's ~60 and w8319's ~40) does;
  the closest is `-mllvm -disable-cgp-branch-opts` at 4506.
* A per-TU cell that lowers a residual function's raw diff while keeping
  `byte_exact 4507` and every section exact. None does.
* Evidence that the original's `.ARM.attributes` `Tag_CPU_name` is not
  `"ARM v7"` (it is byte-identical) or that `-mtune` is encoded anywhere in
  the ELF (it is not).
