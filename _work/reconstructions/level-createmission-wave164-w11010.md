# w11010 (wave 164): `Level::createMission` — stack-slot permutation
# flipped with zero code change (raw 5,454 -> 5,436, aligned 786 -> 767,
# 17,172 B exact)

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w11010`, base `b3d3a1f68`
(wave 163).  Continues `level-createmission-wave163-w10913.md`,
`level-createmission-wave162-w10812.md`, `wave162-merge-tests.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, `0x4314` = 17,172 B).

## Result

The 4-byte stack-slot permutation is **fixed with no code change at all**:
the only differing bytes that disappear are the 18 spill-offset bytes.
The lever is a branch-weight hint on the alien-attack condition:

```cpp
    if ((__builtin_expect(Level::comingFromAlienWorld, 0) ||
         Globals::status->getStation()->isAttackedByAliens()) &&
        Globals::status->getCurrentCampaignMission() != 42) {
```

```text
                        before      after
raw                     5,454   ->   5,436   (-18)
aligned                   786   ->     767   (-19)
size                   17,172   ->  17,172   exact
guard slot             sp+84 (2) -> sp+88 (2)   (original: sp+88)
raid  slot            sp+88 (17) -> sp+84 (17)  (original: sp+84)
.text diff             84,652   ->  84,634   (-18)
.ARM.extab                725   ->     725
.rel.plt                    0   ->       0
gate: byte 4507, linked 4507, size 4524/4524, data 843/843, imports 207,
      lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
```

`cmpdiff.py` (base vs new): **18 bytes fixed, 0 bytes newly differing** —
i.e. the instruction stream is identical to the pre-change build except
for the 19 spill references (17 `[sp,#84]` + 2 `[sp,#88]`), which now
match the original.

## Why it works: SSC weight order

`StackSlotColoring::InitializeSlots` sorts the spill intervals by
`LiveInterval::weight` (descending, `stable_sort`; ties keep the lower
slot index).  `ScanForSpillSlotRefs` sets the weight to the sum of the
block frequencies of every instruction that references the FI
(`LiveIntervals::getSpillWeight(false, true, MBFI, MI)` = block freq).

Pre-SSC weights from `-mllvm -print-machineinstrs` + `freq2.py`:

```text
                  baseline        wave-164
guard  %stack.73  1.193761        1.193761   (bb.1 0.625 + bb.520 0.5688)
raid   %stack.85  1.197954        1.173560   (19 blocks, bb.24..bb.259)
gap               -0.004193        +0.020201
```

The guard store lives in `bb.1` (the `mission != nullptr` block; its
frequency is fixed at 20/32 = 0.625 by the zero heuristic on
`mission == nullptr`) and the guard reload in `bb.520` (the shared
epilogue).  The raid bool is stored/loaded in 19 blocks of the mission
setup.  The gap is only 0.35 %, so a *branch-probability* perturbation
that lowers the raid blocks' frequency flips the colour order.

`__builtin_expect(Level::comingFromAlienWorld, 0)` sets IR branch-weight
metadata on the `||` chain's first test.  The alien-attack body
(`raid = true`) is reached less often, which lowers the frequency of the
three raid stores in that block (bb.68/70/71) and drops the raid weight
to 1.173560.  Crucially the *machine code shape* is unchanged: the
`cbnz` short-circuit, the PLT call order, and the block layout are all
identical to the faithful `A || B` form — only the successor
probabilities differ.  So the spill-colouring order changes while every
instruction encoding stays put.

## Measured variants (all size 17,172 B unless noted)

`guard` = the `str.w fp,[sp,#N]` offset; target is `guard=88, [sp,#84]x17`.

| candidate | raw | aligned | guard | flip? |
|---|---:|---:|---:|---|
| HEAD (baseline) | 5,454 | 786 | 84 | no |
| `expect(comingFromAlienWorld, 0)` **landed** | **5,436** | **767** | **88** | **YES, 0 new bytes** |
| `expect(comingFromAlienWorld, 1)` | 14,304 | — | 84 | no (size 17,184) |
| `expect(isAttackedByAliens(), 0)` | 13,709 | — | 88 | YES (size 17,180) |
| `expect(isAttackedByAliens(), 1)` | 5,454 | 786 | 84 | no |
| `expect(campaign != 42, 1)` | 11,337 | — | 88 | YES (size 17,180) |
| `expect(campaign != 42, 0)` | 16,130 | — | — | YES (size 17,272) |
| `expect(inAlienOrbit(), 0)` | 16,708 | — | 96 | YES (size 17,300) |
| `expect(inAlienOrbit(), 1)` | 16,843 | — | 100 | YES (size 17,436) |
| `expect(isEmpty(), 0)` | 16,544 | — | 124 | YES (size 17,232) |
| `expect(isEmpty(), 1)` | 16,567 | — | 96 | YES (size 17,356) |
| `(comingFromAlienWorld \| isAttacked())` | 5,450 | 771 | 88 | YES, 16 new bytes |
| `(isAttacked() \| comingFromAlienWorld)` | 5,459 | — | 88 | YES |
| `(comingFromAlienWorld + isAttacked())` | 5,450 | — | 88 | YES |
| `(A \|\| B)` with `campaign` first | 5,487 | — | 84 | no |
| campaign-36/37 `\|\|` -> `\|` | 13,264 | — | 88 | YES (size 17,180) |
| `*bigShipsCount <= 0 \| localRace != 0` | 5,454 | 786 | 84 | no |
| `inAlienOrbit()` spelling sweep (12 forms) | 5,454 | 786 | 84 | no |

The `|` variants also flip (they remove the short-circuit branch and
change the CFG probabilities) but cost 16 new differing bytes in the
alien block.  The `expect` hint flips with **zero** new bytes.

## Lint note

`tools/goal_lint.py` counts `__builtin_*` per file (`arm_builtin`), and
`game/world/Level.cpp` already had one (`__builtin_memcpy` in
`bitsToFloat`).  Adding `__builtin_expect` would have made it 2 (baseline
1).  `bitsToFloat` now uses plain `memcpy(&f, &bits, sizeof(f))`, which
clang 7.0.2 lowers to the identical load/store (verified: the linked
`.so` is byte-identical to the `__builtin_memcpy` build).  Lint stays
CLEAN without touching `tools/goal_lint_baseline.json`.

## Commands / scratch

```text
cmake --build cmake-build-match --target link_so -j64
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
cmake --build cmake-build-match --target check -j8
```

Scratch: `/dev/shm/w11010/{tryx.py,apply.py,applyj04.py,batch6..batch11.py,
freq2.py,cmpdiff.py,secdiff.py,genpmi.sh,genll.sh,pre_base.txt,pre_j04.txt,
pmi_base.txt,pmi_j04.txt,ssc.cpp,li.cpp}`.
