# MenuTouchWindow::OnTouchEnd — wave 92 (w4245): the `movs r5,#0` swap, solved

Target: `_ZN15MenuTouchWindow10OnTouchEndEiiPv` (MenuTouchWindow::OnTouchEnd)
Original: 8132 B @ 0x12aabc in `android_2.0.16_libgof2hdaa.so` (image base 0x10000).
Result: **97.9 % -> 99.9 %**, still 8132 B. Gate unchanged (byte_exact 4499/4524,
linked_exact 4499, size-exact 4524, stub_zero 0, missing 0, wrong_type 0, extra 0,
data 843/843, imports 207, lint CLEAN, drift 0).

All addresses below are file addresses in the original `.so`; the function-relative
offset is `addr - 0x12aabc`. Disassembly was produced with
`arm-linux-androideabi-objdump -d --no-show-raw-insn`.

## 0. Correction to the wave-91 note

`menutouchwindow-ontouchend-movs-swap_w4117.md` states the baseline is 98.0 % and
that the residual is five real differences. On the current tree (base `b379d00d`
plus the landed wave-92 commits) the baseline measures **97.9 %**, and the only
real residual is the swapped `movs r5,#0` pair:

* ours has `movs r5,#0` in the shared `clearMessage` tail at 12bdbe and lacks it
  in the save-dialog `!saveDlg` block at 12c00a;
* the original is the reverse (zero at 12c00a, none at 12bdbe).

Everything else in 12bcd0..12bdbe is the resulting uniform -2 address shift.
The `b.n`/`b.w` at 12b6f8 is a byte-range artefact of that shift, not a
block-count artefact (MIR evidence in §4).

## 1. The source shape that fixes it

The save-dialog case (`case 2`, `if (this->messageShowing)`) must be written with
the `!saveDlg` early return carrying its own `messageShowing = 0`, and the
`saveDlg && r == 1` path carrying the pair in the order `messageShowing` then
`saveDialogShowing`:

```cpp
bool saveDlg = this->saveDialogShowing;
int r = this->choiceWindow->OnTouchEnd(x, y);
if (!saveDlg) {
    if (r != 0)
        return 0;
    this->messageShowing = 0;
    return 0;
}
if (r != 1) {
    if (r == 0) {
        this->saveGame(this->selectedRow);
        this->saveDialogShowing = 0;
    }
    return 0;
}
this->messageShowing = 0;
this->saveDialogShowing = 0;
return 0;
```

(The previous source folded both paths into a shared tail after the if/else,
which is why the register allocator put the zero in the shared `clearMessage`
block instead of the `!saveDlg` block.)

This is the only change needed for the save-dialog region. With it, the emitted
blocks are byte-identical to the original:

```
original                                   ours (S28)
12ad52: ldrb.w r5,[fp,#371]               12ad52: ldrb.w r5,[fp,#371]
12ad56: blx ChoiceWindow::OnTouchEnd       12ad56: blx ChoiceWindow::OnTouchEnd
12ad5a: cmp   r5,#0                       12ad5a: cmp   r5,#0
12ad5c: beq.w 12c00a                      12ad5c: beq.w 12c00a
12ad60: movs  r5,#0                       12ad60: movs  r5,#0
12ad62: cmp   r0,#1                       12ad62: cmp   r0,#1
12ad64: beq.w 12c180                      12ad64: beq.w 12c180
12ad68: cmp   r0,#0                       12ad68: cmp   r0,#0
12ad6a: bne.w 12bf78                      12ad6a: bne.w 12bf78
...
12c00a: movs  r5,#0                       12c00a: movs  r5,#0
12c00c: cmp   r0,#0                       12c00c: cmp   r0,#0
12c00e: bne.n 12bf78                      12c00e: bne.n 12bf78
12c010: b.n   12bdc0                      12c010: b.n   12bdc0
...
12c180: strb.w r5,[fp,#371]               12c180: strb.w r5,[fp,#371]
12c184: b.n   12bdc0                      12c184: b.n   12bdc0
```

The `clearMessage` tail is likewise identical in both:

```
12bdbc: strb.w r5,[fp,#376]               12bdbc: strb.w r5,[fp,#376]
12bdc0: strb.w r5,[fp,#368]               12bdc0: strb.w r5,[fp,#368]
12bdc4: b.n   12bf78                      12bdc4: b.n   12bf78
```

The landed patch also keeps the three wave-91/92 shapes that are required for
the rest of the function (they were already in the tree, but are repeated here
because the save-dialog change only makes sense together with them):

* `returnToMenuShowing` r==1: `messageShowing = 0; returnToMenuShowing = 0;
  return 0;` (not `goto clearMessage`).
* `genericConfirmB`: explicit `if (r == 0) { g_android_link_game_gp = 1;
  genericConfirmB = 0; messageShowing = 0; return 0; } else if (r == 1) {
  genericConfirmB = 0; messageShowing = 0; return 0; } return 0;`.
* `loadFailedDialogShowing`: `messageShowing = 0; loadFailedDialogShowing = 0;
  return 0;` (not `goto clearMessage`).

## 2. Why the `b.n` at 12b6f8 is not the problem

The wave-91 note blamed a `b.n` -> `b.w` widening at 12b6f8 for every faithful
shape. That is a consequence, not a cause. In the base and in the fixed variant
the final MIR of the loop-exit branch is the same `t2B %bb.N`; only the byte
distance to the target changes because the save-dialog blocks are 2 B shorter.
Once the save-dialog blocks are right, the branch site/offset is the original's
and the instruction is `b.n` again.

## 3. Remaining diff (99.9 %)

After the save-dialog fix the whole function is instruction-identical except
for one branch target and the literal-pool order it perturbs:

```
12af40: cmp   r0,#0
12af42: beq.w 12bf76    ; original: epilogue trampoline (movs r5,#0; canary check)
12af42: beq.w 12c062    ; ours:     startSupernova trampoline (movs r5,#0; ldr r6; b epilogue)
```

The literal-pool swap is a consequence of the block order:

```
.word 0x000b96da  = pool:pic:str:No link with Google+      (original first)
.word 0x000e365e  = pool:pic:got:g_android_show_leaderboards
```

and the two `ldr [pc,#N]` offsets that decode those words differ by 4 B. No
other instruction differs.

### Why the branch target differs

MIR (`-mllvm -print-after-all`, `Control Flow Optimizer` dump) shows the
loadGame `return 0` block (`bb.167`, `%ir-block.969`) branching to a common
`movs r5,#0` tail:

```
bb.167 (%ir-block.969):        ; loadGame; cmp r0,#0
  t2Bcc %bb.163, 0, ...        ; r == 0 -> return 0
bb.163:                        ; startSupernova trampoline
  predecessors: %bb.162, %bb.495, %bb.451, %bb.242, %bb.237, %bb.187,
                %bb.167, %bb.170, %bb.175
  $r5 = t2MOVi 0
  $r6 = t2LDRi12 $sp, 80      ; canary GOT reload
  t2B %bb.493
bb.492:                        ; epilogue trampoline
  predecessors: %bb.491, %bb.473, %bb.465, %bb.464, %bb.252, %bb.164,
                %bb.176, %bb.292, %bb.294
  $r5 = t2MOVi 0               ; falls through to bb.493
```

The loadGame return has no live `$r6` (`liveins: $r4, $r5, $r11`), so it is
merged with the startSupernova trampoline, which rematerialises the canary
pointer. In the original the loadGame return is merged with the epilogue
trampoline (no `ldr r6`), i.e. `$r6` is live there. This is a tail-merge /
rematerialisation choice (`BranchFolder::CreateCommonTailOnlyBlock` prefers the
layout predecessor, else the lower block number; `bb.163 < bb.492`), not a
source-shape property that was found.

## 4. Variant table (official verifier, autojunk=False)

| variant | save-dialog shape | size | score |
|---|---|---|---|
| base (HEAD) | shared tail after if/else | 8132 | 97.9 |
| V1 | `saveDialogShowing = 0;` + shared tail | 8132 | 98.5 |
| S13 | `!saveDlg` own store, `saveDialogShowing = 0; messageShowing = 0;` | 8132 | 90.6 |
| S19 | `!saveDlg` own store, `saveDialogShowing = 0; goto clearMessage` | 8132 | 90.6 |
| S21 | `else` form of S13 | 8132 | 90.6 |
| S24..S27 | variants of the above | 8132 | 90.6 |
| **S28 (landed)** | **`!saveDlg` own store; r==1 `messageShowing = 0; saveDialogShowing = 0; return 0;`** | **8132** | **99.9** |
| S29 | S28 with `else` instead of early return | 8132 | 99.9 |
| S30 | `if (saveDlg && r == 1)` hoisted | 8140 | 86.3 |
| S31 | S28 r==1 `goto clearMessage` | 8132 | 90.6 |
| S32 | S28 r==1 reversed store order | 8132 | 90.6 |
| S33 | S28 r==1 `goto done` | 8132 | 99.9 |

LoadGame-hint variants on top of S28 (all 8132 B):

| variant | change | score | 12af42 target |
|---|---|---|---|
| L0/S28 | `__builtin_expect(loadGame() == 0, 0)` | 99.9 | 12c062 |
| La/Lh | remove the loadGame `__builtin_expect` | 99.1 | **12bf76** |
| Lb | nested `if (hasChoice()) { if (loadGame()==0) ... }` | 99.1 | 12bf76 |
| Lc | `__builtin_expect(..., 1)` | 99.1 | 12bf76 |
| Ld | `!loadGame()` | 99.1 | 12bf76 |
| Le | local `int lg = loadGame()` | 98.9 | 12bf76 |
| Lg/Li/Lj/Lk/Lq/Lr/Lu/E1 | other expect spellings | 99.9 | 12c062 |
| Lm | `goto done;` | 99.9 | 12c062 |
| Lz | duplicate `messageShowing = 0` in both arms | 99.9 | 12c062 |

Removing the loadGame `__builtin_expect` fixes the branch target but changes the
global block placement (the case-2 `okButton` sound->play/saveGame block moves
before the `messageShowing` check), costing ~0.8 points. Keeping the hint keeps
the layout but leaves the one branch target at 12c062.

## 5. What is proved

* The `movs r5,#0` swap is caused solely by the save-dialog source shape. The
  `!saveDlg` path must carry its own `messageShowing = 0; return 0;`, and the
  `saveDlg && r == 1` path must be `messageShowing = 0; saveDialogShowing = 0;
  return 0;` (in that order). This reproduces the original's 12ad60, 12c00a,
  12c180 and 12bdbc/12bdc0 blocks byte-for-byte.
* The `b.n` at 12b6f8 is a byte-range consequence of the -2 shift, not a
  block-count/tail-merge artefact.
* The remaining diff is exactly one branch target (12af42: 12bf76 vs 12c062)
  plus the literal-pool word order it perturbs.

## 6. What remains

* Make `$r6` live across the loadGame `return 0` so the tail merge picks the
  epilogue trampoline (12bf76) instead of the startSupernova trampoline
  (12c062), without moving the case-2 block layout. The two levers found so far
  are mutually exclusive: the loadGame `__builtin_expect` controls the case-2
  layout, and removing it also changes the tail-merge group.
* Once the branch target is fixed the literal-pool swap should follow
  automatically (the pool is emitted in reference order).
