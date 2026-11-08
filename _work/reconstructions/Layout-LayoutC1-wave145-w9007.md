# `Layout::Layout` C1/C2 — wave 145 / worker w9007
## The `if (iPadHD) { HD } else { shared }` split is a RED HERRING: it shrinks the ctor to 5,868 B. The flat ternary chain is already structurally right (2,494 insns vs the original's 2,523). No size-neutral win found (raw diff stays **5,704 @ 7,072 B**).

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr `0xd1028`,
size **7072 B**, source `src/game/ui/Layout.cpp`.  Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`, image base 0x10000.
Baseline = wave-144 master `eb13ada9b` (target-function raw same-address diff **5,704**).

## 1. New ground truth: the original's exact device-major block layout

Parsing every store in the original (expanding `vst1.32` 4-word groups, `strd` pairs and
`stmia`) gives five disjoint regions and their **exact field sets**:

| region | vaddr range | bytes | fields (offset) |
|---|---|---:|---|
| prologue | `d1028..d1042` | 26 | — |
| **HD arm** | `d1042..d1268` | 550 | `0x4..0x2d8` incl. `0x94,0x98,0x9c,0xa0,0xd0,0xd4,0xd8` (via `stmia`), **except `0x1e4,0x1e8`** |
| **non-HD arm** | `d126c..d21e2` | 3,958 | same 130 words as the HD arm (`n9` arm `d127e..d12fc` then retina/iPad arm `d1570..d185e`, then shared tail `d1860..d21e2`) |
| common 1 | `d21e2..d2204` | 34 | `0x1e4`, `0x1e8` |
| **block2 if/else** | `d2204..d24de` | 730 | `0x1ec,0x1fc,0x204..0x280`, `0x3e0,0x3e4,0x3e8,0x3f4,0x3f8` |
| common 2 | `d24de..d2800` | 802 | `0x1f0,0x1f4,0x1f8,0x200,0x288,0x28c,0x290,0x2bc,0x2f4..0x320,0x3ec,0x3f0,0x3fc` |
| cold blocks | `d2802..d2852` | 80 | out-of-line `if/else` tails |
| literal pools | interleaved | ~1.5 KB | — |

So the original **is** a device-major two-block `if (iPadHD) {...} else {...}` followed by a
second `if (iPadHD) {...} else {...}` and two common islands.  Inside the non-HD arm the
`n9` test is a real branch (`d1276 bne d1570`), not a ternary.

## 2. Measured: reconstructing that split *shrinks* the ctor by 1,204 B

I generated the exact structure above mechanically from the current flat source (split each
`iPadHD ? HD : rest` into the two arms, split the `if (iPadHD)` chains, and put the
`0x1e4/0x1e8` and common-2 fields in the common islands).  All variants built with
`link_so`, scored with raw same-address diff:

| variant | size | raw diff | insns | vbsl |
|---|---:|---:|---:|---:|
| HEAD (flat ternary chain) | **7072** | **5704** | 2494 | 7 |
| original | 7072 | — | 2523 | 11 |
| full single `if (iPadHD)/else` split | 6388 | 5729 | — | 13 |
| **two-block split (exact original field sets)** | **5868** | **5432** | 2264 | 13 |
| two-block + `n9` split | 5768 | 5347 | — | — |
| two-block + full ternary→if/else flatten | 5788 | 5353 | — | — |
| two-block, else arm keeps `iPadHD ? ...` | 5948 | 5506 | — | — |
| flat + all `n9` ternaries → if/else | 7056 | 5804 | — | — |
| flat + five-run contiguous | 6992 | 5913 | — | — |
| flat + five-run contiguous + `n9` split | 6976 | 5899 | — | — |
| flat + five-run contiguous (single run `b0`) | 6964 | 5793 | — | — |
| flat + `c0` run contiguous | 7024 | 5748 | — | — |
| flat + `108` run contiguous | 7024 | 5811 | — | — |
| flat + `2a0` run contiguous | 6984 | 5864 | — | — |
| flat + `ec` run contiguous | 7000 | 5882 | — | — |

**The split removes ~1,204 B of per-field `cmp iPadHD / it eq / moveq` overhead and the
compiler then re-vectorises; it never comes back.**  The instruction counts prove it: the
flat source is 2,494 insns (original 2,523, only 29 short) while the two-block split is
2,264 insns (259 short).  The original is *not* a hoisted-HD-arm source; it is the flat
ternary chain with four more SLP groups (`vbsl` 11 vs 7, `vst1` 81 vs 70, `vld1` 86 vs 77).

## 3. Where the residual really is

The flat source already matches the original's total size exactly and is within 29
instructions.  The 5,704-byte residual is therefore **arrangement**, not missing structure:
the original has 4 extra vector groups (`0xb0..0xcc`, `0xec..0xf8`, `0x108..0x114`,
`0x2a0..0x2ac`, `0x1b0..0x1e0`) and correspondingly ~100 B less scalar code.  Making any of
those runs contiguous in the flat source *does* create the group but drops the size below
7,072 (see table), so the original must pair each group with ~40-80 B of compensating scalar
code that we do not have — most likely the `n9` branch that the original keeps explicit
(`d1276 bne d1570`) while our ternaries fold it.

## 4. Not landed

No source change: every faithful variant either breaks `size_exact` (4,524/4,524) or raises
the raw diff.  The wave-142/143/144 flat statement order remains the best size-neutral local
optimum at **7072 B / 5,704**.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9007
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9007/fdiff.py _ZN6LayoutC1Ev   # 7072/7072, diff 5704
```

Scratch (`/dev/shm/w9007/`): `fdiff.py`, `split.py` (statement splitter), `gen_split.py`
(single split), `gen2.py` (two-block exact), `gen3.py` (+`n9`), `gen5.py` (+flatten),
`gen4.py` (else keeps `iPadHD`), `gen6.py` (duplicate block1), `gen_flatn9.py`,
`reorder.py`/`reorder_one.py` (five-run reorder), `blocks3.py` (store-set extractor),
`hdr2.py` (header offset map), `Layout.base.cpp`, `ctor_*.txt` (generated ctors).

## 6. Open questions

* The flat source is 29 instructions short and 4 `vbsl` groups short.  Find the ~40-80 B of
  scalar code the original pairs with each SLP group *without* changing the total size — the
  explicit `n9` branch (`d1276 bne d1570`) is the prime suspect, but a naive `if (n9)/else`
  rewrite of the non-HD expressions shrinks the ctor (5,768/5,788), so the branch must be
  localised to the fields in the five `vbsl` runs only.
* Do **not** pursue the whole-ctor `if (iPadHD)/else` split: it is measured 1,204 B too small.
