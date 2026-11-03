# `Level::createScene` — wave-95 w4514: the parts-loop RA is whole-function coupled

**Status:** research note, 2026-11-03. Worktree `wt/w4514`, branch `wave95-w4514`,
base `4a88bd6e`. **No source change landed** — no source shape found that raises
`byte_exact`. This commit contains only this note.

Follow-up to `level-createscene-wave94-w4414.md` (which followed
`level-createscene-wave92-w4249.md`, `level-createscene-wave92-w4213.md`,
`level-createscene-wave91-w4113.md`, `level-createscene-phi-use-w4013.md`,
`level-createscene-licm-hoist-w3833.md`, `Level-createScene-flag-free.md`,
`level-createscene-88-3-2252-b_w3813.md`).

## TL;DR

* Target `_ZN5Level11createSceneEv`: **88.3 %**, orig/ours both **2252 B**,
  783/791 instructions, `byte=False`, `linked=False`.
* Gate re-measured this pass, unchanged: **byte_exact 4500/4524,
  linked_exact 4500/4524, size-exact 4524/4524, stub_zero_size 0, missing 0,
  wrong_type 0, extra 0, data_exact 843/843, imports 207, lint CLEAN ✓,
  drift 0, RATCHET PASS.**
* **New evidence (the main result of this pass): the pre-RA machine code of the
  parts loop is *byte-for-byte identical* between the shipped source and the
  "no local `rnd`" (all-globals) source, yet the two get *different* register
  allocations there.** The only pre-RA difference between the two sources is
  three extra `@_ZN7Globals3rndE` GOT+value loads in the **ship** loop. So the
  parts-loop residue is not a parts-loop source-shape problem at all: it is
  whole-function greedy-allocator coupling through the ship loop. §3.
* **Correction to wave-94 §3:** the `t2ADDri %stack.1, 0` MachineLICM hoist is
  *not* decided by `CanCauseHighRegPressure`. `t2ADDri` is
  `isRematerializable=1` and its only non-def operand is a frame index, so
  `TargetInstrInfo::isReallyTriviallyReMaterializableGeneric()` returns `true`
  and `IsProfitableToHoist` returns before the register-pressure estimate.
  The clang `-print-after-all` dump and the `llc -debug-only=machinelicm` log
  both confirm it (no "Hoist non-reg-pressure" / "Won't speculate" line is
  printed for it). §4.
* ~75 new source shapes measured this pass (all-globals family ×37,
  shipped-source family ×36). **None beats 88.3 %**, and — more importantly —
  **none changes the parts-loop register signature at all** except the ones
  that already changed it (`Globals::Canvas` hoisted into a local, removing the
  local `rnd`, and — new this pass — moving `capacity` in front of
  `ownHangar`). §5.
* **New this pass: the original's parts-loop register assignment *is*
  reachable.** All-globals + `int capacity = kHangarCapacity[sceneRace];`
  moved in front of `bool ownHangar = false;` reproduces **all eight**
  parts-loop registers (`row→r9, Canvas→r8, zero→sl, k→r6, hangarPart→r4,
  g→r5, j→sl, child count→fp`), 77.2 % / 2264 B. It also changes the ship-loop
  load order, so it is not byte-exact, but it shows the lever is the ship-loop
  prologue, not the parts loop. §5.3.

## 1. What the diff is

`tools/verify/verify.py --show _ZN5Level11createSceneEv` at the baseline: 276
`|` lines, all inside the `missionPtr == 23` branch. The first 459 instructions
of the function are byte-identical to the original; the first diff is
instruction 460, the parts-loop preheader.

The instruction *stream* is the same on both sides — only register names and
spill-slot numbers differ (plus 8 extra spill/reload instructions on our side).

### 1.1 Register maps recovered from the disassembly

Parts loop (`for (int k = 0; k < 4; k++)`, `kHangarMeshes[sceneRace][k]`):

| value | original | shipped source | all-globals source |
|---|---|---|---|
| row `kHangarMeshes[sceneRace]` | **`r9`** (spill `sp#24`) | `r8` (spill `sp#24`) | `r8` (spill `sp#28`) |
| `Globals::Canvas` value | **`r8`** | `r9` | `sl` |
| zero of `Vector{0,pi,0}` | **`sl`** | `r4` | `r9` |
| `k` | **`r6`** | spilled `sp#44` | `r6` |
| `hangarPart` | **`r4`** (spill `sp#44`) | `r6` | `r4` (spill `sp#36`) |
| `g` | `r5` | `r5` | `r5` |
| `j` | **`sl`** | `sl` | `fp` |
| child count `kBarChildCounts[sceneRace]` | **`fp`** | `r8` | `r9` |
| `&Globals::status` | spill `sp#40` | spill `sp#40` | spill `sp#44` |

Ship loop (`for (int i = 0; i < shipCount; i++)`):

| value | original | shipped source | all-globals source |
|---|---|---|---|
| `&Globals::rnd` | **`r8`** (value reloaded at every site) | spill `sp#44` | `r9` |
| `rnd` value | never cached | `r5` | never cached |
| `capacity` | **`r6`** | `fp` | `r8` |
| `race` | **`r5`** | `r6` | `r5` |
| `type` | **`fp`** | `r8` | `fp` |
| `i` | **`r9`** | `r9` | `sl` |
| `&Globals::status` | **`sl`** | `sl` | `r6` |
| `spotUsed` | `r4` | `r4` | `r4` |

So the shipped source is already right on `{g, j, i, &status, spotUsed}` and
wrong on `{row, Canvas, zero, k, hangarPart, child count, &rnd, capacity,
race, type}`; the all-globals source is right on `{k, hangarPart, g, race,
type, spotUsed}` and wrong on `{row, Canvas, zero, j, child count, &rnd,
capacity, i, &status}`. Neither is a superset of the other, and the two
disagree with each other on 8 values.

## 2. The original source uses `Globals::rnd` and `Globals::status` directly

Re-confirmed from the Ghidra decompile of the target
(`/dev/shm/w4514/ghidra_cs.c`, produced with
`analyzeHeadless … -process android_2.0.16_libgof2hdaa.so -noanalysis
-postScript DecompFile.java`, `FN_ADDR=0xc42ec`; image base `0x10000`):

```c
/* parts loop */
for (uVar22 = 0; uVar22 < 4; uVar22 = uVar22 + 1) {
  iVar4 = *(int *)(&UNK_0020d678 + uVar22 * 4 + uVar6 * 0x10);   /* kHangarMeshes */
  if (-1 < iVar4) {
    pAVar23 = (AEGeometry *)FUN_0006eb24(0xc0);
    AEGeometry::AEGeometry(pAVar23,(ushort)iVar4,Globals::Canvas,false);
    AEGeometry::setRotation((Vector *)pAVar23);
    ...
    if ((int)uVar6 < 4 && uVar6 != 1) {            /* !plainHangar */
      iVar12 = *(int *)(&UNK_0020b818 + uVar6 * 4); /* kBarChildCounts */
      for (iVar4 = 0; iVar4 < iVar12; iVar4 = iVar4 + 1) {
        uVar13 = *(undefined4 *)(&UNK_0020b828 + uVar6 * 4); /* kBarChildMeshes */
        this_00 = (AEGeometry *)FUN_0006eb24(0xc0);
        AEGeometry::AEGeometry(this_00,(short)iVar4 + (short)uVar13,Globals::Canvas,false);
        AEGeometry::addChild(pAVar23,*(uint *)(this_00 + 0xc));
        ...
      }
    }
    ArrayAdd<KIPlayer*>((KIPlayer *)pPVar14,*(Array **)(this + 0xf8));
  }
}
...
/* ship loop */
for (; iVar4 < (int)local_74; iVar4 = iVar4 + 1) {
  iVar12 = AbyssEngine::AERandom::nextInt(Globals::rnd,100);
  uVar16 = uVar6;
  if (iVar12 < 0x1e) {
    uVar16 = AbyssEngine::AERandom::nextInt(Globals::rnd,4);
    iVar12 = AbyssEngine::AERandom::nextInt(Globals::rnd,100);
    if (iVar12 < 0x1e) { uVar16 = 8; }
  }
  iVar12 = Globals::getRandomEnemyFighter(Globals::globals,uVar16);
  pSVar5 = (Station *)Status::getStation(Globals::status);
  iVar17 = Station::getIndex(pSVar5);
  if (iVar17 == 100) {
    iVar17 = AbyssEngine::AERandom::nextInt(Globals::rnd,3);
    ...
  }
  ...
  iVar17 = AbyssEngine::AERandom::nextInt(Globals::rnd,uVar22);
  iVar21 = -100;
  while ((iVar21 != 0 && (*(char *)((int)pvVar3 + iVar17) != '\0'))) {
    iVar17 = AbyssEngine::AERandom::nextInt(Globals::rnd,uVar22);
    iVar21 = iVar21 + 1;
  }
  *(undefined1 *)((int)pvVar3 + iVar17) = 1;
  ...
  uVar13 = AbyssEngine::AERandom::nextInt(Globals::rnd,300);
  ...
}
```

Every `nextInt` site reads `Globals::rnd` (the global pointer), never a cached
local. Every status site reads `Globals::status`. This matches the binary's
`ldr.w r8,[r0]` (= `&Globals::rnd`) + `ldr.w r0,[r8]` at all six ship-loop
`nextInt` sites, and `ldr.w sl,[r0]` (= `&Globals::status`) + `ldr.w r0,[sl]`
at all ten status sites.

## 3. New: the parts-loop pre-RA code is identical in both source shapes

Recipe (works with the NDK clang 7.0.2 release build, which has no
`-debug-only`):

```bash
src/game/world/Level.cpp with the project's exact flags, plus
  -S -mllvm -print-after-all -mllvm -filter-print-funcs=_ZN5Level11createSceneEv
```

The last IR dump before `# *** IR Dump After Expand ISel Pseudo-instructions ***`
(here `dumps_base.txt:117806..118842`, the `Safe Stack instrumentation pass`
dump) is the IR that ISel consumes. The machine dump
`# *** IR Dump After Expand ISel Pseudo-instructions ***`
(`dumps_base.txt:118843`) is the pre-RA machine code.

Normalising vreg names / block numbers / stack slots / `!dbg` line numbers and
diffing the pre-RA machine code of the shipped source against the all-globals
source gives **93.1 % identity, and the only non-trivial differences are in the
ship loop**:

```
delete 574 575 574 574
  - %V:tgpr = tLDRLIT_ga_pcrel target-flags(<unknown>) @_ZN7Globals3rndE,
delete 585 587 583 583
  - %V:gpr = t2LDRi12 killed %V:gpr, 0, 14, $R, :: (dereferenceable load 4 from @_ZN7Globals3rndE, )
  - DBG_VALUE debug-use %V:gpr, debug-use $R, !"rnd", !DIExpression(), ; line no:6649
insert 611 611 607 610
  + %V:tgpr = tLDRLIT_ga_pcrel target-flags(<unknown>) @_ZN7Globals3rndE,
  + %V:gpr = t2LDRi12 killed %V:tgpr, 0, 14, $R, :: (load 4 from got)
  + %V:gpr = t2LDRi12 killed %V:gpr, 0, 14, $R, :: (dereferenceable load 4 from @_ZN7Globals3rndE, )
insert 678 678 677 680        (same 3-instruction group, `pick` site)
insert 782 782 784 787        (same 3-instruction group, `tries` site)
```

Everything else in the diff is `DBG_VALUE` line numbers and `bb.N` /
`%ir-block.N` renumbering. **The parts loop (`bb.9`…`bb.24`) has no
instruction difference at all.**

Yet the two sources allocate the parts loop differently
(`row=r8/Canvas=r9` vs `row=r8/Canvas=sl`, `k` spilled vs `k=r6`). Greedy
register allocation is a single whole-function pass; the three extra
`&Globals::rnd` intervals in the ship loop change the queue order and therefore
which of `{r6,r8,r9,sl,fp}` is free when the parts-loop intervals are
allocated. **This is why ~380 source shapes of the parts loop have failed to
move it: the parts loop is not where the decision is made.**

## 4. Correction: the `t2ADDri` hoist is unconditional, not pressure-driven

Wave-94 §3 argued the `t2ADDri %stack.1, 0` hoist is a
`CanCauseHighRegPressure` decision and that the original must have had one more
live GPR in the parts loop. That is wrong.

`llvm-7.0.0.src/lib/CodeGen/TargetInstrInfo.h`:

```cpp
bool isTriviallyReMaterializable(const MachineInstr &MI, AliasAnalysis *AA = nullptr) const {
  return MI.getOpcode() == TargetOpcode::IMPLICIT_DEF ||
         (MI.getDesc().isRematerializable() &&
          (isReallyTriviallyReMaterializable(MI, AA) ||
           isReallyTriviallyReMaterializableGeneric(MI, AA)));
}
```

`t2ADDri` (`T2I_bin_ii12rs`) is `isRematerializable = 1`; its operands are the
def vreg and the frame index `%stack.1` (a `MachineOperand::isFI()`, not a
register), so `isReallyTriviallyReMaterializableGeneric`
(`TargetInstrInfo.cpp:885`) walks the operand list, finds no virtual-register
uses and no non-constant physregs, and returns `true`.

`MachineLICM.cpp:1218` then returns `true` immediately, before
`calcRegisterCost` / `CanCauseHighRegPressure`:

```cpp
  // Rematerializable instructions should always be hoisted since the register
  // allocator can just pull them down again when needed.
  if (TII->isTriviallyReMaterializable(MI, AA))
    return true;                                   // <- no debug message
```

That path prints **no** `LLVM_DEBUG` message, which is exactly what the
`llc -debug-only=machinelicm` log shows (`/dev/shm/w4514/licm_base.log`):

```
Entering %bb.14
Hoisting %539:tgpr = tLDRpci %const.1, 14, $noreg :: (load 4 from constant-pool)
 from %bb.14 to %bb.8
Hoisting %540:rgpr = t2MOVi 0, 14, $noreg, $noreg
 from %bb.14 to %bb.8
CSEing %540:rgpr = t2MOVi 0, 14, $noreg, $noreg
 with %368:rgpr = t2MOVi 0, 14, $noreg, $noreg
Hoisting %541:gprnopc = t2ADDri %stack.1, 0, 14, $noreg, $noreg
 from %bb.14 to %bb.8
Hoisting %542:rgpr = t2MOVi 304, 14, $noreg, $noreg
 from %bb.14 to %bb.8
```

and the real clang `-print-after-all` dump agrees — `%541:gprnopc =
t2ADDri %stack.1, 0` is in `bb.8` (the parts-loop preheader) after
`# *** IR Dump After Early Machine Loop Invariant Code Motion ***`
(`dumps_base.txt:133179`, body at 133460ff), whereas before that pass it sat in
`bb.19` right after the two `t2STRi12 %540, %stack.1, {0,8}` stores
(`dumps_base.txt:119238`).

So **no amount of extra live GPRs in the parts loop will suppress the hoist**:
`t2ADDri` is trivially rematerialisable, and the only escapes are
`CheapInstr && CreatesCopy` (needs `isAsCheapAsAMove`, i.e. `t2MOVi`, not
`t2ADDri`) or a surviving machine loop-PHI use (wave-92 §2.3 exhausted those).

The post-RA `Virtual Register Rewriter` dump shows the instruction itself is
correctly rematerialised in place, so only the *transient* interval does
damage:

```
4592B  bb.19 (%ir-block.123):
       ; predecessors: %bb.18
       ;   successors: %bb.20(0x7c000000), %bb.24(0x04000000)
       ;   liveins: $r5, $r6, $r9
4596B    t2STRi12 killed renamable $r6, %stack.22, 0, 14, $noreg :: (store 4 into %stack.22)
4608B    renamable $r0 = tLDRpci %const.1, 14, $noreg, debug-location !18085
4624B    t2STRi12 killed renamable $r0, %stack.1, 4, 14, $noreg, debug-location !18085
4632B    renamable $r4 = t2MOVi 0, 14, $noreg, $noreg            ; <- zero -> r4
4640B    t2STRi12 renamable $r4, %stack.1, 0, 14, $noreg, debug-location !18084
4648B    t2STRi12 renamable $r4, %stack.1, 8, 14, $noreg, debug-location !18086
4656B    ADJCALLSTACKDOWN 0, 0, 14, $noreg, implicit-def dead $sp, implicit $sp
4672B    $r0 = COPY renamable $r5, debug-location !18087
4680B    renamable $r1 = t2ADDri %stack.1, 0, 14, $noreg, $noreg, debug-location !18087
4704B    tBL 14, $noreg, @_ZN10AEGeometry11setRotationERKN11AbyssEngine6AEMath6VectorE, …
...
4800B    renamable $r6 = COPY $r0, debug-location !18089
         DBG_VALUE debug-use $r6, debug-use $noreg, !"hangarPart", …
```

(`dumps_base.txt:175918..175940`; the corresponding pre-RA block is
`dumps_base.txt:174081`, `# *** IR Dump After Greedy Register Allocator ***`.)
`$r6` is `k`, spilled at 4596B, and `hangarPart` takes `r6` at 4800B. In the
original it is the other way round: `k` stays in `r6` and `hangarPart` is
spilled to `sp#44` because `r4` is reused by `child` inside the inner loop.

## 5. New source-shape measurements (no flags)

Baseline: **88.3 % / 2252 B**. Every row is a full
`cmake --build cmake-build-match --target link_so` + a single-function verify of
`_ZN5Level11createSceneEv`. "sig" is the parts-loop register signature
`(row, Canvas)`; the original is `(r9, r8)`.

### 5.1 all-globals family (no local `rnd`; base 75.0 % / 2228 B, sig `(r8, sl)`)

| variant | pct | size | sig |
|---|---|---|---|
| all-globals | **75.0** | 2228 | (r8, sl) |
| + `unsigned capacity` | 75.0 | 2228 | (r8, sl) |
| + `const int capacity` | 75.0 | 2228 | (r8, sl) |
| + `int i` shared by both loops | 75.0 | 2228 | (r8, sl) |
| + `for (k = 0; k != 4; ++k)` | 75.0 | 2228 | (r8, sl) |
| + `Status *&status = Globals::status` | 75.0 | 2228 | (r8, sl) |
| + `const Vector rot{…}` named temporary | 75.0 | 2228 | (r8, sl) |
| + `AEGeometry *g` declared before the loop | 75.0 | 2228 | (r8, sl) |
| + `PlayerStatic *hangarPart` declared before the loop | 75.0 | 2228 | (r8, sl) |
| + `int mesh` declared before the loop | 75.0 | 2228 | (r8, sl) |
| + `int j` declared before the loop | 75.0 | 2228 | (r8, sl) |
| + `AEGeometry *child` declared before the loop | 75.0 | 2228 | (r8, sl) |
| + `hangarPart` inlined into `ArrayAdd` | 73.3 | 2228 | (sl, r9) |

### 5.2 shipped-source family (base 88.3 % / 2252 B, sig `(r8, r9)`)

| variant | pct | size | sig |
|---|---|---|---|
| `for (k = 0; k != 4; ++k)` | 88.3 | 2252 | (r8, r9) |
| `int plainHangar` | 88.3 | 2252 | (r8, r9) |
| `bool plainHangar = !(sceneRace < 4 && sceneRace != 1)` | 88.3 | 2252 | (r8, r9) |
| `ArrayAdd<KIPlayer *>(…)` explicit template arg | 88.3 | 2252 | (r8, r9) |
| `static_cast<KIPlayer *>(hangarPart)` | 88.3 | 2252 | (r8, r9) |
| `spotUsed[spot]` instead of `spotUsed[spot] != 0` | 88.3 | 2252 | (r8, r9) |
| `tries > 0` | 88.3 | 2252 | (r8, r9) |
| `while` form of the retry loop | 88.3 | 2252 | (r8, r9) |
| `const int capacity` | 88.3 | 2252 | (r8, r9) |
| `bool ownHangar = … && …` (one expression) | 88.3 | 2252 | (r8, r9) |
| `const int *row = kHangarMeshes[sceneRace]` before the loop | 88.3 | 2252 | (r8, r9) |
| `const int (*row)[4] = &kHangarMeshes[sceneRace]` | 88.3 | 2252 | (r8, r9) |
| `int sr = sceneRace` copied inside the loop | 88.3 | 2252 | (r8, r9) |
| `AEGeometry *g` / `PlayerStatic *hangarPart` / `int mesh` / `int j` / `AEGeometry *child` / `int k` declared before the loop (each, and all together) | 88.3 | 2252 | (r8, r9) |
| extra live `int` / live `const int *` spanning the loop | 88.3 | 2252 | (r8, r9) |
| extra dead local in the `==2` branch / in the `==23` branch | 88.3 | 2252 | (r8, r9) |
| `swap setToSleep` / `setExhaustVisible` | 87.9 | 2252 | (r8, r9) |
| `new unsigned char[capacity]()` | 84.8 | 2260 | (r8, r9) |
| `spots` computed before `spotUsed` | 84.8 | 2252 | (r8, r9) |
| `Globals::Canvas` hoisted into a local before the loop | 82.8 | 2252 | (r8, r8) |
| `int st = getStation()->getIndex()` cached for `sceneRace` | 80.7 | 2244 | (r8, r9) |
| `shipCount` computed after `ownHangar` | 79.5 | 2264 | (r8, r9) |
| `unsigned short mesh` | 76.0 | 2244 | — |
| `int agentCount` inlined in the `==4` branch | 75.6 | 2244 | (r8, r9) |
| `capacity` moved before `ownHangar` (all-globals) | 77.2 | 2264 | **(r9, r8)** |
| `capacity` moved before `ownHangar` (shipped) | 79.8 | 2276 | (r8, r9) |
| `spotUsed` allocated before `ownHangar` (all-globals) | 72.3 | 2268 | **(r9, r8)** |

The parts-loop signature is invariant across **every** semantically neutral
variant tried (36 this pass, ~380 cumulative). The only two things that move it
are (a) hoisting `Globals::Canvas` into a local and (b) removing the local
`rnd` — and both move the score down.

### 5.3 The parts loop *can* be matched — by moving `capacity` before `ownHangar`

The one change found this pass that reproduces the original's **parts-loop
register assignment in full** is: take the all-globals shape and move
`int capacity = kHangarCapacity[sceneRace];` in front of `bool ownHangar = false;`
(the `if (ownHangar) { … }` block and `shipCount` stay where they are):

```cpp
int capacity = kHangarCapacity[sceneRace];
bool ownHangar = false;
if (Globals::status->getStation()->getIndex() == 108)
    ownHangar = Globals::status->field_114 == 3;
int shipCount = Globals::rnd->nextInt(capacity + 1);
```

Measured: **77.2 % / 2264 B** (vs 75.0 / 2228 for plain all-globals and
88.3 / 2252 for the shipped source). Parts-loop register map becomes exactly
the original's:

```
465 add.w  r9, r0, r2, lsl #4        ; row          -> r9   (orig r9)
468 ldr.w  r4, [r9, r6, lsl #2]      ; k            -> r6   (orig r6)
473 mov    r5, r0                    ; g            -> r5   (orig r5)
476 ldr.w  r8, [r0]                  ; Canvas       -> r8   (orig r8)
485 mov.w  sl, #0                    ; Vector zero  -> sl   (orig sl)
492 mov    r4, r0                    ; hangarPart   -> r4   (orig r4)
502 mov.w  sl, #0                    ; j            -> sl   (orig sl)
505 ldr.w  fp, [r0, r1, lsl #2]      ; child count  -> fp   (orig fp)
525 add.w  sl, sl, #1 / cmp sl, fp   ; j loop       -> sl/fp
```

**All eight** parts-loop values now land in the original's registers (the
shipped source gets only `g` and `j` right). The remaining parts-loop
differences are non-register: the `str.w sl,[sp,#40]` / `ldr.w sl,[sp,#40]`
`&Globals::status` spill pair is absent (status is already spilled to `sp#44`
earlier), `hangarPart` spills to `sp#32` instead of `sp#44`, and all branch /
literal-pool offsets shift.

Why it works: moving the `capacity` load in front of the `getStation()` /
`getIndex()` calls lengthens `capacity`'s live interval by ~3 call sites. That
reorders the greedy queue for the whole function, and the parts-loop intervals
then colour exactly as in the original. It also changes the **ship**-loop
instruction order (`capacity` is now loaded before `ownHangar` is computed,
whereas the original loads it after — see §1.1 and the binary at `0x68e`), so
this shape cannot be byte-exact as written:

```
original                                 ag+capacity-first
532 ldr.w r0,[sl]       ; status         537 ldr r1,[pc,#676]  ; kHangarCapacity
533 blx   getStation                     538 ldr r4,[sp,#44]   ; &status
534 blx   getIndex                       539 ldr r2,[sp,#52]
535 cmp   r0,#108                        540 add r1,pc
536 bne.n 0x636                          541 ldr r0,[r4,#0]
537 ldr.w r0,[sl]                        542 ldr.w fp,[r1,r2,lsl #2]  ; capacity
538 ldr.w r0,[r0,#276]                   543 blx   getStation
539 subs  r0,#3                          544 blx   getIndex
540 clz   r0,r0                          545 cmp   r0,#108
541 lsrs  r4,r0,#5   ; ownHangar          ...
...
550 ldr.w r6,[r1,r0,lsl #2] ; capacity
```

The ship loop of this shape is also closer to the original than either
previous shape: `{&rnd→r8 ✓, race→r5 ✓, i→r9 ✓, spotUsed→r4 ✓,
capacity→fp ✗ (orig r6), type→sl ✗ (orig fp), &status→sp#44 ✗ (orig sl)}`
— 4 of 7 right, versus 3 of 7 for both the shipped and the plain
all-globals shapes. Eight further tweaks on top of it (const/unsigned
`capacity`, `spots` moved, `spot` hoisted, `tries` unsigned, `spots` in the
`default:` arm, …) all reproduce exactly 77.2 / 2264 and the same maps.

So the parts loop is reachable; what is missing is a *ship-loop* change that
(a) keeps the original's `ownHangar`-before-`capacity` load order and (b)
produces the same queue reordering. That is the concrete remaining lever.

## 6. What this proves / what remains

**Proved:**

1. The whole `createScene` residue is register allocation; the instruction
   stream is identical and the first 459 instructions are byte-identical.
2. The original source uses `Globals::rnd` and `Globals::status` **directly**
   (address in a callee-saved register, value reloaded at every use); it has
   no `AERandom *rnd` and no `Status *status` local.
3. **The parts-loop pre-RA machine code is identical between the shipped
   source and the all-globals source, yet the parts-loop allocation differs.**
   The parts-loop residue is therefore not controllable from the parts loop;
   it is decided by the *ship*-loop intervals through the single whole-function
   greedy pass.
4. The `t2ADDri %stack.1, 0` MachineLICM hoist is unconditional
   (`isTriviallyReMaterializable` fast path), not a `CanCauseHighRegPressure`
   decision. Adding live values to the parts loop cannot suppress it.
5. The greedy allocation is remarkably stable: ~380 semantically neutral
   source shapes, and the parts-loop signature only ever takes two values —
   `(r8, r9)` for the shipped shape and `(r8, sl)` for the all-globals shape.

**Remaining levers** (none landed):

1. A ship-loop shape that yields the original's four-register cycle
   `{&rnd→r8, capacity→r6, &status→sl, i→r9}`. The all-globals shape gives
   `{&rnd→r9, capacity→r8, &status→r6, i→sl}`, which is exactly a one-step
   rotation of it, and the shipped shape gives `{&rnd→sp#44, capacity→fp,
   &status→sl, i→r9}` (right on `i`/`&status`, wrong on the rest). Neither the
   2^8 local-vs-`Globals::rnd` matrix (wave-92 §2.2) nor any declaration/type
   form tried moves it further.
2. A **ship-loop prologue** shape that keeps the original's
   `ownHangar`-then-`capacity` load order *and* reproduces the queue
   reordering that §5.3 shows is sufficient to colour the parts loop exactly.
   This is now the concrete, evidenced target: the parts loop itself needs no
   further work.
3. A per-function `-sink-insts-to-avoid-spills` (not available in clang 7.0.2)
   would make the parts loop byte-identical (93.3 % / 2252 B with the mask-70
   `rnd` shape, wave-92 §3.1) but is TU-wide and costs 2142 `byte_exact`.

## 7. Scratch artefacts (not committed)

`/dev/shm/w4514/`:

* `Level.cpp.base` — pristine `src/game/world/Level.cpp`;
  `Level.cpp.allglob` — the no-local-`rnd` variant.
* `fscore.py` — single-function score (0.1 s);
  `sig.py` — score + parts-loop / ship-loop register signature;
  `sweep.py` + `v_ag.json` / `v_base1.json` / `v_base2.json` / `v_base3.json`
  — the variant tables above.
* `dump2.py` — address-annotated full disassembly of both binaries
  (`orig.txt`, `ours_base.txt`, `ours_allglob.txt`).
* `dumps_base.txt` (224 026 lines) / `dumps_ag.txt` — clang
  `-mllvm -print-after-all` for the shipped and all-globals sources.
  Key line numbers: pre-ISel IR `117806..118842`; pre-RA machine code
  `118843`; after Early Machine LICM `133179`; after Greedy RA `173709`;
  after Virtual Register Rewriter `175551`.
* `licm_base.log` — `llc -O2 -debug-only=machinelicm` on the pre-ISel IR.
* `ghidra_cs.c` — Ghidra decompile of `Level::createScene`.
* `cc.sh` — the TU's exact clang++ command line.
