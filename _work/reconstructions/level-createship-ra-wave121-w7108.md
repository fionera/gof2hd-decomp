# `Level::createShip` — wave 121 (w7108): real-`new` reproduces the 336 B LSDA, not the prologue

**Status:** analysis only, no `src/` change landed (2026-11-06).
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`,
original 2764 B @ `0xc0214`, Thumb entry `0xc0215`, 1007 insns).
Branch `wave121-w7108`, base `bd061a489`. Worktree left clean; only this
note is committed.

## Result

```
                                   original        HEAD (placement+try)   real-new (old body)
createShip fuzzy                   —               74.4                   70.3
createShip size                    2764 B          2764 B                 2764 B
createShip insns                   1007            1007                   1005
createShip LSDA record             336 B           328 B                  336 B
createShip LSDA rows               57              54                     57
createShip LSDA ttype              0xff            0x00 (type table)      0xff
createShip cleanup stubs           4 distinct      2 + catch              3 distinct
prologue prefix vs original        94 insns         94                     2
.ARM.extab size                    0x26298         0x26294                0x2629c
.rodata base                       0x1c9e10        0x1c9e10               0x1c9e20
gate byte_exact / linked_exact     4505 / 4505     4505 / 4505            (see below)
```

**Answer to the assignment:** a real-`new` source shape *does* reproduce the
original's **336 B cleanup-only LSDA record** (`ttype 0xff`, 57 rows), but it
does **not** reproduce the original's prologue.  The real-`new` cleanup live
ranges push Greedy RA one callee-saved register over budget: `shipClass` and
`race` are both spilled (or `type` is spilled) instead of the original's
`race`-only spill, so the first difference is instruction 2.  The only shape
that matches the prologue is the current **placement-`new` + `try/catch`**
body, and that necessarily costs the 328 B record and a type table.

## 1. Ground truth: the original is real `new`

Parsed from the original `.ARM.extab`/`.ARM.exidx` with the record-length
parser (`_work/reconstructions/exidx_table.py`; LSDA at extab+12, `lps=0xb0`,
`ttype=0xff`, `cs=0x01`, table length 315):

```
orig : rec=336  rows=57  ttype=0xff  table_end=+336
```

The four cleanup landing pads at `0xc0bba..0xc0bd6` are the classic
`new`-expression cleanups:

```
c0bba: mov r8,r0 ; mov r0,sl ; b c0bd0   ; PlayerFighter  (row 29)
c0bc0: mov r8,r0 ; mov r0,fp ; b c0bd0   ; Array<BoundingVolume*> (row 5)
c0bc6: mov r8,r0 ; mov r0,r9 ; b c0bd0   ; PlayerFixedObject (row 3)
c0bcc: mov r8,r0 ; mov r0,r5 ; b c0bd0   ; Player + BoundingAAB (rows 1,7..55)
c0bd0: bl operator delete
c0bd4: mov r0,r8
c0bd6: bl _Unwind_Resume
```

No `__cxa_begin_catch` anywhere: these are cleanups, not catch handlers.
The original's register assignment is therefore:

| allocation | register |
|---|---|
| `Player` | `r5` (then reused for each `BoundingAAB`) |
| `PlayerFixedObject` | `r9` |
| `Array<BoundingVolume*>` | `fp` |
| `PlayerFighter` | `sl` |

The original prologue (first 14 insns):

```
push {r4,r5,r6,r7,lr}
add  r7,sp,#12
stmdb sp!,{r7,r8,r9,sl,fp}
vpush {d0-d10}
str  r1,[sp,#60]      ; race
mov  r9,r0            ; this
ldr  r0,[pc,#572]
mov  fp,r3            ; type
mov  r5,r2            ; shipClass
add  r0,pc
ldr  r4,[r0,#0]       ; status
ldr  r0,[r4,#0]
blx  getCurrentCampaignMission
mov  r8,r0            ; mission
ldr  r0,[r7,#8]       ; waypoint
mov  sl,r5            ; shipClass -> sl
cbz  r0, ...
```

Only `race` is spilled; `type` is `fp`, `shipClass` is `r5 -> sl`.

## 2. HEAD: placement-`new` + `try/catch` matches the prologue

The current body (landed by `e0357a37c`, wave 119 w6966) uses
`(T*)::operator new(sizeof(T))` + placement `new (p) T(...)` for both ships
and wraps `player->setEmpData` in `try { } catch (...) { __builtin_trap(); }`.

```
HEAD : rec=328  rows=54  ttype=0x00  (6-byte type table at +322)
```

Landing pads: two cleanup stubs (`r9`, `r5`) plus the catch handler
(`__cxa_begin_catch; udf`).  The prologue matches the original for the first
**94 instructions** (the first difference is the coordinate block at
instruction 94, `0xc03c8`).

The `try/catch` is what makes the RA match: it adds a live exception value
at exactly the point where the real-`new` cleanup pointers would otherwise
be live, and Greedy RA then keeps `type` in `fp` and `shipClass` in
`r5 -> sl`.  Removing the `try` from the same placement body drops the score
to 70.5 % and the prologue no longer matches (`str r1,[sp,#56]`,
`mov sl,r3`, `mov fp,r5`).

## 3. Real-`new` variants: 336 B record, wrong prologue

All rows below are real `new` for both ships, no `try/catch`, official
scorer (`tools/verify/verify.py`), one-TU mini `.so` / full link.

| variant | % | size | LSDA | first prologue diff |
|---|---:|---:|---:|---|
| **old body (`8f10cc659`), 336 B** | **70.3** | 2764 | 336 / 57 / 0xff | insn 2: `str r2,[sp,#60]` (shipClass) |
| old body + current float coord formulas | 70.3 | 2764 | 336 / 57 / 0xff | insn 2 |
| current body, real new both ships | 64.6 | 2768 | 336 / 57 / 0xff | insn 2: `strd r1,r2,[sp,#56]` |
| current body, real new + faithful int coords | 63.1 | 2788 | 336 / 57 / 0xff | insn 2: `str r3,[sp,#60]` (type) |
| current body, real new + if/else hitpoint chain | 62.8 | 2788 | 336 / 57 / 0xff | insn 2 |
| current body, real new + `ship` assigned early | 63.3 | 2792 | 336 / 57 / 0xff | insn 2 |
| current body, real new + `KIPlayer*` locals | 63.3 | 2792 | 336 / 57 / 0xff | insn 2 |
| current body, real new + `Level *self` alias | 63.1 | 2788 | 336 / 57 / 0xff | insn 2 |
| current body, real new + branch swap (capital first) | 60.0 | 2780 | 336 / 57 / 0xff | insn 2 |
| old body + `try/catch(...) { throw; }` | 48.3 | 2752 | 372 / 57 / 0x00 | insn 2 |
| old body + `try/catch(...) { __builtin_trap(); }` | 64.1 | 2768 | 352 / 57 / 0x00 | insn 2 |
| real new Fighter only (Fixed placement) † | 63.6 | 2768 | 336 / — / — | insn 2 |
| real new Fixed only (Fighter placement) † | 65.4 | 2784 | 336 / — / — | insn 2 |

† measured on the old body in wave 119 (`level-createship-wave119-w6916.md`),
not re-measured this pass.

The real-`new` cleanup live ranges force one of `type`/`shipClass` out of
the callee-saved set.  In the old 70.3 % body the prologue is:

```
str r2,[sp,#60]   ; shipClass spilled
mov r9,r0
ldr r0,[pc,#572]
str r1,[sp,#52]   ; race spilled
mov sl,r3         ; type -> sl
ldr r0,[pc,#560]
...
```

i.e. two argument spills and `type` in `sl`, versus the original's one spill
and `type` in `fp`, `shipClass` in `r5 -> sl`.  In the current-body real-new
the prologue is `strd r1,r2,[sp,#56]` (race + shipClass) and `mov sl,r3`
(type), again two spills.

The original has the same 57 cleanup rows and 336 B record, so the EH shape
is right; only the register permutation is wrong.  No source rewrite tried
in this pass (coordinate formulas, int vs float coords, if/else vs switch,
`ship` assignment order, `KIPlayer*` locals, `self` alias, branch order,
decl-at-top, bool copies, `try/catch` variants) moves the first Greedy
decision while keeping the 336 B record.

## 4. Concrete patch proposal (real-`new` + exact `.ARM.extab`)

The old real-`new` body from `8f10cc659` is the best real-`new` shape:
2764 B (so `.text` stays exact), 336 B LSDA, `ttype 0xff`, 70.3 %.
Switching to it alone makes `.ARM.extab` 4 B too large (`0x2629c`), which
shifts `.rodata` to `0x1c9e20` and drops `byte_exact` from 4505 to 2363.
(The minimal real-`new` edit against the *current* body — the diff below —
is smaller but only reaches 64.6 % and 2768 B, which also grows `.text` by
0x20 and shifts `.ARM.extab` to `0x1a3b98`.)

The fix is to pair it with the `Generator::createMission` 172 B LSDA fix
already present on branch `wave119-w6936` (commit `5be8ac28b`, "172B LSDA
via reward-chain merge + scope guard").  Measured combined result:

```
createShip (old real-new body)   70.3 %   2764 B   336 B LSDA / 0xff
createMission (5be8ac28b)        73.3 %   1996 B   172 B LSDA / 0xff
.ARM.extab                       0x26298 (exact)
.rodata base                     0x1c9e10 (exact)
.text                            0x1337c8 (exact)
check                            byte 4505  linked 4505  avg 99.93
                                 data 843/843  imports 207  RATCHET PASS
```

Patch (Level.cpp only; the Generator half is another TU):

```diff
@@ -5216,16 +5216,11 @@ PlayerFixedObject *Level::createShip(...)
     int x = jitterZ + jitterX - 20000;
     float y = (float) (wy + jitterY - 20000);
     float z = (float) (wz + wx - 20000);
-    try {
-        player->setEmpData(empPoints, empTime);
-    } catch (...) {
-        __builtin_trap(); // lint: arm_builtin anchor: Level::createShip; removal grows body 2764 -> 2772
-    }
+    player->setEmpData(empPoints, empTime);
 
     KIPlayer *ship = nullptr;
     if (shipClass == 0) {
-        PlayerFighter *fighter = (PlayerFighter *) ::operator new(sizeof(PlayerFighter)); // lint: operator_call
-        new (fighter) PlayerFighter(type, race, player, nullptr, x, y, z, hostile);
+        PlayerFighter *fighter = new PlayerFighter(type, race, player, nullptr, x, y, z, hostile);
@@
-        PlayerFixedObject *fixed = (PlayerFixedObject *) ::operator new(sizeof(PlayerFixedObject)); // lint: operator_call
-        new (fixed) PlayerFixedObject(type, race, player, nullptr, x, y, z);
+        PlayerFixedObject *fixed = new PlayerFixedObject(type, race, player, nullptr, x, y, z);
```

This gives the faithful 336 B / `0xff` LSDA and an exact `.ARM.extab`, but
it is a **score regression** (createShip 74.4 -> 70.3, createMission
84.3 -> 73.3, avg 99.94 -> 99.93).  It is the right patch only if the 336 B
cleanup-only record is treated as the target and the score loss is accepted.
The current placement+try body remains the higher-score shape; its 328 B
record is 4 B short of the original's, but `.ARM.extab` is still `0x26294`
(rounds to the same `0x1c9e10` `.rodata` base) and the gate is green.

## 5. Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
python3 _work/reconstructions/exidx_table.py --diff \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
python3 tools/verify/verify.py \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5Level10createShipEiiiP8Waypointbb
```

Scratch helpers (not committed): `/dev/shm/lsda2.py` (record-length-aware
LSDA parser), `/dev/shm/score_cs.py` (single-function official scorer),
`/dev/shm/prefix.py` (prologue prefix match), `/dev/shm/variants*.py`
(variant harness).
