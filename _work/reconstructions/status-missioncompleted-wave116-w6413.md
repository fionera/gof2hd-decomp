# `Status::missionCompleted` — wave 116: rank table re-based to the original
# TBH slot pattern (shared body at slots 0/24) (w6413)

Status: **landed**.  `_ZN6Status16missionCompletedEbbx` **91.5 % → 91.9 %**
(`exact ratio 91.499522 → 91.881566`), 1432 B, unchanged size.  Gate:
`RATCHET PASS`, `byte_exact 4501`, `linked_exact 4501`, imports 207,
`lint CLEAN`, `drift 0`, 27 sections, exact file size 2,187,328 B,
`.ARM.extab` 0x26294 (4 B short, unchanged), `.rodata` 0x41c27 (+0xc0, the
rank table).  Worktree `wt/w6413`, branch `wave116-w6413`, base master
`d672d349f`.

## 0. Ground truth re-checked this wave

The original dispatch at `0xabfda` (function-relative 0xa2) is:

```
abfd4: mov r0, sl
abfd6: blx 0x62874          ; m->getType()
abfda: sub.w r1, r0, #150   ; 0x96
abfde: cmp r1, #39          ; 0x27
abfe0: bhi.n 0xac046        ; second (low) group
abfe2: e8df f011  tbh [pc, r1, lsl #1]
abfe6: ... 40 halfwords ...
```

so the **original source is a plain `switch (m->getType())`** over types
150..189, and its high TBH is **value-ordered**: slot `s` is type `150+s`.
The low group is `sub #8; cmp #6; tbh` over types 8..14 (only 8, 11, 13, 14
present).  The original body *layout*, however, is **not** value order:

```
0xfe  167/174   0x1d8 150   0x1de 151   0x1e4 152   0x218 153
0x21e 154       0x236 155   0x248 158   0x2a4 162   0x30a 166
0x36e 170       0x37c 184   0x3b6 189   0x3f2 156   0x41c 157
0x450 160       0x47e 163   0x49a 164   0x49e 165   0x4a6 168
0x4c4 169       0x4c8 171   0x4e2 172   (latch 0x522, ret 0x566, setWon 0x570)
```

i.e. `L = [167/174, 150, 151, 152, 153, 154, 155, 158, 162, 166, 170, 184,
189, 156, 157, 160, 163, 164, 165, 168, 169, 171, 172]`.  The original's
MachineBlockPlacement laid the bodies out in `L`, not in the jump table's
value order; the two are decoupled in the original binary.

## 1. What changed

Only the rank permutation (both files), no body edits:

1. `tools/mission_case_table.cpp` — the first group's ranks are re-based so
   the **value-sorted slot order is `L`**: the shared 167/174 body is rank 0,
   150..155 are ranks 1..6, 158 rank 7, 162 rank 8, 166 rank 10, 170 rank 12,
   184 rank 13, 189 rank 14, 156..172 ranks 15..22 and 34, 172 rank 39.
   The default stays 250; the second group stays 200..214.  Type 174 now maps
   to rank **24** (not 0), so the `case 24:` label is live.
2. `src/game/mission/Status.cpp` — `case 0:` gains a `case 24:` label (the
   same body).  This makes the emitted 40-entry TBH put the shared body in
   slots 0 **and** 24, exactly as the original's table does (its slots 17 and
   24 both point at 0xfe), and moves the default entries to slots 9 and 11,
   which is also the original's pattern.

Old → new rank map (rank values in the table, not types):

```
0 -> {0,24}   1..6 -> 1..6   7 -> 7   8 -> 8   9 -> 10   10 -> 12
11 -> 13      12 -> 14       13 -> 15 14 -> 16  15 -> 17  16 -> 18
17 -> 19      18 -> 20       19 -> 21 20 -> 22  21 -> 34  39 -> 39
200/208/211/213/214 unchanged
```

Result: the emitted TBH is at `+0xaa` / table `+0xae`, `n=40`, second group
`n=7` at `+0x116`/`+0x11a` — same bases and sizes as the original.  Normalised
table diff (offline `difflib`) improves from **24/40 matching entries (0.60)**
to **26/40 (0.65)**:

```
orig  : 95 98 9b b5 b8 c4 a2 1b7 cd 23a 1d1 23a fb 1e8 1f6 1f8 12e 28 1fc 20b
        160 20d 21a 23a 28 23a... 167 23a... 184
align : 28 95 98 9b b5 b8 c4 cd fb 23a 12e 23a 15e 165 182 1a4 1b9 1d3 1ea
        1f8 1fa 1fe 20b 23a 28 23a... 20d 23a... 21a
```

The body layout produced by this permutation is byte-for-byte the original's
`L` (all 23 body entry offsets match: 0xfe, 0x1d8, 0x1de, 0x1e4, 0x218,
0x21e, 0x236, 0x248, 0x2a4, 0x30a, 0x36a(orig 0x36e), 0x378(0x37c),
0x3b2(0x3b6), 0x3f6(0x3f2), 0x420(0x41c), ... — the drift after body 166 is
the known 4 B body-166 shortfall).

## 2. Ceiling and why the table cannot be matched from this shape

Binary-patch experiment: replacing **only** our 80 TBH bytes at 0xabfe6 with
the original's gives **94.6 %** (`94.555874`).  So the table content is worth
~3.1 points and is the single largest remaining delta.

With the rank indirection the emitted TBH is in rank order, and the body
layout is also in rank order (clang 7 MBP follows the jump-table successor
list; verified with `llc` + `[MBS]`/`[SBCB]` instrumented LLVM 7 at
`/dev/shm/wf303/llvm-build`).  Hence a single rank permutation can match
either the table (value order) or the body layout (`L`), not both.  The
original matches both because its MBP chose `L` for a value-ordered table;
that is a property of the original's CFG/frequencies, not of the case order.
A micro-test with the shipped clang 7.0.2 (`switch` cases written 5,1,3,2,4)
confirms clang emits the bodies in **value** order, so the original's `L` is
not source order either.

## 3. Dead ends measured this wave (all reverted)

| variant | pct | usize |
|---|---:|---:|
| HEAD (wave-115 37-entry rank table) | 91.5 | 1432 |
| **align variant (landed)** | **91.9** | **1432** |
| binary-patched original TBH (not a source shape) | 94.6 | 1432 |
| direct `switch (m->getType())`, value order | 58.0 | 1424 |
| direct switch, physical (`L`) source order | 58.4 | 1424 |
| direct switch, high-group-first | 52.5 | 1424 |
| direct switch, reversed source order | 51.0 | 1424 |
| sequential two-switch split | 45.0 | 1444 |
| ranks = type (identity table) | 85.3 | 1432 |
| ranks 150..172 + 189 (23-entry table) | 70.9 | 1404 |
| ranks 150..189 (40-entry) in body order | 75.2 | 1436 |
| ranks 0..26 (26-entry) | 54.0 | 1408 |
| body 166: `unsigned` index/amount, `const`, `amount` first, `bool inList`, `this->ship`, `cargo` local | 91.3–91.5 | 1432 |
| body 166: re-query `getProductionGoodIndex()` / `ship->hasEquipment` | 75.6 | 1440 |
| body 168: no `(unsigned short)` | 74.5 | 1432 |
| body 168: `(unsigned short)` on both sides | 91.1 | 1432 |
| body 168: `(unsigned)` cast | 74.5 | 1432 |
| `do { switch } while (0)` wrapper | 91.5 | 1432 |
| `switch ((int) MISSION_CASE[...])`, `& 0xff`, `+ 0`, int local | 91.5 | 1432 |
| hoist `Status *g = Globals::status` | 44.8 | 1268 |
| swap source order of two cases | 91.5 | 1432 |

## 4. Remaining known deltas

- Dispatch: ours `ldr/ldrb/cmp/tbh` (rank indirection) vs original
  `sub #150/cmp #39/bhi/tbh`.
- Second group: `sub #208/cmp #6` vs original `sub #8/cmp #6`; outer default
  `cmp #200` vs `cmp #0`.
- Body 166 is 4 B short: ours keeps `index` in r5 / `amount` in r6 and reuses
  `index`; the original keeps `index` in r9 / `amount` in r5 / `this` in r6
  and re-queries `getProductionGoodIndex()`.  Re-querying adds 8 B and
  overshoots to 1440 B.
- Body 189 is 8 B long: the first literal pool (`MISSION_CASE` +
  `Globals::status`, 8 B) lands inside it; the original has no `MISSION_CASE`
  literal.
- Body 168 carries one extra `uxth` (the `(unsigned short)` size device that
  keeps the latch at 0x522).

## 5. Files

- `src/game/mission/Status.cpp` — `case 24:` on the shared 167/174 body.
- `tools/mission_case_table.cpp` — rank permutation above (192-byte table).
