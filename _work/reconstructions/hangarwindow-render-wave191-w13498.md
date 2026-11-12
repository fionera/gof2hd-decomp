# HangarWindow::render — wave 191 (w13498): LSDA pad-split anatomy, no landable win

Worktree `wt/w13498`, branch `wave191-w13498`, base master `f325e2d87`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B, 3,734 insns.

## Result

**No size-exact text+extab net win landed.**  `src/game/ui/HangarWindow.cpp`
is left at the wave-190 base (md5 `7646914356e7cbde83d88b014801b519`).
The one new structural fact is *how* the ship-guard LSDA range is produced:
the row-39/row-40 pad boundary is exactly the `llvm.lifetime.start` of the
`upgTab` local, not the source `if` nesting.  That explains every earlier
row-39 observation and gives a recipe for reproducing the original's
`(2508, 150)` row exactly — but the only forms that reach it grow the guard
by 4 B (`cmp`+`beq.w` instead of `cbz`), which costs more `.text` than the
LSDA gain.

Entry state (re-measured on this branch):

| metric | value |
|---|---:|
| raw same-address diff | **1,038** |
| instruction-index alignment | 620 |
| function size | 9,700 / 9,700 |
| instructions | 3,726 / 3,734 |
| `.text` section diff | 71,811 |
| `.ARM.extab` section diff | 575 |
| build-id | 20 |
| total (text+extab+build-id) | 72,406 |

Render's LSDA record diff is 38 (the other records contribute 537, constant).
Base rows: `39 ours (2508,172)`, `40 ours (2688,148)` vs orig `(2508,150)`,
`(2696,140)`.  Both are >=128, so no ULEB-length shift.

## Key finding: the pad boundary is `upgTab`'s lifetime.start

Decoding the -Oz IR (`clang++ ... -emit-llvm -S`, then
`-mllvm -print-after-all -mllvm -filter-print-funcs=_ZN12HangarWindow6renderEv`)
shows the guard is emitted as:

```
%2367: invoke setVisible(btn1)  unwind %623     ; outer cleanup pad
%2369: lifetime.start(upgTab); store upgTab = tab
%2374: invoke Array::operator[] unwind %2456    ; inner cleanup pad
%2375: invoke draw(btn1)        unwind %2456
...
```

So the call-site-table pad boundary is *the point where `int upgTab = tab;`
is declared*, because clang's lifetime marker splits the EH cleanup scope.
Declaring `upgTab` before the guard puts `getShip()` in the inner pad
(row 39 ends before `getShip`, length 118 -> 1-byte ULEB -> the whole table
shifts by one byte -> `.ARM.extab` diff 575 -> 1,213).  Declaring it at the
start of the guard body reproduces the original's boundary exactly.

Measured variants (all size 9,700 unless stated):

| variant | raw | `.text` | extab | row39 | row40 |
|---|---:|---:|---:|---|---|
| base (`upgTab` after `setVisible`, nested upgrade) | **1,038** | **71,811** | **575** | (2508,172) | (2688,148) |
| faithful sibling, `upgTab` at top | **947** | **71,720** | 1,213 | (2508,118) | (2658,178) |
| `DPV_D` (body D,P,V; decl after D) | **1,028** | **71,801** | 1,208 | (2508,170) | (2710,126) |
| `VPD_V` | 1,036 | 71,809 | 1,208 | (2508,172) | (2712,124) |
| `DVP_D` | 1,039 | 71,812 | 576 | (2508,170) | (2686,150) |
| empty-arm+expect, decl at body start | 1,191 | 71,964 | 584 | **(2508,150)** | (2700,138) |
| plain nested, decl at body start | 1,235 | 72,008 | 585 | (2508,154) | (2700,138) |
| `DPV_before` (decl at body start) | 1,187 | 71,960 | 585 | (2508,150) | (2680,158) |

The faithful sibling shape is the best `.text` form (-91) but its row 39
falls to 118; `DPV_D` is the best raw form (1,028) but its row 40 is 126
(2 bytes short of a 2-byte ULEB).  The decl-at-body-start forms hit
`(2508,150)` exactly but pay 4 B for `cmp r0,#0; beq.w` where the original
has `cbz r0, 149b96` (the original's `tab == 0` edge targets the *near*
upgrade check at +114 B; our compiler proves `tab == 0 => upgTab != 1` and
targets the far continuation at `14a6d0`, which needs the 4-byte branch).

Net totals: base 72,406; faithful 72,953; `DPV_D` 73,029;
decl-at-body-start 72,568.  All worse than base.

## Negative sweeps measured this wave (all size 9,700 unless stated)

* 24 upgrade-block orders x 4 body orders/decl positions: only the table
  above are size-exact; every row-40 >= 128 variant has raw >= 1,134.
* `int upgTab;` / `int upgTab = 0;` / `unsigned` / `int(tab)` / `tab+0` /
  `(upgTab << 0)` at top: all identical to the faithful 947/1,213.
* `volatile` / `*(volatile unsigned*)&upgTab` / reference forms: 9,692-9,708 B.
* dummy body-local (`int dummy = tab;`, used or unused): 9,692 B, raw 6,626.
* goto/switch guard forms: 9,676-9,704 B.
* `static unsigned int upgTab;` (memory, unfoldable): row39 368 but 9,716 B,
  raw 6,803.
* IR confirmation: in the faithful build `%1722` is
  `switch i32 %92, label %1731 [i32 0, label %1962; i32 3, label %1723]`
  and `%1962` is the continuation *after* the upgrade check at `%1768`
  (`br i1 %202, ...`), i.e. SimplifyCFG has already folded the `tab == 0`
  edge past the check.  The original's `cbz` targets the check block
  instead, so the fold is the whole difference.
* buttons[17] `draw/setPosition/replaceTextKeepSize` 6 perms on this base:
  base `DPR` 1,038 best; `PDR` 1,057, `PRD` 1,058, `DRP` 1,222.
* blueprint / free-credits / buy-list cells re-checked: unchanged from the
  wave-190 notes (blueprint `ldrd sl,r9` tie, free-credits `sl` String-temp
  tie, buy-list switch shape).

## Region diff map (base, 32-byte blocks, top contributors)

```
32 0x1ce0  32 0x1c80  32 0x1be0  32 0x1b80  32 0x0ac0   (free-credits / ship)
31 0x21e0  31 0x2180  31 0x2160  31 0x1ca0  31 0x1840   (buttons[17] / buy-list)
30 0x1d80  30 0x1cc0  30 0x1c60  30 0x0a80  30 0x0a60
```

The remaining hot cells are the same ones the wave-190 notes list; no new
tractable single-instruction cell was found.

## Live lead

Reproduce the original's `cbz` at `0x149b24`.  All decl-at-body-start forms
already give the correct row-39 `(2508,150)`; if the `tab == 0` edge could be
made to target the near upgrade check (2-byte `cbz`) instead of the far
continuation, the body would realign at `0x149b44` and the variant would
become a roughly 90-byte `.text` win with an unchanged `.ARM.extab`.  The
blocker is clang proving `tab == 0 => upgTab != 1`; every opaque-value trick
tried (`volatile`, pointers, bit-and, dummy locals) either changes the size
or does not prevent the fold.

## Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN12HangarWindow6renderEv
python3 _work/reconstructions/extab_lsda_decode.py \
        /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
        cmake-build-match/libgof2hdaa.so
```

Scratch (not committed): `/home/debian/GalaxyOnFire2/scratch/w13498/`
(`faithful.py`, `f2.py`..`f20.py`, `side2.py`, `regdiff.py`, `ir/`).
