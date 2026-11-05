# Layout::Layout C1/C2 — wave 114 (w6202): cold-block placement is not
# reachable from the flat-ternary source; 620-swap search confirms the
# 48.5452 % / 7072-B frontier

Worker **w6202**, branch `wave114-w6202`, base `d8557f7e2` (wave-113 harvest).
Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (same body and score),
ELF VMA `0xd1028`, original size **7072 B**.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.

## Result — no landable improvement found; the wave-113 body is the optimum

| metric | before | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| `_ZN6LayoutC1Ev` unrounded linked `difflib` ratio | 48.545237146273 % | **48.545237146273 %** |
| `_ZN6LayoutC2Ev` | 48.545237146273 % | **48.545237146273 %** |
| normalised insns (orig/ours) | 2529 / 2489 | 2529 / 2489 |
| `byte_exact` / `linked_exact` | 4500 / 4500 | **4500 / 4500** |
| `size_exact` / `data_exact` | 4524 / 843 | **4524 / 843** |
| imports / lint / drift | 207 / CLEAN / 0 | **207 / CLEAN / 0** |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
27 section headers, file size 2,187,328 B, `.text` `0x1337c8`, `.ARM.extab`
`0x26298` — all unchanged (no code change was landed).

**This pass is a note-only commit.** Every variant below was built for real
(`link_so` + `tools/verify` scorer) and measured; none beat 48.5452 % at the
mandatory 7072 B.

## The target's 35 missing instructions are the four out-of-line cold blocks

The original ends its main body at `d2800` (`pop {r4,r5,r6,r7,pc}`) and then has
**four** out-of-line blocks before the literal pool:

| addr | site | code | insns |
|---|---|---|---:|
| `d2802` | `0x130/0x134/0x138` retina==0 arm | `0x130=iPad?56:40`, `0x134=iPad?108:80`, `r1=iPad?47:35`; `b.w d1cba` | 16 |
| `d2828` | `0x2c8` retina==0 arm | `r6=iPad?45:30`; `b.w d1814` | 5 |
| `d2836` | `0x2d0` retina==0 arm | `r2=iPad?45:30`; `b.w d1860` | 5 |
| `d2844` | `0x60` retina==0 arm | `r6=iPad?64:44`; `b.w d16d6` | 5 |

All four are the innermost `iPad ? X : Y` select of a
`iPadLarge ? A : (retina ? B : (iPad ? X : Y))` chain, reached by a
`beq.w` on `retinaDisplay == 0`. They are **not** an IR difference: each cold
block is itself `it/moveq`-if-converted, exactly like the inline form. The
difference is clang's `MachineBlockPlacement` putting the retina==0 arm after
the epilogue in the original and inline in our build.

Our build has only **one** out-of-line block (`d28ba`, the `0x1ac/0x1a0`
retina==0 arm, 10 insns). Per the wave-113 note the original main body is
2082 insns + 35 cold = 2117; ours is 2136, i.e. the main body is 54 insns
long, and the four cold blocks are absent.

### Why `__builtin_expect` does not move them

`__builtin_expect(Globals::retinaDisplay, 1)` on the innermost retina test of
`0x60`, `0x2c8`, `0x2d0` (individually and in pairs) is **code-identical**
(7072 B / 48.5452).  On the `0x130` chain it is 7084 B / 48.2772 (wave-113
handoff).  Inverting the test (`!retina ? (iPad?X:Y) : B`) is also
code-identical for `0x60`, `0x2c8`, `0x2d0` and all three together.  The
placement is therefore not driven by branch probability that source-level
`expect` can reach.

## What the original's non-n9 / tail-A stores look like

The original's non-n9 block (`d1570..d185e`) ends with `0x2c4`, `0x2c8`,
`0x2cc`; the shared tail A (`d1860`) starts with `0x2d0`, then stores
`0x2d4`/`0x2d8` with a single `strd r4,r6,[r0,#724]` at `d18b6`, then the
`0x7c` group.  The n9 block stores `0x2c0..0x2cc` with
`stmia.w ip!, {r1,r2,r3,r6}` at `d12c0` (`ip = r0+0x2c0`).

Our build instead stores `0x2c4`, `0x1e0`, `0x2cc`/`0x2d0` in the n9 block,
pushes `0x2c0`/`0x2c8` into the tail, and stores `0x2d8` and `0x2d4`
separately (no `strd`).  The faithful ternary form of `0x2d4`/`0x2d8`
reproduces the original's `strd` (variant Y below) but is 16 B short and
scores 48.145.

## Measurements (all real builds, unrounded linked score)

Baseline (wave-113 HEAD body) = **7072 B / 48.545237146273 %**, 2529/2489 insns.

| variant | size | % |
|---|---:|---:|
| **baseline (wave-113 body)** | **7072** | **48.5452** |
| shared `unsigned char *ip = &Globals::iPad` used in all four innermost selects | 7072 | 48.5452 (code-identical) |
| all 49 `Globals::iPad` reads via one local pointer | 7072 | 48.5452 (code-identical) |
| `__builtin_expect(retina,1)` on `0x60` / `0x2c8` / `0x2d0` (each and pairs) | 7072 | 48.5452 (code-identical) |
| inverted retina test on `0x60` / `0x2c8` / `0x2d0` (each and all) | 7072 | 48.5452 (code-identical) |
| move the whole `0x2d4` if/else block before `0x2d8` | 7072 | 48.5452 (code-identical) |
| reorder `0x2c8,0x2cc,0x2d0` ascending | 7072 | 48.5054 |
| `0x2d4` condition `(retina+n9) != 0` | 7072 | 48.5054 |
| `0x2d8` condition `(retina+n9) != 0` | 7072 | 48.3762 |
| non-volatile `rd` / no `rd` at all | 7064 | 48.4945 |
| volatile `rd = &Globals::iPadLarge` | 7064 | 48.4244 |
| volatile `rd = &Globals::iPad` | 7064 | 43.4037 |
| volatile `rd = &Globals::n9` | 7072 | 37.8012 |
| `0x2d4`/`0x2d8` faithful ternaries (`strd`, matches the original) | 7056 | 48.1452 |
| same, `0x2d8` first | 7056 | 48.1452 |
| both faithful ternaries with a volatile `retina` read | 7072 | 48.1865 |
| move `0x2c0` before `0x2c4` | 7028 | 45.1278 |
| move `0x2d0` after `0x2cc` | 7048 | 48.2332 |
| both of the above | 7028 | 45.4473 |
| ascending `0x2c0,0x2c4,0x2c8,0x2cc,0x2d0,0x2d4,0x2d8` + `eventBannerScale` | 6972 | 44.5291 |
| ascending group with `0x2d4` as a ternary | 6960 | 42.3817 |
| `eventBannerScale` after `0x2d8` | 7012 | 45.3946 |
| `eventBannerScale` after `0x1dc` | 6968 | 44.1413 |
| `eventBannerScale` after `0x1e4` | 6976 | 41.5162 |
| `eventBannerScale` at the end of the ctor | 6984 | 37.1618 |
| `helpWindowX` condition `(retina+n9+iPad) != 0` | 7024 | 45.6903 |

### Exhaustive random search

**620 random pairwise statement swaps** (seed 7, 620 s, all 255 ctor units,
including the multi-line `if/else` groups): **no variant reached 7072 B with a
score above 48.5452**.  Combined with the wave-113 searches (all 207 adjacent
swaps, all 120 permutations of the `0x184` chain, all 120 permutations of the
`0x2c4/eventBannerScale/0x2d0/0x2cc/0x2c8` group, 620 s random moves), the
body is a strong local optimum.

### Match distribution by region (pristine body)

`difflib` equal instructions per tenth of the original instruction stream:

| tenth | original range | equal |
|---|---|---:|
| 0 | `d1028..d1346` (HD block head) | 38.7 % |
| 1 | `d1346..d1548` (HD block tail) | 87.7 % |
| 2 | `d1548..d17ea` (n9 + non-n9 head) | 63.2 % |
| 3 | `d17ea..d1af6` (non-n9 tail) | 42.3 % |
| 4 | `d1af6..d1e16` (tail A/B head) | 31.2 % |
| 5 | `d1e16..d20f4` (tail B) | **20.2 %** |
| 6 | `d20f4..d2460` (tail B/C) | 28.1 % |
| 7 | `d2460..d2750` (tail C) | 39.1 % |
| 8 | `d2750..d299a` (tail C end + cold blocks) | 47.0 % |
| 9 | `d299a..d2be8` (literal pool) | 84.1 % |

The worst region is tail B (`d1e16..d20f4`, 20.2 %), where the original has
~45 explicit branches (`d1e16`, `d1e20`, `d1e26`, `d1e28`, `d1e4a`, `d1e62`,
`d1e7e`, `d1ec4`, `d1ec6`, `d1ed0`, `d1efa`, `d1efc`, `d1f18`, `d1f3c`, …)
while the flat-ternary source if-converts the same selects.

## Proven vs hypothesis

**Proven (binary / measured):**

* The original's four cold blocks are the retina==0 arms of `0x130/0x134/0x138`,
  `0x2c8`, `0x2d0`, `0x60`; they are pure block placement, not IR.
* Our build emits only one cold block (`0x1ac/0x1a0`), not the original's four.
* `__builtin_expect`, inverted tests and a shared `iPad` pointer are all
  code-identical here; they cannot move the cold blocks.
* The faithful `0x2d4`/`0x2d8` ternary pair reproduces the original's
  `strd r4,r6,[r0,#724]` but costs 16 B (7056) and measures 48.145.
* The wave-113 body is a strong local optimum: 620 random pairwise swaps plus
  all prior wave-113 searches find nothing better at 7072 B.

**Hypothesis (not proven):**

* Closing the remaining ~51.5 % needs the original's exact IR block layout:
  the retina==0 arms must be separate blocks placed after the epilogue, and
  tail B must keep the original's ~45 explicit branches instead of
  if-converting them.  The flat-ternary source shape cannot express that
  without changing the function size (the faithful forms measured 6960–7056 B).
* `field_0x1e4` is a `float` in the original (stored with `vstr`); our `int`
  declaration is a real type divergence that cannot be fixed without touching
  `Hud.cpp`.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6202
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 /dev/shm/w6202/score.py                         # 7072 B, 48.545237146273
```

Scratch harness (all under `/dev/shm/w6202`): `score.py` (unrounded linked
scorer), `Layout.pristine.cpp` (wave-113 body), `vtry.py` / `reord.py`
(statement splice + reorder), `randswap.py` (620-swap search),
`orig.asm` / `ours_pristine.asm` (objdump dumps of `0xd1028..0xd2be8`),
`stores.py` (per-offset store map).
