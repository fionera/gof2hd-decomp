# `Status::missionCompleted` — wave 124 (w7409): grouped cases, if/else
# nesting, rank switch, and a 1432-B exact-order weight scheme

Status: **note-only** (no source change landed).  Worktree `wt/w7409`, base
master `6f8ee2320` (wave-123 final snapshot).  Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(sha256 `d4c17be6...bed9483`, armeabi-v7a, NDK r18b clang 7.0.2 r328903,
`-Oz`, image base 0x10000).

Target: `_ZN6Status16missionCompletedEbbx` (`Status::missionCompleted(bool,
bool, long long)`), original `0xabf38`, size **1432 B**.

| | baseline (HEAD `6f8ee2320`) |
|---|---:|
| function score | **71.0 %** |
| function size | 1432 B |
| gate | byte 4506 / linked 4506 |
| avg / data / imports | 99.94 / 843 of 843 / 207 |
| lint / drift | CLEAN / 0 |
| sections | `.text` 0x1337c8 @ 0x69440 (87,014 diff), `.rodata` **0 diff**, `.ARM.exidx` **0 diff**, `.ARM.extab` size/base exact (1,446 diff), 27 sections |

## 0. Summary

The residual is still exactly the high-jump-table body order: the original is
two ascending runs

```
167/174  150 151 152 153 154 155 158 162 166 170 184 189   (run A)
         156 157 160 163 164 165 168 169 171 172           (run B)
latch
```

while our plain `switch (m->getType())` emits ascending **value** order.  The
prologue, the two TBH dispatches (high 40-entry table @ `0xae`, low 7-entry
table @ `0x11a`), the low group and the shared tails are already
byte-identical; only the high body order differs.

This pass tried the four requested plain-C++ lowerings.  **None keeps the
1432-B size and the original order at the same time.**  The two-switch split
does reproduce the order exactly, but it needs a second dispatch (+8 B,
4 TBH instead of 2) and scores only 43.5 %.  A new result is that a `!prof`
weight scheme with **A > B** does give the exact original order **at exactly
1432 B** (e.g. `default 1, A 1029, B 1010, 167/174 2810, low 1005`), but
`__builtin_expect` on a switch can only set a single weight to 2000
(`LowerExpectIntrinsic::handleSwitchExpect`), so it is not expressible from
plain C++.

## 1. Baseline evidence (measured this pass)

```sh
cmake --build cmake-build-match --target check -j6
# verify: byte 4506  linked 4506  stubs 0  missing 0  extra 0  avg 99.94
# data:   exact 843 of 843 exported objects
# sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
# RATCHET PASS
```

`report.json` row: `{"name":"_ZN6Status16missionCompletedEbbx","pct":71.0,
"osize":1432,"usize":1432}`.  The original high-table targets (decoded from
the TBH at `0xabfe6`, base `0xabfea`) are
`167/174, 8, 0/11, 13, 14, 150,151,152,153,154,155,158,162,166,170,184,189,
156,157,160,163,164,165,168,169,171,172, latch`; ours is the same with
`150..189` in value order.

## 2. Requested lowerings, measured

All sizes are the official single-symbol `usize`; order is the decoded TBH
body order.

### 2a. Switch with grouped cases (explicit hole labels -> `default:`)

Giving the 16 high-range holes (`159 161 173 175 176 177 178 179 180 181 182
183 185 186 187 188`) explicit `case` labels grouped with `default:` (at the
start, in the middle after `case 189`, or at the end) does not change the
jump table (holes already point at the latch) and does not change the order:

```
size 1432   pct 71.0   HIGH: 167/174 150 151 152 153 154 155 156 157 158
                                 160 162 163 164 165 166 168 169 170 171 172 184 189
```

Grouping the holes with the A bodies instead (the wave-123 hole-fill) gives
the exact order but changes the table content and the size (1372/1376).

### 2b. Switch inside an if/else chain

```cpp
if (mtype >= 150) { switch (mtype) { ... high ... } }
else              { switch (mtype) { ... low  ... } }
```

| shape | usize | order |
|---|---:|---|
| `if (mtype >= 150)` high first | 1372 | `167/174 150 151 152 153 154 155 156 157 158 160 162 163 164 165 166 168 169 <latch> 170 171 172 184 189` |
| `if (mtype < 150)` low first | 1424 | scrambled |
| same with `__builtin_expect(cond,1)` | 1372 / 1392 | value order / scrambled |

The high-first form loses the second range check (the else branch already
knows `type < 150`), which is why it shrinks to 1372 B.

### 2c. Switch on a small enum/rank

A source-level rank table (`static const unsigned char kRank[190]`, rank
assigned in A-then-B source order, `switch (kRank[m->getType()])`) compiles
to **1388 B with a single 23-entry TBH** (the high/low split disappears) and
the bodies in near-source order — but the dispatch is structurally different
from the original's two TBHs.  The wave-121 manually-linked 192-byte table
variant was 1432 B / 92.45 % but added 192 bytes to `.rodata`.

### 2d. Two-switch split, blocks placed A-then-B

```cpp
int mtype = m->getType();
switch (mtype) { case 167: case 174: ... A ... }   // 167/174 + A
switch (mtype) { case 8: ... case 14: ... B ... }  // low + B
```

| shape | usize | TBH | order | pct |
|---|---:|---:|---|---:|
| `[167/174 + A]` then `[low + B]`, cached `int` | **1440** | 4 | **exact** | **43.5** |
| same without the 4 body hints | 1440 | 4 | exact | — |
| same with hint 0 only | 1456 | 4 | exact | — |
| `[167/174 + low + A]` then `[B]` | 1508 | 3 | exact | 52.1 |
| `[167/174 + A]` then `[B]` then `[low]` | 1480 | 3 | exact | 40.0 |

The exact-order split costs a second `sub/cmp/bhi + tbh` dispatch (+8 B) and
two extra TBHs.  Even at 1432 B it would not be byte-exact, because the
dispatch bytes differ.

### 2e. Nested switch in `default:` (merged away)

Putting the second switch in the first switch's `default:` is folded back
into a single switch by SimplifyCFG:

```
nest_A_B (A + default: switch B)      size 1432   2 TBH   value order   71.0
n3_A_Blow (A + default: switch B+low) size 1380   2 TBH   value order   —
```

A nested grouping switch (A group -> inner switch, B group -> inner switch)
costs 4 TBHs and 1496 B.

## 3. New: exact order is reachable at 1432 B with `!prof` weights

Patching the `switch i32` in the `-g0` IR with `!prof branch_weights` and
compiling with the NDK clang (same flags as the build):

| weights (default, A, B, 167/174, low) | usize | order |
|---|---:|---|
| `1, 1, 1, 2, 1` (uniform + 167/174 double) | 1432 | value |
| `1, 2, 1, 2, 1` | 1376 | exact |
| `1000, 1002, 1000, 1002, 1000` | 1436 | exact |
| `1, 1024, 1000, 1025, 1000` | 1436 | exact |
| **`1, 1029, 1010, 2810, 1005`** | **1432** | **exact** |
| `1, 2000, 1, 1, 1` | 1376 | exact |

So the original order is a pure edge-frequency effect and *can* coexist with
a 1432-B function.  The 1432-B scheme is not the original codegen, though:
its high table is `070 073 076 090 093 09c 17c ...` vs the original
`095 098 09b 0b5 0b8 0c4 1a2 ...` (the bodies are ~37 B smaller), while the
1436-B scheme's table is within a few bytes of the original
(`095 098 09b 0b5 0b8 0c4 1a4 1b9 0cd 23c 1d2 ...`).  The original's weights
are therefore "A slightly hotter than B", and our build of that IR is 1436 B.

## 4. Why plain C++ cannot express it

`LowerExpectIntrinsic.cpp` (LLVM 7.0.0):

```cpp
static bool handleSwitchExpect(SwitchInst &SI) {
  ...
  unsigned n = SI.getNumCases();
  SmallVector<uint32_t, 16> Weights(n + 1, UnlikelyBranchWeight);   // all 1
  if (Case == *SI.case_default())
    Weights[0] = LikelyBranchWeight;                                // 2000
  else
    Weights[Case.getCaseIndex() + 1] = LikelyBranchWeight;          // 2000
  SI.setMetadata(LLVMContext::MD_prof, ...);
```

Exactly one successor gets weight 2000; every other weight is 1.  There is
no other front-end producer of `!prof` on a `switch` (no `MD_prof` emission
in `CGStmt.cpp`; `__builtin_expect_with_probability` does not exist in clang
7.0.2).  Nested `__builtin_expect` is erased after the first
`handleSwitchExpect`.  `__builtin_expect` on the switch operand was swept
for every K in `0..19`, `145..199` and `999`: only **K = 14** keeps 1432 B,
and its order is scrambled (16.0 %); all other K give 1360–1456 B.

Body-level `__builtin_expect` hints do not move the switch order at all
(they only change the body's internal branch weights); adding hints to
cases 150/151/152/158/170 left the value order unchanged (sizes 1432/1400/
1436).

## 5. Root cause (unchanged) and conclusion

`SelectionDAGBuilder::buildJumpTable` adds the high-TBH successors in
ascending case-value order and `MachineBlockPlacement::selectBestCandidateBlock`
drains them by block frequency, keeping the first on ties.  The original's
A-then-B layout is what you get when the A bodies are strictly hotter than
the B bodies, i.e. when the switch carries `!prof` weights with A > B
(consistent with a PGO-built original).  Plain C++ cannot express A > B via
`__builtin_expect`, and every structural rewrite that reproduces the order
changes the dispatch and/or the 1432-B size.  No gate-green source win was
found.

## 6. Reproduction

```sh
cmake --build cmake-build-match --target check -j6   # RATCHET PASS, byte 4506
# per-variant size + TBH body order (no link needed):
python3 /dev/shm/w7409/eval.py <variant>.cpp
# official single-symbol score:
bash /dev/shm/w7409/batch.sh <variant>.cpp          # copies into Status.cpp, link_so, restore
# !prof proof (NDK clang 7.0.2, -g0 IR):
#   patch `switch i32 %52` with `], !prof !99999` and
#   !99999 = !{!"branch_weights", i32 1, i32 1029, i32 1029, i32 1010, ...}
#   (default 1, 167/174 2810, A 1029, B 1010, low 1005)
#   -> 1432 B, exact original body order
```

Scratch (not committed): `/dev/shm/w7409/` — `base.cpp` (current source
snapshot), `base_g0.ll` (`-g0` IR), `eval.py`, `batch.sh`, `score1.py`,
`wtest.py`/`wsweep.py`/`wrand.py`/`wclean.py` (weight patch + size/order),
`gen_ifelse.py`, `gen_two.py`, `gen_nest3.py`, `gen_group.py`,
`gen_holes_default.py`, `gen_rank.py`, `hintsearch.py`, and the generated
`ifelse_*`, `two_*`, `n3_*`, `nest_*`, `group_nested`, `hd_*`, `rank`,
`h150/h151/h152/h158/h170` variants.
