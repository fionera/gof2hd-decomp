# `Status::missionCompleted` — wave 125 (w7508): the original layout is the
# MBB order; `!prof` weights 201:200:199 reproduce it exactly (76.0 % at
# 1436 B) but no C++ construct emits them

Status: **source change landed** (case 168 `(unsigned short)` cast
removed; score unchanged 71.0 -> 71.0, size 1432 -> 1432).  Worktree
`wt/w7508`, base master `02513de33` (wave-124 final snapshot, byte 4506 /
linked 4506).
Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(sha256 `d4c17be6...bed9483`, armeabi-v7a, NDK r18b clang 7.0.2 r328903,
`-Oz`, image base 0x10000).

Target: `_ZN6Status16missionCompletedEbbx` (`Status::missionCompleted(bool,
bool, long long)`), original `0xabf38`, size **1432 B**.

| | before | after |
|---|---:|---:|
| function score | 71.0 % | **71.0 %** |
| function size | 1432 B | **1432 B** |
| gate byte_exact | 4506 | **4506** |
| gate linked_exact | 4506 | **4506** |
| data_exact | 843/843 | **843/843** |
| imports | 207 | **207** |
| lint / drift | CLEAN / 0 | **CLEAN / 0** |
| section sizes/addresses | all 27 match | **all 27 match** |

## 0. Summary

Four new results this pass:

1. The original's body layout is exactly the **MBB (block-creation) order**,
   which for this function is the source order.  `MBP_FORCE_FALLBACK=1`,
   `MBP_SORT_SBCB=1` and `MBP_SKIP_JT_PUSH=1` in the project's custom
   `llvm7build` each reproduce the original body order verbatim.
2. The exact original order **is** reachable with `!prof` weights
   `default 1, 167/174 200, A 200, B 199, low 201` — note `low > A`.  This
   yields the exact order at 1436 B / **76.0 %**.  The wave-124 weights
   (`low 150 < A 200`) misplace the low group; the wave-124 conclusion
   "`wl > wa` has no solution" was true only at 1432 B, not at 1436 B.
3. The original has **no `uxth`** in case 168: `cmp r5, r0; blt.n`.  The
   current `(unsigned short)` cast is *not* a size device — the function is
   1432 B with or without it (the cast is score-neutral, 71.0 % either way).
4. All plain-C++ shapes tried (two-switch splits / nested switches /
   hole-fills) either rotate the register allocation (`this`=r11 instead of
   r6) or shrink the function; none beats the 71.0 % baseline.  The one
   landed source change is the case-168 cast removal below.

## 1. Baseline evidence (measured this pass)

```sh
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6
# verify: byte 4506  linked 4506  stubs 0  missing 0  extra 0  avg 99.93
# data:   exact 843 of 843 exported objects
# sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
# RATCHET PASS
```

`cmake-build-match/verify/report.json`:
`_ZN6Status16missionCompletedEbbx  pct 71.0  osize 1432  usize 1432`.

## 2. Original body order (decoded from both TBH tables)

`decode2.py` on the original `.so`:

```
high TBH 0xabfe2 (40 entries, base 0xabfe6), values 150..189:
  150 ac110  151 ac116  152 ac11c  153 ac150  154 ac156  155 ac16e
  156 ac32a  157 ac354  158 ac180  160 ac388  162 ac1dc  163 ac3b6
  164 ac3d2  165 ac3d6  166 ac242  167 ac036  168 ac3de  169 ac3fc
  170 ac2a6  171 ac400  172 ac41a  174 ac036  184 ac2b4  189 ac2ee
  (159,161,173,175..183,185..188 -> default ac45a)
low TBH 0xac04e (7 entries, base 0xac052), values 8..14:
  8 ac060  9 ac45a  10 ac45a  11 ac0c4  12 ac45a  13 ac0ce  14 ac0dc
```

Sorted by body address:

```
167/174 (ac036), 8 (ac060), 11 (ac0c4), 13 (ac0ce), 14 (ac0dc),
150 151 152 153 154 155 158 162 166 170 184 189   [A run, ac110..ac2ee]
156 157 160 163 164 165 168 169 171 172           [B run, ac32a..ac41a]
default (ac45a)
```

This is exactly the order the `case` blocks are written in
`src/game/mission/Status.cpp`.  Our base emits **value order** for the high
table: `150 151 152 153 154 155 156 157 158 160 162 ... 172 184 189`, i.e.
only the A/B interleaving differs — the low group is already in the right
place in the base (the wave-125 assignment's "base interleaves low bodies"
is stale).

## 3. The original layout is the MBB order

The project's custom LLVM 7.0.0 tree (`/home/debian/llvm7build`) has debug
hooks in `MachineBlockPlacement.cpp`.  Building the *whole* `Status.cpp` TU
to IR with the project clang, then re-compiling it with the custom `llc`
(which reproduces clang's codegen byte-for-byte once
`-relocation-model=pic` is passed):

| variant | size | body order | score |
|---|---:|---|---:|
| vanilla MBP (base) | 1432 | value order (A/B interleaved) | **71.0** |
| `MBP_FORCE_FALLBACK=1` | 1440 | **exact source order** | 63.2 |
| `MBP_SORT_SBCB=1` | 1436 | **exact source order** | 76.0 |
| `MBP_SKIP_JT_PUSH=1` | 1440 | **exact source order** | — |

`MBP_FORCE_FALLBACK` forces `getFirstUnplacedBlock()` (walks MBB order) and
`MBP_SORT_SBCB` sorts the worklist by MBB number; both give the source
order.  But `MBP_FORCE_FALLBACK` also **rotates the `for` loop** (condition
moves to the bottom, `b.n` to the latch at the top), which is why it is
1440 B / 63.2 %.  So the original is *not* "MBP disabled" and not the raw
DAG order.

## 4. `!prof` weights: exact order at 1436 B / 76.0 %

The switch is `switch i32 %52` in `cur.ll`; case order is
`167,174,8,0,11,13,14,150,151,152,153,154,155,158,162,166,170,184,189,
156,157,160,163,164,165,168,169,171,172` (default first).  Attaching

```llvm
], !prof !999999
!999999 = !{!"branch_weights", i32 1, i32 200, i32 200, i32 201, i32 201,
  i32 201, i32 201, i32 201, i32 200, ... i32 199, ...}
```

i.e. `default 1, 167/174 200, A 200, B 199, low (8,0,11,13,14) 201`, gives

```
[167, 8, 11, 13, 14, 150 151 152 153 154 155 158 162 166 170 184 189,
 156 157 160 163 164 165 168 169 171 172, default]
```

— the **exact original order** — at **1436 B / 76.0 %**.

Why `low > A` is required: a case body's `MBFI` block frequency is
`weight / total_switch_weight` (the per-table normalisation cancels the
table's own sum).  So `low_freq = wl/total`, `A_freq = wa/total`,
`B_freq = wb/total`, `167/174 = 2*wa/total`.  With `wl=150 < wa=200` (the
wave-124 vector) the A bodies outrank the low bodies and 11/13/14 are
placed after the B run.  With `wl=201 > wa=200` the low bodies outrank A
and all four low bodies land before the A run.  The residual 24 % is then
only branch-encoding / register-allocation noise (cases 158, 162, 163,
166, 172 and the `uxth`), not block order.

## 5. The original has no `uxth` in case 168

Original `ac3de`:

```
ac3de: ldr r0, [pc, #232]     ; Globals::status
ac3e2: ldr r4, [r0, #0]
ac3e4: ldr r0, [r4, #0]
ac3e6: ldr.w r5, [r0, #376]   ; field_178
ac3ea: mov r0, sl
ac3ec: blx getStatusValue
ac3f0: cmp r5, r0
ac3f2: blt.n ac45a            ; signed, no uxth
```

Removing the `(unsigned short)` cast deletes the `uxth r0, r0` and gives
`cmp r5, r0; blt.n`, matching the original.  **This change is landed**:
`src/game/mission/Status.cpp` case 168 now reads
`if (Globals::status->field_178 >= m->getStatusValue())` and the misleading
"size device" comment is gone.

Measured after the change: function size **1432 B** (unchanged), score
**71.0 %** (unchanged — the 4-byte layout shift cancels the one-instruction
win), gate byte **4506** / linked **4506** / data **843/843** / imports
**207**, lint **CLEAN**, drift **0**, and all 27 ELF section sizes and
addresses still match the original exactly.  So this is a
faithfulness/readability fix, not a score win; the "size device" claim in
the old comment was wrong (the function is 1432 B either way).

## 6. Plain-C++ shapes re-measured this pass

All measured with the custom `llc` (`-relocation-model=pic`, project
mattr/triple) which matches clang's instruction stream exactly for this
function (verified: 475/475 normalised instructions identical).

| variant | size | regalloc | score |
|---|---:|---|---:|
| base (single switch, source order) | 1432 | `this`=r6 `i`=fp `m`=sl | **71.0** |
| full A hole-fill + low holes -> 8 | 1380 | `this`=r11 `i`=r10 | 45.9 |
| full A hole-fill + low holes -> 11/13/14 | 1380 | `this`=r11 `i`=r10 | — |
| two sequential switches (high then low) | 1444 | — | — |
| two sequential switches (low then high) | 1448 | — | — |
| nested: outer 167+low, default A+B | 1432 | `this`=r10 | 23.5 |
| nested: outer 167, default low+A+B | 1432 | `this`=r10 | 26.3 |
| nested: outer A, default 167+low+B | 1400 | `this`=r11 | — |
| nested: outer 167+A, default low+B | 1404 | `this`=r11 | — |
| nested: outer 167+low+A, default B | 1392 | `this`=r11 | — |
| nested: outer 167+low+B, default A | 1436 | `this`=r10 | — |
| nested: outer 167+A+B, default low | 1376 | `this`=r11 | — |
| nested: outer low, default 167+A+B | 1420 | `this`=r10 | — |
| nested: outer A+B, default 167+low | 1396 | `this`=r11 | — |

Nested switches are merged by `SimplifyCFG`
(`FoldValueComparisonIntoPredecessors`), which changes the successor order
and/or the MBB order; the register allocation rotates (`this`=r10/r11
instead of r6) and the score collapses.  Hole-filling (adding the unused
values 159/161/173/175..183/185..188 as extra `case` labels on the A
bodies) *does* reproduce the exact body order, but the extra labels change
the default probability enough to rotate the register allocation and shrink
the function to 1380 B.

## 7. Root cause

`SelectionDAGBuilder::buildJumpTable()` adds the jump-table successors in
**ascending case-value order** ("use table order for determinism"), and
`MachineBlockPlacement::selectBestCandidateBlock()` drains the worklist by
`MBFI` block frequency, keeping the first on ties.  With uniform weights the
bodies drain in value order.  The original's source-order layout is what you
get when the worklist is drained in MBB order (fallback / sorted worklist)
or when the switch carries `!prof` weights that make `low > A > B`.  The
original's dispatch, prologue, register allocation and shared tails are
byte-identical to ours, so the shipped `.so` was almost certainly built
with **profile-guided optimisation**; the profile is not recoverable from
source and no C++11 construct emits multi-case switch weights
(`__builtin_expect` sets exactly one weight to 2000).

## 8. Reproduction

```sh
# baseline
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6    # RATCHET PASS, byte 4506

# decode the original tables
python3 /dev/shm/w7508/decode2.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so

# the 76.0 % proof (scratch, /dev/shm/w7508):
#   cur.ll   = clang++ -S -emit-llvm src/game/mission/Status.cpp
#   prof201.ll = cur.ll with "], !prof !999999" on `switch i32 %52` and
#     !999999 = !{!"branch_weights", i32 1, i32 200, i32 200, i32 201,
#       i32 201, i32 201, i32 201, i32 201, i32 200, ... i32 199, ...}
#   llc -mtriple=thumbv7-none-linux-androideabi -mcpu=generic \
#       -mattr=+dsp,+neon,+thumb-mode,+vfp3,-crypto,-d16,-fp-armv8,-fp-only-sp,-fp16,-vfp4 \
#       -float-abi=soft -relocation-model=pic -filetype=obj prof201.ll
#   (swap the resulting .obj into cmake-build-match/.../Status.cpp.obj,
#    run the gold link command from build.ninja, then verify.py)
```

Scratch (not committed): `/dev/shm/w7508/` — `fastvar.py` (clang -> IR ->
custom-llc -> size/order/regalloc harness), `prof201.ll`, `cur2.ll`,
`decode2.py`, `sorder.py`, `fv_*.cpp` variant sources.
