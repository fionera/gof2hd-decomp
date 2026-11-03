# MenuTouchWindow::OnTouchEnd — case-12 polarity and remaining case-11 shift

Symbol: `_ZN15MenuTouchWindow10OnTouchEndEiiPv` (8132 B, 95.1938 -> 95.2283 %).

## Landed: case-12 `getPlayingTime()` is `> 0`, not `< 1`

The old source had the branch inverted:

```cpp
if (Globals::status->getPlayingTime() < 1) { ask52; } else { start; return 0; }
```

The original binary (0x12bad4) is:

```text
subs  r0, #1
sbcs.w r0, r1, #0
blt.w 0x12bdea        ; -> startSupernova / startValkyrie / startGOF2
```

`blt` after `x - 1` means `x < 1` -> start, so the ask-52 path is the
`x > 0` branch and falls through to the loop increment. The faithful shape is
`if (getPlayingTime() > 0) { ask52; ... } else { start; return 0; }`.
Clang 7 canonicalises `< 1` to `<= 0` and emits `negs/sbcs/bge`; `> 0` emits
`subs #1/sbcs/blt`, which is the original.

The difficulty value must also stay in a single expression. A local
`float difficulty` makes clang pick the `negs` form and shifts the whole
function's register allocation (8132 -> 8140 B, 87.8 %). Writing

```cpp
this->fadeValue = i == 0 ? 0.5f : 1.5f;
optionFloat(44) = this->fadeValue;
```

keeps the `subs` form and the original r6=i / r9=options / sl=status /
r8=gameText allocation. Both stores then match the original exactly.

## Remaining root cause: case-11 touch-control preview 2-byte shift

The case-11 `cinematicBtnB` block is 2 B shorter than the original and shifts
every downstream address, which is the source of essentially all remaining
branch-target diffs. Two independent deltas:

1. `optionInt(88)`/`optionInt(84)` are merged into one `strd r1,r6,[r9,#84]`
   (-4 B). The original stores them separately (`str.w r0,[r9,#84]` then
   `str.w r6,[r9,#88]`) because the `Globals::Canvas` load is scheduled
   between the two stores, which blocks LLVM's consecutive-store merge.
   Source order 84-then-88, locals, references, mixed base expressions and
   `read twice` all still merge; no source form tried prevents it.
2. `fireImageB` is spilled to `[sp,#60]` and reloaded into `sl` (+2 B) where
   the original keeps it in `sl`. The extra `str`/`ldr` pair is a register
   allocation consequence of the same block.

Net -2 B. Fixing the merge alone would make ours +2 B; both must be fixed
together.

## Other observed (not landed)

- Case-7 quality text (0x12b5b0): the original loads `choiceWindow` into r5
  before `GameText::getText`, clobbers touchId, and restores it with
  `ldr r5,[sp,#92]` at the loop increment. Ours loads choiceWindow after the
  call into r0 and needs no restore (-4 B). A local `String *text` or a local
  `ChoiceWindow *` does not change the schedule.
- Case-0 cargo text (0x12bf6c) and the case-17 tail are only shifted; their
  instruction content already matches.
