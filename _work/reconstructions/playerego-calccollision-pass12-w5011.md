# PlayerEgo::calcCollision — pass 12 (w5011): the real source is the nested
# `if (object != nullptr)` shape, but its `Vector surface` scope costs a
# stack slot and the loop-rotation tie-break

**Status:** note only, 2026-11-04, wave 100. Worktree `wt/w5011`, branch
`wave100-w5011`, base master `44225192c`.

Target: `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` (orig 968 B,
347 instructions).  The landed source is unchanged (pass-9 two-anchor shape,
**87.6 % / 968 B / 349 insns**).  No strict score improvement was found this
pass; the value is that the *real source structure* is now identified and the
remaining blocker is narrowed to one stack slot and one MBSP tie-break.

Gate (verified with `check` before and after):
`byte_exact 4500/4524`, `linked_exact 4500/4524`, size-exact 4524/4524,
`data_exact 843/843`, imports 207, lint CLEAN, drift 0, `.text 0x1337c8`,
`.rodata 0x41b67`, `.ARM.exidx 0x6f70`, `.ARM.extab 0x26294 (−4)`.

---

## 1. New this pass: the real source is the *nested* shape, not `continue`

The Ghidra decompile (headless `DecompAt2.java`, `0xaf0cc`) shows the whole
loop body is wrapped in `if (this_01 != 0) { … }` and the outerCollide test
is a nested `if (iVar6 != 0) { … }` — not `if (object == nullptr) continue;`
plus `if (!outerCollide) continue;`.  The decompiler also renders the three
collision arms as an `if (wormhole) … else if (stealFlagByte == 0) … else …`
chain, and `hitCamera` is a single tail-merged block shared by the steal and
collision arms (`LAB_000af386`).

Written out:

```cpp
for (unsigned int i = 0; i < objects->size(); i++) {
    KIPlayer *object = (*objects)[i];
    if (object != nullptr) {
        if (i == 0 && !Globals::status->inAlienOrbit()
            && AbyssEngine::AEMath::VectorLength(this->dockOffsetVec) < 16000.0f
            && object->field_0x71)
            this->collidesWithStationFlag = 1;
        Vector surface = this->geometry->getPosition();
        if (object->outerCollide(surface)) {
            if (object->getType() == 16994 && object->isVisible()) {
                if (!((PlayerWormHole *) object)->isShrinking() && this->miningGame == 0) {
                    … wormhole body …
                }
            } else if (object->stealFlagByte == 0) {
                if (object->isVisible()) {
                    Vector collision = object->projectCollisionOnSurface(
                        this->geometry->getPosition());
                    if (Vector{0,0,0} != collision) { …; if (i == 0 && object->field_0x71) flag = 1; }
                    this->hitCamera();
                }
            } else {
                if (!object->isDead() && … ) {
                    Vector impact = object->getProjectionVector(this->geometry->getPosition());
                    if (impact != Vector{0,0,0}) { …; this->hitCamera(); }
                }
            }
        }
    }
}
```

This shape (`body_NEST.txt` in `/dev/shm/w5011`) reproduces the original's
**block layout exactly**:

* top-tested loop header at `9f134` (`ldr r0,[r4,#0]; cmp fp,r0; bcs.w exit`),
  body at `9f13c`, latch at `9f38c`, cold wormhole blocks after the latch,
  exit last;
* the steal arm laid out before the collision arm (`ldrb r0,[r8,#60];
  cmp r0,#0; beq collision`), exactly like the original `9f244`/`9f2f6`;
* a single tail-merged `hitCamera` for the steal + collision arms;
* 968 B / 349 insns — the same size as the original.

The only remaining differences are the register file and the frame size:

| | score | size | insns | frame | this | object | dock | i | surface | guard |
|---|---:|---:|---:|---:|---|---|---|---|---|---|
| original | 100 | 968 | 347 | 112 | r5 | r8 | r9 | fp | r6 | `[sp,#24]` |
| committed (pass 9) | **87.6** | 968 | 349 | 112 | r5 | r8 | r9 | fp | r6 | `[sp,#24]` |
| **nested (`body_NEST`)** | 64.1 | 968 | 349 | **120** | r5 | r9 | sl | fp | r8 | r6 |
| nested + named `position` (`body_NEST_x`) | 67.5 | 968 | 349 | **128** | r5 | **r8** | fp | sl | **r6** | `[sp,#24]` |
| faithful (continue, no anchors) | 59.8 | 988 | 349 | 112 | sl | r8 | r9 | fp | r5 | `[sp,#20]` (strd) |
| faithful + nested outerCollide (`body_NEST_c`) | 58.6 | 960 | 346 | 120 | r5 | r9 | sl | fp | r8 | r6 |

## 2. The blocker is `Vector surface`'s lifetime (one stack slot)

The original frame is 112 bytes and `surface` lives at `sp+32`, the same
36-byte scratch region as the `ml` Matrix result and the wormhole `velocity`
(`add r6,sp,#32` in the preheader; `mov r0,r6` for `getPosition`, for the
`ml` sret, and `strd r0,r0,[sp,#32]` for `velocity`).  So `surface`'s live
range must end before the wormhole block.

In the nested shape, `Vector surface` is declared in the
`if (object != nullptr)` scope, so clang emits `llvm.lifetime.end` at the
**end of the loop body** (label 344 in the IR).  `StackColoring` therefore
cannot share it with `velocity`/Matrix and gives it its own 12-byte slot at
`sp+104`, growing the frame to 120 and shifting every in-loop offset by 8.

Every source shape that shortens `surface`'s scope also flips the loop
rotation:

| shape | surface scope | frame | loop |
|---|---|---:|---|
| nested `if (object != nullptr)` + `Vector surface` in body | loop body | 120 | **header-first** |
| nested + `{ Vector surface; … }` around the `if (outerCollide)` | block | 120 | header-first |
| `continue` for null check + `Vector surface` in body | loop body | 120 | header-first |
| `continue` + `{ Vector surface; if (!outerCollide) continue; }` | block | 112 | **rotated** |
| `continue` + `{ Vector surface; hit = outerCollide; } if (hit)` | block | 112 | rotated |
| `if (object != nullptr)` + `{ … hit = outerCollide; } if (hit)` | block | 112 | rotated |
| `object->outerCollide(this->geometry->getPosition())` (unnamed temp) | none | 112 | rotated |

So the original has **both** frame 112 **and** header-first, which no shape
tested produces.  `MachineBlockPlacement::rotateLoop` puts the loop's only
exiting block (the header) last — i.e. rotates — whenever the surface slot
is short-lived; with the long-lived surface slot the tie-break goes the
other way.  The entry and exit edge frequencies are equal (the exit edge is
`header × 3.12 %`, the entry edge is one preheader execution), so this is a
pure tie-break, not a probability difference.

## 3. The guard-address spill is a register-pressure effect

`&__stack_chk_guard` is loaded once in the prologue and used once in the
epilogue.  In the original it is **spilled immediately** (`str r0,[sp,#24]`,
`ldr r1,[sp,#24]; ldr r1,[r1,#0]` in the epilogue) and `r6` is free for
`surface`.  In the nested shape it stays in `r6` across the whole loop and is
merged into the preheader `strd` with `&dockOffsetVec` (`strd sl,r6,[sp,#20]`,
reload at the latch), which pushes `surface` to `r8`, `object` to `r9` and
`dock` to `sl`.

Adding one more long-lived value forces the guard out of `r6`: the nested
shape with a named `Vector position` for `projectCollisionOnSurface`
(`body_NEST_x`) spills the guard immediately, gets `this=r5, object=r8,
surface=r6`, and jumps 64.1 → 67.5 — but the extra local grows the frame to
128 and swaps `i`/`dock` to `sl`/`fp`.  The committed body has the same
effect from its anchor latch block (the anchor creates a second latch, which
is why the guard is spilled there).

So the original's register file needs **8** competing callee-saved values
(this, objects, object, dock, i, surface, the collision-block `this` scratch,
and the guard) for 7 registers, with the guard — the lowest-weight value —
spilled.  The nested shape only has 7, so nothing is spilled.

## 4. Variant table run this pass (official `verify.score`)

All on the real source shape unless noted; 968 B unless stated.

| variant | pct | size | insns | frame | note |
|---|---:|---:|---:|---:|---|
| committed (2 anchors) | **87.6** | 968 | 349 | 112 | landed |
| nested (`body_NEST`) | 64.1 | 968 | 349 | 120 | real source structure |
| nested + nested surface block | 64.1 | 968 | 349 | 120 | identical to NEST |
| nested + `const Vector &surface` | 64.1 | 968 | 349 | 120 | identical |
| nested + `int hit` + surface block | 59.7 | 992 | 350 | 112 | rotated |
| nested + `int hit`, surface in body (`body_NEST_j`) | 58.6 | 960 | 346 | 120 | header-first |
| nested, null check `continue`, outerCollide nested (`body_NEST_c`) | 58.6 | 960 | 346 | 120 | header-first |
| nested + named `position` (`body_NEST_x`) | 67.5 | 968 | 349 | 128 | guard spilled |
| nested, no named surface + named `position` (`body_NEST_y`) | 63.1 | 972 | 350 | 120 | |
| nested, `continue` + named `position` (`body_NEST_z`) | 62.3 | 960 | 346 | 128 | |
| nested, surface at loop top (`body_NEST_a`) | 58.4 | 984 | 352 | 120 | extra `Vector::operator=` |
| nested, surface at function scope (`body_NEST_b`) | 44.1 | 1000 | 356 | 120 | ctor zeroing in prologue |
| nested, surface init before null check (`body_NEST_t`) | 59.9 | 964 | 348 | 120 | getPosition hoisted |
| nested, `while`-style cached size (`body_NEST_o`) | 63.1 | 984 | 350 | 120 | objects→sl |
| nested, reversed loop cond (`body_NEST_m`) | 59.8 | 988 | 349 | 112 | rotated |
| nested, `i != size` (`body_NEST_n`) | 59.8 | 988 | 349 | 112 | rotated |
| nested, nested `i == 0` guard (`body_NEST_r`) | 64.1 | 968 | 349 | 120 | identical |
| nested, real collision guard (`body_NEST_s`) | 64.1 | 968 | 349 | 120 | identical |
| faithful, unnamed outerCollide temp (`body_NEST_v`) | 59.8 | 988 | 349 | 112 | rotated |

Anchor-combination matrix on the committed body (`A1` = `i > size-1`
after outerCollide, `A2` = `size == i` after the wormhole check, `G` = the
real `i == 0 && field_0x71` collision guard):

| A1 | A2 | G | pct | size | insns |
|---|---|---|---|---|---:|---:|
| 1 | 1 | 0 | **87.6** | 968 | 349 |
| 1 | 1 | 1 | 85.0 | 976 | 352 |
| 1 | 0 | 1 | 82.6 | 968 | 348 |
| 1 | 0 | 0 | 79.5 | 964 | 347 |
| 0 | 1 | 1 | 70.7 | 988 | 352 |
| 0 | 1 | 0 | 70.8 | 984 | 351 |
| 0 | 0 | 1 | 59.8 | 996 | 352 |
| 0 | 0 | 0 | 59.8 | 988 | 349 |

`-mllvm -stress-regalloc=N` (temporary CMakeLists override, reverted) on the
committed body never beat 87.6: N=12 gives 75.8 / 968 / 344, N=5..16 spans
35–76 %.  The flag is not part of the original build, so it is diagnostic only.

## 5. What is left

The remaining work is a source shape that (a) keeps the nested
`if (object != nullptr)` layout (header-first, one latch, tail-merged
`hitCamera`), (b) shortens `Vector surface`'s lifetime so it shares the
`sp+32` Matrix/velocity slot (frame 112), and (c) still leaves 8 competing
callee-saved values so the guard is spilled to `[sp,#24]`.  The two are
coupled through `MachineBlockPlacement::rotateLoop`'s tie-break; the most
promising untried lever is a source construct that makes `surface`'s
`lifetime.end` land immediately after `outerCollide` *without* adding a
block (e.g. an inlined helper that owns the `Vector surface` local, or a
`goto` that the IR builder folds into the existing latch).

## 6. Artifacts

`/dev/shm/w5011/` (scratch, not committed): `body_current.txt` (landed),
`body_NEST*.txt` (nested-shape family), `body_comb_*.txt` (anchor matrix),
`probe2.sh` (build + score + frame/register/rotation probe), `mir.sh`
(`-print-after-all -filter-print-funcs` MIR dump), `mir_nest.txt`,
`mir_committed.txt`, `orig_dis.txt`, `NEST_dis.txt`.
