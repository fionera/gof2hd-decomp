# SpaceLounge::drawLounge — wave-191b: source-shape + RA-priority sweep (w13675)

Status: research note (2026-11-13). Worktree `wt/w13675`, branch
`wave191b-w13675`, base `7a8dfc0c5`. **No `src/` change landed** — every
size-exact source shape tested keeps or worsens the committed raw score.
Tree is clean apart from this note.

Target: `_ZN11SpaceLounge10drawLoungeEv` (1948/1948 B).

* baseline: raw **138**, aligned **87**, size **1948/1948**, insn **739/741**,
  `.ARM.extab` diff **577 B**
* after: unchanged (note only)

Prior work read first: `...w13629.md`, `...w13620.md`, `...w13566.md`,
`...w13554.md` (and the wave-190 notes they cite). This note adds ~180 fresh
measured cells and one new instrumented-clang finding: the whole 138-byte
residual is the `pad`/`pad2` register pair, and a **priority bump of `pad`
alone** (SIZEVREG >= 9500) drops raw to **123** with the conversation branch
becoming byte-identical — but that colouring is not reachable from source.

## 0. Harness correction (important for reproducing prior cells)

The first sweep harness compiled the candidate `.obj` and then ran
`ninja link_so`; ninja's stored deps log considered the manually-built object
"out of date" and **recompiled the real `src/` file**, silently discarding
every candidate. That is why an early `x - pad` cell measured 138 instead of
175. The corrected harness compiles the candidate with the exact `ninja -t
commands` line and runs the saved gold-link command directly (no ninja).
Every number below is from the corrected harness and reproduces the committed
base at 138/87/1948/739, extab 577.

```bash
# scratch/w13675/measure.sh <candidate.cpp> <label>
#   compile with cc_cmd.txt, then run link_cmd.txt directly
```

## 1. The base/175 colours (confirmed)

```
value        original        base (138)     175 shape (175)
pad          r9              sl (r10)       fp (r11)
pad2         sl (r10)        fp (r11)       sl (r10)
x            r5 -> r3 -> r9  r9 (whole)     r9 (whole)
top          r2 -> r8        r1 -> fp       r2 -> r8
lineHeight   fp              sl (after pad) fp (after pad)
```

The committed base (`DrawRectangle(x - Globals::layout->field_0x94, ...)`) and
the true-source 175 shape (`int left = x - pad; int above = top - pad;` reused
in both calls) are both reproducible. The 175 shape is 741/741 instructions
and aligned 41; the base is 739/741 and aligned 87. The base wins raw because
the pad/pad2 pair lands `sl`/`fp`, which matches the original's conversation
branch (`Globals::Canvas` in `sl`); the 175 shape puts pad in `fp`, which
swaps `sl`/`fp` through the whole conversation branch (16 diff bytes).

## 2. New measured cells (real link, `tools/fn_diff.py` + `.ARM.extab`)

Base = **138 / 87 / 1948 / 739**, extab **577**. All rows size 1948/1948
unless noted. Every cell was built with the corrected direct-link harness.

### 2.1 The `left`/`above` family (re-confirmed)

| variant | raw | aligned | insn | extab |
|---|---:|---:|---:|---:|
| committed base | **138** | **87** | 739 | **577** |
| 175 shape (`left`/`above` reused, rect `left`) | 175 | **41** | 741 | 580 |
| `const int left/above` | 175 | 41 | 741 | 580 |
| `left` only (rect `left`, y inline) | 175 | 41 | 741 | 580 |
| `above` only | 138 | 87 | 739 | 577 |
| `left` used 3x (also in clamp) | 175 | 41 | 741 | 580 |
| `left/above` in rect only | 175 | 41 | 741 | 580 |
| no `lineHeight` local | 175 | 41 | 741 | 580 |
| `int xForBox = x` / `int xForText = x` / both | 175 | 41 | 741 | 580 |
| `DrawString` x = `left + pad` | 175 | 41 | 741 | 580 |
| rect x = `x - pad` (inline, no locals) | 175 | 41 | 741 | 580 |
| `left`/`above` after the drawBox call | 1562 | 202 | 740 | 1972 B size |

### 2.2 Statement order / scheduling (base shape)

All 12 valid permutations of `{pad, SetColor, centerX, top}` with `pad` before
`top` give **138/87/1948** except `SetColor` first (254/72). Moving the `pad`
load before the `hoverAgent` check gives 208/118/1948; before the name/width
block gives 1211-1872/…; hoisting `pad` to function scope gives 1851/250/1976
(the compiler sinks it back to 138 only when the load is unguarded but still
inside the branch). `top` after the clamp gives 1872/200/1972.

### 2.3 Clamp arm spellings (base shape)

| arm | raw | aligned | size |
|---|---:|---:|---:|
| base `+ (-width - pad - pad)` | **138** | **87** | 1948 |
| `- (width + pad + pad)` | 139 | 88 | 1948 |
| `- (width + pad + pad)` via `int sum = width+pad+pad` | 139 | 88 | 1948 |
| `- width - pad - pad` (sequential) | 481 | 100 | **1944** |
| `- (width + pad*2)` / `- (width + (pad<<1))` | 1553 | 167 | 1960 |
| `(w-inset) - (width + pad + pad)` | 1553 | 167 | 1960 |
| `w - (inset + width + pad*2)` | 1559 | 174 | 1964 |
| `-(width+pad+pad)` cast to `unsigned`/`long` | 139 | 88 | 1948 |

The 2-instruction original arm (`addgt.w r1,r6,sl; subgt r5,r0,r1`) is only
emitted by the **sequential** spelling, which shrinks the body to 1944
(gate-fatal). Every 1948-byte spelling keeps the 4-instruction
`itttt/movgt/subgt.w/subgt/addgt.w` form. The 175 shape + sequential arm is
468/64/**1944**/740.

### 2.4 `pad2` locals

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `int pad2 = pad+pad` in top only | 138 | 87 | 1948 |
| pad2 in top+width (named) | 1553 | 167 | 1960 |
| pad2 in top+arm | 1553 | 167 | 1960 |
| pad2 in top+arm+width, 175 shape | 445 | 54 | 1948 |
| pad2-first (`int pad2 = field*2; int pad = field;`) | 138 | 87 | 1948 |

Any *named* pad2 that must survive to the arm grows the body to 1960.

### 2.5 `x`/`centerX`/`top` spellings

`long`/`unsigned`/`double` casts, `(int)(float)`, `float centerXf/topf`
locals, `+0`/`*1`/`^0`, and `bottomRight.x - (bottomRight.x-topLeft.x)*0.5f`
all give 175/41/1948 (or 534-990 for float round trips). `unsigned centerX`
is 176/42. `volatile centerX` is 1471/238/1952.

### 2.6 Branch hints / weights (base shape)

`__builtin_expect(mode==0, …)` 1653-1854 / 1940-1968; `__builtin_expect` on
the right clamp 513-1872 / 1952-1976; `__builtin_expect` on `hoverAgent<0`
977-1628. Per-file `-mllvm -likely-branch-weight` A/B: 1:1/2:1/3:1 give
1976, 5:1 gives 481/1948, **7:1 = 138** (committed), 10:1 = 138, 20:1 = 484.
7:1 is already optimal.

## 3. New finding: the residual is a `pad`/`pad2` pair swap

Instrumented clang (`/tmp/llvm_android_702b/build2/bin/clang++`) with the
`SIZEVREG=<rawvreg>:<size>` knob (override the greedy-RA enqueue size) on the
**committed base source**:

| knob | raw | aligned | size | insn | extab |
|---|---:|---:|---:|---:|---:|
| none (base) | 138 | 87 | 1948 | 739 | 577 |
| `SIZEVREG=%2:6800` | 138 | 87 | 1948 | 739 | 577 |
| `SIZEVREG=%2:7000..9000` | 1862 | 160 | 1972 | 743 | — |
| **`SIZEVREG=%2:9500..12000`** | **123** | **72** | **1948** | 739 | **582** |

`%2` is `pad` (raw vreg 0x80000000+2). At size >= 9500 the allocator colours
`pad -> fp (r11)` and `pad2 -> sl (r10)` — the same pair the 175 shape has —
while keeping the **base schedule** (rect x reload, `left/above` inline after
the `String` construction). With that colouring the **whole conversation
branch becomes byte-identical** (the 16-byte `sl`/`fp` swap disappears); the
remaining 123 bytes are entirely the overview clamp/join plus the downstream
EH landing-pad shift. It is a net win on text (-15) but grows `.ARM.extab` by
5 (577 -> 582), i.e. net -10 bytes.

The source cannot reach it: the base `pad` size is 6528 and every source
spelling tested leaves it below the 9500 threshold. The 175 shape already has
the `fp`/`sl` pair but its schedule (locals before the `String` call) costs
52 more overview bytes. No tested source shape combines the base schedule with
`pad -> fp`.

## 4. Conclusion

The committed flat clamp + `DrawRectangle` pad reload remains the best
size-exact source shape: **raw 138 / aligned 87 / 1948 B / 739 insns /
extab 577**. The 175/41 shape is the structural original but scores 175 raw.
The residual is the single `pad`/`pad2` colouring; the instrumented-clang
`SIZEVREG` probe shows the prize (123/72, conversation branch exact) but the
greedy allocator only reaches it with a priority bump that no source spelling
produces. No `src/` change is landed.

## 5. Reproduction

```bash
# corrected direct-link harness (no ninja relink)
/home/debian/GalaxyOnFire2/scratch/w13675/measure.sh <candidate.cpp> <label>
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN11SpaceLounge10drawLoungeEv
python3 /home/debian/GalaxyOnFire2/scratch/w13675/extab.py \
  cmake-build-match/libgof2hdaa.so
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w13675` (`measure.sh`,
`measure_inst_env.sh`, `measure_bw.sh`, `cc_cmd.txt`, `link_cmd.txt`,
`clusters.py`, `extab.py`, `sweep1.py`..`sweep36.py`, `SpaceLounge.base.cpp`,
`SpaceLounge.pre.cpp`, `ra_base.log`, `ra_base_sz.log`, `dlbase*.log`).
Instrumented clang: `/tmp/llvm_android_702b/build2/bin/clang++`
(`RA_TRACE`, `SIZEVREG`, `HINTVREG`, `NOHINT`; vreg = `0x80000000 + index`;
`%2`=pad, `%45`=above, `%47`=width+pad2, `%380`=pad<<1, `%609`=x).
