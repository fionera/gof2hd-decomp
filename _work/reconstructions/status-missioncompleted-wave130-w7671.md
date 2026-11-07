# `Status::missionCompleted` — wave 130 (w7671): the fallback is confirmed on
# the wave-129 source, but the function-level `markChainSuccessors` re-pushes
# the bodies in **successor (value) order**, so cold-block filtering cannot
# empty the worklist; the `if/else` shape is 1432 B but scrambles the layout

Status: **note-only** (no source change landed; gate kept at 71.1 %, 1432 B).
Worktree `wt/w7671`, base master `003a9623a` (wave-129 snapshot, byte 4506 /
linked 4506).  Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 r328903 / LLVM `1d739ffb`, `-Oz`,
image base 0x10000).

Target: `_ZN6Status16missionCompletedEbbx` (`Status::missionCompleted(bool,
bool, long long)`), original size **1432 B**.

| | baseline (HEAD `003a9623a`) |
|---|---:|
| function score | **71.1 %** (523 vs 524 normalised insns) |
| function size | 1432 B |
| gate byte_exact | 4506 |
| gate linked_exact | 4506 |
| data_exact | 843/843 |
| imports | 207 |
| lint / drift | CLEAN / 0 |

## 0. What this pass adds over w7649

w7649 proved that the original's high-table body order is the MBP
`getFirstUnplacedBlock` fallback (MBB order) and that
`MBP_SKIP_JT_PUSH` / `MBP_SORT_SBCB` / `MBP_FORCE_FALLBACK` reproduce it.
This pass re-verified that on the **wave-129 source** (with the five
`__builtin_expect` hints) and then attacked the open question ("why is the
worklist not empty?") with three new experiments:

1. **Cold-block filtering cannot be the trigger.**  `-force-loop-cold-block`
   with `-loop-to-cold-block-ratio=4` really does remove all 22 high bodies
   from the loop `BlockFilterSet` (verified from the `LOOPSET` trace), yet the
   final body order is still **ascending value order**, not source order.
   Reason: when the loop chain is merged into the function chain,
   `buildCFGChains` calls `buildChain` with **no** `BlockFilter`, so
   `markChainSuccessors` walks `MBB->successors()` of the high jump table and
   re-pushes every body in successor order.  The worklist is value-ordered at
   the function level too (`MBP_RAW`: `bb.36(150) bb.38(151) bb.39(152)
   bb.44(153) bb.45(154) bb.46(155) bb.77(156) bb.80(157) bb.47(158) …`).
2. **The `if/else` shape is a real layout knob but not the original layout.**
   Rewriting the switch as an `if / else if` chain keeps 1432 B but produces a
   completely different body order (17.8 %).
3. **The cold-block threshold is exactly on the edge.**  `LoopFreq = 45090`,
   each high body `f = 8256`, so `LoopFreq/f = 5` and the test is `> 5`:
   the bodies are *kept* by one unit.  A profile that lowered the body
   frequency below 7515 (`q >= 6`) would exclude them — but experiment 1
   shows that even exclusion does not give source order.

## 1. Baseline evidence (measured this pass)

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6
# verify: byte 4506  linked 4506  stubs 0  missing 0  extra 0  avg 99.94
# data:   exact 843 of 843 exported objects
# sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
# RATCHET PASS
```

Single-function score (fast, no full verify):
`pct=71.1 osize=1432 usize=1432 oinsn=523 uinsn=524`.

## 2. Re-confirmation: the fallback order is the original order

Compiled the current `Status.cpp` to optimised IR with the project flags and
ran the project's instrumented LLVM 7.0.0 `llc`
(`/home/debian/llvm7build/build/bin/llc`) with `MBP_FORCE_FALLBACK=1`.
Decoding the emitted TBH (`/dev/shm/w7671/order.py`) gives

```
167/174 150 151 152 153 154 155 158 162 166 170 184 189
156 157 160 163 164 165 168 169 171 172  default
```

which is byte-for-byte the original's body order (decoded from the `.so` with
`order_so.py`).  Stock `llc` on the same IR gives ascending value order.
So the wave-129 source still has the right MBB order; only the placement
mechanism is wrong.

## 3. New experiment: cold-block filtering does not produce source order

`collectLoopBlockSet` excludes cold blocks only when
`Function::hasProfileData() || ForceLoopColdBlock`.  Instrumented the local
LLVM with `MBP_DUMP_COLD` (prints `LoopFreq` and each block's quotient):

```
LOOPFREQ %bb.1 = 45090 ratio=5
  COLD? %bb.36 f=8256 q=5      ; case 150
  COLD? %bb.38 f=8256 q=5      ; case 151
  ...
  COLD? %bb.16 f=16512 q=2     ; 167/174 (two case values)
```

`q = 5` is **not** `> 5`, so every high body stays in the loop set.  With
`-loop-to-cold-block-ratio=4` they are all removed (`LOOPSET` no longer
contains `bb.36/38/39/44/45/46/77/80/47/85/57/88/91/92/62/93/96/66/97/98/67/71`),
but the emitted body order is still

```
167/174  default  150 151 152 153 154 155 156 157 158 160 162 163 164 165
166 168 169 170 171 172  184 189
```

i.e. value order (with the latch/default hoisted to second place), **not**
source order.  The `MBP_RAW` candidate list at the function level is the same
value order as before.

Why: `buildCFGChains` does

```cpp
SmallPtrSet<BlockChain *, 4> UpdatedPreds;
for (MachineBasicBlock &MBB : *F)
  fillWorkLists(&MBB, UpdatedPreds);          // BlockFilter == nullptr
buildChain(&F->front(), FunctionChain);       // BlockFilter == nullptr
```

and `markChainSuccessors` -> `markBlockSuccessors` iterates
`MBB->successors()`, which for the high jump table is `buildJumpTable`'s
`Table` order (ascending value).  The bodies are therefore re-pushed in value
order at the function level even though they were filtered out of the loop
chain.  **Cold-block filtering is a dead end.**

## 4. New experiment: the `if/else` shape

Generated a faithful `if / else if` rewrite of the switch (same bodies, same
source order, `break;` stripped because `else if` already yields the same
control flow).  Compiled with the exact project flags:

| form | size | high-table body order |
|---|---:|---|
| `switch` (landed) | 1432 | value order, 71.1 % |
| `if / else if` (A-then-B source order) | **1432** | `167/174 150 151 153 154 155 156 160 162 164 165 166 168 169 170 171 172 default 184 189 152 157 158 163` |
| `if / else if` (ascending source order) | 1348 | value order-ish, too small |

The A-then-B `if/else` chain keeps the size but scores **17.8 %** in the full
link (534 vs 523 insns) — the four bodies that contain `for` loops
(152, 157, 158, 163) are pushed to the end of the layout.  So the `if/else`
shape is a layout knob, but not the original one.

## 5. Other variants re-tested (all keep the value-order layout)

| variant | size | note |
|---|---:|---|
| explicit `case 159/161/173/175..183/185..188: break;` | 1432 | identical layout; `Done` set dedups the latch successor |
| explicit `default: break;` at the end | 1432 | identical |
| all five `__builtin_expect` hints removed | 1428 | identical value order (the hints are a size anchor) |
| `while (i < missions->size()) { … i++; }` | 1404 | value order, 184/189 after default |
| two switches on `int type` (A then B) | 1504 | 3 `tbh`, not the original single table |
| `-mllvm -force-loop-cold-block -mllvm -loop-to-cold-block-ratio=4` | 1440 | value order (see §3) |

## 6. Conclusion / open question

* The original body order is still exactly the MBP fallback (MBB/source
  order) and the wave-129 source's MBB order is still exactly the original
  order.
* The fallback needs `BlockWorkList` to be empty when the high jump table is
  merged.  `markBlockSuccessors` pushes every body whose
  `UnscheduledPredecessors` reaches 0, and `buildJumpTable` adds the bodies
  in ascending-value order.  The only ways out are:
  (a) each body has **more than one** unplaced predecessor at that moment
      (so the decrement stops at 1), or
  (b) the bodies are already in the current chain.
  Neither is reachable from the plain single-switch source: every body has
  exactly one predecessor (the jump-table MBB).
* Cold-block filtering (the only profile-driven way to remove bodies from the
  loop set) is **not** sufficient — the function-level `markChainSuccessors`
  re-pushes them in successor order (§3).
* The remaining plausible explanation is still a real `-fprofile-use` build
  whose *branch weights* make the A bodies hotter than the B bodies (the
  wave-129 `!prof` result: exact order, 1444 B), i.e. not expressible from
  C++11.  A future pass should look for a source shape that gives each case
  body a second predecessor (e.g. a fallthrough chain) rather than more
  cold-block/if-else variants.

## 7. Reproduction

```sh
# fast single-function score
python3 /dev/shm/w7671/score.py
# pct=71.1 osize=1432 usize=1432 oinsn=523 uinsn=524

# body order of an object / of the original .so
python3 /dev/shm/w7671/order.py    <file.o>
python3 /dev/shm/w7671/order_so.py /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so

# exact-flag compile of a variant (harness.py swaps the function into a copy)
python3 /dev/shm/w7671/harness.py /dev/shm/w7671/fn_ifelse.txt /dev/shm/w7671/v_if.o

# forced fallback (instrumented LLVM 7.0.0)
MBP_FORCE_FALLBACK=1 /home/debian/llvm7build/build/bin/llc \
  -mtriple=thumbv7-none-linux-androideabi16 -mattr=+neon -float-abi=soft -O2 \
  -filetype=obj /dev/shm/w7671/mc.ll -o /dev/shm/w7671/fb.o
python3 /dev/shm/w7671/order.py /dev/shm/w7671/fb.o

# cold-block trace (MBP_DUMP_COLD was added to the local LLVM this pass)
MBP_DUMP_COLD=1 /home/debian/llvm7build/build/bin/llc ... -force-loop-cold-block
```

Scratch (not committed): `/dev/shm/w7671/` — `score.py`, `order.py`,
`order_so.py`, `harness.py`, `cc.sh`, `ccx.sh`, `ccll.sh`, `gen_switch.py`,
`gen_ifelse.py`, `fn_*.txt`, `mc.ll`, `fb.o`, `cold*.o`, `mbp*.txt`.
The local LLVM tree `/home/debian/llvm7build` was patched with an
`MBP_DUMP_COLD` print in `collectLoopBlockSet` (debug-only, does not affect
codegen).
