# `Gun::calcCharacterCollision` pass 13: the PIC split-cost threshold, the loop-rotation
# side effect of the shipped tail order, and an `__builtin_expect` lever that reaches the
# shipped `this=r9`/`enemy=r8` allocation but not the shipped size (w4912)

Worker: w4912. Branch: `wave99-w4912`, base master `44225192c`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base `0x10000`).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`, 650 insns.
Scratch: `/dev/shm/w4912/`.

## 0. Result

**No code change landed.** The tree body (`E A B C D` tail) is still the best measured shape:
**90.0 %**, 1984/1984 B, 424/650 identical instructions (official verifier). This pass adds:

1. the exact **region-split decision arithmetic** that separates the tree from the shipped
   binary, read off the real clang-7.0.2 greedy-allocator trace (`-debug-only=regalloc`):
   the tree's `%307` split falls through to *per-block* splitting (`BestCand == NoCand`,
   `total = 110.41 > spill cost 105.03`), the shipped tail order's split picks `$r11`
   (`total = 98.31 < 105.03`) and folds the scatter block into the main interval;
2. a **new layout fact** not in any earlier note: the shipped tail order also **rotates the
   inner `for i` loop** (bottom-tested) in our PIC build, while the shipped binary and the
   tree's `E A B C D` build are top-tested. The two effects are coupled through the same
   region-split cost model;
3. an **`__builtin_expect` lever** that does reach the shipped `this=r9` / `enemy=r8`
   allocation in the shipped tail order: `__builtin_expect(distance < this->magnitude, 0)`
   in the scatter loop gives **84.8 % / 1980 B / this=r9 / enemy=r8 / explosive=[sp,#20]**.
   It is not a strict improvement (84.8 < 90.0) and the `i*3` value lands in `r5` instead of
   the shipped `r4`, but it proves the allocation is reachable from source without the
   shipped instruction stream.

| build | gate byte/linked | target pct (official) | target size |
|---|---:|---:|---:|
| before (HEAD `44225192c`) | **4500 / 4500** | **90.0 %** | **1984 B** |
| after this pass (note only) | **4500 / 4500** | **90.0 %** | **1984 B** |

Full gate after: size-exact 4524/4524, data 843/843, imports 207, lint `CLEAN ✓`, drift 0,
`RATCHET PASS`.

## 1. Shipped tail order (re-verified from the binary)

`0x153316` onward, instruction order:

```
E  movs r1,#1 ; strb.w r1,[r8,#84]                  reinterpret_cast<uint8_t*>(&enemy->field_54)[0] = 1;
D  ldrb.w r0,[r9,#240] ; cbz ; ldr r0,[r9,#56] ;
   strb.w r1,[r0,#48]                               if (this->playerGun) level->field_30 = 1;
A  ldmia/eor x3 ; blx Player::setHitVector          enemy->setHitVector(-velocity...);
C  ldr r0,[r9,#60] ; ldr r1,[pc,#916] ; str.w       this->lifetimes[i] = -1000000;
B  blx Player::getKIPlayer ; str.w r0,[r9]          this->lastHitKIPlayer = ...;
```

i.e. **`E D A C B`**. The tree emits `E A B C D`. This is the only remaining source-order
divergence in the function (every other block is in shipped order).

## 2. The region-split decision, exactly

`%307 = t2ADDri %stack.10` is the shared 48-byte stack slot (`sp+104`). It is reused by three
temporaries with disjoint lifetimes, so LLVM stack-colors them into one object and CSEs the
address into one instruction:

| use | source | trace slot |
|---|---|---|
| radius computation | `enemyPosition - player->getPosition()` (line 606/607) | `bb.21` |
| `Matrix view` | `Matrix view = *(Matrix*)CameraGetLocal(...)` (line 710) | `bb.83` |
| scatter loop | `*td = enemy->getPosition()` (line 727) | `bb.92` |

The live range is `%307 [1568r,7040B)[9360B,9424B)[11136B,11200B)[11296B,17536B)[17568B,17680B)`
in both builds. The difference is the split decision in `RAGreedy::tryRegionSplit`:

```
Cost of isolating all blocks = 105.0344503          (calcSpillCost, identical in both)
```

**Tree `E A B C D` (base.ra:44649):**

```
$r11  static = 21.32312512, v=69, total = 110.4083111   (19 bundles)
```

`110.41 > 105.03` for the only candidate ⇒ `BestCand == NoCand` ⇒ `tryRegionSplit`
returns 0 ⇒ **`tryBlockSplit`** creates one interval per use block:

```
%464 [3688r,3792r)      weight 2.636666e-02     (radius)
%465 [14680r,15056r)    weight 4.920049e-03     (Matrix view)
%466 [16056r,16160r)    weight 2.818017e-01     (scatter)
...
should evict: %57 ... weight 4.079772e-02
evicting $r4 interference: Cascade 16
unassigning %57 from $r4: R4
assigning %57 to $r9: R9
```

`%57` is `this`. The short, high-weight `%466` copy evicts it from `r4`, and the whole shipped
cascade follows (`this→r9`, `enemy→r8`, `i→fp`, `i*3→r4`, `td=[sp,#24]`, `explosive=[sp,#20]`).

**Shipped tail order `E D A C B` (tail.ra:44631):**

```
$r11  static = 21.32312512, v=69, total = 98.31180978    (24 bundles)
Split for $r11 in 24 bundles, intv 1.
splitAroundRegion with 2 globals.
%bb.92 [16000B;16480B) ... straight through.
rewr %bb.92 16064B:1 $r0 = COPY %465   (not %466)
```

`98.31 < 105.03` ⇒ `BestCand = $r11` ⇒ `doRegionSplit` ⇒ `splitAroundRegion` folds the
scatter block into the main interval. No short high-weight copy is materialised, `this` stays
in `r4`, and the function shrinks to 1912 B.

So the residual is a **single cost-model comparison**: the `$r11` region-split total must be
`>= 105.0344503` (or the per-block fallback must otherwise be selected). The total is
`static(21.323) + calcGlobalSplitCost`, and `calcGlobalSplitCost` is a sum of
`SpillPlacement` block frequencies over the live bundles. The block frequencies are identical
in both builds (same CFG, same branch probabilities); only the **bundle grouping** differs,
because it follows the block layout.

## 3. New: the shipped tail order also rotates the inner loop

`-print-after=block-placement` on the two builds shows the tail order changes the placement of
the inner `for i` header, not just the tail blocks. In the final assembly:

| build | inner-loop header | header position |
|---|---|---|
| shipped | `0x15305e` (`ldr.w r0,[r9,#8]; cmp fp,r0; bcs.w exit`) | **top-tested** |
| tree `E A B C D` | `0x15305e` (same) | **top-tested** |
| `E D A C B` (with `dmg`) | `0x1534cc` (`ldr r0,[r4,#8]; cmp r8,r0; bcc.w body`) | **bottom-tested** |

The `E D A C B` build places the loop header after the loop body (`0x1534cc`) and jumps to it
from `0x15305a` on entry; the body starts at `0x15305c` and the backedge is `0x1534d0`. The
shipped binary and the tree's `E A B C D` build keep the header at `0x15305e`, immediately
after the `i = 0` initialisation. (The inner-loop backedge is the unconditional
`b.n 0x15305e` at `0x15325e`; the conditional backedges at `0x153516`/`0x153504` are the
outer `for e` and scatter loops.)

The block-placement debug (`-debug-only=block-placement`) shows the same
`findBestLoopExit` result (`Best exiting block: %bb.11`) and the same
`Rotating loop to put exit %bb.11 at bottom` in both builds, so the divergence is downstream
of `rotateLoop` (chain stitching / `buildCFGChains`), not a different loop-exit choice.
This is consistent with the cost-model story: the same layout difference that moves the
inner-loop header also changes the SpillPlacement bundle grouping around `bb.92`.

## 4. `__builtin_expect` can reach the shipped allocation (but not the shipped size)

Because the split decision is a block-frequency sum, changing a branch probability can flip
it. Sweeping `__builtin_expect` over every conditional in the shipped-tail-order body found
three variants that reach `this=r9` / `enemy=r8`:

| body | pct | size | `this` | `enemy` | `explosive` slot | `sl_dmg` | inner loop |
|---|---:|---:|---|---|---:|---|---|
| `E D A C B` (baseline) | 64.75 | 1912 | r4 | r6 | 20 | false | bottom |
| `__builtin_expect(distance < magnitude, 0)` | **84.83** | **1980** | **r9** | **r8** | **20** | true | **top** |
| `__builtin_expect(geometries != nullptr, 0)` | 79.08 | 1984 | r9 | r8 | 24 | true | top |
| `__builtin_expect(explosive, 1)` (X block) | 78.86 | 1988 | r9 | r8 | 24 | true | top |

The `distance` variant is the closest: its tail's `E D A C B` sequence matches the shipped
instruction stream exactly (with two extra setup instructions `add.w sl,sp,#192` and
`add r4,sp,#104` hoisted into the block), it keeps `explosive=[sp,#20]`, and it restores the
top-tested loop. Its remaining differences are:

* the tree's `int dmg` local is still present (`sl_dmg = true`; shipped reloads
  `this->damage` at all three damage sites);
* `i*3` lands in `r5` and the `stack.10` temp in `r4` (shipped is the other way round);
* the function is 4 B short (1980 vs 1984; the shipped has a 2-byte `nop` before the literal
  pool at `0x1532f2`).

The allocator trace for this variant shows the eviction is now done by a different interval
(`%441 = t2ADDri %stack.1`, weight 2.949342e-02, evicting `%57` weight 2.799087e-02 from
`r4`), not by the shipped `%466` scatter copy. So the allocation is the same family but not
the same eviction event.

All 120 tail permutations (with and without `int dmg`), all 6 `X`/`I`/`S` block orders,
`dmg` at 5 anchors, and 1–3-way combinations of the expect/type knobs were measured; nothing
beats **90.0 % / 1984 B** with `this=r9`/`enemy=r8` and the shipped tail order.

## 5. Conclusion

The residual is a **PIC region-split cost-model lottery**, not a statement-order problem:

* the shipped source order is `E D A C B` (proved by the `movs r1,#1` shared between `E` and
  `D` before the `A`/`C`/`B` calls);
* compiling that order PIC makes `$r11`'s region split cost `98.31`, below the `105.03`
  per-block spill cost, so LLVM folds the scatter block into the main interval and never
  creates the short `%466` copy that evicts `this` from `r4`;
* compiling the tree's `E A B C D` order PIC makes the same cost `110.41`, above the
  threshold, so LLVM falls back to per-block splitting and reproduces the shipped allocation
  (but not the shipped tail order);
* the tail order also changes the inner-loop layout (top- vs bottom-tested), which is the
  mechanism by which the bundle grouping changes.

The remaining lever is a source construct that makes `$r11`'s `E D A C B` region-split total
`>= 105.0344503` **without changing the emitted instruction count**. `__builtin_expect` can
do it (`distance < magnitude, 0` reaches `this=r9`/`enemy=r8` at 1980 B), but every expect
that flips the split also changes the branch layout or the `i*3`/temp register assignment,
and none reaches 1984 B at > 90.0 %. The tree body stays.

## 6. Reproduction

```
cmake --build cmake-build-match --target check -j8        # RATCHET PASS
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Gun22calcCharacterCollisionEv
# -> pct=90.0 osize=1984 usize=1984 byte=False linked=False
```

Scratch harness (`/dev/shm/w4912/`):

| file | purpose |
|---|---|
| `body.txt` / `Gun.cpp.orig` | HEAD body / pristine `Gun.cpp` |
| `body_tail.txt` | HEAD body with the shipped `E D A C B` tail order |
| `try.py <body>` | splice body into `Gun.cpp`, build `link_so`, print the official-equivalent score |
| `score_so.py` | official-equivalent scorer (pct / osize / usize / matched) |
| `sig.py` | extract `{size, this, enemy, explosive_slot, sl_dmg, sl_pos, frame}` |
| `loopchk.py` | list backward branches (top- vs bottom-tested inner loop) |
| `objtry.py` | object-level compile + score (fast, no link) |
| `irgen.sh <src> <out.ll>` | exact clang-7.0.2 front-end IR |
| `extract.py` | keep only one `define` from a text `.ll` |
| `expect_sweep.py`, `combo.py`, `dist0_sweep.py`, `struct_sweep.py` | the knob sweeps in §4 |
| `base.ra`, `tail.ra`, `dist0.ra` | real `llc -debug-only=regalloc` traces quoted above |
| `mbp_head.txt`, `mbp_tail.txt`, `bfi_head.txt`, `bfi_tail.txt` | `-print-after=block-placement` layouts and `-print-machine-bfi` frequencies |

## 7. Files changed

None under `src/` (note only). Gate: byte 4500 / linked 4500, size-exact 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0.
