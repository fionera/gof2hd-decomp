# `Status::missionCompleted` — MBP probability investigation (w3633)

Status: research note (2026-11-02). Worktree `wt/w3633`, branch `wave87-w3633`,
based on `00dfbc4c` (the two-switch note). Original binary:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b clang 7.0.2
`r328903`, LLVM `1d739ffb…`, `-Oz`, image base 0x10000; real VMA = Ghidra − 0x10000).
No source or flag change landed; only this note is committed.

Topic: is the residual `Status::missionCompleted(bool,bool,long long)` mismatch an
MBP (MachineBlockPlacement) **probability** tie-break, and is any probability flag a
strict gate improvement?

**Answer: no.** The original's case bodies are laid out in *source* order; our build
lays them out in *jump-table (value)* order. The MBP successor list of the high jump
table is value order (`bb.104` below), and the original's physical order requires a
*source-order successor list*, not a different probability value. Every tested
`-mllvm` probability knob changes size and/or order but none produces the original
source-order layout with the original two-TBH shape. A full-gate A/B with
`-mllvm -static-likely-prob=84` scoped to `Status.cpp` is a large regression
(`byte_exact 4496 -> 3025`, `linked_exact 4496 -> 4492`, `RATCHET FAIL`), so no flag
change is a strict improvement.

## 0. Baseline (this worktree)

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8
```

```
verify: byte 4496  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓ (no new soft violations; hard invariants 0)
drift: 0 field(s) across 0 class(es)
```

Section sizes/bases (all exact):

```
.text        0x1337c8 @ 0x69440
.ARM.extab   0x26290  @ 0x1a3b78
.rodata      0x41b67  @ 0x1c9e10
```

`report.json` row: `_ZN6Status16missionCompletedEbbx` -> `osize 1432`,
`usize 1432`, `pct 57.7`, `byte false`, `linked false`.

## 1. Original binary facts

`Status::missionCompleted` is at `0xabf38`, size `1432`. It contains exactly one
`Mission::getType()` call (`0xabfd6`), no `sxth`, and two TBH tables:

```sh
OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -d --start-address=0xabf38 --stop-address=0xac4d0 \
    ../_work/bins/android_2.0.16_libgof2hdaa.so > /tmp/mc_orig.asm
grep -nE 'getType|tbh|tbb|sxth' /tmp/mc_orig.asm
```

```
abfd6: blx  Mission::getType()
abfda: sub.w r1, r0, #150
abfde: cmp   r1, #39
abfe0: bhi.n ac046
abfe2: tbh   [pc, r1, lsl #1]      ; table 1: 40 halfwords @ 0xabfe6 (types 150..189)
ac046: sub.w r1, r0, #8
ac04a: cmp   r1, #6
ac04c: bhi.n ac0be
ac04e: tbh   [pc, r1, lsl #1]      ; table 2: 7 halfwords @ 0xac052 (types 8..14)
```

Decoding the two tables and sorting the targets by address gives the original
case-body layout:

```sh
python3 - <<'PY'
import struct
d=open('../_work/bins/android_2.0.16_libgof2hdaa.so','rb').read()
def dec(table_vma,n,first,base):
    return [(first+i, base+2*struct.unpack_from('<H',d,table_vma+2*i)[0])
            for i in range(n)]
hi=dec(0xabfe6,40,150,0xabfe6)
lo=dec(0xac052,7,8,0xac052)
allc=sorted(hi+lo,key=lambda x:(x[1],x[0]))
groups=[];last=None
for t,a in allc:
    if a!=last: groups.append([]);last=a
    groups[-1].append(t)
print(' '.join('/'.join(map(str,g)) for g in groups))
PY
```

```
167/174 8 11 13 14 150 151 152 153 154 155 158 162 166 170 184 189
156 157 160 163 164 165 168 169 171 172
9/10/12/159/161/173/175/176/177/178/179/180/181/182/183/185/186/187/188
```

The first line is exactly the source order of the `case` labels in
`src/game/mission/Status.cpp` (with `case 0` sharing the `case 11` body). The
jump-table entries themselves are value-indexed (type 150..189); only the
*target addresses* are source order. The default body is at `0xac45a`, at the
far end of the function, which is why both tables must be TBH (the low-table
default is 0x408 bytes from `0xac052`; TBB reaches only 510 bytes).

## 2. Current build: same size, value-order bodies

Compile-only object (exact project flags, from `compile_commands.json`):

```sh
/tmp/status_compile.sh /tmp/status.o
readelf -sW /tmp/status.o | grep _ZN6Status16missionCompletedEbbx
python3 /tmp/decode_obj.py /tmp/status.o
```

```
1432  FUNC  _ZN6Status16missionCompletedEbbx
table 0xac tbh  [pc, r1, lsl #1] first 150 n 40
167/174 150 151 152 153 154 155 156 157 158 160 162 163 164 165 166 168 169 170 171 172
159/161/173/175/176/177/178/179/180/181/182/183/185/186/187/188 184 189
table 0x118 tbh [pc, r1, lsl #1] first 8 n 7
8 11 13 14 9/10/12
```

The high-table bodies are ascending by case value (with the default group before
`184/189`), not source order. The low-table bodies (`8 11 13 14`) happen to be
the same because the low-table source order is also value order.

The current source uses `switch ((short) m->getType())`. Removing the `(short)`
cast (an `int` switch) gives `1428 B` and the same value-order layout; the
`(short)` adds the `sxth` and restores `1432 B`. The original has no `sxth`, so
the original's `1432 B` is not explained by the cast; it is consistent with the
source-order layout having wider branch encodings.

### Source case order does not change the layout

Move the `case 167: case 174:` block from the top of the switch to the bottom
(just before the switch's closing brace) and compile the same TU:

```sh
# /tmp/Status_167last.cpp is src/game/mission/Status.cpp with the 167/174 block moved last
SRC=/tmp/Status_167last.cpp /tmp/status_compile_src.sh /tmp/status_167last.o
readelf -sW /tmp/status_167last.o | awk '$4=="FUNC" && $8=="_ZN6Status16missionCompletedEbbx"{print $3; exit}'
python3 /tmp/decode_obj.py /tmp/status_167last.o
```

```
1432
table 0xac tbh  [pc, r1, lsl #1] first 150 n 40
167/174 150 151 152 153 154 155 156 157 158 160 162 163 164 165 166 168 169 170 171 172
159/161/173/175/176/177/178/179/180/181/182/183/185/186/187/188 184 189
table 0x118 tbh [pc, r1, lsl #1] first 8 n 7
8 11 13 14 9/10/12
```

The object is byte-for-byte the same layout as the baseline. `sortAndRangeify`
sorts the switch clusters by case value before `buildJumpTable`, so the source
`case` order cannot reorder the jump-table successor list.

## 3. MBP evidence: the successor list is value order

Dump the machine instructions for `Status.cpp` and extract the MBP pass:

```sh
/tmp/status_compile.sh /tmp/status_mir.o -mllvm -print-machineinstrs > /tmp/mir_status.txt 2>&1
# missionCompleted's MBP pass is the "# After Branch Probability Basic Block Placement:"
# block around line 872760; /tmp/mc_mbp.txt is that function's pass.
```

`bb.10` is the switch header; `bb.104` is the high jump-table block:

```
bb.10 (%ir-block.43):
  successors: %bb.11(0x17777777), %bb.104(0x68888889); %bb.11(18.33%), %bb.104(81.67%)
  ...
  renamable $r0 = t2SXTH killed renamable $r0, ...
  renamable $r1 = t2SUBri renamable $r0, 150, ...
  t2CMPri renamable $r1, 39, ...
  t2Bcc %bb.11, 8, killed $cpsr

bb.104 (%ir-block.43):
  successors: %bb.36(0x053e08bd), %bb.38(0x053e08bd), %bb.39(0x053e08bd),
             %bb.44(0x053e08bd), %bb.45(0x053e08bd), %bb.46(0x053e08bd),
             %bb.77(0x053e08bd), %bb.80(0x053e08bd), %bb.47(0x053e08bd),
             %bb.103(0x022f2e4e), %bb.85(0x053e08bd), %bb.57(0x053e08bd),
             %bb.88(0x053e08bd), %bb.91(0x053e08bd), %bb.92(0x053e08bd),
             %bb.62(0x053e08bd), %bb.16(0x0a7c1179), %bb.93(0x053e08bd),
             %bb.96(0x053e08bd), %bb.66(0x053e08bd), %bb.97(0x053e08bd),
             %bb.98(0x053e08bd), %bb.67(0x053e08bd), %bb.71(0x053e08bd);
             %bb.36(4.10%), %bb.38(4.10%), %bb.39(4.10%), %bb.44(4.10%),
             %bb.45(4.10%), %bb.46(4.10%), %bb.77(4.10%), %bb.80(4.10%),
             %bb.47(4.10%), %bb.103(1.71%), %bb.85(4.10%), %bb.57(4.10%),
             %bb.88(4.10%), %bb.91(4.10%), %bb.92(4.10%), %bb.62(4.10%),
             %bb.16(8.19%), %bb.93(4.10%), %bb.96(4.10%), %bb.66(4.10%),
             %bb.97(4.10%), %bb.98(4.10%), %bb.67(4.10%), %bb.71(4.10%)
```

Mapping the block numbers to case values gives the successor order

```
150 151 152 153 154 155 156 157 158 D 160 162 163 164 165 166 167/174 168 169 170 171 172 184 189
```

where `D` is the shared default target. This is exactly the value order of the
jump table. MBP picks the highest-probability successor first (`167/174`, 8.19%),
then `selectBestCandidateBlock` picks the low switch header (18.33%) and its
bodies, then the remaining high bodies in successor-list order (value order).
The original's physical order requires this successor list to be in source
order instead:

```
167/174 8 11 13 14 150 151 152 153 154 155 158 162 166 170 184 189 156 157 160 163 164 165 168 169 171 172 D
```

The prior MIR-rewrite experiment (`Status-missionCompleted-blockorder.md`)
confirms that rewriting `bb.104`'s successor list to source order reproduces the
original layout; reversing it reverses the layout. The probability values are
not the variable: the successor-list order is.

## 4. Probability/flag sweep (compile-only)

`D` below is `159/161/173/175/176/177/178/179/180/181/182/183/185/186/187/188`
(the shared default body). Every row is the high-table body order decoded from
the object; `size` is the `missionCompleted` symbol size. The original is
`1432` B with the source-order row in §3.

| flag | size | high-table body order |
|---|---:|---|
| baseline | 1432 | 167/174 150 151 152 153 154 155 156 157 158 160 162 163 164 165 166 168 169 170 171 172 D 184 189 |
| `-static-likely-prob=1` | 1448 | 167/174 D 150 151 152 153 154 155 156 157 158 160 162 163 164 165 166 168 169 170 171 172 184 189 |
| `-static-likely-prob=10` | 1448 | 167/174 D 150 151 152 153 154 155 156 157 158 160 162 163 164 165 166 168 169 170 171 172 184 189 |
| `-static-likely-prob=25` | 1444 | 167/174 D 150 151 152 153 154 155 156 157 158 160 162 163 164 165 166 168 169 170 171 172 184 189 |
| `-static-likely-prob=50` | 1444 | 167/174 150 151 152 153 154 155 D 156 157 158 160 162 163 164 165 166 168 169 170 171 172 184 189 |
| `-static-likely-prob=75` | 1432 | 167/174 150 151 152 153 154 155 156 157 158 160 162 163 164 165 166 168 169 170 171 172 D 184 189 |
| `-static-likely-prob=80` | 1432 | 167/174 150 151 152 153 154 155 156 157 158 160 162 163 164 165 166 168 169 170 171 172 D 184 189 |
| `-static-likely-prob=81` | 1428 | 167/174 150 151 152 153 154 155 156 157 158 160 162 163 164 165 166 168 169 170 171 172 D 184 189 |
| `-static-likely-prob=84` | 1432 | 167/174 150 151 152 153 154 155 156 157 158 160 162 163 164 165 166 168 169 170 171 172 184 189 D |
| `-static-likely-prob=90` | 1424 | 167/174 150 151 152 153 154 155 156 157 158 160 162 163 164 165 166 168 169 170 171 172 184 189 D |
| `-static-likely-prob=100` | 1424 | 167/174 150 151 152 153 154 155 156 157 158 160 162 163 164 165 166 168 169 170 171 172 184 189 D |
| `-block-placement-exit-block-bias=0` | 1432 | 167/174 150 151 152 153 154 155 156 157 158 160 162 163 164 165 166 168 169 170 171 172 D 184 189 |
| `-block-placement-exit-block-bias=50` | 1432 | 167/174 150 151 152 153 154 155 156 157 158 160 162 163 164 165 166 168 169 170 171 172 D 184 189 |
| `-block-placement-exit-block-bias=90` | 1432 | 167/174 150 151 152 153 154 155 156 157 158 160 162 163 164 165 166 168 169 170 171 172 D 184 189 |
| `-block-placement-exit-block-bias=95` | 1476 | 189 184 172 171 170 169 168 166 165 164 163 162 160 158 157 156 167/174 D 150 151 152 153 154 155 |
| `-block-placement-exit-block-bias=99` | 1476 | 189 184 172 171 170 169 168 166 165 164 163 162 160 158 157 156 167/174 D 150 151 152 153 154 155 |
| `-block-placement-exit-block-bias=100` | 1476 | 189 184 172 171 170 169 168 166 165 164 163 162 160 158 157 156 167/174 D 150 151 152 153 154 155 |
| `-block-placement-exit-block-bias=110` | 1432 | 167/174 150 151 152 153 154 155 156 157 158 160 162 163 164 165 166 168 169 170 171 172 D 184 189 |
| `-likely-branch-weight=1` | 1432 | 167/174 150 151 152 153 154 155 156 157 158 160 162 163 164 165 166 168 169 170 171 172 D 184 189 |
| `-likely-branch-weight=80` | 1432 | 167/174 150 151 152 153 154 155 156 157 158 160 162 163 164 165 166 168 169 170 171 172 D 184 189 |
| `-likely-branch-weight=2000` | 1432 | 167/174 150 151 152 153 154 155 156 157 158 160 162 163 164 165 166 168 169 170 171 172 D 184 189 |
| `-likely-branch-weight=10000` | 1432 | 167/174 150 151 152 153 154 155 156 157 158 160 162 163 164 165 166 168 169 170 171 172 D 184 189 |
| `-unlikely-branch-weight=1` | 1432 | 167/174 150 151 152 153 154 155 156 157 158 160 162 163 164 165 166 168 169 170 171 172 D 184 189 |
| `-unlikely-branch-weight=100` | 1432 | 167/174 150 151 152 153 154 155 156 157 158 160 162 163 164 165 166 168 169 170 171 172 D 184 189 |
| `-unlikely-branch-weight=10000` | 1432 | 167/174 150 151 152 153 154 155 156 157 158 160 162 163 164 165 166 168 169 170 171 172 D 184 189 |
| `-profile-likely-prob=1` | 1432 | 167/174 150 151 152 153 154 155 156 157 158 160 162 163 164 165 166 168 169 170 171 172 D 184 189 |
| `-profile-likely-prob=50` | 1432 | 167/174 150 151 152 153 154 155 156 157 158 160 162 163 164 165 166 168 169 170 171 172 D 184 189 |
| `-profile-likely-prob=100` | 1432 | 167/174 150 151 152 153 154 155 156 157 158 160 162 163 164 165 166 168 169 170 171 172 D 184 189 |
| `-disable-block-placement` | 1436 | reverse value order, TBB high table (not TBH) |
| `-enable-tail-merge=false` | 1640 | 167/174 150 151 152 153 154 155 156 157 158 160 162 163 164 165 166 168 169 170 171 172 184 189 D |
| `-use-mbpi=false` | 1400 | different table shape (TBB low table) |
| `-arm-adjust-jump-tables=false` | 1432 | baseline value order |
| `-branch-fold-placement=false` | 1432 | baseline value order |
| `-tail-dup-placement=false` | 1432 | baseline value order |
| `-switch-to-lookup` | 1432 | baseline value order |
| `-forward-switch-cond` | 1432 | baseline value order |

None of these rows is the original source-order layout. The closest are
`-static-likely-prob=84` / `-enable-tail-merge=false`, which move `184/189`
before the default but still leave the high bodies in value order. The
`-block-placement-exit-block-bias=95/99/100` rows are reverse value order.

### PGO variants

A synthetic `-fprofile-instr-use` profile is another way to change MBP's
probabilities. The function has 116 counters (hash `6982503786148174855`).
Two fresh measurements:

```sh
# all counters 10000
python3 - <<'PY'
name='_ZN6Status16missionCompletedEbbx'; h='6982503786148174855'; n=116
with open('/tmp/mc_all.proftext','w') as f:
    f.write(name+'\n# Func Hash:\n'+h+'\n# Num Counters:\n'+str(n)+'\n# Counter Values:\n')
    for _ in range(n): f.write('10000\n')
PY
llvm-profdata merge -o /tmp/mc_all.profdata /tmp/mc_all.proftext
/tmp/status_compile.sh /tmp/status_pgo_all.o -fprofile-instr-use=/tmp/mc_all.profdata
```

```
1424
table 0x5c tbh [pc, r1, lsl #1] first 150 n 40
D 167/174 172 166 171 150 157 163 151 152 153 154 155 156 162 164 165 168 158 160 169 189 170 184
table 0xce tbb [pc, r1] first 8 n 7
9/10/12 8 11 13 14
```

With counter 6 (the default body) set to 1 and all other counters 10000:

```
1420
table 0x5c tbh [pc, r1, lsl #1] first 150 n 40
D 167/174 172 166 171 154 157 163 165 150 151 152 153 155 156 162 164 168 169 158 160 189 170 184
table 0xce tbb [pc, r1] first 8 n 7
9/10/12 8 11 13 14
```

PGO moves the default body earlier and changes the low table from TBH to TBB
(the default is no longer at the far end), and the high bodies become a
profile-dependent permutation, not source order. The original has TBH for both
tables, so these PGO shapes are not the original either. (Earlier synthetic
profiles in this worktree gave the same conclusion: a `10000-rank*100` profile
gave `D 167/174 150 151 152 153 154 155 158 162 166 170 184 189 156 157 160 163 164 165 168 169 171 172`,
low TBB, ~1420 B; uniform profiles gave hash-like orders.)

## 5. Full-gate A/B: `-static-likely-prob=84`

The most promising probability value (it moves `184/189` before the default and
keeps 1432 B) is still a full-gate regression. Add a scoped flag to the build:

```diff
 set_source_files_properties("${CMAKE_SOURCE_DIR}/src/game/weapons/Radar.cpp"
     PROPERTIES COMPILE_OPTIONS "-mllvm;-likely-branch-weight=80;-mllvm;-unlikely-branch-weight=1")
+set_source_files_properties("${CMAKE_SOURCE_DIR}/src/game/mission/Status.cpp"
+    PROPERTIES COMPILE_OPTIONS "-mllvm;-static-likely-prob=84")
```

```sh
cmake --build cmake-build-match --target check -j8
```

```
verify: byte 3025  linked 4492  stubs 0  missing 0  extra 0  avg 99.88
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
  FAIL  byte_exact 4496 -> 3025
  FAIL  linked_exact 4496 -> 4492 (beyond the +-1 band)
RATCHET FAIL
```

Revert `CMakeLists.txt` and rebuild:

```sh
cp /tmp/CMakeLists.txt.w3633 CMakeLists.txt
cmake --build cmake-build-match --target check -j8
```

```
verify: byte 4496  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

The flag changes block placement in many `Status.cpp` functions even when their
sizes are unchanged (only `departStation` 1008 -> 1004 and
`nextCampaignMission` 8492 -> 8484 change size, but the total `byte_exact`
drops by 1471).
Both of those functions are currently byte-exact, so the flag is not a strict
improvement.

## 6. Conclusion: proven vs hypothesis

**Proven (commands above):**

- Baseline is `byte 4496 / linked 4496 / .text 0x1337c8`, lint clean, drift 0.
- Original `missionCompleted` is 1432 B, one `getType` call, no `sxth`, two TBH
  tables (40 + 7), and its case bodies are in source order.
- Current build is 1432 B, two TBH tables (40 + 7), and its case bodies are in
  value order (default group before `184/189`).
- The MBP successor list of the high jump table (`bb.104`) is value order, with
  `4.10%` per single-case body, `8.19%` for `167/174`, and `1.71%` for the
  default. MBP's layout follows that successor list after placing `167/174`
  first and the low switch bodies next.
- Moving `case 167/174` to the bottom of the source switch leaves the object
  layout unchanged: `sortAndRangeify` sorts by value before `buildJumpTable`.
- No tested `-mllvm` probability knob (`static-likely-prob`,
  `block-placement-exit-block-bias`, `likely-branch-weight`,
  `unlikely-branch-weight`, `profile-likely-prob`) produces the original
  source-order layout with the original two-TBH shape.
- Synthetic PGO changes the order and the low-table encoding, but does not
  produce the original.
- `-static-likely-prob=84` scoped to `Status.cpp` is a full-gate regression
  (`byte 4496 -> 3025`, `linked 4496 -> 4492`), not a strict improvement.

**Hypothesis (not proven):**

- The original's source-order layout comes from a **source-order jump-table
  successor list** (i.e. `buildJumpTable`'s `Table` order), not from a
  probability value. The current compiler's `sortAndRangeify` sorts the switch
  clusters by value, so the successor list is value order. The original's
  source-order successor list is not reachable from any natural source shape
  tested so far (single switch, two switches, nested switch, if-chain) while
  keeping the original's two-TBH shape. The exact source/compiler mechanism
  that produced it remains unidentified; a different `Table`/successor
  emission (or a source shape that makes `buildJumpTable` see source-ordered
  clusters) is the leading hypothesis.

## 7. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w3633
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8

# original tables + body order
python3 - <<'PY'
import struct
d=open('../_work/bins/android_2.0.16_libgof2hdaa.so','rb').read()
def dec(table_vma,n,first,base):
    return [(first+i, base+2*struct.unpack_from('<H',d,table_vma+2*i)[0])
            for i in range(n)]
hi=dec(0xabfe6,40,150,0xabfe6); lo=dec(0xac052,7,8,0xac052)
allc=sorted(hi+lo,key=lambda x:(x[1],x[0])); groups=[]; last=None
for t,a in allc:
    if a!=last: groups.append([]); last=a
    groups[-1].append(t)
print(' '.join('/'.join(map(str,g)) for g in groups))
PY

# current object
/tmp/status_compile.sh /tmp/status.o
readelf -sW /tmp/status.o | grep _ZN6Status16missionCompletedEbbx
python3 /tmp/decode_obj.py /tmp/status.o

# MBP MIR
/tmp/status_compile.sh /tmp/status_mir.o -mllvm -print-machineinstrs > /tmp/mir_status.txt 2>&1

# flag probe (example)
/tmp/status_compile.sh /tmp/probe.o -mllvm -static-likely-prob=84
readelf -sW /tmp/probe.o | awk '$4=="FUNC" && $8=="_ZN6Status16missionCompletedEbbx"{print $3; exit}'
python3 /tmp/decode_obj.py /tmp/probe.o
```

`/tmp/status_compile.sh` is the exact project `Status.cpp` compile command from
`compile_commands.json` plus `-ffunction-sections -funwind-tables`;
`/tmp/decode_obj.py` decodes the TBH/TBB tables from an object and prints the
case-body layout.
