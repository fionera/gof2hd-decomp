# Small tail B wave 159b (w10419): Generator `Item *` temp removes the
# `ldr r5` reload (raw 300 -> 162, size exact)

Status: **landed `src/` change** for `Generator::createMission`.  Worktree
`wt/w10419`, base master `9e4795206` (wave 157) + wave-159 note commit
`3b1812290`.  Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).  Scratch
`/dev/shm/w10419`.

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, `data_exact 843/843`, imports 207,
lint CLEAN, drift 0, parity 5370/5370.  All 27 section sizes unchanged;
file size 2,187,328 B unchanged.

Metrics: raw same-address diff bytes (`/dev/shm/w10419/fdiff.py`) and
instruction-index alignment (`/dev/shm/w10419/align_diff.py`).

| fn | before raw/aligned/size | after raw/aligned/size |
|---|---:|---:|
| `Generator::createMission` | 300 / 59 / 1996 | **162 / 64 / 1996** |
| `SpaceLounge::drawLounge` | 175 / 41 / 1948 | 175 / 41 / 1948 (unchanged) |
| `.ARM.extab` total | 1292 | **1284** |

## 1. Generator — `Item *it` temporary removes the redundant `ldr r5`

Wave 159's new bonus block (`remv` + `volatile rem` + `rv`) reproduces the
original bonus block exactly but leaves the function at 2000 B.  The +4 B
was traced to **two** independent 2-byte deltas, not one:

1. A redundant `ldr r5, [sp, #24]` at `0x97888` in the `type == 8` reward
   branch.  It is present in HEAD too (so it predates the bonus work) and
   is a register-allocator rematerialisation of `difficulty` at the entry
   of the cold `type == 8` block.  The original does not reload `r5` there.
2. The landing-pad dispatch gains one unreferenced `b.n` stub (`0x97932`)
   with the wave-159 bonus shape, pushing the literal pool out of 4-byte
   alignment and adding a `nop`.

**Fix for (1)**: give the `type == 8` reward branch an explicit `Item *`
temporary:

```cpp
} else if (type == 8) {
    Item *it = (*Globals::items)[itemId];
    reward = (float) (it->getMaxPrice() * amount) * 1.7f;
}
```

With this, `r5` is no longer rematerialised at `0x97888` and the whole
pre-landing-pad code becomes address-identical to the original
(`0x97904 mov.w sl, #0`, `0x97908 ldr r6, [sp,#24]`, `0x9790a b.n 9760a`).
Landing this **alone** (with HEAD's bonus block) is size-exact:

| shape | raw | aligned | size |
|---|---:|---:|---:|
| HEAD | 300 | 59 | 1996 |
| `Item *it` only (landed) | **162** | **64** | **1996** |
| `Item *it` + wave-159 bonus block | 86 | 31 | 2000 |
| `Item *it` + `raceId` temp before `getStanding()` | 579 | 34 | 1996 |
| `Item *it` + `Standing *standing` temp | 90 | 31 | 2000 |
| `Item *it` + `standing` + `raceId` temps | 92 | 33 | 2004 |

The landed variant trades +5 aligned for -138 raw bytes.  The aligned
regression is entirely the literal-pool shift: the landed landing pad is
2 B shorter than the original's (it is missing the second
`mov r4,r0; add r0,sp,#44` cleanup handler, exactly as HEAD was), so all
seven `vldr [pc, #N]` literal-pool references move.  The instruction
stream itself is closer than HEAD's.

## 2. Why the wave-159 bonus block is still +4 B (open)

With `Item *it` in place the wave-159 bonus block gives **86 raw / 31
aligned** — the best instruction-level match measured for this function —
but 2000 B.  The residual is the landing-pad dispatch: our build emits 13
`b.n` stubs + 5 handlers (18 landing pads), the original 12 + 5 (17).
The extra stub sits at `0x97932` and **no LSDA row references it**:

```
orig rows 10/11/13/15 lp -> 1912(97930) 1914(97932) 1910(9792e) 1916(97934 handler)
ours rows 10/11/13/15 lp -> 1912(97930) 1916(97934) 1910(9792e) 1918(97936 handler)
```

i.e. the stub at `0x97932` is dead in our build.  LSDA row count is 26 in
both.  The extra stub is triggered by the wave-159 bonus shape: with HEAD's
bonus block the landing pad has 12 stubs (but is then missing the
`sp+44` handler).  Variants that all keep the dead stub (86/31/2000):
`rv` assignment form, `rv` before `bonus =`, `rv = rem + 0`, `const int rv`,
`volatile int rem; rem = remv;`, `if (bonus % 50)`, `__builtin_expect`,
`volatile int *const rem = &remv;` pointer form, `Item *item` naming.
`costs` before `new Mission` -> 1137/192/2000.

**Open question**: get the dead `0x97932` stub dropped while keeping the
exact bonus block.  That would give 86/31/1996 and, since the code is then
address-identical to the original except the landing-pad handler set, a
very plausible byte-exact candidate.

## 3. `.ARM.extab` per-record map

The landed build drops the total from 1292 to **1284** B (11 records): the
Generator record diff falls 21 -> 13 B because the `Item *it` temp also
removes the `ldr r5` from the EH-covered region.  Section size is unchanged
at 156,312.  Second-largest is
still `LevelScript::process` (182 B, 348-byte record, 46 rows both); its
row 0 already diverges because our first cleanup call is
`player->shoot(...)` at `LevelScript.cpp:4315` while the original's is
`new Explosion(0)` at `:4325` (case 145).  Not fixable until the 57 KB
function's code layout matches.  The Generator record follows the function
body (21 B before this change; 13 B on the landed build).

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10419
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w10419/fdiff.py _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE
python3 /dev/shm/w10419/align_diff.py _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE
python3 _work/reconstructions/extab_record_map.py
```

## 5. Files changed

* `src/game/mission/Generator.cpp` — `Item *it` temporary in the
  `type == 8` reward branch.
* `_work/reconstructions/small-tail-b-wave159b-w10419.md` (this note).
