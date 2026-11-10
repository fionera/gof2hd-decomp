# `Layout::Layout` C1/C2 — wave 167 / worker w11209

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Baseline (master `d00057e6d`, after w11108): raw **5,658**, aligned **2,280**,
7,072 B.  Exit: unchanged (no landed source change).

## What was decoded

The original tail `0xd21e2..0xd2800` was decoded store-by-store from objdump
(registers at the tail entry: `r0=this`, `r8=retinaDisplay`, `r9=!n9`,
`sl=iPadHD`, `fp=n9`, `ip=n9`; the wave-166 note's `ip=retinaDisplay` is
wrong — `ip` is only used at `d2614` as `!ip` stored to `0x284/0x286`,
which are `n9?0:1`).

Block order is **iPadHD (A) -> else/retina (B) -> n9 (C)**, with three
shared tails:

1. `d2498` shared by B and C: `0x280`, `scrollBarInset`, `scrollBarHandle`,
   `rewardBoxWidth`, `rewardBoxY`, `rewardBoxY2` (values computed in each
   branch, stores sunk into one block).
2. `d24de` (n9: `rewardBoxHeight=352, rewardBoxX=75`) / `d2530`
   (else: `retina?392:196`, `retina?100:50`); block A **branches into**
   these (tail merging).
3. `d2548..d2800` shared by all: `0x2bc`, `footerButtonOffset`, `0x200`,
   `0x1f0`, `0x1f4`, `0x1f8`, `0x284`, `0x285`, `0x286`, `0x288`, `0x28c`,
   `0x290`, `0x2f4`, `0x2f8`, `0x2fc`, `0x300`, `0x304`, `0x308`,
   `0x30c`, `0x310`, `0x314`, `0x318`, `helpWindowX`, `helpWindowY`.

Block A and C each store the same 28 fields as seven `vld1.64`/`vst1.32`
4-word pairs at `0x204,0x214,0x224,0x23c,0x24c,0x25c,0x26c`; block B is the
retina/else per-field cascade, auto-vectorised by clang with `vbsl` from
literal pools `0xd2b70..0xd2bac`.  Values were verified against the pools:
A = `[12,5,7,5][1,7,8,7][28,49,2,1][21,7,4,239][-2,1,5,2][22,1,421,11]
[92,98,136,421]`, C = `[16,7,7,6][-2,7,10,7][30,52,4,2][27,9,5,306]
[-1,2,7,4][28,0,450,12][99,105,145,450]` — all match the per-field
ternary constants in the current source.

## Explicit-block reconstruction (measured, not landed)

Rewrote lines 208-317 as `if (iPadHD) {A} else if (n9) {C} else {B}` with
explicit constant assignments for A/C, per-field ternaries for B, and the
three shared tails above (`rewardBoxHeight/X` as shared ternaries or as a
separate `if (n9) ... else ...`).

| shape | raw | aligned | size |
|---|---:|---:|---:|
| baseline (per-field ternaries) | **5,658** | **2,280** | **7,072** |
| explicit blocks + shared ternaries | 5,863 | 2,652 | 7,008 |
| explicit blocks + shared `if (n9)` tail | 6,065 | 2,552 | 6,952 |
| explicit blocks, branch order `!n9` first | 6,069 | 2,634 | 6,936 |
| explicit blocks + duplicated rewardBoxHeight/X | 5,926 | 2,734 | 6,976 |
| explicit blocks + `0x2bc` moved into tail | 6,047 | 2,711 | 6,960 |
| baseline + `0x2bc` moved after `rewardBoxX` | 5,932 | 2,539 | 6,976 |
| baseline + `0x1e8 = !(n9\|\|iPadHD) ? retina : 1` | 5,826 | 2,604 | 7,024 |

Key measurement: the explicit-block tail is **1,560 B** vs the original's
**1,558 B** (structurally correct), but it starts at relative `+0x1280`
instead of `+0x11ba`: the current head is **198 B longer** than the
original's head.  The head difference is a source-order problem, not a tail
problem: the original sets `0x204` and `0x238_blk16.b` inside its device
blocks (lines 150/121 in our source set them in the head), the original
sets `0x2bc` in the shared tail (our line 153 sets it in the head), and the
original groups `0x1b0..0x1dc` into contiguous vector stores while our
source scatters them.  Until that head order is fixed the tail cannot align
at the same addresses, and the raw metric (same-address bytes) prefers the
per-field-ternary shape.

## Conclusion

The wave-166 finding stands: the per-field-ternary source is a strong local
optimum for the raw metric.  The faithful multi-way dispatch is real and
its tail length matches, but landing it requires first reordering the
`0x1b0..0x1e0` head region (and moving `0x204`/`0x238.b`/`0x2bc` between
head and tail) so the tail starts at `+0x11ba`.  No source change landed;
tree left at baseline (raw 5,658 / aligned 2,280 / 7,072 B).

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11209
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
```
Scratch `/dev/shm/w11209/`: `Layout.base.cpp`, `tail_new.txt`, `gen.py`,
`orig_tail.asm`, `orig_full.asm`, `ours_full.asm`, `stores.py`.
