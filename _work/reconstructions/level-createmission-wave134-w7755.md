# w7755 (wave-134): `Level::createMission` — station-pirate escort `setPosition`
# aggregate-init schedule (`.text` 85,143 -> 85,135)

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w7755`, based on master `ec0acea1e`.
Continues `level-createmission-wave133-w7733.md`, `…-wave132-w7710.md`,
`…-wave131-w7700.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; linked `.text` `0xaffd8`, 17,172 B). Build/flags unchanged
(NDK r18b clang 7.0.2, `-Oz`). Ghidra provider unavailable this session, so the
original disassembly is the NDK `objdump` of the original `.so`.

## Baseline (pristine HEAD, re-measured this session)

```text
_ZN5Level13createMissionEv   92.3 %   size 17172/17172 B
function diff bytes 5771
.text diff_bytes 85143  (whole section)
.ARM.extab 1416  .ARM.exidx 0  .rodata/.data/.bss exact
gate: byte 4506/4524, linked 4506/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
```

## New hypothesis: mission-object store order / call-argument order

w7733 left the mission-creation loops as the open area.  This wave compared the
*store order* inside each creation loop against the original disassembly.  Most
loops already match exactly (wanted/wingman, jumper, big-ships, alienGuns,
pirateFortress, raid, pirate, extraAliens, case-183, case-184).  The one
remaining genuine source-order mismatch is the station-pirate escort loop:

```cpp
for (int i = first + pirates + ...; i < escortsEnd + ...; i++) {
    (*enemies)[i] = createShip(8, 0, ...);
    int x = rnd->nextInt(20000) * (rnd->nextInt(2) == 0 ? 1 : -1) + 10000;
    int y = ...;
    int z = ...;
    (*enemies)[i]->setPosition((int)(pirateBase.x + (float)x),
                               (int)(pirateBase.y + (float)y),
                               (int)(pirateBase.z + (float)z));
    ...
}
```

The original (`0xb3be2`–`0xb3c68`) does **not** compute x/y/z first.  It loads
`pirateBase.x` (`vldr s16,[sp,#312]`) immediately before the first `nextInt`
pair, then `pirateBase.y` (`s18`) before the second pair, then `pirateBase.z`
(`s24`) before the third pair, and adds each component as soon as its RNG pair
is done.  That is the schedule of a single aggregate initialiser whose three
constructor arguments are evaluated left-to-right:

```cpp
Vector pos = {pirateBase.x + (float)(rnd->nextInt(20000) * (rnd->nextInt(2) == 0 ? 1 : -1) + 10000),
              pirateBase.y + (float)(...),
              pirateBase.z + (float)(...)};
(*enemies)[i]->setPosition((int)pos.x, (int)pos.y, (int)pos.z);
```

Landing that form reproduces the interleaved `vldr`/`nextInt`/`vadd` sequence
and drops the function diff bytes **5771 -> 5763** at the exact 17,172 B.

## Variants measured this session (fresh `link_so`)

| variant | function diff bytes | size |
|---|---:|---:|
| **baseline three `int x/y/z` locals** | **5771** | 17172 |
| **`Vector pos = {…}` aggregate init (landed)** | **5763** | 17172 |
| `Vector pos(…)` constructor syntax | 5763 | 17172 |
| `float pos[3] = {…}` | 5763 | 17172 |
| `Vector pos = pirateBase; pos.x += …; …` | 5776 | 17172 |
| RNG expressions inlined directly in `setPosition(...)` | 5884 | 17172 |
| aggregate init with arguments reordered z,y,x | 5930 | 17172 |
| big-ships `station->setPosition` also converted to a `Vector` | 5763 (neutral) | 17172 |
| case-2 `cargo = nullptr` / `crateCaptured() = 0` swap | 5772 | 17172 |
| alienGuns faithful order (setRotation before setScaling, cargo before shipGroup) | 5921 | 17172 |
| pirateFortress `cargo`/`shipGroup` swap | 5771 (neutral) | 17172 |

The inlined-`setPosition` form is structurally closest to the original but
swaps the loop-counter/`nextInt` registers (`r8`/`r9`), which costs 121 bytes;
the aggregate-init form keeps `r8` = loop counter and `r9` = `nextInt`, matching
the original.  The residual in this loop is now only the `this`/`10000`
register tie-break (`this` in `fp` + `mov fp,sl`/`mov sl,fp` vs the original's
`this` in `sl` + `10000` in `fp`).

## Other evidence (no source win)

* `Level::createMission`'s other mission-object stores already match the
  original order: `field_288/friendCount` at `0xb0f3e` (compiler reorders the
  source's `field_288` first), `hostileCount/field_164` at `0xb0b00`, the
  nameLeader block at `0xb0874`, the black-market/pirate-loot blocks at
  `0xb099a`/`0xb09e6`, and every `createShip`/`createStaticObject` loop.
* The remaining large blocks are unchanged backend permutations: the alien
  turret table select (`0xb32c6`, branch vs `select`), branchless `&&`
  (`0xb2e04`, `0xb2f2c`), `pirateBase` register choice (`0xb3c6c`), and the
  extraAliens `start` arithmetic (`0xb3dda`).

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w7755/score.py                 # function diff bytes + size
python3 /dev/shm/w7755/sectdiff.py ORIG OURS    # section diff bytes
python3 /dev/shm/w7755/opdelta.py orig.asm ours.asm
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
```

Scratch: `/dev/shm/w7755/{score.py,sectdiff.py,rangediff.py,opdelta.py,
Level.base.cpp,Level.vecbrace.cpp,Level.inline.cpp,orig.asm,ours.asm}`.

## Gate counts (before -> after)

```text
byte_exact      4506 -> 4506   (4506/4524)
linked_exact    4506 -> 4506   (4506/4524)
size_exact      4524 -> 4524
data_exact       843 -> 843
imports          207 -> 207
lint            CLEAN   -> CLEAN
drift           0       -> 0
RATCHET         PASS    -> PASS
.text diff      85143 -> 85135
```
