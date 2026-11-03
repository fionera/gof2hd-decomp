# MenuTouchWindow::OnTouchEnd — wave 91 (w4117): the `movs r5,#0` swap

Target: `_ZN15MenuTouchWindow10OnTouchEndEiiPv` (MenuTouchWindow::OnTouchEnd)
Original: 8132 B @ 0x12aabc in `android_2.0.16_libgof2hdaa.so` (image base 0x10000).
Result: **98.0 % -> 98.0 %**, note only. Gate unchanged (byte_exact 4498/4524,
linked_exact 4498, size-exact 4524, stub_zero 0, missing 0, wrong_type 0, extra 0,
data 843/843, imports 207, lint CLEAN, drift 0).

This note records the full residual analysis of the function and the reason every
source shape that reproduces the original's `movs r5,#0` placement loses more than
it gains: it flips the ARM constant-island branch shortening at 12b6f8 from
`b.n` to `b.w`, which shifts every address in 12b6f8..12b9c2 by +2.

All addresses are file addresses in the original `.so`; function-relative offset is
`addr - 0x12aabc`. The original was disassembled with
`arm-linux-androideabi-objdump -d --start-address=0x12aabc --stop-address=0x12cac0`.

## 1. The complete residual (5 real differences)

Diff of the normalised disassembly (ours vs original). Everything else is literal
pool decode noise caused by the -2 shift below.

### (a) 12bcd0 — genericConfirmB r==0 path is missing a rematerialised zero

Original:
```
12bcc2: 2500        movs  r5, #0
12bcc4: 2801        cmp   r0, #1
12bcc6: d008        beq.n 12bcda
12bcc8: 2800        cmp   r0, #0
12bcca: f040 8155   bne.w 12bf78
12bcce: 4856        ldr   r0, [pc, #344]
12bcd0: 2500        movs  r5, #0          <-- missing in ours
12bcd2: 2101        movs  r1, #1
12bcd4: 4478        add   r0, pc
12bcd6: 6800        ldr   r0, [r0, #0]
12bcd8: 6001        str   r1, [r0, #0]    ; g_android_link_game_gp = 1
12bcda: f88b 5170   strb.w r5, [fp, #368]  ; messageShowing = 0
12bcde: f88b 5191   strb.w r5, [fp, #401]  ; genericConfirmB = 0
12bce2: e149        b.n   12bf78
```
Ours is identical except that 12bcd0 is absent (the following instructions are all
at -2). The `movs r5,#0` at 12bcc2 already defines r5, so the original's second
`movs` is a redundant rematerialisation inside the `g_android_link_game_gp = 1`
block.

### (b) 12bdbe — the shared clearMessage tail carries the compensating zero

Original:
```
12bdbc: f88b 5178   strb.w r5, [fp, #376]  ; returnToMenuShowing = 0
12bdc0: f88b 5170   strb.w r5, [fp, #368]  ; messageShowing = 0
12bdc4: e0d8        b.n   12bf78
```
Ours:
```
12bdba: f88b 5178   strb.w r5, [fp, #376]
12bdbe: 2500        movs  r5, #0           <-- extra
12bdc0: f88b 5170   strb.w r5, [fp, #368]
12bdc4: e0d8        b.n   12bf78
```

### (c) 12c00a — save-dialog `!saveDlg` path is missing its zero

Original:
```
12c00a: 2500        movs  r5, #0           <-- missing in ours
12c00c: 2800        cmp   r0, #0
12c00e: d1b3        bne.n 12bf78
12c010: e6d6        b.n   12bdc0
```
Ours:
```
12c00a: 2800        cmp   r0, #0
12c00c: d1b3        bne.n 12bf76          ; -> shared `movs r5,#0` before epilogue
12c00e: e6d6        b.n   12bdbe          ; -> tail that materialises its own 0
```

### (d) 12c1ec — loadFailedDialogShowing has its own tail in the original

Original:
```
12c1ec: 2500        movs  r5, #0
12c1ee: f88b 517e   strb.w r5, [fp, #382]  ; loadFailedDialogShowing = 0
12c1f2: f7ff ba94   b.w   12b71e           ; -> messageShowing = 0; return 0
```
Ours:
```
12c1ec: 2000        movs  r0, #0
12c1ee: 9e14        ldr   r6, [sp, #80]     ; reload canary pointer
12c1f0: f88b 017e   strb.w r0, [fp, #382]
12c1f4: e5e3        b.n   12bdbe           ; -> clearMessage tail
```
The original's 12b71e is the *other* `messageShowing = 0; return 0` tail (also reached
from the case-17 r==1 path and the two supernova paths):
```
12b718: 2500        movs  r5, #0
12b71a: f88b 5175   strb.w r5, [fp, #373]  ; field_0x175 = 0
12b71e: f88b 5170   strb.w r5, [fp, #368]  ; messageShowing = 0
12b722: f000 bc9f   b.w   12c064
12c064: 9e14        ldr   r6, [sp, #80]     ; canary reload trampoline
12c066: e787        b.n   12bf78
```

### (e) 12c192 — one alignment `nop` (a consequence of the -2 shift)

Original ends the block at 12c194 (4-byte aligned) and the literal pool starts there;
ours ends at 12c192 (2 mod 4) and inserts `bf00 nop` at 12c192. This disappears as
soon as (a) is fixed (the +2 realigns the pool).

## 2. Root cause: the clearMessage tail is a *tail-merge* choice

The `movs r5,#0` at 12bdbe is not in the source. It is a register-allocator
rematerialisation inserted in the shared `clearMessage` block because one of its
predecessors (the loadFailed path) does not have r5 == 0. The original instead
routes the loadFailed path to the *other* `messageShowing = 0; return 0` tail at
12b71e, whose predecessors all materialise r5 themselves.

Machine-level evidence (`-mllvm -print-after-all`, "ARM constant island placement
and branch shortening pass" dump):

Base (ours):
```
bb.283 (%ir-block.1530):                  ; loadFailed
  $r0 = t2MOVi 0
  $r6 = t2LDRi12 $sp, 80
  t2STRBi12 $r0, $r11, 382
  t2B %bb.219
bb.219 (%ir-block.2468):                  ; the extra zero block
  predecessors: %bb.218, %bb.283, %bb.278, %bb.243
  $r5 = t2MOVi 0
bb.220 (%ir-block.2471):                  ; clearMessage
  predecessors: %bb.219, %bb.207, %bb.229, %bb.13
  t2STRBi12 $r5, $r11, 368
  t2B %bb.416
```
Faithful shape (loadFailed = `messageShowing = 0; loadFailed = 0; return 0;`):
```
bb.283 (%ir-block.1532):
  $r5 = t2MOVi 0
  t2STRBi12 $r5, $r11, 382
  t2B %bb.142
bb.142 (%ir-block.2472):                  ; the 12b71e tail
  predecessors: %bb.50, %bb.421, %bb.420, %bb.283, %bb.141
  t2STRBi12 $r5, $r11, 368
  t2B %bb.250
```
This is exactly the original's block structure. Once the loadFailed path stops
reaching the clearMessage tail, bb.219 disappears, the clearMessage tail loses its
`movs r5,#0`, and (a) and (c) follow automatically: every remaining predecessor of
the tail already has r5 == 0, so the compiler materialises the zero in the
genericConfirmB gp block (12bcd0) and in the save-dialog block (12c00a) instead.

## 3. The obstacle: `b.n` -> `b.w` at 12b6f8

Every source shape that produces the faithful block structure also flips the
unconditional branch at 12b6f8 from `b.n` (2 B) to `b.w` (4 B):

```
12b6f8: e3f1        b.n   12bede      (original)
12b6f8: f000 bbf1   b.w   12bede      (faithful shapes)
```

The branch is the loop-exit of the main-menu button loop:
```
12b6ec: 3501        adds  r5, #1
12b6ee: f8db 00c0   ldr.w r0, [fp, #192]
12b6f2: 6801        ldr   r1, [r0, #0]
12b6f4: 428d        cmp   r5, r1
12b6f6: d3bb        bcc.n 12b670
12b6f8: e3f1        b.n   12bede
```
The final distance is `12bede - (12b6f8 + 4) = 2018` bytes, well inside the Thumb
`B.N` range. `ARMConstantIslands::optimizeThumb2Branches` uses
`MaxOffs = ((1 << 10) - 1) * 2 = 2046` and `PCAdj = 4`
(`isBBInRange`: `DestOffset - (getOffsetOf(MI) + 4) <= MaxOffs`), so the branch
*should* shrink. It does in the base and does not in every faithful variant.

The widening is self-consistent with the pool alignment: the literal pool is at
12b9c4 in both layouts. In the original, `b.n` (2 B) leaves the code before the pool
ending at 12b9c2, so a 2-byte alignment `nop` is emitted at 12b9c2. In the faithful
shapes, `b.w` (4 B) makes the code end at 12b9c4, so no `nop` is needed. The two
bytes simply move from the `nop` to the branch. The net size is unchanged (8132),
but every address in 12b6fa..12b9c2 shifts by +2, which is why the score collapses:
~40 of ~66 normalised instruction diffs are branch targets / literal-pool words in
that window.

The MIR confirms the decision is made by the constant-island pass, not the
assembler:
```
base:  tB  %bb.306, 14, $noreg
L1:    t2B %bb.305, 14, $noreg
```
Both are `t2B` before the pass; the pass shortens the base and leaves the faithful
shape long. The block layout between bb.179 and the target is identical in the final
image, so the pass is being driven by a stale/renumbered `BBInfo` offset when the
block count changes (the faithful shapes remove one or two blocks by tail merging).
This is a compiler-side artifact, not a property of the source.

## 4. Variant table (official verifier, autojunk=False)

| variant | change | size | score |
|---|---|---|---|
| base | — | 8132 | **98.0** |
| L1 | loadFailed `messageShowing=0; loadFailed=0; return 0;` | 8132 | 95.0 |
| L2 | loadFailed `loadFailed=0; messageShowing=0; return 0;` | 8136 | 93.5 |
| GCB-dup | genericConfirmB duplicated store+return | 8132 | 95.3 |
| GCB-dup + L1 | both faithful shapes | 8136 | 91.0 |
| L1 `false` stores | `messageShowing=false; loadFailed=false;` | 8132 | 95.0 |
| L1 `return messageShowing` | `return this->messageShowing;` | 8132 | 95.0 |
| L1 `return (int)messageShowing` | cast form | 8132 | 95.0 |
| L1 + dead `if (0) {}` | extra empty block | 8132 | 95.0 |
| L1 `messageShowing = loadFailed` | store-to-store | 8136 | 93.5 |
| L1 split if | `if (loadFailed) {...} if (loadFailed) return 0;` | 8128 | 87.1 |
| loadFailed moved after supernova (base form) | | 8128 | 90.2 |
| loadFailed moved after supernova (L1 form) | | 8132 | 88.3 |
| loadFailed `goto clearMessage` + duplicate store | | 8144 | 84.2 |
| loadFailed `messageShowing=0; loadFailed=0; goto clearMessage` | | 8136 | 94.4 |
| genericConfirmB `if (r==0) ... else if (r==1) ...` | | 8128 | 93.3 |
| genericConfirmB `if (r==1) ... if (r==0) ...` | | 8132 | 92.1 |
| genericConfirmB `if/else if/else` chain | | 8136 | 65.7 |
| save-dialog duplicated messageShowing+return | | 8136 | 88.0 |
| clearMessage tail `messageShowing = false` | | 8132 | 98.0 |
| clearMessage tail `return this->messageShowing = 0` | | 8132 | 98.0 |
| genericConfirmB `if (r==0) {goto store}` | | 8132 | 98.0 |
| genericConfirmB `if (r==0) ... else if (r!=1) ...` | | 8132 | 98.0 |
| genericConfirmB `switch (r)` | | 8132 | 98.0 |
| case-17 `field_0x175=0` first + label before messageShowing | | 8160 | 81.0 |

Note: the `if (r==0) ... else if (r!=1) ...` and `switch` forms produce *exactly*
the base code (98.0), i.e. they do not create the duplicated store+return that the
original's 12bcd0 `movs` requires.

## 5. What is proved

* The residual is exactly five real instruction differences; the rest is the -2
  address shift between 12bcd0 and 12bdbe.
* The `movs r5,#0` at 12bdbe is a rematerialisation for the loadFailed predecessor of
  the shared clearMessage tail, not source code.
* The original routes the loadFailed path to the 12b71e tail (`messageShowing = 0;
  b.w 12c064`), which is why the original has no zero in the clearMessage tail and
  has zeros at 12bcd0 and 12c00a instead.
* The faithful source shape is
  `if (this->loadFailedDialogShowing) { this->messageShowing = 0;
  this->loadFailedDialogShowing = 0; return 0; }` (and the duplicated
  genericConfirmB store+return), but both trigger the compiler's `b.n` -> `b.w`
  widening at 12b6f8.
* The widening is a constant-island branch-shortening artifact: the final distance
  is 2018 <= 2046, yet `optimizeThumb2Branches` leaves `t2B` when the block count
  changes. The two bytes move from the 12b9c2 pool-alignment `nop` into the branch,
  so the function size is unchanged.

## 6. What remains

* Find a source shape that yields the original block structure (loadFailed -> 12b71e,
  clearMessage tail with no zero) *and* keeps the block count/order such that
  `optimizeThumb2Branches` still shortens 12b6f8 to `b.n`. The block count is the
  only lever found so far; every shape tried that removes the tail zero also removes
  a block and flips the branch.
* Once the swap is reproduced without the `b.w`, the case-3 `volatile` read anchor
  (`ChoiceWindow *cw = this->choiceWindow;`) can be revisited: removing it currently
  changes the function size (8132 -> 8120) and collapses byte_exact 4498 -> 3613.
