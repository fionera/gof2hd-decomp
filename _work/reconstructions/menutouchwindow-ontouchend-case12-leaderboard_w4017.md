# MenuTouchWindow::OnTouchEnd — wave 90 (w4017): case-12 leaderboard + quitConfirm tail

Target: `_ZN15MenuTouchWindow10OnTouchEndEiiPv` (MenuTouchWindow::OnTouchEnd)
Original: 8132 B @ 0x12aabc in `android_2.0.16_libgof2hdaa.so` (image base 0x10000).
Result: **97.3 % -> 98.0 %**, still 8132 B. Gate unchanged (byte_exact 4498/4524,
linked_exact 4498, size-exact 4524, data 843/843, imports 207, lint CLEAN, drift 0).

All addresses below are file addresses in the original `.so`; the function-relative
offset is `addr - 0x12aabc`.

## 1. The guidance's "inline reportLeaderboards" claim is FALSE

`Globals::reportLeaderboards()` is called **out of line** from the case-12 leaderboard
block, not inlined:

```
12c02c: 4865        ldr   r0, [pc, #404]        ; (12c1c4)  -> GOT slot of Globals::globals
12c02e: 4478        add   r0, pc
12c030: 6800        ldr   r0, [r0, #0]
12c032: 6800        ldr   r0, [r0, #0]          ; Globals::globals
12c034: f739 e92a   blx   6528c <_ZN7Globals18reportLeaderboardsEv@plt>
```

The `str r1,[r0,#0]` flag store near the case-12 `genericConfirmB` tail is
`g_android_link_game_gp = 1`, not leaderboard state:

```
12bcce: 4856        ldr   r0, [pc, #344]        ; (12be28) -> GOT slot of g_android_link_game_gp
12bcd0: 2500        movs  r5, #0
12bcd2: 2101        movs  r1, #1
12bcd4: 4478        add   r0, pc
12bcd6: 6800        ldr   r0, [r0, #0]
12bcd8: 6001        str   r1, [r0, #0]          ; *g_android_link_game_gp = 1
```

## 2. The two-compare `r == 1 || r == 0` shape is a source-shape result

Original case-12 leaderboard block:

```
12c012: f89b 0179   ldrb.w r0, [fp, #377]      ; leaderboardDialogShowing
12c016: b1a0        cbz    r0, 12c042
12c018: f8db 0104   ldr.w  r0, [fp, #260]      ; choiceWindow
12c01c: 4621        mov    r1, r4
12c01e: 462a        mov    r2, r5
12c020: f738 eba6   blx    64770 <ChoiceWindow::OnTouchEnd(int,int)@plt>
12c024: 2801        cmp    r0, #1
12c026: bf18        it     ne
12c028: 2800        cmpne  r0, #0
12c02a: d105        bne.n  12c038
12c02c: 4865        ldr    r0, [pc, #404]
12c02e: 4478        add    r0, pc
12c030: 6800        ldr    r0, [r0, #0]
12c032: 6800        ldr    r0, [r0, #0]
12c034: f739 e92a   blx    6528c <Globals::reportLeaderboards()@plt>
12c038: 2000        movs   r0, #0
12c03a: f88b 0179   strb.w r0, [fp, #377]      ; leaderboardDialogShowing = 0
12c03e: f88b 0170   strb.w r0, [fp, #368]      ; messageShowing = 0
```

Two separate `if (r == N) reportLeaderboards();` statements reproduce this exactly
(the compiler folds the pair to `it ne / cmpne / bne`). A single
`if (r == 1 || r == 0)` also works once the surrounding tail blocks are ordered
like the original, but the split form is what is landed.

Our build now emits the identical block (shifted by the residual -2 described in §5):

```
12c022: 2801        cmp    r0, #1
12c024: bf18        it     ne
12c026: 2800        cmpne  r0, #0
12c028: d105        bne.n  12c036
...
12c032: f739 e92c   blx    6528c <Globals::reportLeaderboards()@plt>
```

## 3. The real lever: the quitConfirm r==0 tail must use r5

Original `quitConfirmShowing` block, r == 0 path:

```
12be4e: 2500        movs   r5, #0
12be50: 2801        cmp    r0, #1
12be52: f000 8198   beq.w  12c186
12be56: 2800        cmp    r0, #0
12be58: f040 808e   bne.w  12bf78
12be5c: 48cd        ldr    r0, [pc, #820]      ; Globals::sound
12be5e: 4478        add    r0, pc
12be60: 6805        ldr    r5, [r0, #0]        ; r5 = Globals::sound (clobbers the 0)
12be62: 6828        ldr    r0, [r5, #0]
12be64: f735 ec6c   blx    61740 <FModSound::resumeAll()@plt>
12be68: 6828        ldr    r0, [r5, #0]
12be6a: f73a ec2a   blx    666c0 <FModSound::stopAll()@plt>
12be6e: 48ca        ldr    r0, [pc, #808]
12be70: 2202        movs   r2, #2
12be72: 49ca        ldr    r1, [pc, #808]
12be74: 4478        add    r0, pc
12be76: 4479        add    r1, pc
12be78: 6800        ldr    r0, [r0, #0]
12be7a: 6809        ldr    r1, [r1, #0]
12be7c: 6800        ldr    r0, [r0, #0]
12be7e: 600a        str    r2, [r1, #0]        ; switch_to_target_setting = 2
12be80: 2101        movs   r1, #1
12be82: f735 ef70   blx    61d64 <ApplicationManager::SetCurrentApplicationModule(unsigned)@plt>
12be86: 2500        movs   r5, #0              ; <-- re-materialise 0 in r5
12be88: f88b 5177   strb.w r5, [fp, #375]      ; quitConfirmShowing = 0
12be8c: e798        b.n    12bdc0              ; -> clearMessage tail
```

The original re-materialises the constant in **r5** (the same register the shared
`clearMessage` tail stores) and branches to the shared tail at 12bdc0:

```
12bdc0: f88b 5170   strb.w r5, [fp, #368]      ; clearMessage: messageShowing = 0
12bdc4: e0d8        b.n    12bf78              ; return 0
```

The previous source (`quitConfirmShowing = 0; goto clearMessage;`) made clang pick
`movs r0,#0` for the store, so the compiler could not prove r5 == 0 at the shared
tail and inserted a redundant `movs r5,#0` **in the tail** instead. That extra tail
movs is the size-compensating instruction that keeps the function at 8132 B while
the case-12 `genericConfirmB` `movs r5,#0` is missing — i.e. the two are swapped,
which is what shifts every branch target in between by -2.

Writing the r==0 path as an explicit store pair + `return 0` instead of `goto`:

```cpp
this->messageShowing = 0;
this->quitConfirmShowing = 0;
return 0;
```

makes clang emit exactly the original shape (store `quitConfirmShowing` in r5, then
branch to the shared `clearMessage` tail, which is now `strb r5,[fp,#368]; b.n`):

```
12be86: 2500        movs   r5, #0
12be88: f88b 5177   strb.w r5, [fp, #375]
12be8c: e798        b.n    12bdc0
```

This is the change that moved the score 97.8 -> 98.0.

## 4. Store-order reorders that are required (all semantically order-free)

All of these are `= 0` stores to adjacent `uint8_t` fields, so the order is
semantically irrelevant; they only steer clang's store scheduling:

* `genericConfirmB`: `genericConfirmB = 0; messageShowing = 0;` (original stores
  `#368` then `#401`).
* `quitConfirmShowing` r==1 path: `messageShowing = 0; quitConfirmShowing = 0;
  returnToMenuSubFlag = 0;`.
* case-17 r==1 path: `messageShowing = 0; returnToMenuSubFlag = 0;`.
* `loadFailedDialogShowing`: still `loadFailedDialogShowing = 0; goto clearMessage;`
  (see §5 — the alternative reorder perturbs the block layout).

## 5. Remaining deltas (98.0 %)

The residual is dominated by one swapped pair of `movs r5,#0`:

* **12bcd0** — original has `movs r5,#0` between `ldr r0,[pc]` and `movs r1,#1`
  in the `genericConfirmB` r==0 path; ours does not. This is a re-materialisation
  of the store constant (the return-value 0 is already in r5 from 12bcc2). It can
  be reproduced by duplicating the store block into an `else { ... return 0; }`
  (verified: the block then matches the original byte-for-byte), but that extra
  basic block perturbs MachineBlockPlacement globally and turns the `b.n` at
  12b6f8 into `b.w`, which costs far more than it gains (95.3 %).
* **12bdbe** — ours has the compensating `movs r5,#0` in the `clearMessage` tail
  (see §3). Fixing §3 for the quitConfirm path does not remove it because the
  `loadFailedDialogShowing` path still reaches `clearMessage` with r5 != 0
  (`movs r0,#0; ldr r6; strb r0,[fp,#382]; b.n clearMessage`). The original gives
  loadFailed its own tail instead:

  ```
  12c1ec: 2500        movs   r5, #0
  12c1ee: f88b 517e   strb.w r5, [fp, #382]      ; loadFailedDialogShowing = 0
  12c1f2: f7ff ba94   b.w    12b71e              ; -> messageShowing = 0; return 0
  ```

  Reordering the source to `messageShowing = 0; loadFailedDialogShowing = 0;
  return 0;` reproduces that block exactly, but again perturbs the global block
  layout (jump-table entries shift by +2, score drops to 95.9 %).
* **12c194** — one extra `nop` in ours (literal-pool alignment padding caused by
  the -2 shift above).
* **12ca74** — literal-pool word decode artifact, not real code.

Because of the -2 shift between 12bcd0 and 12bdbe, every intra-function branch
whose target lies in that window compares as `loc_X` vs `loc_X-2`; that accounts
for ~40 of the ~66 differing normalized instructions. The leaderboard block,
quitConfirm block, genericConfirmA/B tails, case-17 tail and the `clearMessage`
tail itself all match instruction-for-instruction modulo that shift.

## 6. Variant table (measured with tools/verify/verify.py, autojunk=False)

| variant | change | size | score |
|---|---|---|---|
| HEAD (base 360c7467) | — | 8132 | 97.3 |
| combo2 | HEAD + genericConfirmB reorder + quitConfirm r==1 reorder + leaderboard split + loadFailed goto + case17 reorder | 8132 | 97.8 |
| **landed (Q)** | **combo2 + quitConfirm r==0 explicit store pair + return** | **8132** | **98.0** |
| Q - case17 reorder | revert case-17 store order | 8132 | 91.5 |
| Q - genericConfirmB reorder | revert genericConfirmB store order | 8132 | 91.4 |
| Q - leaderboard split | single `if (r == 1 \|\| r == 0)` | 8132 | 94.0 |
| Q + loadFailed reorder | `messageShowing` first, own tail | 8132 | 95.0 |
| Q + genericConfirmB `else` dup | extra `movs` at 12bcd0, but `b.w` at 12b6f8 | 8132 | 95.3 |

## 7. What was proved / what remains

Proved:
* `reportLeaderboards` is an out-of-line PLT call; no inlining is needed.
* The `it ne / cmpne r0,#0 / bne` two-compare is produced by splitting the test into
  two `if (r == N)` statements.
* The dominant residual is the swapped `movs r5,#0` pair (12bcd0 vs 12bdbe), caused
  by the `loadFailedDialogShowing` path still sharing the `clearMessage` tail.
* The quitConfirm r==0 path must store in r5 (explicit store pair + `return 0`) so
  the shared tail does not need its own zero materialisation.

Remains:
* Reconstruct a source shape that emits the `genericConfirmB` store-zero at 12bcd0
  **and** keeps the `loadFailedDialogShowing` path on its own tail, without
  perturbing MachineBlockPlacement. Both individual pieces are known
  (see §5); their combination currently shifts the jump table / branch relaxation.
* The case-3 `volatile` pointer read anchor from pass 2 is still required — removing
  it changes the function size.
