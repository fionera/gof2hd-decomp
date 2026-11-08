# Status::missionCompleted — nested / two-level dispatch sweep (wave 162, w10815)

Status: **analysis only, no `src/` win landed**. Worktree `wt/w10815`, base
master `29248c1d0` (wave 161). Scratch `/dev/shm/w10815`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Gate on entry and exit (source restored): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370.

Target `_ZN6Status16missionCompletedEbbx` (1432 B).  Baseline **raw 722 /
aligned 320 / 1432 B**.  Reference plateau (post-optimisation IR `!prof`
injection, default 1 / G1 4 / G2 3 / G3 2) **raw 558 / aligned 47 / 1432 B**.

Read first: `wave161-merge-tests.md`,
`status-switch-nested-wave161-w10715.md`,
`status-switch-profile-wave160-w10615.md`.

## 0. Bottom line

No source-only shape tried this wave is both size-exact (1432 B) and a raw
win.  The plateau is still IR-injection-only.  Three new structural results:

1. **A two-level `switch` on the same value does NOT merge into one table.**
   Splitting `G1+G2` / `G3` into two sequential `switch (mtype)` statements
   produces **three** TBH tables (150..189, 8..14, 156..172) and 1504 B.
2. **A computed coarse-class dispatch adds a second table** (1484 B) even
   though its instruction-index alignment improves to **270** (from 320).
3. **The nested-switch merge weights are geometric with ratio = n_inner+1**,
   not arithmetic.  The merge formula is
   `outer_case *= T_inner`, `inner_case *= outer_default`, where
   `T_inner = sum(inner case weights) + default`.  With the group sizes
   n_G2=12, n_G3=10 this gives **G1=143, G2=11, G3=1** — ratio ~11-13,
   which lands in the 1384-B cell, not 1432.  To reach ratio 1.5 the inner
   switch would need ~0.5 cases, impossible for an integer merge.

## 1. The 1432-B cell is narrow (IR-injection confirmation)

`/dev/shm/w10815/inj.py groups a b c d e` (same vector layout as
`pgo_switchweight_w9920.py`):

| vector | raw | aligned | size | table-0 body order |
|---|---:|---:|---:|---|
| **4/4/4/3/2** | **558** | **47** | **1432** | **source order (target)** |
| 8/8/8/6/4 | 558 | 47 | 1432 | source order |
| 17/17/17/13/9 | 558 | 47 | 1432 | source order |
| 100/100/100/99/98 | 558 | 47 | 1432 | source order |
| 4/4/4/3/3 | 722 | 320 | 1432 | ascending (baseline) |
| 4/4/4/2/1 | 1174 | 200 | 1444 | source order |
| 4/4/4/3/1 | 1265 | 342 | 1380 | source order |
| 4/4/4/2/2 | 1151 | 365 | 1428 | source order |
| 4/4/4/4/2 | 1252 | 332 | 1372 | source order |
| 5/5/5/4/2 | 1265 | 342 | 1380 | source order |
| 6/6/6/4/2 | 1055 | 191 | 1444 | source order |

So the *order* is right for every strictly-decreasing vector, but only the
mild arithmetic progression (or a nearly-flat geometric one) keeps 1432 B.
The raw/aligned numbers are a direct function of the branch distances the
layout produces.

## 2. Nested-switch merge: measured weights and layout

The merge of an inner switch into the outer's default multiplies the outer
case weights by `T_inner` and the inner case weights by the outer default
weight (1).  Verified on a 2-level toy (`t2.cpp`): outer `expect(x,1)`,
inner `expect(x,5)` -> `default 1, case1 4008000, case2..4 2004,
case5 2000, case6..8 1`.

Real source shapes (all `src/` restored afterwards):

| shape | raw | aligned | size | table-0 body order |
|---|---:|---:|---:|---|
| baseline `switch (m->getType())` | 722 | 320 | 1432 | ascending |
| 2-level: outer `expect(mtype,167)`, default -> inner G3 | 1249 | 530 | 1384 | 167/174, **default**, 150..155,158,162,166,170,189,184,156..172 |
| 3-level: outer `expect(167)`, middle plain, inner plain | 1279 | 499 | 1384 | same as above |
| 3-level: outer `expect(167)`, middle `expect(150)`, inner plain | 1276 | 480 | 1380 | same as above |
| 3-level: all three `expect` | 1281 | 496 | 1380 | G1, default, G2, G3 |
| two sequential switches (G1+G2, then G3) | 1157 | 342 | 1504 | 3 tables |
| two-level dispatch (computed `cls` 0/1/2) | 1212 | **270** | 1484 | G3 behind a 2nd table |
| computed rank (`key = mtype+100` for G3) | 1199 | 326 | 1484 | 2 tables |

The 3-level outer-expect merge produces exactly the measured
`G1=143, G2=11, G3=1` vector (confirmed by dumping the optimised IR:
`!{branch_weights, 1, 286000, 143 x6, 11 x12, 1 x10}`).  Its layout is
*almost* the target: G2 then G3 is correct, but the loop-latch `default`
block is tied with G3 at weight 1 and is placed **second** (right after
167/174) instead of last, and 189/184 swap (both weight 11).  The 48-B
shortfall (1384 vs 1432) is the different branch-distance mix.

## 3. Why the arithmetic progression is not reachable

* `LowerExpectIntrinsic::handleSwitchExpect` is one-hot: one case 2000, all
  others 1, default 1 (or default 2000 when the expected value is not a case).
* `SimplifyCFG::FoldValueComparisonIntoPredecessors` on a plain if-chain
  produces **equal** weights (the chain is canonicalised to a switch with no
  `!prof`); with `__builtin_expect` it produces a geometric vector with
  ratio `LikelyBranchWeight/UnlikelyBranchWeight = 2000`.
* The nested-switch merge produces a geometric vector with ratio
  `T_inner = n_inner + 1` (11..13 for the real group sizes; 2 for
  single-case nesting -> 4/2/1 -> 1444 B).
* The 1432-B cell needs ratio <= ~1.5 (4/3/2, 5/4/3, 8/6/4, 17/13/9,
  100/99/98).  No integer source producer gives a fractional ratio.

## 4. Open question for wave 163

The nested merge is the closest source shape (G2 before G3, 1384 B).  The
remaining 48 B and the misplaced `default` need a shape where the loop-latch
block is *strictly colder* than G3 and where the 189/184 tie is broken in
value order.  A candidate not yet tried: put the loop-latch `default` in a
4th nested switch whose total weight is 1 (so it shifts to 0 under
`FitWeights`) while G3 stays at 1.

## 5. Files changed

* `_work/reconstructions/status-switch-nested2-wave162-w10815.md` — this note.
* No `src/`, `CMakeLists.txt`, baseline, allowlist or doc change.

Scratch: `/dev/shm/w10815/{fdiff.py,align_diff.py,tbh.py,inj.py,measure.sh,
gen_nested.py,gen_2level.py,gen_rank.py,Status.base.cpp}`.
