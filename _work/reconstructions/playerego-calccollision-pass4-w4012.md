# PlayerEgo::calcCollision — pass 4 (w4012): the shared `continue` block

**Status:** research note, 2026-11-03. Worktree `wt/w4012`, branch `wave90-w4012`,
base `360c7467`. No source change landed: the committed shape (artificial
`objects->size() <= i` anchor, no final `i == 0` guard) is still the best at
**87.3 % / 968 B**. This note closes the mechanism left open by pass 3
(`playerego-calccollision-pass3-w3832.md`): *why* the anchor's weight cut and
the loop rotation are the same lever, and what the original must look like.

Target: `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` (968 B, 87.3 %,
347/345 instructions).

## 1. The committed diff is exactly five things

Disassembling orig vs. the committed build and aligning the instruction
streams gives only these differences (everything else is byte-identical):

| # | diff | orig | ours |
|---|---|---|---|
| 1 | dock/i register swap | `add.w r9,r5,#328`; `mov.w fp,#0` | `add.w fp,r5,#328`; `mov.w r9,#0` |
| 2 | preheader spill | `str.w r9,[sp,#20]` at `9f12a` | missing |
| 3 | artificial anchor | absent | `ldr r0,[r4,#0]; cmp r0,r9; bls.w latch` at `9f190` |
| 4 | real final guard | `cmp.w fp,#0; bne.n 9f386` at `9f374` | absent |
| 5 | collision-block shuffles | `mov sl,r5; mov r5,r6; add r6,sp,#96; … mov r6,r5; mov r5,sl; mov r1,r6` | `add.w sl,sp,#96; … mov r1,sl` |

The literal pools are both **five** GOT words at `9f480..9f490`; only the
link-dependent GOT deltas differ (`0x17103a` vs `0x171036` for
`Globals::sound`, `0x170eb0` vs `0x170eb2` for the second `Globals::Canvas`
slot), which the verifier masks. Decoded slots:

```
0x20fd04 __stack_chk_guard   0x21015c Globals::status
0x210168 Globals::sound      0x2101f4 Globals::Canvas  (loaded twice)
```

So the whole remaining gap is the `fp`/`r9` swap plus the anchor/guard pair.

## 2. The swap is worth 5.8 points on its own

Score with the verifier's own `score()` after textually renaming `fp`↔`r9`
on **our** normalized instruction stream only:

| variant | score |
|---|---:|
| committed | 87.3 |
| committed with our `fp`/`r9` renamed | **93.1** |

(The pass-3 note quoted 93.6 % with a register-list re-sort; the raw
`score()` number is 93.1.) In other words, if the allocator put
`dockOffsetVec` in `r9` and `i` in `fp`, the remaining anchor/guard/spill
differences would cost only 6.9 points.

## 3. The anchor's weight cut and the rotation are the *same* mechanism

Pass 3 established the allocator decision:

```
%1 pull  weight 1.522203e-04   (anchor shape)
%0 dock  weight 2.394104e-04
=> %0 evicts %1 from r6, takes r6; %34 surface then evicts %0 -> fp
```

and in the faithful (`rg`) shape `pull` is 2.533436e-04 > `dock` 2.398674e-04,
so `dock` takes `r9` directly. The missing piece is why the anchor is also
what stops the rotation. It is **not** the extra block per se — it is the
register the block forces.

### 3.1 Why `rg` rotates: a shared `continue` block

With `dock = r9`, the impact block needs a scratch pointer for
`geometry->getPosition()`:

```
bb.29:  renamable $r9 = t2ADDri $sp, 96     ; impact temp
        ...
        tBLXr projectCollisionOnSurface
```

This clobbers `r9` (dock), so the impact `continue` path must restore it
before the latch. That restore is a *shared* block reached from the impact
path, the impact body and the wormhole path:

```
bb.33:  renamable $r9 = t2LDRi12 $sp, 24    ; restore dock
        successors: bb.40 (latch)
        predecessors: bb.29, bb.32, bb.20
```

`bb.33` has block frequency **63**. When MachineBlockPlacement evaluates the
`hitCamera -> latch` fallthrough it hits the CFG-conflict test
(`hasBetterLayoutPredecessor`, `PredEdgeFreq*0.8 >= CandEdgeFreq*0.2`):

```
CONFLICT bb=%bb.39 succ=%bb.40 pred=%bb.33 predfreq=63 candfreq=93
```

63 >= 93/4 = 23.25, so the edge is rejected. The chain then wanders through
the cold blocks and ends on the latch (`bottomIsSuccTop=1`) -> **rotate**.

### 3.2 Why the anchor shape does not rotate

With `dock = fp` (the anchor shape) the impact temp is `sl`
(`add.w sl,sp,#96`), `r9` is not live there, **no shared restore block
exists**, and the latch's unplaced predecessors at the `hitCamera -> latch`
test are the cold wormhole `playing`/`stop` blocks (edge frequency 4).
`4 < 115/4`, no conflict, the latch is merged early, the chain ends on the
wormhole continuation (`bottomIsSuccTop=0`) -> **bail**.

So the anchor's 50/50 branch halves the hot path, which drops `pull` below
`dock`, which moves `dock` to `fp`, which removes the shared restore block,
which removes the conflict, which stops the rotation. One quantity, five
consequences. That is the coupling pass 3 described; this is the missing
link.

### 3.3 What the original therefore is

The original has `dock = r9` **and** no rotation, so its impact temp must be
`sl`, exactly as the disassembly shows:

```
9f27a: add.w sl, sp, #96        ; impact temp, NOT r9
9f27e: ldr   r1, [r5, #8]
9f280: mov   r0, sl
9f282: blx   getPosition
```

`r9` stays live (holding dock) across the impact block, so no restore block
is emitted, the two `hitCamera()` calls tail-merge into one block (as in the
original at `9f386`), and the chain ends on the wormhole continuation.
The original is the anchor's *layout* with `rg`'s *weights*.

## 4. New negative results (this pass)

All measured with the fast proxy harness (`verify.score`, autojunk=False)
and the official verifier for the interesting ones. Base = 87.3 / 968.

### 4.1 Anchor position / condition

| variant | pct | size | this/dock/i/surf |
|---|---:|---:|---|
| committed (`size() <= i` after `outerCollide`) | **87.3** | 968 | r5/fp/r9/r6 |
| anchor before `i == 0` block ("pretop") | 59.8 | 988 | sl/r9/fp/r5 |
| anchor before `surface` | 69.3 | 960 | r5/sl/r9/fp |
| anchor before `outerCollide` | 64.0 | 1000 | sl/fp/r9/r5 |
| `size() == 0` at committed position | 65.5 | 996 | sl/fp/r9/r5 |
| `size() != 0` at committed position | 77.7 | 960 | r5/fp/r9/r6 |
| `size() == 1000000` / `size() > 1000000` | 80.6 | 968 | r5/fp/r9/r6 |
| `i + 1 == 0`, `i == 1000000`, `i > 1000000`, `i < 1000000` | 67.7–68.7 | 944–960 | r5/r6/r9/fp |
| `(i & 0x80000000u) == 0` | 69.2 | 956 | r5/r6/r9/fp |
| `size() < i + 1` | 79.1 | 964 | r5/fp/r9/r6 |
| `size() - i == 0` / `size() == i` | 79.5 | 960 | r5/fp/r9/r6 |
| `break` form (no guard / +guard) | 68.3 | 956/964 | r5/r9/sl/fp |
| `return` form (no guard / +guard) | 68.3 | 956/964 | r5/r9/sl/fp |
| `__builtin_expect(size() <= i, 0)` (+guard) | 69.3 | 988 | r5/r0/fp/r9 |
| `__builtin_expect(size() <= i, 1)` (+guard) | 62.2 | 940 | r5/r0/?/r6 |

**Why "pretop" is a no-op:** at the top of the loop body the compiler can
reuse the header's `size()` load and prove `size() <= i` false from
`i < size`; InstCombine deletes the anchor entirely (the IR jumps straight
from the null check to `icmp eq i32 %i, 0`). At the committed position the
`outerCollide` virtual call sits between the header and the anchor, so
`size()` is reloaded and the comparison survives. Any anchor that keeps the
hot path at ~96 % (i.e. `break`/`return`) moves `surf` to `fp` and `i` to
`sl`; any anchor whose branch is 50/50 or 37.5/62.5 keeps `dock = fp`.

### 4.2 Anchor inside the wormhole block

| position | pct | size | this/dock/i/surf |
|---|---:|---:|---|
| before `VectorLength(pull)` | 73.8 | 1000 | r5/r9/fp/sl |
| before `strength` / `isPlaying` | 67.8 | 1000 | r5/r9/fp/sl |
| before `if (playing)` | 72.6 | 988 | r5/sl/fp/r6 |
| before `VectorNormalize` | 76.3 | 980 | r5/fp/sl/r6 |
| before `drift =` / `drift *=` | 68.6 / 72.8 | 988/984 | r5/fp/sl/r6 or r5/sl/fp/r6 |
| before `translate` / `hitCamera` / `inWormhole` / matrix | 50.6–53.5 | 1004–1016 | r9/?/fp/? |

The "before `VectorLength`" shape is the only one that reproduces
`dock=r9, i=fp` **and** the correct preheader `str.w r9,[sp,#20]`, but it
costs 32 bytes and puts `surface` in `sl`.

### 4.3 Surface scoping / explicit temporaries

| variant | pct | size | this/dock/i/surf |
|---|---:|---:|---|
| unscoped `Vector surface` + explicit `surface = getPosition()` before `projectCollisionOnSurface` | 66.3 | 1032 | sl/r9/fp/r6 |
| unscoped `Vector surface`, temp-form collision call | 72.2 | 1000 | r5/r9/fp/sl |
| scoped surface + named collision argument | 70.2 | 976 | r5/fp/r9/? |
| `Vector pos` at loop top for the impact call (`rg`) | 50.1 | 1072 | r2/?/r9/? |
| `Vector pos` before the loop (`rg`) | 50.9 | 1012 | r9/?/sl/r5 |
| direct `this->moveTarget` (no `Vector &pull`) | **87.3** | 968 | r5/fp/r9/r6 |
| `Vector pull` copy instead of reference | 63.2 | 992 | r5/r8/?/r6 |
| `Vector &dock = this->dockOffsetVec` (loop top, const, function scope) | **87.3** | 968 | r5/fp/r9/r6 |
| `while` loop with `i++` in the body | 66.2 | 988 | r5/r0/r9/? |
| move `i == 0` block after the anchor | 69.3 | 988 | r5/r0/sl/r6 |

### 4.4 Collision comparison operand order

The original calls `AEMath::ne(collision, zero)` (`r0 = &collision`,
`r1 = &zero`), i.e. the source is `collision != Vector{0,0,0}`. The
committed source is `Vector{0,0,0} != collision`, which emits
`ne(zero, collision)` (`r0 = &zero`, `r1 = &collision`). Swapping it to the
faithful order:

| variant | pct | size |
|---|---:|---:|
| committed (`zero != collision`) | **87.3** | 968 |
| `collision != zero` | 87.0 | 968 |
| `collision != zero` + real guard | 79.1 | 976 |
| `zero != impact` (impact order swapped) | 86.4 | 968 |

The faithful operand order is *slightly worse* because the two `mov`s land
in different slots; it is not a win on its own.

## 5. Remaining work

The one shape not yet found is: **`dock = r9`, `i = fp`, `surf = r6`,
`this = r5`, with no anchor, no rotation and no shared restore block.**
Equivalently, the original's impact-block scratch pointer must be `sl`
while `dock` stays live in `r9`. In `rg` the allocator picks `r9` for the
scratch because `dock` has already been spilled there (the wormhole block's
`strength` uses `r9`). The original's allocator instead keeps `dock` in
`r9` and picks `sl` for the scratch. That is a greedy-allocator ordering
difference, not a source-CFG difference, so the next lever is the live
range / priority of `dock` vs. the impact scratch (e.g. a source shape that
makes the impact scratch's interval start before `dock`'s spill), not
another anchor placement.

## 6. Reproduction

```sh
cmake --build cmake-build-match --target link_so -j8
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE
```

Gate at this commit (unchanged): `byte_exact 4498 / linked_exact 4498 /
size-exact 4524`, `stub_zero_size 0`, `missing 0`, `wrong_type 0`, `extra 0`,
`data_exact 843/843`, imports 207, `.text 0x1337c8`, `.rodata 0x41b67`,
`.ARM.exidx 0x6f70` exact, `.ARM.extab 0x26290` (-8), lint `CLEAN ✓`,
drift 0 (`RATCHET PASS`).
