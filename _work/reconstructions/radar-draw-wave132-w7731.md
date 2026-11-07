# Radar::draw wave 132 (w7731) — post-loop-recompute removal + `stations` register-allocation forcing (no source win)

**Status:** research note, 2026-11-07. Worktree `wt/w7731`, based on `master`
(`144fc16b0`). **No source change landed**; this commit contains this note only.
The target remains the pristine baseline `src/game/weapons/Radar.cpp`
(`sha256 9b315de8bf294d1f5bbc303ef37551b6d828598d57ea6c4a121882bba684fbd3`).

Target `_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).
Original 9908 B / 3492 normalised instructions; ours 9908 B / 3483.
Baseline unrounded `difflib.SequenceMatcher(None, on, un, autojunk=False).ratio()`
= **75.354839 %** (official display 75.4). Gate at start and end:
`RATCHET PASS`, byte **4506**, linked **4506**, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, avg 99.94.

Read with `radar-draw-wave131-w7712.md`, `radar-draw-wave131-w7697.md`,
`radar-draw-wave131-w7696.md`, `radar-draw-wave129-w7674.md`.

This wave attacked the assignment's hypothesis directly: **delta (2) the invented
post-loop recompute at `12e72c`, and delta (3) `stations` in `r6` instead of
`sl`+spill with the `turretMode` slot swap**. It adds three pieces of evidence
the earlier notes did not have:

1. a **byte-accounting proof** that the 72 B post-loop recompute exactly
   compensates the original's +28 B planet-loop body and +44 B tail (so the
   recompute cannot simply be deleted while keeping 9908 B);
2. a **direct `stations` register-allocation forcing sweep** (volatile pointer,
   reference, fetch-inside-loop, `data()` pointer, `SolarSystem*` local) — all
   worse;
3. a **CSE-defeating `isTarget` sweep** that proves the original's
   `stations->data()[i]` reload is a *consequence* of `stations` living in `sl`,
   not a source spelling that can be chosen independently.

The conclusion is unchanged: the baseline is a strong local optimum and the
residual is the coupled register-allocation fixed point.

## 1. Method / scratch

```sh
cd /home/debian/GalaxyOnFire2/wt/w7731
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6     # ~1 s per variant
cmake --build cmake-build-match --target check -j6        # RATCHET PASS
```

Scratch in `/dev/shm/w7731` (not committed): `Radar.base.cpp` (pristine source),
`score.py` (single-symbol difflib harness reproducing 75.354839 exactly),
`batch*.py` (variant generators), `orig.dis`/`ours.dis` (full Thumb
disassembly), `ghidra_draw.md` (Ghidra 12.1.2 decompile of `0x13e050`),
`align2.py`/`alignrange2.py`/`tailhunks.py` (normalised instruction aligners).

## 2. Byte accounting: the post-loop recompute is *exactly* the missing bytes

Region sizes (original vs baseline), measured from the disassembly:

| region | original | baseline | delta |
|---|---:|---:|---:|
| prologue + early block (`12e050..12e376`) | 806 | 806 | 0 |
| planet loop body (`12e376..cold`) | 888 | 860 | **+28** |
| cold initial recompute | 90 | 90 | 0 |
| post-loop recompute | **0** | 72 | **+72** |
| planet-lock tail + enemies + gas + asteroids + music (`tail..end`) | 8124 | 8080 | **+44** |

`806 + 888 + 90 + 0 + 8124 = 9908` (original);
`806 + 860 + 90 + 72 + 8080 = 9908` (baseline).

So deleting the post-loop recompute removes 72 B, but the original has those
72 B distributed as +28 B in the planet-loop body and +44 B in the tail. The
+28 B are the `ldr.w sl,[sp,#116]` reloads / `mov sl,r6` restores that only
exist when `stations` lives in `sl`; the +44 B are the same reloads plus the
extra `ldr r1,[sp,#108]` / `ldr r5,[sp,#112]` spills in the planet-lock tail.
Deleting the recompute without first moving `stations` to `sl` therefore *must*
undershoot 9908 B — confirmed: **9856 B / 70.76 %**.

## 3. The original's planet-loop register split (confirmed with a fresh Ghidra decompile)

Ghidra 12.1.2 (image base 0x10000, procedure `0x13e050`) confirms:

* `stations` is fetched once before the loop
  (`pSVar17 = Status::getSystem(...); iVar7 = SolarSystem::getStations(pSVar17);`);
* the campaign `isTarget` reloads `*(int *)(*(int *)(iVar7 + 4) + uVar13 * 4)`
  (i.e. `stations->data()[i]`) instead of reusing the cached `stationId`;
* the freelance `isTarget` and the 116/120/125 checks reload
  `stations->data()[i]` too;
* there is **no** post-loop recompute — `field_0x1a8` is stored only once, at
  `12e1d0`, from the cold initial computation.

Original loop registers: `r4=this`, `r5=otherLock`, `r6=free (campaignMission
scratch)`, `r8=stationId`, `r9=currentStation`, `sl=stations` (spilled
`[sp,#116]`), `fp=i`. Baseline: `r6=stations`, `sl=stations-backup`,
`fp=i`, and the campaign `isTarget` compares `stationId` (`r8`), not a reload.

The original's reload is a *consequence* of `stations` being in `sl`: because
`sl` is the scratch for `stations->data()[i]`, clang reloads it and restores it
from `[sp,#116]`. In the baseline `stations` is in `r6`, so clang CSEs the
reload against the still-live `stationId` (`r8`) and never touches `sl`.

## 4. Variants measured this wave (all single edits on the pristine baseline)

### 4.1 Post-loop recompute / campaign `isTarget` (delta 2 + 3)

| variant | usize | raw % |
|---|---:|---:|
| **baseline** | 9908 | **75.354839** |
| remove post-loop recompute (`norecomp`) | 9856 | 70.762590 |
| campaign `isTarget = stations->data()[i]` (`camp_reload`) | 9900 | 73.235800 |
| `camp_reload` + remove recompute | 9864 | 72.991650 |
| `camp_reload` + `bool otherLock` | 9808 | 68.443804 |
| `camp_reload` + `bool otherLock` + remove recompute | 9776 | 66.936416 |
| `camp_reload` + positive-OR `otherLock` | 9900 | 73.235800 |
| `camp_reload` + `uint32_t i;` before loop | 9900 | 73.235800 |
| post-loop recompute in early-call order (`getAutoPilotTarget` first) | 9916 | 73.584635 |
| post-loop recompute `getAutoPilotTarget` first, `this->planetTargets` | 9908 | 75.326165 |
| early computation uses `this->planetTargets` | 9908 | 74.028395 |

### 4.2 CSE-defeating campaign `isTarget` spellings (all tie)

| variant | usize | raw % |
|---|---:|---:|
| **baseline** (`stationId == target`) | 9908 | **75.354839** |
| `int target = stationId; isTarget = target == targetStation` | 9908 | 75.354839 |
| `isTarget = targetStation == stationId` (swap) | 9908 | 75.354839 |
| `isTarget = stationId == (int) targetStation` | 9908 | 75.354839 |
| `int sid = stationId; isTarget = sid == targetStation` | 9908 | 75.354839 |
| freelance `isTarget` via local `ftarget` | 9908 | 75.354839 |
| `!(stationId != targetStation)` | 9904 | 72.333716 |

### 4.3 `stations` register-allocation forcing (delta 3)

| variant | usize | raw % |
|---|---:|---:|
| **baseline** | 9908 | **75.354839** |
| `Array<int> *volatile stations` | 9820 | 70.620412 |
| `int &stationId = stations->data()[i]` | 9912 | 73.702323 |
| `volatile int stationId` | 9920 | 73.537844 |
| `volatile int currentStation` | 10180 | 50.901348 |
| fetch `stations` inside the loop (LICM) | 9872 | 73.142693 |
| declare `stations` outside, assign inside loop | 9872 | 73.142693 |
| `SolarSystem *system` local reused for `getStations`+`getWarpGate` | 9884 | 70.821367 |
| `int *stationData = stations->data()` | 9828 | 71.366906 |
| `i` declared before `stations` (`uint32_t i;`) | 9908 | 75.354839 |
| `const Array<int> *stations` / `Array<int> const *stations` | 9908 | 75.354839 |
| `auto *stations` | 9908 | 75.354839 |

### 4.4 `otherLock` guard (delta 1) — re-confirmed

| variant | usize | raw % |
|---|---:|---:|
| **baseline** (negated `&&`, expect 0) | 9908 | **75.354839** |
| `bool otherLock` | 9812 | 69.067858 |
| `bool otherLock` + `getAutoPilotTarget`-first recompute | 9812 | 69.077810 |
| positive OR (`!= nullptr`) | 9908 | 74.179211 |
| positive OR + `expect(...,1)` | 9908 | 75.326165 |
| positive OR + `expect(...,0)` | 9872 | 73.440644 |
| `bool anyLock; int otherLock = anyLock ? 1 : (field_0x14 != 0)` | 9908 | 74.179211 |
| `bool anyLock = !(all null); int otherLock = ...` | 9908 | 74.179211 |
| `int otherLock = (field_0x14 != 0) ? 1 : 0` explicit | 9908 | 74.179211 |
| gas-cloud-first condition order | 9896 | 72.406371 |
| `field_0x1c`-first condition order | 9924 | 67.001291 |

### 4.5 Guard / flag / type tweaks (all tie or worse)

| variant | usize | raw % |
|---|---:|---:|
| **baseline** | 9908 | **75.354839** |
| `planetHit` as `int` | 9892 | 72.750753 |
| `marked` as `bool` | 9916 | 72.732491 |
| `textOffset` / `currentStation` / `stationId` / `warpGate` unsigned | 9908 | 75.354839 (tie) |
| `i == (uint32_t) warpGate` / `i == (uint32_t) orbitPlanetIndex` | 9908 | 75.354839 (tie) |
| `!(otherLock || planetHit || turretMode)` / reordered | 9908 | 75.354839 (tie) |
| `!(turretMode || landmarkHit)` / reordered | 9908 | 75.354839 (tie) |
| planet guard `isDockingToPlanet` first | 9900 | 73.332377 |
| second `otherLock` assignment reversed | 9908 | 74.580645 |
| landmark `i == 2` split out / null split out / `i != 2 && ...` | 9908 | 75.354839 (tie) |
| landmark loop `uint32_t i` | 9908 | 75.354839 (tie) |
| landmark loop size local | 9900 | 70.556193 |
| `__builtin_expect` on landmark continue (0 / 1) | 10156 / 9868 | 41.7 / 64.5 |
| `__builtin_expect` on planet null (0) / onScreen (0) | 9876 | 72.507900 |
| `__builtin_expect` on planet-hit guard (1) | 9840 | 60.544120 |
| `__builtin_expect` on planet-hit box (1) | 9896 | 72.090687 |

## 5. Root cause (unchanged, now with byte accounting)

The residual is the coupled fixed point described in waves 122/127/128/129:

1. the first `otherLock` guard's `cbz`+`itt` shape and the first pool at
   `12e310` (original `cbnz` chain, pool at `12e340`);
2. the invented post-loop recompute at `12e72c` — its 72 B exactly compensate
   the original's +28 B loop body and +44 B tail, so it can only be removed
   after `stations` moves to `sl`+spill;
3. `stations` in `r6` instead of `sl`+spill, and the corresponding
   `turretMode`/`planetHit` stack-slot swap (`[sp,#148]`/`[sp,#152]`);
4. the bottom-placed landmark loop header (`12eb48` vs `12e7d4`);
5. the ship `inSight` `s24`/`s26` live range (`vpush {d8-d13}` vs `{d8-d11}`).

Every source lever tried this wave either ties at 75.354839 or lands below it.
The `bool otherLock` shape is confirmed to be the original's type but is a 96 B
loss because the later `otherLock` uses compact too aggressively.

## 6. Gate / section deltas (baseline, unchanged)

```
compared 4524  avg 99.94%  byte_exact 4506  linked_exact 4506  fuzzy_perfect 4506
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Section deltas vs the original (measured directly): `.text` **85,244**
differing bytes (size/base exact), `.ARM.extab` **1,416** (size exact
156,312), `.note.gnu.build-id` **20**. `.rodata`, `.data`, `.bss`, `.got`,
`.ARM.exidx`, `.rel.plt`, `.rel.dyn`, `.dynsym`, `.dynstr`, `.hash` are
byte-identical.
