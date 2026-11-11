# `Layout::Layout` C1/C2 — wave 183 / worker w12712

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave183-w12712`, base master `9555474d8` (post-wave-182 tree: the
w12612 `orbitInfoSystemY` move is landed, raw **5,385**).

**Landed: size-exact raw wins** (one commit per win; see the table).

| metric | before | after |
|---|---:|---:|
| raw same-address bytes | **5,385** | **5,383** (-2) |
| aligned (difflib) | 2,562 | 2,560 |
| size | 7,072/7,072 | **7,072/7,072** |
| instructions | 2,518/2,552 | 2,517/2,552 |

## 1. Winning move

One W=4 window rotation in the middle of the constructor, no value/type/
control-flow change.  Source order

```
field_0x18c
field_0x184
field_0x1a0
field_0x198
```

became

```
field_0x1a0
field_0x184
field_0x198
field_0x18c
```

i.e. window 142 permutation `(2,1,3,0)`.  Raw **5,385 -> 5,383** (-2), size
exact, one instruction fewer (2,517 vs 2,518).

## 2. Method

The ctor was re-parsed into **228 top-level units** (statements / complete
if-else chains; the parser round-trips the file byte-for-byte).  Every
candidate was scored with a **compile-only proxy** and only proxy winners were
re-linked and measured with `tools/fn_diff.py`:

* compile the candidate TU with the exact `compile_commands.json` flags,
* `objcopy -O binary -j .text._ZN6LayoutC2Ev`, mask every 4-byte word that has
  a relocation (`objdump -r`; 21 `R_ARM_GOT_PREL` Globals loads),
* count differing bytes against the original function body at `0xd1028`
  (plus size delta).

The proxy is exact for ranking: base proxy **5,319** vs linked raw **5,385**
(constant +66); on 20 sampled W=3 candidates the proxy tracked the linked raw
one-for-one (offset 66..76, identical ordering, identical size).  Proxy
throughput is 0.30 s/candidate vs 0.87 s for a full `link_so`, and four
parallel workers give ~15 candidates/s.

Searches (all on the post-wave-182 base order):

* **W=2 exhaustive** (227 adjacent swaps): no improvement.
* **W=3 exhaustive** (226 windows x 5 permutations = 1,130): no improvement.
* **W=4 exhaustive** (225 windows x 23 permutations = 5,175): one win,
  window 142 `(2,1,3,0)` (5,319 -> 5,317 proxy); a second full W=4 pass on
  the landed order found no further win.
* **distance-4..10 single-unit moves**: see below.

## 3. Residual

Unchanged in kind: the original is 6,186 B / 2,552 insns, ours 6,312 B /
2,517 insns.  The original has more 16-bit `movs`/`it`, a larger literal pool
and SLP-vectorises a different group set; the residual is instruction
selection / register allocation / store schedule, not a wrong value.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12712
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5383 aligned=2560 size=7072/7072 insn=2517/2552
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w12712/`: `units.py` (228-unit
parser), `proxy.py` (compile-only scorer), `psweep.py` (parallel W/move
search), `p_w4.txt` (per-candidate log), `pfinal_w4.json` (landed order).
