# MGame::OnTouchEnd — wave 158 (w10511): the `switch` arm order `0x200,0x400,0x800` + `expect(won,1)` lands the join at raw 4022 / aligned 762, but is +12 B

Status: analysis only, source left at wave-158 base (gate green). Worktree
`/home/debian/GalaxyOnFire2/wt/w10511`, base `cfde1f5f2`. Owned TU
`src/game/menu/MGame.cpp` (`OnTouchEnd` only). Scratch `/dev/shm/w10511`.
Target `_ZN5MGame10OnTouchEndEiiPv`, original 8080 B @ `0x17a144`.

| | base | best new shape |
|---|---:|---:|
| size | 8080 | **8092 (+12)** |
| raw same-address diff | 4387 | **4022** |
| aligned (align_diff) | 901 | **762** |
| join address | `0x17aeb2` | **`0x17afee`** (orig `0x17afe2`) |
| epilogue | `0x17ba18` | **`0x17b010`** (orig `0x17b004`) |

The shape lands the join *structurally* (hot tail right after the join, cold body
after the epilogue) and improves both metrics by a wide margin (raw -365,
aligned -139), but the function grows 12 B, which shifts every later function and
drops `byte_exact` 4507 -> 3106.  It is therefore **not landable at 8080 B yet**.

## 1. The winning shape

Two independent source edits on the base:

1. **Arm order `0x200, 0x400, 0x800`** (the `0x400` initHudMenu(2) arm moved
   directly after the `0x200` arm) with a semantically-null store after the
   `initHudMenu(2)` call:
   ```cpp
   if (this->hudTouchFlags & 0x200) {
       this->hud->initHudMenu(1, this->level);
   } else if (this->hudTouchFlags & 0x400) {
       this->hud->initHudMenu(2, this->level);
       this->hudMenuOpen = 0;          // already 0 here; real store, breaks the tail merge
   } else if (this->hudTouchFlags & 0x800) {
       ... useCloak();
   }
   ```
2. **`__builtin_expect(won, 1)`** on the `if (won)` guard in the cutscene branch
   (the `hasWon() || campaign->hasWon()` chain).  This is what places the `won`
   continuation *before* arm2, which is the missing piece w10412 called "the
   return path needs work".

Resulting layout (ours):

```
17ad4c: arm chain test (0x200, 0x400, 0x800, 0x2000, ...)
17adc0: arm1 (0x200) ldrd/movs/blx/b.n join
17adcc: won chain first part
17af50: won continuation (keepObjectives/mission)
17afb8: literal pool
17afd0: cold body (hudTouchFlags==0)        <-- placed BEFORE the join here
17afee: join: ldr [fp,#248]; cbz -> 17b026
17aff4: hot tail (isAutoPilot && !wasAutoPilot)
17b010: epilogue
17b026: cold body (hudTouchFlags==0)
```

This mirrors the original's `0x17afd8..0x17b01a` structure exactly, shifted +12.

## 2. Region diff

| region | base | this shape |
|---|---:|---:|
| `pre 0x17a144..0x17adc0` | 114 | **77** |
| `pre-dock 0x17adc0..0x17bb3a` | 3277 | **2625** |
| `dock 0x17bb3a..0x17bf7e` | 982 | 1059 |
| `post 0x17bf7e..end` | 14 | 261 |

The `pre` drop is the branch offsets to the epilogue: the epilogue is now only
12 B from the original instead of 0xa14 B.  The `post` rise is the +12 B shift.

## 3. Why it is +12 B

`expect(won,1)` alone on the base is 8092 B (+12).  The store is size-neutral in
this layout (arm2's `movs r0,#1; strb.w r0,[fp,#214]` is 6 B, but the arm
reorder lets arm1 keep its own `blx` and arm2 keep its own `blx`; the original
shares one `blx`).  Concretely, vs the original:

- arm1 owns a `blx` instead of `b.n` to arm2's shared `blx`: **+4 B**
- arm2 has its own `b.n` to the join (the cold body sits between arm2 and the
  join): **+2 B**
- the `expect` re-lays the `won` continuation and turns two 16-bit `ldr` into
  `ldr.w`: **+6 B**

Measured size-reduction attempts that did **not** reach 8080:

| variant | size | raw | aligned |
|---|---:|---:|---:|
| base | 8080 | 4387 | 901 |
| **p248 + store + expect(won,1)** | **8092** | **4022** | **762** |
| p248 + store, no expect | 8072 | 5536 | 1138 |
| p248 + store + `int won` + `expect(won!=0,1)` | 8072 | 5536 | 1138 |
| p248 + store + `expect(failed==1,0)` | 8080 | 5305 | 2063 |
| p248 + store + expect + cold `resumeSounds` last | 8096 | 4667 | 875 |
| p248 + store + expect + no player null check | 8096 | 4665 | 852 |
| p248 + store + expect + `bool failed` | 8076 | 5868 | 2003 |

## 4. Other sweeps (all on the current base, 8080 B unless noted)

Arm permutations (`if/else if` chain):

| order | raw | aligned | size |
|---|---:|---:|---:|
| 284 (base) | 4387 | 901 | 8080 |
| 248 | 4392 | 1020 | 8080 |
| 428 | 4394 | 1022 | 8080 |
| 482 | 4390 | 904 | 8080 |
| 824 | 6059 | 1057 | 8060 |
| 842 | 6059 | 1059 | 8060 |

Store-after-call probes (single store, no expect): arm1/arm2 `hudMenuOpen=1`
gives 8072; cloak/khad `hudMenuOpen=1` gives 8088 (aligned 873/875, raw
4656/4648); `touch0Id=nullptr` 8084; `pauseOpen=0` 8084.

`failed` spelling: `if (failed)` / `!=0` / `>0` / `bool` all give 8072 and
aligned 1967 (the `cmp #0; beq` form re-lays the whole `won` chain badly);
`== true` / `== 1u` canonicalise to the base `cmp #1; bne` (4387/901).

Cold-body reorders: `resumeSounds()` last -> 8076/4647/899; no player null
check -> 8076/4649/900; `touch0Id` first / `pauseOpen` first canonicalise to
base.

`expect(won,1)` alone: 8092/4752/835.  `expect(won,0)`: 7832/5145/2162.

## 5. Next lever

The shape is right; only the 12 B are wrong.  The cleanest fix is to make arm1
share arm2's `blx` (the original's `b.n` form) while keeping the store after the
shared call — i.e. a source shape where the `0x200` arm sets `r1=1` and jumps to
the `0x400` arm's `blx`+store.  That is worth -4; the remaining -8 must come
from the `won` continuation (the `ldr.w` pair) or from dropping the `expect`
hint in favour of a cheaper block-placement trigger.

## 6. Reproduction

```
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10511/fdiff.py _ZN5MGame10OnTouchEndEiiPv      # 4022 / 8092
python3 /dev/shm/w10511/align_diff.py _ZN5MGame10OnTouchEndEiiPv  # 762
python3 /dev/shm/w10511/regions.py _ZN5MGame10OnTouchEndEiiPv
```
Scratch: `/dev/shm/w10511/{fdiff.py,align_diff.py,regions.py,regdiff.py,
aligndetail.py,harness.py,V2..V9.py,batch*.py}`.
The authoritative binary is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
