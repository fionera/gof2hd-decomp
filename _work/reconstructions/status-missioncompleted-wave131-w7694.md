# `Status::missionCompleted` — wave 131 (w7694): the A-run/B-run body order is a
# **switch-edge frequency effect**, and `!prof` weights A=3 / B=2 / low=4 /
# default=1 reproduce the original order **and the exact 1432 B size**; the
# if/else shape is a no-op (same switch), the two-switch shape gets the order
# but costs +72 B

Status: **note-only** (no source change landed; gate kept at 71.1 %, 1432 B).
Worktree `wt/w7694`, base master `7c6933c2d` (wave-130b snapshot, byte 4506 /
linked 4506).  Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 r328903 / LLVM `1d739ffb`, `-Oz`,
image base 0x10000).

Target: `_ZN6Status16missionCompletedEbbx` (`Status::missionCompleted(bool,
bool, long long)`), original `0xabf38`, size **1432 B**.

| | baseline (HEAD `7c6933c2d`) |
|---|---:|
| function score | **71.1 %** (523 vs 524 normalised insns) |
| function size | 1432 B |
| gate byte_exact | 4506 |
| gate linked_exact | 4506 |
| data_exact | 843/843 |
| imports | 207 |
| lint / drift | CLEAN / 0 |

## 0. Summary (new results this pass)

1. **The A-run/B-run order is a switch-edge *frequency* effect, not the MBP
   fallback.**  Hand-written `!prof branch_weights` on the single
   `switch (m->getType())` reproduce the original body order **exactly** and
   keep the function at **1432 B** when the weights are

   ```
   A     = {150,151,152,153,154,155,158,162,166,170,184,189}  -> 3
   B     = {156,157,160,163,164,165,168,169,171,172}          -> 2
   low   = {8,0,11,13,14}                                     -> 4
   default                                                    -> 1
   ```

   The resulting body order is
   `167/174, 150 151 152 153 154 155 158 162 166 170 184 189,
    156 157 160 163 164 165 168 169 171 172, default`
   i.e. the original, and the prologue register allocation matches the
   original exactly (`this`=r6, `i`=fp, `atStation`=r8, `m`=sl, no spill).
   This is the first weight set found that gives both the order **and**
   1432 B (see §1 for the four small in-body deltas) (wave 129's set gave 1444 B; wave 125's 1436 B).
2. **The `if/else` shape is a no-op.**  Rewriting the whole switch as a
   27-arm `if (mtype == N || mtype == M) ... else if ...` chain compiles to
   the *same* single switch (two TBHs, 1432 B) and the *same* value-order
   body layout.  clang's `SimplifyCFG` folds the chain back into a
   `SwitchInst` before ISel, so the MBB order is unchanged.
3. **Source-level case order is irrelevant** (re-reproduced): sorting the
   `case` labels to value order keeps 1432 B and the value-order layout.
4. **The two-switch shape gets the order but cannot hit 1432 B.**  A cached
   `int mtype` + two sequential switches is **1504 B** (+72); using a field
   load (`m->id`) for the second switch is **1448 B** but changes the first
   dispatch to a 25-entry table (150..174), so it is not faithful.
5. **The MBP fallback would also give the source order** (MBB order = source
   order = A-then-B), but stock clang 7.0.2 never reaches it: the high
   switch's 24 successors are all pushed to `BlockWorkList` at equal
   frequency, so `selectBestCandidateBlock` drains them in jump-table
   (ascending value) order.  The `!prof` weights are the real mechanism.

## 1. The `!prof` experiment (the decisive result)

The current `src/game/mission/Status.cpp` was compiled to optimised IR with
the project's exact flags (`-Oz -g0`), the `switch i32 %52` in
`_ZN6Status16missionCompletedEbbx` was given

```llvm
switch i32 %52, label %380 [ ... ], !prof !9999
!9999 = !{!"branch_weights", i32 1, i32 3, i32 3, i32 4, i32 4, i32 4,
          i32 4, i32 4, i32 3, i32 3, i32 3, i32 3, i32 3, i32 3, i32 3,
          i32 3, i32 3, i32 3, i32 3, i32 3, i32 2, i32 2, i32 2, i32 2,
          i32 2, i32 2, i32 2, i32 2, i32 2, i32 2}
```

(one weight per `SI.cases()` entry in IR order plus the default; A=3, B=2,
low=4, default=1), and the IR was compiled with the NDK clang 7.0.2 backend
(full pipeline, not `-disable-llvm-passes`).  Result:

```
size 0x598 (1432 B)
high TBH bodies:
  167/174 0xfd
  150 0x1d7  151 0x1dd  152 0x1e3  153 0x217  154 0x21d  155 0x235
  158 0x247  162 0x29d  166 0x303  170 0x371  184 0x37f  189 0x3b9
  156 0x3f5  157 0x41f  160 0x451  163 0x47f  164 0x49b  165 0x49f
  168 0x4a7  169 0x4c3  171 0x4c7  172 0x4e1
  default 0x521
```

The original's offsets are `0xfe, 0x1d8, 0x1de, 0x1e4, 0x218, 0x21e,
0x236, 0x248, 0x2a4, 0x30a, 0x36e, 0x37c, 0x3b6, 0x3f2, 0x41c, 0x450,
0x47e, 0x49a, 0x49e, 0x4a6, 0x4c4, 0x4c8, 0x4e2, 0x522`.  **The order is
identical and most bodies land on the exact original offset** (the whole
low group, 150–155, 158, 162, 167/174, 169, 171, 172, default).  Four
bodies differ by a few bytes — 158 −6, 166 +10, 157 −2, 168 −2 — which
sum to zero, so the total size is still exactly 1432 B.  The prologue is
byte-identical: `mov r6,r0; mov.w fp,#0; mov r8,r1; ...; str r6,[sp,#16]`.
The four body deltas are inside-body codegen fallout of the weights (the
`__builtin_expect` hints already in cases 157/166 interact with the switch
weights), not a different body order.

A weight sweep (A,B,low ∈ 1..8, default=1) shows the size is very sensitive
to the weights: `(1,1,1)` -> 1432 B value order, `(3,2,4)` -> **1432 B exact
order**, `(2,1,3)` -> 1444 B, `(2,1,2)` -> 1372 B, `(3,1,3)` -> 1380 B.
So the original is consistent with a real `-fprofile-use` build whose
relative case frequencies were A:B:low:default = 3:2:4:1.  The project's
documented build flags contain no PGO, which is why the plain source cannot
reproduce the layout.

### Why `__builtin_expect` cannot express it

`LowerExpectIntrinsic::handleSwitchExpect` (LLVM 7.0.2, same as 7.0.0)
emits `getNumCases()+1` weights with **exactly one** successor set to
`LikelyBranchWeight` (2000) and the rest `UnlikelyBranchWeight` (1).  It
cannot encode A > B.  It is also rejected for this switch because
`getNumCases()` = 29 but `getNumSuccessors()` = 27 (167/174 and 0/11 share
bodies), so `calcMetadataWeights`' `getNumSuccessors()+1` check fails.
Measured `switch (__builtin_expect(m->getType(), K))` sizes for every
case value: only K=14 keeps 1432 B, but it peels case 14 and shrinks the
high table to a non-original shape.

## 2. `if/else` shape (control)

The switch was mechanically rewritten as

```cpp
int mtype = m->getType();
if (mtype == 167 || mtype == 174) { ... }
else if (mtype == 8) { ... }
... 27 arms ...
```

(top-level `break;` removed, inner-loop `break;` kept).  Result: **1432 B,
two TBHs, identical value-order body layout** — `SimplifyCFG` folds the
chain back into the same `SwitchInst`.  So the `if/else` shape is not the
original source shape (or at least cannot change the layout).

## 3. Two-switch shapes (order yes, size no)

| source shape | size | high-table body order |
|---|---:|---|
| baseline single switch | 1432 | value order |
| `if/else` chain | 1432 | value order |
| `case` labels sorted to value order | 1432 | value order |
| cached `int mtype` + two switches (A then B) | **1504** | **exact original** |
| `switch(m->getType()){A} switch(m->id){B}` | 1448 | order but first table shrinks to 150..174 |
| `switch(m->id){A} switch(m->id){B}` | 1448 | same |
| nested `default: switch` | 1432 | value order (folded) |
| first switch = all cases, second = B (cached) | 1504 | exact original (second dispatch survives) |
| first switch = all cases, second = B (`m->id`) | 1772 | B bodies tail-duplicated |

The 1504 B two-switch form reproduces the original order because the B
bodies get a second unplaced predecessor (the redundant second switch) and
are therefore pushed only after the A run.  But the second dispatch
(`sub/cmp/bhi/tbb` + 18-byte table, ~28 B) and the extra live `mtype`
(which takes `fp`, pushes `i` to r8 and `atStation` to r9, and spills
`this` to `[sp,#16]`) cost 72 B.  No plain two-switch shape tested reaches
1432 B.

## 4. MBP fallback vs frequency

Wave 129 proved `MBP_FORCE_FALLBACK` / `MBP_SKIP_JT_PUSH` /
`MBP_SORT_SBCB` reproduce the original order in a local LLVM 7.0.0 build.
That is because the fallback order is MBB order = source order = A-then-B.
But the fallback is only reached when `BlockWorkList` is empty, and for the
high switch `markBlockSuccessors` pushes all 24 distinct successors
(`UnscheduledPredecessors` = 1 each), so `selectBestCandidateBlock` drains
them in jump-table (ascending value) order.  The `!prof` result shows the
original did not need the fallback: a frequency gradient A > B is enough
to split the worklist into the two ascending runs, and it also reproduces
the exact 1432 B size.  **The frequency explanation is the correct one.**

## 5. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6   # RATCHET PASS, byte 4506

# optimised IR with the project flags (-g0 for a compact .ll)
/dev/shm/w7694/ccll.sh src/game/mission/Status.cpp /dev/shm/w7694/cur.ll

# inject the 3/2/4/1 switch weights (see §1) and compile with the NDK backend
python3 /dev/shm/w7694/one.py            # writes one.ll, one.o
python3 /dev/shm/w7694/bodies.py one.o   # size 0x598, exact original order

# body-order decoder for a single-function .o section
python3 /dev/shm/w7694/bodies.py <obj>   # TBH entries -> body offsets

# if/else and two-switch generators
python3 /dev/shm/w7694/mkif.py           # v_if.cpp
python3 /dev/shm/w7694/mktwo.py cache    # v_cache.cpp  (1504 B)
python3 /dev/shm/w7694/mktwo.py id       # v_id.cpp     (1448 B)
```

Scratch (not committed): `/dev/shm/w7694/` — `cc.sh`, `ccll.sh`,
`bodies.py`, `bodies2.py`, `bodies3.py`, `allbodies.py`, `mkif.py`,
`mktwo.py`, `mktwo2.py`, `mkfull.py`, `mkfull2.py`, `mknest.py`,
`mkexp.py`, `mkprof.py`, `one.py`, `sweep_prof.py`, `sweep2.py`,
`sweep_exp.py`, `cur.ll`, `one.ll`, `one.o`, `v_*.cpp`, `v_*.o`.

## 6. Open question

The source is already the faithful single switch in the original case
order; the residual is purely the missing PGO switch-edge weights
(A:B:low:default = 3:2:4:1).  A byte-identical result needs either
(a) a source-level construct that emits multi-case switch weights (none
found in C++11 — `__builtin_expect` sets exactly one weight), or
(b) accepting a PGO/`!prof` build input for this TU.  Do **not** re-add the
`.missioncase` rank table: it changes `.rodata` and is not original data.
