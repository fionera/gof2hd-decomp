# LevelScript::process — wave 141 / w8601: the `s4gp` receiver is the -8 B companion
# that turns the re-based w8404 set into a raw-diff win at 57232 B

Status: **code change landed**, 2026-11-07. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, size-exact.
Owned TU: `src/game/world/LevelScript.cpp`. Base master `e18e0c2a4` (wave-140 docs).
Scratch helpers in `/dev/shm/w8601/` (not tracked): `fdiff.py`, `secdiff.py`,
`textdiff.py`, `lsda.py`, `lsdatab.py`, `census.py`, `regions.py`,
`apply.py` (named-edit harness), `sweep.py` (subset sweep).

## Result

```
                         before (master)   after (landed)
process raw diffbytes    54,138            54,034            (-104)
whole .text diffbytes    86,472            86,368            (-104)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
.text size               1,259,496         1,259,464         (EXACT; the +32 tail pad is gone)
.ARM.extab size/diff     156,312 / 1,411   156,312 / 1,411   (size exact)
.ARM.exidx               0                 0
.rodata / .data / .bss   exact             exact
.rel.plt                 0 (byte-identical) 0 (byte-identical)
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

The landed set is three genuine faithfulness fixes (all verified against objdump
of the original): `case40` (the nested state-0 test, -8), `routefold` (the
faithful float fold, 0), and **`s4gp`** (the fresh `getPlayer()` receiver for
the state-4 `setCollide`, +8).  Net size is exactly 0, so every later function
keeps its address.

## The new finding: `s4gp` is the missing -8/-104 lever

The state-4 teardown at original `0x138d86..0x138d96` is:

```
138d86  ldr.w r0, [sl, #24]   ; m_pLevel
138d8a  movs  r1, #1
138d8c  strb.w r1, [sl, #17]  ; m_bCinematicBreak = 1
138d90  blx   Level::getPlayer
138d94  movs  r1, #0
138d96  blx   PlayerEgo::setCollide
```

The source used the cached `player->setCollide(false)`.  Spelling the receiver
faithfully is **+8 B in isolation** (57240) and, crucially, **-8 B in
combination with the case-40 nest** (57224 + 8 = 57232).  The pair also
re-rolls MachineBlockPlacement into a much better layout: the process raw diff
drops from 54,138 to **54,033** (case40+s4gp) / **54,034** (with routefold),
and the whole `.text` section size becomes exactly 1,259,464 (the baseline's
+32 tail pad disappears).

## Why the mission's w8404-set re-base is -12, not -24

On the current master, `rgd2` (docking fresh `getPlayer`), `rgd` (case-154
state-9 `resetGunDelay`) and `dupreset` (the duplicate `resetCamera` removal)
are **already in the source**.  The only unapplied w8404 edits are:

| edit | Δ size (isolated) | Δ size in the set |
|---|---:|---:|
| `case40` (nested state-0 test) | -8 | -8 |
| `routefold` (`(int)(camPos.z + 30000.0f)`) | 0 | 0 |
| `tailorder` (both globals before `SetCurrentApplicationModule`) | -12 | -12 |
| **re-based w8404 set** | | **-12** (57220) |

`tailorder` interacts +8 with `case40`/`routefold`, so the set is 57220, not
57208.

## The condfetch companion on this base

`condfetch` (the three `m_pLevel->getEnemies()->count` loop conditions) is still
the faithful +24 B edit: the baseline emits **31** `Level::getEnemies` calls,
condfetch emits **34**, exactly the original's count.  But on the current base
it lands the w8404 set at **57240 B**, raw **54,090** — the +24 is not
size-neutral against the -12 set:

| variant | size | raw diff |
|---|---:|---:|
| master | 57232 | 54,138 |
| re-based w8404 set | 57220 | 54,963 |
| w8404 set + condfetch | **57240** | **54,090** |
| w8404 set + condfetch + s1sep | 57224 | 55,147 |
| w8404 set + condfetch + s1sep + s4gp | **57232** | **54,687** |
| w8404 set + condfetch + s1sep + move4b | **57232** | **54,695** |

So the +24 companion *can* be landed at 57232 (with `s1sep` + `s4gp`), but its
raw diff is 54,687, i.e. 549 B worse than the `case40 + s4gp` landing.  The
raw-diff win comes from the `s4gp` layout re-roll, not from condfetch.

## Exhaustive subset sweep (8 named edits, 256 builds)

All subsets of `{case40, routefold, tailorder, condfetch, nonull, s1sep,
move4b, s4gp}` were built, linked and measured.  Every 57232-B result with an
exact `.ARM.extab` (gate-green):

| variant | raw diff |
|---|---:|
| **`case40 + s4gp`** | **54,033** |
| **`case40 + routefold + s4gp` (landed)** | **54,034** |
| `routefold` | 54,136 |
| master | 54,138 |
| `routefold + move4b` | 54,144 |
| `move4b` | 54,146 |
| `routefold + nonull + s4gp` | 54,195 |
| `nonull + s4gp` | 54,199 |
| `routefold + s1sep + s4gp` | 54,211 |
| `s1sep + s4gp` | 54,218 |

`case40 + s4gp` is the unique best; adding `routefold` costs 1 raw byte but is
a genuine faithfulness fix, so it is kept.

## New negative result: the case-125 `s1first` shape still grows the LSDA table

The w8506 open lead was re-tested on this base.  Both `s1first` (state-1 test
first, state-0 as `else if`, switch 2/3/default) and `s01chain` (state-0/1
`else if`, switch 2/3/default) reach **57232 B / raw 54,072-54,074**, but their
process LSDA call-site table is **0x148** (master 0x146, original 0x144), which
grows `.ARM.extab` by 4, shifts `.rodata` by 0x10 and drops `byte_exact` to
**4044** (RATCHET FAIL).  The table needs to be <= 0x147; the growth is the
state-2 body pushing entries 18/19 past 0x4000 (start 0x48fc/0x4904).  Shaving
one table byte still needs ~0x900 B of code moved before the state-2 body.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w8601/apply.py case40 routefold s4gp   # 57232, raw 54034
python3 /dev/shm/w8601/textdiff.py                      # .text 86368, extab 1411
```

## Open leads

1. `condfetch` is faithful (+24, 31 -> 34 `getEnemies`) but raw-positive on
   this base.  It needs a -8 B faithful companion that does not perturb the
   `s4gp` layout (the `nonull` -8 interacts +20 with condfetch).
2. `move4b` (the full four-call state-4 teardown move) is faithful but is -8 in
   combination with `case40` (57224); it needs a +8 B companion to land at
   57232.  `s4gp` is the receiver-only subset of `move4b`.
3. The 447-instruction block at original `0x13d806` (mission-0 states 5/6) is
   still placed out of line in our build; matching the original's
   mission-0 state-0 register map (`sl=stackguard, r4=1.0f, r9=enemies`) is the
   next global layout lever.
