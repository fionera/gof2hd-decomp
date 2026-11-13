# `Gun::calcCharacterCollision` wave 191b (w13736): the residual is one
# greedy-RA priority decision; ~90 new size-exact reshapes all land on 455

Worker: w13736. Branch `wave191b-w13736`, base master `22c4b25f5`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`,
660 insns. Scratch: `/home/debian/GalaxyOnFire2/scratch/w13736`.

## 0. Result

**No `src/` change landed.** The committed body remains the raw optimum:

| build | raw | aligned | size | insns | `.text` | `.ARM.extab` | `.ARM.exidx` |
|---|---:|---:|---:|---:|---:|---:|---:|
| committed (HEAD `22c4b25f5`) | **455** | **93** | 1984/1984 | 660/660 | 0x7c0 | 0x10 | 0x8 |
| faithful C-D-E (`magnitude > distance`) | 463 | **57** | 1984/1984 | 660/660 | 0x7c0 | 0x10 | 0x8 |
| `expr` scatter form | 459 | 103 | 1984/1984 | 660/660 | 0x7c0 | 0x10 | 0x8 |
| `expr` + faithful C-D-E | 467 | 67 | 1984/1984 | 660/660 | 0x7c0 | 0x10 | 0x8 |

`git status --porcelain` is empty apart from this note. The gate is untouched
(the tree is byte-for-byte the base).

## 1. Byte-diff decomposition (re-confirmed this pass)

`python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so
_ZN3Gun22calcCharacterCollisionEv` -> `raw=455 aligned=93 size=1984/1984
insn=660/660`. 51 raw runs, 455 bytes:

| region (function-relative) | bytes | cause |
|---|---:|---|
| `0x0df,0x0ea,0x0fe` | 3 | `i*3` in r5 (ours) vs r4 (orig) |
| `0x13c,0x142,0x14c` | 3 | `i*3` r5/r4 + scatter slot swap |
| `0x1ca` | 1 | `i*3` |
| `0x1de..0x204` | 7 | branch displacements from the 2-byte phase shift |
| `0x208..0x2ec` | 226 | 2-byte phase shift in the i-loop |
| `0x438..0x466` | 44 | view-block rematerialisation |
| `0x494` | 1 | branch displacement |
| `0x4a8` | 1 | `i*3` |
| `0x4b0..0x576` | 117 | k-loop block placement (post-RA rotation) |

The 226-byte blob is a pure phase shift: ours emits a 2-byte
`add r4,sp,#104` at `0x208`/`0x2d0` where the original emits the 4-byte
`ldr.w fp,[sp,#40]` (`i` reload), and ours compensates with one extra 4-byte
`ldr.w fp,[sp,#40]` at `0x2e8`. Same total size, different instruction at
three sites.

## 2. New evidence: the RA priority competition, measured from the MIR

`-mllvm -print-after-all -mllvm
-filter-print-funcs=_ZN3Gun22calcCharacterCollisionEv` on the committed body.
After `Live Interval Analysis` the long-lived vregs that compete for the
callee-saved low registers are (span = last use slot - def slot):

| vreg | meaning | def | last use | span | uses | final reg |
|---|---|---:|---:|---:|---:|---|
| `%152` | `&position` (`%stack.1`) | 1440 | 16256 | 14816 | 5 | `sl`/r10 |
| `%16`  | `i` | 2768 | 17504 | 14736 | 9 | `fp`/r11 |
| `%308` | `&%stack.10` (view/delta/kpos) | 1568 | 16176 | **14608** | 7 | **r4** |
| `%282` | `&%stack.11` (playerPosition/MSTR) | 1648 | 14880 | 13232 | 3 | (remat) |
| `%99`  | `i*3` | 2880 | 15712 | **12832** | 6 | **r5** |
| `%291` | `&velocity` (`%stack.2`) | 1552 | 13760 | 12208 | 1 | (remat) |

`%308` is 1776 bytes longer than `%99`, so greedy RA enqueues it first and it
takes r4; `i*3` falls to r5. That one swap is the entire residual: it produces
the six `i*3` bytes, the two 2-byte remats (and hence the 226-byte phase
shift), the view-block rematerialisation, and (post-RA) the un-rotated k-loop.

The frame-object map is unchanged from w13588: `fi#10` (60 B, align 8) =
`Matrix view` at `sp+104`, `fi#11` (60 B, align 4) = `MatrixSetTranslation`
sret at `sp+44`; `StackColoring` merges the 12-byte `delta` temp into
`fi#10` and `playerPosition` into `fi#11` (the `expr`/`diff_first` alloca
order), giving the original's slot assignment but not its registers.

## 3. Cells measured this pass (all `link_so` + `tools/fn_diff.py`, `-j2`)

Size-exact 1984 / 660 insns unless stated; none beats 455.

| variant | raw | aligned |
|---|---:|---:|
| committed | **455** | **93** |
| faithful C-D-E, `expect(magnitude > distance, 0)` | 463 | **57** |
| faithful C-D-E + `expr` scatter | 467 | 67 |
| faithful C-D-E + `expect(k < size, 1)` | 1800 | 355 (1948 B) |
| faithful C-D-E + `expect(k < size, 0)` | 1717 | 303 (1944 B) |
| `expr` scatter | 459 | 103 |
| `expr` + `alignas(8) Matrix view` | 459 | 103 |
| `alignas(8) Vector delta` (named, copy-elided) | 478 | 125 |
| `alignas(16) Vector delta` | 1859 | 249 (1996 B) |
| `alignas(16) Vector kpos` / `playerPosition` / `view` | 1859 | 245 (1996 B) |
| `alignas(8) Vector kpos` / `view` / `playerPosition` | 455 | 93 |
| `unsigned radius` | 459 | 99 |
| `float radius` | 1829 | 552 (1908 B) |
| `short`/`char`/`unsigned short`/`long long radius` | 1537-1815 | 167-383 (2000-2004 B) |
| `reorder enemyPosition,velocity,position` | 500 | 117 |
| `reorder position,enemyPosition,velocity` | 538 | 140 |
| `Vector &position` / `Vector &velocity` | 1812 / 1665 | 350 / 357 (1912/1968 B) |
| `Vector *posData = positions.data()` cache | 455 | 93 |
| both positions/velocities `data()` cached | 477 | 97 |
| `Vector *pp = positions.data() + i` | 455 | 93 |
| `collision test z,y,x` | 455 | 99 |
| `velocity.x + enemyPosition.x - position.x` | 1399 | 149 (1992 B) |
| `int outer` instead of `bool outer` | 1341 | 153 (1976 B) |
| `const Vector` position/velocity/enemyPosition (any/all) | 455 | 93 |
| `Vector position(positions.data()[i])` copy-ctor | 455 | 93 |
| `long distance`, `short distance` | 455 | 93 |
| `hitPositions.data()[i] = positions.data()[i]` | 1059 | 130 (1988 B) |
| `volatile int dv` instead of `const volatile int &dv` | 1327 | 135 (1988 B) |
| `Matrix view[1]` + `view[0]` | 1138 | 201 (1988 B) |
| `Matrix view;` at i-loop top + `operator=` | 1880 | 393 (2024 B) |
| `Matrix view = ...` hoisted to i-loop top | 1876 | 482 (1904 B) |
| `Vector kpos;` at i-loop top + `operator=` | 1744 | 309 (1976 B) |
| `*td = kpos = enemy->getPosition()` | 1739 | 275 (1976 B) |
| `enemyPosition = enemy->getPosition(); *td = enemyPosition` | 1739 | 202 (1992 B) |
| `Vector diff = enemyPosition - getPlayer()->getPosition()` (`diff_first`) | 467 | 67 |
| swap `impact` and `scatter` blocks | 1784 | 251 (1972 B) |
| `Matrix view` hoisted before `if (explosive)` | 1821 | 315 (1964 B) |
| swap the two `SetAnimationState` calls | 457 | 95 |
| cache `TransformGetTransform` result | 1174 | 142 (1968 B) |
| cache `this->enemies` / `this->level` / `td` as reference | 455 | 93 |
| cache `this->impact` | 1673 | 212 (1972 B) |
| mine-branch `expect` on y or z axis (instead of x) | 665 | 190 (659 insns) |
| mine-branch `expect(...,0)` | 984 | 345 (2000 B) |
| main collision test `expect(...,1)` / `expect(...,0)` | 1396 / 1511 | 169 / 257 (1980/1984 B) |
| `expect(diff > 0, 1)` | 1887 | 281 (1908 B) |
| `expect(diff >= 1, 0)` / `expect(diff != 0, 0)` | 455 / 1047 | 93 / 135 (1984/1980 B) |
| `int`/`unsigned long` `i`, `int` `e`, `int` `k` | 455 | 93 |
| `lifetimes[i] = 0xfff0bdc0`, `hitFlags[i] = (bool)1` | 455 | 93 |
| `setBombForce(0)`, `ignite()` unqualified, `owner == 0` | 455 | 93 |

Every frame-neutral (208-byte) reshape is either codegen-identical to the
committed body (455) or moves the whole stream; every shape that separates the
`view`/k-loop stack objects grows the frame (>= 1996 B) or shrinks it
(1904-1976 B). The 455 plateau is stable.

## 4. Conclusion

The residual is a single greedy-RA priority decision (`%308` 14608 B takes r4,
`i*3` 12832 B falls to r5). It is upstream of the 2-byte phase shift, the
view-block rematerialisation and the post-RA k-loop rotation, so all three
downstream clusters move together with it. No source shape found this pass
changes the priority order while keeping the 208-byte frame and the 1984-byte
size. The committed body stays at **raw 455 / aligned 93 / 1984 B**.

The faithful C-D-E body (463 / **57** / 1984 / 660, `.text` 0x7c0,
`.ARM.extab` 0x10, `.ARM.exidx` 0x8) remains the best aligned candidate and is
recorded here for a future pass that values aligned over raw.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13736
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN3Gun22calcCharacterCollisionEv
# committed base -> raw=455 aligned=93 size=1984/1984 insn=660/660
```

Per-function section sizes (`arm-linux-androideabi-objdump -h
cmake-build-match/CMakeFiles/gof2.dir/src/game/weapons/Gun.cpp.obj`):

```
.text._ZN3Gun22calcCharacterCollisionEv            000007c0
.ARM.extab.text._ZN3Gun22calcCharacterCollisionEv  00000010
.ARM.exidx.text._ZN3Gun22calcCharacterCollisionEv  00000008
```

## 6. Files changed

`_work/reconstructions/gun-calccollision-wave191b-w13736.md` only.
