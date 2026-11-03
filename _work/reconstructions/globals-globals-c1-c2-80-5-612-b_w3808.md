# Globals::Globals C1/C2 (80.5%, 612 B) — w3808

Status: research note, 2026-11-03. Worktree `wt/w3808`, branch `wave88-w3808`,
base `41804126`. No source change landed: the current constructor source order is
a local optimum. This commit contains only this note.

## TL;DR

* Baseline and final: **`_ZN7GlobalsC1Ev` / `_ZN7GlobalsC2Ev` = 80.5%, 612 B**
  (size-exact; 186 code instructions in both). Gate unchanged:
  **byte_exact 4498 / linked_exact 4498 / size-exact 4524**, `.text 0x1337c8`,
  lint CLEAN, drift 0, `RATCHET PASS`.
* The original and ours are identical for the first 34 instructions. The first
  structural difference is the original's early `movw r6, #583` (the default
  `resWidth = 0x247`) versus ours materializing `movw r0, #583` late in the
  quality block. The rest is register allocation / store scheduling.
* The original's **7 prologue GOT loads** are
  `Canvas, lastSpaceMusicPlayed, lastStationMusicPlayed,
  lastCampaignMissionFailed, lastCampaignMissionFailCount, options,
  recordHandler`. Ours is identical except the 7th load is `status`; ours loads
  `recordHandler` after `movs r2,#0`.
* The original's literal-pool order is
  `Canvas, lastSpace, lastStation, lastCampaignFailed, lastCampaignFailCount,
  options, recordHandler, shipTemplate, gameText, font, fontAlien,
  fontLangSelect, rnd, globals, layout, appManager, generator, hints,
  gameLoaded, gameSaving, initMemoryWarning, imageFactory, achievements,
  galaxy, instantActionScore, instantActionWave, status, instantActionType,
  instantActionPlayerName, items, ships`.
  Ours is the same except `status` is at index 10 (right after `options`) and
  everything after it shifts by one.
* Moving `status` late to match the pool order is **mutually exclusive** with
  the `movw r6,#583` hoist: the scheduler changes the prologue GOT hoisting and
  the fuzzy drops to 77.0% (or worse). The baseline order keeps the prologue
  match and is the best measured score.

## 1. Baseline

```text
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Target rows from `cmake-build-match/verify/report.json`:

```text
_ZN7GlobalsC1Ev  pct 80.5  osize 612  usize 612
_ZN7GlobalsC2Ev  pct 80.5  osize 612  usize 612
```

## 2. Instruction-level diff (code region only)

Both functions have 186 code instructions before the literal pool. The first
34 instructions are identical. The first difference:

```text
orig e3236: f240 2647  movw r6, #583      ; 0x247 default resWidth
ours e3236: 6002       str  r2, [r0, #0]  ; shipTemplate = 0
```

The original keeps `583` in callee-saved `r6` across the whole settings block
and uses it as the `resWidth` default at `e334a`; ours rematerializes it into
`r0` at `e3332`. The original's `resHeight` default `0x201` is likewise
materialized late but into `r0` (original `e3328`, ours `e3350`), so only the
`resWidth` constant is hoisted.

Other code-region differences (all after the first 34 instructions):

* settings store order: original stores `flagAt3f` (`#63`) before `intAt3b`
  (`#59`) and stores `intAt48` (`#72`) immediately before `secondary->slot0`;
  ours stores `intAt3b` first and sinks `intAt48` into the late block;
* `lastSpaceMusicPlayed` store position (`e32f8` orig vs `e32e4` ours);
* the late zero-init block's GOT-load/store schedule and register assignment
  (original uses `r6` for `galaxy`, `r3` for `instantActionType`, etc.; ours
  uses `r3`/`r0` differently);
* the final store order (`shortAt60`, `ships`, `soundResources`).

The settings-block and late-block differences are consequences of the same
register allocator decision; reordering the source to the original's *machine*
store order makes the scheduler diverge more, not less.

## 3. Measured source-order variants

All variants were built with the normal `check`/`verify` pipeline. `pct` is
the official `verify.py` fuzzy for `_ZN7GlobalsC1Ev` (C2 is identical).

| variant | pct | byte_exact |
|---|---:|---:|
| **baseline** | **80.5** | **4498** |
| `status` after `instantActionWave` | 77.0 | 4491 |
| `status` after `items` | 77.0 | 4491 |
| `status` after `instantActionType` | 77.0 | 4491 |
| `status` after `field_0x0` | 76.1 | 4491 |
| `status` at very end (after `lastCampaignMissionFailCount`) | 56.2 | 4491 |
| `status` after `soundResources` | 55.3 | 4491 |
| `status` at early-block positions 0..11 | 79.2–80.1 | 4498 |
| `Canvas` first / `recordHandler` after `appManager` | 79.2 | 4498 |
| all 6 permutations of `shipTemplate`/`recordHandler`/`Canvas` | 79.6–80.5 | 4498 |
| `lastSpace`/`lastStation`/`lastCampaign*` moved early | 64.5–75.2 | 4491–4498 |
| settings block reordered to original machine order | 79.2 | 4498 |
| `intAt48` moved next to `secondary->slot0` | 80.5 | 4498 |
| `instantActionWave` before `instantActionType` | 80.1 | 4498 |
| `instantActionWave` after `ships` | 80.5 | 4498 |
| `instantActionWave` after `lastCampaignMissionFailed` | 79.6 | 4491 |
| quality block moved early | 49.1–52.3 | 4491–4498 |
| ternary / helper-function form of the quality block | 72.1 | 4498 |
| packed typed `GameSettingsRecord` (pre-w3659 form) | 80.5 | 4498 |

The baseline source order is the best of all measured variants. In particular,
the `status` position is a genuine local optimum: every move away from
"after `generator`" loses at least 0.4 points, and moving it into the late
block loses 3.5 points.

## 4. Original `instantAction` defaults

The original's late block initializes, in literal-pool order:

```text
instantActionScore = 0
instantActionWave = 0
status = 0
instantActionType = -1
instantActionPlayerName = 0
items = 0
ships = 0
```

The current source has `instantActionType = -1` before `instantActionWave = 0`
and `status` in the early block. Swapping `instantActionWave` before
`instantActionType` gives 80.1% (a small loss), not an improvement. The
machine store order is further scrambled by the scheduler (`status` is stored
after `items`, `ships` last), so the literal-pool order is the better witness
of source order here.

## 5. Root cause

The remaining gap is **register allocation / instruction scheduling**, not a
missing statement or wrong constant:

1. The original's `movw r6,#583` hoist keeps the `resWidth` default in a
   callee-saved register across the settings block.
2. That hoist is coupled to the `status` store being late (the original's
   `status` GOT load is at `e3374`, pool index 30). Moving `status` late in
   source changes the scheduler's prologue GOT hoisting, breaking the
   first-34-instruction match and losing more than the hoist gains.
3. The two choices are mutually exclusive at this source shape: the best
   compromise is the current baseline (80.5%), which matches the original's
   prologue except that the 7th GOT load is `status` instead of
   `recordHandler`.

No source-only reordering tested improves the score without a gate
regression. A future pass would need a source construct that changes the
allocator's live range for the `v54` default without moving the `status`
store (e.g. a different IR shape for the quality block), or a targeted
per-TU `-mllvm` rematerialization knob.

## 6. Reproduction

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN7GlobalsC2Ev
```
