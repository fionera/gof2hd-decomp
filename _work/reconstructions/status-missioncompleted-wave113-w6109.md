# `Status::missionCompleted` — wave 113: MBP fallback mechanism confirmed;
# the rank table is still the best shape (w6109)

Status: **research note only** (no source change). Worktree `wt/w6109`, branch
`wave113-w6109`, base master `a979ace18`. Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 r328903 / LLVM 1d739ffb, `-Oz`).
Target: `_ZN6Status16missionCompletedEbbx`, original `0xabf38`, size **1432
(0x598)**, score **87.0 % / 1432 B** (unchanged; no variant beat it).

## 0. Result

Every variant measured this pass is **<= 87.0 %**.  The landed wave-110 rank
table (`tools/mission_case_table.cpp`, fixed address `0x20b977`,
`switch (MISSION_CASE[(unsigned char)m->getType()])`, 32-entry TBH) remains the
best shape.  The gate is unchanged and green: `RATCHET PASS`, `byte_exact
4500`, `linked_exact 4500`, imports 207, `lint CLEAN`, `drift 0`, 27 sections,
exact file size 2,187,328 B, `.ARM.extab` `0x26294` (-4).

**New this pass:** the wave-112 "two-predecessor / MF-order fallback"
hypothesis is now *proved*, and the exact source shape that triggers it is
identified (`S_seq`, two sequential switches on one cached `int`).  It
reproduces the original body order **and** the original single 40-entry TBH
dispatch exactly, but costs +116 B (1548 B) because `this` spills; score
**46.8 %**.  The reason the NDK clang cannot do it with one switch is a real
MBP difference vs LLVM 7.0.0svn (see §4).

## 1. Original decode (unchanged, re-verified)

`getType()` is switched directly: `sub.w r1,r0,#150; cmp r1,#39; bhi
second; tbh` (40 entries, values 150..189), then `sub #8; cmp #6; bhi third;
tbh` (7 entries, 8..14), then `cmp r0,#0` (case 0, which falls through into
the case-11 body).  One plain switch with cases `{0, 8..14, 150..174, 184,
189}`.

Body order by address (23 distinct bodies, `latch` = `ac45a`):

```
167/174, 8, 11, 13, 14, 150, 151, 152, 153, 154, 155, 158, 162, 166,
170, 184, 189, 156, 157, 160, 163, 164, 165, 168, 169, 171, 172, latch
```

This is **not** value order.  Value order (what a plain `switch (getType())`
emits with this clang) is

```
167/174, 150, 151, 152, 153, 154, 155, 156, 157, 158, 160, 162, 163,
164, 165, 166, 168, 169, 170, 171, 172, 184, 189, latch
```

The only difference is that the ten bodies `{156, 157, 160, 163, 164, 165,
168, 169, 171, 172}` are moved to the end, after 189.

## 2. The mechanism, proved: two predecessors => MF-order fallback

`SelectionDAGBuilder::buildJumpTable` adds the JT block's successors in
**table (value) order** (`// Add successors. Note: use table order for
determinism.`), and `MachineBlockPlacement::selectBestCandidateBlock` breaks
equal-frequency ties by keeping the **first** worklist entry.  So a one-switch
CFG always lays the bodies out in value order (verified: source case order is
irrelevant — ascending / original-body-order / reversed all give the same
object; `S_direct.cpp`, `S_direct_orig.cpp`, `S_direct_rev.cpp`).

If instead the ten bodies each get a **second predecessor**, their chains have
`UnscheduledPredecessors != 0` when the JT block is placed, so they are not
pushed onto `BlockWorkList`.  Once the rest of the chain is placed,
`selectBestSuccessor` and `selectBestCandidateBlock` both return null and
`buildChain` falls back to

```cpp
BestSucc = getFirstUnplacedBlock(Chain, PrevUnplacedBlockIt, BlockFilter);
DEBUG(dbgs() << "Unnatural loop CFG detected, forcibly merging the "
                "layout successor until the CFG reduces\n");
```

i.e. **MachineFunction (IR) order**, which is source order.  That is exactly
the original layout: the first 17 bodies come out in value order (which
coincides with source order for them) and the ten delayed bodies come out in
source order at the end.

The cheapest real-code way to give those ten bodies a second predecessor is
**two sequential switches on one cached `int`**:

```cpp
int mtype = m->getType();
switch (mtype) { /* 17 cases: 167/174, 8, 0/11, 13, 14, 150..155, 158,
                    162, 166, 170, 184, 189 */ }
switch (mtype) { /* 10 cases: 156, 157, 160, 163, 164, 165, 168, 169,
                    171, 172 */ }
```

`SimplifyCFG` folds the second switch's cases into the first (the merged
switch's case list is `[first group..., second group...]`) while keeping the
second switch reachable from the default, so the ten bodies end up with two
IR predecessors.  The merged switch is still lowered to **one** 40-entry TBH
plus the 7-entry 8..14 TBH — the dispatch is byte-identical to the original.

Measured (`S_seq.cpp`, official verifier): **46.8 % / 1548 B**.  Decoded body
offsets (function-relative) vs original:

| body | orig | S_seq | delta |
|---|---:|---:|---:|
| 167/174 | 0x0fe | 0x0fe | 0 |
| 150 | 0x1d8 | 0x20c | +52 |
| 151 | 0x1de | 0x214 | +54 |
| 152 | 0x1e4 | 0x21c | +56 |
| 153 | 0x218 | 0x250 | +56 |
| 154 | 0x21e | 0x258 | +58 |
| 155 | 0x236 | 0x272 | +60 |
| 158 | 0x248 | 0x286 | +62 |
| 162 | 0x2a4 | 0x2e2 | +62 |
| 166 | 0x30a | 0x34a | +60 |
| 170 | 0x36e | 0x3b8 | +74 |
| 184 | 0x37c | 0x3c6 | +74 |
| 189 | 0x3b6 | 0x400 | +74 |
| 156 | 0x3f2 | 0x45a | +104 |
| 157 | 0x41c | 0x486 | +106 |
| 160 | 0x450 | 0x4c0 | +112 |
| 163 | 0x47e | 0x4f0 | +114 |
| 164 | 0x49a | 0x50e | +116 |
| 165 | 0x49e | 0x512 | +116 |
| 168 | 0x4a6 | 0x51a | +116 |
| 169 | 0x4c4 | 0x532 | +116 |
| 171 | 0x4c8 | 0x536 | +116 |
| 172 | 0x4e2 | 0x552 | +116 |
| latch | 0x522 | 0x592 | +112 |

The **first body is exact** (0x0fe) and the dispatch is exact — but the
`mtype` value is live across the whole first switch, which costs one
callee-saved register: `this` spills to `[sp,#16]` and is reloaded in every
body (`str r0,[sp,#16]` in the prologue, `ldr r0,[sp,#16]` before each
`this->` use).  That is the +52 in the first body and the +116 at the tail.
The original keeps `this` in `r6` and `i` in `fp`, i.e. it has no live switch
value across the bodies.

Trying to avoid the live value by writing `switch (m->getType())` twice
(`S_two_gettype.cpp`) does **not** work: `getType()` is not `readonly`, so the
two calls are not CSE'd, the second switch keeps its own SSA value, the ten
bodies get only one predecessor, and the layout reverts to value order
(1460 B).  `unsigned char mtype` is worse (1560 B).

## 3. Rank-table re-parameterisation (measured, all worse)

The rank table lets the body order be chosen freely (ascending rank == body
order), so the residual error is only the dispatch + body-offset shift.  The
first body lands at `18 + 2N - 92 = 2N - 74` bytes relative to the original
(`N` = TBH entry count), and the latch at that plus the body-size excess
(+10 B: anchor +6, mid-function pool island +10, type-168 body -6).

| variant | pct | usize | notes |
|---|---:|---:|---|
| **HEAD rank table N=32** | **87.0** | **1432** | latch exact; first body -10 |
| N=36, all ranks in 150..185 (one TBH, no 8..14 TBH) | 69.6 | 1408 | first body -2, latch +8 |
| N=37 ranks `{0,8,11,13,14,150..170,186}` (`S_r37c`) | 47.6 | 1384 | exact order, dispatch +3 insns |
| N=37 ranks `150..175,186` (`S_r37b`) | 70.0 | 1408 | first body exact, latch +10 |
| table as function-local `static const` (`S_localtbl`) | 69.2 | 1408 | -24 B, worse |
| two sequential switches (`S_seq`) | 46.8 | 1548 | **exact order + exact dispatch**, this spills |
| `switch (m->getType())` twice (`S_two_gettype`) | 45.6 | 1460 | no CSE, value order |
| nested `default:` switch (`S_nest`) | 39.07 | 1412 | order wrong (156 second) |
| direct switch, original body order (`S_direct_orig`) | 60.4 | 1428 | value order |
| no `case 159` anchor (`S_noanchor`) | 70.6 | 1428 | regalloc changes |
| fallthrough `{158,162,166,170,184,189}` | 44.3 | 1356 | exact order, bodies merge |

The 32-entry table wins because it is the only size that puts the **latch at
exactly 0x522**: every body's `break` is a `b <latch>` and there are far more
of those than branches to any individual body.  N=37 makes the first body
exact but moves the latch to +10 and loses more than it gains.

## 4. Why the NDK clang cannot do it with one switch (new)

Compiling the *same* `.ll` with two backends gives two different layouts:

```sh
# IR produced by the NDK clang frontend
clang++ <flags> -emit-llvm -S S_direct_orig.cpp -o direct_oz.ll
# backend A: NDK r18b clang 7.0.2
clang++ <flags> -S direct_oz.ll            -> bodies in VALUE order
# backend B: LLVM 7.0.0svn llc
llc -O=2 -mtriple=thumbv7-none-linux-android16 -mcpu=generic \
    -mattr=+soft-float-abi,-fp-only-sp,-d16,+vfp3,-fp16,-vfp4,-fp-armv8,+neon,-crypto \
    -float-abi=soft direct_oz.ll           -> bodies in ORIGINAL order
```

`llc` 7.0.0svn's `MachineBlockPlacement` debug log for the target shows the
exact original body order (`%bb.104` = JT, then `%bb.16`, `%bb.19`, `%bb.13`,
`%bb.31`, `%bb.33`, `%bb.36`, `%bb.38`, `%bb.39`, `%bb.44`, `%bb.45`,
`%bb.46`, `%bb.47`, `%bb.55`, `%bb.60`, `%bb.65`, `%bb.66`, `%bb.70`,
`%bb.76`, `%bb.79`, `%bb.84`, `%bb.87`, `%bb.90`, `%bb.91`, `%bb.92`,
`%bb.94`, `%bb.95`, `%bb.96`), i.e. it hits the `getFirstUnplacedBlock`
fallback ("Unnatural loop CFG detected") for the ten bodies.  The NDK clang
7.0.2 backend does not.  The `MachineBlockPlacement.cpp` sources are
functionally identical (only `DEBUG`->`LLVM_DEBUG` and bracing), so the
difference is in the analyses feeding it (MBFI/MBPI) or in the IR at MBP time.

`-mllvm -static-likely-prob=N` changes the NDK layout but never reproduces the
original order for N = 30,40,45,50,55,60,65,70,75,80,85,90 (it moves the
default block around; the ten bodies never all move past 184/189).
`-tail-dup-placement=0/1` and `-align-all-blocks=0` change nothing.

Consequence: with the pinned NDK r18b clang a **single** switch cannot produce
the original body order; the only shapes that do are (a) the rank table
(exact order, +3 dispatch instructions) and (b) the two-switch shape (exact
order and dispatch, +116 B from the spill).

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6109
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
```

Scratch (not committed): `/dev/shm/w6109/` — `cc.sh`, `fastorder.sh`,
`sorder.sh`, `measure2.sh` (official verifier, reads
`cmake-build-match/verify/report.json`), `dec2.py`/`dec3.py` (TBH decoder),
`mkn.py` (N-entry rank-table generator), `S_head_restore.cpp` (= HEAD),
`S_direct.cpp`, `S_direct_orig.cpp`, `S_direct_rev.cpp`, `S_seq.cpp`,
`S_seq_uc.cpp`, `S_two_gettype.cpp`, `S_nest.cpp`, `S_noanchor.cpp`,
`S_localtbl.cpp`, `S_r37b.cpp`/`mct_r37b.cpp`, `S_r37c.cpp`/`mct_r37c.cpp`,
`S_n36.cpp`/`mct_n36.cpp`, `direct_oz.ll`, `direct_cpp.s`, `ndk_fromll.s`,
`llc7b.s`.  LLVM 7.0.0svn tools: `/tmp/llvm_ndk_build/bin/llc`.  NDK MBP
source: `/dev/shm/w6109/mbp_ndk.cpp`.
