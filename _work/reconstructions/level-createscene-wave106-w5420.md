# `Level::createScene` — wave-106 w5420: sink-flag upper bound re-confirmed
# (89.3 %), ~180 further source shapes measured, no source-only gain

**Status:** research note, 2026-11-04. Worktree `wt/w5420`, branch
`wave106-w5420`, base `486f8a119`. **No source change landed** — no source
shape found that raises either `byte_exact` or the fuzzy score while keeping
the 2252-byte size. This commit contains only this note.

Follow-up to `level-createscene-wave105-w5308.md` (which followed
`…-wave103-wf312.md`, `…-wave102-wf212.md`, `…-wave101-wf111.md`,
`…-wave100-w5012.md`, `…-wave99-w4911.md`, `…-wave98-w4814.md`,
`…-wave97-w4712.md`, `…-wave95-w4514.md`, `…-wave94-w4414.md`,
`…-wave92-w4249.md`, `…-wave92-w4213.md`, `…-wave91-w4113.md`,
`level-createscene-phi-use-w4013.md`, `level-createscene-licm-hoist-w3833.md`,
`Level-createScene-flag-free.md`, `level-createscene-88-3-2252-b_w3813.md`).

## TL;DR

* Target `_ZN5Level11createSceneEv`: **88.3 %**, orig/ours both **2252 B**,
  783/791 instructions, `byte=False`, `linked=False`, first diff at instruction
  460 (`0xb4830`, the parts-loop preheader `add.w r9/r8, r0, r2, lsl #4`).
  Unchanged from waves 88–105.
* Gate re-measured before and after this pass, green:
  **byte_exact 4500/4524, linked_exact 4500/4524, size-exact 4524/4524,
  stub_zero_size 0, missing 0, wrong_type 0, extra 0, data_exact 843/843,
  imports 207, lint CLEAN ✓, drift 0, RATCHET PASS.**
* **Re-confirmed the `-mllvm -sink-insts-to-avoid-spills` upper bound on this
  exact base: 89.3 % / 2256 B / 783 vs 790 insns, first real diff at 547.**
  The parts loop becomes *byte-identical* (register signature
  `row/canvas/zero/k/hp/j/cc/cm = r9/r8/sl/r6/r4/sl/fp/r9`, exactly the
  original). The +4 B is the ship-loop `rnd` spill pair, not the parts loop.
* ~180 further source shapes measured this pass (full 2^7 `rnd` site mask,
  `rnd` declaration-position permutations, ship-loop prelude permutations,
  `Vector` construction/scope/type variants, PHI-use attempts, type changes
  across the whole 23/4 branches, frame-padding probes). **None beats
  88.3 % at 2252 B.** The function is extremely robust: most semantic
  rewrites are byte-identical to the shipped base.
* Root cause unchanged: the parts-loop residue is the MachineLICM hoist of
  `%541 = t2ADDri %stack.1, 0` (the `Vector{0, π, 0}` temp address), which is
  `isReMaterializable` and therefore *unconditionally* hoisted; the ship loop
  is the `Globals::rnd` value cache. §2.

## 1. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5420
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

Scratch is `/dev/shm/w5420` only (not committed):

* `fscore.py` — official single-function score (`pct/osize/usize/byte/firstdiff/insns`);
* `sig.py` — extracts the 8 parts-loop registers
  (`row/canvas/zero/k/hp/j/cc/cm`); original =
  `r9/r8/sl/r6/r4/sl/fp/r9`, shipped base = `r8/r9/r4/r6/r6/sl/r8/fp`;
* `batch.py <json>` / `batch_sig.py <json>` — apply `{name:[[old,new],…]}`
  replacements inside `createScene`, rebuild `link_so`, score (and print the
  parts-loop signature), restore the source;
* `apply.py <json> <name>` — leave one variant applied (used with a manual
  `link_so` + objdump);
* `Level.cpp.base` — pristine `src/game/world/Level.cpp`;
* `base_body.txt` — the `createScene` body sliced out of it;
* `sw1.json` … `sw16.json`, `sw8.log`, `gen9.py` … `gen16.py` — this pass's
  variant sets and results;
* `show_base_new.txt`, `orig.asm`, `ours_base_so.asm`, `sink.asm`,
  `allg_new.asm` — side-by-side / raw disassemblies.

The 55 s `-mllvm -print-machineinstrs` dump of the full TU used for the MIR
evidence is `/dev/shm/w5420/mi_full.txt` (createScene "After Early Machine
Loop Invariant Code Motion" at line 1122676).

## 2. The two residues (re-confirmed)

### 2.1 Parts loop — the hoisted `Vector` temp address

Pre-RA MIR (base, before *and* after EarlyMachineLICM) has the address in the
parts-loop **preheader** `bb.8`:

```
%530:rgpr   = t2MOVi 192                     ; sizeof(AEGeometry)
%541:gprnopc= t2ADDri %stack.1, 0            ; &Vector{0, π, 0} temp
%542:rgpr   = t2MOVi 304                     ; sizeof(PlayerStatic)
%545:rgpr   = t2MOVi -1
```

`MachineLICMBase::IsProfitableToHoist` returns `true` unconditionally for
`TII->isTriviallyReMaterializable(MI, AA)`, and `t2ADDri FI,0` / `t2MOVi` are
both trivially rematerializable (`TargetInstrInfo::isReallyTriviallyReMaterializableGeneric`
sees no virtual-register uses). So they are always hoisted; the register
allocator then rematerializes them at their uses, but the *transient* long
live interval is what flips the greedy allocation:
`%541` is hinted to `$r1`, misses it (taken by `%535` `uxth`), and takes `r4`,
evicting `hangarPart` and spilling `k`.

The only escape in `IsProfitableToHoist` is the early-out

```cpp
bool CheapInstr  = IsCheapInstruction(MI);   // true for t2ADDri / t2MOVi
bool CreatesCopy = HasLoopPHIUse(&MI);
if (CheapInstr && CreatesCopy)
    return false;                            // keeps it in the loop body
```

`HasLoopPHIUse` follows copies *inside the loop*; `%541`'s only use is
`$r1 = COPY %541` (a physical-register copy), so the walk stops immediately.
A genuine PHI use needs two *different* incoming address values, which needs a
second `Vector` object — measured at 78.1 % / 2272 B (§3, `twovec_phi`), and
the address is then also recomputed in the latch.

### 2.2 Ship loop — the `Globals::rnd` value cache

REA on the shipped `.so` (wave-105 note §1) prints
`AbyssEngine::AERandom::nextInt(Globals::rnd, …)` at **all seven** sites, so
the original source is the all-`Globals::rnd` form. Disassembly confirms the
original keeps `&Globals::rnd` in `r8` and reloads the value (`ldr r0,[r8]`)
at every site. Our tuned source (`AERandom *rnd = Globals::rnd;` used at 3 of
7 sites) instead caches the *value* in `r5` and spills `&Globals::rnd` to
`sp#44`.

The pure all-globals form is **75.0 % / 2228 B** (§3): it fixes the ship loop
but rotates the prologue (`r5` becomes the stack-guard address instead of the
first ship's `AEGeometry`, so the guard address is no longer spilled to
`sp#36`) and perturbs the `missionPtr==4` branch. The 2^7 site mask has no
better cell than the shipped 3-local/4-global mix (best non-base mask 84.1 %,
§3).

## 3. Measured this pass (all ≤ 88.3 % at 2252 B unless noted)

Format: `variant pct / ours-size / firstdiff`.

### 3.1 `rnd` site mask and declaration position

Full 2^7 mask over the seven `nextInt` sites (112 non-base masks, all keeping
the `AERandom *rnd` declaration so the size stays 2252):

| mask class | pct | size | firstdiff |
|---|---|---|---|
| shipped base (mask 41: top/pick/retry local) | **88.3** | 2252 | 460 |
| best non-base (m119, m117, m115, m113, m103, m101, m099, m097, m055, m053, m051, m049, m039, m037, m035) | 84.1 | 2252 | 28 |
| all-local (m127) | 83.7 | 2236 | 9 |
| all-global (m0, decl removed) | 75.0 | 2228 | 6 |

`rnd` declaration moved around the ship-loop prelude:

| position | pct | size | firstdiff |
|---|---|---|---|
| after `spots` (shipped) / after `spotUsed` | **88.3** | 2252 | 460 |
| before `spotUsed` / after the `if (ownHangar)` block | 76.0 | 2228 | 6 |
| before `ownHangar` | 71.9 | 2232 | 6 |
| before `capacity` / before `shipCount` / after `ownHangar` / after `shipCount` | 63.6 | 2224 | 4 |

### 3.2 Ship-loop prelude permutations

| variant | pct | size | firstdiff |
|---|---|---|---|
| shipped order (`ownHangar`, `capacity`, `shipCount`, `if(ownHangar)`, `spotUsed`, `rnd`, `spots`) | **88.3** | 2252 | 460 |
| `spots` before `spotUsed` | 75.2 | 2232 | 6 |
| `spots` before `shipCount` | 62.8 | 2224 | 4 |
| `spotUsed` before `shipCount` | 65.2 | 2248 | 4 |
| `ownHangar` after `capacity` | 79.8 | 2276 | 28 |
| `ownHangar` after `shipCount` | 82.8 | 2244 | 28 |
| `capacity` before `ownHangar` (all-globals) | 77.2 | 2264 | 6 |
| `capacity` before `ownHangar` + `unsigned sceneRace` (all-globals) | 76.9 | 2264 | 6 |

### 3.3 `Vector` construction / scope / type

| variant | pct | size | firstdiff |
|---|---|---|---|
| shipped `setRotation(Vector{0.0f, 3.1415927f, 0.0f})` | **88.3** | 2252 | 460 |
| `Vector rot(…); setRotation(rot)` (paren, copy-init, brace-copy) | 88.3 | 2252 | 460 |
| `const Vector &rot = Vector(…); setRotation(rot)` | 88.3 | 2252 | 460 |
| `float rot[3]; setRotation(Vector(rot[0],rot[1],rot[2]))` | 88.3 | 2252 | 460 |
| `const float pi = 3.1415927f;` local / `static const float pi` | 88.3 | 2252 | 460 |
| `plainHangar ? V : V` (identical branches) | 88.3 | 2252 | 460 |
| `Vector rot` before the loop, constructed before the loop | 83.9 | 2260 | 28 |
| `Vector rot` before the loop, assigned in the loop | 76.6 | 2280 | 4 |
| `Vector rot; rot[1] = π;` (subscript) | 85.0 | 2264 | 28 |
| `static const Vector rot{…}` (rodata, PC-relative address) | 72.6 | 2228 | 3 |
| 3-float overload `setRotation(0.0f, π, 0.0f)` (wrong callee) | 73.2 | 2172 | 3 |
| `setRotation(*new Vector{…})` | 74.5 | 2184 | 3 |
| two `Vector`s + `(k & 1) ? &rotA : &rotB` (real address PHI) | 78.1 | 2272 | 4 |
| `Vector *lastRot` PHI, `(void)` after loop / `if (lastRot==nullptr) return` | 88.3 / 76.8 | 2252 / 2268 | 460 / 4 |

### 3.4 Parts-loop control flow / expressions

| variant | pct | size | firstdiff |
|---|---|---|---|
| shipped `if (mesh < 0) continue;` | **88.3** | 2252 | 460 |
| `if (mesh >= 0) { … }` block form | 88.3 | 2252 | 460 |
| `int four = 4; k < four` | 88.3 | 2252 | 460 |
| `const int *row0 = kHangarMeshes[sceneRace]; mesh = row0[k]` | 88.3 | 2252 | 460 |
| `mesh = *(kHangarMeshes[sceneRace] + k)` | 88.3 | 2252 | 460 |
| `(unsigned short) mesh` at the ctor call | 88.3 | 2252 | 460 |
| `new PlayerStatic(-1, g, 0, 0, 0)` / `new AEGeometry(mesh, Canvas, 0)` | 88.3 | 2252 | 460 |
| `KIPlayer *hangarPart` + `ArrayAdd(hangarPart, …)` / `ArrayAdd<KIPlayer *>(…)` | 88.3 | 2252 | 460 |
| `for (int k = 0; k != 4; k++)` | 87.9 | 2252 | 460 |
| `unsigned short mesh = (unsigned short) kHangarMeshes[…]` | 76.0 | 2244 | 8 |
| `int childMeshBase = kBarChildMeshes[sceneRace]` hoisted | 71.7 | 2244 | 6 |
| both child count + child mesh base hoisted | 73.7 | 2252 | 8 |
| `hangarPart->geometry->addChild(…)` | 64.8 | 2240 | 4 |
| `unsigned t = child->transform; delete child; g->addChild(t);` | 86.1 | 2256 | 457 |

`plainHangar` spellings: `int` / `sceneRace >= 4` / `3 < sceneRace` /
inline `!(sceneRace == 1 || sceneRace > 3)` all 88.3; `!(…)` inverted
(negated bool) and `plainHangar == false` 86.1 / firstdiff 28.

### 3.5 Types across the whole function (all 88.3 / 2252 / 460 unless noted)

`unsigned sceneRace` 88.1 / 452; `unsigned capacity`, `unsigned shipCount`,
`unsigned i`, `unsigned race`, `unsigned spot`, `unsigned tries`,
`unsigned type`, `uint8_t *spotUsed`, `int ownHangar`, `unsigned look`,
`unsigned seat`, `unsigned mesh` (agent branch), `unsigned staticCount`,
`unsigned` zero-init loop — all 88.3. `unsigned agentCount` and
`unsigned i` in the agent loop 88.2 / firstdiff 295.

### 3.6 Frame-padding probe

`volatile char pad[N]; pad[0] = 0;` before the parts loop (N = 100/200/300):
78.0 / 2260 / 4, parts-loop signature **unchanged** (`r8/r9/…`). The `Vector`
temp sits at the top of the 72-byte frame (`sp+56`), so objects allocated
below it do not move it; a >255 offset (which would select the
non-rematerializable `t2ADDri12` and stop the hoist) is unreachable without
changing the frame size and therefore the prologue/epilogue.

## 4. Proven vs hypothesis

**Proven (measured on the shipped binary / this exact base):**

1. The parts loop is byte-identical to the original under
   `-mllvm -sink-insts-to-avoid-spills` (89.3 %, parts signature
   `r9/r8/sl/r6/r4/sl/fp/r9`), and the first real diff moves to the ship loop
   (instruction 547).
2. The sink is the *only* lever found in ~2100 variants across waves 88–106
   that reproduces the original parts-loop allocation.
3. The source-level sink trigger (`CheapInstr && HasLoopPHIUse`) is
   unreachable for a single stack `Vector` temp: a PHI needs two distinct
   incoming addresses, which costs a second `Vector` and a latch recompute
   (78.1 %).
4. `t2ADDri FI,0` is trivially rematerializable in clang 7.0.2 and is
   therefore hoisted unconditionally; the frame offset (`sp+56`) is a
   modified immediate, so the non-rematerializable `t2ADDri12` form is never
   selected.
5. The shipped source shape (local `rnd` at 3 of 7 sites) is the unique
   maximum of the full 2^7 site mask at 2252 B.
6. The all-`Globals::rnd` form is the faithful REA source but scores 75.0 % /
   2228 B because it rotates the prologue and the `missionPtr==4` branch.

**Hypothesis (unproven):**

* The original binary's `createScene` was produced by a build in which the
  MachineLICM sink ran for this function (or an equivalent live-interval
  shrink), while the rest of the TU is unaffected. A source-only shape that
  achieves this with the 72-byte frame and 2252-byte size was not found.

## 5. Bottom line

`Level::createScene` remains **88.3 % / 2252 B** (783/791 insns, first diff
460). The sink-flag upper bound on this base is **89.3 % / 2256 B**. No source
change landed; the worktree is clean and the gate is green
(byte 4500 / linked 4500 / RATCHET PASS).
