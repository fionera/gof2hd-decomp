# `Status::missionCompleted` — remove the `.missioncase` rank table:
# direct `switch (m->getType())`, 28 -> 27 sections (wm101, wave 102)

Status: **landed pass**. Worktree `wt/wm101`, branch `wave102-wm101`, base
master `53d9ee4d5`. Original binary:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b clang
7.0.2 `r328903`, `-Oz`, image base 0x10000; real VMA = Ghidra − 0x10000).
Target: `_ZN6Status16missionCompletedEbbx`
(`Status::missionCompleted(bool, bool, long long)`), original `0xabf38`,
size **1432 (0x598)**.

## 0. Gate (before -> after)

```
cmake --build cmake-build-match --target check -j8
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`byte_exact`, `linked_exact`, `size-exact` (1432 B), `data_exact`, `imports`,
lint and drift are all unchanged. The target's fuzzy score moves
**87.0 -> 60.4** (`osize 1432`, `usize 1432`). The win is structural: the
extra `.missioncase` section is gone, the section count is **28 -> 27**
(identical to the original), the file size is **2,187,328 B exactly** (was
192+ B larger), and `.shstrtab` is **byte-identical** (`0x117`, 0 differing
bytes; was `0x124`, 86 differing bytes). Every section header now matches the
original except the known `.ARM.extab` size residual (`0x2628c` vs `0x26298`,
-12).

## 1. The original dispatch is TBH in `.text`, not a `.rodata` TBB

The mission brief said "the original's jump table is in `.rodata` (4-byte
offsets)". That is **not** what the shipped artifact contains. The original
function has exactly two `tbh` instructions and no `tbb`:

```
abfd4  bl    Mission::getType()
abfda  sub.w r1, r0, #150
abfde  cmp   r1, #39
abfe0  bhi.n ac046
abfe2  tbh   [pc, r1, lsl #1]     ; 40 halfwords @ 0xabfe6, types 150..189
...
ac046  sub.w r1, r0, #8
ac04a  cmp   r1, #6
ac04c  bhi.n ac0be
ac04e  tbh   [pc, r1, lsl #1]     ;  7 halfwords @ 0xac052, types 8..14
ac0be  cmp   r0, #0               ; case 0 falls through to the 0/11 body
ac0c0  bne.w ac45a
```

Both tables are inline halfword offsets in `.text`; a scan of the whole
`.rodata` (VMA 0x1c9e10, size 0x41b67) finds **0** 4-byte words and **0**
halfword-derived targets pointing into `0xabf38..0xac4d0`. `.rodata` is
byte-identical and contains no rank table either (a search for the committed
192-byte `kMissionCase` pattern and for 16 consecutive `190`s finds nothing).

Commands:

```sh
OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -d --start-address=0xabf38 --stop-address=0xac4d0 \
    /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
# decode the 40 halfwords at 0xabfe6: target = 0xabfe6 + 2*entry
```

## 2. Decoded original tables (proven)

High table (`0xabfe6`, 40 entries, types 150..189), target addresses:

```
150 ac110  151 ac116  152 ac11c  153 ac150  154 ac156  155 ac16e
156 ac32a  157 ac354  158 ac180  159 ac45a  160 ac388  161 ac45a
162 ac1dc  163 ac3b6  164 ac3d2  165 ac3d6  166 ac242  167 ac036
168 ac3de  169 ac3fc  170 ac2a6  171 ac400  172 ac41a  173 ac45a
174 ac036  175..183 ac45a  184 ac2b4  185..188 ac45a  189 ac2ee
```

Low table (`0xac052`, 7 entries, types 8..14): `8 -> ac060`, `11 -> ac0c4`,
`13 -> ac0ce`, `14 -> ac0dc`, `9/10/12 -> ac45a` (default). Type `0` is the
`cmp r0,#0` fall-through to `ac0c4`, i.e. it shares the 0/11 body.

`ac45a` is the loop latch (`add fp, fp, #1; b abf82`), so `159`, `161`,
`173`, `175..183`, `185..188` are plain `break`/default cases.

Physical body order (ascending address):

```
167/174, 8, 0/11, 13, 14,
150,151,152,153,154,155,158,162,166,170,184,189,
156,157,160,163,164,165,168,169,171,172, default
```

This is exactly the `case` order the original source must have used (the
committed rank table's ranks are a bijection onto it).

## 3. Why the direct switch does not reproduce the body order (proven)

`SelectionDAGBuilder::buildJumpTable` (LLVM 7.0.2, `SDB_7.0.2.cpp:9180`)
adds the jump-table MBB's successors in `Table` order; `Table` is built from
`Clusters`, which `sortAndRangeify` sorts by case value. So
`MBB->successors()` is **value-sorted**. `MachineBlockPlacement::
selectBestSuccessor` (`MBP_7.0.2.cpp:1463`) walks `collectViableSuccessors`
(which preserves `successors()` order) and keeps the **first** maximum
probability (`BestProb >= SuccProb` skips ties). The case bodies all have
equal frequency (4.10%; 167/174 is 8.19% because two values share it), so
the layout is: 167/174 first, then ascending value order.

The original is instead in **source order** (group A then group B, section
2). The committed `kMissionCase[192]` rank table works by making the
*value* order equal the original body order; the switch is then
`switch (kMissionCase[m->getType()])` and clang's value-sorted successors
land in source order. It is a codegen device, not original data.

Measured with the real NDK clang (full link + official scorer,
`/dev/shm/wm101/measure.sh`):

| variant | size | pct |
|---|---:|---:|
| committed rank table + `.missioncase` (HEAD) | 1432 | **87.0** |
| rank table, same table in `.rodata` (no pragma) | 1432 | 87.0 (byte_exact **4422**) |
| direct switch, plain case-166 (no null check) | 1428 | 59.5 |
| direct switch, case-166 `s==0 -> continue` | 1428 | 60.4 |
| direct switch, case-166 `s==0 -> return m` | **1432** | **60.4** |
| direct switch, source reordered to original body order + `s==0 -> return m` | **1432** | **60.4** |
| direct switch, `(short) m->getType()` | 1432 | 57.8 |
| direct switch, `if (ship != 0 && ...)` | 1428 | 60.4 |
| direct switch, `while` loop rewrite | 1456 | 35.2 |
| if/else-if chain + `goto sw_end` (converted to a switch by SimplifyCFG) | 1432 | 60.4 |

The rank table in `.rodata` (no `#pragma clang section`) still scores 87.0
for the function but shifts every later `.rodata` literal-pool reference and
drops `byte_exact` to **4422** — the separate section is load-bearing.

## 4. The landed change

`src/game/mission/Status.cpp`:

* delete `kMissionCase[192]`, the two `#pragma clang section` lines and the
  rank-table comment block;
* `switch (kMissionCase[m->getType()])` -> `switch (m->getType())`;
* relabel every `case <rank>:` to the real mission type and reorder the
  bodies to the original physical order (section 2);
* keep a 4-byte size anchor in `case 166`:
  `Ship *s = ship; if (s == 0) return m;`. The original has no null check
  here; the plain form is 1428 B (size-exact would drop to 4523/4524). The
  anchor splits the body into two MBBs and restores the original's
  `(index r9, amount r5, ship r0)` allocation. Documented in the source.

The dispatch is byte-identical to the original: same `sub/cmp/bhi/tbh`
sequence, same 40-entry high table and 7-entry low table, same
`cmp r0,#0` case-0 fall-through, same 1432-byte size.

## 5. Proven vs hypothesis

**Proven (command-backed):**

* The original's dispatch is two inline TBH tables in `.text` (40 + 7
  entries) plus a `cmp r0,#0`; there is no `.rodata` jump table and no rank
  table in the original.
* The original body order is `167/174, 8, 0/11, 13, 14, 150,151,152,153,
  154,155,158,162,166,170,184,189, 156,157,160,163,164,165,168,169,171,172`
  (section 2).
* clang 7's jump-table successors are value-sorted and MBP keeps the first
  maximum-frequency candidate, so a direct switch on `getType()` lays the
  bodies out in ascending value order (verified with
  `llc -print-after=block-placement` and `-debug-only=block-placement` on
  the exact project IR).
* The direct switch keeps `byte_exact 4500 / linked_exact 4500 /
  size-exact 4524`, drops the section count 28 -> 27, makes the file size
  2,187,328 B exactly, and makes `.shstrtab` byte-identical.
* Putting the same rank table in `.rodata` drops `byte_exact` to 4422.

**Hypothesis / open:**

* The original source's case order is the physical order in section 2 (the
  rank table's bijection is consistent with it, but the source itself is not
  observable).
* The original's MBP produced source order rather than value order. With
  vanilla NDK r18b clang 7.0.2 the value order is what a direct switch
  produces; a patched oracle with `MBP_SORT_SBCB` (sort the placement
  worklist by MBB number) reproduces the original order but the real
  compiler does not sort. A source shape that makes vanilla clang take the
  source-order path (e.g. an `if/else` chain that `SimplifyCFG` converts to
  a switch with different block frequencies) was not found: the if/else
  variant above still lays out in value order. Recovering 87.0 -> 100
  without the rank table therefore needs either a different source shape or
  a compiler-behaviour explanation; it is not a source-only switch rewrite.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/wm101
cmake --build cmake-build-match --target check -j8      # RATCHET PASS
OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -d --start-address=0xabf38 --stop-address=0xac4d0 \
    /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so --objdump $OD --top 0
```

Scratch (not committed): `/dev/shm/wm101/` — `measure.sh` (link + score one
function), `mv.sh` (copy variant, build, score, restore), `genll.sh`
(exact-flag `.ll`), `scoreo.py`/`linkwith.sh` (link a custom `Status.o`),
`Status.final.cpp` (landed source), `Status.direct_plain.cpp`,
`Status.direct_reorder_null.cpp`, `Status.rank_rodata.cpp`,
`Status.ifelse.cpp`.
