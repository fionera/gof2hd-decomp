# `HangarWindow::render` — pass 9 (w4411): lazy-GOT / 376-byte-frame tie re-audit

Symbol `_ZN12HangarWindow6renderEv`, original `0x1490d8`, size `0x25e4` (9700 B).
Branch `wave94-w4411`, base master `f262bd4b`. Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so` (Ghidra image base
0x10000; ELF addresses below are the real ones).

This pass re-audits the known **lazy `&Globals::layout` GOT load at `0x149136` vs the
376-byte frame tie** left open by `hangarwindow-render-w4270.md`, with the `padS` EH sink
already reverted (`.ARM.extab` back to `0x26290`, `-8`). No source change was found that
beats the 80.6 % baseline; this note records the measurements, the exact-role shape, and
the reason the tie is still open. **No code change is landed** (gate unchanged).

## TL;DR

- The original `render` is the **fully lazy** shape: `Globals::layout->...` everywhere,
  no `Layout *layout` local. Its GOT address is CSE'd into `sl` at the first use
  (`0x149132`/`ldr.w sl` at `0x149136`), the value is reloaded `ldr.w rX,[sl]` 38 times,
  and the frame is **376**.
- Our 80.6 % baseline is the **eager** shape (`Layout *layout = Globals::layout;` at the
  top). It reproduces the original's entry-block register roles exactly
  (`this -> fp`, `&appManager -> r8`, `&layout -> sl`, frame 376) but hoists the layout
  GOT load to `0x1490f6` and caches the value in `r4` for `drawBG`.
- Removing the local gives the lazy load at `0x149136`, but the allocator flips the two
  long-lived pointers (`this -> sl`, `&layout -> fp`), the frame grows to **384**, and the
  score collapses to **61.6 %**.
- The only plain-C++ shape found that gives the original's *exact* entry-block roles with
  the lazy load and a 376-byte frame is the **appManager value cache** (`AM3`): it scores
  **73.5 % / 9688 B** because caching the appManager value changes the body codegen
  (removes the original's repeated `ldr.w rX,[r8]` reloads).
- A full anchor sweep and a 32-combination sweep confirm the 80.6 % body is a robust local
  optimum. Every single-anchor removal, every lazy shape, and every global-cache variant
  measured this pass is strictly worse.

## Gate (before == after)

```
verify: byte 4499  linked 4499  stubs 0  missing 0  extra 0  wrong_type 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS; lint CLEAN; drift 0
sections: .text 0x1337c8, .rodata 0x41b67, .ARM.exidx 0x6f70, .ARM.extab 0x26290 (-8)
render: 9700/9700 B, raw 0.8064427313, insns 3626/3638, frame 376
```

Build (canonical):

```sh
cmake -S . -B cmake-build-match \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
```

## 1. Entry blocks, side by side

Original (`objdump -d --start-address=0x1490d8 --stop-address=0x149142`):

```
1490d8: push {r4,r5,r6,r7,lr}; add r7,sp,#12; stmdb sp!,{r7,r8,r9,sl,fp}
1490e0: vpush {d8-d10}
1490e4: sub  sp, #376
1490e6: mov  fp, r0                     ; this -> fp
1490e8: ldr  r0,[pc,#808]  ; guard GOT
1490ea: ldr  r1,[pc,#812]  ; Canvas GOT
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
149106: ldr  r0,[pc,#788]  ; &appManager
149108: movs r1,#5
14910a: add  r0, pc
14910c: ldr.w r8,[r0]                   ; r8 = &appManager
149110: ldr.w r0,[r8]                   ; reload appManager value
149114: blx  GetApplicationModule
149118: cmp  r0,#0
14911a: str.w fp,[sp,#136]              ; spill this
14911e: itt ne; ldrbne r0,[r0,#24]; cmpne r0,#0
149124: bne.n 1491cc
149126: mov  r6, fp
149128: ldr.w r0,[r6,#88]!              ; this->viewMode
14912c: cmp  r0,#0
14912e: bne.w 14a83c
149132: ldr  r0,[pc,#748]  ; &Globals::layout (lazy)
149134: add  r0, pc
149136: ldr.w sl,[r0]                   ; &layout -> sl
14913a: ldr.w r0,[sl]                   ; layout value
14913e: blx  drawBG
```

80.6 % baseline (eager local), same address range:

```
1490e4: sub  sp, #376
1490e6: mov  fp, r0                     ; this -> fp          (same)
1490e8: ldr  r0,[pc,#816]  ; guard GOT
1490ea: ldr  r1,[pc,#820]  ; &Globals::layout GOT   <-- hoisted
1490ec: ldr  r2,[pc,#820]  ; Canvas GOT
1490ee: add  r0, pc
1490f0: add  r1, pc
1490f2: add  r2, pc
1490f4: ldr  r0,[r0,#0]
1490f6: ldr.w sl,[r1]                   ; &layout -> sl        (same reg, eager)
1490fa: ldr  r2,[r2,#0]
1490fc: str  r2,[sp,#140]
1490fe: str  r0,[sp,#112]
149100: ldr  r1,[r0,#0]
149102: ldr  r0,[r2,#0]
149104: ldr.w r4,[sl]                   ; layout value -> r4 (cached)
149108: str  r1,[sp,#372]
14910a: mov.w r1,#-1
14910e: blx  SetColor
149112: ldr  r0,[pc,#788]  ; &appManager
149114: movs r1,#5
149116: add  r0, pc
149118: ldr.w r8,[r0]                   ; r8 = &appManager   (same)
14911c: ldr.w r0,[r8]                   ; reload value       (same)
149120: blx  GetApplicationModule
...
14913e: mov  r0, r4                     ; drawBG uses cached value
149140: blx  drawBG
```

Lazy shape (`Layout *layout` removed, `Globals::layout->drawBG()`), same range:

```
1490e4: sub  sp, #384                   ; <-- 8 bytes larger
1490e6: mov  sl, r0                     ; this -> sl   <-- flipped
1490e8: ldr  r0,[pc,#816]  ; guard GOT
1490ea: ldr  r1,[pc,#820]  ; Canvas GOT
1490ec: add  r0, pc
1490ee: add  r1, pc
1490f0: ldr  r0,[r0,#0]
1490f2: ldr  r2,[r1,#0]
1490f4: str  r2,[sp,#148]
1490f6: str  r0,[sp,#120]
1490f8: ldr  r1,[r0,#0]
1490fa: ldr  r0,[r2,#0]
1490fc: str  r1,[sp,#380]
1490fe: mov.w r1,#-1
149102: blx  SetColor
149106: ldr  r0,[pc,#796]  ; &appManager
149108: movs r1,#5
14910a: add  r0, pc
14910c: ldr.w r8,[r0]                   ; r8 = &appManager   (same)
149110: ldr.w r0,[r8]                   ; reload value       (same)
149114: blx  GetApplicationModule
...
149132: ldr  r0,[pc,#756]  ; &Globals::layout (lazy, same position)
149134: add  r0, pc
149136: ldr.w fp,[r0]                   ; &layout -> fp  <-- flipped
14913a: ldr.w r0,[fp]
14913e: blx  drawBG
```

So the lazy shape reproduces the original's *load position* and *appManager* codegen
exactly, and differs only in `mov fp,r0` vs `mov sl,r0` and the resulting `this`/`&layout`
swap (plus the 8-byte frame shift).

## 2. The exact-role lazy shape: appManager value cache (`AM3`)

The only plain-C++ shape found that keeps `this -> fp`, `&layout -> sl`, frame **376**,
and the lazy load at `0x149132` is:

```cpp
Globals::Canvas->SetColor(0xffffffffu);
AbyssEngine::ApplicationManager *appManager = Globals::appManager;
ModStation *station = (ModStation *) appManager->GetApplicationModule(5);
...
// later:
HangarAppData *data = (HangarAppData *) appManager->GetApplicationData();
```

with the top `Layout *layout` local removed and `Globals::layout->drawBG()` used directly.
Its prologue is **byte-identical to the original from `0x1490d8` through `0x149102`** and
again from `0x149126` through `0x14913e`; only the appManager load differs:

```
AM3 (0x149106):
149106: ldr  r0,[pc,#796]  ; &appManager
149108: movs r1,#5
14910a: add  r0, pc
14910c: ldr  r0,[r0,#0]    ; GOT entry (address of Globals::appManager)
14910e: ldr.w r8,[r0]      ; appManager value -> r8
149112: mov  r0, r8
149114: blx  GetApplicationModule
```

vs the original's `ldr.w r8,[r0]` (`&appManager -> r8`) + `ldr.w r0,[r8]` (reload value).
AM3 is **9688 B / 73.5 % / 3626:3635 insns**, i.e. 12 B short, because caching the
appManager value removes the original's repeated `ldr.w rX,[r8]` reloads and shifts the
body's register allocation (the diff shows `r5`↔`r9` and stack-slot shifts from the
second item-loop block onward).

## 3. Variant table (all measured in this tree, official verifier, `autojunk=False`)

| shape | raw | size | insns | frame | this | &layout | load |
|---|---:|---:|---:|---:|---|---|---:|
| **baseline (eager local)** | **0.8064427313** | 9700 | 3626/3638 | 376 | fp | sl | 0x1490f6 |
| local after `SetColor` | 0.8058920705 | 9700 | 3626/3638 | 376 | fp | sl | 0x1490f6 |
| local after `GetApplicationModule` (D) | 0.8050660793 | 9700 | 3626/3638 | 376 | fp | sl | 0x149124 |
| `Layout *const` / `Layout *volatile` local | 0.8064427313 | 9700 | 3626/3638 | 376 | fp | sl | 0x1490f6 |
| lazy, no top local | 0.6161241588 | 9724 | 3626/3655 | 384 | sl | fp | 0x149132 |
| lazy local inside `if (station...)` (P3) | 0.6161241588 | 9724 | 3626/3655 | 384 | sl | fp | 0x149132 |
| faithful lazy (no anchors, plain float) | 0.5277816076 | 9632 | 3626/3627 | 384 | fp | r8 | 0x149134 |
| faithful + `volatile textYf` | 0.6136050675 | 9672 | 3626/3636 | 384 | sl | fp | 0x149132 |
| **appManager value cache (AM3)** | 0.7348850021 | 9688 | 3626/3635 | 376 | fp | sl | 0x149132 |
| appManager value cache + eager layout | 0.7368421053 | 9668 | 3626/3632 | 376 | fp | — | — |
| `Layout **layoutPtr = &Globals::layout` | 0.6161241588 | 9724 | 3626/3655 | 384 | sl | fp | 0x149132 |
| `Layout *&layoutRef = Globals::layout` | 0.6161241588 | 9724 | 3626/3655 | 384 | sl | fp | 0x149132 |

## 4. Anchor sweep on the 80.6 % body

Every single-anchor removal is strictly worse (full 32-combination sweep in the w4270
note; re-confirmed here):

| change | raw |
|---|---:|
| none (baseline) | **0.8064427313** |
| remove `hintLayout` | 0.7706 |
| remove `pad3` | 0.7915 |
| remove `pad0`/`pad0b` | 0.7925 |
| remove 3 `description = description` | 0.6199 |
| plain `float textYf` (no volatile) | 0.6161 |
| width-first free-credits order | 0.6196 |
| `pad3` moved into `if (right)` | 0.7893 |
| `pad3` moved before `if (left > 0)` | 0.7862 |
| `pad3` moved after the buttonOffset block | 0.7907 |
| `pad3 = 1/2/5` instead of `0` | 0.8064427313 (neutral) |
| `pad0 = 0/1/2` instead of `count` | 0.8058 / 0.8063 / 0.8063 |
| `hintLayout` used for the rowHeight/field_0x48 too | 0.8064427313 (neutral) |
| first hint access via `Globals::layout` | 0.7586 |
| second hint access via `Globals::layout` | 0.7878 |

Global-value caches (lazy layout + cache one global `n` times) are all far below the
baseline: `gameText` 55.7–56.4 %, `status` 46.6–47.2 %, `imageFactory` 54.7–55.4 %,
`Canvas` 57.5–61.6 % for `n = 1..10`.

## 5. Stack-slot evidence (why 376 vs 384)

`[sp,#N]` and `add rX,sp,#N` uses in the whole function (`slots2.py`):

```
orig: 0:57 4:53 8:2 12:3 20:2 24:2 28:12 32:8 36:16 40:14 44:4 48:4 52:4 56:4
      60:6 64:4 72:6 76:8 80:14 84:4 88:48 92:12 96:12 100:8 104:24 108:46
      112:4 116:88 120:132 124:18 128:48 132:90 136:90 140:82 144:3 156:3
      168:3 180:4 192:2 204:5 216:12 228:4 240:3 252:4 264:4 276:12 288:22
      300:24 312:39 324:2 336:2 348:2 360:2 372:4 376:2
base: (same slot set, counts differ by <=4; extra 204:9 216:10 276:14 288:24
      300:27 312:37, i.e. the volatile anchors)
lazy: every slot shifted +8 (88 -> 96, 372 -> 380), plus a new 88:7 slot
      (buttonOffset) and frame 384
```

The base reproduces the original's **exact stack-slot set**; the lazy shape needs an
extra 8-byte slot at `sp+88` for `buttonOffset`, which pushes every other slot up by 8
and makes the frame 384. That is a consequence of the lazy shape's different register
allocation, not a source-level difference we can remove independently.

## 6. Ghidra statement order (Android 2.0.16)

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
```

`piVar43` is the CSE'd GOT address of `Globals::layout`; Ghidra shows **86 `*piVar43`
dereferences** and no `Layout*` local. The original therefore has no source-level
`Layout *layout` local.

Free-credits loop (Ghidra lines 915-925): `getWidth` -> `layout->field_0x2c` ->
`getHeight` -> `SetColor` -> `textY = y + height*0.2f` -> `textX = width + baseX +
rowHeight` -> `DrawString`. The original computes `textY` in registers
(`vcvt.s32.f32 s0,s2; vmov r6,s0` at `0x14ad70`/`0x14ad8c`), with **no `[sp,#204]`
round-trip**; our baseline's `volatile float textYf` adds `vstr s2,[sp,#204]; vldr
s18,[sp,#204]` at `0x14ad8c`/`0x14ad90`. The volatile is load-bearing for the 80.6 %
score (removing it -> 61.6 %), so it stays documented as an anchor.

## 7. iOS 1.1.19 dump

`source file list dump/gof2hd_1.1.19_ios.cpp` (lines 953-989) is a symbol/TU dump, not a
source dump. It confirms `HangarWindow::render` is the ninth function of
`Classes/Game/HangarWindow.cpp` (after `render3D`, before `getRelativeScrollHeight`),
and that `FREE_CREDIT_AMOUNTS` and `HangarWindow::lastTab` are the TU's only statics. It
contains no statement order.

## 8. `.ARM.extab`

The 80.6 % build keeps `.ARM.extab 0x26290` (`-8` vs original). `render`'s LSDA record is
940 B vs the original's 948 B; the missing row is the amount block at code offset 1406
(`0x149656`), whose two landing pads in the original (`lp=0x14b5a2`, `lp=0x14b64c`) are
merged into one in ours (`lp=0x14b64a`). The reverted `padS` sink was the only working
row-adder and cost 1.5 points, so the `-8` is accepted.

## 9. What remains

- The tie is a **RegAllocGreedy live-interval-size tie** between the two long-lived
  pointers `this` and `&Globals::layout`. In the eager shape both are defined in `bb.0`
  and `this` wins `fp` (the original's assignment). In the lazy shape `&layout` is defined
  in `bb.4` (`0x149132`), the allocator's priority order flips, and `this` takes `sl`.
  A source shape that defines the layout GOT vreg in `bb.0` *without* loading its value
  (so the value is still reloaded at each use) would match the original entry block and
  the 38 `[sl]` reloads. `Layout **layoutPtr = &Globals::layout` and
  `Layout *&layoutRef = Globals::layout` both fold back to the lazy shape.
- The lazy body needs an extra 8-byte `buttonOffset` slot (`sp+88`) that the original does
  not have; that slot is what makes the frame 384.
- The second layout GOT load at `0x14a85a` (before `drawFooter`) is a `sl` spill/reload;
  the base already reproduces it (38 `[sl]` loads, same as the original).
- `.ARM.extab` remains 8 bytes short (known w4270 `render` EH residual).

## 10. Reproduction

```sh
# score render (official verifier internals, autojunk=False)
python3 /dev/shm/w4411/raw.py            # 0.8064427313 size 9700 insns 3626/3638

# prologue roles (GOT-slot resolved)
python3 /dev/shm/w4411/roles.py          # frame=376 this=fp

# stack slots (both [sp,#N] and add rX,sp,#N)
python3 /dev/shm/w4411/slots2.py

# variants
python3 /dev/shm/w4411/pLazyBase.py      # lazy, no top local
python3 /dev/shm/w4411/pAM3.py           # appManager value cache (exact roles)
python3 /dev/shm/w4411/pD.py             # local after GetApplicationModule

# gate
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
```
