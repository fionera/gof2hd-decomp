# MenuTouchWindow::OnTouchBegin — wave 112 (w6017)

Status: research pass, 2026-11-05. Worktree `wt/w6017`, branch `wave112-w6017`,
base = current master (`5d98bd391`). **No `src/` change landed**: the baseline
is still the best measured shape. This note extends
`menutouchwindow-ontouchbegin-mgame-onupdate-wave110-w5820.md` (which extends
wave 109/106) with a new batch of measured source shapes and a re-derivation
of the residual from the 7.0.2 machine IR.

Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`; real VMA = Ghidra − 0x10000).

| symbol | size orig/ours | fuzzy | structural diff lines |
|---|---|---|---|
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 1896 / 1896 | **98.1** | 18 |

Gate before and after (pristine source): `RATCHET PASS`, byte_exact 4500 /
linked_exact 4500 / size 4524 / data 843 / imports 207 / lint `CLEAN ✓` /
drift 0. Structural state unchanged: 27 sections, file size 2,187,328 B,
`.ARM.extab` 4 B short.

## 1. The residual, re-derived

The 18 structural lines are two independent groups:

* **case 3/8 (2 lines)**: `ldr.w ip,[sl,#340]` / `add r3,ip` in the original
  vs `ldr.w r4,[sl,#340]` / `add r3,r4` in ours. `this->listEntryWidth` is
  short-lived; `r4` is dead in both binaries at that instruction. Pure greedy
  tie-break.
* **case 15 (16 lines)**: the scrollbar block. The original has

  ```
  block 1:  ldr r2,[r4,#0]      ; Globals::layout  (r4 = [sp,#16])
            ldr r2,[r2,#40]     ; buttonInsetX
  block 2:  ldr r2,[sp,#16]     ; &Globals::layout (rematerialised)
            ldr r2,[r2,#0]      ; Globals::layout
            ldr r5,[r2,#12]     ; leftMargin
            ldr r6,[r2,#32]     ; top
  block 3:  ldr r0,[sp,#16]     ; &Globals::layout (rematerialised)
            ldr r2,[r0,#0]      ; Globals::layout
            ldr r5,[r2,#40]     ; buttonInsetX
  ```

  with `fp = &Globals::Canvas` throughout. Ours (baseline, `Layout &layout =
  *Globals::layout;` used for block-1 inset and block-2 top):

  ```
  block 1:  ldr r6,[r4,#0]      ; Globals::layout cached in r6
            ldr r5,[r6,#40]     ; buttonInsetX
  block 2:  ldr r2,[sp,#16]     ; &Globals::layout
            ldr r4,[r6,#32]     ; top  <- cached r6, not fresh r2
            ldr r2,[r2,#0]      ; Globals::layout
            ldr r5,[r2,#12]     ; leftMargin
  block 3:  ldr r0,[sp,#16]     ; &Globals::layout
            ldr r2,[r0,#0]      ; Globals::layout
            ldr r4,[r2,#40]     ; buttonInsetX (r4 vs orig r5)
  ```

  So the baseline already has `fp = &Globals::Canvas` and rematerialises
  `&Globals::layout` from `sp+16` exactly like the original. The only
  semantic difference is that the baseline caches the *value*
  `Globals::layout` in `r6` across the `GetImage2DWidth` call and uses it for
  `top`; the original reloads the value from the rematerialised address.

## 2. Why the two properties cannot both be had

The all-direct source (the original's pre-RA IR)

```cpp
if (x > Globals::w - Globals::layout->buttonInsetX - Canvas->GetImage2DWidth(...) &&
    y < Globals::layout->field_0x20_top + (Globals::layout->field_0xc_leftMargin + Canvas->GetImage2DHeight(...)))
```

gives the correct fresh value loads (1892 B, 90.3 %) but greedy keeps
`&Globals::layout` in `fp` (the address is a single spilled vreg reloaded once
in the scrollSlots-loop preheader and reused in blocks 1/2/3). The baseline's
`Layout &layout` local adds a live-across-call value, which raises the
register pressure enough that greedy rematerialises the address from `sp+16`
and gives `fp` to `&Globals::Canvas` — but the cached value is then used for
`top`. The two effects are coupled: the cache is what creates the pressure.

MIR evidence (all-direct, `-mllvm -print-after-all`):

```
bb.37 preheader:  %508:gpr = t2LDRi12 %stack.0, 0   ; &Globals::layout
bb.42:            %301:gpr = t2LDRi12 %508:gpr, 0   ; Globals::layout
                  %304:gpr = t2LDRi12 %299:tgpr, 0  ; &Globals::Canvas
...
VRR:              %508 -> $r11 (fp)   %304 -> $r5
```

The original has `%508` rematerialised per block (r4/r2/r0) and `%304` in
`fp`. The pre-RA IR is identical; only the greedy split/assignment differs.

## 3. Measured shapes this wave

`H = Globals::Canvas->GetImage2DHeight(this->heapBufB[this->field_0x1e0])`;
`Td/Tc` = direct/cached `field_0x20_top`; `Ld/Lc` = direct/cached
`field_0xc_leftMargin`; `Id/Ic` = direct/cached `buttonInsetX`.

| variant | size | fuzzy |
|---|---:|---:|
| **baseline `Ic + T c + (Ld + H)`** | **1896** | **98.1** |
| all-direct `Id + Td + (Ld + H)` | 1892 | 90.3 |
| all-direct left-assoc / left-first / paren-dot | 1892 | 90.3 |
| all-direct + `int w = Globals::w` | 1892 | 90.3 |
| all-direct + `PaintCanvas *canvas` local | 1892 | 90.5 |
| all-direct + `Layout *layout` / `Layout *const` | 1892 | 90.9 |
| all-direct + `Layout **layoutPtr = &Globals::layout` | 1892 | 90.3 |
| `Layout *&layout = Globals::layout` (all sites) | 1892 | 90.3 |
| `PaintCanvas *&canvas = Globals::Canvas` | 1892 | 90.3 |
| local used for block-1 inset only | 1892 | 90.3 |
| local used for block-3 inset only | 1888 | 90.1 |
| local used for block-1 + block-3 | 1888 | 90.1 |
| local used for block-1 + block-2 top (baseline) | 1896 | 98.1 |
| local used for block-1 + block-2 leftMargin | 1896 | 98.0 |
| local used for block-2 top only | 1896 | 98.1 |
| local used for block-2 leftMargin only | 1896 | 98.0 |
| local used for block-2 both | 1892 | 90.9 |
| local used for block-2 top + block-3 | 1892 | 90.9 |
| local used for block-2 both + block-3 | 1888 | 90.6 |
| `Layout &layout` + direct top (first block direct) | 1892 | 90.3 |
| `Layout &layout` + direct leftMargin | 1896 | 98.0 |
| `int &inset` / `int &top` / `int &lm` field refs | 1888/1896/1896 | 90.1/98.1/98.0 |
| `volatile Layout &layout` | 1888 | 89.7 |
| `Layout *volatile layout` | 1928 | 76.2 |
| nested `if` / `bool hit` / ternary / bool assign | 1892–1896 | 89.4–90.3 |
| `(layout = Globals::layout)->top` assignment-in-condition | 1892 | 90.3 |
| `xx/yy` parameter copies in the block | 1892 | 90.3 |
| `int width/h` locals for the two image sizes | 1880 | 88.7 |
| `int w` + `int inset` locals | 1884 | 89.9 |
| `w - width - inset` / `w - (inset + width)` / `x + width >` | 1884–1892 | 88.7–89.6 |
| `(top + H) + lm` / `(lm + H) + top` | 1896/1892 | 94.4/90.7 |
| `Layout *layout` declared at case-15 top (early load) | 1892 | 87.5 |
| `PaintCanvas *canvas` declared at case-15 top | 1892 | 87.5 |
| `PaintCanvas *canvas` declared at function top | 1896 | 52.4 |
| `Layout *layout` declared at function top | 1908 | 45.0 |

Case-3/8 tie-break variants (all 1896 B, all still `ip` vs `r4`):
condition reorders (`swap-y`, `order-1243`), `const/unsigned/auto/long
previous`, `int width` + assignment-in-condition, `swap-or`/`int-or` on the
case-7 bitwise OR, `state` local / casts on the switch, `!= 0` on the two
early `if`s. All canonicalise to the same IR; none moves the tie-break.

## 4. Conclusion

The baseline is a genuine local optimum: it is the unique shape that keeps
`fp = &Globals::Canvas` and rematerialises `&Globals::layout`, at the cost of
the `r6` value cache. The all-direct shape has the original's exact pre-RA
IR but greedy assigns `&Globals::layout` to `fp`, saving 4 B. No C++ spelling
tested in waves 106–112 separates the two decisions without adding an
instruction. The case-3 `ip`/`r4` difference is a pure allocator tie-break.

Before → after: byte 4500 → 4500, linked 4500 → 4500, fuzzy 98.1 → 98.1,
size 1896 B. No `src/` change; only this note.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6017
TMPDIR=/dev/shm/w6017/tmp cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
TMPDIR=/dev/shm/w6017/tmp cmake --build cmake-build-match --target check -j6
```

Per-variant measurement: build `link_so`, then
`tools/verify/verify.py --show _ZN15MenuTouchWindow12OnTouchBeginEiiPv`.
MIR: compile the TU with the `compile_commands.json` command plus
`-mllvm -print-after-all`; the greedy dump is the last
`# Machine code for function _ZN15MenuTouchWindow12OnTouchBeginEiiPv` block
before `Virtual Register Rewriter`.
