# Layout::Layout C1/C2 — wave 119 / worker w6798

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (identical bodies), ELF VMA `0xd1028`,
original size **7072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave119-w6798`, base = current wave-119 master
(`251d2c254`, after w6761's `0x138`/`0x28c`/`0x128` pass).
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.

## Result — no 7072-B source variant beats the wave-119 base; note-only commit

| metric | before (base) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| unrounded `difflib` ratio (linked) | 48.655110579 % | **48.655110579 %** |
| `byte_exact` / `linked_exact` | 4505 / 4505 | **4505 / 4505** |
| `size_exact` / `data_exact` | 4524 / 843 | **4524 / 843** |
| imports / lint / drift | 207 / CLEAN / 0 | **207 / CLEAN / 0** |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
No source change landed, so file size (2,187,328 B), the 27 section headers,
`.text`, `.ARM.exidx`, `.ARM.extab` and `.rodata` are unchanged from the base.

## The 7072-B wall, re-measured (the w6720 number was stale)

The best source shape found in this pass is **7052 B, 48.793137842 %**
(`S + move 0x28c`, see below).  A full `link_so` + `verify` of that variant
gives:

| metric | base | 7052 variant |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | 7052 |
| `_ZN6LayoutC1Ev` pct | 48.7 | **48.8** |
| `byte_exact` | 4505 | **3221** |
| `linked_exact` | 4505 | 4505 |

So the wall is real but the collapse is to **3221**, not the 2454 quoted in
`layout-literal-pool-got-wave119-w6720.md` §5.  3221 < 4503, so a 20-B shrink
still cannot land.

## Method — exact linked scoring, exhaustive where affordable

`/dev/shm/w6798/fastscore.py` compiles `Layout.cpp` with the exact
`compile_commands.json` flags and gold-links the single object into a throwaway
`.so`; `verify.disassemble` + `reconcile` + `difflib` then score
`_ZN6LayoutC1Ev`.  Verified to reproduce the full-link number exactly on the
base (7072 / 48.65511057979678).  ~0.4 s/variant, 32-way parallel.

## Exhaustive searches (all variants scored at the exact linked metric)

| search | scope | variants | land at 7072 | best 7072 ratio |
|---|---|---:|---:|---:|
| single-statement move, **all** distances | 244 statement lines × every destination | 80,520 | 474 | **48.655110580** (= base) |
| pairwise swap of statement lines | C(244,2) | 29,646 | 601 | **48.655110580** (= base) |
| single move from the 7052 best (`SM`) | same | 80,520 | 8 | 42.811629 |
| single move from `S+150+9c` (7068) | same | 80,520 | 513+ (killed early) | 48.645418 |
| cast add/remove on `SM` | 202 fields | 404 | 0 | — |
| cast add/remove on `S+150+9c` | 202 fields | 404 | 0 | — |

Random moves/swaps (2,584 samples) best ratio per size:

| size | best ratio % |
|---:|---:|
| 7056 | 48.693918 |
| **7072** | **48.655111** |
| 7048 | 48.652963 |
| 7060 | 48.574277 |
| 7064 | 48.515049 |

**Conclusion:** at the mandatory 7072 B, no single-statement reorder, no
statement swap, no cast edit and no float/type rewrite found beats the base.
The best source shape overall (`S + move 0x28c`, 48.793138 %) is 20 B short.

## Variants measured (full-link-equivalent ratio)

| variant | change | size | ratio % |
|---|---|---:|---:|
| base | — | **7072** | **48.655111** |
| `S` | swap `field_0x1a0` / `field_0x1a8` lines | 7076 | 48.675563 |
| `150` | drop `(int)(float)` on `field_0x150` | 7064 | 48.664807 |
| `S+150` | | 7060 | 48.684211 |
| `S+150+9c` | + `(int)(float)` on `field_0x9c` | 7068 | 48.704663 |
| **`SM`** | `S` + move `this->field_0x28c = …;` after `rewardBoxX` | **7052** | **48.793138** |
| `SM` + `(int)(float)` on `field_0x9c` | | 7060 | 48.773679 |
| `SM` + `(int)(float)` on `field_0x2d8` | | 7076 | 48.6056 |

## New analysis: the `vstr` (float round-trip) census

Restricting to the hot body (instructions before `pop`), the sets of
`vstr sN, [r0, #off]` destinations differ between original and base:

| | offsets |
|---|---|
| original only | `0x138, 0x158, 0x15c, 0x170, 0x1e4, 0x3f8` |
| ours only | `0x20, 0x10c, 0x148, 0x1f4, 0x204, 0x3e8` |

`vstr` here means a real `(int)(float)(int)` round-trip (`vmov s0,rN;
vcvt.f32.s32; vcvt.s32.f32; vstr`), i.e. the source expression is float-typed
and converted back to int.  The original therefore has round-trips on six
fields ours lacks, and ours has six the original lacks.  Individual fixes
(all real `link_so` builds):

| change | size | ratio % |
|---|---:|---:|
| `field_0x138 = (int)(float)v138` | 7088 | 48.457097 |
| `field_0x1e4 = (int)(float)(…)` | 7032 | 45.681229 |
| `field_0x20_top` HD arm `5*1.40625f` → `7` | 6872 | 20.559244 |
| `field_0x148` HD arm `2*1.40625f` → `2` | 7064 | 48.505381 |
| `field_0x1f4` HD arm `16*1.40625f` → `22` | 7064 | 48.425668 |
| `field_0x204` HD arm `9*1.40625f` → `12` | 7072 | 48.127490 |
| `rewardBoxWidth` HD arm `100*1.40625f` → `140` | 7144 | 24.578122 |
| `field_0x10c_rowBaseY` remove `(int)(float)(int)` | 7024 | 43.504291 |
| all six "ours-only" fixes together | 6800 | 24.818108 |

Note `(int)(N * 1.40625f)` is folded by clang 7.0.2 exactly like `N * 1.40625f`
(same size/score), so the cast spelling does not matter; only removing the
float from the ternary changes the IR type.

## New analysis: `field_0x2d4` / `field_0x2d8`

The original materialises both with one `strd r4, r6, [r0, #724]`
(`0x2d4`, `0x2d8`); the base stores them separately (and in the opposite
order).  Rewrites:

| variant | size | ratio % |
|---|---:|---:|
| `0x2d4` chain as one nested ternary | 7064 | 48.146672 |
| `0x2d4` chain without the `volatile rd` local | 7064 | 48.604466 |
| move the whole `0x2d4` if-chain before `0x2d8` | **7072** | 48.535565 |
| move `0x2d8` after the `0x2d4` if-chain | **7072** | 48.535565 |

## Other dead ends (all real builds)

| variant | size | ratio % |
|---|---:|---:|
| sort every single-line `this->field = …;` by struct offset | 5992 | 16.559966 |
| SLP adjacency `0xb0..0xcc` (8 fields, address order) | 7024 | 41.892971 |
| SLP adjacency `0xb0..0xbc` | 7096 | 43.376365 |
| SLP adjacency `0xc0..0xcc` | 7088 | 47.423911 |
| SLP adjacency `0xec..0xf8` | 6936 | 40.786833 |
| SLP adjacency `0x108..0x114` | 7016 | 43.353293 |
| SLP adjacency `0x118..0x124` | 7064 | 48.655111 |
| SLP adjacency `0x2a0..0x2ac` | 7048 | 44.098884 |
| SLP adjacency `0x16c..0x178` | **7072** | 48.655111 |
| move `field_0x2c0` next to `field_0x2c4` | 7028 | 45.238571 |
| move `field_0x2c4` next to `field_0x2c0` | 6968 | 21.624875 |
| `helpWindowY` n9/retina arm swap | 7016 | 40.968194 |
| `helpWindowX` `||` operand reorder | **7072** | 48.655111 |

## Proven vs hypothesis

**Proven (measured):**

* No single-statement move (any distance) and no pairwise statement swap at
  7072 B beats 48.655110580 %; the exhaustive searches cover every such edit.
* The best source shape found (`S + move 0x28c`) is 7052 B / 48.793138 % and
  cannot land: full link gives `byte_exact` 3221.
* The original's hot body has `vstr` round-trips at six fields ours lacks and
  ours has six the original lacks (table above).
* The original pairs `0x2d4`/`0x2d8` in one `strd`; every source rewrite tried
  either shrinks below 7072 or scores lower.

**Hypothesis (not proven):**

* The remaining gap is dominated by the original's eight extra 4-int SLP
  groups (32 pool words) plus four outlined cold `else` tails (52 B), which
  together replace ~184 B of our hot scalar code.  Reproducing them from the
  base requires a source order that both restores the SLP groups and keeps the
  total at exactly 7072 B; every adjacency reorder measured here lands below
  7072 B.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6798
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6     # RATCHET PASS, byte 4505

# fast single-function scorer (reproduces the full-link number exactly)
python3 /dev/shm/w6798/score.py          # -> size 7072 ratio 48.655110580
python3 /dev/shm/w6798/score2.py <src.cpp> <tag>
```

Scratch harnesses (not committed, `/dev/shm/w6798/`): `fastscore.py`,
`score.py`, `score2.py`, `trace.py`, `gather.py`, `exsearch.py`, `exswap.py`,
`rsearch*.py`, `rbest.py`, `sweep2.py`, `side2.py`, `diffsum.py`,
`Layout.base.cpp`, `SM.cpp`, `S1509C.cpp`, `G118.cpp`, `orig.dis`, `ours.dis`.
