# `Layout::Layout` C1/C2 — wave 191b / worker w13808

Continuation of the w13775 store-order campaign on `_ZN6LayoutC1Ev` /
`_ZN6LayoutC2Ev` (aliased, one body), original vaddr `0xd1028`, linked symbol
size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave191b-w13808`, base master `088c6e950` (raw 5,578 / aligned 2,012).

**Result: 1 size-exact aligned win landed.**

| | before | after |
|---|---:|---:|
| raw | 5,578 | **5,577** |
| aligned | 2,012 | **2,011** |
| size | 7,072/7,072 | 7,072/7,072 |
| insn | 2,516/2,552 | 2,516/2,552 |
| `.text` diff bytes | 75,937 | 75,936 (−1) |
| `.ARM.extab` diff bytes | 594 | 594 (0) |

Rebased onto master `62a081c2d` during harvest: the same swap measures `.text` 76,636 -> 76,635 and `.ARM.extab` 623 (unchanged); raw 5,578 -> 5,577, aligned 2,012 -> 2,011, size 7,072/7,072 exact.
| `.ARM.exidx` / `.rodata` | 0 / 0 | 0 / 0 |

`check` = RATCHET PASS: byte_exact 4508, linked_exact 4508, size-exact 4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
`.rel.plt` byte-identical; all ELF section sizes unchanged.

## Landed step

| step | swap | raw | aligned | size | insn | `.text` | extab |
|---|---|---:|---:|---:|---:|---:|---:|
| 1 | `s(141,197)` | 5,577 | **2,011** | 7,072 | 2,516 | 75,936 | 594 |

`s(141,197)` swaps the independent, side-effect-free stores
`this->field_0x94` (`Globals::iPadHD ? 14 : (n9 ? 15 : (retina ? 20 : 10))`,
source line 208) and `this->field_0x1ec`
(`Globals::iPadHD ? 26 : (n9 ? 34 : (retina ? 38 : 19))`, source line 304).
Both write distinct scalar fields and read only `Globals` flags, so the store
image is unchanged for every flag combination.

## Search

* Full single-swap sweep of **distance 51..100** (6,525 pairs) on the pre-session
  point: exactly one size-exact improving pair, `s(141,197)` → 2,011. Next best
  size-exact results: `s(142,197)` 2,014, `s(83,177)` 2,022, `s(83,174)` 2,023,
  `s(133,197)` 2,027. Only 140/6,375 measured pairs stayed size-exact at all —
  far-apart units are almost never independent (temporaries/`this` reads cross).
* Distance 11..50 (6,825 pairs) and 1..10 (1,917 pairs) had already been
  exhausted on the same point by w13775 / the preceding session: 0 improving.
* Double swaps, moves and permutations around the best units were all worse
  (e.g. `s(177,189)+s(31,45)` 2,015, `m(189,177)` 2,045 size 7,024,
  `s(17,28)` 2,018 size 7,072).

## Remaining structural gaps (not source-order fixable in the tried space)

* Instruction count: original 2,552 vs ours 2,516. The original re-materializes
  constants (`movs` 163 vs 99, `mov.w` 40 vs 35) where we CSE into fewer
  registers and emit more `str.w` (84 vs 43) / `cmp.w`.
* `stmia` shape at `0x2c0`: original covers 4 words `{0x2c0,0x2c4,0x2c8,0x2cc}`
  from `add.w ip,r0,#704`; ours covers 3 words from `#708` because `0x2c0` is
  stored separately earlier (`strd r3,r6,[r0,#704]`).
* `0x94..0xa0` group: original `stmia.w r4,{r5,r8,ip,lr}` at `#148` then
  `strd r9,r3,[r0,#164]`; ours stores `0x9c` early and `0x98/0xa0/0xa8/0xa4`
  late. Contiguous-block moves and individual moves were all worse.
* `0xdc..0x104` group: original uses `strd` for `0xdc/0xe0`, `0xe4/0xe8`,
  `0xfc/0x100` and `str.w` for `0x104`; ours uses `vst1.32` for `0xdc`.
* Literal pool: original 5 runs (150+16+5+2+189 words); ours differs in tail
  placement.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13808
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5577 aligned=2011 size=7072/7072 insn=2516/2552
```

Scratch harness (`/home/debian/GalaxyOnFire2/scratch/w13808`):
`python3 harness.py base.cpp` → `(5578, 2012, 7072, '7072')`;
`python3 try.py tag 141,197`; `python3 sweep_all.py <dmin> <dmax> <out.tsv>`.
