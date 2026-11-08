# Extab LP-identity swaps (wave 148, w9419): MBP reads **machine-level** MBFI,
# and the Radar::Radar tie needs a branch weight clang-7 cannot spell

Status: **analysis only, no landed `src/` change**. Worktree `wt/w9419`, base
`cbe3b5f7f` (wave 147). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0. `.ARM.extab` 1,309 differing bytes at exact 156,312 B; `.text`
86,160; `.ARM.exidx`/`.rodata`/`.data`/`.bss` exact.

Targets and unchanged metrics (raw same-address diff / instruction-index
alignment / extab bytes):

| fn | bodyraw | aligned | extab |
|---|---:|---:|---:|
| `Radar::Radar` (`0x12d484`) | **0** | **0** | **7** |
| `SpaceLounge::drawLounge` (`0x170d38`) | 176 | 42 | 14 |
| `ModStation::ModStation` (`0xd52d0`) | 43 | 12 | 2 |

## 0. New result: MBP does **not** use the IR-level `BlockFrequencyInfo`

w9317's analysis read the `-debug-only=block-freq` dump of the *IR* function
(`lpad` 519.2697, `lpad219` 519.5021, both truncating to 519) and concluded the
tie is a truncation artifact. That is true for the IR BFI, but
`MachineBlockPlacement` calls `MBFI->getBlockFreq()` on the **machine CFG**, and
the machine-level scaling can be different. Instrumented `llc` prints both:

```
# IR-level BFI (Analysis/BlockFrequencyInfoImpl.cpp)
float-to-int: min = 0.00000017814539, max = 9.564169797, factor = 44907140.16
 - lpad:    float = 0.00001156318791, scaled = 519.2697003, int = 519
 - lpad219: float = 0.00001156836373, scaled = 519.5021314, int = 519

# machine-level MBFI (same DEBUG_TYPE, later pass; MBP reads this)
float-to-int: min = 0.00000017814539, max = 9.564169797, factor = 44907140.16
 - BB195[lpad452]: float = 0.00000017814539, scaled = 8.0, int = 8
```

In the base build the two agree (both 519/519, min = `lpad452`). But they can
diverge: probe J below has IR BFI 649/649 and MBFI **1019/1020**. So future
work must read the `MBPTRACE ... CAND ... freq=` line (or the machine-level
`float-to-int` dump), not the first `block-freq` section.

The MBP tie-break itself is unchanged: for EH pads it keeps the least probable
block and, on an exact tie, the **later worklist entry** wins. The worklist is
`... lpad152, lpad, lpad162, ... lpad226, lpad219, ...`, so a 519/519 tie lays
`lpad219` first (ours); the original lays `lpad` first, i.e. it needs
`freq(lpad) < freq(lpad219)` strictly.

## 1. The min block is `lpad452`, and only the `autoTurret` guard can move it

`lpad452` is the landing pad of the `new Array<KIPlayer*>()` invoke in the
`if (autoTurret != nullptr)` tail block. Its MBFI frequency is

```
freq(lpad452) = P(autoTurret != nullptr) * freq(invoke.cont446) / 2^20
```

and it is the unique minimum (next: `lpad342/330/323` at 2.85e-7). The scaling
factor is `8 / min`, so the scaled pad frequencies are
`519.2697 * k` and `519.5021 * k` with `k = 0.625 / P` (0.625 is the pointer
heuristic `20/32`). The tie therefore breaks iff `floor(519.2697 k) <
floor(519.5021 k)`, which happens only for `k` in the narrow intervals
`[n/519.5021, n/519.2697)` for integer `n` (density ≈ 0.23 of `k`-space, but
each interval is only ~6e-4 wide near `k=1`).

Achievable clang-7 probabilities and the resulting `k`:

| P | source | k | scaled | tie? |
|---|---|---:|---|---|
| 0.625 | `p != nullptr` (pointer heuristic) | 1.0 | 519.27 / 519.50 | **tie** |
| 0.5 | non-heuristic compare (`(unsigned)p > 2`) | 1.25 | 649.09 / 649.38 | **tie** |
| 2000/2001 | `__builtin_expect(p != nullptr, 1)` | 0.6253 | 324.71 / 324.85 | **tie** |
| 1/2001 | `__builtin_expect(p != nullptr, 0)` | 1250.6 | 649411 / 649702 | **split** |
| 0.3125 | 0.625 * 0.5 | 2.0 | 1038.54 / 1039.00 | **split** |
| 0.125 | 0.625 * 0.5 * 0.5 | 5.0 | 2596.35 / 2597.51 | **split** |

So a split needs `P = 0.3125` or `0.125` (or `1/2001`). clang-7 has no
`__builtin_expect_with_probability` (verified: undeclared in 7.0.2), and the
heuristics only emit 20/32, 1/2, 124/128, 4/68, 1/2^32 or the 2000:1
`__builtin_expect` ratio. There is no source spelling for 0.3125/0.125 that
does not add a second real condition (which changes the body).

## 2. New body-identical shape: early `return`

The tail can be written

```cpp
Item *autoTurret = Globals::status->getShip()->getFirstEquipmentOfSort(37);
if (autoTurret == nullptr) {
    return;
}
this->players = new Array<KIPlayer *>();
Array<KIPlayer *> *players = this->players;
(void) players;
ArraySetLength(autoTurret->getAttribute(55), *this->players);
```

This is **byte-identical** (bodyraw 0, size 2488) and extab-identical (7) to the
landed `if (autoTurret != nullptr) { ... }` form. It is a genuinely different
CFG shape (the epilogue is the branch target) but MBP still lays the true block
first and the pads still tie at 519/519. It is a useful alternative spelling for
future weight experiments, not a win by itself.

## 3. Probes measured this wave (instrumented `llc`, `MBPTRACE=1`)

`MBP` = `CAND` frequencies for `lpad` / `lpad219`; `order` = MERGE order
(`lpad` first = original). All probes use the landed body unless noted.

| probe | MBP | order | body |
|---|---|---|---|
| base | 519 / 519 | lpad219 first | 0 |
| early `return` (§2) | 519 / 519 | lpad219 first | **0** |
| early `return` + `expect(p == nullptr, 0)` | 324 / 324 | lpad219 first | 0 |
| early `return` + `expect(p == nullptr, 1)` | 649411 / 649702 | **lpad first** | 167 |
| `expect(p != nullptr, 0)` (no return) | 649411 / 649702 | **lpad first** | 167 |
| `expect(p != nullptr, 1)` | 324 / 324 | lpad219 first | 0 |
| `(unsigned)p > 2` (P=0.5) | 649 / 649 | lpad219 first | – |
| `p > (Item*)0` (canonicalises to `!=`) | 519 / 519 | lpad219 first | – |
| `p != nullptr && w > 100` (P=0.5) | 649 / 649 | lpad219 first | – |
| `p != nullptr && w > 100 && h > 100` | 1019 / 1020 | **lpad first** | 755 (size +16) |
| `expect(turret != nullptr, {0,1})` | 519 / 519 | lpad219 first | – |
| nested `if (inAlienOrbit) if (dlc1Won) if (!inEmptyOrbit)` | 519 / 519 | lpad219 first | – |
| `bool alien = a && b && !c; if (alien)` | 299 / 219 | lpad219 first | 2379 (size +12) |
| `for (i = 0; i < 5; ++i)` / `while` / `i++` | 519 / 519 | lpad219 first | not built |
| `expect(i != 5, {0,1})` on the asteroidSlots loop | 519 / 519 | lpad219 first | – |

The `p != nullptr && w > 100 && h > 100` probe is the only body-changing one
that reaches the original LP order; it costs 755 raw bytes and +16 B of
`.text`, so it is not landable. Note its IR BFI is 649/649 while its MBFI is
1019/1020 — the extra `&&` blocks lower the machine-level minimum.

## 4. `ModStation::ModStation` (2 B): literal-pool load order

The 43 raw bytes are two regions: `0x169..0x19f` (the `MatrixSetRotation`
setup) and `0x3e4..0x3e9` (the literal pool). The root cause is the
instruction-scheduler order of the three PC-relative table loads and of the
`mov r4, r2` copy that preserves `pitch` across the call:

```
orig: ldr r0; ldr r3; ldr r4; add r0; add r3; ldrb r1; add r4; ldr r2; ...
      ... mov r0,r8; mov r4,r2; blx MatrixSetRotation
ours: ldr r0; ldr r4; ldr r3; add r0; ldrb r1; add r4; ldr r2; add r3; ...
      ... str r8; mov r4,r2; ldr r3; ... mov r0,r8; blx
```

The different literal-load order permutes the literal pool, which is the second
diff region. The extab row `(390,30,922)` vs `(398,22,922)` is the same
`mov r4,r2` placement (the protected range starts at the first instruction of
the invoke region).

New probes this wave (all leave extab at 2): swap the `iPad` ternary
(`!iPad ? yaw : yawIPad`), reorder the `pitchTable`/`yawTable` declarations,
reorder the `yaw`/`pitch` locals. Reordering `pitchBits`/`yawBits` *loads*
worsens the body (46) or the size (1000). This is a scheduler choice, not a
source-order choice; no lever found.

## 5. `SpaceLounge::drawLounge` (14 B)

Still downstream of the 176-byte body (the pad/x `RAGreedy` priority-hint swap
of w9020 §2). `__builtin_expect(agent->isKnown(), 0)` grows the function to
1980 B (section-size regression) and extab to 80; the landed `expect(...,1)` is
still the best shape. The `{-10:1}` row is the LP identity swap; the five
`{+2:5}` rows are address shifts and will fall with the body.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9419
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6        # RATCHET PASS
python3 /dev/shm/w9419/fdiff.py _ZN5RadarC2EP5Level
python3 _work/reconstructions/align_diff.py _ZN5RadarC2EP5Level
python3 _work/reconstructions/extab_census.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so \
  .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
```

MBP trace (instrumented clang, not committed):

```sh
MBP_TRACE=1 /tmp/llvm_android_702b/build2/bin/llc \
  -mtriple=armv7-none-linux-androideabi16 -mattr=+neon -float-abi=soft -Oz \
  -debug-only=block-freq /dev/shm/w9419/base.ll -o /dev/null
```

Scratch (not committed), `/dev/shm/w9419/`: `fdiff.py`, `dranges.py`,
`ll.sh`, `try.sh`, `ms_try.sh`, `Radar_base.cpp`, `Radar_{A..AE}.cpp`,
`ModStation_{base,1..4}.cpp`, `SpaceLounge_{base,1}.cpp`, `base.mbp`,
`A.mbp`..`AE.mbp`.

## 7. Open questions

1. A source spelling that yields `P(autoTurret != nullptr) = 0.3125` (or
   `0.125`) **without adding an instruction**. The only candidates are a
   second real condition (adds a load/compare/call) or a future
   `__builtin_expect_with_probability` (not in clang 7.0.2).
2. A way to move `lpad` later in the MBP worklist (then the 519/519 tie would
   favour it). The worklist order follows the machine layout of the
   predecessors, which is fixed by the byte-exact body.
3. Recovering the original PGO weights (`.profdata`/`.profraw`) would settle
   both Radar::Radar and the ModStation scheduling question directly.
