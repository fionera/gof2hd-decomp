# PlayerEgo::calcCollision — pass 7 (w4254): the canary's r6-vs-stack split,
# measured on the real NDK clang MIR

**Status:** research note, 2026-11-03. Worktree `wt/w4254`, branch
`wave92-w4254`, base master `b379d00d`. **No source change landed.** The
committed shape (one artificial `objects->size() <= i` anchor after
`outerCollide`, no final `i == 0` guard) is still the best at
**87.3 % / 968 B**.

Target: `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` (968 B, 87.3 %,
347 original instructions vs 345 ours).

This pass was asked to find a **faithful** (anchor-free) source shape that
spills `&__stack_chk_guard` while keeping `this=r5 / surface=r6 / object=r8`
and leaving `sl` free for the impact scratch. The new evidence is the
**actual NDK clang 7.0.2 MIR** (`-mllvm -print-after-all`), not the local
LLVM 7.0.0svn `-O3` model used in passes 3–6. It pins the blocker exactly:
the canary pointer's second-round `RS_Split` chooses `$r6` at a split cost of
**1.448** against a spill cost of **2.0**; the original must have made that
split cost exceed 2.0. No source shape tested this pass does that without
also changing `dock`/`surface`.

---

## 1. Ground truth: the original body (Ghidra, inlined)

Produced with headless Ghidra 12.1.2 on
`android_2.0.16_libgof2hdaa.so_ghidra` (`DecompCalc.java`, `-noanalysis`).
Same body as pass 5/6 §1:

```c
void __thiscall PlayerEgo::calcCollision(PlayerEgo *this,Array *param_1)
{
  float fVar1; float fVar2; int *piVar3; Vector *this_00; undefined4 *puVar4;
  int *piVar5; int iVar6; float fVar7; Matrix *pMVar8; Matrix *pMVar9;
  KIPlayer *this_01; uint uVar10; undefined4 *puVar11; uint uVar12;
  uint in_fpscr; float extraout_s0; float extraout_s0_00; float fVar13;
  float extraout_s1; float extraout_s2;
  undefined8 local_88; undefined4 local_80; undefined4 local_48;
  undefined4 uStack_44; undefined4 local_40; int local_3c;

  fVar2 = DAT_000af47c;  fVar1 = DAT_000af470;
  piVar3 = *(int **)(DAT_000af480 + 0xaf0e6);
  local_3c = *piVar3;
  if ((param_1 != (Array *)0x0) &&
     ((this[0x356] == (PlayerEgo)0x0 || (2 < *(int *)(this + 0x1c4) - 1U)))) {
    this_00 = (Vector *)(this + 0xec);
    puVar4 = *(undefined4 **)(DAT_000af484 + 0xaf11e);
    piVar5 = *(int **)(DAT_000af488 + 0xaf132);
    for (uVar12 = 0; uVar12 < *(uint *)param_1; uVar12 = uVar12 + 1) {
      this_01 = *(KIPlayer **)(*(int *)(param_1 + 4) + uVar12 * 4);
      if (this_01 != (KIPlayer *)0x0) {
        if ((uVar12 == 0) && (iVar6 = Status::inAlienOrbit((Status *)*puVar4), iVar6 == 0)) {
          fVar7 = (float)AbyssEngine::AEMath::VectorLength((Vector *)(this + 0x148));
          in_fpscr = in_fpscr & 0xfffffff;
          if ((fVar7 < fVar1) && (this_01[0x71] != (KIPlayer)0x0)) {
            this[0x234] = (PlayerEgo)0x1;
          }
        }
        AEGeometry::getPosition();
        iVar6 = (**(code **)(*(int *)this_01 + 0x40))(this_01,(Vector *)&local_88);
        if (iVar6 != 0) {
          iVar6 = KIPlayer::getType(this_01);
          if ((iVar6 == 0x4262) && (iVar6 = KIPlayer::isVisible(this_01), iVar6 != 0)) {
            iVar6 = PlayerWormHole::isShrinking((PlayerWormHole *)this_01);
            if ((iVar6 == 0) && (*(int *)(this + 0x1e4) == 0)) {
              (**(code **)(*(int *)this_01 + 0x28))((Vector *)&local_88,this_01);
              AbyssEngine::AEMath::Vector::operator=(this_00,(Vector *)&local_88);
              AEGeometry::getPosition();
              AbyssEngine::AEMath::Vector::operator-=(this_00,(Vector *)&local_88);
              fVar7 = (float)AbyssEngine::AEMath::VectorLength(this_00);
              uVar10 = 40000 - (int)fVar7;
              iVar6 = FModSound::isPlaying(*piVar5);
              if ((int)uVar10 < 1) {
                if (iVar6 != 0) { FModSound::stop((FModSound *)*piVar5,0x22); }
              }
              else {
                if (iVar6 == 0) {
                  FModSound::play((FModSound *)*piVar5,0x22,(Vector *)0x0,(Vector *)0x0,extraout_s0);
                }
                else {
                  local_88 = 0; local_80 = 0;
                  (**(code **)(*(int *)this_01 + 0x28))(&local_48,this_01);
                  FModSound::updateEvent3DAttributes
                            ((FModSound *)*piVar5,0x22,(Vector *)&local_48,(Vector *)&local_88,false);
                }
                AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_88,this_00);
                AbyssEngine::AEMath::Vector::operator=(this_00,(Vector *)&local_88);
                local_88 = *(undefined8 *)this_00;
                local_80 = *(undefined4 *)(this + 0xf4);
                AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0xe0),(Vector *)&local_88);
                fVar13 = (float)VectorSignedToFloat(uVar10 >> 8,(byte)(in_fpscr >> 0x16) & 3);
                AbyssEngine::AEMath::Vector::operator*=((Vector *)(this + 0xe0),fVar13);
                AEGeometry::translate(*(Vector **)(this + 8));
                hitCamera(this);
                if ((int)fVar7 < 1000) { this[0x25] = (PlayerEgo)0x1; }
                puVar11 = *(undefined4 **)(DAT_000af48c + 0xaf41e);
                pMVar8 = (Matrix *)AbyssEngine::PaintCanvas::TransformGetLocal
                                   ((PaintCanvas *)*puVar11,*(uint *)(*(int *)(this + 8) + 0xc));
                pMVar9 = (Matrix *)AbyssEngine::PaintCanvas::TransformGetLocal
                                   ((PaintCanvas *)*puVar11,*(uint *)(*(int *)(this + 4) + 0xc));
                AbyssEngine::AEMath::operator*((AEMath *)&local_88,pMVar8,pMVar9);
                AbyssEngine::AEMath::Matrix::operator=((Matrix *)(*(int *)this + 4),(Matrix *)&local_88);
              }
            }
          }
          else if (this_01[0x3c] == (KIPlayer)0x0) {
            iVar6 = KIPlayer::isVisible(this_01);
            if (iVar6 != 0) {
              AEGeometry::getPosition();
              (**(code **)(*(int *)this_01 + 0x58))((Vector *)&local_48,this_01,(Vector *)&local_88);
              local_88 = 0; local_80 = 0;
              iVar6 = AbyssEngine::AEMath::operator!=((Vector *)&local_48,(Vector *)&local_88);
              if (iVar6 != 0) {
                AEGeometry::setPosition(*(Vector **)(this + 8));
                puVar11 = *(undefined4 **)(DAT_000af490 + 0xaf342);
                pMVar8 = (Matrix *)AbyssEngine::PaintCanvas::TransformGetLocal
                                   ((PaintCanvas *)*puVar11,*(uint *)(*(int *)(this + 8) + 0xc));
                pMVar9 = (Matrix *)AbyssEngine::PaintCanvas::TransformGetLocal
                                   ((PaintCanvas *)*puVar11,*(uint *)(*(int *)(this + 4) + 0xc));
                AbyssEngine::AEMath::operator*((AEMath *)&local_88,pMVar8,pMVar9);
                AbyssEngine::AEMath::Matrix::operator=((Matrix *)(*(int *)this + 4),(Matrix *)&local_88);
                if ((uVar12 == 0) && (this_01[0x71] != (KIPlayer)0x0)) {
                  this[0x234] = (PlayerEgo)0x1;
                }
              }
LAB_000af386:
              hitCamera(this);
            }
          }
          else {
            iVar6 = KIPlayer::isDying(this_01);
            if (((iVar6 == 0) && (iVar6 = KIPlayer::isDead(this_01), iVar6 == 0)) &&
               (((this[0x1c0] == (PlayerEgo)0x0 && (this[0x356] == (PlayerEgo)0x0)) ||
                (this_01 != *(KIPlayer **)(this + 0x1bc))))) {
              AEGeometry::getPosition();
              (**(code **)(*(int *)this_01 + 0x50))((Vector *)&local_88,this_01,(Vector *)&local_48);
              local_48 = 0; uStack_44 = 0; local_40 = 0;
              iVar6 = AbyssEngine::AEMath::operator!=((Vector *)&local_88,(Vector *)&local_48);
              if (iVar6 != 0) {
                fVar7 = (float)Player::setHitVector(*(Player **)(this_01 + 4),extraout_s0_00,
                                                    extraout_s1,extraout_s2);
                Player::setBombForce(*(Player **)(this_01 + 4),fVar7);
                Player::damage(*(Player **)(this_01 + 4),9999);
                Player::damage(*(Player **)this,0x14);
                if (this[0x398] != (PlayerEgo)0x0) {
                  *(float *)(*(int *)this + 0x60) = *(float *)(*(int *)this + 0x60) + fVar2;
                }
                goto LAB_000af386;
              }
            }
          }
        }
      }
    }
  }
  if (*piVar3 != local_3c) { __stack_chk_fail(); }
  return;
}
```

There is no `objects->size()` check inside the loop body — the only size
load is the loop-header test at `9f134`. The committed anchor is artificial.

The original's final guard is real and is `if (i == 0 && object->field_0x71)`
at `9f374`:

```
9f374: f1bb 0f00  cmp.w  fp, #0
9f378: d105       bne.n  9f386          ; i != 0 -> skip
9f37a: f898 0071  ldrb.w r0, [r8, #113] ; object->field_0x71
9f37e: b110       cbz    r0, 9f386
9f380: 2001       movs   r0, #1
9f382: f885 0234  strb.w r0, [r5, #564]
```

---

## 2. The remaining diff, enumerated exactly

Official verifier (`verify.score`, `autojunk=False`) on the committed build:

```
orig insns 347   ours 345   score 87.3
equal 302
replace blocks 33   (45 original lines / 43 ours)
insert blocks  3    (anchor 3 insns; a `str.w fp,[sp,#20]`; one `mov r1,sl`)
delete blocks  3    (preheader `str.w r9,[sp,#20]`; final `cmp.w fp,#0; bne.n`)
```

The 33 replace blocks collapse to **five conceptual differences**:

| # | diff | original | committed | # replace blocks |
|---|---|---|---|---|
| 1 | `dock`/`i` register swap | `add.w r9,r5,#328`; `mov.w fp,#0` | `add.w fp,r5,#328`; `mov.w r9,#0` | 22 |
| 2 | preheader dock spill | `str.w r9,[sp,#20]` at `9f12a` | — (spilled at the use sites instead) | 2 |
| 3 | artificial anchor | — | `ldr r0,[r4,#0]; cmp r0,r9; bls.w latch` at `9f190` | 1 insert |
| 4 | real final guard | `cmp.w fp,#0; bne.n 9f386` at `9f374` | — | 1 delete |
| 5 | collision-block shuffle | `mov sl,r5; mov r5,r6; add r6,sp,#96; … mov r6,r5; mov r5,sl; mov r1,r6` | `add.w sl,sp,#96; … mov r1,sl` | 1 |

Post-register-allocation table (matched from the preheader
`add.w X,r5,#328`, the `mov.w Y,#0` counter, the `add Z,sp,#32` surface temp
and the `add.w W,sp,#96` impact temp):

| value | original | committed | faithful (no anchor, real guard) |
|---|---|---|---|
| `this` | r5 | r5 | **sl** |
| `objects` | r4 | r4 | r4 |
| `object` | r8 | r8 | r8 |
| `dockOffsetVec` | **r9** | fp | r9 |
| `i` | **fp** | r9 | fp |
| `surface` temp (sp+32) | r6 | r6 | **r5** |
| impact temp (sp+96) | **sl** | sl | r9 |
| collision temp (sp+96) | r6 | sl | r6 |
| `&__stack_chk_guard` | `[sp,#24]` | `[sp,#24]` | **r6** |

So the committed build has *one* wrong pair (dock/i) and everything else
right. The faithful shape has the *right* dock/i pair and the right
collision temp, but `this`/`surface`/canary are permuted and the loop
rotates.

---

## 3. NEW: the real NDK clang MIR (not the local LLVM `-O3` model)

Passes 3–6 used a locally built `llc` (LLVM 7.0.0svn) with `-O3`. The project
builds with the NDK r18b clang 7.0.2 at **`-Oz`**, and the two differ. This
pass captured the real MIR by adding `-mllvm -print-after-all` to the actual
`compile_commands.json` command:

```sh
cd cmake-build-match
CMD=$(python3 -c "import json;print([e['command'] for e in json.load(open('compile_commands.json')) if 'PlayerEgo.cpp' in e['file']][0])")
CMD=${CMD/ -o CMakeFiles\/gof2.dir\/src\/game\/ship\/PlayerEgo.cpp.obj / -mllvm -print-after-all -o \/dev\/shm\/w4254\/ndk.o }
eval "$CMD" > /dev/shm/w4254/ndk_pa.log 2>&1
```

The `calcCollision` pass dumps are located by scanning for
`# *** IR Dump After <pass>` followed by
`# Machine code for function _ZN9PlayerEgo13calcCollision…`. For the
committed source the relevant dumps are at log lines 3065219 (Live Register
Matrix), 3065951 (Greedy), 3066724 (Virtual Register Rewriter); for the
faithful source, 3064722 / 3065453 / 3066240.

### 3.1 Committed shape — canary fully spilled

`Virtual Register Rewriter` (physical registers):

```
 72B   t2STRi12 renamable $r0, %stack.16       ; canary ptr: def in r0, spilled to [sp,#24]
 80B   %13 = t2LDRi12 renamable $r0, 0         ; load the canary value
384B   renamable $r11 = t2ADDri renamable $r5, 328   ; dock = fp, this = r5
544B   renamable $r6  = t2ADDri %stack.8, 0          ; surface = r6
720B   renamable $r9  = t2MOVi 0                     ; i = r9
8408B  renamable $r1  = t2LDRi12 %stack.16           ; canary ptr reloaded at exit
```

The canary pointer lives entirely in `%stack.16` (sp+24). `dock` is in `fp`,
`i` in `r9`, `surface` in `r6`, `this` in `r5` — exactly the committed
disassembly.

### 3.2 Faithful shape — canary split with r6

`Virtual Register Rewriter`:

```
384B   renamable $r9  = t2ADDri renamable $r10, 328  ; dock = r9, this = sl
544B   renamable $r5  = t2ADDri %stack.8, 0          ; surface = r5
720B   renamable $r11 = t2MOVi 0                     ; i = fp
728B   t2STRi12 renamable $r6, %stack.16             ; canary ptr: home r6, spilled at preheader
4760B  renamable $r6  = t2LDRi12 %stack.16           ; reload after Matrix::operator=
4808B  renamable $r6  = t2LDRi12 %stack.16           ; reload after the isDead call
7720B  renamable $r6  = t2LDRi12 %stack.16           ; reload after the Canvas load clobbers r6
```

`Live Register Matrix` intervals (same vreg numbers as the old model):

```
%12  canary  [64r, 8400B]     ; largest interval -> dequeued first
%34  surface [544r, 8000B]
%0   dock    [384r, 8304B]
%174 i       [720r, 8304B]
```

The faithful shape already has the **correct `dock = r9` / `i = fp` pair**.
The local LLVM decision log (`regalloc_fa.log`) shows the mechanism: the
canary is first assigned `$r11`, `i` evicts it from `$r11`, then `dock`
takes the free `$r9`. The only error is the canary's *second-round* split:
it chooses `$r6` over a stack slot.

### 3.3 The decisive numbers

`RAGreedy::tryRegionSplit` prints (local LLVM model, same `%12`/`%34` vreg
numbering; the NDK has no `-debug-only`):

```
Cost of isolating all blocks = 2.0            <-- SpillCost
$r6  static = 0.0, v=38, total = 1.448168993 with bundles EB#1..#5 EB#10..#13
$r9  static = 0.0, v=36, total = 1.448168993 with bundles EB#1..#5 EB#10..#13
Best split candidate of vreg %12 ... Split for $r6
```

`calcSpillCost()` = entry-block freq (1.0) + exit-block freq (1.0) = **2.0**.
The `$r6`/`$r9` region-split cost is the loop-body frequency, **1.448**,
because the loop's backedge probability estimate makes the body 1.448× the
entry. `1.448 < 2.0`, so `RS_Split` to `$r6` always beats `RS_Spill`; the
tie between `$r6` and `$r9` is broken by allocation-order position (r6
first), which is why the canary takes r6 and evicts `surface` to r5, which
in turn evicts `this` to `sl`.

**To spill the canary faithfully, the `$r6` region-split cost must exceed
2.0.** Nothing in the source controls the loop-body frequency directly; the
only lever is interference (more/hotter values live in r6 across the loop),
which is exactly what the artificial anchor provides.

### 3.4 Why the anchor flips it

The anchor adds a 50/50 branch block after `outerCollide` (base `bb.12`:
`%38 = load objects->count; cmp %38, %175; bcc latch`). That halves the
wormhole block's frequency, which halves `pull`'s spill weight
(`UseDefFreq / (Size + 25*InstrDist)` from 2.55e-4 to 1.52e-4), letting
`dock` evict `pull` from `r6`. `surface` then evicts `dock` to `fp`, and the
canary's split no longer finds a cheap `$r6` region (the local model prints
`$r6 no positive bundles`, `$r9 no positive bundles`, `$r10 total = 0.419`,
`$r11 total = 0.724` and picks `$r10`), after which the canary is spilled.
The anchor is therefore load-bearing for *both* the canary spill and the
`dock = fp` / `i = r9` swap.

---

## 4. Variant table (official `verify.score`; this pass)

Base = committed 87.3 / 968 / `this=r5 dock=fp i=r9 surf=r6 scratch=sl
canary=sp24`. "RA" is `this/dock/i/surf/object/canary` from
`analyze.py` (`?` = extractor could not match).

### 4.1 Faithful shapes — canary-spill triggers

| variant | pct | size | RA |
|---|---:|---:|---|
| faithful (no anchor, real guard) | 59.8 | 996 | sl/r9/fp/r5/r8/r6 |
| faithful + `outerCollide(getPosition())` inline temp | 59.8 | 996 | sl/r9/fp/r5/r8/r6 |
| faithful + `Vector surface;` before the null check, assign after top | 58.4 | 984 | r5/r0/fp/r6/r9/sp24 |
| faithful + `Vector surface;` at function scope | 44.7 | 1028 | r8/fp/sl/?/r9/sp24 |
| faithful + named matrix ptr/ref (both blocks) | 69.4 | 980 | r5/r0/fp/r9/r8/sp24 |
| faithful + named matrix **collision block only** | 69.4 | 980 | r5/r0/fp/r9/r8/sp24 |
| faithful + named `AEGeometry *geom` in collision block | 69.4 | 980 | r5/r0/fp/r9/r8/sp24 |
| faithful + named `Vector zero` in collision block | 63.6 | 980 | r5/r0/fp/?/r8/sp24 |
| faithful + `collision != Vector{0,0,0}` | 60.1 | 996 | sl/r9/fp/r5/r8/r6 |
| faithful + surface unscoped | 64.1 | 968 | r5/sl/fp/r8/r9/r6 |
| faithful + `size_t i` / `i += 1` / nested top block | 59.8 | 996 | sl/r9/fp/r5/r8/r6 |
| faithful + guard order `field_0x71 && i == 0` | 59.8 | 996 | sl/r9/fp/r5/r8/r6 |

Note the recurring `r5/r0/fp/r9/r8/sp24`: naming *any* pointer or temporary
in the collision block spills the canary and keeps `this=r5`/`i=fp`, but
`r6` is then taken by the hoisted `movw r6,#16994` constant, so `surface`
lands in `r9` and `dock` is demoted to `r0`.

### 4.2 Committed-shape micro-variants

| variant | pct | size | RA |
|---|---:|---:|---|
| committed | **87.3** | 968 | r5/fp/r9/r6/r8/sp24 |
| committed + `outerCollide(getPosition())` inline temp | 87.3 | 968 | r5/fp/r9/r6/r8/sp24 |
| committed + named matrix ptr (both / first / second) | 87.3 | 968 | r5/fp/r9/r6/r8/sp24 |
| committed + named matrix `M_TWO` form | 86.4 | 968 | r5/fp/r9/r6/r8/sp24 |
| committed + `collision != Vector{0,0,0}` | 87.0 | 968 | r5/fp/r9/r6/r8/sp24 |
| committed + anchor form `i >= objects->size()` | 87.3 | 968 | r5/fp/r9/r6/r8/sp24 |
| committed + anchor form `objects->size() == i` | 79.5 | 960 | r5/fp/r9/r6/r8/sp24 |
| committed + anchor form `i + 1 > objects->size()` | 79.1 | 964 | r5/fp/r9/r6/r8/sp24 |
| committed + real guard | 79.4 | 976 | r5/fp/r9/r6/r8/sp24 |
| committed + surface unscoped | 72.2 | 1000 | r5/r9/fp/sl/r8/sp24 |
| committed + surface before top | 73.2 | 976 | r5/fp/sl/r6/r8/sp24 |
| committed + surface before top + named matrix | 74.9 | 972 | r5/r9/sl/r6/r8/sp24 |
| committed + named Canvas in matrix block | 73.5 | 1000 | r5/fp/sl/r6/r8/? |
| committed + named `Vector zero` | 78.0 | 968 | r5/fp/r9/?/r8/sp24 |

`committed + surface before top + named matrix` is the closest
near-miss: `this=r5, dock=r9, surface=r6, object=r8, canary=sp24` are all
correct; only `i` is `sl` instead of `fp` (fp is free). It is still only
74.9 % because moving `getPosition()` before the top block reorders ~15
instructions and the named matrix adds two moves.

### 4.3 Synthetic score components (pass 5, re-used)

| edit | score |
|---|---:|
| committed (baseline) | 87.3 |
| swap `fp`↔`r9` only | **92.2** |
| collision-block shuffle only | 88.6 |
| swap + collision-block shuffle | **93.5** |

The swap is worth ≈5 points and is the only lever big enough to matter.

---

## 5. What this pass proved / what remains

**Proved:**

1. The real NDK `-Oz` MIR confirms the pass-6 model: in the faithful shape
   `dock = r9` and `i = fp` are already correct, and the *only* wrong value
   is the canary pointer, which is split with `$r6` as its home register
   instead of being spilled to `[sp,#24]`.
2. The canary's `RS_Split` decision is a numeric comparison:
   `$r6` region cost **1.448** vs `RS_Spill` cost **2.0**. The loop-body
   frequency (1.448) is below the entry+exit sum (2.0), so a register split
   always wins unless interference raises the region cost.
3. Naming any pointer/temporary in the collision block (matrix pointer,
   `AEGeometry *`, `Vector zero`) *does* spill the canary while keeping
   `this=r5` / `i=fp` / `object=r8`, but the freed `r6` is then taken by the
   hoisted `movw r6,#16994` constant, so `surface` moves to `r9` and `dock`
   is demoted to `r0`. The constant is rematerialized inside the loop in the
   original precisely because `r6` is occupied by the surface pointer.
4. No faithful shape tested this pass produces the exact original RA
   (`dock=r9, i=fp, surface=r6, this=r5, object=r8, canary=sp24`) without
   also changing the instruction stream.

**Remaining (the win condition):** a faithful (anchor-free) shape in which
the canary's `$r6` region-split cost exceeds 2.0 **without** adding a named
temporary that changes the code. The only source-level lever left is
register pressure in the hot loop that does not add instructions — e.g. a
different `Vector surface` lifetime that makes the surface pointer compete
for `r6` earlier, or a `Globals::Canvas`/`Globals::status` access shape that
raises the canary's interference. The loop-body frequency itself is a
compiler heuristic and is not source-addressable.

**Do not land** any of the 59–79 % variants: they are strictly worse than
the committed 87.3.

---

## 6. Reproduction

```sh
# gate
cmake --build cmake-build-match --target check -j8        # RATCHET PASS

# one-function score / RA
python3 /dev/shm/w4254/score.py        # pct / size / insns
python3 /dev/shm/w4254/analyze.py      # insns + this/dock/i/surf/object/canary

# real NDK MIR (~65 s, 280 MB log)
cd cmake-build-match
CMD=$(python3 -c "import json;print([e['command'] for e in json.load(open('compile_commands.json')) if 'PlayerEgo.cpp' in e['file']][0])")
CMD=${CMD/ -o CMakeFiles\/gof2.dir\/src\/game\/ship\/PlayerEgo.cpp.obj / -mllvm -print-after-all -o \/dev\/shm\/w4254\/ndk.o }
eval "$CMD" > /dev/shm/w4254/ndk_pa.log 2>&1
# locate the calcCollision dumps:
python3 - <<'PY'
t='_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE'
cur=None
for i,l in enumerate(open('/dev/shm/w4254/ndk_pa.log',errors='replace'),1):
    if l.startswith('# *** IR Dump After'): cur=l.strip()
    if 'Machine code for function '+t in l and ('Live Register Matrix' in cur or 'Greedy' in cur or 'Virtual Register Rewriter' in cur):
        print(i,cur)
PY

# official verifier
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE
```

Gate at this commit (unchanged): `byte_exact 4499 / linked_exact 4499 /
size-exact 4524`, `stub_zero_size 0`, `missing 0`, `wrong_type 0`,
`extra 0`, `data_exact 843/843`, imports 207, `.text 0x1337c8`,
`.rodata 0x41b67`, `.ARM.exidx 0x6f70` exact, `.ARM.extab 0x26290` (−8),
lint `CLEAN ✓`, drift 0 (`RATCHET PASS`).
