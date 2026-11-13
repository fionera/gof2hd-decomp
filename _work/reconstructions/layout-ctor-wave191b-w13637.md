# `Layout::Layout` C1/C2 — wave 191b / worker w13637

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, linked symbol size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave191b-w13637`, base master `57ce1a4c8` (w13607 final: raw 5,235 /
aligned 2,668).

**Result: one size-exact aligned win landed (raw +9, aligned −93).**

| | before | after |
|---|---:|---:|
| raw | 5,235 | 5,244 |
| aligned | 2,668 | **2,575** |
| size | 7,072/7,072 | 7,072/7,072 |
| insn | 2,517/2,552 | 2,512/2,552 |
| `.text` diff | 71,197 | 71,206 |
| `.ARM.extab` diff | 577 | 577 |
| `.ARM.exidx` / `.rodata` | 0 / 0 | 0 / 0 |

`check` = RATCHET PASS: byte_exact 4508, linked_exact 4508, size-exact 4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
`.rel.plt` byte-identical.  Semantics verified with `valall.py` (store image
identical for all 32 `iPadHD/n9/retinaDisplay/iPad/iPadLarge` combinations).

## The win

Three independent-field statement swaps in the constructor body (each a
semantic no-op because every target is a distinct, side-effect-free store):

```
field_0x3c        moved after field_0x2c_rowHeight   (source order 2c, 38, 3c)
field_0x50      <-> field_0x74
field_0x248     <-> field_0x278
```

The first swap is the `0x3c`/`0x2c` pair found by the adjacent-swap sweep;
the other two are non-adjacent.  The `0x248`/`0x278` swap alone is worth
raw +5 / aligned −61; adding the `0x50`/`0x74` swap costs no raw bytes and
another −3 aligned; adding the `0x3c`/`0x2c` swap costs +4 raw and −29
aligned.  The combination is the best Pareto point found (see table).

## Measured candidates (all real `link_so` builds, size-exact unless noted)

Pareto frontier on the w13607 base (raw / aligned / size):

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base (w13607) | 5,235 | 2,668 | 7,072 |
| **landed: 3c/2c + 50/74 + 248/278** | **5,244** | **2,575** | **7,072** |
| 248/278 only | 5,240 | 2,607 | 7,072 |
| 3c/2c + 248/278 | 5,244 | 2,578 | 7,072 |
| 40/2c + 248/278 | 5,241 | 2,596 | 7,072 |
| 3c/2c + 50/74 + 248/278 + 188/194 | 5,302 | 2,534 | 7,072 |
| 3c/2c + 50/74 + 248/278 + 2d0/2cc | 5,248 | 2,570 | 7,072 |
| 188/194 only | 5,298 | 2,622 | 7,072 |
| 25c/248 only | 5,239 | 2,636 | 7,072 |
| 3c/2c only | 5,239 | 2,639 | 7,072 |
| 40/2c only | 5,236 | 2,657 | 7,072 |

The more aggressive `188/194` addition buys another −41 aligned for +58 raw;
left unlanded because the raw cost exceeds the whole w13607 trade.

## Negative / neutral results this session

All measured with `link_so -j2` + `tools/fn_diff.py`; size-exact unless noted.

* **Full adjacent-swap sweep of all 191 same-depth simple pairs on the base**:
  only the known `0x188`/`0x194` (5,298/2,622) and the `0x2d0`/`0x2cc`
  (5,239/2,663) improve aligned; no raw win.
* **Full non-adjacent swap sweep** (`j-i` in 2..6) over the `0x80..0x1e8`
  region (≈600 builds) and over the rest of the body (≈700 builds):
  best raw 5,234 is the semantically **wrong** `field_0x7c` 12<->18 value
  swap (moves values between the `iPadHD`/`retinaDisplay` arms); the best
  sound raw is 5,235 (codegen-identical).  Best aligned is the landed
  combination.
* **`field_0x7c` if/else arm permutations** (all 6 orders of the
  `iPadHD`/`n9`/`retinaDisplay` arms, values kept): 5,992–6,430 raw,
  sizes 6,704–7,012 — all size-fatal.  The current arm order is load-bearing
  for the 7,072 ridge.
* **`(int)` cast on the `field_0x2cc` iPadHD arm** (making the ternary
  int-typed, matching the original's `str.w` store): 5,710 / 2,866 /
  **7,056** — 16 B short.
* **`(int)(5*1.40625f)` on `field_0x20_top`** (alone / with `0x24` / with
  `0x24`+`buttonInsetX`): 6,195 / 6,202 / 6,216 raw, sizes 6,788–6,820 —
  the float round-trip is load-bearing for the size ridge.
* **Full declaration-order reorder of source lines 25–52** (fields
  `0x8..0x78`): aligned improves to 2,620 but size drops to **7,016**
  (56 B short) and raw rises to 6,019.
* **`0x50`/`0x58` swap on the landed variant**: 5,345 / 2,568 (raw +101 for
  −7 aligned).

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13637
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5244 aligned=2575 size=7072/7072 insn=2512/2552
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13637/`: `base.cpp`, `cH.cpp`
(landed), `cE.cpp`, `cF2.cpp`, `cF3.cpp`, `adj.py`/`adj2.py`/`adj3.py`/`adj4.py`,
`swap2.py`/`swap3.py`, `targ.py`/`targ2.py`, `meas.sh`, `secdiff.py`,
`valall.py`, `emu2.py`, `trace2.py`, `stseq.py`, `kindmap.py`, `blocks.py`.
