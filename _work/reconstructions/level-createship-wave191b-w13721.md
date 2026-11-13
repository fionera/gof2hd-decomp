# `Level::createShip` — wave 191b (w13721): LSDA-row arithmetic re-check, no landable win

Worktree `wt/w13721`, branch `wave191b-w13721`, base master `5e8bc222f`.
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
2,764 B, VMA `0xc0214`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`).  Scratch `scratch/w13721`.

**Status: no landable win.  Tree left at the pristine baseline.**
Raw **1,165**, aligned **385**, size **2,764/2,764** exact, insns
1,117/1,117, `.text` contentdiff **72,153**, `.ARM.extab` **156,312**
(contentdiff **564**), `.ARM.exidx` contentdiff **0**, `.rel.plt`
byte-identical.

## 0. Baseline re-verified

```
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb   # raw=1165 aligned=385 size=2764/2764
# .text diff 72153 (size 1,259,464)
# .ARM.extab diff 564 (size 156,312)
# .ARM.exidx diff 0 (size 28,528)
```

## 1. The binding constraint is the 336-byte LSDA record

The record is `16 B personality + 5 B LSDA header + call-site table`,
rounded up to 4.  The baseline call-site table is **315 B / 57 rows**
(record 336, extab 156,312, exidx byte-identical).  A row is
`(start, len, lp, action)`, all ULEB128; `lp` is always > 0x80 here, so a
row costs **5 B** only when `start < 128` and `len < 128`, and **6 B**
otherwise.

Consequence (also derived independently in
`level-createship-wave191b-w13542.md`):

* dropping the top `EmpGuard` (the guard on
  `getCurrentCampaignMission`, the **first** call) removes exactly one
  5-byte row: 315 -> 310, record 336 -> 332, extab 156,312 -> 156,308,
  and every later `prel31` in `.ARM.exidx` shifts -> contentdiff 1,307.
* every other call in the function sits at `start >= 128`, so a guard
  there adds a **6-byte** row (or two rows, 9-11 B).  There is no way to
  put back the missing 5 B except by guarding the first call again --
  i.e. the top `EmpGuard` itself.  This is why the drop-top raw win
  cannot be made admissible.

## 2. Measured this session (all size-exact 2,764 unless noted)

| variant | raw | aligned | `.text` | extab diff | extab size | exidx diff |
|---|---:|---:|---:|---:|---:|---:|
| baseline (top + setEmpData + setShipGroup guards) | **1,165** | **385** | 72,153 | **564** | **156,312** | **0** |
| drop top guard (g1off) | **1,129** | 358 | 72,117 | 122,285 | 156,308 | 1,307 |
| drop setShipGroup guard (g3off) | 1,161 | 405 | 72,149 | 107,783 | 156,304 | 1,327 |
| **g3off + guard `getShipGroup`** (h1) | 1,166 | **383** | 72,154 | **563** | **156,312** | **0** |
| g1off + guard `getShipGroup` | 1,130 | 356 | 72,118 | 122,284 | 156,308 | 1,307 |
| g1off + guard `setLevel` | 1,165 | 385 | 76,925 | 107,906 | 156,320 | 1,325 |
| g3off + guard `setLevel` | 1,166 | 383 | 72,154 | 618 | 156,312 | 0 |
| g3off + guard `inAlienOrbit` | 1,167 | 383 | 72,155 | 586 | 156,312 | 0 |
| g3off + guard `addObject` (fighter) | 1,169 | 382 | 76,929 | 122,253 | 156,316 | 1,304 |

`h1` is the only admissible guard swap, and it is a **perfect tie in
total section bytes**: 72,154 + 563 = 72,717 = 72,153 + 564.  It buys
aligned 385 -> 383 at raw 1,165 -> 1,166, so it is not a raw win and was
not landed (same conclusion as w13542/w13706).

## 3. Guard placements measured against the drop-top form

Every one of these keeps size 2,764 but overshoots the 315-byte table
(the row is 6 B, or two rows); none restores extab 156,312:

| guard added after dropping the top guard | raw | extab size | table / record |
|---|---:|---:|---:|
| `gameWon` | 1,193 | 156,320 | 322 / 343 |
| `inAlienOrbit` | 1,166 | 156,320 | 321 / 342 |
| `getShipGroup` (fighter) | 1,165 | 156,316 | 316 / 337 |
| `addObject` (fighter) | 1,168 | 156,316 | 316 / 337 |
| `setWreckedMeshId` | 1,168 | 156,316 | 316 / 337 |
| first `ArraySetLength(11)` | 1,168 | 156,316 | 316 / 337 |
| `PlayerFixedObject` placement ctor | 1,176 | 156,320 | 321 / 342 |
| `getLevel` (clamp) | 1,190 | - | 320 / 341 |
| `nextInt` j3 / j1 | 2,372 / 2,369 | - | 319 / 340 |
| `nextInt` j2 | 1,463 | - | 319 / 340 |
| `hardCoreMode` | 2,465 (size 2,768) | - | - |

The `nextInt`/`getLevel` guards split the prologue row and add *two*
rows (9-11 B), so they cannot hit 315 either.

## 4. Spawn-form re-check (declaration order around `setEmpData`)

The landed shape is deliberately non-faithful
(`z = wpx+wpy`, `x = wpz+jitter3`, `y = jitter1+jitter2+jitter3`).
Re-sweeping the six declaration orders of those exact expressions
around the `setEmpData` guard (Z/X/Y = the three declarations, G = the
guard) confirms the w13706 result:

| order | raw | aligned | size |
|---|---:|---:|---:|
| **Z G X Y** (landed) | **1,165** | **385** | **2,764** |
| Z X G Y | 1,166 | 385 | 2,764 |
| X Z G Y | 1,166 | 385 | 2,764 |
| Z Y G X | 1,990 | 382 | 2,760 |
| Z X Y G | 1,990 | 382 | 2,760 |
| X Y Z G / Y Z G X / Y X Z G | 1,990 | 382 | 2,760 |

So exactly one of `x`/`y` must stay after the guard for size 2,764, and
the landed `Z G X Y` is the unique raw optimum.

## 5. Clamp re-check

The faithful `min(getLevel(),20)` shape
(`int e = getLevel() > 20 ? 20 : getLevel(); int empPoints = e*5+40;`)
reproduces the original `cmp #20 / movs #20 / shared add` block exactly
but the `this` spill is still hoisted into `bb.17` (`c031c`) instead of
`bb.20` (`c0332`): raw 1,175, aligned 381, `.text` 72,163, extab 564,
size 2,764.  Not a win on its own (w13683/w13706).

## 6. Branch-order swap

Putting the `shipClass == 1` body first changes the layout badly:
raw 2,481, aligned 799, **size 2,788** (fatal).  The original's source
order is `shipClass == 0` first, as reconstructed.

## 7. Conclusion

The residual is the known `this`-in-`sl` register rotation (race-0
type-14 hull 696 B, race-2/3 307 B, spawn block 127 B) plus the
`bb.17` `this` spill.  All four fresh angles in the task brief were
re-measured:

* (a) hull blocks -- no source lever flips `this` out of `sl`;
* (b) `missionPtr`/`hostile`/`group` ladder -- neutral (w13706);
* (c) `EmpGuard`/`setShipGroup`/`setEmpData` sequence -- the h1 tie is
  the only admissible swap; the drop-top raw win cannot be compensated
  because the only 5-byte LSDA row is the top guard itself;
* (d) `lodManager->addObject` tail -- `setBV` first and inline
  `geometry` are both worse (w13706).

No measured win; `src/` left untouched.

## 8. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13721
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb   # raw=1165 aligned=385 size=2764/2764
python3 scratch/w13721/lsda2.py cmake-build-match/libgof2hdaa.so 0x1ab09c
# lp=ff tt=ff cs=01 len=315 table_start=21   (57 rows)
```

Scratch helpers (all in `scratch/w13721`): `try.py` (splice + build +
measure), `idiff.py`, `lsda2.py`, `grid.py`, `sweep31..44.py`,
`Level.cpp.base` (pristine source copy), `nog1.so` (drop-top build).

## 9. Files changed

* this note only; `src/game/world/Level.cpp` is unchanged from `HEAD`.
