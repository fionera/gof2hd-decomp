# `Layout::Layout` C1/C2 — wave 191b / worker w13686

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, linked symbol size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave191b-w13686`, base master `004b2758f` (w13637 final: raw 5,244 /
aligned 2,575).

**Coordinator merge note (2026-11-13):** re-measured on `2655cec94`:
raw 5,265 / aligned 2,442 both before and after the `field_0x1ec` /
`field_0x94` swap — a tie on this base (the earlier −2 was measured on
`004b2758f`).  Kept because the store image is identical for all 32 flag
combos and the swap is believed to reflect the original source order.

**Result: two size-exact aligned wins landed (raw unchanged, aligned −41).**
Both are strict Pareto improvements on the current base.

| | before | after |
|---|---:|---:|
| raw | 5,244 | **5,244** |
| aligned | 2,575 | **2,534** |
| size | 7,072/7,072 | 7,072/7,072 |
| insn | 2,512/2,552 | 2,512/2,552 |
| `.text` diff | 71,247 | 71,247 |
| `.ARM.extab` diff | 577 | 577 |
| `.ARM.exidx` / `.rodata` / `.data` / `.bss` | 0 | 0 |

`check` = RATCHET PASS: byte_exact 4508, linked_exact 4508, size-exact 4524,
data 843/843, imports 207, lint `CLEAN ✓`, drift 0, parity 5370/5370.
`.rel.plt` byte-identical.  All 27 section sizes/base addresses unchanged.

## The wins

Two swaps of independent field stores (semantic no-ops; every RHS is a pure
`Globals` ternary):

1. **`progressBarYBase` (0xd8) <-> `marqueeWidth` (0xd4)** — aligned −39.
2. **`field_0x1ec` <-> `field_0x94`** — a further aligned −2 at equal raw.

```
this->progressBarYBase  (0xd8)   <->   this->marqueeWidth  (0xd4)
this->field_0x1ec                <->   this->field_0x94
```

`marqueeWidth` moves up to sit immediately after `centerYBase` (0xd0), which
is its original source adjacency (`ios_source_order.md`: `... c8 cc d0 d4 d8
dc ...`); `progressBarYBase` moves to the old `marqueeWidth` slot.  The swap
is a distance-12 unit swap (units 58 and 70 of the 231-unit ctor split), so it
was invisible to the adjacent-swap sweeps of w13633/w13637/w13648.

Semantics verified with the store-image emulator over all 32 flag combinations
(`compare_combos.py`, `scratch/w13686/emu.py`): **bad combos 0 of 32**.

## Search that found it

* Fresh adjacent-swap sweep on the w13637 base (193 simple-statement pairs):
  no size-exact raw win; best aligned 2,534 at `0x188/0x194` (raw 5,302).
* Distance-2..10 swap sweep (1,692 candidates, 204 size-exact): raw floor
  5,239 at the `0x248/0x278` revert (aligned 2,636); best aligned 2,462 at
  units 11/20 (raw 5,688).  No dominating point.
* Distance-11..30 swap sweep (3,470 candidates, 83 size-exact): **unit 58/70
  (`progressBarYBase`/`marqueeWidth`) = 5,244 / 2,536**, the only dominating
  point; `(68,84)` is codegen-identical (5,244/2,575).
* Distance-31..60 swap sweep from the 5,244/2,536 point (4,455 candidates,
  97 size-exact): **unit 153/220 (`field_0x1ec`/`field_0x94`) = 5,244 /
  2,534**, the only dominating point.  Re-swept distances 2..30 (5,162
  candidates, 293 size-exact) and 31..60 (4,455, 101) from the 2,534 point:
  no further dominating single swap.

## Measured candidates (all real `link_so` builds, size-exact)

| variant | raw | aligned |
|---|---:|---:|
| base (w13637) | 5,244 | 2,575 |
| **landed: both swaps above** | **5,244** | **2,534** |
| `progressBarYBase` <-> `marqueeWidth` only | 5,244 | 2,536 |
| + `0x2cc`/`0x2d0` swap | 5,248 | 2,531 |
| + `0x3c`/`0x2c` revert (units 13/15) | 5,240 | 2,564 |
| + `0x248`/`0x278` revert (units 180/183) | 5,239 | 2,597 |
| + units 109/115 | 5,245 | 2,532 |
| + units 147/153 | 5,245 | 2,534 |
| + `0x188`/`0x194` | 5,302 | 2,495 |
| + units 141/143 | 5,254 | 2,515 |
| + units 141/143 + 163/173 | 5,307 | 2,519 |
| + units 11/20 | 5,688 | 2,423 |

`(58,70)` + every one of the 287 size-exact single swaps from the two sweeps
was measured: the best aligned with raw ≤ 5,244 is **2,536** (the landed
point, with or without a codegen-identical addition).  No point with
raw < 5,244 and aligned < 2,536 was found.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13686
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5244 aligned=2536 size=7072/7072 insn=2512/2552
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13686/`: `harness.py`
(compile+gold-link+measure), `units.py` (ctor unit splitter), `combo.py`
(swap/move applier), `sweep_adj.py`, `sweep_dist.py`/`sweep_dist_dump.py`,
`sweep_pairs2.py`, `sweep_add.py`, `dist2_10.json`, `dist11_30.json`,
`addB.json`, `compare_combos.py` + patched `emu.py`.
