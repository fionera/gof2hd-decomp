# LevelScript::process — wave 140 / w8502: the +24 B companion is the three missing
# `getEnemies()` loop-condition re-fetches

Status: **code change landed**, 2026-11-07. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, size-exact.
Owned TU: `src/game/world/LevelScript.cpp`. Base master `d4857f174` (wave-139 head;
`process` raw same-address diff 54,262, `.text` 84,651).
Scratch helpers in `/dev/shm/w8502/` (not tracked): `fdiff.py`, `secdiff.py`,
`apply.py` (named-edit harness), `sweep.py`, `sweep2.py`, `calls.py`,
`namedcalls.py`, `callcount.py`, `calllines.py`.

## Result

```
                         before (master)   after (landed)
official verify pct      57.4              57.4
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
process raw diff bytes   54,262            54,749           (+487)
whole .text diff bytes   84,651            85,138           (+487)
.ARM.extab diff bytes    1,395             1,395            (size 156,312 exact)
.ARM.exidx               0                 0
.rodata / .data / .bss   exact             exact
.rel.plt                 byte-identical    byte-identical
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

The raw diff rises because the faithful case-40 cold-chain re-roll of
`MachineBlockPlacement` (waves 129/132/134/136/137/139) moves the whole 57 KB
body; the official fuzzy pct is unchanged at 57.4 and every hard gate number is
identical.  The new content is a genuine faithfulness fix: the function now
issues **34** `Level::getEnemies()` calls, exactly the original's count (it was
31 before this pass).

## The NEW hypothesis — three missing loop-condition re-fetches (`condfetch`)

w8407/3b009ad7b changed the *element* access of three loops to
`(*m_pLevel->getEnemies())[i]` but left the loop *condition* on the cached
`enemies->count`.  The original re-fetches `getEnemies()` for the condition as
well.  The call census is decisive:

```
$ python3 /dev/shm/w8502/callcount.py
only in orig:
  _ZN5Level10getEnemiesEv 34 31
```

and the three sites are exactly the loops w8407 touched:

| loop | original | original condition |
|---|---|---|
| case 24 state 0 | `0x13ea4c` / `0x13ea62` | `blx getEnemies; mov r1,r0; ldr r0,[r6]; ldr r1,[r1]; cmp r4,r1; bcc` |
| case 16 state 3 | `0x13f1f4` / `0x13f2e0` | `blx getEnemies; mov r1,r0; ldr r0,[r5]; ldr r1,[r1]; cmp fp,r1; bcc` |
| case 16 state 4 | `0x140578` / `0x14059c` | `blx getEnemies; mov r1,r0; ldr r0,[r6]; ldr r1,[r1]; cmp r5,r1; bcc` |

The source now spells the condition faithfully:

```cpp
for (unsigned int i = 0; i < m_pLevel->getEnemies()->count; i++)   // case 16 s3/s4
for (unsigned int j = 0; j < m_pLevel->getEnemies()->count; ++j)   // case 24
```

The generated loop code is **instruction-for-instruction identical** to the
original at all three sites (verified with objdump; case 16 state 3 additionally
reloads `m_pLevel` from `[sp,#80]` because our register map spills it, a 4-byte
register-allocation artifact, not a source difference).  `condfetch` alone is
**+24 B** (57256 B, `getEnemies` 31 -> 34) — the exact "+24 B faithful companion"
the mission asked for.

## The landing set

The mission's w8404 set (`case40 + tailorder + dupreset`; the case-154 state-9
`rgd` is already in the base) is **-24 B** on this base (`tailorder+dupreset` is
the -24; `case40` is -8 and the two cancel against the fresh-`getPlayer` docking
edit already present).  `condfetch` is +24 B in isolation but **+28 B** in
combination (a 4-byte alignment interaction), so the mission's suggested
w8405 `cine9` (-4 B) closes the accounting:

| edit | Δ size (isolated) |
|---|---:|
| `case40` (nested state-0 test) | -8 |
| `routefold` (`(int)(camPos.z + 30000.0f)`) | 0 |
| `tailorder` (both globals before `SetCurrentApplicationModule`) | -12 |
| `dupreset` (drop duplicate `resetCamera`) | -12 |
| `condfetch` (3 loop-condition re-fetches) | +24 |
| `cine9` (state-9 tail `m_bCinematicBreak = 0`, drop timer B) | -4 |
| **measured combination** | **0** (57232 B) |

`routefold` is size-neutral in this combination and lowers the raw diff by 25 B
(54,774 -> 54,749), so it is kept.  `dockrgd` (fresh `getPlayer()` for the
docking `resetGunDelay`) is faithful but +8 B in this combination and would
overshoot to 57240, so it is left for a later pass.

## Measured variants (all at 57232 B unless noted)

| variant | size | raw diff |
|---|---:|---:|
| master | 57232 | 54,262 |
| `case40` alone | 57224 | 55,033 |
| `tailorder` alone | 57220 | 55,016 |
| `dupreset` alone | 57220 | 55,065 |
| `condfetch` alone | 57256 | 55,144 |
| `case40+tailorder+dupreset` | 57208 | 55,004 |
| `case40+tailorder+dupreset+condfetch` | 57236 | 55,168 |
| `case40+tailorder+dupreset+condfetch+cine9` | 57232 | 54,774 |
| **`case40+routefold+tailorder+dupreset+condfetch+cine9` (landed)** | **57232** | **54,749** |
| `case40+routefold+dockrgd+tailorder+dupreset+condfetch3` | 57232 | 54,942 |
| `case40+dockrgd` | 57232 | 54,270 |

The last row (`case40+dockrgd`, raw 54,270) is the smallest raw diff of any
exact-57232 combination found, but it does not carry the `tailorder`/`dupreset`
faithfulness fixes or the `condfetch` call-count fix; it is recorded as the
fallback if a later pass wants the raw metric instead of the faithful set.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
cmake --build cmake-build-match --target drift -j6      # 0 fields
python3 /dev/shm/w8502/fdiff.py _ZN11LevelScript7processEi   # 57232/57232, diff 54749
python3 /dev/shm/w8502/secdiff.py                           # .text 85138, extab 1395
```

## Open leads

1. The `condfetch` case-16-state-3 site reloads `m_pLevel` from `[sp,#80]`
   (4 B) where the original keeps it in `r5`.  A spelling that keeps
   `m_pLevel` live across the loop would recover those 4 B and make the site
   byte-identical.
2. `dockrgd` (+8 B) is still unlanded; it needs a -8 B faithful companion.
3. The case-154 state-4 teardown call order (w8306 `move4b`) is still unfixed.
