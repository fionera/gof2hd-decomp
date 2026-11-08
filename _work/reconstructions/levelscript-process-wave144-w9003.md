# LevelScript::process — wave 144 / w9003 (prologue `strd` slot order:
# StackSlotColoring weight, not declaration order)

Status: **analysis-only, no source change** (gate unchanged, 2026-11-08).
Target `_ZN11LevelScript7processEi` (`LevelScript::process`), original
`0x137d18`, **57232 B (0xdf90)**, size-exact. Owned TU:
`src/game/world/LevelScript.cpp`. Base: wave-144 head `eb13ada9b`
(byte_exact 4507, raw 54,909).
Scratch helpers in `/dev/shm/w9003/` (not tracked): `fdiff.py`,
`measure.py`, `perm.py`, `batch*.py`, `weights*.py`, `wblocks.py`,
`regalloc.txt` / `ssc.txt` / `pei.txt` / `bfi.txt` (clang machine dumps).

## Result

```
                         before (HEAD)     after (this pass)
process raw diffbytes    54,909            54,909            (0)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
RATCHET                  PASS              PASS
```

## The prologue pair

```
orig  137d6e  e9cd 5612   strd r5, r6, [sp, #72]   ; r5=messages @72, r6=delta @76
ours  137d74  e9cd 6512   strd r6, r5, [sp, #72]   ; r6=delta @72, r5=messages @76
```

The register assignment is identical in both (messages=r5, delta=r6); only
the **stack slot index** differs.  The pair is formed by the post-RA
`ARMLoadStoreOptimizer` (runs in `addPreSched2`, i.e. after regalloc), which
merges the two adjacent spill stores into one `STRD` in ascending-slot order.
So the root cause is which of the two spilled live ranges got the lower
`MachineFrameInfo` object index.

## Where the slot order comes from (new, machine-verified)

The two values are ordinary greedy-allocator spill slots.  After
`VirtRegRewriter`, `StackSlotColoring` (runs unconditionally in
`TargetPassConfig::addMachinePasses`) re-colours every `LiveStacks`
interval:

* `InitializeSlots` sorts the spill intervals by `LiveInterval::weight`
  (descending, `IntervalSorter`).
* `ColorSlot` gives the heaviest interval the first free colour, which is
  the **lowest** original frame index (`AllColors[].find_first()`).
* PEI then assigns offsets in frame-index order, and a lower frame index
  lands at a **higher** address (`AdjustStackOffset`, stack grows down;
  verified against the stack-protector `fi#0` at `sp+428`).

Weight is `ScanForSpillSlotRefs`: the sum of
`LiveIntervals::getSpillWeight(false, true, MBFI, MI)` over every non-debug
machine instruction that references the slot, i.e. the **sum of the
block frequencies** of the blocks that load/store it.

Dumping the machine function confirms it (`-mllvm
-print-machineinstrs=greedy` / `=stack-slot-coloring` /
`=prologepilog` plus `-mllvm -print-machine-bfi`):

```
final colour  value      weight   refs   offset
675           enemies     4.26     358    sp+96
676           status     10.85     428    sp+92
677           this        5.85     428    sp+88
678           player      5.58     320    sp+84
679           &m_pLevel   4.98      50    sp+80
680           messages    4.67     120    sp+76
681           delta       4.53     390    sp+72
```

(the weights are from the `MachineBlockFrequencyInfo` snapshot that
`StackSlotColoring` actually consumes; the order reproduces the observed
colour assignment exactly).  **messages beats delta by only 0.14.**  The
whole difference is one hot block:

* messages: `bb0` 2.0 + `bb221` 0.918 + **`bb65` 0.625** + `bb236` 0.276 +
  `bb243` 0.207 + ...
* delta:    `bb0` 2.0 + `bb221` 0.918 + `bb1105` 0.222 + `bb1129` 0.135 + ...

`bb65` is the mission==1 arm and its `(*messages)[2]` load
(original `0x137e18 ldr r0,[sp,#72]`).  Delta has no load in that block —
it stays in r6 across the arm.

## Why "declaration order" cannot fix it (measured)

All six permutations of the three prologue locals were built and linked on
this head:

| declaration order | size | raw diff | prologue `strd` |
|---|---:|---:|---|
| enemies, player, messages (HEAD) | 57232 | 54,909 | `r6,r5` |
| enemies, messages, player | 57232 | 54,909 | `r6,r5` |
| player, enemies, messages | 57224 | 55,039 | — |
| player, messages, enemies | 57308 | 55,392 | `r6,r5` |
| messages, enemies, player | 57308 | 55,392 | `r6,r5` |
| messages, player, enemies | 57308 | 55,380 | `r6,r5` |

The original's accessor order is `getMessages, getPlayer, getEnemies`
(`0x137d3e/46/50`); declaring `messages, player, enemies` reproduces it
exactly, but the `strd` still stores `r6,r5` (delta first) and the function
grows to 57,308 B.  Splitting declaration from initialisation
(`T *x; ... x = ...;` in five different orders), renaming the parameter and
copying it to a local (`int delta = delta_;` before/after the accessors),
and `T *x;` + assignment all produce **byte-identical** code to HEAD
(raw 54,909, same `strd`).  Clang coalesces the copy and its scheduler
fixes the emitted accessor order, so the C++ declaration order never
reaches the spill weights.

## Variants measured (all size-exact unless noted)

| edit | size | raw diff | note |
|---|---:|---:|---|
| HEAD | 57232 | 54,909 | |
| state-2 arm written before state-1 arm | 57232 | **54,890** | -19, but the dispatch becomes `cmp #1/beq cold; cmp #2/fall-through`, i.e. the opposite of the original `cmp #2/beq cold; cmp #1/fall-through` (0x13c04e). **METRIC-ONLY**, reverts the w8902 faithful shape; not landed |
| `if (m_nState == 0 && msg2->isOver())` (swap short-circuit) | 57404 | 55,352 | frame grows, `strd` moves to `[sp,#64]`; needs a -172 companion |
| copy parameter to local, 5 declaration/assignment split forms | 57232 | 54,909 | byte-identical codegen |

## Conclusion / next lead

The `strd` slot order is a pure **block-frequency-weight** artefact, and the
current source is only 0.14 below the flip point.  It cannot be moved by any
declaration/parameter order (proved by the six permutations plus five
split-declaration forms, all byte-identical).  To flip it the source must
change which *blocks* reference the two slots — e.g. make the mission==1
`(*messages)[2]` load land in a colder block, or force one extra delta
reload in a block of frequency > 0.14.  The most promising faithful lead is
therefore the mission==1 arm's block layout (its `msg2` load is the single
0.625 contributor), not the prologue declarations.  A `strd` flip would
recover the ~78 raw bytes of `[sp,#72]`/`[sp,#76]` operand swaps scattered
through the function.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w9003/fdiff.py _ZN11LevelScript7processEi   # 54,909
python3 /dev/shm/w9003/perm.py                              # six declaration orders
# machine dumps:
clang++ ... -mllvm -print-machineinstrs=stack-slot-coloring
clang++ ... -mllvm -print-machine-bfi
```
