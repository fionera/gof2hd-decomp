# Per-TU compiler-flag sensitivity, wave 120 (w7005)

Status: research note, 2026-11-06. Worktree `/home/debian/GalaxyOnFire2/wt/w7005`,
branch `wave120-w7005`, base `ef7fb9cde`. Scratch work is under `/dev/shm/w7005`.
**No `src/`, `tools/`, baseline or allowlist change is landed.** This note is the only commit.

Canonical gate, measured before and after this pass (unchanged):

```text
verify: byte 4505  linked 4505  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint CLEAN ✓   drift 0
```

The original is 2,187,328 B. The 19 non-exact functions are all size-exact at baseline.

## 0. Bottom line

* **450 cells** were compiled, linked and scored: 14 TUs owning the 19 non-exact
  functions × 32 uniform flags, plus two SpaceLounge TU-specific cells. There were
  **0 compile failures and 0 link failures**.
* **No cell improves any non-exact function.** All 19 stay non-byte-exact; no cell
  raises a target's official fuzzy percentage; no cell makes a target `byte_exact`
  or `linked_exact`. The best delta is **+0.0**.
* 153 cells change the target function's object bytes; 148 of those lower the
  target percentage. The remaining **5 cells (6 target functions)** change target
  bytes but leave the rounded percentage unchanged. 45 further cells change only
  non-target functions in the same TU.
* `-Oz` is uniquely best among the tested optimization levels. `-O2`/`-Os` change
  every target and lower every target; `-O0`/`-O1`/`-O3` are much worse.
* `-g`/`-g2` are exactly baseline; `-g0` changes 7 target functions and lowers 3
  (including `MenuTouchWindow::OnTouchBegin` 98.1 -> 83.2). `-g` is load-bearing.
* Fully neutral on all 14 TUs: `-fno-vectorize`, `-fno-omit-frame-pointer`,
  `-mno-restrict-it`, `-fno-math-errno`, `-ffp-contract=off`, `-ffp-contract=fast`,
  `-fno-delete-null-pointer-checks`, `-fno-threadsafe-statics`,
  `-fno-use-cxa-atexit`, `-fno-sized-deallocation`, `-fno-common`,
  `-fvisibility=default`, and the baseline-equivalent `-Oz`/`-g`/`-g2`.
* The SpaceLounge `-mllvm -likely-branch-weight=7 -mllvm -unlikely-branch-weight=1`
  override already in `CMakeLists.txt` is real but only a matching knob: removing
  it changes the target bytes and drops `drawLounge` 95.7 -> 95.5, with the gate
  still 4505/4505. It is not a gate improvement.
* **Conclusion:** in the tested single-flag space, the remaining 19 mismatches are
  source-shape residuals, not per-TU build-flag artifacts.

## 1. TU map

The 14 TUs and 19 functions were identified from `report.json` and the baseline
objects' symtabs, then cross-checked against `_work/original_layout/tu_mapping.md`.
All 14 are `exact(HD)` original game TUs; no invented or split TU is involved.

| our TU | original TU (dump) | non-exact function(s) |
|---|---|---|
| `src/game/ui/Layout.cpp` | `Classes/Game/Layout.cpp` | `_ZN6LayoutC1Ev`, `_ZN6LayoutC2Ev` |
| `src/game/world/LevelScript.cpp` | `Classes/Game/LevelScript.cpp` | `_ZN11LevelScript7processEi` |
| `src/game/world/Level.cpp` | `Classes/Game/Level.cpp` | `_ZN5Level10createShipEiiiP8Waypointbb`, `_ZN5Level13createMissionEv`, `_ZN5Level11createSceneEv` |
| `src/game/weapons/Radar.cpp` | `Classes/Game/Radar.cpp` | `_ZN5Radar4drawEP6PlayerP3Hudi` |
| `src/game/menu/MGame.cpp` | `Classes/Game/MGame.cpp` | `_ZN5MGame10OnTouchEndEiiPv`, `_ZN5MGame8OnUpdateEv` |
| `src/game/mission/Generator.cpp` | `Classes/Game/Generator.cpp` | `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` |
| `src/game/ui/HangarWindow.cpp` | `Classes/Game/HangarWindow.cpp` | `_ZN12HangarWindow6renderEv` |
| `src/game/mission/Status.cpp` | `Classes/Game/Status.cpp` | `_ZN6Status16missionCompletedEbbx` |
| `src/game/weapons/Gun.cpp` | `Classes/Game/Gun.cpp` | `_ZN3Gun22calcCharacterCollisionEv` |
| `src/game/ship/PlayerEgo.cpp` | `Classes/Game/PlayerEgo.cpp` | `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` |
| `src/game/menu/SpaceLounge.cpp` | `Classes/Game/SpaceLounge.cpp` | `_ZN11SpaceLounge10drawLoungeEv` |
| `src/game/ui/Hud.cpp` | `Classes/Game/Hud.cpp` | `_ZN3Hud4drawExxP9PlayerEgobjj` |
| `src/game/ui/MenuTouchWindow.cpp` | `Classes/Game/MenuTouchWindow.cpp` | `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` |
| `src/game/menu/ModStation.cpp` | `Classes/Game/ModStation.cpp` | `_ZN10ModStationC1Ev`, `_ZN10ModStationC2Ev` |

## 2. Method

For each `(TU, flag)` cell:

1. Take the exact canonical compile command from
   `cmake-build-match/compile_commands.json`, replace only `-o`, and append the
   candidate flag(s). Compile the whole TU.
2. Read every defined `STT_FUNC` symbol from the variant ET_REL object and compare
   its raw bytes, size and SHA-1 with the canonical object. This is the
   **"changes code"** measurement.
3. Relink the whole `.so` with the exact canonical gold command, substituting only
   that TU's object, then run the same GNU `strip --strip-all` and
   `tools/reorder_shstrtab.py` post-link steps. This is a real full link, not a
   partial-object estimate.
4. Score the target functions with the official verifier's own
   `disassemble()`/`reconcile()`/`score()` path (`autojunk=False`), against the
   original `.so`. This is the **"improves match"** measurement.
5. For selected cells, run the full official `tools/verify/verify.py` and report
   its `byte_exact`/`linked_exact`/`size_exact` gate counts.

Baseline sanity: the `base` cell reproduces the canonical `report.json` percentage
for all 19 functions exactly (49.0, 49.0, 60.0, 74.4, 74.5, 81.6, 84.7, 89.7,
92.3, 92.5, 92.8, 93.4, 95.5, 95.7, 96.0, 98.1, 99.2, 99.2, 99.9), and the
canonical relink is 2,187,328 B.

The uniform flags are:

```text
base, -Oz, -O0, -O1, -O2, -O3, -Os, -g, -g0, -g2,
-fno-strict-aliasing, -fno-inline, -fno-slp-vectorize, -fno-vectorize,
-fno-unroll-loops, -fno-jump-tables, -fno-omit-frame-pointer,
-fno-stack-protector, -fstack-protector, -mno-unaligned-access,
-mno-restrict-it, -mrestrict-it, -fno-math-errno, -fno-honor-nans,
-ffp-contract=off, -ffp-contract=fast, -fno-delete-null-pointer-checks,
-fno-threadsafe-statics, -fno-use-cxa-atexit, -fno-sized-deallocation,
-fno-common, -fvisibility=default
```

`-O0`/`-O1`/`-O3` are extra context rows beyond the requested `-Oz`/`-O2`/`-Os`.
`-g` and `-g2` are the canonical `-g` default; `-Oz` is the canonical level.

SpaceLounge has two extra rows:

* `tu_specific`: the canonical command including the checked-in 7:1 branch-weight
  pair (identical to `base` for that TU).
* `tu_specific_off`: the canonical command with the 7:1 pair removed.

## 3. Matrix

Notation: each cell is the **maximum target percentage delta** over the targets
owned by that TU. `0` means no rounded change; `+x` would be an improvement; `-x`
is a regression. `*` means the target function's object bytes differ from the
canonical object (so the cell "changes code" even when the rounded percentage is
unchanged). `·` means the row does not apply to that TU.

### Matrix A
| TU | `base` | `Oz` | `O0` | `O1` | `O2` | `O3` | `Os` | `g` | `g0` | `g2` | `fno_strict_aliasing` | `fno_inline` | `fno_slp_vectorize` | `fno_vectorize` | `fno_unroll_loops` | `fno_jump_tables` |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| `Layout.cpp` | 0 | 0 | -48.2* | -46.6* | -4.8* | -25.9* | -1.5* | 0 | 0 | 0 | 0 | 0 | -30.6* | 0 | 0 | 0 |
| `LevelScript.cpp` | 0 | 0 | -57.6* | -52.9* | -33.8* | -45.7* | -31.7* | 0 | 0 | 0 | -2.0* | -27.3* | -0.9* | 0 | 0 | -22.4* |
| `Level.cpp` | 0 | 0 | -65.4* | -42.0* | -40.0* | -40.0* | -37.6* | 0 | 0* | 0 | 0* | -16.1* | 0* | 0 | 0* | -22.6* |
| `Radar.cpp` | 0 | 0 | -69.0* | -53.4* | -47.6* | -41.9* | -49.9* | 0 | 0* | 0 | -4.7* | -15.6* | 0 | 0 | 0 | 0 |
| `MGame.cpp` | 0 | 0 | -77.1* | -36.6* | -53.9* | -52.9* | -14.0* | 0 | 0* | 0 | -5.5* | -22.9* | 0* | 0 | 0 | 0 |
| `Generator.cpp` | 0 | 0 | -75.7* | -52.6* | -57.9* | -56.2* | -47.4* | 0 | -0.3* | 0 | -33.9* | -39.5* | 0 | 0 | 0 | -19.2* |
| `HangarWindow.cpp` | 0 | 0 | -84.4* | -81.0* | -43.0* | -66.7* | -43.1* | 0 | 0 | 0 | 0 | -37.4* | 0 | 0 | 0 | -2.0* |
| `Status.cpp` | 0 | 0 | -84.2* | -80.7* | -84.3* | -84.4* | -58.4* | 0 | 0 | 0 | 0 | -63.8* | 0 | 0 | 0 | -79.2* |
| `Gun.cpp` | 0 | 0 | -90.9* | -56.0* | -31.6* | -31.8* | -30.6* | 0 | 0 | 0 | 0 | -45.8* | 0 | 0 | 0 | 0 |
| `PlayerEgo.cpp` | 0 | 0 | -89.3* | -57.8* | -66.8* | -28.6* | -24.3* | 0 | -0.3* | 0 | 0 | -32.4* | 0 | 0 | 0 | 0 |
| `SpaceLounge.cpp` | 0 | 0 | -87.8* | -38.9* | -17.7* | -18.3* | -17.5* | 0 | 0 | 0 | 0 | -31.3* | 0 | 0 | 0 | 0 |
| `Hud.cpp` | 0 | 0 | -88.9* | -38.0* | -43.5* | -47.4* | -34.7* | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | -4.1* |
| `MenuTouchWindow.cpp` | 0 | 0 | -93.6* | -62.1* | -53.4* | -53.6* | -53.4* | 0 | -14.9* | 0 | -7.2* | -49.2* | 0 | 0 | 0 | -27.5* |
| `ModStation.cpp` | 0 | 0 | -95.8* | -55.0* | -8.4* | -12.0* | -5.8* | 0 | 0 | 0 | 0 | -53.8* | -25.4* | 0 | 0 | 0 |
### Matrix B
| TU | `fno_omit_frame_pointer` | `fno_stack_protector` | `fstack_protector` | `mno_unaligned_access` | `mno_restrict_it` | `mrestrict_it` | `fno_math_errno` | `fno_honor_nans` | `ffp_contract_off` | `ffp_contract_fast` | `fno_delete_null_pointer_checks` | `fno_threadsafe_statics` | `fno_use_cxa_atexit` | `fno_sized_deallocation` | `fno_common` | `fvisibility_default` | `tu_specific` | `tu_specific_off` |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| `Layout.cpp` | 0 | 0 | 0 | 0 | 0 | -0.3* | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | · | · |
| `LevelScript.cpp` | 0 | -16.5* | -16.5* | 0 | 0 | -2.8* | 0 | -1.1* | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | · | · |
| `Level.cpp` | 0 | 0* | 0* | 0* | 0 | -2.8* | 0 | 0* | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | · | · |
| `Radar.cpp` | 0 | -6.8* | -6.8* | 0 | 0 | -5.3* | 0 | 0* | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | · | · |
| `MGame.cpp` | 0 | -10.7* | -10.7* | 0 | 0 | -10.5* | 0 | 0* | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | · | · |
| `Generator.cpp` | 0 | -21.2* | -21.2* | 0 | 0 | -9.1* | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | · | · |
| `HangarWindow.cpp` | 0 | -16.1* | -16.1* | 0 | 0 | -6.8* | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | · | · |
| `Status.cpp` | 0 | 0 | 0 | 0 | 0 | -28.8* | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | · | · |
| `Gun.cpp` | 0 | -24.7* | -24.7* | 0 | 0 | -7.1* | 0 | -1.1* | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | · | · |
| `PlayerEgo.cpp` | 0 | -30.3* | -30.3* | 0 | 0 | -3.7* | 0 | -0.3* | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | · | · |
| `SpaceLounge.cpp` | 0 | -20.1* | -20.1* | 0 | 0 | -1.8* | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | -0.2* |
| `Hud.cpp` | 0 | -32.5* | -32.5* | -8.3* | 0 | -15.4* | 0 | -2.5* | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | · | · |
| `MenuTouchWindow.cpp` | 0 | 0 | 0 | 0 | 0 | -15.3* | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | · | · |
| `ModStation.cpp` | 0 | -23.6* | -23.6* | 0 | 0 | -9.1* | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | · | · |

### Per-flag summary

| flag | TUs target bytes changed | TUs target size changed | TUs any function changed | max Δ | min Δ | cells with pct change |
|---|---:|---:|---:|---:|---:|---:|
| `base` | 0 | 0 | 0 | +0.0 | +0.0 | 0 |
| `Oz` | 0 | 0 | 0 | +0.0 | +0.0 | 0 |
| `O0` | 14 | 14 | 14 | -48.2 | -95.8 | 19 |
| `O1` | 14 | 14 | 14 | -36.6 | -81.0 | 19 |
| `O2` | 14 | 14 | 14 | -4.8 | -84.3 | 19 |
| `O3` | 14 | 14 | 14 | -12.0 | -84.4 | 19 |
| `Os` | 14 | 12 | 14 | -1.5 | -58.4 | 19 |
| `g` | 0 | 0 | 0 | +0.0 | +0.0 | 0 |
| `g0` | 6 | 1 | 12 | +0.0 | -14.9 | 3 |
| `g2` | 0 | 0 | 0 | +0.0 | +0.0 | 0 |
| `fno_strict_aliasing` | 6 | 6 | 13 | +0.0 | -33.9 | 7 |
| `fno_inline` | 12 | 11 | 14 | +0.0 | -63.8 | 16 |
| `fno_slp_vectorize` | 5 | 5 | 10 | +0.0 | -30.6 | 7 |
| `fno_vectorize` | 0 | 0 | 0 | +0.0 | +0.0 | 0 |
| `fno_unroll_loops` | 1 | 1 | 1 | +0.0 | -18.4 | 1 |
| `fno_jump_tables` | 7 | 7 | 12 | +0.0 | -79.2 | 9 |
| `fno_omit_frame_pointer` | 0 | 0 | 0 | +0.0 | +0.0 | 0 |
| `fno_stack_protector` | 11 | 11 | 14 | +0.0 | -32.5 | 14 |
| `fstack_protector` | 11 | 11 | 14 | +0.0 | -32.5 | 14 |
| `mno_unaligned_access` | 2 | 2 | 10 | +0.0 | -8.3 | 2 |
| `mno_restrict_it` | 0 | 0 | 0 | +0.0 | +0.0 | 0 |
| `mrestrict_it` | 14 | 12 | 14 | -0.3 | -28.8 | 19 |
| `fno_math_errno` | 0 | 0 | 0 | +0.0 | +0.0 | 0 |
| `fno_honor_nans` | 7 | 3 | 13 | +0.0 | -10.8 | 5 |
| `ffp_contract_off` | 0 | 0 | 0 | +0.0 | +0.0 | 0 |
| `ffp_contract_fast` | 0 | 0 | 0 | +0.0 | +0.0 | 0 |
| `fno_delete_null_pointer_checks` | 0 | 0 | 0 | +0.0 | +0.0 | 0 |
| `fno_threadsafe_statics` | 0 | 0 | 0 | +0.0 | +0.0 | 0 |
| `fno_use_cxa_atexit` | 0 | 0 | 0 | +0.0 | +0.0 | 0 |
| `fno_sized_deallocation` | 0 | 0 | 0 | +0.0 | +0.0 | 0 |
| `fno_common` | 0 | 0 | 0 | +0.0 | +0.0 | 0 |
| `fvisibility_default` | 0 | 0 | 0 | +0.0 | +0.0 | 0 |
| `tu_specific` | 0 | 0 | 0 | +0.0 | +0.0 | 0 |
| `tu_specific_off` | 1 | 0 | 1 | -0.2 | -0.2 | 1 |

### Per-TU summary

| TU | target(s) | baseline pct | baseline size | target-byte-changing flags | target-size-changing flags | any-function-changing flags | fully neutral flags | max Δ | min Δ |
|---|---|---:|---:|---|---|---|---|---:|---:|
| `src/game/ui/Layout.cpp` | `_ZN6LayoutC1Ev, _ZN6LayoutC2Ev` | 49.0 | 7072 | `O0`, `O1`, `O2`, `O3`, `Os`, `fno_slp_vectorize`, `mrestrict_it` | `O0`, `O1`, `O2`, `O3`, `Os`, `fno_slp_vectorize`, `mrestrict_it` | 13/32 | 19/32 | +0.0 | -48.2 |
| `src/game/world/LevelScript.cpp` | `_ZN11LevelScript7processEi` | 60.0 | 57232 | `O0`, `O1`, `O2`, `O3`, `Os`, `fno_strict_aliasing`, `fno_inline`, `fno_slp_vectorize`, `fno_jump_tables`, `fno_stack_protector`, `fstack_protector`, `mrestrict_it`, `fno_honor_nans` | `O0`, `O1`, `O2`, `O3`, `Os`, `fno_strict_aliasing`, `fno_inline`, `fno_slp_vectorize`, `fno_jump_tables`, `fno_stack_protector`, `fstack_protector`, `mrestrict_it`, `fno_honor_nans` | 14/32 | 18/32 | +0.0 | -57.6 |
| `src/game/world/Level.cpp` | `_ZN5Level10createShipEiiiP8Waypointbb, _ZN5Level13createMissionEv, _ZN5Level11createSceneEv` | 74.4 | 2764 | `O0`, `O1`, `O2`, `O3`, `Os`, `g0`, `fno_strict_aliasing`, `fno_inline`, `fno_slp_vectorize`, `fno_unroll_loops`, `fno_jump_tables`, `fno_stack_protector`, `fstack_protector`, `mno_unaligned_access`, `mrestrict_it`, `fno_honor_nans` | `O0`, `O1`, `O2`, `O3`, `Os`, `fno_strict_aliasing`, `fno_inline`, `fno_slp_vectorize`, `fno_unroll_loops`, `fno_jump_tables`, `fno_stack_protector`, `fstack_protector`, `mno_unaligned_access`, `mrestrict_it` | 16/32 | 16/32 | +0.0 | -89.7 |
| `src/game/weapons/Radar.cpp` | `_ZN5Radar4drawEP6PlayerP3Hudi` | 74.5 | 9908 | `O0`, `O1`, `O2`, `O3`, `Os`, `g0`, `fno_strict_aliasing`, `fno_inline`, `fno_stack_protector`, `fstack_protector`, `mrestrict_it`, `fno_honor_nans` | `O0`, `O1`, `O2`, `O3`, `Os`, `fno_strict_aliasing`, `fno_inline`, `fno_stack_protector`, `fstack_protector`, `mrestrict_it` | 14/32 | 18/32 | +0.0 | -69.0 |
| `src/game/menu/MGame.cpp` | `_ZN5MGame10OnTouchEndEiiPv, _ZN5MGame8OnUpdateEv` | 81.6 | 8080 | `O0`, `O1`, `O2`, `O3`, `Os`, `g0`, `fno_strict_aliasing`, `fno_inline`, `fno_slp_vectorize`, `fno_stack_protector`, `fstack_protector`, `mrestrict_it`, `fno_honor_nans` | `O0`, `O1`, `O2`, `O3`, `Os`, `fno_strict_aliasing`, `fno_inline`, `fno_slp_vectorize`, `fno_stack_protector`, `fstack_protector`, `mrestrict_it`, `fno_honor_nans` | 15/32 | 17/32 | +0.0 | -93.2 |
| `src/game/mission/Generator.cpp` | `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 84.7 | 1996 | `O0`, `O1`, `O2`, `O3`, `Os`, `g0`, `fno_strict_aliasing`, `fno_inline`, `fno_jump_tables`, `fno_stack_protector`, `fstack_protector`, `mrestrict_it` | `O0`, `O1`, `O2`, `O3`, `Os`, `fno_strict_aliasing`, `fno_inline`, `fno_jump_tables`, `fno_stack_protector`, `fstack_protector`, `mrestrict_it` | 13/32 | 19/32 | +0.0 | -75.7 |
| `src/game/ui/HangarWindow.cpp` | `_ZN12HangarWindow6renderEv` | 89.7 | 9700 | `O0`, `O1`, `O2`, `O3`, `Os`, `fno_inline`, `fno_jump_tables`, `fno_stack_protector`, `fstack_protector`, `mrestrict_it` | `O0`, `O1`, `O2`, `O3`, `Os`, `fno_inline`, `fno_jump_tables`, `fno_stack_protector`, `fstack_protector`, `mrestrict_it` | 13/32 | 19/32 | +0.0 | -84.4 |
| `src/game/mission/Status.cpp` | `_ZN6Status16missionCompletedEbbx` | 92.5 | 1432 | `O0`, `O1`, `O2`, `O3`, `Os`, `fno_inline`, `fno_jump_tables`, `mrestrict_it` | `O0`, `O1`, `O2`, `O3`, `Os`, `fno_inline`, `fno_jump_tables`, `mrestrict_it` | 14/32 | 18/32 | +0.0 | -84.4 |
| `src/game/weapons/Gun.cpp` | `_ZN3Gun22calcCharacterCollisionEv` | 93.4 | 1984 | `O0`, `O1`, `O2`, `O3`, `Os`, `fno_inline`, `fno_stack_protector`, `fstack_protector`, `mrestrict_it`, `fno_honor_nans` | `O0`, `O1`, `O2`, `O3`, `fno_stack_protector`, `fstack_protector`, `mrestrict_it` | 11/32 | 21/32 | +0.0 | -90.9 |
| `src/game/ship/PlayerEgo.cpp` | `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` | 95.5 | 968 | `O0`, `O1`, `O2`, `O3`, `Os`, `g0`, `fno_inline`, `fno_stack_protector`, `fstack_protector`, `mrestrict_it`, `fno_honor_nans` | `O0`, `O1`, `O2`, `O3`, `Os`, `fno_inline`, `fno_stack_protector`, `fstack_protector` | 15/32 | 17/32 | +0.0 | -89.3 |
| `src/game/menu/SpaceLounge.cpp` | `_ZN11SpaceLounge10drawLoungeEv` | 95.7 | 1948 | `O0`, `O1`, `O2`, `O3`, `Os`, `fno_inline`, `fno_stack_protector`, `fstack_protector`, `mrestrict_it`, `tu_specific_off` | `O0`, `O1`, `O2`, `O3`, `Os`, `fno_inline`, `fno_stack_protector`, `fstack_protector` | 15/32 | 19/32 | +0.0 | -87.8 |
| `src/game/ui/Hud.cpp` | `_ZN3Hud4drawExxP9PlayerEgobjj` | 96.0 | 9332 | `O0`, `O1`, `O2`, `O3`, `Os`, `fno_jump_tables`, `fno_stack_protector`, `fstack_protector`, `mno_unaligned_access`, `mrestrict_it`, `fno_honor_nans` | `O0`, `O1`, `O2`, `O3`, `Os`, `fno_jump_tables`, `fno_stack_protector`, `fstack_protector`, `mno_unaligned_access`, `mrestrict_it`, `fno_honor_nans` | 15/32 | 17/32 | +0.0 | -88.9 |
| `src/game/ui/MenuTouchWindow.cpp` | `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 98.1 | 1896 | `O0`, `O1`, `O2`, `O3`, `Os`, `g0`, `fno_strict_aliasing`, `fno_inline`, `fno_jump_tables`, `mrestrict_it` | `O0`, `O1`, `O2`, `O3`, `Os`, `g0`, `fno_strict_aliasing`, `fno_inline`, `fno_jump_tables`, `mrestrict_it` | 15/32 | 17/32 | +0.0 | -93.6 |
| `src/game/menu/ModStation.cpp` | `_ZN10ModStationC1Ev, _ZN10ModStationC2Ev` | 99.2 | 1008 | `O0`, `O1`, `O2`, `O3`, `Os`, `fno_inline`, `fno_slp_vectorize`, `fno_stack_protector`, `fstack_protector`, `mrestrict_it` | `O0`, `O1`, `O2`, `O3`, `fno_inline`, `fno_slp_vectorize`, `fno_stack_protector`, `fstack_protector`, `mrestrict_it` | 15/32 | 17/32 | +0.0 | -95.8 |

## 4. Exact object fingerprints

Baseline target sizes and SHA-1s. Every cell not listed in the next table has the
same target object bytes, size and SHA-1 as baseline, and therefore the same
target percentage. The next table lists every cell whose target object bytes
changed, with the exact variant size, SHA-1, differing-byte count (when the size is
unchanged) and target percentage.

### Baseline target objects

| function | TU | size | SHA-1 | pct |
|---|---|---:|---|---:|
| `_ZN6LayoutC1Ev` | `src/game/ui/Layout.cpp` | 7072 | `17404233cfb1d597ec264d859b36eb17c9047e89` | 49.0 |
| `_ZN6LayoutC2Ev` | `src/game/ui/Layout.cpp` | 7072 | `17404233cfb1d597ec264d859b36eb17c9047e89` | 49.0 |
| `_ZN11LevelScript7processEi` | `src/game/world/LevelScript.cpp` | 57232 | `5fba859242841984b202e2243c6755523bb40e0e` | 60.0 |
| `_ZN5Level10createShipEiiiP8Waypointbb` | `src/game/world/Level.cpp` | 2764 | `428c9e8bba83f37a4060b9072475037152bcb4e7` | 74.4 |
| `_ZN5Level13createMissionEv` | `src/game/world/Level.cpp` | 17172 | `7669c5504ed93002459d5671b9da4434f5395e8b` | 92.3 |
| `_ZN5Level11createSceneEv` | `src/game/world/Level.cpp` | 2252 | `df59bfb9cf831683005fce9b361bd530710132e4` | 92.8 |
| `_ZN5Radar4drawEP6PlayerP3Hudi` | `src/game/weapons/Radar.cpp` | 9908 | `c0eec29fe3090517f60e7a299467f26acb74e72f` | 74.5 |
| `_ZN5MGame10OnTouchEndEiiPv` | `src/game/menu/MGame.cpp` | 8080 | `b00e46ec653962314c12e2aeb7f0f344ecdd51b6` | 81.6 |
| `_ZN5MGame8OnUpdateEv` | `src/game/menu/MGame.cpp` | 11440 | `9ead1b62d638b54062dc7d00cc6b2798301ff399` | 99.9 |
| `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | `src/game/mission/Generator.cpp` | 1996 | `b25205768cc0d3182d49fe17772387740c2b9120` | 84.7 |
| `_ZN12HangarWindow6renderEv` | `src/game/ui/HangarWindow.cpp` | 9700 | `2e1ad5ba67eb37440df104d321e346129d86d4f8` | 89.7 |
| `_ZN6Status16missionCompletedEbbx` | `src/game/mission/Status.cpp` | 1432 | `8952a5c28b15d75241124024b8186ba212e89555` | 92.5 |
| `_ZN3Gun22calcCharacterCollisionEv` | `src/game/weapons/Gun.cpp` | 1984 | `59e9519e355b2804aa3a796f06fcccd6e6be1f56` | 93.4 |
| `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` | `src/game/ship/PlayerEgo.cpp` | 968 | `f11bb63b0888f296412cdf1c610ae81b3f4e2901` | 95.5 |
| `_ZN11SpaceLounge10drawLoungeEv` | `src/game/menu/SpaceLounge.cpp` | 1948 | `ac32746019b1aee9e16043707705dc2915cc1413` | 95.7 |
| `_ZN3Hud4drawExxP9PlayerEgobjj` | `src/game/ui/Hud.cpp` | 9332 | `004df4affb13a087ecd944e9ed37b9f85e3720cc` | 96.0 |
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | `src/game/ui/MenuTouchWindow.cpp` | 1896 | `8e098545c530bf40acd45226ea67d020295608fc` | 98.1 |
| `_ZN10ModStationC1Ev` | `src/game/menu/ModStation.cpp` | 1008 | `880f94a89f21ce7f0cd8810b6e059c0cf0724125` | 99.2 |
| `_ZN10ModStationC2Ev` | `src/game/menu/ModStation.cpp` | 1008 | `880f94a89f21ce7f0cd8810b6e059c0cf0724125` | 99.2 |

### Every cell that changes target object bytes

| TU | flag | function | size | SHA-1 | diff bytes | pct | Δ |
|---|---|---|---:|---|---:|---:|---:|
| `src/game/ui/Layout.cpp` | `O0` | `_ZN6LayoutC1Ev` | 24024 | `df3b1de5eddeec35464d3c58c7ca029bf0623eb4` | size | 0.8 | -48.2 |
| `src/game/ui/Layout.cpp` | `O0` | `_ZN6LayoutC2Ev` | 24024 | `df3b1de5eddeec35464d3c58c7ca029bf0623eb4` | size | 0.8 | -48.2 |
| `src/game/ui/Layout.cpp` | `O1` | `_ZN6LayoutC1Ev` | 8020 | `da5f4c69580ba08dc7f14cdb615ae2425c9cf694` | size | 2.4 | -46.6 |
| `src/game/ui/Layout.cpp` | `O1` | `_ZN6LayoutC2Ev` | 8020 | `da5f4c69580ba08dc7f14cdb615ae2425c9cf694` | size | 2.4 | -46.6 |
| `src/game/ui/Layout.cpp` | `O2` | `_ZN6LayoutC1Ev` | 7120 | `096c6c8cca2fc8e3fa63bd06d1c5220fdbba6a24` | size | 44.2 | -4.8 |
| `src/game/ui/Layout.cpp` | `O2` | `_ZN6LayoutC2Ev` | 7120 | `096c6c8cca2fc8e3fa63bd06d1c5220fdbba6a24` | size | 44.2 | -4.8 |
| `src/game/ui/Layout.cpp` | `O3` | `_ZN6LayoutC1Ev` | 7276 | `da2d5de9b8461f27a90fe8cc53d4cdce5b2a83d9` | size | 23.1 | -25.9 |
| `src/game/ui/Layout.cpp` | `O3` | `_ZN6LayoutC2Ev` | 7276 | `da2d5de9b8461f27a90fe8cc53d4cdce5b2a83d9` | size | 23.1 | -25.9 |
| `src/game/ui/Layout.cpp` | `Os` | `_ZN6LayoutC1Ev` | 7080 | `5ca69ab877fe7da4b95a4cb1d0223ff04dbbb799` | size | 47.5 | -1.5 |
| `src/game/ui/Layout.cpp` | `Os` | `_ZN6LayoutC2Ev` | 7080 | `5ca69ab877fe7da4b95a4cb1d0223ff04dbbb799` | size | 47.5 | -1.5 |
| `src/game/ui/Layout.cpp` | `fno_slp_vectorize` | `_ZN6LayoutC1Ev` | 5992 | `b0174bef0ef3170bbbb199475db0ae35adeb1ccb` | size | 18.4 | -30.6 |
| `src/game/ui/Layout.cpp` | `fno_slp_vectorize` | `_ZN6LayoutC2Ev` | 5992 | `b0174bef0ef3170bbbb199475db0ae35adeb1ccb` | size | 18.4 | -30.6 |
| `src/game/ui/Layout.cpp` | `mrestrict_it` | `_ZN6LayoutC1Ev` | 7032 | `4c8c41ca650a9c472ff96a92ec57de85b9c557f3` | size | 48.7 | -0.3 |
| `src/game/ui/Layout.cpp` | `mrestrict_it` | `_ZN6LayoutC2Ev` | 7032 | `4c8c41ca650a9c472ff96a92ec57de85b9c557f3` | size | 48.7 | -0.3 |
| `src/game/world/LevelScript.cpp` | `O0` | `_ZN11LevelScript7processEi` | 132904 | `fa3ef8b9dde1ba9d349be359a971251dbb50d684` | size | 2.4 | -57.6 |
| `src/game/world/LevelScript.cpp` | `O1` | `_ZN11LevelScript7processEi` | 61732 | `424bd588bc5f9954f4dc0b2a351d269236053724` | size | 7.1 | -52.9 |
| `src/game/world/LevelScript.cpp` | `O2` | `_ZN11LevelScript7processEi` | 63380 | `51d96bff6abe0cb17de47dc4085192baf172e7a1` | size | 26.2 | -33.8 |
| `src/game/world/LevelScript.cpp` | `O3` | `_ZN11LevelScript7processEi` | 66592 | `673592f611885a596ad379fe17a64b7381d6f1f2` | size | 14.3 | -45.7 |
| `src/game/world/LevelScript.cpp` | `Os` | `_ZN11LevelScript7processEi` | 57356 | `6bea3d23715e34e76b2858bee49e68b803788ff1` | size | 28.3 | -31.7 |
| `src/game/world/LevelScript.cpp` | `fno_strict_aliasing` | `_ZN11LevelScript7processEi` | 57316 | `91787fe4419366cfbb022ad508ad6f5b992a8d65` | size | 58.0 | -2.0 |
| `src/game/world/LevelScript.cpp` | `fno_inline` | `_ZN11LevelScript7processEi` | 61236 | `b8515ca735fb6b26e5d9fa51e800e312493761b5` | size | 32.7 | -27.3 |
| `src/game/world/LevelScript.cpp` | `fno_slp_vectorize` | `_ZN11LevelScript7processEi` | 57196 | `4a3a5c572ec372da432b84ea69518f8fc17d58f6` | size | 59.1 | -0.9 |
| `src/game/world/LevelScript.cpp` | `fno_jump_tables` | `_ZN11LevelScript7processEi` | 57480 | `d31a21ba792eb8438e8cea59876ac95c79daa0a7` | size | 37.6 | -22.4 |
| `src/game/world/LevelScript.cpp` | `fno_stack_protector` | `_ZN11LevelScript7processEi` | 57344 | `6fcb7a799bca1301c6a26a85b216068d05bcb505` | size | 43.5 | -16.5 |
| `src/game/world/LevelScript.cpp` | `fstack_protector` | `_ZN11LevelScript7processEi` | 57344 | `6fcb7a799bca1301c6a26a85b216068d05bcb505` | size | 43.5 | -16.5 |
| `src/game/world/LevelScript.cpp` | `mrestrict_it` | `_ZN11LevelScript7processEi` | 57208 | `7803d14e4b0bae93da9af173d726104e017c5bbd` | size | 57.2 | -2.8 |
| `src/game/world/LevelScript.cpp` | `fno_honor_nans` | `_ZN11LevelScript7processEi` | 57124 | `31cfffd4f194b6dfb33b5dab8a5f584675c883c7` | size | 58.9 | -1.1 |
| `src/game/world/Level.cpp` | `O0` | `_ZN5Level10createShipEiiiP8Waypointbb` | 4640 | `7675dcded691667fe0a643705c2253e170cd3614` | size | 9.0 | -65.4 |
| `src/game/world/Level.cpp` | `O0` | `_ZN5Level13createMissionEv` | 32668 | `d4c0e4b0c42bfc6814e0aaa291131ba95d3bcca3` | size | 5.3 | -87.0 |
| `src/game/world/Level.cpp` | `O0` | `_ZN5Level11createSceneEv` | 3528 | `3cf4806f8bd5ed8458e66706aa0d5eb4fa90077f` | size | 3.1 | -89.7 |
| `src/game/world/Level.cpp` | `O1` | `_ZN5Level10createShipEiiiP8Waypointbb` | 3064 | `a85a6ff873e884892319334cf7c4e21cd29e64b8` | size | 19.6 | -54.8 |
| `src/game/world/Level.cpp` | `O1` | `_ZN5Level13createMissionEv` | 18744 | `e248990fbff6b664db2dcebf5bb278e33314729e` | size | 22.5 | -69.8 |
| `src/game/world/Level.cpp` | `O1` | `_ZN5Level11createSceneEv` | 2316 | `55f9d0e0fcf2556e1bb06d64a10c2087b34b45c8` | size | 50.8 | -42.0 |
| `src/game/world/Level.cpp` | `O2` | `_ZN5Level10createShipEiiiP8Waypointbb` | 3076 | `5ecc235f0b852e7218f847ed1133d41bf9a07a1e` | size | 34.4 | -40.0 |
| `src/game/world/Level.cpp` | `O2` | `_ZN5Level13createMissionEv` | 19244 | `7fa718c8d63830531775d2be7d67ad2390cd910b` | size | 13.3 | -79.0 |
| `src/game/world/Level.cpp` | `O2` | `_ZN5Level11createSceneEv` | 2448 | `5cc26406658e5223c5b806ff3dd93ab03f588f37` | size | 49.6 | -43.2 |
| `src/game/world/Level.cpp` | `O3` | `_ZN5Level10createShipEiiiP8Waypointbb` | 3084 | `6e55f8bb7cc3e56f56a7d307e31356b43b812024` | size | 34.4 | -40.0 |
| `src/game/world/Level.cpp` | `O3` | `_ZN5Level13createMissionEv` | 20276 | `099bf12821928b8ac049fdcbde87e80bc3a95716` | size | 30.0 | -62.3 |
| `src/game/world/Level.cpp` | `O3` | `_ZN5Level11createSceneEv` | 3728 | `844171ffb37f1e385b38f418bd744d02656299ca` | size | 37.1 | -55.7 |
| `src/game/world/Level.cpp` | `Os` | `_ZN5Level10createShipEiiiP8Waypointbb` | 2976 | `8b1576b98a20fff40faf990173d49b542e2b07bb` | size | 36.8 | -37.6 |
| `src/game/world/Level.cpp` | `Os` | `_ZN5Level13createMissionEv` | 18160 | `3380cf6cb90af18ded7c6d7af3c112aed56a830d` | size | 40.3 | -52.0 |
| `src/game/world/Level.cpp` | `Os` | `_ZN5Level11createSceneEv` | 2352 | `dee258ac72f324c9721484b793ac0367c7a6f6b0` | size | 53.5 | -39.3 |
| `src/game/world/Level.cpp` | `g0` | `_ZN5Level13createMissionEv` | 17172 | `01c3d8d585a0a2949ef17791a82f5d0fc82a47c4` | 7 | 92.3 | +0.0 |
| `src/game/world/Level.cpp` | `fno_strict_aliasing` | `_ZN5Level13createMissionEv` | 17292 | `100bd3c1bf0f0423b285f5c1afa76e4fe73d5efc` | size | 69.0 | -23.3 |
| `src/game/world/Level.cpp` | `fno_inline` | `_ZN5Level10createShipEiiiP8Waypointbb` | 2864 | `dfad130a18f349fed2ecb04aa4f0c89d865fbec9` | size | 58.3 | -16.1 |
| `src/game/world/Level.cpp` | `fno_inline` | `_ZN5Level13createMissionEv` | 18356 | `1fe2378434476f58aea665882b008cffd34a485d` | size | 41.6 | -50.7 |
| `src/game/world/Level.cpp` | `fno_inline` | `_ZN5Level11createSceneEv` | 2328 | `73a061bac779a1ca2c83105cdea1ea93ed1ab2ee` | size | 64.4 | -28.4 |
| `src/game/world/Level.cpp` | `fno_slp_vectorize` | `_ZN5Level13createMissionEv` | 17116 | `8ba7170f7a129fe5e1efbf481949a023d4105295` | size | 88.0 | -4.3 |
| `src/game/world/Level.cpp` | `fno_unroll_loops` | `_ZN5Level11createSceneEv` | 2236 | `274c2f4c42d72df407557bdb371c367498ad3784` | size | 74.4 | -18.4 |
| `src/game/world/Level.cpp` | `fno_jump_tables` | `_ZN5Level10createShipEiiiP8Waypointbb` | 2760 | `3731966e40d4b5ecefbeec60b199bf64ae92e7b8` | size | 39.4 | -35.0 |
| `src/game/world/Level.cpp` | `fno_jump_tables` | `_ZN5Level13createMissionEv` | 17324 | `1f38a138d64f90d110a432d7a9375bd1aed573ce` | size | 63.4 | -28.9 |
| `src/game/world/Level.cpp` | `fno_jump_tables` | `_ZN5Level11createSceneEv` | 2252 | `a7311ea2c3dcc83a92b3b8f33736c3f4bb915ade` | 1984 | 70.2 | -22.6 |
| `src/game/world/Level.cpp` | `fno_stack_protector` | `_ZN5Level13createMissionEv` | 17172 | `68148f970c738b81a3c0655963a316d493b0814b` | 14674 | 69.4 | -22.9 |
| `src/game/world/Level.cpp` | `fno_stack_protector` | `_ZN5Level11createSceneEv` | 2180 | `dc3cd9d9401670e0173ba430cd0d0f29b6fa3fb5` | size | 71.7 | -21.1 |
| `src/game/world/Level.cpp` | `fstack_protector` | `_ZN5Level13createMissionEv` | 17172 | `68148f970c738b81a3c0655963a316d493b0814b` | 14674 | 69.4 | -22.9 |
| `src/game/world/Level.cpp` | `fstack_protector` | `_ZN5Level11createSceneEv` | 2180 | `dc3cd9d9401670e0173ba430cd0d0f29b6fa3fb5` | size | 71.7 | -21.1 |
| `src/game/world/Level.cpp` | `mno_unaligned_access` | `_ZN5Level13createMissionEv` | 17180 | `38b2c4393c58ad65670edc2cbf5070fcb999a5c7` | size | 88.9 | -3.4 |
| `src/game/world/Level.cpp` | `mrestrict_it` | `_ZN5Level10createShipEiiiP8Waypointbb` | 2760 | `52ea04c1cb7338d571a63a5f21e24c68644abee6` | size | 71.6 | -2.8 |
| `src/game/world/Level.cpp` | `mrestrict_it` | `_ZN5Level13createMissionEv` | 17156 | `dbfc9f7fbae0fcaa0a0d697d9cfb9456a2257cd6` | size | 86.8 | -5.5 |
| `src/game/world/Level.cpp` | `mrestrict_it` | `_ZN5Level11createSceneEv` | 2244 | `c54d4b6e9badb358da89fe70e1cc11ceb0737caf` | size | 81.4 | -11.4 |
| `src/game/world/Level.cpp` | `fno_honor_nans` | `_ZN5Level13createMissionEv` | 17172 | `700ccf007178f6404921eb34ec094a26d5b0cd6c` | 2 | 92.3 | +0.0 |
| `src/game/weapons/Radar.cpp` | `O0` | `_ZN5Radar4drawEP6PlayerP3Hudi` | 17604 | `8dfc7b9153d4d80302764e9342cc7f6e11bcaad7` | size | 5.5 | -69.0 |
| `src/game/weapons/Radar.cpp` | `O1` | `_ZN5Radar4drawEP6PlayerP3Hudi` | 10536 | `d9752d90be31671bec75dc5db2ef0f3beb57dec9` | size | 21.1 | -53.4 |
| `src/game/weapons/Radar.cpp` | `O2` | `_ZN5Radar4drawEP6PlayerP3Hudi` | 10320 | `90eb244531d210c559ad1c83b84dd9ac73686dcd` | size | 26.9 | -47.6 |
| `src/game/weapons/Radar.cpp` | `O3` | `_ZN5Radar4drawEP6PlayerP3Hudi` | 10744 | `b7dce02e9f75729c689753e68e0f8031f341ed29` | size | 32.6 | -41.9 |
| `src/game/weapons/Radar.cpp` | `Os` | `_ZN5Radar4drawEP6PlayerP3Hudi` | 10244 | `d0c1ffc19cc5a753630c7362ad800ca9c8265e89` | size | 24.6 | -49.9 |
| `src/game/weapons/Radar.cpp` | `g0` | `_ZN5Radar4drawEP6PlayerP3Hudi` | 9908 | `0683240b15df9a9292099a99d03b62af65a5d5f5` | 13 | 74.5 | +0.0 |
| `src/game/weapons/Radar.cpp` | `fno_strict_aliasing` | `_ZN5Radar4drawEP6PlayerP3Hudi` | 9948 | `a57b717bdda17cdb8615d181f98f4e6c254ac64e` | size | 69.8 | -4.7 |
| `src/game/weapons/Radar.cpp` | `fno_inline` | `_ZN5Radar4drawEP6PlayerP3Hudi` | 10036 | `46440cdda9ee474653d5252d88157a664a2a0ace` | size | 58.9 | -15.6 |
| `src/game/weapons/Radar.cpp` | `fno_stack_protector` | `_ZN5Radar4drawEP6PlayerP3Hudi` | 9820 | `b612010284156e913277425280e004d9d969334b` | size | 67.7 | -6.8 |
| `src/game/weapons/Radar.cpp` | `fstack_protector` | `_ZN5Radar4drawEP6PlayerP3Hudi` | 9820 | `b612010284156e913277425280e004d9d969334b` | size | 67.7 | -6.8 |
| `src/game/weapons/Radar.cpp` | `mrestrict_it` | `_ZN5Radar4drawEP6PlayerP3Hudi` | 9940 | `cee20222a25ddf9662e3bac670a266b074ff991d` | size | 69.2 | -5.3 |
| `src/game/weapons/Radar.cpp` | `fno_honor_nans` | `_ZN5Radar4drawEP6PlayerP3Hudi` | 9908 | `946710aec3a580360e8f42a2bfc5fd4e7cc8c9d2` | 13 | 74.5 | +0.0 |
| `src/game/menu/MGame.cpp` | `O0` | `_ZN5MGame10OnTouchEndEiiPv` | 12068 | `8d5c68e8b98ee7a8a4e26705e8aa4a8c2db4c9a3` | size | 4.5 | -77.1 |
| `src/game/menu/MGame.cpp` | `O0` | `_ZN5MGame8OnUpdateEv` | 20096 | `7c0d23e0dd65ceed4460ac4c69876294b1988f4e` | size | 6.7 | -93.2 |
| `src/game/menu/MGame.cpp` | `O1` | `_ZN5MGame10OnTouchEndEiiPv` | 8464 | `16b0ed191b51ae62efba154ae2f4bf42360c57e6` | size | 45.0 | -36.6 |
| `src/game/menu/MGame.cpp` | `O1` | `_ZN5MGame8OnUpdateEv` | 12024 | `d0c5352a68585808d1a4a70276de38fe109b98fd` | size | 32.1 | -67.8 |
| `src/game/menu/MGame.cpp` | `O2` | `_ZN5MGame10OnTouchEndEiiPv` | 9700 | `880e557a1e5bfc91062f25d49cb80eb9eb231c04` | size | 27.7 | -53.9 |
| `src/game/menu/MGame.cpp` | `O2` | `_ZN5MGame8OnUpdateEv` | 14328 | `0c1eda879f61f72d968113e7ff6652715c2a2fe3` | size | 29.1 | -70.8 |
| `src/game/menu/MGame.cpp` | `O3` | `_ZN5MGame10OnTouchEndEiiPv` | 9908 | `4d588444541a7956d4f8e39fffadf5c863b7c9e5` | size | 28.7 | -52.9 |
| `src/game/menu/MGame.cpp` | `O3` | `_ZN5MGame8OnUpdateEv` | 14412 | `2d2c4510e8596b18c7fd853d2f731ab672de2657` | size | 28.8 | -71.1 |
| `src/game/menu/MGame.cpp` | `Os` | `_ZN5MGame10OnTouchEndEiiPv` | 8284 | `7ede114e73a03234c1012324bbc2cbcbd036a01c` | size | 67.6 | -14.0 |
| `src/game/menu/MGame.cpp` | `Os` | `_ZN5MGame8OnUpdateEv` | 11504 | `e86fbeb3aad7c7cac5c247302fcc28ee7a72c4e5` | size | 63.5 | -36.4 |
| `src/game/menu/MGame.cpp` | `g0` | `_ZN5MGame10OnTouchEndEiiPv` | 8080 | `23c6f43734fd43dc538216a8c38307c8736f10dd` | 37 | 81.6 | +0.0 |
| `src/game/menu/MGame.cpp` | `g0` | `_ZN5MGame8OnUpdateEv` | 11440 | `ee3905e1c8179563fcc066e281cf50235c87257d` | 14 | 99.9 | +0.0 |
| `src/game/menu/MGame.cpp` | `fno_strict_aliasing` | `_ZN5MGame10OnTouchEndEiiPv` | 8104 | `0cda55b7f469aeca57333ed8f9a9d2aeb7e3f529` | size | 76.1 | -5.5 |
| `src/game/menu/MGame.cpp` | `fno_strict_aliasing` | `_ZN5MGame8OnUpdateEv` | 11448 | `a8b99ace0f098cde43dd1a13d648d1f616fc67e3` | size | 88.9 | -11.0 |
| `src/game/menu/MGame.cpp` | `fno_inline` | `_ZN5MGame10OnTouchEndEiiPv` | 8364 | `8f0d1c4eff8cccb17630a60a747ada346696d56f` | size | 57.9 | -23.7 |
| `src/game/menu/MGame.cpp` | `fno_inline` | `_ZN5MGame8OnUpdateEv` | 11524 | `5ad84551be72e5fbd502fc8b0720ef28e130ba93` | size | 77.0 | -22.9 |
| `src/game/menu/MGame.cpp` | `fno_slp_vectorize` | `_ZN5MGame8OnUpdateEv` | 11436 | `21d0b2d5f172ae9740a62be027e325e89a8b531f` | size | 87.6 | -12.3 |
| `src/game/menu/MGame.cpp` | `fno_stack_protector` | `_ZN5MGame10OnTouchEndEiiPv` | 7924 | `4c9a2f5b24573d12b628506a920133ab852fef7e` | size | 70.9 | -10.7 |
| `src/game/menu/MGame.cpp` | `fno_stack_protector` | `_ZN5MGame8OnUpdateEv` | 11404 | `0b15bdf5a962ead9ec44af65edf203f11b480bc1` | size | 80.4 | -19.5 |
| `src/game/menu/MGame.cpp` | `fstack_protector` | `_ZN5MGame10OnTouchEndEiiPv` | 7924 | `4c9a2f5b24573d12b628506a920133ab852fef7e` | size | 70.9 | -10.7 |
| `src/game/menu/MGame.cpp` | `fstack_protector` | `_ZN5MGame8OnUpdateEv` | 11404 | `0b15bdf5a962ead9ec44af65edf203f11b480bc1` | size | 80.4 | -19.5 |
| `src/game/menu/MGame.cpp` | `mrestrict_it` | `_ZN5MGame10OnTouchEndEiiPv` | 8072 | `3864b599589047223cdfa6a6bbaed8657bd42a3a` | size | 71.1 | -10.5 |
| `src/game/menu/MGame.cpp` | `mrestrict_it` | `_ZN5MGame8OnUpdateEv` | 11412 | `8c4622603078ff84997dafc1fcdf24473014f756` | size | 88.7 | -11.2 |
| `src/game/menu/MGame.cpp` | `fno_honor_nans` | `_ZN5MGame8OnUpdateEv` | 11428 | `ed40ccedb573b6a2f7d1b8f8a9363b46569cea3b` | size | 89.1 | -10.8 |
| `src/game/mission/Generator.cpp` | `O0` | `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 3480 | `8ae8c83c148d9be7fffd8e511831f3995b5567af` | size | 9.0 | -75.7 |
| `src/game/mission/Generator.cpp` | `O1` | `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 2240 | `9c921f76d74b348ceecbaac433a9384a99f12d9a` | size | 32.1 | -52.6 |
| `src/game/mission/Generator.cpp` | `O2` | `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 2176 | `9c7c2ac36866bf3422cfafcddecbd4bbb1c0fc2b` | size | 26.8 | -57.9 |
| `src/game/mission/Generator.cpp` | `O3` | `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 2556 | `8df7b1d3335572edf1aefcd5e58c56967326b3e5` | size | 28.5 | -56.2 |
| `src/game/mission/Generator.cpp` | `Os` | `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 2088 | `fd6b8600eca554db07186c1fd5daf1462fbb06e6` | size | 37.3 | -47.4 |
| `src/game/mission/Generator.cpp` | `g0` | `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 1996 | `6b4405221b11c7940a5e4b8096a6b36c3715e33d` | 21 | 84.4 | -0.3 |
| `src/game/mission/Generator.cpp` | `fno_strict_aliasing` | `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 1992 | `270abacdb299a1322c3174ee45f517f36721b3ac` | size | 50.8 | -33.9 |
| `src/game/mission/Generator.cpp` | `fno_inline` | `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 2096 | `37be40dc0c43ab5fd029171d78b25d790fbe1b97` | size | 45.2 | -39.5 |
| `src/game/mission/Generator.cpp` | `fno_jump_tables` | `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 1992 | `c6b426aeac3ff0f9fbf75a52af2125588418847f` | size | 65.5 | -19.2 |
| `src/game/mission/Generator.cpp` | `fno_stack_protector` | `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 1976 | `6d22bcd51cc9961c5e71e1918064f2186b325bcd` | size | 63.5 | -21.2 |
| `src/game/mission/Generator.cpp` | `fstack_protector` | `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 1976 | `6d22bcd51cc9961c5e71e1918064f2186b325bcd` | size | 63.5 | -21.2 |
| `src/game/mission/Generator.cpp` | `mrestrict_it` | `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 1992 | `ffb34160395efe1bf4e13929f7842d6b2a8e1bbb` | size | 75.6 | -9.1 |
| `src/game/ui/HangarWindow.cpp` | `O0` | `_ZN12HangarWindow6renderEv` | 17204 | `10f3bc7e3ae7e93ed7e809f8004232325e0381e9` | size | 5.3 | -84.4 |
| `src/game/ui/HangarWindow.cpp` | `O1` | `_ZN12HangarWindow6renderEv` | 9920 | `9a6551cd6d34ea1b089374b98e000530e1cf9f5a` | size | 8.7 | -81.0 |
| `src/game/ui/HangarWindow.cpp` | `O2` | `_ZN12HangarWindow6renderEv` | 9916 | `0e06fc3bd92885938f033aefd7dac704cc9726d7` | size | 46.7 | -43.0 |
| `src/game/ui/HangarWindow.cpp` | `O3` | `_ZN12HangarWindow6renderEv` | 10164 | `539dee9f39a16b38aa7adb1fc07af2aa1c8cabe6` | size | 23.0 | -66.7 |
| `src/game/ui/HangarWindow.cpp` | `Os` | `_ZN12HangarWindow6renderEv` | 9836 | `bf5f642b07cf61509ae50cec9dcfa2c599b84c88` | size | 46.6 | -43.1 |
| `src/game/ui/HangarWindow.cpp` | `fno_inline` | `_ZN12HangarWindow6renderEv` | 9940 | `cc2f514a200f655de7594b31395d53c40def1b12` | size | 52.3 | -37.4 |
| `src/game/ui/HangarWindow.cpp` | `fno_jump_tables` | `_ZN12HangarWindow6renderEv` | 9728 | `aad8caaea8c26488c09a76a7b535bb66005534da` | size | 87.7 | -2.0 |
| `src/game/ui/HangarWindow.cpp` | `fno_stack_protector` | `_ZN12HangarWindow6renderEv` | 9668 | `5c4ac51eb430f31014041b5f44d4c06bb533cc4c` | size | 73.6 | -16.1 |
| `src/game/ui/HangarWindow.cpp` | `fstack_protector` | `_ZN12HangarWindow6renderEv` | 9668 | `5c4ac51eb430f31014041b5f44d4c06bb533cc4c` | size | 73.6 | -16.1 |
| `src/game/ui/HangarWindow.cpp` | `mrestrict_it` | `_ZN12HangarWindow6renderEv` | 9708 | `1a3edf7a21dd23311ab9ba4fe38c9c8830ceb7a8` | size | 82.9 | -6.8 |
| `src/game/mission/Status.cpp` | `O0` | `_ZN6Status16missionCompletedEbbx` | 3104 | `b73667b18432e4bc38552a4475a97f945dd6c841` | size | 8.3 | -84.2 |
| `src/game/mission/Status.cpp` | `O1` | `_ZN6Status16missionCompletedEbbx` | 1980 | `49e2c3c72bdcd65273458da4fe6ea4c42af03cce` | size | 11.8 | -80.7 |
| `src/game/mission/Status.cpp` | `O2` | `_ZN6Status16missionCompletedEbbx` | 1800 | `7916931fa6a7a076208ebaa81414b5814f574be7` | size | 8.2 | -84.3 |
| `src/game/mission/Status.cpp` | `O3` | `_ZN6Status16missionCompletedEbbx` | 1884 | `80ead99e1f8a7f08770ec66254592c6e422b3067` | size | 8.1 | -84.4 |
| `src/game/mission/Status.cpp` | `Os` | `_ZN6Status16missionCompletedEbbx` | 1460 | `17afdfbc5e59382f17e75f150c6efb590e7690f9` | size | 34.1 | -58.4 |
| `src/game/mission/Status.cpp` | `fno_inline` | `_ZN6Status16missionCompletedEbbx` | 1588 | `cc0697d893b98744db189d599e59c98cf8105268` | size | 28.7 | -63.8 |
| `src/game/mission/Status.cpp` | `fno_jump_tables` | `_ZN6Status16missionCompletedEbbx` | 1400 | `5703ea31ff174c8c63bddd42727af552d2dc2cb3` | size | 13.3 | -79.2 |
| `src/game/mission/Status.cpp` | `mrestrict_it` | `_ZN6Status16missionCompletedEbbx` | 1420 | `9bd5f8603fd6ef9df19a2ecbc0bcfc8d07aebec5` | size | 63.7 | -28.8 |
| `src/game/weapons/Gun.cpp` | `O0` | `_ZN3Gun22calcCharacterCollisionEv` | 3060 | `392f6058779ca8b24fddfcba715e1a73e7ef8cb1` | size | 2.5 | -90.9 |
| `src/game/weapons/Gun.cpp` | `O1` | `_ZN3Gun22calcCharacterCollisionEv` | 2000 | `08f305f853eafdf9655ead9119d6dac47459b54d` | size | 37.4 | -56.0 |
| `src/game/weapons/Gun.cpp` | `O2` | `_ZN3Gun22calcCharacterCollisionEv` | 2032 | `77bf7b391947ce23c99e58bfae40c94690b32351` | size | 61.8 | -31.6 |
| `src/game/weapons/Gun.cpp` | `O3` | `_ZN3Gun22calcCharacterCollisionEv` | 2040 | `a187d6dc28bf28df49f35ca1ed06c83656ec5b87` | size | 61.6 | -31.8 |
| `src/game/weapons/Gun.cpp` | `Os` | `_ZN3Gun22calcCharacterCollisionEv` | 1984 | `9fb6607ea84fba72a8ea03f58ab28987475b5f5a` | 1862 | 62.8 | -30.6 |
| `src/game/weapons/Gun.cpp` | `fno_inline` | `_ZN3Gun22calcCharacterCollisionEv` | 1984 | `388476046843693c6007dbfd29de7b014022928d` | 1820 | 47.6 | -45.8 |
| `src/game/weapons/Gun.cpp` | `fno_stack_protector` | `_ZN3Gun22calcCharacterCollisionEv` | 1944 | `34c9d8337d89d353184966b2f4c9ff068da8cd34` | size | 68.7 | -24.7 |
| `src/game/weapons/Gun.cpp` | `fstack_protector` | `_ZN3Gun22calcCharacterCollisionEv` | 1944 | `34c9d8337d89d353184966b2f4c9ff068da8cd34` | size | 68.7 | -24.7 |
| `src/game/weapons/Gun.cpp` | `mrestrict_it` | `_ZN3Gun22calcCharacterCollisionEv` | 1972 | `dcbcdf9f5a0b4f8d426bec07913a2c4d425ea94e` | size | 86.3 | -7.1 |
| `src/game/weapons/Gun.cpp` | `fno_honor_nans` | `_ZN3Gun22calcCharacterCollisionEv` | 1984 | `6ad47221df6ddd0fd8d247950d02d1be78eaea7c` | 7 | 92.3 | -1.1 |
| `src/game/ship/PlayerEgo.cpp` | `O0` | `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` | 1380 | `88311c6af37d69ffbe90cedbc455797a70d3fc0f` | size | 6.2 | -89.3 |
| `src/game/ship/PlayerEgo.cpp` | `O1` | `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` | 1056 | `65d678fecf53264652b04960ba906b1ff07fcb2c` | size | 37.7 | -57.8 |
| `src/game/ship/PlayerEgo.cpp` | `O2` | `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` | 1040 | `6d89402b45210b502368ce6d37c8d3695c53bda2` | size | 28.7 | -66.8 |
| `src/game/ship/PlayerEgo.cpp` | `O3` | `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` | 1072 | `6352567f7df72aca684f6962ea1d10812014fa5c` | size | 66.9 | -28.6 |
| `src/game/ship/PlayerEgo.cpp` | `Os` | `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` | 1036 | `353dd022414f2d77c8c1b8af18e3b196804c95e8` | size | 71.2 | -24.3 |
| `src/game/ship/PlayerEgo.cpp` | `g0` | `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` | 968 | `52343d3468c324ba7a3cec2c496b271f1c702b9c` | 23 | 95.2 | -0.3 |
| `src/game/ship/PlayerEgo.cpp` | `fno_inline` | `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` | 1016 | `e2237bb745691faad23c99b5778be75fa50f4bcf` | size | 63.1 | -32.4 |
| `src/game/ship/PlayerEgo.cpp` | `fno_stack_protector` | `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` | 972 | `9644d1add690ef454a6cb51a1a1cff213955493e` | size | 65.2 | -30.3 |
| `src/game/ship/PlayerEgo.cpp` | `fstack_protector` | `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` | 972 | `9644d1add690ef454a6cb51a1a1cff213955493e` | size | 65.2 | -30.3 |
| `src/game/ship/PlayerEgo.cpp` | `mrestrict_it` | `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` | 968 | `d13f839eac2ee20fac0afab6a5ad0a8a5c29912a` | 46 | 91.8 | -3.7 |
| `src/game/ship/PlayerEgo.cpp` | `fno_honor_nans` | `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` | 968 | `99ffc2c0176242fcfdedaf5dc2eb3d27f74e29fd` | 1 | 95.2 | -0.3 |
| `src/game/menu/SpaceLounge.cpp` | `O0` | `_ZN11SpaceLounge10drawLoungeEv` | 3164 | `00636d991e9acd00a23e3dcecc48a13a2b136157` | size | 7.9 | -87.8 |
| `src/game/menu/SpaceLounge.cpp` | `O1` | `_ZN11SpaceLounge10drawLoungeEv` | 2072 | `0e7592d2b9c867504f0216b0a2ee75cb42c775a2` | size | 56.8 | -38.9 |
| `src/game/menu/SpaceLounge.cpp` | `O2` | `_ZN11SpaceLounge10drawLoungeEv` | 1976 | `d2e40f0f8827079d7350d2962d9965804951c112` | size | 78.0 | -17.7 |
| `src/game/menu/SpaceLounge.cpp` | `O3` | `_ZN11SpaceLounge10drawLoungeEv` | 2000 | `787b07c8be2fc4caf13611ae25458a4b2782a6fa` | size | 77.4 | -18.3 |
| `src/game/menu/SpaceLounge.cpp` | `Os` | `_ZN11SpaceLounge10drawLoungeEv` | 1972 | `6daf3dc79baa79f7a43cf583553a808293a1d805` | size | 78.2 | -17.5 |
| `src/game/menu/SpaceLounge.cpp` | `fno_inline` | `_ZN11SpaceLounge10drawLoungeEv` | 2028 | `a64f2362b0b0bbb755aad51620e27d0c964d780b` | size | 64.4 | -31.3 |
| `src/game/menu/SpaceLounge.cpp` | `fno_stack_protector` | `_ZN11SpaceLounge10drawLoungeEv` | 1912 | `6d52f0ea0989320f7a36d60582ededfb998ef6a5` | size | 75.6 | -20.1 |
| `src/game/menu/SpaceLounge.cpp` | `fstack_protector` | `_ZN11SpaceLounge10drawLoungeEv` | 1912 | `6d52f0ea0989320f7a36d60582ededfb998ef6a5` | size | 75.6 | -20.1 |
| `src/game/menu/SpaceLounge.cpp` | `mrestrict_it` | `_ZN11SpaceLounge10drawLoungeEv` | 1948 | `0fcf038b5d7dc1e578dbebc89075c93851cb9a69` | 132 | 93.9 | -1.8 |
| `src/game/menu/SpaceLounge.cpp` | `tu_specific_off` | `_ZN11SpaceLounge10drawLoungeEv` | 1948 | `d2c1f3d84f612ac236c51ce57e7625fd5e01a12e` | 4 | 95.5 | -0.2 |
| `src/game/ui/Hud.cpp` | `O0` | `_ZN3Hud4drawExxP9PlayerEgobjj` | 16192 | `6967f3a747c874e48c1da39734d9d4fa8d76055c` | size | 7.1 | -88.9 |
| `src/game/ui/Hud.cpp` | `O1` | `_ZN3Hud4drawExxP9PlayerEgobjj` | 9448 | `c990812e7d8eed5368d41e22da88653256ec4856` | size | 58.0 | -38.0 |
| `src/game/ui/Hud.cpp` | `O2` | `_ZN3Hud4drawExxP9PlayerEgobjj` | 9680 | `281a1e9658a33b018a9b650bd76ef504e9139a53` | size | 52.5 | -43.5 |
| `src/game/ui/Hud.cpp` | `O3` | `_ZN3Hud4drawExxP9PlayerEgobjj` | 9876 | `1da0485cba37b3700d2ee3341d1e11dd7e78edb0` | size | 48.6 | -47.4 |
| `src/game/ui/Hud.cpp` | `Os` | `_ZN3Hud4drawExxP9PlayerEgobjj` | 9524 | `58521e46afa15fe12e59d6f86f9d920dd389fa94` | size | 61.3 | -34.7 |
| `src/game/ui/Hud.cpp` | `fno_jump_tables` | `_ZN3Hud4drawExxP9PlayerEgobjj` | 9336 | `723aad5847f14da88f4db7f039013829bbf19d3c` | size | 91.9 | -4.1 |
| `src/game/ui/Hud.cpp` | `fno_stack_protector` | `_ZN3Hud4drawExxP9PlayerEgobjj` | 9248 | `a708dce5c9968fa09ea517ff3704c643e8e321de` | size | 63.5 | -32.5 |
| `src/game/ui/Hud.cpp` | `fstack_protector` | `_ZN3Hud4drawExxP9PlayerEgobjj` | 9248 | `a708dce5c9968fa09ea517ff3704c643e8e321de` | size | 63.5 | -32.5 |
| `src/game/ui/Hud.cpp` | `mno_unaligned_access` | `_ZN3Hud4drawExxP9PlayerEgobjj` | 9340 | `cb06b30056218c12bf68cdc71a1711e829e4eed3` | size | 87.7 | -8.3 |
| `src/game/ui/Hud.cpp` | `mrestrict_it` | `_ZN3Hud4drawExxP9PlayerEgobjj` | 9372 | `743d34d6b8de6b2f6419c87ff776448538d5be0c` | size | 80.6 | -15.4 |
| `src/game/ui/Hud.cpp` | `fno_honor_nans` | `_ZN3Hud4drawExxP9PlayerEgobjj` | 9320 | `aab64a1f14e5a361c35b79ebdca23cea017a8ab6` | size | 93.5 | -2.5 |
| `src/game/ui/MenuTouchWindow.cpp` | `O0` | `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 3468 | `1d0b1d273ed53e911598da164ec39dd93bf7d527` | size | 4.5 | -93.6 |
| `src/game/ui/MenuTouchWindow.cpp` | `O1` | `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 2188 | `4beffda79056848ba0a453437b4383220e4aaef1` | size | 36.0 | -62.1 |
| `src/game/ui/MenuTouchWindow.cpp` | `O2` | `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 2000 | `f81d8ead95231ee4184e6a7002b63eb081bb1211` | size | 44.7 | -53.4 |
| `src/game/ui/MenuTouchWindow.cpp` | `O3` | `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 2016 | `9d6ce0b83a423aac925408799a43981c04207545` | size | 44.5 | -53.6 |
| `src/game/ui/MenuTouchWindow.cpp` | `Os` | `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 2000 | `427e973c7bf93605fa5b4aa3800a075d1281a7a3` | size | 44.7 | -53.4 |
| `src/game/ui/MenuTouchWindow.cpp` | `g0` | `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 1892 | `709d765e9a40b4cc70dcb345733ec3e0de3fd48d` | size | 83.2 | -14.9 |
| `src/game/ui/MenuTouchWindow.cpp` | `fno_strict_aliasing` | `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 1892 | `23a6063117626ed5c70420cae80f0b4efca20802` | size | 90.9 | -7.2 |
| `src/game/ui/MenuTouchWindow.cpp` | `fno_inline` | `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 2064 | `732fe72b74a8db61fe4b73fdc9cb358da4901dc2` | size | 48.9 | -49.2 |
| `src/game/ui/MenuTouchWindow.cpp` | `fno_jump_tables` | `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 1932 | `1159ea4e2510ee7d41a23a1fc53511521696de20` | size | 70.6 | -27.5 |
| `src/game/ui/MenuTouchWindow.cpp` | `mrestrict_it` | `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 1888 | `2995cd9d975ac2c8e01cb8915ceaaa9da447889a` | size | 82.8 | -15.3 |
| `src/game/menu/ModStation.cpp` | `O0` | `_ZN10ModStationC1Ev` | 1652 | `d357f49c89fd5352d7e5a0fb5c31bfe0a3bf02ee` | size | 3.4 | -95.8 |
| `src/game/menu/ModStation.cpp` | `O0` | `_ZN10ModStationC2Ev` | 1652 | `d357f49c89fd5352d7e5a0fb5c31bfe0a3bf02ee` | size | 3.4 | -95.8 |
| `src/game/menu/ModStation.cpp` | `O1` | `_ZN10ModStationC1Ev` | 908 | `597fe5c4933704e422049596caefe885ac623fb5` | size | 44.2 | -55.0 |
| `src/game/menu/ModStation.cpp` | `O1` | `_ZN10ModStationC2Ev` | 908 | `597fe5c4933704e422049596caefe885ac623fb5` | size | 44.2 | -55.0 |
| `src/game/menu/ModStation.cpp` | `O2` | `_ZN10ModStationC1Ev` | 1016 | `3e8445b016e73ac47703fca07d9d4f62713faf5d` | size | 90.8 | -8.4 |
| `src/game/menu/ModStation.cpp` | `O2` | `_ZN10ModStationC2Ev` | 1016 | `3e8445b016e73ac47703fca07d9d4f62713faf5d` | size | 90.8 | -8.4 |
| `src/game/menu/ModStation.cpp` | `O3` | `_ZN10ModStationC1Ev` | 1088 | `04a813edd3bed7e0048f1726217b93e75013a381` | size | 87.2 | -12.0 |
| `src/game/menu/ModStation.cpp` | `O3` | `_ZN10ModStationC2Ev` | 1088 | `04a813edd3bed7e0048f1726217b93e75013a381` | size | 87.2 | -12.0 |
| `src/game/menu/ModStation.cpp` | `Os` | `_ZN10ModStationC1Ev` | 1008 | `18a83554ba1f551811d3cce08cdc9f5a5763d897` | 926 | 93.4 | -5.8 |
| `src/game/menu/ModStation.cpp` | `Os` | `_ZN10ModStationC2Ev` | 1008 | `18a83554ba1f551811d3cce08cdc9f5a5763d897` | 926 | 93.4 | -5.8 |
| `src/game/menu/ModStation.cpp` | `fno_inline` | `_ZN10ModStationC1Ev` | 908 | `666b03b1bd7bbed13da8749e5e2b833b00ebd783` | size | 45.4 | -53.8 |
| `src/game/menu/ModStation.cpp` | `fno_inline` | `_ZN10ModStationC2Ev` | 908 | `666b03b1bd7bbed13da8749e5e2b833b00ebd783` | size | 45.4 | -53.8 |
| `src/game/menu/ModStation.cpp` | `fno_slp_vectorize` | `_ZN10ModStationC1Ev` | 972 | `14a4e039fa28fbf5ce9667c61fd5aa4e890b1e66` | size | 73.8 | -25.4 |
| `src/game/menu/ModStation.cpp` | `fno_slp_vectorize` | `_ZN10ModStationC2Ev` | 972 | `14a4e039fa28fbf5ce9667c61fd5aa4e890b1e66` | size | 73.8 | -25.4 |
| `src/game/menu/ModStation.cpp` | `fno_stack_protector` | `_ZN10ModStationC1Ev` | 972 | `57c04904aa66771ea8260cf10fd69c88fbb6ed88` | size | 75.6 | -23.6 |
| `src/game/menu/ModStation.cpp` | `fno_stack_protector` | `_ZN10ModStationC2Ev` | 972 | `57c04904aa66771ea8260cf10fd69c88fbb6ed88` | size | 75.6 | -23.6 |
| `src/game/menu/ModStation.cpp` | `fstack_protector` | `_ZN10ModStationC1Ev` | 972 | `57c04904aa66771ea8260cf10fd69c88fbb6ed88` | size | 75.6 | -23.6 |
| `src/game/menu/ModStation.cpp` | `fstack_protector` | `_ZN10ModStationC2Ev` | 972 | `57c04904aa66771ea8260cf10fd69c88fbb6ed88` | size | 75.6 | -23.6 |
| `src/game/menu/ModStation.cpp` | `mrestrict_it` | `_ZN10ModStationC1Ev` | 1000 | `ddae6feb580366a6a2c851b00038e92c81e9d9a0` | size | 90.1 | -9.1 |
| `src/game/menu/ModStation.cpp` | `mrestrict_it` | `_ZN10ModStationC2Ev` | 1000 | `ddae6feb580366a6a2c851b00038e92c81e9d9a0` | size | 90.1 | -9.1 |

The five cells (six target functions) with changed target bytes but unchanged rounded percentage are:

| TU | flag | function | size | diff bytes | pct |
|---|---|---|---:|---:|---:|
| `src/game/world/Level.cpp` | `g0` | `_ZN5Level13createMissionEv` | 17172 | 7 | 92.3 |
| `src/game/weapons/Radar.cpp` | `g0` | `_ZN5Radar4drawEP6PlayerP3Hudi` | 9908 | 13 | 74.5 |
| `src/game/menu/MGame.cpp` | `g0` | `_ZN5MGame10OnTouchEndEiiPv` | 8080 | 37 | 81.6 |
| `src/game/menu/MGame.cpp` | `g0` | `_ZN5MGame8OnUpdateEv` | 11440 | 14 | 99.9 |
| `src/game/world/Level.cpp` | `fno_honor_nans` | `_ZN5Level13createMissionEv` | 17172 | 2 | 92.3 |
| `src/game/weapons/Radar.cpp` | `fno_honor_nans` | `_ZN5Radar4drawEP6PlayerP3Hudi` | 9908 | 13 | 74.5 |

(That is five cells; `MGame.cpp g0` owns two of the six functions.)

## 5. Full official-verifier validation

No cell improved a target, so there is no positive cell to promote. The table
below validates the code-changing/score-neutral cells, the TU-specific cell, and
the strongest known load-bearing negatives with the full official verifier.
Baseline is `4505 / 4505 / 4524`, avg 99.94.

| TU | flag | byte | linked | size | avg |
|---|---|---:|---:|---:|---:|
| `src/game/world/Level.cpp` | `g0` | 4486 | 4497 | 4524/4524 | 99.93 |
| `src/game/weapons/Radar.cpp` | `g0` | 4504 | 4504 | 4524/4524 | 99.94 |
| `src/game/menu/MGame.cpp` | `g0` | 4471 | 4501 | 4524/4524 | 99.93 |
| `src/game/world/Level.cpp` | `fno_honor_nans` | 4480 | 4498 | 4524/4524 | 99.93 |
| `src/game/weapons/Radar.cpp` | `fno_honor_nans` | 4504 | 4504 | 4524/4524 | 99.94 |
| `src/game/menu/SpaceLounge.cpp` | `tu_specific_off` | 4505 | 4505 | 4524/4524 | 99.94 |
| `src/game/mission/Generator.cpp` | `fno_strict_aliasing` | 2615 | 4505 | 4524/4524 | 99.93 |
| `src/game/ui/MenuTouchWindow.cpp` | `g0` | 3616 | 4502 | 4524/4524 | 99.93 |
| `src/game/ui/Layout.cpp` | `mrestrict_it` | 2454 | 4485 | 4524/4524 | 99.9 |

All selected cells are at or below the canonical gate. The only non-baseline selected cell that is gate-neutral
is `SpaceLounge.cpp tu_specific_off` (4505/4505), but it is a target regression
(95.7 -> 95.5), so it is not an improvement. `Generator.cpp
-fno-strict-aliasing` keeps `linked_exact` at 4505 but collapses `byte_exact` to
2615; this is the stale wave-119 positive cell, now a regression at the current
source. `Layout.cpp -mrestrict_it` also shrinks `.text` by 32 B (1,259,432 vs
1,259,464). Every other selected cell keeps the canonical section sizes
(`.text` 1,259,464, `.rodata` 269,351, `.ARM.exidx` 28,528, `.ARM.extab`
156,312).

## 6. Proven / disproven / unobservable

**Proven by measurement**

* No tested per-TU flag makes any of the 19 functions byte-exact or linked-exact.
* No tested per-TU flag improves any of the 19 official fuzzy percentages.
* `-Oz` is uniquely best; `-O2`/`-Os`/`-O0`/`-O1`/`-O3` all lower every target.
* `-g`/`-g2` are baseline; `-g0` is not (`MenuTouchWindow::OnTouchBegin`
  98.1 -> 83.2, `Generator::createMission` 84.7 -> 84.4, `PlayerEgo::calcCollision`
  95.5 -> 95.2).
* `-fno-vectorize`, `-fno-omit-frame-pointer`, `-mno-restrict-it`,
  `-fno-math-errno`, `-ffp-contract=off/fast`,
  `-fno-delete-null-pointer-checks`, `-fno-threadsafe-statics`,
  `-fno-use-cxa-atexit`, `-fno-sized-deallocation`, `-fno-common` and
  `-fvisibility=default` are fully neutral on all 14 TUs. (`-fno-common` is
  per-TU neutral here; the global duplicate `g_android_back_button_pressed` is in
  `BackButton.c`, outside these 14 TUs.)
* `-fno-slp-vectorize` is load-bearing (5 TUs, 7 target functions; `Layout` drops
  to 18.4). `-fno-inline` is load-bearing (12 TUs, 16 target functions).
  `-fno-jump-tables` is load-bearing (7 TUs, 9 target functions).
  `-fno-stack-protector` and `-fstack-protector` both lower 14 target functions;
  the canonical `-fstack-protector-strong` is load-bearing.
* `-mrestrict_it` changes all 19 target functions and lowers all 19;
  `-mno-restrict-it` is fully neutral. `-mno-unaligned-access` changes only
  `Level::createMission` and `Hud::draw`, lowering both.
* `-fno-honor-nans` changes 7 target functions and lowers 5.
* `-fno-unroll-loops` changes only `Level::createScene` and lowers it 18.4.
* `-fno-strict-aliasing` changes 7 target functions and lowers all 7; the old wave-119
  `Generator::createMission` +1.4 is stale (now 84.7 -> 50.8).
* SpaceLounge's checked-in 7:1 branch weights are real but gate-neutral:
  removing them changes `drawLounge` bytes and drops 95.7 -> 95.5, while the gate
  remains 4505/4505.

**Disproven**

* A per-TU optimization-level artifact (`-O2`/`-Os`/`-O0`/`-O1`/`-O3`) explains
  any remaining mismatch.
* A per-TU `-g0` artifact explains any remaining mismatch.
* A per-TU stack-protector, frame-pointer, jump-table, SLP, inlining,
  strict-aliasing, restrict-IT, unaligned-access, NaN or unroll flag explains any
  remaining mismatch.
* The wave-119 `Generator.cpp -fno-strict-aliasing` positive result still holds at
  the current source.

**Unobservable / not excluded**

* The fully neutral flags cannot be excluded from the original build; they also
  cannot explain the residual because they change neither target nor any function
  in these TUs.
* The exact per-TU flag history of a TU that is codegen-neutral under a flag is
  not recoverable from the stripped `.so`.
* A multi-flag combination outside the tested 450-cell single-flag space could in
  principle change a target; the single-flag space is exhausted, and the neutral
  flags offer no positive fingerprint.
* The exact original build driver and per-file flag list remain unrecoverable from
  the stripped ELF; this pass only rules out the tested per-TU single-flag
  explanations.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7005
cmake --preset default   -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so   -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8

# 450-cell per-TU matrix (scratch harness)
python3 /dev/shm/w7005/pertu/run_matrix.py 8
python3 /dev/shm/w7005/pertu/run_official.py
```

Scratch artifacts (not tracked): `/dev/shm/w7005/pertu/results.json` (all 450
cells), `/dev/shm/w7005/pertu/official.json` (full-verifier cells),
`/dev/shm/w7005/pertu/harness.py`, `run_matrix.py`, `run_official.py`,
`make_tables.py`, `make_matrix.py`, and the per-cell objects/`.so`s under
`/dev/shm/w7005/pertu/obj/` and `/dev/shm/w7005/pertu/so/`.

## 8. Files touched

* `_work/reconstructions/per-tu-flag-sensitivity-wave120-w7005.md` (this note).
* Scratch only under `/dev/shm/w7005/`.
* No `src/`, `tools/`, `CMakeLists.txt`, baseline, allowlist or `report.json`
  change is committed.
