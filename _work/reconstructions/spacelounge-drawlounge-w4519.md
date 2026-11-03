# SpaceLounge::drawLounge — faithful clamp/pad2 form and greedy-RA ordering re-confirmed (w4519, wave 95)

Status: research note (2026-11-03). Worktree `wt/w4519`, branch `wave95-w4519`,
base `4a88bd6e`, HEAD `4a88bd6e`. **No `src/` change landed**: every source
shape tested this wave keeps the official score at 95.5 (object oracle 95.3).
The tree is clean.

Gate (`cmake --build cmake-build-match --target check -j8`) before and after:

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`_ZN11SpaceLounge10drawLoungeEv`: official `pct 95.5`, `osize = usize = 1948`,
`byte = linked = false` (from `report.json`). Object-level oracle
(`/dev/shm/w4519/oracle.py`, official compile command, `autojunk=False`) reports
`95.3`.

Prior work: `spacelounge-drawlounge-w4246.md` (newest), `-w4227.md`,
`-w4118.md`, `-w4018.md` (greedy-RA trace, `-stress-regalloc` proof),
`-w3838.md`, `-w3818.md`, `SpaceLounge-OnRender3D.md`, `SpaceLounge-layout.md`.
This note records the new evidence gathered in wave 95 and supersedes nothing.

## 1. Residual diff (current build, official verifier)

704 instructions each side, 1948 bytes each side. The complete structural diff
(`tools/verify/verify.py --show _ZN11SpaceLounge10drawLoungeEv`; `|` = differing
instruction). `0xffeee0e0` = `String::String(char const*, bool)`,
`0xffef38f4` = `Layout::drawBox`, `0xffeee0f8` = `String::~String`,
`0xffeeed88` = `PaintCanvas::SetColor`, `0xffef40e0` = `DrawRectangle`:

```text
  ldr.w	r9, [r6, #148]                   | ldr.w	fp, [r6, #148]
  mov.w	sl, r9, lsl #1                   | mov.w	sl, fp, lsl #1
  vmov	r5, s18                           | vmov	r9, s18
  sub.w	r1, r5, r9                       | sub.w	r1, r9, fp
  add.w	r5, r0, r9                       | add.w	r9, r0, fp
  b.n	0x5ac                              | b.n	0x5b2
  add.w	r1, r9, r5                       | add.w	r1, fp, r9
  itt	gt                                 | itttt	gt
  addgt.w	r1, r6, sl                     | movgt	r1, #0
  subgt	r5, r0, r1                       | subgt.w	r1, r1, fp, lsl #1
  ldr	r1, [pc, #476]                     | subgt	r1, r1, r6
  ldr.w	fp, [r4, #48]                    | addgt.w	r9, r1, r0
  add	r1, pc                             | str.w	r8, [sp, #12]
  add	r0, sp, #56                        | mov	r5, fp
  movs	r2, #0                            | ldr	r1, [pc, #464]
  blx	0xffeee0e0                         | ldr.w	fp, [r4, #48]
  vmov	r2, s16                           | add	r1, pc
  mov	r3, r5                             | add	r0, sp, #56
  add	sl, r6                             | movs	r2, #0
  mov	r1, r9                             | blx	0xffeee0e0
  subs	r6, r3, r1                        | vmov	r2, s16
  str.w	r8, [sp, #12]                    | mov	r1, r5
  sub.w	r5, r2, r9                       | add	sl, r6
  mov	r9, r3                             | sub.w	r6, r9, r1
  mov	r8, r2                             | subs	r5, r2, r5
  strd	sl, fp, [sp]                      | mov	r8, r2
  movs	r1, #2                            | strd	sl, fp, [sp]
  str	r0, [sp, #8]                       | movs	r1, #2
  mov	r0, r4                             | str	r0, [sp, #8]
  mov	r2, r6                             | mov	r0, r4
  mov	r3, r5                             | mov	r2, r6
  blx	0xffef38f4                         | mov	r3, r5
  add	r0, sp, #56                        | blx	0xffef38f4
  blx	0xffeee0f8                         | add	r0, sp, #56
  ldr	r0, [sp, #24]                      | blx	0xffeee0f8
  ldr	r1, [pc, #380]                     | ldr	r0, [sp, #24]
  ldr	r0, [r0, #0]                       | ldr	r1, [pc, #376]
  blx	0xffeeed88                         | ldr	r0, [r0, #0]
  mov	fp, r8                             | blx	0xffeeed88
  ldr.w	r8, [sp, #12]                    | ldr	r0, [sp, #20]
  ldr	r0, [sp, #20]                      | ldr	r1, [r0, #0]
  ldr	r1, [r0, #0]                       | ldr	r0, [sp, #24]
  ldr	r0, [sp, #24]                      | ldr	r1, [r1, #48]
  ldr	r1, [r1, #48]                      | ldr	r0, [r0, #0]
  ldr	r0, [r0, #0]                       | str	r1, [sp, #0]
  str	r1, [sp, #0]                       | mov	r1, r6
  mov	r1, r6                             | mov	r2, r5
  mov	r2, r5                             | mov	r3, sl
  mov	r3, sl                             | blx	0xffef40e0
  blx	0xffef40e0                         | ldr	r0, [sp, #24]
  ldr	r0, [sp, #24]                      | ldr	r0, [r0, #0]
  ldr	r0, [r0, #0]                       | mov.w	r1, #4294967295
  mov.w	r1, #4294967295                  | blx	0xffeeed88
  blx	0xffeeed88                         | ldr	r0, [sp, #16]
  ldr	r0, [sp, #16]                      | ldr	r6, [r0, #0]
  ldr	r6, [r0, #0]                       | ldr	r0, [sp, #24]
  ldr	r0, [sp, #24]                      | ldr	r4, [r0, #0]
  ldr	r4, [r0, #0]                       | ldr	r5, [sp, #12]
  mov	r0, r8                             | mov	r0, r5
  add	r0, fp                             | add	r0, r8
  mov	r0, r8                             | mov	r0, r5
  add	r1, fp                             | add	r1, r8
  b.n	0x756                              | b.n	0x758
  b.n	0x734                              | b.n	0x72a
  b.n	0x756                              | b.n	0x734
  b.n	0x72a                              | b.n	0x758
  b.n	0x758                              | b.n	0x75a
  b.n	0x760                              | b.n	0x762
  b.n	0x756                              | b.n	0x758
  mov	r4, r0                             | b.n	0x758
  add	r0, sp, #44                        | mov	r4, r0
  b.n	0x760                              | add	r0, sp, #44
  mov	r4, r0                             | b.n	0x762
  b.n	0x75e                              | mov	r4, r0
  mov	r4, r0                             | b.n	0x760
  add	r0, sp, #92                        | mov	r4, r0
  blx	0xffeee0f8                         | add	r0, sp, #92
  add	r0, sp, #104                       | blx	0xffeee0f8
  blx	0xffeee0f8                         | add	r0, sp, #104
  mov	r0, r4                             | blx	0xffeee0f8
  blx	0xffeede1c                         | mov	r0, r4
  nop                                    | blx	0xffeede1c
```

The diff is one contiguous region (clamp right arm, join, post-`drawBox`
agent/top restore) plus the tail landing-pad branch offsets that follow from the
2-byte size difference in the clamp/join region.

## 2. Register map

```text
value        original        ours (current)
layout       r4              r4
x            r5 -> r3 -> r9  r9 (whole)
width        r6              r6
agent        r8 (spilled)    r8 (spilled earlier)
top          r2 -> r8        r2 -> r8
pad          r9              fp (r11)
pad*2 (sl)   r10             r10
lineHeight   fp (r11)        fp (r11)
x-pad        r6 -> r2        r6 -> r2
top-pad      r5 -> r3        r5 -> r3
```

`top` and `lineHeight` are already correct in the current build. The only wrong
values are `pad` (fp instead of r9) and `x` (whole in r9 instead of a
`r5 -> r9` split). The clamp arm form (`itt gt; addgt.w r1,r6,sl;
subgt r5,r0,r1` vs the 4-instruction `itttt` form) is a consequence of the same
allocation difference: with pad in fp the compiler must materialise a pad copy in
both clamp arms.

## 3. Original vs ours, key region

Original (ELF `0x170d38`, size 1948; `0x1712b2` = clamp start):

```text
1712b2: vmov r5, s18            ; x = centerX
1712b6: ldr r0, [sp, #20]       ; Globals::layout
1712b8: ldr r4, [r0, #0]        ; layout
1712ba: ldr r0, [r4, #40]       ; buttonInsetX
1712bc: sub.w r1, r5, r9        ; x - pad
1712c0: cmp r1, r0
1712c2: bge.n 1712ca
1712c4: add.w r5, r0, r9        ; x = inset + pad
1712c8: b.n 1712e4
1712ca: ldr r1, [pc, #500]      ; Globals::w
1712d2: subs r0, r1, r0         ; w - inset
1712d4: add.w r1, r9, r5        ; pad + x
1712d8: add r1, r6              ; + width
1712da: cmp r1, r0
1712dc: itt gt
1712de: addgt.w r1, r6, sl      ; width + pad2
1712e2: subgt r5, r0, r1        ; x = (w-inset) - (width+pad2)
1712e4: ldr r1, [pc, #476]
1712e6: ldr.w fp, [r4, #48]     ; lineHeight
...
1712f4: vmov r2, s16            ; top
1712f8: mov r3, r5              ; x -> r3
1712fa: add sl, r6              ; boxW = pad2 + width
1712fc: mov r1, r9              ; pad -> r1
1712fe: subs r6, r3, r1         ; x - pad
171300: str.w r8, [sp, #12]     ; spill agent
171304: sub.w r5, r2, r9        ; top - pad
171308: mov r9, r3              ; x -> r9
17130a: mov r8, r2              ; top -> r8
17130c: strd sl, fp, [sp]       ; drawBox args 4,5
...
171318: mov r3, r5              ; top-pad -> r3
17131a: blx Layout::drawBox
...
17132e: mov fp, r8              ; fp = top
171330: ldr.w r8, [sp, #12]     ; r8 = agent
...
171394: add r0, fp              ; y = field_0x2c0 + top
1713a0: mov r3, r9              ; DrawString x = x
```

Ours, same region:

```text
1712b2: vmov r9, s18            ; x = centerX
...
1712bc: sub.w r1, r9, fp        ; x - pad
1712c4: add.w r9, r0, fp        ; x = inset + pad
1712d4: add.w r1, fp, r9        ; pad + x
1712dc: itttt gt
1712de: movgt r1, #0
1712e0: subgt.w r1, r1, fp, lsl #1
1712e4: subgt r1, r1, r6
1712e6: addgt.w r9, r1, r0
1712ea: str.w r8, [sp, #12]
1712ee: mov r5, fp              ; pad copy
1712f2: ldr.w fp, [r4, #48]
...
171304: mov r1, r5              ; pad
171308: sub.w r6, r9, r1        ; x - pad
17130c: subs r5, r2, r5         ; top - pad
17130e: mov r8, r2              ; top -> r8
...
171394: add r0, r8              ; y = field_0x2c0 + top
1713a0: mov r3, r9              ; DrawString x = x
```

## 4. Ghidra faithful-form evidence

Ghidra (`decompile_function` at image base 0x10000, `0x180d38`) gives the
faithful form with a real `pad2` local:

```c
iVar9  = *(int *)(*piVar1 + 0x94);          /* pad */
...
iVar11 = iVar9 * 2;                          /* pad2, a real local */
fVar24 = (float)VectorSignedToFloat(iVar11, ...);
iVar4  = (int)(fVar28 + (fVar25 - fVar28) * 0.5);   /* centerX */
iVar26 = (int)(fVar27 - fVar24);                    /* top */
...
pLVar14 = (Layout *)*piVar1;
iVar8 = *(int *)(pLVar14 + 0x28);            /* buttonInsetX */
if (iVar4 - iVar9 < iVar8) {
    iVar4 = iVar8 + iVar9;
} else {
    iVar8 = **(int **)(DAT_001814c0 + 0x1812d0) - iVar8;   /* w - inset */
    if (iVar8 < iVar9 + iVar4 + iVar23) {
        iVar4 = iVar8 - (iVar23 + iVar11);  /* (w-inset) - (width + pad2) */
    }
}
uVar17 = *(undefined4 *)(pLVar14 + 0x30);    /* lineHeight */
Layout::drawBox(pLVar14, 2, iVar4 - iVar9, iVar26 - iVar9,
                iVar11 + iVar23, uVar17, String(...));
```

TU witness: `source file list dump/gof2hd_1.1.19_ios.cpp` lines 2776-2805 lists
`SpaceLounge::drawLounge` between `draw()` and `draw3DShip()`; TU membership is
correct.

The faithful source shape (with `int pad2 = pad * 2;` used in `top`, the clamp,
and `boxW`) compiles to the **target's 2-instruction clamp arm**
(`itt gt; addgt.w r1,r6,sl; subgt r5,r0,r1`) but to `pad=fp, x=r9`, so it scores
**93.8** (object oracle) rather than 95.3. The current flat source
(`x = w - inset + (-width - pad - pad)`) lowers to the 4-instruction arm but
keeps `pad=fp, x=r9`; its diff is smaller, hence 95.3. Both are the same
`pad=fp/x=r9` allocation; the flat form is a scoring artifact, not the original
source shape.

## 5. Greedy-RA mechanism (re-confirmed)

Instrumented LLVM 7 (`/tmp/llvm_android_702b/build2`, same revision as NDK r18b;
`RA_TRACE`, `VRW`, `NOHINT`, `NOHINTCO`, `HINTVREG`, `PRIOVREG`, `FORCEVREG`
knobs; scratch only). Raw vreg numbers in the full TU:

```text
vreg   value        size   weight          hint      prio
%1     agent        8800   2.259431e-04    $r0       0xE0000000+8800
%2     pad          6400   2.154168e-04    none      0xA0000000+6400
%44    lineHeight    384   5.978895e-04    none      0xA0000000+384
%45    x-pad         704   6.432504e-04    $r2       0xE0000000+704
%46    top-pad       736   6.251306e-04    $r3       0xE0000000+736
%47    boxW          768   6.080038e-04    $r3       0xE0000000+768
%365   Canvas       9312   2.171786e-04    none      0xA0000000+9312
%381   pad*2        6176   1.079917e-04    %382      0xA0000000+6176
%609   x            4220   2.946139e-04    $r3       0xE0000000+4220
```

Selection order is by `Prio` (priority class first, then size), not by weight;
weight only decides evictions. All hinted (`0xE`) intervals are therefore
selected before all unhinted (`0xA`) ones, regardless of size.

The relevant trace:

```text
select %609 x        w=2.946139e-04 hint=$r3 -> $r5
select %47  boxW     w=6.080038e-04          -> $r6
select %46  top-pad  w=6.251306e-04          -> $r5   (evicts x)
select %609 x        w=2.946139e-04          -> $r9   (r9 still free -> whole)
select %45  x-pad    w=6.432504e-04          -> $r6
select %47  boxW     w=6.080038e-04          -> $r10
select %365 Canvas   w=2.171786e-04          -> $r11  (tryEvict)
select %2   pad      w=2.154168e-04          -> cannot evict
     try %2 cand=$r9  i0=%609(w=2.946139e-04)
     try %2 cand=$r11 i0=%365(w=2.171786e-04)
     ... all other candidates have higher weight ...
select %381 pad*2    w=1.079917e-04          -> $r10
select %44  lineH    w=5.978895e-04          -> $r11  (evicts Canvas)
ENQ %2 pad stage=2 (split)
```

Stage 2 splits pad into `%647` (the load, size 6104, w=1.910772e-04), `%646`
(COPY, w=7.045131e-04) and `%648` (w=1.674879e-03); `%647` is assigned to
`$r11` (fp) because r11 is free by then. That is why our pad ends up in fp.

The target requires pad to be selected **before** x's re-enqueue and to take r9.
Since pad is `0xA` and x is `0xE`, that is only possible if pad also carries a
physical copy hint (`0xE`) **and** the hint names r9 (otherwise pad takes r5,
the first free callee-saved register). Then pad (size 6400) is selected before x
(4220), takes r9, x takes r5, `%46` evicts x from r5, and x's re-enqueue finds
r9 occupied -> x splits `r5 -> r9`, exactly as in the original.

Why pad has no copy hint: `-mllvm -print-machineinstrs` on the full TU (pass
just before RA) shows pad `%2` has **no COPY at all** -- seven ALU/load uses
only:

```text
496B   %2:rgpr = t2LDRi12 %372:gpr, 148      ; pad = layout->field_0x94
688B   %381:rgpr = t2LSLri %2:rgpr, 1         ; pad*2
6832B  %475 = t2SUBrr %609:rgpr, %2:rgpr      ; x - pad   (compare)
6912B  %609 = t2ADDrr %39:rgpr, %2:rgpr       ; inset + pad
7072B  %480 = t2ADDrr %2:rgpr, %609:rgpr      ; pad + x
7200B  %483 = t2SUBrs %482:rgpr, %2:rgpr, 10  ; -(pad<<1)
7568B  %46  = t2SUBrr %389:rgpr, %2:rgpr      ; top - pad
7584B  %45  = t2SUBrr %609:rgpr, %2:rgpr      ; x - pad   (join)
```

`copyHint()` only looks at `mi->isCopy()`, so pad never gets a hint and never
gets the `1u<<30` priority bit nor the `totalWeight *= 1.01F` boost.

## 6. New experiments this wave

### 6.1 Harness bug: `score.py` never substitutes the variant

`/dev/shm/w4519/score.py` (and therefore `diffblock.py`) builds the compile
command from `compile_commands.json` but forgets `cmd[-1] = variant.cpp`; it
recompiles the **unmodified** `src/game/menu/SpaceLounge.cpp` every time. Every
variant therefore scores 95.3. `score2.py` / `scoreenv.py` do substitute the
file and are the correct harness. The w4246 note's numbers (93.8 faithful, 94.6
faith_group, 95.2 flat_neg, ...) were produced with the correct harness and
reproduce with `score2.py`; the bug only affects `score.py`/`diffblock.py`
runs. Example:

```text
score.py  faith_block.cpp  -> 95.3  (WRONG: compiles base source)
score2.py faith_block.cpp  -> 93.8  (correct)
```

### 6.2 Faithful-form sweep (384 variants, official/instrumented clang)

Dimensions: `pad2` decl (`pad*2`, `pad+pad`, `pad<<1`, `2*pad`, none) x `top`
(`(float)(pad+pad)`, `(float)(pad*2)`, `(float)pad2`, `(float)(pad<<1)`) x
clamp right arm (12 forms: flat, `-(width+pad+pad)`, `-(width+pad2)`,
`-width-pad-pad`, `-(inset+width+pad2)`, `+(-(width+pad2))`, reversed operand
order, ...) x condition (`sum > right` vs `right < sum`), with `boxW` and the
`DrawRectangle` width fixed to `pad2 + width`. **Best: 95.3**, always the flat
clamp form. The faithful arm forms top out at 93.8.

### 6.3 Other source shapes (all <= 95.3)

```text
variant                                   pct   size
faithful + pad2 (faith_block)             93.8  1948
faithful inline (width + pad*2)           93.8  1948
Ghidra `else { right; if }` + pad2        89.1  1944
`else { right; if }` + flat               95.2  1948
`else { right; if }` + `right - width - pad - pad`  92.0  1944
`right` local + `else if` (flat)          95.3  1948
`right` local + `else if` (faithful)      93.8  1948
`inset` local + flat                      95.3  1948
`inset` local + faithful                  95.2  1948
`w`+`inset` locals + flat                 95.3  1948
direct `Globals::layout` in clamp         95.3  1948
`lineHeight` before clamp                 91.6  1952
swap centerX/top declaration order        95.3  1948
pad declared after SetColor/centerX/top   92.6  1948
pad2 declared before SetColor             93.8  1948
precompute boxX/boxY                      95.3  1948
precompute boxW                           70.0  1968
lambda-inlined clamp helper               93.8  1948
`String("", pad != pad)` etc.             95.3  1948
`__builtin_expect` removed                91.8  1952
`__builtin_expect(..., 0)`                70.4  1984
```

### 6.4 RA knobs (instrumented LLVM 7)

`PRIOVREG` (new this wave) ORs the `1u<<30` hint bit for a named vreg at
enqueue. `PRIOVREG=pad` gives pad `0xE+6368`, but pad is then selected before x
and takes **r5** (first free callee-saved), so the score drops to 76.4 (base) /
77.1 (faithful). `HINTVREG` alone does not affect the priority bit (the hint is
set after `VirtRegMap` is populated; trace still shows `knownpref=0`).
`FORCEVREG=pad:r9` is a no-op for the final allocation because pad is later
split/spilled (`FINAL %2 -> $noreg`). `NOHINT` on x makes x `0xA`, but then
boxW/x-pad grab r9 first and the score falls to 65.7. `VRW` scaling of pad to
1.40-2.00 gives 95.7 with pad=r9, x=r5, but top=r1 and lineHeight=r8 (wrong);
on the faithful source it gives 93.2/1944. `-stress-regalloc=8..16` on the
faithful source gives 46-71%.

## 7. Conclusion

The blocker is fully characterised:

1. pad is an unhinted `0xA` interval (no COPY anywhere in the MIR), so it is
   selected after every `0xE` interval, by which time x already occupies r9.
2. pad's weight (2.154e-4 flat / 2.078e-4 faithful) is below both x
   (2.946e-4) and Canvas (2.172e-4), so even when forced into r9 it is
   evicted; the 1.01 copy-hint boost is just enough to survive Canvas but not
   agent (2.259e-4).
3. The target needs pad to have a **physical r9 copy hint** (not merely the
   `0xE` priority bit): with only the priority bit pad takes r5. No source shape
   tested this wave creates a COPY involving pad and a physreg.
4. The faithful source (Ghidra's `pad2` form) is semantically right and
   produces the target's 2-instruction clamp arm, but still allocates
   `pad=fp, x=r9` -> 93.8. The current flat source is a scoring artifact at
   95.3: it keeps the same wrong allocation but its 4-instruction arm has a
   smaller diff.

No source shape tested this wave is a strict improvement over the current
95.5/95.3. The tree is unchanged.

## 8. Remaining directions

* A source shape in which `pad` participates in a COPY that coalesces onto it
  and carries a physical hint for r9 (e.g. an inlined helper whose parameter is
  passed as a call argument, or a value the coalescer merges with pad).
* A source shape that raises pad's weight above x (2.946e-4) *and* gives it a
  physical r9 hint without adding instructions. The flat clamp form's extra pad
  use gives only ~3.7%.
* A source shape with one more live value across the clamp (the
  `-stress-regalloc=12` result), e.g. an extra cached global or an additional
  temporary that the original kept alive.
* Cross-check the original's `pad` vreg size/weight by finding a second
  function in the binary that uses the same `field_0x94` padding idiom and whose
  allocation is byte-exact; that would pin the source shape.

## 9. Harness (scratch, `/dev/shm/w4519`, not committed)

```text
oracle.py / score2.py / scoreenv.py   object-level oracle (official compile
                                      command, normalised instruction diff,
                                      autojunk=False); score2 substitutes the
                                      variant source, score.py does not
diff2.py <block> [ENV=V]              score + unified diff (score2)
runenv.py <block> [ENV=V]             compile with instrumented clang, dump
                                      stderr (RA_TRACE etc.) to env.err
bigsweep2.py                          384-variant faithful-form sweep
score.py / diffblock.py               BROKEN substitution (see 6.1)
base_block.cpp                        current source block (95.3)
faith_block.cpp                       faithful pad2 form (93.8)
ghidra_block.cpp                      Ghidra `else { right; if }` form (89.1)
lambda_block2.cpp                     lambda-inlined clamp (93.8)
orig.asm / ours.asm                   full disassembly
verify_new.txt                        official verify side-by-side (95.5)
```

Instrumented LLVM: `/tmp/llvm_android_702b/build2/bin/clang++` (LLVM 7,
scratch; `CalcSpillWeights.cpp` and `RegAllocGreedy.cpp` patched with
`RA_TRACE`/`VRW`/`NOHINT`/`NOHINTCO`/`HINTVREG`/`PRIOVREG`/`FORCEVREG`).
This is tooling only, not part of the repo.
