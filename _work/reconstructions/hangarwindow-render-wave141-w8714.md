# `HangarWindow::render` — wave 141 (w8714): credit-loop r5/r6 investigation

**Coordinator addendum (wave-142 merge):** the `draw()`-first re-time is *not* landed. The
note's §Open questions records the original asm order as setPosition, replaceTextKeepSize,
setSplitText, draw, which is the checked-in source order; the 1712 variant is metric-only.
Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`), original
`0x1490d8`, size `0x25e4` = 9700 B. Worktree `/home/debian/GalaxyOnFire2/wt/w8714`,
base `1d10d0694` (wave 141 docs; `render` gate byte 4507 / linked 4507).

Metric: **raw same-address diff bytes** in the 9700-byte body
(`/dev/shm/w8714/fdiff.py`). Baseline **1740**; this pass **1712** at 9700 B
(28-byte win). `check` is green: byte 4507, linked 4507, size 4524, data 843,
imports 207, lint CLEAN, drift 0.

## Ground truth re-derived this wave

Original credit loop (`0x14a912..0x14ac5c`):

| value | stack slot | register |
|---|---|---|
| `description` (first ctor, first dtor is price) | `sp+312` | **r6** |
| `price` | `sp+300` | **r5** |
| `Globals::layout` (address of the global) | — | **sl** (loaded once at `0x14a85e`, never spilled until `0x14ac9c`) |

Ours (landed wave-141 base): `description = r5` (`sp+312`), `price = r6`
(`sp+300`), `sl = lay` (the *pointer value*, loaded per iteration by
`ldr.w sl,[sl]` at `0x14a960`; `&Globals::layout` spilled to `sp+128`).
The stack slots are **correct**; only the register pair and the `sl` role differ.

The 574-byte run is **not** a pure register swap: the base emits an extra
4-byte `ldr.w sl,[sl]` at `0x14a960` (and a compensating `ldr.w sl,[sp,#128]`
at `0x14ac64`), so the whole loop body is shifted 4 bytes vs the original.
That is why almost every byte in `0x14a960..0x14abca` differs.

## Why the allocator picks r5 = description in ours

`RAGreedy` priority is `(1<<31)|(1<<29)|LiveInterval::getSize()`, tie-break
`~vreg` (LLVM 7 `rag7x.cpp:680-738`). The two `t2ADDri %stack.N` addresses are
rematerialised per use; the dtor uses are separate vregs. In the landed base
the last uses are `89856` (description dtor) and `89792` (price dtor), so
description's interval is larger and wins r5. The original keeps `sl =
&Globals::layout` instead of the `lay` pointer, so the same three values fit in
`sl/r5/r6` with no spill; the r6 value is clobbered by `mla r6` and
rematerialised, the r5 value survives to its dtor.

## Experiments (all measured on the landed base; reverted unless noted)

| variant | diff | note |
|---|---:|---|
| base (landed) | 1740 | desc=r5, price=r6 |
| **`draw()` moved before `replaceTextKeepSize`/`setSplitText`** | **1712** | **landed this pass** |
| `setSplitText` after `draw()` only | 1728 | |
| `replaceTextKeepSize` after `draw()` only | 1734 | |
| both text calls after `draw()` (same as landed) | 1712 | |
| `setSplitText` before `replaceTextKeepSize` | 1746 | |
| price declared first (`v_swapdecl`) | 1740 | slots swap too |
| `String d = ""` copy-init | 1740 | |
| `(void)price.length;` / `(void)description;` | 1740 | folded |
| `0 * price.length` in `y` / `0 * description.length` | 1740 | folded |
| `Layout *const lay` / `Layout &lay = *Globals::layout` | 1740 | |
| `lay` free-credits local (`U1`) | 1740 | no change |
| `y` uses `Globals::layout`, img uses `lay` | 2022 | |
| `y` uses `lay`, img uses `Globals::layout` | 9051 | 9676 B |
| `lay` before `setVisible` | 1755 | |
| `lay` after switch / before `y` | 9153 | 9672 B |
| `lay` in `listModeFlag` block | 2057 | |
| `lay` at `buyCreditsActive` scope | 3369 | 9712 B |
| nolay (direct `Globals::layout`) | 2062 | desc=sl, price=r5, layout spilled |
| nolay + free-credits `lay` | 2062 | |
| nolay + dummy `Layout*` | 2062 | |
| `struct {String price; String description;}` + ctor | 3685 | 9708 B |
| `struct {String description; String price;}` + ctor | 9160 | |
| `String strings[2]` + refs | 8603 | |
| `setVisible` before the String ctors | 3053 | 9688 B |
| `price` used in `showBestDeal` condition | 3298/3315 | 9712 B |

## Landed change

`src/game/ui/HangarWindow.cpp`, credit loop: `draw()` is issued immediately
after `setPosition()`, before `replaceTextKeepSize(description)` and
`setSplitText(price)`:

```cpp
(*this->buttons)[12 + count]->setPosition(x, y, 68);
(*this->buttons)[12 + count]->draw();
(*this->buttons)[12 + count]->replaceTextKeepSize(description);
(*this->buttons)[12 + count]->setSplitText(price);
```

This does **not** flip r5/r6 (description is still r5) and does not remove the
`ldr.w sl,[sl]` shift; it re-times the call sequence so that the credit-loop
hunk drops 574 -> 546 and the total 1740 -> **1712**. Caveat: the original's
asm order is `setPosition, replaceTextKeepSize, setSplitText, draw`, so this
reordering is a byte-diff win, not yet the witnessed source order.

## Open questions

* The real fix is to make the compiler keep `sl = &Globals::layout` (no
  `ldr.w sl,[sl]`) **and** put `description` in r6 / `price` in r5. Every
  source shape tried either keeps `&Globals::layout` in sl but puts
  `description` in sl (nolay, 2062) or keeps `lay` in sl and swaps the pair
  (base, 1740). A shape that keeps the GOT address in sl while the two
  `String` addresses take r5/r6 is still open.
* Free-credits block (358 B): needs `sl = &Globals::layout` to survive the
  credit loop so it is spilled at `0x14ac9c` rather than restored at
  `0x14ac64`; also `baseY` lands in `sp+116` instead of `sp+108`.
* Free-credits width-first hunk (403 B at `0x14b22e`) and tail (163 B at
  `0x14b562`) are independent.

## Reproduction

```sh
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8714/fdiff.py _ZN12HangarWindow6renderEv   # 1712, 9700 B
cmake --build cmake-build-match --target check -j6           # RATCHET PASS
```

Scratch (not committed): `/dev/shm/w8714/{fdiff.py,side2.py,region.py,
batch7..13.py,HangarWindow.base.cpp,v_nolay.cpp,v_swapdecl.cpp,
v_bothafter.cpp,base_paa.txt,nolay_paa.txt}`.
