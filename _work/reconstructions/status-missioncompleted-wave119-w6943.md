# `Status::missionCompleted` — wave 119 (w6943): the plain switch body order,
# the fallthrough-chain shape, and why clang 7.0.2 emits value order here

Status: **source change landed** (rank table dropped, `.rodata` byte-exact) +
this research note.  Worktree `wt/w6943`, branch `wave119-w6943`, base master
`7a031742d`.  Original binary: `../_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 r328903, `-Oz`, image base 0x10000).

Target: `_ZN6Status16missionCompletedEbbx` (`Status::missionCompleted(bool,
bool, long long)`), original `0xabf38`, size **1432 B**.

| | before (HEAD `7a031742d`) | after (this pass) |
|---|---:|---:|
| function score | **92.5 %** | **59.4 %** |
| function size | 1432 B | 1432 B |
| `.rodata` size | 0x41c27 (+0xc0 = 192 B) | **0x41b67 (exact)** |
| `.rodata` bytes | 192 B of rank table appended | **byte-identical** |
| gate | byte 4505 / linked 4505 | **byte 4505 / linked 4505** |

Everything else is unchanged: `.text` 0x1337c8 at 0x69440, `.ARM.exidx`
0x6f70, `.ARM.extab` 0x26298, `.data` 0x296c, 27 sections, file size
2,187,328 B, imports 207, data 843/843, lint CLEAN, drift 0.

## 1. What changed

`tools/mission_case_table.cpp` (the 192-byte mission-type -> case-rank
table, dumped in `status-missioncompleted-wave119-w6930.md` §1) is deleted,
`CMakeLists.txt` no longer builds/links the `mission_table` object, and
`Status::missionCompleted` switches directly on `m->getType()` with the real
mission-type labels in the original body order.  This is exactly the source
state of commit `95df4d1be` (w6935), re-landed here; master had kept only
w6935's note.

Two faithful source details are kept (both were already in the w6935 tree):

* body 166 (case 10) re-queries `m->getProductionGoodIndex()` for the
  `hasEquipment` call (the original does this);
* body 168 (case 20) keeps the `(unsigned short)` cast on
  `m->getStatusValue()` as a two-byte size device (the original has no
  `uxth`; without the cast the function is 1428 B and shifts `.text`).

## 2. Gate / structural result

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
# verify: byte 4505  linked 4505  stubs 0  missing 0  extra 0  avg 99.93
# data:   exact 843 of 843 exported objects
# sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
# RATCHET PASS
cmake --build cmake-build-match --target drift -j6   # DRIFT: 0 field(s)
cmake --build cmake-build-match --target lint  -j6   # CLEAN
```

Section table measured after the change (original vs ours):

| section | original | ours | |
|---|---:|---:|---|
| `.text` | 0x69440 / 0x1337c8 | 0x69440 / 0x1337c8 | exact |
| `.rodata` | 0x1c9e10 / 0x41b67 | 0x1c9e10 / 0x41b67 | **exact, 0 differing bytes** |
| `.ARM.exidx` | 0x19cc08 / 0x6f70 | 0x19cc08 / 0x6f70 | exact |
| `.ARM.extab` | 0x1a3b78 / 0x26298 | 0x1a3b78 / 0x26298 | exact |
| `.data` | 0x214000 / 0x296c | 0x214000 / 0x296c | exact |
| `.bss` | 0x216970 / 0x69ac | 0x216970 / 0x69ac | exact |
| `.got` | 0x20fcf4 / 0x430c | 0x20fcf4 / 0x430c | exact |
| file size | 2,187,328 | 2,187,328 | exact |

`.rodata` is now **byte-identical** (269,159/269,159 bytes compare equal).
This removes the last known `.rodata` delta.  `.text` size/base is unchanged
because the function still compiles to 1432 B, so every following function
keeps its original address (`Status::setJumpgateUsed` is still at `0xac4d0`).

## 3. The body-order problem, restated

The original's 40-entry TBH (`sub.w r1,r0,#150; cmp r1,#39; bhi; tbh` at
`0xaa`, table at `0xae`) points at bodies in this address order:

```
167/174, 150 151 152 153 154 155 158 162 166 170 184 189,
         156 157 160 163 164 165 168 169 171 172,  default/latch
```

i.e. two strictly increasing value runs
`A = 150,151,152,153,154,155,158,162,166,170,184,189` then
`B = 156,157,160,163,164,165,168,169,171,172`.

A plain `switch (m->getType())` with the real labels emits the same dispatch,
the same 40-entry TBH and the same body bytes, but lays the bodies out in
**ascending value order**:

```
167/174, 150 151 152 153 154 155 156 157 158 160 162 163 164 165 166
         168 169 170 171 172 184 189,  default
```

Measured layout (TBH decode of the linked `.so`, `dumptbh.py`):

```
orig : 167 8 11 13 14 150 151 152 153 154 155 158 162 166 170 184 189 156 157 160 163 164 165 168 169 171 172 159
plain: 167 8 11 13 14 150 151 152 153 154 155 156 157 158 160 162 163 164 165 166 168 169 170 171 172 159 184 189
```

The first seven bodies (167/174, 150..155) are at **identical addresses** in
both builds; the divergence starts at 156 vs 158.  Body sizes match
type-by-type (166 = 100 B, 162 = 102 B, 158 = 92 B, 157 = 52 B, 156 = 42 B,
168 = 30 B, 171 = 26 B, 170 = 14 B, 184 = 58 B, 172 = 64 B), so only the
order differs.

## 4. Why: `visitSwitch` adds the jump-table successors in *table* order

`lib/CodeGen/SelectionDAG/SelectionDAGBuilder.cpp` (`buildJumpTable`):

```cpp
// Add successors. Note: use table order for determinism.
SmallPtrSet<MachineBasicBlock *, 8> Done;
for (MachineBasicBlock *Succ : Table) {
  if (Done.count(Succ)) continue;
  addSuccessorWithProb(JumpTableMBB, Succ, JTProbs[Succ]);
  Done.insert(Succ);
}
```

`Table` is built from `Clusters`, which `sortAndRangeify` sorts by *value*.
So the switch MBB's successor list is value-sorted, and
`MachineBlockPlacement::markBlockSuccessors` pushes the successors into
`BlockWorkList` in exactly that order.

Every case body has the same block frequency (1/40 of the switch; only
167/174 is 2/40 because it has two labels), so
`selectBestCandidateBlock` — which keeps the *first* block of maximal
frequency — drains the worklist in value order.

Confirmed in the instrumented-clang MBP trace (`MBP_TRACE=1`,
`/tmp/llvm_android_702b/build2/bin/clang++`):

```
MERGE %bb.104 ('if.end37') -> %bb.16 ('sw.bb')      ; selectBestSuccessor peels 167/174 (2/40)
CONFLICT bb=%bb.16 succ=%bb.17 pred=%bb.45 ...      ; shared tail 17 has a better pred -> chain breaks
WORKLIST size=23: %bb.11 %bb.36 %bb.38 %bb.39 %bb.44 %bb.45 %bb.46 %bb.77 %bb.80 %bb.47 %bb.85 ...
CAND %bb.36 freq=256 ... CAND %bb.71 freq=256       ; all equal
MERGE %bb.16 -> %bb.11                              ; worklist head (freq 1403)
...
MERGE %bb.36 -> %bb.38                              ; then strictly worklist order = value order
```

`%bb.36,38,39,44,45,46,77,80,47,85,57,88,91,92,62,93,96,66,97,98,67,71`
map to `150,151,152,153,154,155,156,157,158,160,162,163,164,165,166,168,
169,170,171,172,184,189` — value order.

`BranchProbabilityInfo::getEdgeProbability` returns `1/succ_num` for every
switch edge when there is no `!prof`, so the frequencies are exactly equal;
source case order therefore cannot influence the layout (verified: the
value-order source gives the identical layout, 59.0 % instead of 59.4 %).

## 5. Proof that the original layout is `getFirstUnplacedBlock` order

`MachineBlockPlacement::getFirstUnplacedBlock` walks the MachineFunction in
MBB order, i.e. the order the IR basic blocks were created — the **source
order** of the case bodies.  Forcing it with an instrumented clang
(`MBP_FORCE_FALLBACK=1` makes `selectBestCandidateBlock` return `nullptr`):

```sh
MBP_FORCE_FALLBACK=1 bash /dev/shm/w6943/mkfb.sh
bash /dev/shm/w6943/linkfb.sh
python3 /dev/shm/w6943/score.py
# _ZN6Status16missionCompletedEbbx: pct 63.8  osize 1432 usize 1440
```

Layout with the forced fallback:

```
167 8 11 13 14 150 151 152 153 154 155 158 162 166 170 184 189 156 157 160 163 164 165 168 169 171 172 159
```

— **exactly the original**.  Size 1440 B (+8), score 63.8 %.

So the original source order *is* the A-then-B order (which the landed source
already has), and the original build simply placed the bodies in MBB order.
Our build reaches `selectBestCandidateBlock` with a 23-entry worklist, so the
fallback is never taken.

## 6. A same-TU counter-example: `Status::nextCampaignMission` is source-ordered

`_ZN6Status19nextCampaignMissionEb` (8492 B, `0xa9a8c`) is **byte-exact**
(100 %) and contains a 162-case plain `switch (mission)` in the *same*
translation unit, compiled by the *same* clang.  Its TBH layout is the
**source order**, not value order:

```
source : 52 128 147 148 149 150 44 21 7 38 11 47 16 24 14 18 42 45 106 49 8 31 39 30 32 33 29 1 20 23 34 51 53 58 59 73 77 80 87 99 109 113 157 159 0 2 12 28 54 55 56 57 60 ...
layout : 52      147             44 21 7 38 11 47 16 24 14 18 42 45      49 8 31 39 30 32 33 29 1 20 23 34 51 53 58 59 73 77 80 87 99 109 113 157 159 0 2 12 28 54 55 56 57 60 ...
```

(duplicate values are the shared-label bodies 52/128 and 147..150, 45/106).
The MBP trace shows the chain walking fallthrough edges
(`MERGE %bb.6 -> %bb.7 -> %bb.8 -> %bb.9 -> %bb.11 -> %bb.13 -> %bb.14 ->
%bb.16 -> %bb.19 -> %bb.20 -> %bb.22 -> ...`), so the bodies end up in MBB
order.  **Source-order layout for a big switch is therefore reachable with
this exact compiler**; `missionCompleted` fails to reach it only because its
case bodies do not chain.

## 7. Making `missionCompleted` chain: full-A fallthrough

The chain in `missionCompleted` breaks at the *shared* comparison tails
(`0xac166`, `0xac172`): body 16 (167/174) -> 17 is rejected by
`hasBetterLayoutPredecessor` because 17 also has predecessor 45 with a
comparable edge frequency (`CONFLICT ... predfreq=256 candfreq=512`,
`HotProb = 80 %`).

If the **whole A group is written with fallthrough** (drop the `break;`
after every A body except the last), the A bodies become one chain, the
worklist only contains that chain head plus the B bodies, and the layout
becomes the original A-then-B order:

| variant | usize | pct | layout |
|---|---:|---:|---|
| **A-group fallthrough** (no `break;` in A except the last) | 1388 | 49.7 | **original A-then-B** |
| A fallthrough incl. 189 -> 156 | 1384 | 50.0 | original except 172 after default |
| A+B fallthrough | 1428 | 49.2 | original except 172 |
| partial A fallthrough (150,153,154 only) | 1456 | 55.4 | value order |
| partial A fallthrough (150,151,153 only) | 1496 | 48.5 | value order |

Only the *full* A chain changes the order; partial chains leave the worklist
in value order.  The fallthrough shape is semantically wrong (a type-150
mission would also test `kills`, ...) and merges the tiny counter bodies, so
it is 44 B shorter and scores 49.7 % — lower than the plain switch even
though its layout is right.  It is recorded here as the **only plain-C++
source shape found that emits the original body order**.

## 8. All measured variants (table dropped, same `link_so` + single-symbol score)

| variant | usize | pct | body order |
|---|---:|---:|---|
| **plain switch, original source order (landed)** | **1432** | **59.4** | value |
| plain switch, value-order source | 1432 | 59.0 | value |
| full-A fallthrough | 1388 | 49.7 | **original** |
| A fallthrough incl. 189->156 | 1384 | 50.0 | original (172 last) |
| A+B fallthrough | 1428 | 49.2 | original (172 last) |
| partial A fallthrough (150,153,154) | 1456 | 55.4 | value |
| partial A fallthrough (150,151,153) | 1496 | 48.5 | value |
| two sequential switches, cached `int mtype` | 1512 | 52.6 | **original** |
| two sequential switches, re-query `getType()` | 1456 | 45.8 | 3 tables |
| nested switch (A outer, LOW+B inner) | 1404 | 24.6 | 3 tables |
| duplicate A case labels (each A body 2 labels) | 1376 | 45.8 | A-then-B-ish |
| `!prof` hand-edited IR (`default=2, 167/174=3, A=2, B=1, low=2`) | 1376 | 46.3 | **original** |
| forced `getFirstUnplacedBlock` (instrumented clang, not plain C++) | 1440 | 63.8 | **original** |

The `!prof` row is the decisive experiment: giving the big switch
`branch_weights` with A=2 and B=1 makes `selectBestCandidateBlock` pick every
A body before every B body, and the ties inside each group fall back to the
worklist's value order — which is exactly A's and B's order.  Plain C++
cannot attach `!prof` to a `switch` (`__builtin_expect` on the operand
becomes a `select`; an `if`-chain loses the jump table).

The duplicate-label row is the same effect from the front end: giving each A
body a second (currently-default) case value raises its frequency to 2/40,
and the A bodies are then placed before B.

## 9. Proven vs hypothesis

**Proven (built and measured in this worktree):**

1. Dropping the rank table makes `.rodata` **byte-identical** (0x41b67,
   269,159/269,159 bytes) with every other section size/base, the 27-section
   layout and the 2,187,328 B file size unchanged; gate stays byte 4505 /
   linked 4505, lint CLEAN, drift 0.
2. The plain switch emits the same dispatch, TBH and body bytes but lays the
   bodies out in ascending value order (59.4 %, 1432 B).
3. Source case order does not affect the layout (value-order source: same
   layout, 59.0 %).
4. Forcing `getFirstUnplacedBlock` reproduces the original A-then-B order
   exactly (1440 B, 63.8 %) — so the original layout is the MBB/source order.
5. Full-A fallthrough makes a plain switch emit the original body order
   (1388 B, 49.7 %); partial fallthrough does not.
6. Two sequential switches also reproduce the order (1512 B, 52.6 %).
7. `!prof` weights `A=2, B=1` reproduce the order (1376 B, 46.3 %).
8. `Status::nextCampaignMission` — same TU, same compiler, 162-case plain
   switch — is byte-exact with a **source-ordered** TBH layout, because its
   bodies chain through fallthrough edges.

**Hypothesis (not proven):** the original `missionCompleted` was laid out by
`getFirstUnplacedBlock` (equivalently, with A/B branch weights) because its
CFG differed from ours in a way plain C++ cannot express here — the only
knobs that change the order are unequal switch-edge frequencies or a
fallthrough chain, and neither is available without changing the body code.

## 10. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6     # RATCHET PASS, byte 4505

# single-function score (fast, ~1 s)
python3 /dev/shm/w6943/score.py _ZN6Status16missionCompletedEbbx
# layout decode of the linked .so
python3 /dev/shm/w6943/dumptbh.py
# layout decode of a compile with the instrumented clang
python3 /dev/shm/w6943/layout_fast.py
```

Scratch (not committed): `/dev/shm/w6943/` — `score.py` (single-symbol
verify score), `layout_fast.py` (instrumented-clang TBH layout decode),
`dumptbh.py` (TBH dump of the linked `.so`), `decode2.py` (generic TBH
decoder), `t.sh` (copy variant -> build -> score -> layout), `mkft.py`
(fallthrough variant generator), `vgen.py` (case-order generator),
`mkfb.sh` + `linkfb.sh` (forced-fallback object + gold link),
`linkwt.sh` (link the `!prof` object), `Status_plain.cpp`, `fallthru.cpp`,
`twosw.cpp`, `nested.cpp`, `dupcase.cpp`, `plain_wt2.ll`, `ncm.txt`
(`nextCampaignMission` MBP trace), `mc.txt` (`missionCompleted` MBP trace).
