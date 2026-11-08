# Status::missionCompleted — synthetic instrumented PGO: 1432 B is reachable,
# `.rel.plt` 0 is not (wave 156, w10214)

Status: **harness + profile artifacts + note, no `src/` / `CMakeLists.txt`
change**. Worktree `wt/w10214`, base master `cf74f4ee6` (wave 155). Scratch
`/dev/shm/w10214`. Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370. `.text` 86,044 differing bytes, `.ARM.extab`
1,292, `.rel.plt` byte-identical.

Mission (w10117 follow-up): build a synthetic instrumented `.profdata` that
reproduces `Status::missionCompleted` at 1432 B **with `.rel.plt` 0**, and
test whether the same profile improves `MGame::OnTouchEnd`, `Layout::Layout`,
`Radar::draw`, `LevelScript::process`.

Target: `_ZN6Status16missionCompletedEbbx` (1432 B).
Baseline: **raw 722, aligned 320, size 1432, `.rel.plt` 0**.

## 0. Bottom line

1. **Reproduced the w10117 base-3 flat vector exactly**: flat groups
   `4/3/2`, every non-case counter = 3, latch 1, c28 1 gives
   **raw 1376, aligned 674, size 1432, `.rel.plt` 7**. The 7-byte
   `.rel.plt` delta is a 3-cycle of `Mission::getClientImage`,
   `Item::isInList(int,Array<Item*>*)` and `Mission::setWon`: the
   instrumented body places the `getClientImage` filter block *after* the
   switch, so those three PLT entries are allocated in a different order.
2. **NEW: the `.rel.plt` delta is caused by `!prof` metadata on the
   non-switch branches, not by the switch weights.** The unprofiled body
   (which the original matches) has `!prof` only on the two
   `__builtin_expect` branches. `-fprofile-instr-use` adds a weight to
   *every* branch. In particular the `isCampaignMission` branch becomes
   `!{!"branch_weights", i32 -1, i32 1}` (the successor switch block has
   no own counter), which flips the fallthrough from the filter chain to
   the switch and moves `getAgent`/`getClientImage`/`getTargetStation`
   after `getType`. That is exactly the call-order permutation in (1).
3. **NEW: a flow-consistent profile fixes the filter order.** A real
   execution model (N=1 call, K loop iterations, every mission passes the
   filter: counters 0-3=1, 4=K, 8-11=K, 12=K, case edges 4/3/2,
   default=K-68) turns the branch into `!{!"branch_weights", i32 1,
   i32 1001}`, so the filter is the fallthrough. The first **eight**
   first-occurrence calls then match the original
   (`hasWon, isCampaignMission, getAgent, getClientImage,
   getTargetStation, getTargetStation, getType, getStatusValue`), but the
   body grows to **1448 B** (raw 1377, aligned 762) and `.rel.plt` is still
   6 (the case-body order is frequency-grouped, not source order).
4. **NEW: a per-case source-order gradient gives the source-order jump
   table.** Assigning `group1=4000`, `group2=3000+descending source index`,
   `group3=2000+descending source index` (same 4:3:2 ratio, so the same
   branch probabilities) reproduces the original's base-150 table order
   `167/174,150,151,152,153,154,155,158,162,166,170,184,189,156,157,160,
   163,164,165,168,169,171,172` exactly. But the body is **1408 B**
   (raw 1367, aligned 855). Combining the consistent flow model with the
   gradient (`consgrad`, K=100000) gives source-order table + filter-first
   order, size **1428**, raw 1354, aligned 823, `.rel.plt` 6.
5. **No instrumented vector reaches 1432 B *and* `.rel.plt` 0.** The
   size-1432 points (base 3 flat) have the frequency-grouped table; the
   source-order-table points are 1408-1428 B. The two constraints pull on
   the same `MachineBlockPlacement` block frequencies. The original body
   is the *unprofiled* body: only the switch carries weights. Instrumented
   PGO cannot express "weights on the switch only" because it annotates
   every branch.
6. **The same profile is a no-op on the other four functions.** A uniform
   instrumented profile (every counter 1) and a near-uniform hot profile
   (entry 1, all others 1000) leave `MGame::OnTouchEnd`, `Layout::Layout`,
   `Radar::draw` and `LevelScript::process` byte-identical to the baseline:
   raw/aligned/size are unchanged (4391/903/8080, 5837/1986/7072,
   8490/1456/9908, 55123/17180/57232). Uniform scaling preserves branch
   probabilities, so it is neutral; it neither improves nor regresses.

## 1. Measurements

`raw` = same-address differing bytes (`fdiff.py`); `aligned` =
instruction-index alignment (`align_diff.py`); `size` = function bytes;
`relplt` = differing bytes of `.rel.plt` vs the original (29392 B).

### 1.1 Status::missionCompleted

| variant | raw | aligned | size | relplt | first-occurrence call order |
|---|---:|---:|---:|---:|---|
| baseline (no profile) | **722** | **320** | **1432** | **0** | canonical |
| instr flat base 3, latch 1, c28 1 | **1376** | **674** | **1432** | 7 | grouped |
| instr flat base 0 | 1375 | 850 | 1424 | 7 | grouped |
| instr flat base 1 | 1357 | 652 | 1440 | 7 | grouped |
| instr flat base 2 | 1364 | 676 | 1424 | 7 | grouped |
| instr flat base 4 | 1365 | 682 | 1424 | 7 | grouped |
| gradient (4:3:2 + source order) | 1367 | 855 | 1408 | 7 | source table |
| consistent flow K=1000 | 1377 | 762 | 1448 | 6 | filter-first |
| consistent flow K=10000 | 1393 | 781 | 1468 | 6 | filter-first |
| consgrad K=100000 | 1354 | 823 | 1428 | 6 | filter-first + source table |
| sink=0 + flat base 3 | 1312 | 780 | 1428 | 7 | grouped |
| sink=0 + flat base 1 | 1315 | 748 | 1436 | 4 | grouped |
| sink=0 + flat base 16 | 1335 | 649 | 1416 | 2 | grouped |

The `sink=0` variants (`-mllvm -max-uses-for-sinking=0`) restore the eager
`shortMission`/`longMission` entry computation (`eager=True`) but still do
not hit 1432 with a matching call order.

### 1.2 Other four functions (uniform instrumented profile)

| function | baseline raw/aligned/size | uniform instr raw/aligned/size | hot instr |
|---|---|---|---|
| `MGame::OnTouchEndEiiPv` | 4391 / 903 / 8080 | 4391 / 903 / 8080 | same |
| `Layout::Layout()` (C2) | 5837 / 1986 / 7072 | 5837 / 1986 / 7072 | same |
| `Radar::draw(Player*,Hud*,int)` | 8490 / 1456 / 9908 | 8490 / 1456 / 9908 | same |
| `LevelScript::process(int)` | 55123 / 17180 / 57232 | 55123 / 17180 / 57232 | same |

## 2. Mechanism

The unprofiled optimized IR of `missionCompleted` has exactly two
`!prof` nodes:

```
!9551 = !{!"branch_weights", i32 2000, i32 1}   ; case 166 __builtin_expect
!9657 = !{!"branch_weights", i32 1, i32 2000}   ; case 184 __builtin_expect
```

and the `shortMission`/`longMission` (`icmp sgt i64 %3, 2000/10000`)
computations sit in the entry block. `-fprofile-instr-use` changes both:

* it sets `function_entry_count`, which makes `hasProfileData()` true and
  runs `LoopSink`, moving the two `icmp sgt i64` into the case bodies
  (`eager=False`; `-mllvm -max-uses-for-sinking=0` suppresses this);
* it attaches a `branch_weights` node to every conditional branch. The
  `isCampaignMission` branch (`br i1 %22, label %switch, label %filter`)
  gets `i32 -1, i32 1` because the switch block has no counter of its own;
  `-1` is the "unknown/unreachable" sentinel and lets the layout put the
  switch on the fallthrough path.

A flow-consistent count vector gives the switch block a real frequency and
the branch becomes `i32 1, i32 1001`, restoring the filter-first order.
But the case blocks then carry exact frequencies, so
`MachineBlockPlacement` orders them by frequency; the 4:3:2 plateau ties
are broken by the block-frequency sums (not by CFG order), which is why
the source-order table needs the per-case gradient. The two fixes are
independent and can be combined (`consgrad`), but the resulting body is
1428 B, not 1432.

## 3. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10214
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6

# 1432 B, relplt 7 (the w10117 result)
W9620_SCRATCH=/dev/shm/w10214/run \
  python3 _work/reconstructions/prof_instr_w10214.py flat 3

# source-order table, 1408 B
W9620_SCRATCH=/dev/shm/w10214/run \
  python3 _work/reconstructions/prof_instr_w10214.py gradient

# filter-first, 1448 B
W9620_SCRATCH=/dev/shm/w10214/run \
  python3 _work/reconstructions/prof_instr_w10214.py consistent 1000

# filter-first + source-order table, 1428 B
W9620_SCRATCH=/dev/shm/w10214/run \
  python3 _work/reconstructions/prof_instr_w10214.py consgrad 100000

# restore the canonical .so
cmake --build cmake-build-match --target link_so -j6
```

Committed artifacts:

* `_work/reconstructions/prof_instr_w10214.py` — the driver (uses the
  existing `pgo_combine_w9620.py` low-level helpers).
* `_work/reconstructions/status_instr_flat_base3_w10214.proftext` — the
  1432 B flat profile (116 counters).
* `_work/reconstructions/status_instr_consistent_w10214.proftext` — the
  flow-consistent profile.
* `_work/reconstructions/status_instr_consgrad_w10214.proftext` — the
  consistent + source-order-gradient profile.

## 4. Open questions

1. **Make `.rel.plt` 0 at 1432 B.** The only remaining lever is to keep
   the *unprofiled* block placement while giving the switch the flat
   weights. A partial/edge-only profile (weights on the switch, no
   `!prof` on the filter branch) would do it; LLVM 7.0.2's
   `-fprofile-instr-use` has no per-branch opt-out. Stripping the
   non-switch `!prof` nodes from the optimized IR reproduces the w9920
   result (raw 558) but is not a `.profdata`.
2. **Suppress `function_entry_count`.** As w10020 noted, sample PGO sets
   it unconditionally (`headSamples + 1`), which is what triggers
   `LoopSink`. A backend flag or a newer LLVM that drops it would let the
   sample profile keep the 1432 B body.
3. The source-order table at 1432 B may require a *real* profile whose
   case frequencies are graded in source order but whose switch weights
   are still 4:3:2 — exactly the ambiguous plateau w9920 found. A real
   execution count vector (not synthetic) is the natural next probe.
