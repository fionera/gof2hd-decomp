# `Status::missionCompleted` — wave 191b (w13742): residual sweep, no win

Worktree `wt/w13742`, branch `wave191b-w13742`, base master `4c4d81cf7`.
Target `_ZN6Status16missionCompletedEbbx` (`src/game/mission/Status.cpp`,
1,432 B, VMA `0xabf38`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
Scratch `/home/debian/GalaxyOnFire2/scratch/w13742`.

**Status: no measured win; `src/` unchanged.**  The landed `case 158`
body (`A_landed`, wave 191b / w13726) remains the local optimum at
**raw 90 / aligned 32 / size 1,432 / 527 insns**.  This pass tried the
remaining cheap spellings of the production-good block and confirmed the
wave-191b residual map.  No commit to `src/`; this note is the only
artifact.

## Baseline re-confirmed this session

```
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so --json _ZN6Status16missionCompletedEbbx
raw 90  aligned 32  size 1432/1432  insn 527/527
```

Section deltas (tools/secdiff.py equivalent, whole `.so`):

| section | size (orig/ours) | base | differing bytes |
|---|---|---|---|
| `.text` | 1259464 / 1259464 | 69440 | **72,954** |
| `.ARM.extab` | 156312 / 156312 | 1a3b78 | **564** |
| `.ARM.exidx` | 28528 / 28528 | 19cc08 | 0 |
| `.rodata` | 269159 / 269159 | 1c9e10 | 0 |
| `.data` / `.bss` | identical | identical | 0 |
| `.rel.plt` / `.rel.dyn` | identical | identical | 0 |
| `.dynsym` / `.dynstr` / `.hash` / `.got` | identical | identical | 0 |

All sizes and bases are identical; `.rel.plt` is byte-identical.

## Key finding: the standalone-object shape is not the in-TU shape

A faithful `index, amount, cargo` spelling with a re-queried
`getProductionGoodIndex()` for `hasEquipment` (`v_iac` / `C_iac`):

```cpp
int index  = m->getProductionGoodIndex();
int amount = m->getProductionGoodAmount();
Array<Item *> *cargo = ship->getCargo();
if (Item::isInList(index, amount, cargo)) return m;
if (Globals::status->ship->hasEquipment(m->getProductionGoodIndex(), 1)) return m;
```

compiled **standalone** (`probe.o`, same flags) reproduces the original's
exact prefix (`mov r9,r0` / `mov r5,r0` / `mov r2,r0`) and the exact
hasEquipment block.  In the **full TU** it measures
**raw 682 / aligned 145 / size 1,436 / 529 insns** — not size-exact.
The full-TU register allocation keeps `amount` in `r6` and spills `this`
(`mov r1,r6`), so the standalone object is not evidence for the in-TU
build.  The full-build measurement is authoritative.

## Variants measured this session (full `link_so`, `case 158` only)

| variant | raw | aligned | size | insns |
|---|---:|---:|---:|---:|
| **`A_landed` (HEAD)** | **90** | **32** | **1432** | 527 |
| `V1`: `ship->hasEquipment(...)` instead of `Globals::status->ship` | 85 | 127 | 1428 | 524 |
| `V2`: `else if (hasEquipment)` | 90 | 32 | 1432 | 527 |
| `V3`: `cargo` declared before `amount` | 94 | 35 | 1432 | 527 |
| `V4`: `cargo = Globals::status->ship->getCargo()` | 399 | 53 | 1432 | 527 |
| `V5`: `bool found = isInList(...)` then `if (found)` | 90 | 32 | 1432 | 527 |
| `W1`: `amount` local, `cargo` inline | 686 | 144 | 1436 | 529 |
| `W2`: `bool he = hasEquipment(...)` then `if (he)` | 90 | 32 | 1432 | 527 |
| `W3`: negated `isInList` + nested `if`/`else` | 90 | 32 | 1432 | 527 |
| `W4`: `if (!hasEquipment) break; return m;` | 90 | 32 | 1432 | 527 |

`V1` lowers raw by 5 and size by 4 B but aligned rises 32 -> 127 (the
`ship` vs `Globals::status->ship` GOT load changes the shared tail), so it
is not a win.  Every other size-exact shape ties `A_landed`.

## Residual map (unchanged)

| region | orig | ours | note |
|---|---|---|---|
| TBH table `abff2..ac035` | — | — | **24 B** (tier order; not source-expressible without `!prof`) |
| `ac1d0..ac1d5` | `and.w r0,r9,r6; ldr r6,[sp,#16]; mov r8,r4; lsls` | `and.w r0,r9,r6; mov r8,r4; lsls; ldr r6,[sp,#16]` | **6 B** scheduler reload placement; never moved under any variant |
| case-158 prefix `ac260..ac284` | `index->r9`, `amount->r5`, `cargo->r2` | `amount->r5`, `cargo->r6`, `index->r0` | 46 B |
| body tail | `b.n ac458` (2 B) | `cmp r0,#0; b.n ac1d4` (4 B) | |
| literal `ac4c4` | `0x00163ece` | `0x00163ed0` | 1 B, `Globals::status` GOT slot (other-TU layout) |

Byte-diff regions (current HEAD): `abff2..abff7` 6, `abffa` 1,
`abffe..ac000` 3, `ac002`/`ac004`/`ac006` 1 each, `ac00a` 1,
`ac00c..ac00e` 3, `ac010..ac012` 3, `ac02a..ac02b` 2, `ac034..ac035` 2,
`ac1d0..ac1d5` 6, `ac264..ac266` 3, `ac268..ac274` 13,
`ac276..ac278` 3, `ac27a` 1, `ac27c..ac28b` 16, `ac28e..ac29a` 13,
`ac29c..ac2a5` 10, `ac4c4` 1 — **total 90**.

## Why `A_landed` is the local optimum

The original's `mov r9,r0` / `mov r5,r0` / `mov r2,r0` requires the
allocator to process `amount` before `index`.  LLVM 7 `RAGreedy` allocates
local live ranges in linear instruction order (larger begin-to-end
distance first), so with `index` defined first it always wins `r5`
(`index->r5, amount->r6`, spilling `this`, +4 B = 1,436).  Any spelling
that hoists `index` therefore either grows the function or ties.  The
landed shape avoids the `index` local entirely, keeps `amount->r5` and
`cargo->r6`, and re-queries `getProductionGoodIndex()` for
`hasEquipment`, which reproduces the original's hasEquipment block
byte-for-byte.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13742
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so --json _ZN6Status16missionCompletedEbbx
```

`check` is unchanged from HEAD (byte_exact 4508, linked_exact 4508,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0,
parity 5370/5370); no `src/` edit and no baseline update.
