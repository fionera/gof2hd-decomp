# `HangarWindow::render` — wave 121 (w7111): allocator tie-break sweep, no gain

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`), original
`0x1490d8`, size `0x25e4` = 9700 B. Worktree `/home/debian/GalaxyOnFire2/wt/w7111`,
base master `bd061a489` (wave-120 state; the w6978 ternary amount block is landed,
so the `render` `.ARM.extab` record is already exact at 948 B).

This pass: **91.6552 -> 91.6552** (raw difflib; report rounds to 91.7). No source
change landed; the deliverable is this measurement record.

## Result

| metric | before (HEAD bd061a489) | landed |
|---|---:|---:|
| `render` fuzzy (raw difflib, fresh resolvers) | 91.6552 | **91.6552** |
| `render` size | 9700 | 9700 |
| `render` `.ARM.extab` record | 948 (exact) | 948 (exact) |
| `.text` | `0x1337c8` @ `0x69440` | same |
| ELF section count | 27 | 27 |
| file size | 2,187,328 | 2,187,328 |
| byte_exact / linked_exact | 4505 / 4505 | 4505 / 4505 |
| size-exact / data / imports | 4524, 843, 207 | same |
| lint / drift | CLEAN / 0 | CLEAN / 0 |

`check`: **RATCHET PASS** — `verify: byte 4505 linked 4505 stubs 0 missing 0
extra 0 avg 99.94`; `data: exact 843 of 843`; `sodiff: imports 207`; lint
`CLEAN ✓`; drift `0`.

## Residual map (current source, re-derived this wave)

Everything before `0x1496da` is instruction-identical. The residual is the same
set of allocator/scheduling decisions recorded in the wave-116/117/118 notes,
now measured against the w6978 ternary base:

| region (absolute) | what |
|---|---|
| `0x1496da`-`0x149840` | blueprint branch: orig `ldrd sl,r9,[sp,#132]` (`sl=&Globals::layout`, `r9=this`) vs ours `ldrd r9,sl` (swapped). Cascades into the `DrawRegion2D` argument schedule (`*layout` vs `this->progressBarWidth` first). |
| `0x149b20`-`0x149bc0` | ship branch: orig `ldrd r5,r4` + `cbz r0,149b96` vs ours `ldrd r6,r5` + `cmp r0,#0; beq.w 14a6d0`. The `cbz` threads to the `tab==1`/`upgradeMode` test; ours threads past it to the join block (4 B larger, shifts every later target). |
| `0x14a846` | `volatile int pV = 0;` emits `str r0,[sp,#312]` (4 B) the original does not have. |
| `0x14a910`-`0x14a93a` | buy-credits preheader: ours emits an extra `mov r8,sl` (2 B) so `sl` can hold the `sp+288` `String` temporary; the original keeps `&Globals::layout` in `sl` and puts the temp in `r8`. |
| `0x14ac72` | buy-credits loop tail: ours restores `mov sl,r8` (2 B) for the same `r8`/`sl` role swap. |
| `0x14ad22`-`0x14ad44` | free-credits: original calls `getWidth()` first, then reads `layout->field_0x2c_rowHeight`, then `getHeight()`; landed source calls `getHeight()` first. |
| `0x14ad6a`-`0x14ada0` | free-credits `textY`: the original keeps `rate*height*0.2f` in `s2`, converts to int before `getText`, and keeps it in `r6`. The landed `volatile float textYf` forces the store/reload through `[sp,#204]`/`s18` so the conversion happens after `getText`. |
| `0x14ae3c`-`0x14ae7c` | free-credits `i==0`/`i!=0` block ordering and loop tail; same code, different layout/registers. |
| `0x14b5xx`-`0x14b6xx` | cleanup landing pads: identical code, shifted 2-6 B by the above. |

The `ldrd sl,r9` swap is the dominant cost: it renames `sl`/`r9` through the
whole blueprint branch and re-schedules its independent loads. The `r8`/`sl`
swap in the buy-credits/free-credits half is the second independent allocator
tie.

## What was tried this wave (all measured with the real build + verify)

Base = 91.6552, 9700 B, unless noted.

### Free-credits block

| variant | fuzzy | size |
|---|---:|---:|
| landed: `height` first, `lay` alias, `volatile float textYf` | **91.6552** | 9700 |
| faithful `textX` first (`lay` alias, volatile textY) | 91.0419 | 9700 |
| faithful `textX` first, no `lay`, volatile textY | 91.1000 | 9700 |
| faithful `textX` first, no `lay`, non-volatile `textY` | 90.5733 | 9700 |
| faithful `textX` first, `int width` temporary, non-volatile | 90.5733 | 9700 |
| faithful `textX` first, `lay` alias, non-volatile | 90.4230 | 9700 |
| faithful + `textY` computed before `SetColor` | 90.4230 | 9700 |
| faithful + `textY` computed before `SetColor` (volatile) | 88.5029 | 9692 |
| drop `volatile` from `textYf` (landed order) | 90.5000 | 9700 |
| `volatile int textY` | 88.8705 | 9688 |
| `int textY` + `volatile int textYbarrier` | 91.6001 | 9700 |
| `const int textY` | 90.5333 | 9700 |
| `volatile int textYf` (int) | 88.6777 | 9692 |
| `volatile float textYf` then non-volatile float copy | 91.6552 | 9700 |
| `volatile double textYf` | 74.2668 | 9704 |
| `height` after `textX` (landed order otherwise) | 91.0419 | 9700 |
| `SetColor` before `height` | 91.1797 | 9700 |
| `textX = width + rowHeight + baseX` | 91.6552 | 9700 |
| `textYf = height*0.2f + y` | 91.6552 | 9700 |
| `if (i == 0) {small} else {big}` (body swap) | 91.7138 | **9720** |
| body swap + drop `pV` | 91.5208 | 9720 |
| body swap + `String("", false)` | 91.7138 | 9720 |

The body swap is the only variant above 91.6552, but it costs +20 B (the
`i==0` arm becomes the fallthrough and the buy-credits half gains extra moves),
so it cannot be landed without a compensating -20 B elsewhere.

### Blueprint branch (`ldrd` tie)

| variant | fuzzy | size |
|---|---:|---:|
| landed | **91.6552** | 9700 |
| `Layout *lay = Globals::layout;` at the top of the branch | 91.6552 | 9700 |
| `HangarWindow *self = this;` at the top of the branch | 91.6552 | 9700 |
| `x` expression reordered layout-first | 91.6552 | 9700 |
| `y` expression reordered layout-first / `1`-first | 91.6552 | 9700 |
| `x` and `y` arguments swapped | 91.3798 | 9700 |
| `int pw = this->progressBarWidth;` local | 84.5105 | 9708 |
| `int bg = this->progressBarBgImage;` local | 83.8266 | 9708 |
| `int hint = this->hintOffsetX;` local | 83.8266 | 9708 |
| single `getCompletionRate()` call (`float rate` before `if`) | 86.3286 | 9692 |
| `double rate` | 85.6474 | 9720 |
| `Layout *lay` at function top, all `Globals::layout->` -> `lay->` | 40.5327 | 9576 |

### Ship branch / condition shapes

| variant | fuzzy | size |
|---|---:|---:|
| landed `if (tab != 0) { if (!(...)) ... }` | **91.6552** | 9700 |
| `if (tab != 0 && !(tab==3 && ...))` | 91.6552 | 9700 |
| `if (tab != 3 || ship != getShip())` | 91.6552 | 9700 |
| `upgradeMode` check nested inside the inner `if` | 91.6552 | 9700 |
| `upgradeMode` check nested inside the outer `if` | 91.6552 | 9700 |
| `if (tab > 0)` | 91.5175 | 9700 |
| explicit `if (tab == 0) {} else if (...)` | 91.5175 | 9700 |
| `this->selectedItem->ship` -> `item->ship` | 84.7808 | 9700 |
| `if (this->selectedItem == item)` | 91.6277 | 9700 |
| `isShip() || isItem() || ...` | 91.6001 | 9700 |
| `isShip() || isBluePrint() || isPendingProduct() || isItem()` | 91.5726 | 9700 |

### Anchors / declarations / branch order

| variant | fuzzy | size |
|---|---:|---:|
| drop `pV` | 91.4624 | 9700 |
| `pV = 1` | 91.6001 | 9700 |
| `pV` after `this->render()` | 91.5726 | 9700 |
| `volatile int padX` (int, not pointer) | 84.4163 | 9708 |
| drop `padX` | 84.8568 | 9700 |
| `barHeight`/`barPos` declaration swap | 91.5175 | 9700 |
| `listWidth` declared first | 84.1526 | 9708 |
| `tab`/`items` declaration swap | 78.2405 | 9664 |
| `iconSpace` declared before `listWidth` | 91.6552 | 9700 |
| `row`/`col` declaration swap | 91.6552 | 9700 |
| `x`/`y` declaration swap (buy credits) | 91.6552 | 9700 |
| `Layout *lay` in buy-credits block | 91.6552 | 9700 |
| `data` after `count` | 91.6552 | 9700 |
| reorder `else if` chain (blueprint first / ship first) | 62.9 / 45.6 | 9692 / 9668 |

## Conclusion

The landed wave-119 source is still a strong local optimum at 91.6552. The
residual is two independent register-allocation tie-breaks (`ldrd sl,r9` vs
`ldrd r9,sl` at `0x1496da`; `cbz` vs `cmp`/`beq.w` at `0x149b24`), their
`r8`/`sl` knock-on in the buy-credits/free-credits half, and the 4 B `pV`
store. Every source-shape variant that makes a block more faithful (notably
the original `getWidth`-before-`getHeight` order and the non-volatile
`textY`) re-allocates the whole function and loses more than it gains.

The only measured improvement, the free-credits `i==0`/`i!=0` body swap
(91.7138), is +20 B and therefore cannot be landed under the size-exact gate.
A future pass that can pair it with a -20 B text-neutral change should
re-measure it.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
cmake --build cmake-build-match --target drift -j6      # 0 fields
cmake --build cmake-build-match --target lint -j6       # CLEAN
python3 tools/verify/verify.py --show _ZN12HangarWindow6renderEv
```

Scratch (not committed): `/dev/shm/w7111_orig.asm`, `/dev/shm/w7111_ours.asm`,
`/dev/shm/score1.py` (raw single-function difflib scorer), `batch*.py` (the
variant sweeps above).
