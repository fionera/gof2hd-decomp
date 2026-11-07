# Per-TU build-flag matrix, wave 132 (w7721)

Status: note-only research commit, 2026-11-07. Worktree
`/home/debian/GalaxyOnFire2/wt/w7721`, base `70e451ffe` ("PlayerJumpgate ctor
extab wave 131: lifetime-marker scopes"). Scratch is `/dev/shm/w7721`.
**No `src/`, `tools/`, `CMakeLists.txt`, baseline, allowlist or doc change is
landed.** This note is the only commit.

The question: the build recipe is proven *globally* (NDK r18b / clang 7.0.2 /
`-Oz` / no `-fdata-sections` / gold 1.12), but the original could still have
used **per-TU** flags. This note systematically tests per-TU alternatives on
the worst residual functions: `-O2`/`-O3`/`-Os`, `-fno-exceptions`,
`-fno-rtti`, `-fno-threadsafe-statics`, `-fomit-frame-pointer` /
`-fno-omit-frame-pointer`, and clang 7.0.0 / 7.0.1 vs 7.0.2.

Gate at the base (fresh `cmake --build cmake-build-match --target check`):

```text
verify: byte 4506/4524  linked 4506/4524  size 4524/4524  avg 99.94
data:   exact 843/843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0  imports 207
symbol parity 5370/5370 (0 extra / 0 missing)
lint CLEAN ✓   drift 0
RATCHET PASS
```

Section deltas at this HEAD (fresh measurement, canonical build): `.text`
**85,379** differing bytes (size exact 1,259,464), `.ARM.extab` **1,420**
(size exact 156,312), `.note.gnu.build-id` **20**. `.rodata`, `.data`, `.bss`,
`.ARM.exidx`, `.rel.plt` are exact. This matches `docs/VALIDATION.md`.

Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`,
2,187,328 B, `sha256 d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`.

---

## 0. Bottom line

1. **No per-TU flag improves any residual function.** Across 56 cells (7 TUs ×
   8 variants) plus 12 secondary cells, no variant raises a single residual
   percentage. The only variants that are ever neutral are `-fno-rtti`,
   `-fno-threadsafe-statics` and `-fno-omit-frame-pointer`; they leave both
   `byte_exact` (4506) and every residual pct exactly unchanged on the TUs
   where they change no codegen (`-fno-rtti` is negative on MGame and
   ModStation; see item 5).
2. **`-O2`/`-O3`/`-Os` are strongly negative on every tested TU.** The worst
   functions fall from 48.9/59.9/70.6/71.0/75.4/82.0/92.3/93.4 to
   13.2–64.1. `-Oz` is not a global accident; it is the right level for these
   TUs too.
3. **`-fno-exceptions` is negative for every TU that contains cleanup code**
   (Level, Radar, MGame, LevelScript) and neutral for the target function in
   Layout, Status and Gun (it still changes other code in those TUs). The original's LSDA/`.ARM.extab` records
   require exceptions on.
4. **`-fomit-frame-pointer` is negative everywhere.** The original keeps frame
   pointers (consistent with `-g` and the wave-127 finding).
5. **`-fno-rtti` is neutral on 11 of 13 tested TUs but negative on `MGame.cpp`
   (byte_exact 4506 → 2929) and `ModStation.cpp` (4506 → 1742)** because those
   TUs emit RTTI-bearing code; it never helps a residual function.
6. **All NDK r18 variants ship the same Android clang 7.0.2 build.** NDK
   r18-beta1 (`18.0.4912477-beta1`), r18 (`18.0.5002713`) and r18b
   (`18.1.5063045`) all print `Android (4751641 based on r328903) clang version
   7.0.2` — the exact build in the original's `.comment`. No NDK ships 7.0.0
   or 7.0.1.
7. **Upstream clang 7.0.0 and 7.0.1 are codegen-identical to each other on
   this tree** (only `StarSystem::StarSystem` C1/C2 differ, by 6 bytes), and
   both are far worse than the Android 7.0.2 build (byte_exact 1372 vs 4506).
   This is confounded by Android's LLVM patches, so it is *not* a clean
   version test; it does show that a 7.0.0/7.0.1 substitution cannot be the
   missing ingredient.
8. **A methodological caveat:** `byte_exact` is a *link-layout* metric, not a
   clean per-TU metric. Raw-byte equality includes literal-pool absolute
   addresses; any TU whose function sizes change shifts every later `.text`
   function and breaks raw equality for unrelated functions. That is why a
   one-TU `-O2` drops `byte_exact` to ~1350 while the target pcts are the
   meaningful per-TU signal. The full gate is still reported for every cell.

---

## 1. Method

### 1.1 Per-TU compile launcher

A scratch build (`/dev/shm/w7721/build`) was configured with the stock toolchain
and a compiler launcher that appends `$GOF2_TU_FLAGS` only when compiling
`$GOF2_TU`:

```sh
#!/bin/sh
src=""
for a in "$@"; do
  case "$a" in *.cpp|*.cc|*.cxx|*.c) src="$a" ;; esac
done
if [ -n "$GOF2_TU" ] && [ -n "$GOF2_TU_FLAGS" ] && [ "$(basename "$src")" = "$GOF2_TU" ]; then
  exec "$@" $GOF2_TU_FLAGS
fi
exec "$@"
```

For each cell the seven target-TU objects were deleted, the tree was rebuilt
(`link_so -j6`, ~4 s), and the resulting `.so` was scored. Flags are appended
after the canonical flags, so `-O2`/`-O3`/`-Os` *replace* `-Oz` (clang uses the
last `-O` flag) and `-fno-*` overrides the earlier positive form.

### 1.2 Fast scorer

`tools/verify/verify.py` takes ~20 s for all 4,524 functions. A small wrapper
(`/dev/shm/w7721/score.py`) imports its `Elf32`/`Resolver`/`disassemble`/
`reconcile`/`score` machinery and computes:
* full `byte_exact` over all compared symbols (pure byte comparison, no
  disassembly), and
* the official fuzzy pct for the requested residual symbols.

It reproduces the official verifier exactly: the scratch canonical build and
`cmake-build-match` differ only in the 20-byte build-id, and both score
`byte_exact 4506` with identical per-function pcts.

### 1.3 Baseline residual table (fresh canonical build)

| function | TU | pct | size |
|---|---|---:|---:|
| `Layout::Layout` C1/C2 | Layout.cpp | 48.9 | 7072 |
| `LevelScript::process` | LevelScript.cpp | 59.9 | 57232 |
| `Level::createShip` | Level.cpp | 70.6 | 2764 |
| `Status::missionCompleted` | Status.cpp | 71.0 | 1432 |
| `Radar::draw` | Radar.cpp | 75.4 | 9908 |
| `MGame::OnTouchEnd` | MGame.cpp | 82.0 | 8080 |
| `Level::createMission` | Level.cpp | 92.3 | 17172 |
| `Gun::calcCharacterCollision` | Gun.cpp | 93.4 | 1984 |
| `HangarWindow::render` | HangarWindow.cpp | 94.7 | 9700 |
| `SpaceLounge::drawLounge` | SpaceLounge.cpp | 95.7 | 1948 |
| `Level::createScene` | Level.cpp | 96.5 | 2252 |
| `Hud::draw` | Hud.cpp | 96.6 | 9332 |
| `Generator::createMission` | Generator.cpp | 97.2 | 1996 |
| `MenuTouchWindow::OnTouchBegin` | MenuTouchWindow.cpp | 98.1 | 1896 |
| `ModStation::ModStation` C1/C2 | ModStation.cpp | 99.2 | 1008 |
| `MGame::OnUpdate` | MGame.cpp | 99.9 | 11440 |

Note: the checked-in repo-root `report.json` is one commit stale for four of
these (`LevelScript::process` 60.1, `Level::createShip` 70.5,
`MGame::OnTouchEnd` 81.9, `Generator::createMission` 97.2 in the docs/report;
a fresh verify gives 59.9 / 70.6 / 82.0 / 97.2). The gate counts are unchanged;
only the rounded pct snapshots differ.

---

## 2. The per-TU matrix (worst 8 functions, 7 TUs, 8 variants)

Every cell is a full link of the whole tree with only the named TU recompiled
with the named flag. `byte_exact` is the whole-gate count (canonical 4506);
the remaining columns are the official per-function pct.

### Layout.cpp

| variant | byte_exact | Layout C1 | Layout C2 |
|---|---:|---:|---:|
| `-O2` | 1369 | 44.3 | 44.3 |
| `-O3` | 1369 | 22.9 | 22.9 |
| `-Os` | 1506 | 47.6 | 47.6 |
| `-fno-exceptions` | 1369 | 48.9 | 48.9 |
| `-fno-rtti` | 4506 | 48.9 | 48.9 |
| `-fno-threadsafe-statics` | 4506 | 48.9 | 48.9 |
| `-fomit-frame-pointer` | 2446 | 42.0 | 42.0 |
| `-fno-omit-frame-pointer` | 4506 | 48.9 | 48.9 |

### LevelScript.cpp

| variant | byte_exact | process |
|---|---:|---:|
| `-O2` | 1443 | 25.1 |
| `-O3` | 1369 | 14.1 |
| `-Os` | 1378 | 28.7 |
| `-fno-exceptions` | 2049 | 35.4 |
| `-fno-rtti` | 4506 | 59.9 |
| `-fno-threadsafe-statics` | 4506 | 59.9 |
| `-fomit-frame-pointer` | 2728 | 37.4 |
| `-fno-omit-frame-pointer` | 4506 | 59.9 |

### Level.cpp

| variant | byte_exact | createShip | createMission | createScene |
|---|---:|---:|---:|---:|
| `-O2` | 1351 | 23.8 | 13.3 | 50.4 |
| `-O3` | 1351 | 23.7 | 30.0 | 38.3 |
| `-Os` | 1353 | 35.0 | 40.3 | 51.6 |
| `-fno-exceptions` | 1369 | 23.4 | 33.5 | 53.8 |
| `-fno-rtti` | 4506 | 70.6 | 92.3 | 96.5 |
| `-fno-threadsafe-statics` | 4506 | 70.6 | 92.3 | 96.5 |
| `-fomit-frame-pointer` | 2339 | 44.2 | 58.2 | 64.1 |
| `-fno-omit-frame-pointer` | 4506 | 70.6 | 92.3 | 96.5 |

### Status.cpp

| variant | byte_exact | missionCompleted |
|---|---:|---:|
| `-O2` | 1351 | 21.1 |
| `-O3` | 1351 | 13.2 |
| `-Os` | 1365 | 22.4 |
| `-fno-exceptions` | 1498 | 71.0 |
| `-fno-rtti` | 4506 | 71.0 |
| `-fno-threadsafe-statics` | 4506 | 71.0 |
| `-fomit-frame-pointer` | 2285 | 25.7 |
| `-fno-omit-frame-pointer` | 4506 | 71.0 |

### Radar.cpp

| variant | byte_exact | draw |
|---|---:|---:|
| `-O2` | 1377 | 13.5 |
| `-O3` | 1470 | 30.6 |
| `-Os` | 1462 | 24.6 |
| `-fno-exceptions` | 1369 | 47.2 |
| `-fno-rtti` | 4506 | 75.4 |
| `-fno-threadsafe-statics` | 4506 | 75.4 |
| `-fomit-frame-pointer` | 2690 | 42.7 |
| `-fno-omit-frame-pointer` | 4506 | 75.4 |

### MGame.cpp

| variant | byte_exact | OnTouchEnd | OnUpdate |
|---|---:|---:|---:|
| `-O2` | 1355 | 30.5 | 29.1 |
| `-O3` | 1355 | 25.3 | 28.8 |
| `-Os` | 2201 | 64.1 | 63.5 |
| `-fno-exceptions` | 1369 | 48.9 | 63.7 |
| `-fno-rtti` | 2929 | 82.0 | 99.9 |
| `-fno-threadsafe-statics` | 4506 | 82.0 | 99.9 |
| `-fomit-frame-pointer` | 3084 | 38.5 | 58.1 |
| `-fno-omit-frame-pointer` | 4506 | 82.0 | 99.9 |

### Gun.cpp

| variant | byte_exact | calcCharCollision |
|---|---:|---:|
| `-O2` | 1377 | 61.8 |
| `-O3` | 1356 | 61.6 |
| `-Os` | 1365 | 62.8 |
| `-fno-exceptions` | 1369 | 93.4 |
| `-fno-rtti` | 4506 | 93.4 |
| `-fno-threadsafe-statics` | 4506 | 93.4 |
| `-fomit-frame-pointer` | 2871 | 44.6 |
| `-fno-omit-frame-pointer` | 4506 | 93.4 |

**No cell beats the canonical pct.** The best possible outcome is exact
neutrality (`-fno-rtti`, `-fno-threadsafe-statics`, `-fno-omit-frame-pointer`),
which also keeps `byte_exact` at 4506.

### 2.1 Secondary residual TUs (neutral variants only)

The remaining six residual TUs were tested with the two flags that were
neutral on the worst-8 TUs. All 12 cells keep `byte_exact = 4506` and every
residual pct unchanged, except `ModStation.cpp -fno-rtti` (`byte_exact 1742`,
pcts unchanged):

| TU | `-fno-rtti` byte_exact | `-fno-threadsafe-statics` byte_exact | residual pct |
|---|---:|---:|---:|
| HangarWindow.cpp | 4506 | 4506 | 94.7 |
| SpaceLounge.cpp | 4506 | 4506 | 95.7 |
| Hud.cpp | 4506 | 4506 | 96.6 |
| Generator.cpp | 4506 | 4506 | 97.2 |
| MenuTouchWindow.cpp | 4506 | 4506 | 98.1 |
| ModStation.cpp | **1742** | 4506 | 99.2 |

---

## 3. Clang 7.0.0 / 7.0.1 vs 7.0.2

### 3.1 What the NDK ships

| package | `Pkg.Revision` | `clang --version` |
|---|---|---|
| NDK r18-beta1 | `18.0.4912477-beta1` | `Android (4751641 based on r328903) clang version 7.0.2` |
| NDK r18 | `18.0.5002713` | same `4751641 / 7.0.2` |
| NDK r18b (original) | `18.1.5063045` | same `4751641 / 7.0.2` |

All three r18 packages contain the **same** Android clang 7.0.2 build. The
original's `.comment` names exactly this build (`4751641`, LLVM
`1d739ffb...`). There is no NDK that ships clang 7.0.0 or 7.0.1.

### 3.2 Upstream LLVM 7.0.0 / 7.0.1

To test the version axis anyway, the upstream prebuilt releases were used:

* `clang+llvm-7.0.0-x86_64-linux-gnu-ubuntu-16.04.tar.xz`
* `clang+llvm-7.0.1-x86_64-linux-gnu-ubuntu-16.04.tar.xz`

The whole tree was compiled with each (C++ TUs swapped to the upstream driver
with `-nostdinc++` so the NDK libc++ headers stay in use; C TUs stayed on the
NDK clang because upstream's C mode trips a tentative-definition merge in
`BackButton.c`/`GOF2HD2012.c`), then linked with the NDK gold.

| build | byte_exact | .so size | notes |
|---|---:|---:|---|
| NDK Android clang 7.0.2 (canonical) | **4506** | 2,187,328 | gate |
| upstream clang 7.0.0 | 1372 | 2,187,340 | +12 B |
| upstream clang 7.0.1 | 1372 | 2,187,340 | +12 B |

The two upstream builds differ from each other by **only 6 bytes of `.text`**
(`StarSystem::StarSystem` C1/C2) plus the 20-byte build-id. Their residual
pcts are identical:

| function | canonical 7.0.2 | upstream 7.0.0 | upstream 7.0.1 |
|---|---:|---:|---:|
| `Layout::Layout` C1/C2 | 48.9 | 48.9 | 48.9 |
| `LevelScript::process` | 59.9 | 52.3 | 52.3 |
| `Level::createShip` | 70.6 | 70.6 | 70.6 |
| `Status::missionCompleted` | 71.0 | 71.0 | 71.0 |
| `Radar::draw` | 75.4 | 68.4 | 68.4 |
| `MGame::OnTouchEnd` | 82.0 | 65.7 | 65.7 |
| `Level::createMission` | 92.3 | 53.2 | 53.2 |
| `Gun::calcCharacterCollision` | 93.4 | 93.4 | 93.4 |
| `HangarWindow::render` | 94.7 | 53.4 | 53.4 |
| `SpaceLounge::drawLounge` | 95.7 | 86.8 | 86.8 |
| `Level::createScene` | 96.5 | 73.1 | 73.1 |
| `Hud::draw` | 96.6 | 88.8 | 88.8 |
| `Generator::createMission` | 97.2 | 52.9 | 52.9 |
| `MenuTouchWindow::OnTouchBegin` | 98.1 | 74.2 | 74.2 |
| `ModStation::ModStation` C1 | 99.2 | 91.6 | 91.6 |
| `MGame::OnUpdate` | 99.9 | 79.1 | 79.1 |

**Interpretation.** This is not a clean 7.0.0-vs-7.0.2 test: the upstream
release lacks the Android LLVM patches that the NDK clang carries, and the
`.comment` independently proves the original used the Android 7.0.2 build. What
it does establish is (a) 7.0.0 and 7.0.1 are codegen-identical for this
codebase, and (b) neither is closer to the original than the Android 7.0.2
build. A 7.0.0/7.0.1 substitution is not the missing ingredient.

---

## 4. Why `byte_exact` collapses under a one-TU flag change

`byte_exact` compares the raw bytes of each function, including literal-pool
words. A literal-pool word that holds an absolute address (a `.rodata`/`.data`
pointer, a GOT-relative delta, a function pointer) is link-layout dependent.
Changing one TU's optimization level or exception mode changes its function
sizes, which shifts every later function in `.text` and therefore every later
literal-pool absolute address. The result is that a *local* codegen change
breaks raw-byte equality for thousands of unrelated functions.

This is why:
* `-fno-rtti` / `-fno-threadsafe-statics` / `-fno-omit-frame-pointer` (which
  change no function sizes in these TUs) keep `byte_exact = 4506`;
* `-O2` / `-O3` / `-Os` / `-fno-exceptions` / `-fomit-frame-pointer` (which
  change sizes) drop `byte_exact` to ~1350–3100 even when the target pct is
  unchanged.

The per-function pct is the meaningful per-TU signal; `byte_exact` is the
whole-gate signal and is reported for completeness.

---

## 5. Reproduction

```sh
# scratch build with the per-TU launcher
cmake -S . -B /dev/shm/w7721/build -G Ninja \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs \
  -DGOF2_MATCH_OPT=-Oz \
  -DCMAKE_C_COMPILER_LAUNCHER=/dev/shm/w7721/launch.sh \
  -DCMAKE_CXX_COMPILER_LAUNCHER=/dev/shm/w7721/launch.sh

# one cell: recompile Level.cpp at -O2, relink, score
GOF2_TU=Level.cpp GOF2_TU_FLAGS=-O2 \
  cmake --build /dev/shm/w7721/build --target link_so -j6
python3 /dev/shm/w7721/score.py /dev/shm/w7721/build/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb _ZN5Level13createMissionEv

# clang 7.0.0 / 7.0.1 (upstream releases)
curl -sL -o clang700.tar.xz \
  https://releases.llvm.org/7.0.0/clang+llvm-7.0.0-x86_64-linux-gnu-ubuntu-16.04.tar.xz
curl -sL -o clang701.tar.xz \
  https://releases.llvm.org/7.0.1/clang+llvm-7.0.1-x86_64-linux-gnu-ubuntu-16.04.tar.xz
# build dir with a launcher that swaps the C++ driver and adds -nostdinc++
GOF2_CC_DIR=/dev/shm/w7721/c700 \
  cmake --build /dev/shm/w7721/build_cc --target link_so -j6
```

The full matrix is `/dev/shm/w7721/matrix.jsonl` (68 JSON lines: 56 worst-8
cells + 12 secondary cells). It is scratch, not committed; every number in
this note is reproduced by the commands above.

---

## 6. What would falsify this note

* A per-TU variant that raises any residual pct above the canonical value while
  the full gate stays at `byte_exact 4506`. None of the 68 cells does.
* An NDK package that ships clang 7.0.0 or 7.0.1. The three r18 packages all
  ship the same `4751641` / 7.0.2 build.
* An Android clang 7.0.0/7.0.1 prebuilt that scores closer to the original than
  the Android 7.0.2 build. The upstream 7.0.0/7.0.1 releases are far worse
  (byte_exact 1372), though the Android-patch confound means this is not a
  clean version test.

Until one of those happens, the conclusion is: **the per-TU flag space is
exhausted for the residual functions; the remaining 18 functions are source
shape, not a per-TU build configuration.**
