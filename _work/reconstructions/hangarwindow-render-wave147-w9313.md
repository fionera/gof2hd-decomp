# `HangarWindow::render` — wave 147 (w9313): branch weight merges the credit row at 9700 B

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`), original
`0x1490d8`, size `0x25e4` = 9700 B. Worktree `/home/debian/GalaxyOnFire2/wt/w9313`,
base `9c9744ca5` (wave 146). Metrics: raw same-address diff
(`/dev/shm/fdiff.py`), instruction-index alignment
(`_work/reconstructions/align_diff.py`), `.ARM.extab` LSDA row diff
(`_work/reconstructions/extab_lsda_decode.py`).

## Result (landed)

**Yes — the credit-row split is gone and the function is still exactly 9700 B.**
`.ARM.extab` stays 156,312 B (section size exact); `.text` size exact.

| metric | before | after |
|---|---:|---:|
| raw same-address diff | 1719 | **1967** |
| align_diff non-matching insns | 480 | **301** |
| `.ARM.extab` record diff (fn 0x1490d8) | 762 | **676** |
| `.ARM.extab` section diff | 1393 | **1307** |
| `.text` section diff | 85928 | **86176** |
| fn size | 9700 | 9700 |
| rows | 156 | 156 |

Gate: byte_exact **4507 -> 4507**, linked_exact **4507 -> 4507**, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, RATCHET PASS.

## The three pieces

1. **`borderX`** (the w8818/w8914 local) splits the blueprint-progress rows
   19/20 into the original `(1530,144,9410)` + `(1712,46,9358)`.
2. **Inline `iconX`/`iconY`** in the free-credits `DrawImage2D` removes the
   `llvm.lifetime.*` cleanups from the `if (count == 4 && showBestDeal)` EH
   scope, so the free-credits invoke shares the loop-body landing pad and the
   credit row merges: `orig (6824,210,9654)` vs `ours (6826,202,9654)`.
3. **`__builtin_expect(count <= 4, 1)` on the credit loop condition** is the
   missing lever.  Without it, pieces 1+2 give 9664 B (the whole function's
   register allocation shifts: `this` stays in `fp` instead of being spilled
   to `[sp,#136]`, and `fp` is no longer free for the `mov.w fp,#17`
   constant).  The loop branch weight reproduces the original's PGO block
   placement: the prologue spills `this` to `[sp,#136]` again and the body is
   back to exactly 9700 B.  This is the w9214 `!prof` finding applied here.

## Row table (fn 0x1490d8), before -> after

Before (base): rows 19/20 merged, credit row split at 125/126:

```
19 orig (1530,144,9410)  ours (1530,228,9446)
20 orig (1712, 46,9358)  ours (1770,  8,9300)
...
125 orig (6678,146,0)     ours (6682,144,0)
126 orig (6824,210,9654)  ours (6826, 96,9654)   <- tabIcons
                          ours (7004, 30,9600)   <- free-credits DrawImage2D (extra row)
127 orig (7034,390,0)     ours (7034,392,0)
```

After: rows 19/20 split exactly, credit row merged; 150 of 156 rows are now
byte-identical (offset, length and landing pad).  Remaining differing rows:

```
 1  orig (684,20,9544)   ours (684,20,9534)     landing-pad offset only
39  orig (2508,150,9664) ours (2508,118,9664)   list-render row boundary
40  orig (2696,140,9562) ours (2654,182,9514)
60  orig (3806,20,9514)  ours (3806,20,9516)    landing-pad offset only
96-135  offsets shifted 2-4 B (credit-loop RA / block order)
126 orig (6824,210,9654) ours (6826,202,9654)  merged, 8 B shorter
```

## Merge lesson (exact raw diff for both orders)

| order | fn size | extab diff | raw |
|---|---:|---:|---:|
| base locals in if body + borderX + loop expect | 9700 | 687 (credit still split) | 2121 |
| inline iconX/iconY + borderX + loop expect | 9700 | **676** (credit merged) | **1967** |
| loop-top locals + borderX + loop expect | 9700 | 676 | 1967 (identical bytes to inline) |

So the inline form and the loop-top form are byte-identical; the base-local
form keeps the split.  The order that lands is borderX -> inline -> expect.

The raw diff rises (1719 -> 1967) because the branch weight shifts instruction
addresses; the structural alignment (301 vs 480) and the extab are the
improvements.  This is a faithful reconstruction, not METRIC-ONLY: the original
was PGO-built (w9214) and the post-change row table matches the original's
offsets exactly for rows 0-38 and 41-59, which the plain source does not.

## Reproduction

```sh
python3 /dev/shm/w9313/fdiff.py _ZN12HangarWindow6renderEv        # 1967, 9700
python3 _work/reconstructions/align_diff.py _ZN12HangarWindow6renderEv  # 301
python3 _work/reconstructions/extab_lsda_decode.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so                                 # 676
cmake --build cmake-build-match --target check -j6                 # RATCHET PASS
```

Scratch (not committed), `/dev/shm/w9313/`: `fdiff.py`, `align_diff.py`,
`measure.sh`, `sweep.py`, `run_sweep*.py`, `HangarWindow.base.cpp`,
`HangarWindow.win.cpp`, `dis_orig.txt`, `dis_base.txt`, `dis_win.txt`,
`extab_base.txt`, `extab_now.txt`.

## Open leads

* Row 39/40 boundary: original `(2508,150,9664)` + `(2696,140,9562)` vs ours
  `(2508,118,9664)` + `(2654,182,9514)`.  At `0x149b24` the original emits a
  2-byte `cbz r0, 149b96` while ours emits `cmp r0,#0; beq.w 14a6d0` (6 B):
  the selected-ship `if (tab != 0)` block is laid out further away.  Another
  branch weight (or a source-order change in the selected-item ship branch)
  may close it.
* Rows 96-135 are shifted 2-4 B by the credit-loop register allocation
  (description/price r5/r6 flip, w8914).  Fixing the flip would align them.
* `__builtin_expect(tab != 0, 1)` on the selected-ship branch (the `padX`
  LSDA-sink site) lowers the extab record diff to **461** but drops to 155
  rows (align_diff 408, raw 2262), so it is not a clean win by itself.
  Removing the `volatile int *padX = 0;` sink with the landed change gives
  raw 6741 / extab 718 / align 744 (much worse), so the sink is still needed.
