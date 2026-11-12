# `Level::createShip` — wave 190 (w13395): LSDA budget + callee-saved rotation

Worktree `wt/w13395`, branch `wave190-w13395`, base master `6e67e3904`.
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
2,764 B, VMA `0xc0214`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13395`.

**Status: no landable win; `src/` left at the base.**  Base is raw **1,192**,
aligned **396**, size **2,764/2,764**, insns **1,117/1,117**, `.text` diff
**73,025**, `.ARM.extab` **156,312 B @ 0x1a3b78, diff 603**, `.ARM.exidx` 0,
`.rel.plt` byte-identical, file size exact.  `check` green.

## 0. The hard constraint nobody can dodge: `.ARM.extab` size

The base LSDA record is 336 B / **57 rows / 315-byte table** (header 19 +
ULEB(len) + table, 4-byte aligned).  `.ARM.extab` is **156,312 B** and
`.rodata` follows at `0x1c9e10` (16-byte aligned).

If the record shrinks or grows, `.ARM.extab` changes size, `.rodata` moves to
`0x1c9e20` (+0x10) and **every PC-relative `.rodata` literal in the whole
`.text` changes**.  Measured: landing a 156,316 B extab (e.g. the mission-guard
removal compensated by a +8 guard) raises the whole-`.text` diff from
**73,025 to 77,796** (+4,771) and `.ARM.exidx` from 0 to 1,304.  So the only
landable change is one that keeps `.ARM.extab` **exactly 156,312 B**.

Table budget: 332 B <=> table 310 (56 rows); **336 B requires table 312..315**.
A call-guard row is 5 B (start >= 128) and a gap row is 5 B; adding a guard
around a call inside an existing gap therefore adds **2 rows (+10 B)** and
lands at 156,316 (payload 320), never 156,312.  Only a guard whose LP abuts an
existing LP (no new gap row) would add exactly one row (+5 B -> 315).

## 1. LSDA rows, base vs original

```
orig lsda=1ab0ac len=315 rows=57      ours len=315 rows=57
  0 (0, 406, 0, 0)                     (28, 4, 2496, 0)   <<<  mission guard LP
  1 (406, 30, 2488, 0)                 (32, 374, 0, 0)    <<<
  2 (436, 94, 0, 0)                    (406, 30, 2486, 0) <<<
  3 (530, 32, 2482, 0)                 (456, 10, 2484, 0) <<<  setEmpData LP
  4 (562, 8, 0, 0)                     (466, 102, 0, 0)   <<<
  5 (570, 4, 2476, 0)                  (568, 4, 2476, 0)
  ...
```

The original has **no LP for `getCurrentCampaignMission`** and **no LP for
`player->setEmpData`** (row 2 covers 436..530 with LP 0), but it *does* have a
`PlayerFixedObject` ctor try-range `[530,562)` plus a gap `[562,570)` holding
the `Array` `operator new`.  Our reconstruction uses a placement `new` for
`PlayerFixedObject`, so it has no ctor try-range and the `Array` `operator new`
sits inside the big `[466,568)` gap.  These two structural differences cancel
to the same 57 rows / 315 bytes in the base.

## 2. Guard cost table (measured on the current base / no-mission body)

Removing the mission guard: raw **1,192 -> 1,156** (-36), table 315 -> 310,
`.ARM.extab` 156,312 -> **156,308** (-4, shifts `.rodata`).  Every +8 guard
measured on that body lands at 156,316; the cheapest is the fighter
`getShipGroup` guard at **+36 raw** (1,192, tie).  Summary of +extab cost:

| guard site | table bytes | extab |
|---|---:|---:|
| fighter `getShipGroup` (split local) | +10 | 156,316 |
| `ArraySetLength(11)` | +10 | 156,316 |
| `setWreckedMeshId` (any race) | +10 | 156,316 |
| `lodManager->addObject` (fighter) | +10 | 156,316 |
| `new Array<BoundingVolume*>` | +10 | 156,316 |
| `new Player` | +10 | 156,316 |
| `setBV` / `setLevel` / `getLevel` / `gameWon` | +12 | 156,320 |
| `delete fighter->cargo`, `field_0x25 = 0`, `ship = fixed`, `field_0x40 = 1` | +0 | 156,308 |

No guard was found that adds exactly **+4** (one row) on the current body.

## 3. Removing the fighter `setShipGroup` guard (G1)

`fighter->setShipGroup(grp, type, hostile);` without its `EmpGuard` gives raw
**1,188** (-4), size 2,764 exact, but `.ARM.extab` **156,304** (two rows
removed).  Re-adding a +8 guard costs >= +5 raw (`g1_getgrp_fighter` = 1,193,
`g1_setlevel` = 1,193), so the best landable G1 combination is **1,193** —
one worse than the base.  `g1_wreck0` is the only guard that adds just +4 on
this body, but it costs +8 raw (1,196).

## 4. `body_iff` (mission guard removed + x/y before the guard)

`int mission = ...;` (no guard) with `int x` / `float y` moved before the
`setEmpData` guard gives raw **1,173** (-19), size 2,764 exact, table 310 /
156,308.  It needs +4 extab; every guard on that body adds +8 or +12, and
restoring the mission guard gives 1,209.  Not landable without the extab fix.

## 5. Callee-saved rotation (the real residual)

The remaining ~1,192 bytes are almost entirely a systematic register rotation,
not source order:

| value | original | ours |
|---|---|---|
| `fixed` | `r9` | `fp` |
| `volumes` | `fp` | `r9` |
| `x` (float) | `sl` | `r6` |
| `y` (float) | `r8` | `r8` |
| `z` (float) | `r4` | `sl` |
| `race` | `r6` (then reloaded) | `r4` (kept live) |
| persistent zero | `r6` | `r0` |

Consequences: every `ldr.w r0,[fp,#4]` (original, `volumes`) vs `[r9,#4]`
(ours) is a 1-byte register-field diff; the BoundingAAB staging uses
`strd r6,r0,[sp]` (orig) vs `movs r0,#0; str r0,[sp]` (ours); the fighter arm
has an extra `mov r6,r4` (2 B) in ours.  Because `race` stays in `r4` in ours,
`type` is reloaded from the stack for the `type == 14` test, whereas the
original keeps it in `r4`.

## 6. New cells swept this pass (all size 2,764 unless noted)

All landed at **>= 1,192**:

* **identity jitter mapping** (1st nextInt -> x, 2nd -> y, 3rd -> z, i.e. the
  semantically faithful pairing; the base maps 1st -> z, 2nd -> x, 3rd -> y):
  late placement + `fx` = **1,194** (aligned 398); all-int = 2,101/2,752.
* coordinate type sweep (x/y/z int vs float, base placement): only
  `int x / float y / float z` = 1,192; everything else 1,203..2,515.
* coordinate before/after-guard placement (8 combos, base types): only
  `z before, x/y after` = 1,192; others 1,198..2,265.
* `wpz,wpx,wpy` declaration order: current is best; other orders 2,421/2,768.
* jitter declaration orders: `3,1,2` (current) 1,192; others 1,193/1,194.
* cargo-cleanup order: `0x2c,0x31,0x33` and `0x2c,0x33,0x31` both 1,192;
  `0x31`-first 1,195; `0x33`-first 2,038.
* tail statement orders (8 permutations of setShipGroup/addObject/
  field_0x40/setBV/ship): current 1,192; best alternative 1,195.
* `missionPtr` order: `!=23 && !=1` (current) 1,192; `!=1 && !=23` 1,193;
  no-expect / `!(==1||==23)` 1,199.
* `float fx` placement (after `::operator new`, after ctor): 1,197;
  `fx/fy/fz` declaration permutations: all 1,192.
* `race`/`type` local copies, `isBig` early, unsigned casts: all 1,192.
* `fixed`/`volumes` reference forms, `auto`, `const`, declaration-first,
  construction order, `ship = fixed` early: all 1,192.
* `__builtin_expect` on shipClass / type==14 / mission / waypoint: changes
  size (2,716..2,784), not landable.
* `level` clamp `<= 20 ?` form: 1,206; `>= 21` form: 1,192.
* `hitpoints *=` if/switch forms: 1,192.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13395
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb
# -> raw=1192 aligned=396 size=2764/2764 insn=1117/1117
python3 /home/debian/GalaxyOnFire2/scratch/w13395/lsda_cmp.py   # row dump
python3 /home/debian/GalaxyOnFire2/scratch/w13395/secdiff.py    # .text/extab
```

Scratch: `body_base.cpp`, `body_nomission.cpp`, `body_iff.cpp`,
`gen_*.py` / `v_*.json` sweeps, `lsda_cmp.py`, `secdiff.py`, `sweep2.py`.

## 8. Files changed

* this note only (no `src/` change; tree left clean at the base).
