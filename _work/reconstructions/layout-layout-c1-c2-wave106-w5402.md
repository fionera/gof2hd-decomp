# Layout::Layout C1/C2 — second angle on the mid-block order and the
# shared-int (`vstr`) round trip (w5402, wave 106)

Worker **w5402**, branch `wave106-w5402`, base `486f8a119`.
Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` at ELF VMA `0xd1028`, original size
**7072 B**. Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.

Independent second angle. Read alongside
`layout-layout-c1-c2-branchtree-w5302.md`,
`layout-layout-c1-c2-cast-roundtrip-w5302.md`,
`layout-layout-c1-c2-literal-pool-constants-wf102.md` and
`layout-layout-c1-c2-pad-removal_w3821.md`.

## Result

**No size-exact improvement landed.** The tree stays at **7072 B /
47.84860557768924 %** (`autojunk=False`, unrounded `usc.py`). This pass
contributes one new near-miss and several new negative results:

1. **The true mid-block order is `0x98, 0x9c, 0xac, 0xa8`** (the iOS order),
   not the landed `0x9c, 0xac, 0x98, 0xa8`. That reorder alone compiles to
   **6968 B / 48.429057434 %** — the best fuzzy score measured for this
   function so far (+0.58 pp) — but it is **104 B short of 7072**, and any
   size change shifts `.text` and collapses `byte_exact` (see §4).
2. **The shared-int `vstr` round trip is real but cannot be reproduced at
   `-Oz` without moving the block.** `int v = <ternary>; ac = v;
   a8 = (int)(float)v;` reproduces the original's
   `vmov s0,rN; str rN,[r0,#0xac]; vcvt.f32.s32; vcvt.s32.f32;
   vstr s0,[r0,#0xa8]` in isolation (verified with the NDK clang), but in the
   full ctor clang hoists the shared computation into the retina/default
   block and the fuzzy score collapses to ~18 % (§2).
3. **The 7072-byte size is a fragile coincidence of the landed order.** Of
   the 24 permutations of the four statements `{0x98, 0x9c, 0xac, 0xa8}`,
   exactly one compiles to 7072 B — the landed order. Every other order is
   6912–7056 B (§3).

Gate before == after (verified with `check` on the pristine tree):

| metric | value |
|---|---:|
| `byte_exact` | 4500 / 4524 |
| `linked_exact` | 4500 / 4524 |
| `size_exact` | 4524 / 4524 |
| `data_exact` | 843 / 843 |
| imports | 207 |
| lint / drift | CLEAN / 0 |

`RATCHET PASS`.

## 1. The mid-block store order (new)

The original's mid block stores `0x98` at `d199a`, `0x9c` at `d19a8` (vstr),
`0xac` at `d19ce` and `0xa8` at `d19de` (vstr). The iOS 1.1.4 source order is
the same: `94 98 9c ac a8 a0 a4 b0 …`.

The landed source has `0x9c` (if/else), `0xac`, `0x98`, `0xa8`. Moving `0x98`
in front of the `0x9c` if/else gives the measured table below (all 7072-B
rows are size-exact; the rest are not):

| order (after `0x90`) | `usize` | raw % |
|---|---:|---:|
| `9c ac 98 a8` (landed) | **7072** | **47.848605578** |
| `98 9c ac a8` | 6968 | **48.429057434** |
| `9c 98 ac a8` | 6984 | 48.260695722 |
| `98 ac 9c a8` | 7024 | 47.408293461 |
| `ac 9c 98 a8` | 7024 | 47.586757080 |
| `9c a8 98 ac` | 7064 | 46.880605940 |
| `ac 98 9c a8` | 7016 | 47.516457211 |
| `98 ac a8 9c` | 7000 | 40.800800801 |
| `a8 98 9c ac` | 7008 | 46.506986028 |
| `a8 9c 98 ac` | 7016 | 46.537617242 |

`0x98` alone moved to **every** one of the 234 statement positions in the
ctor: only its landed position (after `0xac`, before `0xa8`) keeps 7072 B.

## 2. The shared-int round trip (new negative result)

The handoff hypothesis is `int v = <ternary>; field = (int)(float)v;` where
`v` is reused by a later field. The pattern is confirmed in isolation with the
NDK r18b clang (`/dev/shm/w5402/t4.cpp`, function `g`):

```
vmov s0, r1
vcvt.f32.s32 s0, s0
vcvt.s32.f32 s0, s0
str  r1, [r0, #0]     ; first field (int)
vstr s0, [r0, #4]     ; second field (int bits of (int)(float)v)
```

but a *single-use* `(int)(float)(<ternary>)` folds to the int select (function
`f`). In the full ctor:

| variant | `usize` | raw % |
|---|---:|---:|
| `int vAc = …; ac = vAc; a8 = (int)(float)vAc;` (order `98 ac a8`) | 7076 | 18.1213 |
| same, order `ac a8 98` | 7100 | 17.8760 |
| `ac = <ternary>; a8 = (int)(float)(<same ternary>)` (order `98 ac a8`) | 6984 | 48.2607 |

The shared variable makes clang compute the value in the retina/default block
(instruction 647 of 2468) instead of the mid block (instruction 922 of 2529 in
the original), which destroys the instruction-stream alignment. Moving the
block to 40 different source positions gives 15.6–39.1 % at 6864–7140 B.

## 3. Expression-space scans (new negative results)

All measured with the stale-`.obj`-removal harness (`/dev/shm/w5402/try.py`),
`autojunk=False`:

* **Single `(int)(float)` wrap** on every single-line ternary of the landed
  body (179 candidates): no 7072-B hit; the best is the unchanged 47.8486.
  On the `98 9c ac a8` body (196 candidates): no 7072-B hit either.
* **Single if/else→ternary conversion** of the 13 ctor if/else chains on the
  `98 9c ac a8` body (179 candidates): no 7072-B hit; best 48.429 (unchanged).
* **Single-statement random moves** on the `98 9c ac a8` body (500 builds):
  only 2 landed at 7072 B, both < 37 %.
* **Adjacent swaps** on the `98 9c ac a8` body (311 builds): best 7072-B hit
  is the landed order (47.8486); the best overall is 48.470 at 6976 B (an
  invalid swap of the `field_0x190` if/else brace).
* **All 24 permutations** of `{0x98, 0x9c, 0xac, 0xa8}`: only the landed
  order is 7072 B (§1).
* **All 120 permutations** of `{0x94, 0x98, 0x9c, 0xac, 0xa8}`: no 7072-B
  hit; best 45.55 at 6916 B.
* **`(int)(float)`, `(int)(float)(double)`, `(float)`, `(double)`** on
  `0x158/0x15c/0x160/0x170/0x138`: all code-identical (clang folds the
  constant select). Only `0x1e4` changes (7056–7072, score ≤ 47.85).

The landed 7072-B tree remains a genuine local optimum of the fuzzy metric
under every order/expression move tried here (≈ 2000 new measured builds).

## 4. Why the size may not move

`byte_exact` compares each function's raw bytes at its **own** address, but
the literal-pool words hold `GOT_entry - pool_address` offsets. Shrinking
`Layout::Layout` by 104 B shifts `.text` and changes every pool word in every
following function: with the `98 9c ac a8` body the verifier reports
`byte_exact 2454` (down from 4500) even though `linked_exact` stays 4500
(`LevelScript::process` drops to 54.2 %). So the ctor **must** stay exactly
7072 B; a non-size-exact variant cannot be landed.

## 5. Proven vs hypothesis

* **Proven (measured):** the `98 9c ac a8` order is 6968 B / 48.429 %; the
  landed order is the unique 7072-B permutation of the four statements; the
  shared-int pattern is exact in isolation and disruptive in the full ctor;
  the fuzzy optimum at 7072 B is still the landed tree.
* **Hypothesis:** the remaining 104 B are the original's extra 16-bit
  flag/branch code and `strd`/`vst1.32` pairing (w3821 §"Remaining root
  cause"), not a missing source construct. The original's end pool has 221
  words vs our 194; the missing 27 words are float constants our clang folds
  (`5.625`, `75.0`, …) and cannot be re-added without changing the code.

## 6. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 /dev/shm/w5402/usc.py                           # 47.848605578, 7072 B
```

Scratch (`/dev/shm/w5402`): `usc.py` (unrounded scorer), `try.py`
(rewrite→`link_so`→score harness, deletes the stale `.obj`), `perm4.py`,
`perm5.py`, `scan3.py`/`scan9a.py`, `conv9a.py`, `if2t.py`, `adj3.py`,
`rand3.py`, `rand9a.py`, `movesh.py`, `t4.cpp` (isolated shared-int proof),
`v9a.cpp` (the 6968-B `98 9c ac a8` body), `Layout.base.cpp` (pristine).
