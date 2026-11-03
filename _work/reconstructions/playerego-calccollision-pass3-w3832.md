# PlayerEgo::calcCollision — pass 3 (w3832): register-weight root cause

**Status:** research note, 2026-11-03. Worktree `wt/w3832`, branch `wave89-w3832`,
base `5710f6d0`. No source change landed: the committed shape (artificial
`objects->size() <= i` anchor, no final `i == 0` guard) is still the best at
**87.3 % / 968 B**. This note records the *register-allocation* mechanism that
wave-88 left open, plus the negative sweep run against it.

Target: `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` (968 B, 87.3 %).

## What is now established (new in this pass)

The wave-88 note said the `dock -> r9` / `i -> fp` swap was "a consequence of
the rotation". That is imprecise. `MachineBlockPlacement` runs **after**
regalloc in this pipeline (`TargetPassConfig::addMachinePasses`: RA ->
`addMachinePostRegAlloc` -> `addMachineLateOptimization` (BranchFolder,
TailDuplicate, MachineCopyPropagation) -> PostRAScheduler -> `addBlockPlacement`),
and the pre-placement MIR dump is already `NoVRegs`. So the rotation cannot
influence the allocation. Both the swap and the rotation are downstream of the
**spill weights**, which the source shape changes directly.

### The exact allocation decision

In the committed (anchor) shape the greedy allocator does:

```
%10 objects -> r4
%9  this    -> r5
%2  drift   -> r6
%1  pull    -> r6   (evicts %2; %2 -> r8)
%0  dock    -> r6   (evicts %1; %1 -> r9)      <-- the swap happens here
%34 surface -> r6   (evicts %0; %0 -> r10/fp)
%12 GOT     -> r9
%175 i      -> r9   (evicts %12)
```

`%0` (dock) has spill weight **2.394104e-04**, `%1` (pull) **1.522203e-04**.
At `%0`'s turn the first free register in the GPRnopc order is `r9`, but
`r9` has `CostPerUse = 1` (ARMRegisterInfo.td: r8–r12 are the 32-bit-only
registers). `tryEvict` is therefore called with `CostPerUseLimit = 1` and looks
for a *cheaper* (cost-0, i.e. r4–r7) register whose interval has a **lower**
spill weight. `%1` sits in `r6` with 1.52e-4 < 2.39e-4, so `%0` evicts it and
takes `r6`; `%1` is re-queued and takes `r9`; `%34` then evicts `%0` from `r6`
and `%0` falls through to `fp`.

In the faithful shape (`body_rg`, real guard, no anchor) the same decision goes
the other way:

```
%0 dock weight 2.398674e-04   %1 pull weight 2.533436e-04
$r9 is available at cost 1
assigning %0 to $r9
```

`%1` is now *heavier* than `%0`, so no eviction is legal and dock takes `r9`
directly. That is exactly the original's `dock = r9`.

### Why the anchor changes the weight

`VirtRegAuxInfo::normalizeSpillWeight` is `UseDefFreq / (Size + 25*InstrDist)`;
`UseDefFreq` is the sum of the block frequencies of the interval's uses. `pull`
is used only in the wormhole block (`if.end37`/`if.end55`). The artificial
anchor sits on the path *into* the wormhole block and has a 50/50 branch, so it
halves that block's frequency. The measured pull weights are 1.52e-4 (anchor)
vs 2.53e-4 (no anchor) — a factor 1.66, i.e. the anchor's probability cut.
Dock's weight is essentially unchanged (2.394e-4 vs 2.399e-4). The whole
dock/i swap is therefore a *branch-probability* effect of the anchor, not a
block-placement effect.

### Why the faithful shape rotates

With dock in `r9` the collision block's matrix temp clobbers `r9`, so regalloc
places a dock spill-reload at the start of the impact `continue` join
(`cleanup113`, `renamable $r9 = t2LDRi12 $sp, 24`). That makes `cleanup113`
non-empty, so BranchFolder cannot tail-merge it into the collision-end block.
In the committed shape dock lives in `fp`, is never clobbered, and the
corresponding `cleanup117` is empty and merged into `if.end148`.

The non-empty `cleanup113` is then a not-yet-placed predecessor of the latch
when `if.end146 -> cleanup147` is evaluated:

```
[CFG] BB=if.end146 Succ=cleanup147 Pred=cleanup113 cand=93 prededge=63
[CONFLICT] ... (non-cold CFG conflict)
```

so the latch edge is rejected, the chain falls through to `if.else70` and ends
on the latch (`bottom = cleanup147`, `bottomIsSuccTop = 1`) -> rotate. In the
committed shape `cleanup117` is gone, `if.end148 -> cleanup149` is a 100 %
`CFG break`, the chain continues through the latch and the wormhole fallback
and ends on `if.end55` (`bottomIsSuccTop = 0`) -> bail.

So the two problems are coupled through one quantity: the anchor's branch
probability. Fixing the weight fixes dock; fixing dock removes the reload that
makes the chain rotate.

## Negative sweep (this pass)

All measured with the fast proxy harness, then the promising ones with the
official verifier. Proxy = `verify.score` with `resolver=None`; official pct is
proxy + ~12.

| variant | size | rot | proxy | dock/surf/i | official |
|---|---:|---|--:|---|---:|
| committed (anchor, no guard) | 968 | bail | 74.9 | fp / r6 / r9 | **87.3** |
| `body_rg` (guard, no anchor) | 996 | ROT | 48.1 | r9 / r5 / fp | 59.8 |
| base + real guard | 976 | bail | 67.1 | fp / r6 / r9 | 79.4 |
| rg + `i < size` anchor before getType | 968 | bail | 69.9 | fp / r6 / r9 | 82.3 |
| base + `size <= i` **break** anchor | 956 | bail | 57.1 | r9 / fp / sl | 68.3 |
| rg + unscoped `Vector surface` | 968 | bail | 40.2 | sl / r8 / fp | 64.1 |

Anchor-form sweep (10 forms x 10 insertion points, 66 variants): every
`continue`-form anchor at the committed position (before the `getType` check)
gives `dock = fp`; `break`/`return` forms give `dock = r9` but move `surf`/`i`
to `fp`/`sl` and shrink the function to 956 B; every anchor placed *after* the
wormhole (before `stealFlagByte` / `isVisible` / `collision`) rotates.

Also neutral (968 B / bail / proxy 74.9, identical allocation): reordering the
top-guard `&&` operands, nesting the `i == 0` test, `i != size` / `int i` /
`size_t i` / pre-increment / `i += 1` / `i` declared outside the loop,
`!objects`, splitting the `dockedFlag` test, hoisting `pull`/`drift`/`dock`
references to function scope or loop-body top, a local `objects` copy/reference,
inverting the `impact != 0` or `collision != 0` tests, dropping the trailing
`continue` in the impact block, and `outerCollide(this->geometry->getPosition())`
directly.

## Remaining root cause (single most important)

The anchor is doing two jobs at once. It has to sit before the wormhole block to
stop the chain ending on the latch, but sitting there its 50/50 branch halves
`pull`'s spill weight, which makes `dock` evict `pull` from `r6` and land in
`fp` instead of `r9`. A source construct that suppresses the rotation **without
a conditional branch on the path into the wormhole block** (or one whose branch
is predicted ~100 % not-taken, so `pull`'s weight is not cut) would fix both the
`dock/i` swap and the rotation at once. None of the ~70 shapes tried here does
that.

## Reproduction

```sh
cmake --build cmake-build-match --target link_so -j8
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
```

Gate at this commit: `byte_exact 4498 / linked_exact 4498 / size-exact 4522`,
`.text 0x1337c8`, lint `CLEAN ✓`, drift 0 (`RATCHET PASS`).

## Addendum (same pass): why `this`/`surf` move too

The note above treats `dock`/`i` as the only RA difference from the original.
It is not: in the faithful (`rg`) shape `this` is evicted from `r5` to `sl` and
`surf` moves `r6 -> r5` as well. The mechanism is the same greedy-allocator
eviction, one step later:

* `rg`: `%9 this` (weight 2.340e-03) is assigned `r5`; a later split of the
  collision-block `this` value, `%232` (weight 3.185e-03), evicts it
  (`unassigning %9 from $r5`), so `%9 -> r10/sl` and `%34 surf -> r5`.
* base (anchor): the anchor halves the collision block's frequency, so the
  corresponding split is below `%9`'s weight and never evicts it; `this` stays
  `r5`, `surf` stays `r6`, exactly as in the original.

So the anchor buys *both* `this=r5`/`surf=r6` and the shared `hitCamera` block
(2 call sites like the original) at the cost of `dock=fp`. The original has
`this=r5`, `surf=r6`, a shared `hitCamera` **and** `dock=r9`, i.e. the anchor's
frequency profile without the anchor's pull-weight cut. That is the same
single-quantity contradiction described above.

Additional shapes measured this pass (fast proxy; proxy ~= official - 12):

| variant | size | rot | proxy | this/surf/dock/i |
|---|---:|---|--:|---|
| base (committed) | 968 | bail | 74.9 | r5/r6/fp/r9 |
| `rg` + shared `hitCamera` (`if(steal){impact}else{collision}`, one trailing `hitCamera()`) | 992 | ROT | 48.2 | sl/r5/r9/fp |
| shared-hitCamera + anchor | 976 | bail | 66.5 | r5/r6/fp/r9 |
| shared-hitCamera + anchor + `__builtin_expect(...,0)` | 1000 | ROT | 47.7 | sl/r5/r9/fp |
| anchor + `__builtin_expect(...,0)` (no guard) | 996 | ROT | 47.8 | r9/r5/r6/fp |
| anchor + guard + `__builtin_expect(...,0)` | 988 | bail | 57.9 | r5/r6/r0/sl |
| `break`/`return` anchor + guard | 964 | bail | 57.4 | r5/fp/r9/sl |
| anchor inside wormhole after `pull -= ...` + expect | 1000 | bail | 55.8 | r5/r6/r0/fp |
| loop-invariant anchor (`size()<1`, `size()==0`) + guard | 1004 | bail | 60.3 | r5/r6/fp/r9 |

`__builtin_expect` on the anchor does push `pull`'s weight above `dock`'s
(2.51e-4 vs 2.38e-4) and gives `dock=r9`, but the same weight change makes the
getType block hot enough that `16994` is hoisted into `r6` (`movw r6,#16994`),
which the original does not do; `i` then lands in `sl`. The `break`/`return`
anchors get `dock=r9` via the 96.9 % loop-exit branch probability but move
`surf` to `fp` and `i` to `sl`. No shape tried gives all six of
`this=r5, surf=r6, obj=r8, dock=r9, i=fp` with a 968-byte body.
