# `Layout::Layout` C1/C2 — wave 191b / worker w13519

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, linked symbol size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave191b-w13519`, base master `5c99979c4`.

**Result: one size-exact raw win landed.**

| | before | after |
|---|---:|---:|
| raw | 5,243 | **5,241** |
| aligned | 2,728 | **2,726** |
| size | 7,072/7,072 | 7,072/7,072 |
| insn | 2,521/2,552 | 2,521/2,552 |

`check` = RATCHET PASS: byte_exact 4507, linked_exact 4507, data 843/843,
imports 207, lint CLEAN, drift 0.

## The win

`field_0x7c` was a 3-arm ternary chain; spelling it as an `if / else if / else`
chain is codegen-identical in size (7,072) but two bytes closer to the original:

```cpp
    if (Globals::iPadHD)
        this->field_0x7c = 12;
    else if (Globals::n9)
        this->field_0x7c = 13;
    else if (Globals::retinaDisplay)
        this->field_0x7c = 18;
    else
        this->field_0x7c = 9;
```

`field_0x84` and `field_0x88` are interchangeable with `field_0x7c` for this
cell (each alone also gives 5,241 / 2,726 / 7,072); combinations do not stack
(`0x7c+0x84` = 5,241, `0x7c+0x88` = 5,841). `field_0x7c` was chosen.

## How it was found

A complete ternary→if/else sweep of every multi-ternary single-line assignment
in the ctor (`scratch/w13519/tern2if.py`, ~120 builds). Only these three lines
were size-exact and raw-positive; everything else was neutral or size-fatal.
This axis was previously untried (earlier waves only went the other way,
if/else → ternary).

## Negative results this session (do not redo)

All measured with `link_so -j2` + `fn_diff`; baseline 5,243 / 2,728 / 7,072.

* The wave-191 handoff candidate `field_0x288 = Globals::n9 ? 0 : 20` does give
  5,242 but is **semantically wrong** (the oracle `lay_orig.json` says
  n9→20). The value-preserving `!Globals::n9 ? 0 : 20` is neutral (5,243).
* `field_0x20_top`/`field_0x24`/`buttonInsetX` HD arm `5 * 1.40625f` → `7`
  (int-typed chain): 6,768 / 6,796 / 7,036 — all size-fatal. The float-typed
  chain is required for the original's code size.
* Full iOS-recovered source order (`layout_ctor/ios_source_order.md`, 204
  fields, offset order): 6,370 / 7,148 (+76). Single-statement moves *toward*
  that order: best 6,271. Prefix sorts of the first N units: best 6,363.
* Exhaustive permutation of the `0x2cc / 0x2d0 / 0x2d8 / 0x2c8 / rd / 0x2d4`
  region (120 perms): best stays 5,243.
* Exhaustive consecutive-triple permutations of all statement units (1,190
  builds): best 5,243.
* Declaration-placement sweeps (`t_field_0x2c8`, `rd`, `int v`): neutral or
  worse (`decls_top` = 6,381 / 7,088).
* `0x130` default arm split into `else if (iPad)` (outlines the cold block):
  5,397 / 7,064; combinations with `0x1f8`/`0x1f4` variants do not recover
  7,072.
* `__builtin_expect(iPadHD,0)` at the top-level branch: 6,278 / 7,000;
  `expect(n9,0)`: 6,008 / 7,072.
* `0x18c` chain negated (inlines the extra cold block): 5,725 / 6,928.
* Boolean simplifications (`0x1e8`, `0x114`, `helpWindowX`, `0x2bc`, `0x280`):
  neutral or size-fatal.
* Second-ternary arm swaps (negate+swap, all lines): neutral.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13519
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5241 aligned=2726 size=7072/7072 insn=2521/2552
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13519/`: `Layout.base.cpp`
(pre-win source), `measure.sh`, `tern2if.py`, `batch17.py`, `iosorder.py`,
`trip.py`, `declsweep.py`.

## Addendum — post-win sweeps (all neutral, tree left clean)

Once `field_0x7c` is an if/else chain the baseline is 5,241 / 2,726 / 7,072.
Re-running the ternary→if/else sweep on the new base finds **no further win**:
`choiceWindowTitleHeight`, `eventBannerScale`, `field_0x108`, `0x110`, `0x118`,
`0x11c`, `0x120`, `0x124`, `0x12c`, `0x140`, `0x14c`, `0x170`, … all land on
5,241 exactly. `field_0x80_touchMargin`, `field_0x84`, `field_0x88` are
equivalent to `field_0x7c` (each alone = 5,241); pairing them does not stack
(`0x7c+0x80`, `0x7c+0x84`, `0x80+0x84` = 5,241; `0x7c+0x88` = 5,841).
Pairwise if/else conversion of the 40 multi-ternary lines around the win: no
pair beats 5,241.

The winning if/else chain is form-robust: `cond != 0`, fully braced, and
`else { if … }` nesting all give 5,241 / 2,726 / 7,072.

Other axes measured on the new base (all neutral or size-fatal):
`(int)(float)` → `(int)` / `(int)(double)` / `(int)(float)(int)` at every cast
site (neutral); `field_0xa8` `(int)(float)v` variants (7,064); `int v` →
`float v` (7,064); `0x130`/`0x190`/`field_0x4`/`field_0x280` if/else → ternary
(7,080 / 6,944 / 5,249 / 5,241); `(A||B) ? X : Y` split into nested ternaries
for `0x2d8`, `helpWindowX`, `0x114`, `0x2d4` (all size-fatal); duplicate-arm
merges (`mergearms`) all worse.

Diff of the winning build vs the pre-win build: exactly **one instruction
moves** (`vldr s0,[r5]` swaps places with `movs r4,#60` at `0xd1956`); no size,
instruction-count, pool or `.rel.plt` change.
