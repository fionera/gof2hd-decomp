# SpaceLounge::drawLounge — wave-129 raw-vreg / hint / join-shape re-check (w7657)

Status: research note (2026-11-07). Worktree `wt/w7657`, base `fee528cbe`.
**No `src/` change landed**: the committed HEAD (`short pad3`, raw
**95.676047 %**, 1948 B) is still the best measured shape.
The tree is clean apart from this note.

Target: `_ZN11SpaceLounge10drawLoungeEv` (1948/1948 B, `byte = linked = false`).

* before: raw **95.676047 %**, official **95.7 %**, 1948 B
* after:  unchanged (note only)
* gate: `RATCHET PASS` — byte 4506 / linked 4506, avg 99.94 %, size 4524/4524,
  data 843/843, imports 207, stub/missing/extra 0, lint CLEAN, drift 0

Prior work: `spacelounge-drawlounge-wave125-w7636.md` (EH/branch-weight),
`-wave124-w7613.md` (MIR priority/hint), `-wave122-w7217.md` (pad weight),
`-wave121-w7117.md` (RA order), `-ra-split-w3838.md` (split root cause).
This note supersedes nothing.

## 1. Residual (re-verified, unchanged)

Only the clamp / join `0x1712b2..0x171330` differs:

```text
value        original        HEAD
layout       r4              r4
x            r5 -> r3 -> r9  r9 (whole)
width        r6              r6
agent        r8 (spilled)    r8 (spilled)
top          r2 -> r8        r2 -> r8
pad          r9              fp (r11)
pad*2 (sl)   r10             r10
lineHeight   fp (r11)        fp (r11)
```

## 2. NEW: the `VRW`/`NOHINT` knobs compare the *raw* vreg, not the index

`CalcSpillWeights.cpp:173` compares `vr == li.reg` and `RegAllocGreedy.cpp:93`
compares `strtoul(item) == Reg`, where `Reg`/`li.reg` is the **raw** vreg
(`0x80000000 | index`).  The wave-122/124/125 notes used `NOHINT=608`
(index) — that is a **no-op**; the correct spelling is
`NOHINT=2147484256` (0x80000260).  The pad knobs in wave-122
(`VRW=2147483650`) were already raw and therefore valid.

Re-run with the correct raw number:

* `NOHINT=2147484256` (x unhinted) → x priority drops to `0xA000107C`,
  the whole allocation rotates (x→fp, pad→r4, 1948 B, not the target).
* `VRW=2147484256:<w>` (x weight) → for w ≤ 0.9 x lands in r2/r5 with
  pad in r8/fp; w ≥ 1.0 is the HEAD allocation; w ≥ 2.2 spills agent and
  rotates everything.  No value reaches pad=r9.
* `VRW=2147483650:<w>` (pad weight, faithful clamp) → pad=r9 for
  w ≈ 1.5..2.1 (see §3).

## 3. NEW: faithful clamp + pad weight 2.0 reaches the target x/pad registers

Compiling the *faithful* clamp (`width + pad * 2`, no `pad3`) with
`VRW=2147483650:2.0` gives (single-TU NDK r18b object, 1944 B):

```text
57a vmov r5, s18
588 sub.w r1, r5, r9        ; x - pad      (pad in r9!)
590 add.w r5, r0, r9        ; x = inset + pad
5a0 add.w r1, r9, r5        ; pad + x
5a8 itt gt
5aa addgt.w r1, r6, sl      ; width + pad2  (faithful right arm)
5ae subgt r5, r0, r1
5b2 ldr.w r8, [r4, #48]     ; lineHeight -> r8   (target: fp)
5c0 vmov r1, s16            ; top -> r1           (target: r2)
5c4 mov r2, r5              ; x -> r2             (target: r3)
5c8 sub.w r5, r1, r9        ; top - pad
5cc mov fp, r1              ; top -> fp           (target: r8)
5ce mov r1, r9              ; pad -> r1
5d0 subs r6, r2, r1         ; x - pad
5d2 mov r9, r2              ; x -> r9
```

So **pad=r9 and x=r5→r2→r9 are reachable**; the residual after that is the
colouring of `lineHeight`/`top` (r8/r1/fp vs fp/r2/r8) and the earlier agent
spill (`str r8,[sp,#12]` before the clamp instead of at the join), which makes
the body 1944 B (4 B short).  The target is a *later* agent spill: lineHeight
must take fp (not r8), which requires r8 to still hold the agent when the
lineHeight interval is allocated.

## 4. NEW: branch-weight sweep on the HEAD (pad3) form

`-mllvm -likely-branch-weight` / `-unlikely-branch-weight` swept over
{7:1, 1:1, 2:1, 3:1, 4:1, 8:1, 16:1, 31:1, 100:1, 1:7, 7:7, 20:12, 5:3}.
The clamp register colouring is *not* invariant: at 1:1 and 7:7 pad moves to
**r5** (x still r9), at 1:7 the body grows to 1980 B; the default 7:1 is the
only ratio that keeps the HEAD 95.7 shape.  **No ratio gives pad=r9** (a
36-point LW×UW grid was checked).  Branch weights are not the lever.

## 5. NEW: inline helper taking `pad` as an argument (hint attempt)

An inlined `clampNamePlateX(x, pad, width, inset, w)` (faithful body) scores
**95.6104**, 1948 B, with the *flat* right arm and pad still in fp: the
inliner substitutes the arguments, so no `COPY` hint reaches `pad`.

## 6. Other source shapes measured this wave (all ≤ 95.676)

| variant | raw % | size | note |
|---|---:|---:|---|
| HEAD (`short pad3`) | **95.6760** | 1948 | pad=fp, x=r9 |
| faithful `width + pad*2` | 94.0 | 1948 | right arm exact, pad copy in both arms |
| faithful + named left/top2/boxW (l,t,b) | 94.0 | 1948 | same allocation |
| pad3 + named left/top2/boxW (l,t,b) | 95.6760 | 1948 | same as HEAD |
| pad3 + named left,top2 (t,b order) | 93.1 | 1948 | allocation rotates |
| pad3 + named boxW first | 71.1 | 1964 | |
| reversed comparisons (`inset > x-pad`, `right < …`) | 95.3–95.5 | 1948 | |
| `x < inset + pad` | 91.1 | 1944 | |
| `__builtin_expect` on either clamp arm | 91.1–92.1 | 1952 | adds instructions |
| `top` via `pad*2` / `pad+pad` / float / int | 81.6–95.7 | 1960–1968 | |
| `centerX` via `/2.0f` | 95.7 | 1948 | same |
| `int drawX = x` / `x + 0` for DrawString | 95.7 | 1948 | coalesces, hint stays on x |
| inline `clampNamePlateX(...)` helper | 95.6104 | 1948 | flat arm, pad=fp |
| `inset` / `right` named locals | 94.0 / 95.7 | 1948 | |
| `pad` as `short` / `unsigned` / `long` | 75.4 / 95.4 / 95.7 | 1948–1960 | |
| declaration-order permutation of `pad`/`SetColor`/`centerX`/`top` (12 valid) | 93.1–95.7 | 1948 | 8 orders keep 95.7 |

## 7. Root cause (unchanged, now pinned to the raw-vreg evidence)

`pad` is an unhinted `0xA` interval of size 6384; `x` is a `$r3`-hinted
`0xE` interval of size 4220.  The greedy order is

```text
x -> r5, boxW -> r6, top-pad (tryEvict, r5 cheaper than the free r9 because
r9 has CostPerUse=1) -> r5, x re-selects -> r9 whole, x-pad -> r6,
boxW -> r10, pad -> r11 (evicts Canvas, w=2.145e-4 < x's 2.946e-4)
```

`pad` never evicts `x` from r9 because `%365` (Canvas) is the cheaper victim.
To reach the target, `x` must be **split at the join** (clamp part r5, tail
r9) so that pad's interval `[6752,7568)` no longer interferes with the tail.
The allocator only splits when `tryAssign` fails; r9 is free, so it never
does.  The faithful+VRW=2.0 build proves the split is reachable once pad's
weight is doubled (pad wins r9), but then the agent spill moves before the
clamp and lineHeight takes r8 instead of fp.

## 8. Conclusion

The residual is the single greedy-RA split/colouring fact already pinned in
wave-124/125, now with corrected raw-vreg knob evidence and a concrete
target-register build (faithful + pad weight 2.0).  No source-level shape
found this wave reaches pad=r9 *and* keeps the agent live in r8 until the
join; the committed HEAD (`short pad3`, **95.67604667124228 %**, 1948 B)
remains the best shape.

## 9. Reproduction

```bash
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
# raw-vreg knobs (raw = 0x80000000|index):
#   x = 2147484256, pad = 2147483650
VRW=2147483650:2.0 CLANG=/tmp/llvm_android_702b/build2/bin/clang++ \
  OUT=/dev/shm/w7657/SpaceLounge.o /dev/shm/w7657/compile.sh
```
