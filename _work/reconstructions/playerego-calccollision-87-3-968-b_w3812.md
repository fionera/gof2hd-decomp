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
