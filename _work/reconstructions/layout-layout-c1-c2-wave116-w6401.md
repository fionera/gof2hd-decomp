# Layout::Layout C1/C2 (`_ZN6LayoutC1Ev`) — wave 116 / worker w6401

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (identical body), original size
**7072 B**, source `src/game/ui/Layout.cpp`. Branch `wave116-w6401`, base
`d672d349f` (wave-115 harvest).

## Result — no landable improvement; note-only commit

| metric | before (HEAD) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| unrounded `difflib` ratio | 48.545237146273 % | **48.545237146273 %** |
| `byte_exact` / `linked_exact` | 4501 / 4501 | **4501 / 4501** |
| `size_exact` / `data_exact` | 4524 / 843 | **4524 / 843** |
| imports / lint / drift | 207 / CLEAN / 0 | **207 / CLEAN / 0** |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
No code change landed; 27 section headers, file size 2,187,328 B and
`.ARM.extab 0x26298` unchanged.

## Correction to the wave-115 code/pool accounting

The w6302 note (and the wave-116 task text) split the body at the `pop`
(`0xd2800`): "6106 code + 966 pool (241 words)".  That is **wrong**: the
original has **four cold blocks after the epilogue**, `0xd2802..0xd2852`
(80 B), which are code, not pool.  The real split is:

| body | code | end-pool start | pool bytes | pool words |
|---|---:|---:|---:|---:|
| **original** | **6188** (`0xd1028..0xd2854`) | `0xd2854` | **884** | **221** |
| HEAD | 6290 (`0xd1028..0xd28ba`) | `0xd28d0` | 764 | 191 |
| faithful typing set (below) | 6312 (`0xd1028..0xd28d0`) | `0xd28d0` | 764 | 191 |

So the real deficit is **30 pool words (120 B)**, not 46, and the original's
code is 124 B shorter than HEAD (not 184).  The original also has 4 cold
blocks (80 B, after the `pop`) where HEAD has 1 (26 B); the cold-block
difference is 20 of the "missing pool words" in the old accounting.

Original end-pool floats: `0.325, 0.65, 100, 50, 281.25, 400, 200, 150, 76`
(plus int words and `0x0013xxxx` pointers).  The mid-function pool
(`0xd1308..0xd156e`) holds `5.625f` (`4*1.40625f`, field_0x1e4 HD arm) and
`1.40625f`, which HEAD keeps in its end pool.

## Faithful typing audit (re-derived)

Checked every `(int)(float)`/`(int)(float)(int)` site against the original's
store instruction:

| field | original store | faithful form | Δ size vs HEAD |
|---|---|---|---:|
| `field_0x9c` (0x9c) | `vldr s0; vcvt.s32.f32; vstr` | float ternary `HD ? 70.0f : …` | **+24** |
| `field_0xac` (0xac) | plain `str.w` int | `(int)(…)` (drop double cast) | **0** |
| `field_0x194` (0x194) | plain `str.w` int | drop `(int)(float)` | **−8** |
| `scrollBarInset` (0x3e0) | `vmov s0,rN; vcvt.f32.s32; vcvt.s32.f32; vstr` | base `(int)(float)(int)(…)` — **no edit** | 0 |
| `rewardBoxWidth` (0x3e8) | plain `str.w` int | `HD ? (int)(100*1.40625f) : …` | −28 |

Note: w6302's `i3e8` edit was applied to `scrollBarInset`, but the original
stores `scrollBarInset` (0x3e0) through a **round trip**, so that edit is not
faithful.  The field actually at 0x3e8 is `rewardBoxWidth`, which is
int-typed.  w6302's four-edit set (`u9c+i194+i3e8+cac`) therefore mixes one
non-faithful edit; the strictly faithful set is `u9c+i194+cac` = **7084 B**
(+12).  With w6302's `i3e8` included it is **7076 B** (+4).

## Every 7072-B variant measured this wave (all real `link_so` builds)

| variant | size | unrounded % |
|---|---:|---:|
| HEAD (baseline) | **7072** | **48.545237** |
| HEAD + `cac` | 7072 | 48.395455 |
| HEAD + move field_0x2d8 after field_0x90 | 7072 | 48.416019 |
| HEAD + move field_0x2d8 after field_0x8c | 7072 | 48.416019 |
| HEAD + move field_0x2d8 after field_0x80_touchMargin | 7072 | 47.349542 |
| `u9c+i194+i3e8+novol` | 7072 | 48.406375 |
| `u9c+i194+i3e8+cac+novol` | 7068 | 48.634642 |
| `u9c+i194+i3e8+cac` (w6302 faithful) | 7076 | **48.685259** |
| `u9c+i194+i3e8+cac+nheight` | 7068 | **48.704663** |
| `u9c+i194+i3e8+cac` + field_0x2d4 if/else using `Globals::retinaDisplay` instead of `*rd` | 7068 | 48.634642 |
| `u9c+i194+i3e8+cac` + move field_0x2d8 after field_0x8c | 7068 | 48.594778 |
| `u9c+i194+cac` (strictly faithful) | 7084 | 48.466746 |
| `u9c+i194+cac` + field_0x1c8 int-typing + `nheight` | 7072 | 48.425668 |

Searches run this wave (all with the unrounded `verify.py` ratio):

* all 16 subsets of `{u9c, i194, i3e8, cac}` — only `()` and `cac` land on
  7072, both ≤ baseline;
* all ~70 `HD ? N * 1.40625f` → `(int)(N * 1.40625f)` int-typing edits on the
  faithful body — no 7072 hit (sizes 6912…7140);
* 170 `(int)(float)(…)` cast additions on the faithful body, on
  faithful+`nheight`, on the `i3e8`-only body and on the
  `retinaDisplay`-condition body — no 7072 hit above baseline (deltas are
  0, ±8, ±16, …; never +4);
* 1000 random single-statement moves of the faithful body — no 7072 hit above
  baseline;
* all 256 subsets of `{u9c, i194, i3e8, cac, nheight, novol, oy2cast,
  move2d8}` — best 7072 is the baseline;
* 17 placements of `field_0x2d8` in the constructor (before/after 0x70/0x74/
  0x78/0x2c4/eventBannerScale/0x2d0/0x2cc/0x2c8/`rd`/0x84/0x80/0x7c/0x88/
  0x8c/0x90/0x98) — no 7072 hit above baseline;
* 5 forms of the 0x2d4/0x2d8 pair (adjacent ternaries, shared `int`
  temporaries, shared `rd`, reordered) — best 7076 / 48.327;
* 4 forms of field_0x1e4's HD arm (`5`, `4*1.40625f`, `(int)(4*1.40625f)`,
  `5.625f`) — 7060/7076, none 7072;
* 6 forms of field_0x1e8 (`||` form, `iPad ? retina : 1`, …) — 6928…7076.

## Why 7072 B is mandatory

Measured again on this branch with the full `check` target: a 7076-B ctor
drops `byte_exact` 4501 → 2454 (every later function shifts and its literal
pool addresses change).  `linked_exact` survives but `byte_exact` is the hard
count.  So the ctor must stay exactly 7072 B.

## Conclusion

The HEAD body remains the best landable 7072-B variant (48.5452 %).  The
faithful typing set is a real +0.14 pp improvement but is +4 B, and the
missing 4-byte saving was not found: every +4/−4 edit tried changes the size
by 0 or ±8, and every 7072-B combination found scores below the baseline.
The remaining gap is the original's extra SLP grouping in the non-HD branch
(30 more pool words, 124 fewer code bytes) plus its four outlined cold
blocks; reproducing those needs the original statement order, which is not
the iOS order and not the HD-trace order (w6302 measured both).

Scratch harnesses (not committed): `/dev/shm/w6401/{score,align,variants,
inttype,addcast*,combo,move*,d2d4*,e1e4*,e1e8,final*}.py`.
