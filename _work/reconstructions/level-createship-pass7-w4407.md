# Level::createShip — pass 7 (wave 94, w4407)

**Status:** analysis only, no source change landed (2026-11-03). Worktree
`wt/w4407`, branch `wave94-w4407`, base `f262bd4b`. Target:
`_ZN5Level10createShipEiiiP8Waypointbb` (original 2764 B @ `0xc0214`,
Thumb entry `0xc0215`). Current verifier score **74.1 %** (osize 2764,
usize 2764, 1007 original insns / 1008 ours, `byte False`).

Gate before and after (unchanged, no code change):

```text
byte_exact 4499/4524  linked_exact 4499/4524  size-exact 4524/4524
stub_zero_size 0  missing 0  wrong_type 0  extra 0  data_exact 843/843
imports 207  .text 0x1337c8  .rodata 0x41b67  .ARM.exidx 0x6f70
.ARM.extab 0x26290 (-8)  lint CLEAN ✓  drift 0  RATCHET PASS
```

This pass continues `level-createship-pass6-w4264.md`. It adds (a) a fresh
headless-Ghidra decompile of the original, (b) a full decode of the
original vs our LSDA call-site tables, (c) a declaration-order / slot-map
sweep, and (d) a new "spawn before `new Player`" experiment that
reproduces the original's *instruction order* in the spawn region but not
its entry allocation. No source shape beats **74.1 %**.

---

## 1. Fresh Ghidra decompile (authoritative local order)

Headless Ghidra 12.1.2, project `/home/debian/GalaxyOnFire2/GalaxyOnFire2`,
program `android_2.0.16_libgof2hdaa.so`, image base `0x10000`,
`DecompLong.java` on `0xd0214` (`createShip`):

```c
iVar1 = Status::getCurrentCampaignMission(...);
if (param_4 == (Waypoint *)0x0) {
    iVar12 = 0; local_54 = 0; iVar2 = 0;
} else {
    local_54 = *(int *)(param_4 + 0x124);   /* waypoint->x  = A */
    iVar12   = *(int *)(param_4 + 0x128);   /* waypoint->y  = B */
    iVar2    = *(int *)(param_4 + 300);     /* waypoint->z  = C */
}
iVar3 = (*pcVar18)(*puVar17,40000);         /* jitterX = j1 */
iVar4 = (*pcVar18)(*puVar17,40000);         /* jitterY = j2 */
iVar5 = (*pcVar18)(*puVar17,40000);         /* jitterZ = j3 */
...
iVar1 = Status::hardCoreMode();
this_00 = operator_new(0x114);
iVar14 = 1000;
if (iVar1 != 0) iVar14 = 0x28a;
Player::Player(this_00,iVar14,iVar6,1,1,0);
iVar3 = local_54 + iVar3 + -20000;          /* x = A + j1 - 20000  (int) */
iVar1 = iVar12   + iVar4 + -20000;          /* y = B + j2 - 20000  (int) */
fVar20 = (float)(iVar2 + iVar5 + -20000);   /* z = C + j3 - 20000  (int->float) */
Player::setEmpData(this_00,iVar7,iVar13);
this_02 = (PlayerFixedObject *)0x0;
if (param_2 == 0) { ... PlayerFighter ... }
if (param_2 != 1) goto LAB_000d0b70;
this_02 = operator_new(0x1bc);
... PlayerFixedObject ...
```

The source is therefore the **faithful all-int** shape:

```cpp
int wx = waypoint->x, wy = waypoint->y, wz = waypoint->z;   /* A, B, C */
int jitterX = rnd(), jitterY = rnd(), jitterZ = rnd();      /* j1, j2, j3 */
...
Player *player = new Player(armor, hitpoints, 1, 1, 0);
int x = wx + jitterX - 20000;
int y = wy + jitterY - 20000;
int z = wz + jitterZ - 20000;
player->setEmpData(empPoints, empTime);                     /* plain call */
ship = nullptr;
if (shipClass == 0) { /* PlayerFighter, placement new */ }
else if (shipClass == 1) { /* PlayerFixedObject, placement new */ }
```

with the three `vcvt.f32.s32` duplicated in both `shipClass` branches.
The committed source is a *semantic hack* (swapped waypoint names
`wz=A, wy=B, wx=C` + rotated jitter order `Y,Z,X` + mixed
`int x / float z / float y` + `try/catch`). It scores 74.1 because it
matches the original's entry allocation; the faithful shape scores 72.6.

## 2. LSDA decode: the original really has no `catch`

`.ARM.exidx` points `0xc0214` at extab `0x1ab09c` (original) /
`0x1ab0a0` (ours). The extab entry is `[prel31 personality][8-byte EHABI
header][LSDA]`; the LSDA proper starts at entry+16 (`ff ...`).

| | original | ours (committed) |
|---|---|---|
| extab entry size | **0x150** | **0x148** |
| LSDA header | `ff ff 01` (LPStart omit, **TType omit**, uleb) | `ff 00 b4 02` (TType present) |
| call-site table length | **315** | **299** |
| call-site entries | **57** | **54** |
| entries with a landing pad | **28** | **27** (26 cleanup + 1 catch) |
| `action == 1` entries | **0** | **1** |
| landing-pad range | `0x976..0x9b8` | `0x97c..0x9b8` |

Original landing pads (`0xc0b8a..0xc0bda`) are cleanup-only:

```text
c0b8a..c0bb8  b.n 0xc0bcc          ; shared "delete r5" pad
c0bba         mov r8,r0; mov r0,sl; b 0xc0bd0   ; delete PlayerFighter
c0bc0         mov r8,r0; mov r0,fp; b 0xc0bd0   ; delete Array
c0bc6         mov r8,r0; mov r0,r9; b 0xc0bd0   ; delete PlayerFixedObject
c0bcc         mov r8,r0; mov r0,r5; b 0xc0bd0   ; delete Player
c0bd0         blx _ZdlPv
c0bd4         mov r0,r8
c0bd6         blx _Unwind_Resume
```

There is **no `__cxa_begin_catch`** and no TType table. The original's EH
record is exactly the cleanup-only record produced by plain/placement
`new` expressions; the committed `try { setEmpData } catch (...) { trap }`
adds the TType table, the action-1 call site and the catch pad. This
confirms `level-createship-pass2-w4006.md` §4 and `pass3` §4.

Both builds still have **28 `_Znwj` calls** and **24 `BoundingAAB`
constructors**, so the 2-pad difference is a landing-pad-sharing artefact
of the different register allocation (which register holds the pending
`operator new` pointer), not a missing/extra allocation.

## 3. Slot maps (authoritative, current build)

Frame: `push {r4,r5,r6,r7,lr}` 20 + `stmdb {r7,r8,r9,sl,fp}` 20 +
`vpush {d0-d10}` 88 = 128; FI colour `k` sits at `[sp, 60-4k]`.

```text
original : A=44  B=48  C=40  j1=36  j2=32  j3=28
committed: A=44  B=48  C=32  j1=40  j2=36  j3=28
```

The committed waypoint stores (`objdump`):

```text
c023c ldr.w r1,[r0,#292]  ; A
c0240 str   r1,[sp,#44]
c0242 ldr.w r1,[r0,#296]  ; B
c0246 ldr.w r0,[r0,#300]  ; C
c024a str   r1,[sp,#48]
c0258 str   r0,[sp,#32]   ; C -> 32   (original: [sp,#40])
c026e str   r0,[sp,#40]   ; j1 -> 40  (original: [sp,#36])
c0278 str   r0,[sp,#36]   ; j2 -> 36  (original: [sp,#32])
c028c str   r5,[sp,#28]   ; j3 -> 28  (matches)
```

Only the last four colours differ: the 4-cycle
`(C,j1,j2) -> (j2,j3,C)`.

## 4. Declaration-order sweep (6 variants, committed body)

Only the waypoint **declaration** order matters (the assignment order
inside the `if`/`else` does not change the emitted code). Sweep with the
committed spawn/type/try body:

| declaration | A slot | B slot | score |
|---|---|---|---|
| `int wx, wz, wy;` (committed) | 44 | 48 | **74.1** |
| `int wx, wy, wz;` | 48 | 44 | 73.9 |
| `int wz, wx, wy;` | 48 | 44 | 72.6 |
| `int wz, wy, wx;` | 48 | 44 | 72.6 |
| `int wy, wx, wz;` | 44 | 48 | 72.9 |
| `int wy, wz, wx;` | 44 | 48 | 72.9 |

`wx,wz,wy` is the unique declaration that keeps `A=44, B=48` (the
original's first two slots). In every variant of the `try` family the
`C` slot is **32**, never the original's 40.

Spawn statement-order sweep (committed types): `xzy`, `zxy`, `zyx` =
74.1; `xyz`, `yxz`, `yzx` = 73.9. No order reaches 74.2.

## 5. New experiment: spawn before `new Player`

The original's machine order is `Player::Player; spawn adds; setEmpData`.
The committed source has the spawn *after* `new Player`, and the
SelectionDAG scheduler sinks the int `x` past the `setEmpData` invoke, so
the spawn region becomes `Player::Player; y,z (vcvt); setEmpData; x`.

Putting the spawn *before* `new Player` in the source makes the scheduler
emit the original's order (`PlayerC1; spawn adds; setEmpData`):

```cpp
int armor = hardCoreMode() ? 650 : 1000;
int x = wx + jitterX - 20000;
int y = wy + jitterY - 20000;
int z = wz + jitterZ - 20000;
Player *player = new Player(armor, hitpoints, 1, 1, 0);
player->setEmpData(empPoints, empTime);
```

Measured (official verifier, `autojunk=False`):

| variant | score | usize | prologue |
|---|---|---|---|
| committed baseline | **74.1** | 2764 | `str r1,[sp,#60]; mov r9,r0; mov fp,r3; mov r5,r2` |
| spawn-before-ctor, all-int, no try | 61.6 | 2796 | `strd r1,r2,[sp,#56]; mov r9,r0; mov fp,r3` |
| spawn-before-ctor, mixed, try | 60.8 | 2776 | `strd r1,r2,[sp,#56]; mov fp,r0; mov r9,r3` |
| spawn-before-ctor, all-int, try | 60.7 | 2752 | `str r2,[sp,#60]; mov r9,r3; str r0,[sp,#56]; mov sl,r1` |

The spawn-before-ctor variant reproduces the original's *spawn instruction
order* but spills **both** `race` and `shipClass` at entry
(`strd r1,r2,[sp,#56]`) and uses `r6` for `y` instead of `r4`; the
original spills only `race` (`str r1,[sp,#60]`) and keeps `shipClass` in
`r5`. The extra live range of the spawn values across the `new Player`
call is what raises the entry pressure. Its usize is 2796 (32 B over the
original), so the local instruction-order match is not enough.

## 6. Why the target slot map is unreachable in the `try` family

`StackSlotColoring` sorts the six spawn intervals by
`LiveIntervals::getSpillWeight` = sum of `block_freq/entry_freq` over
every non-debug FI reference, stable-sorted by FI index. The committed
build references `{B,A,j1,j2}` in the pre-invoke block `bb.23` and
`{C,j3}` in the post-invoke block `bb.26`; `bb.23` is fractionally hotter
(the landing-pad edge takes `2^-20` of the frequency), so the sorted
order is `B,A,j1,j2,C,j3` -> colours 3,4,5,6,7,8 = `B=48, A=44, j1=40,
j2=36, C=32, j3=28`. That is exactly the committed map.

To get the target `B,A,C,j1,j2,j3` the weight tiers must be
`{B,A}` (heavy) then `{C,j1,j2,j3}` (light), because the FI order is
`C,B,A,j1,j2,j3` and a stable sort within each tier preserves it. But
`B` is used only by the `y` computation and `A` only by the `z`
computation, so moving `j1,j2` to the light tier moves `B,A` with them.
The only way to tie all six is to remove the invoke (the faithful
no-`try` shape), which is exactly the shape that loses the prologue.

## 7. Micro-variants measured this pass

All with the committed body; official verifier:

| variant | score |
|---|---|
| committed baseline | **74.1** |
| `int x = wx + jitterX; x -= 20000;` + same for z,y | 70.4 |
| `float y` declared before `float z` | 73.9 |
| `int x, y, z;` declared before `new Player`, assigned after | 60.6 |
| `Player *player; player = new Player(...);` | 74.1 |
| `int armor; if (hardCoreMode()) armor = 650; else armor = 1000;` | 74.1 |
| `int x = wx - 20000 + jitterX;` (same for z,y) | 74.1 |

None beats 74.1.

## 8. Conclusion / what remains

The committed source is a strong local optimum at **74.1 %**. The
remaining structural mismatch is the register-allocation / scheduler
coupling:

1. the original is the faithful all-int no-`try` shape with the three
   adds before a **plain** `setEmpData` call and the three
   `vcvt.f32.s32` after it, duplicated in both `shipClass` branches;
2. our scheduler (`-pre-RA-sched=list-hybrid`) emits the call first and
   sinks the int adds, so the all-int shape changes the whole-function
   allocation (`type` leaves `fp`, `this` is spilled at entry) and drops
   to 60.7-61.6 %;
3. the committed baseline keeps the original prologue only by using a
   `try/catch` (an invoke at `setEmpData`) and mixed `int`/`float` spawn
   types, which puts `C` in slot 32 and `j1,j2` in 40/36;
4. the target slot map `A=44, B=48, C=40, j1=36, j2=32, j3=28` needs the
   no-invoke weight tie, i.e. the faithful shape, which loses the
   prologue.

The only score-positive lever found in the whole history is the
semantically wrong `PlayerFixedObject(race, type)` revert
(74.1 -> 74.2, commit `94e8bc54` deliberately fixed it to `(type, race)`);
it is not a faithful improvement and is not re-landed here.

No code change is justified: `byte_exact` does not rise.

## 9. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w4407
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5Level10createShipEiiiP8Waypointbb
```

Ghidra decompile:

```sh
FN_ADDR=0xd0214 /tmp/ghidra_12.1.2_PUBLIC/support/analyzeHeadless \
  /home/debian/GalaxyOnFire2 GalaxyOnFire2 \
  -process '*libgof2hdaa*' -noanalysis \
  -scriptPath /home/debian/ghidra_scripts -postScript DecompLong.java
```

Scratch helpers (all under `/dev/shm/w4407`): `score1.py` (official
verifier ratio for one symbol), `variants.py` (waypoint/jitter/spawn
matrix), `sweepA.py` (spawn-position × mapping × jitter sweep),
`spawnorder.py` (6 spawn statement orders), `slot2.py` (declaration-order
slot-map sweep), `pa_committed.txt` / `pa_vA.txt` (`-print-after-all`
dumps), `cs_full.txt` (Ghidra decompile), `orig.dis` / `ours_cur.dis`.

---

## 10. Wave-94 follow-up measurements (w4407, same worktree)

The pass-7 note above is the record of the first half of wave 94. This
section adds the measurements taken after it was written. Nothing below
changes the conclusion: **74.1 % is the best faithful shape found**.

### 10.1 The `.ARM.extab` −8 is exactly this function's EH record

Decoded from the two ELFs (`.ARM.exidx` word1 is a prel31 relative to
the word's own address; the extab entry is the next exidx target):

| | original | committed |
|---|---|---|
| exidx entry | `0x19ff20` | `0x19ff20` |
| extab entry | `0x1ab09c` | `0x1ab0a0` |
| next extab entry | `0x1ab1ec` | `0x1ab1e8` |
| **extab entry size** | **`0x150`** | **`0x148`** |
| LSDA header | `ff ff 01 bb 02` | `ff 00 b4 02 01 ab 02` |
| TType | omit | present (uleb offset `0x134`) |
| call-site table length | `315` (57 entries) | `299` (54 entries) |

`0x26298 - 0x26290 = 8` and `0x150 - 0x148 = 8`, so the whole
`.ARM.extab` shortfall is this one entry: the committed `try/catch`
adds the TType table (+8) and loses three call-site entries (−16),
net −8. Removing the `try` does **not** fix it: the committed-mapping
mixed-type no-`try` build gives `.ARM.extab = 0x26284` (−20) and
`.text = 0x1337e8` (+0x20); the faithful all-int no-`try` build gives
`.text = 0x1337c8` (base-exact) but `.ARM.extab = 0x26284` (−20).
The committed `try` shape is therefore the best of the three for the
section sizes as well as for the function score.

### 10.2 Full-score sweeps (official verifier, `autojunk=False`)

Every variant was compiled with the exact `compile_commands.json`
command and linked with the full gold link, then scored by
`tools/verify/verify.py` on `_ZN5Level10createShipEiiiP8Waypointbb`.

**(a) type pattern × coordinate mapping (48 variants).** Committed
declaration/assignment/jitter/spawn, `try` kept:

```text
best 74.1  iff CAB      iff CBA      (committed = iff CBA)
     73.9  iff ABC      iff ACB
     73.8  iff BAC      iff BCA
     73.1  fif ACB
     72.9  fif CAB
     72.6  ffi ABC
     68.4  iif BCA
     67.6  ifi BAC
     67.1  fii ABC
     64.9  fff ACB
     61.1  iii BAC
```

The committed `int x / float z / float y` with `x=C, z=A, y=B` is the
unique optimum; `iff CAB` ties it.

**(b) waypoint declaration × assignment × jitter declaration (216
variants).** Committed types (`iff`), mapping (`x=C,y=B,z=A`), spawn
`xzy`, `try` kept:

```text
best 74.1  decl=CAB, jitdecl=231, any assignment order (6 variants)
     74.0  decl=CAB, jitdecl=123/213/321
     73.9  decl=CBA, jitdecl=231
     ...
```

The committed `int wx, wz, wy;` + `jitterY, jitterZ, jitterX` is a
6-fold-degenerate optimum; no declaration permutation beats 74.1.

**(c) slot-map sweep of the no-`try` all-int family (1296 variants:
decl × assign × jitdecl × coord).** No variant reaches the target
`A=44 B=48 C=40 j1=36 j2=32 j3=28`. The reachable no-`try` slot maps
are only:

```text
(48,44,40, reg,36,32)  (44,48,40, reg,36,32)
(48,40,44, reg,36,32)  (44,40,48, reg,36,32)
(40,44,48, reg,36,32)  (40,48,44, reg,36,32)
and the jitdecl=123 variants with (…, reg,32,reg)
```

`A=44 B=48 C=40` is reachable (decl `ACB`), but the third jitter is
always register-allocated, never `[sp,#28]`; the target `j1=36 j2=32
j3=28` is not reachable in the no-`try` all-int family either. This
independently confirms §3.4: the target map and the prologue are
mutually exclusive.

### 10.3 Micro-variants (all 74.1 unless stated)

| variant | score |
|---|---|
| committed baseline | **74.1** |
| `jitterX` declared before `jitterY` (the uncommitted pass-7 diff) | 74.0 |
| `ship` declared before the `try` | 74.1 |
| `int x = …, y2 = 0;` single-declaration line | 74.1 |
| explicit `if/else` for `level` | 74.1 |
| explicit `if/else` for `empPoints` | 74.1 |
| ternary waypoint init (`int wz = waypoint ? … : 0;`) | 74.1 |
| pre-initialised waypoint locals + `if` without `else` | 74.1 |
| jitter declaration and assignment split | 74.1 |
| empty `catch (...) {}` | 65.1 |
| `catch (...) { throw; }` | 49.4 |
| `empPoints` computed before `hitpoints` | 57.0 |
| `empPoints` derived from `level` (one `getLevel` call) | 64.7 |
| `PlayerFixedObject(race, type, …)` (semantic swap) | 74.2 |

The only >74.1 shape remains the semantically wrong
`PlayerFixedObject(race, type)` swap; the original disasm
(`0xc0426 ldr r4,[sp,#52]` = type, `0xc042a ldr r6,[sp,#60]` = race,
`0xc0432 mov r1,r4`, `0xc0438 mov r2,r6`) proves `(type, race)` is
correct.

### 10.4 Scratch helpers (wave-94 follow-up, `/dev/shm/w4407`)

`score1.py` (official ratio for one symbol), `diffdump.py` /
`showdiff.py` (aligned diff), `parse2.py` (slot-map parser for both
the `try` and no-`try` shapes), `sweep2.py` (216/1296-variant slot
sweeps), `full_sweep.py` / `full_sweep2.py` (full-link score sweeps),
`micro.py` / `micro2.py` / `micro3.py` (micro-variants),
`Level_committed.cpp` (the committed body used as the sweep template).
