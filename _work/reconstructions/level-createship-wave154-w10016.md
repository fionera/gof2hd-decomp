# `Level::createShip` — wave 154 / worker w10016: the five-way mission test
# as `if (==56) … else if (range)` is the first shape to improve *both*
# metrics at 2,764 B

Target `_ZN5Level10createShipEiiiP8Waypointbb`, original 2,764 B @ `0xc0214`.
Base master `4417db0b4` (wave 153).  Baseline (wave-153 landed body):
raw same-address diff **1,577**, instruction-index alignment **457**,
size 2,764, entry status **r4** / rnd r5 / nextInt r6.

## Landed (one source edit, no `__builtin_*`, no new lint criterion)

```cpp
    if (mission == 56) {
        hitpoints = 270;
    } else if (mission >= 49 && mission <= 52) {
        hitpoints = 270;
    }
```

was

```cpp
    if (mission == 56 || (mission >= 49 && mission <= 52)) {
        hitpoints = 270;
    }
```

| variant | size | raw | aligned |
|---|---:|---:|---:|
| master (wave 153) | 2,764 | **1,577** | **457** |
| **landed (`if/else if`)** | **2,764** | **1,564** | **452** |

Both metrics improve: raw **−13**, aligned **−5**.  Gate green:
`byte_exact 4507/4524`, `linked_exact 4507/4524`, size 4524/4524, data
843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS.
All 27 section sizes unchanged; `.text` differing bytes **86,623 -> 86,610**
(−13) and `.ARM.extab` **1,265 -> 1,264** (−1).

## Why this works

The original's mission test is the clang bitmask lowering of a five-case
`switch` (`movs r1,#143; lsr.w; lsls; it ne; movne r5,#270`).  That form
(`switch (mission) { case 49..52, 56: }`) is byte-identical *inside* the test
but scores raw 1,564 / aligned **527** — the greedy alignment is thrown off by
the 9-instruction bitmask block and the following `getLevel` call, costing 70
aligned instructions elsewhere.  The `if (==56) … else if (range)` form emits

```
cmp.w r8,#56 ; bne .L ; mov.w r5,#270 ; b .done ; .L: sub.w r0,r8,#49 ;
cmp r0,#4 ; it cc ; movcc.w r5,#270 ; .done:
```

which keeps the same 2,764 B size and the status-r4 entry map but re-aligns
the fall-through of the following `getLevel`/`empPoints` block: aligned
457 -> 452.  It is not the literal source form of the original, but it is the
first shape that moves both metrics in the right direction at the exact size.

## Entry map (landed, unchanged from wave 153)

```
c0220  strd r1, r2, [sp, #56]   ; race -> [sp,#56], shipClass -> [sp,#60]
c0224  mov  r9, r0              ; this -> r9
c0226  ldr  r0, [pc, ...]       ; &Globals::status
c0228  mov  sl, r3              ; type -> sl (r10)
c022c  ldr  r4, [r0, #0]        ; Status* -> r4
c0230  blx  getCurrentCampaignMission
c0234  mov  r8, r0              ; mission -> r8
c0236  ldr  r0, [r7, #8]        ; waypoint
c0238  cbz  r0, ...
...
c0262  ldr  r5, [r0, #0]        ; &Globals::rnd -> r5
c0264  ldr  r6, [r1, #0]        ; &AERandom::nextInt -> r6
c026e  mov  fp, r0              ; jitterX -> fp (r11)
```

| value | reg |
|---|---|
| `this` | r9 |
| `type` | sl (r10) |
| `race` | `[sp,#56]` |
| `shipClass` | `[sp,#60]` |
| `Status *` | **r4** |
| `AERandom *` | r5 |
| `&nextInt` | **r6** |
| `mission` | r8 |
| `jitterX` | fp (r11) |
| `jitterY` / `jitterZ` | `[sp,#28]` / `[sp,#24]` |

The residual map delta is still the original's `type fp / shipClass r5→sl→r6 /
race [sp,#60] / all jitters spilled` (the shipClass-spill-weight/PGO residual
waves 150–153 identified).

## Hypotheses tried this wave (all full link, exact TU, clang 7.0.2 `-Oz`)

Every measurement below is on top of the wave-153 base body (hitpoints-first,
`x = 0` register-local null guard).  The aligned column is the *worktree*
`align_diff.py` (the repo copy points at the main checkout, so re-run with
`/dev/shm/w10016/align_diff.py`).

### Mission-range test (the winning axis)

| form | size | raw | aligned |
|---|---:|---:|---:|
| `56 || (>=49 && <=52)` (master) | 2,764 | 1,577 | **457** |
| **`if (==56) … else if (>=49 && <=52)`** | **2,764** | **1,564** | **452** |
| `if (==56) … else { if (range) … }` | 2,764 | 1,564 | 452 |
| `hitpoints = (==56 || range) ? 270 : hitpoints` | 2,764 | 1,564 | 452 |
| `if (==56) … else if (>48 && <53)` | 2,764 | 1,564 | 452 |
| `(>=49 && <=52) || ==56` | 2,764 | 1,564 | 527 |
| `switch { case 49..52,56 }` (bitmask) | 2,764 | 1,564 | 527 |
| `==49||==50||==51||==52||==56` | 2,764 | 1,564 | 527 |
| `if (==56) …; if (range) …` (two ifs) | 2,764 | 1,577 | 457 |
| `if (range) … else if (==56) …` | 2,764 | 1,577 | 457 |
| `>=49 && <=56 && !=53 && !=54 && !=55` | 2,764 | 1,563 | 548 |

### `type` local / `shipClass` local (mission hint) — all coalesce

`int ty = type;` (used everywhere), `int ty = type;` (switch only),
`int sc = shipClass;` (everywhere / empTime only / ship block only),
`const int sc`, `bool isCapital = shipClass == 1` (top / late / everywhere) —
all produce the identical 1,564 / 452 body on the landed mission form
(and 1,577 / 457 on the master form).  GVN/coalescing removes the copy.

### `do/while` around the entry calls (mission hint) — coalesces / regresses

`do { mission = …; } while (0)` and `do { mission…jitterZ } while (0)`
coalesce to 1,564 / 452.  `do { mission = …; } while (mission == -1)` is
2,764 B but raw 2,312 / aligned 697 (re-rolls the whole map: this+race
spilled, type sl, shipClass r8, mission r9).

### Register-permutation levers (all non-size-exact or coalesced)

`int armor = …` moved before `empPoints` (shipClass -> fp, but +12 B,
2,776 / raw 2,439 / aligned 544); `shipClass - 1 == 0` (2,776, whole map
re-rolls); `__builtin_expect(shipClass == 1, 1)` (+4 B, 2,768, raw 2,114 /
aligned 503); `expect(shipClass == 0, 1)` (2,744, aligned 1,306);
`expect(mission == 0x9a, 0)` (2,772, raw 2,423 / aligned 544);
`expect(gameWon/hardCoreMode/type/hostile, …)` coalesce.

### Coordinate types (unchanged from wave 153)

`int x, float y, float z` (current) is the only 2,764 B shape; `fif` +12,
`ffi` +20, `iif` +20, `ifi` +20, `fii` +32, `iii` +36, `fff` +36.  The original
converts all three at the constructor (`vcvt.f32.s32 s20/s18/s16`), i.e. int
locals, but every `iii`-family shape overshoots.

## Conclusion

The five-way mission test is the lever: `if (mission == 56) … else if
(mission >= 49 && mission <= 52)` is the first source shape at exactly
2,764 B that improves **both** the raw same-address diff (1,577 -> 1,564)
and the instruction-index alignment (457 -> 452) while keeping the
status-r4 / rnd-r5 / nextInt-r6 entry map.  The remaining residual is the
shipClass-spill-weight/PGO register permutation, unchanged.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10016/fdiff.py _ZN5Level10createShipEiiiP8Waypointbb
python3 /dev/shm/w10016/align_diff.py _ZN5Level10createShipEiiiP8Waypointbb
cmake --build cmake-build-match --target check -j6
```

Scratch `/dev/shm/w10016/`: `fdiff.py` / `align_diff.py` (worktree-pointed
copies of the shared tools), `run.py` (body splice + build + raw/aligned +
entry map), `sbs.py` (side-by-side aligned diff), `gen*.py` (variant
generators: mission forms `genm`/`genn`/`geno`, type/sc locals `genb`/`gend`,
coordinates `gen8`, expects `gen5`/`gen9`, combos `genp`).
