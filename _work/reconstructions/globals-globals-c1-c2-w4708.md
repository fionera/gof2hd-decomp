# Globals::Globals C1/C2 (80.5%, 612 B) — w4708 (wave 97)

Status: research note, 2026-11-03. Worktree `wt/w4708`, branch `wave97-w4708`,
base `9b58a39d5`. **No source change landed** — the w3808..w4510 baseline source
order is still the best measured shape (80.5%). This commit contains only this
note. Everything below is self-contained; no transcript is required.

## TL;DR

* Baseline and final: **`_ZN7GlobalsC1Ev` / `_ZN7GlobalsC2Ev` = 80.5%, 612 B**
  (size-exact; 225 vs 227 normalised entries, M = 182 matched,
  `2·182/452 = 0.8053097345132744`).
* Gate unchanged: **byte_exact 4500 / linked_exact 4500 / size-exact 4524,
  stubs 0, missing 0, wrong_type 0, extra 0, data 843/843, imports 207,
  lint CLEAN, drift 0, `RATCHET PASS`**.
* This pass attacked the three axes named in the brief with searches that had
  not been run before, and established a new, sharper fact:
  **the real instruction count of the original and of the baseline is
  identical (186 each). The entire remaining code-size delta is exactly two
  Thumb-16→Thumb-32 promotions** — `ldr r0,[r0]` → `ldr.w ip,[r0]` (the
  `&imageFactory` address) and `str r2,[r0]` → `str.w r2,[ip]` (its store) —
  which are worth 2 of the 4 trailing `nop` bytes. The mnemonic histograms
  differ in nothing else.
* The `movw r6,#583` hoist and the pool `status` slot are both *scheduling*
  outcomes, not source-order ones: they can be obtained (WIN early block) but
  only at the cost of a worse early-block instruction interleave, and the
  net is always negative (78.8 vs 80.5).
* New searches this pass (all negative):
  * full-sequence random search (early + late + quality position), 50 000
    samples: best **73.9**;
  * exhaustive 8! settings-tail permutation with the *baseline* early/late
    blocks, 40 320 samples: 330 permutations reach the maximum **80.5** (the
    baseline tail is one of them); the rest score 76.5-80.1. The tail's
    machine order is scheduler-determined but not invariant;
  * `status` at each of the 14 early-block positions: best 80.5 at position 13
    (the baseline); positions 3–12 give 80.1, 0–2 give 79.2;
  * `tintR` at each of the 14 early-block positions: invariant (80.5) — the
    compiler merges glowR/glowG/tintR/tintG into one `vld1.64`;
  * `imageFactory` at each of the 18 late positions × 19 quality positions
    (342 combos): best 80.5 at (4, 0–3); **no combination produces the
    `ip` form**;
  * quality block inserted at each of the 28 settings-block positions: best
    80.5 (positions 25–27), never `movw r6`;
  * seven quality-block source forms (v58-first, interleaved, direct stores,
    ternary, unsigned, declarations hoisted to the top, defaults stored
    early): all ≤ 80.5, never `movw r6`;
  * `ImageFactory **pif = &Globals::imageFactory; *pif = 0;` (declared at the
    top / before the quality block / `volatile` / `ImageFactory *volatile *` /
    `__atomic_store_n`): never the `ip` form;
  * store-order-as-source with four different early blocks: 46.9–49.1
    (store LCS 42–45, pool LCS 18–23);
  * the 330 maximum settings tails crossed with all 171 late-block single
    swaps (50 490 samples): maximum still 80.5;
  * all four `-1` stores grouped at the end of the late block: 58.8;
  * 20 000 random late orders with the WIN early block: 1071 produce the
    `movw r6,#583` hoist, best of those **73.5** (all at quality position
    12/13);
  * 20 000 random late orders filtered for the exact original prologue hoist
    (`Canvas,lastSpace,lastStation,lcmf,lcmfc,options,recordHandler`): only
    **4** hits, best **69.0**.

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

The baseline source (unchanged; `src/game/core/Globals.cpp`,
`Globals::Globals()`) is the w3808..w4410 order:

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

## 2. Why the score is what it is: the verifier masks every immediate

`tools/verify/verify.py::normalize()` ends with `HEX.sub("#A", insn)`, so every
hex operand — including every `ldr rX,[pc,#N]` pool offset and every
`str rX,[rY,#N]` field offset — is replaced by `#A` before comparison. The
score is therefore a match over **mnemonic + register-operand sequences**, plus
the literal-pool words (which are compared by resolved symbol name).

This is the single most important fact for this function. The PIC idiom

```text
ldr r0,[pc,#N] / add r0,pc / ldr r0,[r0] / str r2,[r0]
```

is *identical text* no matter which global it targets. That is why the
baseline's late block (whose **store** order is demonstrably wrong: `lastSpace`
first instead of third, `recordHandler` second instead of tenth) still scores
higher than the "store-order-correct" variants: the repeated idiom aligns even
when the symbols do not, and the symbols are only checked in the 31 pool words.

## 3. Ground truth: the original 55 stores and 31 pool words

`_ZN7GlobalsC2Ev` @ `0xe31e0`, 612 B, literal pool @ `0xe33c8`
(31 `R_ARM_GOT_PREL` words + 4 float words). Normalised stream length 225
(186 code + 8 float-word entries + 31 pool entries).

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

## 4. The decisive new measurement: the instruction histograms

Counting normalised entries by mnemonic over the whole stream:

```text
mnemonic   original   baseline   delta
ldr              59         60      +1
ldr.w             5          4      -1
str              29         30      +1
str.w            10          9      -1
nop               0          2      +2
total           225        227      +2
```

Every other mnemonic count is identical. In particular both sides have the
same 186 real code instructions and the same 7 `movw`s. The **only** real
instruction-identity differences are:

1. `ldr r0,[r0]` (ours, index 120) vs `ldr.w ip,[r0]` (original, index 128)
   — the `&Globals::imageFactory` address load;
2. `str r2,[r0]` (ours, index 121) vs `str.w r2,[ip]` (original, index 181)
   — the `imageFactory` store.

Each promotion is +2 bytes. Ours is 468 B of code and the original 472 B;
the 4-byte gap is exactly these two promotions, and clang pads the function
with two `nop`s (indices 186, 187) to keep the literal pool 4-aligned at 612 B.
If the two `ip` forms were recovered, `len_u` would drop 227 → 225 and the
score would be `2·182/450 = 80.9` with M unchanged.

The original keeps `&imageFactory` in `ip` across the whole quality block
(`0x128` → `0x1c8`); ours stores it immediately in `r0`. This is a
register-allocation outcome: at the original's load point `r0` is busy with
`movw r0,#513` (the `resHeight` default), so the allocator picks `ip`; in ours
the load is scheduled before the quality block, when `r0` is free.

The second difference, `movw r6,#583` at original index 34 (immediately after
`ldr.w ip,[r6]`, i.e. the moment `r6` dies) vs `movw r0,#583` at our index 139
(inside the quality block), is the same phenomenon: the original's `v54`
live interval starts at 34 and spans the quality block, so the allocator gives
it the callee-saved `r6`; ours starts at 139 and takes `r0`. It is worth one
matched instruction (`M` 182 → 183, score 81.0) if it can be recovered.

## 5. Searches run this pass (all negative)

All variants were compiled with the exact repo compile line (NDK r18b clang
7.0.2, `-target armv7-none-linux-androideabi16 -march=armv7-a -mthumb
-mfpu=neon -mfloat-abi=softfp -std=c++11 -fPIC -frtti
-fstack-protector-strong -Oz -g -stdlib=libc++`) and scored with the official
`tools/verify/verify.py` normalisation (`autojunk=False`). The fast single-TU
scorer reproduces 80.5 / 227 / 225 exactly.

### 5.1 Exhaustive settings-tail permutation, baseline blocks (new)

All **8! = 40 320** permutations of the 8-token tail
`int3b flag4e short4c int48 sec_slot1 sec_flag13 sec_slot0 flag3f` were
measured with the *baseline* early and late blocks (w4510 did this with the
WIN blocks). The score distribution is:

```text
80.5 x 330    80.1 x 592    79.6 x 2802   79.2 x 6494   78.8 x 7540
78.3 x 12364  77.9 x 6168   77.4 x 2106   77.0 x 1888   76.5 x 36
```

The maximum is 80.5 (M = 182), reached by 330 permutations including the
baseline tail; no permutation exceeds it. The tail's machine order is
scheduler-determined but **not** invariant under source permutation — w4510's
"all 40 320 collapse to 78.3" was measured at the WIN local optimum and does
not hold at the baseline.

The 330 maximum tails were then crossed with the full late-block single-swap
neighbourhood (**330 × 171 = 50 490** evaluations): the maximum is still
**80.5 / M = 182**. So the baseline is a strict local optimum in the joint
(settings-tail × late-block) space as well, not just along each axis.

### 5.2 Full-sequence random search (new)

50 000 uniformly random (early permutation × late permutation × quality
position) samples with the settings block fixed: best **73.9** (M = 167).
The baseline is far outside the random cloud; no random draw came near 80.

### 5.3 `status` position in the early block (new)

`status` at each of the 14 early-block positions, baseline late block:

```text
pos 0-2   -> 79.2 (pool LCS 27-28)
pos 3-12  -> 80.1 (pool LCS 29; prologue hoist ends ...options,generator,recordHandler)
pos 13    -> 80.5 (pool LCS 30; prologue hoist ends ...options,status,recordHandler)
```

Position 13 is the baseline. No position gives the original's 31/31 pool
while keeping the baseline's early-block interleave.

### 5.4 `tintR` position in the early block (new)

All 14 positions score exactly 80.5. The four floats
`glowR/glowG/tintR/tintG` are merged into one `vld1.64 {d16-d17}` regardless of
where `tintR` appears in the source.

### 5.5 `imageFactory` position × quality position (new)

All 18 late positions × 19 quality positions (342 combinations). Best 80.5 at
`(imageFactory index 4, qpos 0-3)` — the baseline. **No combination produces
`str.w r2,[ip]` twice** (the second `ip` form never appears).

### 5.6 Quality-block placement inside the settings block (new)

Inserting the quality block at each of the 28 settings-block positions: best
80.5 at positions 25-27 (i.e. after the settings block, as today). Never
`movw r6`.

### 5.7 Quality-block source forms (new)

Seven forms — `v58` computed first, interleaved `if`s, direct stores to
`settings->resWidth/resHeight`, nested ternary, `unsigned`, declarations
hoisted to the top of the function, defaults stored before the `if`s. Four
compile to the baseline code (80.5); the others score 79.2 (interleaved),
79.6 (v58-first) and 72.1 (ternary). **None produces `movw r6,#583`.**

### 5.8 `ip`-form attempts (new)

`ImageFactory **pif = &Globals::imageFactory; ... *pif = 0;` with the pointer
declared (a) at the top of the function, (b) immediately before the quality
block, (c) as `volatile`, (d) as `ImageFactory *volatile *`, (e) as an
`__atomic_store_n` — all fold the load to the store and emit `str r2,[r0]`.
The `str.w r2,[ip]` count stays at 1 (the `lcmfc` store).

### 5.9 Store-order-as-source (new, with four early blocks)

Writing the late block in the original's exact store order
(`gameLoaded, gameSaving, lastSpace, initMem, lastStation, lcmfc, lcmf,
QUALITY, ias, iaw, iat, iapn, items, status, short60, galaxy, achievements,
imageFactory, ships, soundRes`) reproduces the original's store order exactly
(store LCS 45/55) but scores only **46.9–49.1**, because the DAG scheduler then
picks a different prologue hoist (`Canvas, lastSpace, achievements,
imageFactory, ships, options, recordHandler`) and a different zero register
(`r1` instead of `r2`), so the whole instruction stream misaligns. Four
different early blocks give the same 46.9–49.1.

### 5.10 Other measured variants (new)

| variant | pct | note |
|---|---:|---|
| baseline | **80.5** | reference |
| WIN early (recordHandler last) + status@9 late | 78.8 | pool 31/31, early interleave loses 5 |
| WIN early + original settings tail | 77.4 | — |
| baseline + original settings tail | 79.2 | tail LCS 22→24, code loses 3 |
| all four `-1` stores grouped at end | 58.8 | store LCS 37, pool LCS 18 |
| quality block first in the late block (q=0..3) | 80.5 | baseline |
| quality block after `initMem` (q=4..10) | 79.2 | — |
| quality block after `lcmf` (q=15) | 72.6 | `ldr.w ip` moves to `r5` |
| duplicate `status` store in late block | 77.4 | — |
| `__atomic_store_n(&imageFactory, 0, relaxed)` | 75.2 | — |

### 5.11 Hoist searches (new)

* 20 000 random late orders with the WIN early block: **1071** contain
  `movw r6,#583` in the first 60 instructions, but every one has the quality
  block at position 12 or 13; best **73.5**.
* 20 000 random late orders with the WIN early block filtered for the exact
  original prologue hoist: only **4** hits, best **69.0**.

## 6. What this proves

1. **The baseline is a genuine local optimum of the fuzzy metric** under every
   neighbourhood measured so far: w4410's complete single-move (1 892) and
   two-move (46 838) late-block searches, w4510's 8! settings-tail permutation,
   and this pass's 50 000 full-sequence random samples, 342 `imageFactory`×
   quality placements, 28 quality-block placements, 14 `status` positions and
   14 `tintR` positions.
2. **The settings tail and the prologue hoist are scheduler properties, not
   source-order properties.** The tail's 40 320 source permutations collapse
   onto only 10 distinct scores (330 of them at the 80.5 maximum); the
   prologue hoist is chosen by the DAG scheduler from the whole function
   shape.
3. **The remaining code-size delta is exactly two Thumb-16→Thumb-32
   promotions** (`ldr.w ip,[r0]`, `str.w r2,[ip]` for `&imageFactory`). The
   real instruction count is already identical (186). Recovering them is worth
   80.5 → 80.9. Recovering the `movw r6,#583` hoist is worth 80.5 → 81.0.
4. **The two local optima are mutually exclusive at the source level.** The
   WIN early block is the only measured shape with the exact 31/31 pool and
   the `movw r6` hoist, but its early-block instruction interleave loses 5
   matches; the baseline's early block is 97/98 (only the missing `movw r6`)
   but has `status` at pool slot 6. Combining them would give
   `M = 96 + 49 + 39 = 184 → 81.4`; no source order found in three passes
   achieves the combination.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w4708
cmake --build cmake-build-match --target check -j8
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN7GlobalsC2Ev
```

Scratch harness (not tracked, `/dev/shm/w4708`): `ev.py` (single-TU scorer,
0.35 s/eval), `gen.py` / `gen2.py` (token→statement body generators),
`trace.py` (PIC store tracer), `probe.py` (score + pool + store table),
`exp.py` (pool/store LCS helpers), `blocks.py` / `showblock.py` (matching-block
dump), `dump_orig.py` / `dump_ours.py` (indexed disassembly), `sweep_if.py`,
`sweep_ip.py`, `sweep_win.py`, `sweep_q.py`, `tailperm_base2.py`,
`hoist_win.py`, `poolsearch.py`, `randfull.py`, `climb_from.py`,
`qforms.py`.

## 8. What remains

The function cannot be improved by statement reordering. The two precise
targets are unchanged from w4510, now with sharper evidence:

1. **Recover the two `ip` 32-bit forms** (`ldr.w ip,[r0]` for `&imageFactory`,
   `str.w r2,[ip]` for its store). This is the only route to removing the two
   `nop`s and it alone is worth 80.5 → 80.9. It needs a source shape that
   makes the scheduler defer the `imageFactory` store past the quality block
   (so `r0` is busy with `movw r0,#513` at the load point) while keeping its
   GOT load at pool slot 21. Moving the statement later in source moves the
   GOT load too (pool 6/31 measured), so this is a register-pressure problem,
   not an ordering one.
2. **Recover the `movw r6,#583` hoist at index 34.** It is the register
   allocator reusing `r6` the instant it dies at `ldr.w ip,[r6]`; it requires
   `v54`'s live interval to start there. Worth 80.5 → 81.0.
3. **Combine the WIN early block with a 49/90 late block.** The WIN early
   order is the only one with the exact 31/31 pool; the missing piece is a
   late-block IR shape that keeps that prologue/pool while aligning the
   zero-init group to the original's indices. All 40 320 tail permutations,
   46 838 two-move reorders, 50 000 full-sequence random samples and the 342
   `imageFactory`×quality placements leave this open.
