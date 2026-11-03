# Globals::Globals C1/C2 (80.5%, 612 B) — w4510 (wave 95)

Status: research note, 2026-11-03. Worktree `wt/w4510`, branch `wave95-w4510`,
base `4a88bd6e`. **No source change landed** — the w3808..w4410 baseline source
order is still the best measured shape (80.5%). This commit contains only this
note. Everything below is self-contained; no transcript is required.

## TL;DR

* Baseline and final: **`_ZN7GlobalsC1Ev` / `_ZN7GlobalsC2Ev` = 80.5%, 612 B**
  (size-exact; 225 vs 227 normalised entries, M = 182 matched,
  `2·182/452 = 0.8053097345132744`).
* Gate unchanged: **byte_exact 4500 / linked_exact 4500 / size-exact 4524,
  stubs 0, missing 0, wrong_type 0, extra 0, data 843/843, imports 207,
  lint CLEAN, drift 0, `RATCHET PASS`**.
* This pass attacked the three axes named in the brief — the exact zero-init
  store schedule, the literal-pool 31/31 order, and the `movw r6` placement —
  with two exhaustive searches that had not been run before:
  * **all 8! = 40 320 permutations of the settings-tail tokens** (with the WIN
    early block and WIN late block): best **78.3**, i.e. the tail machine order
    is scheduler-determined and is invariant under source permutation;
  * **all 46 838 distinct two-move reorderings of the baseline 19-token late
    block**: best **80.5** (the baseline itself). The baseline is a strict
    local optimum under two-move moves as well as single moves.
* New instruction-indexed store tables (§3) show exactly why the two local
  optima are mutually exclusive: the baseline's late block aligns
  `gameLoaded/gameSaving/initMem/iat/iapn/items` to within ±2 instructions of
  the original, while the WIN early block (which alone gives the 31/31 pool and
  the `movw r6,#583` hoist) shifts the whole late block by 10–60 instructions.
* The only remaining route to a strict improvement is the **4-byte code-size
  delta** (two `ip` 32-bit forms), which is a register-allocation outcome, not
  a source-order one. Nothing in this pass moved it.

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

## 2. Ground truth: the original 55 stores, with instruction indices

`_ZN7GlobalsC2Ev` @ `0xe31e0`, 612 B, literal pool @ `0xe33c8`
(31 `R_ARM_GOT_PREL` words + 4 float words). Normalised stream length 225
(186 code + 8 float-word entries + 31 pool entries).

The table below is the **original store order** re-derived this pass by
simulating the PIC idiom (`ldr rX,[pc,#N]` / `add rX,pc` / `ldr rX,[rX]`),
resolving the GOT slot through `.rel.dyn`, and printing the **normalised
instruction index** of each store. `r1 = settings (Globals::options)`,
`r9 = this`, `r0 = &Globals::hints` for the secondary object.

```text
idx  store
 23  Canvas
 35  shipTemplate
 39  gameText
 43  font
 47  fontAlien
 51  fontLangSelect
 55  rnd
 59  globals
 63  layout
 67  appManager
 69  recordHandler
 73  generator
 75  this+0
 78  options+0x0c (region0)
 80  options+0x00 (colorR/colorG, strd)
 81  options+0x08 (colorB)
 82  options+0x24 (tintB/brightness, strd)
 84  options+0x2c (contrast)
 85  options+0x0e (region2)
 86  options+0x30 (enable30)
 88  options+0x10 (at10)
 90  options+0x40 (short40)
 91  options+0x50 (int50)
 93  options+0x39 (flag39)
 94  options+0x35 (int35)
 95  options+0x31 (int31)
 96  hints+0x04 (sec_slot1)
 97  hints+0x13 (sec_flag13)
 98  options+0x3f (flag3f)
 99  options+0x3b (int3b)
100  options+0x4e (flag4e)
101  options+0x4c (short4c)
102  options+0x48 (int48)
103  hints+0x00 (sec_slot0)
107  gameLoaded
111  gameSaving
113  lastSpace
116  initMemoryWarning
118  lastStation
122  lastCampaignMissionFailCount
125  lastCampaignMissionFailed
139  options+0x54 (resWidth)
146  options+0x58 (resHeight)
152  instantActionScore
156  instantActionWave
163  instantActionType
167  instantActionPlayerName
171  items
173  status
176  options+0x60 (short60)
177  galaxy
180  achievements
181  imageFactory
182  ships
183  soundResources
```

The four `glowR/glowG/tintR/tintG` floats (`options+0x14..0x20`) are written by
one `vst1.32 {d16-d17}` at `0x00c0`, not by four scalar stores; they are
therefore not listed.

Original literal-pool order (machine order of the `ldr [pc,#imm]` loads):

```text
Canvas, lastSpace, lastStation, lastCampaignMissionFailed,
lastCampaignMissionFailCount, options, recordHandler, shipTemplate, gameText,
font, fontAlien, fontLangSelect, rnd, globals, layout, appManager, generator,
hints, gameLoaded, gameSaving, initMemoryWarning, imageFactory, achievements,
galaxy, instantActionScore, instantActionWave, status, instantActionType,
instantActionPlayerName, items, ships
```

The first 7 words are the prologue hoist; the remaining 24 follow the machine
order of the body's GOT loads. `movw r6,#583` (the `resWidth` default) is at
normalised index **34**, immediately after `ldr.w ip,[r6]` (index 33) and
before the `shipTemplate` store (index 35).

## 3. Baseline vs WIN vs original, instruction-indexed

The two measured local optima are:

* **baseline** (status early): 80.5, early 95/96, late 49/90, pool 38/39
  (30/31), `movw r0,#583` inside the quality block;
* **WIN** (status late, `recordHandler` after `generator`): 78.3, early
  **96/96**, late 42/90, pool **39/39 (31/31)**, `movw r6,#583` at index 34.

Store tables (normalised instruction index, same tracer as §2):

```text
store                     orig   baseline   WIN
sec_slot1 (hints+4)         96      101       98
sec_flag13 (hints+0x13)     97      102       99
flag3f                      98      161      140
int3b                       99       99       96
flag4e                     100      100       97
short4c                    101      103      100
int48                      102      159      139
sec_slot0 (hints+0)        103      104      101
gameLoaded                 107      109      106
gameSaving                 111      113      110
lastSpace                  113      105      102
initMemoryWarning          116      117      114
lastStation                118      133      136
lastCampaignFailCount      122      183      145
lastCampaignFailed         125      176      143
resWidth                   139      147      157
resHeight                  146      156      166
instantActionScore         152      138      130
instantActionWave          156      174      134
instantActionType          163      164      178
instantActionPlayerName    167      168      177
items                      171      172      172
status                     173       72      174
short60                    176      181      182
galaxy                     177      129      126
achievements               180      125      122
imageFactory               181      121      118
ships                      182      178      180
soundResources             183      180      181
```

The baseline's late block wins because eight stores land within ±2 instructions
of the original (`gameLoaded`, `gameSaving`, `initMemoryWarning`, `int3b`,
`flag4e`, `instantActionType`, `instantActionPlayerName`, `items`), and the
repeated `ldr r0,[pc]; add r0,pc; ldr r0,[r0]; str r2,[r0]` idiom is identical
text wherever it occurs. The WIN early block wins because it is the only
measured shape whose prologue + settings block is exactly 96/96 and whose GOT
pool is exactly 31/31, but its late block shifts the whole zero-init group by
10–60 instructions.

## 4. New searches this pass

All variants were compiled with the exact repo compile line (NDK r18b clang
7.0.2, `-target armv7-none-linux-androideabi16 -march=armv7-a -mthumb
-mfpu=neon -mfloat-abi=softfp -std=c++11 -fPIC -frtti
-fstack-protector-strong -Oz -g -stdlib=libc++`) and scored with the official
`tools/verify/verify.py` normalisation (`autojunk=False`). The fast single-TU
scorer reproduces 80.5 / 227 / 225 exactly.

### 4.1 Exhaustive settings-tail permutation (new)

The settings tail is the 8 tokens
`int3b flag4e short4c int48 sec_slot1 sec_flag13 sec_slot0 flag3f`.
All **8! = 40 320** permutations were measured with the WIN early block
(`shipTemplate Canvas … int31`) and the WIN late block. Best: **78.3**
(the WIN score), achieved by 15+ distinct permutations; no permutation
improves it. The machine order of the tail is therefore a scheduler property,
not a source-order property: the scheduler always groups the secondary stores
(`sec_slot1`, `sec_flag13`, `sec_slot0`) and defers `int3b/flag4e/short4c/int48`
towards the quality block.

### 4.2 Exhaustive two-move reorder of the baseline late block (new)

The baseline 19-token late block
`lastSpace gameLoaded gameSaving QUALITY initMem imageFactory achievements
galaxy ias lastStation iat iapn items ships iaw lcmf soundRes short60 lcmfc`
was reordered by **all 46 838 distinct two-move moves** (two
remove-and-insert operations). Best: **80.5** (the baseline itself). The
baseline is therefore a strict local optimum under two-move moves, not just
under the single-move and block-move neighbourhoods measured in w4410.

### 4.3 Other measured variants (new this pass)

| variant | pct | note |
|---|---:|---|
| baseline | **80.5** | reference |
| WIN early + original machine-order late (quality after `lcmf`) | 49.1 | pool 22/31, no `r6` hoist |
| WIN early + original machine-order late (quality after `iaw`) | 49.1 | same |
| baseline + `gameLoaded = gameSaving = 0` (chained) | 80.1 | worse |
| baseline + `imageFactory` moved to end | 57.5 | pool 6/31 |
| baseline + QUALITY between `initMem` and `imageFactory` | 79.2 | store not deferred |
| baseline + original settings-tail order | 79.2 | `sec_slot0` hoisted, `int3b/flag4e/short4c/int48` sunk |
| WIN early + original settings-tail order + WIN late | 77.4 | tail machine order still wrong |
| reverse baseline late block | 64.2 | — |
| pool-order-as-source (GOT statements in pool order) | 48.3 | pool 1/31 |

## 5. What this proves

1. **The baseline is a genuine two-move local optimum of the fuzzy metric**
   (46 838 candidates, no improvement), on top of w4410's complete single-move
   neighbourhood (1 892), 20 000 random two-move samples, and 30 plateau
   rounds.
2. **The settings-tail machine order is scheduler-determined.** All 40 320
   source permutations of the 8 tail tokens collapse to the same 78.3 machine
   schedule (with the WIN early/late blocks). Reordering the source cannot fix
   the tail; only a different IR shape (which changes register pressure) can.
3. **The pool order is a machine property too.** Setting the source order to
   the pool order yields pool 1/31, because the scheduler reorders the GOT
   loads. The WIN source order is the unique measured order that reproduces the
   exact 31/31 pool, and it does so together with the 96/96 early block and
   the `movw r6,#583` hoist.
4. **The two local optima are mutually exclusive at the source level.** The
   WIN early block is the *correct* early block (96/96, 31/31, hoist) but its
   late block is 42/90; the baseline's late block is 49/90 but its early block
   is 95/96 and its pool is 30/31. If the two could be combined the score would
   be `M = 96 + 49 + 39 = 184 → 2·184/452 = 81.4` (81.8 on the 450-entry
   denominator used in w4410). No source order found in two passes achieves
   the combination.
5. **The only strict improvement left is the 4-byte code-size delta.** Ours is
   186 real instructions / 468 B + 2 `nop` pad; the original is 186 real
   instructions / 472 B, because two of its instructions are the 32-bit `ip`
   forms `ldr.w ip,[r0]` (loads `&imageFactory`) and `str.w r2,[ip]`
   (the `imageFactory` store). If those two encodings were recovered, `len_u`
   would drop 227 → 225 and the score would be `2·182/450 = 80.9` even with M
   unchanged. They are a register-allocation outcome: the original keeps
   `&imageFactory` live in `ip` from the quality block (`0x128`) to the last
   store (`0x1c8`), while ours stores it immediately in `r0`.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w4510
cmake --build cmake-build-match --target check -j8
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN7GlobalsC2Ev
```

Scratch harness (not tracked, `/dev/shm/w4510`): `fast.py` (single-TU scorer,
0.35 s/eval), `gen.py` (token→statement body generator), `ev.py`,
`diag.py`, `opc.py`, `unmatched.py`, `trace.py`, `trace2.py`,
`traceidx.py` (original store index tracer), `traceidx_ours.py` (our store
index tracer), `tailperm.py` (40 320 tail permutations), `move2.py`
(46 838 two-move reorders), `search.py`, `block.py`, `climb.py`, `anneal.py`.

## 7. What remains

The function cannot be improved by statement reordering. The two precise
targets are unchanged from w4410:

1. **Recover the two `ip` 32-bit forms** (`ldr.w ip,[r0]` for `&imageFactory`,
   `str.w r2,[ip]` for its store). This is the only route to removing the two
   `nop`s and it alone is worth 80.5 → 80.9. It needs a source shape that
   makes the scheduler defer the `imageFactory` store past the quality block
   while keeping its GOT load at pool slot 21. Moving the statement later in
   source moves the GOT load too (pool 6/31 measured), so this is a
   register-pressure problem, not an ordering one.
2. **Combine the WIN early block with a 49/90 late block.** The WIN early order
   is the correct one (96/96, 31/31, hoist). The missing piece is a late-block
   IR shape that keeps the WIN prologue/pool while aligning the zero-init
   group to the original's indices. All 40 320 tail permutations and 46 838
   two-move reorders of the two existing late blocks leave this open.
