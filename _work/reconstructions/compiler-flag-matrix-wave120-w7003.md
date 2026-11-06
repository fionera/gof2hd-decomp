# Compiler-flag matrix, wave 120 (w7003)

Status: research note, 2026-11-06. Worktree `/home/debian/GalaxyOnFire2/wt/w7003`,
branch `wave120-w7003`, base `ef7fb9cde`. Scratch: `/dev/shm/w7003`.
**No `src/`, build-flag, baseline or allowlist change is landed. This note is the only commit.**

## 0. Bottom line

* The canonical build is unchanged and green: **byte_exact 4505/4524, linked_exact
  4505/4524, size-exact 4524/4524, avg 99.94%**, data 843/843, imports 207,
  lint `CLEAN ✓`, drift 0, `RATCHET PASS`.
* I measured **56 curated global flag variants** on top of the canonical
  `-Oz -g -std=c++11 -fPIC -frtti -fstack-protector-strong -ffunction-sections
  -funwind-tables -DANDROID -DNDEBUG -stdlib=libc++` recipe: **49 built, 7 failed
  to build/link; 29 were exactly gate-neutral; 20 were worse; none improved the
  gate.**
* **No variant improved any of the 19 non-exact functions.** There are zero
  positive per-function fuzzy deltas in the whole matrix. Fourteen variants
  changed at least one target's fuzzy score, and every such change was a
  regression; six more changed target bytes without changing the normalized
  instruction stream.
* The most useful new negatives are:
  * `-fno-builtin-memcpy`, `-fno-builtin-memset` and `-fno-builtin-malloc` are
    **not** neutral (1359 / 1351 / 1490 byte_exact), while
    `-fno-builtin-printf` and `-fno-builtin-sinf` are neutral.
  * `-ffp-contract=on` is worse (2076); `-ffp-contract=off` and
    `-ffp-contract=fast` are both neutral.
  * `-mrestrict-it` is worse (1271); `-mno-restrict-it` is neutral.
  * `-mexecute-only` is worse (1299); `-mno-execute-only` is neutral.
  * `-mthread-model single` is worse (2085).
  * `-fno-common` still link-fails on the duplicate
    `g_android_back_button_pressed`; `-fno-integrated-as` fails because the
    external assembler rejects `-mfpu=neon`.
  * `-mvectorize-with-neon-quad`, `-fno-weak`, `-fno-semantic-interposition`,
    `-mno-fp16` and `-fno-excess-precision=fast` are unknown to clang 7.0.2.
* The remaining 19 are **source/TU-shape-driven**, not flag-driven. No per-TU
  application is indicated: the only historically interesting candidate,
  `-fno-strict-aliasing`, already regresses `Generator::createMission` by
  **-33.9** globally, and that function is owned by `Generator.cpp`, so a
  per-TU application would reproduce the same regression, not an improvement.

## 1. Baseline and artifacts

Commands:

```sh
cd /home/debian/GalaxyOnFire2/wt/w7003
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j64
cmake --build cmake-build-match --target verify -j64
cmake --build cmake-build-match --target check -j64
```

Measured baseline:

```text
verify: byte 4505  linked 4505  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

| artifact | sha256 | size |
|---|---|---:|
| original `android_2.0.16_libgof2hdaa.so` | `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483` | 2,187,328 |
| canonical `cmake-build-match/libgof2hdaa.so` | `5f67c94e399bb7a16a05b5c31c1c7df4676230a3a3608971a7475c5c2f1b391e` | 2,187,328 |

Section sizes (original → canonical):

| section | original | canonical |
|---|---:|---:|
| `.text` | `0x1337c8` | `0x1337c8` |
| `.rodata` | `0x41b67` | `0x41c27` (+192 B rank table) |
| `.ARM.exidx` | `0x6f70` | `0x6f70` |
| `.ARM.extab` | `0x26298` | `0x26298` |

Baseline differing bytes: `.text` 88,216; `.ARM.exidx` 863;
`.ARM.extab` 109,364; `.rel.plt` 6; `.note.gnu.build-id` 20; `.rodata` is a
strict prefix plus the 192 B table. Total same-size differing bytes: 198,469.

### 1.1 The 19 non-exact functions

| function | mangled | pct |
|---|---|---:|
| `Layout::Layout` | `_ZN6LayoutC1Ev` | 49.0 |
| `Layout::Layout` | `_ZN6LayoutC2Ev` | 49.0 |
| `LevelScript::process` | `_ZN11LevelScript7processEi` | 60.0 |
| `Level::createShip` | `_ZN5Level10createShipEiiiP8Waypointbb` | 74.4 |
| `Radar::draw` | `_ZN5Radar4drawEP6PlayerP3Hudi` | 74.5 |
| `MGame::OnTouchEnd` | `_ZN5MGame10OnTouchEndEiiPv` | 81.6 |
| `Generator::createMission` | `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 84.7 |
| `HangarWindow::render` | `_ZN12HangarWindow6renderEv` | 89.7 |
| `Level::createMission` | `_ZN5Level13createMissionEv` | 92.3 |
| `Status::missionCompleted` | `_ZN6Status16missionCompletedEbbx` | 92.5 |
| `Level::createScene` | `_ZN5Level11createSceneEv` | 92.8 |
| `Gun::calcCharacterCollision` | `_ZN3Gun22calcCharacterCollisionEv` | 93.4 |
| `PlayerEgo::calcCollision` | `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` | 95.5 |
| `SpaceLounge::drawLounge` | `_ZN11SpaceLounge10drawLoungeEv` | 95.7 |
| `Hud::draw` | `_ZN3Hud4drawExxP9PlayerEgobjj` | 96.0 |
| `MenuTouchWindow::OnTouchBegin` | `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 98.1 |
| `ModStation::ModStation` | `_ZN10ModStationC1Ev` | 99.2 |
| `ModStation::ModStation` | `_ZN10ModStationC2Ev` | 99.2 |
| `MGame::OnUpdate` | `_ZN5MGame8OnUpdateEv` | 99.9 |

## 2. Method

Each variant is one extra flag appended after the canonical flags to both
`CMAKE_CXX_FLAGS` and `CMAKE_C_FLAGS`, in a fresh build directory under
`/dev/shm/w7003/flag_fingerprint/build-<variant>`. The harness is
`tools/flag_fingerprint.py --mode variants` with a 56-entry JSON file
(`/dev/shm/w7003/variants_wave120.json`). The canonical
`cmake-build-match` is never modified.

The vendored libcrypto archive is shared in `third_party/openssl`, so parallel
variant builds can race while rebuilding it. After the first parallel pass I
rebuilt the three vendored archives serially and relinked every successful
variant with `cmake --build <build-dir> --target link_so`; the final table is
from those relinked outputs. This is a harness detail, not a flag result.

Gate counts are the official verifier's `byte_exact` / `linked_exact` /
size-exact, run through the validated fast path (the fast path only
short-circuits the difflib fuzzy score and the pool-reconciliation
`SequenceMatcher`). The 19 target percentages were then computed with the
official `verify.disassemble()`, `verify.reconcile()` and `verify.score()`
(`autojunk=False`) on the relinked `.so`s, not with the fast short-circuit.

Fast-counter validation against the full official verifier:

| build | fast | official |
|---|---|---|
| canonical `-Oz` | 4505 / 4505 / 4524 | 4505 / 4505 / 4524 |
| `-fno-plt` | 4505 / 4505 / 4524 | 4505 / 4505 / 4524 |
| `-fno-jump-tables` | 1390 / 4413 / 4437 | 1390 / **4416** / 4437 |
| `-fno-strict-aliasing` | 1395 / 4349 / 4411 | 1395 / **4351** / 4411 |

`byte_exact` and size-exact are identical; `linked_exact` can undercount by a
few because the fast path skips pool reconciliation. That does not affect the
conclusion: no variant's `byte_exact` is above 4505.

## 3. Full matrix

`19 chg/imp` = number of the 19 non-exact functions whose raw bytes changed /
number whose fuzzy percentage improved. `neutral` means exactly
4505/4505/4524 with zero changed functions; `worse` means any gate count or
section size moved away from baseline. Build failures are reported with the
compiler/linker reason.

| variant | flags | byte | linked | size-exact | .text | .rodata | .ARM.exidx | .ARM.extab | 19 chg/imp | verdict |
|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---|
| `fexcess_precision_fast` | `-fexcess-precision=fast` | 4505 | 4505 | 4524/4524 | 0x1337c8 | 0x41c27 | 0x6f70 | 0x26298 | 0/0 | neutral |
| `ffp_contract_fast` | `-ffp-contract=fast` | 4505 | 4505 | 4524/4524 | 0x1337c8 | 0x41c27 | 0x6f70 | 0x26298 | 0/0 | neutral |
| `ffp_contract_off` | `-ffp-contract=off` | 4505 | 4505 | 4524/4524 | 0x1337c8 | 0x41c27 | 0x6f70 | 0x26298 | 0/0 | neutral |
| `fmerge_all_constants` | `-fmerge-all-constants` | 4505 | 4505 | 4524/4524 | 0x1337c8 | 0x41c27 | 0x6f70 | 0x26298 | 0/0 | neutral |
| `fno_associative_math` | `-fno-associative-math` | 4505 | 4505 | 4524/4524 | 0x1337c8 | 0x41c27 | 0x6f70 | 0x26298 | 0/0 | neutral |
| `fno_builtin_printf` | `-fno-builtin-printf` | 4505 | 4505 | 4524/4524 | 0x1337c8 | 0x41c27 | 0x6f70 | 0x26298 | 0/0 | neutral |
| `fno_builtin_sinf` | `-fno-builtin-sinf` | 4505 | 4505 | 4524/4524 | 0x1337c8 | 0x41c27 | 0x6f70 | 0x26298 | 0/0 | neutral |
| `fno_delete_null_pointer_checks` | `-fno-delete-null-pointer-checks` | 4505 | 4505 | 4524/4524 | 0x1337c8 | 0x41c27 | 0x6f70 | 0x26298 | 0/0 | neutral |
| `fno_finite_math_only` | `-fno-finite-math-only` | 4505 | 4505 | 4524/4524 | 0x1337c8 | 0x41c27 | 0x6f70 | 0x26298 | 0/0 | neutral |
| `fno_honor_infinities` | `-fno-honor-infinities` | 4505 | 4505 | 4524/4524 | 0x1337c8 | 0x41c27 | 0x6f70 | 0x26298 | 0/0 | neutral |
| `fno_lax_vector_conversions` | `-fno-lax-vector-conversions` | 4505 | 4505 | 4524/4524 | 0x1337c8 | 0x41c27 | 0x6f70 | 0x26298 | 0/0 | neutral |
| `fno_merge_constants` | `-fno-merge-constants` | 4505 | 4505 | 4524/4524 | 0x1337c8 | 0x41c27 | 0x6f70 | 0x26298 | 0/0 | neutral |
| `fno_omit_frame_pointer` | `-fno-omit-frame-pointer` | 4505 | 4505 | 4524/4524 | 0x1337c8 | 0x41c27 | 0x6f70 | 0x26298 | 0/0 | neutral |
| `fno_plt` | `-fno-plt` | 4505 | 4505 | 4524/4524 | 0x1337c8 | 0x41c27 | 0x6f70 | 0x26298 | 0/0 | neutral |
| `fno_rounding_math` | `-fno-rounding-math` | 4505 | 4505 | 4524/4524 | 0x1337c8 | 0x41c27 | 0x6f70 | 0x26298 | 0/0 | neutral |
| `fno_short_enums` | `-fno-short-enums` | 4505 | 4505 | 4524/4524 | 0x1337c8 | 0x41c27 | 0x6f70 | 0x26298 | 0/0 | neutral |
| `fno_sized_deallocation` | `-fno-sized-deallocation` | 4505 | 4505 | 4524/4524 | 0x1337c8 | 0x41c27 | 0x6f70 | 0x26298 | 0/0 | neutral |
| `fno_strict_enums` | `-fno-strict-enums` | 4505 | 4505 | 4524/4524 | 0x1337c8 | 0x41c27 | 0x6f70 | 0x26298 | 0/0 | neutral |
| `fno_trapping_math` | `-fno-trapping-math` | 4505 | 4505 | 4524/4524 | 0x1337c8 | 0x41c27 | 0x6f70 | 0x26298 | 0/0 | neutral |
| `fno_unsafe_math_optimizations` | `-fno-unsafe-math-optimizations` | 4505 | 4505 | 4524/4524 | 0x1337c8 | 0x41c27 | 0x6f70 | 0x26298 | 0/0 | neutral |
| `fvisibility_default` | `-fvisibility=default` | 4505 | 4505 | 4524/4524 | 0x1337c8 | 0x41c27 | 0x6f70 | 0x26298 | 0/0 | neutral |
| `mno_crypto` | `-mno-crypto` | 4505 | 4505 | 4524/4524 | 0x1337c8 | 0x41c27 | 0x6f70 | 0x26298 | 0/0 | neutral |
| `mno_dsp` | `-mno-dsp` | 4505 | 4505 | 4524/4524 | 0x1337c8 | 0x41c27 | 0x6f70 | 0x26298 | 0/0 | neutral |
| `mno_execute_only` | `-mno-execute-only` | 4505 | 4505 | 4524/4524 | 0x1337c8 | 0x41c27 | 0x6f70 | 0x26298 | 0/0 | neutral |
| `mno_madd4` | `-mno-madd4` | 4505 | 4505 | 4524/4524 | 0x1337c8 | 0x41c27 | 0x6f70 | 0x26298 | 0/0 | neutral |
| `mno_movt` | `-mno-movt` | 4505 | 4505 | 4524/4524 | 0x1337c8 | 0x41c27 | 0x6f70 | 0x26298 | 0/0 | neutral |
| `mno_neg_immediates` | `-mno-neg-immediates` | 4505 | 4505 | 4524/4524 | 0x1337c8 | 0x41c27 | 0x6f70 | 0x26298 | 0/0 | neutral |
| `mno_restrict_it` | `-mno-restrict-it` | 4505 | 4505 | 4524/4524 | 0x1337c8 | 0x41c27 | 0x6f70 | 0x26298 | 0/0 | neutral |
| `mnocrc` | `-mnocrc` | 4505 | 4505 | 4524/4524 | 0x1337c8 | 0x41c27 | 0x6f70 | 0x26298 | 0/0 | neutral |
| `fno_signed_zeros` | `-fno-signed-zeros` | 2355 | 4493 | 4521/4524 | 0x133788 | 0x41c27 | 0x6f70 | 0x26298 | 16/0 | worse |
| `mthread_model_single` | `-mthread-model single` | 2085 | 4499 | 4523/4524 | 0x1337c8 | 0x41c27 | 0x6f70 | 0x26298 | 19/0 | worse |
| `ffp_contract_on` | `-ffp-contract=on` | 2076 | 4473 | 4515/4524 | 0x1337e8 | 0x41c27 | 0x6f70 | 0x26298 | 19/0 | worse |
| `fno_zero_initialized_in_bss` | `-fno-zero-initialized-in-bss` | 1522 | 4492 | 4516/4524 | 0x133768 | 0x41c27 | 0x6f70 | 0x26298 | 19/0 | worse |
| `fno_builtin_malloc` | `-fno-builtin-malloc` | 1490 | 4499 | 4522/4524 | 0x1337c8 | 0x41c27 | 0x6f70 | 0x26298 | 19/0 | worse |
| `fno_honor_nans` | `-fno-honor-nans` | 1437 | 4373 | 4480/4524 | 0x1334c8 | 0x41c27 | 0x6f70 | 0x26298 | 19/0 | worse |
| `fno_strict_aliasing` | `-fno-strict-aliasing` | 1395 | 4349 | 4411/4524 | 0x133ce8 | 0x41c27 | 0x6f80 | 0x262a0 | 19/0 | worse |
| `fno_jump_tables` | `-fno-jump-tables` | 1390 | 4413 | 4437/4524 | 0x134908 | 0x41a07 | 0x6f78 | 0x262b8 | 19/0 | worse |
| `fno_strict_overflow` | `-fno-strict-overflow` | 1388 | 4464 | 4498/4524 | 0x1338e8 | 0x41c27 | 0x6f70 | 0x26298 | 19/0 | worse |
| `fwrapv` | `-fwrapv` | 1388 | 4464 | 4498/4524 | 0x1338e8 | 0x41c27 | 0x6f70 | 0x26298 | 19/0 | worse |
| `fno_threadsafe_statics` | `-fno-threadsafe-statics` | 1368 | 4497 | 4520/4524 | 0x133684 | 0x41a77 | 0x6f50 | 0x2625c | 19/0 | worse |
| `mno_unaligned_access` | `-mno-unaligned-access` | 1367 | 4444 | 4471/4524 | 0x1334e8 | 0x41c27 | 0x6f70 | 0x2629c | 19/0 | worse |
| `fno_builtin_memcpy` | `-fno-builtin-memcpy` | 1359 | 4317 | 4377/4524 | 0x134278 | 0x41c27 | 0x6fb0 | 0x26354 | 19/0 | worse |
| `fno_math_errno` | `-fno-math-errno` | 1356 | 4491 | 4514/4524 | 0x133638 | 0x41c27 | 0x6f58 | 0x26270 | 19/0 | worse |
| `fno_use_cxa_atexit` | `-fno-use-cxa-atexit` | 1356 | 4502 | 4523/4524 | 0x133818 | 0x41c27 | 0x6f80 | 0x2628c | 19/0 | worse |
| `fno_builtin_memset` | `-fno-builtin-memset` | 1351 | 4196 | 4256/4524 | 0x134518 | 0x41c27 | 0x6f78 | 0x262d0 | 19/0 | worse |
| `fno_builtin` | `-fno-builtin` | 1350 | 3988 | 4119/4524 | 0x1347b8 | 0x41c27 | 0x6fb0 | 0x2637c | 19/0 | worse |
| `fno_optimize_sibling_calls` | `-fno-optimize-sibling-calls` | 1316 | 3592 | 3686/4524 | 0x1315b8 | 0x41c27 | 0x72e8 | 0x262a4 | 19/0 | worse |
| `mexecute_only` | `-mexecute-only` | 1299 | 3994 | 4081/4524 | 0x1211f4 | 0x42da7 | 0x6930 | 0x26294 | 19/0 | worse |
| `mrestrict_it` | `-mrestrict-it` | 1271 | 3325 | 4191/4524 | 0x1334a8 | 0x41c27 | 0x6f70 | 0x26258 | 19/0 | worse |
| `fno_common` | `-fno-common` | — | — | — | — | — | — | — | — | **build fail**: link failure: duplicate g_android_back_button_pressed |
| `fno_excess_precision_fast` | `-fno-excess-precision=fast` | — | — | — | — | — | — | — | — | **build fail**: clang 7.0.2: unknown argument (the accepted -fexcess-precision=fast is ignored) |
| `fno_integrated_as` | `-fno-integrated-as` | — | — | — | — | — | — | — | — | **build fail**: external as rejects -mfpu=neon |
| `fno_semantic_interposition` | `-fno-semantic-interposition` | — | — | — | — | — | — | — | — | **build fail**: clang 7.0.2: unknown argument |
| `fno_weak` | `-fno-weak` | — | — | — | — | — | — | — | — | **build fail**: clang 7.0.2: unknown argument |
| `mno_fp16` | `-mno-fp16` | — | — | — | — | — | — | — | — | **build fail**: clang 7.0.2: unknown argument |
| `mvectorize_with_neon_quad` | `-mvectorize-with-neon-quad` | — | — | — | — | — | — | — | — | **build fail**: clang 7.0.2: unknown argument |

### 3.1 Target changes and per-function deltas

Only the 20 non-neutral variants are listed. There are **no positive deltas**:
`improved` is 0 in every row. A dash in the last column means the target bytes
changed but the normalized instruction stream (and therefore the fuzzy score)
did not.

| variant | target bytes changed | improved | regressed | per-function pct deltas (only non-zero) |
|---|---:|---:|---:|---|
| `mexecute_only` | 19/19 | 0 | 19 | `MGame::OnUpdate` -40.6; `Level::createShip` -34.7; `Level::createMission` -29.6; `Layout::Layout` -25.6; `Layout::Layout` -25.6; `Status::missionCompleted` -25.1; `ModStation::ModStation` -21.0; `ModStation::ModStation` -21.0; `HangarWindow::render` -19.4; `SpaceLounge::drawLounge` -17.4; `MGame::OnTouchEnd` -16.4; `MenuTouchWindow::OnTouchBegin` -15.5; `PlayerEgo::calcCollision` -11.5; `Gun::calcCharacterCollision` -11.1; `Hud::draw` -9.9; `Radar::draw` -9.6; `Generator::createMission` -9.5; `LevelScript::process` -9.1; `Level::createScene` -5.9 |
| `mthread_model_single` | 19/19 | 0 | 0 | — (bytes changed, normalized stream unchanged) |
| `fno_jump_tables` | 19/19 | 0 | 9 | `Status::missionCompleted` -79.2; `Level::createShip` -35.0; `Level::createMission` -28.9; `MenuTouchWindow::OnTouchBegin` -27.5; `Level::createScene` -22.6; `LevelScript::process` -22.4; `Generator::createMission` -19.2; `Hud::draw` -4.1; `HangarWindow::render` -2.0 |
| `fno_optimize_sibling_calls` | 19/19 | 0 | 3 | `Hud::draw` -21.8; `Level::createScene` -11.5; `MGame::OnTouchEnd` -6.3 |
| `fno_builtin` | 19/19 | 0 | 6 | `ModStation::ModStation` -34.5; `ModStation::ModStation` -34.5; `Level::createScene` -8.4; `Level::createMission` -5.9; `MGame::OnTouchEnd` -0.9; `LevelScript::process` -0.1 |
| `fno_builtin_memcpy` | 19/19 | 0 | 3 | `ModStation::ModStation` -34.5; `ModStation::ModStation` -34.5; `Level::createMission` -4.8 |
| `fno_builtin_memset` | 19/19 | 0 | 3 | `ModStation::ModStation` -34.5; `ModStation::ModStation` -34.5; `Level::createMission` -4.8 |
| `fno_builtin_malloc` | 19/19 | 0 | 0 | — (bytes changed, normalized stream unchanged) |
| `fno_math_errno` | 19/19 | 0 | 0 | — (bytes changed, normalized stream unchanged) |
| `fno_signed_zeros` | 16/19 | 0 | 1 | `Level::createMission` -9.7 |
| `ffp_contract_on` | 19/19 | 0 | 1 | `LevelScript::process` -0.9 |
| `fno_zero_initialized_in_bss` | 19/19 | 0 | 0 | — (bytes changed, normalized stream unchanged) |
| `fno_threadsafe_statics` | 19/19 | 0 | 0 | — (bytes changed, normalized stream unchanged) |
| `fno_use_cxa_atexit` | 19/19 | 0 | 0 | — (bytes changed, normalized stream unchanged) |
| `fno_strict_aliasing` | 19/19 | 0 | 7 | `Generator::createMission` -33.9; `Level::createMission` -23.3; `MGame::OnUpdate` -11.0; `MenuTouchWindow::OnTouchBegin` -7.2; `MGame::OnTouchEnd` -5.5; `Radar::draw` -4.7; `LevelScript::process` -2.0 |
| `fwrapv` | 19/19 | 0 | 2 | `MGame::OnTouchEnd` -5.7; `Radar::draw` -1.3 |
| `fno_strict_overflow` | 19/19 | 0 | 2 | `MGame::OnTouchEnd` -5.7; `Radar::draw` -1.3 |
| `mrestrict_it` | 19/19 | 0 | 19 | `Status::missionCompleted` -28.8; `Hud::draw` -15.4; `MenuTouchWindow::OnTouchBegin` -15.3; `Level::createScene` -11.4; `MGame::OnUpdate` -11.2; `MGame::OnTouchEnd` -10.5; `ModStation::ModStation` -9.1; `ModStation::ModStation` -9.1; `Generator::createMission` -9.1; `Gun::calcCharacterCollision` -7.1; `HangarWindow::render` -6.8; `Level::createMission` -5.5; `Radar::draw` -5.3; `PlayerEgo::calcCollision` -3.7; `LevelScript::process` -2.8; `Level::createShip` -2.8; `SpaceLounge::drawLounge` -1.8; `Layout::Layout` -0.3; `Layout::Layout` -0.3 |
| `mno_unaligned_access` | 19/19 | 0 | 2 | `Hud::draw` -8.3; `Level::createMission` -3.4 |
| `fno_honor_nans` | 19/19 | 0 | 5 | `MGame::OnUpdate` -10.8; `Hud::draw` -2.5; `LevelScript::process` -1.1; `Gun::calcCharacterCollision` -1.1; `PlayerEgo::calcCollision` -0.3 |

## 4. Proven / disproven / unobservable

### 4.1 Disproven (the alternative moves away from the original)

These lower `byte_exact` and/or size-exact, or fail to build/link:

| family | variants | evidence |
|---|---|---|
| jump tables | `-fno-jump-tables` | 1390 / 4413 / 4437; `.text` `0x134908`; `Status::missionCompleted` -79.2 |
| sibling calls | `-fno-optimize-sibling-calls` | 1316 / 3592 / 3686; `.text` `0x1315b8` |
| builtins | `-fno-builtin` | 1350 / 3988 / 4119 |
| builtin memcpy/memset/malloc | `-fno-builtin-memcpy`, `-fno-builtin-memset`, `-fno-builtin-malloc` | 1359 / 1351 / 1490 byte_exact; not neutral |
| math errno / signed zeros | `-fno-math-errno`, `-fno-signed-zeros` | 1356 / 2355 |
| FP contraction | `-ffp-contract=on` | 2076 / 4473 / 4515; `LevelScript::process` -0.9 |
| zero-init in BSS | `-fno-zero-initialized-in-bss` | 1522 / 4492 / 4516 |
| thread-safe statics | `-fno-threadsafe-statics` | 1368 / 4497 / 4520 |
| `__cxa_atexit` | `-fno-use-cxa-atexit` | 1356 / 4502 / 4523 |
| strict aliasing | `-fno-strict-aliasing` | 1395 / 4349 / 4411; `Generator::createMission` -33.9 |
| overflow semantics | `-fwrapv`, `-fno-strict-overflow` | 1388 / 4464 / 4498 each |
| unaligned access | `-mno-unaligned-access` | 1367 / 4444 / 4471 |
| execute-only | `-mexecute-only` | 1299 / 3994 / 4081; `.text` `0x1211f4` |
| restrict IT | `-mrestrict-it` | 1271 / 3325 / 4191; 19/19 targets regress |
| thread model | `-mthread-model single` | 2085 / 4499 / 4523 |
| NaN semantics | `-fno-honor-nans` | 1437 / 4373 / 4480 |
| common symbols | `-fno-common` | link failure: duplicate `g_android_back_button_pressed` |
| external assembler | `-fno-integrated-as` | external `as` rejects `-mfpu=neon` |

### 4.2 Unobservable on this tree

These are exactly 4505/4505/4524, `.text` byte-identical to baseline, and
unchanged on all 19 targets:

`-mno-madd4`, `-mno-neg-immediates`, `-mno-execute-only`, `-fno-plt`,
`-fno-omit-frame-pointer`, `-fno-builtin-printf`, `-fno-builtin-sinf`,
`-fno-trapping-math`, `-fno-finite-math-only`, `-fno-rounding-math`,
`-ffp-contract=off`, `-ffp-contract=fast`, `-fmerge-all-constants`,
`-fno-merge-constants`, `-fno-sized-deallocation`, `-fvisibility=default`,
`-fno-delete-null-pointer-checks`, `-fno-lax-vector-conversions`,
`-mno-restrict-it`, `-mno-movt`, `-mno-dsp`, `-mnocrc`, `-mno-crypto`,
`-fexcess-precision=fast`, `-fno-unsafe-math-optimizations`,
`-fno-associative-math`, `-fno-honor-infinities`, `-fno-strict-enums`,
`-fno-short-enums`.

Some are accepted but ignored by clang 7.0.2:
`-fno-rounding-math`, `-fno-merge-constants`,
`-fno-delete-null-pointer-checks`, `-fexcess-precision=fast`.
Some are accepted but unused for this target:
`-mno-madd4`, `-mno-dsp`, `-mno-crypto`.

### 4.3 Unsupported by clang 7.0.2 / this toolchain

| flag | result |
|---|---|
| `-mvectorize-with-neon-quad` | unknown argument |
| `-fno-weak` | unknown argument |
| `-fno-semantic-interposition` | unknown argument |
| `-mno-fp16` | unknown argument |
| `-fno-excess-precision=fast` | unknown argument (the accepted `-fexcess-precision=fast` is ignored) |
| `-fno-integrated-as` | external assembler rejects `-mfpu=neon` |

## 5. Per-TU applicability

There is nothing to apply per-TU: no variant improves a non-exact function.
The only historically interesting cell, `Generator.cpp` +
`-fno-strict-aliasing`, is now a regression (`Generator::createMission`
84.7 → 50.8, -33.9) and the function is in that TU, so a per-TU build cannot
recover the old positive result. The same applies to the `ModStation.cpp`
builtin-memcpy/memset cells (-34.5 on both `ModStation::ModStation` C1/C2).

## 6. Reproduction

```sh
# baseline
cd /home/debian/GalaxyOnFire2/wt/w7003
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j64
cmake --build cmake-build-match --target verify -j64
cmake --build cmake-build-match --target check -j64

# 56-variant matrix (scratch; canonical build untouched)
python3 tools/flag_fingerprint.py --mode variants \
  --variants-file /dev/shm/w7003/variants_wave120.json \
  --workdir /dev/shm/w7003/flag_fingerprint \
  --jobs 1

# target percentages (official verify.disassemble/reconcile/score)
python3 /dev/shm/w7003/score_targets.py
```

Scratch artifacts (not tracked): `/dev/shm/w7003/variants_wave120.json`,
`/dev/shm/w7003/flag_fingerprint/`, `/dev/shm/w7003/matrix.json`,
`/dev/shm/w7003/target_scores.json`, `/dev/shm/w7003/score_targets.py`,
`/dev/shm/w7003/build_matrix.py`, `/dev/shm/w7003/relink_verify_all.py`.

## 7. Gate state

No source or build-flag change was landed; `tools/verify_baseline.json` and
`tools/sodiff_allow.txt` are untouched. The final `check` is:

```text
verify: byte 4505  linked 4505  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```
