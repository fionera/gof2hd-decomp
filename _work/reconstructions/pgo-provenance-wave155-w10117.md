# PGO provenance: sample vs instrumented, and why the flat Status switch
# vector is a sample signature that LLVM 7 cannot replay (wave 155, w10117)

Status: **harness + profile files + note, no `src/` / `CMakeLists.txt`
change**. Worktree `wt/w10117`, base master `f4e0ea40b` (wave 154). Scratch
`/dev/shm/w10117`. Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370. `.text` 86,347 differing bytes, `.ARM.extab`
1,255.

Mission (w10020 follow-up): decide whether the original was built with
sample PGO (`-fprofile-sample-use`) or instrumented PGO
(`-fprofile-instr-use`), and whether a synthetic profile can reproduce the
residual branch layouts of `Status::missionCompleted`, `MGame::OnTouchEnd`,
`Layout`, `Radar::draw`, `LevelScript::process`.

## 0. Bottom line

1. **The flat Status vector is a sample-count signature, but LLVM 7.0.2's
   sample loader cannot replay it on this source shape.**  The loader
   weights a switch edge from the *successor block's* sample weight.  Eight
   of the 29 cases begin with a branch-only block (`br i1 %atStation`,
   `%docked`, `%shortMission`, `%longMission`, `%notAtStation`,
   `%dockedAtStation`) whose only instruction is a `br`.  LLVM 7 skips
   branches in `getInstWeight`, so those blocks stay weight 0 and their
   switch edges stay at 1.  This is independent of the sample counts:
   sampling **every line** of the function still leaves cases
   `0, 11, 13, 156, 164, 165, 169, 171` at weight 1.  The best sample
   vector is
   `[1,4,4,4,1,1,1,4,3x12,1,2,2,1,1,1,2,1,1,2]`, not the flat
   `4,4,4,3,2`.
2. **Sample PGO always sets `function_entry_count` and grows the body.**
   The profile header `name:total:head` with `head=0` still emits
   `!{!"function_entry_count", i64 1}` (verified in the optimized IR).
   The best sample build is **size 1440** (raw 1318, aligned 561) with
   `-mllvm -max-uses-for-sinking=0 -mllvm -profile-guided-section-prefix=false`;
   without those flags it is 1464.  No sample vector tried gives 1432
   (4/4/4/3/2: 1452; 2/2/2/1/1: 1448; 1/1/1/1/1: 1448; 5/5/5/4/3: 1452;
   8/6/5/4/3: 1452).  The +8 B shifts every later function, so the gate
   collapses to **byte_exact 2327**.
3. **Instrumented PGO can encode the flat vector exactly, but the full
   profile also changes the non-switch block frequencies.**  The body
   shrinks to 1424 (base 0), 1440 (base 1) or 1388 (base 12, c28 28,
   latch 0), and the entry `shortMission`/`longMission` computation is
   sunk (`eager=False`).  One base value (3, latch 1, c28 1) hits **1432 B**
   (raw 1376, aligned 674), but the first-occurrence call order becomes
   `[1,2,3,4,5,0]` instead of the canonical `[0,1,2,3,4,5]`, so
   `.rel.plt` differs by 7 bytes and 20 extra functions break:
   **byte_exact 4487**.  A 240-point grid over `base/latch/c28/filter`
   found no size-1432 point with the canonical call order.
4. **Uniform profiles are a large regression on all five functions.**
   The original's layout is the *heuristic* layout, not a uniform-profile
   layout: every uniform sample or instrumented profile roughly doubles
   the raw and aligned diffs and changes the function size.
5. **Verdict.**  The flat `4 > 3 > 2` plateau is a genuine sample-profile
   signature (a real execution would produce graded per-case counts), but
   it is **not reproducible from a synthetic line-based sample profile**
   on the current source shape, and the sample loader's mandatory
   `function_entry_count` makes the body 8 B too large.  Instrumented PGO
   is falsified as a whole-tree mechanism (uniform profile collapses the
   gate) and cannot hit 1432 + canonical order with a flat switch vector.
   The remaining plausible provenance is a **real sample profile whose
   source shape put a call before each `atStation`/`docked` guard** (so
   the successor block is not branch-only), or an instrumented profile
   whose non-switch counters happen to reproduce the heuristic body.

## 1. Measurements

`raw` = same-address differing bytes (`fdiff.py`); `aligned` =
instruction-index alignment (`align_diff.py`); `size` = function bytes.
Baseline is the canonical no-profile build.

### 1.1 Status::missionCompleted (`_ZN6Status16missionCompletedEbbx`)

Baseline: **raw 722, aligned 320, size 1432**.

| variant | raw | aligned | size | gate byte_exact |
|---|---:|---:|---:|---:|
| baseline | **722** | **320** | **1432** | **4507** |
| sample 3/3/3/2/1, no flags | 1400 | 607 | 1464 | — |
| sample 3/3/3/2/1 + sink=0 | 1326 | 559 | 1440 | — |
| sample 3/3/3/2/1 + sink=0 + prefix off | **1318** | **561** | **1440** | **2327** |
| sample 4/4/4/3/2 + sink=0 + prefix off | 1331 | 579 | 1452 | — |
| sample 2/2/2/1/1 + sink=0 + prefix off | 1323 | 580 | 1448 | — |
| sample 1/1/1/1/1 + sink=0 + prefix off | 1327 | 595 | 1448 | — |
| sample 5/5/5/4/3 + sink=0 + prefix off | 1339 | 572 | 1452 | — |
| sample 8/6/5/4/3 + sink=0 + prefix off | 1339 | 572 | 1452 | — |
| sample + propagate-iterations=0 | 1193 | 696 | 1496 | — |
| instr flat base 0, latch 1, c28 1 | 1375 | 850 | 1424 | 3044 |
| instr flat base 1 | 1357 | 652 | 1440 | — |
| instr flat base 2 | 1364 | 676 | 1424 | — |
| instr flat base 3 | **1376** | **674** | **1432** | **4487** |
| instr flat base 4 | 1366 | 682 | 1424 | — |
| instr flat base 5 | 1362 | 650 | 1408 | — |
| instr flat base 12, c28 28, latch 0 | 1323 | 660 | 1388 | — |
| instr rank (w9620) base 12, c28 28, latch 0 | 1350 | 654 | 1428 | — |

The sample vector for 3/3/3/2/1 is
`[1,4,4,4,1,1,1,4,3,3,3,3,3,3,3,3,3,3,3,3,1,2,2,1,1,1,2,1,1,2]`
(default, then cases 167,174,8,0,11,13,14,150..172 in IR order).
Sampling every line gives
`[1,5,5,5,1,1,1,5,5,5,5,5,5,5,5,5,5,5,5,5,1,5,5,5,1,1,5,1,1,5]`:
the same eight cases stay at 1.  Sampling the switch terminator line
(offset 20) does not change either vector.

The instrumented flat profile uses the exact w9920 group vector
(`{167,174,8,0,11,13,14}=4`, group2=3, group3=2, default 1) on every
counter, with the non-case counters set to `base` (and 6/7/12=`latch`,
28=`c28`).  The base-3 build is the only size-1432 point; its table order
is `[167,174,9,10,12,8,14,13,184,170,162,189,153,155,150,154,166,152,151,
158,163,165,157,156,160,172,168,164,169,171,11]`, not the original's
source order.

### 1.2 The other four functions

Uniform all-lines sample profile (`-fprofile-sample-use`, every line of
the function at 4 samples) and uniform instrumented profile (every
counter 1, `-fprofile-instr-use`, section prefix off).  Baseline is the
canonical build.

| function | baseline raw/aligned/size | sample raw/aligned/size | instr raw/aligned/size | sample gate | instr gate |
|---|---|---|---:|---:|---:|---:|
| `MGame::OnTouchEndEiiPv` | 4391 / 903 / 8080 | 7844 / 4350 / 8252 | 7638 / 4106 / 8168 | 1378 | 3106 |
| `Layout::Layout()` (C2) | 5849 / 2006 / 7072 | 6631 / 4151 / 7300 | 6644 / 4119 / 7332 | 1378 | — |
| `Radar::draw(Player*,Hud*,int)` | 8577 / 1484 / 9908 | 9634 / 5244 / 10420 | 9629 / 5801 / 10240 | 1471 | — |
| `LevelScript::process(int)` | 55327 / 17176 / 57232 | 55604 / 34323 / 57588 | 55594 / 36284 / 57652 | 1378 | — |

Every variant changes the function size, so the gate collapses to the
number of functions before the target.  `Layout` is the only one whose
sample profile leaves the function byte-identical when the profile name
is wrong (`_ZN6LayoutC1Ev` does not match the instrumented IR's
`_ZN6LayoutC2Ev`); the table above uses C2.

## 2. Mechanism: why the sample loader leaves eight cases at 1

`SampleProfileLoader::emitAnnotations` computes a switch edge weight from
`getBlockWeight(successor)`.  `getInstWeight` ignores branch instructions.
For the cases whose first block is a guard on a function argument:

```
case 0: case 11:
    if (atStation && station->getIndex() == m->getTargetStation()) ...
```

the optimized IR is

```
; <label>:109:
  br i1 %1, label %110, label %380      ; %1 = atStation
```

so the successor block has no sample-able instruction and its weight is 0.
The same applies to case 13 (`atStation && field_0xf0`), case 156
(`!atStation && ...`), 160 (`if (atStation)`), 164 (`if (longMission)`),
165 (`if (notAtStation && ...)`), 169 (`if (shortMission)`), 171/172
(`if (dockedAtStation && ...)`), and 189 (`if (atStation)`).  Cases 14,
162 and 166 have a call in the first block (lines 1375, 1440-1442,
1458-1460) and do get weight.

This also explains why the sample vector's group-2 plateau (3) is exact
but the first group is `4,4,4,1,1,1,4` and the group-3 tail is
`1,2,2,1,1,1,2,1,1,2`.

## 3. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10117
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6

# sample profile (best variant: raw 1318, aligned 561, size 1440, gate 2327)
python3 _work/reconstructions/prof_driver_w10117.py sample-lines \
  src/game/mission/Status.cpp _ZN6Status16missionCompletedEbbx 1314 \
  "$(python3 - <<'PY'
a=[23,24,25]; b=[29,30,31,33,39,40]; c=[50,51,55,56,60,61,62]
d=[67,68,73,74,80,81,89,90,95,96,101,102,108,109,110,111,112,125,126,130,131,143,144,150,156,157,162,163,164,165,171,172,173]
e=[181,182,187,188,196,199,206,218,223,224,228,229,235,236,240,241,245,246,249]
print(','.join('%d:%d'%(o,c) for g,c in ((a,3),(b,3),(c,3),(d,2),(e,1)) for o in g))
PY
)" --extra -mllvm -max-uses-for-sinking=0 -mllvm -profile-guided-section-prefix=false

# instrumented flat profile (raw 1375, aligned 850, size 1424, gate 3044)
python3 _work/reconstructions/flat_status_w10117.py 0 4 3 2

# restore the canonical .so
cmake --build cmake-build-match --target clean
cmake --build cmake-build-match --target link_so -j6
```

The committed profile files are
`_work/reconstructions/status_sample_w10117.prof` (the 3/3/3/2/1 sample
profile, offsets relative to line 1314) and
`_work/reconstructions/status_flat_w10117.proftext` (the base-0 flat
instrumented profile).  `prof_common_w10117.py` and
`prof_driver_w10117.py` are the harness; `flat_status_w10117.py` builds
the instrumented Status variant.  All three currently hardcode the
`wt/w10117` root and `/dev/shm/w10117` scratch.

## 4. Open questions

1. **Source-shape test.**  If the original wrote the guards with the call
   first (`if (station->getIndex() == m->getTargetStation() && atStation)`),
   the successor block would contain the call and the sample loader would
   weight it.  Reordering the eight guards and re-running the sample
   profile is the cheapest way to test "sample PGO + different source
   shape".
2. **Suppress `function_entry_count`.**  LLVM 7.0.2 sets it
   unconditionally (`headSamples + 1`).  A future LLVM or a backend flag
   that drops it would let the sample profile keep the 1432 B body.
3. **Instrumented flat + canonical call order.**  The 240-point grid found
   no size-1432 point with `[0,1,2,3,4,5]`.  A wider search over the
   pre-switch filter counters (8-11) and the loop counters (6/7/12) might
   find one; the w9620 shifted-rank profile already proves the call order
   is controllable, just not at 1432.
4. The `Layout` SLP groups and `MGame::OnTouchEnd` pre-dock join are not
   reproduced by uniform profiles; they need a targeted hot/cold profile
   derived from the original's block layout, not a uniform one.
