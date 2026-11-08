# Small tail B wave 158 (w10319): Generator volatile-rem raw win,
# SpaceLounge clamp colouring sweep, `.ARM.extab` per-record map

Status: **landed one source change** (Generator bonus block).  Worktree
`wt/w10319`, base master `08575efff` (wave 156).  Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).  Scratch
`/dev/shm/w10319`.

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, `data_exact 843/843`, imports 207,
lint CLEAN, drift 0, parity 5370/5370.  All 27 section sizes unchanged.

Read first: `small-tail-b-spacelounge-generator-wave157-w10218.md`,
`status-generator-spacelounge-wave155-w10110.md`,
`extab-largest-record-wave156-w10219.md`,
`SpaceLounge-drawLounge-wave123-w7315.md`,
`spacelounge-drawlounge-wave130-w7682.md`.

Metrics: raw same-address diff bytes (`/dev/shm/w10319/fdiff.py`) and
instruction-index alignment (`/dev/shm/w10319/align_diff.py`).

| fn | before raw/aligned/size | after raw/aligned/size |
|---|---:|---:|
| `SpaceLounge::drawLounge` (`_ZN11SpaceLounge10drawLoungeEv`) | 175 / 41 / 1948 | 175 / 41 / 1948 |
| `Generator::createMission` (`_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`) | 393 / 52 / 1996 | **300 / 59 / 1996** |
| `.ARM.extab` total diff bytes | 1292 | 1292 |

## 1. Generator — landed `volatile int rem` + `bonus -= rem * 2`

The landed bonus block is

```cpp
volatile int rem = rawBonus % 50;
bonus = rawBonus + rem;
if (bonus % 50 != 0) {
    bonus -= rem * 2;
}
```

It is size-exact (1996 B, frame 96) and improves the raw same-address diff
393 -> 300.  The aligned metric rises 52 -> 59 because the volatile reload
prevents if-conversion (`cbz` instead of `it ne`) and the block is one
instruction shorter than the original's 10-instruction block, so every
later branch target index shifts.

Original block (0x9770c):

```
9770c: adds r5, r1, r6      ; bonus = rem + rawBonus
9770e: str r1, [sp, #32]    ; spill rem
97710: movs r1, #50
97712: mov r0, r5
97714: blx __aeabi_idivmod
97718: ldr r0, [sp, #32]    ; reload rem
9771a: cmp r1, #0
9771c: it ne
9771e: subne r5, r6, r0     ; bonus = rawBonus - rem
97720: str r5, [sp, #16]
```

Landed block:

```
9770a: str r1, [sp, #44]    ; volatile rem store
9770c: movs r1, #50
9770e: ldr r0, [sp, #44]
97710: add r6, r0           ; bonus = rawBonus + rem
97712: mov r0, r6
97714: blx __aeabi_idivmod
97718: cbz r1, 97720
9771a: ldr r0, [sp, #44]
9771c: sub.w r6, r6, r0, lsl #1
```

### Measured Generator variants this wave (all real TU compile + gold link)

| shape | raw | aligned | size |
|---|---:|---:|---:|
| HEAD `rawBonus + rawBonus - bonus` | 393 | **52** | 1996 |
| **`volatile int rem` + `bonus -= rem * 2` (landed)** | **300** | 59 | **1996** |
| `bonus -= rem * 2` (non-volatile) | 445 | 59 | 1996 |
| `bonus = rawBonus - rem` | 304 | 112 | 2000 |
| `bonus = rawBonus - (bonus - rawBonus)` | 304 | 112 | 2000 |
| `bonus = bonus - rem - rem` | 304 | 112 | 2000 |
| `int rem2 = rem; bonus = rawBonus - rem2` | 304 | 112 | 2000 |
| `bonus = rawBonus + 0 - rem` / `-rem + rawBonus` / `rawBonus - rem - 0` | 304 | 112 | 2000 |
| `const`/`unsigned`/`short`/`register` rem + `rawBonus - rem` | 304 | 112 | 2000 |
| `rem` address-taken (`int *rp = &rem`) + `rawBonus - *rp` | 304 | 112 | 2000 |
| manual modulo `rawBonus - (rawBonus/50)*50` | 304 | 112 | 2000 |
| ternary `(bonus % 50 != 0) ? rawBonus - rem : bonus` | 304 | 112 | 2000 |
| `if (bonus % 50 == 0) {} else { bonus = rawBonus - rem; }` | 304 | 112 | 2000 |
| `if (!(bonus % 50 == 0))` / `if (bonus % 50)` / `while`/`do` | 304 | 112 | 2000 |
| `__builtin_expect(bonus % 50 != 0, 0/1)` + `rawBonus - rem` | 304 | 112 | 2000 |
| `int rem` hoisted to function scope | 690 | 161 | 2024 |
| `rawBonus` hoisted to function scope | 304 | 112 | 2000 |
| `volatile int rem` + HEAD body | 592 | **34** | 2000 |
| `volatile int rem` + `rawBonus - rem` | 702 | 91 | 2000 |
| `volatile int rem` + `bonus -= rem + rem` | 578 | 65 | 2000 |
| `volatile int rem` + `bonus -= rem; bonus -= rem;` | 578 | 65 | 2000 |
| `volatile short rem` + `bonus -= rem * 2` | 656 | 44 | 2004 |
| `volatile unsigned rem` + `bonus -= rem * 2` | 300 | 59 | 1996 |
| `__builtin_expect((type \| 4) == 12, 1)` + `rawBonus - rem` | 1801 | 176 | 1996 |
| `__builtin_expect((type \| 4) == 12, 0)` + `rawBonus - rem` | 386 | 142 | 2004 |

### Why the exact original block is not reachable

The original spills `rem` to the existing scratch slot `[sp,#32]` and
reloads it (`str`/`ldr`), keeping `agent` in `r9`.  Every `rawBonus - rem`
spelling instead lets RAGreedy put `rem` in `r9` (`mov r9, r1`) and evict
`agent` (which is reloadable from its own `[sp,#24]` slot).  That adds one
4-byte stack slot at the bottom of the frame, shifts every local by +4 and
grows the frame 96 -> 104 (`sub sp, #104`), so the function is 2000 B and
the size-exact class (and therefore `byte_exact`) drops.

The allocator choice is a spill-weight tie: `rem` is a single-MBB local
range and gets the `RC.AllocationPriority << 24` local-range priority, so
it is assigned before the long global `agent` range.  The original's
sample-profile branch weights evidently make the bonus block cold enough
that `rem` loses the tie and spills.  `__builtin_expect` on the bonus
condition does not change the weights (the block is if-converted), and
`__builtin_expect` on the outer `(type | 4) == 12` changes the block layout
too much (raw 1801+).  `volatile` forces the store but also blocks
if-conversion, which is why the aligned metric rises.

`volatile int rem` + HEAD body gives the best aligned result (34) because
its block is the same 22 bytes / 10 instructions as the original, but the
volatile slot grows the frame to 104 (2000 B), which fails the size gate.
No size-exact shape measured this wave improves both metrics.

## 2. SpaceLounge — clamp colouring sweep, HEAD retained

HEAD remains 175 / 41 / 1948.  The residual is still the `pad`/`x`
colouring: original `pad -> r9`, `x -> r5`; HEAD `pad -> fp`, `x -> r9`
plus one `mov r5, fp` split artifact.  The original's right arm is the
2-instruction `itt gt; addgt.w r1, r6, sl; subgt r5, r0, r1`; HEAD's
`+ (-width - pad - pad)` emits the 4-instruction
`movgt r1,#0; subgt.w r1,r1,fp,lsl#1; subgt r1,r1,r6; addgt.w r9,r1,r0`.

New shapes tried this wave (all real TU compile + gold link; unchanged
result means 175 / 41 / 1948):

| shape | raw | aligned | size |
|---|---:|---:|---:|
| HEAD | **175** | **41** | **1948** |
| mutate `centerX` directly, no `int x` | 175 | 41 | 1948 |
| `int x; x = centerX;` (assignment form) | 175 | 41 | 1948 |
| `int &x = centerX;` | 175 | 41 | 1948 |
| `top` declared before `centerX` | 175 | 41 | 1948 |
| `top` with `pad * 2` instead of `pad + pad` | 175 | 41 | 1948 |
| `float centerXf; int centerX = (int) centerXf;` | 175 | 41 | 1948 |
| `float topF; int top = (int) topF;` | 175 | 41 | 1948 |
| `long` / `int32_t` / `intptr_t` / `(int)(intptr_t)` / pointer-deref pad | 175 | 41 | 1948 |
| `unsigned width` | 175 | 41 | 1948 |
| `lineHeight` hoisted before the clamp | 175 | 41 | 1948 |
| `Layout *layout` hoisted before `pad` | 1836 | 276 | 1972 |
| `pad` after `centerX` | 254 | 72 | 1948 |
| `pad` after `top` | 254 | 72 | 1948 |
| `width` declared before the name/role strings | 1249 | 176 | 1952 |
| `(short)` cast on `pad` | 1490 | 219 | 1964 |
| `volatile int pad` | 1832 | 384 | 1980 |
| `__builtin_expect(x - pad < inset, 0)` | 488 | 71 | 1952 |
| `pad * 2` right arm | 445 | 54 | 1948 |
| `int boxW = width + pad * 2` used twice | 1833 | 258 | 1968 |

The `boxW` family is still the only shape that reaches a different
`pad`/`x` colouring (it spills `x` to `[sp,#24]` and puts `pad` in `r5`,
`x` in `r2`, not the original's `pad=r9`/`x=r5`), and it grows the frame.
No new colouring shape was found; HEAD 175/41/1948 is retained.

## 3. `.ARM.extab` per-record map (unchanged: 1292 bytes)

`python3 _work/reconstructions/extab_record_map.py` on the landed build:

```
  diff function                                                         k   olen    len orow urow
   676 _ZN12HangarWindow6renderEv                                       0    948    948  156  156
   182 _ZN11LevelScript7processEi                                       0    348    348   46   46
   159 _ZN5Level13createMissionEv                                       0    956    956  148  148
    87 _ZN5Radar4drawEP6PlayerP3Hudi                                    0    132    132   19   19
    77 _ZN5Level10createShipEiiiP8Waypointbb                            0    336    336   57   57
    37 _ZN3Hud4drawExxP9PlayerEgobjj                                    0    476    476   77   77
    30 _ZN5MGame10OnTouchEndEiiPv                                       0    280    280   44   44
    21 _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE      0    172    172   26   26
    14 _ZN11SpaceLounge10drawLoungeEv                                   0    168    168   25   25
     7 _ZN5RadarC1EP5Level                                              0    256    256   39   39
     2 _ZN10ModStationC1Ev                                              0    112    112   15   15
TOTAL diff bytes 1292 in 11 records
```

The second-largest record is `LevelScript::process` (182 B, 348-byte
record, 46 LSDA rows in both).  It is **not fixable at source level in this
wave**: the row structure already diverges at row 0 (original
`(0,5832,0)` vs ours `(0,5654,0)`), i.e. the first call that needs a
cleanup landing pad is 178 bytes earlier in our code.  Disassembly shows
the original's first cleanup call is `Explosion::Explosion` at
`0x1393e0` (`new Explosion(0)` in `case 145`), while ours is
`Player::shoot` at `0x1392de`; the whole 57 KB function is one of the
large non-byte-exact residuals (55 KB raw diff), so the LSDA cannot match
until the function's code layout does.  Rows 4, 8, 9-12, 19, 21, 25, 29,
31, 33, 35, 39, 41, 43, 45 all have different lengths as well.

The Generator record stays 21 B and the SpaceLounge record 14 B; both are
tied to their function's remaining code differences (Generator's bonus
block / frame, SpaceLounge's clamp colouring), not to a separate LSDA
construct.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10319
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w10319/fdiff.py _ZN11SpaceLounge10drawLoungeEv
python3 /dev/shm/w10319/align_diff.py _ZN11SpaceLounge10drawLoungeEv
python3 /dev/shm/w10319/fdiff.py _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE
python3 /dev/shm/w10319/align_diff.py _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE
python3 _work/reconstructions/extab_record_map.py
```

Scratch (not committed), `/dev/shm/w10319/`: `fdiff.py`, `align_diff.py`,
`vediff.py`, `showalign.py`, `try.py`, `sweep_new.py`, `sweep_sl2.py`,
`sweep_sl3.py`, `sweep_gen.py`..`sweep_gen10.py`, `SpaceLounge.base.cpp`,
`Generator.base.cpp`, `gen_volsub2.cpp`, `gen_rawrem.cpp`, `mi_head.txt`.

## 5. Files changed

* `src/game/mission/Generator.cpp` — bonus block changed to
  `volatile int rem` + `bonus -= rem * 2` (raw 393 -> 300, size 1996).
* `_work/reconstructions/small-tail-b-wave158-w10319.md` (this note).
