# `Status::missionCompleted` — the fallback layout is confirmed; the trigger is the open problem (w4201)

Status: research note, **no source change landed** (the gate is unchanged). Worktree
`wt/w4201`, branch `wave92-w4201`, base master `b379d00d`. Original binary:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b clang 7.0.2
`r328903` / LLVM `1d739ffb`, `-Oz`, image base 0x10000; real VMA = Ghidra − 0x10000).

Target: `_ZN6Status16missionCompletedEbbx` (`Status::missionCompleted(bool, bool,
long long)`), original `0xabf38`, size **1432 (0x598)**, currently **57.7 % / 1432 B**
with the committed `switch ((short) m->getType())`.

This pass re-verifies and sharpens the w4003/w4103 conclusions with fresh evidence:

1. The original's body layout is **exactly** the `MachineBlockPlacement`
   `getFirstUnplacedBlock` fallback (MBB order = source order). A forced-fallback
   build of the project IR reproduces the original's body order **body for body**,
   including the low group and the latch/default at the end. (New: the forced
   build's 28 bodies are all mapped and compared to the original in §3.)
2. The fallback is not taken because **every high body has exactly one
   predecessor** (the high jump table), so `markBlockSuccessors` pushes all 22 of
   them into `BlockWorkList` in the jump table's successor order, which
   `buildJumpTable` emits in **ascending case-value order**. `selectBestCandidateBlock`
   then keeps the first entry on the equal-frequency tie. This is measured from the
   NDK MIR (`mc_mir.txt`), not inferred.
3. The 4-byte deficit is **layout, not missing code**: a body-by-body instruction
   diff of the original vs the plain-`int` build shows identical instruction
   sequences everywhere; the only deltas are short/wide branch encodings and the
   constant-island placement, both consequences of the layout.
4. A wide sweep of source shapes (loop forms, `default` placement, hole cases,
   two-switch, `-mllvm` flags) still never produces the fallback order.

No landable change: the plain-`int` form (the faithful one) is 1428 B and shifts
`.text`; the `(short)` cast is a size anchor that keeps the gate at 4498.

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

`report.json` row: `_ZN6Status16missionCompletedEbbx` → `pct 57.7`, `osize 1432`,
`usize 1432`. `.text 0x1337c8`, `.rodata 0x41b67`, `.ARM.exidx` exact,
`.ARM.extab 0x26290 (-8)`, lint `CLEAN ✓`, drift 0.

## 1. Original dispatch and body layout (re-verified from the ELF)

`Status::missionCompleted` at `0xabf38`, size 1432. One `Mission::getType()` call
(`0xabfd6`), **no `sxth`**, exactly two `tbh` in the whole function:

```
abfda: sub.w r1, r0, #150
abfde: cmp   r1, #39
abfe0: bhi.n ac046
abfe2: tbh   [pc, r1, lsl #1]      ; 40 halfwords @ abfe6, types 150..189
...
ac046: sub.w r1, r0, #8
ac04a: cmp   r1, #6
ac04c: bhi.n ac0be
ac04e: tbh   [pc, r1, lsl #1]      ;  7 halfwords @ ac052, types 8..14
ac0be: cmp   r0, #0
ac0c0: bne.w ac45a                 ; latch
```

Decoding the high table (`target = 0xabfe6 + 2*entry`) confirms the table is in
ascending **value** order while the bodies in memory are in **source** order:

```
ac036 167/174   ac110 150  ac116 151  ac11c 152  ac150 153  ac156 154
ac16e 155       ac172 tail ac180 158  ac1dc 162  ac242 166  ac2a6 170
ac2b4 184       ac2ee 189  ac32a 156  ac354 157  ac388 160  ac3b6 163
ac3d2 164       ac3d6 165  ac3de 168  ac3fc 169  ac400 171  ac41a 172
ac45a latch/default
```

i.e. exactly the `case` order in `src/game/mission/Status.cpp`:

```
167/174  8  0/11  13  14  150 151 152 153 154 155 158 162 166 170 184 189
156 157 160 163 164 165 168 169 171 172  default
```

The low group is `8, 0/11, 13, 14` (source order; type 0 and 11 share a body).

## 2. MIR evidence: why the fallback is not taken

The NDK build's first-ISel MIR (`/dev/shm/w4201/mc_mir.txt`, produced with
`-mllvm -print-machineinsts`) shows:

* `bb.102` is the high jump table. Its successor list is **ascending case value**
  (`buildJumpTable` adds successors in `Table` order; `sortAndRangeify` sorts by
  `slt`):

```
successors: %bb.32 %bb.34 %bb.36 %bb.41 %bb.43 %bb.45 %bb.77 %bb.79 %bb.47
            %bb.99 %bb.84 %bb.55 %bb.10 %bb.88 %bb.89 %bb.60 %bb.11 %bb.91
            %bb.93 %bb.64 %bb.94 %bb.96 %bb.66 %bb.71
```

* **Every one of those body blocks has exactly one predecessor, `%bb.102`:**

```
bb.10 ['%bb.102']  bb.11 ['%bb.102']  bb.32 ['%bb.102']  bb.34 ['%bb.102']
bb.36 ['%bb.102']  bb.41 ['%bb.102']  bb.43 ['%bb.102']  bb.45 ['%bb.102']
bb.47 ['%bb.102']  bb.55 ['%bb.102']  bb.60 ['%bb.102']  bb.64 ['%bb.102']
bb.66 ['%bb.102']  bb.71 ['%bb.102']  bb.77 ['%bb.102']  bb.79 ['%bb.102']
bb.84 ['%bb.102']  bb.88 ['%bb.102']  bb.89 ['%bb.102']  bb.91 ['%bb.102']
bb.93 ['%bb.102']  bb.94 ['%bb.102']  bb.96 ['%bb.102']
```

Therefore `fillWorkLists` gives each body chain `UnscheduledPredecessors == 1`, and
when the high jump table is merged, `markBlockSuccessors`
(`MachineBlockPlacement.cpp`) decrements each to 0 and **pushes all 22 bodies** into
`BlockWorkList`, in successor (ascending) order. `selectBestCandidateBlock` then
walks the worklist, and because every single-value body has the same frequency
(4.10 %) the first entry wins on the `BestFreq >= CandidateFreq` tie-break. The
result is the ascending layout.

The low jump table (`bb.104`, types 8/9..14) is placed first because its block
frequency is much higher (1.02 vs 0.1866), which is why the low group lands right
after 167/174 in both the original and our build. That part is *not* the
difference.

### 2.1 The fallback is only reached when the worklist is empty

`buildChain()` calls, in order:

```cpp
BestSucc = selectBestSuccessor(BB, Chain, BlockFilter);
if (!BestSucc) BestSucc = selectBestCandidateBlock(Chain, BlockWorkList);
if (!BestSucc) BestSucc = selectBestCandidateBlock(Chain, EHPadWorkList);
if (!BestSucc) BestSucc = getFirstUnplacedBlock(Chain, PrevUnplacedBlockIt, BlockFilter);
```

`getFirstUnplacedBlock` walks the `MachineFunction` in **MBB order**, which is the
IR block order, which is the source order (verified in the MIR: the case bodies are
created in source order). That is exactly the original's layout.

`markBlockSuccessors` skips a successor only when (a) it is in the same chain,
(b) it is the loop header, (c) `UnscheduledPredecessors == 0`, (d) the decrement
leaves it `> 0`, or (e) it is not in the loop's `BlockFilter`. The bodies satisfy
none of these: one predecessor each, all in the loop.

## 3. Forced-fallback build reproduces the original body order exactly

Patching `MachineBlockPlacement::selectBestCandidateBlock` to return `nullptr` in a
local LLVM 7.0.0 (`/home/debian/llvm7build`), compiling the project's plain-`int`
IR with `-function-sections`, and decoding the two TBH tables gives the physical
body order:

```
0xf4  167/174
0x11e 8      0x17e 0/11   0x1de 13    0x1ec 14
0x220 150    0x232 151    0x238 152   0x26c 153   0x272 154   0x284 155
0x28a 158    0x2e6 162    0x34c 166   0x3b0 170   0x3be 184   0x3f6 189
0x432 156    0x45e 157    0x490 160   0x4bc 163   0x4d8 164   0x4dc 165
0x4e6 168    0x4fa 169    0x4fe 171   0x508 172   0x548 default
```

— **the original's order, body for body, including the low group between 167/174
and 150 and the latch/default last.** (The body identification was done by
disassembling each target; e.g. `0x220` is `ldr r4,[r6,#452]` = case 150,
`0x4bc` is the `field_90` loop = case 163, `0x548` is `add fp,fp,#1` = latch.)

This is the existence proof: the original's layout *is* the
`getFirstUnplacedBlock` fallback. The local build's absolute size (1416 B) is not
comparable to the NDK's because LLVM 7.0.0 vs the NDK's 7.0.2svn differ in the
tail/constant-pool codegen (the local unforced build is 1404 B vs the NDK's 1428 B,
with the bodies at identical addresses up to case 172; the whole difference is in
the shared tails after case 189).

## 4. The 4 bytes are layout, not a missing statement

Recompiling the faithful plain-`int` form and diffing every body's instruction
sequence against the original (`bodysig2.py`, normalising branch targets) gives:

| body | orig | int | Δ | cause |
|---|---:|---:|---:|---|
| 156 | 42 | 44 | +2 | `bge.n` (orig) vs `bge.w` (ours) |
| 160 | 46 | 50 | +4 | `beq.n` vs `beq.w` (+2) and a branch-target width |
| 163 | 28 | 30 | +2 | `bcs.n` vs `bcs.w` |
| 166 | 100 | 100 | 0 | `beq.w`/`bne.w` (orig) vs `beq.n`/`bne.n` (ours); ours hoists `ldr.w r6,[r6,#400]` and needs `mov r0,r6` where the original keeps `this` in `r6` and does `ldr.w r0,[r6,#400]` (2 extra insns, same bytes) |
| 168 | 30 | 24 | −6 | the original's constant island (`nop` + 4-byte literal) sits after body 168 |
| 170 | 14 | 12 | −2 | `bne.w` (orig) vs `bne.n` (ours) |
| 172 | 64 | 66 | +2 | `cbnz` (orig) vs `cmp`+`bne.n` (ours) |

Net **+4** ⇒ 1432 vs 1428. Every other body is instruction-for-instruction
identical (167/174, 8, 0/11, 13, 14, 150, 151, 152, 153, 154, 155, 158, 162, 170,
184, 189, 156, 157, 160, 163, 164, 165, 168, 169, 171). The prologue and the whole
dispatch are byte-identical between the original and the `int` build; the first
difference is the 4-byte-shifted branch offsets. **There is no missing statement,
constant, or case body.**

Whole-function instruction accounting (excluding the 47 table halfwords and the 8
literal-pool halfwords): original **472 instructions / 1322 code bytes**, plain
`int` **475 instructions / 1318 code bytes**. The original has *three fewer*
instructions but *four more* bytes: the three extra `int` instructions are the
`mov r0,r6` in body 166 and the `cmp r0,#0` in body 172, while the original spends
its bytes on wider branches (`bne.w`/`beq.w`/`bge.n`→`bge.w` etc.) and the inline
constant island. Both are layout consequences.

The `(short)` cast is a size anchor: it inserts `sxth r0, r0` (2 bytes, not in the
binary) and changes the signed-narrow lowering so 184/189 are placed after the
default, which happens to restore 1432 B.

### 4.1 The constant pool is a consequence of the layout

The original has four GOT-relative literals for `Globals::status`
(`0x001640ee`, `0x00163ece`, `0x00163d78`, `0x00163e96`), one per `add r0, pc`
site (the literal value is `target - pc`, so each use needs its own entry). Three
sit at the end of the function (`0xac4c4`, `0xac4c8`, `0xac4cc`); the one used by
body 8 (`0xac068`) is placed as a constant island after body 168 (`0xac3f8`,
preceded by `nop` at `0xac3f6`). The plain-`int` build has the same four literals
but its island lands at `0x4b4` (between the default block and 184). The island
position, and therefore the +6 in body 168, follows the body layout.

## 5. Variant sweep (compile-only; `ordername.py` decodes the TBH targets)

All variants below are semantically equivalent rewrites of the switch/loop; each
was compiled with the project's exact flags (`/dev/shm/w4201/cc.sh`) and its body
order decoded.

| source form | size | high-table body order |
|---|---:|---|
| `switch ((short) m->getType())` (committed) | 1432 | ascending, 184/189 after default |
| `switch (m->getType())` (int, faithful) | 1428 | ascending |
| `switch ((int) …)` / `(unsigned)` / cached `int mtype` | 1428 | ascending |
| `default: break;` first / last, `default: continue;` | 1428 | ascending |
| `if (m) switch`, `if (true) switch`, `if (atStation \|\| !atStation) switch` | 1428 | ascending |
| `do { switch } while (0)`, explicit `continue` after the switch | 1428 | ascending |
| `for (i != size)`, `for (i <= size-1)`, `for (i >= 1; i--)` | 1428/1432 | ascending |
| `while (i < size)` / `do { } while (i < size)` / `for(;;)` | 1400/1432/1400 | ascending |
| hole cases `case 173:`, `case 159: case 161:`, `case 175..183:` | 1428 | ascending |
| `default: return 0;` | 1432 | ascending |
| two sequential switches on a cached `int` | 1508 | **original** |
| `-mllvm -block-placement-exit-block-bias=0..93` | 1428 | ascending |
| `-mllvm -block-placement-exit-block-bias=95..99` | 1472 | different (not original) |
| `-mllvm -tail-dup-placement`, `-tail-dup-placement-threshold=0` | 1428 | ascending |
| `-mllvm -force-precise-rotation-cost` | 1448 | ascending-ish |
| `-mllvm -force-loop-cold-block` | 1456 | ascending-ish |
| `-mllvm -loop-to-cold-block-ratio=1/100` | 1428 | ascending |
| `-mllvm -disable-block-placement` | 1436 | garbled |
| **forced `selectBestCandidateBlock -> nullptr`** | 1416 (local) | **exact original order** |

The two-switch form (1508 B) is the only *source* shape that reproduces the order,
and it costs +76 B because `mtype` stays live in `r11` across the first switch
(w4103). The forced-fallback build is the only shape that reproduces the order at
the plain-`int` register allocation.

## 6. Why the two-switch form cannot be trimmed to 1432

The two-switch form's extra 76 B is not just the second dispatch block (~5
instructions + a 17-byte TBB): it also moves `i` out of `fp`, `atStation` out of
`r8`, and the zero register out of `fp`, which changes the encoding of most bodies.
The original's prologue (`fp = 0` as the zero register, `i` in `fp`, `atStation` in
`r8`, `m` in `sl`, `this` in `r6` + `[sp,#16]`) is exactly the plain-`int` single
switch's allocation, so the original cannot have had a live `mtype` across the
switch. That rules out the two-switch source as the original form (it also emits an
extra TBB the binary does not have).

## 7. Conclusion — proven vs open

**Proven (this pass):**

1. The original's body layout is the MBP `getFirstUnplacedBlock` fallback (MBB
   order = source order). The forced-fallback build reproduces it body for body,
   including the low group and the latch/default.
2. The fallback is not taken because every high body has exactly one predecessor
   (the high jump table), so `markBlockSuccessors` pushes all 22 bodies in the
   jump table's ascending successor order; `selectBestCandidateBlock` keeps the
   first on the equal-frequency tie.
3. The 4-byte deficit is entirely branch-encoding + constant-island drift caused by
   the different layout; the body code is otherwise identical. There is no missing
   statement/constant/case body.
4. The plain-`int` single switch is the faithful form (byte-identical prologue and
   dispatch, no `sxth`) and is 1428 B / 60.2 %; the `(short)` cast is a size anchor.
5. No source shape tried (loop form, `default` placement, hole cases, two-switch,
   MBP flags) triggers the fallback at 1432 B with the plain-`int` allocation.

**Open:** a source shape that makes the high bodies' `UnscheduledPredecessors`
different from 1 at the moment the high jump table is merged — i.e. gives them a
second in-loop predecessor, or puts them in the jump table's chain — while keeping
the plain-`int` register allocation. If found, the function should become
byte-exact at 1432 B and `byte_exact` should rise 4498 → 4499.

## 8. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w4201
cmake --build cmake-build-match --target check -j8    # RATCHET PASS, byte 4498 linked 4498
cmake --build cmake-build-match --target lint  -j8    # CLEAN
cmake --build cmake-build-match --target drift -j8    # 0 fields

OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -d --start-address=0xabf38 --stop-address=0xac4d0 \
    /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so

# body order + size of a variant (scratch)
/dev/shm/w4201/cc.sh /dev/shm/w4201/Status_int.cpp /dev/shm/w4201/int.o
$OD -d /dev/shm/w4201/int.o > /dev/shm/w4201/int.asm
python3 /dev/shm/w4201/ordername.py /dev/shm/w4201/int.asm

# forced-fallback experiment (local LLVM 7.0.0 with assertions)
#   patch selectBestCandidateBlock -> nullptr in
#     llvm-7.0.0.src/lib/CodeGen/MachineBlockPlacement.cpp
#   make -C /home/debian/llvm7build/build llc -j8
#   llc -mtriple=thumbv7-none-linux-androideabi -mattr=+neon -float-abi=soft \
#       -function-sections -filetype=obj int_ndk_g0.ll -o int_forced.o
#   (restore with /dev/shm/w4201/MBP_orig.cpp)
```

Scratch (not committed): `/dev/shm/w4201/` — `Status_int.cpp` (faithful plain-int
form), `Status_short.cpp` (committed form), `cc.sh`, `ordername.py`, `bodysig2.py`,
`bodydiff.py`, `sweep2.py`..`sweep5.py`, `flag_sweep.py`, `mc_mir.txt` (NDK ISel
MIR), `int.asm` / `orig.asm` / `int_forced.asm`, `MBP_orig.cpp`.
