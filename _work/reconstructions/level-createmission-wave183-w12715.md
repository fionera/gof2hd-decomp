# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 183 / w12715

Target: `src/game/world/Level.cpp`, function at VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave183-w12715`, base master `9555474d8` (wave-182 harvest 5).
Ground truth `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base `0x10000`).

Scope: **branch layout / EH-LSDA / mission-type switch**.  Nothing from wave 181/182
is re-landed; every experiment below is on top of the wave-182 tree (raw 3630).

## 1. Result

**No size-exact `.text` raw win was found.**  `src/` is left exactly at the base
(no code change).  This note is the deliverable.  The base is a strong local
optimum for the raw metric: an exhaustive independent adjacent-statement swap
sweep over the whole body (155 candidates) and over the switch half (83
candidates) produced **no** variant with raw < 3630.

```text
                                     size            raw    aligned  insn
base 9555474d8                       17172/17172     3630   1127     6206/6197
```

Section bytes at the base (re-measured in this worktree):

```text
.text        size 1,259,464 / 1,259,464   differing bytes 75,015
.ARM.extab   size   156,312 /   156,312   differing bytes    646
.ARM.exidx   size    28,528 /    28,528   differing bytes      0
.rel.plt     size    29,392 /    29,392   byte-identical (diff 0)
.rodata/.data/.bss/.got/.dynsym/.dynstr/.hash/.rel.dyn   exact
.note.gnu.build-id 20
```

Gate at the base: `byte_exact 4507/4524`, `linked_exact 4507/4524`,
`size_exact 4524/4524`, `stub_zero_size 0`, `missing 0`, `wrong_type 0`,
`extra 0`, `data_exact 843/843`, imports **207**, lint **CLEAN ✓**, drift **0**,
symbol parity **5370/5370**, 27/27 headers, file size 2,187,328 exact,
`RATCHET PASS`.

## 2. Mission-type switch: TBH table and case-body sizes

`switch (mission->getType())` lowers to `subs r1,r0,#1; cmp r1,#14;
bhi .L183_184; tbh [pc,r1,lsl#1]`.  The dispatch itself is byte-identical; the
15-entry TBH table at `0xb02ac` differs in exactly three entries:

```text
type   original   ours     delta
 1     0xb14b2    0xb14b6   +4
 2     0xb16d4    0xb16d6   +2
 4     0xb19c0    0xb19be   -2
```

Decoded case-body extents (entry-to-entry):

```text
body            original   ours     delta
case1 (1->2)    0x222      0x220    -2
case2 (2->4)    0x2ec      0x2e8    -4
case4 (4->next) ...        ...      +2
```

The deltas are downstream of code-size differences inside case1/case2 (a missing
2-byte `nop` before case1's literal pool, an extra `ldr r5,[sp,#116]` in ours,
and the case2 convoy-loop schedule).  The case1/case2 literal pools are 4 bytes
apart (`0xb159c` orig vs `0xb15a0` ours), so every `ldr [pc,#imm]` in case1
differs by 4 in the immediate.

### 2a. `switch (k)` convoy-offset block order (case 183)

The original emits the four convoy-offset blocks in **source order**
`case0, case1, case2, default`; ours emits `case0, case2, case1, default`.
The emitted per-case store order is also different:

```text
              original emitted      ours emitted
case0         offset1,0,2            offset0,2,1
case1         offset1,0,2            offset2,1,0
case2         offset1,0,2            offset1,0,2
default       offset1,0,2            offset0,2,1
```

The original shares the third store (`offset[2] -> [sp,#380]`) in one common
tail block; ours emits it inline in each case, which is why ours is larger.
The current source store orders are `case0=201, case1=120, case2=012,
default=201` (wave-181/182 optimum for raw).

Measured variants (all `link_so` + `tools/fn_diff.py`):

```text
variant                                      raw   aligned  size
base (201/120/012/201)                       3630   1127    17172
all cases source order 102 (offset1,0,2)     11316*  1219    17164  *collapses to table lookup, .rodata +32
case0 102 only                               3634   1126    17172
case1 102 only                               3653   1124    17172
case2 102 only                               3658   1139    17172
default 102 only                             3641   1125    17172
case0 210 only                               3633   1123    17172
case1 210 only (== base binary)              3630   1127    17172
default 210 only                             3635   1127    17172
```

Source-order permutations of the four `case` blocks: **any** permutation that
keeps `case 0` first produces the identical binary (raw 3630); permutations that
put `case 1` first measure raw 5366, `case 2` first raw 5363.  The block
placement is value-driven, not source-order-driven.

All 16 combinations with `offset[2]` assigned last in every case are
size-fatal (17180 / 17188 / 17164), so the tail-merge hypothesis cannot be
landed size-exact.

## 3. Branch layout: raid/security block swap

First structural difference in the function is at `0xb04b2`: the `raid = false`
cold block and the `security == 0 -> 90` threshold block are emitted in the
opposite order (original: `raid=false` first; ours: `security==0` first).  It is
a 2-instruction block swap of equal size and does not shift the rest of the
function.  Six source spellings were measured and all produce the **identical**
binary (raw 3630, size 17172):

```text
if/else (base); inverted if/else; bool raid=false + if(!intro||hardCore);
ternary; && form; empty-then if/else; separate raidThreshold local
```

`__builtin_expect` on the `introSystem && !hardCoreMode()` condition is
size-fatal (17208 / 17204).  `__builtin_expect(raidRoll < (...), 0|1)` is
canonicalised away (raw 3630).  `__builtin_expect(security==0, 0)` is 17180;
`security==0,1` is 17312.

## 4. Alien-turret loop (largest single diff region, 364 B)

The loop body at `0xb32bc` is the largest diff region.  The original selects
`t` with a **branch** (`cbz r0; ldr r1,[pc]; add r1,pc; b; ldr r1,[pc]; add r1,pc`),
ours with a branchless `it ne / movne`.  The source hack
`(bigAlienWave + 2 * (i < 0)) ? ... : ...` is what supplies the extra 16-byte
stack slot that keeps the frame size exact.

```text
variant                                   raw    aligned  size
base (hack, branchless select)            3630   1127     17172
remove `(i<0)` (plain bool ternary)       5360   1141     17156  (16 B short)
if/else with `(i<0)`                      3630   1127     17172  (identical binary)
init-to-outpost + if(bigAlienWave)        5360   1141     17156
bigAlienWave == 1 / !bigAlienWave         5360   1141     17156
hoist loop end into a local               6494   1640     17188
hoist loop index `i` into a local         3630   1127     17172  (identical)
k-driven loop (`kk`, i = start + kk)      6567   1636     17212
```

The original's loop register map is `this=r4, gun=r6, i=fp, k*24=r9, t=sl,
temp=r5, rotation/v=r4`; ours is `this=fp, gun=r4, i=[sp,#120], k*24=r6, t=r5,
temp=r9, rotation/v=fp`.  `i` is spilled in ours but kept in `fp` in the
original.  No source spelling tried moves `i` into `fp` while staying size-exact.

## 5. EH / LSDA

`createMission`'s exidx entry is `@0x1a9ac4` in both binaries; the generic
personality routine is `0x5eb60`.  The LSDA content differs in **125 bytes**
(first diff at LSDA offset 19: original `a6 07`, ours `a5 07` — a 1-byte
call-site landing-pad offset).  The landing-pad block at the end of `.text`
(`0xb4200..0xb42c4`) is a run of 2-byte `b.n` stubs into the cleanup blocks at
`0xb42a4/0xb42b6/0xb42c0/0xb42ca`; the original has ~67 stubs, ours ~65, in a
different order.  The order follows the source order of the throwing `new`
calls, so matching it requires matching the allocation order of the whole
function — not attempted here (no size-exact raw win available from the
individual reorders measured).

## 6. Other measured reorders (all size-exact unless noted)

```text
variant                                        raw   aligned  extab
base                                           3630   1127     646
case1 attackerCount before `new Array`         3639   1105     645
pirateBase after createStaticObject            3632   1096     646
case2 setAlwaysFriend before getPosition       3635   1117     646
case2 setSpeed/setRotationSpeed swap           3634   1128     646
alien cargo/v swap (reverse of w12615 §2b)     3631    943     646
c1_reorder + pirate reorder                    3641   1074     645
c1_reorder + cargo/v swap                      3640    921     646
c1_reorder + pirate + cargo/v                  3642    890     646
c1_reorder + setAlwaysFriend                   3644   1095     645
```

The aligned metric improves substantially in several variants (best 890), but
every one of them raises the raw byte count, so none is landable under the
"raw win only" rule.  `c1_reorder` is the only variant that also improves
`.ARM.extab` (645 vs 646), but its raw +9 makes the total worse.

`bool`->`int`/`unsigned` conversions in the case1/case2 loops are codegen-neutral
(raw 3630).  `case 1`'s `attackerCount` reorder is the only change that makes
the emitted arithmetic schedule match the original from `0xb1540` onward; the
residual is a 4-byte shift inherited from the case183 body and an extra
`ldr r5,[sp,#116]` (`enemyRace`/`stationIndex` spill) at the end of case1.

## 7. Sweep methodology

* Independent adjacent-statement swap sweep (both statements `;`-terminated,
  same indentation, disjoint identifier sets): **155 candidates over the whole
  body** (`1525..2600`) and **83 candidates over the switch half** (`1940..2600`).
  Each candidate was a real `link_so` build + `tools/fn_diff.py` measurement.
  Minimum raw over all candidates: **3630** (ties with base); no candidate
  below 3630.
* Case-block source-order permutations (24), `switch(k)` store-order
  permutations (20 single-case + 16 offset2-last combos), raid-block spellings
  (6), alien-loop spellings (7), int/unsigned variants (5), expect variants (5).
* Builds used `-j2` under `ulimit -v 12000000`.

## 8. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w12715
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
cmake --build cmake-build-match --target check -j2
```

Expected: `raw=3630 aligned=1127 size=17172/17172`, `.text` diff 75,015,
`.ARM.extab` diff 646, `.rel.plt` diff 0, `RATCHET PASS`.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w12715/`
(`measure.sh`, `sweep.py`, `sweep_extab.py`, `map3.py`, `regions.py`,
`variants*.json`, `Level.base.cpp`, `adj3.log`).
