# w8412 (wave-139): `Level::createMission` — station-pirate escort loop /
# Greedy-RA remat of `movw #10000` (no source win)

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w8412`, based on master
`bf123bd47` (wave 138 landed). Continues
`level-createmission-wave138-w8311.md`, `…-wave137-w8214.md`,
`…-wave136-w8112.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base `0x10000`; `_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314`
= 17,172 B). Manual NDK `objdump` disassembly of the original is the
authority; the MIR below is from the checked-in `Level.cpp` compiled with the
original flags plus `-mllvm -print-machineinstrs
-mllvm -filter-print-funcs=_ZN5Level13createMissionEv`.

## Baseline (pristine HEAD, re-measured)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw same-address diff 5766
.text diff 85,476   .ARM.extab 1,396   .rel.plt 0   .note.gnu.build-id 20
.rodata/.data/.bss/.ARM.exidx exact
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
```

## The hypothesis

The station-pirate escort loop (source `Level.cpp:2043-2054`) has three
`+ 10000` offsets.  The original keeps the literal in a **preheader**
register (`movw fp,#10000` at `b3ba4`, used by the three `mla` at
`b3c18/b3c2e/b3c5c`).  Ours **rematerialises** it inside the loop body:

```text
orig                                           ours
b3ba2 ldr  r6,[sp,#20]                         b3b8e ldr  r6,[sp,#32]
b3ba4 movw fp,#10000                           b3b90 ldr  r5,[sp,#20]
b3ba8 ldr  r5,[sp,#112]                        b3b92 add  r6,r5
b3baa add  r6,r5                               b3b94 ldr  r5,[sp,#104]
b3bac ldr  r5,[sp,#32]                         b3b96 add  r6,r5
b3bae add  r6,r5                               b3b98 str  r6,[sp,#92]
b3bb0 str  r6,[sp,#104]                        b3b9a ldrd r9,r5,[sp,#96]
b3bb2 ldrd r9,r5,[sp,#96]                      b3b9e b.n  b3cbc
b3bb6 b.n  b3ccc
...
b3c18 mla  r0,r0,r6,fp                         b3bfe mov  fp,sl
b3c2e mla  r0,r0,r1,fp                         b3c00 movw sl,#10000
b3c5c mla  r0,r0,r4,fp                         b3c06 mla  r0,r0,r6,sl
                                               b3c1c mla  r0,r0,r1,sl
                                               b3c4a mla  r0,r0,r4,sl
                                               b3c52 mov  sl,fp
```

So ours uses `sl` for the literal and parks `this` in `fp` for the middle of
the body (two extra `mov`s).  The original uses `sl` for `this` throughout and
`fp` for the literal.  Everything else in the loop body is
instruction-for-instruction identical (same `nextInt`/`createShip`/`mla`/
`vcvt`/`vadd`/`setPosition` sequence, same `s16/s18/s20/s22/s24` map).

## MIR evidence: the literal is hoisted before RA, rematerialised by RA

`After Simple Register Coalescing` (pre-RA):

```text
bb.276 (%ir-block.2781):          ; escort-loop preheader
  ...
  %3522:rgpr = t2MOVi16 10000, 14, $noreg
  t2B %bb.280
bb.282 (%ir-block.2874):          ; loop body
  %3523:rgpr = t2MLA %3521, %3517, %3522
  %3526:rgpr = t2MLA %3525, %3511, %3522
  %3536:rgpr = t2MLA %3535, %3530, %3522
```

`After Greedy Register Allocator` (post-RA):

```text
bb.276:  (no t2MOVi16)
bb.282:
  %5418:gpr = COPY %5419:gprnopc     ; this: sl -> fp
  %5416:rgpr = t2MOVi16 10000        ; remat in the body
  %3523:rgpr = t2MLA ..., %5416
  %3526:rgpr = t2MLA ..., %5416
  %3536:rgpr = t2MLA ..., %5416
```

`After Virtual Register Rewriter` confirms `$r11 = COPY $r10`,
`$r10 = t2MOVi16 10000`, `$r10 = COPY $r11`.  So the literal is
**loop-invariant and already in the preheader before RA**; Greedy RA's
`tryAssign` fails, `trySplit` (`SplitKit::defFromParent`) splits the interval
at the loop body and rematerialises it (`canRematerializeAt` is true for
`t2MOVi16`).

## Why `tryAssign` fails: the loop needs all 7 callee-saved registers

The loop body carries exactly seven callee-saved values:

| value | orig reg | ours reg |
|---|---|---|
| `this` | `sl` (r10) | r10 (start) / r11 (middle) |
| `i` | r8 | r8 |
| `&Globals::rnd` | r5 | r5 |
| `AERandom::nextInt` | r9 | r9 |
| `10000` | `fp` (r11) | r10 (middle) |
| x sign (`r4`) | r4 | r4 |
| y value (`r6`) | r6 | r6 |

`r7` is the frame pointer, so the allocatable callee-saved set is
`{r4,r5,r6,r8,r9,r10,r11}` — exactly seven registers for seven values.  Both
assignments are legal; the difference is a Greedy-RA tie-break.  In ours the
literal is processed after `this` and evicts `this` from r10 (the literal has
3 uses in a short interval, `this` has 4 uses over the whole function, so the
literal's spill weight is much higher).  In the original the literal takes the
last free register r11 and `this` stays in r10.  The eviction is
`tryEvict`/`trySplit`, not a source-order bug.

## Source-shape sweep (43 variants, fresh `link_so` each)

Raw same-address diff bytes inside the 17,172-byte symbol (deterministic;
primary metric).  All variants keep the `mov fp,sl` / `movw sl,#10000` /
`mov sl,fp` remat unless noted.

| variant | raw diff | size |
|---|---:|---:|
| **baseline (checked-in)** | **5766** | **17172** |
| named `int escortOffset = 10000;` before loop | 5766 | 17172 |
| `const int` / `register int` / `static const int` / `enum` | 5766 | 17172 |
| `10000L` / `(int)10000.0f` / `10000u` | 5766 | 17172 |
| `10000 + …` (constant first) | 5766 | 17172 |
| `Vector pos(a,b,c)` / `Vector pos = Vector(a,b,c)` | 5766 | 17172 |
| `Vector pos; pos.x = …` (component assignment) | 5766 | 17172 |
| `this->enemies->data_[i]` | 5766 | 17172 |
| `Vector pirateBase(0,0,0)` / `= Vector(0,0,0)` | 5766 | 17172 |
| `escortsEnd` in all 6 `a+b+c` orders | 5766 (2 orders 17168) | 17172 |
| `unsigned i` / `unsigned escortsEnd` | 5766 | 17172 |
| `nextInt(2) != 0 ? -1 : 1` | 5766 | 17172 |
| reverse component order (z,y,x) / (y,x,z) | 5766 | 17172 |
| `int dx,dy,dz` temps then `Vector pos` | 5774 | 17172 |
| `int off[3]` then `Vector pos` | 5774 | 17172 |
| `float px,py,pz` locals | 5779 | 17172 |
| `Vector base = pirateBase` copy | 5779 | 17172 |
| `Vector pos = pirateBase; pos.x += …` | 5779 | 17172 |
| `KIPlayer *ship` for setToSleep/field only | 5766 | 17172 |
| `KIPlayer *ship` for all three calls | 5917 | 17156 |
| `Array<KIPlayer*> *enemies` cached before loop | 15621 | 17232 |
| `Vector pos` before `createShip` | 16512 | 17336 |
| inline `setPosition((int)(…))` args (no `pos`) | 5887 | 17176 |
| lambda `jitter(20000)` | 16356 | 17208 |
| `Vector delta` + `pirateBase + delta` | 16526 | 17348 |
| loop over components `for c<3` | 15713 | 17208 |
| `escortsEnd = full bound` (5 terms) | 5918 | 17164 |
| `int raid` instead of `bool raid` | 5993 | 17172 |

The compiler canonicalises every spelling to the same MIR before RA, so the
RA result is unchanged.  Reordering the case-183 convoy switch (wave-138
landed `default`-first vs ascending 0,1,2) also leaves the escort loop's
`mov fp,sl` untouched (it changes only the case-183 block; raw diff
5766 -> 5763, official fuzzy 92.5 -> 92.3, so not landed).

## Conclusion (no source win this wave)

The escort-loop residual is a **Greedy-RA tie-break**, not a source-order
bug.  The literal is loop-invariant and already in the preheader before RA
(`%3522` in `bb.276`); Greedy RA splits it at the loop body and
rematerialises it because the loop needs all seven allocatable callee-saved
registers and the literal's spill weight beats `this`'s.  No source spelling
of the `Vector pos` initializer, the `10000` literal, the `escortsEnd`
computation, the loop variable, or the `this->enemies` accesses moves the
literal back into `fp`; the MIR is identical for all 43 variants.  The
`movw fp,#10000` preheader form is only reachable by changing the RA's
register choice, which is a backend decision.

Scratch: `/dev/shm/w8412/{Level.base.cpp,fdiff.py,secdiff.py,regiondiff.py,
try.py,orig_fn.asm,ours_fn.asm,mir.txt,mir_coal.txt,mir_gra.txt,mir_vrr.txt,
v/*.txt}`.

## Gate counts (unchanged; note-only commit)

```text
byte_exact      4507 -> 4507   (4507/4524)
linked_exact    4507 -> 4507   (4507/4524)
size_exact      4524 -> 4524
data_exact       843 -> 843
imports          207 -> 207
lint            CLEAN   -> CLEAN
drift           0       -> 0
RATCHET         PASS    -> PASS
.text diff      85,476 -> 85,476
.ARM.extab      1,396  -> 1,396
.rel.plt        0      -> 0
.note.gnu.build-id 20  -> 20
```
