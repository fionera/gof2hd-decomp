# HangarWindow::render — wave 116 (w6412): allocation-anchor sweep, no gain

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`), real
address `0x1490d8`, size `0x25e4` = 9700 B. Worktree
`/home/debian/GalaxyOnFire2/wt/w6412`, branch `wave116-w6412`.

Builds on `_work/reconstructions/hangarwindow-render-wave115-w6313.md`
(91.199559 landed). This pass: **91.199559 -> 91.199559** (no improvement
found). The deliverable is this measurement record.

## Result

| metric | before (HEAD d672d349f) | landed |
|---|---:|---:|
| `render` fuzzy (raw difflib) | 91.199559 | **91.199559** |
| `render` size | 9700 | 9700 |
| normalised insns | 3626 / 3635 | 3626 / 3635 |
| `.text` | `0x1337c8` @ `0x69440` | same |
| `.ARM.extab` | `0x26294` @ `0x1a3b78` | same |
| ELF section count | 27 | 27 |
| file size | 2,187,328 | 2,187,328 |
| byte_exact / linked_exact | 4501 / 4501 | 4501 / 4501 |
| size-exact / data / imports | 4524, 843, 207 | same |
| lint / drift | CLEAN / 0 | CLEAN / 0 |

No ELF section size, base or count changed. `.ARM.extab` stays 4 B short of the
original (`0x26294` vs `0x26298`), as at the start of the pass.

## The remaining diff is 226 difflib opcodes / 391 changed tokens

Using `tools/verify`'s normalised streams (not the index-aligned `--show`
output, which overstates the diff because a single pool word shifts every
following line):

```
ops 226  total changed 391
```

The largest clusters are all in the free-credits block (`0x14acae`-`0x14af00`):

| cluster | orig | ours | len |
|---|---|---|---:|
| `i == 0` body vs literal pool | `[2857:2858]` | `[2843:2869]` | 26 |
| `i != 0` body / pool | `[2817:2838]` | `[2828:2829]` | 21 |
| getWidth/getHeight register roles | `[2710:2718]` | `[2716:2719]` | 8 |
| textX/textY register roles | `[2734:2741]` | `[2738:2739]` | 7 |
| loop tail | `[2839:2845]` | `[2830:2831]` | 6 |
| `visible` branch | `[2707:2708]` | `[2709:2714]` | 5 |
| textX add | `[2750:2750]` | `[2748:2753]` | 5 |
| cleanup landing pads | `[3546:3613]` | `[3554:3613]` | ~20 |

Everything before normalised index 522 (about 14% of the function) is
instruction-identical; the first divergence is the selected-item block at
`0x149648`.

## Root cause: one register-role swap propagates

At the blueprint branch (`0x1496da`) the original reloads the two spilled
pointers as

```
1496da: e9dd a921   ldrd sl, r9, [sp, #132]   ; sl = Globals::layout, r9 = this
```

while ours emits

```
1496da: e9dd 9a21   ldrd r9, sl, [sp, #132]   ; r9 = Globals::layout, sl = this
```

`sp+132` is `Globals::layout` (spilled at `0x1491b0`) and `sp+136` is `this`
(spilled at `0x14911a`). The instruction stream after the `ldrd` is otherwise
identical, only `sl`/`r9` are exchanged. This one allocator tie-break is
responsible for most of the 391 changed tokens: every subsequent
`ldr.w rX,[sl]` vs `[r9]`, and every branch that crosses the region, differs.
It also shows up in the free-credits loop as the `layout`/`baseX` roles
(`r8` vs `sl`), which is why the faithful getWidth-first source shape loses so
much (see below).

A second, independent allocator choice is the `cbz` at `0x149b24`:

```
orig:  149b24: b3b8        cbz r0, 149b96      ; tab == 0 -> upgradeMode check
ours:  149b24: 2800        cmp r0, #0
       149b26: f000 85d3   beq.w 14a6d0        ; tab == 0 -> join block
```

Both are semantically correct; the original keeps the 2-byte `cbz` to the
nearby `cmp tab,#1` block, ours threads the edge to the far join block with a
6-byte `cmp`/`beq.w`. This costs 4 bytes and shifts every later branch target.

## What was tried (all measured with the real build + verify)

Base = 91.199559 unless noted. Every row keeps 9700 B / `.text 0x1337c8` /
`.ARM.extab 0x26294` unless a different size is shown.

### Free-credits call order / block shape

| variant | fuzzy | size | insns |
|---|---:|---:|---:|
| `int width = getWidth(); int height = getHeight(); textX = width + baseX + lay->rowHeight` | 76.480308 | 9696 | 3626:3636 |
| `int textX = getWidth() + baseX + lay->rowHeight; int height = getHeight();` | 76.937904 | 9700 | 3626:3637 |
| drop `volatile` from `textYf` (`int textY = (int)((float)y+(float)height*0.2f)`) | 76.012118 | 9696 | 3626:3636 |
| swap `if (i != 0)` / `else` | 91.036955 | 9720 | 3626:3626 |
| `if (i == 0) { empty } else { ... }` + `continue` | 91.036955 | 9720 | 3626:3626 |
| ternary `String text = i != 0 ? ... : String("")` | 71.375362 | 9640 | 3626:3623 |
| named `String text; if/else text = ...` | 80.435982 | 9700 | 3626:3622 |
| ternary directly in `DrawString` | 88.272627 | 9668 | 3626:3622 |
| `String("", false)` for the empty branch | 91.199559 | 9700 | 3626:3635 |
| `if (tab != 0)` -> `if (tab)` / `if (tab >= 1)` / `if (tab > 0)` | 91.199559 / 91.061837 / 91.061837 | 9700 | 3626:3635 |
| `if (this->upgradeMode && tab == 1)` -> `if (tab == 1 && this->upgradeMode)` | 59.862259 | 9684 | 3626:3634 |
| same, nested `if (tab == 1) { if (upgradeMode) ... }` | 91.144470 | 9700 | 3626:3635 |
| same, nested `if (upgradeMode) { if (tab == 1) ... }` | 91.199559 | 9700 | 3626:3635 |
| `if (upgradeMode * (tab == 1))` | 91.199559 | 9700 | 3626:3635 |
| `if (upgradeMode & (tab == 1))` | 66.657489 | 9708 | 3626:3638 |

Note: removing the `volatile` from `textYf` *does* fix the free-credits block
order (the `i == 0` body moves before the literal pool, matching the original),
but it changes the whole-function allocation and drops the score by 15 points.
Adding a compensating `volatile int anchor = 0;` at the top, in the
free-credits block, in the buy-credits loop, or in the list loop gives
64.12 / 76.43 / 76.38 / 64.23 respectively (all 9700 B, 3626:3638).

### Buy-credits block

| variant | fuzzy | size |
|---|---:|---:|
| `for (count = 0; count < 5; count++)` | 91.199559 | 9700 |
| `int count = 0;` | 91.199559 | 9700 |
| `unsigned int count` + `count < 5` | 91.199559 | 9700 |
| swap `description`/`price` declarations | 91.034293 | 9700 |
| `description = Globals::cItemListDescription_00;` (implicit) | 62.598154 | 9668 |
| implicit for both description and price | 65.618102 | 9660 |
| `String("", false)` in `default:` | 91.199559 | 9700 |
| `switch ((int) count)` | 91.199559 | 9700 |
| move `pad0` after `replaceTextKeepSize` | 91.116926 | 9700 |
| drop `pad0` | 89.296046 | 9696 |
| swap `x`/`y` declaration order | 91.199559 | 9700 |

### Anchors / declarations

| variant | fuzzy | size |
|---|---:|---:|
| drop `padX` | 83.413627 | 9700 (`.ARM.extab 0x26288`) |
| move `padX` after `if (tab != 0)` | 86.091986 | 9696 (`.ARM.extab 0x26290`) |
| move `padX` into the blueprint branch | 79.146593 | 9708 (`.ARM.extab 0x2628c`) |
| add a second `padX` in the blueprint branch | 86.458190 | 9704 (`.ARM.extab 0x26298`) |
| move `pad3` into the blueprint branch | 71.247590 | 9712 (`.ARM.extab 0x2628c`) |
| drop `pad3` | 88.855214 | 9696 (`.ARM.extab 0x26288`) |
| `pad3 = 0` | 87.319290 | 9704 |
| `volatile int *pad3 = &left;` | 90.786393 | 9700 |
| `int *volatile padX = 0;` | 74.697137 | 9724 (`.ARM.extab 0x2628c`) |
| drop `pV` | 91.006748 | 9700 |
| `pV = 1` | 91.144470 | 9700 |
| move `pV` before `viewMode = 0` | 91.199559 | 9700 |
| `String name;`/`int buttonOffset;` swap or `buttonOffset = 0` | 91.199559 | 9700 |
| `String name("")` | 86.017066 | 9712 |
| `volatile bool upgradeMode` (header) | 86.823627 | 9708 |

### Condition / operand order

| variant | fuzzy | size |
|---|---:|---:|
| `if (this->selectedItem == item)` | 91.172015 | 9700 |
| `if (item->isShip() || item->isItem() || ...)` | 91.144470 | 9700 |
| `if (0.0f < getCompletionRate())` | 91.199559 | 9700 |
| `if (tab == 1 || tab == 4)` | 91.199559 | 9700 |
| `if (i == items->size() - 1 && tab == 4)` | 85.722153 | 9704 |
| `int left, right;` / separate declarations | 91.199559 | 9700 |
| `col = (unsigned char)(count - row*3)` | 72.119879 | 9724 |
| `if (left >= 1)` / `if (right >= 1)` | 91.199559 | 9700 |
| `if (tab != 4)` for the `tab == 1` split | 90.015149 | 9700 |

### Whole-function aliases

| variant | fuzzy | size |
|---|---:|---:|
| `HangarWindow *self = this;` at top, `self->` in render | 91.199559 | 9700 |
| `Layout *layout = Globals::layout;` at top, `layout->` in render | 39.699958 | 9508 |
| `Layout *lay = Globals::layout;` inside `if (items != nullptr)` | 48.316941 | 9628 |
| `Layout *lay` inside the blueprint branch | 86.210222 | 9704 |
| `HangarWindow *self` inside the blueprint branch | 86.458190 | 9704 |
| temporaries for the blueprint `DrawImage2D` args | 83.535242 | 9708 |

## Conclusion

The landed wave-114/115 source remains a strong local optimum at 91.199559.
The residual is two allocator decisions, not source-shape errors:

1. `ldrd sl,r9` vs `ldrd r9,sl` at `0x1496da` (`Globals::layout`/`this`), which
   propagates through the whole list loop and the free-credits block.
2. `cbz` vs `cmp`/`beq.w` at `0x149b24` (jump threading of the `tab == 0`
   edge), which costs 4 bytes and shifts every later branch target.

The free-credits call order is the only call-sequence mismatch
(`getWidth` before `getHeight` in the original), but making it faithful swaps
the `layout`/`baseX` register roles upstream and costs ~15 points. The
`volatile` anchors (`textYf`, `pV`, `padX`, `pad3`) are load-bearing for the
current allocation and for `.ARM.extab`; every relocation/removal measured
worse. No source change measured above 91.199559, so no code change was
landed; the gate counts are unchanged (byte 4501 / linked 4501, lint CLEAN,
drift 0, `.ARM.extab 0x26294`, 27 sections, file size 2,187,328).
