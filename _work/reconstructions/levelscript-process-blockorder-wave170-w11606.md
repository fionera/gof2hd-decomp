# LevelScript::process — wave 170 / w11606: wingman-loop body + outer dispatch

Target `_ZN11LevelScript7processEi` (original `0x137d18`, **57,232 B = 0xdf90**).
Worktree `wt/w11606`, branch `wave170-w11606`, base master `50716a763`. Scratch
`/dev/shm/w11606/`.

Follow-up to `levelscript-process-blockorder-wave169-w11506.md`.

## Baseline (HEAD, tree left clean)

```
process raw diffbytes    53,932
process aligned insns    17,900
orig / ours size         57232 / 57232   (size-exact)
orig / ours insns        20830 / 20722
```

## 1. Correction: the original does NOT share the two wingman-loop bodies

w11506 claimed the second wingman loop branches back into the *first* loop's
body (`bcc 137fb0`).  The actual encoding at `0x138128` is `d3f2` = `bcc.n`
with imm8 `0xf2` (-14), i.e. target `0x13812c - 28 = 0x138110`, the second
loop's **own** body.  The original therefore has two separate, identical
bodies, exactly like the source:

```
first  body 0x137fb0..0x137fc2   cond 0x137fc4..0x137fc8
second body 0x138110..0x138122   cond 0x138124..0x138128
```

Both bodies use `r4`=enemies, `r6`=index, `r5`=wingman, `r8`=stored value
(1 then 0).  There is no tail merge to chase.

## 2. First loop: `__builtin_expect` is the only structural difference — removed

HEAD's first loop carries `__builtin_expect(isWingMan(), 0)`, which lowers to a
condition-at-top loop with an out-of-line `strb` block:

```
HEAD   137fa8 mov.w r8,#1 ; 137fac ldr/cmp/bcs ; body ; cbnz store ; b.n
```

Removing the expect (or using `expect(...,1)`, identical codegen) gives the
original's shape instruction-for-instruction:

```
ours   137fa6 movs r6,#0 ; 137fa8 mov.w r8,#1 ; 137fac b.n cond
       137fae ldr r0,[r4,#4] ... 137fba cbz ; 137fbc strb r8
       137fc0 adds r6,#1 ; 137fc2 ldr/cmp ; 137fc6 bcc
orig   137fa8 movs r6,#0 ; 137faa mov.w r8,#1 ; 137fae b.n cond
       137fb0 ... 137fbc cbz ; 137fbe strb r8 ; 137fc2 adds r6
       137fc4 ldr/cmp ; 137fc8 bcc
```

The reconstructed loop is 34 B, same as the original, but sits **2 B earlier**
because the mission-0/1 body in front of it is 6 B short (the `str.w
sl,[sp,#68]` spill in the tail preamble is +4 B, net -2).  Size stays 57,232 B.

| shape | raw | aligned | size |
|---|---:|---:|---:|
| HEAD | **53,932** | **17,900** | **57,232** |
| first-loop expect removed | 53,947 | 17,896 | 57,232 |

Landed: the loop body now matches the original exactly; raw is +15 only
because the whole tail is shifted -2 B against the original's mission-0 body.

## 3. Outer dispatch: the `else if` vs separate `if`

The original's shared tail ends by **falling through** to the mission-5 check
(`0x138128 bcc 138110` then `0x13812a`), and the `m_bStartSequence == 0` path
jumps to the same `0x13812a`.  So the mission 5/26/50/51 chain is a separate
`if`, not an `else if` of the `if (m_bStartSequence != 0)` block.  Changing our
source to a separate `if` moves mission 5 right after the tail (matching the
original's first body), but the function shrinks to 57,164 B (-68, branch
shortening as the switch guard moves from `0x138786` to `0x13828c`) and raw
rises to 55,251.  Not landed.

Disassembly check of the outer chains (original vs HEAD):

```
orig chain A: mission 1 bne -> 0x1381b2 (81) -> 0x13832c (78) -> 0x138574 (89) -> 0x138c7a (106)
orig chain B: mission 5 bne -> 0x1382dc (26) -> 0x1383e4 (50) -> 0x13876c (51) -> switch guard
```

The compare order equals our source order (81,78,89 and 5,26,50,51).  The
physical body order `5,81,26,78,50,89,51` vs ours `81,5,78,26,89,50,51` is
MachineBlockPlacement interleaving the two chains; the source chain order is
already correct.  Mission 50's condition order was tried both ways
(`messages && mission==50` vs `mission==50 && messages`): the latter is
source-faithful but the compiler still emits the messages test first, and raw
moves by +1 only.

## Conclusion

The wingman-loop body is now source-faithful (expect removed).  The outer
dispatch swaps are not source-order controllable; the remaining 2 B tail shift
comes from the mission-0/1 body and is the next lever.

## Files changed

* `src/game/world/LevelScript.cpp` — drop the first wingman loop's expect.
* `_work/reconstructions/levelscript-process-blockorder-wave170-w11606.md`
  — this note.
