# SpaceLounge::drawLounge — wave-191b: pad-load / reload / clamp sweep (w13737)

Status: research note (2026-11-13). Worktree `wt/w13737`, branch
`wave191b-w13737`, base `22c4b25f5`. **No `src/` change landed** — every
size-exact source shape tested keeps or worsens the committed raw score.
Tree is clean apart from this note.

Target: `_ZN11SpaceLounge10drawLoungeEv` (1948/1948 B).

* baseline: raw **138**, aligned **87**, size **1948/1948**, insn **739/741**,
  `.text` diff **72646**, `.ARM.extab` diff **564**, `.ARM.exidx` 0,
  `.rodata` 0
* after: unchanged (note only)

Prior work read first: `spacelounge-drawlounge-wave191b-w13709.md`,
`...-w13675.md`, `...-w13629.md`, `...-w13620.md`, `...-w13566.md`,
`spacelounge-drawlounge-wave191-w13554.md` (and the wave-190 notes they cite).
This note adds ~250 fresh measured cells on pad-load placement, redundant
`field_0x94` reloads, clamp forms, `lineHeight` placement, and a direct
instrumented-clang weight probe. It confirms the earlier conclusion and adds
one new fact: **pad -> r9 is reachable from source, but only by adding an
extra `field_0x94` load, which recolours the overview and costs more than it
saves.**

## 0. Harness (reproduces the committed base exactly)

`scratch/w13737/measure.sh <cand.cpp>` compiles the candidate with the exact
`ninja -t commands` clang line and runs the saved gold-link command directly
(no ninja relink). It reproduces the committed base at
**138 / 87 / 1948 / 739**, `.text` 72646, `.ARM.extab` 564.
`scratch/w13737/extab.py` prints the section deltas;
`scratch/w13737/padreg.py` extracts the `ldr.w rX,[rY,#148]` pad load from
the candidate.

## 1. The 175 shape is still the structural original

The `left`/`above` shape (rect reuses `left`/`above`) is 741/741 insns and
has a **byte-identical conversation branch** (0x1dc..0x576), but scores worse
overall:

| shape | raw | aligned | size | insn | .text | extab |
|---|---:|---:|---:|---:|---:|---:|
| committed base | **138** | 87 | 1948 | 739 | **72646** | **564** |
| 175 (`left`/`above`, rect reuses them) | 175 | **41** | 1948 | **741** | 72683 | 567 |
| 175 + rect x = `x - field_0x94` (base rect) | 138 | 87 | 1948 | 739 | 72646 | 564 |

The 175 shape's only remaining colouring is `pad = fp` (original `r9`) and
`x = r9` whole (original `r5 -> r3 -> r9`); `pad2 = sl`, `top = r8`,
`width = r6`, `lineHeight = fp` and the conversation branch already match.
The base wins raw/.text because it keeps `pad = sl`, which makes the
conversation branch pay a 19-byte `sl`/`fp` swap but keeps the overview
schedule.

## 2. New measured cells (real link, `tools/fn_diff.py`)

Base = **138 / 87 / 1948 / 739**, `.text` 72646, extab 564. All rows
size 1948/1948 unless noted. `padreg` is the register of
`ldr.w rX,[rY,#148]`.

### 2.1 `pad` load placement (175 shape)

| placement | raw | aligned | padreg |
|---|---:|---:|---|
| after the agent/rect loads (source order, base) | **175** | **41** | fp |
| after the `hoverAgent` check | 175 | 41 | fp |
| before the `hoverAgent` check | 245 | 73 | fp |
| after `SetColor` | 175 | 41 | fp |
| after `centerX` (before `top`) | 254 | 72 | fp |

No placement moves pad off fp.

### 2.2 `pad` spelling / type (175 shape)

| spelling | raw | aligned | size | padreg |
|---|---:|---:|---:|---|
| `int pad = Globals::layout->field_0x94;` | **175** | **41** | 1948 | fp |
| `+ 0`, `- 0`, `\| 0`, `^ 0`, `* 1`, unary `+` | 175 | 41 | 1948 | fp |
| `(int)(long)`, `(int)(unsigned)`, `(int)(double)` | 175 | 41 | 1948 | fp |
| `layoutMetric(Globals::layout, 0x94)` | 175 | 41 | 1948 | fp |
| `Layout *l = Globals::layout; l->field_0x94` | 175 | 41 | 1948 | fp |
| `Layout &l = *Globals::layout; l.field_0x94` | 175 | 41 | 1948 | fp |
| `Layout *const l = Globals::layout; l->field_0x94` | 175 | 41 | 1948 | fp |
| `int pad0 = ...; int pad = pad0;` (copy coalesced) | 175 | 41 | 1948 | fp |
| `(int)(unsigned short)field` (`ldrh`) | 176 | 41 | 1948 | fp |
| `(int)(short)field` (sign-extend) | 1506 | 219 | 1964 | sl |
| conditional `field > 0 ? field : 0` | 1473 | 133 | 1952 | r4 |

### 2.3 `x` spelling (175 shape)

| spelling | raw | aligned |
|---|---:|---:|
| `int x = centerX;` | **175** | **41** |
| `centerX + 0`, `* 1`, `\| 0`, `-(-centerX)`, `(int)centerX` | 175 | 41 |
| `int x = (int)(...)` inline (no `centerX`) | 1133 | 117 |
| separate `textX`/`clampX` locals | 175 | 41 |
| `left = centerX - pad` | 1870 | 176 |

### 2.4 Clamp forms (175 shape)

| form | raw | aligned | size |
|---|---:|---:|---:|
| committed `if/else if` + `+(-width-pad-pad)` | **175** | **41** | 1948 |
| arm `-(width+pad+pad)` | 176 | 42 | 1948 |
| arm sequential `-width-pad-pad` | 468 | 64 | **1944** |
| `SPACELOUNGE_MIN/MAX` | 561 | 95 | **1936** |
| limit locals + `if/else if` | 560 | 88 | **1936** |
| ternaries | 561 | 95 | **1936** |
| swap the two conditions | 493 | 94 | 1952 |
| `x < inset+pad` left test | 492 | 73 | **1944** |
| `w-inset < pad+x+width` right test | 176 | 42 | 1948 |
| nested `else { if }` | 175 | 41 | 1948 |

### 2.5 `lineHeight` placement (175 shape)

| placement | raw | aligned | size | padreg |
|---|---:|---:|---:|---|
| after the clamp (source order) | **175** | **41** | 1948 | fp |
| after `pad` / after `SetColor` | 1848 | 233 | 1968 | r4 |
| after `top` | 1861 | 251 | 1972 | r5 |
| before the clamp | 588 | 79 | 1952 | r5 |

### 2.6 Branch hints (175 shape)

| hint | raw | aligned | size | padreg |
|---|---:|---:|---:|---|
| `__builtin_expect(mode==0, 1..16)` | 1853 | 635 | 1968 | fp |
| `__builtin_expect(hoverAgent<0, 0)` | 1026 | 274 | 1948 | fp |
| `__builtin_expect(hoverAgent<0, 1)` | 1628 | 354 | 1948 | fp |
| left-clamp hint 0 | 492 | 71 | 1952 | fp |
| right-clamp hint 0 | 496 | 74 | 1952 | fp |
| second `isKnown` hint 0 | 1145 | 173 | 1952 | fp |

## 3. New finding: a redundant `field_0x94` reload flips pad to r9

The pad load is a plain `t2LDRi12` with no allocation hint, so it is always
selected after the hint-carrying vregs; by then `sl`/`fp` are the only free
callee-saved registers. Adding a **second** `Globals::layout->field_0x94`
load changes the allocation:

| shape | raw | aligned | size | padreg | .text | extab |
|---|---:|---:|---:|---|---:|---:|
| base + rect y = `top - field_0x94` (`recty`) | 310 | 90 | 1948 | **r9** | 72818 | 574 |
| 175 + rect x = `x - field_0x94` | 138 | 87 | 1948 | sl | 72646 | 564 |
| 175 + `left = x - field_0x94` | 573 | 163 | **1956** | **r9** | 234556* | 1271* |
| 175 + `above = top - field_0x94` | 565 | 83 | **1956** | r5 | — | — |

`*` size grows -> all downstream functions shift, gate-fatal.

`recty` is the interesting cell: pad -> r9 and `x -> r5`, and the
**conversation branch becomes byte-identical**, but the overview recolours
badly (`top -> fp`, `lineHeight -> sl`, the clamp arm stays 4 insns), so raw
rises to 310 and `.text` to 72818. The extra load costs more than the pad
colouring saves.

## 4. Instrumented-clang weight probe (new)

`/tmp/llvm_android_702b/build2/bin/clang++` has a `VRW=<rawvreg>:<factor>`
knob (multiplies the spill weight in `CalcSpillWeights`; unlike `HINTVREG`
it is **not** function-name-filtered, so `.text` is polluted, but the
drawLounge body is valid). On the 175 shape (`pad = 0x80000002`):

| knob | raw | aligned | size | insn | padreg |
|---|---:|---:|---:|---:|---|
| none | **175** | **41** | 1948 | 741 | fp |
| `VRW=2147483650:1.05..1.35` | 203 | 46 | 1948 | 741 | fp |
| `VRW=2147483650:1.4` | 194 | 63 | 1948 | 741 | **r9** |

At 1.4 pad finally evicts x from r9 (`pad.weight > x.weight`), giving
`pad r9 / x r5` — but `top -> fp` and `lineHeight -> r8`, the reverse of the
original (`top r8 / lineHeight fp`), so raw is still 194. This confirms the
original's pad weight must be ~40 % above ours, and that pad -> r9 alone is
not sufficient: the top/lineHeight pair must also fall the other way.

The earlier `SIZEVREG=2147483650:9500` cell (123/72) also applies to every
function in the TU (no name filter), so its `.text` 73145 is not a clean
drawLounge-only measurement; the drawLounge-relative 123/72 is real but the
source shape that reaches it is still unknown.

## 5. Conclusion

The committed base remains the best size-exact source shape:
**raw 138 / aligned 87 / 1948 B / 739 insns / .text 72646 / extab 564**.
The 175 shape is the structural original (741/741, conversation exact) but
scores 175 raw and 72683 `.text`. The residual is the single
`pad r9 / pad2 sl / x r5->r3->r9` triple; pad is selected late (no hint) and
only an extra `field_0x94` reload (which recolours the overview) or an
instrumented weight bump (which swaps top/lineHeight) reaches `pad r9`.
No `src/` change is landed.

## 6. Reproduction

```bash
# scratch/w13737/measure.sh <candidate.cpp>   (direct gold link, no ninja)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN11SpaceLounge10drawLoungeEv
python3 /home/debian/GalaxyOnFire2/scratch/w13737/extab.py
python3 /home/debian/GalaxyOnFire2/scratch/w13737/padreg.py
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w13737` (`measure.sh`,
`measure_inst.sh`, `cc_cmd.txt`, `cc_inst.txt`, `link_cmd.txt`, `extab.py`,
`padreg.py`, `sweep*.py`, `base.cpp`, `v_175.cpp`, `c9_recty.cpp`,
`c10_175_rectx.cpp`, `ra*.log`, `wdbg*.log`).
