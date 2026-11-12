# `Level::createShip` — wave 191 (w13529): drop the non-original top `EmpGuard`

Worktree `wt/w13529`, branch `wave191b-w13529`, base master `ce4bd3c6c`.
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
2,764 B, VMA `0xc0214`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13529`.

**Status: landed a size-exact, `.rel.plt`-identical raw win.**
Raw **1,173 -> 1,137** (-36), aligned 468 -> 489, size **2,764/2,764**
exact, insns 1,115/1,117.  `.text` diff **71,268 -> 71,232** (-36).
`.rel.plt` byte-identical.  `.ARM.extab` **shrinks** 156,312 -> 156,308
(-4); `.ARM.exidx` content diff 0 -> 1,307 (the 4-byte extab shrink
shifts every later LSDA entry by 4).  `check` green (RATCHET PASS).

## 0. What changed

The `EmpGuard` around the very first call was removed:

```cpp
-    {
-        EmpGuard guard;
-        mission = Globals::status->getCurrentCampaignMission();
-    }
+    mission = Globals::status->getCurrentCampaignMission();
```

The original has **no** cleanup range for the first 406 bytes of the
function.  Its LSDA call-site table starts

```
start=0   len=406 lp=0        <- no landing pad at all
start=406 len=30  lp=2488     <- new Player(...) ctor cleanup
```

whereas our previous form started `start=28 len=4 lp=2494` (the
`getCurrentCampaignMission` guard).  Removing the guard makes the first
row `(0,406,0)` — matching the original — and removes one landing pad
from `.text` (the -36 raw bytes are entirely in the landing-pad tail at
`0xc0bc0..0xc0bd6`; the main body is unchanged).

## 1. Why the .ARM.extab record shrinks

The original `createShip` LSDA record is 336 B / 57 rows (documented in
`docs/VALIDATION.md`).  Our base form was also 336 B / 57 rows.  Dropping
the top guard removes one 5-byte call-site row, leaving a 332 B / 56-row
record.  The section therefore becomes 156,308 B (4 short) and every
later LSDA entry shifts down by 4, which is why the `.ARM.exidx` content
diff jumps from 0 to 1,307 and the `.ARM.extab` content diff from 1,216
to 122,273.  Verified: all **1,282** differing `.ARM.exidx` entries are
`word1 - 4` and nothing else (a pure pointer shift); the `.ARM.extab`
diff is dominated by the same shift (1,379 bytes still differ after
re-aligning by 4, vs 1,216 for the whole section before).

This is the same class of effect that made wave 132 revert a 328 B
`try`/`catch` record.  It is reported here rather than hidden: the win
is **-36 raw / -4 extab size** and the cost is the 4-byte LSDA shift.
A future pass that re-adds a 5-byte call-site row *without* adding
`.text` (e.g. a cleanup the compiler can tail-merge with an existing
landing pad) would restore the 336 B record and the exact `.ARM.exidx`.

## 2. Measurements (all size-exact 2,764, `.rel.plt` identical)

| variant | raw | aligned | size | extab size |
|---|---:|---:|---:|---:|
| baseline (w13499 shape) | 1,173 | 468 | 2,764 | 156,312 |
| **drop top `EmpGuard` (landed)** | **1,137** | **489** | **2,764** | **156,308** |
| drop ship `EmpGuard` | 1,169 | 492 | 2,764 | 156,304 |
| drop ship + guard `ship->setLevel` | 1,174 | — | 2,764 | 156,312 |
| drop top + guard `addObject` | 1,176 | 468 | 2,764 | 156,316 |
| drop top + guard `ArraySetLength(5)` | 1,176 | 468 | 2,764 | 156,320 |
| faithful spawn (`wpz+j2`, `wpy+j1`) on baseline | 1,178 | — | 2,764 | 156,312 |
| drop `setEmpData` guard | 2,181 | — | 2,780 | 156,308 |
| drop all three guards | 2,180 | — | 2,776 | 156,292 |

Note the non-monotonicity: the `setEmpData` guard is load-bearing for the
2,764 B size (removing it re-allocates and grows the function to 2,780),
so it cannot be dropped even though the original has no such cleanup.

Every guard added back on top of the landed form costs +8 or +12 bytes
of `.ARM.extab` (never the +4 needed to restore 156,312), so no
extab-size-exact form with raw < 1,173 was found in this pass.

## 3. Rejected cells (this session)

* `EmpGuard` around `getCurrentCampaignMission` (baseline) — 1,173.
* `EmpGuard` around `getLevel` / `gameWon` / `hardCoreMode`: 1,198 / 1,201
  / 2,463.
* `EmpGuard` around `getShipGroup`, `setBV`, `field_0x40`, `geometry`,
  `addObject`, `setLevel`, `ArraySetLength(2/3/5/11)`,
  `setWreckedMeshId(0x477c/0x477d/0x477e/0x477f/0x4780)`, the cargo
  deletes, `new Player`, `ship = fighter`: all >= 1,174 or don't move
  the LSDA.  (`getShipGroup`/`setLevel` on the ship-guard-dropped form
  give 1,174 with a 156,312 extab, i.e. 1 raw byte worse than the base.)
* `int t = type;` / `int r = race;` / `bool isType14` locals: neutral
  (1,173).
* `volumes` created before `fixed`: 2,345.
* regular `new PlayerFighter(...)` / `new PlayerFixedObject(...)` instead
  of `::operator new` + placement new: 2,279-2,416 (size 2,764-2,776).
* faithful spawn forms (`wpz+jitter2`, `wpy+jitter1`): 1,178; all-int
  locals: size 2,752/2,760 (not exact).
* tail `setBV` first (baseline): 1,180 (w13508 handoff measurement).

## 4. Gate

`cmake --build cmake-build-match --target check -j2` -> **RATCHET PASS**:
byte_exact **4507/4524**, linked_exact **4507/4524**, size-exact
4524/4524, data_exact 843/843, imports 207, lint CLEAN, drift 0,
parity 5370/5370.  `tools/verify_baseline.json` untouched.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13529
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb
# -> raw=1137 aligned=489 size=2764/2764 insn=1115/1117
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

## 6. Files changed

* `src/game/world/Level.cpp` — drop the top `EmpGuard` around
  `getCurrentCampaignMission`.
* `docs/IDENTICALITY.md` — wave-191 numbers.
* `docs/VALIDATION.md` — `.ARM.extab` record 336 -> 332 B / 57 -> 56 rows.
* this note.
