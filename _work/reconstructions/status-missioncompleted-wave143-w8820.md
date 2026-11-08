# Wave 143 (w8820): `Status::missionCompleted` — the 4:3:2:1 vector is
# **not unique**: a whole family of shallow *group-uniform descending*
# vectors reaches 558 B / 1432 B, and the if-chain fold family
# `(L+1)^k : (L+1)^(k-1)L : ... : L^k` is inside it.  Nested switches are
# too steep; per-case hot weights are not group-uniform and lose.

Status: **note-only** (no `src/` change landed).  Gate on the unchanged
tree: `byte_exact 4507/4524`, `linked_exact 4507/4524`, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0.  Worktree `wt/w8820`,
base master `bf083ab41` (wave 142).  Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2 r328903 / LLVM `1d739ffb`, `-Oz`, image base 0x10000).

Target: `_ZN6Status16missionCompletedEbbx` — original `0xabf38`,
**1432 B**, baseline raw same-address diff **722 B**.

Metric: raw same-address diff bytes (`/dev/shm/w8820/fdiff.py`), not the
PYTHONHASHSEED-dependent fuzzy %.

## 0. New results

### 0.1 The 558 B layout target is a *family*, not one vector

Injecting `!prof` weights into the current optimised IR switch (case order
= source order `167,174,8,0,11,13,14,150..189,156..172`, 29 cases +
default) and scoring the real gold link:

| vector `(default,A,low,B,C)` (group-uniform) | raw diff | size |
|---|---:|---:|
| `1,3,4,3,2` (the w137 4:3:2:1 vector) | **558** | 1432 |
| `1,5,4,3,2` (linear 5:4:3:2:1) | **558** | 1432 |
| `1,6,5,4,3` | **558** | 1432 |
| `1,7,6,5,4` | **558** | 1432 |
| `1,10,9,8,7` | **558** | 1432 |
| `1,4,4,3,2` (A=low) | **558** | 1432 |
| `1,5,5,4,4` | **558** | 1432 |
| `1,4,3,2,1` (C=default) | 1174 | 1444 |
| `1,2,4,3,2` (A too low) | 1254 | 1372 |
| `1,4,4,4,2` (B too high) | 1252 | 1372 |
| `1,4,4,3,3` (C too high) | 722 | 1432 |
| `1,2,2,2,2` / `1,3,3,3,3` (flat) | 722 | 1432 |

So the load-bearing property is **A >= ~3, low = 4, B = 3, C = 2,
default = 1** (or any common scaling), with C strictly above the default
and B strictly above C.  The exact 4:3:2:1 is not required.

### 0.2 The if/switch *fold* family is inside the winning family

A chain of `k` `if (__builtin_expect(type == c, 0))` arms followed by a
`switch` (the non-geometric `PredDefault == BB` fold) emits
`default = L^k`, `arm_j = (L+1)^(k-j+1) * L^(j-1)`, switch cases = `L^k`
(verified on a small program with the real clang 7.0.2).  Scored on the
real IR as group-uniform vectors:

| fold family | vector `(default,A,low,B,C)` | raw diff | size |
|---|---|---:|---:|
| `k=3, L=3` | `27,64,48,36,27` | **558** | 1432 |
| `k=4, L=3` | `81,256,192,144,108` | **558** | 1432 |
| `k=5, L=3` | `243,1024,768,576,432` | **558** | 1432 |
| `k=4, L=4` | `256,500,400,320,256` | **558** | 1432 |
| `k=3, L=4` | `64,100,80,64,64` | 722 | 1432 |
| `k=2, L=4` | `16,20,16,16,16` | 722 | 1432 |
| `k=3, L=2` | `8,27,18,12,8` | 1174 | 1444 |
| `k=2, L=3` | `9,16,12,9,9` | 722 | 1432 |
| `k=4, L=2` | `16,108,72,48,32` | 1174 | 1444 |

So the **if-chain fold at L=3 or L=4 with k>=4 produces the original block
order exactly** (`k=4, L=3` is the cleanest: four group arms + default).
This is the first fold family found that does, and it is a *natural*
four-arm if/else-if chain shape with `-mllvm -likely-branch-weight=3`.

### 0.3 Nested switches and per-case hot weights lose

* 2/3/4-level nested `switch` trees (all-default or case-targeted expects,
  L=2..10) emit group-uniform but **too steep** vectors, e.g.
  `L=4 all-default`: `4,858,572,44,4` -> raw **1312 / 1412**;
  `L=6 (156,150,8,156)`: `216,2178,1188,396,216` -> **1283 / 1428**.
  None reaches 558.
* Per-case hot weights (only the first case of each group hot, rest 1) are
  **not group-uniform** and lose: `167=5,8=4,150=3,156=2` -> **1288/1416**;
  `167=64,8=48,150=36` -> **795/1432**.
  Group-uniformity is essential: it is what keeps each layout run
  contiguous in `MachineBlockPlacement`'s successor chase.
* The non-geometric `if (expect(type==X,0)) body else switch` shape on the
  real 29-case switch emits `[L, 29, L, ...]` (special case = total), which
  is far too steep: measured **1228-1301 / 1416-1448** for L=1..10.

### 0.4 Source shapes measured directly (all worse than 722)

Real `Status.cpp`, real gold link, extra `-mllvm -likely-branch-weight=N`:

| shape | raw diff | size |
|---|---:|---:|
| master | **722** | 1432 |
| master + `-mllvm -likely-branch-weight=1..6` (5 in-body expects) | 884-896 | 1428 |
| master + `-mllvm -likely-branch-weight>=8` | 722 | 1432 |
| `switch (__builtin_expect(getType(), K))`, K in {8,167,150,0,156}, L=2..8 | 884-1300 | 1412-1436 |
| `if (expect(getType()==999,0)) return m; else switch` | 1228-1301 | 1416-1448 |
| if-chain over 167/174 bodies (2 arms) | 1279-1310 | 1416-1424 |
| if-chain over 167 / 8 / 150 bodies (3 arms) | 1248-1330 | 1352-1416 |

The real if-chain moves *single cases* out of the switch, so the groups are
not uniform and the layout does not move.  The fold's group-uniform family
is only reachable if a whole *group* is one arm of the chain.

## 1. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8820/fdiff.py _ZN6Status16missionCompletedEbbx

# IR injection + real gold link (score one vector):
python3 /dev/shm/w8820/inject.py status_base.ll status_inj.ll \
  1,5,4,3,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2
bash /dev/shm/w8820/cc_obj.sh status_inj.ll \
  cmake-build-match/CMakeFiles/gof2.dir/src/game/mission/Status.cpp.obj
bash /dev/shm/w8820/linkcmd.sh
```

Scratch (`/dev/shm/w8820/`): `fdiff.py` (raw-diff scorer), `inject.py`
(insert `!prof` into the real switch), `cc_obj.sh` / `cc_ll.sh` / `linkcmd.sh`
(exact TU compile + checked-in gold link), `score_vec.py` (inject+link+score),
`score.sh` (source variant + extra flags), `exp/gen*.py` (small-shape fold
weight generators), `exp/gennest.py` (nested-switch weights).

## 2. Open items

1. **Status:** the fold family `k>=4, L=3/4` is the first producer of the
   original block order, and `k=4, L=3` is exactly a four-arm if/else-if
   chain over the four groups plus a default.  The remaining problem is
   source *shape*: the original's groups have 2/5/12/10 cases, so a chain
   arm must test a whole group.  The `||`-chain form is not folded (SimplifyBranchOnICmpChain
   drops weights, `TODO: Preserve branch weight metadata`), and a nested
   `switch` per group is too steep.  The next lever is a *group* arm that
   `FoldValueComparisonIntoPredecessors` accepts as a single equality
   comparison on the same SSA value (e.g. a helper returning a group id, or
   a `case a ... b:` range) with `-mllvm -likely-branch-weight=3`.
2. The residual 558 B is *source-level* (case-body instruction differences:
   the extra `ldr r6,[sp,#16]` reloads, the `cmp r0,#0; ldr r6,[sp,#16]`
   sequence, RA around `0x32e..0x344`), not weights (w137 §1).
