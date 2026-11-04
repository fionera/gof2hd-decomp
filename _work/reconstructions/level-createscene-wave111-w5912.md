# `Level::createScene` — wave-111 w5912: structural-source sweep confirms
# 88.3 % / 2252 B is the flag-free ceiling; sink+mask 35/37 remains 93.3 %

**Status:** research note, 2026-11-04. Worktree `wt/w5912`, branch
`wave111-w5912`, base `8eea060cc`. **No source change landed** — the shipped
source is still the best flag-free shape found (88.3 % / 2252 B). This commit
contains only this note and leaves `src/` untouched.

Follow-up to `level-createscene-wave110-w5812.md` (and the chain back through
waves 88–110). The wave-110 note established that the exact 93.3 % target is
`-mllvm -sink-insts-to-avoid-spills` **plus `rnd` mask 35 or 37**, and that
`MachineSinking` cannot reproduce `SinkIntoLoop` from source because the
parts-loop preheader has a single successor. This pass tested a further
~55 structurally different source shapes (helper/inlining boundary, branch
polarity, object construction order, data-array shape, scope/declaration
order) rather than tuning the existing `rnd` mask.

## TL;DR

* Target `_ZN5Level11createSceneEv`: **88.3 %**, orig/ours both **2252 B**,
  783/791 instructions, `byte=False`, `linked=False`, first diff at
  instruction 460 (`0xb4830`, the parts-loop preheader
  `add.w r9/r8, r0, r2, lsl #4`). Unchanged from waves 88–110.
* Gate re-measured before and after this pass, green:
  **byte_exact 4500/4524, linked_exact 4500/4524, stub_zero_size 0,
  missing 0, wrong_type 0, extra 0, data_exact 843/843, imports 207,
  lint CLEAN ✓, drift 0, RATCHET PASS.**
* No flag-free source shape among the ~55 new variants beats **88.3 % /
  2252 B**. The best measured variant remains the shipped source (mask 41:
  sites 0/3/5 use the local `rnd`).
* The best measured variant overall is still `sink + mask 35` (or 37):
  **93.3 % / 2252 B / 783/791**, with the entire parts loop byte-identical.
  Branch polarity (`if (mesh >= 0) { ... }`) does not change the sink result
  (still 93.3 % with mask 35) and does not change the flag-free result
  (88.3 %).
* The structural state is unchanged: 27 section headers exact, exact file
  size, `.ARM.extab` `0x26294` (4 B short), every original `.rodata` byte
  before the `Status::missionCompleted` rank table exact. No source file was
  touched, so no section/extab delta was introduced.

## 1. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5912
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
```

Scratch (`/dev/shm/w5912`, not committed): `cc2.sh`, `link2.sh`,
`score2.py`, `mkvar.py`, `try.sh` (same harness as wave 110, paths
rewritten to `wt/w5912`). Every variant below was compiled with the exact
NDK r18b clang 7.0.2 `-Oz` flags, linked with the exact gold link +
`strip --strip-all` + `tools/reorder_shstrtab.py`, and scored with the
official single-function verifier.

## 2. Best measured variants

| variant | pct | size | insns | note |
|---|---|---|---|---|
| shipped base (mask 41, no flag) | **88.3** | 2252 | 783/791 | first diff 460 |
| base + sink flag | 89.3 | 2256 | 783/790 | parts loop exact |
| **sink + mask 35** | **93.3** | **2252** | 783/791 | parts loop + ship loop; best overall |
| sink + mask 37 | **93.3** | 2252 | 783/791 | same score |
| sink + mask 35 + branch polarity | 93.3 | 2252 | 783/791 | polarity is neutral |
| branch polarity (no flag) | 88.3 | 2252 | 783/791 | `if (mesh >= 0) { ... }` |
| polarity + sink | 89.3 | 2256 | 783/790 | |

The exact 93.3 % target is fully characterised in the wave-110 note:
`sink-insts-to-avoid-spills` plus `rnd` mask 35/37, i.e.
`pick = nextInt(3)` must use `Globals::rnd` and one of
`race = nextInt(4)` / `nextInt(100) < 30` must use the local `rnd`.
No flag-free source shape reproduces `SinkIntoLoop`.

## 3. New structural variants measured this pass

All scores are the official fuzzy %; base = 88.3 / 2252. Unless noted, the
size is 2252 B and the instruction count is 783/791.

### 3.1 Helper / inlining boundary

| variant | pct | size | note |
|---|---|---|---|
| static helper `addHangarPart(Level*, mesh, race, plain)` + `level->getEnemies()` | 88.3 | 2252 | inlined, identical code |
| static helper `addHangarPart(Array<KIPlayer*>*, ...)` | 81.8 | 2260 | parameter passing changes RA |
| lambda `auto addHangarPart = [&](int mesh) { ... }` | 86.1 | 2252 | |
| lambda + sink | 87.4 | 2252 | |
| member helper `this->createHangarPart(...)` (equivalent to `Level*` form) | 88.3 | 2252 | inlined |

### 3.2 Branch polarity / CFG

| variant | pct | size | note |
|---|---|---|---|
| `if (mesh >= 0) { body }` | 88.3 | 2252 | polarity neutral |
| `if (mesh != -1) { body }` | 88.2 | 2252 | |
| `if (mesh == -1) continue;` | 88.2 | 2252 | |
| `if (mesh < 0) { } else { body }` | 88.3 | 2252 | |
| `if (plainHangar) { } else { child loop }` | 88.3 | 2252 | |
| `while (k < 4)` parts loop | 88.1 | 2252 | |
| `do { ... } while (++k < 4)` parts loop | 85.1 | 2256 | (wave-110) |
| child loop `while (j < count)` | 88.3 | 2252 | |
| child loop `for (j = 0; j != count; j++)` | 88.3 | 2252 | sink: 89.0 |
| child loop `do { ... } while (++j < count)` | 69.1 | 2256 | |
| `plainHangar` computed inside the loop | 88.3 | 2252 | hoisted |
| `plainHangar` as `int` | 88.3 | 2252 | |
| `plainHangar` as `if` statement | 88.3 | 2252 | |
| `int sceneRace = 0;` initialiser | 88.3 | 2252 | |
| `unsigned sceneRace` | 88.1 | 2252 | |
| `short sceneRace` | 79.2 | 2256 | |

### 3.3 Object construction order / scope

| variant | pct | size | note |
|---|---|---|---|
| `PlayerStatic` constructed before `setRotation` | 85.2 | 2256 | |
| `PlayerStatic` before `setRotation` + sink | 85.1 | 2264 | |
| `Vector rot` constructed before the loop | 83.9 | 2260 | |
| `Vector rot` constructed before the `mesh < 0` check | 84.0 | 2264 | |
| `Vector rot` before `new AEGeometry` | 85.4 | 2252 | (wave-110) |
| `Vector rot` named, `PlayerStatic(-1,g,rot.x,rot.x,rot.x)` | 85.1 | 2260 | |
| extra braces around the loop body | 88.3 | 2252 | |
| `g`/`hangarPart` declared outside the loop | 88.3 | 2252 | |
| `hangarPart` declared before `g` | 88.3 | 2252 | |
| `j` declared before the `Vector` | 88.3 | 2252 | |
| `int zero = 0;` used for Vector/PlayerStatic/child counter | 88.3 | 2252 | |
| `float zero = 0.0f;` used for Vector/PlayerStatic | 88.3 | 2252 | |
| `new PlayerStatic(-1, g, 0, 0, 0)` (integer zeros) | 88.3 | 2252 | |
| `KIPlayer *hangarPart = new PlayerStatic(...)` | 88.3 | 2252 | |
| `auto *g` / `auto *hangarPart` | 88.3 | 2252 | |
| `ArrayAdd` before the child loop | 64.6 | 2240 | |
| local `enemies` pointer instead of `this->enemies` | 85.7 | 2256 | |
| child mesh base hoisted before the child loop | 80.0 | 2252 | |

### 3.4 Data shape / indexing

| variant | pct | size | note |
|---|---|---|---|
| arrays moved to file scope (same names) | 88.3 | 2252 | scope is neutral |
| `kHangarMeshes` flattened to `int[40]`, index `race*4+k` | 84.4 | 2256 | 2D shape matters |
| row pointer inside the loop (`const int *row = ...`) | 88.3 | 2252 | hoisted |
| `&kHangarMeshes[race][0]` row pointer | 88.3 | 2252 | |
| `*(kHangarMeshes[race] + k)` | 88.3 | 2252 | |
| direct `kHangarMeshes[race][k]` in check and ctor | 88.3 | 2252 | CSE'd |
| `kHangarMeshes[(unsigned)race][k]` | 88.3 | 2252 | |
| `savedRace = sceneRace` used for indexing/capacity | 88.3 | 2252 | coalesced |
| pointer-walk parts loop (`meshIt++`) | 84.6 | 2260 | |
| range-based `for (int mesh : kHangarMeshes[race])` | 86.3 | 2256 | |

### 3.5 `rnd` declaration / scope

| variant | pct | size | note |
|---|---|---|---|
| shipped `AERandom *rnd = Globals::rnd` before the ship loop | 88.3 | 2252 | mask 41 |
| `AERandom *const rnd` | 88.3 | 2252 | |
| `auto *rnd` | 88.3 | 2252 | |
| `AERandom &rnd = *Globals::rnd` + `rnd.nextInt` | 88.3 | 2252 | rematerialised |
| `AERandom *&rnd = Globals::rnd` | 75.0 | 2228 | |
| `AERandom **rnd = &Globals::rnd` + `(*rnd)->nextInt` | 75.0 | 2228 | |
| `rnd` declared inside the ship loop | 86.8 | 2252 | |
| `rnd` declared before the parts loop | 74.5 | 2244 | |
| `rnd` at function scope, used in the `missionPtr == 4` branch | 68.3 | 2212 | |
| `rnd` local in the `missionPtr == 4` branch | 69.1 | 2216 | |
| `AERandom *volatile rnd` | 76.8 | 2232 | |

### 3.6 Full 128-mask `rnd` sweep (no flag)

The wave-110 sweep was re-confirmed on this base: no-flag maximum is
**88.3 %** at mask 41 (shipped). Next best: m097 88.2, m049 88.1,
m056 87.5, m057 87.4. With `-mllvm -sink-insts-to-avoid-spills`, the
maximum is **93.3 % / 2252 B / 783/791** at masks **35** and **37**.

## 4. Why the flag-free ceiling is 88.3 %

The first divergence is the parts-loop preheader register assignment:

```
orig:  add.w r9, r0, r2, lsl #4     ; r9 = kHangarMeshes + sceneRace*16
ours:  add.w r8, r0, r2, lsl #4     ; r8 = kHangarMeshes + sceneRace*16
```

The MIR after `Virtual Register Rewriter` (base, no flag) shows the
allocator assigned:

* `$r8` = `kHangarMeshes` base (defined in the preheader `bb.8`, 3520B)
* `$r9` = `&Globals::Canvas` (defined in the loop body `bb.18`, 4368B)
* `$r6` = `k`
* `$r4` = `mesh`, then the zero constant
* `$r5` = `g`
* `$r10` = `&Globals::status` (spilled to `%stack.4` in the preheader)

The original has the opposite assignment (`$r8 = &Globals::Canvas`,
`$r9 = base`). The sink flag changes the MIR order so the base interval
starts later, and the allocator gives `$r8` to `&Globals::Canvas` and `$r9`
to the base. The scheduler then places the base computation back in the
preheader, so the final assembly order is unchanged. This is the same
`SinkIntoLoop` mechanism described in the wave-110 note.

The spill choice follows from the same assignment: in our base the zero
constant takes `$r4`, so `hangarPart` takes `$r6` and `k` is spilled to
`[sp,#44]`; in the original the zero takes `$r10` (sl), `hangarPart` takes
`$r4`, `k` stays in `$r6`, and `hangarPart` is spilled to `[sp,#44]`.
Every attempt to force the zero into `sl` from source (named zero variable,
shared int/float zero, child-counter reuse, `PlayerStatic`-first, etc.)
either produced identical code or made the score worse.

## 5. Proven vs hypothesis

**Proven (measured on this exact base):**

1. No flag-free source shape among the ~55 new variants and the ~2580 from
   waves 88–110 beats **88.3 % / 2252 B**.
2. The exact 93.3 % / 2252 B target is `sink-insts-to-avoid-spills` plus
   `rnd` mask 35 or 37. Branch polarity is neutral.
3. Moving the arrays to file scope, using a helper that is inlined, using
   `auto`/`const`, reordering declarations, or changing the loop form to
   `while`/`!=`/`else` does not change the flag-free codegen.
4. Changing the `kHangarMeshes` shape to a flat `int[40]` changes the
   codegen and drops the score to 84.4 %.
5. The parts-loop preheader register assignment is the first and dominant
   divergence; the ship-loop spill and capacity register follow from it.

**Hypothesis (unproven):**

* The original `createScene` was compiled by a build in which
  `SinkInstsToAvoidSpills` was enabled for this function (LLVM 8 made it the
  default). The rest of the `Level.cpp` TU is tuned to the non-sink build
  (whole-TU flag: `byte_exact 4500 → 2357`), so the flag cannot be landed
  per-TU either.

## 6. Structural state (unchanged by this pass)

* `createScene` is 2252 B in both, 783/791 instructions.
* No source file touched, so no ELF section or `.ARM.extab` delta was
  introduced: 27 section headers exact, exact file size, `.ARM.extab`
  `0x26294` (4 B short), `.rodata` / `.dynsym` / `.dynstr` / `.hash` /
  `.dynamic` / `.rel.dyn` size-exact, and every original `.rodata` byte
  before the `Status::missionCompleted` rank table exact.
* The only remaining `createScene` diffs are the parts-loop / ship-loop
  register allocation and 8 literal-pool words whose global addresses are
  shifted by +2/+4/+6 bytes by the `.data`/`.bss` layout of other TUs.

## 7. Bottom line

`Level::createScene` remains **88.3 % / 2252 B** (783/791, first diff 460).
The path to 93.3 % is fully characterised (sink flag + `rnd` mask 35/37) but
no flag-free source shape that reproduces `SinkIntoLoop` has been found in
~2635 variants over waves 88–111. Worktree clean, gate green
(byte 4500 / linked 4500 / lint CLEAN / drift 0 / RATCHET PASS).
