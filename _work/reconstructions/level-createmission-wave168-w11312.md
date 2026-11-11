# w11312 (wave 168): `Level::createMission` — convoy-switch block layout decoded
# (no raw win; 5,435 / 767 / 17,172 B unchanged)

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w11312`, branch `wave168-w11312`,
base master `631c26f0b`.  Continues `level-createmission-wave167-w11211.md`
(frame map + scope/lifetime sweep) and `level-createmission-wave167-w11210.md`
(guard `icmp` de-sinking).

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, `0x4314` = 17,172 B).

## Baseline (pristine HEAD, re-measured)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw 5435   align 767   insn 6204/6197
```

## 1. NEW: the convoy `switch (k)` block layout is the source of the +2 shift

The whole `0x1468..0x1a10` region (1,004 B) is a constant **+2 shift** of the
original (ours is 2 B shorter).  Root cause found:

* IR (post-opt, `-Oz -emit-llvm`): `switch i31 %2987, label %2992 [ i31 0 ->
  %2989, i31 1 -> %2990, i31 2 -> %2991 ]`, blocks in IR order
  `case0, case1, case2, default, join`.
* `SelectionDAGBuilder::lowerWorkItem` emits the compare chain in cluster
  order and inserts one test block per cluster after the switch block; the
  first successor added is the *first* cluster's case block, so
  `MachineBlockPlacement` places `case0` as the switch fallthrough.
* After `case0`, the remaining worklist order is the MBB order
  (`case2, case1, default`), and `selectBestCandidateBlock` keeps the first
  of equal-frequency blocks -> layout `case0, case2, case1, default`.
* Original layout is `case0, case1, case2, default` (ascending), i.e. its
  worklist order was `case1, case2, default`.

The second half of the difference is the **tail store**:

```text
ORIGINAL                                   OURS
b1420 case0 (ldr,str,ldr,str,ldr)          b1420 case0  (same)
b142c case1 (ldr,str,ldr,str,ldr)          b142c case2  (ldr,str.w sl,str,ldr)
b1438 case2 (ldr,str.w sl,str,ldr)         b1438 case1  (ldr,str,ldr,str,ldr)
b1444 default (ldr,str,ldr,str,ldr)        b1442 tail   str r1,[sp,#380]
b144e tail   str r1,[sp,#380]              b1444 b join
b1450 join                                 b1446 default (ldr,str,ldr,str,ldr)
                                           b1452 join
```

* Original: **all four** cases end with the same `str r1,[sp,#380]`
  (`offset[2]`), so BranchFolder tail-merges a 4-way shared tail that is
  placed *after* `default`; every case ends in an explicit `b`.
* Ours: only the three real cases share the tail (the `default` body's last
  store is `[376]`), so the tail sits between `case1` and `default` and
  `case1` falls through into it (one `b` saved = the 2-byte shortfall).
* Original default store order (from the machine code) is
  `offset[1]=6000, offset[0]=12000, offset[2]=-45000`; ours is
  `offset[0], offset[2], offset[1]` (current source order `2,0,1`).

## 2. Default-store-order sweep (all `link_so -j2`, `fn_diff`)

Source order -> machine store order is a non-identity permutation; only two
orders change the size:

| default source order | machine order | size | raw | aligned | note |
|---|---|---:|---:|---:|---|
| `2,0,1` (HEAD) | `0,2,1` | 17172 | **5435** | **767** | 3-way tail, case1 falls through |
| `2,1,0` | `1,2,0` | 17172 | 5438 | 764 | no tail merge |
| `2,0,1` variants | - | 17172 | 5435-5442 | 762-769 | neutral |
| `0,2,1` | `2,0,1` | 17172 | 5441 | 767 | no tail merge |
| `1,0,2` | `1,0,2` | **17188** | 11346 | 805 | 4-way tail, but value kept in `s0` |
| `0,1,2` | `1,0,2` | **17164** | 11330 | 838 | compiler turns switch into a table lookup |

* `0,1,2`: all four bodies become structurally identical, so the whole
  switch is strength-reduced to a literal-pool table
  (`vldr s4,[r1,#-40]` etc.) — 8 B short, unusable.
* `1,0,2`: gets the 4-way tail but isel keeps the merged value in a
  callee-saved FP register (`vmov.f32 s0,s16/s18/s20/s22` + `vstr
  s0,[sp,#380]`) instead of the original's `ldr r1,[pc]` + `str r1` — +16 B.
* Case order (`0,1,2` / `0,2,1` / `2,1,0` / `1,0,2` / `default` first) is
  irrelevant: `sortAndRangeify` + `lowerWorkItem` sort by (prob, value), so
  every source order gives the same IR cluster order; only the default body
  order changes codegen.

## 3. Shift map (ours relative to orig, 64-byte windows, normalised insns)

```text
offset   shift    cause
0x0000   +0       prologue/early body exact
0x1460   +2       convoy switch (case1 falls through the shared tail)
0x15a0   +4       case2 body (str.w sl) shorter
0x1640   +2       back
0x19a0   -2       convoy tail
0x2e20   -8       alien-wave guard: ours 14 B (ldr/cmp/beq/cmp/bne),
                  orig 22 B (subs/mov/it/movne/ldr/eor/orrs/bne)
0x2f40   -14      second guard (pirateFortress) same 6-byte extra
0x31a0   -12      +2 recovered
0x3260   -14
0x32e0   +2       alien-gun loop entry
0x33c0   +4
0x3500   +0
0x3780   -2..-6   ArraySetLength sum / raid block scheduling
0x39c0   -8
0x3ac0   -4
0x3be0   -8
0x3c40   -2
0x3c80   +0
0x3e20   +4
0x4100   -2
0x4200   -6
0x4260   +0
0x4280   -4
```

The `-8`/`-14` guard deltas are the w11210 `SinkCmpExpression` finding: the
original keeps `xor i1 !alienWave` in the loop block (`eor r2,r2,#1`), ours
sinks the `icmp` and `FindMergedConditions` splits the `and` into two
branches.  Every size-exact de-sinking shape measured in wave 167 was
raw-worse because `!alienWave` gets hoisted/spilled.

## 4. Raid-block hoist sweep (all worse)

The original loads `Status*` (`sp+108`) *before* the `if (raid)` branch and
`fortressGuns` (`sp+40`) before it too; ours loads `Status*` inside.

| variant | size | raw | aligned |
|---|---:|---:|---:|
| HEAD | 17172 | **5435** | **767** |
| hoist `int guns` above `if (raid)` | 17176 | 5993 | 726 |
| hoist `Status *status` above `if (raid)` | 17172 | 6394 | 1183 |
| hoist `raidType` above `if (raid)` | 17188 | 14581 | 1356 |
| hoist all three | 17172 | 14469 | 1357 |

## 5. Conclusion

* The convoy +2 shift is fully explained: it is the 4-way vs 3-way shared
  `offset[2]` tail store plus the case1/case2 block order.  Both are decided
  by BranchFolder/MachineBlockPlacement, not by any source order reachable
  from the switch (all 6 case orders and all 6 default store orders tried).
* The only way to get the original's `str r1` tail is for isel to keep the
  merged value in a GPR; the `1,0,2` default order gets the 4-way tail but
  isel picks `s0` (+16 B), and the `0,1,2` order triggers a table lookup
  (-8 B).  No size-exact shape found.
* Nothing landed; tree left clean at the baseline (5,435 / 767 / 17,172 B).

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
```

Scratch: `/dev/shm/w11312/{orig.dis,ours.dis,side.py,regions.py,shift.py,
shiftmap.py,ldrdiff.py,mkvariant.py,mkdef.py,mk2.py,mk3.py,measure.sh,
Level.cpp.base,Level.ll,Level0.ll,sdb.cpp,mbp.cpp,cgstmt.cpp}`.
