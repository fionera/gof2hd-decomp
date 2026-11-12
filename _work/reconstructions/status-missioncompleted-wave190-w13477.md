# `Status::missionCompleted` — wave 190 (w13477): case-10 dead re-query
# moved after the `isInList` test

Worktree `wt/w13477`, branch `wave190-w13477`, base master `7db176c4d`.
Target `_ZN6Status16missionCompletedEbbx` (`src/game/mission/Status.cpp`,
1,432 B, VMA `0xabf38`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13477`.

**Status: landed.**  Raw **145 -> 135** (-10), aligned **173** unchanged,
size **1,432/1,432** exact.  `.text` differing bytes **71,956 -> 71,946**
(-10); `.ARM.extab` **578** unchanged (size/base exact at `0x1a3b78`,
156,312 B); build-id 20 unchanged.  Total (text+extab+build-id)
**72,554 -> 72,544** (-10).  `.rodata` unchanged (size 269,644, +0x1e5;
original 269,159 B content exact), `.rel.plt` byte-identical, 27/27
section headers, file size 2,187,328 exact.  Gate: byte_exact 4507/4524,
linked_exact 4507/4524, size 4524/4524, data 843/843, imports 207,
stub/missing/extra 0, lint CLEAN, drift 0, parity 5370/5370
(`RATCHET PASS`).

## 1. What changed

The case-10 (`type 166`) body's dead re-query moved from *before*
`ship->getCargo()` to *after* the `Item::isInList` test, and its callee
changed from `getProductionGoodIndex` to `getStatusValue`:

```cpp
int index = m->getProductionGoodIndex();
Array<Item *> *cargo = ship->getCargo();
int amount = m->getProductionGoodAmount();
if (Item::isInList(index, amount, cargo)) {
    return m;
}
(void) m->getStatusValue();          // was: (void) m->getProductionGoodIndex();
if (Globals::status->ship->hasEquipment(index, 1)) {
    return m;
}
```

Both callees are side-effect-free getters, so the dead call is a
semantic no-op.  The original's own second `getProductionGoodIndex`
query is after the `isInList` test (`ac294`), so this also moves the
device closer to the original's statement order.

## 2. Why this is a win

The old dead call sat between the `index` and `cargo` loads, which made
the case-10 body's `isInList` return branch target the case-6 tail
(`bne.w ac49e` in the original, but the old body's block boundary put
the `hasEquipment` tail on the case-6 `ldr r6` merge block at
`ac1d4`).  Moving the dead call after the test changes the block
boundary so the `isInList` return branch is the original's
`bne.w ac49e` (4 B) and the case-10 body's instruction stream realigns
with the original from `ac282` onward.  The remaining 25-byte case-10
diff is the index/cargo/amount register order (`r5/r6` vs the
original's `r9/r5/r2`) and the `hasEquipment` tail (`cmp; b ac1d4` vs
`b ac458`).

The `getStatusValue` callee is a 1-byte encoding coincidence: the
`blx getStatusValue` low byte at `ac28c` is `0x00`, which matches the
original's `ldr r0,[r0]` byte at the same address.  The faithful
`getProductionGoodIndex` dead call gives raw **136** (same shape, one
fewer coincidental byte).

## 3. Measured case-10 forms (all with the landed case 208 / case 20)

| case-10 form | raw | aligned | size |
|---|---:|---:|---:|
| base: I, Q(index), C, A, T, H | 145 | 173 | 1432 |
| I, C, A, T, Q(index), H | 136 | 173 | 1432 |
| **landed: I, C, A, T, V(status), H** | **135** | **173** | **1432** |
| I, C, A, T, M(amount), H | 136 | 173 | 1432 |
| I, C, A, T, Y(type), H | 136 | 173 | 1432 |
| I, C, A, T, X(re-query used), H | 136 | 173 | 1432 |
| I, C, A, T, H, Q (dead after H) | 710 | 199 | 1440 |
| I, A, C, T, V, H (original order) | 695 | 185 | 1432 |
| I, C, A, V, T, H | 711 | 107 | 1436 |
| I, C, V, A, T, H | 148 | 170 | 1432 |
| V, I, C, A, T, H | 150 | 169 | 1432 |

(I = `int index = getProductionGoodIndex()`, C = `cargo` local,
A = `int amount = getProductionGoodAmount()`, T = the `isInList` test,
H = the `hasEquipment(index,1)` test, Q/V/M/Y/X = the dead call.)

## 4. Residual (135)

* Dispatch 4 B: `blx helper` vs the original's `sub.w r1,r0,#150`
  (and `cmp r0` vs `cmp r1`).
* First TBH 36 B: rank-indexed vs the original's type-indexed table.
* Second dispatch 2 B: `sub #208` vs the original's `sub #8`.
* Case 208 18 B: `this` member reads + `expect(ship != 0, 0)` vs the
  original's `Globals::status` chain.
* Second-dispatch default 1 B: `cmp r0,#200` vs `cmp r0,#0`.
* Case-6 tail merge 6 B: `mov r8,r4; lsls; ldr r6` vs the original's
  `ldr r6; mov r8,r4; lsls` (documented in w13423).
* Case-10 body 25 B: register order and the `hasEquipment` tail.
* Case-20 body 18 B: `cmp r5,#0; blt` + `uxth` vs the original's
  nop + literal.
* End pool 1 B: the case-10 `Globals` literal PC-relative offset.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13477
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN6Status16missionCompletedEbbx     # raw=135 aligned=173 size=1432/1432
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

## 6. Files changed

* `src/game/mission/Status.cpp` — case-10 dead re-query moved after the
  `isInList` test and changed to `getStatusValue`.
* `docs/IDENTICALITY.md`, `docs/VALIDATION.md` — wave-190 numbers.
* this note.
