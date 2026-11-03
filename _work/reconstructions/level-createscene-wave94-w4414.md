# `Level::createScene` — wave-94 w4414: the real `-Oz` register-allocation model

**Status:** research note, 2026-11-03. Worktree `wt/w4414`, branch `wave94-w4414`,
base `f262bd4b`. **No source change landed** — no source shape found that raises
`byte_exact`. This commit contains only this note.

Follow-up to `level-createscene-wave92-w4249.md` (which followed
`level-createscene-wave92-w4213.md`, `level-createscene-wave91-w4113.md`,
`level-createscene-phi-use-w4013.md`, `level-createscene-licm-hoist-w3833.md`,
`Level-createScene-flag-free.md`, `level-createscene-88-3-2252-b_w3813.md`).

## TL;DR

* Target `_ZN5Level11createSceneEv`: **88.3 %**, orig/ours both **2252 B**,
  783/791 instructions, 276 raw-diff lines. `byte=False`, `linked=False`.
* Gate unchanged and re-measured this pass: **byte_exact 4499/4524,
  linked_exact 4499/4524, size-exact 4524/4524, stub_zero_size 0, missing 0,
  wrong_type 0, extra 0, data_exact 843/843, imports 207, lint CLEAN ✓,
  drift 0, RATCHET PASS.**
* **Correction to the wave-94 handoff's `-debug-only=regalloc` analysis:** the
  local `llc` (LLVM 7.0.0, `-O2`) does **not** reproduce clang 7.0.2 `-Oz`
  codegen for this function. In the real build `%336` = `&Globals::status` is
  **not split at all** — it stays in `$r10`/`sl` for every one of its ten uses.
  The `%607`/`%608`/`%609` split + `%stack.4` spill described in the handoff is
  an artefact of the `-O2` model.
* A model that **does** reproduce the real `-Oz` allocation is documented in
  §5 (feed `llc` the IR clang's codegen actually sees, i.e. the last IR dump of
  `-mllvm -print-after-all`, not the `-emit-llvm -S` output).
* The whole diff is still **one register-allocation permutation** of
  `{r6, r8, r9, sl}` plus its spill cascade; the instruction stream is
  identical. ~70 new source shapes measured this pass, none beats 88.3 %.

## 1. What the real `-Oz` allocation looks like

`tools/verify/verify.py --show _ZN5Level11createSceneEv` at the baseline: 276
`|` lines, all inside the `missionPtr == 23` branch. The `missionPtr == 4`
branch is byte-identical; the first diff is the parts-loop preheader
(`0xb4830`).

Register maps recovered from the disassembly (all three variants have the
**same instruction sequence**; only the register names and spill slots differ):

| value | original | baseline (shipped) | all-globals |
|---|---|---|---|
| parts: mesh row `kHangarMeshes[sceneRace]` | `r9` | `r8` | `r8` |
| parts: `Globals::Canvas` | `r8` | `r9` | `sl` |
| parts: zero constant (`Vector{0,pi,0}`) | `sl` | `r4` | `r9` |
| parts: `k` | `r6` | spilled `sp#44` | `r6` |
| parts: `hangarPart` | `r4` (spilled `sp#44` around the child loop) | `r6` | `r4` |
| parts: `g` | `r5` | `r5` | `r5` |
| parts: child count | `fp` | `fp` | `fp` |
| parts: `&Globals::status` | spilled `sp#40` | spilled `sp#40` | spilled `sp#44` |
| ship: `&Globals::rnd` | **`r8`** (value reloaded `ldr r0,[r8]`) | spilled `sp#44` | `r9` |
| ship: `Globals::rnd` value | never cached | `r5` | never cached |
| ship: `capacity` | `r6` | `fp` | `r8` |
| ship: `type` | `fp` | `r8` | `fp` |
| ship: `race` | `r5` | `r6` | `r5` |
| ship: `i` | `r9` | `r9` | `sl` |
| ship: `&Globals::status` | `sl` | `sl` | `r6` |
| ship: `spotUsed` | `r4` | `r4` | `r4` |
| stack | `sp#44`=spots, `sp#24`=ownHangar, `sp#20`=globals, `sp#16`=flag, `sp#28`=shipCount | `sp#24`=spots, `sp#20`=ownHangar, `sp#16`=globals, `sp#12`=flag, `sp#28`=shipCount | `sp#32`=spots, `sp#28`=ownHangar, `sp#24`=globals, `sp#20`=flag, `sp#36`=shipCount |

Two independent facts fall out:

1. **The original has no local `AERandom *rnd`.** It keeps the *address*
   `&Globals::rnd` in `r8` and reloads `[r8]` at all six `nextInt` sites in the
   ship loop (`0xb49ae`, `0xb49be`, `0xb49ca`, `0xb49f6`, `0xb4a56`, `0xb4ad8`).
   The shipped source's local `rnd` caches the *value* in `r5` and pushes the
   address to `sp#44`. The all-globals source is therefore the right *shape*.
2. **The original has no local `Status *status` either.** It keeps the *address*
   `&Globals::status` in `sl` and reloads `[sl]` at all ten status sites
   (`ldr.w r0,[sl]` at `0xb4900`, `0xb4910`, `0xb4948`, `0xb49e6`, `0xb4a18`,
   and the early `0xb4342`/`0xb4652`/`0xb4758` group).

## 2. The real `-Oz` greedy allocation does **not** split `&Globals::status`

The wave-94 handoff (and the wave-89 note) analysed
`llc -mtriple=armv7-none-linux-androideabi -relocation-model=pic -O2` on the
`-emit-llvm -S` module. That model produces:

```
selectOrSplit GPR:%336 [720r,4016B:0)[4176B,5280B:0)[5600B,6256B:0)[6416B,11968B:0)
  ... Split 2 components: %608 [720r,2112r:0)  -> $r5
  ...                      %607 [2104r,...)    -> fi#4 (stack.4)
  ...                      %609 [3720r,...)    -> $r4
```

In the **real** clang 7.0.2 `-Oz` build the same interval is *not* split. From
clang's own `-mllvm -print-after-all` dumps (see §5 for the recipe):

```
# After Greedy Register Allocator:
704B  %335:tgpr = tLDRLIT_ga_pcrel @_ZN7Globals6statusE
720B  %607:gpr  = t2LDRi12 %335:tgpr, 0        ; (load 4 from got)
736B  %337:gpr  = t2LDRi12 %607:gpr, 0
1040B %344:gpr  = t2LDRi12 %607:gpr, 0
1344B %350:gpr  = t2LDRi12 %607:gpr, 0
1936B %361:gpr  = t2LDRi12 %607:gpr, 0
2112B %364:gpr  = t2LDRi12 %607:gpr, 0
3728B %406:gpr  = t2LDRi12 %607:gpr, 0
6624B %412:gpr  = t2LDRi12 %607:gpr, 0
7072B %426:gpr  = t2LDRi12 %607:gpr, 0
8896B %463:gpr  = t2LDRi12 %607:gpr, 0
9520B %473:gpr  = t2LDRi12 %607:gpr, 0

# After Virtual Register Rewriter: every one of the ten loads reads $r10.
```

So `&Globals::status` is a single unsplit interval in `$r10` (`sl`) in the real
build. The handoff's conclusion that "the whole-function allocator cascade is
driven by ONE interval: `%336` … spilled to `%stack.4`" is a property of the
`-O2` model, **not** of the binary. The `%607`/`%608`/`%609` numbering in the
handoff's log is the *split* of `%336`; the real build's `%607` is the
*unsplit* interval.

Why the model diverges: `-emit-llvm -S` stops before clang's codegen-only IR
passes. The IR that ISel actually consumes has extra `bitcast`/`getelementptr
i8` pairs (CodeGenPrepare) and different value numbering, and the greedy
allocator's heap order depends on that numbering. Feeding `llc` the
`-emit-llvm` IR yields `mov r10, r0` for `this` and `sub sp, #72` with a
different frame; feeding it the real pre-ISel IR yields `mov r6, r0`,
`ldr.w r10, [r0]` and the real spill slots.

## 3. The MachineLICM hoist is a register-pressure decision

`MachineLICMBase::IsProfitableToHoist` (LLVM 7.0.0
`lib/CodeGen/MachineLICM.cpp:1188`):

```cpp
bool CheapInstr = IsCheapInstruction(MI);
bool CreatesCopy = HasLoopPHIUse(&MI);
if (CheapInstr && CreatesCopy) return false;
if (TII->isTriviallyReMaterializable(MI, AA)) return true;   // <- t2ADDri? no
...
auto Cost = calcRegisterCost(&MI, false, false);
if (!CanCauseHighRegPressure(Cost, CheapInstr)) return true;
```

and `CanCauseHighRegPressure` (line 1149):

```cpp
for (const auto &RP : BackTrace)
  if (static_cast<int>(RP[Class]) + RPIdAndCost.second >= Limit)
    return true;
```

`t2ADDri %stack.1, 0` is **not** in `ARMBaseInstrInfo::isReallyTriviallyRe
Materializable`'s opcode list, so the hoist is decided by
`CanCauseHighRegPressure`: it is suppressed only when the loop's GPR pressure
is already at `RegLimit - 1`. That is the lever the original source pulled and
ours does not: the original's parts loop must have had **one more live GPR**
than ours at the hoist point. `-sink-insts-to-avoid-spills` bypasses the whole
question by sinking the instruction back into the loop after LICM.

The hoisted interval is hinted to `$r1` (its only use is `$r1 = COPY %541`), so
greedy enqueues it with the `(1u<<30)` hint-priority bit ahead of
`hangarPart`; it takes `r4`, which displaces `hangarPart` to `r6`, which spills
`k` to `sp#44`. That is the entire parts-loop residue.

## 4. New source-shape measurements (all no-flag)

Baseline: **88.3 % / 2252 B**. Every row is a full
`cmake --build --target link_so` + `tools/verify` measurement of
`_ZN5Level11createSceneEv`.

| variant | pct | size |
|---|---|---|
| baseline (shipped) | **88.3** | 2252 |
| all-globals (no local `rnd`) | 75.0 | 2228 |
| all-globals + explicit `const int *row = kHangarMeshes[sceneRace]` | 75.0 | 2228 |
| all-globals + `const int capacity` | 75.0 | 2228 |
| all-globals + `int ownHangar` / `int plainHangar` | 75.0 | 2228 |
| all-globals + `unsigned` `i`/`k`/`j`/`race`/`type`/`tries`/`spotUsed` | 75.0 | 2228 |
| all-globals + `i != n` loops | 75.0 | 2228 |
| all-globals + `spots` before `spotUsed` | 74.3 | 2228 |
| all-globals + `Status *status` local | 74.5 | 2224 |
| all-globals + `Globals *globals` local | 72.7 | 2228 |
| all-globals + `if` chain instead of `switch` | 70.1 | 2252 |
| all-globals + no `plainHangar` variable | 74.1 | 2224 |
| `const int *row` hoist (baseline) | 88.3 | 2252 |
| `int i` declared once for both loops | 88.3 | 2252 |
| `unsigned` loop counters (baseline) | 88.3 | 2252 |
| `int plainHangar` (baseline) | 88.3 | 2252 |
| `++i` / `i != n` (baseline) | 88.3 | 2252 |
| `const int capacity` / `const int mesh` (baseline) | 88.3 | 2252 |
| `char *spotUsed` instead of `unsigned char *` | 88.3 | 2252 |
| `tries > 0` instead of `tries != 0` | 88.3 | 2252 |
| `while` form of the retry loop | 88.3 | 2252 |
| `Vector` paren ctor / `AEMath::Vector` / named `rot` in loop | 88.3 | 2252 |
| `Vector{0, pi, 0}` (int zeros) | 88.3 | 2252 |
| `AEGeometry *g` / `PlayerStatic *hangarPart` declared outside the loop | 88.3 | 2252 |
| local `rnd` declared after `spots` | 88.3 | 2252 |
| `spots` computed before `ownHangar` (baseline) | 84.4 | 2252 |
| `capacity` before `ownHangar` | 79.8 | 2276 |
| `Globals::Canvas` hoisted into a local (parts loop) | 82.8 | 2252 |
| `Globals::Canvas` + row hoisted | 82.8 | 2252 |
| `kBarChildCounts`/`kBarChildMeshes` hoisted | 73.7 | 2252 |
| `plainHangar` removed, condition inlined | 86.1 | 2252 |
| `Vector rot;` outside the loop, `rot = Vector{…}` inside | 76.6 | 2280 |
| `Vector rot;` outside, `x/y/z =` inside | 85.3 | 2264 |
| `setRotation` after `new PlayerStatic` | 85.2 | 2256 |
| `static const Vector` | 72.6 | 2228 |
| local `rnd` declared at top of the `==23` block | 63.5 | 2192 |
| local `rnd` declared at function top | 59.9 | 2200 |
| local `rnd` declared before `ownHangar` | 71.9 | 2232 |
| `Status *status` local (baseline) | 72.6 | 2244 |
| `Globals *globals` local (baseline) | 70.0 | 2244 |

The allocator is remarkably stable: every semantically neutral rewrite of the
ship loop reproduces exactly 75.0/2228 (all-globals) or 88.3/2252 (baseline).
Only changes that alter the *live ranges* (hoisting `Globals::Canvas`, moving
`spots`, removing `plainHangar`, adding a `Status*`/`Globals*` local) move the
number, and all of them move it **down**.

## 5. Reproducing the real `-Oz` allocation with the local `llc`

The NDK clang is a release build (`-debug-only` unavailable), and `llc`
7.0.0 has no `-Oz`. But `llc -O2` on the **pre-ISel IR** reproduces the real
allocation. Recipe:

```bash
# 1. dump the IR clang's codegen actually consumes (last IR pass before ISel)
clang++ <the TU's exact flags> -S -mllvm -print-after-all \
        -mllvm -filter-print-funcs=_ZN5Level11createSceneEv \
        -o /dev/null src/game/world/Level.cpp 2> dumps.txt
#    the last "*** IR Dump After ..." block before
#    "Expand ISel Pseudo-instructions" is the function ISel sees.

# 2. splice that function body into the module emitted by -emit-llvm -S,
#    keep its !dbg/!tbaa metadata (use the module's own !7548 DISubprogram
#    and a valid DILocation), then:

llc -mtriple=armv7-none-linux-androideabi -relocation-model=pic -O2 \
    -debug-only=regalloc preisel.ll -o preisel.s
```

`llc` on the plain `-emit-llvm -S` module gives `mov r10, r0` (this) and
`sub sp, #72` with a *different* frame; on the pre-ISel IR it gives
`mov r6, r0`, `ldr.w r10, [r0]`, matching `lv_real.s` from the real clang
build. Scratch scripts: `/dev/shm/w4414/` (`emit.sh`, `minimize.py`,
`dump.py`, `score.py`, `harness.py`).

## 6. What this proves / what remains

**Proved:**

1. The diff is *only* register allocation: the pre-RA instruction stream of
   the `missionPtr == 23` branch is identical to the original's, and the
   `missionPtr == 4` branch is byte-identical.
2. The original source uses `Globals::rnd` and `Globals::status` **directly**
   (address in a callee-saved register, value reloaded at every use); it has
   no `AERandom *rnd` and no `Status *status` local.
3. The real `-Oz` build does **not** split `&Globals::status`; the wave-94
   handoff's `%336` split is an `-O2`-model artefact (§2).
4. The parts-loop residue is a `MachineLICM` **register-pressure** decision on
   `t2ADDri %stack.1, 0`, not a "trivially rematerializable" fast path (§3).
5. The allocator permutation `{r6, r8, r9, sl}` is stable under every
   semantically neutral source rewrite tried (~70 this pass, ~300 cumulative).

**Remaining levers:**

1. A source shape that raises the parts loop's GPR pressure by exactly one at
   the hoist point (so `CanCauseHighRegPressure` returns true and the `t2ADDri`
   stays in the loop body). Nothing tried so far does this without also
   changing the instruction stream.
2. A source shape that gives `&Globals::rnd` `r8` (not `r9`) in the
   all-globals shape — i.e. an allocation-order flip of
   `{rnd, status, capacity, i}` from `{r9, r6, r8, sl}` to `{r8, sl, r6, r9}`.
   The greedy heap order depends on vreg numbering, which the source does not
   appear to control from C++ level.
3. The all-globals shape is 24 B shorter (2228 vs 2252) because it uses 16-bit
   encodings where the original uses 32-bit (`movs r4,#0` vs `mov.w sl,#0`,
   `adds r1,r6,#1` vs `add.w r1,r8,#1`); even a perfect RA match must also
   reproduce the original's high-register choices to keep 2252 B.
