# `Level::createScene` — wave-91 re-check of the MachineLICM hoist (w4113)

**Status:** research note, 2026-11-03. Worktree `wt/w4113`, branch `wave91-w4113`,
base `a4557300`. **No source change landed** — no source shape found that raises
`byte_exact`. This commit contains only this note.

Follow-up to `level-createscene-licm-hoist-w3833.md` (root cause) and
`level-createscene-phi-use-w4013.md` (PHI-use hunt). This pass re-derived the
hoist decision from an instrumented LLVM 7 `-debug-only=machinelicm` run,
pinned down the sink-flag residue as a **second, independent** RA decision in
the ship loop, and re-tested the loop-PHI escape with an `undef`-operand PHI
(which does stop the hoist, but moves the address into the latch).

## TL;DR

* Target `_ZN5Level11createSceneEv`: **88.3 %**, orig/ours both **2252 B**,
  `byte=False`, `linked=False`.
* Gate unchanged: **byte_exact 4498/4524, linked_exact 4498/4524,
  size-exact 4524/4524, stub_zero_size 0, missing 0, wrong_type 0, extra 0,
  data_exact 843/843, imports 207, `.text 0x1337c8`, `.rodata 0x41b67`,
  `.ARM.exidx 0x6f70`, `.ARM.extab 0x26290 (-8)`, lint CLEAN ✓, drift 0,
  RATCHET PASS.**
* New evidence this pass:
  1. `-debug-only=machinelicm` prints a bare `Hoisting %512 … t2ADDri
     %stack.1, 0` with **no** reason line. The only `IsProfitableToHoist`
     path that prints nothing is the `isTriviallyReMaterializable` early
     `return true` (line 1218). The frame-index operand is a `FrameIndex`
     MO, not a register, so the generic "no virtual-register uses" rule does
     not reject it.
  2. The `-sink-insts-to-avoid-spills` fix works through
     `MachineLICMBase::SinkIntoLoop()`, whose criterion is exactly "the
     hoisted value's only uses are copies". `%512`'s only use is
     `$r1 = COPY %512`, so it is sunk straight back into the body.
  3. With the flag, the parts loop is byte-identical to the original, but
     the **ship loop** gains one instruction: ours spills `&Globals::rnd`
     to `sp#44` and reloads it, while the original keeps it in `r8` and
     reloads the *global* with `ldr r0,[r8]`. That is a second, separable
     RA decision — so even a hypothetically per-function sink flag would
     give **2256 B**, not 2252 B.
  4. A loop-carried pointer PHI with an `undef` preheader operand really
     does suppress the hoist (`HasLoopPHIUse` returns true), but the address
     then materialises once per iteration in the **latch** (`add.w r9, sp,
     #56`), and the body calls with `mov r1, r9`: 86.0 %, 2252 B.

## 1. The target, re-confirmed

Original parts loop (`0xb483a`):

```text
b483a: ldr.w r4, [r9, r6, lsl #2]      ; mesh = row[k]
b483e: cmp   r4, #0
b4840: blt.n b48fa
b4842: movs  r0, #192
b4844: blx   _Znwj
b4848: mov   r5, r0
b484a: ldr   r0, [pc, #840]            ; &GOT[Globals::Canvas]
b484c: add   r0, pc
b484e: ldr.w r8, [r0]                  ; r8 = Globals::Canvas
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

The first structural diff is the parts-loop preheader `0xb4830`
(`add.w r9,…` vs `add.w r8,…`); the whole function differs only in register
names / spill slots.

## 2. `-debug-only=machinelicm` — the exact decision

Full-module run (`llc -O2 -filetype=asm Level_nodebugvar.ll -debug-only=machinelicm`):

```text
******** Pre-regalloc Machine LICM: _ZN5Level11createSceneEv ********
Entering %bb.9
...
Entering %bb.14
Hoisting %510:tgpr = tLDRpci %const.17, 14, $noreg :: (load 4 from constant-pool)
 from %bb.14 to %bb.8
Hoisting %511:rgpr = t2MOVi 0, 14, $noreg, $noreg
 from %bb.14 to %bb.8
CSEing %511:rgpr = t2MOVi 0, 14, $noreg, $noreg
 with %351:rgpr = t2MOVi 0, 14, $noreg, $noreg
Hoisting %512:gprnopc = t2ADDri %stack.1, 0, 14, $noreg, $noreg
 from %bb.14 to %bb.8
Hoisting %513:rgpr = t2MOVi 304, 14, $noreg, $noreg
 from %bb.14 to %bb.8
Hoisting %516:rgpr = t2MOVi -1, 14, $noreg, $noreg
 from %bb.14 to %bb.8
```

`%512` gets a bare `Hoisting` with no reason line. In
`MachineLICMBase::IsProfitableToHoist` (LLVM 7) the only silent success path
is:

```cpp
bool CheapInstr = IsCheapInstruction(MI);
bool CreatesCopy = HasLoopPHIUse(&MI);
if (CheapInstr && CreatesCopy)
  return false;                              // "Won't hoist cheap instr with loop PHI use"
// Rematerializable instructions should always be hoisted …
if (TII->isTriviallyReMaterializable(MI, AA))
  return true;                               // <-- silent; this is the path taken
```

`%512` is `t2ADDri %stack.1, 0`. `%stack.1` is a **FrameIndex** machine
operand, not a register, so `isReallyTriviallyReMaterializableGeneric`'s
"no virtual-register uses" loop never sees it and returns true. (The
generic rule that would have rejected a virtual-register use is therefore
irrelevant here.)

`HasLoopPHIUse` walks virtual-register defs and their uses:

```cpp
for (const MachineOperand &MO : MI->operands()) {
  if (!MO.isReg() || !MO.isDef()) continue;
  unsigned Reg = MO.getReg();
  if (!TargetRegisterInfo::isVirtualRegister(Reg)) continue;
  for (MachineInstr &UseMI : MRI->use_instructions(Reg)) {
    if (UseMI.isPHI()) {
      if (CurLoop->contains(&UseMI)) return true;
      if (isExitBlock(UseMI.getParent())) return true;
      continue;
    }
    if (UseMI.isCopy() && CurLoop->contains(&UseMI))
      Work.push_back(&UseMI);
  }
}
```

`%512`'s only use is `$r1 = COPY %512` (a **physical** def), so the walk
stops at the next level and returns false.

## 3. Why the sink flag fixes the parts loop, and what it costs

`SinkInstsToAvoidSpills` runs at the end of the same (early) MachineLICM pass:

```cpp
void MachineLICMBase::SinkIntoLoop() {
  ...
  for (MachineBasicBlock::instr_iterator I = Preheader->instr_begin(); …) {
    if (IsLoopInvariantInst(*I) && !HasLoopPHIUse(&*I))
      Candidates.push_back(&*I);
  }
  for (MachineInstr *I : Candidates) {
    ...
    for (MachineInstr &MI : MRI->use_instructions(MO.getReg())) {
      if (!MI.isCopy()) { CanSink = false; break; }   // <-- only-copy uses
      ...
    }
    ...
    B->splice(B->getFirstNonPHI(), Preheader, I);
  }
}
```

`%512`'s only use is a copy, so it is sunk back to the block that contains
that copy — the loop body. Result (`/dev/shm/w4113/sink_cs.txt`), which is
instruction-for-instruction the original:

```text
b4876: ldr.w r4, [r9, r6, lsl #2]
...
b488a: ldr.w r8, [r0]                  ; Canvas in r8
b488e: ldr.w r2, [r8]
b4892: uxth  r1, r4
b4894: mov   r0, r5
b4896: movs  r3, #0
b4898: blx   AEGeometry::AEGeometry
b489c: ldr   r0, [pc, #812]
b489e: add   r1, sp, #56               ; &Vector in the body
b48a0: str   r0, [sp, #60]
b48a2: mov.w sl, #0
b48a6: mov   r0, r5
b48a8: str.w sl, [sp, #56]
b48ac: str.w sl, [sp, #64]
b48b0: blx   AEGeometry::setRotation
...
b48ce: str   r4, [sp, #44]             ; hangarPart spilled, k in r6
```

**But the flag is not a win even per-function.** The same sink build's ship
loop is 4 B longer than the original:

```text
; original (0xb4924)                       ; sink build (0xb4960)
ldr   r0, [pc, #632]                        ldr   r0, [pc, #636]
ldr   r1, [pc, #636]                        ldr   r1, [pc, #640]
add   r0, pc                                add   r0, pc
add   r1, pc                                add   r1, pc
ldr.w r8, [r0]        ; r8 = &Globals::rnd  ldr   r2, [r0, #0]   ; r2 = &Globals::rnd
ldr   r0, [sp, #52]                         ldr   r0, [sp, #52]
ldr.w r6, [r1, r0, lsl #2]  ; capacity      str   r2, [sp, #44]  ; <-- spill
ldr.w r0, [r8]                              ldr.w fp, [r1, r0, lsl #2]
adds  r1, r6, #1                            ldr   r0, [r2, #0]
blx   nextInt                               add.w r1, fp, #1
                                            blx   nextInt
```

The original keeps the address of the global (`&Globals::rnd`, from the GOT
slot at `0x2101f8`) in the callee-saved `r8` and re-loads the *value*
(`ldr r0,[r8]`) at every `nextInt` site; ours spills the address and uses
`fp` for `capacity`. That is a second RA decision in a different loop, so
even a per-function sink flag would land at **2256 B**, not 2252 B. (The
flag applied per-TU also re-sizes many other `Level.cpp` functions:
`byte_exact 4498 → 2357`.)

## 4. The loop-PHI escape, re-tested with an `undef` operand

`OptimizePHIs` folds a PHI when all incoming values trace to a single
non-copy value (`IsSingleValuePHICycle`). A PHI whose other incoming value
is `undef` traces to `nullptr` and therefore **survives**:

```cpp
Vector rot{0.0f, 3.1415927f, 0.0f};
const Vector *rotp;                    // undef on the first iteration
g->setRotation(*rotp);
rotp = &rot;
```

Measured: **86.0 %, 2252 B**. The hoist is gone, but the address is
materialised in the **latch**, not the body:

```text
b486a: mov   r1, r9                    ; body: pass the carried pointer
...
b48f0: add.w r9, sp, #56               ; latch: &Vector
b48f4: ldrd  sl, r0, [sp, #40]
```

So the mechanism works but the shape is wrong (and the first iteration
dereferences `undef`). A semantically clean loop-carried pointer
(`const Vector *rotp = &rot;` before the loop) costs 8 B: **83.2 %,
2260 B**. Both are worse than the 4 B `k` spill they remove.

## 5. Shapes measured this pass (official verifier, autojunk=False)

Baseline: **88.3 %, 2252 B**. All of the following are **byte-identical IR**
to the baseline (same 88.3 % / 2252 B):

| variant | note |
|---|---|
| `Vector{0.0f, (float) 3.141592653589793, 0.0f}` | double literal folds |
| `Vector(0.0f, 3.1415927f, 0.0f)` (parens) | |
| named `Vector rot{…}; g->setRotation(rot);` | |
| `const AbyssEngine::AEMath::Vector &rot = Vector{…};` | |
| `const int capacity = kHangarCapacity[sceneRace];` | |
| `unsigned capacity = (unsigned) kHangarCapacity[sceneRace];` | |
| `unsigned type = (unsigned) Globals::globals->getRandomEnemyFighter(race);` | |
| `const int *const spots = …` | |
| `if (rnd->nextInt(100) < 30)` (vs `<= 29`) | |
| `bool *spotUsed = new bool[capacity];` | |

Worse variants:

| variant | pct | size |
|---|---|---|
| drop the local `rnd`, use `Globals::rnd` everywhere in the ship loop | 75.0 | 2228 |
| `spot = Globals::rnd->nextInt(capacity);` in the `tries` loop | 75.0 | 2228 |
| `if (Globals::rnd->nextInt(100) <= 29)` at the top | 83.6 | 2252 |
| `int pick = Globals::rnd->nextInt(3);` | 84.1 | 2252 |
| `int spot = rnd->nextInt(capacity);` | 87.4 | 2252 |
| `Vector rot{…}` declared before `new AEGeometry` | 85.4 | 2256 |
| `undef`-operand loop PHI (above) | 86.0 | 2252 |
| loop-carried pointer PHI `= &rot` before the loop | 83.2 | 2260 |

## 6. Conclusion / remaining levers

The parts loop is fully determined by the hoisted `%512`; the ship loop is a
second, independent greedy-RA decision (`&Globals::rnd` → `r8` vs spill).
Both must be fixed for `byte_exact`, and neither has a known source shape.

Remaining levers, in order of plausibility:

1. A **free in-body PHI use** of the `Vector*`: a PHI *inside* the loop body
   whose result is coalesced with `%512` and whose other operand costs
   nothing, so that `add r1, sp, #56` stays at the call site. `undef`
   achieves the suppression but not the placement (the address ends up in
   the latch).
2. A shape that makes `%512` non-rematerializable while still lowering to a
   constant `add r1, sp, #56`. `ISD::FrameIndex` always selects
   `t2ADDri FI, 0` (`ARMISelDAGToDAG.cpp`, `case ISD::FrameIndex`), and
   `t2ADDri12` (not rematerializable) is only produced for a non-zero
   `imm0_4095` add, so this needs a different DAG shape.
3. Reproduce the ship loop's `&Globals::rnd → r8` allocation. The 2^5
   local-vs-global site matrix from w3813 is exhausted; the current mix
   (`rnd` at the top / `pick` / `tries`, `Globals::rnd` at `spot` /
   `setRotation`) is the best measured.

Scratch artefacts (not committed): `/dev/shm/w4113/` — `score.py`,
`rebuild_score.sh`, `v.py`, `batch8.py`…`batch11.py`, `licm.log`,
`sink2.log`, `cs_standalone.ll`, `cs_after_licm.mir`, `sink_cs.txt`,
`fn_baseline.txt`, `orig_raw.txt`, `ours_raw.txt`, `Level.cpp.bak`.
