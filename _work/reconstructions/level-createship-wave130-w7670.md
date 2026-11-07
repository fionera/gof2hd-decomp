# `Level::createShip` — wave 130 (w7670): landing-pad block order is creation order; r6/r8 is decided before RA

Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`, original
2764 B @ `0xc0214`, Thumb entry `0xc0215`, 1007 insns).
Base `003a9623a` (wave-129 final). Worktree `/home/debian/GalaxyOnFire2/wt/w7670`.
Owned TU: `src/game/world/Level.cpp` (createShip only).

**Status: analysis only — no `src/` change landed.**  The checked-in body stays
the best at **70.5415 %** (official rounded 70.5), 2764 B, 1007/1006 insns,
exact 336 B / 57-row / `ttype 0xff` cleanup-only LSDA.  This pass attacks the
wave-126/129 open question ("a source shape that makes clang emit the cleanup
landing pads at the end of the IR block list") with a new mechanism-level
answer: **clang 7 creates each `lpad` block at its `invoke`, and
`createBasicBlock` appends to the function's block list, so the lpad block order
is the invoke order — not the cleanup-scope pop order.**  The only way to move
the pads to the end is therefore to move the *invokes* to the end, which the
original's code order forbids for `new Player`.  The r6/r8 choice is thus fixed
before RA and cannot be flipped by scope/early-return/destructor spelling.

## Result

```
                                   original        HEAD (003a9623a)
createShip exact ratio              —               70.541480
createShip size                    2764 B          2764 B
createShip insns (orig/ours)       1007            1007 / 1006
createShip LSDA record             336 B           336 B
createShip LSDA rows               57              57
createShip LSDA ttype              0xff            0xff
byte_exact / linked_exact          —               4506 / 4506
data / imports / lint / drift      —               843 / 207 / CLEAN / 0
```

`check` on the unchanged tree: **RATCHET PASS** — `verify: byte 4506 linked
4506 stubs 0 missing 0 extra 0 avg 99.94`, `data: exact 843 of 843`,
`sodiff: imports 207`, lint `CLEAN`, drift 0.

## 1. New evidence: lpad blocks are created at the invoke, not at scope pop

`clang/lib/CodeGen/CGException.cpp:773` (`EmitLandingPad`) does
`createBasicBlock("lpad")`; `CodeGenFunction::createBasicBlock` appends to
`CurFn`'s block list.  The lpad is requested lazily from
`getEHDispatchBlock`/`getLandingPad` when the first `invoke` of that cleanup
scope is emitted.  `EmitIfStmt` (`CGStmt.cpp:602`) wraps the then/else bodies in
`RunCleanupsScope`, which only pops cleanups created *inside* that body
(`RunCleanupsScope` saves `EHStack.stable_begin()`).  So neither a nested `{}`,
an early `return`, nor a local destructor can move an already-created `lpad`
block: the block was appended at the invoke.

Consequence for the exception PHI (`%676` in the pre-RA MIR): the first incoming
`COPY $r0` is in the lpad of the **first invoke**, i.e. `new Player`
(`lpad`, IR block index 14 of 92).  The PHI is used at `bb.100` (the
`ehcleanup`/`_Unwind_Resume` block, the last IR block).  Its live interval is
therefore `[bb.19, bb.100]` (MIR offset 4336B..26352B) and overlaps both
`r6` (shipClass/this) and `r8` (mission, then `z`), so Greedy RA picks `r6`.
The original's `r8` exception pointer is only possible if its PHI is short,
i.e. its first pad is after `z`'s last use (`c0b04`) — which requires the
`new Player` invoke to be late in the IR, not early.

## 2. New finding: the original's BoundingAAB x/y/zero map is the r6 cascade

Instruction-by-instruction, the first `new BoundingAAB` block is:

```
original                              ours
c048c vmov sl, s20   ; x -> sl        c0486 vmov r6, s20   ; x -> r6
c0490 mov  r5, r0    ; ptr -> r5      c048a mov  r5, r0
c0492 vmov r8, s18   ; z -> r8        c048c vmov r8, s18
c0498 vmov r4, s16   ; y -> r4        c0494 vmov r3, s16   ; y -> r3
c04a2 movs r6, #0    ; zero -> r6     c049c mov  sl, r3    ; y -> sl
c04ae mov  r1, sl    ; x arg          c04a6 mov  r1, r6    ; x arg
c04b8 str  r6,[sp,#0]; 0.0f           c04ac movs r0,#0
                                      c04ae str  r0,[sp,#0]
```

The original keeps `r6 = 0` across the whole BoundingAAB sequence and folds
`strd r6,r0,[sp]` (12 B saved over 6 calls); ours rematerialises
`movs r0,#0; str r0,[sp]` every call.  The `x`/`y` register swap
(`sl`/`r4` vs `r6`/`sl`) is the same single decision: `r6` is the exception
pointer, so `x` takes `r6` and `y` is pushed to `sl`.  Fixing the exception
pointer fixes the 12-byte delta and the x/y map together.

## 3. Variants run this pass (full link, one-function exact ratio)

All rows keep the exact 336 B / 57-row / `ttype 0xff` LSDA unless noted.
`first lpad` is the IR block index of the first `landingpad` block.

| variant | exact % | size | insns | first lpad | note |
|---|---:|---:|---:|---:|---|
| **HEAD: mixed `float z, float y, int x` + status guard** | **70.541480** | 2764 | 1007/1006 | 14/92 | best |
| no status guard | 63.558897 | 2764/2748 | 1007/988 | 19/90 | faithful shape, worse |
| no guard + early `return` in both ship branches | 62.424850 | 2764/2740 | 1007/989 | 19/90 | |
| no guard + `if (shipClass == 2) return nullptr;` at end | 66.368604 | 2764/2764 | 1007/1006 | 19/92 | size matches, prologue wrong |
| guard + `if (shipClass == 2) return nullptr;` at end | 60.700000 | 2764/2752 | 1007/993 | — | |
| `missionPtr == 1 \|\| missionPtr == 23` empty-then form | 70.541480 | 2764 | 1007/1006 | 14/92 | identical CFG |
| `Player *player; player = new Player(...)` | 70.541480 | 2764 | 1007/1006 | 14/92 | identical |
| swap shipClass branches (`==1` then `==0`) | 58.630952 | 2764/2808 | 1007/1009 | 14/92 | |
| guard moved to the end (after branches) | 63.441397 | 2764/2760 | 1007/998 | — | sink lost |
| extra `{ }` around whole body | 70.541480 | 2764 | 1007/1006 | 14/92 | folded away |
| local `Array<int> dummy;` at top | 23.095823 | 2764/2852 | 1007/1028 | 8/158 | adds 5 pads |

The "no guard + `shipClass == 2`" row is the only new shape that reaches the
original size (2764 B) without the guard hack, but its prologue is
`str r0,[sp,#52]; mov r9,r3; mov fp,r2; mov r5,r1; str r5,[sp,#60]`, still
not the original's `str r1,[sp,#60]; mov r9,r0; mov fp,r3; mov r5,r2`.
It still uses `r6` for the exception pointer.

## 4. Why the scope/destructor hypotheses fail

* Extra `{}` around the body: `EmitCompoundStmt` creates a `LexicalScope`
  (`RunCleanupsScope`), but the `new Player` lpad block was already created at
  the invoke inside the same scope, so block order is unchanged (14/92).
* Early `return` in the ship branches: removes the final `ship` PHI but the
  first lpad is still the `new Player` lpad (19/90 without guard).
* Local destructor (`Array<int> dummy`): adds a *new* cleanup scope with its own
  lpad at the top of the function, moving the first lpad to index 8/158 and
  growing the body by 88 B; it does not shorten the `new Player` PHI.
* Guard at the end: the guard no longer sinks the coordinate block below
  `setEmpData`, so the schedule is lost (63.44 %).

## 5. Remaining lever

The original's `new Player` invoke must be **late in the IR block list** while
still being early in the emitted code.  The only source-level way to do that is
to make the `Player` allocation part of a statement whose IR block is emitted
after the ship branches — e.g. a single full-expression that also contains the
branch bodies — or to find a Clang 7 construct that defers the lpad block
creation.  None of the scope/early-return/destructor spellings tried here do
that.  A future pass should look at the IR block order of the original's
`new Player` (not just its code order); if it is late, the source must be a
single giant expression.

## 6. Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
python3 tools/verify/verify.py \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5Level10createShipEiiiP8Waypointbb
```

MIR (lpad block order / PHI interval):

```
<clang++> ... -c src/game/world/Level.cpp \
  -mllvm -print-machineinstrs -mllvm -filter-print-funcs=_ZN5Level10createShipEiiiP8Waypointbb
```

IR block order (first lpad index):

```
<clang++> ... -S -emit-llvm -fno-discard-value-names -c src/game/world/Level.cpp
```

Clang 7 sources used for the mechanism: `CGException.cpp:773`
(`EmitLandingPad`), `CGStmt.cpp:602` (`EmitIfStmt`), `CGCleanup.cpp:424`
(`PopCleanupBlocks`), `CodeGenFunction.h:699` (`RunCleanupsScope`).

Scratch helpers (not committed): `/dev/shm/w7670/{exact.py,apply.py,score.sh,
irorder.sh,body*.txt,mir.txt,Level.ll}`.
