# LevelScript::process — wave 142 / w8703 (case-157 tail call sharing + the two
# missing `getEnemies` loop conditions + case-144 duplicate `setStation`)

Status: **code change landed**, 2026-11-08. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, size-exact.
Owned TU: `src/game/world/LevelScript.cpp`. Base: wave-141 head `1d10d0694`.
Scratch helpers in `/dev/shm/w8703/` (not tracked): `fdiff.py`, `combo.py`,
`census.py`, `callseq.py`, `scan_rev.py`, `scan_fwd.py`, `rdiff2.py`.

## Result

```
                         before (HEAD)     after (landed)
process raw diffbytes    54,108            54,677            (+569)
whole .text diffbytes    84,567            85,136            (+569)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
.ARM.extab diffbytes     1,395             1,394             (size 156,312 exact)
.ARM.exidx               0                 0
.rodata / .data / .bss   exact             exact
.rel.plt                 0                 0   (byte-identical, 29,392 B)
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

## 1. The mission fix: case-157 tail call sharing (w8603 lead 2)

The case-157 `m_nScriptTimerA >= 6500` tail (source line 5138) is outlined by
clang into the cold region at the end of the function.  The original cold block
(`0x145bd0..0x145bec`) stores both globals and then **branches to the single
shared `SetCurrentApplicationModule(2)` call site** at `0x13d6b6`:

```
145bd0  ldr r0,[pc]   ; &Globals::appManager
145bd2  ldr r1,[pc]   ; &Globals::switch_to_target_setting
145bd4  ldr r2,[pc]   ; &Level::initStreamOutPosition
...
145be4  str.w r8,[r1] ; switch_to_target_setting = 1
145be8  strb.w r8,[r2]; initStreamOutPosition = 1
145bec  b.w 13d6b6    ; movs r1,#2 ; blx SetCurrentApplicationModule
```

The source had the call between the two stores, so clang emitted a *second*
`movs r1,#2; blx` in the cold block (10 calls vs the original's 9).  Moving the
`Level::initStreamOutPosition = 1;` store before the call:

```cpp
player->player->setGammaHP(100);
Level::initStreamOutPosition = 1;
Globals::switch_to_target_setting = 1;
Globals::appManager->SetCurrentApplicationModule(2);
```

lets the tail-merge fire: the cold block now ends with `b.w 13d682`
(`movs r1,#2; blx; b.w <continuation>`) and the call multiset is **9/9**
(was 10).  The store order also matches the original exactly
(`str.w switch` then `strb initStream`).

This reshape is **-4 B in isolation** on this base (57228 B).  It is the known
w8404 `tailorder` lever; w8404 measured -8 and w8601 -12 on older bases, the
current base gives -4.

## 2. The size-neutral companion set: the two missing `getEnemies` re-fetches
## and the case-144 duplicate `setStation`

The tail reshape needs +4 B.  No genuine +4 B statement was found; the landed
companion set is three genuine faithfulness fixes that net to +4:

* **case-16 state-3 loop condition** (source line 1709, original `0x13f2e0`):
  `i < enemies->count` -> `i < m_pLevel->getEnemies()->count`.  The original
  re-fetches `getEnemies()` in the loop condition every iteration.
* **case-24 loop condition** (source line 1797, original `0x13ea60`): same fix
  for `j < ...`.
* **case-144 state-3 duplicate `setStation`** (source line 4295): the original
  `0x141494..0x1414a8` does `getStation(98)` then `departStation`, with no
  `setStation`; the source had a spurious
  `Globals::status->setStation(Globals::status->getStation());`.

After the set, the call census matches the original on the two levers the
mission targeted:

| symbol | orig | ours (before) | ours (after) |
|---|---:|---:|---:|
| `ApplicationManager::SetCurrentApplicationModule` | 9 | 10 | **9** |
| `Level::getEnemies` | 34 | 32 | **34** |

## Measured combinations (all built + linked)

| variant | size | raw diff |
|---|---:|---:|
| HEAD | 57232 | 54,108 |
| `tail` alone | 57228 | 54,177 |
| `tail + e24` | 57244 | 54,959 |
| `tail + e16a` | 57240 | 55,090 |
| `tail + move4b` | 57220 | 55,065 |
| `tail + move4b + e24` | 57236 | 54,538 |
| `tail + e24 + e16a` | 57248 | 55,074 |
| **`tail + e24 + e16a + dupset` (landed)** | **57232** | **54,677** |
| `tail + move4b + e24 + dupset + e16a` | 57232 | 54,685 |

`move4b` is the w8406 state-4 teardown move (three calls before
`m_bCinematicBreak`, `removeAllGuns` after `setComputerControlled`); it is -8 B
on this base.  The landed set is the lowest-raw 57232 B combination found that
contains the tail reshape and both missing `getEnemies` conditions.

The raw same-address diff rises because the added calls shift the following
blocks; the faithfulness (call multiset, store order, shared tail) improves.
`.ARM.extab` drops 1 byte (1,395 -> 1,394).

## Negative results / open leads

1. **No genuine +4 B statement was found.**  Scans of every
   `setPosition(Vector)` <-> `setPosition(fff)` site (41 reverts, 3 forwards)
   gave only 0 / +8 / -20 B.  `__builtin_expect(m_nState == 0, 1)` on case 91
   is +4 B on this base (not -4 as on the wave-133 head).
2. The cold block is still **2 bytes early** relative to the original
   (`0x145bd4` vs `0x145bd0` after the landed set) because the prologue loads
   `&Globals::status` into `r9` (`ldr.w`, 4 B) where the original uses `r4`
   (`ldr`, 2 B).  Reordering the prologue accessors to `messages, player,
   enemies` is the fix but costs +96 B (w8604).
3. `tail + move4b + e24` is 57236 B and raw 54,538 (lower raw than the landed
   set) but is not size-exact; it needs a -4 B companion.
4. `.ARM.extab` is 1,394 differing bytes at an exact 156,312 B size.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w8703/combo.py tail,e24,e16a,dupset     # 57232, raw 54677
python3 /dev/shm/w8703/census.py                         # SetCurrentApplicationModule 9/9
```
