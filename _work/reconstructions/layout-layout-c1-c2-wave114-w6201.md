# Layout::Layout C1/C2 — wave 114 (w6201): code/pool split, faithful
# int-typing, and the measured 7072-B ceiling

Worker **w6201**, branch `wave114-w6201`, base `d8557f7e2` (wave-113 harvest).
Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (same body and score),
ELF VMA `0xd1028`, original size **7072 B**.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.

## Result — no landable improvement; the wave-113 body is the 7072-B optimum

| metric | before (HEAD) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| `_ZN6LayoutC1Ev` unrounded linked `difflib` ratio | 48.545237146273 % | **48.545237146273 %** |
| `_ZN6LayoutC2Ev` | 48.545237146273 % | **48.545237146273 %** |
| normalised insns (orig/ours) | 2529 / 2489 | 2529 / 2489 |
| `byte_exact` / `linked_exact` | 4500 / 4500 | **4500 / 4500** |
| `size_exact` / `data_exact` | 4524 / 843 | **4524 / 843** |
| imports / lint / drift | 207 / CLEAN / 0 | **207 / CLEAN / 0** |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
27 section headers, file size 2,187,328 B, `.ARM.extab` `0x26298` — unchanged
(no code change was landed).

**This pass is a note-only commit.** Every variant below was built for real
(`link_so` + `tools/verify` scorer) and measured; none beat 48.5452 % at the
mandatory 7072 B.

## New measurement: the code / literal-pool split (this is the key number)

The ctor ends with an epilogue (`pop {r4,r5,r6,r7,pc}`) followed by a single
ADR-addressed literal pool.  The pool is *not* detected by `verify.py`'s
`PCREL_LOAD` sweep (it is addressed with `add rX, pc, #N` + `vld1.64`), so its
words are decoded and compared as Thumb instructions.  Measured split:

| body | code bytes | pool bytes | pool words | total |
|---|---:|---:|---:|---:|
| **original** | **6106** (`0x17da`) | **966** | **241** | 7072 |
| wave-113 HEAD | 6290 (`0x1892`) | 782 | 195 | 7072 |
| wave-89 all-ternary (`4ce92d572`) | 6092 (`0x17cc`) | 708 | 177 | 6800 |

The original's **code is 184 B shorter** than ours and its **pool is 184 B
longer** (46 extra words).  So the "35 missing instructions" is not missing
code — our body already carries *more* code instructions than the original —
it is 46 missing **literal-pool words**: the original materialises more
constants through `vld1.64` vector groups and fewer through `movw`/`movs`.

Instruction-class counts (whole function, pool words included):

| class | original | HEAD | wave-89 |
|---|---:|---:|---:|
| `vld1.64` | 86 | 77 | 72 |
| `vst1.32` | 81 | 70 | 70 |
| `strd` | 62 | 49 | 54 |
| `stmia` | 8 | 4 | 4 |
| `str.w` | 103 | **166** | **169** |
| `movw` | 15 | 17 | 18 |

The 63 excess scalar `str.w` are fields that are not memory-adjacent in our
source order, so SLP cannot fold them into `strd`/`stmia`/`vst1.32` groups and
their constants never reach the pool.

## The wave-89 all-ternary body is still the score ceiling (unlandable)

Rebuilt from `git show 4ce92d572:src/game/ui/Layout.cpp` (one
`reinterpret_cast<unsigned long>` had to be updated to `unsigned int &` for the
current `Globals::font` type):

| body | size | % |
|---|---:|---:|
| wave-113 HEAD | 7072 | 48.5452 |
| wave-89 all-ternary | 6800 | **50.7885** |
| wave-89 + `0x1cc` if/else chain | 6800 | **50.8694** (best score seen) |

The `0x1cc` if/else conversion is size-neutral and is the only individual
if/else conversion that *raises* the score on the wave-89 body.  Everything
else in the wave-113 body's extra 272 B lowers it — the score is monotonically
inverse to size on this family (6800 → 50.79, 6944 → 49.97, 7056 → 48.57,
7064 → 48.56, 7072 → 48.55).  The extra 272 B is *not* matching the original.

## Faithful int-typing: two real (but size-changing) improvements

The original stores several fields as plain `int` where our source makes the
ternary `float`-typed (because the HD arm is `N * 1.40625f`), forcing a
`vmov/vcvt.f32.s32/vcvt.s32.f32/vstr` round trip in the non-HD branch.
Two of these are provably wrong in our source:

* `field_0x194` (`0x194`) — original `str.w` plain int; ours round-trips.
  Removing the `(int)(float)` cast: **7064 B / 48.554913 %**.
* `scrollBarInset` (`0x3e8`) — original `str.w` plain int; ours round-trips.
  Making the HD arm `(int)(1 * 1.40625f)` (int-typed ternary):
  **7064 B / 48.564593 %**.
* both together: **7056 B / 48.574277 %**.

`field_0x10c` and `field_0x150` also round-trip in ours; making them int-typed
gives 7064 B / 48.2264 % and 7064 B / 48.5150 % (both worse).

These are real source-shape fixes, but each removes 8 B, and **any size change
collapses the whole-`.so` gate**: a 196-B shrink (`field_0x20_top` int-typed)
drops `byte_exact 4500 -> 2582` (`RATCHET FAIL`), because every function after
`Layout` shifts and its literal-pool address deltas change.  The ctor must stay
byte-for-byte 7072 B.

## Searches that found nothing better at 7072 B

All measured with real `link_so` builds:

* **512-combination sweep** of the 9 "revert to wave-89 form" edits
  (`u9c`, `a8`, `0x2d4`, `0x184`, `0x190`, `0x260`, `rewardBoxHeight`,
  `rewardBoxY2`, `0x130/0x134/0x138`) on the HEAD body: the only 7072-B
  combinations found are `()` (baseline), `('e184',)` 48.5054,
  `('p130',)` 48.5452 (code-identical), `('u9c','d2d4')` 48.2662,
  `('e184','p130')` 48.5054, `('f190','oy2')` 47.7309 — all ≤ baseline.
* **All 87 `(int)(N * 1.40625f)` int-typing edits** on the HEAD body: 60 are
  code-identical (48.5452); the rest change size and none improves at 7072.
* **All 87 int-typing edits on the `field_0x194` variant** (7064 B): no 7072-B
  hit at all.
* **All 206 adjacent swaps** of the `scrollBarInset` variant (7064 B): 11 land
  on 7072 B, best 48.5452 (tie), e.g. swap 96.
* **All 206 adjacent swaps** of the `field_0x194` variant (7064 B): 10 land on
  7072 B, best 48.5356.
* **608-move random hill climb** from the `scrollBarInset` variant: no 7072-B
  improvement.
* **765-move random hill climb** from the 6944-B `g1cc+a8+ac+d2d4+e184+l260+
  nheight` body (49.9699 %): no 7072-B improvement.
* **807 random 1–4-unit moves** from the same 6944-B body: **zero** landed on
  7072 B — the size is a narrow coincidence of the HEAD body.
* **60 random single-unit moves** from the wave-89 + `0x1cc` body: sizes
  6752–6848, never 7072.
* Moving `field_0x94` next to `field_0x98` on the HEAD body: 7148 B / 25.39 %.
  On the wave-89 body: 6760 B / 48.90 %.
* Moving `eventBannerScale` to the end of the HD block: 7000 B / 44.74 %.
* Moving `field_0x2c0` next to `field_0x2c4`: 7028 B / 45.13 %.

## Proven vs hypothesis

**Proven (measured):**

* The original's code is 6106 B and its ADR literal pool is 966 B (241 words);
  ours is 6290 B / 782 B (195 words).  The missing content is pool words, not
  code instructions.
* The original has 63 fewer scalar `str.w` and 11 more `vst1.32` groups than
  the HEAD body — the field-adjacency / SLP story is real.
* The wave-89 all-ternary body scores 50.79 % at 6800 B; adding the `0x1cc`
  if/else chain raises it to 50.8694 % at the same size.
* `field_0x194` and `scrollBarInset` are int-typed in the original and
  float-typed in our source; fixing either is worth ≈ +0.01–0.02 pp but −8 B.
* The HEAD body is a strong local optimum at 7072 B: 512-combination sweep,
  ~2000 random moves, all adjacent swaps and every int-typing edit fail to beat
  48.5452 %.

**Hypothesis (not proven):**

* The remaining ~51.5 % mismatch is dominated by the 46 missing pool words
  (the original's extra `vld1.64` groups) and by register allocation
  (`stmdb sp!, {r6,r7,r8,r9,sl,fp}` vs ours `{r5,r6,r7,r8,r9,sl,fp}`).
  Closing it needs a source shape whose consecutive-field stores SLP-fold into
  the same 4-word groups as the original's, at exactly 7072 B.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6201
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6     # RATCHET PASS
```

Scratch harness (not committed): `/dev/shm/w6201/{fscore,stats,trans,rev,sweep,
castsearch,cast2,cast3,swapsearch,hill2,hill3}.py`, `w89.cpp`, `base.cpp`.
