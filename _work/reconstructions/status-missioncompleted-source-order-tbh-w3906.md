# `Status::missionCompleted` — source-order TBH sweep (w3906)

Status: research note (no source change landed). Worktree `wt/w3906`, branch
`wave89-w3906`, base master `36c65db7`. Original binary:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b clang
7.0.2 `r328903`, LLVM `1d739ffb…`, `-Oz`, image base 0x10000; real VMA =
Ghidra − 0x10000).

Topic: the faithful direct `switch ((short) m->getType())` is **57.7 % /
1432 B**; the invented `.missioncase` rank table is **86.7 % / 1432 B** but
adds a section. This pass re-derived the original dispatch from the ELF,
corrected a misreading in the w3906 handoff, and swept plain-switch forms.
**No plain switch reproduces the original's source-order bodies at 1432 B.**
The closest plain form is a sequential two-switch shape that reproduces the
body order exactly but is **1508 B / 53.2 %**.

## 0. Baseline (measured in this worktree)

```sh
cmake --build cmake-build-match --target check -j8
```

```
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`report.json` row: `_ZN6Status16missionCompletedEbbx` → `pct 57.7`,
`osize 1432`, `usize 1432`. `.text 0x1337c8`, `.rodata 0x41b67`,
27 sections, `.shstrtab 0x117` (byte-identical), lint `CLEAN ✓`, drift 0.

## 1. Original dispatch — the table is *value* order, the bodies are *source* order

`Status::missionCompleted` is at `0xabf38`, size 1432. It calls
`Mission::getType()` exactly once and dispatches with two in-text TBH tables:

```
abfd6: blx  Mission::getType()
abfda: sub.w r1, r0, #150
abfde: cmp   r1, #39
abfe0: bhi.n ac046
abfe2: tbh   [pc, r1, lsl #1]      @ 40 halfwords at 0xabfe6 (types 150..189)
...
ac046: sub.w r1, r0, #8
ac04a: cmp   r1, #6
ac04c: bhi.n ac0be
ac04e: tbh   [pc, r1, lsl #1]      @ 7 halfwords at 0xac052 (types 8..14)
```

There is **no `sxth`** and no rank/byte-table load. The TBH PC is
`0xabfe2 + 4 = 0xabfe6`; `target = 0xabfe6 + 2*entry`. Decoding the 40
halfwords at `0xabfe6`:

```
idx type  entry  target    idx type  entry  target
  0  150  0x0095 0xac110    20  170  0x0160 0xac2a6
  1  151  0x0098 0xac116    21  171  0x020d 0xac400
  2  152  0x009b 0xac11c    22  172  0x021a 0xac41a
  3  153  0x00b5 0xac150    23  173  0x023a 0xac45a (default)
  4  154  0x00b8 0xac156    24  174  0x0028 0xac036 (167/174 body)
  5  155  0x00c4 0xac16e    25..33 175..183  0x023a 0xac45a (default)
  6  156  0x01a2 0xac32a    34  184  0x0167 0xac2b4 (campaign body)
  7  157  0x01b7 0xac354    35..38 185..188 0x023a 0xac45a (default)
  8  158  0x00cd 0xac180    39  189  0x0184 0xac2ee
  9  159  0x023a 0xac45a
 10  160  0x01d1 0xac388
 11  161  0x023a 0xac45a
 12  162  0x00fb 0xac1dc
 13  163  0x01e8 0xac3b6
 14  164  0x01f6 0xac3d2
 15  165  0x01f8 0xac3d6
 16  166  0x012e 0xac242
 17  167  0x0028 0xac036 (same body as 174)
 18  168  0x01fc 0xac3de
 19  169  0x020b 0xac3fc
```

So `entry[i]` is the body for **type 150+i**: the table is in ascending type
(value) order, exactly as clang's `buildJumpTable` emits it. The *bodies* in
memory are in a different order:

```
0xac036 167/174
0xac110 150   0xac116 151   0xac11c 152   0xac150 153
0xac156 154   0xac16e 155   0xac180 158   0xac1dc 162
0xac242 166   0xac2a6 170   0xac2b4 184   0xac2ee 189
0xac32a 156   0xac354 157   0xac388 160   0xac3b6 163
0xac3d2 164   0xac3d6 165   0xac3de 168   0xac3fc 169
0xac400 171   0xac41a 172   0xac45a default
```

i.e. `167/174, 8, 11, 13, 14, 150, 151, 152, 153, 154, 155, 158, 162, 166,
170, 184, 189, 156, 157, 160, 163, 164, 165, 168, 169, 171, 172, default` —
exactly the order of the `case` labels in the current `src/game/mission/Status.cpp`.

**Correction to the w3906 handoff.** The handoff's "key finding" said the
original's *table entries* are in source order (`entry[0]=167/174,
entry[1]=150, …`). That is a misreading: `entry[0]=0x0095` targets
`0xabfe6 + 0x12a = 0xac110`, which is body **150**, not 167/174. The table is
value order; it is the *body layout* that is source order. `entry[17]` (and
`entry[24]`) is the peeled `167/174` body (`0x0028`).

## 2. Why the direct switch lays the bodies out in value order

LLVM 7 (`SelectionDAGBuilder.cpp`, local copy in `/dev/shm/w3906`):

* `sortAndRangeify()` sorts `CaseCluster`s by signed case value
  (`a.Low->getValue().slt(b.Low->getValue())`).
* `buildJumpTable()` builds `Table` from the sorted clusters and adds the
  jump-table MBB's successors in **table order**
  (`// Add successors. Note: use table order for determinism.`).
* `MachineBlockPlacement::selectBestSuccessor` / `selectBestCandidateBlock`
  walk that successor list; all case bodies have equal frequency (4.10 %;
  `167/174` is 8.19 % because two values share a body, and `default` is
  1.71 %), so ties are broken by successor order = ascending type.

The MBP dump for the current build confirms the successor list:

```
%jump-table.1: %bb.36 %bb.38 %bb.39 %bb.44 %bb.45 %bb.46 %bb.77 %bb.80
               %bb.47 %bb.103 %bb.85 %bb.103 %bb.57 %bb.88 %bb.91 %bb.92
               %bb.62 %bb.16 %bb.93 %bb.96 %bb.66 %bb.97 %bb.98 %bb.103
               %bb.16 %bb.103 ... %bb.67 ... %bb.71
# 150,151,152,153,154,155,156,157,158,default,160,default,162,163,...
```

and the final body order is ascending type:
`167/174 150 151 152 153 154 155 156 157 158 160 162 163 164 165 166 168 169
170 171 172 default 184 189`.

Reordering the source case blocks does not change this: the handoff measured
the same value order for the current order, ascending order, reversed order,
and group-reordered variants. `-mllvm -arm-adjust-jump-tables=true/false`,
`-switch-peel-threshold=0..100`, `-disable-block-placement`,
`-block-placement-exit-block-bias=100`, `-force-precise-rotation-cost`, and
the other MBP/switch flags tried in w3590/w3632 all leave value order (or
reverse value order), never source order.

## 3. Variant sweep (compile-only layout; official verifier for scored rows)

Layout parser: compile `Status.cpp` with the project's exact flags, decode the
`.LJTI` tables (`target = base + 2*entry`), print the bodies in address order.

| source form | size | high-table body order | score |
|---|---:|---|---:|
| baseline `switch ((short) m->getType())` | 1432 | value order | **57.7** |
| `switch (m->getType())` (int) | 1428 | value order | 60.2 |
| `(unsigned short)` cast | 1432 | value order (TBH shifted) | — |
| `case 0:` split from `case 11:` | 1432 | value order | — |
| explicit `default:` at each position | 1432 | value order | — |
| if/else-if chain on `int type` | 1428 | value order (2 TBHs) | — |
| if/else-if + `__builtin_expect` every arm | 1348 | `167/174 default 150…172 184 189` | — |
| nested `default: switch(...)` | 1428 | value order | — |
| two-level `if (type >= 150) switch … else switch` | 1380 | 3 TBHs, 2 `getType` calls | — |
| **sequential two-switch** (special cases, then misc cases) | **1508** | **exact original order** | **53.2** |
| two-switch, second switch re-calls `getType()` | 1468 | `189 167/174 150…` | — |
| rank table `kMissionCase[192]` (`.missioncase`) | 1432 | exact original order | **86.7** |

### 3.1 The sequential two-switch form is the only plain form with the exact order

Splitting the one switch into two sequential switches — first the "special"
cases (`167/174, 8, 11, 13, 14, 150..155, 158, 162, 166, 170, 184, 189`), then
the "misc" cases (`156,157,160,163,164,165,168,169,171,172`) — makes clang
merge both switches into a single 40-entry TBH whose bodies are laid out in
the original order:

```
TBH 0xaa base 0xae
  order: 167/174 150 151 152 153 154 155 158 162 166 170 184 189 156 157
         160 163 164 165 168 169 171 172 default
  table: 0x206 0x20c 0x212 0x244 0x24a 0x262 0x43c 0x466 0x274 0x56c
         0x4a0 0x56c 0x2ce 0x4ce 0x4ea 0x4ee 0x332 0xfe 0x4f6 0x50e
         0x39a 0x512 0x52c 0x56c 0xfe 0x56c ... 0x3a8 ... 0x3e2
TBH 0x11c base 0x120
  order: 8 11 13 14 9/10/12
```

This is structurally the original table (same 40-entry span, same
`167/174`/`184`/`189` positions, same default holes). But the function is
**1508 B**, not 1432 B: the two switches keep `type` live in `fp`, which
displaces `i` from `fp` to `r8` and `atStation` from `r8` to `r9`, and the
changed register allocation makes the low bodies and the peeled body larger.
Measured with the official verifier: **53.2 % / 1508 B** (the body order
matches, but every body differs by register names), and the 76-byte growth
shifts the rest of `.text` (`byte_exact 4498 -> 2326`).

### 3.2 The rank table still passes the gate on this base

On the current base the `.missioncase` rank-table form scores **86.7 % /
1432 B** and the full gate is **PASS** (`byte 4498, linked 4498, data 843,
imports 207`). It differs from the baseline only in the extra section:

```
baseline:      27 sections, .shstrtab 0x117
rank table:    28 sections, .shstrtab 0x124
               [16] .missioncase PROGBITS 0x20b977 size 0xc0
```

The `.missioncase` section is placed in the gap between `.rodata`
(ends 0x20b977) and `.fini_array` (0x20cba0), so no existing section address
moves and `byte_exact` stays 4498. Putting the same 192-byte table in
`.rodata` (or `.data`) instead drops `byte_exact` to 4422 because the table
lands in the middle of the concatenated data sections and shifts every
PC-relative literal-pool reference after it. The separate section is
load-bearing for the gate, but it is exactly the `.shstrtab` regression that
commit `6e3fd629` removed the table to fix.

## 4. Conclusion

**Proven:**

1. The original's high TBH is indexed by `type-150` in ascending type order;
   its *bodies* are laid out in source order
   (`167/174, 8, 11, 13, 14, 150..155, 158, 162, 166, 170, 184, 189,
   156, 157, 160, 163, 164, 165, 168, 169, 171, 172, default`).
2. The current direct `(short)` switch is 1432 B / 57.7 % and lays the bodies
   out in ascending type order because clang's jump-table successor list is
   value-sorted and MBP follows it.
3. No plain single switch, if/else ladder, case reordering, default
   placement, or tested MBP/switch flag produces the source-order bodies at
   1432 B.
4. A sequential two-switch form *does* produce the exact original body order
   with a 40-entry TBH (no helper table), but is 1508 B / 53.2 %.
5. The `.missioncase` rank table is 86.7 % / 1432 B and still passes the gate
   on this base, at the cost of a 28th section (`.shstrtab 0x124`).

**Remaining:** the original's source-order body layout is a
`MachineBlockPlacement` artifact that a plain single switch does not
reproduce under clang 7.0.2 `-Oz`. Reproducing it without a helper table
needs a source shape that keeps the high bodies' IR order (or their
frequencies) aligned with source order — the two-switch form is the only
shape found, and its register pressure costs 76 B. The 13.3 % residual of
the rank-table form is within-body code shape (register allocation,
scheduling, short/wide branch encodings), not the dispatch.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w3906
cmake --build cmake-build-match --target check -j8   # RATCHET PASS, byte 4498 linked 4498
cmake --build cmake-build-match --target lint -j8    # CLEAN
cmake --build cmake-build-match --target drift -j8   # 0 fields

OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -d --start-address=0xabf38 --stop-address=0xac4d0 \
    ../_work/bins/android_2.0.16_libgof2hdaa.so

# decode the original high table (target = 0xabfe6 + 2*entry)
python3 - <<'PY'
import struct
d=open('../_work/bins/android_2.0.16_libgof2hdaa.so','rb').read()
# .text file offset 0x69440, VMA 0x69440
def hw(v): return struct.unpack_from('<H',d,v)[0]
base=0xabfe6
for i in range(40):
    e=hw(base+2*i)
    print(150+i, hex(base+2*e), hex(e))
PY
```

Scratch (not committed): `/dev/shm/w3906/` — `Status.cpp.base`,
`Status_2sw.cpp`, `Status_rank.cpp`, `layout.py`, `decode2.py`, `vfn_rank.txt`.
