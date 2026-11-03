# `Level::createScene` — 88.3 %, 2252 B (w3813)

**Status:** research note, 2026-11-03. Worktree `wt/w3813`, branch `wave88-w3813`,
base `41804126`. **No source change landed** — no source shape found that improves
the official verifier. This commit contains only this note.

## TL;DR

* Target `_ZN5Level11createSceneEv`: **88.3 %**, orig/ours both **2252 B**,
  sha1(ours) `c4630a4f66bd`, 276 of 783 instructions differ, first diff at
  instruction 460 (`0xb4830`, the parts/hangar-mesh loop).
* The whole diff is **one register-allocation decision** plus its downstream
  cascade. There is no missing/extra code, no wrong constant, no wrong callee,
  no wrong field offset, no jump-table change.
* Gate unchanged: **byte_exact 4498/4524, linked_exact 4498/4524,
  size-exact 4524/4524, `.text 0x1337c8`, lint CLEAN ✓, drift 0, RATCHET PASS**.

## 1. The diff

Original vs ours at the first differing instruction (`0xb4830`, parts loop preheader):

```
orig:  add.w r9, r0, r2, lsl #4      ; r9 = kHangarMeshes[sceneRace]  (row)
       str.w r9, [sp, #24]
       ...
       ldr.w r8, [r0]                ; r8 = &Globals::Canvas
ours:  add.w r8, r0, r2, lsl #4      ; r8 = row
       str.w r8, [sp, #24]
       ...
       ldr.w r9, [r0]                ; r9 = &Globals::Canvas
```

So the two long-lived values in the parts loop are swapped: **row→r9 / Canvas→r8
in the original, row→r8 / Canvas→r9 in ours**. Everything else follows:

| site | original | ours |
|---|---|---|
| parts loop row pointer | `r9` | `r8` |
| parts loop `&Globals::Canvas` | `r8` | `r9` |
| parts loop `hangarPart` | spilled `sp#44` | `r6` |
| parts loop `k` | `r6` | spilled `sp#44` |
| ship loop `&Globals::rnd` | `r8` | spilled `sp#44` |
| ship loop `type` | `fp` (r11) | `r8` |
| ship loop `capacity` | `r6` | `fp` (r11) |
| ship loop `Globals::rnd` value | reloaded `ldr r0,[r8]` | cached in `r5` |
| stack slots | `sp#44`=spots, `sp#16`=bitmask, `sp#20`=globals | `sp#24`=spots, `sp#12`=bitmask, `sp#16`=globals |
| loop tail | `ldr.w sl,[sp,#40]; ldr.w r9,[sp,#24]` | `ldr.w r8,[sp,#24]; ldrd sl,r6,[sp,#40]` |
| literal pool | `... 0x00146f74,0x00146f78,0x0015b8cc(rnd),...` | `... 0x00146f7a,0x00146f7e,0x0015b8d2,...` (one extra word) |

The two `Vector::operator=` "GOT vs PLT" notes in earlier passes were a
misreading: both binaries emit the same `blx <PLT>` for the two
`setRotation`/`Vector` sites.

## 2. Root cause (MIR evidence)

Pre-RA MIR (`-mllvm -print-machineinstrs`, "After Machine Instruction Scheduler")
for the current source:

```
3520B  %529:gprnopc = t2ADDrs %528:tgpr, %3:rgpr, 34      ; row = kHangarMeshes + sceneRace*16
4192B  %9:rgpr     = t2LDRs  %529:gprnopc, %6:rgpr, 2     ; row[k]  (one use)
...
4368B  %533:gpr    = t2LDRi12 %532:tgpr, 0                ; &Globals::Canvas (GOT)
4384B  %534:gpr    = t2LDRi12 %533:gpr, 0                 ; *Canvas
5744B  %558:gpr    = t2LDRi12 %533:gpr, 0                 ; *Canvas (child loop)
```

The critical vreg is **not** either of those; it is the loop-invariant frame
address of the `Vector{0, pi, 0}` temporary used by
`g->setRotation(Vector{0.0f, 3.1415927f, 0.0f})`:

```
3552B  %539:tgpr    = t2LDRpci %const.1                   ; 3.1415927f
3568B  %541:gprnopc = t2ADDri %stack.1, 0                 ; &Vector temp  (bb.12 = preheader)
...
4688B  $r1 = COPY %541:gprnopc                            ; bb.19 = parts-loop body
```

`%541` is hoisted into the preheader by `MachineLICM`, so its live interval
spans the whole parts loop. That extra long-lived value is what pushes the
greedy allocator into `row→r8 / Canvas→r9` and spills `&Globals::rnd` later.

Why it is hoisted: `t2ADDri` is declared `isReMaterializable = 1` in
`ARMInstrThd2.td` (`multiclass T2I_bin_ii12rs`, comment: *"The register-immediate
version is re-materializable. This is useful in particular for taking the
address of a local."*), and `MachineLICMBase::IsProfitableToHoist` returns
`true` unconditionally for trivially rematerializable instructions
(`lib/CodeGen/MachineLICM.cpp`, "Rematerializable instructions should always be
hoisted"). `SinkInstsToAvoidSpills` (`-mllvm -sink-insts-to-avoid-spills`) is
the only thing that moves it back, because its sole use is a `COPY`
(`MachineLICMBase::SinkIntoLoop`).

### Confirmation with the sink flag

Scoping `-mllvm -sink-insts-to-avoid-spills` to `Level.cpp`:

* parts loop becomes **byte-identical** to the original (first structural diff
  moves from instruction 460 to instruction 547, the ship loop);
* score rises to **89.3 %**, but the function becomes **2256 B** (the flag also
  re-sizes other `Level.cpp` functions, so `createScene` shifts to `0xb4328`
  and the downstream `b.w <plt>` thunks move — it is not a gate win).

The flag does **not** fix the ship loop: `&Globals::rnd` is still spilled to
`sp#44` there. The ship-loop diff is a second, independent RA decision.

### Regalloc ordering

LLVM 7.0.1 greedy `AllocationOrder(GPR)` is

```
[ $r0 $r1 $r2 $r3 $r12 $lr $r4 $r5 $r6 $r8 $r9 $r10 $r11 ]
```

(r7 is the frame pointer). `r8` is allocated before `r9`, so whichever of the
two long-lived parts-loop values is enqueued first gets `r8`. The queue is
ordered by descending `LiveInterval::getSize()` (`RAGreedy::enqueue`), with
`(1<<30)` set for physical-register hints. The hoisted `%541` changes the
relative sizes/hints and therefore the winner.

## 3. What was measured this pass (all 88.3 % or worse, 2252 B unless noted)

Build flags were not re-explored (see `worst_function_flag_sensitivity_w3751.md`
and `build_flags_independent_audit_w3750.md`).

Parts-loop / `Vector` temp shapes:

* `Vector rot{...}` local before `new AEGeometry` — 85.4 %, **2256 B**
* `Vector rot{...}` local after `new AEGeometry` — 88.3 %
* `Vector rot; rot = Vector{...}` — 76.9 %, 2276 B
* `Vector rot; rot.x/y/z = ...` — 88.3 %
* `Vector rot` hoisted before the `k` loop — 83.9 %, 2260 B
* `Vector rot = {...}`, `Vector rot(...)`, `const Vector rot{...}` — 88.3 %
* `static const Vector rot{...}` — 72.6 %, 2228 B
* `3.14159265f`, `AbyssEngine::AEMath::Vector{...}` — 88.3 %
* `if (mesh == -1)` instead of `mesh < 0` — 88.2 %
* `k != 4` — 87.9 %; `++k` — 88.3 %
* `(unsigned short) mesh` cast — 88.3 %
* `plainHangar = !(sceneRace < 4 && sceneRace != 1)` — 88.3 %
* `const int mesh`, `AEGeometry *const g`, `PlayerStatic *const hangarPart` — 88.3 %
* `new PlayerStatic(-1, g, 0, 0, 0)` — 88.3 %
* `j != count`, `++j` — 88.3 %
* `ArrayAdd<KIPlayer *>(hangarPart, ...)` — 88.3 %

Ship-loop `rnd` shapes:

* `AERandom *&rnd = Globals::rnd` (reference) — 75.0 %, 2228 B
* `AERandom **rndp = &Globals::rnd` — 75.0 %, 2228 B
* all sites `Globals::rnd` (local removed) — 75.0 %, 2228 B
* all sites use the local `rnd` — 83.7 %, 2236 B
* `AERandom *const rnd` — 88.3 %
* `rnd; rnd = Globals::rnd;` — 88.3 %
* rotation site local (`rnd->nextInt(300)`) — 87.7 %
* `rnd` declared before `kHangarCapacity`, used for `shipCount` — 63.6 %, 2224 B

The earlier 2^5 local-vs-global site matrix (`subsets2.log`) still has the
current source (`loc11010`) as its best at 88.3 %; every other combination is
≤ 88.2 % or changes the prologue (`firstdiff` 4–28) and the size.

## 4. Conclusion / next angle

The parts-loop swap is driven by the hoisted `t2ADDri` frame address; the
ship-loop swap is driven by the cached `Globals::rnd` value in `r5`. A source
shape that gets both right without the sink flag was not found. The two most
plausible remaining levers:

1. Make the `Vector` temp address **non-rematerializable** so `MachineLICM`
   leaves it in the loop body. Every plain C++ spelling of the temporary
   (`Vector{...}`, named local, const, field assignment, hoisted) still lowers
   to a `t2ADDri` frame address and is hoisted. A shape that selects
   `t2ADDri12`/`addw` (not marked `isReMaterializable`) would not be hoisted,
   but the observed frame offset is `sp+56`, which is `t2ADDri`-encodable.
2. Reconstruct a ship-loop shape that keeps **no cached `Globals::rnd` value**
   (so the original's `ldr r0,[r8]` reloads are reproduced) while keeping the
   current prologue (stack-guard pointer spilled to `sp#36`). The all-`Globals::rnd`
   and reference forms both drop the prologue match and the function to 2228 B.

Scratch artefacts (not committed): `/dev/shm/w3813/` — `orig.asm`, `ours.asm`,
`verify_createScene.txt`, `verify_flag.txt`, `mi.txt`, `regalloc_cs.txt`,
`subsets2.log`, `try.py`, `score_cs.py`.
