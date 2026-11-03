# `RocketGun::update(int)` — pass 8 (w4412, wave 94)

**Status:** research note, 2026-11-03. Worktree `wt/w4412`, branch `wave94-w4412`,
base `f262bd4b`. **No source change landed**: every faithful source shape tried
this pass scores <= 86.9 %. Tree clean, gate unchanged.

Target `_ZN9RocketGun6updateEi`: original **1908 B / 635 insns**, ours
**1908 B / 636 insns**, official verify score **86.9**.

Earlier notes: `rocketgun-update-w4265.md` (pass 7), `rocketgun-update-w4248.md`
(pass 6), `rocketgun-update-w4216.md` (pass 5), `rocketgun-update-w4111.md`
(pass 4), `rocketgun-update-w4011.md` (pass 3), `rocketgun-update-w3831.md`
(pass 2), `rocketgun-update-86-9-1908-b_w3811.md`.

This pass contributes (a) the **full Ghidra decompilation** of the original
(inlined in the appendix), which confirms the source shape and, in particular,
that the original has **no `trailMatrices` local and no anchors**; (b) a
**register-normalised instruction diff** that isolates the *only* structural
(non-register) differences; (c) the observation that the **`s` spill is the
root** of the frame-slot/`strd` residual, and that it is itself downstream of
which register the `VectorCross` sret pointer takes; and (d) a ~120-variant
grid this pass (anchor × type × position × declaration order × cluster-block
shape × loop shape × scheduler flags). Nothing moves the score.

---

## 1. Baseline (NDK r18b clang 7.0.2, this worktree)

```
cmake --build cmake-build-match --target check -j8
```
```
verify: byte 4499  linked 4499  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`_ZN9RocketGun6updateEi`: 1908 B both sides, **635 vs 636** instructions,
**86.9 %**. Every other `RocketGun` function is byte-exact, so the TU's source
style is already faithful; only `update` is off.

Fast oracle used this pass (in `/dev/shm/w4412`, not in the repo):
`cc.sh` = the exact NDK compile of `RocketGun.cpp`; `try.sh` = copy variant into
`src/`, `ninja link_so`, then `fscore.py` (imports `tools/verify/verify.py` and
scores just this symbol with the official `disassemble`/`reconcile`/`score`
machinery, ~1.5 s/variant). `slots.py` classifies the entry `mov`s and the
launch-block spill slot. All scratch is under `/dev/shm/w4412`.

---

## 2. Register-normalised structural diff (new)

Disassembling both functions and replacing every register name with `R` and
every immediate with `#I`, the *only* structural (non-register) differences
are, in address order:

| # | original | ours | meaning |
|---|---|---|---|
| 1 | `strd r4, sl, [sp,#16]` | `str r5,[sp,#12]` + `str.w sl,[sp,#20]` | elapsed+this spill; no post-RA merge |
| 2 | — | `str r4,[sp,#28]` | `volatile int padx` anchor (launch branch) |
| 3 | — | `str r5,[sp,#28]` + `ldr [sl,#216]` + `str [sp,#16]` | `volatile int unused` + `trailMatrices` local |
| 4 | `str r0,[sp,#8]` after `Sinf` | `mov r6,r0` after `Sinf` | **original spills `s`, ours keeps it in `r6`** |
| 5 | `ldr r4,[sp,#24]` at `15f256` (before `Sinf`) | `ldr r4,[sp,#24]` at `15f2d8` (after `Cosf`) | `dt` spill-reload placement |
| 6 | `mov r9,r4` … `mov r4,r9` | — | cursor `i*60` in `r4` (caller-saved), saved across cluster calls |
| 7 | `adds r4,#60` | `add.w r9,r9,#60` | cursor register |
| 8 | `ldr [sl,#216]` ×5 in loop | `ldr [sp,#16]` ×3 + `ldr [sl,#216]` ×2 | trailMatrices rematerialised vs cached |
| 9 | `nop` before literal pool | — | alignment |

Everything else is a register rename or a branch-offset shift. There is no
block-order, scheduling, or semantic difference.

Items 1, 4, 5, 6, 7, 8 are all consequences of one colouring permutation:
`elapsed` is `r4` in the original and `r5` in ours.

---

## 3. The `s` spill is the frame-slot root (new emphasis)

Frame slots (confirmed from the disassembly):

| slot | original | ours |
|---|---|---|
| `[sp,#4]` | stack-guard copy | stack-guard copy |
| `[sp,#8]` | **`s` (`Sinf` result)** | `c` (`Cosf` result) |
| `[sp,#12]` | `c` (`Cosf` result) | **`elapsed`** |
| `[sp,#16]` | **`elapsed`** | `trailMatrices` local |
| `[sp,#20]` | `this` | `this` |
| `[sp,#24]` | `dt` | `dt` |
| `[sp,#28]` | — | `unused` / `padx` anchors |
| `[sp,#144]` | `ahead` | `ahead` |
| `[sp,#156]` | stack-guard | stack-guard |

The original spills **both** trig results:

```
15f274:  str r0, [sp, #8]     ; s = Sinf(...)
15f29c:  str r0, [sp, #12]    ; c = Cosf(...)
...
15f2be:  ldr r0, [sp, #8]     ; reload s for (s+s)*dt
15f2f4:  ldr r0, [sp, #12]    ; reload c for (c+c)*dt
```

Ours keeps `s` in `r6` and spills only `c`:

```
        blx  Sinf
        mov  r6, r0            ; s stays in r6
        ...
        blx  Cosf
        str  r0, [sp, #8]      ; c -> [sp,#8]
```

This is the direct cause of the slot shift: with `s` spilled, `c` is forced to
`[sp,#12]`, so `elapsed` lands on `[sp,#16]` next to `this` on `[sp,#20]`, and
the post-RA `ARMLoadStoreOptimizer` merges the two adjacent `STRi12`s into one
`strd r4, sl, [sp,#16]` (635 insns). Ours has `c` on 8, `elapsed` on 12 and
`trailMatrices` on 16, so the two stores are not adjacent and cannot merge
(636 insns).

### 3.1 Why `s` spills in the original but not in ours

In the original, `r6` holds the `lifetimes` **base pointer** across the `Sinf`
call:

```
15f242:  ldr  r6, [r0, #60]        ; r6 = gun->lifetimes
15f248:  blx  __aeabi_uidiv
15f24c:  ldr  r1, [r6, r8, lsl #2] ; life = lifetimes[i]
...
15f270:  blx  Sinf
15f274:  str  r0, [sp, #8]         ; s spilled
15f276:  ldr  r0, [sl, #8]         ; gun
15f27a:  ldr  r1, [r0, #60]        ; reload lifetimes base
15f27c:  ldr  r0, [r0, #68]        ; total
15f27e:  ldr  r1, [r1, r8, lsl #2] ; life2
```

`r6` is then reused as the `VectorCross` sret pointer at `15f29e`
(`add r6, sp, #128`), which is *before* `s`'s use at `15f2be`. So `s` cannot
stay in `r6`; the allocator spills it at its definition.

In ours, `r6` holds `life` (not the base) and is dead before `Sinf`; the
`VectorCross` sret pointer takes `r4` (`add r4, sp, #40` at `15f29c`), `r6`
stays free, and `s` stays in `r6`.

So the chain is:

```
s spill  <-  VectorCross sret pointer = r6  <-  r4 busy with dt
         <-  dt reload placed before Sinf  <-  RA/scheduler decision
```

and that last decision is itself downstream of the `elapsed` colouring. It is
a closed loop: `elapsed` -> `r4` frees `r4` for the cursor -> `dt` can be
reloaded late -> `VectorCross` takes `r4` -> `s` stays in `r6` -> `elapsed`
slot is 12 not 16 -> no `strd`.

### 3.2 `dt` reload placement

Original (`15f256`, in the cluster block before `Sinf`):

```
15f24c:  ldr  r1, [r6, r8, lsl #2]  ; life
15f250:  vmov s2, r0                ; phase
15f254:  subs r1, r4, r1            ; total - life
15f256:  ldr  r4, [sp, #24]         ; dt  <-- reloaded here
15f258:  vmov s0, r1
...
15f270:  blx  Sinf
```

Ours (`15f2d8`, after `Cosf`/`VectorNormalize`):

```
15f296:  blx  Cosf
15f29a:  str  r0, [sp, #8]
...
15f2d8:  ldr  r4, [sp, #24]         ; dt  <-- reloaded here
15f2da:  add  r5, sp, #128
```

Both are genuine spill reloads; only the placement differs.

---

## 4. Ghidra decompilation (new, confirms the source shape)

The original was decompiled from the analysed project
(`..._work/bins/android_2.0.16_libgof2hdaa.so_ghidra`, function `update` at
`0x16ecd8`, image base `0x10000`). The key confirmations:

* `trailMatrices` is read directly from the member every time:
  `*(int *)(this + 0xd8)` appears at `15ef02`, `15f1d8`, `15f216`, `15f32c`,
  `15f370`, `15f39a` — **six** loads, no local. Our source has a local and
  only four member loads (two member + three local).
* The loop is a `do { ... } while (true)` with the bound check at the top
  (`if (*(uint *)(gun + 8) <= i) { stack-check; return; }`); this is exactly
  what the `for` compiles to.
* `dt` is `fVar18 = (float)param_1`, computed once before the loop; the
  cluster block multiplies by `fVar18`, not by a re-converted value.
* The loop fade is `iVar12 - param_1` (`trailTimers[i] - elapsed`), i.e. the
  source uses `elapsed` directly.
* `ahead` is `local_50`/`uStack_4c`/`local_48`, initialised to `{0,0,0}` at
  function entry and re-assigned in the loop via `Vector::operator=`.
* There is no `unused`/`padx` local and no second scalar at `[sp,#28]`.

The full decompilation is inlined in the appendix so this note is
self-contained.

---

## 5. Variant grid tried this pass (~120 variants)

All measured with the official verify machinery. The baseline is **86.9**
(635/636). The complete list of *new* shapes that did not improve:

| variant | pct | note |
|---|---|---|
| baseline | **86.9** | 636 insns, `elapsed=r5@12`, `this=sl@20`, cursor `r9` |
| `unused` × {volatile int/unsigned/long/float/short/char, int} × 5 positions × {trail local, no trail local} | 86.9 / 57.6 | 70-variant grid; with the trail local always 86.9; without it `elapsed=r4` only when `unused` is before the launch `if`, but `this=fp` and cursor `r0` |
| declaration order of `dt`/`unused`/`trailMatrices` (all 6 permutations + 2 without `unused`) | 86.9 | slots identical (`elapsed@12`, `this@20`) |
| loop-fade value: `const int&`, `const int*`, `int[1]`, `int[2..4]`, one-field struct, two/three-field struct | 86.9 (refs/ptr/first) / 67.0 (arrays/structs) | arrays force `this` out of `sl` |
| `volatile int frameTime = elapsed` + loop fade (5 positions) | 48.2–53.7 | adds a store+load, 1876–1924 B |
| `int frameTime = elapsed` + loop fade (5 positions) | 85.8–86.9 | coalesces |
| `frameTime` used for `dt` too | 86.9 | coalesces |
| `Gun *g2` / `gun->` / `this->gun->` in the cluster block | 54.5 / 86.9 / 86.9 | |
| `int *lifetimes = gun->lifetimes` + both trig args | 85.8 | `s` -> `sl`, not spilled |
| `total - gun->lifetimes[i]` for `Cosf` | 85.8 | |
| `total - life` for `Cosf` | 53.2 | |
| `total - this->gun->lifetimes[i]` for `Cosf` | 85.8 | |
| shared `arg` for both `Sinf` and `Cosf` | 53.2 | |
| shared `arg` for `Sinf`, separate for `Cosf` | 86.9 | |
| `cross`/`norm` hoisted before `Sinf` | 53.9 / 57.1 / 67.0 | |
| `Vector cross` at function scope | 60.7 / 59.5 | |
| `Vector &pos` / `Vector *pos` for the two `+=` | 59.4 | |
| `Vector norm` local for `VectorNormalize` | 85.1 | |
| `const Vector &upv` local | 61.2 | |
| `s`/`c` declared at function scope, assigned in loop | 86.9 | `s` still in `r6` |
| `s`/`c` non-const | 86.9 | |
| `dt` non-const | 86.9 | |
| `dt` volatile / `volatile float` | 53.6 | |
| `dt2 = dt` / `dt*1.0f` / `dt+0.0f` used in the block | 86.9 | coalesces/folds |
| `(float)elapsed` in the cluster block | 86.9 | CSEs to `dt` |
| `phase + (float)(elapsed-elapsed)` | 86.9 | folds |
| `do { ... } while(true)` with explicit bound check | 86.7 | |
| `ahead` initialised member-by-member | 86.9 | |
| `i` as `int` / `unsigned` | 86.9 | |
| `count` cached in a local | 74.9 | 1916 B |
| positions base cached in a local | 44.7 | 1832 B |
| `__builtin_expect` split forms (`e1`/`e2`/`e3`) | 84.6 / 84.6 / 86.9 | |
| `__builtin_expect(...,0)` | 70.3 | 1912 B |
| `volatile int dummy8` at top / after `Gun&` / before launch / before fade | 85.9 / 81.1 / 85.4 / 74.3 | |
| `volatile int dummy8` + no trail local | 57.7 | |
| extra cached-member locals (`trailTimers`, `trailSystems`, `particleManager`, `gun`) | 86.9 | optimised away |

Per-TU scheduler flags (NDK clang 7.0.2, applied only to this TU):

| flag | insns | elapsed/this/cursor | pct |
|---|---|---|---|
| default | 636 | `r5`/`sl`/`r9` | **86.9** |
| `-mllvm -pre-RA-sched=list-ilp` | 635 | `r5`/`sl`/`r9` | 67.0 |
| `-mllvm -pre-RA-sched=list-hybrid` | 636 | `r5`/`sl`/`r9` | 86.9 |
| `-mllvm -pre-RA-sched=source` | 638 | `r4`/`fp`/`?` | — |
| `-mllvm -pre-RA-sched=list-burr` | 638 | `r5`/`sl`/`?` | — |
| `-mllvm -enable-misched=false` | 636 | `r5`/`sl`/`r9` | — |
| `-mllvm -misched-postra=false` | 636 | `r5`/`sl`/`r9` | — |
| `-mllvm -regalloc=pbqp` | 640 | `?`/`sl`/`?` | — |
| `-mllvm -regalloc=basic` | 633 | `?`/`r9`/`?` | — |
| `-mllvm -no-stack-coloring` | 637 | `r9@20`/`r5`/`?` | — |

The `list-ilp` result is interesting: it matches the original's **635**
instruction count but with the wrong colouring (67.0 %), confirming that the
instruction count alone is not the target.

---

## 6. What remains

The residual is one `RegAllocGreedy::shouldEvict` comparison plus its
downstream frame-slot/`strd`/`s`-spill consequences:

1. `%29` (`elapsed`, size 14894, weight 9.40e-4) is allocated first and takes
   `r4`.
2. `%251` (hoisted `t2ADDri %stack.4, 0`, weight 8.57e-3) evicts it.
3. `%29` is split; the pre-loop part ends up in `r5` (ours) rather than `r4`
   (original).
4. Because `elapsed` is `r5`, `r4` is taken by `player` and later by the
   `VectorCross` sret pointer; `s` stays in `r6`; `c` lands on `[sp,#8]`,
   `elapsed` on `[sp,#12]`, `trailMatrices` on `[sp,#16]`; the `elapsed`/`this`
   stores are not adjacent and no `strd` merge happens (636 vs 635 insns).

To match, a source shape must do **all** of:

1. keep `elapsed` in `r4` through the launch block (the pre-spill register
   interval ends at `15edea`, so it must not be evicted by `%251`);
2. keep `this` in `sl` (r10), which currently needs the `trailMatrices` local
   (removing it moves `this` to `fp` and drops to 57.6 %);
3. force `s` to spill so `c`/`elapsed`/`this` land on `12`/`16`/`20` and the
   post-RA `strd` fires;
4. keep the 160-byte frame.

(1) and (2) currently pull in opposite directions: the `trailMatrices` local
that puts `this` in `sl` also raises preheader pressure and moves `elapsed` to
`r5`. (3) is downstream of (1): with `elapsed` in `r4`, `r4` is free for the
`VectorCross` sret pointer, `s` can stay in `r6`, and the `strd` does not fire.

No source-level shape found in ~120 variants this pass (plus ~270 across
passes 2–7) breaks this fixed point. The remaining lever is the exact pre-RA IR shape that makes the
split of `%29` choose `r4` (the split-candidate analysis at `RS_Split
Cascade 4` scores `$r4 static = 0.0, total = 96.7` vs `$r10 total = 8.5`), and
that is not reachable by reordering/renaming the existing expressions.

---

## 7. Reproduction

```
# gate
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8

# side-by-side
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN9RocketGun6updateEi

# fast single-function score (scratch)
python3 /dev/shm/w4412/fscore.py _ZN9RocketGun6updateEi
```

Scratch lives in `/dev/shm/w4412`: `base.cpp`, `cc.sh`, `try.sh`,
`fscore.py`, `slots.py`, `classify.py`, `gen_*.py`, `v*.cpp`, `grid/`, `ord/`,
`orig.asm`, `ours.asm`, `ghidra_update.txt`, `ra_base.log`; none of it is in
the repo.

---

## Appendix: Ghidra decompilation of `RocketGun::update` (0x16ecd8)

```c
void __thiscall RocketGun::update(RocketGun *this,int param_1)
{
  float fVar1;
  int *piVar2;
  int *piVar3;
  int iVar4;
  Matrix *pMVar5;
  undefined8 *puVar6;
  undefined4 uVar7;
  ParticleSystemManager *pPVar8;
  int iVar9;
  int iVar10;
  bool bVar11;
  AEGeometry *this_00;
  int iVar12;
  int iVar13;
  code *pcVar14;
  uint uVar15;
  undefined4 *puVar16;
  uint in_fpscr;
  float fVar17;
  float fVar18;
  float fVar19;
  float fVar20;
  float extraout_s1;
  float extraout_s1_00;
  float extraout_s1_01;
  float extraout_s1_02;
  float extraout_s2;
  float extraout_s2_00;
  float extraout_s2_01;
  float fVar21;
  AEMath aAStack_c4 [12];
  undefined4 local_b8;
  undefined4 uStack_b4;
  undefined4 local_b0;
  undefined4 local_a8;
  undefined4 local_a4;
  undefined4 uStack_a0;
  undefined4 local_98;
  undefined4 local_94;
  float fStack_90;
  undefined4 local_78;
  undefined4 local_74;
  undefined4 local_70;
  undefined4 local_6c;
  undefined4 uStack_68;
  undefined4 local_64;
  undefined8 local_60;
  float local_58;
  undefined4 local_50;
  undefined4 uStack_4c;
  float local_48;
  int local_44;

  piVar2 = *(int **)(DAT_0016f044 + 0x16ecf0);
  local_44 = *piVar2;
  local_50 = 0;
  uStack_4c = 0;
  local_48 = 0.0;
  Gun::update(*(Gun **)(this + 8),param_1);
  if ((this[0x1c] != (RocketGun)0x0) && (*(char *)(*(int *)(this + 8) + 0xa9) != '\0')) {
    piVar3 = (int *)Level::getPlayer(*(Level **)(this + 0xc));
    PlayerEgo::getPosition();
    local_60 = *(undefined8 *)(*(int *)(this + 8) + 0x7c);
    local_58 = *(float *)(*(int *)(this + 8) + 0x84) + DAT_0016f048;
    AbyssEngine::AEMath::MatrixRotateVector
              ((AEMath *)&local_6c,(Matrix *)(*piVar3 + 4),(Vector *)&local_60);
    AbyssEngine::AEMath::Vector::operator+=((Vector *)&local_b8,(Vector *)&local_6c);
    AEGeometry::setPosition(*(Vector **)(this + 0x18));
    fVar17 = (float)VectorSignedToFloat(*(undefined4 *)(*(int *)(this + 8) + 0x70),
                                        (byte)(in_fpscr >> 0x16) & 3);
    fVar17 = (float)VectorSignedToFloat((int)((fVar17 / DAT_0016f04c + 1.0) * DAT_0016f050),
                                        (byte)(in_fpscr >> 0x16) & 3);
    AEGeometry::setScaling
              (*(AEGeometry **)(this + 0x18),fVar17 / DAT_0016f050,extraout_s1,DAT_0016f050);
    this_00 = *(AEGeometry **)(this + 0x18);
    AbyssEngine::AEMath::MatrixGetDir((AEMath *)&local_6c,(Matrix *)(*piVar3 + 4));
    local_74 = 0x3f800000;
    local_78 = 0;
    local_70 = 0;
    AEGeometry::setDirection(this_00,(Vector *)&local_6c,(Vector *)&local_78);
  }
  iVar4 = *(int *)(this + 8);
  this[0x1d] = *(RocketGun *)(iVar4 + 0xa9);
  iVar10 = DAT_0016f054;
  if (*(char *)(iVar4 + 0x4d) != '\0') {
    *(undefined4 *)(this + 0xc4) = 0xffffffff;
    *(undefined4 *)(this + 0xd4) = 0;
    *(undefined1 *)(iVar4 + 0x4d) = 0;
    puVar16 = *(undefined4 **)(iVar10 + 0x16ee04);
    pMVar5 = (Matrix *)
             AbyssEngine::PaintCanvas::TransformGetLocal
                       ((PaintCanvas *)*puVar16,*(uint *)(this + 0x10));
    AbyssEngine::AEMath::MatrixSetTranslation
              ((AEMath *)&local_b8,pMVar5,
               *(float *)(*(int *)(*(int *)(this + 8) + 0xc) +
                          *(int *)(*(int *)(this + 8) + 0xa0) * 0xc + 8),extraout_s1_00,extraout_s2)
    ;
    puVar6 = (undefined8 *)
             (*(int *)(*(int *)(this + 8) + 0x18) + *(int *)(*(int *)(this + 8) + 0xa0) * 0xc);
    local_60 = *puVar6;
    local_58 = *(float *)(puVar6 + 1);
    uVar7 = AbyssEngine::PaintCanvas::TransformGetLocal
                      ((PaintCanvas *)*puVar16,*(uint *)(this + 0x10));
    __aeabi_memcpy((Matrix *)&local_b8,uVar7,0x3c);
    local_6c = 0;
    uStack_68 = 0x3f800000;
    local_64 = 0;
    AbyssEngine::AEMath::VectorCross((AEMath *)&local_78,(Vector *)&local_6c,(Vector *)&local_60);
    AbyssEngine::AEMath::VectorNormalize(aAStack_c4,(Vector *)&local_78);
    pcVar14 = *(code **)(DAT_0016f058 + 0x16eea8);
    (*pcVar14)((AEMath *)&local_78,aAStack_c4);
    AbyssEngine::AEMath::VectorCross(aAStack_c4,(Vector *)&local_60,(Vector *)&local_78);
    (*pcVar14)((Vector *)&local_6c,aAStack_c4);
    AbyssEngine::AEMath::VectorNormalize(aAStack_c4,(Vector *)&local_6c);
    (*pcVar14)((Vector *)&local_6c,aAStack_c4);
    local_a8 = local_74;
    local_98 = local_70;
    local_b8 = local_78;
    uStack_b4 = local_6c;
    local_b0 = (undefined4)local_60;
    local_a4 = uStack_68;
    uStack_a0 = local_60._4_4_;
    local_94 = local_64;
    fStack_90 = local_58;
    AbyssEngine::PaintCanvas::TransformSetLocal
              ((PaintCanvas *)*puVar16,*(uint *)(this + 0x10),(Matrix *)&local_b8);
    if (*(int *)(this + 0xd8) == 0) {
      if ((*(int *)(this + 0xd0) - 4U < 2) || (*(int *)(this + 0xd0) == 0x28)) {
        ParticleSystemManager::enableSystemRender
                  (*(ParticleSystemManager **)(**(int **)(this + 0xb0) + 0x80),*(int *)(this + 0xcc)
                   ,true);
        ParticleSystemManager::enableSystemEmit
                  (*(ParticleSystemManager **)(**(int **)(this + 0xb0) + 0x80),*(int *)(this + 0xcc)
                   ,true);
      }
      else {
        if (*(int *)(*(int *)(this + 8) + 0x58) == 0xe8) {
          iVar10 = *(int *)(this + 0xcc);
          bVar11 = true;
          pPVar8 = *(ParticleSystemManager **)(**(int **)(this + 0xb0) + 0x9c);
        }
        else {
          iVar10 = *(int *)(this + 0xcc);
          pPVar8 = *(ParticleSystemManager **)(**(int **)(this + 0xb0) + 0x84);
          bVar11 = false;
          if (*(int *)(*(int *)(this + 8) + 0x58) != 0xb3) {
            bVar11 = true;
          }
        }
        ParticleSystemManager::enableSystemEmit(pPVar8,iVar10,bVar11);
      }
    }
    else {
      iVar10 = *(int *)(*(int *)(this + 8) + 0xa0);
      in_fpscr = in_fpscr & 0xfffffff |
                 (uint)(*(float *)(*(int *)(*(int *)(this + 8) + 0xc) + iVar10 * 0xc + 8) ==
                       DAT_0016f05c) << 0x1e;
      if ((byte)(in_fpscr >> 0x1e) == 0) {
        AbyssEngine::AEMath::Matrix::operator=
                  ((Matrix *)(*(int *)(*(int *)(this + 0xd8) + 4) + iVar10 * 0x3c),
                   (Matrix *)&local_b8);
        ParticleSystemManager::resetSystem
                  (*(ParticleSystemManager **)(this + 0xe4),
                   *(int *)(*(int *)(*(int *)(this + 0xdc) + 4) +
                           *(int *)(*(int *)(this + 8) + 0xa0) * 4));
        ParticleSystemManager::enableSystemEmit
                  (*(ParticleSystemManager **)(this + 0xe4),
                   *(int *)(*(int *)(*(int *)(this + 0xdc) + 4) +
                           *(int *)(*(int *)(this + 8) + 0xa0) * 4),true);
        ParticleSystemManager::enableSystemRender
                  (*(ParticleSystemManager **)(this + 0xe4),
                   *(int *)(*(int *)(*(int *)(this + 0xdc) + 4) +
                           *(int *)(*(int *)(this + 8) + 0xa0) * 4),true);
        *(undefined4 *)
         (*(int *)(*(int *)(this + 0xe0) + 4) + *(int *)(*(int *)(this + 8) + 0xa0) * 4) = 0;
      }
      else {
        ParticleSystemManager::enableSystemEmit
                  (*(ParticleSystemManager **)(this + 0xe4),
                   *(int *)(*(int *)(*(int *)(this + 0xdc) + 4) + iVar10 * 4),false);
        ParticleSystemManager::resetSystem
                  (*(ParticleSystemManager **)(this + 0xe4),
                   *(int *)(*(int *)(*(int *)(this + 0xdc) + 4) +
                           *(int *)(*(int *)(this + 8) + 0xa0) * 4));
        *(undefined4 *)
         (*(int *)(*(int *)(this + 0xe0) + 4) + *(int *)(*(int *)(this + 8) + 0xa0) * 4) = 0;
      }
    }
    iVar4 = *(int *)(this + 8);
    if ((*(int *)(iVar4 + 0x5c) == 0x28) && (0 < *(int *)(iVar4 + 0xa0))) {
      *(undefined1 *)(iVar4 + 0x4d) = 1;
      *(int *)(iVar4 + 0xa0) = *(int *)(iVar4 + 0xa0) + -1;
      (**(code **)(*(int *)this + 0x10))(this,param_1);
      Gun::update(*(Gun **)(this + 8),param_1);
      iVar4 = *(int *)(this + 8);
    }
  }
  if (((*(char *)(iVar4 + 0x4c) == '\0') || (*(int *)(iVar4 + 8) != 1)) ||
     (in_fpscr = in_fpscr & 0xfffffff |
                 (uint)(*(float *)(*(int *)(iVar4 + 0xc) + 8) == DAT_0016f440) << 0x1e,
     (byte)(in_fpscr >> 0x1e) == 0)) {
    if (((0 < *(int *)(this + 0xd4)) && (*(int *)(iVar4 + 8) == 1)) &&
       ((in_fpscr = in_fpscr & 0xfffffff |
                    (uint)(*(float *)(*(int *)(iVar4 + 0xc) + 8) == DAT_0016f440) << 0x1e,
        (byte)(in_fpscr >> 0x1e) != 0 &&
        (iVar10 = *(int *)(this + 0xd4) - param_1, *(int *)(this + 0xd4) = iVar10, iVar10 < 1)))) {
      *(undefined4 *)(this + 0xd4) = 0;
      ParticleSystemManager::enableSystemRender
                (*(ParticleSystemManager **)(this + 0xe4),*(int *)(this + 0xcc),false);
    }
  }
  else {
    *(undefined1 *)(iVar4 + 0x4c) = 0;
    if ((*(int *)(this + 0xd0) - 4U < 2) || (*(int *)(this + 0xd0) == 0x28)) {
      if (*(int *)(this + 0xd4) == 0) {
        *(undefined4 *)(this + 0xd4) = 2000;
      }
      iVar10 = *(int *)(this + 0xcc);
      pPVar8 = *(ParticleSystemManager **)(**(int **)(this + 0xb0) + 0x80);
    }
    else {
      iVar10 = *(int *)(this + 0xcc);
      if (*(int *)(iVar4 + 0x58) == 0xe8) {
        pPVar8 = *(ParticleSystemManager **)(**(int **)(this + 0xb0) + 0x9c);
      }
      else {
        pPVar8 = *(ParticleSystemManager **)(**(int **)(this + 0xb0) + 0x84);
      }
    }
    ParticleSystemManager::enableSystemEmit(pPVar8,iVar10,false);
  }
  fVar1 = DAT_0016f444;
  fVar17 = DAT_0016f440;
  iVar10 = 0;
  fVar18 = (float)VectorSignedToFloat(param_1,(byte)(in_fpscr >> 0x16) & 3);
  iVar4 = 0;
  uVar15 = 0;
  fVar19 = fVar18;
  do {
    iVar9 = *(int *)(this + 8);
    if (*(uint *)(iVar9 + 8) <= uVar15) {
      if (*piVar2 == local_44) {
        return;
      }
      __stack_chk_fail();
    }
    iVar12 = *(int *)(iVar9 + 0xc);
    fVar19 = (float)AbyssEngine::AEMath::operator*
                              ((AEMath *)&local_6c,(Vector *)(*(int *)(iVar9 + 0x18) + iVar4),fVar19
                              );
    AbyssEngine::AEMath::operator*((AEMath *)&local_60,(Vector *)&local_6c,fVar19);
    AbyssEngine::AEMath::operator+
              ((AEMath *)&local_b8,(Vector *)(iVar12 + iVar4),(Vector *)&local_60);
    fVar19 = (float)AbyssEngine::AEMath::Vector::operator=((Vector *)&local_50,(Vector *)&local_b8);
    iVar9 = *(int *)(this + 8);
    if (*(char *)(iVar9 + 0x4c) != '\0') {
      in_fpscr = in_fpscr & 0xfffffff | (uint)(local_48 == fVar17) << 0x1e;
      fVar19 = local_48;
      if ((byte)(in_fpscr >> 0x1e) == 0) {
        if ((this[0xc0] != (RocketGun)0x0) &&
           ((iVar12 = *(int *)(this + 0xd8), iVar12 != 0 ||
            (iVar12 = *(int *)(*(int *)(iVar9 + 0x3c) + uVar15 * 4),
            iVar12 < *(int *)(iVar9 + 0x44) + -1000)))) {
          fVar19 = (float)seekEnemy(this,iVar12,uVar15);
          iVar9 = *(int *)(this + 8);
        }
        if (*(int *)(iVar9 + 0x5c) == 0x28) {
          iVar12 = *(int *)(iVar9 + 0x44);
          iVar13 = *(int *)(iVar9 + 0x3c);
          uVar7 = __aeabi_uidiv(iVar12 * uVar15,*(undefined4 *)(iVar9 + 8));
          fVar19 = (float)VectorSignedToFloat(iVar12 - *(int *)(iVar13 + uVar15 * 4),
                                              (byte)(in_fpscr >> 0x16) & 3);
          fVar21 = (float)VectorUnsignedToFloat(uVar7,(byte)(in_fpscr >> 0x16) & 3);
          fVar19 = (float)AbyssEngine::AEMath::Sinf((fVar21 + fVar19) * fVar1);
          fVar20 = (float)VectorSignedToFloat(*(int *)(*(int *)(this + 8) + 0x44) -
                                              *(int *)(*(int *)(*(int *)(this + 8) + 0x3c) +
                                                      uVar15 * 4),(byte)(in_fpscr >> 0x16) & 3);
          fVar20 = (float)AbyssEngine::AEMath::Cosf((fVar21 + fVar20) * fVar1);
          AbyssEngine::AEMath::VectorCross
                    ((AEMath *)&local_60,(Vector *)(*(int *)(*(int *)(this + 8) + 0x18) + iVar4),
                     (Vector *)(*(int *)(*(int *)(this + 8) + 0x24) + iVar4));
          AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_b8,(Vector *)&local_60);
          pcVar14 = *(code **)(DAT_0016f448 + 0x16f2d2);
          (*pcVar14)(&local_6c,(AEMath *)&local_b8,fVar19 + fVar19);
          (*pcVar14)((Vector *)&local_60,&local_6c,fVar18);
          AbyssEngine::AEMath::Vector::operator+=
                    ((Vector *)(*(int *)(*(int *)(this + 8) + 0xc) + iVar4),(Vector *)&local_60);
          (*pcVar14)(&local_6c,*(int *)(*(int *)(this + 8) + 0x24) + iVar4,fVar20 + fVar20);
          (*pcVar14)((Vector *)&local_60,&local_6c,fVar18);
          fVar19 = (float)AbyssEngine::AEMath::Vector::operator+=
                                    ((Vector *)(*(int *)(*(int *)(this + 8) + 0xc) + iVar4),
                                     (Vector *)&local_60);
        }
      }
      else {
        if (*(int *)(this + 0xd8) == 0) goto LAB_0016f3c6;
        fVar19 = (float)ParticleSystemManager::enableSystemEmit
                                  (*(ParticleSystemManager **)(this + 0xe4),
                                   *(int *)(*(int *)(*(int *)(this + 0xdc) + 4) + uVar15 * 4),false)
        ;
        if (*(int *)(*(int *)(*(int *)(this + 0xe0) + 4) + uVar15 * 4) == 0) {
          *(undefined4 *)(*(int *)(*(int *)(this + 0xe0) + 4) + uVar15 * 4) = 2000;
        }
      }
      if (*(int *)(this + 0xd8) != 0) {
        iVar9 = *(int *)(*(int *)(this + 8) + 0xc);
        fVar19 = (float)AbyssEngine::AEMath::operator*
                                  ((AEMath *)&local_6c,
                                   (Vector *)(*(int *)(*(int *)(this + 8) + 0x18) + iVar4),fVar19);
        AbyssEngine::AEMath::operator*((AEMath *)&local_60,(Vector *)&local_6c,fVar19);
        AbyssEngine::AEMath::operator+
                  ((AEMath *)&local_b8,(Vector *)(iVar9 + iVar4),(Vector *)&local_60);
        AbyssEngine::AEMath::Vector::operator=((Vector *)&local_50,(Vector *)&local_b8);
        AbyssEngine::AEMath::MatrixSetTranslation
                  ((AEMath *)&local_b8,(Matrix *)(*(int *)(*(int *)(this + 0xd8) + 4) + iVar10),
                   local_48,extraout_s1_01,extraout_s2_00);
        AbyssEngine::AEMath::VectorNormalize
                  ((AEMath *)&local_60,(Vector *)(*(int *)(*(int *)(this + 8) + 0x18) + iVar4));
        iVar9 = *(int *)(*(int *)(this + 0xd8) + 4);
        iVar12 = iVar4 * 5 + iVar9;
        *(undefined4 *)(iVar12 + 8) = (undefined4)local_60;
        *(undefined4 *)(iVar12 + 0x18) = local_60._4_4_;
        *(float *)(iVar12 + 0x28) = local_58;
        fVar19 = (float)AbyssEngine::AEMath::MatrixSetTranslation
                                  ((AEMath *)&local_b8,(Matrix *)(iVar9 + iVar10),local_48,
                                   extraout_s1_02,extraout_s2_01);
      }
    }
LAB_0016f3c6:
    if ((*(int *)(this + 0xe0) != 0) &&
       (in_fpscr = in_fpscr & 0xfffffff | (uint)(local_48 == fVar17) << 0x1e, fVar19 = local_48,
       (byte)(in_fpscr >> 0x1e) != 0)) {
      iVar9 = *(int *)(*(int *)(this + 0xe0) + 4);
      iVar12 = *(int *)(iVar9 + uVar15 * 4);
      if ((0 < iVar12) &&
         (iVar12 = iVar12 - param_1, *(int *)(iVar9 + uVar15 * 4) = iVar12, iVar12 < 1)) {
        *(undefined4 *)(iVar9 + uVar15 * 4) = 0;
        fVar19 = (float)ParticleSystemManager::enableSystemRender
                                  (*(ParticleSystemManager **)(this + 0xe4),
                                   *(int *)(*(int *)(*(int *)(this + 0xdc) + 4) + uVar15 * 4),false)
        ;
      }
    }
    iVar10 = iVar10 + 0x3c;
    iVar4 = iVar4 + 0xc;
    uVar15 = uVar15 + 1;
  } while( true );
}
```
