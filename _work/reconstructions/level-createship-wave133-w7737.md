# `Level::createShip` — wave 133 (w7737): non-EH RA levers, no source win

Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`, original
2764 B @ `0xc0214`, Thumb entry `0xc0215`, 1007 insns).
Base `144fc16b0` (wave-132 master). Worktree `/home/debian/GalaxyOnFire2/wt/w7737`.
Owned TU: `src/game/world/Level.cpp` (createShip only).

**Status: analysis only — no `src/` change landed.**  Master stays at
**70.6 %** (2764 B, 1007/1005 insns) with the exact 336 B / 57-row /
`ttype 0xff` cleanup-only LSDA.  The task was the w7709 open question: find a
**non-EH** source shape that reproduces the w7693 register allocation
(`type -> fp`, `shipClass -> r5 -> sl`, `race -> [sp,#60]`) while keeping
`setEmpData` a plain call and the exact LSDA record.  ~35 shapes were measured;
none reproduced the allocation without EH.

`check` on the unchanged tree: **RATCHET PASS** — `verify: byte 4506 linked
4506 stubs 0 missing 0 extra 0 avg 99.94`, `data: exact 843 of 843`,
`sodiff: imports 207`, lint `CLEAN`, drift 0.

## 1. What actually flips Greedy RA (new evidence)

`-mllvm -print-machineinstrs` on the real TU (NDK r18b clang 7.0.2) gives the
pre-RA and post-RA MIR.  Comparing the w7693 body with and without the
`try`/`catch` around `setEmpData` isolates the mechanism:

| body | score | prologue (first 8) |
|---|---:|---|
| w7693 (`try`, placement `new`, double `getLevel`) | 73.4 | `str r1,[sp,#60]; mov r9,r0; ldr r0,[pc,#572]; mov fp,r3; mov r5,r2; add r0,pc; ldr r4,[r0]; ldr r0,[r4]` |
| same, `try` removed | 61.1 | `str r1,[sp,#60]; mov r9,r0; ... mov fp,r2; ...` (bad) |

The `try` inserts `EH_LABEL` pseudo-instructions around the `setEmpData`
call.  `EH_LABEL` is a **scheduling boundary** (`MachineInstr::isLabel()` ->
`TargetInstrInfo::isSchedulingBoundary`), so the pre-RA MachineScheduler cannot
sink the coordinate arithmetic across it.  In the `try` body `bb.17` therefore
computes `y`/`z` *before* the call and `bb.20` computes `x` after; without the
`try` all three coordinates sink below the call into the status-check block.
That changes the live ranges of the coordinate/jitter vregs across the call and
re-seats Greedy RA's first decisions.

So the w7693 `try` is **both** a scheduling barrier and (as w7709 said) an RA
lever; the RA change is a downstream consequence of the scheduling change plus
the extra landing-pad block.

## 2. The good allocation needs the whole w7693 shape, not just the `try`

Starting from the w7693 body and changing one thing at a time:

| variant | score | prologue | extab |
|---|---:|---|---:|
| w7693 (baseline) | 73.4 | **good** | 156,304 (328 B) |
| w7693 + single-call `getLevel` (both) | 71.0 | **good** | 156,304 |
| w7693 + `Vector pos` in capital branch | 73.5 | **good** | 156,304 |
| w7693 + placement->plain `new` (keep `try`) | 63.3 | bad | 156,328 |
| w7693 + drop `try` (keep placement `new`) | 69.7 | bad | 156,292 |
| w7693 + plain `new` + drop `try` | 66.8 | bad | **156,312 (exact)** |
| w7693 + status guard after `setEmpData` | 61.9 | bad | 156,304 |

So the exact LSDA (plain `new`, no `try`) and the good allocation are coupled:
removing either the `try` or the placement `new` breaks the allocation.

## 3. Non-EH shapes tried on the master (plain-`new`, exact-LSDA) body

All measured with the official one-function scorer on a full link.  "exact
extab" means size 156,312 @ `0x1a3b78`.

| variant | score | usize | extab |
|---|---:|---:|---:|
| **master** | **70.6** | 2764 | exact |
| direct `x,y,z` at ctor sites (no `Vector pos`) | 70.3 | 2764 | exact |
| double-call `getLevel` for `level` only | 68.7 | 2772 | exact |
| double-call `getLevel` for `empPoints` only | 68.4 | 2772 | exact |
| double-call `getLevel` for both | 68.7 | 2780 | exact |
| all-`int` coordinates (faithful types) | 61.5 | 2772 | exact |
| coords computed before `new Player` | 62.9 | 2768 | exact |
| drop status guard | 63.6 | 2748 | exact |
| double-call `getLevel` for both **+ drop status guard** (faithful shape, 2764 B) | 67.0 | 2764 | exact |
| status guard moved before `setEmpData` | 63.2 | 2776 | exact |
| status guard at function entry | 59.8 | 2792 | exact |
| `if (empPoints == 0) goto` block before the call | 60.3 | 2796 | exact |
| `if (player != nullptr) setEmpData(...)` | 70.6 | 2764 | exact (folded) |
| `if (empTime > 0) setEmpData(...)` | 70.6 | 2764 | exact (folded) |
| `if (armor > 0) setEmpData(...)` | 70.6 | 2764 | exact (folded) |
| `{ setEmpData(...); }` extra scope | 70.6 | 2764 | exact (folded) |
| `if (empPoints > 0) setEmpData(...)` | 60.3 | 2796 | exact |
| `if (mission >= 0) setEmpData(...)` | 57.6 | 2796 | exact |
| `if (shipClass < 2) setEmpData(...)` | 62.9 | 2788 | exact |
| `switch (empTime) { case 0x7fffffff: return nullptr; default: setEmpData }` | 70.6 | 2764 | exact (folded) |
| `if (empTime == 0x7fffffff) return nullptr` after call | 70.6 | 2764 | exact (folded) |
| `if (empPoints == 0x7fffffff) return nullptr` after call | 51.1 | 2788 | exact |
| `if (shipClass > 1) return nullptr` after call | 47.2 | 2800 | exact |
| `if (shipClass == 2) return nullptr` after call | 41.4 | 2768 | exact |
| `if (mission == 0x7ffffff) return nullptr` after call | 48.2 | 2756 | 156,316 |
| `switch (shipClass)` instead of if/else | 41.2 | 2768 | 156,316 |
| `volatile` x/y/z | 59.4 | 2840 | 156,316 |
| coordinate declaration order (6 perms of x,y,z) | 70.6 all | 2764 | exact |

Every non-EH control-flow boundary either folds away (`empTime` is provably
45000/15000, `player` is provably non-null, `armor` is 650/1000) or adds real
code and makes the score worse.  None produces `mov fp,r3; mov r5,r2;
mov sl,r5`.

## 4. Live-interval sizes (why the order does not obviously change)

Parsed from the "After Machine Instruction Scheduler" MIR (span in slot units,
including DBG_VALUE uses):

```
master (bad RA)                 w7693 (good RA)
this   v126 26096               this   v124 26160
type   v129 25808               type   v127 25872
race   v127 25616               race   v125 25680
player  v43 22000               player  v44 21008
excobj v676 21984               excobj v679 21584
ship   v128  6592               ship   v126  6880
```

The relative order (`this > type > race > ... > shipClass`) is identical in
both, so the flip is not a simple size reorder.  Greedy RA's queue is
`priority_queue<pair<unsigned,unsigned>>` with `Prio = (1<<31)|(1<<29)|Size`
for global ranges (`RegAllocGreedy::enqueue`, LLVM 7.0.2); the changed
allocation must come from the changed *interference* / block-frequency inputs
(the landing-pad block and the shifted slot indices), not from `Size` alone.

Post-RA prologue maps:

```
master:  type->r10(sl)  shipClass->[sp]  race->[sp]  this->r9  status->r4
w7693 :  type->r11(fp)  shipClass->r5    race->[sp]  this->r9  status->r4
```

## 5. Open items for the next wave

1. The only known good-allocation shape is `placement new + try + double
   getLevel` (73.4 %, 328 B record).  The only known exact-record shape is
   plain `new` + no `try` (70.6 %).
2. Untried: a `try` around a call that is *already* an invoke (e.g. around
   `new Player`) to add a mid-function landing-pad block without touching the
   `setEmpData` call site; or a `catch` clause that yields a 336 B cleanup-only
   record.
3. Untried: forcing the coordinate arithmetic above the call with a
   non-`EH_LABEL` scheduling boundary that survives `-Oz` (e.g. an
   `alloca`/SP-modifying instruction between the coords and the call).
4. The original's source has **no** `Globals::status == nullptr` guard (the
   original's first instruction after `setEmpData` is `mov r9,#0; cmp r6,#0`),
   so the master's 16-byte guard is an RA anchor, not faithful source.

## 6. Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
python3 tools/verify/verify.py \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5Level10createShipEiiiP8Waypointbb
```

Scratch helpers (not committed): `/dev/shm/w7737/{try.sh,score.py,pro.sh,
mirdump.sh,cc.sh,bodies/*.txt}`.
