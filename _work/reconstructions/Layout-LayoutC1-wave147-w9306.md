# `Layout::Layout` C1/C2 — wave 147 / worker w9306
## Store-list diff against the original: **no missing field assignments** (204/204 on all
## 32 flag combinations), but one real value bug at `0x184`.  Faithful iOS-order reorder
## improves raw to 5,532 but shrinks the function 7,072 -> 6,260 B.  No size-exact win landed.

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr `0xd1028`,
size **7,072 B**, source `src/game/ui/Layout.cpp`.  Baseline (wave 146 master `9c9744ca5`):
raw same-address diff **5,704**, aligned instruction diff **2,304**.  Gate on the unchanged
tree: byte_exact 4507/4524, linked_exact 4507/4524, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0.

## 1. Store-list diff (mission hypothesis)

The `layout_ctor/emu.py` Thumb/VFP emulator was run over **all 32 device-flag
combinations** for both the original and our linked `.so` (`table.py`), and the
per-byte store maps compared.  Result:

* every combination writes the **same set of bytes** (202 words / 807 bytes);
* the trace has exactly **204 store instructions per path** in both;
* only **one** value differs: field `0x184` for `n9=1 && iPad=1` (combos `01100`,
  `01101`): original **173**, ours **210**.

So the wave-146 premise "the original emits more stores than us" is **not** a missing
assignment: the original's extra `movs`/`mov.w`/`str.w` counts are the *duplicated
device arms* and the less-vectorised schedule, not absent fields.  (This agrees with the
older `layout_ctor_missing_272_w3647.md` finding.)

### 1.1 The `0x184` bug is real

The original's truth table for `0x184` is

```
iPadHD ? 163 : iPadLarge ? 346 : iPad ? 173 : n9 ? 210 : retina ? 232 : 116
```

i.e. **iPad before n9**.  Our source has `n9` before `iPad`, so `n9 && iPad` gives 210
instead of 173.  Original disassembly `d1fa2..d1fd6` confirms the chain
`r4(iPad) ? 173 : (!n9 ? (retina ? 232 : 116) : 210)`.

Fixing only that ternary (`n9`/`iPad` swapped) measures **7,064 B / raw 5,841**
(aligned 2,429): semantically correct but 8 B short and 137 raw bytes worse, so it
cannot be landed without a size anchor.

## 2. Faithful iOS source order (re-tested with the *current* expressions)

The original's first-write order for combo `00000` is the iOS source order of
`layout_ctor/ios_source_order.md` (modulo scheduler pair swaps).  The current source is
a hill-climbed permutation.  Reordering the current statements (expressions unchanged)
to the iOS order gives:

| variant | size | raw | aligned | vst1 | vbsl | clz |
|---|---:|---:|---:|---:|---:|---:|
| HEAD (scrambled order) | **7,072** | **5,704** | **2,304** | 70 | 7 | 2 |
| iOS order, whole body | 6,324 | 5,665 | 2,845 | 57 | 4 | 2 |
| iOS order, tail from `0x94` on | 6,260 | **5,532** | — | — | — | — |
| iOS order, tail from `0x108` on | 6,244 | 5,600 | — | — | — | — |
| iOS order, tail from `0x13c` on | 6,180 | 5,535 | — | — | — | — |
| scheduled order (`order_default.txt`) | 6,176 | 5,631 | 2,964 | — | — | — |
| original | 7,072 | — | — | 81 | 11 | 6 |

The reorder is structurally right (it fixes the shared-tail/common-tail partition:
`0x94/0x140/0x144/0x28c` move back into the shared tail and `0x204/0x2bc/0x2f8/0x30c..`
move into the common tail, exactly as the original) and raw improves by 172 bytes.
But it shrinks the function by ~800 B: the compiler then vectorises more
(57 `vst1` vs the original's 81) and the literal pool loses the duplicated-arm
constants.  The original's 7,072 B is a **less-vectorised schedule**, not the plain
iOS order.

## 3. `__builtin_expect` / branch-weight sweep (w9214 lever)

`__builtin_expect` was injected on the flag tests of the iOS-order body (the w9214
`!prof` producer).  All variants are worse than the plain reorder:

| expect mask | size | raw |
|---|---:|---:|
| none | 6,324 | **5,665** |
| `iPadHD=1` | 6,280 | 5,794 |
| `iPadHD=0` | 6,328 | 5,961 |
| `iPadHD=1,n9=0` | 6,328 | 5,805 |
| `iPadHD=1,n9=1` | 6,360 | 5,809 |
| all flags = 1 | 6,468 | 5,923 |
| `iPadHD=1,retina=0` | 6,304 | 5,795 |
| `iPadHD=1,iPad=0` | 6,312 | 5,834 |

So branch weights do not recover the original's less-vectorised 7,072 B layout here.

## 4. Static store counts (original vs HEAD)

| opcode | orig | HEAD |
|---|---:|---:|
| `str.w` | 103 | 182 |
| `strd` | 62 | 47 |
| `str` | 18 | 22 |
| `vst1.32` | 81 | 70 |
| `vstr` | 27 | 28 |
| `movs` | 778 | 719 |
| `mov.w` | 99 | 94 |

HEAD already emits *more* scalar stores than the original; the original's larger
`movs`/`mov.w` count is constant materialisation for its duplicated arms.

## 5. Not landed

`src/game/ui/Layout.cpp` is unchanged (7,072 / 5,704, gate green).  Candidate files
committed alongside this note:

* `LayoutC1-w9306-reorder-ios.cpp` — whole body in iOS source order (6,324 / 5,665)
* `LayoutC1-w9306-reorder-tail0x94.cpp` — head kept, tail from `0x94` in iOS order
  (6,260 / **5,532**, the best raw measured)
* `LayoutC1-w9306-reorder-sched.cpp` — scheduled order `order_default.txt` (6,176 / 5,631)

All are size-negative; landing one needs the original's ~800 B duplicated-arm schedule
reconstructed first.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9306
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9306/fdiff.py _ZN6LayoutC1Ev            # 7,072 / 5,704
python3 /dev/shm/w9306/align_diff.py _ZN6LayoutC1Ev       # 2,304
# store-list diff:
python3 /dev/shm/w9306/table.py _ZN6LayoutC1Ev <so> out.json
```

Scratch (`/dev/shm/w9306/`): `emu.py`, `table.py`, `trace.py`, `cmptrace.py`,
`stores.py`, `offs.py`, `hdr.py`, `order.py`, `reorder.py`, `reorder_tail.py`,
`reorder_def.py`, `splice.py`, `expvar.py`, `Layout.base.cpp`, `cand_*.cpp`,
`orig.dis`, `ours.dis`, `reord.dis`, `tp_*.txt`.

## 7. Open questions

* The original's 7,072 B needs a source shape that keeps the iOS-order partition but
  prevents the extra SLP vectorisation (81 vs 57 `vst1`).  The `0x1b0/0x1dc/0x2d8`
  HD-arm stores that HEAD leaves to the common tail are a concrete instance.
* The `0x184` ternary must be fixed (`iPad` before `n9`) in any future landing; it is a
  genuine semantic bug, currently masked by the size anchor.
