# Globals::Globals C1/C2 — wave 116 (w6408)

Status: analysis only, **no source change landed**. Worktree `wt/w6408`,
branch `wave116-w6408`, based on master `d672d349f`. Target
`_ZN7GlobalsC1Ev` / `_ZN7GlobalsC2Ev` (`src/game/core/Globals.cpp`), 612 B.

## TL;DR

The wave-115 80.5 % shape is **pool-set-faithful (31/31 words) but not
pool-order-faithful**: `status` occupies slot 6 (original slot 26), and
`movw r6,#583` is at normalised index **139** (original 34). The wave-114
shape (recordHandler last, status late, quality block after `lcmf`/`lcmfc`)
is **pool-order-identical (31/31)** and has `movw #583` at index **34**, but
scores **78.8 %** (first diff at index 96). No source-order variant found in
this pass raises the fuzzy score above 80.5.

Measured both:

| shape | fuzzy | pool order | movw@ | first diff | size |
|---|---:|---|---:|---:|---:|
| **wave-115 (committed)** | **80.5** | 31/31 set, order differs at slot 6 | 139 | 34 | 612 |
| wave-114 (pool-faithful) | 78.8 | **31/31 identical** | **34** | 96 | 612 |
| original | 100 | 31/31 | 34 | – | 612 |

## What was checked

The wave-115 source is unchanged (gate green). All measurements below are
from a full `Globals.cpp` compile + gold link + the official `verify.py`
scorer (`/dev/shm/w6408/score.py` reproduces the same fuzzy number).

### Pool order

Wave-115 pool (first 8): `Canvas, lastSpace, lastStation, lcmf, lcmfc,
options, status, recordHandler, ...`; original: `Canvas, lastSpace,
lastStation, lcmf, lcmfc, options, recordHandler, shipTemplate, ...`.
The only out-of-order word is `status` (slot 6 vs 26). Moving `status` back
to the late block restores the exact 31-word order but costs 5.7 fuzzy points
(77.4 best; wave-114's 78.8 needs `recordHandler` last as well).

### `movw r6,#583`

The hoist is a post-RA scheduler decision. The wave-114 late order
(`... lastStation, lcmf, lcmfc, QUALITY, items, ...`) hoists it to index 34.
Any shape that keeps the wave-115 early block (`recordHandler` 2nd,
`status` early) and moves the quality block late loses the tail alignment:
best such shape measured **74.8 %** (`movw@34`, first diff 69), versus
**78.8 %** for the full wave-114 shape.

### Searches (all full compile + gold link)

| search | best fuzzy | notes |
|---|---:|---|
| 200 random early-block permutations (805 late fixed) | 80.5 | no gain |
| 300 random settings-tail permutations | 80.5 | no gain |
| 500 random whole-settings-block permutations | 80.5 | no gain |
| 500 random late-block permutations, quality pos 10–18 | 80.5 | no gain; best was still the baseline |
| quality-block position sweep | 80.5 (pos 0–5) / 79.2 (pos 10–12) / 77.9 (13) / 73.0 (14) / 71.7 (15) / 72.1 (16) / 72.6 (17); `movw@34` from pos 14 | trade-off |
| `status` position sweep (early + late) | 80.5 (last in early block) | any move ≤80.1 |
| original settings tail `E F H A B C D G` | 66.4–79.2 | RA swaps r4/r5 at index 9 |
| hints group at tail positions 2–6 | 77.4–78.8 | front position 66.4 |
| `imageFactory` address kept in a local slot | 80.5 | compiler rematerialises it |
| quality block as ternary / `else if` | 72.1 | worse |
| quality block with `unsigned short` temps | 80.5 | byte-identical |
| `settings->tintR` moved into settings block | 80.5 | byte-identical |
| `Globals::hints` direct vs `secondary` pointer | 80.5 | byte-identical |

### Why the trade-off exists

The original keeps `v54 = 0x247` in `r6` from index 34 to the `resWidth`
store at 139, and keeps `&imageFactory` in `ip` from index 128 to the store
at 181. The wave-115 shape rematerialises both constants/addresses; the
wave-114 shape keeps `v54` early but rematerialises `imageFactory`. The
remaining differences are post-RA scheduling/register-allocation artefacts
driven by whole-function live ranges, not by statement order: changing the
settings tail by one store can swap `r4`/`r5` at index 9 and move the first
diff from 96 to 9.

## Dead ends recorded

- `status` late (restores pool order) always costs ≥3 fuzzy points.
- Moving the quality block late (restores `movw@34`) costs ≥5.7 points with
  the 805 early block.
- The original settings store order `E F H A B C D G` cannot be reproduced
  while keeping the wave-114/115 early register assignment.
- `imageFactory` address-lifetime forms (`void**` slot, reference, direct
  pointer) are optimised to the same rematerialised sequence.
- No namespace-scope globals, no new TUs, no source change.

## Gate

`cmake --build cmake-build-match --target check -j6` (unchanged source):

```text
verify: byte 4501  linked 4501  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`drift`: 0 fields. `lint`: CLEAN. 27 section headers, file size
2,187,328 B, `.ARM.extab` 0x26298 (unchanged).
