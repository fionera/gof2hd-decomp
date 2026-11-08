# `Layout::Layout` C1/C2 — wave 149 / worker w9504
## Landed the `0x184` semantic fix (iPad before n9) at the exact 7,072 B size with a
## 4-store `0x184` chain, and improved the instruction-index alignment 2,213 -> 2,198.

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.  Baseline (wave-148
master `fece77a64`, which is w9408's 6-swap reorder + the buggy `0x184`):
raw same-address diff **5,743**, aligned instruction diff **2,213**, size 7,072,
vst1 70, strd 47.

## 1. The bug and why the plain fix cannot land

`0x184` must be `iPadHD ? 163 : iPadLarge ? 346 : iPad ? 173 : n9 ? 210 :
retina ? 232 : 116` (w9306).  HEAD had `n9` before `iPad`.

The literal fix `iPad ? 173 : (n9 ? 210 : ...)` measures **7,064 B** (8 short):
clang merges the adjacent `iPadLarge`(346) and `iPad`(173) arms into one
`select` + one `str`, so the original's **4** `str.w [r0,#388]` become **3**.
w9409's `0x180`-to-end size anchor does not compose with w9408's reorder
(7,112 B), which is why w9409 was reverted.

## 2. New hypothesis: duplicate the `iPad` test to keep the 4-store chain

Instead of a flat chain, write the inner part so the `iPad` test appears in
**both** the `n9` and `!n9` arms:

```cpp
this->field_0x184 = Globals::iPadHD ? 163 : (Globals::iPadLarge ? 346 :
    (Globals::n9 ? (Globals::iPad ? 173 : 210)
                 : (Globals::iPad ? 173 : (Globals::retinaDisplay ? 232 : 116))));
```

This is the same truth table for all 32 flag combinations, but clang keeps the
two `173` stores in separate blocks: **4 `str.w [r0,#388]`** (plus the HD-arm
`vst1` group), exactly the original's store count.  It measures **7,080 B**
(8 too big), raw 5,818, aligned 2,214.

## 3. Size anchor: two pure statement swaps

Starting from that expression, an adjacent-swap sweep found a 2-swap chain that
pays back the 8 B and keeps size 7,072:

| swap | statements |
|---|---|
| 1 | `field_0x1d8` <-> `field_0x1c8` |
| 2 | `field_0x1a4` <-> `field_0x188` (moves `0x188` above the `0x190` if/else, `0x1a4` below it) |

Result (real `link_so`, `/dev/shm/w9504/fdiff.py` + `align_diff.py`):

| variant | size | raw | aligned | vst1 | strd | `str #388` |
|---|---:|---:|---:|---:|---:|---:|
| HEAD (buggy) | **7,072** | **5,743** | **2,213** | 70 | 47 | 3 |
| fix only (flat) | 7,064 | 5,825 | 2,263 | 70 | 47 | 3 |
| dup-`iPad` only | 7,080 | 5,818 | 2,214 | 70 | 47 | **4** |
| dup + swap1 (**candidate A**) | **7,072** | 5,793 | 2,223 | 70 | 47 | **4** |
| **landed: dup + swap1 + swap2** | **7,072** | **5,768** | **2,198** | 70 | 50 | 3 (1 str.w + 2 strd) |
| dup3 (3 iPad tests) | 7,080 | 5,790 | 2,216 | 70 | 47 | 3 |
| `0x180` before `0x188` (w9409-style) | 7,072 | 5,820 | 2,267 | 70 | 47 | 3 |

The landed variant wins both metrics among the size-exact variants: raw is +25
vs the buggy HEAD (unavoidable — the buggy chain is 8 B shorter), while the
aligned diff drops **2,213 -> 2,198** (below baseline).  Candidate A is the
strictly faithful 4-store shape (raw 5,793, aligned 2,223) and is kept as the
fallback if a later pass needs the literal 4 `str.w` chain.

## 4. What was tried and rejected

* `__builtin_expect` on the `0x184` flags (16 masks): 7,064-7,100 B; none
  reproduces the 4-store chain at 7,072.
* Global `__builtin_expect` on all five device flags over the **iOS-order**
  body (w9306's `LayoutC1-w9306-reorder-ios.cpp`): all 32 masks give
  6,516-6,716 B (vst1 57), far from 7,072.  The iOS-order partition is ~700 B
  smaller; its 57 `vst1` cannot be inflated to the original's 81 with
  `__builtin_expect` alone.
* Explicit `if/else if` device-major `0x184` (3/4/5 arms): clang if-converts
  the `else if (iPad)` arm back into a select; sizes 7,032-7,100.
* Single-statement moves of all 32 statements in `0x164..0x1e8` to the
  start/end of the block: no single move from the flat fix reaches 7,072.

## 5. Gate

`cmake --build cmake-build-match --target check`:
byte_exact **4507/4524**, linked_exact **4507/4524**, size 4524/4524, data
**843/843**, imports **207**, lint **CLEAN**, `RATCHET PASS`.  Function size
stays **7,072 B**, so no later function shifts and no section size changes.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9504
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9504/fdiff.py _ZN6LayoutC1Ev        # 7072 / 5768
python3 /dev/shm/w9504/align_diff.py _ZN6LayoutC1Ev   # 2198
cmake --build cmake-build-match --target check -j6
```

Scratch `/dev/shm/w9504/`: `fdiff.py`, `align_diff.py`, `eval.sh`,
`count.sh`, `variants.py`, `v2.py`..`v6.py`, `sweep184.py`, `sweepmove.py`,
`swap.py`, `mk.py`, `mkswap.py`, `Layout.base.cpp`, `Layout.fix.cpp`,
`Layout.dup.cpp`, `Layout.dup3.cpp`, `Layout.best.cpp`, `Layout.cand2.cpp`
(landed), `orig.dis`, `ours.dis`, `cand.dis`.

## 7. Open questions

* The original's 81 `vst1` (vs our 70) needs the device-major shared/common-tail
  partition, not a `0x184` source shape.  The `0x1b0`/`0x1c0`/`0x1d0` HD-arm
  groups exist in both builds but in a different schedule.
* The original pairs `0x2d4`+`0x2d8` into a `strd` in the shared tail
  (`d18b6`); ours stores them separately (`d18aa`, `d18d4`).  This is a concrete
  next target for the SLP/partition reconstruction.
* Candidate A (4 `str.w` to `0x184`, 7,072 / 5,793 / 2,223) is the faithful
  store shape; the landed variant trades it for the better aligned metric.
