# `Gun::calcCharacterCollision` wave 191b (w13700): the 455-byte raw residual is
# a single 2-byte i-loop shift plus the k-loop block order; no landed win

Worker: w13700. Branch `wave191b-w13700`, base master `6e208a911`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`,
660 insns. Scratch: `/home/debian/GalaxyOnFire2/scratch/w13700`.

## 0. Result

**No `src/` change landed.** The committed body remains the raw optimum:

| build | raw | aligned | size | insns | `i*3` | `this` |
|---|---:|---:|---:|---:|---|---|
| committed (HEAD `6e208a911`) | **455** | **93** | 1984/1984 | 660/660 | r5 | r9 |
| faithful C-D-E `expect(mag>dist,0)` | 463 | **57** | 1984/1984 | 660/660 | r5 | r9 |

`check` is green on the untouched tree: byte 4508, linked 4508, size 4524,
stub 0, missing 0, extra 0, data 843/843, imports 207, lint CLEAN, drift 0,
`RATCHET PASS`.

`git status --porcelain` is empty apart from this note.

## 1. New evidence (a): the 226-byte i-loop raw blob is a pure 2-byte shift

The committed raw diff (455 B) decomposes into exactly six runs:

| region | bytes | cause |
|---|---:|---|
| `0xdf,0xea,0xfe` | 3 | `i*3` in r5 vs r4 |
| `0x13c,0x142,0x14c` | 3 | `i*3` r5/r4 + scatter slot swap |
| `0x1ca..0x204` | 7 | branch offsets from the 2-byte shift |
| `0x208..0x2ec` | **226** | 2-byte shift in the i-loop |
| `0x438..0x466` | 44 | view-block rematerialisation |
| `0x494..0x578` | 117 | k-loop block order |

The 226-byte blob is **not** 226 independent differences: the whole region is
shifted by 2 bytes because ours emits the 16-bit `add r4,sp,#104` at
`0x153198`/`0x153262` where the original emits the 32-bit
`ldr.w fp,[sp,#40]` (reload of `i`), and ours compensates with one extra
32-bit `ldr.w fp,[sp,#40]` at `0x153278`.  Same total size, different
instruction at two sites:

```
orig 153198  ldr.w fp,[sp,#40]     ; reload i
ours 153198  add   r4,sp,#104      ; materialise &view (2 B)
orig 153262  ldr.w fp,[sp,#40]     ; reload i
ours 153260  add   r4,sp,#104      ; materialise &view (2 B)
orig 153278  ldr   r0,[pc,#132]
ours 153276  ldr   r0,[pc,#136]
ours 153278  ldr.w fp,[sp,#40]     ; extra reload
```

The original's `i` reload at `0x153198` is the reaching definition for the
`this->lifetimes[i]` store at `0x153346` (fp stays live across the whole
damage block); ours reloads `i` later at `0x153278`.  `&view` is needed at
`0x153198` because the k-loop's `getPosition` sret is at `sp+104` and is
reached on the `geometries == nullptr` path.  Removing that reaching def
(i.e. giving the k-loop sret its own slot) is the only way to kill the
shift, and every such shape is size-inexact (see §3).

## 2. New evidence (b): the faithful body's +8 is entirely the k-loop

The faithful C-D-E body's raw diff is byte-identical to the committed body's
in **every** region except the k-loop; the k-loop run grows from 117 to 125
bytes (+8).  The body instructions are the same, only the block order
differs: ours lays the loop out condition-at-top (cold then-block
out-of-line), the original body-first:

```
orig 153440  movs r5,#0 ; b 1534fc          ; body-first
     15347a  cmp r0,r1 ; ble 1534fa         ; then-block fallthrough
     1534fa  adds r5,#1 ; ldr;ldr;cmp;bcc body
ours 153440  ldr;ldr;cmp;bcs.w exit          ; condition-at-top
     153482  cmp r0,r1 ; bgt then
     153484  adds r5,#1 ; b cond
```

`expect(magnitude > distance, 0)` is what makes the then-block cold; removing
it (or `expect(...,1)`) gives body-first but flips `this` to r4 and shrinks
the function to 1908 B (the `this`-in-a-low-register 16-bit `ldr/str`
savings).  The original is body-first **and** `this=r9`, which no measured
source shape reproduces.

Guard sweep on the C-D-E body (all size-exact 1984 / 660 insns):

| guard | raw | aligned |
|---|---:|---:|
| `magnitude > distance` (faithful) | 463 | **57** |
| `distance < magnitude` | 463 | 57 |
| `distance - magnitude < 0` | 463 | 57 |
| `magnitude >= distance + 1` | 463 | 57 |
| `magnitude != distance` | 463 | 57 |
| `magnitude - distance > 0` | **458** | 63 |
| `(magnitude - distance)` | 1042 | 105 (1980 B) |

Best C-D-E raw is 458 (`mag-dist>0`), but that guard is *not* faithful: it
computes `subs r1,r0,r1; cmp r1,#1` in the guard, whereas the original does
`cmp r0,r1; ble` and `subs r0,r0,r1` inside the then-block.  Still 3 B short
of 455.

## 3. New evidence (c): the r4/r5 swap is use-count driven; every flip is
## size-inexact

MIR `Live Interval Analysis` (committed base, `-print-after-all`):

| vreg | meaning | def | last use | size |
|---|---|---:|---:|---:|
| `%308` | `t2ADDri %stack.10` (`&view`) | 1568 (bb.5, hoisted) | 16176 | **14608** |
| `%99` | `t2ADDrs %16,%16,10` (`i*3`) | 2880 | 15712 | **12832** |

`%308` is one CSE-merged vreg used at the scatter getPosition sret (3616),
the scatter operator- arg (3728), the view memcpy/MatrixSetTranslation/
TransformSetLocal (14704/14896/15072) and the k-loop getPosition/operator=
(16080/16176).  `RAGreedy` enqueues by size and gives it r4; `i*3` falls to
r5.  To flip it `%308` must fall below 12832:

* prevent the LICM hoist: def moves 1568 -> 3616 => 12560 (would flip), or
* separate the k-loop sret: last use 15072 => 13504 (not enough), or
* separate the view too: last use 3728 => 2160 (would flip).

Every source shape that achieves one of these re-runs `MachineBlockPlacement`
and changes the size.  Measured this pass (all with `this=r9` unless noted):

| variant | raw | aligned | size | `i*3` |
|---|---:|---:|---:|---|
| `expect(geometries != nullptr, 1)` | 1608 | 265 | **1956** | r4 |
| `expect(scatter, 0)` on the i-loop scatter branch | 1828 | 311 | **1992** | r4 |
| `expect(mine, 1)` on the i-loop mine branch | 1835 | 303 | **1996** | r4 |
| `expect(enemy->isAsteroid(), 0)` | 1813 | 376 | **1980** | r4 (this=r1) |
| `expect(explosive, 1)` | 1828 | 490 | **1948** | r4 (this=r1) |
| `Vector kpos;` at i-loop top, `kpos = getPosition()` in k-loop | 1739 | 345 | **1976** | r4 |
| `alignas(16) Vector playerPosition` | 1859 | 245 | **1996** | r5 |
| `volatile int radius` | 1657 | 342 | **1972** | r4 (this=r1) |
| `hitvec` before `playerGun` | 1444 | 165 | **1980** | r4 |

Body-first (`continue` / no-expect) family, all `this=r4`, all size-inexact:

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `if (diff <= 0) continue;` C-D-E | 1888 | 263 | **1908** |
| + `expect(mine,0)` i-loop | 1778 | 166 | **1928** |
| + `expect(mine,1)` i-loop | 1817 | 751 | **1896** |
| + `expect(scatter,0)` | 1815 | 750 | **1896** |
| + `expect(scatter,1)` | 1728 | 194 | **1924** |
| + `expect(geometries,1)` | 1821 | 286 | **1900** |
| no expect | 1886 | 257 | **1908** |
| no expect + `expect(mine,1)` e-loop | 1747 | 325 | **1908** |

## 4. Faithful-body compensating shapes (all size-exact 1984 unless noted)

| variant | raw | aligned |
|---|---:|---:|
| faithful C-D-E | 463 | **57** |
| + `alignas(8) Matrix view` | 463 | 57 |
| + `alignas(8) Vector playerPosition` | 463 | 57 |
| + `const` position/velocity | 463 | 57 |
| + `Matrix *vp = &view` | 463 | 57 |
| + `expect(impact != nullptr, 1)` | 463 | 57 |
| + `expect(playerGun, 1)` | 463 | 57 |
| + `expect(egoFlag, 1)` | 463 | 57 |
| + `expect(empDamage != kNoAttribute, 1)` | 465 | 60 |
| + swap the two `SetAnimationState` calls | 465 | 59 |
| + diff-first (`Vector diff = enemyPosition - getPlayer()->getPosition()`) | 467 | 67 |
| + diff-first + `alignas(8) view` | 467 | 67 |
| + `expect(owner == nullptr || ..., 1)` | 541 | 79 |
| + `expect(owner == nullptr || ..., 0)` | 562 | 92 |
| + `expect(mine, 1)` e-loop | 672 | 101 |
| + `expect(mine, 0)` e-loop | 1681 | 212 |
| + impact block before geometries block | 1219 | 123 |

None recovers the 8-byte raw cost.

## 5. Angle (c): the collision-test operand order is already matched

The main collision test `enemyPosition.{x,y,z} - position.{x,y,z} +
velocity.{x,y,z}` (`0x1531b8..0x153254`) and the mine test
(`0x15351c..0x1535aa`) are byte-identical between the committed build and the
original (the raw diff contains no bytes in those ranges except the §1
shift).  The compiler reassociates into `(enemyPosition.x - position.x) +
velocity.x` and compares against `radius` and `-radius`, exactly the
original's `vsub/vadd/vcmpe` sequence.  No operand-order change is available.

## 6. Codegen-identical i-loop/k-loop shapes (455/93/1984)

`Vector diff = enemyPosition - playerPosition;` (named diff, playerPosition
still first), `const Vector playerPosition`, `Vector *pp = &playerPosition`,
`volatile Vector playerPosition` (folded), `{ Vector playerPosition; ... }`
extra scope, `const Vector &playerPosition`, `long radius`, `long distance`,
`unsigned long i`, `unsigned long e`, `i = i + 1`, `i != count`,
`i <= count-1`, `int i`, `k = k + 1`, `k != size`, `int k`,
`for (unsigned i = 0, i3 = 0; ...; ++i, i3 += 3)`, `bool explosive`,
`char explosive`, `unsigned weaponType`, cached `positions.count`,
`k`/`enemy`/`e` declared outside the loop, `expect(impact != nullptr, 1)`,
`expect(egoFlag, 1)`, `expect(playerGun, 1)`, `expect(enemies == nullptr, 0)`,
and a `if (this->lifetimes[i] == 0x7fffffff) {}` read after the collision
test (optimised away).

## 7. `.ARM.extab` / `.ARM.exidx`

Per-function sections from the `Gun.cpp.obj` (`objdump -h`):

| build | `.text` | `.ARM.extab` | `.ARM.exidx` |
|---|---:|---:|---:|
| committed base | 1984 | 16 | 8 |
| faithful C-D-E | 1984 | 16 | 8 |

The faithful body is extab-neutral, so its only cost is the 8 raw bytes
(`.text` differing-byte delta 71,340 -> 71,348).

## 8. Conclusion

The residual is two independent items:

1. **The i-loop 2-byte shift (226 raw bytes, 50% of the total).** Ours
   materialises `&view` (16-bit `add`) where the original reloads `i`
   (32-bit `ldr.w`), and compensates with an extra `ldr.w` later.  Fixing it
   requires either preventing the LICM hoist of `%308` or separating the
   k-loop sret from the view slot; both are reachable but size-inexact
   (1956-1996 B).
2. **The k-loop block order (117 raw bytes).** The original is body-first;
   our cold-then `expect(diff>0,0)` rotates the loop.  Body-first requires
   dropping the cold expect, which flips `this` to r4 and shrinks the
   function to 1908 B.

The faithful C-D-E body is a free aligned win (93 -> 57) at +8 raw and is
recorded here for a future pass that values aligned.  The committed body
stays (raw 455 / aligned 93 / 1984 B).

## 9. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13700
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN3Gun22calcCharacterCollisionEv
# committed base -> raw=455 aligned=93 size=1984/1984 insn=660/660
```

MIR probe (scratch, 12 GB ulimit, output to disk):

```sh
clang++ <Gun.cpp compile line from compile_commands.json> \
  -mllvm -print-after-all -mllvm -filter-print-funcs=_ZN3Gun22calcCharacterCollisionEv \
  -o /home/debian/GalaxyOnFire2/scratch/w13700/gun_mir.o 2> mir.txt
# Live Interval Analysis: %308 def 1568 (bb.5) last use 16176 -> 14608
#                         %99  def 2880        last use 15712 -> 12832
```

## 10. Files changed

`_work/reconstructions/gun-calccollision-wave191b-w13700.md` only.
