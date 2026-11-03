# `Level::createScene` — the MachineLICM frame-address hoist (w3833)

**Status:** research note, 2026-11-03. Worktree `wt/w3833`, branch `wave89-w3833`,
base `5710f6d0`. **No source change landed** — no source shape found that improves
the official verifier. This commit contains only this note.

## TL;DR

* Target `_ZN5Level11createSceneEv`: **88.3 %**, orig/ours both **2252 B**,
  sha1(ours) `c4630a4f66bd`, 276 of 783 instructions differ, first diff at
  instruction 460 (`0xb4830`, the parts/hangar-mesh loop preheader).
* The whole diff is **one register-allocation decision** plus its downstream
  cascade; there is no missing/extra code, wrong constant, wrong callee, wrong
  field offset or jump-table change.
* Gate unchanged: **byte_exact 4498/4524, linked_exact 4498/4524,
  `.text 0x1337c8`, lint CLEAN ✓, drift 0, RATCHET PASS**.
* Root cause (re-confirmed at the `-debug-only=regalloc` level this pass):
  MachineLICM hoists `%541 = t2ADDri %stack.1, 0` (the frame address of the
  parts-loop `Vector{0, pi, 0}` temporary) from the loop body `bb.14` to the
  preheader `bb.8`. The hoisted interval is **hinted to `$r1`**, so greedy
  enqueues it with the `(1<<30)` priority bit *before* `hangarPart`, and it
  grabs `r4`; that single occupancy is what flips the rest of the function.

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

| site | original | ours |
|---|---|---|
| parts loop row pointer | `r9` | `r8` |
| parts loop `&Globals::Canvas` | `r8` | `r9` |
| parts loop `hangarPart` | spilled `sp#44` | `r6` |
| parts loop `k` | `r6` | spilled `sp#44` |
| ship loop `&Globals::rnd` | `r8` (value reloaded `ldr r0,[r8]`) | spilled `sp#44` |
| ship loop `type` | `fp` (r11) | `r8` |
| ship loop `capacity` | `r6` | `fp` (r11) |
| ship loop `Globals::rnd` value | reloaded per site | cached in `r5` |
| stack slots | `sp#44`=spots, `sp#16`=bitmask, `sp#20`=globals | `sp#24`=spots, `sp#12`=bitmask, `sp#16`=globals |

The parts loop body itself is instruction-for-instruction the same shape; only
register names / spill slots differ. The literal-pool words at the end differ by
±6 (`0x146f74/78` vs `0x146f7a/7e`) only because the code before them shifted;
the resolved array addresses (`kBarChildCounts` = `0x1fb818`,
`kBarChildMeshes` = `0x1fb82a`) are identical in both binaries.

## 2. The MachineLICM hoist (MIR evidence)

Pre-RA MIR (`-mllvm -print-machineinstrs`, "After Early Machine Loop Invariant
Code Motion") has the address in the **preheader** `bb.8`:

```
bb.8 (%ir-block.43):                       ; parts-loop preheader
  ...
  %529:gprnopc = t2ADDrs %528:tgpr, %3:rgpr, 34   ; row = kHangarMeshes + race*16
  ...
  %541:gprnopc = t2ADDri %stack.1, 0              ; &Vector temp   <-- hoisted
  ...
bb.14 (%ir-block.123):                     ; parts-loop body
  ...
  $r1 = COPY %541:gprnopc                          ; only use
  tBL ... @_ZN10AEGeometry11setRotationERKN11AByssEngine6AEMath6VectorE
```

`llc -debug` prints exactly:

```
Hoisting %541:gprnopc = t2ADDri %stack.1, 0, 14, $noreg, $noreg
 from %bb.14 to %bb.8
```

`t2ADDri` is `isReMaterializable = 1` (`T2I_bin_ii12rs` in
`ARMInstrThumb2.td`), and `MachineLICMBase::IsProfitableToHoist` returns `true`
unconditionally for trivially rematerializable instructions — the
register-pressure / speculation guards are never reached. The address is
eventually rematerialized at its use by the allocator (`%690 = t2ADDri` at
4680B, assigned `$r1`), so the *final* asm still contains `add r1, sp, #56` in
the body; the damage is the transient long-lived interval.

### Why that flips the allocator

`RAGreedy::enqueue` gives every interval with a physical-register hint the
`(1u << 30)` priority bit; among hinted intervals it orders by descending
`LiveInterval::getSize()`. From the NDK `-debug-only=regalloc` log
(`/dev/shm/w3833/ra_cs.txt`):

```
selectOrSplit GPRnopc:%541 [3568r,3712B:0)[4176B,5280B:0)[5600B,6256B:0)[6416B,6608B:0)
hints: $r1
missed hint $r1                       ; r1 is taken by %535 (uxth) at 4448
assigning %541 to $r4: R4 ...
...
selectOrSplit GPR:%11 [4800r,5280B:0)[5440B,5504r:0)[5600B,6256B:0)[6416B,6464r:0)  ; hangarPart
assigning %11 to $r6: R6 ...          ; r4 still occupied by %541
...
selectOrSplit GPR:%44 [9984r,11840r:0)                                                ; k
evicting $r6 ... assigning %44 to $r10
```

`%541`'s hoisted size is ≈3040, `%11` (hangarPart) ≈1600, so `%541` is enqueued
first and takes `r4`. With the address left in the body (sink flag) the log
instead shows `assigning %541 to $r1` and `%11` keeps `r4`, which is exactly the
original's parts loop.

## 3. What was measured this pass (all 88.3 %, 2252 B, sha `c4630a4f66bd`)

Every one of these produced **byte-identical IR** to the current source (same
sha, same first diff 460):

* `const Vector &rot = Vector{...}; g->setRotation(rot);`
* named local `Vector rot{...}`, `const Vector rot{...}`, `Vector rot(...)`,
  `Vector rot = {...}`, `Vector rot = Vector(...)`, copy-ctor `Vector rot2 = rot`
* `AbyssEngine::AEMath::Vector rot{...}`, `(*g).setRotation(...)`,
  `AEGeometry &gr = *g; gr.setRotation(...)`
* `float pi = 3.1415927f; Vector{0, pi, 0}`, `(float)M_PI`,
  `static_cast<const Vector&>(Vector{...})`, helper `PiRot::make()`
* `g->setRotation(plainHangar ? V : V)` / `k == 0 ? V : V` (identical branches
  fold away)
* `if (mesh >= 0) { ... }` block form (same CFG as `if (mesh < 0) continue`)
* `const int *row = kHangarMeshes[sceneRace]; mesh = row[k]` (and `int *row`)
* `PlayerStatic *` → `KIPlayer *hangarPart`
* `hangarPart` declared early / `= nullptr` / after `g`
* `plainHangar` moved before/after the `setAlwaysFriend` call, `>= 4` spelling

Worse variants (all measured):

| variant | pct | size |
|---|---|---|
| `mesh == -1` / `unsigned mesh` | 88.2 | 2252 |
| pointer induction `for (const int *p = row; p != row+4; ++p)` | 86.3 | 2256 |
| `while (k < 4)` with `k++` at top | 78.0 | 2260 |
| loop-carried `Vector *rotp` reassigned in the loop | 83.9 | 2260 |
| `const Vector *volatile p = &rot` | 79.9 | 2260 |
| `Vector rot; rot = Vector(...)` | 76.9 | 2276 |
| two locals + `k == 0 ? rot : rot2` | 85.5 | 2276 |
| `struct { char c; Vector v; }` holder | 82.0 | 2284 |
| `Vector rot` before `new AEGeometry` | 85.4 | 2256 |
| hoist `childCount = kBarChildCounts[race]` | 84.0 | 2244 |
| `mesh != -1` / `mesh > -1` / `short mesh` | 78.6–78.9 | 2260 |

Ship-loop variants (re-checked): `rnd` declared after `spots` (identical),
inside the loop (86.8 %, first diff 133 — perturbs the missionPtr==4 block too),
before `spotUsed` (76.0 %). The 2^5 local-vs-global matrix from w3813
(`loc11010`) is still the best.

## 4. The sink flag (re-confirmed, but not a gate win)

Scoping `-mllvm -sink-insts-to-avoid-spills` to `Level.cpp` makes the **parts
loop byte-identical** (first structural diff moves from 460 to 547) and raises
the score to **89.3 %**, but:

* `createScene` becomes **2256 B** (the ship loop gains one instruction);
* the flag re-sizes *many* other `Level.cpp` functions, so applying it per-TU is
  a massive gate regression: `byte_exact 4498 → 2357`, `linked_exact
  4498 → 4484` (measured with the flag added to `CMakeLists.txt`). Not viable.

## 5. Why a surviving loop-PHI use is the only non-flag escape

`IsProfitableToHoist` has exactly one early-out that beats the rematerializable
check:

```cpp
bool CheapInstr  = IsCheapInstruction(MI);      // true for t2ADDri
bool CreatesCopy = HasLoopPHIUse(&MI);
if (CheapInstr && CreatesCopy)
    return false;                                // <-- would keep it in the body
if (TII->isTriviallyReMaterializable(MI, AA))
    return true;
```

So a **surviving PHI use of the address inside the loop** would stop the hoist
without any flag. `HasLoopPHIUse` follows copies, but `%541`'s only use is
`$r1 = COPY %541` (a physical-register copy), which terminates the walk.

A PHI of the `Vector*` either (a) has both incoming values equal to `%2`, in
which case the IR optimiser folds it away before ISel, or (b) has a distinct
incoming value in the latch, in which case the machine PHI needs a second
`t2ADDri` in the latch (verified with the local LLVM 7.0.1 `llc` on
`/dev/shm/w3833/phi3.ll` and `phi_same.ll`). Neither reproduces the original's
single in-body `add r1, sp, #56`.

Any source shape that makes the address depend on a loop value (pointer
induction, volatile pointer, variable index) changes the address computation
itself and loses far more than it gains (see table above).

## 6. Cross-reference: `StarSystem::StarSystem` (100 %)

`StarSystem.cpp:342/344` uses the same `setRotation(Vector{0.0f, ..., 0.0f})`
idiom and is byte-exact, but in that function the `t2ADDri %stack.8` sites live
in straight-line `switch` blocks (`bb.4`, `bb.5`, `bb.7`, `bb.8`), not inside a
loop — there is no preheader to hoist into. It is not a counter-example.

## 7. Conclusion / next angle

The parts-loop swap is driven entirely by the hoisted `%541` grabbing `r4`
before `hangarPart`; the ship-loop swap is a second, independent consequence of
the same global allocation. A source shape that keeps the address in the loop
body (or, equivalently, gets `%11` allocated before `%541`) without the sink
flag was not found. The remaining levers, in order of plausibility:

1. A source shape that survives to a machine PHI use of the `Vector*` **without
   adding a latch `t2ADDri`** (e.g. a pointer PHI whose back-edge operand is
   CSE'd with the preheader def).
2. A shape that makes `%541` non-rematerializable while still lowering to a
   constant `add r1, sp, #56` (no such shape found: `ISD::FrameIndex` always
   selects `t2ADDri FI, 0`).
3. Reconstruct the ship loop independently: keep `&Globals::rnd` in `r8` with
   `ldr r0,[r8]` reloads while preserving the current prologue (stack-guard
   address spilled to `sp#36`). The 2^5 site matrix is exhausted.

Scratch artefacts (not committed): `/dev/shm/w3833/` — `ra_cs.txt`,
`ra_cs_sink.txt`, `mi.txt`, `mi_flag.txt`, `show_now2.txt`, `show_flag.txt`,
`show_allglob.txt`, `phi3.ll`, `phi_same.ll`, `try.py`, `score_cs.py`,
`v1.json`…`v13.json`.
