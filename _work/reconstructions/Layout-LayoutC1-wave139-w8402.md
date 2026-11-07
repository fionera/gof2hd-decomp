# `Layout::Layout` C1/C2 — wave 139 / worker w8402
## `0xdc` strd-vs-vst1 and `0x140..0x1d0` tail: the flip is reachable, but never size-neutral (note only)

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr `0xd1028`,
size **7072 B**, source `src/game/ui/Layout.cpp`.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`,
image base `0x10000`.  Baseline = wave-138 master `bf123bd47` (7072 B, target-function
same-address differing bytes **5,775**).

## Result — no source change landed; baseline re-confirmed

| metric | HEAD |
|---|---:|
| `_ZN6LayoutC1Ev` size | **7072** |
| target-function same-address diff bytes | **5,775** |
| `.text` size | unchanged |
| byte_exact / linked_exact | **4507 / 4507** |
| size_exact / data_exact | 4524 / 843 |
| imports / lint / drift | 207 / CLEAN / 0 |

`cmake --build cmake-build-match --target check -j6` -> **RATCHET PASS**.

## 1. Ground truth (re-confirmed)

Original HD arm, `d11c0..d11d4`:

```
d11c0  stmia.w fp, {r3, r6, r8}     ; 0xd0, 0xd4, 0xd8
d11c8  strd    r2, r4, [r0, #220]   ; 0xdc = 14.0625f, 0xe0 = 1.40625f
d11cc  movs    r2, #5
d11ce  strd    r1, r4, [r0, #228]   ; 0xe4 = 28.125f,  0xe8 = 1.40625f
```

The float pair is `(0xdc,0xe0)` then `(0xe4,0xe8)`, i.e. the natural pairing of four
adjacent `store float <const>` once SLP has declined the 4-wide group (verified with a
micro TU: four float-constant stores compiled with `-fno-slp-vectorize` emit exactly
`strd r1,r3,[r0]; strd r2,r3,[r0,#8]`).  Original literal pool has three scalar words
(`d1444`=1.40625, `d14b8`=28.125, `d14bc`=14.0625), not a 16-byte vector.

Original HD tail (`d11f2..d123e`) is ten 16-byte groups starting at
`0x140,0x150,0x160,0x170,0x180,0x190,0x1a0,0x1b0,0x1c0,0x1d0`.
Ours starts at `0x148` because `field_0x140`/`field_0x144` are emitted in the common arm.

## 2. NEW: the `0xdc` flip is reachable by moving the float group later in the BB

The wave-138 note's open question was whether a source shape can defeat SLP for the float
group.  It can: move the four float statements to a later point in the HD arm.  Then SLP's
cost model declines the 4-wide group and the backend emits the original's two `strd` pairs.

Anchor = statement after which the four float lines are inserted (ascending order
`0xdc,0xe0,0xe4,0xe8`).  Every entry is a real `link_so` build.

| float group placed after | `strd 0xdc/0xe0`? | size | diff bytes |
|---|---|---:|---:|
| **HEAD (scattered, lines 88/93/96/102)** | no (`vst1`) | **7072** | **5,775** |
| `field_0x2b4` | **yes** | 7024 | 5,841 |
| `field_0x110_strip58` | **yes** | 7040 | 5,990 |
| `field_0x2b8` | **yes** | 6992 | 5,972 |
| `field_0x12c` | **yes** | 6976 | 5,913 |
| `field_0x13c` | **yes** | 6920 | 5,840 |
| `field_0x14c` | **yes** | 6944 | 5,810 |
| `field_0x150` | **yes** | 6864 | 5,819 |
| `field_0x194` | **yes** | 6920 | 5,863 |
| `field_0x1a4` | **yes** | 6872 | 5,797 |
| `field_0x1c8` | **yes** | 6848 | 5,808 |
| `field_0x1c4` | **yes** | 6844 | 5,813 |
| `field_0x1d8` | **yes** | 6840 | 5,924 |

So the flip itself is understood.  The problem is that it is **never size-neutral**: the
`vst1` form is 8 B shorter than the two-`strd` form, and moving the group also perturbs the
common-arm SLP groups.  The best size at a flipping anchor is 7040 (-32 B) and the raw diff
regresses to 5,990.

## 3. The `0xec` group is the natural size counterweight — but it does not combine

Original `0xec` group is one `vst1` of `(56,112,281,562)`.  Ours is two `strd`
(`0xec/0xf0`, `0xf4/0xf8`).  Making the four `0xec..0xf8` statements contiguous ascending
in the source vectorizes the group:

| variant | size | diff bytes | `0xdc` flip? |
|---|---:|---:|---|
| HEAD | 7072 | 5,775 | no |
| `0xec` group contiguous | 7076 | 5,922 | no |
| `0xec` contiguous + float after `0x110` | 7024 | 5,869 | yes |
| `0xec` contiguous + float after `0x2b4` | 6984 | 5,842 | yes |
| `0xec` contiguous + float after `0x2b8` | 6968 | 5,819 | yes |

The `+4 B` from vectorizing `0xec` does not offset the `-24..-32 B` from moving the float
group, because the move also changes which of `0x294`/`0x2a4` vectorize.

## 4. Other new sweeps (all real builds)

* **All 24 permutations** of the float group at the `0x110` anchor: sizes 7040/7048/7056,
  diff 5,915-5,990.  Best flipping is 7040/5,975.
* **`0xb0`-alignment fix** (move `field_0xb0`/`field_0xb4`/`field_0xbc` so the SLP chain
  starts at `0xb0` instead of `0xb4`): 6960 B, diff 5,831, no flip.  Combined with the
  float move: 7056 B, diff 5,842, but the float group does **not** flip.
* **Full decompile-order reorder** of the whole `0xb0..0x134` block (using the original
  Ghidra store order `0xc0,0xb0,0xec,0x294,0x2a4,0x108,0x118,0xd0,0xdc,0xfc,0x2b4,0x128`):
  6960 B, diff 5,856, still `vst1`.
* **All 44 adjacent swaps** inside the `0xb0..0x108` block: none flips the float group;
  the size-7072 swaps are codegen no-ops (diff 5,775) or worse.
* **120 random 1-3 adjacent-swap walks**: every size-7072 result keeps `vst1`; every
  flipping result is < 7056 B.
* **Tail flip** (`0x140`/`0x144` before `0x148`): 7032 B, diff 5,939; combined with the
  float flip 6984 B, diff 5,951.

## 5. Conclusion

The `0xdc` flip is a real, reproducible SLP-boundary effect (move the float group later and
clang 7.0.2 emits the original's two `strd` pairs).  But at the current body it always costs
24-32 B and shifts the literal pool, so the raw same-address diff rises from 5,775 to
5,9xx.  Reaching the original's 7072 B with the flip requires the wave-135 common-arm/tail
redistribution (original tail 2534 B vs ours 2348 B, original common arm 2434 B vs ours
2602 B), which is a device-major restructuring, not a local store-group reorder.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w8402
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w8402/fdiff.py _ZN6LayoutC1Ev          # 7072/7072, diff 5775
```

Scratch harnesses (not committed, `/dev/shm/w8402/`): `fdiff.py` (size + raw same-address
diff), `variant.py`/`multi.py` (single/multi statement move + build + score),
`sweep*.py` (anchor sweeps), `perm*.py` (24-permutation sweeps), `adj*.py` (adjacent-swap
sweeps), `rand.py` (random walks), `reorder.py` (decompile-order reorder),
`Layout.base.cpp` (pristine wave-138 body).

## 7. Open questions

* The flip is size-negative only because the current body is already size-exact at 7072.
  The missing 24-32 B must come from the common-arm/tail duplication (wave-135 residual).
  A device-major tail rebuild that reproduces the original's 2534-B tail would let the
  `0xdc` and `0x140` flips land at 7072.
* The `0x294` and `0x2a4` SLP groups are the coupling lever: every float-group move that
  flips `0xdc` also changes whether `0x294` vectorizes.  Pinning those two groups first
  (e.g. by making their source runs contiguous) may decouple the size budget.
