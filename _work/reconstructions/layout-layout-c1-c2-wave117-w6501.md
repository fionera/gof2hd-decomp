# Layout::Layout C1/C2 (`_ZN6LayoutC1Ev`) — wave 117 / worker w6501

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (identical bodies), original size
**7072 B**, source `src/game/ui/Layout.cpp`. Branch `wave117-w6501`, base
`c22678d64` (wave-116 harvest).

## Result — no landable improvement; note-only commit

| metric | before (HEAD) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| unrounded `difflib` ratio | 48.554913294798 % | **48.554913294798 %** |
| `byte_exact` / `linked_exact` | 4501 / 4501 | **4501 / 4501** |
| `size_exact` / `data_exact` | 4524 / 843 | **4524 / 843** |
| imports / lint / drift | 207 / CLEAN / 0 | **207 / CLEAN / 0** |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
No source change landed; 27 section headers, file size 2,187,328 B unchanged.

## The 7072-B wall — re-verified this wave with the full `check` target

A 7076-B ctor really does collapse `byte_exact`:

| ctor size | `byte_exact` | verdict |
|---:|---:|---|
| 7072 | 4501 | PASS |
| 7076 | **2454** | RATCHET FAIL (measured again this wave) |

Every later function shifts by 4 B and its literal-pool words move, so the
ctor must stay *exactly* 7072 B.  The destructor and every other Layout
function is byte-exact, so compensating the +4 in another TU would also cost
a `byte_exact` (the ratchet forbids any decrease).

## Best measured variants this wave (all real `link_so` builds, unrounded ratio)

| variant | size | score % |
|---|---:|---:|
| HEAD (baseline) | 7072 | 48.554913 |
| **`rewardBoxWidth` RHS wrapped `(int)(float)(int)` on the 7068 body** | **7076** | **48.725100** |
| `field_0x1f4` RHS wrapped `(int)(float)(int)` on the 7068 body | 7076 | 48.725100 |
| `field_0x2d8` RHS wrapped `(int)(float)` on the 7068 body | 7076 | 48.694959 |
| 7068 body (`0x9c` float ternary + int-folded `0x2c8` init) | 7068 | 48.704663 |
| `0x150` cast dropped on the `0x9c`-float body | 7068 | 48.674507 |
| `0x138` cast dropped on the `0x9c`-float body | 7068 | 48.664807 |
| `0x160` cast dropped on the `0x9c`-float body | 7076 | 48.645418 |
| `0x9c` float-literal ternary on HEAD (`u9c`) | 7084 | 48.665870 |
| `u9c` + `0x2c8` assignment cast dropped (`v_7076`) | 7076 | 48.685259 |
| `u9c` + `rewardBoxHeight` ternary (`v_nh`) | 7076 | 48.685259 |

So the best *non-landable* body is **48.725100 % at 7076 B** — only 4 B over
the wall.  Every 7072-B body found this wave scores **≤ 48.554913 %**.

## Faithful-typing finding: `field_0x2c8` round trips

The original stores `0x2c8` with **one** `vmov s0,rN; vcvt.f32.s32;
vcvt.s32.f32; vstr` round trip (`d1814..d1820`).  HEAD's

```cpp
int t_field_0x2c8 = Globals::iPadHD ? 45 * 1.40625f : ...;   // int <- float
this->field_0x2c8 = (int)(float)t_field_0x2c8;               // int -> float -> int
```

performs **two** round trips (`d1878..d1884` in our build).  Both faithful
single-round-trip spellings were measured:

* `int t = <int-folded expr>; this->f = (int)(float)t;` → 7068 B (48.704663)
* `int t = <float expr>; this->f = t;`                  → 7076 B (48.685259)

Neither is 7072 B, and no −4 B edit on either body was found that keeps the
score (see below).

## Searches run this wave (all real `link_so` builds)

* 120 random 1–2 statement moves of the HEAD ctor → best 7072 hit = baseline.
* 400 random 1-statement moves of the `u9c` body (7084) → no 7072 hit.
* 300 random 1-statement moves of the 7068 body → no 7072 hit.
* 250 random 1-statement moves of the `v_7076` body → no 7072 hit.
* all 238 single-line int assignments wrapped `(int)(float)` on `u9c`:
  deltas are 0, −4, −8, −12, −16, −20, −32, +8, +12, +16, +32 — **no +4**.
  Only `field_0x1bc` and `field_0x1b0` land on 7072 (47.657 / 47.896).
* all 238 single-line int assignments wrapped `(int)(float)` on the 7076 body:
  only `field_0x2c_rowHeight` and `eventBannerScale` land on 7072
  (48.0175 / 47.9474) — the −4 B edit costs more score than it gains.
* all 238 single-line int assignments wrapped `(int)(float)(int)` or `(float)`
  on the 7068 body: 7072 hits are `field_0x1d8` (48.4257) and `field_0x27c`
  (43.43).
* 112 cross-moves of the `0x2c0/0x2c4/0x2c8/0x2cc/0x2d0/0x2d4/0x2d8/0x1e0`
  block: 11 land on 7072, **all exactly 48.554913** (size- and score-neutral).
* 168 cross-moves of the `0xb0/0xb4/0xb8/0xbc/0xc0/0xc4/0xc8/0xcc/0xd0/0xd8`
  block: 19 land on 7072, best 48.554913.
* 160 placements of `eventBannerScale` (0x1e0) anywhere in the ctor: only two
  7072 hits, both ≤ baseline.
* 46 two-edit combinations from a 24-edit pool (cast add/drop, `0x9c` float,
  `rewardBoxHeight` ternary, `0x2d4` else-merge, `rd` constness, `0x1e8`
  `||`-form): best 7072 = baseline.
* 8 expression rewrites of `0x1e8`, `0x1f8`, `0x164`, `0x8c`, `0x90`, `0x1f4`,
  `0x2d8`, `0x1e4` on the 7076 body: none reach 7072 at a better score
  (`0x1e4` float form 7044, `0x1e4` cast 7036, `0x1f8` `&&` form 7100).
* `volatile`/`const`/`float`/`long`/`short`/`unsigned` spellings of the
  `0x2c8` temporary: `volatile` collapses to 33.4 %, the rest are neutral.
* `0x2d4` if/else chain merged into `else this->field_0x2d4 = Globals::iPad ?
  100 : 60;` → 7080 (48.127); `0x190`/`0x184` chain merges → 7052 / 7080.

## Why the remaining gap cannot be closed from the source

The original body is **6188 B of code + 221 pool words**; HEAD is **6290 B +
191 words**.  The 30-word pool deficit is SLP grouping, not wrong constants:
the original's non-HD branches emit `vst1.32` groups at `0x140, 0x150, 0x160,
0x170, 0x180, 0x190, 0x1a0, 0x1b0, 0x1c0, 0x1d0`, while HEAD groups
`0x148, 0x158, 0x168, 0x178, 0x188, 0x198` because `field_0x140`/`field_0x144`
are assigned near the end of the ctor.  Moving them next to `0x148` produces
the right groups but changes the ctor to 7008 B (−64), and every −64/+64
compensation tried changes the score by more than it recovers.

The original also has four outlined cold blocks after the `pop`
(`d2802..d2852`, 80 B) where HEAD has one (26 B); they are the iPad/retina
tails of the `0x130/0x134/0x138` chain and cannot be reproduced without
reordering that chain, which changes the size.

The only remaining 4 B on the best (48.7251 %) body would have to come from a
source shape that removes exactly one 32-bit instruction **and** keeps the
literal-pool alignment; every cast/fold/swap/temporary tried changes the size
by 0, ±8, ±12 or more, never ±4.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6501
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6     # RATCHET PASS
```

Scratch harnesses (not committed): `/dev/shm/w6501/{try,score,diffblocks,sim,
mv,batch,batch2..batch12,pairsearch,pair2,randsearch,randsearch2..4,
reorder_by_store}.py`, `Layout.base.cpp`, `v_u9c.cpp`, `v_7076.cpp`,
`v_7068.cpp`, `v_7076b.cpp`, `v_nh.cpp`.
