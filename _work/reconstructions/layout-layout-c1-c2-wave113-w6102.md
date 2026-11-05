# Layout::Layout C1/C2 — wave 113 (w6102): a landable 7072-B gain via the 0x2d8 condition

Worker **w6102**, branch `wave113-w6102`, base `a979ace18`.
Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (same body and score),
ELF VMA `0xd1028`, original size **7072 B**.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.

## Result — first landable improvement in several waves

| metric | before | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** |
| `_ZN6LayoutC1Ev` unrounded linked `difflib` ratio | 48.385811 % | **48.435320 %** |
| normalised insns (orig/ours) | 2529 / 2489 | 2529 / 2489 |
| `byte_exact` / `linked_exact` | 4500 / 4500 | **4500 / 4500** |
| `size_exact` / `data_exact` | 4524 / 843 | **4524 / 843** |
| imports / lint / drift | 207 / CLEAN / 0 | **207 / CLEAN / 0** |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
27 section headers, file size 2,187,328 B, `.text` `0x1337c8`, `.ARM.extab` `0x26298` — all unchanged.

## The landed change

`src/game/ui/Layout.cpp`, the `field_0x2d8` assignment:

```cpp
// before
this->field_0x2d8 = Globals::iPadHD ? 141
    : ((Globals::retinaDisplay || Globals::n9) ? (Globals::iPad ? 200 : 125)
                                              : (Globals::iPad ? 125 : 75));
// after
this->field_0x2d8 = Globals::iPadHD ? 141
    : (((Globals::retinaDisplay + Globals::n9) != 0) ? (Globals::iPad ? 200 : 125)
                                                    : (Globals::iPad ? 125 : 75));
```

`Globals::retinaDisplay` / `Globals::n9` are `unsigned char` (0/1 flags), so the sum is zero
iff both are zero — semantically identical to `||`.  The sum form makes clang emit
`cmn.w r8, fp` (add-and-compare-with-zero) where the `||` form emits a separate
`orrs.w` + `cmp`, which aligns one more instruction with the original's tail.

## Measured variants (all real `link_so` builds, unrounded `difflib`)

Baseline (wave-112 HEAD body) = **7072 B / 48.385811 %**.

| variant | size | % |
|---|---:|---:|
| **landed: `0x2d8` condition as `(retina + n9) != 0`** | **7072** | **48.435320** |
| same, `(retina + n9)` (no `!= 0`) / `> 0` / reversed operands | 7072 | 48.435320 (code-identical) |
| same on the `0x2d4` `else if` instead | 7072 | 48.395455 |
| both `0x2d8` and `0x2d4` converted | 7004 | 43.645084 |
| `helpWindowX` `(retina \|\| n9 \|\| iPad)` → sum | 7004 | 47.112887 |
| `(retina \|\| n9)` → `(retina \| n9)` / `? 1 : n9` | 7072 | 48.385811 (code-identical) |
| `(retina != 0 \|\| n9 != 0)` | 7096 | 45.346062 |
| `rewardBoxHeight` if/else → ternary (conv11) | 7064 | 48.454636 |
| conv11 + `(int)(float)` on `0x1a4` / `0x2d8` | 7080 | 48.406375 |
| conv11 + redundant `0x285 = 0x286` (+8 B) | 7072 | 48.355591 |
| `0x1e8` → `(n9 \|\| iPadHD) ? 1 : retina` | 7008 | 47.063524 |
| `0x1e4` gets `(int)(float)` | 7016 | 45.917349 |
| swap `0x1e4`/`0x1e8` statements | 6956 | 34.204180 |
| `0x20_top` HD arm → `(int)(5*1.40625f)` | 6844 | 29.062247 |
| `0x204` moved next to `0x208` | 7024 | 45.254305 |
| `0x234` moved next to `0x238` | 7064 | 47.757624 |
| `0x27c` moved next to `0x280` | 7064 | 47.757624 |
| `0xb4` moved next to `0xb0` | 7000 | 43.150548 |
| `0xc4` moved next to `0xc0` | 7052 | 43.144279 |
| shared `retina?10:5` cluster for `0x20/0x24/0x28` (branch-scoped) | 7040 | 47.065868 |
| `u9c` local inlined | 7080 | 48.247012 |
| `v` local inlined | 7064 | 48.325359 |

Searches that found nothing better at 7072 B: all 13 if/else→ternary conversions
(single, all 78 pairs, all 286 triples), a `(int)(float)` wrap scan over every
single-line field assignment (236 builds), all adjacent statement swaps of the
landed body, and 200 random statement moves of the landed body.

## Established facts (re-confirmed this pass)

* The mandatory size is exactly **7072 B**: a 7064-B body drops whole-`.so`
  `byte_exact` from 4500 to **3221** (`RATCHET FAIL`), because `.text` shrinks and
  every following PC-relative `.rodata` reference shifts.  Measured directly.
* `vst1.32 {dN-dM}, [rX]` stores **16 bytes** (4 int fields), not 8 — the
  original's HD/tail "pairs" in earlier notes are 4-field groups.
* The original's tail C stores `0x1e8` before `0x1e4`, groups `0x204/0x208/0x20c/0x210`
  in one vector store, and has 4 `0x184` stores to our 3; the flat-ternary source
  cannot reproduce those groupings without losing more than it gains.
* `field_0x174` is *not* missing from our build: it is covered by the
  `vst1.32 {d16-d17}, [r0+0x16c]` 16-byte store (offsets 0x16c..0x178).
