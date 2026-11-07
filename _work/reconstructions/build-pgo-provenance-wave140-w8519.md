# Build-flag / PGO provenance, wave 140 (w8519)

Status: **note-only research commit**, 2026-11-07. Worktree
`/home/debian/GalaxyOnFire2/wt/w8519`, base `d4857f174` ("Wave 139 docs:
Layout w8401 raw 5,765"). Scratch: `/dev/shm/w8519`. No `src/`, `tools/`,
`CMakeLists.txt`, baseline, allowlist or doc change is landed. This note is
the only commit.

Gate at the base (fresh `cmake --build cmake-build-match --target check`):

```text
verify: byte 4507/4524  linked 4507/4524  size 4524/4524  avg 99.94
data:   exact 843/843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0  imports 207
symbol parity 5370/5370 (0 extra / 0 missing)
lint CLEAN ✓   drift 0
RATCHET PASS
```

Section deltas (fresh measurement of the canonical build; the numbers quoted
in the task/`docs/IDENTICALITY.md` are stale by the six wave-139 source
landings):

| section | size | differing bytes |
|---|---:|---:|
| `.text` | 1,259,464 | **84,651** |
| `.ARM.extab` | 156,312 | **1,395** |
| `.note.gnu.build-id` | 36 | **20** |
| `.rodata`, `.data`, `.bss`, `.ARM.exidx`, `.rel.plt` | exact | **0** |

Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`,
2,187,328 B.  Baseline residual raw diffs (same-address, per symbol):
ModStation C1/C2 11/11, `Status::missionCompleted` 722, `MenuTouchWindow::
OnTouchBegin` 28, `SpaceLounge::drawLounge` 175, `Gun::calcCharacterCollision`
463, `Generator::createMission` 445, `Level::createShip` 1,411, `Layout`
C1/C2 5,765/5,765, `MGame::OnTouchEnd` 4,363, `Hud::draw` 1,416,
`HangarWindow::render` 2,062, `Radar::draw` 7,724, `MGame::OnUpdate` 41,
`Level::createMission` 5,763, `LevelScript::process` 54,262 (residual total
90,427; the C1/C2 aliases are counted twice).

---

## 0. Bottom line

1. **No build flag raises `byte_exact` above 4507.** This pass adds **644
   cells** (612 scored; whole-gate + per-TU) to the ~220 of
   w8119/w8319/w8419. Every
   codegen-relevant flag is negative; every neutral flag leaves `.text`
   byte-identical (every neutral whole-gate cell reproduces the exact
   baseline residual total 90,427, i.e. they change nothing at all).
2. **The closest near-misses are 4506, and there are now two of them.**
   `-mllvm -disable-cgp-branch-opts` (w8419) breaks exactly
   `AbyssEngine::Engine::SetUVMatrix`; **`-mllvm -arm-force-fast-isel`** (new
   this pass) breaks exactly `FileInterfaceAndroid::~FileInterfaceAndroid`
   (16 B, and it shifts that symbol by 2 B). Combining the two gives 4505.
   Neither flag is the original's.
3. **The machine scheduler is off, and every scheduler knob is either
   neutral or catastrophic.** `-enable-misched` = 1219 (with any
   `-misched=converge/ilpmax/ilpmin` and any `-mtune`), `-misched-postra` =
   1575, `-disable-post-ra` = 1226, `-pre-RA-sched=source/list-burr/list-ilp`
   = 1199/1257/1266. All `-disable-sched-*` heuristic knobs, `-scheditins`,
   `-schedmodel`, `-max-sched-reorder`, `-dfa-sched-reg-pressure-threshold`,
   `-misched-*` (while misched is off) are **neutral and change no byte**.
4. **`-fno-integrated-as` (GNU as 4.9) is a global regression (2767) but
   neutral per-TU on the residual TUs.** The original used clang's integrated
   assembler; GNU as happens to emit identical bytes for the residual TUs'
   functions, so the 2767 loss is entirely in other TUs.
5. **Per-TU `-fno-exceptions` is worse on all 13 residual TUs** (re-confirmed
   on the post-wave-139 tree; `Level.cpp` even fails to compile with it).
   `-fvisibility=default` is neutral; `hidden`/`protected` are hard
   regressions. The exact `-g` level is `-g2` (`-g`/`-g2`/`-g3` neutral,
   `-g0`/`-g1`/`-gmlt` = 2071).
6. **The branch-weight default is the clang 7 default 2000:1.** A global
   `-likely-branch-weight` sweep (1..10000, with/without
   `-unlikely-branch-weight`) never exceeds 4001. The project's per-file
   7:1 override on `StarMap.cpp`/`SpaceLounge.cpp` is a *source-shape
   compensation*, not an original build flag: with it removed the whole
   layout shifts and `byte_exact` collapses to 2405.
7. **No `-mllvm` reproduces the residual block order.** Placement knobs
   (`-block-placement-exit-block-bias` 0..100, `-tail-dup-placement-*`,
   `-tail-merge-*`, `-switch-peel-threshold`, `-jump-table-density`,
   `-max-jump-table-size`, `-min-jump-table-entries`, `-phi-node-folding-
   threshold`, `-branch-fold-placement`, `-arm-adjust-jump-tables`) either do
   nothing or regress globally.

**Verdict (unchanged): the original is a plain `-Oz -g2` build with clang 7's
default scheduler, default visibility, default FP semantics, integrated
assembler, default 2000:1 branch weights, and heuristic block placement. The
17 residual functions are source shape, not build configuration.**

---

## 1. Method

Scratch builds (`/dev/shm/w8519/build`, `b1..b8`, `t1..t8`, `c1..c8`) use a
compiler launcher that appends `$GOF2_EXTRA_FLAGS` (or `$GOF2_TU_FLAGS` when
the source basename equals `$GOF2_TU`) *after* the canonical flags:

```sh
#!/bin/sh
exec "$@" $GOF2_EXTRA_FLAGS          # launch.sh
# launch_tu.sh checks basename($arg) == $GOF2_TU before appending
```

Each whole-gate cell deletes every `CMakeFiles/gof2.dir/*.obj`, relinks the
whole tree (`link_so -j6`, ~10 s), and scores with a byte-only scorer that
reproduces `verify.py`'s `byte_exact` exactly (canonical 4507, 17 bad). Eight
build dirs run in parallel; a 60-cell matrix takes ~85 s. Per-TU cells use
`t1..t8` and delete all objects first so no stale flagged object survives.
`c1..c8` are configured with `-DGOF2_NO_PER_FILE_WEIGHTS=ON` for the global
branch-weight sweep (clang 7 rejects a repeated `-likely-branch-weight`).

The canonical scratch build reproduces the gate exactly: `byte_exact 4507`,
`.text` diff 84,651, `.ARM.extab` 1,395.

---

## 2. Whole-gate matrix (new cells in this pass)

Canonical is `byte_exact 4507`, `.text` diff 84,651. "Neutral" below means
`byte_exact 4507` **and** the residual total is the baseline 90,427 (i.e. the
flag changes no byte of any of the 4,524 functions).

### 2.1 Machine scheduler / pre-RA scheduler (neutral)

| flag | note |
|---|---|
| `-mllvm -disable-sched-critical-path` | list-hybrid priority knob |
| `-mllvm -disable-sched-height` | " |
| `-mllvm -disable-sched-live-uses` | " |
| `-mllvm -disable-sched-reg-pressure` | " |
| `-mllvm -disable-sched-stalls` | " |
| `-mllvm -disable-sched-vrcycle` | " |
| `-mllvm -disable-2addr-hack` | " |
| `-mllvm -disable-dfa-sched` | " |
| `-mllvm -scheditins` / `-mllvm -schedmodel` | latency model choice |
| `-mllvm -max-sched-reorder=0` / `=100` | list-ilp only |
| `-mllvm -dfa-sched-reg-pressure-threshold=0` / `=100` | " |
| `-mllvm -misched=converge` / `ilpmax` / `ilpmin` | misched off, no effect |
| `-mllvm -misched-topdown` / `-misched-cluster` / `-misched-regpressure` / `-misched-fusion` / `-misched-cyclicpath` / `-misched-limit=0` / `=10` | misched off |
| `-mllvm -enable-misched=false` | explicit default |
| `-mllvm -enable-post-misched` | default on |
| `-mllvm -post-RA-scheduler` | default on |
| `-mllvm -break-anti-dependencies=none` | default |
| `-mllvm -machine-sink-bfi` / `-machine-sink-split` / `-max-uses-for-sinking=0` | default sink behaviour |
| `-mllvm -join-globalcopies=false` is *not* neutral (1893) | default on |

### 2.2 Machine scheduler (negative)

| flag | byte_exact |
|---|---:|
| `-mllvm -arm-force-fast-isel` | **4506** (breaks `FileInterfaceAndroid::~FileInterfaceAndroid`) |
| `-mllvm -disable-cgp-branch-opts` (w8419) | **4506** (breaks `Engine::SetUVMatrix`) |
| `-mllvm -disable-sched-physreg-join` | 1713 |
| `-mllvm -disable-sched-hazard` | 1370 |
| `-mllvm -break-anti-dependencies=critical` | 4356 |
| `-mllvm -enable-misched` | 1219 (same for every `-mtune=a5/a7/a8/a9/a15` and every `-misched=`) |
| `-mllvm -enable-misched -misched-topdown` | 1206 |
| `-mllvm -enable-misched -misched-postra` | 1207 |
| `-mllvm -enable-misched -misched=ilpmax` / `ilpmin` | 1159 / 1127 |
| `-mllvm -misched-postra` | 1575 |
| `-mllvm -disable-post-ra` | 1226 |
| `-mllvm -enable-post-misched=false -post-RA-scheduler=false` | 1218 |
| `-mllvm -disable-postra-machine-sink` (w8419) | 1573 |
| `-mllvm -disable-postra-machine-licm` | 2301 |
| `-mllvm -disable-machine-licm` / `-machine-cse` / `-machine-dce` | 1362 / 1345 / 1402 |
| `-mllvm -machine-sink-split-probability-threshold=0` | 1912 |
| `-mllvm -hoist-cheap-insts` / `-join-globalcopies` | 1766 / 1893 |
| `-mllvm -cgp-freq-ratio-to-skip-merge=0` | 3029 |
| `-mllvm -consthoist-with-block-frequency` | neutral; `-disable-constant-hoisting` 1543 |
| `-mllvm -enable-aa-sched-mi -disable-sched-cycles` | 1319 |
| `-mllvm -disable-sched-hazard -disable-sched-cycles` | 1387 |
| `-mllvm -enable-machine-outliner` | **clang 7 crash** (`Machine Outliner` pass, aborts on every TU) |

### 2.3 Branch / block placement (the block-order hypothesis)

| flag | byte_exact |
|---|---:|
| `-mllvm -block-placement-exit-block-bias=1` | 3022 |
| `=5` / `=10` / `=20` / `=25` / `=33` / `=50` / `=75` | 1734 / 1729 / 1729 / 1729 / 1729 / 1734 / 1627 |
| `-mllvm -tail-dup-placement-threshold=1/5/10/50/100/200` | **4507** (neutral) |
| `-mllvm -tail-dup-placement-aggressive-threshold=0` / `-tail-dup-placement-penalty=0/1000` | neutral |
| `-mllvm -tail-dup-size=2/5/10/20/50` | 1359 / 1392 / 1378 / 1378 / 1378 |
| `-mllvm -tail-dup-indirect-size=0` / `-tail-dup-limit=0` | neutral / 2024 |
| `-mllvm -tail-merge-threshold=1/2/5` | 1324 / 1346 / 1367 |
| `-mllvm -tail-merge-size=1/2/5` | 1327 / 1370 / 1727 |
| `-mllvm -switch-peel-threshold=1/10/25/75/90/99` | **4507** (neutral) |
| `-mllvm -branch-fold-placement=false` | 1402 |
| `-mllvm -disable-branch-fold` | 1355 |
| `-mllvm -jump-table-density=0/10/50/100` | **4507** (neutral) |
| `-mllvm -max-jump-table-size=0/100/1000` | **4507** (neutral) |
| `-mllvm -min-jump-table-entries=0/1/10/100` | 1359 / 1359 / 1405 / 1390 |
| `-mllvm -optsize-jump-table-density=100` | 1414 |
| `-mllvm -phi-node-folding-threshold=0/1/100` | 1407 / 1403 / 1422 |
| `-mllvm -jump-is-expensive=true` / `=false` | 1352 / neutral |
| `-mllvm -jump-inst-cost=0/10` | neutral |
| `-mllvm -min-predictable-branch=0/10/100` | neutral |
| `-mllvm -arm-adjust-jump-tables` (default) | neutral |
| `-mllvm -canon-nth-basicblock=0` | neutral |
| `-mllvm -align-all-blocks=2/4` | 1190 / 1173 |
| `-mllvm -jump-threading-threshold=0/100` | 1352 / 1662 |
| `-mllvm -unswitch-threshold=0/100`, `-triangle-chain-count=0` | neutral |
| `-mllvm -simplifycfg-*`, `-disable-mergeicmps`, `-mergefunc-sanity=0` | neutral |

### 2.4 ARM target / other codegen

| flag | byte_exact |
|---|---:|
| `-mllvm -arm-load-store-opt=false` | 1233 |
| `-mllvm -arm-assume-misaligned-load-store` | 1225 |
| `-mllvm -arm-use-mulops=false` | 1545 |
| `-mllvm -arm-promote-constant=false`, `-max-size=0/4`, `-max-total=0` | **4507** (neutral) |
| `-mllvm -arm-synthesize-thumb-1-tbb`, `-arm-atomic-cfg-tidy`, `-arm-default-it`, `-arm-no-restrict-it` | neutral |
| `-mllvm -arm-implicit-it` | build error |
| `-mllvm -global-merge-group-by-use=false` | 4469 |
| `-mllvm -global-merge-ignore-single-use=false` | 1397 |
| `-mllvm -global-merge-max-offset=0`, `-on-const=false`, `-on-external=false` | neutral |
| `-mllvm -disable-cgp` | 1241 |
| `-mllvm -disable-cgp-ext-ld-promotion` / `-gc-opts` / `-store-extract` / `-select2branch` | neutral |
| `-mllvm -cgp-type-promotion-merge=false` | neutral |
| `-mllvm -inline-threshold=5` | **4507** (neutral, = the `-Oz` default) |
| `-mllvm -inline-threshold=10/20/30/50/75` | 1354 / 1352 / 1352 / 1350 / 1350 |
| `-mllvm -inlinehint-threshold=0/100/1000`, `-inlinecold-threshold=0/100` | neutral |
| `-mllvm -slp-threshold=1/10` | 1637 / 1492 |
| `-mllvm -unroll-threshold=1/10/100/400` | 2716 / 2716 / 1598 / 1413 |
| `-mllvm -unroll-count=0/1/4/8`, `-unroll-runtime`, `-unroll-partial-threshold=1/10/100`, `-unroll-allow-partial=false` | neutral |
| `-mllvm -licm-max-num-uses-traversed=0/1/100` | neutral |
| `-mllvm -cold-branch-ratio=0/0.5/1`, `-cold-synthetic-count=0/1000`, `-hot/cold-callsite-*` | neutral |
| `-mllvm -vectorize-slp=true/false` | neutral |
| `-mllvm -ifcvt-limit=1` neutral / `=0` 1278; `-early-ifcvt-limit=1/10`, `-stress-early-ifcvt` neutral | |
| `-mllvm -enable-selectiondag-sp` | neutral |
| `-mllvm -loop-to-cold-block-ratio=1/2/5/20/50` | neutral |
| `-mllvm -avoid-speculation`, `-condsider-local-interval-cost=false` | neutral |
| `-fmerge-functions` / `-fno-merge-functions` | clang 7 rejects both |

### 2.5 `-g` level, visibility, assembler

| flag | byte_exact | note |
|---|---:|---|
| `-g` / `-g2` | 4507 | `-g` is `-g2` |
| `-g3` / `-gdwarf-4` | 4507 | stripped debug |
| `-g -fno-eliminate-unused-debug-types` / `-feliminate-unused-debug-symbols` | 4507 | stripped debug |
| `-g0` / `-g1` / `-gmlt` | 2071 | frame pointer / debug info changes codegen |
| `-fvisibility=default` | 4507 | canonical |
| `-fvisibility=hidden` | 57 / 244 common | collapses `.dynsym` |
| `-fvisibility=protected` | 1345 | |
| `-fvisibility-inlines-hidden` | 1278 / 4383 common | |
| `-fno-omit-frame-pointer` | 4507 | already the `-Oz -g` default |
| `-fomit-frame-pointer` | 1298 | |
| `-fno-integrated-as --gcc-toolchain=...4.9...` | **2767** | GNU as 4.9; original used the integrated assembler |

---

## 3. Per-TU matrix

### 3.1 `-fno-exceptions` on the 13 residual TUs (all worse)

| TU | target raw diff (baseline -> cell) |
|---|---|
| `Layout.cpp` | 5,765 -> 5,765 (but 1,369 whole-gate: StarMap layout shift) |
| `LevelScript.cpp` | 54,262 -> 55,170 |
| `Level.cpp` | **build error** (TU uses exceptions) |
| `Status.cpp` | 722 -> 809 |
| `Radar.cpp` | 7,724 -> 9,624 |
| `MGame.cpp` | 4,363 -> 7,453 (OnUpdate 41 -> 8,856) |
| `Gun.cpp` | 463 -> 522 |
| `HangarWindow.cpp` | 2,062 -> 8,573 |
| `SpaceLounge.cpp` | 175 -> 1,745 |
| `Hud.cpp` | 1,416 -> 8,866 |
| `Generator.cpp` | 445 -> 1,505 |
| `MenuTouchWindow.cpp` | 28 -> 143 |
| `ModStation.cpp` | 11 -> 834 |

### 3.2 Per-TU flags that keep `byte_exact 4507` — none improves a residual

208 cells: 16 flags x 13 TUs. The flags that keep 4507
(`-disable-cgp-branch-opts`, `-arm-force-fast-isel`, `-global-merge-group-by-
use=false`, `-disable-sched-physreg-join`, `-disable-postra-machine-licm`,
`-join-globalcopies`, `-hoist-cheap-insts`, `-cgp-freq-ratio-to-skip-merge=0`,
`-jump-table-density=0`, `-tail-merge-size=5`, `-tail-dup-size=10`,
`-phi-node-folding-threshold=0`, `-min-jump-table-entries=0`,
`-arm-load-store-opt=false`, `-disable-machine-cse`, `-disable-machine-licm`)
all leave the residual total at exactly **90,427** — they change no residual
byte. Only two cells move it, both up: `Radar.cpp -disable-postra-machine-
licm` 90,436 and `HangarWindow.cpp -disable-postra-machine-licm` 90,445.

### 3.3 Per-TU optimization level / `-g` / assembler / scheduler

| TU | flag | byte_exact | note |
|---|---|---:|---|
| any | `-Os`/`-O1`/`-O2`/`-O3` | 1346..2201 | all negative |
| `Layout.cpp`, `Gun.cpp` | `-g0` / `-g1` | 4507 | neutral (residual total 90,427) |
| `Generator.cpp` | `-g0` / `-g1` | 4504 | 3 exact fns break |
| `Radar`, `MenuTouchWindow`, `LevelScript`, `Hud`, `Gun`, `Generator` | `-fno-integrated-as` | 4507 | GNU as identical for these TUs |
| any | `-fomit-frame-pointer` | 2214..3084 | negative |
| `Gun.cpp` | `-mllvm -enable-misched` | 3797 | best per-TU misched, still negative |
| any | `-mllvm -pre-RA-sched=source` | <= 2818 | negative |

---

## 4. Branch-weight provenance (new)

The canonical build carries **per-file** `-mllvm -likely-branch-weight=7
-mllvm -unlikely-branch-weight=1` on `StarMap.cpp` and `SpaceLounge.cpp`
(`CMakeLists.txt`). Those are documented *matching knobs for source shape*,
not original build flags. To test whether the original used a non-default
**global** weight, `c1..c8` are configured with
`-DGOF2_NO_PER_FILE_WEIGHTS=ON` and a global weight is appended.

| global weight | byte_exact |
|---|---:|
| 1:1 / 2:1 / 3:1 / 5:1 / 7:1 / 10:1 | 2126 / 2045 / 2327 / 2087 / 2045 / 3155 |
| 20:1 / 50:1 / 100:1 | 3155 / 4001 / 4001 |
| 500:1 / 1000:1 / 2000:1 / 4000:1 / 10000:1 | 2405 / 2405 / 2405 / 2405 / 2109 |
| 7:2 / 7:3 / 7:5 / 7:10 | 2262 / 2092 / 2110 / 2340 |
| 10:2 / 20:2 / 50:2 / 100:2 / 2000:2 | 2087 / 3155 / 3456 / 4001 / 2405 |
| 2000:100 | 3155 |

The best global weight (50:1 or 100:1) is 4001 — far below 4507. The
`GOF2_NO_PER_FILE_WEIGHTS=ON` baseline (global 2000:1, no per-file 7:1) is
**2405**: removing the 7:1 on `StarMap.cpp` changes that TU's code size and
shifts every later function, so 2,102 cross-TU `bl` offsets no longer match.
This proves the 7:1 overrides are load-bearing source-shape compensations and
that the original's global default is the clang 7 default 2000:1.

---

## 5. What the residual diffs are (unchanged)

* **Block/switch-body order** (`Status::missionCompleted` TBH entries,
  `LevelScript::process`, `Level::createMission`): the original's TBH table is
  jumbled (`0x1a2,0x1b7,0xcd,...`), ours is mostly ascending
  (`0xcd,0xe3,0xfd,...`). This is `MachineBlockPlacement` with different
  branch weights, i.e. source shape. No placement knob reproduces it while
  keeping the other 4,507 functions exact.
* **Register allocation / receiver caching** (`MGame::OnUpdate` 41 B,
  `MGame::OnTouchEnd`, `MenuTouchWindow::OnTouchBegin`, `ModStation` 11 B):
  the original reloads `[r9]` before each call; our source caches the receiver
  in a callee-saved register. Source shape, not scheduling.
* **Literal-pool rotation** (`Level::createMission`, `Radar::draw`): pool
  entries follow first-use order, which follows body order.

The one new observation is that `-mllvm -arm-force-fast-isel` is a second
"single-function" near-miss: it shifts `FileInterfaceAndroid::~FileInterface-
Android` by 2 B and changes its two PC-relative targets, but leaves all 17
residual functions byte-identical. It is not the original's flag.

---

## 6. Reproduction

```sh
# canonical gate
cd /home/debian/GalaxyOnFire2/wt/w8519
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6

# scratch whole-gate build with the flag launcher
cmake -S . -B /dev/shm/w8519/build -G Ninja \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs \
  -DGOF2_MATCH_OPT=-Oz \
  -DCMAKE_C_COMPILER_LAUNCHER=/dev/shm/w8519/launch.sh \
  -DCMAKE_CXX_COMPILER_LAUNCHER=/dev/shm/w8519/launch.sh

# one cell
GOF2_EXTRA_FLAGS="-mllvm -arm-force-fast-isel" \
  cmake --build /dev/shm/w8519/build --target link_so -j6
python3 /dev/shm/w8519/score.py /dev/shm/w8519/build/libgof2hdaa.so

# per-TU cell (launch_tu.sh checks the source basename)
GOF2_TU=Gun.cpp GOF2_TU_FLAGS="-mllvm -disable-cgp-branch-opts" \
  cmake --build /dev/shm/w8519/t1 --target link_so -j6

# global branch weight (per-file overrides disabled)
cmake -S . -B /dev/shm/w8519/c1 -G Ninja ... -DGOF2_NO_PER_FILE_WEIGHTS=ON ...
GOF2_EXTRA_FLAGS="-mllvm -likely-branch-weight=50 -mllvm -unlikely-branch-weight=1" \
  cmake --build /dev/shm/w8519/c1 --target link_so -j6

# external assembler (needs --gcc-toolchain; otherwise host /bin/as)
GOF2_EXTRA_FLAGS="-fno-integrated-as --gcc-toolchain=/home/debian/GalaxyOnFire2/wt/w8519/.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64" \
  cmake --build /dev/shm/w8519/build --target link_so -j6
```

Scratch (not committed): `/dev/shm/w8519/` — `launch.sh`, `launch_tu.sh`,
`matrix.sh`, `matrix2.sh`, `matrix_tu.sh`, `matrix_c.sh`, `score.py`,
`resid.py`, `sec.py`, `cells_*.txt`, `out_*.txt`, `build/`, `b1..b8/`,
`t1..t8/`, `c1..c8/`.

---

## 7. What would falsify this note

* A full-gate flag cell with `byte_exact >= 4508` and no size regression.
  None of the 644 cells in this pass (plus w8119's ~60, w8319's ~40 and
  w8419's ~100) does; the closest are `-disable-cgp-branch-opts` and
  `-arm-force-fast-isel` at 4506.
* A per-TU cell that lowers a residual function's raw diff while keeping
  `byte_exact 4507` and every section exact. None of the 364 per-TU cells
  does.
* Evidence that the original's global branch weight is not 2000:1 (the whole
  global sweep tops out at 4001) or that `-g` is not `-g2` (`-g0`/`-g1`/
  `-gmlt` collapse to 2071).
