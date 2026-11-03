# Globals::Globals C1/C2 (80.5%, 612 B) — w4008

Status: research note, 2026-11-03. Worktree `wt/w4008`, branch `wave90-w4008`,
base `360c7467`. **No source change landed** — the w3808/w3827 baseline source
order is still the best measured shape (80.5%). This commit contains only this
note. Everything below is self-contained; no transcript is required.

## TL;DR

* Baseline and final: **`_ZN7GlobalsC1Ev` / `_ZN7GlobalsC2Ev` = 80.5%, 612 B**
  (size-exact; 225 vs 227 normalised instructions). Gate unchanged:
  **byte_exact 4498 / linked_exact 4498 / size-exact 4524, stubs 0, missing 0,
  wrong_type 0, extra 0, data 843/843, imports 207, .text 0x1337c8,
  .rodata 0x41b67, lint CLEAN, drift 0, `RATCHET PASS`**.
* The remaining delta is **scheduler state**, not a missing statement or wrong
  constant. Two independent scheduling facts were established this pass with
  `-mllvm -print-after-all`:
  1. The **GOT-load hoist** (first 7 `tLDRLIT_ga_pcrel`) is decided by the
     SelectionDAG `list-hybrid` scheduler during ISel. The baseline's post-ISel
     DAG order is *byte-for-byte the original's* except `status` sits at slot 7
     instead of slot 27.
  2. The **`movw r6,#583` hoist** is a *post-RA* artefact and is gated on the
     greedy register allocator first choosing callee-saved `r6` for the
     `resWidth` select chain. That only happens when the quality block is
     scheduled after `items`/`ships`, which pollutes the GOT hoist.
* The two requirements are coupled through the same priority queues: every
  source shape that fixes one breaks the other. Measured ~60 new variants plus
  100 random permutations; none beats 80.5.

## 1. Baseline

```text
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

```text
_ZN7GlobalsC1Ev  pct 80.5  osize 612  usize 612  len_o 225  len_u 227
_ZN7GlobalsC2Ev  pct 80.5  osize 612  usize 612  len_o 225  len_u 227
```

`C1` is a plain alias of `C2` in the shipped `.so` and in ours; both score the
same, so all experiments below were scored on `C2`.

## 2. Ground truth: original machine code

Original `_ZN7GlobalsC2Ev` @ `0xe31e0`, 612 B, literal-pool base `0xe33c8`,
31 `R_ARM_GOT_PREL` slots. Pool order (= order of first GOT load in the final
machine code):

```text
Canvas, lastSpace, lastStation, lastCampaignFailed, lastCampaignFailCount,
options, recordHandler, shipTemplate, gameText, font, fontAlien,
fontLangSelect, rnd, globals, layout, appManager, generator, hints,
gameLoaded, gameSaving, initMemoryWarning, imageFactory, achievements,
galaxy, instantActionScore, instantActionWave, status, instantActionType,
instantActionPlayerName, items, ships
```

Key offsets:

```text
0x56  movw r6, #583          <- resWidth default, hoisted, kept in callee-saved r6
0x148 movw r0, #513          <- resHeight default, hoisted only to here, in r0
0x16a str  r6, [r1, #84]     <- settings->resWidth = v54
0x17e str  r0, [r1, #88]     <- settings->resHeight = v58
```

Ours (baseline) has `movw r0,#583` at `0x152` and `movw r0,#513` at `0x16e`;
`str r0,[r1,#84]` is at `0x16c`. The literal pool is identical except `status`
occupies slot 7 (right after `options`) and everything after shifts by one.

## 3. The SelectionDAG GOT-load hoist (confirmed)

`-mllvm -print-after-all` on `src/game/core/Globals.cpp` emits per-function
dumps. The `Globals` constructor's whole pipeline is in the block that starts
at the `# *** IR Dump After Expand ISel Pseudo-instructions ***:` header
immediately preceding `# Machine code for function _ZN7GlobalsC2Ev` (in the
w4008 dump: line 1686063, with the post-RA scheduler dump at line 1694327).

The post-ISel DAG order of the 31 `tLDRLIT_ga_pcrel` nodes is exactly the
literal-pool order. For the **baseline** it is:

```text
Canvas, lastSpace, lastStation, lastCampaignFailed, lastCampaignFailCount,
options, status, recordHandler, shipTemplate, gameText, font, fontAlien,
fontLangSelect, rnd, globals, layout, appManager, generator, hints,
gameLoaded, gameSaving, initMemoryWarning, imageFactory, achievements,
galaxy, instantActionScore, instantActionWave, instantActionType,
instantActionPlayerName, items, ships
```

Compare with §2: the original is **the same list with `status` moved from
position 7 to position 27** (between `instantActionWave` and
`instantActionType`). Nothing else differs. This is the cleanest statement of
the remaining gap: the two binaries differ by one element's position in the
DAG order, and that one element is `status`.

The first seven entries are the prologue-hoisted set. The `list-hybrid`
scheduler is bottom-up (`createHybridListDAGScheduler` uses
`HybridBURRPriorityQueue(..., IsBottomUp=true)`), so the "first 7" are the last
seven nodes it schedules; the set is stable but its membership is sensitive to
the height/depth of every store in the block.

### 3.1 Moving `status` late reshuffles everything

If `status` is moved out of the early block to any late position, the DAG
order changes far more than one slot. Example (baseline early block, `status`
inserted after `instantActionWave`):

```text
recordHandler, lastSpace, lastStation, lastCampaignFailed,
lastCampaignFailCount, options, generator, shipTemplate, Canvas, gameText,
font, ... , instantActionScore, status, instantActionType, ...,
ships, instantActionWave
```

`recordHandler` jumps to slot 1, `Canvas` falls to slot 9, `generator` enters
the hoisted set, and `instantActionWave` falls to the very end. Fuzzy 77.0.

The only shape found that restores the original's **first seven** is to also
move `recordHandler` to the *end* of the early block:

```text
early block: shipTemplate, Canvas, gameText, font, fontAlien, fontLangSelect,
             rnd, globals, tintR, layout, appManager, generator, recordHandler
```

which yields

```text
Canvas, lastSpace, lastStation, lastCampaignFailed, lastCampaignFailCount,
options, recordHandler, shipTemplate, gameText, font, fontAlien,
fontLangSelect, rnd, globals, layout, appManager, generator, hints,
gameLoaded, gameSaving, initMemoryWarning, imageFactory, achievements,
galaxy, instantActionScore, instantActionType, status, instantActionPlayerName,
items, ships, instantActionWave
```

i.e. the original's pool with only `status`/`instantActionWave` swapped.
Fuzzy **78.3** (the rest of the late block reorders), and the `movw` is *not*
hoisted (`movw r0,#583` at `0x14a`).

## 4. The `movw r6,#583` hoist is post-RA, not SelectionDAG

This refines the w3827 note, which described the *GOT-load* hoist. The
`resWidth` constant hoist is a different mechanism. Dump evidence for a shape
that does hoist (`quality block after ships`, §5):

| pass | `t2MOVi16 583` |
|---|---|
| After Expand ISel Pseudo-instructions | `%70:rgpr = t2MOVi16 583` at the quality block |
| After Greedy Register Allocator | `%74:rgpr = t2MOVi16 583` at the same position |
| After Virtual Register Rewriter | `$r6 = t2MOVi16 583` at the same position |
| After Post RA top-down list latency scheduler | `$r6 = t2MOVi16 583` hoisted to the top of `bb.0` |

So the sequence is:

1. The greedy RA must pick **callee-saved `r6`** for the `v54` select chain.
   It does this only when `r0` is still occupied by a GOT pointer at the
   quality block.
2. The post-RA top-down list latency scheduler then hoists the `movw` to the
   earliest point of the live range (right after the last hoisted GOT load),
   which is exactly `0x56` in the original.

In the baseline the RA picks `r0` (the quality block sits after `gameSaving`,
where `r0` is free), so the post-RA scheduler leaves the `movw` in place at
`0x152`. `-print-after-all` on the baseline shows `$r0 = t2MOVi16 583` at both
the Greedy RA and post-RA dumps.

## 5. The hoist is reachable — but only by polluting the GOT hoist

Sweeping the quality block through every position in the late zero-init block
(`qpos0..qpos18`, statements in baseline order) gives:

| quality block after … | `movw` 583 | pool7 |
|---|---|---|
| `lastSpace` … `instantActionPlayerName` | `r0` @ 0x14a/0x152/0x15a | baseline/other |
| **`items`** (`qpos12`) | **`r6` @ 0x56** | Canvas, lastSpace, instantActionType, instantActionPlayerName, items, options, status |
| **`ships`** (`qpos13`) | **`r6` @ 0x56** | Canvas, lastSpace, instantActionPlayerName, items, ships, options, status |
| **`instantActionWave`** (`qpos14`) | **`r6` @ 0x56** | Canvas, lastSpace, items, ships, instantActionWave, options, status |
| `lastCampaignMissionFailed` (`qpos15`) | `r3` @ 0x170 | Canvas, lastSpace, ships, instantActionWave, lastCampaignFailed, options, status |
| `soundResources` / `shortAt60` / `lastCampaignMissionFailCount` | `r0` @ 0x180+ | no hoist |

`qpos13` is the interesting one: it matches the original's **first 69
instructions byte-for-byte** (including `movw r6,#583` at `0x56`), because the
early block, the store order and the `movw` all line up. But the four hoisted
late-block GOT loads are `instantActionPlayerName, items, ships` instead of
`lastStation, lastCampaignFailed, lastCampaignFailCount`, so the pool is wrong
and the late block diverges. Fuzzy **71.7** (`qpos12` = 73.0, `qpos14` =
72.1).

This is the core coupling: the quality block must be *late* for `r0` to be
busy at the RA, but the original's quality block is *early* (right after
`gameSaving`) so that the three `lastCampaign*` stores stay at the end of the
block and their GOT loads are the ones the SelectionDAG scheduler hoists.

## 6. Variant sweep (this pass)

All variants were compiled with the repo's exact `cc.txt` command and scored
with `tools/verify/verify.py` (`autojunk=False`) against the linked `.so`.
`pct` is `_ZN7GlobalsC2Ev`.

| variant | pct | note |
|---|---:|---|
| **baseline** | **80.5** | |
| `q_after_settings` (quality block after `flagAt3f=0`) | 80.5 | same pool7 + same `movw` position |
| `v54decl_early` / `bothdecl_early` / `v54decl_top` | 80.5 | constant rematerialised, identical code |
| `q13_hints_first` (quality after ships + hints-first settings) | 73.0 | |
| `q13_orig` (quality after ships) | 71.7 | first 69 insns match, pool polluted |
| `qpos12` / `qpos14` (quality after items / instantActionWave) | 73.0 / 72.1 | `movw r6,#583` @ 0x56, pool polluted |
| `rech_last` + status late (pool7 exact) | 78.3 | no `movw` hoist |
| `rech_last` + `iaw_after_ias_status` | 77.4 | |
| `rech_last` + status after instantActionType | 77.0 | |
| `status_after_*` (baseline early block, 11 positions) | 76.1–77.4 | `recordHandler` jumps to slot 1 |
| `calc_before_settings` (compute v54/v58 before settings, store after) | 72.1 | `movw r0` @ 0x14e |
| `allcalc_early` | 54.1 | usize 612 |
| `items_ships_before_q` | 77.9 | |
| `iapn_items_ships_before_q` | 76.5 | |
| `q_after_lcmfc` / `q_after_lcmfc_ships` | 74.3 / 73.9 | |
| `machine_store` late block (original store order as source) | 64.0 | usize 600 |
| settings block: `f3f_after_i31`, `f3f_after_i3b`, `i48_f3f_early` | 80.5 | byte-identical |
| settings block: `hints_first`, `machine`, `hints_before_i3b` | 79.2 | |
| settings block: `f3f_first` | 80.1 | |
| random early-block permutations (40, seed 7) | best 78.3 | |
| random late-block permutations + random quality position (60, seed 1) | best 69.9 | |

## 7. Settings-block residual (scheduler-determined)

The settings block is the largest contiguous mismatch. Original store order:

```text
intAt50, flagAt39, intAt35, intAt31, hints.slot1, hints.flagAt13, flagAt3f,
intAt3b, flagAt4e, shortAt4c, intAt48, hints.slot0
```

Ours:

```text
intAt50, flagAt39, intAt35, intAt31, intAt3b, flagAt4e, hints.slot1,
hints.flagAt13, shortAt4c, hints.slot0, [quality block], intAt48, flagAt3f
```

i.e. ours emits `intAt3b`/`flagAt4e` before the `hints` stores and sinks
`intAt48`/`flagAt3f` past the quality block. Reordering the source to the
original's machine order (`hints_first`) makes the scheduler sink
`intAt3b`/`flagAt4e`/`shortAt4c`/`intAt48` instead and loses 1.3 points. The
store order is fully scheduler-determined; there is no source lever that keeps
`intAt48`/`flagAt3f` in the settings block while also keeping the rest of the
block in the original's order.

## 8. What remains

The gap is one DAG-order slot (`status`) plus the RA's register choice for the
`resWidth` select chain. Both are decided by the `list-hybrid` /
`HybridBURRPriorityQueue` height/depth comparison, which is a function of the
whole block's store chain. To close it we would need either:

* a source construct that gives the `status` store a *late* height while
  leaving every other store's height unchanged (so the DAG order moves only
  `status`), or
* a source construct that makes the greedy RA pick `r6` for `v54` while the
  quality block stays at its current position (e.g. an extra live value in
  `r0` across the quality block that does not add an instruction), or
* binary evidence for a different original statement order in the late
  zero-init block — the current baseline order is a genuine local optimum
  (random search over 40 early-block and 60 late-block permutations never
  exceeded 78.3).

The build-flag question is settled (`build_flags_independent_audit_w3750.md`,
`worst_function_flag_sensitivity_w3751.md`); `-pre-RA-sched=source` moves the
`movw` but the fuzzy stays 80.5.

## 9. Reproduction

```sh
cmake --preset default
# this worktree needs the original .so and FMOD libs passed explicitly:
cmake -B cmake-build-match \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN7GlobalsC2Ev
```

`-mllvm -print-after-all` on the single TU (25 s, ~220 MB stderr) reproduces
§3/§4; the `Globals` constructor block starts at the `Expand ISel
Pseudo-instructions` header immediately before `# Machine code for function
_ZN7GlobalsC2Ev`.
