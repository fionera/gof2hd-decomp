# `Gun::calcCharacterCollision` wave 191b (w13628): expect-weight, slot-separation
# and hoist-prevention sweep; the `%308`/`i*3` r4/r5 swap is still the residual

Worker: w13628. Branch `wave191b-w13628`, base master `ebd084c5e`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`,
660 insns. Scratch: `/home/debian/GalaxyOnFire2/scratch/w13628`.

## 0. Result

**No `src/` change landed.** The committed body remains the raw optimum:

| build | raw | aligned | size | insns |
|---|---:|---:|---:|---:|
| committed (HEAD `ebd084c5e`) | **455** | **93** | 1984/1984 | 660/660 |
| faithful C-D-E `expect(mag>dist,0)` | 463 | **57** | 1984/1984 | 660/660 |

`check` is green on the untouched tree: byte 4508, linked 4508, size 4524,
stub 0, missing 0, extra 0, data 843/843, imports 207, lint CLEAN, drift 0,
`RATCHET PASS`.

`git status --porcelain` is empty apart from this note.

## 1. The residual is exactly the `%308` / `i*3` r4/r5 swap

The original's i-loop keeps `i*3` in **r4** and rematerialises the
`sp+104` slot address into scratch registers (`add r5,sp,#104` in the view
block, `add r4,sp,#104` in the k-loop). Our committed build keeps the
hoisted `%308 = t2ADDri %stack.10` in **r4** and `i*3` in **r5**, so every
`ldr.w fp,[sp,#40]` (i reload) in the original is an `add r4,sp,#104`
rematerialisation in ours. That one decision accounts for the bulk of the
455 raw bytes; the k-loop body order is secondary.

Live-interval sizes (committed build, `-mllvm -print-after-all`):

| vreg | meaning | def | last use | size |
|---|---|---:|---:|---:|
| `%308` | `t2ADDri %stack.10` (hoisted to bb.5) | 1568 | 16176 | **14608** |
| `%99` | `i*3` | 2880 | 15712 | **12832** |

`RAGreedy` dequeues `%308` first (size-descending) and gives it r4; `i*3`
falls to r5. To flip it, `%308` must fall below 12832 (prevent the hoist:
def moves to the first use at 3616 -> 12560, margin only 272) or `i*3` must
grow past 14608 (impossible: the i-loop body ends at the k-loop).

## 2. Expect-weight experiments (angle a)

`__builtin_expect_with_probability` does **not exist** in clang 7.0.2
(`use of undeclared identifier`). `__builtin_expect` is strictly 0/1 and
every then-hot form loses the 1984-B RA (drops to 1908/1920):

| k-loop guard / body | raw | aligned | size | insns |
|---|---:|---:|---:|---:|
| committed E-C-D `expect(diff>0,0)` | **455** | **93** | 1984 | 660 |
| faithful C-D-E `expect(mag>dist,0)` | 463 | **57** | 1984 | 660 |
| C-D-E `expect(mag>dist,1)` (then hot) | 1886 | 257 | **1908** | 646 |
| E-C-D `expect(diff>0,1)` (then hot) | 1887 | 281 | **1908** | 646 |
| C-E-D `expect(mag>dist,0)` | 462 | 78 | 1984 | 660 |
| C-E-D `expect(mag>dist,1)` | 1891 | 263 | **1908** | 646 |
| C-D-E `expect(mag-dist>0,0)` | 458 | 63 | 1984 | 660 |
| C-D-E `expect(dist<mag,0)` | 463 | 57 | 1984 | 660 |
| C-D-E plain (no expect) | 1886 | 257 | **1908** | 646 |
| C-D-E local `bool hit` + `expect(hit,0)` | 463 | 57 | 1984 | 660 |
| C-D-E `expect(mag>dist,0) || expect(mag>dist,1)` | 463 | 57 | 1984 | 660 |
| C-D-E `expect(mag>dist,0) && expect(mag>dist,1)` | 463 | 57 | 1984 | 660 |
| C-D-E nested cold-outer/hot-inner | 1886 | 257 | **1908** | 646 |
| C-D-E nested hot-outer/cold-inner | 463 | 57 | 1984 | 660 |
| C-D-E inverted `expect(diff<=0,1)` empty-then | 458 | 63 | 1984 | 660 |

The compound `||`/`&&` forms fold to the first (cold) expect; nested forms
either fold cold or go hot. No partial weight is expressible.

## 3. Slot-separation experiments (angle b)

Separating the k-loop sret from the view slot **does** flip `i*3` to r4, but
only by adding a `Vector` whose default ctor costs 6 instructions; the
function shrinks to 1976 and the whole layout shifts (raw ~1740):

| variant | raw | aligned | size | insns | `i*3` |
|---|---:|---:|---:|---:|---|
| `Vector kp;` at i-loop top, `kp = getPosition()` in k-loop | 1739 | 348 | **1976** | 666 | **r4** |
| same + faithful C-D-E body | 1731 | 324 | **1976** | 666 | **r4** |
| `Vector kp = position;` at i-loop top, assign in k-loop | 1796 | 354 | **1976** | 665 | r4 |
| `Vector kp = enemyPosition;` at i-loop top | 1795 | 348 | **1976** | 665 | r4 |
| `Vector kp = *this->positions.data()` at i-loop top | 1616 | 358 | **1980** | 666 | r4 |
| `Vector kp = getPosition(); *td = kp;` inside k-loop | 455 | 93 | 1984 | 660 | r5 |

The in-k-loop named temp is codegen-identical to the committed body (the
sret temp still merges with the view slot). Only a temp whose live range
spans the view block prevents the merge.

## 4. Hoist-prevention experiments (angle c)

Making the view address loop-varying removes it from the hoisted `%308`,
but the added address arithmetic grows the function and the RA still gives
r5 to `i*3`:

| view-address form | raw | aligned | size | insns |
|---|---:|---:|---:|---:|
| committed `Matrix view = *(...)` | **455** | **93** | 1984 | 660 |
| `Matrix *vp = (Matrix*)((char*)&view + (i >> 31))` | 1097 | 141 | **1992** | 663 |
| same, `(e >> 31)` | 1792 | 246 | **2000** | 667 |
| `Matrix *volatile vp = &view` | 1128 | 174 | **1988** | 662 |
| `(char*)&view + ((int)position.x >> 31)` | 1136 | 144 | **2004** | 666 |
| `(char*)&view + (velocity.x != velocity.x)` | 1152 | 142 | **2004** | 665 |
| `(char*)&view + (radius >> 31)` | 1790 | 257 | **1952** | 658 |
| `Matrix *vp = &view` / `Matrix &vp = view` / nested `{}` | 455 | 93 | 1984 | 660 |

In the `i >> 31` build the view address is local (`%275 = t2ADDri
%stack.10` at 14480, `%282 = t2ORRrs %275,%16,251` at 14720) and the
k-loop sret address is local, yet `i*3` is still r5: the greedy allocator
still gives r4 to the short `%stack.10` address vregs. The hoist is not
the whole story.

## 5. Other measured levers (all size-exact unless stated)

Outer-branch hints (committed body):

| hint | raw | aligned | size |
|---|---:|---:|---:|
| base | **455** | **93** | 1984 |
| `expect(this->impact != nullptr, 1)` | 455 | 93 | 1984 |
| `expect(this->empDamage != kNoAttribute, 1)` | 457 | 96 | 1984 |
| `expect(this->owner == nullptr || ..., 1)` | 533 | 112 | 1984 |
| `expect(explodeOnContact, 0)` | 665 | 190 | 1984 |
| `expect(mine, 0)` | 1049 | 157 | 1984 |
| `expect(this->playerGun, 1)` / `expect(enemy->egoFlag, 1)` | 455 | 93 | 1984 |
| `expect(this->geometries != nullptr, 1)` | 1608 | 265 | 1956 |
| `expect(scatter, 0)` on the radius branch | 1828 | 311 | 1992 |
| `expect(scatter, 1)` on the final scatter branch | 1804 | 349 | 1932 |

View-block reorderings (committed body):

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | **455** | **93** | 1984 |
| swap the two `SetAnimationState` calls | 457 | 95 | 1984 |
| impact block after the view block | 455 | 93 | 1984 |
| impact block before the view block | 1221 | 159 | 1984 |
| `lifetimes`/`lastHitKIPlayer` after the view block | 1260 | 162 | 1984 |
| cache the `Transform*` | 1174 | 142 | 1968 |
| cache `PaintCanvas*` | 1644 | 206 | 1976 |
| cache `geometries[i]` | 1212 | 224 | 1972 |
| `view` initialised at the i-loop top | 1845 | 383 | 1900 |

Loop forms / conditions (committed body):

| form | raw | aligned | size |
|---|---:|---:|---:|
| base `for (k=0;k<size;++k)` | **455** | **93** | 1984 |
| `for(;;){ if(k>=size) break; ... }` | 455 | 93 | 1984 |
| `while(k<size){...;++k;}` | 455 | 93 | 1984 |
| `for(k=0;k<size;k+=1)` | 455 | 93 | 1984 |
| `do{...;++k;}while(k<size)` | 1027 | 156 | **1988** |
| `for(;k<size;)` with `++k` at end | 455 | 93 | 1984 |
| `k<=size-1` | 946 | 125 | 1984 |
| `expect(k<size,1)` on loop condition | 1798 | 379 | **1948** |
| `expect(k<size,0)` on loop condition | 1710 | 341 | **1944** |
| cached `n = enemies->size()` | 1714 | 306 | **1960** |

i-loop local declaration order: `position,velocity,enemyPosition` (the
committed order) is the best of the six permutations (455/93); `p,e,v`
gives 500/117, `v,p,e` 470/116, `e,p,v` 538/137.

## 6. Conclusion

The residual is the single greedy-RA decision documented in
w13576/w13588/w13602/w13609/w13619: the hoisted `%stack.10` address
(`%308`, 14608 B) takes r4 and pushes `i*3` (12832 B) to r5. Every source
shape that shortens `%308` enough (prevent the hoist, separate the k-loop
sret, make the view address loop-varying) either grows the function or
re-runs the allocator to a different 1952-2004 B layout with far worse raw.
The faithful C-D-E body remains a free aligned win (93 -> 57) but an
8-byte raw regression (455 -> 463), so the committed body is kept.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13628
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN3Gun22calcCharacterCollisionEv
# committed base -> raw=455 aligned=93 size=1984/1984 insn=660/660
```

Live-interval / MIR probe (scratch, 12 GB ulimit, output to disk):

```sh
clang++ <Gun.cpp compile line from compile_commands.json> \
  -mllvm -print-after-all > /home/debian/GalaxyOnFire2/scratch/w13628/mir_all.txt
# Early MachineLICM: %308 = t2ADDri %stack.10 in bb.5
# Live Interval Analysis: %308 def 1568, last use 16176; %99 def 2880, last use 15712
```

## 8. Files changed

`_work/reconstructions/gun-calccollision-wave191b-w13628.md` only.
