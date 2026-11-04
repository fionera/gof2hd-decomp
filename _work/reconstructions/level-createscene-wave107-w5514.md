# `Level::createScene` — wave-107 w5514: ~40 further source shapes, still 88.3 %;
# the per-TU sink flag is *not* landable (gate byte 4500 → 2357)

**Status:** research note, 2026-11-04. Worktree `wt/w5514`, branch
`wave107-w5514`, base `f857b5171`. **No source change landed** — no source
shape found that raises either `byte_exact` or the fuzzy score while keeping
the 2252-byte size. This commit contains only this note.

Follow-up to `level-createscene-wave106-w5420.md` (which followed
`…-wave105-w5308.md`, `…-wave103-wf312.md`, `…-wave102-wf212.md`,
`…-wave101-wf111.md`, `…-wave100-w5012.md`, `…-wave99-w4911.md`,
`…-wave98-w4814.md`, `…-wave97-w4712.md`, `…-wave95-w4514.md`,
`…-wave94-w4414.md`, `…-wave92-w4249.md`, `…-wave92-w4213.md`,
`…-wave91-w4113.md`, `level-createscene-phi-use-w4013.md`,
`level-createscene-licm-hoist-w3833.md`, `Level-createScene-flag-free.md`,
`level-createscene-88-3-2252-b_w3813.md`).

## TL;DR

* Target `_ZN5Level11createSceneEv`: **88.3 %**, orig/ours both **2252 B**,
  783/791 instructions, `byte=False`, `linked=False`, first diff at instruction
  460 (`0xb4830`, the parts-loop preheader `add.w r9/r8, r0, r2, lsl #4`).
  Unchanged from waves 88–106.
* Gate re-measured before and after this pass, green:
  **byte_exact 4500/4524, linked_exact 4500/4524, size-exact 4524/4524,
  stub_zero_size 0, missing 0, wrong_type 0, extra 0, data_exact 843/843,
  imports 207, lint CLEAN ✓, drift 0, RATCHET PASS.**
* Re-confirmed the `-mllvm -sink-insts-to-avoid-spills` upper bound on this
  exact base: **89.3 % / 2256 B / 783 vs 790 insns** (first structural diff
  moves to the ship loop; the only earlier `|` is the branch-target shift at
  instruction 28 caused by the +4 B). **New this pass:** the flag applied to
  the *whole* `Level.cpp` TU is **not landable** — the gate collapses to
  `byte_exact 2357 / linked_exact 4486` (§2). So the flag cannot be used even
  as a per-TU override (unlike the `StarMap.cpp` branch-weight precedent).
* ~40 further source shapes measured this pass (canvas hoisting, `Vector`
  construction/scope/type permutations, declaration-order permutations, PHI
  attempts, loop-form permutations, `short`/`unsigned` types). **None beats
  88.3 % at 2252 B.** Most are byte-identical to the shipped base.
* Root cause unchanged: the parts-loop residue is the MachineLICM hoist of
  `%541 = t2ADDri %stack.1, 0` (the `Vector{0, π, 0}` temp address) plus the
  `t2MOVi` size/arg constants; the ship loop is the `Globals::rnd` value
  cache. §2.

## 1. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5514
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5Level11createSceneEv
```

Scratch is `/dev/shm/w5514` only (not committed):

* `score.py` — official single-function score (`pct osize usize ninsn firstdiff`);
  `score.py -v` prints the full diff list (268 differing instructions).
* `try.py <spec.json>` — applies `{name: [[old,new],…]}` replacements inside
  `Level.cpp`, rebuilds `link_so`, scores, restores the pristine source.
* `batch12.json` … `batch17.json` — this pass's variant sets.
* `Level.cpp.base` — pristine `src/game/world/Level.cpp`;
  `build.ninja.base` — pristine `cmake-build-match/build.ninja`.
* `orig.asm`, `ours.asm`, `show_now.txt`, `diff_now.txt` — raw / side-by-side
  disassemblies of the two `.so` files.

## 2. The sink flag: upper bound and why it is not landable

`-mllvm -sink-insts-to-avoid-spills` is a hidden `cl::opt` in
`lib/CodeGen/MachineLICM.cpp` (default `false`, not enabled by any pass
config in 7.0.2). It runs `SinkIntoLoop()` after the pre-RA hoist and moves
preheader instructions back into the loop when *all* their uses are `COPY`s:

```cpp
// MachineLICMBase::SinkIntoLoop()
for (I : Preheader)
  if (IsLoopInvariantInst(*I) && !HasLoopPHIUse(&*I))
    Candidates.push_back(&*I);
for (I : Candidates)
  if (all uses of I->getOperand(0) are COPYs)
    B->splice(B->getFirstNonPHI(), Preheader, I);
```

On this base it moves `%530 = t2MOVi 192` into the `new AEGeometry` block,
`%545/%542/%541 = t2MOVi -1 / t2MOVi 304 / t2ADDri %stack.1,0` into the
`setRotation` block, and the ship-loop `t2MOVi` constants `100/4/3/300` into
their use blocks. The parts loop then becomes **byte-identical** to the
original (register signature `row/canvas/zero/k/hp/j/cc/cm =
r9/r8/sl/r6/r4/sl/fp/r9`); the remaining +4 B is the ship-loop `rnd` spill
pair.

| build | pct | size | insns | first real diff |
|---|---|---|---|---|
| shipped base | **88.3** | **2252** | 783/791 | 460 (parts preheader) |
| base + sink flag (Level.cpp only) | **89.3** | 2256 | 783/790 | ship loop (28 is only the branch-target shift) |
| base + sink flag + `rnd` mask 18 | **93.3** | 2252 | — | 548 |

**New this pass:** applying the flag to the whole `Level.cpp` TU regresses the
gate badly — `byte_exact 4500 → 2357`, `linked_exact 4500 → 4486`
(`RATCHET FAIL`). The flag perturbs many other functions in the TU, so it is
not a viable per-file override even though `StarMap.cpp` already carries one
(`-mllvm -likely-branch-weight=7 -mllvm -unlikely-branch-weight=1`).

### Why a source-level sink is hard

`IsProfitableToHoist` hoists a loop-invariant instruction unconditionally when
`TII->isTriviallyReMaterializable(MI, AA)` is true, and `t2MOVi` /
`t2ADDri FI,0` are trivially rematerializable (no virtual-register uses). The
only early-out is

```cpp
bool CheapInstr  = IsCheapInstruction(MI);
bool CreatesCopy = HasLoopPHIUse(&MI);
if (CheapInstr && CreatesCopy)
    return false;          // keeps it in the loop body
```

`HasLoopPHIUse` follows `COPY`s **inside the loop** and returns true only if
the chain reaches a PHI. For `%541` the sole use is `$r1 = COPY %541` (a
physical-register copy), so the walk stops immediately and the instruction is
hoisted. Getting a genuine PHI use needs a second address value (two
`Vector`s), which costs a second `Vector` and a latch recompute — measured
78.1 % / 2272 B (waves 105–107).

## 3. Measured this pass (all ≤ 88.3 % at 2252 B unless noted)

Format: `variant pct / ours-size / firstdiff`.

### 3.1 `Vector` construction / scope / type

| variant | pct | size | firstdiff |
|---|---|---|---|
| shipped `setRotation(Vector{0.0f, 3.1415927f, 0.0f})` | **88.3** | 2252 | 460 |
| `Vector(0.0f, pi, 0.0f)` (paren ctor) / `(Vector){…}` (compound literal) | 88.3 | 2252 | 460 |
| `const float pi = …; Vector{0, pi, 0}` | 88.3 | 2252 | 460 |
| `const float zf = 0.0f; Vector{zf, pi, zf}` | 88.3 | 2252 | 460 |
| `Vector rot = Vector(…); g->setRotation(rot)` (copy-init) | 88.3 | 2252 | 460 |
| `Vector rot; rot.x/y/z = …` / `rot.y` first | 88.3 / 88.2 | 2252 | 460 |
| `float *p = &rot.x; p[0..2] = …` | 88.3 | 2252 | 460 |
| static helper `RotHelper::make()` returning `Vector` | 88.3 | 2252 | 460 |
| immediately-invoked lambda returning `Vector` | 88.3 | 2252 | 460 |
| `rot[0..2] = …` (subscript operator) | 84.5 | 2292 | 28 |
| `Vector rot` as first statement of the loop body | 84.0 | 2264 | 28 |
| `Vector rot` before `new AEGeometry` | 85.4 | 2256 | 28 |
| `Vector rot; rot = Vector(…)` (assign from temp) | 76.9 | 2276 | 4 |
| `static const Vector rot{…}` (rodata) | 72.6 | 2228 | 3 |
| two `Vector`s + `plainHangar ? ra : rb` (loop-invariant select) | 78.1 | 2272 | 4 |

### 3.2 Parts-loop declarations / control flow

| variant | pct | size | firstdiff |
|---|---|---|---|
| shipped | **88.3** | 2252 | 460 |
| `PlayerStatic *hangarPart;` declared before `g` | 88.3 | 2252 | 460 |
| `AEGeometry *g; g = new …` | 88.3 | 2252 | 460 |
| `int mesh;` declared outside the loop | 88.3 | 2252 | 460 |
| `const int *row = kHangarMeshes[sceneRace]; mesh = row[k]` | 88.3 | 2252 | 460 |
| `const int *rowp` declared before the loop | 88.3 | 2252 | 460 |
| `KIPlayer *hangarPart = new PlayerStatic(…)` | 88.3 | 2252 | 460 |
| `unsigned mesh` / `unsigned k` / `int plainHangar` / `unsigned capacity` | 88.3 | 2252 | 460 |
| `bool plainHangar` moved inside the loop / after the mesh check | 88.3 | 2252 | 460 |
| `if (plainHangar) {} else { child loop }` | 88.3 | 2252 | 460 |
| `if (mesh >= 0) { … }` block form | 88.3 | 2252 | 460 |
| `k++` / `k <= 3` / `k < 4u` / `k < 4L` | 88.3 | 2252 | 460 |
| `AEGeometry *child;` declared outside the child loop | 88.3 | 2252 | 460 |
| `int childCount = kBarChildCounts[sceneRace]` | 88.3 | 2252 | 460 |
| `int childMesh = kBarChildMeshes[sceneRace] + j` | 88.3 | 2252 | 460 |
| `(short)(kBarChildMeshes[sceneRace] + j)` | 88.3 | 2252 | 460 |
| `int k;` declared outside the loop | 88.1 | 2252 | 460 |
| pointer loop `for (const int *kp = …; kp != … + 4; ++kp)` | 86.3 | 2256 | 28 |
| `short mesh` | 78.6 | 2260 | 8 |
| `short j` | 84.7 | 2256 | 28 |
| `unsigned t = child->transform; delete child; g->addChild(t)` | 86.1 | 2256 | 457 |

### 3.3 Canvas / ship-loop prelude

| variant | pct | size | firstdiff |
|---|---|---|---|
| shipped | **88.3** | 2252 | 460 |
| `AbyssEngine::AERandom &rnd = *Globals::rnd` (reference form) | 88.3 | 2252 | 460 |
| `rnd` declared after `spots` | 88.3 | 2252 | 460 |
| `PaintCanvas *canvas = Globals::Canvas;` hoisted before the loop | 82.8 | 2252 | 28 |
| `PaintCanvas *canvas = Globals::Canvas;` inside the loop | 82.2 | 2248 | 28 |
| `const int *spots` declared before `spotUsed` | 84.8 | 2252 | 28 |
| `rnd` declared inside the ship loop | 86.8 | 2252 | 133 |
| `rnd` declared before the parts loop | 71.0 | 2232 | 6 |

### 3.4 Frame-padding probe (re-run)

`volatile char pad[300]; pad[0] = 0;` immediately after `setRotation`:
85.7 % / 2256 B / firstdiff 28; the `Vector` temp stays at `sp+56`, so the
parts-loop signature is unchanged.

## 4. Proven vs hypothesis

**Proven (measured on the shipped binary / this exact base):**

1. The parts loop is byte-identical to the original under
   `-mllvm -sink-insts-to-avoid-spills` (89.3 %, parts signature
   `r9/r8/sl/r6/r4/sl/fp/r9`), and the first real diff moves to the ship loop.
2. The sink flag on the whole `Level.cpp` TU is not landable: gate
   `byte_exact 4500 → 2357`, `linked_exact 4500 → 4486`.
3. The source-level sink trigger (`CheapInstr && HasLoopPHIUse`) is
   unreachable for a single stack `Vector` temp: a PHI needs two distinct
   incoming addresses, which costs a second `Vector` and a latch recompute
   (78.1 % / 2272 B).
4. `t2ADDri FI,0` and `t2MOVi` are trivially rematerializable in clang 7.0.2
   and are therefore hoisted unconditionally; the frame offset (`sp+56`) is a
   modified immediate, so the non-rematerializable `t2ADDri12` form is never
   selected.
5. The shipped source shape (local `rnd` at 3 of 7 sites) is the unique
   maximum of the full 2^7 site mask at 2252 B (wave 106).
6. The all-`Globals::rnd` form is the faithful REA source but scores 75.0 % /
   2228 B because it rotates the prologue and the `missionPtr==4` branch.

**Hypothesis (unproven):**

* The original binary's `createScene` was produced by a build in which the
  MachineLICM sink ran for this function (or an equivalent live-interval
  shrink), while the rest of the TU is unaffected. A source-only shape that
  achieves this with the 72-byte frame and 2252-byte size was not found in
  ~2400 variants across waves 88–107.

## 5. Structural state (unchanged by this pass)

* ELF sections: original **27**, ours **28**. The extra section is
  `.missioncase`, emitted by
  `#pragma clang section rodata=".missioncase"` in
  `src/game/mission/Status.cpp:1323`. No change introduced.
* `.ARM.extab`: original `0x26298`, ours `0x26294` (4 B short). Per-function
  records: `LevelScript` +4, `Hud` +4, `createShip` −8, `Generator` −4.
  No change introduced.
* `.text` is 2252 B for `createScene` in both (size-exact).

## 6. Bottom line

`Level::createScene` remains **88.3 % / 2252 B** (783/791 insns, first diff
460). The sink-flag upper bound on this base is **89.3 % / 2256 B**, but the
flag is not landable at TU scope (gate byte 4500 → 2357). No source change
landed; the worktree is clean and the gate is green
(byte 4500 / linked 4500 / lint CLEAN / drift 0 / RATCHET PASS).
