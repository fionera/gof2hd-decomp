# `Status::missionCompleted` — wave 112: body-order search (w6007)

Status: **research note only** (no source change). Worktree `wt/w6007`, branch
`wave112-w6007`, base master `5d98bd391`. Original binary:
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

## 1. Precise original decode (this pass)

`getType()` result is switched directly: `sub.w r1,r0,#150; cmp r1,#39; bhi
second; tbh` (40 entries, values 150..189), then `sub #8; cmp #6; bhi third;
tbh` (7 entries, values 8..14), then `cmp r0,#0`.  One plain switch with cases
`{0, 8..14, 150..189}`.

First TBH (value -> absolute target):

```
150 ac110 151 ac116 152 ac11c 153 ac150 154 ac156 155 ac16e 156 ac32a
157 ac354 158 ac180 159 ac45a 160 ac388 161 ac45a 162 ac1dc 163 ac3b6
164 ac3d2 165 ac3d6 166 ac242 167 ac036 168 ac3de 169 ac3fc 170 ac2a6
171 ac400 172 ac41a 173 ac45a 174 ac036 175..183 ac45a 184 ac2b4
185..188 ac45a 189 ac2ee
```

Sorted by address (the true original body order, 23 distinct bodies):

```
167/174, 8, 11, 13, 14, 150, 151, 152, 153, 154, 155, 158, 162, 166,
170, 184, 189, 156, 157, 160, 163, 164, 165, 168, 169, 171, 172, latch
```

This is **not** the value order.  The landed rank table is exactly the
permutation that makes the switch's value order equal this body order, which
is why it reproduces the body layout (deltas only).

## 2. Why a plain single switch cannot reproduce the order

Empirically (NDK clang 7.0.2, `-Oz`), a plain `switch (m->getType())` with
cases `{0,8,11,13,14,150..189}` lays the bodies out in **jump-table
(value-sorted) order**, with the shared body (167/174, two case edges) first:

```
167/174, 8, 11, 13, 14, 150, 151, 152, 153, 154, 155, 156, 157, 158,
160, 162, 163, 164, 165, 166, 168, 169, 170, 171, 172, default, 184, 189
```

This is invariant under source case order: three different source orders
(current/rank order, original body order, fully reversed) all produced the
same layout (`S_dir_origorder.o`, `S_origbody.o`, `S_rev.o`).  A `default:
break;` also changed nothing.  The reason is `buildJumpTable`'s
`addSuccessorWithProb` loop, which adds the JT block's successors in table
(value) order, and MBP's `selectBestCandidateBlock` tie-break which keeps the
first equal-frequency worklist entry.

## 3. Two-switch shape *does* reproduce the order — but costs bytes

Splitting the cases into two sequential switches on the same value

```cpp
int mtype = m->getType();
switch (mtype) { case 167: case 174: ... case 189: }   // 17 cases
switch (mtype) { case 156: ... case 172: }             // 10 cases
```

makes the ten bodies of the second switch have **two predecessors**, which
delays their placement in MBP's worklist.  The resulting body order is exactly
the original one (verified on the object: `167/174, 150..155, 158, 162, 166,
170, 184, 189, 156, 157, 160, 163, 164, 165, 168, 169, 171, 172`), and the
dispatch is still the original 40-entry + 7-entry TBH chain.  The cost is the
extra live value across the two switches: `this` spills to the stack, the
prologue changes (`str r0,[sp,#16]` instead of `mov r6,r0`), and the function
grows to 1548 B (116 B over).  Writing `switch (m->getType())` twice (two
`getType` calls, no local) gives 1460 B.  Both score far below 87.0 %.

## 4. Measurements (full link + official verifier)

| variant | pct | usize | notes |
|---|---:|---:|---|
| **HEAD rank table (32-entry TBH, anchor)** | **87.0** | **1432** | baseline |
| rank table N=35 (last body rank 184) | 74.4 | 1440 | first body -4, latch +6 |
| rank table N=37 (last body rank 186) | 76.4 | 1444 | first 10 bodies exact, latch +10 |
| rank table N=37, no anchor | 78.0 | 1440 | first 10 bodies exact, latch +6 |
| rank table N=32, no anchor | 70.6 | 1428 | latch -4 |
| direct switch, original body order | 60.4 | 1428 | value-order layout |
| direct switch, fallthrough `{158,162,166,170,184,189}` | 44.3 | 1356 | order exact, 76 B short |
| two switches (`int mtype` local) | 46.8 | 1548 | order exact, `this` spilled |
| two switches (`getType()` twice) | 45.6 | 1460 | order exact, two calls |
| if/else-if chain (converted back to a 40-entry TBH) | 20.7 | 1436 | different CFG |
| anchor moved from case 159 to case 150 | 77.4 | 1440 | |

## 5. Offset arithmetic (why 87.0 is the ceiling for the rank-table shape)

Baseline body offsets vs original: first group (167/174 .. 166) **-10**,
middle (170, 184, 189) **-4**, last (156..168) **+6**, 169/171/172 and the
latch **0**.

* The dispatch is `ldr;uxtb;ldrb;sub;cmp;bhi;tbh` (18 B) + 32-entry table
  (64 B) = 82 B from the `sub` to the first body, vs the original's
  `sub;cmp;bhi;tbh` (8 B) + 80 B = 88 B: **-6 B**, and the TBH itself sits
  6 B later, so the first body is **-10 B**.
* The `case 159` size anchor adds 4 B in the type-166 body, lifting the
  middle group to **-4 B**.
* The extra literal-pool word for the table address (4 B + 2 B `nop`) sits
  between the 189 and 156 bodies, lifting the last group to **+6 B**; the
  type-168 body is 4 B shorter than the original there, which restores
  169/171/172 and the latch to **0**.

Matching the first group needs a 37-entry table (first body 0), but that
moves the latch to +10 and every `b <latch>` (the most numerous branch in the
function) then mismatches — net loss (76.4 %).  Matching the latch is worth
more than matching the early table entries, so the 32-entry table wins.

To be byte-exact one would need the original's 4-instruction dispatch
(`sub/cmp/bhi/tbh`, 40-entry table) **and** the original body order, i.e. a
plain single switch that MBP lays out in source order.  No such source shape
was found (waves 92-112); the two-switch shape reproduces the order but not
the register allocation / size.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6007
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
cmake --build cmake-build-match --target lint  -j6   # CLEAN
cmake --build cmake-build-match --target drift -j6   # 0 fields
```

Scratch (not committed): `/dev/shm/w6007/` — `cc.sh`, `fn.sh`, `tbh.py`
(TBH decoder), `layout.py`, `cat.py`, `measure.sh`, `measure2.sh`, `genN.py`,
`relabel.py`, `reorder.py`, `rmbreak.py`, `twosw.py`, `ifchain.py`,
`Status.orig.cpp`, `S_dir_origorder.cpp`, `S_origbody.cpp`, `S_ft.cpp`,
`S_twosw2.cpp`, `S_twosw3.cpp`, `S_ifchain.cpp`, `N35*/N37*` variants.
