# Globals::Globals C1/C2 (80.5%, 612 B) — w5008 (wave 100)

Status: research note, 2026-11-04. Worktree `wt/w5008`, branch
`wave100-w5008`, base `44225192c`. **No source change landed** — the
w3808..w4708 baseline source order is still the best measured shape (80.5%).
This commit contains only this note. Everything below is self-contained; no
transcript is required.

## TL;DR

* Baseline and final: **`_ZN7GlobalsC1Ev` / `_ZN7GlobalsC2Ev` = 80.5%, 612 B**
  (size-exact; 225 vs 227 normalised entries, M = 182 matched,
  `2·182/452 = 0.8053097345132744`).
* Gate unchanged: **byte_exact 4500 / linked_exact 4500 / size-exact 4524,
  stubs 0, missing 0, wrong_type 0, extra 0, data 843/843, imports 207,
  lint CLEAN, drift 0, `RATCHET PASS`**.
* This pass reconstructed the **original member-init order** (the complete
  55-store sequence, with store widths and stored values) directly from the
  shipped `.so`, and then ran the **joint settings-tail × late-block search**
  requested by the brief. Every search tops out at **80.5**:
  * full store-order-as-source (all 55 stores in the original machine order):
    **52.7** (new; w4708 only reordered the late block, not the whole body);
  * simulated annealing over the 20-token late block, 8 000 moves: **80.5**;
  * 20 000 random **three-move** permutations of the whole 60-token sequence:
    **80.5**;
  * exhaustive **two-move** neighbourhood of the 14-token early block
    (4 095 candidates): **80.5**;
  * complete single-move hill-climb of the 27-token settings block: **80.5**;
  * complete single-move hill-climb of the 14-token early block: **80.5**;
  * complete single-move late-block neighbourhood (baseline early; w4410):
    **80.5**;
  * WIN early block with the quality block pinned at late position 12, then
    complete single-move hill-climb of the other tokens: best **78.8**.
* The WIN early block (`recordHandler` after `generator`, `status` late) still
  gives the exact **31/31 pool** and the `movw r6,#583` hoist, but its best
  late-block shape is **78.8** (quality at late position 0) or **78.3** (quality
  at position 12, with the `r6` hoist). The two local optima remain mutually
  exclusive at the source level.
* The only remaining code-size delta is still the **two Thumb-16→Thumb-32
  promotions** for `&Globals::imageFactory` (`ldr.w ip,[r0]` +
  `str.w r2,[ip]`). No source order found this pass produces them; they are a
  register-allocation outcome, not an ordering one.

## 1. Baseline and gate

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
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
The exact score is `difflib.SequenceMatcher(autojunk=False).ratio()` over the
normalised streams = **182 matched of 225/227**.

The baseline source (`src/game/core/Globals.cpp`, `Globals::Globals()`) is the
w3808..w4708 order:

```cpp
GameSettingsRecord *settings = (GameSettingsRecord *) Globals::options;
shipTemplate, recordHandler, Canvas, gameText, font, fontAlien,
fontLangSelect, rnd, globals, tintR, layout, appManager, generator, status,
this0, colorR, colorG, colorB, region0, at10, tintG, glowG, tintB, glowR,
brightness, contrast, region2, enable30, short40, int50, flag39, int35,
int31, int3b, flag4e, short4c, int48, sec_slot1, sec_flag13, sec_slot0,
flag3f, lastSpace, gameLoaded, gameSaving, QUALITY, initMem, imageFactory,
achievements, galaxy, ias, lastStation, iat, iapn, items, ships, iaw, lcmf,
soundRes, short60, lcmfc
```

## 2. The original member-init order (reconstructed)

`_ZN7GlobalsC2Ev` @ `0xe31e0`, 612 B, literal pool @ `0xe33c8`
(31 `R_ARM_GOT_PREL` words + 4 float words). The table below is the complete
store sequence in **machine order**, re-derived by simulating the PIC idiom
(`ldr rX,[pc,#N]` / `add rX,pc` / `ldr rX,[rX]`), resolving the GOT slot
through `.rel.dyn`, and decoding the stored constant from the register
definition. `r1 = settings (Globals::options)`, `r9 = this`, `r0 = &hints`.

```text
off   width  target                          value
0x038  str    Globals::Canvas                 0
0x05a  str    Globals::shipTemplate           0
0x062  str    Globals::gameText               0
0x06a  str    Globals::font                   0
0x072  str    Globals::fontAlien              0
0x07a  str    Globals::fontLangSelect         0
0x082  str    Globals::rnd                    0
0x08a  str    Globals::globals                0
0x092  str    Globals::layout                 0
0x09a  str    Globals::appManager             0
0x09e  str    Globals::recordHandler          0
0x0a8  str    Globals::generator              0
0x0ae  str    this+0x0                        0
0x0b2  vst1   options+0x14..0x20              {0.5,0.5,0.6,0.6}  (glowR,glowG,tintR,tintG)
0x0ba  strh   options+0x0c                    0x0101
0x0c0  strd   options+0x00,0x04               {0.5,0.5}          (colorR,colorG)
0x0c4  str    options+0x08                    0.5                (colorB)
0x0c6  strd   options+0x24,0x28               {0.5,1.0}          (tintB,brightness)
0x0cc  str    options+0x2c                    0.5                (contrast)
0x0ce  strb   options+0x0e                    1                  (region2)
0x0d0  strb   options+0x30                    1                  (enable30)
0x0d6  strh   options+0x10                    0
0x0da  strh   options+0x40                    0                  (short40)
0x0de  str    options+0x50                    0                  (int50)
0x0e2  strb   options+0x39                    0                  (flag39)
0x0e6  str    options+0x35                    0                  (int35)
0x0ea  str    options+0x31                    0                  (int31)
0x0ee  str    hints+0x04                      0                  (sec_slot1)
0x0f0  strb   hints+0x13                      0                  (sec_flag13)
0x0f2  strb   options+0x3f                    0                  (flag3f)
0x0f6  str    options+0x3b                    0                  (int3b)
0x0fa  strb   options+0x4e                    0                  (flag4e)
0x0fe  strh   options+0x4c                    0                  (short4c)
0x102  str    options+0x48                    0                  (int48)
0x104  str    hints+0x00                      0                  (sec_slot0)
0x10c  strb   Globals::gameLoaded             0
0x114  strb   Globals::gameSaving             0
0x118  str    Globals::lastSpaceMusicPlayed   -1
0x120  strb   Globals::initMemoryWarning      0
0x124  str    Globals::lastStationMusicPlayed -1
0x12e  str    Globals::lastCampaignMissionFailCount 0
0x138  str    Globals::lastCampaignMissionFailed    -1
0x16a  str    options+0x54                    0x247/0x33e/0x19f  (resWidth)
0x17e  str    options+0x58                    0x201/0x2da/0x16d  (resHeight)
0x18a  str    Globals::instantActionScore     0
0x192  str    Globals::instantActionWave      0
0x1a0  str    Globals::instantActionType      -1
0x1a8  str    Globals::instantActionPlayerName 0
0x1b0  str    Globals::items                  0
0x1b4  str    Globals::status                 0
0x1bc  strh   options+0x60                    0x0100             (short60)
0x1c0  str    Globals::galaxy                 0
0x1c6  str    Globals::achievements           0
0x1c8  str    Globals::imageFactory           0
0x1cc  str    Globals::ships                  0
0x1ce  str    this+0x4                        0                  (soundResources)
```

Notes on the reconstruction:

* The four floats are written by one `vst1.32 {d16-d17}` at `0x0b2`; the
  `strd` pairs are `colorR/colorG` and `tintB/brightness`. A naive
  store-by-store tracer therefore sees only 51 of the 55 stores; the table
  above expands the `vst1`/`strd` forms.
* `options+0x0c` is the 16-bit `0x0101` volume pair; `options+0x10` is the
  separate 16-bit zero.
* The `-1` stores are `lastSpaceMusicPlayed`, `lastStationMusicPlayed`,
  `lastCampaignMissionFailed`, `instantActionType`.
* The original literal-pool order (machine order of the `ldr [pc]` loads) is:

```text
Canvas, lastSpace, lastStation, lastCampaignMissionFailed,
lastCampaignMissionFailCount, options, recordHandler, shipTemplate, gameText,
font, fontAlien, fontLangSelect, rnd, globals, layout, appManager, generator,
hints, gameLoaded, gameSaving, initMemoryWarning, imageFactory, achievements,
galaxy, instantActionScore, instantActionWave, status, instantActionType,
instantActionPlayerName, items, ships
```

The first 7 words are the prologue hoist. `movw r6,#583` (the `resWidth`
default) is at normalised index 34, immediately after `ldr.w ip,[r6]`
(index 33) and before the `shipTemplate` store (index 35).

## 3. Full store-order-as-source (new)

The w4708 pass reordered only the late block. This pass rewrote the **entire**
constructor body in the original machine store order above (early block,
settings block, float group, and late block), with the `settings`/`secondary`
declarations placed where their GOT loads occur in the original pool order.
That source compiles to the original's store order almost exactly (the `strd`
and `vst1` merges reproduce), but scores only **52.7**:

```text
_ZN7GlobalsC2Ev  pct 52.7  osize 612  usize 612  len_o 225  len_u 227
```

The reason is the same one w4708 identified: the scheduler picks a different
prologue hoist (`shipTemplate, lastSpace, instantActionWave, imageFactory,
ships, options, generator` instead of `Canvas, lastSpace, lastStation, lcmf,
lcmfc, options, recordHandler`), so the zero register becomes `r3` instead of
`r2` and every settings store mismatches on its register operand. Store order
and pool order cannot both be obtained from the same source order with this
statement set.

The exact body of that experiment is:

```cpp
Globals::Globals() {
    GameSettingsRecord *settings = (GameSettingsRecord *) Globals::options;
    Globals::Canvas = 0;
    Globals::shipTemplate = 0;
    Globals::gameText = 0;
    Globals::font = 0;
    Globals::fontAlien = 0;
    Globals::fontLangSelect = 0;
    Globals::rnd = 0;
    Globals::globals = 0;
    Globals::layout = 0;
    Globals::appManager = 0;
    Globals::recordHandler = 0;
    Globals::generator = 0;
    CtorSecondaryObject *secondary = (CtorSecondaryObject *) Globals::hints;

    this->field_0x0 = 0;

    settings->glowR = 0.5f;
    settings->glowG = 0.5f;
    settings->tintR = 0.6f;
    settings->tintG = 0.6f;
    reinterpret_cast<int16_t &>(settings->_region[0]) = 0x101;
    settings->colorR = 0.5f;
    settings->colorG = 0.5f;
    settings->colorB = 0.5f;
    settings->tintB = 0.5f;
    settings->brightness = 1.0f;
    settings->contrast = 0.5f;
    settings->_region[2] = 1;
    settings->enableFlag30 = 1;
    *reinterpret_cast<int16_t *>(reinterpret_cast<uint8_t *>(settings) + 0x10) = 0;
    reinterpret_cast<int16_t &>(settings->shortAt40) = 0;
    settings->intAt50 = 0;
    settings->flagAt39 = 0;
    reinterpret_cast<int32_t &>(settings->intAt35) = 0;
    reinterpret_cast<int32_t &>(settings->intAt31) = 0;
    reinterpret_cast<int32_t &>(secondary->slot1) = 0;
    secondary->flagAt13 = 0;
    settings->flagAt3f = 0;
    reinterpret_cast<int32_t &>(settings->intAt3b) = 0;
    settings->flagAt4e = 0;
    reinterpret_cast<int16_t &>(settings->shortAt4c) = 0;
    settings->intAt48 = 0;
    reinterpret_cast<int32_t &>(secondary->slot0) = 0;

    Globals::gameLoaded = 0;
    Globals::gameSaving = 0;
    Globals::lastSpaceMusicPlayed = -1;
    Globals::initMemoryWarning = 0;
    Globals::lastStationMusicPlayed = -1;
    Globals::lastCampaignMissionFailCount = 0;
    Globals::lastCampaignMissionFailed = -1;

    float fv = settings->qualityLevel;
    int v54 = 0x247;
    if (1.0f <= fv) v54 = 0x33e;
    if (fv <= 0.0f) v54 = 0x19f;
    settings->resWidth = v54;
    int v58 = 0x201;
    if (1.0f <= fv) v58 = 0x2da;
    if (fv <= 0.0f) v58 = 0x16d;
    settings->resHeight = v58;

    Globals::instantActionScore = 0;
    Globals::instantActionWave = 0;
    Globals::instantActionType = -1;
    Globals::instantActionPlayerName = 0;
    Globals::items = 0;
    Globals::status = 0;
    reinterpret_cast<int16_t &>(settings->shortAt60) = 0x100;
    Globals::galaxy = 0;
    Globals::achievements = 0;
    Globals::imageFactory = 0;
    Globals::ships = 0;
    this->soundResources = 0;
}
```

## 4. Joint settings-tail × late-block search (new)

All variants were compiled with the exact repo compile line (NDK r18b clang
7.0.2, `-target armv7-none-linux-androideabi16 -march=armv7-a -mthumb
-mfpu=neon -mfloat-abi=softfp -std=c++11 -fPIC -frtti
-fstack-protector-strong -Oz -g -stdlib=libc++`) and scored with the official
`tools/verify/verify.py` normalisation (`autojunk=False`). A minimal single-TU
harness (the anonymous-namespace structs + the constructor only) reproduces
the baseline 80.5 / 227 / 225 exactly and evaluates in ~0.05 s.

| search | space | best |
|---|---|---:|
| baseline | — | **80.5** |
| full store-order-as-source (all 55 stores) | 1 | 52.7 |
| simulated annealing, late block, 8 000 moves | 8 000 | **80.5** |
| random **three-move**, whole 60-token sequence | 20 000 | **80.5** |
| exhaustive **two-move**, 14-token early block | 4 095 | **80.5** |
| single-move hill-climb, 27-token settings block | full | **80.5** |
| single-move hill-climb, 14-token early block | full | **80.5** |
| single-move hill-climb, 20-token late block (WIN early) | full | 78.8 |
| complete single-move late-block neighbourhood (baseline early; w4410) | 342 | **80.5** |
| WIN early + quality block pinned at late position 12 + hill-climb | full | 78.8 |
| WIN early + baseline late + `status`@10 | 1 | 78.8 |
| WIN early + WIN late (quality at pos 12) | 1 | 78.3 |
| WIN early + hill-climbed late (quality free) | full | 78.8 |
| WIN early + hill-climbed late (quality pinned at 12) | full | 78.8 |

The baseline is therefore a local optimum under every neighbourhood measured
in this pass, on top of w4410's complete single-move neighbourhood (1 892),
w4510's complete two-move late-block neighbourhood (46 838), w4708's 8!
settings-tail permutation (40 320) and 50 000 full-sequence random samples.

## 5. The two `ip` forms

The original keeps `&Globals::imageFactory` in `ip` across the whole quality
block:

```text
0x128  ldr.w ip,[r0]      ; ip = Globals::imageFactory
...    quality block ...
0x1c8  str.w r2,[ip]      ; Globals::imageFactory = 0
```

The baseline stores it immediately:

```text
0x120  ldr r0,[r0]        ; r0 = Globals::imageFactory
0x126  str r2,[r0]        ; Globals::imageFactory = 0
```

The two promotions are +4 bytes; the baseline pads the function with two
`nop`s to keep the pool 4-aligned at 612 B. The original's `ip` choice is a
register-allocation outcome: at the original's load point `r0` is busy with
`movw r0,#513` (the `resHeight` default), while in ours the load is scheduled
before the quality block, when `r0` is free. This pass tried, in addition to
the w4708 forms:

* `ImageFactory **pif = &Globals::imageFactory;` declared at the top, before
  the quality block, before the late block, and at the end, with `*pif = 0;`
  at every late-block position;
* `ImageFactory *volatile *pif`, `ImageFactory **const pif`, and
  `ImageFactory *&pif = Globals::imageFactory;`;
* every one of the 19 positions of `Globals::imageFactory = 0;` in the
  baseline late block (with the baseline early/settings blocks) — all 19
  compile to the same two `ip` instructions (the `lcmfc` load/store only) and
  the `imageFactory` load/store is always `ldr r0,[r0]` / `str r2,[r0]`.

No source shape found this pass moves the `imageFactory` store past the
quality block while keeping its GOT load at pool slot 21.

## 6. What this proves

1. **The original member-init order is now fully reconstructed** (§2): 55
   stores, with widths and values, including the two `strd` pairs and the
   `vst1.32` float group.
2. **The baseline is a genuine local optimum of the fuzzy metric** under the
   joint settings-tail × late-block neighbourhood, not just along each axis:
   simulated annealing (8 000), random three-move whole-sequence (20 000),
   exhaustive two-move early block (4 095), and complete single-move
   hill-climbs of all three blocks all top out at **80.5**.
3. **Store order and pool order are mutually exclusive at the source level.**
   Writing the whole body in the original store order reproduces the store
   sequence but changes the prologue hoist (and therefore the zero register),
   scoring 52.7. The WIN early block reproduces the pool (31/31) and the
   `movw r6` hoist but caps at 78.8.
4. **The remaining code-size delta is exactly the two `imageFactory` `ip`
   promotions** (4 bytes), which are a register-allocation outcome. Recovering
   them is worth 80.5 → 80.9 with no change in the gate counts; recovering the
   `movw r6` hoist as well is worth 80.5 → 81.0.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5008
cmake --build cmake-build-match --target check -j8
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN7GlobalsC2Ev
```

Scratch harness (not tracked, `/dev/shm/w5008`): `ev.py` (minimal single-TU
scorer, ~0.05 s/eval), `trace2.py` (original store tracer with constant
tracking), `tokens.py` (statement token sets), `search.py` / `sa.py` /
`rnd3.py` / `climb*.py` (the searches above), `body_origorder.txt` (the full
store-order source), `body_win_bestguess.txt` (WIN early + 78.8 late),
`body_climb_fixedq_best.txt` (quality-pinned WIN hill-climb).

## 8. What remains

The function cannot be improved by statement reordering. The two precise
targets are unchanged from w4708, now with the full store-order reconstruction
as evidence:

1. **Recover the two `ip` 32-bit forms** for `&Globals::imageFactory`
   (80.5 → 80.9). It needs a source shape that makes the scheduler defer the
   `imageFactory` store past the quality block while keeping its GOT load at
   pool slot 21. Every pointer-local and every late-block position measured
   this pass folds the load to the store.
2. **Recover the `movw r6,#583` hoist** at index 34 (80.5 → 81.0). It is the
   register allocator reusing `r6` the instant it dies at `ldr.w ip,[r6]`; it
   requires `v54`'s live interval to start there. The WIN early block is the
   only measured shape that produces it, and its late block caps at 78.8.
