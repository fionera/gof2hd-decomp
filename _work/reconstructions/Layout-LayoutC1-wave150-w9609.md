# `Layout::Layout` C1/C2 — wave 150 / worker w9609
## `0x148` joins the `0x13c..0x148` SLP group at exact size 7,072 B by moving the
## four-statement group *after* `0x16c` (not to the front).  Group forms
## (`add.w r1,r0,#316; vst1.32 {d16-d17},[r1]`) and the function stays 7,072 B
## without any tail/device-major reconstruction.  Raw 5,768 -> 5,808,
## aligned 2,198 -> 2,270 (both slightly worse; gate green).

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Baseline = wave-149 HEAD `7eed2966e` + w9503/w9408 landed reorders: raw
same-address diff **5,768**, aligned **2,198**, size 7,072.  Gate on entry and
exit: byte_exact 4507/4524, linked_exact 4507/4524, size 4524/4524, data
843/843, imports 207, lint CLEAN, `RATCHET PASS`.

## 1. What was already known (w9503)

* The original stores `0x13c,0x140,0x144,0x148` with one `vst1.32
  {d16-d17}` at `d1d26` (function offset `0xcfe`); `0x14c` is a scalar
  `str.w` immediately before it.
* Moving `0x140/0x144/0x148` to the front (source order `13c,140,144,148,14c`)
  forms the group but **shrinks** the function 7,072 -> 7,008 B (-64).
* The full ascending (iOS-order) tail also forms the group but collapses to
  6,804 B because the original's duplicated device-arm literal pool (242 vs
  194 words) is missing.
* Open question: find a source shape that puts `0x148` in the group *without*
  the tail, or with a smaller tail, at exactly 7,072 B.

## 2. New hypothesis: the group does not have to sit at the front

The group is formed by the SLP vectorizer from any four *consecutive*
statements that write `0x13c,0x140,0x144,0x148`.  It does **not** have to be
placed at the start of the tail.  I swept the position of the four-statement
block `[0x13c,0x140,0x144,0x148]` through every slot of the tail (all
statements after it keep the wave-149 order).  One placement is exactly
size-neutral:

| placement of the block | size | raw | aligned |
|---|---:|---:|---:|
| front (variant A, w9503) | 7,008 | 5,874 | 2,224 |
| after `0x154` | 7,044 | 5,908 | 2,316 |
| after `0x168` | 7,052 | 5,844 | 2,581 |
| **after `0x16c`** | **7,072** | **5,879** | 2,307 |
| after `0x178` (u112) | 6,956 | 5,804 | 2,309 |
| after `0x1a0` (u124) | 6,952 | 5,766 | 2,778 |

So placing the block after `0x16c` is the unique size-neutral position in this
sweep.  In the emitted code the group is at `d1f50`:

```
d1f50: f500 719e  add.w  r1, r0, #316   ; 0x13c
d1f54: f941 0a8f  vst1.32 {d16-d17}, [r1]  ; 0x13c,0x140,0x144,0x148
```

The first `0x164` store moves from `0xed8` (baseline) to `0xea2`, i.e. 0x36 B
closer to the original's `0xdf4`; the remaining 0xae is still the missing
device-major tail.

## 3. Local refinement at exact 7,072 B

Starting from the after-`0x16c` placement, an adjacent-swap hill climb over the
tail statements (accept only size == 7,072, minimise raw+aligned) found one
further swap: `field_0x2c0` and `field_0x1d0` exchanged.  Result:

| metric | baseline | landed |
|---|---:|---:|
| size | 7,072 | **7,072** |
| raw same-address diff | 5,768 | **5,808** (+40) |
| align_diff (non-matching insns) | 2,198 | **2,270** (+72) |
| first `0x164` store | `0xed8` | **`0xea2`** |
| `0x13c..0x148` SLP group | absent | **present** |

The raw and aligned metrics are slightly worse because moving the group away
from the front re-schedules code that the wave-149 hill climb had tuned; the
structural target (the group, and a `0x164` block 0x36 B closer) is met at the
mandatory exact size.  No other size-7,072 variant found in the sweep improved
either metric.

## 4. Landed source change

Five statements move; no expression changes:

* `field_0x13c`, `field_0x140`, `field_0x144`, `field_0x148` are emitted
  consecutively immediately after `field_0x16c` (was: `0x13c` first, `0x140`
  and `0x144` at the end of the constructor, `0x148` between `0x154` and
  `0x158`).
* `field_0x2c0` and `field_0x1d0` swapped.

All are independent `this->field = ...` assignments; `0x158` still reads
`0x154` after it is written.

## 5. Gate

```
verify: byte 4507  linked 4507  stubs 0  missing 0  extra 0  avg 99.93
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0  imports: 207
RATCHET PASS
```

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9609
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9609/fdiff.py _ZN6LayoutC1Ev        # 7072/7072, diff 5808
python3 /dev/shm/w9609/align_diff.py _ZN6LayoutC1Ev   # 2270
cmake --build cmake-build-match --target check -j6    # RATCHET PASS
```

Scratch (`/dev/shm/w9609/`): `gen.py` (constructor statement parser /
permutation emitter), `measure.py`, `place2.py` (group-position sweep),
`climb2.py`, `order_final.txt`, `final.cpp`, `oursFinal.dis`.

## 7. Open questions

* The group is now present but emitted at `d1f50`, *after* the `0x164..0x16c`
  block; the original emits it at `d1d26` *before* `0x164`.  Getting the group
  to `0xcfe` needs the code before it to be ~0x280 B larger, i.e. the
  duplicated device-major tail (same blocker as w9503).
* The full ascending tail is still the structurally right source order; it is
  268 B short only because of the missing 48 literal-pool words.
