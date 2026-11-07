# `Status::missionCompleted` — wave 131 (w7695): case-162 is now
# instruction-identical; three NEW single-table body-order hypotheses
# (if-chain+expect, two-switch on `m->id`, `!prof` weight sweep) all fail to
# reach 1432 B

Status: **note-only** (no source change landed; gate kept at byte 4506 /
linked 4506).  Worktree `wt/w7695`, branch `wave131-w7695`, base
`7c6933c2d` (wave-130b snapshot).  Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 r328903 / LLVM `1d739ffb`, `-Oz`,
image base 0x10000).

Target: `_ZN6Status16missionCompletedEbbx` (`Status::missionCompleted(bool,
bool, long long)`), original `0xabf38`, size **1432 B**.

| | baseline (HEAD `7c6933c2d`) |
|---|---:|
| function score | **71.0 %** (523 vs 525 normalised insns) |
| function size | 1432 B |
| gate byte_exact | 4506 |
| gate linked_exact | 4506 |
| `.text` differing bytes | 85,379 |

## 0. Summary

1. **Case 162 is now instruction-identical to the original.**  The wave-130
   `continue` form is confirmed body-for-body (the only remaining textual
   difference in that body is the shared-tail branch *target*, which is a
   layout artifact).  Case 163 is the only remaining real source-shape
   mismatch in the whole function.
2. **Case-163 j-loop fix alone is not landable**: it shrinks the function to
   **1428 B** (4 B), which shifts `.text` and drops `byte_exact` 4506 →
   **3044** (linked_exact stays 4506).  The 4 B are branch-width narrowing
   (`bgt.w`→`ble.n` and `bcs.w`→`bcs.n`) that only balance at the original
   body order.
3. **Three new single-table body-order routes were tested and all fail on
   size** (§2): an `if`/`else-if` chain with `__builtin_expect` on the A
   conditions (single TBH, but SimplifyCFG drops the weights → value order,
   1392 B), a two-switch form on `m->id` (exact order, 1448 B), and a
   `!prof branch_weights` sweep (exact order, but sizes 1412/1424/1428 —
   never 1432).

## 1. Per-body audit (current build)

Decoded both TBHs and compared each body (branch targets normalised,
`/dev/shm/w7695/perbody.py`).  Bodies 150,151,152,153,154,155,162 and the
low-group 0/11,13,14 are instruction-identical.  The rest differ only in
branch width / shared-tail distance caused by the wrong body order, plus
case 163's polarity:

```
163 o=047e u=0396 len 11/11
     cmp  r2, r1
    -bcs.n  loc_566        (orig, 2 B)
    +bcs.w  loc_566        (ours, 4 B -- tail is 392 B away, not 232 B)
     ...
     cmp.w r3, #4294967295
    -ble.n  loc_486        (orig, 2 B)
    -b.n    loc_522
    +bgt.w  loc_522        (ours, 4 B)
    +b.n    loc_39e
```

This confirms the wave-128/130 conclusion: the source shape is known; only
the layout is wrong.

## 2. New body-order experiments (all measured this pass)

Body order decoded from the `.o` TBH (`/dev/shm/w7695/order.py`).  Original
order (relative body starts, 0xabf38 base):

```
150 151 152 153 154 155 158 162 166 170 184 189   (A run)
156 157 160 163 164 165 168 169 171 172           (B run)
```

| source shape | size | high-TBH body order |
|---|---:|---|
| current single switch | 1432 | value order (150…189) |
| **`if`/`else-if` chain, `__builtin_expect(cond,1)` on A cases** | **1392** | value order |
| **two switches, first on `m->getType()`, second on `m->id`** | **1448** | **exact original** |
| **`!prof branch_weights` (A>B), `-disable-llvm-passes`** | **1412/1424/1428** | **exact original** |

### 2a. if-chain + `__builtin_expect`

The whole switch was rewritten as

```cpp
int mtype = m->getType();
if (__builtin_expect(mtype == 167 || mtype == 174, 1)) { ... }
else if (mtype == 8) { ... }
...
else if (__builtin_expect(mtype == 189, 1)) { ... }
else if (mtype == 156) { ... }
...
```

Clang folds it back into a **single TBH**, but the optimised IR switch has
**no `!prof` metadata**: the fold path used here
(`SimplifyBranchOnICmpChain` / `FoldValueComparisonIntoPredecessors`) does
not carry `__builtin_expect` weights through to the switch.  The IR case
operand order even changes (`174, 167, 8, 11, 0, 13, 14, …`), and the layout
is value order.  Size 1392 B, so also wrong on size.

### 2b. two switches on `m->id`

Splitting the switch before `case 156` and putting the B cases in a second
`switch (m->id)` gives the **exact original body order** and keeps the A
bodies contiguous, but:

* the first switch's table only spans **150..174** (25 entries), because
  without the B cases 184/189 are sparse and clang emits two explicit
  compares instead of extending the table to 40 entries;
* the second switch adds its own `subs/cmp/bhi/tbb` + 17-byte table.

Net size **1448 B** (+16).  Using `m->getType()` for the second switch is
code-identical (getType is CSE'd to the `id` load).  This is the wave-130
"two-switch" form re-measured against the current (case-162-continue) base;
the earlier 1464 figure was the pre-wave-130 source.

### 2c. `!prof` weight sweep

Optimised IR (`-g0 -fPIC -Oz`) was given
`switch i32 %52, … !prof !{!"branch_weights", …}` in the switch's operand
order (default, 167,174, 8,0,11,13,14, A cases, B cases).  With
`default=1, 167/174=3, A=2, B=1, low=10` the layout is **exactly the
original** and the size is **1428 B**.  Sweeping `low` 8..20 and
`default` 1..8 only produces 1412/1424/1428 — the low-group tail merging is
extremely sensitive to the low-group weight and never lands on 1432.
Representative:

```
low=8  -> 1424      low=10 -> 1428      low=11 -> 1428
low=14 -> 1412      low=20 -> 1412      low=100 -> 1364
```

This independently confirms the wave-129 finding: the original layout *is*
a switch-edge-frequency (PGO) effect, but the exact `!prof` weight set also
changes low-group tail merging, so "A>B weights" alone is not the original
mechanism.

## 3. Reproduction

```sh
# exact-flag compile of the real TU (0.4 s)
/dev/shm/w7695/cc.sh src/game/mission/Status.cpp /dev/shm/w7695/status.o
python3 /dev/shm/w7695/order.py   /dev/shm/w7695/status.o   # body order + size
python3 /dev/shm/w7695/perbody.py cmake-build-match/libgof2hdaa.so -v

# fast single-function score (needs the linked .so)
python3 /dev/shm/w7695/fastverify.py cmake-build-match/libgof2hdaa.so

# if-chain variant generator
python3 /dev/shm/w7695/mkifchain.py   # writes /dev/shm/w7695/variant.cpp
# two-switch / !prof variants: /dev/shm/w7695/mkvariant.py + mkprof2.py
```

Scratch (not committed): `/dev/shm/w7695/` — `cc.sh`, `ccll.sh`,
`bodies.py`, `order.py`, `perbody.py`, `fastverify.py`, `mkifchain.py`,
`mkvariant.py`, `mkprof.py`, `mkprof2.py`, `status.ll`, `ifchain.ll`.

## 4. Open question

The exact body order is reproducible from source only via a **second
dispatch** (+16 B, two switches) or via **PGO weights** (which also perturb
low-group tail merging, 1412–1428 B).  A source shape that empties
`MachineBlockPlacement`'s worklist at the switch (so the fallback walks MBB
order) *without* adding a dispatch block would give 1432 B + exact order.
The two-switch form shows the required ingredient is a second unplaced
predecessor on the B bodies; the missing piece is making it free.
