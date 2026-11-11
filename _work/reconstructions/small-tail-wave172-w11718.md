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

