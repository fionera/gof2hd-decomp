# w11009 (wave 164): `Level::createMission` — the `and`-operand gate is
# `SelectionDAGBuilder::FindMergedConditions`, not `Reassociate`
# (measured; no size-exact source win landed)

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w11009`, branch
`wave164-w11009`, base `b3d3a1f68`.  Continues
`level-createmission-wave162-w10813.md` and
`level-createmission-wave163-w10912.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, `0x4314` = 17,172 B).  NDK
`arm-linux-androideabi-objdump` is the authority.  Raw/aligned metric
`tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv`.

## Baseline (re-measured, pristine HEAD)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw 5454   align 786
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
```

## 1. Target shape (unchanged ground truth)

```text
b2e04: subs.w r0, fp, r3      ; i - firstIndex
b2e08: mov    r1, r0
b2e0a: it     ne
b2e0c: movne  r1, #1          ; (i != firstIndex)
b2e0e: ldr    r2, [sp, #60]   ; alienWave
b2e10: eor.w  r2, r2, #1      ; !alienWave   <-- INLINE, not hoisted
b2e14: orrs   r1, r2
b2e16: bne.w  b2f2a           ; -> next guard
```

Ours (baseline):

```text
b2e04: ldr r0, [sp, #60]
b2e06: cmp r0, #0
b2e08: beq.w b2f20
b2e0c: cmp fp, r3
b2e0e: bne.w b2f20
```

## 2. Two independent gates, and why `Reassociate` is only the first one

Wave 162/163 stopped at "`Reassociate` sorts the `and` operands, so
`alienWave` ends up operand 0".  That is true but it is *not* the gate
that decides the emitted shape.  There are **two** conditions and both
must hold:

**(a) `Reassociate` pins `and(alienWave_icmp, ifirst_icmp)`.**

`ReassociatePass::canonicalizeOperands` (`Reassociate.cpp:220`) swaps iff
`getRank(RHS) < getRank(LHS)`.  `alienWave`'s `icmp ne i32 %phi, 0`
depends only on a pre-header phi, so its rank is the pre-header block's
`BBRank`; `ifirst`'s `icmp eq i32 %i, %first` depends on the loop-header
induction phi, whose rank is the (later) loop block's `BBRank`.  So the
swap is unconditional for every source spelling of `i == first &&
alienWave`.  Confirmed in `-print-after-all`: the frontend and the first
`SimplifyCFG` both produce `and i1 %ifirst, %alienWave`, and the dump
"After Reassociate expressions" has `and i1 %alienWave, %ifirst`.

**(b) `SelectionDAGBuilder::FindMergedConditions` must NOT split the
`and` into two branches.**

`SelectionDAGBuilder::visitBr` (`SelectionDAGBuilder.cpp:2005`) calls
`FindMergedConditions` for `br i1 (and a, b)` when `!isJumpExpensive() &&
BOp->hasOneUse()`.  `FindMergedConditions` (`:1811`) bails out to
`EmitBranchForMergedCondition` (a **single** branch on the whole `and`)
unless

```cpp
  BOp->getParent() == CurBB->getBasicBlock() &&
  InBlock(BOp->getOperand(0), CurBB->getBasicBlock()) &&
  InBlock(BOp->getOperand(1), CurBB->getBasicBlock())
```

i.e. both operands must be defined in the branch block.  `InBlock` is a
plain `I->getParent() == BB` test.

* When it **splits** (our baseline), it emits operand 0's compare first.
  Operand 0 is `alienWave` ⇒ `cmp r0, #0`.  Then
  `ARMBaseInstrInfo::isProfitableToIfCvt` (`ARMBaseInstrInfo.cpp:1878`)
  hits its `optForSize()` early-out (`t2Bcc` preceded by
  `tCMPi8`/`t2CMPri`, imm 0, low register) and refuses to if-convert the
  second (ifirst) diamond ⇒ the two-branch short-circuit form.
* When it **bails** (single branch on the `and`), `visitSwitchCase`
  inverts the condition because the true block is the fall-through, so
  `not(and a, b)` becomes `or(not a, not b)` in the DAG.  `not(icmp ne
  x, 0)` folds to `xor x, 1` (`eor`) and `not(icmp eq i, first)` folds to
  `icmp ne i, first` (`subs`/`it ne`/`movne`); `orrs` fuses them.  That
  is exactly the original block, with **ifirst first** regardless of what
  `Reassociate` did.

So the operand order that `Reassociate` pins only matters on the *split*
path.  The real lever is making one `and` operand live outside the branch
block.

This is directly reproducible: a 20-line loop reproducer whose
`alienWave` `icmp` lives in the pre-header emits the original
`subs / it ne / movne / ldr / eor / orrs` block; the same reproducer with
the `icmp` in the loop block emits `cmp #0 / beq / cmp / bne`.

## 3. Why our `and` operands are both in the loop block

Pre-`CodeGenPrepare` the guard reuses the pre-header `icmp ne alienWave, 0`
that also feeds the `alienGuns` select:

```text
; <label>:1440:                       ; loop block
  %1441 = icmp eq i32 %1433, %1434    ; i == first
  %1442 = and i1 %1146, %1441         ; %1146 is in the PRE-header
```

After `CodeGenPrepare` the same guard has a **second, duplicated** `icmp`
inside the loop block (`%1378` in the pre-header and `%1722` in the loop
block, identical operands):

```text
; <label>:1721:                       ; loop block
  %1722 = icmp ne i32 %1367, 0
  %1723 = icmp eq i32 %1714, %1715
  %1724 = and i1 %1722, %1723
```

That duplication is what makes `InBlock` true and forces the split.  It is
the only IR change between the two dumps and it is inside
`CodeGenPrepare` (the pass that owns `splitBranchCondition` and the
`optForSize` branch shaping).

## 4. Measured sweep (all size 17,172 B unless stated)

Every row is a real build + `tools/fn_diff.py`.

| candidate (guard) | raw | align | size |
|---|---:|---:|---:|
| baseline `i == first && alienWave` | **5454** | **786** | **17172** |
| `alienWave != 0` / `unsigned alienWave` / `int flag` | 5454 | 786 | 17172 |
| `__builtin_expect(alienWave, 0)` | 16352 | 2352 | 17164 |
| `(alienWave & 1) != 0` / `(alienWave & 1) == 1` | 5465 | 809 | 17172 |
| `alienWave == 1` / `alienWave * 1` (one guard only) | 5465 / 5454 | 809 / 786 | 17172 |
| `(alienWave ^ (i << 1)) & 1` / `(alienWave + (i << 1)) & 1` | 5465 | 809 | 17172 |
| `bool alienWaveZero` + `!flag` | 5465 | 809 | 17172 |
| `(i == first) & (alienWave != 0)` (bitwise) | 13358 | 1388 | 17192 |
| `alienWave && i >= first` / `&& !(i < first)` / `i <= first && i >= first` | 5454 | 786 | 17172 |
| `char alienWave` (either or both) | 12430 | **767** | **17156** |
| `bool alienWave` | 15967 | 3350 | 17168 |
| `volatile int alienWave` (w10912) | 13350 | 3467 | 17116 |
| **`or` form** — pre-loop `bool alienWaveFlag = alienWave != 0` | 12634 | 1727 | **17168** |
| **`or` form** — `alienWave * 1` / `alienWave ^ 0` / `alienWave + 0` on **both** guards | 12634 | 1727 | **17168** |
| **`or` form** — `alienWave & (i \| 1)` | 6973 | 1319 | **17168** |

The above forms only change anything when applied to **both** guards;
applying them to one guard is folded back and reproduces the baseline
exactly.

### The `or` form is reachable and emits the right block

`bool alienWaveFlag = alienWave != 0;` (or any non-`ICmp`/non-`And`
operand, e.g. `alienWave * 1`) makes `InBlock` false, and the guard block
becomes

```text
b2dce: subs.w r0, fp, r3
b2dd2: mov    r1, r0
b2dd4: it     ne
b2dd6: movne  r1, #1
b2dd8: ldr    r2, [sp, #116]   ; !alienWave, precomputed
b2dda: orrs   r1, r2
b2ddc: beq.w  b2ef0
```

i.e. the original shape except that the `eor` has been replaced by a
pre-header `eor.w r2, r2, #1; str r2, [sp, #116]` — `LICM` (IR) and
`MachineLICM` both hoist the loop-invariant `xor`, and the frame then
needs a slot for it (or reuses `stationRace`'s `sp+116`).

That single missing inline `eor` (4 B) is exactly the size gap:
**17,168 = 17,172 - 4**, and the whole stack/block layout shifts with it
(raw 12,634, aligned 1,727).  The branch polarity also flips
(`beq body` instead of `bne next`), which re-lays the rest of the
function.

## 5. Why no size-exact win could be landed

* Route A (`or` form, `FindMergedConditions` bails): correct block, but
  the `xor` is loop-invariant and always hoisted ⇒ 4 B short and a full
  stack/block relayout ⇒ raw 12,634 ≫ 5,454.
* Route B (flip the `and` order so the split emits ifirst first): blocked
  by `Reassociate`'s rank sort, and every spelling tried (`!= 0`, `== 1`,
  `& 1`, `> 0`, `>= 1`, `% 2`, `* 1`, `^ 0`, `+ 0`, `bool`, `char`,
  `short`, `unsigned`, `volatile`, `__builtin_expect`) either
  canonicalises back to `icmp ne %x, 0` or costs more than it saves.
* The guard block is only **8 of the 786** aligned diffs, so even a
  perfect guard fix moves the metric by ~1 %.

The best measured variant remains the pristine baseline
**5,454 / 786 / 17,172 B**.

## 6. Next steps (for a later wave)

1. Find the `CodeGenPrepare` routine that duplicates the pre-header
   `icmp ne alienWave, 0` into the loop block.  If it can be defeated
   *without* adding a value (i.e. keep the guard's operand as the shared
   pre-header `icmp`), the `or` form appears for free — but the `xor` is
   still hoisted, so the 4 B gap remains unless the `xor` is created
   post-`MachineLICM`.
2. Alternatively make the negation loop-variant (a real in-loop load that
   `SROA` refuses to promote) so `LICM`/`MachineLICM` cannot hoist it and
   the `eor` stays inline; that is the only way to keep 17,172 B with the
   `or` form.
3. `char alienWave` gives the best *aligned* count seen so far (767) but
   is 16 B short (17,156).

## 7. Gate (before -> after)

```text
_ZN5Level13createMissionEv  raw 5454 -> 5454   align 786 -> 786
                            size 17172 -> 17172 B
byte_exact 4507/4524, linked_exact 4507/4524, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370,
RATCHET PASS
```

No source change is landed.

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j8
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
cmake --build cmake-build-match --target check -j8
```

Scratch `/dev/shm/w11009/`: `Level.pristine.cpp`, `try.py`, `run.py`,
`batch.py`, `regions.py`, `cc.sh`, `Level.ll`, `Level_ptr.ll`,
`Level_bool.ll`, `filt_base.txt`, `filt.txt`, `v1..v9.json`,
`regions_base.txt`, `regions_flag.txt`, plus LLVM 7.0.0 sources
`Reassociate70.cpp`, `SDB70.cpp`, `VT70.cpp`, `LICM70.cpp`,
`ARMBaseInstrInfo70.cpp`, `CGP70.cpp`.
