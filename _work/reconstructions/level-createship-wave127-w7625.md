# `Level::createShip` — wave 127 (w7625): EH shape already exact; the residual is pure RA

Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`, original
2764 B @ `0xc0214`, Thumb entry `0xc0215`, 1007 insns).
Base `4bbe88776` (wave-126 final). Worktree `/home/debian/GalaxyOnFire2/wt/w7625`.
Owned TU: `src/game/world/Level.cpp` (createShip only).

**Status: analysis only — no `src/` change landed.** The checked-in body stays
the best at **70.5000 %**, 2764 B, 1006 insns, exact 336 B / 57-row / `ttype
0xff` cleanup-only LSDA.  This pass confirms the wave-126 exception-pointer
finding (original `r8`, ours `r6`) and adds the missing piece of evidence: the
original and our LSDA **call-site tables are structurally identical** (same 57
rows, same `ttype 0xff`, same start/length pattern), so the EH/cleanup shape is
already right and the residual is entirely register allocation.

## Result

```
                                   original        HEAD (4bbe88776)
createShip fuzzy (official)        —               70.5000
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

## 1. The wave-126 finding is correct (and the tail listing is right)

Original landing-pad tail (force-thumb disassembly of the original at
`0xc0b8a`..`0xc0bd6`):

```
c0b8a..c0bb8  24 x b c0bcc           ; BoundingAAB cleanups -> player pad
c0bba         mov r8,r0; mov r0,sl; b c0bd0   ; fighter (sl)
c0bc0         mov r8,r0; mov r0,fp; b c0bd0   ; volumes (fp)
c0bc6         mov r8,r0; mov r0,r9; b c0bd0   ; fixed (r9)
c0bcc         mov r8,r0; mov r0,r5            ; player / BoundingAAB (r5)
c0bd0         blx operator delete
c0bd4         mov r0,r8
c0bd6         blx _Unwind_Resume
```

Ours is identical except the register and one pad:

```
c0b8e..c0bbc  24 x b c0bcc           ; BoundingAAB cleanups -> player pad
c0bbe         mov r6,r0; mov r0,r9; b c0bd0   ; fixed (r9)
c0bc4         b c0bc6                         ; empty pad (fighter cleanup)
c0bc6         mov r6,r0; mov r0,fp; b c0bd0   ; volumes (fp)
c0bcc         mov r6,r0; mov r0,r5            ; player / BoundingAAB (r5)
c0bd4         mov r0,r6
```

So the original's exception pointer is `r8`; ours is `r6`.  The original's
fighter cleanup actually deletes `sl`; ours is an empty pad (the `sl` delete
was folded away because our `fighter` value is already in `sl` and the
allocator chose to merge the pad with the volumes pad).  `r6` in the original
is free for the `shipClass` third piece (`mov r6,sl` @ `c02b4`); ours spends
`r6` on the exception PHI, so `shipClass` is spilled instead.

## 2. New evidence: the LSDA is already structurally exact

Parsing both `.ARM.extab` records (`_work/reconstructions/exidx_table.py`,
`parse_lsda`) gives:

```
orig : extab=0x1ab09c  ttype=0xff  cs=0x01  rows=57
ours : extab=0x1ab0bc  ttype=0xff  cs=0x01  rows=57
```

The row count, `ttype`, `callSiteEncoding` and the start/length pattern are
the same shape in both (e.g. the first cleanup row is `start=0x196 len=0x1e`
orig / `start=0x1ba len=0x1e` ours; the `new Player` row is `start=0x212
len=0x20` / `start=0x222 len=0x1c`; the fighter row is `start=0x4c8 len=0x20`
/ `start=0x4d0 len=0x22`).  Only the offsets differ, and only because the code
layout/RA differs.
**Conclusion: there is no EH-shape work left to do.**  The remaining delta is
the register allocation of the exception PHI vs `shipClass`.

## 3. Stack-slot map: the original keeps `this`/`type` live, we spill them early

```
original:  race = [sp,#60]   this = [sp,#56]   type = [sp,#52]
ours:      shipClass = [sp,#60]   race = [sp,#52]   type = [sp,#56]
```

Original `str r1,[sp,#60]` at `c0220` spills **race**; `this` stays in `r9`
until `c0332`, `type` stays in `fp` until `c0336`.  Ours spills **shipClass**
at `c0220`, spills `type` at `c031c` and `this` at `c030a`.  The original
therefore runs with all seven callee-saved registers occupied at `c0262`
(`r4=status, r5=rnd, r6=nextInt, r8=mission, r9=this, sl=shipClass, fp=type`);
ours leaves `fp` free and spills two arguments.  This is the same cascade the
wave-125/126 notes describe, seen from the stack-slot side.

## 4. Variants measured this pass (official one-function scorer)

All rows keep the exact 336 B / 57-row / `ttype 0xff` LSDA unless noted.
Everything below is a regression; the checked-in body is a local maximum.

| variant | % | size | insns | note |
|---|---:|---:|---:|---|
| **HEAD** | **70.5** | 2764 | 1007/1006 | best |
| `try { body } catch (...) { }` | 23.5 | 2816 | 1007/1007 | prologue `str r1,[sp,#60]` (race) matches, but `type`->r6 and `this` spilled; `ttype 0x00` |
| remove `Globals::status == nullptr` guard (faithful: original has no such `cmp`) | 63.6 | 2748 | 1007/988 | original really has no status-null check (`cbz` list: only waypoint/gameWon/hardCore/shipClass/cargo) |
| coords computed before `new Player` | 62.8 | 2768 | 1007/998 | shortens the PHI range but reorders the entry schedule |
| two-call `MIN(getLevel(),20)` diamonds (level + empPoints) | 68.6 | 2780 | 1007/1012 | faithful wave-125 shape |
| two-call MIN + no guard | 67.0 | 2764 | 1007/994 | |
| two-call MIN + coords first | 58.2 | 2780 | 1007/1011 | |
| `switch (shipClass)` for the ship branch | 41.2 | 2768 | 1007/989 | |
| `int empTime = shipClass == 1 ? 45000 : 15000;` + separate `if` | 61.5 | 2784 | 1007/996 | |
| empTime block before empPoints | 61.5 | 2784 | 1007/996 | |
| `int sc = shipClass;` used everywhere | 70.5 | 2764 | 1007/1006 | coalesced, no change |
| drop `Vector pos`, pass `(float)x,y,z` directly | 70.2 | 2764 | 1007/1006 | |
| `Vector pos` hoisted to function top | 70.5 | 2764 | 1007/1006 | |
| all-`int` coords (faithful) | 61.5 | 2772 | 1007/994 | |
| all-`float` coords | 57.2 | 2776 | 1007/1004 | |
| local `Array<int> guard;` destructor at top | 23.1 | 2852 | 1007/1028 | adds ctor/dtor calls + a destructor pad; `ttype` changes |
| `if (shipClass != 1)` instead of `== 0` | 62.1 | 2740 | 1007/988 | |

## 5. Remaining lever

The original's exception PHI is `r8` because `r6` is occupied by the
`shipClass` third piece; ours is `r6` because the PHI is the fourth-largest
live range (span ~22 kB, def at MIR 4368 B, use at 26352 B) and is enqueued
before `shipClass` (span 6592 B).  To flip it, the PHI's *pre-RA* live range
must shrink below `shipClass`'s, i.e. the cleanup landing-pad blocks must be
emitted at the end of the IR block order.  A plain `try`/`catch` does move
them, but necessarily introduces a catch-all type table (`ttype 0x00`) and a
catch handler, so it cannot be the original shape.  No source spelling tried
in waves 119–127 both moves the pads and keeps the 336 B cleanup-only LSDA.

## 6. Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
```

Scratch helpers (not committed): `/dev/shm/w7625/{score.py,body_*.txt,apply.py,
build_score.sh,lsda.py,orig.dis,ours.dis,mir.txt,Level.ll}`.
