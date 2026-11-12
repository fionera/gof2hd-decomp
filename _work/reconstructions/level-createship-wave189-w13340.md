# `Level::createShip` — wave 189 / w13340: EXTAB-RESTORE (note only)

Target `_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
original 2,764 B @ `0xc0214`). Branch `wave189-w13340`, base master `86875c756`.
Task: keep the mission-guard-removal codegen (raw **1,166**) and restore the
original 336 B / 57-row / table-315 LSDA.

**Status: no landable win; `src/` is left at the base.** Base is raw **1,202**,
aligned **484**, size **2,764/2,764**, insns 1,116/1,117. `.text` diff 73,983;
`.ARM.extab` **156,312 B @ 0x1a3b78, diff 615**; `.ARM.exidx` diff 0;
`.rel.plt` 0; file size exact. `check` green (byte 4507, linked 4507,
size 4524, data 843, imports 207, lint CLEAN, drift 0).

## 0. The LSDA budget (exact)

Entry = `19 (fixed header) + ULEB(table_len) + table`, 4-byte aligned.
So 332 B <=> table 310, and **336 B requires table 312..315**.
Row bytes: LP row = ULEB(start)+ULEB(len)+ULEB(lp)+1 (6 B when start >= 128),
gap row = ULEB(start)+ULEB(len)+2 (5 B when start >= 128 and len < 128).

Original: 28 LP rows (168) + 29 gap rows (27*5 + 2*6 = 147) = **315**.
Mission-guard-removal (raw 1,166): 28 LP + 28 gaps = **310**.

The original's extra gap is `[562,570)` (8 B) between the `PlayerFixedObject`
ctor try-range `[530,562)` and the `Array` ctor try-range `[570,574)`; it holds
the `Array` `operator new` call. Our reconstruction has no `PlayerFixedObject`
ctor try-range (placement `new`), so that gap is inside the 102-byte
`[466,568)` gap.

## 1. The raw-1,166 candidate (mission guard removed) — rejected

Dropping the entry `EmpGuard` around `getCurrentCampaignMission()`:

```
                                   base        candidate
raw                                1,202       1,166   (-36)
aligned                            484         460
size                               2,764       2,764   exact
LSDA                               336 B/57    332 B/56
table                              315         310
.text diff                         73,983      73,947  (-36)
.ARM.extab size                    156,312     156,308 (-4)
.ARM.extab content diff (naive)    615         122,305 (shift)
.ARM.exidx diff                    0           1,307
.rel.plt / .rodata / .data         exact       exact
```

The 4-byte shrink shifts every later `.ARM.extab` entry, so the naive
`.ARM.extab` content diff grows 615 -> 122,305 and `.ARM.exidx` loses
byte-identity (0 -> 1,307). The ratchet still passes, but the doc-visible
total differing bytes grows 74,618 -> ~196,272 for a 36-byte raw win — a net
loss by the "extab content growth <= raw win" rule. **Not landed.**

Key measured fact: the base body and the 1,166 candidate body are
**instruction-identical** (all 903 body instructions, 0x0..0x97c); the 36 raw
bytes are entirely in the landing-pad/literal-pool tail. The mission guard
only adds a cleanup stub.

## 2. Restoring table 315 while keeping the 1,166 body — attempts

All measured with a full `link_so` + `fn_diff` + LSDA row dump.

### (a) Move a guard onto the `PlayerFixedObject` ctor

`v_missionPFixed` = mission guard kept + `EmpGuard` around the
`PlayerFixedObject` placement-`new` ctor + the fighter `setShipGroup` guard
removed:

```
raw 1,212   aligned 468   size 2,764 exact
LSDA 336 B / 57 rows / table 315   (.ARM.extab size exact, diff 632)
.ARM.exidx diff 0   .rel.plt 0   .text diff 73,993
rows: 0:28/4 2:406/30 3:456/10 5:532/28 7:568/4 9:654/30 ...
```

This is the only variant found with **table 315, exact `.ARM.extab` size and
`.ARM.exidx` diff 0**, but raw is **1,212** (10 worse than the base) and the
extab content diff is 632 (17 worse than the base's 615). Not a win.

### (b) Remove the `setEmpData` guard and guard the `PlayerFixedObject` ctor

`v_moveGuardPFixed` / `v_origGuards` (guards on both placement-`new` ctors,
no `setEmpData`/`setShipGroup` guards):

```
raw 2,182..2,184   size 2,780 (+16, gate-fatal)
LSDA 336 B / 57 rows / table 315
```

Removing the `setEmpData` invoke changes the register allocation
(`fp`<->`sl`, stack slots shift by 4) and grows the body by 16 B. Table is
right, size is not.

### (c) Keep the `setEmpData` guard, add the `PlayerFixedObject` guard

`v_guardPFixedCtor`: raw 1,211, size 2,764, but table **321** (340 B) —
29 LP rows (the extra `setEmpData` row is not compensated).

### (d) Move the fighter `setShipGroup` guard instead of removing it

| target | raw | size | table |
|---|---:|---:|---:|
| `getShipGroup` (fighter) | 1,167 | 2,764 | 310 |
| `addObject` (fighter) | 1,171 | 2,764 | **311** |
| `cargo delete` | 1,425 | 2,760 | 299 |
| `setBV` | 1,174 | 2,764 | 310 |
| `setLevel` | 1,167 | 2,764 | 310 |
| `fixed->setShipGroup` | 2,072 | 2,752 | 310 |

Moving the guard after `addObject` makes the `[1180,1316)` gap 136 B
(crosses the 128 ULEB threshold), giving table **311** — still 332 B, one
byte short of 312. Raw 1,171 / 1,187 (nested-guard spelling).

### (e) Guard matrix sweep

`E`=setEmpData guard, `P`=PlayerFixedObject ctor guard, `F`=PlayerFighter ctor
guard, `S`=fighter setShipGroup guard (`orig`/`none`/`addobj`):

```
E1P0F0Sorig   raw 1166  size 2764  table 310   (the 1,166 candidate)
E1P1F0Sorig   raw 1211  size 2764  table 321
E1P1F0Snone   raw 1176  size 2764  table 310
E1P0F1Snone   raw 1580  size 2764  table 310
E0P1F0Sorig   raw 2182  size 2780  table 315
E0P1F1Snone   raw 2184  size 2780  table 315
E0*           size 2780..2784 (setEmpData invoke removal)
```

Every size-2,764 variant is table 310 or 321; every table-315 variant is
size 2,780. There is no size-2,764 / table-312..315 / raw < 1,202 point.

## 3. Why the 5 bytes are hard

The candidate's 28 LP rows are `Player`, `setEmpData`, `Array`, 24
`BoundingAAB`, `setShipGroup`; the original's 28 are `Player`,
`PlayerFixedObject`, `Array`, 24 `BoundingAAB`, `PlayerFighter` (both are
regular `new` in the original). Matching the original's gap structure needs
the `PlayerFixedObject` ctor try-range, i.e. one more LP row (+6) plus a gap
split (+5); the `setEmpData` row must then go (-11). But removing the
`setEmpData` invoke is what changes the body codegen (register allocation),
and adding the ctor guard without removing it leaves 29 LP rows (table 321).

The only 5-byte LP row is one with start < 128 (the mission guard,
`[28,32)`); every guard added at a later call splits the preceding gap and
costs +9..+11. A guard around a non-call region (`float z = ...`) is a no-op
(measured: identical raw/table), because no try-range is emitted.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13340
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb
# base: raw=1202 aligned=484 size=2764/2764 insn=1116/1117
```

Scratch (`/home/debian/GalaxyOnFire2/scratch/w13340/`): `measure.py`,
`showrows.py`, `combo.py`, `gsdc.cpp` (1,166 body), `v_missionPFixed.cpp`
(table-315 raw-1,212 body), `v_moveGuardPFixed.cpp`, `v_origGuards.cpp`,
`v_sg_addObject.cpp`, `v_nestedGuard.cpp`, `orig_rows.txt`.
