# `Level::createScene` — wave-109 w5713: the parts-loop residue is
# `SinkIntoLoop`'s "all uses are COPYs" rule; no flag-free source shape found

**Status:** research note, 2026-11-04. Worktree `wt/w5713`, branch
`wave109-w5713`, base `7c34c4098`. **No source change landed** — a full
128-mask re-sweep of the `Globals::rnd` sites without the sink flag confirms
the shipped source is the best of that family, and ~40 further source shapes
(Vector construction/scope/type, loop forms, `rnd` caching, statement
reordering, CFG rewrites) are byte-identical to the shipped base or worse.
This commit contains only this note.

Follow-up to `level-createscene-wave108-w5615.md` (and the chain back through
`…-wave107-w5514.md`, `…-wave106-w5420.md`, `…-wave105-w5308.md`,
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
  Unchanged from waves 88–108.
* Gate re-measured before and after this pass, green:
  **byte_exact 4500/4524, linked_exact 4500/4524, size-exact 4524/4524,
  stub_zero_size 0, missing 0, wrong_type 0, extra 0, data_exact 843/843,
  imports 207, lint CLEAN ✓, drift 0, RATCHET PASS.**
* **New this pass:** the exact mechanism of the residue is pinned down.
  `MachineLICMBase::SinkIntoLoop` (LLVM 7.0.2 `MachineLICM.cpp:753`) moves a
  preheader instruction back into the loop **iff every use of its def is a
  `COPY`**. The parts-loop `Vector` temporary address `%541 = t2ADDri
  %stack.1, 0` has exactly one use, `$r1 = COPY %541`, so
  `-sink-insts-to-avoid-spills` sinks it into `bb.19`. Without the flag it
  stays in the preheader `bb.12`, its live interval spans the whole loop, and
  Greedy RA spills `k` (`%690`) instead of `hangarPart`; with the flag it is
  a short in-block interval and Greedy reproduces the original assignment
  *exactly* (`zero → $r10/sl`, `hangarPart → $r4` then spilled in `bb.20`,
  `k → $r6`). §2.
* The other early-out in `IsProfitableToHoist` is `CheapInstr &&
  CreatesCopy`; `HasLoopPHIUse(%541)` walks def→uses→`COPY`s→PHIs and returns
  `false` because the `COPY`'s def is the physical `$r1`. §3.
* A non-flag route would require the frame address to select `t2ADDri12`
  (which is *not* `isReMaterializable`), but `rewriteT2FrameIndex`
  (`Thumb2InstrInfo.cpp:478`) always rewrites a frame address whose offset is
  a valid `t2_so_imm` back to `t2ADDri`. The `Vector` temp is at `sp+56`, a
  valid `t2_so_imm`, so it can never become `t2ADDri12` without changing the
  frame layout. §3.
* Full 128-mask sweep of the seven `Globals::rnd` sites **without** the sink
  flag: maximum is **88.3 %** (mask 41 = shipped source; masks 25/43/49/… also
  88.3). No site choice beats the shipped source. §4.

## 1. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5713
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

Scratch is `/dev/shm/w5713` only (not committed):

* `cc.sh` — standalone clang++ 7.0.2 compile of only `Level.cpp` with the
  exact ninja flags (~1.5 s) → `Level.o`;
* `ccsink.sh` — same + `-mllvm -sink-insts-to-avoid-spills` → `Level-sink.o`;
* `cc_mir.sh` / `cc_mirsink.sh` — same + `-mllvm -print-machineinstrs` →
  `mir_all.txt` (base, 4.6 M lines) / `mir_sink.txt` (sink, 4.6 M lines);
* `link.sh` — the exact gold link+strip+reorder command (from
  `ninja -t commands libgof2hdaa.so`), used after copying an object over
  `cmake-build-match/CMakeFiles/gof2.dir/src/game/world/Level.cpp.obj`;
* `score.py` / `score6.py` — official single-function score (imports
  `tools/verify/verify.py` + `elf.py`);
* `try.py <spec.json>` — applies replacements **only inside the
  `Level::createScene` body**, builds (optionally with `"sink": true`), links,
  scores, restores pristine source;
* `masksearch3.py` — scoped 128-mask sweep of the `rnd` sites without the sink
  flag → `mask_nosink.txt`;
* `Level.cpp.base` — pristine copy of `src/game/world/Level.cpp`;
* `dump.py` — emits the reconciled instruction lists to `last_on.txt` /
  `last_un.txt` for `difflib` alignment.

LLVM 7.0.2 sources used for the pass analysis are at
`/dev/shm/wf213/llvm702src/lib/CodeGen/MachineLICM.cpp` and
`/dev/shm/wf213/llvm702src/lib/Target/ARM/{ARMISelDAGToDAG,Thumb2InstrInfo}.cpp`.

## 2. The residue, proven from the MIR

### 2.1 Base build (no flag)

After `Early Machine Loop Invariant Code Motion` the parts-loop preheader
`bb.8`/`bb.12` contains (base `mir_all.txt`, line 1122938):

```
  %529:gprnopc = t2ADDrs %528:tgpr, %3:rgpr, 34   ; kHangarMeshes[sceneRace]
  %530:rgpr    = t2MOVi 192                       ; sizeof(AEGeometry)
  %539:tgpr    = tLDRpci %const.1                 ; pi
  %541:gprnopc = t2ADDri %stack.1, 0              ; &Vector{0, pi, 0} temp
  %542:rgpr    = t2MOVi 304                       ; sizeof(PlayerStatic)
  %545:rgpr    = t2MOVi -1
```

Those stay in the preheader through `Machine code sinking` (line 1126873).
After the pre-RA scheduler `%541` is still defined at 3568B (in `bb.12`) and
used at 4688B (in `bb.19`) — i.e. its live interval spans the loop header and
body. Greedy RA rematerialises it at the use (`%687 = t2ADDri %stack.1, 0`
inside `bb.19`), but the long interval still changes the allocation:

```
; base VRR, bb.19 (mir_all.txt:1158214)
  t2STRi12 killed renamable $r6, %stack.22, 0   ; spill k
  renamable $r4 = t2MOVi 0                      ; zero -> r4
  renamable $r6 = COPY $r0                      ; hangarPart -> r6
```

### 2.2 Sink build

With `-mllvm -sink-insts-to-avoid-spills` the same instructions are emitted in
`bb.14`/`bb.19` (sink `mir_sink.txt`:1123590), so `%541`'s interval is one
block. The VRR output (sink `mir_sink.txt`:1158757) is then **exactly the
original**:

```
; sink VRR, bb.19
  renamable $r1 = t2ADDri %stack.1, 0           ; &temp -> r1
  renamable $r10 = t2MOVi 0                     ; zero -> sl
  renamable $r4 = COPY $r0                      ; hangarPart -> r4
; sink VRR, bb.20
  t2STRi12 killed renamable $r4, %stack.22, 0   ; spill hangarPart
```

which matches the shipped binary byte-for-byte in the parts loop
(`mov.w sl, #0`, `str.w sl, [sp,#56]`, `str.w sl, [sp,#64]`,
`strd sl, sl, [sp]`, `mov r4, r0`, `str r4, [sp,#44]`).

### 2.3 Measured

| variant | pct | size | insns | first diff |
|---|---|---|---|---|
| shipped base | **88.3** | 2252 | 783/791 | 460 |
| sink flag only | 89.3 | 2256 | 783/790 | 28 |
| **sink + `rnd` mask 19/21/35/37** | **93.3** | **2252** | 783/791 | 548 |

The sink flag is **not landable**: applied to the whole `Level.cpp` TU it
drops the gate to `byte 4500 → 2357`, `linked 4500 → 4486` (measured wave
107/108), because the other ~90 functions in the TU are tuned to the
non-sink build.

## 3. Why no flag-free source shape reproduces it

`IsProfitableToHoist` (`MachineLICM.cpp:1188`) has exactly two early exits
before the register-pressure heuristic:

```cpp
bool CheapInstr  = IsCheapInstruction(MI);
bool CreatesCopy = HasLoopPHIUse(&MI);
if (CheapInstr && CreatesCopy) return false;          // (A)
if (TII->isTriviallyReMaterializable(MI, AA)) return true;  // (B)
```

* (B) fires for `%541`: `t2ADDri` is the `ri` member of
  `T2I_bin_ii12rs` (`ARMInstrThumb2.td:769`), which is
  `let isReMaterializable = 1` with the comment *"This is useful in
  particular for taking the address of a local."* `t2MOVi` and `t2LDRpci`
  are likewise rematerializable. So all five hoisted instructions are
  hoisted unconditionally.
* (A) would need `HasLoopPHIUse(%541) == true`
  (`MachineLICM.cpp:1056`). It iterates the def regs of `%541`, then the uses
  of `%541`: the only use is `$r1 = COPY %541`. That `COPY` is inside the
  loop, so it is pushed on the worklist; its def is the **physical** `$r1`,
  so the walk stops. Result: `false`. A real PHI use needs a loop-carried
  pointer update, and every such variant measured in waves 88–108 loses the
  preheader (≥ 2260 B).
* `t2ADDri12` (`ri12`, `ARMInstrThumb2.td:784`) is **not** rematerializable.
  But `rewriteT2FrameIndex` (`Thumb2InstrInfo.cpp:478`) starts from either
  `t2ADDri` or `t2ADDri12`, sets the descriptor to `t2ADDri`, and if the
  resolved offset is a valid `t2_so_imm` returns immediately — so the frame
  address is always `t2ADDri`. The `Vector` temp lives at `sp+56`, a valid
  `t2_so_imm`, so it can never become `t2ADDri12` without moving it to an
  offset ≥ 256 that is not an 8-bit rotation.

## 4. Measured this pass (full official score; `osize=2252`)

Base = **88.3 / 2252**. Everything below is byte-identical to the base unless
noted.

### 4.1 Full 128-mask `Globals::rnd` sweep, no sink flag (`masksearch3.py`)

Sites (in source order): 0 `nextInt(100)<=29`, 1 `race=nextInt(4)`,
2 `nextInt(100)<30`, 3 `pick=nextInt(3)`, 4 `spot=nextInt(capacity)`,
5 `spot=nextInt(capacity)` (tries loop), 6 `nextInt(300)`.
Mask bit = 1 means the site uses the local `rnd` variable, 0 means
`Globals::rnd`. Shipped source = mask 41.

* maximum **88.3 %** at masks **25** and **41** (mask 41 is the shipped
  source; mask 25 = local `rnd` at sites 0/3/4, `Globals::rnd` at 1/2/5/6
  produces the same codegen). Next best: 49 = 88.1, 81/97 = 88.2.
* mask 0 (all `Globals::rnd`) = 75.0 / 2228;
* mask 59 = 85.1 / 2244; mask 19/21/35/37 = 85.0 / 2248.
* With the sink flag the same sweep peaks at **93.3 / 2252** for masks
  19/21/35/37 and 88.8 for mask 18 (scoped) — see §2.3.

### 4.2 Vector construction / scope / type (all 88.3 unless noted)

`vec_assign_expr` (`rot = Vector{...}`) 77.1/2272; `vec_ref`
(`const Vector &rot = Vector{...}`) 88.3; `vec_array` (`Vector rots[1]`) 88.3;
`vec_pi_var` / `vec_zero_var` / `vec_both_var` / `vec_ref_pi` 88.3;
`rot_after_mesh` (named `rot` after `new AEGeometry`) 88.3;
`rot_before_mesh` (named `rot` **before** `new AEGeometry`) 85.4/2256;
`static_vec` (function-local `static const Vector`) 72.6/2228;
`full_name` (`AbyssEngine::AEMath::Vector{...}`) 88.3.

### 4.3 Loop / CFG / type permutations (all 88.3 unless noted)

`if (mesh >= 0) { … }` instead of `if (mesh < 0) continue;` 88.3;
`plain_lt4_ne1` / `plain_direct` 88.3; `unsigned k`, `unsigned i` (ship),
`unsigned i` (spotUsed zero loop), `unsigned capacity`, `unsigned shipCount`,
`unsigned_all`, `unsigned_cap_k/i/i_zero` 88.3;
`KIPlayer *hangarPart` 88.3; `AEGeometry *g;` hoisted 88.3;
`PlayerStatic *hangarPart;` hoisted 88.3; `const int *row` (in-loop or
hoisted) 88.3; `mesh = 0; mesh = …` 88.3; `child_while` 88.3;
`spots_in_default` / `spots_in_default2` 88.3; `rnd_const` 88.3;
`rnd_after_spots` 88.3; `rnd_ref` (`AERandom &rr = *rnd`) 88.3.
Worse: `k_ne4` 87.9, `k_out` 88.1, `mesh_eq_m1` 88.2, `tries_neg` 87.6,
`spots_ternary_only` 86.5, `ps_before_rot` 85.2/2256,
`child_before_ps` 62.7/2236, `k_while` 67.8/2292,
`rnd_before_cap` 63.6/2224, `spots_in_loop` compile error (needs `spots`).

### 4.4 Best loop-variant address (unchanged from wave 108)

`off_ge0m1` = `Vector *v = (Vector *) ((char *) &rot + ((mesh >= 0) - 1));`
reproduces the original parts-loop register signature and prevents the
`%541` hoist (the offset folds into `t2ORRrs` inside the loop), but costs
**86.2 % / 2260 B** (extra `orr.w r1, r0, r4, lsr #31`, `str r6, [sp,#44]`,
`mov r6, r9`, `mov r9, r6`).

## 5. Proven vs hypothesis

**Proven (measured on the shipped binary / this exact base):**

1. The parts-loop residue is caused solely by `%541`'s live interval spanning
   `bb.12 → bb.19`; the sink build's VRR output reproduces the original
   assignment exactly (`zero→sl`, `hangarPart→r4`, `k→r6`, `hangarPart`
   spilled in `bb.20`).
2. `SinkIntoLoop`'s acceptance test is "all uses of the def are `COPY`s"; the
   `Vector` temp address satisfies it (single `$r1 = COPY %541` use).
3. `IsProfitableToHoist` hoists `t2ADDri` unconditionally because it is
   `isReMaterializable`; `t2ADDri12` is not, but `rewriteT2FrameIndex`
   normalises a `t2_so_imm` frame offset (56) back to `t2ADDri`.
4. `HasLoopPHIUse(%541)` returns `false` (COPY def is physical `$r1`).
5. No `Globals::rnd` site mask beats 88.3 % without the sink flag (full 128
   sweep).
6. ~40 further source shapes (Vector construction/scope/type, loop forms,
   `rnd` caching, statement reordering, CFG rewrites) are byte-identical to
   the shipped base or worse.

**Hypothesis (unproven):**

* The original binary's `createScene` was produced by a build in which the
  MachineLICM sink ran for this function (or an equivalent live-interval
  shrink), while the rest of the TU is unaffected. A source-only shape that
  achieves this with the 72-byte frame and the 2252-byte size was not found
  in the ~2500 variants across waves 88–109.

## 6. Structural state (unchanged by this pass)

* ELF section headers: **28** in ours vs **27** in the original — the extra
  section is the 192-byte `.jcr` that carries the `Status::missionCompleted`
  rank table (wave-108 state, still present). File size is **+44** (2,187,372
  vs 2,187,328).
* `.text` size `0x133828` (orig `0x1337c8`, +0x60).
* `.ARM.extab` at `0x1a3bd8`, size `0x26294` — **4 B short** of the original
  (`0x1a3b78`, size `0x26298`). Unchanged by this pass.
* `.rodata` size `0x41b67` — size-exact; `.dynsym`, `.dynstr`, `.hash`,
  `.dynamic`, `.rel.dyn` size-exact (content deltas as recorded in wave 108).
* `createScene` is 2252 B in both.
* This pass touched no source file, so no section delta was introduced.

## 7. Bottom line

`Level::createScene` remains **88.3 % / 2252 B** (783/791 insns, first diff
460). The wave-108/109 sink experiments prove *why*: the parts-loop `Vector`
temporary address is hoisted out of the loop by MachineLICM and only
`-sink-insts-to-avoid-spills` (or a different frame offset that selects
`t2ADDri12`) can put it back. No source change landed; the worktree is clean
and the gate is green (byte 4500 / linked 4500 / lint CLEAN / drift 0 /
RATCHET PASS).
