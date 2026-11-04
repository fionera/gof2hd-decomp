# Globals::Globals C1/C2 (80.5%, 612 B) — w4810 (wave 98)

Status: research note, 2026-11-04. Worktree `wt/w4810`, branch `wave98-w4810`,
base `fe4f5e7cf`. **No source change landed** — the w3808..w4510 baseline source
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
  store schedule, the 31/31 literal pool, and the `movw r6` placement — plus
  the 4-byte code-size delta, with **eight new exhaustive / large searches**
  (§4). Every one bottoms out at 80.5 or below.
* New evidence this pass:
  1. **Ghidra is usable again for this function** once the right address is
     used: the project's `.text` is zero-filled and the function is misnamed
     `GlobalsCtorW3021` at Ghidra `0xe31e0`; the real ctor is at Ghidra
     **`0xf31e0`** (ELF vaddr `0xe31e0`, size 472). Its decompile is in §2.
  2. The **iOS 1.1.4 oracle** (`_work/bins/ios_1.1.4_HD`, ctor at `0xdce00`)
     confirms the same constants (0x247/0x33e/0x19f, 0x201/0x2da/0x16d,
     0x100 short60, −1 sentinels) and the same two independent if-chains, but
     is an older source shape (byte-wise stores, `movw`+`movt` absolute
     addresses, no GOT), so it is a logic oracle only.
  3. The original uses **`ip` twice**: once for `lastCampaignMissionFailCount`
     (prologue load `ldr.w ip,[r6]` at index 33, store `str.w r2,[ip]` at 122)
     and once for `imageFactory` (load `ldr.w ip,[r0]` at 128, store
     `str.w r2,[ip]` at 181). Ours uses `ip` only for
     `lastCampaignMissionFailCount`, which is why our code is 4 B short and
     pads with two `nop`s (see §5). This corrects the w4510 note's phrasing:
     the missing pair is the **imageFactory** pair, not the lcmfc pair.

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

## 2. Ground truth: Ghidra decompile of the real ctor

The Ghidra project `_work/bins/android_2.0.16_libgof2hdaa.so_ghidra` has a
zero-filled `.text` block and a misnamed `GlobalsCtorW3021` at Ghidra
`0xe31e0`; the real function is `Globals` at Ghidra **`0xf31e0`** (ELF
`0xe31e0`, 472 B). Decompiled with
`/tmp/ghvenv/bin/python .tmp/ghidra_decomp2.py <proj> <projname> / <prog> 0xf31e0`:

```c
Globals * __thiscall Globals::Globals(Globals *this)
{
  ...
  puVar7 = (undefined4 *)(DAT_000f33d0 + 0xf3200);
  puVar9 = (undefined4 *)(DAT_000f33d8 + 0xf3208);
  puVar8 = (undefined4 *)(DAT_000f33dc + 0xf320e);
  puVar11 = *(undefined4 **)(DAT_000f33cc + 0xf31fc);
  puVar5 = (undefined4 *)(DAT_000f33e0 + 0xf3216);
  puVar12 = *(undefined4 **)(DAT_000f33d4 + 0xf3204);
  **(undefined4 **)(DAT_000f33c8 + 0xf31f6) = 0;
  uVar2 = DAT_000f33c0;  uVar1 = DAT_000f33b8;
  ...
  uVar10 = 0x247;
  ...
  iVar3 = DAT_000f3408;  *puVar5 = 0;
  **(undefined4 **)(iVar3 + 0xf3288) = 0;
  *(undefined4 *)this = 0;
  *(undefined8 *)(puVar8 + 5) = uVar1;   /* glowR/glowG */
  *(undefined8 *)(puVar8 + 7) = uVar2;   /* tintR/tintG */
  ...                                    /* settings block, see §3 */
  fVar13 = (float)puVar8[0x11];          /* qualityLevel */
  ...
  uVar6 = 0x201;
  if (1.0 <= fVar13) uVar10 = 0x33e;
  if (fVar13 <= 0.0) uVar10 = 0x19f;
  puVar8[0x15] = uVar10;                 /* resWidth */
  if (1.0 <= fVar13) uVar6 = 0x2da;
  if (fVar13 <= 0.0) uVar6 = 0x16d;
  puVar8[0x16] = uVar6;                  /* resHeight */
  ...
  *(undefined2 *)(puVar8 + 0x18) = 0x100;/* shortAt60 */
  ...
  *(undefined4 *)(this + 4) = 0;         /* soundResources */
  return this;
}
```

This confirms the source shape is the current one: a long straight-line
sequence of `Globals::member = 0;`, the `settings` block, the quality block as
**two independent if-chains** (`uVar10 = 0x247; if (1.0 <= fv) ...; if
(fv <= 0.0) ...; resWidth = uVar10;` then the same for `uVar6 = 0x201` /
`resHeight`), and the late zero-init group.

## 3. Ground truth: store order and literal pool

`_ZN7GlobalsC2Ev` @ `0xe31e0`, 612 B, literal pool @ `0xe33c8`
(31 `R_ARM_GOT_PREL` words + 4 float words). Normalised stream length 225
(186 code + 8 float-word entries + 31 pool entries).

Original store order (instruction offset from `0xe31e0`, re-derived this pass
by simulating the PIC idiom and resolving the GOT slot through `.rel.dyn`):

```text
0038 Canvas          005a shipTemplate    0062 gameText        006a font
0072 fontAlien       007a fontLangSelect  0082 rnd            008a globals
0092 layout          009a appManager      009e recordHandler  00a8 generator
00ae this+0          00ba options+0x0c    00c0 options+0x00    00c4 options+0x08
00c6 options+0x24    00cc options+0x2c    00ce options+0x0e    00d0 options+0x30
00d6 options+0x10    00da options+0x40    00de options+0x50    00e2 options+0x39
00e6 options+0x35    00ea options+0x31    00ee hints+0x04      00f0 hints+0x13
00f2 options+0x3f    00f6 options+0x3b    00fa options+0x4e    00fe options+0x4c
0102 options+0x48    0104 hints+0x00      010c gameLoaded      0114 gameSaving
0118 lastSpace       0120 initMemoryWarning 0124 lastStation   012e lcmfc
0138 lcmf            016a resWidth        017e resHeight       018a ias
0192 iaw             01a0 iat            01a8 iapn            01b0 items
01b4 status          01bc short60         01c0 galaxy          01c6 achievements
01c8 imageFactory    01cc ships           01ce soundResources
```

Ours (same tracer, correct `.so` GOT map):

```text
0038 Canvas          0056 recordHandler  005e shipTemplate    0066 gameText
006e font            0076 fontAlien      007e fontLangSelect  0086 rnd
008e globals         0096 layout         009e appManager      00a2 status
00ac generator       00b2 this+0         00be options+0x0c    00c4 options+0x00
00c8 options+0x08    00ca options+0x24    00d0 options+0x2c    00d2 options+0x0e
00d4 options+0x30    00da options+0x10    00de options+0x40    00e2 options+0x50
00e6 options+0x39    00ea options+0x35    00ee options+0x31    00f2 options+0x3b
00f6 options+0x4e    00fa hints+0x04      00fc hints+0x13      00fe options+0x4c
0102 hints+0x00      0104 lastSpace       010e gameLoaded      0116 gameSaving
011e initMemoryWarning 0126 imageFactory  012e achievements    0136 galaxy
0140 lastStation     0150 ias            016e resWidth        018a resHeight
0190 options+0x48    0194 options+0x3f    019c iat             01a4 iapn
01ac items           01b0 iaw            01b4 lcmf            01ba ships
01c0 soundResources  01c4 short60         01ca lcmfc
```

Original literal-pool order (machine order of the `ldr [pc,#imm]` loads):

```text
Canvas, lastSpace, lastStation, lastCampaignMissionFailed,
lastCampaignMissionFailCount, options, recordHandler, shipTemplate, gameText,
font, fontAlien, fontLangSelect, rnd, globals, layout, appManager, generator,
hints, gameLoaded, gameSaving, initMemoryWarning, imageFactory, achievements,
galaxy, instantActionScore, instantActionWave, status, instantActionType,
instantActionPlayerName, items, ships
```

Ours is identical except `status` at slot 6 (after `options`) instead of slot
26 (between `instantActionWave` and `instantActionType`). That is the 30/31
pool. The WIN shape (status late) gets 31/31 but loses late-block matches.

The first 7 pool words are the prologue hoist; the remaining 24 follow the
machine order of the body's GOT loads. `movw r6,#583` (the `resWidth` default)
is at normalised index **34**, immediately after `ldr.w ip,[r6]` (index 33) and
before the `recordHandler` store (index 35).

## 4. New searches this pass (all measured with the official verifier)

All variants were compiled with the exact repo compile line (NDK r18b clang
7.0.2, `-target armv7-none-linux-androideabi16 -march=armv7-a -mthumb
-mfpu=neon -mfloat-abi=softfp -std=c++11 -fPIC -frtti
-fstack-protector-strong -Oz -g -stdlib=libc++`) and scored with the official
`tools/verify/verify.py` normalisation (`autojunk=False`). The fast single-TU
scorer reproduces 80.5 / 227 / 225 exactly.

### 4.1 Exhaustive settings-tail permutation with the *baseline* blocks (new)

w4510 ran all **8! = 40 320** permutations of the settings tail
`int3b flag4e short4c int48 slot1 flag13 slot0 flag3f` only with the WIN
early/late blocks. This pass ran the same exhaustive search with the
**baseline** early and late blocks. Best: **80.5**, achieved by at least 15
distinct permutations in the top-15 (the baseline order and its
near-neighbours; w4708's independent run counted 330 such permutations). No
permutation improves it. The tail is a plateau at 80.5, not a strict peak, but the
plateau has no exit above 80.5.

### 4.2 Exhaustive quality-block position sweep (new)

The 9-token quality block `fv v54 if54hi if54lo storeW v58 if58hi if58lo
storeH` was inserted at every one of the 61 positions in the baseline order.
Best: **80.5** at positions 41–46 (the baseline position and its immediate
neighbours); all other positions ≤ 80.1. At positions 16–35 the function
shrinks to 604 B (46 32-bit instructions) and the score drops to 50.7.

### 4.3 Exhaustive single-store position sweep for `imageFactory` (new)

`imageFactory` was removed and re-inserted at every one of the 27 late-block
positions. Best: **80.5** at position 13 (the baseline position, right after
`initMem`); all other positions ≤ 80.1. Positions 24–26 (the end) collapse to
56.6–57.5 because the GOT load moves out of pool slot 21.

### 4.4 WIN early block + 12 000 random full late shuffles (new)

The WIN early block (96/96, 31/31 pool, `movw r6,#583` hoist) was combined with
12 000 random full shuffles of the 19-token late block and a random quality
position. Best: **77.9** (the WIN order itself). No shuffle reaches 80.5.

### 4.5 WIN early block + exhaustive two-move late reorder (new)

All 342 distinct single-token moves of the WIN late block (quality fixed at
position 10) were measured. Best: **78.3** (four variants that move `iat` next
to `ias`); no move reaches 80.5.

### 4.6 Quality-block IR-shape variants (new)

| variant | pct | note |
|---|---:|---|
| baseline (`int v54 = 0x247; …; resWidth = v54; int v58 = 0x201; …`) | **80.5** | reference |
| `int v54 = 0x247, v58 = 0x201;` combined declaration | 80.5 | identical code |
| `v54` declared at the top of the function | 80.5 | identical code |
| `v58` declared before the `v54` ifs | 80.5 | identical code |
| both defaults before both if-chains | 80.5 | identical code |
| both stores after both if-chains | 79.2 | — |
| combined ifs `{ v54 = …; v58 = …; }` | 79.2 | — |
| ternary `(fv <= 0.0f) ? 0x19f : ((1.0f <= fv) ? 0x33e : 0x247)` | 79.6 | — |
| `else if` chain | 31.1 | size 628 |
| inline helper functions `qualityWidth(fv)` / `qualityHeight(fv)` | 79.2 | — |
| `>=` instead of `<=` | 80.5 | canonicalised |

### 4.7 `imageFactory` store-shape variants (new)

| variant | pct | usize | note |
|---|---:|---:|---|
| baseline `Globals::imageFactory = 0;` | **80.5** | 612 | 48 32-bit insns |
| local `ImageFactory **p = &Globals::imageFactory; *p = 0;` | 57.5–79.6 | 612 | folded to the same store |
| reference `ImageFactory *&r = Globals::imageFactory; r = 0;` | 57.5–79.6 | 612 | folded |
| `*reinterpret_cast<void **>(&Globals::imageFactory) = nullptr;` | 80.5 | 612 | identical code |
| `*reinterpret_cast<int *>(&Globals::imageFactory) = 0;` | 80.5 | 612 | identical code |
| `__builtin_memset` / `memset` | 80.5 | 612 | inlined to the same store |
| `ImageFactory **volatile p = …; *p = 0;` | 32.8–43.5 | 640 | 50/49 32-bit insns, but 28 B larger |

The volatile variants are the only ones that produce the extra 32-bit forms
(50/49 32-bit instructions), but they add 28 B of code (usize 640), so they are
not size-exact and score far worse.

### 4.8 Early-block variants (new)

The WIN early block's only differences from the baseline are the position of
`recordHandler` (after `generator` instead of after `shipTemplate`) and of
`status` (late instead of after `generator`). All combinations were measured
with the baseline late block: baseline (rech early, status early) **80.5**;
rech-after-generator + status-after-generator 79.2; rech-after-appManager +
status-after-generator 79.2; rech-after-generator + status-after-rech 79.6;
rech-after-generator + status late 77.9; rech-after-appManager + status late
77.4.

### 4.9 Machine-store-order-as-source variants (new)

Using the original's machine store order as the source order (with the WIN
early block) gives 49.1; with the baseline early block 46.9; with the WIN
early block but the baseline late block 72.9 (usize 600). The literal-pool
order is destroyed by these orders, which is why the score collapses.

## 5. The 4-byte code-size delta (the only remaining route)

Original: **186 real code instructions, 50 32-bit / 136 16-bit = 472 B** of
code, then the 140 B literal pool (31 GOT words + 4 float words), total 612 B.

Ours: **186 real code instructions, 48 32-bit / 138 16-bit = 468 B** of code,
then two `nop` pads (4 B) to align the pool at the same address, then the same
140 B pool, total 612 B. The two `nop`s are what make `len_u = 227` instead of
225.

The two missing 32-bit forms are exactly the `imageFactory` load/store pair:

```text
original:  e3324  f8d0 c000  ldr.w  ip, [r0]     ; ip = Globals::imageFactory
           e33a8  f8cc 2000  str.w  r2, [ip]     ; Globals::imageFactory = 0
ours:      ...    6800       ldr    r0, [r0]     ; 16-bit
           ...    6002       str    r2, [r0]     ; 16-bit
```

The original keeps the `imageFactory` pointer in `ip` from the quality block
(`0x128`) to the last store (`0x1c8`), while ours stores it immediately in
`r0`. That is a register-allocation outcome: at the original's load point the
live set is `{r0, r1, r2, r3, r4, r5, r6, r8, r9, ip}` and the only free
registers are `r7` (frame pointer), `r10` (already saved), `r11` and `ip`; the
allocator picks `ip` to avoid a further callee-saved push. In ours the store is
adjacent to the load, so `r0` is free.

If those two forms were recovered, `len_u` would drop 227 → 225 and the score
would be `2·182/450 = 80.9` even with M unchanged. No source shape tried this
pass (local pointer, reference, volatile, casts, `memset`, helper, position
moves) produces them with `usize = 612`.

## 5b. Relationship to w4708 (pass 9)

The wave-97 note `globals-globals-c1-c2-w4708.md` (commit `1cebcae2f`, on
master but not in this worktree's base) already established the 186/186
instruction-count fact and the two Thumb-16→Thumb-32 promotions, and ran an
exhaustive 8! settings-tail search with the baseline blocks (330 permutations
at 80.5), `status`/`tintR` early-position sweeps, `imageFactory` × quality
position combos (342), quality-block source forms, local-pointer/volatile
variants, store-order-as-source, and 20 000 random WIN-late orders. This note
is consistent with all of that.

The **new** evidence in this pass, not in w4708, is:

* the **Ghidra decompile of the real ctor** at Ghidra `0xf31e0` (the project's
  `.text` is zero-filled and the function is misnamed `GlobalsCtorW3021` at
  `0xe31e0`), which confirms the two independent quality if-chains and the
  exact store shape (§2);
* the **iOS 1.1.4 oracle** at `0xdce00` (same constants, older byte-wise
  source shape), which independently confirms the logic;
* the corrected **`ip`-twice** observation: the original uses `ip` for
  `lastCampaignMissionFailCount` (prologue load, store at index 122) **and**
  for `imageFactory` (load at 128, store at 181); ours uses `ip` only for
  `lastCampaignMissionFailCount` (§5);
* an **exhaustive WIN-late two-move reorder** (342 moves, best 78.3) in
  addition to the 12 000 WIN-late full shuffles (best 77.9);
* **early-block `recordHandler`/`status` combinations** (six variants, best
  80.5) — w4708 swept `status` alone, not `recordHandler`;
* **quality-block IR-shape variants** (inline helper functions, combined
  if-chains, combined declarations, `else if`, ternary, `>=`) and
  **`imageFactory` store-shape variants** (casts, `memset`, reference,
  volatile) with code-size and 32-bit-instruction counts (§4.6–4.7).

## 6. What this proves

1. **The baseline is a genuine local optimum under a much wider neighbourhood
   than w4510 measured**: exhaustive 8! settings-tail permutation with the
   baseline blocks, exhaustive quality-position sweep, exhaustive
   `imageFactory`-position sweep, 12 000 WIN-late shuffles, exhaustive WIN
   two-move reorder, and the IR-shape variants of §4.6–4.9. Every one is
   ≤ 80.5.
2. **The settings tail is a plateau, not a peak**: 15+ tail permutations give
   exactly 80.5. This is consistent with w4510's "scheduler-determined" finding,
   but the plateau is flat at 80.5 rather than at the WIN 78.3.
3. **The WIN early block is the correct early block** (96/96, 31/31 pool,
   `movw r6,#583` hoist), but its late block is a strict local optimum at
   77.9/78.3. The two local optima are still mutually exclusive: combining
   them would give `M = 96 + 49 + 39 = 184 → 81.4`.
4. **The only strict improvement left is the 4-byte code-size delta** (the two
   `imageFactory` `ip` forms), worth 80.5 → 80.9. It is a register-allocation
   outcome, not a statement-order one.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w4810
cmake --build cmake-build-match --target check -j8
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN7GlobalsC2Ev
# Ghidra decompile of the real ctor (image base 0x10000):
/tmp/ghvenv/bin/python /home/debian/GalaxyOnFire2/.tmp/ghidra_decomp2.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so_ghidra \
  android_2.0.16_libgof2hdaa.so_ghidra / android_2.0.16_libgof2hdaa.so 0xf31e0
```

Scratch harness (not tracked, `/dev/shm/w4810`): `cc.sh` (exact repo compile
line), `gen.py` (token→statement body generator), `feval.py` (single-TU scorer,
~0.4 s/eval), `tailsearch.py` (exhaustive 8! tail search with the baseline
blocks), `sweep_q.py` (quality-position sweep), `sweep_if.py`
(`imageFactory`-position sweep), `winlate.py` (12 000 WIN-late shuffles),
`winmove2.py` (exhaustive WIN two-move reorder), `test_qshape.py`,
`test_q2.py`, `test_helper.py`, `test_ref.py`, `test_cast.py`, `test_early.py`,
`test_mach.py`, `poolorder.py`, `traceboth2.py` (store-order tracer),
`instdiff.py` / `list32.py` (instruction-width diff).

## 8. What remains

1. **Recover the two `imageFactory` `ip` forms** (`ldr.w ip,[r0]` for
   `&imageFactory`, `str.w r2,[ip]` for its store). This is the only route to
   removing the two `nop`s and it alone is worth 80.5 → 80.9. It needs a source
   shape that makes the scheduler defer the `imageFactory` store past the
   quality block while keeping its GOT load at pool slot 21. Moving the
   statement later in source moves the GOT load too (pool 6/31 measured), so
   this is a register-pressure problem, not an ordering one.
2. **Combine the WIN early block with a 49/90 late block.** The WIN early order
   is the correct one (96/96, 31/31, hoist). The missing piece is a late-block
   IR shape that keeps the WIN prologue/pool while aligning the zero-init
   group to the original's indices. All 40 320 tail permutations, 342 WIN
   two-move reorders and 12 000 WIN full shuffles leave this open.
