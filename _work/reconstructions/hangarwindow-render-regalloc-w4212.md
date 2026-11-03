# `HangarWindow::render` — pass 6: register-tie sweep + second-`&layout` GOT-load correction (w4212)

Symbol `_ZN12HangarWindow6renderEv`, original `0x1490d8`, size `0x25e4` (9700 B).
Branch `wave92-w4212`, base master `b379d00d`. Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so` (image base 0x10000 in
Ghidra; ELF addresses below are the real ones). This pass is source-shape analysis; **no code
change landed** (no shape beats 80.6 %, and `byte_exact` only rises if `render` becomes
byte-identical, which it does not).

## TL;DR

Baseline re-measured in this tree: **80.6 % / 9700 B** (raw difflib
`0.8064427312775331`, 3626 vs 3638 normalized insns), gate **byte 4498 / linked 4498 /
size 4524 / stub 0 / missing 0 / wrong_type 0 / extra 0 / data 843 / imports 207**,
lint CLEAN, drift 0, LSDA **155 rows** (original **156**).

New results this pass:

1. **Correction to the w4109/w4212 handoff.** The original's *second* `&Globals::layout`
   GOT load is **not** at `0x14aa7c` and **not** inside the credit-dialog list loop. The
   *load instruction* is at **`0x14a85a`**, immediately before `Globals::layout->drawFooter()`
   and immediately after the `if (viewMode == 1)` recursive-`render()` block. `0x14aa7c` is
   merely the **literal-pool word** (`0x000c59c4`) that the `ldr r0,[pc,#544]` reads; the
   `add r0,pc` at `0x14a85c` (PC = `0x14a860`) turns it into GOT slot **`0x210224`**, which
   `readelf -r` confirms is `R_ARM_GLOB_DAT _ZN7Globals6layoutE`. The *first* layout load at
   `0x149132` uses pool word `0x149420` (`0x000c70ec`) and reaches the **same** GOT slot
   `0x210224` (PC = `0x149138`). So the original really does reload `&Globals::layout`
   twice; the second reload is a *rematerialisation at `drawFooter`*, not a credit-dialog
   access.
2. **The baseline body already matches the original's 38 `[sl]` reloads exactly.** Both the
   original and the current baseline execute `ldr.w rX,[sl]` **38** times (`sl` =
   `&Globals::layout`). The earlier "orig 17 / ours 18" figure was measured on the *lazy*
   build, not the baseline. The baseline's only layout-handling deltas are therefore:
   (a) the prologue GOT load + value cache in `r4` (4 extra instructions), (b) `mov r0,r4`
   at `drawBG` instead of `ldr.w r0,[sl]`, (c) the missing `drawFooter` rematerialisation.
3. **The tie is decided by the entry block, not by the body.** Every source shape that puts
   a `Globals::layout` load in the entry block (before the first call) gives
   `this -> fp (r11)`, `&layout -> sl (r10)`, frame **376** — the original's roles. Every
   shape that defers the load past `SetColor`/`GetApplicationModule` gives
   `this -> sl (r10)`, `&layout -> fp (r11)`, frame **384**. This is true even when the
   *values* produced later are identical. Variant **D** (`Layout *layout = Globals::layout;`
   placed after `GetApplicationModule`, before the `if`) is the closest near-miss: its first
   22 instructions are instruction-identical to the original (only the PC-relative literal
   offsets differ by 4), `this -> fp`, frame 376, and only the layout load is 2 instructions early (`0x149124` vs `0x149136`) — yet it scores 80.5 %,
   *below* baseline, because the 4-byte code shift moves every intra-function branch target.
4. **A faithful fully-lazy body does not by itself break the tie.** Variant T (lazy, no
   `hintLayout`, no pads, no self-assigns, getHeight-first) is 384/sl; variant S (same but
   width-first) is 384/**fp** — so the free-credits `getWidth`/`getHeight` order can flip the
   tie, but S is 52 B short of the original and scores only 51.6 %.
5. **No strict improvement found.** 40+ measured shapes; the baseline `0.8064427312775331`
   is a robust local optimum. Several expression-form changes are *exactly* score-neutral
   (`if (!viewMode)`, `if (!station || ...)`, `if (0 < barHeight)`, `if (upgradeMode == 0)`,
   `if (isSelectable() != 0)`, `if (right > 0)`, `if (0 < left)`) — the compiler emits the
   same bytes.

## 0. Baseline (canonical, this tree)

```
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  wrong_type 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS; lint CLEAN; drift 0
render: 9700/9700 B, pct 80.6, raw 0.8064427312775331, insns 3626/3638
LSDA:   155 rows (original 156)
```

Environment (needed to reproduce):

```sh
cmake -S . -B cmake-build-match \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
```

## 1. The exact prologue tie

Original (`objdump -d --start-address=0x1490d8 --stop-address=0x149150`):

```
1490d8: push {r4,r5,r6,r7,lr}; add r7,sp,#12; stmdb sp!,{r7,r8,r9,sl,fp}
1490e0: vpush {d8-d10}
1490e4: sub  sp, #376
1490e6: mov  fp, r0                      ; this -> fp
1490e8: ldr  r0,[pc,#808]  ; pool 149414 ; stack-chk-guard GOT
1490ea: ldr  r1,[pc,#812]  ; pool 149418 ; Canvas GOT
1490ec: add  r0,pc
1490ee: add  r1,pc
1490f0: ldr  r0,[r0,#0]
1490f2: ldr  r2,[r1,#0]
1490f4: str  r2,[sp,#140]
1490f6: str  r0,[sp,#112]
1490f8: ldr  r1,[r0,#0]
1490fa: ldr  r0,[r2,#0]
1490fc: str  r1,[sp,#372]
1490fe: mov.w r1,#-1
149102: blx  SetColor
149106: ldr  r0,[pc,#788]  ; pool 14941c ; &Globals::appManager
149108: movs r1,#5
14910a: add  r0,pc
14910c: ldr.w r8,[r0]                     ; r8 = &appManager (GOT entry)
149110: ldr.w r0,[r8]
149114: blx  GetApplicationModule
149118: cmp  r0,#0
14911a: str.w fp,[sp,#136]                ; spill this
14911e: itt ne; ldrbne r0,[r0,#24]; cmpne r0,#0
149124: bne.n 1491cc
149126: mov  r6,fp
149128: ldr.w r0,[r6,#88]!                ; this->viewMode
14912c: cmp  r0,#0
14912e: bne.w 14a83c
149132: ldr  r0,[pc,#748]  ; pool 149420 ; &Globals::layout (1st)
149134: add  r0,pc
149136: ldr.w sl,[r0]                     ; &layout -> sl
14913a: ldr.w r0,[sl]                     ; layout value
14913e: blx  drawBG
...
14a85a: ldr  r0,[pc,#544]  ; pool 14aa7c ; &Globals::layout (2nd)
14a85c: add  r0,pc
14a85e: ldr.w sl,[r0]                     ; &layout -> sl again
14a862: ldr.w r0,[sl]
14a866: blx  drawFooter
```

Baseline (current source, `Layout *layout = Globals::layout;` at the top, used only at
`drawBG`):

```
1490e4: sub  sp, #376
1490e6: mov  fp, r0                       ; this -> fp  (same as original)
1490e8: ldr  r0,[pc,#816]  ; pool 14941c
1490ea: ldr  r1,[pc,#820]  ; pool 149420
1490ec: ldr  r2,[pc,#820]  ; pool 149424
1490ee: add  r0,pc
1490f0: add  r1,pc
1490f2: add  r2,pc
1490f4: ldr  r0,[r0,#0]
1490f6: ldr.w sl,[r1]                     ; &layout -> sl  (extra)
1490fa: ldr  r2,[r2,#0]
1490fc: str  r2,[sp,#140]
1490fe: str  r0,[sp,#112]
149100: ldr  r1,[r0,#0]
149102: ldr  r0,[r2,#0]
149104: ldr.w r4,[sl]                     ; layout value -> r4 (extra)
149108: str  r1,[sp,#372]
14910a: mov.w r1,#-1
14910e: blx  SetColor
...
14913e: mov  r0, r4                       ; cached value at drawBG
149140: blx  drawBG
```

Lazy shape (`Layout *layout` removed; `Globals::layout->drawBG()`):

```
1490e4: sub  sp, #384                     ; +8
1490e6: mov  sl, r0                       ; this -> sl  (swapped)
1490e8: ldr  r0,[pc,#816]  ; pool 14941c
1490ea: ldr  r1,[pc,#820]  ; pool 149420
1490ec: add  r0,pc
1490ee: add  r1,pc
1490f0: ldr  r0,[r0,#0]
1490f2: ldr  r2,[r1,#0]
1490f4: str  r2,[sp,#148]
1490f6: str  r0,[sp,#120]
1490f8: ldr  r1,[r0,#0]
1490fa: ldr  r0,[r2,#0]
1490fc: str  r1,[sp,#380]
1490fe: mov.w r1,#-1
149102: blx  SetColor
...
14911a: str.w sl,[sp,#144]
...
149136: ldr.w fp,[r0]                     ; &layout -> fp
14913a: ldr.w r0,[fp]
14913e: blx  drawBG
```

So the whole prologue is identical modulo `fp<->sl` and the 8-byte frame. The baseline is
the lazy body **plus** a top-level pin that forces the original's `this=fp` role at the cost
of one extra GOT load and one cached value.

## 2. The second `&Globals::layout` GOT load — corrected

`readelf -r` on the original:

```
00210224  0008a615 R_ARM_GLOB_DAT    002181d4   _ZN7Globals6layoutE
```

Decoding both PIC idioms (Thumb `ldr rX,[pc,#imm]` uses `PC = (addr+4) & ~3`;
`add rX,pc` uses `PC = (addr+4) & ~3`):

| site | pool word | `add` PC | `pool + PC` | GOT slot |
|---|---|---|---|---|
| `0x149132` (drawBG) | `0x149420` = `0x000c70ec` | `0x149138` | `0x210224` | `Globals::layout` |
| `0x14a85a` (drawFooter) | `0x14aa7c` = `0x000c59c4` | `0x14a860` | `0x210224` | `Globals::layout` |

Both hit the same slot. The second is a **rematerialisation after the `if (viewMode == 1)`
recursive-`render()` block**, immediately before `drawFooter`:

```
14a83c: cmp  r0,#1
14a83e: bne.n 14a85a
14a840: movs r0,#0
14a842: str.w r0,[fp,#88]      ; viewMode = 0
14a846: mov  r0,fp
14a848: blx  render            ; recursive
14a84c: ldr.w r0,[fp,#24]      ; listItemWindow
14a850: movs r1,#1
14a852: str.w r1,[fp,#88]      ; viewMode = 1
14a856: blx  ListItemWindow::draw
14a85a: ldr  r0,[pc,#544]      ; <-- second &Globals::layout
14a85c: add  r0,pc
14a85e: ldr.w sl,[r0]
14a862: ldr.w r0,[sl]
14a866: blx  drawFooter
```

The baseline uses `sl` (still live) at `drawFooter` and therefore has only **one** layout GOT
load. Measured with the verifier's normalised pool names:

```
onorm.txt: 2 x pool:pic:got:_ZN7Globals6layoutE   (orig)
unorm.txt: 1 x pool:pic:got:_ZN7Globals6layoutE   (baseline)
```

The only source shapes that reproduce the second load are the *lazy* ones (variant G4:
`Layout *layout = Globals::layout;` declared immediately before `drawFooter`), and they all
also flip the tie to `this=sl` / frame 384, scoring 61.6 %.

## 3. Why the tie flips — allocator evidence

`-mllvm -print-machineinstrs=greedy`, after Greedy RA, `bb.0` of `render`:

Baseline (layout load in the entry block):

```
16B   %3319:gprnopc = COPY $r0                       ; this
32B   %613:tgpr = tLDRLIT_ga_pcrel @__stack_chk_guard
48B   %614:tgpr = tLDRLIT_ga_pcrel @_ZN7Globals6CanvasE
...   tBL SetColor
240B  %620:tgpr = tLDRLIT_ga_pcrel @_ZN7Globals10appManagerE
...   tBL GetApplicationModule
404B  t2STRi12 %3319:gprnopc, %stack.78, 0            ; spill this
```

Lazy (no layout load in the entry block):

```
16B   %3319:gprnopc = COPY $r0
32B   %613:tgpr = tLDRLIT_ga_pcrel @__stack_chk_guard
48B   %614:tgpr = tLDRLIT_ga_pcrel @_ZN7Globals6CanvasE
...
```

Both entry blocks are byte-for-byte the same instruction sequence; the difference is the
layout GOT load, which in the baseline is *sunk into `bb.4`* in the pre-RA IR but re-hoisted
by the scheduler into the prologue. Empirically the rule is sharp:

* layout load present in the entry block ⇒ `this -> r11 (fp)`, `&layout -> r10 (sl)`,
  frame 376 (original's roles);
* layout load deferred past the first call ⇒ `this -> r10 (sl)`, `&layout -> r11 (fp)`,
  frame 384.

`-regalloc-csr-first-time-cost=0..100` does not change the prologue. `-debug-only=regalloc`
is unavailable in this release clang; `-print-machineinstrs=<pass>` accepts only `greedy`
(the live-interval pass name is not exposed).

Frame-object deltas (after Greedy, non-dead `fi#`): baseline has 3 extra size-4 frame objects
(`fi#130..132`) yet a *smaller* frame (376 vs 384) — the extra objects are stack-slot spills
that colour away, so the 8-byte delta is a spill-count consequence, not a source-level object.

## 4. Ghidra evidence for the body

`decompile_function` of `render` (Ghidra base 0x10000, so Ghidra `0x1590d8` = ELF
`0x1490d8`) gives the fully-lazy shape:

```c
local_44 = *piVar4;                       /* stack canary */
AbyssEngine::PaintCanvas::SetColor(*puVar21);
puVar40 = *(undefined4 **)(DAT_0015941c + 0x15910e);
iVar5 = ApplicationManager::GetApplicationModule((ApplicationManager *)*puVar40, 5);
cVar1 = '\0';
if (iVar5 != 0) cVar1 = *(char *)(iVar5 + 0x18);
if (iVar5 == 0 || cVar1 == '\0') {
    iVar5 = *(int *)(this + 0x58);        /* viewMode */
    if (iVar5 != 0) goto LAB_0015a83c;
    piVar43 = *(int **)(DAT_00159420 + 0x159138);
    Layout::drawBG((Layout *)*piVar43);   /* <-- direct global deref, no local */
    ...
```

Selected-item amount block (Ghidra lines ~455-475) is a plain `if/else` on `iVar5 == 1`:

```c
if (iVar5 == 1) { right = getStationAmount(...); left = getAmount(...); }
else            { right = getAmount(...); left = getCurrentAmount(...); }
```

yet the original LSDA has **six** isolated `+4` rows for those three call sites (see w4109
§3). The ternary form reproduces the six rows but shrinks the body by 24 B.

Free-credits loop (Ghidra lines ~900-990) is `getWidth` **then** `getHeight`, with
`textY = (int)((float)y + (float)height * 0.2f)` and no `volatile`. The current source has
`getHeight` first and a `volatile float textYf`; both are load-bearing for the 80.6 % score
(variant BA width-first: 0.6199/9744; variant BB non-volatile: 0.6162/9748).

`iVar11 = 3 - (*(int *)(this_02 + 0x3c) >> 0x1f);` — the `inTabIndex == -1 ? 4 : 3`
computation is a signed-shift form (`inTabIndex < 0 ? 4 : 3`), but variant CA measures
0.8060030 (slightly worse) and variant CB 0.8060030; the current `== -1` form is better.

## 5. Variant sweep (official verifier, `autojunk=False`)

Baseline `0.8064427312775331 / 9700 / this=fp / &layout=sl(prologue) / frame 376`.

### 5.1 Measured this pass (all on the baseline body unless noted)

| variant | raw | size | frame / this | note |
|---|---:|---:|---|---|
| **baseline** | **0.8064427** | **9700** | 376 / fp | reference |
| D: local after `GetApplicationModule`, before `if` | 0.8050661 | 9700 | 376 / fp | prologue identical to orig through 22 insns; load at `0x149124` |
| D + ternary amount | 0.7797685 | 9676 | 376 / fp | |
| D + remove `hintLayout` | 0.7697812 | 9716 | 376 / fp | |
| D + ternary + remove `hintLayout` | 0.7901982 | 9700 | 376 / fp | |
| D + fully faithful body | 0.6591347 | 9660 | 376 / fp | |
| D + local used in `listWidth` | 0.7736161 | 9704 | 376 / fp | |
| D + local in `listWidth`, drawBG via `Globals` | 0.7772734 | 9724 | 376 / fp | |
| B: local inside `if (station...)` | 0.6161242 | 9724 | 384 / sl | |
| C: local inside `if (viewMode==0)` | 0.6161242 | 9724 | 384 / sl | |
| E: declare at top, assign before drawBG | 0.6161242 | 9724 | 384 / sl | |
| G4: lazy, local declared before `drawFooter` | 0.6161242 | 9724 | 384 / sl | **2nd layout GOT load reproduced** |
| E1: top local used for *all* layout accesses | 0.4524008 | 9556 | — | |
| E2: local inside viewMode block, all accesses | 0.4715492 | 9624 | — | |
| E4: baseline + `hintLayout` removed, local used there | 0.6593497 | 9720 | 376 / r9 | |
| E5: baseline + local used in `listWidth` | 0.7735096 | 9708 | 376 / fp | |
| E6: baseline + local used at `drawHeader` | 0.6121212 | 9700 | 376 / fp | |
| E7: baseline + local used at `drawScrollBar` | 0.5881218 | 9696 | 376 / fp | |
| G1: baseline + local at `drawFooter` | 0.6036904 | 9692 | 376 / fp | |
| G2: baseline + scoped `footerLayout` at `drawFooter` | 0.8064427 | 9700 | 376 / fp | compiler folds it away |
| G3: baseline + `layout = Globals::layout` before `drawFooter` | 0.8064427 | 9700 | 376 / fp | folded away |
| G5: `if (viewMode==1 && Globals::layout != nullptr)` | 0.6152787 | 9716 | 376 / fp | |
| H1: `if (!this->viewMode)` | 0.8064427 | 9700 | 376 / fp | neutral |
| H2: `if (this->viewMode != 0)` (second block) | 0.7912421 | 9696 | 376 / fp | worse |
| H3: `if (!station || ...)` | 0.8064427 | 9700 | 376 / fp | neutral |
| H4: `if (0 < barHeight)` | 0.8064427 | 9700 | 376 / fp | neutral |
| H6: `if (this->upgradeMode == 0)` | 0.8064427 | 9700 | 376 / fp | neutral |
| H7: `if (item->isSelectable() != 0)` | 0.8064427 | 9700 | 376 / fp | neutral |

### 5.2 Prior passes (re-cited, measured in this tree)

| variant | raw | size | frame / this |
|---|---:|---:|---|
| lazy (no top local) | 0.6161242 | 9724 | 384 / sl |
| lazy + `hintLayout` removed | 0.6258073 | 9712 | 384 / sl |
| lazy + no hint + ternary | 0.6351389 | 9700 | 384 / sl |
| lazy + no hint + width-first | 0.5286913 | 9664 | 384 / fp |
| lazy + no hint + ternary + width-first | 0.5305449 | 9668 | 384 / fp |
| ternary only (if/else→`?:`) | 0.7805954 | 9676 | 376 / fp |
| `ApplicationManager *am` value cache + lazy | 0.7329569 | 9688 | 376 / fp |
| `Layout *const layout` | 0.8064427 | 9700 | 376 / fp |
| `(*layout).drawBG()` | 0.8064427 | 9700 | 376 / fp |
| `Layout *volatile layout` | 0.8064427 | 9700 | 376 / fp (prologue shifts) |
| `volatile Layout *layout` | 0.8064427 | 9700 | 376 / fp |
| baseline + `if (layout == nullptr) return;` | — | 9704 | 376 / fp |
| baseline, remove `hintLayout` | 0.7706069 | 9716 | 376 / fp |
| baseline, remove hint + ternary | 0.7904736 | 9700 | 376 / fp |
| baseline, remove hint + pads + self-assigns + width-first + ternary | 0.6597 | 9660 | 376 / fp |
| fully faithful lazy, no hint/pads/self-assigns, getHeight-first | 0.6027 | 9664 | 384 / sl |
| fully faithful lazy, no hint/pads/self-assigns, width-first | 0.5162 | 9648 | 384 / fp |
| baseline + ternary | 0.7805954 | 9676 | 376 / fp |
| lazy + ternary | 0.5937 | 9708 | — |
| top local used in hint loop, drawBG via `Globals` | 0.7644 | 9704 | 376 / fp |
| same but drawBG via local | 0.6593 | 9720 | — |
| `getWidth` before `getHeight` | 0.6199 | 9744 | — |
| remove `volatile textYf` | 0.6162 | 9748 | — |
| `right > 0` / `0 < left` | 0.8064427 | 9700 | 376 / fp |
| `inTabIndex < 0 ? 4 : 3` | 0.8060030 | 9700 | 376 / fp |
| `inTabIndex >= 0 ? 3 : 4` | 0.8060030 | 9700 | 376 / fp |
| non-volatile `textYf` | 0.6162 | 9748 | — |

### 5.3 Diff classification (baseline vs original, verifier-normalised stream)

Using `tools/verify/verify.py::normalize` + `reconcile`:

```
normalised lengths 3626 (orig) / 3638 (ours), ratio 80.8
matched instructions 2934
475 diff blocks:
  112 blocks / 113 insns are *shift-only* (identical text once loc_XXXX is masked)
  363 blocks / 756 insns are real differences
```

The real differences are dominated by (a) the 4-instruction prologue insertion, (b) stack
slot swaps such as `str r0,[sp,#92]; str r6,[sp,#96]` vs the reverse, (c) body register-role
swaps (`mov r3,r9` vs `mov r3,fp`, `cmp fp,sl` vs `cmp sl,r8`, `ldr.w sl,[r4,#40]` vs
`ldr.w fp,[r4,#40]`), and (d) the EH cleanup tail (the largest single block is a 110-entry
`replace` at the end, caused by the extra cleanup scopes the `pad0`/`pad0b` anchor adds).
Because the verifier compares intra-function branch targets *literally* (`loc_XXXX`), the
4-byte prologue shift alone costs a large fraction of the score — which is why variant D
(prologue closer, body shifted) loses to the baseline (prologue worse, body offsets closer).

## 6. Proven vs hypothesis

Proven (measured in this tree):

* Baseline is `0.8064427312775331 / 9700`; gate byte 4498 / linked 4498 / size 4524 /
  data 843 / imports 207; lint CLEAN; drift 0; LSDA 155 rows (orig 156).
* The original's prologue is the fully-lazy shape (`this -> fp`, frame 376, two GOT loads,
  `&layout -> sl` loaded lazily at `drawBG`), and it reloads `&Globals::layout` a **second**
  time at `0x14a85a` immediately before `drawFooter`; both loads hit GOT slot `0x210224`
  (`_ZN7Globals6layoutE`). The earlier "inside the credit-dialog loop at `0x14aa7c`"
  description is wrong: `0x14aa7c` is only the pool word.
* Baseline and original both execute exactly **38** `ldr.w rX,[sl]` layout-value reloads.
* The register tie is decided by whether a `Globals::layout` load exists in the entry block:
  present ⇒ `this=fp`/`&layout=sl`/frame 376; deferred ⇒ `this=sl`/`&layout=fp`/frame 384.
  This holds across 40+ source shapes, including all declaration-order and scope
  permutations tried.
* Variant D is the closest near-miss (prologue instruction-identical for 22 instructions
  apart from the PC-relative literal offsets,
  `this=fp`, frame 376) but scores 80.5 % because its 4-byte shift moves every
  intra-function branch target.
* The selected-item amount block is a plain `if/else` in the Ghidra source yet has six
  isolated `+4` EH rows in the binary; the ternary form reproduces the row pattern but
  shrinks the body 24 B.
* The `pad0`/`pad0b` credit-dialog anchor adds EH scopes the original does not have (the
  original has one contiguous `14ab80 +210` region; ours splits it).
* Every anchor in the current source is load-bearing for the 80.6 % score.

Hypothesis / open:

* The original is the fully-lazy source with the ternary amount block. To land it we still
  need the greedy allocator to pick `fp` for `this` in the lazy IR *and* keep the 376-byte
  frame. No plain-C++ shape tried does this without also adding a non-faithful entry-block
  value; the tie appears to be decided by the entry-block live-interval ordering, which the
  faithful body does not reproduce on its own.
* Once the tie is flipped, the remaining body deltas to chase are (a) the `if (left > 0)`
  EH-scope fold (`+16`+`+32` → `+154`), (b) the blueprint-branch `+144`/`+46` split, and
  (c) the credit-dialog contiguous region.

## 7. Commands

```sh
cmake --build cmake-build-match --target check -j8   # RATCHET PASS byte 4498 linked 4498
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN12HangarWindow6renderEv
python3 _work/reconstructions/gameresource_gen/lsda.py \
  cmake-build-match/libgof2hdaa.so _ZN12HangarWindow6renderEv
```

Scratch (this pass): `/dev/shm/w4212/{variant.py,batch*.py,score2.sh,raw.py,
normdiff.py,classify.py,gots.py,pmi.sh,DecompRender.java,orig_render.c}`.
