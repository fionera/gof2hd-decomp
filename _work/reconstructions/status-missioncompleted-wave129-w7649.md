# `Status::missionCompleted` — wave 129 (w7649): the original body order is the
# MachineBlockPlacement **fallback** (MBB/source order), not a PGO weight effect;
# it is reproduced exactly by `MBP_SKIP_JT_PUSH` / `MBP_SORT_SBCB` /
# `MBP_FORCE_FALLBACK`, and by `!prof` weights (but `!prof` costs +12 B)

Status: **note-only** (no source change landed; gate kept at 71.1 %, 1432 B).
Worktree `wt/w7649`, base master `fee528cbe` (wave-128 snapshot, byte 4506 /
linked 4506).  Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 r328903 / LLVM `1d739ffb`, `-Oz`,
image base 0x10000).

Target: `_ZN6Status16missionCompletedEbbx` (`Status::missionCompleted(bool,
bool, long long)`), original `0xabf38`, size **1432 B**.

| | baseline (HEAD `fee528cbe`) |
|---|---:|
| function score | **71.1 %** (523 vs 524 normalised insns) |
| function size | 1432 B |
| gate byte_exact | 4506 |
| gate linked_exact | 4506 |
| data_exact | 843/843 |
| imports | 207 |
| lint / drift | CLEAN / 0 |
| `.text` differing bytes | 85,475 |

## 0. New hypothesis tested

The wave-128 handoff proposed that the high-TBH body order is a
switch-edge-frequency / PGO effect: the original lays the 22 high case bodies
out in **source order** (A run then B run), while clang lays them out in
**ascending case-value order**.  This pass tested that directly against the
project's instrumented LLVM 7.0.0 tree (`/home/debian/llvm7build`, which has
`MBP_RAW`, `MBP_SKIP_JT_PUSH`, `MBP_SORT_SBCB`, `MBP_FORCE_FALLBACK`,
`MBP_COLD_JT` hooks) and against the real NDK clang 7.0.2 backend with
hand-written `!prof` weights.

**Result:** the hypothesis is half right and half wrong.

1. The original layout is *exactly* the MBP **fallback order** (= MBB order =
   IR basic-block order = source order).  Three independent tie-break
   mechanisms reproduce it byte-for-byte in the body sequence:
   `MBP_SKIP_JT_PUSH=1`, `MBP_SORT_SBCB=1`, and `MBP_FORCE_FALLBACK=1`.
2. It is **not** a PGO weight effect in the sense the handoff assumed: the
   stock worklist carries all 22 high bodies at *identical* block frequency
   (`f=8256`), so the only reason they drain in value order is the worklist
   insertion order (= jump-table `Table` order = ascending case value).
   `selectBestSuccessor` peels `167/174` first (f=16512, 2 case values), then
   every remaining body ties and `selectBestCandidateBlock` keeps the
   first-in-worklist, i.e. ascending value.
3. `!prof` weights *can* reproduce the exact order with the real NDK clang,
   but they change tail-merging of the low group and cost **+12 B**
   (1444 B), so they are not the original mechanism either.

## 1. Baseline evidence (measured this pass)

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
cmake --build cmake-build-match --target check -j6
# verify: byte 4506  linked 4506  stubs 0  missing 0  extra 0  avg 99.94
# data:   exact 843 of 843 exported objects
# sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
# RATCHET PASS
```

Report row: `{"name":"_ZN6Status16missionCompletedEbbx","pct":71.1,
"osize":1432,"usize":1432}`; normalised instruction counts 523 (orig) vs 524
(ours).

## 2. MBP trace: the high worklist is value order at equal frequency

Compiled the current `Status.cpp` to optimised IR with the exact project flags
(`-Oz`, `-g0`) and ran the instrumented `llc` with `MBP_RAW=1`.  The
`missionCompleted` high jump-table worklist is the 22-line run at
`f=8256`:

```
RAW %bb.36 f=8256   (150)   RAW %bb.38 f=8256   (151)
RAW %bb.39 f=8256   (152)   RAW %bb.44 f=8256   (153)
RAW %bb.45 f=8256   (154)   RAW %bb.46 f=8256   (155)
RAW %bb.77 f=8256   (156)   RAW %bb.80 f=8256   (157)
RAW %bb.47 f=8256   (158)   RAW %bb.85 f=8256   (160)
RAW %bb.57 f=8256   (162)   RAW %bb.88 f=8256   (163)
RAW %bb.91 f=8256   (164)   RAW %bb.92 f=8256   (165)
RAW %bb.62 f=8256   (166)   RAW %bb.93 f=8256   (168)
RAW %bb.96 f=8256   (169)   RAW %bb.66 f=8256   (170)
RAW %bb.97 f=8256   (171)   RAW %bb.98 f=8256   (172)
RAW %bb.67 f=8256   (184)   RAW %bb.71 f=8256   (189)
```

`167/174` (`%bb.16`, f=16512) is absent because `selectBestSuccessor` already
peeled it as the hottest successor of the switch.  Every remaining body has
the same frequency, so `selectBestCandidateBlock`'s
`BestFreq >= CandidateFreq` tie-break keeps the first worklist entry.  The
worklist insertion order is `MachineBasicBlock::successors()` order, which
`SelectionDAGBuilder::buildJumpTable` fills in `Table` order (ascending
value).  There is no source-visible knob between "equal frequency" and
"ascending value".

## 3. Three tie-break hooks reproduce the original order exactly

Using the same `llc` on the same IR (body-order decoder below):

| env hook | high-body order | size (llc 7.0.0) |
|---|---|---:|
| none (stock) | 167/174 150 151 152 153 154 155 156 157 158 160 162 163 164 165 166 168 169 170 171 172 184 189 | 1412 |
| `MBP_SKIP_JT_PUSH=1` | **167/174 150 151 152 153 154 155 158 162 166 170 184 189 156 157 160 163 164 165 168 169 171 172** | 1416 |
| `MBP_SORT_SBCB=1` | **same exact original order** | 1412 |
| `MBP_FORCE_FALLBACK=1` | **same exact original order** | 1416 |

`MBP_SKIP_JT_PUSH` skips `BlockWorkList.push_back` for successors of a block
with `succ_size() > 20`; the switch then drains via
`getFirstUnplacedBlock`, which walks the function in **MBB order**.  The MBB
order after ISel is the IR basic-block order, which for this switch is the
source order — exactly the original layout.  `MBP_SORT_SBCB` sorts the
candidate worklist by MBB number and gives the same result.  So the original
layout is the **fallback**, not a frequency ranking.

This is new evidence: prior waves established that `MBP_SORT_SBCB` /
`MBP_FORCE_FALLBACK` reproduce the order, but wave 128 concluded the residual
"needs switch-edge frequencies (`!prof`/PGO) that C++11 cannot emit."  The
`MBP_SKIP_JT_PUSH` result shows the order is a **worklist-population**
effect, not a weight effect: the original did not push the high bodies to the
worklist at all.

## 4. `!prof` weights with the real NDK clang: exact order, +12 B

To check the PGO half of the hypothesis at the real compiler, the optimised
`base.ll` switch was given `!prof branch_weights` (successor order is
`default, 167, 174, 8, 0, 11, 13, 14, 150..189, 156..172`; weights
`default=1, 167/174=300, low=250, A=200, B=100`) and compiled with the NDK
clang 7.0.2 backend (`-Xclang -disable-llvm-passes`, so the already-optimised
IR is not re-optimised):

```
switch i32 %52, label %380 [ ... ], !prof !9999
!9999 = !{!"branch_weights", i32 1, i32 300, i32 300, i32 250, ... }
```

Result: **exact original body order**, size **1444 B**.  The extra 12 B are
in the low group: case 11's body grows from `0x0a` to `0x20` bytes because
the shared station-index comparison tail is no longer merged.  So the
original is not simply "this switch with A>B weights": the profile also
changes tail-merging, and no weight assignment tried restores 1432 B.
Wave 125's different weight set gave 1436 B — the same tail-merging effect,
different magnitude.

`__builtin_expect` cannot express this from C++11: it sets exactly one
switch successor weight to 2000 (`LowerExpectIntrinsic::handleSwitchExpect`),
leaving the rest at 1, so it cannot encode A > B.

## 5. Source-level case order (control)

Reordering the `case` labels in the source does not move the layout: the
jump-table successor list is always ascending value (`sortAndRangeify` sorts
`CaseCluster`s and `buildJumpTable` iterates `Table` in table order).  The
MBB order *does* follow source order, but the worklist is non-empty, so the
fallback is never reached.  This confirms waves 119–125/128: source order is
irrelevant while the worklist drains in value order.

## 6. Conclusion / open question

* The original's high-body order is exactly the MBP **fallback** order
  (MBB order = source order).
* Stock clang 7.0.2 cannot reach the fallback here because
  `markBlockSuccessors` pushes all 22 high bodies (each has exactly one
  unplaced predecessor, the switch) into `BlockWorkList`, and they all tie
  at `f=8256`.
* `!prof` weights reproduce the order but not the size (1444 B).
* Plain C++11 has no construct that empties the worklist or emits multi-case
  switch weights.

The remaining open question is therefore **not** "which source case order"
but "why did the original's `markBlockSuccessors` not push the high bodies".
The most likely answer is that the original was built with a profile whose
weights also changed tail-merging (the 12 B low-group difference), i.e. a
real `-fprofile-use` build.  Any future pass should try to reproduce the
*low-group* tail merging at 1432 B, not the high-group order.

## 7. Reproduction

```sh
# body-order decoder for an object file (also prints the full body map):
#   /dev/shm/w7649/order.py  <file.o>
#   /dev/shm/w7649/bodies.py <file.o>
# exact-flag compile (project flags):
#   /dev/shm/w7649/cc.sh   <in.cpp> <out.o>
#   /dev/shm/w7649/ccll.sh <in.cpp> <out.ll>     # -g0 optimised IR

# instrumented MBP trace (custom LLVM 7.0.0 tree):
R=/home/debian/GalaxyOnFire2/wt/w7649
$R/.cache/ndk/android-ndk-r18b/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++ \
  -target armv7-none-linux-androideabi16 -march=armv7-a -mthumb -mfpu=neon \
  -mfloat-abi=softfp -std=c++11 -Oz -g0 -ffunction-sections -funwind-tables \
  -S -emit-llvm -o /dev/shm/w7649/base.ll $R/src/game/mission/Status.cpp
/home/debian/llvm7build/build/bin/llc \
  -mtriple=thumbv7-none-linux-androideabi16 -mattr=+neon -float-abi=soft -O2 \
  -filetype=obj base.ll -o /dev/shm/w7649/base_llc.o
MBP_RAW=1 MBP_SKIP_JT_PUSH=1 /home/debian/llvm7build/build/bin/llc ... # order

# !prof variant with the real NDK backend:
python3 /dev/shm/w7649/mkprof.py        # writes prof.ll (weights above)
$R/.cache/ndk/android-ndk-r18b/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++ \
  -target armv7-none-linux-androideabi16 -march=armv7-a -mthumb -mfpu=neon \
  -mfloat-abi=softfp -std=c++11 -Oz -Xclang -disable-llvm-passes \
  -c prof.ll -o prof.o
/dev/shm/w7649/bodies.py prof.o          # size 1444, exact original body order
```

Scratch (not committed): `/dev/shm/w7649/` — `cc.sh`, `ccll.sh`, `order.py`,
`bodies.py`, `mkprof.py`, `base.ll`, `prof.ll`, `base.o`, `prof.o`,
`base_llc.o`, `skip.o`.
