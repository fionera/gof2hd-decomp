# `Gun::calcCharacterCollision` wave 191b (w13602): the faithful C-D-E body is
# an aligned win (93 -> 57) but a raw regression (455 -> 463); note only

Worker: w13602. Branch `wave191b-w13602`, base master `49a44a262`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`,
660 insns. Scratch: `/home/debian/GalaxyOnFire2/scratch/w13602`.

## 0. Result

**No `src/` change landed.** The committed body remains the raw optimum:
**raw 455 / aligned 93 / 1984 B**, 660/660 insns, verify pct **90.2**.

The faithful C-D-E body (the original's actual instruction order) is
**raw 463 / aligned 57**, verify pct **93.4**, size exact. It is a clear
*aligned* win but a raw regression, and waves w13365/w13388 already traded
aligned (63 -> 84 -> 93) for raw (458 -> 457 -> 455) on purpose, so the base
is kept. The candidate is recorded here for a future pass that values the
aligned metric.

`git status --porcelain` is empty apart from this note.

## 1. New evidence: the original's k-loop body order is C-D-E, not E-C-D

The original's then-block (disassembly `0x15347c..0x1534f6`) is:

```
15347c  vmov s0, r0            ; magnitude
153480  subs r0, r0, r1        ; diff = magnitude - distance
153486  vmov s2, r0
15348a  vcvt.f32.s32 s0, s0    ; (float) magnitude
15348e  vcvt.f32.s32 s2, s2    ; (float) diff
153492  vdiv.f32 s0, s2, s0    ; falloff = diff / magnitude
153496  vcmpe.f32 s0, s20      ; clamp: if (falloff > 1.0f)
15349e  bgt  1534b2
1534a0  vcmpe.f32 s0, #0.0     ; else if (falloff < 0.0f)
1534a8  vmov.f32 s30, s0
1534ac  it mi / vmovmi.f32 s30, s22
1534b2  ... damage(falloff * damage, ...)
1534d4  ... if (empDamage >= 1) damageEmp(falloff * empDamage, ...)
```

i.e. **clamp -> damage -> damageEmp (C-D-E)**. The committed source is
E-C-D (`damageEmp` before the clamp), which is also *semantically* different:
it applies `damageEmp` with the **unclamped** falloff. The original clamps
first. w13365/w13388 moved the committed body E-C-D purely to chase raw bytes.

Guard: the original compares `magnitude` (r0) with `distance` (r1) directly
(`0x153478 cmp r0, r1; 0x15347a ble`) and computes `diff` *inside* the
then-block, so the faithful guard is `__builtin_expect(this->magnitude >
distance, 0)` with `int diff = this->magnitude - distance;` inside.

## 2. New evidence: why `%308` wins r4 (MIR live-interval sizes)

From the `Live Interval Analysis` dump (`li.txt`, committed base):

| vreg | meaning | def | last use | size |
|---|---|---:|---:|---:|
| `%308` | `t2ADDri %stack.10` (`&view` slot) | 1568 (bb.5) | 16176 | **14608** |
| `%282` | `t2ADDri %stack.11` (`&mst` slot) | 1648 (bb.5) | 14880 | **13232** |
| `%99` | `t2ADDrs %16,%16,10` (`i*3`) | 2880 | 15712 | **12832** |

`RAGreedy::enqueue` orders global ranges by `(1u<<29)+Size`, so `%308` is
dequeued first and takes **r4**; `%99` (`i*3`) gets **r5**. The original is
the opposite (`i*3` = r4, `&view` rematerialised into r5). To flip it,
`%308` must shrink below 12832 B, i.e. its def must move later than bb.5
(not hoisted) or its last use (the k-loop `Player::getPosition()` sret at
16176) must move earlier. Both `%308` and `%282` are `t2ADDri %stack.N`
frame addresses, which `MachineLICM` always hoists (trivially
rematerialisable) and `EliminateCSE` merges into one vreg; `Machine code
sinking` does not sink them (multi-block uses). No source shape found that
prevents the hoist without changing the frame size.

The diff-first slot swap (`Vector diff = enemyPosition - getPlayer()->
getPosition();`) reproduces the original's slot *contents* (fi#11 =
playerPosition+mst, fi#10 = diff+view) but still gives `%308` r4 because
`%308`'s interval is the longest. Measured 459/103.

## 3. Cells measured this pass (all `link_so` + `tools/fn_diff.py`, `-j2`)

Size-exact 1984 / 660 insns unless stated; none beats raw 455.

### 3.1 faithful k-loop body (clamp -> damage -> damageEmp)

| guard | raw | aligned | size | insns |
|---|---:|---:|---:|---:|
| committed E-C-D, `expect(diff > 0, 0)` | **455** | **93** | 1984 | 660 |
| C-D-E, `expect(diff > 0, 0)` | 458 | 63 | 1984 | 660 |
| **C-D-E, `expect(magnitude > distance, 0)`** | **463** | **57** | 1984 | 660 |
| C-D-E, `expect(distance < magnitude, 0)` | 463 | 57 | 1984 | 660 |
| C-D-E, `expect(magnitude - distance > 0, 0)` | 458 | 63 | 1984 | 660 |
| C-E-D, `expect(diff > 0, 0)` | 457 | 84 | 1984 | 660 |
| C-E-D, `expect(magnitude > distance, 0)` | 462 | 78 | 1984 | 660 |
| D-C-E, `expect(diff > 0, 0)` | 463 | 83 | 1984 | 660 |
| D-C-E, `expect(magnitude > distance, 0)` | 466 | 80 | 1984 | 660 |
| E-D-C, `expect(diff > 0, 0)` | 1794 | 171 | **1940** | 648 |
| C-D-E, no `expect` | 1886 | 257 | **1908** | 646 |
| C-D-E, `expect(..., 1)` | 1886 | 257 | **1908** | 646 |

`int diff` inside vs outside the `if`, `float falloff = (float)(magnitude -
distance) / magnitude`, and a cached `int mag = this->magnitude` are all
codegen-identical to C-D-E (`463/57`).

Section deltas for the two aligned candidates (base `.text` 71,202,
`.ARM.extab` 577, `.ARM.exidx` 0, `.rodata` 0):

| build | raw | aligned | .text | .ARM.extab | verify pct |
|---|---:|---:|---:|---:|---:|
| committed base | 455 | 93 | 71,202 | 577 | 90.2 |
| C-D-E + `diff > 0` guard | 458 | 63 | 71,205 | 577 | — |
| C-D-E + `magnitude > distance` guard | 463 | 57 | 71,210 | 577 | 93.4 |

### 3.2 scatter-radius-block shapes (committed body)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `Vector diff = enemyPosition - playerPosition;` (both named) | 455 | 93 | 1984 |
| `const Vector playerPosition = ...` | 455 | 93 | 1984 |
| `const Vector &playerPosition = ...` | 455 | 93 | 1984 |
| `enemyPosition -= playerPosition;` | 1461 | 188 | **1980** |
| `Vector diff = enemyPosition; diff -= playerPosition;` | 1553 | 229 | **1992** |
| manual `Vector diff(x-x, y-y, z-z)` | 1825 | 221 | **2016** |

### 3.3 positions/velocities locals (committed body)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `const Vector position/velocity` | 455 | 93 | 1984 |
| copy-ctor `Vector position(this->positions.data()[i])` | 455 | 93 | 1984 |
| `const Vector &position = this->positions.data()[i]` | 1787 | 415 | **1920** |
| `enemyPosition` declared before `position` | 538 | 137 | 1984 |
| `unsigned distance` | 455 | 94 | 1984 |

### 3.4 cached module/state pointers (committed body)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `PaintCanvas *canvas = Globals::Canvas;` in geometries block | 1785 | 213 | **1964** |
| `int *geoms = this->geometries;` in geometries block | 1424 | 182 | **1960** |
| `int *geoms = ...` at function scope | 1677 | 261 | **1968** |
| `canvas` at function scope | 1865 | 288 | **1896** |

## 4. Conclusion

The residual is still the single greedy-RA decision of w13576/w13588: `%308`
(`&view` slot, 14608 B) takes r4 and pushes `i*3` (12832 B) to r5. The only
new, measured lever this pass is the k-loop body order: the original is
C-D-E, and compiling C-D-E improves the instruction-stream alignment from
93 to 57 (verify 90.2 -> 93.4) at the cost of raw 455 -> 463 and `.text`
71,202 -> 71,210 (`.ARM.extab` 577 unchanged). Because w13365/w13388
deliberately traded aligned for raw to reach 455, the base is kept and the
faithful form is recorded here.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13602
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN3Gun22calcCharacterCollisionEv
# committed base -> raw=455 aligned=93 size=1984/1984 insn=660/660
```

Faithful C-D-E body (raw 463 / aligned 57 / 1984 exact):

```c++
int distance = (int) AbyssEngine::AEMath::VectorLength(*td);
if (__builtin_expect(this->magnitude > distance, 0)) {  // lint: arm_builtin
    int diff = this->magnitude - distance;
    float falloff = (float) diff / (float) this->magnitude;
    if (falloff > 1.0f) {
        falloff = 1.0f;
    } else if (falloff < 0.0f) {
        falloff = 0.0f;
    }
    enemy->damage((int) (falloff * (float) this->damage), this->friendGun, this->itemIndex);
    if (this->empDamage > 0) {
        enemy->damageEmp((int) (falloff * (float) this->empDamage), this->friendGun);
    }
}
```

## 6. Files changed

`_work/reconstructions/gun-calccollision-wave191b-w13602.md` only.
