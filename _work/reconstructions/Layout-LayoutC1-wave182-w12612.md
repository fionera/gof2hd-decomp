# `Layout::Layout` C1/C2 — wave 182 / worker w12612

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave182-w12612`, base master `57e085059` (post-wave-181 tree; the
wave-181 `w12510` permutation is landed, raw **5,391**).

**Landed: a size-exact raw win.**

| metric | before | after |
|---|---:|---:|
| raw same-address bytes | **5,391** | **5,385** (-6) |
| aligned (difflib) | 2,520 | 2,562 (+42) |
| size | 7,072/7,072 | **7,072/7,072** |
| instructions | 2,518/2,552 | 2,518/2,552 |

The raw metric is the worker objective; aligned is reported honestly and moved
the other way (the original has 34 more instructions than us, so raw wins tend
to trade aligned structure for same-address byte agreement).

## 1. Winning move

One pure statement move in the constructor, no value/type/control-flow change:

* `orbitInfoSystemY` (unit 149) moved **after** the `field_0x218` if/else
  chain (unit 153).  In source order the window

  ```
  orbitInfoSystemY
  field_0x234
  if (iPadHD) orbitInfoStationY ... else ...
  if (iPadHD) field_0x218 ... else ...
  ```

  became

  ```
  field_0x234
  if (iPadHD) orbitInfoStationY ... else ...
  if (iPadHD) field_0x218 ... else ...
  orbitInfoSystemY
  ```

  `git diff` is one moved line.  Raw **5,391 -> 5,385** (-6), size exact.

## 2. Method

The ctor was re-parsed into **204 top-level units** (statements and complete
if/else chains; the parser keeps brace-less `if/else` chains together and the
round-trip reproduces the file byte-for-byte).  Every candidate was a real
`cmake --build ... --target link_so -j2` + `tools/fn_diff.py` measurement
(~0.9 s/build); only size-exact (`7,072/7,072`) raw improvements were kept.

* **W=2 exhaustive**: all 203 adjacent-unit swaps, repeated to a fixed point
  after any win.  No improvement (5,391 throughout).
* **W=3 exhaustive**: all 202 windows x the 5 non-identity permutations
  (1,010 builds).  No improvement.
* **Distance-4..8 single-unit moves**: every unit moved to every position
  4..8 away in both directions (2,040 builds).  One win: `149 -> 153`
  (5,391 -> 5,385).  A second full distance-4..8 pass on the landed order
  found no further size-exact win, so the landed order is a local optimum for
  W=2, W=3 and distance-4..8 moves.

## 3. Residual

Unchanged in kind: the original is 6,186 B / 2,552 insns, ours 6,312 B /
2,518 insns.  The original has more 16-bit `movs`/`it`/`moveq`, a larger
literal pool (221 vs 190 words) and SLP-vectorises a different group set, so
the residual is instruction selection / register allocation / store schedule,
not a wrong value (the wave-100 store-trace work showed all 807 stored bytes
agree in all six flag configurations).  Local statement permutation around the
current body is exhausted.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12612
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5385 aligned=2562 size=7072/7072 insn=2518/2552
cmake --build cmake-build-match --target check -j2
# RATCHET PASS: byte_exact 4507, linked_exact 4507, size-exact 4524/4524,
# data_exact 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w12612/`: `units.py` (204-unit
parser), `units.json` (parsed units), `sweep.py` (`w2`/`w3`/`move` searches),
`w2_run.txt` / `w3_run.txt` / `move48_run.txt` (per-candidate logs),
`order_win1.json` + `Layout.win1.cpp` (landed ctor), `secdiff.py` (section
byte diff).

Gate at exit: byte_exact **4507/4524**, linked_exact **4507/4524**,
size-exact **4524/4524**, stub_zero_size **0**, missing **0**, wrong_type
**0**, extra **0**, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift **0**, parity **5370/5370**, `.rel.plt` byte-identical.
`.text` differing bytes **75,359** (-6), `.ARM.extab` **655** (unchanged),
build-id **20**, total **76,034**.  `Layout` C1/C2 raw **5,385**, aligned
**2,562**, size **7,072/7,072**, 2,518 instructions.
