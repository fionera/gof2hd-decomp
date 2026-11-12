# `Layout::Layout` C1/C2 — wave 185 / worker w12912

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave185-w12912`, base master `f390b0e68` (post-wave-184 tree; the
wave-183 `w12712` permutation is landed, raw **5,383**).

**Landed: two size-exact raw wins.**

| metric | before | after |
|---|---:|---:|
| raw same-address bytes | **5,383** | **5,361** (-22) |
| aligned (difflib) | 2,560 | 2,609 (+49) |
| size | 7,072/7,072 | **7,072/7,072** |
| instructions | 2,517/2,552 | 2,517/2,552 |

The raw metric is the worker objective; aligned is reported honestly and moved
the other way (the original has 35 more instructions than us, so raw wins tend
to trade aligned structure for same-address byte agreement).

## 1. Winning move

One pure statement move of six independent device-constant stores, no
value/type/control-flow change.  Source order

```
field_0x1a0
field_0x184
field_0x198
field_0x18c
field_0x1a8
field_0x1ac
```

became

```
field_0x1ac
field_0x184
field_0x1a0
field_0x198
field_0x1a8
field_0x18c
```

i.e. a K=6 window permutation `(5,1,0,2,4,3)` over top-level units
118..123 (0-based, statements only).  `git diff` is 3 moved lines.
Raw **5,383 -> 5,364** (-19), size exact, instruction count unchanged.  The
proxy predicted 5,317 -> 5,298 (-19) exactly; the linked delta matched the
proxy one-for-one.

This is a store-scheduling/SLP-grouping effect: moving `field_0x1ac` ahead of
`field_0x184`/`field_0x1a0` changes how clang groups the literal-pool
`vld1.64` pair loads and the `vst1.32` stores, which is the dominant residual
in this constructor.

### 1b. Second winning move (post-win re-sweep)

After landing the K=6 move a fresh W=2/W=3 sweep on the improved order found
one more adjacent swap in the same block: `field_0x19c` and `field_0x1ac`
(units 117/118 of the improved order) exchanged, i.e.

```
field_0x1ac
field_0x19c
field_0x184
field_0x1a0
field_0x198
field_0x1a8
field_0x18c
```

Raw **5,364 -> 5,361** (-3), size exact, instruction count unchanged; proxy
5,298 -> 5,295 (-3), again one-for-one with the linked delta.

## 2. Method

The ctor was re-parsed into **204 top-level units** (statements / complete
if-else chains; the parser round-trips the file byte-for-byte).  Candidates
were scored with a compile-only proxy:

* write a reduced TU (`prefix.h` with the same includes as `Layout.cpp`, then
  only the ctor body) and compile it with the exact `compile_commands.json`
  flags; the reduced TU produces byte-identical `.text._ZN6LayoutC2Ev` to the
  full TU (verified with `cmp`),
* `objcopy -O binary -j .text._ZN6LayoutC2Ev`, mask every 4-byte word that
  carries a relocation (`objdump -r`), and count differing bytes against the
  original function body at `0xd1028` (plus size delta).

Proxy base **5,317** vs linked raw **5,383** (constant +66); the winning
candidate's proxy delta (-19) matched the linked delta (-19) exactly.
Throughput ~26 candidates/s with 4 parallel workers (0.15 s/compile).

Searches (all on the post-wave-184 base order):

* **W=2 exhaustive** (203 adjacent swaps): no improvement.
* **W=3 exhaustive** (809 unique windows x permutations): no improvement.
* **K=4 exhaustive** (3,623 unique): no improvement.
* **K=5 exhaustive** (19,223 unique): no improvement.
* **K=6 exhaustive** (119,519 unique): one win, window 118
  `(5,1,0,2,4,3)` (proxy 5,317 -> 5,298).
* **distance-11..20 single-unit moves** (3,770 unique, both directions):
  no improvement.
* After the K=6 win: **W=2** on the improved order found the
  `field_0x19c`/`field_0x1ac` swap (proxy 5,298 -> 5,295); a **W=3** sweep on
  the improved order found nothing beyond it.

A second W=2/W=3 pass on the landed order was not run (the K=6 sweep had
already covered every 6-window at the base order); the landed order is a
local optimum for the W=2/W=3/K=4/K=5/distance-11..20 neighbourhoods of the
base.

## 3. Residual

Unchanged in kind: the original is 6,108 B of code / 2,552 insns with a
221-word literal pool; ours is 6,288 B / 2,517 insns with a 190-word pool.
The original has more 16-bit `movs`/`it` and SLP-vectorises a different group
set; the residual is instruction selection / register allocation / store
schedule, not a wrong value.  The mid-block store order is already close to
the original's, so local statement permutation has largely run out.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12912
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5361 aligned=2609 size=7072/7072 insn=2517/2552
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w12912/`: `units.py` (204-unit
parser), `proxy.py` (reduced-TU compile-only scorer), `psweep.py` (parallel
W/K/move search), `k6.log`-equivalent output in the wave transcript,
`Layout.base.cpp` (pristine ctor), `prefix.h` (reduced-TU prefix).
