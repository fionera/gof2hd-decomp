# MGame::OnTouchEnd — wave 156 (w10108): the `won` guard is a branch-weight artifact; faithful dock owners + initHudMenu(2) owner land at 8080 B

Status: source change landed, 2026-11-08. Worktree `/home/debian/GalaxyOnFire2/wt/w10108`,
base `f4e0ea40b` (wave 154 final). Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only).
Scratch `/dev/shm/w10108`. Full `check` green before and after: byte **4507/4524**,
linked **4507/4524**, size **4524/4524**, data **843/843**, imports **207**, lint CLEAN,
drift 0, RATCHET PASS. Target `_ZN5MGame10OnTouchEndEiiPv`, original 8080 B @ `0x17a144`.

| | base (wave 154) | this wave |
|---|---:|---:|
| size | 8080 | **8080** |
| raw same-address diff | 4391 | **4427** |
| aligned (align_diff) | 903 | **926** |
| shared `blx dockToAsteroid` owner | stream | **asteroid (right)** |
| shared `blx hudEvent(6)` owner | stream | **autopilot (right)** |
| shared `blx initHudMenu` owner | (1) @0x17adca | **(2) @0x17adca** |

## 1. The `won` guard: `beq` is a branch-weight artifact, not a source spelling

The original guard is `cmp r0,#0 ; beq.w` at `0x17adda`; ours is `cmp r0,#1 ; bne.w`
at `0x17ade2`.  The IR difference is exactly one constant and the branch order:

```
base  (== true):  %689 = icmp eq i8 %688, 1 ; br %690, %1381
truthy (A || B):  %689 = icmp eq i8 %688, 0 ; br %1381, %690
```

Both guards are 6 B, so the 20 B size difference is **not** at the guard: it is the
whole-function block re-layout caused by LLVM 7 `BranchProbabilityInfo`.
`calcZeroHeuristics` (`llvm-7.0.0/src/lib/Analysis/BranchProbabilityInfo.cpp:653`)
assigns `X == 0` a **20:12 likely-false** weight (`ZH_TAKEN_WEIGHT=20`,
`ZH_NONTAKEN_WEIGHT=12`), while `icmp eq i8 %688, 1` matches no heuristic and keeps the
default **50:50**.  The original was PGO-built, so its `cmp #0` branch carries a profile
weight, not the zero heuristic; the `== true` spelling happens to reproduce the original
layout because it keeps 50:50.

Every spelling that canonicalises to `icmp eq i8 %688, 0` therefore re-lays to **8060**
(and the raw explodes because the function is 20 B short).  Measured on the exact base
(all 8080/8080 unless noted):

| variant | size | raw | aligned |
|---|---:|---:|---:|
| `A || B == true` (base) | 8080 | 4391 | 903 |
| `A || B` / `A || B != 0` / `A || (bool)B` | 8060 | 6069 | 1871 |
| `bool won = A; if (!won) won = B;` | 8060 | 6069 | 1871 |
| `if (A) won = true; else won = B;` | 8060 | 6069 | 1871 |
| `bool won = A ? true : B;` | 8060 | 6069 | 1871 |
| `bool cw = B; bool won = A || cw == true;` | 8080 | 4759 | 1297 |
| `bool b = B == true; bool won = A || b;` | 8092 | 4612 | 1417 |
| `switch (won)` / `do { won = A || B == true; } while(0)` | 8080 | 4391 | 903 |
| `if (!won) {...} else {...}` (inverted) | 8080 | 4391 | 903 |
| `if (won == true)` / `if (won != false)` / `if (!!won)` | 8080 | 4391 | 903 |
| `A || (B & 1) == 1` | 8080 | 4371 | 904 |
| `__builtin_expect(B, 1/0)` / `expect(B != 0, 1/0)` | 8060 | 6069 | 1871 |
| `expect(B == true, 0)` / `expect(B == 1, 1)` | 8080 | 4391 | 903 |
| `expect(A, 1) || B` | 8100 | 4760 | 861 |
| `expect(A || B, 1)` | 8096 | 4786 | 863 |
| `if (__builtin_expect(won, 1))` (truthy) | 8096 | 4786 | 863 |
| `bool Mission::hasWon()` + `== true` (global) | 8076 | 6062 | 1081 |

`__builtin_expect` cannot express a 50:50 weight (it emits a 2000:1 `!prof` weight), so
the `== true` spelling remains load-bearing.  The `& 1` variant is the only raw win
(4371) but costs aligned 904 and moves MGame.cpp's literal pool (breaks
`MGame::dockEvent`, wave-154 §3).  **Conclusion: no `beq` spelling is landable at
8080 B without a profile weight.**  The original's `cmp #0` is the last unrecoverable
PGO signature in this guard.

## 2. Landed: faithful tail-merge owners + initHudMenu(2) owner (w10014 shape)

Two independent changes on the wave-154 base:

1. **Faithful dock tail-merge owners** (w9914 shape): named `asteroidTarget` local in the
   asteroid arm and `__builtin_expect(isDockingToDockingPoint(), 1)` on the middle arm.
   The shared `blx dockToAsteroid` is now owned by the asteroid arm
   (`0x17bc66`) and `blx hudEvent(6)` by the autopilot arm (`0x17baec`), as in the
   original (`0x17bcf4` / `0x17bb5c`).
2. **initHudMenu tail-merge owner**: reorder the three source arms from
   `0x200, 0x800, 0x400` to `0x400, 0x800, 0x200`.  The two `initHudMenu` calls are
   mutually exclusive bits, so this is semantically identical; it makes the `0x400` arm
   the lower-numbered MBB, which is the block `BranchFolder::CreateCommonTailOnlyBlock`
   keeps on an `EstimateRuntime` tie.  The shared `blx` is now owned by the `(2)` site
   (`0x17adca`), as in the original (`0x17afde`).  The dock chain is unmoved (the
   `pre-dock`/`dock`/`post` regions are byte-identical to the faithful-owner base).

The price is **+36 raw / +23 aligned** over the wave-154 base, all from the cold-block
placement (the original's join is at `0x17afe2`, ours at `0x17aeb2`); the ownership flip
itself is worth +3 raw (w10014 §2).  This is the Pareto-optimal 8080 B shape with both
faithful owners and the `(2)` owner among the ~150 shapes measured in waves 154-156.

## 3. Reproduction

```
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10108/fdiff.py _ZN5MGame10OnTouchEndEiiPv      # 4427 / 8080
python3 /dev/shm/w10108/align_diff.py _ZN5MGame10OnTouchEndEiiPv  # 926
cmake --build cmake-build-match --target check -j6               # RATCHET PASS
```

Scratch: `/dev/shm/w10108/{fdiff.py,align_diff.py,batch.py,try.py,tryfull.py,
v1..v8.json,MGame.base.cpp,base.ll,truth.ll,base.dis,truth8060.dis,final.dis}`.
The authoritative binary is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
