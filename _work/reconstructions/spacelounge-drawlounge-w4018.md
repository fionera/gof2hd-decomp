# SpaceLounge::drawLounge — greedy-RA split/eviction ordering (w4018)

Status: research note (2026-11-03). Worktree `wt/w4018`, branch `wave90-w4018`,
base `360c7467`. No `src/` change landed: every source shape tested keeps the
official score at 95.6 (byte_exact unchanged at 4498).

Baseline (`cmake --build cmake-build-match --target check -j8`):

```text
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
.text 0x1337c8
```

`_ZN11SpaceLounge10drawLoungeEv`: official `pct 95.6`, `osize = usize = 1948`,
`byte = linked = false`. Object-level oracle (`w3692_oracle.py`) reports `95.5`.

## 1. The remaining diff is exactly one block

Comparing the mnemonic streams of the original and our build (746 vs 729
instructions; the extra 17 are `blx` vs `bl` PLT spellings and literal-pool
`.word`s) leaves exactly one non-cosmetic region: the clamp right arm plus the
merge-block copy that follows it. Instruction indices 535..553:

```text
index  original (0x1712dc..)                     ours (0x5a4..)
535    itt      gt                                itttt    gt
536    addgt.w  r1, r6, sl      ; width+pad*2     movgt    r1, #0
537    subgt    r5, r0, r1      ; x = (w-inset)-…  subgt.w  r1, r1, fp, lsl #1
538    ldr      r1, [pc,#476]   ; String("")       subgt    r1, r1, r6
539    ldr.w    fp, [r4,#48]    ; lineHeight      addgt.w  r9, r1, r0
540    add      r1, pc                             str.w    r8, [sp,#12]
541    add      r0, sp, #56                        mov      r5, fp
542    movs     r2, #0                             ldr      r1, [pc,#464]
543    blx      String::String                     ldr.w    fp, [r4,#48]
544    vmov     r2, s16         ; top              add      r1, pc
545    mov      r3, r5          ; copy x           add      r0, sp, #56
546    add      sl, r6          ; pad*2+width      movs     r2, #0
547    mov      r1, r9          ; pad              bl       String::String
548    subs     r6, r3, r1      ; x - pad          vmov     r2, s16
549    str.w    r8, [sp,#12]    ; spill agent      mov      r1, r5
550    sub.w    r5, r2, r9      ; top - pad        add      sl, r6
551    mov      r9, r3          ; x -> r9          sub.w    r6, r9, r1
552    mov      r8, r2          ; top -> r8        subs     r5, r2, r5
553    strd     sl, fp, [sp]    ; args 4,5         mov      r8, r2
```

Both streams are 1948 bytes; the original spends two instructions on the clamp
arm and two on the `x` split (`mov r3,r5` / `mov r9,r3`), ours spends four on
the arm and one on the pad copy (`mov r5,fp`).

## 2. Register maps

```text
value        original   ours
layout       r4         r4
x            r5 -> r9   r9
width        r6         r6
agent/top    r8         r8
pad          r9         r11 (fp)
pad*2 (sl)   r10        r10
lineHeight   r11 (fp)   r11 (fp)
top-pad      r5         r5
x-pad        r6         r6
width+pad*2  r10        r10
```

Only `x` and `pad` differ. Everything else, including all downstream uses
(`DrawRectangle`, both `DrawString`s), is already in the original's registers.
The `mov r5, fp` pad copy is a *consequence* of pad living in `fp` while
`lineHeight` also needs `fp`.

## 3. Why the clamp source shape does not matter

The clamp expression form and the `x` declaration form do not change the RA at
all. Measured this session (official oracle pct, size); all reverted:

| variant | pct | size | x | pad |
|---|---:|---:|---|---|
| base `+(-width-pad-pad)` | 95.5 | 1948 | r9 | r11 |
| faithful `-(width+pad*2)` | 93.8 | 1948 | r9 | r11 |
| `int x; if(centerX…) … else x=centerX;` | 95.5 | 1948 | r9 | r11 |
| mutate `centerX`, then `int x = centerX;` | 95.5 | 1948 | r9 | r11 |
| nested `if`/`else{if}` | 95.5 | 1948 | r9 | r11 |
| clamp as a ternary chain | 95.5 | 1948 | r9 | r11 |
| `int dx = x;` / `x+0` / `(int)x` / `x*1` / `x-0` after clamp | 93.8 | 1948 | r9 | r11 |
| `__builtin_expect` on clamp1/clamp2 (0 or 1, all 9 combos) | 93.8 | 1948 | r9 | r11 |
| `lineHeight` before the clamp + faithful | 95.0 | 1948 | r9 | r5 |
| `int boxW = pad*2+width;` before drawBox + faithful | 71.6 | 1968 | r5 | r9 |
| `String empty("");` named temp | 72.6 | 1988 | r9 | r11 |
| no `__builtin_expect` on `isKnown` + faithful | 93.5 | 1948 | r9 | r11 |
| permutations of `{pad, SetColor, centerX, top}` (12 valid) | 95.5/92.8 | 1948 | r9 | r11 |

The `int boxW` variant is interesting: it *does* produce `x=r5, pad=r9`, but the
extra live value pushes the frame from 0x78 to 0x80 and adds two spills, so it
scores 71.6.

## 4. Instrumented LLVM 7 greedy-RA trace

Tool: LLVM 7.0.2 built from the NDK r18b sources with `RA_TRACE` instrumentation
in `lib/CodeGen/RegAllocGreedy.cpp` / `CalcSpillWeights.cpp` (prints
`ENQ`/`select`/`try`/`TRYASSIGN`/`assign`/`FINAL`). `llc -march=thumb
-mcpu=cortex-a8 -mattr=+neon -O2 -filetype=asm` on the TU IR reproduces the
official build's allocation exactly, so the trace is trustworthy.

Priority encoding (`RegAllocGreedy::enqueue`): `Prio = (1u<<29) + Size`, then
`|= 1u<<31` for every global/local range, then `|= 1u<<30` when the interval has
a known register preference. Tie-break is `~vreg` (lower vreg first).

Relevant intervals (faithful IR; `%2`=pad, `%44`=lineHeight, `%45`=x-pad,
`%46`=top-pad, `%47`=width+pad*2, `%369`=pad*2, `%378`=top, `%573`=x):

```text
ENQ %2   size=6272 prio=2684360832 (0xA0000000+6272)   ; pad, no hint
ENQ %44  size=368  prio=2684354928 (0xA0000000+368)    ; lineHeight
ENQ %45  size=704  prio=3758097088 (0xE0000000+704)    ; x-pad, hinted
ENQ %46  size=736  prio=3758097120 (0xE0000000+736)    ; top-pad, hinted
ENQ %47  size=768  prio=3758097152 (0xE0000000+768)    ; width+pad*2, hinted
ENQ %369 size=6032 prio=2684360592 (0xA0000000+6032)   ; pad*2
ENQ %378 size=3504 prio=2684358064 (0xA0000000+3504)   ; top
ENQ %573 size=4204 prio=3758100588 (0xE0000000+4204)   ; x, hinted
```

So the dequeue order is `x`, `%47`, `%46`, `%45`, …, then `pad`, `pad*2`, `top`,
`lineHeight`.

The eviction chain that produces our (wrong) allocation:

```text
select %573 w=2.956381e-04 hint=0,$r3 knownpref=1
  try %573 cand=$r5 intf=0            -> assign %573 -> $r5
select %47  w=6.080045e-04            -> assign %47  -> $r6
select %46  w=6.251314e-04 hint=0,$r3
  try %46 cand=$r5 intf=1 i0=%573(w=2.956381e-04)
  try %46 cand=$r9 intf=0
  TRYASSIGN2 %46 phys=$r9 cheap=$r5 cost=1   ; r9 costs 1 (high reg), r5 is free
  assign %46 -> $r5                   ; <-- evicts x from r5
ENQ %573 (re-enqueued, same priority)
select %573 ... try %573 cand=$r9 intf=0 -> assign %573 -> $r9
select %45  w=6.432512e-04            -> assign %45  -> $r6 (evicts %47)
select %2   w=2.107677e-04            -> assign %2   -> $r11 (tryEvict)
```

`shouldEvict` for the `%46`/`%573` pair is the plain weight test
(`6.251e-04 > 2.956e-04`), because `%46`'s hint is `$r3`, not `$r5`, so the
`CanSplit && IsHint && !BreaksHint` shortcut does not fire.

## 5. What the original must have done

The original's `mov r3,r5` / `mov r9,r3` is a *split* of `x` at the top-pad
definition: `x` stays in `r5` through the clamp, is copied to a scratch `r3`
for the single `x - pad` use, and the long-lived continuation lands in `r9`
only after `pad` dies at 0x171304. For that to happen, `pad` must already own
`r9` when `x` is re-allocated, i.e. `pad` must be dequeued *before* `top-pad`:

```text
x -> r5                       (hinted, first)
pad -> r9                     (r9 free: x is in r5)
top-pad -> r5                 (evicts x; r9 no longer free)
x re-enqueued -> no free CSR  -> SPLIT (r3 for the local use, r9 after pad dies)
```

That ordering requires `top-pad` to have **no hint bit** (priority
`0xA0000000+736` < pad's `0xA0000000+6272`), while `x` keeps its hint bit.

Clearing `%46`'s hint alone (`NOHINT=2147483694`) does not reproduce it: `%45`
(x-pad, weight 6.43e-4) is then dequeued before pad and evicts `x` from `r5`
instead, and `%47` occupies `r9`. An exhaustive search over all 16 subsets of
`{clear %45 hint, clear %46 hint, clear %47 hint, clear x hint}` × `{pad hint
r9 on/off}` (32 llc runs on the faithful IR) produced **no** run with both
`x=r5` and `pad=r9`; the closest is `{clear %46,%47}` which gives `x=r5,
pad=r10`.

## 6. Proof that the target allocation is reachable

`llc -stress-regalloc=12` (limit the allocatable GPRs to 12) on the *same*
faithful IR produces the original's exact clamp/split structure:

```text
.LBB32_69:                        @ %if.end94
        vldr    s0, [sp, #28]     @ 4-byte Reload
        ldr     r0, .LCPI32_2
        vmov    r5, s0            ; x = r5
        str.w   r8, [sp, #16]
        ldr     r4, [r0]
        ldr     r0, [r4, #40]
        sub.w   r1, r5, r9        ; x - pad, pad = r9
        cmp     r1, r0
        bge     .LBB32_71
        add.w   r5, r0, r9        ; x = inset + pad
        b       .LBB32_72
.LBB32_71:
        ldr     r1, .LCPI32_10
        ldr     r1, [r1]
        subs    r0, r1, r0
        add.w   r1, r9, r5
        add     r1, r6
        cmp     r1, r0
        itt     gt
        addgt.w r1, r6, r10       ; width + pad*2 (pad*2 = r10)
        subgt   r5, r0, r1        ; x = (w-inset) - (width+pad*2)
.LBB32_72:
        ldr     r1, .LCPI32_3
        add     r0, sp, #56
        ldr.w   r8, [r4, #48]     ; lineHeight = r8
        movs    r2, #0
        bl      String::String
        vldr    s0, [sp, #24]     @ 4-byte Reload
        add     r10, r6           ; pad*2 + width
        mov     r1, r9            ; pad
        mov     r3, r5            ; copy x
        vmov    r2, s0            ; top
        subs    r6, r3, r1        ; x - pad
        str     r2, [sp, #28]
        sub.w   r5, r2, r9        ; top - pad
        mov     r9, r3            ; x -> r9
        strd    r10, r8, [sp]
```

Every register matches the original (`x=r5`, `pad=r9`, `top-pad=r5`,
`x-pad=r6`, `pad*2=r10`, split `r3`→`r9`) except `lineHeight`, which takes `r8`
because `top` was forced onto the stack (`centerX` is also reloaded). The
assembled function is 1856 bytes / 54.4 % — the spills themselves cost too much
— but it proves the target colouring is a *natural* greedy-RA outcome at one
notch more register pressure, not an impossible one.

## 7. Conclusion / next steps

The remaining 95.6 → 100 gap is purely greedy-RA colouring: `x` must keep `r5`
and be split at the top-pad definition, and `pad` must take `r9`. That requires
`pad` to be dequeued before `top-pad`, i.e. `top-pad` must lose its `$r3`
copy-hint bit (or `x`/`%45`/`%47` must be reordered) without changing the
instruction stream. No source shape tried so far changes the enqueue order:
the clamp form, `x` declaration form, `lineHeight`/`layout`/`pad` placement,
`String` temp form, `__builtin_expect` weights and statement permutations all
reproduce the same `x=r9, pad=r11` allocation.

The most promising remaining lever is the `$r3` copy hint on `%46` (top-pad).
It is not a real copy in the final MIR (`%46:gprnopc = t2SUBrr %378, %2`), so it
is inherited from the pre-isel `%46 = COPY %468` chain; a source shape that
makes `top - pad` a non-copy (or that changes which virtual register resolves
the hint) would drop the `1u<<30` priority bit and flip the ordering.

## 8. Follow-up experiments (this session)

All measurements below are the object-level oracle (`oracle.py`, official
`pct`); the tree was left unchanged, so the gate stayed at
`byte 4498 linked 4498, 95.6`.

### 8.1 The downstream `agent`/`top` colouring is *also* part of the diff

The earlier note said "everything downstream already matches"; the raw
normalized diff shows that is not quite true.  The original has a two-
instruction pair that our build lacks:

```text
original 0x17132e:  mov     fp, r8          ; top (r8) -> fp
original 0x171330:  ldr.w   r8, [sp, #12]   ; agent  -> r8
ours      0x624:    ldr     r5, [sp, #12]   ; agent  -> r5 (top stays in r8)
```

and consequently `top + field_0x2c0` is `add r0, fp` in the original but
`add r0, r8` in ours, and `agent->isKnown()` is `mov r0, r8` vs `mov r0, r5`.
So the target colouring is global: `agent` must stay in `r8` after the
`drawBox` spill and `top` must move to `fp` (`r11`), not just `x`/`pad`.

### 8.2 `Globals::layout->field_0x94` in the `DrawRectangle` args flips `x`/`pad`

Replacing all three `pad` uses in the `DrawRectangle` call with a direct
`Globals::layout->field_0x94` access (faithful clamp, `drawBox` still uses the
local `pad`) gives the *target* `x`/`pad` colouring:

```text
faith_rect_all: pct 89.3  size 1944  x=r5  pad=r9  faithful arm  no pad copy
```

The reason is that the direct access forces a reload of `field_0x94` after the
`drawBox` call (the callee may mutate the `Layout`), which shortens the local
`pad` interval and changes the global pressure enough for greedy RA to give
`pad = r9`.  The price is the reload plus a different join: `top` goes to
`fp`, `lineHeight` to `r8`, `x - pad`/`top - pad`/`width` are materialised
directly in `r2`/`r3`/`r1` for the call, and the conversation block's
`sl`/`fp` colouring also changes (size 1944, score 89.3).  This is the first
source-level shape found that produces `x=r5, pad=r9` with the faithful
two-instruction clamp arm; it is a useful starting point but the reload is
not present in the original.

### 8.3 Extra live values flip the colouring but cost spills

`int boxW = pad * 2 + width;` placed before `drawBox` and used in the call
(faithful clamp) also gives `x=r5, pad=r9`, but the extra live value raises
the frame and adds spills: `pct 71.6, size 1968`.  Placing `boxW` before the
clamp and using it only in the right arm gives `pct 89.4, size 1940` and does
*not* flip `x`/`pad`.  This confirms the target is a pressure-sensitive
greedy-RA colouring, not a different expression tree.

### 8.4 More shapes that do not move it

Measured this session (all keep `x=r9, pad=r11`, flat 95.5 / faithful 93.8,
size 1948 unless noted):

* `top` as `(float)(pad * 2)`, `(float)pad * 2.0f`, `(int)bottomRight.y - pad - pad`,
  `(float)(2 * pad)` — only the last two change codegen (1968/1956, 81%);
* `pad * 2 + width` as `width + pad * 2`, `2 * pad + width`, `pad + pad + width`;
* `x - pad`/`top - pad` as `-pad + x`/`-pad + top`, `x + (-pad)`/`top + (-pad)`;
* the second clamp condition operand order (`x+pad+width`, `width+x+pad`, …);
* `int drawX = x;` / `int x2 = x;` inserted after the clamp, after
  `lineHeight`, or before `drawBox`, used for the `DrawString` x / `drawBox`
  x / all uses (coalesced away; 95.5 / 93.8);
* a `drawX` copy inserted after `drawBox` or before `DrawRectangle`
  (78.7/78.4 and 74.2/78.0; extra spills).

### 8.5 Remaining lead

The `$r3` copy hint on `%46` (top-pad) is still the only identified mechanism
that makes `top-pad` dequeue before `pad`.  A source shape that keeps the
original instruction stream (local `pad`, faithful arm, cached derived values
for `DrawRectangle`) but makes the top-pad interval local/unhinted — or that
otherwise raises the pressure in `bb.44` the way the `field_0x94` reload does,
without adding the reload — is the remaining route to 100 %.
