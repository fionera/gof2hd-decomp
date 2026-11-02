# `Status::missionCompleted` without the rank table (w3590)

Status: research note (no source change landed). Worktree `wt/w3590`, branch
`wave87-w3590`, based on master `76364fa7`. Original binary:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b clang 7.0.2
`r328903`, LLVM `1d739ffb…`, `-Oz`, image base 0x10000; real VMA = Ghidra - 0x10000).

Topic: the tree now switches directly on `(short) m->getType()` with no
`kMissionCase` rank table (w3570). The function is size-exact (1432 B) but only
**57.7 %** instruction-match. This note asks whether the original's source-order
body layout can be recovered **without** a rank table, by source shape or by
compile flags. Answer: no tested source/flag variant does; the direct switch's
jump-table successor order is ascending type value, and clang 7's
`MachineBlockPlacement` follows that order. A dense rank table is the only
mechanism found that reproduces the original body order.

## 1. Baseline (measured in this worktree)

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint -j8
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

`report.json` row:

```
{'name': '_ZN6Status16missionCompletedEbbx', 'pct': 57.7,
 'byte': False, 'linked': False, 'osize': 1432, 'usize': 1432}
```

`.text` is `0x1337c8 @ 0x69440`; `.rodata` is `0x41b67 @ 0x1c9e10` (0 differing
bytes). The current source is the direct `switch ((short) m->getType())` with
the real mission-type case labels; there is no `kMissionCase`, no
`.missioncase` section, and no lookup-table load.

## 2. Original dispatch (binary evidence)

```sh
OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -d --start-address=0xabf38 --stop-address=0xac4d0 \
    ../_work/bins/android_2.0.16_libgof2hdaa.so
```

The original body is 1432 B at `0xabf38`. At `0xabfd6`:

```
abfd6: blx  Mission::getType()
abfda: sub.w r1, r0, #150
abfde: cmp   r1, #39
abfe0: bhi.n ac046
abfe2: tbh   [pc, r1, lsl #1]      @ 40 halfwords at 0xabfe6
...
ac046: sub.w r1, r0, #8
ac04a: cmp   r1, #6
ac04c: bhi.n ac0be
ac04e: tbh   [pc, r1, lsl #1]      @ 7 halfwords at 0xac052
```

There is **no `sxth`** and **no byte-table load**. Decoding the two tables
(`target = table_base + 2*entry`, `table_base = 0xabfe6` / `0xac052`) gives the
original body order:

```
167/174, 8, 11, 13, 14, 150, 151, 152, 153, 154, 155, 158, 162, 166, 170,
184, 189, 156, 157, 160, 163, 164, 165, 168, 169, 171, 172, default
```

That is exactly the order the case labels appear in the current
`src/game/mission/Status.cpp` (the w3570 source order).

## 3. The direct switch always lays out ascending type value

The current 1432-byte `(short)` form and a direct `int` form both emit the same
body order, which is **ascending type value**, not the original order:

```
167/174, 8, 11, 13, 14, 150, 151, 152, 153, 154, 155, 156, 157, 158, 160,
162, 163, 164, 165, 166, 168, 169, 170, 171, 172, default, 184, 189
```

Compile-only proof (no link needed). The exact compile command is taken from
`cmake-build-match/compile_commands.json`; the parser reads the emitted `.s`,
decodes the two `.LJTI` tables, and prints the case bodies in layout order:

```sh
# /tmp/mc_cmd.sh is the Status.cpp compile command with -S -o /tmp/status.s
bash /tmp/mc_cmd.sh
python3 /tmp/mc_layout.py /tmp/status.s
```

Results:

| source variant | layout |
|---|---|
| baseline `(short) m->getType()` | value order (above) |
| `m->getType()` (no cast) | value order (same) |
| case blocks moved (156 before 150; mid-table group first; 163 first) | value order (same) |
| explicit `default: break;` | value order (same) |

Source order therefore does not influence the emitted layout. The reason is in
clang 7's SelectionDAG lowering:

* `SelectionDAGBuilder::sortAndRangeify()` sorts `CaseCluster`s by signed case
  value (`llvm::sort(..., a.Low->getValue().slt(b.Low->getValue()))`).
* `SelectionDAGBuilder::buildJumpTable()` builds `Table` by walking the sorted
  clusters and adds the jump-table MBB's successors in **table order**
  (`// Add successors. Note: use table order for determinism.`).
* `MachineBlockPlacement` follows the successor list when all successor
  probabilities are equal. The MIR dump confirms the successor order:

```sh
# compile Status.cpp with -mllvm -print-machineinstrs and inspect the
# "After Instruction Selection" dump for missionCompleted
%jump-table.1: %bb.32 %bb.34 %bb.36 %bb.41 %bb.43 %bb.45 %bb.77 %bb.79
               %bb.47 %bb.99 %bb.84 %bb.99 %bb.55 %bb.10 %bb.88 %bb.89
               %bb.60 %bb.11 %bb.91 %bb.93 %bb.64 %bb.94 %bb.96 ...
# 150,151,152,153,154,155,156,157,158,default,160,default,162,163,...
```

The only non-equal *case* successor probability is `167/174` (two case values
share one body, 8.19 % vs 4.10 %; the default edge is 1.71 %), which is why it
is placed first in both our and the original layout.

## 4. Direct `int` switch A/B (full gate)

```sh
cp src/game/mission/Status.cpp /tmp/Status.cpp.bak
sed -i 's/switch ((short) m->getType())/switch (m->getType())/' \
    src/game/mission/Status.cpp
cmake --build cmake-build-match --target check -j8
cp /tmp/Status.cpp.bak src/game/mission/Status.cpp
cmake --build cmake-build-match --target check -j8
```

Measured:

| tree | byte_exact | linked_exact | `.text` | `missionCompleted` | verdict |
|---|---:|---:|---|---|---|
| baseline `(short)` | **4496** | **4496** | `0x1337c8` | 1432 B / 57.7 % | PASS |
| direct `int` | 3043 | 4496 | `0x1337c8` | 1428 B / 60.2 % | RATCHET FAIL |

The direct `int` form is 4 B shorter; the `(short)` cast adds the `sxth` and
restores the size. The original has no `sxth`, so the cast is a size/layout
compensation, not recovered source. It is still the only gate-safe form.

## 5. Compile-flag sweep (compile-only layout)

All rows compile `Status.cpp` with the project's exact flags plus the listed
`-mllvm` option and print the layout with `/tmp/mc_layout.py`.

| flag | layout | size |
|---|---|---|
| none | value order | 1432 |
| `-arm-adjust-jump-tables=true/false` | value order | 1432 |
| `-switch-peel-threshold=0..1000` | value order | 1432 |
| `-tail-dup-placement=true/false` | value order | 1432 |
| `-branch-fold-placement=true/false` | value order | 1432 |
| `-tail-dup-placement-threshold=0/1000` | value order | 1432 |
| `-tail-dup-placement-penalty=0/100` | value order | 1432 |
| `-triangle-chain-count=0/100` | value order | 1432 |
| `-force-precise-rotation-cost` | value order, `184/189` before default | 1432 |
| `-block-placement-exit-block-bias=100` | reverse value order (`189…150`, `167/174` and default interleaved) | 1432 |
| `-disable-block-placement` | reverse value order (`189,184,167/174,172…150`), high table becomes `tbb` | 1436 |

None produces the original source order. `-disable-block-placement` is the only
one that changes the jump-table encoding (TBH -> TBB) and the size (1436), so it
is not the original build either.

Full-gate negative control: `-mllvm -force-precise-rotation-cost` scoped to
`Status.cpp` via `set_source_files_properties(... COMPILE_OPTIONS
"-mllvm;-force-precise-rotation-cost")` gives **byte 3015 / linked 4477**
(RATCHET FAIL), so it is not a strict improvement.

## 6. The rank-table mechanism (positive control)

A dense switch on a rank that is ascending in source order does reproduce the
original body order. A scratch `Status.cpp` with a 192-byte
`static const unsigned char kRank[192]` (rank 0..26 assigned in source order,
default 27) and `switch ((short) kRank[m->getType()])` emits a single 27-entry
table and lays the bodies out as `0,1,2,…,26` — i.e. source order. This is the
mechanism the removed `kMissionCase` used. It is a codegen-shaping device, not
original data: the original has no table load and two type-indexed TBH tables.

## 7. Conclusion

**Proven (commands above):**

1. The original `Status::missionCompleted` is 1432 B, switches directly on
   `m->getType()` with two in-text TBH tables (40 + 7 entries), has no `sxth`,
   and lays the case bodies out in source order.
2. The current direct `(short) m->getType()` switch is 1432 B / 57.7 % and
   gate-neutral (byte 4496, linked 4496, `.text 0x1337c8`, lint clean, drift 0).
3. The direct `int` switch is 1428 B / 60.2 % and fails the ratchet
   (byte 3043); the `(short)` cast is required to keep the size.
4. clang 7's `sortAndRangeify` + `buildJumpTable` add the jump-table successors
   in ascending case-value order; `MachineBlockPlacement` follows that order, so
   the direct switch always lays out bodies in ascending type order.
5. Source case reordering and every tested `-mllvm` block-placement/switch flag
   leave the layout in value order (or reverse value order for
   `-block-placement-exit-block-bias=100` / `-disable-block-placement`).
6. A dense source-order rank switch does reproduce source-order layout; that is
   the only mechanism found.

**Hypothesis (not proven):**

* The original's source-order layout implies its `MachineBlockPlacement` did not
  follow the value-order jump-table successor list (or its successor list was
  source order). The exact source/CFG feature that caused this is not recovered.
  It is not reproduced by any source ordering or flag tested here.
* Without a rank table the function is therefore an irreducible 57.7 % near-miss
  under the project's fixed clang 7.0.2 `-Oz` flags; the remaining delta is the
  block order and the branch encodings it causes.

## 8. Reproduction commands

```sh
cd /home/debian/GalaxyOnFire2/wt/w3590
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint -j8
cmake --build cmake-build-match --target drift -j8

# original dispatch + body order
OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -d --start-address=0xabf38 --stop-address=0xac4d0 \
    ../_work/bins/android_2.0.16_libgof2hdaa.so
python3 - <<'PY'
import struct
d=open('../_work/bins/android_2.0.16_libgof2hdaa.so','rb').read()
def decode(base,n,start):
    for i in range(n):
        e=struct.unpack_from('<H',d,base+2*i)[0]
        print(start+i, hex(base+2*e))
decode(0xabfe6,40,150)
decode(0xac052,7,8)
PY

# compile-only layout parser: /tmp/mc_cmd.sh is the Status.cpp command with
# -S -o /tmp/status.s; /tmp/mc_layout.py decodes the .LJTI tables and prints
# the case bodies in layout order.
bash /tmp/mc_cmd.sh
python3 /tmp/mc_layout.py /tmp/status.s

# MIR successor order
#   add -mllvm -print-machineinstrs to the compile command and inspect the
#   "After Instruction Selection" dump for _ZN6Status16missionCompletedEbbx

# direct int A/B
cp src/game/mission/Status.cpp /tmp/Status.cpp.bak
sed -i 's/switch ((short) m->getType())/switch (m->getType())/' \
    src/game/mission/Status.cpp
cmake --build cmake-build-match --target check -j8
cp /tmp/Status.cpp.bak src/game/mission/Status.cpp
cmake --build cmake-build-match --target check -j8
```
