# Small tail wave 160 (w10518): MGame::OnUpdate ccm receiver direct
# (raw 37 -> 318, aligned 109 -> 19); Gun / ModStation / MenuTouchWindow re-probed

Status: **one landed `src/` change** (`MGame::OnUpdate`, mission-122
`getCurrentCampaignMission` receiver now `Globals::status` instead of the
live `Status *st`).  Worktree `wt/w10518`, base master `cfde1f5f2` (wave
158).  Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Read with `small-tail-wave159-w10418.md`, `small-tail-wave158-w10318.md`,
`small-tail-wave155-w10017.md`.

Gate before/after (full `check`): **RATCHET PASS**, `byte_exact 4507 -> 4507`,
`linked_exact 4507 -> 4507`, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, parity 5370/5370, avg 99.93.  All four bodies stay
size-exact (1984 / 1008 / 1896 / 11440).  File size exact 2,187,328 B.

Metrics: raw same-address diff bytes (`/dev/shm/w10518/fdiff_w.py`) and
instruction-index alignment (`/dev/shm/w10518/align_diff_w.py`, path-adjusted
copy of the committed tool).

| fn | raw before | raw after | aligned before | aligned after | size |
|---|---:|---:|---:|---:|---:|
| `Gun::calcCharacterCollision` (`_ZN3Gun22calcCharacterCollisionEv`) | 463 | 463 | 57 | 57 | 1984 |
| `ModStation::ModStation` C1 (`_ZN10ModStationC1Ev`) | 43 | 43 | 12 | 12 | 1008 |
| `MenuTouchWindow::OnTouchBegin` (`_ZN15MenuTouchWindow12OnTouchBeginEiiPv`) | 29 | 29 | 15 | 15 | 1896 |
| `MGame::OnUpdate` (`_ZN5MGame8OnUpdateEv`) | 37 | **318** | 109 | **19** | 11440 |

Section deltas after the change: `.text` 85,653 differing bytes (was 85,372),
`.ARM.extab` 1,293 (was 1,284), `.rodata`/`.data`/`.bss`/`.ARM.exidx`/`.rel.plt`
byte-identical, all 27 section sizes exact.

## 0. Measurement pitfall found this wave: fdiff is address-sensitive

`fdiff.py` compares same-address bytes *inside* the function, but the
`ldr rX,[pc,#N]` literal-pool words are PC-relative offsets to `.rodata`.
If any earlier TU changes size, the function moves and its literal words
change, so the raw diff changes even when the function's own code did not.
Example: with `MGame::OnUpdate` grown by 52 B (the `lhs_st` experiment),
`ModStation::ModStation` measured 48/15 instead of 43/12.  **Every sweep
below restores the other three targets to their base source before
building**, and the numbers are only comparable in that state.

## 1. Gun::calcCharacterCollision — inline shape has the original stack
##    slots but keeps the r4<->r5 Greedy-RA tie

HEAD is **463 / 57 / 1984** (named `Vector playerPosition`).  The
wave-154 inline shape (getPosition inlined into the subtraction) is
**467 / 67 / 1984**: it has the original's stack slots (getPosition sret at
`sp+44`, subtraction temp at `sp+104`) but the allocator puts `i*3` and the
sret in r5 and the subtraction temp in r4, the exact reverse of the
original (i*3/sret r4, temp r5).  The original saves i*3 to r6 across the
getPlayer call (`mov r6,r4` ... `mov r4,r6`); ours does the same with r5.

New shapes measured this wave (all 1984 B unless noted, all reverted):

| shape | raw | aligned |
|---|---:|---:|
| HEAD (named `playerPosition`) | **463** | **57** |
| inline getPosition | 467 | 67 |
| inline + named `delta` | 467 | 67 |
| inline + `const Vector delta` | 467 | 67 |
| inline + explicit `AEMath::operator-` | 467 | 67 |
| inline + `Level *lvl` local | 463 | 57 |
| inline + `PlayerEgo *player` local | 463 | 57 |
| inline + `Vector &&playerPosition` | 463 | 57 |
| inline + `auto playerPosition` | 463 | 57 |
| inline + `const Vector playerPosition` | 463 | 57 |
| inline + `Vector playerPosition(...)` direct-init | 463 | 57 |
| inline + nested scope around `playerPosition` | 463 | 57 |
| `const` position/velocity | 463 | 57 |
| `auto` position/velocity | 463 | 57 |
| `Vector &` position/velocity | 1732 | 391 (1920 B) |
| velocity loaded before position | 478 | 80 |
| `playerPosition` hoisted before `position` | 1741 | 292 (1940 B) |
| `distance > 20000` -> `20000 < distance` | 463 | 57 |
| `distance > 10000` -> `10000 < distance` | 463 | 57 |
| `field_0x71` -> `field_0x71 != 0` | 463 | 57 |

The `i*3` vreg is `%526` (rewriter dump: `$r5 = t2ADDrs $r11,$r11,10`).
Greedy RA consistently picks r5; the sret and `i*3` share it and the
subtraction temp takes r4.  A source shape that flips the pair without
changing the 1984-byte body is still open.

## 2. ModStation::ModStation — volatile yaw load is still the only lever

HEAD remains **43 / 12 / 1008**.  The residual is the literal-pool order
(`YawIPad`/`Yaw` ldr order), `mov r0,r3` before `cmp r1,#0`, and the
PostRA schedule of `str r8,[sp,#184]` / `mov r4,r2` / `ldr r3,[r0,r8,lsl#2]`.
Both loads must stay `const volatile int *` bit-loads: making the pitch
load non-volatile turns it into a `vldr` float load (669/61).

New shapes measured this wave (all 1008 B, all reverted):

| shape | raw | aligned |
|---|---:|---:|
| HEAD | **43** | **12** |
| `yawTable` declared before `pitchTable` | 43 | 12 |
| no `pitchTable` local (direct `gHangarCamPitch`) | 43 | 12 |
| `const int *` tables | 43 | 12 |
| `const volatile int *` tables (`vol_ptr_both`) | 43 | 12 |
| `float pitch` declared after `yaw` | 43 | 12 |
| `!iPad ? Yaw : YawIPad` | 43 | 12 |
| `iPad == 0 ? Yaw : YawIPad` | 43 | 12 |
| `if (iPad) ... else ...` | 43 | 12 |
| `if (!iPad) ... else ...` | 43 | 12 |
| `const float *const` tables | 43 | 12 |
| `const float *volatile yawTable` | 278 | 149 |
| inline select in the load | 690 | 52 |
| yaw load before pitch load | 744 | 40 (1000 B) |
| pitch volatile, yaw non-volatile | 44 | 18 |
| pitch non-volatile, yaw volatile | 666 | 60 |

`pitch volatile / yaw non-volatile` (44/18) reproduces the original's
`mov r0,r3`-before-`cmp` but sinks the yaw pointer load; both-volatile
(43/12) keeps the pointer order.  No shape gets both.  Open.

## 3. MenuTouchWindow::OnTouchBegin — 29/15 is a hard Greedy-RA tie

HEAD is **29 / 15 / 1896**.  The 29 raw bytes are 15 runs, all register
fields or one 14-byte schedule block; there is no structural difference.
The five sites are: `this->listEntryWidth` (ip vs r4) in case 1/2, the
`field_0x138`/`field_0x1e0`/`[r4]`/`field_0x28` colouring in case 15, the
`field_0x20_top` load hoisted before `GetImage2DHeight`, and the second
`buttonInsetX` (r5 vs r4).

New shapes measured this wave (all 1896 B, all reverted):

| shape | raw | aligned |
|---|---:|---:|
| HEAD (B1 ref, B2 direct) | **29** | **15** |
| `Layout &l` used for buttonInsetX + top | 29 | 15 |
| `const Layout &layout` | 29 | 15 |
| `Layout *layout = Globals::layout` (pointer) | 29 | 15 |
| reorder second condition (`(leftMargin+height)+top`) | 292 | 35 (1892 B) |
| case-1/2 `y < leftMargin + top + listEntryWidth` reorders | 29-34 | 15-16 |
| `(leftMargin + top) + listEntryWidth` | 29 | 15 |
| `leftMargin + (top + listEntryWidth)` | 29 | 15 |
| `listEntryWidth + leftMargin + top` | 34 | 16 |

The `ip`/`r4` and `r5`/`r4` ties are Greedy-RA choices; every source
reordering canonicalises back to the same IR.  Open.

## 4. MGame::OnUpdate — LANDED: ccm receiver direct (removes one mov)

HEAD is **37 / 109 / 11440**.  The two `mov r0,r5` are the live
`Status *st` value in r5 at the mission-122 `getCurrentCampaignMission`
and `getPlayingTime` receivers.  `&Globals::status` is in r9; the original
uses `ldr.w r0,[r9]` at both sites.

The landed one-line change:

```cpp
int radioBase = Globals::status->getCurrentCampaignMission() == 122 ? 3164 : 3166;
```

removes the ccm `mov r0,r5` (the site becomes `ldr.w r0,[r9]`, matching
the original) and drops the aligned metric from 109 to 19.  The raw metric
rises 37 -> 318 because the ccm site is 2 B longer and the whole shared
tail shifts by 2 B; the store still uses `st->getPlayingTime()` (the second
`mov r0,r5; b.w 17e270`), so `st` stays live and the body stays 11440 B.

Measured variants (all 11440 B unless noted, all reverted):

| shape | raw | aligned | size |
|---|---:|---:|---:|
| HEAD (`st` ccm + store) | **37** | 109 | 11440 |
| **LANDED ccm direct, store via `st`** | 318 | **19** | 11440 |
| store direct, ccm via `st` | 914 | 109 | 11436 |
| both direct (no `st`) | 5148 | 597 | 11392 |
| `Status &st = *Globals::status` (ref, both) | 37 | 109 | 11440 |
| `Status &st` ref, store direct | 318 | 19 | 11440 |
| `Status &st` ref, LHS via `st` | 5253 | 761 | 11492 |
| store LHS via `st`, calls direct | 5253 | 761 | 11492 |
| store LHS+ccm via `st`, RHS direct | 5197 | 758 | 11488 |
| `Status *const st` | 37 | 109 | 11440 |
| `volatile Status *st`, all direct | 5148 | 597 | 11392 |
| `(void) st` / dead use after store | 5148 | 597 | 11392 |

Trade-off: raw 37 -> 318, aligned 109 -> 19, size unchanged.  The raw
regression is a pure 2-byte tail shift caused by the still-live store
`mov`; the aligned 19 is the true structural distance (only the store
`mov r0,r5; b.w` vs `b.w` remains).  Removing the store `mov` as well
(`Globals::status->getPlayingTime()`) makes `st` dead and the allocator
removes `&Globals::status` from r9, shrinking the body to 11392 (a
size-exact regression).  A source shape that keeps r9 without a live
`Status*` in r5 is still open.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10518
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w10518/fdiff_w.py <mangled>
python3 /dev/shm/w10518/align_diff_w.py <mangled>
```

Scratch (not committed), `/dev/shm/w10518/`: `fdiff_w.py`,
`align_diff_w.py`, `measure.sh`, `cc.sh`, `gun_sweep*.py`,
`mod_sweep*.py`, `mtw_sweep*.py`, `mg_sweep*.py`, `*.base.cpp`.

## 6. Files changed

* `src/game/menu/MGame.cpp` — mission-122 `radioBase` now reads
  `Globals::status->getCurrentCampaignMission()` instead of `st->...`.
  Size 11440 unchanged; raw 37 -> 318, aligned 109 -> 19.
* `_work/reconstructions/small-tail-wave160-w10518.md` (this note).

Gate: byte 4507 / linked 4507, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, RATCHET PASS, avg 99.93.
