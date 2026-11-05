# Layout::Layout (`_ZN6LayoutC2Ev` / `_ZN6LayoutC1Ev`) — wave 116 / worker w6402

Target: `_ZN6LayoutC2Ev` / `_ZN6LayoutC1Ev` (identical bodies), original size
**7072 B**, source `src/game/ui/Layout.cpp`.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
Branch `wave116-w6402`, base `d672d349f` (wave-115 harvest).

## Result — landed a real (small) improvement at exactly 7072 B

| metric | before (HEAD) | after |
|---|---:|---:|
| `_ZN6LayoutC2Ev` size | 7072 | **7072** (unchanged) |
| `_ZN6LayoutC2Ev` unrounded `difflib` ratio | 48.545237146273 % | **48.554913294798 %** |
| `byte_exact` / `linked_exact` | 4501 / 4501 | **4501 / 4501** |
| `size_exact` / `data_exact` | 4524 / 843 | **4524 / 843** |
| imports / lint / drift | 207 / CLEAN / 0 | **207 / CLEAN / 0** |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
27 sections, file size **2,187,328 B**, `.ARM.extab` `0x26294` (4 B short) and
`.rodata` size are **bit-for-bit unchanged from the baseline build** (verified by
rebuilding HEAD and diffing the section table).

## Landed source shape (4 edits)

All four are real C++ and were cross-checked against the original's store
instructions (see `sig.py`-style per-field store-kind trace):

1. `field_0x9c` — the original stores it with
   `vcvt.s32.f32` + `vstr` (a float ternary converted to int), *not* a
   pre-computed `int u9c` if/else. Replace the `int u9c;` declaration and the
   whole if/else with
   `this->field_0x9c = (int)(float)(Globals::iPadHD ? 70 : (Globals::n9 ? 75 : (Globals::retinaDisplay ? 100 : 50)));`
2. `field_0xac` — drop the `(int)(float)(int)` round trip; the original stores a
   plain integer (`str.w`) here.
3. `field_0x194` — drop the `(int)(float)` round trip; plain integer store.
4. `scrollBarInset` (`0x3e8`) — drop the `(int)(float)(int)` round trip.
5. `field_0x2c8` — the original stores it with `vstr` after `vcvt.s32.f32`
   (`d1820: vstr s0, [r0, #712]`), i.e. the value really flows through a float.
   Materialise the RHS in an `int` temporary and assign
   `this->field_0x2c8 = (int)(float)t_field_0x2c8;`.

Edits 1–4 alone are the "faithful typing set" (w6302): **7076 B / 48.685259 %**,
which is *not landable* (see size wall below). Edit 5 (`rt48`) is the +8 B
change that brings the faithful body back to exactly 7072 B; it costs
0.02 pp of score but keeps the faithful typing. The alternative `field_0x9c`
form without the float round trip (`(int)(float)(int ternary)`) plus `rt48`
scores the same 48.554913 % — both variants are byte-size-identical; the landed
one keeps the float-typed `field_0x9c` that the original demonstrably uses
(`vldr` of `75.0f` at `0xd1900`, `vcvt.s32.f32` at `0xd199e`, `vstr` at
`0xd19a8`).

## The 7072-B wall (measured again on this branch)

`byte_exact` is content-based, so any Layout size change relocates every later
function and its literal-pool words. Only **exactly 7072 B** passes:

| ctor size | `byte_exact` | verdict |
|---:|---:|---|
| 7072 | 4501 | PASS |
| 7076 | 2454 | RATCHET FAIL |
| 7068 | **4458** | RATCHET FAIL (new measurement this wave) |
| 7064 | 3221 | RATCHET FAIL |

So the faithful 7076-B body needs exactly a **−4 B** edit, and the 7068-B
`four+nheight` body needs exactly a **+4 B** edit. Neither was found.

## Measurements (all real `link_so` builds, unrounded `verify.py` ratio)

| variant | size | score % |
|---|---:|---:|
| HEAD (baseline) | 7072 | 48.545237 |
| **landed: faithful 4 edits + `(int)(float)` `0x9c` + `rt48`** | **7072** | **48.554913** |
| faithful 4 edits (`u9c` float ternary, `cac`, `i194`, `i3e8`) | 7076 | 48.685259 |
| faithful 4 + `rewardBoxHeight` ternary (`nheight`) | 7068 | **48.704663** |
| faithful 4 + `(int)(float)(int)` `0x9c` (`u9ci`) | 7064 | 48.574277 |
| faithful 4 + `u9ci` + `rt48` | 7072 | 48.554913 |
| faithful 4 + `nheight` + `field_0x1c8` fold | 7072 | 48.425668 |
| faithful 4 + `nheight` + `field_0x1f4` fold | 7052 | 48.713345 |
| faithful 4 + `nheight` + swap `field_0x94`/`field_0x1f4` | 7060 | 48.733799 |
| faithful 4 + `nheight` + swap `field_0x110`/`field_0x11c` | 7060 | 48.724083 |
| faithful 4 + `v` as `float` | 7060 | 48.644338 |
| faithful 4 + drop `volatile` on `rd` | 7068 | 48.634546 |
| faithful 4 + `field_0x1e4` HD arm `4 * 1.40625f` | 7040 | 46.159984 |

## Searches that found no better 7072-B body

Every build below was a real `link_so`; only 7072-B results are landable.

* **256 single-field `(int)(float)(RHS)` wraps** on `four+nheight` (every
  single-line `int` field assignment): 227 size-neutral (7068), 4 at 7076,
  none at 7072.
* **68 `iPadHD ? N * 1.40625f` → folded-int** rewrites on `four+nheight`
  (the "per-field constant" angle): exactly one lands on 7072
  (`field_0x1c8`, 48.425668); the rest are 7044/7052/7060/7068/7076/7080/7084.
* **422 adjacent statement swaps** on `four+nheight`: two land on 7072
  (`field_0x1ac`/`field_0x1bc` 45.356716, `field_0x1c4`/`field_0x1c8`
  48.425668); the best scores overall are at 7060
  (`field_0x94`/`field_0x1f4` 48.733799, `field_0x110`/`field_0x11c`
  48.724083).
* **60 `int t = RHS; this->f = (int)(float)t;` temporaries** on `four+u9ci`
  (7064): 16 land on 7072, best 48.554913 (`0x2c8`, `0x2d0`, `0x6c`,
  `rewardBoxWidth`, `0x1f4`).
* **768 `(float)` / `(double)` / `(int)` wraps** on `four+nheight`: no 7072 hit.
* **700 random 1–3 edit subsets** of a 24-edit pool on `four+nheight`: only
  `field_0x1c8` combinations hit 7072 (48.425668).
* **700 random 1–3 edit subsets** on `four`: best 7072 hit 48.425668.
* `field_0x9c` shape variants on `four`: `(int)(float ternary)` 7076,
  `(int)(float)(int ternary)` 7064, `static_cast<int>(float ternary)` 7076 —
  no 7072.
* `scrollBarInset` shape variants on `four`: `(int)(float ternary)` 7076,
  `(int)(int ternary)` 7060 — no 7072.
* `rd` (`volatile unsigned char *`) moved to the top of the ctor or just
  before `field_0x2d8`: size- and score-neutral (7068 / 48.704663).
* Full declaration-order sort of the first 20 statements (`0x4..0x58`): the
  store order then matches the original's `def`-leaf trace almost exactly, but
  the difflib score collapses to **24.925105 %** (7068 B) because the literal
  pool/register allocation shifts. Do not reorder.

## Proven vs hypothesis

**Proven (measured):**

* The original ctor is 6106 B code + 241 pool words (966 B); HEAD is
  6284–6290 B code + 195–198 pool words. The 46-word deficit is store-merging
  (SLP), not wrong constants: every one of the 202 field values matches the
  original in all 6 flag combinations (`emu2.py`).
* The original's per-field store kinds for `0x9c` (`vcvt.s32.f32`+`vstr`),
  `0xac` (`str.w`), `0x194` (`str.w`), `0x3e8` (`str.w`) and `0x2c8`
  (`vstr`) are reproduced by the landed source shape.
* The HD leaf store order already matches the original exactly (first 80
  stores identical in `order.py`); the HD block's low difflib score is entirely
  literal-pool offset / register-number noise.
* The non-HD leaf store order diverges after ~store 40 (`eventBannerScale`
  stored early, `0x2c4/0x2cc/0x2d0` before `0x2d8`, `0x1e0` early); sorting to
  declaration order fixes the order but destroys the score.

**Hypothesis (not proven):**

* A −4 B faithful edit on the 7076-B body exists but is not a cast, a fold, a
  swap or a temporary on any single field; it likely needs a multi-statement
  restructure that keeps the literal pool byte count at 198 while shrinking
  code by 4 B.
* The 46 missing pool words can only be recovered by reproducing the original's
  non-HD SLP groups; the original's pool keeps `12.0f`, `15.0f`, `1.5f`, `32.0f`
  as *integer* bit patterns because clang merges adjacent float stores into
  `strd`/`str.w` (`d1a52: strd r4, r2, [r0,#216]` = `0xd8=55`, `0xdc=15.0f`;
  `d1a58: strd r3, r1, [r0,#224]` = `0xe0=1.5f`, `0xe4=12.0f`). Our source
  keeps `driftSpeedDivisor`/`field_0xdc`/`animSpeedScale`/`ringRadiusScale`
  scattered (lines 87/92/95/101), so clang emits four separate `vstr`s. Moving
  them adjacent in field order was measured at 6928 B / 38.86 % — the SLP
  merge happens but the pool/register layout loses far more than it gains.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6402
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6     # RATCHET PASS
cmake --build cmake-build-match --target lint -j6      # CLEAN
cmake --build cmake-build-match --target drift -j6     # 0 fields
```

Scratch harness (not committed): `/dev/shm/w6402/{fscore,build_score,ed,dump,
pooldiff,sig,order,ordercmp,opcodes,reorder,wrapsearch,foldsearch,swapsearch,
search8,search_floatwrap,search_rt,poolsearch}.py`, `Layout.cpp.base`,
`Layout.cpp.faithful`, `Layout.cpp.final`.
