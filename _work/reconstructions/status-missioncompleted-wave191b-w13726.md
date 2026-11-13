# `Status::missionCompleted` — wave 191b (w13726): faithful `case 158` body

Worktree `wt/w13726`, branch `wave191b-w13726`, base master `6fbc91363`.
Target `_ZN6Status16missionCompletedEbbx` (`src/game/mission/Status.cpp`,
1,432 B, VMA `0xabf38`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
Scratch `/home/debian/GalaxyOnFire2/scratch/w13726`.

**Status: landed a size-exact `case 158` body that is faithful to the
original's source shape.**  Raw 71 -> **90**, aligned 38 -> **32**, fuzzy
95.4 -> **96.4 %**, size **1,432/1,432** exact, 527/527 insns.  Whole-file
`.text` differing bytes 72,175 -> **72,194** (+19), `.ARM.extab` **564**
(unchanged), `.ARM.exidx` 0, `.rodata` 0.  `check` (ratchet) PASS:
byte_exact 4508, linked_exact 4508, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, parity 5370/5370.  `byte_exact` is
unchanged (raw != 0), so `tools/verify_baseline.json` is untouched.

## Landed body

```cpp
case 158:
    if (atStation && station->getIndex() == m->getTargetStation()) {
        int amount = m->getProductionGoodAmount();
        Array<Item *> *cargo = ship->getCargo();
        if (Item::isInList(m->getProductionGoodIndex(), amount, cargo)) {
            return m;
        }
        if (Globals::status->ship->hasEquipment(m->getProductionGoodIndex(), 1)) {
            return m;
        }
    }
    break;
```

This is the original's *semantic* shape: `getProductionGoodIndex` is
re-queried for `hasEquipment` (never cached), and the tail is the positive
`if (hasEquipment(...)) return m;`.  It reproduces the original's whole
`ac288..ac2a0` hasEquipment block **byte-for-byte** (GOT load, `ship` into
`r5`, re-query into `r1`, `hasEquipment`) and reuses the shared
`if (r0) return m` test block.  That is the source of the aligned 38 -> 32
and fuzzy +1.0 win.

## Residual map after this pass (function-internal)

| region | orig | ours | note |
|---|---|---|---|
| 40-entry TBH table | — | — | 24 B differ (tier order, see below) |
| body prefix `ac260..ac284` | `index->r9`, `amount->r5`, `cargo->r2` | `amount->r5`, `cargo->r6`, `index->r0` | call order amount,cargo,index |
| hasEquipment block | `ac288..ac2a0` | `ac286..ac29e` | **byte-identical** (2 B earlier) |
| body tail | `b.n ac458` | `cmp r0,#0; b.n ac1d4` | 4 B vs 2 B, shared test differs |
| literal `ac4c4` | `0x00163ece` | `0x00163ed0` | GOT slot `Globals::status` (link layout) |

The body is still 70 B, same as the original: ours saves the original's
`mov r9,r0` (-2 B) and pays a `cmp r0,#0` (+2 B) before the shared test.

## Why the prefix cannot be matched from source (this build)

The original's `mov r9,r0` / `mov r5,r0` / `mov r2,r0` means the allocator
processed `amount` before `index`.  LLVM 7 `RAGreedy::enqueue` allocates
*local* live ranges in linear instruction order (larger begin-to-end
distance first), so with `index` defined first it always wins `r5`
(`index->r5, amount->r6`, spilling `this` to `r1`, +4 B = 1,436).  The
original's `index->r9` requires `amount` to be allocated first (or `index`
to be a non-local range), which in turn requires the original's block
order.  Measured size-exact alternatives (all 1,432 B unless noted):

| body shape | raw | aligned |
|---|---:|---:|
| **landed** (`amount`,`cargo` locals; index inline; positive re-query) | **90** | **32** |
| `cargo`,`amount` locals; index inline; positive re-query | 94 | 35 |
| `Status *st = Globals::status;` hoisted before the calls | 79 | 38 |
| `index`,`cargo` locals; positive re-query | 384 | 43 |
| `index` local only; positive re-query | 682 | 145 (1,436 B) |
| `index`,`amount` locals; cargo inline; positive re-query | 682 | 145 (1,436 B) |
| all-inline args; positive re-query | 682 | 145 (1,436 B) |
| committed baseline (`index`,`cargo`,`amount`; `!hasEquipment(index,1)`) | 71 | 38 |

`index` local + `amount` local + re-query is the faithful spelling but is
**1,436 B**: the allocator keeps `amount` in `r6` and spills `this`
(`mov r1,r6`).  No spelling in ~150 variants produced `mov r9,r0`; the
allocation is a function of the surrounding block order, not the body text.

## TBH rank table

Decoded both 40-entry tables (`tbh` at `0xabfe2`, table at `0xabfe6`).
The original body order is tiered:
`167/174, 150,151,152,153,154,155, 158,162,166,170,184,189,
156,157,160,163,164,165,168,169,171,172`.
Ours is ascending (`...155,156,157,158,160,162,...`), so 16 slots differ
(24 B).  Physically reordering the `case` bodies in the source to the
tiered order does **not** change the table (MBP re-sorts jump-table
successors by value; measured raw 126/aligned 51, size 1,432).  The tier
split needs branch-probability weights; w13703's `__builtin_expect`
one-hot (2000:1) variants reach the tier order but lose size
(1,368-1,424 B).  A flat `!prof` injection (w13640) is the only known way
to hit the exact table (raw 47) and is not source-expressible.

## Literal pool

`ac4c4` holds the PC-relative GOT delta for `Globals::status`:
orig `0x00163ece`, baseline `0x00163eca`, landed body `0x00163ed0`.
The GOT slot is `0x20f15c` (orig) vs `0x20f158` (ours) - a 4-byte GOT
layout delta from another TU, not addressable from `Status.cpp`.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13726
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN6Status16missionCompletedEbbx     # raw=90 aligned=32 size=1432/1432
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)
# RATCHET PASS
```

Scratch: `cands4.txt`/`cands8.txt`-`cands14.txt` (body sweeps),
`cand90.body`, `apply.py`, `build1.sh`, `sweep.py`, `sweep_r9.py`,
`tbh.py` (TBH decoder), `secs.py` (section diff), `score1.py` (fuzzy
score), `Status.base.cpp`, `Status.reordered.cpp`, `orig.dis`,
`ours90.dis`.
