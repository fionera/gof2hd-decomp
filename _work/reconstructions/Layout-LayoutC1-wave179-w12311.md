# `Layout::Layout` C1/C2 — wave 179 / worker w12311

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Baseline: branch `wave179-w12311`, base master `64014ce6f` (post-wave-178 tree):
raw **5,576**, aligned **2,410**, size **7,072/7,072**, 2,519 / 2,552
instructions.  `reload()` is untouched (byte-exact).

**Landed: a size-exact raw win.**

| metric | before | after |
|---|---:|---:|
| raw same-address bytes | **5,576** | **5,569** (-7) |
| aligned (difflib) | 2,410 | 2,434 (+24) |
| size | 7,072/7,072 | **7,072/7,072** |
| instructions | 2,519/2,552 | 2,519/2,552 |

The raw metric is the worker objective; aligned is reported honestly and moved
the other way.  The landed edit is a single statement move: `field_0x2c_rowHeight`
now sits **after** `field_0x38` instead of before `field_0x3c`.

## 1. Winning move

In source order, the constructor's early scalar block was

```
field_0x30_rowHeight
field_0x2c_rowHeight   <- moved
field_0x3c
field_0x38
field_0x34
```

and is now

```
field_0x30_rowHeight
field_0x3c
field_0x38
field_0x2c_rowHeight
field_0x34
```

Pure reordering of existing field assignments: no value, type or control-flow
change, so the store trace is unchanged.  `git diff` is one moved line.

## 2. Method

`Layout::Layout` is a 204-unit straight-line initializer (units = top-level
statements / if-else chains).  The wave-178 pass had already searched windows
`W=2..5` over units 38..228; units 0..37 were never covered.  This pass ran:

* an adjacent-unit scan over units 0..37 (W=2), then
* exhaustive `W=3` and `W=4` window permutations over units 0..37, and
* `W=3` over units 38..80 as a cross-check.

Every candidate was re-linked and measured with `tools/fn_diff.py`; only
size-exact (`7,072/7,072`) raw improvements were kept.  The search is a
*window* permutation search over the head of the ctor, complementary to
w12310's tail permutation search.

The winning `W=3` window was units 12..14
(`field_0x2c_rowHeight`, `field_0x38`, `field_0x3c`) with permutation
`(2,1,0)`, i.e. `0x3c, 0x38, 0x2c`.  Re-running W=2/W=3/W=4 on top of the win
found no further size-exact improvement in units 0..37; the landed order is a
local optimum for those neighbourhoods.

## 3. Region budget and byte-diff map (base)

Boundaries from `objdump -M force-thumb`: prologue `d1028..d1042`; iPadHD
block `d1042..d126c` (orig) / `d1042..d127a` (ours); non-iPadHD path
`d126c..d21e2`; shared tail `d21e2..d2852` (orig) / `d21e2..d28d0` (ours);
literal pool `d2854..d2bc8` (orig, 221 words) / `d28d0..d2bc8` (ours, 190
words).  Both bodies end at `d2bc8`.

Same-address differing bytes at the base (sum = 5,576):

| region | original | ours | differing bytes |
|---|---:|---:|---:|
| first half `d1028..d21e2` | 4,538 B | 4,538 B | **3,640** |
| tail `d21e2..d2852` | 1,648 B | 1,648 B | **1,544** |
| extra code `d2852..d28d0` | – | 126 B | 122 |
| pool `d28d0..d2bc8` | – | 760 B | 270 |

The first half has the same length in both builds but differs byte-for-byte:
clang's register allocation and store schedule diverge early (first differing
byte is `d102e`, the `add r7,sp,#12`), so the raw metric is dominated by the
shifted instruction stream, not by a single bad constant.

## 4. `field_0x90` (the `0.325f` / `0.65f` select)

GOT mapping (relocations in the original): `iPadHD`→`0x21038c`,
`retinaDisplay`→`0x2103e4`, `n9`→`0x2103e8`, `iPad`→`0x21032c`.

Original `d1940..d195c`:

```
orr.w  r2, fp, r3        ; fp = retina, r3 = iPad
orrs.w r1, r2, r8        ; r8 = n9
addw   r1, pc, #3848     ; -> tail pool d2854 (0.325f, 0.65f)
it eq
addeq  r1, #4
cmp.w  fp, #0
vldr   s2, [r1]
vstr   s0, [r0, #140]    ; field_0x8c
vstr   s2, [r0, #144]    ; field_0x90
```

so the original is `(retina|iPad|n9) ? 0.325f : 0.65f` and reads the two
constants from the **tail** pool.  Our nested source
`retina ? 0.325f : (iPad ? 0.325f : (n9 ? 0.325f : 0.65f))` already emits the
same 3-way OR; it differs only in that clang places `0.325f`/`0.65f` in an
**inline** pool at `d1924` and references it with `subw r1, pc, #88`.  The
original's tail pool starts `0.325f, 0.65f, 100.0f, 50.0f`; ours starts
`100.0f, 50.0f, ...` and the two floats live inline.

Explicit-form variants (all real `link_so` + `fn_diff`, C1 alias):

| `field_0x90` form | raw | aligned | size |
|---|---:|---:|---:|
| base nested ternary | **5,576** | 2,410 | 7,072 |
| `(retinaDisplay \|\| iPad \|\| n9) ? 0.325f : 0.65f` | 5,576 | 2,410 | 7,072 |
| `(iPad \|\| retinaDisplay \|\| n9) ? ...` | 5,576 | 2,410 | 7,072 |
| `(n9 \|\| retinaDisplay \|\| iPad) ? ...` | 5,825 | 2,394 | 7,064 |
| `(n9 \|\| iPad \|\| retinaDisplay) ? ...` | 6,054 | 3,873 | 7,052 |
| `(iPad \|\| n9 \|\| retinaDisplay) ? ...` | 6,054 | 3,873 | 7,052 |

Only the retina-first orders are size-exact, and they are byte-identical to
the base.  The earlier wave-177 note's "size 7,052" is the n9-first order, not
an intrinsic property of the explicit form.

## 5. `volatile unsigned char *rd` block

Original `d1870..d18ba` computes `0x2d4`/`0x2d8` branchlessly from one shared
`retina|n9` and merges the two stores into one `strd r4,r6,[r0,#724]`; ours
emits a separate `str` for `0x2d8` plus real branches for `0x2d4` through
`volatile unsigned char *rd`.

Measured shapes (base position):

| `0x2d4` shape | raw | aligned | size |
|---|---:|---:|---:|
| base if/else chain (with `volatile`) | **5,576** | 2,410 | 7,072 |
| flat ternary, `volatile` kept | 5,678 | 2,431 | 7,072 |
| `iPad`-outer if/else | 5,922 | 2,711 | 7,064 |
| nested per-`iPad` if/else | 5,678 | 2,431 | 7,072 |

(`wave177` reported 5,710 for the ternary form; the 5,678 figure is the same
shape with the current base's surrounding order.)  Dropping `volatile` is
size-fatal (7,064).  The base if/else form remains the raw-best choice at its
current position; wave-177's 839-position sweep already showed the base
position is optimal for that form.

## 6. `rewardBoxY2` — a real source bug that is *not* raw-cheap to fix

The source computes `rewardBoxY2 = (rewardBoxY + 17) << retina` **before**
assigning `rewardBoxY`, so it reads the uninitialised member.  The original
assigns `y` first (`d2514..d252c`): `add.w r2,r1,#17; lsl.w r2,r2,r8;
vmov s0,r2; vcvt.f32.s32; vcvt.s32.f32; str y; vstr y2`.  The emulator confirms
the original's `0x3f8` is `186/127/72/55` for retina/iPadLarge/iPad/default
while ours is `34/17/17/17`.

Fixing the order is size-exact in two forms but *worse* on raw:

| y2 form (y assigned first) | raw | aligned | size |
|---|---:|---:|---:|
| base (buggy, y2 before y) | **5,576** | 2,410 | 7,072 |
| `(y + 17) * (retina ? 2 : 1)` | 5,638 | 2,411 | 7,088 |
| `(y + 17) << retina` | 5,659 | 2,409 | 7,080 |
| `(int)(float)((y + 17) << retina)` | 5,671 | 2,474 | 7,096 |
| local `ry`, `(ry+17)<<retina` | 5,902 | 2,778 | 7,072 |

A future pass that also reconstructs the surrounding per-branch store
duplication should fix it; on its own it costs raw.

## 7. Other measured variants (all size-exact unless noted)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | **5,576** | 2,410 | 7,072 |
| **landed: `0x2c` after `0x38`** | **5,569** | 2,434 | 7,072 |
| `0x38` before `0x3c` (wave178 base order) | 5,574 | 2,396 | 7,072 |
| `field_0x1e8` explicit `(retina\|\|iPadHD\|\|n9)` | 5,576 | 2,410 | 7,072 |
| `field_0x2d8` `n9\|\|retina` / bitwise `\|` | 5,576 | 2,410 | 7,072 |
| `0x3c/0x38/0x34` ascending | 6,010 | 2,481 | 7,024 |
| `0x204` moved next to `0x208` | 5,980 | 2,520 | 7,000 |
| `0x234` moved next to `0x238.a` | 5,779 | 2,457 | 7,064 |
| orbit-info fields ascending `0x214..0x230` | 5,658 | 2,500 | 6,988 |
| `0x94`+`0x98` before `0x9c` | 5,923 | 2,439 | 6,968 |
| `0x98` before `0x9c` | 5,868 | 2,433 | 6,992 |
| `0x94` before `0x9c` | 5,877 | 2,660 | 7,032 |
| `0x2bc` after `0x2b8` | 5,883 | 2,714 | 7,032 |
| `0x2bc` at end | 5,993 | 2,411 | 6,996 |
| `0x98` at end | 6,026 | 2,840 | 7,008 |
| reward-box nested if/else (whole block) | 5,886 | 2,568 | 7,072 |
| iOS first-part order (`0xc..0x5c`) | 6,057 | 3,907 | 7,008 |

## 8. Why the residual is hard

The original's code is 6,186 B / 2,552 insns; ours is 6,312 B / 2,519 insns.
The original has 33 more instructions but 126 fewer code bytes (more 16-bit
`movs`/`it`/`moveq`) and a 31-word-larger literal pool (221 vs 190).  The
original also vectorises 7 groups at `0x204..0x278` in the iPadHD tail
(`vst1.32`), where ours vectorises `0x1b0..0x1d0` and shifts the `0x204` group
by one field (`0x208..0x27f`).  Matching the original therefore needs the
non-iPadHD branch duplication and the tail dispatch reconstructed together, not
a local reorder.  The wave-178 tail order is a local optimum for `W=2..5` over
units 38..228; this pass adds a local optimum for `W=2..4` over units 0..37.

## 9. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12311
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5569 aligned=2434 size=7072/7072 insn=2519/2552
cmake --build cmake-build-match --target check -j2
# RATCHET PASS: byte_exact 4507, linked_exact 4507, size-exact 4524/4524,
# data_exact 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w12311/`: `Layout.base.cpp`
(pristine ctor), `Layout.win1.cpp` / `Layout.win2.cpp` (landed ctor),
`units.py` (204-unit parser), `adjswap.py` (W=2 single-line scan),
`adjunit.py` (W=2 unit scan), `w3.py` / `w4.py` (window permutation scans),
`measure.sh`, `emu_orig.py` / `emu_ours.py` / `trace_order.py` (store-trace
emulator), `orig2.dis` / `ours2.dis`.

Gate at exit: byte_exact **4507/4524**, linked_exact **4507/4524**,
size-exact **4524/4524**, stub_zero_size **0**, missing **0**, wrong_type
**0**, extra **0**, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift **0**, parity **5370/5370**, `.rel.plt` byte-identical.
`Layout` C1/C2 raw **5,569**, aligned **2,434**, size **7,072/7,072**,
2,519 instructions.
