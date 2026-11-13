# Radar::draw wave 191b (w13714) — planet-loop `onScreen` guard + enemy `update()` order landed (raw +3, aligned −10)

**Status:** complete, 2026-11-13. Worktree `wt/w13714`, branch `wave191b-w13714`,
base `master` `df4e658df`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.
Read with `radar-draw-wave191b-w13699.md` (landed enemy lock-blip `setFrame`
order), `-w13678.md` (enemy on-screen `field_0x76` order), `-w13669.md`
(notTurretMode/notOtherLock order), `-w13660.md` (gas-blip store order),
`-w13597.md` (r10/r11 `{zero,68}` cascade), `-w13562.md` (campaign spill root
cause).

**Landed:** two independent, faithful statement-order reconstructions:

1. **`os`** — the planet-target loop tests `this->onScreen` *before* reading
   `stations->data()[i]` and `Globals::status->getStation()->getIndex()`.
2. **`UPD`** — the enemy loop calls `this->update(enemy)` *before* computing
   `wreck` and clearing `enemy->field_0x75`.

`raw` 6,018 → 6,021 (+3), `aligned` 1,927 → 1,917 (−10), size stays
9,908/9,908, `.text` differing bytes 71,375 → 71,378 (+3), `.ARM.extab` 577
(unchanged), `.ARM.exidx` / `.rodata` 0 / 0.  Full `check` gate is green
(byte_exact 4508, linked_exact 4508, size-exact 4524, data 843/843, imports
207, lint CLEAN, parity 5370/5370, drift 0).

## 0. Baseline (verified on this worktree)

| metric | value |
|---|---:|
| raw same-address diff | 6,018 |
| instruction-index alignment | 1,927 |
| function size | 9,908 / 9,908 |
| instructions | 3,541 ours / 3,532 orig |
| `.text` differing bytes | 71,375 |
| `.ARM.extab` differing bytes | 577 |
| `.ARM.exidx` / `.rodata` differing bytes | 0 / 0 |
| zero / `missionBlocksPlanets` reg | `sl` (r10) |
| `campaign` | spilled `sp+0x90` |
| original zero / campaign | `fp` (r11) / `sl` (r10) |

Build: `cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs`
then `cmake --build cmake-build-match --target link_so -j2` inside
`ulimit -v 12000000`.

## 1. Landed change `os`: planet-loop `onScreen` guard first

Source (`Radar.cpp:543-548`) was:

```cpp
this->update(this->planetTargets->data()[i]);
int stationId = stations->data()[i];
int currentStation = Globals::status->getStation()->getIndex();
if (!this->onScreen) {
    continue;
}
```

The original (`0x12e3ec`-`0x12e40c`) is:

```
12e3ec mov  r0, r4
12e3ee blx  Radar::update(KIPlayer*)
12e3f2 ldrb.w r0, [r4, #284]   ; this->onScreen
12e3f6 cmp  r0, #0
12e3f8 beq.n 12e4ec           ; continue
12e3fa ldr.w r0, [sl, #4]      ; stations->data()
12e3fe ldr.w r8, [r0, fp, lsl #2]  ; stationId
12e402 ldr  r0, [r5, #0]
12e404 blx  Status::getStation
12e408 blx  Station::getIndex
12e40c mov  r9, r0             ; currentStation
```

so the guard precedes both loads.  The reorder is semantically free (`onScreen`
does not affect `stationId`/`currentStation`) and reproduces the original's
`ldrb.w [r4,#284]` immediately after `update`.

## 2. Landed change `UPD`: enemy `update()` before `wreck`/`field_0x75`

Source (`Radar.cpp:692-695`) was:

```cpp
bool wreck = enemy->hasCargo && (enemy->isDead() || enemy->isDying());
enemy->field_0x75 = 0;
this->update(enemy);
```

The original (`0x12ed5e`-`0x12ed86`) is:

```
12ed5e mov  r0, r4
12ed60 mov  r1, r8
12ed62 blx  Radar::update(KIPlayer*)
12ed66 ldrb.w r0, [r8, #76]    ; enemy->hasCargo
12ed6a cbz  r0, 12ed78
12ed6c mov  r0, r8
12ed6e blx  KIPlayer::isDead
...
12ed7c blx  KIPlayer::isDying
12ed82 str  r0, [sp, #152]     ; wreck
12ed84 movs r0, #0
12ed86 strb.w r0, [r8, #117]   ; enemy->field_0x75 = 0
```

i.e. `update`, then `wreck`, then `field_0x75 = 0`.  The reorder is faithful
(`update` writes only the cached `screenX/screenY/onScreen` state and the
`wreck` computation reads `hasCargo`/`isDead`/`isDying`; the original performs
the same three steps in this order).

## 3. Fresh angles measured this pass (all size 9,908 unless noted)

Base = 6,018 / 1,927 / 9,908, `.text` 71,375, `.ARM.extab` 577.
Full 2^5 subset sweep over the five faithful order fixes found this pass:

| cell | raw | aligned | `.text` | `.ARM.extab` |
|---|---:|---:|---:|---:|
| base | **6,018** | **1,927** | 71,375 | 577 |
| `os` (planet-loop `onScreen` guard first) | 6,019 | 1,923 | 71,376 | 577 |
| `UPD` (enemy `update()` before `wreck`) | 6,020 | 1,921 | 71,377 | 577 |
| `sblip` (station blip `setRefPixelPosition` before `draw`) | 6,024 | 1,922 | 71,381 | 577 |
| `GF` (gas-spark in-sight geometry X-first, int `top`) | 6,034 | 1,902 | 71,391 | 577 |
| `gasdead` (gas `isDead()` before `isDying()`) | 6,019 | 1,927 | 71,376 | 577 |
| **`os`+`UPD` (landed)** | **6,021** | **1,917** | 71,378 | 577 |
| `os`+`sblip` | 6,025 | 1,918 | 71,382 | 577 |
| `os`+`GF` | 6,035 | 1,898 | 71,392 | 577 |
| `UPD`+`sblip` | 6,026 | 1,916 | 71,383 | 577 |
| `UPD`+`GF` | 6,036 | 1,896 | 71,393 | 577 |
| `sblip`+`GF` | 6,040 | 1,897 | 71,397 | 577 |
| `os`+`UPD`+`sblip` | 6,027 | 1,912 | 71,384 | 577 |
| `os`+`UPD`+`GF` | 6,037 | 1,892 | 71,394 | 577 |
| `os`+`UPD`+`sblip`+`GF` | 6,043 | **1,887** | 71,400 | 577 |
| `os`+`UPD`+`sblip`+`GF`+`gasdead` | 6,044 | 1,887 | 71,401 | 577 |

`os`+`UPD` is the best aligned-per-raw cell (raw +3 for aligned −10) and keeps
the change to two independent statement moves.  `GF` and the `GF` combinations
reach the lowest aligned counts (1,887-1,902) but cost 16-26 raw bytes.

### 3.1 `gasdead`

Original gas-loop exit test (`0x12f5f0`/`0x12f5fc`) calls `isDead` then
`isDying`; our source had `if (cloud->isDying() || cloud->isDead())`.  The
swap is raw-neutral/aligned-neutral on this base (6,019 / 1,927) and is not
landed (it only changes the local hunk, no metric movement).

## 4. Angle (a): asteroid lock-blip call order is not source-controllable

The original's asteroid lock-blip shared tail (`0x130552`/`0x130578`/`0x130588`)
is `setFrame`, `setRefPixelPosition`, `draw`.  Ours (`0x1304a4`/`0x1304b2`/
`0x1304d8`) is `setFrame`, `draw`, `setRefPixelPosition`.  Swapping the two
source branches:

* `ast_if_swap` (only the `if (frame < ...)` branch): 6,623 / 1,957 / **9,972**
  (size-fatal +64: the two branches stop tail-merging).
* `ast_else_swap` (only the `else` branch): 6,624 / 1,967 / **9,972**.
* `ast_both_swap`: 6,069 / 1,935 / 9,908 — size-exact but the emitted order
  is *unchanged* (`setFrame`, `draw`, `setRefPixel`) because the compiler
  reloads `this->blipSprite` for the later calls and schedules the
  `crosshairPos` loads after `draw` regardless of source order.  Raw +51,
  aligned +8 → not a win.

The station blip block (source `setFrame`, `draw`, `setRefPixel`) does emit
`setFrame`, `setRefPixel`, `draw` because `blip->draw` uses the local `blip`
pointer while `setRefPixelPosition` reloads `this->blipSprite`; the asteroid
block has no such local, so the two calls are freely reorderable.  The
remaining asteroid order is an RA/scheduling artefact, not a source-order
divergence.

## 5. Angle (b): the `field_0x76` redundant store / `{zero,68}` cascade

Re-confirmed on this base:

* The original stores `enemy->field_0x76` (`+118`) exactly four times
  (`0x12ee00`, `0x12eed6`, `0x12f61a`, `0x12f774`); ours stores it five times
  (extra at `0x12f15e`, the wreck branch).  Removing the redundant store
  (`RM`) gives 8,349 / 2,095 / **9,900** — size-fatal (−8) and the 8-byte
  shrink shifts the whole tail (`.text` diff 383,098, `.ARM.exidx` 1,047).
* The original's four `mov.w sl,#68` wide encodings (`0x12ecd0`, `0x12eea2`,
  `0x12f128`, `0x12f196`) are exactly 8 B wider than our four `movs r6,#68`;
  the redundant store currently compensates that.  Both sides of the
  `{zero, campaign} × {r10, r11}` colouring are unchanged from w13597: ours
  keeps the shared zero in `sl` and spills `campaign` to `sp+0x90`, so the
  literal 68 lands in `r6`/`r11` (narrow) instead of `sl` (wide).

### 5.1 RA perturbation cells (all reverted)

| cell | raw | aligned | size |
|---|---:|---:|---:|
| `camp_first` (campaign decl before alienOrbit) | 6,025 | 1,928 | 9,908 |
| `camp_const` | 6,018 | 1,927 | 9,908 |
| `camp_unsigned` | 6,019 | 1,927 | 9,908 |
| `camp_down` (campaign computed at its use) | 9,236 | 2,062 | **9,904** |
| `init_swap` (lockConfirmed/plasmaInRange stores) | 6,020 | 1,927 | 9,908 |
| `setcolor_first` | 6,049 | 1,935 | 9,908 |
| `explicit_zero` (`int zero = 0`) | 6,018 | 1,927 | 9,908 (byte-identical) |
| `tractor_cond` (`isDockedToDockingPoint` before `!wreck`) | 8,696 | 2,051 | **9,868** |

None flips the campaign spill; `camp_const`/`explicit_zero` are byte-identical.

## 6. Angle (c): gas-cloud/spark geometry

`GF` (the faithful X-first form with `int top`) is 6,034 / 1,902 / 9,908,
`.text` 71,391, `.ARM.extab` 577 — aligned −25 for raw +16.  It is the
single largest aligned win among the individually-landable fixes, but its
raw cost is 5× the landed `os`+`UPD` pair; it is left for a future pass that
wants the aligned Pareto point (6,043 / 1,887 with `sblip`).

## 7. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6021 aligned= 1917 size=9908/9908 insn=3541/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13714/`: `measure.py`
(fn_diff + section-diff), `harness.py` (patch/build/measure cell harness),
`cand*.py` (targeted cells), `sweep1.py` / `sweep1.json` (2^5 subset sweep),
`diffop.py` / `diffstruct.py` / `diffhunks*.py` (instruction-stream diffs),
`orig.dis` / `ours.dis`.
