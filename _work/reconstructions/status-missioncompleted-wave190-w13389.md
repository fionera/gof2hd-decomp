# `Status::missionCompleted` — wave 190 (w13389): case-10 cargo load hoist

Worktree `wt/w13389`, branch `wave190-w13389`, base master `5ff423480`.
Target `_ZN6Status16missionCompletedEbbx` (`src/game/mission/Status.cpp`,
1,432 B, VMA `0xabf38`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13389`.

**Status: landed.**  Raw **475 -> 468** (-7), aligned 187 -> 193 (the
`fn_diff` literal-pool/instruction-alignment artifact; the code stream
is 7 bytes closer).  Size 1,432/1,432 exact.  `.text` differing bytes
**73,273 -> 73,266** (-7); `.ARM.extab` **606** (unchanged, size/base
exact at `0x1a3b78`, 156,312 B); build-id 20.  Total
(text+extab+build-id) **73,899 -> 73,892** (-7).  `.rodata` unchanged
(size 269,351, +0xc0; original 269,159 B content still exact),
`.fini_array` 8 B @ `0x20cba0`, `.dynamic`/`.rel.dyn`/`.rel.plt`/
`.dynsym`/`.dynstr`/`.hash`/`.gnu.version`/`.data.rel.ro`/`.got`/
`.init_array` unchanged, 27/27 section headers, file size 2,187,328
exact.  Gate: byte_exact 4507/4524, linked_exact 4507/4524, size
4524/4524, data 843/843, imports 207, stub/missing/extra 0, lint CLEAN,
drift 0, parity 5370/5370 (`RATCHET PASS`).

## 1. What changed

The case-10 (`type 166`) body now hoists `ship->getCargo()` into a local
before the `getProductionGoodAmount()` call:

```cpp
int index = m->getProductionGoodIndex();
Array<Item *> *cargo = ship->getCargo();
int amount = m->getProductionGoodAmount();
if (Item::isInList(index, amount, cargo)) {
    return m;
}
if (Globals::status->ship->hasEquipment(index, 1)) {
    return m;
}
```

This is the same values and same call count as the previous
`Item::isInList(index, amount, ship->getCargo())` form; only the
evaluation order of the third argument changes.  The original's own
order is index -> amount -> cargo, so this is not a semantic
reconstruction, it is a register-allocation lever: clang 7 at `-Oz`
now keeps `cargo` in `r6` and the cached `index` in `r5`, and the
case-10 body shrinks from 64 B to 62 B.  The 2 B shift moves the
literal pool / tail boundary, so the raw byte diff drops by 7 while
the function stays exactly 1,432 B.

## 2. Why this is a win

The residual is dominated by the +2-byte shift of the tail after the
case-10 body: the table-address literal pool adds 8 B at the end of
the `type 189` body (see `status-missioncompleted-wave189-w13358.md`),
and the cached-index case-10 body is 4 B shorter than the original.
The hoist shortens the body by another 2 B and changes the block
boundary where the pool is placed; the net effect is 7 fewer differing
`.text` bytes at the same size.  The aligned metric moves 187 -> 193
because `fn_diff` aligns the shifted instruction stream differently;
the raw byte count is the primary residual metric and the code stream
is unchanged in size.

## 3. Measured forms (all size 1,432 unless noted)

| form | raw | aligned | size |
|---|---:|---:|---:|
| base (index, amount, `ship->getCargo()` inline) | 475 | 187 | 1432 |
| **landed: index, cargo local, amount** | **468** | **193** | **1432** |
| amount, cargo local, index | 469 | 188 | 1432 |
| cargo local, index, amount | 474 | 194 | 1432 |
| cargo local, amount, index | 474 | 189 | 1432 |
| amount, index, cargo local | 483 | 182 | 1432 |
| index, cargo local, amount, re-query `hasEquipment` | 665 | 196 | 1440 |
| index, amount, cargo local, re-query `hasEquipment` | 695 | 176 | 1440 |

The re-query forms (`hasEquipment(m->getProductionGoodIndex(), 1)`) are
8 B larger and not size-exact; the cached-index forms are the only
size-exact family.

## 4. Structural checks

* `.text` 0x1337c8 exact, 73,266 differing bytes (was 73,273).
* `.ARM.extab` size/base exact, 606 differing bytes (unchanged);
  `.ARM.exidx` 0.
* `.rodata` sh_size 269,351 (+0xc0), original 269,159 B content exact;
  `.fini_array` 8 B @ `0x20cba0`; `.dynamic`, `.rel.dyn`, `.rel.plt`,
  `.dynsym`, `.dynstr`, `.hash`, `.gnu.version`, `.data.rel.ro`,
  `.init_array`, `.got` byte-identical; 27/27 headers; file size
  2,187,328 exact.
* `check` RATCHET PASS, `lint` CLEAN, `drift` 0 fields.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13389
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN6Status16missionCompletedEbbx     # raw=468 aligned=193 size=1432/1432
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
cmake --build cmake-build-match --target lint  -j2   # CLEAN
cmake --build cmake-build-match --target drift -j2   # 0 fields
```

## 6. Files changed

* `src/game/mission/Status.cpp` — case-10 cargo-load hoist.
* `docs/IDENTICALITY.md` — wave-190 residual table and section totals.
* this note.

## Current-master harvest (wave 190)

Re-applied the case-10 cargo-load hoist on current master (`26da752c0`):
`Array<Item *> *cargo = ship->getCargo();` is loaded once before
`getProductionGoodAmount()`/`isInList`, matching the original's register
lifetime. Status raw **475 -> 468** (-7), aligned 187 -> 193, size
1,432/1,432. `.text` contributes -7 to the combined 73,248 -> 73,233
harvest. `check` green, RATCHET PASS.
