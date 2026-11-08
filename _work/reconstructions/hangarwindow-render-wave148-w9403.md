# `HangarWindow::render` — wave 148 (w9403): ship-branch EH region + credit-loop expect probes

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`), original
`0x1490d8`, size `0x25e4` = 9700 B. Worktree `/home/debian/GalaxyOnFire2/wt/w9403`,
base `cbe3b5f7f` (wave 147). Metrics: raw same-address diff
(`/dev/shm/w9403/fdiff.py`, patched to this worktree), instruction-index
alignment (`/dev/shm/w9403/align_diff.py`, patched), `.ARM.extab` LSDA row diff
(`_work/reconstructions/extab_lsda_decode.py`).

## Result

**No source change landed.** Baseline re-measured and confirmed:
raw **1967**, align **301**, `.ARM.extab` record diff **676** (948 B, 156 rows,
43 differing rows), fn size 9700.  The tree is byte-identical to HEAD.

| variant | raw | align | extab | rows diff | fn size |
|---|---:|---:|---:|---:|---:|
| base (committed) | **1967** | **301** | **676** | **43** | 9700 |
| `expect(!(tab==3 && getShip()==ship),1)` | 1958 | 305 | 676 | 43 | 9700 |
| `expect(count==4 && showBestDeal,1)` | **1598** | 470 | 683 | 54 | 9700 |
| `expect(tab!=0,1)` | 2262 | 408 | 461 | 119 | 9700 |
| `expect(count==4 && showBestDeal,0)` | 8659 | 1259 | 764 | 156 | 9684 |
| `expect(listModeFlag,0)` | 8671 | 2729 | 0 | 0 | 9660 |

`bestdeal1` is the interesting one: it fixes LSDA rows **96-100 exactly** (the
first five credit-loop rows) and drops raw by 369 B, but the landing-pad block
moves 10 B (rows 36/39/41 LP 9664 -> 9654) so the row count rises 43 -> 54 and
align regresses 301 -> 470.  It is left as a candidate, not landed, because the
stated target metric is aligned 301.

## Row table (base, fn 0x1490d8) — the rows the mission targets

```
 39 orig (2508,150,9664) ours (2508,118,9664)   list-render row boundary
 40 orig (2696,140,9562) ours (2654,182,9514)   getShip call-site row
 96 orig (5918,198,0)    ours (5918,196,0)      credit loop
 97 orig (6116,8,9216)   ours (6114,8,9216)
 98 orig (6124,128,0)    ours (6122,132,0)
 99 orig (6252,10,9652)  ours (6254,10,9652)
101-135 shifted 2-4 B (credit-loop RA)
126 orig (6824,210,9654) ours (6826,202,9654)
```

## New findings

1. **Row 39/40 is an EH-region split, not a branch-shape problem.**
   The source `if (tab != 0) { if (!(tab == 3 && status->getShip() == ship)) ... }`
   is turned into `switch i32 %tab [0 -> skip, 3 -> getShip]` in the optimized
   IR.  `ListItem::isShip` unwinds to `%452` (real cleanup, LP 0x14b698) but
   `Status::getShip` unwinds to `%1765` (the switch landing pad, a
   `b 0x14b698` thunk).  The original puts **both** in row 39 (same LP), i.e.
   its `getShip` invoke shares `%452`.  At `-O0` *all* invokes unwind to one
   label, so the split is created by an `-Oz` pass, i.e. it is a PGO-layout
   consequence, not a source-scope difference.
   - Moving `if (upgradeMode && tab == 1)` *inside* `if (tab != 0)` is
     semantically identical and produces **byte-identical** code (no lever).
   - `if (tab != 0 && !(...))`, `if (tab != 3 || getShip() != ship)`,
     `bool sameShip = ...; if (!sameShip)`, `if (tab == 0) {} else if (...)`
     all leave getShip on a distinct landing pad.
   - `__builtin_expect(!(tab==3 && getShip()==ship), 1)` moves getShip's LP to
     9562 (matching row 40's original LP) but does not move the row boundary.

2. **Rows 96-100 are the `showBestDeal` branch weight.**
   `__builtin_expect(count == 4 && Globals::showBestDeal, 1)` makes rows
   96/97/98/99/100 byte-identical to the original (198/8/128/10/6) — the first
   direct evidence that the original's profile treats the best-deal hint as
   hot.  It also shortens the credit-loop body by 2 B and moves the landing
   pad block, which is why the global row count rises.

3. **The r5/r6 credit-loop RA is unchanged by declaration order.**
   `col` before `row`, `int row, col;` assignments, `count/3` without the
   `(unsigned char)` cast, all produce byte-identical code to base.
   `Layout **layp = &Globals::layout` and dropping the `lay` local both give
   9704 B / raw 3311 (the `lay` local is load-bearing).

4. **`padX` must stay a `volatile int *`.**
   `volatile int`, `(void)padX`, `nullptr` and removal: only `nullptr` is
   byte-identical; `volatile int` gives 9708/7317, removal 9700/6741/744 align.

## Open leads

* Land `bestdeal1` only if the coordinator accepts raw 1598 at align 470; it is
  the only variant that reproduces credit-loop rows 96-100.
* The row 39/40 fix needs `getShip` to share `%452` with `isShip`.  Since the
  split is created by an `-Oz` EH-preparation pass from PGO block layout, the
  remaining lever is a branch weight that keeps the switch's landing pad
  identical to the outer one (not yet found).

## Reproduction

```sh
python3 /dev/shm/w9403/fdiff.py _ZN12HangarWindow6renderEv        # 1967, 9700
python3 /dev/shm/w9403/align_diff.py _ZN12HangarWindow6renderEv  # 301
python3 _work/reconstructions/extab_lsda_decode.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so                                 # 676, 43 rows
```

Scratch (not committed), `/dev/shm/w9403/`: `fdiff.py`, `align_diff.py`,
`aligndump.py`, `meas.sh`, `lpcheck.py`, `tryvariant.py`, `batch.py`,
`batch2.py`..`batch5.py`, `HangarWindow.base.cpp`.
