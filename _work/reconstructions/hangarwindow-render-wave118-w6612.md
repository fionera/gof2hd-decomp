# HangarWindow::render — wave 118 (w6612): residual map and measured variants, no gain

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`), original
`0x1490d8`, size `0x25e4` = 9700 B. Worktree
`/home/debian/GalaxyOnFire2/wt/w6612`, branch `wave118-w6612`.

Builds on `_work/reconstructions/hangarwindow-render-wave117-w6512.md`
(91.199559 landed). This pass: **91.199559 -> 91.199559** (no improvement
found). The deliverable is this measurement record.

## Result

| metric | before (HEAD 3d4a98a3e) | landed |
|---|---:|---:|
| `render` fuzzy (raw difflib, fresh resolvers) | 91.199559 | **91.199559** |
| `render` size | 9700 | 9700 |
| normalised insns (ours/orig) | 3635 / 3626 | 3635 / 3626 |
| `.text` | `0x1337c8` @ `0x69440` | same |
| `.ARM.exidx` | `0x6f70` @ `0x19cc08` | same |
| `.ARM.extab` | `0x2628c` @ `0x1a3b78` | same (orig `0x26298`) |
| `.rodata` | `0x41c27` @ `0x1c9e10` | same (orig `0x41b67`) |
| ELF section count | 27 | 27 |
| file size | 2,187,328 | 2,187,328 |
| byte_exact / linked_exact | 4501 / 4501 | 4501 / 4501 |
| size-exact / data / imports | 4524, 843, 207 | same |
| lint / drift | CLEAN / 0 | CLEAN / 0 |

No ELF section size, base or count changed. `render`'s own `.ARM.extab` record
is already exact (`exidx_table.py --diff` lists only `Level::createShip` −8 and
`Generator::createMission` −4). Gate: **RATCHET PASS**.

## Measurement method (important)

The old `harness.py` reused `Resolver` objects built from the `.so` that
existed at import time; when a variant moved other symbols the stale resolver
mis-named branches and produced wrong scores (e.g. 91.076838 vs the true
91.187001). All numbers below come from `measure2.py`, which rebuilds the
`Elf32`/`Resolver` pair on every measurement exactly like
`tools/verify/verify.py` does. Baseline is 91.199559 with 3635/3626 insns.

## Where the residual actually is (address-mapped)

The residual is four compact register-allocation regions plus their
literal-pool knock-on, not a source-shape error:

| region (function-relative) | absolute | what |
|---|---|---|
| `0x570`-`0x592` | `0x149648`-`0x14966a` | `item` spilled instead of kept in `r4` across `getStationAmount`; `this`+`layout` fetched as one `ldrd` before the branch instead of `this` before / `layout` after |
| `0x602`-`0x768` | `0x1496da`-`0x149840` | `ldrd sl,r9` (orig: `sl=&Globals::layout`, `r9=this`) vs `ldrd r9,sl` (ours: swapped) in the blueprint branch |
| `0xa48`-`0xae8` | `0x149b20`-`0x149bc0` | `ldrd r5,r4` + `cbz` (orig) vs `ldrd r6,r5` + `cmp/beq.w` (ours) in the ship branch |
| `0xbb4`-`0xbd4` | `0x149c8c`-`0x149cac` | else-arm of the `tab == 1` block (`item` reload) |
| `0x1766`-`0x1a2a` | `0x14a83e`-`0x14ab02` | `pV` store (2 B) + `mov r8,sl` copy in the buy-credits preheader (2 B) |
| `0x1b0e`-`0x1e60` | `0x14abe6`-`0x14af38` | buy-credits tail + free-credits block |

Everything between `0x149cac` and `0x14a83e` (2962 B: the rest of the list loop,
the scrollbar, `drawHeader`, the tab-button loop) is instruction-identical.

### The `item`-in-`r4` decision (selected-item block)

Original (`0x149648`):

```
149648: ldr r4,[sp,#120]   ; item
14964a: ldr r1,[sp,#108]   ; tab
14964c: ldr r6,[sp,#136]   ; this
14964e: ldr r0,[r4,#16]    ; item->item
149650: cmp r1,#1
149652: bne.w 149c88
149656: blx getStationAmount
14965a: ldr r5,[sp,#132]   ; layout (lazy, after the call)
14965e: ldr r0,[r4,#16]    ; item->item again, from r4
```

Ours:

```
149648: ldr r0,[sp,#120]   ; item (2 B)
14964c: ldr r0,[r0,#16]
149650: ldrd r5,r6,[sp,#132]  ; layout+this together, BEFORE the branch
149658: blx getStationAmount
149662: ldr r0,[sp,#120]   ; item reloaded
149664: ldr r0,[r0,#16]
```

The IR (`hw.ll`, block 410/414/427) computes `%385 = &item->item` and reloads
`*%385` after each call. Clang 7.0.2 keeps `%385` rematerialisable from the
spilled `item` (stack.98 = sp+120) instead of pinning `item` in `r4`. This is a
pure allocator tie; the source shape is the natural `item->item->getX()` in
both arms. The landed `volatile int pad3 = left;` (2 B at `0x149caa`) is a
byte-count compensation for the missing `ldr r4` and re-aligns the following
2962-byte identical region. Removing it alone is 88.855214.

### The `pV` store and the `mov r8,sl` copy

`volatile int pV = 0;` in the `viewMode == 1` block emits
`movs r0,#0; str r0,[sp,#312]` at `0x14a846`; the original has only `movs`.
The 2-byte store shifts every later branch target.

In the buy-credits preheader ours emits an extra `mov r8, sl` (2 B,
`0x14a914`) that copies `&Globals::layout` from `sl` to `r8` so `sl` can be
reused for the `sp+288` String temporary. The original keeps `sl` =
`&Globals::layout` and puts the String temp in `r8` directly. The two 2-byte
differences (pV store, `mov r8,sl`) are what keep the literal-pool offsets
matching; removing either alone is worse (pV: 91.006748, `mov r8,sl` cannot be
removed from source without re-allocating the whole function).

## What was tried this wave (all measured with `measure2.py`)

Base = 91.199559, 9700 B, 3635/3626 unless noted.

### Selected-item amount block

| variant | fuzzy | size | insns |
|---|---:|---:|---:|
| baseline (`if (tab == 1) { right=...; left=...; } else {...}`) | **91.199559** | 9700 | 3635:3626 |
| both amount blocks as `?:` (w3603 shape) | 87.527533 | 9704 | 3638:3626 |
| later block back to `if/else` (both `if/else`) | 81.667587 | 9652 | 3618:3626 |
| selected block `?:`, later block `if/else` | 81.887157 | 9660 | 3623:3626 |
| then-arm both calls via `this->selectedItem` | 91.187001 | 9700 | 3636:3626 |
| then-arm `getStationAmount` via `selectedItem` only | 87.346826 | 9704 | 3637:3626 |
| then-arm `getAmount` via `selectedItem` only | 91.199559 | 9700 | 3635:3626 |
| else-arm `getIndex` via `selectedItem` only | 91.199559 | 9700 | 3635:3626 |
| else-arm `getAmount` via `selectedItem` only | 87.346826 | 9704 | 3637:3626 |
| `Item *it = item->item;` both arms | 62.351969 | 9680 | 3636:3626 |
| `Item *it = item->item;` then-arm only | 87.291753 | 9704 | 3637:3626 |
| `Item &it = *item->item;` both arms | 62.957863 | 9680 | 3636:3626 |
| `int left, right;` / `int right = 0, left = 0;` / `const ListItem *item` | 91.199559 | 9700 | — |
| `if (0 < left)` / `if (right >= 1)` | 91.199559 | 9700 | — |
| `if ((unsigned) left > 0)` | 90.758849 | 9700 | — |

### `pad3` anchor (`if (left > 0)`)

| variant | fuzzy | size |
|---|---:|---:|
| `volatile int pad3 = left;` (landed) | **91.199559** | 9700 |
| `pad3 = y;` | 91.172015 | 9700 |
| `pad3 = this->field_0x114 + y;` | 91.116926 | 9700 |
| `pad3 = this->field_0x114;` | 91.006748 | 9700 |
| drop `pad3` | 88.855214 | 9696 |
| `pad3 = item->inTabIndex;` | 87.401900 | 9704 |
| `pad3 = tab;` / `pad3 = listWidth;` | 87.319290 | 9704 |
| `pad3 = item->item->getIndex();` | 68.238864 | 9708 |
| `pad3 = buttonOffset;` | 84.576154 | 9680 |
| drop `pad3` + drop `pV` | 88.662350 | 9696 |

### `pV` anchor (`viewMode == 1`)

| variant | fuzzy | size |
|---|---:|---:|
| `volatile int pV = 0;` (landed) | **91.199559** | 9700 |
| drop `pV` | 91.006748 | 9700 |
| `volatile int pV = this->viewMode;` / `pV = pV;` | 91.199559 | 9700 |

### Ship branch

| variant | fuzzy | size |
|---|---:|---:|
| `if (tab != 0) { if (!(tab==3 && selectedItem->ship==getShip())) ... }` (landed) | **91.199559** | 9700 |
| `if (tab == 1 && this->upgradeMode)` (operand order) | 91.144470 | 9700 |
| nested `if (tab == 1) { if (upgradeMode) ... }` | 91.144470 | 9700 |
| `if (tab == 0) {} else if (!(...)) ...` | 91.061837 | 9700 |
| inverted inner `if (tab==3 && ...) {} else {...}` | 91.199559 | 9700 |
| `padX` before the `if (tab != 0)` / `(void)padX` | 91.199559 | 9700 |
| `padX` inside the `if (tab != 0)` | 86.091986 | 9696 |

### Buy-credits block

| variant | fuzzy | size |
|---|---:|---:|
| baseline | **91.199559** | 9700 |
| `(void) data;` after the `GetApplicationData` call | 91.199559 | 9700 |
| `volatile int *const pad0 = &x;` | 91.199559 | 9700 |
| `volatile int *pad0 = &y;` | 91.199559 | 9700 |
| `pad0` moved after `replaceTextKeepSize` | 91.116926 | 9700 |
| `String price; String description;` (swap) | 91.034293 | 9700 |
| `pad0` before `setPosition` | 89.296046 | 9696 |
| `setVisible` before the String declarations | 89.158286 | 9696 |
| `pad0` after `setSplitText` / after `draw` | 88.800110 | 9696 |
| `Layout *lay = Globals::layout;` before the loop, used in the loop | 87.820336 | 9696 |
| `for (count = 0; count < 5; count++)` | 91.199559 | 9700 |
| `while (count <= 4)` | 63.777967 | 9112 |

### Free-credits block (`getWidth`/`getHeight` order)

| variant | fuzzy | size | insns |
|---|---:|---:|---:|
| landed order (`height=getHeight()` first, `textX=getWidth()+...`) | **91.199559** | 9700 | 3635:3626 |
| faithful `getWidth` first, `volatile float textYf` | 76.937904 | 9700 | 3637:3626 |
| faithful `getWidth` first, no `lay`, non-volatile `textYf` | 76.569383 | 9700 | 3638:3626 |
| faithful `getWidth` first, non-volatile `textYf`, with `lay` | 76.039658 | 9696 | 3636:3626 |

The faithful call order re-allocates the function and moves the literal pool;
it costs ~15 points despite being the true source order.

### Other

| variant | fuzzy | size |
|---|---:|---:|
| `int buttonOffset;` before `String name;` (swap) | 91.199559 | 9700 |
| `ListItem *item` before `int y` | 91.199559 | 9700 |
| `int i` instead of `unsigned int i` | 91.172015 | 9700 |
| `HangarList *list = this->hangarList;` alias in the selected block | 65.439446 | 9664 |
| `list` alias only for the first `getCurrentTab()` | 65.236645 | 9668 |

## Conclusion

The landed source is a strong local optimum. The residual is not a source-shape
error: it is (1) `item` rematerialised from its stack slot instead of pinned in
`r4`, (2) a `this`/`&Globals::layout` register-pair swap in the blueprint
branch, (3) a jump-threading difference (`cbz` vs `cmp/beq.w`) on the ship
branch's `tab == 0` edge, and (4) a `mov r8,sl` live-range split in the
buy-credits preheader. The `pad3`/`pV` volatile anchors are 2-byte
compensations that keep the following 2962-byte identical region and the
literal-pool offsets aligned; every relocation/removal measured worse.

No source change measured above 91.199559, so no code change was landed; the
gate counts are unchanged (byte 4501 / linked 4501, size 4524, data 843/843,
imports 207, lint CLEAN, drift 0, 27 sections, file size 2,187,328).

## Commands

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6   # RATCHET PASS byte 4501 linked 4501
cmake --build cmake-build-match --target lint  -j6   # CLEAN
cmake --build cmake-build-match --target drift -j6   # 0 fields
# per-variant fuzzy: measure2.py (rebuilds Elf32/Resolver each run, like verify.py)
```
