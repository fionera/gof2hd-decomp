# `Gun::calcCharacterCollision` wave 192 (w14229): no-win variant sweep

Worker w14229, branch `wave192-w14229`, base master `02c4622b7`.
Symbol `_ZN3Gun22calcCharacterCollisionEv`, size 1984 B, 660 insns.
Scratch: `/home/debian/GalaxyOnFire2/scratch/w14229`.
Predecessors: `gun-calccollision-wave191b-w13822.md` (§1 root-cause evidence,
46-variant table, reproduction), `gun-calccollision-wave190-w13483.md`.

## 0. Result

**No `src/` change landed.** HEAD measured this wave:

| build | raw | aligned | size | insns |
|---|---:|---:|---:|---:|
| HEAD `723749876` | **463** | **57** | 1984/1984 | 660/660 |

Full `check` remained green: byte_exact 4508/4524, linked_exact 4508/4524,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0, parity
5370/5370. `.text` 74,628, `.ARM.extab` 562, `.ARM.exidx` 0, `.rodata` 0;
`.rel.plt` untouched.

## 1. Root cause (re-confirmed, not re-derived)

Single Ra/Rb asymmetry: ORIG keeps the scaled index `i*3` in **r4** and
rematerialises `sp+104` in **r5**; ours MachineCSE-hoists `sp+104` (`%513`)
into **r4** and keeps `i*3` (`%99`) in **r5**. Hunks 1,2,3,5,6,8,11,13,14,15
are pure relocation/branch-displacement fallout; hunk 4 (`0x1530cc`,
`mov r6,r4` vs `mov r6,r5` then the `sp+44`/`sp+104` swap) and hunk 7
(`mov r4,r6` vs `mov r5,r6`, uniform -2 insn shift after `0x153196`) are the
instruction-level witnesses; hunk 9 is placement-only. Details and objdump
witnesses in `gun-calccollision-wave191b-w13822.md` §1.

## 2. Variants measured this pass (all reverted)

| variant | raw | aligned | size | insns | verdict |
|---|---:|---:|---:|---:|---|
| HEAD | 463 | 57 | 1984 | 660 | baseline |
| `int i` / `int e` / `int k` loop indices | 463 | 57 | 1984 | 660 | neutral |
| named `dx`/`dy`/`dz` in box test | 1730 | 401 | 1980 | 663 | regression, size-fatal |
| `const Vector &position/velocity` | 1796 | 391 | 1920 | 649 | regression |
| `velocity` declared before `position` | 478 | 80 | 1984 | 660 | regression |
| hoist `bx/by/bz` | 1730 | 401 | 1980 | 663 | regression |
| named delta temp in scatter branch | 463 | 57 | 1984 | 660 | neutral |
| `Vector& td` (targetDir ref) | 463 | 57 | 1984 | 660 | neutral |
| named x-axis temp in mine box test | 463 | 57 | 1984 | 660 | neutral |

Pre-recorded no-wins from wave 191b (not retried): scatter stack-slot swap
(459, aligned not better), whole-axis temp hoist (463/60), `kcount_comma`
(1715/266/1960), `icount`, `ecount`, `positions_ptr`, `velocities_ptr`,
`enemies_local`, `canvas_top`, `view_decl_*`, `outer_int` (1342/117/1976),
`radius_ternary_if` (1454/113/1976), plus the truthiness/const/type/k-loop
spelling sweeps.

## 3. Remaining untried angle

Force the i-loop header spill: make `i*3` live across the `getPosition` calls
(touch `this->lifetimes[i]` or reorder a use at the top of the i-body) so clang
prefers spilling `fp` at the loop header and rematerialising `sp+104`, rather
than CSE-ing the address. Also untried: per-axis named `dy`/`dz` only, and a
non-mine box-test short-circuit layout (per-axis early `continue`, reordered
six comparisons). Both are speculative; every faithful spelling tried so far
is neutral or size-fatal.

## 4. Reproduction

```
python3 tools/hunks.py _ZN3Gun22calcCharacterCollisionEv > /tmp/hunks_w14229.txt
sed -n '/=== hunk 4\/15/,/=== hunk 5\/15/p' /tmp/hunks_w14229.txt | head -40
(ulimit -v 12000000; CMAKE_BUILD_PARALLEL_LEVEL=2 cmake --build cmake-build-match --target check)
python3 tools/fn_diff.py _ZN3Gun22calcCharacterCollisionEv --ours cmake-build-match/libgof2hdaa.so
```
