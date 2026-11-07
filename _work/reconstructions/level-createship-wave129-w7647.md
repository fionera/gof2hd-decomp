# `Level::createShip` — wave 129 (w7647): exception-pointer live-range sweep (no source win)

Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`, original
2764 B @ `0xc0214`, Thumb entry `0xc0215`, 1007 insns).
Base `fee528cbe` (wave-128 final). Worktree `/home/debian/GalaxyOnFire2/wt/w7647`.
Owned TU: `src/game/world/Level.cpp` (createShip only).

**Status: analysis only — no `src/` change landed.**  The checked-in body stays
the best at **70.5 %**, 2764 B, 1007/1006 insns, exact 336 B / 57-row /
`ttype 0xff` cleanup-only LSDA.  This pass attacks the wave-126/127
exception-pointer finding directly: it confirms the mechanism (original
exception pointer = `r8`, ours = `r6`) and shows that every source shape which
flips the pointer to `r8` also destroys the surrounding schedule, so the net
fuzzy score falls.  The checked-in body remains a local maximum.

## Result

```
                                   original        HEAD (fee528cbe)
createShip fuzzy (official)        —               70.5
createShip size                    2764 B          2764 B
createShip insns (orig/ours)       1007            1007 / 1006
createShip LSDA record             336 B           336 B
createShip LSDA rows               57              57
createShip LSDA ttype              0xff            0xff
byte_exact / linked_exact          —               4506 / 4506
data / imports / lint / drift      —               843 / 207 / CLEAN / 0
```

`check` on the unchanged tree: **RATCHET PASS** — `verify: byte 4506 linked
4506 stubs 0 missing 0 extra 0 avg 99.94`, `data: exact 843 of 843`,
`sodiff: imports 207`, lint `CLEAN`, drift 0.

## 1. The mechanism is confirmed: `r6` vs `r8`

The original's cleanup landing-pad tail keeps the exception pointer in **r8**;
ours keeps it in **r6**:

```
original                         ours
c0bba mov r8,r0; mov r0,sl       mov r6,r0; mov r0,r9
c0bc0 mov r8,r0; mov r0,fp       (empty pad)
c0bc6 mov r8,r0; mov r0,r9       mov r6,r0; mov r0,fp
c0bcc mov r8,r0; mov r0,r5       mov r6,r0; mov r0,r5
c0bd0 blx operator delete        blx operator delete
c0bd4 mov r0,r8                  mov r0,r6
c0bd6 blx _Unwind_Resume         blx _Unwind_Resume
```

The MIR (`-mllvm -print-machineinstrs`) shows the exception pointer is the
coalesced PHI `%676` / `%125`: defined at the **first** landing pad
(`bb.19`, MIR offset 4336 B) and used at the resume block (`bb.100`, MIR
offset 26352 B).  Its live interval therefore spans almost the whole function.
`r8` is occupied over that span by `mission` (then `z`), so Greedy RA cannot
give it `r8` and picks the next free callee-saved register, `r6`.  `r6` is
then unavailable for the `shipClass` third piece (`mov r6,sl` @ `c02b4` in the
original), so `shipClass` is spilled to `[sp,#60]` and `type` moves to `sl`.
That single decision is the whole prologue cascade.

The original's exception pointer is short-lived: the landing pads are laid out
*after* the main body (`c0b8a`..`c0bd6`, after the `pop` at `c0b88`), and `r8`
is free there because `z` dies at `c0ad4`.  So in the original the RA-time
block order must have the landing pads at the end; in ours they are
interleaved with the main body.

## 2. What was tried this pass

All rows keep the exact 336 B / 57-row / `ttype 0xff` LSDA unless noted.
`exreg` is the exception-pointer register read from the final disassembly.
Full-link, one-function scorer.

| variant | raw % | size | insns | exreg | note |
|---|---:|---:|---:|---|---|
| **HEAD** | **70.5** | 2764 | 1007/1006 | r6 | best |
| coords before `new Player` | 62.8 | 2768 | 1007/998 | **r8** | flips pointer, wrecks prologue |
| coords before player, all-int | 61.5 | 2772 | 1007/994 | r6 | |
| coords before player, `f f i` | 62.0 | 2784 | 1007/1001 | **r8** | |
| coords before player, `f i f` | 61.3 | 2768 | 1007/998 | **r8** | |
| dummy `if (mission < 0) return nullptr;` after guard | 48.0 | 2756 | 1007/1002 | **r8** | flips pointer only |
| `if (mission == 0) return nullptr;` guard | 59.8 | 2772 | 1007/1002 | **r8** | |
| `if (missionPtr == 0) return nullptr;` guard | 49.3 | 2788 | 1007/1009 | **r8** | |
| guard `== 0` / `!status` / braces / `nullptr ==` | 70.5 | 2764 | 1007/1006 | r6 | identical |
| guard at top / after mission / after coords / before emp / none | 57.9 / 58.7 / 63.1 / 63.1 / 63.6 | 2764-2788 | | r6 | |
| `LEVEL_MIN` (two-call) for level + empPoints | 68.6 | 2780 | 1007/1012 | r6 | faithful, worse |
| `LEVEL_MIN` + all-int coords + no guard (faithful body) | 60.8 | 2792 | 1007/1009 | r6 | faithful, much worse |
| `LEVEL_MIN` + `f f i` | 63.7 | 2784 | 1007/1013 | **r8** | |
| `LEVEL_MIN` + `f i f` | 67.4 | 2780 | 1007/1011 | r6 | |
| `LEVEL_MIN` + coords before player | 58.2 | 2780 | 1007/1011 | **r8** | |
| coord type sweep `iii/fff/fii/iif/ifi/ffi/fif` | 57.2-69.3 | 2764-2788 | | r6/r8 | `fif` 69.3 best |
| coord order sweep (all 6 permutations of `x/y/z`) | 70.3-70.5 | 2764 | 1007/1006 | r6 | `z y x`, `x z y`, `z x y` tie |
| `switch (type)` case order `33,31,2c` | 70.5 | 2764 | 1007/1006 | r6 | no change |
| `switch (type)` case order `31,33,2c` | 70.1 | 2764 | 1007/1006 | r6 | |
| drop `Vector pos`, pass floats directly | 70.2 | 2764 | 1007/1006 | r6 | |
| `Player *player;` declared then assigned | 70.5 | 2764 | 1007/1006 | r6 | no change |
| extra `{ }` around ship branch / whole body | 70.5 | 2764 | 1007/1006 | r6 | folded away |
| `try { setEmpData } catch(...) { __builtin_trap(); }` | 67.0 | 2752 | 1007/1004 | r6 | |
| `try { body } catch(...) { }` | 23.5 | 2816 | 1007/1013 | r5 | ttype 0x00 |

## 3. Why the pointer flip never wins

Every variant that moves the exception pointer to `r8` does so by changing the
IR block order (moving the coordinate block before `new Player`, or inserting
an extra early-return block after the guard).  That reorders the entry
schedule at the same time, so the prologue no longer matches:

```
original:  str r1,[sp,#60]   mov r9,r0   ldr r0,[pc,#572]  mov fp,r3  mov r5,r2
coords-first variant:
           str r2,[sp,#60]   mov r5,r3   str r0,[sp,#32]   mov r9,r1  ldr r0,[pc,#568]
```

The original needs *both* the `r8` exception pointer *and* the entry schedule
`race=[sp,#60], this=r9, type=fp, shipClass=r5`.  No source spelling tried in
waves 119-129 produces both.  The remaining lever is still a source shape that
makes clang emit the cleanup landing pads at the end of the IR block list
(so `%676`'s live interval no longer overlaps `z`) while leaving the entry
block order untouched.

## 4. IR evidence: landing pads are created at the end of their compound stmt

`-mllvm -print-after-all` on the checked-in body shows the first landing pad is
`%220` (the `new Player` cleanup), placed after `%219` (the
`if (Globals::status == nullptr) return nullptr;` early-return block).  The
remaining pads follow at `%285`, `%351`, `%355`, `%508`, ... i.e. interleaved
with the main body.  In a minimal straight-line `new A(); new B();` example
clang groups the pads after the whole compound statement; with a branch between
the `new`s it interleaves them, exactly as here.  This is why the landing-pad
position is coupled to the early-return/scope structure.

## 5. Reproduction

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

MIR (exception-pointer interval and landing-pad order):

```
<clang++> ... -c src/game/world/Level.cpp \
  -mllvm -print-machineinstrs -mllvm -filter-print-funcs=_ZN5Level10createShipEiiiP8Waypointbb
```

Scratch helpers (not committed): `/dev/shm/w7647/{score2.py,gen.py,gen2.py,
gen3.py,gen4.py,sweep.py,sweep2.py,sweep3.py,sweep4.py,lsda.py,mir.sh,irpass.txt}`.
