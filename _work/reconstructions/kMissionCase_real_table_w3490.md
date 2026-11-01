# `kMissionCase` and the real table at `0x1fb974` (w3490)

Status: research note + landed source fix. Branch `wave87-w3490`, based on current
master `34b3b162`. Original binary: `../_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000; real VMA = Ghidra - 0x10000).

## TL;DR

* The real `.rodata` table at **`0x1fb974` is `_SHIP_HANGAR_OFFSETS[16..63]`** — the
  48-int tail of the 64-int CutScene copy of `SHIP_HANGAR_OFFSETS` (base
  `0x1fb934`). It is not a mission-case table.
* The current tree's invented `kMissionCase[192]` in `CutScene.cpp` is laid
  exactly over that tail. It differs from the original in **191 of 192 bytes**
  (`0x1fb974..0x1fba33`) and also breaks `CutScene::replacePlayerShip` for ship
  indices >= 16 at runtime.
* `kMissionCase` is not present anywhere in the original `.rodata`: the exact
  192-byte table and its 190-byte prefix both `find()` as `-1`, and no 190-byte
  window in the original `.rodata` satisfies the required type->rank grouping.
  The original `Status::missionCompleted` uses in-text `tbh` tables, not a byte
  load.
* Restoring `CUTSCENE_SHIP_HEIGHT[64]` and switching directly on
  `(short) m->getType()` (with the actual mission-type case labels) removes the
  fake table and makes `.rodata` **byte-identical again (0 differing bytes)**.
  Gate is unchanged: **byte_exact 4494, linked_exact 4496**; `.text` stays
  `0x1337c8`, `missionCompleted` stays 1432 B / 86.7%, lint `CLEAN`, drift 0.
* A direct `int` switch (no `(short)` cast) is still a ratchet failure
  (byte_exact 3043), so the signed-narrow cast is retained.

The task brief's baseline (`byte 4182`, `linked 4495`) is stale; the measured
baseline in this worktree is **4494 / 4496**.

## 1. Baseline

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint -j8
cmake --build cmake-build-match --target drift -j8
```

```
verify: byte 4494  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓ (no new soft violations; hard invariants 0)
drift: 0 field(s) across 0 class(es)
```

`.rodata` (same base/size in both files, `0x41b67 @ 0x1c9e10`):

```
differing bytes: 191
runs:            2
first:           0x1fb974
last:            0x1fba33
```

The baseline `missionCompleted` row is `osize 1432 / usize 1432 / 86.7%`,
`byte false`, `linked false`.

## 2. What is at `0x1fb974` in the original

The original `.rodata` at `0x1fb934` is a 64-int table:

```
160,150,220,310,160,180,140,170,170,110,250,205,240,250,250,250,
170,170,190,460,170,210,140,170,160,190,170,240,220,280,200,200,
140,160,180,150,310,390,480,200,300,300,170,200,170,370,370,300,
440,340,170,470,170,170,170,170,200,170,250,150,170,250,270,170
```

`0x1fb974` is exactly `0x1fb934 + 16*4`, i.e. entry 16 of that table. The
original has four copies of the 64-int sequence:

```sh
python3 - <<'PY'
import struct
o=open('../_work/bins/android_2.0.16_libgof2hdaa.so','rb').read()
tbl=[160,150,220,310,160,180,140,170,170,110,250,205,240,250,250,250,
     170,170,190,460,170,210,140,170,160,190,170,240,220,280,200,200,
     140,160,180,150,310,390,480,200,300,300,170,200,170,370,370,300,
     440,340,170,470,170,170,170,170,200,170,250,150,170,250,270,170]
b=struct.pack('<64I',*tbl)
i=o.find(b)
while i!=-1:
    print(hex(i)); i=o.find(b,i+1)
PY
# 0x1fb934 0x1fba48 0x1fd578 0x1fe130
```

The source-layout dump names this symbol `_SHIP_HANGAR_OFFSETS` in
`Classes/Game/CutScene.cpp`, `Level.cpp`, `PlayerEgo.cpp` and
`PlayerFighter.cpp` (`_work/original_layout/original_layout.json`,
`tu_symbols.gof2hd_1.1.19_ios`). So the four copies are the per-TU copies of the
same source table; the CutScene copy is the one at `0x1fb934`.

The CutScene consumer is `CutScene::replacePlayerShip`. Its original disassembly
loads the table base `0x1fb934` and indexes it by `shipIndex`:

```
99714: ldr r1, [pc, #108]      ; literal at 0x99784 = 0x162218
99718: add r1, pc              ; pc = 0x9971c -> r1 = 0x1fb934
9971a: add.w r1, r1, r8, lsl #2
9971e: vldr s0, [r1]
```

`shipIndex` comes from `Globals::status->getShip()->getIndex()` (see
`ModStation.cpp` call sites), i.e. a ship type 0..63, so the whole 64-entry
table is live. The current `CUTSCENE_SHIP_HEIGHT[16]` + `kMissionCase[192]`
split therefore returns wrong heights for ship types >= 16.

## 3. Is `kMissionCase` in the original?

No. Three independent checks:

1. The exact 192-byte table currently in `CutScene.cpp` does not occur in the
   original binary; neither does its 190-byte prefix.
2. No 190-byte window anywhere in the original `.rodata` satisfies the
   type->rank grouping required by the original `missionCompleted` body order
   (script in §7; 0 hits).
3. The original `Status::missionCompleted` has no `ldrb` table load. It switches
   directly on `m->getType()` with two in-text `tbh` tables (40 entries for
   types 150..189, 7 entries for types 8..14). The body order decoded from the
   first table is `167/174, 150, 151, 152, 153, 154, 155, 158, 162, 166, 170,
   184, 189, 156, 157, 160, 163, 164, 165, 168, 169, 171, 172, default`.

So `kMissionCase` is a reconstruction device (a rank table that makes clang's
ascending case-value lowering reproduce the original body order), not recovered
original data.

## 4. Controlled experiments

All rows are full `cmake --build cmake-build-match --target check -j8` builds;
no `--update-baseline`.

| # | change | byte | linked | `.rodata` diff | `.text` | `missionCompleted` | verdict |
|---|---|---:|---:|---:|---|---|---|
| 0 | baseline (`kMissionCase[192]` overlay) | 4494 | 4496 | 191 B (`0x1fb974..0x1fba33`) | `0x1337c8` | 1432 B / 86.7% | — |
| A | restore `CUTSCENE_SHIP_HEIGHT[64]`, remove `kMissionCase`, direct `(short) m->getType()` switch with type labels | **4494** | **4496** | **0 B** | `0x1337c8` | 1432 B / 86.7% | **landed** |
| B | A but direct `int` switch (drop `(short)`) | 3043 | 4496 | 0 B | `0x1337c8` | 1432 B / 86.7% | RATCHET FAIL |
| C | A plus remove the unused `kMissionOrder[190]` anchor | 4494 | 4496 | 0 B | `0x1337c8` | 1432 B / 86.7% | neutral (kept) |

Experiment A details:

* `src/game/core/CutScene.cpp`: `CUTSCENE_SHIP_HEIGHT[16]` -> `[64]` with the
  original 64 values; delete the `kMissionCase[192]` definition and its
  `#pragma GCC visibility` pair.
* `src/game/mission/Status.cpp`: delete the `extern const unsigned char
  kMissionCase[190]` declaration and the unused `kMissionOrder[190]` anchor;
  change `switch ((short) kMissionCase[m->getType()])` to
  `switch ((short) m->getType())` and relabel the cases with the actual mission
  types (rank -> type inverse of the old table: `163->167`, `151->174`,
  `152->150`, `155->151`, `154->152`, `156->153`, `157->154`, `159->155`,
  `160->158`, `161->162`, `162->166`, `150->170`, `165->184`, `166->189`,
  `167->156`, `168->157`, `169->160`, `171->163`, `172->164`, `173->165`,
  `174->168`, `176->169`, `177->171`, `182->172`; `0/8/11/13/14` unchanged).

Measured after A:

```
verify: byte 4494  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓
drift: 0 field(s) across 0 class(es)
```

All ELF sections remain size- and base-exact:

```
.text        0x1337c8 @ 0x69440
.rodata      0x41b67  @ 0x1c9e10
.data.rel.ro 0x2fe8   @ 0x20cbb0
.data        0x296c   @ 0x214000
.bss         0x69ac   @ 0x216970
```

Experiment B is the important negative control: removing the `(short)` cast
keeps the function and `.text` sizes exact but still fails the ratchet
(`byte_exact 4494 -> 3043`) because the direct `int` switch lays the case bodies
out in ascending type order and shifts local `.text` sections. The signed-narrow
cast is still required.

## 5. Conclusion

The real table at `0x1fb974` is the second half of the CutScene copy of
`_SHIP_HANGAR_OFFSETS`. The invented `kMissionCase[192]` table had been
overlaid on it to keep `.rodata` size exact while forcing a rank-table switch.
Restoring the real 64-int table and switching directly on the mission type
removes the overlay, makes `.rodata` byte-identical (191 -> 0 differing bytes),
and fixes the runtime ship-height lookup for ship types 16..63, with no gate
movement.

## 6. Proven vs hypothesis

**Proven (commands in §7):**

1. `0x1fb974` is `SHIP_HANGAR_OFFSETS[16..63]`; the original has four copies of
   the 64-int table at `0x1fb934`, `0x1fba48`, `0x1fd578`, `0x1fe130`.
2. `CutScene::replacePlayerShip` loads the table base `0x1fb934` and indexes it
   by `shipIndex` (`add.w r1, r1, r8, lsl #2; vldr s0, [r1]`).
3. The current `kMissionCase[192]` is not in the original binary and no 190-byte
   window in the original `.rodata` is a valid rank table for the original
   `missionCompleted` body order.
4. The original `missionCompleted` uses two in-text `tbh` tables and no byte
   table load.
5. Experiment A restores `.rodata` to 0 differing bytes with byte_exact 4494 /
   linked_exact 4496, `.text` `0x1337c8`, lint clean, drift 0.
6. Experiment B (direct `int` switch) fails the ratchet at byte_exact 3043.

**Hypothesis (not proven):**

* The original `CutScene.cpp` source declared `SHIP_HANGAR_OFFSETS[64]` and used
  it from `replacePlayerShip`; the 64-entry size is inferred from the four
  identical 64-int copies and the ship-type index range, not from a size-bearing
  symbol dump (the iOS dumps list only the symbol name).
* The remaining `missionCompleted` 86.7% delta is the `sxth` inserted by the
  `(short)` cast plus the body order; recovering a cast-free 1432-byte form
  would need a different clang-7 lowering trick.

## 7. Reproduction commands

```sh
cd /home/debian/GalaxyOnFire2/wt/w3490

# baseline gate
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint -j8
cmake --build cmake-build-match --target drift -j8

# .rodata diff (both files use VMA == file offset for .rodata)
python3 - <<'PY'
o=open('../_work/bins/android_2.0.16_libgof2hdaa.so','rb').read()
u=open('cmake-build-match/libgof2hdaa.so','rb').read()
base=0x1c9e10; size=0x41b67
a=o[base:base+size]; b=u[base:base+size]
d=[i for i in range(size) if a[i]!=b[i]]
print(len(d), hex(base+d[0]) if d else None, hex(base+d[-1]) if d else None)
PY

# identify the 0x1fb934 table and its four copies
python3 - <<'PY'
import struct
o=open('../_work/bins/android_2.0.16_libgof2hdaa.so','rb').read()
tbl=[160,150,220,310,160,180,140,170,170,110,250,205,240,250,250,250,
     170,170,190,460,170,210,140,170,160,190,170,240,220,280,200,200,
     140,160,180,150,310,390,480,200,300,300,170,200,170,370,370,300,
     440,340,170,470,170,170,170,170,200,170,250,150,170,250,270,170]
b=struct.pack('<64I',*tbl)
i=o.find(b)
while i!=-1:
    print(hex(i)); i=o.find(b,i+1)
PY

# CutScene::replacePlayerShip table-base calculation
python3 - <<'PY'
d=open('../_work/bins/android_2.0.16_libgof2hdaa.so','rb').read()
lit=0x99784
val=int.from_bytes(d[lit:lit+4],'little')
pc=(0x99718+4)&~3
print(hex(val), hex(pc), hex(pc+val))   # 0x162218 0x9971c 0x1fb934
PY

# no 190-byte rank-table window in the original .rodata
python3 - <<'PY'
d=open('../_work/bins/android_2.0.16_libgof2hdaa.so','rb').read()
base=0x1c9e10; size=0x41b67
ro=d[base:base+size]
groups=[[167,174],[150],[151],[152],[153],[154],[155],[158],[162],[166],
        [170],[184],[189],[156],[157],[160],[163],[164],[165],[168],[169],
        [171],[172],[0,11],[8],[13],[14]]
exp={}
for i,g in enumerate(groups):
    for t in g: exp[t]=i
def check(win):
    vals={}
    for t,i in exp.items():
        vals.setdefault(i,set()).add(win[t])
    if any(len(s)!=1 for s in vals.values()): return False
    seq=[next(iter(vals[i])) for i in range(27)]
    return all(seq[i]<seq[i+1] for i in range(26))
print(sum(check(ro[off:off+190]) for off in range(size-190+1)))
PY
```
