# Globals::Globals C1/C2 (80.5%, 612 B) — w4908 (wave 99)

Status: research note, 2026-11-04. Worktree `wt/w4908`, branch `wave99-w4908`,
base `44225192c`. **No source change landed** — the w3808..w4510 baseline source
order is still the best measured shape (80.5%). This commit contains only this
note. Everything below is self-contained; no transcript is required.

## TL;DR

* Baseline and final: **`_ZN7GlobalsC1Ev` / `_ZN7GlobalsC2Ev` = 80.5%, 612 B**
  (size-exact; 225 vs 227 normalised entries, M = 182 matched,
  `2·182/452 = 0.8053097345132744`).
* Gate unchanged: **byte_exact 4500 / linked_exact 4500 / size-exact 4524,
  stubs 0, missing 0, wrong_type 0, extra 0, data 843/843, imports 207,
  lint CLEAN, drift 0, `RATCHET PASS`**.
* This pass attacked the two axes named in the brief — the exact zero-init
  store schedule and the `movw r6,#583` placement — with searches that had not
  been run before, and established where the `movw` really comes from:
  **it is an instruction-selection-order property, not a scheduler one.**
  In our build the `t2MOVi16 583` is already at machine-IR line 119 (right
  before the first `VCMPES`) immediately after ISel; the pre-RA and post-RA
  schedulers keep it in that late position. The original's `movw r6,#583` sits
  at normalised index 34, 90 instructions before the compares, so the original's
  *ISel order* must have had it early (or the `if` bodies were still branches
  when the machine sinker ran, so it could not sink the movw into the merge
  block). No source form tried in this pass reproduces that.
* New searches this pass (all negative):
  * full 60-token order SA, 8 independent chains × 1200 iterations: best
    **80.5** (two chains returned the baseline; the rest 61.7–75.7);
  * all 210 single moves of the 15-token early block: **no** variant produces
    `movw r6,#583` in the first 60 normalised entries; best 80.5;
  * all 650 single moves of the 26-token settings block: **no** hoist; best 80.5;
  * all 225 `recordHandler`×`status` two-move placements in the early block:
    three 80.5 configurations (the baseline and two `recordHandler`-last
    variants), **no** hoist;
  * quality-block source forms (`else if`, combined `if`, `bool`, `?:`,
    helper function, direct stores, early default stores, split
    compute-early/store-late, `register`, `const`): 34.1–80.5, **no** hoist;
  * `imageFactory` source forms (conditional store, ternary, pointer,
    `volatile` pointer): 55.2–80.5, still only one `str.w r2,[ip]`;
  * the original machine store order as source: 46.9–63.5; the original late
    block alone: 46.9–54.4 (the prologue/zero register change);
  * quality block at late position 9/10: **does** hoist (`movw r6,#583` at
    index 34) but scores only 73.0/71.7; WIN early + quality at position 9:
    75.2 with the hoist.
* Conclusion: the baseline remains a strict local optimum at 80.5. The
  remaining gap is late-block scheduling + the two `ip` promotions + the
  `movw r6` hoist, and the three are coupled through the register allocator.
  Recovering the hoist alone is worth 80.5 → ~81.0; recovering the two `ip`
  forms alone is worth 80.5 → 80.9; doing both would be ~81.4.

## 1. Baseline and gate

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
drift:  0 field(s) across 0 class(es)
```

```text
_ZN7GlobalsC1Ev  pct 80.5  osize 612  usize 612
_ZN7GlobalsC2Ev  pct 80.5  osize 612  usize 612
```

`C1` is a plain alias of `C2`; every experiment below was scored on `C2`.
The exact score is `difflib.SequenceMatcher(autojunk=False).ratio()` over the
normalised streams = **182 matched of 225/227**.

The baseline source (unchanged; `src/game/core/Globals.cpp`,
`Globals::Globals()`) is the w3808..w4410 order:

```cpp
GameSettingsRecord *settings = (GameSettingsRecord *) Globals::options;
shipTemplate, recordHandler, Canvas, gameText, font, fontAlien,
fontLangSelect, rnd, globals, tintR, layout, appManager, generator, status,
this0, colorR, colorG, colorB, region0c, at10, tintG, glowG, tintB, glowR,
brightness, contrast, region2, enable30, short40, int50, flag39, int35,
int31, int3b, flag4e, short4c, int48, sec_slot1, sec_flag13, sec_slot0,
flag3f, lastSpace, gameLoaded, gameSaving, QUALITY, initMem, imageFactory,
achievements, galaxy, ias, lastStation, iat, iapn, items, ships, iaw, lcmf,
soundRes, short60, lcmfc
```

## 2. Ground truth: the original 55 stores and 31 pool words

`_ZN7GlobalsC2Ev` @ `0xe31e0`, 612 B, literal pool @ `0xe33c8`
(31 `R_ARM_GOT_PREL` words + 4 float words at `0xe33b8`: 0.5, 0.5, 0.6, 0.6).
Normalised stream length 225 (186 code + 8 float halfwords + 31 pool entries).

Original **store** order (PIC idiom resolved through `.rel.dyn`; `options:N`
are the settings-record field offsets, `hints:N` the secondary object):

```text
Canvas, shipTemplate, gameText, font, fontAlien, fontLangSelect, rnd,
globals, layout, appManager, recordHandler, generator, this+0,
options:0xc, options:0x0, options:0x8, options:0x24, options:0x2c,
options:0xe, options:0x30, options:0x10, options:0x40, options:0x50,
options:0x39, options:0x35, options:0x31, hints:0x4, hints:0x13,
options:0x3f, options:0x3b, options:0x4e, options:0x4c, options:0x48,
hints:0x0, gameLoaded, gameSaving, lastSpace, initMem, lastStation,
lcmfc, lcmf, options:0x54, options:0x58, ias, iaw, iat, iapn, items,
status, options:0x60, galaxy, achievements, imageFactory, ships, soundRes
```

Original **pool** order (machine order of the `tLDRLIT_ga_pcrel` nodes; the
first 7 are the prologue hoist):

```text
Canvas, lastSpace, lastStation, lcmf, lcmfc, options, recordHandler,
shipTemplate, gameText, font, fontAlien, fontLangSelect, rnd, globals,
layout, appManager, generator, hints, gameLoaded, gameSaving, initMem,
imageFactory, achievements, galaxy, ias, iaw, status, iat, iapn, items, ships
```

The baseline's pool is the original's with `status` inserted at slot 6
(pushing `recordHandler` to 7 and everything else by one): pool LCS 30/31.
The baseline's store order is:

```text
Canvas, recordHandler, shipTemplate, gameText, font, fontAlien,
fontLangSelect, rnd, globals, layout, appManager, status, generator,
options:0xc, options:0x0, options:0x4, options:0x8, options:0x24,
options:0x28, options:0x2c, options:0xe, options:0x30, options:0x10,
options:0x40, options:0x50, options:0x39, options:0x35, options:0x31,
options:0x3b, options:0x4e, hints:0x4, hints:0x13, options:0x4c,
hints:0x0, lastSpace, gameLoaded, gameSaving, initMem, imageFactory,
achievements, galaxy, lastStation, ias, options:0x54, options:0x58,
options:0x48, options:0x3f, iat, iapn, items, iaw, lcmf, ships,
options:0x60, lcmfc
```

Store LCS 38/55.

## 3. The exact divergence (normalised index map)

The two streams are identical for the first **34** entries (the whole prologue
through `ldr.w ip,[r6]`). The first difference is the original's early
`movw r6,#583`:

```text
O 34 movw r6, #583
U 34 str  r2, [r0, #0]      ; shipTemplate
U 35 ldr  r0, [pc, #428]    ; gameText GOT
U 36 add  r0, pc
U 37 ldr  r0, [r0, #0]
```

From index 35 the streams are again identical for **61** entries
(`O 35:96 == U 38:99`), i.e. the whole early pointer block and the settings
block up to `hints:0x4`. All remaining differences are in the settings tail
(`flag3f`/`int3b`/`int48`) and the late zero-init block. The matched runs are:

```text
34   prologue (0..34)
61   early+settings (35..96)
2    hints:0x13, ...
1+1+9+3+1+1+1+1+1+3+1+1+5+2+1+1+3+1+1+1+2+1+1+1+2  late-block fragments
14   float-pool halfwords
20+4 GOT pool words
= 182
```

## 4. Where the `movw r6,#583` comes from (new)

Compiling the baseline with `-mllvm -print-after-all` and inspecting the
machine IR for `_ZN7GlobalsC2Ev`:

* **After ISel** (`Expand ISel Pseudo-instructions`) the function is already a
  single basic block with `t2MOVCCi16` for the two `if` chains. The
  `t2MOVi16 583` is at machine-IR line **119**, immediately before the first
  `VCMPES`; the `shipTemplate` store is at line 33.
* The `Machine code sinking` pass leaves it at line 119 (it has already been
  sunk to its first use by the IR-level select lowering).
* The pre-RA `Machine Instruction Scheduler` leaves it at the same line 119;
  the post-RA `Post RA top-down list latency scheduler` keeps it in the same
  late position. The final `movw r0,#583` is at normalised index 139.

So the hoist cannot be produced by reordering the *scheduled* code: the movw
must be early in the **ISel order**, which follows the source order. The
original's `movw r6,#583` is at index 34, i.e. its ISel order had the movw
before the `shipTemplate` store, 90 instructions before the compares. Two
possible explanations, both consistent with the machine code:

1. the original's source declares/initialises the `resWidth` default early
   (before the early pointer block), and the machine sinker could not sink it
   because its use was in a different basic block (the `if` bodies were still
   branches at sink time); or
2. the original's `if` bodies are large enough that ISel emits branches rather
   than `select`, so the sinker cannot move the default into the merge block;
   the later `If Converter` pass turns the branches into the observed
   `it ge`/`movwge` + `it ls`/`movwls`.

Every source form tried in this pass still lowers to `select` in the optimised
IR (verified with `-print-after-all`), so the machine sinker always sinks the
default to the first `VCMPES`.

## 5. Searches run this pass (all negative)

All variants were compiled with the exact repo compile line (NDK r18b clang
7.0.2, `-target armv7-none-linux-androideabi16 -march=armv7-a -mthumb
-mfpu=neon -mfloat-abi=softfp -std=c++11 -fPIC -frtti
-fstack-protector-strong -Oz -g -stdlib=libc++`) and scored with the official
`tools/verify/verify.py` normalisation (`autojunk=False`). The fast single-TU
scorer reproduces 80.5 / 227 / 225 exactly.

### 5.1 Full-order SA (new)

A full 60-token order (15 early + 26 settings + 3 mid + 15 late + `QUALITY` as
a movable token) was searched with 8 independent simulated-annealing chains,
1200 iterations each, starting from perturbed baselines. Results:

```text
chain results: 80.5, 80.5, 75.7, 75.7, 69.2, 66.8, 63.7, 61.7
best: 80.5 (two chains returned the exact baseline order)
```

No chain exceeded 80.5. This is the strongest evidence that the baseline is not
just a single-move local optimum but a basin optimum for the fuzzy metric.

### 5.2 Early-block single moves (new)

All **15×14 = 210** single moves of the 15-token early block were measured.
**No** variant puts `movw r6,#583` in the first 60 normalised entries; the best
score is 80.5. (The `recordHandler`-last `WIN` order only hoists when `status`
is also moved out of the early block, i.e. it is a two-move change.)

### 5.3 Settings-block single moves (new)

All **26×25 = 650** single moves of the settings block were measured. **No**
variant hoists the `movw`; the best score is 80.5.

### 5.4 `recordHandler`×`status` two-move sweep (new)

All **15×15 = 225** placements of `recordHandler` and `status` in the early
block were measured. Three score 80.5:

```text
rh=12, st=1   shipTemplate, status, Canvas, ... , generator, recordHandler, this0
rh=12, st=0   status, shipTemplate, Canvas, ... , generator, recordHandler, this0
rh=1,  st=13  baseline (shipTemplate, recordHandler, Canvas, ... , generator, status, this0)
```

None hoists the `movw`. The `WIN` early order (`rh=12`, `status` moved to the
late block) is the only two-move shape that hoists, and it scores 78.8.

### 5.5 Quality-block source forms (new)

| form | pct | hoist |
|---|---:|---|
| baseline (`if` ×2, default 583/513) | **80.5** | no |
| `if/else if` | 37.6 | no |
| combined `if` for both variables | 79.2 | no |
| `bool` hi/lo temporaries | 80.5 | no |
| `?:` select chain | 80.5 | no |
| helper `qualityWidth()/qualityHeight()` | 79.2 | no |
| direct stores (`settings->resWidth = ...`) | 80.5 | no |
| early default stores + late conditionals | 34.1 | no |
| compute-early / store-late split | 52.3 | no |
| `register int` / `const int` | 80.5 | no |
| declaration hoisted to top of function | 80.5 | no |

All forms still lower to `select` in the optimised IR; the movw is always sunk
to the first `VCMPES`.

### 5.6 `imageFactory` source forms (new)

| form | pct | second `ip` form |
|---|---:|---|
| baseline `Globals::imageFactory = 0` | **80.5** | no |
| `if (0.0f <= fv) Globals::imageFactory = 0` | 55.2 | no |
| ternary | 80.5 | no |
| `ImageFactory **pif = &Globals::imageFactory; *pif = 0` | 80.5 | no |
| `ImageFactory *volatile *pif = ...` | 80.5 | no |

The only `str.w r2,[ip]` remains the `lcmfc` store. The `imageFactory` store is
still `str r2,[r0]` (Thumb-16), so the two trailing `nop`s remain.

### 5.7 Original order as source (new)

| variant | pct |
|---|---:|
| original early order, baseline settings/mid/late | 63.5 |
| original early order, baseline settings, base mid, base late + `tintR` in settings | 71.1 |
| original early + original settings tail + original late | 46.9 |
| original late block alone (mid empty) | 46.9 |
| original late block with the six mid permutations | 46.5–54.4 |

The original machine store order is **not** the original source order: writing
it as source changes the prologue hoist and the zero register (`r1` instead of
`r2`) and loses far more than it gains.

### 5.8 Quality placement (new)

| quality position | pct | hoist |
|---|---:|---|
| before the late block (baseline, q=0) | **80.5** | no |
| late position 9 | 73.0 | **yes** (`movw r6,#583` at index 34) |
| late position 10 | 71.7 | **yes** |
| WIN early + late position 9 | 75.2 | **yes** |

This is the only measured shape that gets the hoist *and* keeps the baseline
early block: moving the quality block to late position 9 makes `r0` busy at the
scheduler's first opportunity and the allocator picks `r6`. But the late block
then diverges so much that the net is −7.5.

### 5.9 Other measured variants (new)

| variant | pct | note |
|---|---:|---|
| baseline | **80.5** | reference |
| WIN early + WIN late | 78.3 | pool 31/31, hoist present |
| WIN early + baseline late + `status`@6 | 78.8 | pool 31/31, hoist present |
| WIN early + original settings tail + WIN late | 77.9 | — |
| `status` at each late position (baseline early) | 76.1–77.4 | best at position 6 |
| `mid` order `gameLoaded,gameSaving,lastSpace` | 79.6 | baseline `lastSpace` first is best |
| `tintR` moved out of the early block | 80.5 | vectoriser merges the four floats |

## 6. What this proves

1. **The baseline is a basin optimum of the fuzzy metric.** The full-order SA,
   the 210 early-block moves, the 650 settings-block moves, the 225
   `recordHandler`×`status` moves, the 40 320 settings-tail permutations
   (w4510) and the 50 490 tail×late crosses (w4708) all top out at 80.5.
2. **The `movw r6,#583` hoist is an ISel-order property.** It is already at
   machine-IR line 119 immediately after ISel; neither the pre-RA nor the
   post-RA scheduler moves it into the prologue. The only source-level way to get it is to make
   the quality block early in the IR (late position 9/10, or the WIN early
   block), both of which cost more in the late block than the hoist gains.
3. **The three remaining deltas are coupled through the register allocator:**
   the two `ip` promotions (`ldr.w ip,[r0]`, `str.w r2,[ip]` for
   `&imageFactory`) and the `movw r6` hoist all depend on the same
   pressure/allocation decision. Recovering the hoist alone is worth
   80.5 → ~81.0; the two `ip` forms alone are worth 80.5 → 80.9; both would be
   ~81.4.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w4908
cmake --build cmake-build-match --target check -j8
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN7GlobalsC2Ev
```

Machine-IR inspection used the same compile line plus
`-mllvm -print-after-all`; the relevant dumps are
`Expand ISel Pseudo-instructions`, `Machine code sinking`,
`Machine Instruction Scheduler`, `Greedy Register Allocator`,
`Virtual Register Rewriter` and `Post RA top-down list latency scheduler`.

Scratch harness (not tracked, `/dev/shm/w4908`): `fastscore2.py`
(single-TU scorer, 0.3 s/eval), `gen.py` (token→statement body generator),
`full_orig.py` / `full_ours.py` (annotated disassembly with GOT/store
resolution), `normboth.py` (indexed normalised-stream diff),
`blockdiff.py` (matched-run map), `sa_full2.py` (full-order SA),
`sweep_early_hoist.py`, `sweep_set_hoist.py`, `sweep_rh_status.py`,
`try_qforms2..6.py`, `try_live.py`, `try_if.py`, `try_split.py`,
`try_split2.py`, `try_earlystore.py`, `try_helper.py`, `try_decl.py`,
`sweep_win_q.py`.

## 8. What remains

The function cannot be improved by statement reordering. The three precise
targets are unchanged from w4708, now with sharper evidence:

1. **Recover the two `ip` 32-bit forms** (`ldr.w ip,[r0]` for
   `&imageFactory`, `str.w r2,[ip]` for its store). This is the only route to
   removing the two `nop`s and it alone is worth 80.5 → 80.9. It needs a source
   shape that makes the scheduler defer the `imageFactory` store past the
   quality block while keeping its GOT load at pool slot 21. Moving the
   statement later in source moves the GOT load too (pool 6/31 measured).
2. **Recover the `movw r6,#583` hoist at index 34.** It is the register
   allocator reusing `r6` the instant it dies at `ldr.w ip,[r6]`; it requires
   the `resWidth` default's live interval to start there. Worth
   80.5 → ~81.0. The new ISel evidence says the source must keep the movw in
   the entry block (not sink it to the first `VCMPES`), i.e. the quality block's
   `if`s must still be branches when the machine sinker runs.
3. **Combine the WIN early block with a 49/90 late block.** The WIN early
   order is the only one with the exact 31/31 pool; the missing piece is a
   late-block IR shape that keeps that prologue/pool while aligning the
   zero-init group to the original's indices. All 40 320 tail permutations,
   46 838 two-move reorders, 50 000 full-sequence random samples, the 8-chain
   full-order SA and the 342 `imageFactory`×quality placements leave this open.
