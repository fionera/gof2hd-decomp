# Layout::Layout C1/C2 — size-filtered order search finds a strict improvement (w4901, wave 99)

Worker **w4901**, branch `wave99-w4901`, base `44225192c`.
Target: `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` at ELF VMA `0xd1028`, original size
**7072 B**. Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(Ghidra image base `0x10000`; ctor at `0xe1028` in the Ghidra project).

Continues `layout-layout-c1-c2-ghidra-order-w4801.md` (wave 98, the landed
47.733 state), `layout-layout-c1-c2-hd-store-table-w4701.md`,
`layout-layout-c1-c2-store-order-w4502.md` and
`layout-layout-c1-c2-hd-vector-w4101.md`.

## Result

Landed a **size-neutral** statement reorder: **47.7326968973747 -> 47.77247414478918**
(unrounded `fscore.py`, `autojunk=False`), `usize = 7072` unchanged. This is a
strict improvement of **+0.0398 pp** on the official fuzzy metric.

Gate before == after (the function size is unchanged, so no downstream function
moves):

| metric | before | after |
|---|---:|---:|
| `byte_exact` | 4500 / 4524 | **4500 / 4524** |
| `linked_exact` | 4500 / 4524 | **4500 / 4524** |
| `size_exact` | 4524 / 4524 | **4524 / 4524** |
| `stub_zero_size` / `missing` / `wrong_type` / `extra` | 0 | **0** |
| `data_exact` | 843 / 843 | **843 / 843** |
| imports | 207 | **207** |
| lint / drift | CLEAN / 0 | **CLEAN / 0** |

`RATCHET PASS`.

## 1. The landed change

Only `src/game/ui/Layout.cpp` changes (13 insertions / 13 deletions): the
`field_0x184` if/else chain and the single `field_0x1a4` assignment move from
*after* `field_0x188` to *before* the `field_0x190` if/else chain.

```diff
     this->field_0x194 = ...;
+    if (Globals::iPadHD)
+        this->field_0x184 = 163;
+    else if (Globals::iPadLarge)
+        this->field_0x184 = 346;
+    else if (Globals::iPad)
+        this->field_0x184 = 173;
+    else if (Globals::n9)
+        this->field_0x184 = 210;
+    else if (Globals::retinaDisplay)
+        this->field_0x184 = 232;
+    else
+        this->field_0x184 = 116;
+    this->field_0x1a4 = ...;
     if (Globals::iPadHD) {
         this->field_0x190 = -6;
     } else if (Globals::iPadLarge) {
@@
     this->field_0x180 = ...;
     this->field_0x188 = ...;
-    if (Globals::iPadHD)
-        this->field_0x184 = 163;
-    ... (same chain) ...
-    this->field_0x1a4 = ...;
     this->field_0x198 = ...;
```

This is the window `0xd1f28..0xd2128` that w4701 §4 identified as the worst
(14.1 %); moving `0x184`/`0x1a4` ahead of `0x190` lets clang's scheduler emit a
longer matching run through the `0x170..0x1a8` chain without changing the
function size.

## 2. Method: size-filtered order search with a fast object-level scorer

The previous passes hill-climbed the 204 top-level statements with the *linked*
scorer (~1.5 s/eval) and only adjacent swaps / a few hundred block moves. This
pass uses a fast scorer that compiles only `Layout.cpp` with the exact project
flags (NDK r18b clang 7.0.2, `-Oz`, `-mthumb`, ...), extracts the
`_ZN6LayoutC1Ev` bytes from the object with `objcopy`, and reuses
`tools/verify`'s `disassemble`/`reconcile`/`SequenceMatcher(autojunk=False)`.

Validation of the fast scorer against the official linked scorer:

| body | fast | official |
|---|---:|---:|
| pristine tree | 47.8123 | 47.7327 |
| full iOS source order | 38.0474 | 38.0072 |
| original default-path store order as source | 27.5918 | 27.5512 |
| `0x20` HD branch -> int `7` | 15.6169 | 13.4711 |

The fast scorer tracks the official ranking (it is ~0.08 pp optimistic on the
pristine body, but the ordering is consistent). The search:
- 8 parallel clang compiles, candidates written to `/dev/shm/w4901`;
- random moves: block moves of length 1-8, single swaps, block moves of length
  1-25, and segment reversals;
- **accept only candidates whose object-level function size is exactly 7072**,
  because `size_exact` is 4524/4524 and any other size is unlandable;
- keep the best, repeat for 7 rounds of 400-600 candidates.

The winning candidate was found in round 1 of the first 400-candidate run
(seed 4901). A second 600-candidate run (seed 12345) from the new best and a
targeted exhaustive "move one of the 18 units in `0x16c..0x1bc` to every
position" search found no further improvement.

## 3. Why the size filter matters

The unconstrained search immediately finds a higher-scoring **7048 B** body
(47.8295 official) by moving `field_0x308` next to `field_0xac` and `0x98` to
the tail. It is not landable: `size_exact` would drop from 4524/4524. The
size-7072 filter rejects it and keeps the `0x184`/`0x1a4` move instead.

## 4. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8          # RATCHET PASS
python3 /dev/shm/w4901/fscore.py                            # 47.77247414478918, 7072 B
```

Scratch (`/dev/shm/w4901`): `fscore.py` (official unrounded linked scorer),
`fastscore.py` (object-level scorer), `units.py` (header offset map + exact
204-unit splitter), `harness.py` (rewrite ctor body), `search2.py` /
`rsearch.py` / `movesearch.py` (parallel size-filtered searches),
`best_order.txt` (the landed 204-offset order), `orig_order.txt` (base order).
