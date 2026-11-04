# PlayerEgo::calcCollision — wave 101 (wf110): the r9-restore block is what rotates the loop

**Status:** research note, 2026-11-04. Worktree `wt/wf110`, branch `wave101-wf110`,
base `3856949ab`. **No source change landed** — the checked-in 87.6 % / 968 B shape is still
the best measured. This note adds one new, *measured* root-cause result that the wave-88 note
(`playerego-calccollision-87-3-968-b_w3812.md`) did not have: the exact `hasBetterLayoutPredecessor`
conflict that forces `MachineBlockPlacement` to rotate the loop, and the register-allocation
cascade that creates the conflicting block.

Target: `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` (968 B, 87.6 %).

## TL;DR

The original loop is **not** rotated: header at `0x9f134`, latch at `0x9f38c`.
The real source shape (`v_nocheck_guard`, no artificial checks + the real `i == 0` guard) *is*
rotated (996 B, 59.8 %). The rotation is decided at the **`hitCamera → latch`** edge:

```
Selecting best successor for: %bb.39 ('')          ; hitCamera
    Not a candidate: %bb.40 ('') -> 100.00% (prob) (non-cold CFG conflict)
```

The conflicting predecessor is the shared **`r9`-restore block** `bb.33`
(`ldr r9,[sp,#24]; b latch`), which is reached from the impact path and the normalize path:

```
CONFLICT pred %bb.33 predfreq 0.5675675676 edgeprob 100.00% prededgefreq 0.5675675676 candfreq 0.8378378378
```

(`prededgefreq * 0.8 = 0.454 >= candfreq * 0.2 = 0.168`, so `BadCFGConflict = true`.)

Without `bb.33` the next-highest unplaced latch predecessor is `bb.22` at `0.072`, well under the
`0.2095` threshold, so `hitCamera → latch` would be accepted, the latch would be placed right after
`hitCamera` (exactly the original layout `… 0x2ba hitCamera / 0x2c0 latch / 0x2c6 strength<1 …`),
the chain would end at the `normalize` block (whose successor, the latch, is already placed), and
`rotateLoop` would bail — matching the original.

`bb.33` exists because the impact block's `getPosition()` temporary is allocated to **`r9`**
(`$r9 = t2ADDri $sp, 96`), which clobbers `&dockOffsetVec`. In the original that temporary is in
**`sl`** (`9f27a add.w sl, sp, #96`) because `this` is in `r5`; the original therefore restores
`r9` *inline* at three separate points (`9f362`, `9f392`, `9f44c`) and never needs a shared
restore block. So the loop rotation and the register allocation are one fixed point:
`this=r5` ⇒ impact temp in `sl` ⇒ no restore block ⇒ no rotation ⇒ `this=r5`.
The artificial-check shape (current source) is the *other* fixed point: `this=r5`, latch placed
early by the check block, no rotation — but with 18 B of extra instructions.

## Measured table (official `tools/verify/verify.py`, `autojunk=False`)

| variant | size | pct | notes |
|---|---:|---:|---|
| **checked-in** (2 redundant size checks, no bottom guard) | **968** | **87.6** | latch at `0xc6`, `this=r5 surf=r6 dock=r9 i=fp` |
| `if (i == objects->size()) continue;` as check 1 | 968 | 87.4 | latch `0xc4` |
| check 1 moved before the collision block (`p2`) | 976 | 85.6 | latch `0x2c8` |
| check 1 = `objects->size() <= i` | 976 | 85.5 | latch `0x2c8` |
| checked-in + real bottom guard (both checks kept) | 976 | 85.0 | latch `0xc6` |
| no checks + guard + check 1 only | 968 | 82.6 | `dock=fp i=r9` |
| check 1 only + real guard | 964 | 79.5 | `dock=fp i=r9` |
| check 2 only + real guard | 984 | 70.8 | |
| **real shape** (`v_nocheck_guard`) | 996 | 59.8 | **rotated**, `this=sl` |
| real shape + unscoped `Vector surface` | 968 | 64.1 | not rotated, `surf=r8 obj=r9 dock=sl` |
| real shape, fully nested `if/else` (no `continue`) | 968 | 64.1 | same codegen as unscoped |
| real shape, single shared `hitCamera()` at loop end | 992 | 60.0 | still rotated |

Every variant was built with the real toolchain (`cmake --build cmake-build-match --target link_so`)
and scored with the official verifier; sizes are the verifier's `usize`.

## Reproduction

Baseline (expect `pct 87.6 osize 968 usize 968`):

```sh
cd /home/debian/GalaxyOnFire2/wt/wf110
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j8
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE
```

`report.json` row: `{"name":"_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE","pct":87.6,"byte":false,"linked":false,"osize":968,"usize":968}`.

Machine-block-placement evidence: build an assertions LLVM 7.0.2 (`llc` only is enough), run the
NDK clang with `-mllvm -print-machineinstrs`, extract the pre-`Branch Probability Basic Block
Placement` MIR for the function, and feed it to

```
llc -run-pass=block-placement -mtriple=thumbv7-none-linux-android16 -mcpu=generic \
    -mattr=-fp-only-sp,-d16,+vfp3,-fp16,-vfp4,-fp-armv8,+neon,-crypto -float-abi=soft \
    -debug-only=block-placement -o /dev/null fn.mir
```

To see *which* predecessor trips the CFG-conflict test, add this to
`MachineBlockPlacement::hasBetterLayoutPredecessor` just before `BadCFGConflict = true`:

```cpp
DEBUG(dbgs() << "      CONFLICT pred " << getBlockName(Pred) << " predfreq ";
      MBFI->printBlockFreq(dbgs(), MBFI->getBlockFreq(Pred));
      dbgs() << " edgeprob " << MBPI->getEdgeProbability(Pred, Succ)
             << " prededgefreq ";
      MBFI->printBlockFreq(dbgs(), PredEdgeFreq);
      dbgs() << " candfreq ";
      MBFI->printBlockFreq(dbgs(), CandidateEdgeFreq); dbgs() << "\n");
```

## Proven vs. hypothesis

**Proven (measured):**

* The original loop is top-tested and **not** rotated (`0x9f134` header, `0x9f38c` latch,
  `0x9f390 b.n 0x9f134`).
* The original has exactly one `objects->size()` load per iteration (header only) and **no**
  redundant size checks; the real bottom guard `if (i == 0 && object->field_0x71)` exists
  (`0x9f374 cmp.w fp,#0 / 0x9f378 bne / 0x9f37a ldrb [r8,#113] / 0x9f37e cbz / 0x9f380 movs / 0x9f382 strb [r5,#564]`).
* The original spills `&dockOffsetVec` **once, in the preheader** (`0x9f12a str.w r9,[sp,#20]`) and
  reloads it at `0x9f362`, `0x9f392`, `0x9f44c`.
* The original's `hitCamera` is a **single shared block** at `0x9f386` reached from both the impact
  body (`0x9f2f4 b.n`) and the collision path (`0x9f330 cbz`).
* With the real shape, LLVM 7.0.2 rejects `hitCamera → latch` **only** because of the shared
  `r9`-restore block; with that block removed the conflict list is empty and the layout matches.
* The current source is a genuine local optimum of the fuzzy metric (87.6 %); ~35 source variants
  were measured (table above) and none beat it.

**Hypothesis (not proven):**

* That the original source is exactly `v_nocheck_guard` with `Vector surface` in a nested scope.
  Its IR is structurally identical to the original's block sequence, but the register allocator
  picks `this=r10` (⇒ impact temp `r9` ⇒ shared restore block ⇒ rotation), while the original has
  `this=r5`. No source spelling found so far moves `this` back to `r5` without also re-introducing
  a basic block that pins the latch early.
* That the remaining gap is entirely `MachineBlockPlacement` + the `r9` live-range split; no
  semantic/constant/struct error was found in the body (all call targets, offsets, constants and
  call order match the original).

## Gate

Unchanged: `byte_exact 4500 / linked_exact 4500 / size-exact 4524 / data_exact 843 / imports 207`,
lint `CLEAN`, drift 0, `.text 0x1337c8`. `RATCHET PASS`.
