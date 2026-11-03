# MenuTouchWindow::OnTouchBegin (97.7%) + ModStation C1/C2 (98.6%) — w3819

Date: 2026-11-03. Branch `wave88-w3819` (base master 41804126).
Gate before/after: byte_exact 4498 / linked_exact 4498 / size-exact 4524,
`.text` 0x1337c8, lint CLEAN, drift 0. No code change landed; this note records
the source-shape search.

## Current state

| symbol | size orig/ours | fuzzy | structural diff lines |
|---|---|---|---|
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 1896 / 1896 | 97.7% | 27 |
| `_ZN10ModStationC1Ev` | 1008 / 1008 | 98.6% | 16 |
| `_ZN10ModStationC2Ev` | 1008 / 1008 | 98.6% | 16 |

Both functions are size-exact and differ only in scheduling / register
allocation; there is no missing code, wrong constant, wrong field offset or
wrong callee.

## MenuTouchWindow::OnTouchBegin

Only two regions differ (offsets relative to the original VMA 0x129bd4):

1. Case 8 (`upButtonPressed` / `downButtonPressed`), +0x3fe:
   original `ldr.w ip,[sl,#340]` / `add r3,ip`; ours `ldr.w r4,[sl,#340]` /
   `add r3,r4`. Pure register choice (r4 is free in ours, live in the original).
2. Case 15/16 (`scrollbarHit` + drag init), +0x606..+0x65e:
   ours hoists `int top = Globals::layout->field_0x20_top` out of the condition
   and caches `Globals::layout` in r6; the original reloads `Globals::layout`
   after the `GetImage2DWidth` call via `[sp,#16]` (`ldr r2,[sp,#16];
   ldr r2,[r2,#0]`) and loads `field_0xc_leftMargin` then `field_0x20_top`
   before the `GetImage2DHeight` call.

Ghidra (`decompile_function` at Ghidra 0x139bd4) confirms the original source
shape is direct `Globals::layout` access in case 15:

```
iVar10 = *(int *)(*piVar3 + 0x28);          // buttonInsetX
iVar12 = GetImage2DWidth(...);
if (((w - iVar10) - iVar12 < x) &&
    (iVar10 = *(int *)(*piVar3 + 0xc),      // leftMargin
     iVar17 = *(int *)(*piVar3 + 0x20),     // top
     iVar12 = GetImage2DHeight(...),
     y < iVar12 + iVar17 + iVar10)) ...
```

Replacing the local `Layout &layout` / `int top` with direct `Globals::layout`
accesses (the faithful shape) makes the function 4 bytes shorter (1892) because
clang 7.0.2 then keeps `&Globals::layout` in a callee-saved register (`fp`) and
reloads it with one `ldr`, instead of spilling it to `[sp,#16]` and reloading
with two instructions as the original does.  The original's `fp` is
`Globals::Canvas`; the direct variant swaps `fp` to `&Globals::layout` and
uses `r5` for Canvas, changing the whole block.

Variants tried (all measured with `tools/verify/verify.py --show`):
direct global (top+left / left+top / grouped), local `Layout&`, local
`Layout*`, pointer-to-pointer, pointer-reference, `volatile` pointer,
`volatile` pointer-to-pointer, local `PaintCanvas*`, local `int w`,
local `int inset/leftMargin/top`, and combinations.  Results:
- direct global: 1892 B, 88.4–90.9%
- local `Layout&` + hoisted `top` (current): 1896 B, 97.7% (best)
- local `Layout&` + direct second condition: 1896 B, 94.4%
- `volatile Layout**`: 1896 B, 95.7%
- everything else ≤ 97.7%.

The current source is a local optimum; the residual is clang's choice of
callee-saved register for `&Globals::layout` vs `Globals::Canvas`.

## ModStation::ModStation (C1/C2)

The only diff is 16 instruction lines in the camera setup after
`MatrixSetTranslation(nearKey, ...)`, at original VMA 0xd5436..0xd5470.
Original:

```
ldr r0,[pc]        ; &gHangarCamPitch
ldr r3,[pc]        ; &gHangarCamYawIPad
ldr r4,[pc]        ; &gHangarCamYaw
add r0,pc
add r3,pc
ldrb r1,[sl]       ; Globals::iPad
add r4,pc
ldr r2,[r0,r8,lsl#2]   ; pitchBits
mov r0,r3
cmp r1,#0
str r3,[sp,#180]
str r4,[sp,#176]
it eq
moveq r0,r4
ldr r3,[r0,r8,lsl#2]   ; yawBits
movs r0,#2
str r8,[sp,#184]       ; save race
movs r1,#0
add r8,sp,#256
strd r1,r0,[sp]
add r1,sp,#320
mov r0,r8
mov r4,r2              ; save pitchBits
blx MatrixSetRotation
```

Ours is identical except that `str r8,[sp,#184]` and `mov r4,r2` are
scheduled *before* the `yawBits` load instead of after it.  This is a pure
instruction-scheduler difference; the values and call are the same.

The volatile int bit-loads plus `__atomic_signal_fence(__ATOMIC_SEQ_CST)` are
required: plain `float` locals give 86.7%, non-volatile int loads 85.9%.
Variants tried: removing/moving the fence, `ACQ_REL`, fence between reads,
fence after reads, `const volatile int*` table pointers, `volatile` locals,
`reinterpret_cast`, inlining the yaw load into the call, swapping the
pitch/yaw declaration order, swapping the call arguments, `const int` locals,
`register` locals, and a separate `farRace` local.  All either stay at 98.6%
or regress (88.6–97.5%, some shrink to 1000/992 B).  The current shape is the
best measured.

## Root cause

Both functions are source-shape complete: the remaining differences are
clang 7.0.2 `-Oz` scheduling / callee-saved register allocation, not missing
logic.  The build-flag audit (`worst_function_flag_sensitivity_w3751.md`,
`build_flags_independent_audit_w3750.md`) already ruled out per-TU/global
flags, so no flag matrix was re-run.
