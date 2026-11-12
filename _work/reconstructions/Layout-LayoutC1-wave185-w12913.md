# `Layout::Layout` C1/C2 — wave 185 / worker w12913

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave185-w12913`, base master `f390b0e68` (wave-184 harvest 10).

**Result: no size-exact raw win found.  `src/` left at the base.**

| metric | base |
|---|---:|
| raw same-address bytes | **5,383** |
| aligned (difflib) | 2,560 |
| size | **7,072/7,072** |
| instructions | 2,517/2,552 |

The task asked for the `Globals::iPadHD/n9/retinaDisplay/iPadLarge/iPad`
ternaries, the `volatile unsigned char *rd` block, and the `rewardBoxY2`
bug.  Every variant below is a real `link_so` + `tools/fn_diff.py`
measurement.  All were size-fatal or raw-worse; none beat 5,383 at 7,072 B.

## 1. Correct `0x2d4` / `0x2d8` shapes (and a correction to the wave-183 note)

The `Layout-LayoutC1-wave183-w12713.md` note decoded the original's
`0x2d4`/`0x2d8` block as

```
0x2d4 = *rd ? 160 : ((iPad || n9) ? 100 : 60)
0x2d8 = *rd ? 125 : ((iPad || n9) ? 125 : 75)
```

Re-tracing the `it eq` chains at `0xd188e..0xd18b6` shows that is **wrong**:
the `moveq`/`moveq` pair after `cmp r3,#0` both test the same `r3`, and the
`moveq` after `orrs r5,r8,fp` tests `r5 = iPad|n9`, not `*rd`.  The actual
data flow is

```
r6 = 200; r4 = 160; r1 = 125
if (*rd == 0) { r1 = 75; r6 = 125; }
if ((iPad|n9) == 0) r6 = r1
r1 = 100
if (*rd == 0) { r1 = 60; r4 = 100; }
if ((iPad|n9) == 0) r4 = r1
strd r4, r6, [r0, #724]
```

so the original semantics are

```
0x2d4 = (iPad || n9) ? (*rd ? 160 : 100) : (*rd ? 100 : 60)
0x2d8 = (iPad || n9) ? (*rd ? 200 : 125) : (*rd ? 125 : 75)
```

(the `iPadHD` arms are `112`/`141`).  The current HEAD source spells these as

```
0x2d4 = HD ? 112 : ((*rd || n9) ? (iPad ? 160 : 100) : (iPad ? 100 : 60))
0x2d8 = HD ? 141 : ((retina || n9) ? (iPad ? 200 : 125) : (iPad ? 125 : 75))
```

which is value-identical except for the `retina && n9 && !iPad` corner
(base gives 125/100, original gives 200/160), and stores `0x2d8` far from
`0x2d4`, so clang emits two `str.w` instead of the original's single `strd`.

The critical size fact: the original block is branchless
(`it/moveq` + `strd`), while ours is a branchy `if/else if/else` chain.
Faithfully writing the branchless shape **shrinks** the function by
136-160 B, so it is size-fatal on its own.

## 2. Measured variants (all `_ZN6LayoutC1Ev`, size must stay 7,072)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| **base** | **5,383** | 2,560 | **7,072** |
| `0x2d8` correct shape (positive form `(iPad\|\|n9) ? (retina?200:125) : ...`) | 5,817 | 3,950 | 7,080 |
| `0x2d8` correct shape (`!iPad && !n9` form) | 5,817 | 3,950 | 7,080 |
| `0x2d8` correct shape, n9-first nested form | 6,049 | 3,494 | 7,128 |
| `0x2d8` correct shape, `n9 ? ... : (iPad ? 200 : ...)` | 5,995 | 3,485 | 7,112 |
| `0x2d4` correct shape as a ternary | 5,965 | 2,738 | 6,936 |
| `0x2d4` correct shape as if/else | 5,965 | 2,738 | 6,936 |
| both `0x2d4`+`0x2d8` correct, `*rd` kept | 5,902 | 2,803 | 7,032 |
| both correct, `retinaDisplay` instead of `*rd` | 5,960 | 2,898 | 6,912 |
| `0x2d8` fix + `inlinev` (first exact-size combo) | 5,776 | 3,948 | **7,072** |
| `0x2d8` fix + `c10c` | 5,876 | 3,933 | 7,064 |
| `0x2d8` fix + `c2c8` | 5,910 | 3,931 | 7,064 |
| `0x2d8` fix + `novol` | 5,895 | 2,925 | 6,960 |
| `rewardBoxY2` `(int)(float)` round-trip | 5,483 | 2,575 | 7,092 |
| `rewardBoxY2` round-trip + `inlinev` + `novol` + `c10c` | 5,502 | — | 7,068 |
| `inlinev` (inline `v` into `field_0xa8`) | 5,519 | 2,563 | 7,064 |
| `novol` (drop `volatile` on `rd`) | 5,495 | 2,565 | 7,064 |
| `c10c` (drop outer `(int)(float)` on `field_0x10c`) | 5,502 | 2,540 | 7,064 |
| `c2c8` (drop `(int)(float)` on `field_0x2c8`) | 5,556 | 2,569 | 7,064 |
| `ct` (`t_field_0x2c8` HD arm loses `(int)`) | 5,596 | 2,534 | 7,088 |
| `field_0x20_top` HD arm `(int)(5*1.40625f)` | 6,282 | 3,725 | 6,808 |
| `buttonInsetX` HD arm `(int)(5*1.40625f)` | 5,888 | 2,734 | 7,032 |
| `field_0x24` HD arm `(int)(5*1.40625f)` | 6,331 | 3,574 | 6,688 |
| `field_0x148` HD arm `(int)(2*1.40625f)` | 5,523 | 2,563 | 7,064 |
| `field_0x1f4` HD arm `(int)(16*1.40625f)` | 5,477 | 2,575 | 7,064 |
| `field_0x204` HD arm `(int)(9*1.40625f)` | 5,511 | 2,568 | 7,064 |

`(int)` on the HD arm makes the whole ternary int-typed and removes the
`vcvt.f32.s32` round-trip that the original *does* keep in the non-HD
blocks, so it is consistently raw-worse.

### Exact-size combinations (all raw-worse than 5,383)

`inlinev+d2d8` 5,776 · `inlinev+c10c+ct` 5,513 · `inlinev+c2c8+ct` 5,532 ·
`inlinev+ct+i148` 5,550 · `inlinev+ct+i1f4` 5,530 ·
`inlinev+ct+i204` 5,555 · `c10c+c2c8+ct` 5,593 · `c10c+ct+i148` 5,567 ·
`c10c+ct+i1f4` 5,529 · `c10c+ct+i204` 5,557.  All size 7,072/7,072.

## 3. `flip.py` condition-flip sweep (units 39..203)

Flipping the top-level ternary condition (`!(cond)` with swapped arms,
semantics-preserving) for every ctor statement unit 39..203 is **neutral**:
every result is exactly `raw=5383 aligned=2560 size=7072/7072` (units 94 and
173 fail to build because of a float-literal `flip` bug, not a real
failure).  Combined with the wave-183 run over units 1..38, the full ctor is
flip-invariant.

## 4. Why the residual is hard

Original code = **6,188 B** of instructions + **884 B** literal pool; ours =
**6,320 B** + **752 B** (total both 7,072).  The base is exactly the right
size, but the split is wrong: we carry 132 B too much code and 132 B too
little pool.  The aligned-diff category counts show the mechanism:

```
extra-orig:movs   247    extra-ours:movs   190
extra-orig:add.w   56    extra-ours:str.w  115
extra-orig:str.w   51    extra-ours:mov.w   53
                         extra-ours:it      53
                         extra-ours:cmp.w   53
```

The original pairs/vectorises far more stores (`strd` / `add.w`+`vst1.32`),
while ours emits individual `str.w` with `cmp`/`it` predicates.  The
`0x2d4`/`0x2d8` `strd` is one instance of that global difference.  Any
single local fix that makes one block faithful changes the code/pool split
and is therefore size-fatal; recovering 7,072 B requires the *whole* store
pairing to match, which is a multi-block scheduling change, not a local
edit.  The ctor is a sharp local optimum for single-statement source edits.

## 5. Artifacts

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w12913/`
(`flip2.py`, `combo2.py`, `detail.py`, `fields.py`, candidate `.cpp`s,
`flip2_run.txt`, `fields_orig.txt`, `fields_ours.txt`).
