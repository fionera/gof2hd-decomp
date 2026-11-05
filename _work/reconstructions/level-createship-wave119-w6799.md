# Level::createShip — wave 119 (w6799): a +16 B null-check shape reaches 68.3 %

**Status:** analysis only, no `src/` change landed (2026-11-05).
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`,
original 2764 B @ `0xc0214`, Thumb entry `0xc0215`). Branch
`wave119-w6799`, base `251d2c254`. Worktree left clean; only this note is
committed.

## Result

```
                                   baseline (HEAD)      best measured
createShip fuzzy (raw)             67.400000            68.300000
createShip size                    2764 B               2780 B  (+16)
createShip insns (orig/ours)       1007 / 994           1007 / 1012
createShip LSDA record             336 B (exact)        336 B (exact)
.ARM.extab size / addr             0x26294              0x26294
.text                              0x1337c8 (exact)     0x1337e8 (+0x20)
.ARM.exidx                         0x19cc08 / 0x6f70    0x19cc28 / 0x6f70
.rodata                            0x1c9e10 / 0x41c27   0x1c9e30 / 0x41c27
file size                          2,187,328 (exact)    2,187,328 (exact)
byte_exact / linked_exact          4505 / 4505          2372 / 4505
```

`check` on the unchanged tree: **RATCHET PASS** — `verify: byte 4505
linked 4505 stubs 0 missing 0 extra 0 avg 99.94`, `data: exact 843 of
843`, `sodiff: imports 207`, lint CLEAN, drift 0.

The 68.3 % shape is **not landable**: its +16 B function body pushes
`.text` from `0x1337c8` to `0x1337e8` (the section grows by 0x20, not 0x10;
the file size itself is unchanged because the tail is 256-byte padded),
which moves `.ARM.exidx`, `.ARM.extab` and `.rodata` by +0x20 and breaks
the PC-relative literal encodings of ~2133 unrelated functions
(`byte_exact 4505 -> 2372`, ratchet FAIL).  Because the gate is
`byte_exact >= 4505`, any size change in this function is fatal; the only
landable shapes are exactly 2764 B.

## The 68.3 % variant (best measured, +16 B)

Inserted immediately before `KIPlayer *ship = nullptr;`:

```cpp
    if (Globals::status == nullptr) return nullptr;
    KIPlayer *ship = nullptr;
```

`!Globals::status`, `Globals::status == 0` and placing the statement after
`ship = nullptr` all give the identical 68.3 % / 2780 B shape.
`if (Globals::status == nullptr) return (PlayerFixedObject *) ship;` also
ties (the compiler still materialises `movs r0,#0`).

## What the null check changes (the useful part)

The extra late read of `Globals::status` lengthens the `&Globals::status`
vreg (`%139` in the Greedy dump) and moves the whole allocation one step
toward the original:

```
orig  c0220 str r1,[sp,#60]        ; race spilled
      c0222 mov r9,r0              ; this
      c0224 ldr r0,[pc,#572]
      c0226 mov fp,r3              ; type -> fp
      c0228 mov r5,r2              ; shipClass -> r5
      c022a add r0,pc
      c022c ldr r4,[r0,#0]         ; &Globals::status -> r4
      c022e ldr r0,[r4,#0]
      c0230 blx getCurrentCampaignMission

HEAD  c0220 strd r1,r2,[sp,#56]    ; race AND shipClass spilled
      c0222 mov r9,r0
      c0224 ldr r0,[pc,#576]
      c0226 mov sl,r3              ; type -> sl
      c0228 add r0,pc
      c022a ldr r6,[r0,#0]         ; &Globals::status -> r6
      c022c ldr r0,[r6,#0]
      c022e blx

+null c0220 str r2,[sp,#60]        ; shipClass spilled
      c0222 mov r9,r0
      c0224 str r1,[sp,#52]        ; race spilled
      c0226 mov sl,r3              ; type -> sl
      c0228 ldr r0,[pc,#576]
      c022a add r0,pc
      c022c ldr r4,[r0,#0]         ; &Globals::status -> r4  (matches orig)
      c022e ldr r0,[r4,#0]
      c0230 blx
```

So the null check buys `status -> r4` (the original's first free register)
and makes all three `nextInt` results spill (the original spills all
three), but it still leaves `type -> sl` (original `fp`) and `shipClass`
spilled (original `r5`).  The remaining gap is the same one documented in
`level-createship-wave119-w6785.md`: the original's `shipClass` is
evicted `r5 -> sl -> r6` while ours is spilled, and that only happens
when `type` is in `fp` (freeing `sl` for the eviction).

A useful diagnostic: `if (Globals::status != nullptr) { <dispatch> }`
(one branch around the whole `shipClass` dispatch) also gives
`status -> r4` **at exactly 2764 B** (`wrap_status`, 64.0 %), but it
assigns `race -> r5` / `shipClass -> sp#60` — i.e. exactly the original's
`race`/`shipClass` roles swapped — and the extra dispatch branch costs
3.4 points.  This confirms the `r4` shift is achievable at zero size, but
the `r5`/`sl` half of the cascade needs `shipClass` to outrank `race` in
spill weight, which no source-level rewrite found here did.

## Measurements this session (official scorer, full link)

All rows are real `new`, no `try`/`catch`, 336 B LSDA, unless noted.
`nc` = the `if (Globals::status == nullptr) return nullptr;` line above.

| variant | raw % | size |
|---|---:|---:|
| **HEAD (baseline)** | **67.4** | 2764 |
| **`nc` before `ship`** | **68.3** | **2780** |
| `nc` after `ship` / `!status` / `== 0` / before dispatch | 68.3 | 2780 |
| `nc` + `empTime=15000` init form | 68.3 | 2780 |
| `nc` + `Player *player; player = new Player(...)` | 68.3 | 2780 |
| `nc` + `int cls = shipClass` copy | 68.3 | 2780 |
| `nc` + `armor` if-form | 68.3 | 2780 |
| `nc` + `float zero = 0.0f` BoundingAAB arg | 68.3 | 2780 |
| `nc` + `LODManager *lod = lodManager` | 68.3 | 2780 |
| `nc` + `int sc = shipClass` (empTime or dispatch only) | 68.3 | 2780 |
| `nc` + `0` / `0.f` / `(float)0` BoundingAAB arg | 68.3 | 2780 |
| `nc` + `gameWon` if-form | 68.0 | 2780 |
| `nc` + `int x,y,z` (`iii`) | 62.4 | 2788 |
| `nc` + `int x,int y,float z` (`iif`) | 61.6 | 2800 |
| `nc` + all-float coords (`fff`) | 58.4 | 2792 |
| `nc` + `int x, float y, float z` (`iff`, HEAD types) | 68.3 | 2780 |
| `nc` + `float x, int y, float z` (`fif`) | 67.2 | 2780 |
| `nc` + `armor` inlined into `new Player` | 59.8 | 2796 |
| `nc` + uninitialised `ship` | 61.8 | 2796 |
| `nc` + `else` instead of `else if (shipClass == 1)` | 49.7 | 2784 |
| `nc` before `setEmpData` | 64.4 | 2788 |
| `nc` before `ship->setLevel` | 63.8 | 2776 |
| `nc` in fighter branch / capital branch | 62.6 / 62.8 | 2776 / 2768 |
| `nc` on `Globals::rnd` / `Globals::globals` / `waypoint` / `lodManager` | 61.2 / 42.9 / 60.2 / 50.3 | 2796 / 2768 / 2800 / 2804 |
| `if (Globals::status == nullptr) shipClass = 0;` | 58.3 | 2808 |
| `if (Globals::status != nullptr) { <dispatch> }` (`wrap_status`) | 64.0 | **2764** |
| `wrap_status` + `int sc = shipClass` copy | 64.0 | 2764 |
| `wrap_status` + `shipClass >= 0` in the guard | 49.7 | 2812 |
| `wrap_status` + `race >= 0` in the guard | 47.8 | 2812 |
| `else if (shipClass == 1 && Globals::status != nullptr)` | 63.4 | 2768 |
| `if (shipClass == 0 && Globals::status != nullptr)` | 37.3 | 2776 |
| `if (shipClass == 0) {...} if (shipClass == 1) {...}` (two ifs) | 25.1 | 2692 |
| `else { if (shipClass == 1) {...} }` (nested) | 67.4 | 2764 |
| `else if (shipClass >= 1)` / `> 0` | 62.7 | 2768 |
| `else if (shipClass != 0)` / `else` | 47.7 | 2756 |
| `else if (shipClass - 1 == 0)` | 36.6 | 2760 |
| `if (shipClass - 0 == 0)` | 67.4 | 2764 |
| `else if (shipClass == 1 && shipClass != 0 / >= 1 / < 2 / > -1 / == 1)` | 67.4 | 2764 |
| `int sc = shipClass` copy (empTime / dispatch / both) | 67.4 | 2764 |
| `int wx/wy/wz` direct ternary reads | 66.8 | 2764 |
| `int wx=0,wy=0,wz=0; if (waypoint) {...}` | 67.2 | 2764 |
| `(void) Globals::status;` / `Globals::status = Globals::status;` / empty ifs | 67.4 | 2764 |
| `if (player == nullptr) return nullptr;` (optimised away) | 67.4 | 2764 |
| `if (Globals::status != nullptr) ship = nullptr;` (optimised away) | 67.4 | 2764 |
| `ship->setLevel(Globals::status ? this : this)` (optimised away) | 67.4 | 2764 |
| `Status *st = Globals::status` for mission only / late only | 67.4 | 2764 |
| `Status *st` for the whole hitpoints block | 59.5 | 2796 |
| `unsigned mission` / `unsigned level` | 67.4 | 2764 |
| `mission >= 49 && <= 56 && != 53/54/55` mask form | 66.2 | 2764 |
| `bool hardcore = hardCoreMode()` | 67.4 | 2764 |
| `armor = 650 + hardCoreMode() * 350` | 67.2 | 2764 |
| `hitpoints = level*14 + 20 + (...)` reorder / `mission << 2` | 67.4 | 2764 |
| `armor` inlined into `new Player` | 62.9 | 2752 |
| `armor` inline + uninitialised `ship` | 62.8 | 2748 |
| `const float zero = 0.0f` BoundingAAB arg (any scope) | 67.4 | 2764 |

No variant at exactly 2764 B beat 67.4 %.  The 68.3 % shape is the only
measured improvement and it is 16 B too large.

## Why 67.4 is the landable ceiling

`-mllvm -print-machineinstrs=greedy` shows the spill decisions are
identical in HEAD and in the `nc` shape: `shipClass -> stack.3`,
`race -> stack.2`, `type` in a vreg.  The null check only changes the
*physical register* chosen for the `&Globals::status` vreg (`r6 -> r4`).
The original's remaining advantage needs the `shipClass` vreg to be
evicted `r5 -> sl -> r6` instead of spilled, which requires `type` to be
in `fp`; that only happens in shapes that change the block structure
(`two_ifs`, `wrap_status`) or the coordinate types, and those either
change the size (2788+) or lose more elsewhere than they gain.

## Structural state (unchanged)

27 sections, file size 2,187,328 B, `.text` `0x1337c8`, `.ARM.exidx`
`0x6f70`, `.ARM.extab` `0x26294` (original `0x26298`; the only remaining
short record is `Generator::createMission` in another TU), `.rodata`
`0x41c27` (original `0x41b67`; every byte before the fixed-address
`Status::missionCompleted` rank table at `0x20b977` is exact).
`createShip`'s own 336 B LSDA record (57 sites, TType `0xff`) is exact in
the baseline and in every variant above.

## Reproduction

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

Scratch helpers (not committed): `/dev/shm/w6799/{score,harness,probe,analyze,mnemo}.py`,
`s12.py`–`s40.py` (the sweeps above), `nc_fn.cpp`, `wrap_fn.cpp`,
`nc.asm` / `base.asm` / `nc_greedy.txt`.
