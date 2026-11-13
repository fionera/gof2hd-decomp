# MenuTouchWindow::OnTouchBegin — wave 192 (w13927)

Date: 2026-11-13. Branch `wave192-w13927`, base master `a665b1036`.
Target: `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` (1,896 B, size-exact).
Gate before/after: byte_exact 4508/4524, linked_exact 4508/4524,
size-exact 4524/4524, stub_zero_size 0, missing 0, wrong_type 0, extra 0,
data_exact 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
`.text` 76,874 and `.ARM.extab` 597 unchanged (no new relocations/extab).

## Change (one line, case 15 scrollbar test)

`src/game/ui/MenuTouchWindow.cpp`, case 15:

```cpp
Layout &layout = *Globals::layout;
if (x > Globals::w - Globals::layout->buttonInsetX - Globals::Canvas->GetImage2DWidth(this->heapBufB[this->field_0x1e0]) &&
    y < layout.field_0x20_top + (Globals::layout->field_0xc_leftMargin + Globals::Canvas->GetImage2DHeight(this->heapBufB[this->field_0x1e0])))
```

The c2 (`y < ...`) expression now reads the **fresh** `Globals::layout->field_0xc_leftMargin`
and the **cached** `layout.field_0x20_top`.  HEAD had the two swapped
(`layout.field_0x20_top` fresh / `layout.field_0xc_leftMargin` cached).

Measured with `tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN15MenuTouchWindow12OnTouchBeginEiiPv`:

| | raw | aligned | size | insn |
|---|---:|---:|---:|---:|
| HEAD | 28 | 16 | 1896/1896 | 754/754 |
| this change | **29** | **15** | 1896/1896 | 754/754 |

Raw rises by 1 but aligned falls by 1 — a net win under the task rule
(aligned primary; raw may rise only if aligned falls).  `check` passes with
all counts unchanged.

## Why this is the right shape (binary evidence)

Original disasm `0x12a1a0..0x12a270` (arm-linux-androideabi-objdump):

```
12a1bc  ldr r4,[sp,#16]        ; &Globals::layout, loaded once before the block
12a1e0  ldr.w r3,[sl,#312]     ; heapBufB
12a1e6  ldr.w r6,[sl,#480]     ; field_0x1e0
12a1f0  ldr r2,[r4,#0]         ; fresh layout value (c1 buttonInsetX)
12a1f6  ldr r5,[r0,#0]         ; Globals::w
12a1fc  ldr r2,[r2,#40]        ; buttonInsetX
12a200  blx GetImage2DWidth
12a20a  ldr.w r0,[sl,#312]     ; c2 heapBufB reload
12a20e  ldr.w r1,[sl,#480]
12a212  ldr r2,[sp,#16]        ; rematerialised &Globals::layout
12a218  ldr r2,[r2,#0]         ; fresh layout value AGAIN for c2
12a21e  ldr r5,[r2,#12]        ; leftMargin
12a220  ldr r6,[r2,#32]        ; top
12a222  blx GetImage2DHeight
12a226  adds r1,r6,r5
12a23c  ldr r0,[sp,#16]        ; rematerialised again for c3
12a246  ldr r5,[r2,#40]        ; buttonInsetX
```

The original keeps neither the layout value nor the address live across the
first `GetImage2DWidth` call; it rematerialises `[sp,#16]` and reloads
`[r2,#0]` for c2.  The landed shape makes the c2 `leftMargin` a fresh load
(one of the two c2 fields), which removes one keep/remat decision from the
greedy allocator and converts the c2 hunk from a cached-`leftMargin` mismatch
into a match.  The remaining 6 aligned hunks are register-allocation
permutations only (A `0x129fd2` `ip` vs `r4`; B `0x12a1e0..0x12a1fe`; C
`0x12a212..0x12a226`; D `0x12a246/0x12a24c`).

## Exhausted search space (do not repeat)

16-way `{c1,c2lm,c2top,c3}` local/direct grid (best HEAD 28/16 `GLGG`, also
`GGLG`, `LGLG`; `LLGG` = 29/15; no both-c2-fresh cell stays 1,896 B),
volatile refs/ptr-refs/ptr-to-ptr, Canvas/texture/`int w` locals,
`__builtin_expect`, nested-if, declaration order, 48-cell
parenthesisation/association sweep, `unsigned` compare, `int i` loops, case-8
condition rewrites, line-layout/blank-line perturbations, per-TU scheduling
flags.  Details: w3819, w4019, pass3..pass7, wave191b w13712/w13739 notes and
`first-divergence-map-wave192.md`.

## Optional follow-up

Try a shape that makes the layout *value* dead after c1 while the *address*
is not live across the call (e.g. a scoped `Layout &layout` used only for
c1's `buttonInsetX`, c2/c3 direct), aiming for 1,896 B with `fp = Canvas`.
One change at a time, forced rebuild, `-j2`, subshell `ulimit`.
