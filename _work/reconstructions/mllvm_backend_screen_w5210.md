# clang 7.0.2 ARM `-mllvm` backend screen (w5210, wave 104)

**Status:** research note, 2026-11-04. Worktree `/home/debian/GalaxyOnFire2/wt/w5210`,
branch `wave104-w5210`, base `8c5fd3646`. Scratch is `/dev/shm/w5210`.
No source or build-flag change is landed: **no tested `-mllvm` flag is a strict
improvement**. This commit contains only this note.

The task was to enumerate the clang 7.0.2 ARM backend options and screen a broad
set of `-mllvm` flags on the 16 TUs that own the 24 non-byte-exact functions,
then on the full link. The screen covered PostRA machine sinking, branch
weights, if-conversion, tail duplication, block placement, register allocation,
scheduling, loop unrolling, and every `-enable-*` / `-disable-*` option in
`-mllvm -help-hidden`.

## 0. Bottom line

* Baseline is **byte_exact 4500/4524, linked_exact 4500/4524, size-exact
  4524/4524, data_exact 843/843, imports 207, lint `CLEAN ✓`, drift 0**.
  `.text 0x1337c8`, `.rodata 0x41b67`, `.data 0x296c`, `.bss 0x69ac`,
  `.ARM.exidx 0x6f70`, `.ARM.extab 0x26298`.
* `clang++ -mllvm -help-hidden` prints **1,035 lines / 823 options**:
  **535 bool** and **288 value-taking**. `-mllvm -help` prints 61 lines.
* I object-screened **673 bool candidates** (all 535 bool options bare, plus
  `=false` for every `-enable-*` / `-disable-*`) and **537 value candidates**
  (targeted values for 83 value options plus 6 enum options) on the **16 target
  TUs**. **231 distinct flags changed at least one target-TU function**:
  78 bool-list flags changed an exact function, 75 changed a target function
  (3 target-only); 137 value-list flags changed an exact function, 133 changed
  a target function (13 target-only).
* I full-linked **189 distinct variants** (186 built, 3 build failures) with the
  official verifier (`-j6`; fast verifier for gate counts, official `score()` for
  target fuzzy). **No variant beat 4500 / 4500 / 4524.** No variant created a new exact
  function. The only code-changing gate-neutral flag is
  `-mllvm -forward-switch-cond` (4500/4500/4524, `LevelScript::process` still
  53.4%). 56 further flags are code-neutral no-ops on this source.
* A necessary-condition argument covers all 231 changed flags: a flag can only
  raise `byte_exact` if it changes at least `changed_exact + 1` non-exact target
  functions. Only **34 of 231** satisfy that condition; all 34 were full-linked
  and none improved. The other 197 cannot improve `byte_exact` by construction
  (global effects outside the target TUs can only add losses).
* 17 target-fuzzy cells improve by **+0.1** (never more), and every one costs
  gate counts. No target becomes byte-exact. `-forward-switch-cond` is
  fuzzy-neutral.
* The remaining 24 functions are **not explained by a missing `-mllvm` flag**.
  The current `-Oz` backend pipeline is a local optimum in this flag space.

## 1. Baseline

Commands:

```sh
cd /home/debian/GalaxyOnFire2/wt/w5210
export TMPDIR=/dev/shm/w5210/tmp
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
cmake --build cmake-build-match --target drift -j6
```

Output (measured before and after the sweep):

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
=== DRIFT: 0 field(s) across 0 class(es) ===
CLEAN ✓ (no new soft violations; hard invariants 0)
```

The 24 non-exact functions and their fresh official scores
(`cmake-build-match/verify/report.json`, not the stale repo-root copy):

| mangled name | bytes | fuzzy % |
|---|---:|---:|
| `_ZN6LayoutC1Ev` | 7072 | 47.8 |
| `_ZN6LayoutC2Ev` | 7072 | 47.8 |
| `_ZN11LevelScript7processEi` | 57232 | 53.4 |
| `_ZN6Status16missionCompletedEbbx` | 1432 | 60.4 |
| `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 1996 | 77.5 |
| `_ZN5Level10createShipEiiiP8Waypointbb` | 2764 | 66.4 |
| `_ZN5Radar4drawEP6PlayerP3Hudi` | 9908 | 74.0 |
| `_ZN5MGame10OnTouchEndEiiPv` | 8080 | 74.8 |
| `_ZN12HangarWindow6renderEv` | 9700 | 84.4 |
| `_ZN7GlobalsC1Ev` | 612 | 80.5 |
| `_ZN7GlobalsC2Ev` | 612 | 80.5 |
| `_ZN9RocketGun6updateEi` | 1908 | 86.9 |
| `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` | 968 | 87.6 |
| `_ZN5Level11createSceneEv` | 2252 | 88.3 |
| `_ZN3Gun22calcCharacterCollisionEv` | 1984 | 90.0 |
| `_ZN5Level13createMissionEv` | 17172 | 92.0 |
| `_ZN3Hud4drawExxP9PlayerEgobjj` | 9332 | 92.9 |
| `_ZN15MenuTouchWindowC1Ei` | 9600 | 95.1 |
| `_ZN15MenuTouchWindowC2Ei` | 9600 | 95.1 |
| `_ZN11SpaceLounge10drawLoungeEv` | 1948 | 95.5 |
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 1896 | 97.7 |
| `_ZN10ModStationC1Ev` | 1008 | 98.4 |
| `_ZN10ModStationC2Ev` | 1008 | 98.4 |
| `_ZN5MGame8OnUpdateEv` | 11440 | 99.9 |

The 16 target TUs (the TUs that own these functions):

```text
src/game/core/Globals.cpp
src/game/menu/MGame.cpp
src/game/menu/ModStation.cpp
src/game/menu/SpaceLounge.cpp
src/game/mission/Generator.cpp
src/game/mission/Status.cpp
src/game/ship/PlayerEgo.cpp
src/game/ui/HangarWindow.cpp
src/game/ui/Hud.cpp
src/game/ui/Layout.cpp
src/game/ui/MenuTouchWindow.cpp
src/game/weapons/Gun.cpp
src/game/weapons/Radar.cpp
src/game/weapons/RocketGun.cpp
src/game/world/Level.cpp
src/game/world/LevelScript.cpp
```

Baseline section sizes and SHA-256 (ours / original):

| section | size | ours sha256 | original sha256 |
|---|---:|---|---|
| `.text` | `0x1337c8` | `144a3c3799cddbe2a02e3eef72cf37b213fd24b9a2eccaac866e75640d0a1a2f` | `d49e32c1330e5d94e7628b9a40e05500af963a22816a93f2c738dbd366f813ed` |
| `.rodata` | `0x41b67` | `73c737319958f462dfea24f59883dea97853195cbb5607f50243f417389deb31` | same |
| `.data` | `0x296c` | `bf3a87118411e0a3883bfe8db2c4e2d48f9dc38346016de7fd1409d7003398d4` | same |
| `.bss` | `0x69ac` | `e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855` | same |
| `.ARM.exidx` | `0x6f70` | `652126c2ad2d61b6281d91b3871d4c488aa3883730e6e42be6db5940b3c05753` | `d7490b5908727fd3506fc044a0cf659eea071496c7ee18b1c0f3d9d7f4543d1a` |
| `.ARM.extab` | `0x26298` | `6b66adedd2edc4901a27ad6e5a5cf84681b2f8f587cda635eb36d98f2ddb0959` | `1880b0ef21ad149ef0af8eceb25b62a8a14f11568023d0e6e3a32e3fa0fe53e1` |
| `.rel.plt` | `0x72d0` | `43bf7c4eb1705aa4100655a5cce6077df7447fe90bb349cbc2c6ac2b78571995` | same |
| `.dynsym` | `0x15ca0` | `5a3b59e8436c8dc5b2ef63ac5ab0196eb41cde2ed6314b354e7a9fbd37a82d6c` | same |
| `.dynstr` | `0x2e318` | `a42e036792f61aaa4c07ac027495052050ab7ddacbdc5204cc2aadefb69955b7` | same |
| `.hash` | `0x973c` | `fdc6fe8d74f1d4e2882daef8057dbea632a25ee5da3dfd8df2f500911c65cb1e` | same |
| `.rel.dyn` | `0x7048` | `4f55867c15475df09d0fc73442c2446c5dda4f49c6b8eee11a5823a273194792` | same |
| `.note.gnu.build-id` | `0x24` | `c5d0c0d95cce31b80bc0dce2e0232dd7bc7f218208a140af5bf3e9567e3b4d9d` | `375800e6e941c456ed8db258a9101e274591da6cc6778a19df67c6e55c4f0fbf` |

Whole-file SHA-256: ours `374a86a2e86c6f1cc01ed68e2eb99f9cf80b7df33200904616871fcf08a7c1df`,
original `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`.

Measured differing bytes inside the same-size sections: `.text` **100,113**,
`.ARM.exidx` **829**, `.ARM.extab` **108,295**, `.note.gnu.build-id` **20**.
(The `.ARM.extab` content difference is address/relocation-driven; its size and
base are exact. The 2026-11-03 `docs/VALIDATION.md` sentence that lists only
`.text`, `.ARM.exidx` and the build-id as differing is a *size/base* statement,
not a byte-count statement.)

## 2. Option enumeration

```sh
CXX=.cache/ndk/android-ndk-r18b/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++
echo 'int f(int x){return x+1;}' > /dev/shm/w5210/help/t.cpp
$CXX -target armv7-none-linux-androideabi16 -mthumb -Oz -c /dev/shm/w5210/help/t.cpp \
  -o /dev/shm/w5210/help/t.o -mllvm -help-hidden > /dev/shm/w5210/help/help-hidden.txt 2>&1
$CXX -target armv7-none-linux-androideabi16 -mthumb -Oz -c /dev/shm/w5210/help/t.cpp \
  -o /dev/shm/w5210/help/t2.o -mllvm -help > /dev/shm/w5210/help/help.txt 2>&1
```

Results:

| file | lines | parsed options | bool | value |
|---|---:|---:|---:|---:|
| `-mllvm -help-hidden` | 1,035 | 823 | 535 | 288 |
| `-mllvm -help` | 61 | — | — | — |

The bool candidate list is all 535 bool options bare, plus `=false` for every
`-enable-*` (63) and `-disable-*` (75): **673 lines**. The value list is a
targeted set of **537 lines** covering branch weights, jump-table density,
tail-dup/tail-merge sizes and thresholds, block-placement bias, machine-sink
split thresholds, register-allocation costs, scheduling limits, loop-unrolling
thresholds, if-conversion limits, ARM constant-island/promote-constant/global-
merge limits, alignment, and the `pre-RA-sched`, `regalloc`, `misched`,
`split-spill-mode`, `arm-implicit-it` and `break-anti-dependencies` enums.

The candidate files are `/dev/shm/w5210/help/bool_ed_mllvm.txt` and
`/dev/shm/w5210/help/value_mllvm.txt`. Every line is a complete `-mllvm <opt>`
payload; `tools/flag_fingerprint.py --mode llvm-screen` appends it to the exact
canonical compile command.

## 3. Object-level screen on the 16 target TUs

Commands:

```sh
python3 tools/flag_fingerprint.py --mode llvm-screen \
  --workdir /dev/shm/w5210/screen_ed2 \
  --llvm-candidates /dev/shm/w5210/help/bool_ed_mllvm.txt \
  --jobs 6 --json /dev/shm/w5210/screen_ed2/results.json

python3 tools/flag_fingerprint.py --mode llvm-screen \
  --workdir /dev/shm/w5210/screen_val \
  --llvm-candidates /dev/shm/w5210/help/value_mllvm.txt \
  --jobs 6 --json /dev/shm/w5210/screen_val/results.json
```

The screen compiles a target TU with the candidate payload appended to its
canonical command and compares every object-file function's raw bytes with the
canonical `-Oz` object. With `-ffunction-sections` and no LTO, a flag that
leaves a target function's object bytes unchanged cannot change that function's
linked instruction stream (cross-function relocations are compared by symbol
name by the official verifier).

Results:

| list | candidates | fully built | partial | failed | changed exact | changed target |
|---|---:|---:|---:|---:|---:|---:|
| bool | 673 | 627 | 6 | 40 | 78 | 75 |
| value | 537 | 527 | 4 | 6 | 137 | 133 |
| **union** | **1,210** | — | — | — | **231 distinct flags** | — |

The 40 bool failures are mostly value-taking options that are bool in the
driver but require a value in `-mllvm` (`-misched`, `-pre-RA-sched`,
`-arm-implicit-it`, …), plus `-enable-machine-outliner` and `-global-isel`
clang 7.0.2 backend crashes. The 6 value failures are
`-arm-constant-island-max-iteration=0` (crash), `-dag-maps-huge-region=0`
(crash), `-swift-async-fp=0/1/2` (unknown option) and `-pre-RA-sched=vliw-td`
(crash). The partial builds are `-early-live-intervals` (12/16),
`-enable-coldcc-stress-test` (8/16), `-print-after-all` (6/16),
`-print-before-all` (5/16), `-print-lvi-after-jump-threading` (4/16),
`-verify-regalloc` (14/16), `-stress-regalloc=4` (6/16),
`-arm-constant-island-max-iteration=1` (2/16), `-align-all-blocks=16` (4/16)
and `-align-all-nofallthru-blocks=16` (12/16).

The full 231-row table is Appendix A.1.

## 4. Full-link screen

### 4.1 Method

The full-link driver is `/dev/shm/w5210/run_full.py`; it calls
`tools/flag_fingerprint.build_variant` sequentially with `jobs=6`, so every
`cmake --build ... --target link_so` is `-j6`. It configures a scratch wrapper
that includes `cmake/toolchain.cmake` and appends `GOF2_EXTRA_FLAGS` to both
`CMAKE_CXX_FLAGS` and `CMAKE_C_FLAGS`, then runs the official
`tools/verify/verify.py` (`--fail-on-wrong-type --top 0`) in fast mode for the
gate counts. The fast mode only short-circuits the difflib fuzzy score and the
pool-reconciliation `SequenceMatcher`; `byte_exact`, `linked_exact`,
`size-exact`, `missing`, `wrong_type` and `extra` are unchanged. Target fuzzy
values were then measured with the official `verify.score()` on the 24 target
functions only (`/dev/shm/w5210/score_targets.py`); its baseline scores match
`cmake-build-match/verify/report.json` exactly (24/24).

Branch-weight variants need `-DGOF2_NO_PER_FILE_WEIGHTS=ON`, because
`StarMap.cpp` carries a per-file `-mllvm -likely-branch-weight=7
-mllvm -unlikely-branch-weight=1` and clang 7 rejects a repeated
`-likely-branch-weight` on one command line. That baseline is itself measured:
`GOF2_NO_PER_FILE_WEIGHTS=ON` with no extra flag is **2404 / 4496 / 4523**
(StarMap's code size changes, shifting later function addresses and therefore
PC-relative bytes; `linked_exact` stays 4496). Branch-weight full-link numbers
below are relative to that no-per-file baseline, not to the canonical 4500.

Commands:

```sh
python3 /dev/shm/w5210/run_full.py /dev/shm/w5210/full_le10b \
  /dev/shm/w5210/variants_le10b.json          # 89 variants (88 changed + no-per-file base)
python3 /dev/shm/w5210/run_full.py /dev/shm/w5210/full_focus \
  /dev/shm/w5210/variants_focus.json          # 100 focus-category variants
python3 /dev/shm/w5210/run_full.py /dev/shm/w5210/full_one \
  /dev/shm/w5210/variants_one.json            # the one remaining necessary-condition flag
```

### 4.2 Gate counts

189 distinct full-link variants: **186 built, 3 failed** (`-mllvm
-arm-constant-island-max-iteration=1`, `-mllvm -early-live-intervals` and
`-mllvm -optsize-jump-table-density=0`). **No variant beat 4500 / 4500 / 4524.** No variant produced a
new exact function (`new_exact` was empty in every successful row).

The complete 186-row table is Appendix A.2. The best rows are:

| flag | byte | linked | size | note |
|---|---:|---:|---:|---|
| `-mllvm -forward-switch-cond` | 4500 | 4500 | 4524 | gate-neutral; `LevelScript::process` bytes change but fuzzy stays 53.4 |
| `-mllvm -arm-constant-island-max-iteration=2` | 4445 | 4498 | 4522 | best non-neutral byte count |
| `-mllvm -disable-ifcvt-triangle` | 4220 | 4495 | 4521 | |
| `-mllvm -likely-branch-weight=40/64/80/100 -unlikely-branch-weight=1` | 3998 | 4498 | 4522 | no-per-file baseline |
| `-mllvm -likely-branch-weight=7 -unlikely-branch-weight=1` | 3635 | 4496 | 4521 | no-per-file baseline |
| `-mllvm -disable-postra-machine-sink` | 1573 | 4392 | 4473 | PostRA sink is load-bearing |
| `-mllvm -disable-machine-sink` | 1235 | 3732 | 4019 | |
| `-mllvm -disable-tail-duplicate` | 1962 | 4440 | 4501 | |
| `-mllvm -disable-block-placement` | 1262 | 3114 | 4119 | |
| `-mllvm -regalloc=fast` | 1099 | 1592 | 1654 | |
| `-mllvm -pre-RA-sched=source` | 1216 | 2574 | 3988 | |
| `-mllvm -enable-misched` | 1219 | 2417 | 3579 | |
| `-mllvm -disable-post-ra` | 1224 | 2329 | 4308 | |
| `-mllvm -ifcvt-limit=0` | 1279 | 3417 | 4250 | |
| `-mllvm -inline-threshold=225` | 1350 | 3385 | 3419 | |

**56 full-link variants are exactly 4500 / 4500 / 4524 with `changed=0`**
(code-neutral on this source); a 57th, `-mllvm -forward-switch-cond`, is also
4500 / 4500 / 4524 but changes one function's linked bytes. The code-neutral
set includes `-jump-table-density=0`, `-old-thumb2-ifcvt`,
`-enable-if-conversion`, `-disable-early-ifcvt`, `-tail-dup-placement`,
`-tail-dup-placement-threshold=0`, `-tail-dup-placement-penalty=0`,
`-block-placement-exit-block-bias=0`, `-machine-sink-split`,
`-machine-sink-bfi`, `-sink-freq-percent-threshold=0`,
`-max-uses-for-sinking=0`, `-enable-shrink-wrap`, `-disable-spill-fusing`,
`-disable-spill-hoist`, `-post-RA-scheduler`, `-enable-post-misched`,
`-disable-sched-critical-path`, `-disable-sched-live-uses`,
`-disable-sched-stalls`, `-disable-sched-reg-pressure`,
`-disable-sched-height`, `-disable-sched-vrcycle`,
`-tail-dup-placement-aggressive-threshold=0`, `-tail-dup-indirect-size=0`,
`-arm-adjust-jump-tables`, `-arm-promote-constant-max-size=0`,
`-global-merge-max-offset=0`, `-enable-global-merge`, `-arm-global-merge`,
`-align-all-blocks=0`, `-align-all-nofallthru-blocks=0`,
`-regalloc-csr-first-time-cost=0`, `-max-sched-reorder=0`, `-misched-limit=0`,
`-sched-avg-ipc=0`, `-sched-high-latency-cycles=0`,
`-dfa-sched-reg-pressure-threshold=0`, `-loop-unswitch-threshold=0`,
`-unswitch-threshold=0`, `-licm-max-num-uses-traversed=0`,
`-min-predictable-branch=0`, `-cold-branch-ratio=0`,
`-guards-predicate-pass-branch-weight=0`, `-spec-exec-max-not-hoisted=0`,
`-ssc-dce-limit=0`, `-spp-rematerialization-threshold=0`,
`-machine-combiner-inc-threshold=0`, `-max-num-inline-blocks=0`,
`-max-counter-promotions-per-loop=0`, `-gvn-hoist-max-depth=0`,
`-gvn-hoist-max-bbs=0`, `-gvn-max-hoisted=0`, `-rewrite-phi-limit=0`,
`-arm-promote-constant-max-total=0`, `-align-all-functions=0`.

### 4.3 Necessary-condition proof for `byte_exact`

For a flag to raise `byte_exact` above 4500, it must make at least one more
non-exact target function exact than the number of exact target functions it
disturbs. The object screen gives `changed_exact` (exact target-TU functions
whose object bytes change) and `changed_targets` (non-exact target functions
whose object bytes change). A necessary condition is therefore

```text
changed_targets >= changed_exact + 1
```

Only **34 of the 231** changed flags satisfy it:

```text
0/4  -likely-branch-weight=5 -unlikely-branch-weight=1
0/4  -memdep-block-number-limit=16
0/2  -addr-sink-new-phis
0/2  -likely-branch-weight=10/100/20/64/7/80 -unlikely-branch-weight=1
0/1  -enable-dse-partial-overwrite-tracking=false
0/1  -forward-switch-cond
0/1  -likely-branch-weight=40/200/4000/500/1000 -unlikely-branch-weight=1
1/3  -extra-vectorizer-passes
2/4  -likely-branch-weight=3 -unlikely-branch-weight=1
2/3  -no-phi-elim-live-out-early-exit
3/5  -likely-branch-weight=1 -unlikely-branch-weight=1
3/5  -likely-branch-weight=1 -unlikely-branch-weight=2
3/4  -likely-branch-weight=2 -unlikely-branch-weight=1
3/4  -likely-branch-weight=20 -unlikely-branch-weight=12
4/5  -align-all-functions=16
4/5  -likely-branch-weight=0 -unlikely-branch-weight=1
4/5  -likely-branch-weight=1 -unlikely-branch-weight=10/20/2000/5/80
8/9  -machine-sink-split-probability-threshold=64/75/90
11/12 -machine-sink-split-probability-threshold=100
```

All 34 were full-linked. **None raised `byte_exact`, `linked_exact` or
`size-exact`; none produced a new exact function.** The other 197 flags have
`changed_targets <= changed_exact`, so even if every changed non-exact target
became exact they could at best break even in the target TUs; global effects
outside the target TUs can only add losses. This is why the 104 changed flags
that were not full-linked cannot be strict improvements. Their target-fuzzy
deltas were not measured (the object screen proves they change target-TU code),
but the gate question is settled by the condition above.

### 4.4 Target fuzzy

17 target-fuzzy cells improve; all are **+0.1** and all cost gate counts:

| flag | target | base -> variant | gate (byte/linked/size) |
|---|---|---|---|
| `-mllvm -condsider-local-interval-cost` | `LevelScript::process` | 53.4 -> 53.5 | 1765/4476/4506 |
| `-mllvm -disable-ifcvt-triangle-rev` | `Level::createShip` | 66.4 -> 66.5 | 2290/4463/4510 |
| `-mllvm -disable-machine-dce` | `Hud::draw` | 92.9 -> 93.0 | 1402/4367/4460 |
| `-mllvm -ifcvt-limit=0` | `Level::createShip` | 66.4 -> 66.5 | 1279/3417/4250 |
| `-mllvm -likely-branch-weight=10 -unlikely-branch-weight=1` | `SpaceLounge::drawLounge` | 95.5 -> 95.6 | 3154/4494/4521 |
| `-mllvm -likely-branch-weight=20 -unlikely-branch-weight=12` | `SpaceLounge::drawLounge` | 95.5 -> 95.6 | 2366/4491/4517 |
| `-mllvm -likely-branch-weight=3 -unlikely-branch-weight=1` | `SpaceLounge::drawLounge` | 95.5 -> 95.6 | 3052/4493/4518 |
| `-mllvm -likely-branch-weight=7 -unlikely-branch-weight=1` | `SpaceLounge::drawLounge` | 95.5 -> 95.6 | 3635/4496/4521 |
| `-mllvm -machine-sink-split-probability-threshold=64/75/90/100` | `Layout::Layout` C1/C2 | 47.8 -> 47.9 | 2036/4476/4506 (64/75/90), 2036/4473/4503 (100) |
| `-mllvm -t2-reduce-limit2=0` | `LevelScript::process` | 53.4 -> 53.5 | 1210/3124/4097 |

No target reached 100.0. The gate-neutral `-forward-switch-cond` leaves
`LevelScript::process` at 53.4 (it changes the linked bytes but not the
normalized instruction sequence).

### 4.5 Branch-weight detail

The canonical build carries a per-file `StarMap.cpp` 7:1 branch-weight
override. Removing it (`GOF2_NO_PER_FILE_WEIGHTS=ON`) is **2404 / 4496 / 4523**,
so the override is load-bearing for 2,096 `byte_exact` functions through
address/layout shifts. Global branch-weight values with the override removed:

| likely:unlikely | byte | linked | size |
|---|---:|---:|---:|
| none (no-per-file baseline) | 2404 | 4496 | 4523 |
| 40:1 | 3998 | 4498 | 4522 |
| 64:1 | 3998 | 4498 | 4522 |
| 80:1 | 3998 | 4498 | 4522 |
| 100:1 | 3998 | 4498 | 4522 |
| 7:1 | 3635 | 4496 | 4521 |
| 20:1 | 3454 | 4494 | 4520 |
| 10:1 | 3154 | 4494 | 4521 |
| 3:1 | 3052 | 4493 | 4518 |
| 2:1 | 3371 | 4492 | 4517 |
| 1:1 | 2338 | 4489 | 4518 |
| 1000:1 | 2404 | 4496 | 4523 |

None reaches the canonical 4500. The global branch-weight defaults are
therefore not a missing original flag; the per-file StarMap override is a
matching knob, not a global flag.

## 5. What is proven, disproven, and unobservable

### Proven

1. Baseline gate is 4500 / 4500 / 4524, data 843/843, imports 207, lint
   `CLEAN ✓`, drift 0; `.text` and all data sections are size/base-exact.
2. `clang++ -mllvm -help-hidden` in NDK r18b clang 7.0.2 exposes 823 options
   (535 bool, 288 value); the 673-line bool list and 537-line value list are a
   reproducible enumeration.
3. 231 distinct `-mllvm` flags change at least one function in the 16 target
   TUs (78 bool exact-changers, 137 value exact-changers, etc.).
4. No full-link variant among 189 distinct flags (186 built) beats
   4500 / 4500 / 4524; no variant creates a new exact function.
5. `-mllvm -forward-switch-cond` is gate-neutral (4500/4500/4524) and
   fuzzy-neutral; it changes `LevelScript::process` linked bytes but not its
   normalized score.
6. 56 other full-link flags are code-neutral no-ops at `-Oz` on this source.
7. PostRA machine sinking, machine sinking, tail duplication, block placement,
   the register allocator choice, the pre-RA scheduler, machine scheduling and
   if-conversion are all load-bearing: disabling or changing them drops the
   gate by hundreds to thousands of functions.
8. Global branch-weight values are unobservable on matched code; the
   `StarMap.cpp` 7:1 per-file override is load-bearing for `byte_exact` through
   layout shifts.
9. 17 target-fuzzy cells improve by +0.1, all with gate losses; no target
   becomes exact.

### Disproven

1. A missing global `-mllvm` flag is not the explanation for the 24
   non-byte-exact functions: every non-neutral flag lowers the gate.
2. `-mllvm -disable-postra-machine-sink`, `-disable-machine-sink`,
   `-disable-tail-duplicate`, `-disable-block-placement`, `-regalloc=fast`,
   `-pre-RA-sched=source`, `-enable-misched`, `-disable-post-ra`,
   `-ifcvt-limit=0` and `-inline-threshold=225` are not the original settings.
3. `-mllvm -likely-branch-weight=*` is not a global original flag; the
   canonical per-file StarMap override is a compensation, and global values
   with the override removed are worse.
4. `-mllvm -enable-machine-outliner` and `-mllvm -global-isel` crash clang
   7.0.2 on this source; `-arm-constant-island-max-iteration=0/1` and
   `-dag-maps-huge-region=0` also crash.

### Unobservable / unknown

1. The original build command line is not recoverable from the stripped `.so`.
   A flag that is code-neutral on this source cannot be excluded as *used but
   neutral*; there is simply no positive gate evidence for it. The 56 neutral
   full-link flags are in this class.
2. The exact original global branch-weight defaults are unobservable on
   matched code; only the StarMap per-file override is witnessed by layout.
3. `-mllvm` flags that only affect debug info, diagnostics, or pass-manager
   printing are outside the codegen question and were not treated as candidates.
4. The `.ARM.extab` content difference is address/relocation-driven; this note
   measures it but does not attribute it to any flag.

## 6. Confidence table

| claim | confidence | hard evidence |
|---|---|---|
| Baseline is 4500/4500/4524, lint CLEAN, drift 0 | **certain** | `check` + `drift` output in §1; fresh `cmake-build-match/verify/report.json` |
| `-help-hidden` has 823 options (535 bool / 288 value) | **certain** | parsed 1,035-line output; §2 |
| 231 distinct flags change target-TU code | **high** | object-byte A/B on all 16 target TUs; Appendix A.1 |
| No full-link variant beats 4500/4500/4524 | **high** | 186 official-verifier full-link rows (189 distinct, 3 build failures); Appendix A.2; no `new_exact` |
| No strict improvement among all 231 changed flags | **high** | necessary-condition filter (34 flags) + all 34 full-linked; §4.3 |
| `-forward-switch-cond` is gate-neutral | **certain** | 4500/4500/4524 full-link; target score 53.4 unchanged |
| 56 flags are code-neutral no-ops | **certain** | 4500/4500/4524 and `changed=0` in full link |
| PostRA sink / machine sink / block placement / regalloc are load-bearing | **certain** | full-link rows in §4.2 and §5 |
| Global branch weights are unobservable; StarMap 7:1 is load-bearing | **high** | no-per-file baseline 2404/4496/4523; §4.5 |
| The 24 remaining functions are source/TU-shape driven | **medium-high** | no flag raises any target; all 24 size-exact; prior notes agree |
| A code-neutral flag was not used by the original | **low / unobservable** | no positive evidence either way |

## 7. Strict improvement verdict

**No strict improvement exists in the tested `-mllvm` space at this tree
state.** The canonical build remains:

```text
byte_exact 4500/4524  linked_exact 4500/4524  size-exact 4524/4524
data_exact 843/843  imports 207  lint CLEAN  drift 0
```

No build or source change is landed. The only code-changing gate-neutral flag,
`-mllvm -forward-switch-cond`, is not an improvement (it leaves every gate count
and every target fuzzy value unchanged), so it is not added to the build.

## Appendix A: full tables

### A.1 Object-screen flags that changed at least one target-TU function

| list | flag | built/16 | changed exact | changed target | TUs |
|---|---|---:|---:|---:|---:|
| bool | `-mllvm -addr-sink-new-phis` | 16 | 0 | 2 | 2 |
| value | `-mllvm -align-all-blocks=16` | 4 | 73 | 5 | 4 |
| value | `-mllvm -align-all-blocks=2` | 16 | 460 | 22 | 16 |
| value | `-mllvm -align-all-blocks=4` | 16 | 483 | 22 | 16 |
| value | `-mllvm -align-all-blocks=8` | 16 | 483 | 22 | 16 |
| value | `-mllvm -align-all-functions=16` | 16 | 4 | 5 | 8 |
| value | `-mllvm -align-all-functions=8` | 16 | 1 | 0 | 1 |
| value | `-mllvm -align-all-nofallthru-blocks=16` | 12 | 300 | 17 | 12 |
| value | `-mllvm -align-all-nofallthru-blocks=2` | 16 | 306 | 22 | 16 |
| value | `-mllvm -align-all-nofallthru-blocks=4` | 16 | 397 | 22 | 16 |
| value | `-mllvm -align-all-nofallthru-blocks=8` | 16 | 416 | 22 | 16 |
| bool | `-mllvm -arm-assume-misaligned-load-store` | 16 | 240 | 21 | 16 |
| value | `-mllvm -arm-constant-island-max-iteration=1` | 2 | 3 | 2 | 2 |
| value | `-mllvm -arm-constant-island-max-iteration=2` | 16 | 2 | 0 | 2 |
| bool | `-mllvm -arm-promote-constant` | 16 | 65 | 11 | 14 |
| value | `-mllvm -available-load-scan-limit=0` | 16 | 8 | 4 | 9 |
| value | `-mllvm -available-load-scan-limit=1` | 16 | 12 | 7 | 11 |
| value | `-mllvm -available-load-scan-limit=16` | 16 | 7 | 5 | 7 |
| value | `-mllvm -available-load-scan-limit=2` | 16 | 11 | 7 | 11 |
| value | `-mllvm -available-load-scan-limit=4` | 16 | 7 | 4 | 8 |
| value | `-mllvm -available-load-scan-limit=8` | 16 | 5 | 4 | 7 |
| value | `-mllvm -block-placement-exit-block-bias=1` | 16 | 2 | 0 | 2 |
| value | `-mllvm -block-placement-exit-block-bias=10` | 16 | 10 | 3 | 10 |
| value | `-mllvm -block-placement-exit-block-bias=100` | 16 | 19 | 5 | 11 |
| value | `-mllvm -block-placement-exit-block-bias=20` | 16 | 10 | 3 | 10 |
| value | `-mllvm -block-placement-exit-block-bias=5` | 16 | 10 | 2 | 9 |
| value | `-mllvm -block-placement-exit-block-bias=50` | 16 | 13 | 3 | 11 |
| value | `-mllvm -break-anti-dependencies=all` | 16 | 258 | 22 | 16 |
| value | `-mllvm -break-anti-dependencies=critical` | 16 | 24 | 8 | 11 |
| bool | `-mllvm -combiner-global-alias-analysis` | 16 | 156 | 19 | 16 |
| bool | `-mllvm -condsider-local-interval-cost` | 16 | 10 | 2 | 8 |
| bool | `-mllvm -disable-2addr-hack=false` | 16 | 3 | 0 | 2 |
| bool | `-mllvm -disable-basicaa` | 16 | 187 | 20 | 16 |
| bool | `-mllvm -disable-block-placement` | 16 | 314 | 22 | 16 |
| bool | `-mllvm -disable-branch-fold` | 16 | 57 | 7 | 15 |
| bool | `-mllvm -disable-cgp` | 16 | 427 | 22 | 16 |
| bool | `-mllvm -disable-complex-addr-modes` | 16 | 9 | 7 | 9 |
| bool | `-mllvm -disable-constant-hoisting` | 16 | 10 | 5 | 8 |
| bool | `-mllvm -disable-copyprop` | 16 | 355 | 19 | 16 |
| bool | `-mllvm -disable-early-taildup` | 16 | 4 | 2 | 6 |
| bool | `-mllvm -disable-ifcvt-diamond` | 16 | 36 | 8 | 15 |
| bool | `-mllvm -disable-ifcvt-simple` | 16 | 19 | 1 | 9 |
| bool | `-mllvm -disable-ifcvt-simple-false` | 16 | 151 | 17 | 16 |
| bool | `-mllvm -disable-ifcvt-triangle` | 16 | 2 | 1 | 3 |
| bool | `-mllvm -disable-ifcvt-triangle-false` | 16 | 76 | 16 | 16 |
| bool | `-mllvm -disable-ifcvt-triangle-rev` | 16 | 15 | 5 | 10 |
| bool | `-mllvm -disable-lftr` | 16 | 29 | 7 | 12 |
| bool | `-mllvm -disable-licm-promotion` | 16 | 1 | 0 | 1 |
| bool | `-mllvm -disable-lsr` | 16 | 93 | 10 | 16 |
| bool | `-mllvm -disable-machine-cse` | 16 | 268 | 22 | 16 |
| bool | `-mllvm -disable-machine-dce` | 16 | 23 | 7 | 12 |
| bool | `-mllvm -disable-machine-licm` | 16 | 119 | 15 | 16 |
| bool | `-mllvm -disable-machine-sink` | 16 | 202 | 21 | 16 |
| bool | `-mllvm -disable-peephole` | 16 | 195 | 22 | 16 |
| bool | `-mllvm -disable-phi-elim-edge-splitting` | 16 | 28 | 11 | 15 |
| bool | `-mllvm -disable-post-ra` | 16 | 449 | 24 | 16 |
| bool | `-mllvm -disable-postra-machine-licm` | 16 | 7 | 4 | 6 |
| bool | `-mllvm -disable-postra-machine-sink` | 16 | 32 | 12 | 13 |
| bool | `-mllvm -disable-preheader-prot` | 16 | 16 | 2 | 8 |
| bool | `-mllvm -disable-sched-cycles` | 16 | 264 | 23 | 16 |
| bool | `-mllvm -disable-sched-hazard` | 16 | 272 | 23 | 16 |
| bool | `-mllvm -disable-sched-physreg-join` | 16 | 5 | 2 | 3 |
| bool | `-mllvm -disable-shifter-op` | 16 | 139 | 18 | 16 |
| bool | `-mllvm -disable-ssc` | 16 | 76 | 22 | 16 |
| bool | `-mllvm -disable-tail-duplicate` | 16 | 12 | 0 | 6 |
| bool | `-mllvm -early-live-intervals` | 12 | 8 | 6 | 7 |
| bool | `-mllvm -enable-aa-sched-mi` | 16 | 110 | 19 | 15 |
| bool | `-mllvm -enable-deferred-spilling` | 16 | 61 | 18 | 16 |
| bool | `-mllvm -enable-dse-partial-overwrite-tracking=false` | 16 | 0 | 1 | 1 |
| bool | `-mllvm -enable-earlycse-memssa=false` | 16 | 9 | 7 | 9 |
| bool | `-mllvm -enable-gvn-hoist` | 16 | 40 | 11 | 15 |
| bool | `-mllvm -enable-gvn-sink` | 16 | 63 | 15 | 15 |
| bool | `-mllvm -enable-ipra` | 16 | 76 | 6 | 16 |
| bool | `-mllvm -enable-load-pre=false` | 16 | 104 | 15 | 15 |
| bool | `-mllvm -enable-misched` | 16 | 462 | 24 | 16 |
| bool | `-mllvm -enable-newgvn` | 16 | 125 | 16 | 16 |
| bool | `-mllvm -enable-pre=false` | 16 | 115 | 15 | 15 |
| bool | `-mllvm -enable-selectiondag-sp=false` | 16 | 147 | 17 | 16 |
| bool | `-mllvm -enable-shrink-wrap=false` | 16 | 3 | 0 | 3 |
| bool | `-mllvm -enable-tail-merge=false` | 16 | 170 | 22 | 16 |
| bool | `-mllvm -enable-tbaa=false` | 16 | 45 | 12 | 15 |
| bool | `-mllvm -extra-vectorizer-passes` | 16 | 1 | 3 | 3 |
| bool | `-mllvm -fast-isel` | 16 | 176 | 19 | 16 |
| bool | `-mllvm -force-loop-cold-block` | 16 | 9 | 5 | 9 |
| bool | `-mllvm -force-precise-rotation-cost` | 16 | 127 | 14 | 16 |
| bool | `-mllvm -forward-switch-cond` | 16 | 0 | 1 | 1 |
| bool | `-mllvm -generate-type-units` | 16 | 17 | 6 | 11 |
| bool | `-mllvm -global-merge-on-const` | 16 | 12 | 5 | 6 |
| bool | `-mllvm -hoist-cheap-insts` | 16 | 7 | 1 | 6 |
| value | `-mllvm -ifcvt-limit=0` | 16 | 228 | 22 | 16 |
| value | `-mllvm -inline-threshold=0` | 16 | 3 | 1 | 2 |
| value | `-mllvm -inline-threshold=100` | 16 | 161 | 13 | 16 |
| value | `-mllvm -inline-threshold=1000` | 16 | 196 | 15 | 16 |
| value | `-mllvm -inline-threshold=150` | 16 | 168 | 13 | 16 |
| value | `-mllvm -inline-threshold=225` | 16 | 177 | 13 | 16 |
| value | `-mllvm -inline-threshold=25` | 16 | 146 | 13 | 16 |
| value | `-mllvm -inline-threshold=300` | 16 | 179 | 13 | 16 |
| value | `-mllvm -inline-threshold=50` | 16 | 148 | 13 | 16 |
| value | `-mllvm -inline-threshold=500` | 16 | 184 | 13 | 16 |
| value | `-mllvm -inline-threshold=75` | 16 | 155 | 13 | 16 |
| bool | `-mllvm -join-globalcopies` | 16 | 4 | 1 | 5 |
| bool | `-mllvm -join-splitedges` | 16 | 5 | 2 | 5 |
| bool | `-mllvm -jump-is-expensive` | 16 | 54 | 15 | 15 |
| bool | `-mllvm -keep-loops` | 16 | 2 | 0 | 2 |
| value | `-mllvm -likely-branch-weight=0 -mllvm -unlikely-branch-weight=1` | 16 | 4 | 5 | 7 |
| value | `-mllvm -likely-branch-weight=1 -mllvm -unlikely-branch-weight=1` | 16 | 3 | 5 | 6 |
| value | `-mllvm -likely-branch-weight=1 -mllvm -unlikely-branch-weight=10` | 16 | 4 | 5 | 7 |
| value | `-mllvm -likely-branch-weight=1 -mllvm -unlikely-branch-weight=2` | 16 | 3 | 5 | 6 |
| value | `-mllvm -likely-branch-weight=1 -mllvm -unlikely-branch-weight=20` | 16 | 4 | 5 | 7 |
| value | `-mllvm -likely-branch-weight=1 -mllvm -unlikely-branch-weight=2000` | 16 | 4 | 5 | 7 |
| value | `-mllvm -likely-branch-weight=1 -mllvm -unlikely-branch-weight=5` | 16 | 4 | 5 | 7 |
| value | `-mllvm -likely-branch-weight=1 -mllvm -unlikely-branch-weight=80` | 16 | 4 | 5 | 7 |
| value | `-mllvm -likely-branch-weight=10 -mllvm -unlikely-branch-weight=1` | 16 | 0 | 2 | 2 |
| value | `-mllvm -likely-branch-weight=100 -mllvm -unlikely-branch-weight=1` | 16 | 0 | 2 | 2 |
| value | `-mllvm -likely-branch-weight=1000 -mllvm -unlikely-branch-weight=1` | 16 | 0 | 1 | 1 |
| value | `-mllvm -likely-branch-weight=2 -mllvm -unlikely-branch-weight=1` | 16 | 3 | 4 | 6 |
| value | `-mllvm -likely-branch-weight=20 -mllvm -unlikely-branch-weight=1` | 16 | 0 | 2 | 2 |
| value | `-mllvm -likely-branch-weight=20 -mllvm -unlikely-branch-weight=12` | 16 | 3 | 4 | 6 |
| value | `-mllvm -likely-branch-weight=200 -mllvm -unlikely-branch-weight=1` | 16 | 0 | 1 | 1 |
| value | `-mllvm -likely-branch-weight=3 -mllvm -unlikely-branch-weight=1` | 16 | 2 | 4 | 5 |
| value | `-mllvm -likely-branch-weight=40 -mllvm -unlikely-branch-weight=1` | 16 | 0 | 1 | 1 |
| value | `-mllvm -likely-branch-weight=4000 -mllvm -unlikely-branch-weight=1` | 16 | 0 | 1 | 1 |
| value | `-mllvm -likely-branch-weight=5 -mllvm -unlikely-branch-weight=1` | 16 | 0 | 4 | 4 |
| value | `-mllvm -likely-branch-weight=500 -mllvm -unlikely-branch-weight=1` | 16 | 0 | 1 | 1 |
| value | `-mllvm -likely-branch-weight=64 -mllvm -unlikely-branch-weight=1` | 16 | 0 | 2 | 2 |
| value | `-mllvm -likely-branch-weight=7 -mllvm -unlikely-branch-weight=1` | 16 | 0 | 2 | 2 |
| value | `-mllvm -likely-branch-weight=80 -mllvm -unlikely-branch-weight=1` | 16 | 0 | 2 | 2 |
| bool | `-mllvm -lsr-exp-narrow` | 16 | 3 | 3 | 4 |
| bool | `-mllvm -lsr-insns-cost` | 16 | 9 | 0 | 4 |
| value | `-mllvm -machine-sink-split-probability-threshold=0` | 16 | 11 | 1 | 6 |
| value | `-mllvm -machine-sink-split-probability-threshold=1` | 16 | 11 | 1 | 6 |
| value | `-mllvm -machine-sink-split-probability-threshold=10` | 16 | 10 | 1 | 5 |
| value | `-mllvm -machine-sink-split-probability-threshold=100` | 16 | 11 | 12 | 13 |
| value | `-mllvm -machine-sink-split-probability-threshold=25` | 16 | 10 | 1 | 5 |
| value | `-mllvm -machine-sink-split-probability-threshold=50` | 16 | 7 | 6 | 10 |
| value | `-mllvm -machine-sink-split-probability-threshold=64` | 16 | 8 | 9 | 11 |
| value | `-mllvm -machine-sink-split-probability-threshold=75` | 16 | 8 | 9 | 11 |
| value | `-mllvm -machine-sink-split-probability-threshold=90` | 16 | 8 | 9 | 11 |
| value | `-mllvm -memdep-block-number-limit=0` | 16 | 109 | 15 | 15 |
| value | `-mllvm -memdep-block-number-limit=1` | 16 | 106 | 15 | 15 |
| value | `-mllvm -memdep-block-number-limit=16` | 16 | 0 | 4 | 3 |
| value | `-mllvm -memdep-block-number-limit=2` | 16 | 80 | 11 | 15 |
| value | `-mllvm -memdep-block-number-limit=4` | 16 | 25 | 10 | 14 |
| value | `-mllvm -memdep-block-number-limit=8` | 16 | 10 | 6 | 7 |
| value | `-mllvm -memdep-block-scan-limit=1` | 16 | 120 | 15 | 15 |
| value | `-mllvm -memdep-block-scan-limit=16` | 16 | 9 | 8 | 8 |
| value | `-mllvm -memdep-block-scan-limit=2` | 16 | 120 | 15 | 15 |
| value | `-mllvm -memdep-block-scan-limit=4` | 16 | 101 | 15 | 15 |
| value | `-mllvm -memdep-block-scan-limit=8` | 16 | 33 | 11 | 14 |
| value | `-mllvm -min-jump-table-entries=0` | 16 | 61 | 15 | 16 |
| value | `-mllvm -min-jump-table-entries=1` | 16 | 61 | 15 | 16 |
| value | `-mllvm -min-jump-table-entries=16` | 16 | 25 | 8 | 13 |
| value | `-mllvm -min-jump-table-entries=2` | 16 | 38 | 9 | 14 |
| value | `-mllvm -min-jump-table-entries=8` | 16 | 20 | 6 | 12 |
| bool | `-mllvm -misched-postra` | 16 | 128 | 23 | 16 |
| bool | `-mllvm -no-phi-elim-live-out-early-exit` | 16 | 2 | 3 | 4 |
| bool | `-mllvm -no-stack-coloring` | 16 | 91 | 15 | 15 |
| bool | `-mllvm -no-stack-slot-sharing` | 16 | 39 | 14 | 14 |
| value | `-mllvm -optsize-jump-table-density=0` | 16 | 22 | 8 | 12 |
| value | `-mllvm -optsize-jump-table-density=1` | 16 | 20 | 7 | 10 |
| value | `-mllvm -optsize-jump-table-density=10` | 16 | 14 | 4 | 10 |
| value | `-mllvm -optsize-jump-table-density=100` | 16 | 17 | 5 | 8 |
| value | `-mllvm -optsize-jump-table-density=2` | 16 | 20 | 7 | 10 |
| value | `-mllvm -optsize-jump-table-density=20` | 16 | 10 | 4 | 9 |
| value | `-mllvm -optsize-jump-table-density=5` | 16 | 17 | 5 | 10 |
| value | `-mllvm -optsize-jump-table-density=80` | 16 | 13 | 4 | 8 |
| bool | `-mllvm -phi-elim-split-all-critical-edges` | 16 | 17 | 7 | 14 |
| value | `-mllvm -pre-RA-sched=list-burr` | 16 | 371 | 24 | 16 |
| value | `-mllvm -pre-RA-sched=list-ilp` | 16 | 363 | 24 | 16 |
| value | `-mllvm -pre-RA-sched=source` | 16 | 419 | 24 | 16 |
| bool | `-mllvm -profile-generate` | 16 | 877 | 24 | 16 |
| value | `-mllvm -regalloc-csr-first-time-cost=1` | 16 | 3 | 0 | 2 |
| value | `-mllvm -regalloc-csr-first-time-cost=16` | 16 | 7 | 0 | 4 |
| value | `-mllvm -regalloc-csr-first-time-cost=2` | 16 | 5 | 0 | 3 |
| value | `-mllvm -regalloc-csr-first-time-cost=3` | 16 | 5 | 0 | 3 |
| value | `-mllvm -regalloc-csr-first-time-cost=4` | 16 | 6 | 0 | 4 |
| value | `-mllvm -regalloc-csr-first-time-cost=8` | 16 | 6 | 0 | 4 |
| value | `-mllvm -regalloc=basic` | 16 | 540 | 24 | 16 |
| value | `-mllvm -regalloc=fast` | 16 | 605 | 24 | 16 |
| value | `-mllvm -regalloc=pbqp` | 16 | 569 | 24 | 16 |
| bool | `-mllvm -simplifycfg-dup-ret` | 16 | 70 | 5 | 14 |
| bool | `-mllvm -sink-common-insts` | 16 | 23 | 7 | 11 |
| bool | `-mllvm -sink-insts-to-avoid-spills` | 16 | 48 | 12 | 15 |
| value | `-mllvm -slp-schedule-budget=0` | 16 | 24 | 11 | 12 |
| value | `-mllvm -slp-schedule-budget=1` | 16 | 24 | 11 | 12 |
| value | `-mllvm -slp-schedule-budget=16` | 16 | 3 | 2 | 3 |
| value | `-mllvm -slp-schedule-budget=2` | 16 | 22 | 11 | 12 |
| value | `-mllvm -slp-schedule-budget=4` | 16 | 19 | 11 | 12 |
| value | `-mllvm -slp-schedule-budget=8` | 16 | 19 | 9 | 11 |
| value | `-mllvm -split-spill-mode=default` | 16 | 67 | 19 | 16 |
| value | `-mllvm -split-spill-mode=size` | 16 | 36 | 16 | 14 |
| value | `-mllvm -stress-regalloc=12` | 16 | 164 | 22 | 16 |
| value | `-mllvm -stress-regalloc=16` | 16 | 67 | 17 | 16 |
| value | `-mllvm -stress-regalloc=4` | 6 | 138 | 7 | 6 |
| value | `-mllvm -stress-regalloc=8` | 16 | 317 | 24 | 16 |
| bool | `-mllvm -switch-to-lookup` | 16 | 2 | 2 | 3 |
| value | `-mllvm -t2-reduce-limit2=0` | 16 | 296 | 22 | 16 |
| value | `-mllvm -t2-reduce-limit3=0` | 16 | 612 | 24 | 16 |
| value | `-mllvm -t2-reduce-limit=0` | 16 | 641 | 24 | 16 |
| value | `-mllvm -tail-dup-limit=0` | 16 | 16 | 2 | 11 |
| value | `-mllvm -tail-dup-placement-penalty=100` | 16 | 2 | 0 | 2 |
| value | `-mllvm -tail-dup-placement-penalty=50` | 16 | 1 | 0 | 1 |
| value | `-mllvm -tail-dup-size=0` | 16 | 16 | 2 | 11 |
| value | `-mllvm -tail-dup-size=16` | 16 | 85 | 13 | 16 |
| value | `-mllvm -tail-dup-size=2` | 16 | 39 | 10 | 12 |
| value | `-mllvm -tail-dup-size=32` | 16 | 88 | 13 | 16 |
| value | `-mllvm -tail-dup-size=4` | 16 | 71 | 11 | 15 |
| value | `-mllvm -tail-dup-size=64` | 16 | 90 | 14 | 16 |
| value | `-mllvm -tail-dup-size=8` | 16 | 83 | 13 | 16 |
| value | `-mllvm -tail-merge-size=0` | 16 | 27 | 6 | 13 |
| value | `-mllvm -tail-merge-size=1` | 16 | 27 | 6 | 13 |
| value | `-mllvm -tail-merge-size=16` | 16 | 31 | 8 | 16 |
| value | `-mllvm -tail-merge-size=2` | 16 | 15 | 6 | 11 |
| value | `-mllvm -tail-merge-size=32` | 16 | 31 | 8 | 16 |
| value | `-mllvm -tail-merge-size=4` | 16 | 14 | 3 | 10 |
| value | `-mllvm -tail-merge-size=8` | 16 | 28 | 7 | 15 |
| value | `-mllvm -tail-merge-threshold=0` | 16 | 170 | 22 | 16 |
| value | `-mllvm -tail-merge-threshold=1` | 16 | 170 | 22 | 16 |
| value | `-mllvm -tail-merge-threshold=16` | 16 | 15 | 6 | 11 |
| value | `-mllvm -tail-merge-threshold=2` | 16 | 129 | 22 | 16 |
| value | `-mllvm -tail-merge-threshold=32` | 16 | 5 | 1 | 5 |
| value | `-mllvm -tail-merge-threshold=4` | 16 | 64 | 19 | 16 |
| value | `-mllvm -tail-merge-threshold=8` | 16 | 29 | 10 | 14 |
| bool | `-mllvm -terminal-rule` | 16 | 8 | 1 | 5 |
| bool | `-mllvm -trap-unreachable` | 16 | 178 | 15 | 16 |
| value | `-mllvm -unroll-threshold=100` | 16 | 4 | 0 | 1 |
| value | `-mllvm -unroll-threshold=1000` | 16 | 4 | 0 | 1 |
| value | `-mllvm -unroll-threshold=150` | 16 | 4 | 0 | 1 |
| value | `-mllvm -unroll-threshold=200` | 16 | 4 | 0 | 1 |
| value | `-mllvm -unroll-threshold=300` | 16 | 4 | 0 | 1 |
| value | `-mllvm -unroll-threshold=500` | 16 | 4 | 0 | 1 |

231 rows.

### A.2 Full-link variants (186 built of 189 distinct; 3 build failures)

Gate counts are from the official verifier; `changed` is the number of linked functions whose bytes changed vs the canonical build; `new exact` is the number of functions that became byte-exact.

| flag | byte | linked | size | changed | new exact | target fuzzy wins |
|---|---:|---:|---:|---:|---:|---|
| `-mllvm -t2-reduce-limit3=0` | 838 | 1199 | 1216 | 3686 | 0 |  |
| `-mllvm -t2-reduce-limit=0` | 947 | 1583 | 1650 | 3577 | 0 |  |
| `-mllvm -regalloc=pbqp` | 957 | 1717 | 2559 | 3567 | 0 |  |
| `-mllvm -regalloc=basic` | 993 | 1899 | 2863 | 3531 | 0 |  |
| `-mllvm -regalloc=fast` | 1099 | 1592 | 1654 | 3425 | 0 |  |
| `-mllvm -t2-reduce-limit2=0` | 1210 | 3124 | 4097 | 3314 | 0 | 11LevelScript7proc +0.1 |
| `-mllvm -pre-RA-sched=source` | 1216 | 2574 | 3988 | 3308 | 0 |  |
| `-mllvm -enable-misched` | 1219 | 2417 | 3579 | 3305 | 0 |  |
| `-mllvm -disable-post-ra` | 1224 | 2329 | 4308 | 3300 | 0 |  |
| `-mllvm -arm-load-store-opt=false` | 1225 | 3311 | 3906 | 3299 | 0 |  |
| `-mllvm -disable-machine-sink` | 1235 | 3732 | 4019 | 3289 | 0 |  |
| `-mllvm -pre-RA-sched=list-burr` | 1256 | 2742 | 4089 | 3268 | 0 |  |
| `-mllvm -disable-block-placement` | 1262 | 3114 | 4119 | 3262 | 0 |  |
| `-mllvm -pre-RA-sched=list-ilp` | 1264 | 2823 | 3936 | 3260 | 0 |  |
| `-mllvm -ifcvt-limit=0` | 1279 | 3417 | 4250 | 3245 | 0 | 5Level10createShip +0.1 |
| `-mllvm -tail-merge-threshold=0` | 1324 | 3851 | 3875 | 3200 | 0 |  |
| `-mllvm -stress-regalloc=8` | 1341 | 3169 | 3297 | 3183 | 0 |  |
| `-mllvm -disable-machine-cse` | 1345 | 3353 | 3461 | 3179 | 0 |  |
| `-mllvm -align-all-functions=16` | 1345 | 2307 | 4517 | 3179 | 0 |  |
| `-mllvm -inline-threshold=225` | 1350 | 3385 | 3419 | 2947 | 0 |  |
| `-mllvm -tail-merge-size=0` | 1355 | 4403 | 4485 | 3169 | 0 |  |
| `-mllvm -inline-threshold=0` | 1356 | 4448 | 4498 | 3168 | 0 |  |
| `-mllvm -tail-merge-size=16` | 1364 | 4412 | 4455 | 3160 | 0 |  |
| `-mllvm -disable-lftr` | 1369 | 4381 | 4495 | 3155 | 0 |  |
| `-mllvm -disable-machine-licm` | 1374 | 4092 | 4182 | 3150 | 0 |  |
| `-mllvm -align-all-functions=8` | 1378 | 4489 | 4523 | 3146 | 0 |  |
| `-mllvm -tail-dup-size=16` | 1378 | 4239 | 4290 | 3146 | 0 |  |
| `-mllvm -disable-sched-cycles` | 1379 | 3395 | 4268 | 3145 | 0 |  |
| `-mllvm -disable-ifcvt-triangle-false` | 1383 | 4263 | 4461 | 3141 | 0 |  |
| `-mllvm -disable-sched-hazard` | 1383 | 3376 | 4249 | 3141 | 0 |  |
| `-mllvm -optsize-jump-table-density=20` | 1385 | 4474 | 4497 | 3139 | 0 |  |
| `-mllvm -arm-promote-constant` | 1386 | 4190 | 4221 | 3138 | 0 |  |
| `-mllvm -disable-phi-elim-edge-splitting` | 1387 | 4407 | 4463 | 3137 | 0 |  |
| `-mllvm -lsr-insns-cost` | 1402 | 4439 | 4483 | 3122 | 0 |  |
| `-mllvm -disable-machine-dce` | 1402 | 4367 | 4460 | 3122 | 0 | 3Hud4drawExxP9Play +0.1 |
| `-mllvm -enable-aa-sched-mi` | 1408 | 4019 | 4497 | 3116 | 0 |  |
| `-mllvm -memdep-block-number-limit=16` | 1409 | 4492 | 4519 | 3115 | 0 |  |
| `-mllvm -unroll-threshold=150` | 1413 | 4489 | 4516 | 3111 | 0 |  |
| `-mllvm -unroll-threshold=1000` | 1413 | 4486 | 4512 | 3111 | 0 |  |
| `-mllvm -unroll-threshold=300` | 1413 | 4486 | 4512 | 3111 | 0 |  |
| `-mllvm -unroll-threshold=500` | 1413 | 4486 | 4512 | 3111 | 0 |  |
| `-mllvm -slp-schedule-budget=16` | 1427 | 4479 | 4507 | 3097 | 0 |  |
| `-mllvm -disable-ifcvt-diamond` | 1428 | 4398 | 4459 | 3096 | 0 |  |
| `-mllvm -unroll-threshold=200` | 1429 | 4487 | 4514 | 3095 | 0 |  |
| `-mllvm -split-spill-mode=size` | 1437 | 4411 | 4446 | 3087 | 0 |  |
| `-mllvm -enable-shrink-wrap=false` | 1457 | 4460 | 4500 | 3067 | 0 |  |
| `-mllvm -memdep-block-number-limit=8` | 1458 | 4471 | 4501 | 3066 | 0 |  |
| `-mllvm -lsr-exp-narrow` | 1460 | 4482 | 4511 | 3064 | 0 |  |
| `-mllvm -extra-vectorizer-passes` | 1464 | 4491 | 4516 | 3060 | 0 |  |
| `-mllvm -disable-ifcvt-simple` | 1519 | 4364 | 4456 | 3005 | 0 |  |
| `-mllvm -disable-licm-promotion` | 1559 | 4493 | 4520 | 2965 | 0 |  |
| `-mllvm -disable-constant-hoisting` | 1566 | 4473 | 4504 | 2958 | 0 |  |
| `-mllvm -block-placement-exit-block-bias=100` | 1572 | 4433 | 4481 | 2952 | 0 |  |
| `-mllvm -disable-postra-machine-sink` | 1573 | 4392 | 4473 | 2951 | 0 |  |
| `-mllvm -misched-postra` | 1575 | 3980 | 4504 | 2949 | 0 |  |
| `-mllvm -slp-schedule-budget=0` | 1580 | 4390 | 4479 | 2944 | 0 |  |
| `-mllvm -enable-dse-partial-overwrite-tracking=false` | 1587 | 4485 | 4518 | 2937 | 0 |  |
| `-mllvm -disable-complex-addr-modes` | 1587 | 4481 | 4504 | 2937 | 0 |  |
| `-mllvm -addr-sink-new-phis` | 1595 | 4487 | 4514 | 2929 | 0 |  |
| `-mllvm -unroll-threshold=100` | 1598 | 4490 | 4517 | 2926 | 0 |  |
| `-mllvm -disable-sched-physreg-join` | 1633 | 4478 | 4515 | 2891 | 0 |  |
| `-mllvm -no-phi-elim-live-out-early-exit` | 1643 | 4494 | 4520 | 2881 | 0 |  |
| `-mllvm -block-placement-exit-block-bias=10` | 1729 | 4464 | 4506 | 2795 | 0 |  |
| `-mllvm -block-placement-exit-block-bias=20` | 1729 | 4464 | 4506 | 2795 | 0 |  |
| `-mllvm -block-placement-exit-block-bias=5` | 1734 | 4464 | 4506 | 2790 | 0 |  |
| `-mllvm -condsider-local-interval-cost` | 1765 | 4476 | 4506 | 2759 | 0 | 11LevelScript7proc +0.1 |
| `-mllvm -memdep-block-scan-limit=16` | 1796 | 4445 | 4484 | 2727 | 0 |  |
| `-mllvm -disable-2addr-hack=false` | 1802 | 4468 | 4516 | 2722 | 0 |  |
| `-mllvm -keep-loops` | 1840 | 4485 | 4514 | 2684 | 0 |  |
| `-mllvm -available-load-scan-limit=0` | 1844 | 4485 | 4513 | 2679 | 0 |  |
| `-mllvm -hoist-cheap-insts` | 1847 | 4487 | 4514 | 2677 | 0 |  |
| `-mllvm -tail-merge-threshold=32` | 1871 | 4489 | 4514 | 2653 | 0 |  |
| `-mllvm -enable-earlycse-memssa=false` | 1881 | 4466 | 4506 | 2643 | 0 |  |
| `-mllvm -regalloc-csr-first-time-cost=16` | 1890 | 4481 | 4510 | 2634 | 0 |  |
| `-mllvm -join-globalcopies` | 1893 | 4483 | 4512 | 2631 | 0 |  |
| `-mllvm -machine-sink-split-probability-threshold=10` | 1931 | 4476 | 4511 | 2593 | 0 |  |
| `-mllvm -disable-tail-duplicate` | 1962 | 4440 | 4501 | 2559 | 0 |  |
| `-mllvm -tail-dup-limit=0` | 1962 | 4427 | 4493 | 2559 | 0 |  |
| `-mllvm -regalloc-csr-first-time-cost=1` | 1967 | 4488 | 4516 | 2557 | 0 |  |
| `-mllvm -regalloc-csr-first-time-cost=4` | 1997 | 4484 | 4512 | 2527 | 0 |  |
| `-mllvm -regalloc-csr-first-time-cost=2` | 2027 | 4485 | 4513 | 2497 | 0 |  |
| `-mllvm -regalloc-csr-first-time-cost=3` | 2027 | 4485 | 4513 | 2497 | 0 |  |
| `-mllvm -machine-sink-split-probability-threshold=50` | 2027 | 4479 | 4509 | 2497 | 0 |  |
| `-mllvm -machine-sink-split-probability-threshold=64` | 2036 | 4476 | 4506 | 2488 | 0 | 6LayoutC1Ev +0.1, 6LayoutC2Ev +0.1 |
| `-mllvm -machine-sink-split-probability-threshold=75` | 2036 | 4476 | 4506 | 2488 | 0 | 6LayoutC1Ev +0.1, 6LayoutC2Ev +0.1 |
| `-mllvm -machine-sink-split-probability-threshold=90` | 2036 | 4476 | 4506 | 2488 | 0 | 6LayoutC1Ev +0.1, 6LayoutC2Ev +0.1 |
| `-mllvm -machine-sink-split-probability-threshold=100` | 2036 | 4473 | 4503 | 2488 | 0 | 6LayoutC1Ev +0.1, 6LayoutC2Ev +0.1 |
| `-mllvm -likely-branch-weight=1 -mllvm -unlikely-branch-weight=2` | 2045 | 4488 | 4516 | 2479 | 0 |  |
| `-mllvm -force-loop-cold-block` | 2052 | 4477 | 4504 | 2472 | 0 |  |
| `-mllvm -available-load-scan-limit=4` | 2060 | 4482 | 4516 | 2460 | 0 |  |
| `-mllvm -likely-branch-weight=5 -mllvm -unlikely-branch-weight=1` | 2072 | 4494 | 4519 | 2452 | 0 |  |
| `-mllvm -switch-to-lookup` | 2084 | 4493 | 4519 | 2440 | 0 |  |
| `-mllvm -likely-branch-weight=1 -mllvm -unlikely-branch-weight=2000` | 2095 | 4487 | 4511 | 2429 | 0 |  |
| `-mllvm -available-load-scan-limit=16` | 2140 | 4485 | 4510 | 2382 | 0 |  |
| `-mllvm -likely-branch-weight=1 -mllvm -unlikely-branch-weight=80` | 2179 | 4487 | 4511 | 2345 | 0 |  |
| `-mllvm -likely-branch-weight=1 -mllvm -unlikely-branch-weight=20` | 2281 | 4488 | 4512 | 2238 | 0 |  |
| `-mllvm -disable-ifcvt-triangle-rev` | 2290 | 4463 | 4510 | 2233 | 0 | 5Level10createShip +0.1 |
| `-mllvm -disable-postra-machine-licm` | 2292 | 4479 | 4520 | 2229 | 0 |  |
| `-mllvm -likely-branch-weight=1 -mllvm -unlikely-branch-weight=10` | 2325 | 4488 | 4511 | 2197 | 0 |  |
| `-mllvm -likely-branch-weight=1 -mllvm -unlikely-branch-weight=1` | 2338 | 4489 | 4518 | 2183 | 0 |  |
| `-mllvm -regalloc-csr-first-time-cost=8` | 2352 | 4483 | 4511 | 2170 | 0 |  |
| `-mllvm -likely-branch-weight=20 -mllvm -unlikely-branch-weight=12` | 2366 | 4491 | 4517 | 2155 | 0 | 11SpaceLounge10dra +0.1 |
| `-mllvm -likely-branch-weight=1 -mllvm -unlikely-branch-weight=5` | 2377 | 4487 | 4511 | 2145 | 0 |  |
| `` | 2404 | 4496 | 4523 | 2116 | 0 |  |
| `-mllvm -likely-branch-weight=1000 -mllvm -unlikely-branch-weight=1` | 2404 | 4496 | 4523 | 2116 | 0 |  |
| `-mllvm -likely-branch-weight=200 -mllvm -unlikely-branch-weight=1` | 2404 | 4496 | 4523 | 2116 | 0 |  |
| `-mllvm -likely-branch-weight=4000 -mllvm -unlikely-branch-weight=1` | 2404 | 4496 | 4523 | 2116 | 0 |  |
| `-mllvm -likely-branch-weight=500 -mllvm -unlikely-branch-weight=1` | 2404 | 4496 | 4523 | 2116 | 0 |  |
| `-mllvm -machine-sink-split-probability-threshold=25` | 2614 | 4479 | 4513 | 1910 | 0 |  |
| `-mllvm -tail-dup-placement-penalty=100` | 2654 | 4487 | 4517 | 1869 | 0 |  |
| `-mllvm -tail-dup-placement-penalty=50` | 2659 | 4491 | 4519 | 1864 | 0 |  |
| `-mllvm -enable-tail-merge` | 2674 | 4497 | 4523 | 1842 | 0 |  |
| `-mllvm -disable-early-taildup` | 2697 | 4483 | 4516 | 1819 | 0 |  |
| `-mllvm -available-load-scan-limit=8` | 2773 | 4488 | 4514 | 1747 | 0 |  |
| `-mllvm -likely-branch-weight=0 -mllvm -unlikely-branch-weight=1` | 2773 | 4487 | 4513 | 1746 | 0 |  |
| `-mllvm -join-splitedges` | 2962 | 4485 | 4510 | 1555 | 0 |  |
| `-mllvm -terminal-rule` | 3016 | 4477 | 4521 | 1506 | 0 |  |
| `-mllvm -block-placement-exit-block-bias=1` | 3017 | 4485 | 4517 | 1495 | 0 |  |
| `-mllvm -likely-branch-weight=3 -mllvm -unlikely-branch-weight=1` | 3052 | 4493 | 4518 | 1467 | 0 | 11SpaceLounge10dra +0.1 |
| `-mllvm -likely-branch-weight=10 -mllvm -unlikely-branch-weight=1` | 3154 | 4494 | 4521 | 1364 | 0 | 11SpaceLounge10dra +0.1 |
| `-mllvm -likely-branch-weight=2 -mllvm -unlikely-branch-weight=1` | 3371 | 4492 | 4517 | 1146 | 0 |  |
| `-mllvm -likely-branch-weight=20 -mllvm -unlikely-branch-weight=1` | 3454 | 4494 | 4520 | 1061 | 0 |  |
| `-mllvm -likely-branch-weight=7 -mllvm -unlikely-branch-weight=1` | 3635 | 4496 | 4521 | 874 | 0 | 11SpaceLounge10dra +0.1 |
| `-mllvm -likely-branch-weight=100 -mllvm -unlikely-branch-weight=1` | 3998 | 4498 | 4522 | 513 | 0 |  |
| `-mllvm -likely-branch-weight=40 -mllvm -unlikely-branch-weight=1` | 3998 | 4498 | 4522 | 512 | 0 |  |
| `-mllvm -likely-branch-weight=64 -mllvm -unlikely-branch-weight=1` | 3998 | 4498 | 4522 | 513 | 0 |  |
| `-mllvm -likely-branch-weight=80 -mllvm -unlikely-branch-weight=1` | 3998 | 4498 | 4522 | 513 | 0 |  |
| `-mllvm -disable-ifcvt-triangle` | 4220 | 4495 | 4521 | 285 | 0 |  |
| `-mllvm -arm-constant-island-max-iteration=2` | 4445 | 4498 | 4522 | 57 | 0 |  |
| `-mllvm -align-all-blocks=0` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -align-all-functions=0` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -align-all-nofallthru-blocks=0` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -arm-adjust-jump-tables` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -arm-global-merge` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -arm-promote-constant-max-size=0` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -arm-promote-constant-max-total=0` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -block-placement-exit-block-bias=0` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -cold-branch-ratio=0` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -dfa-sched-reg-pressure-threshold=0` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -disable-early-ifcvt` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -disable-sched-critical-path` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -disable-sched-height` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -disable-sched-live-uses` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -disable-sched-reg-pressure` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -disable-sched-stalls` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -disable-sched-vrcycle` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -disable-spill-fusing` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -disable-spill-hoist` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -enable-global-merge` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -enable-if-conversion` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -enable-post-misched` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -enable-shrink-wrap` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -forward-switch-cond` | 4500 | 4500 | 4524 | 1 | 0 |  |
| `-mllvm -global-merge-max-offset=0` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -guards-predicate-pass-branch-weight=0` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -gvn-hoist-max-bbs=0` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -gvn-hoist-max-depth=0` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -gvn-max-hoisted=0` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -jump-table-density=0` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -licm-max-num-uses-traversed=0` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -loop-unswitch-threshold=0` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -machine-combiner-inc-threshold=0` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -machine-sink-bfi` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -machine-sink-split` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -max-counter-promotions-per-loop=0` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -max-num-inline-blocks=0` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -max-sched-reorder=0` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -max-uses-for-sinking=0` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -min-predictable-branch=0` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -misched-limit=0` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -old-thumb2-ifcvt` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -post-RA-scheduler` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -regalloc-csr-first-time-cost=0` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -rewrite-phi-limit=0` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -sched-avg-ipc=0` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -sched-high-latency-cycles=0` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -sink-freq-percent-threshold=0` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -spec-exec-max-not-hoisted=0` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -spp-rematerialization-threshold=0` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -ssc-dce-limit=0` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -tail-dup-indirect-size=0` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -tail-dup-placement` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -tail-dup-placement-aggressive-threshold=0` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -tail-dup-placement-penalty=0` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -tail-dup-placement-threshold=0` | 4500 | 4500 | 4524 | 0 | 0 |  |
| `-mllvm -unswitch-threshold=0` | 4500 | 4500 | 4524 | 0 | 0 |  |

186 rows.


## Appendix B: reproduction scripts

### B.1 Candidate-list generation

```python
# bool_ed_mllvm.txt: every bool option bare, plus =false for enable-*/disable-*
import re
lines = open('help-hidden.txt').read().splitlines()
opts = []
for ln in lines:
    m = re.match(r'^  (-[A-Za-z0-9_.-]+)(=<[^>]+>)?\s{2,}(.*)$', ln)
    if m:
        opts.append((m.group(1), bool(m.group(2)), m.group(3).strip()))
boolopts = [o[0] for o in opts if not o[1]]
c = []
for x in boolopts:
    c.append('-mllvm ' + x)
    if x.startswith('-enable-') or x.startswith('-disable-'):
        c.append('-mllvm ' + x + '=false')
seen = set(); out = []
for x in c:
    if x not in seen:
        seen.add(x); out.append(x)
open('bool_ed_mllvm.txt', 'w').write('\n'.join(out) + '\n')
```

The value list is the 537-line targeted set described in §2; it is generated
by the same loop over a table of `(option, [values...])` pairs, plus the six
enum options (`pre-RA-sched`, `regalloc`, `misched`, `split-spill-mode`,
`arm-implicit-it`, `break-anti-dependencies`).

### B.2 Full-link driver (`/dev/shm/w5210/run_full.py`)

```python
import json, sys
from pathlib import Path
ROOT = Path('/home/debian/GalaxyOnFire2/wt/w5210')
sys.path.insert(0, str(ROOT / 'tools'))
import flag_fingerprint as ff

workdir = Path(sys.argv[1])
specs = json.load(open(sys.argv[2]))
workdir.mkdir(parents=True, exist_ok=True)
wrapper = ff.write_wrapper(workdir)
baseline_build = ROOT / 'cmake-build-match'
baseline_report = json.loads((baseline_build / 'verify' / 'report.json').read_text())
baseline_rows = baseline_report['rows']
base = {r['name']: r for r in baseline_rows}
watch = [n for n, r in base.items() if not r['byte']]
orig_sections = ff.section_digest(ff.ORIGINAL)
out = []
for i, s in enumerate(specs):
    s['watch'] = watch
    s['baseline_build'] = str(baseline_build)
    try:
        r = ff.build_variant(workdir, wrapper, s, baseline_rows,
                             orig_sections, 6, fast=True)   # -j6, fast gate verify
    except Exception as e:
        r = dict(name=s['name'], flags=s['flags'], error=str(e))
    out.append(r)
    (workdir / 'results_partial.json').write_text(json.dumps(out, indent=1) + '\n')
```

`ff.build_variant` runs `cmake --build <scratch> --target link_so -j 6` and
then `tools/verify/verify.py --fail-on-wrong-type --top 0` (fast mode for the
fuzzy score only). Branch-weight specs also carry
`cmake_args=['-DGOF2_NO_PER_FILE_WEIGHTS=ON']`.

### B.3 Target-fuzzy scorer (`/dev/shm/w5210/score_targets.py`)

The scorer imports `tools/verify/verify.py`, builds the same `Resolver` objects
as `verify.main()`, and calls `verify.disassemble` + `verify.reconcile` +
`verify.score` for only the 24 non-exact functions. Its baseline output matches
`cmake-build-match/verify/report.json` exactly (24/24), so the +0.1 deltas in
§4.4 are on the official metric.

### B.4 Scratch artifacts

```text
/dev/shm/w5210/help/help-hidden.txt          clang -mllvm -help-hidden (1,035 lines)
/dev/shm/w5210/help/help.txt                 clang -mllvm -help (61 lines)
/dev/shm/w5210/help/bool_ed_mllvm.txt        673 bool candidates
/dev/shm/w5210/help/value_mllvm.txt          537 value candidates
/dev/shm/w5210/screen_ed2/results.json       bool object screen (231 changed rows)
/dev/shm/w5210/screen_val/results.json       value object screen
/dev/shm/w5210/full_le10b/results_partial.json  89 full-link variants
/dev/shm/w5210/full_focus/results_partial.json  100 focus-category variants
/dev/shm/w5210/full_one/results_partial.json    the last necessary-condition flag
/dev/shm/w5210/target_scores.json             official target scores, 88 variants
/dev/shm/w5210/target_scores_focus.json       official target scores, 99 variants
```

No `--update-baseline` was run; `tools/verify_baseline.json`,
`tools/sodiff_allow.txt`, `tools/source_order.txt`, `src/` and `CMakeLists.txt`
are unchanged.
