# `Level::createScene` — wave-108 w5615: ~60 further source shapes, still 88.3 %;
# `off_ge0m1` reproduces the original parts-loop *register signature* but costs 8 B

**Status:** research note, 2026-11-04. Worktree `wt/w5615`, branch
`wave108-w5615`, base `8f98ec4d6`. **No source change landed** — no source
shape found that raises either `byte_exact` or the fuzzy score while keeping
the 2252-byte size. This commit contains only this note.

Follow-up to `level-createscene-wave107-w5514.md` (which followed
`…-wave106-w5420.md`, `…-wave105-w5308.md`, `…-wave103-wf312.md`,
`…-wave102-wf212.md`, `…-wave101-wf111.md`, `…-wave100-w5012.md`,
`…-wave99-w4911.md`, `…-wave98-w4814.md`, `…-wave97-w4712.md`,
`…-wave95-w4514.md`, `…-wave94-w4414.md`, `…-wave92-w4249.md`,
`…-wave92-w4213.md`, `…-wave91-w4113.md`, `level-createscene-phi-use-w4013.md`,
`level-createscene-licm-hoist-w3833.md`, `Level-createScene-flag-free.md`,
`level-createscene-88-3-2252-b_w3813.md`).

## TL;DR

* Target `_ZN5Level11createSceneEv`: **88.3 %**, orig/ours both **2252 B**,
  783/791 instructions, `byte=False`, `linked=False`, first diff at instruction
  460 (`0xb4830`, the parts-loop preheader `add.w r9/r8, r0, r2, lsl #4`).
  Unchanged from waves 88–107.
* Gate re-measured before and after this pass, green:
  **byte_exact 4500/4524, linked_exact 4500/4524, size-exact 4524/4524,
  stub_zero_size 0, missing 0, wrong_type 0, extra 0, data_exact 843/843,
  imports 207, lint CLEAN ✓, drift 0, RATCHET PASS.**
* ~60 further source shapes measured (declaration order, placement-new,
  `Vector` construction/scope/type permutations, `rnd` placement, loop-form
  permutations, `__builtin_expect`, negative-zero, PHI-address attempts).
  **None beats 88.3 % at 2252 B**; 32 are byte-identical to the shipped base.
* **New this pass:** `off_ge0m1`
  (`Vector *v = (Vector *) ((char *) &rot + ((mesh >= 0) - 1));`) makes the
  parts loop's **register signature exactly the original's**
  (`row/canvas/zero/k/hp/j/cc/cm = r9/r8/sl/r6/r4/sl/fp/r9`) and prevents the
  `%541` MachineLICM hoist (the `+offset` folds into `t2ORRrs` inside
  `bb.14`), but the ORR plus the resulting spills cost **+8 B** and the score
  drops to **86.2 % / 2260 B**. It is the cheapest loop-variant address found.
* Root cause unchanged: the parts-loop residue is the MachineLICM hoist of
  `%541 = t2ADDri %stack.1, 0` (the `Vector{0, π, 0}` temp address); the ship
  loop is the `Globals::rnd` value cache. §2.

## 1. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5615
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

Scratch is `/dev/shm/w5615` only (not committed):

* `cc.sh` — standalone clang++ 7.0.2 compile of only `Level.cpp` with the
  exact ninja flags (~1.5 s) → `Level.o`;
* `ccsink.sh` — same + `-mllvm -sink-insts-to-avoid-spills`;
* `sbs.py` — side-by-side normalizer (`body`, `norm`);
* `loopscore.py <obj>` — parts-loop-only difflib ratio (preheader
  `add.w rX, r0, r2, lsl #4` … latch `bcc.n`; original region = 72 insns);
* `lsbatch.py <json>` — fast loop-only batch (`size` + `loop%`);
* `fullbatch.py <json>` — official `link_so` + `tools/verify/verify.py` per
  variant (~25–35 s each);
* `Level.cpp.base` — pristine `src/game/world/Level.cpp`;
* `w1.json` … `w8.json`, `w1_full.txt` … `w7_full.txt` — this pass's variant
  sets and results;
* `orig.asm`, `base_sbs.txt`, `Level.ll` — raw / side-by-side disassemblies and
  the final `-O` IR.

## 2. The two residues (re-confirmed, unchanged)

### 2.1 Parts loop — the hoisted `Vector` temp address

Pre-RA MIR (base, after EarlyMachineLICM) has the address in the parts-loop
**preheader** `bb.8`:

```
%530:rgpr   = t2MOVi 192                     ; sizeof(AEGeometry)
%541:gprnopc= t2ADDri %stack.1, 0            ; &Vector{0, π, 0} temp
%542:rgpr   = t2MOVi 304                     ; sizeof(PlayerStatic)
%545:rgpr   = t2MOVi -1
```

`MachineLICMBase::IsProfitableToHoist` returns `true` unconditionally for
`TII->isTriviallyReMaterializable(MI, AA)`, and `t2ADDri FI,0` / `t2MOVi` are
both trivially rematerializable. The only early-out is

```cpp
bool CheapInstr  = IsCheapInstruction(MI);
bool CreatesCopy = HasLoopPHIUse(&MI);
if (CheapInstr && CreatesCopy)
    return false;                            // keeps it in the loop body
```

**New this pass — exact LLVM 7.0.2 semantics of `HasLoopPHIUse`** (source
inspected at `/dev/shm/w5620/llvm702src/lib/CodeGen/MachineLICM.cpp`): it
iterates the **def** operands of `MI`, then the uses of that def; a use that
is a `PHI` *inside the current loop* (or in an exit block) returns `true`, and
`COPY` uses inside the loop are pushed on the worklist. For
`%541 = t2ADDri %stack.1, 0` the def is `%541`; its only use is
`$r1 = COPY %541` (a physical-register copy), whose def is physical, so the
walk stops and the result is `false` → hoist. To make it `true` the address
must be a **loop-carried PHI incoming value**, i.e. the pointer must be
updated in the latch with a value the compiler cannot fold to the same SSA
value.

### 2.2 Ship loop — the `Globals::rnd` value cache

The `Globals::rnd` load is CSE'd to the pre-`capacity` position and then
spilled to `sp+44` across `operator new[]` + the `spotUsed` zero loop; the
original keeps it in `r8` (`ldr.w r8,[r0]` at `b492c`) and uses
`ldr.w r0,[r8]` at every `nextInt`. This is the +4 B that the sink-flag build
still carries (89.3 % / 2256 B).

## 3. Measured this pass (full official score; all `osize=2252`)

Format: `variant pct / ours-size`. Base = **88.3 / 2252**.

### 3.1 `Vector` construction / scope / type (all ≤ base)

| variant | pct | size |
|---|---|---|
| `vec_named_assign` (`Vector rot; rot.x=0; rot.y=π; rot.z=0;`) | **88.3** | 2252 |
| `vec_named_assign2` (z,y,x order) | **88.3** | 2252 |
| `vec_named_assign3` (y,x,z order) | 88.2 | 2252 |
| `vec_copy_list` (`Vector rot = {…};`) | **88.3** | 2252 |
| `vec_brace_named` (`Vector rot{…};`) | **88.3** | 2252 |
| `vec_const_temp` (`const Vector rot(…)`) | **88.3** | 2252 |
| `vec_paren_pi` (`(float) 3.1415927`) | **88.3** | 2252 |
| `vec_m_pi` (exact float literal) | **88.3** | 2252 |
| `vec_double_ctor` (`Vector(0.0, π, 0.0)`) | **88.3** | 2252 |
| `vec_int_zero` (`int zi = 0; Vector(zi, π, zi)`) | **88.3** | 2252 |
| `vec_zero_float_var` (`float zf = 0.0f; Vector(zf, π, zf)`) | **88.3** | 2252 |
| `vec_zero_ptr` (`const float *zp = &zero; Vector(zp[0], π, zp[0])`) | **88.3** | 2252 |
| `rot_after_new` (`Vector rot(…);` after `new AEGeometry`) | **88.3** | 2252 |
| `rot_after_mesh` (`Vector rot(…);` before `new AEGeometry`) | 85.4 | 2256 |
| `rot_out` (`Vector rot(…);` before the loop, `setRotation(rot)`) | 83.9 | 2260 |
| `rot_out_assign` (`Vector rot;` before the loop, field-assign inside) | 85.3 | 2264 |
| `vec_union` (`float f[3]; setRotation(*(Vector*)f)`) | 84.9 | 2260 |
| `vec_neg_zero` (`Vector{-0.0f, π, -0.0f}`) | 86.4 | 2260 |
| `place_geom` (`new (::operator new(sizeof(AEGeometry))) AEGeometry(…)`) | 86.7 | 2252 |
| `place_ps` (placement `new` for `PlayerStatic`) | 85.5 | 2248 |
| `place_both` | 83.2 | 2244 |

### 3.2 `off_ge0m1` family (loop-variant `Vector` address)

`off_ge0m1` = `Vector *v = (Vector *) ((char *) &rot + ((mesh >= 0) - 1));`.
The offset is 0 on the taken path (`mesh >= 0` after the `continue`), but the
compiler does not fold it: it emits `add r0, sp, #56; orr.w r1, r0, r4, lsr #31`
inside `bb.14`, so `%541` is **not** hoisted.

| variant | pct | size | parts-loop regs |
|---|---|---|---|
| base | **88.3** | **2252** | `r8/r9/r4/r6/r6/sl/r8/fp` |
| `off_ge0m1` | 86.2 | 2260 | **`r9/r8/sl/r6/r4/sl/fp/r9`** (= original) |
| `off_ge0m1` + `unsigned k` | — | 2258 | (loop 80.3 %) |
| `off_ge0m1` + `KIPlayer *hangarPart` | — | 2258 | (loop 80.3 %) |
| `off_ge0m1` + `int plainHangar` | — | 2258 | (loop 80.3 %) |
| `off_ge0m1` + `const int *row` | — | 2258 | (loop 80.3 %) |
| `off_ge0m1` + `unsigned j` | — | 2258 | (loop 77.4 %) |
| `off_ge0m1` + local `childCount` | — | 2258 | (loop 80.3 %) |
| `off_ge0m1` + `(unsigned) mesh >> 31` | — | 2258 | (loop 80.3 %) |
| `off_ge0m1` + `rot` declared outside the loop | — | 2262 | (loop 70.4 %) |
| `off_ge0m1` + `volatile Vector` | — | 2262 | (loop 67.6 %) |
| `off_ge0m1` + `int off = …` / `(unsigned) &rot + …` | — | 2258 | (loop 80.3 %) |
| `off_ship_sr31` (`shipIndex >> 31`) | 85.7 | 2264 | — |
| `off_cap_sr31` (`missionPtr >> 31`) | 77.3 | 2248 | — |

`off_ge0m1`'s parts loop matches the original's **register assignment**
exactly, but the extra ORR forces `k` (to `sp+44`) and `hangarPart` (to
`sp+28`) to spill and adds `mov r6, r9` / `mov r9, r6` around the child loop:
75 vs 72 insns, +8 B.

### 3.3 `volatile` / `rnd` / zero-register probes

| variant | pct | size |
|---|---|---|
| `volatile_vec_assign` (`volatile Vector rot; rot.x/y/z = …`) | 87.0 | 2260 |
| `rnd_ref_all` (`AERandom &rnd = *Globals::rnd` at all 7 sites) | 83.7 | 2236 |
| `rnd_before_cap` (`rnd` declared before `capacity`) | 63.6 | 2224 |
| `rnd_before_own` (`rnd` declared before `ownHangar`) | 71.6 | 2228 |
| `zero_top` (`const float zero` shared by `Vector` + `PlayerStatic`) | **88.3** | 2252 |
| `zero_top2` (shared by `Vector` only) | **88.3** | 2252 |
| `vec_zero_var_args_lit` (`const float zf` inside the loop) | **88.3** | 2252 |
| `volatile_off` (`volatile int off = 0; &rot + off`) | 79.8 | 2260 |
| `args_neg_zero` (`PlayerStatic(-1, g, -0.0f, …)`) | 84.8 | 2260 |

### 3.4 Loop form / types / branch hints (all ≤ base)

| variant | pct | size |
|---|---|---|
| `k_preinc` (`++k`), `k_rev` (`4 > k`), `j_preinc` | **88.3** | 2252 |
| `plain_or` (`(a) | (b)`), `plain_int` (`int plainHangar`) | **88.3** | 2252 |
| `hp_pre` / `hp_pre_kip` (`hangarPart` declared before the loop) | **88.3** | 2252 |
| `hp_copy` (`KIPlayer *hp = hangarPart`) | **88.3** | 2252 |
| `g_pre` (`AEGeometry *g;` before the loop) | **88.3** | 2252 |
| `mesh_var` (`int mesh;` before the loop) | **88.3** | 2252 |
| `child_pre` (`AEGeometry *child;` before the inner loop) | **88.3** | 2252 |
| `scene_local` (`const int scene = sceneRace`) | **88.3** | 2252 |
| `ps_int0` / `ps_double0` / `ps_mixed0` (float-arg types) | **88.3** | 2252 |
| `mesh_eq_m1` (`if (mesh == -1)`) | 88.2 | 2252 |
| `mesh_lt1` (`if (mesh < 1)`) | 88.2 | 2252 |
| `mesh_unsigned` (`unsigned mesh; mesh == 0xffffffffu`) | 88.2 | 2252 |
| `plain_false` (`if (plainHangar == false)`) | 86.1 | 2252 |
| `expect_mesh` (`__builtin_expect(mesh < 0, 0)`) | 79.4 | 2244 |
| `expect_plain` (`__builtin_expect(!plainHangar, 1)`) | 74.7 | 2268 |
| `add_before_child` (`ArrayAdd` before the child loop) | 64.6 | 2240 |
| `hp_after_child` (`new PlayerStatic` after the child loop) | 62.7 | 2236 |

### 3.5 PHI-address attempts (all lose the preheader)

`Vector rot(…); Vector *pv = &rot;` before the loop, `setRotation(*pv)`, and a
latch update `pv = (Vector *)((char *)&rot + f(mesh))`:

| update | size | loop score |
|---|---|---|
| `mesh >> 20` / `mesh >> 16` / `(unsigned) mesh >> 20` | 2278 | lost (`-1`) |
| `mesh & 0x100000` | 2282 | lost |
| `mesh / 1000000` | 2290 | lost |
| `sceneRace >> 20` | 2270 | lost |

`k`-based updates (`k >> 31`, `k >> 2`, `k == 4`) are folded away because the
compiler knows `k < 4`; `mesh`-based updates are real PHIs but add 2–3 insns
and destroy the `add.w rX, r0, r2, lsl #4` preheader shape.

## 4. Why the zero/hp overlap is the real RA trigger

The final MIR (`bb.19`, `ir-block.123`) shows the `Vector` zero `%677` and the
`PlayerStatic *` result `%11` **overlapping**: `%11 = COPY $r0` is emitted
right after `operator new(304)` and before the `strd %677, %677, [sp]` that
sets up the 4th/5th float arguments. Both therefore need a register at the
same time:

* original: zero → `sl`, `hangarPart` → `r4` (then spilled to `sp+44`);
* base ours: zero → `r4`, `hangarPart` → `r6`, which evicts `k` to `sp+44`.

`off_ge0m1` removes the `%541` hoist, which frees `r4` for `hangarPart`, and
the zero then lands in `sl` — exactly the original assignment. The remaining
cost is the ORR and the extra `k`/row spills, which is why the score drops.

## 5. Proven vs hypothesis

**Proven (measured on the shipped binary / this exact base):**

1. `off_ge0m1` makes the parts-loop register signature **exactly** the
   original's (`r9/r8/sl/r6/r4/sl/fp/r9`) by preventing the `%541` hoist
   (offset folds into `t2ORRrs` in `bb.14`); it costs +8 B and scores 86.2 %.
2. The cheapest loop-variant address is one extra `orr` (`(mesh >= 0) - 1`);
   `(mesh & 0)`, `(mesh - mesh)`, `(mesh < 0 ? -1 : 0)` all fold to the
   invariant address and are byte-identical to the base.
3. `HasLoopPHIUse` in LLVM 7.0.2 walks **def → uses → COPYs → PHIs**; a real
   PHI use requires a loop-carried pointer update. All measured real-PHI
   variants lose the preheader and are ≥ 2270 B.
4. The zero/`hangarPart` live-range overlap in `bb.19` is what forces the
   `k` spill; the original avoids it by putting zero in `sl`.
5. The ship-loop `Globals::rnd` spill is independent of the parts loop and is
   still present in the sink-flag build (+4 B, 89.3 %).

**Hypothesis (unproven):**

* The original binary's `createScene` was produced by a build in which the
  MachineLICM sink ran for this function (or an equivalent live-interval
  shrink), while the rest of the TU is unaffected. A source-only shape that
  achieves this with the 72-byte frame and the 2252-byte size was not found in
  the ~2460 variants across waves 88–108.

## 6. Structural state (unchanged by this pass)

* ELF section headers: **27** in both (readelf `e_shnum`); no extra section.
* `.ARM.extab` `00026298` size- and base-exact (`0x1a3b78`).
* `.text` `001337c8`, `.rodata` `00041b67`, `.dynsym`, `.dynstr`, `.hash`
  size- and base-exact; `.rodata`/`.dynsym`/`.dynstr`/`.hash` byte-identical.
* `.fini_array` is **0xc8 (200 B) in ours vs 8 B** in the original (the
  `Status::missionCompleted` rank table lives there), so `.dynamic` content
  (3 B) and `.rel.dyn` content (4 B) differ; sizes are unchanged. This is the
  pre-existing wave-107 caveat — not touched by this pass.
* `createScene` is 2252 B in both.

## 7. Bottom line

`Level::createScene` remains **88.3 % / 2252 B** (783/791 insns, first diff
460). `off_ge0m1` is the best new shape at reproducing the original RA
(86.2 % / 2260 B), but its extra ORR costs more than the allocation gain. No
source change landed; the worktree is clean and the gate is green
(byte 4500 / linked 4500 / lint CLEAN / drift 0 / RATCHET PASS).
