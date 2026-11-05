# `Generator::createMission` — wave 116 (w6409): LSDA 172 vs the store/null placement

Target: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(original `0x971b8`, 1996 B, `.ARM.extab` LSDA record **172 B**, `tlen=151`, 26 call-site rows).

Status: **research note, no source change landed.** The wave-114/115 shape
(`volatile double padA` + `volatile int amount`, `__builtin_expect(pick == 10, 1)`,
`__builtin_expect(pick == 12, 0)`, `__builtin_expect(((1U << type) & 0x4801U) != 0, 1)`)
remains the unique best variant: **83.3 % official / 1996 B / extab 168 B**.

Gate at the start and end of this pass (full `check`):

```
verify: byte 4501  linked 4501  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓   drift: 0 field(s) across 0 class(es)
```

27 sections, file size 2,187,328 B, `.ARM.extab` `0x26294` (4 B short of `0x26298`).
`git status --short` is empty after the note commit.

## 0. Corrected `.ARM.extab` accounting

A full walk of every `.ARM.exidx` entry (compact + extab) in both binaries shows
**exactly one** record differs:

```
orig total 184811   ours 184807
0x971b8   172  168        <- Generator::createMission, the only delta
```

`Hud::draw` is **not** +4. The wave-115 note's claim that `Hud::draw`'s record is
480 vs 476 and cancels the shortfall is wrong; fixing this record to 172 B alone
restores `.ARM.extab` size-exactness (`0x26294 -> 0x26298`).

## 1. Where the 4 bytes come from

Original call-site table (26 rows, `tlen=151`, record 172) vs ours (25 rows,
`tlen=147`, record 168). The missing row is an `lp=0` entry:

```
orig [6]: (944, 24, 1878, 0)   [0x97568,0x97580)  case-15 try-range
orig [7]: (968, 32,    0, 0)   [0x97580,0x975A0)  lp=0 gap   <-- extra row
orig [8]: (1000, 56, 1934, 0)  [0x975A0,0x975D8)  case-0 invoke range
ours [6]: (960, 24, 1876, 0)   [0x97578,0x97590)
ours [7]: (994, 54, 1896, 0)   [0x9759A,0x975D0)  one merged range
```

The original places the `getRoutes() == nullptr` block
(`type = 4; if (nextInt(2) == 0) type = 1;`) at **`0x97586`**, between the
case-15 body and the case-0 body, i.e. *after* the first `lp=0` range
`[0x971b8,0x97404)`. Its `nextInt(2)` call therefore sits in a gap between two
try-ranges and gets its own `lp=0` entry.

Ours places the same block at `0x973f8`, immediately before `getName`
(`0x97410`), inside the first `lp=0` range `[0x971b8,0x97416)`, so it merges
into entry 0 and costs nothing.

The original also has a *single* shared `type` store at `0x973f8`
(`str r0,[sp,#36]`) that both the `type = 8` arm (`0x97354 b.n 973f8`) and the
`type = 13` arm (`0x973f6` falls through) branch to, placed immediately before
`getName` (`0x973fa`). Ours has that store at `0x97354` (right after the
`type = 8` arm) and a second store at `0x9740a` (null block), with `getName` at
`0x97410`.

So the whole function is the same code, **shifted by +0x12** after `0x973f8`;
the 4-byte LSDA delta is a pure basic-block-placement difference.

## 2. Why MBP puts the store after the `type = 8` arm

`MachineBlockPlacement` merges the `type` store block `M` into the chain after
`bb.32` (the `type = 8` arm). `hasBetterLayoutPredecessor(bb.32, M)` is false
because `M`'s other predecessor, `bb.48` (the `type = 13` arm), is too cold:

```
HotProb = StaticLikelyProb = 0.8
BadCFGConflict  iff  freq(bb.48) * 0.8 >= freq(bb.32) * 0.2
                iff  freq(bb.48) >= freq(bb.32) / 4 = 0.125
```

In the baseline `freq(bb.32) = 0.5` and `freq(bb.48) ≈ 3.8e-5`:

* `JumpThreading` redirects the `type >= 15` edge of `%117`
  (`icmp ult i32 %111, 15`) straight to `getName` (`%177`), because at the
  race-check block `%132` the condition `%135 = icmp eq i32 %111, 13` is
  provably false. So `bb.37` (race check) is only reached through the
  `mask == 0` edge.
* `__builtin_expect(((1U << type) & 0x4801U) != 0, 1)` makes that edge 0.05 %.
* The two loop back-edge heuristics (`124:4`) then scale the exit frequency of
  `while (targetStation == ...)` and `while (!found)` by 1/32 each.

The chain therefore reaches `bb.32` first, accepts `M` (the `type = 13`
predecessor is 3000x too cold), and later reaches `bb.48`, rejects `M` again,
and places the null block `bb.50` after `bb.48` instead.

## 3. The masked `type == 13` test fixes the target but not the record

`if (race < 4 && (type & 0xff) == 13)` defeats the `JumpThreading` correlation
(the `and` is a different SSA value), so the emitted `bhi.n` targets the
race-check block exactly like the original (`0x9734a -> 0x97378`). But it also
changes register allocation: `type` stays in `r5` across the whole region, the
shared store block disappears, and the single store is sunk to after the
difficulty branch:

```
mask   64.4 %  1972 B  tlen 147 rows 25 rec 168   stores 0x97224,0x97418  getName 0x973f4
```

Raising `freq(bb.48)` is therefore not enough: the store block must also stay
memory-resident. `volatile int32_t type` keeps it in memory and does produce
26 rows, but costs 40 B and a large codegen regression:

```
volatile int32_t type          53.5 %  2036 B  tlen 152 rows 26 rec 176
mask + volatile int32_t type   53.8 %  2040 B  tlen 152 rows 26 rec 176
*(volatile int32_t*)&type==13  54.2 %  2036 B  tlen 141 rows 24 rec 164
```

All 26-row variants found have `tlen = 152` (not the original's 151), so even
the row count alone does not reproduce the record.

## 4. New measurements this pass

### 4.1 Source shapes that are byte-identical to the baseline (83.3 % / 1996 / 168)

```
if (race < 4) { if (type == 13) { ... } }        nested ifs; SimplifyCFG re-merges
if (type >= 15) goto raceCheck; ... raceCheck:   goto form; same IR
(type ^ race) == (13 ^ race)                     same IR after instcombine
if (type == 3 || type == 5) with 7,9,8,11 order  neutral (compiler already merges)
```

`JumpThreading`/`SimplifyCFG` re-derive the single `and i1` on the race check,
so none of these can keep the `type >= 15` edge pointing at the race-check
block.

### 4.2 Hint grid (all on top of the baseline)

```
F1   (expect(routes != nullptr, 1))   48.0 %  2036 B  147/25/168
     null block moved right before getName (nextInt(2) at 0x973ce,
     getName at 0x973e2) and the found loop outlined to 0x978e0; the
     null block is still inside the first lp=0 range, so no new row.
F0   (expect(routes == nullptr, 0))   64.5 %  1996 B  147/25/168  store 0x97760
F1+G1                                 44.7 %  2032 B  152/26/176
B1+C0                                 57.9 %  1988 B  147/25/168
volatile type + C0                    51.5 %  2028 B  152/26/176
volatile type + B1                    52.6 %  2024 B  152/26/176
t8rew (expect(type == 8, 0) on the
       reward chain)                  75.7 %  1992 B  152/26/176
```

`B1+C0` is the case the frequency model predicts should flip the store
(`freq(bb.37) ~ 0.5`, `freq(bb.48) ~ 0.156 > 0.125`), but it does not: `B1`
already makes `type` register-resident, so there is no store block to place.

### 4.3 172-byte records that are size coincidences

```
A1+B0+E0   38.6 %  2060 B  tlen 148 rows 25 rec 172
A1+E0+F0/F1/G0, B0+E0+F0/F1/G0   38-39 %  172 B / 25 rows
```

These are `tlen 148 + 25 rows` (the original is `151 + 26`). `.text` grows to
`0x133808` (+0x40) and `byte_exact` collapses to 2267, so they fail the
ratchet. They are not the real shape.

### 4.4 Reward-chain order

The v2 ARM oracle (`_ALL_FUNCTIONS.c` line 50275) and the original disassembly
(`0x97690..0x97902`) both show the chain order `type==9 -> 7 -> 8 -> 3||5 -> 11`.
Rewriting our chain to that order **lowers** the score:

```
9,7,8,3||5,11   79.8 %  2000 B  147/25/168
7,9,8,3||5,11   80.2 %  2000 B  147/25/168
7,9,8,11,3||5   83.3 %  1996 B  147/25/168   (current, best)
```

So the current `else if` order is the one the block placer wants; the source
order is not recoverable from the emitted comparison order.

### 4.5 Frame / declaration variants

The original frame is 96 B; the baseline's `volatile double padA` makes it
112 B. Removing it restores the frame but loses 16 B of code and ~4 % score:

```
padA float       79.5 %  1992 B      padA int       79.1 %  1992 B
padA int64       79.2 %  1996 B      padA uint32    79.1 %  1992 B
padA no store    71.9 %  1984 B      padA late      82.8 %  1996 B
padA after level 75.3 %  1996 B      amount first   75.3 %  1996 B
padA before race 70.2 %  2008 B      amount uint32  83.1 %  1996 B
amount int64     67.7 %  2036 B/176  amount short   77.5 %  2008 B
type no init     77.4 %  1992 B      selected no init 76.9 % 1992 B
guard 1000       83.2 %  1996 B      level = 0      83.3 %  1996 B
amount = 0       80.6 %  1996 B      int race       83.2 %  1996 B
```

## 5. Conclusion

The remaining 16.7 % is still the two coupled `MachineBlockPlacement`
decisions:

1. the shared `type` store block is placed after the `type = 8` arm instead of
   after the `type = 13` arm, and
2. consequently the cold `getRoutes() == nullptr` block is placed before
   `getName` instead of inside the `switch` region, which is the sole remaining
   `.ARM.extab` delta.

A faithful source shape needs **both** the `type >= 15 -> race-check` edge
(i.e. no `JumpThreading` correlation) **and** a memory-resident `type` (so the
shared store block exists). Every plain-C++ spelling found that achieves the
first (masked compare, volatile type) destroys the second, and vice versa.
No variant measured this pass beats **83.3 % / 1996 B / extab 168 B**.

## 6. Reproduction

```sh
cp /dev/shm/w6409/Generator.base.cpp src/game/mission/Generator.cpp
cmake --build cmake-build-match --target check -j6

# masked type-13 test (right branch target, wrong regalloc)
sed -i 's/if (race < 4 \&\& type == 13)/if (race < 4 \&\& (type \& 0xff) == 13)/' \
    src/game/mission/Generator.cpp
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w6409/score1.py     # 64.4 % 1972 B extab 168

# volatile type (26 rows, 176 B)
sed -i 's/    int32_t type = 0;/    volatile int32_t type = 0;/' \
    src/game/mission/Generator.cpp
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w6409/score1.py     # 53.5 % 2036 B extab 176
```

Scratch files: `/dev/shm/w6409/` (`score1.py`, `lsda.py`, `extab.py`, `t2.py`,
`runvar.sh`, `batchM..Z.py`, `Generator.base.cpp`, `orig_full2.txt`,
`ours_base.txt`). Not part of the repo.
