# SpaceLounge::drawLounge — wave-106 re-measure (w5418)

Status: research note (2026-11-04). Worktree `wt/w5418`, branch
`wave106-w5418`, base `486f8a119`. **No `src/` change landed**: every source
shape tested this wave keeps the official score at drawLounge raw
**95.473251** (pct 95.5, 1948 B, 729 insns). The tree is clean apart from this
note.

Gate (`cmake --build cmake-build-match --target check -j6`) before and after:

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.91
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Target: `_ZN11SpaceLounge10drawLoungeEv` (1948/1948 B, byte=false,
linked=false, 82 structural `|` lines in the side-by-side diff).

Prior work: `spacelounge-drawlounge-modstation-ctor-w5320.md` (newest),
`spacelounge-drawlounge-w5016.md`, `-w4519.md`, `-w4246.md`, `-w4227.md`,
`-w4118.md`, `-w4018.md`, `-w3838.md`, `-w3818.md`. This note records only the
new evidence gathered in wave 106 and supersedes nothing.

## 1. Build / measurement commands

```bash
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
```

Per-variant measurement was done with a scratch harness under `/dev/shm/w5418`
(not committed): copy the variant over `src/game/menu/SpaceLounge.cpp`,
`cmake --build cmake-build-match --target link_so -j6`, then run the official
`tools/verify/verify.py` score on the linked `.so`, then
`git checkout -- src/game/menu/SpaceLounge.cpp`. Raw (unrounded) ratio is
`difflib.SequenceMatcher(..., autojunk=False).ratio()*100` over the normalised
instruction stream, exactly as `tools/verify/verify.py:score`.

## 2. Residual diff (unchanged from w5016)

`osize = usize = 1948`, 729 instructions each side. The residual is confined to
the mode-0 name-plate clamp / join / post-`drawBox` restore plus the tail
branch offsets that follow from the clamp's 2-byte size difference. Register
map:

```text
value        original        ours (base)
layout       r4              r4
x            r5 -> r3 -> r9  r9 (whole)
width        r6              r6
agent        r8 (spilled)    r8 (spilled) -> r5 after drawBox
top          r2 -> r8 -> fp  r2 -> r8 (whole)
pad          r9              fp (r11)
pad*2 (sl)   r10             r10
lineHeight   fp (r11)        fp (r11)
```

Original clamp right arm (2 insns) vs base flat form (4 insns):

```text
orig:  itt gt
       addgt.w r1, r6, sl        ; width + pad*2
       subgt   r5, r0, r1        ; x = (w - inset) - (width + pad*2)
base:  itttt gt
       movgt   r1, #0
       subgt.w r1, r1, fp, lsl #1
       subgt   r1, r1, r6
       addgt.w r9, r1, r0
```

The base flat form is a *scoring artifact*: it is 6 bytes longer in the clamp
but 4+ bytes shorter in the post-`drawBox` restore, and the net 2-byte shift
makes the tail branch targets (`0x6ec`) match the original. The faithful
`-(width + pad*2)` form gets the 2-instruction arm but shifts every tail
branch by 2 bytes and scores lower (93.964335).

## 3. RA mechanism (re-confirmed with the LLVM 7.0.2 greedy trace)

`RegAllocGreedy::enqueue` orders the queue by priority class, then size, not by
spill weight:

```text
hinted global   (1<<31)|(1<<30)|(1<<29)|size
unhinted global (1<<31)|(1<<29)|size
local           (1<<31)|distance
split           size
```

`VirtRegMap::hasKnownPreference` (the `1<<30` bit) is set only from a COPY
involving a physical register. In this function:

```text
%574 x      size 4208  hint $r3 (from $r3 = COPY %574 at the DrawString call)
%47  boxW   size  768  hint $r3
%46  top-pad size 736  hint $r3/$r2
%45  x-pad  size  704  hint $r2
%2   pad    size 6304  NO hint
```

So `x` (hinted) is selected before `pad` (unhinted) even though pad is larger.
`x` takes `r5`; `%46` (top-pad) then evicts `x` from `r5`; `x` is re-selected
and takes the still-free `r9` whole; `pad` is selected later and, with `r9`
occupied, spills/splits and lands in `fp`.

The target needs `pad` to be selected before `x`'s re-enqueue and to take `r9`.
The prior w4519 note showed that with an instrumented `VRW=pad:1.4` weight
boost, pad does take `r9` and x `r5`, scoring **95.7** — but `top=r1` and
`lineHeight=r8` are then wrong. There is no source-level knob for that weight
boost; pad is a plain load with no COPY, so it never gets the `1<<30` bit.

## 4. Source shapes measured this wave (all <= 95.473251)

All measured with the official linked verifier. `base` is the committed source
(flat clamp `+(-width-pad-pad)`, `__builtin_expect(...,1)`).

| variant | raw pct | size |
|---|---:|---:|
| **base** | **95.473251** | 1948 |
| faithful `-(width + pad*2)` | 93.964335 | 1948 |
| `+(-(width + pad*2))` / `-(pad*2 + width)` / `-width-pad*2` / `-(pad+pad+width)` | 93.964335 | 1948 |
| `-(width + pad + pad)` / `-(pad + pad + width)` / `-width-(pad+pad)` | 95.336077 | 1948 |
| `+(-pad-pad-width)` / `+(-width-(pad+pad))` | 95.473251 (= base bytes) | 1948 |
| `-width-pad-pad` | 92.5 | 1944 |
| condition `right < sum` | 95.336077 | 1948 |
| condition `inset > x-pad` | 95.1 | 1948 |
| `int inset = layout->buttonInsetX` local + flat | 95.473251 (= base bytes) | 1948 |
| `int right = Globals::w - inset` local + flat | 95.473251 (= base bytes) | 1948 |
| ternary clamp (two `?:`) | 92.0 | 1948 |
| `int x; x = centerX;` / `int x = 0; x = centerX;` | 95.473251 (= base bytes) | 1948 |
| `if (centerX - pad < ...)` / `if (x < inset + pad)` | 95.473251 / 91.0 | 1948 / 1944 |
| `int drawX = x;` (int/unsigned/const/long/long long/memcpy/`+0`/`*1`/`|0`/`^0`/`x-x+x`) | 95.473251 (= base bytes) | 1948 |
| `short`/`unsigned short`/`signed char` drawX copy | 78.6 | 1976 |
| `int drawX = agent->isKnown() ? x : x;` (select survives) | 84.4 | 1956 |
| `volatile int drawX = x;` | 66.3 | 1972 |
| `int boxX/boxY/boxW` locals (any order, flat clamp) | 95.473251 (= base bytes) | 1948 |
| `int boxX/boxY/boxW` locals + faithful clamp | 94.0 | 1948 |
| `int pad2 = pad * 2` local: top only / box only / top+box | 95.473251 (= base bytes) | 1948 |
| `int pad2` local: clamp only / top+clamp / clamp+box / all | 94.0 | 1948 |
| `float topF = ...; int top = (int)topF;` | 95.473251 (= base bytes) | 1948 |
| `float cxF = ...; int centerX = (int)cxF;` | 95.473251 (= base bytes) | 1948 |
| `(float)pad * 2.0f` in top (adds `vmul.f32`) | 81.7 | 1968 |
| `(float)(pad<<1)` / `(float)(2*pad)` / `(float)(pad*2)` in top | 95.473251 (= base bytes) | 1948 |
| separate `topPad` load + late `pad` load | 72.0 | 1976 |
| direct `Globals::layout->field_0x94` in top + late `pad` | 69.9 | 1976 |
| `__builtin_expect` on `mode == 0` / `hoverAgent < 0` | 38.8 / 77.3 | 1940 / 1944 |
| `__builtin_expect` on either clamp condition | 91.0-91.9 | 1952-1956 |
| `volatile` pad load (pointer cast / member cast / `volatile int pad`) | 93.0 / 93.0 / 66.2 | 1948 / 1948 / 1980 |
| `volatile` `Globals::w` load in the clamp | 95.473251 (= base bytes) | 1948 |
| `volatile` `Globals::layout` pointer load | 95.473251 (= base bytes) | 1948 |
| declaration-order permutations of `pad/SetColor/centerX/top/name-role-width` (120) | 95.473251 for 8 orders, all byte-identical to base; 61.9-92.9 otherwise | 1948-1972 |
| `role` declared before `name` | 72.9 | 1896 |

## 5. What is proven vs hypothesis

Proven (measured, reproducible):

1. The committed base is a strict local optimum: 95.473251 / 1948 B / 729
   insns. Every semantically equivalent declaration order, clamp algebraic
   form, box-local, `drawX` copy, `pad2` local, cast, and volatile placement
   tested either reproduces the base bytes exactly or scores lower.
2. The residual is not a source-shape issue in the clamp algebra: all
   `pad+pad`/`pad*2`/`pad<<1`/`2*pad` spellings canonicalise to the same MIR
   and the same `mov.w sl, rX, lsl #1`.
3. The tail branch offsets (`0x6ec` vs `0x6ea`) are a consequence of the
   clamp/restore byte-count, not an independent defect. The flat clamp's
   6-byte-longer arm is what makes the base's tail offsets match.
4. LLVM 7.0.2's greedy allocator selects by priority class (hinted globals
   first, then unhinted globals by size), not by spill weight. `pad` has no
   physical copy hint, so it is always selected after `x`, `%45`, `%46`, `%47`.

Hypothesis (not proven):

1. The original source is the faithful `pad2` form (Ghidra shows
   `iVar12 = iVar10 * 2` and `iVar4 = (Globals::w - iVar9) - (iVar18 + iVar12)`),
   but the exact clang 7.0.2 greedy-RA tie-breaking that produced
   `pad=r9 / x=r5->r3->r9` is not reachable from any source shape tested so
   far. The only way found to reach that allocation is an instrumented
   `VRW=pad:1.4` weight boost (scratch compiler), which scores 95.7 but leaves
   `top`/`lineHeight` in the wrong registers.
2. A source-level construct that gives `pad` a physical-copy hint naming `r9`
   (or otherwise forces `pad` into the `1<<30` priority class before `x`'s
   re-enqueue) would reproduce the target allocation. No such construct was
   found: `pad` is never a call argument, and every attempted `drawX`/`pad2`
   copy is either coalesced away or costs an extra instruction.

## 6. Conclusion

No source variant tested this wave is a strict improvement over the committed
base. The base stays at raw **95.473251** (pct 95.5, 1948 B, 729 insns), with
gate `byte 4500 / linked 4500 / data 843 / imports 207 / lint CLEAN / drift 0`.
