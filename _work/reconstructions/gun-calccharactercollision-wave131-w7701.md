# `Gun::calcCharacterCollision` wave 131 (w7701): the `r4`/`r5` swap is a
# StackColoring-merge + LICM-hoist effect, not a source COPY hint

Worker: w7701. Base master `7c6933c2d`. Original:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`,
650 insns. Scratch: `/dev/shm/w7701/` (not tracked).

## 0. Result

No code change. HEAD is still the best measured shape.

| build | gate byte/linked | target pct | size |
|---|---:|---:|---:|
| before (HEAD `7c6933c2d`) | **4506 / 4506** | **93.4** | 1984 |
| after this pass (note only) | **4506 / 4506** | **93.4** | 1984 |

`check` after: `RATCHET PASS`, byte 4506/4524, linked 4506/4524, size-exact
4524/4524, data 843/843, imports 207, lint CLEAN, drift 0.

## 1. New finding: the `%513 = COPY` is downstream of a *frame-index merge*,
## not the primary tie-break

Wave 128/129 attributed the residual to the long-lived `%513 = COPY %498`
of `&stack.10`. This wave traced the value through the **pre-RA** MIR and
found the COPY is only the *symptom*. The causal chain is:

1. **StackColoring merges the k-loop temp into the view.**
   In the first machine IR (`-print-after-all`, `IsSSA`) the view is
   `%stack.10` (fi#10, 60 B) and the k-loop `Player::getPosition()` sret is
   a *separate* `%stack.12` (fi#12, 12 B).  The `Merge disjoint stack slots`
   pass (dump at `# *** IR Dump After Merge disjoint stack slots ***`)
   merges fi#12 into fi#10 because their `lifetime` ranges are disjoint:

   ```
   view       LIFETIME_START %stack.10  14256B ... LIFETIME_END %stack.10  14992B
   k-loop tmp LIFETIME_START %stack.12  15968B ... LIFETIME_END %stack.12  16176B
   ```

   After the pass `fi#12: dead` and the k-loop sret is `%stack.10`.

2. **LICM then CSEs the two `t2ADDri %stack.10` and hoists one to the
   i-loop preheader.**  Before `Early Machine Loop Invariant Code Motion`
   there are two independent adds:

   ```
   view block : %275 = t2ADDri %stack.10
   k-loop     : %308 = t2ADDri %stack.10
   ```

   After LICM the k-loop add is hoisted to `bb.8` (the i-loop preheader,
   `%ir-block.36`, 1568B) and `%275` disappears:

   ```
   bb.8: %308 = t2ADDri %stack.10      ; live from the preheader
   ```

3. **The hoisted interval starts before `i*3`.**  `i*3` is materialised in
   the i-loop body (`add.w r5, r11, r11, lsl #1` at 0x356c).  Because
   `%308` is already live at `bb.8`, Greedy RA gives it `r4`; `i*3` is
   pushed to `r5`.  The post-RA MIR (`# *** IR Dump After Virtual Register
   Rewriter ***`) shows the fixed assignment:

   ```
   %stack.11 (sp+44, pp / MatrixSetTranslation sret) -> r5
   %stack.10 (sp+104, view / delta / k-loop tmp)     -> r4
   ```

   The shipped wants the opposite: `%stack.11 -> r4`, `%stack.10 -> r5`.

## 2. Direct confirmation: disabling the merge flips `i*3` to `r4`

Compiling the `expr` body with `-mllvm -no-stack-coloring` (test-only flag,
not a build change) prevents the fi#12→fi#10 merge.  The result:

```
3596: add.w r4, r10, r10, lsl #1     ; i*3 = r4  (shipped register!)
```

i.e. the register swap is exactly the merge/hoist, not a COPY hint.  The
drawback is the frame layout: with no stack sharing the view sits at
`sp+112` and the sret at `sp+52`, size `1948 B`, pct `59.2`.  The shipped
needs the merge to *place* both at `sp+104` but must not let LICM CSE them.

`-mllvm -disable-ssc` (StackSlotColoring, spill slots) does **not** prevent
the alloca merge (`fi#12` still dead); `-mllvm -disable-machine-licm`
changes size to `1928 B`; `-mllvm -disable-postra-machine-licm` is
identical to baseline.

## 3. Source shapes tried this wave

Fast harness `/dev/shm/w7701/{compile.sh,build_score2.sh,score.py,score2.py,
variants2.py}`; local score is the official normaliser (≈2 pts below the
official `verify` number).  Format: `pct(local) / size / i3 / pp / delta`.

| variant | pct | size | i3 | pp | delta | notes |
|---|---:|---:|---|---|---|---|
| `expr` baseline | 92.6 | 1984 | r5 | r5/44 | r4/104 | shipped slots, swapped regs |
| `stmt` (HEAD) | 93.4 | 1984 | r5 | r4/104 | r5/44 | shipped regs, swapped slots |
| `expr` + `Vector ep;` at i-loop top | 54.8 | 1976 | **r4** | — | — | default ctor adds 3 stores, shifts frame |
| `expr` + `Vector ep` at i-loop top, `ep = enemyPosition` | 54.6 | 1976 | — | — | — | copy not eliminated |
| `expr` + `Vector ep` at i-loop top, `ep = position` | 54.4 | 1976 | — | — | — | copy not eliminated |
| `expr` + `Vector ep(0,0,0)` at i-loop top | 54.8 | 1976 | — | — | — | ctor not eliminated |
| `expr` + `Matrix view;` at i-loop top (assign in branch) | 53.6 | 2024 | r4 | — | — | `Matrix()` ctor 15 stores |
| `expr` + `Matrix view` hoisted to i-loop top (uncond. camera) | 51.7 | 1900 | — | — | — | camera call not sunk |
| `expr` + conditional `Matrix view = geometries ? ... : Matrix()` | 48.0 | 2044 | — | — | — | branch + ctor |
| `expr` + `alignas(8) Vector ep` in k-loop | 92.6 | 1984 | r5 | r5/44 | r4/104 | identical (still merged) |
| `expr` + `alignas(16/32) Vector ep` in k-loop | 71.0 | 1996 | r5 | — | — | frame grows, no flip |
| `expr` + `Matrix view(...)` direct-init | 92.6 | 1984 | r5 | r5/44 | r4/104 | identical |
| `expr` + `Matrix view = Matrix(...)` | 92.6 | 1984 | r5 | r5/44 | r4/104 | identical |
| `expr` + `Matrix *view` (pointer, no copy) | 80.0 | 1972 | r5 | r5/104 | r4/44 | drops 60-byte memcpy |
| `expr` + `volatile Matrix *p = &view` escape | 92.6 | 1984 | r5 | r5/44 | r4/104 | escape eliminated |
| `expr` + `Vector ep` at scatter block top | 64.3 | 2004 | r0 | r4/44 | r6/104 | extra alloca |
| `expr` + `Vector ep` inside k-loop body | 64.8 | 1972 | r0 | r8/104 | r8/104 | extra alloca |

The only shapes that flip `i*3` to `r4` are ones that add a new alloca
(`Vector ep` at the i-loop top) or disable the merge; both shift the frame
and lose the shipped `sp+44`/`sp+104` slots.

## 4. Why the obvious source fix cannot work

The shipped simultaneously needs:

* view and k-loop temp at the **same offset** `sp+104` (so the merge /
  `LocalStackSlotAllocation` reuse must happen), and
* the two `&stack.10` computations **not CSE'd by LICM** (so the view
  address stays short-lived and `i*3` wins `r4`).

With the current IR the two requirements are mutually exclusive: the same
frame index is what lets `Merge disjoint stack slots` put both at `sp+104`,
and the same frame index is what lets LICM CSE `%275` into `%308`.  The
shipped must therefore have had either (a) a different `Matrix`/`Vector`
alloca layout that made `LocalStackSlotAllocation` reuse `sp+104` *after*
RA without StackColoring merging the frame indices, or (b) a frontend
difference that omitted the `lifetime` markers for one of the two objects.

## 5. Open question for the next wave

Find a source shape (or a `Matrix`/`Vector` declaration detail) that keeps
fi#12 and fi#10 as **distinct frame indices** through `Merge disjoint stack
slots` while still letting `Local Stack Slot Allocation` place both at
`sp+104`.  Candidate levers not yet tried:

* make the k-loop temp's `lifetime.start` overlap the view's (e.g. a
  non-trivial `Vector` wrapper whose ctor is free) without adding stores;
* make the view's `lifetime.end` extend past the k-loop without a default
  `Matrix()` ctor (a `Matrix` copy ctor from a local, not a fresh ctor);
* give the two objects different `align` so `Merge disjoint stack slots`
  refuses the merge but `Local Stack Slot Allocation` still reuses the
  offset.

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
`# *** IR Dump After Merge disjoint stack slots ***` (fi#12 → fi#10) and
`# *** IR Dump After Early Machine Loop Invariant Code Motion ***`
(`%308` hoisted to `bb.8`).

## 7. Files changed

`_work/reconstructions/gun-calccharactercollision-wave131-w7701.md` only.
Gate: byte 4506 / linked 4506, size-exact 4524/4524, data 843/843, imports
207, lint CLEAN, drift 0.
