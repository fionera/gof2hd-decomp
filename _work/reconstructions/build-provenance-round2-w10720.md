# Build provenance round 2 (w10720): the remaining code-gen levers are all
# neutral on the residual bodies; the original used `-g` (limited) and a
# single deterministic gold link

Status: **research note only**, 2026-11-08. Worktree `wt/w10720`, base master
`01c7f92d6` (wave 160). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000). Scratch `/dev/shm/w10720`.
No `src/`, `tools/`, `CMakeLists.txt`, baseline, allowlist or doc change is
landed; this note is the only commit.

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370. `.text` **85,518** differing bytes (size
1,259,464 B), `.ARM.extab` **1,258** (size 156,312 B), `.rel.plt` 0,
`.note.gnu.build-id` 20. All 27 section sizes exact; file size 2,187,328 B.

## 0. Bottom line

1. **No lever in the round-2 list moves any residual body.** Every
   size-preserving variant below leaves the 15 residual function bodies
   bit-for-bit identical at the object level (whole `.text.*` code sections),
   so the linked raw/aligned metrics cannot move either.
2. **(a) `-g` is `-g2` is `-g3` in clang 7.0.2** (`-debug-info-kind=limited`);
   `-g1`/`-gline-tables-only`/`-gmlt` are `line-tables-only`. All are
   codegen-neutral on the residual TUs, but `line-tables-only` is
   codegen-equivalent to `-g0` *globally* (byte_exact 2071, `.text` diff
   804,136), so the original definitely used `limited` (`-g`). It does not
   explain a residual.
3. **(b) Flag order is irrelevant.** `-g -Oz`, `-Oz -g`, and
   `-fstack-protector-strong` first all produce identical residual bodies.
4. **(c) A response file / `-fdebug-compilation-dir` is irrelevant.** `@args`
   reproduces the object exactly; `-fdebug-compilation-dir` is debug-only.
5. **(d) `-fno-standalone-debug`, `-fstandalone-debug`,
   `-fdebug-info-for-profiling` are all neutral.** `-femit-struct-debug-baseonly`
   does not exist in clang 7.0.2 (`unknown argument`).
6. **(e) The build-id style is SHA-1 (20-byte descriptor), gold's default.**
   Verified: `sha1(unstripped output with the 20 descriptor bytes zeroed)`
   equals the build-id exactly; `--build-id=sha1` is byte-identical to the
   default; `--build-id=md5` emits a 16-byte note (size-fatal). The 20
   differing bytes are a hash of the whole file, i.e. downstream of `.text`.
7. **(f) The link is single-pass and deterministic.** A repeated gold link is
   byte-identical; `--incremental` is fatal with `-z now`; `--icf=all` shrinks
   the unstripped output by 41,504 B (so ICF is off). All 4,524 function
   addresses already match the original, so the link layout is already exact —
   no two-pass/layout lever can exist without shifting addresses.
8. **New size-preserving mover found but it is worse everywhere:**
   `-mllvm -disable-postra-machine-sink=true` changes 9 of the 15 residual
   bodies at the object level yet is worse on every linked body that moves.
   This confirms PostRA machine sinking is ON in the original, consistent with
   the `-g` DBG_VALUE-pinning note in `cmake/toolchain.cmake`.

## 1. Baseline (canonical build, this worktree)

`raw` = same-address differing bytes (`/dev/shm/w10720/fdiff.py`); `aligned` =
instruction-index alignment (`align_diff.py`); `size` = function bytes.

| function | mangled | raw | aligned | size |
|---|---|---:|---:|---:|
| `Layout::Layout` | `_ZN6LayoutC1Ev` | 5,733 | 1,978 | 7,072 |
| `LevelScript::process` | `_ZN11LevelScript7processEi` | 55,048 | 17,049 | 57,232 |
| `Level::createShip` | `_ZN5Level10createShipEiiiP8Waypointbb` | 1,563 | 449 | 2,764 |
| `Status::missionCompleted` | `_ZN6Status16missionCompletedEbbx` | 722 | 320 | 1,432 |
| `Radar::draw` | `_ZN5Radar4drawEP6PlayerP3Hudi` | 8,124 | 1,320 | 9,908 |
| `MGame::OnTouchEnd` | `_ZN5MGame10OnTouchEndEiiPv` | 4,387 | 901 | 8,080 |
| `Level::createMission` | `_ZN5Level13createMissionEv` | 5,461 | 781 | 17,172 |
| `Gun::calcCharacterCollision` | `_ZN3Gun22calcCharacterCollisionEv` | 463 | 57 | 1,984 |
| `HangarWindow::render` | `_ZN12HangarWindow6renderEv` | 1,965 | 291 | 9,700 |
| `SpaceLounge::drawLounge` | `_ZN11SpaceLounge10drawLoungeEv` | 175 | 41 | 1,948 |
| `Hud::draw` | `_ZN3Hud4drawExxP9PlayerEgobjj` | 1,325 | 249 | 9,332 |
| `Generator::createMission` | `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 162 | 64 | 1,996 |
| `MenuTouchWindow::OnTouchBegin` | `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 29 | 15 | 1,896 |
| `ModStation::ModStation` | `_ZN10ModStationC1Ev` | 43 | 12 | 1,008 |
| `MGame::OnUpdate` | `_ZN5MGame8OnUpdateEv` | 318 | 19 | 11,440 |
| **total (15 aligned)** | | **85,518** | **23,546** | |

The 17 non-byte-exact symbols are the 15 bodies above plus the `Layout`/`ModStation`
C2 aliases (identical bytes to C1).

## 2. Method

`/dev/shm/w10720/objtest.py` recompiles one TU from the exact
`compile_commands.json` command with extra flags and compares the residual
function's own `.text.<mangled>` section bytes (the object has
`-ffunction-sections`, so `.text` itself is empty — the section is mapped by
symbol section name, not the `.text` offset). A variant that leaves the object
code sections identical cannot move the linked metric, so only non-neutral
cells are linked. `/dev/shm/w10720/linktest2.py` compiles one TU in place and
runs the real gold + GNU-strip + `reorder_shstrtab.py` chain directly (ninja
would recompile the object), then measures and restores.

## 3. (a) Debug-info kind / `-g` spelling

`clang -###` mapping in clang 7.0.2:

| driver flag | cc1 flag |
|---|---|
| `-g`, `-g2`, `-g3` | `-debug-info-kind=limited` |
| `-g1`, `-gline-tables-only`, `-gmlt` | `-debug-info-kind=line-tables-only` |
| `-g0` | (no debug info) |

Object-level result on all 15 residual bodies: `-g0`, `-g1`, `-g2`, `-g3`,
`-gline-tables-only`, `-gmlt`, `-gdwarf-2`, `-gdwarf-3`, `-gdwarf-4` are **all
exactly neutral** (0 differing code-section bytes). Stack-slot numbering is
unaffected.

Whole-build control (`/dev/shm/w10720/build-glt`, full 192-TU build + link):

| build | byte_exact | linked_exact | `.text` diff | `.ARM.extab` diff |
|---|---:|---:|---:|---:|
| canonical `-g` | **4507** | **4507** | **85,518** | **1,258** |
| `-gline-tables-only` | 2071 | 4253 | 804,136 | 1,571 |

So `line-tables-only` behaves like `-g0` (wave-146 measured `-g0` at 2071 /
804,437); the original used `limited`. That is a *global* codegen fact (DBG_VALUE
users pin PostRA sinking) but it is invisible on every residual body.

## 4. (b) Flag order

`-g` moved before `-Oz`, `-Oz` moved after `-g`, and
`-fstack-protector-strong` moved to the front of the command: all three give
**0** differing code-section bytes for `LevelScript::process` and `Radar::draw`
(and the driver is order-independent for these flags).

## 5. (c) Response file / compilation dir

* Invoking clang with every argument in an `@response` file reproduces
  `LevelScript::process` exactly (size 57,232, diff 0).
* `-Xclang -fdebug-compilation-dir -Xclang /foo`: neutral on all 15 bodies.
* `-I` path order cannot affect codegen; the 4,507 byte-exact functions already
  prove the include resolution is right.

## 6. (d) Debug-emission flags

Neutral on all 15 bodies: `-fno-standalone-debug`, `-fstandalone-debug`,
`-fdebug-info-for-profiling`, `-fno-eliminate-unused-debug-types`.
`-femit-struct-debug-baseonly` is not a clang 7.0.2 driver or cc1 argument
(`error: unknown argument`), so the original cannot have used it.

## 7. (e) Build-id style

* The note is `GNU / NT_GNU_BUILD_ID`, descriptor length **20** (note size 36).
  MD5 would be 16 (note size 32), so the style is SHA-1.
* Reproduced gold's algorithm on our own link: zero the 20 descriptor bytes at
  file offset `0x1dc` of the **unstripped** gold output and SHA-1 the whole
  file — it equals the emitted build-id exactly
  (`2d4f8a6e…f3f961`).
* `--build-id=sha1` is byte-identical to the default `--build-id`;
  `--build-id=md5` emits a 16-byte note (gate-fatal size change).
* The 20 differing bytes are therefore a hash over the whole file: they are
  downstream of `.text` (and of the build directory path only through
  `.text`), exactly as wave 160 stated.

## 8. (f) Single-pass vs two-pass link

* A repeated gold link with the identical inputs is **byte-identical**.
* `--incremental` is rejected by gold with `-z now` (`incremental linking is
  not compatible with -z relro`), so the original's fully-RELRO link cannot
  have been incremental.
* `--icf=all` shrinks the unstripped output by 41,504 B, i.e. ICF is off in
  the original (our `.text` size already matches exactly).
* Decisive: **all 4,524 function addresses already match the original** (the
  17 residuals are same-address, same-size). Any two-pass/layout difference
  would move addresses; there is none to find.

## 9. New size-preserving mover: `-mllvm -disable-postra-machine-sink=true`

Object-level (15 residual bodies): changes 9 of 15, size-exact on all 15.
Linked (one TU at a time, real gold chain):

| function | baseline raw/aligned | with flag | verdict |
|---|---|---|---|
| `LevelScript::process` | 55,048 / 17,049 | 55,048 / 17,049 | neutral |
| `Level::createShip` | 1,563 / 449 | 1,563 / 449 | neutral |
| `ModStation::ModStation` | 43 / 12 | **179 / 34** | worse |
| `MGame::OnTouchEnd` | 4,387 / 901 | **4,401 / 907** | worse |
| `MGame::OnUpdate` | 318 / 19 | **329 / 24** | worse |
| `HangarWindow::render` | 1,965 / 291 | **2,039 / 322** | worse |
| `Level::createMission` | 5,461 / 781 | **5,471 / 782** | worse |
| `SpaceLounge::drawLounge` | 175 / 41 | **309 / 63** | worse |
| `Hud::draw` | 1,325 / 249 | **1,723 / 308** | worse |
| `Generator::createMission` | 162 / 64 | **174 / 68** | worse |

Every linked body that moves gets worse on both metrics, so PostRA machine
sinking is ON in the original. This is the same mechanism the `-g` comment in
`cmake/toolchain.cmake` describes (DBG_VALUE users pin the sink), and it is
consistent with `-g` being required.

## 10. Other size-preserving, exactly-neutral cells (new in this pass)

`-mtune=cortex-a7/a8/a9/a15`, `-march=armv7ve`, `-mfpu=neon-vfpv4`,
`-mfpu=vfpv3-d16`, `-fstack-protector`, `-fstack-protector-all`,
`-fno-stack-protector` (no residual body has a canary), `-ffp-contract=fast`,
`-ffp-contract=off`, `-fno-sized-deallocation`, `-fno-use-cxa-atexit`,
`-std=gnu++11`, `-mllvm -enable-shrink-wrap=true/false`,
`-mllvm -arm-global-merge=true/false`, `-mllvm -enable-aa-sched-mi=false`,
`-mllvm -post-RA-scheduler=true`, `-mllvm -disable-early-ifcvt=true`,
`-mllvm -old-thumb2-ifcvt=true`, `-mllvm -arm-atomic-cfg-tidy=false`,
`-mllvm -max-uses-for-sinking=1..16`, `-mllvm -machine-sink-bfi=true/false`,
`-mllvm -machine-sink-split=true`.

Size-changing (gate-fatal, recorded for completeness): `-mcpu=cortex-a8`
(+32), `-mcpu=cortex-a9` (+116), `-mllvm -post-RA-scheduler=false` (-8),
`-mllvm -disable-machine-sink=true` (+16),
`-mllvm -machine-sink-split-probability-threshold=100` (+4).

## 11. Conclusion

The build provenance is now closed on every axis the round-2 mission listed:
same clang build string, same gold, same SHA-1 build-id algorithm, same
`-debug-info-kind=limited`, order-independent driver, single deterministic
gold link with ICF off. None of these levers moves a residual body. The
85,518 `.text` bytes and 1,258 `.ARM.extab` bytes remain **source shape /
block order**, not build configuration.

## 12. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10720
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6

# object-level flag filter (fast)
python3 /dev/shm/w10720/objtest.py <TU.cpp> <mangled> <extra flags...>
python3 /dev/shm/w10720/batch.py <extra flags...>

# linked verification of one TU (real gold chain, no ninja recompile)
EXTRA="<extra flags>" python3 /dev/shm/w10720/linktest2.py <TU.cpp> <mangled...>

# whole-build -gline-tables-only control
cmake -S . -B /dev/shm/w10720/build-glt -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/dev/shm/w10720/toolchain_glt.cmake \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build /dev/shm/w10720/build-glt --target link_so -j6
```

## 13. Files changed

* `_work/reconstructions/build-provenance-round2-w10720.md` (this note).
* No source, tool, CMake, baseline, allowlist or doc change.
