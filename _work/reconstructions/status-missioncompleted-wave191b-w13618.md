# `Status::missionCompleted` — wave 191b (w13618): TBH weights / case-label audit

Worktree `wt/w13618`, branch `wave191b-w13618`, base master `7301219a1`.
Target `_ZN6Status16missionCompletedEbbx` (`src/game/mission/Status.cpp`,
1,432 B, VMA `0xabf38`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13618`.

**Status: research note, no `src/` change landed.**  Baseline re-confirmed
on the committed source: raw **71**, aligned **38**, size
**1,432/1,432** exact, 527/527 instructions.  Section differing bytes:
`.text` **71,196**, `.ARM.extab` **577**, `.ARM.exidx` **0**,
`.rodata` **0**; all section sizes are identical to the original.  Tree
clean, gate green.

## 0. Per-region diff map (re-measured)

| region | diff bytes | VMA |
|---|---:|---|
| 40-entry TBH table | 24 | `abff2..ac013` |
| production-good body (source `case 158`) | 46 | `ac266..ac2a5` |
| literal pool (`Globals::status` GOT offset) | 1 | `ac4c4` |

The 46-byte body diff starts at `ac266` (after the shared
`cmp r8,#0 / station->getIndex / getTargetStation` prefix, which is
byte-identical) and runs to `ac2a5`; the literal-pool byte is a
consequence of the body size (GOT slot order), not independent.

## 1. New finding: our case labels are a permutation of the original's

Decoding both TBHs (`target = table_base + 2*entry`) and matching bodies
by address gives the original's type→body map versus ours.  Types
`150..155`, `167/174` and the low group `8/11/13/14` are identical; the
rest are permuted:

| body address | original type | our type (source label) |
|---|---|---|
| `ac180` | 158 | 156 |
| `ac1dc` | 162 | 157 |
| `ac242` | 166 | 158 |
| `ac2a6` | 170 | 160 |
| `ac2b4` | 184 | 162 |
| `ac2ee` | 189 | 163 |
| `ac32a` | 156 | 164 |
| `ac354` | 157 | 165 |
| `ac388` | 160 | 166 |
| `ac3b6` | 163 | 168 |
| `ac3d2` | 164 | 169 |
| `ac3d6` | 165 | 170 |
| `ac3de` | 168 | 171 |
| `ac3fc` | 169 | 172 |
| `ac400` | 171 | 184 |
| `ac41a` | 172 | 189 |

i.e. our→orig: `156→158, 157→162, 158→166, 160→170, 162→184,
163→189, 164→156, 165→157, 166→160, 168→163, 169→164, 170→165,
171→168, 172→169, 184→171, 189→172`.  So the current source's
`case 158:` is semantically the original's **type 166**, and the
original's type 158 is the equipment-loop body now written as
`case 156:`.  This is a real semantic bug in the decomp, but it is
**not** free to fix by relabelling: MBP lays bodies out in ascending
*label* order, so relabelling to the original types reorders the bodies
and measures **raw 904, size 1,424** (measured this session with the
`relabel.py` mapping above).  The body layout and the TBH are coupled.

## 2. The TBH needs MBP to place bodies in the original order

Original body order `T` (by VMA):
`167/174, 8, 11, 13, 14, 150,151,152,153,154,155, 158,162,166,170,184,189,
156,157,160,163,164,165,168,169,171,172, default`.
Ours is ascending type order.  `SelectionDAGBuilder::buildJumpTable`
(LLVM 7 `release_70`) adds the jump-table MBB successors in **table
(value) order**, and `MachineBlockPlacement::selectBestCandidateBlock`
ties break on that worklist order, so a plain `switch (m->getType())`
always places bodies in ascending type order.  The only lever that
changes the order is **edge probability**: the original order is exactly
what MBP produces if `167/174` has double weight, group
`{150..155,158,162,166,170,184,189}` has higher weight than group
`{156,157,160,163,164,165,168,169,171,172}`, and each group is internally
value-ordered.

That weight pattern is what `SimplifyCFG::FoldValueComparisonIntoPredecessors`
produces when a second switch sits in the `default:` of the first and the
first carries `!prof` weights (`Weights.push_back(Weights[0] *
SuccWeights[i+1])`).  Without `!prof` the fold is weight-neutral and the
order stays value order (measured: `nested_shared` [LOW+A]/[B] is
**size 1,432, raw 107**, body order still ascending).

### 2.1 `__builtin_expect` nested form: right order, wrong size

`switch (__builtin_expect(mtype, 167)) { LOW+A; default: switch (mtype) { B } }`
reproduces the exact `T` order but is not size-exact (measured this
session, all `raw / aligned / size`):

| variant | raw | al | size |
|---|---:|---:|---:|
| nested expect, LOW+A / B, ev=167 (or 174) | 1,297 | 621 | **1,420** |
| nested expect, LOW+A / B, ev=150 (or 155) | 1,330 | 682 | **1,408** |
| nested expect, LOW+A / B, ev=0 | 1,326 | 675 | **1,388** |
| nested expect, A / LOW+B, ev=167 | 1,321 | 607 | **1,360** |
| nested expect, A / LOW+B, ev=150 | 1,316 | 650 | **1,384** |
| nested expect, T-ordered source, LOW/A/B, ev=150/174/155/156 | 1,321 | 566 | **1,404** |
| nested expect, T-ordered source, LOW/A/B, ev=8 | 1,352 | 673 | **1,348** |

The expect also changes the low-group table from `tbh` to `tbb` and
perturbs the whole function's layout, so none of these is landable
(size must stay 1,432).  A 36-variant sweep (current vs `T` source
order × 3 group splits × 6 expected values) produced no 1,432-byte
result.

## 3. Re-measured alternatives (all from this worktree)

| variant | raw | al | size | notes |
|---|---:|---:|---:|---|
| committed (`index,cargo,amount` + `hasEquipment(index,1)` negated) | **71** | 38 | 1,432 | best raw |
| old `.rodata` rank table (`StatusMissionTable.cpp`, `b957fbd12^`) | 74 | 38 | 1,432 | TBH still 24 B off; +3 B `blx` target; +46 case-158; +1 literal |
| relabel to original types (`relabel.py`) | 904 | 342 | **1,424** | semantic fix, layout changes |
| `nested_shared` [LOW+A]/[B] (two-switch, no expect) | 107 | 57 | 1,432 | body order still value order |
| sequential two-switch (`int mtype` shared, two switches) | 1,360 | 536 | **1,548** | 3 TBHs, does not fold |
| `Ship *s = ship` hoists (`s_hoist`, `s_hoist2`) | 694 | 139 | **1,436** | `this` still copied to r1 |
| `Ship *s = Globals::status->ship` hoists | 698 | 163 | **1,436** | |
| faithful `index,amount,cargo` + re-query positive | 682 | 145 | **1,436** | original r9/r5/r2 prefix only with `this` spill |

The old rank table's 24-byte TBH delta is structural: a rank table makes
the TBH entry `j` point at the `j`-th *placed* body, so it can only
reproduce the original table if the placement order is already value
order — in which case the direct switch already matches.  Rank tables
cannot fix the TBH.

## 4. Case-158 (production-good body) register allocation

Original (`ac242..ac2a5`, body of original type 166):
`index→r9`, `amount→r5`, `cargo→r2`, `this` stays in `r6`, tail is a
single `b.n ac458` (`cbnz r0, ac49e`).  Our committed shape uses
`index→r5`, `cargo→r6`, `amount→r1`, reloads `this` from `[sp,#16]`
(`ldr r6,[sp,#16]`) and ends `cmp r0,#0; bne.w ac45a; b.n ac49e`.
The faithful order (`index,amount,cargo` + re-query) reproduces the
`r9/r5/r2` prefix only if `this` is not clobbered, but the greedy
allocator spills `this` and reuses `r6` for `amount` (post-RA MIR
`mir_fn_vrr.txt`: `renamable $r1 = COPY killed renamable $r6;
renamable $r6 = COPY $r0` and `$r6 = t2LDRi12 %stack.0` before the
`Bcc`).  Hoisting `ship` into a local makes the allocator use `r9` for
`ship` and `r6` for `index` instead.  Every faithful shape is 1,436 B.

## 5. Conclusion

No size-exact shape below raw 71 was found.  The residual is
24 B TBH (needs the `T` placement order, reachable only with `!prof`
weights, and every `__builtin_expect` form tried changes the size) +
46 B production-good body (greedy register allocation) + 1 B literal
pool (GOT slot order, downstream of the body size).  `src/` is left
unchanged.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13618
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN6Status16missionCompletedEbbx     # raw=71 aligned=38 size=1432/1432
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13618/`: `Status.base.cpp`,
`chunks.py`, `mk2.py` (nested/shared/expect builders), `reorder.py`,
`splice.py`, `relabel.py`, `exp.sh` (build + fn_diff + TBH decode +
diffmap), `decode2.py`/`decode3.py`, `diffmap.py`, `MBP.cpp`,
`SDB.cpp`, `SCFG.cpp`, `BPI.cpp`, `LEI.cpp` (LLVM 7 sources used for
the placement analysis), `mir_fn_vrr.txt` (post-RA MIR of the faithful
shape).
