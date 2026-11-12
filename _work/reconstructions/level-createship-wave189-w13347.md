# `Level::createShip` — wave 189 (w13347): `addObject` geometry local, raw 1,202 -> 1,200

Worktree `wt/w13347`, branch `wave189-w13347`, base master `f258b29c0` (wave
189).  Target `_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
2,764 B, VMA `0xc0214`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13347`.

**Status: landed a size-exact, `.rel.plt`-identical raw win.**  Raw
**1,202 -> 1,200** (-2), aligned 484 unchanged, size **2,764/2,764** exact,
insns 1,116/1,117.  The `.ARM.extab` entry stays the faithful **336 B /
57-row / 315-byte** table (size- and base-exact 156,312 B @ `0x1a3b78`,
content diff 615), `.ARM.exidx` diff 0, `.rel.plt` byte-identical.
`.text` differing bytes **73,953 -> 73,951**, total **74,588 -> 74,586**.

## 0. What changed

In the capital-ship (`shipClass == 1`) arm the `addObject` argument is staged
through a local:

```cpp
        fixed->setShipGroup(Globals::globals->getShipGroup(type, race, false), type, false);
-       lodManager->addObject(fixed->geometry);
+       AEGeometry *g = fixed->geometry;
+       lodManager->addObject(g);
        fixed->field_0x40 = 1;
        fixed->setBV(volumes);
```

Semantically identical; it only changes the live-range/register choice for the
`fixed->geometry` load feeding `LODManager::addObject`, which is worth 2 raw
bytes at `-Oz`.  No EH scope changes: the record keeps 57 rows / 315 table
bytes, so `.ARM.extab` and `.ARM.exidx` are untouched.

## 1. Why this shape (EXTAB-restore framing)

The prior notes (w13303, w13304, w13325) established the budget: a 336 B
`.ARM.extab` record needs a call-site table of **312..315** bytes; the only
raw-win candidate (`iff_rmM`, raw 1,177) is **310** bytes (332 B record), which
shrinks the section to 156,308 and shifts `.ARM.exidx` (net loss).  Every guard
that restores the 5 missing bytes costs >= 36 raw.  So the landable win must
come from a source change that leaves the 57-row / 315-byte table intact while
lowering raw.

Sweeping the no-mission body (`mi0co0em1sg1`, raw 1,166, 56 rows / 310) found
`AEGeometry *g = fixed->geometry; lodManager->addObject(g);` = **1,164**
(-2 vs the same body).  Re-applying it to the landed base (mission guard
present, 57 rows / 315) gives **1,200** with the table unchanged.

## 2. Measurements (all size 2,764 unless noted)

| variant | raw | aligned | rows / table |
|---|---:|---:|---|
| base | 1,202 | 484 | 57 / 315 |
| **base + `addObject` geometry local (landed)** | **1,200** | **484** | **57 / 315** |
| no-mission body (`mi0co0em1sg1`) | 1,166 | 460 | 56 / 310 |
| no-mission + geometry local | 1,164 | 460 | 56 / 310 |
| no-mission + `empPoints` `>20 ? 20 :` flip | 1,173 | 458 | 56 / 310 |
| no-mission + `jit312` | 1,166 | 370 | 56 / 310 |
| no-mission + `switch` 0x33/0x31/0x2c order | 1,189 | 467 | 56 / 310 |
| no-mission + `missionPtr` no-expect / `&&`-folded expect | 1,170 | 451 | 56 / 310 |
| no-mission + `field_0x40` before `addObject` | 1,170 | 460 | 56 / 310 |
| no-mission + `setBV` before `addObject` | 1,170 | 459 | 56 / 310 |
| no-mission + `ship` early / `setLevel` in arm | 2,373 (size 2,760) | 487 | 56 / 310 |
| no-mission + `mission` range / bitmask / switch chain | 1,166 / 1,193 / 1,166 | — | 56 / 310 |

Other one-at-a-time knobs on the no-mission body that were neutral (1,166):
`zero` local, `armor` if-form, `mission` switch/range chain, `return
reinterpret_cast`, `setLevel(this)` marker.  `empTime` ternary is size 2,768.

## 3. Gate

`cmake --build cmake-build-match --target check -j2` -> **RATCHET PASS**:
byte_exact **4507/4524**, linked_exact **4507/4524**, size-exact 4524/4524,
data_exact 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
`tools/verify_baseline.json` untouched (no count changed).

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13347
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb
# -> raw=1200 aligned=484 size=2764/2764 insn=1116/1117
python3 /home/debian/GalaxyOnFire2/scratch/w13347/secdiff.py \
  cmake-build-match/libgof2hdaa.so
# -> .ARM.extab 615 (156,312 @ 0x1a3b78), build-id 20, .text 73,951, total 74,586
```

Scratch: `sw1.py` (one-at-a-time sweep on the no-mission body), `sw2.py`
(landed variant), `Level_base.cpp`, `secdiff.py`, `rows.py`, `measure.sh`.

## 5. Files changed

* `src/game/world/Level.cpp` — stage `fixed->geometry` in a local before
  `lodManager->addObject`.
* `docs/IDENTICALITY.md` — wave-189 table row and section deltas.
* `docs/VALIDATION.md` — `.text` differing-byte count.
* this note.
