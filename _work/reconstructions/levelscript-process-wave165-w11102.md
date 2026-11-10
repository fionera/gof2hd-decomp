# LevelScript::process — wave 165 / w11102: mission-type switch shape

Target `_ZN11LevelScript7processEi` (original `0x137d18`, 57,232 B = 0xdf90),
size-exact.  Worktree `wt/w11102`, base `3633505f8` (wave 164).  Scratch
`/dev/shm/w11102/`.

## Baseline (HEAD, unchanged)

```
process raw diffbytes    54,247
process aligned insns    17,532
orig / ours size         57232 / 57232   (size-exact)
.rel.plt                 0        (BYTE-IDENTICAL)
byte_exact 4507 / linked_exact 4507 / size 4524 / data 843 / imports 207
```

## What is already matched (verified this pass)

### Outer campaign-mission switch — physical body order is IDENTICAL

The linear compare chain in the original is
`14,16,21,24,29,40,41,42,56,80,91,92,94,102,105,114,125,154,157,158`
(0x1387e2..0x13885e) and ours is byte-for-byte the same order.

The out-of-line case-body physical order is also identical:

```
original: 158(inline @0x138862), 29,41,80,56,125,92,157,40,102,154,
          24,14,21,42,91,94,105,114,16
ours:     158(inline @0x13884e), 29,41,80,56,125,92,157,40,102,154,
          24,14,21,42,91,94,105,114,16
```

Only the per-body sizes differ (our bodies are smaller), which shifts the
absolute addresses but not the order.

### Range prong 64..73 — entry order AND physical order identical

`subs r1,r0,#64; cmp r1,#9; bls tbh` (ours tbh @0x138c44, orig @0x138c3e).
Table entry order and physical order both `64,69,73,67,70` (rest -> tail).

### Range prong 131..145 — entry order identical, PHYSICAL ORDER DIFFERS

Table entry order is identical (`131,132,133,134,135,136,137,138,139,140,
141,142,143,144,145` with the empty ones -> tail).  Physical body order:

```
original: 131(@0x138aac), 135(@0x1391c0), 145(@0x1392f6),
          144(@0x1393e0), 142(@0x139544), 139(@0x13965c)
ours:     131(@0x138ab2), 135(@0x139316), 144(@0x13944c),
          139(@0x1394c8), 145(@0x1394f8), 142(@0x13959c)
```

i.e. the original places **145 before 144 before 142 before 139**, while we
place **144, 139, 145, 142**.  All six bodies end in an unconditional
`b.w` to the post-switch tail (no fallthroughs), so this is pure
`MachineBlockPlacement`.  Our 131 body is ~336 B larger (2148 vs 1812) and
144/139/142/145 are much smaller (our 139 is split, with most of it outlined
at `0x13ccae`), so the weight/size inputs to MBP differ.  Source case-label
order cannot change it (the IR sorts switch cases by value); it can only be
fixed by making the six bodies faithful.

## The mission-0 state switch (`switch (m_nState)`)

### Original shape (from the binary)

```
0x137e84  mission-0 entry
0x1383fe  cmp r1,#2 ; bgt 0x1389c8          ; if (state <= 2) body inline
0x1389c8  cmp r1,#4 ; beq.w 0x138d2a        ; test 4 first, body4 OUT OF LINE
0x1389ce  cmp r1,#3 ; bne.w 0x138fd8        ; test 3
0x1389d4  <state-3 body>                    ; body3 INLINE, falls to tail
0x138d2a  <state-4 body>
0x138fd8  cmp r1,#5 ; bne.w 0x139b1a        ; test 5
0x138fde  <state-5 body>
0x139b16  ldr r1,[sl,#28]; subs r0,r1,#6; cmp r0,#9; bhi 0x13f906
0x139b22  tbh [pc,r0,lsl#1]                 ; 10 entries, states 6..15
```

So the source order is **4, 3, 5**, and the emitted test order is
**test4, test3, body3 inline, body4 out-of-line, test5, body5 inline**.
State 16 is the switch default (`bhi` -> `cmp r1,#16; bne`).

Original `tbh` (base 0x139b26): physical order
**6, 10, 7, 8, 9, 11, 12, 13, 15, 14** (14 is last, after 15).

### Why our build cannot reproduce it from source order alone

Our current source is **4, 5, 3**.  `SimplifyCFG` folds the trailing
`else if (m_nState == 3)` into the following `switch`, producing a
**13-entry tbh (states 3..15)**; the original's 10-entry table (6..15)
requires state 3 to *not* be adjacent to the switch.  The IR for the
3-arm chain is `switch i32 %state [ i32 4, %b4; i32 3, %b3; i32 5, %b5 ]`
(confirmed with `-S -emit-llvm`), and the ARM small-switch lowering emits
the compare order **3, 5, 4** (first/last/middle of the sorted set), not the
original's 4, 3, 5.  That is why the 4,3,5 source swap does not emit the
original test order.

### Measured shapes on the current (wave-164) base

All full links, `tools/fn_diff.py`, size in bytes.

| shape | raw | aligned | size | tbh | rel.plt |
|---|---:|---:|---:|---|---|
| **HEAD 4,5,3** | **54,247** | 17,532 | **57,232** | 13 (3..15) | **0** |
| 4,3,5 | 55,312 | 18,234 | 57,240 | 10 (6..15) | rotated |
| 3,4,5 | 55,306 | 18,220 | 57,248 | 10 (6..15) | rotated |
| 3,5,4 | 54,680 | **17,427** | **57,232** | 12 (4..15) | rotated |
| 5,3,4 | 55,194 | 19,536 | 57,224 | 12 | rotated |
| 5,4,3 | 55,194 | 19,536 | 57,224 | 12 | rotated |
| 4,3,5 + `expect(state==4,1)` | 55,688 | 18,954 | 57,412 | — | — |
| 4,3,5 + `expect(state==4,0)` | 55,379 | 18,911 | 57,248 | — | — |
| 4,3,5 + `expect(state==5,1)` | 55,273 | 19,606 | 57,256 | — | — |
| 4,3,5 + swapped state-5 `&&` order | 54,894 | 19,021 | 57,248 | 10 | rotated |
| 4,3,5 + nested state-5 (`if (state==5){if(isOver)…}`) | 55,227 | 19,091 | 57,220 | 10* | rotated |
| 4,3,5 + `expect` on body4/body3 inner `isOver` | 55,312 | 18,234 | 57,240 | 10 | rotated |
| state-3 arm nested inside `else { if (state==3) … }` (4,5,3) | 55,350 | 18,857 | 57,256 | 10 | rotated |
| HEAD + `expect(state==3,1)` | 55,531 | 18,816 | 57,472 | — | — |
| HEAD + `expect(state==3,0)` | 55,114 | 18,916 | 57,252 | — | — |
| HEAD + `m_nState - 3 == 0` | 54,247 | 17,532 | 57,232 | 13 | 0 |

\* the nested-state-5 variant merges the 3/4/5 chain into a different
`tbh [r1,r0]` dispatch at 0x138990; it is not the original layout.

Every shape that produces the 10-entry table is a raw regression of at least
+647 B on this base (the wave-164 `__builtin_expect` cells were tuned for the
4,5,3 layout).  The only size-exact, raw-neutral shape is HEAD itself.

## Conclusion / exact case order

* Original mission-0 source order: **`if (state<=2) … else if (state==4) …
  else if (state==3) … else if (state==5 && msg12->isOver()) … else
  switch (state) { 6..15 }`** (i.e. **4, 3, 5**).
* Original mission-0 `tbh`: **10 entries, states 6..15**, physical body order
  **6, 10, 7, 8, 9, 11, 12, 13, 15, 14**; state 16 = default.
* Original outer campaign switch: 20-value linear chain
  `14,16,21,24,29,40,41,42,56,80,91,92,94,102,105,114,125,154,157,158`
  (matched), range prongs 131..145 and 64..73 (entry order matched; the
  131..145 **body** order differs because our bodies are incomplete).
* Best measured shape this pass: **HEAD 4,5,3** — raw 54,247 / aligned
  17,532 / 57,232 B, 13-entry tbh.  The 10-entry-table shapes all regress
  raw, so nothing was landed.

## Reproduce

```sh
python3 /dev/shm/w11102/batch.py head ord:4,3,5 ord:3,5,4
# or: python3 /dev/shm/w11102/gen.py 4,3,5 [nest5]
cmake --build cmake-build-match --target link_so -j8
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
```

`/dev/shm/w11102/LevelScript.cpp.orig` is the pristine HEAD source;
`batch.py` re-applies arm permutations / expect variants to it.

## Next step

The 10-entry table is reachable without a raw loss only if the state-4 body
is outlined (as in the original) while state 3 stays inline.  In our build
the ARM small-switch lowering always emits `test3, test5, test4`, making
body4 the fallthrough.  A faithful state-4 body (or a real cold-path marker
that survives ISel) is needed before the 4,3,5 swap can pay off; the
`__builtin_expect` cells tested here are all ignored by the switch lowering.
