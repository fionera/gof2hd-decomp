# `Layout::Layout` C1/C2 — wave 143 / worker w8806
## Literal-pool alignment analysis: pre-pool split is HD arm +12 B, n9 arm +4 B; pool is 24 B short

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr `0xd1028`,
size **7072 B**, source `src/game/ui/Layout.cpp`.  Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`, image base 0x10000.
Baseline = wave-142 master `bf083ab41` (target-function raw same-address diff **5,704**,
`.text` differing bytes **85,119**).

## 1. Hypothesis

The original's literal pool starts at **0x2d8**; ours at **0x2e8** (16 B late).  If the
pre-pool code shrank by 16 B, every pool-relative `add rX, pc, #N` immediate in the HD and
n9 arms would match the original, and the common tail would shift onto the original's
addresses.  The mission was to find the extra 16 B and keep the function at 7072 B.

## 2. Result — measured, **not landed** (no size-neutral win found)

The source is unchanged from wave 142; the target diff stays **5,704** at 7,072 B.
All experiments below were built with `link_so` and scored with a raw same-address diff
(`/dev/shm/w8806/fdiff.py`, `lib.py`).

## 3. Where the 16 B lives (exact)

Disassembly region sizes (orig vs ours):

| region | orig | ours | delta |
|---|---:|---:|---:|
| prologue (`d1028..d1042`) | 26 | 26 | 0 |
| HD arm (`d1042..d1268`/`d1274`) | 550 | 562 | **+12** |
| n9 arm (`d126c..d12fc`/`d1278..d130c`) | 144 | 148 | **+4** |
| **pre-pool total** (`d1028..pool`) | **728 (0x2d8)** | **744 (0x2e8)** | **+16** |
| literal pool | 624 (0x270) | 600 (0x258) | **−24** |
| retina arm (`d1570..d1860`/`d1568..d186e`) | 752 | 774 | **+22** |
| common tail (`d1860..end`/`d186e..end`) | 4968 | 4954 | **−14** |

So the 16 B is **12 B in the HD (iPadHD) arm + 4 B in the n9 arm**, not one 16 B block.
Instruction-mix census: HD arm orig 51×2 B + 112×4 B; ours 55×2 B + 113×4 B; n9 arm
orig 16×2 + 28×4; ours 16×2 + 29×4.

The HD-arm excess is a **store-packing** difference, not literal-pool padding:

* original packs `0x94,0x98,0x9c,0xa0` into one `stmia.w r4, {r5,r8,ip,lr}` (4 B);
  ours uses individual `str.w` for `0x98/0x9c/0xa0` (and stores `0x94` in the common tail);
* original packs `0xd0,0xd4,0xd8` into `stmia.w fp, {r3,r6,r8}`; ours uses `strd 0xd4,0xd8`;
* original packs `0x2c0,0x2c4,0x2c8,0x2cc` into `stmia.w ip!, {r1,r2,r3,r6}` in the n9 arm;
  ours uses `str.w 0x2c4` + `str.w 0x98` + `strd 0x2cc,0x2d0`.

The SLP vector groups also differ in alignment:

| original group | ours group |
|---|---|
| `0xc0..0xcc` | `0xb4..0xc0` + `0xc4..0xd0` |
| `0xec..0xf8` | `0xdc..0xe8` |
| `0x140..0x14c`, `0x150..0x15c`, `0x160..0x16c`, `0x170..0x17c` | `0x148..0x154`, `0x158..0x164`, `0x168..0x174` (+8 B shift) |
| `0x108..0x114` then `0x118..0x124` | `0x118..0x124` then `0x108..0x114` |
| `0x24..0x30` then `0x34..0x40` | `0x34..0x40` then `0x24..0x30` |

The pool **content order** also differs (the original's pool is 6 words larger and its
first vectors are `[9,11,5,253]`=0xc0..0xcc, `[1,21,78,151]`=0x128..0x134,
`[15,18,2,189]`=0x140..0x14c, …; ours starts `[20,25,11,9]`, `[9,35,21,1]`, …), so
aligning the pool start alone would not make the pool bytes match.

## 4. Experiments (all measured, reverted)

| variant | size | raw diff |
|---|---:|---:|
| HEAD (wave 142) | 7072 | **5,704** |
| move `0xc0,0xc4,0xc8,0xcc` group after `0xbc` | 7072 | 5,705 |
| sort statements `0..21` by field offset | 7060 | 5,964 |
| sort statements `7..21` | 7056 | 5,914 |
| sort statements `96..111` (0x140..0x17c region) | 6996 | 5,779 |
| move `0x94,0x98,0x9c,0xa0` together after `0x90` | 7000 | 5,867 |
| move `0x24..0x30` group after `0x34..0x40` group | 7048 | 5,920 |
| move `0x108..0x114` group after `0x118..0x124` group | 7008 | 5,848 |
| move `0x2c0,0x2c4,0x2c8,0x2cc` together after `0x98` | 7088 | 5,978 |
| remove the `volatile unsigned char *rd` reload hack | 7064 | 5,843 |
| rewrite `field_0x2d4` if/else as one ternary (drops `rd`) | 7056 | 5,838 |
| `retinaDisplay \|\| n9` → `n9 \|\| retinaDisplay` | 7072 | 5,704 (no change) |

Every store-packing fix that moves the source toward the original's `stmia` groups
**shrinks** the function (7000..7060 B), because the same source order also changes the
n9/retina arms and the common tail; the original's 7072 B therefore needs all four arms
reconstructed together, not a local reorder.

## 5. w8608 `0xdc` strd flip on the 5,704 shape — tested, not landed

The current shape stores the four floats `0xdc,0xe0,0xe4,0xe8` as one `vst1.32 {d2-d3}`
(16 B vector in the pool at `0x460..0x46c`).  Re-running w8608's flip on the 5,704 order:

| variant | size | raw diff |
|---|---:|---:|
| flip group (`0xdc,0x2b8,animSpeedScale,driftSpeedDivisor,ringRadiusScale`) after `0xcc` | 7024 | 5,988 |
| flip group + `0xf0,0xf8` after `0xcc` | 7024 | 5,945 |
| flip group after `0xc8` | 7056 | **5,847** |
| flip group without `0x2b8` after `0xcc` | 6992 | 5,866 |
| flip `0xdc,0x2b8,animSpeedScale` only after `0xcc` | 7016 | 5,973 |

The strd form is reproduced but costs 48..80 B of function size and 143..284 diff bytes;
none is size-neutral at 7072 B.  Not landed.

## 6. Move climb from 5,704 with new seeds — no improvement

Statement-level move-to-position climbs from the 5,704 order (203 top-level statements,
`size == 7072` gate, strict raw-diff improvement, plateau walk on equal diff):

| seed | move set | iterations | best |
|---|---:|---:|---:|
| 101 | ±1..20 single moves | 350 | 5,704 |
| 202 | random single moves + 50 % plateau | 250 | 5,704 |
| 303 | single moves inside statements 39..95 | 220 | 5,704 |
| 404 | contiguous block moves (2..5 statements) | 130 | 5,704 |
| 505 | contiguous block moves (2..5 statements) | 100 | 5,704 |

Exhaustive adjacent-swap sweeps (all `size == 7072`, keep only if diff < 5,704):

| statement range | swaps | best |
|---|---:|---:|
| 4..21 (early ternaries / SLP groups) | 18 | 5,704 (swap 20, equal) |
| 95..134 (0x140..0x17c region) | 40 | 5,704 (swap 107, equal) |
| 130..201 (tail statements) | 72 | 5,704 (swap 133, equal) |

The wave-142 basin is exhausted for this move set; all new seeds hit the same 5,704 floor.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w8806
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8806/fdiff.py _ZN6LayoutC1Ev     # 7072/7072, diff 5704
```

Scratch harnesses (`/dev/shm/w8806/`, not committed): `lib.py` (statement splitter +
build/score), `apply.py` (move/swap spec), `sortblock2.py` (block sort by field offset),
`climb2/3/4/5.py` (move climbs), `dis.py` / `words.py` / `stores.py` / `lcsbytes.py`
(region, pool-word, store-sequence and byte-weighted LCS diffs), `fdiff.py` (raw diff).

## 8. Open questions

* The dominant residual is the **common tail** (0x838..0x1ba0: 4,210 of 5,704 diff bytes)
  and the **retina arm** (630/752).  Both are conditional-move (`it eq`) / `vbsl`
  ternary code; the original selects with `it eq` where ours selects with `it ne` and a
  different operand order.  Reconstructing the exact ternary expression shape is the
  next big lever, not statement reordering.
* The original's `stmia` packing of `0x94..0xa0`, `0xd0..0xd8` and `0x2c0..0x2cc` must be
  reproduced *together* with the pool-vector alignment if the 7,072 B size is to hold.
* The literal pool is 24 B short and its content order differs; matching it requires the
  original's whole-constructor statement order, which the wave-141/142 climbs scrambled
  in exchange for byte wins.
