# Synthetic instrumented PGO on MGame/createMission/Layout/Radar — a profile
# always moves *away* from the original (wave 157, w10314)

Status: **research note + harness only**, 2026-11-08. Worktree `wt/w10314`,
base master `08575efff` (wave 156). Scratch `/dev/shm/w10314`. Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370. `.text` 85,831 differing bytes, `.ARM.extab`
1,292, `.rel.plt` byte-identical.

Mission (wave-155/156 PGO probes only moved Status): build a synthetic
instrumented profile that targets the branch-weight signatures in
`MGame::OnTouchEnd` (pre-dock join), `Level::createMission` (raid/alien),
`Layout::Layout` (SLP groups) and `Radar::draw` (register permutation), and
test whether any single profile improves those functions without regressing
the 4507 exact functions.

## 0. Bottom line

1. **Every profile tested is strictly worse than the no-profile build on all
   four functions.** The original's block layout is the *heuristic*
   (`-Oz` no-profile) layout, not a profile layout. A uniform instrumented
   profile (all counters 1) already regresses all four; extreme hot/cold
   profiles regress further.
2. **A profile that changes the target's size collapses `byte_exact`.** The
   uniform `OnTouchEnd` profile is 8,168 B (vs 8,080), which shifts `.text`
   and drops `byte_exact` 4507 -> 2041 (`linked_exact` stays 4507). No
   single-counter profile found keeps the size exact.
3. **NEW mechanism: `__builtin_expect` `!prof` metadata is not overridden by
   `-fprofile-instr-use` in LLVM 7.0.2.** The `isDockingToStream` branch in
   `OnTouchEnd` keeps `!{!"branch_weights", i32 2000, i32 1}` even when its
   true-successor counter is set to 100,000. So the pre-dock join cannot be
   moved by PGO while the expect hint is in the source. Branches without an
   expect hint *are* re-weighted (e.g. `isDockingToAsteroid` 2:1 ->
   1001:1), but the re-weighting does not change the object bytes.
4. **No profile is landable.** The gate is green only with the canonical
   no-profile build.

## 1. Measurements

`raw` = same-address differing bytes; `aligned` = instruction-index
alignment; `size` = function bytes. Baseline = canonical no-profile build.

### 1.1 Per-function profile variants

| function | baseline raw/aligned/size | uniform | all1000, entry 0 | all0, entry 1000 | all1000, mid 0 |
|---|---|---|---|---|---|
| `MGame::OnTouchEnd` | **4391 / 903 / 8080** | 7638 / 4106 / 8168 | 7685 / 4085 / 8060 | 7583 / 3924 / 7920 | 7685 / 4085 / 8060 |
| `Level::createMission` | **5487 / 774 / 17172** | 16612 / 9239 / 17492 | 16704 / 8929 / 17272 | 16606 / 6553 / 17248 | 16573 / 9942 / 17768 |
| `Layout::Layout` (C2) | **5822 / 1984 / 7072** | 6653 / 4162 / 7332 | 6586 / 4047 / 7268 | 6293 / 3910 / 7376 | 6597 / 4099 / 7268 |
| `Radar::draw` | **8320 / 1296 / 9908** | 9628 / 5942 / 10596 | 9676 / 6020 / 10780 | 9637 / 5321 / 10412 | 9601 / 5218 / 10224 |

(`all1000, entry 0` = every counter 1000 except counter 0 = 0; `all0, entry
1000` = every counter 0 except counter 0 = 1000; `all1000, mid 0` sets the
middle counter to 0.)

### 1.2 Best single-counter-high profiles (all counters 1, one counter 1000)

| function | best raw | best aligned |
|---|---|---|
| `MGame::OnTouchEnd` | ctr 2: 7519 / 4149 / 7872 | ctr 26: 7647 / 3755 / 8196 |
| `Level::createMission` | ctr 188: 16539 / 9168 / 17484 | ctr 222: 16639 / 8557 / 17460 |

Both are far worse than the no-profile baseline (4391/903 and 5487/774).
The full 245- and 225-counter sweeps produced no variant below baseline on
either metric.

### 1.3 Status control

`Status::missionCompleted` with its committed flat base-3 profile reproduces
w10214 exactly: raw 1376, aligned 674, size 1432 (baseline 722/320/1432).
The generic harness is therefore faithful; the four target functions are
simply insensitive to *improving* weights.

### 1.4 Gate

| build | byte_exact | linked_exact | size_exact | data | imports |
|---|---:|---:|---:|---:|---:|
| canonical (no profile) | **4507** | **4507** | **4524** | **843** | **207** |
| uniform `OnTouchEnd` profile | 2041 | 4507 | 4524 | 843 | 207 |
| `all1000,entry0` `OnTouchEnd` | 2041 | 4507 | 4524 | 843 | 207 |

The `byte_exact` collapse is the address-shift artefact of the 88 B size
change (literal pools / absolute words in later functions), not a change in
the other 4,523 functions' instruction streams (`linked_exact` stays 4507).

## 2. Mechanism

The no-profile `-Oz` layout of these four functions is already the original's
layout (they are among the 17 residual non-exact functions for source-shape
reasons, not block-order reasons). Adding any profile:

* sets `function_entry_count`, which enables `LoopSink` and moves entry
  computations;
* attaches `branch_weights` to every conditional branch, which changes the
  `MachineBlockPlacement` fallthrough choices even when the relative order is
  preserved;
* therefore always moves the body away from the heuristic layout.

The `__builtin_expect` branches are the one exception: their `!prof` node is
preserved verbatim by `-fprofile-instr-use`, so the pre-dock join is pinned
to 2000:1 regardless of the profile. The branch weights of non-expect
branches do change (verified in the optimized IR), but for these functions
the changed weights do not alter the emitted bytes at all — only the
function-entry/loop-sink effect moves the body.

## 3. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10314
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6

# generic harness (instrumented profile for any TU/function)
python3 _work/reconstructions/profgen_w10314.py \
  src/game/menu/MGame.cpp _ZN5MGame10OnTouchEndEiiPv

# single-counter sweep
python3 /dev/shm/w10314/sweep_best.py \
  src/game/menu/MGame.cpp _ZN5MGame10OnTouchEndEiiPv

# Status control (committed flat profile)
python3 - <<'PY'
import sys, os
sys.path.insert(0, '_work/reconstructions')
from profgen_w10314 import *
p = Prof('src/game/mission/Status.cpp', '_ZN6Status16missionCompletedEbbx')
dat = os.path.join(SCRATCH, 'flat3.profdata')
run([PROFDATA, 'merge',
     '_work/reconstructions/status_instr_flat_base3_w10214.proftext',
     '-o', dat])
obj = os.path.join(SCRATCH, 'flat3.obj')
compile_use(p.meta['cmd'], dat, obj, PREFIX_OFF)
so = os.path.join(SCRATCH, 'flat3.so')
link_with([(obj_path(p.tu), obj)], so)
print(measure(so, p.mangle))   # raw 1376 aligned 674 size 1432
PY
```

`profgen_w10314.py` is the committed generic harness. It differs from the
w10214 driver in one important way: the gold link command names objects as
`CMakeFiles/gof2.dir/./src/...`, so the replacement key must include the
`./` (the w10214 driver hard-coded the Status path). Without it the link
silently keeps the unprofiled object and every profile looks neutral.

## 4. Open questions

1. **A profile that reproduces the heuristic layout.** The original's layout
   is the no-profile layout; a real PGO build would need weights that happen
   to give the same fallthrough choices. The flat Status vector is one such
   coincidence. For these four functions the search space is large and no
   single-counter variant reached it.
2. **Remove the `__builtin_expect` before profiling `OnTouchEnd`.** The
   expect pins the pre-dock branch; a source variant without it (8,084 B
   plain) plus a profile could move the shared dock block to the
   `isDockingToAsteroid` site (the original's placement). This is a source
   experiment, not a build flag.
3. **Sample PGO** sets `function_entry_count` unconditionally and grows the
   body (w10117/w10220), so it is even less likely to keep the size exact.

## 5. Files touched

* `_work/reconstructions/profgen_w10314.py` (generic harness).
* `_work/reconstructions/pgo-synthetic-profile-wave157-w10314.md` (this note).
* No `src/`, `CMakeLists.txt`, baseline, allowlist or doc change.
