# Layout::Layout C1/C2 — wave 122 / worker w7201

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (identical bodies, aliased at
`0xd1028`), original size **7072 B**, source `src/game/ui/Layout.cpp`.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(sha256 `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`).

This pass (a) re-derived the residual from the *executed* instruction stream
rather than the linear disassembly, (b) corrected a wrong claim in the wave-119
hand-off about the store order, (c) built a fast single-TU scorer and ran
~2 500 reorder/expression variants, and (d) landed the one proven semantic
difference (`field_0x184` arm order).

## Result

| metric | before | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| unrounded `difflib` ratio | 48.963317384370 % | **48.704663212435 %** |
| `report.json` pct (1 dp) | 49.0 | 48.7 |
| semantic mismatches over 32 flag combos | **2** | **0** |
| `byte_exact` / `linked_exact` | 4506 / 4506 | **4506 / 4506** |
| `size_exact` / `data_exact` | 4524 / 843 | 4524 / 843 |
| imports / lint / drift | 207 / CLEAN / 0 | 207 / CLEAN / 0 |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.

The landed change is a **correctness** fix: the constructor now writes exactly
the same 204 `this` words as the original for **all 32** combinations of the
five device flags.  The fuzzy metric penalises it by 0.26 points because the
arm swap perturbs clang's register allocation; the alternative (leave the
semantic bug) is a guaranteed non-byte-identical `.so`, so the fix is landed.

## 1. The landed change — `field_0x184`

The original checks `iPad` **before** `n9`:

```cpp
// before (n9 first)
this->field_0x184 = Globals::iPadHD ? 163 : (Globals::iPadLarge ? 346
    : (Globals::n9 ? 210 : (Globals::iPad ? 173 : (Globals::retinaDisplay ? 232 : 116))));
// after (faithful)
this->field_0x184 = Globals::iPadHD ? 163 : (Globals::iPadLarge ? 346
    : (Globals::iPad ? 173 : (Globals::n9 ? 210 : (Globals::retinaDisplay ? 232 : 116))));
```

Evidence: emulate both constructors over all 32 flag combinations and compare
the raw byte map written to `this` (`/dev/shm/w7201/cmp32.py`, built on
`_work/reconstructions/layout_ctor/emu.py`).  Before the fix exactly two
combinations differ, both at byte offset `0x184`:

| flags (`iPadHD n9 iPad iPadLarge retina`) | original | before |
|---|---:|---:|
| `01100` | 173 | 210 |
| `01101` | 173 | 210 |

After the fix: `bad combos 0 of 32`.  This reproduces the wave-119 finding
(`layout-literal-pool-241-words-wave119-w6964.md` §4) and is the only semantic
difference in the whole constructor.

## 2. Correction: the store order is *not* identical

The wave-119 hand-off states "store order is already identical to the original
for all three configs".  That is an artefact of comparing the **final store
dictionary** (`emu.run(...)` returns `{offset: byte}`; two different orders
give the same dict).  Comparing the **store trace** (`emu.run.last.trace`)
gives:

| config | trace length orig / ours | order agreement |
|---|---:|---:|
| `iPadHD=1` | 620 / 608 | **82.4 %** |
| `n9=1` | 585 / 578 | **82.2 %** |
| default | 486 / 414 | **56.2 %** |

(the default-path length differs because our build stores `field_0x20_top`
with `vstr` — 4 byte writes in the emulator — while the original uses a plain
word store.)

So statement order *is* still a live lever; it is just that the current
hill-climbed order is a very strong local optimum for the fuzzy metric.

## 3. Where the residual actually is

Score is a `difflib` ratio over the **linear** disassembly.  378 equal runs,
average length 3.25, total 1228 of 2529 instructions.  Executed-trace ratios
(same normalisation, per config) are much lower and show where the code
diverges:

| config | orig insns | ours insns | executed-trace ratio |
|---|---:|---:|---:|
| `iPadHD=1` | 323 | 329 | **19.9 %** |
| `n9=1` | 587 | 544 | 34.5 % |
| `retinaDisplay=1` | 1062 | 1070 | 39.0 % |
| `iPad=1` | 1056 | 1055 | 36.5 % |
| `iPadLarge=1` | 845 | 908 | 34.9 % |
| default | 1138 | 1130 | 39.2 % |

The HD path is the worst and is also the largest block (137 stores), so it
dominates the linear score.

### 3a. Provenance census (default path)

Instrumenting the emulator to tag every stored word with whether its value
passed through a `vcvt` (float round-trip) or a `vst1` (SLP group) gives the
exact per-offset type differences (orig vs ours, default config):

* original round-trips, ours does not: `0x138 0x158 0x15c 0x170 0x1e4 0x3f8`
* ours round-trips, original does not: `0x20 0x10c 0x124 0x148 0x204 0x3e8`
* original SLP-vectorises, ours stores scalar:
  `0xb0..0xbc 0xec..0xf8 0x108..0x114 0x13c..0x148 0x1d0..0x1dc 0x20c 0x218
  0x220 0x23c 0x2a4..0x2ac`
* ours SLP-vectorises, original stores scalar: `0x16c..0x178 0x24c 0x298`

The SLP groups on the original side are exactly the adjacent runs in the iOS
source order (`4 8 c 10 14 18 1c 20 24 28 2c 30 ...`, `b0 b8 b4 bc`,
`ec f0 f4 f8`, `108 10c 110 114`, `13c 140 144 148`, `1d0 1d4 1d8 1dc`,
`2a0 2a4 2a8 2ac`) — i.e. the original really is field-major in the iOS order
and clang SLP-vectorised the adjacent runs.

### 3b. Why the round-trips cannot simply be added

`(int)(float)(<int ternary>)` is folded by clang whenever the value range is
provable.  A minimal reproducer compiled with the project's clang 7.0.2 at
`-Oz` shows the fold happens for a select of constants but **not** for
`(int)(float)(s->a + 17)` (a memory-derived value).  Consequently every
`(int)(float)` wrapper added to a constant-ternary field in `Layout.cpp`
compiles to a no-op: variants `t2 t3 t4 t6 t7 t8` (wrap `0x158`, `0x15c`,
`0x170`, or a temporary) all reproduce the baseline byte-for-byte at 7072 B /
48.963317 %.  The original's round-trips exist because its value is a *phi*
across basic blocks, which the folding cannot see — a scheduling consequence,
not a source-level cast.

## 4. Variants measured this pass

All scores are the exact unrounded `difflib` ratio of `_ZN6LayoutC1Ev`
(`/dev/shm/w7201/score.py`, which reproduces `report.json` bit-for-bit).

### 4a. Statement order (size must be exactly 7072 — see §5)

| variant | size | ratio % |
|---|---:|---:|
| **baseline (hill-climbed order)** | **7072** | **48.9633** |
| iOS source order (`ios_source_order.md`) | 6848 | 36.1066 |
| original HD compiled store order | 6760 | 34.4422 |
| ascending offset order | 6912 | 20.2247 |
| `field_0x20_top` moved next to `field_0x24` | 7072 | 46.3502 |

### 4b. Searches (fast single-TU scorer, `/dev/shm/w7201/fastvar.py`)

The scorer compiles only `Layout.cpp`, extracts `.text._ZN6LayoutC2Ev`
(0x1ba0 B) and diffs it against the original with the same normalisation as
`tools/verify`.  It reproduces the ranking of every full-link variant tested
(absolute offset ≈ −0.68 pt, monotone).

| search | variants | best 7072-B result |
|---|---:|---:|
| all adjacent unit swaps (203 units) | 202 | 48.9633 (no hit) |
| random 2-opt unit swaps, hill-climbed | 1 262 | 48.9633 (no hit) |
| random unit insertions, hill-climbed | 1 258 | 48.9633 (no hit) |
| `(int)(N * 1.40625f)` on every HD float arm (86 fields) | 86 | 48.9633 (no hit) |
| add/remove `(int)` / `(int)(float)` layers (196 edits) | 196 | 48.9633 (no hit) |
| the above two sets, all pairs | ~1 700 | none reached 7072 with a gain |

Only two single edits beat the baseline, and both shrink the body by 8 B:
`field_0x124` or `field_0x128` HD arm → `(int)(1 * 1.40625f)` gives
**7064 B / 48.9828 %**.  Every combination tried that restored 7072 B
(`+0x204`, `+0x194`, `+0x180`, `+rewardBoxY2`, `+0x160` drop) landed between
48.44 % and 48.93 %, i.e. below the baseline.

### 4c. Other structural edits

| variant | size | ratio % |
|---|---:|---:|
| `field_0x130/0x134/0x138` as three plain ternaries | 7088 | 48.6454 |
| `field_0x184` iPad-first (**landed**) | 7072 | 48.7047 |
| `rewardBoxY2` wrapped in `(int)(float)` | 7088 | 48.4559 |

## 5. Why the size must stay exactly 7072

`byte_exact` compares raw function bytes, and many functions carry inline
literal pools whose `adr`/`addw` offsets depend on the section layout.  Any
size change to `Layout::Layout` shifts the whole `.text` and collapses
`byte_exact`:

| Layout size | `byte_exact` |
|---:|---:|
| **7072** | **4506** |
| 7064 | 3222 |
| 7080 | (also non-7072) |

So the task is a constrained search: 7072 B *and* a higher ratio.  The
baseline is a 2 500-variant local optimum of exactly that constrained problem.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7201
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506

# exact single-function score
python3 /dev/shm/w7201/score.py cmake-build-match/libgof2hdaa.so

# semantic check over all 32 flag combinations
python3 /dev/shm/w7201/cmp32.py                         # bad combos 0 of 32

# executed-trace comparison for one config
python3 /dev/shm/w7201/cmptrace.py iPadHD=0 n9=0 retinaDisplay=0 iPad=0 iPadLarge=0
```

Scratch harnesses (not committed, `/dev/shm/w7201/`): `emu3.py` (emulator with
`vcvt`/`vst1` provenance), `cmp32.py`, `cmptrace.py`, `cmporder.py`,
`prov.py`, `vcmap.py`, `fastvar.py` (single-TU scorer), `search2.py` /
`search3.py` / `search4.py` (order searches), `sweep.py` / `sweep2.py`
(expression sweeps), `reorder_ios.py` / `reorder_hd.py` / `reorder_asc.py`
(order generators), `body_*.txt`.

## 7. Remaining root cause

The residual is **clang's register allocation and SLP scheduling**, not the
source semantics (which now match for all 32 combos).  The original is
field-major in the iOS source order; that order compiles to 6848 B here,
224 B short of the original, because the original's `(int)(float)` round-trips
survive as phi-crossing conversions while ours are folded.  The current
hill-climbed order buys 224 B of extra code (and therefore the 7072-B layout
the gate needs) at the cost of diverging from the true source order.  Breaking
this trade-off needs either the exact Android 2.0.16 source order (not the iOS
1.1.4 order) or a way to make clang keep the round-trips, neither of which was
found in ~2 500 measured variants.
