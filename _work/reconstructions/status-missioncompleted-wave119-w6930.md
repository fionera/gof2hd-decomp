# `Status::missionCompleted` — wave 119 (w6930): the 192-byte rank table,
# the original case order, and the plain-switch experiment

Status: **research note (no source change landed)**.  Worktree `wt/w6930`,
branch `wave119-w6930`, base master `9c284a233` (24 commits after `be3034dc1`).
Original binary: `../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a,
NDK r18b clang 7.0.2 r328903, `-Oz`, image base 0x10000).

Target: `_ZN6Status16missionCompletedEbbx` (`Status::missionCompleted(bool,
bool, long long)`), original `0xabf38`, size **1432 B**.  Baseline row:
**92.5 % / 1432 B** (byte False, linked False).  Gate after this pass (source
unchanged):

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
# verify: byte 4505  linked 4505  stubs 0  missing 0  extra 0  avg 99.94
# data:   exact 843 of 843 exported objects
# sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
# RATCHET PASS
```

Structural state measured fresh in this worktree (the wave-119 handoff's
"4 B short" is stale; the current tree is the `298979092` state):

| section | original | ours | delta |
|---|---:|---:|---:|
| `.text` | 0x1337c8 | 0x1337c8 | exact |
| `.ARM.exidx` | 0x6f70 | 0x6f70 | exact |
| `.ARM.extab` | 0x26298 | 0x26290 | **-8** |
| `.rodata` | 0x41b67 | 0x41c27 | **+0xc0** (192 = the rank table) |

27 section headers, exact file size 2,187,328 B.  The rank table is the only
extra `.rodata` input and sits at the first byte past the original image,
`0x20b977` (original `.rodata` is `0x1c9e10..0x20b977`; its last bytes are
`"BAL__N_120PostfixQualifiedType\0"`).

## 1. The 192-byte table at 0x20b977 (dumped from the linked `.so`)

`tools/mission_case_table.cpp` is `unsigned char[192]`, indexed by mission
type (0..191); 250 is the "default" sentinel.  Exact contents:

```
idx  0: 200 250 250 250 250 250 250 250 208 250 250 211 250 213 214 250
idx 16: 250 250 250 250 250 250 250 250 250 250 250 250 250 250 250 250
idx 32: 250 250 250 250 250 250 250 250 250 250 250 250 250 250 250 250
idx 48: 250 250 250 250 250 250 250 250 250 250 250 250 250 250 250 250
idx 64: 250 250 250 250 250 250 250 250 250 250 250 250 250 250 250 250
idx 80: 250 250 250 250 250 250 250 250 250 250 250 250 250 250 250 250
idx 96: 250 250 250 250 250 250 250 250 250 250 250 250 250 250 250 250
idx112: 250 250 250 250 250 250 250 250 250 250 250 250 250 250 250 250
idx128: 250 250 250 250 250 250 250 250 250 250 250 250 250 250 250 250
idx144: 250 250 250 250 250 250   0   1   2   3   4   5  14  15   6 250
idx160:  16 250   9  17  18  19  10  21  20  22  11  23  39 250  25 250
idx176: 250 250 250 250 250 250 250 250  12 250 250 250 250  13 250 250
```

Non-250 entries (type -> rank):

```
0->200  8->208  11->211  13->213  14->214
150->0  151->1  152->2  153->3  154->4  155->5
156->14 157->15 158->6  160->16 162->9  163->17 164->18 165->19
166->10 167->21 168->20 169->22 170->11 171->23 172->39 174->25
184->12 189->13
```

Read in increasing rank, the table spells the original body layout order:

```
0(150) 1(151) 2(152) 3(153) 4(154) 5(155) 6(158) 9(162) 10(166)
11(170) 12(184) 13(189) 14(156) 15(157) 16(160) 17(163) 18(164)
19(165) 20(168) 21(167) 22(169) 23(171) 25(174) 39(172)
```

The two jump tables emitted by the current build are exactly the original's
shape: the big table is 40 halfwords at `0xae` (dispatch
`ldr/ldrb/cmp #39/bhi/tbh`), the small table is 7 halfwords at `0x11a`
(dispatch `sub #208/cmp #6/bhi/tbh`).  The dispatch costs one extra `ldrb`
(2 B) plus the table-address literal-pool word (4 B) inside body 189, which
is why the rank-table build is 1432 B and the plain switch is 1424 B.

## 2. The original case order, decoded from the disassembly

The original big-table dispatch is `sub.w r1,r0,#150; cmp r1,#39; bhi; tbh
[pc,r1,lsl#1]` at `0xaa`; table base `0xae`.  Target = `0xae + 2*entry`.
Decoded (index = type-150):

```
  0 150->0x1d8   1 151->0x1de   2 152->0x1e4   3 153->0x218
  4 154->0x21e   5 155->0x236   6 156->0x3f2   7 157->0x41c
  8 158->0x248   9 159->0x522  10 160->0x450  11 161->0x522
 12 162->0x2a4  13 163->0x47e  14 164->0x49a  15 165->0x49e
 16 166->0x30a  17 167->0x0fe  18 168->0x4a6  19 169->0x4c4
 20 170->0x36e  21 171->0x4c8  22 172->0x4e2  23 173->0x522
 24 174->0x0fe  25..33 ->0x522  34 184->0x37c  35..38 ->0x522
 39 189->0x3b6
```

The small table (`sub #8; cmp #6; tbh` at `0x116`, base `0x11a`) is
`8->0x128, 11->0x18c, 13->0x196, 14->0x1a4`, default `0x522`; type 0 is the
`bhi` target `0x186` (`cmp r0,#0; bne 0x522; cmp r8,#0; bne 0x4cc`).

Body layout order by address (the "original case order"):

```
0x0fe [167,174]  0x1d8 150  0x1de 151  0x1e4 152  0x218 153
0x21e 154        0x236 155  0x248 158  0x2a4 162  0x30a 166
0x36e 170        0x37c 184  0x3b6 189  0x3f2 156  0x41c 157
0x450 160        0x47e 163  0x49a 164  0x49e 165  0x4a6 168
0x4c4 169        0x4c8 171  0x4e2 172  0x522 default/latch
```

This is two strictly increasing value runs:

```
A = 150 151 152 153 154 155 158 162 166 170 184 189
B = 156 157 160 163 164 165 168 169 171 172
```

`167/174` is the shared two-label body and is placed first (frequency 2/N);
the rest of A is placed before B.  The A-before-B split is the only reason
the rank table exists: a plain switch's jump-table successor list is
value-sorted, so clang 7's MachineBlockPlacement lays the bodies out in
ascending type order, not A-then-B.

## 3. The plain-switch experiment (drop `tools/mission_case_table.cpp`)

The switch was rewritten as a plain `switch (m->getType())` with the real
type labels in the current source order (low group, 167/174, A, B), and the
`MISSION_CASE` macro/table reference removed.  Measured with the same
`link_so` + single-function score as the baseline:

| variant | usize | score | layout order |
|---|---:|---:|---|
| **committed rank table** | **1432** | **92.5** | original A-then-B |
| plain switch, source order low/167/A/B | 1424 | **58.4** | value order |
| plain switch, source order 167/A/B/low | 1368 | 52.5 | value order |
| plain switch, source order A/B/167/low | 1368 | 52.5 | value order |
| plain switch, source order A/167/B/low | 1368 | 52.5 | value order |
| plain switch, source order low/A/B/167 | 1424 | 58.0 | value order |
| plain switch, source order low/167/B/A | 1424 | 56.5 | value order |
| two sequential switches (cached `int mtype`) | 1504 | 47.1 | original A-then-B |
| two sequential switches (re-query `getType()`) | 1452 | 45.4 | (3 tables) |
| `!prof` IR, A=2/B=1 (hand-edited `plain.ll`, w6836) | 1368 | 47.1 | original A-then-B |

The plain switch's best measured score is **58.4 % at 1424 B** (the current
source order).  Source order changes the *size* (1368/1424) but never the
layout order: the big TBH is always value-sorted, so the body sequence is
`150,151,152,153,154,155,156,157,158,160,162,163,164,165,166,168,169,
170,171,172,184,189` instead of the original's A-then-B.

Why it cannot be fixed in plain C++: the original's layout is the MBP
*frequency* order (`A=2, B=1`); the only plain-C++ way to change switch-edge
frequencies is to give a body more than one case label.  `167/174` does that
naturally (and is placed first), but the A bodies each have exactly one
type.  `__builtin_expect` on the switch operand becomes a `select`; an
`if`-chain loses the jump table; `!prof` metadata is not expressible from
C++.  The two-switch shape reproduces the layout but emits three jump tables
and is 1504 B (47.1 %).  This re-confirms the w6815/w6836 conclusion.

## 4. Structural effect of dropping the table

Removing `mission_table` from the link (manual gold link with the same
objects, table object omitted) changes **only** `.rodata`:

```
[15] .rodata  PROGBITS  001c9e10 1c9e10 041b67   (was 041c27)
```

`.text`, `.ARM.exidx`, `.ARM.extab` (still `0x26290`), all section bases,
the 27-section count and the file size (2,187,328 B) are unchanged.  So
dropping the table would make `.rodata` size-exact (the docs' "only 192 B of
extra `.rodata`" delta disappears) at the cost of the function's fuzzy score
(92.5 -> 58.4 %) and a small `avg_match` drop.  It is **not** a strict gate
improvement: `byte_exact`/`linked_exact` stay 4505 (the function is not
byte-exact either way), `data_exact` stays 843 (the table is hidden, not an
exported object), and the import set is unchanged.  Not committed for that
reason.

## 5. Proven vs hypothesis

**Proven (built and measured):**

1. The 192-byte table at `0x20b977` is exactly the bytes in §1; it is the
   only extra `.rodata` input and the only thing that makes `.rodata`
   `0x41c27` instead of `0x41b67`.
2. The original case order is A-then-B (§2), decoded from the 40-entry TBH.
3. A plain `switch (m->getType())` with the real type labels produces the
   value-sorted layout at 1424 B / 58.4 % (best plain order tried); other
   source orders give 1368 B / 52.5 % or 1424 B / 56.5-58.0 %.
4. Removing the table object makes `.rodata` size-exact with no other
   section/base/file-size change.
5. The two-switch shape reproduces the A-then-B layout but is 1504 B /
   47.1 % (cached `mtype`) or 1452 B / 45.4 % (re-query).

**Hypothesis / open:** the original's A=2/B=1 switch-edge frequencies are
the only remaining route to a byte-exact body order; they are not
expressible in plain C++ with clang 7 (the `!prof` IR proof is w6836's
1368 B / 47.1 % shape).  The current rank table is the best plain-C++
approximation: it reproduces the original layout and the 1432 B total, at
the cost of one `ldrb` and a capped 18/40 TBH positional match.

## 6. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4505

# single-function score (fast, ~2 s): compile Status.cpp + link + verify one symbol
python3 /dev/shm/w6930/score.py _ZN6Status16missionCompletedEbbx

# dump the table
python3 - <<'PY'
import sys; sys.path.insert(0,'tools/verify')
from elf import Elf32
b=Elf32('cmake-build-match/libgof2hdaa.so').bytes_at(0x20b977,192)
for i in range(0,192,16): print(' '.join('%3d'%x for x in b[i:i+16]))
PY
```

Scratch (not committed): `/dev/shm/w6930/` — `score.py` (single-function
verify score, reuses `tools/verify/verify.py`), `Status.orig.cpp` (baseline),
`Status_plain.cpp` (plain switch), `Status_twoswitch.cpp` /
`Status_twoswitch_rq.cpp`, `Status_lowlast.cpp`, `reorder2.py` (case-order
sweep), `link_notable.sh` (manual gold link without the table object),
`mbp.log` / `mbp_plain.log` (instrumented-llc MBP traces).
