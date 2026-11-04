# SpaceLounge::drawLounge — branch-weight re-check and pad/x allocation re-measured (w5016, wave 100)

Status: research note (2026-11-04). Worktree `wt/w5016`, branch `wave100-w5016`,
base `44225192c`. **No `src/` change landed**: every source shape tested this
wave keeps the official score at 95.5 (raw 95.473251). The tree is clean.

Gate (`cmake --build cmake-build-match --target check -j6`) before and after:

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`_ZN11SpaceLounge10drawLoungeEv`: `osize = usize = 1948`, `byte = linked =
false`, official `pct 95.5` (raw `difflib` ratio **95.473251**), 83 structural
`|` lines in the side-by-side diff.

Prior work: `spacelounge-drawlounge-w4519.md` (newest), `-w4246.md`,
`-w4227.md`, `-w4118.md`, `-w4018.md`, `-w3838.md`, `-w3818.md`,
`branch-weight-overrides_w4218.md`, `SpaceLounge-OnRender3D.md`,
`SpaceLounge-layout.md`. This note records the wave-100 re-check and supersedes
nothing.

## 1. The 20:12 per-file override is already gone

`CMakeLists.txt` currently carries only two per-file branch-weight overrides:

```text
src/game/world/StarMap.cpp    -likely-branch-weight=7  -unlikely-branch-weight=1
src/game/weapons/Radar.cpp    -likely-branch-weight=80 -unlikely-branch-weight=1
```

The old `SpaceLounge.cpp` `20:12` override was removed in wave 92
(`branch-weight-overrides_w4218.md`, section 1.2).  There is no CMake change to
make: `grep -n SpaceLounge CMakeLists.txt` only finds the explanatory comment
at line 89.

### 1.1 What the branch actually is

The only `__builtin_expect` left in the function is the width branch:

```cpp
int width = Globals::Canvas->GetTextWidth(..., name);
if (__builtin_expect(agent->isKnown(), 1)) { // lint: arm_builtin anchor
    width += Globals::Canvas->GetTextWidth(..., role + String("  "));
}
```

`Agent::isKnown()` is `return this->eventCount > 0;` (`Agent.cpp:133`), but it
is a cross-TU call, so the branch is on a **call result**, not on an `icmp`:

```llvm
%115 = invoke zeroext i1 @_ZN5Agent7isKnownEv(...)
br i1 %115, label %117, label %158, !dbg !13469, !prof !13470
!13470 = !{!"branch_weights", i32 2000, i32 1}
```

With the `__builtin_expect` the IR carries `2000:1`.  Without it there is no
`!prof` metadata, and the MIR block successors are **50/50 (1:1)**, not 20:12:

```text
bb.27.invoke.cont74:
  successors: %bb.28(0x40000000), %bb.38(0x40000000); %bb.28(50.00%), %bb.38(50.00%)
  t2CMPri killed %447:gprnopc, 0, ...
  t2Bcc %bb.38, 0, killed $cpsr
  t2B %bb.28
```

`BranchProbabilityInfo`'s zero-compare heuristic (20:12) only fires on an
`icmp` against zero; a boolean call result gets the default 1:1.  The 20:12 in
the task title is the *removed override value*, not a natural heuristic for this
branch.

### 1.2 The branch weight is score-neutral

With the `__builtin_expect` present, forcing the override weight to 20:12 and
leaving the default 2000:1 both produce the **same** linked function:

| weight | raw pct | size | pad | x |
|---|---:|---:|---|---|
| `-likely=2000 -unlikely=1` (default) | **95.473251** | 1948 | fp | r9 |
| `-likely=20 -unlikely=12` (old override) | **95.473251** | 1948 | fp | r9 |

So the override was indeed a no-op for the official score and its removal is
correct.  Removing the `__builtin_expect` itself is a different matter: the
intrinsic is needed for the 1948-byte body in the current flat-clamp shape
(plain form grows to 1952; see §3).

## 2. Current source shape and residual diff

The mode-0 name-plate block (committed base, `src/game/menu/SpaceLounge.cpp`
lines 1193-1222):

```cpp
int pad = Globals::layout->field_0x94;
Globals::Canvas->SetColor(0xffffffffu);
int centerX = (int) (topLeft.x + (bottomRight.x - topLeft.x) * 0.5f);
int top = (int) (bottomRight.y - (float) (pad + pad));
...
int width = Globals::Canvas->GetTextWidth(..., name);
if (__builtin_expect(agent->isKnown(), 1)) {
    width += Globals::Canvas->GetTextWidth(..., role + String("  "));
}
Layout *layout = Globals::layout;
int x = centerX;
if (x - pad < layout->buttonInsetX) {
    x = layout->buttonInsetX + pad;
} else if (pad + x + width > Globals::w - layout->buttonInsetX) {
    x = Globals::w - layout->buttonInsetX + (-width - pad - pad);
}
int lineHeight = layout->field_0x30_rowHeight;
layout->drawBox(2, x - pad, top - pad, pad * 2 + width, lineHeight, String(""));
```

The residual diff is one contiguous region: the clamp right arm, the join, and
the post-`drawBox` agent/top restore; the tail branch offsets follow the 2-byte
difference in the clamp/join region.  Register map:

```text
value        original        ours (base)
layout       r4              r4
x            r5 -> r3 -> r9  r9 (whole)
width        r6              r6
agent        r8 (spilled)    r8 (spilled)
top          r2 -> r8        r2 -> r8
pad          r9              fp (r11)
pad*2 (sl)   r10             r10
lineHeight   fp (r11)        fp (r11)
x-pad        r6 -> r2        r6 -> r2
top-pad      r5 -> r3        r5 -> r3
```

The original clamp right arm is two instructions:

```text
1712dc: itt gt
1712de: addgt.w r1, r6, sl     ; width + pad2
1712e2: subgt r5, r0, r1       ; x = (w - inset) - (width + pad2)
```

The base flat form is four:

```text
1712dc: itttt gt
1712de: movgt r1, #0
1712e0: subgt.w r1, r1, fp, lsl #1
1712e4: subgt r1, r1, r6
1712e6: addgt.w r9, r1, r0
```

The target's two-instruction arm is the faithful `pad2` form (Ghidra
`iVar11 = iVar9 * 2`; `iVar4 = iVar8 - (iVar23 + iVar11)`), but the faithful
form allocates `pad = fp, x = r9` and adds a `mov r5, fp` pad copy in each
clamp arm, so its official score is lower (94.0).

## 3. Official scores of the measured shapes

All measured with the official `tools/verify/verify.py` on the linked `.so`
(`/dev/shm/w5016/fast.py`, raw score from `/dev/shm/w5016/raw.py`):

| variant | clamp | width branch | raw pct | size |
|---|---|---|---:|---:|
| **base** | flat `+(-width-pad-pad)` | `__builtin_expect(...,1)` | **95.473251** | 1948 |
| paren | `-(width + pad*2)` | builtin | 93.964335 | 1948 |
| faith | faithful `pad2`, `-(width+pad2)` | builtin | 93.964335 | 1948 |
| faith_plain | faithful `pad2` | plain `isKnown()` | 93.689986 | 1948 |
| noexpect | flat | plain `isKnown()` | 92.054795 | **1952** |
| right_local | `right - width - pad - pad` | builtin | 92.5 (rounded) | 1944 |
| nested_faith | Ghidra `else { right; if }` | plain | 89.6 (rounded) | 1944 |
| expect0 | flat | `__builtin_expect(...,0)` | 71.4 (rounded) | 1984 |

The object-level oracle used in earlier waves does **not** rank these the same
way as the official linked verifier (e.g. it reports 98.63 for the base and
97.53 for the faithful form).  Every conclusion below uses the official linked
score.

## 4. Why the target allocation is not reproduced

Base RA trace (`/dev/shm/w5016/runtrace.py`, instrumented LLVM 7.0.2):

```text
select %609 x       size=4220 w=2.946139e-04 hint=$r3 -> $r5
select %47  boxW    size=768  w=6.080038e-04 hint=$r3 -> $r6
select %46  top-pad size=736  w=6.251306e-04 hint=$r3 -> $r5   (evicts x)
select %609 x       size=4220 w=2.946139e-04 hint=$r3 -> $r9   (r9 free -> whole)
select %45  x-pad   size=704  w=6.432504e-04 hint=$r2 -> $r6
select %47  boxW    size=768  w=6.080038e-04 hint=$r3 -> $r10
select %2   pad     size=6400 w=2.154168e-04 no hint -> cannot evict x in r9
     -> spills (stage 1), spills (stage 2), lands in $r11 (fp)
```

The target needs `x` to stay in `r5` and `pad` to take `r9`.  That requires:

1. `%46` (top-pad) **not** to evict `x` from `r5`; and
2. `pad` to be selected while `r9` is still free, i.e. before `%47` (boxW)
   claims `r9`.

Both `%46` and `%47` are `0xE` (physical-hint priority) because they are call
arguments: the MIR has `$r3 = COPY %46`, `$r2 = COPY %46`, `$r3 = COPY %47`.
`pad` `%2` is a plain load with **no COPY anywhere**, so it is `0xA` and is
selected after every hinted range.  By then `x` has been evicted to `r9`.

The target's `x` is split at the x-pad use (`mov r3, r5` then `mov r9, r3`),
which lets `r5` be reused by `%46` and `r9` by `pad` first.  The base's `x` is
whole in `r9`, so no split is needed.  The blocker is the same one documented
in `spacelounge-drawlounge-w4519.md` §7: `pad` has no physical copy hint.

Knob probes (instrumented clang, scratch only):

* `PRIOVREG=pad` (priority bit only) makes `pad` take `r5` (first free
  callee-saved), score 76.4.
* `VRW=pad:1.4` raises pad's weight above x's (3.016e-4 > 2.946e-4), but pad
  evicts the cheapest candidate `%365` (Canvas) into `r11`, then lineHeight
  evicts it; score unchanged (95.473251 in the linked build).
* `VRW=x:2.5` raises x's weight above `%46`'s; x stays `r5`, but `%47` (boxW)
  then takes `r9`, so pad still cannot get it.
* `-stress-regalloc=11..14` on base/faithful: 64-76%, sizes 1968-1980.

## 5. Source shapes re-measured this wave (all <= 95.5)

About 100 shapes were generated and measured with the official verifier.  The
main dimensions:

* clamp right arm: flat `+(-width-pad-pad)`, `-(width+pad+pad)`,
  `-(width+pad2)`, `-width-pad2`, `-(width+pad*2)`, `+(-width-pad2)`,
  `-(pad2+width)`, `-pad2-width`, reversed operand order, `right` local,
  `inset` local, nested `else { right; if }`, ternary;
* condition: `pad + x + width > right`, `x + pad + width > right`,
  `width + pad + x > right`, `right < ...`;
* `pad2` declaration: `pad*2`, `pad+pad`, `pad<<1`, `2*pad`, `pad2/2`,
  before/after `SetColor`, before/after `centerX`/`top`;
* `top`: `(float)(pad+pad)`, `(float)pad2`, `(float)(pad<<1)`;
* width branch: `__builtin_expect(...,1)`, plain, `!= 0`, `== 1`, `> 0`,
  `!isKnown()` inverted, empty `else`, bool local, `__builtin_expect(...,0)`;
* `x`/`centerX` use in condition/arms, `int drawX = x`, `int boxX/boxY/boxW`
  locals, `x` declared before/after name/role/width, `layout` declared early;
* types: `int`, `unsigned`, `long`, `const int`, `unsigned short` for pad/x;
* helpers: inlined `clampX(...)`, identity `idX`/`idPad`, lambda-inlined clamp;
* named `String empty("")` before/after the clamp.

The best is always the committed base (flat clamp + `__builtin_expect`), at
95.473251/1948.  The faithful `pad2` form is semantically right but scores
93.964335 (builtin) / 93.689986 (plain).  The plain flat form grows to 1952 and
scores 92.054795.

## 6. Conclusion

1. The per-file `20:12` override is already gone; no CMake change is needed.
2. The remaining `__builtin_expect` is required for the 1948-byte body in the
   flat shape.  Its 2000:1 weight is score-identical to the old 20:12 override
   (raw 95.473251 both ways), so it is harmless and can stay.
3. The natural weight of the branch without the intrinsic is 1:1 (call result,
   not an `icmp`), not 20:12.
4. No source shape tested this wave is a strict improvement over the committed
   base.  The remaining diff is the `pad=fp/x=r9` allocation, blocked by pad
   having no physical copy hint and by `%46`/`%47` (both call-argument values)
   being selected before it.

## 7. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
# verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
# data:   exact 843 of 843 exported objects
# sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
# RATCHET PASS
```

Scratch (not committed, `/dev/shm/w5016`): `base.cpp`, `paren.cpp`,
`faith.cpp`, `faith_plain.cpp`, `noexpect.cpp`, `fast.py` (official linked
score, ~2 s/variant), `raw.py` (unrounded ratio), `gen.py`/`sweep*.py`
(source-shape generators), `runtrace.py`/`trace.err` (RA trace),
`diff_base.txt` (83-line residual).
