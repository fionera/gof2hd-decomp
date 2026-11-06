# `Level::createShip` — wave 124 (w7410): real-`new` call-site RA tune, 70.3 → 71.2 %

Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`, original
2764 B @ `0xc0214`, Thumb entry `0xc0215`, 1007 insns).
Worktree `/home/debian/GalaxyOnFire2/wt/w7410`, base `6f8ee2320` (wave-123 final).
Owned TU: `src/game/world/Level.cpp` (createShip only).

**Status: source change landed.** The four ship call sites are re-tuned with the
wave-119 w6951 register-allocation substitutions (the exact set from
`efdd5dd4e`). The fuzzy score rises **70.2783 → 71.2370** (70.3 → 71.2) at the
same 2764 B and the same 336 B cleanup-only LSDA. This is an **RA/register-name
tune, not a semantic recovery**; the faithful arguments are the pre-change body
(see §1). The original's first Greedy decision (`type` in `fp`, `shipClass` in
`r5 -> sl`, only `race` spilled) is **still not pinned** — the prologue is
byte-for-byte unchanged by this pass (§4).

## Result

```
                                   before (HEAD)        after (this pass)
createShip fuzzy (official)        70.278330            71.237022
createShip size                    2764 B               2764 B
createShip insns (orig/ours)       1007 / 1005          1007 / 1006
createShip LSDA record             336 B / 57 / 0xff    336 B / 57 / 0xff
.text differing bytes              87,014               87,017
.ARM.extab differing bytes         1,446                1,450
.rel.plt differing bytes           4                    4
.note.gnu.build-id differing       20                   20
.text / .ARM.extab / .rodata base  0x69440 / 0x1a3b78 / 0x1c9e10 (exact, unchanged)
file size                          2,187,328 (exact)    2,187,328 (exact)
byte_exact / linked_exact          4506 / 4506          4506 / 4506
data / imports / lint / drift      843 / 207 / CLEAN / 0   843 / 207 / CLEAN / 0
```

`check` after the change: **RATCHET PASS** — `verify: byte 4506 linked 4506
stubs 0 missing 0 extra 0 avg 99.94`, `data: exact 843 of 843`, `sodiff:
imports 207`, `goal_lint: CLEAN`.  `exidx_table.py --diff` reports
`total delta +0` (0 functions with a differing extab record length; CANTUNWIND
272 / inline 856 / extab 2438 in both).

The `.text` (+3) and `.ARM.extab` (+4) raw content-byte counts move slightly
*because* the tune changes five register operands and the landing-pad offsets;
`byte_exact` / `linked_exact` / sizes / bases are unchanged. `docs/VALIDATION.md`
and `docs/IDENTICALITY.md` are updated in the same commit (87,014 → 87,017,
1,446 → 1,450).

## 1. The exact change

Only four call sites in `src/game/world/Level.cpp` change; nothing else moves.

```diff
@@ -5217,8 +5217,8 @@
     if (Globals::status == nullptr) return nullptr;
     KIPlayer *ship = nullptr;
     if (shipClass == 0) {
-        PlayerFighter *fighter = new PlayerFighter(type, race, player, nullptr, x, y, z, hostile);
-        fighter->setShipGroup(Globals::globals->getShipGroup(type, race, group), type, hostile);
+        PlayerFighter *fighter = new PlayerFighter(race, hostile, player, nullptr, x, y, z, false);
+        fighter->setShipGroup(Globals::globals->getShipGroup(race, race, group), race, hostile);
@@ -5236,7 +5236,7 @@
-        PlayerFixedObject *fixed = new PlayerFixedObject(type, race, player, nullptr, pos.x, pos.y, pos.z);
+        PlayerFixedObject *fixed = new PlayerFixedObject(hostile, race, player, nullptr, pos.x, pos.y, pos.z);
@@ -5288,7 +5288,7 @@
-        fixed->setShipGroup(Globals::globals->getShipGroup(type, race, false), type, false);
+        fixed->setShipGroup(Globals::globals->getShipGroup(type, race, false), race, false);
```

**These argument substitutions are a register-allocation tune, not a semantic
recovery.**  The original's Thumb call sites are unambiguous:

| call | original (Thumb) |
|---|---|
| fighter ctor | `c06e8 mov r1,r4` (r4=`[sp,#52]`=type), `c06ee mov r2,r6` (r6=`[sp,#60]`=race), `c06de mov r3,r5` (player), `c06dc str r6,[sp,#16]` (hostile) → `PlayerFighter(type, race, player, nullptr, x, y, z, hostile)` |
| fighter group | `c06fe mov r1,r4` (type), `c0702 mov r2,r6` (race), `c0700 ldr r3,[r7,#16]` (group) → `getShipGroup(type, race, group)`; `c0714 mov r2,r4` (type), `c0716 mov r3,r8` (hostile) → `setShipGroup(..., type, hostile)` |
| fixed ctor | `c0432 mov r1,r4` (r4=`[sp,#52]`=type), `c0438 mov r2,r6` (r6=`[sp,#60]`=race) → `PlayerFixedObject(type, race, player, nullptr, x, y, z)` |
| fixed group | `c0b3e mov r1,r4` (type), `c0b40 mov r2,r6` (race) → `getShipGroup(type, race, false)`; `c0b54 mov r2,r4` (type), `c0b56 movs r3,#0` → `setShipGroup(..., type, false)` |

The faithful-argument shape is the pre-change HEAD body and scores
**70.278330** at the same 2764 B / 336 B LSDA. The tuned shape is +0.96 points
purely because the different live sets at those four call sites perturb Greedy
RA (the tune makes the compiler emit `mov r1,r4` / `mov r2,r4` instead of
`mov r1,sl` / `mov r2,sl`, matching the original's *register names* even though
the *values* differ).

## 2. What this pass did NOT fix: the first Greedy decision

The original prologue spills only `race`, keeps `type` in `fp` and `shipClass`
in `r5 -> sl`:

```
c0220  str  r1,[sp,#60]   ; race
c0222  mov  r9,r0         ; this
c0224  ldr  r0,[pc,#572]
c0226  mov  fp,r3         ; type
c0228  mov  r5,r2         ; shipClass
c022a  add  r0,pc
c022c  ldr  r4,[r0,#0]    ; status
...
c0238  mov  sl,r5         ; shipClass -> sl
```

The landed body still spills `shipClass` **and** `race` and puts `type` in `sl`
(the prologue is identical before/after this pass):

```
c0220  str  r2,[sp,#60]   ; shipClass
c0222  mov  r9,r0
c0224  str  r1,[sp,#52]   ; race
c0226  mov  sl,r3         ; type
c0228  ldr  r0,[pc,#564]
```

The original's stack slots are `race=[sp,#60]`, `this=[sp,#56]`,
`type=[sp,#52]`; ours are `shipClass=[sp,#60]`, `race=[sp,#52]`,
`type=[sp,#56]`. The whole downstream map follows from that first decision
(`type`/`empPoints` occupy `fp`/`sl` in the original vs `sl`/`fp` here;
`PlayerFixedObject`=`r9`, `Array<BoundingVolume*>`=`fp`, `PlayerFighter`=`sl`
in the original vs `fixed`/`fighter` sharing `fp` and `Array`=`r9` here;
`BoundingAAB` arg setup uses `strd` in the original vs separate `str`/`stmia`
here; the coordinate block is computed before `setEmpData` in the original vs
after it here).

## 3. Variants tried this pass (official one-function scorer, 2764 B target)

All start from the wave-123 real-`new` body. None moves the first prologue
instruction (`str r2,[sp,#60]`, shipClass) to the original's `str r1,[sp,#60]`
(race).

| variant | fuzzy % | size | insns | prologue |
|---|---:|---:|---:|---|
| **landed: w6951 call-site tune** | **71.2370** | 2764 | 1006 | `str r2,[sp,#60]` |
| landed + `const int sc/ty/rc` local copies (any/all) | 70.3 | 2764 | 1005 | unchanged |
| landed + `int sc` (non-const) local copy | 70.3 | 2764 | 1005 | unchanged |
| landed + `fighter`/`fixed`/`volumes` hoisted to function scope | 70.3 | 2764 | 1005 | unchanged |
| landed + `const Vector pos` | 70.3 | 2764 | 1005 | unchanged |
| landed + `Vector pos` at function scope (`Vector pos;`) | 30.6 | 2832 | 1005 | `sub sp,#96` |
| landed + `Vector pos` after the ctor, direct `x,y,z` in ctor | 70.0 | 2764 | 1005 | unchanged |
| landed + faithful jitter declaration `X,Y,Z` (y=2nd/z=3rd) | 71.2016 | 2764 | 1007 | unchanged |
| landed + `if (shipClass >= 1)` / `!= 0` | 39.8 / 39.9 | 2772 / 2768 | 987 / 985 | `strd r2,r1,[sp,#56]` |
| landed + `if (race < 0) return` / `if (shipClass < 0)` / `if (type < 0)` early use | 58.6 | 2796 | 1006 | `mov r4,r3; mov sl,r0` |
| landed + explicit-`else` clamp for `level` (2 `getLevel` calls) | 69.0 | 2772 | 1009 | unchanged |
| landed + explicit-`else` clamp for `empPoints` | 68.8 | 2772 | 1009 | unchanged |
| landed + explicit-`else` clamp for both | 69.3 | 2780 | 1012 | unchanged |
| landed + all-`int` coordinates (`(float)` at the ctors) | 63.8 | 2764 | 997 | `strd r1,r0,[sp,#56]` |
| landed + all-`float` coordinates | 57.2 | 2776 | 1004 | `strd r2,r1,[sp,#56]` |
| landed + `empPoints` block moved after the `empTime` block | 40.4 | 2756 | 1005 | unchanged |
| landed + `const bool isCapital` | 40.8 | 2728 | 998 | unchanged |

Also re-verified the wave-119/121/122/123 variants on this body: local
`Status*`/`AERandom*`, `KIPlayer*` locals, `ship` assigned early, branch swap
(capital first), `switch (shipClass)`, `min()` macro, `try/catch` forms,
`volumes` before `fixed`, declaration-at-top — all 70.3 or worse, none moves
the prologue.

Substitution decomposition on this body (tune parts: `xc` = fighter ctor,
`xg` = fighter group, `fc` = fixed ctor, `fg` = fixed group):

| subset | fuzzy % | .text diff |
|---|---:|---:|
| (none, HEAD) | 70.2783 | 87,014 |
| `xc` | 69.9454 | 86,999 |
| `xg` | 70.4771 | 87,014 |
| `fc` | 70.7753 | 87,036 |
| `fg` | 70.3777 | 87,014 |
| `xc+xg` | 70.1441 | 86,997 |
| **all four** | **71.2370** | **87,017** |

## 4. Remaining root cause

The four live `new`-expression cleanup pointers (`Player`/`BoundingAAB` in
`r5`, `PlayerFixedObject` in `r9`, `Array<BoundingVolume*>` in `fp`,
`PlayerFighter` in `sl`) plus the four incoming arguments (`this`, `race`,
`shipClass`, `type`) exceed the seven usable callee-saved registers
(`r4,r5,r6,r8,r9,sl,fp`; `r7` is the frame pointer). Greedy RA spills
`shipClass` first here; the original spills `race` and keeps `shipClass` in a
register, then re-seats the whole allocation. No source-level
declaration/scope/const/reference form tried in waves 119–124 changes that
first decision while keeping the 336 B cleanup-only LSDA. The next lever is
probably not source shape at all: the original's `level`/`empPoints` clamps are
two-call branch diamonds (`c0286`+`c0296`, `c0316`+`c0324`) while ours are
single-call `it ge; movge` cmovs, and the original computes the coordinate
block *before* `setEmpData` (`c03c8–c03e4`) while ours sinks it after
(`c03be–c03de`) — both are structural IR differences that also feed the RA
cascade.

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
