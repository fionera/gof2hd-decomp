# `Status::missionCompleted` — wave 108: table-free exact-layout cost is 36–40 B (w5601)

Status: **research note only** (no source change landed; the `.fini_array` rank
table is still in `src/game/mission/Status.cpp`). Worktree `wt/w5601`, branch
`wave108-w5601`, base master `8f98ec4d6`. Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`). Target:
`_ZN6Status16missionCompletedEbbx`, original `0xabf38`, size **1432 (0x598)**.

## 0. Result

The task was: remove the `.fini_array` rank table (`kMissionCase[192]`, 8 -> 200 B
of `.fini_array`) while keeping the original body order, 1432 B and 27 sections.
No table-free source shape found this pass is **both** size-exact (1432 B) and
body-order-exact. The best table-free exact-order shape is **1472 B / 76.4 %**
(36–40 B over); the best size-exact table-free shape is **1432 B / 60.4 %**
(ascending layout). The rank-table baseline stays at **87.0 % / 1432 B**.

The one genuinely new result vs. the wave-90 note
(`status-missioncompleted-mbp-fallback-w4003.md`) is that the **sequential
two-switch form can be written so that the original's register allocation is
reproduced exactly**: keep the group-B bodies in the *first* switch under
`l156:`-style labels, and make the *second* switch re-dispatch with a fresh
`m->getType()` call (`Status.AF.cpp`). Its prologue is byte-identical to the
original's and its body code differs from the original by only a handful of
instructions; the whole 40-byte excess is the second dispatch table.

## 1. Gate (baseline, unchanged)

```
cmake --build cmake-build-match --target check -j6
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.91
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓
drift: 0 field(s) across 0 class(es)
```

`report.json` row: `pct 87.0`, `osize 1432`, `usize 1432`, `byte false`.
No source change was landed, so `tools/verify_baseline.json` is untouched and
`git status` is clean apart from this note.

## 2. Original layout (re-confirmed from the ELF)

```
abfda: sub.w r1, r0, #150 ; cmp r1, #39 ; bhi.n ac046 ; tbh (40 halfwords, 150..189)
ac046: sub.w r1, r0, #8   ; cmp r1, #6  ; bhi.n ac0be ; tbh (7 halfwords, 8..14)
ac0be: cmp r0, #0         ; bne latch   ; (case 0 shares the 0/11 body at ac0c4)
```

Decoded body order (by target address):

```
167/174, 8, 0/11, 13, 14, 150, 151, 152, 153, 154, 155, 158, 162, 166,
170, 184, 189, 156, 157, 160, 163, 164, 165, 168, 169, 171, 172, latch
```

i.e. **group A** `{150..155,158,162,166,170,184,189}` ascending, then
**group B** `{156,157,160,163,164,165,168,169,171,172}` ascending. Only
`getType()` is called once (`0xabfd6`); no `sxth`/`uxth`; the group-B bodies
have exactly one predecessor each (only the TBH entries point at them — verified
by grepping every group-B body address in the disassembly).

## 3. Measured variants (full link + official verifier)

All rows are `Status.cpp`-only edits. `usize` is the function size; `pct` is
`report.json`'s fuzzy score.

| variant | pct | usize | layout | notes |
|---|---:|---:|---|---|
| HEAD rank table in `.fini_array` | **87.0** | **1432** | exact | baseline; `.fini_array` 8 -> 200 B |
| direct `int` switch, source order (A2/Lbin) | 60.4 | 1432 | ascending, 184/189 last | size-exact, table-free |
| nested `switch (t)` in `default:` (NEST3) | 60.4 | 1432 | ascending | SimplifyCFG merges to one 40-entry TBH |
| nested `switch (t - 156)` (NEST4) | 60.4 | 1432 | ascending | still merged |
| nested `switch (t + 1000)` (NEST5) | 60.4 | 1432 | ascending | still merged |
| `int t` + two sequential switches (AF7) | 60.4 | 1432 | ascending | second switch optimised away |
| **AF**: first switch = all cases (group B bodies labelled) + `goto after189;` + second `switch (m->getType()) { case 156: goto l156; ... }` | **76.4** | **1472** | **exact** | prologue byte-identical to original |
| AF2 = AF without the case-166 anchor | 75.3 | 1468 | exact | |
| AF5 = AF with `m->id` in the second switch | 75.3 | 1468 | exact | |
| SEQ2/SEQ3 = `switch (m->getType()){A} switch (m->getType()){B}` | — | 1460 | A then B, 184/189 peeled to compares | 3 tables, not merged |
| SEQ4 = `int type` + two sequential switches | — | 1512 | garbled | two 40-entry TBHs, not merged |
| AH3 (184/189 as `if`s + group-B goto switch) | 35.0 | 1448 | A then B, 184/189 misplaced | |
| AH5 (`int t` + 184/189 `if`s + goto switch) | — | 1476 | garbled | |
| AF8 (group-B `goto after189` + second switch with bodies) | — | 1492 | garbled | |

`AF` source shape (`/dev/shm/w5601/Status.AF.cpp`):

```cpp
switch (m->getType()) {
    case 167: case 174: ...          // the whole original body order,
    ...                              // group-B bodies carry `l156:`-style labels
    case 189: ...; goto after189;
}
goto latch;
after189:
switch (m->getType()) {              // a *fresh* getType() call, not a saved int
    case 156: goto l156;
    ...
    case 172: goto l172;
    default: break;
}
latch:;
```

## 4. Why AF reproduces the original register allocation (new)

The wave-90 note measured the natural `int type` two-switch form at 1508 B and
blamed the 76-byte excess on `type` staying live across the first switch
(`type` takes `fp`, pushing `i`->`r8`, `atStation`->`r9`, and spilling `this`).
That analysis is correct, but it is avoidable: **re-call `getType()` in the
second switch** instead of keeping an `int`. AF's prologue is then byte-for-byte
the original's:

```
push {r4,r5,r6,r7,lr} ; add r7,sp,#12
stmdb sp!,{r3,r4,r5,r6,r7,r8,r9,sl,fp}
ldrd r3,r5,[r7,#8] ; mov r6,r0 ; mov.w fp,#0 ; mov r8,r1
```

and the instruction-stream diff against the original is 529 vs 545 instructions
(16 extra), essentially all inside the second dispatch. Every case body matches
the original's instruction sequence (only a few `mov r9,r0` / `mov r5,r0`
scratch-register choices and one `cmp r0,#0` differ).

## 5. Where the 40 bytes go

| region | original | AF |
|---|---:|---:|
| prologue + loop header + both dispatches + tables | 254 | 287 |
| 27 bodies + latch + shared tails | 1178 | 1185 |
| **total** | **1432** | **1472** |

The second dispatch is exactly:

```
mov r0, sl ; bl getType        (6)
subs r0, #156 ; cmp r0, #16    (4)
bhi.w latch                    (4)
tbb [pc, r0]                   (2)
17-byte offset table           (18 with padding)
```

= 34 B, plus the `goto after189` (2) and `goto latch` (2). ARMConstantIslands
does **not** fold the second TBB into the first 40-entry TBH (the first table
already contains the group-B bodies, so the second table's targets are bodies,
not a new island), which is why the cost is not free.

## 6. Proven vs hypothesis

**Proven (commands in §7):**

1. The original body order is group A ascending, then group B ascending, with
   `167/174` first (they share one body, so `JTProbs` doubles and
   `selectBestSuccessor` peels them) and the latch last.
2. A direct `int` switch always lays the bodies out in ascending type order
   (worklist order == jump-table successor order); reordering the case bodies in
   the source changes nothing. Nested switches on `t`, `t-156` and `t+1000` are
   all merged by SimplifyCFG into one 40-entry TBH with the same ascending
   layout.
3. The AF shape (group-B bodies in the first switch with labels + a second
   `switch (m->getType())` re-dispatch after `189`) reproduces the original
   body order **exactly** and reproduces the original's prologue/register
   allocation byte-for-byte, at 1472 B / 76.4 %.
4. The entire 40-byte excess is the second dispatch table (34 B) plus two
   `goto`s; the bodies are within ~7 B of the original.
5. Every size-exact table-free shape measured tops out at 60.4 % (ascending
   layout). Every exact-layout shape measured is >= 1448 B.

**Hypothesis / open:**

* The original source is the AF shape (or the natural `int type` two-switch
  form) and the original compiler found a way to make the group-B bodies'
  second predecessor **free** — e.g. the second dispatch was lowered into the
  first switch's own table, or `ARMConstantIslands` folded the two tables
  because the second switch's cases were the bodies rather than `goto` labels.
  Neither reproduced here at 1432 B.
* A source-level way to make `computeUnlikelySuccessors` /
  `calcLoopBranchHeuristics` demote the group-B case edges (so MBP places them
  after group A with a single table) was not found: the switch terminator is not
  a `BranchInst`, so `computeUnlikelySuccessors` returns immediately, and every
  in-loop successor gets the same `LBH_TAKEN_WEIGHT/numInEdges` probability.
* `__builtin_expect` is allowed (not `__attribute__`) and does change case
  probabilities, but every expect form measured either peeled a case first or
  produced an if/else layout, never the group-A-then-group-B order at 1432 B.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5601
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6     # RATCHET PASS
cmake --build cmake-build-match --target lint  -j6     # CLEAN
cmake --build cmake-build-match --target drift -j6     # 0 fields

OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -d --start-address=0xabf38 --stop-address=0xac4d0 \
    /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so

# per-variant: compile Status.cpp alone, then decode the TBH/TBB layout
/dev/shm/w5601/cc.sh                       # -> /dev/shm/w5601/Status.o
python3 /dev/shm/w5601/dec2.py .text._ZN6Status16missionCompletedEbbx /dev/shm/w5601/Status.o
# full score (rebuild + link + verify):
/dev/shm/w5601/measure.sh /dev/shm/w5601/Status.AF.cpp
python3 /dev/shm/w5601/row.py
```

Scratch (not committed): `/dev/shm/w5601/` — `Status.orig.cpp` (rank-table
baseline), `Status.A2.cpp` (direct switch, source order), `Status.AF.cpp`
(exact order, 1472 B), `Status.AF2.cpp`, `Status.AF5.cpp`, `Status.AF7.cpp`,
`Status.AF8.cpp`, `Status.NEST3/4/5.cpp`, `Status.SEQ2/3/4.cpp`,
`Status.AH3/AH5.cpp`, `dec2.py` (TBH/TBB layout decoder), `try.sh`
(compile+size+layout), `measure.sh` (full verify), `row.py`.
