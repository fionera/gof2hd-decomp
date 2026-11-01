# Removing `kMissionCase[192]` and its `.missioncase` section (w3570)

Status: research note + landed source fix. Worktree `wt/w3570`, branch `wave87-w3570`,
based on master `54e36581`. Original binary:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b clang 7.0.2,
`-Oz`, image base 0x10000; real VMA = Ghidra - 0x10000).

## TL;DR

* The current tree carries a 192-byte invented `kMissionCase[192]` type->rank
  permutation in its own ELF section `.missioncase`
  (`#pragma clang section rodata=".missioncase"` in `src/game/mission/Status.cpp`).
  The original has no such section and no such table.
* Removing the table and switching directly on `(short) m->getType()` with the
  actual mission-type case labels keeps the gate **exactly unchanged**
  (`byte_exact 4496`, `linked_exact 4496`, `.text 0x1337c8`, `.rodata` 0
  differing bytes, lint `CLEAN`, drift 0) and removes the extra ELF section:
  section count **28 -> 27** (original: 27), `.shstrtab` **0x124 -> 0x117**
  (original: 0x117). The section table now matches the original except the
  known `.ARM.extab` `-8` delta.
* The rank table was a codegen hack, not original data. It forced clang 7 to
  lay the switch bodies out in source order, raising the fuzzy match of
  `Status::missionCompleted` from **57.7% to 86.7%**. It is semantically wrong
  for six mission types (161, 162, 166, 168, 169 and the 169/171 split); the
  direct switch restores the original type -> body mapping.
* A direct `int` switch (no `(short)` cast) is still a ratchet failure
  (`byte_exact 4496 -> 3043`) because clang emits a 1428-byte body instead of
  the original 1432 bytes. The `(short)` cast is a size/layout compensation,
  not original codegen: the original has no `sxth` before `sub #150`.

## 1. Baseline

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint -j8
cmake --build cmake-build-match --target drift -j8
```

Measured on `54e36581` (rank table present):

```
verify: byte 4496  linked 4496  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓ (no new soft violations; hard invariants 0)
drift: 0 field(s) across 0 class(es)
```

`Status::missionCompleted` row: `pct 86.7`, `osize 1432`, `usize 1432`,
`byte false`, `linked false`.

ELF sections (rank table present):

```
[16] .missioncase PROGBITS 0020b977 20b977 0000c0 00 A 0 0 1
[27] .shstrtab    STRTAB   00000000 215af0 000124 00   0 0 1
28 section headers
```

The original has **27** section headers, no `.missioncase`, and
`.shstrtab` size `0x117`.

## 2. What the invented table is, and why it is wrong

`kMissionCase[type]` is a type -> case-label (rank) permutation. The switch then
dispatches on the rank, not on the mission type. The original
`Status::missionCompleted` (VMA `0xabf38`, size 1432) switches directly on
`m->getType()` and uses two in-text `tbh` tables:

* `0xabfe6`: 40 halfwords for types 150..189 (`sub r1, r0, #150; cmp r1, #39`);
* `0xac052`: 7 halfwords for types 8..14 (`sub r1, r0, #8; cmp r1, #6`).

Decoding the original tables (script in §7) gives the type -> body mapping.
Comparing it with the rank table's type -> rank -> body mapping shows six
semantic mismatches:

| type | original body | rank-table body | correct? |
|---:|---|---|:---:|
| 161 | default (no body) | blueprint-ingredient body (rank 161) | **no** |
| 162 | blueprint-ingredient body | production-good body (rank 162) | **no** |
| 166 | production-good body | `field_178 >= statusValue` body (rank 174) | **no** |
| 168 | `field_178 >= statusValue` body | `shortMission` body (rank 176) | **no** |
| 169 | `shortMission` body | `dockedAtStation && target` body (rank 177) | **no** |
| 171 | `dockedAtStation && target` body | `dockedAtStation && target` body (rank 177) | yes |

(The rank table maps both 169 and 171 to rank 177, so type 169 executes the
wrong body. Types 0/8/11/13/14/150..160/163..165/170/172/184/189 are correct.)

The table is also not present in the original `.rodata` (the exact 192-byte
sequence and its 190-byte prefix both `find()` as `-1`; see the w3490 note).
The real `.rodata` at `0x1fb974` is `_SHIP_HANGAR_OFFSETS[16..63]`, the tail of
the 64-int CutScene copy at `0x1fb934`.

## 3. Experiment A — remove the table, direct `(short)` switch

Patch (`src/game/mission/Status.cpp`):

* delete the `#pragma clang section rodata=".missioncase"` pair and the
  `static const unsigned char kMissionCase[192]` definition;
* change `switch ((short) kMissionCase[m->getType()])` to
  `switch ((short) m->getType())`;
* relabel the cases with the actual mission types in the existing source order:
  `167/174, 8, 0/11, 13, 14, 150, 151, 152, 153, 154, 155, 158, 162, 166, 170,
  184, 189, 156, 157, 160, 163, 164, 165, 168, 169, 171, 172`.

Measured:

```
verify: byte 4496  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓
drift: 0 field(s) across 0 class(es)
```

| metric | baseline (rank table) | direct `(short)` switch | original |
|---|---:|---:|---:|
| `byte_exact` | 4496 | **4496** | — |
| `linked_exact` | 4496 | **4496** | — |
| `.text` | 0x1337c8 | **0x1337c8** | 0x1337c8 |
| `.rodata` differing bytes | 0 | **0** | 0 |
| `.missioncase` | 0xc0 | **absent** | absent |
| section headers | 28 | **27** | 27 |
| `.shstrtab` | 0x124 | **0x117** | 0x117 |
| `missionCompleted` pct | 86.7 | **57.7** | — |
| `missionCompleted` size | 1432 | **1432** | 1432 |

The full section-table diff against the original after the change is only the
known `.ARM.extab` delta:

```
< [14] .ARM.extab PROGBITS 001a3b78 1a3b78 026298 ...
> [14] .ARM.extab PROGBITS 001a3b78 1a3b78 026290 ...
```

All other section names, types, addresses, offsets, sizes, flags and alignments
match.

## 4. Experiment B — direct `int` switch (negative control)

Changing only the cast to `switch (m->getType())` (no `(short)`) keeps the
correct type labels but emits a 1428-byte body (`usize 1428`, `osize 1432`) and
fails the ratchet:

```
verify: byte 3043  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
FAIL  byte_exact 4496 -> 3043
RATCHET FAIL
```

`missionCompleted` pct is 60.2% in this form. The original has no `sxth` before
`sub #150`, so the `(short)` cast is not faithful codegen; it is the minimum
source change that keeps the function size (and therefore the whole `.text`
layout) exact.

## 5. Experiment C — cast variants

All rows are full `verify` builds with the direct labels; `pct` is the
`Status::missionCompleted` row.

| switch expression | pct | osize | usize | gate |
|---|---:|---:|---:|---|
| `(short) m->getType()` | 57.7 | 1432 | 1432 | **PASS** |
| `m->getType()` | 60.2 | 1432 | 1428 | FAIL (byte 3043) |
| `(unsigned) m->getType()` | 60.2 | 1432 | 1428 | FAIL |
| `(unsigned short) m->getType()` | 55.3 | 1432 | 1436 | FAIL |
| `(char) m->getType()` | 55.3 | 1432 | 1436 | FAIL |
| `(unsigned char) m->getType()` | 55.3 | 1432 | 1436 | FAIL |
| `(signed char) m->getType()` | 12.7 | 1432 | 380 | FAIL |

The rank table is the only tested form that reaches 86.7%, because the rank
values are dense and ascending in source order, so clang's ascending jump-table
block order coincides with the original's source-order body layout. With type
labels clang 7 lays the 150..189 bodies out in ascending type order, which is
not the original order.

### Note on the w3490 report

`_work/reconstructions/kMissionCase_real_table_w3490.md` records 86.7% for the
direct `(short)` switch. That number is stale: the root `report.json` at
`1cb0c638` (direct switch) still contains the previous rank-table row (86.7%),
and the report at `8b634df3` (rank table) still contains the direct-switch row
(57.7%). Rebuilding both forms in this worktree gives 57.7% for the direct
switch and 86.7% for the rank table.

## 6. Proven vs hypothesis

**Proven (commands in §7):**

1. The original `Status::missionCompleted` switches directly on `m->getType()`
   with two in-text `tbh` tables (types 150..189 and 8..14); it has no byte
   table load and no `.missioncase` section.
2. The invented `kMissionCase[192]` is not in the original `.rodata` and its
   type -> body mapping is wrong for types 161, 162, 166, 168 and 169.
3. Removing the table and switching on `(short) m->getType()` with the real
   type labels keeps `byte_exact 4496` / `linked_exact 4496`, `.text 0x1337c8`,
   `.rodata` 0 differing bytes, lint `CLEAN`, drift 0, and removes the
   `.missioncase` section (28 -> 27 sections; `.shstrtab` 0x124 -> 0x117).
4. The direct `int` switch emits a 1428-byte body and fails the ratchet
   (`byte_exact 3043`).

**Hypothesis (not proven):**

* The direct-switch 57.7% vs rank-table 86.7% difference is clang 7's
  `MachineBlockPlacement` ordering the jump-table blocks by ascending case
  value; the original's source/codegen produced source-order blocks. The
  original's exact source shape (or any profile/expect hints) is not recovered.
* The `(short)` cast is a size/layout compensation for that ordering, not
  original source. A faithful cast-free form that is still 1432 bytes would
  need a different clang-7 lowering trick.

## 7. Reproduction commands

```sh
cd /home/debian/GalaxyOnFire2/wt/w3570

# baseline gate
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint -j8
cmake --build cmake-build-match --target drift -j8

# section table (baseline vs direct vs original)
OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-readelf
$OD -SW cmake-build-match/libgof2hdaa.so | grep -E "missioncase|shstrtab"
$OD -SW ../_work/bins/android_2.0.16_libgof2hdaa.so | grep -E "missioncase|shstrtab"

# .rodata content diff (VMA == file offset for .rodata)
python3 - <<'PY'
o=open('../_work/bins/android_2.0.16_libgof2hdaa.so','rb').read()
u=open('cmake-build-match/libgof2hdaa.so','rb').read()
base=0x1c9e10; size=0x41b67
a=o[base:base+size]; b=u[base:base+size]
d=[i for i in range(size) if a[i]!=b[i]]
print(len(d), hex(base+d[0]) if d else None, hex(base+d[-1]) if d else None)
PY

# the invented 192-byte table is not in the original; the real 0x1fb974 data
# is the tail of the 64-int CutScene copy at 0x1fb934 (four copies total)
python3 - <<'PY'
import struct
o=open('../_work/bins/android_2.0.16_libgof2hdaa.so','rb').read()
tbl=[0,190,190,190,190,190,190,190,8,190,190,11,190,13,14,190,
190,190,190,190,190,190,190,190,190,190,190,190,190,190,190,190,
190,190,190,190,190,190,190,190,190,190,190,190,190,190,190,190,
190,190,190,190,190,190,190,190,190,190,190,190,190,190,190,190,
190,190,190,190,190,190,190,190,190,190,190,190,190,190,190,190,
190,190,190,190,190,190,190,190,190,190,190,190,190,190,190,190,
190,190,190,190,190,190,190,190,190,190,190,190,190,190,190,190,
190,190,190,190,190,190,190,190,190,190,190,190,190,190,190,190,
190,190,190,190,190,190,190,190,190,190,190,190,190,190,190,190,
190,190,190,190,190,190,152,155,154,156,157,159,167,168,160,190,
169,161,162,171,172,173,174,163,176,177,150,177,182,190,151,190,
190,190,190,190,190,190,190,190,165,190,190,190,190,166,190,190]
b=bytes(tbl)
print("192-byte table find:", o.find(b))
print("190-byte prefix find:", o.find(b[:190]))
real=[160,150,220,310,160,180,140,170,170,110,250,205,240,250,250,250,
     170,170,190,460,170,210,140,170,160,190,170,240,220,280,200,200,
     140,160,180,150,310,390,480,200,300,300,170,200,170,370,370,300,
     440,340,170,470,170,170,170,170,200,170,250,150,170,250,270,170]
rb=struct.pack('<64I',*real)
i=o.find(rb)
while i!=-1:
    print("64-int table copy:", hex(i)); i=o.find(rb,i+1)
PY

# decode the original missionCompleted tbh tables
python3 - <<'PY'
import struct
d=open('../_work/bins/android_2.0.16_libgof2hdaa.so','rb').read()
def decode(base,n,start):
    pc=base
    for i in range(n):
        e=struct.unpack_from('<H',d,base+2*i)[0]
        print(f"type {start+i:3d} -> 0x{pc+2*e:x}")
decode(0xabfe6,40,150)
decode(0xac052,7,8)
PY

# decode the direct-switch tbh tables (addresses shift by +2 from the sxth)
python3 - <<'PY'
import struct
d=open('cmake-build-match/libgof2hdaa.so','rb').read()
def decode(base,n,start):
    pc=base
    for i in range(n):
        e=struct.unpack_from('<H',d,base+2*i)[0]
        print(f"type {start+i:3d} -> 0x{pc+2*e:x}")
decode(0xabfe8,40,150)
decode(0xac054,7,8)
PY

# per-function row
python3 - <<'PY'
import json
r=json.load(open('cmake-build-match/verify/report.json'))
print([e for e in r['rows'] if 'missionCompleted' in e['name']])
PY
```
