# Layout::Layout C1/C2 (`_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev`) — wave 117 / worker w6502

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (identical body), original size
**7072 B**, source `src/game/ui/Layout.cpp`. Branch `wave117-w6502`, base
`c22678d64` (wave-116 harvest). Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.

## Result — no landable improvement; note-only commit

| metric | before (HEAD) | after |
|---|---:|---:|
| `_ZN6LayoutC2Ev` size | 7072 | **7072** (unchanged) |
| `_ZN6LayoutC2Ev` unrounded `difflib` ratio | 48.554913294798 % | **48.554913294798 %** |
| `byte_exact` / `linked_exact` | 4501 / 4501 | **4501 / 4501** |
| `size_exact` / `data_exact` | 4524 / 843 | **4524 / 843** |
| imports / lint / drift | 207 / CLEAN / 0 | **207 / CLEAN / 0** |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
No code change landed: 27 section headers, file size **2,187,328 B**,
`.text` `0x1337c8`, `.rodata` `0x41c27`, `.ARM.extab` `0x26294` (4 B short) —
all unchanged from the baseline build.

## Independent angle: per-field store-kind audit from the original pool

Decoded every `vstr sN, [r0, #off]` in the original and in HEAD. A `vstr`
means the field value flows through a float (`(int)(float)(int_expr)` round
trip, `vmov s0,rN; vcvt.f32.s32; vcvt.s32.f32; vstr`); a `str.w`/`strd`/
`vst1.32` means a plain int store.

| field | original store | HEAD store | faithful source shape |
|---|---|---|---|
| 0x60,0x68,0x6c,0x70 | vstr | vstr | ok |
| 0x8c | vstr | vstr | ok |
| 0x9c | vstr (`70.0f`/`100.0f` arms) | vstr only with the **float-arm** form | `(int)(float)(HD ? 70.0f : …)` (w6402 `u9c`) |
| 0xa8 | vstr | vstr | ok |
| 0xdc,0xe0,0xe4,0xe8 | vstr/strd | vstr (one `vst1` group) | ok value-wise |
| 0x138 | vstr | vstr | ok |
| **0x150** | vstr | vstr | ok |
| **0x158** | vstr | **int** | `(int)(float)(int_expr)` — see below |
| **0x15c** | vstr | **int** | `(int)(float)(int_expr)` |
| 0x168 | vstr | vstr | ok |
| **0x170** | vstr | **int** | `(int)(float)(int_expr)` |
| 0x1a8 | vstr | vstr | ok |
| **0x1e4** | vstr | **int** | `(int)(float)(int_expr)` |
| **0x3f8** (`rewardBoxY2`) | vstr | **int** | `(int)(float)((rewardBoxY + 17) << retina)` |
| **0x20** (`field_0x20_top`) | int | **vstr** | int-typed ternary |
| **0x10c** | int | **vstr** | int-typed ternary |
| **0x148** | int | **vstr** | int-typed ternary |
| **0x1f4** | int | **vstr** | int-typed ternary |
| **0x204** | int | **vstr** | int-typed ternary |
| **0x3e8** (`rewardBoxWidth`) | int | **vstr** | int-typed ternary |

So the original has 5 float round trips HEAD is missing (0x158, 0x15c, 0x170,
0x1e4, 0x3f8) and 6 extra round trips HEAD has that the original does not
(0x20, 0x10c, 0x148, 0x1f4, 0x204, 0x3e8). This is a real, previously
unrecorded typing divergence (w6401's table only covered 0x9c/0xac/0x194/
0x3e0/0x3e8).

**Why it cannot be fixed from source with clang 7.0.2 -Oz:** clang folds
`(int)(float)x` to `x` whenever it can prove the range of `x`. All the
candidate values are small constants (13…206), so every form tried
(`(int)(float)(ternary)`, `int t = ternary; f = (int)(float)t;`,
`float t = ternary; f = (int)t;`, a full `if/else` chain into a local int)
compiles to **byte-identical code** to the plain int store. The original's
round trips survive because its value is a non-foldable phi; reproducing them
needs the original statement order, not a cast. Measured examples (all
`usize=7072`, ratio exactly `48.554913294798`):

```
field_0x158 = (int)(float)(ternary)              7072  48.554913
int v158 = ternary; field_0x158 = (int)(float)v158; 7072  48.554913
float v158 = ternary; field_0x158 = (int)v158;      7072  48.554913
if/else chain into int v158; field = (int)(float)v158; 7072  48.554913
```

## Measurements (all real `link_so` builds, unrounded `verify.py` ratio)

HEAD baseline = **7072 B / 48.554913294798 %**. The 7072-B wall is mandatory:
any size delta relocates every later function and its literal pool, so only
exactly 7072 B passes `byte_exact` (7076 → 2454, 7068 → 4458, 7064 → 3221).

| variant | size | % |
|---|---:|---:|
| HEAD | **7072** | **48.554913** |
| faithful typing `u9c` + `nheight` (`four+nheight`) | 7068 | **48.704663** |
| same + `rewardBoxWidth` local-int temp | 7076 | **48.725100** |
| same + `field_0x1f4` local-int temp | 7076 | **48.725100** |
| `u9c` + `nheight` + keep `rt48` | 7076 | 48.685259 |
| faithful typing `u9c` (`four`) | 7076 | 48.685259 |
| `four` + remove `int v` temp (0xa8) | 7068 | 48.624950 |
| `u9c` + `nheight` + `field_0x184`→ternary | 7068 | 48.664807 |
| `u9c` + `nheight` + `field_0x1cc`→ternary | 7068 | 48.624950 |
| HEAD + `nheight` | 7064 | 48.574277 |
| `four` + `1 * 1.40625f`→`1.40625f` | **7072** | 48.306098 |
| HEAD + `field_0x1c8` HD-arm fold | **7072** | 48.425668 |
| HEAD + `(retina+n9)!=0` (wave-113 form) | 7072 | 48.475184 |
| HEAD + append `this->fading = 0;` etc. | **7072** | 48.216776 |
| HEAD + append `this->drawColor = -1;` | 7076 | 48.565737 |
| wave-89 all-ternary body | 6800 | **50.788516** |

## Searches run this wave (all real builds, all 7072 hits listed)

* **312 single-field `(int)(float)(RHS)` wraps on HEAD+`nheight` (7064)**:
  2 hits at 7072 — `marqueeWidth` 48.416019, `field_0xdc` 44.276329.
* **106 ternary→if/else + `Globals::X != 0` + `(int)(N*1.40625f)` edits on
  `four+nheight` (7068)**: 1 hit at 7072 — `field_0x1c8` 48.425668.
* **23 appended end-of-ctor statements on `four+nheight`**: 17 land at 7072
  but all score 48.216776 (the extra store shifts the pool).
* **185 float-literal→double-literal edits on `four+nheight`**: no 7072 hit
  (sizes 7052/7068/7076/7080/…).
* **269 local-int-temp forms on `four+nheight`**: no 7072 hit above 48.5549;
  best 7076 (48.725100, `rewardBoxWidth`/`field_0x1f4`).
* **176 `-4` simplification edits on `u9c+nheight+rt48` (7076)**: no 7072 hit;
  best 7060 (48.724083).
* **602 wraps/temps/folds on `four`−`v` (7068)**: no 7072 hit above 48.5549.
* **600 random 1–3 edit subsets of a 383-edit pool on `four+nheight`**: no
  7072 hit above 48.5549.
* **13 if/else→ternary conversions measured individually on the wave-89 body**
  (deltas): `0x2d4` +56, `0x130` +0, `0x184` +16, `0x190` −24, `0x1cc` +0,
  `0x20c` +0, `orbitInfoTextX` +0, `0x218` −40, `orbitInfoStationY` +0,
  `0x260` +16, `0x280` +0, `rewardBoxHeight` +16, `rewardBoxY2` −80. No
  subset reaches +272 (7072) without the full HEAD body.
* **64 subsets of `{u9c, rm_v, nheight, rm_rt48, retina+n9, fold1}` on HEAD**:
  6 hits at 7072, best is the empty set (HEAD, 48.554913).
* Reordering `field_0x2c0/0x2c4/0x2c8/0x2cc/0x2d0` into address order (the
  original n9 branch stores them as one `stmia` group at 0x2c0): 7040 B,
  44.999002 %.

## Per-field pool audit (the task's "distinct vs shared" angle)

The original's end pool (`0xd2854..0xd2bc8`, 221 words) starts with
`0.325f, 0.65f, 100.0f, 50.0f` (fields 0x90/0x9c), then `281.25f, 400.0f,
200.0f, 150.0f`, then the int run `4,22,25,12,11,11,10,270,60,120,300,600,
360,146,8,6,198,180,189,54,…`. HEAD's end pool (`0xd28d8..0xd2bc8`, 188
words) starts with pointer words, then `281.25f,400.0f,200.0f,150.0f`, then
`4,22,25,12,11,11,10,270,360,146,8,6,198,180,189,54,…` — i.e. it is missing
the leading `0.325/0.65/100/50` and the `60,120,300,600` run, and its first
constant use order differs. All per-field **values** agree in all 6 flag
combinations (re-confirmed); the difference is purely which pool region and
which SLP window each constant lands in. Note the original's first four end
pool words are not referenced by any pc-relative load in the function (dead
pool data), so they cannot be reproduced from source.

## Proven vs hypothesis

**Proven (measured):**

* The 7072-B wall is real and content-based; the current HEAD body is the best
  7072-B body found across ~2600 built variants this wave (and ~2500 in
  waves 115–116).
* The faithful typing body is 4 B too long (`four` 7076) or 4 B too short
  (`four+nheight` 7068); no single cast/fold/temp/append/swap edit moves it by
  exactly +4/−4 without dropping the score below 48.5549.
* The original's float round trips on 0x158/0x15c/0x170/0x1e4/0x3f8 and its
  plain-int stores on 0x20/0x10c/0x148/0x1f4/0x204/0x3e8 are a genuine
  source-shape divergence, but clang 7.0.2 folds every source form that
  expresses them, so they cannot be reproduced by a cast alone.

**Hypothesis (not proven):**

* A 7072-B body at ~48.70 % exists and needs a multi-statement restructure
  that changes the n9/retina branch layout (so the original's `stmia` group
  at 0x2c0 and its end-pool order appear) while keeping the total code+pool
  sum at exactly 7072. Every reorder tried so far loses more alignment than it
  gains.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6502
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6     # RATCHET PASS
```

Scratch harness (not committed): `/dev/shm/w6502/{score,try,vecmap,hdmap2,
pooldump,search_wrap,search_edits,search_append,search_arms,search_dbl,
search_temp2,search_minus4,search_rmv,randsearch,combo,convsearch,uy_forms,
e158,reorder_2c}.py`, plus `Layout.cpp.base`, `Layout_four.cpp`,
`Layout_four_nh.cpp`, `Layout_uy.cpp`, `Layout_7076.cpp`, `Layout_rmv.cpp`.
