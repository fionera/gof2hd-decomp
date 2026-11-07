# SpaceLounge::drawLounge + Generator::createMission — wave 133 (w7740)

Status: research note (2026-11-07). Worktree `wt/w7740`, base `144fc16b0`.
**No `src/` change landed**: both targets keep their committed shapes.
* `SpaceLounge::drawLounge` raw **95.676047 %**, 1948 B (unchanged)
* `Generator::createMission` raw **97.200000 %**, 1996 B (unchanged)
* gate: `RATCHET PASS` — byte 4506 / linked 4506, avg 99.94 %, size 4524/4524,
  data 843/843, imports 207, stub/missing/extra 0, lint CLEAN, drift 0

Prior notes: `spacelounge-generator-wave132-w7718.md`,
`spacelounge-drawlounge-wave130-w7682.md`,
`generator-createmission-wave127-w7652.md`. This note supersedes nothing.

## 0. Method

Instrumented clang 7.0.2 (`/tmp/llvm_android_702b/build2/bin/clang++`) with
`RA_TRACE=1` / `VRW` / `HINTVREG` / `NOHINT` as in wave 132.  Scratch scripts
are in `/dev/shm/w7740/` (`compile_sl.sh`, `compile_gen.sh`, `try_sl.sh`,
`try_gen.sh`, `gen_sl_variants.py`, `gen_sl_perm.py`, `gen_gen_variants*.py`).
The quick screen compiles one TU to an object and greps the pad load
`ldr.w rX, [rY, #148]` inside `.text._ZN11SpaceLounge10drawLoungeEv`; only
promising shapes are linked and scored.

## 1. SpaceLounge — NEW: a `boxW` local before `drawBox` reaches the target
##    colouring (pad=r9, x=r5), but costs +20 B

Wave 132 concluded the target colouring (pad=r9, x=r5) was unreachable.  This
wave found a **source-level lever that does reach it**: compute the box width
as a named local at the call site, and use the faithful right arm
(`width + pad * 2`):

```c++
int boxW = pad * 2 + width;              // declared immediately before drawBox
layout->drawBox(2, x - pad, top - pad, boxW, lineHeight, String(""));
...
Globals::Canvas->DrawRectangle(x - pad, top - pad, boxW, ...);
```

Object code (unlinked `.o`) now has the target colouring:

```text
186:  ldr.w r9, [r6, #148]     ; pad -> r9          (target)
58a:  vmov  r5, s18            ; x -> r5            (target)
598:  sub.w r1, r5, r9         ; x - pad
5a0:  add.w r5, r0, r9         ; x = inset + pad
5b0:  add.w r1, r9, r5         ; pad + x
5b4:  add   r1, r6             ; + width
5b8:  itt   gt
5ba:  addgt.w r1, r6, fp       ; width + pad*2  (pad*2 is in fp, not sl)
5be:  subgt r5, r0, r1
```

The right arm is now the original's 2-instruction `itt gt / addgt.w / subgt`
form (the faithful clamp), and pad/x are in the target registers.  The
residual is the **rest of the packing**: lineHeight is in `r8` (original `fp`),
`pad*2` is in `fp` (original `sl`/r10), and the join therefore has extra moves.
Linked size is **1968 B** (731 insns vs 729), so the shape is not size-exact and
cannot be landed as-is.

Variants measured (all with the faithful `width + pad * 2` right arm; pad
register from the `.o`):

| variant | pad | linked size |
|---|---|---:|
| HEAD (`short pad3`) | fp | 1948 |
| faithful clamp, no local | fp | 1948 |
| `int boxW = pad*2+width;` before drawBox (W only) | **r9** | 1968 |
| `boxW` then `boxX` (WX) | **r9** | 1968 |
| `boxW` then `boxY` (WY) | **r9** | 1968 |
| `boxW`, `boxX`, `boxY` (WXY) | **r9** | 1968 |
| `boxX`, `boxW`, `boxY` (XWY) | **r9** | 1976 |
| `boxY`, `boxW`, `boxX` (YWX) | fp | 1968 |
| `boxX` only / `boxY` only | fp / r5 | 1948 / 1944 |
| `boxW` + `pad + pad` | r5 | 1968 |
| `boxW` + HEAD `pad3` arm | r5 | 1968 |
| `boxW` declared before `lineHeight` | **r9** | 1968 |
| `boxW` declared before the clamp | fp | 1968 |
| `boxW = width + pad*2` / `width+pad+pad` / `pad+pad+width` | **r9** | 1968 |

So the wave-132 "unreachable" conclusion is too strong: the target colouring is
reachable, but every shape that reaches it pays a 20-byte allocation rotation.
The remaining lever is to make `lineHeight` land in `fp` and `pad*2` in `sl`
while keeping pad=r9; the `boxW` local changes the vreg priority order enough
to flip pad/x but also flips lineHeight/pad*2.

Other reorderings tried this wave (faithful clamp, no `boxW`): moving
`lineHeight` before `x`, `x` before `width`, `top` after `width`, `top` before
`centerX`, `layout` before `centerX`, pad load after `centerX`, and reordering
the clamp condition (`x+pad+width`, `width+x+pad`, `x+width+pad`,
`pad+width+x`, `width+pad+x`, regrouped `x > w-inset-width-pad*2`,
`inset > x-pad`).  All leave pad in `fp` (or change the load base), 1948 B.

## 2. Generator — NEW: literal spellings are one IR; VRW "spill" is a split
##    range, not a memory spill

Every literal `bonus = rawBonus - rem` spelling tried this wave canonicalises
to the same machine IR: frame **104**, `rem` in `r9`, `subne.w r5,r6,r9`,
bonus in `r5`.  Spellings tested: plain literal; `int rem2 = rem; bonus =
rawBonus - rem2;`; `int neg = -rem; bonus = rawBonus + neg;`; `rem + 0`;
`rem * 1`; `rem << 0`; `if (bonus % 50)`; `bonus - (bonus/50)*50 != 0`;
`rawBonus + (-rem)`; `bonus = bonus` else-arm; `bonus = rawBonus - rem + 0`;
`bonus = bonus;` after the block; function-scope `remI`; function-scope
`rawBonus`; `int remI = 0` initialised early.  All are 104-byte frames with
`rem` in `r9`.

Branch-weight sweep on the literal shape (`-mllvm -likely-branch-weight=L
-mllvm -unlikely-branch-weight=U`, L in {1,2,3,4,5,7,8,10,16,20,50,100,500,2000},
U in {0,1}): every combination is frame 104 with `rem` in `r9` (L=2/U=1 and
L=3/U=1 rotate to `subne.w r4,r6,r8`, still not the target).

New VRW evidence: `VRW=2147484157:0.5` (rem weight 1.407590e-3 -> 7.037948e-4,
below agent's 8.679897e-4) makes the greedy allocator report
`FINAL %509 -> $noreg` (spilled), but the emitted code still materialises rem
in `r9` (`mov r9,r1`) because the interval is re-enqueued as a stage-2 split
range and the split range is assigned r9.  The trace is:

```text
FINALDBG ... raw=2147484157 size=160 w=1.407590e-03 li=%509 [18544r,18704r:0)
select %509 size=160 w=7.037948e-04 ... def=[%509 = COPY $r1]
TRYASSIGN1 %509 -> $noreg
FINAL %509 -> $noreg
ENQ %509 size=160 prio=160 stage=2
...
FINAL %509 -> $r9
```

So lowering rem's weight alone does not produce a *memory* spill; the target
needs the value to be live across the second `__aeabi_idivmod` with no
callee-saved register available.  The `rem * 2` form remains the only shape
that keeps the 96-byte frame and 1996 B, at the cost of bonus in `r6` and rem
in `r5` (`subne.w r6,r6,r5,lsl #1`).

## 3. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7740
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
# instrumented allocation / weights
CLANG=/tmp/llvm_android_702b/build2/bin/clang++ RA_TRACE=1 \
  /dev/shm/w7740/compile_sl.sh src/game/menu/SpaceLounge.cpp /dev/shm/w7740/sl.o
CLANG=/tmp/llvm_android_702b/build2/bin/clang++ RA_TRACE=1 \
  /dev/shm/w7740/compile_gen.sh src/game/mission/Generator.cpp /dev/shm/w7740/gen.o
# fast screen: pad load register in the unlinked .o
/dev/shm/w7740/try_sl.sh <variant.cpp> <tag>
```
