# MGame::OnTouchEnd — wave 157 (w10312): natural `0x200,0x800,0x400` arm order beats the w10212 reorder

Status: source change landed, 2026-11-08. Worktree `/home/debian/GalaxyOnFire2/wt/w10312`,
base `08575efff` (wave 156 final). Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only).
Scratch `/dev/shm/w10312`. Full `check` green before and after: byte **4507/4524**,
linked **4507/4524**, size **4524/4524**, data **843/843**, imports **207**, lint CLEAN,
drift 0, RATCHET PASS. Target `_ZN5MGame10OnTouchEndEiiPv`, original 8080 B @ `0x17a144`.

| | base (wave 156) | w10212 shape | this wave |
|---|---:|---:|---:|
| size | 8080 | 8080 | **8080** |
| raw same-address diff | 4391 | 4407 | **4404** |
| aligned (align_diff) | 903 | 923 | **920** |
| `initHudMenu` owner | (1) @0x17adca | (2) @0x17adca | **(1) @0x17adca** |
| `dockToAsteroid` owner | stream | asteroid | **asteroid (right)** |
| `hudEvent(6)` owner | stream | autopilot | **autopilot (right)** |

(w10212 quotes 4415/926 on its wave-155 base; re-measured on wave 156 it is 4407/923 —
the wave-156 `.text` changes move MGame.cpp's literal pool, worth +8 raw in `post`.)

## 1. Landed shape

Two changes on the wave-156 base, i.e. the w10212 shape **without** its arm reorder:

1. **Faithful dock tail-merge owners** (w9914): named `asteroidTarget` local in the
   asteroid arm and `__builtin_expect(isDockingToDockingPoint(), 1)` on the middle arm.
   The shared `blx dockToAsteroid` is owned by the asteroid arm (`0x17bc66`) and
   `blx hudEvent(6)` by the autopilot arm (`0x17baec`), as in the original
   (`0x17bcf4` / `0x17bb5c`).
2. **Fold `if (hudTouchFlags == 0)` into the wingman `else if` chain** (w10212).
   `wingmanCommand >= 0` implies a wingman bit is set, so the block is dead after the
   wingman arm; the flat chain is the natural source.

The initHudMenu arms stay in **source order `0x200, 0x800, 0x400`** (the original's
binary test order). w10212 reordered them to `0x400, 0x800, 0x200` to make the `(2)`
arm the lower MBB and hence the shared-tail owner. That reorder fixes the owner but
changes the *test* order (`tst #0x400; lsls #20; lsls #22`), which the original does
not have. The natural order is 3 raw / 3 aligned better on the wave-156 base.

## 2. Region diff

| region | base | w10212 | this wave |
|---|---:|---:|---:|
| `pre 0x17a144..0x17adc0` | 114 | 124 | **121** |
| `pre-dock 0x17adc0..0x17bb3a` | 3277 | 3268 | **3268** |
| `dock 0x17bb3a..0x17bf7e` | 986 | 994 | **994** |
| `post 0x17bf7e..end` | 14 | 21 | **21** |

The 3268-byte `pre-dock` run is unchanged by any shape measured this wave. It is the
PGO join-placement signature identified in w10014 §2: the original places the
`hudTouchFlags == 0` test at `0x17afe2` (after the initHudMenu arms) with the epilogue
inline at `0x17afe8` and the body cold at `0x17b01a`; ours places the test at
`0x17aeb2` (before the arms) with the body inline and the epilogue far away at
`0x17b9fc`. The epilogue move is the whole difference: the aligned diff is 920, and
17 branch targets plus ~900 shifted branch targets all trace to the epilogue index
(orig 1306, ours 2209).

## 3. New hypotheses measured (all 8080 B unless noted)

All on top of the faithful dock owners + fold.

| variant | raw | aligned | note |
|---|---:|---:|---|
| **landed: natural `0x200,0x800,0x400`** | **4404** | **920** | owner (1) |
| w10212 `0x400,0x800,0x200` | 4407 | 923 | owner (2) |
| `0x200,0x400,0x800` | 4409 | 1039 | owner (1) |
| `0x800,0x200,0x400` | 4745 | 1141 | size 8068 |
| `0x800,0x400,0x200` | 4709 | 1143 | size 8076 |
| `0x400,0x200,0x800` | 4748 | 1140 | size 8076 |
| `0x400,0x800,0x200` (w10212) | 4709 | 1136 | size 8076 (re-measured) |
| `__builtin_expect(hudTouchFlags == 0, 0)` | 5756 | 1070 | **size 8068**, epilogue moves to 0x17aeba |
| `expect(==0,0)` + `0x400,0x800,0x200` | 5756 | 1073 | size 8068 |
| `expect(==0,0)` separate if | 5756 | 1070 | size 8068 |
| `expect(hudTouchFlags != 0, 1)` | 7348 | 1765 | size 8148 |
| `expect(hudMenuOpen, 1)` | 5951 | 1200 | size 8064 |
| `expect(hudTouchFlags & 0x40, 1/0)` | 6781/7533 | 1662/2408 | size 8056/8036 |
| `expect(bigDockCondition, 1/0)` | 5352/4736 | 1098/1841 | size 8064/8060 |
| `expect(wingmanCommand >= 0, 0/1)` | 4404 | 920 | canonicalised away |
| `switch (hudTouchFlags) { case 0: ... }` | 4404 | 920 | canonicalised to `cmp eq 0` |
| `switch (hudTouchFlags & 0xe00)` for the 3 arms | 4794 | 1264 | size 8084 |
| `(hudTouchFlags & 1) == 0 && == 0` (AND exception) | 4404 | 920 | folded to `== 0` |
| `hudTouchFlags <= 0` / `< 1` / `!(> 0)` / `== 0u` | 4404 | 920 | folded to `== 0` |
| body moved before the arm chain | 4472 | 930 | pre-dock 3259 |
| body moved out of the `else` (w10212) | 4467 | 938 | — |

## 4. The `__builtin_expect` lever: right join, wrong size

`__builtin_expect(hudTouchFlags == 0, 0)` **does** fix the join: the epilogue moves
from index 2209 to 1255 (original 1306), i.e. right after the initHudMenu arms, and
the pre-dock region drops 3268 -> 2974. But the 2000:1 weight re-lays the hot region
(`pre` 121 -> 1463) and the function shrinks to 8068 B, so it is not landable at
8080 B. The original's branch is a *mild* PGO weight (the body is cold but not
2000:1); clang 7.0.2 has no `__builtin_expect_with_probability` (confirmed: undeclared
identifier), and every source spelling of `hudTouchFlags == 0` canonicalises to
`icmp eq 0`, which `calcZeroHeuristics` weights 62.5 % taken (body hot). The
`(x & 1) == 0` AND-exception gives 50:50 but folds back to `== 0`.

## 5. Honest assessment

The mission's pre-dock join is **not fixed** at 8080 B. The natural arm order is a
small win over w10212 (raw 4407 -> 4404, aligned 923 -> 920) and restores the
original's test order, but it gives up the `initHudMenu(2)` owner (the natural order
makes the `(1)` arm the lower MBB). The two are mutually exclusive in source order:
owner (2) needs `0x400` first, the original's test order needs `0x200` first, and
clang 7.0.2 lays the arms out in MBB order without a profile weight.

Next lever (not this wave): a genuine mild branch weight on the `hudTouchFlags == 0`
join. The only in-source weight is `__builtin_expect` (2000:1), which is too strong;
a PGO `.prof` replay or a clang flag would be needed (out of scope: no link tricks).

## 6. Reproduction

```
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10312/fdiff.py _ZN5MGame10OnTouchEndEiiPv      # 4404 / 8080
python3 /dev/shm/w10312/align_diff.py _ZN5MGame10OnTouchEndEiiPv  # 920
python3 /dev/shm/w10312/regions.py _ZN5MGame10OnTouchEndEiiPv
cmake --build cmake-build-match --target check -j6               # RATCHET PASS
```

Scratch: `/dev/shm/w10312/{fdiff.py,align_diff.py,regions.py,aligndetail.py,
harness.py,batch1..8.py}`.  The authoritative binary is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
