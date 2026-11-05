# `Status::missionCompleted` — wave 115: rank table re-based to 0..39 so the
# dispatch drops the `sub` (w6311)

Status: **landed**.  `_ZN6Status16missionCompletedEbbx` **88.3 % → 91.5 %**
(1432 B, unchanged size).  Gate: `RATCHET PASS`, `byte_exact 4500`,
`linked_exact 4500`, imports 207, `lint CLEAN`, `drift 0`, 27 sections, exact
file size 2,187,328 B, `.ARM.extab` 0x26298 (size- and base-exact),
`.rodata` +0xc0 (the rank table, as before).  Worktree `wt/w6311`, branch
`wave115-w6311`, base master `b847d5d46`.

## 0. What changed

Two edits:

1. `tools/mission_case_table.cpp` — the rank table is re-based.  The first
   group now maps the 23 case bodies to **ranks 0..21 + 39** (40 entries,
   range starting at 0); the second group (types 0, 8, 11, 13, 14) maps to
   **ranks 200..214**; the default rank is 250.
2. `src/game/mission/Status.cpp` — the `case` labels follow the new ranks
   (`case 0..21`, `case 39`, `case 200/211`, `case 208`, `case 213`,
   `case 214`).

Because the switch is on the rank and the rank range now starts at 0, clang
emits **no `sub.w`**:

```
ldr  r1, [pc, #804]     ; MISSION_CASE pointer
ldrb r0, [r0, r1]       ; rank = MISSION_CASE[type]
cmp  r0, #39
bhi  <second group>
tbh  [pc, r0, lsl #1]
```

That is 12 B (the `ldr` is the 16-bit literal form), exactly the original's
12-byte `sub/cmp/bhi/tbh` prefix, and the 80-byte (40-entry) TBH then lands
the first case body at the original's **0xfe**.  The previous 37-entry table
needed an 18-byte prefix (`ldr/ldrb/sub/cmp/bhi/tbh`) and a 74-byte table to
reach the same 0xfe, which cost 4 B of body alignment elsewhere.

The second group stays a separate `sub #208; cmp #6; tbh` because ranks
200..214 are far from 0..39, mirroring the original's separate `sub #8;
cmp #6; tbh` for types 8..14.

## 1. Rank map

| original type | rank | body |
|---|---:|---|
| 167, 174 | 0 | getStatusValue >= getProductionGoodAmount |
| 150 | 1 | missionCount |
| 151 | 2 | kills |
| 152 | 3 | equipment getIndex scan |
| 153 | 4 | stationsVisited |
| 154 | 5 | currentLoad |
| 155 | 6 | goodsProduced |
| 158 | 7 | equipment getType/getSort scan |
| 162 | 8 | blueprint ingredients |
| 166 | 9 | production good / hasEquipment |
| 170 | 10 | statusValue == 1 |
| 184 | 11 | statusValue == 0 / campaign 0x5c |
| 189 | 12 | equipment getSort scan |
| 156 | 13 | !atStation && target && extra > 10000 |
| 157 | 14 | equipment getType scan |
| 160 | 15 | atStation / target + extra |
| 163 | 16 | field_90 all-negative |
| 164 | 17 | longMission |
| 165 | 18 | notAtStation && target |
| 168 | 19 | field_178 >= statusValue |
| 169 | 20 | shortMission |
| 171 | 21 | dockedAtStation && target |
| 172 | 39 | dockedAtStation && target + production good |
| 0 | 200 | atStation && target (shares body with 11) |
| 8 | 208 | campaign 0x8f + production good |
| 11 | 211 | atStation && target |
| 13 | 213 | atStation && field_0xf0 |
| 14 | 214 | atStation && agent station + isInList |
| everything else | 250 | default |

## 2. Resulting body layout (function-relative)

Original and ours after this pass (TBH-decoded, `base = tbh+4`):

```
body    orig   ours
0       0xfe   0xfe   (167/174)
1       0x1d8  0x1d8  (150)
2       0x1de  0x1de  (151)
3       0x1e4  0x1e4  (152)
4       0x218  0x218  (153)
5       0x21e  0x21e  (154)
6       0x236  0x236  (155)
7       0x248  0x248  (158)
8       0x2a4  0x2a4  (162)
9       0x30a  0x30a  (166)
10      0x36e  0x36a  (170)   -4  (body 9 is 4 B short)
11      0x37c  0x378  (184)   -4
12      0x3b6  0x3b2  (189)   -4
13      0x3f2  0x3f6  (156)   +4  (body 12 is 8 B long: literal pool)
14      0x41c  0x420  (157)   +4
15      0x450  0x454  (160)   +4
16      0x47e  0x482  (163)   +4
17      0x49a  0x49e  (164)   +4
18      0x49e  0x4a2  (165)   +4
19      0x4a6  0x4aa  (168)   +4
20      0x4c4  0x4c4  (169)    0
21      0x4c8  0x4c8  (171)    0
22      0x4e2  0x4e2  (172)    0
latch   0x522  0x522           0
return  0x566  0x566           0
setWon  0x570  0x570           0
```

(Offsets are function-relative, decoded from the TBH in each binary.)

## 3. Why the rank table is still needed

`switch (m->getType())` with the 23 cases written in the original body order
(167/174, 8, 0, 11, 13, 14, 150, 151, ... 172) still emits the bodies in
**ascending case-value order** (150, 151, ..., 172), because clang 7 builds
the jump table value-sorted and `MachineBlockPlacement` walks the jump-table
successor list in that order.  That direct-switch variant measures
**58.2 % / 1424 B**.  The rank table reorders the *values* so the value-sorted
order is the original body order.

## 4. Remaining differences (all measured)

- **Dispatch** (4 slots): ours `ldr/ldrb/cmp/tbh` vs original
  `sub/cmp/bhi/tbh`.  Same 12 B, but the rank indirection is unavoidable
  while the table lives at the fixed address 0x20b977.
- **Second-group dispatch**: `sub #208` vs original `sub #8`; the outer
  default check is `cmp #200` vs original `cmp #0`.  Same size.
- **Body 9 (type 166)** is 4 B short: ours keeps `index` in `r5` and
  `amount` in `r6` (saving `this` in `r1`), the original keeps `index` in
  `r9`, `amount` in `r5`, `this` in `r6`, and re-queries
  `getProductionGoodIndex()` for `hasEquipment`.  Re-querying adds 8 B and
  overshoots to +4 (1440 B total), so it was not kept.
- **Body 12 (type 189)** is 8 B long: clang places the first constant pool
  (`MISSION_CASE` + `Globals::status`, 8 B) after body 11, inside body 12.
  The original has no `MISSION_CASE` literal and places its first
  `Globals::status` literal at 0x4c0 (after body 19).
- **Body 19 (type 168)** is 4 B short: the `(unsigned short)` cast emits a
  `uxth` that fills the original's trailing `nop`+literal-pool padding.
  Removing it drops the score to 74.5 %.

## 5. Dead ends measured this wave

| variant | pct | usize |
|---|---:|---:|
| HEAD (wave-114 37-entry rank table) | 88.3 | 1432 |
| direct switch, original body order | 58.2 | 1424 |
| direct switch, source order | 28.2 | 1364 |
| direct switch, descending source order | 50.1 | 1364 |
| rank table indexed by type (ranks = type) | 85.3 | 1432 |
| rank table with 187 instead of 186 | 89.3 | 1432 |
| ranks 150..172 in original layout order + second group 8..14 | 70.9 | 1404 |
| ranks 150..189 (40-entry) in original body order + second group 0/8/11/13/14 | 75.2 | 1436 |
| ranks 0..26 (26-entry table) | 54.0 | 1408 |
| **ranks 0..21+39 and 200..214 (this pass)** | **91.5** | **1432** |
| case 166 re-queries `getProductionGoodIndex()` | 75.6 | 1440 |
| case 168 without `(unsigned short)` | 74.5 | 1432 |
| case 166 re-query + case 168 no `(unsigned short)` | 75.6 | 1440 |
| `unsigned` index/amount in case 166 | 91.5 | 1432 |
| `const int` index/amount | 91.5 | 1432 |
| `cargo` local in case 166 | 91.5 | 1432 |
| `inList` bool local in case 166 | 91.5 | 1432 |
| `(unsigned short)` on both sides in case 168 | 91.1 | 1432 |
| `(unsigned)` cast in case 168 | 74.5 | 1432 |
| `this->ship` in case 166 | 91.5 | 1432 |
| `Globals::status->ship` in case 12 | 73.8 | 1444 |
| hoist `Status *g = Globals::status` | 44.8 | 1268 |
| swap source order of cases 11 and 12 | 91.5 | 1432 |
| move case 8 body to the end of the switch | 66.9 | 1384 |

## 6. Files

- `src/game/mission/Status.cpp` — rank-table case labels + comments.
- `tools/mission_case_table.cpp` — 192-byte type→rank table (first group
  0..21+39, second group 200..214, default 250).
