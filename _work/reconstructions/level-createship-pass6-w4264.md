# Level::createShip — pass 6 (wave 92, w4264)

**Status:** analysis only, no source change landed (2026-11-03). Worktree
`wt/w4264`, branch `wave92-w4264`, base `b379d00d`. Target:
`_ZN5Level10createShipEiiiP8Waypointbb` (original 2764 B @ `0xc0214`,
Thumb entry `0xc0215`). Current verifier score **74.1439205955335 %**
(orig 1007 insns, ours 1008, osize 2764, usize 2764, `byte False`).

Gate before and after (unchanged, no code change):

```text
byte_exact 4499/4524  linked_exact 4499/4524  size-exact 4524/4524
stub_zero_size 0  missing 0  wrong_type 0  extra 0  data_exact 843/843
imports 207  .text 0x1337c8  .rodata 0x41b67  lint CLEAN ✓  drift 0
RATCHET PASS
```

This pass continues `level-createship-pass5-w4242.md`. It (a) corrects a
target-detection bug in the pass-5 scratch harness, (b) re-derives the
exact `StackSlotColoring` colour permutation from a fresh
`-mllvm -print-after-all` dump of the committed build and of the
all-float control, (c) re-runs the declaration/assignment/type sweeps
with the corrected target over **41 472** source variants, and (d)
shows that the target slot map and the target prologue are mutually
exclusive in every faithful family swept. No source shape beats
74.1439 %.

---

## 1. The target slot map (authoritative, ELF disasm)

Original spawn (`objdump -d`, `0xc03c8`..`0xc03ee`):

```text
c03c8  ldr  r2,[sp,#48]   ; B = waypoint->y
c03ca  ldr  r3,[sp,#32]   ; 2nd rnd call result
c03cc  ldr  r0,[sp,#44]   ; A = waypoint->x
c03ce  ldr  r1,[sp,#36]   ; 1st rnd call result
c03d0  add  r2,r3         ; B + 2nd
c03d2  ldr  r3,[sp,#40]   ; C = waypoint->z
c03d4  ldr  r4,[sp,#28]   ; 3rd rnd call result
c03d6  add  r0,r1         ; A + 1st
c03d8  movw r1,#20000
c03dc  sub.w fp,r0,r1     ; x (int)
c03e0  add  r3,r4         ; C + 3rd
c03e2  subs r4,r2,r1      ; y (int)
c03e4  sub.w r8,r3,r1     ; z (int)
c03e8  mov  r0,r5
c03ea  mov  r1,sl
c03ec  mov  r2,r9
c03ee  blx  setEmpData
```

The three rnd calls store their results in call order to `[sp,#36]`,
`[sp,#32]`, `[sp,#28]`. The spawn then uses 1st for `x`, 2nd for `y`,
3rd for `z`. Therefore the byte-matching target, expressed as the slot
written by each rnd call in *call order*, is

```text
jit slots (call order) = [36, 32, 28]
waypoint fields        = {292: 44, 296: 48, 300: 40}
```

**Harness bug corrected.** The pass-5 scratch `fast.py` used
`TARGET_JIT=[32,28,36]`, a rotation of the true target. Every pass-5
sweep therefore reported `target hits 0` even when the correct slot
triple was produced. With `TARGET_JIT=[36,32,28]` the sweeps below find
hundreds of hits (but none that keep the prologue).

Committed build spawn (`Level.cpp.obj`, committed source):

```text
1b4  ldr  r0,[sp,#44]   ; A
1b6  movw r4,#20000
1ba  ldr  r1,[sp,#36]   ; 2nd rnd (jitterZ in the committed call order)
1bc  ldr  r2,[sp,#40]   ; 1st rnd (jitterY)
1be  add  r0,r1         ; z = A + jitterZ
1c0  ldr  r1,[sp,#48]   ; B
1c2  subs r0,r0,r4      ; z (float)
1c4  add  r1,r2         ; y = B + jitterY
1c6  subs r1,r1,r4      ; y (float)
1c8  vmov s0,r1 / 1cc vcvt s18   ; converted BEFORE the call
1d0  vmov s0,r0 / 1d4 vcvt s16
1d8  mov  r0,r5 / 1da mov r1,sl / 1dc mov r2,r9
1de  bl   setEmpData
1e2  ldrd r1,r0,[sp,#28] ; 3rd rnd, C
1e6  cmp  r6,#0
1e8  ldr.w sl,[sp,#56]
1ec  add  r0,r1          ; x = C + jitterX
1ee  sub.w r4,r0,r4      ; x (int, converted later at the ctor)
```

Slot table (semantic coordinate -> final `[sp,#N]`):

| value | original | committed |
|---|---|---|
| `waypoint->y` (`B`) | 48 | 48 |
| `waypoint->x` (`A`) | 44 | 44 |
| `waypoint->z` (`C`) | **40** | **32** |
| 1st rnd / `jitterX` | **36** | **28** |
| 2nd rnd / `jitterY` | **32** | **40** |
| 3rd rnd / `jitterZ` | **28** | **36** |

The first five colours (`race`, `this`, `type`, `B`, `A`) match; the
last four are the 4-cycle `(C, 1st, 2nd, 3rd) -> (2nd, 3rd, C, 1st)`.

## 2. Exact `StackSlotColoring` mechanism (fresh dump)

`-mllvm -print-after-all` was run on the committed `Level.cpp` with the
exact `compile_commands.json` flags (dump `/dev/shm/w4264/pa_base.txt`,
885 MB) and, for the all-float control, on a one-function TU
(`/dev/shm/w4264/pa_fff.txt`, 30 MB). The relevant passes are
`# *** IR Dump After Greedy Register Allocator ***` and
`# *** IR Dump After Stack Slot Coloring ***`.

### 2.1 Committed build

Pre-colouring frame objects and the `%stack.N` stores in the
waypoint/jitter blocks (`gra_base.txt`):

```text
bb.1 (else)  mov 0 -> stack.4, stack.5, stack.3
bb.2 (if)    ldr [wp+292] -> stack.5   (A)
             ldr [wp+296] -> stack.4   (B)
             ldr [wp+300] -> stack.3   (C)
bb.3         rnd 1 -> stack.7, rnd 2 -> stack.8, rnd 3 -> stack.6
bb.23        ldr stack.5, stack.8      (z = A + rnd2)
             ldr stack.4, stack.7      (y = B + rnd1)
bb.26        ldr stack.3, stack.6      (x = C + rnd3)
```

So the **FI creation order** is `stack.3=C, stack.4=B, stack.5=A,
stack.6=jitterX, stack.7=jitterY, stack.8=jitterZ`.

Post-colouring (`ssc_base.txt`), the `DBG_VALUE`s name the coloured
slots:

```text
stack.0 = race   stack.3 = wy (B)   stack.5 = jitterY
stack.1 = this   stack.4 = wz (A)   stack.6 = jitterZ
stack.2 = type   stack.7 = wx (C)   stack.8 = jitterX
```

`PrologEpilogInserter` lays the nine 4-byte objects out at
`[sp, 60-4k]` (frame: `push {r4,r5,r6,r7,lr}` 20 + `stmdb
{r7,r8,r9,sl,fp}` 20 + `vpush {d0-d10}` 88 = 128), so the final colour
order is

```text
committed: race, this, type, B, A, jitterY, jitterZ, C, jitterX
target   : race, this, type, B, A, C, jitterX, jitterY, jitterZ
```

### 2.2 Why the committed order is `B,A,jY,jZ,C,jX`

`StackSlotColoring::ScanForSpillSlotRefs` adds
`LiveIntervals::getSpillWeight(false, true, MBFI, MI)` for every
non-debug FI reference; in LLVM 7.0 that is
`Freq.getFrequency() * (1.0f / MBFI->getEntryFreq())`
(`LiveIntervals.cpp:860-866`). `InitializeSlots` then `std::stable_sort`s
by descending `li.weight`, and `ColorSlot` assigns the lowest free
colour (`ssc.cpp:246`, `ssc.cpp:269`).

All six spawn values have a store in a block with the entry frequency
(`bb.1`+`bb.2` sum to the entry frequency; `bb.3` equals it), so the
store term is 1.0 for each. The load term differs:

```text
C, jitterX : load in bb.26   weight = 1 + f(bb.26)
B, A, jY, jZ : load in bb.23 weight = 1 + f(bb.23)
```

`bb.23` ends in an invoke: `successors: %bb.26(0x7ffff800),
%bb.25(0x00000800)`, so `f(bb.26) = f(bb.23) * (1 - 2^-20)` — a
difference of ~1e-6 that survives in float32. Hence
`B,A,jY,jZ > C,jX`, the stable sort keeps the FI order inside each
group, and the colour order is `B,A,jY,jZ,C,jX`. This reproduces the
observed table exactly.

The original has **no invoke**: all six spawn values are used in one
block, all six weights tie, and the colour order collapses to the FI
creation order. The original's FI creation order is therefore the
target `B,A,C,jX,jY,jZ`.

### 2.3 The all-float control's order

For the `fff` control (all three coordinates `float`, same `try`), the
fresh dump gives:

```text
GRA:  stack.3=B, stack.4=A, stack.5=C, stack.6=jitterZ,
      stack.7=jitterY, stack.8=jitterX
SSC:  stack.0=type, stack.1=race, stack.2=this,
      stack.3=B, stack.4=A, stack.5=C,
      stack.6=jitterZ, stack.7=jitterY, stack.8=jitterX
```

The spawn slots are exactly the target (`B=3, A=4, C=5`, rnd calls in
order `6,7,8`), but the first three colours are `type,race,this`
instead of `race,this,type`, so the prologue becomes
`race->[sp,#56], type->sl, shipClass->r6` and the score drops to
64.76 %.

## 3. Sweeps with the corrected target

All variants are full `Level.cpp` rewrites compiled with the exact
`compile_commands.json` command. Slot maps are parsed from the object;
headline scores use the official verifier (`autojunk=False`). A
mini-link proxy (`Level.o` alone, `--unresolved-symbols=ignore-all`)
was used only for ranking; it reproduces the full score ordering
(baseline 66.997 vs full 74.1439, `iii` 62.911 vs 70.4885, `fff`
57.758 vs 64.7648).

### 3.1 All type patterns, committed field mapping (20 736 variants)

Dimensions: 8 `int/float` patterns for `(x,z,y)`, 6 waypoint
declaration orders, 6 waypoint assignment orders, 6 jitter call orders,
6 spawn orders, `implicit` vs `float_after` conversion.

```text
variants 20736   best = committed iff (int x, float z, float y),
                 jitter order jitterY,jitterZ,jitterX,
                 spawn order x,z,y  ->  66.9975 mini / 74.1439 full
```

No variant beats the committed shape. The next-best distinct shape is
66.9315 mini (1007 insns).

### 3.2 Faithful field mapping (20 736 variants)

Field mapping `wx=waypoint->x, wy=waypoint->y, wz=waypoint->z` (the
Ghidra names), all 8 type patterns, all declaration/assignment/jitter/
spawn orders, both conversion modes:

```text
variants 20736   target-slot hits 360   of which prologue-preserving 0
```

All 360 target hits are the all-float pattern (`fff`) and all lose the
prologue (`race60=False`). The faithful mixed pattern (`int x`,
`float z`, `float y`) never reaches `C=40`.

### 3.3 All-int family (15 552 variants)

All three coordinates `int`, both conversion modes, all orders and both
field-mapping conventions:

```text
target hits: many, all with race60=False / type_fp=False / class_r5=False
```

The best-scoring all-int shape is the pass-5 `iii` control:
**70.4885 %**, 999 insns, 2772 B, target slots
`{A:44,B:48,C:40}` / `[36,32,28]`, but `type->sl` instead of `type->fp`
and `strd` instead of two `str`s. Its diff is dominated by the
`fp`<->`sl` swap (`mov fp,r3` vs `mov sl,r3`, `cmp.w fp,#51` vs
`cmp.w sl,#51`, ...), which costs ~4 points.

### 3.4 The target map and the prologue are mutually exclusive

Combining §3.1–§3.3: in every family swept, the shapes that produce
the target slot map (`fff`, and `iii` with `float_after`) lose
`type->fp`; the shapes that keep `type->fp` (`iff`, the committed
shape) put `C` in slot 32. The reason is visible in §2: the committed
`iff` shape sinks the `int x` computation past the invoke (two-block
spawn, unequal weights), while the single-block shapes that tie the
weights (`fff`, `iii`) raise register pressure enough to move `type`
out of `fp`.

## 4. What was tried and rejected

| variant | score | note |
|---|---|---|
| committed `iff` (baseline) | **74.1439** | 2764 B, 1008 insns |
| `PlayerFixedObject(race, type, ...)` | 74.2432 | semantic swap, reverted by `94e8bc54` |
| `iii` + `float_after` (pass 5) | 70.4885 | target slots, `type->sl` |
| `fff` (all float) | 64.7648 | target slots, wrong prologue |
| `(void)(x+y+z)` before the try | 64.8649 | target slots, prologue lost |
| faithful `iff` (jitter X,Y,Z) | 72.5919 | pass-5 measurement |
| faithful `iff` (jitter Y,Z,X) | 72.6912 | pass-5 measurement |

The only score-positive shape (74.2432) is the semantic
`PlayerFixedObject(race, type)` swap. The original disasm at `0xc0426`
is `ldr r4,[sp,#52]` (kind/type) and `0xc042a ldr r6,[sp,#60]`
(param2/race), then `0xc0432 mov r1,r4`, `0xc0438 mov r2,r6`, i.e. the
committed `(type, race)` order is semantically correct; the 74.2432
gain is a register-allocation coincidence (`mov r1,r4` vs
`ldr r1,[sp,#52]`) and was deliberately reverted.

## 5. Conclusion

The target slot map is reachable, but only in source shapes that lose
the `type->fp` prologue slot; every shape that keeps the prologue
leaves `waypoint->z` in slot 32. No faithful C++ shape found beats
**74.1439 %**. The remaining structural mismatch is the spawn region
(13 instructions: all three ints before the invoke, all three
`vcvt.f32.s32` after it, duplicated in both `shipClass` branches) plus
the downstream register allocation it drives (`fixed` in `r9` vs
`fp`, `volumes` in `sl` vs `r9`, `x/y/z` floats in `sl/r8/r4` vs
`r4/s18/s16`).

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w4264
cmake --build cmake-build-match --target check -j8
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5Level10createShipEiiiP8Waypointbb
```

Scratch helpers (all under `/dev/shm/w4264`): `fast.py` (compile one
variant and parse slots; `TARGET_JIT=[36,32,28]`), `harness.py`,
`swscore.py` (mini-link + official-ratio scorer), `scoreall.py`
(20 736-variant all-pattern sweep), `final_sweep.py` (20 736-variant
faithful-mapping sweep), `iii_sweep.py` (15 552-variant all-int sweep),
`pa_base.txt` / `gra_base.txt` / `ssc_base.txt` (committed
`-print-after-all` dumps), `pa_fff.txt` / `gra_fff.txt` / `ssc_fff.txt`
(all-float control dumps).
