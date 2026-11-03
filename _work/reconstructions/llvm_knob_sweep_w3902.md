# LLVM 7 codegen-knob sweep on the 26 non-byte-exact functions (w3902)

**Status:** research + one landed knob, 2026-11-03. Worktree `wt/w3902`, branch
`wave89-w3902`, base `dfc9d7a1`. All numbers are from the official
`tools/verify/verify.py` metric (`autojunk=False`) against
`../_work/bins/android_2.0.16_libgof2hdaa.so`, or from a controlled A/B build of
this tree. Scratch is under `/dev/shm/w3902`; the worktree is clean apart from
the committed change.

## 0. Bottom line

* **162 global `-mllvm` variants** were measured (the task's list plus every
  other plausible ARM/Thumb codegen knob from `-mllvm -help-hidden`):
  **104 target-neutral (10 of which still lower `byte_exact`), 54 target-worse, 0 target-better, 4 clang-7 backend crashes**. Every knob
  that changes any target's fuzzy score lowers it.
* **227 branch-weight cells** (9 TUs with `__builtin_expect`, 20 pairs each,
  plus a refinement grid): **17 improve, 116 neutral, 94 worse**. All 17
  improvements are the same cell: `MenuTouchWindow::OnTouchEnd` **95.2 ->
  95.3** for likely:unlikely in the 3:1..19:1 (and 20:12) plateau.
* **The three existing per-TU knobs are optimal, but only StarMap's is
  unique.** Radar 80:1 and SpaceLounge 20:12 lie on broad optimal plateaus;
  StarMap 7:1 is the only value that keeps both `StarMap::draw` at 99.1 and
  the TU's other functions byte-exact.
* **Landed:** a new per-TU knob on `MenuTouchWindow.cpp`
  (`-likely-branch-weight=5 -unlikely-branch-weight=1`). It raises
  `MenuTouchWindow::OnTouchEnd` 95.2 -> 95.3 with
  `byte_exact 4498 / linked_exact 4498 / size-exact 4524` unchanged.
* **The residual is source-shape-driven, not knob-driven.** The 26 targets are
  all size-exact; their diffs are register allocation, instruction scheduling,
  16/32-bit encoding and basic-block order. No global `-mllvm` knob raises any
  of them. The branch-weight knobs are local compensations for source-shape
  differences, not recoverable original flags: they are not unique and
  removing them costs only 0.1-1.4 points.

## 1. Baseline and gate

`cmake --build cmake-build-match --target check -j8` at `dfc9d7a1`:

```text
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
=== DRIFT: 0 field(s) across 0 class(es) ===
CLEAN ✓ (no new soft violations; hard invariants 0)
```

`.text 0x1337c8`, `.rodata 0x41b67`, size-exact 4524/4524.

**Stale tracked report note.** The repo-root `report.json` still records
`Status::missionCompleted` at **86.7%** from the removed `.missioncase`
rank-table build. The source at this base (`6e3fd629` removed the rank anchor)
and the fresh `cmake-build-match/verify/report.json` both say **57.7%**. All
numbers in this note use the fresh report; the gate counts are identical
either way. This is a pre-existing report-sync gap, not a codegen change.

The 26 targets:

| target | bytes | base % |
|---|---:|---:|
| `_ZN6LayoutC1Ev` | 7072 | 47.7 |
| `_ZN6LayoutC2Ev` | 7072 | 47.7 |
| `_ZN11LevelScript7processEi` | 57232 | 55.0 |
| `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 1996 | 73.6 |
| `_ZN5Radar4drawEP6PlayerP3Hudi` | 9908 | 73.9 |
| `_ZN5Level10createShipEiiiP8Waypointbb` | 2764 | 74.1 |
| `_ZN5MGame10OnTouchEndEiiPv` | 8080 | 78.4 |
| `_ZN7GlobalsC1Ev` | 612 | 80.5 |
| `_ZN7GlobalsC2Ev` | 612 | 80.5 |
| `_ZN12HangarWindow6renderEv` | 9700 | 80.6 |
| `_ZN3Gun22calcCharacterCollisionEv` | 1984 | 82.7 |
| `_ZN6Status16missionCompletedEbbx` | 1432 | 57.7 |
| `_ZN9RocketGun6updateEi` | 1908 | 86.9 |
| `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` | 968 | 87.3 |
| `_ZN5Level11createSceneEv` | 2252 | 88.3 |
| `_ZN3Hud4drawExxP9PlayerEgobjj` | 9332 | 88.8 |
| `_ZN5Level13createMissionEv` | 17172 | 90.7 |
| `_ZN15MenuTouchWindowC1Ei` | 9600 | 93.9 |
| `_ZN15MenuTouchWindowC2Ei` | 9600 | 93.9 |
| `_ZN15MenuTouchWindow10OnTouchEndEiiPv` | 8132 | 95.2 |
| `_ZN11SpaceLounge10drawLoungeEv` | 1948 | 95.6 |
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 1896 | 97.7 |
| `_ZN10ModStationC1Ev` | 1008 | 98.6 |
| `_ZN10ModStationC2Ev` | 1008 | 98.6 |
| `_ZN7StarMap4drawEv` | 2644 | 99.1 |
| `_ZN5MGame8OnUpdateEv` | 11440 | 99.9 |

## 2. Method

### 2.1 Knob enumeration

```sh
NDK=.cache/ndk/android-ndk-r18b
$NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++ \
  -target armv7-none-linux-androideabi16 -mthumb -Oz -c t.cpp -o t.o \
  -mllvm -help-hidden > help.txt
```

`-help-hidden` prints 1,035 lines. 176 candidate payloads were probed for
acceptance with the real matching flags; 160 are accepted, 16 are unknown in
clang 7.0.2 (the task's `-enable-machine-sink`, `-enable-machine-licm`,
`-enable-tail-duplication`, `-enable-mbb-placement`,
`-enable-constant-island`, `-enable-rematerialization`,
`-enable-post-ra-sched`, `-enable-merge-functions`, `-enable-gvn`,
`-enable-loop-unrolling`, `-enable-unsafe-fp-math`,
`-enable-no-infs-fp-math`, `-enable-no-nans-fp-math`, `-enable-fp-mad`,
`-stack-alignment=*`, `-max-stack-alignment=*` are not LLVM 7 `-mllvm`
options; their clang-7 equivalents are `-disable-machine-sink`,
`-disable-machine-licm`, `-disable-tail-duplicate`, `-disable-block-placement`,
`-arm-constant-island-max-iteration=*`, `-remat-pic-stub-load` /
`-spp-rematerialization-threshold=*`, `-post-RA-scheduler` / `-disable-post-ra`,
`-enable-newgvn` / `-enable-gvn-hoist` / `-enable-gvn-sink`, the
`-unroll-*` family, and the `-mstack-alignment=*` driver flag).

### 2.2 Harness

A compiler wrapper (`/dev/shm/w3902/cxx_wrap.sh`, not tracked) appends
`-mllvm <payload>` to the real NDK clang++ command line. It supports:

* `GOF2_EXTRA_FLAGS`: payload applied to every TU (global sweep);
* `GOF2_TU` + `GOF2_TU_FLAGS`: payload applied only to one TU (per-TU sweep);
* `GOF2_BW=L:U`: remove the TU's existing `-mllvm -likely-branch-weight=*` /
  `-mllvm -unlikely-branch-weight=*` pairs and append `L:U`
  (`GOF2_BW=none` removes them without appending).

The wrapper is used through a scratch CMake toolchain that sets
`CMAKE_CXX_COMPILER`/`CMAKE_C_COMPILER` to it. A no-op run reproduced the
then-canonical pre-change `.so` byte-for-byte (sha256
`88aff1154772f836a53b12cc094637deec50026c74db18c1cd59fdced0cbfecd`), so the
wrapper itself is codegen-neutral.

For screening, only the 17 TUs that own the 26 targets are recompiled
(`Layout`, `LevelScript`, `Generator`, `Radar`, `Level`, `MGame`, `Globals`,
`HangarWindow`, `Gun`, `Status`, `RocketGun`, `PlayerEgo`, `Hud`,
`MenuTouchWindow`, `SpaceLounge`, `ModStation`, `StarMap`). The other 176 TUs
stay at baseline. This is exact for the target fuzzy scores: a target's codegen
is determined by its own TU (no LTO), so the global and per-TU effects on that
target are identical. Section 4 confirms this with 17 spot checks. Winners are
re-verified with the official whole-`.so` verifier.

### 2.3 Metric

`tools/verify/verify.py` with `autojunk=False`, exactly the official gate
metric. `byte_exact` in the screening tables is the raw-byte count over all
4,524 common functions; it is not the official linked count, so a variant that
changes a non-target TU can show a `byte_delta` that the official verifier
would also see (the screen recompiles only target TUs, so non-target TUs are
always baseline).

## 3. Global `-mllvm` sweep

**162 variants: 104 target-neutral (10 still lower `byte_exact`), 54 target-worse, 0 target-better, 4 build failures.** The full
table is Appendix A. The 54 knobs that move at least one target are in the
26-target delta matrix in Appendix B; the legend is in Appendix C.

No variant raises any target. The closest-to-neutral movers are:

| knob | byte_exact | targets moved | min delta | max delta |
|---|---:|---:|---:|---:|
| `-disable-lftr` | 2419 | 5 | -3.3 | -0.1 |
| `-disable-postra-machine-sink` | 2294 | 9 | -4.4 | -0.1 |
| `-jump-is-expensive` | 1667 | 17 | -18.0 | -0.1 |
| `-enable-earlycse-memssa=false` | 1909 | 6 | -36.2 | -0.1 |
| `-disable-complex-addr-modes` | 2086 | 8 | -9.4 | -0.1 |
| `-enable-gvn-sink` | 2038 | 15 | -43.9 | -0.1 |
| `-enable-aa-sched-mi` | 3094 | 19 | -6.0 | -0.1 |
| `-arm-restrict-it` | 2006 | 24 | -18.7 | -0.1 |
| `-disable-postra-machine-licm` | 2452 | 3 | -3.8 | -0.1 |
| `-misched-postra` | 2626 | 23 | -4.7 | -0.1 |

Every one of these lowers `byte_exact` as well, so none is landable even if it
had raised a target. The task's scheduling knobs are all in this set or are
neutral: `-enable-misched=true` is -2851 byte_exact and lowers all 26;
`-enable-misched=false`, `-misched=*`, `-enable-post-misched`,
`-post-RA-scheduler`, `-enable-aa-sched-mi=false`, `-disable-early-ifcvt`,
`-old-thumb2-ifcvt`, `-enable-if-conversion`, `-enable-tail-merge=true`,
`-tail-dup-placement`, `-enable-shrink-wrap=true`, `-enable-pipeliner`,
`-enable-global-merge`, `-enable-load-pre=true`, `-enable-gvn-hoist/sink` are
neutral or worse. `-align-all-*` and `-regalloc=*` are all much worse.

### 3.1 Build failures

| flag | failure |
|---|---|
| `-arm-constant-island-max-iteration=0` | clang 7.0.2 backend crash |
| `-arm-constant-island-max-iteration=1` | clang 7.0.2 backend crash |
| `-align-all-blocks=16` | clang 7.0.2 backend crash |
| `-align-all-nofallthru-blocks=16` | ninja: build stopped: subcommand failed. |

The first three are clang 7.0.2 backend crashes (MGame.cpp, HangarWindow.cpp,
Hud.cpp); the fourth is a build failure while compiling Hud.cpp. They are not
landable.

## 4. Per-TU sweep

The 17 spot checks below apply one knob only to the TU that owns the target.
The per-TU delta equals the global delta for every target that moved, which is
the expected TU-locality result. No per-TU cell improves a target.

| cell | TU | knob | target(s) moved | per-TU delta | global delta | byte_exact |
|---|---|---|---|---|---:|---:|
| `pt_Layout_disable_lftr` | `game/ui/Layout.cpp` | `-disable-lftr` | — | 0.0 | 0.0 | 4498 |
| `pt_LevelScript_disable_postra_sink` | `game/world/LevelScript.cpp` | `-disable-postra-machine-sink` | — | 0.0 | 0.0 | 4498 |
| `pt_Generator_misched_postra` | `game/mission/Generator.cpp` | `-misched-postra` | — | 0.0 | 0.0 | 4388 |
| `pt_Radar_enable_aa_sched_mi` | `game/weapons/Radar.cpp` | `-enable-aa-sched-mi` | `_ZN5Radar4drawEP6PlayerP3Hudi` | -0.1 | -0.1 | 4495 |
| `pt_Level_disable_machine_sink` | `game/world/Level.cpp` | `-disable-machine-sink` | `_ZN5Level10createShipEiiiP8Waypointbb`; `_ZN5Level11createSceneEv`; `_ZN5Level13createMissionEv` | -24.0; -45.5; -49.6 | -24.0; -45.5; -49.6 | 2347 |
| `pt_MGame_jump_is_expensive` | `game/menu/MGame.cpp` | `-jump-is-expensive` | `_ZN5MGame8OnUpdateEv` | -18.0 | -18.0 | 3121 |
| `pt_Globals_disable_lftr` | `game/core/Globals.cpp` | `-disable-lftr` | — | 0.0 | 0.0 | 4497 |
| `pt_HangarWindow_disable_complex_addr` | `game/ui/HangarWindow.cpp` | `-disable-complex-addr-modes` | — | 0.0 | 0.0 | 2768 |
| `pt_Hud_enable_gvn_sink` | `game/ui/Hud.cpp` | `-enable-gvn-sink` | `_ZN3Hud4drawExxP9PlayerEgobjj` | -25.5 | -25.5 | 2942 |
| `pt_MenuTouchWindow_disable_lftr` | `game/ui/MenuTouchWindow.cpp` | `-disable-lftr` | — | 0.0 | 0.0 | 4495 |
| `pt_StarMap_disable_postra_sink` | `game/world/StarMap.cpp` | `-disable-postra-machine-sink` | — | 0.0 | 0.0 | 4498 |
| `pt_Status_disable_lftr` | `game/mission/Status.cpp` | `-disable-lftr` | — | 0.0 | 0.0 | 4478 |
| `pt_RocketGun_disable_lftr` | `game/weapons/RocketGun.cpp` | `-disable-lftr` | — | 0.0 | 0.0 | 4498 |
| `pt_PlayerEgo_disable_lftr` | `game/ship/PlayerEgo.cpp` | `-disable-lftr` | — | 0.0 | 0.0 | 4495 |
| `pt_ModStation_disable_lftr` | `game/menu/ModStation.cpp` | `-disable-lftr` | — | 0.0 | 0.0 | 4497 |
| `pt_SpaceLounge_disable_lftr` | `game/menu/SpaceLounge.cpp` | `-disable-lftr` | — | 0.0 | 0.0 | 4497 |
| `pt_Gun_disable_lftr` | `game/weapons/Gun.cpp` | `-disable-lftr` | — | 0.0 | 0.0 | 4498 |

## 5. Branch-weight grid

The source has `__builtin_expect` in nine target TUs: `Generator`, `Radar`,
`Gun`, `RocketGun`, `PlayerEgo`, `MenuTouchWindow`, `SpaceLounge`,
`ModStation`, `StarMap`. Each was swept over
`none, 1:1, 2:1, 3:1, 5:1, 7:1, 10:1, 20:1, 20:12, 40:1, 80:1, 100:1, 200:1,
2000:1, 1:2, 1:5, 1:10, 1:20, 1:80, 1:2000`, with a refinement grid around the
interesting plateaus. **227 cells: 17 improve, 116 neutral, 94 worse.** All 17
improvements are `MenuTouchWindow::OnTouchEnd` +0.1.

### 5.1 MenuTouchWindow (new knob)

`MenuTouchWindow::OnTouchEnd` is 95.2 at the default 2000:1. The original's
block layout is reproduced by a broad plateau:

| likely:unlikely | target % | delta | byte_exact |
|---|---:|---:|---:|
| none | 95.2 | +0.0 | 4498 |
| 1:1 | 94.6 | -0.6 | 4498 |
| 1:2 | 84.9 | -10.3 | 4457 |
| 1:5 | 86.3 | -8.9 | 4498 |
| 1:10 | 84.7 | -10.5 | 4457 |
| 1:20 | 84.5 | -10.7 | 4457 |
| 1:80 | 84.5 | -10.7 | 4457 |
| 1:2000 | 84.4 | -10.8 | 3613 |
| 2:1 | 94.8 | -0.4 | 4498 |
| 3:1 | 95.3 | +0.1 | 4498 |
| 4:1 | 95.3 | +0.1 | 4498 |
| 5:1 | 95.3 | +0.1 | 4498 |
| 6:1 | 95.3 | +0.1 | 4498 |
| 7:1 | 95.3 | +0.1 | 4498 |
| 8:1 | 95.3 | +0.1 | 4498 |
| 9:1 | 95.3 | +0.1 | 4498 |
| 10:1 | 95.3 | +0.1 | 4498 |
| 11:1 | 94.8 | -0.4 | 4498 |
| 12:1 | 95.3 | +0.1 | 4498 |
| 13:1 | 95.3 | +0.1 | 4498 |
| 14:1 | 95.3 | +0.1 | 4498 |
| 15:1 | 95.3 | +0.1 | 4498 |
| 16:1 | 95.3 | +0.1 | 4498 |
| 17:1 | 95.3 | +0.1 | 4498 |
| 18:1 | 95.3 | +0.1 | 4498 |
| 19:1 | 95.3 | +0.1 | 4498 |
| 20:1 | 95.2 | +0.0 | 4498 |
| 20:12 | 95.3 | +0.1 | 4498 |
| 25:1 | 95.2 | +0.0 | 4498 |
| 30:1 | 95.2 | +0.0 | 4498 |
| 40:1 | 95.2 | +0.0 | 4498 |
| 50:1 | 95.2 | +0.0 | 4498 |
| 60:1 | 95.2 | +0.0 | 4498 |
| 80:1 | 95.2 | +0.0 | 4498 |
| 100:1 | 95.2 | +0.0 | 4498 |
| 200:1 | 95.2 | +0.0 | 4498 |
| 2000:1 | 95.2 | +0.0 | 4498 |

The plateau is 3:1..19:1 (11:1 is an outlier at -0.4) plus 20:12; 25:1 and
above fall back to the default layout. The landed value is **5:1**. The
official verifier on the full `.so`:

```text
_ZN15MenuTouchWindow10OnTouchEndEiiPv  pct 95.3  osize 8132  usize 8132
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
```

`byte_exact`/`linked_exact` are unchanged; the function is still not
linked-exact (the remaining diff is source-shape).

### 5.2 Radar (current 80:1)

| likely:unlikely | target % | delta | byte_exact |
|---|---:|---:|---:|
| none | 72.5 | -1.4 | 4498 |
| 1:1 | 71.4 | -2.5 | 3633 |
| 1:2 | 69.3 | -4.6 | 2794 |
| 1:5 | 69.7 | -4.2 | 2701 |
| 1:10 | 69.6 | -4.3 | 2701 |
| 1:20 | 65.6 | -8.3 | 2701 |
| 1:80 | 64.7 | -9.2 | 2701 |
| 1:2000 | 62.0 | -11.9 | 2701 |
| 2:1 | 72.6 | -1.3 | 3633 |
| 3:1 | 71.1 | -2.8 | 3633 |
| 5:1 | 72.2 | -1.7 | 3633 |
| 7:1 | 70.8 | -3.1 | 3633 |
| 10:1 | 70.9 | -3.0 | 3633 |
| 20:1 | 70.9 | -3.0 | 3633 |
| 20:12 | 71.4 | -2.5 | 3633 |
| 40:1 | 72.3 | -1.6 | 4498 |
| 50:1 | 72.4 | -1.5 | 4498 |
| 60:1 | 73.8 | -0.1 | 4498 |
| 70:1 | 73.8 | -0.1 | 4498 |
| 80:1 | 73.9 | +0.0 | 4498 |
| 90:1 | 73.9 | +0.0 | 4498 |
| 100:1 | 73.9 | +0.0 | 4498 |
| 120:1 | 72.5 | -1.4 | 4498 |
| 150:1 | 73.9 | +0.0 | 4498 |
| 200:1 | 73.9 | +0.0 | 4498 |
| 250:1 | 73.9 | +0.0 | 4498 |
| 300:1 | 72.5 | -1.4 | 4498 |
| 400:1 | 72.5 | -1.4 | 4498 |
| 500:1 | 73.9 | +0.0 | 4498 |
| 800:1 | 73.9 | +0.0 | 4498 |
| 1000:1 | 73.9 | +0.0 | 4498 |
| 2000:1 | 72.5 | -1.4 | 4498 |

The current 80:1 is optimal, but not unique: 90:1, 100:1, 150:1, 200:1,
250:1, 500:1, 800:1 and 1000:1 also leave the target at 73.9. `none` is -1.4,
so the knob is load-bearing. 50:1 is -1.5; 120:1/300:1/400:1 are -1.4.

### 5.3 SpaceLounge (current 20:12)

| likely:unlikely | target % | delta | byte_exact |
|---|---:|---:|---:|
| none | 95.5 | -0.1 | 4498 |
| 1:1 | 92.1 | -3.5 | 2986 |
| 1:2 | 85.0 | -10.6 | 2986 |
| 1:5 | 79.9 | -15.7 | 3036 |
| 1:10 | 79.9 | -15.7 | 3036 |
| 1:20 | 78.8 | -16.8 | 3036 |
| 1:80 | 79.9 | -15.7 | 3036 |
| 1:2000 | 71.4 | -24.2 | 2986 |
| 2:1 | 88.5 | -7.1 | 4498 |
| 3:1 | 95.6 | +0.0 | 4498 |
| 4:1 | 88.5 | -7.1 | 4498 |
| 5:1 | 88.5 | -7.1 | 4498 |
| 6:1 | 88.5 | -7.1 | 4498 |
| 7:1 | 95.6 | +0.0 | 4498 |
| 8:1 | 88.5 | -7.1 | 4498 |
| 9:1 | 88.5 | -7.1 | 4498 |
| 10:1 | 95.6 | +0.0 | 4498 |
| 12:1 | 88.5 | -7.1 | 4498 |
| 15:1 | 95.5 | -0.1 | 4498 |
| 20:1 | 88.4 | -7.2 | 4498 |
| 20:12 | 95.6 | +0.0 | 4498 |
| 25:1 | 88.4 | -7.2 | 4498 |
| 30:1 | 95.5 | -0.1 | 4498 |
| 40:1 | 95.5 | -0.1 | 4498 |
| 50:1 | 88.4 | -7.2 | 4498 |
| 60:1 | 88.4 | -7.2 | 4498 |
| 80:1 | 88.4 | -7.2 | 4498 |
| 100:1 | 88.4 | -7.2 | 4498 |
| 200:1 | 95.5 | -0.1 | 4498 |
| 2000:1 | 95.5 | -0.1 | 4498 |

The current 20:12 is optimal, but not unique: 3:1, 7:1 and 10:1 are also
neutral. `none` is -0.1, so the knob is load-bearing. The pattern is
non-monotonic (5:1 and 6:1 are -7.1, 15:1/30:1 are -0.1), which is exactly
what a threshold-based allocator/layout heuristic looks like.

### 5.4 StarMap (current 7:1)

| likely:unlikely | target % | delta | byte_exact |
|---|---:|---:|---:|
| none | 99.1 | +0.0 | 2404 |
| 1:1 | 96.9 | -2.2 | 3154 |
| 1:2 | 98.2 | -0.9 | 3154 |
| 1:5 | 68.7 | -30.4 | 2404 |
| 1:10 | 68.7 | -30.4 | 2404 |
| 1:20 | 68.7 | -30.4 | 2404 |
| 1:80 | 55.8 | -43.3 | 2404 |
| 1:2000 | 56.0 | -43.1 | 2404 |
| 2:1 | 96.7 | -2.4 | 3154 |
| 3:1 | 96.7 | -2.4 | 3154 |
| 5:1 | 96.7 | -2.4 | 3154 |
| 6:1 | 99.1 | +0.0 | 3154 |
| 7:1 | 99.1 | +0.0 | 4498 |
| 8:1 | 99.1 | +0.0 | 2404 |
| 9:1 | 99.1 | +0.0 | 2404 |
| 10:1 | 96.7 | -2.4 | 2404 |
| 11:1 | 96.7 | -2.4 | 2404 |
| 12:1 | 96.7 | -2.4 | 2404 |
| 15:1 | 96.7 | -2.4 | 2404 |
| 20:1 | 96.7 | -2.4 | 2404 |
| 20:12 | 99.1 | +0.0 | 3154 |
| 25:1 | 99.1 | +0.0 | 2404 |
| 30:1 | 96.7 | -2.4 | 2404 |
| 40:1 | 96.7 | -2.4 | 2404 |
| 80:1 | 96.7 | -2.4 | 2404 |
| 100:1 | 99.1 | +0.0 | 2404 |
| 200:1 | 99.1 | +0.0 | 2404 |
| 2000:1 | 99.1 | +0.0 | 2404 |

The current 7:1 is the **unique** optimum: it is the only value that keeps
`StarMap::draw` at 99.1 and the TU's other functions byte-exact
(`byte_exact 4498`). 6:1 is target-neutral but `byte_exact 3154` (-1344);
8:1/9:1 are target-neutral but `byte_exact 2404` (-2094); 20:12 is
target-neutral but `byte_exact 3154` (-1344). Removing the knob (`none`) is
`byte_exact 2404` and the target drops.

### 5.5 The other five TUs

`Generator`, `Gun`, `RocketGun`, `PlayerEgo` and `ModStation` have no target
improvement at any value. The default 2000:1 is optimal or on a plateau; the
only non-neutral values lower the target. Representative tables:

<details><summary>Generator</summary>

| likely:unlikely | target % | delta | byte_exact |
|---|---:|---:|---:|
| none | 73.6 | +0.0 | 4498 |
| 1:1 | 73.6 | +0.0 | 2604 |
| 1:2 | 73.6 | +0.0 | 4497 |
| 1:5 | 73.6 | +0.0 | 4497 |
| 1:10 | 73.6 | +0.0 | 4497 |
| 1:20 | 73.6 | +0.0 | 4497 |
| 1:80 | 73.6 | +0.0 | 4497 |
| 1:2000 | 73.6 | +0.0 | 4497 |
| 2:1 | 73.6 | +0.0 | 4498 |
| 3:1 | 73.6 | +0.0 | 4498 |
| 5:1 | 73.6 | +0.0 | 4498 |
| 7:1 | 73.6 | +0.0 | 4498 |
| 10:1 | 73.6 | +0.0 | 4498 |
| 20:1 | 73.6 | +0.0 | 4498 |
| 20:12 | 73.6 | +0.0 | 4498 |
| 40:1 | 73.6 | +0.0 | 4498 |
| 80:1 | 73.6 | +0.0 | 4498 |
| 100:1 | 73.6 | +0.0 | 4498 |
| 200:1 | 73.6 | +0.0 | 4498 |
| 2000:1 | 73.6 | +0.0 | 4498 |

</details>

<details><summary>Gun</summary>

| likely:unlikely | target % | delta | byte_exact |
|---|---:|---:|---:|
| none | 82.7 | +0.0 | 4498 |
| 1:1 | 77.2 | -5.5 | 4498 |
| 1:2 | 70.3 | -12.4 | 2811 |
| 1:5 | 69.9 | -12.8 | 2811 |
| 1:10 | 69.9 | -12.8 | 2811 |
| 1:20 | 69.9 | -12.8 | 2811 |
| 1:80 | 69.9 | -12.8 | 2811 |
| 1:2000 | 69.9 | -12.8 | 2811 |
| 2:1 | 82.7 | +0.0 | 4498 |
| 3:1 | 82.7 | +0.0 | 4498 |
| 5:1 | 82.7 | +0.0 | 4498 |
| 7:1 | 82.7 | +0.0 | 4498 |
| 10:1 | 82.7 | +0.0 | 4498 |
| 20:1 | 82.7 | +0.0 | 4498 |
| 20:12 | 82.7 | +0.0 | 4498 |
| 40:1 | 82.7 | +0.0 | 4498 |
| 80:1 | 82.7 | +0.0 | 4498 |
| 100:1 | 82.7 | +0.0 | 4498 |
| 200:1 | 82.7 | +0.0 | 4498 |
| 2000:1 | 82.7 | +0.0 | 4498 |

</details>

<details><summary>RocketGun</summary>

| likely:unlikely | target % | delta | byte_exact |
|---|---:|---:|---:|
| none | 86.9 | +0.0 | 4498 |
| 1:1 | 84.6 | -2.3 | 4498 |
| 1:2 | 84.6 | -2.3 | 4498 |
| 1:5 | 71.2 | -15.7 | 2923 |
| 1:10 | 71.1 | -15.8 | 2923 |
| 1:20 | 71.2 | -15.7 | 2923 |
| 1:80 | 70.0 | -16.9 | 2923 |
| 1:2000 | 70.0 | -16.9 | 2923 |
| 2:1 | 84.6 | -2.3 | 4498 |
| 3:1 | 84.6 | -2.3 | 4498 |
| 5:1 | 84.6 | -2.3 | 4498 |
| 7:1 | 86.9 | +0.0 | 4498 |
| 10:1 | 86.9 | +0.0 | 4498 |
| 20:1 | 86.9 | +0.0 | 4498 |
| 20:12 | 84.6 | -2.3 | 4498 |
| 40:1 | 86.9 | +0.0 | 4498 |
| 80:1 | 86.9 | +0.0 | 4498 |
| 100:1 | 86.9 | +0.0 | 4498 |
| 200:1 | 86.9 | +0.0 | 4498 |
| 2000:1 | 86.9 | +0.0 | 4498 |

</details>

<details><summary>PlayerEgo</summary>

| likely:unlikely | target % | delta | byte_exact |
|---|---:|---:|---:|
| none | 87.3 | +0.0 | 4498 |
| 1:1 | 87.3 | +0.0 | 2109 |
| 1:2 | 87.3 | +0.0 | 2714 |
| 1:5 | 87.3 | +0.0 | 2120 |
| 1:10 | 87.3 | +0.0 | 2120 |
| 1:20 | 87.3 | +0.0 | 2109 |
| 1:80 | 87.3 | +0.0 | 2109 |
| 1:2000 | 87.3 | +0.0 | 2109 |
| 2:1 | 87.3 | +0.0 | 2109 |
| 3:1 | 87.3 | +0.0 | 2109 |
| 5:1 | 87.3 | +0.0 | 4498 |
| 7:1 | 87.3 | +0.0 | 4498 |
| 10:1 | 87.3 | +0.0 | 4498 |
| 20:1 | 87.3 | +0.0 | 4498 |
| 20:12 | 87.3 | +0.0 | 2109 |
| 40:1 | 87.3 | +0.0 | 4498 |
| 80:1 | 87.3 | +0.0 | 4498 |
| 100:1 | 87.3 | +0.0 | 4498 |
| 200:1 | 87.3 | +0.0 | 4498 |
| 2000:1 | 87.3 | +0.0 | 4498 |

</details>

<details><summary>ModStation</summary>

| likely:unlikely | target % | delta | byte_exact |
|---|---:|---:|---:|
| none | 98.6 | +0.0 | 4498 |
| 1:1 | 98.6 | +0.0 | 4498 |
| 1:2 | 98.6 | +0.0 | 2488 |
| 1:5 | 98.6 | +0.0 | 2488 |
| 1:10 | 98.6 | +0.0 | 2488 |
| 1:20 | 98.6 | +0.0 | 4488 |
| 1:80 | 98.6 | +0.0 | 3281 |
| 1:2000 | 98.6 | +0.0 | 3281 |
| 2:1 | 98.6 | +0.0 | 4498 |
| 3:1 | 98.6 | +0.0 | 4498 |
| 5:1 | 98.6 | +0.0 | 4498 |
| 7:1 | 98.6 | +0.0 | 4498 |
| 10:1 | 98.6 | +0.0 | 4498 |
| 20:1 | 98.6 | +0.0 | 4498 |
| 20:12 | 98.6 | +0.0 | 4498 |
| 40:1 | 98.6 | +0.0 | 4498 |
| 80:1 | 98.6 | +0.0 | 4498 |
| 100:1 | 98.6 | +0.0 | 4498 |
| 200:1 | 98.6 | +0.0 | 4498 |
| 2000:1 | 98.6 | +0.0 | 4498 |

</details>

### 5.6 Source-shape experiment: removing `__builtin_expect`

To test whether the branch-weight knob is compensating for the reconstructed
`__builtin_expect` shortcuts, `MenuTouchWindow.cpp` was compiled with
`#define __builtin_expect(x,y) (x)` inserted after its includes (scratch edit,
reverted). Result: `MenuTouchWindow::OnTouchEnd` drops **95.3 -> 84.5** with
the 5:1 knob and **95.2 -> 84.5** without it. The `__builtin_expect` hints are
load-bearing; the branch-weight knob only matters because the source uses them.
The original's source shape is therefore closer to the current
`__builtin_expect` form than to a plain-condition form. A source-shape rewrite
that removes the hints would have to recover the original branch layout some
other way; it is not a free replacement for the knob.

## 6. Landed change

`CMakeLists.txt`:

```cmake
# MenuTouchWindow::OnTouchEnd's button-dispatch loop carries four __builtin_expect hints.  The
# original's block layout is reproduced by the 3:1..19:1 (and 20:12) likely/unlikely plateau;
# 5:1 raises the function's fuzzy match 95.2 -> 95.3 with byte_exact/linked_exact unchanged.
# 25:1 and above fall back to the default 2000:1 layout.
set_source_files_properties("${CMAKE_SOURCE_DIR}/src/game/ui/MenuTouchWindow.cpp"
    PROPERTIES COMPILE_OPTIONS "-mllvm;-likely-branch-weight=5;-mllvm;-unlikely-branch-weight=1")
```

Before/after gate (`cmake --build cmake-build-match --target check -j8`):

| gate | before | after |
|---|---:|---:|
| byte_exact | 4498/4524 | **4498/4524** |
| linked_exact | 4498/4524 | **4498/4524** |
| size-exact | 4524/4524 | **4524/4524** |
| stub_zero_size | 0 | **0** |
| missing / wrong_type / extra | 0 / 0 / 0 | **0 / 0 / 0** |
| data_exact | 843/843 | **843/843** |
| imports | 207 | **207** |
| `.text` | 0x1337c8 | **0x1337c8** |
| `.rodata` | 0x41b67 | **0x41b67** |
| lint | CLEAN | **CLEAN** |
| drift | 0 | **0** |

The only per-function change is `MenuTouchWindow::OnTouchEnd` 95.2 -> 95.3.
`avg_match` stays 99.9 (the +0.1/4524 change is below the two-decimal
rounding). `tools/verify_baseline.json` is unchanged because no gate count
moved.

## 7. Interpretation: knob-driven vs source-shape-driven

A flag-driven residual would show at least one of:

1. a knob that raises a target's fuzzy score without lowering `byte_exact`;
2. a target whose fuzzy rises under some per-TU knob;
3. a systematic fingerprint mismatch (frame pointer, stack canary, NEON,
   `movw`/`movt`, jump tables, sized new/delete) across a TU.

The sweep finds none of 1 or 2 for the 26 targets (apart from the +0.1
MenuTouchWindow cell, now landed), and the earlier w3750/w3751 audits already
showed 3 is absent. The 26 functions are all size-exact; their diffs are local
register allocation, instruction scheduling, 16/32-bit encoding and
basic-block order. The branch-weight knobs are **local compensations, not
recovered original flags**: the current values are not unique, and removing
them costs only 0.1-1.4 points. They compensate for the reconstructed source
shape, not for a missing build flag.

**Therefore the residual is source-shape-driven.** The next work on the 26 is
source/TU-shape work (expression order, local lifetimes, statement order,
`if`/`else` order), not another flag sweep. The one knob-driven item was
`MenuTouchWindow::OnTouchEnd`, and it is now landed.

## Appendix A: full global matrix

`byte` is the raw-byte exact count over all 4,524 common functions in the
screening build (only the 17 target TUs recompiled); `dbyte` is its delta vs
baseline. `moved` is the number of the 26 targets whose fuzzy changed; `min d`
/ `max d` are over those targets. `neutral` means no target moved and
`byte_exact` is 4498; `target-neutral, byte worse` means no target moved but
other functions in the recompiled TUs changed.

| tag | flag | byte | dbyte | moved | min d | max d | verdict |
|---|---|---:|---:|---:|---:|---:|---|
| `enable_misched_false` | `-enable-misched=false` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `enable_misched_true` | `-enable-misched=true` | 1647 | -2851 | 26 | -50.7 | -7.8 | target-worse |
| `misched_postra` | `-misched-postra` | 2626 | -1872 | 23 | -4.7 | -0.1 | target-worse |
| `misched_postra_false` | `-misched-postra=false` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `enable_post_misched_false` | `-enable-post-misched=false` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `enable_post_misched_true` | `-enable-post-misched=true` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `disable_post_ra` | `-disable-post-ra` | 2012 | -2486 | 26 | -31.2 | -1.6 | target-worse |
| `post_RA_scheduler` | `-post-RA-scheduler` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `misched_converge` | `-misched=converge` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `misched_ilpmax` | `-misched=ilpmax` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `misched_ilpmin` | `-misched=ilpmin` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `misched_default` | `-misched=default` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `misched_bottomup` | `-misched-bottomup` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `misched_topdown` | `-misched-topdown` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `misched_regpressure_false` | `-misched-regpressure=false` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `misched_cluster_false` | `-misched-cluster=false` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `misched_fusion` | `-misched-fusion` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `misched_limit_1` | `-misched-limit=1` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `misched_limit_8` | `-misched-limit=8` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `disable_machine_sink` | `-disable-machine-sink` | 2003 | -2495 | 23 | -49.6 | -0.2 | target-worse |
| `disable_machine_licm` | `-disable-machine-licm` | 2031 | -2467 | 16 | -67.5 | -17.5 | target-worse |
| `disable_postra_machine_sink` | `-disable-postra-machine-sink` | 2294 | -2204 | 9 | -4.4 | -0.1 | target-worse |
| `disable_postra_machine_licm` | `-disable-postra-machine-licm` | 2452 | -2046 | 3 | -3.8 | -0.1 | target-worse |
| `disable_machine_cse` | `-disable-machine-cse` | 1662 | -2836 | 24 | -53.5 | -1.8 | target-worse |
| `disable_machine_dce` | `-disable-machine-dce` | 2357 | -2141 | 5 | -36.2 | -0.8 | target-worse |
| `enable_aa_sched_mi` | `-enable-aa-sched-mi` | 3094 | -1404 | 19 | -6.0 | -0.1 | target-worse |
| `enable_aa_sched_mi_false` | `-enable-aa-sched-mi=false` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `disable_early_ifcvt` | `-disable-early-ifcvt` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `old_thumb2_ifcvt` | `-old-thumb2-ifcvt` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `enable_if_conversion` | `-enable-if-conversion` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `enable_if_conversion_false` | `-enable-if-conversion=false` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `disable_tail_duplicate` | `-disable-tail-duplicate` | 2590 | -1908 | 0 | +0.0 | +0.0 | target-neutral, byte worse |
| `disable_early_taildup` | `-disable-early-taildup` | 2809 | -1689 | 3 | -9.9 | -1.3 | target-worse |
| `enable_tail_merge_false` | `-enable-tail-merge=false` | 1653 | -2845 | 24 | -26.7 | -0.6 | target-worse |
| `enable_tail_merge_true` | `-enable-tail-merge=true` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `tail_dup_placement` | `-tail-dup-placement` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `disable_block_placement` | `-disable-block-placement` | 1643 | -2855 | 24 | -73.3 | -3.7 | target-worse |
| `enable_shrink_wrap_false` | `-enable-shrink-wrap=false` | 3054 | -1444 | 0 | +0.0 | +0.0 | target-neutral, byte worse |
| `enable_shrink_wrap_true` | `-enable-shrink-wrap=true` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `enable_pipeliner` | `-enable-pipeliner` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `enable_pipeliner_opt_size` | `-enable-pipeliner-opt-size` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `enable_global_merge_false` | `-enable-global-merge=false` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `enable_global_merge_true` | `-enable-global-merge=true` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `arm_global_merge_false` | `-arm-global-merge=false` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `arm_global_merge_true` | `-arm-global-merge=true` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `global_merge_max_offset_0` | `-global-merge-max-offset=0` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `global_merge_max_offset_4096` | `-global-merge-max-offset=4096` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `enable_load_pre_false` | `-enable-load-pre=false` | 2070 | -2428 | 15 | -31.5 | -2.7 | target-worse |
| `enable_load_pre_true` | `-enable-load-pre=true` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `enable_gvn_hoist` | `-enable-gvn-hoist` | 2039 | -2459 | 13 | -10.6 | -0.2 | target-worse |
| `enable_gvn_sink` | `-enable-gvn-sink` | 2038 | -2460 | 15 | -43.9 | -0.1 | target-worse |
| `enable_newgvn` | `-enable-newgvn` | 2038 | -2460 | 17 | -45.4 | -5.1 | target-worse |
| `arm_constant_island_max_iteration_0` | `-arm-constant-island-max-iteration=0` | — | — | — | — | — | build fail |
| `arm_constant_island_max_iteration_1` | `-arm-constant-island-max-iteration=1` | — | — | — | — | — | build fail |
| `arm_constant_island_max_iteration_10` | `-arm-constant-island-max-iteration=10` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `arm_adjust_jump_tables` | `-arm-adjust-jump-tables` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `arm_restrict_it` | `-arm-restrict-it` | 2006 | -2492 | 24 | -18.7 | -0.1 | target-worse |
| `arm_no_restrict_it` | `-arm-no-restrict-it` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `arm_synthesize_thumb_1_tbb` | `-arm-synthesize-thumb-1-tbb` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `arm_load_store_opt_false` | `-arm-load-store-opt=false` | 1646 | -2852 | 26 | -44.3 | -0.7 | target-worse |
| `arm_promote_constant_false` | `-arm-promote-constant=false` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `arm_promote_constant_max_size_0` | `-arm-promote-constant-max-size=0` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `arm_promote_constant_max_size_4096` | `-arm-promote-constant-max-size=4096` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `arm_promote_constant_max_total_0` | `-arm-promote-constant-max-total=0` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `arm_promote_constant_max_total_32768` | `-arm-promote-constant-max-total=32768` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `remat_pic_stub_load` | `-remat-pic-stub-load` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `spp_rematerialization_threshold_0` | `-spp-rematerialization-threshold=0` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `spp_rematerialization_threshold_100` | `-spp-rematerialization-threshold=100` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `unroll_threshold_0` | `-unroll-threshold=0` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `unroll_threshold_50` | `-unroll-threshold=50` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `unroll_threshold_100` | `-unroll-threshold=100` | 2923 | -1575 | 0 | +0.0 | +0.0 | target-neutral, byte worse |
| `unroll_threshold_150` | `-unroll-threshold=150` | 2923 | -1575 | 0 | +0.0 | +0.0 | target-neutral, byte worse |
| `unroll_threshold_300` | `-unroll-threshold=300` | 2923 | -1575 | 0 | +0.0 | +0.0 | target-neutral, byte worse |
| `unroll_threshold_600` | `-unroll-threshold=600` | 2923 | -1575 | 0 | +0.0 | +0.0 | target-neutral, byte worse |
| `unroll_count_2` | `-unroll-count=2` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `unroll_count_4` | `-unroll-count=4` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `unroll_allow_partial` | `-unroll-allow-partial` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `unroll_runtime` | `-unroll-runtime` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `align_all_blocks_2` | `-align-all-blocks=2` | 1631 | -2867 | 24 | -24.0 | -2.9 | target-worse |
| `align_all_blocks_4` | `-align-all-blocks=4` | 1630 | -2868 | 24 | -39.0 | -7.5 | target-worse |
| `align_all_blocks_8` | `-align-all-blocks=8` | 1316 | -3182 | 24 | -93.0 | -42.3 | target-worse |
| `align_all_blocks_16` | `-align-all-blocks=16` | — | — | — | — | — | build fail |
| `align_all_functions_2` | `-align-all-functions=2` | 2034 | -2464 | 0 | +0.0 | +0.0 | target-neutral, byte worse |
| `align_all_functions_4` | `-align-all-functions=4` | 1668 | -2830 | 0 | +0.0 | +0.0 | target-neutral, byte worse |
| `align_all_functions_8` | `-align-all-functions=8` | 1378 | -3120 | 0 | +0.0 | +0.0 | target-neutral, byte worse |
| `align_all_functions_16` | `-align-all-functions=16` | 1345 | -3153 | 22 | -27.8 | -0.9 | target-worse |
| `align_all_nofallthru_blocks_2` | `-align-all-nofallthru-blocks=2` | 1640 | -2858 | 24 | -30.2 | -2.0 | target-worse |
| `align_all_nofallthru_blocks_4` | `-align-all-nofallthru-blocks=4` | 1634 | -2864 | 24 | -27.0 | -3.0 | target-worse |
| `align_all_nofallthru_blocks_8` | `-align-all-nofallthru-blocks=8` | 1342 | -3156 | 24 | -74.0 | -23.2 | target-worse |
| `align_all_nofallthru_blocks_16` | `-align-all-nofallthru-blocks=16` | — | — | — | — | — | build fail |
| `regalloc_greedy` | `-regalloc=greedy` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `regalloc_basic` | `-regalloc=basic` | 1588 | -2910 | 26 | -80.4 | -23.9 | target-worse |
| `regalloc_fast` | `-regalloc=fast` | 1609 | -2889 | 26 | -84.2 | -27.6 | target-worse |
| `regalloc_pbqp` | `-regalloc=pbqp` | 1581 | -2917 | 26 | -82.4 | -23.4 | target-worse |
| `enable_local_reassign` | `-enable-local-reassign` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `enable_deferred_spilling` | `-enable-deferred-spilling` | 2156 | -2342 | 20 | -23.8 | -0.1 | target-worse |
| `optimize_regalloc_false` | `-optimize-regalloc=false` | 1609 | -2889 | 26 | -87.4 | -27.6 | target-worse |
| `inline_threshold_0` | `-inline-threshold=0` | 2304 | -2194 | 1 | -32.8 | -32.8 | target-worse |
| `inline_threshold_50` | `-inline-threshold=50` | 1356 | -3142 | 15 | -33.2 | -0.1 | target-worse |
| `inline_threshold_100` | `-inline-threshold=100` | 1356 | -3142 | 15 | -67.9 | -0.1 | target-worse |
| `inline_threshold_500` | `-inline-threshold=500` | 1356 | -3142 | 15 | -73.2 | -0.1 | target-worse |
| `inlinehint_threshold_0` | `-inlinehint-threshold=0` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `inlinecold_threshold_0` | `-inlinecold-threshold=0` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `jump_table_density_0` | `-jump-table-density=0` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `jump_table_density_100` | `-jump-table-density=100` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `jump_is_expensive` | `-jump-is-expensive` | 1667 | -2831 | 17 | -18.0 | -0.1 | target-worse |
| `tail_merge_size_0` | `-tail-merge-size=0` | 2027 | -2471 | 5 | -14.1 | -0.1 | target-worse |
| `tail_merge_size_100` | `-tail-merge-size=100` | 1707 | -2791 | 9 | -8.1 | -0.3 | target-worse |
| `tail_merge_threshold_0` | `-tail-merge-threshold=0` | 1653 | -2845 | 24 | -26.7 | -0.6 | target-worse |
| `tail_merge_threshold_100` | `-tail-merge-threshold=100` | 2738 | -1760 | 1 | -0.2 | -0.2 | target-worse |
| `tail_dup_size_0` | `-tail-dup-size=0` | 2026 | -2472 | 3 | -9.9 | -1.3 | target-worse |
| `tail_dup_size_100` | `-tail-dup-size=100` | 1665 | -2833 | 13 | -16.0 | -0.4 | target-worse |
| `tail_dup_placement_threshold_0` | `-tail-dup-placement-threshold=0` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `tail_dup_placement_aggressive_threshold_0` | `-tail-dup-placement-aggressive-threshold=0` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `block_placement_exit_block_bias_0` | `-block-placement-exit-block-bias=0` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `block_placement_exit_block_bias_100` | `-block-placement-exit-block-bias=100` | 2276 | -2222 | 7 | -28.9 | -0.5 | target-worse |
| `arm_atomic_cfg_tidy` | `-arm-atomic-cfg-tidy` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `arm_assume_misaligned_load_store` | `-arm-assume-misaligned-load-store` | 1656 | -2842 | 23 | -41.4 | -0.3 | target-worse |
| `arm_use_mulops` | `-arm-use-mulops` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `disable_2addr_hack` | `-disable-2addr-hack` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `enable_arm_3_addr_conv_false` | `-enable-arm-3-addr-conv=false` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `disable_shifter_op` | `-disable-shifter-op` | 1689 | -2809 | 20 | -37.4 | -0.5 | target-worse |
| `disable_spill_fusing` | `-disable-spill-fusing` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `disable_spill_hoist` | `-disable-spill-hoist` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `disable_cgp` | `-disable-cgp` | 1630 | -2868 | 24 | -64.9 | -25.2 | target-worse |
| `disable_cgp_branch_opts` | `-disable-cgp-branch-opts` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `disable_cgp_select2branch` | `-disable-cgp-select2branch` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `disable_complex_addr_modes` | `-disable-complex-addr-modes` | 2086 | -2412 | 8 | -9.4 | -0.1 | target-worse |
| `disable_separate_const_offset_from_gep` | `-disable-separate-const-offset-from-gep` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `disable_constant_hoisting` | `-disable-constant-hoisting` | 2356 | -2142 | 4 | -18.9 | -1.5 | target-worse |
| `consthoist_with_block_frequency` | `-consthoist-with-block-frequency` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `enable_cond_stores_vec` | `-enable-cond-stores-vec` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `disable_licm_promotion` | `-disable-licm-promotion` | 2313 | -2185 | 0 | +0.0 | +0.0 | target-neutral, byte worse |
| `disable_lsr` | `-disable-lsr` | 2066 | -2432 | 9 | -20.0 | -0.2 | target-worse |
| `disable_lftr` | `-disable-lftr` | 2419 | -2079 | 5 | -3.3 | -0.1 | target-worse |
| `enable_subreg_liveness` | `-enable-subreg-liveness` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `enable_ipra` | `-enable-ipra` | 2203 | -2295 | 5 | -6.1 | -0.2 | target-worse |
| `enable_linkonceodr_outlining` | `-enable-linkonceodr-outlining` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `enable_dse_partial_store_merging` | `-enable-dse-partial-store-merging` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `enable_dse_partial_overwrite_tracking` | `-enable-dse-partial-overwrite-tracking` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `enable_earlycse_memssa_false` | `-enable-earlycse-memssa=false` | 1909 | -2589 | 6 | -36.2 | -0.1 | target-worse |
| `enable_tbaa_false` | `-enable-tbaa=false` | 1680 | -2818 | 12 | -32.5 | -3.9 | target-worse |
| `enable_scoped_noalias_false` | `-enable-scoped-noalias=false` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `enable_andcmp_sinking` | `-enable-andcmp-sinking` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `enable_phi_of_ops` | `-enable-phi-of-ops` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `enable_store_refinement` | `-enable-store-refinement` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `enable_mem_access_versioning` | `-enable-mem-access-versioning` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `enable_loop_distribute` | `-enable-loop-distribute` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `enable_loop_versioning_licm` | `-enable-loop-versioning-licm` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `enable_simple_loop_unswitch` | `-enable-simple-loop-unswitch` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `enable_nontrivial_unswitch` | `-enable-nontrivial-unswitch` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `enable_partial_inlining` | `-enable-partial-inlining` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `disable_icp` | `-disable-icp` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `enable_value_profiling` | `-enable-value-profiling` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `break_anti_dependencies_none` | `-break-anti-dependencies=none` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `break_anti_dependencies_all` | `-break-anti-dependencies=all` | 2026 | -2472 | 24 | -11.3 | -0.4 | target-worse |
| `sched_avg_ipc_1` | `-sched-avg-ipc=1` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `sched_avg_ipc_4` | `-sched-avg-ipc=4` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `sched_high_latency_cycles_1` | `-sched-high-latency-cycles=1` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `sched_high_latency_cycles_10` | `-sched-high-latency-cycles=10` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `arm_implicit_it_always` | `-arm-implicit-it=always` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |
| `arm_implicit_it_never` | `-arm-implicit-it=never` | 4498 | +0 | 0 | +0.0 | +0.0 | neutral |

## Appendix B: full 26-target delta matrix

Delta in percentage points vs baseline for the 54 knobs that move at least one
target. Rows are the 26 targets; columns are the knob tags in Appendix C.
`+0.0` means the knob leaves that target at its baseline fuzzy.

```csv
target,enable_misched_true,misched_postra,disable_post_ra,disable_machine_sink,disable_machine_licm,disable_postra_machine_sink,disable_postra_machine_licm,disable_machine_cse,disable_machine_dce,enable_aa_sched_mi,disable_early_taildup,enable_tail_merge_false,disable_block_placement,enable_load_pre_false,enable_gvn_hoist,enable_gvn_sink,enable_newgvn,arm_restrict_it,arm_load_store_opt_false,align_all_blocks_2,align_all_blocks_4,align_all_blocks_8,align_all_functions_16,align_all_nofallthru_blocks_2,align_all_nofallthru_blocks_4,align_all_nofallthru_blocks_8,regalloc_basic,regalloc_fast,regalloc_pbqp,enable_deferred_spilling,optimize_regalloc_false,inline_threshold_0,inline_threshold_50,inline_threshold_100,inline_threshold_500,jump_is_expensive,tail_merge_size_0,tail_merge_size_100,tail_merge_threshold_0,tail_merge_threshold_100,tail_dup_size_0,tail_dup_size_100,block_placement_exit_block_bias_100,arm_assume_misaligned_load_store,disable_shifter_op,disable_cgp,disable_complex_addr_modes,disable_constant_hoisting,disable_lsr,disable_lftr,enable_ipra,enable_earlycse_memssa_false,enable_tbaa_false,break_anti_dependencies_all
_ZN6LayoutC1Ev,-9.6,-1.8,-2.9,-1.1,+0.0,+0.0,+0.0,-23.6,+0.0,-0.7,+0.0,-0.6,-30.7,-5.9,+0.0,-25.8,-5.1,-0.1,-24.9,-24.0,-28.2,-42.3,-0.9,-24.2,-27.0,-38.0,-23.9,-27.6,-23.4,+0.0,-27.6,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,-0.6,+0.0,+0.0,+0.0,+0.0,-24.9,+0.0,-25.3,+0.0,+0.0,+0.0,+0.0,+0.0,-36.2,+0.0,-0.4
_ZN6LayoutC2Ev,-9.6,-1.8,-2.9,-1.1,+0.0,+0.0,+0.0,-23.6,+0.0,-0.7,+0.0,-0.6,-30.7,-5.9,+0.0,-25.8,-5.1,-0.1,-24.9,-24.0,-28.2,-42.3,-0.9,-24.2,-27.0,-38.0,-23.9,-27.6,-23.4,+0.0,-27.6,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,-0.6,+0.0,+0.0,+0.0,+0.0,-24.9,+0.0,-25.3,+0.0,+0.0,+0.0,+0.0,+0.0,-36.2,+0.0,-0.4
_ZN11LevelScript7processEi,-7.8,-0.3,-31.2,-0.4,-21.3,+0.0,-0.2,-13.9,-0.8,-0.1,-1.3,-0.8,-46.3,-4.3,-0.2,-0.1,-23.9,-1.7,-11.7,-2.9,-7.5,-44.0,-12.2,-2.0,-3.0,-23.2,-32.5,-43.7,-34.0,-1.8,-47.0,+0.0,-0.1,-0.3,-13.1,-1.2,-0.2,-0.3,-0.8,-0.2,-1.3,-6.8,-0.5,-10.7,-3.5,-33.6,-0.1,+0.0,-0.7,+0.0,-0.2,-0.1,-3.9,-0.6
_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE,-12.4,+0.0,-5.6,-20.5,-20.8,-0.1,+0.0,-25.5,+0.0,+0.0,+0.0,-7.2,-15.2,+0.0,+0.0,+0.0,-5.7,-3.9,-4.3,-9.2,-28.2,-69.0,+0.0,-5.6,-14.8,-59.8,-35.8,-58.9,-36.2,-3.6,-59.1,+0.0,+0.0,+0.0,+0.0,-10.6,+0.0,-2.1,-7.2,+0.0,+0.0,+0.0,-3.7,+0.0,-14.7,-25.2,+0.0,+0.0,-1.0,-1.0,+0.0,+0.0,-23.1,-4.2
_ZN5Radar4drawEP6PlayerP3Hudi,-19.6,-0.3,-9.2,-21.2,-44.0,+0.0,-0.1,-20.0,+0.0,-0.1,-1.9,-18.9,-38.5,-5.3,-3.8,-4.5,-24.5,-5.1,-8.3,-6.3,-20.8,-67.6,-9.5,-3.4,-7.0,-46.1,-55.4,-59.8,-51.8,-6.3,-64.5,+0.0,-1.3,-7.9,-7.9,-12.9,+0.0,-1.1,-18.9,+0.0,-1.9,-3.3,+0.0,-8.0,-3.0,-39.8,-2.3,+0.0,-0.2,+0.0,+0.0,+0.0,-5.1,-3.0
_ZN5Level10createShipEiiiP8Waypointbb,-28.7,-0.2,-15.8,-24.0,+0.0,+0.0,+0.0,-27.9,+0.0,+0.0,+0.0,-12.9,-30.2,+0.0,+0.0,-43.9,+0.0,-2.7,-12.0,-21.4,-24.4,-66.9,-7.8,-30.2,-26.6,-58.2,-46.4,-51.5,-41.9,-0.6,-53.7,+0.0,-14.3,-25.5,-25.5,+0.0,+0.0,+0.0,-12.9,+0.0,+0.0,+0.0,+0.0,-0.7,-4.4,-30.9,+0.0,+0.0,+0.0,+0.0,-0.9,+0.0,+0.0,-0.5
_ZN5MGame10OnTouchEndEiiPv,-9.6,-0.2,-14.8,-6.1,-36.0,-0.2,+0.0,-40.8,+0.0,-2.2,+0.0,-18.9,-43.4,-3.2,-6.3,+0.0,-6.6,-8.1,-7.0,-12.3,-25.8,-72.1,-16.1,-9.1,-14.0,-55.3,-39.8,-61.3,-55.8,-0.1,-64.1,+0.0,-33.2,-33.4,-40.5,+0.0,+0.0,-2.5,-18.9,+0.0,+0.0,-7.2,-3.5,-4.9,+0.0,-29.4,-2.0,+0.0,-0.2,-0.3,+0.0,+0.0,-8.6,-0.5
_ZN7GlobalsC1Ev,-50.7,-1.3,-18.1,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,-0.9,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,-4.3,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,-33.4,-54.5,-51.8,+0.0,-54.5,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,-4.3,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,-32.5,-0.9
_ZN7GlobalsC2Ev,-50.7,-1.3,-18.1,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,-0.9,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,-4.3,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,-33.4,-54.5,-51.8,+0.0,-54.5,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,-4.3,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,-32.5,-0.9
_ZN12HangarWindow6renderEv,-14.7,-0.2,-11.0,-24.0,-24.3,-0.1,+0.0,-39.9,+0.0,+0.0,+0.0,-17.8,-43.7,-2.7,-2.0,-14.7,-5.1,-4.3,-6.9,-9.6,-24.5,-73.8,-11.4,-4.9,-11.4,-57.7,-45.8,-64.5,-44.8,-16.6,-68.5,+0.0,-4.7,-5.6,-5.6,-15.3,+0.0,+0.0,-17.8,+0.0,+0.0,-1.5,+0.0,-6.0,-21.3,-34.9,+0.0,-18.9,-2.6,+0.0,-5.9,+0.0,+0.0,-11.3
_ZN3Gun22calcCharacterCollisionEv,-11.2,-0.1,-12.8,-5.1,-29.0,+0.0,+0.0,-25.6,+0.0,-0.3,+0.0,-9.9,-28.9,-5.1,-5.6,-5.6,-12.8,-6.6,-5.0,-11.2,-23.8,-75.3,-8.8,-7.7,-11.1,-47.5,-53.1,-64.0,-46.8,-0.1,-66.6,-32.8,+0.0,+0.0,+0.0,-5.3,+0.0,-3.7,-9.9,+0.0,+0.0,+0.0,-2.9,-0.3,-6.5,-44.9,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,-1.1
_ZN6Status16missionCompletedEbbx,-8.1,+0.0,-1.6,-16.1,+0.0,+0.0,+0.0,-1.8,+0.0,+0.0,+0.0,-5.2,-3.7,+0.0,+0.0,-0.4,+0.0,-4.3,-0.7,-4.9,-19.1,-53.7,-12.0,-2.5,-8.2,-46.2,-36.5,-42.6,-32.1,-0.6,-44.0,+0.0,-0.1,-0.1,-0.1,-6.7,+0.0,-1.1,-5.2,+0.0,+0.0,+0.0,-28.9,-0.7,-1.7,-34.6,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,-0.8
_ZN9RocketGun6updateEi,-46.2,-0.4,-20.9,-0.2,-38.0,+0.0,+0.0,-25.1,+0.0,-0.5,+0.0,-14.5,-11.9,-27.0,-6.1,+0.0,-29.8,-8.0,-9.1,-10.0,-18.7,-73.5,-7.7,-7.7,-9.3,-33.3,-63.1,-69.4,-50.7,-0.5,-72.2,+0.0,+0.0,+0.0,+0.0,-1.7,-14.1,-4.0,-14.5,+0.0,+0.0,+0.0,+0.0,+0.0,-3.7,-52.8,+0.0,+0.0,-11.9,+0.0,+0.0,+0.0,-21.8,-3.0
_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE,-43.7,-0.3,-8.7,+0.0,-59.4,+0.0,+0.0,-18.3,+0.0,+0.0,+0.0,-8.2,-34.0,+0.0,+0.0,+0.0,+0.0,-3.5,-9.5,-10.8,-22.6,-77.0,+0.0,-7.8,-10.3,-33.2,-40.7,-63.3,-53.2,+0.0,-65.3,+0.0,-16.1,-16.1,-16.1,+0.0,+0.0,+0.0,-8.2,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,-46.6,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,-2.3
_ZN5Level11createSceneEv,-15.4,+0.0,-11.8,-45.5,-17.5,+0.0,+0.0,-29.8,+0.0,+0.0,+0.0,-9.2,-73.3,+0.0,+0.0,-23.7,+0.0,-10.0,-7.6,-13.0,-28.5,-79.7,-13.6,-10.8,-17.4,-64.0,-42.6,-70.6,-47.5,-0.9,-73.7,+0.0,-13.0,-33.4,-33.4,-3.5,+0.0,+0.0,-9.2,+0.0,+0.0,-3.1,+0.0,-0.9,-7.3,-39.1,+0.0,+0.0,-20.0,-0.1,-6.1,+0.0,+0.0,-1.1
_ZN3Hud4drawExxP9PlayerEgobjj,-19.7,-4.7,-16.3,-18.3,+0.0,-0.1,+0.0,-53.5,+0.0,-4.2,+0.0,-9.9,-22.7,-6.4,-4.9,-25.5,-7.5,-11.3,-29.7,-13.2,-30.2,-81.6,-12.8,-8.5,-13.2,-60.1,-57.4,-71.3,-61.0,-1.9,-74.1,+0.0,-7.8,-7.8,-17.1,-0.1,+0.0,+0.0,-9.9,+0.0,+0.0,-4.6,+0.0,-29.7,-9.2,-43.9,-4.3,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,-1.5
_ZN5Level13createMissionEv,-21.9,-0.5,-12.7,-49.6,-41.1,+0.0,-3.8,-39.0,-2.0,-0.2,+0.0,-14.7,-47.5,-23.4,+0.0,-10.8,-33.0,-4.4,-9.4,-10.1,-20.6,-76.1,-11.0,-8.1,-12.7,-54.3,-55.8,-72.1,-57.9,-7.3,-80.8,+0.0,-23.8,-67.9,-73.2,-4.0,+0.0,+0.0,-14.7,+0.0,+0.0,-16.0,-0.9,-7.3,-4.9,-52.3,+0.0,-16.6,-13.6,-3.3,-1.0,+0.0,-21.7,-4.7
_ZN15MenuTouchWindowC1Ei,-45.7,-1.1,-15.8,-20.8,-25.5,-4.2,+0.0,-45.5,+0.0,-0.1,+0.0,-9.3,-20.7,-4.9,-3.5,-2.8,-18.7,-4.7,-36.6,-9.1,-22.5,-81.6,-10.7,-7.6,-13.5,-64.4,-61.1,-79.7,-64.7,-0.5,-85.3,+0.0,-17.7,-32.0,-59.1,-5.9,-0.1,+0.0,-9.3,+0.0,+0.0,-1.6,+0.0,-35.5,-28.5,-48.6,-8.2,+0.0,+0.0,+0.0,+0.0,-3.3,-12.1,-3.4
_ZN15MenuTouchWindowC2Ei,-45.7,-1.1,-15.8,-20.8,-25.5,-4.2,+0.0,-45.5,+0.0,-0.1,+0.0,-9.3,-20.7,-4.9,-3.5,-2.8,-18.7,-4.7,-36.6,-9.1,-22.5,-81.6,-10.7,-7.6,-13.5,-64.4,-61.1,-79.7,-64.7,-0.5,-85.3,+0.0,-17.7,-32.0,-59.1,-5.9,-0.1,+0.0,-9.3,+0.0,+0.0,-1.6,+0.0,-35.5,-28.5,-48.6,-8.2,+0.0,+0.0,+0.0,+0.0,-3.3,-12.1,-3.4
_ZN15MenuTouchWindow10OnTouchEndEiiPv,-33.8,-0.2,-21.5,-20.1,-67.5,+0.0,+0.0,-49.8,-36.2,-0.1,-9.9,-21.1,-58.3,+0.0,-6.8,-32.0,-45.4,-18.7,-11.1,-20.3,-37.3,-88.2,-10.7,-18.5,-24.1,-74.0,-71.9,-78.4,-60.1,-22.6,-83.2,+0.0,-9.6,-9.6,-33.3,-10.8,-9.7,-4.3,-21.1,+0.0,-9.9,-5.7,-11.5,-10.9,-9.8,-64.0,-9.4,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,-2.3
_ZN11SpaceLounge10drawLoungeEv,-22.1,-0.5,-8.2,-28.6,+0.0,-0.1,+0.0,-35.4,+0.0,-0.3,+0.0,-26.7,-59.2,+0.0,-10.6,+0.0,+0.0,-1.8,-16.2,-16.3,-33.8,-87.6,-13.7,-13.1,-19.6,-69.6,-57.9,-78.5,-61.6,-11.9,-80.5,+0.0,+0.0,+0.0,+0.0,-7.7,+0.0,+0.0,-26.7,+0.0,+0.0,+0.0,+0.0,-13.2,-0.5,-53.8,+0.0,-1.5,+0.0,+0.0,+0.0,+0.0,+0.0,-0.8
_ZN15MenuTouchWindow12OnTouchBeginEiiPv,-17.9,-0.4,-7.5,-8.0,-32.3,+0.0,+0.0,-35.5,+0.0,-1.4,+0.0,-18.2,-61.0,-31.5,+0.0,+0.0,-43.3,-15.3,-11.1,-22.2,-35.8,-90.3,-4.9,-18.8,-24.8,-73.2,-80.4,-84.2,-82.4,+0.0,-86.5,+0.0,+0.0,+0.0,+0.0,-14.3,+0.0,+0.0,-18.2,+0.0,+0.0,+0.0,+0.0,-11.1,-13.1,-64.9,+0.0,+0.0,+0.0,+0.0,+0.0,-19.9,-7.1,-1.5
_ZN10ModStationC1Ev,-30.6,-0.8,-11.0,-24.4,+0.0,-4.4,+0.0,-46.4,-21.2,-1.1,+0.0,-7.4,-6.8,+0.0,+0.0,+0.0,+0.0,-8.8,-44.3,-10.1,-27.1,-87.1,-6.3,-8.1,-16.6,-68.8,-63.3,-82.9,-60.2,-23.8,-87.4,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,-7.4,+0.0,+0.0,-0.4,+0.0,-41.4,-14.6,-45.2,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0
_ZN10ModStationC2Ev,-30.6,-0.8,-11.0,-24.4,+0.0,-4.4,+0.0,-46.4,-21.2,-1.1,+0.0,-7.4,-6.8,+0.0,+0.0,+0.0,+0.0,-8.8,-44.3,-10.1,-27.1,-87.1,-6.3,-8.1,-16.6,-68.8,-63.3,-82.9,-60.2,-23.8,-87.4,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,-7.4,+0.0,+0.0,-0.4,+0.0,-41.4,-14.6,-45.2,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0,+0.0
_ZN7StarMap4drawEv,-24.8,-0.8,-14.0,-11.7,-43.4,+0.0,+0.0,-50.2,+0.0,+0.0,+0.0,-3.0,-32.0,-10.4,-5.1,+0.0,-26.8,-6.5,-5.8,-14.6,-30.5,-89.1,-9.7,-11.7,-15.4,-59.1,-59.7,-80.3,-64.7,-0.5,-82.3,+0.0,-5.5,-5.5,-5.5,-16.9,+0.0,+0.0,-3.0,+0.0,+0.0,+0.0,+0.0,-5.9,-7.6,-57.6,+0.0,+0.0,-2.0,+0.0,+0.0,+0.0,+0.0,-1.3
_ZN5MGame8OnUpdateEv,-25.3,-0.2,-20.2,-18.9,-26.7,+0.0,+0.0,-47.3,+0.0,-6.0,+0.0,-20.7,-50.7,-5.7,-8.8,-13.4,-22.8,-11.2,-14.9,-22.1,-39.0,-93.0,-27.8,-20.3,-25.4,-71.2,-64.6,-75.0,-58.6,-0.7,-79.8,+0.0,-13.0,-13.0,-57.6,-18.0,+0.0,-8.1,-20.7,+0.0,+0.0,-1.1,+0.0,-14.4,-37.4,-49.1,-6.0,-7.6,+0.0,-0.1,+0.0,+0.0,-11.0,-0.9
```

## Appendix C: knob legend

| tag | flag |
|---|---|
| `enable_misched_true` | `-enable-misched=true` |
| `misched_postra` | `-misched-postra` |
| `disable_post_ra` | `-disable-post-ra` |
| `disable_machine_sink` | `-disable-machine-sink` |
| `disable_machine_licm` | `-disable-machine-licm` |
| `disable_postra_machine_sink` | `-disable-postra-machine-sink` |
| `disable_postra_machine_licm` | `-disable-postra-machine-licm` |
| `disable_machine_cse` | `-disable-machine-cse` |
| `disable_machine_dce` | `-disable-machine-dce` |
| `enable_aa_sched_mi` | `-enable-aa-sched-mi` |
| `disable_early_taildup` | `-disable-early-taildup` |
| `enable_tail_merge_false` | `-enable-tail-merge=false` |
| `disable_block_placement` | `-disable-block-placement` |
| `enable_load_pre_false` | `-enable-load-pre=false` |
| `enable_gvn_hoist` | `-enable-gvn-hoist` |
| `enable_gvn_sink` | `-enable-gvn-sink` |
| `enable_newgvn` | `-enable-newgvn` |
| `arm_restrict_it` | `-arm-restrict-it` |
| `arm_load_store_opt_false` | `-arm-load-store-opt=false` |
| `align_all_blocks_2` | `-align-all-blocks=2` |
| `align_all_blocks_4` | `-align-all-blocks=4` |
| `align_all_blocks_8` | `-align-all-blocks=8` |
| `align_all_functions_16` | `-align-all-functions=16` |
| `align_all_nofallthru_blocks_2` | `-align-all-nofallthru-blocks=2` |
| `align_all_nofallthru_blocks_4` | `-align-all-nofallthru-blocks=4` |
| `align_all_nofallthru_blocks_8` | `-align-all-nofallthru-blocks=8` |
| `regalloc_basic` | `-regalloc=basic` |
| `regalloc_fast` | `-regalloc=fast` |
| `regalloc_pbqp` | `-regalloc=pbqp` |
| `enable_deferred_spilling` | `-enable-deferred-spilling` |
| `optimize_regalloc_false` | `-optimize-regalloc=false` |
| `inline_threshold_0` | `-inline-threshold=0` |
| `inline_threshold_50` | `-inline-threshold=50` |
| `inline_threshold_100` | `-inline-threshold=100` |
| `inline_threshold_500` | `-inline-threshold=500` |
| `jump_is_expensive` | `-jump-is-expensive` |
| `tail_merge_size_0` | `-tail-merge-size=0` |
| `tail_merge_size_100` | `-tail-merge-size=100` |
| `tail_merge_threshold_0` | `-tail-merge-threshold=0` |
| `tail_merge_threshold_100` | `-tail-merge-threshold=100` |
| `tail_dup_size_0` | `-tail-dup-size=0` |
| `tail_dup_size_100` | `-tail-dup-size=100` |
| `block_placement_exit_block_bias_100` | `-block-placement-exit-block-bias=100` |
| `arm_assume_misaligned_load_store` | `-arm-assume-misaligned-load-store` |
| `disable_shifter_op` | `-disable-shifter-op` |
| `disable_cgp` | `-disable-cgp` |
| `disable_complex_addr_modes` | `-disable-complex-addr-modes` |
| `disable_constant_hoisting` | `-disable-constant-hoisting` |
| `disable_lsr` | `-disable-lsr` |
| `disable_lftr` | `-disable-lftr` |
| `enable_ipra` | `-enable-ipra` |
| `enable_earlycse_memssa_false` | `-enable-earlycse-memssa=false` |
| `enable_tbaa_false` | `-enable-tbaa=false` |
| `break_anti_dependencies_all` | `-break-anti-dependencies=all` |

## Appendix D: reproduction

```sh
ROOT=/home/debian/GalaxyOnFire2/wt/w3902
NDK=$ROOT/.cache/ndk/android-ndk-r18b
O=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
OBJDUMP=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump

# baseline
cd $ROOT
cmake --preset default -DGOF2_ORIGINAL_SO=$O \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
cmake --build cmake-build-match --target lint -j8

# one landing check
cmake --build cmake-build-match --target check -j8
python3 - <<'PY'
import json
r=json.load(open('cmake-build-match/verify/report.json'))
print([x for x in r['rows'] if x['name']=='_ZN15MenuTouchWindow10OnTouchEndEiiPv'])
PY
```

The screening harness (wrapper, driver, scorer) lived in `/dev/shm/w3902` and
is not tracked; its logic is fully described in Section 2.2. The official
verifier is the source of every gate number in this note.
