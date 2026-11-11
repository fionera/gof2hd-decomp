# Status::missionCompleted — grouped-weight cell + producer re-sweep (wave 172, w11717)

Status: **analysis only, no `src/` win landed**.  Worktree `wt/w11717`,
branch `wave172-w11717`, base master `41ef4c8b6`.  Scratch
`/home/debian/GalaxyOnFire2/scratch/w11717`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Target `_ZN6Status16missionCompletedEbbx` (1432 B) baseline **raw 722 /
aligned 320 / 1432 B**, `insn 529/527`.  Gate on entry: clean tree at
`41ef4c8b6` (byte_exact 4507, size 4524/4524, data 843/843, imports 207).

Read first: `status-432-producers-wave170-w11617.md`,
`status-432-switchweights-wave166-w11116.md`,
`status-missioncompleted-source-order-tbh-w3906.md`.

## 0. Bottom line

1. **Reproduced the wave-170 IR target exactly.**  Attaching
   `!prof !{!"branch_weights", i32 4, i32 4 x9, i32 3 x10, i32 2 x9, i32 1}`
   to the `switch i32 %52` in `Status.ll` (metadata order is
   *default first*) and relinking gives **raw 637 / aligned 59 / 1432 B,
   insn 527/527**.  This is the same 637/59 cell wave 170 reported.
2. **New: the exact original body order is reachable at 1432 B, not only at
   1436 B.**  Wave 170 concluded that the exact order needs a strictly
   decreasing per-case vector and therefore costs 4 B more.  A search over
   grouped vectors `{default, 167/174, low(0/8/11/13/14), A(150..189 win
   group), B(156..172 misc group)}` finds many vectors that give the
   **exact** original body order at **1432 B**, e.g.
   `{d=1, p=2, low=5, a=4, b=3}`, `{d=1, p=6, low=2, a=2, b=1}`,
   `{d=1, p=3, low=4, a=3, b=2}`, `{d=2, p=2, low=4, a=3, b=2}`.
   The ordering condition is `weight(low) >= weight(A) > weight(B)`;
   167/174's shared body doubles its successor frequency, so it lands first
   even at a modest weight.  **The 1432/1436 split in wave 170 was an
   artifact of the strictly-decreasing vector, not of the exact order.**
3. **No source-only producer of those weights was found.**  The only
   source-level `!prof` producer on a `switch` in clang 7 is
   `__builtin_expect(value, C)` (`LowerExpectIntrinsic::handleSwitchExpect`),
   which is one-hot `[2000 at C, 1 elsewhere]`.  All 29 case values were
   compiled: sizes 1356..1456, body order `C, default, ascending`; only
   `C=14` is 1432 B and its order is scrambled.  Body-level
   `__builtin_expect` only shifts 184/189 and the size (1428 without them).
4. The three requested shapes were re-measured; none is 1432 B **and** a raw
   win (see §2).  The rank forms are raw-worse than the 722 baseline even
   when their body order is exact.

## 1. Measurements

`raw` = same-address differing bytes (`tools/fn_diff.py`); `aligned` =
instruction-index alignment; `size` = function bytes.

| shape | raw | aligned | size |
|---|---:|---:|---:|
| **baseline `switch (m->getType())`** | **722** | **320** | **1432** |
| IR `!prof` default=4 / 4x9 / 3x10 / 2x9 / 1 (wave-170 cell) | **637** | **59** | **1432** |
| IR grouped `{d1,p2,low5,a4,b3}` (exact body order) | (not linked) | — | 1432 |
| rank if-chain `rank = t-150+20+(isB?34:0)`, this implementation | — | — | 1496 |
| rank if-chain, wave-170 implementation | 1312 | 442 | 1436 |
| duplicated 167->174 fallthrough (wave 170) | 1215 | 370 | 1452 |
| `(type, flag)` pair `switch(t + (atStation?40:0))` (wave 169) | 1447 | 436 | 1560 |

Body-order decode of the original (target):
`167/174, 8, 11, 13, 14, 150,151,152,153,154,155,158,162,166,170,184,189,
156,157,160,163,164,165,168,169,171,172, default`.
Baseline decode: `167/174, 8,11,13,14, 150..158,160,162,163,164,165,166,
168,169,170,171,172,184,189, default` (ascending).

### 1.1 The grouped-weight search

Weights were injected as `!prof` on the `switch` in `Status.ll`, compiled
back to the `.o` with the project flags, and the TBH tables decoded
(`target = base + 2*entry`).  Exact-order hits at 1432 B (subset):

```
{d=1,p=2,low=5,a=4,b=3}   {d=1,p=6,low=2,a=2,b=1}
{d=1,p=3,low=4,a=3,b=2}   {d=1,p=4,low=4,a=3,b=2}
{d=2,p=2,low=4,a=3,b=2}   {d=2,p=6,low=4,a=3,b=2}
```

where `d` = default, `p` = 167/174, `low` = {0,8,11,13,14},
`A` = {150,151,152,153,154,155,158,162,166,170,184,189},
`B` = {156,157,160,163,164,165,168,169,171,172}.
Sizes 1372/1380/1444 also occur for neighbouring ratios; 1432 is one of the
four size cells the layout can take.

### 1.2 One-hot `__builtin_expect` sweep

`switch (__builtin_expect(m->getType(), C))` for each of the 29 case values:
all produce `C, default, ascending` and sizes 1356..1456 (C=14 -> 1432 but
scrambled).  None matches the target order.  Adding an
`__builtin_expect(...,1)` to the loop condition does not move the default
(1384 B, same `167, default, ascending`).

## 2. Requested source shapes

* **Hand-written if-chain over a precomputed rank.**  Reimplemented with
  `isB = t >= 156 && t <= 172 && (t & 3) != 2`,
  `rank = t - 150 + 20 + (isB ? 34 : 0)`, `switch (rank)`: **1496 B** in this
  implementation (the negative ranks of the low cases force a second table);
  wave 170's variant is **1436 B / raw 1312**.  Both are raw-worse than 722.
* **Duplicated case bodies with different fallthroughs.**  Splitting
  167/174 is tail-merged (722/320/1432); making 167 fall through to a
  duplicated 174 body is semantically identical but **1452 B / raw 1215**
  (wave 170).  Both fallthroughs 1484 B.
* **`switch` over a `(type, flag)` pair.**  `switch (t + (atStation ? 40 : 0))`
  with bodies duplicated for both flag values: **1560 B / raw 1447**
  (wave 169).  The key expression adds a select and doubles the table.

## 3. Why no source producer

`LowerExpectIntrinsic::handleSwitchExpect` (LLVM 7) only recognises a
`switch` whose condition is a direct `llvm.expect` call, and emits one-hot
weights `LikelyBranchWeight=2000` / `UnlikelyBranchWeight=1`.  A chain of
`if (__builtin_expect(t == c, 1))` does preserve `!prof` through
`FoldValueComparisonIntoPredecessors`, but the multiplication is geometric
(`2000^k`) and overflows uint64 for a 29-arm chain (wave 166: scrambled,
1420 B).  `SimplifyBranchOnICmpChain` (OR-chains) sorts the values and
deliberately drops branch weights (TODO in LLVM 7), so grouped OR-chains
cannot carry the 5/4/3 grouping either.

The grouped vector is therefore still a post-optimisation/profile artifact.
A source-only landing would need a clang knob that sets per-edge switch
weights (e.g. `-mllvm -likely-branch-weight` on a one-hot expect, which
gives only one hot case, not three groups) or C++20 `[[likely]]` on cases.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11717
ulimit -v 12000000
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN6Status16missionCompletedEbbx
```

Scratch (`/home/debian/GalaxyOnFire2/scratch/w11717/`): `Status.ll`,
`inj2.py` / `inj3.py` (weight injection), `mkobj_ll.sh`, `fast.py`
(.o section + TBH decode), `search2.py` (grouped-vector search),
`mkrank.py` / `mkchain*.py` / `reorder.py` (source-shape generators),
`linkcmd.sh` (manual gold link, needed because ninja re-runs the compile).

## 5. Files changed

* `_work/reconstructions/status-432-producers-wave172-w11717.md` — this note.
* No `src/`, `CMakeLists.txt`, baseline, allowlist or doc change.
