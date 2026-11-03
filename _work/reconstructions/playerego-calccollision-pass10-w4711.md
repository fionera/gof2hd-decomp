# PlayerEgo::calcCollision — pass 10 (w4711): the *wrap* form, and why the
# loop rotates

**Status:** note only, 2026-11-03, wave 97. Worktree `wt/w4711`,
branch `wave97-w4711`, base master `9b58a39d5`.

Target: `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` (orig 968 B,
347 instructions).

The landed source is unchanged (still the pass-9 two-anchor shape,
87.6 % / 968 B).  This pass found a **better structural match** than the
committed shape — a single `if (i < objects->size()) { … }` *wrap* around
the rest of the loop body reproduces the original's exact basic-block
layout at the original 968 B and is 97.7 % mnemonic-identical — but it
flips the `dock`/`i` register pair (`dock=fp, i=r9`), which costs more
score than the layout gains (87.3 vs 87.6).  Restoring `dock=r9` needs the
pass-9 second anchor and pushes the function to 976 B (85.5–86.2).  So no
strict improvement could be landed; the analysis is recorded here.

| | score | size | insns | mnemonic¹ | this | dock | i | surface | canary |
|---|---:|---:|---:|---:|---|---|---|---|---|
| original | 100 | 968 | 347 | — | r5 | r9 | fp | r6 | `[sp,#24]` |
| committed (pass 9) | **87.6** | 968 | 349 | 94.1 % | r5 | r9 | fp | r6 | `[sp,#24]` |
| faithful (no anchors) | 60.1 | 996 | 352 | 84.7 % | sl | r9 | fp | r5 | `[sp,#20]` (`strd`) |
| **wrap after `outerCollide`** | 87.3 | **968** | 345 | **97.7 %** | r5 | **fp** | **r9** | r6 | `[sp,#24]` |
| wrap + second anchor | 85.5 | 976 | 348 | 97.3 % | r5 | r9 | fp | r6 | `[sp,#24]` |
| wrap in collision block + anchor | 85.9 | 976 | 349 | 97.2 % | r5 | r9 | fp | r6 | `[sp,#24]` |
| `isVisible() && i < size()` + anchor | **86.2** | 976 | 349 | 97.2 % | r5 | r9 | fp | r6 | `[sp,#24]` |

¹ mnemonic ratio = `difflib.SequenceMatcher` over the instruction stream with
all register names and immediates replaced by placeholders (so it measures
instruction *selection* and *order* only, not allocation or addresses).
The gate is unchanged either way: `byte_exact 4500/4524`,
`linked_exact 4500/4524`, size-exact 4524/4524, `data_exact 843/843`,
imports 207, lint CLEAN, drift 0, `.text 0x1337c8`, `.rodata 0x41b67`,
`.ARM.exidx 0x6f70`, `.ARM.extab 0x26294 (−4)`.

---

## 1. Where the loop actually gets rotated (corrects pass 9 §2)

Pass 9 attributed the layout difference to the *fall-through direction* of
the anchor branch.  That is right about the anchor, but the underlying
mechanism is `MachineBlockPlacement`'s **`rotateLoop`**, not
`findBestLoopTop`:

* `findBestLoopTop()` bails at `-Oz` (`if (F->getFunction().optForSize())
  return L.getHeader();`, `MachineBlockPlacement.cpp:1801`), so it never
  rotates.
* `rotateLoop()` (`MachineBlockPlacement.cpp:1977`) is still called
  (`buildLoopChains` → `rotateLoop(LoopChain, PreferredLoopExit, …)`) and
  rotates the chain so the *selected exit block* sits at the bottom.

Verified directly from the NDK clang 7.0.2 `-mllvm -print-after-all` MIR
dumps (the pass prints as *“Branch Probability Basic Block Placement”*):

* **faithful body** — layout order is
  `bb.0 bb.1 bb.2 bb.3 | bb.5 … bb.40 | bb.4 bb.41`, i.e. the loop header
  `bb.4` is the *last* loop block and the preheader ends in `t2B %bb.4`.
  The loop is rotated; the header is at the bottom.
* **committed (anchor) body** — layout order is
  `bb.0 bb.1 bb.2 bb.3 bb.4 bb.5 … bb.12 bb.39 bb.13 … bb.38 bb.23 bb.24
  bb.21 bb.22 bb.40`, i.e. the header `bb.4` is at the top and the latch
  `bb.39` is spliced in after the anchor block `bb.12`.  Not rotated.
* **wrap body** — header at the top, latch (`add.w fp,fp,#1; b.n 0x64`)
  at the *end*, right after `hitCamera`, exactly like the original
  (`0x9f38c: add.w fp,fp,#1; b.n 0x9f134`).

So the original is an **un-rotated** loop with the latch after
`hitCamera`, and the faithful source shape is rotated away from it.  The
anchor accidentally suppresses the rotation.

## 2. The wrap form reproduces the original block layout at 968 B

Replacing the pass-9 anchors with a single redundant wrap around the rest
of the loop body (inserted right after the `outerCollide` block) gives the
original's block layout *and* the original canary codegen:

```cpp
        {
            Vector surface = this->geometry->getPosition();
            if (!object->outerCollide(surface))
                continue;
        }
        if (i < objects->size()) {          // <-- the wrap (always true)
        if (object->getType() == 16994 && object->isVisible()) {
            … unchanged body …
        }
        this->hitCamera();
        }
    }
```

Result: 968 B, 345 insns, `this=r5 dock=fp i=r9 surface=r6
canary=[sp,#24]`, score 87.3, **mnemonic ratio 97.7 %** (the highest of
any shape measured, including the committed one at 94.1 %).

Why the wrap works: `if (cond) continue;` emits `br i1 %cond, label
%latch, label %body`, so the latch is the *then* block and MBP lays it out
as the fall-through.  Writing the same test as `if (cond) { body }`
inverts the successor order (`then` = body), so the latch is no longer a
fall-through and MBP leaves it at the end of the loop chain.

The wrap costs 8 B of code in the collision-block placement
(`ldr r0,[r4,#0]; cmp fp,r0; bcs.n`) and 10 B in the after-`outerCollide`
placement (`ldr r0,[r4,#0]; cmp fp,r0; bcs.w`).

The remaining diff of the wrap body vs the original is then only:

1. the wrap itself (3 instructions),
2. the missing preheader `str.w r9,[sp,#20]` (the wrap body spills `dock`
   at the two use sites instead — one extra store),
3. the collision-block register shuffle
   (`mov sl,r5; mov r5,r6; add r6,sp,#96; … mov r6,r5; mov r5,sl` vs our
   single `add.w sl,sp,#96`),
4. the missing real final guard `if (i == 0 && object->field_0x71)`,
5. `dock`/`i` swapped.

## 3. The `dock`/`i` flip is caused by the extra block, not by the `i` use

Pass 8/9 explained the flip as “the anchor's hot use of `i` raises `i`'s
spill weight above `dock`'s”.  That is not the whole story: a wrap whose
condition does **not** mention `i` flips `dock`/`i` just the same.

Measured on the wrap-after-`outerCollide` body (all 968 B unless noted):

| wrap condition | score | size | dock | i | canary |
|---|---:|---:|---|---|---|
| `i < objects->size()` | 87.3 | 968 | fp | r9 | `[sp,#24]` |
| `objects->size() > i` | 87.3 | 968 | fp | r9 | `[sp,#24]` |
| `object->field_0x71 != 2` | 79.7 | 964 | fp | r9 | `[sp,#24]` |
| `object->stealFlagByte != 2` | 79.7 | 964 | fp | r9 | `[sp,#24]` |
| `this->dockedFlag != 2` | 79.7 | 964 | fp | r9 | `[sp,#24]` |
| `this->inWormhole != 2` | 79.7 | 964 | fp | r9 | `[sp,#24]` |
| `this->boostingFlag != 2` | 79.7 | 964 | fp | r9 | `[sp,#24]` |
| `i != objects->size()` | 79.5 | 960 | fp | r9 | `[sp,#24]` |
| `i <= objects->size() - 1` | 79.5 | 964 | fp | r9 | `[sp,#24]` |
| `Globals::status != 0` | 66.2 | 964 | r6 | r9 | `[sp,#20]` |
| `i < 0xFFFFFFFFu` | 67.9 | 944 | r6 | r9 | `[sp,#24]` |

So: **any** extra hot block on the loop-entry path flips `dock`→`fp`,
`i`→`r9`; only the pass-9 second anchor (which halves the frequency of the
block that contains the final `i == 0` guard) buys `r9` back for `dock`.
Loop-index type (`int`, `unsigned`, `size_t`, `uint32_t`), a local
`Vector &dockRef = this->dockOffsetVec`, and moving the wrap before/after
the `i == 0` guard are all codegen-identical.

## 4. Restoring `dock=r9` costs 8 B too many

The second anchor (`if (objects->size() == i) continue;` after the
wormhole `isShrinking() || miningGame` check) restores `dock=r9, i=fp` but
costs 10 B; with the wrap the function lands at 976 B:

| shape | score | size | dock/i |
|---|---:|---:|---|
| wrap after `outerCollide` + anchor2 | 85.5 | 976 | r9/fp |
| wrap in collision block (before `isVisible`) + anchor2 | 85.9 | 976 | r9/fp |
| `if (object->isVisible() && i < objects->size())` + anchor2 | **86.2** | 976 | r9/fp |
| same + real guard `i == 0 &&` | 85.5 | 980 | r9/fp |
| same + correct `collision != Vector{0,0,0}` order | 85.9 | 976 | r9/fp |

Every anchor2 relocation tried (after the wrap, before the guard, in the
`stealFlag` block, before the wrap) destroys the allocation (56–66 %,
984–1012 B); anchor2 must stay in the wormhole block.  Anchor2 forms
`==`, `<=`, `>=`, `size - i < 1` are all byte-identical (85.9/976).
Merging it into the `||` chain is also byte-identical.

To reach 968 B from 976 B one would have to remove 8 B, but the only
removable bytes are the two duplicate `str.w r9,[sp,#20]` dock spills
(4 B each) — and those disappear only if the allocator spills `dock` in
the preheader, which is exactly the allocation decision that is wrong.
There is no source lever for it (see §3).

## 5. Negative results (this pass)

* Adding the wrap on top of the pass-9 anchors: `committed + wrap` =
  70.1/996 (the wrap's extra block re-flips the allocation through `r0`).
* `committed + wrap` without anchor2 = 71.1/988.
* `committed + wrap` + real guard = 69.8/1004.
* Moving the `i == 0` guard to the end of the loop body: 53.8/872.
* Moving the `i == 0` guard after `outerCollide`: 73.2/976.
* Merging anchor2 into the `isShrinking() || miningGame` `||` chain:
  byte-identical to the committed shape (87.6/968).
* Anchor1 placements on the committed body (continue form):
  before `isVisible` 85.6/976, before `stealFlag` 79.2/980,
  before the `i == 0` guard 70.5/992, before the surface block 68.9/992,
  before the final `hitCamera` 62.1/996.
* `this->miningGame != -1` / `object->isVisible()` as the wrap condition:
  65.4/1000 (breaks `this`/`surface`).

## 6. Conclusion

The pass-9 two-anchor shape (87.6 / 968 B) remains the best *score*.
The wrap shape is the best *structure*: it is the only shape found that
reproduces the original's un-rotated block layout (header at `0x64`,
latch after `hitCamera` at `0x2c0`/`0x2c6`) together with the original
canary spill, and it is 97.7 % mnemonic-identical.  It fails only on the
`dock`/`i` register pair, and the pass-9 second anchor that fixes that
pair costs 8 B more than the original function has.

The remaining work is therefore still: **find a source shape that keeps
the un-rotated layout and the canary spill while leaving `dock` in `r9`
without an extra hot block** — i.e. a construct that suppresses
`rotateLoop` without adding a block on the loop-entry path.

## 7. Reproduction

```sh
cmake --build cmake-build-match --target check -j8       # RATCHET PASS
python3 /dev/shm/w4711/fastscore.py                      # score/size/insns
python3 /dev/shm/w4711/analyze.py \
  cmake-build-match/CMakeFiles/gof2.dir/src/game/ship/PlayerEgo.cpp.obj v
```

Scratch harnesses (throw-away, `/dev/shm/w4711`): `q.sh` splices a body
into `src/game/ship/PlayerEgo.cpp`, rebuilds `link_so` and prints the
official `verify.score` plus the register signature; `mnem.py` prints the
register/immediate-normalised mnemonic diff; `dump3.py` prints the
original and current disassembly; `pp.sh -print-after-all` regenerates the
NDK clang MIR dump (the MBP section is the one headed *“IR Dump After
Branch Probability Basic Block Placement”*); `wbatch*.py`, `abatch*.py`,
`cbatch.py` … `mbatch.py` are the variant sweeps tabulated above.
