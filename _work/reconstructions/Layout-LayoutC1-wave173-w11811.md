# `Layout::Layout` C1/C2 — wave 173 / worker w11811

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Baseline (master `3fafa5b03`): raw **5,640**, aligned **2,282**, 7,072 B,
2,519 / 2,552 instructions.
Exit: **no code change landed** (nothing kept size 7,072 and lowered raw).

Scope: the `0x2c0`/`0x2c4`, `0x2b4`/`0x2b8`, `0x94..0xa0` and n9 4-store
groups, plus the "missing code" hypothesis.

## The store set is complete — the missing code is per-branch duplication

A dataflow parse of every store in the function (immediate `str`/`strd`/
`vstr`, `vst1.32` 4-word groups, and `stmia` with a resolved `r0+imm` base)
gives **201 distinct field offsets in both binaries**.  No field that the
original writes is absent from our reconstruction.  What differs is the
**number of stores per field** (branch-local stores vs. our flat ternary
cascade):

| offset | original stores | ours | note |
|---|---:|---:|---|
| 0x94  | 3 | 2 | iPadHD + n9 + retina vs. ours tail-only |
| 0x140 | 3 | 1 | one store per device branch |
| 0x3e8 (rewardBoxWidth)  | 5 | 1 | one per reward-box branch |
| 0x3ec (rewardBoxHeight) | 2 | 1 | |
| 0x3f4 (rewardBoxY)      | 3 | 2 | |
| 0x2cc | 5 | 2 | |
| 0x2d0 | 2 | 5 | ours sinks it into every branch |
| 0x2bc | 1 | 5 | |

The original has **8 `stmia` groups**, ours has **3**:
`0x94..0xa0`, `0xd0..0xd8`, `0x2c0..0x2cc`, `0xc..0x14`,
`0x2b0..0x2b8`, `0x118..0x124`, `0x194..0x19c`, `0x2fc..0x304`
(original) vs. `0x7c..0x84`, `0x1f0..0x1f8`, `0x2fc..0x304` (ours).
The original also has more `strd` pairs.  So the merges are not caused by
a missing field store; they are caused by the original source's **nested
`if/else` device dispatch with stores duplicated in each branch**, which
keeps adjacent stores adjacent and keeps shared local variables alive.

## Why the `(int)(float)` chains survive in the original

The original keeps a runtime `vmov s0,rN; vcvt.f32.s32; vcvt.s32.f32;
vstr` for `0x2d0` at `d1860..d1882` (shared merge point), while our flat
ternary is constant-folded to a plain `str`.  The Ghidra decompile
(`_work/reconstructions/layout-orig-ghidra-decompile.txt`, LAB_e1860)
shows the original assigns an `int` local in the `0x2cc` branches and
converts it **after** the branch merge:

```cpp
if (...) { field_0x2cc = ...; uVar45 = 60; goto L; }   /* side effect */
...
L:  this->field_0x2d0 = (int)(float)uVar45;
```

Because the branches also store `0x2cc`, SimplifyCFG cannot turn the
local into a select, so the conversion is not folded.  Reproducing the
`if/else` shape does emit the `vcvt` chain, but it tail-merges the rest
of the region and shrinks the body to 6,964 B.

## Measured shapes (real `link_so` + `fn_diff`; size must stay 7,072)

| shape | raw | aligned | size |
|---|---:|---:|---:|
| baseline | **5,640** | **2,282** | **7,072** |
| `0x2d0` after `0x2cc` (n9 3-store `stmia 0x2c4..0x2cc`) | 5,644 | **2,277** | 7,072 |
| `0x2d0` before `0x2cc` (clang normalises, no code change) | 5,640 | 2,282 | 7,072 |
| `0x94` before `0x98` (iPadHD+n9+retina 0x94 stores) | 5,955 | 2,311 | 7,032 |
| `0x94` after `0xa0` | 5,770 | 2,434 | 7,024 |
| `0x94` before `0x9c` | 5,890 | 2,291 | 7,048 |
| `0x2b8` after `0x2b4` (`strd 0x2b4/0x2b8`) | 5,884 | 2,389 | 7,024 |
| `0x2c0` before `0x2c4` (`strd 0x2c0/0x2c4`) | 6,066 | 2,419 | 7,044 |
| `0x2c0` after `0x2c4` | 5,988 | 2,353 | 7,056 |
| `0x140` as 4-way `if/else` (duplicate branch stores) | 6,039 | 2,611 | 7,152 |
| `rewardBoxY` as 5-way `if/else` | 5,965 | 3,882 | 7,104 |
| `0x2cc`/`0x2d0` as shared-variable `if/else` (vcvt restored) | 5,965 | 2,503 | 6,964 |
| ascending `0x34,0x38,0x3c,0x44` (w11609) | 5,682 | 2,102 | 7,072 |
| ascending + `0x2d0` after `0x2cc` | 5,685 | 2,097 | 7,072 |

Full 3^1 x 2 x 5 x 2 grid over the four scoped moves: the only size-7,072
states are the baseline and `0x2d0 after 0x2cc`.  A subset search over
`{0x140 if/else, rewardBoxY if/else, 0x94 before/after 0x98/0x9c/0xa0,
0x2b8 after 0x2b4, 0x2c0 before/after 0x2c4, 0x2d0 after/before 0x2cc,
rewardBoxWidth/Height/0x2cc if/else}` (all combinations up to 4 moves)
found **no size-7,072 state with raw < 5,640**; the best is the baseline.
Random single- and two-statement relocation searches (150 + 200 samples)
likewise found nothing better at size 7,072.

## Conclusion

The baseline's scrambled statement order is a strong local optimum that
holds exactly 7,072 B.  The original's merges come with **more code, not
less**: duplicated branch stores (`0x140`, `0x3e8`, `0x94`, `0x2cc`) and
a shared-variable `if/else` that keeps the `0x2d0` `(int)(float)` chain
alive.  Landing any faithful merge requires reconstructing the whole
nested device dispatch (first half + reward box + tail cascade) at once,
not just the scoped groups.  The closest faithful step is
`0x2d0 after 0x2cc`, which forms a 3-store `stmia 0x2c4..0x2cc`, keeps
size 7,072 and improves aligned 2,282 -> 2,277, but costs +4 raw bytes
(5,640 -> 5,644), so it was not landed.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11811
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w11811/`: `Layout.base.cpp`
(baseline source), `harness.py` (build + `fn_diff` + restore),
`parse_stores.py`/`union_stores.py`/`stmias.py` (store-map parsers),
`adiff.py` (aligned diff), `grid.py`/`subset.py`/`randsearch.py`/
`randsearch2.py`/`grow.py` (move searches), `orig.dis`/`ours.dis`
(full disassembly).
