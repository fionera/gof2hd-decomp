# SpaceLounge::drawLounge — residual diff enumeration and weight/priority experiments (w4227, wave 92)

Status: research note (2026-11-03). Worktree `wt/w4227`, branch `wave92-w4227`,
base `b379d00d`. **No `src/` change landed**: every source shape tested keeps the
official score at 95.6 (byte_exact unchanged at 4498). The tree is clean.

Baseline (`cmake --build cmake-build-match --target check -j8`, before and after):

```text
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`_ZN11SpaceLounge10drawLoungeEv`: official `pct 95.6`, `osize = usize = 1948`,
`byte = linked = false` (from `report.json`). Object-level oracle
(`_work/reconstructions/w3692_oracle.py` copy at `/dev/shm/w4227/oracle.py`)
reports `95.3`.

Prior work: `spacelounge-drawlounge-w4118.md` (full greedy-RA trace and the
`faith_xy` near-miss), `-w4018.md`, `-w3818.md`, `-ra-split-w3838.md`,
`SpaceLounge-OnRender3D.md`, `SpaceLounge-layout.md`. This note only records the
new evidence gathered in wave 92.

## 1. Residual diff, enumerated

The normalised mnemonic streams are 704 instructions (original) and 704 (ours),
both 1948 bytes. `difflib` (`autojunk=False`) gives 74 changed lines (37 removed +
37 added), all in
one contiguous region: the clamp right arm, the join, the post-`drawBox` agent
restore, and the tail branch offsets of the landing pads. The full diff of the
current build is:

```text
@@ -141,10 +141,10 @@
 mov.w	r1, #4294967295
 str	r5, [sp, #24]
-ldr.w	r9, [r6, #148]          ; pad -> r9
+ldr.w	fp, [r6, #148]          ; pad -> fp
 vldr	s16, [r2]
 vldr	s18, [r2, #4]
 vldr	s20, [r3]
 call	sym:_ZN11AbyssEngine11PaintCanvas8SetColorEj
-mov.w	sl, r9, lsl #1          ; pad*2 -> sl
+mov.w	sl, fp, lsl #1          ; pad*2 -> sl
 vmov.f32	s0, #96
 vmov	s4, sl
@@ -504,13 +504,13 @@
 call	sym:_ZN11AbyssEngine6StringD1Ev
 add	r6, r4                  ; width += ...
-vmov	r5, s18                 ; x = centerX -> r5
+vmov	r9, s18                 ; x = centerX -> r9
 ldr	r0, [sp, #20]
 ldr	r4, [r0, #0]
 ldr	r0, [r4, #40]           ; buttonInsetX
-sub.w	r1, r5, r9              ; x - pad
+sub.w	r1, r9, fp              ; x - pad
 cmp	r1, r0
 bge.n	loc_592
-add.w	r5, r0, r9             ; x = inset + pad
-b.n	loc_5ac
+add.w	r9, r0, fp             ; x = inset + pad
+b.n	loc_5b2
 ldr	r1, [pc, #500]
 add	r1, pc
@@ -518,11 +518,15 @@
 ldr	r1, [r1, #0]
 subs	r0, r1, r0             ; r0 = w - inset
-add.w	r1, r9, r5             ; pad + x
+add.w	r1, fp, r9             ; pad + x
 add	r1, r6                  ; + width
 cmp	r1, r0
-itt	gt
-addgt.w	r1, r6, sl             ; width + pad*2
-subgt	r5, r0, r1             ; x = (w-inset) - (width+pad*2)
-ldr	r1, [pc, #476]
+itttt	gt
+movgt	r1, #0
+subgt.w	r1, r1, fp, lsl #1
+subgt	r1, r1, r6
+addgt.w	r9, r1, r0
+str.w	r8, [sp, #12]
+mov	r5, fp                 ; pad copy
+ldr	r1, [pc, #464]
 ldr.w	fp, [r4, #48]          ; lineHeight
 add	r1, pc
@@ -531,11 +535,8 @@
 call	sym:_ZN11AbyssEngine6StringC1EPKcb
 vmov	r2, s16                ; top
-mov	r3, r5                  ; x -> r3
+mov	r1, r5                  ; pad -> r1
 add	sl, r6                  ; boxW = pad*2 + width
-mov	r1, r9                  ; pad
-subs	r6, r3, r1             ; x - pad
-str.w	r8, [sp, #12]          ; spill agent
-sub.w	r5, r2, r9             ; top - pad
-mov	r9, r3                  ; x -> r9
+sub.w	r6, r9, r1             ; x - pad
+subs	r5, r2, r5             ; top - pad
 mov	r8, r2                  ; top -> r8
 strd	sl, fp, [sp]
@@ -549,9 +550,7 @@
 call	sym:_ZN11AbyssEngine6StringD1Ev
 ldr	r0, [sp, #24]
-ldr	r1, [pc, #380]
-ldr	r0, [r0, #0]
-call	sym:_ZN11AbyssEngine11PaintCanvas8SetColorEj
-mov	fp, r8                  ; top -> fp
-ldr.w	r8, [sp, #12]          ; agent -> r8
+ldr	r1, [pc, #376]
+ldr	r0, [r0, #0]
+call	sym:_ZN11AbyssEngine11PaintCanvas8SetColorEj
 ldr	r0, [sp, #20]
 ldr	r1, [r0, #0]
@@ -572,5 +571,6 @@
 ldr	r0, [sp, #24]
 ldr	r4, [r0, #0]
-mov	r0, r8
+ldr	r5, [sp, #12]
+mov	r0, r5
 call	sym:_ZN5Agent7isKnownEv
 cbz	r0, loc_63c
@@ -594,5 +594,5 @@
 ldr.w	r0, [r0, #704]
 movs	r1, #0
-add	r0, fp
+add	r0, r8
 add	r2, sp, #80
 strd	r0, r1, [sp]
@@ -605,5 +605,5 @@
 add	r0, sp, #68
 call	sym:_ZN11AbyssEngine6StringD1Ev
-mov	r0, r8
+mov	r0, r5
 call	sym:_ZN5Agent7isKnownEv
 cbz	r0, loc_6de
@@ -633,5 +633,5 @@
 ldr.w	r1, [r1, #704]
 movs	r2, #0
-add	r1, fp
+add	r1, r8
 strd	r1, r2, [sp]
 add.w	r3, r0, r9
@@ -670,9 +670,9 @@
 b.n	loc_1e2
 b.n	loc_734
-b.n	loc_756
+b.n	loc_758
+b.n	loc_72a
 b.n	loc_72a
 b.n	loc_734
-b.n	loc_756
-b.n	loc_72a
+b.n	loc_758
 mov	r4, r0
 add	r0, sp, #80
@@ -682,5 +682,5 @@
 add	r0, sp, #68
 call	sym:_ZN11AbyssEngine6StringD1Ev
-b.n	loc_758
+b.n	loc_75a
 mov	r4, r0
 add	r0, sp, #56
@@ -688,11 +688,12 @@
 mov	r4, r0
 add	r0, sp, #32
+b.n	loc_762
+b.n	loc_758
+b.n	loc_758
+mov	r4, r0
+add	r0, sp, #44
+b.n	loc_762
+mov	r4, r0
 b.n	loc_760
-b.n	loc_756
-mov	r4, r0
-add	r0, sp, #44
-b.n	loc_760
-mov	r4, r0
-b.n	loc_75e
 mov	r4, r0
 add	r0, sp, #92
@@ -702,3 +703,2 @@
 mov	r0, r4
 call	#X
-nop
```

The tail differences are landing-pad branch offsets; they are a consequence of
the two-instruction size difference in the clamp/join region and disappear once
that region matches.

## 2. Register map

```text
value        original   ours (current)
layout       r4         r4
x            r5 -> r9   r9
width        r6         r6
agent        r8         r8 (then r5 after drawBox)
top          r8 -> fp   r8
pad          r9         fp (r11)
pad*2 (sl)   r10        r10
lineHeight   r11 (fp)   r11 (fp)
```

The original's clamp right arm is the faithful two-instruction form
`itt gt; addgt.w r1,r6,sl; subgt r5,r0,r1` (i.e.
`x = (w - inset) - (width + pad*2)`). The current source uses
`x = (w - inset) + (-width - pad - pad)`, which lowers to the four-instruction
`itttt` form. The faithful source scores 93.8 because pad is in `fp`, so the
allocator emits a `mov r5,fp` pad copy in *both* clamp arms (pad is split: `fp`
for the clamp, `r5` for the join).

The target is exactly: pad unsplit in `r9`, x in `r5` during the clamp, then
split at the join (`mov r3,r5` / `mov r9,r3`), top in `r8` after spilling agent,
lineHeight in `fp`.

## 3. Greedy-RA trace (instrumented LLVM 7, `RA_TRACE=1`)

`/tmp/llvm_android_702b/build2/bin/llc` reproduces the official allocation on the
extracted function IR. Base order (vreg numbers from the extracted IR):

```text
ENQ  %2   pad       size=6272  prio=0xA0000000+6272  hint=0,$noreg  knownpref=0
ENQ  %44  lineH     size=368   prio=0xA0000000+368
ENQ  %45  x-pad     size=704   prio=0xE0000000+704   hint=0,$r2
ENQ  %46  top-pad   size=736   prio=0xE0000000+736   hint=0,$r3
ENQ  %47  boxW      size=768   prio=0xE0000000+768   hint=0,$r3
ENQ  %369 pad*2     size=6032  prio=0xA0000000+6032  hint=0,%370     knownpref=0
ENQ  %573 x         size=4204  prio=0xE0000000+4204  hint=0,$r3

select %573 x       w=2.956378e-04 -> $r5
select %47  boxW    w=6.080038e-04 -> $r6
select %46  top-pad w=6.251306e-04 -> $r5   (evicts x; 6.25e-4 > 2.96e-4)
select %573 x       w=2.956378e-04 -> $r9   (r9 still free)
select %45  x-pad   w=6.432504e-04 -> $r6   (evicts boxW)
select %47  boxW    w=6.080038e-04 -> $r10
select %2   pad     w=2.107675e-04 -> $r11  (tryEvict; r9 = x, r10 = boxW)
select %369 pad*2   w=1.196102e-04 -> $r10  (reuses boxW's reg after boxW dies)
select %44  lineH   w=6.103455e-04 -> $r11  (evicts pad; 6.10e-4 > 2.11e-4)
ENQ %2 pad (stage 1) -> no free register -> ENQ stage 2 (split)
```

So x is **not split** in our build: it is evicted whole from `$r5` by `%46` and
re-assigned whole to the free `$r9`. The target's `x = r5 -> r9` split requires
`$r9` to be occupied when x is re-enqueued. The only interval that can occupy it
in time is pad, and pad is selected *after* `%46` because pad has no copy hint
(`prio 0xA...`) while `%46` does (`prio 0xE...`).

Weights (`CalcSpillWeights::normalizeSpillWeight = UseDefFreq/(Size+50)`):

```text
pad       2.107675e-04  size 6272  -> UseDefFreq ~1.332
x         2.956378e-04  size 4204  -> UseDefFreq ~1.258
%46       6.251306e-04  size 736
%45       6.432504e-04  size 704
%47       6.080038e-04  size 768
lineH     6.103455e-04  size 368
pad*2     1.196102e-04  size 6032
```

Note that pad's *use-def frequency* is already slightly higher than x's; only
its much longer live range keeps its weight below x's.

## 4. Proof that pad's weight is the blocker (`VRW` knob)

The instrumented llc has a `VRW=<vreg>:<factor>` knob
(`lib/CodeGen/CalcSpillWeights.cpp`) that scales one interval's weight after
normalisation. Scaling pad (`0x80000002`) by >= 1.45 makes its weight
`3.056129e-04 > x's 2.956378e-04`; pad then evicts x from `$r9` and x is split.
Register maps from `hintsearch.classify` on the *faithful* IR:

```text
VRW pad factor   pad   x    top   lineHeight  agent
none             r11   r9   r8    r11         r8
1.05             r11   r9   r8    r11         r8
1.10 - 1.40      r11   r9   r11   r8          r8
1.45 - 2.00      r9    r5   r11   r8          r8
3.00             r11   r5   r1    r8          r8
```

At factor 1.45 the clamp is exactly the target's two-instruction faithful arm
and the join has the target's shape:

```text
vmov r5, s18          ; x = r5
sub.w r1, r5, r9      ; x - pad (pad = r9)
add.w r5, r0, r9      ; x = inset + pad
add.w r1, r9, r5      ; pad + x
itt gt
addgt.w r1, r6, r10   ; width + pad*2
subgt r5, r0, r1      ; x = (w-inset) - (width+pad*2)
...
vmov r1, s16          ; top
mov r2, r5            ; x -> r2
add r10, r6           ; boxW
sub.w r5, r1, r9      ; top - pad
mov r11, r1           ; top -> r11
mov r1, r9            ; pad
subs r6, r2, r1       ; x - pad
mov r9, r2            ; x -> r9
```

This is the correct *mechanism* (pad evicts x from r9), but the downstream
colouring is still off: top lands in `r11` and lineHeight in `r8` (the target
has top `r8`, lineHeight `r11`). So a pure weight boost is not sufficient; the
original's allocation must also keep top/lineHeight in their target registers.

## 5. Physical-hint experiment (recap + new confirmation)

`HINTVREG` sets a target-independent hint at enqueue. On the faithful IR,
`HINTVREG=2147484221:71,2147484017:76,2147483695:76` (x->r5, pad*2->r10,
boxW->r10) gives:

```text
{'pad': 'r9', 'agent': 'r8', 'width': 'r6', 'centerX': 'r5',
 'x': 'r5', 'layout': 'r4', 'lineHeight': 'r11', 'top': 'r8'}
```

i.e. the target's register map *and* the target's two-instruction clamp arm
(with `vmov r5,s18`, `add.w r5,r0,r9`, `itt gt; addgt.w r1,r6,r10; subgt r5,r0,r1`).
The join still differs slightly (`subs r6,r5,r1; sub.w r9,r2,r9` instead of the
target's `mov r3,r5; ...; mov r9,r3`), so this is a strong near-miss, not a
byte match. The mechanism is the same as §4: giving pad*2/boxW the `1<<30`
priority bit makes them dequeue before pad, so pad is selected while r9 is free.

The trace of that run:

```text
select %369 pad*2  (hinted r10) -> $r10
select %573 x      (hinted r5)  -> $r5
select %47  boxW   (hinted r10) -> $r10
select %46  top-pad             -> $r6
select %45  x-pad               -> $r6   (evicts %46)
select %46  top-pad             -> $r9
...
select %2   pad                 -> $r11  (r9 = %46)
select %44  lineH               -> $r11  (evicts pad)
```

## 6. Source variants measured this session

All measurements are the object-level oracle (official compile command,
normalised instruction diff, `autojunk=False`). Base = 95.3. Register columns
are from the generated object (`ldr.w rX,[..,#148]` for pad, `vmov rX,s18` for
x). Every variant is 1948 B unless noted.

### 6.1 Clamp arm forms

| variant | pct | pad | x |
|---|---:|---|---|
| `+(-width - pad - pad)` (current, flat) | 95.3 | fp | r9 |
| `-(width + pad * 2)` (faithful) | 93.8 | fp | r9 |
| `-(width + pad + pad)` | 95.2 | fp | r9 |
| `-(width + 2 * pad)` | 93.8 | fp | r9 |
| `-(width + (pad << 1))` | 93.8 | fp | r9 |
| `-width - pad * 2` | 93.8 | fp | r9 |
| `+ -(width + pad * 2)` | 93.8 | fp | r9 |
| `(w - inset) - (width + pad*2)` | 93.8 | fp | r9 |
| `x = w - inset; x -= width + pad*2;` | 93.8 | fp | r9 |
| `int right = w - inset; x = right - (width+pad*2);` | 93.8 | fp | r9 |
| `-(width + pad + pad)` (pad+pad form) | 95.2 | fp | r9 |
| two separate `if`s | 91.5 (1940 B) | fp | r9 |
| nested `else { if }` | 93.8 | fp | r9 |
| reversed condition | 93.8 | fp | r9 |
| two ternaries | 91.5 (1940 B) | fp | r9 |

### 6.2 Declaration / statement order (faithful arm)

| variant | pct | pad | x |
|---|---:|---|---|
| base order | 93.8 | fp | r9 |
| `x` before width | 93.8 | fp | r9 |
| `x` after top | 93.8 | fp | r9 |
| `x` after pad | 93.8 | fp | r9 |
| top before centerX | 93.8 | fp | r9 |
| pad after SetColor | 91.1 | fp | r9 |
| pad before top | 91.1 | fp | r9 |
| pad after centerX | 91.1 | fp | r9 |
| top after width | 70.1 (1972 B) | fp | r6 |
| centerX after width | 85.5 | fp | — |
| top+centerX after width | 69.6 (1972 B) | fp | — |
| pad+top after width | 67.7 (1964 B) | r8 | r6 |
| pad+top+centerX after width | 66.8 (1968 B) | fp | — |
| layout after pad | 65.8 (1980 B) | r4 | sl |
| lineHeight before clamp | 94.9 | fp | r9 |
| `String("")` before clamp | 72.5 (1988 B) | fp | r9 |
| layout before width | 78.5 (1964 B) | fp | r9 |
| `__builtin_expect(mode==0,1)` | 93.8 | fp | r9 |
| `__builtin_expect(hoverAgent<0,0/1)` | 93.8 | fp | r9 |

### 6.3 `top` / `pad` expression forms

| variant | pct | pad | x |
|---|---:|---|---|
| `(float)(pad * 2)` | 93.8 | fp | r9 |
| `(float)(pad + pad)` (current) | 93.8 | fp | r9 |
| `(float)pad * 2.0f` | 81.1 (1968 B) | fp | r9 |
| `(float)pad + (float)pad` | 81.1 (1968 B) | fp | r9 |
| `bottomRight.y - pad - pad` (implicit) | 81.1 (1968 B) | fp | r9 |
| `(int)bottomRight.y - pad - pad` | 80.9 (1960 B) | fp | r9 |
| `int pad2 = pad*2` in top only | 93.8 | fp | r9 |
| `int pad2 = pad*2` in boxW only | 93.8 | fp | r9 |
| `int pad2 = pad*2` in top+clamp+boxW | 93.8 | fp | r9 |
| `int pad2 = pad+pad` in top+clamp+boxW | 93.8 | fp | r9 |
| `int pad2 = pad<<1` in top+clamp+boxW | 93.8 | fp | r9 |
| `const int pad` | 93.8 | fp | r9 |
| `int pad0=...; int pad=pad0;` | 93.8 | fp | r9 |
| `int pad = ... + 0` | 93.8 | fp | r9 |
| `int &pad = ...` | 75.4 (1972 B) | fp | r9 |
| top uses `pad*2`, boxW uses `pad+pad` | 71.8 (1960 B) | fp | r6 |

### 6.4 Join localisation (faithful arm)

| variant | pct | pad | x |
|---|---:|---|---|
| `int xp=x-pad; int tp=top-pad;` before drawBox | 93.8 | fp | r9 |
| `int xPad/yPad/boxW` after clamp, both calls | 93.8 | fp | r9 |
| `int xPad/yPad` direct field, both calls | 80.1 (1956 B) | r8 | r5 |
| `int pad2` direct field, both calls | 80.7 (1952 B) | r8 | r5 |
| `int boxW = width + pad*2` after clamp | 71.6 (1968 B) | fp | r9 |
| `int xPad2/yPad2/boxW2` only for DrawRectangle | 77.3 (1976 B) | fp | r9 |

### 6.5 Direct `Globals::layout->field_0x94` in `DrawRectangle` (all 8 subsets)

Faithful arm; `pad` local still used for `boxW`. (1,1,0) = x-pad and top-pad
direct, boxW local:

| subset (x-pad, top-pad, boxW) | pct | size | clamp pad | x |
|---|---:|---:|---|---|
| 000 (none) | 93.8 | 1948 | fp | r9 |
| 001 | 71.5 | 1976 | sl | r5 |
| 010 | 81.2 | 1964 | fp | r9 |
| 011 | 81.2 | 1964 | fp | r9 |
| 100 | 77.9 | 1960 | sl | r9 |
| 101 | 77.3 | 1964 | sl | r9 |
| **110** (`faith_xy`) | **89.6** | **1940** | **r9** | **r5** |
| 111 | 89.3 | 1944 | r9 | r5 |

The "clamp pad" column is the first `ldr.w rX,[..,#148]`; the last load in the
001/010/011/100/101/110/111 variants is the DrawRectangle reload (`r1`/`r3`).

`faith_xy` is the only source shape that gives the target's clamp colouring
(pad=r9, x=r5) *and* the two-instruction arm. Its join is:

```text
57a vmov r5, s18          ; x = r5
588 sub.w r1, r5, r9      ; x - pad
590 add.w r5, r0, r9      ; x = inset + pad
5a0 add.w r1, r9, r5      ; pad + x
5a8 itt gt
5aa addgt.w r1, r6, sl
5ae subgt r5, r0, r1
5b0 ldr r1, [pc,#464]
5b2 ldr.w r8, [r4,#48]    ; lineHeight = r8   (target: fp)
5c0 vmov fp, s16          ; top = fp          (target: r2)
5c4 add r6, sl            ; boxW
5c6 sub.w r2, r5, r9      ; x-pad  (target: subs r6,r3,r1)
5ca sub.w r3, fp, r9      ; top-pad (target: sub.w r5,r2,r9)
5ce strd r6, r8, [sp]
```

The direct field access forces a reload of `Globals::layout->field_0x94` after
`drawBox` (the calls may clobber the global), which shortens pad's live range and
makes the compiler recompute x-pad/top-pad directly into the call-argument
registers instead of keeping them in `r6`/`r5` across the two calls. That reuse
change costs more than the clamp gain (89.6 vs 95.3).

The same 8 subsets with the *flat* clamp: best 95.3 (000), (110) = 91.0 with
clamp pad r9/x r5, (100) = 92.3.

## 7. Conclusion

The source is otherwise correct: the normalised instruction streams match for
the first ~500 instructions, and the only differences are the pad/x colouring
and its downstream consequences (agent spill point, landing-pad offsets).

The target requires one of:

1. **pad gets a physical copy hint / `1<<30` priority bit**, so it dequeues
   before `%46` (top-pad) and takes `r9` while it is free; then `%46` evicts x
   from `r5` and x splits. pad is a plain `t2LDRi12` with no COPY in the machine
   IR, and no source shape tested creates one. (`HINTVREG` proves this path
   yields the target clamp and near-target join.)
2. **pad's spill weight exceeds x's**, so pad evicts x from `r9` when selected
   after `%46`. `VRW` proves this path too (>=1.45x), but it also moves
   top/lineHeight off their target registers.

Every natural source shape tested (clamp arm forms, declaration order, `top`
expression forms, `pad`/`pad2` locals, join localisation, all 8 direct-field
subsets, branch-weight hints, statement reordering) keeps pad in `fp` and x in
`r9`; the best remains the current flat clamp at 95.3 object / 95.6 official.
No strict improvement was found, so no `src/` change is landed.

Remaining lead: a source shape that gives pad a copy hint *and* keeps
x-pad/top-pad/boxW live across `drawBox` (i.e. the target's reuse), without the
`field_0x94` reload. The `faith_xy` reload is the only known way to get pad into
`r9`; if the reload could be avoided (e.g. by a source construct that the
compiler can prove does not clobber `Globals::layout`), the target join should
follow.

## 8. Harness (all in `/dev/shm/w4227`, not committed)

* `oracle.py` — copy of `_work/reconstructions/w3692_oracle.py` with paths
  patched for this worktree.
* `score_variant.py` — compile one TU with the official command and score
  `_ZN11SpaceLounge10drawLoungeEv` by normalised instruction diff.
* `vtest.py` — extracts the `drawLounge` mode-0 block into `BASE` and exposes
  `test(name, block)` (writes `/dev/shm/w4227/variant.cpp`, scores it).
* `regs.py` — compiles a source and reports the pad (`ldr.w rX,[..,#148]`) and
  x (`vmov rX,s18`) registers plus the function size.
* `dump2.py <src>` — writes `orig_norm.txt` / `ours_norm.txt` / `ours_raw.txt`.
* `raworig.py` — writes `orig_raw.txt` (index + address + raw insn).
* `batchE.py` … `batchJ.py` — the variant batches tabulated in §6.
* `hintsearch.py` — runs the instrumented llc
  (`/tmp/llvm_android_702b/build2/bin/llc`, with `RA_TRACE`, `HINTVREG`,
  `RA_NOHINT`, `VRW`) on `faith.ll` and reads the `@DEBUG_VALUE` register map.
* `dl_trace.log`, `faith_trace.log`, `base_trace.log`, `vrw145_trace.log` —
  greedy-RA traces.
* `faith_xy.cpp` — the 89.6 / 1940 B near-miss.
