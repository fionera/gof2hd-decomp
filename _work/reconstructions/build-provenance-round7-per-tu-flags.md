# Build provenance round 7 (w11218): remaining per-TU flag families; one more
# size-exact raw win (Level.cpp EarlyCSE MemorySSA)

Status: **research note + one landed per-TU matching knob**, 2026-11-11.
Worktree `wt/w11218`, base master `d00057e6d` (wave 166). Scratch
`/dev/shm/w11218`. Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Gate on entry: **RATCHET PASS**, `byte_exact 4507/4524`, `linked_exact
4507/4524`, size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0,
parity 5370/5370, `.text` **82,898** differing bytes, `.ARM.extab` **660**,
`.rel.plt` **0** (byte-identical).

Gate on exit: **RATCHET PASS**, same counts, `.text` **82,897** (-1),
`.ARM.extab` **660**, `.rel.plt` **0**. The only source-tree change is one
per-file `COMPILE_OPTIONS` line in `CMakeLists.txt`.

## 0. Bottom line

1. Round 6's 5,712-cell `-mllvm` sweep is extended here with the families it
   did not isolate: the driver `-fno-*` family (including
   `-fno-threadsafe-statics`, `-fno-exceptions`, `-fno-rtti`,
   `-fno-strict-aliasing`, `-fwrapv`, `-fno-delete-null-pointer-checks`),
   the full 138-option `-mllvm -enable-*` / `-disable-*` hidden-option set
   (bare and inverted `=false` forms), and 63 ARM target-feature flags.
2. **One new size-exact, `byte_exact`-neutral raw win landed:**
   `Level.cpp -mllvm -enable-earlycse-memssa=false`.
   `Level::createMission` raw **5,436 -> 5,435** (-1), aligned **767**
   (unchanged), size 17,172 B; `Level::createShip` untouched; whole `.text`
   **82,898 -> 82,897**. Every alloc section (`.ARM.exidx`, `.ARM.extab`
   = 660, `.rodata`, `.data`, `.bss`, `.rel.plt` = 0) is unchanged.
3. **No flag makes any residual function byte-exact.** The residual is still
   source/TU shape, not a compiler-flag difference, consistent with rounds
   5-6.
4. **Measurement caveat (new in this round):** a per-TU link-measurement
   harness that copies a candidate object over the build object must also
   satisfy ninja's *deps-log mtime equality* check (ninja records the
   output mtime when deps were recorded and rebuilds if it differs, not only
   if it is older). Restoring the object's old mtime makes ninja silently
   recompile the canonical object, so a "no change" measurement can be an
   artifact. The harness used here copies the canonical object from a
   snapshot, sets its mtime to the deps-log mtime, and deletes the `.so`
   before `ninja link_so` so the link always runs without a recompile.

## 1. Baseline (this worktree)

| function | mangled | raw | aligned | size |
|---|---|---:|---:|---:|
| `Layout::Layout` C1/C2 | `_ZN6LayoutC1Ev` / `C2` | 5,658 | 2,280 | 7,072 |
| `LevelScript::process` | `_ZN11LevelScript7processEi` | 54,095 | 17,621 | 57,232 |
| `Level::createShip` | `_ZN5Level10createShipEiiiP8Waypointbb` | 1,560 | 455 | 2,764 |
| `Level::createMission` | `_ZN5Level13createMissionEv` | 5,436 | 767 | 17,172 |
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

Three stages, as in round 6:

1. **Object-level filter.** Compile each of the 13 residual TUs with the
   canonical `ninja -t commands` line plus the candidate flag. A cell
   survives only if every `.text.*` section keeps its exact size, every
   non-target function keeps its exact bytes, and at least one target
   function changes. (Round 6 additionally required the TU `.text` size to
   be unchanged, which is implied by the per-section sizes.)
2. **Link measurement.** Each survivor is linked with the real gold link and
   measured with `tools/fn_diff.py` plus a direct whole-`.text` byte diff
   against the original. The harness snapshots the canonical objects, swaps
   in the flagged object, sets its mtime to ninja's recorded deps mtime,
   deletes the `.so`, relinks, measures, then restores the canonical object.
3. **Official gate.** The landed cell was rebuilt in `cmake-build-match` and
   `check` was run.

## 3. Flag space swept

* **Driver / language flags (54 cells x 13 TUs = 702):**
  `-fno-threadsafe-statics`, `-fno-exceptions`, `-fno-rtti`,
  `-fno-strict-aliasing`, `-fwrapv`, `-fno-strict-overflow`,
  `-fno-delete-null-pointer-checks`, the `-fno-exceptions -fno-rtti` and
  `-fno-threadsafe-statics` combinations, `-fomit-frame-pointer`,
  `-fno-inline[-functions[-called-once]]`, `-fno-builtin`,
  `-fno-jump-tables`, `-fno-common`, `-fvisibility=hidden`,
  `-fvisibility-inlines-hidden`, `-fno-semantic-interposition`,
  `-fno-math-errno`, `-ffp-contract=off`, `-fno-honor-nans`,
  `-fno-signed-zeros`, `-freciprocal-math`, `-funsafe-math-optimizations`,
  `-fassociative-math`, `-ffinite-math-only`, `-fno-unwind-tables`,
  `-fno-asynchronous-unwind-tables`, `-fstack-protector`,
  `-fno-stack-protector`, `-fshort-enums`, `-fno-sized-deallocation`,
  `-faligned-allocation`, `-fno-use-cxa-atexit`, `-fno-elide-constructors`,
  `-fno-lax-vector-conversions`, `-fno-vectorize`, `-fno-slp-vectorize`,
  `-fno-unroll-loops`, `-fmerge-all-constants`, `-fno-merge-constants`,
  `-fno-zero-initialized-in-bss`, and `-O0/-O1/-O2/-O3/-Os/-Oz`.
* **`-mllvm` enable/disable pairs (138 x 13 = 1,794, twice):** the complete
  clang 7.0.2 `-help-list-hidden` set of `-enable-*` / `-disable-*` options,
  first bare, then inverted (`-disable-X=false` / `-enable-X=false`).
* **ARM target features (63 x 13 = 819):** `-mcpu=` (cortex-a5/a7/a8/a9/a15,
  generic), `-mtune=`, `-mfpu=` (vfpv3-d16/vfpv3/vfpv4/neon-vfpv4/
  neon-fp16), `-mfloat-abi=soft/hard`, `-marm`, `-mno-unaligned-access`,
  `-mno-movt`, `-mno-restrict-it`, `-mimplicit-it=*`,
  `-mno-thumb-interwork`, `-mno-global-merge`, `-mtp=soft/cp15`,
  `-mno-neon`, `-mno-vfpv3`, `-mno-vmlx`, `-mno-dsp`, `-mno-hwdiv`,
  `-mno-ldrexd`, `-mslow-flash-data`, `-mlong-calls`,
  `-mvectorize-with-neon-quad`, `-mno-crc`, `-mno-aes`, `-mno-sha2`,
  `-mno-fp16`, `-mfp16-format=*`, `-mstrict-align`, and combinations.

Total 702 + 1,794 + 1,794 + 819 = **5,109 object-level cells** in this round,
plus 77 relaxed cells link-measured (size-exact but a non-target function
moves).

## 4. The landed cell

### `Level.cpp -mllvm -enable-earlycse-memssa=false`

EarlyCSE in clang 7 has an optional MemorySSA mode. Turning it off changes
exactly two bytes in `Level::createMission` and leaves `Level::createShip`,
every other function in the TU, every `.text.*` size and every alloc section
untouched. Linked, one of those two bytes moves into agreement and the other
was already inside a differing run, for a net -1 raw byte. The aligned
instruction stream is unchanged (767), and the function size is 17,172 B.

| | raw | aligned | size | `.text` diff |
|---|---:|---:|---:|---:|
| before | 5,436 | 767 | 17,172 | 82,898 |
| after | **5,435** | 767 | 17,172 | **82,897** |

## 5. All link-measured strict candidates

Baseline `.text` 82,898. `textdiff` is the whole-`.text` differing-byte count
for that cell alone.

| TU | flag | target raw | textdiff | verdict |
|---|---|---|---:|---:|---|
| **Level** | **`-mllvm -enable-earlycse-memssa=false`** | **5,436 -> 5,435** | **82,897** | **landed (-1)** |
| LevelScript | `-mllvm -disable-ifcvt-triangle-rev` | 54,095 -> 54,095 | 82,898 | neutral (6 bytes swap) |
| LevelScript | `-mllvm -enable-earlycse-memssa=false` | 54,095 -> 54,095 | 82,898 | neutral |
| Radar | `-fwrapv` | 7,487 -> 7,487 | 82,898 | neutral (1 byte swap) |
| Radar | `-fno-strict-overflow` | 7,487 -> 7,487 | 82,898 | neutral (same byte) |
| Generator | `-mllvm -disable-lftr` | 127 -> 140 | 82,911 | worse |
| Generator | `-mllvm -disable-postra-machine-sink` | 127 -> 139 | 82,910 | worse |
| LevelScript | `-mllvm -disable-constant-hoisting` | 54,095 -> 54,104 | 82,907 | worse |
| LevelScript | `-mllvm -enable-tail-merge` | 54,095 -> 54,180 | 82,983 | worse |
| HangarWindow | `-mllvm -disable-postra-machine-licm` | 2,245 -> 2,263 | 82,916 | worse |
| Gun | `-mllvm -disable-ifcvt-triangle-rev` | 463 -> 467 | 82,902 | worse |
| SpaceLounge | `-mllvm -disable-constant-hoisting` | 175 -> 218 | 82,941 | worse |
| MGame | `-mllvm -disable-postra-machine-sink` | 4,082 -> 4,096 | 82,923 | worse |
| Radar | `-mllvm -enable-load-pre=false` | 7,487 -> 8,499 | 83,910 | worse |

## 6. Driver-family observations

* `-fno-threadsafe-statics` and `-fno-delete-null-pointer-checks` are exact
  no-ops on all 13 TUs (no function-local statics with guards; no null
  checks to delete).
* `-fno-rtti` is an exact no-op on all 13 TUs (no `dynamic_cast`/`typeid`).
* `-fno-exceptions` is size-changing on every TU that has EH cleanup
  (`__clang_call_terminate` disappears, `.ARM.extab` collapses to 12 bytes
  per function); it is not usable.
* `-fno-strict-aliasing` is size-changing on 11 of 13 TUs.
* `-fwrapv` / `-fno-strict-overflow` are strict movers only on Radar
  (one byte swaps, net raw unchanged); they are size-changing elsewhere.
* `-mcpu=cortex-a8` is a strict mover only on Layout (raw unchanged, whole
  `.text` worse); the remaining ARM target-feature flags are either exact
  no-ops or size-changing.

## 6b. Relaxed candidates (size-exact, non-target function also changes)

77 further cells changed the target but also a non-target function in the
same TU while keeping every `.text.*` size unchanged. All were link-measured
against the post-landing baseline `.text` 82,897; **none improved it.** The
closest (whole-`.text` diff):

| TU | flag | target raw | textdiff |
|---|---|---|---:|
| Generator | `-fno-honor-nans` / `-ffinite-math-only` | 127 | 82,898 |
| Radar | `-fno-honor-nans` / `-ffinite-math-only` | 7,487 | 82,898 |
| Hud | `-mllvm -disable-lftr` | 762 | 82,898 |
| Generator / Status / Layout / ModStation / HangarWindow | `-mllvm -disable-ifcvt-triangle-rev` / `-disable-early-taildup` / `-disable-lftr` / `-disable-ifcvt-simple` | unchanged | 82,899 |
| LevelScript | `-mllvm -disable-lftr` | 54,095 -> 54,096 | 82,900 |
| Level | `-mllvm -disable-ifcvt-triangle-rev` | 5,435 | 82,909 |

Everything else was worse by a larger margin (up to +1,000 when a link-order
or scheduling change moved a whole block).

## 7. Interpretation

No positive evidence that the original used a different per-file flag for
any residual TU. The one landed cell is a matching knob: it removes a single
same-address byte from a 17 KB body while leaving every gate count, every
function size, `.ARM.extab` (660) and `.rel.plt` (0) untouched. The residual
remains source/TU shape.

## 8. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11218
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so verify check -j2
```

Scratch artifacts: `/dev/shm/w11218/{common.py,elf.py,sweep.py,
link_batch2.py,flags_*.json,res_*.json}`.

## 9. Files changed

* `_work/reconstructions/build-provenance-round7-per-tu-flags.md` — this note.
* `CMakeLists.txt` — per-file
  `-mllvm -enable-earlycse-memssa=false` on `Level.cpp`.
