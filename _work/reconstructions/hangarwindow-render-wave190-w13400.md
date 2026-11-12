# HangarWindow::render — wave 190 (w13400): sibling ship-guard LSDA anatomy

Worktree `wt/w13400`, branch `wave190-w13400`, base master `ca19fd252`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B, 3,734 insns.

## Result

**No size-exact text+extab win found this wave.**  `src/game/ui/HangarWindow.cpp`
is left at the wave-190 base (the w13379 body).  This note records the
negative results and the one concrete mechanism that would unlock the
sibling ship-guard shape (a 55-byte `.text` win currently eaten by `.ARM.extab`).

Entry state (re-measured on this branch):

| metric | value |
|---|---:|
| raw same-address diff | 1,355 |
| instruction-index alignment | 617 |
| function size | 9,700 / 9,700 |
| instructions | 3,730 / 3,734 |
| `.text` section diff | 73,003 |
| `.ARM.extab` section diff | 603 |
| render's `.ARM.extab` record diff | 57 (948 B / 156 rows) |
| `.note.gnu.build-id` | 20 |
| total differing bytes | 73,626 |

Region split (base): prologue 0, ship/blueprint 224, mid 4, buy-list 576,
free-credits 228, nlm 70, after-buylist 201, landing 52.

## Key finding: the sibling ship guard is the faithful shape, but its LSDA shifts

The original's ship guard is a **sibling** pair of `if`s, not nested:

```
149b1e  ldr  r0,[sp,#108]        ; tab
149b20  ldrd r5,r4,[sp,#132]
149b24  cbz  r0,149b96           ; tab == 0 -> jump to the upgrade check (near!)
149b26  cmp  r0,#3
...
149b94  ldr  r0,[sp,#108]        ; reload tab
149b96  cmp  r0,#1               ; if (tab == 1 && upgradeMode)
149b98  bne.w 14a6d0
149b9c  ldrb r0,[r4,#285]
```

The base (nested) reproduces the reload/cmp exactly but the `tab == 0`
target becomes the far continuation `14a6d0`, so clang emits
`cmp r0,#0` + `beq.w` (6 B) instead of `cbz` (2 B) and the `ldrd` is sunk to
`0x149b5c`.

Rebuilding the sibling shape (`if (guard) { body } if (upgradeMode && upgTab == 1) {...}`
with `int upgTab = tab;` before the guard) gives a real `.text` win:

| variant | raw | aligned | size | `.text` | `.ARM.extab` |
|---|---:|---:|---:|---:|---:|
| base (nested) | 1,355 | 617 | 9,700 | 73,003 | 603 |
| sibling (`g1_u3_orig_snap`) | **1,300** | **503** | 9,700 | **72,948** | **1,222** |

The 55-byte `.text` win is a net loss because the render LSDA record diff
jumps 57 -> 676.  Cause (decoded with `extab_lsda_decode.py`):

```
row  orig                          sibling
39   (2508, 150, 9664)             (2508, 118, 9664)
40   (2696, 140, 9562)             (2654, 182, 9514)
```

Row 39's length drops from 150 to 118.  150 needs a 2-byte ULEB, 118 needs
1 byte, so the call-site-table length becomes 926 instead of 927 and **every
byte of the table after row 39 shifts by one**.  That single shift is the
whole 676-byte record diff.

If row 39's length could be kept >= 128 (i.e. the `Status::getShip()` call at
function offset 2654 kept landing pad 9664), the shift would disappear and
the variant would win roughly 48 bytes net.  The row-40 scope is created by
the `lifetime.start` of `upgTab`; declaring it before the guard puts
`getShip()` inside that scope (pad 9514).  Declaring it after the guard is
9,720 B, and using `tab` directly is 6,876 raw.  Hoisting `getShip()` into a
local before the guard explodes `.ARM.extab` to 27,733.  No placement tried
keeps both the 9,700 B size and row 39 >= 128.

## Negative sweep (all size 9,700 unless stated)

| variant | raw | aligned | `.text` | `.ARM.extab` |
|---|---:|---:|---:|---:|
| **base** | **1,355** | **617** | **73,003** | **603** |
| sibling ship guard (`g1_u3_orig_snap`) | 1,300 | 503 | 72,948 | 1,222 |
| sibling + `upgTab` in guard condition / reference / unsigned | 1,300 | 503 | 72,948 | 1,222 |
| sibling + `int upgTab = 0;` before guard, assign inside | 1,300 | 503 | 72,948 | 1,222 |
| sibling + `getShip()` hoisted before guard | 1,514 | 489 | 77,917 | 27,733 |
| sibling, `upgTab` after guard | 7,437 | 834 | 9,720 B | 1,964 |
| sibling, `tab` used directly | 6,876 | 777 | 78,524 | 19,025 |
| `upgTab` before `setVisible` | 1,532 | 639 | 73,180 | 627 |
| `upgTab` after `draw` | 1,356 | 703 | 73,004 | 1,217 |
| guard expect `,0` / on 2nd operand / on `tab!=3` / whole | 6,946-8,090 | 806-2,018 | 9,652-9,716 B | 837-27,915 |
| buy-list call order, all 24 perms of `setPosition/draw/replace/setSplit` | 1,355-9,199 | 616-1,567 | `pdrs` best | 603 |
| buy-list switch -> if/else chain (price-first) | 1,355 | 617 | 73,003 | 603 |
| buy-list switch -> if/else chain (description-first) | 1,355 | 611 | 73,003 | 603 |
| buy-list `x` value form (`mla`) | 3,315 | 627 | 9,696 B | 746 |
| buy-list `x` `(1-col)` reorder (`x_step`) | 1,355 | 617 | 73,003 | 603 |
| buy-list `lay` decl order (5 forms) | 1,357-1,473 | 498-623 | 9,696-9,700 | 603-26,893 |
| buy-list `lay` placement (before/after switch/after y) | 1,355-3,409 | 617-652 | 9,692-9,700 | 603-755 |
| buy-list `String description`/`price` order | 1,357 | 623 | 73,005 | 603 |
| free-credits `canvas`/`textYv` volatile forms | 1,355-8,701 | 607-1,477 | 9,700-9,716 | 603-1,579 |
| free-credits `Globals::Canvas->` direct (no local) | 3,476 | 722 | 9,720 B | 1,464 |
| free-credits `textX` decl order (3 forms) | 1,355 | 617 | 73,003 | 603 |
| after-buylist `rh`/`top`/`gsy` decl order | 1,355 | 617 | 73,003 | 603 |
| after-buylist buttons[17] order (6 perms) | 1,355-8,242 | 588-1,372 | `dpr` best | 603 |
| `String name` forms (`name("")`, `= String("")`, decl swap) | 1,355-8,211 | 617-756 | 9,700-9,708 | 603-1,533 |
| micro (`[count+12]`, `++i`, `row/col` casts, `x` operand swap) | 1,355 | 617 | 73,003 | 603 |
| `__builtin_expect` on `isItem`/`isShip`/`selectedItem`/`isSelectable` | 8,254-9,319 | 1,592-3,247 | 9,496-9,720 | 1,414-28,551 |
| loop-top `hintX` / `insetX` / `rowH` hoists | 9,147-9,238 | 1,980-2,271 | 9,504-9,608 | 1,760-2,184 |
| shared `String emptyStr` for drawBox | 9,164 | 1,925 | 9,648 B | 22,000 |

## Notes for the next pass

* The base is an extremely stable local optimum: call order inside the ship
  guard body (all 24 perms) and inside the upgrade block (all 24 perms) is
  **codegen-neutral** under the sibling form, and the buy-list `pdrs` call
  order is the best of all 24 perms.
* The big remaining cells are structural, not source-order:
  buy-list 576 (the `lay` value form forces an extra `sl` spill:
  `strd sl,r9,[sp,#128]` vs `str r9,[sp,#132]`, plus `ldr.w sl,[sl]` at
  `0x14a960` and the tail `ldr.w sl,[sp,#128]` at `0x14ac64`), and the
  r9/sl role swap at `0x1496da` (`ldrd sl,r9` vs `ldrd r9,sl`).
* Removing `lay` fixes the deref but is 9,696 B (`(1-col)` no longer folds to
  `mla`); the value-form `x` is 9,692 B.  No `x` form recovered 9,700 B.
* The switch block layout differs (orig `0,default,1,2,3,4`; ours
  `0,2,4,1,3,default`).  Source case order does not change the IR order.

## Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN12HangarWindow6renderEv
python3 _work/reconstructions/extab_lsda_decode.py \
        /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
        cmake-build-match/libgof2hdaa.so
cmake --build cmake-build-match --target check -j2
```

Scratch (not committed): `/home/debian/GalaxyOnFire2/scratch/w13400/`
(`HangarWindow.base.cpp`, `sweep.py`, `batchrun.py`, `regions.py`, `vdiff.py`,
`v_*.py`).
