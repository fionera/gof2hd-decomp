# `Layout::Layout` C1/C2 — wave 183 / worker w12713

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave183-w12713`, base master `9555474d8` (post-wave-182 tree; the
wave-182 `w12612` permutation is landed, raw **5,385**).

**Result: no size-exact raw win found.  `src/` left at the base.**

| metric | base |
|---|---:|
| raw same-address bytes | **5,385** |
| aligned (difflib) | 2,562 |
| size | **7,072/7,072** |
| instructions | 2,518/2,552 |

The task asked for the `Globals::iPadHD/n9/retinaDisplay/iPadLarge/iPad`
ternaries, the `volatile unsigned char *rd` block, and the `rewardBoxY2`
bug.  Every structural variant below was a real `link_so` + `tools/fn_diff.py`
measurement.  All were size-fatal or raw-worse; none beat 5,385 at 7,072 B.

## 1. What the original actually does in block C

`w6784` established the original's four device blocks: A = iPadHD
(`0xd1042..0xd1268`), B = n9 (`0xd127e..0xd14c6`), C = everything else
(`0xd1570..0xd21e2`), tail (`0xd21e2..0xd2800`).  Re-reading the block-C
disassembly for the `0x2c4..0x2d8` group gives the exact source shapes:

* `0x2c4` stored at `0xd180a` (int, `str.w`).
* `0x2c8` stored at `0xd1820` via `vmov/vcvt.f32.s32/vcvt.s32.f32/vstr`
  (a `(int)(float)` round-trip of the `0x2c8` ternary).
* `0x2cc` stored at `0xd182c`/`0xd183a`/`0xd1856` (int, `str.w`).
* `0x2d0` stored at `0xd1882` via the same float round-trip.
* `0x2d4`/`0x2d8` stored together at `0xd18b6` as
  `strd r4, r6, [r0, #724]` (0x2d4/0x2d8).

The `0x2d4`/`0x2d8` values are built by conditional moves on `*rd`
(`retinaDisplay`), `iPad`, and `n9`:

```
d1866: movs r4, #160          ; r4 = 0x2d4
d1888: movs r1, #125          ; r1 = 0x2d8
d188e: cmp  r3, #0            ; r3 = *rd
d1890: it eq
d1892: moveq r1, #75
d1894: it eq
d1896: moveq r6, #125
d1898: orrs.w r5, r8, fp      ; r5 = iPad | n9
d189c: it eq
d189e: moveq r6, r1
d18a0: movs r1, #100
d18a2: cmp  r3, #0
d18a4: it eq
d18a6: moveq r1, #60
d18a8: it eq
d18aa: moveq r4, #100
d18ac: cmp  r5, #0
d18ae: it eq
d18b0: moveq r4, r1
d18b2: cmp.w fp, #0
d18b6: strd r4, r6, [r0, #724]
```

So, for the non-HD blocks:

```
0x2d4 = *rd ? 160 : ((iPad || n9) ? 100 : 60)
0x2d8 = *rd ? 125 : ((iPad || n9) ? 125 : 75)
```

The current HEAD source instead uses the older, more redundant shapes

```
0x2d4 = HD ? 112 : ((*rd || n9) ? (iPad ? 160 : 100) : (iPad ? 100 : 60))
0x2d8 = HD ? 141 : ((retina || n9) ? (iPad ? 200 : 125) : (iPad ? 125 : 75))
```

and stores `0x2d8` far from `0x2d4` (before `0x2c8`), so clang emits two
`str.w` instead of the original's `strd`.

## 2. Measured variants

All at `_ZN6LayoutC1Ev`; size must be exactly 7,072.  `raw` lower is better.

| variant | raw | aligned | size |
|---|---:|---:|---:|
| **base** | **5,385** | 2,562 | **7,072** |
| exact `0x2d4` shape (`*rd ? 160 : (iPad\|\|n9 ? 100 : 60)`) | 5,990 | 2,750 | 7,152 |
| exact `0x2d8` shape (`*rd ? 125 : (iPad\|\|n9 ? 125 : 75)`) | 5,531 | 2,558 | 7,064 |
| simplify `0x2d4` to `(retina\|\|iPad\|\|n9) ? 100 : 60` | 5,949 | 2,552 | 7,000 |
| simplify `0x2d8` to `(retina\|\|iPad\|\|n9) ? 125 : 75` | 5,531 | 2,558 | 7,064 |
| `0x2d4` `iPad ? 160 : 100` → `iPad ? 100 : 100` | 5,914 | 3,920 | **7,072** |
| swap n9/iPadLarge in `rewardBoxY`+`rewardBoxWidth` | 5,574 | 2,609 | 6,952 |
| swap n9/iPadLarge in `0x5c..0x2d0` group | 5,981 | 3,967 | 7,032 |
| move `0x2d8` after the `rd` block (strd merge) | 6,249 | 3,644 | 6,912 |
| `0x2d8` after `0x2c8`, before `rd` block | 6,256 | 3,643 | 6,912 |
| `0x2d8` after `0x2c8` with `0x2c8` moved too | 5,518 | 2,586 | **7,072** |
| `int v2d8` local + store after `0x2d4` | 5,385 | 2,567 | **7,072** |
| `int v2d8` local + store after `0x2c8` | 5,385 | 2,562 | **7,072** |
| `int v2d4/v2d8` locals, adjacent stores | 5,505 | 2,581 | 7,064 |
| exact `0x2d4` + `int v2d4/v2d8` locals | 5,813 | 2,881 | 7,144 |
| `rewardBoxY2` `(int)(float)` round-trip | 5,485 | 2,577 | 7,092 |
| `rewardBoxY2` round-trip + remove `0x10c` outer cast | 5,532 | 2,549 | 7,084 |
| `rewardBoxY2` round-trip + remove `0x10c`/add `0x194` | 5,564 | 2,541 | 7,060 |
| `rewardBoxY2` round-trip + `0x180`/`0x194` casts | 5,567 | 2,533 | 7,076 |
| `helpWindowY` int form (`202` instead of `144*1.40625f`) | 5,509 | 2,613 | 7,032 |
| `helpWindowY` HD/iPadLarge int + `(int)(float)` else | 5,509 | 2,613 | 7,032 |
| move `field_0x94` next to `field_0x90` | 5,907 | 2,690 | 7,032 |
| move `field_0x98` next to `field_0x90` | 5,923 | 2,594 | 6,984 |
| move both `0x94`+`0x98` next to `0x90` | 5,951 | 2,640 | 6,952 |
| move `0x94` next to `0x90` + `0x2c4`/`0x180` casts | 5,920 | 2,682 | 7,064 |
| `field_0x94` + `0x2c4`/`0x180` casts (no move) | — | — | 7,064 |
| all `retinaDisplay` reads via `*rd` (volatile at top) | 6,583 | 4,397 | 7,324 |
| `rewardBoxY2` if/else order n9-first | 6,020 | 3,954 | 7,148 |
| `rewardBoxY2` as a ternary (HD-first) | 5,561 | 2,594 | 6,960 |
| `rewardBoxY2` as a ternary (n9-first) | 6,020 | 3,954 | 7,148 |
| `float t_field_0x2c8` | 5,726 | 2,543 | 7,080 |
| `float v` for `field_0xa8` | 5,529 | 2,565 | 7,064 |

`(int)(float)` casts were also swept one-at-a-time over ~90 int-typed
ternaries (scratch `try_casts.py`): most are codegen-neutral at 7,072/5,385;
the few that change size are `0x2c4` (+24), `0x180` (+16),
`driftSpeedDivisor` (+28), `0x194` (−16), `0x1b0` (−32), `0x1b4`/`0x1b8`
(−128), `0x1bc` (−120), `0x2d8` (−120), `0x2c_rowHeight` (+0 but raw 5,810),
`0x4c` (−72), `0x34` (−80), `0x48` (−88), `progressBarYBase` (−64),
`hudInset` (−40), `0xc0`/`0x294_buttonWidth` (−184).  No single cast produced
a size-exact raw improvement.

## 3. Why no win

The raw metric is dominated by the **code-length / literal-pool shift**, not
by source fidelity.  The original's code ends at `0xd2800` (6,108 B of code
before the pool); HEAD's code ends at `0xd28b6` (6,288 B).  HEAD's literal
pool starts 180 B later and is 31 words smaller (190 vs 221).  Any change that
makes the source more faithful to the original's block-C shapes also changes
the code length (usually shrinking it by 40–160 B) and the pool does not grow
to compensate, so the function either loses size-exactness or moves the pool
further out of alignment.

The current 7,072-B body is a sharp local optimum: the exact `0x2d4`/`0x2d8`
shapes, the `strd` pair, the `rewardBoxY2` float round-trip, the n9/iPadLarge
order, and the `0x94`/`0x98` placement all reproduce the original's *source*
semantics better but score worse on same-address bytes.  A win would need a
change that simultaneously shortens the code toward 6,108 B and grows the
literal pool by the same amount (i.e. more SLP `vld1.64` pair loads), which is
a whole-function scheduling effect, not a local ternary edit.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12713
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5385 aligned=2562 size=7072/7072 insn=2518/2552
cmake --build cmake-build-match --target check -j2
# RATCHET PASS: byte_exact 4507, linked_exact 4507, size-exact 4524/4524,
# data_exact 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w12713/`: `Layout.base.cpp`
(pristine ctor), `orig.dis` / `ours.dis` (ctor disassembly), `try_casts.py`
(one-cast-at-a-time sweep), `orig_full.dis` (whole-binary disassembly).

Gate at exit: byte_exact **4507/4524**, linked_exact **4507/4524**,
size-exact **4524/4524**, stub_zero_size **0**, missing **0**, wrong_type
**0**, extra **0**, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift **0**, parity **5370/5370**, `.rel.plt` byte-identical.
`Layout` C1/C2 raw **5,385**, aligned **2,562**, size **7,072/7,072**,
2,518 instructions.  `src/` unchanged from base.
