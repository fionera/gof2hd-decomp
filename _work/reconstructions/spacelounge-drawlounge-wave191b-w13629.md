# SpaceLounge::drawLounge — wave-191b: the true source shape is 175/41, not 138/87 (w13629)

Status: research note (2026-11-13). Worktree `wt/w13629`, branch
`wave191b-w13629`, base `ebd084c5e`. **No `src/` change landed** — the
committed base still scores better raw (138) than the reconstructed original
source shape (175). Tree is clean apart from this note.

Target: `_ZN11SpaceLounge10drawLoungeEv` (1948/1948 B).

* baseline: raw **138**, aligned **87**, size **1948/1948**, insn **739/741**,
  `.ARM.extab` diff **577 B**
* after: unchanged (note only)

Prior work read first: `spacelounge-drawlounge-wave191b-w13620.md`,
`...w13566.md`, `...w13554.md` (and the wave-190 notes they cite). This note
adds ~120 fresh measured cells and one new structural finding: **the 175/41
shape is the original source shape**, and the committed 138 shape is a
different (non-original) spelling that happens to allocate better.

## 1. The true original source shape (175 / 41 / 1948 / 741)

The original disassembly computes `left = x - pad` and `above = top - pad`
once and reuses them in *both* `drawBox` and `DrawRectangle`
(`subs r6, r3, r1` / `sub.w r5, r2, r9`, then `mov r2, r6` / `mov r3, r5`
for drawBox and `mov r1, r6` / `mov r2, r5` for DrawRectangle). The
`DrawRectangle` height is a *reload* of `layout->field_0x30_rowHeight`
(`ldr r1, [r1, #48]`), not the `lineHeight` local. The `DrawRectangle` x
argument is `left` (the variable), not `x - Globals::layout->field_0x94`.

Spelling that reproduces 741/741 instructions:

```cpp
int left = x - pad;
int above = top - pad;
int lineHeight = layout->field_0x30_rowHeight;
layout->drawBox(2, left, above, pad * 2 + width, lineHeight, String(""));
...
Globals::Canvas->DrawRectangle(left, above, pad * 2 + width,
                               Globals::layout->field_0x30_rowHeight);
```

Measured: **raw 175 / aligned 41 / 1948 B / 741 insns / extab 580**.
The aligned diff is now only **41 instructions**, and every one is the single
`pad`/`x` register swap:

```
value   original       175-shape
pad     r9             fp (r11)
pad2    sl (r10)       sl (r10)   <- already matches
x       r5 -> r3 -> r9 r9 (whole)
top     r2 -> r8       r2 -> r8   <- matches
```

The conversation branch (0x1dc..0x576) is **byte-identical** in this shape
(the committed 138 shape has a 19-byte `sl`/`fp` swap there). This is strong
evidence that 175/41 is the original source and 138/87 is not.

The single change that flips the committed base (138) to the original shape
(175) is the `DrawRectangle` x argument:
`x - Globals::layout->field_0x94` -> `x - pad` (or `left`).

## 2. Why pad lands in fp, not r9 (instrumented-clang trace)

Using `/tmp/llvm_android_702b/build2/bin/clang++` (`RA_TRACE=1`) on the
175 shape:

* `pad` is `%2` / `%647` (`t2LDRi12 %372, 148`, `%372 = Globals::layout`);
  `%647` is assigned `$r5` and `%2` is assigned `$r9` then **split/spilled**
  (`FINAL %2 -> $noreg`, stage 2), so the final code rematerialises the load
  into `fp`.
* `x` is `%609` (`COPY %386:spr`, physical-hint bit set, prio `0xE...`). It
  is selected first, takes `r5`, is evicted from `r5` by `%45`
  (`above = top - pad`, exactly as in the original), is re-enqueued, and
  takes `r9` **whole**.
* `pad2` is `%381` (`t2LSLri %2, 1`) and takes `sl` (r10), matching the
  original.
* The `Globals::layout` GOT temp `%369` (size 9872) and the `agent` pointer
  `%1` (size 8800) also contend for `r9`; `%1` evicts `pad` from `r9`.

So the residual is the coupled fact already described in w13554/w13566:
`pad` must win `r9` *and* `x` must be split at the `x - pad` use so that
`pad`/`x2` share `r9`. In the 175 shape `x` is not split (it is one live
range in `r9`), because `r9` is free when `x` is re-enqueued.

Knobs tried on the 175 shape (all still **175/41/1948/741**):

| knobs | raw | aligned |
|---|---:|---:|
| `NOHINT=%609` (x) | 175 | 41 |
| `NOHINTCO=%609` | 175 | 41 |
| `NOHINT=%47` (width+pad2) | 175 | 41 |
| `NOHINT=%47,%609` | 175 | 41 |
| `HINTVREG=%2:75` (pad -> r9) | 175 | 41 |
| `HINTVREG=%2:75..79` | 175 | 41 |
| `HINTVREG=%609:69..74` (x -> r5..r10) | 175 | 41 |
| `HINTVREG=%1:74` (agent -> r8) | 175 | 41 |
| `HINTVREG=%2:75 NOHINT=%47,%609` | 175 | 41 |
| `HINTVREG=%2:75 SIZEVREG=%1:1,%365:1,%47:1` | 175 | 41 |
| `FORCESPLIT=%609` | 175 | 41 |

The hints are honoured in the trace (e.g. `HINTVREG=%2:75` does assign pad
to `$r9` first) but a later split/spill/rematerialisation always restores the
`fp` colouring in the emitted code. Source-level hints (`__builtin_expect`,
`register`, `const`, type changes) are equally ineffective.

## 3. Measured cells (all real link, `tools/fn_diff.py`)

Base = **138 / 87 / 1948 / 739**, extab 577. Every row below was built with
the real `link_so` target and scored with `tools/fn_diff.py`.

### 3.1 The 175/41 source shape and its immediate variants

| variant | raw | aligned | size | insn | extab |
|---|---:|---:|---:|---:|---:|
| committed base (rect x = `x - Globals::layout->field_0x94`) | **138** | **87** | 1948 | 739 | 577 |
| **175 shape** (rect x = `left`, rect h reload) | **175** | **41** | 1948 | **741** | 580 |
| 175 + `-(width + pad + pad)` arm | 176 | 42 | 1948 | 741 | 580 |
| 175 + arm `-(width + (pad<<1))` | 445 | 54 | 1948 | 741 | 592 |
| 175 + arm `-(width + pad*2)` | 445 | 54 | 1948 | 741 | 592 |
| 175 + arm `-(width + pad2)` (named pad2) | 445 | 54 | 1948 | 741 | 592 |
| 175 + arm sequential `-width-pad-pad` | 468 | 64 | 1944 | 740 | 599 |
| 175 + rect height `lineHeight` local | 990 | 162 | 1948 | 737 | 594 |
| 175 + no left/above locals (inline in both calls) | 175 | 41 | 1948 | 741 | 580 |
| 175 + left/above in rect only | 175 | 41 | 1948 | 741 | 580 |
| 175 + left/above in box only | 175 | 41 | 1948 | 741 | 580 |
| 175 + left-only local | 175 | 41 | 1948 | 741 | 580 |
| 175 + above-only local | 265 | 50 | 1948 | 741 | 583 |
| 175 + above declared before left | 265 | 50 | 1948 | 741 | 583 |
| 175 + left/above `const` / `long` / comma / after lineHeight | 175 | 41 | 1948 | 741 | 580 |

### 3.2 `pad` / `pad2` spellings

| variant | raw | aligned | size | insn | extab |
|---|---:|---:|---:|---:|---:|
| 175 + `unsigned pad` | 177 | 43 | 1948 | 741 | 580 |
| 175 + `unsigned int pad` | 177 | 43 | 1948 | 741 | 580 |
| 175 + `long` / `const` / `+0` / `*1` / `(int)(uintptr_t)` pad | 175 | 41 | 1948 | 741 | 580 |
| 175 + `short pad` | 1506 | 219 | 1964 | 745 | 9527 |
| 175 + `layoutMetric(Globals::layout, 0x94)` | 175 | 41 | 1948 | 741 | 580 |
| 175 + named `pad2` in top only | 175 | 41 | 1948 | 741 | 580 |
| 175 + named `pad2` in top+box | 175 | 41 | 1948 | 741 | 580 |
| 175 + named `pad2` in top+box+arm | 445 | 54 | 1948 | 741 | 592 |
| 175 + separate `pad2 = Globals::layout->field_0x94 * 2` | 1869 | 200 | 1972 | 744 | 9534 |

### 3.3 `x` / `centerX` / `top` spellings

| variant | raw | aligned | size | insn | extab |
|---|---:|---:|---:|---:|---:|
| 175 + `x = centerX + 0` / `*1` / `-(-centerX)` / `\|0` / `^0` / `&-1` / `(int)(double)` | 175 | 41 | 1948 | 741 | 580 |
| 175 + `x = (int)(float)centerX` / `(int)(0.0f + (float)centerX)` | 534 | 77 | 1956 | 743 | 9514 |
| 175 + `long x` | 175 | 41 | 1948 | 741 | 580 |
| 175 + no `centerX` (x = full float expr) | 990 | 162 | 1948 | 737 | 594 |
| 175 + clamp modifies `centerX` directly (no x) | 175 | 41 | 1948 | 741 | 580 |
| 175 + `int x = centerX;` after the clamp | 175 | 41 | 1948 | 741 | 580 |
| 175 + `int drawX = x;` used in DrawString | 175 | 41 | 1948 | 741 | 580 |
| 175 + `float centerXf; int centerX = (int)centerXf;` | 175 | 41 | 1948 | 741 | 580 |
| 175 + `float topf; int top = (int)topf;` | 175 | 41 | 1948 | 741 | 580 |
| 175 + top `(float)(pad<<1)` / `(float)(pad*2)` | 175 | 41 | 1948 | 741 | 580 |
| 175 + top `(double)(pad+pad)` | 1472 | 139 | 1952 | 742 | 9525 |
| 175 + top `(int)bottomRight.y - pad - pad` | 1847 | 168 | 1956 | 738 | 9529 |
| 175 + top `bottomRight.y - (float)pad - (float)pad` | 1845 | 163 | 1968 | 742 | 9531 |

### 3.4 `layout` declaration / access placement

| variant | raw | aligned | size | insn | extab |
|---|---:|---:|---:|---:|---:|
| 175 + `Layout *layout = Globals::layout;` after pad | 1860 | 276 | 1972 | 744 | 9531 |
| 175 + same after top / after SetColor | 1859 | 287 | 1972 | 744 | 9532 |
| 175 + same before pad | 1860 | 276 | 1972 | 744 | 9531 |
| 175 + same before pad, later uses via `layout` | 1808 | 173 | 1960 | 741 | 9531 |
| 175 + clamp via `Globals::layout` | 175 | 41 | 1948 | 741 | 580 |
| 175 + drawBox via `Globals::layout` | 175 | 41 | 1948 | 741 | 580 |
| 175 + lineHeight via `Globals::layout` | 175 | 41 | 1948 | 741 | 580 |

### 3.5 Clamp / branch / String spellings

| variant | raw | aligned | size | insn | extab |
|---|---:|---:|---:|---:|---:|
| 175 + clamp sum orders (`x+pad+width`, `x+width+pad`, `width+pad+x`, `width+x+pad`) | 175 | 41 | 1948 | 741 | 580 |
| 175 + clamp `x > w-inset-width-pad-pad` | 181 | 43 | 1948 | 741 | 580 |
| 175 + clamp `x+width > w-inset-pad-pad` | 477 | 60 | 1952 | 742 | 9513 |
| 175 + left clamp `x < inset+pad` | 492 | 73 | 1944 | 740 | 596 |
| 175 + left clamp `pad+x < inset+pad+pad` | 204 | 56 | 1948 | 741 | 580 |
| 175 + `hoverAgent == -1` | 179 | 43 | 1948 | 741 | 580 |
| 175 + `hoverAgent <= -1` / `!(>=0)` | 175 | 41 | 1948 | 741 | 580 |
| 175 + `!mode` / `0 == mode` | 175 | 41 | 1948 | 741 | 580 |
| 175 + drawBox `String()` default | 471 | 69 | 1940 | 737 | 610 |
| 175 + drawBox `String("", false)` | 175 | 41 | 1948 | 741 | 580 |
| 175 + drawBox `String("", true)` | 175 | 42 | 1948 | 741 | 580 |
| 175 + named `String empty("")` | 1911 | 313 | 2012 | 765 | 237619 |
| 175 + `name` only in DrawString | 912 | 129 | 1876 | 710 | 233674 |
| 175 + `name + String("  ")` unconditionally | 1279 | 157 | 1924 | 729 | 617 |
| 175 + `isKnown() ? name+String("  ") : name` | 1264 | 262 | 1980 | 753 | 233085 |

### 3.6 Statement order permutations

| order (pad, SetColor, centerX, top) | raw | aligned | size |
|---|---:|---:|---:|
| pad, SetColor, centerX, top (original) | **175** | **41** | 1948 |
| pad, top, SetColor, centerX | 175 | 41 | 1948 |
| pad, centerX, top, SetColor | 175 | 41 | 1948 |
| pad, SetColor, top, centerX | 175 | 41 | 1948 |
| pad, centerX, SetColor, top | 175 | 41 | 1948 |
| centerX, pad, top, SetColor | 175 | 41 | 1948 |
| SetColor, pad, top, centerX | 254 | 72 | 1948 |
| SetColor, centerX, pad, top | 254 | 72 | 1948 |

## 4. Conclusion

The original source shape is the `left`/`above` + `DrawRectangle(left, above,
..., Globals::layout->field_0x30_rowHeight)` shape: **175 raw / 41 aligned /
1948 B / 741 insns / extab 580**. The committed base (138 raw / 87 aligned)
is a *different* spelling (rect x = `x - Globals::layout->field_0x94`) that
allocates the `sl`/`fp` pair more favourably but has a 19-byte conversation-
branch register swap and the 4-instruction clamp arm.

The whole 41-instruction aligned residual of the original shape is the single
`pad = fp` (vs `r9`) / `x = r9` (vs `r5 -> r3 -> r9`) colouring. It is
blocked by greedy RA: `x` carries a physical hint, is evicted from `r5` by
`above` exactly as in the original, but is then re-enqueued *whole* into the
free `r9` (because `pad` was spilled to `fp`), so `pad` never gets `r9` and
`x` is never split at the `x - pad` use. Every instrumented-clang hint
(`NOHINT`/`NOHINTCO`/`HINTVREG`/`PRIOVREG`/`SIZEVREG`/`FORCESPLIT`) and every
source spelling tested here leaves the emitted code at 175/41/1948/741.

No size-exact source shape found this session beats the committed **138 raw**
score, so no `src/` change is landed. The 175/41 shape is recorded here as the
authoritative original-source reconstruction for the next attempt.

## 5. Reproduction

```bash
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN11SpaceLounge10drawLoungeEv
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w13629` (`measure.sh`,
`icompile.sh`, `hv.py`, `hv2.py`, `gen.py`, `dis.py`, `lsda.py`,
`base.cpp`, build logs). Instrumented clang:
`/tmp/llvm_android_702b/build2/bin/clang++` (`RA_TRACE`, `NOHINT`,
`NOHINTCO`, `HINTVREG`, `PRIOVREG`, `SIZEVREG`, `FORCESPLIT`; vreg numbers
are `0x80000000 + index`; in the 175 shape `%2`/`%647`=pad, `%609`=x,
`%381`=pad2, `%1`=agent, `%369`=Globals::layout GOT temp).
