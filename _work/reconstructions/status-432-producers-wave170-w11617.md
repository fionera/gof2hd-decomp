# Status::missionCompleted — rank/fallthrough/(type,flag) re-sweep + weight-vector probe (wave 170, w11617)

Status: **analysis only, no `src/` win landed**.  Worktree `wt/w11617`,
branch `wave169-w11617`, base master `50716a763`.  Scratch `/dev/shm/w11617`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Gate on entry and exit (full `check`): **RATCHET PASS**, byte_exact 4507,
linked_exact 4507, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370.  Target `_ZN6Status16missionCompletedEbbx`
(1432 B) baseline **raw 722 / aligned 320 / 1432 B**, `insn 529/527`.

Read first: `status-432-producers-wave169-w11517.md`,
`status-432-producers-wave168-w11317.md`,
`status-432-switchweights-wave166-w11116.md`,
`status-missioncompleted-wave117-w6513.md`.

## 0. Bottom line

The three requested producer classes were re-implemented and measured.  **None
is size-exact (1432 B) and a raw win.**  The new facts:

1. **The target body order is exactly the IR `!prof` weight vector
   `4/3/2` applied by *thirds of the IR case order*.**  The IR case order is
   the source order (`167,174,8,0,11,13,14,150,...,172`).  Injecting
   `branch_weights = [4 x10, 3 x10, 2 x9, 1]` on the existing switch
   reproduces the original body order at **1432 B, raw 637 / aligned 59**
   (`insn 527/527`).  This is the first weight vector found that reaches
   1432 B and improves raw; the wave-166 `4/3/2` grouping gave 558/47 but
   used a different group assignment.
2. A **strictly decreasing per-case vector** (each case a distinct weight in
   the original body order) gives the exact body order but **1436 B**
   (raw 1044 / aligned 214): the exact order forces one extra wide branch
   (body 8 grows 100 -> 104 B), so the function is 4 B over.
3. Re-introducing the wave-117 **rank table** (fixed-address `.rodata`
   permutation, `switch (MISSION_CASE[getType()])`) on the *current* bodies
   is **1436 B, raw 837 / aligned 180** — worse raw than the 722 baseline
   and 4 B over.  The rank indirection (`ldr/ldrb`) plus the shifted bodies
   costs more than the body order saves.
4. **Hand-written if-chain over a precomputed rank**: `rank = t - 150 + 20 +
   (isB ? 34 : 0)` with `isB = t >= 156 && t <= 172 && (t & 3) != 2` and a
   single `switch (rank)` is the smallest form: **1436 B, raw 1312 /
   aligned 442** (the `||`-chain form is 1436/1312 too; the range form is
   1440).  Nested low-switch + high-rank-switch is **1448 B**.  The rank
   computation and the split tables shift every body, so raw is worse than
   the plain switch even when the body order is right.
5. **Duplicated case bodies with different fallthroughs**: splitting the
   shared `167/174` and `0/11` bodies is tail-merged (exact 722 no-op).
   Making `167` fall through to `174` (or `0` to `11`) with the body
   duplicated is semantically identical but **1452 B** (raw 1215); both
   fallthroughs **1484 B** (raw 1234).  Not size-exact.
6. **`switch` over a `(type, flag)` pair** (`t + (atStation ? 40 : 0)`,
   bodies duplicated for both flag values) remains **1560 B / raw 1447**
   (wave 169), not size-exact.

The producer of the original layout is therefore **post-optimisation edge
weights** (a profile), not a source shape: the source-level dispatch is the
plain `switch (m->getType())` and its body order is whatever MBP does with
uniform weights.

## 1. Measurements

`raw` = same-address differing bytes (`tools/fn_diff.py`); `aligned` =
instruction-index alignment; `size` = function bytes.  All shapes keep the
29 case bodies verbatim; only the dispatch wrapper changes.

| shape | raw | aligned | size | body order |
|---|---:|---:|---:|---|
| **baseline `switch (m->getType())`** | **722** | **320** | **1432** | ascending |
| IR `!prof` thirds (4 x10 / 3 x10 / 2 x9 / default 1) | **637** | **59** | **1432** | original (low 150/151 interleaved) |
| IR `!prof` distinct decreasing per case | 1044 | 214 | 1436 | original exact |
| IR `!prof` `g4332` (167/174=4, low=3, A=3, B=2) | 1302 | 359 | 1372 | G1, G2, G3 |
| wave-117 rank table, current bodies | 837 | 180 | 1436 | original exact |
| rank `t-150+20+(isB?34:0)`, `isB` OR-chain | 1312 | 442 | 1436 | A-then-B |
| rank `t-150+20+(isB?34:0)`, `isB` bit test | 1312 | 442 | 1436 | A-then-B |
| rank, nested low switch + high rank switch | 1354 | 587 | 1448 | A-then-B |
| split `167/174` body | 722 | 320 | 1432 | identical (tail-merged) |
| split `0/11` body | 722 | 320 | 1432 | identical (tail-merged) |
| `167` fallthrough `174` (duplicated body) | 1215 | 370 | 1452 | reordered |
| `0` fallthrough `11` (duplicated body) | 1076 | 362 | 1464 | reordered |
| both fallthroughs | 1234 | 387 | 1484 | reordered |
| three group switches (if-chain over thirds) | 1313 | 787 | 1364 | G1, G2, G3 |
| `(type, flag)` pair (wave 169) | 1447 | 436 | 1560 | ascending |

## 2. The weight-vector probe

The existing switch is at `Status.ll` line 8999 (`switch i32 %52, label
%380`) with 29 cases in source order.  Attaching

```
!prof !{!"branch_weights", i32 4, i32 4, ... (10), i32 3, ... (10), i32 2, ... (9), i32 1}
```

and compiling the `.ll` back to the object (same flags) gives a 1432-B
function with the original body order and raw 637 / aligned 59.  The only
remaining body-order defect is that `150`/`151` (weight 4) are laid out
between `11` and `13`/`14`; the original has `11,13,14,150,151,152`.  A
strictly decreasing vector fixes that but costs 4 B (1436).

The 4-B size delta comes from body 8: in the exact-order layout its
`beq.w` to the pending-products loop is 4 B wider than in the baseline
layout (body 8 is 100 B in the original/baseline, 104 B in the exact-order
layout).

## 3. Why no source shape reaches the weights

`__builtin_expect` on the switch value is one-hot (2000/1/1).  An if-chain
that folds into a switch preserves `!prof`, but its weights are geometric
(`2^(n-1):...:2:1`) and overflow or reorder for 29 cases (wave 166).  A
three-group if-chain (`if (G1) switch(G1) else if (G2) switch(G2) else
switch(G3)`) does not fold into one switch (1364 B).  The only source-level
way to set per-case weights would be C++20 `[[likely]]` on cases or a
clang-level weight knob, neither available here.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11617
ulimit -v 12000000
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN6Status16missionCompletedEbbx
```

Scratch (`/dev/shm/w11617/`): `Status.base.cpp`, `m.sh` (incremental
build+fn_diff), `mkchain.py`, `mkchain2.py`, `mkchain3.py`, `mkrank.py`,
`mkgap.py`, `mksplit.py`, `mkfall.py`, `mkgroups.py`, `mkranktable.py`,
`Status.ll`, `inj.py`, `inj2.py`, `inj3.py`, `linkcmd.sh`,
`linkcmd_tbl.sh`, `mission_case_table.cpp`.

## 5. Files changed

* `_work/reconstructions/status-432-producers-wave170-w11617.md` — this note.
* No `src/`, `CMakeLists.txt`, baseline, allowlist or doc change.
