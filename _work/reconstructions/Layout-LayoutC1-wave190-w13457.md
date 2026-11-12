# `Layout::Layout` C1/C2 — wave 190 / worker w13457

**Rejected on current master: the K=6 reorder is a regression there.**
On master `a76c924b0` (after the w13458/w13467 Layout changes) the same
reorder measures raw **5,243 -> 5,247** (+4) and `.text` **71,859 -> 71,863**
(+4), `.ARM.extab` unchanged at 575. It was a -1 win only on the older
`83d52fe52` base. Not landed.

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), source
`src/game/ui/Layout.cpp`.  Branch `wave190-w13457`, base master `83d52fe52`.

**Result: size-exact raw win landed — raw 5,249 -> 5,248 (-1).**
Aligned 2,725 -> 2,742, size 7,072/7,072 (exact), insn 2,520/2,552.
`.text` differing bytes 72,257 -> **72,256** (-1), `.ARM.extab` 584 unchanged,
build-id 20, total **72,861 -> 72,860** (-1).  `.rel.plt` byte-identical,
27/27 headers, file size 2,187,328 exact.  `check` green: `RATCHET PASS`,
byte_exact 4507/4524, linked_exact 4507/4524, size-exact 4524/4524, stubs 0,
missing 0, wrong_type 0, extra 0, data_exact 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370.

## 1. The win

A K=6 window permutation of the six consecutive HD-block statements (source
units 130..135, `src/game/ui/Layout.cpp`):

```
before:  0x1d0  0x2c0  0x1c0  0x1cc  0x1d8  0x1dc
after:   0x1d8  0x2c0  0x1dc  0x1d0  0x1cc  0x1c0
```

i.e. permutation tuple `(134, 131, 135, 130, 133, 132)` of the original
0-based indices.  `(134, 131, 135, 130, 132, 133)` also measures 5,248 (the
last two units are interchangeable for the metric); the landed file uses
`(134, 131, 135, 130, 133, 132)`.  Values are unchanged — only the emission
order of the six stores moves.  The `git diff` is 4 moved lines (8 lines
+/-).

| | before | after |
|---|---:|---:|
| raw | 5,249 | **5,248** |
| aligned | 2,725 | 2,742 |
| size | 7,072/7,072 | 7,072/7,072 |
| insn | 2,520/2,552 | 2,520/2,552 |
| `.text` diff | 72,257 | **72,256** |
| `.ARM.extab` diff | 584 | 584 |
| total | 72,861 | **72,860** |

## 2. Why this window

The six units are the tail of the HD ternary block.  The original's HD block
is 164 insns / 277 B (`0xd1042..0xd126c`); ours was 169 insns / 284 B
(`0xd1042..0xd127a`), i.e. +7 B, and the non-HD path started at `d127a` vs
the original `d126c`.  Reordering this window shaves one `.text` byte without
changing the function size or any other section.  It is a pure scheduling /
branch-target effect, not a value change (all 32 flag combinations are
value-identical per the w13385 emulator).

## 3. Negatives measured this session (do not repeat)

All size-exact at 7,072 unless noted.  Baseline `(raw, aligned, size)` =
`(5249, 2725, 7072)`.

* Exhaustive **single-unit moves**: windows 130..203 (5,402 pairs),
  60..129 (4,830), 0..59 (3,540) — best = identity 5,249.  The K=6 window
  above is the only win found.
* Exhaustive **K=6 windows**: 84..89, 112..117, 118..123, 128..133
  (identity), **130..135 (win 5,248)**, 136..141, 186..191, 192..197,
  198..203, 6..11, 12..17 — all others identity or worse.
* `field_0x2bc` moved to end / after `0x284` / before footer:
  5,987 / 5,943 / 5,929, sizes 6,884 / 6,976 / 6,960 (not size-exact).
* Removing the `rd` pointer (using `Globals::retinaDisplay` directly):
  5,398 / size 7,064 (not exact).
* `volatile unsigned char &rd` / `const unsigned char *rd` forms:
  6,539 / 7,308 or 5,398 / 7,064.
* `__builtin_expect(iPadHD,0)` on first if: 6,277 / 7,000.
* `__builtin_expect(retinaDisplay,1/0)` on the `field_0x130` chain:
  5,838 / 7,044 and 5,844 / 7,040.
* `field_0x1e8` as `(retina||iPadHD||n9) ? 1 : 0`: neutral 5,249.
* `int v = ... 61.0f` -> int literals with `(int)(61.0f*1.40625f)`:
  5,501 / 7,032.
* `0x140` etc. moves are already documented in w13383 / w13372 / w13420 /
  w13443.

## 4. Remaining residue (unchanged by this pass)

The original HD block is still 164 insns / 277 B vs ours 169 / 284 B (+7 B),
and the original keeps `Globals::iPadHD` in `sl` (callee-saved) and
`&Globals::retinaDisplay` in `r8`; ours keeps the flag in `lr` then `r5`,
retina pointer in `ip`, reloading the GOT at `d27ec`/`d286e` (the two trailing
pool words `0xd2bc0/0xd2bc4` are GOT addends in ours but `200.0/100.0` in the
original).  The original literal pool is 221 words starting `0xd2854`; ours
188 words starting `0xd28d8`.  The original outlines 5 cold tails at
`0xd2802..0xd2852` (`0x130/0x134/0x138` default, `0x2c8` default, `0x2d0`
default, `0x60` default); ours outlines only the `0x18c` iPad arm and the
`0x60` default.  A further win still needs a source shape that makes clang
pick a callee-saved register for the flag/pointer.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13457
ulimit -v 12000000
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5248 aligned=2742 size=7072/7072 insn=2520/2552
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13457/`: `measure.sh`,
`units.py`, `search.py`, `k6.py`, `k6b.py`, `k6_*.json`, `cand_p1.cpp`,
`cand_p2.cpp`, `Layout.cpp.base`, `orig.dis`, `ours.dis`, `ours2.dis`,
`p0..p3/build` (path-rewritten build copies).

## 7. Pass 2 on current master: neutral

w13457's pass-2 order (`0x1d8, 0x1dc, 0x1d0, 0x1d4, 0x2c0, 0x1cc, 0x1c0`)
was re-tested on current master `7db4ca023` (after the w13458/w13467 Layout
changes). It measures raw **5,243** and `.text` **71,859**, exactly neutral
with the current master order, `.ARM.extab` unchanged at 575. Not landed.
