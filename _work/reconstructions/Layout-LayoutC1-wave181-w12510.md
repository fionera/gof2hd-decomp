# `Layout::Layout` C1/C2 — wave 181 / worker w12510

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave181-w12510`, base master `1e270aa0e` (post-wave-180 tree; the
wave-180 `w12410` permutation is landed as `5d9a14d6a`, raw **5,408**).

**Landed: a size-exact raw win.**

| metric | before | after |
|---|---:|---:|
| raw same-address bytes | **5,408** | **5,393** (-15) |
| aligned (difflib) | 2,438 | 2,520 (+82) |
| size | 7,072/7,072 | **7,072/7,072** |
| instructions | 2,514/2,552 | 2,518/2,552 |

The raw metric is the worker objective; aligned is reported honestly and moved
the other way (the original has 33 more instructions than us, so raw wins tend
to trade aligned structure for same-address byte agreement).

## 1. Winning moves

All edits are pure statement moves of existing independent device-constant
stores; no value, type or control-flow change.  In source order:

1. `field_0xc_leftMargin` moved **after** `headerTitleY` (units 3/4).
2. `field_0x208` moved **before** `orbitInfoSecurityY` / `quickMenuIconGap`
   (units 160/161).
3. `field_0x248` moved **after** `field_0x25c`, and `field_0x258` moved
   **after** `field_0x25c` (units 173/174, a 3-window rotation).
4. `footerButtonOffset` moved **before** the `field_0x280` if/else chain and
   `field_0x285` moved **before** that chain (units 171/172, 3-window).

`git diff` is 6 moved lines (12 changed lines).

## 2. Method

The wave-180 `w12410` permutation re-shaped the whole constructor, so the
wave-178/179 local optima no longer apply.  This pass re-parsed the ctor into
**204 top-level units** (statements / complete if-else chains; round-trip
exactly reproduces the file) and ran a fresh greedy window search:

* `W=2`: exhaustive adjacent-unit swaps over all 204 units, repeated to a
  fixed point (2 rounds).  Wins: `3,4` (5408->5406), `160,161`
  (5406->5405), `173,174` (5405->5404).
* `W=3`: exhaustive 5-permutation scan over all 202 windows, repeated to a
  fixed point (2 rounds).  Wins: window 141 `(2,0,1)` (5404->5399), window
  161 `(2,1,0)` (5399->5395), window 171 `(2,1,0)` (5395->5393).

Every candidate was a real `link_so` + `tools/fn_diff.py` measurement; only
size-exact (`7,072/7,072`) raw improvements were kept.  Windows that separate
the two local-declaration/use pairs (`volatile unsigned char *rd` from its
if/else, `int v` from `field_0xa8`) fail to compile and are skipped.  A second
`W=2` and `W=3` pass on the landed order found no further size-exact win, so
the landed order is a local optimum for both neighbourhoods.

## 3. Residual

The base body is 5,408 raw-differing bytes; the original is 6,186 B / 2,552
insns and ours 6,312 B / 2,518 insns.  The original has 34 more instructions,
more 16-bit `movs`/`it`, a 31-word-larger literal pool (221 vs 190 words) and
SLP-vectorises 7 groups in the iPadHD tail, while ours vectorises a different
group set.  The residual is instruction selection / register allocation and
store schedule, not a wrong value: the wave-100 store-trace work
(`layout-layout-c1-c2-store-order-w5001.md`) showed all 807 stored bytes agree
in all six flag configurations and the HD/n9 store *order* is already ~82 %
identical.  The mid block (non-HD arm of the first 137 fields plus the
per-flag chains for `0x7c..0x1dc`) is the largest loss and is already in the
original store order, so local statement permutation has largely run out.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12510
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5393 aligned=2520 size=7072/7072 insn=2518/2552
cmake --build cmake-build-match --target check -j2
# RATCHET PASS: byte_exact 4507, linked_exact 4507, size-exact 4524/4524,
# data_exact 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w12510/`: `units.py` (204-unit
parser), `sweep.py` (`w2`/`w3` greedy window search), `Layout.base.cpp`
(pristine post-wave-180 ctor), `best_all.json` (landed unit order),
`w2_run.txt` / `w3_run.txt` (per-candidate logs).

Gate at exit: byte_exact **4507/4524**, linked_exact **4507/4524**,
size-exact **4524/4524**, stub_zero_size **0**, missing **0**, wrong_type
**0**, extra **0**, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift **0**, parity **5370/5370**, `.rel.plt` byte-identical.
`Layout` C1/C2 raw **5,393**, aligned **2,520**, size **7,072/7,072**.
