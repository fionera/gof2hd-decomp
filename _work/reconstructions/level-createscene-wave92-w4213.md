# `Level::createScene` — wave-92 re-check of the MachineLICM hoist (w4213)

**Status:** research note, 2026-11-03. Worktree `wt/w4213`, branch `wave92-w4213`,
base `b379d00d`. **No source change landed** — no source shape found that raises
`byte_exact`. This commit contains only this note.

Follow-up to `level-createscene-wave91-w4113.md` (which followed
`level-createscene-phi-use-w4013.md` and `level-createscene-licm-hoist-w3833.md`).
This pass re-derived the hoist with `-mllvm -print-after-all` (the NDK clang 7.0.2
driver does **not** accept `-debug-only`, but does accept `-print-after-all`),
re-confirmed the sink-flag residue is the ship loop, and measured ~450 more
variants (2×2^8 rnd-site matrices, three random searches, and the declaration /
type / debug-info sweeps below).

## TL;DR

* Target `_ZN5Level11createSceneEv`: **88.3 %**, orig/ours both **2252 B**,
  `byte=False`, `linked=False`.
* Gate unchanged: **byte_exact 4498/4524, linked_exact 4498/4524,
  size-exact 4524/4524, stub_zero_size 0, missing 0, wrong_type 0, extra 0,
  data_exact 843/843, imports 207, `.text 0x1337c8`, `.rodata 0x41b67`,
  `.ARM.exidx 0x6f70`, `.ARM.extab 0x26290 (-8)`, lint CLEAN ✓, drift 0,
  RATCHET PASS.**
* The 291 structural diff lines are **all in the `missionPtr == 23` branch**
  and are **only register names / spill slots** — no encoding, scheduling,
  constant, callee or field-offset diff. The `missionPtr == 4` branch is
  byte-identical.
* Root cause (unchanged): MachineLICM hoists
  `%541 = t2ADDri %stack.1, 0` (the frame address of the parts-loop
  `Vector{0, π, 0}` temporary) from the loop body `bb.14` to the preheader
  `bb.8`. The hoisted interval is hinted to `$r1`, misses (the body range
  interferes with `uxth r1` at the `AEGeometry` ctor), and takes `r4`. That
  displaces `hangarPart` to `r6`, which spills `k` to `sp#44` — the one extra
  instruction in the parts loop. The same cascade spills `&Globals::rnd` in the
  ship loop.
* `-mllvm -sink-insts-to-avoid-spills` makes the **parts loop byte-identical**
  and lifts the function to **93.3 % / 2252 B**, but the ship loop still differs
  (`&Globals::rnd` spilled to `sp#44` vs `r8`; `capacity` in `fp` vs `r6`;
  `type` in `r8` vs `fp`). The flag is per-TU and re-sizes 12 other `Level.cpp`
  functions, so it is not usable as-is.

## 1. The diff, classified

`tools/verify/verify.py --show _ZN5Level11createSceneEv` (baseline build):

```
Level::createScene()
  orig 2252B @0xb42ec  |  ours 2252B @0xb42ec
  ... 291 '|' lines ...
```

* First diff: parts-loop preheader `0xb4830`,
  `add.w r9, r0, r2, lsl #4` (orig, row→`r9`) vs
  `add.w r8, r0, r2, lsl #4` (ours, row→`r8`).
* Parts loop: ours has one extra `str r6, [sp, #44]` (spill `k`), uses `r4`
  for the zero constant and `r6` for `hangarPart`; the original keeps `k` in
  `r6`, `hangarPart` in `r4`, zero in `sl` (see §2).
* Ship loop: ours loads `&Globals::rnd` into `r2`, spills it to `sp#44`, and
  uses `fp` for `capacity`, `r8` for `type`; the original keeps `&Globals::rnd`
  in `r8`, `capacity` in `r6`, `type` in `fp`.
* Everything else (both `missionPtr == 4` and the rest of `missionPtr == 23`)
  is byte-identical. There is no missing/extra code, wrong constant, wrong
  callee, or wrong field offset.

## 2. The hoist (MIR evidence)

`-mllvm -print-after-all` on the baseline (11.3 M lines; createScene's machine
passes start at line 7 496 399). After **Early Machine Loop Invariant Code
Motion** the address is in the preheader `bb.8`:

```
bb.8 (%ir-block.101):
  ...
  %528:tgpr  = tLDRLIT_ga_pcrel @_ZZN5Level11createSceneEvE13kHangarMeshes
  %529:gprnopc = t2ADDrs %528:tgpr, %3:rgpr, 34
  %539:tgpr  = tLDRpci %const.1                 ; pi
  %541:gprnopc = t2ADDri %stack.1, 0            ; &Vector temp  <-- hoisted
  ...
bb.14 (%ir-block.123):
  ...
  $r1 = COPY %541:gprnopc                       ; only use
  tBL ... @_ZN10AEGeometry11setRotationERKN11AbyssEngine6AEMath6VectorE
```

After **Greedy Register Allocator** the hoisted value is rematerialized at the
use, so the final asm still has `add r1, sp, #56` in the body; the damage is
the transient long-lived interval:

```
selectOrSplit GPRnopc:%541 [3568r,3712B:0)[4176B,5280B:0)[5600B,6256B:0)[6416B,6608B:0)
hints: $r1
missed hint $r1
assigning %541 to $r4: R4
...
selectOrSplit GPR:%44 ...  ; k
evicting $r6 ... assigning %44 to $r6
```

`t2ADDri` is `isReMaterializable = 1` in LLVM 7's
`ARMInstrThumb2.td` (`T2I_bin_ii12rs`: *"The register-immediate version is
re-materializable. This is useful in particular for taking the address of a
local."*; the `ri12` form is not). `MachineLICMBase::IsProfitableToHoist`
returns `true` unconditionally for trivially rematerializable instructions, so
the register-pressure guard is never reached. `-mllvm
-hoist-cheap-insts=false` and `-mllvm -hoist-const-stores=false` therefore have
**no effect** (measured: still 88.3 % / 2252 B).

## 3. The sink flag: parts loop exact, ship loop not

`-mllvm -sink-insts-to-avoid-spills` runs `MachineLICMBase::SinkIntoLoop()`,
whose criterion is "the hoisted value's only uses are copies". `%541`'s only
use is `$r1 = COPY %541`, so it is sunk back into the body. After RA the
instruction becomes a direct physical def:

```
$r1 = t2ADDri %stack.1, 0      ; no COPY, no hint, no long interval
```

With this flag the parts loop is **instruction-for-instruction the original**,
and the function scores **93.3 % / 2252 B** (best of the whole 2^8 matrix, see
§4). The remaining 255 diff lines are the ship loop:

| | original | sink build |
|---|---|---|
| `&Globals::rnd` | `r8` (value reloaded `ldr r0,[r8]`) | spilled `sp#44` |
| `capacity` | `r6` | `fp` |
| `type` | `fp` | `r8` |
| `spots` slot | `sp#44` | `sp#24` |
| `ownHangar` slot | `sp#24` | `sp#20` |
| `Globals::globals` slot | `sp#20` | `sp#16` |

Both loops reload `Globals::rnd` at every `nextInt` site (the original does
`ldr r0,[r8]` at all eight sites), so the *shape* is right; only the greedy
allocator's choice of which of `{&rnd, capacity, type}` gets the callee-saved
registers differs.

### Per-TU cost of the flag (why it cannot simply be enabled)

With `-mllvm -sink-insts-to-avoid-spills` on `Level.cpp`, `byte_exact` drops
**4498 → 2361** and `linked_exact` **4498 → 4484**. Twelve `Level.cpp`
functions change size:

| function | before | after |
|---|---|---|
| `createMission` | 17172 | 17192 |
| `assignGuns` | 2948 | 2968 |
| `createSpace` | 3076 | 3108 |
| `updateOrbit` | 968 | 972 |
| `createWingmen` | 660 | 652 |
| `createAsteroids` | 1588 | 1608 |
| `createGasClouds` | 684 | 672 |
| `getBoundingVolume` | 536 | 552 |
| `createStaticObject` | 6960 | 6968 |
| `initParticleSystems` | 1872 | 1880 |
| `updateMissionOrbit` | 1160 | 1156 |
| `createCampaignMission` | 29676 | 29680 |

There is no per-function `-mllvm` switch in clang 7.0.2, and moving
`createScene` to its own TU would change `.rodata` layout and the `.plt` order
(the in-source comment records that it must stay in `Level.cpp` for the PLT
order).

## 4. The 2^8 `rnd`-site matrices

Eight `nextInt` sites, each either a local `AbyssEngine::AERandom *rnd =
Globals::rnd` (1) or `Globals::rnd` directly (0), bit order
`shipCount, top, nextInt(4), inner nextInt(100), pick, spot, tries, rotation`.

* **No flag** (`/dev/shm/w4213/matrix.log`): best **88.3 % / 2252 B** at
  `01001010` (local at top / pick / tries; globals at shipCount / nextInt(4) /
  inner / spot / rotation) — the current source. Nothing else beats it.
* **With sink flag** (`/dev/shm/w4213/matrix_sink.log`): best
  **93.3 % / 2252 B** at `01100010` (local at top and tries, globals
  elsewhere). All-globals (`00000000`) is only 75.9 % / 2232 B; the baseline
  mix (`01001010`) is 89.3 % / 2256 B.

## 5. New measurements this pass (all no-flag unless stated)

Baseline: **88.3 % / 2252 B**. None of the following beat it.

### 5.1 `Vector` temporary shapes (all IR-identical, 88.3 % / 2252 B)

`Vector rot{…}; g->setRotation(rot)`, `Vector rot(…);`, `g->setRotation(Vector(…))`,
`const Vector rot(…);`, `Vector rot; rot.x=…; rot.y=…; rot.z=…;`,
`Vector rot = {…};`, `const Vector *rp = &rot; g->setRotation(*rp);`,
`Vector rots[1] = {{…}}; g->setRotation(rots[0]);`, `Vector &rr = rot;`,
`g->setRotation(*(const Vector *)&rot)`, `volatile Vector` + cast,
`struct { Vector v; RotHolder() : v(…) {} } holder; g->setRotation(holder.v);`,
`Vector rots[1]; rots[0].x=…; g->setRotation(rots[0])`.
Worse: a non-trivial `~RotHolder()` 66.3 % / 2288; `g->setRotation(0.0f, π, 0.0f)`
(the 3-float overload) 73.2 % / 2172; `volatile Vector *` 79.9 % / 2260.

### 5.2 Loop shapes

`if (mesh >= 0) { … }` instead of `if (mesh < 0) continue` — 88.3 %;
`++k` — 88.3 %; `unsigned k` — 88.3 %; `while (k < 4)` — 88.1 %;
`if (mesh == -1) continue` — 88.2 %; `const int *row = kHangarMeshes[sceneRace];
mesh = row[k]` — 88.3 %; pointer-walking `for (k=0; k<4; k++, row++)` —
84.6 % / 2260.

### 5.3 Loop-PHI escapes (suppress the hoist, but cost code)

`const Vector *p; … g->setRotation(*p); p = &rot;` (undef first iteration) —
86.0 % / 2252 (address materialises in the latch).
`const Vector *p = &rot; … p = &rot;` before/after the use — 83.9 % / 2260.
`if (mesh >= 0) p = &rot;` — 83.9 % / 2260. `*p = rot;` — 84.0 % / 2272.
All are worse than the 4-byte `k` spill they remove.

### 5.4 Ship-loop / declaration / type sweeps

`const int capacity`, `unsigned capacity`, `const int shipCount0`, `unsigned
race/type/spot/tries`, `int ownHangar`, `bool *spotUsed`, `const int *const
spots`, `createShip(...)` without `this->`, `++i`, `Station *station =
Globals::status->getStation()`, `Globals *globals = Globals::globals`,
`Status *status = Globals::status`, `int spot; spot = …;`, `while` retry loop,
moving the `rnd` declaration before the parts loop / before `capacity` /
before `ownHangar` / inside the loop, `AbyssEngine::AERandom *const rnd`,
`AERandom *&rnd = Globals::rnd`, `AERandom &rnd = *Globals::rnd` (with
`rnd.nextInt`) — all 88.3 % or worse (declaration moves are 63–87 %).
`short`/`unsigned char` variants of `type`, `capacity`, `race`, `spot`,
`shipCount`, `i` are 76–93 % (with the flag) and 76–87 % (without), never
better than the baseline.

### 5.5 Random searches

Three random searches over independent small edits (declaration form, type,
`spots` placement, `rnd` site bits) — 120 + 150 + 160 variants, all measured;
no variant exceeded 88.3 % without the flag or 93.3 % with it.

### 5.6 `-g` does not matter here

The build uses `-g`, but compiling the baseline with `-g0` gives the same
88.3 % / 2252 B (and the `01100010` source with `-g0` gives the same
85.0 % / 2248 B as with `-g`). Declaration-form changes such as `int race =
sceneRace;` → `int race; race = sceneRace;` are also IR-identical and do not
move the score. The debug-info lever is therefore **not** available for this
function.

## 6. Conclusion / remaining levers

The parts loop is fully determined by the hoisted `%541`; the ship loop is a
second, independent greedy-RA decision. Both must be fixed for `byte_exact`.

Remaining levers, in order of plausibility:

1. **A source shape that gives `%541` a loop PHI use for free.** The
   `CheapInstr && CreatesCopy` early-out in `IsProfitableToHoist` is the only
   way to stop the hoist without adding code; every PHI shape measured so far
   adds a `mov`/copy (2260 B).
2. **A source shape that makes `%541` non-rematerializable while still
   lowering to `add r1, sp, #56`.** A bare `FrameIndex` always selects
   `t2ADDri FI, 0` (`ARMISelDAGToDAG.cpp`, `case ISD::FrameIndex`); `t2ADDri12`
   needs a non-modified-immediate addend, which would move the address away
   from `sp+56`.
3. **Reproduce the ship loop's `&Globals::rnd → r8` / `capacity → r6` /
   `type → fp` allocation.** The 2^8 local-vs-global matrix is exhausted;
   the current mix is the best measured.
4. A per-function sink flag (not available in clang 7.0.2) would give 93.3 %,
   not 100 %, because of lever 3.

## 7. Scratch artefacts (not committed)

`/dev/shm/w4213/`: `Level_base.cpp` (pristine `Level.cpp`), `tryvar.py`,
`fastscore.py`, `genvar.py`, `matrix.log` / `matrix_sink.log` (the two 2^8
matrices), `show_now.txt` (baseline diff), `show_sinkbest.txt` (93.3 % diff),
`paa.log` (11.3 M-line `-print-after-all` baseline), `paa_sink.log` (sink
variant), `mir_licm_base.mir`, `ra_novregs_base.mir`, `ra_novregs_sink.mir`,
`d_orig.txt` / `d_ours.txt` (instruction-only dumps), `batch*.py`,
`randsearch*.py`, `v_sinkbest.cpp` (`01100010`), `v_sinkall.cpp`.
