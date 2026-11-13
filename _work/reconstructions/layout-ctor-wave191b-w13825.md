# `Layout::Layout` C1/C2 — wave 191b / worker w13825

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, linked symbol size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave191b-w13825`, base master `a37bfa0a7` (raw 5,578 / aligned 2,012).

**Result: one size-exact aligned win landed (raw −1, aligned −4).**

| | before | after |
|---|---:|---:|
| raw | 5,578 | **5,577** |
| aligned | 2,012 | **2,008** |
| size | 7,072/7,072 | 7,072/7,072 |
| insn | 2,516/2,552 | 2,516/2,552 |
| `.text` byte diff | 76,746 | **76,745** |
| `.ARM.extab` byte diff | 625 | 625 |
| `.ARM.exidx` / `.rodata` / `.data` / `.bss` | 0 | 0 |
| `.rel.plt` | identical | identical |

`check` = RATCHET PASS: byte_exact 4508, linked_exact 4508, size-exact 4524,
data 843/843, imports 207, lint `CLEAN ✓`, drift 0, parity 5370/5370.
All 27 section sizes/base addresses unchanged; `.rel.plt` byte-identical.

## The win

Four independent-field statement reorderings in the ctor body.  Every target is
a distinct, side-effect-free store whose RHS reads only `Globals::`, so the
final store image is unchanged for every flag combination.

```
m   field_0x114_strip5c  -> before field_0x118        # 0x114 before 0x118
s   field_0x200          <-> orbitInfoTextX if/else   # 0x200 / 0x220
s   field_0x300          <-> field_0x1ec              # 0x300 / 0x1ec
s   field_0x2a4_metricA  <-> field_0xf0               # 0x2a4 / 0xf0
```

The load-bearing edit is `m(89,84)` (`field_0x114_strip5c` before
`field_0x118`): alone it measures 5,583 / 2,008 / 7,072 — the only single
move found that reaches aligned 2,008 at size-exact.  The three swaps are
aligned-neutral raw recoveries (each alone 5,575–5,577 / 2,012); combined they
bring raw back below the 5,578 base.

`field_0x114` sits in the original's `0x108..0x114` vector-store group
(`add.w r2, r0, #264 ; vst1.32` at orig +0x172), immediately before the
`0x118..0x124` group — moving it before `0x118` reflects that original source
adjacency and is the reason the alignment improves.

## Search

* Distance-1..8 swap sweep over all 206 ctor units (1,612 candidates, 300
  size-exact): no swap reaches aligned < 2,012.  Best aligned 2,012.
* Move sweep of units 75..106 → every position 40..110 (2,139 candidates):
  `m(89,84)` is the only size-exact point below aligned 2,012.
* Full move sweep of `field_0x114_strip5c` and `field_0x118` to *every*
  position (410 builds): `m(89,84)` is the unique best; `m(89,92)` is the
  runner-up at 5,579 / 2,012.
* Move sweep on top of `m(89,84)` over units 80..96 × positions 80..96 (272
  builds): the aligned 2,008 plateau is stable; no further move improves it.
* Distance-1..8 swap sweep on top of `m(89,84)` (1,612 builds): best raw at
  aligned ≤ 2,008 is 5,580 (`s(147,150)`); adding `s(196,197)` + `s(77,78)`
  gives the landed 5,577.  No further swap lowers raw below 5,577.

### Negative / not landed

* **`0x94..0xa8` group.**  The original emits `stmia.w` for
  `0x94,0x98,0x9c,0xa0` and `strd` for `0xa4/0xa8`; ours stores them
  individually.  Making them contiguous in source order (e.g.
  `[0xac,0x8c,0x90,0x94,0x98,0x9c,0xa0,0xa4,0xa8]`) merges the stores and
  **shrinks** the body to 6,840 B (candidate A) — not size-exact.  Single
  `m(0x94, 44..46)` is 6,984 B and aligned ≈ 2,304 (worse).  The original has
  compensating bytes elsewhere that we do not yet reproduce.
* **`0x108..0x12c` vector groups.**  Ascending reorder
  `[0x108,0x10c,0x110,0x114,0x118,0x11c,0x120,0x124,0x128,0x12c]` produces
  the two `vst1.32` groups plus the `0x128/0x12c` `strd`, but shrinks to
  7,016 B (aligned 2,180).  Partial groups: `0x108..0x114` → 7,040 B;
  `0x128/0x12c` → 7,040 B; `0x118..0x124` → 7,008 B.  All short of 7,072.
* **Largest residual diff blocks** are structural, not store order:
  orig `+4786` (56 insn, literal-pool/vector tail), `+264` (54, `0x94`
  group), `+4302` (52), `+6492` (45, literal-pool words decoded as `movs`),
  `+3888` (41), `+5092` (38), `+3998` (37).

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13825
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5577 aligned=2008 size=7072/7072 insn=2516/2552
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13825/`: `base.cpp`, `unitsmod.py`,
`gen.py`, `harness.py` (single-TU compile + gold link + `fn_diff`),
`dsweep.py`/`dsweep2.py`, `msweep.py`/`msweep1.py`/`msweep2.py`,
`dsweep_8_0_206.json`, `m1_89-84.json`, `m2_80_97_80_97.json`, `ds2_8.json`.

## Harvest rebase (2026-11-13, on master a5ba6e02b = w13816 + w13808)

The `field_0x114_strip5c` move is measured on the combined tree:

| tree | raw | aligned | `.text` diff |
|---|---:|---:|---:|
| w13816 + w13808 (kept) | **5,571** | 2,011 | **75,987** |
| + this `field_0x114` move | 5,576 | **2,007** | 75,992 |

So the move is aligned-positive (-4) but raw/.text-negative (+5/+5) on top of the
w13808 `field_0x94`/`field_0x1ec` order.  It was a raw -1 win on the w13816-only
order, i.e. the two reorders interact.  Kept as a note; the code change was not
landed because raw and `.text` are the primary residual metrics.
