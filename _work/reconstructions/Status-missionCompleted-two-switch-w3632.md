# `Status::missionCompleted` — the two-switch shape (w3632)

Status: research note (2026-11-02). Worktree `wt/w3632`, branch `wave87-w3632`,
based on master `9b607dd6`. Original binary:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b clang 7.0.2
`r328903`, LLVM `1d739ffb…`, `-Oz`, image base 0x10000; real VMA = Ghidra − 0x10000).

Topic: does the original `Status::missionCompleted(bool,bool,long long)` have a
**two-switch** source shape, and can that shape reproduce its block order / size?

**Answer: no.** The original is a *single* `switch (m->getType())` with two
in-text TBH clusters (40 + 7 entries). A two-source-switch shape reproduces the
original's case-body order exactly, but it compiles to **three** jump tables and
**1508 B** (original 1432 B), so it is not the original source and is not a
strict gate improvement. The body order is a `MachineBlockPlacement` artifact,
as concluded in `Status-missionCompleted-no-rank-table-w3590.md`.

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
.ARM.exidx   0x6f70   @ 0x19cc08
.ARM.extab   0x26290  @ 0x1a3b78
.rodata      0x41b67  @ 0x1c9e10
.data.rel.ro 0x2fe8   @ 0x20cbb0
.data        0x296c   @ 0x214000
.bss         0x69ac   @ 0x216970
```

`report.json` row: `_ZN6Status16missionCompletedEbbx` → `osize 1432`,
`usize 1432`, `pct 57.7`, `byte false`, `linked false`.

## 1. Original dispatch — one call, two TBH tables, no `sxth`

```sh
OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -d --start-address=0xabf38 --stop-address=0xac4d0 \
    ../_work/bins/android_2.0.16_libgof2hdaa.so > /tmp/mc_orig.asm
grep -nE 'getType|tbh|tbb|sxth' /tmp/mc_orig.asm
```

```
abfd6: blx  Mission::getType()          <-- exactly one call
abfda: sub.w r1, r0, #150
abfde: cmp   r1, #39
abfe0: bhi.n ac046
abfe2: tbh   [pc, r1, lsl #1]           <-- table 1: 40 halfwords @ 0xabfe6
...
ac046: sub.w r1, r0, #8
ac04a: cmp   r1, #6
ac04c: bhi.n ac0be
ac04e: tbh   [pc, r1, lsl #1]           <-- table 2: 7 halfwords @ 0xac052
```

There is **no `sxth`** and no byte-table load. Both tables are indexed by the
single `getType()` result (`r0`): table 1 is `type-150` (types 150..189),
table 2 is `type-8` (types 8..14). Type 0 is handled by a compare at `ac0be`
that targets the same body as type 11 (`ac0c4`). The function contains exactly
two TBH encodings (`e8df f011` = `tbh [pc,r1,lsl#1]`), verified by scanning
`0xabf38..0xac4d0`.

## 2. Original case-body order (binary evidence)

Decoding the two tables (`target = table_base + 2*entry`,
`table_base = tbh_addr + 4`) and sorting the targets gives:

```
167/174  8  11  13  14  150 151 152 153 154 155 158 162 166 170 184 189
156 157 160 163 164 165 168 169 171 172  default
```

This is exactly the order of the `case` labels in the current
`src/game/mission/Status.cpp` (the w3570 source order), with `case 0` sharing
the `case 11` body.

Self-contained decoder (reads the ELF directly; `.text` VMA == file offset
`0x69440` for this binary):

```sh
python3 - <<'PY'
import struct
d=open('../_work/bins/android_2.0.16_libgof2hdaa.so','rb').read()
def decode(table_vma,n,first_case,pc_base):
    return [(first_case+i, pc_base+2*struct.unpack_from('<H',d,table_vma+2*i)[0])
            for i in range(n)]
hi=decode(0xabfe6,40,150,0xabfe6)
lo=decode(0xac052,7,8,0xac052)
allc=sorted(hi+lo,key=lambda x:(x[1],x[0]))
groups=[];last=None
for cv,tgt in allc:
    if tgt!=last: groups.append([]);last=tgt
    groups[-1].append(cv)
print(' '.join('/'.join(map(str,g)) for g in groups))
PY
# 167/174 8 11 13 14 150 151 152 153 154 155 158 162 166 170 184 189 156 157 160 163 164 165 168 169 171 172 9/10/12/159/161/173/175/176/177/178/179/180/181/182/183/185/186/187/188
```

(The trailing `9/10/12/…` group is the shared default/continue target.)

## 3. Current single-switch shape — value order, not source order

The current source is `switch ((short) m->getType())` (w3570). Compile-only
body order (object `.text._ZN6Status16missionCompletedEbbx`, TBH targets
sorted):

```
167/174 8 11 13 14 150 151 152 153 154 155 156 157 158 160 162 163 164 165
166 168 169 170 171 172 default 184 189
```

i.e. **ascending type value**, not the original source order. This is the
57.7% near-miss already documented in
`Status-missionCompleted-no-rank-table-w3590.md`; the `(short)` cast adds the
`sxth` and restores the 1432 B size (the direct `int` switch is 1428 B).

The LLVM IR switch case order is source order (verified with
`-S -emit-llvm`: `167,174,8,0,11,13,14,150,…,189,156,…,172`), but
`SelectionDAGBuilder::sortAndRangeify` sorts the clusters by value and
`buildJumpTable` adds the jump-table successors in table order, so
`MachineBlockPlacement` produces value order.

## 4. Two-source-switch experiment

If the source is split into two sequential switches on a cached local, the
compiler lays out each switch's bodies in its own value order. Splitting after
`case 189` — first switch = all cases except the late group
`{156,157,160,163,164,165,168,169,171,172}`, second switch = the late group —
gives exactly the original body order:

```cpp
int mtype = m->getType();
switch (mtype) { /* 167,174,8,0,11,13,14,150..155,158,162,166,170,184,189 */ }
switch (mtype) { /* 156,157,160,163,164,165,168,169,171,172 */ }
```

Measured (compile-only, exact project flags):

| shape | size | jump tables | body order |
|---|---:|---:|---|
| current single `(short)` switch | 1432 | 2 (40 + 7) | value order |
| current single `int` switch | 1428 | 2 (40 + 7) | value order |
| two switches, split after 189 (`int mtype`) | **1508** | **3 (40 + 7 + 17)** | **original order** |
| two switches, `short mtype` | 1512 | 3 | original order |
| first switch + if/else chain for late group | 1508 | 3 | original order |
| natural high/low two switches (`high` then `low`) | 1404 | 3 | high value order, then low |
| natural high/low two switches (`low` then `high`) | 1424 | 3 | 167/174, 11, high value order, low |
| nested switch in `default` (early outer / late inner) | 1428 | 2 | value order |
| nested switch in `default` (high outer / low inner) | 1376 | 2 | value order |

The split-after-189 shape is the only one that reproduces the original body
order, but it emits **three** tables: the first switch's 40-entry high TBH,
its 7-entry low TBH, and a redundant 17-entry TBB for the second switch. The
compiler *merges* the second switch's cases into the first high table (the
first high table's entries for 156,157,160,163,164,165,168,169,171,172 point
directly at the late bodies), but still emits the second dispatch/table. The
extra dispatch + table makes the function **76 B larger** (1508 vs 1432), so
it cannot be the original source.

First high table of the split-after-189 build (targets, sorted by address):

```
167/174 150 151 152 153 154 155 158 162 166 170 184 189
156 157 160 163 164 165 168 169 171 172 default
```

i.e. the merge preserves the original source order in the high table, but the
redundant second TBB (156..172) is still there. The original has only the
40-entry high table and the 7-entry low table, so the second switch is not in
the original source.

### Full gate A/B (split-after-189, `int mtype`, two switches)

```sh
cp src/game/mission/Status.cpp /tmp/Status.cpp.orig
# apply the split-after-189 shape (see diff below)
cmake --build cmake-build-match --target check -j8
```

```
verify: byte 2326  linked 4495  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
  WARN  linked_exact 4496 -> 4495 (transient band; must recover next link, revert if persistent)
  FAIL  byte_exact 4496 -> 2326
RATCHET FAIL
```

`.text` grows `0x1337c8 -> 0x133828` (+0x60), `.rodata` base moves
`0x1c9e10 -> 0x1c9e70`, `.ARM.exidx` base moves `0x19cc08 -> 0x19cc68`.
`report.json`: `osize 1432`, `usize 1508`, `pct 53.2` (worse than 57.7%).

The source diff is exactly:

```diff
-        switch ((short) m->getType()) {
+        int mtype = m->getType();
+        switch (mtype) {
@@
                 break;
+        }
+        switch (mtype) {
             case 156:
```

### Exhaustive split sweep (compile-only)

All 28 group boundaries × 2 orders were compiled with the project flags
(`/tmp/mc_compile.sh` is the `Status.cpp` command from
`compile_commands.json`). Sizes ranged **1368..1548 B**; only split-after-189
in the `ab` order produced the original body order (verified with a raw-byte
TBH/TBB decoder that reads the object section directly, so TBB entries are
decoded as bytes, not halfwords). No split produced 1432 B with the original
order. The nearest-size two-switch shapes (1368/1372 B) are all value
order.

## 5. Direct `int` switch full gate (control)

```sh
sed -i 's/switch ((short) m->getType())/switch (m->getType())/' src/game/mission/Status.cpp
cmake --build cmake-build-match --target check -j8
```

```
verify: byte 3043  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
  FAIL  byte_exact 4496 -> 3043
RATCHET FAIL
```

`.text` stays `0x1337c8` (alignment absorbs the 4-byte shrink);
`report.json`: `usize 1428`, `pct 60.2`. This reproduces the w3590 note.

## 6. Conclusion

**Proven (commands above):**

1. The original `Status::missionCompleted` calls `Mission::getType()` **once**,
   has **no `sxth`**, and contains exactly **two** TBH tables: 40 entries
   (types 150..189) and 7 entries (types 8..14). It is a single switch with two
   jump-table clusters.
2. Its case-body order is source order:
   `167/174, 8, 11, 13, 14, 150..155, 158, 162, 166, 170, 184, 189,
   156, 157, 160, 163, 164, 165, 168, 169, 171, 172, default`.
3. The current single `(short)` switch is 1432 B / 57.7% and lays bodies in
   ascending value order; the direct `int` switch is 1428 B / 60.2%.
4. A two-source-switch shape (split after `case 189`) reproduces the original
   body order, but compiles to **three** jump tables and **1508 B**; the full
   gate is byte 2326 / linked 4495 (RATCHET FAIL). It is not the original
   source and not a strict improvement.
5. No two-switch split (28 boundaries × 2 orders) gives 1432 B with the
   original body order. Natural high/low and nested-switch shapes all give
   value order.

**Hypothesis (not proven):**

* The original's source-order block layout is a `MachineBlockPlacement` result
  that our reconstruction does not reproduce under the fixed clang 7.0.2
  `-Oz` flags. The two-switch shape is a *block-order oracle* (it proves the
  original order is "early group, then late group"), not the original source.
  The residual is the block order and the short/wide branch encodings it
  causes; it is not a missing table or a missing call.

## 7. Reproduction commands

```sh
cd /home/debian/GalaxyOnFire2/wt/w3632
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8

# original dispatch / body order
python3 /tmp/mc_orig_tables.py          # see §2 for the inline version

# compile-only body-order probe: take the Status.cpp command from
# cmake-build-match/compile_commands.json, add -ffunction-sections -funwind-tables,
# compile to an object, then decode the TBH/TBB tables in
# .text._ZN6Status16missionCompletedEbbx (target = tbh_addr + 4 + 2*entry).
# A compact decoder (objdump supplies base/count; raw section bytes supply the
# table, so TBB entries are read as bytes and TBH entries as halfwords):
python3 - "$OBJ" <<'PY'
import re,subprocess,sys,struct
obj=sys.argv[1]
OD='.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump'
out=subprocess.run(['readelf','-SW',obj],capture_output=True,text=True).stdout
off=[int(l.split()[4],16) for l in out.splitlines()
     if '.text._ZN6Status16missionCompletedEbbx' in l and 'PROGBITS' in l
     and 'ARM.extab' not in l][0]
d=open(obj,'rb').read()
txt=subprocess.run([OD,'-d','-j','.text._ZN6Status16missionCompletedEbbx',obj],
                   capture_output=True,text=True).stdout
ins=[]
for l in txt.splitlines():
    m=re.match(r'\s*([0-9a-f]+):\s+([0-9a-f ]+)\t(.*)',l)
    if m: ins.append((int(m.group(1),16),m.group(3).strip()))
ent=[]
for i,(a,t) in enumerate(ins):
    if not (t.startswith('tbh') or t.startswith('tbb')): continue
    base=cnt=None
    for j in range(i-1,max(-1,i-10),-1):
        mm=re.search(r'subs?(?:\.w)?\s+\w+,\s*(?:\w+,\s*)?#(\d+)',ins[j][1])
        if mm and base is None: base=int(mm.group(1))
        mm=re.search(r'cmp\s+\w+,\s*#(\d+)',ins[j][1])
        if mm and cnt is None: cnt=int(mm.group(1))+1
        if base is not None and cnt is not None: break
    pc=a+4
    for k in range(cnt):
        v=(struct.unpack_from('<H',d,off+pc+2*k)[0] if t.startswith('tbh')
           else d[off+pc+k])
        ent.append((base+k,pc+2*v))
ent.sort(key=lambda x:(x[1],x[0]))
g=[];last=None
for cv,tgt in ent:
    if tgt!=last: g.append([]);last=tgt
    g[-1].append(cv)
print(' '.join('/'.join(map(str,x)) for x in g))
PY

# full-gate A/B: apply the two-switch diff from §4, then
cmake --build cmake-build-match --target check -j8
# revert:
git checkout -- src/game/mission/Status.cpp
cmake --build cmake-build-match --target check -j8
```
