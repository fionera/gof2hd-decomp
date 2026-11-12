# `Layout::Layout` C1/C2 — wave 190 / worker w13467

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, linked symbol size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave190-w13467`, base master `a7e268b3c` (raw **5,249**, aligned
**2,725**, whole-`.text` diff **72,083**, `.ARM.extab` diff **578**, build-id
**19**, total **72,680**).

**Result: one size-exact raw win (boolean-chain simplification).**

| metric | before | after |
|---|---:|---:|
| raw same-address differing bytes | **5,249** | **5,248** (-1) |
| aligned (difflib) | 2,725 | 2,726 |
| size | **7,072/7,072** | **7,072/7,072** |
| instructions | 2,520/2,552 | 2,521/2,552 |
| whole-`.text` diff | 72,083 | **72,082** (-1) |
| `.ARM.extab` diff | 578 | 578 (size/base exact `0x1a3b78`, 156,312 B) |
| build-id diff | 19 | 19 |
| total (text+extab+build-id) | 72,680 | **72,679** |

Gate after the change: `RATCHET PASS` — byte_exact 4507/4524, linked_exact
4507/4524, size-exact 4524/4524, stub_zero_size 0, missing 0, wrong_type 0,
extra 0, data_exact 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.

## Landed change

`field_0x114_strip5c` had the four-arm ternary

```cpp
this->field_0x114_strip5c =
    Globals::iPadHD ? 1 * 1.40625f
                    : (Globals::n9 ? -1 : (Globals::retinaDisplay ? -1 : 1));
```

The `n9` and `retina` arms are both `-1` and the `iPadHD`/default arms are both
`1` (the HD arm `1 * 1.40625f` truncates to `1`), so it was rewritten to the
equivalent two-arm form

```cpp
this->field_0x114_strip5c =
    (Globals::iPadHD || (!Globals::n9 && !Globals::retinaDisplay)) ? 1 : -1;
```

Value-identical for all 32 flag combinations (HD -> 1; n9 or retina -> -1;
otherwise -> 1).  The raw same-address metric moves 5,249 -> 5,248; size stays
exactly 7,072 B and `.rel.plt`/`.ARM.extab` content are unchanged.

## Search that found it

Prompted by the Ghidra decompile (`_work/reconstructions/layout-orig-ghidra-decompile.txt`),
which shows the original ctor lowering many int fields as pure-int conditionals
(`str`, no `vcvt`/`vstr`) while our source uses float HD arms.  The decompile's
`(int)fVar` sites are only the 20 fields `0x60,0x68,0x6c,0x70,0x2c8,0x2d0,0x8c,
0x9c,0xa8,0x138,0x150,0x158,0x15c,0x160,0x168,0x170,0x1a8,0x1e4,0x3e0,0x3f8`.

Also measured and rejected in this pass (all size-exact unless noted):

* exhaustive **real-build** 2-opt statement swaps of the 186 top-level ctor
  assignments (17,205 pairs): no candidate beats raw 5,249; the 81 proxy ties
  all link at 5,249;
* exhaustive **real-build** single-statement moves within +-12 positions
  (4,464): no candidate beats 5,249;
* exhaustive permutations of the `0x7c/0x84/0x88/0x9c/0x90/eventBannerScale/0x8c`
  group (5,040): identity is the only size-exact 5,205-proxy candidate;
* converting int-field float HD arms to integer literals: all 110 fields
  individually (61 neutral, none better), whole body `(6395, 6724)`;
* `field_0x1e4` as `(int)(float)(...)` (the original really does use a float
  conversion there): `(5953, 6960)` — size-fatal;
* `field_0x90` recovered flag order: `(5407, 7072)`;
* `field_0x2d4`/`field_0x2d8` rewritten to the decompile's
  `(!iPad && !n9) ? (retina ? a : b) : (retina ? c : d)` structure:
  `(5491, 6940)` / `(5609, 7064)`;
* equal-arm simplifications of the other 21 four-arm ternaries: only
  `field_0x200` is neutral; the rest are worse or size-fatal.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13467
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5248 aligned=2726 size=7072/7072 insn=2521/2552
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13467/`: `measure.py`,
`score_linked.py`, `run.py`, `sweep_swap2.py`, `sweep_move.py`,
`sweep_perm7.py`, `sweep_field.py`, `cand_114_v1.cpp`.
