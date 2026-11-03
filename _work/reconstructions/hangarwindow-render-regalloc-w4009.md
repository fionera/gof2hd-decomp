# `HangarWindow::render` — pass 4: lazy-layout tie, free-credits order, LSDA rows (w4009)

Symbol `_ZN12HangarWindow6renderEv`, original `0x1490d8`, size `0x25e4` (9700 B).
Branch `wave90-w4009`, base master `360c7467`. Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so` (Ghidra
`android_2.0.16_libgof2hdaa.so`, image base 0x10000). This pass is source-shape
and register-allocation analysis only; **no code change landed**.

## TL;DR

No source shape beats the current tree. Baseline is **80.6 % / 9700 B**
(raw difflib `0.8064427312775331`, 3626 vs 3638 normalized insns), gate
byte 4498 / linked 4498 / size 4524 / data 843 / imports 207, lint CLEAN,
drift 0, `.text 0x1337c8`, `.rodata 0x41b67`, `.ARM.exidx 0x6f70`,
`.ARM.extab 0x26290` (original `0x26298`, −8).

New results in this pass:

1. The original's three long-lived callee-saved values are
   **`this = fp`, `Globals::appManager = r8`, `&Globals::layout = sl`**.
   The current baseline matches all three roles (but caches the *value* of
   `Globals::layout` in `r4`); every faithful lazy shape either swaps
   `this`/`&layout` or moves `appManager` into `sl`.
2. A **top-level `Layout *layout = Globals::layout;` whose value is used at
   `drawBG`** pins `this = fp` even when every other use is a direct
   `Globals::layout->` (40 `[sl]` derefs vs the original's 38). This is the
   closest lazy shape (raw `0.7706068529`, 9716 B), but it materialises the
   `&Globals::layout` GOT address in the prologue instead of at `drawBG`,
   which shifts every later branch target and costs ~3.6 points.
3. Making the free-credits loop **width-first with a `rowHeight` local**
   (`getWidth` → `layout->field_0x2c_rowHeight` → `getHeight`) also flips the
   tie to `this = fp` with a genuinely lazy `&Globals::layout` at `drawBG`,
   but it moves `appManager` from `r8` to `sl`, grows the frame to 384 and
   cascades into a large body diff (raw `0.5286913444`, 9664 B).
4. The original reloads the `Globals::layout` pointer value far more often
   than the current tree: **38 `ldr [sl]` sites** in the original vs
   **8** in the baseline, **11** in the faithful lazy shape and **40** in the
   pin-at-drawBG shape. The baseline's `r4` cache is the main body-level
   divergence.
5. The original's `.ARM.extab` record for `render` is 948 B / 156 rows;
   ours is 928 B / 153 rows. The six missing rows are the isolated 4-byte
   `getAmount()`/`getStationAmount()` call sites; their landing pads are all
   plain `b.n 14b698` (resume), so the isolation is an EH-scope difference,
   not a cleanup body difference.

## 0. Baseline (this tree, canonical)

```
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS; lint CLEAN; drift 0
.text 0x1337c8 @ 0x69440, .ARM.exidx 0x6f70, .ARM.extab 0x26290 @ 0x1a3b78,
.rodata 0x41b67 @ 0x1c9e10
render: 9700/9700 B, pct 80.6, raw 0.8064427312775331, insns 3626/3638
```

Baseline source anchors (all load-bearing; removing any regresses):

* `Layout *layout = Globals::layout;` at the top of `render()`;
* `Layout *hintLayout = Globals::layout;` before the hint loop;
* `volatile int pad3 = 0;` before `buttons[8]->draw()`;
* `volatile int pad0/pad0b` around `setSplitText`;
* three `description = description;` in the credit-dialog block;
* `volatile float textYf`.

## 1. Register roles: original vs the three closest shapes

The original prologue (Thumb, `od.txt`):

```
1490e6: mov  fp, r0                 ; this            -> fp
14910c: ldr.w r8, [r0]              ; Globals::appManager -> r8
149136: ldr.w sl, [r0]              ; &Globals::layout    -> sl  (at drawBG)
14913a: ldr.w r0, [sl]              ; layout value (lazy, at drawBG)
14913e: blx   Layout::drawBG
```

| shape | `this` | `appManager` | `&Globals::layout` | layout value | frame | raw |
|---|---|---|---|---|---:|---:|
| **original** | **fp** | **r8** | **sl** (at drawBG) | reload `[sl]` per use (38×) | 376 | — |
| **baseline** | **fp** | **r8** | **sl** (prologue) | cached in `r4` (8 `[sl]`) | 376 | **0.8064427** |
| lazy (no top local) | sl | r8 | fp (at drawBG) | reload `[fp]` (11×) | 384 | 0.6258073 |
| lazy + no hint | sl | r8 | fp (at drawBG) | reload `[fp]` (11×) | 384 | 0.6258073 |
| pin-at-drawBG (`nohint`/v30/v33) | **fp** | **r8** | **sl** (prologue) | reload `[sl]` (40×) | 376 | 0.7706069 |
| lazy + width-first free-credits | **fp** | sl | spilled (at drawBG) | reload `[sl]` | 384 | 0.5286913 |

The baseline is the only shape that gets all of `this = fp`,
`appManager = r8` and `&layout = sl` right. The pin-at-drawBG shape gets
`this = fp` and `&layout = sl` right but materialises the GOT address one
basic block early.

### 1.1 The pin is the top *value* load used at `drawBG`

`mkvariant33.py` (keep the top local, use it only at a chosen site, direct
`Globals::layout->` everywhere else) isolates the mechanism:

| top-local use kept | raw | size |
|---|---:|---:|
| `drawBG` only | 0.7706069 | 9716 |
| `drawBG` + `drawFooter` | 0.7706069 | 9716 |
| `drawBG` + `drawHeader`/`drawScrollBar` | 0.7706069 | 9716 |
| `drawBG` + all `drawBox` calls | 0.7706069 | 9716 |
| `drawFooter` only | 0.6258073 | 9712 |
| `drawBox` only | 0.6258073 | 9712 |

So the *only* thing that pins `this = fp` is a top-level load of the layout
pointer whose result is consumed at `drawBG`. `drawFooter` and later uses do
not matter. This is consistent with the original: its first layout use is
`drawBG`, and `this` is live from entry.

The pin-at-drawBG prologue (ours) vs the original:

```
ours:                                      original:
1490e8: ldr  r0, [pc]   ; guard            1490e8: ldr  r0, [pc]   ; guard
1490ea: ldr  r1, [pc]   ; Canvas           1490ea: ldr  r1, [pc]   ; Canvas
1490ec: ldr  r2, [pc]   ; &layout           ...
1490ee: add  r0, pc                         1490ec: add  r0, pc
1490f0: add  r1, pc                         1490ee: add  r1, pc
1490f2: add  r2, pc
1490f4: ldr  r0, [r0]   ; guard addr        1490f0: ldr  r0, [r0]
1490f6: ldr.w sl, [r1]  ; &layout           1490f2: ldr  r2, [r1]
1490fa: ldr  r2, [r2]   ; Canvas addr       ...
1490fc: str  r2, [sp,#140]                  1490f4: str  r2, [sp,#140]
1490fe: str  r0, [sp,#108]                  1490f6: str  r0, [sp,#112]
149100: ldr  r1, [r0]                        1490f8: ldr  r1, [r0]
149102: ldr  r0, [r2]                        1490fa: ldr  r0, [r2]
149104: ldr.w r4, [sl]  ; layout value      1490fc: str  r1, [sp,#372]
149108: str  r1, [sp,#372]                  1490fe: mov.w r1, #-1
14910a: mov.w r1, #-1                       149102: blx  SetColor
14910e: blx  SetColor
```

The extra `ldr r2,[pc]`/`add r2,pc`/`ldr.w sl,[r1]`/`ldr.w r4,[sl]` group
shifts every later branch target by 2 instructions; the original's group is
at `drawBG` instead. The stack slots also shift by 4 (`sp+108` vs
`sp+112` for the guard address).

### 1.2 The faithful lazy shape always swaps `this` and `&layout`

`mkvariant.py lazy` (remove the top local, use `Globals::layout->drawBG()`)
gives:

```
1490e6: mov  sl, r0        ; this -> sl
...
149136: ldr.w fp, [r0]    ; &Globals::layout -> fp
14913a: ldr.w r0, [fp]
14913e: blx   Layout::drawBG
```

Every attempted way to keep `this = fp` while leaving the layout address
lazy failed: `HangarWindow *self = this` (plain, round-trip via `uintptr_t`,
null-checked, volatile), `HangarWindow &self = *this`, `Layout *&layoutRef`,
`Layout **layoutPtr` (live throughout), `Layout *volatile`, null-check on
`Globals::layout`, `__builtin_assume`, and hoisting `w`/`h`/`viewMode`/
`hintInset`/field locals. All score 0.45–0.66.

### 1.3 Pointer-load counts (body-level divergence)

Counts of `ldr [reg]` where `reg` holds `&Globals::layout`:

| shape | `[sl]` | `[fp]` |
|---|---:|---:|
| original | 38 | — |
| baseline | 8 | — |
| lazy / lazy+nohint | — | 11 |
| pin-at-drawBG | 40 | — |

The original's 38 reloads are the direct `Globals::layout->` uses; the
baseline's `r4` cache removes 30 of them. The pin-at-drawBG shape reproduces
the reload count (40 vs 38) but pays the prologue/branch-target shift.

## 2. Free-credits loop: width-first flips the tie

The original free-credits loop is width-first (Ghidra `render` lines ~955):

```c
iVar10 = TouchButton::getWidth(pTVar31);
iVar11 = *(int *)(*piVar43 + 0x2c);      // rowHeight
uVar34 = TouchButton::getHeight(pTVar31);
AbyssEngine::PaintCanvas::SetColor(...);
fVar8 = (float)height * 0.2f;
...
iVar11 = iVar10 + iVar19 + iVar11;       // textX, before getText
pSVar13 = GameText::getText(...);
```

Original asm (`0x14ad26`..`0x14ad90`):

```
14ad28: blx getWidth
14ad2c: mov r1, sl            ; r1 = &Globals::layout
14ad2e: mov sl, r0            ; sl = width
14ad30: ldr r0, [r1]
14ad32: ldr.w r9, [r0, #44]   ; rowHeight
14ad36: mov r0, r4
14ad38: blx getHeight
14ad3c: ldr r6, [sp, #140]    ; Canvas
14ad3e: mov r4, r0            ; r4 = height
...
14ad78: add.w r1, sl, r8      ; textX = width + baseX
14ad7e: add.w r8, r1, r9      ; textX += rowHeight   (before getText)
14ad90: blx getText
```

The current source is height-first:

```c
int height = button->getHeight();
int textX = button->getWidth() + baseX + Globals::layout->field_0x2c_rowHeight;
```

Rewriting it faithfully (`mkvariant40.py`/`mkvariant41.py`):

```c
int width = button->getWidth();
int rowHeight = Globals::layout->field_0x2c_rowHeight;
int height = button->getHeight();
int textX = width + baseX + rowHeight;      // before the getText DrawString
```

Measured:

| variant | raw | size | `this` | `appManager` | frame |
|---|---:|---:|---|---|---:|
| baseline (height-first) | 0.8064427 | 9700 | fp | r8 | 376 |
| width-first, keep top local | 0.6198541 | 9744 | fp | r8 | 376 |
| width-first, lazy + no hint | 0.5286913 | 9664 | **fp** | **sl** | 384 |

So width-first is the first faithful shape found that keeps `this = fp`
with a genuinely lazy `&Globals::layout` at `drawBG`. But it moves
`appManager` from `r8` to `sl`, spills `width` to `sp+124` (the original
keeps width in `sl`), grows the frame to 384, and cascades into a large
body diff (struct diff 1349 insns vs the baseline's 624). The baseline's
height-first order is therefore *closer overall* even though it is not
faithful to the original's statement order.

## 3. Stack-slot comparison (baseline vs original)

`[sp,#N]` use counts, baseline vs original, are nearly identical:

```
orig: 0:25 4:26 8:1 12:1 20:1 24:1 28:6 32:4 36:8 40:7 44:2 48:2 52:2
      56:2 60:3 64:2 72:3 76:4 80:7 84:2 88:24 92:6 96:6 100:4 104:12
      108:23 112:2 116:44 120:66 124:9 128:24 132:45 136:45 140:41 372:2
base: 0:24 4:26 8:1 12:1 20:1 24:1 28:6 32:4 36:6 40:7 44:2 48:2 52:2
      56:2 60:3 64:2 72:3 76:4 80:7 84:2 88:24 92:6 96:6 100:3 104:14
      108:24 112:2 116:46 120:67 124:9 128:23 132:50 136:50 140:40 372:2
      (+ extra 204:2 276:1 288:1 300:1 from the credit-dialog anchors)
```

The pin-at-drawBG shape instead has an extra slot at `sp+68` and shifts the
72–112 range by 4, which is why its raw score is lower despite the better
`[sl]` reload count.

Two residual ties in the baseline:

* `iconSpace`/`textX` slots are swapped: original
  `str textX,[sp,#92]; str iconSpace,[sp,#96]`; ours
  `str textX,[sp,#96]; str iconSpace,[sp,#92]`. Declaration order,
  `iconSpace = 0` initialisation, `else if (tab == 1 || tab == 4)`,
  parenthesisation of `textX`, and splitting the expression are all
  codegen-neutral (0.8064427313).
* Blueprint branch `ldrd` register order is reversed: original
  `ldrd sl,r9,[sp,#132]` (`sl=&layout`, `r9=this`); ours
  `ldrd r9,sl,[sp,#132]`. The stack slots are identical. Swapping the
  `if (left > 0)` / `if (right)` blocks regresses to 0.8001101 and does not
  change the `ldrd` order.

## 4. LSDA: the six isolated amount calls

`_work/reconstructions/exidx_table.py --diff` and
`gameresource_gen/lsda.py` give the original's call-site rows:

```
149656 +4 lp=14b5a2   ; getStationAmount
149660 +4 lp=14b64c   ; getAmount
149c88 +4 lp=14b5a2   ; getAmount
149e26 +4 lp=14b600   ; getStationAmount
149e30 +4 lp=14b658   ; getAmount
149f14 +4 lp=14b600   ; getAmount
```

Ours merges them (e.g. `149656 +18 lp=14b64a`). The landing pads
`14b5a2`, `14b64c`, `14b600`, `14b658` are all just `b.n 14b698`
(`_Unwind_Resume`), so the difference is the EH *scope* around each call,
not a different cleanup body. Converting the second amount block from
ternaries to `if/else` does not restore the rows (w3908 result). The
original's 948 B / 156-row record vs our 928 B / 153 rows accounts for the
`.ARM.extab` −8.

## 5. Variant sweep (all measured with the official verifier, `autojunk=False`)

Baseline `0.8064427312775331 / 9700`. New this pass (beyond w3908's table):

| variant | raw | size |
|---|---:|---:|
| top local used at `drawBG` only (v30/v33) | 0.7706069 | 9716 |
| top local used at `drawBG` + `drawFooter` | 0.7706069 | 9716 |
| top local used at `drawBG` + all `drawBox` | 0.7706069 | 9716 |
| top local used at `drawFooter` only | 0.6258073 | 9712 |
| top local used at `drawBox` only | 0.6258073 | 9712 |
| `Layout *&layoutRef = Globals::layout` throughout | 0.6258073 | 9712 |
| `Layout **layoutPtr = &Globals::layout` throughout | 0.6258073 | 9712 |
| `HangarWindow *self = (HangarWindow *)(uintptr_t)this` + lazy | 0.6258073 | 9712 |
| `HangarWindow *self = this; if (self == nullptr) return;` + lazy | 0.5184167 | 9696 |
| `HangarWindow *volatile self = this` + lazy | 0.4537770 | 10048 |
| lazy + width-first free-credits (faithful) | 0.5286913 | 9664 |
| keep top local + width-first free-credits | 0.6198541 | 9744 |
| `layout` local after `SetColor` | 0.8058921 | 9700 |
| `layout` local after `GetApplicationModule` | 0.8050661 | 9700 |
| `layout` local after the `station` check | 0.8050661 | 9700 |
| `layout` local inside the outer `if` | 0.6161242 | 9724 |
| inverted `station` condition (`if (...) {} else { body }`) | 0.8064427 | 9700 |
| `else if (tab == 1 || tab == 4)` | 0.8064427 | 9700 |
| `iconSpace = 0` initialiser | 0.8064427 | 9700 |
| `textX` parenthesised / reordered / split | 0.8064427 | 9700 |
| `if (left > 0)` block moved after `if (right)` | 0.8001101 | 9700 |

All are ≤ baseline. The baseline remains the unique best local optimum.

## 6. Proven vs hypothesis

Proven (measured in this tree):

* Baseline is 0.8064427312775331 / 9700 B; gate byte 4498 / linked 4498 /
  size 4524 / data 843 / imports 207; lint CLEAN; drift 0.
* The original's roles are `this = fp`, `appManager = r8`,
  `&Globals::layout = sl`; the baseline matches all three, the lazy shapes
  do not.
* A top-level layout value load consumed at `drawBG` is the unique source
  pin for `this = fp`; using it only at `drawFooter`/`drawBox` does not pin.
* The faithful lazy shape always swaps `this`/`&layout` (sl/fp) and scores
  ~0.62.
* The original has 38 `[sl]` layout-pointer reloads; the baseline has 8.
* The width-first free-credits shape flips the tie to `this = fp` with a
  lazy `&Globals::layout` at `drawBG`, but moves `appManager` to `sl`,
  spills `width`, grows the frame to 384 and scores 0.5287.
* The original's `render` LSDA is 948 B / 156 rows; ours 928 B / 153. The
  six missing rows are the isolated amount calls; their landing pads are
  plain resume branches.
* The `iconSpace`/`textX` slot swap and the blueprint `ldrd` order are not
  source-order sensitive.

Hypothesis / open:

* The original source is the faithful lazy shape (direct
  `Globals::layout->` everywhere, no `hintLayout`, no self-assigns), and the
  `this = fp` allocation follows from some *other* IR difference we have
  not reconstructed — most likely the six isolated `getAmount()` EH scopes
  or the credit-dialog tail. Reconstructing those may flip the tie without
  the prologue/branch-target penalty.
* Alternatively the original may have a top-level layout value load that
  the compiler rematerialises at `drawBG`; no plain-C++ shape tried makes
  clang 7 do that while keeping `appManager = r8`.

## 7. Commands

```sh
cmake --build cmake-build-match --target check -j8   # RATCHET PASS byte 4498 linked 4498
cmake --build cmake-build-match --target lint  -j8   # CLEAN
cmake --build cmake-build-match --target drift -j8   # 0 fields
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN12HangarWindow6renderEv
python3 _work/reconstructions/exidx_table.py --diff \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
python3 _work/reconstructions/gameresource_gen/lsda.py \
  cmake-build-match/libgof2hdaa.so _ZN12HangarWindow6renderEv
```

Scratch scripts used this pass: `/dev/shm/w4009/mkvariant30.py`..`mkvariant43.py`,
`score.py`, `diffsum.py`, `structdiff.py`, `normdiff2.py`, `normdiff3.py`.
