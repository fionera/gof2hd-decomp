# `Layout::Layout` C1/C2 — wave 190 / worker w13420

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, linked symbol size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave190-w13420`, base master `03b9dfe03`.

**Result: no size-exact raw win found.  `src/` left at the base (raw 5,249,
aligned 2,725, size 7,072/7,072, insns 2,520/2,552).**  This note records the
three new lenses tried (literal-pool ordering, cold-block/branch layout,
callee-saved rotation) and the measured negatives, so they are not repeated.

## 1. Literal-pool ordering (new lens)

Both functions end their body with a literal pool at `0xd2bc8`; the pools are
*not* the same length and do *not* start at the same address:

| | pool start | pool bytes | words | code bytes |
|---|---:|---:|---:|---:|
| original | `0xd2854` | 884 | 221 | 6,188 |
| ours | `0xd28d8` | 752 | 188 | 6,320 |

The pool start is `0xd1028 + code_size`, so the +132 B of extra code in ours
shifts our pool +132 B (+33 words).  At the same address `A`, ours' pool index
is `i = (A-0xd28d8)/4` and the original's is `i+33`.

* Only **15 of 188** pool words match at the same address (60 B).
* The **last 6 words match exactly** (`0xd2bb0..0xd2bc4` = 200.0, 100.0, 150.0,
  144.0, 150.0, 76.0), because both functions end with the same last statements
  (`field_0x2f8`, `helpWindowY`).
* Longest common run between the two pool sequences is **16 words**
  (ours`[90:106]` == orig`[127:143]`).  The best global sequence offset is
  **+38 words** (39 matching words); the address offset is +33, so the pool
  sequences are misaligned by 5 words even if the code size were fixed.
* The 5-word discrepancy is exactly the pool head: the original's first 8 words
  are `0.325, 0.65, 100.0, 50.0, GOT, GOT, GOT, GOT` (field `0x90` then `0x9c`,
  then four GOT slots), whereas ours' first 5 are `64.0, 32.0, GOT, GOT, GOT`
  (field `0x8c` first).
* In the original, `field_0x8c`'s `64.0/32.0` are **not** in the tail pool at
  all: they live in an inline island at `0xd1d94/0xd1d98`, referenced by
  `addw r5, pc, #1156` at `0xd190e` (the next unconditional branch after the
  use is `b.n d1da6` at `0xd1d7a`, so the island is placed there).  In ours the
  same constants are first tail-pool entries (`0xd28d8`), referenced at
  `0xd19a8`.
* Ours also keeps `0.325/0.65/100.0/50.0` in a *local island* at
  `0xd1918..0xd1930` (`75.0, 100.0, 50.0, 0.325, 0.65, 57.6`), created by the
  `b.n d1976` at `0xd18ec`; the original has no such island there.

Conclusion: matching the pool at same addresses requires (a) reproducing the
original's block/island structure in the non-HD path and (b) shrinking code by
exactly 132 B while growing the pool by 33 words.  Pure statement reorders do
not do this (see §4).

## 2. Cold-block / branch-target layout (new lens)

The original outlines **five** cold tails at `0xd2802..0xd2852` (80 B); ours
outlines **two** at `0xd28b8` / `0xd28ca`:

| orig tail | source arm | continuation |
|---|---|---|
| `0xd2802` | `field_0x130/0x134/0x138` default (`iPad ? 56 : 40`, …) | `b.w d1cba` |
| `0xd2828` | `field_0x2c8` default (`iPad ? 45 : 30`) | `b.w d1814` |
| `0xd2836` | `field_0x2d0` default (`iPad ? 45 : 30`) | `b.w d1860` |
| `0xd2844` | `field_0x60` default (`iPad ? 64 : 44`) | `b.w d16d6` |

Ours outlines `field_0x18c`'s iPad arm (348/174) and `field_0x60`'s default.
The outlining is **not** controlled by ternary-vs-if/else: rewriting
`field_0x2c8` / `field_0x2d0` as flat `if/else if/…/else` chains (with the
`iPad ? 45 : 30` arm kept as the final `else`) is **codegen-identical**
(`5249 / 2725 / 7072`).  The nested form
(`else if (iPad) x=45; else x=30;`) is *worse*: `0x2c8` → `(5423, 2727, 7064)`,
`0x2d0` → `(5419, 2706, 7064)` — matching the earlier w13385 numbers.
`field_0x130` nested → `(5403, 2727, 7064)`.  `__builtin_expect` on the HD
flag (`expect(…,1)`) → `(6198, 4155, 7000)`; on `field_0x5c`'s iPad arm →
`(6103, 2770, 6920)`.

## 3. Callee-saved rotation (new lens)

| | prologue | HD flag | n9 flag | non-HD spill |
|---|---|---|---|---|
| original | `push {r4,r5,r6,r7,lr}` + `stmdb {r6,r7,r8,r9,sl,fp}` | `sl` (`ldrb.w sl,[r1]` at `0xd1036`) | `fp` (`ldrb.w fp,[r1]` at `0xd1272`) | `strd fp, sl, [sp]` at `0xd1570`; `ldrd fp, sl, [sp]` at `0xd16a4/d16d2/d1710` |
| ours | `push {r4,r5,r6,r7,lr}` + `stmdb {r7,r8,r9,sl,fp}` | `lr` (then `mov r5, lr`) | `fp` | `str.w lr, [sp]` at `0xd1568`; `ldr.w lr, [sp]` at `0xd16a4/d16d2/d1710` |

The original keeps the HD flag in a callee-saved register and spills both flags
with `strd`; ours keeps it in `lr` and spills one word.  Frame size differs by
4 B (44 vs 40), which changes `[sp,#4]` vs `[sp]` offsets (`ldr r6,[sp,#4]` in
the original vs `ldr.w r9,[sp]` in ours).  Prologue/epilogue instruction sizes
are identical, so this rotation is size-neutral in principle, but every source
shape tried to force it (`const bool hd`, `int hd`, `volatile`/`register`
variants, `__builtin_expect`) changed the whole schedule and lost raw.

## 4. Measured negatives (do not repeat)

All size-exact unless noted; baseline `(5249, 2725, 7072)`.

* Adjacent + distance-2 statement swaps, **all 459 pairs**, whole ctor body
  (`scratch/w13420/adj2.py`, `adj2.json`): best is identity; 39 pairs are
  neutral `(5249,2725,7072)`, none improves.
* `field_0x9c` ↔ `field_0x90` swap: `(5954, 2896, 7024)`.
* `field_0x8c` moved first in the 0x8c/0x90/0x9c group: `(5701, 2716, 7032)`.
* `field_0x7c/0x80/0x84/0x88` group moved after `field_0x8c`: `(5978, 2775, 6880)`.
* flat `if/else` for `field_0x2c8` / `field_0x2d0`: **neutral** (codegen-identical).
* `field_0x2d8` moved after the `field_0x2d4` if/else: `(6248, 3588, 6880)`;
  before it (after `rd`): `(6253, 3580, 6880)`.
* combined 0x2d4/0x2d8 config-major block: `(6272, 3595, 6868)`.
* `field_0x94` moved next to `field_0x98`: `(5923, 2781, 7008)`; after
  `field_0x8c`: `(5924, 2808, 7008)`; after `field_0xa0`: `(5856, 2874, 6984)`.
* `field_0x2bc` moved before `footerButtonOffset`: `(5984, 2759, 6992)`; before
  `field_0x300`: `(5943, 2736, 6976)`; before `field_0x200`: `(6027, 2760, 6960)`.
* `field_0x285 = 1` moved next to `field_0x284`: `(5365, 2729, 7064)`;
  `field_0x284` moved next to `field_0x285`: `(5393, 3095, 7048)`.
* all 120 permutations of the 5-statement group
  `{0x28c, 0x238.a, 0x288, 0x286, 0x284}`: identity optimal; a few neutral.
* `field_0x18c` moved after `field_0x180`: `(5718, 2887, 6904)`; after
  `field_0x188`: `(5957, 2846, 7048)`; `(int)(float)` round-trip on its last
  arm: **neutral**.
* `helpWindowX` moved to the end (before `helpWindowY`): `(5455, 2775, 6948)`;
  before the `rewardBoxY2` block: `(5968, 2859, 6952)`.
* late-block reorder to the original's observed store order
  (`scrollBar/0x144/rewardBox*/0x2bc/0x3fc/0x200/0x1f0/0x1f4/0x1f8/0x284/0x285/0x286/0x288/0x28c`):
  `(5960, 2723, 6976)` — aligned improves to 2,723 but raw/size are worse.
* cast canonicalisation: `(int)(float)` → `(int)` is neutral for
  `field_0x150`, `field_0x160`, `field_0x2d0`, `field_0xa8`; it is **worse**
  for `field_0x2c8` `(5423,2727,7064)`, `field_0xa8` `(5382,2728,7064)`,
  `field_0x10c_rowBaseY` `(5408,2705,7064)`.
* `field_0x5c` / `field_0x70` nested `if/else`: `(5796,2905,7000)` /
  `(5857,3373,7056)`.

## 5. Store duplication observed (why the residual is structural)

Several fields are stored **once** in the original but **multiple times** in
ours, because our source order puts them inside branchy regions and clang
duplicates the stores:

| field | original | ours |
|---|---|---|
| `0x2bc` | 1 store (`0xd25f8`) | 5 stores (`0xd1254`, `0xd1e88`, `0xd1eba`, `0xd1eda`, `0xd1f04`) |
| `0x285` | 1 store (`0xd2620`) | 3 stores (`0xd2310`, `0xd23b6`, `0xd2544`) |
| `0x280` | 1 store (`0xd24aa`) | 2 stores (`0xd2314`, `0xd23bc`) |
| `0x28c` | 1 strd with `0x288` (`0xd2648`) | 2 stores (`0xd25ce`, `0xd2680`) |

The original's late region stores `0x284,0x285,0x286` as three consecutive
`strb.w` (`0xd261c/0xd2620/0xd2628`) and `0x288/0x28c` as one `strd`
(`0xd2648`), i.e. those five fields are adjacent in the original source.
Moving them together in our source is worse (§4), so the surrounding schedule
must also change.

## 6. Recommendation

The remaining 5,249 differing bytes are dominated by the non-HD/shared-tail
schedule (only 1,129 of 6,188 code bytes match before `0xd2854`; the
`0xd1928..0xd2828` region is ~4% matched).  A win needs a *structural*
reconstruction of the non-HD path that reproduces (a) the original's inline
constant islands and (b) its predicated (`it eq`) arms, while keeping the
total at 7,072 B.  Single-statement moves, adjacent/distance-2 swaps, K=6
windows, pool-head reorders and if/else-vs-ternary are all exhausted.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13420
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5249 aligned=2725 size=7072/7072 insn=2520/2552
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13420/`: `adj2.py`/`adj2.json`
(all 459 adjacent/distance-2 swaps), `perm5.py`/`perm5.json` (120 permutations
of the 0x284..0x28c group), `exp.py` + `edit_*.py` (the single-edit
experiments), `ours_new.dis` / `orig.dis` (disassemblies).
