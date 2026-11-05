# `Status::missionCompleted` — wave 111: rank-table variants are a dead end
# (87.0 % / 1432 B is the measured maximum) (w5910)

Status: **research note only** (no source change). Worktree `wt/w5910`, branch
`wave111-w5910`, base master `8eea060cc`. Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 r328903 / LLVM 1d739ffb, `-Oz`).
Target: `_ZN6Status16missionCompletedEbbx`, original `0xabf38`, size **1432
(0x598)**.

## 0. Result

The wave-110 rank table (`tools/mission_case_table.cpp`, fixed address
`0x20b977`, `switch (MISSION_CASE[(unsigned char)m->getType()])`) is still the
highest-scoring shape measured: **87.0 % / 1432 B**.  Every variant tried this
pass is either the same 87.0 % or lower.  The residual 13 % is *not* body
order (the rank table already reproduces the original body order exactly); it
is the **dispatch cost** (three extra instructions for the table lookup) and
the resulting **body-offset shift** (group A at `-10`, group B at `+6`), which
makes every intra-function branch target and every 32-entry TBH halfword
differ from the original.

The gate is unchanged and green: `RATCHET PASS`, `byte_exact 4500`,
`linked_exact 4500`, imports 207, `lint CLEAN`, `drift 0`, 27 sections, exact
file size 2,187,328 B, `.ARM.extab` still `0x26294` (-4).

## 1. Exact body-offset comparison (why 87.0 is the ceiling for this shape)

Decoded TBH targets, function-relative (`orig` = original, `head` = current
build).  The rank labels map to the original types in brackets.

| rank (type) | orig | head | delta |
|---|---:|---:|---:|
| 150 (167/174) | 0x0fe | 0x0f4 | -10 |
| 151 (150) | 0x1d8 | 0x1ce | -10 |
| 152 (151) | 0x1de | 0x1d4 | -10 |
| 153 (152) | 0x1e4 | 0x1da | -10 |
| 154 (153) | 0x218 | 0x20e | -10 |
| 155 (154) | 0x21e | 0x214 | -10 |
| 156 (155) | 0x236 | 0x22c | -10 |
| 157 (158) | 0x248 | 0x23e | -10 |
| 158 (162) | 0x2a4 | 0x29a | -10 |
| 159 (166) | 0x30a | 0x300 | -10 |
| 160 (170) | 0x36e | 0x36a | -4 |
| 161 (184) | 0x37c | 0x378 | -4 |
| 162 (189) | 0x3b6 | 0x3b2 | -4 |
| 163 (156) | 0x3f2 | 0x3f8 | +6 |
| 164 (157) | 0x41c | 0x422 | +6 |
| 165 (160) | 0x450 | 0x456 | +6 |
| 166 (163) | 0x47e | 0x484 | +6 |
| 167 (164) | 0x49a | 0x4a0 | +6 |
| 168 (165) | 0x49e | 0x4a4 | +6 |
| 169 (168) | 0x4a6 | 0x4ac | +6 |
| 170 (169) | 0x4c4 | 0x4c4 | 0 |
| 171 (171) | 0x4c8 | 0x4c8 | 0 |
| 181 (172) | 0x4e2 | 0x4e2 | 0 |
| latch | 0x522 | 0x522 | 0 |

The shift is produced by two real code differences:

* **dispatch**: original `sub.w r1,r0,#150; cmp r1,#39; bhi; tbh` (8 B);
  ours `ldr r1,[pc,#844]; uxtb r0,r0; ldrb r0,[r0,r1]; sub.w r1,r0,#150;
  cmp r1,#31; bhi; tbh` (14 B) plus a 32-entry TBH (64 B) instead of a
  40-entry TBH (80 B): net **-10 B** before the first body.
* **literal pool**: the extra table-address pool word (`0x20b977`) plus its
  `nop`/alignment is emitted at `0xac328`, i.e. between body 189 and body
  156; that is the `+16` recovery seen from rank 163 onward.  The original's
  pool words are at `0xac3f8` and `0xac4c4..0xac4cc`; ours are at
  `0xac328`, `0xac32c`, `0xac4c4..0xac4cc`.

Because the latch/return tail is at exactly the original offsets, all
branches to the tail match; the mismatches are the dispatch (3 insns), the
TBH halfwords (32 vs 40 entries, different relative offsets) and the
intra-body branch targets.

## 2. Measured variants (full link + official verifier)

All rows are `Status.cpp`/`tools/mission_case_table.cpp` edits rebuilt with
the canonical `cmake-build-match` build dir and scored with
`tools/verify/verify.py`.  The baseline row is the current HEAD.

| variant | pct | usize | notes |
|---|---:|---:|---|
| **HEAD rank table (32 entries, `(unsigned char)`)** | **87.0** | **1432** | baseline |
| table index cast `(short)` / `(char)` / `(unsigned short)` / `&0xff` / `&0xff`+u8 / `+0` / `^0` | 87.0 | 1432 | identical code |
| `(unsigned)` / `(int)` / `(long)` cast | 71.6 | 1428 | no `uxtb`; tail shifts -4 |
| `unsigned short[192]` table | 85.7 | 1432 | `ldrh` lookup |
| `int[192]` / `unsigned int[192]` table | 85.7 | 1432 | `ldr` lookup |
| table as file-static in `Status.cpp` | 85.6 | 1432 | pool address differs |
| source case order = original body order | 87.0 | 1432 | identical code |
| source case order reversed | 44.1 | 1432 | layout follows rank order |
| source case order group B first | 67.4 | 1432 | |
| `case 0`/`case 11` split into two bodies | 87.0 | 1432 | identical code |
| explicit `case 172..180: break;` | 87.0 | 1432 | identical code |
| `int mtype = m->getType(); switch (MISSION_CASE[(unsigned char)mtype])` | 87.0 | 1432 | identical code |
| `unsigned char rank = MISSION_CASE[...]; switch (rank)` | 87.0 | 1432 | identical code |
| hoist `const unsigned char *caseTable = MISSION_CASE;` | 87.0 | 1432 | compiler re-materialises the load |
| hoist + `if (caseTable == 0) return 0;` | 87.0 | 1432 | optimised away |
| anchor `if (s == 0) break;` / `return m;` instead of `continue;` | 87.0 | 1432 | identical code |
| anchor `if (index == amount) continue;` | 87.0 | 1432 | identical code |
| anchor `if (amount < 0) continue;` | 87.0 | 1432 | identical code |
| anchor `if (index < 0) continue;` | 86.2 | 1432 | |
| anchor moved after the switch | 70.6 | 1428 | optimised away; allocation changes |
| anchor moved to case 181 | 69.0 | 1432 | |
| anchor `Globals::status->ship != 0 && ...` | 71.1 | 1436 | |
| anchor `if (ship == 0) continue;` (no `Ship *s`) | 87.0 | 1432 | identical code |
| no anchor, `s->getCargo()` -> `ship->getCargo()` | 70.6 | 1428 | type-166 allocation changes |
| no anchor + `m->getStatusValue();` after the switch | 69.8 | 1436 | |
| no anchor + `m->getType();` after the switch | 69.8 | 1436 | |
| no anchor + `m->hasWon();` after the switch | 69.8 | 1436 | |
| no anchor + `__sync_synchronize()` after the switch | 77.1 | 1432 | `dmb` mismatch |
| no anchor + `__atomic_thread_fence` after the switch | 77.1 | 1432 | `dmb` mismatch |
| no anchor + `__atomic_signal_fence` after the switch | 71.6 | 1428 | no instruction |
| swap `index`/`amount` computation order | 69.6 | 1428 | |
| `Array<Item*> *cargo = ship->getCargo();` | 70.6 | 1428 | |
| `int index`/`amount` computed before the `if` | 70.4 | 1428 | |
| rank-table spread N=33 | 71.9 | 1436 | |
| rank-table spread N=35 | 72.7 | 1440 | |
| rank-table spread N=37 | 76.4 | 1444 | |
| rank-table spread N=38 | 72.0 | 1448 | |
| rank-table spread N=39 | 71.5 | 1448 | |
| rank-table spread N=40 | 73.6 | 1448 | |
| N=40 + no anchor | 72.2 | 1448 | |
| N=40 + no anchor + `(unsigned)` | 72.7 | 1444 | |
| direct `switch (m->getType())`, original source order | 60.4 | 1428 | ascending layout |
| `after189` two-predecessor shape (AF) | 76.4 | 1472 | exact order, +40 B second dispatch |
| `after189` without case-166 anchor (AF2) | 75.3 | 1468 | |
| two sequential switches on a cached `int` | 46.8 | 1548 | exact order, wrong regalloc |
| two sequential switches (seq2) | 43.3 | 1448 | |
| nested `default:` switch | 60.4 | 1428 | SimplifyCFG merges |

## 3. Fallthrough sweep (direct switch, table-free)

Removing the top-level `break;` from a body makes it fall through to the next
source body, which lets MBP follow the source order.  The exact original body
order is produced by any subset that contains `{158,162,166,170,184,189}`
(the chain that forces 158..189 before 156), with or without any of
`{150,151,152,153,154,155}`.  All such subsets are **1368-1392 B**, i.e.
40-64 B short of 1432:

```
subset (removed breaks)                         size
{155,158,162,166,170,184,189}                   1368
{152,155,158,162,166,170,184,189}               1368
{154,155,158,162,166,170,184,189}               1372
{150,155,158,162,166,170,184,189}               1380
{150,151,152,153,154,155,158,162,166,170,184,189} 1388
```

The exact-order fallthrough shape therefore cannot be size-exact without
adding 40-64 B of extra real code; every "anchor" tried (null checks, extra
calls, fences) either changes the register allocation or the tail offset and
scores below 87.0.

## 4. Conclusion / open question

The original's single-switch body order is an MBP tie-break artifact:
`buildJumpTable` adds successors in value-sorted order, so a plain
`switch (m->getType())` lays the bodies out ascending.  The rank table makes
the value order equal the original source order, which is why it scores 87.0.
The remaining 13 % needs the dispatch to be the original's 4-instruction
`sub/cmp/bhi/tbh` **and** the body offsets to match.  No table-free shape
found (waves 92-111) is both exact-order and 1432 B; the closest exact-order
table-free shape is 1368-1392 B (fallthrough) or 1472 B (AF second
dispatch).  A future pass would need a source shape that gives the group-B
bodies a second predecessor for free (no second dispatch) while keeping the
original register allocation.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5910
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
cmake --build cmake-build-match --target lint  -j6   # CLEAN
cmake --build cmake-build-match --target drift -j6   # 0 fields
```

Scratch (not committed): `/dev/shm/w5910/` — `Status.head.cpp` (HEAD),
`mct.head.cpp` (HEAD table), `cc.sh`, `order.py`, `opcodes.py`, `score.sh`,
`search.py`, `sweep.py`, `spreadN.py`, and the variant files named in §2.
