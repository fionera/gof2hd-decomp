# `Status::missionCompleted` — wave 125 (w7507): plain-C++ switch shapes
# cannot place the low group *and* the A-run at 1432 B

Status: **note-only** (no source change landed).  Worktree `wt/w7507`, base
master `02513de33` (wave-124 final snapshot).  Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(sha256 `d4c17be6...bed9483`, armeabi-v7a, NDK r18b clang 7.0.2 r328903,
`-Oz`, image base 0x10000).

Target: `_ZN6Status16missionCompletedEbbx` (`Status::missionCompleted(bool,
bool, long long)`), original `0xabf38`, size **1432 B**.

| | baseline (HEAD `02513de33`) |
|---|---:|
| function score | **70.992366 %** (523 vs 525 normalised insns) |
| function size | 1432 B |
| gate | byte 4506 / linked 4506 / avg 99.93 |
| data / imports | 843/843 / 207 |
| lint / drift | CLEAN / 0 |

## 0. Summary

Wave 124 pinned the residual to the switch's branch weights.  This pass
tried the assignment's plain-C++ shapes (transformed switch value, switch in
an if chain, grouped cases, nested switch in `default`, `__builtin_expect`
combinations) plus a broad `!prof` weight sweep.

Three new results:

1. A **two-switch** source shape — the A cases in the first switch, the B
   cases in a second switch that is the first switch's `break` target —
   reproduces the **exact original body order** (`167/174, 8, 11, 13, 14,
   150..189 A, 156..172 B, latch`) from plain C++ at **1508 B** (official
   score 52.09 %, because the +76 B shifts every later body).
2. An exhaustive `!prof` weight sweep over
   `wa ∈ {2..1000}, wb ∈ {wa-1,wa-2,wa-3,wa/2,wa/3,1}, wl ∈ {wa-1,wa,wa+1,
   wa+2,wa+5,2wa}, w167 ∈ {wa,wa+1,2wa,2000}, wd ∈ {1,wa}` finds the exact
   target order only at sizes **1372, 1436 and 1448 B** — never 1432.
   `default 1, 167/174 200, A 200, B 199, low 201` is the 1436-B optimum
   (official 75.95 %); `low 500` gives the same order at 1428 B.
3. Every plain-C++ shape that keeps 1432 B keeps the base (value-order)
   layout.  A +4 B size change is fatal: measured `byte_exact 4506 -> 4467`
   for a 1436-B source variant, so the size really must stay 1432.

## 1. Baseline evidence (measured this pass)

```sh
cmake --build cmake-build-match --target link_so -j6
python3 score1.py        # 70.992366  523 525  osize 1432  usize 1432
cmake --build cmake-build-match --target check -j6
# verify: byte 4506  linked 4506  stubs 0  missing 0  extra 0  avg 99.93
# data:   exact 843 of 843 exported objects
# sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
# RATCHET PASS
```

Original body order (decoded from both TBHs, `0xabfe6` / `0xac052`):

```
0xac036 167/174   0xac060 8    0xac0c4 11   0xac0ce 13   0xac0dc 14
0xac110 150  0xac116 151  0xac11c 152  0xac150 153  0xac156 154
0xac16e 155  0xac180 158  0xac1dc 162  0xac242 166  0xac2a6 170
0xac2b4 184  0xac2ee 189  0xac32a 156  0xac354 157  0xac388 160
0xac3b6 163  0xac3d2 164  0xac3d6 165  0xac3de 168  0xac3fc 169
0xac400 171  0xac41a 172  0xac45a latch
```

Our base order is `167/174, 8, 11, 13, 14, 150 151 152 153 154 155 156 157
158 160 162 163 164 165 166 168 169 170 171 172 184 189, latch` — i.e. the
low group is already in the right place; only the A-run (150..189) is
interleaved with the B-run (156..172).

## 2. Why the low group is already first, and why A>B breaks it

`MachineBlockPlacement::selectBestCandidateBlock` picks the **maximum block
frequency**, first-on-tie, and `SelectionDAGBuilder::buildJumpTable` adds the
jump-table successors in ascending case-value order.  With uniform switch
weights the high bodies therefore drain in value order.

The high table spans `[150,189]` (40 slots) but only 24 are cases: the 16
gaps point at the latch.  `normalizeSuccProbs()` gives each high body
`1/40` of the high-JT frequency, while the low table (7 slots, 5 cases) gives
each low body `1/7` of the low-JT frequency.  With `P_high = 24/30`,
`P_low = 5/30`:

```
high body freq = 0.8 * 1/40 = 0.0200
low  body freq = 0.1667 * 1/7 = 0.0238
```

so the low bodies are 19 % hotter than the high bodies and are placed first
— exactly what the original does.  Any weight ratio `A > B` (needed for the
A-run) that is expressible through case multiplicities also **removes the
gaps**, which raises `P_high` and depresses `P_low` below A.  Measured with
`!prof`:

| weights (default / 167,174 / A / B / low) | size | high order | official |
|---|---:|---|---:|
| none (base) | 1432 | value order | **70.99** |
| 1 / 200 / 200 / 199 / 150 | 1432 | A then B, low last | **78.3** (wave 124) |
| 1 / 200 / 200 / 199 / 201 | 1436 | **exact source order** | **75.95** |
| 1 / 200 / 200 / 199 / 500 | 1428 | exact source order | — |
| 1 / 8 / 8 / 6 / 8 | 1372 | exact source order | 48.38 |

The 1436-B variant is only +4 B because two bodies differ from the original:
the 158 body is 6 B short (missing the `ldr r6,[sp,#16]` reload and using a
shared tail branch) and the 166 body is 10 B long (a `r5/r6/r9` register
rotation).  Net +4 B.  With the 16 gaps removed, the low bodies drop below
A, so the low group moves after B at 1432.

## 3. The two-switch shape reproduces the exact order (1508 B)

The only plain-C++ shape found that produces the original order is to split
the switch:

```cpp
int type = m->getType();
switch (type) {                       // 167/174, 8, 0/11, 13, 14, A
    case 167: case 174: ...
    ...
    case 189: ...
}
switch (type) {                       // B: the first switch's break target
    case 156: ... case 172: ...
}
```

The IR keeps two `switch i32 %52` instructions (the second in the common
`break` block `%297`), and the B bodies are laid out after the A bodies.
Decoded body order is **exactly** the original.  But the second dispatch
(`sub.w r0,fp,#156 / cmp #16 / bhi / tbb` + 17-byte table) plus lost tail
merging in the low bodies costs 76 B: **1508 B**, official **52.09 %**
(523 vs 552 insns).  The original has only two TBHs, so it is a single
switch.

## 4. Plain-C++ shapes measured this pass

All sizes measured with the project's exact clang 7.0.2 flags; order is the
`ev.py` body-sequence match against the original (base = 76.4).

| shape | size | order |
|---|---:|---:|
| base plain switch | 1432 | 76.4 |
| `switch (type - 150)` | 1432 | 76.4 |
| `case h: break;` for all 16 high holes (merged into latch) | 1432 | 76.4 |
| `default:` first / last | 1432 | 76.4 |
| nested switch (B) inside `default:` (merged back) | 1432 | 76.4 |
| `if (type >= 150) { switch A,B } else { switch low }` | 1372 | 76.4 |
| `if (type < 150) { switch low } else { switch A,B }` | 1424 | 23.0 |
| two switches: A then B (above) | 1508 | **98.2** |
| two switches: high then low | 1408 | 55.7 |
| three switches: A, B, low | 1480 | 63.6 |
| grouped A: `case 150..189:` → nested A switch | 1504 | 56.0 |
| grouped low: `case 0,8,11,13,14:` → nested low switch | 1444 | 63.0 |
| if/else chain, `__builtin_expect(cond,1)` | 1460 | 15.4 |
| if/else chain, plain conds | 1448 | 41.4 |
| `switch (__builtin_expect(type, K))` | 1360–1456 | 8.5–69.1 |
| — only K = 14 keeps 1432 | 1432 | 34.9 |
| full A hole-fill (12 holes, one per A body) | 1376 | 50.9 |
| A hole-fill + low hole-fill (9,10,12) | 1380 | 42.3 |
| partial A hole-fill (k = 0..6) | 1412–1424 | 50.9–69.1 |
| partial A hole-fill (k = 7) | 1436 | 50.9 |
| partial A hole-fill (k = 8..16) | 1364–1376 | 50.9 |
| random hole assignments (400 samples) | — | **0 hits at 1432** |
| random source case orderings (300 samples) | 1432 (26 hits) | 76.4 (all) |

`switch (__builtin_expect(type, K))` sets exactly one branch weight to 2000
(`LowerExpectIntrinsic::handleSwitchExpect`); every other weight is 1, so it
cannot express A > B.  `__builtin_expect_with_probability` does not exist in
clang 7.0.2.  The if/else-chain → switch fold produces uninitialised
`branch_weights` (a clang-7 bug) and only the first 7 successors are
non-zero, so it cannot express A > B either.

## 5. Root cause

The original's body order is the source order, which requires the high
jump-table edges to carry `branch_weights` with A > B **and** the low bodies
to stay at least as hot as A.  With the 16 high-range gaps, the low bodies
are already hotter than A, so A > B alone would suffice — but every
multiplicity trick that produces A > B in plain C++ (extra case labels on
the A bodies) also removes the gaps and pushes the low bodies below A.  The
best 1432-B plain-C++ layout is therefore the base value order (70.99 %),
and the best 1432-B weight layout (78.3 %) is not expressible from source.
The shipped `.so` was almost certainly PGO-built.

## 6. Reproduction

```sh
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w7507/score1.py        # 70.992366  523 525  1432/1432
cmake --build cmake-build-match --target check -j6   # RATCHET PASS, byte 4506

# exact-order two-switch shape (plain C++, 1508 B):
#   /dev/shm/w7507/s_twosw_AB.cpp
# exact-order !prof variant (1436 B, 75.95 %):
#   python3 /dev/shm/w7507/prof.py p.ll 1 200 201 200 199
#   bash /dev/shm/w7507/buildll.sh p.ll v.o && bash /dev/shm/w7507/relink.sh v.o
#   python3 /dev/shm/w7507/score1.py
```

Scratch (not committed): `/dev/shm/w7507/` — `base2.ll` (current IR),
`mkll.sh`/`buildll.sh`/`relink.sh`, `prof.py`/`profev.py` (weight patch +
size/order), `sweepw.py` (the 4608-point `!prof` sweep), `ev.py` (size +
TBH body order), `score1.py` (official single-symbol score),
`sodump.py`/`order2.py` (linked body addresses), `gen4.py` (case-group
generator), `s_twosw_AB.cpp`, `s_ifhigh.cpp`, `s_chain.cpp`, `h_*.cpp`,
`k_*.cpp`.
