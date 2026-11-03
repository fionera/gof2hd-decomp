# Globals::Globals C1/C2 (80.5%, 612 B) — w3827

Status: research note, 2026-11-03. Worktree `wt/w3827`, branch `wave89-w3827`,
base `5710f6d0`. No source change landed: the w3808 baseline source order is
still the best measured shape. This commit contains only this note.

## TL;DR

* Baseline and final: **`_ZN7GlobalsC1Ev` / `_ZN7GlobalsC2Ev` = 80.5%, 612 B**
  (size-exact; 186 code instructions). Gate unchanged: **byte_exact 4498 /
  linked_exact 4498 / size-exact 4522**, `.text 0x1337c8`, lint CLEAN, drift 0,
  `RATCHET PASS`.
* The remaining delta is **SelectionDAG scheduling of the two `select` chains
  and of the prologue GOT-load hoist**, not a missing statement or wrong
  constant. The source order is a genuine local optimum.
* New this pass: identified the exact mechanism of the prologue hoist (the
  `list-hybrid` SelectionDAG scheduler hoists exactly the first 7
  `tLDRLIT_ga_pcrel` GOT address computations) and measured ~30 more source
  shapes (table below). None beats 80.5.

## 1. Baseline

```text
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.91
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

```text
_ZN7GlobalsC1Ev  pct 80.5  osize 612  usize 612
_ZN7GlobalsC2Ev  pct 80.5  osize 612  usize 612
```

## 2. What the original does (objdump of the shipped .so)

The original's constructor is one straight-line basic block. Its literal pool
(base `e33c8`, 31 GOT slots) is emitted in **order of first GOT load in the
final machine code**, so the pool order is a direct witness of the scheduled
load order:

```text
Canvas, lastSpace, lastStation, lastCampaignFailed, lastCampaignFailCount,
options, recordHandler, shipTemplate, gameText, font, fontAlien,
fontLangSelect, rnd, globals, layout, appManager, generator, hints,
gameLoaded, gameSaving, initMemoryWarning, imageFactory, achievements,
galaxy, instantActionScore, instantActionWave, status, instantActionType,
instantActionPlayerName, items, ships
```

The first 7 are the prologue-hoisted GOT loads (`e31e8`..`e3202`). The 8th
(`shipTemplate`, `e3220`) follows immediately. The original's **8th machine
instruction after the prologue is `movw r6,#583`** (`e3236`), i.e. the
`resWidth` default is hoisted to the very top of the store sequence and kept
live in callee-saved `r6` until `str r6,[r1,#84]` at `e334a`. The `resHeight`
default `513` is hoisted only to `e3328` (just before the first predicated
`movwge`), and lands in `r0`.

Ours is identical for the first 34 instructions; at `e3236` we have the
`shipTemplate` store instead, and our `movw r0,#583` is materialized at
`e3332` inside the quality block. The pool order is identical except `status`
is at slot 7 (right after `options`) and everything after it shifts by one.

## 3. Mechanism (new)

The hoist is done by the **SelectionDAG scheduler**, not the pre-RA machine
scheduler and not the register allocator:

* The default SelectionDAG scheduler for this target is `list-hybrid` — a
  rebuild with `-mllvm -pre-RA-sched=list-hybrid` is instruction-identical to
  the default. `source` / `list-ilp` / `list-burr` all differ.
* The "IR Dump After Peephole Optimizations" (i.e. immediately after ISel)
  already contains the 7 hoisted `tLDRLIT_ga_pcrel` + `t2LDRi12` pairs at the
  top of the block. The pre-RA `Machine Instruction Scheduler` then leaves the
  order unchanged; the greedy RA assigns `r0`; the post-RA top-down list
  scheduler only reorders locally.
* Consequently the only lever is the SelectionDAG shape. The 7 hoisted loads
  are exactly the first 7 GOT-address computations the `list-hybrid` scheduler
  pulls to the top; in the baseline they are
  `Canvas, lastSpace, lastStation, lastCampaignFailed, lastCampaignFailCount,
  options, status`. The original's set is the same with `recordHandler` in
  place of `status`.
* Moving `status` late in the source (after `instantActionWave`) does make the
  7th hoisted load `recordHandler`, but it simultaneously pushes `Canvas` out
  of the hoisted set (replaced by `generator`), so the prologue/pool still
  mismatches and the fuzzy drops to 77.0. The two effects are coupled through
  the same `list-hybrid` priority queue; no source permutation tried so far
  separates them.

## 4. Variant sweep (this pass)

All variants were built and scored with the official verifier
(`tools/verify/verify.py --show _ZN7GlobalsC1Ev`). `pct` is the fuzzy for C1;
C2 is identical.

| variant | pct | note |
|---|---:|---|
| **baseline** | **80.5** | |
| early default stores + conditional overrides | 35.6 | usize 604 |
| height select before width select | 79.6 | |
| if/else chain low→high | 46.8 | usize 620 |
| if/else chain high→low | 48.6 | usize 620 |
| `v54`/`v58` declared early, ifs late | 79.2 | |
| both assigned in each if, decls together | 79.2 | |
| `status` after quality block | 76.1 | |
| `status` after initMemoryWarning | 76.1 | |
| `status` after imageFactory | 76.1 | |
| `status` after achievements | 76.1 | |
| `status` after galaxy | 76.1 | |
| `status` after score | 76.5 | |
| `status` after wave / type / name / items / ships | 77.0 | |
| initMemoryWarning before quality + status after wave | 76.1 | |
| … + late-block reorder to original GOT order | 76.5 | |
| … + lastStation/lastCampaign* before quality | 72.1 | |
| `fv` load moved early, rest late | 53.2 | FP regalloc changes |
| compute early, store late | 52.3 | FP regalloc changes |
| `unsigned` v54/v58 | 80.5 | identical code |
| separate conditional stores (4 ifs) | 80.5 | identical code |
| `fv >= 1.0f` spelling | 80.5 | identical IR |
| no local `fv` (read member each time) | 79.2 | |
| low check before high check | 72.1 | |
| two scoped blocks, separate `fv` | 79.2 | |
| Canvas first + status late | 77.0 | |
| Canvas second + status late | 77.4 | |
| generator moved after quality + status late | 54.0 | |
| initMemoryWarning before quality | 79.2 | |
| lastStation before quality | 80.1 | |
| initMemoryWarning + lastStation before quality | 80.1 | |
| lastCampaignFailCount before quality | 77.0 | |
| lastCampaignFailCount+Failed before quality | 75.2 | |
| flagAt3f before intAt3b | 80.5 | identical code |
| intAt48 after shortAt4c | 80.5 | identical code |
| both of the above | 80.5 | identical code |
| intAt48 before slot0 | 80.5 | identical code |
| flagAt3f before intAt3b + intAt48 before slot0 | 79.2 | |

The settings-block store order (`flagAt3f` before `intAt3b`, `intAt48` before
`secondary->slot0`) is fully scheduler-determined: moving those statements in
the source (T1–T4 above) produces byte-identical machine code. Only moving
both at once changes the schedule, and it loses 1.3 points.

## 5. Original `instantAction` defaults

The original's late-block GOT load order is

```text
gameLoaded, gameSaving, initMemoryWarning, imageFactory, achievements,
galaxy, instantActionScore, instantActionWave, status, instantActionType,
instantActionPlayerName, items, ships
```

which is our source order except that `status` is late (between
`instantActionWave` and `instantActionType`). The stored values are
`score=0, wave=0, status=0, type=-1, name=0, items=0, ships=0`. Moving
`status` to that position costs 3.5 points because of the coupled prologue
hoist described in §3.

## 6. Root cause / next step

The remaining gap is a **SelectionDAG `list-hybrid` scheduling decision**:
the original keeps the `resWidth` default `583` in callee-saved `r6` across
the whole settings block (and correspondingly puts `recordHandler` rather than
`status` in the prologue GOT hoist), while clang 7.0.2 at `-Oz` rematerializes
`583` into `r0` inside the quality block for every source shape tried. The
source order is a local optimum at 80.5%.

A future pass would need a source construct that changes the `list-hybrid`
priority queue for the two `select` chains without moving the `status` store
(e.g. a different IR shape for the quality block that survives to
SelectionDAG), or binary evidence for a different original statement order in
the late zero-init block. The build-flag question is settled
(`build_flags_independent_audit_w3750.md`,
`worst_function_flag_sensitivity_w3751.md`); `-pre-RA-sched=source` moves the
`movw` from `0x152` to `0x10a` but the fuzzy stays 80.5 (the diff is merely
rearranged), so it is not a candidate.

## 7. Reproduction

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
