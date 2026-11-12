# `Layout::Layout` C1/C2 — wave 190 / worker w13372

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, linked symbol size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave190-w13372`, base master `0f54626b3` (raw **5,256**, whole-`.text`
diff **73,516**, `.ARM.extab` diff **610**, build-id **20**, total **74,146**).

**Result: one size-exact raw win (if/else chain → ternary for `field_0x1cc`).**

| metric | before | after |
|---|---:|---:|
| raw same-address bytes | **5,256** | **5,255** (-1) |
| aligned (difflib) | 2,726 | 2,726 |
| size | **7,072/7,072** | **7,072/7,072** |
| instructions | 2,520/2,552 | 2,520/2,552 |
| whole-`.text` diff | 73,516 | **73,515** |
| `.ARM.extab` diff | 610 | 610 (size/base exact `0x1a3b78`, 156,312 B) |
| build-id diff | 20 | 20 |
| total (text+extab+build-id) | 74,146 | **74,145** |

Gate after the change: `RATCHET PASS` — byte_exact 4507/4524, linked_exact
4507/4524, size-exact 4524/4524, stub_zero_size 0, missing 0, wrong_type 0,
extra 0, data_exact 843/843, imports 207, lint CLEAN, drift 0, parity
5370/5370.  `.rel.plt` byte-identical (29,392 B, diff 0); `.ARM.exidx`,
`.data`, `.rel.dyn` diff 0.

## Landed change

`field_0x1cc` was reconstructed as a four-arm `if/else if/else` chain; the
original is a plain ternary chain:

```cpp
    this->field_0x1cc = Globals::iPadHD ? 36 * 1.40625f
                                        : (Globals::n9 ? 54 : (Globals::retinaDisplay ? 72 : 36));
```

Values are unchanged (`HD 50, n9 54, retina 72, default 36`, matching the
recovered chain below).  The compile-only proxy moved 5,253 -> 5,252 and the
linked body moved 5,256 -> 5,255 one-for-one; size, instruction count,
`.rel.plt`, `.ARM.extab` content and all section bases are unchanged.

## Method: recovered per-field flag chains vs. the source

The checked-in `_work/reconstructions/layout_ctor/` tooling (`emu.py`,
`lay_orig.json`, `chain.py`) emulates the original ctor for all 32 flag
combinations and recovers, per field, the minimal flag-test chain
(`iPadHD`, `n9`, `iPadLarge`, `retinaDisplay`, `iPad`, default).  Running
`chain.py lay_orig.json` and diffing the recovered chain order against every
`this->field = ...` RHS in `Layout.cpp` (correct header offsets via a
comment-stripped `Layout.h` parse) leaves only these genuine order
differences:

| field | source order | recovered chain order |
|---|---|---|
| `0x090` | HD, retina, iPad, n9 | HD, n9, retina, iPad |
| `0x184` | HD, iPadLarge, n9, iPad, retina | HD, iPadLarge, iPad, n9, retina |
| `0x1e8` | retina, HD, n9 | HD, n9, retina |
| `0x31c` | HD, iPadLarge, retina, n9, iPad | HD, iPadLarge, n9, retina, iPad |

(`0x090`'s non-HD arms are all the same value 0.325f, so its order is
ambiguous in the chain; `0xa8`/`0x2c8` are assigned through locals and were
checked separately.)  Rewriting `0x1e8` and `0x31c` to the recovered order is
codegen-neutral (clang canonicalises the `||`/ternary order); rewriting
`0x184` is **worse** (size 6,924, raw 5,481).  Only the `if/else -> ternary`
form change for `0x1cc` is a win.

## Searches run on the current body (all negative or neutral)

* random K=6 window permutations over the whole body, 8,607 candidates
  (12,000 requested, 25 min timeout): best = identity, proxy 5,253 / 7,072.
* exhaustive K=6 over window units 84..89 (the w13317 landing window):
  identity is optimal; every other permutation is 5,254 or worse.
* all 24 permutations of units 124..127 (`0x1b0/0x1b4/0x1b8/0x1bc`): neutral.
* ascending-offset sort of units 8..27 (`0x2c..0x78`): size 7,056, raw 5,820;
  of units 8..33: 7,056 / 5,904.  Matching the original HD store order makes
  the raw metric worse, so the current scrambled order is a real local optimum.
* moving `0x2c0` (unit 131) next to `0x2c4..0x2cc`: size 6,872..6,904, raw
  ~5,914..5,973 (breaks the common-block grouping).
* moving `0x1c0` / the `0x1cc` if/else / both to the end: size 6,904..6,944,
  raw 5,868..5,976.
* `0x2d4` if/else -> ternary: size 6,956, raw 5,439.  `0x190` if/else ->
  ternary: size 7,088, raw 5,358.  `0x20c`/`orbitInfoTextX`: size 7,088.
  `0x218`/`orbitInfoStationY`: size 7,048.  `0x260`: size 7,072 but raw 5,846.
  `0x280`: neutral.  `rewardBoxY2`: size 6,960.

## Residual structure (for the next worker)

The remaining diff is dominated by SLP grouping / register allocation, not
values (all 32 flag combos still store identical bytes):

* HD block: original groups `{0x34,0x38,0x3c,0x40}`, `{0x44..0x50}`,
  `{0x54..0x60}`, `{0x64..0x70}` into `vst1.32`; ours groups `{0x38..0x44}`,
  `{0x48..0x54}`, `{0x58..0x64}`, `{0x68..0x74}` (shifted +4).  This one
  grouping difference also makes our n9 block 16 B longer (pool base
  `0xd1310` vs original `0xd1300`).
* n9 block: original uses `strd` for `0x74/0x78` and `stmia` for
  `0x2c0..0x2cc`; ours stores `0x78`, `0x34`, `0x2c4`, `0x98`, `0x2cc`
  individually.
* common block: ours SLP-vectorises `0x1b0..0x1cc` into `vbsl` + two
  `vst1.32`; the original stores them as `strd` pairs.
* literal pool: original end pool starts `0.325, 0.65, 100, 50` at
  `0xd2854`; ours has a retina-local island at `0xd1918..0xd1928`.
* tail: ours starts at `0xd22a6` (HD block branches to it), original at
  `0xd21e2`; ours' pre-tail gap is 0xb4 larger and its tail correspondingly
  shorter.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13372
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5255 aligned=2726 size=7072/7072 insn=2520/2552
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13372/`
(`score.py`, `sweep.py`, `k6sweep.py`, `randk6.py`, `cmpchains.py`,
`hdroff.py`, `uoff.py`, `tryifelse2.py`, `ordercmp.py`, `emu2.py`,
`chains2.json`, `pools.py`, `stypes2.py`, `blockstores.py`).

## Current-master harvest (wave 190)

Applied on master `6493e7e74`. The `field_0x1cc` if/else -> nested ternary
change still wins: Layout C1/C2 raw **5,256 -> 5,255** each, aligned 2,726
unchanged, size exact 7,072 B. `.text` **73,007 -> 73,006** (-1);
`.ARM.extab` 603 and build-id 20 unchanged; total (text+extab+build-id)
**73,630 -> 73,629** (-1). `check` -> RATCHET PASS. w13390 found the same
change independently (duplicate); the w13394 HangarWindow `rh`/`top`
declaration swap was measured neutral on this base and not harvested.
