# SpaceLounge::drawLounge — wave-191b: draw-order / literal / branch-polarity sweep (w13709)

Status: research note (2026-11-13). Worktree `wt/w13709`, branch
`wave191b-w13709`, base `d4e8f032d`. **No `src/` change landed** — every
size-exact source shape tested keeps or worsens the committed raw score.
Tree is clean apart from this note.

Target: `_ZN11SpaceLounge10drawLoungeEv` (1948/1948 B).

* baseline: raw **138**, aligned **87**, size **1948/1948**, insn **739/741**,
  `.ARM.extab` diff **577 B** (`.ARM.exidx` 0, `.rodata` 0, `.text` 71351)
* after: unchanged (note only)

Prior work read first: `spacelounge-drawlounge-wave191b-w13675.md`,
`...w13629.md`, `...w13620.md`, `...w13566.md` (and the wave-190 notes they
cite). This note adds ~250 fresh measured cells on the three requested
angles (independent draw/store order; literal-pool / constant spelling;
branch polarity / loop form) plus a per-region raw-byte split of the residual.

## 0. Harness (reproduces the committed base exactly)

`scratch/w13709/measure.sh <cand.cpp>` compiles the candidate with the exact
`ninja -t commands` clang line and runs the saved gold-link command directly
(no ninja relink). It reproduces the committed base at
**138 / 87 / 1948 / 739**, `.ARM.extab` 577, `.ARM.exidx` 0, `.rodata` 0,
`.text` 71351. `scratch/w13709/extab.py` reports the section deltas;
`scratch/w13709/sbs.py` prints the aligned instruction diff.

## 1. Per-region raw-byte split (new)

Using the function-relative region boundaries `conversation [0,0x14a)`,
`overview [0x14a,0x6ec)`, `epilogue [0x6ec,0x79c)`:

| shape | conv | overview | epilogue | total |
|---|---:|---:|---:|---:|
| committed base (rect x = `x - Globals::layout->field_0x94`) | 7 | 94 | 37 | **138** |
| 175 shape (rect x = `left`, `left/above` locals) | **0** | 138 | 37 | **175** |

So the committed base wins raw *because its overview instruction lengths line
up better with the original*, even though its conversation branch has a 7-byte
`sl`/`fp` swap. The 175 shape has the conversation branch byte-identical but
pays 44 more raw bytes in the overview. The 37-byte epilogue delta (EH landing
pads + literal pool) is identical in both shapes and is downstream of the code
layout, not separately fixable.

## 2. The residual is exactly the `pad` colouring (re-confirmed)

The original overview is the 175 shape: `pad -> r9`, `pad2 -> sl`,
`x = r5 -> r3 -> r9` (split at the `x - pad` use). The committed base is
`pad -> sl`, `pad2 -> fp`, `x -> r9` (whole). The 175 shape is `pad -> fp`,
`pad2 -> sl`, `x -> r9` (whole).

Instrumented clang (`/tmp/llvm_android_702b/build2/bin/clang++`, knob values
are decimal `0x80000000+index`; `%2`=pad, `%609`/`%610`=x, `%381`=pad2):

| build | raw | aligned | size | insn | extab |
|---|---:|---:|---:|---:|---:|
| base, no knob | 138 | 87 | 1948 | 739 | 577 |
| **base + `SIZEVREG=2147483650:9500`** | **123** | **72** | 1948 | 739 | **582** |
| base + `SIZEVREG=2147483650:12000` | 123 | 72 | 1948 | 739 | — |
| base + `SIZEVREG=2147483650:20000` | 243 | 92 | 1948 | 739 | — |
| 175 shape, no knob | 175 | 41 | 1948 | 741 | 580 |
| 175 + `HINTVREG=2147483650:75 NOHINT=2147484257` | 199 | 78 | 1948 | 741 | — |
| 175 + `HINTVREG=...:75 NOHINT=... SIZEVREG=GOT:6000` | 235 | 101 | 1948 | 741 | — |
| base + `HINTVREG=2147483650:75` | 1872 | 222 | 1984 | 738 | — |
| base + `NOHINT=2147484258` (x) | 155 | 106 | 1948 | 739 | — |

`SIZEVREG=pad:9500` on the committed base gives the **123 / 72 / extab 582**
cell reported in w13675 (net −10 bytes including the extab +5): it flips
`pad -> fp`, `pad2 -> sl` and makes the conversation branch byte-identical
while keeping the base schedule. It is not reachable from source: `pad`'s
enqueue size is 6400 (base) / 6400 (175) and every source spelling tested
leaves it below 9500. Forcing `pad -> r9` (`HINTVREG=...:75`) on either shape
does not reproduce the original either (base schedule 1872, 175 schedule 199),
so the original's `pad r9 / pad2 sl / x split` triple is still unreached.

## 3. Fresh measured cells (all real link, `tools/fn_diff.py`)

Base = **138 / 87 / 1948 / 739**, extab 577. All rows size 1948/1948 unless
noted. `extab` shown where it changed.

### 3.1 Independent draw/store order (angle a)

All 12 valid permutations of `{pad, SetColor, centerX, top}` with `pad` before
`top`: **138/87**, except `SetColor` first: **1872/208/1976**.

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `lineHeight` before `int x` | 606 | 88 | 1952 |
| `x` before `name` / before `width` | 138 | 87 | 1948 |
| `x = centerX` inline (no `centerX`) | 138 | 87 | 1948 |
| `if (mode==0){...return;} conv` (base) | 138 | 87 | 1948 |
| `if (mode==0){...} else {conv}` | 138 | 87 | 1948 |
| `if (mode!=0){conv;return;} overview` (inverted) | 180 | 161 | 1948 |
| inverted + 175 shape | 216 | 109 | 1948 |
| `bottomRight` declared before `topLeft` | 138 | 87 | 1948 |
| `Vector` refs / pointers instead of copies | 230 | 124 | 1948 |
| float `tlX/brX/brY` locals | 138 | 87 | 1948 |
| cache `hoverAgent` local | 138 | 87 | 1948 |
| cache `isKnown()` (1 call) | 1354 | 238 | 1948 |
| cache `isKnown()` late | 1869 | 272 | 1976 |
| `role` as if/else chain | 1940 | 226 | 2060 |
| cache `Globals::Canvas` in overview | 1855 | 264 | 1964 |
| cache `Globals::font` in overview | 646 | 119 | 1912 |
| `Layout *const layout` / `const int pad` / `const int centerX/top` | 138 | 87 | 1948 |
| `Layout *layout` before `pad` (pad via `layout`) | 943 | 161 | 1948 |
| conversation: `inset`/`boxHeight` swapped, `stacked` first | 138 | 87 | 1948 |
| conversation: `panelW`/`panelH` locals | 1646 | 192 | 1936 |
| conversation: `panelH` local only | 138 | 87 | 1948 |
| conversation: cache `canvas` | 1783 | 163 | 1964 |
| conversation: `layout` before `SetColor` | 1828 | 183 | 1968 |
| conversation: `SetColor` after `drawBox` | 1762 | 214 | 1940 |

### 3.2 Literal-pool / constant spelling (angle b)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `SetColor(0x193043FF)` / `SetColor(-1)` / `0xff8000ff` (no `u`) | 138 | 87 | 1948 |
| `0.5f` -> `(1.0f / 2.0f)` | 138 | 87 | 1948 |
| `(int)(float)(...)` double cast on `centerX` / `top` | 138 | 87 | 1948 |
| `(unsigned int)` / `(unsigned)` / `(size_t)` font cast | 138 | 87 | 1948 |
| drawBox `String("", false)` / `String("", true)` | 138 | 87/88 | 1948 |
| rect x `x - *(int*)((char*)Globals::layout + 0x94)` | 138 | 87 | 1948 |
| rect x `x - layoutMetric(Globals::layout, 0x94)` | 138 | 87 | 1948 |
| rect x `x - (pad + 0)` / `(pad - 0)` / `padCopy` | 175 | 41 | 1948 |
| `unsigned`/`long` `width`, `top`, `centerX`, `x` | 138 | 87 | 1948 |
| `unsigned int x` | 139 | 88 | 1948 |
| `int pad2 = pad + pad` (top only, base arm) | 138 | 87 | 1948 |
| `int pad2 = pad << 1` (top only) | 138 | 87 | 1948 |
| named `pad2` in top+arm (base schedule) | 1553 | 167 | 1960 |
| named `pad2` in top+arm+width (base schedule) | 1553 | 167 | 1960 |

The rect-x spelling is a hard binary switch: any spelling whose value is a
**reload of `field_0x94`** gives 138; any spelling whose value is the `pad`
register gives 175. `x - pad`, `x - (pad+0)`, `x - padCopy` all give 175.

### 3.3 Branch polarity / loop form (angle c)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `__builtin_expect(isKnown(),1)` (committed) | 138 | 87 | 1948 |
| `isKnown()!=0`, `isKnown()==1`, `isKnown(),2`, `isKnown(),1u` | 138 | 87 | 1948 |
| `__builtin_expect(!isKnown(),0)` | 1846 | 225 | 1972 |
| hint on 2nd `isKnown` (1) / (0) | 542 / 1873 | 109 / 205 | 1952 / 1976 |
| hint on name `isKnown` | 1582 | 273 | 1960 |
| hint on `hoverAgent<0` | 977 | 281 | 1948 |
| hint on `mode==0` | 1854 | 642 | 1968 |
| left clamp `!(x-pad >= inset)` | 138 | 87 | 1948 |
| left clamp `inset > x-pad` | 140 | 89 | 1948 |
| left clamp `x < inset+pad` | 542 | 102 | 1944 |
| right clamp `!(pad+x+width <= w-inset)` | 138 | 87 | 1948 |
| right clamp `w-inset < pad+x+width` | 139 | 88 | 1948 |
| top `pad+pad` / `pad*2` / `2*pad` / `pad<<1` (arm A0) | 138 | 87 | 1948 |
| arm `-(width+pad+pad)` | 139 | 88 | 1948 |
| arm `-(width+pad*2)` / `-(width+2*pad)` / `-(width+(pad<<1))` | 1553 | 167 | 1960 |

## 4. Conclusion

The committed base remains the best size-exact raw shape:
**138 / 87 / 1948 B / 739 insns / extab 577**. No tested source shape beats
raw 138; the 175/41 shape is the structural original but scores raw 175. The
only measured improvement is the source-unreachable instrumented-clang cell
`SIZEVREG=pad:9500` -> **123/72/extab 582** (net −10 bytes). The prize stays
the `pad -> r9` colouring, which needs `pad` selected before `x` *and* `x`
split at the `x - pad` use; every source spelling tested here reaches at most
one of the two. No `src/` change is landed.

## 5. Reproduction

```bash
# scratch/w13709/measure.sh <candidate.cpp>   (direct gold link, no ninja)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN11SpaceLounge10drawLoungeEv
python3 /home/debian/GalaxyOnFire2/scratch/w13709/extab.py
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w13709` (`measure.sh`,
`measure_inst.sh`, `cc_cmd.txt`, `link_cmd.txt`, `extab.py`, `sbs.py`,
`gen.py`..`gen23.py`, `base.cpp`, `shape175.cpp`, `v1.cpp`, `inv.cpp`,
`ifelse.cpp`, `ra*.log`). Instrumented clang:
`/tmp/llvm_android_702b/build2/bin/clang++` (`RA_TRACE`, `NOHINT`,
`HINTVREG`, `SIZEVREG`, `PRIOVREG`, `FORCESPLIT`; decimal vreg =
`0x80000000 + index`; base `%2`=pad, `%610`=x; 175 `%2`=pad, `%609`=x,
`%381`=pad2).
