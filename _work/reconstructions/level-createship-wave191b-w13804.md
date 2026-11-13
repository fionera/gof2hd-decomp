# `Level::createShip` — wave 191b (w13804): `empPoints` clamp polarity + cargo compare order

Worktree `wt/w13804`, branch `wave191b-w13804`, base master `1ca813f1e`.
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
2,764 B, VMA `0xc0214`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`).  Scratch `scratch/w13804`.

**Status: landed the clamp polarity.**  Raw **1,190 -> 1,183** (-7), aligned
**366 -> 368** (+2), size 2,764/2,764 exact, insns 1,117/1,117.  `.text`
75,786 -> 75,779, `.ARM.extab` contentdiff **590** (unchanged, size 156,312),
`.ARM.exidx` 0, `.rel.plt` byte-identical.  `check`: RATCHET PASS, byte_exact
4508/4524, linked 4508, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0.

## 0. Baseline

```
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb   # raw=1190 aligned=366 size=2764/2764
# .text diff 75,786; .ARM.extab diff 590; .ARM.exidx diff 0
```

## 1. Landed change

The `empPoints` clamp is the second `getLevel()` clamp (the one feeding
`empPoints = 40 + e + e*4`), not the `level` clamp that feeds `hitpoints`.
Flipping its polarity:

```cpp
-    int e = Globals::status->getLevel() > 20 ? 20 : Globals::status->getLevel();
+    int e = Globals::status->getLevel() <= 20 ? Globals::status->getLevel() : 20;
```

is algebraically identical for every `int` and moves the clamp's branch/block
layout so that the `ldr r0,[r4] / blx getLevel` arm lands at `0xc0322/0xc0324`
(the original addresses) instead of `0xc0326/0xc0328`.  The `this` spill
(`str.w r9,[sp,#56]`) stays at `0xc031c` in both forms; the original keeps it
at `0xc0332` (see w13536 §3 — no source form moves it).

Note the original's *branch polarity* is the un-flipped `ble` form; the flip
is a raw-byte win from the resulting block layout, not a branch-opcode match.
`> 20 ? 20 : getLevel()` and the equivalent `if (e > 20) e = 20; else
e = getLevel();` both measure 1,190/366.

## 2. Measured table (all size 2,764/2,764, insns 1,117/1,117)

| variant | raw | aligned | `.text` | extab |
|---|---:|---:|---:|---:|
| baseline | 1,190 | 366 | 75,786 | 590 |
| **clamp `<= 20 ? getLevel() : 20` (landed)** | **1,183** | **368** | **75,779** | **590** |
| clamp `if (getLevel() <= 20) e = getLevel(); else e = 20;` | 1,183 | 368 | 75,779 | 590 |
| clamp `if (getLevel() > 20) e = 20; else e = getLevel();` | 1,190 | 366 | 75,786 | 590 |
| clamp inline `40 + (lv > 20 ? 20 : lv) * 5` | 1,190 | 366 | 75,786 | 590 |
| `empPoints = 40 + e*5` / `e*5 + 40` | 1,190 | 366 | 75,786 | 590 |
| single-call `e = getLevel(); e = e > 20 ? 20 : e;` | 2,174 | 445 | — | — (size 2,756, 1,114 insns) |

## 3. Cargo branch compare order (measured, not landed)

The original's fighter cargo branch at `0xc073c..0xc074a` tests **0x2c, 0x33,
0x31** in that order, with the two `delete fighter->cargo` bodies shared:

```
cmp  r4, #44 ; beq -> delete
ldr  r0, [sp, #52]
cmp  r0, #51 ; beq -> field_0x25 = 0
cmp  r0, #49 ; (mov r9,sl) ; bne -> tail
delete fighter->cargo; fighter->cargo = nullptr
```

Our source's `if (0x2c) ... else if (0x31) ... else if (0x33)` canonicalises to
`cmp 51 / cmp 49 / it ne / cmpne 44`, i.e. the unique `field_0x25` body first.
Writing the two delete cases together reproduces the original order:

```cpp
if (type == 0x2c || type == 0x31) {
    delete fighter->cargo;
    fighter->cargo = nullptr;
} else if (type == 0x33) {
    fighter->field_0x25 = 0;
}
```

Verified both in a minimal NDK r18b `-Oz` repro and in the real TU: the emitted
compare sequence becomes `cmp 44 / beq / cmp 51 / beq / cmp 49 / bne`, matching
the original.  The same order is produced by
`switch (type) { case 0x2c: case 0x31: ... break; case 0x33: ... }`.

**But it is a raw loss on the current baseline:** 1,183 -> **1,186** (+3),
aligned 368 unchanged, `.text` 75,779 -> 75,782, extab 590.  The +3 is a pad
`nop` before the literal pool (ours is 6 B shorter in the cargo block because
`type` stays in callee-saved `r6` and there is no `ldr r0,[sp,#52]` reload and
no `mov r9,sl` copy).  Those are register-allocation artifacts of the same
`this`-spill root cause, so the source-fidelity fix does not pay until the
spill is fixed.  Left unlanded.

## 4. Residual / why the spill is the root cause

Original tail keeps `fixed`/`fighter` in `r9` and `volumes` in `fp`; ours keeps
`fixed` in `fp` and `volumes` in `r9`.  The original reloads `this` from
`[sp,#56]` into `r6` at `0xc0b5e`; we reload it into `sl` at `0xc0b34`.  The
`str.w r9,[sp,#56]` spill sits at `0xc031c` in every source form we tried
(w13536's sweep agrees: ternary, if/else, `if (lv>20)`, `lv` local, `empTime`
first, `mp` local), whereas the original spills at `0xc0332` after
`add.w sl,r0,#40`.  This one 4-byte insertion shifts the body and rotates
`fp`/`r9` for the whole race-0 hull block.

## 5. Rejected / neutral

* cargo order alone (`||` / grouped switch): raw +3, aligned neutral (above).
* cargo `if (0x2c) ... else if (0x33) ... else if (0x31)`: byte-identical to
  the current `0x2c/0x31/0x33` order (clang re-canonicalises).
* `notop` (drop the top `EmpGuard`): raw 1,128 but `.ARM.extab` 156,308 (-4)
  and `.ARM.exidx` shifts by 1,307 (w13536) — section-level net loss.

## 6. Files changed

* `src/game/world/Level.cpp` — the `e` clamp polarity.
* `docs/IDENTICALITY.md`, `docs/VALIDATION.md` — residual table / `.text` delta.
* this note.
