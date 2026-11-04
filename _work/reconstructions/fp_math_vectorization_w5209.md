# FP / math / vectorization build-flag archaeology (w5209)

Status: research note, 2026-11-04. Worktree `wt/w5209`, branch `wave104-w5209`,
base `8c5fd3646`. Scratch is `/dev/shm/w5209`. **No `src/`, build-file, tool,
baseline or allowlist change is landed**; this commit contains this note only.

## 0. Bottom line

* Baseline is green: `byte_exact 4500/4524`, `linked_exact 4500/4524`,
  `size-exact 4524/4524`, `data_exact 843/843`, `imports 207`, lint `CLEAN`,
  drift `0`, `RATCHET PASS`.
* 22 global FP/math/vectorization flags were A/B-built and verified.
  **10 are exactly code-neutral** (the stripped `.so` is byte-identical to the
  baseline after zeroing the 20-byte build-id descriptor). **12 are worse**.
  **None raises `byte_exact` above 4500.**
* All 47 per-TU cells that actually change a non-exact target function were
  full-linked and measured. Every cell has `byte_exact <= 4500`, and **no
  non-exact target function becomes byte-exact**. The closest cells are 4499
  (`-fno-honor-nans` on `Gun.cpp` or `Radar.cpp`, one exact function lost).
* **There is no strict improvement. No code or build change is justified.**
  The original's reconstructed ndk-build `Android.mk` carries no FP/math
  flags, and the default build already reproduces the original's
  `.ARM.attributes` and 4500 function bodies.

## 1. Baseline and exact commands

```sh
cd /home/debian/GalaxyOnFire2/wt/w5209
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
cmake --build cmake-build-match --target drift -j6
```

Raw result:

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
=== DRIFT: 0 field(s) across 0 class(es) ===
```

Baseline hashes and section geometry (measured in this worktree):

```text
ours  sha256 80cdbc8eaa43337afe62ebec3cc013bb6a0353383eba3bedef0f4bac8b098f7e
orig  sha256 d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483
ours with the 20-byte build-id descriptor zeroed:
      sha256 bf30731a74bdbcac22f3c87c2d917d4179844164ee00e0a4df2ea3b404b09b47

.text        size 0x1337c8  addr 0x69440
.ARM.exidx   size 0x006f70  addr 0x19cc08
.ARM.extab   size 0x026298  addr 0x1a3b78
.rodata      size 0x041b67  addr 0x1c9e10
```

Differing bytes vs the original at this baseline (as reported by the same
section-diff code used by `tools/flag_fingerprint.py`):

| section | differing bytes |
|---|---:|
| `.text` | 100,113 |
| `.ARM.exidx` | 829 |
| `.ARM.extab` | 108,295 |
| `.note.gnu.build-id` | 20 |
| total | 209,257 |

(The older inline numbers in `docs/VALIDATION.md` / `docs/IDENTICALITY.md`
say `.text` 102,777 and `.ARM.exidx` 842 and omit `.ARM.extab`; the values
above are the current base-`8c5fd3646` measurement. This note does not change
the docs because this pass changes no code.)

## 2. Global A/B matrix

The harness is `tools/flag_fingerprint.py`. It writes a wrapper toolchain that
appends `GOF2_EXTRA_FLAGS` to both `CMAKE_CXX_FLAGS` and `CMAKE_C_FLAGS`, then
builds a fresh scratch tree, links the real `.so`, and runs the official
verifier. The canonical `cmake-build-match` is never modified.

```sh
cat > /dev/shm/w5209/variants.json <<'JSON'
[
 {"name":"ffast_math","flags":"-ffast-math"},
 {"name":"fno_math_errno","flags":"-fno-math-errno"},
 {"name":"fno_trapping_math","flags":"-fno-trapping-math"},
 {"name":"fno_signed_zeros","flags":"-fno-signed-zeros"},
 {"name":"freciprocal_math","flags":"-freciprocal-math"},
 {"name":"ffp_contract_off","flags":"-ffp-contract=off"},
 {"name":"ffp_contract_fast","flags":"-ffp-contract=fast"},
 {"name":"fno_vectorize","flags":"-fno-vectorize"},
 {"name":"fno_slp_vectorize","flags":"-fno-slp-vectorize"},
 {"name":"fno_unroll_loops","flags":"-fno-unroll-loops"},
 {"name":"unroll_threshold_0","flags":"-mllvm -unroll-threshold=0"},
 {"name":"unroll_threshold_1","flags":"-mllvm -unroll-threshold=1"},
 {"name":"unroll_threshold_5","flags":"-mllvm -unroll-threshold=5"},
 {"name":"unroll_threshold_10","flags":"-mllvm -unroll-threshold=10"},
 {"name":"fno_jump_tables","flags":"-fno-jump-tables"},
 {"name":"fno_builtin","flags":"-fno-builtin"},
 {"name":"fno_honor_nans","flags":"-fno-honor-nans"},
 {"name":"fno_honor_infinities","flags":"-fno-honor-infinities"},
 {"name":"fno_finite_math_only","flags":"-fno-finite-math-only"},
 {"name":"fno_associative_math","flags":"-fno-associative-math"},
 {"name":"fno_rounding_math","flags":"-fno-rounding-math"},
 {"name":"fno_unsafe_math_optimizations","flags":"-fno-unsafe-math-optimizations"}
]
JSON

python3 tools/flag_fingerprint.py --mode variants \
  --variants-file /dev/shm/w5209/variants.json \
  --workdir /dev/shm/w5209/fp_matrix --jobs 4 \
  --json /dev/shm/w5209/fp_matrix/results.json
```

Two variants (`-ffp-contract=fast`, `-fno-finite-math-only`) failed in the
first parallel run because every variant shares
`third_party/openssl/openssl-1.1.0l/_obj_gof2/sha256-armv4.S` and the perlasm
step rewrites that file concurrently. They were regenerated and retried
sequentially:

```sh
cd third_party/openssl/openssl-1.1.0l
perl crypto/sha/asm/sha256-armv4.pl linux32 _obj_gof2/sha256-armv4.S
perl crypto/armv4cpuid.pl linux32 _obj_gof2/armv4cpuid.S
cd /home/debian/GalaxyOnFire2/wt/w5209
python3 tools/flag_fingerprint.py --mode variants \
  --variants-file /dev/shm/w5209/variants.json \
  --variants ffp_contract_fast,fno_finite_math_only \
  --workdir /dev/shm/w5209/fp_matrix --jobs 1 \
  --json /dev/shm/w5209/fp_matrix/results_retry.json
```

### 2.1 Results

`same .so` means the variant's stripped `.so` is byte-identical to the
canonical `.so` after zeroing the 20-byte `.note.gnu.build-id` descriptor.
`changed` is the number of exported functions whose raw bytes differ from the
canonical build. `size` is `sum(osize == usize)` out of 4524.

| variant | flags | byte | linked | size | .text | changed | new exact | new inexact | same .so |
|---|---|---:|---:|---:|---:|---:|---:|---:|---|
| ffast_math | `-ffast-math` | 1319 | 4222 | 4369/4524 | 0x132998 | 3205 | 0 | 3181 | no |
| fno_math_errno | `-fno-math-errno` | 1356 | 4486 | 4514/4524 | 0x133638 | 3168 | 0 | 3144 | no |
| fno_trapping_math | `-fno-trapping-math` | 4500 | 4500 | 4524/4524 | 0x1337c8 | 0 | 0 | 0 | **yes** |
| fno_signed_zeros | `-fno-signed-zeros` | 2371 | 4489 | 4521/4524 | 0x133788 | 2150 | 0 | 2129 | no |
| freciprocal_math | `-freciprocal-math` | 1561 | 4402 | 4484/4524 | 0x133808 | 2963 | 0 | 2939 | no |
| ffp_contract_off | `-ffp-contract=off` | 4500 | 4500 | 4524/4524 | 0x1337c8 | 0 | 0 | 0 | **yes** |
| ffp_contract_fast | `-ffp-contract=fast` | 4500 | 4500 | 4524/4524 | 0x1337c8 | 0 | 0 | 0 | **yes** |
| fno_vectorize | `-fno-vectorize` | 4500 | 4500 | 4524/4524 | 0x1337c8 | 0 | 0 | 0 | **yes** |
| fno_slp_vectorize | `-fno-slp-vectorize` | 1484 | 4383 | 4408/4524 | 0x132ec8 | 3040 | 0 | 3016 | no |
| fno_unroll_loops | `-fno-unroll-loops` | 2883 | 4490 | 4520/4524 | 0x1337a8 | 1631 | 0 | 1617 | no |
| unroll_threshold_0 | `-mllvm -unroll-threshold=0` | 4500 | 4500 | 4524/4524 | 0x1337c8 | 0 | 0 | 0 | **yes** |
| unroll_threshold_1 | `-mllvm -unroll-threshold=1` | 2713 | 4495 | 4522/4524 | 0x1337a8 | 1802 | 0 | 1787 | no |
| unroll_threshold_5 | `-mllvm -unroll-threshold=5` | 2713 | 4495 | 4522/4524 | 0x1337a8 | 1802 | 0 | 1787 | no |
| unroll_threshold_10 | `-mllvm -unroll-threshold=10` | 2713 | 4495 | 4522/4524 | 0x1337a8 | 1802 | 0 | 1787 | no |
| fno_jump_tables | `-fno-jump-tables` | 1390 | 4408 | 4436/4524 | 0x1348e8 | 3134 | 0 | 3110 | no |
| fno_builtin | `-fno-builtin` | 1350 | 3985 | 4119/4524 | 0x1347b8 | 3174 | 0 | 3150 | no |
| fno_honor_nans | `-fno-honor-nans` | 1437 | 4370 | 4480/4524 | 0x1334c8 | 3087 | 0 | 3063 | no |
| fno_honor_infinities | `-fno-honor-infinities` | 4500 | 4500 | 4524/4524 | 0x1337c8 | 0 | 0 | 0 | **yes** |
| fno_finite_math_only | `-fno-finite-math-only` | 4500 | 4500 | 4524/4524 | 0x1337c8 | 0 | 0 | 0 | **yes** |
| fno_associative_math | `-fno-associative-math` | 4500 | 4500 | 4524/4524 | 0x1337c8 | 0 | 0 | 0 | **yes** |
| fno_rounding_math | `-fno-rounding-math` | 4500 | 4500 | 4524/4524 | 0x1337c8 | 0 | 0 | 0 | **yes** |
| fno_unsafe_math_optimizations | `-fno-unsafe-math-optimizations` | 4500 | 4500 | 4524/4524 | 0x1337c8 | 0 | 0 | 0 | **yes** |

The `linked_exact` column is from the harness's fast verifier for the rows
with thousands of changed functions; `byte_exact` and `size-exact` are
independent of the fast-verifier short-circuit. The 10 neutral rows were also
confirmed by the whole-file hash comparison above, which is stronger than any
per-function score.

**No row has `new exact > 0`.** Every non-neutral row also has
`new inexact > 0`.

## 3. Per-TU screen

The 24 non-exact functions live in 16 TUs:

```text
game/core/Globals.cpp
game/menu/MGame.cpp
game/menu/ModStation.cpp
game/menu/SpaceLounge.cpp
game/mission/Generator.cpp
game/mission/Status.cpp
game/ship/PlayerEgo.cpp
game/ui/HangarWindow.cpp
game/ui/Hud.cpp
game/ui/Layout.cpp
game/ui/MenuTouchWindow.cpp
game/weapons/Gun.cpp
game/weapons/Radar.cpp
game/weapons/RocketGun.cpp
game/world/Level.cpp
game/world/LevelScript.cpp
```

For each of those TUs and each of the 22 flags, the TU was recompiled with
the exact baseline `compile_commands.json` command plus the flag, and the
object's function bytes were compared with the baseline object. The screen
used `tools/flag_fingerprint.object_functions()` and is reproduced by
`/dev/shm/w5209/per_tu_screen.py`:

```python
cmd = shlex.split(entry['command'])
cmd[cmd.index('-o') + 1] = str(out)
cmd += shlex.split(flag)
subprocess.run(cmd, cwd=entry['directory'], check=True)
a = object_functions(baseline_obj)
b = object_functions(out)
changed = [n for n in a if n in b and a[n] != b[n]]
```

Only 8 flags changed any non-exact target function at object level:
`-ffast-math`, `-fno-signed-zeros`, `-freciprocal-math`,
`-fno-slp-vectorize`, `-fno-unroll-loops`, `-fno-jump-tables`,
`-fno-builtin`, `-fno-honor-nans`. The other 14 flags changed **no** target
function in any of the 16 TUs; a per-TU application of those flags therefore
cannot improve a non-exact function.

### 3.1 Full-link measurement

For every one of the 47 cells in which a target function changed, the TU's
object was swapped into a neutral full build and the `.so` was relinked with
the exact gold command extracted from the build:

```sh
ninja -C /dev/shm/w5209/fp_matrix/build-ffp_contract_off -t commands link_so
# run the line containing ld.gold with bash -c, cwd = that build dir
```

The manual link command is necessary: `ninja link_so` recompiles a swapped
object because ninja's stored deps hash no longer matches the object mtime
(`ninja -d explain` says `stored deps info out of date ... is dirty`). The
manual gold command uses the swapped object directly. `byte_exact` was then
computed by the same raw-byte comparison the official verifier uses
(`orig_blob == ours_blob` for every exported function). The method was
validated on one cell against the official verifier:

```text
-fno-honor-nans on game/weapons/Gun.cpp
blob compare:  byte_exact 4499
verify.py:     byte_exact 4499  linked_exact 4499  size-exact 4524/4524
```

### 3.2 Results

`exact changed` is the number of already-exact functions in that TU whose
object bytes changed (so the cell is not a clean single-function change).
`target byte-exact?` is the full-link raw-byte result for the non-exact
target. `target pct` is filled only for the 10 cells with `exact changed = 0`
(these are the only cells that could have been a clean gain); all of them get
worse.

| flag | TU | target function(s) | exact changed | full-link byte | target byte-exact? | target pct before -> after |
|---|---|---|---:|---:|---|---|
| `-ffast-math` | game/core/Globals.cpp | `Globals::Globals` C1/C2 | 4 | 2668 | no | |
| `-ffast-math` | game/menu/MGame.cpp | `MGame::OnUpdate` | 7 | 3095 | no | |
| `-ffast-math` | game/mission/Generator.cpp | `Generator::createMission` | 2 | 2035 | no | |
| `-ffast-math` | game/ship/PlayerEgo.cpp | `PlayerEgo::calcCollision` | 26 | 2244 | no | |
| `-ffast-math` | game/ui/Hud.cpp | `Hud::draw` | 3 | 2939 | no | |
| `-ffast-math` | game/ui/MenuTouchWindow.cpp | `MenuTouchWindow` C1/C2 | 4 | 3610 | no | |
| `-ffast-math` | game/weapons/Gun.cpp | `Gun::calcCharacterCollision` | 3 | 3800 | no | |
| `-ffast-math` | game/weapons/Radar.cpp | `Radar::draw` | 4 | 4496 | no | |
| `-ffast-math` | game/weapons/RocketGun.cpp | `RocketGun::update` | 0 | 3994 | no | 86.9 -> 79.7 |
| `-ffast-math` | game/world/Level.cpp | `Level::createScene`, `Level::createMission` | 12 | 2357 | no | |
| `-ffast-math` | game/world/LevelScript.cpp | `LevelScript::process` | 0 | 2819 | no | 53.4 -> 52.3 |
| `-fno-signed-zeros` | game/world/Level.cpp | `Level::createMission` | 0 | 2501 | no | 92.0 -> 81.3 |
| `-freciprocal-math` | game/menu/MGame.cpp | `MGame::OnUpdate` | 3 | 4497 | no | |
| `-freciprocal-math` | game/mission/Generator.cpp | `Generator::createMission` | 2 | 4390 | no | |
| `-freciprocal-math` | game/ui/Hud.cpp | `Hud::draw` | 1 | 4032 | no | |
| `-freciprocal-math` | game/weapons/Radar.cpp | `Radar::draw` | 2 | 4498 | no | |
| `-freciprocal-math` | game/weapons/RocketGun.cpp | `RocketGun::update` | 0 | 4495 | no | 86.9 -> 81.1 |
| `-freciprocal-math` | game/world/Level.cpp | `Level::createScene`, `Level::createMission` | 3 | 2357 | no | |
| `-freciprocal-math` | game/world/LevelScript.cpp | `LevelScript::process` | 0 | 4322 | no | 53.4 -> 52.9 |
| `-fno-slp-vectorize` | game/core/Globals.cpp | `Globals::Globals` C1/C2 | 1 | 2664 | no | |
| `-fno-slp-vectorize` | game/menu/MGame.cpp | `MGame::OnUpdate` | 2 | 4269 | no | |
| `-fno-slp-vectorize` | game/menu/ModStation.cpp | `ModStation` C1/C2 | 1 | 2604 | no | |
| `-fno-slp-vectorize` | game/ui/Layout.cpp | `Layout::Layout` C1/C2 | 0 | 2454 | no | 47.8 -> 17.4 (both) |
| `-fno-slp-vectorize` | game/ui/MenuTouchWindow.cpp | `MenuTouchWindow` C1/C2 | 0 | 2672 | no | 95.1 -> 82.8 (both) |
| `-fno-slp-vectorize` | game/world/Level.cpp | `Level::createMission` | 6 | 2357 | no | |
| `-fno-slp-vectorize` | game/world/LevelScript.cpp | `LevelScript::process` | 1 | 2831 | no | |
| `-fno-unroll-loops` | game/world/Level.cpp | `Level::createScene` | 1 | 4036 | no | |
| `-fno-jump-tables` | game/mission/Generator.cpp | `Generator::createMission` | 0 | 2615 | no | 77.5 -> 64.4 |
| `-fno-jump-tables` | game/mission/Status.cpp | `Status::missionCompleted` | 2 | 1840 | no | |
| `-fno-jump-tables` | game/ui/HangarWindow.cpp | `HangarWindow::render` | 1 | 2770 | no | |
| `-fno-jump-tables` | game/ui/Hud.cpp | `Hud::draw` | 2 | 2939 | no | |
| `-fno-jump-tables` | game/ui/MenuTouchWindow.cpp | `MenuTouchWindow::OnTouchBegin` | 4 | 2689 | no | |
| `-fno-jump-tables` | game/world/Level.cpp | `Level::createShip`, `createScene`, `createMission` | 6 | 1369 | no | |
| `-fno-jump-tables` | game/world/LevelScript.cpp | `LevelScript::process` | 0 | 2729 | no | 53.4 -> 33.0 |
| `-fno-builtin` | game/menu/MGame.cpp | `MGame::OnTouchEnd` | 6 | 3120 | no | |
| `-fno-builtin` | game/menu/ModStation.cpp | `ModStation` C1/C2 | 12 | 2478 | no | |
| `-fno-builtin` | game/ui/MenuTouchWindow.cpp | `MenuTouchWindow` C1/C2 | 13 | 2672 | no | |
| `-fno-builtin` | game/world/Level.cpp | `Level::createScene`, `createMission` | 34 | 2345 | no | |
| `-fno-builtin` | game/world/LevelScript.cpp | `LevelScript::process` | 5 | 3683 | no | |
| `-fno-honor-nans` | game/core/Globals.cpp | `Globals::Globals` C1/C2 | 3 | 3412 | no | |
| `-fno-honor-nans` | game/menu/MGame.cpp | `MGame::OnUpdate` | 6 | 4275 | no | |
| `-fno-honor-nans` | game/ship/PlayerEgo.cpp | `PlayerEgo::calcCollision` | 19 | 2099 | no | |
| `-fno-honor-nans` | game/ui/Hud.cpp | `Hud::draw` | 2 | 4031 | no | |
| `-fno-honor-nans` | game/weapons/Gun.cpp | `Gun::calcCharacterCollision` | 1 | 4499 | no | |
| `-fno-honor-nans` | game/weapons/Radar.cpp | `Radar::draw` | 1 | 4499 | no | |
| `-fno-honor-nans` | game/world/Level.cpp | `Level::createMission` | 7 | 4475 | no | |
| `-fno-honor-nans` | game/world/LevelScript.cpp | `LevelScript::process` | 0 | 2738 | no | 53.4 -> 53.1 |

Why a one-function change can drop `byte_exact` by hundreds: `byte_exact`
compares raw bytes. Changing the size of a function shifts every later
function in `.text`; PC-relative `bl`/`b`/literal-pool operands then change in
otherwise untouched functions, so they are no longer raw-byte-identical. The
official verifier's `linked_exact` normalizes those addresses and stays at
4500 in all 10 measured cells, but the gate's primary hard count is
`byte_exact`, and it drops.

## 4. Why each neutral flag is neutral

The NDK r18b clang 7.0.2 driver was probed with `-###`; the relevant cc1
arguments are:

```text
default:                    -fmath-errno -vectorize-slp
-ffast-math:                -menable-no-infs -menable-no-nans -menable-unsafe-fp-math
                            -fno-signed-zeros -mreassociate -freciprocal-math
                            -fno-trapping-math -ffp-contract=fast -ffast-math
                            -ffinite-math-only -vectorize-slp
-fno-math-errno:            -vectorize-slp
-fno-trapping-math:         -fmath-errno -fno-trapping-math -vectorize-slp
-fno-signed-zeros:          -fmath-errno -fno-signed-zeros -vectorize-slp
-freciprocal-math:          -fmath-errno -freciprocal-math -vectorize-slp
-ffp-contract=off:          -fmath-errno -ffp-contract=off -vectorize-slp
-ffp-contract=fast:         -fmath-errno -ffp-contract=fast -vectorize-slp
-fno-vectorize:             -fmath-errno -vectorize-slp        (no change)
-fno-slp-vectorize:         -fmath-errno
-fno-unroll-loops:          -fmath-errno -fno-unroll-loops -vectorize-slp
-mllvm -unroll-threshold=0: -fmath-errno -vectorize-slp -mllvm -unroll-threshold=0
-fno-jump-tables:           -fno-jump-tables -fmath-errno -vectorize-slp
-fno-builtin:               -fno-builtin -fmath-errno -vectorize-slp
-fno-honor-nans:            -menable-no-nans -fmath-errno -vectorize-slp
-fno-honor-infinities:      -menable-no-infs -fmath-errno -vectorize-slp
-fno-finite-math-only:      -fmath-errno -vectorize-slp        (no change)
-fno-associative-math:      -fmath-errno -vectorize-slp        (no change)
-fno-rounding-math:         -fmath-errno -vectorize-slp        (no change)
-fno-unsafe-math-optimizations: -fmath-errno -vectorize-slp    (no change)
```

The neutral flags fall into three groups:

1. **Compiler defaults / no-op on this driver.**
   `-fno-finite-math-only`, `-fno-associative-math`, `-fno-rounding-math`,
   `-fno-unsafe-math-optimizations` do not even appear on the cc1 line; the
   default is already the `-fno-...` form. They cannot change code by
   construction.
2. **No matching hardware/operation.**
   `-ffp-contract=off` and `-ffp-contract=fast` are both neutral because the
   target is `Tag_FP_arch: VFPv3` / `Tag_Advanced_SIMD_arch: NEONv1` with no
   fused multiply-add. The original and our build have byte-identical
   `.ARM.attributes`; there is no FMA instruction to contract into.
   `-fno-honor-infinities` adds `-menable-no-infs`, but the source has no
   `isinf`/`INFINITY` observation (`grep -rhoE '\b(isnan|isinf)\s*\(' src`
   is empty), so the change is not observable.
   `-fno-trapping-math` adds the flag but no source operation is sensitive to
   FP trapping (no FP exception handlers or `fetestexcept`).
3. **Pass already idle at `-Oz`.**
   `-fno-vectorize` is neutral because clang's cc1 line at `-Oz` already
   carries only `-vectorize-slp` (the loop vectorizer is not on); disabling it
   changes nothing. `-mllvm -unroll-threshold=0` is neutral, while
   `-fno-unroll-loops` is worse and thresholds 1/5/10 are worse, so the
   unroll-threshold knob is not the mechanism that matters on this source.

The observable-but-worse flags are the opposite: `-fno-math-errno` removes
the default `-fmath-errno` and drops to 1356; `-fno-honor-nans` adds
`-menable-no-nans` and drops to 1437; `-fno-signed-zeros`,
`-freciprocal-math`, `-ffast-math`, `-fno-slp-vectorize`,
`-fno-unroll-loops`, `-fno-jump-tables` and `-fno-builtin` all change real
code and all lose exact functions.

## 5. Proven / disproven / unobservable

### Positively proven

1. The base-`8c5fd3646` gate is 4500/4500 byte/linked, 4524/4524 size,
   lint `CLEAN`, drift 0.
2. **No tested global FP/math/vectorization flag raises `byte_exact` above
   4500.** 22 flags, one at a time, full link + official verifier.
3. **Ten flags are exactly code-neutral**: `-fno-trapping-math`,
   `-ffp-contract=off`, `-ffp-contract=fast`, `-fno-vectorize`,
   `-mllvm -unroll-threshold=0`, `-fno-honor-infinities`,
   `-fno-finite-math-only`, `-fno-associative-math`,
   `-fno-rounding-math`, `-fno-unsafe-math-optimizations`. Their stripped
   `.so` hashes are `bf30731a...` after zeroing the build-id, identical to
   the baseline.
4. **Twelve flags are worse**, with the counts in §2.1.
5. **No per-TU application produces a strict improvement.** 47 cells
   full-linked; all `byte_exact <= 4500`; no target becomes byte-exact. The
   best cells are 4499 (one exact function lost).
6. The original's reconstructed ndk-build `Android.mk` has no FP/math flags
   (`_work/reconstructions/android_mk_reconstruction.md`), and the original
   `.ARM.attributes` is identical to the default build's (`VFPv3`, `NEONv1`,
   `Tag_ABI_FP_denormal: Needed`, `Tag_ABI_FP_exceptions: Needed`,
   `Tag_ABI_FP_number_model: IEEE 754`). The default build already matches
   4500 function bodies.

### Disproven

* `-ffast-math` (1319), `-fno-math-errno` (1356), `-fno-signed-zeros`
  (2371), `-freciprocal-math` (1561), `-fno-honor-nans` (1437),
  `-fno-slp-vectorize` (1484), `-fno-unroll-loops` (2883),
  `-fno-jump-tables` (1390), `-fno-builtin` (1350) as original global flags.
* `-fno-vectorize`, `-ffp-contract=off/fast`, `-mllvm -unroll-threshold=0`
  as *distinguishing* original flags: they are neutral, so the final binary
  cannot tell whether the original passed them.
* Any per-TU flag on the 16 non-exact TUs as a way to raise `byte_exact`:
  every measured cell loses exact functions and none makes its target exact.

### Unobservable / not tested

* The 10 neutral flags are unobservable in the shipped stripped `.so`; the
  original could have passed any of them. The reconstructed default
  ndk-build recipe suggests it did not pass them explicitly.
* `-fno-math-errno` etc. on TUs other than the 16 target TUs: they cannot
  make a non-exact function exact (no non-exact function lives there) and can
  only lose exact functions, so they were not full-linked. The object-level
  screen already shows the target functions do not change.
* Combinations of flags were not exhaustively searched. `-ffast-math` is the
  main bundle and is much worse. A combination could be neutral (like the
  individual neutral flags) but cannot raise `byte_exact` above 4500; it
  would only be an alternative neutral recipe.
* Per-TU flags on the exact TUs were not full-linked for the same reason:
  no gain is possible, only loss.

## 6. Confidence table

| claim | confidence | hard evidence |
|---|---|---|
| Baseline 4500/4500, 4524 size, lint clean, drift 0 | high | `check` + `drift` output in §1 |
| No global flag raises byte_exact | high | 22-variant matrix, official verifier, `new exact = 0` in every row |
| 10 flags are exactly code-neutral | high | whole-`.so` hash `bf30731a...` after zeroing the 20-byte build-id; `changed = 0` |
| 12 flags are worse | high | byte_exact 1319–2883, `new exact = 0`, `new inexact > 0` |
| No per-TU strict improvement | high | 47 full-link cells; blob compare validated against `verify.py` on one cell (4499 = 4499) |
| Per-TU target pct gets worse in the 10 clean cells | high | official `verify.py` full run: 86.9→79.7, 53.4→52.3, 92.0→81.3, 86.9→81.1, 53.4→52.9, 47.8→17.4, 95.1→82.8, 77.5→64.4, 53.4→33.0, 53.4→53.1 |
| `-ffp-contract=off/fast` neutral because no FMA | high | `.ARM.attributes` VFPv3/NEONv1 identical to original; cc1 accepts both flags; code identical |
| `-fno-vectorize` neutral because loop vectorizer is off at `-Oz` | high | default cc1 has only `-vectorize-slp`; `-fno-slp-vectorize` is worse (1484) |
| `-fno-finite-math-only` / `-fno-associative-math` / `-fno-rounding-math` / `-fno-unsafe-math-optimizations` are defaults | high | cc1 diff shows no change |
| Original used default FP/math flags | medium-high | reconstructed `Android.mk` has none; `.ARM.attributes` and 4500 bodies match the default build; the neutral flags are unobservable |

## 7. Strict improvement?

**No.** There is no strict improvement in `byte_exact`, `linked_exact`,
`size_exact`, `data_exact`, imports, lint or drift from any tested global or
per-TU FP/math/vectorization flag. The minimal change is therefore **no
change**: this commit adds only this note. The canonical build remains at
4500/4500 byte/linked, 4524/4524 size, lint `CLEAN`, drift 0, and the
baseline is not updated.

## 8. Reproduction artifacts

All scratch artifacts are under `/dev/shm/w5209`:

```text
variants.json                 the 22-variant spec
fp_matrix/results.json        first global run (20 valid rows)
fp_matrix/results_retry.json  the two retried rows
fp_matrix/build-*/            per-variant build dirs + libgof2hdaa.so
per_tu_screen.py              object-level per-TU screen
per_tu_screen.json            352 object-level cells
per_tu_blob2.py               full-link per-TU screen (47 cells)
per_tu_blob2.json             47 full-link byte_exact results
per_tu_full10.py              official-verifier run for the 10 clean cells
per_tu_full10.json            target pct/linked results for those 10 cells
per_tu_blob2.log, per_tu_full10.log
```

The canonical `cmake-build-match` was re-run after all experiments and is
green. The shared `third_party/openssl/openssl-1.1.0l/_obj_gof2/sha256-armv4.S`
was regenerated once after the parallel-variant race; the canonical
`libcrypto_gof2.a` is the same content because the libcrypto recipe does not
consume `GOF2_EXTRA_FLAGS`.
