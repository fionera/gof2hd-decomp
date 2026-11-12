# `Layout::Layout` C1/C2 — wave 191 / worker w13509

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, linked symbol size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave191b-w13509`, base master `554e18a35`.  Fourth independent lens,
continued from `Layout-LayoutC1-wave191-w13504.md`.

**Result: no size-exact raw win found.  `src/` left at the base (raw 5,243,
aligned 2,728, size 7,072/7,072, insn 2,521/2,552).**  Tree clean; gate re-run
green (`RATCHET PASS`, byte_exact 4507/4524, linked_exact 4507/4524,
size-exact 4524/4524, data 843/843, imports 207, lint CLEAN, parity 5370/5370).

This note records one **new** cell that the previous wave-190/191 notes did not
measure — the **SLP grouping phase of the early `0x4..0x78` store block** — and
corrects a factual error in the w13504 handoff.  Read
`Layout-LayoutC1-wave190-w13420.md`, `...-w13443.md`, `...-w13458.md`,
`...-w13467.md`, `...-w13473.md`, `...-w13490.md` and `...-w13504.md` first.

## 1. Correction: the original *does* SLP-vectorise the early block

The w13504 handoff states "the original does not SLP-vectorise the early
`0x4..0x78` block (it emits `movs`/`strd`/`str`), whereas ours SLP-vectorises
`0x4..0x78` with `vld1.64/vst1.32`".  That is wrong.  Both bodies vectorise the
early block; disassembling `0xd10a4..0xd1112` of the original shows nine
`vst1.32 {d2-d3}` stores, exactly like ours.

What actually differs is the **16-byte bundle phase** of the middle groups.
The first nine `vst1` store offsets (HD path, in emission order) are:

| # | original | ours |
|---|---|---|
| 1 | `0x14` | `0x14` |
| 2 | `0x4`  | `0x4`  |
| 3 | `0x24` | `0x38` |
| 4 | `0x34` | `0x24` |
| 5 | `0x44` | `0x58` |
| 6 | `0x54` | `0x48` |
| 7 | `0x64` | `0x68` |
| 8 | `0x2c8`| `0x2c8`|
| 9 | `0x7c` | `0x7c` |

So the original's 4-wide bundles are `0x04/0x14/0x24/0x34/0x44/0x54/0x64`
(i.e. every group starts at `4 mod 16`), while ours are
`0x04/0x14/0x24/0x38/0x48/0x58/0x68` (the middle four start at `8 mod 16`).
The original groups `0x34..0x40, 0x44..0x50, 0x54..0x60, 0x64..0x70` and
leaves `0x74/0x78` as an adjacent pair (emitted `strd r6,r5,[r0,#116]`), whereas
ours groups `0x38..0x44, 0x48..0x54, 0x58..0x64, 0x68..0x74` and leaves `0x34`
and `0x78` as singles.  The `0x34` store is the tell: the original's
`t_field_0x34` joins the 16-byte bundle, ours is the one store that falls
outside it.

This is an **IR-level SLP grouping decision**, not a backend one: at `-Oz` the
IR already contains `<4 x i32>` stores (`%680` at `0x4`, `%682` at `0x14`,
`%684` at `0x38`, `%685` at `0x24`, `%690` at `0x58`, `%692` at `0x48`,
`%693` at `0x68`, ...), and the ARM backend merely splits each `<4 x i32>`
into one `vld1.64 {d0-d1}` + one `vst1.32 {d0-d1}`.  `-fno-slp-vectorize`
removes all `<4 x i32>` stores, confirming the source of the grouping.

The grouping is a function of the **scalar store program order**, which is the
source assignment order.  The original's `0x04/0x14/0x24/0x34/...` phase is
exactly what a purely **ascending-offset** assignment order produces, so the
original source very likely assigned these fields in offset order (or an order
equivalent under the SLP chain scan), while our reconstructed source interleaves
them (`0x40,0x44,0x30,0x3c,0x38,0x2c,0x4c,0x58,0x50,0x78,0x34,0x48,...`).

## 2. Literal-pool head (re-verified)

The pool follows the emission order of the vector constants, so the grouping
phase also shows up at the pool head:

| | first 12 words of the pool |
|---|---|
| original (`0xd2854`) | `0.325, 0.65, 100.0, 50.0, GOT, GOT, GOT, GOT, 281.25, 400.0, GOT, 200.0` |
| ours (`0xd28d8`) | `64.0, 32.0, GOT, GOT, GOT, 281.25, 400.0, GOT, 200.0, 150.0, 11, 10` |

The original materialises `field_0x90`/`field_0x9c` (`0.325/0.65/100.0/50.0`)
before `field_0x8c` (`64.0/32.0`); ours does the reverse.  Combined with the
grouping phase this means the residual is not a few isolated stores but a
whole-function ordering difference, which is why the raw count is 5,243 of
7,072 bytes.

## 3. Measured negatives (all size-exact unless stated)

Baseline proxy (compile-only, `.text._ZN6LayoutC2Ev` in
`Layout.cpp.obj`) is **5,240 B**; the linked function is **5,243 B** (3-byte
link transient).  A win must be `< 5,240` in the proxy and confirmed with the
real link.  Every variant below was scored with the proxy; the promising ones
were re-linked.

### 3.1 Exhaustive single-statement moves inside the early block

All `27 x 26 = 702` single-statement moves of the `field_0xc_leftMargin` ..
`field_0x74` block (`scratch/w13509/search1.py`) were compiled and scored.
**No move is a size-exact raw win.**  The best size-exact results are the
identity (5,240) and the two neutral `0x34`/`0x48` swaps (5,242); every other
size-exact move is >= 5,251.  This subsumes the w13504 adjacent/distance-2 swap
sweep and extends it to all distances.

### 3.2 Ascending-offset reorder of the early block

Sorting the whole `field_0xc` .. `field_0x74` block into ascending offset order
(which is what produces the original's grouping) is **size-fatal**:
`raw 5,842 / size 7,024` (48 B short).  The baseline interleaved order is a
delicate 7,072 B balance; the grouping cannot be fixed by simply sorting.

### 3.3 Targeted "move the phase fields" search

Moving the four phase fields `field_0x34/0x44/0x54/0x64` to each of 6/5/5/5
candidate anchor positions (750 combinations, `scratch/w13509/combo.py`)
produced **zero size-7,072 variants**.  Individual results:

| variant | raw | size |
|---|---:|---:|
| baseline | 5,240 | 7,072 |
| `0x34` before `0x40` | 5,971 | 7,008 |
| `0x54` before `0x58` | 5,801 | 6,968 |
| `0x44` before `0x48` | 6,089 | 6,984 |
| `0x34` before `0x40` + `0x54` before `0x58` | 5,985 | 6,936 |
| `0x44` before `0x48` + `0x34` before `0x38` | 5,757 | 7,080 |

Moving `0x34` to the top of the block flips the middle bundle to start at
`0x30` (`0x30..0x3c`), not `0x34`, and shrinks the body to 7,000-7,008 B.  The
SLP chain scan is not address-sorted: it fills a 4-wide bundle from the earliest
program-order seed and stops at four, so a single moved store changes which
neighbour is excluded and the body size with it.

### 3.4 Re-verified cold tails / callee-saved rotation (unchanged from w13504)

* Original cold blocks `0xd2802` (38 B, `0x130` default), `0xd2828` (14 B,
  `0x2c8`), `0xd2836` (14 B, `0x2d0`), `0xd2844` (14 B, `0x60`), then
  `0xd2852 nop` = 80 B / 4 blocks + pad.  Ours: `0xd28b8` (20 B, `0x18c`
  retina) and `0xd28ca` (14 B, `0x60`, byte-identical to the original's block
  D) = 30 B / 2 blocks.
* Original prologue `push {r4,r5,r6,r7,lr}` + `stmdb sp!,{r6,r7,r8,r9,sl,fp}`
  (frame 44); HD in `sl` (`ldrb.w sl,[r1]` `0xd1036`), n9 in `fp`, retina in
  `r8` (`ldr.w r8,[r2]` `0xd158c`), non-HD spill `strd fp,sl,[sp]` `0xd1570`,
  reloads `ldrd fp,sl,[sp]` at `0xd16a4/d16d2/d1710`.
* Ours prologue `push {r4,r5,r6,r7,lr}` + `stmdb sp!,{r7,r8,r9,sl,fp}`
  (frame 40); HD in `lr` (`ldrb.w lr,[r1]` `0xd1036`), retina in `ip`
  (`ldr.w ip,[r1]` `0xd1586`), non-HD spill `str.w lr,[sp]` `0xd1568`,
  reloads `ldr.w lr,[sp]` at `0xd16a4/d16d2/d1710`.  Ours also loads `n9`
  twice (`ldrb.w fp` at `0xd126e` and `0xd1280`); the original loads it once
  (`0xd1272`).

## 4. Conclusion

The early-block SLP grouping phase is a real, previously unmeasured residue
(4 bundle offsets + the pool head), but it is not reachable by a size-exact
source edit on the wave-191 base:

* the original's grouping requires an ascending-ish scalar order, and the
  ascending reorder is 48 B short (7,024);
* every single-statement move that keeps 7,072 leaves the grouping at the
  `0x38` phase, and every move that flips the phase changes the body size;
* the 750-way targeted phase-field search has no size-7,072 member.

A raw win still needs the full structural reconstruction: the original's
non-HD store order with `strd 0x2d4/0x2d8` (≈ -192 B), the four outlined
`iPad` tails (+66 B), the `0x34`-phase SLP bundles, and the `0x2c8`/`0x2d0`
separate `vcvt` sequences, all landing on exactly 7,072 B.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13509
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5243 aligned=2728 size=7072/7072 insn=2521/2552
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13509/`:
`fast.py` (compile-only proxy, baseline 5,240 / 7,072), `measure.sh`
(real incremental link + `fn_diff`), `group.py` (extract the emitted `vst1`
offset sequence), `search1.py` (702 single moves), `combo.py` (750 phase-field
combinations), `asc.py` (ascending reorder), `mv1.py`/`mv34.py` (targeted
moves), `Layout.base.cpp`, `Layout.ll` (`-emit-llvm -S -Oz` IR),
`orig.dis`/`ours.dis` (objdump).
