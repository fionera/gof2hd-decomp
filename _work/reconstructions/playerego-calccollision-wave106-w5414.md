# PlayerEgo::calcCollision — wave 106 (w5414): exhaustive source-shape
# search confirms the 87.6 % / 968 B two-anchor body is a local optimum

**Status:** research note, 2026-11-04. Worktree `wt/w5414`, branch
`wave106-w5414`, base `486f8a119`. **No source change landed** — the
checked-in 87.6 % / 968 B shape is still the best measured of ~800
compiled variants. This note records the exhaustive search, the exact
remaining diff, and the one new structural finding: the two attractors
(right register file vs. right block layout + real guard) are mutually
exclusive from source in this clang 7.0.2 `-Oz` build, and the
checked-in A1+A2 anchors buy the register file for exactly the 14 bytes
that the original spends on the bottom guard + preheader dock spill.

Target: `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE`
(orig 968 B, 347 instructions, 87.6 %).

## TL;DR

* The checked-in body (`A1` = `if (i > objects->size() - 1) continue;`
  after `outerCollide`, `A2` = `if (objects->size() == i) continue;` in
  the wormhole arm) is the **unique** measured fixed point at
  87.6 % / 968 B with the original register file
  (`this=r5 objects=r4 object=r8 dock=r9 i=fp surface=r6`, frame 112).
* A full cross-product of 576 source variants (A1 form × A2 form × guard
  × surface scope × zero-compare order × strength-branch form) plus ~200
  targeted probes produced **no score above 87.6 %**. The four 87.6 %
  variants are codegen-identical to the checked-in body.
* The nearest *structurally* faithful shape is `wrap + A2 + guard`
  (`if (i < objects->size()) { ... }` after `outerCollide`, plus the real
  `if (i == 0 && object->field_0x71)` bottom guard): **85.7 % / 984 B**,
  un-rotated latch, original register file, original branch widths — but
  16 B too long because the wrap check and A2 are redundant
  `objects->size()` loads the original does not contain.
* The nearest *guard-bearing, size-near* shape is `A1c + A2 + guard`
  (`A1c` = `if (i == objects->size()) continue;`): **85.2 % / 972 B**,
  original register file, real guard, but rotated latch (so all
  `.n`/`.w` branch widths differ) and one extra lazy dock spill (4 B).

## 1. Original binary facts (re-confirmed this wave)

Manual Thumb disassembly of `android_2.0.16_libgof2hdaa.so`
`0x9f0cc`–`0x9f492`:

* preheader: `0x9f110 add.w r9,r5,#328` (dock), `0x9f12a str.w r9,[sp,#20]`
  (spill at def), `0x9f118 add r6,sp,#32` (surface), canary pointer
  spilled to `[sp,#24]` at `0x9f0e6`, frame 112.
* top-tested loop, header `0x9f134`, latch `0x9f38c`
  (`add.w fp,fp,#1; b.n 0x9f134`). The latch is at the **end** of the hot
  body (un-rotated); the wormhole cold blocks (`0x9f392`–`0x9f450`) sit
  after it, then the exit at `0x9f452`.
* register file `this=r5 objects=r4 object=r8 dock=r9 i=fp surface=r6`;
  `sl` is free scratch.
* two `cmp fp,#0`: `0x9f14a` (alien-orbit condition) and `0x9f374`
  (the real bottom guard `if (i == 0 && object->field_0x71)`).
* collision-arm register shuffle is real:
  `0x9f30c mov sl,r5; 0x9f30e mov r5,r6; 0x9f310 add r6,sp,#96; …
  0x9f326 mov r6,r5; 0x9f328 mov r5,sl; 0x9f32a mov r1,r6`.
* `operator!=` in the collision arm is called as `(temp, zero)`
  (`r0=&temp, r1=surface-slot-zero`), i.e. source
  `if (collision != Vector{0,0,0})` — the checked-in body writes
  `Vector{0,0,0} != collision`; both compile to the same instruction
  sequence in the checked-in shape (measured 87.6 vs 87.4).
* literal pool order: `16000.0f`, `0.2f`, `0.1f`, `0.2f`, then the GOT
  slots `__stack_chk_guard`, `Globals::status`, `Globals::sound`,
  `Globals::Canvas` ×2.
* only one `objects->size()` load per iteration (in the header); the
  original contains **no** redundant size check.

## 2. Measured table (all via `link_so` + `tools/verify` score)

| variant | source shape | pct | size | insns | this | dock | i | surf | latch |
|---|---|---:|---:|---:|---|---|---|---|---|
| **checked-in** | A1+A2, no guard, braces | **87.6** | **968** | **349** | r5 | **r9** | **fp** | r6 | rotated |
| wrap + A2 + guard | un-rotated, real guard | 85.7 | 984 | 351 | r5 | r9 | fp | r6 | end |
| A1c + A2 + guard | `i==size` anchor + guard | 85.2 | 972 | 350 | r5 | r9 | fp | r6 | rotated |
| A1+A2+guard | checked-in + guard | 85.0 | 976 | 352 | r5 | r9 | fp | r6 | rotated |
| wrap only | un-rotated, no guard | 87.3 | 968 | 345 | r5 | fp | r9 | r6 | end |
| wrap + A2 | un-rotated, no guard | 85.5 | 976 | 348 | r5 | r9 | fp | r6 | end |
| A1 only | no guard | 79.5 | 964 | 347 | r5 | fp | r9 | r6 | rotated |
| A2 only | no guard | 70.8 | 984 | 351 | r5 | — | r9 | r6 | rotated |
| no anchors + guard | natural REA shape | 59.8 | 996 | 352 | sl | r9 | fp | r5 | rotated |
| positive-if + guard | `if (outerCollide) { body }` | 64.1 | 968 | 349 | r5 | sl | fp | r6 | end |
| no anchors, no guard, braces | natural | 59.8 | 988 | 349 | sl | r9 | fp | r5 | rotated |
| no anchors, no guard, no braces | natural | 63.8 | 968 | 349 | r5 | sl | fp | r6 | end |

The four 87.6 % variants in the 576-variant sweep are
`A1 ∈ {a,b,c,e}` with `A2 = a`, no guard, braces, zero-first,
`strength >= 1`; they are codegen-identical.

## 3. Why the two attractors are mutually exclusive

The register file is decided by `RAGreedy::enqueue`
(`RegAllocGreedy.cpp:680`, LLVM 7): `Prio = (1u<<29) + LI->getSize()`,
tie-broken by the lower vreg number. `dock` (the `dockOffsetVec` GEP) and
`i` (the induction phi) both span the loop, so whichever has the larger
sum-of-segments is dequeued first and takes `r9`; the other takes `fp`.

Two source levers move the balance:

1. **Block layout / latch placement.** Removing A1 lets
   `MachineBlockPlacement` put the latch at the end (the original's
   layout) and makes every `continue` branch to the far latch a `.w`
   branch, matching the original's branch widths. But the un-rotated
   linearisation leaves `i`'s interval larger, so `i` takes `r9` and
   `dock` takes `fp`.
2. **A second `objects->size()` load.** A2 (or the wrap check) adds a
   redundant size load in the middle of the loop. This is the only
   measured construct that flips `dock` to `r9`; cheap `i`-only anchors
   (`i == 0xFFFFFFFFu`, `i & 0x80000000u`, `i + 1 == 0`, `i >> 31`,
   `i == 0xFFFFFFFEu`) are either folded away or do **not** flip it, and
   a `dock.x`/`dock.z` compare in the alien/bottom condition does not
   flip it either.

The checked-in body uses A1+A2 to get the register file, and pays for it
with a rotated latch (narrow `.n` branches, so the size stays 968) and
the two redundant checks. The original gets the same register file from
the preheader spill-at-def plus the real bottom guard, with no redundant
loads — but no source spelling found so far reproduces that combination.

### Exact remaining diff (checked-in vs original)

Branch targets and PC-relative literals normalised; `TGT` = any address.

```
 ldr r0, [pc, #860]
-str.w r9, [sp, #20]                       <- missing preheader dock spill
 add r0, pc
 ...
 cmp.w r8, #0
-beq.w TGT
+beq.n TGT
 cmp.w fp, #0
 ...
 blx r2
-cmp r0, #0
-beq.w TGT
+cbz r0, TGT                               <- A1 (5 insns)
+ldr r0, [r4, #0]
+subs r0, #1
+cmp fp, r0
+bls.n TGT
+add.w fp, fp, #1                          <- rotated latch
+b.n TGT
 mov r0, r8
 ...
 cmp r0, #0
-bne.w TGT
+bne.n TGT
 ldr.w r0, [r5, #484]
 cmp r0, #0
-bne.w TGT
-ldr.w r0, [r8]
-mov r1, r8
+bne.n TGT
+ldr r0, [r4, #0]                          <- A2 (3 insns)
+cmp fp, r0
+beq.n TGT
+ldr.w r0, [r8]
+mov r1, r8
+str.w r9, [sp, #20]                       <- lazy dock spill
 ...
 blx TGT
 ldr.w r0, [r8]
-mov sl, r5                                <- collision-arm shuffle (4 insns)
-mov r5, r6
-add r6, sp, #96
-mov r1, r8
-mov r2, r5
+add.w sl, sp, #96
+mov r1, r8
+mov r2, r6
 ldr r3, [r0, #88]
-mov r0, r6
+mov r0, sl
 blx r3
 movs r0, #0
+mov r1, sl
 strd r0, r0, [sp, #32]
 str r0, [sp, #40]
 mov r0, r6
-mov r6, r5
-mov r5, sl
-mov r1, r6
 blx TGT
 ...
-cmp.w fp, #0                              <- missing bottom guard (2 insns)
-bne.n TGT
 ldrb.w r0, [r8, #113]
```

The checked-in body is 968 B despite the 5 A1 + 3 A2 instructions
because the rotated latch keeps every `continue` target close, so clang
emits `beq.n`/`bne.n` (2 B) where the original's end-of-body latch forces
`beq.w`/`bne.w` (4 B). The original's un-rotated layout is only reachable
with the wrap, and the wrap's extra `i`/size use is what swaps `dock`/`i`.

## 4. New probes this wave (all no better than 87.6)

* 576-variant cross product: A1 ∈ {none, `i > size-1`, `i == size`,
  `i >= size`, `i+1 > size`, `size == i`} × A2 ∈ {none, `size == i`,
  `i >= size`, `size <= i`, `i+1 > size`, `i == size`} × guard {0,1} ×
  surface scope {braces, no braces} × zero-compare order {0,1} ×
  strength branch {`>=1` first, `<1` first}. Max 87.6 (4 variants).
* surface forms: `Vector surface`, `const Vector &surface`,
  `Vector surface(...)`, `Vector surface; surface = ...`, surface
  hoisted before the loop / to the top of the loop body, positive-if
  with/without braces, braces around the whole body. All ≤ 87.6.
* loop forms: `while (i < size) { …; i++; }`, `while` with increment at
  top, `for` with `++i` / `i += 1`, index types `int`/`long`/`short`/
  `unsigned short`. Only `++i`/`i += 1`/`int`/`long` are identical to the
  checked-in; the rest are worse.
* collision arm: named `position` temp, named `surface2`, reuse of the
  outer `surface`, `Vector collision; collision = ...`, `zero` local,
  component-wise compare, `collision != Vector(0,0,0)`. All ≤ 87.6.
* dock declaration position: `Vector &dock` before the null check,
  between the null and docked checks, before the loop, as a pointer.
  Declaring it before the docked check changes the stack layout
  (dock at `sp+24`, canary ptr at `sp+20`) and drops to 76.7 %.
* dock-interval probes: `dock.x` in the wormhole arm flips `dock` to
  `r9` in the wrap shape but costs 24 B (85.7 %, 992 B); `dock.x` at the
  end of the body / in the collision arm / in the steal arm does not
  flip it.
* structural rewrites: REA `if/else if/else` order, steal-arm without the
  redundant `continue`, `hitCamera` shared, `drift = pull` vs
  `Vector(pull)`, `strength < 1` first, `playing` inverted, `objects->count`
  / `objects->data_[i]` / `objects->data()[i]`, local `Array&` alias.
  All codegen-identical to the checked-in or worse.

## 5. Proven vs. hypothesis

**Proven (measured):**

* The checked-in A1+A2 body is a strict local optimum: 576 exhaustive
  variants + ~200 targeted probes, none > 87.6 %.
* A second `objects->size()` load in the loop is the only construct that
  flips `dock` from `fp` to `r9`; cheap `i`-only anchors do not.
* The un-rotated layout (original branch widths) and the original
  register file are mutually exclusive from source in this build: the
  un-rotated shapes (`wrap`, `wrap+A2`) give `dock=fp` or cost 16 B.
* The closest un-rotated shape (`wrap+A2+guard`, 85.7 %, 984 B) matches
  the original's branch widths and register file exactly; its only extra
  instructions are the wrap check (3) and A2 (3), and it is missing the
  preheader spill (has 2 lazy spills instead).

**Hypothesis (not proven):**

* The original source is the REA nested shape (no redundant size checks)
  compiled with the un-rotated block layout; the preheader spill-at-def
  plus the real bottom guard are what make `dock`'s interval larger than
  `i`'s. Reproducing that from source would require a construct that
  lengthens dock's loop-carried segment without adding a block — a
  `dockOffsetVec` use the scheduler sinks past the wormhole arm, or a
  different `Vector` copy form. `Vector &dock`/`Vector *dock` hoists and
  `dock.x` compares were tried and do not reproduce it.
* If a future wave finds that construct, the target shape is: un-rotated
  latch, no A1/A2, real bottom guard, preheader `str.w r9,[sp,#20]`,
  collision-arm shuffle, frame 112 — i.e. the original's exact block
  layout.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5414
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6          # RATCHET PASS
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE
```

Scratch (not committed): `/dev/shm/w5414/` — `body_*.txt` variant bodies,
`batch/`, `batch2/`, `batch3/` (576 variants), `batch3_results.txt`,
`splice.py`, `measure.sh`, `score1.py`, `regs.py`, `orig_dis.txt`,
`show_cur.txt`, `F0_now.txt`, `F3_now.txt`, `regalloc_cur.txt`,
`regalloc_f0.txt`.

## 7. Gate

Unchanged: `byte_exact 4500 / linked_exact 4500 / size-exact 4524 /
data_exact 843 / imports 207`, lint `CLEAN`, drift 0. `RATCHET PASS`.
No source change, so `tools/verify_baseline.json` is untouched.
