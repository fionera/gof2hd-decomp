# Small tail wave 172 (w11718): Generator bonus-block register shape

Status: in progress.  Worktree `wt/w11718`, branch `wave172-w11718`, base
master `41ef4c8b6`.  Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Metrics: raw same-address diff bytes and instruction-index alignment
(`tools/fn_diff.py`).  Baselines re-confirmed exactly:

| symbol | raw | aligned | size |
|---|---:|---:|---:|
| `_ZN3Gun22calcCharacterCollisionEv` | 463 | 57 | 1984 |
| `_ZN11SpaceLounge10drawLoungeEv` | 175 | 41 | 1948 |
| `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 127 | 107 | 1996 |

## Generator::createMission — landed raw 127 -> 40

Root cause of the 127 raw is the bonus block's register allocation.  The
original keeps `rawBonus` in `r6`, `b` in `r5`, spills `rem` to the reused
`status` slot (`sp+32`), and if-converts the fixup:

```
orig 97702 vmov r6,s0        ; rawBonus
     9770c adds r5,r1,r6     ; b = rem + rawBonus
     9770e str  r1,[sp,#32]  ; rem spill (reuses status slot)
     97718 ldr  r0,[sp,#32]  ; reload rem
     9771a cmp  r1,#0
     9771c it   ne
     9771e subne r5,r6,r0    ; b = rawBonus - rem
     97720 str  r5,[sp,#16]  ; bonus store (reuses fromSystem slot)
```

Clang 7 at `-Oz` naturally picks the opposite assignment (`rawBonus` r5,
`b` r6) and lets `rem` take `r9`, spilling `agent` instead (frame 104,
size 2000).  A `volatile` copy of `rem` placed after the `b` add forces the
store-after-add/reload-before-branch order, and a second volatile anchor
store of `rawBonus` flips the allocator back to the original `r6/r5`
assignment.  The landed shape:

```cpp
volatile int bonus;
volatile int bv;          // volatile anchor: fixes r6/r5 assignment
...
int remVal = rawBonus % 50;
int b = rawBonus + remVal;
volatile int rem = remVal; // store after the add, reload before the branch
if (b % 50 != 0) {
    b = rawBonus - rem;
}
bv = rawBonus;             // volatile anchor (dead store, keeps r6 for rawBonus)
bonus = b;
```

Result: **raw 127 -> 40, aligned 107 -> 21, size 1996 (exact)**.  The
remaining 40 bytes are the `rem` slot (`sp+44` vs `sp+32`), the branchy
`cbz` vs `cmp/it/subne`, and the two volatile anchor slots (`sp+92/96` vs
`sp+16` for bonus).  Full `check` is green.

### Shape table (volatile `bonus` unless noted; all size 1996 unless noted)

| shape | raw | aligned | size |
|---|---:|---:|---:|
| baseline (`volatile rem`, `int b`) | 127 | 107 | 1996 |
| `int rem`, `int b` (non-volatile) | 162 | 97 | 2000 |
| `volatile b` (`int bonus`) | 163 | 25 | 1996 |
| `int rem`, `volatile int vrem = rem`, `b -= vrem` | 582 | 126 | 1996 |
| `int rem`, `*(volatile int*)&rem`, normal sub | 582 | 126 | 1996 |
| `int remVal`, `volatile int rem = remVal`, normal sub | 582 | 126 | 1996 |
| same + `volatile int bv = b` after if (AE) | 53 | 36 | 1996 |
| same + `bv` store after if (BD) | 49 | 35 | 1996 |
| `bv` declared at top, store after if (TA) | 45 | 31 | 1996 |
| `bv = rawBonus` after if, `bv` at top (**landed**) | **40** | **21** | **1996** |
| `bv = rawBonus + 0` after if | 40 | 21 | 1996 |
| `bv = rawBonus` before if | 627 | 76 | 2004 |
| `bv = b + 1` after if | 609 | 74 | 2000 |
| `bv = rem` after if | 607 | 69 | 2000 |
| `bv = 0` after if | 607 | 69 | 2000 |
| `int bonus` + volatile pointer stores | 638-758 | 106-171 | 2000 |
| `fromSystem` reused as bonus storage | 638 | 121 | 1996 |
| non-volatile `bonus` + `bv = rawBonus` | 758 | 171 | 2000 |


## SpaceLounge::drawLounge — re-confirmed, no new win

Baseline `175/41` (1948 B) re-confirmed.  The residual is the right-edge
clamp: the original keeps `pad` in `r9` and `x` in `r5`, and emits
`addgt.w r1,r6,sl; subgt r5,r0,r1` (`x = right - (width + 2*pad)` reusing
the `sl = pad<<1` live from `top`).  Clang 7 `-Oz` reassociates
`right - (width + pad + pad)` to `right + (-2*pad - width)` and
rematerialises `pad<<1` from `fp`.  New shapes this wave (all size 1948
unless noted):

| shape | raw | aligned | size |
|---|---:|---:|---:|
| baseline | 175 | 41 | 1948 |
| `int right` local, both condition and clamp | 176 | 42 | 1948 |
| `int right` local + `right - (width + pad*2)` | 445 | 54 | 1948 |
| `int pad2 = pad+pad` + `right - (width + pad2)` | 445 | 54 | 1948 |
| `int total = width+pad+pad` + `right - total` | 484 | 76 | 1952 |
| `right - width - pad - pad` | 468 | 64 | 1944 |
| `right - (width + pad + pad)` (direct) | 176 | 42 | 1948 |
| `right - (width + pad*2)` (direct) | 445 | 54 | 1948 |
| `volatile int pad2` + `right - (width + pad2)` | 541 | 193 | 1952 |
| `pad2` before `x`, clamp via `pad2` | 445 | 54 | 1948 |
| `x = right; x -= width + pad + pad;` | 176 | 42 | 1948 |
| `x = right - width; x -= pad + pad;` | 445 | 54 | 1948 |
| condition `x + pad + width > right` | 175 | 41 | 1948 |
| condition `width + pad + x > right` | 175 | 41 | 1948 |
| `int right` local, condition only | 175 | 41 | 1948 |
| `short pad2` + `right - (width + pad2)` | 175 | 57 | 1948 (740 insn) |
| `int clampX = right - (width+pad2); x = clampX;` | 445 | 54 | 1948 |
| `pad2` live but clamp uses `pad+pad` | 176 | 42 | 1948 |
| `volatile int x` | 1862 | 454 | 1968 |

The `pad2` shapes do recover `addgt.w r1,r6,sl; subgt r5,r0,r1`, but the
extra live int moves `pad` from `r9` to `fp` and `x` from `r5` to `r9`,
cascading through the rest of the function.  Master keeps `175/41`.

## Gun::calcCharacterCollision — re-confirmed, no new win

Baseline `463/57` (1984 B) re-confirmed.  The largest residual is the
outer `for (unsigned e = 0; e < this->enemies->size(); ++e)` loop: the
original is laid out bottom-tested (`b.n` from the preheader to the test
at `1534fc`, body at `153442`, `bcc 153442`), ours is top-tested (test at
`153440`, body at `15344c`, `b.n 153440`).  New outer-loop shapes:

| shape | raw | aligned | size |
|---|---:|---:|---:|
| baseline | 463 | 57 | 1984 |
| `__builtin_expect(e < size, 1)` in condition | 673 | 154 | 1984 |
| `__builtin_expect(e < size, 0)` in condition | 1868 | 580 | 1940 |
| `for (e=0;;++e) { if (e >= size) break; ... }` | 463 | 57 | 1984 |
| same with `__builtin_expect(e >= size, 0)` | 673 | 154 | 1984 |
| `while (e < size) { ++e; ... }` | 1329 | 121 | 1980 |
| `for (e=0; e<size;) { ++e; ... }` | 1329 | 121 | 1980 |
| condition reversed `size > e` | 466 | 59 | 1984 |

The `break`-at-top form is byte-identical to the baseline.  The rotation
and the `i*3` index / scratch-Vector stack-slot residuals remain
MachineBlockPlacement / greedy-RA ties.  Master keeps `463/57`.

## Conclusion

One measured win landed: `Generator::createMission` raw **127 -> 40**,
aligned **107 -> 21**, size exact 1996, via a volatile `rem` copy placed
after the `b` add plus a volatile `rawBonus` anchor that restores the
original `r6/r5` register assignment.  Whole-`.text` differing bytes
**81,661 -> 81,574** (87-byte drop, exactly the function's raw delta).
Full `check` green: byte_exact 4507, linked_exact 4507, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370,
`.rel.plt` byte-identical.
