# Build provenance round 8 (w11319): remaining per-TU flag families; one more
# size-exact raw win (Layout.cpp ARM `+prof-unpr`)

Status: **research note + one landed per-TU matching knob**, 2026-11-11.
Worktree `wt/w11319`, branch `wave168-w11319`, base master `631c26f0b`
(wave 167). Scratch `/dev/shm/w11319`. Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Gate on entry: **RATCHET PASS**, `byte_exact 4507/4524`, `linked_exact
4507/4524`, size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0,
parity 5370/5370, `.text` **82,526** differing bytes, `.ARM.extab` **670**,
`.rel.plt` **0** (byte-identical).

Gate on exit: **RATCHET PASS**, same counts, `.text` **82,525** (-1),
`.ARM.extab` **670**, `.rel.plt` **0**. The only source-tree change is one
per-file `COMPILE_OPTIONS` line in `CMakeLists.txt`.

## 0. Bottom line

1. Round 7's driver/ARM sweep is extended here with the families it did not
   isolate with a *correct* clang-7 spelling: the full `-march=` /
   `-mcpu=` / `-mfpu=` value space, the `-Xclang -target-feature` ARM
   subtarget-feature set in both polarities, and combinations of the
   `+prof-unpr` feature with every other feature and with the round-5/6/7
   per-TU knobs.
2. **One new size-exact, `byte_exact`-neutral raw win landed:**
   `Layout.cpp -Xclang -target-feature -Xclang +prof-unpr`.
   `Layout::Layout` C1/C2 raw **5,658 -> 5,657** (-1), aligned
   **2,280 -> 2,276** (-4), size 7,072 B; whole `.text`
   **82,526 -> 82,525**. Every alloc section (`.ARM.extab` = 670,
   `.rodata`, `.data`, `.bss`, `.rel.plt` = 0) is unchanged.
3. **No other flag makes any residual function byte-exact.** The residual is
   still source/TU shape, consistent with rounds 5-7.
4. The driver `-fno-*` families behave exactly as round 7 reported:
   `-fno-threadsafe-statics`, `-fno-rtti`, `-fno-delete-null-pointer-checks`
   and `-fvisibility=default` are exact no-ops on all 13 residual TUs;
   `-fno-exceptions` and `-fno-strict-aliasing` are size-changing; `-fwrapv`
   is strict only on Radar (one byte swaps, net raw unchanged).
5. Many ARM `-m*` spellings round 7 listed are not accepted by this clang 7
   driver (`-mno-neon`, `-mno-vfpv3`, `-mno-vmlx`, `-mno-hwdiv`,
   `-mno-ldrexd`, `-mno-thumb-interwork`, `-mmovt`, `-mno-crc`,
   `-mno-sha2`, `-mno-fp16`, `-mfp16-format=*`, `-mno-strict-align`, ...).
   The `-Xclang -target-feature` spelling reaches the same subtarget
   features and was swept here instead.

## 1. Baseline (this worktree)

| function | mangled | raw | aligned | size |
|---|---|---:|---:|---:|
| `Layout::Layout` C1/C2 | `_ZN6LayoutC1Ev` / `C2` | 5,658 | 2,280 | 7,072 |
| `LevelScript::process` | `_ZN11LevelScript7processEi` | 54,031 | 17,764 | 57,232 |
| `Level::createShip` | `_ZN5Level10createShipEiiiP8Waypointbb` | 1,560 | 455 | 2,764 |
| `Level::createMission` | `_ZN5Level13createMissionEv` | 5,435 | 767 | 17,172 |
| `Status::missionCompleted` | `_ZN6Status16missionCompletedEbbx` | 722 | 320 | 1,432 |
| `Radar::draw` | `_ZN5Radar4drawEP6PlayerP3Hudi` | 7,487 | 1,454 | 9,908 |
| `MGame::OnTouchEnd` | `_ZN5MGame10OnTouchEndEiiPv` | 4,082 | 1,018 | 8,080 |
| `MGame::OnUpdate` | `_ZN5MGame8OnUpdateEv` | 15 | 111 | 11,440 |
| `Gun::calcCharacterCollision` | `_ZN3Gun22calcCharacterCollisionEv` | 463 | 57 | 1,984 |
| `HangarWindow::render` | `_ZN12HangarWindow6renderEv` | 2,245 | 620 | 9,700 |
| `SpaceLounge::drawLounge` | `_ZN11SpaceLounge10drawLoungeEv` | 175 | 41 | 1,948 |
| `Hud::draw` | `_ZN3Hud4drawExxP9PlayerEgobjj` | 762 | 222 | 9,332 |
| `Generator::createMission` | `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 127 | 107 | 1,996 |
| `MenuTouchWindow::OnTouchBegin` | `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 28 | 16 | 1,896 |
| `ModStation::ModStation` C1/C2 | `_ZN10ModStationC1Ev` / `C2` | 43 | 12 | 1,008 |

13 TUs (Layout C1 is an alias of C2; ModStation likewise).

## 2. Method

Three stages, as in rounds 6-7:

1. **Object-level filter.** For each (TU, flag) cell the canonical compile
   command is taken from `ninja -t commands` (so the per-file StarMap /
   SpaceLounge / LevelScript / Radar / Level knobs are already in it) and
   the candidate flag appended. The TU's object is recompiled and compared
   with the canonical object. A strict cell survives only if every
   `.text.*` section keeps its exact size, every non-target function keeps
   its exact bytes, and at least one target function changes. "Relaxed"
   cells (target and one non-target function change, all sizes exact) are
   recorded separately.
2. **Link measurement.** Each strict survivor is linked with the real gold
   link and measured with `tools/fn_diff.py` plus a direct whole-`.text`
   byte diff against the original. The harness restores every canonical
   object before each cell, copies the flagged object over the canonical
   path, runs the exact `ninja -t commands link_so` link line, measures,
   and restores the canonical object; the canonical `.so` is relinked at
   the end.
3. **Official gate.** The landed cell was rebuilt in `cmake-build-match`
   and `check` was run.

Parallelism was 4 concurrent single-TU clang invocations with
`ulimit -v 12000000`; links were sequential.

## 3. Flag space swept

* **Driver / language flags (20 x 13 = 260 cells):**
  `-fno-threadsafe-statics`, `-fno-exceptions`, `-fno-rtti`,
  `-fno-strict-aliasing`, `-fwrapv`, `-fno-delete-null-pointer-checks`,
  `-fvisibility={default,hidden,protected,internal}`,
  `-fvisibility-inlines-hidden`,
  `-fvisibility-global-new-delete-hidden` (rejected by clang 7), and the
  `-fno-threadsafe-statics` / `-fno-rtti` / `-fno-strict-aliasing` /
  `-fwrapv` / `-fno-delete-null-pointer-checks` pairings.
* **ARM driver flags (74 x 13 = 962 cells):** `-mcpu=` (a5/a7/a8/a9/a15/
  generic), `-mtune=` (same), `-mfpu=` (vfpv3-d16/vfpv3/vfpv4/neon/
  neon-vfpv4/neon-fp16/neon-fp-armv8), `-mfloat-abi=soft/hard`,
  `-marm`/`-mthumb`, `-m[no-]unaligned-access`, `-m[no-]movt`,
  `-m[no-]restrict-it`, `-mimplicit-it=*`, `-m[no-]thumb-interwork`,
  `-m[no-]global-merge`, `-mtp=soft/cp15/auto`, `-m[no-]neon`,
  `-m[no-]vfpv3`, `-m[no-]vmlx`, `-m[no-]dsp`, `-m[no-]hwdiv`,
  `-mhwdiv=thumb/arm/arm,thumb`, `-m[no-]ldrexd`, `-mslow-flash-data`,
  `-mlong-calls`, `-mvectorize-with-neon-quad`, `-m[no-]crc`,
  `-m[no-]aes`, `-m[no-]sha2`, `-m[no-]fp16`, `-mfp16-format=*`,
  `-m[no-]strict-align`, `-march=armv7-a/armv7ve`,
  `-m[no-]odd-spreg`, `-m[no-]ldrd`, `-m[no-]omit-leaf-frame-pointer`,
  `-m[no-]tp2`, `-m[no-]merge-fp-loads`, `-m[no-]merge-fp-stores`, and
  combinations.
* **ARM `-march=` / `-mcpu=` / `-mfpu=` value space (70 x 13 = 910
  cells):** 22 `-march=` values (armv5te ... armv8.5-a, armv8-m/r),
  21 `-mcpu=` values (cortex-a5..a17, cortex-r4..r7, cortex-m0..m7,
  arm1176jzf-s, arm926ej-s, arm1136jf-s, arm1156t2-s, generic, none),
  23 `-mfpu=` values (vfp ... crypto-neon-fp-armv8), and 10 combinations.
* **ARM subtarget features, both polarities (120 x 13 = 1,560 cells):**
  `-Xclang -target-feature -Xclang ±<feature>` for vfp2/vfp3/vfp4/
  fp-armv8/neon/fp16/d16/fp-only-sp/crypto/dotprod/fullfp16/fp16fml/ras/
  v8..v8.5a/v8m*/hwdiv/hwdiv-arm/dsp/mclass/thumb2/thumb-mode/v7*/v6*/
  v5te/v5t/v4t/crc/aes/sha2/neonfp/perfmon/speck/mve/strict-align/
  unaligned-access/long-calls/slow-flash-data/read-tp-hard/read-tp-soft/
  reserve-r9/no-branch-predictor/use-misched/execute-only/prof-unpr/
  avoid-partial-cpsr/fix-cortex-a57-aes-1742098/
  fix-cortex-a72-aes-1655431/muxed-units/expand-fp-mlx/no-trapping-math/
  disable-postra-scheduler/fuse-aes/fuse-literals/ret-addr-stack/lob/
  disable-zero-copy/vmlx-forwarding/neonfp16arith/fp16arith/
  splat-vfp-neon.
* **`+prof-unpr` combinations (132 x 13 = 1,716 cells):** `+prof-unpr`
  plus every feature in both polarities, plus `-fwrapv`,
  `-fno-strict-aliasing`, `-fno-threadsafe-statics`,
  `-fno-delete-null-pointer-checks`, `-mhwdiv=thumb`, `-march=armv7ve`,
  `-mno-unaligned-access`, `-mstrict-align`, and the round-5/6/7
  `-mllvm` knobs.

Total **6,500 object-level cells** (1,404 + 2,964 + 2,132), plus every
strict survivor link-measured.

## 4. The landed cell

### `Layout.cpp -Xclang -target-feature -Xclang +prof-unpr`

The ARM `prof-unpr` subtarget feature is off in the canonical command.
Enabling it changes exactly one byte in `Layout::Layout`, leaves every
function size, every other function in the TU and every alloc section
untouched, and moves that byte into agreement with the original. Linked,
`Layout::Layout` raw **5,658 -> 5,657**, aligned **2,280 -> 2,276**, size
7,072 B; whole `.text` **82,526 -> 82,525**. `.ARM.extab` stays 670 and
`.rel.plt` stays byte-identical (0).

| | raw | aligned | size | `.text` diff |
|---|---:|---:|---:|---:|
| before | 5,658 | 2,280 | 7,072 | 82,526 |
| after | **5,657** | **2,276** | 7,072 | **82,525** |

The same feature is a no-op on LevelScript (raw unchanged, whole `.text`
unchanged) and is *worse* on Hud (`Hud::draw` 762 -> 783, whole `.text`
+21). It is a matching knob, not evidence that the original build enabled
`prof-unpr` globally.

## 5. All link-measured strict candidates

Baseline `.text` 82,526. `delta` is the whole-`.text` differing-byte count
for that cell alone. Rows marked `link-fail` are size-exact at the object
level but produce a gold architecture-profile conflict.

| TU | flag | target raw | delta | verdict |
|---|---|---|---:|---:|
| **Layout** | **`-Xclang -target-feature -Xclang +prof-unpr`** | **5,658 -> 5,657** | **-1** | **landed** |
| LevelScript | `-march=armv7ve` | 54,031 -> 54,034 | +3 | worse |
| LevelScript | `-Xclang -target-feature -Xclang +hwdiv` | 54,031 -> 54,034 | +3 | worse |
| LevelScript | `-Xclang -target-feature -Xclang +prof-unpr` | 54,031 | 0 | neutral |
| LevelScript | `-march=armv7ve -mfpu=neon -mfloat-abi=softfp` | 54,031 -> 54,034 | +3 | worse |
| LevelScript | `-march=armv7-a -mhwdiv=thumb -mno-unaligned-access` | 54,031 -> 54,034 | +3 | worse |
| LevelScript | `-mhwdiv=thumb` | 54,031 -> 54,034 | +3 | worse |
| LevelScript | `-mhwdiv=arm,thumb` | 54,031 -> 54,034 | +3 | worse |
| LevelScript | `-march=armv7-r` | - | link-fail | R/A profile |
| LevelScript | `-march=armv8-m.main` | - | link-fail | M/A profile |
| LevelScript | `-mcpu=cortex-r4` | - | link-fail | R/A profile |
| Radar | `-fwrapv` | 7,487 | 0 | neutral (1 byte swap) |
| Radar | `-fwrapv -fno-delete-null-pointer-checks` | 7,487 | 0 | neutral |
| MGame | `-Xclang -target-feature -Xclang +mclass` | 4,082 -> 3,775; 15 -> 257 | +242 | worse |
| MGame | `-march=armv8-m.main` | - | link-fail | M/A profile |
| Hud | `-Xclang -target-feature -Xclang +prof-unpr` | 762 -> 783 | +21 | worse |

### Relaxed candidates (`+d16`, size-exact, non-target function also moves)

These change an exact constructor in the same TU, so they would drop
`byte_exact`; all are link-measured worse and were not landed.

| TU | flag | target raw | delta |
|---|---|---|---:|
| LevelScript | `-Xclang -target-feature -Xclang +d16` | 54,031 -> 54,034 | +3 |
| Level | `-Xclang -target-feature -Xclang +d16` | 1,560; 5,435 -> 5,483 | +239 |
| MGame | `-Xclang -target-feature -Xclang +d16` | 4,082 -> 3,775; 15 -> 23 | +43 |
| Gun | `-Xclang -target-feature -Xclang +d16` | 463 -> 467 | +16 |
| ModStation | `-Xclang -target-feature -Xclang +d16` | 43 -> 55 | +25 |

### `+prof-unpr` combination sweep

262 further strict cells (133 LevelScript, 125 Hud, plus one each in
Layout/Gun/HangarWindow/Generator) were queued on top of `+prof-unpr`;
260 linked and 2 (`+v8m.main`) hit a gold unknown-CPU-architecture
failure. **None improved 82,526.** 129 LevelScript cells were neutral
(`+prof-unpr` cancels the `-march=armv7ve`/`+hwdiv` byte), 3 were +3 and
one +71; the 124 Hud cells were uniformly +21 (the `+prof-unpr` Hud
regression is not recovered by any feature polarity). The Layout control
`+prof-unpr -prof-unpr` correctly returns raw 5,658 and delta 0.

## 6. Driver-family observations

* `-fno-threadsafe-statics`, `-fno-rtti`, `-fno-delete-null-pointer-checks`
  and `-fvisibility=default` are exact no-ops on all 13 TUs (no guarded
  function-local statics, no RTTI, no null checks to delete; default
  visibility is already the canonical setting).
* `-fno-exceptions` is size-changing on every TU with EH cleanup
  (`__clang_call_terminate` disappears, `.ARM.extab` collapses); 5 TUs
  change `.text.*` sizes and 8 change the section set. Not usable.
* `-fno-strict-aliasing` is size-changing on 12 of 13 TUs (only Layout
  target bytes are unchanged).
* `-fwrapv` is strict only on Radar (one byte swaps, net raw unchanged);
  it is size-changing on 8 TUs.
* `-fvisibility=hidden/protected/internal` are size-changing on 6 TUs
  (they change PLT/GOT codegen) and no-ops on the other 7.
* `-fvisibility-inlines-hidden` is a no-op on 12 TUs and size-changing on
  one.

## 7. ARM target-feature observations

* The clang-7.0.2 ARM driver rejects many spellings that round 7 listed:
  `-mno-neon`, `-mno-vfpv3`, `-mno-vmlx`, `-mno-hwdiv`, `-mno-ldrexd`,
  `-mno-thumb-interwork`, `-mmovt`, `-mno-crc`, `-mno-sha2`, `-mno-fp16`,
  `-mfp16-format=*`, `-mno-strict-align`, `-mvectorize-with-neon-quad`,
  `-mslow-flash-data`, `-mno-tp2`, `-m[no-]merge-fp-*`,
  `-fvisibility-global-new-delete-hidden`. The `-Xclang -target-feature`
  spelling is the correct way to reach those subtarget bits.
* `-march=armv7ve`, `-march=armv7-r`, `-march=armv8-m.main`,
  `-mcpu=cortex-r4`, `+hwdiv`, `+mclass` are the only driver/feature
  movers that survive the object filter. All are worse or fail to link
  against the armv7-a objects (gold rejects mixed R/A and M/A profiles).
* `+prof-unpr` is the only feature that improves a residual function.
  It is off by default in clang 7 and is a per-TU matching knob.
* `-mfpu=neon` and `-mfloat-abi=softfp` are already the canonical
  settings; `-mfpu=neon-vfpv4`/`neon-fp16` are exact no-ops on all 13 TUs.

## 8. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11319
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so verify check -j2
```

Scratch artifacts: `/dev/shm/w11319/{common.py,sweep.py,sweep2.py,
sweep3.py,link.py,link2.py,link_d16.py,results*.json,link_results*.json}`.

## 9. Files changed

* `_work/reconstructions/build-provenance-round8-per-tu-flags.md` — this note.
* `CMakeLists.txt` — per-file
  `-Xclang -target-feature -Xclang +prof-unpr` on `Layout.cpp`.
