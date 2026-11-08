# Status::missionCompleted — an instrumented profile that hits 1432 B with
# `.rel.plt` 0 (wave 157, w10313)

Status: **analysis + profile artifact + note, no `src/` / `CMakeLists.txt`
change**. Worktree `wt/w10313`, base master `08575efff` (wave 156). Scratch
`/dev/shm/w10313`. Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370.

Target: `_ZN6Status16missionCompletedEbbx` (1432 B).
Baseline: **raw 722, aligned 320, size 1432, `.rel.plt` 0**.

Mission (w10214 follow-up): find a synthetic instrumented profile vector that
reaches **1432 B** with **`.rel.plt` 0** and the filter-first call order.
Read first: `status-instrumented-pgo-wave156-w10214.md`,
`pgo-provenance-wave155-w10117.md`, `status-sample-profile-w10020.md`.

## 0. Bottom line

1. **FOUND: a 116-counter instrumented vector that gives size 1432,
   `.rel.plt` 0, raw 1299, aligned 583.** It is the w10214 `flat base 3`
   vector with **base lowered from 3 to 1**, the **loop counter set to 3**,
   and **counter 10 (the `getClientImage` filter block) raised to 5**;
   compiled with `-mllvm -max-uses-for-sinking=0` (eager entry, matching the
   original) and `-mllvm -profile-guided-section-prefix=false`.
   The `isCampaignMission` branch weight becomes `!{!"branch_weights", i32 1,
   i32 2}` (filter is the fallthrough) instead of the w10214 `{-1, 1}`
   (switch fallthrough). `getClientImage` is emitted before `getType`,
   which is exactly the 3-cycle that made `.rel.plt` 7.
2. **The mechanism is the `isCampaignMission` branch weight.** It is
   `{switch_count, filter_count}` where `switch_count = loop_count -
   filter_count`. With the w10214 flat base 3 vector `loop_count =
   filter_count = 3`, so the switch count is 0 and the loader emits the
   unknown sentinel `-1`; MachineBlockPlacement then puts the switch on the
   fallthrough. Lowering the internal base to 1 and raising the loop count to
   3 (with counter 10 = 5) gives a positive filter weight and a real switch
   weight, so the filter is the fallthrough.
3. **The +4 B is a layout/encoding transient.** The unperturbed
   `base 1 / loop 3 / groups 4:3:2` vector is **1428 B** (raw 1310,
   aligned 562) with `.rel.plt` 0. Raising counter 10 from 1 to 5 flips one
   block-placement tie and lands on exactly **1432 B** (raw 1299,
   aligned 583). Many other single-counter perturbations also hit
   1432 + `.rel.plt` 0 (see §2); counter 10 = 5 has the lowest raw.
4. **The gate is unchanged at 4507/4507** because the body is still not
   byte-identical (raw 1299). The profile is a provenance artifact, not a
   gate win. The original body is the *unprofiled heuristic* body; the
   instrumented body still differs in case-body order and shared-tail
   placement (see §3).
5. **The uniform-case vector has the best full call order.** With every
   counter equal (base 3, loop 1000, filter 1000) the first-occurrence call
   order matches the original 21/25 and the first eight calls match exactly
   (`hasWon, isCampaignMission, getAgent, getClientImage, getTargetStation,
   getType, getStatusValue, getProductionGoodAmount`), but the body is only
   1372 B. No single case-counter perturbation of that vector reaches
   1432 B; the 1432 B size is tied to the 4:3:2 plateau.

## 1. Measurements

`raw` = same-address differing bytes (`fdiff.py`); `aligned` =
instruction-index alignment (`align_diff.py`); `size` = function bytes;
`relplt` = differing bytes of `.rel.plt` (29392 B).

| variant | raw | aligned | size | relplt |
|---|---:|---:|---:|---:|
| baseline (no profile) | **722** | **320** | **1432** | **0** |
| w10214 flat base 3 | 1376 | 674 | 1432 | 7 |
| w10214 consgrad K=100000 | 1354 | 823 | 1428 | 6 |
| **NEW: base 1, loop 3, groups 4:3:2, c10=5, sink=0** | **1299** | **583** | **1432** | **0** |
| base 1, loop 3, groups 4:3:2, sink=0 (no c10) | 1310 | 562 | 1428 | 0 |
| base 1, loop 4, groups 4:3:2, sink=0 | 1325 | 577 | 1436 | 0 |
| base 1, loop 3, c20=2, sink=0 | 1310 | 563 | 1432 | 0 |
| base 1, loop 3, c22=2, sink=0 | 1308 | 565 | 1432 | 0 |
| base 1, loop 3, c80=2, sink=0 | 1309 | 563 | 1432 | 0 |
| uniform base 3, loop 1000, filter 1000, sink=0 | 1271 | 630 | 1372 | 0 |

The exact counter vector (116 values) is
`_work/reconstructions/status_instr_1432_relplt0_w10313.proftext`:

```
all 1 except
  [4]  = 3                       ; loop body
  [10] = 5                       ; getClientImage filter block
  group1 {167,174}->13, 8->16, {0,11}->28, 13->32, 14->35 = 4
  group2 {150,151,152,153,154,155,158,162,166,170,184,189} = 3
  group3 {156,157,160,163,164,165,168,169,171,172} = 2
```

The `isCampaignMission` branch metadata is `!{!"branch_weights", i32 1,
i32 2}` (filter fallthrough). The w10214 flat base 3 build is `{-1, 1}`.

## 2. Other 1432 + relplt-0 single-counter perturbations

All of these are `base 1 / loop 3 / groups 4:3:2 / sink=0` plus one change:

| counter | value | raw | aligned |
|---|---|---:|---:|
| 8 (second getTargetStation filter) | 3/4/5 | 1337 | 578 |
| 10 (getClientImage filter) | 5 | **1299** | 583 |
| 19 (case 8 pendingProducts loop) | 0 | 1324 | 849 |
| 20 (case 8 pendingProducts body) | 2..5 | 1310 | 563 |
| 22 (case 8 getIndex) | 2..5 | 1308 | 565 |
| 23 (case 8 getTargetStation) | 0 | 1310 | 563 |
| 24 (case 8 getProductionGoodIndex) | 0 | 1308 | 565 |
| 26 (case 8 isInList) | 2 | 1338 | 578 |
| 54 (case 158 hasGood) | 0..5 | 1309-1326 | 592-614 |
| 56 (case 158 hasSpecial) | 0 | 1310 | 604 |
| 65 (case 166 expect) | 0 | 1327 | 578 |
| 80 (case 189 loop) | 2..5 | 1309 | 563 |
| 81 (case 189 getSort) | 2..5 | 1328 | 572 |
| 82 (case 189 return) | 0 | 1309 | 563 |
| 83 (case 189 getStatusValue) | 0/4/5 | 1322-1328 | 572-579 |

## 3. Why the body is still not byte-identical

The original body is the *unprofiled heuristic* body with only the switch
carrying the 4:3:2 plateau. `-fprofile-instr-use` annotates every branch.
`-mllvm -max-uses-for-sinking=0` restores the eager
`shortMission`/`longMission` entry computation (the original's `rsbs`/`sbcs`
sequence), but the case-body order is still frequency-driven, not the
original's source order. The original's shared `isInList(int,int)` tail sits
at case 172 (address 0xac454) with case 166 carrying its own copy; the
instrumented build places the merged tail earlier. Reproducing that needs a
profile whose shared-tail frequency is *lower* than the case-162 block,
which the sum-of-predecessors tail frequency cannot express.

The uniform-case vector (§0.5) gets the full call order right but is 60 B
short; the 4:3:2 plateau is what supplies the 1432 B size. The two
constraints still pull on the same MachineBlockPlacement frequencies.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10313
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6

# build the 1432 / relplt-0 candidate
W9620_SCRATCH=/dev/shm/w10313/run python3 - <<'PY'
import sys, os
sys.path.insert(0, '_work/reconstructions')
import pgo_combine_w9620 as H, prof_instr_w10214 as P
meta = H.get_meta()
v = P.flat(meta, 1, 4, 3, 2)
v[4] = 3
v[10] = 5
m = H.build(meta, v, 'final_candidate',
            ['-mllvm', '-max-uses-for-sinking=0'])
print(m['raw'], m['aligned'], m['size'])
PY
# -> 1299 583 1432
```

The committed profile text is
`_work/reconstructions/status_instr_1432_relplt0_w10313.proftext`
(116 counters, func hash 2003737435339834133).

## 5. Files changed

* `_work/reconstructions/status_instr_1432_relplt0_w10313.proftext` — the
  winning counter vector.
* `_work/reconstructions/status-instrumented-1432-relplt0-wave157-w10313.md`
  — this note.
