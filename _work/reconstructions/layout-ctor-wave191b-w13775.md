# `Layout::Layout` C1/C2 — wave 191b / worker w13775

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, linked symbol size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave191b-w13775`, base master `42b002f39` (raw 5,494 / aligned 2,095).

**Result: 8 size-exact aligned wins landed (raw +85, aligned −83).**

| | before | after |
|---|---:|---:|
| raw | 5,494 | 5,579 |
| aligned | 2,095 | **2,012** |
| size | 7,072/7,072 | 7,072/7,072 |
| insn | 2,512/2,552 | 2,516/2,552 |
| `.text` diff | 74,984 | 74,667 (−317) |
| `.ARM.extab` diff | 564 | 561 (−3) |
| `.ARM.exidx` / `.rodata` | 0 / 0 | 0 / 0 |

`check` = RATCHET PASS: byte_exact 4508, linked_exact 4508, size-exact 4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
`.rel.plt` byte-identical; all ELF section sizes unchanged.

## Landed steps

Every step is a swap of two **independent, side-effect-free stores** in the
constructor body.  The only RHS that reads `this` (`field_0x158 = ... field_0x154`)
is never separated from `field_0x154`, so the store image is unchanged for all
32 `Globals` flag combinations (verified with an x86 host emulator of the ctor
that prints the full 7,072-byte object for each of the 32 combos; every landed
candidate diffs clean against the base image).

Unit indices below are **relative to the immediately preceding file** (the
sweep harness re-derives them per candidate).

| step | swaps | raw | aligned | size | insn | `.text` | extab |
|---|---|---:|---:|---:|---:|---:|---:|
| base | — | 5,494 | 2,095 | 7,072 | 2,512 | 74,984 | 564 |
| 1 | `s(145,152)` | 5,538 | 2,032 | 7,072 | 2,512 | 74,626 | 561 |
| 2 | `s(101,106) s(182,184)` | 5,591 | 2,029 | 7,072 | 2,514 | 74,679 | 561 |
| 3 | `s(101,102)` | 5,573 | 2,028 | 7,072 | 2,516 | 74,661 | 561 |
| 4 | `s(131,132)` | 5,610 | 2,026 | 7,072 | 2,514 | 74,698 | 561 |
| 5 | `s(98,99)` | 5,625 | 2,025 | 7,072 | 2,516 | 74,713 | 561 |
| 6 | `s(135,137) s(97,106)` | 5,618 | 2,016 | 7,072 | 2,516 | 74,706 | 561 |
| 7 | `s(75,79) s(101,102)` | 5,644 | 2,013 | 7,072 | 2,514 | 74,732 | 561 |
| 8 | `s(102,103)` | 5,579 | **2,012** | 7,072 | 2,516 | 74,667 | 561 |

Step 1 is also an `.ARM.extab` win (−3 bytes of content).

## Canonical permutation from `base.cpp`

The per-step indices are relative to successive candidates.  The equivalent
single permutation of the pre-session `base.cpp` (206 units, verified to
reproduce the landed `src/game/ui/Layout.cpp` byte-for-byte) is 23 swaps:

```
s(58,62) s(58,64) s(75,79) s(76,77) s(97,101) s(98,99) s(97,106)
s(102,103) s(117,123) s(131,132) s(135,137) s(145,153) s(149,152)
s(149,145) s(173,175) s(173,176) s(182,184) s(185,186) s(187,189)
s(188,196) s(187,188) s(190,187) s(192,190)
```

(`s(a,b)` swaps the units currently holding original labels `a` and `b`, applied
left to right.)

## Search

* Full distance-1..10 swap sweep on the accumulating point after every landed
  step (1,917 size-exact pairs per sweep, 5 sweeps).  Best size-exact results:
  step 1 `s(145,152)` 2,032; step 2 `s(101,106)+s(182,184)` 2,029;
  step 3 `s(101,102)` 2,028; step 4 `s(131,132)` 2,026; step 5 `s(98,99)` 2,025;
  step 6 `s(135,137)+s(97,106)` 2,016; step 7 `s(75,79)+s(101,102)` 2,013;
  step 8 `s(102,103)` 2,012.
* Adjacent-only sweeps after each step: exhausted (no size-exact swap below the
  current point) at every step.
* Final point: no size-exact single swap in distance 1..10 improves on 2,012;
  a targeted distance-11..20 probe over units 95..125 (310 pairs) also finds
  nothing (best 2,161).  Two size-exact 2,012 swaps combined are still 2,012
  (the whole `0x180..0x1ac` region is insensitive to these swaps).

### Size-compensation attempts (did not pay)

Several single swaps are much better on aligned but shrink the symbol, and the
required compensating swap always costs more aligned than it saves:

| candidate | size | aligned | needed | best size-exact completion |
|---|---:|---:|---:|---:|
| `s(118,122)` | 7,048 | 1,996 | +24 | `+s(89,94)` → 2,023 |
| `s(119,121)` | 7,032 | 2,000 | +40 | — |
| `s(105,107)` | 7,040 | 2,002 | +32 | — |
| `s(105,106)` | 7,056 | 2,005 | +16 | — |
| `s(182,184)` | 7,032 | 2,006 | +40 | `+s(179,180)` → 2,205 (z15 point) |

The size effect of a pair is not additive (e.g. `s(118,122)+s(89,94)` is 7,072
on one base and 7,080 on another), so each completion had to be measured.

### Remaining structural gap

The largest remaining diff block is `orig[83:137]` vs `ours[84:127]`
(`origoff=+264`, `oursoff=+270`): the original stores `0x94,0x98,0x9c,0xa0`
as one `stmia.w r4, {r5,r8,ip,lr}` and `0xa4/0xa8` as a `strd`, while ours
stores `0x9c` early and `0x98/0xa0/0xa8/0xa4` late.  Making those four
assignments contiguous in the source (`0x94` unit 141, `0x98` unit 31,
`0x9c` unit 46, `0xa0` unit 50) was tried as a block move and is much worse
(6,036 / 2,288 and 6,215 / 2,784, sizes 6,912 / 6,764), so clang's scheduler is
not following plain source order there.  This block is the obvious next target
for a non-swap reconstruction.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13775
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5579 aligned=2012 size=7072/7072 insn=2516/2552
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13775/`: `base.cpp` (pre-session
source), `harness.py`, `combo.py`, `move.py`, `try.py`, `gen_pairs.py`,
`sweep_pairs.py`, `gen_host.py` (32-combo store-image emulator), `secdiff.py`,
`permdiff.py`, `chain.py`, `diffdump_full.py`, the `z*.cpp` candidate chain
(`z20.cpp` = landed), the `d1_10_z*.tsv` sweep logs, and `img_base.txt` /
`img_z*.txt` store images.
