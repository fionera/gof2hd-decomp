# `Layout::Layout` C1/C2 — wave 147 / worker w9409
## Landed the `0x184` semantic fix at the exact 7,072 B size.  A one-statement move
## (`0x180` to the end of its block) restores the original's duplicated `0x1dc`/`0x2d8`
## tail stores, pays back the 8 B the fix removes, and keeps alignment near baseline.

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr `0xd1028`,
size **7,072 B**, source `src/game/ui/Layout.cpp`.  Baseline = wave-147 master `cbe3b5f7f`:
raw same-address diff **5,704**, aligned instruction diff **2,304**, gate byte_exact
4507/4524, linked_exact 4507/4524, data 843/843, imports 207, lint CLEAN.

## 1. The `0x184` bug (confirmed) and the fix

The original's truth table is
`iPadHD ? 163 : iPadLarge ? 346 : iPad ? 173 : n9 ? 210 : retina ? 232 : 116`
— **iPad before n9**.  HEAD had `n9` before `iPad`, so `n9 && iPad` produced 210
instead of 173 (w9306).  The landed expression is exactly the original chain:

```cpp
this->field_0x184 = Globals::iPadHD ? 163 : (Globals::iPadLarge ? 346 : (Globals::iPad ? 173 : (Globals::n9 ? 210 : (Globals::retinaDisplay ? 232 : 116))));
```

The fix alone measures **7,064 B / raw 5,841 / aligned 2,429** — semantically right
but 8 B short and 137 raw bytes worse (it merges the `iPadLarge` and `iPad` stores:
3 `str [r0,#388]` instead of the original's 4).  It cannot land without a size anchor.

## 2. The size anchor is the duplicated tail store, not a padding trick

The mission hint was the `0x1b0`/`0x1dc`/`0x2d8` HD-arm stores.  Store census:

| field | original | HEAD | fix alone | landed |
|---|---:|---:|---:|---:|
| `0x184` (`str #388`) | 4 | 4 | 3 | 3 |
| `0x1dc` (`str #476` / `vst1` group) | 2 | 1 | 1 | **2** |
| `0x2d8` (`str #728`) | 2 | 2 | 2 | 2 |
| `add.w rX,r0,#432` (`0x1b0` group) | 3 | 3 | 3 | 3 |
| `vst1.32` total | 81 | 70 | 70 | 70 |

HEAD leaves the second `0x1dc` store to the common tail.  Moving the `0x180`
assignment from before `0x188` to the end of the `0x180..0x1a0` block changes the
shared-tail/common-tail partition so clang re-emits the `0x1d0..0x1dc` vector group in
*both* tail arms — exactly the original's duplicated `0x1dc` store — and that is the
8 B the fix had removed.

Landed change is two lines: the `0x184` ternary and the `0x180` statement move.

## 3. Measurements (real `link_so`, `/dev/shm/w9409/fdiff.py` + `align_diff.py`)

| variant | size | raw | aligned |
|---|---:|---:|---:|
| HEAD (buggy) | **7,072** | **5,704** | **2,304** |
| fix only | 7,064 | 5,841 | 2,429 |
| **landed: fix + `0x180` moved to end** | **7,072** | **5,781** | **2,317** |
| fix + `0x180` before `0x188` | 7,072 | 5,759 | 2,444 |
| fix + `0x188` to end | 7,072 | 5,823 | 2,309 |
| fix + `0x180` before `0x188` + `0x188` to end | 7,072 | 5,823 | 2,309 |
| iOS ascending `0x164..0x1a4` | 7,104 | 5,806 | 2,420 |
| iOS ascending + `0x180` to end | 7,072 | 5,818 | 2,482 |

The landed variant was chosen over the raw-best (`0x180` before `0x188`, 5,759) because
it keeps the aligned diff at 2,317 (baseline 2,304) instead of 2,444, and it is the
variant that reproduces the original's second `0x1dc` store.  The fix inherently costs
~55–120 raw bytes because the original's 4-store `0x184` chain lives in a device-major
tail we have not reconstructed yet.

## 4. `__builtin_expect` sweep on the `0x184` chain

`__builtin_expect` on the four flag tests (16 masks, fix-only base) does not produce the
4-store chain.  The best aligned result is mask `expect(iPadLarge,0), expect(iPad,0),
expect(n9,1)`: **7,088 B / raw 5,824 / aligned 2,268** (better aligned than baseline,
16 B too big; moving `0x19c`/`0x194`/`0x168` to shave 16 B gives 7,072 at raw 5,784–5,891
but aligned 2,266–2,348).  Not landed because the landed variant is smaller in source
change and has the correct duplicated `0x1dc` store.

## 5. Gate

`cmake --build cmake-build-match --target check` on the landed tree:
byte_exact **4507/4524**, linked_exact **4507/4524**, size 4524/4524, data **843/843**,
imports **207**, lint **CLEAN**, ratchet **PASS**.  Function size stays **7,072 B**, so no
later function shifts and no section size changes.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9409
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9409/fdiff.py _ZN6LayoutC1Ev        # 7072 / 5781
python3 /dev/shm/w9409/align_diff.py _ZN6LayoutC1Ev   # 2317
cmake --build cmake-build-match --target check -j6
```

Scratch (`/dev/shm/w9409/`): `fdiff.py`, `align_diff.py`, `eval.sh`, `gen.py`,
`movesweep.py`, `movesweep4.py`, `exp184.py`, `combo.py`, `ascmove.py`,
`Layout.base.cpp`, `Layout.fix.cpp`, `Layout.exp4.cpp`, `v_asc_fix.cpp`,
`v_pos11.cpp`, `orig.dis`, `ours.dis`, `fix.dis`, `candA.dis`, `candB.dis`.

## 7. Open questions

* The original's 4th `0x184` store (346 in the iPadLarge arm) needs the device-major
  tail: the iPadLarge block stores `0x180..0x1a0` as a unit.  Our per-field ternaries
  let clang merge the 346/173 stores.
* The landed `0x180`-to-end move is a codegen lever, not the original source order
  (the original stores `0x180` before `0x184` in window `0xe00`).  A faithful device-major
  reconstruction should supersede it.
* `expect(iPadLarge,0),expect(iPad,0),expect(n9,1)` reaches aligned **2,268** (below the
  2,304 baseline) at 7,088 B; pairing it with the right 16 B tail reduction is the
  shortest path to a sub-baseline aligned diff.
