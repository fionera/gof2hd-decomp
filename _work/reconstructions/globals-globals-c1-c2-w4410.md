# Globals::Globals C1/C2 (80.5%, 612 B) — w4410 (wave 94)

Status: research note, 2026-11-03. Worktree `wt/w4410`, branch `wave94-w4410`,
base `f262bd4b`. **No source change landed** — the w3808..w4259 baseline source
order is still the best measured shape (80.5%). This commit contains only this
note. Everything below is self-contained; no transcript is required.

## TL;DR

* Baseline and final: **`_ZN7GlobalsC1Ev` / `_ZN7GlobalsC2Ev` = 80.5%, 612 B**
  (size-exact; 225 vs 227 normalised entries, M = 182 matched).
* Gate unchanged: **byte_exact 4499 / linked_exact 4499 / size-exact 4524,
  stubs 0, missing 0, wrong_type 0, extra 0, data 843/843, imports 207,
  lint CLEAN, drift 0, `RATCHET PASS`**.
* This pass built a fast single-TU scorer (compile `Globals.cpp` to `.o`, read
  the function section + its relocations, run the official
  `verify.disassemble`/`reconcile` on it) that reproduces every known score
  exactly at **0.4 s/eval instead of ~2.5 s**, then ran a much larger search
  than earlier passes:
  * the **complete 1892-move single-element neighbourhood** of the whole
    44-token post-prologue sequence (13 early + `status` + 11 settings-tail +
    19 late) — **no move improves 80.5**;
  * **20 000 random two-move samples** — best 80.1;
  * **30 rounds of plateau walking** (accept equal-score moves to escape the
    local optimum) — never above 80.5.
* New structural facts proved this pass (details in §5):
  1. The baseline GOT pool is the original's **with `status` inserted at slot
     6**; every other slot is identical. Moving `status` into the late block
     gives the exact 31/31 pool but *costs* ≥2.8 points.
  2. The `WIN` early order (`recordHandler` moved after `generator`, `status`
     out of the early group) is the **unique** single-move change that yields
     the exact 31/31 pool — proved by hill-climbing a pool-match objective —
     and it also makes the early code region exactly **96/96** (the
     `movw r6,#583` hoist). It costs 7 late-block matches, so the total is
     78.3 rather than 80.5.
  3. The original's extra 4 bytes of real code are exactly **two `ip`-register
     32-bit forms** (`ldr.w ip,[r0]` for `imageFactory`'s address and one
     `str.w r2,[ip]`) where we emit 16-bit `r0` forms. This is a register
     allocator decision, not a source-order one.
  4. The `movw r6,#583` hoist is only produced when the quality block is
     scheduled after `iaw` (`q_before_iaw` → **71.7**), i.e. the hoist and the
     late-block alignment are mutually exclusive at the source level.
  5. Every quality-block source variation tried (`int`/`unsigned`/`long`,
     declaration order, both-defaults-first, `fv >= 1.0f` vs `1.0f <= fv`)
     produces **byte-identical** code.

## 1. Baseline and gate

```text
verify: byte 4499  linked 4499  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
drift:  0 field(s) across 0 class(es)
```

```text
_ZN7GlobalsC1Ev  pct 80.5  osize 612  usize 612  len_o 225  len_u 227
_ZN7GlobalsC2Ev  pct 80.5  osize 612  usize 612  len_o 225  len_u 227
```

`C1` is a plain alias of `C2`; every experiment below was scored on `C2`.
Exact score: `difflib.SequenceMatcher(autojunk=False).ratio()` =
**0.8053097345132744** (M = 182, 2·182/(225+227)).

Region split of the 182 matched (original-side indices; "late" starts at the
first `hints+0` store and ends before the literal pool):

| region | matched | of |
|---|---:|---:|
| early (prologue + settings block) | 95 | 96 |
| late (settings tail + zero-inits + quality) | 49 | 90 |
| literal pool (8 float words + 31 GOT words) | 38 | 39 |

The `WIN` early order (see §5 item 3) instead scores **early 96/96, pool 39/39,
late 42/90** — i.e. it is the *fully correct* early block and GOT pool, and
the whole remaining problem is the late-block schedule.

The single early miss is the original's `movw r6,#583` at `0xe3236`; the pool
miss is `status`; the two extra `len_u` entries are the `nop`/`nop` literal-pool
padding (our real code is 468 B, the original's 472 B).

## 2. Baseline source (unchanged by this pass)

```cpp
GameSettingsRecord *settings = (GameSettingsRecord *) Globals::options;
Globals::shipTemplate = 0;
Globals::recordHandler = 0;
Globals::Canvas = 0;
Globals::gameText = 0;
Globals::font = 0;
Globals::fontAlien = 0;
Globals::fontLangSelect = 0;
Globals::rnd = 0;
Globals::globals = 0;
settings->tintR = 0.6f;
Globals::layout = 0;
Globals::appManager = 0;
Globals::generator = 0;
Globals::status = 0;

this->field_0x0 = 0;

settings->colorR = 0.5f;
settings->colorG = 0.5f;
settings->colorB = 0.5f;
reinterpret_cast<int16_t &>(settings->_region[0]) = 0x101;
*reinterpret_cast<int16_t *>(reinterpret_cast<uint8_t *>(settings) + 0x10) = 0;

settings->tintG = 0.6f;
settings->glowG = 0.5f;

settings->tintB = 0.5f;
settings->glowR = 0.5f;
CtorSecondaryObject *secondary = (CtorSecondaryObject *) Globals::hints;
settings->brightness = 1.0f;
settings->contrast = 0.5f;
settings->_region[2] = 1;
settings->enableFlag30 = 1;
reinterpret_cast<int16_t &>(settings->shortAt40) = 0;
settings->intAt50 = 0;
settings->flagAt39 = 0;
reinterpret_cast<int32_t &>(settings->intAt35) = 0;
reinterpret_cast<int32_t &>(settings->intAt31) = 0;
reinterpret_cast<int32_t &>(settings->intAt3b) = 0;
settings->flagAt4e = 0;
reinterpret_cast<int16_t &>(settings->shortAt4c) = 0;
settings->intAt48 = 0;
reinterpret_cast<int32_t &>(secondary->slot1) = 0;
secondary->flagAt13 = 0;
reinterpret_cast<int32_t &>(secondary->slot0) = 0;
settings->flagAt3f = 0;

Globals::lastSpaceMusicPlayed = -1;
Globals::gameLoaded = 0;
Globals::gameSaving = 0;

float fv = settings->qualityLevel;
int v54 = 0x247;
if (1.0f <= fv) v54 = 0x33e;
if (fv <= 0.0f) v54 = 0x19f;
settings->resWidth = v54;
int v58 = 0x201;
if (1.0f <= fv) v58 = 0x2da;
if (fv <= 0.0f) v58 = 0x16d;
settings->resHeight = v58;

Globals::initMemoryWarning = 0;
Globals::imageFactory = 0;
Globals::achievements = 0;
Globals::galaxy = 0;
Globals::instantActionScore = 0;
Globals::lastStationMusicPlayed = -1;
Globals::instantActionType = -1;
Globals::instantActionPlayerName = 0;
Globals::items = 0;
Globals::ships = 0;
Globals::instantActionWave = 0;
Globals::lastCampaignMissionFailed = -1;
this->soundResources = 0;
reinterpret_cast<int16_t &>(settings->shortAt60) = 0x100;
Globals::lastCampaignMissionFailCount = 0;
```

## 3. Ground truth (re-derived this pass, both directions)

### 3.1 Original store order (`trace` over the shipped `.so`)

`r1 = settings (Globals::options)`, `r0 = &Globals::hints` for the secondary
object, `r9 = this`. Derived by simulating the `ldr rX,[pc,#N]` / `add rX,pc` /
`ldr rX,[rX]` PIC idiom and resolving the GOT slot through `.rel.dyn`:

```text
0x38  Canvas            0x5a  shipTemplate       0x62  gameText
0x6a  font              0x72  fontAlien          0x7a  fontLangSelect
0x82  rnd               0x8a  globals            0x92  layout
0x9a  appManager        0x9e  recordHandler      0xa8  generator
0xae  this+0            0xba  options+0x0c       0xc4  options+0x08
0xcc  options+0x2c      0xce  options+0x0e       0xd0  options+0x30
0xd6  options+0x10      0xda  options+0x40       0xde  options+0x50
0xe2  options+0x39      0xe6  options+0x35       0xea  options+0x31
0xee  hints+0x04        0xf0  hints+0x13         0xf2  options+0x3f
0xf6  options+0x3b      0xfa  options+0x4e         0xfe  options+0x4c
0x102 options+0x48      0x104 hints+0x00         0x10c gameLoaded
0x114 gameSaving        0x118 lastSpace          0x120 initMemoryWarning
0x124 lastStation       0x12e lastCampaignMissionFailCount
0x138 lastCampaignMissionFailed
0x16a options+0x54 (resWidth)   0x17e options+0x58 (resHeight)
0x18a instantActionScore 0x192 instantActionWave  0x1a0 instantActionType
0x1a8 instantActionPlayerName  0x1b0 items         0x1b4 status
0x1bc options+0x60 (shortAt60) 0x1c0 galaxy       0x1c6 achievements
0x1c8 imageFactory      0x1cc ships               0x1ce soundResources
```

### 3.2 Baseline store order (same trace, our build)

```text
0x38  recordHandler     0x56  shipTemplate       0x5e  Canvas
0x66  gameText          0x6e  font               0x76  fontAlien
0x7e  fontLangSelect    0x86  rnd                0x8e  globals
0x96  layout            0x9a  generator          0xa4  appManager
0xaa  this+0            0xb6  options+0x0c       0xc0  options+0x08
0xc8  options+0x2c      0xca  options+0x0e       0xcc  options+0x30
0xd2  options+0x10      0xd6  options+0x40       0xda  options+0x50
0xde  options+0x39      0xe2  options+0x35       0xe6  options+0x31
0xea  options+0x3b      0xee  options+0x4e         0xf2  hints+0x04
0xf4  hints+0x13        0xf6  options+0x4c         0xfa  hints+0x00
0xfc  lastSpace         0x106 gameLoaded         0x10e gameSaving
0x116 initMemoryWarning 0x11e imageFactory       0x126 achievements
0x12e galaxy            0x138 lastStation        0x148 instantActionScore
0x166 options+0x54      0x182 options+0x58       0x188 options+0x48
0x18c options+0x3f      0x194 instantActionType  0x19c instantActionPlayerName
0x1a4 items             0x1ac status             0x1b0 instantActionWave
0x1b4 lastCampaignMissionFailed  0x1ba ships     0x1c0 soundResources
0x1c4 options+0x60      0x1ca lastCampaignMissionFailCount
```

The early block is textually identical to the original's (every store is
`str r2,[r0,#0]`), which is why the early region matches 95/96 despite the
different store order; only the `ldr rX,[pc,#N]` immediates matter there, and
the 4-byte `movw` shift keeps them aligned.

### 3.3 Literal pools (31 `R_ARM_GOT_PREL` words)

```text
original: Canvas, lastSpace, lastStation, lastCampaignFailed,
          lastCampaignFailCount, options, recordHandler, shipTemplate,
          gameText, font, fontAlien, fontLangSelect, rnd, globals, layout,
          appManager, generator, hints, gameLoaded, gameSaving,
          initMemoryWarning, imageFactory, achievements, galaxy,
          instantActionScore, instantActionWave, status, instantActionType,
          instantActionPlayerName, items, ships

baseline: identical, except `status` sits at slot 6 (right after `options`);
          every other slot keeps the same relative order.
```

So the pool delta is **exactly one inserted word**, and it is what costs the
single pool match.

### 3.4 The 4-byte code-size delta

```text
orig: 186 instructions = 50 Thumb-32 + 136 Thumb-16           -> 472 B
ours: 186 instructions = 48 Thumb-32 + 138 Thumb-16 + 2 nop   -> 468 B + 4 B pad
```

The two instructions that are Thumb-32 in the original but Thumb-16 in ours
are both **`ip` (r12) forms**:

| original | ours |
|---|---|
| `ldr.w ip, [r0]` (loads `&imageFactory`, live 0x144→0x1c8) | `ldr r0, [r0, #0]` |
| `str.w r2, [ip]` (one of the two `str.w r2,[ip]` stores) | `str r2, [r0, #0]` |

The original's scheduler defers the `imageFactory` store to the end of the late
block while hoisting its GOT load into the quality block; the long live range
forces `ip`, whose 32-bit encodings are what make the original's real code
472 B and therefore `nop`-free. This is a register-allocation/scheduling
decision, not a source-order one.

## 4. Search performed this pass

All variants were compiled with the exact repo compile line (NDK r18b clang
7.0.2, `-target armv7-none-linux-androideabi16 -march=armv7-a -mthumb
-mfpu=neon -mfloat-abi=softfp -std=c++11 -fPIC -frtti
-fstack-protector-strong -Oz -g -stdlib=libc++`) and scored with the official
`tools/verify/verify.py` normalisation (`autojunk=False`).

### 4.1 Fast scorer

`/dev/shm/w4410/par.py` compiles only `Globals.cpp` to a `.o`, extracts
`.text._ZN7GlobalsC2Ev` + its `SHT_REL` relocations, reconstructs the
`got:SYM` names from the `R_ARM_GOT_PREL` pool words, and feeds the blob to
`verify.disassemble` with a stub resolver. It reproduces every previously
published score to the digit (baseline 80.5, `WIN` 78.3, `WIN`+swap 78.8,
`tailorig` 79.2, `lateorig` 46.9, `status@items` 77.0), at 0.40 s per
evaluation, and with 128 cores it evaluates ~250 variants/s.

### 4.2 Search results

| search | space | best |
|---|---|---:|
| single-element move, whole 44-token sequence | 44·43 = 1892 | **80.5** (no move improves) |
| single-element move, 30-token tail+late | 30·29 = 870 | 80.5 |
| single-element move, 19-token late block | 342 | 80.5 |
| quality block at end of late block / before `items` / before `iaw` | 3 | 74.3 / 77.9 / 71.7 |
| random two-move samples (whole 44-token sequence) | 20 000 | 80.1 |
| plateau walk, 30 rounds, 44-token sequence | 30·1892 | 80.5 |
| `status` inserted at every late-block position (20) | 20 | 77.4 |

The plateau walk accepted equal-score moves for 30 rounds (plateau size 59–165
neighbours per round) and never found anything above 80.5.

### 4.3 Measured variants (all `len 225/227` unless noted)

| variant | pct | early | late | pool |
|---|---:|---:|---:|---:|
| **baseline** | **80.5** | **95/96** | **49/90** | 38/39 (30/31) |
| `WIN` early + `WIN` late (`body_win.txt`) | 78.3 | **96/96** | 42/90 | **39/39 (31/31)** |
| `WIN` early + baseline late + `status`@10 | 78.8 | 90/96 | 49/90 | **39/39 (31/31)** |
| `WIN` early + original store order late | 49.1 | — | — | — |
| original settings-tail order (`tailorig`) | 79.2 | 95/96 | 46/90 | 38/39 (30/31) |
| original store order as source (`lateorig`) | 46.9 | — | — | — |
| `status` after `items` | 77.0 | — | — | — |
| `status` after `ias` (best `status`-late position) | 77.4 | — | — | — |
| `recordHandler` moved to end of early block (status still early) | 79.2 | 95/96 | 49/90 | 35/39 |
| `lastSpace` after `gameSaving` | 79.6 | — | — | — |
| `lcmfc`,`lcmf` before quality | 75.2 | — | — | — |
| quality block at end of late block | 74.3 | — | — | — |
| quality block before `items` | 77.9 | — | — | — |
| quality block before `iaw` (`movw r6` hoist!) | 71.7 | — | — | — |
| quality `unsigned` / `long` / `v58` first / both defaults first | 80.5 | 95/96 | 49/90 | 38/39 (30/31) |

## 5. What this proves

1. **The baseline is a genuine local optimum of the fuzzy metric**, not an
   artefact of a small search: the complete single-move neighbourhood of the
   entire post-prologue statement sequence, 20 000 two-move samples and 30
   rounds of plateau walking all top out at 80.5.
2. **The `status` pool slot is a zero-sum trade.** The baseline's `status` in
   the early block costs exactly one pool match but is worth ~3 points of
   late-block alignment; moving it late fixes the pool (31/31) and loses more.
3. **The `WIN` early order is the correct early block.** Moving
   `recordHandler` after `generator` (and `status` out of the early group) is
   the unique single move that makes the pool exactly 31/31 (proved by
   hill-climbing a pool-match objective to 31/31), and it *also* makes the
   early code region exactly **96/96** (the `movw r6,#583` hoist lands where
   the original has it — `WIN` is the only measured shape that does). It costs
   7 late-block matches, which is why the total is 78.3 rather than 80.5. If
   the late block could be fixed while keeping this early order, the score
   would be M = 96+49+39 = 184 → **2·184/450 = 81.8**. This is the sharpest
   remaining target.
4. **The `movw r6,#583` hoist is gated on the quality block's position.**
   It appears only when the quality block is scheduled after `iaw` — `WIN`
   does exactly that (its `QUALITY` token sits at late-block position 12,
   after `iaw`), and `q_before_iaw` on the baseline early block reproduces the
   hoist at 0x56 but scores 71.7. Every source order that keeps the quality
   block near its original position leaves the default in `r0` inside the
   quality block. `WIN` early + baseline late + `status`@10 shows this
   directly: pool 31/31 but early only 90/96, because the quality block is
   back at position 3 and the hoist is gone.
5. **The 2 `nop`s are size-free to remove:** real code 468 B + 4 B = 472 B is
   already 8-aligned, so the pool needs no padding and the function stays
   exactly 612 B. The only faithful 4 bytes available are the original's two
   `ip` 32-bit forms (§3.4); they are a register-allocation outcome, not a
   source-order one.
6. **The quality-block source form is fully determined** — `int`/`unsigned`/
   `long`, declaration order, `fv >= 1.0f` vs `1.0f <= fv`, and
   both-defaults-first all compile to byte-identical code. The remaining
   quality-block delta (default placement, `r6` vs `r0`) is pure
   scheduling/RA.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w4410
cmake --build cmake-build-match --target check -j8     # gate
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN7GlobalsC2Ev
```

Scratch harness (not tracked, `/dev/shm/w4410`):
`Globals.cpp.orig` (byte-identical to HEAD), `body_baseline.txt`,
`body_win.txt`, `gen2.py`/`gen4.py`/`gen5.py` (body generators),
`par.py` (parallel fast scorer), `fast.py` (single-shot scorer),
`climb2.py`/`climb3.py`/`climb4.py` (hill climbers),
`plateau.py` (plateau walk), `rnd2.py` (random two-move search),
`poolsearch.py` (pool-order hill climb), `trace2.py` (store-order tracer),
`qvar.py` (quality-block variants), `side.py`/`align2.py`/`align3.py`
(instruction alignment).

## 7. What remains

The function cannot be improved by statement reordering. The sharpest target is
now precise:

**Keep the `WIN` early block (early 96/96, pool 31/31, `movw r6,#583`
hoisted) and fix only the late block.** That is worth
M = 96 + 49 + 39 = 184 → **81.8** (`len_u` would still be 227 because the two
`ip` forms are still missing). `WIN` currently scores 78.3 only because its
late block is 42/90 vs the baseline's 49/90. If the two `ip` forms are also
recovered, `len_u` drops to 225 and M rises by 2 → 186 → **82.7**.

The three coupled residuals:

1. **`status` pool slot** — the pool is the original's with one inserted word.
   Fixing it costs more than it gains unless the late-block schedule is fixed
   at the same time.
2. **`movw r6,#583` hoist** — requires the quality block to be scheduled after
   `iaw`. `WIN` does this and gets the hoist; the baseline does not.
3. **`ip` register allocation for `imageFactory`** — the original defers that
   store to the end of the late block and holds `&imageFactory` in `ip` across
   ~130 bytes; we store it immediately. This is the only route to removing the
   two `nop`s without changing the function size.

A future pass should start from the `WIN` early order and search *only* the
late block (20 tokens) for a shape that recovers the 7 lost matches, ideally
with a construct that changes the late-block scheduling heights (e.g. a
different IR shape for the `instantAction*` / `items` / `ships` group) rather
than the statement order. The fast scorer in §6 makes a 20-token exhaustive
plateau walk cheap (≈8 s/round on 128 cores).
