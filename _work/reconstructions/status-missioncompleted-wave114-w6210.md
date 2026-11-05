# `Status::missionCompleted` — wave 114: 37-entry rank table lands the first
# body at 0xfe and the latch at 0x522 (w6210)

Status: **landed**.  `_ZN6Status16missionCompletedEbbx` **87.0 % → 88.3 %**
(1432 B, unchanged size; 84 → 74 mismatched instruction slots).  Gate:
`RATCHET PASS`, `byte_exact 4500`, `linked_exact 4500`, imports 207,
`lint CLEAN`, `drift 0`, 27 sections, exact file size 2,187,328 B,
`.ARM.extab` 0x26298 (size- and base-exact), `.rodata` +0xc0 (the rank table,
as before).  Worktree `wt/w6210`, branch `wave114-w6210`, base master
`d8557f7e2`.

## 0. What changed

Three edits, all in `src/game/mission/Status.cpp` (+ the rank table in
`tools/mission_case_table.cpp`):

1. The rank table now maps the 23 case bodies to labels **150..171 + 186**
   (was 150..171 + 181).  That is a **37-entry TBH** instead of 32.
2. `case 159` (original type 166) no longer carries the `Ship *s = ship;
   if (s == 0) continue;` size anchor, and the second call reuses the local
   `index` instead of re-calling `getProductionGoodIndex()`.
3. `case 169` (original type 168) compares against
   `(unsigned short) m->getStatusValue()`, which emits one `uxth` (2 B).

Net effect on the body offsets (function-relative, original in brackets):

| body | before (N=32) | after (N=37) | original |
|---|---:|---:|---:|
| 167/174 | 0xf4 | **0xfe** | 0xfe |
| 150..166 | 0x1ce..0x300 | **0x1d8..0x30a** | 0x1d8..0x30a |
| 170, 184, 189 | 0x36a, 0x378, 0x3b2 | 0x36a, 0x378, 0x3b2 | 0x36e, 0x37c, 0x3b6 |
| 156..168 | 0x3f8..0x4ac | 0x3f8..0x4ac | 0x3f2..0x4a6 |
| 169, 171, 172 | 0x4c4, 0x4c8, 0x4e2 | **0x4c4, 0x4c8, 0x4e2** | 0x4c4, 0x4c8, 0x4e2 |
| latch | 0x522 | **0x522** | 0x522 |
| return m / setWon | 0x566 / 0x570 | **0x566 / 0x570** | 0x566 / 0x570 |

The whole first group (167/174 through 166, the 17 bodies that clang lays out
in value order) is now **byte-offset exact**, and the tail (latch, `return m`,
`setWon`, 36 + 15 + 7 branch references) stays exact.  Before, the first group
was -10 and only the tail was exact.

## 1. Why 37 entries

The dispatch of the rank-table shape is

```
ldr r1,[pc,#N] ; uxtb r0,r0 ; ldrb r0,[r0,r1] ; sub.w r1,r0,#150
cmp r1,#(N-1) ; bhi second ; tbh [pc,r1,lsl#1] ; <2N bytes of table>
```

i.e. 18 B of prefix + 2N bytes of table.  The original is 12 B of prefix
(`sub.w/cmp/bhi/tbh`) + 80 B (40 entries).  The first body therefore lands at
`0xa2 + 18 + 2N`; the original lands at `0xa2 + 12 + 80 = 0xfe`, so
`2N = 74` → **N = 37**.  (N = 32, the previous value, puts it at 0xf4.)

The 37-entry table also shifts every body after the table by +10 relative to
N=32, which moves the latch from 0x522 to 0x528.  The two body-size edits
below take back exactly 6 B, restoring the latch (and the tail) to 0x522.

## 2. Why the `(unsigned short)` cast and the `hasEquipment(index, 1)`

With N = 37 and the old `case 159` anchor removed, the body-size deltas are
`body166 = -4` (the anchor is gone) and `body189 = +8` (the literal-pool island
grew), so the latch sits at 0x520.  Two bytes are needed.

* **`case 169` `(unsigned short)`** adds `uxth r0,r0` right after
  `getStatusValue()` and before `cmp r5,r0`, i.e. exactly 2 B.  Mission status
  values are 16-bit counters, so the truncation is semantically inert.  It is a
  size device, like the rank table itself.
* **`case 159` `hasEquipment(index, 1)`** replaces the original's second
  `getProductionGoodIndex()` call with the already-computed local.  That is the
  same value (the getter just returns a field) and it keeps the body 4 B
  shorter, which is what lets the 37-entry table's +10 be absorbed without
  spilling `this` (the old anchor forced `this` to `[sp,#16]`).

Note the ordering trap: the 2 B `uxth` has to be in body 168 (the last body
before the latch) because the literal-pool island is 4-byte aligned — adding
2 B anywhere before the island moves it by 4 B, and adding 2 B after it moves
the latch by 2 B.  Only the body-168 position gives exactly +2.

## 3. Measurements (official verifier, `verify/report.json`)

| variant | pct | usize | first body | latch |
|---|---:|---:|---:|---:|
| HEAD (N=32, anchor) | **87.0** | 1432 | 0xf4 | 0x522 |
| **N=37, no anchor, `index` reuse, `(unsigned short)`** | **88.3** | **1432** | **0xfe** | **0x522** |
| N=37, no anchor, `index` reuse (no cast) | 74.2 | 1432 | 0xfe | 0x520 |
| N=37, no anchor, `(unsigned short)` only | 75.1 | 1440 | 0xfe | 0x52a |
| N=37, anchor, `(unsigned short)` | 76.4 | 1444 | 0xfe | 0x52c |
| N=33, no anchor | 85.6 | 1432 | 0xf6 | 0x522 |
| N=36, reuse + `(unsigned short)` | 83.2 | 1432 | 0xfc | 0x522 |
| N=38, reuse + `(unsigned short)` | 69.7 | 1436 | 0x100 | 0x526 |
| N=32, `unsigned short` table (16-bit entries) | 85.7 | 1432 | 0xf6 | 0x522 |
| N=31, `unsigned short` table | 85.9 | 1432 | 0xf4 | 0x522 |

The latch offset dominates the fuzzy score: every 2 B the latch moves costs
about 15 points (36 `b <latch>` references plus the `return m` / `setWon`
tail).  N=32 (latch exact, first group -10) and N=37 (latch exact, first group
exact) are the only two configurations that keep the latch at 0x522; N=37 wins
because it also makes the 17-body first group offset-exact.

## 4. Remaining mismatches (74 slots, was 84)

| region | slots | note |
|---|---:|---|
| dispatch (`ldr`/`uxtb`/`ldrb` + `cmp #36` vs `#39`) | 5 | inherent to a data table |
| TBH table contents (37 vs 40 entries) | 36 | body order != value order |
| body 166 register allocation | 8 | `index` in r5 not r9; `amount` displaces `this` from r6 |
| last group (156..168) at +4 | 7 | literal-pool island in body 189 |
| literal-pool island (nop + table word + GOT word) | 3 | 10 B here vs the original's 6 B |
| `ldr r0,[pc,#…]` immediates / misc | 15 | pool distance |

The literal-pool island is the next target: `ARMConstantIslandPass` places the
table-address word and the `Globals::status` GOT word in one 8–10 B island
after body 189, while the original has only the GOT word (6 B) after body 168.
No source shape tried (hoisting the table pointer, `static const` local table,
16-bit table, `movw`/`movt`-forcing forms) moved it.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6210
cmake --build cmake-build-match --target check -j6    # RATCHET PASS
cmake --build cmake-build-match --target lint  -j6    # CLEAN
cmake --build cmake-build-match --target drift -j6    # 0 fields
```

Scratch (not committed): `/dev/shm/w6210/` — `fv.py` (single-function official
scorer, 0.1 s), `m.sh`, `gentbl.py` / `gen2.py` / `mkvar.py` (rank-table
generators), `bodyorder.py` / `offs.py` (TBH decoder), `fvd.py` (opcode diff),
`scanN.py` / `scanN2.py` / `sweep*.py` / `search.py` / `mut*.py` (the N,
anchor, table-width and body-form sweeps quoted above).
