# `HangarWindow::render` — wave 141 (w8616): credit-loop `Layout *lay` local
# frees `sl`; raw 2059 -> 1740 at 9700 B

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`), original
`0x1490d8`, size `0x25e4` = 9700 B. Worktree `/home/debian/GalaxyOnFire2/wt/w8616`,
base `e18e0c2a4` (wave-140 docs; `render` gate byte 4507 / linked 4507).

This pass: **2059 -> 1740 raw diff bytes, size 9700 B** (landed). `check` is green
(byte 4507, linked 4507, size 4524, data 843, imports 207, lint CLEAN, drift 0).

Metric: **diffbytes** = differing bytes in the 9700-byte body (`/dev/shm/w8616/fdiff.py`).
Landed baseline 2059 (wave 140); base-order control 2062.

## Finding 1: wave-140's declaration swap also swapped the *stack slots*

Re-reading the disassembly: with `String description(""); String price("");`
(description first) the slots are **description = sp+312, price = sp+300** and
the registers are **description = sl, price = r5**. With price first the slots
swap too (price = sp+312, description = sp+300), so wave 140's 3-byte win was a
stack-slot swap, not a register win. The original is description-first:
description = sp+312 = **r6**, price = sp+300 = **r5**, and **sl = &Globals::layout**
(loaded once at `0x149136`, `ldr.w r1,[sl]` at each use). Ours spilled
`&Globals::layout` to `sp+128` (`strd sl,r9,[sp,#128]` at `0x14a8ec`) so `sl`
could hold the description pointer.

## Finding 2 (NEW, the win): a `Layout *lay` local in the credit loop

Declaring, immediately after `setVisible(true)` and before the `switch`:

```cpp
Layout *lay = Globals::layout;
```

and using `lay->field_0x20_top` in the `y` expression and
`lay->field_0x2c_rowHeight` in the `DrawImage2D` call (both inside the credit
loop only) makes the allocator keep `lay` in **sl** (`ldr.w sl,[sl]` once per
iteration) and gives the two Strings **r5 / r6** — the original's register
*pattern*. The long `0x14a90a..` run collapses: the credit-block diff falls from
1329 B to 583 B and total raw diff from 2059 to **1740** at 9700 B.

The region `0x14a93a..0x14a95c` (both constructors + `setVisible`) is now
byte-identical except the r5/r6 pair. Original: description=r6, price=r5.
Ours: description=r5, price=r6 (the allocator processes the first-started
interval first and assigns r5 before r6).

## Hunk classification at 1740 (25 clusters)

| addr | diff B | region |
|---|---:|---|
| 0x1496dd–0x1496f1 | 4 | blueprint `ldrd sl,r9` vs `r9,sl` |
| 0x14970a–0x14974c | 39 | blueprint progress |
| 0x149762–0x1497a6 | 16 | blueprint |
| 0x1497e4–0x14980d | 39 | blueprint |
| 0x149830–0x149841 | 4 | blueprint |
| 0x1499d4 | 1 | |
| 0x149b24–0x149b40 | 21 | ship branch (`this=r4,layout=r5` + ship load) |
| 0x149c80 | 1 | |
| 0x14a85a / 0x14a87a / 0x14a89c | 1 each | literal-pool offsets |
| 0x14a8ed–0x14a8f5 | 4 | `str r9,[sp,#132]` vs `strd sl,r9,[sp,#128]` |
| 0x14a90a–0x14a945 | 9 | credit-loop r5/r6 swap at the constructors |
| **0x14a960–0x14abca** | **574** | credit-loop r5/r6 swap (switch + y) |
| 0x14abe8–0x14abf3 | 2 | |
| 0x14ac52–0x14acad | 70 | loop exit / sl restore |
| 0x14ace8–0x14acf3 | 2 | |
| 0x14ad24–0x14aeca | 358 | free-credits block (sl no longer reused for buttonWidth) |
| 0x14b22e–0x14b3cc | 403 | free-credits width-first/layout |
| 0x14b3ec / 0x14b4e4 / 0x14b52a / 0x14b544 | 2/2/2/1 | |
| 0x14b562–0x14b61f | 163 | tail |
| 0x14b632–0x14b659 | 20 | EH cleanup dispatch |

## Experiments this wave (all on base-order source; reverted except the win)

| variant | diff | size |
|---|---:|---:|
| control (description-first) | 2062 | 9700 |
| price-first (wave-140 landed) | 2059 | 9700 |
| `(void)Globals::layout;` after / before the String decls | 2062 | 9700 |
| `(void)Globals::layout->field_0x20_top;` after / before | 2062 | 9700 |
| `Layout *lay` before `y`, used for y only | 2062 | 9700 |
| `Layout *lay` before `y`, used for y + DrawImage2D | 9153 | 9700 |
| `Layout &lay = *Globals::layout` for y + DrawImage2D | 9153 | 9700 |
| `y` computed before the switch | 9147 | 9700 |
| `lay` at loop top (before Strings), both uses | 1770 | 9700 |
| `lay` after both Strings, both uses | 1755 | 9700 |
| **`lay` after `setVisible`, both uses** | **1740** | **9700** |
| `lay` after `setVisible`, y only | 9105 | 9700 |
| `lay` const / `const` pointer | 1740–1770 | 9700 |
| `lay` + `*(Layout*volatile*)&Globals::layout` derefs | 2057 | 9700 |
| `lay` declared before `if (listModeFlag)` | 3369 | 9700 |
| swap `replaceTextKeepSize`/`setSplitText` order | 1746 | 9700 |
| `(void)price;` / `(void)description;` before switch | 1740 | 9700 |

## Open questions / next levers

* **Flip r5/r6.** Original description=r6, price=r5; ours description=r5,
  price=r6. The greedy allocator assigns the first-started interval first; a
  source shape that makes price's interval larger than description's (or that
  occupies r5 at description's allocation) should flip the pair and collapse the
  574-byte run. Swapping the call order is close (1746) but not a flip.
* **Free-credits block (358 B).** With `lay` in scope only in the credit loop,
  `sl` stays `&Globals::layout` through the free-credits block, whereas the
  original spills it at `0x14ac9c` and reuses `sl` for `buttonWidth`. Extending
  `lay` over the free-credits block (declared before `if (listModeFlag)`) is
  worse overall (3369) — needs a narrower shape.
* Blueprint `ldrd` pair (4 B) and ship-branch pair (21 B) are independent RA
  ties (see waves 138–140).

## Reproduction

```sh
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 /dev/shm/w8616/fdiff.py _ZN12HangarWindow6renderEv   # 1740, 9700 B
```

Scratch (not committed): `/dev/shm/w8616/{fdiff.py,side.py,classify.py,regen.sh,
probe.py,batch_a..g.py,HangarWindow.base.cpp,HangarWindow.setv.cpp}`.
