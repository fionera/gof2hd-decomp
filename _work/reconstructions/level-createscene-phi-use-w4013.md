# `Level::createScene` — wave-90 PHI-use hunt (w4013)

**Status:** research note, 2026-11-03. Worktree `wt/w4013`, branch `wave90-w4013`,
base `360c7467`. **No source change landed** — no source shape found that raises
`byte_exact`. This commit contains only this note.

Follow-up to `_work/reconstructions/level-createscene-licm-hoist-w3833.md`
(which established the root cause). This pass re-confirmed the target with the
project's instrumented LLVM 7 and exhaustively re-tested the "loop-PHI use"
escape, including exit-block PHIs, debug-info-only uses, and two-temporary
selects.

## TL;DR

* Target `_ZN5Level11createSceneEv`: **88.3 %**, orig/ours both **2252 B**,
  783/791 reconciled instructions. Gate unchanged:
  **byte_exact 4498/4524, linked_exact 4498/4524, size-exact 4524/4524,
  stub_zero_size 0, missing 0, wrong_type 0, extra 0, data_exact 843/843,
  imports 207, `.text 0x1337c8`, `.rodata 0x41b67`, `.ARM.exidx 0x6f70`,
  `.ARM.extab 0x26290 (-8)`, lint CLEAN ✓, drift 0, RATCHET PASS.**
* The whole diff remains one greedy-RA decision: MachineLICM hoists
  `t2ADDri %stack.1, 0` (the parts-loop `Vector{0, pi, 0}` frame address) to the
  preheader; it is hinted `$r1`, misses because the `uxth` of `mesh` owns `r1`,
  and takes `r4`, forcing `hangarPart→r6` and `k→spill`.
* **New evidence this pass:** the project's instrumented LLVM 7 `llc`
  (`/home/debian/llvm7build/build/bin/llc`, `-debug-only=regalloc`) reproduces
  the identical hoist (`%512 = t2ADDri %stack.1, 0` in bb.8, hinted `$r1`,
  missed, assigned `$r4`), so the MIR shape is reproducible locally. Scoping
  `-mllvm -sink-insts-to-avoid-spills` to `Level.cpp` makes the parts loop
  **instruction-for-instruction identical** to the original (same registers
  `r9`=row, `r8`=Canvas, `r6`=k, `r4`=part, `sl`=zero), confirming that the
  original's target MIR is exactly "address left in the loop body".
* Every source shape that keeps a loop-PHI use of the address either folds
  away (dead PHI removed by InstCombine) or costs 12–24 bytes of real code,
  which is worse than the 4-byte k-spill it removes.

## 1. The target, re-confirmed

Original parts loop (`0xb483a`, 2252 B function):

```text
b483a: ldr.w r4, [r9, r6, lsl #2]      ; mesh = row[k]
b483e: cmp   r4, #0
b4840: blt.n b48fa
b4842: movs  r0, #192
b4844: blx   _Znwj
b4848: mov   r5, r0
b484a: ldr   r0, [pc, #840]            ; &Globals::Canvas
b484c: add   r0, pc
b484e: ldr.w r8, [r0]
b4852: ldr.w r2, [r8]
b4856: uxth  r1, r4
b4858: mov   r0, r5
b485a: movs  r3, #0
b485c: blx   AEGeometry::AEGeometry
b4860: ldr   r0, [pc, #808]            ; pi
b4862: add   r1, sp, #56               ; &Vector  <-- in the body
b4864: str   r0, [sp, #60]             ; y = pi
b4866: mov.w sl, #0
b486a: mov   r0, r5
b486c: str.w sl, [sp, #56]             ; x = 0
b4870: str.w sl, [sp, #64]             ; z = 0
b4874: blx   AEGeometry::setRotation
b4878: mov.w r0, #304
b487c: blx   _Znwj
b4880: mov   r4, r0                     ; hangarPart
b4882: mov.w r1, #-1
b4886: mov   r2, r5
b4888: movs  r3, #0
b488a: strd  sl, sl, [sp]
b488e: blx   PlayerStatic::PlayerStatic
b4892: str   r4, [sp, #44]             ; hangarPart spilled
b4894: ldr   r0, [sp, #28]             ; plainHangar
b4896: cbnz  r0, b48e6
```

Ours (baseline) is the same shape but with `r8`=row, `r9`=Canvas, `r6`=part,
`r4`=zero, and one extra instruction in the body:

```text
b4860: ldr   r0, [pc, #808]
b4862: add   r1, sp, #56
b4864: str   r0, [sp, #60]
b4866: movs  r4, #0
b4868: mov   r0, r5
b486a: str   r4, [sp, #56]
b486c: str   r6, [sp, #44]             ; <-- k spilled (original: r6 = k)
b486e: str   r4, [sp, #64]
b4870: blx   AEGeometry::setRotation
```

The first structural diff is instruction 460 (`0xb4830`, the parts-loop
preheader); the whole function differs only in register names / spill slots
(276 of 783 instructions by the official verifier).

## 2. Instrumented-LLVM confirmation

The local `llc` (LLVM 7.0.0, instrumented build) on the extracted
`createScene` IR reproduces the hoist exactly:

```text
# After Early Machine Loop Invariant Code Motion:
bb.8 (%ir-block...):                    ; parts-loop preheader
  ...
  %512:gprnopc = t2ADDri %stack.1, 0, 14, $noreg, $noreg
  ...
bb.9:                                    ; loop header
  %6:rgpr = PHI ...
```

and the RA log (second `Function: _ZN5Level11createSceneEv` section):

```text
selectOrSplit GPRnopc:%512 [3552r,3728B:0)[4192B,5248B:0)[5568B,6208B:0)[6368B,6560B:0)
hints: $r1
missed hint $r1
assigning %512 to $r4: R4
```

With `-sink-insts-to-avoid-spills`, `%512` stays in the body and the resulting
parts loop is instruction-for-instruction the original's (registers
`r9`=row, `r8`=Canvas, `r6`=k, `r4`=part, `sl`=zero, `str r4,[sp,#44]` for
`hangarPart`). The flag is not usable as a fix: applied per-TU it re-sizes many
other `Level.cpp` functions and drops `byte_exact 4498 → 2357` (measured by
w3833; re-confirmed here that the sink build scores 89.3 % but is 2256 B and
changes the ship loop).

## 3. Why a PHI use is the only non-flag escape

`MachineLICMBase::IsProfitableToHoist` (LLVM 7):

```cpp
bool CheapInstr  = IsCheapInstruction(MI);   // true for t2ADDri
bool CreatesCopy = HasLoopPHIUse(&MI);
if (CheapInstr && CreatesCopy)
    return false;                            // keep it in the loop
if (TII->isTriviallyReMaterializable(MI, AA))
    return true;                             // <-- current path
```

`HasLoopPHIUse` follows copies but stops at a physical-register copy. In our
MIR the address's only use is `$r1 = COPY %512` (physical), so the walk stops
and the rematerializable check wins. A surviving PHI use of the address (or of
a virtual copy of it) inside the loop, or of a PHI in an exit block, is the
only source-level way to take the early-out. `HasLoopPHIUse` also rejects a
value used by a PHI in an exit block, so a post-loop use works too.

## 4. Shapes measured this pass (official verifier)

Baseline: `88.3 %`, 2252 B, 783/791. All measurements use
`tools/verify/verify.py` (`reconcile` + `score`, autojunk=False).

### 4.1 Exit-block PHI (post-loop use)

The `t7` shape in isolation (`Vector *p = nullptr; ...; p = &v; ...;
if (p) g(1);`) keeps the address in the body, but every createScene embedding
adds real code:

| variant | pct | size | note |
|---|---|---|---|
| `const Vector *lastRot=nullptr;` + `setRotation(lastRot ? *lastRot : rot)` + `lastRot=&rot` | 83.2 | 2276 | select costs `cmp/it/moveq` + preheader `mov` |
| same with `k == 0 ? rot : *lastRot` | 83.1 | 2276 | same cost |
| `lastRot` + post-loop `if (lastRot == nullptr) { }` | 88.3 | 2252 | dead, folded |
| `lastRot` + post-loop `lastRot = nullptr;` / `(void)lastRot;` / `lastRot = lastRot;` / `bool unused = ...` | 88.3 | 2252 | dead, folded |
| `lastRot` + `if (lastRot != nullptr && getIndex()==108)` (real exit PHI) | 84.3 | 2264 | +3 insns, perturbs loop rotation |
| `lastRot` + `capacity = lastRot ? kHangarCapacity[race] : 0` | 83.0 | 2268 | +5 insns |

The debug-info-only route does not work: a named pointer assigned `&rot` and
never read is removed by InstCombine (verified: `opt -instcombine` deletes the
dead PHI; `opt -simplifycfg` keeps it, but clang's `-Oz` pipeline runs
InstCombine). `-g` is on, but no surviving `llvm.dbg.value` use of a PHI exists
in the emitted IR for these shapes.

### 4.2 In-loop PHI / two-temporary shapes

| variant | pct | size |
|---|---|---|
| `Vector rot{...}` declared before the loop, used in the loop | 83.2 | 2276 |
| `Vector rot{...}` before loop + `rot = Vector{...}` inside | 77.0 | 2280 |
| `Vector rot;` before loop + field assignments inside | 85.3 | 2264 |
| `Vector rot;` before loop + `rot.x=0; rot.y=pi; rot.z=0;` inside | 85.3 | 2264 |
| `Vector rot{...}` before loop, `setRotation(rot)` (no reassign) | 83.9 | 2260 |
| `Vector rot0{...}; const Vector *p=&rot0;` before loop, `Vector rot{...}` + `setRotation(*p); p=&rot;` inside | 76.3 | 2272 |
| `Vector a{...}; Vector b{...}; setRotation(plainHangar ? a : b)` inside | 78.1 | 2272 |
| `Vector a{...}; Vector b{...}; setRotation(k == 0 ? a : b)` inside | 77.4 | 2272 |
| `Vector rots[2] = {...}; setRotation(rots[plainHangar ? 0 : 0])` | 77.6 | 2288 |
| `Vector rot{...}; const Vector *p=&rot; setRotation(*p);` (folds) | 88.3 | 2252 |
| `const Vector &r = rot; setRotation(r);` (folds) | 88.3 | 2252 |
| `Vector rots[1] = {{...}}; setRotation(rots[0]);` (folds) | 88.3 | 2252 |
| `Vector rot{...}; Vector copy = rot; setRotation(copy);` (copy elided) | 88.3 | 2252 |
| `static Vector makePiRotation() { return Vector{...}; }` inlined (folds) | 88.3 | 2252 |

### 4.3 Construction / declaration variants (all identical IR)

All of these are byte-identical to the baseline (88.3 %, 2252 B, same sha):

* `Vector rot(0.0f, 3.1415927f, 0.0f);` (parens, named)
* `Vector rot{...}` / `const Vector rot{...}` / `Vector rot = {...}`
* `Vector rot; rot.y = 3.1415927f;` (default ctor + one field)
* `Vector rot; rot.x = 0; rot.y = pi; rot.z = 0;`
* `Vector rot = {}; rot.y = pi;`
* `const Vector &rot = Vector{...};`
* `g->setRotation(*&rot);`
* `AbyssEngine::AEMath::Vector rot(...)`
* `unsigned k` / `k <= 3` loop spellings
* `bool plainHangar = !(sceneRace < 4 && sceneRace != 1);` (Ghidra spelling)
* `int mesh = 0;` / `AEGeometry *g = nullptr;` declared before the loop
* `PlayerStatic *hangarPart = nullptr;` declared before the loop (84.0 %, 2288 B)
* `Vector rot` constructed before the `mesh < 0` check (84.0 %, 2264 B)
* `Vector rot` constructed after `new PlayerStatic` (87.9 %, 2252 B)
* inline `new PlayerStatic(...)` into `ArrayAdd` (62.7 %, 2236 B)

### 4.4 Other loop spellings

| variant | pct | size |
|---|---|---|
| `for (unsigned k = 0; k < 4; k++)` | 88.3 | 2252 |
| `for (int k = 0; k != 4; k++)` | 87.9 | 2252 |
| `for (int k = 0; k <= 3; k++)` | 88.3 | 2252 |
| `for (int k = 1; k <= 4; k++)` with `k-1` index | 84.8 | 2260 |
| `while (k < 4)` (w3833) | 78.0 | 2260 |
| pointer induction (w3833) | 86.3 | 2256 |
| `mesh == -1` / `mesh > -1` / `short mesh` (w3833) | 78.6–88.2 | 2252–2260 |

## 5. Why every PHI shape loses

The original's parts loop is exactly one instruction shorter than ours (the
`k` spill). Any surviving PHI use must therefore cost **≤ 1 instruction** to
break even, and **0** to be byte-exact. A PHI of a stack address needs:

1. a materialised preheader value (`mov rX, #0` for `nullptr`, 1 insn), and
2. a use that consumes the PHI. The cheapest real use is a conditional move
   (`cmp` + `it` + `moveq`, 3 insns) or a compare-and-branch (2 insns).

The only zero-code use is a debug value, and InstCombine deletes a
debug-only PHI. A PHI whose two incoming values are the same address is folded
by the IR optimiser or CSE'd by MachineCSE before MachineLICM. A second
`Vector` object (to make the incoming values distinct) grows the frame from
72 to 84 bytes and shifts every stack offset, which is worse than the 4-byte
k-spill.

## 6. Conclusion / next angle

The parts-loop allocation is fully determined by the hoisted
`t2ADDri %stack.1, 0`. The only non-flag escape is a surviving loop-PHI use of
the address, and every source shape that produces one costs more code than the
single spill it removes. The remaining levers, in order of plausibility:

1. A source shape whose PHI use is **free** — e.g. a `Vector` pointer that is
   loop-carried but whose PHI is coalesced away while still present at
   MachineLICM. No such shape was found: C++ that produces a live PHI always
   materialises its preheader value, and C++ that produces a dead PHI has it
   removed by InstCombine before ISel.
2. A shape that makes `%541` non-rematerializable while still lowering to
   `add r1, sp, #56` (no such shape: `ISD::FrameIndex` always selects
   `t2ADDri FI, 0`).
3. Reproduce the original's greedy order by another global-RA lever (branch
   weights / spill weights) without changing the instruction stream; the
   priority is `(1<<30)|(1<<29)|size` for both `%541` and `hangarPart`, and
   `%541`'s interval is larger, so it is enqueued first regardless of weight.

Scratch artefacts (not committed): `/dev/shm/w4013/` — `mir.sh`,
`mir_out.txt`, `cs_only.ll`, `cs_only.s`, `ra_fix.txt`, `cmp.py`, `score1.py`,
`runbatch*.py`, `Level.cpp.orig`, `CMakeLists.txt.orig`.
