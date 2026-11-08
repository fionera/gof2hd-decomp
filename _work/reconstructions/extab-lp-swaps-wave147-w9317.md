# Extab LP-identity swaps (wave 147, w9317): Radar::Radar root cause = MBP EH-pad
# frequency tie, not a source scope shape

Status: **analysis only, no landed `src/` change**. Worktree `wt/w9317`, base
`9c9744ca5` (wave 146). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0. `.ARM.extab` 1,393 differing bytes at exact 156,312 B; `.text` 85,928;
`.ARM.exidx`/`.rodata`/`.data`/`.bss` exact.

Targets: the three pure LP-identity swaps from w9219 — `Radar::Radar` (7 B,
bodyraw 0), `ModStation::ModStation` (2 B, bodyraw 43), `SpaceLounge::drawLounge`
(14 B, bodyraw 175).

## 0. New result: the swap is a **MachineBlockPlacement EH-pad frequency tie**

`Radar::Radar` (`0x12d484`) has 39 LSDA rows in both binaries. Exactly 7 rows
differ, and they are a clean 2-block permutation:

| rows | orig lp | ours lp | scope |
|---|---|---|---|
| 1, 3, 11, 13, 15 | 2462 (`0x12de22`) | 2464 (`0x12de24`) | function-level cleanup (`%175`) |
| 19, 21 | 2464 (`0x12de24`) | 2462 (`0x12de22`) | `enemyGuns`+Image2D block (`%372`) |
| 20 | 2466 | 2466 | `enemyGuns` loop (`%376`) |

`0x12de22` and `0x12de24` are two adjacent identical `b.n 0x12de28`
trampolines (the cleanup body at `0x12de28` destroys the member `String` at
`this+0x18c` and resumes). Because `.text` is byte-identical, the only
observable is which IR landingpad block got which of the two addresses.

The mapping is decided by `MachineBlockPlacement::selectBestCandidateBlock`
(`lib/CodeGen/MachineBlockPlacement.cpp:1574`). For EH pads the loop keeps the
**least probable** block, and on an exact frequency tie the **later** worklist
entry wins:

```cpp
if (BestBlock && (IsEHPad ^ (BestFreq >= CandidateFreq)))
  continue;
```

Instrumented clang (`/tmp/llvm_android_702b/build2`, `MBP_TRACE=1`) on the
landed source:

```
WORKLIST size=30: ... %bb.60 ... %bb.88 %bb.87 ...
CAND %bb.60 freq=519      <- function-level cleanup %175
CAND %bb.87 freq=519      <- enemyGuns cleanup     %372
CAND %bb.88 freq=7837338
MERGE %bb.73 -> %bb.87
MERGE %bb.87 -> %bb.60    <- enemyGuns pad laid out first (0x12de22)
```

So the two pads are **exactly tied at 519**, and the tie-break (later worklist
entry wins) lays the `enemyGuns` pad first. The original has the function-level
pad first, i.e. `freq(%175) < freq(%372)`. This is *not* a source scope shape:
the IR/scope structure is identical (the row boundaries match); only the pad
frequency ordering differs.

## 1. Why the frequencies differ in the original: `!prof` (PGO)

The invoke unwind-edge probability is **fixed** at `IH_NONTAKEN_WEIGHT`
= 1/2^20 by `BranchProbabilityInfo::calcInvokeHeuristics`; `calcMetadataWeights`
explicitly ignores `InvokeInst` (`BranchProbabilityInfo.cpp:249`:
`if (!(isa<BranchInst> || isa<SwitchInst> || isa<IndirectBrInst>)) return false;`).
So injecting `!prof` on an invoke does nothing (verified: the MIR still shows
`%bb.80(0x7ffff800), %bb.87(0x00000800)`).

The pad frequency is therefore `sum(pred_freq) / 2^20`. The predecessor block
frequencies are set by the *body* branch weights. The w9214 finding (the
original was built with branch weights / PGO) explains why the original's pad
frequencies are not tied: its body branch probabilities differ from our
unweighted build even though the final `.text` layout is identical.

## 2. `__builtin_expect` probes on `Radar::Radar`

`__builtin_expect` is ordinary C++ and is allowed. Measured on the real TU +
gold link (`extab` = differing `.ARM.extab` bytes for `0x12d484`, `body` =
raw same-address `.text` diff):

| variant | extab | body |
|---|---:|---:|
| base | **7** | **0** |
| `expect(inAlienOrbit && dlc1Won && !inEmptyOrbit, 1)` | 7 | 0 |
| `expect(dlc1Won(), 1)` | 7 | 0 |
| `expect(inEmptyOrbit(), 0)` | 7 | 0 |
| `expect(turret != nullptr, 1)` | 7 | 0 |
| `expect(autoTurret != nullptr, 1)` | 10 | 6 |
| `expect(enemyGuns != nullptr, 1)` | 25 | 43 |
| `expect(playerGuns != nullptr, 1)` | 29 | 57 |
| `expect(inAlienOrbit && ..., 0)` | 78 | 1799 |
| `expect(dlc1Won(), 0)` | 84 | 1765 |
| `expect(inEmptyOrbit(), 1)` | 78 | 1799 |
| `expect(playerGuns != nullptr, 0)` | 132 | 2009 |
| `expect(enemyGuns != nullptr, 0)` | 108 | 1262 |

The `!prof` trace confirms the mechanism: the "likely" alien-orbit variant
(body still byte-exact) moves the function-level pad from **519 to 684**:

```
CAND %bb.60 freq=684     <- function-level pad
CAND %bb.87 freq=519     <- enemyGuns pad
```

That is the *wrong direction* (we need `freq(early) < freq(enemyGuns)`), and it
does not change the layout order. Every variant that lowers the early pad below
519 (`inAlienOrbit` unlikely / `inEmptyOrbit` likely / `dlc1Won` unlikely)
flips the body layout of the `if` block and costs 1.7-2.0 KB of raw diff.

**Conclusion:** the 7 bytes are a genuine PGO-frequency artifact. A source-only
fix would need `freq(%175) < freq(%372)` while keeping the body byte-exact; no
`__builtin_expect` placement found does both. This is a **METRIC-ONLY** target
in the sense that the original's weights are not recoverable from a
no-profile source build.

## 3. `ModStation::ModStation` (2 B): not EH at all

Row 3 is `(390,30,922)` orig vs `(398,22,922)` ours; both end at 420. The call
is `MatrixSetRotation` at `0xd5470`. The original loads the 4th argument
directly into `r3` (`ldr.w r3,[r0,r8,lsl#2]` at `0xd5456`); ours loads into
`r4` and then `mov r3,r4` (`0xd545a`/`0xd546c`). The row start moves 8 bytes
earlier because the protected range begins at the argument load. This is a
register-allocation difference, not an EH scope/lifetime construct. The w9219
probes (local `farYaw`, local `zero`, swapped `yaw`/table args) all leave the
row at `(398,22,922)`; re-measured this wave, unchanged.

## 4. `SpaceLounge::drawLounge` (14 B)

Still blocked on the body: 175 raw bytes, dominated by the pad/x register swap
documented in w9020 §2 (the `RAGreedy` priority-hint bit). The LP deltas
`{-10:1, +2:5}` are downstream of that body layout; once the body is exact the
swap should be re-measured. No new source shape tried this wave.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9317
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6        # RATCHET PASS
python3 _work/reconstructions/extab_census.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so \
  .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
```

MBP trace (instrumented clang, not committed):

```sh
MBP_TRACE=1 /tmp/llvm_android_702b/build2/bin/llc \
  -mtriple=armv7-none-linux-androideabi16 -mattr=+neon -float-abi=soft -Oz \
  Radar.ll -o /dev/null
```

Scratch (not committed), `/dev/shm/w9317/`: `measure.py`, `fdiff.py`,
`run_variant.sh`, `sweep*.py`, `Radar.base.cpp`, `Radar.ll`, `mir_bp.txt`,
`radar_mbp.txt`, `trace_alien.txt`.

## 6. Open questions

1. The original's PGO weights are the only remaining unknown for
   `Radar::Radar`. If a profile can be recovered (e.g. from the APK's
   `.profraw`/`.profdata` or a sibling build), replay it with `-fprofile-use`.
2. A source shape that lowers the function-level pad's frequency without
   flipping the `inAlienOrbit` `if` layout would zero the 7 bytes.

## 7. Wave-147 follow-up (w9317): the tie is a **truncation artifact**, and it is frequency-driven

The two pads are not exactly equal in the *unscaled* block-frequency domain; they only
collide because `convertFloatingToInteger` truncates. From
`llc -debug-only=block-freq` on the landed `Radar.ll`:

```
 - ir36: float = 0.00001156318791, scaled = 519.2697003, int = 519   <- %175 (function-level)
 - ir87: float = 0.00001156836373, scaled = 519.5021314, int = 519   <- %372 (enemyGuns)
```

`%175` is already the *smaller* float; the tie exists only because the scaling factor
(`ScalingFactor = Min.inverse() << 3`, i.e. `8/min`) maps both into `[519,520)`.
`Scaled64::toInt()` right-shifts (truncates), so both become 519. The original needs
`int(%175) < int(%372)`; that can be produced either by (a) a weight change that moves
one float across the 519/520 boundary, or (b) a **smaller minimum block frequency**
(a larger scaling factor), which would separate 519.27 and 519.50 into different ints.

The minimum block is a 5-instruction landing pad with a single predecessor whose first
instruction is a `bitcast` (`n5_landingpad_p1:bitcast_np1`, freq `0.00000017814539`,
scaled to 8; the predecessor's freq is `0.1868`, matching the `String(" ", false)`
temporary path in the `getStation()->getIndex() == 101` else-arm). Halving it would
double the scaling factor and break the tie in the correct direction.

### 7.1 Proof the swap is frequency-driven: `expect(isRocketGun(), 0)`

Applying `__builtin_expect((*playerGuns)[i]->isRocketGun(), 0)` in the playerGuns loop
**fixes the LP swap** (rows 1/3/11/13/15 -> 2462, rows 19/21 -> 2464, exactly the
original) but inverts the loop layout (`add r8`/`cmp`/`bcc` move to the top), costing
116 raw body bytes and shifting row 18 (`(798,84)` -> `(802,90)`), leaving extab 3.
So the swap is genuinely decided by the body branch weights, but the weight change
needed to flip it also flips the loop, i.e. no body-0 `__builtin_expect` reaches it.

### 7.2 Full single-condition `__builtin_expect` sweep (body-0 variants marked *)

| condition (expect 0 / 1) | extab / body (0) | extab / body (1) |
|---|---|---|
| `playerGuns != nullptr` | 132 / 2009 | 29 / 57 |
| `i < playerGuns->size()` | 83 / 1528 | 25 / 43 |
| `(*playerGuns)[i]->isRocketGun()` | **3 / 116** | 11 / 12 |
| `(*playerGuns)[i]->isBombGun()` | 60 / 1655 | 11 / 12 |
| `(*playerGuns)[i]->isMineGun()` | **3 / 122** | 11 / 12 |
| `enemyGuns != nullptr` | 108 / 1262 | 25 / 43 |
| `i < enemyGuns->size()` | 26 / 148 | 26 / 43 |
| `(*enemyGuns)[i]->isRocketGun()` | 57 / 1526 | 11 / 12 |
| `radarItem != nullptr` | 51 / 496 | 45 / 478 |
| `tractor != nullptr` | 28 / 301 | 45 / 397 |
| `i != 5` (asteroidSlots) | *7 / 0* | *7 / 0* |
| `turret != nullptr && getAttribute(50) != 0` | 49 / 235 | *7 / 0* |
| `autoTurret != nullptr` | 56 / 167 | 10 / 6 |

Plus the 13 earlier probes in §2 and the `inAlienOrbit`/`dlc1Won`/`inEmptyOrbit`
sub-condition probes (`ao1`, `dlc1`, `emp0` all *7 / 0*; `ao0` 88/1477, `dlc0`
84/1765, `emp1` 78/1799). **No variant gives extab 0 with body 0.** The best extab
without a body regression is the base 7.

### 7.3 Loop-form probes (all body 0-2, no swap change)

`i != size` (either loop) -> body 1; `i++` -> body 0; `while` (w9219) -> body 0.
None change the pad tie.

### 7.4 Conclusion

`Radar::Radar` remains a genuine PGO/weight artifact: the body is byte-exact, the
scopes and landing-pad sets are identical, and every source-level weight lever
(`__builtin_expect`, loop form, nesting) either leaves the tie at 519/519 or flips
the playerGuns loop. A profile (`-fprofile-use`) or a finer-grained weight primitive
than clang-7 `__builtin_expect` (which is hard-wired to 2000:1) is required.

Scratch for this follow-up: `/dev/shm/w9317/sweep6.py` .. `sweep11.py`.
