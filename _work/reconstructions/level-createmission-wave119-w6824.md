# w6824: `Level::createMission` (92.293729 -> 92.293729 %, 17172 B) — wave-119
# convoy-switch MachineBlockPlacement audit, `&&`/cmp/turret sweeps; no source win

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w6824`, branch `wave119-w6824`,
based on master `d4d2d3f91`.  Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; function `0xaffd8`-`0xb42ec`, 17172 B).
This continues `level-createmission-wave119-w6800.md` and `…-w6804.md`.
`src/game/world/Level.cpp` is unchanged; this note is the only commit.

## Baseline (pristine HEAD, re-measured this wave)

```text
_ZN5Level13createMissionEv   official 92.293729 %   size 17172/17172 B
                             6055 orig / 6065 ours insns
gate: byte 4505/4524, linked 4505/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
ELF: 27 sections, file 2,187,328 B, .ARM.extab 0x26290 (8 B short, createShip only).
```

## 1. New: the case-183 convoy switch is a MachineBlockPlacement tie-break

The original body/literal order is `[k0][k1][k2][default]` with descending
tests (`cmp #2; beq; cmp #1; beq; cbnz`).  All six source orders were
re-measured (if-chain and `switch` give identical results):

| source order | official % | final pool order |
|---|---:|---|
| **2,1,0 (baseline)** | **92.293729** | k2, k0, k1, default |
| 2,0,1 | 92.293729 | k2, k0, k1, default |
| 1,0,2 | 92.161716 | k1, k0, k2, default |
| 1,2,0 | 92.161716 | k1, k0, k2, default |
| 0,1,2 | 92.145215 | k0, k2, k1, default |
| 0,2,1 | 92.145215 | k0, k2, k1, default |

New diagnostic: `-mllvm -print-after-all -mllvm
-filter-print-funcs=_ZN5Level13createMissionEv` shows that for **source order
0,1,2** the MIR *before* `Branch Probability Basic Block Placement` is already
exactly the original's shape:

```text
bb.338: t2CMPri r1, 2 ; t2Bcc bb.343   ; k2 body
bb.339: t2CMPri r1, 1 ; t2Bcc bb.342   ; k1 body
bb.340: t2CMPri r1, 0 ; t2Bcc bb.344   ; default
bb.341: k0 body (fallthrough)
bb.342: k1 body
bb.343: k2 body
bb.344: default body
```

i.e. descending tests, block order `[k0][k1][k2][default]`.  The *final*
layout is `[k0][k2][k1][default]`: MachineBlockPlacement swaps the k1/k2
blocks.  The same swap happens for every source order, so the original's
`[k0][k1][k2]` layout is not reachable from any case permutation with clang
7.0.2 `-Oz`.  (The four-case forms do give the right pool order — see §2.)

## 2. Four-case forms give the right literal pool but a `tbb`

Writing the `else` as an explicit fourth case (`if (k==0) A else if (k==1) B
else if (k==2) C else if (k==3) D`, or `switch` with `case 3`) makes LLVM
emit a `tbb` jump table instead of the compare chain, but the body/pool order
becomes `[k0][k1][k2][default]`:

| variant | official % | size | insns |
|---|---:|---:|---:|
| **baseline (3 cases + else)** | **92.293729** | 17172 | 6065 |
| 4 explicit cases (if or switch) | 92.218830 | 17172 | 6064 |
| 4 cases, last `k - 3 == 0` / `k == 3 \|\| false` | 92.218830 | 17172 | 6064 |
| 4 cases + trailing empty `else` | 92.218830 | 17172 | 6064 |
| `if (k < 1) A else if (k < 2) B else if (k < 3) C else D` | 89.941414 | 17172 | 6064 |
| `if (k > 2) D else if (k > 1) C else if (k > 0) B else A` | 91.651543 | 17172 | 6067 |
| `switch` with `case 4`/`case 5` dummy (sparse) | 89.244738 | 17156 | 6060 |
| nested `else { if (k==3) D }` | 92.218830 | 17172 | 6064 |

The `tbb` costs more than the correct pool order saves (~0.15 pp net).
Swapping the *bodies* of k1/k2 in the source (semantically wrong, but it puts
the B/C constant runs in the original order) does not help either:
`swap_012` 92.095710, `swap_021` 92.145215, `swap_102` 92.293729.

## 3. `&&` at Level.cpp:1924 (pirate fortress): short-circuit vs branchless

The original lowers both `i == X && alienWave` (line 1904) and
`i == X && pirateFortress` (line 1924) branchlessly (`it ne; movne #1; eor
#1; orrs; bne`).  Ours is branchless for 1904 but short-circuits for 1924
(`ldr; cmp #0; beq; cmp; bne`).  Every spelling tried is byte-identical to
the baseline (92.293729 / 17172 / 6065):

- swap the second condition's operands (`pirateFortress && i == X`)
- swap the first condition's operands
- swap both
- `pirateFortress != 0`
- `(i == X) & pirateFortress` → 63.045811 / 17312 (much worse)
- `(i == X) & alienWave` → 75.689058 / 17172

## 4. `*bigShipsCount > 0` (line 1837): `cmp #0; ble` vs `cmp #1; blt`

| variant | official % | size |
|---|---:|---:|
| **baseline `> 0`** | **92.293729** | 17172 |
| `*bigShipsCount` (truthiness) | 89.666557 | 17164 |
| `!= 0` | 89.666557 | 17164 |
| `>= 1` on line 1843 (second test) | 92.293729 | 17172 |
| `*bigShipsCount` on line 1843 | 92.260726 | 17172 |

The `cmp #1; blt` form is a backend branch-folding decision; no source
spelling reaches `cmp #0; ble` in this context.

## 5. Guard/`raid` 84/88 spill-slot swap: `__builtin_expect` re-measured

`exp_X` = `__builtin_expect` inserted at X.  `guard` is the offset of the
first `str.w fp,[sp,#N]` (original `#88`; baseline ours `#84`).

| variant | guard | official % | size | insns |
|---|---|---:|---:|---:|
| **baseline** | #84 | **92.293729** | 17172 | 6065 |
| `expect(raid,0)` on final `if (raid)` | **#88** | 73.104246 | 17296 | 6051 |
| `expect(raid,1)` on final `if (raid)` | #84 | 84.908926 | 17192 | 6078 |
| `expect(mission == nullptr, 0)` | other | 85.905596 | 17188 | 6042 |
| `expect(inAlienOrbit(), 0)` | #84 | 92.293729 | 17172 | 6065 |
| `expect(raid,0)` on line 1610 | #84 | 88.395876 | 17188 | 6070 |
| `expect(raid,1)` on line 1610 | #84 | 92.293729 | 17172 | 6065 |
| `expect(raid,0)` on line 1584 | #84 | 88.582742 | 17180 | 6067 |
| `expect(raid,0)` on line 1593 | #84 | 92.293729 | 17172 | 6065 |
| `volatile bool raid` | #84 | 82.483077 | 17196 | 6059 |
| `volatile int raid` | #84 | 83.034756 | 17156 | 6058 |

Only `expect(raid, 0)` on the final `if (raid)` flips the slot, and it costs
~19 pp (the block layout changes globally).  The swap is worth ~+0.31 pp if
it could be flipped in isolation.

## 6. Other measured negatives

| variant | official % | size |
|---|---:|---:|
| `raidType`/`hard` declaration swap | 92.277228 | 17172 |
| `int hard` instead of `bool` | 92.293729 | 17172 |
| inline `hardCoreMode()` in the raid condition | 91.354562 | 17192 |
| `RaidCoords3` → `int raidCoords[3]` | 92.293729 | 17172 |
| `(security == 3) & raid` → `&&` | 87.262828 | 17196 |
| `raid && security == 3` | 92.293729 | 17172 |
| `alienWave ? (bigAlienWave ? 8 : 7) : 0` | 92.244224 | 17172 |
| `int alienGuns = 0; if (alienWave) …` | 92.293729 | 17172 |
| convoy loop over `i` with `k = i - 10` | 89.692168 | 17164 |
| `unsigned k` loop | 92.145215 | 17172 |
| `volatile int n` in case4Tail | 92.293729 | 17172 |

## 7. Residual map (unchanged)

- guard/`raid` 84/88 slot swap (§5).
- case-183 convoy body/literal order (§1) — MachineBlockPlacement tie-break.
- `&&` at 1924 short-circuit (§3).
- `*bigShipsCount > 0` cmp form (§4).
- alien-turret table select: original real `cbz` branch, ours `cmp/it/movne`
  cmov (SimplifyCFG folds the diamond; `volatile` pointer costs 72 B).
- `this` r4 (orig) vs r5 (ours) from the alien-turret preheader; `gun` r6 vs
  r4; `t` sl vs r6; `i` fp vs r9.
- tail-merged `objectivesA` store block (`b41e4` shared vs inline stores).
- jump-table halfwords for cases 1/2/4 differ by 1-2 (downstream placement).

## 8. Conclusion

HEAD remains a local optimum at **92.293729 %** (17172 B, 6055/6065 insns).
The convoy switch is now fully explained: the MIR before MachineBlockPlacement
is already correct for source order 0,1,2, and the remaining k1/k2 swap is a
block-placement tie-break that no case permutation reaches.  The four-case
forms fix the literal pool but force a `tbb`.  The guard/`raid` slot swap is
only reachable via `__builtin_expect(raid, 0)`, which costs ~19 pp.

## 9. Commands / scratch

```text
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w6824/score.py                  # official %, size, insns
cmake-build-match/../../.cache/ndk/.../clang++ <Level.cpp flags> \
  -mllvm -print-after-all -mllvm -filter-print-funcs=_ZN5Level13createMissionEv
cmake --build cmake-build-match --target check -j6
```

Scratch under `/dev/shm/w6824` (`Level.base.cpp` = pristine HEAD copy,
`score.py`, `cc.sh`, `convtest*.py`, `exptest*.py`, `mir_filt*.txt`).
