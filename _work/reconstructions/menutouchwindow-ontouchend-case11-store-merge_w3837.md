# MenuTouchWindow::OnTouchEnd — case-11 store merge and the +4 B lever

Symbol: `_ZN15MenuTouchWindow10OnTouchEndEiiPv` (8132 B, 95.2283 -> 97.3000 %).

Landed on `wave89-w3837`: the case-11 touch-control preview block is now
byte-for-byte identical (68 insns / 228 B) and the function stays 8132 B, so
`byte_exact`/`linked_exact` stay at 4498/4498 and the fuzzy match rises to
97.3 %. `.text` remains 0x1337c8.

## Root cause (confirmed)

Baseline case-11 block (`cinematicBtnB`) was 69 insns / 230 B. The original is:

```text
str.w r0,[r9,#84]      ; optionInt(84)
ldr.w r0,[r8]          ; Globals::Canvas  (between the two stores)
str.w r6,[r9,#88]      ; optionInt(88)
```

Ours hoisted the `Globals::Canvas` load above both stores, so the ARM
post-RA `arm-ldst-opt` merged them into one `strd r1,r6,[r9,#84]` (-4 B) and
spilled/reloaded `fireImageB` (+2 B). Net -2 B, and every downstream branch
offset shifted.

The original's predicates are `ge ls ge ls` (the `>= 1.0` test is `ge`, the
`<= 0.0` test is `ls`). Our inline ternary emitted the `MOVCCi16` form under
the *outer* predicate instead of the `t2MOVi16` tied-def form, which is what
keeps `$r1`/`$r6` live and blocks the post-RA store merge.

## Fix: extract the ternary into a file-static inline helper

```cpp
static inline int mtw_scaleControlSize(float size, int lo, int mid, int hi) {
    return size <= 0.0f ? lo : (size >= 1.0f ? hi : mid);
}
...
optionInt(84) = mtw_scaleControlSize(controlSize, 415, 583, 830);
optionInt(88) = mtw_scaleControlSize(controlSize, 365, 513, 730);
```

The helper is fully inlined (no new symbol, no new import). Its own
`size <= 0.0f` compare is the *first* compare in the inlined body, so clang
emits the `t2MOVi16` (tied-def, `ls`) form rather than the outer-predicate
`MOVCCi16` form. Both stores then keep `$r1`/`$r6` live and the post-RA
ldst-opt cannot merge them. The block becomes exactly 68 insns / 228 B with
`str.w` x2, `ldr.w r0,[r8]` between them, `fireImageB` in `sl`, and conds
`ge ls ge ls`.

## The 8128 trap and the +4 B lever

The helper alone makes the function 8128 B (4 B short). That is catastrophic
for `byte_exact`: every downstream branch target moves and the count collapses
4498 -> 3613. The missing 4 B are in the case-3 (iPad options) slider-5 text
block, which the original loads as:

```text
ldr.w r5,[fp,#260]     ; this->choiceWindow, before getText
...
ldr r0,[sp,#76]
ldr r0,[r0,#0]
blx   GameText::getText
mov   r1,r0
mov   r0,r5
blx   ChoiceWindow::set
```

With the baseline `String *text = getText(...); this->choiceWindow->set(*text)`
clang loads `choiceWindow` *after* `getText` (2 B shorter). Writing the call
inline, or using a plain `ChoiceWindow *cw = this->choiceWindow;`, lets clang
hoist the load above the `iPadLargePossible` test and also shares the
`gameText` load between the two branches (8120 B). The landed shape pins the
branch-local order with a volatile read:

```cpp
ChoiceWindow *cw = *(ChoiceWindow *volatile *) &this->choiceWindow;
String *text = Globals::gameText->getText(...);
cw->set(*text);
```

This reproduces the original case-3 block exactly (including the loop-tail
`ldr r5,[sp,#92]`) and restores the function to 8132 B. The volatile read is a
single `ldr.w r5,[fp,#260]`; there is no spill.

## Other levers measured (not landed)

- `unsigned short r` for the case-12 leaderboard `r == 1 || r == 0` test
  restores the original two-compare shape (+4 B) but adds `uxth` and changes
  the surrounding schedule: 8140 B / 86.8 % with the helper.
- The case-12 leaderboard block still differs: the original inlines
  `Globals::reportLeaderboards()` as a flag store (`str r1,[r0]`), ours calls
  the out-of-line `Globals::reportLeaderboards()`, and ours folds
  `r == 1 || r == 0` to `bhi`. This is the largest remaining delta.
- Case-17 tail (0x16c8/0x16d6/0x1732/0x1736) is block-order noise only; the
  instruction content already matches.
