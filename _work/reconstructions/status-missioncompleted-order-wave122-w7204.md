# `Status::missionCompleted` — wave 122 (w7204): plain-C++ A-then-B body order

Status: **note-only** (no source change landed).  Worktree `wt/w7204`, base
master `29cd275f6`.  Original binary:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b clang
7.0.2 r328903, `-Oz`, image base 0x10000).

Target: `_ZN6Status16missionCompletedEbbx` (`Status::missionCompleted(bool,
bool, long long)`), original `0xabf38`, size **1432 B**.

| | baseline (HEAD) | two-switch proposal |
|---|---:|---:|
| function score | **59.4 %** | **52.6 %** |
| function size | 1432 B | **1512 B** |
| body order | value order | **original A-then-B** |
| gate `byte_exact` if landed | 4506 | **2327** (size shift) |
| gate `linked_exact` if landed | 4506 | 4506 |

The proposal is semantically equivalent and reproduces the original body order
exactly, but it is **not gate-green**: the +80 B size shift moves every
following `.text` symbol and drops `byte_exact` from 4506 to 2327.  It is
therefore recorded as a concrete patch proposal, not landed source.

## 1. Original body order and dispatch (binary evidence)

`Status::missionCompleted` calls `Mission::getType()` exactly once
(`0xabfd6`) and dispatches with two in-text TBH tables:

```
abfda: sub.w r1, r0, #150
abfde: cmp   r1, #39
abfe0: bhi.n ac046
abfe2: tbh   [pc, r1, lsl #1]      ; 40 halfwords @ 0xabfe6, types 150..189
...
ac046: sub.w r1, r0, #8
ac04a: cmp   r1, #6
ac04c: bhi.n ac0be
ac04e: tbh   [pc, r1, lsl #1]      ; 7 halfwords @ 0xac052, types 8..14
...
ac0be: cmp   r0, #0
ac0c0: bne.w ac45a                 ; loop latch
```

Decoding the tables (`target = table_base + 2*entry`, table bases `0xabfe6`
and `0xac052`) and sorting the targets by address gives the original body
layout:

```
167/174  8  0/11  13  14
150 151 152 153 154 155 158 162 166 170 184 189      ; run A
156 157 160 163 164 165 168 169 171 172              ; run B
default/latch
```

The tables are **value-indexed** (slot `i` is type `150+i`); only the target
addresses are in source order.  The low group `8, 0/11, 13, 14` is before A;
the shared `167/174` body is first; the loop latch/default is last.

## 2. Why the current plain switch emits value order

The landed source has the same case labels in the original A-then-B source
order, but a plain `switch (m->getType())` emits the same dispatch and the
same body bytes in **ascending type order**:

```
167/174  8  0/11  13  14
150 151 152 153 154 155 156 157 158 160 162 163 164 165 166 168 169 170 171 172
default  184 189
```

Mechanism (clang 7 / LLVM `1d739ffb`):

* `SelectionDAGBuilder::sortAndRangeify()` sorts switch clusters by case
  value.
* `buildJumpTable()` adds the jump-table MBB's successors in `Table` order,
  i.e. ascending value.
* `MachineBlockPlacement`'s `markBlockSuccessors()` pushes those successors
  into `BlockWorkList` in that order.
* Every case body has the same block frequency (only `167/174` is 2/40 and
  the default is 1/40), so `selectBestCandidateBlock()` keeps the first
  maximum-frequency block and drains the worklist in ascending type order.

The original layout is the `MachineBlockPlacement::getFirstUnplacedBlock()`
fallback (MBB order = source order).  Our build never reaches that fallback
because the high jump table's worklist is non-empty (23 entries) at the
moment the table is placed.

## 3. Concrete patch proposal: two sequential switches

The smallest semantically equivalent plain-C++ change that makes clang 7
emit A-then-B is to split the single switch into two sequential switches on
a cached `int`:

```cpp
        int mtype = m->getType();
        switch (mtype) {
            case 167:
            case 174:
                ...
            case 8:
                ...
            case 0:
            case 11:
                ...
            case 13:
                ...
            case 14:
                ...
            case 150:
                ...
            case 151:
                ...
            case 152:
                ...
            case 153:
                ...
            case 154:
                ...
            case 155:
                ...
            case 158:
                ...
            case 162:
                ...
            case 166:
                ...
            case 170:
                ...
            case 184:
                ...
            case 189:
                ...
                break;
        }
        switch (mtype) {
            case 156:
                ...
            case 157:
                ...
            case 160:
                ...
            case 163:
                ...
            case 164:
                ...
            case 165:
                ...
            case 168:
                ...
            case 169:
                ...
            case 171:
                ...
            case 172:
                ...
                break;
        }
```

The split point is **after `case 189`'s closing `break;` and before
`case 156:`**.  The first switch keeps the shared `167/174` body, the low
group, and all of run A; the second switch contains exactly run B.

Semantics are unchanged: for an A type the second switch matches no case and
does nothing; for a B type the first switch matches no case and does nothing,
then the second switch executes the B body.

Why this produces A-then-B: the second switch is a second in-loop
predecessor for every B body.  When the high TBH is placed,
`markBlockSuccessors()` decrements each B body's `UnscheduledPredecessors`
but does not push it (the second dispatch is still unplaced).  The B bodies
are pushed only when the second dispatch is placed, after the first switch's
bodies.  `ARMConstantIslands` then merges the second switch's cases into the
same 40-entry TBH, so the dispatch shape remains the original two TBHs
(`sub #150; cmp #39; tbh` and `sub #8; cmp #6; tbh`); the second dispatch
block itself survives and is what delays the B bodies.

## 4. Measured result

Compiled with the project's exact flags (`-Oz`, clang 7.0.2 r328903) and
linked with the normal target:

```
usize = 1512 B
score = 52.6 %
high-table body order:
  167/174 150 151 152 153 154 155 158 162 166 170 184 189
          156 157 160 163 164 165 168 169 171 172 default
low-table body order:
  8 0/11 13 14 default
```

i.e. **exactly the original body order**, including the low group before A
and the latch/default after B.  The high TBH is still 40 entries and the low
TBH still 7 entries; the table halfwords differ only because the body
addresses now match the original order.

If landed, `byte_exact` drops 4506 -> 2327 because the function is 80 B
larger and every following `.text` symbol shifts; `linked_exact` stays 4506.
The 80 B is body-code growth, not dispatch growth: the dispatch/prologue is
the same 254 B, but `mtype` stays live in `r11` across the first switch, so
`i` moves from `fp` to `r8`, `atStation` from `r8` to `r9`, `this` spills to
`[sp,#16]`, and the bodies grow by 76 B.  This is the same register-pressure
cost measured in the earlier two-switch notes.

## 5. Variants tried

| variant | usize | pct | body order | note |
|---|---:|---:|---|---|
| **two sequential switches (`int mtype`)** | **1512** | **52.6** | **exact A-then-B** | semantically equivalent; proposal above |
| two sequential switches, cached body-166 index | 1508 | ~53.2 | exact A-then-B | body 166 not faithful (original re-queries) |
| A-group fallthrough (drop `break` in A except 189) | 1388 | 49.7 | exact A-then-B | semantically wrong: failure path falls into next A body |
| duplicate A case labels (159,161,173,175..183 -> A bodies) | 1376 | 45.8 | A-then-B-ish | low group moves after A; table entries change; -56 B |
| `goto` B bodies after the switch | 1520 | — | mixed | low group/167 misplaced |
| two switches, second on `m->getType()` re-call | 1456 | — | wrong table | compiler merges differently; B bodies collapse to default |
| two switches, `short`/`unsigned`/`const`/`int32_t` | 1512 | — | exact A-then-B | `unsigned short`/`unsigned char`: 1524 |
| rank table (removed in wave 121) | 1432 | 92.45 | exact A-then-B | +192 B `.rodata`, not the target of this note |

The A-group fallthrough is the smallest variant that reaches the exact body
order, but it changes semantics (an A body whose condition fails executes the
next A body), so the two-switch shape is the only semantically faithful
plain-C++ proposal found.

## 6. Conclusion

The two-switch patch is a concrete, semantically equivalent plain-C++ way to
make clang 7 emit the original A-then-B body order without the removed rank
table.  It reproduces the body order exactly and keeps the original two-TBH
dispatch shape, but costs +80 B (1512 B) and is not gate-green.  The remaining
root cause is the `MachineBlockPlacement` worklist order: the high TBH's
successors are ascending by type, and the original layout is the MBB/source
order fallback.  The second switch forces the B bodies to wait for a second
predecessor, which is the only plain-source mechanism found to reproduce the
original order.

## 7. Reproduction

```sh
# baseline
cmake --build cmake-build-match --target check -j6
# verify row: 59.4 %, osize 1432, usize 1432, byte 4506 linked 4506

# apply the two-switch patch to src/game/mission/Status.cpp, then:
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target verify -j6
# verify row: 52.6 %, osize 1432, usize 1512, byte 2327 linked 4506

# TBH body-order decode (object or linked .so):
#   high table base = tbh_addr + 4, 40 entries, target = base + 2*entry
#   low  table base = tbh_addr + 4,  7 entries, target = base + 2*entry
#   sort unique targets by address; the resulting groups are the body order.
```

Scratch (not committed): `/dev/shm/w7204/` — `Status_two.cpp` (two-switch
proposal), `Status_fall.cpp` (A-group fallthrough), `Status_goto.cpp`,
`Status_dupA.cpp`, `parse.py`, `search3.py`.
