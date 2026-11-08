# `HangarWindow::render` — wave 143 (w8813): free-credits declaration order

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`), original
`0x1490d8`, size `0x25e4` = 9700 B. Worktree `/home/debian/GalaxyOnFire2/wt/w8813`,
base `bf083ab41` (wave 142; `render` gate byte 4507 / linked 4507).

This pass: **1740 -> 1731 raw diff bytes, size 9700 B** (landed, faithful).
`check` is green: byte 4507, linked 4507, size 4524, data 843, imports 207,
lint CLEAN, drift 0.

Metric: **raw same-address diff bytes** in the 9700-byte body
(`/dev/shm/w8813/fdiff.py`). Baseline 1740; this pass 1731.

## Landed change (faithful to the original asm order)

The free-credits loop's four locals are reordered to match the original's
witnessed evaluation order (`getWidth`, `rowHeight`, `getHeight`, `textX`):

```cpp
int width = button->getWidth();
int rh = Globals::layout->field_0x2c_rowHeight;
int height = button->getHeight();
int textX = width + baseX + rh;
```

The old source was `rh; width; textX; height;`. The original free-credits
loop at `0x14ad26..0x14ad7e` is `getWidth` -> `rowHeight` -> `getHeight` ->
`textX`, so this is a source-order fix, not a scheduling trick. It is worth
9 raw bytes (the `getWidth`/`rowHeight` register roles shift and the
`0x14ad2c..0x14ad3f` hunk shrinks).

## Credit-loop r5/r6 investigation (the main mission; no flip found)

Ground truth re-confirmed this wave from the original disassembly
(`/dev/shm/w8813/orig_credit.txt`):

| value | stack slot | original reg | ours (base) |
|---|---|---|---|
| `description` | `sp+312` | **r6** | r5 |
| `price` | `sp+300` | **r5** | r6 |
| `&Globals::layout` | — | **sl** (loaded `0x14a85e`, spilled `0x14ac9c`) | `lay` value in sl; address spilled `0x14a8ec` |

The base's extra `ldr.w sl,[sl]` at `0x14a960` shifts the whole loop body by
4 bytes and is the source of the 255 + 81 + 89 + ... byte runs. The original
keeps `&Globals::layout` in `sl` through the credit loop and spills it only
in the free-credits block (`str.w sl,[sp,#128]` at `0x14ac9c`).

### Why the pair is sticky (new analysis this wave)

LLVM 7 `RAGreedy` priority is `(1<<31)|(1<<29)|Size` (global ranges), tie-break
`~vreg` (lower vreg first). From the pre-RA `Live Interval Analysis` dump
(`/dev/shm/w8813/lia.txt`): `%2208` (description, `stack.5`) is defined at
`77424B` and last used at `89856B` (its dtor); `%2210` (price, `stack.6`) is
defined at `77440B` and last used at `89792B` (its dtor). Description is
therefore **5 instructions (80 slots) longer**, so it wins r5. The dtor order
is fixed by C++ (price first, description second), so simply adding uses of
price cannot change the endpoint.

The original's r6 is clobbered by the y computation (`ldrd lr,r6,[r3,#288]` at
`0x14ab08`) and rematerialised at the dtor (`add r6,sp,#312` at `0x14ac58`);
its replaceText argument is a scratch (`add r1,sp,#312` at `0x14ab98`). In ours
description is r5, which is restored at replaceText (`add r5,sp,#312` at
`0x14ab8c`) and survives to the dtor (`mov r0,r5`). The pair flip is therefore
the root cause of both the register swap and the rematerialisation difference.

### Experiments this wave (all size 9700 unless noted; reverted)

| variant | diff | note |
|---|---:|---|
| base (wave-141 landed) | 1740 | desc=r5, price=r6, sl=lay |
| **free-credits decl order `width,rh,height,textX`** | **1731** | **landed (faithful)** |
| same + `draw()` moved before the text calls | 1703 | **METRIC-ONLY** (original asm order is setPosition, replaceText, setSplitText, draw) |
| same + `setSplitText` moved after `draw()` | 1719 | **METRIC-ONLY** |
| `String price(""); { String description(""); ... }` (scopeA) | 1740 | **pair flips** (price=r5, description=r6) but slots swap (price=sp+312, description=sp+300) and dtor order reverses |
| price declared first | 1740 | slots swap too |
| `const Layout *lay` | 1740 | no change |
| `lay` before `setVisible` | 1755 | |
| `lay` after the switch | 9153 (9672 B) | size regression |
| cache `int top = lay->field_0x20_top` | 9062 (9668 B) | size regression |
| cache `int rh = lay->field_0x2c_rowHeight` | 9160 (9672 B) | size regression |
| `(void)lay;` / `(void)lay->field_0x20_top;` after DrawImage2D | 1740 | folded |
| `int dummy = lay->field_0x20_top;` before showBestDeal | 1740 | folded |
| `Layout *const &lay = Globals::layout` | 2062 | = nolay |
| `Layout *volatile lay` | 2039 | |
| `struct {String price; String description;}` + direct members | 3870 | struct frame object lands at sp+328; default ctors |
| struct with `Strings() : price(""), description("") {}` | 3685 (9708 B) | size regression |
| `String &descRef = description` for replaceText | 1815 | |
| `String *descPtr = &description` for replaceText | 1815 | |
| `String &priceRef = price` for setSplitText | 3212 (9696 B) | |
| `String description; String price;` (default ctors) | 3330 (9688 B) | loop-head ctor becomes `StringC1Ev` |
| `PaintCanvas *canvas` (drop volatile) | 2977 (9696 B) | size regression |
| `int textY` (drop volatile) | 3119 (9716 B) | size regression |
| `volatile int textX` | 9143 (9616 B) | size regression |
| `Layout &layout = *Globals::layout` in free loop | 3077 (9712 B) | size regression |
| `Layout *layout = Globals::layout` in free loop | 3077 (9712 B) | size regression |

## Open questions

* **r5/r6 flip.** The only shape found that flips the pair is scopeA
  (price outer, description inner), but it swaps the stack slots and reverses
  the dtor order, so the net raw diff is unchanged (1740). A shape that makes
  price's interval longer while keeping `description=sp+312` /
  `price=sp+300` is still open. The interval endpoints are fixed by the
  declaration/destruction order, so the lever must be a pre-RA rematerialisation
  of description at replaceText (which would shorten its interval below
  price's). `String &descRef` / `String *descPtr` did not do it.
* **`sl = &Globals::layout`.** The original keeps the GOT address in `sl`
  through the credit loop and spills it at `0x14ac9c`; ours spills it at
  `0x14a8ec`. Fixing this would also fix the free-credits block's width/rh
  register roles (`width=sl`, `rh=r9` in the original; ours is the reverse).
  Every source shape tried either keeps `&Globals::layout` in `sl` but puts
  description in `sl` (nolay/`Layout**`/`Layout*&`, 2062) or keeps the `lay`
  value in `sl` (base, 1740).
* Free-credits `textX` is still scheduled after `getText` in ours but before
  it in the original (`0x14ad78` vs `0x14ad98`); source reordering of the
  `textX` declaration does not move it.
* Free-credits width-first hunk (`0x14b22e`, 403 B) and tail (`0x14b562`,
  163 B) are independent.

## Reproduction

```sh
cp /dev/shm/w8813/HangarWindow.base.cpp src/game/ui/HangarWindow.cpp  # optional
python3 /dev/shm/w8813/p_fc_order.py
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8813/fdiff.py _ZN12HangarWindow6renderEv   # 1731, 9700 B
cmake --build cmake-build-match --target check -j6           # RATCHET PASS
```

Scratch (not committed): `/dev/shm/w8813/{fdiff.py,region.py,try.sh,cc.sh,
HangarWindow.base.cpp,orig_credit.txt,ours_credit.txt,lia.txt,greedy.txt,
p_fc_order.py,p_fc_drawfirst.py,p_scopeA.py,...}`.
