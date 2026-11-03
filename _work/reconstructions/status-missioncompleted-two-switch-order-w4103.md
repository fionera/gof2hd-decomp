# `Status::missionCompleted` — the two-switch body order, and why it costs 76 B (w4103)

Status: research note (no source change landed). Worktree `wt/w4103`, branch
`wave91-w4103`, base master `a4557300`. Original binary:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b clang
7.0.2 r328903 / LLVM `1d739ffb`, `-Oz`, image base 0x10000; real VMA =
Ghidra − 0x10000).

Target: `_ZN6Status16missionCompletedEbbx` (`Status::missionCompleted(bool,
bool, long long)`), original `0xabf38`, size **1432 (0x598)**, currently
**57.7 % / 1432 B** with the committed `switch ((short) m->getType())`.

This pass establishes, with a fresh instrumented `MachineBlockPlacement`
trace and a new source-shape sweep:

1. The original layout is **MBB order = source order**, and the current
   build's layout is the **jump-table successor order = ascending case
   value**. The mechanism is now pinned to the exact `markBlockSuccessors`
   push list (23 entries, ascending).
2. A **two sequential `switch` statements on the same cached `int`** is the
   only source shape found that reproduces the original's body order
   *exactly* — and it does so with the original's single merged 40-entry TBH
   plus 7-entry TBH. But it costs **+76 B (0x5e4 / 1508 B)** because the
   cached type stays live in `r11` across the first switch.
3. The forced `getFirstUnplacedBlock` fallback still reproduces the body
   order exactly, but in the local LLVM 7.0.0 build it canonicalises the
   shared `cmp r4,r0` tail differently (body 150 inlines it, tail lands after
   151). The **NDK** build already places that tail after 155 exactly like
   the original, so the only remaining blocker is triggering the fallback in
   the NDK compiler.

No landable change: the two-switch form is 76 B too large, and the plain-int
single switch is 4 B too small (its 1428 B shifts `.text` and drops
`byte_exact` 4498 → 3043). See §7 for the open question.

## 0. Baseline (measured in this worktree)

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
```

```
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`cmake-build-match/verify/report.json` row:
`{'name': '_ZN6Status16missionCompletedEbbx', 'pct': 57.7, 'byte': False,
'linked': False, 'osize': 1432, 'usize': 1432}`.
(The repo-root `report.json` is stale at 86.7 — use the build-dir copy.)

## 1. Ground truth recap (re-verified this pass)

`Status::missionCompleted` at `0xabf38`, size 1432. One `Mission::getType()`
call (`0xabfd6`), **no `sxth`**, exactly **two `tbh`** in the whole function:

```
abfda: sub.w r1, r0, #150
abfde: cmp   r1, #39
abfe0: bhi.n ac046
abfe2: tbh   [pc, r1, lsl #1]      ; 40 halfwords @ abfe6, types 150..189
...
ac046: sub.w r1, r0, #8
ac04a: cmp   r1, #6
ac04c: bhi.n ac0be
ac04e: tbh   [pc, r1, lsl #1]      ; 7 halfwords @ ac052, types 8..14
...
ac0be: cmp   r0, #0
ac0c0: bne.w ac45a                 ; latch
```

There is **no second dispatch** anywhere after the bodies: every body's
`break` goes to the loop latch `ac45a` (`add.w fp,fp,#1; b.n abf82`). So the
source is a single `switch` on an `int`.

The plain-int build's prologue is byte-identical to the original
(`fp=0` zero register, `r8=atStation`, `r6=this`, `str r6,[sp,#16]`, `i`
lives in `fp` inside the loop). The committed `(short)` cast is a size anchor
that adds an `sxth` the binary does not have.

### 1.1 Original body map (sorted by address)

```
ac036 167/174   ac110 150  ac116 151  ac11c 152  ac150 153  ac156 154
ac16e 155       ac172 shared tail (cmp r4,r0)          ac180 158  ac1dc 162
ac242 166       ac2a6 170  ac2b4 184  ac2ee 189       ac32a 156  ac354 157
ac388 160       ac3b6 163  ac3d2 164  ac3d6 165       ac3de 168  ac3fc 169
ac400 171       ac41a 172  ac45a latch/default
```

i.e. exactly the `case` order in `src/game/mission/Status.cpp`:

```
167/174  8  0/11  13  14  150 151 152 153 154 155 158 162 166 170 184 189
156 157 160 163 164 165 168 169 171 172  default
```

Two ascending runs: `{150,151,152,153,154,155,158,162,166,170,184,189}` then
`{156,157,160,163,164,165,168,169,171,172}`. The low group is `8, 0, 11, 13,
14` (also source order — case 8 is written before case 0).

## 2. The current build's layout is the JT successor order

`MachineBlockPlacement` places a jump-table's bodies from `BlockWorkList`,
which `markBlockSuccessors` fills from `MBB->successors()`. For a jump table
that list is `buildJumpTable`'s `Table` order — **ascending case value**
(`SelectionDAGBuilder.cpp`: `// Add successors. Note: use table order for
determinism.`; `sortAndRangeify` sorts by `slt`). All bodies have the same
block frequency (`0.1865889213`), so `selectBestCandidateBlock` keeps the
first one and the bodies come out ascending.

Instrumented LLVM 7.0.0 trace of the plain-int IR (`Status_int.cpp`), at the
high jump table:

```
WLPUSH mark %bb.36 via %bb.104   ; 150
WLPUSH mark %bb.38 via %bb.104   ; 151
WLPUSH mark %bb.39 via %bb.104   ; 152
WLPUSH mark %bb.44 via %bb.104   ; 153
WLPUSH mark %bb.45 via %bb.104   ; 154
WLPUSH mark %bb.46 via %bb.104   ; 155
WLPUSH mark %bb.77 via %bb.104   ; 156
WLPUSH mark %bb.80 via %bb.104   ; 157
WLPUSH mark %bb.47 via %bb.104   ; 158
WLPUSH mark %bb.85 via %bb.104   ; 160
WLPUSH mark %bb.57 via %bb.104   ; 162
WLPUSH mark %bb.88 via %bb.104   ; 163
WLPUSH mark %bb.91 via %bb.104   ; 164
WLPUSH mark %bb.92 via %bb.104   ; 165
WLPUSH mark %bb.62 via %bb.104   ; 166
WLPUSH mark %bb.16 via %bb.104   ; 167/174
WLPUSH mark %bb.93 via %bb.104   ; 168
WLPUSH mark %bb.96 via %bb.104   ; 169
WLPUSH mark %bb.66 via %bb.104   ; 170
WLPUSH mark %bb.97 via %bb.104   ; 171
WLPUSH mark %bb.98 via %bb.104   ; 172
WLPUSH mark %bb.67 via %bb.104   ; 184
WLPUSH mark %bb.71 via %bb.104   ; 189
...
SBCB size=23 [%bb.11 %bb.36 %bb.38 %bb.39 %bb.44 %bb.45 %bb.46 %bb.77
              %bb.80 %bb.47 %bb.85 %bb.57 %bb.88 %bb.91 %bb.92 %bb.62
              %bb.93 %bb.96 %bb.66 %bb.97 %bb.98 %bb.67 %bb.71]
```

`%bb.11` is the low jump table (frequency 1.0226, picked first); the 22 high
bodies are pushed in ascending value order. The original order needs the
worklist in **MBB order** instead, which is only produced by
`getFirstUnplacedBlock` (called when `selectBestCandidateBlock` returns
nullptr, i.e. the worklist is empty).

## 3. New finding: two sequential switches reproduce the order exactly

The only source shape found that reproduces the original's body order is two
sequential `switch` statements on the same cached `int`:

```cpp
int mtype = m->getType();
switch (mtype) {                      // group A
    case 167: case 174: ... case 189:
}
switch (mtype) {                      // group B
    case 156: ... case 172:
}
```

Compiled with the project's exact flags (`/dev/shm/w4103/Status_seq.cpp`), it
emits **one merged 40-entry TBH + the 7-entry TBH** (the same two tables as
the original) plus a 17-entry TBB for the second switch. Decoding the 40-entry
table (`sorder.py`) gives the body order by position:

```
167/174 150 151 152 153 154 155 158 162 166 170 184 189
156 157 160 163 164 165 168 169 171 172  default
```

— **the original order, exactly**. (The list printed as table indices is
`[17,24,0,1,2,3,4,5,8,12,16,20,34,39,6,7,10,13,14,15,18,19,21,22,...]`.)

Mechanism: `SimplifyCFG`/`CodeGenPrepare` folds the second switch's cases
into the first switch's table (the 40-entry table's holes for 156..172 point
directly at the group-B bodies), but the second switch's dispatch block
survives because it is reachable from the group-A bodies' `break` edges. So
the group-B bodies have **two** predecessors (the high jump table and the
second dispatch). When the high jump table is placed, `markBlockSuccessors`
decrements them to 1 and does *not* push them; they are pushed only when the
second dispatch is placed, after group A. That is exactly the two-run order.

Cost: **0x5e4 (1508 B)**, +76 B vs the original, because `mtype` stays live
in `r11` across the first switch. Register allocation becomes:

| | original / plain-int | two-switch |
|---|---|---|
| `i` | `fp` | `r8` |
| `atStation` | `r8` | `r9` |
| `m` | `sl` | `r10` |
| `this` | `r6` (+ `[sp,#16]`) | `r6` (+ `[sp,#16]`) |
| cached type | — | **`r11`** |
| zero register | `fp` | `r8` |

The extra live value shifts `i` out of `fp` and `atStation` out of `r8`, adds
the second dispatch block (~5 instructions + 17-byte TBB), and widens a
number of branches; instruction count 817 → 835 (+18).

Measured with the official verifier (full `link_so` + `verify` on the real
tree, then reverted): the two-switch source scores **53.2 %** (usize 1508),
i.e. *worse* than the committed 57.7 %, and the 76-byte shift drops
`byte_exact` 4498 → 2326. So the right body order alone is not enough — the
plain-int register allocation is required as well.

## 4. Variant sweep (compile-only; `sorder.py` decodes the TBH targets)

`/dev/shm/w4103/try.sh <file>.cpp` compiles with the project flags, emits
`.s`, and prints the 40-entry-table body order plus the object size.

| source form | size | high-table body order |
|---|---:|---|
| `switch ((short) m->getType())` (committed) | 0x598 (1432) | ascending |
| `switch (m->getType())` (plain int) | 0x594 (1428) | ascending |
| `switch (m->getType() & 0x7fffffff)` | 0x598 (1432) | ascending |
| nested `default: switch (mtype)` | 0x594 (1428) | ascending |
| **two sequential switches on cached `int`** | **0x5e4 (1508)** | **original** |
| two switches, `unsigned` / `short` / `const int` / `int32_t` / copy / reference / `+0` / `\|0` / `int[2]` / struct member | 0x5e4 | original |
| two switches, second on `(short) mtype` | 0x60c | — (different table) |
| two switches, second on `*(volatile int*)&mtype` / `volatile int mtype` | 0x604 | — |
| two switches, second on `m->getType()` (re-call) | 0x5bc | 25-entry table, wrong order |
| two switches, second on `m->id` | 0x5bc | 25-entry table, wrong order |
| both switches on `m->getType()` | 0x5bc | 25-entry table |
| first on `m->getType()`, second on cached | 0x5b4 | 25-entry table |
| two switches, second guarded by `if (mtype >= 156)` | 0x600 | — |
| two switches in `do { } while (0)` / `if (1)` | 0x5e4 | original |
| `-mllvm -tail-dup-placement` (plain int) | 0x594 | ascending |
| `-mllvm -force-loop-cold-block` (plain int) | 0x5b0 | `167/174 150..155 156..172 184 189` |
| `-mllvm -block-placement-exit-block-bias=0` (plain int) | 0x594 | ascending |

The re-call / `m->id` forms do **not** merge the two tables (the first table
shrinks to 25 entries covering 150..174), so they lose the original dispatch
shape.

## 5. Forced `getFirstUnplacedBlock` still reproduces the order

Patching `selectBestCandidateBlock` to `return nullptr` in a local LLVM 7.0.0
and compiling the plain-int IR (`sint.ll`) gives, by position:

```
167/174 150 151 152 153 154 155 158 162 166 170 184 189
156 157 160 163 164 165 168 169 171 172  default
```

— the original order again, from the *single-switch* source. So the
fallback is a valid existence proof for the layout, independent of the
two-switch mechanism.

Caveat (unchanged from w4003, but now understood): the local 7.0.0 build's
`BranchFolder` canonicalises the shared `cmp r4,r0` tail differently — body
150 inlines it and the tail block is placed after 151, whereas the original
(and the NDK 7.0.2 build of the plain-int source) has 150 branch to the tail
and the tail placed after 155. The forced local build is 0x588 (1416 B), 16 B
short of the original. The NDK build's tail placement already matches the
original, so this is a local-LLVM artifact, not a source problem.

## 6. Why the fallback is not taken in the NDK build

`getFirstUnplacedBlock` is only reached when `selectBestCandidateBlock`
returns nullptr, i.e. when `BlockWorkList` is empty after erasing entries in
the current chain. The high jump table's 22 body chains are pushed by
`markBlockSuccessors` the moment the jump-table block is placed, because
each body has exactly one in-loop predecessor (the jump table) and its
`UnscheduledPredecessors` therefore drops to 0. The worklist is 23 entries
(22 high bodies + the low jump table) at that point, so the fallback never
runs and the bodies come out in successor (ascending) order.

For the fallback to run, the bodies must **not** be pushed there. In
`markBlockSuccessors` a successor is skipped when
`UnscheduledPredecessors == 0` or when the decrement leaves it `> 0`, i.e.
when the body has a second unplaced in-loop predecessor, or when the body is
not in the loop's `BlockFilter`, or when it is already in the same chain.
The two-switch source achieves the "second predecessor" case (the second
dispatch), but pays for the extra dispatch and the live `mtype`.

## 7. Conclusion — proven vs open

**Proven (this pass):**

1. The original layout is MBB order = source order; the current build's is
   the jump-table successor order (ascending value). The exact 23-entry push
   list is in §2.
2. The two-sequential-switch source reproduces the original body order
   **exactly** with the original's two TBH tables, but is 0x5e4 / 1508 B
   because `mtype` stays live in `r11` across the first switch.
3. Forcing `getFirstUnplacedBlock` reproduces the order from the
   single-switch source; the only local-build difference is the shared-tail
   canonicalisation, which the NDK build already gets right.
4. The plain-int single switch is the faithful form (prologue and dispatch
   byte-identical, no `sxth`) and is 1428 B / 60.2 %; the 4-byte deficit is
   branch-encoding drift from the wrong layout, not a missing statement.

**Open:** a source shape that empties `BlockWorkList` at the high jump table
(or otherwise gives the bodies a second unplaced in-loop predecessor) while
keeping the plain-int register allocation and the NDK's shared-tail
placement. If found, the function should become byte-exact at 1432 B and
`byte_exact` should rise 4498 → 4499. The two-switch form is the closest
known shape but its +76 B register-pressure cost must be removed.

## 8. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w4103
cmake --build cmake-build-match --target check -j8   # RATCHET PASS
cmake --build cmake-build-match --target lint  -j8   # CLEAN
cmake --build cmake-build-match --target drift -j8   # 0 fields

OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -d --start-address=0xabf38 --stop-address=0xac4d0 \
    /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so

# body order + size of a variant (scratch)
/dev/shm/w4103/try.sh /dev/shm/w4103/Status_seq.cpp     # 0x5e4, original order
/dev/shm/w4103/try.sh /dev/shm/w4103/Status_int.cpp     # 0x594, ascending

# forced-fallback experiment (local assertions LLVM 7.0.0)
#   patch selectBestCandidateBlock -> return nullptr
#   make -C /home/debian/llvm7build/build llc -j8
#   llc -mtriple=thumbv7-none-linux-androideabi -mattr=+neon -float-abi=soft \
#       -filetype=asm -o out.s sint.ll
#   python3 sorder.py out.s
```

Scratch (not committed): `/dev/shm/w4103/` — `Status_int.cpp`,
`Status_seq.cpp`, `Status_nested.cpp`, `Status_seq_retype.cpp`,
`Status_id2.cpp`, `Status_idid.cpp`, the `v_*`/`w_*`/`x_*`/`y_*`/`z_*`
variant generators, `cc.sh`, `ccir0.sh`, `try.sh`, `sorder.py`, `layout.py`,
`orig.asm`, `sint.ll`, `sint_trace2.txt` (instrumented MBP trace),
`MBP_orig.cpp`, `SDB_clean.cpp`.
