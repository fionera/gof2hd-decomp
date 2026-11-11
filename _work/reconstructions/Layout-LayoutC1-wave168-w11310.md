# `Layout::Layout` C1/C2 — wave 168 / worker w11310

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Baseline (master `631c26f0b`): raw **5,658**, aligned **2,280**, 7,072 B.
Exit: unchanged (no landed source change; tree clean).

## What was measured

The original store stream was decoded from objdump (`stores.py`: register →
`r0+off` tracking, `str/strb/strh/strd/vstr/vst1.32` including lane stores)
and expanded to per-field offsets.  The head (before `0xd21e2`) is a
three-way `iPadHD` / `n9` / else dispatch exactly like the source:

| region | original | baseline ours |
|---|---|---|
| A (iPadHD) | `d1042..d1268` 550 B | `d1042..d1276` 564 B |
| C (n9) | `d127e..d1570` 754 B | `d128c..d1568` 732 B |
| B (retina/else) | `d1570..d21e2` 3186 B | `d1568..d22a6` 3390 B |
| shared tail start | `+0x11ba` | `+0x127e` (+196) |

The original A-block store order is
`14,4,24,34,44,54,64,2c8,7c,74,2c0,2d8,ac,8c,a4,c0,b0,ec,294,2a4,108,118,dc,e4,fc,104,2b4,128,140..1d0,138,1e0`
(4-field vector groups at `0x14,0x4,0x24,0x34,0x44,0x54,0x64,0x2c8,0x7c,
0xc0,0xb0,0xec,0x294,0x2a4,0x108,0x118,0x128,0x140..0x1d0`).  The baseline
A block differs only in grouping: it emits `0x38,0x48,0x58,0x68` groups
(one field higher) because `field_0x34` is written after `field_0x38`, and
it sinks `eventBannerScale` (`0x1e0`) early instead of last.

## Key finding: the original tail is *per-field ternaries*, not explicit blocks

The C (n9) branch at `d22b8` stores a subset of the 28 "vector" fields
scalar (`0x204,0x210,0x21c,0x23c,0x264,0x254,0x25c,0x260,0x274`) **and
then re-stores the full seven `vld1/vst1` groups** (`0x204,0x214,0x224,
0x23c,0x24c,0x25c,0x26c`).  That is the signature of clang SLP-vectorising
a per-field ternary cascade, not of an explicit constant block (which
would emit each group once).  So the wave-167 explicit-block reconstruction
is a different source shape from the original; the baseline ternary shape
is already the faithful one.

## Shapes measured (all reverted; none beat 5,658 raw at 7,072 B)

| shape | raw | aligned | size |
|---|---:|---:|---:|
| baseline | **5,658** | **2,280** | **7,072** |
| `0x1e8 = retina\|\|iPadHD ? 1 : (n9?1:0)` | 5,658 | 2,280 | 7,072 |
| `0x1e8 = retina\|\|iPadHD ? 1 : 0` | 5,969 | 2,523 | 7,024 |
| `0x1e8 = !(n9\|\|iPadHD) ? retina : 1` | 5,826 | 2,604 | 7,024 |
| `0x1e8 = retina ? 1 : (iPadHD?1:0)` | 5,979 | 2,784 | 7,080 |
| `0x1e8 = retina ? 1 : (iPadHD?1:(n9?1:(iPadLarge?1:0)))` | 5,877 | 2,577 | 7,052 |
| move `eventBannerScale` before `0x1e4` | 5,913 | 2,709 | 7,008 |
| move `0x204`+`0x238.b` before `0x1e4` | 6,004 | 2,712 | 7,024 |
| move `0x2bc` after `rewardBoxX` | 5,932 | 2,539 | 6,976 |
| move `0x34` before `0x38` | 6,020 | 2,387 | 7,024 |
| full A-order head reorder (target store order) | 6,433 | 2,973 | 6,304 |
| tail reorder to original store order | 5,981 | 2,340 | 6,896 |
| 3-way duplicated tail (explicit A/C/B) | 6,277 | 3,515 | 7,148 |

Every structural reorder *shrinks* the function (clang merges more), so
size-exactness and the original store order pull in opposite directions.
The only shape that reproduces 7,072 B exactly is the baseline.

## Why raw is hard to move

Only the first 6 bytes match at the same address; the whole body is
PC-relative (literal-pool `add rX, pc` / `ldr [pc,#imm]` and branch
offsets), so once any instruction count differs the raw metric is
dominated by address bytes.  Raw 5,658 = 1,414 matching bytes.  Matching
it requires reproducing the *literal-pool layout*, i.e. the exact code
order, not just the same statements.

## Next step

The head is 196 B long and the B branch is 204 B long while C is 22 B
short.  Reproducing the original needs the B-branch source order that
makes clang emit `cmp`+`it eq`+`moveq` pairs in the same sequence as
`d1596..d21e0` (ours has +48 `it`, +51 `cmp.w`, +100 `str.w` aligned
extras).  A full statement-level rewrite of the B branch against the
decoded store stream is the remaining work; the A/C blocks are already
structurally right.
