# `Status::missionCompleted` — wave 190 (w13413): pool-alignment lens

Worktree `wt/w13413`, branch `wave190-w13413`, base master `96f53fadf`.
Target `_ZN6Status16missionCompletedEbbx` (`src/game/mission/Status.cpp`,
1,432 B, VMA `0xabf38`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13413`.

**Status: research note, no `src/` change landed.**  Baseline re-confirmed
on the committed source: raw **451**, aligned **205**, size
**1,432/1,432**.  `check` green, tree clean.

## 0. Per-region diff map (new evidence)

The 451 raw bytes are not spread evenly.  Splitting the function at the
original body boundaries gives:

| region | diff bytes / region bytes |
|---|---:|
| prologue `abf38..abfda` | 0 / 56 |
| dispatch `abfda..ac036` (rank table + TBH) | 43 / 43 |
| 167/174 body `ac036..ac060` | 1 / 17 |
| 8 body `ac060..ac0c4` | 2 / 35 |
| 0/11, 13, 14 bodies `ac0c4..ac110` | 0 / 23 |
| 150..166 bodies `ac110..ac2a6` | 18 / 138 |
| case-10 body `ac2a6..ac2b4` | 4 / 4 |
| 184 body `ac2b4..ac2ee` | 11 / 15 |
| 189 body `ac2ee..ac32a` | 17 / 17 |
| 156..168 bodies `ac32a..ac3fc` | **60 / 60** |
| 169..172 bodies `ac3fc..ac45a` | 0 / 36 |
| latch/return blocks `ac45a..ac4c4` | 0 / 41 |
| end literal pool `ac4c4..ac4d0` | 3 / 8 |

So 60 of the 451 bytes are the **156..168 block shift**: our 156 body
starts at `ac32e` instead of `ac32a` (+4).  Every byte in that region
differs only because the whole run is four bytes late; the code itself is
instruction-for-instruction identical.  The 189 body (17/17) and most of
150..166 (18/138) are the same shift.

## 1. Where the +4 comes from

The original's 189 (case-38) body is 60 B (`ac2ee..ac32a`) and contains
no literal pool.  Ours is 68 B because clang places the first constant
pool (`ac300`, 8 B = rank-table address + `Globals::status` for case 208)
*inside* the case-38 loop preheader.  The shift at 156 is exactly

```
case10_delta + case12_delta + pool_delta
   -6        +    +2       +   +8            = +4
```

`case10_delta = -6` is the documented 64-vs-70 B case-10 body;
`case12_delta = +2` is the landed CBA `||` order (60 vs 58 B); the 8 B
pool is the remaining lever.

If the first pool were **rank-only (4 B)** and the `Globals::status`
literal moved to the end pool, the 189 body would be 64 B and 156 would
land exactly on `ac32a`, collapsing the 60-byte shift (and most of the
189/150..166 diffs) at unchanged function size.  That is the target shape.

## 2. Measured pool-placement cells (all semantic-equivalent)

`case 208` is the only early `Globals::status` user.  Replacing it with
the equivalent member access `currentCampaignMission` (the singleton is
`this`) removes its literal from the first pool, making the pool
rank-only (4 B).  It also shortens the case-208 body by 8 B:

| variant | 189 start | 156 start | size | raw | aligned |
|---|---|---|---:|---:|---:|
| base (CBA, full `Globals`) | `ac2ea` | `ac32e` | 1432 | **451** | 205 |
| case-208 `this` | `ac2e2` | `ac322` | 1420 | 1141 | 204 |
| case-208 `this` + case-10 re-query | `ac2ea` | **`ac32a`** | 1428 | 928 | 190 |
| case-208 `this` + re-query + ABC | `ac2e8` | `ac32a` | 1428 | 942 | 178 |
| case-208 `this` + re-query + no uxth | `ac2ea` | `ac32a` | 1428 | 923 | 183 |
| case-10 `ship->hasEquipment` (drops end-pool word) | `ac2ea` | `ac32e` | 1428 | 464 | 120 |
| case-10 re-query (full `Globals`) | `ac2f2` | `ac336` | 1440 | 666 | 205 |
| case-12 ABC (full `Globals`) | `ac2e8` | `ac32e` | 1432 | 466 | 193 |

The `this` + re-query row proves the pool hypothesis: **156..168 land
exactly on the original addresses** (`ac32a`, `ac354`, `ac388`, `ac3b6`,
`ac3d2`, `ac3d6`, `ac3de`).  But the 8 B lost in the case-208 body shifts
the whole early run (150..166) to -8 and the function is 4 B short
(1428), so raw rises to 928.  Every size-exact combination of these
knobs still measures 451 (see §3).

## 3. Exhaustive knob sweep

54 combinations of `{base, re-query, ship->hasEquipment, ship+re-query}
× {CBA, ABC} × {(unsigned short), plain, both-cast}` were built and
measured.  All size-exact results are 451 or worse:

* 451: base/CBA/`(unsigned short)` (committed), base/CBA/both-cast
* 466: base/ABC/*
* 680: base/CBA/plain
* 464: ship/CBA/* (size 1428)
* 666..678: re-query/* (size 1440)

Also re-measured and neutral (451): case-6 declaration order, comma
declaration, `hasGood`/`hasSpecial` operand swap, `&`, `== 1`, `!!`,
`&& true`, nested `if`, `for` counter type/`++j`, outer loop `++i`/
`i += 1`/`i = i + 1`, `Mission *const`, `bool won` local, `int idx`
local, `const` locals, `isInList(...) != 0`, case-9 `int complete`,
case-9 `hasCargo(...) == 0`, source-order permutations of cases
6/9/10/12/20/208 (28 moves; only 10-20-12 and a few neighbours keep
451), and 5 rank-table gap patterns (current, gap-24→25, all-front,
all-back, original-default-pattern) — the TBH bytes are all different
either way because the `tbh` base is 2 B late, so gap placement does not
change the raw count.

## 4. Remaining irreducible pieces

* dispatch 43 B: rank-table `ldr/add/ldrb` is 2 B longer than the
  original's `sub/cmp`, so the `tbh` is at `abfe4` instead of `abfe2` and
  every TBH halfword differs by one; the table is rank-ordered
  (monotonic) while the original's is type-ordered.
* 156..168 60 B: the +4 block shift, fixable only by the 4 B pool above.
* 189 17 B: the same pool.
* case-10 4 B: r5/r6 vs the original's r9/r5 allocation plus the shared
  bool tail.
* 184 11 B: CBA vs the original's ABC instruction order (CBA is 2 B
  longer but avoids the pool-alignment `nop`, which is why it wins).
* 169..172, latch/return, prologue: 0 B.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13413
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN6Status16missionCompletedEbbx     # raw=451 aligned=205 size=1432/1432
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w13413/` (`runvar.py`,
`combo.py`, `gap.py`, `probe.py`, `bodymap.py`, `moveblock.py`,
`reorder.py`, `Status.base.cpp`).
