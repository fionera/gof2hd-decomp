# `Gun::calcCharacterCollision` wave 132 (w7714): LICM hoist *order* in the
# i-loop preheader is the r4/r5 tie-break (no source win)

Worker: w7714. Base master `70e451ffe`. Original:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`,
650 insns. TU `src/game/weapons/Gun.cpp`. Scratch: `/dev/shm/w7714/`
(not tracked).

## 0. Result

No code change. HEAD is still the best measured shape.

| build | gate byte/linked | target pct | size |
|---|---:|---:|---:|
| before (HEAD `70e451ffe`) | **4506 / 4506** | **93.4** | 1984 |
| after this pass (note only) | **4506 / 4506** | **93.4** | 1984 |

`check` after: `RATCHET PASS`, byte 4506/4524, linked 4506/4524, size-exact
4524/4524, data 843/843, imports 207, lint CLEAN, drift 0.

## 1. Instruction-level map of the three shapes

`i*3` = `add.w rX, fp, fp, lsl #1` (loop index × 3, used for every
`data()[i]`). `pp` = `PlayerEgo::getPosition()` sret. `delta` =
`AEMath::operator-` sret.

| form | i*3 | pp | delta | size |
|---|---|---|---|---|
| **shipped** | **r4** | **r4 / sp+44** | **r5 / sp+104** | 1984 |
| HEAD (`stmt`, named `Vector playerPosition`) | r5 | r4 / sp+104 | r5 / sp+44 | 1984 |
| `expr` (inline `enemyPosition - getPlayer()->getPosition()`) | r5 | r5 / sp+44 | r4 / sp+104 | 1984 |

The two forms are *complementary*: HEAD has the shipped **registers** but the
wrong **slots**; `expr` has the shipped **slots** but the wrong **registers**.
The shipped is exactly `expr` slots + HEAD registers, and the whole diff is
one consistent r4↔r5 swap (`mov r6, r4` vs `mov r6, r5` at `0x1530cc`,
`mov r4, r6` vs `mov r5, r6` at `0x15315a`, …).

The shipped also has two further shapes that fall out of the same codegen
decision (see §3):

* view block: **one** `add r5, sp, #104` at `0x1533c8`, reused for the
  `memcpy` dest, the `MatrixSetTranslation` arg and the `TransformSetLocal`
  arg.  Ours (`expr`) emits **three** separate adds (`0x1533ca`, `0x1533dc`,
  `0x1533ea`).
* k-loop: the shipped re-materialises `add r4, sp, #104` **inside** the
  rotated loop body at `0x153444`.  Ours reuses the delta's long-lived `r4`
  (set at `0x153198`/`0x153260`, in the i-loop body).

## 2. New: the tie-break is the *order* of the two hoisted frame-index adds

`-mllvm -print-after-all` on the `expr` body, `# *** IR Dump After Merge
disjoint stack slots ***` … `# *** IR Dump After Early Machine Loop
Invariant Code Motion ***` (dump at `expr_afterall2.txt:627213` /
`:631785`):

Before LICM:

```
bb.52 (scatter) : %114 = t2ADDri %stack.11   ; &pp
                  %115 = t2ADDri %stack.10   ; &delta
bb.72 (view)    : %275 = t2ADDri %stack.10   ; &view
                  %282 = t2ADDri %stack.11   ; &MatrixSetTranslation sret
bb.81 (k-loop)  : %308 = t2ADDri %stack.10   ; &k-loop getPosition sret
```

After `Merge disjoint stack slots`, `fi#12` (the 12-byte k-loop sret) is
`dead` and folded into `fi#10` (the 60-byte view), so `%308` becomes
`t2ADDri %stack.10`.  After `Early Machine LICM` **both** groups are hoisted
to `bb.5` (`%ir-block.36`, the i-loop preheader), in this order:

```
bb.5:  %308 = t2ADDri %stack.10     ; sp+104  (delta/view/k-loop)
       %282 = t2ADDri %stack.11     ; sp+44   (pp/MatrixSetTranslation sret)
```

Greedy RA walks intervals in start order; both start in `bb.5`, `%308`
first, so `%stack.10 -> r4` and `%stack.11 -> r5`.  `i*3` is defined later
(`bb.9`, `0x15306c`) and can only take `r5` once `pp`'s interval ends.

**The shipped needs `%stack.11` (pp) hoisted before `%stack.10`.**  The
inner-loop value (`%308`, from the k-loop) is inserted first, which is why
the k-loop temp wins `r4`.

## 3. New: `-no-stack-coloring` reproduces the shipped *shape* of both blocks

Compiling `expr` with `-mllvm -no-stack-coloring` (test-only) gives:

* view block: `add.w r10, sp, #112` at `0x38d6`, **one** add reused for
  memcpy / MatrixSetTranslation / TransformSetLocal — exactly the shipped
  shape;
* `i*3` = **r4** at `0x3596` — exactly the shipped register;
* but the frame is `sp+112` / `sp+52`, size `1948 B`, pct `59.2`.

So the shipped's view-block and register shapes are what you get when the
k-loop sret is **not** merged into the view slot.  The blocker is purely
that `Merge disjoint stack slots` is also what places both objects at
`sp+104` (see wave 131 §4).

## 4. New variants measured this wave (local score; official ≈ +0.8)

All 1984 B unless noted. `expr` baseline 92.6 (`r5, r5/44, r4/104`); HEAD
`stmt` 93.4 (`r5, r4/104, r5/44`).

| variant | pct | size | i*3 / pp / delta |
|---|---:|---:|---|
| `expr` + `const Vector &playerPosition` (pp_ref) | **93.4** | 1984 | r5 / r4/104 / r5/44 (= HEAD) |
| `expr` + `Vector &&playerPosition` (pp_rref) | **93.4** | 1984 | = HEAD |
| `expr` + named pp **and** named `Vector pdiff` (both_named) | **93.4** | 1984 | = HEAD |
| `expr` + named pp then `VectorLength(enemyPosition - pp)` | **93.4** | 1984 | = HEAD |
| `expr` + named `Vector pdiff = enemyPosition - <temp>` (delta_named) | 92.6 | 1984 | = expr |
| `expr` + named pp, then reassign `pdiff = enemyPosition - pdiff` | 84.3 | 1992 | — |
| `expr` + `Vector ep;` before k-loop (scatter scope) | 64.3 | 2004 | r0 / r4/44 / r6/104 |
| `expr` + `Vector ep = getPosition(); ep -= position; *td = ep` | 59.6 | 1892 | — |
| `expr` + `this->targetDir = ...; this->targetDir -= ...` | 92.6 | 1984 | = expr |
| `expr` + `k != size()` | 92.6 | 1984 | = expr |
| `expr` + cached `unsigned n = size()` | 66.8 | 1960 | r0 / r11/104 / r11/104 |
| `expr` + `while (k < size())` (+`++k`) | 69.1 | 1940 | — |
| `expr` + `unsigned k; while (k < size())` + `++k` at end | 92.6 | 1984 | = expr |
| `expr` + `Matrix view; view = *(...)` (copy-assign) | 67.4 | 2056 | — |
| `expr` + `*reinterpret_cast<Matrix *>(...)` | 92.6 | 1984 | = expr |
| `expr` + `const Vector position` | 92.6 | 1984 | = expr |
| `expr` + `hitPositions.data()[i] = position;` moved after k-loop | 61.5 / 60.3 | 1912 / 1968 | — |
| `expr` + local `unsigned geom = this->geometries[i];` | 92.6 | 1984 | = expr |
| `expr` + dead use of `view` after `TransformSetLocal` | 77.7 | 2008 | — |
| `expr` + cached `Array<Player*> *en = this->enemies` | 59.8 | 1952 | — |
| `expr` + k-loop writes `enemyPosition` instead of a temp | 77.1 | 1992 | — |
| `expr` + `*td = getPosition(), *td -= position;` (comma) | 92.6 | 1984 | = expr |
| `expr` + hand-rotated `do { … } while (++k < size())` | 83.1 | 1988 | — |

Every 93.4 shape is the HEAD slot layout (`pp` at `sp+104`); every 92.6
shape is the `expr` slot layout (`pp` at `sp+44`).  No shape produced the
shipped combination.

## 5. Additional observation: the shipped k-loop is rotated, ours is not

Shipped: `movs r5,#0; b.n 0x1534fc` — the condition sits at the bottom
(`0x1534fc..0x153504`, `bcc.n 0x153442`), i.e. `LoopRotate` fired.
Ours: `movs r5,#0; ldr; ldr; cmp; bcs` at the top (`0x15343e..0x153448`),
condition at the top.  A hand-written `do { … } while (++k < size())` is
semantically wrong for `size()==0` and scores `83.1 / 1988`, so the rotation
is a *consequence* of the shipped IR being smaller, not a source lever.

## 6. Open question for the next wave

Force `%stack.11` (pp / `MatrixSetTranslation` sret) to be hoisted **before**
`%stack.10` (delta/view/k-loop) in `bb.5`, or stop the k-loop sret's add
from being hoisted out of the k-loop.  Candidate levers not yet tried:

* make the k-loop sret a frame index that `Early Machine LICM` treats as
  non-invariant (e.g. a slot that is also written inside the loop) while
  `Local Stack Slot Allocation` still places it at `sp+104`;
* make the pp temporary's alloca be created *before* the k-loop sret's in
  frame-index order **and** keep it at `sp+44` (the `expr` layout), i.e.
  break the `fi#12 -> fi#10` merge without changing the offsets;
* a `Matrix`/`Vector` declaration detail that omits the `lifetime`
  markers on exactly one of the two objects (wave 131 §4(b)).

## 7. Reproduction

```
cmake --build cmake-build-match --target check -j6        # RATCHET PASS
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Gun22calcCharacterCollisionEv
```

MIR evidence: `clang++ … -mllvm -print-after-all` on the `expr` body;
`# *** IR Dump After Merge disjoint stack slots ***` (`fi#12` dead),
`# *** IR Dump After Early Machine Loop Invariant Code Motion ***`
(`bb.5: %308 = t2ADDri %stack.10` then `%282 = t2ADDri %stack.11`).

## 8. Files changed

`_work/reconstructions/gun-calccharactercollision-wave132-w7714.md` only.
Gate: byte 4506 / linked 4506, size-exact 4524/4524, data 843/843, imports
207, lint CLEAN, drift 0.
