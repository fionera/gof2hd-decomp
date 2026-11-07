# `Layout::Layout` C1/C2 — wave 140 / worker w8508
## `0xdc` strd-vs-vst1 flip: defeat SLP by interleaving one scalar int store (flip at 7072 B found; +85 raw diff, not landed)

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr `0xd1028`,
size **7072 B**, source `src/game/ui/Layout.cpp`.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`,
image base `0x10000`.  Baseline = wave-139 master `d4857f174` (7072 B, target-function
same-address differing bytes **5,765**).

## Result — no source change landed; baseline re-confirmed

| metric | HEAD |
|---|---:|
| `_ZN6LayoutC1Ev` size | **7072** |
| target-function same-address diff bytes | **5,765** |
| `.text` size | unchanged |
| byte_exact / linked_exact | **4507 / 4507** |
| size_exact / data_exact | 4524 / 843 |
| imports / lint / drift | 207 / CLEAN / 0 |

`cmake --build cmake-build-match --target check -j6` -> **RATCHET PASS**.

## 1. NEW hypothesis that works: interleave one scalar int store inside the float group

w8402 established that the `0xdc` group flips to the original's two `strd` pairs only when the
four float assignments are *moved* to a later point in the HD arm, and that every such move is
size-negative (7024..7048 B).  The open question was whether the flip can be obtained without
moving the group.

It can.  The four float stores (`0xdc`, `0xe0`, `0xe4`, `0xe8`) are sunk by clang to the end of
the HD block and land contiguous in the pre-SLP store list, so SLP merges them into one
`<4 x float>` literal (`vst1`).  If **one non-vectorizable scalar int store is interleaved
between the float assignments in source**, it survives in the store list between the floats and
breaks the 4-chain; the backend then emits the original's two `strd` pairs.  No float statement
moves, so the literal pool is not shifted by the move itself.

Verified with a base-register-tracked disassembly check (`strd ... [#220]` present):

| source shape | size | diff bytes | `0xdc` strd? |
|---|---:|---:|---|
| HEAD | 7072 | **5,765** | no |
| floats contiguous ascending after `0xcc` | 7024 | 5,818 | **yes** |
| floats contiguous descending after `0xcc` | 7024 | 5,842 | **yes** |
| `0xdc`, `field_0xf8`, then `0xe0/0xe4/0xe8` | 7068 | 5,861 | **yes** |
| `0xdc`, `field_0x2b4`, then rest | 7032 | 5,848 | **yes** |
| `0xdc`, `field_0x2b8`, then rest | **7072** | 5,906 | **yes** |
| `0xdc`, `ringMidFarThreshold`, then rest | 7000 | 5,835 | **yes** |
| `0xdc`, `field_0x2a8_metricB`, then rest | 7048 | 5,881 | **yes** |
| `0xdc`, `field_0xb0`-class single store, then rest | 6992 | 5,850 | **yes** |
| `0xdc`, `field_0x298_buttonYBias`, then rest | 6984 | 5,917 | **yes** |
| `0xdc`, `field_0x12c`, then rest | 7032 | 5,944 | **yes** |

**Companion that restores 7072 B:** `interleave field_0xf8` + make the `0xec` group contiguous
(`ringNearMidThreshold, field_0xf0, ringMidFarThreshold, field_0xf8`) gives
**7072 B with the `0xdc` strd flip, diff 5,850**.
`interleave field_0x2b8` alone also gives **7072 B with the flip, diff 5,906**.

So the flip *is* reachable at size-exact 7072 B.  The blocker is the raw diff: every flipping
shape costs **+85..+180** same-address bytes versus HEAD.

## 2. Tail group (`0x140..0x1d0`) + float flip

Moving `field_0x140`/`field_0x144` into the tail source order (before `field_0x13c`) flips the
HD tail to the original's `0x140/0x150/0x160/...` grouping (verified: `add r1,r0,#320` present)
and, combined with the float move, flips `0xdc` too.  Every combination is size-negative:

| source shape | size | diff bytes | `0xdc` | tail `0x140` |
|---|---:|---:|---|---|
| HEAD | 7072 | **5,765** | no | no |
| tail flip alone | 7032 | 5,909 | no | **yes** |
| tail + float@`0x110` | 6984 | 5,950 | **yes** | **yes** |
| tail + float@`0x14c` | 6848 | **5,720** | **yes** | **yes** |
| tail + float@`0x1c4` | 6788 | **5,705** | **yes** | **yes** |
| tail + float@`0x1d8` | 6788 | **5,705** | **yes** | **yes** |
| tail + float@`0x1c8` | 6792 | 5,730 | **yes** | **yes** |

Note the sub-5,765 numbers at 6,7xx B are *not* comparable: the raw metric is computed over
`min(orig,ours)` bytes, so a 284-B-shorter body trivially has fewer differing bytes.  At exactly
7072 B the flip floor is 5,850.

## 3. Where the residual diff actually lives (region histogram, HEAD)

| region | bytes | differing |
|---|---:|---:|
| arms (HD + n9 + default) `0x000..0x2e0` | 736 | 640 |
| literal pool `0x2e0..0x540` | 608 | 223 |
| **common arm `0x540..0x1880`** | 4928 | **4591 (93 %)** |
| all-device tail `0x1880..0x1ba0` | 800 | 311 |

The flip variant moves the histogram to arms 660 / common 4663 / tail 304 — i.e. it slightly
*worsens* the two regions that dominate.  The common arm, not the `0xdc`/tail store groups, is
the real byte-matching blocker (wave-135 residual: our common arm is ~168 B too big and the
tail ~186 B too small).  Any flip landing must be paired with a common-arm/tail redistribution
that is diff-positive by more than the flip's ~85 B cost.

## 4. Conclusion

* The `0xdc` flip is now understood at the source level: interleave one scalar int store in the
  float run; SLP declines the 4-chain and the backend emits `strd` pairs.
* A size-exact 7072 B flipping shape exists (`interleave field_0xf8` + `0xec` contiguous,
  diff 5,850), but it regresses the primary raw metric by 85 B, so HEAD is kept.
* The tail `0x140..0x1d0` flip is cheap to trigger and also size-negative; it is the same
  common-arm/tail redistribution problem.
* Next lever: reproduce the original's 2534-B tail (per-device duplication) so the tail grows
  toward 7072 B while the common arm shrinks; then the flip becomes diff-neutral.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w8508
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w8508/fdiff.py _ZN6LayoutC1Ev          # 7072/7072, diff 5765
```

Scratch harnesses (not committed, `/dev/shm/w8508/`): `fdiff.py` (size + raw same-address diff),
`lib.py` (build/score/move/contiguity/flip-check helpers), `exp1..exp9.py` (the sweeps above),
`parse_ir.py` (pre-SLP store-order dump), `region.py` (region diff histogram),
`Layout.base.cpp` (pristine wave-139 body).

## 6. Open questions

* The flip's +85 B diff cost is concentrated in the arms/common arm, not the literal pool
  (pool diff is unchanged at 223).  A companion that improves the common arm by >85 B would
  let the flip land net-positive.
* `interleave field_0x2b8` is the only *single* move that hits 7072 with the flip; its diff is
  5,906.  Combining it with common-arm reorders is the cheapest next search space.
