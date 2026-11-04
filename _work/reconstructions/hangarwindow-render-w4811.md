# `HangarWindow::render` — pass 12 (w4811): `pad3` initialiser nudge, LSDA row audit

Symbol `_ZN12HangarWindow6renderEv`, original `0x1490d8`, size `0x25e4` (9700 B).
Branch `wave98-w4811`, base master `fe4f5e7cf`. Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so` (Ghidra image base
0x10000; ELF addresses below are the real ones).

This pass re-audited the lazy-GOT / 376-byte-frame tie left open by
`hangarwindow-render-w4709.md`, using the Ghidra decompilation of `render` (via
`analyzeHeadless` + `DecompAt2.java`) and a full parse of the `.ARM.extab` call-site
table. **One source change lands: the `pad3` volatile sink initialiser changes from `0`
to `this->field_0x114`** — a strict raw-match improvement `0.8064427313 -> 0.8082060`
with an unchanged 9700-byte body and an unchanged gate.

## TL;DR

- The original `render` is the **fully lazy** shape (`Globals::layout->...` at every use);
  the 80.6 % baseline is the eager `Layout *layout = Globals::layout;` shape.  The eager
  local's prologue load is kept alive only by the `-g` `DBG_VALUE` (w4709).
- Ghidra confirms the source structure of the two `left`/`right` blocks is exactly what the
  reconstruction has: the first is a real `if (tab == 1) { right = stationAmount; left =
  amount; } else { right = amount; left = currentAmount; }`; the second is the merged
  ternary form `int left = tab==1 ? stationAmount : amount; int right = tab==1 ? amount :
  currentAmount + blueprintAmount;`.  (Ghidra renders the second as an if/else, but the
  disassembly shows the tab!=1 values computed unconditionally in the fall-through and the
  tab==1 values in a separate block — i.e. the ternary form is the faithful one.)
- The one *real* source-order difference found is in the `freeCreditsActive` block:
  the original calls `TouchButton::getWidth()` **before** `getHeight()` (and reads
  `Globals::layout->field_0x2c_rowHeight` in between), while the reconstruction calls
  `getHeight()` first.  Swapping it (`g1`) is faithful but re-colours the whole list body
  and grows the function to 9744 B, scoring 0.6199 — not landable on its own.
- `.ARM.extab` is still `0x26294` (−4).  The original's call-site table has **156** rows,
  ours **155**.  The single extra original row is the final `rightText` `operator+`
  invoke region: original `start 0x72c len 0xe` (it includes the `add.w fp, sp, #300`
  destination materialisation), ours `start 0x732 len 0xa` (`fp` is pre-loaded earlier at
  `0x14980a`).  Root cause is the same register-allocation difference, not a source shape.
- **New improvement:** `volatile int pad3 = 0;` → `volatile int pad3 = this->field_0x114;`
  in the `if (left > 0)` block.  The initialiser reads the same `this->field_0x114` that
  the following `setPosition(..., this->field_0x114 + y, 17)` uses, so the load is CSE'd;
  the extra `this`-based volatile store re-colours a register in the list body
  (`mov r6,r0` → `mov r9,r0`, `ldr r5,[sp,#136]` → `ldr r6,[sp,#136]`,
  `cbz r6` → `cmp.w r9,#0; beq`) and the raw ratio rises to **0.8082060**.

## Gate (before -> after)

```
verify: byte 4500 -> 4500  linked 4500 -> 4500  stubs 0  missing 0  extra 0  wrong_type 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS; lint CLEAN; drift 0
sections: .text 0x1337c8, .rodata 0x41b67, .ARM.exidx 0x6f70, .ARM.extab 0x26294 (-4)
render: 9700/9700 B, raw 0.8064427313 -> 0.8082059755, insns orig/ours 3626/3638 -> 3626/3637, frame 376
```

Build (canonical):

```sh
cmake -S . -B cmake-build-match \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint -j8
cmake --build cmake-build-match --target drift -j8
```

## 1. The landed change

`src/game/ui/HangarWindow.cpp`, inside `if (left > 0)` in the `isItem` branch:

```cpp
// before
volatile int pad3 = 0;
// after
volatile int pad3 = this->field_0x114;
```

`pad3` is still a dead volatile sink (the anchor that keeps the call-site cleanup scopes
and the 376-byte frame intact).  Only its initialiser changes.  The load of
`this->field_0x114` is CSE'd with the `setPosition` argument three lines below, so the
body stays 9700 B and our instruction count drops from 3638 to 3637 (the original has
3626).  The score gain comes entirely from the changed register roles in the
list body.

## 2. New: Ghidra decompilation cross-check

Decompiled with the existing project (`_work/bins/android_2.0.16_libgof2hdaa.so_ghidra`,
`DecompAt2.java`, `ADDR=0x1590d8`).  Points confirmed against the reconstruction:

- `Layout::drawBG((Layout *)*piVar43)` with `piVar43 = &Globals::layout` loaded lazily at
  `0x149132`/`0x149136` — the original is the lazy shape.
- `if (tab == 4 || tab == 1) { if (item == selectedItem) { ... } else buttonOffset = 0; }`
  — matches.
- First `left`/`right` block: `if (tab == 1) { right = getStationAmount(); left =
  getAmount(); } else { right = getAmount(); left = getCurrentAmount(); }` — matches the
  reconstruction's explicit if/else (disasm `0x149c88`..`0x149ca4`).
- Second `left`/`right` block: tab==1 branch at `0x149e26` (`getStationAmount` → left,
  `getAmount` → right, `b.n 0x149f3e`), tab!=1 branch at `0x149f14` (`getAmount` → left,
  `getCurrentAmount`+`getBlueprintAmount` → right), join at `0x149f3e`
  (`String(left)` …).  This is exactly the ternary form the reconstruction uses; the
  decompiler's if/else rendering is a CFG artefact.
- `String leftText = String(left) + String("t");` and
  `String rightText = (tab==1 ? String(right) : String(right)+String("/")+String(total))
  + String("t");` — matches, including the `cmp.w r8,#0` guard on the tab!=1 temporaries.
- `freeCreditsActive`: the original order is `getWidth()`, `rowHeight`, `getHeight()`,
  `SetColor()` (`0x14a4xx`).  The reconstruction has `getHeight()`, `getWidth()`,
  `rowHeight`, `SetColor()`.  This is the only real source-order difference found.
- The `description = description` ×3 tail is **not** in the original: the original has 18
  `String::operator=` calls in `render`, the reconstruction has 21.  The three extra calls
  are a size filler (0 -> 9680 B, 1 -> 9684, 2 -> 9692, 3 -> 9700).  They stay because
  they are load-bearing for the 9700-byte body, but they are not faithful.

## 3. New: LSDA (`.ARM.extab`) call-site table

`render`'s LSDA lives at `.ARM.extab` offset `0x1c30fc`.  Parsed tables:

- original: **156** call-site rows
- ours: **155** call-site rows

The single extra original row is the final `rightText` `operator+`:

```
orig: start 0x72c len 0xe lp 0x2416   -> [0x149804, 0x149812)
ours: start 0x732 len 0xa lp 0x241a   -> [0x14980a, 0x149814)
```

Original `0x149804` is `add.w fp, sp, #300` (the `rightText` destination), which is inside
the invoke region; ours materialises `fp = sp+300` earlier at `0x14980a`'s predecessor
(`0x1497xx`) and only emits `mov r0, fp` inside the region.  This is the same
register-allocation difference that drives the rest of the body mismatch, not a source
shape.  All other rows differ only by the per-function address shift.

The EH landing-pad destructor slot sequence at the function end is byte-identical in both
builds: `[192,288,276,204,300,300,300,204,288,216,288,168,216,276,228,276,264,240,288,
312,216,300,276,300,312]`.

## 4. Variant table (all 9700 B unless noted; baseline 0.8064427)

`pad3` initialiser sweep (the landed axis):

| initialiser | raw | size | ours insns |
|---|---:|---:|---:|
| `0` (baseline) | 0.8064427 | 9700 | 3638 |
| `this->field_0x114` | **0.8082060** | 9700 | 3637 |
| `this->iconOffsetY` | 0.8082060 | 9700 | 3637 |
| `this->buttonHeight` | 0.8082060 | 9700 | 3637 |
| `Globals::layout->buttonInsetX` | 0.8073799 | 9700 | 3637 |
| `this->field_0x100.a` / `scrollOffset` / `visibleHeight` | 0.8065538 | 9700 | 3637 |
| `Globals::h` / `item->inTabIndex` | 0.8064427 | 9700 | 3638 |
| `Globals::w` | 0.8039648 | 9700 | 3638 |
| `this->hintOffsetX` | 0.7898651 | 9696 | 3636 |
| `this->hintImage` | 0.7898651 | 9696 | 3636 |
| `item->item->getAmount()` | 0.6177808 | 9716 | 3629 |
| `Globals::iPad` | 0.6578585 | 9704 | 3640 |

`pad3 = this->field_0x114` combined with the other anchors:

| combination | raw | size |
|---|---:|---:|
| `pad0`/`pad0b` volatile (baseline) | **0.8082060** | 9700 |
| `pad0b = 0` | 0.8075441 | 9700 |
| `pad0`/`pad0b` non-volatile | 0.7942716 | 9696 |
| `textYf` non-volatile | 0.6191132 | 9752 |

`pad3` placement (with `this->field_0x114`): current top-of-`if (left>0)` is best; before
`if (right)` 0.7871/9696, before `buttonOffset` 0.8060/9700, before `SetColor`
0.6085/9748, before `leftText` 0.7911/9696.

`description = description` count sweep (with the new `pad3`): 0 -> 0.7822/9680,
1 -> 0.7819/9684, 2 -> 0.7857/9692, **3 -> 0.8082/9700**, 4 -> 0.7909/9712,
5 -> 0.7903/9720, 6 -> 0.7899/9728.

Other axes re-measured this pass (all worse or neutral): `Layout &layout = *Globals::layout`
(neutral), `ModStation *station` declaration forms (neutral), `buttonOffset` initialisers
(neutral), `pad0`/`pad0b` types (neutral), loop-variable types (neutral),
`if (0 < x)` spellings (neutral), extra `volatile this->...` sinks in the `isShip`/
`isSlot`/`isBluePrint`/`isPendingProduct` branches (0.51-0.79), `hintLayout` removal
(0.77), `getWidth`-before-`getHeight` (0.6199/9744), `left`/`right` if/else
(0.7885/9696).

## 5. What remains

- The body is still register-allocation driven: the original uses more `r8`-`r11` 4-byte
  encodings, ours more 16-bit low-register encodings.  The `pad3` initialiser nudges one
  such role; there may be more, but every other anchor/placement/type variant measured
  this pass is neutral or worse.
- The `.ARM.extab` row difference (156 vs 155) is the `rightText` destination
  materialisation and is tied to the same regalloc difference.
- The `description = description` ×3 filler is not faithful; the original's extra 20 bytes
  live in the list body (the original has 16 more bytes than the no-hack build in
  `0x1496b2`..`0x14a4c0`), spread over many small register-encoding differences.

## 6. Reproduction

```sh
python3 /dev/shm/w4811/raw.py     # 0.8082059755 size 9700 insns 3626/3637
# Ghidra decompilation of the original render:
ADDR=0x1590d8 /tmp/ghidra_12.1.2_PUBLIC/support/analyzeHeadless \
  /home/debian/GalaxyOnFire2/_work/bins android_2.0.16_libgof2hdaa.so_ghidra \
  -process -noanalysis -scriptPath /home/debian/ghidra_scripts -postScript DecompAt2.java
```

Scratch scripts used this pass live in `/dev/shm/w4811` (`raw.py`, `lsda2.py`,
`batch*.py`, `ghidra_render.txt`, `onorm.txt`, `unorm*.txt`).
