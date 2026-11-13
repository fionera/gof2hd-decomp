# `Level::createShip` — wave 191b (w13536): LSDA/landing-pad audit + `notop` budget

Worktree `wt/w13536`, branch `wave191b-w13536`, base master `7ace220f3`.
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
2,764 B, VMA `0xc0214`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13536`.

**Status: no raw win landed; tree clean.**  Baseline unchanged at raw **1164**,
aligned 400, size 2,764/2,764, insns 1,117/1,117, `.text` diff 71,506,
`.ARM.extab` 586 (size/base exact), `.ARM.exidx` 0, `.rel.plt` 0.
This note records the new evidence and the exhaustive negative sweeps.

## 0. The `.ARM.extab` / landing-pad structure (new evidence)

Parsing the LSDA of the original and of our build (`parselsda.py`) gives the
key structural fact:

* **Original**: 57 call-site rows, 28 landing pads, table length 315, LSDA 332,
  extab entry 336.  **Every** landing pad belongs to a `new` expression
  (`new Player`, `new PlayerFixedObject`, `new Array<BoundingVolume*>`,
  24 × `new BoundingAAB`, `new PlayerFighter`).  There is **no** landing pad
  for `setEmpData`, `setShipGroup`, `getShipGroup`, `addObject`, `setBV`,
  `setLevel` or `getCurrentCampaignMission` — all of those calls sit in
  `lp = 0` regions.  So the original has **no `EmpGuard` at all**.
* **Our base**: 57 rows, 29 landing pads, table 315, entry 336.  It has three
  fake `EmpGuard` pads (top `getCurrentCampaignMission`, `setEmpData`,
  `setShipGroup`) and is **missing** the `PlayerFixedObject` pad (placement
  `new` does not delete on ctor throw).  The counts happen to cancel.

The landing-pad code confirms it: the original has four delete variants
(`sl`=PlayerFighter, `fp`=Array, `r9`=PlayerFixedObject, `r5`=BoundingAAB),
saving the exception in `r8`; ours has only two delete variants (`r9`, `r5`)
plus two resume-only variants (placement new).

## 1. `notop` — the obvious raw win, but a section-budget loss

Removing only the top `EmpGuard` around `getCurrentCampaignMission`
(`int mission = Globals::status->getCurrentCampaignMission();`):

| metric | base | `notop` |
|---|---:|---:|
| raw | 1164 | **1128** (-36) |
| size | 2764/2764 | 2764/2764 |
| `.rel.plt` | 0 | 0 |
| `.text` diff | 71,506 | **71,470** |
| LSDA rows / table | 57 / 315 | 57 / **310** |
| `.ARM.extab` | 156,312 exact | **156,308 (-4)** |
| `.ARM.exidx` diff | 0 | **1,307** |
| `.ARM.extab` content diff | 586 | 122,283 (shift) |

The function is faithful without the guard (the original's first row is
`(0,406,0,0)`, i.e. `lp=0`), but the extab entry drops 336 -> 332 and every
later `.ARM.extab` entry shifts by 4, so the raw-byte extab diff explodes.
`check` still passes (byte_exact 4507, linked_exact 4507) but the section-level
parity regresses.  **Net loss by the text+extab+build-id metric.**

### Why a "4-byte row" is hard

The extab entry is `4 + round_up(17 + table, 4)`.  To restore 336 the table
must be **312..315**; `notop` is 310, so we need +2..+5 bytes.  Row encodings
(all fields uleb128): a landing-pad row is always 6 B (offset 2, len 1, lp 2,
action 1); an `lp=0` row is 5 B (or 6 if len >= 128).  A 4-byte row requires
`lp < 128`, which is impossible here (all landing pads are at 0x9xx), *or* an
`lp=0` row with offset < 128 and len < 128.  The only 5-byte addition found is
the **top guard itself**: it is around the call at offset 28, where the region
before it (0..28) contains no call and becomes a gap, so it costs exactly +5
(`(28,4,lp,0)` = 5 B, replacing `(0,406,0,0)` = 5 B with `(32,374,0,0)` = 5 B).
Guards on the next calls (jitters at offsets 88/98/108, getLevel, gameWon)
all cost +6 or +9 (the region before them keeps a call, so it is emitted as
its own `lp=0` row).  Tested: top (`+5`, base), jitter1/2/3 (`+6`), getLevel
(`+6`), gameWon (`+8`).  No guard gives +2..+5.

## 2. Faithful `new` shape (wave-111) vs. the tuned `EmpGuard` shape

Re-applying the wave-111 "real `new`" shape (no `EmpGuard`s, plain
`new PlayerFighter` / `new PlayerFixedObject`) gives the **correct** 57 rows /
28 pads / 336 B extab, but raw **2096** (size 2764): its prologue differs
(`strd r1,r2,[sp,#56]; mov sl,r3` vs. the original
`str r1,[sp,#60]; mov fp,r3; mov r5,r2`).  The current `EmpGuard` shape is a
codegen lever that reproduces the original's *prologue* (it matches 0x0-0x106
byte for byte) at the cost of the fake landing pads; it is 932 raw bytes better
than the faithful shape despite being less faithful.  Plain `new` for just one
of the two objects (keeping the guards) gives 60-62 rows and extab 156,324 /
156,336 (size 2768-2772).

## 3. Other negative sweeps (all size 2764 unless noted)

| lever | raw |
|---|---:|
| **base** | **1164** |
| `rm_grp` (drop fighter `setShipGroup` guard) | 1160, extab 156,304 (-8) |
| `rm_grp` + `getShipGroup` guard (guard moved) | 1165, extab **exact** |
| `rm_grp` + `setLevel` guard | 1165, extab exact |
| `notop` + `rm_grp` + `getShipGroup` guard | 1129, extab 156,308 |
| `notop` + `setEmpData` guard removed + jitter2 guard | extab exact, size 2776 |
| faithful all-int spawn (`x=wpx+j1`, `y=wpy+j2`, `z=wpz+j3`, before `setEmpData`) | 2296-2354, size 2748-2772 |
| spawn `x` before the guard (sunk anyway) | 1165 |
| spawn `y = jitter1+jitter2-20000` (2-jitter) | 1165 |
| spawn expression reorder (`wpy+wpx`, `wpx-20000+wpy`) | 1164 |
| spawn `(float)wpx + (float)wpy - 20000.0f` | size 2780 |
| clamp `(lv<=20?lv:20)*5` / if-else forms | 1167-1174 |
| tail `setBV` first (faithful) | 1170 |
| hull `ArraySetLength` first (faithful) | 1172 |
| declaration-order / alias / `mp` local / `Level *self` | 1164 or worse |

The clamp region (0x100-0x122) is the only early mismatch: the original spills
`this` (`r9`) at 0x11e *after* `add.w sl,r0,#40`, while clang always spills it
at 0x108 (right after `cmp r0,#20`) in our build; this 4-byte insertion is what
shifts the whole body and produces the `fp`/`r9` (fixed/volumes) register swap
from 0x1b4 onward.  No source form tried (ternary, if/else, `if (lv>20)`,
`lv` local, `empTime` declared first, `mp` local) moves the spill.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13536
( ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2 )
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb   # raw=1164 aligned=400 size=2764/2764
```

Scratch: `Level.cpp.base` (current source), `parselsda.py`, `full.py`,
`bytediff.py`, `insdiff.py`, `sweep1.py`..`sweep17.py`, `rand5.py`, saved
`.so`s `base.so`, `e6_notop.so`, `e12_rmg_gg.so`, `e15_wave111.so`, etc.

## 5. Files changed

* this note only (no `src/` change; tree clean).

## 6. Follow-up (same worker): body is guard-invariant; pad-tail budget

Additional measurements this session (all rebuilds from the pristine base):

* **The top `EmpGuard` does not touch the body at all.**  `base.so` and
  `e6_notop.so` have byte-identical `.text` from 0x000 to the landing-pad
  tail; the *only* differing bytes are 0x97c..0x9c7 (36 B).  So the top
  guard is a pure pad-tail cost, not a codegen lever.  `notop` raw 1,128 vs
  base 1,164 is exactly the pad tail.
* **Original pad tail (0x976..0x9c6, 82 B)** has 24 `b.n` then *three*
  delete variants (`sl`=PlayerFighter, `fp`=Array, `r9`=PlayerFixedObject)
  plus the `r5`=BoundingAAB delete and a shared resume.  Our base tail
  (0x97c..0x9c8, 76 B) has only two delete variants (`r9`=Array, `r5`),
  because PlayerFighter and PlayerFixedObject are placement-new.  The body
  is 6 B longer than the original (0x97c vs 0x976), so the totals match.
* **Declared call-site length** (LSDA header `01 <uleb>` at `e+4+14`):
  original 315, base 315, `notop` 310, `rmgrp` 304, `rmemp` 310.
  Entry size = `4 + round_up(17 + cs_len, 4)`, so 336 requires
  **cs_len 312..315**.
* **A guard costs at least +5 rows** (a 5-B lp=0 row `(0,406,0,0)` split at
  its first call becomes `(28,4,lp,0)`+`(32,374,0,0)` = 10 B).  Every
  other placement splits a row with a 2-byte start and costs +6..+15.
  `notop` (310) can therefore only be repaired by the top guard itself
  (+5 -> 315), which is exactly base.
* `rmgrp` (304) + one guard: `getShipGroup` 315 raw **1165**,
  `ship->setLevel` 315 raw **1165**, `inAlienOrbit` 315 raw 1166,
  `gameWon` 315 raw 1194, `getLevel` 314 raw 1191, `jitter3` 313 raw 1464,
  `setWreckedMeshId(0x4780)` 310 raw 1168, `fixed->setShipGroup` 321,
  `addObject` 315 raw 2410.
  Best in-window is 1165, one byte worse than base.
* `notop` + one guard: `setWreckedMeshId(0x4780)` 316 raw 1167,
  `getLevel` 320 raw 1189, `gameWon` 322 raw 1192, `jitter2/3` 319 raw
  1462-1468, `getShipGroup`/`addObject`/`setLevel` 319-323 raw 1164-1167.
  All overshoot 315.
* Guarding `new Player` / `new PlayerFighter` / `new PlayerFixedObject` /
  `new Array` changes the delete-pad set and the body (raw 2,091-2,474,
  size 2,748-2,772).
* Faithful spawn (`int x=wpx+jitter1-20000; int y=...; int z=...` before
  `setEmpData`) gives size **2,748** (16 B short) and raw 2,446; the
  float form gives 2,732.  The current reassociated spawn is a strong
  local optimum.
* The 6-byte body overhang starts at 0x1b6 (the spawn block), not at the
  clamp: the clamp region is exactly 40 B in both, only the `str r9` is
  hoisted from 0x11e to 0x108.

Conclusion: the residual 1,164 B is dominated by the `fp`/`r9`
(fixed/volumes) register swap that starts at 0x1b4 and by the spawn-block
reassociation; every source lever tried either keeps 1,164 or lands at
1,165+ with cs_len 315.  No size-exact, extab-exact, raw-positive change
was found.
