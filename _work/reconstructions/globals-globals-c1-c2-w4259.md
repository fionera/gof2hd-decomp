# Globals::Globals C1/C2 (80.5%, 612 B) — w4259 (wave 92)

Status: research note, 2026-11-03. Worktree `wt/w4259`, branch `wave92-w4259`,
base `b379d00d`, HEAD before this pass `e3379c7c`. **No source change landed** —
the w3808/w3827/w4008/w4108/w4224 baseline source order is still the best
measured shape (80.5%). This commit contains only this note. Everything below
is self-contained; no transcript is required.

## TL;DR

* Baseline and final: **`_ZN7GlobalsC1Ev` / `_ZN7GlobalsC2Ev` = 80.5%, 612 B**
  (size-exact; 225 vs 227 normalised instructions, 182 matched).
* Gate unchanged: **byte_exact 4499 / linked_exact 4499 / size-exact 4524,
  stubs 0, missing 0, wrong_type 0, extra 0, data 843/843, imports 207,
  .text 0x1337c8, .rodata 0x41b67, .ARM.exidx 0x6f70, .ARM.extab 0x26298,
  lint CLEAN, drift 0, `RATCHET PASS`**. (The wave-92 prompt quoted 4498; the
  actual repo baseline is 4499 — `StarMap::draw` pass 93 landed the extra one.)
* The w4224 `WIN` source order still reproduces the original's **31/31 GOT
  pool** *and* the **`movw r6,#583` hoist**, but scores 78.3% because its late
  zero-init store schedule matches the original's worse under difflib.
* **New this pass:** the 2-instruction gap between ours (227) and the original
  (225) is exactly the two `nop`s the assembler inserts to 8-align the literal
  pool. Our real code is 468 B; the original's is 472 B. The pool is 140 B in
  both (31 GOT words + 4 float words), so the function is 612 B in both only
  because ours spends 4 B on padding. Eliminating the padding requires **4 more
  bytes of real code** (most faithfully the missing `movw r6,#583`); if that
  were achieved the fuzzy would jump to ~80.9 (M unchanged) or ~81.8 (M + 2 for
  the aligned float words, + 1 for the hoist). This is the sharpest remaining
  target.
* This pass also re-verified the quality-block source form (`fv >= 1.0f` vs
  `1.0f <= fv`), ran a full 190-pair swap sweep and a 19-position quality sweep
  from the baseline, and tried the original *store* order as source. Nothing
  beats 80.5.

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

`C1` is a plain alias of `C2` in the shipped `.so` and in ours; both score the
same, so every experiment below was scored on `C2`.

Exact score: `difflib.SequenceMatcher(autojunk=False).ratio()` over the
normalised streams is **0.8053097345132744** (M = 182, 2*182/(225+227)).
One more matched instruction would give 2*183/452 = 0.80973 → **81.0**, so the
gap to a strict improvement is a single instruction.

## 2. Ground truth: original machine code

Original `_ZN7GlobalsC2Ev` @ `0xe31e0`, 612 B, literal-pool base `0xe33c8`,
31 `R_ARM_GOT_PREL` slots. The pool is 140 B: 31 GOT words + the four float
constants (`0.6f, 0.6f, 0.5f, 0.5f`) loaded by `vld1.64 {d16-d17}`.

Pool order (= order of first GOT load in the final machine code, which is the
SelectionDAG `list-hybrid` order, **not** the store order):

```text
Canvas, lastSpace, lastStation, lastCampaignFailed, lastCampaignFailCount,
options, recordHandler, shipTemplate, gameText, font, fontAlien,
fontLangSelect, rnd, globals, layout, appManager, generator, hints,
gameLoaded, gameSaving, initMemoryWarning, imageFactory, achievements,
galaxy, instantActionScore, instantActionWave, status, instantActionType,
instantActionPlayerName, items, ships
```

Full annotated store order (post-RA machine code; `r1` = settings, `r2` = 0,
`r5` = -1, `r4` = 0.5f). This is the *address* order; the note in w4224 listed
`galaxy` before `ias`, which was a transcription slip — the `galaxy` store is
at `0x1c0`, after `shortAt60` at `0x1bc`:

```text
0x10c  strb  r2,[r0]           gameLoaded = 0
0x114  strb  r2,[r0]           gameSaving = 0
0x118  str.w r5,[r8]           lastSpace = -1
0x120  strb  r2,[r0]           initMemoryWarning = 0
0x124  str   r5,[r3]           lastStation = -1
0x12e  str.w r2,[ip]           lastCampaignMissionFailCount = 0
0x138  str.w r5,[lr]           lastCampaignMissionFailed = -1
0x12a..0x17e                   quality block (see below)
0x18a  str   r2,[r0]           instantActionScore = 0
0x192  str   r2,[r0]           instantActionWave = 0
0x1a0  str   r5,[r3]           instantActionType = -1
0x1a8  str   r2,[r3]           instantActionPlayerName = 0
0x1b0  str   r2,[r3]           items = 0
0x1b4  str   r2,[r0]           status = 0
0x1bc  strh.w r0,[r1,#96]      shortAt60 = 0x100
0x1c0  str   r2,[r6]           galaxy = 0
0x1c6  str   r2,[r4]           achievements = 0
0x1c8  str.w r2,[ip]           imageFactory = 0
0x1cc  str   r2,[r3]           ships = 0
0x1ce  str.w r2,[r9,#4]        soundResources = 0
```

The early block (before `gameLoaded`) is:

```text
Canvas, shipTemplate, gameText, font, fontAlien, fontLangSelect, rnd, globals,
layout, appManager, recordHandler, generator, this+0, options+0xc, options+0,
options+8, options+0x24, options+0x2c, options+0xe, options+0x30, options+0x10,
options+0x40, options+0x50, options+0x39, options+0x35, options+0x31,
hints+4, hints+0x13, options+0x3f, options+0x3b, options+0x4e, options+0x4c,
options+0x48, hints+0
```

The quality block (note the unconditional default, then `ge`/`ls` overrides):

```text
0x12a  vldr  s2,[r1,#68]      fv = settings->qualityLevel
0x134  vcmpe.f32 s2,s0        fv vs 1.0
0x140  vcmpe.f32 s2,#0.0      fv vs 0.0
0x148  movw  r0,#513          v58 default
0x150  movwge r6,#830         v54 high
0x15e  movwls r6,#415         v54 low
0x16a  str   r6,[r1,#84]      resWidth = v54
0x16e  movwge r0,#730         v58 high
0x178  movwls r0,#365         v58 low
0x17e  str   r0,[r1,#88]      resHeight = v58
```

`movw r6,#583` is at `0xe3236`, immediately after `ldr.w ip,[r6]`
(`0xe3232`, the last use of the prologue GOT temp in `r6`) and immediately
before the `shipTemplate` store (`0xe323a`). The `resWidth` default is live in
`r6` from `0xe3236` to `0xe334a` (~69 instructions).

## 3. Baseline machine code

The baseline source order (unchanged by this pass) is:

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
/* ... settings block ... */
Globals::lastSpaceMusicPlayed = -1;
Globals::gameLoaded = 0;
Globals::gameSaving = 0;
/* ... quality block ... */
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

Baseline pool order (31 entries, `status` at slot 6 instead of 26):

```text
Canvas, lastSpace, lastStation, lastCampaignFailed, lastCampaignFailCount,
options, status, recordHandler, shipTemplate, gameText, font, fontAlien,
fontLangSelect, rnd, globals, layout, appManager, generator, hints,
gameLoaded, gameSaving, initMemoryWarning, imageFactory, achievements,
galaxy, instantActionScore, instantActionWave, instantActionType,
instantActionPlayerName, items, ships
```

Baseline store order:

```text
lastSpace, gameLoaded, gameSaving, initMemoryWarning, imageFactory,
achievements, galaxy, lastStation, instantActionScore, [quality],
options+0x48, options+0x3f, instantActionType, instantActionPlayerName,
items, instantActionWave, lastCampaignMissionFailed, ships, soundResources,
shortAt60, lastCampaignMissionFailCount
```

Baseline quality block (already `movwge`/`movwls`; the default is `r0`, not
`r6`, and the two select chains are sequential, not interleaved):

```text
0x152  movw  r0,#583          v54 default
0x158  movwge r0,#830         v54 high
0x166  movwls r0,#415         v54 low
0x16e  str   r0,[r1,#84]      resWidth
0x170  movw  r0,#513          v58 default
0x17a  movwge r0,#730         v58 high
0x184  movwls r0,#365         v58 low
0x18a  str   r0,[r1,#88]      resHeight
```

Baseline matching blocks against the original (32 blocks, M = 182):

```text
O[0:34]   U[0:34]   size 34   (whole prologue)
O[35:96]  U[38:99]  size 61   (settings block)
O[96:98]  U[101:103] size 2
O[101:102] U[103:104] size 1
O[103:104] U[104:105] size 1
O[104:113] U[106:115] size 9
O[114:117] U[115:118] size 3
O[117:118] U[122:123] size 1
O[118:119] U[133:134] size 1
O[124:125] U[134:135] size 1
O[126:127] U[135:136] size 1
O[127:128] U[137:138] size 1
O[131:132] U[140:141] size 1
O[133:136] U[142:145] size 3
O[137:138] U[146:147] size 1
O[138:139] U[149:150] size 1
O[140:145] U[150:155] size 5
O[146:148] U[156:158] size 2
O[149:150] U[160:161] size 1
O[151:152] U[163:164] size 1
O[154:157] U[166:169] size 3
O[159:160] U[170:171] size 1
O[161:162] U[171:172] size 1
O[167:168] U[174:175] size 1
O[173:175] U[178:180] size 2
O[176:177] U[181:182] size 1
O[178:179] U[182:183] size 1
O[181:182] U[183:184] size 1
O[184:186] U[184:186] size 2
O[186:206] U[188:208] size 20   (float words + first GOT entries)
O[207:208] U[209:210] size 1
O[209:210] U[211:212] size 1
O[211:212] U[213:214] size 1
O[213:214] U[215:216] size 1
O[215:218] U[217:220] size 3
O[219:222] U[225:228] size 3
O[223:224] U[229:230] size 1
O[225:226] U[231:232] size 1
O[229:234] U[233:238] size 5
O[235:236] U[239:240] size 1
O[237:238] U[241:242] size 1
O[239:240] U[243:244] size 1
O[241:244] U[249:252] size 3
O[245:246] U[253:254] size 1
O[247:248] U[255:256] size 1
O[249:250] U[257:258] size 1
```

## 4. The `WIN` source order (w4224), reproduced

```cpp
shipTemplate, Canvas, gameText, font, fontAlien, fontLangSelect, rnd, globals,
tintR, layout, appManager, generator, recordHandler, field_0x0, [settings],
lastSpace, gameLoaded, gameSaving, initMem, imageFactory, achievements,
galaxy, ias, iaw, lastStation, lcmf, lcmfc, [quality], items, ships, iapn,
iat, status, soundRes, short60
```

Measurements: **pct 78.3, size 612/612, pool 31/31, `movw r6` @0x56,
store match 41/55, first 96 normalised instructions equal**. With
`soundRes` and `short60` swapped: **78.8, pool 31/31, `movw r6`, store match
42/55** — the best WIN-family variant, still below 80.5.

`WIN`'s late store order:

```text
lastSpace, gameLoaded, gameSaving, initMem, imageFactory, achievements,
galaxy, ias, iaw, lastStation, lcmf, lcmfc, [quality], items, ships, iapn,
iat, status, soundRes, short60
```

The original's late store order is:

```text
gameLoaded, gameSaving, lastSpace, initMem, lastStation, lcmfc, lcmf,
[quality], ias, iaw, iat, iapn, items, status, short60, galaxy,
achievements, imageFactory, ships, soundRes
```

The baseline's (status-early) late block aligns better with the original's
under difflib's greedy matching (49 matched instructions in the late region vs
WIN's 41), which more than offsets WIN's gains in the prologue (+1) and pool
(+1).

## 5. New evidence this pass

### 5.1 The 2-instruction gap is pool-alignment padding

`verify` normalises the function into 225 (orig) / 227 (ours) entries:

```text
orig: 186 code + 8 float-word entries + 31 pool:pic:got: entries = 225
ours: 188 code + 8 float-word entries + 31 pool:pic:got: entries = 227
```

The 8 float-word entries are the four `vld1.64` constants decoded as Thumb-16
(`movs r0,r0` / `subs r7,#0`), because `verify`'s `PCREL_LOAD` regex matches
`vldr` but not `vld1.64`. They are identical on both sides.

The two extra code entries are `nop` at `0x1d4` and `0x1d6`:

```text
orig: pop {r4,r5,r6,r7,pc} @0x1d6 ; pool @0x1d8
ours: pop {r4,r5,r6,r7,pc} @0x1d2 ; nop @0x1d4 ; nop @0x1d6 ; pool @0x1d8
```

The literal pool contains the `vld1.64` constants and is therefore 8-byte
aligned. Our real code ends at `0x1d4` (468 B, 4 mod 8); the assembler pads 4 B
to `0x1d8`. The original's real code ends at `0x1d8` (472 B) and needs no
padding. Both functions are 612 B because ours spends 4 B on the two `nop`s.

Consequence: **removing the `nop`s requires 4 more bytes of real code**, not
just a reordering. If that were achieved:

* with M unchanged (182) and `len_u` 225: 2*182/450 = 80.9%;
* with the float words then aligning at their original indices (+2) and a
  matched `movw r6,#583` (+1): M = 185, `len_u` = 226 → 82.0%.

The most faithful way to add those 4 bytes is the original's `movw r6,#583`
hoist (a 4-byte instruction that the baseline lacks at that point). That is
exactly the `WIN` property, but `WIN` pays for it with a worse late schedule.

A `nop`-free variant was seen only once: a quality form using
`if (fv <= 0.0f) { ... } else if (1.0f <= fv) { ... }` produced
`len_o/len_u = 225/225` (no padding), but M collapsed to 103 (pct 45.8), so it
is not useful.

### 5.2 The quality-block source form is already faithful

The original's quality block uses an unconditional default followed by
`movwge`/`movwls` overrides. The baseline already emits exactly that pattern
(`movw r0,#583; it ge; movwge r0,#830; it ls; movwls r0,#415; ...`). Changing
the source conditions from `1.0f <= fv` to `fv >= 1.0f` produces **identical
machine code and the identical 80.5% score** (the compiler canonicalises both).
The remaining quality-block differences are:

1. the register: ours uses `r0` for the `resWidth` chain, the original uses
   `r6` (callee-saved);
2. the interleaving: the original computes the `resHeight` default
   (`movw r0,#513`) before the `resWidth` select and interleaves the two
   chains; ours computes the `resWidth` chain first and then the `resHeight`
   chain.

Both are RA/scheduler decisions, not source-order decisions. Rewriting the
quality block as two live variables (`int v54, v58;` with both defaults first,
or with a shared `if` body) either canonicalises back to the baseline or
regresses (79.2 / 72.1 / 45.8 measured).

### 5.3 Search coverage this pass

All variants were built with the exact repo compile line (NDK r18b clang 7.0.2,
`-target armv7-none-linux-androideabi16 -march=armv7-a -mthumb -mfpu=neon
-mfloat-abi=softfp -std=c++11 -fPIC -frtti -fstack-protector-strong -Oz -g
-stdlib=libc++`) and scored with the official verifier
(`tools/verify/verify.py`, `autojunk=False`).

| variant | pct | M | len | pool | `movw` 583 | store match |
|---|---:|---:|---:|---:|---|---:|
| **baseline** | **80.5** | **182** | **225/227** | 30/31 | `r0` @0x152 | 40/55 |
| `WIN` | 78.3 | 177 | 225/227 | **31/31** | **`r6`** @0x56 | 41/55 |
| `WIN` + swap soundRes/short60 | 78.8 | 178 | 225/227 | **31/31** | **`r6`** | 42/55 |
| quality `fv >= 1.0f` (QA) | 80.5 | 182 | 225/227 | 30/31 | `r0` | 40/55 |
| QA, all 190 late-block pair swaps | ≤80.5 | ≤182 | 225/227 | — | — | — |
| QA, quality at positions 0–18 | ≤80.5 | ≤182 | 225/227 | — | — | — |
| quality after `ias` (positions 12–14) | 71.7–73.0 | 162–165 | 225/227 | 23–24/31 | **`r6`** | 37–38/55 |
| original *store* order as source | 46.9 | 106 | 225/227 | 18/31 | `r0` | 44/55 |
| WIN early + original-store-order late | 49.1–54.4 | 111–123 | 225/227 | 23/31 | `r0` | 45–46/55 |
| WIN early + baseline late | 76.1–76.5 | 172–173 | 225/227 | 28/31 | **`r6`** | 38–39/55 |
| baseline + trio (imageFactory/achievements/galaxy) after quality | 80.1 | 181 | 225/227 | 28/31 | `r0` | 40/55 |
| baseline + trio after short60 | 54.1 | 121 | 225/**222** | 22/31 | `r0` | 38/55 |
| `v58` declared/computed first (R1–R3) | 80.5 | 182 | 225/227 | 30/31 | `r0` | 40/55 |
| shared `if` body for v54/v58 (R4/QC) | 79.2 | 179 | 225/227 | 30/31 | `r0` | 40/55 |
| `if/else if` quality (Q5/QD) | 45.8–47.6 | 103–107 | 225/225 | 22/31 | `r0` | 36/55 |

The 190-pair sweep was the full set of pairwise swaps of the baseline late
block with the QA quality form; the best results were the baseline itself and
three score-neutral swaps (`lastSpace<->imageFactory`,
`gameSaving<->QUALITY`, `iat<->iaw`). The 19-position quality sweep inserted
`QUALITY` at every position 0–18 of the baseline late block; positions 0–3
reproduce 80.5, positions 4–10 give 79.2, positions 12–14 give `r6` but
71.7–73.0.

## 6. Why 80.5 is the local optimum

The remaining delta is three coupled facts:

1. **GOT-load order** is the SelectionDAG `list-hybrid` schedule. The first 7
   `tLDRLIT_ga_pcrel` nodes become the prologue hoist; the rest keep their DAG
   order. The baseline's DAG order is the target's with `status` moved from
   slot 26 to slot 6.
2. **`movw r6`** is chosen by the greedy register allocator, not the scheduler.
   The pre-RA `t2MOVi16 583` sits at the quality block in every variant. If the
   greedy RA picks `r6` (callee-saved) for the `v54` select chain, the post-RA
   top-down list latency scheduler hoists the `movw` to the earliest legal
   point — immediately after the last use of `r6` (`ldr.w ip,[r6]`), i.e.
   `0xe3236`. If the RA picks `r0`, the `movw` stays in the quality block.
   The RA picks `r6` only when the caller-saved allocation order is occupied at
   the `movw`'s live interval; empirically that happens when the quality block
   is pre-RA-scheduled after `items`/`ships`/`iaw`.
3. **The two `nop`s** are 8-byte literal-pool padding. They are unmatched by
   construction; the original has none because its real code is 4 B longer.

`WIN` proves (1) and (2) are not mutually exclusive at the source level. They
are only mutually exclusive for the *fuzzy score*, because the source order
that produces both also reorders the late zero-init stores in a way that
difflib aligns worse than the baseline's (status-early) order. (3) is the
reason the baseline can never reach 100% without the `movw r6` hoist: the
padding costs two normalised instructions and the hoist is the natural 4-byte
replacement.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w4259
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j8
cmake --build cmake-build-match --target check -j8
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN7GlobalsC2Ev
```

The scratch harness (`evq.py`, `evfull.py`, `seq.py`, `swap_qa.py`,
`sweep_qpos.py`, `run_q*.py`, `run_late.py`, `run_store.py`, `dump_q.py`,
`widths.py`, `wide.py`) lives in `/dev/shm/w4259` (scratch, not tracked).
`Globals.cpp.bak` there is byte-identical to HEAD.

## 8. What remains

The sharpest remaining target is the **4-byte padding**: find a faithful source
shape that makes the real code 472 B (so the pool needs no `nop`s) *without*
changing the late store order. The natural candidate is the `movw r6,#583`
hoist. The RA picks `r6` when the quality block is scheduled late; the
challenge is to get that scheduling without moving `status`/`items`/`ships` in
the DAG order. A construct that changes the post-RA list-scheduler heights of
the late block (e.g. a different IR shape for the `status`/`instantAction*`/
`items`/`ships` group) without touching the GOT-load DAG order is the most
promising direction. Any such change must be measured with the official
verifier; the baseline 80.5 is a genuine local optimum under every swap and
quality-position move tried so far.
