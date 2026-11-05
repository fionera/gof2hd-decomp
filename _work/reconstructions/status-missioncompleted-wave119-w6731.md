# `Status::missionCompleted` — wave 119, angle C: pool placement is the last blocker (w6731)

Status: **research note (no source change landed)**.  Worktree `wt/w6731`,
branch `wave119-w6731`, base master `fcd88b45b`.  Original binary:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b clang
7.0.2, `-Oz`, image base 0x10000).

Target: `_ZN6Status16missionCompletedEbbx` (`Status::missionCompleted(bool,
bool, long long)`), original `0xabf38`, size **1432 B**.  Baseline row:
**92.5 % / 1432 B** (byte False, linked False, 523/524 insns).  Gate after this
pass: `RATCHET PASS`, `byte_exact 4503`, `linked_exact 4503`, imports 207,
`lint CLEAN`, `drift 0`, 27 sections, exact file size 2,187,328 B.

No variant beat the committed wave-117 rank table.  The note records the
measurements, the exactdisp2 structural candidate, and the literal-pool
analysis that explains why the three remaining body-size deltas cannot be
removed independently of the table.

## 0. Baseline

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
# verify: byte 4503  linked 4503  stubs 0  missing 0  extra 0  avg 99.93
# data:   exact 843 of 843 exported objects
# sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
# RATCHET PASS
```

`report.json` row: `_ZN6Status16missionCompletedEbbx` → `pct 92.5`,
`osize 1432`, `usize 1432`, `byte False`, `linked False`, `523/524`.

## 1. Exact body map (original vs baseline)

The rank table reproduces the original body order exactly:

```
shared 167/174, low group 0/8/11/13/14,
150 151 152 153 154 155 158 162 166 170 184 189 156 157 160 163 164 165
168 169 171 172, default
```

The emitted body addresses differ only by the three known size deltas:

| body | original | baseline | delta | cause |
|---|---:|---:|---:|---|
| 166 | 100 B | 96 B | −4 | source reuses `index`; the faithful re-query is 104 B |
| 168 | 30 B | 26 B | −4 | original carries a `nop` + the `Globals::status` GOT word; ours carries one `uxth` |
| 189 | 60 B | 68 B | +8 | the rank-table address word and the `Globals::status` GOT word land in this body |

Cumulative: bodies `170/184/189` are 4 B early, bodies `156..168` are 4 B
late, and `169..default` match exactly.  The deltas cancel at the loop latch
(`0x522`), which is why the function size stays 1432 B.

## 2. Literal-pool analysis (new this wave)

The original function has four PC-relative literal loads:

```
orig 0xac068 ldr r0,[pc,#908] -> 0xac3f8   (GOT Globals::status, 16-bit ldr)
orig 0xac288 ldr r0,[pc,#568] -> 0xac4c4
orig 0xac2c0 ldr r0,[pc,#520] -> 0xac4cc
orig 0xac3de ldr r0,[pc,#232] -> 0xac4c8
```

The baseline has five:

```
ours 0xabfda ldr r1,[pc,#804] -> 0xac300   (MISSION_CASE table address)
ours 0xac068 ldr r0,[pc,#664] -> 0xac304   (GOT Globals::status)
ours 0xac28a ldr r0,[pc,#568] -> 0xac4c4
ours 0xac2bc ldr r0,[pc,#520] -> 0xac4c8
ours 0xac3e2 ldr r0,[pc,#232] -> 0xac4cc
```

The table word at `0xac300` and the GOT word at `0xac304` are adjacent, inside
body 189 (which starts at `0xac2ea`).  In the original the GOT word is at
`0xac3f8`, inside body 168, and there is no table word.

Why the table word lands at body 189: the dispatch `ldr` is a 16-bit
`tLDRpci` (max forward offset 1020 B), and the end-of-function pool
(`0xac4c4`) is 1058 B away, out of range.  `ARMConstantIslands` therefore
moves the table word to the highest in-range water block, which is the
unconditional branch at the end of body 189 (`0xac300`).  The GOT word is
processed next and reuses that new island (`0xac304`).  Without the table word
the GOT word would take its natural highest in-range water, body 168
(`0xac3f8`), exactly as in the original.

Consequence: a table-free form would fix body 168 and body 189 but changes the
body layout to value order (see §3); the rank table cannot be removed without
losing the source-order layout.

## 3. Variants measured this wave

All built with the project's exact flags and scored with the official verifier
unless marked otherwise.

| variant | pct | usize | note |
|---|---:|---:|---|
| **committed wave-117 rank table** | **92.5** | **1432** | baseline, 523/524 |
| `exactdisp2` (type-labelled switch, table maps types to 150..189) | 87.6 | 1432 | 523/523 insns; exact `sub #150` dispatch shape; TBH 4 B late, body offsets differ |
| `exactdisp2` + body-166 re-query | 73.8 | 1440 | |
| `exactdisp2` + drop body-168 `uxth` | 73.8 | 1432 | |
| `exactdisp2` + both | 73.6 | 1440 | |
| plain switch, original source order | 58.4 | 1424 | exact dispatch, value-order bodies |
| plain switch + body-166 re-query | 59.4 | 1432 | |
| `goto`-label switch (value-order cases, source-order labels) | 50.4 | 1356 | compiler merged case blocks with labels; dispatch `sub #151; cmp #38` |
| GOT-relative table (`extern const unsigned char kMissionCase[192]`) | 70.9 | 1436 | dispatch gains a load; `.dynsym` risk |
| fixed-address table via local pointer | 92.5 | 1432 | identical codegen |
| `(long long)`/`unsigned long long` index forms | 92.5 | 1432 | identical codegen |
| `volatile int z = 0; MISSION_CASE[type + z]` | 54.6 | 1484 | |
| body-166 re-query (`hasEquipment(m->getProductionGoodIndex(), 1)`) | 75.8 | 1440 | +8 B, latch moves `0x522`→`0x52a` |
| body-166 re-query via local `index2` | 74.7 | 1440 | |
| body-166 re-query both index and amount | 75.2 | 1448 | |
| body-168 `unsigned`/`int`/`unsigned short` local forms | 73.7–91.3 | 1432 | baseline 92.5 still best |
| body-168 drop `(unsigned short)` | 74.9 | 1432 | pool placement moves |
| body-158 `while` loop | 92.5 | 1432 | identical codegen |
| body-158 nested/continue/`&`/`bool both`/`if(!(a&&b))` | 90.9–92.5 | 1432 | identical or worse; scheduler swap unfixed |
| body-158 `int`/`unsigned char` flags | 59.8 | 1472 | |
| source case moved to end (each of 22 cases) | 63.0–92.5 | 1420–1432 | pool words stay at `0xac300/0xac304` |
| source case moved to front (each of 22 cases) | 44.9–92.5 | 1368–1432 | pool words stay at `0xac300/0xac304` |
| swap equal-size bodies 156/157 | 71.9 | 1432 | body code swap hurts alignment |
| swap equal-size bodies 150/151 | 92.1 | 1432 | |
| swap 156↔158 | 63.7 | 1436 | |

The source-case reorderings confirm that the pool placement follows the
**layout** order (address), not the MBB/source order: moving a case in the
source does not move the table/GOT words.

## 4. Body-158 scheduler swap (re-confirmed)

The only remaining size-neutral in-body mismatch is the two-instruction swap
at the end of body 158:

```
orig: and.w r0,r9,r6 ; ldr r6,[sp,#16] ; mov r8,r4 ; lsls r0,r0,#31 ; beq
ours: and.w r0,r9,r6 ; mov r8,r4 ; lsls r0,r0,#31 ; ldr r6,[sp,#16] ; beq
```

Every source rewrite tried this wave (`while`, nested `if`, `continue`,
`bool both`, `if (!(hasGood && hasSpecial))`, `if (hasGood & hasSpecial)`,
`if (hasGood) { if (hasSpecial) ... }`, `if (...) return m; break;`) produced
byte-identical code to the baseline.  The swap is a machine-scheduler
decision, not a source-shape decision.

## 5. Conclusion — proven vs open

**Proven (built and measured):**

1. The committed rank table remains the best 1432 B form: **92.5 %**.
2. `exactdisp2` is the most structurally faithful candidate (exact
   `sub #150` dispatch, 523/523 insns) but scores **87.6 %** because its TBH
   is 4 B late and every body offset is shifted; the fuzzy metric prefers the
   tuned rank-table layout.
3. The table word and the `Globals::status` GOT word are adjacent inside body
   189.  The table word is forced there by the 16-bit `tLDRpci` range
   (end-of-function pool is 1058 B away, limit 1020 B).  Removing the table
   word would let the GOT word take its natural body-168 slot, exactly as in
   the original.
4. The three body-size deltas (166 −4, 168 −4, 189 +8) cancel at the latch;
   fixing 166 alone shifts every later body and costs ~17 fuzzy points.
5. Source-case reordering does not move the pool words; the placement follows
   the layout order.
6. Body 158's scheduler swap is not reachable from the tested source forms.

**Open:** a real source shape that makes `MachineBlockPlacement` lay the
plain-switch bodies out in source order (so the rank table and its 4-byte
literal word can be deleted), or a way to make the dispatch's table-address
load a 32-bit `t2LDRpci` so the table word stays at the end.  Either would
make body 168 and body 189 match the original and leave only body 166's
4-byte re-query delta.

## 6. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6   # RATCHET PASS, byte 4503
cmake --build cmake-build-match --target lint  -j6   # CLEAN
cmake --build cmake-build-match --target drift -j6   # 0 fields
```

Scratch (not committed): `/dev/shm/w6731/` — `Status.base.cpp`,
`table.base.cpp`, `Status.exactdisp2.cpp`, `table_exactdisp2.cpp`,
`Status.goto.cpp`, `Status.gottable.cpp`, `Status.requery2.cpp`,
`Status.move_{front,end}_*.cpp`, `checkpool.sh`, `gen_move.py`,
`gen_rank.py`, `layout.py`, `fast.py`, and the LLVM 7.0.2 build with
`MBP_TRACE`/`CP_TRACE` (`/tmp/llvm_android_702b/build2/bin/llc`).
