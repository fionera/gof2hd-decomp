# `MGame::OnTouchEnd` — wave 176 (w12013) branch-layout pass

Target: `_ZN5MGame10OnTouchEndEiiPv` in `src/game/menu/MGame.cpp`
(original 8,080 B @ `0x17a144`). Branch-layout / shared-tail / local-hoist
pass only (expects and aliases belong to w12012).

## Result

| metric | base (`b21088985`) | after |
|---|---|---|
| raw same-address diff bytes | **3,771** | **3,770** |
| aligned (instruction) diff | 1,453 | **1,521** |
| function size | 8,080 / 8,080 | 8,080 / 8,080 |
| last `popeq` (ours) | `0x17ba32` | `0x17ba32` |
| `.rel.plt` diff | 0 | 0 |

Gate after the change (unchanged from the wave-175 base):

```
byte_exact 4507/4524   linked_exact 4507/4524   size-exact 4524/4524
stub_zero_size 0  missing 0  wrong_type 0  extra 0
data_exact 843/843     imports 207
lint CLEAN ✓           drift 0 field(s)         parity 5370/5370
```

`.text` differing bytes **80,166 -> 80,165**; `.ARM.extab` 746 (size/base
exact); `.rodata`/`.data`/`.bss`/`.rel.plt` byte-identical;
`.note.gnu.build-id` 20. File size exact (2,187,328 B).

This is a **raw-byte win, not a structural one**: the reorder fixes three
same-address bytes and breaks two, and it makes the instruction stream less
aligned (+68). It is landed because the task's acceptance rule is
"size-exact, `.rel.plt`-identical raw wins".

## The change (`dacb`)

In the `if (weaponSlot >= 0)` arm, the tail statement order was changed from
the source order `A B C D` to `D A C B`:

```cpp
// before
this->hudMenuOpen = 0;      // A
this->pauseOpen = 0;        // B
resumeSounds();             // C
this->hud->closeHudMenu();  // D
} else if (wingmanCommand >= 0) {

// after
this->hud->closeHudMenu();  // D
this->hudMenuOpen = 0;      // A
resumeSounds();             // C
this->pauseOpen = 0;        // B
} else if (wingmanCommand >= 0) {
```

Byte-level effect vs base in `0x17b9e0..0x17ba30`:

* fixed: `0x17b9ef`, `0x17b9f1`, `0x17b9f3` (3 bytes now match the original)
* broken: `0x17b9fa`, `0x17b9fb` (2 bytes that base matched now differ)
* net **-1** raw byte.

The original arm is `movs r0,#0; strb hudMenuOpen; strb pauseOpen; mov r0,fp;
blx resumeSounds; ldr hud; blx closeHudMenu` (i.e. the `ABCD` order), but the
whole arm sits ~30 B later in the original than in ours, so same-address bytes
and instruction order disagree. `dacb` happens to place the `movs`/`strb` byte
pair at the address the original uses.

## Permutation sweep (all 24 orders of A/B/C/D)

Every order was built and measured. `ABCD` is the only order with aligned
1,453; every other order gives aligned 1,521. Raw minimum is 3,770, reached by
four orders:

```
ABCD 3771/1453   ABDC 3772/1521   ACBD 3774/1521   ACDB 3774/1521
ADBC 3772/1521   ADCB 3772/1521   BACD 3771/1521   BADC 3772/1521
BCAD 3774/1521   BCDA 3774/1521   BDAC 3772/1521   BDCA 3772/1521
CABD 3774/1521   CADB 3774/1521   CBAD 3774/1521   CBDA 3774/1521
CDAB 3774/1521   CDBA 3774/1521   DABC 3771/1521   DACB 3770/1521
DBAC 3771/1521   DBCA 3770/1521   DCAB 3770/1521   DCBA 3770/1521
```

So the raw/aligned trade is inherent to this arm: no order gives both
3,770 raw and 1,453 aligned.

## Other sweeps (all size 8,080, `.rel.plt` 0 unless noted)

| experiment | raw | aligned | note |
|---|---|---|---|
| base | 3,771 | 1,453 | |
| `_bc8` block tail store order (6 perms, resume/close both ways) | 3,771 min | 1,453 | `ABC RS` = base is best |
| `hudTouchFlags == 0` arm tail orders (9 candidates) | 3,771 min | 1,453 | base is best |
| final block as nested `if (isAutoPilot()) { if (!wasAutoPilot) }` | 3,771 | 1,453 | identical codegen |
| final block via `bool ap = isAutoPilot(); if (ap && !wasAutoPilot)` | 3,771 | 1,453 | identical codegen |
| final block as `if (!wasAutoPilot) { if (isAutoPilot()) }` | 4,375 | 1,486 | size 8,076 |
| `!wasAutoPilot && isAutoPilot()` | 4,375 | — | size 8,076 |
| wingman arm body permutations | base best | | |
| `htf_swap` (pauseOpen/resumeSounds swap) | 3,773 | | |
| `dacb + htf_swap` | 3,772 | | |
| `count > 0` | 3,773 | | |
| `count != 0` | 6,636 | | |
| dispatcher arm reorder `[2,1,0]` (0x400 first) | 3,774 | 1,456 | |
| `case 0` first in cameraMode switch | 3,764 | | **`.rel.plt` 5 — fatal** |
| `menuId` single shared call site | 4,705 | | size 8,076 |
| hoisting `Hud*` / `Level*` | 4,750 | | size 8,076 |
| campaign chain in original order (`orig_split`) | 4,673 | **892** | **size 8,092 (+12) — fatal** |
| `bool won` + `Mission::hasWon()` + campaign-first (`C1`) | 3,907 | 1,352 | size 8,092, epi `0x17b014` |
| `bool won` alone | — | | size 8,076, epi `0x17aff8` |
| `C1` alone | — | | size 8,084, epi `0x17af92` |
| `orig_split + C1 + bool` | — | **804** | size 8,092 |
| `orig_split + C1 + htf_swap + bool` | 4,014 | 802 | size 8,092 |

`orig_split` is the most structurally faithful reconstruction found (2,865/2,865
instructions match, aligned 892), but its +12 B is pure instruction-width
divergence (`b.n` vs `b.w`, `add` vs `add.w`) and shifts every later function,
dropping `byte_exact` 4507 -> 4487. It is kept here as the target for a future
size-neutral pass.

## Structural finding: hot/cold split is mirrored

The fundamental residual is not any single statement order; it is where the
compiler puts the epilogue.

* Original: epilogue `popeq` at `0x17b014`; the `hudTouchFlags == 0` arm and the
  whole campaign-mission chain are cold code after it. Post-epilogue region
  `0x17b016..0x17c0d4` = **1,552 instructions / 4,286 B**.
* Ours: epilogue `popeq` at `0x17ba32`; the final `isAutoPilot` block and the
  campaign chain follow it. Post-epilogue region `0x17ba34..0x17c0d4` =
  **641 instructions / 1,696 B**.

So ours keeps ~2,590 B more code in the hot path (before the epilogue) and
~2,590 B less in the cold region. The original places the final `isAutoPilot`
block hot (`0x17afe8`, just before the epilogue) and the `hudTouchFlags == 0`
arm cold (`0x17b01a`, after the epilogue); ours does the opposite. This is a
block-placement decision, not a source-order decision: reordering the
dispatcher arms in source did not change which arm owns the shared
`initHudMenu` `blx`.

MIR block IDs on the base build: `bb.356` is the shared `blx` owned by the
`0x200` arm, `bb.358` the `hudTouchFlags` test, `bb.371` the final block, and
the epilogue is last.

## Reproduction

```bash
cd /home/debian/GalaxyOnFire2/wt/w12013
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 /home/debian/GalaxyOnFire2/scratch/w12013/measure.py
cmake --build cmake-build-match --target check -j2
cmake --build cmake-build-match --target diff -j2
cmake --build cmake-build-match --target lint -j2
cmake --build cmake-build-match --target drift -j2
```

## Scratch helpers (all under `scratch/w12013/`)

`MGame.base.cpp`, `MGame.dacb.cpp`, `MGame.orig_split.cpp`,
`MGame.orig_comb.cpp`, `perm4.py`, `sweep_bc8.py`, `sweep_htf0.py`,
`sweep_final.py`, `batch.py`, `runfile.py`, `runbool.py`, `dumpvar.py`,
`measure.py`, `textdiff.py`, `bytedump.py`, `align.py`, `cmpseq.py`,
`cumdiff.py`, `clusters*.txt`, `orig.dis`, `ours.dis`.

Note: `batch.py`, `runfile.py`, `runbool.py`, `dumpvar.py` restore
`MGame.base.cpp` at the end; re-apply `dacb` after using them.
