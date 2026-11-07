# `Gun::calcCharacterCollision` wave 133 (w7739): the merge grouping is the
# `expr` form's, but LICM hoists the k-loop sret add first and steals r4
# (no source win)

Worker: w7739. Base master `144fc16b0`. Original:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`,
650 insns. TU `src/game/weapons/Gun.cpp`. Scratch: `/dev/shm/w7739/`.

## 0. Result

No code change. HEAD is still the best measured shape.

| build | gate byte/linked | target pct | size |
|---|---:|---:|---:|
| before (HEAD `144fc16b0`) | **4506 / 4506** | **93.4** | 1984 |
| after this pass (note only) | **4506 / 4506** | **93.4** | 1984 |

`check` after: `RATCHET PASS`, byte 4506/4524, linked 4506/4524, size-exact
4524/4524, data 843/843, imports 207, lint CLEAN, drift 0.

## 1. New: the `expr` form already has the shipped *merge grouping*

Wave 132 established the two complementary shapes:

| form | i*3 | pp | delta | size |
|---|---|---|---|---|
| shipped | r4 | r4 / sp+44 | r5 / sp+104 | 1984 |
| HEAD (`stmt`, named `Vector playerPosition`) | r5 | r4 / sp+104 | r5 / sp+44 | 1984 |
| `expr` (inline `getPosition()`) | r5 | r5 / sp+44 | r4 / sp+104 | 1984 |

This wave traced the **StackColoring merge** (LLVM 7
`lib/CodeGen/StackColoring.cpp`, `Merge disjoint stack slots`) on both forms.
The pass sorts frame objects by size descending then greedily merges smaller
disjoint-lifetime objects into the first compatible larger one.

`expr` lifetimes (before merge):

```
%stack.4 (delta, operator- sret)  [2880, 3360]   <- lower index
%stack.5 (pp, getPosition sret)   [2896, 3328]
%stack.10 (view)                  [14256, 14992]
%stack.11 (MatrixSetTranslation)  [14656, 14832]
%stack.12 (k-loop getPosition)    [15968, 16176]
```

Merge trace (`fi#10` = view, `fi#11` = MatrixSetTranslation):

* `fi#4` (delta) is disjoint from `fi#10` -> **delta -> fi#10**
* `fi#12` (k-loop) is disjoint from `fi#10` -> **k-loop -> fi#10**
* `fi#5` (pp) is disjoint from `fi#11` -> **pp -> fi#11**

So `expr` produces exactly the shipped grouping:
`%stack.10 = {delta, view, k-loop}` at **sp+104**,
`%stack.11 = {pp, MatrixSetTranslation}` at **sp+44**.

The `stmt` form has pp as a named local, so its alloca is `fi#4`
(`[2880, 4160]`) and delta is `fi#5` (`[3088, 3344]`). The merge then gives
**pp -> fi#10 (view, sp+104)** and **delta -> fi#11 (MST, sp+44)** — the
swapped slots.  This is the whole slot difference between HEAD and `expr`,
and it is decided purely by the *frame-index order* of pp vs delta, not by
the call sites.

## 2. New: the register swap is LICM hoist order, not the merge

`Early Machine LICM` hoists every stack-address `t2ADDri` to the e/i-loop
preheader `bb.5`.  After LICM (`-mllvm -print-after-all`, `expr`):

```
bb.5: %108 = t2ADDri %stack.3    ; enemyPosition
      %152 = t2ADDri %stack.1    ; position
      %291 = t2ADDri %stack.2    ; velocity
      %308 = t2ADDri %stack.10   ; delta/view/k-loop   (fi#10)
      %282 = t2ADDri %stack.11   ; pp/MST              (fi#11)
```

Greedy RA walks intervals in start order, so `%308` (fi#10) takes **r4** and
`%282` (fi#11) takes **r5**.  The shipped needs the opposite.  The k-loop
sret add (`%308`) is hoisted before the MatrixSetTranslation sret add
(`%282`) because the dominator-tree DFS visits the k-loop block first and
`t2ADDri` is trivially rematerializable (`IsProfitableToHoist` returns true
unconditionally for remat instructions, `MachineLICM.cpp:1218`).

Critically, the shipped has **no hoisted stack-address adds at all**: its
i-loop preheader (`0x153512`) is only `ldr/cmp/bcc`, and every `add rX, sp,
#44/#104` sits in the loop body.  With no hoist, the body order decides the
registers: `i*3` -> r4, pp (fi#11) -> r4 after `mov r6, r4` spill, delta
(fi#10) -> r5, and the view reuses delta's r5 (one `add r5, sp, #104`).

So the shipped is exactly the `expr` grouping **without LICM hoisting**.  The
blocker is that our clang driver hoists the cheap address adds; the shipped
build did not (register-pressure heuristic `CanCauseHighRegPressure`, or a
different dominator-tree visit order).

## 3. Variants measured this wave (all 1984 B)

Fast harness `/dev/shm/w7739/{try2.sh,batch*.py,score1.py,shape.py}` (0.7 s
per build+link, 0.1 s score; local score is the official normaliser).

| variant | pct | shape / notes |
|---|---:|---|
| `expr` | 92.6 | shipped slots, swapped regs |
| `stmt` (HEAD) | **93.4** | shipped regs, swapped slots |
| `expr` + `Vector enemyPos = enemy->getPosition(); *td = enemyPos; ...` | 92.6 | identical |
| `stmt` + same named k-loop temp | 93.4 | identical |
| `expr` + `enemyPosition = enemy->getPosition(); *td = enemyPosition; ...` | 77.1 | 1992 B |
| `expr` + `*td = enemy->getPosition() - position;` | 61.8 | 1956 B |
| `expr` + `Vector &d = *td; d = ...; d -= position;` | 92.6 | identical |
| `expr` + `this->targetDir = ...` | 92.6 | identical |
| `expr` + named `Vector pdiff` (delta named) | 92.6 | still delta fi#4 |
| `stmt` + named `Vector pdiff` | 93.4 | = HEAD |
| `expr` + `const Vector &playerPosition` | 93.4 | = HEAD (pp fi#4) |
| `expr` + `auto` / `Vector&&` / direct-init pp | 93.4 | = HEAD |
| `expr` + `Matrix translated = MatrixSetTranslation(...)` | 82.9 | 1988 B |
| `expr` + `view = MatrixSetTranslation(view, ...)` | 86.3 | 1992 B |
| `expr` + helper `gunVectorDistance(a, b)` | 92.6 | identical |
| `expr` + `Vector kpos;` at i-loop top (default ctor) | 59.7 | 1976 B |
| `expr` + `Vector kpos = enemyPosition;` at i-loop top | 54.7 | 1976 B |

Every shape that keeps pp as a *named local* is 93.4 with pp at sp+104;
every shape that keeps pp as a *temporary* is 92.6 with pp at sp+44.  No
source form produced shipped slots + shipped registers.

## 4. Negative result on the w7714 candidate levers

* "make the k-loop sret a frame index LICM treats as non-invariant": the
  operand of `t2ADDri` is a frame index, never a vreg, so
  `IsLoopInvariantInst` always returns true; the add is also
  `isTriviallyReMaterializable`, so `IsProfitableToHoist` returns true
  before the register-pressure check.  A named k-loop temp (written inside
  the loop) does not change this — measured 92.6/93.4, identical shape.
* "create the pp alloca before the k-loop sret in frame-index order while
  keeping sp+44": pp is already `fi#5` < k-loop `fi#12` before merge.  The
  problem is the *merged* indices (`pp -> fi#11`, k-loop -> fi#10`), which
  is forced by delta being `fi#4` (delta must precede pp to get the shipped
  grouping).  Making pp `fi#4` (named local) swaps the slots back.
* `-mllvm -avoid-speculation`, `-mllvm -hoist-cheap-insts=true/false` all
  leave the same `add r4, sp, #104` rematerialisation at `0x153198`; the
  hoist is not controlled by those flags in this build.

## 5. Open questions for the next wave

* Find a source shape that makes the i-loop address adds **not** hoisted
  (shipped has none) while keeping pp at sp+44.  The shipped is the `expr`
  grouping with a clean preheader; our build hoists `%stack.3/.1/.2/.10/.11`.
  Candidates: raise the i-loop register pressure just past the cheap-inst
  limit (without adding an alloca), or change the dominator-tree block order
  so the MatrixSetTranslation add is hoisted before the k-loop add.
* Alternatively, find a `Matrix`/`Vector` declaration detail that keeps
  `%stack.10` value live in a callee-saved register across the i-loop body
  (shipped keeps delta/view in r5, one `add r5, sp, #104`), instead of
  rematerialising it at `0x153198`/`0x153260`/`0x1533ca`/`0x1533dc`.
* The shipped k-loop is rotated (`movs r5,#0; b.n 0x1534fc`), ours is not;
  the note's earlier conclusion (rotation is a consequence of the smaller
  IR) still holds — no source lever found.

## 6. Reproduction

```
cmake --build cmake-build-match --target check -j6        # RATCHET PASS
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Gun22calcCharacterCollisionEv
```

MIR evidence: `clang++ … -mllvm -print-after-all` on the `expr` body;
`# *** IR Dump After Merge disjoint stack slots ***` (fi#4 delta -> fi#10,
fi#5 pp -> fi#11, fi#12 k-loop -> fi#10) and
`# *** IR Dump After Early Machine Loop Invariant Code Motion ***`
(`bb.5: %308 = t2ADDri %stack.10` then `%282 = t2ADDri %stack.11`).

LLVM 7 sources used for the pass semantics:
`/home/debian/llvm7build/llvm-7.0.0.src/lib/CodeGen/{StackColoring,MachineLICM}.cpp`.

## 7. Files changed

`_work/reconstructions/gun-calccharactercollision-wave133-w7739.md` only.
Gate: byte 4506 / linked 4506, size-exact 4524/4524, data 843/843, imports
207, lint CLEAN, drift 0.
