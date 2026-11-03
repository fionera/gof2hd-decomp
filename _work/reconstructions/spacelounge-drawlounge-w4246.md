# SpaceLounge::drawLounge — pad copy-hint / priority search (w4246, wave 92)

Status: research note (2026-11-03). Worktree `wt/w4246`, branch `wave92-w4246`,
base `b379d00d`, HEAD `cb47a768`. **No `src/` change landed**: every source shape
and every RA-knob probe tested this wave keeps the official score at 95.5
(object oracle 95.3). The tree is clean.

Gate (`cmake --build cmake-build-match --target check -j8`) before and after:

```text
verify: byte 4499  linked 4499  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`_ZN11SpaceLounge10drawLoungeEv`: official `pct 95.5`, `osize = usize = 1948`,
`byte = linked = false` (from `report.json`). Object-level oracle (`oracle.py`
copy in `/dev/shm/w4246`) reports `95.3`.

Prior work: `spacelounge-drawlounge-w4227.md` (residual-diff enumeration,
weight/priority experiments), `-w4118.md`, `-w4018.md` (greedy-RA trace,
`-stress-regalloc` proof), `-w3838.md`, `-w3818.md`, `SpaceLounge-OnRender3D.md`,
`SpaceLounge-layout.md`. This note records the new evidence gathered in wave 92
and supersedes nothing.

## 1. Residual diff (current build, `autojunk=False`)

704 instructions each side, 1948 bytes each side. The complete diff:

```text
--- orig
+++ ours
@@ -140,12 +140,12 @@
 ldr	r0, [r5, #0]
 mov.w	r1, #4294967295
 str	r5, [sp, #24]
-ldr.w	r9, [r6, #148]
+ldr.w	fp, [r6, #148]
 vldr	s16, [r2]
 vldr	s18, [r2, #4]
 vldr	s20, [r3]
 call	sym:_ZN11AbyssEngine11PaintCanvas8SetColorEj
-mov.w	sl, r9, lsl #1
+mov.w	sl, fp, lsl #1
 vmov.f32	s0, #96
 vmov	s4, sl
 mov	r0, r8
@@ -503,40 +503,41 @@
 add	r0, sp, #68
 call	sym:_ZN11AbyssEngine6StringD1Ev
 add	r6, r4
-vmov	r5, s18
+vmov	r9, s18
 ldr	r0, [sp, #20]
 ldr	r4, [r0, #0]
 ldr	r0, [r4, #40]
-sub.w	r1, r5, r9
+sub.w	r1, r9, fp
 cmp	r1, r0
 bge.n	loc_592
-add.w	r5, r0, r9
-b.n	loc_5ac
+add.w	r9, r0, fp
+b.n	loc_5b2
 ldr	r1, [pc, #500]
 add	r1, pc
 ldr	r1, [r1, #0]
 ldr	r1, [r1, #0]
 subs	r0, r1, r0
-add.w	r1, r9, r5
+add.w	r1, fp, r9
 add	r1, r6
 cmp	r1, r0
-itt	gt
-addgt.w	r1, r6, sl
-subgt	r5, r0, r1
-ldr	r1, [pc, #476]
+itttt	gt
+movgt	r1, #0
+subgt.w	r1, r1, fp, lsl #1
+subgt	r1, r1, r6
+addgt.w	r9, r1, r0
+str.w	r8, [sp, #12]
+mov	r5, fp
+ldr	r1, [pc, #464]
 ldr.w	fp, [r4, #48]
 add	r1, pc
 add	r0, sp, #56
 movs	r2, #0
 call	sym:_ZN11AbyssEngine6StringC1EPKcb
 vmov	r2, s16
-mov	r3, r5
+mov	r1, r5
 add	sl, r6
-mov	r1, r9
-subs	r6, r3, r1
-str.w	r8, [sp, #12]
-sub.w	r5, r2, r9
-mov	r9, r3
+sub.w	r6, r9, r1
+subs	r5, r2, r5
 mov	r8, r2
 strd	sl, fp, [sp]
 movs	r1, #2
@@ -548,11 +549,9 @@
 add	r0, sp, #56
 call	sym:_ZN11AbyssEngine6StringD1Ev
 ldr	r0, [sp, #24]
-ldr	r1, [pc, #380]
-ldr	r0, [r0, #0]
-call	sym:_ZN11AbyssEngine11PaintCanvas8SetColorEj
-mov	fp, r8
-ldr.w	r8, [sp, #12]
+ldr	r1, [pc, #376]
+ldr	r0, [r0, #0]
+call	sym:_ZN11AbyssEngine11PaintCanvas8SetColorEj
 ldr	r0, [sp, #20]
 ldr	r1, [r0, #0]
 ldr	r0, [sp, #24]
@@ -571,7 +570,8 @@
 ldr	r6, [r0, #0]
 ldr	r0, [sp, #24]
 ldr	r4, [r0, #0]
-mov	r0, r8
+ldr	r5, [sp, #12]
+mov	r0, r5
 call	sym:_ZN5Agent7isKnownEv
 cbz	r0, loc_63c
 ldr	r1, [pc, #352]
@@ -593,7 +593,7 @@
 ldr	r0, [r0, #0]
 ldr.w	r0, [r0, #704]
 movs	r1, #0
-add	r0, fp
+add	r0, r8
 add	r2, sp, #80
 strd	r0, r1, [sp]
 mov	r0, r4
@@ -604,7 +604,7 @@
 add	r0, sp, #68
 call	sym:_ZN11AbyssEngine6StringD1Ev
-mov	r0, r8
+mov	r0, r5
 call	sym:_ZN5Agent7isKnownEv
 cbz	r0, loc_6de
 ldr	r0, [sp, #24]
@@ -632,7 +632,7 @@
 ldr	r1, [r1, #0]
 ldr.w	r1, [r1, #704]
```

The tail landing-pad branch offsets (`loc_6ec`/`loc_708`/`loc_756`) are not
shown: they are a consequence of the 2-instruction size difference in the
clamp/join region and disappear once that region matches.

## 2. Register map

```text
value        original        ours (current)
layout       r4              r4
x            r5 -> r3 -> r9  r9
width        r6              r6
agent        r8 (spilled)    r8 (spilled earlier)
top          r2 -> r8        r2 -> r8
pad          r9              fp (r11)
pad*2 (sl)   r10             r10
lineHeight   fp (r11)        fp (r11)
x-pad        r6 -> r2        r6 -> r2
top-pad      r5 -> r3        r5 -> r3
```

Note that `top` and `lineHeight` are **already correct** in the current build;
the only wrong values are `pad` (fp instead of r9) and `x` (whole in r9 instead
of a `r5 -> r9` split). The clamp arm form (`itt gt; addgt.w r1,r6,sl;
subgt r5,r0,r1` vs the 4-instruction `itttt` form) is a *consequence* of the
same allocation difference: with pad in fp the compiler must materialise a pad
copy in both clamp arms.

The original's x path is `vmov r5,s18` (clamp) -> `mov r3,r5` (join, so that r5
can hold top-pad) -> `mov r9,r3` (callee-saved home for the drawBox call) ->
`mov r3,r9` (DrawString arg3). Ours has x in r9 from the start, so no copies.

## 3. Greedy-RA trace (instrumented LLVM 7, `RA_TRACE=1`, full TU)

`/tmp/llvm_android_702b/build2/bin/clang++` (LLVM 7, same revision as NDK r18b)
with an instrumented `RegAllocGreedy.cpp` / `CalcSpillWeights.cpp` reproduces
the official allocation. Raw vreg numbers in the full TU:

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

The target requires pad to be selected **before** x's re-enqueue and to take
r9. Since pad is `0xA` and x is `0xE`, that is only possible if pad also
carries a physical copy hint (`0xE`). Then pad (size 6400) is selected before x
(4220), takes r9, x takes r5, `%46` evicts x from r5, and x's re-enqueue finds
r9 occupied -> x splits `r5 -> r9`, exactly as in the original.

## 4. Why pad has no copy hint (MIR evidence)

`-mllvm -print-machineinstrs` on the full TU (pass just before RA, the one whose
vreg numbering matches the trace) shows pad `%2` has **no COPY at all** — seven
ALU/load uses only:

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

By contrast x `%609` *does* get a physical hint: the DrawString call emits
`$r3 = COPY %609` (arg3), and `CopyHint::operator<` always prefers a physreg
hint, so x's hint is `$r3`.

**Flat vs faithful clamp form.** The current source uses
`x = w - inset + (-width - pad - pad)`. That lowers to
`movgt r1,#0; subgt.w r1,r1,pad,lsl#1; subgt r1,r1,width; addgt x,r1,right`,
i.e. it uses pad a *second* time for the shift (MIR `%483`). The faithful form
`x = (w - inset) - (width + pad*2)` uses the already-computed `sl` (`%381`)
instead. Consequences measured on the full TU:

```text
source form   pad size   pad weight     pad reg   pct
flat (current)  6400     2.154168e-04   fp        95.3
faithful        6368     2.077779e-04   fp        93.8
```

So the flat form's extra pad use *raises* pad's weight (1.389 vs 1.334
use-def-frequency) but still not enough to matter, and it costs the wrong clamp
arm.

## 5. Priority experiments (knobs, to isolate the mechanism)

All numbers are the official compile command with the instrumented clang
(`knobscore.py`), object oracle.

### 5.1 `VRW` (scale one interval's weight)

```text
VRW pad factor   pct    pad   x     top   lineHeight
1.00 (base)      95.3   fp    r9    r8    fp
1.20 - 1.35      94.7   -     -     -     -
1.40 - 2.00      95.7   r9    r5    r1    r8
2.50 - 3.00      79.9   -     -     -     -
```

`VRW=1.45` is the best known configuration (95.7): it reproduces the target's
pad=r9 / x=r5 and the faithful two-instruction clamp arm, but the downstream
colouring moves `top` to r1 and `lineHeight` to r8 (the target has top r8,
lineHeight fp). On the faithful source `VRW=1.45` gives pad=r9, x=r5 but
top=fp, lineHeight=r8 (93.2, size 1944).

### 5.2 `HINTVREG` (force a physical hint at enqueue)

`HINTVREG=pad:r9` makes pad `0xE`+6400, so pad is selected 3rd and gets r9:

```text
select %2 pad w=2.154168e-04 hint=$r9 -> $r9   (tryAssign)
select %365 Canvas w=2.171786e-04 -> $r9 (tryEvict)   <-- Canvas evicts pad
```

Canvas's weight is only 0.81 % higher than pad's. Adding the copy-hint 1.01
boost (`HINTVREG=pad:r9 VRW=pad:1.01`, pad weight 2.175709e-04) makes pad
survive Canvas, but then **agent** `%1` (w=2.259431e-04) evicts it:

```text
assign %2 -> $r9 (tryAssign)
... Canvas cannot evict (shouldEvict false) ...
assign %1 -> $r9 (tryEvict)                    <-- agent evicts pad
```

So a copy hint alone is not sufficient: pad would additionally need to outrank
agent (a 4.9 % weight increase) or agent must be selected earlier (agent's hint
is `$r0`, occupied, so it falls back to the allocation order and lands on r8 in
the base allocation).

Other single-hint results on the current source (object oracle):

```text
pad -> r5   79.9 (1960 B)
pad -> r6   79.9 (1960 B)
pad -> r8   78.5 (1980 B)
pad -> r9   82.5 (1964 B)
pad -> r10  81.4 (1972 B)
pad -> r11  95.3 (1948 B)   (== base, pad already in fp)
```

Hint-subset searches (64 combinations of agent->r8, x->r5, top->r8, lh->r11,
boxw->r10, pad2->r10, with pad fixed to r9) top out at 95.3 on the flat source
and 93.8 on the faithful source; no combination reaches 100 %.

## 6. Source-variant sweep (new dimensions this wave)

126 variants over

```text
clamp form   : flat | faithful | faithful+pad2
pad2 decl    : none | `int pad2 = pad * 2;` right after pad
extra locals : none | inset | w | right | lineHeight-before-clamp
lineHeight   : after clamp (current) | before clamp
__builtin_expect on first / second clamp condition
```

Every variant stays at or below 95.3. The best remains the current source
(95.3). The previously reported 371-variant random sweep (`vrand.py`) over
top-expression x clamp-expression x boxW-expression x declaration position also
never exceeded 95.3.

Additional faithful-source forms measured this wave (all 93.8, pad=fp, x=r9):

```text
faithful + `int pad2 = pad * 2;` used in top/clamp/boxW
faithful + `int pad2 = pad + pad;` used in top/clamp/boxW
faithful + pad2 declared right after SetColor
faithful + pad2 only in clamp/boxW (top keeps `pad + pad`)
faithful clamp with `centerX` in the comparisons instead of `x`
faithful + `int lineHeight` before the clamp (94.9, pad=r5)
```

## 7. Conclusion / what remains

The blocker is now fully characterised:

1. pad is an unhinted `0xA` interval (no COPY anywhere in the MIR), so it is
   selected after every `0xE` interval, by which time x already occupies r9.
2. pad's weight (2.154e-4 flat / 2.078e-4 faithful) is below both x
   (2.946e-4) and Canvas (2.172e-4), so even when forced into r9 it is
   evicted; the 1.01 copy-hint boost is just enough to survive Canvas but not
   agent (2.259e-4).
3. The target is a natural greedy-RA outcome at one notch more register
   pressure (`-stress-regalloc=12` on the faithful IR reproduces the exact
   clamp/split structure, w4018 §6), so the original source very likely has one
   extra live value or a different vreg split that our reconstruction does not
   have.

No source shape tested this wave produces a physical copy hint for pad, and no
shape reproduces both the target's pad=r9 and its top=r8/lineHeight=fp
simultaneously. The remaining candidate directions:

* a source shape in which `pad` participates in a COPY (e.g. an inlined helper
  whose parameter is passed as a call argument) that coalesces onto pad and
  carries a physical hint;
* a source shape that raises pad's weight by ~5 % without adding instructions
  (the flat clamp form's extra pad use gives only ~3.7 %);
* a source shape with one more live value across the clamp (the
  `-stress-regalloc=12` result), e.g. an extra cached global or an additional
  temporary that the original kept alive.

## 8. Harness (scratch, `/dev/shm/w4246`, not committed)

```text
oracle.py / verify.py        object-level oracle (official compile command,
                             normalised instruction diff, autojunk=False)
knobscore.py <src> ENV=VAL   score with the instrumented clang++ and env knobs
scoreone.py <src> [ENV=VAL]  score + (pad,x,size) from the generated object
regmap2.py <src> [ENV=VAL]   register map from the .text.<fn> section only
diffsrc.py <src> [ENV=VAL]   unified diff against the original
tracefull.py <src> [ENV=VAL] RA_TRACE -> tf.log
genmir.py <src>              -mllvm -print-machineinstrs -> mir.txt
hs.py / hs2.py / hs3.py / hs4.py   HINTVREG subset searches
vsweep2.py                   the 126-variant source sweep
sweep.py / vrand.py          parallel 8-way variant runner (371-variant sweep)
```

Instrumented LLVM 7: `/tmp/llvm_android_702b/build2/bin/{clang++,llc}` with
knobs `RA_TRACE`, `HINTVREG=<rawvreg>:<phys>`, `VRW=<rawvreg>:<factor>`,
`NOHINT=<rawvreg>`. Phys numbers: r0=66 r1=67 r2=68 r3=69 r4=70 r5=71 r6=72
r8=74 r9=75 r10=76 r11=77.

Ghidra decompile of the lounge block (confirms the source shape):

```c
iVar10 = *(int *)(Globals::layout + 0x94);      // pad
AbyssEngine::PaintCanvas::SetColor((uint)Globals::Canvas);
iVar12 = iVar10 * 2;                            // pad*2, computed after SetColor
fVar19 = (float)VectorSignedToFloat(iVar12, ...);
iVar6  = (int)(fVar23 + (fVar20 - fVar23) * 0.5);  // centerX
iVar21 = (int)(fVar22 - fVar19);                   // top
...
iVar9 = *(int *)(Globals::layout + 0x28);       // buttonInsetX (cached)
if (iVar6 - iVar10 < iVar9) {
  iVar6 = iVar9 + iVar10;
} else if (Globals::w - iVar9 < iVar10 + iVar6 + iVar18) {
  iVar6 = (Globals::w - iVar9) - (iVar18 + iVar12);   // faithful clamp arm
}
uVar14 = *(undefined4 *)(Globals::layout + 0x30);     // lineHeight
uVar4  = AbyssEngine::String::String(aSStack_80, "", false);
Layout::drawBox(pLVar3, 2, iVar6 - iVar10, iVar21 - iVar10,
                iVar12 + iVar18, uVar14, uVar4);
```

The original TU witness (`source file list dump/gof2hd_1.1.19_ios.cpp`
lines 2776-2805) lists `SpaceLounge::drawLounge` between `draw()` and
`draw3DShip()`, i.e. our TU membership is correct.

## 9. Continuation (w4246, same wave): knob encoding, branch weights, and a
##    top/lineHeight incompatibility

Everything below was measured after the first draft of this note. No `src/`
change resulted; the gate is unchanged (byte 4499, linked 4499, size 4524,
data 843/843, imports 207, lint CLEAN, drift 0).

### 9.1 The instrumented-knob arguments must carry the virtual-register bit

`CalcSpillWeights.cpp` compares the parsed knob value against `li.reg`, and
`RegAllocGreedy.cpp` against `LI->reg`; both carry `TargetRegisterInfo`'s
`0x80000000` virtual-register bit. So the values in the §3/§5 tables are the
*low* halves only: the knob spellings are

```text
pad   %2    -> 2147483650   (0x80000002)
x     %609  -> 2147484257   (0x80000261)
agent %1    -> 2147483649   (0x80000001)
Canvas %365 -> 2147484013   (0x8000016d)
pad2  %381  -> 2147484029   (0x8000017d)
top   %389  -> 2147484037   (0x80000185)
```

`VRW=2:1.45` (bare `%N`) is silently inert and reproduces the base 95.3; the
tables above were produced with the full values, e.g.
`VRW=2147483650:1.45` -> 95.7.

### 9.2 Full pad-hint x pad-weight sweep (56 configurations)

`HINTVREG=pad:<phys>` x `VRW=pad:<factor>`, flat source, object oracle:

```text
hint   1.00   1.01   1.02   1.05   1.10   1.20   1.40   1.45   1.60   2.00
r4     79.8   79.8   79.8   79.8   79.8   79.8   79.8   79.8   79.8   79.8
r5     79.9   79.9   79.9   79.9   79.9   79.9   79.9   79.9   79.9   79.9
r6     79.9   79.9   79.9   79.9   79.9   79.9   79.9   79.9   79.9   79.9
r8     78.5   82.5   82.5   79.5   79.5   79.5   79.5   79.5   79.5   69.1
r9     82.5   78.5   78.5   81.0   81.0   81.0   82.2   82.2   82.2   82.2
r10    81.4   92.9   92.9   93.9   93.9   93.9   94.8   94.8   94.8   94.8
r11    95.3   93.6   93.6   94.5   94.5   94.5   95.7   95.7   95.7   95.7
```

The best is still 95.7 (`pad=r9`, `x=r5`), reached either with
`HINTVREG=pad:r11 VRW=pad:>=1.40` (pad evicts x from r9 by weight) or with
no hint at all at `VRW=pad:1.45`.

### 9.3 The blocker is not just "pad must take r9"

Every configuration that reaches `pad=r9` **and** `x=r5` also moves the two
values the current build already gets right:

```text
configuration                 pad  x    top  lh   size  pct
base                          fp   r9   r8   fp   1948  95.3
HINTVREG=pad:r11 VRW>=1.40    r9   r5   r1   r8   1948  95.7
VRW=pad:1.45 (no hint)        r9   r5   r1   r8   1948  95.7
HINTVREG=pad:r9 VRW=1.40      r9   r5   r1   r8   1972  82.2
target                        r9   r5   r8   fp   1948  100
```

`top` is `r8` and `lineHeight` is `fp` in the target *and* in the current
build (both are `mov r8, r2` / `ldr.w fp, [r4, #48]` around the `drawBox`
call); only the post-call agent reload differs (`mov fp, r8; ldr.w r8,
[sp,#12]` in the target vs `ldr r5, [sp,#12]` in ours). So "pad gets r9" is
necessary but **not sufficient**: the original allocation is not a pure
weight/hint perturbation of ours. A hint/weight search of
`{agent,top,lineHeight,x} -> phys` on top of `pad -> r9` (faithful and
flat sources, 24 hint subsets x 5 weight levels each) never exceeds 95.7 and
never recovers `top=r8`/`lh=fp` simultaneously with `pad=r9`.

### 9.4 Branch-weight perturbations move pad's spill weight but change layout

`__builtin_expect` on the two clamp conditions changes pad's spill weight
(base 2.154168e-04, Canvas 2.171786e-04, agent 2.259431e-04):

```text
variant                              pad weight    size  pct   pad  x
base                                 2.154168e-04  1948  95.3  fp   r9
left clamp expect(...,1)             2.068087e-04  1952  90.7  fp   r9
left clamp expect(...,0)             2.240248e-04  1952  91.6  fp   r9
right clamp expect(...,1)            2.234989e-04  1956  91.6  fp   r9
right clamp expect(...,0)            2.068087e-04  1956  91.6  fp   r9
left 0 + right 1                     2.406660e-04  1956  91.4  fp   r9
```

`left 0 + right 1` pushes pad's weight above both Canvas and agent
(2.406660e-04) yet pad still lands in `fp`, because x (2.946e-04) still
outranks it and takes r9 first. Every branch-weight change also adds 4-8
bytes of branch layout, so none is a candidate.

### 9.5 Ghidra re-decompile confirms the faithful form

Decompiling the real function (Ghidra address `0x180d38`, which is ELF
`0x170d38`; `readelf` reports `0x170d39` because of the Thumb bit) gives:

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

So the original source really is the **faithful** clamp form with a separate
`pad2 = pad * 2` local, i.e. `spacelounge-drawlounge-w4227.md`'s "faithful +
pad2" variant (93.8), not the flat form currently in `src/` (95.3). The flat
form is a scoring artefact: it happens to keep `top`/`lineHeight` and lose
only pad/x, whereas the faithful form keeps the correct 2-instruction clamp
arm but needs two extra `mov r5, fp` pad copies. Neither reaches 100.

### 9.6 Additional source variants tried (all <= 95.3)

```text
unsigned pad / width / x / all           95.0 / 95.3 / 95.2 / 95.0
const pad / const lineHeight             95.3 / 95.3
Layout* cached at top of block           77.2 (1984 B)
top before centerX                       95.3
top = (int)(y - (float)pad - (float)pad) 81.1 (1968 B)
top = (int)y - pad - pad                 81.3 (1956 B)
top = (int)(y - (float)(2*pad))          95.3
top = (int)(y - (float)(pad*2))          95.3
centerX = (int)((topLeft.x+bottomRight.x)*0.5f)   94.6
centerX = (int)(bottomRight.x+(topLeft.x-bottomRight.x)*0.5f)  94.9
no separate centerX local                95.3
centerX in the clamp comparisons         87.1
x initialised directly from the float expr 87.1
pad loaded after SetColor                92.6
pad loaded between centerX and top       92.6
```

### 9.7 Conclusion (unchanged, but sharper)

The blocker is not "pad has no copy hint" alone. The target's allocation
(`pad=r9`, `x=r5` split `r5 -> r9`, `top=r8`, `lineHeight=fp`) is not on the
reachable manifold of our source under any pad hint/weight perturbation: every
move of pad into r9 drags `top` to r1 and `lineHeight` to r8. The original
must therefore differ in a way that changes *which* intervals are live and in
what order they are selected -- a different vreg structure (e.g. an extra
value live across the clamp, or a different split of `x`), not a different
weight of the same structure. `-stress-regalloc=12` on the faithful IR still
reproduces the target's clamp/split arm but with `top` spilled and
`lineHeight` in r8, i.e. the same trade-off.

Concrete next directions, in order of promise:

1. Find a source shape in which `x` is **not** the DrawString argument
   (`$r3 = COPY %x`) but a distinct value, so x loses its `0xE` priority and
   pad (0xA, size 6400) is selected before it. `NOHINT` on x is far too
   destructive (65.7), but a *different* copy chain might not be.
2. Find a source shape that gives `pad` a physical copy hint **to r9**
   specifically. A hint to r0-r3 makes pad take r5 (79.9); only r9 works.
   r9 is not an argument register, so this needs a COPY that names r9 --
   which in practice means a different vreg split, not a call argument.
3. Reproduce the "one extra live value across the clamp" that
   `-stress-regalloc=12` simulates, without changing the instruction stream.
