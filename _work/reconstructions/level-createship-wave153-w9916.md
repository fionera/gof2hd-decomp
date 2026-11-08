# `Level::createShip` — wave 153 / worker w9916: status-r4 hitpoints-first
# lands at exactly 2,764 B via a register-store null guard + reordered
# mission-range test

Target `_ZN5Level10createShipEiiiP8Waypointbb`, original 2,764 B @ `0xc0214`.
Base master `4a6104a43` (wave 152).  Baseline (hitpoints-first, `wy wz wx`):
raw same-address diff **2,056**, instruction-index alignment **480**,
size 2,764, entry status **r6**.

## Landed

Two source edits to the hitpoints-first body (no `__builtin_expect`, no
new lint criterion):

```cpp
    if (mission == 56 || (mission >= 49 && mission <= 52)) {   // was 5x == ||
        hitpoints = 270;
    }
    ...
    player->setEmpData(empPoints, empTime);
    if (Globals::status == nullptr) {   // null guard lever (wave 148)
        x = 0;                          // store to a *register* local, 0 literal
    }
```

| variant | size | raw | aligned | status |
|---|---:|---:|---:|---|
| master (wave 152) | 2,764 | **2,056** | **480** | r6 |
| **landed** | **2,764** | **1,577** | **457** | **r4** |
| wave-152 `Status *st` late (size-exact r4) | 2,764 | 2,291 | 487 | r4 |
| wave-152 `guard(hostile=false)` (+4 B) | 2,768 | 2,424 | 530 | r4 |

Both metrics improve over master: raw **−479**, aligned **−23**.  Gate:
`byte_exact 4507/4524`, `linked_exact 4507/4524`, size 4524/4524, data
843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS.
All 27 section sizes unchanged.

## The −4 B companion

The wave-152 "+4 B" shape is `if (Globals::status == nullptr) hostile = false;`
after `setEmpData`.  `hostile` is a spilled parameter, so clang folds the
guard into `hostile &= (status != nullptr)`:

```
ldr r2,[r4,#0] ; status
cmp r2,#0
it ne
movne r2,#1
...
ldr r0,[r7,#12] ; hostile
and.w r9,r2,r0
```

`hostile` is a *stack* store/fold.  Guarding `x` instead — `x` is already live
in r4 at that point — compiles to a single conditional subtract:

```
ldr r2,[r4,#0] ; status
cmp r2,#0
subne r4,r0,r1 ; x = 0  (4 B, replaces movne+and+ldr = 10 B)
```

That is exactly the required −4 B: a store of the smaller literal `0` to a
register-resident local instead of a store/fold through a spilled one.
`x`, `y`, `z`, `race`, `group`, `shipClass`, `type`, `hitpoints`, `level`,
`armor`, `empTime`, `jitterX/Y/Z`, `wz/wy/wx`, `waypoint`, `player`,
`ship`, `empPoints`, `mission` and `return nullptr` were all swept as guard
bodies; only `x = 0` is size-exact at 2,764 B (the rest are 2,768–2,800 or
coalesce back to the r6 baseline).  `if (!status)`, `== 0`, `&&` and
ternary spellings of the same guard all coalesce.

The second edit is the block-layout lever: reordering the five-way mission
test to `mission == 56 || (mission >= 49 && mission <= 52)` reproduces the
aligned win that `__builtin_expect(...,1)` gives (527 → 457) without adding
a `__builtin_*` occurrence (Level.cpp already has one `__builtin_memcpy`,
and the lint baseline is 1).  The original's `movs r1,#143; lsr` bitmask
lowering is unchanged; only the fall-through/`270` layout moves.

## Entry register map (landed)

```
c0220  strd r1, r2, [sp, #56]   ; race -> [sp,#56], shipClass -> [sp,#60]
c0224  mov  r9, r0              ; this -> r9
c0226  ldr  r0, [pc, ...]       ; &Globals::status
c0228  mov  sl, r3              ; type -> sl (r10)
c022c  ldr  r4, [r0, #0]        ; Status* -> r4          <-- requested
c0230  blx  getCurrentCampaignMission
c0234  mov  r8, r0              ; mission -> r8
c0236  ldr  r0, [r7, #8]        ; waypoint (incoming stack arg)
c0262  ldr  r5, [r0, #0]        ; &Globals::rnd -> r5
c0264  ldr  r6, [r1, #0]        ; &AERandom::nextInt -> r6
c026e  mov  fp, r0              ; jitterX -> fp (r11)
c0278  str  r0, [sp, #28]       ; jitterY -> [sp,#28]
c0282  str  r0, [sp, #24]       ; jitterZ -> [sp,#24]
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
| `hostile` / `group` | `[r7,#12]` / `[r7,#16]` |
| `x` | r4 (after status dies) |
| `y` / `z` | s16 / s18 (float locals) |

So `status r4 / rnd r5 / nextInt r6` matches the original; the residual
re-roll is `type` sl vs fp, `race`/`shipClass` spilled (original keeps
shipClass in r5→sl), and `jitterX` fp vs `[sp,#36]` — the same
shipClass-spill-weight/PGO residual wave 150/151 identified.

## New hypotheses measured (all full link, exact TU, clang 7.0.2 `-Oz`)

### Guard body sweep (hitpoints-first body, after `setEmpData`)

| guard body | size | raw | aligned |
|---|---:|---:|---:|
| `x = 0;` | **2,764** | **1,564** | 527 |
| `hostile = false;` | 2,768 | 2,424 | 530 |
| `race = 0;` | 2,772 | 2,432 | 479 |
| `group = false;` | 2,788 | 2,425 | 431 |
| `return nullptr;` | 2,780 | 2,441 | 481 |
| `type = 0;` | 2,780 | 2,429 | 501 |
| `y = 0.0f;` / `z = 0.0f;` | 2,776 | 2,440 | 502 |
| `shipClass = 0;` | 2,808 | 2,528 | 532 |
| `waypoint/hitpoints/level/armor/empTime/jitter*/wz/wy/wx/empPoints/mission = 0` | 2,764 | 2,056 | 480 (coalesced, r6) |

### Mission-range reorder (with `x=0` guard)

| form | size | raw | aligned |
|---|---:|---:|---:|
| `49||50||51||52||56` (base) | 2,764 | 1,564 | 527 |
| **`56 || (>=49 && <=52)`** | **2,764** | **1,577** | **457** |
| `>=49 && <=52 || ==56` | 2,764 | 1,564 | 527 |
| `switch(mission){case 49..52,56:}` | 2,764 | 1,564 | 527 |
| `__builtin_expect(49||50||51||52||56,1)` | 2,764 | 1,577 | 457 |
| `__builtin_expect(...,0)` | 2,764 | 1,577 | 457 |

### Coordinate-type permutations with the guard (all hitpoints-first)

| z,y,x | guard | size | raw | aligned |
|---|---|---:|---:|---:|
| f,f,i | `x=0` | **2,764** | **1,564** | 527 |
| f,f,i | `hostile` | 2,768 | 2,424 | 530 |
| f,i,i | `hostile` | 2,784 | 2,488 | 453 |
| i,f,i | `hostile` | 2,784 | 2,488 | 463 |
| i,i,f | `hostile` | 2,784 | 2,488 | 468 |
| i,i,i | `hostile` | 2,796 | 2,520 | 464 |
| f,f,f | `hostile` | 2,792 | 2,447 | 546 |

The original converts all three coordinates at the constructor
(`vcvt.f32.s32 s20/s18/s16` at `c0412/c041a/c0422`), i.e. int locals; every
`iii`-family shape is +20..+32 B, so the float-local shape is kept.

### Other perturbations (hostile guard, none size-exact)

`empTime` ternary 2,772 / 2,458 / **470**; integer hitpoint multipliers
2,776–2,800; `armor` if/else, `level` if/else, `Vector` member assignment,
`empPoints` single expression, `KIPlayer *kiFighter` removal, coordinate
reassociation, `hitpoints <<= if` — all coalesce to 2,768 / 2,424 / 530.
Moving `x`/`x,y,z` computation earlier gives 2,780.

## Conclusion

The `+4 B` status-r4 shape is now size-exact at 2,764 B by replacing its
spilled-`hostile` store/fold with a register-local `x = 0` guard (exactly
−4 B), and the mission-range reorder recovers the aligned win without a
new `__builtin_*`.  Both metrics improve on master; the entry map has the
requested `status r4 / rnd r5 / nextInt r6`.  The remaining map delta is
the known shipClass-spill-weight/PGO residual, not a source-order issue.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9916/fdiff.py _ZN5Level10createShipEiiiP8Waypointbb
python3 /dev/shm/w9916/align_diff.py _ZN5Level10createShipEiiiP8Waypointbb
cmake --build cmake-build-match --target check -j6
```

Scratch `/dev/shm/w9916/`: `harness.py` (body splice + build + raw/aligned +
entry map), `diffs.py`/`diffbytes.py` (structural diff), `sweep_guard.py`
(guard bodies/conditions), `sweep_guardform.py` (guard spellings),
`sweep_coord.py` (coordinate types), `sweep_combo.py` (guard × expect),
`sweep_sc.py` (shipClass expects), `sweep_perturb.py` (body perturbations),
`sweep_m49.py` (mission-range forms), `sweep_place.py` (coordinate placement).
