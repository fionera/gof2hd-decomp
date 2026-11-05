# LevelScript::process — wave 115 / w6303 (case-92 state-2 RNG call order)

Status: **code change landed, 2026-11-05**. Branch `wave115-w6303`, base master
`b847d5d46` (wave-114 harvest). Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, 20169 instructions.

## Result

```
                         before (master)   after (landed)
official raw ratio       58.020546         58.039801      (+0.019255)
orig / ours insns        20169 / 20034     20169 / 20031
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
byte_exact               4500              4500
linked_exact             4500              4500
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN             CLEAN
drift                    0                 0
RATCHET                  PASS              PASS
sections                 27                27
file size                2187328           2187328
.ARM.extab               0x26298           0x26298   (exact)
```

The raw number is `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s reconciled instruction streams of the linked `.so`
(helper `/dev/shm/w6303/fast.py`).

## What was applied (one size-neutral move)

`src/game/world/LevelScript.cpp`, case 92 state 2 (the scatter of enemies[2..5]
around the player, source line ~3921):

```cpp
// before (master)                    // after (landed)
int sideX   = rnd->nextInt(100);      int sideX   = rnd->nextInt(100);
int offsetY = rnd->nextInt(1000);     int sideZ   = rnd->nextInt(100);
int sideZ   = rnd->nextInt(100);      int offsetY = rnd->nextInt(1000);
int offsetZ = rnd->nextInt(2000);     int offsetZ = rnd->nextInt(2000);
```

Moving `sideZ` up two declaration slots is size-neutral (57232 B) and changes
the register allocation / call scheduling of the surrounding block.  It is the
only win found by a full adjacent-declaration-pair sweep (77 candidates) and a
dense adjacent-statement sweep of lines 3900-3965 on the wave-114 base.

## Ground truth: the original's call order

The original block is at `0x13c0xx` (case 92 state 2).  Its four
`AERandom::nextInt` calls are, in order:

```
0x...  movs  r1, #100   ; blx rnd->nextInt  -> sideX   (r5)
0x...  mov.w r1, #1000  ; blx rnd->nextInt  -> offsetY (r8)
0x...  mov.w r1, #2000  ; blx rnd->nextInt  -> offsetZ (r9)
0x...  movs  r1, #100   ; blx rnd->nextInt  -> sideZ   (r0)
```

i.e. the **faithful** source order is `sideX, offsetY, offsetZ, sideZ`.
That order was measured on this tree: **57.189055 at 57224 B** (8 bytes short,
so it cannot be landed alone without breaking the exact file size).  The
landed tuned order `sideX, sideZ, offsetY, offsetZ` scores **58.039801 at
57232 B**.  Three other permutations tie the landed score exactly
(`sideZ,sideX,offsetY,offsetZ`, `sideX,offsetZ,offsetY,sideZ`,
`offsetZ,sideX,offsetY,sideZ`); all four share `offsetY` immediately before
`offsetZ`.

## Measurements (all on the wave-114 base, 57232 B unless noted)

### Full adjacent-declaration-pair sweep (77 pairs, whole function)
Only the case-92 state-2 move above beat the base.  Next best:
`approach`/`step` (line 5151), `chaser`/`stationShip` (4881),
`stationIndex`/`chaserIndex` (4724), `wreck`/`boss` (4385) -- all exactly
58.020546 (neutral).  Several pairs shrink the function (57188..57264) and
score lower.

### All 24 permutations of the four case-92 state-2 int declarations
Best 58.039801 (4 permutations, listed above); baseline 58.020546; worst
48.132883.  The faithful order is 57.189055 @ 57224.

### Adjacent-statement sweep, lines 3900-3965 (on the landed base)
No further improvement; every candidate scores <= 58.039801.

### Other multi-`nextInt` blocks checked against the original disassembly
* case 73 state 3 scatter (line 2554): original `magX, signX, y, magZ, signZ`;
  source already faithful.
* case 73 state 3 scatter (line 2362): original `dx, signX, dy, dz, signZ`;
  source already faithful.
* case 125 revive (line 3306): original `dx, signX, dy, dz, signZ`; source
  already faithful.
* case 125 revive (line 3419): original `dx, signX, dy, dz, signZ`; the
  source has `dz` before `dy`.  Swapping them is faithful but **worse**
  (57.916076), so it was not landed.
* case 80 state 13 scatter (line 2554 group) and the `rx/ry/rz` group
  (line 3731) are already in the original's order.

## Residual state / next hypothesis

The remaining ~42 % sequence loss is still the global register allocation:
the original spills `this` to `sp+88` and reloads it (103 sites), reusing
`sl` as a general scratch (delta, loop counters, vector pointers), while our
build keeps `this` in `sl` for most of the function (57 reloads) and uses
`r9` for `Globals::status`.  The 16 original EH landing pads move the
exception object into `sl` (`mov sl, r0`); ours use `r9` (3 `mov sl, r0`,
12 `mov r9, r0`).  Forcing the spill with a `Level *level = m_pLevel;` local
was measured and is much worse (50.8-52.6, 57072-57480 B), so the original
does not cache `m_pLevel` in a local either.  The prologue accessor order is
`messages, player, enemies` in the original (REA/Ghidra); our `enemies,
player, messages` order is the only size-exact one (the faithful order is
57324 B).

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
cmake --build cmake-build-match --target drift -j6      # 0 fields
```
