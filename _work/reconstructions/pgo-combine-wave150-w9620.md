# Build/PGO: w9515 profile + section-prefix off, eager entry search (wave 150, w9620)

Status: **harness + note, no `src/` / `CMakeLists.txt` change**. Worktree
`wt/w9620`, base master `7eed2966e` (wave 149). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000). Scratch `/dev/shm/w9620`.

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370.

Target: `_ZN6Status16missionCompletedEbbx` (original `0xabf38`, **1432 B**).
Baseline: raw same-address diff **722**, instruction-index aligned diff **320**.

Mission: combine w9518 (`-mllvm -profile-guided-section-prefix` is ON by
default and relocates a profiled function) with w9515 (hand-built rank-derived
profile `base=12, c28=28, latch(6,7,12)=0` reproduces both original switch
tables at 1428 B but sinks the eager `shortMission`/`longMission` entry). Find
the non-case weight that keeps the entry eager at exactly 1432 B, and test
whether a per-TU `-fprofile-instr-use` on `Status.cpp` alone leaves the rest of
the TU byte-exact.

## 0. Bottom line

1. **The w9515 profile with section-prefix OFF gives 1428 B, raw 1349,
   aligned 654, eager entry = NO** (`other .text` sections 145/145 identical).
   Section-prefix off does not change the target's size (1428 either way); it
   only keeps the function at its TU-order address `0xabf38`.
2. **No uniform non-case `base` reaches 1432.** Sweeping `base = 0..80` with
   `c28=28`, `latch=0`: sizes are 1368, 1396, 1424, 1388, …, **1428 at
   base 11/12**, 1424 at 13..16, 1412 from 17 on. None is 1432 and none is
   eager.
3. **No single non-case counter reaches eager.** Sweeping every non-case
   counter `k` with value 0 and 64 (178 compiles) finds **7 size-1432 hits**
   (`k=19 v=0`, `k=23 v=0`, `k=44 v=64`, `k=46 v=0`, `k=81 v=64`,
   `k=82 v=0`, `k=83 v=0`), but **all 7 still sink the entry**. Best aligned
   among them: `k=44 v=64` raw 1377 / aligned 619; `k=81/83 v=64` raw 1363 /
   aligned 624.
4. **The sink is `LoopSink`, and it runs whenever the function has profile
   data.** `lib/Transforms/Scalar/LoopSink.cpp` (LLVM 7.0.2):
   `if (!Preheader->getParent()->hasProfileData()) return false;`. A full-TU
   `-fprofile-instr-use` always sets `!{!"function_entry_count"}`. Disabling
   the pass with `-mllvm -max-uses-for-sinking=0` restores the eager IR, but
   the function then shrinks to **1404 B** (base 12; best 1424 at base 13/14),
   never 1432.
5. **Why the entry sinks: the switch-case blocks are cold.** The BFI loop
   scale saturates at ~32 (`counter 4 = 12 -> 9.96`, `1000 -> 31.04`,
   `100000 -> 31.99`), and the switch distributes that over 29 cases, so the
   shared-boolean use blocks (`case 164`, `165`, `169`, `171`, `172`) sit at
   `0.5..0.8` of the preheader frequency (`1.0`). `findBBsToSinkInto` therefore
   sinks the `shortMission`/`longMission`/`dockedAtStation`/`notAtStation`
   computations into those blocks. The source-order rank weights are what make
   the use blocks cold, so the source-order switch and the eager entry are
   mutually exclusive under a full-TU profile.
6. **Per-TU object is byte-exact; the linked `.so` is not.** For every variant
   the other 145 `.text.*` sections of `Status.cpp.obj` are byte-identical to
   the canonical object. But a 1432 B hit (`k=23 v=0`) still gives
   `byte_exact 4493` / `linked_exact 4507`: 14 downstream functions differ by
   one immediate byte (literal-pool/relocation artefact, the w9518 effect).
   The 1428 B w9515 base shifts everything after the target by 4 B and gives
   `byte_exact 3044` / `linked_exact 4507`.
7. **No clean profile exists**, so `CMakeLists.txt` is unchanged. The only
   1432 B + eager shape remains w9305's switch-only `!prof` injection (no
   `function_entry_count`, so `LoopSink` does not run).

## 1. Measurements (real NDK r18b compile + gold link + `verify.py`)

`byte_exact` is the global count from `tools/verify/verify.py`; `raw` /
`aligned` / `size` are the target function.

| variant | flags | raw | aligned | size | eager | other `.text` | byte_exact |
|---|---|---:|---:|---:|---|---:|---:|
| master | — | **722** | **320** | **1432** | **yes** | — | **4507** |
| w9515 `base=12,c28=28,latch=0`, prefix off | `-mllvm -profile-guided-section-prefix=false` | 1349 | 654 | 1428 | no | 145/145 | 3044 |
| w9515 `base=2`, prefix off | + same | 1378 | 851 | 1424 | no | 145/145 | — |
| w9515 `base=11`, prefix off | + same | 1349 | 654 | 1428 | no | 145/145 | — |
| single `k=23 v=0` | + same | 1367 | 636 | **1432** | no | 145/145 | 4493 |
| single `k=44 v=64` | + same | 1377 | **619** | **1432** | no | 145/145 | — |
| single `k=81 v=64` | + same | 1363 | 624 | **1432** | no | 145/145 | — |
| w9515 `base=12` + `-max-uses-for-sinking=0` | + `-mllvm -max-uses-for-sinking=0` | 1329 | 635 | 1404 | **yes** | 145/145 | — |
| w9515 `base=13` + `-max-uses-for-sinking=0` | + same | 1345 | 657 | 1424 | **yes** | 145/145 | — |

The `base=0..80` sweep (c28=28, latch=0) is in §3 of the harness output; the
full table is reproducible with `sweep-base 0 80`.

## 2. The LoopSink mechanism (new this wave)

`-mllvm -print-after-all` on the profiled TU shows the shared booleans leave
the preheader between the `Loop-Closed SSA Form Pass` and
`Remove redundant instructions` (InstSimplify), i.e. in the `Loop Sink` loop
pass. The `-mllvm -print-after=loop-sink` dump shows the preheader reduced to

```
%5 = getelementptr inbounds %class.Status, %class.Status* %0, i32 0, i32 115
br label %6
```

with the `xor`/`icmp`/`and` chain cloned into the case blocks. `-mllvm
-max-uses-for-sinking=0` (which makes `sinkInstruction` return false for any
non-empty use set) restores the eager IR and the `rsbs`/`sbcs` entry in the
assembly.

`-mllvm -print-bfi -mllvm -print-bfi-func-name=_ZN6Status16missionCompletedEbbx`
gives the block frequencies. The loop header saturates at `31.99` (the
`BlockMass`/`ScaledNumber` loop-scale precision limit), and the case blocks
are `0.5..0.8` of the entry (`1.0`):

```
entry 1.0;  loop header/body 31.99;
case 164 0.652;  case 165 0.00065;  case 169 0.546;
case 171 0.507;  case 172 0.546
```

`findBBsToSinkInto` starts from the use block and only refuses to sink when
`adjustedSumFreq(use blocks) > freq(preheader)`; the use blocks are below 1.0,
so it sinks. Making the loop hotter does not help because the scale saturates
at 32 (`counter 4`: 12 -> 9.96, 20 -> 13.28, 32 -> 16.74, 64 -> 21.77,
1000 -> 31.04, 100000 -> 31.99).

## 3. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9620
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6

python3 _work/reconstructions/pgo_combine_w9620.py meta
python3 _work/reconstructions/pgo_combine_w9620.py run 12 28 0        # 1428, raw 1349, aligned 654
python3 _work/reconstructions/pgo_combine_w9620.py sweep-base 0 80    # no 1432
python3 _work/reconstructions/pgo_combine_w9620.py sweep-single       # 7 size-1432, none eager
python3 _work/reconstructions/pgo_combine_w9620.py run 12 28 0 --extra \
  -mllvm -max-uses-for-sinking=0                                      # eager, 1404
python3 _work/reconstructions/pgo_combine_w9620.py verify /dev/shm/w9620/run/run_12_28_0.so
```

## 4. Open questions

1. The 14 downstream single-byte diffs of the size-1432 profile build are
   still unexplained (object `.text.*` sections are identical). If they can be
   removed, a 1432 B profile build would give `byte_exact 4507`; the eager
   entry would still need `LoopSink` disabled or a source shape that keeps the
   shared booleans out of the preheader.
2. A two-counter non-case search (e.g. `counter 4` + `counter 0`) was not
   exhausted; the single-counter sweep is complete and negative.
3. A profile with the switch weights but **without** a `function_entry_count`
   (so `hasProfileData()` is false) would keep the eager entry; only
   `-fprofile-sample-use`/`-fprofile-instr-use` always add it, so this points
   back to w9305's switch-only `!prof` injection.
