# `Level::createScene` — wave-105 w5308: REA decompilation of the *shipped* target
# confirms the all-`Globals::rnd` source; the sink-flag parts loop is exact and
# the residual is the ship-loop `rnd` value cache (2252 B)

**Status:** research note, 2026-11-04. Worktree `wt/w5308`, branch `wave105-w5308`,
base `835748c18`. **No source change landed** — no source shape found that raises
`byte_exact` or the fuzzy score while keeping the 2252-byte size. This commit
contains only this note.

Follow-up to `level-createscene-wave103-wf312.md` (which followed
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
* Gate re-measured before and after this pass, unchanged and green:
  **byte_exact 4500/4524, linked_exact 4500/4524, size-exact 4524/4524,
  stub_zero_size 0, missing 0, wrong_type 0, extra 0, data_exact 843/843,
  imports 207, lint CLEAN ✓, drift 0, RATCHET PASS.**
* **New evidence: REA (Ghidra 12.1.2) decompilation of the shipped
  `android_2.0.16_libgof2hdaa.so` itself** (not the KiritoJPK / iOS oracles used
  by waves 99–103) confirms the source semantics statement-for-statement and
  prints `AbyssEngine::AERandom::nextInt(Globals::rnd, …)` at **all seven**
  `nextInt` sites in the ship loop. §1.
* **New: the `-mllvm -sink-insts-to-avoid-spills` flag on the current base is
  89.3 % / 2256 B / 783 vs 790 insns.** The parts loop becomes byte-identical
  (first real structural diff moves to instruction 547, the ship loop); the
  +4 bytes are the ship-loop `rnd` spill pair. This reproduces the wave-88
  result on the wave-104 base and proves the parts-loop residue is the hoisted
  preheader live intervals, of which `%541` (`t2ADDri %stack.1, 0`) is the
  largest (the flag also sinks the preheader `t2MOVi` constants; see §7). §2.
* The flag's effect is deterministic and TU-context-independent: the
  pre-RA/after-greedy/VRR MIR of `createScene` in a standalone TU and in the
  full `Level.cpp` are identical (only `debug-location`/TBAA IDs differ), and
  both give `row → r9` (the original's parts loop). The full TU is 2256 B with
  the flag. §2.1.
* ~95 further source shapes were measured this pass (branch-order permutations,
  inline `plainHangar` condition, `if (mesh >= 0)` block, `k != 4`, `mesh == -1`,
  child-count/child-mesh hoists, `unsigned`/`int` types, `char`/`uchar`
  `spotUsed`, `new[]()` zero-init, `&&`/ternary `ownHangar`, `min` shipCount,
  `Vector` position locals, `table + spot*3`, `no_scope`, `enemies_bool`,
  dummy vregs, `rnd` reference/pointer-to-pointer, …; the continuation batch is
  in §7). **None beats 88.3 % at 2252 B.** §3, §7.
* Conclusion unchanged: the parts loop is a greedy-RA artifact of the hoisted
  Vector-temp address; the ship loop is the `r5` value cache. No source-only
  lever was found that fixes both while preserving the prologue and size.

## 1. REA decompilation of the shipped target

Command (first import ~3 min, saved output reused):

```sh
GHIDRA_INSTALL_DIR=/tmp/ghidra_12.1.2_PUBLIC rea decompile \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --procedure _ZN5Level11createSceneEv --format json > rea-createscene.json
```

The normalized result is semantically identical to `src/game/world/Level.cpp`
in every statement, loop, call order and constant. The missionPtr==23 branch
reads (Ghidra spelling):

```c
uVar13 = createShip(this,iVar12,0,iVar4,(Waypoint *)0x0,
                    *(int *)(this + 0xc0) != 0x17,false);
…
for (uVar22 = 0; uVar22 < 4; uVar22 = uVar22 + 1) {
  iVar4 = *(int *)(&UNK_0020d678 + uVar22 * 4 + uVar6 * 0x10);   /* kHangarMeshes */
  if (-1 < iVar4) {
    pAVar23 = operator_new(0xc0);
    AEGeometry::AEGeometry(pAVar23,(ushort)iVar4,Globals::Canvas,false);
    AEGeometry::setRotation((Vector *)pAVar23);                  /* Vector temp */
    pPVar14 = operator_new(0x130);
    PlayerStatic::PlayerStatic(pPVar14,-1,pAVar23,extraout_s0_05,extraout_s1_05,extraout_s2_05);
    if ((int)uVar6 < 4 && uVar6 != 1) {                          /* !plainHangar */
      iVar12 = *(int *)(&UNK_0020b818 + uVar6 * 4);              /* kBarChildCounts */
      for (iVar4 = 0; iVar4 < iVar12; iVar4 = iVar4 + 1) {
        uVar13 = *(undefined4 *)(&UNK_0020b828 + uVar6 * 4);     /* kBarChildMeshes */
        this_00 = operator_new(0xc0);
        AEGeometry::AEGeometry(this_00,(short)iVar4 + (short)uVar13,Globals::Canvas,false);
        AEGeometry::addChild(pAVar23,*(uint *)(this_00 + 0xc));
        pvVar3 = (void *)AEGeometry::~AEGeometry(this_00);
        operator_delete(pvVar3);
      }
    }
    ArrayAdd<KIPlayer*>((KIPlayer *)pPVar14,*(Array **)(this + 0xf8));
  }
}
```

The ship loop prints `AbyssEngine::AERandom::nextInt(Globals::rnd, …)` at
**all seven** sites (top 100, inner 4, inner 100, pick 3, spot, retry spot,
rotation 300). This is the *shipped* binary, so the original source is the
all-`Globals::rnd` form — not the local `AERandom *rnd` our tree carries.

The `spots`/`table` selection is the compiler-generated 9-entry pointer table
at `0x20d9c0` (image base `0x10000`; Ghidra `PTR_DAT_0021d9c0`), filled with
`kHangarSpotsTerran[0]` in the unused slots, guarded by
`sceneRace < 9 && ((0x10b >> sceneRace) & 1)` — exactly the lowering of the
`switch (sceneRace) { case 0,1,3,8: … default: table = spots; }` in our source.
No source difference was found.

## 2. The sink flag on the current base: parts loop exact, ship loop remains

`-mllvm -sink-insts-to-avoid-spills` scoped to `Level.cpp` (temporarily patched
into `cmake-build-match/build.ninja`; **not** landed):

| build | pct | size | insns | first real diff |
|---|---|---|---|---|
| shipped base | **88.3** | **2252** | 783/791 | 460 (parts preheader) |
| base + sink flag | **89.3** | 2256 | 783/790 | 547 (ship loop) |

The first `|` at instruction 28 in the sink build is only the `bne.w 0x840`
vs `0x844` branch target (function is 4 bytes longer); every prologue
instruction is byte-identical. The parts loop then matches the original
instruction-for-instruction (`row → r9`, `Canvas → r8`, `zero → sl`, `k → r6`,
`hangarPart → sp#44`, `j → sl`, `childCount → fp`, `childMesh → r9`,
`child → r4`, `g → r5`). The remaining diff is the ship-loop `rnd` spill pair
(`str r2,[sp,#44]` + `ldr r0,[r2,#0]` instead of `ldr r0,[r8]`) and the
`capacity/race/type` rotation.

This is the wave-88 result reproduced on the wave-104 base and confirms the
parts-loop residue is the hoisted `%541` (`t2ADDri %stack.1, 0`, the
`Vector{0, π, 0}` temp address) live interval, as wave-88 §2 established.

### 2.1 The flag is deterministic and TU-context-independent

A standalone TU containing only the 64 `Level.cpp` includes + the
`createScene` body (`/dev/shm/w5308/cs_only.cpp`) compiles with the same
`-print-machineinstrs` MIR as the full `Level.cpp`:

* `# After Early Machine Loop Invariant Code Motion` without the flag: `%541`
  is in the preheader (`t2ADDri %stack.1, 0` at slot 3568B);
* with the flag: `%541` is in the loop body (slot 4680B).
* The pre-greedy, after-greedy and after-VRR MIR of the standalone and full TU
  are identical apart from `debug-location`/TBAA IDs; both give
  `renamable $r9 = t2ADDrs …, 34` (row in `r9`).
* Standalone size: 2252 B base, 2256 B with the flag. Full TU: 2252 / 2256.
  The 4 bytes are the ship-loop `rnd` spill, not the parts loop.

## 3. New source shapes measured this pass (all ≤ 88.3 % at 2252 B)

All via the official `link_so` + `tools/verify` score on the current base.

Branch order (`if (missionPtr == …)` blocks permuted):

| order | pct | size | firstdiff |
|---|---|---|---|
| **23,4,2 (shipped)** | **88.3** | 2252 | 460 |
| 23,2,4 | 88.3 | 2252 | 460 |
| 2,4,23 | 85.9 | 2252 | 22 |
| 2,23,4 | 74.1 | 2252 | 5 |
| 4,23,2 / 4,2,23 | 59.0 | 2256 | 6 |

Parts loop / prelude:

| variant | pct | size | firstdiff |
|---|---|---|---|
| `if (mesh >= 0) { … }` block | 88.3 | 2252 | 460 |
| `k != 4` | 87.9 | 2252 | 460 |
| `mesh == -1` | 88.2 | 2252 | 460 |
| `int childCount = kBarChildCounts[sceneRace]` local | 88.3 | 2252 | 460 |
| `(unsigned short) mesh` | 88.3 | 2252 | 460 |
| `new PlayerStatic(-1, g, 0, 0, 0)` | 88.3 | 2252 | 460 |
| `Vector rot(0, π, 0); g->setRotation(rot)` | 88.3 | 2252 | 460 |
| inline `if (sceneRace < 4 && sceneRace != 1)` (no `plainHangar`) | 86.1 | 2252 | 28 |
| `if (plainHangar == false)` | 86.1 | 2252 | 28 |
| `int childMesh = kBarChildMeshes[sceneRace]` inside `if` | 80.0 | 2252 | 25 |
| both child-count/mesh hoists | 79.9 | 2252 | 25 |
| child-mesh base hoisted before `plainHangar` | 71.7 | 2244 | 6 |
| `unsigned k` | 88.3 | 2252 | 460 |
| `j != count` | 88.3 | 2252 | 460 |
| `mesh <= -1` | 88.3 | 2252 | 460 |

Ship loop / prelude:

| variant | pct | size | firstdiff |
|---|---|---|---|
| `int race` hoisted before loop | 88.3 | 2252 | 460 |
| `int type` hoisted before loop | 88.3 | 2252 | 460 |
| `tries > 0` | 88.3 | 2252 | 460 |
| `while (tries != 0 && …)` | 88.3 | 2252 | 460 |
| `pick` if/else chain | 88.3 | 2252 | 460 |
| `ownHangar != 0` | 88.3 | 2252 | 460 |
| `ownHangar = idx == 108 && field_114 == 3` | 88.3 | 2252 | 460 |
| `ownHangar = idx == 108 ? field_114 == 3 : false` | 88.3 | 2252 | 460 |
| `shipCount` if/else instead of ternary | 88.3 | 2252 | 460 |
| `char *spotUsed` | 88.3 | 2252 | 460 |
| `new unsigned char[capacity]()` | 84.9 | 2244 | 28 |
| `missionPtr != 0x17` | 88.3 | 2252 | 460 |
| `int stationIndex` local | 80.7 | 2244 | 6 |
| `const int *table = spots` + switch without default | 83.7 | 2248 | 9 |
| `Vector pos(…); ship->setPosition(pos)` | 75.0 | 2264 | 8 |
| `ship->setPosition(Vector{…})` | 75.0 | 2264 | 8 |
| `int height = kShipHangarHeights[type]` | 88.3 | 2252 | 460 |
| `const int *spotPos = table + spot*3` | 88.3 | 2252 | 460 |
| remove `sceneRace` inner scope | 88.3 | 2252 | 460 |
| `if (this->enemies)` | 88.3 | 2252 | 460 |

Types / dummy vregs (all 88.3 / 2252 / 460 unless noted):
`unsigned sceneRace` 88.1/452, `unsigned shipIndex`, `unsigned capacity`,
`unsigned shipCount`, `unsigned spot`; `int four = 4`, `int zero = 0`,
`int one = 1`, `float pi = 3.1415927f`, `int sceneRace2 = sceneRace`,
`plainHangar` before `shipIndex`; `unsigned mesh` 76.0/2244/8.

All-globals hybrids (with and without the sink flag):

| shape | pct | size |
|---|---|---|
| all-globals | 75.0 | 2228 |
| all-globals + sink flag | 75.9 | 2232 |
| `AERandom *&rnd = Globals::rnd` | 75.0 | 2228 |
| `AERandom *&rnd` + sink flag | 75.9 | 2232 |
| `AERandom **rndp` | 75.0 | 2228 |
| all-globals + `capacity` first | 77.2 | 2264 |
| all-globals + `enemies` local | 73.6 | 2216 |
| mask 6 (top global, pick/tries local) + sink | 86.9 | 2260 |
| mask 5 (pick global) + sink | 87.2 | 2260 |
| mask 4 (tries local only) + sink | 66.7 | 2236 |
| `rnd` inside the loop + sink | 87.3 | 2256 |

## 4. Exact remaining diff (shipped base)

First diff `0xb4830` (instruction 460):

```
orig:  add.w r9, r0, r2, lsl #4      ; row = kHangarMeshes[sceneRace]
       str.w r9, [sp, #24]
ours:  add.w r8, r0, r2, lsl #4
       str.w r8, [sp, #24]
```

Register maps (unchanged from waves 98–103):

| value | original | shipped |
|---|---|---|
| parts `row` | `r9` | `r8` |
| parts `&Globals::Canvas` | `r8` | `r9` |
| parts zero | `sl` | `r4` |
| parts `k` | `r6` | spilled `sp#44` |
| parts `hangarPart` | `sp#44` | `r6` |
| parts `j` / child count | `sl` / `fp` | `sl` / `r8` |
| parts child mesh | `r9` | `fp` |
| ship `&Globals::rnd` | `r8` (value reloaded) | spilled `sp#44` |
| ship `rnd` value | never cached | `r5` |
| ship `capacity` | `r6` | `fp` |
| ship `race` | `r5` | `r6` |
| ship `type` | `fp` | `r8` |
| ship `i` | `r9` | `r9` |
| ship `&Globals::status` | `sl` | `sl` |
| ship `spotUsed` | `r4` | `r4` |

The instruction *stream* is identical modulo register names, spill/reload
pairs and literal-pool offsets; there is no missing/extra code, wrong constant,
wrong callee or wrong field.

## 5. Next hypothesis

The two residues are independent:

1. **Parts loop:** `%541 = t2ADDri %stack.1, 0` (the `Vector{0, π, 0}` temp
   address) is hoisted by `EarlyMachineLICM` because `t2ADDri` is
   `isReMaterializable = 1` (`ARMInstrThumb2.td`, `T2I_bin_ii12rs`). Its live
   interval spans the loop and pushes the greedy allocator into
   `row→r8 / Canvas→r9` and `k↔hangarPart`. `-sink-insts-to-avoid-spills`
   fixes it exactly but costs +4 bytes (ship loop) and is a build flag.
   A source shape that selects the non-rematerializable `t2ADDri12` (used when
   the frame offset is not a `t2_so_imm`) would not be hoisted. The observed
   offset is `sp+56`; every offset ≤ 255 is a modified immediate, so this
   needs a frame-layout change that keeps the 72-byte frame — not yet found.
2. **Ship loop:** the original keeps `&Globals::rnd` in `r8` and reloads the
   value at all seven sites (all-`Globals::rnd` source, confirmed by REA). The
   shipped local `rnd` is rematerialised as a value cache in `r5`, which forces
   `race→r6`, `capacity→fp`, `type→r8` and spills `&rnd`. Removing the local
   frees `r5`, which the allocator then gives to the `&Globals::status` GOT
   address in the prologue (first diff at instruction 6–28). The remaining
   lever is a source shape that keeps the shipped prologue (guard address
   spilled to `sp#36`, `&Globals::status` in `sl`, `this` in `r6`) while the
   ship loop uses the all-globals form. ≈1850 variants have now been measured
   across waves 88–105 without finding it.

## 6. Reproduction

Scratch is `/dev/shm/w5308` only.

```sh
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

Scratch files:

* `/dev/shm/w5308/rea-createscene.json` — REA decompilation of the shipped
  target (the §1 evidence);
* `/dev/shm/w5308/Level.cpp.base` — pristine `src/game/world/Level.cpp`;
* `/dev/shm/w5308/fscore.py` — official single-function score;
* `/dev/shm/w5308/runvar.py <spec.json>` — apply `[[old,new],…]` replacements
  inside `createScene`, build `link_so`, score;
* `/dev/shm/w5308/reorder.py <order>` — permute the missionPtr branches;
* `/dev/shm/w5308/cs_only.cpp`, `build_cs.sh`, `mkmir2.sh` — standalone-TU
  MIR/RA experiment;
* `/dev/shm/w5308/show_base.txt`, `show_sink2.txt`, `show_allg.txt`,
  `show_allg_capf.txt` — side-by-side disassemblies.

The REA oracle and the original binary were read-only and not modified.

## 7. Continuation: further shapes and the sink-pass scope

A second batch (~40 shapes) was measured on the same base after the note above
was first drafted. All are **88.3 % / 2252 B / first diff 460** unless noted:

* `int spot = rnd->nextInt(capacity)` (spot site local) — **87.4 % / 452**;
  `spot` local + `tries` global — 88.3 %;
* `race = rnd->nextInt(4)` — 85.3 % / 2244 B / first diff 28;
* rotation site local (`rnd->nextInt(300)`) — 87.7 % / first diff 28;
* `AEGeometry *const g`, `KIPlayer *hangarPart`, `ArrayAdd<KIPlayer *>(...)`,
  `delete (AEGeometry *) child`, `(uint16_t) mesh`, `(int) kBarChildMeshes[...] + j`;
* `Vector(0.0f, 3.1415927f, 0.0f)` parens, `Vector rot; rot.y = ...`,
  `const Vector &rot = Vector(...)`, `Vector(0, 3.1415927f, 0)` int zeros;
* `Vector rot` declared before `new AEGeometry` — 83.9 % / 2260 B;
  `Vector rot` at function scope — 78.1 % / 2192 B;
* `bool plainHangar = (sceneRace == 1 || sceneRace > 3)` and
  `sceneRace == 1 || 3 < sceneRace`;
* `for (int k = 0; k < 4; ++k)`, `4 > k`, `k += 1`;
* `if (mesh > -1) {} else continue;`, `int mesh = *(kHangarMeshes[sceneRace] + k)`;
* `int capacity = *(kHangarCapacity + sceneRace)`,
  `&kHangarSpotsKhador[0][0]` (86.5 % / first diff 9),
  `for (int i = 0; i != shipCount; i++)`, `(int) sceneRace`,
  implicit float conversion at the rotation site;
* `Level *self = this` (used in the 23 branch) — 88.3 %;
  `Status *status = Globals::status` — 79.3 % / 2240 B / first diff 6;
  `PaintCanvas *canvas = Globals::Canvas` — 65.4 % / 2212 B / first diff 5;
* `AERandom *volatile rnd` / `*const volatile rnd` — 76.8 % / 2232 B /
  first diff 8 (reloads from the stack, not from `&Globals::rnd`).

**Sink-pass scope (new MIR evidence).** The `-sink-insts-to-avoid-spills`
effect is *not* limited to `%541`. Diffing the "After Early Machine Loop
Invariant Code Motion" MIR of `cs_only.cpp` with and without the flag shows the
pass also moves `%530` (`t2MOVi 192`), `%542` (`t2MOVi 304`), `%545`
(`t2MOVi -1`) and the ship-loop constants `100/4/3/300` from the preheader into
the loop body (all their uses are `COPY`s). So "the flag fixes the parts loop"
is reproducible but the fix is the whole set of sunk rematerializable
instructions, not `%541` alone; the wave-88 live-interval argument still
identifies `%541` as the largest of them.

`self_local` is the only new shape that keeps the shipped 88.3 %/2252 B; it is
a no-op rename (`this` → `self` for the two `createShip` calls) and changes
nothing observable.
