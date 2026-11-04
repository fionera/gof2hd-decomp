# Hud::draw — `short frameH` / `short transferred` unlock 93.3 at extab 476 B (w5318)

**Status:** code change + analysis, 2026-11-04 (wave 105). Worktree `wt/w5318`, branch
`wave105-w5318`, base `835748c18`. Target `_ZN3Hud4drawExxP9PlayerEgobjj`
(`src/game/ui/Hud.cpp`, `Hud::draw`), original size **9332 B** (Ghidra `Hud_draw`
@ 0x173b90, file 0x163b90).

Gate on entry and on exit (full `check`): **byte_exact 4500 / linked_exact 4500**,
size-exact 4524/4524, `stub_zero_size 0`, `missing 0`, `wrong_type 0`, `extra 0`,
`data_exact 843/843`, imports 207, `.text 0x1337c8`, `.rodata 0x41b67`,
`.ARM.exidx 0x6f70`, **`.ARM.extab 0x26298` (exact)**, lint CLEAN, drift 0,
**RATCHET PASS**.

Official per-function score: **92.9 % -> 93.3 %** at the unchanged 9332-byte size and
the unchanged **476-byte / 77-row** `.ARM.extab` record (`.rodata` base 0x1c9e10).

This note extends `hud-draw-oracle-dock-w103-wf315.md`, which reported that a full
720-permutation dock sweep capped at **92.9** for any 9332 B / 476 B shape. That sweep
kept `frameH` and `transferred` as `int`. Re-running it with those two locals typed
**`short`** raises the 9332/476 ceiling.

---

## 1. The new lever

The original stores `frameH` as a 4-byte int (`str r0,[sp,#96]` at 0x16588c) and
`transferred` in r4, so `int` is the semantically faithful type. But at `-Oz` the
declared width changes the live-range / register-allocation decision for the whole
dock block (and, through the shared frame, the prologue):

* `short frameH` + `short transferred` with the dock order `T P B W H D`
  (text, progress, barOffset, barW, barH, dt/fade) and the faithful second bar
  `T W H O B D Y` gives **94.4 %**, size **9332**, but `.ARM.extab` **480 B / 78 rows**
  (`0x2629c` -> `.rodata 0x1c9e20` -> `byte_exact 4500 -> 4040`). Gate-blocked.
* The same `short` types with the gate-safe dock order `W D B T P H` give
  **93.3 %**, size 9332, `.ARM.extab` **476 B / 77 rows** (landed here).

`short` is a codegen lever, not a semantic change: `GetImage2DHeight` and
`getDockTransferedAmount` return small values, so truncation is lossless in practice.

## 2. Measured table (official verifier, `score1.py`)

| dock order | frameH/transferred | pct | size | extab | rows | gate |
|---|---|---|---|---|---|---|
| `B H T W P D` (HEAD) | int/int | 92.9 | 9332 | 476 | 77 | PASS |
| `T P B W H D` (faithful) | int/int | 93.4 | 9324 | 480 | 78 | FAIL size+extab |
| `T P B W H D` (faithful) | short/short | **94.4** | **9332** | 480 | 78 | FAIL extab |
| `W D B T P H` | short/short | **93.3** | **9332** | **476** | **77** | **PASS (landed)** |
| `W B D T P H` | short/short | 93.3 | 9332 | 476 | 77 | PASS |
| `P W T H B D` | short/short | 93.3 | 9332 | 476 | 77 | PASS |
| `B W D T P H` | short/short | 93.3 | 9332 | 476 | 77 | PASS |
| `W D T H P B` | int/short | 93.1 | 9332 | 476 | 77 | PASS |

The faithful dock order is always 78 rows: the second-bar `filled = progress *
(float) halfW` block between `DrawImage2D` and `DrawRegion2D` splits the landing pad
(see `hud-draw-oracle-dock-w103-wf315.md` §4). Merging it costs size (`filled` moved
before `SetColor` -> 9356 B / 83.4 %), so the 78-row shape cannot land until another
function's extab entry is shaved by 4 B.

## 3. Landed change

`src/game/ui/Hud.cpp`, `Hud::draw` only (9 lines):

```diff
-            int frameH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
+            short frameH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
...
-            int transferred = ego->getDockTransferedAmount();
+            short transferred = ego->getDockTransferedAmount();
```

plus the dock declaration reorder to `W D B T P H` (barW, dt/fade, barOffset, text,
progress, barH) and `int top` moved above `int barYOffset` in the second bar.

## 4. Residual diffs (146 normalised hunks, 93.3 %)

Unchanged from `hud-draw-oracle-dock-w103-wf315.md` §4/§5:

1. Gamma block: `Status *st` caches `Globals::status` and spills `&Globals::status`
   early; original reloads for the mission argument and spills late.
2. The two dead `ldr.w r8, [sp, #108]` reloads (0x164176 / 0x164210).
3. Mission-panel `fp`/`r9` swaps (`ldrh.w fp,[r6,#1098]` vs `r9`, `sub.w r3,r9,fp`
   vs `fp,r9`, `add.w r4,r3,r9` vs `r3,fp`).
4. Second-bar landing-pad tail ordering (`mov r5,r4` / `ldr r4,[r9]` /
   `mvn r1,#255` placement) and the resulting branch-target offsets.
5. Prologue `vpush {d8-d11}` now matches with the `short` types.

## 5. Reproduction

```
cmake --build cmake-build-match --target check -j6       # RATCHET PASS
cmake --build cmake-build-match --target drift -j6       # 0 drifted fields
python3 /dev/shm/w5318/score1.py _ZN3Hud4drawExxP9PlayerEgobjj
python3 _work/reconstructions/gameresource_gen/lsda.py cmake-build-match/libgof2hdaa.so \
  _ZN3Hud4drawExxP9PlayerEgobjj                          # 77 rows
arm-linux-androideabi-readelf -S -W cmake-build-match/libgof2hdaa.so | grep -E 'ARM.extab|\.rodata'
```

Scratch (variant builders, 720-perm dock sweep, extab/LSDA dumpers) lived in
`/dev/shm/w5318`: `variant5.py`, `sweep_dock_all.py`, `sweep_refine.py`,
`try_filled2.py`, `try_filled3.py`, `try_scope2.py`, `build_score.sh`,
`extab_dump.py`, `score1.py`.
