# `Layout::Layout` C1/C2 — wave 186 / worker w13012

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave186-w13012`, base master `2d02567f6` (wave-185 tree; the wave-185
`w12912` permutation is landed, raw **5,358**).

**Landed: two size-exact raw wins (5,358 -> 5,354).**

| metric | before | after win 1 | after win 2 |
|---|---:|---:|---:|
| raw same-address bytes | **5,358** | **5,355** (-3) | **5,354** (-1) |
| aligned (difflib) | 2,626 | 2,625 | 2,680 |
| size | 7,072/7,072 | **7,072/7,072** | **7,072/7,072** |
| instructions | 2,517/2,552 | 2,517/2,552 | 2,521/2,552 |

Gate counts are unchanged (the function is still not byte-exact): byte_exact
4507/4524, linked_exact 4507/4524, size-exact 4524/4524, data_exact 843/843,
imports 207, lint CLEAN, drift 0, parity 5370/5370.

## 1. Winning move

One pure statement permutation of a six-unit window, no value/type/control-flow
change.  Source order of top-level units 85..90 (0-based, statements only)

```
field_0x12c
field_0x11c
field_0x124
field_0x10c_rowBaseY
field_0x114_strip5c
field_0x110_strip58
```

became

```
field_0x10c_rowBaseY
field_0x114_strip5c
field_0x11c
field_0x110_strip58
field_0x124
field_0x12c
```

i.e. a K=6 window permutation `(3,4,1,5,2,0)`.  `git diff` is 3 moved lines.
Raw **5,358 -> 5,355** (-3), size exact, instruction count unchanged.  The
compile-only proxy predicted 5,292 -> 5,289 (-3) exactly; the linked delta
matched the proxy one-for-one.

This is the same store-scheduling / SLP-grouping mechanism as the wave-185
wins: the six stores are independent, and the reorder changes which literal-pool
constants clang pairs into `vld1.64` loads and which stores it merges.

### 1b. Second winning move (composition test on the improved order)

After landing 1, the two runner-up K=6 candidates from the base sweep were
re-tested against the improved order.  Window 112 `(2,0,1,3,5,4)` now scores
proxy **5,289 -> 5,288** (-1), size exact, and composes with win 1.  Source
order of units 112..117

```
field_0x188
field_0x194
field_0x1a4
field_0x180
if (iPadHD) field_0x190 ... else ...
field_0x1ac
```

became

```
field_0x1a4
field_0x188
field_0x194
field_0x180
field_0x1ac
if (iPadHD) field_0x190 ... else ...
```

Raw **5,355 -> 5,354** (-1), size exact.  Instruction count moved
2,517 -> 2,521 (same byte size; shorter encodings compensate).  The aligned
metric moved 2,625 -> 2,680; raw is the worker objective.

## 2. Search performed (fresh, on the wave-185 landed order)

The ctor was re-parsed into **204 top-level units** (statements / complete
if-else chains; the parser round-trips the file byte-for-byte).  Candidates
were scored with a compile-only proxy:

* `prefix.h` = the exact include prefix of `Layout.cpp`, precompiled once with
  the exact `compile_commands.json` flags (`-include-pch`);
* candidate TU = `#include "prefix.h"` + only the ctor body;
* the reduced TU produces byte-identical `.text._ZN6LayoutC2Ev` to the full TU
  (verified with `cmp` against a full-TU compile);
* `objcopy -O binary -j .text._ZN6LayoutC2Ev`, mask the 21 relocation words
  (`objdump -r`), count differing bytes against the original function body at
  `0xd1028` (plus size delta).

Proxy base **5,292** vs linked raw **5,358** (constant +66); the winning
candidate's proxy delta (-3) matched the linked delta (-3) exactly.
Throughput ~36 candidates/s with 4 parallel workers (~0.11 s/compile).

Searches (all on the post-wave-185 base order):

* **W=2 exhaustive** (203 adjacent swaps): no improvement.
* **W=3 exhaustive** (1,010 unique windows/perms): no improvement.
* **K=4 exhaustive** (4,623 unique): no improvement.
* **2-opt exhaustive** (20,706 arbitrary single swaps): no improvement.
* **3-opt, span 2..10** (44,250 unique triples x 5 orders): no improvement.
* **distance-11..20 single-unit moves** (3,770 unique, both directions): no
  improvement.
* **K=5 exhaustive** (23,800 unique): no improvement.
* **K=6 exhaustive** (143,081 unique): **one win**, window 85
  `(3,4,1,5,2,0)` (proxy 5,292 -> 5,289).  Two other windows produced
  raw 5,291 (window 87 and window 112) but not better than 5,289.

The landed order is a local optimum for the W=2/W=3/K=4/K=5/2-opt/3-opt(span<=10)/
distance-11..20 neighbourhoods of the wave-185 order.

## 3. Residual

Unchanged in kind: the original is 6,188 B of code / 2,552 insns with a
221-word literal pool; ours is 6,320 B / 2,517 insns with a 190-word pool.
The original pairs more stores (`strd` 62 vs our 41, `vst1.32` 81 vs our 71,
individual `str` 121 vs our 220); the residual is instruction selection /
register allocation / store pairing, not a wrong value.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13012
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5355 aligned=2625 size=7072/7072 insn=2517/2552
```

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13012/`
(`units.py` 204-unit parser with span-preserving rewriter, `proxy.py`
reduced-TU scorer, `sweep.py` worker, `search.py` W/K/2-opt/3-opt/move driver,
`apply2.py` separator-preserving permutation applier, `search_*.log`).
