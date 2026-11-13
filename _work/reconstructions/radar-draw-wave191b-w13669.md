# Radar::draw wave 191b (w13669) — notTurretMode/notOtherLock order landed (raw tie, aligned −3)

**Status:** complete, 2026-11-13. Worktree `wt/w13669`, branch `wave191b-w13669`,
base `master` `2064dfd40`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.
Read with `radar-draw-wave191b-w13660.md` (gas-blip store order landed),
`-w13656.md`, `-w13647.md` (target brackets), `-w13644.md` (blip/store order),
`-w13597.md` (r10/r11 cascade).

**Landed:** swap the two independent local declarations

```cpp
-    int notTurretMode = !turretMode;
-    int notOtherLock = !otherLock;
+    int notOtherLock = !otherLock;
+    int notTurretMode = !turretMode;
```

`raw` is unchanged (6,013), `aligned` improves 1,958 → 1,955, size stays
9,908/9,908, `.text` differing bytes 71,227 (unchanged), `.ARM.extab` 577
(unchanged), `.ARM.exidx` / `.rodata` 0 / 0.  Full `check` gate is green
(byte_exact 4508, linked_exact 4508, size-exact 4524, data 843/843, imports
207, lint CLEAN, parity 5370/5370, drift 0).

## 0. Baseline (verified on this worktree)

| metric | value |
|---|---:|
| raw same-address diff | 6,013 |
| instruction-index alignment | 1,958 |
| function size | 9,908 / 9,908 |
| instructions | 3,541 ours / 3,532 orig |
| `.text` differing bytes | 71,227 |
| `.ARM.extab` differing bytes | 577 |
| `.ARM.exidx` / `.rodata` differing bytes | 0 / 0 |
| zero / `missionBlocksPlanets` reg | `sl` (r10) |
| `campaign` | spilled `sp+0x90` |
| original zero / campaign | `fp` (r11) / `sl` (r10) |

Build: `cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs`
then `cmake --build cmake-build-match --target link_so -j2` inside
`ulimit -v 12000000`.

## 1. Landed change: `notOtherLock` declared before `notTurretMode`

The two locals are pure negations of independent values (`otherLock` is
already computed just above; `turretMode` is read at function entry), so
their declaration order is semantically free.  The original's emitted code
schedules the `notOtherLock` negation/store before the `notTurretMode`
negation/store:

```
12e776 ldr  r0,[sp,#148]    ; otherLock
12e778 add.w r9,r4,#364
12e77c str.w sl,[sp,#116]
12e780 mov.w fp,#68
12e784 eor.w r0,r0,#1       ; notOtherLock
12e788 str  r0,[sp,#128]
12e78a eor.w r0,r5,#1       ; notTurretMode
12e78e str  r0,[sp,#112]
```

The base source emitted the opposite order (and the opposite stack-slot
assignment).  Swapping the declarations reproduces the original's
`notOtherLock`-first schedule, leaves the instruction count and every
instruction length unchanged, and therefore changes no section bytes except
the local hunk (`.text` differing bytes stay 71,227 because the whole
12e3d4–12f710 window is already shifted by the campaign spill pair).

## 2. Fresh angles measured this pass

All cells are size 9,908 unless noted.  Base = 6,013 / 1,958 / 9,908.

### 2.1 Range rings — already byte-identical

The four radar-background `DrawImage2D(field_0x1c4, ...)` calls
(`12e1bc`–`12e224`) are instruction-for-instruction identical to ours,
including the `field_0x1a8 = onPlanetCourse` store between the first and
second call.  No change possible.

### 2.2 Target bracket / remaining blip geometry

Re-measured the faithful X-first target bracket and the remaining blip
geometry on this base:

| cell | raw | aligned | size | `.text` | `.ARM.extab` |
|---|---:|---:|---:|---:|---:|
| base | **6,013** | **1,958** | 9,908 | 71,227 | 577 |
| gas-spark full geometry (`screenX < left + size*2`, `screenY < top + size*2`, int `top`) | 6,029 | **1,933** | 9,908 | 71,243 | 577 |
| gas-spark X-half only | 6,014 | 1,954 | 9,908 | 71,228 | 577 |
| gas-spark Y-half only | 6,016 | 1,958 | 9,908 | 71,230 | 577 |
| planet bracket `P` (screen-first X-first) | 6,019 | 1,997 | 9,908 | 71,233 | 577 |
| planet bracket `P2` (bound-first X-first) | 6,016 | 1,951 | 9,908 | 71,230 | 577 |
| planet bracket `P3` (right/bottom temps) | 6,019 | 1,997 | 9,908 | 71,233 | 577 |
| planet bracket `P4` (`left < screenX` form) | 6,019 | 1,997 | 9,908 | 71,233 | 577 |
| landmark bracket `L` | 6,020 | 1,991 | 9,908 | 71,234 | 577 |
| landmark bracket `L2` | 6,020 | 1,995 | 9,908 | 71,234 | 577 |
| planetDockIndex inside the `field_0x18` if (`ittt ne`) | **6,011** | 1,959 | 9,908 | **71,225** | 577 |
| planetDockIndex before the if | 6,013 | 1,958 | 9,908 | 71,227 | 577 |
| planetDockIndex after `planetHit` | 6,013 | 1,958 | 9,908 | 71,227 | 577 |
| `ss2` field_0x18/planetDockIndex swap | 6,014 | 1,957 | 9,908 | 71,228 | 577 |
| planet-loop `onScreen` guard first (`os`) | 6,014 | 1,954 | 9,908 | 71,228 | 577 |
| enemy `field_0x76` before `egoPos` | 6,014 | 1,950 | 9,908 | 71,228 | 577 |
| enemy offscreen `0x73` before `0x76` | 6,013 | 1,978 | 9,908 | 71,227 | 577 |
| gas offscreen `0x73` before `0x76` | 6,015 | 1,962 | 9,908 | 71,229 | 577 |
| asteroid tractorAuto `dockNavPtr` first | 6,013 | 1,962 | 9,908 | 71,227 | 577 |
| asteroid tractorMode2 swap | 6,013 | 1,958 | 9,908 | 71,227 | 577 |
| enemy lock blip `setFrame,setRefPixel,draw` | 6,017 | **1,938** | 9,908 | 71,231 | 577 |
| station lock blip `setFrame,setRefPixel,draw` | 6,019 | 1,953 | 9,908 | 71,233 | 577 |
| music tail `A || B || C` | 6,036 | 1,956 | 9,908 | 71,250 | 577 |
| sound ladder `inAlienOrbit` first | 6,028 | 1,953 | 9,908 | 71,242 | 577 |
| gas `field_0x73 = 0` after `DrawImage2D` | 6,016 | 1,960 | 9,908 | 71,230 | 577 |
| **`notswap` (landed)** | **6,013** | **1,955** | 9,908 | 71,227 | 577 |
| `notswap` + planetDockIndex-inside-if | 6,011 | 1,956 | 9,908 | 71,225 | 577 |

The `planetDockIndex`-inside-if cell is the only raw-negative cell found, but
it changes the original's unconditional `str.w fp,[r4,#64]` at `0x12e5ae`
into a third predicated store (`ittt ne`), i.e. it changes the branch layout;
the faithful unconditional store is kept.

### 2.3 Exhaustive adjacent-statement / rotation sweeps

* 144 adjacent simple-statement swaps across the whole `Radar::draw` body:
  134 compiled; the unique raw-tie aligned winner is `notswap` (1,955); every
  other raw-tie cell is 1,958.
* 118 two/three-position rotations of simple statements: the only raw-tie
  cells below 1,958 are the same `notswap` (1,955).  The only raw-negative
  cell is the unfaithful planetDockIndex-inside-if (6,011 / 1,959).

## 3. Why the residual is still the RA cascade

The first byte difference is still the `mode` spill slot (`sp+0x68` vs
`sp+0x64`); the first length difference is still `cmp.w sl,#2` vs
`ldr r0,[sp,#0x90]; cmp r0,#2` at `0x12e38a`.  The landed swap only rotates
the `notOtherLock`/`notTurretMode` negation schedule and the two stack slots
they use; it does not touch the `{zero, campaign} × {r10, r11}` colouring.

## 4. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6013 aligned= 1955 size=9908/9908 insn=3541/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
cmake --build cmake-build-match --target drift -j2   # DRIFT: 0
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13669/`: `cell.py`
(compile+link+strip+`fn_diff`+section-diff cell harness), `drive.py`
(named-edit driver), `sweep.py` / `sweep2.py` (adjacent-swap and rotation
sweeps, `sweep.json` / `sweep2.json`), `Radar.base.cpp` (pristine source),
`ours_base.dis` / `ours_notswap.dis`, `seqdiff.py` / `seqstruct.py` /
`seqreorder.py` / `storeorder.py` (instruction-sequence diff tools).
