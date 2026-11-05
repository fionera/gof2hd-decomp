# Layout::Layout C1/C2 — wave 111 / worker w5901: the 7072-B ceiling and the
# SLP-group / shared-int evidence

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev`, ELF VMA `0xd1028`, original size
**7072 B**, source `src/game/ui/Layout.cpp`.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
Base `8eea060cc` (wave 110). Branch `wave111-w5901`.

## Result

No 7072-B variant above the wave-110 baseline was found. `src/game/ui/Layout.cpp`
is unchanged; this note records the measurements and the structural evidence
found while searching. `cmake --build cmake-build-match --target check -j6`
→ **RATCHET PASS** (byte 4500, linked 4500, size 4524, data 843, imports 207,
lint CLEAN, drift 0). No section / `.ARM.extab` change (still 27 sections,
`.ARM.extab` `0x26294`).

Baseline (HEAD) unrounded linked score: **48.337646824606814 %** at **7072 B**
(report rounds to 48.3 %).

## Exact commands

```sh
cd /home/debian/GalaxyOnFire2/wt/w5901
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 /dev/shm/w5901/fscore.py _ZN6LayoutC1Ev         # unrounded score
```

`fscore.py` is the wave-110 `fscore.py` (same `verify.disassemble` +
`difflib.SequenceMatcher` pipeline, unrounded ratio). One `link_so` build is
~0.9 s, so the whole scan space below is a few minutes of real builds.

## The six "fixes" (faithful, but they shrink the function by 52 B)

The six edits below were measured as a group and in all 64 subsets. They are
**correct against the binary** (each HD arm matches a literal in the original's
HD `vst1` pool vector, see below), but they make the function 7020 B, which is
not landable (a Layout size change shifts `.text` and collapses `byte_exact`).

| # | edit |
|---|---|
| `6c`  | `field_0x6c` HD arm `136 * 1.40625f` → `191` |
| `2d0` | `field_0x2d0` HD arm `45 * 1.40625f` → `63` |
| `138` | `field_0x138` HD arm `47 * 1.40625f` → `66` |
| `180` | drop the `(int)(float)(int)` around `field_0x180` |
| `194` | drop the `(int)(float)` around `field_0x194` |
| `sbi` | `scrollBarInset` HD arm `1 * 1.40625f` → `1` |

Why they are faithful: the original's HD block loads its constants from the
literal pool as 4-word vectors and stores them with `vst1.32 {dN-dM}` (16 B
each). The pool at `0xd13f0` is `{487, 341, 63, 90}` (→ `0x54..0x60`) and at
`0xd1400` `{112, 566, 191, 63}` (→ `0x64, 0x68, 0x6c, 0x70`), so `0x6c`'s HD
value is the integer **191**, not `136 * 1.40625f`. Likewise `0x2d0` is the
fourth word of the `0x2c8` vector (`{63, 126, 63, 112}` at `0xd1410`), i.e.
integer **63**; `0x138`/`0x13c` are `{66, 23}` (`strd r1,r3,[r0,#312]`, with
`movs r1,#66`); `scrollBarInset` is `1`.

The group score/size frontier (all real `link_so` builds):

| fixes applied | size | score |
|---|---:|---:|
| (none, HEAD) | **7072** | **48.337647** |
| `6c` | 7064 | 48.356901 |
| `2d0` | 7064 | 48.356901 |
| `sbi` | 7064 | 48.356901 |
| `138` | 7072 | 48.328025 |
| `180` | 7064 | 48.337647 |
| `194` | 7064 | 48.347272 |
| `6c,2d0` | 7056 | 48.376171 |
| `6c,sbi` / `2d0,sbi` / `138,194` | 7056 | 48.376171 |
| `6c,2d0,138` | 7056 | 48.366534 |
| `6c,2d0,194` / `6c,138,194` | 7048 | 48.395455 |
| `6c,2d0,sbi` | 7048 | 48.395455 |
| `138,180,194` | 7044 | 48.435320 |
| `6c,138,180,194` / `2d0,138,180,194` / `138,180,194,sbi` | 7036 | 48.454636 |
| `6c,2d0,138,180,194` | 7028 | 48.473968 |
| `6c,2d0,138,194,sbi` | 7032 | 48.434071 |
| all six | **7020** | **48.533227** |

The full 64-subset table is reproducible; the best unlandable body is the
all-six body at **7020 B / 48.533227 %**.

## The 7072-B ceiling

Every attempt to restore the missing 52 B on the all-six body was measured.
The best 7072-B results are all **≤ 48.337647** (the HEAD score):

| variant (all-six body + …) | size | score |
|---|---:|---:|
| 6 × `+8` wraps (`scrollBarInset,0x150,0x168,0x1a8,0x204,0x2d0`) + `0x138` `+4` | 7072 | 48.318408 |
| 5 × `+8` + `0x180` `+12` | 7072 | 48.297830 |
| 2 × `+8` + 3 × `+12` | 7072 | 48.297830 |
| `+44` (`0x198`) + `+8` | 7072 | 48.140044 |
| `+16` (`0x26c`) + `+36` (`0x18c`) | 7068 | 46.890523 |
| `+12` (`0x180`) + `+40` (`0x1fc`) | 7068 | 46.958250 |
| single `+52` (`field_0x2c4`) | 7072 | 47.982508 |
| single `+52` (`ringMidFarThreshold`) | 7072 | 43.241091 |

Single `(int)(float)(int)` wrap deltas on the all-six body (236 candidate
`this->X = <ternary>` lines): `+4` fields are `0x138, marqueeWidth, 0xcc,
0xc0, 0x27c, 0x68`; `+8` fields are `scrollBarInset, rewardBoxWidth, 0x6c,
0x2d0, 0x204, 0x1f4, 0x1a8, 0x168, 0x150, 0x148, helpWindowY, 0x8c, 0x2c8,
0x1d8, 0xb8, 0xc_leftMargin`; `+12` `0x180,0x1a4,0x194,0x2d8,0x30_rowHeight,
oreTextYOffset,0xa4,hudInset,0xf0`; `+16` `0x26c,0x5c,0x20_top`; `+20`
`0x1e4`; `+24` `0x248,0x130`; `+36` `0x18c,0xdc`; `+40` `0x1fc,0x250`; `+44`
`0x198,0x2cc,0x128,0x234`; `+52` `0x2c4,ringMidFarThreshold`. Many wraps fold
to delta 0 (`(int)(float)(int)` of a single-use int is a no-op at `-Oz`).

Also measured (all worse):

* `int u9c` → float ternary on the base body: 7096 B / 48.092210.
* all-six + `u9c` float ternary: 7044 B / 48.416019; + wraps to 7072:
  48.269001 best.
* `field_0x1e4` as `(int)(iPadHD ? 5.625f : …)`: all-six + `1e4` = 7056 B /
  44.688061 (adds +36 B but costs ~4 pp).
* `field_0x1e8` as `(retinaDisplay || iPadHD) ? 1 : Globals::n9` (the original's
  `orr/clz/lsr` shape): base + `1e8` = 7056 B / 46.693227.
* shared-int temp `int vB = (int)(iPadHD ? 5.625f : (n9 ? 6 : (retina ? 8 : 4)))`
  feeding `0x150 = (int)(float)vB`, `0x1e4 = vB`, `0x1d8 = vB`, `0x210 = vB`:
  6976 B / 40.016038 (live-range cost dominates).

## Structural evidence found (for the next pass)

1. **HD-block SLP groups are 16-byte aligned in the original.** The original's
   HD block groups are `[0x140-0x14f]`, `[0x150-0x15f]`, `[0x160-0x16f]`,
   `[0x170-0x17f]`, `[0x180-0x18f]`, `[0x190-0x19f]`, `[0x1a0-0x1af]`,
   `[0x1b0-0x1bf]`, `[0x1c0-0x1cf]`, `[0x1d0-0x1df]` (each a `vst1.32
   {dN-dM}` = 16 B). Ours are `[0x148-0x157]`, `[0x158-0x167]`,
   `[0x168-0x177]`, then `[0x180-…]` — offset by 8 B. The cause is visible in
   the source: `field_0x140` and `field_0x144` are at lines 311/302, far from
   `field_0x148` (line 141). Moving them next to `0x148` does align the groups
   (`0x140,0x150,0x160,0x17c,0x18c,…`) but the non-HD code shifts with it:
   base + move `0x140,0x144` before `0x148` = 7040 B / 45.957277; ordering
   `0x140..0x17c` ascending = 7056 B / 45.405621. The same 4-byte offset
   appears at `0xb0/0xb4/0xc0/0xc4` and `0xdc/0xec` (original groups `0xc0`,
   `0xb0`, `0xec`; ours `0xb4`, `0xc4`, `0xdc`).
2. **The original's HD block stores `0x94,0x98,0x9c,0xa0` as one `stmia`
   group** (`add.w r4,r0,#148; stmia.w r4,{r5,r8,ip,lr}`). Ours stores them as
   separate `str.w`, because `field_0x94` is at line 298. Moving it before
   `0x98` gives 7016 B / 45.68 — the size/score cost elsewhere dominates.
3. **The original's common tail is 178 B longer than ours** (original
   `0xd21e2..0xd2bc8` = 2534 B; all-six body `0xd2260..0xd2b94` = 2356 B),
   while our non-HD block is 114 B longer (4072 vs 3958 B). The original's
   tail uses SLP conditional selects (`vmov.i32 qN,#31`, `vshl.s32`, `vbsl`,
   `vdup.16`, `vst1.32 {dN[0]}`) for the `0x204..0x223`, `0x234..0x247`,
   `0x24c..0x273` regions; ours uses scalar `it eq; moveq; str`. Reproducing
   those groups needs the original's exact per-field source adjacency in that
   region, which the current hill-climbed order does not have.
4. **Shared-int groups (from the task hint) confirmed by the store shapes.**
   `0x70/0x5c` are the same expression; `0xa8/0xac` share one int
   (`vmov s0,r2; str r2,[r0,#172]; vcvt.f32.s32; vcvt.s32.f32;
   vstr s0,[r0,#168]`); `0x150/0x1e4/0x1d8/0x210` all evaluate to
   `{5,6,8,4}`; `0x3e0/0x10c/0x124/0x128/0x1bc/0x230/0x234/0x250` all
   evaluate to `{1,2,2,1}`. The original's int→float→int round trips
   (`vmov sN,rM; vcvt.f32.s32; vcvt.s32.f32; vstr`) are on
   `0x60, 0x68, 0x6c, 0x70, 0x2c8, 0xa8, 0x158, 0x15c, 0x160, 0x170, 0x3f8`.
   Our source has the round trip on `0x6c, 0x70, 0x2c8, 0xa8, 0x3e0, 0x150,
   0x138, 0x10c, 0x194, 0x204, 0x1f4` and is missing it on
   `0x60, 0x68, 0x158, 0x15c, 0x160, 0x170, 0x3f8`. Wrapping those folds at
   `-Oz` (delta 0) unless the int has a second use, which is exactly the
   shared-int-temp hypothesis; a global temp for the `{5,6,8,4}` group was
   measured at 6976 B / 40.0 (too much live range).
5. **`0x1e4` is a float ternary in the original** (`vldr s0, 5.625f` on the HD
   path, `vmov.f32 s0,#24` (6.0f) on the non-HD path, then a single
   `vcvt.s32.f32` + `vstr s0,[r0,#484]`), not an int ternary. `0x1e8` is
   `(retinaDisplay || iPadHD) ? 1 : Globals::n9` (`orr; clz; lsr #5;
   cmp; mov #1; it ne; movne r1,r8`). Both edits are faithful and were
   measured (see above) but cost more than they gain on their own.

## Proven vs hypothesis

**Proven (binary / measured):**

* The six "fixes" are faithful (HD literal pool vectors at `0xd13f0` /
  `0xd1400` / `0xd1410` carry `191`, `63`, `66`, `1`), and the all-six body is
  the best fuzzy body measured at **7020 B / 48.533227 %**.
* No 7072-B variant above **48.337647** was found: 64 fix subsets, all single
  wraps on the all-six body, the `+4/+8/+12/+16/+20/+24/+28/+36/+40/+44/+52`
  delta combinations, targeted structural moves (`0x94`, `0x140/0x144`,
  `0x140..0x17c` ascending, `0x204`), the `u9c` float ternary, `0x1e4`,
  `0x1e8`, and the shared-int `{5,6,8,4}` temp. All are real `link_so` builds.
* The original's HD SLP groups are 16-byte aligned at `0x140/0x150/0x160/0x170`
  and `0xc0/0xb0/0xec`; ours are offset by 4–8 B because of source placement
  of `0x140/0x144` and the `0xb0..0xc4` block.
* The original's common tail is 178 B longer than ours and uses SLP
  `vbsl`/`vshl`/`vdup` conditional selects; ours is scalar.

**Hypothesis (not proven):**

* The remaining ~51.7 % mismatch is dominated by the original's SLP grouping
  in the non-HD/tail regions. Matching it requires the original's exact
  per-field source adjacency, not just field order: the current source is a
  hill-climbed local optimum and every reorder tried (ascending, HD-group
  order, `0x140/0x144` move, `0x204` move) lowers the score.
* The 52 B missing from the all-six body is the SLP/`vbsl` code in the tail
  (`0x204..0x273` region), not a missing assignment.

## Reproduction / scratch

All scratch is under `/dev/shm/w5901`: `fscore.py` (unrounded scorer),
`Layout.base.cpp` (pristine HEAD body), `fixes.cpp` (all-six body),
`mkfix.py` (fix subsets + wraps), `mk5.py` (u9c/1e4/1e8 + wraps),
`mk6.py` (statement moves), `mk7.py` (statement reorder), `mk8.py` (shared-int
temps), `scan_fixes_wrap.py` (all 188 single wraps on the all-six body),
`scan_subsets.py` (all 64 fix subsets), `scan_subsets.json`,
`scan_fixes_wrap.json`, `orig_full.asm` (original Thumb disasm),
`un.txt`/`on.txt` (verify-normalised instruction streams).
