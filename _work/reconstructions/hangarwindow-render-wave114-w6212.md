# HangarWindow::render — wave 114 (w6212): faithful `selectedItem->ship` + `b`

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`), real
address `0x1490d8`, size `0x25e4` = 9700 B. Worktree
`/home/debian/GalaxyOnFire2/wt/w6212`, branch `wave114-w6212`, base master
`d8557f7e2`.

Builds on `_work/reconstructions/hangarwindow-render-wave113-w6111.md`
(89.341779). This pass: **89.341779 -> 91.199559**.

## Result

| metric | before (HEAD d8557f7e2) | landed |
|---|---:|---:|
| `render` fuzzy (raw difflib) | 89.341779 | **91.199559** |
| `render` fuzzy (verify, 0.1) | 89.3 | **91.2** |
| `render` size | 9700 | 9700 |
| normalised insns | 3626 / 3635 | 3626 / 3635 |
| `.text` | `0x1337c8` @ `0x69440` | same |
| `.ARM.exidx` | `0x6f70` @ `0x19cc08` | same |
| `.ARM.extab` | `0x26298` @ `0x1a3b78` | same |
| ELF section count | 27 | 27 |
| file size | 2,187,328 | 2,187,328 |
| byte_exact / linked_exact | 4500 / 4500 | 4500 / 4500 |
| size-exact / data / imports | 4524, 843, 207 | same |
| lint / drift | CLEAN / 0 | CLEAN / 0 |

No ELF section size, base or count changed. `.rodata` remains `0x41c27`
(known post-mission-table delta); everything before `0x20b977` is unchanged.

## Landed diff (3 hunks)

```diff
-                    ListItem *item = (*items)[i];
+                    ListItem *const item = (*items)[i];

-                                if (!(tab == 3 && item->ship == Globals::status->getShip())) {
+                                if (!(tab == 3 && this->selectedItem->ship == Globals::status->getShip())) {

-                                        int b = item->inTabIndex == -1 ? 4 : 3;
+                                        int b = 3 + (item->inTabIndex < 0);

-            int visible;
+            bool visible;
```

The two selected-ship hunks are **only** a win together. Ground truth from the
original Thumb at `0x149b24`:

```
149b24: b3b8      cbz  r0, 149b96      ; tab == 0 -> skip button[1]
149b26: 2803      cmp  r0, #3
149b28: d10c      bne.n 149b44         ; tab != 3 -> button[1] body
149b2a: 4855      ldr  r0, [pc, #340]
149b2c: 6ea1      ldr  r1, [r4, #104]  ; r4 = this; this->selectedItem (+0x68)
149b2e: 4478      add  r0, pc
149b30: 6800      ldr  r0, [r0, #0]
149b32: 68cc      ldr  r4, [r1, #12]   ; selectedItem->ship (+0xc)
149b34: 6800      ldr  r0, [r0, #0]
149b36: f717 ef90 blx  Status::getShip
149b3a: 4284      cmp  r4, r0
```

so the original really reads `this->selectedItem->ship` (not the loop's `item`,
even though `item == this->selectedItem` at that point). And at `0x14a680`:

```
14a666: 2403      movs r4, #3
14a670: 6bdb      ldr  r3, [r3, #60]   ; item->inTabIndex
14a680: eb04 74d3 add.w r4, r4, r3, lsr #31   ; b = 3 + (inTabIndex < 0)
```

so `b = 3 + (item->inTabIndex < 0)` is the faithful source, not the `?:` form.

`bool visible` (free-credits loop) is worth +0.055 on its own
(89.341779 -> 89.396860); `int`/`char`/`uint8_t`/`unsigned` all measure
89.341779. The original computes it as
`(i != 4) | !options[76]` (`0x14aeb4`: `clz`/`lsr #5`/`orr`), i.e. a genuine
`bool`.

## Measurements (base = 89.341779 unless noted)

All rows 9700 B / 3626:3635 unless marked.

### The winning combination

| variant | fuzzy |
|---|---:|
| `bool visible` only | 89.396860 |
| `selectedItem->ship` only | 88.02 |
| `3 + (inTabIndex < 0)` only | 87.6 |
| `selectedItem->ship` + `b` (no bool) | 91.144470 |
| **all three (landed)** | **91.199559** |

### Dead ends (all on top of the landed trio)

| variant | fuzzy | size | insns |
|---|---:|---:|---:|
| swap `if (i != 0)` / `else` branch order | 91.036955 | 9720 | 3626:3626 |
| `visible = !(i == 4 && options[76])` | 91.116926 | 9700 | 3626:3635 |
| `if (!visible) continue;` restructure | 91.199559 | 9700 | 3626:3635 |
| `bool visible = true;` + if/else | 91.199559 | 9700 | 3626:3635 |
| `button->setVisible(visible != false)` | 91.199559 | 9700 | 3626:3635 |
| `for (count = 0; count < 5; count++)` | 91.199559 | 9700 | 3626:3635 |
| `int count = 0;` | 91.199559 | 9700 | 3626:3635 |
| `inTabIndex >= 0` / `left >= 1` / `right >= 1` | 91.199559 | 9700 | 3626:3635 |
| `tab != 3 \|\| selectedItem->ship != getShip()` | 91.199559 | 9700 | 3626:3635 |
| `(i == 1 && options[73]) ...` operand order | 88.503373 | 9692 | 3626:3637 |
| `col = (unsigned char)(count - row*3)` | 72.119879 | 9724 | 3626:3648 |
| `width` / `textX` before `height` (free credits) | 76.937904 | 9700 | 3626:3637 |
| `width` local, no `lay` alias | 75.657804 | 9688 | 3626:3633 |
| drop the `Layout *lay` alias | 90.806340 | 9700 | 3626:3629 |
| drop `volatile` from `textYf` | 76.012118 | 9696 | 3626:3636 |
| `SetColor` after `textYf` | 88.392611 | 9696 | 3626:3628 |
| `textX` after `textY` | 90.696072 | 9700 | 3626:3629 |

## Remaining diff clusters (226 opcodes, 315 vs 324 changed insns)

* **`0x14ae68`/`0x14ae80` literal-pool placement** (norm. insn 2817-2860,
  ~46 lines, the largest chunk). The original lays out
  `[i==0 empty branch][loop tail][loop increment][literal pool(13 words)]`;
  ours lays out `[literal pool(12 words)][i==0 empty branch][loop tail]
  [loop increment][literal pool(1 word)]`. Pure `ARMConstantIslandPass`
  block-placement; every source reordering tried (branch swap, `continue`,
  hoisting `String("")`, moving `row++`) either keeps it or loses points.
* **`0x14ad2c`-`0x14ae40` buy-credits `String` temp register roles**
  (norm. 2379-2520, ~30 `mov r0, r8` vs `mov r0, sl`). Original keeps the
  `String` temp at `sp+312` in `r6`; ours keeps it in `sl`, and spills an
  extra live value to `sp+128` (`strd sl, r9` vs `str r9`).
* **`0x149b1e` selected-ship block register roles** (norm. 981-1046):
  original `r4 = this`, `r5 = [sp+132]`; ours `r5 = this`, `r6 = [sp+132]`,
  plus a `cbz` vs `cmp/beq.w` difference because the original's `tab == 0`
  target is the local `0x149b96` while ours is the distant `0x14a6d0`.
* **`0x14a6d0`-`0x14a718` `b`/button body**: `sl`/`r8` role swap and a
  `GlobalTable` load order difference.
* **`0x14b4xx`-`0x14b6xx` cleanup / landing-pad region** (norm. 3424-3616):
  ~44 `b.n` target deltas; `.ARM.extab` is already size- and base-exact.

The stack-slot colouring of `buttonOffset` (original `[sp,#80]`, ours
`[sp,#308]`) is unchanged from wave 111 and is not reachable from source
without perturbing the whole list-body allocation.
