# `HangarWindow::render` — pass 8 (w4270): revert the 34e EH sink, iOS 1.1.19 dump and the exact-role lazy tie

Symbol `_ZN12HangarWindow6renderEv`, original `0x1490d8`, size `0x25e4` (9700 B).
Branch `wave92-w4270`, base master `b379d00d`. Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so` (Ghidra image base
0x10000; ELF addresses below are the real ones). This pass lands the **source + doc revert of
commit `34e76bbe`** (the `volatile padS` EH sink) and records the iOS 1.1.19 dump analysis and
the new exact-role lazy-load shapes. No new anchors, no new `volatile`.

## TL;DR

- **`34e76bbe` was a regression.** It moved the `render` EH sink from `pad0` to a
  `volatile padS` to make `.ARM.extab` size-exact, but dropped the function from
  **80.6 % to 78.8 %** (raw `0.806` -> `0.788`). The task's stated gate
  (`.ARM.extab 0x26290 (-8)`, render 80.6 %) is the pre-34e source.
- This pass reverts the `src/game/ui/HangarWindow.cpp` hunk and the matching
  `docs/IDENTICALITY.md` text. After the revert: **80.6 %, 9700 B, 3626/3638 insns,
  `.ARM.extab 0x26290 (-8)`**, gate unchanged (`byte_exact 4499`, `linked_exact 4499`,
  size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0).
- **The iOS 1.1.19 dump is a symbol/TU dump, not a source dump.** It confirms
  `HangarWindow::render` is the ninth function of `Classes/Game/HangarWindow.cpp` (after
  `render3D`, before `getRelativeScrollHeight`) and that `FREE_CREDIT_AMOUNTS` and
  `HangarWindow::lastTab` are the TU's only statics. It contains no statement order, so the
  statement order below is from the Android Ghidra decompile + binary.
- **New exact-role lazy shape.** Caching the `Globals::appManager` value in a local
  (`ApplicationManager *appManager = Globals::appManager;` after `SetColor`, used for both
  `GetApplicationModule` and `GetApplicationData`) gives the original's exact entry-block
  roles — `this -> fp`, `&Globals::layout -> sl`, frame **376**, first layout GOT load at
  `0x149132`/`ldr.w sl` at `0x149136` — but measures **9688 B / 73.5 %** because caching the
  appManager value changes the body codegen. It is the closest plain-C++ lazy shape found.

## Baseline / gate

```
verify: byte 4499  linked 4499  stubs 0  missing 0  extra 0  wrong_type 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS; lint CLEAN; drift 0
render: 9700/9700 B, pct 80.6, raw 0.8059999999999999, insns 3626/3638
sections: .text 0x1337c8, .ARM.exidx 0x6f70, .ARM.extab 0x26290 (-8), .rodata 0x41b67 / base 0x1c9e10
```

Environment (canonical build):

```sh
cmake -S . -B cmake-build-match \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
```

## 1. The 34e regression (measured)

`34e76bbe` changed, in `render`:

```diff
@@ item-kind test @@
                         if (item->isItem() || item->isShip() || item->isBluePrint() || item->isPendingProduct()) {
+                            volatile int padS = 0; (void) padS;
@@ credit-row block @@
-                    volatile int pad0 = count;
+                    // Sink: keeps the original 4-byte size/regalloc delta.
                     volatile int pad0b = count;
```

Measured in this tree (same compiler, same link):

| source | `render` raw | pct | insns | `.ARM.extab` | render record |
|---|---:|---:|---:|---:|---:|
| `34e76bbe^` (`pad0`+`pad0b`) | 0.806 | **80.6** | 3626/3638 | 0x26290 | 940 B |
| `34e76bbe` (`volatile padS`+`pad0b`) | 0.788 | 78.8 | 3626/3639 | 0x26298 | 948 B |

`byte_exact`/`linked_exact` are 4499 in both, because `render` is not byte-exact either way;
the only gate-visible difference is `.ARM.extab` size. This pass restores the 80.6 body.

## 2. iOS 1.1.19 dump: what it does and does not say

`source file list dump/gof2hd_1.1.19_ios.cpp` is the Xcode `nm`-style symbol list per object
file. The `HangarWindow.cpp` block (lines 953-989) is:

```
HangarWindow.cpp
.../Objects-normal/arm64/HangarWindow-d08abb5ab94dc558a78da371accea6be.o
_HangarWindow::HangarWindow()
Classes/Game/HangarWindow.cpp
_HangarWindow::HangarWindow()
_HangarWindow::~HangarWindow()
_void ArrayReleaseClasses<TouchButton*>(Array<TouchButton*>&)
_HangarWindow::~HangarWindow()
_HangarWindow::initialize()
_void ArraySetLength<TouchButton*>(unsigned int, Array<TouchButton*>&)
_HangarWindow::refreshCurrentContentHeight()
_HangarWindow::update(int)
_HangarWindow::transaction(bool)
_HangarWindow::refreshCargoAvailabilityForBlueprints()
_HangarWindow::render3D()
_HangarWindow::render()
_HangarWindow::getRelativeScrollHeight()
_HangarWindow::OnTouchBegin(int, int)
...
_HangarWindow::hideMessage()

_FREE_CREDIT_AMOUNTS
_HangarWindow::lastTab
```

It proves:

- `render` belongs to `Classes/Game/HangarWindow.cpp` (our `src/game/ui/HangarWindow.cpp`),
  and its within-TU order is `render3D`, `render`, `getRelativeScrollHeight`, ...
  (our source order matches).
- `FREE_CREDIT_AMOUNTS` and `HangarWindow::lastTab` are the only namespace-scope statics of
  the TU (our source has both).
- `getRelativeScrollStartPos` is **not** in the 1.1.19 object's symbol list (it is in the
  Android 2.0.16 `.dynsym`, 48 B at `0x14b6bc`); it was either inlined or introduced after
  1.1.19. This does not affect `render`'s statement order.

It does **not** contain line numbers, local names, or statement order. The statement order
below is therefore taken from the Android Ghidra decompile and the binary, not the dump.

## 3. Ghidra statement order (Android 2.0.16)

`decompile_function` at Ghidra `0x1590d8` gives the fully-lazy shape:

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
    Layout::drawBG((Layout *)*piVar43);   /* direct global deref, no local */
    ...
```

Key statement orders recovered:

- Free-credits loop (Ghidra lines 915-925): `getWidth` -> `layout->field_0x2c` ->
  `getHeight` -> `SetColor` -> `textY = y + height*0.2f` -> `textX = width + baseX +
  rowHeight` -> `DrawString`. Our source is height-first; implementing Ghidra's width-first
  order on the 80.6 body measures **62.0 % / 9744 B** (variant F1), so the current
  height-first shape is the better local optimum even though it differs from Ghidra.
- Selected-item amount block (Ghidra lines 465-475): a plain `if (tab == 1)` with
  `right = getStationAmount(); left = getAmount();` else
  `right = getAmount(); left = bluePrint->getCurrentAmount(index);`, then
  `if (0 < left) { buttons[8] ... }`, `if (0 < right) { buttons[9] ... }`,
  `buttonOffset = buttons[9]->getWidth() + layout->field_0x2c`. Our source matches this
  order (the `if (right)` truthiness form is score-neutral).
- Second amount block (Ghidra lines 557-570): `left = tab == 1 ? getStationAmount() :
  getAmount()`, `right = tab == 1 ? getAmount() : bluePrint->getCurrentAmount(index) +
  getBlueprintAmount()`. Our source matches.
- `if (item->isSelectable())` draws: Ghidra's `else` arm is
  `if (tab != 0 || inTabIndex < 0) drawBox(3) else drawBox(9)`; our source uses the
  equivalent `else if (tab == 0 && inTabIndex >= 0) drawBox(9) else drawBox(3)`.
  Reordering to Ghidra's form measures **79.2 % / 9716 B** (variant G1), so the current
  form is better for alignment.

The body is otherwise statement-faithful; the remaining deltas are register allocation,
16/32-bit encoding width, scheduling and the EH-scope anchors.

## 4. The entry-block tie

Original (`objdump -d --start-address=0x1490d8 --stop-address=0x149142`):

```
1490d8: push {r4,r5,r6,r7,lr}; add r7,sp,#12; stmdb sp!,{r7,r8,r9,sl,fp}
1490e0: vpush {d8-d10}
1490e4: sub  sp, #376
1490e6: mov  fp, r0                     ; this -> fp
1490e8: ldr  r0,[pc,#808]  ; pool 149414 ; stack-chk guard GOT
1490ea: ldr  r1,[pc,#812]  ; pool 149418 ; Canvas GOT
1490ec: add  r0, pc
1490ee: add  r1, pc
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
14910a: add  r0, pc
14910c: ldr.w r8,[r0]                   ; r8 = &appManager
149110: ldr.w r0,[r8]
149114: blx  GetApplicationModule
149118: cmp  r0,#0
14911a: str.w fp,[sp,#136]              ; spill this
14911e: itt ne; ldrbne r0,[r0,#24]; cmpne r0,#0
149124: bne.n 1491cc
149126: mov  r6, fp
149128: ldr.w r0,[r6,#88]!              ; this->viewMode
14912c: cmp  r0,#0
14912e: bne.w 14a83c
149132: ldr  r0,[pc,#748]  ; pool 149420 ; &Globals::layout (lazy)
149134: add  r0, pc
149136: ldr.w sl,[r0]                   ; &layout -> sl
14913a: ldr.w r0,[sl]                   ; layout value
14913e: blx  drawBG
```

Current 80.6 baseline:

```
1490e6: mov  fp, r0                     ; this -> fp   (same)
1490e8: ldr  r0,[pc,#816]  ; stack-chk guard GOT
1490ea: ldr  r1,[pc,#820]  ; &Globals::layout GOT   <-- hoisted
1490ec: ldr  r2,[pc,#820]  ; Canvas GOT
1490ee: add  r0, pc
1490f0: add  r1, pc
1490f2: add  r2, pc
1490f4: ldr  r0,[r0,#0]
1490f6: ldr.w sl,[r1]                   ; &layout -> sl   <-- 0x1490f6, not 0x149136
1490fa: ldr  r2,[r2,#0]
1490fc: str  r2,[sp,#140]
1490fe: str  r0,[sp,#112]
149100: ldr  r1,[r0,#0]
149102: ldr  r0,[r2,#0]
149104: ldr.w r4,[sl]                   ; layout value cached in r4
149108: str  r1,[sp,#372]
14910a: mov.w r1,#-1
14910e: blx  SetColor
...
14913e: mov  r0, r4                     ; drawBG uses the cached value
149140: blx  drawBG
```

The baseline is structurally right (`this -> fp`, `&layout -> sl`, frame 376) but the
top-level `Layout *layout = Globals::layout;` makes clang hoist the GOT load into the entry
block and cache the value in `r4`. The original has no such local: the `Globals::layout` GOT
address is CSE'd into `sl` at the first use (`drawBG`) and the value is reloaded
(`ldr.w rX,[sl]`) at every later use.

Removing the top local moves the load to `0x149136` but flips the roles to `this -> sl`,
`&layout -> fp`, frame 384 (**61.6 %**). Caching three `Globals::gameText` uses restores
`this -> fp` and frame 376 at `0x149136`, but `&layout` lands in `r9` (**67.5 %**).

## 5. New exact-role lazy shape: appManager value cache

The only plain-C++ shape found that reproduces the original's *exact* entry-block roles
(`this -> fp`, `&layout -> sl`, frame 376, first layout GOT load at `0x149132`) is:

```cpp
Globals::Canvas->SetColor(0xffffffffu);
AbyssEngine::ApplicationManager *appManager = Globals::appManager;
ModStation *station = (ModStation *) appManager->GetApplicationModule(5);
...
// later:
HangarAppData *data = (HangarAppData *) appManager->GetApplicationData();
```

with the top `Layout *layout` local removed and `Globals::layout->drawBG()` used directly.
Measured: **9688 B, 73.5 %, insns 3626/3635, frame 376, this=fp, &layout=sl, load 0x149132**.

Its prologue is byte-identical to the original through `SetColor` (instructions 5-18), then
diverges at the appManager load because the cached value is materialised in `r8`:

```
1490e6: mov  fp, r0
1490e8: ldr  r0,[pc,#816]  ; stack-chk guard GOT
1490ea: ldr  r1,[pc,#820]  ; Canvas GOT
1490ec: add  r0, pc
1490ee: add  r1, pc
1490f0: ldr  r0,[r0,#0]
1490f2: ldr  r2,[r1,#0]
1490f4: str  r2,[sp,#140]
1490f6: str  r0,[sp,#112]
1490f8: ldr  r1,[r0,#0]
1490fa: ldr  r0,[r2,#0]
1490fc: str  r1,[sp,#372]
1490fe: mov.w r1,#-1
149102: blx  SetColor
149106: ldr  r0,[pc,#796]  ; &Globals::appManager
149108: movs r1,#5
14910a: add  r0, pc
14910c: ldr  r0,[r0,#0]    ; GOT entry (address of Globals::appManager)
14910e: ldr.w r8,[r0]      ; appManager value
149112: mov  r0, r8
149114: blx  GetApplicationModule
```

The body is 12 B short (9688 vs 9700) and the cached appManager value removes the
original's repeated `ldr.w rX,[r8]` reloads, so the score is below the 80.6 baseline. It is
the best evidence that the tie is solvable in principle, but not with a shape that also
matches the body.

Other lazy shapes measured this pass:

| shape | size | pct | frame | this | &layout | load |
|---|---:|---:|---:|---|---|---:|
| baseline (eager layout local) | 9700 | **80.6** | 376 | fp | sl | 0x1490ea |
| lazy, no top local | 9724 | 61.6 | 384 | sl | fp | 0x149132 |
| faithful lazy (no anchors) | 9632 | 52.8 | 384 | fp | r8 | 0x149134 |
| appManager value cache (AM3) | 9688 | 73.5 | 376 | fp | sl | 0x149132 |
| appManager value cache + eager layout | 9668 | 73.5 | 376 | fp | — | — |
| `Layout **layoutPtr = &Globals::layout` | 9724 | 61.6 | 384 | sl | fp | 0x149132 |
| `Layout *&layoutRef = Globals::layout` | 9724 | 61.6 | 384 | sl | fp | 0x149132 |
| local assigned after `GetApplicationModule` | 9700 | 80.5 | 376 | fp | — | 0x149124 |
| local assigned inside `if (viewMode==0)` | 9724 | 61.6 | 384 | sl | fp | 0x149132 |

The faithful lazy body (all anchors removed, `float textYf`, height-first) is particularly
informative: it keeps `this -> fp` but swaps the two globals (`&appManager -> sl`,
`&layout -> r8`), i.e. the allocator's tie-break is between the two long-lived GOT pointers,
not between `this` and `&layout`.

## 6. Anchor sweep on the 80.6 body

Removing any existing anchor lowers the score. Measured (single and pair removals; the full
3-combination sweep is in `/dev/shm/w4270/sweep_anchor.py`):

| change | pct |
|---|---:|
| none (baseline) | **80.6** |
| remove `hintLayout` | 77.1 |
| remove `pad3` | 79.1 |
| remove `pad0` | 79.3 |
| remove `pad0b` | 79.3 |
| remove 3 `description = description` | 78.0 |
| non-volatile `textYf` | 61.6 |
| width-first free-credits order | 62.0 |
| remove `hintLayout` + `pad3` | 77.3 |
| remove `pad0` + `pad0b` | 79.4 |
| remove `pad3` + `pad0` | 79.0 |

So the 80.6 body is a robust local optimum for the current source shape.

## 7. `.ARM.extab`: the 8-byte delta

The 80.6 build has four differing LSDA records (parsed from `readelf --unwind` and keyed by
function address):

```
0x971b8  Generator::createMission        orig 172  ours 176   +4
0xc0214  Level::createShip               orig 336  ours 328   -8
0x137d18 LevelScript::process            orig 348  ours 352   +4
0x1490d8 HangarWindow::render            orig 948  ours 940   -8
                                                              ----
                                                                -8
```

`render`'s missing row is the amount block at code offset 1406 (`0x1490d8 + 1406 =
0x149656`):

```
orig  (0x149656, 4, lp=0x14b5a2, 0)   (0x149660, 4, lp=0x14b64c, 0)
ours  (0x149656, 18, lp=0x14b64a, 0)
```

Both original landing pads are 2-byte thunks to `0x14b698` (destroy the `name` String at
`sp+0x138`); ours shares `0x14b64a` -> `0x14b68e` (destroy `sp+0x12c` then `sp+0x138`).
The split is a clang EH-scope artifact of the original source: the original's amount calls
only need to destroy `name`, while our merged region also destroys a String temporary at
`sp+0x12c`. The `34e76bbe` `padS` sink adds the missing row (record 948 B / 156 rows,
`.ARM.extab 0x26298`) but costs 1.5 points because the new landing-pad thunk shifts every
later landing-pad branch target. The revert keeps the higher function score and accepts
`-8`.

## 8. Commands used

```sh
# score one function (autojunk=False, official verifier internals)
python3 /dev/shm/w4270/score.py            # 80.6, raw 0.8059999999999999

# prologue roles (GOT-slot resolved)
python3 /dev/shm/w4270/roles2.py           # size 9700 frame 376 this fp &layout sl load 0x1490ea
python3 /dev/shm/w4270/roles2.py <so>      # original: load 0x149132

# full opcode diff
python3 /dev/shm/w4270/udiff.py > /dev/shm/w4270/udiff80.txt

# extab per-function records
python3 /dev/shm/w4270/extab_fn.py

# gate
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
```

## 9. What remains

- `render` is still 19.4 % off. The main unresolved axis is the `&layout` register once the
  load is lazy: the original keeps `&layout -> sl` and `&appManager -> r8`, while every
  shape that defers the layout load either swaps those two or caches the appManager value.
  A source shape that keeps the original's `ldr.w rX,[r8]` appManager reloads *and* pins
  `this -> fp`/`&layout -> sl`/frame 376 would likely jump well past 80.6.
- The `.ARM.extab` row split needs a source shape that gives the two amount calls separate
  landing pads without shifting the landing-pad code (or a compensating +8 record
  elsewhere). The `padS` sink is the only working row-adder found, and it costs 1.5 points.
