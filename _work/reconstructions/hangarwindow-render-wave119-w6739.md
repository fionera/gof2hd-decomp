# HangarWindow::render — wave 119 (w6739): anchor/scope sweep, no gain

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`), original
`0x1490d8`, size `0x25e4` = 9700 B. Worktree
`/home/debian/GalaxyOnFire2/wt/w6739`, branch `wave119-w6739`, base `6552d3af5`.

Builds on `hangarwindow-render-wave119-w6710.md` and
`hangarwindow-render-wave118-w6612.md` (91.199559 landed). This pass:
**91.199559 -> 91.199559** (no improvement found). Deliverable is this
measurement record plus the new negative results below.

## Result

| metric | before (HEAD 6552d3af5) | landed |
|---|---:|---:|
| `render` fuzzy (fresh Elf32/Resolver, same normalisation as `verify.py`) | 91.199559 | **91.199559** |
| `render` size | 9700 | 9700 |
| normalised insns (ours/orig) | 3635 / 3626 | 3635 / 3626 |
| byte_exact / linked_exact | 4505 / 4505 | 4505 / 4505 |
| size-exact / data / imports | 4524, 843, 207 | same |
| lint / drift | CLEAN / 0 | CLEAN / 0 |
| ELF section count / file size | 27 / 2,187,328 | same |

No section, base or size changed. Gate: **RATCHET PASS**.

## Method

Every variant below was applied to a pristine copy of
`src/game/ui/HangarWindow.cpp`, rebuilt with
`cmake --build cmake-build-match --target link_so -j8` (~1.1 s) and scored with a
harness that rebuilds the `Elf32`/`Resolver` pair on every measurement (the
same way `tools/verify/verify.py` does — reusing resolvers across variants
mis-names branches and gives wrong scores). Baseline is 91.199559 %,
9700 B, 3635/3626 insns.

## Why the anchors matter (new observation)

`verify.normalize()` masks only `0x`-prefixed hex (`HEX = \b0x[0-9a-fA-F]+\b`).
objdump prints **stack offsets and literal-pool offsets in decimal**
(`[sp, #120]`, `ldr r0, [pc, #340]`), so those are compared literally. The
`pad3`/`pV`/`pad0`/`padX` "anchors" are therefore not register-allocation
noise: they pin the frame layout / pool layout, and a dead declaration can
change the score by >8 points. Confirmed directly:

| variant | fuzzy |
|---|---:|
| `volatile int *padX = 0;` (landed) | **91.199559** |
| `int padX;` (unused, no volatile) | 91.199559 |
| `int padX = 0;` (unused) | 91.199559 |
| `bool padX = false;` | 91.199559 |
| `const int *padX = 0;` | 91.199559 |
| `volatile int *padX = nullptr;` | 91.199559 |
| `volatile int *padX = &tab;` | 91.199559 |
| padX deleted | 83.413627 (3639 insns) |
| padX moved after the `if (tab != 0)` block | 83.413627 (3639) |
| `volatile int padX = 0;` | 74.697137 (9724 B, 3638) |

So the ship-branch declaration is a pure frame/scope anchor; its *value* is
irrelevant, its *position* is not.

## Residual map (unchanged from wave 118)

| region (fn-relative) | what |
|---|---|
| `0x570`-`0x592` | selected-item amount block: orig keeps `item` in `r4` across `getStationAmount` and reloads `item->item` from `r4`; we rematerialise `item` from `sp+120`. Orig loads `this` (`r6`) before the branch and `&Globals::layout` (`r5`) after the call; we `ldrd r5,r6` both before |
| `0x602`-`0x768` | blueprint block: orig `ldrd sl,r9` (`sl`=layout, `r9`=this) vs ours `ldrd r9,sl` — global `r8`/`sl`/`r9` role swap (~40 lines) |
| `0xa48`-`0xae8` | ship branch: orig `ldrd r5,r4` + `cbz r0,0x149b96`; ours `ldrd r6,r5` + `cmp/beq.w 0x14a6d0` (SimplifyCFG threads the `tab==0` edge past the shared `tab==1` test) |
| `0xbb4`-`0xbd2` | else-arm `item` reload (`mov r0,r4` vs `ldr r0,[sp,#120]`) |
| `0x1766`-`0x1a2a` | `pV` store (2 B) + `mov r8,sl`/`mov sl,r8` around the buy-credits block |
| `0x1b0e`-`0x1e60` | buy-credits tail + free-credits block (role swap; free-credits `getWidth`/`getHeight` order) |

Everything between `0x149cac` and `0x14a83e` (2962 B) is instruction-identical.
The buy-credits loop body itself is instruction-identical except that the
`String` temporary address lives in `r8` in the original and `sl` in ours
(`mov r0,r8` vs `mov r0,sl`, ~180 diff blocks); it is the same `layout`/
temporary role swap as the blueprint block, plus the two extra `mov`s that
move `layout` between `sl` and `r8` at the block boundaries.

## New measurements this wave (all <= 91.199559)

Baseline = 91.199559 / 9700 B / 3635:3626 unless noted.

### Selected-item amount block — anchors

| variant | fuzzy | size / insns |
|---|---:|---:|
| landed `volatile int pad3 = left;` | **91.199559** | 9700 / 3635 |
| `volatile ListItem *pad3 = item;` (pointee-volatile, store DCE'd) | 91.199559 | 9700 / 3635 |
| `pad3` + `volatile ListItem *padI = item;` (DCE'd) | 91.199559 | 9700 / 3635 |
| `pad3` + `padI` + `(void) padI;` | 91.199559 | 9700 / 3635 |
| `padI` only (DCE'd) | 90.786393 | 9700 / 3635 |
| `volatile Item *pad3 = item->item;` | 90.786393 | 9700 / 3635 |
| `ListItem *volatile pad3 = item;` | 87.319290 | 9704 / 3637 |
| `volatile int *pad3 = &left;` | 90.786393 | 9700 / 3635 |
| `int pad3 = left; volatile int *pad3p = &pad3;` | 90.786393 | 9700 / 3635 |
| `volatile int *pad3 = &right;` / `&y;` / `&tab;` / `&listWidth;` | 90.786393 | 9700 / 3635 |
| `volatile int padE = left;` in the else arm | 87.346826 | 9704 / 3637 |
| `volatile int padE = right;` in the then arm | 85.875551 | 9708 / 3638 |
| `volatile int padE = left;` after the if/else join | 86.599642 | 9700 / 3635 |
| `volatile int padE = 0;` before the if/else | 86.465648 | 9704 / 3637 |
| `volatile int padE = right;` after the join | 82.472807 | 9708 / 3637 |

### Selected-item amount block — source forms

All of the following are exactly 91.199559 / 9700 / 3635 (no codegen change):
`int left, right;` declaration order; `if (0 < left)`; `if (0 < right)`;
both flipped; `ListItem *const item`; `items->data()[i]`; `int right = 0,
left = 0;`; `int right(0), left(0);`; `if (left >= 1)`; `if (right >= 1)`;
`if (right > 0 || tab == 4)`.

| variant | fuzzy | size / insns |
|---|---:|---:|
| `int right, left;` moved to the top of the loop body | 85.151515 | 9688 / 3634 |
| `int right, left;` moved before `if (item == this->selectedItem)` | BUILD FAIL (scope) | — |

### Blueprint block

| variant | fuzzy | size / insns |
|---|---:|---:|
| landed (`Globals::layout->` inline) | **91.199559** | 9700 / 3635 |
| `Layout *lay = Globals::layout;` at block start, `lay->` inside | 85.170893 | 9704 / 3630 |

### Ship branch

| variant | fuzzy | size / insns |
|---|---:|---:|
| landed (`volatile int *padX = 0;`) | **91.199559** | 9700 / 3635 |
| `if (tab)` instead of `if (tab != 0)` | 91.199559 | 9700 / 3635 |
| inner `tab != 3 \|\| this->selectedItem->ship != getShip()` | 91.199559 | 9700 / 3635 |
| `if (tab == 1 && this->upgradeMode)` | 59.862259 | 9684 / 3634 |
| `bool sameShip = ...; if (!sameShip)` | 83.934968 | 9684 / 3632 |

### Buy / free credits

| variant | fuzzy | size / insns |
|---|---:|---:|
| landed | **91.199559** | 9700 / 3635 |
| `Layout *lay = Globals::layout;` at buy-block start | 70.820124 | 9620 / 3629 |
| same for buy+free block | BUILD FAIL (duplicate `lay`) | — |
| `volatile int pV = this->viewMode;` | 91.199559 | 9700 / 3635 |
| `pV` before `this->viewMode = 0;` | 91.199559 | 9700 / 3635 |
| `volatile long pV = 0;` | 91.199559 | 9700 / 3635 |
| `pV` after `this->render();` | 91.116926 | 9700 / 3635 |
| `int pV = 0;` (non-volatile) | 91.006748 | 9700 / 3635 |
| `volatile int *pV = 0;` | 91.006748 | 9700 / 3635 |

### Dead-declaration scan

Inserted `int padF;` (and separately `volatile int padF;`) after **every 4th
line** of the whole buy-credits + free-credits region (lines 916-1078, 41
positions each): every insertion scored exactly 91.199559. A single dead
declaration in that region has no effect; only the ship-branch `padX` position
is load-bearing.

### Misc

| variant | fuzzy |
|---|---:|
| `const int tab = ...` | 91.199559 |
| `Array<ListItem *> *const items` | 91.199559 |
| `ModStation *const station` | 91.199559 |
| `int i` loop counter | 91.199559 |
| `(unsigned int) items->size()` cast | 91.199559 |
| `for (unsigned int i = 0; ...)` inline decl | 91.144470 |

## Conclusion

The landed source is a strong local optimum. This wave added (a) a direct
demonstration that the `pad*` anchors work through the *decimal* stack/pool
offsets that `verify.normalize()` does not mask, (b) an exhaustive
dead-declaration scan of the buy/free-credits region, and (c) anchor-form and
scope-placement sweeps around the selected-item amount block and the ship
branch. Nothing beat 91.199559.

The residual is not a source-shape error: it is the global callee-saved
register assignment (`&Globals::layout` wants `sl`; ours puts it in `r9` in the
blueprint block and `r8` in the buy-credits block), the `item`
rematerialisation in the amount block, and the branch-folder's `cbz` ->
`cmp/beq.w` fold on the ship branch. All three are allocator/CFG heuristics
that a local source edit cannot reach without reallocating the 2962-byte
identical middle of the function. Every attempt to force the faithful source
order (e.g. free-credits `getWidth` before `getHeight`) reallocates the whole
function and costs >= 13 points, so the tuned shape is kept.

## Commands

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j8
cmake --build cmake-build-match --target check -j6   # RATCHET PASS byte 4505 linked 4505
# per-variant fuzzy: rebuild Elf32/Resolver each run, like tools/verify/verify.py
```
