# `Level::createScene` — wave-92 w4249: source-shape search for the MachineLICM hoist

**Status:** research note, 2026-11-03. Worktree `wt/w4249`, branch `wave92-w4249`,
base `b379d00d`. **No source change landed** — no source shape found that raises
`byte_exact`. This commit contains only this note.

Follow-up to `level-createscene-wave92-w4213.md` (which followed
`level-createscene-wave91-w4113.md`, `level-createscene-phi-use-w4013.md`,
`level-createscene-licm-hoist-w3833.md`, `level-createscene-88-3-2252-b_w3813.md`).

## TL;DR

* Target `_ZN5Level11createSceneEv`: **88.3 %**, orig/ours both **2252 B**,
  `byte=False`, `linked=False`.
* Gate unchanged: **byte_exact 4499/4524, linked_exact 4499/4524,
  size-exact 4524/4524, stub_zero_size 0, missing 0, wrong_type 0, extra 0,
  data_exact 843/843, imports 207, lint CLEAN ✓, drift 0, RATCHET PASS.**
  (The task text quotes 4498; the checked-in baseline is 4499 and ratchet passes.)
* ~60 new source shapes measured this pass (parts-loop temporaries, ship-loop
  declarations/types, loop-PHI shapes, `capacity`/`type` forms). **None beats
  88.3 %** without the sink flag.
* Reproduced the w4213 sink-flag result and **corrected the mask**: the
  **93.3 % / 2252 B** variant is mask **70** (`01000110`, local `rnd` at
  `top`, `nextInt(4)`, `tries`), not 98. With the flag the parts loop is
  **byte-identical** to the original.
* New measurement: the **TU-wide cost of the flag** is
  `byte_exact 4499 -> 2357`, `linked_exact 4499 -> 4485`. Only **14**
  `Level.cpp` functions change size, but the resulting `.text` shift re-addresses
  the literal pools of ~2100 functions, so `byte_exact` collapses.
* New measurement: with the flag the ship-loop residue is exactly **"one
  callee-saved register short"**: `capacity r6 -> fp`, `type fp -> r8`,
  `&Globals::rnd r8 -> sp#44`, and the local `rnd` *value* sits in `r5`.
  The original has **no `rnd` register at all** — it keeps `&Globals::rnd` in
  `r8` and reloads `[r8]` at all eight `nextInt` sites.
* The all-globals source (no local `rnd`, i.e. the original's apparent shape)
  removes the ship-loop spill and the parts-loop `k` spill, but scores only
  **75.0 % / 2228 B** because the whole function's register assignment is a
  different permutation.

## 1. The two residues (disassembly)

`tools/verify/verify.py --show _ZN5Level11createSceneEv` at the baseline:
276 `|` lines, all inside the `missionPtr == 23` branch, all register names /
spill slots. The `missionPtr == 4` branch is byte-identical.

### 1.1 Parts loop — one extra spill

Original (`0x574..0x5a8`):

```
00000574: ldr   r0, [pc, #808]
00000576: add   r1, sp, #56          ; &Vector{0, pi, 0}
00000578: str   r0, [sp, #60]        ; pi
0000057a: mov.w sl, #0
0000057e: mov   r0, r5
00000580: str.w sl, [sp, #56]
00000584: str.w sl, [sp, #64]
00000588: blx   setRotation
0000058c: mov.w r0, #304
00000590: blx   operator new
00000594: mov   r4, r0               ; hangarPart -> r4
...
000005a6: str   r4, [sp, #44]
```

Ours:

```
00000574: ldr   r0, [pc, #808]
00000576: add   r1, sp, #56
00000578: str   r0, [sp, #60]
0000057a: movs  r4, #0
0000057c: mov   r0, r5
0000057e: str   r4, [sp, #56]
00000580: str   r6, [sp, #44]        ; <-- extra: spill k
00000582: str   r4, [sp, #64]
00000584: blx   setRotation
00000588: mov.w r0, #304
0000058c: blx   operator new
00000590: mov   r6, r0               ; hangarPart -> r6
```

The hoisted `%513 = t2ADDri %stack.1, 0` (frame address of the parts-loop
`Vector`) is hinted `$r1`, misses (its live range crosses `uxth r1, r4` at the
`AEGeometry` ctor), and is assigned `$r4`. That displaces `hangarPart` from
`r4` to `r6`, which spills `k` (`r6`) to `sp#44`. After RA the hoisted def is
rematerialised, so the instruction itself is the correct `add r1, sp, #56`;
only the transient interval does damage.

### 1.2 Ship loop — three-register permutation

Original ship-loop preheader (`0x68e..0x6c0`) and body (`0x6c2..`):

```
0000068e: ldr   r0, [pc, #556]
00000690: mov.w r9, #0               ; i = 0
00000694: ldr   r2, [pc, #552]
00000696: ldr   r1, [sp, #52]        ; sceneRace
00000698: add   r0, pc
0000069a: add   r2, pc
0000069c: vldr  s16, [pc, #508]
000006a0: cmp   r1, #7
000006a2: it    ne
000006a4: movne r2, r0               ; spots
000006a6: uxth  r0, r1
000006a8: movw  r1, #267
000006ac: lsr.w r0, r1, r0
000006b0: str   r2, [sp, #44]        ; spots -> sp#44
000006b2: and.w r0, r0, #1
000006b6: str   r0, [sp, #16]        ; "special table" flag
000006b8: ldr   r0, [pc, #520]
000006ba: add   r0, pc
000006bc: ldr   r0, [r0, #0]
000006be: str   r0, [sp, #20]        ; Globals::globals
000006c0: b.n   0x832
000006c2: ldr.w r0, [r8]             ; r8 = &Globals::rnd  (reload every site)
```

Ours:

```
00000688: ldr   r0, [pc, #560]
0000068a: mov.w r9, #0
0000068e: ldr   r2, [pc, #560]
00000690: ldr   r1, [sp, #52]
00000692: add   r0, pc
00000694: add   r2, pc
00000696: vldr  s16, [pc, #516]
0000069a: cmp   r1, #7
0000069c: it    ne
0000069e: movne r2, r0
000006a0: uxth  r0, r1
000006a2: movw  r1, #267
000006a6: lsr.w r0, r1, r0
000006aa: str   r2, [sp, #24]        ; spots -> sp#24
000006ac: and.w r0, r0, #1
000006b0: str   r0, [sp, #12]        ; flag -> sp#12
000006b2: ldr   r0, [sp, #44]        ; &Globals::rnd spilled
000006b4: ldr   r5, [r0, #0]         ; rnd value -> r5
000006b6: ldr   r0, [pc, #524]
000006b8: add   r0, pc
000006ba: ldr   r0, [r0, #0]
000006bc: str   r0, [sp, #16]        ; Globals::globals
```

| value | original | ours |
|---|---|---|
| `&Globals::rnd` | `r8` | `sp#44` |
| `rnd` (the value) | not kept (reload `[r8]`) | `r5` |
| `capacity` | `r6` | `fp` |
| `type` | `fp` | `r8` |
| `spots` | `sp#44` | `sp#24` |
| `ownHangar` | `sp#24` | `sp#20` |
| `Globals::globals` | `sp#20` | `sp#16` |

Ours is exactly one callee-saved register short because it keeps the local
`rnd` value in `r5`; the original keeps the *address* `&Globals::rnd` in `r8`
and reloads the value at every site.

## 2. New source-shape measurements (all no-flag unless stated)

Baseline: **88.3 % / 2252 B**. Every row below is a full
`cmake --build --target link_so` + `tools/verify` measurement of
`_ZN5Level11createSceneEv`.

### 2.1 Parts-loop temporaries / structure

| variant | pct | size |
|---|---|---|
| baseline `Vector{0.0f, 3.1415927f, 0.0f}` temporary | **88.3** | 2252 |
| `Vector(…)` paren ctor | 88.3 | 2252 |
| `AbyssEngine::AEMath::Vector{…}` | 88.3 | 2252 |
| `const Vector rot{…}; g->setRotation(rot);` | 88.3 | 2252 |
| `Vector rot{…}; g->setRotation(rot);` | 88.3 | 2252 |
| `g` / `hangarPart` / `child` declared `*const` | 88.3 | 2252 |
| `g` / `hangarPart` / `mesh` / row-pointer hoisted out of the loop | 88.3 | 2252 |
| `KIPlayer *hangarPart = new PlayerStatic(…)` (no cast) | 88.3 | 2252 |
| `int plainHangar` instead of `bool` | 88.3 | 2252 |
| `++k` instead of `k++` | 88.3 | 2252 |
| `if (mesh >= 0) { … }` block instead of `continue` | 88.3 | 2252 |
| `AEGeometry &gr = *g; gr.setRotation(…)` | 88.3 | 2252 |
| `Vector rot;` outside the loop, `rot = Vector{…}` inside | 76.6 | 2280 |
| `Vector rot;` outside, `rot.x/y/z = …` inside | 85.3 | 2264 |
| `setRotation` moved after `new PlayerStatic` | 85.2 | 2256 |
| `static const Vector rot{…}` (rodata address) | 72.6 | 2228 |
| `ArrayAdd((KIPlayer *) new PlayerStatic(…), …)` (no variable) | 62.7 | 2236 |

### 2.2 Ship-loop declarations / types

| variant | pct | size |
|---|---|---|
| baseline (local `rnd` at top/pick/tries, `Globals::rnd` elsewhere) | **88.3** | 2252 |
| `spots` moved into the `default:` arm (LICM'd back out) | 88.3 | 2252 |
| `spots` via `if (sceneRace == 7)` instead of `?:` | 86.5 | 2252 |
| `int *spots` (non-const) | 86.5 | 2252 |
| `const int &capacity = kHangarCapacity[sceneRace]` | 88.3 | 2252 |
| `capacity` inlined as `kHangarCapacity[sceneRace]` at every use | 88.3 | 2252 |
| `short capacity` | 83.8 | 2268 |
| `unsigned type` | 88.3 | 2252 |
| `table` hoisted before the loop | 72.1 | 2232 |
| no local `rnd` at all (all eight sites `Globals::rnd`) | 75.0 | 2228 |
| local `rnd` at top + pick only | 66.0 | 2236 |
| local `rnd` at top + spot + tries (mask 98) | 88.1 | 2252 |
| local `rnd` at top + pick + tries + rotation (mask 210) | 87.7 | 2252 |
| `AERandom *&rnd = Globals::rnd;` (reference to the global pointer) | 75.0 | 2228 |
| `AERandom &rnd = *Globals::rnd;` with `rnd.nextInt(…)` | 88.3 | 2252 |
| local `rnd` declared but only `(void)`-used | 75.0 | 2228 |

The full 2^8 local-vs-`Globals::rnd` matrix (`/dev/shm/w4249/matrix_new.log`,
bit order `shipCount, top, nextInt(4), inner, pick, spot, tries, rotation`)
still peaks at **88.3 % at masks 82 and 83** (the shipped source); the runner-up
is 88.1 at 98/99.

### 2.3 Loop-PHI shapes (the "free PHI use" idea)

`MachineLICMBase::IsProfitableToHoist` refuses to hoist a *cheap* instruction
that has a loop-PHI use (`CheapInstr && CreatesCopy`, MachineLICM.cpp:1211).
`t2ADDri` is `isAsCheapAsAMove`-cheap (`ARMInstrThumb2.td`, `T2I_bin_ii12rs`
`ri`), so a PHI use of the `Vector*` would suppress the hoist without any flag.
`HasLoopPHIUse` walks virtual defs and looks through copies inside the loop.

| shape | pct | size |
|---|---|---|
| `Vector rot; const Vector *p = &rot;` before loop, `p = &rot` in loop | 76.6 | 2280 |
| `p = nullptr` before loop, `p = &rot` in loop | 76.6 | 2280 |
| same + `(void) p` after the loop (exit-block PHI attempt) | 88.3 | 2252 |
| `Vector rot{…}` in loop, `p = &rot`, `(void) p` after loop | 88.3 | 2252 |
| `Vector rotA, rotB; Vector &rot = plainHangar ? rotA : rotB;` | 81.4 | 2292 |
| `const Vector *p; … setRotation(*p); p = &rot;` (undef latch) | 86.0 | 2252 |

Every PHI shape either gets folded by `OptimizePHIs` (which runs *before*
Early MachineLICM in the codegen pipeline: pass 45 vs pass 50) or costs a
`mov`/extra register. The `&rot`/`&rot` PHI is a single-value PHI and folds;
the `nullptr`/`&rot` PHI survives but materialises the null.

## 3. The sink flag: corrected mask and true cost

### 3.1 Corrected mask

`-mllvm -sink-insts-to-avoid-spills` on `Level.cpp`:

| mask | local `rnd` at | pct | size |
|---|---|---|---|
| 70 (`01000110`) | top, nextInt(4), tries | **93.3** | **2252** |
| 78 (`01001110`) | top, nextInt(4), pick, tries | 92.2 | 2252 |
| 86 (`01010110`) | top, nextInt(4), spot, tries | 92.4 | 2252 |
| 82 (`01010010`) | top, pick, tries (shipped source) | 89.3 | 2256 |
| 98 (`01100010`) | top, spot, tries | 89.3 | 2256 |
| 210 (`11010010`) | top, pick, tries, rotation | 89.8 | 2252 |
| 0 / 1 (all globals) | — | 75.0 | 2228 |

The w4213 note's `01100010` string is bit-0-leftmost; as a mask integer it is
**70**, not 98. With mask 70 the parts loop is **byte-identical** (the diff
starts only at the ship-loop preheader, `0x68e`).

### 3.2 The ship-loop residue under the flag

With mask 70 + flag the ship loop still has

```
000006b2: ldr   r0, [sp, #44]     ; &Globals::rnd spilled
000006b4: ldr   r5, [r0, #0]      ; rnd value -> r5
...
capacity -> fp, type -> r8, spots -> sp#24
```

i.e. exactly the same three-register permutation as §1.2. Sinking the parts
loop address does not touch it: it is an independent greedy-RA decision.

### 3.3 TU-wide cost of the flag (measured)

```
cmake --build cmake-build-match --target check
  FAIL  byte_exact 4499 -> 2357
  FAIL  linked_exact 4499 -> 4485 (beyond the +-1 band)
```

Only **14** functions are no longer linked-exact, but because
`-ffunction-sections` + the section-ordering file fix each function's address
only when all earlier functions keep their size, the `.text` shift re-addresses
the literal pools of ~2140 functions and `byte_exact` collapses. The flag is
therefore unusable without simultaneously re-matching all 14 functions.

## 4. What this proves / what remains

**Proved:**

1. The whole `createScene` diff is register allocation, not code shape. The
   `missionPtr == 4` branch is already byte-identical.
2. The parts-loop residue is exactly one spilled `k` caused by the MachineLICM
   hoist of `%513 = t2ADDri %stack.1, 0`; the hoist is a trivially
   rematerialisable instruction, so `IsProfitableToHoist` returns `true` before
   the register-pressure guard and `-hoist-cheap-insts=false` cannot help.
3. The only in-tree mechanism that removes the hoist is
   `-sink-insts-to-avoid-spills` (its `SinkIntoLoop` criterion is "all uses are
   copies", which `%513` satisfies). It makes the parts loop byte-identical
   but is TU-wide and costs 2142 `byte_exact`.
4. The ship-loop residue is independent: the original has **no local `rnd`
   register** (it keeps `&Globals::rnd` in `r8` and reloads at all eight
   sites), while every source form that keeps a local `rnd` spends `r5` on the
   value and spills the address. Removing the local entirely (all-globals)
   removes both spills but produces a different whole-function register
   permutation (75.0 %).

**Remaining levers** (none landed):

1. A source shape that gives `%513` a *free* loop-PHI use (surviving
   `OptimizePHIs`, no extra `mov`). Every measured shape costs >= 8 B or folds.
2. A source shape that makes the `Vector` address non-rematerialisable while
   still selecting `t2ADDri` (2-byte `add r1, sp, #56`). `t2ADDri12`/`addw`
   would be 4 bytes and is not the original encoding.
3. Reproduce the ship loop's `&rnd -> r8 / capacity -> r6 / type -> fp`
   allocation. The 2^8 local-vs-global matrix and all declaration/type forms
   tested are exhausted; the original's "no local `rnd`" shape is the right
   *shape* but yields a different permutation.
4. A per-function sink flag (not available in clang 7.0.2) would give 93.3 %,
   and would still need lever 3 for 100 %.

## 5. Scratch artefacts (not committed)

`/dev/shm/w4249/`: `Level.cpp.base` (pristine `Level.cpp`), `fscore.py`
(single-function score), `dump.py` (address-annotated disasm), `apply.py`,
`genmask.py`, `matrix_new.log` (no-flag 2^8 matrix), `show_now.txt` (baseline
diff), `show_sink70.txt` (flag+mask-70 diff), `show_ag2.txt` (all-globals diff),
`orig_full.txt` / `ours_full.txt` / `ours_ag2.txt` (full disassembly),
`batch1.py` … `batch6.py` (the variant sweeps tabulated above).
