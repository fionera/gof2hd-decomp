# `Layout::Layout` C1/C2 — wave 124 / worker w7401

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7072 B (0x1ba0)**, source `src/game/ui/Layout.cpp`
(constructor only).  Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(sha256 `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`).

## Result — no source win

| metric | before (HEAD `6f8ee2320`) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| unrounded `difflib` ratio | 48.894202 % | **48.894202 %** |
| `report.json` pct (1 dp) | 48.9 | 48.9 |
| `byte_exact` / `linked_exact` | 4506 / 4506 | **4506 / 4506** |
| `size_exact` / `data_exact` | 4524 / 843 | 4524 / 843 |
| imports / lint / drift | 207 / CLEAN / 0 | 207 / CLEAN / 0 |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
No `src/` change landed; the worktree is clean apart from this note.

## 1. What this pass established

The wave-123 hand-off proposed reconstructing the original as a *config-major
204-store initializer with the stores inside each branch arm*, so that the
`(int)(float)` round-trips survive as int phis across store-bearing blocks.
This pass rebuilt that structure three ways and measured every variant with
real `link_so` builds.  The conclusion is that the config-major body is
**structurally correct but ~400 B too small**, and that the 7072-B size is a
hard gate (any other size shifts `.text` and collapses `byte_exact`).

### 1.1 The original really is config-major, but its common1 is a *tree*

Recovered from the executed-PC trace and the branch CFG (all 32 flag
combinations, `emu2.py` + capstone basic blocks):

```
if (iPadHD) {                       // 0xd1042..0xd1268, 137 stores
    ...HD constants (10 SLP preloads, then vst1 groups)...
    b.w 0xd21e2                     // HD skips common1
} else {
    if (n9) { ...34 stores... }     // 0xd127e..0xd12fc
    else    { ...34 stores... }     // 0xd1570..0xd1830 (straight-line selects)
    /* common1: 0xd1860..0xd21e0, 101..103 stores, a tree of if/else groups */
}
/* part2 / common2: 0xd21e2..0xd2800, 25..26 stores */
/* outlined cold tails: 0xd2802..0xd2852 (4 iPad arms) */
/* literal pool: 0xd2854..0xd2bc8, 221 words */
```

The branch conditions are `n9` (`fp`), `retinaDisplay` (`r8`), `iPadLarge`
(`r9`), `iPad` and the `(retina|n9|iPad)` composite.  The first common1
blocks, with the exact store sets, are:

| block | condition | stores |
|---|---|---|
| `0xd185e` | all non-HD | `2d0 2d4 2d8` |
| `0xd18bc` | `n9` | `7c 80 84 88` (one `vst1`) |
| `0xd1904` | `!n9` | `7c 80 84 88` (two `strd`) + `8c 90` |
| `0xd1962` | `n9` | `94` |
| `0xd1972` | `!n9` | `94 98 9c` |
| `0xd19b6` | `iPadLarge` | (no stores) |
| `0xd19bc` | `!iPadLarge && iPad` | `ac` |
| `0xd19c0` | all | `ac a8` |
| `0xd19e4` | `n9` | 22 stores `a0 a4 d0 d4 d8 dc e0 e4 e8 104 2b4 2b8 b0 c0 ec 294 2a4 118 108 128 140 138 13c` |
| `0xd1ac6` | `!n9` | 23 stores `d4 d0 a0 a4 d8 fc 100 104 298 dc e0 e4 e8 b0 c0 ec 2a0 108 118 11c 120 124 128 12c` |
| `0xd1c9a` | `!n9 && iPadLarge` | `130 134` |
| `0xd1cae` | `!n9 && !iPadLarge && retina` | `130 134 14c 138 13c 150` |
| `0xd1d38` | `iPadLarge` | `154 158 15c` |
| `0xd1d52` | `iPad` | `154` |
| `0xd1d62` | `!iPad && n9` | `154` |
| `0xd1d9c` | `!iPad && !n9` | `154` |
| `0xd1da6` | all | `158` via `vcvt.f32.s32`/`vcvt.s32.f32`/`vstr` |

So the original's common1 is **not** a per-field ternary chain; it is a tree
of grouped if/else blocks.  This is exactly what makes it 2434 B long.

### 1.2 The wave-121 config-major body is 400 B short

The wave-121 body (`/dev/shm/w7401/wave121_body.txt`) already has the HD /
n9 / else arms in the iOS source order and the common1 fields in the original
store order.  It compiles to **6672 B / 27.729914 %** (0 `vcvt` — all the
round-trips are folded).  A generator that uses the *current* expressions
instead of the final ints (`gen3.py`) gives **5488 B / 23.464183 %**; grouping
consecutive fields by their leading `Globals::X ?` condition (`group.py`)
gives **6768 B / 26.746507 %**.

The reason the config-major body scores far below the 48.9 % field-major
baseline is register allocation, not source semantics.  The original's HD arm
preloads ten constant groups into `d0-d1, d30-d31, d16-d17, …, d2-d3` and then
uses `d2-d3` as the store scratch; the wave-121 body preloads the same ten
groups but uses `d30-d31` as the scratch.  The baseline (field-major) uses yet
another rotation.  The allocator choice depends on the whole function's
register pressure, which is why the 400-B size difference changes it.

## 2. Variants measured this pass (all real `link_so` builds)

| variant | size | unrounded % |
|---|---:|---:|
| **baseline (HEAD)** | **7072** | **48.894202** |
| wave-121 config-major body | 6672 | 27.729914 |
| `gen3.py` config-major (current exprs) | 5488 | 23.464183 |
| wave-121 grouped by leading condition | 6768 | 26.746507 |
| field-major in wave-121 (iOS) order | 7012 | 21.669980 |
| baseline, region `8c..2b8` reordered to iOS | 7032 | 39.968153 |
| baseline, `154/158/15c` grouped int-phi | 7020 | 48.750250 |
| wave-121, `154/158/15c` grouped int-phi | 6664 | 27.729914 |
| baseline, `0x158` int-phi alone | 7072 | 48.894202 (code-identical: folded) |
| baseline, SLP group `b0 b8 b4 bc` made contiguous | 7016 | 43.773736 |

### 2.1 Removing the six extra round-trips (single edits, baseline)

The baseline round-trips `0x20 0x10c 0x148 0x1f4 0x204 0x3e8` which the
original does not.  Converting each HD arm to `(int)(N * 1.40625f)` (so the
ternary is int-typed and the conversion folds) gives:

| field | size | % |
|---|---:|---:|
| `0x20` | 6760 | 18.599034 |
| `0x10c` | 7048 | 45.024925 |
| `0x148` | 7064 | 48.624950 |
| `0x204` | 7080 | 48.606688 |
| `0x1f4` | 7064 | 48.784376 |
| `0x3e8` | 7200 | 24.099723 |

The size deltas are not additive.  The only 7072-B combinations
(`0x148+0x204`, `0x204+0x1f4`) both compile to 7064 B and score 48.496 /
48.536 — below the baseline.

### 2.2 Why the int-phi alone does not survive

`int u158; if (iPadHD) u158 = 70; else if (…) u158 = …; this->field_0x158 =
(int)(float)u158;` compiles **byte-identically** to the baseline (the branches
contain no stores, so clang if-converts to selects and folds the conversion).
Grouping `154/158/15c` so the branches *do* contain stores keeps the phi
(`vcvt` count 41 → 39, `0x158` gains a `vstr`), but the body shrinks to
7020 B and the score falls to 48.750.  This confirms the wave-123 finding:
the round-trip needs a store-bearing branch *and* the exact surrounding
register pressure.

## 3. Remaining root cause

The residual is clang 7.0.2 register allocation + SLP scheduling on a body
that must be exactly 7072 B.  The field-major baseline is a strong local
optimum for the fuzzy metric: every reorder measured this pass (iOS region,
SLP-group contiguity, grouped int-phi, config-major tree) changes the size
away from 7072 and/or lowers the score.  The true source is config-major with
a grouped common1 tree, but reproducing it needs the original's *exact*
outline/merge decisions in common1 (the 400-B gap) so that the whole-function
register pressure — and hence the HD-arm `d2-d3` scratch — matches.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7401
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
python3 /dev/shm/w7401/score.py                         # 7072 B, 48.894202
```

Scratch harnesses (not committed, `/dev/shm/w7401/`): `score.py` (exact
single-function ratio), `try.py` (splice body → `link_so` → score),
`emu2.py` / `emu_ours.py` (store/PC emulator), `cfg3.py` / `tree.py` /
`blockcfg.py` (CFG, store sets, per-block flag conditions), `gen3.py`
(config-major generator), `group.py` (leading-condition grouping),
`gen_ios2.py` (iOS-order field-major generator), `wrap.py` (int-wrap ballast).
