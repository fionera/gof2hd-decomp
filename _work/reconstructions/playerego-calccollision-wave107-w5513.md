# PlayerEgo::calcCollision — wave 107 (w5513): the guard-pointer spill
# position is the last fixed point; ~700 more variants, none beat 87.6437 %

**Status:** research note, 2026-11-04. Worktree `wt/w5513`, branch
`wave107-w5513`, base `f857b5171`. **No source change landed** — the
checked-in 87.6 % / 968 B shape is still the best measured. This note adds
the *exact* (unrounded) score of the landed shape, a fast single-function
exact scorer, and the wave-107 measurement of the one remaining lever:
the `__stack_chk_guard` pointer's spill position.

Target: `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE`
(orig 968 B, 347 normalised instructions, 87.6 %).

## TL;DR

* The landed two-anchor body scores **87.64367816091954 %** exactly
  (verify's `pct` is rounded to one decimal, so this is the true fixed
  point).  Any improvement therefore needs to round to 87.7 % or more,
  i.e. at least one more matching normalised instruction.
* The **natural** source shape (no anchors, real `i == 0 && field_0x71`
  guard, block-scoped `Vector surface`) reproduces the original's block
  layout *and* the original's `dock`-spilled-at-def / `i=fp` register
  assignment, but permutes the last three callee-saved values:
  `this=sl, surface=r5, guard=r6` instead of `this=r5, surface=r6,
  guard=spilled`.
* The permutation is caused by the **guard pointer's spill position**:
  in the natural shape `&__stack_chk_guard` is loaded into `r6` in the
  prologue and only spilled in the preheader (`strd r6,r9,[sp,#20]`),
  so `r6` is still occupied when `surface` is defined and `surface`
  falls to `r5`; in the original it is spilled *at its def*
  (`str.w r9,[sp,#24]`), freeing `r6` for `surface`.
* The wrap shape (`if (i < objects->size())` after `outerCollide`) gives
  the original's un-rotated layout and, with the A2 anchor, the original
  register file — but is 8 B too long (976 B, 85.5 %).
* ~700 new variants (anchor forms/positions, guard forms/positions,
  surface forms, loop forms, hoists, pre-loop checks, branch hints,
  arm order, `__builtin_expect`, extra `dockOffsetVec` uses) produce
  nothing above 87.6437 %.

## 1. Exact score and the true remaining diff

`tools/verify/verify.py` rounds `pct` to one decimal
(`verify.py:431`).  Re-running `verify.main()` with an unrounded
`score()` gives the landed body's exact ratio:

```
LENS 347 349 RATIO 87.64367816091954
```

The normalised diff (original `-`, landed `+`, branch targets
`loc_*`):

```
delete   35 36 : - str.w r9, [sp, #20]          <- preheader dock spill
replace  45 46 : - beq.w loc_2c0 / + beq.n loc_c6
replace  47 48 : - bne.n loc_ae  / + bne.n loc_a8
... (branch-target shifts: loc_2c0 vs loc_c6, loc_ae vs loc_a8, ...)
replace  70 72 : - cmp r0,#0; - beq.w loc_2c0
                 + cbz loc_c6; + ldr r0,[r4]; + subs r0,#1;
                 + cmp fp,r0; + bls.n loc_cc;
                 + add.w fp,fp,#1; + b.n loc_64      <- A1 + rotated latch
replace  84 85 : - bne.w loc_2c0 / + bne.n loc_c6
replace  87 88 : - bne.w loc_2c0 / + bne.n loc_c6
                 + ldr r0,[r4]; + cmp fp,r0; + beq.n loc_c6   <- A2
insert   90 90 : + str.w r9, [sp, #20]          <- lazy dock spill
replace 205 208: - mov sl,r5; - mov r5,r6; - add r6,sp,#96
                 + add.w sl,sp,#96               <- collision-arm shuffle
replace 209 210: - mov r2,r5 / + mov r2,r6
replace 211 212: - mov r0,r6 / + mov r0,sl
insert  214 214: + mov r1,sl
delete  217 220: - mov r6,r5; - mov r5,sl; - mov r1,r6
delete  246 248: - cmp.w fp,#0; - bne.n loc_2ba  <- missing bottom guard
replace 254 256: - add.w fp,fp,#1; - b.n loc_68 / + b.n loc_c6
```

The landed body is 968 B despite the 5 A1 + 3 A2 instructions because
the rotated latch (`loc_c6`) keeps every `continue` target close, so
clang emits `beq.n`/`bne.n` (2 B) where the original's end-of-body latch
forces `beq.w`/`bne.w` (4 B).

## 2. New this wave: the natural shape is one spill away

The natural shape (`body_F4`, no anchors, real guard, block-scoped
`Vector surface`) is 994 B / 56.8 % (fast) but has the right *structure*:

| | latch | frame | this | surface | object | dock | i | guard |
|---|---|---:|---|---|---|---|---|---|
| original | end (0x2c0) | 112 | r5 | r6 | r8 | spilled@def (sp+20) | fp | spilled@def (sp+24) |
| **F4 (natural)** | end | 112 | **sl** | **r5** | r8 | spilled@preheader (sp+24) | fp | **r6 → spilled@preheader (sp+20)** |
| landed (A1+A2) | rotated (0xc6) | 112 | r5 | r6 | r8 | r9 | fp | spilled@def (sp+24) |

F4's prologue is

```
0e: mov  sl, r0          ; this
18: ldr  r6, [r0]        ; &__stack_chk_guard
1a: ldr  r0, [r6]        ; canary
1c: str  r0, [sp, #108]
...
4a: add  r5, sp, #32     ; surface = sp+32   <- r6 still occupied
62: strd r6, r9, [sp, #20]  ; guard -> sp+20, dock -> sp+24
```

whereas the original's is

```
9f0da: mov  r5, r0
9f0e4: ldr  r0, [r0]     ; &__stack_chk_guard
9f0e6: str  r0, [sp, #24]   ; guard spilled AT DEF
9f0e8: ldr  r0, [r0]     ; canary
9f0ea: str  r0, [sp, #108]
...
9f110: add.w r9, r5, #328   ; dock
9f118: add  r6, sp, #32     ; surface = sp+32   <- r6 free
9f12a: str.w r9, [sp, #20]  ; dock spilled at def
```

So the whole `this/surface/guard` permutation is a *single* allocator
decision: whether `&__stack_chk_guard` is spilled at its def or at the
preheader.  Once it is spilled at def, `r6` is free for `surface`,
`this` keeps `r5`, and the collision-arm shuffle (`mov sl,r5;
mov r5,r6; add r6,sp,#96; ...`) appears automatically.

The landed body gets the same effect from its anchor latch block (the
anchors create a second latch, which raises prologue pressure enough to
spill the guard at def) — but pays for it with the rotated latch and the
two redundant size checks.

## 3. The wrap shape: right layout + right registers, 8 B too long

`body_Y1` = landed body with A1 removed and `if (i < objects->size())`
wrapped around the wormhole/steal/collision arms (A2 kept).  It gives
the original's register file *and* a near-original layout:

| variant | pct | size | latch | this | dock | i | note |
|---|---:|---:|---|---|---|---|---|
| landed (A1+A2) | **87.6** | 968 | loc_c6 (rotated) | r5 | r9 | fp | best |
| wrap only (`Y2`) | 87.3 | 968 | loc_2c0 | r5 | **fp** | **r9** | right layout, swapped pair |
| **wrap + A2 (`Y1`)** | 85.5 | 976 | loc_2c8 | r5 | **r9** | **fp** | right layout + pair, 8 B long |
| wrap + A2 + guard | 84.5 | 984 | loc_2c8 | r5 | r9 | fp | |
| natural (`F4`) | 56.8 | 994 | end | sl | spilled | fp | right layout + dock, wrong this/surface |

`Y1`'s exact diff is the same *shape* as the original's: every branch
target is `loc_2c8` vs the original's `loc_2c0` (an 8-byte shift), and
the only extra instructions are the wrap check (`ldr; cmp; bcs.w`, 8 B)
and A2 (`ldr; cmp; beq.w`, 8 B).  It is missing only the preheader dock
spill (4 B), the bottom guard (4 B) and the collision shuffle (6 B).
Removing either the wrap check (→ `Y2`, dock/i swap) or A2 (→ `Y2`,
dock/i swap) loses the register file.

## 4. Measured this wave (fast `q.py` score; landed = 87.2 fast / 87.6437 exact)

All ≤ landed unless noted.

| family | variants | best | note |
|---|---|---:|---|
| anchor form swap (A1↔A2) | 6 | 87.2 | `R1` is codegen-identical to landed |
| anchor position (top/null/alien/wormhole/steal/collision/end) | 15 | 87.2 | only the landed positions keep `dock=r9,i=fp` |
| guard position (inner/outer/before-matrix/after-hitCamera) | 8 | 87.2 | guard outside block without `i==0` is identical |
| guard form (`i==0`, `!i`, field-first, nested) | 5 | 87.2 | `i==0` costs 8 B → 85.0 |
| surface form (block/none/named/`bool hit`/`int hit`/`!=0`) | 12 | 87.2 | all identical except loop-scope (frame 120) |
| loop decl (`i` predeclared, `int`, `while`, `do`, `++i`, `i+=1`) | 8 | 87.2 | `while`/`do` worse |
| hoists (`pull`/`drift`/`dock` ref/ptr, all) | 8 | 87.2 | identical |
| pre-loop checks (`size==0`, `size!=0`, cached `n`) | 8 | 87.2 | cached `n` frame 120 |
| `__builtin_expect` (null/A1/A2/outerCollide/isVisible) | 6 | 87.2 | A1 hint re-rotates |
| arm order (collision before wormhole) | 4 | 87.2 | identical |
| strength/playing branch order | 6 | 87.2 | `strength<1` first 86.5 |
| extra `dockOffsetVec` use (x/y/z, guard/wormhole condition) | 8 | 86.9 | flips dock in wrap shape but costs 16 B |
| random 12-dimension sweep (surf×guard×A1×A2×null×outer×zero×strength×playing×hoist×preloop×armorder) | 400 | 86.9 | no hit ≥ landed |

The fast screen (`q.py`, clang `-c` only) is ~0.5 s/variant; the exact
single-function scorer (`fexact.py`) is ~1 s; the full `t.py` link +
verify is ~20 s.

## 5. Proven vs. hypothesis

**Proven (measured):**

* The landed body's exact score is 87.64367816091954 % (347 vs 349
  normalised instructions, 968 B).
* The natural shape (no anchors + real guard) reproduces the original's
  frame (112), un-rotated latch, `dock`-spilled-at-def and `i=fp`, but
  gives `this=sl, surface=r5, guard=r6`.
* The permutation is caused by `&__stack_chk_guard` being kept in `r6`
  until the preheader in the natural shape, versus spilled at its def in
  the original.  `r6` is therefore unavailable for `surface`.
* The wrap shape (`if (i < objects->size())` after `outerCollide`) gives
  the un-rotated layout; adding A2 gives the original register file; the
  pair is 8 B too long (976 B, 85.5 %).
* ~700 new builds (including a 400-sample random sweep) do not beat
  87.6437 %.

**Hypothesis (not proven):**

* The original source is the natural (REA/oracle) shape compiled with
  the guard pointer spilled at def.  That single allocator decision
  would simultaneously free `r6` for `surface`, keep `this` in `r5`,
  produce the collision-arm shuffle, and drop the preheader `strd` in
  favour of `str.w r9,[sp,#20]` — i.e. the original's exact code.
* No source-level construct found so far forces that spill position
  without also changing the block layout (the anchors do it via the
  second latch, at the cost of the rotation).  A construct that raises
  prologue register pressure *without* adding a block — e.g. an extra
  address-taken local whose lifetime ends before the loop — is the most
  promising untried lever.

## 6. Reproduction / harness

```sh
cd /home/debian/GalaxyOnFire2/wt/w5513
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6          # RATCHET PASS
# fast screen (splice + clang -c + local score):
python3 /dev/shm/w5513/q.py body_X.txt
# full link + official verify:
python3 /dev/shm/w5513/t.py body_X.txt
# exact unrounded score of just this function:
python3 /dev/shm/w5513/fexact.py
# exact normalised diff (original vs ours):
python3 /dev/shm/w5513/vdiff2.py
# MIR / regalloc dumps:
bash /dev/shm/w5513/mir.sh /dev/shm/w5513/paa.txt
python3 /dev/shm/w5513/emit.py body_X.txt x.ll
python3 /dev/shm/w5513/extract_fn2.py x.ll x_fn.ll
/dev/shm/wf110/llvm702build/bin/llc -march=arm -mcpu=cortex-a8 \
  -mattr=+thumb2 -O2 -debug-only=regalloc x_fn.ll -o /dev/null
```

Scratch (not committed): `/dev/shm/w5513/` — `body_*.txt` variants,
`gen3.py`/`gen4.py`/`gen5.py` generators, `q.py`, `t.py`, `fexact.py`,
`vdiff2.py`, `emit.py`, `extract_fn2.py`, `mir.sh`, `orig_dis.txt`,
`fast_fn.s`, `paa_f4.txt`, `lia_base_dbg.txt`.

## 7. Gate

Unchanged: `byte_exact 4500 / linked_exact 4500 / size-exact 4524 /
data_exact 843 / imports 207`, lint `CLEAN`, drift 0.
`RATCHET PASS`.  No source change, so `tools/verify_baseline.json` is
untouched.
