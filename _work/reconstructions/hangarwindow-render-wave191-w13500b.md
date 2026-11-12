# HangarWindow::render — wave 191 (w13500b): second lens, no size-exact net win

Worker `w13500b`, branch `wave191b-w13500`, base master `721e5ab06`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B, 3,734 insns.

**Status: no `src/` change landed.**  The committed body is still the best
measured shape.  Gate re-confirmed at HEAD: `RATCHET PASS` — byte_exact
4507/4524, linked_exact 4507/4524, size 4524/4524, data 843/843, imports
207, stub/missing/extra 0, lint CLEAN, drift 0, parity 5370/5370.
`.rel.plt` content 0.

Baseline re-measured exactly:

| metric | value |
|---|---:|
| raw same-address diff | **1,038** |
| instruction-index alignment | 620 |
| function size | 9,700 / 9,700 |
| instructions | 3,726 / 3,734 |
| `.text` section diff | 71,806 |
| `.ARM.extab` section diff | 582 |
| build-id | 20 |
| total (text+extab+build-id) | 72,408 |

Every variant below is a full `link_so` + `tools/fn_diff.py` + section-diff
measurement (`-j2`, `ulimit -v 12000000`).  All variants are size-exact
9,700 B unless noted; none beats raw 1,038 / total 72,408.

## New structural facts established this wave

### 1. The whole free-credits region is downstream of the buy-list `sl` tie

At `0x14a8ec` (just before the `buyCreditsActive` test) the original spills
only `r9` (`&this->buttons`) to `sp+132`:

```
orig  a8ec  str.w r9,[sp,#132]
ours  a8ec  strd  r9,sl,[sp,#128]      ; also spills sl = &Globals::layout
```

The extra `sl` spill exists because our buy-list loop reuses `sl` for the
second `String("")` temporary (`add.w sl,sp,#300` at `0x183e`), while the
original uses `r6` for it (`add r6,sp,#312` / `mov r0,r6`) and keeps
`&Globals::layout` in `sl`.  Consequences, all measured:

* free-credits preheader is 6 B shorter in ours (no `str.w sl,[sp,#128]`;
  layout fields load before `dialog->x/y` instead of after);
* free-credits body: ours reloads `&Globals::layout` from `sp+128`/`sp+132`
  instead of `sl`, keeps `rh` in `sl` and width in `r6` (original: width in
  `sl`, `rh` in `r9`), and spills `textY` to `sp+204` (original keeps it in
  `r6`);
* the `0x1e40` and `0x2372` stack-slot bytes (`sp+132` vs `sp+128`) and the
  `0x0ba8` literal-pool addend follow from the same shift.

Every attempt to flip the buy-list allocation (`Layout *lay` / `Layout &lay`
/ `const Layout *lay` in the loop, `String` decl order, default-ctor
`String`s, `name`/`buttonOffset` order, `int i` loop variable) either ties
or loses size (9,704–9,760), so the committed `nolay` form is still best.

### 2. The `.ARM.extab` section diff is mostly *other* functions

The LSDA record for this function is 948 B at `.ARM.extab` rel `0x1f584`
(vma `0x1c30fc`, personality `0x5eb60`).  Only **38** of its bytes differ
(mostly call-site-table landing-pad VMAs at record offsets `0x263..0x341`,
plus `0x1d`, `0xfd`, `0x102`, `0x104`, `0x106`).  The other 544 bytes of the
582-byte section diff belong to other residual functions.  So a change here
scores `extab = 582 + (record_diff - 38)`; e.g. the ship-body `DPV_u1`
reorder raises the record diff to 671 (`extab` 1,215) and is a net loss.

### 3. The blueprint `ldrd` tie is purely physical-register order

At `0x1496da` both binaries reload the same spill pair
(`[sp+132] = &Globals::layout`, `[sp+136] = this`) but encode
`ldrd sl,r9` (orig) vs `ldrd r9,sl` (ours).  Every later `[sl]`/`[r9,#244]`
access is the same operation with the two registers swapped.  The choice
survives all local rewrites of the blueprint block (`bp_y_swap`,
`bp_bg_cast`, `bp_region_x_first`, `bp_x_local`, `bp_rate_before_if`,
`bp_self`, `bp_lay`, `bp_x_hint_first`, `bp_progress_*`).

## Negative results measured this wave (base raw 1,038)

### Free-credits body / preheader

| variant | raw | size | `.text` | extab |
|---|---:|---:|---:|---:|
| base | **1,038** | 9,700 | 71,806 | 582 |
| `fc_vpd` (setVisible,setPosition,draw,if — faithful order) | 1,072 | 9,700 | 71,840 | 582 |
| `fc_width_first` (width,rh,height — faithful load order) | 1,088 | 9,700 | 71,856 | 583 |
| `fc_no_rh_local` (inline `layout->0x2c` in textX) | 1,088 | 9,700 | 71,856 | 583 |
| `fc_width_height_rh` | 1,088 | 9,700 | 71,856 | 583 |
| `fc_no_textyv` (drop volatile anchor) | 8,771 | 9,744 | 392,470 | 1,704 |
| `fc_vpd_no_textyv` | 8,768 | 9,744 | 392,467 | 1,704 |
| `fc_vpd_width_first_no_textyv` | 8,707 | 9,740 | 392,154 | 1,699 |
| `fc_basex_swap` (`dialog->x + buttonInsetX`) | 1,039 | 9,700 | 71,807 | 582 |
| `fc_basey_swap` / `fc_basey_first` / `fc_row_first` / `fc_f4_swap` | 1,038 | 9,700 | 71,806 | 582 |
| `fc_f4_hoist` (`int f4` before loop) | 2,714 | 9,696 | 117,249 | 715 |
| `tyv_ptr` (`volatile int *`) / `tyv_constref` | 1,038 | 9,700 | 71,806 | 582 |
| `tyv_after_setcolor` | 1,038 | 9,700 | 71,806 | 582 |
| `tyv_after_first` | 8,771 | 9,744 | 392,470 | 1,704 |
| `vis_hybrid` (`visible = i != 4 || !options[76]` + 0..3 chain) | 2,838 | 9,712 | 381,161 | 1,480 |
| `vis_hybrid_noexp` | 2,753 | 9,716 | 381,851 | 1,422 |

The free-credits `visible` chain itself already matches the original
(`movs r6,#0` / `cmp.w fp,#0` / `cbnz` chain); only its address is shifted.

### Ship body (re-measured on this base)

| variant | raw | `.text` | extab | total |
|---|---:|---:|---:|---:|
| base `VDP_u1` | **1,038** | 71,806 | 582 | 72,408 |
| `VPD_u1` | 1,036 | 71,804 | 1,215 | 73,039 |
| `DPV_u1` | 1,028 | 71,796 | 1,215 | 73,031 |

`VPD_u1` is a raw −2 but the LSDA record diff jumps 38 -> 671, so net +631.

### Buy-list switch / loop

| variant | raw | size |
|---|---:|---:|
| `switch (count & 0x7fffffff)` / `switch (count)` / `switch (count+0)` | 3,442 | 9,704 |
| `switch ((unsigned char) count)` / `switch ((char) count)` | 3,409 | 9,704 |
| `String price; String description;` (default ctor) | 3,449 | 9,692 |
| `buylist_decl_swap` (description first) | 1,040 | 9,700 |

### buttons[17] block

| variant | raw | `.text` |
|---|---:|---:|
| base (D,P,R) | **1,038** | 71,806 |
| `rh` declared after `y` / after `x` | 1,038 | 71,806 |
| `P,D,R` | 1,057 | 71,825 |
| `P,R,D` (faithful order) | 1,058 | 71,826 |
| `rh`-after-`y` + `P,R,D` / `P,D,R` | 1,058 / 1,057 | 71,826 / 71,825 |

### Guard / loop / declarations

| variant | raw | size |
|---|---:|---:|
| `if (expect(tab==0,0)) {}` else-if | 8,424 | 9,668 |
| guard without `__builtin_expect` | 4,322 | 9,704 |
| `int i` loop variable | 1,039 | 9,700 |
| `int buttonOffset = 0;` | 1,038 | 9,700 |
| `int buttonOffset; String name;` | 1,038 | 9,700 |

### Pairwise combinations of neutral edits

14 pairs over {`fc_basey_first`, `fc_texty_before_textx`, `fc_row_first`,
`b17_rh_after_y`, `buylist_decl_swap`, `name_after_bo`}: best 1,038
(`basey+txt`, `txt+row`, `basey+rh`, `txt+rh`, `row+rh`, `basey+name`,
`txt+name`, `row+name`, `rh+name`); every pair containing
`buylist_decl_swap` is 1,040.  The earlier 36-run `combos2` sweep over
{`fc_height_first`, `fc_canvas_before_texty`, `fc_canvas_after_height`,
`buylist_decl_swap`, `guard_swap`, `b17_PDR`, `b17_PRD`, `fc_VD`,
`fc_VPD`} likewise bottoms out at 1,040 (`fc_height_first` +
`fc_canvas_before_texty`, total 72,409).

## Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN12HangarWindow6renderEv
python3 /home/debian/GalaxyOnFire2/scratch/w13500/secdiff.py
cmake --build cmake-build-match --target check -j2
```
