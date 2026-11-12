# `Layout::Layout` C1/C2 — wave 190 / worker w13383

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, linked symbol size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave190-w13383`, base master `5cd492611` (wave-190; raw **5,256**,
aligned **2,726**, whole-`.text` diff **73,487**, `.ARM.extab` diff **610**).

**Result: one size-exact raw win (statement move).**

| metric | before | after |
|---|---:|---:|
| raw same-address bytes | **5,256** | **5,250** (-6) |
| aligned (difflib) | 2,726 | **2,725** (-1) |
| size | **7,072/7,072** | **7,072/7,072** |
| instructions | 2,520/2,552 | 2,520/2,552 |
| whole-`.text` diff | 73,487 | **73,481** (-6) |
| `.ARM.extab` diff | 610 | **610** (size/base exact `0x1a3b78`, 156,312 B) |
| build-id diff | 20 | 20 |
| total (text+extab+build-id) | 74,117 | **74,111** |

Gate after the change: `RATCHET PASS` — byte_exact 4507/4524, linked_exact
4507/4524, size-exact 4524/4524, stub_zero_size 0, missing 0, wrong_type 0,
extra 0, data_exact 843/843, imports 207, lint CLEAN, drift 0, parity
5370/5370.  `.rel.plt` unchanged.

## Landed change

Move the `field_0x140` assignment (offset `0x140`) from its position after
`field_0x1f8` (unit 186) to just before `field_0x200` (unit 139), i.e. swap
the two ctor statements:

```
    this->field_0x238_blk16.a = ... ;   // 0x238, unit 139
    ...
    this->field_0x140 = ... ;           // 0x140, unit 186
```

becomes

```
    this->field_0x140 = ... ;           // 0x140
    this->field_0x238_blk16.a = ... ;   // 0x238
```

i.e. the `0x140` store is hoisted above the `0x238` block field.  The
compile-only proxy (`scratch/w13383/proxy2.py`, constant +2 offset from the
linked body) predicted 5,254 -> 5,248; the linked raw moved 5,256 -> 5,250
one-for-one.

## Search that found it

Exhaustive single-pair statement swap (20,706 valid pairs, 4 workers,
`scratch/w13383/opt2.py`) on the current wave-190 body.  Adjacent and
distance-2 swaps alone are all neutral (5,254 proxy); the only improving
pairs are `(139,186)` -> 5,248 and `(142,186)` -> 5,251 (proxy).  The
`(139,186)` swap was landed.

## Structural study (why the body is still a deep local optimum)

The remaining 5,250 differing bytes are spread almost uniformly across the
non-iPadHD path (~60 of every 64 bytes from file offset `0x400` to `0x18c0`),
i.e. the whole non-HD instruction schedule differs, not a local window.

* The original ctor is **config-major**: an `if (iPadHD)` straight-line
  block at `d1042..d126c` (554 B, 164 insns) that stores every first-part
  field with literal-pool constants, then an `if (n9)` partial block at
  `d127e..d12fc` (126 B), then a mostly-predicated retina/default path
  (`d1570..`) and a shared tail from `d1860`/`d21e2`.
* HEAD's iPadHD block is 568 B (14 B longer), so the whole non-HD path is
  shifted by +14 bytes: original non-HD starts `d126c`, ours `d127a`.
  Even after a 14-byte shift only 402/3,958 non-HD bytes match, so the
  schedule itself (vst1/strd grouping, `it`/`moveq` predication) differs.
* Reconstructed candidates (scratch `gen2.py`..`gen8.py`, compile-only
  proxy): 2-way HD/else split = 6,904 B; 3-way HD/n9/else = 6,400 B;
  4-way HD/n9/iPadLarge/else = 7,220..7,288 B; 5-way = 8,168 B.  The
  original 7,072 sits between the 3-way and 4-way shapes; none of the
  generated shapes is size-exact, and their raw is 6,200..6,600 (worse
  than 5,250) because the non-HD schedule is still not reproduced.
* A faithful 2-way split does reproduce the original HD block size
  (552 B vs 554 B) but leaves the non-HD path ~176 B too small.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13383
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5250 aligned=2725 size=7072/7072 insn=2520/2552
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13383/`
(`proxy2.py`, `units3.py`, `opt2.py`, `tern.py`, `gen2.py`..`gen8.py`).
