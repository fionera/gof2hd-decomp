# Globals::Globals C1/C2 (80.5%, 612 B) — w4108

Status: research note, 2026-11-03. Worktree `wt/w4108`, branch `wave91-w4108`,
base `a4557300`. **No source change landed** — the w3808/w3827 baseline source
order is still the best measured shape (80.5%). This commit contains only this
note. Everything below is self-contained; no transcript is required.

## TL;DR

* Baseline and final: **`_ZN7GlobalsC1Ev` / `_ZN7GlobalsC2Ev` = 80.5%, 612 B**
  (size-exact; 225 vs 227 normalised instructions). Gate unchanged:
  **byte_exact 4498 / linked_exact 4498 / size-exact 4524, stubs 0, missing 0,
  wrong_type 0, extra 0, data 843/843, imports 207, .text 0x1337c8,
  .rodata 0x41b67, lint CLEAN, drift 0, `RATCHET PASS`**.
* The remaining delta is one SelectionDAG slot plus one register-allocation
  choice, and the two are coupled through the same priority queues:
  1. The original's post-ISel DAG order of the 31 GOT loads is the baseline's
     with `status` moved from slot 7 to slot 27 (between
     `instantActionWave` and `instantActionType`). The baseline is otherwise
     identical.
  2. The original keeps the `resWidth` default `583` in callee-saved `r6`
     across the whole settings block (`movw r6,#583` @0x56). The baseline
     rematerialises it into `r0` inside the quality block (`movw r0,#583`
     @0x152). The RA only picks `r6` when `r0` is still live at the
     pre-RA position of the `movw`; that happens only when the quality block is
     scheduled after `instantActionPlayerName`/`items`/`ships`, which changes
     the DAG order of the late GOT loads.
* New this pass: an exact-pool source order exists (both `cand` below and
  variant `X` reproduce the target's 31/31 pool), but neither gets `r6`; the
  best `r6` variant found (75.2%) has a wrong hoist set. The trade-off is
  measured and tabulated in §5. Nothing beats 80.5.

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

Key offsets in the original:

```text
0x56  movw r6, #583          <- resWidth default, hoisted, kept in callee-saved r6
0x148 movw r0, #513          <- resHeight default, hoisted only to here, in r0
0x16a str  r6, [r1, #84]     <- settings->resWidth = v54
0x17e str  r0, [r1, #88]     <- settings->resHeight = v58
```

The original's quality block is at `0xe330a..0xe335e`, i.e. after the
`gameLoaded/gameSaving/initMemoryWarning/imageFactory/achievements` stores and
before `galaxy`. The width default is nevertheless materialised at `0xe3236`,
just before the `shipTemplate` store, and stays live in `r6` until `0xe334a`.
The height default is materialised at `0xe3328`, immediately after the last
`r0` use (`ldr ip,[r0]` @0xe3324), and stays in `r0`.

## 3. Baseline vs original: one DAG slot

`-mllvm -print-after-all` on `src/game/core/Globals.cpp` (filtered to
`_ZN7GlobalsC2Ev`) emits the post-ISel DAG order at the
`Expand ISel Pseudo-instructions` dump. The 31 `tLDRLIT_ga_pcrel` nodes are in
this order for the baseline:

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
`instantActionType`). Nothing else differs. (The original `.so` is stripped,
so the original's DAG order is inferred from its machine-code pool order; the
first GOT load of each slot is the pool order.) The first seven entries are the
prologue-hoisted set. The `list-hybrid` scheduler is bottom-up, so the first
seven are the last seven nodes it schedules; their membership is sensitive to
the height/depth of every store in the block.

Ours (baseline) machine code has `movw r0,#583` @0x152 and `movw r0,#513`
@0x16e; `str r0,[r1,#84]` is @0x16c. The literal pool is identical except
`status` occupies slot 7 and everything after shifts by one (pool LCS 30/31).

## 4. The `movw r6` hoist is a register-allocation cliff

Dump evidence (shape with quality after `ships`, which does hoist):

| pass | `t2MOVi16 583` |
|---|---|
| After Expand ISel Pseudo-instructions | `%70:rgpr = t2MOVi16 583` at the quality block |
| After Greedy Register Allocator | `%74:rgpr = t2MOVi16 583` at the same position |
| After Virtual Register Rewriter | `$r6 = t2MOVi16 583` at the same position |
| After Post RA top-down list latency scheduler | `$r6 = t2MOVi16 583` hoisted to the top of `bb.0` |

So the sequence is:

1. The greedy RA must pick **callee-saved `r6`** for the `v54` select chain.
   It does this only when all caller-saved scratch registers (`r0`, `r3`,
   `r12`, `lr`, ...) are still live at the pre-RA position of the `movw`.
2. The post-RA top-down list latency scheduler then hoists the `movw` to the
   earliest point of the live range (right after the last hoisted GOT load),
   which is exactly `0x56` in the original.

In the baseline the RA picks `r0` (the quality block sits after `gameSaving`,
where `r0` is free), so the post-RA scheduler leaves the `movw` in place at
`0x152`. `-print-after-all` on the baseline shows `$r0 = t2MOVi16 583` at both
the Greedy RA and post-RA dumps.

The original's `movw r6` @0x56 sits *inside* the live range of the
`shipTemplate` address (`r0` is loaded from the `shipTemplate` GOT slot at
0xe322e and is not stored until 0xe323a), which is exactly the kind of
pressure that makes the RA pick a callee-saved register. This is consistent
with the pre-RA position of the `movw` having been before the `shipTemplate`
store in the original.

## 5. Measurements this pass

All variants were built with the exact repo compile line
(`-target armv7-none-linux-androideabi16 -march=armv7-a -mthumb -mfpu=neon
-mfloat-abi=softfp -std=c++11 -fPIC -frtti -fstack-protector-strong -Oz -g
-stdlib=libc++`, NDK r18b clang 7.0.2) and scored with the official verifier
(`tools/verify/verify.py`, `autojunk=False`). `pool` is the LCS against the
target's 31-slot pool.

| variant | source-order change vs baseline | pct | pool | `movw` 583 |
|---|---|---:|---:|---|
| **baseline** | — | **80.5** | 30/31 | `r0` @0x152 |
| `cand` | `status` after `lastStation`; `recordHandler` after `generator` | 78.8 | **31/31** | `r0` @0x14a |
| `X` | `cand` late order with `wave` immediately after `score`, quality after `lastCampaignMissionFailCount` | 73.0 | **31/31** | `r0` @0x180 |
| `S` | `X` but `lastStation/failed/failCount` immediately before the quality block (quality after `failCount`) | 71.7 | 30/31 | `r3` @0x170 |
| `AC` | `X` with quality after `ships`; `lastStation/failed/failCount` after quality | 74.3 | 25/31 | **`r6` @0x56** |
| `X` qpos 12 | quality after `instantActionPlayerName` | 75.2 | 25/31 | **`r6` @0x56** |
| `X` qpos 15 | quality after `lastStation` | 75.2 | 27/31 | **`r6` @0x56** |
| `X` qpos 14 | quality after `ships` | 74.3 | 25/31 | **`r6` @0x56** |

Key observations:

* **Exact pool is reachable but does not buy `r6`.** `cand` and `X` both
  reproduce the original's 31/31 pool. In both, the quality block is early
  enough (or late enough after all GOT stores) that `r0` is free at the
  pre-RA `movw`, so the RA picks `r0` and the post-RA scheduler does not
  hoist. `cand` = 78.8, `X` = 73.0.
* **`r6` is reachable but only with a wrong hoist set.** Moving the quality
  block after `instantActionPlayerName`/`items`/`ships` puts the `movw` at a
  point where `r0` is live and yields `movw r6,#583` @0x56. But the
  bottom-up scheduler then hoists the three stores immediately before the
  quality block instead of `lastStation/lastCampaignFailed/lastCampaignFailCount`.
  `AC` is the cleanest example: `r6` @0x56 and the first two pool slots
  correct, but the hoisted set is `playerName, items, ships` (pool 25/31).
* **The `r3` discovery.** In `S`, the three target stores are immediately
  before the quality block, so they *are* the hoisted set (first 25 pool
  slots exact). The wave store is after the quality block in source, so its
  GOT pointer is live across the quality block and the RA assigns it `r0`;
  the width default then gets caller-saved `r3` (not `r6`), because `r3` is
  free after the `lastStation` store. To get `r6` one must also keep
  `r3`/`r12`/`lr` live across the `movw`; every source shape tried that does
  this moves the hoist set away from the target.
* **Quality-block variants are otherwise equivalent.** With the target-pool
  source order, `if`/`else if`, ternaries, `{v54;v58}` grouped blocks,
  `bool`/`unsigned` temporaries, `fv >= 1.0f`, width-first and split stores
  all produce identical machine code (`movw r0` in the quality block,
  78.8%); only `else`/`else if` forms are much worse (47–59%).
* **Declaring `v54`/`v58` early does not help.** `int v54 = 0x247;` at the
  top of the function (before the settings block) is sunk by clang to the
  phi at the quality block; the post-ISel `t2MOVi16 583` is at the same
  position as in `cand` and the RA still picks `r0`.
* **Splitting computation from stores does not help.** Moving the
  `fv`/compare/select computation before the late stores and the
  `resWidth`/`resHeight` stores after them produces 55–72% and still `r0`
  (the scheduler sinks the computation to the stores).

### 5.1 `cand` source order (exact pool, 78.8%)

Early block: `shipTemplate, Canvas, gameText, font, fontAlien,
fontLangSelect, rnd, globals, tintR, layout, appManager, generator,
recordHandler, field0, settingsblock`.

Late block: `lastSpace, gameLoaded, gameSaving, [quality], initMemoryWarning,
imageFactory, achievements, galaxy, instantActionScore, lastStation, status,
instantActionType, instantActionPlayerName, items, ships,
instantActionWave, lastCampaignMissionFailed, soundResources, shortAt60,
lastCampaignMissionFailCount`.

This is the minimal edit from the baseline that makes the DAG order exactly
the target: move `status` after `lastStation` and move `recordHandler` to the
end of the early block (so it, not `status`, is the 7th hoisted GOT load).

### 5.2 `X` source order (exact pool, 73.0%)

Same early block as `cand`, but the late block is reordered to

```text
lastSpace, gameLoaded, gameSaving, initMemoryWarning, imageFactory,
achievements, galaxy, instantActionScore, instantActionWave, status,
instantActionType, instantActionPlayerName, items, ships, lastStation,
lastCampaignMissionFailed, lastCampaignMissionFailCount, [quality],
soundResources, shortAt60
```

`X` gives the exact target pool with the quality block late, but the `movw`
lands at 0x180 after the last GOT store, where `r0` is free, so the RA picks
`r0`.

## 6. What remains

The gap is the conjunction of two facts:

1. The original's `status` store is at DAG slot 27 (not 7).
2. The original's `resWidth` default is live from the prologue to the
   quality block in `r6`.

Every source shape measured satisfies one but not both. A future pass would
need a source construct that keeps `r0`/`r3`/`r12`/`lr` live at the `movw`
*while* the bottom-up scheduler still hoists `lastStation,
lastCampaignMissionFailed, lastCampaignMissionFailCount` as the first seven
GOT loads. The most promising unexplored direction is a construct that
separates the `shipTemplate` GOT load from its store (so `r0` is live across
the `movw`) without moving the quality block past the late zero-init stores.
The build-flag question is settled (`build_flags_independent_audit_w3750.md`,
`worst_function_flag_sensitivity_w3751.md`): `-pre-RA-sched=source` moves the
`movw` from 0x152 to 0x10a but the fuzzy stays 80.5, so it is not a candidate.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w4108
cmake --build cmake-build-match --target check -j8
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN7GlobalsC2Ev
```

The variant sources and the `feval.py`/`harness.py` helpers used above live in
`/dev/shm/w4108` (scratch, not tracked). The compile line is the repo's own
`cc.sh`; the verifier is the repo's `tools/verify/verify.py`.
