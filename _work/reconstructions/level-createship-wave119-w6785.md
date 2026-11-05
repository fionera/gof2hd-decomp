# Level::createShip — wave 119 (w6785): real-new RA sweep, 67.4 is a hard local optimum

**Status:** analysis only, no source change landed (2026-11-05).
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`,
original 2764 B @ `0xc0214`, Thumb entry `0xc0215`). Branch
`wave119-w6785`, base `109d153e9`. Worktree left clean; only this note is
committed.

## Result

```
createShip fuzzy (raw)            67.400000  (unchanged)
createShip size                   2764 B      (unchanged)
createShip LSDA record            336 B / 57 sites / TType 0xff  (exact)
.ARM.extab size / addr            0x26294 (original 0x26298; Generator -4)
.text                             0x1337c8    (exact)
byte_exact / linked_exact         4505 / 4505
imports / data / lint / drift     207 / 843 / CLEAN / 0
```

`check` (final): **RATCHET PASS**.  ~150 new source variants were built
and scored with the official verifier.  None beats **67.4 %**; the best
non-base variant ties it.  The 336-byte LSDA is preserved by every
variant below (real `new`, no `try`/`catch`).

## Why 67.4 is the ceiling here: the EH pads pick a different Greedy RA

`-mllvm -print-after-all` on a one-function mini TU (same headers/flags;
the mini object reproduces the linked function byte-for-byte) gives the
Virtual-Register-Rewriter prologue for the two shapes:

```
real new (HEAD)                     placement new (no EH pads)
$r10 = COPY $r3   ; type            $r10 = COPY $r3   ; type
str  $r2, stack.3 ; shipClass       $r5  = COPY $r2   ; shipClass
str  $r1, stack.2 ; race            str  $r1, stack.2 ; race
$r9  = COPY $r0   ; this            $r9  = COPY $r0   ; this
$r6  = status                       $r4  = status
$r4  = nextInt fn ptr               $r6  = nextInt fn ptr
$r11 = jitterX                      ...
```

The placement-new shape reproduces the original prologue exactly
(`str r1,[sp,#60]`, `mov fp,r3`, `mov r5,r2`, status in r4, nextInt fn
ptr in r6) and scores 72.7 % with the faithful spawn.  Real `new` adds the
28 cleanup landing pads; their exception-pointer vreg is allocated to
**r6** in our build (original: **r8**), which pushes the nextInt function
pointer to r4 and status to r6, and the whole allocation cascades:

| value | original | ours (real new) |
|---|---|---|
| status | r4 | r6 |
| nextInt fn ptr | r6 | r4 |
| shipClass | r5 -> sl | spilled (sp#60) |
| type | fp (r11) | sl (r10) |
| ship / fixed | r9 | fp |
| volumes (Array) | fp | r9 |
| fighter | sl | fp (shared with fixed) |
| `this` | spilled (sp#56), reloaded per branch | kept in sl |
| landing-pad exception ptr | r8 | r6 |
| delete stubs | sl, fp, r9, r5 (4) | r9, fp, r5 (3) |

The original spills `this` at `0xc0332` (after the `empPoints` merge,
before the `shipClass` branch) and reloads it at `0xc071e`, `0xc0b5e`,
`0xc0b74`; ours keeps `this` in `sl` and therefore cannot give `sl` to
the fighter, `r9` to `ship`, and `fp` to `volumes`.  The original's
landing pads delete four distinct registers (`sl` fighter, `fp` volumes,
`r9` fixed, `r5` player/BoundingAAB); ours only three because fixed and
fighter coalesce through `ship` into `fp`.  The LSDA call-site table is
identical (57 sites, 28 with landing pads, all action 0), so this is
purely a Greedy-RA tie-break, not a missing cleanup.

The literal pools are exact: both sides have 121 referenced pool entries
and the multisets are identical, so no constant/spawn value is wrong.

## Measurement table (official scorer, full link; all rows 2764 B unless noted)

`iff` = int x / float y / float z (HEAD).  All rows real `new`, no
`try`/`catch`, faithful spawn unless marked tuned.

| variant | raw % | note |
|---|---:|---|
| **HEAD: iff, wp `wy,wz,wx`, jitter XYZ** | **67.4** | base |
| iff, jitter XZY | 67.4 | tie |
| iff, wp `wy,wz,wx` × jitter XYZ/XZY | 67.4 | best of 36 wp×jit combos |
| iff, wp `wx,wy,wz` / `wx,wz,wy` / `wy,wx,wz` | 67.2 | |
| iff, wp `wz,wy,wx` | 67.0 | |
| iff, wp `wz,wx,wy` | 66.8 | |
| iff, jitter YXZ/YZX/ZXY/ZYX | 67.3 | |
| tuned spawn (`x=jz+jx`, `z=wz+wx`, group) | 67.3 | |
| `fif` (float x) | 66.3 | |
| `ffi` (float x,y) | 65.6 | |
| `iif` (int x,y) | 64.0 | 2780 B |
| `iii` (all int) | 63.1 | 2792 B |
| `fii` | 61.5 | 2784 B |
| `ifi` | 63.0 | 2780 B |
| `fff` (all float) | 56.9 | 2784 B |
| all-int, coords before `new Player` | 62.3 | 2792 B |
| all-int, coords after `setEmpData` | 63.1 | 2792 B |
| all-int tuned × 6 jitter orders | 63.4-63.5 | 2792 B |
| coords before `new Player` (mixed) | 62.2 | 2788 B |
| coords after `setEmpData` (mixed) | 67.4 | tie |
| coords right after jitter | 59.5 | 2772 B |
| x right after jitterX | 64.8 | 2772 B |
| hitpoint switch `0x2c,0x31,0x33` / `0x2c,0x33,0x31` | 67.4 | tie |
| hitpoint switch other 4 orders | 67.0-67.3 | |
| hitpoint if-chain | 67.4 | tie |
| mission 49..56 as range / switch / reversed or | 67.4 | tie |
| cargo test bitwise `|` / switch / `0x31||0x2c` | 67.4 | tie |
| `missionPtr` bitwise `&` | 67.4 | tie |
| `empTime` ternary | 66.4 | |
| `empTime` computed late (before setEmpData) | 35.3 | 2772 B |
| `empPoints` via `empLevel` local | 67.4 | tie |
| `empPoints` if-form | 64.5 | 2756 B |
| `armor` if-form / early | 67.4 / 63.3 | |
| `cls` copy (top / after wp / dispatch-only) | 67.4 | tie |
| `bool` flags (early / dispatch-only) | 67.4 | tie |
| `Level *self` alias (all / branches / end) | 67.4 | tie |
| `this->` explicit | 67.4 | tie |
| `ship` declared top / before player / after player | 67.4 | tie |
| `ship` as `PlayerFixedObject *` + casts | 47.6 | 2752 B |
| `ship` uninitialised | 65.4 | 2760 B |
| `fixed`/`fighter` as `KIPlayer *` + casts | 67.4 | tie |
| `fixed`/`fighter`/`volumes` declared at top | 67.4 | tie |
| `fixed`/`fighter`/`volumes` decl-then-assign | 67.4 | tie |
| `ship = fixed/fighter` moved early | 67.4 | tie |
| capital branch first / nested | 31.2 | 2784 B |
| `goto` join | 67.4 | tie |
| early return from fighter branch | 64.6 | 2760 B |
| `status` local | 61.7 | 2768 B |
| `rnd` local | 64.2 | 2768 B |
| both locals | 61.0 | 2764 B |
| `LODManager *lod` local (early / late) | 29.2 / 18.4 | 2752/2756 B |
| `int mp = missionPtr` local | 37.8 | 2760 B |
| `volatile Level *self` | 15.0 | 2812 B |
| dummy live value (type/race/shipClass/waypoint) | 24.0-33.2 | |
| `__builtin_expect` on dispatch | 34.5 / 18.6 | |
| zero arg as `0` / `0.f` / `0.0` | 67.4 | tie |
| `static_cast` / `reinterpret_cast` return | 67.4 | tie |
| real new + `try` around setEmpData | 63.5 | 2768 B, LSDA 0x262a4 |
| real new + dummy try (no calls) | 67.4 | tie, LSDA 0x26294 |
| placement new, faithful (no try) | 72.7 | 328 B LSDA |
| placement new + try/trap (wave-117 shape) | 74.4 | 328 B LSDA |

## Where the 336 B LSDA comes from (verified by parsing `.ARM.exidx`)

Original record @ `0x1ab09c`: personality + 12 bytes of unwind opcodes,
LSDA at +0x10 with `LPStart=0xff`, `TType=0xff`, call-site enc `0x01`,
table length 315, **57 sites, every action 0**, empty action table.
The 28 sites with a landing pad are exactly:

```
0xc03aa Player ctor        -> c0bcc (delete r5)
0xc0426 PlayerFixedObject  -> c0bc6 (delete r9)
0xc044e Array ctor         -> c0bc0 (delete fp)
0xc06dc PlayerFighter      -> c0bba (delete sl)
24 x BoundingAAB ctor      -> c0b8a..c0ba4 trampolines -> c0bcc (delete r5)
```

So the original really does use real `new` for all three ship objects,
the `Array`, and all 24 `BoundingAAB`s; the 336-byte record is the
signature of that shape and is reproduced exactly by HEAD.

## Remaining `.ARM.extab` delta

Only `Generator::createMission` (172 vs 168) — different TU, out of
scope.  With it exact the section would be `0x26298` (original).

## Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 tools/verify/verify.py \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5Level10createShipEiiiP8Waypointbb
cmake --build cmake-build-match --target check -j6
```

Scratch helpers (not committed): `/dev/shm/w6785/fastscore.py`
(single-function official scorer), `mini_score.py` / `sweep*.py`
(one-TU fast RA harness), `gen*.py` (variant generators), `lsda.py`
(LDSA call-site parser).
