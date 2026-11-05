# Layout::Layout C1/C2 — wave 119 / worker w6807
## SLP-group / cold-block continuation: no 7072-B source win; best raw 48.9706 % at 7024 B

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (identical bodies, aliased at
`0xd1028`), original size **7072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave119-w6807`, base wave-119 master `319bd4d30`.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.

## Result — note only, no source change landed

| metric | before (HEAD) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| unrounded `difflib` ratio | 48.824233 % | **48.824233 %** |
| `report.json` pct (1 dp) | 48.8 | 48.8 |
| `byte_exact` / `linked_exact` | 4505 / 4505 | **4505 / 4505** |
| `size_exact` / `data_exact` | 4524 / 843 | 4524 / 843 |
| imports / lint / drift | 207 / CLEAN / 0 | 207 / CLEAN / 0 |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
No source change landed, so the 27 section headers, file size 2,187,328 B,
`.text`, `.ARM.exidx`, `.ARM.extab` (`0x26290`, 8 B short) and `.rodata`
are unchanged.

## The 7072-B wall

`Layout::Layout` is the **last large function before the rest of `.text`**.
Changing its size by any amount shifts every later symbol and every
PC-relative PLT reference, which collapses whole-`.so` `byte_exact`.
Measured this wave: the best 7024-B variant gives `byte 2454`
(`RATCHET FAIL: byte_exact 4505 -> 2454`).  So only a **7072-B** source
shape is landable.

## Best measured variants (all real `link_so` builds, unrounded ratio)

| variant | size | ratio % |
|---|---:|---:|
| **HEAD (baseline)** | **7072** | **48.824233** |
| `0x16c` chain `retinaDisplay` before `n9` (`n9last`) | 7024 | 48.930642 |
| `n9last` + `0x138 = (int)(float)v138` | 7040 | 48.931496 |
| `n9last` + `0x138` + swap lines 103/104 + swap 107/108 | **7024** | **48.970618** (best raw) |
| `n9last` + `0x138` + swap 103/104 | 7032 | 48.951049 |
| `0x1a0`/`0x1a8` statement swap (`1a0swap`) | 7076 | 48.844622 |
| `1a0swap` + `0x160` int wrap | 7084 | 48.864994 |
| `0x2c8` + `0x2d0` double wraps | 7092 | 48.845541 |
| `0x138` + `1a0swap` | 7092 | 48.845541 |
| `0x130/0x134/v138` if/else → nested ternary (`conv130`) | 7088 | 48.825169 |
| `conv130` + `1a0swap` | 7092 | 48.845541 |

**Every 7072-B variant found this wave scores ≤ 48.824233.**  The best
7072-B alternatives are all neutral reorderings (`48.824233`, identical
code) or worse: `0x180` group adjacency `48.087649`, `swap103`
`48.804781`, `conv130+0x1a4` `48.714371`.

## What the measurements say

The score is dominated by the **hot body** (2069/2529 tokens, ratio 43.7)
not the after-`pop` region (460 tokens, ratio 74.8).  Every edit that
improves the after-`pop` pool alignment (the `n9last` chain reorder, the
`0x138` round-trip) **shrinks** the function; every edit that grows it
back (casts, if/else conversions, field adjacencies) either is absorbed
by clang (no size change) or perturbs the hot body enough to lose more
than it gains.

* `n9last` alone: −48 B, +0.106 pp.  It changes the `0x16c` branch tree
  so clang shares the `0x138`/`0x16c` constant blocks and the end-pool
  order moves closer to the original.
* `0x138 = (int)(float)v138`: +16 B, +0.001 pp.  The original stores
  `0x138` with a single int→float→int round-trip.
* Once `n9last` is applied, **all** the usual int-wrap edits
  (`0x2c8`, `0x2d0`, `0x9c`, `0x160`, `0x180`, `0x194`, `0x1a4`,
  `0x2c4`) become codegen no-ops (same 7040 B / 48.931496 %).  The
  compiler already materialises those round-trips in the reordered tree.
* The `0x1a0`/`0x1a8` swap is the only single edit above baseline at
  `+4 B` (7076 B, 48.844622 %); no `−4 B` edit exists to bring it back
  to 7072 (the extra 4 B is a real extra end-pool word, not padding).
* The original's four cold outlined `else` tails (`0x130/0x134/v138`,
  `0x2c8`, `0x2d0`, `0x60`, 80 B total) still do not reproduce.  The
  best base outlines only the `0x1ac` tail (14 B).  Converting those
  chains to explicit `if/else` **shrinks** the body (`0x60` → 6968 B,
  `0x2d0` → 7016 B on the best base), so the missing 52 B of cold code
  cannot be added this way.

## Searches run (all real `link_so` builds)

* adjacent-statement swap sweep on the **true baseline** (185 builds):
  91 hits at 7072 B, **max 48.824233**; only `1a0swap` (line 176) beats
  it at 7076 B / 48.844622.
* adjacent-statement swap sweep on `n9last+0x138` (7032 B): best
  7024 B / 48.970618; no 7072 hit.
* adjacent-statement swap sweep on `1a0swap` (7076 B): 7072 hits at
  48.824233 max; no win.
* full single-statement `(int)(float)` wrap sweep on the baseline
  (185 variants): no 7072 hit above baseline; best 7072 is
  `field_0x1fc` at 47.281418.
* same sweep on the 7040-B `n9last+0x138` base (185 variants): only
  `field_0x1fc` reaches 7072 B (47.281418); 152 variants are codegen
  no-ops at 7040 B.
* same sweep on the `1a0swap` base (185 variants): no 7072 hit; best
  `1a0swap + 0x160` at 7084 B / 48.864994.
* 2/3/4-edit combinations of `{0x138, 0x2c8, 0x2d0, 0x180, 0x194,
  0x1a4, 0x2c4, 0x9c, 0x160, 1a0swap}` on the baseline (375 builds):
  best 7092 B / 48.845541; no 7072 win.
* `conv130` combinations with `{0x1a4, 0x2c4, 1a0swap, 0x2c8, 0x2d0,
  0x9c, 0x160, 0x194, 0x180}`: best 7076 B / 48.734808; best 7072 is
  `conv130+0x1a4` at 48.714371.
* targeted field-move sweep on the best 7024-B base
  (`eventBannerScale`, `0x2c4`, `0x2c8`, `0x2cc`, `0x2d0`, `0x2d4`,
  `0x2d8`, `0x140`, `0x144`, `0x1e0` × 13 anchors, 120 builds): no
  7072 hit, no score above 48.970618.
* partial `0x180..0x18c` adjacency moves on the 7040-B base: best
  7040 B / 48.931496; the full group gives 7072 B only on the 7024-B
  base and scores 48.087649.
* `rewardBoxY2` if/else → ternary: codegen no-op (7072 / 48.824233).
  `0x2d4` if/else → ternary: 7072 / 48.285486.

## Proven vs hypothesis

**Proven (measured):**

* No 7072-B source variant above 48.824233 exists in the searched
  neighbourhood (single swaps, single wraps, 2–4 edit combos, targeted
  moves, partial SLP adjacencies).
* The best raw shape is 7024 B / 48.970618 %
  (`n9last + 0x138 + swap103 + swap107`), i.e. the original's more
  compact hot body is real; the missing 48 B are the cold outlined
  tails + extra SLP pool words, which cannot be added without losing
  the alignment gain.
* Any size change from 7072 B is unlandable (`byte_exact` 4505 → 2454).

**Hypothesis (untested / needs the original source):** the original
`Layout.cpp` is a flat per-field ternary list in a source order that
lets clang 7.0.2 if-convert the `iPadHD`/`n9` arms into straight-line
SLP blocks and outline the four `iPad ? A : B` tails.  Recovering that
order from the binary is not possible from the store order alone —
clang's SLP vectorizer reorders stores, so the HD-block group order is
not source order (verified: the first nine groups match HEAD exactly
even though HEAD's source order is scrambled).

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6807
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6     # RATCHET PASS, byte 4505
```

Scratch harnesses (not committed): `/dev/shm/w6807/{score.py,try.sh,
edits.py,edits2.py,edits3.py,combo_base.py,sweepwrap.py,sweep_adj2.py,
movesweep.py,adj.py,hdorder.py}` plus the saved variant sources
`Layout.base.cpp`, `Layout.7024.cpp`, `Layout.7040.cpp`,
`Layout.best7024.cpp`, `Layout.1a0swap.cpp`.
