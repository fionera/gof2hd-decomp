# `Level::createShip` — wave 189 (w13325): independent full-body / RA + EH-LSDA pass

Worktree `wt/w13325`, branch `wave189-w13325`, base master `b9d910e0e`.
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
2,764 B, VMA `0xc0214`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13325`.

**Status: no landable raw win; note only.**  `src/` is left at the base.
Base is raw **1,202**, aligned **484**, size **2,764/2,764**, insns
1,116/1,117; `.text` diff **74,017**; `.ARM.extab` **156,312 B @
0x1a3b78, diff 639**; `.ARM.exidx` 0; `.rel.plt` 0; file size exact.

This pass was asked to re-derive the whole function from the Ghidra
decompilation and try (a) the original `ble` polarity and `str.w r9` sink
together, and (b) a 336 B LSDA without guards around
`getCurrentCampaignMission` / `setEmpData`.  Everything below was
measured against the real link.

## 0. Bottom line

The only raw win found is `iff_rmM` (**1,177**, -25 raw), but its LSDA is
332 B, which shrinks `.ARM.extab` to 156,308 and shifts `.ARM.exidx`
(diff 0 -> 1,307).  With the section size restored, the best landable
variant is still the 1,202 base.  `g_rmS_addH` is 1,203 with `.ARM.extab`
content diff 638 (one byte better content, one byte worse raw).

## 1. Ghidra decompilation (new evidence)

`DecompAt2` on the Ghidra address `0xd0214` (image base `0x10000`;
note: `0xc0214` in the ELF is `0xd0214` in the project) gives the
authoritative shape:

* `mission = Status::getCurrentCampaignMission()` is a plain call, **no
  guard**.
* Waypoint fields are `0x124/0x128/0x12c`; the null arm zeroes them.
* Three `rnd->nextInt(40000)` calls in that order.
* `level = getLevel() > 20 ? 20 : getLevel()` (two calls, `ble`
  polarity).
* `hitpoints = (gameWon() ? 180 : mission * 4) + level * 14 + 20`.
* `switch (type)` order in the binary is `0x33, 0x31, 0x2c`.
* Mission 49/50/51/52/56 -> 270 (bitmask form).
* `empPoints = (getLevel() > 20 ? 20 : getLevel()) * 5 + 40` (two calls,
  same `ble` polarity).
* `shipClass == 1`: `hitpoints *= type == 14 ? 25 : 5`, `empPoints *= 3`,
  `empTime = 45000`, else 15000.
* float hitpoint adjustment; mission `0x9a` shift; `armor` ternary;
  `new Player(armor, hitpoints, 1, 1, 0)`.
* **x, y, z are all int computations before `setEmpData`**:
  `x = wpx + jitter1 - 20000`, `y = wpy + jitter2 - 20000`,
  `z = wpz + jitter3 - 20000`, then `setEmpData`; the ints are converted
  to float after the call in each ship branch.
* `shipClass == 0`: `new PlayerFighter(...)`; `getShipGroup`; virtual
  `setShipGroup`; `missionPtr` (`this+0xc0`) check; `addObject`;
  cargo delete / `field_0x25`.
* `shipClass == 1`: `new PlayerFixedObject(...)`;
  `new Array<BoundingVolume*>`; 24 `new BoundingAAB` (11/3/2/3/5 per
  race/type arm); `setWreckedMeshId`; `getShipGroup`; virtual
  `setShipGroup`; `addObject`; `field_0x40 = 1`; `setBV`.
* `ship->setLevel(this)`; return.

The decompiler's `iVar3/iVar1/fVar20` coordinate block confirms the
original pairs **x with the first `nextInt`, y with the second, z with
the third** (natural jitter order), unlike the base source which names
them `jitter3, jitter2, jitter1` and pairs x with the third.  The
natural order is semantically faithful but measures 1,204/1,205 raw
(slots move, codegen otherwise identical), so the base naming stays.

## 2. `.ARM.extab` record format (new, exact)

The per-function entry is:

```
word   personality routine (prel31, relocated)
15 B   fixed header  c9 4d 97 02 f0 80 00 82 b0 b0 b0 ab ff ff 01
ULEB   call-site table length
table  (start, length, landing_pad, action) ULEB tuples
```

Total entry size is `align4(21 + table_len)`.  Therefore a 332 B entry
is **table_len 310**, and 336 B requires **table_len >= 312** (not 315:
21+312 = 333 -> padded 336).  This is the key budget fact: `iff_rmM`
is only **2 table bytes** short of the section size, not 4.

The table is a list of explicit ranges; gaps between ranges are
implicit no-landing-pad regions.  Original: **57 rows, 315 table
bytes**.  Base: 57 rows, 315.  `iff_rmM`: **56 rows, 310**.

Row-size accounting for `iff_rmM`:

* 28 landing-pad rows: `start(2) + len(1) + lp(2) + action(1) = 6`
  each -> 168 B.
* 28 no-LP rows: 26 with `start(2)+len(1)+lp(1)+action(1) = 5`, plus
  `[1296,1610)` len 314 and `[2318,2764)` len 446 at 6 each -> 142 B.
* total 310.

Original: 28 LP rows (168) + 29 no-LP rows (27*5 + 2*6 = 147) = 315.
The missing 5 bytes are exactly **one extra no-LP gap row**.  The
original's 28 LP rows are exactly its 28 `new` expressions (1 Player,
1 PlayerFighter, 1 PlayerFixedObject, 1 Array, 24 BoundingAAB).  The
base/`iff_rmM` 28 LP rows are 26 real-`new` + 2 `EmpGuard` call sites
(setEmpData, setShipGroup); the PlayerFighter/PlayerFixedObject
placement-new calls contribute no LP, and the PlayerFixedObject LP+gap
in the original collapses into one lp0 row in ours.

## 3. Guard sweep on `iff_rmM` (raw 1,177, table 310)

Every guard was added as `{ EmpGuard guard; call; }` and measured.

| guard site | raw | table | entry |
|---|---:|---:|---:|
| none (`iff_rmM`) | **1,177** | 310 | 332 |
| `getCurrentCampaignMission` | 1,213 | **315** | **336** |
| `getShipGroup` (fighter) | 1,213 | 316 | 340 |
| `addObject` (fighter) | 1,216 | 316 | 340 |
| `ArraySetLength(11)` | 1,216 | 316 | 340 |
| `setWreckedMeshId(0x4780)` | 1,216 | 316 | 340 |
| `getLevel` (level clamp) | 1,258 | 319 | 340 |
| `gameWon` | 1,241 | 323 | 344 |
| `getLevel` (empPoints) | 1,213 | 323 | 344 |
| `inAlienOrbit` | 1,220 | 323 | 344 |
| `hardCoreMode` | 1,584 | 323 | 344 |
| `setBV` / `setLevel` / `ArraySetLength(3)` | 1,219/1,213/1,216 | 323 | 344 |
| `PlayerFixedObject` ctor | 1,221 | 323 | 344 |
| `delete fighter->cargo` | 1,177 | 310 | 332 (optimised away) |

Only the mission guard lands on 315/336, and it costs 36 raw bytes
(1,177 -> 1,213).  No guard at offset <128 other than the mission call
exists; guarding the `nextInt` calls (offset ~88-104) changes codegen
badly (2,272-2,455, size -16).  Guarding the first `getLevel` (offset
114) is +81 raw.

## 4. Real-`new` row map and the 336 B faithful shape

The faithful real-`new` body (`PlayerFighter`/`PlayerFixedObject` as
`new` expressions, no guards) is 336 B/57 rows but raw **1,987-1,990**
(size 2,764).  It is the only shape that reproduces the original's
28 `new` landing pads exactly; the raw cost is the delete landing pads
and the changed register allocation.  `rn_flip_iff` = 1,987, extab
diff 638.

## 5. `str.w r9` sink attempts

Base/`iff_rmM` spill `this` at `0xc031c` (end of the `cmp #20` block);
the original spills at `0xc0332` (end of the join, after `cmp r6,#1` /
`add sl,r0,#40`).  `this` is reloaded at `0xc03fe` in ours vs
`0xc071e` in the original; the tail then assigns `this` to `sl` instead
of `r9`.

Tried without moving the spill: `ble` flip and explicit-if forms
(1,202-1,209), comma/`(void)this`/`Level *self` aliases (neutral),
`register` copy (neutral), `this->` forms (neutral), volatile dummy
(+44 size), `__builtin_expect` on the clamp (size +8) and on
`shipClass`/mission (1,392-2,274), `empTime` reorder/inversion
(1,209-2,486), switch-case order (1,202-1,230), mission-chain form
(1,202), coordinate types `iff/fif/ffi/iii/iif/fii/ifi/fff`
(1,177-2,492), and jitter declaration orders (1,202-1,205).  No
source-level shape moved the RA spill point.

## 6. Landable candidates

| variant | raw | aligned | size | table | entry |
|---|---:|---:|---:|---:|---:|
| base | **1,202** | 484 | 2,764 | 315 | 336 |
| `g_rmS_addH` (base, no mission guard, flip, getShipGroup guard, no setShipGroup guard) | 1,203 | 484 | 2,764 | 315 | 336 |
| `iff_rmM` | **1,177** | 369 | 2,764 | 310 | **332** |
| `iff_rmM` + mission guard | 1,213 | 396 | 2,764 | 315 | 336 |

`iff_rmM` linked: `.text` diff 73,992, but `.ARM.extab` 156,308
(diff 122,309) and `.ARM.exidx` diff 1,307 -> net loss.  With the
section size restored (mission guard) it is 11 raw worse than base.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13325
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb
# -> raw=1202 aligned=484 size=2764/2764 insn=1116/1117
```

Scratch: `try.py` (splice body + build + `fn_diff` + object
`.ARM.extab` length/rows), `measure.sh`, `sec.py`, `regions.py`,
`dump2.py` (row parser), `gen_coord.py`/`gen_guards.py`/
`gen_guards2.py`/`gen_combo.py` (variant generators), and the
`fn_*.txt` bodies used above.

## 8. Files changed

* this note (no `src/` change; `Level.cpp` left at the base).

## Harvest on wave-189 master (`f08f21868`)

Note-only: raw stays 1,202 / 484 / 2,764.  The 1,177 `iff_rmM` candidate is
`.ARM.extab`-short (156,308 B, `.ARM.exidx` diff 1,307) — a net loss.  The
next target is a non-call guard/cleanup scope that restores the 336 B / 57-row
LSDA without changing the 1,177 codegen (w13340).
