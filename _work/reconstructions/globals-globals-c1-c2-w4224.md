# Globals::Globals C1/C2 (80.5%, 612 B) — w4224

Status: research note, 2026-11-03. Worktree `wt/w4224`, branch `wave92-w4224`,
base `b379d00d`. **No source change landed** — the w3808/w3827/w4008/w4108
baseline source order is still the best measured shape (80.5%). This commit
contains only this note. Everything below is self-contained; no transcript is
required.

## TL;DR

* Baseline and final: **`_ZN7GlobalsC1Ev` / `_ZN7GlobalsC2Ev` = 80.5%, 612 B**
  (size-exact; 225 vs 227 normalised instructions). Gate unchanged:
  **byte_exact 4498 / linked_exact 4498 / size-exact 4524, stubs 0, missing 0,
  wrong_type 0, extra 0, data 843/843, imports 207, .text 0x1337c8,
  .rodata 0x41b67, lint CLEAN, drift 0, `RATCHET PASS`**.
* New this pass: a source order was found that simultaneously reproduces the
  original's **31/31 literal-pool order** *and* the **`movw r6,#583` hoist**
  (variant `WIN` below). Its machine code matches the original for the first
  **96** normalised instructions (the whole prologue + settings block), but its
  late zero-init block is scheduled differently and the fuzzy is **78.3%**, i.e.
  a net *regression* vs the 80.5% baseline. Measured ~900 more variants; nothing
  beats 80.5.
* The remaining delta is still two coupled scheduler/allocator facts:
  1. the original's GOT-load order has `status` at slot 27 (0-based 26), the
     baseline has it at slot 7 (0-based 6);
  2. the original keeps the `resWidth` default `583` in callee-saved `r6`
     across the settings block (`movw r6,#583` @0x56), the baseline
     rematerialises it into `r0` in the quality block (`movw r0,#583` @0x152).
* `WIN` proves the two are *not* mutually exclusive at the source level. They
  are only mutually exclusive for the *fuzzy score*, because the source order
  that produces both also reorders the late zero-init stores in a way that
  difflib aligns worse than the baseline's (status-early) order.

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

Full annotated store order (post-RA machine code; `r1` = settings, `r2` = 0,
`r5` = -1, `r4` = 0.5f):

```text
e323a  str  r2,[r0]           shipTemplate = 0
e3242  str  r2,[r0]           gameText = 0
e324a  str  r2,[r0]           font = 0
e3252  str  r2,[r0]           fontAlien = 0
e325a  str  r2,[r0]           fontLangSelect = 0
e3262  str  r2,[r0]           rnd = 0
e326a  str  r2,[r0]           globals = 0
e3272  str  r2,[r0]           layout = 0
e327a  str  r2,[r0]           appManager = 0
e327e  str  r2,[r5]           recordHandler = 0
e3288  str  r2,[r0]           generator = 0
e328e  str.w r2,[r9]          this->field_0x0 = 0
e3292  vst1.32 {d16-d17},[r0] options+0x14..0x20 = {.5,.5,.6,.6}
e329a  strh r0,[r1,#12]       region[0] = 0x101
e32a0  strd r4,r4,[r1]        colorR = colorG = 0.5
e32a4  str  r4,[r1,#8]        colorB = 0.5
e32a6  strd r4,r0,[r1,#36]    tintB = 0.5, brightness = 1.0
e32ac  str  r4,[r1,#44]       contrast = 0.5
e32ae  strb r0,[r1,#14]       region[2] = 1
e32b0  strb.w r0,[r1,#48]     enableFlag30 = 1
e32b6  strh r2,[r1,#16]       at10 = 0
e32ba  strh.w r2,[r1,#64]     shortAt40 = 0
e32be  str  r2,[r1,#80]       intAt50 = 0
e32c2  strb.w r2,[r1,#57]     flagAt39 = 0
e32c6  str.w r2,[r1,#53]      intAt35 = 0
e32ca  str.w r2,[r1,#49]      intAt31 = 0
e32ce  str  r2,[r0,#4]        hints->slot1 = 0
e32d0  strb r2,[r0,#19]       hints->flag13 = 0
e32d2  strb.w r2,[r1,#63]     flagAt3f = 0
e32d6  str.w r2,[r1,#59]      intAt3b = 0
e32da  strb.w r2,[r1,#78]     flagAt4e = 0
e32de  strh.w r2,[r1,#76]     shortAt4c = 0
e32e2  str  r2,[r1,#72]       intAt48 = 0
e32e4  str  r2,[r0]           hints->slot0 = 0
e32ec  strb r2,[r0]           gameLoaded = 0
e32f4  strb r2,[r0]           gameSaving = 0
e32f8  str.w r5,[r8]          lastSpace = -1
e3300  strb r2,[r0]           initMemoryWarning = 0
e3304  str  r5,[r3]           lastStation = -1
e330e  str.w r2,[ip]          lastCampaignFailCount = 0
e3318  str.w r5,[lr]          lastCampaignFailed = -1
e330a..e335e                   quality block (see below)
e33a0  str  r2,[r6]           galaxy = 0
e336a  str  r2,[r0]           instantActionScore = 0
e3372  str  r2,[r0]           instantActionWave = 0
e3380  str  r5,[r3]           instantActionType = -1
e3388  str  r2,[r3]           instantActionPlayerName = 0
e3390  str  r2,[r3]           items = 0
e3394  str  r2,[r0]           status = 0
e339c  strh.w r0,[r1,#96]     shortAt60 = 0x100
e33a6  str  r2,[r4]           achievements = 0
e33a8  str.w r2,[ip]          imageFactory = 0
e33ac  str  r2,[r3]           ships = 0
e33ae  str.w r2,[r9,#4]       soundResources = 0
```

The quality block:

```text
e330a  vldr  s2,[r1,#68]      fv = settings->qualityLevel
e3314  vcmpe.f32 s2,s0        fv vs 1.0
e3320  vcmpe.f32 s2,#0.0      fv vs 0.0
e3328  movw  r0,#513          v58 default
e3330  movwge r6,#830         (width high)
e333e  movwls r6,#415         (width low)
e334a  str   r6,[r1,#84]      resWidth = v54
e334e  movwge r0,#730         (height high)
e3358  movwls r0,#365         (height low)
e335e  str   r0,[r1,#88]      resHeight = v58
```

`movw r6,#583` is at `0xe3236`, immediately after `ldr.w ip,[r6]`
(`0xe3232`, the last use of the prologue GOT temp in `r6`) and immediately
before the `shipTemplate` store (`0xe323a`). The `resWidth` default is live in
`r6` from `0xe3236` to `0xe334a` (~69 instructions).

## 3. The mechanism (confirmed with `-mllvm -print-after-all`)

1. **GOT-load order** is the SelectionDAG `list-hybrid` schedule. The first 7
   `tLDRLIT_ga_pcrel` nodes become the prologue hoist; the rest keep their DAG
   order. The baseline's post-ISel DAG order is the target's with `status`
   moved from slot 27 to slot 7.
2. **`movw r6`** is chosen by the greedy register allocator, not the scheduler.
   The pre-RA `t2MOVi16 583` sits at the quality block in every variant. If the
   greedy RA picks `r6` (callee-saved) for the `v54` select chain, the
   post-RA top-down list latency scheduler hoists the `movw` to the earliest
   legal point — immediately after the last use of `r6`
   (`ldr.w ip,[r6]`), i.e. `0xe3236`. If the RA picks `r0`, the `movw` stays
   in the quality block.
3. The RA picks `r6` only when the caller-saved allocation order is occupied
   at the `movw`'s live interval. Empirically this happens when the quality
   block is pre-RA-scheduled after `items`/`ships`/`iaw` — which, in the
   `cand` source order, also drags `instantActionPlayerName`/`items`/`ships`
   into the prologue hoist and breaks the pool.

`-mllvm -disable-post-ra` leaves the pool at 31/31 and the `movw` at `r0`,
confirming the hoist (not the RA choice) is the post-RA pass.

## 4. New result: `WIN` — exact pool *and* `movw r6`

Source order (constructor body only; the settings block is unchanged from the
baseline):

```cpp
GameSettingsRecord *settings = (GameSettingsRecord *) Globals::options;
Globals::shipTemplate = 0;
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
Globals::recordHandler = 0;
this->field_0x0 = 0;
/* ... baseline settings block ... */
Globals::lastSpaceMusicPlayed = -1;
Globals::gameLoaded = 0;
Globals::gameSaving = 0;
Globals::initMemoryWarning = 0;
Globals::imageFactory = 0;
Globals::achievements = 0;
Globals::galaxy = 0;
Globals::instantActionScore = 0;
Globals::instantActionWave = 0;
Globals::lastStationMusicPlayed = -1;
Globals::lastCampaignMissionFailed = -1;
Globals::lastCampaignMissionFailCount = 0;
/* quality block (unchanged) */
Globals::items = 0;
Globals::ships = 0;
Globals::instantActionPlayerName = 0;
Globals::instantActionType = -1;
Globals::status = 0;
this->soundResources = 0;
reinterpret_cast<int16_t &>(settings->shortAt60) = 0x100;
```

Machine code (first 36 instructions):

```text
 0 push {r4, r5, r6, r7, lr}
 1 add  r7, sp, #12
 2 stmdb sp!, {r8, r9, sl}
 3 ldr  r1, [pc, #476]      Canvas
 4 mov  r9, r0
 5 ldr  r2, [pc, #476]      lastSpace
 6 vmov.f32 s0, #112
 7 add  r1, pc
 8 ldr  r3, [pc, #472]      lastStation
 9 ldr  r5, [pc, #476]      lastCampaignFailed
10 add  r2, pc
11 ldr  r6, [pc, #476]      lastCampaignFailCount
12 add  r3, pc
13 ldr  r4, [pc, #476]      options
14 add  r5, pc
15 ldr  r0, [pc, #476]      recordHandler
16 add  r6, pc
17 ldr.w sl, [r1]
18 add  r4, pc
19 ldr.w r8, [r2]
20 movs r2, #0
21 add  r0, pc
22 ldr.w lr, [r5]
23 str.w r2, [sl]
24 add  r1, pc, #408
25 ldr  r5, [r0, #0]
26 ldr  r0, [pc, #448]      shipTemplate
27 vld1.64 {d16-d17}, [r1]
28 add  r0, pc
29 ldr  r1, [r4, #0]
30 mov.w r4, #1056964608
31 ldr  r0, [r0, #0]
32 ldr  r3, [r3, #0]
33 ldr.w ip, [r6]
34 movw r6, #583            <-- exact target
35 str  r2, [r0, #0]
```

The normalised diff against the original has one `equal` block `0..96`
(the whole prologue + settings block) and the full 39-instruction literal pool
`186..225`; the only remaining differences are the late zero-init schedule and
the `resHeight` chain, which `WIN` allocates to `r6` (reusing it after the
`resWidth` store) where the original uses `r0`.

`WIN` measurements: **pct 78.3, size 612/612, pool 31/31, `movw r6`**.
It is a strict *regression* in fuzzy vs the 80.5% baseline, so it was not
landed.

## 5. Measurements this pass

All variants were built with the exact repo compile line (NDK r18b clang 7.0.2,
`-target armv7-none-linux-androideabi16 -march=armv7-a -mthumb -mfpu=neon
-mfloat-abi=softfp -std=c++11 -fPIC -frtti -fstack-protector-strong -Oz -g
-stdlib=libc++`) and scored with the official verifier
(`tools/verify/verify.py`, `autojunk=False`). `pool` is the positional LCS
against the target's 31-slot pool (`31` = exact; the baseline's `status` shift
makes the positional LCS 10 even though the note's shift-aware count is 30/31).

| variant | source-order change vs baseline | pct | pool | `movw` 583 |
|---|---|---:|---:|---|
| **baseline** | — | **80.5** | 10 (shifted) | `r0` @0x152 |
| `cand` | `status` after `lastStation`; `recordHandler` last in early block | 78.8 | **31** | `r0` |
| **`WIN`** | `cand` early + quality after `lcmfc` + late tail `items,ships,iapn,iat,status` | 78.3 | **31** | **`r6`** @0x56 |
| `X` q=17 | `cand` early, quality after `lcmfc`, `lastStation/lcmf/lcmfc` before it | 73.0 | **31** | `r0` |
| `WIN` with settings reordered to original machine order | `S1` | 77.4 | 31 | `r6` |
| `WIN` with `gameLoaded,gameSaving,lastSpace,initMem` | `A` permuted | 77.4 | 31 | `r6` |
| `WIN` post-group permutations | 120 perms | 74.3–78.3 | 24–31 | `r6` |
| baseline + `lastSpace`/`gameLoaded` swapped | — | ≤80.5 | 10 | `r0` |
| baseline + settings machine order (`S1`) | — | 79.2 | 10 | `r0` |
| baseline + `intAt48` next to `hints->slot0` | — | 80.5 | 10 | `r0` |

Search coverage this pass: full quality-position sweep over the baseline order
(61 positions), full sweep over the `cand` order (20 positions), full sweep over
the `WIN` order (20 positions), 400 random late orders × quality position,
24 permutations of `lastSpace/gameLoaded/gameSaving/initMem`, 120 random
settings orders, and two hill-climbs (250 iterations each from the baseline and
from `cand`, whole-statement swaps + quality-position moves). The best score
found was 80.5 (the baseline itself).

## 6. Why `WIN` loses the fuzzy even though it fixes both known defects

The fuzzy is `difflib.SequenceMatcher` over the normalised instruction stream.
`WIN` matches the original for `0..96` and the whole literal pool, but its late
zero-init block is scheduled as

```text
lastSpace, gameLoaded, gameSaving, initMem, imageFactory, achievements,
galaxy, ias, iaw, lastStation, lcmf, lcmfc, [quality], items, ships, iapn,
iat, status, soundRes, short60
```

whereas the original's is

```text
gameLoaded, gameSaving, lastSpace, initMem, lastStation, lcmfc, lcmf,
[quality], galaxy, ias, iaw, iat, iapn, items, status, short60,
achievements, imageFactory, ships, soundRes
```

The baseline's (status-early) late block happens to align better with the
original's under difflib's greedy matching (49 matched instructions in the late
region vs `WIN`'s 41), which more than offsets `WIN`'s gains in the prologue
(+1) and literal pool (+1). The `status` position is the coupling: putting
`status` late is required for the exact pool and for `r6`, but it changes the
scheduler's late-store order enough to lose ~8 late-block matches.

The baseline remains a genuine local optimum: a 250-iteration whole-statement
hill-climb from it (random swaps + quality-position moves, accepting equal or
better scores) never left the start point.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w4224
cmake --build cmake-build-match --target check -j8
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN7GlobalsC2Ev
```

The `WIN` source body is reproduced in §4; the scratch harness
(`fast.py`/`feval.py`/`gen2.py`/`search*.py`/`climb*.py`) lives in
`/dev/shm/w4224` (scratch, not tracked).

## 8. What remains

The gap is now fully characterised: the original's source order produces both
the exact GOT pool and the `r6` hoist, and `WIN` is a concrete witness of that
source order. To beat 80.5% one needs a *third* property — the same late
zero-init store schedule as the original — which the `list-hybrid`/post-RA
scheduler will not produce from any source order tried so far. The most
promising remaining direction is a construct that changes the late-block
scheduling heights without touching the GOT-load DAG order (e.g. a different
IR shape for the `status`/`instantAction*`/`items`/`ships` group), or a
per-TU scheduler knob that is neutral for every other function in the TU.
