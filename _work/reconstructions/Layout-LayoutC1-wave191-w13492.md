# `Layout::Layout` C1/C2 — wave 191 / worker w13492

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, linked symbol size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave191-w13492`, base master `f325e2d87`.

**Result: no size-exact raw win landed.** Baseline unchanged at raw **5,243**,
aligned **2,728**, size **7,072/7,072**, insn 2,521/2,552. Everything measured
below is either neutral (raw 5,240 at object level) or size-fatal. Tree left
clean.

## New evidence: the HD/n9 arm is fully vectorised in the original

The original's `iPadHD` arm (`0xd1042`–`0xd1268`) is not a scalar store chain.
It is a sequence of **27 `vld1.64`/`vst1.32` 4-word constant groups** decoded
from the literal pools:

```
0x14  30 12 28 7        0x4   21 42 40 52      0x24  7 7 4 42
0x34  8 9 82 140        0x44  7 15 7 125       0x54  487 341 63 90
0x64  112 566 191 63    0x2c8 63 126 63 112    0x7c  12 14 112 42
0xb0  4 20 25 11        0xc0  9 11 5 253       0xec  56 112 281 562
0x294 281 118 16 16     0x2a4 154 140 147 42   0x108 -2 1 2 1
0x118 9 35 21 1         0x128 1 21 78 151      0x140 15 18 2 189
0x150 5 70 70 206       0x160 68 0 35 184      0x170 174 84 84 217
0x180 23 163 23 244     0x190 -6 8 16 90       0x1a0 18 22 22 243
0x1b0 18 42 46 1        0x1c0 92 26 40 50      0x1d0 7 17 5 14
```

The n9 arm (`0xd127e`) is vectorised the same way (7 groups at
`0x14/0x4/0x24/0x34/0x44/0x54/0x64` plus `stmia 0x2c0`, `strd 0x74`).
The original's HD arm is therefore **shorter** than ours (its literal pool
starts at `0xd1300`; ours at `0xd1310`), and its code/pool split is
`0xd2854` code + 884 B pool vs ours `0xd28b6` + 776 B.

### The 0x20..0x78 block

Original groups are aligned on `0x34/0x44/0x54/0x64`; ours on
`0x38/0x48/0x58/0x68`, with `field_0x34` and `field_0x78` stored separately.
Sorting the 23-statement `0x20..0x78` block by offset makes our HD/n9 groups
**exactly** match the original's nine leading groups, but the function shrinks
to **7,032 B** (-40). A 400-permutation random search over the block found no
permutation that both keeps size 7,072 and reproduces the original groups, so
the missing 40 B is outside the block.

`sorted block + 0x130 all-ternary (mode A) + 0x2cc if/else` restores size
7,072 and matches the nine HD groups, but the *retina/default* arm
(`0xd1570`–`0xd21e2`) regresses: linked raw **5,792** (aligned 2,647, i.e.
-81 aligned but +549 raw). Not landed.

## Cold-block layout

The original outlines exactly four cold blocks after the epilogue
(`0xd2802`–`0xd2852`):

| addr | arm |
|---|---|
| `0xd2802` | `field_0x130/0x134` default (`iPad ? 56:40`, `iPad ? 108:80`) |
| `0xd2828` | `field_0x2c8` default (`iPad ? 45:30`) |
| `0xd2836` | `field_0x2cc` default (`iPad ? 90:60`) |
| `0xd2844` | `field_0x60` default (`iPad ? 64:44`) |

Ours outlines only two (`0x18c` default `retina ? 348:174` and `0x60`
default). Splitting the `0x130` final `else` into `else if (iPad) … else …`
makes the compiler outline it (size 7,064, -8) but does not restore size.
Converting `0x2c8` to an explicit if/else chain is byte-neutral; `0x2cc`
if/else is +20 (size-fatal).

## Other measurements (all size-exact unless noted)

* swapping `!iPadHD ? else : then` on all 171 `Globals::iPadHD ?` lines:
  every size-7,072 result is raw 5,240 (neutral);
* swapping the `n9`/`retina` condition order on all simple
  `n9 ? A : (retina ? B : C)` lines: 8 size-7,072 results, all raw 5,240;
* converting every `N * 1.40625f` HD arm to its truncated integer literal:
  all individually neutral (5,240) except a few size-fatal;
* `field_0x314` equal-arm fold `(n9 || retina) ? -8 : -5`: size 7,072,
  raw 5,251 (worse);
* `field_0x2d4` decompile shape `retina ? 160 : ((n9||iPad) ? 100 : 60)`:
  size 7,064 (fatal);
* `field_0x2d4` as a single ternary (keeping `*rd`): size 7,072, raw 5,395;
* reordering `0x8c..0xac` to the original's `stmia 0x94` grouping: 7,032;
* moving `ringNearMidThreshold` next to `field_0xf0/ringMidFarThreshold/
  field_0xf8` (the original's `0xec` group): 7,068 (fatal);
* `bool hd/n9/ret` locals, `Globals::retinaDisplay` → `(*rd)` everywhere,
  full-statement sort by offset: all size-fatal.

## Conclusion

The remaining delta is dominated by the retina/default arm, whose register
allocation and cond-move chains differ throughout (~2,000 B). Matching the
HD/n9 vector groups alone is a net raw loss because it perturbs that arm.
A win needs the original's actual ctor statement order, which the vector
groups only partially constrain.
