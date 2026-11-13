# `Gun::calcCharacterCollision` wave 191b (w13708): slot-merge / lifetime /
# body-first-k-loop sweep; the hoisted `&view` frame address is still the residual

Worker: w13708. Branch `wave191b-w13708`, base master `d4e8f032d`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`,
660 insns. Scratch: `/home/debian/GalaxyOnFire2/scratch/w13708`.

## 0. Result

**No `src/` change landed.** The committed body remains the raw optimum:

| build | raw | aligned | size | insns | `.text` | `.ARM.extab` | `.ARM.exidx` |
|---|---:|---:|---:|---:|---:|---:|---:|
| committed (HEAD `d4e8f032d`) | **455** | **93** | 1984/1984 | 660/660 | 0x7c0 | 0x10 | 0x8 |
| best new candidate (`expr`, shipped slots) | 459 | 103 | 1984/1984 | 660/660 | 0x7c0 | 0x10 | 0x8 |
| body-first `do{}while` | 1027 | 156 | **1988**/1984 | 662/660 | 0x7c4 | 0x10 | 0x8 |

`check` is green on the untouched tree: byte 4508, linked 4508, size 4524,
stub 0, missing 0, extra 0, data 843/843, imports 207, lint CLEAN, drift 0,
`RATCHET PASS`.

`git status --porcelain` is empty apart from this note.

## 1. The residual, restated from this pass's evidence

The raw diff still decomposes exactly as w13700/w13674 described:

| region | bytes | cause |
|---|---:|---|
| `0xdf,0xea,0xfe` | 3 | `i*3` in r5 vs r4 |
| `0x13c,0x142,0x14c` | 3 | `i*3` r5/r4 + scatter slot swap |
| `0x1ca..0x204` | 7 | branch offsets from the 2-byte shift |
| `0x208..0x2ec` | **226** | 2-byte shift in the i-loop |
| `0x438..0x466` | 44 | view-block rematerialisation |
| `0x494..0x578` | 117 | k-loop block order |
| `0x67a,0x68e,0x6aa` | 3 | `i*3` r5/r4 in the mine-branch tail |

The single root cause is still that `Merge disjoint stack slots` merges the
scatter `playerPosition` (fi#4), the `Matrix view` (fi#10, 60 B) and the k-loop
`enemy->getPosition()` sret (fi#12) into one `%stack.10`. `Early Machine LICM`
then hoists `t2ADDri %stack.10` to the i-loop preheader (`%308`, def 1568),
`Machine CSE` folds the view-block and k-loop addresses into it, and
`RAGreedy` gives the 14608-B interval **r4**; `i*3` (12832 B) falls to **r5**.
Because the address is one long vreg, the view block rematerialises it three
times (`add r0,sp,#104` / `add r1,sp,#104` / `add r4,sp,#104`) instead of the
shipped's one `add r5,sp,#104` + two `mov rX,r5`, and the `i` reload is pushed
from `0x153198` to `0x153278`.

## 2. Angle (a): the `expr` form gets the shipped *slots* but not the registers

`VectorLength(enemyPosition - getPlayer()->getPosition())` (no named
`playerPosition`) reproduces the shipped frame contents exactly:

| value | shipped | `expr` | committed (`stmt`) |
|---|---|---|---|
| `playerPosition` | r4 / sp+44 | r5 / sp+44 | r4 / sp+104 |
| `delta` | r5 / sp+104 | r4 / sp+104 | r5 / sp+44 |
| `i*3` | r4 | r5 | r5 |

The `expr` build is **459 / 103 / 1984** — the base's 455 plus 4 bytes: the
scatter block is the shipped instruction sequence with every r4<->r5 field
swapped (`mov r6,r5` vs `mov r6,r4`, `add r5,sp,#44` vs `add r4,sp,#44`,
`add r4,sp,#104` vs `add r5,sp,#104`, `mov r2,r5` vs `mov r2,r4`,
`mov r0,r4` vs `mov r0,r5`). So the slot assignment is not the blocker; the
LICM hoisting order (`stack.10 -> r4`, `stack.11 -> r5`) is. The shipped needs
`stack.11 -> r4`, i.e. neither address hoisted.

## 3. Angle (b): body-first k-loop spellings

The shipped k-loop is `[body, guard, then, latch, header]` (then-block hot,
header at the end). Measured spellings on the committed body:

| k-loop spelling | raw | aligned | size | insns | layout |
|---|---:|---:|---:|---:|---|
| committed `for(;;++k) expect(diff>0,0)` | **455** | **93** | 1984 | 660 | header-first, then cold |
| `while(true){ if(k>=size)break; ...; ++k; }` | 455 | 93 | 1984 | 660 | identical |
| `for(;;){ if(k>=size)break; ... }` | 455 | 93 | 1984 | 660 | identical |
| `for(;k<size;){ ...; ++k; }` | 455 | 93 | 1984 | 660 | identical |
| `do{ ...; ++k; }while(k<size)` | 1027 | 156 | **1988** | 662 | body-first, guard after then |
| `for(;;++k){ ...; if(k+1>=size)break; }` | 1027 | 156 | **1988** | 662 | body-first, guard after then |
| `if(k<size) do{...}while(k<size)` | 1812 | 331 | 1952 | 652 | body-first |
| plain `if(magnitude>distance)` | 1885 | 277 | **1908** | 646 | body-first, `this`=r4 |
| `expect(diff>0,1)` | 1887 | 281 | **1908** | 646 | then hot, no rotation |

The `do{}while`/`for-break` forms genuinely rotate the loop (body first) but
add the loop-entry `b.n` (+4 B) and put the guard *after* the then-block,
where the shipped has it *before* (body falls through into the guard, then the
then-block). The plain `if` gives the shipped rotation but drops the cold
anchor, so the whole RA map moves (`this`=r4, 1908 B).

### 3.1 Cold-anchor relocation (plain k-loop + `expect(...,0)` elsewhere)

Every attempt to move the spill anchor out of the k-loop guard changed the
size away from 1984:

| anchor | raw | size | insns |
|---|---:|---:|---:|
| `expect(mine,0)` on `radius*=5` | 1416 | **1980** | 658 |
| `expect(mine,0)` on the e-loop mine filter | 1882 | 1912 | 647 |
| `expect(geometries!=nullptr,0)` | 1383 | **1972** | 657 |
| `expect(geometries!=nullptr,0)` + `expect(impact!=nullptr,0)` | 1397 | **1980** | 660 |
| `expect(impact!=nullptr,0)` | 1871 | 1920 | 651 |
| `expect(explodeOnContact,0)` | 1886 | 1908 | 645 |
| `expect(empDamage!=kNoAttribute,0)` | 1862 | 1912 | 647 |
| `expect(owner==nullptr||...,0)` | 1889 | 1908 | 646 |
| `expect(scatter,0)` | 1816 | 1896 | 650 |
| `expect(enemies==nullptr,0/1)` | 1885 / 1859 | 1908 / 1916 | 646 / 654 |
| `expect(!isActive(),1)` | 1857 | 1876 | 640 |
| `expect(getHitpoints()<1,1)` | 1856 | 1876 | 640 |
| `expect(!egoFlag&&...,0/1)` | 1876 / 1807 | 1900 / 1928 | 652 / 653 |
| `expect(isAsteroid(),0/1)` | 1878 / 1796 | 1924 / 1968 | 657 / 653 |
| `expect(playerGun,0/1)` | 1867 / 1885 | 1920 / 1908 | 651 / 646 |
| `expect(!(outer&&...),0)` | 1898 | 1904 | 659 |
| `expect(geometries!=nullptr,1)` | 1818 | 1900 | 655 |
| `expect(impact!=nullptr,1)` | 1885 | 1908 | 646 |
| `expect(empDamage!=kNoAttribute,1)` | 1885 | 1908 | 646 |
| `expect(owner==nullptr||...,1)` | 1878 | 1908 | 646 |
| `expect(explosive,0/1)` | 1843 / 1407 | 1912 / **1976** | 653 / 658 |
| `expect(scatter,1)` on the final scatter block | 1819 | 1888 | 648 |

No relocation lands on 1984 with the plain k-loop.

## 4. Angle (c): `&view` lifetime / sret-path separation

To stop the merge, the k-loop sret (and/or the scatter delta) must have a
frame index whose live range overlaps `view`. Every lifetime-extension shape
changes the size:

| variant | raw | size | insns |
|---|---:|---:|---:|
| `Vector kpos;` before `if(explosive)`, assign in k-loop | 1793 | 1920 | 662 |
| `Vector kpos;` before the collision test | 1746 | **1976** | 666 |
| `Vector kpos;` at i-loop top | 1739 | **1976** | 666 |
| `Vector kpos = position;` at i-loop top | 1796 | **1976** | 665 |
| `Vector kpos = position;` before `if(explosive)` | 1884 | 1928 | 665 |
| `alignas(8) Vector kpos;` at i-loop top | 1739 | **1976** | 666 |
| `alignas(16) Vector kpos;` at i-loop top | 1634 | 1988 | 670 |
| `Vector delta;` at i-loop top (assign in scatter) | 1668 | **1964** | 661 |
| `Vector delta = position;` at i-loop top | 1774 | **1968** | 662 |
| `Vector delta;` at function scope | 1595 | **1964** | 661 |
| `delta` + `kpos` at i-loop top | 1823 | **2000** | 674 |
| `delta` + `kpos` at function scope | 1790 | 1988 | 669 |
| `expr` + `Vector kpos = position;` at i-loop top | 1796 | **1976** | 665 |

The 1976 cluster is the "k-loop rotated, i-loop still shifted" layout: the
default ctor / copy costs 6 insns but the rotated k-loop saves ~14 B, so the
function is 8 B short and the whole stream moves (raw ~1740). None is 1984.

### 4.1 View-block spellings (all codegen-identical, 455/93/1984)

`Matrix *const vp = &view`, `Matrix &vp = view`, `(void)MatrixSetTranslation`,
`Matrix translated = MatrixSetTranslation(...); (void)translated`, nested
scope around the `MatrixSetTranslation` call, `const Matrix *cam` + copy,
`alignas(8) Matrix view`, `Matrix view(*(Matrix*)...)`, and caching
`this->positions.count` are all **455 / 93 / 1984 / 660** — the view address
is still rematerialised because `%308` is the same hoisted vreg.

`alignas(16) Matrix view` -> 1859 / 245 / **1996**; `view` moved before the
two `SetAnimationState` calls -> 1162 / 156 / **1988**; the 2-arg
`MatrixSetTranslation(view, Vector(...))` -> 1822 / 343 / **1940**.

## 5. Angle (a) again: moving the `i` reload

Reordering the `lifetimes[i]` / `lastHitKIPlayer` stores to force an earlier
`i` reload:

| variant | raw | size | insns |
|---|---:|---:|---:|
| `lifetimes[i]` before `isAsteroid` | 1388 | **1980** | 658 |
| `lifetimes[i]` after the empDamage block | 1333 | 1984 | 660 |
| `lastHitKIPlayer` after the empDamage block | 1425 | **1980** | 658 |
| both after the empDamage block | 1395 | 1988 | 662 |
| `hitPositions[i]` after the k-loop | 1859 | 1952 | 659 |

`lifetimes[i]` after the empDamage block is size-exact but moves the whole
stream (raw 1333), so it is not a win.

## 6. Other measured levers (all 455/93/1984/660)

`explodeOnContact = bomb || mine;`, the ternary form, `Vector *const td`,
`Vector &td`, `expect` on the e-loop / i-loop conditions, `expect` on the
clamp branches (`falloff>1.0f` / `falloff<0.0f`, 0 or 1), `expect(diff>=1,0)`,
`expect(0<diff,0)`, `expect(diff,0)>0`, `expect(diff,0)>0` with weight 1,
`expect(magnitude>distance,0)` reordered as E-C-D (`guard_maggt`: 460/89),
`float`/`char`/`bool` spellings, and the `for`/`while`/`for(;;)` header forms
are all codegen-identical to the committed body.

## 7. `.ARM.extab` / `.ARM.exidx`

Per-function sections from `Gun.cpp.obj`:

| build | `.text` | `.ARM.extab` | `.ARM.exidx` |
|---|---:|---:|---:|
| committed base | 0x7c0 (1984) | 0x10 (16) | 0x8 (8) |
| `expr` (459/103, 1984) | 0x7c0 | 0x10 | 0x8 |
| `do{}while` (1027/156, 1988) | 0x7c4 | 0x10 | 0x8 |

The extab is 16 B for every candidate measured this pass; no candidate grows
it.

## 8. Conclusion

The residual is unchanged: the single hoisted `%stack.10` frame address
(`playerPosition` + `view` + k-loop sret merged by `Merge disjoint stack
slots`) takes r4 and pushes `i*3` to r5. The shipped has neither address
hoisted, which requires separating `playerPosition`, `delta` and the k-loop
sret into distinct frame indices; every source shape that does so (lifetime
extension, `alignas`, `expr` form, body-first `do{}while`) moves the function
off 1984 B. The committed body stays at **raw 455 / aligned 93 / 1984 B**.

The `expr` form is the closest structural match to the shipped frame contents
(playerPosition=sp+44, delta=sp+104) but is 4 raw bytes worse (459) because
the LICM hoisting order transposes r4/r5. The `do{}while` form is the only
body-first spelling that keeps the cold anchor, but it is 1988 B.

## 9. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13708
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN3Gun22calcCharacterCollisionEv
# committed base -> raw=455 aligned=93 size=1984/1984 insn=660/660
```

Per-function section sizes:

```sh
arm-linux-androideabi-objdump -h \
  cmake-build-match/CMakeFiles/gof2.dir/src/game/weapons/Gun.cpp.obj | \
  grep -A1 calcCharacterCollision
```

## 10. Files changed

`_work/reconstructions/gun-calccollision-wave191b-w13708.md` only.
