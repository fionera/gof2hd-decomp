# PlayerEgo::calcCollision analysis (w3812)

**Status:** research note, 2026-11-03. Worktree `wt/w3812`, branch `wave88-w3812`,
base `41804126`. No source change landed: the current 87.3% / 968 B shape is
still the best measured. This commit contains only this note.

Target: `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` (968 B, 87.3%).

## Ground truth

Ghidra (`decompile_function` at `0xaf0cc`, image base `0x10000`) and the Thumb
disassembly agree on the real body:

* the loop is a plain top-test `for (unsigned int i = 0; i < objects->size(); i++)`;
* the final collision block really does have a second `i == 0` guard:
  `if (i == 0 && object->field_0x71) collidesWithStationFlag = 1;`
  (`9f374 cmp.w fp,#0; 9f378 bne.n 9f386`);
* there is **no** `objects->size()` check inside the loop body. The original
  loads `objects->size()` exactly once per iteration, at the loop header
  (`9f134 ldr r0,[r4,#0]`);
* original register allocation: `r5 = this`, `r4 = objects`, `r6 = &surface`,
  `r8 = object`, `r9 = &dockOffsetVec`, `sl = distance/temp`, `fp = i`.
  `dockOffsetVec` is hoisted to the preheader (`9f110 add.w r9,r5,#328`) and
  spilled to `sp+20`; `i` stays in `fp` and is never spilled.

## Current source and why it is an anchor

The current body keeps the artificial anti-rotation check

```cpp
// Redundant bounds check: keeps the loop from being rotated so the
// register allocation matches the original.
if (objects->size() <= i)
    continue;
```

and omits the real final `i == 0` guard. The check is what keeps the machine
block placement from rotating the loop; it also forces `dockOffsetVec` into
`fp` (and `i` into `r9`) instead of the original `r9`/`fp`.

## Measurements (official verify, `autojunk=False`)

| variant | pct | size | layout / allocation |
|---|---:|---:|---|
| current (size check, no guard) | **87.3** | **968** | top-test, `dock=fp`, `i=r9` |
| no size check + real guard | 59.8 | 996 | loop rotated (header at bottom), `this=sl`, `dock=r9`, `i=fp` |
| size check + real guard | 79.4 | 976 | top-test, `dock=fp`, `i=r9` |
| real guard moved to top of collision block | 60.0 | 984 | rotated, `this=sl`, `dock=r9` |
| no size check + guard + `__builtin_expect(i==0,1)` | 70.6 | 960 | rotated, `this=r5`, `dock=r9`, but `i=sl`, temp=`fp` |
| `i >= size`, `size < i`, `size-1 < i`, signed compare | 87.3 | 968 | same as current |
| `size < i+1`, `i+1 > size` | 79.1 | 964 | different RA |
| `size == i` / `size-1 < i` | 79.5 | 960-964 | bottom-test duplicate |
| `break` form | 68.3 | 956 | duplicated epilogue |
| null/outerCollide/i==0 branch hints | 45-60 | 960-1024 | rotates or changes RA |
| per-TU `-mllvm -likely-branch-weight=80/-unlikely=1` | 59.8 | 996 | no effect |
| per-TU `-mllvm -force-precise-rotation-cost` | 43.2 | 992 | worse |
| per-TU `-mllvm -disable-block-placement` | 39.3 | 992 | worse |

## Root cause

The remaining gap is **machine block placement**, not the source expression
itself. With the real guard and no artificial size check the IR is already the
original shape (same blocks/branch probabilities as the original), but
`MachineBlockPlacement` rotates the loop: it lays the body out first, the latch
second, and the header last, so the prologue branches to the header and the
header's fallthrough is the exit. That rotation changes the whole register
allocation (`this` moves to `sl`, the `surface` temp to `fp`, `i` to `sl`/`fp`)
and costs 28 B.

The current size check adds one block to the loop body; that is enough to make
the placement keep the header at the top (matching the original layout), but it
also pins `dockOffsetVec` to `fp` and `i` to `r9`. Every other extra block tried
(a redundant `miningGame` compare, a second null check, a `break`, etc.) also
keeps the loop unrotated but leaves the same `dock=fp`/`i=r9` allocation.

So the original must contain a source-shape detail that suppresses the
rotation *without* adding a basic block. It is not any of the branch hints,
check forms, `-mllvm` block-placement knobs, or per-TU branch weights tried
here. The next pass should look at the exact `pull`/`drift`/`surface` scoping
and declaration order (the `Vector &pull`/`Vector &drift` hoisting was tried
and is neutral) rather than at flags.

## Reproduction

```sh
cmake --preset default
cmake --build cmake-build-match --target link_so -j8
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE
```

Gate at this commit: `byte_exact 4498 / linked_exact 4498 / size-exact 4524`,
`.text 0x1337c8`, lint `CLEAN ✓`, drift 0.

## Second pass (2026-11-03, w3812): MBP chain evidence and variant sweep

Still no code change: base (size check, no final guard) remains the best at
**87.3 % / 968 B**. This section records the mechanism and the negative results
so the next pass does not repeat them.

### Fresh `[ROT]` evidence (patched LLVM 7.0.1, `-debug-only=block-placement`)

Base (artificial `objects->size() <= i` check):

```
[ROT] top=%bb.4 ('for.cond') bottom=%bb.21 ('if.end55') exit=%bb.4 ('for.cond')
[ROT] bottomsucc=%bb.38 ('cleanup149') inloop=1 chainbegin=%bb.4 ('for.cond')
[ROT] next=%bb.5 ('for.body') exitIsSuccNext=1 bottomIsSuccTop=0
[ROT] bail fallthrough-trade
```

`rg` (real final guard, no size check):

```
[ROT] top=%bb.4 ('for.cond') bottom=%bb.40 ('cleanup147') exit=%bb.4 ('for.cond')
[ROT] bottomsucc=%bb.4 ('for.cond') inloop=1 chainbegin=%bb.4 ('for.cond')
[ROT] next=%bb.5 ('for.body') exitIsSuccNext=1 bottomIsSuccTop=1
Rotating loop to put exit %bb.4 ('for.cond') at bottom
```

So the whole difference is `Bottom->isSuccessor(Top)`:
* base: the chain ends at `if.end55` (the `pull = VectorNormalize(pull)` merge),
  whose successor is the latch `cleanup149`, not the header -> bail;
* rg: the chain ends at the latch `cleanup147`, whose successor *is* the header
  -> rotate.

### Why the chain ends in different places

The chain is built by `selectBestSuccessor`; when the latch's only successor
(the header) is already in the chain, `buildChain` falls back to
`selectBestCandidateBlock` and appends an arbitrary unplaced block. The
divergence is one CFG-conflict test:

Base — `if.end148 -> cleanup149` is accepted:

```
[CFG] BB=%bb.37 ('if.end148') Succ=%bb.38 ('cleanup149') Pred=%bb.22 ('if.else74') cand=115 prededge=4
[CFG] BB=%bb.37 ('if.end148') Succ=%bb.38 ('cleanup149') Pred=%bb.23 ('if.then76') cand=115 prededge=8
[CFG] BB=%bb.37 ('if.end148') Succ=%bb.38 ('cleanup149') Pred=%bb.21 ('if.end55') cand=115 prededge=21
Candidate: %bb.38 ('cleanup149'), probability: 100.00% (CFG break)
```

rg — the same edge is rejected because the impact `lifetime.end` block is a
not-yet-placed predecessor of the latch:

```
[CFG] BB=%bb.39 ('if.end146') Succ=%bb.40 ('cleanup147') Pred=%bb.21 ('if.else70') cand=93 prededge=4
[CFG] BB=%bb.39 ('if.end146') Succ=%bb.40 ('cleanup147') Pred=%bb.22 ('if.then72') cand=93 prededge=8
[CFG] BB=%bb.39 ('if.end146') Succ=%bb.40 ('cleanup147') Pred=%bb.33 ('cleanup113') cand=93 prededge=63
[CONFLICT] ... Pred=%bb.33 ('cleanup113') cand=93 prededge=63 (non-cold CFG conflict)
```

`cleanup113` is the `continue` target of the `Vector impact` scope
(`llvm.lifetime.end` of the impact temp); in the base IR the corresponding
block (`<label>:212` / MIR `bb.36.cleanup117`) has already been merged into the
chain when `if.end148 -> cleanup149` is evaluated, so it is skipped by
`hasBetterLayoutPredecessor`. That single ordering difference is the whole
rotation.

### Source-shape sweep (fast proxy, then official verify for the best)

`size`/`frame`/RA from the fast harness; pct from `tools/verify/verify.py`.

| variant | size | frame | rot | this | surf | obj | dock | i | pct |
|---|---:|---:|:-:|---|---|---|---|---:|---:|
| base (size check, no guard) | 968 | 112 | 0 | r5 | r6 | r8 | fp | r9 | **87.3** |
| `rg` (real guard, no size check) | 996 | 112 | 1 | sl | r5 | r8 | r9 | fp | 59.8 |
| `rg` + unscoped `Vector surface` ("S") | 968 | 120 | 0 | r5 | r6 | r9 | sl | fp | 64.1 |
| `rg` + fully nested `if`/`else if` (no `continue`) | 968 | 120 | 0 | r5 | r6 | r9 | sl | fp | 64.1 |
| base + real guard (both) | 976 | 112 | 0 | r5 | r6 | r8 | fp | r9 | 79.4 |
| `size == i` (4-byte no-op check) | 960 | 112 | 0 | r5 | r6 | r8 | fp | r9 | 79.5 |
| `rg` + top guard `... && field_0x71 && i == 0` | 960 | 112 | 0 | r5 | fp | r8 | r9 | sl | 70.3 |
| `rg` + top guard `i, alien, flag, len` | 988 | 112 | 0 | sl | r5 | r8 | r0 | fp | 59.4 |
| `rg` + top guard `i, len, alien, flag` | 984 | 112 | 0 | sl | r5 | r8 | fp | r9 | 55.7 |

Scoping variants that were tried and all collapse to one of the two shapes
above: `surface` scoped/plain/`Vector s; s = ...`, `collision` scoped/plain,
`impact`/`velocity`/`position` scoped/plain or hoisted to the loop-body top,
`pull`/`drift` hoisted to the body top or to the preheader, `collision`/
`surface`/`impact` declared before the loop, explicit trailing `continue`,
`hitCamera` inside/outside the collision `if`, `for (i != size)`, `int i`,
`i` declared outside the loop, braces around `continue`, positive/negative
`outerCollide`/`isVisible` forms, and every nesting of the top guard.

### Register-allocation order (NDK clang 7.0.2, `-debug-only=regalloc`)

Base assignment order (priority order, with evictions):

```
%10 objects   -> r4
%9  this      -> r5
%2  &moveDirection -> r8 (after evicting from r6)
%1  &moveTarget    -> r9 (after evicting from r6)
%0  &dockOffsetVec -> r10 (fp)
%34 &surface       -> r6
%75 position temp  -> r11
%175 i             -> r9 (evicts the stack-canary pointer)
```

The original wants `i -> fp` and `&dockOffsetVec -> r9`, i.e. exactly the
opposite of the base. `i` has the highest weight (8.39e-03) but is popped late
because the loop-carried `rGPR` interval is processed after the `GPRnopc`
address intervals; with the real guard the same order yields `i -> fp`,
`&dockOffsetVec -> r9` (rg), so the RA swap is a *consequence* of the rotation
(and of the extra guard use), not an independent lever.

### Conclusion

The base is a genuine local optimum of the fuzzy metric: its byte layout is the
only one that reproduces the original's block order (header at top, latch, then
the wormhole `else`/`play`/`normalize` blocks), so every intra-function branch
target matches. Adding the real guard costs 6 bytes at the end and shifts the
epilogue, which alone drops the score to 79.4 % even though the guard
instructions themselves match.

To beat 87.3 % one needs a source construct that (a) keeps the chain ending at a
non-latch block *without* adding a block that changes the frame/RA, and (b)
adds no instructions. No such construct was found among ~150 variants. The
remaining gap is MachineBlockPlacement chain order, not source semantics.

Gate at this commit: `byte_exact 4498 / linked_exact 4498 / size-exact 4524`,
`.text 0x1337c8`, lint `CLEAN ✓`, drift 0 (`RATCHET PASS`).
