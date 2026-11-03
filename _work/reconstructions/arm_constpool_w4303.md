# ARM constant materialization / literal pools / movw-movt (w4303, wave 93)

Status: research note, 2026-11-03. Worktree `wt/w4303`, branch `wave93-w4303`,
base `eea2e3c1` ("Globals::Globals pass 6"). Scratch is `/dev/shm/w4303`.
**No source or build-flag change is landed**: no tested flag raises the gate.
This commit contains this note only.

## 0. Bottom line

* The original's **constant-materialization policy is already reproduced exactly**:
  across the 4,499 byte-exact functions the whole-binary counts are
  `movw 8681/8681`, `movt 18/18`, `ldr [pc] 13524/13524`, literal-pool words
  `13375/13375`.
* The **only** materialization-count differences in the whole binary are inside the
  25 non-exact functions: `movw +3`, `ldr [pc] -12`, pool words `-12`, instructions
  `-153`. 18 of the 25 have *identical* `movw`/`movt`/`ldr pc`/pool-word counts and
  differ only in register allocation, scheduling or block order.
* There is **no `movt` in any app function** on either side. The 18 `movt` in the
  binary are in vendored `libcrypto` (`SHA224_Init`, `SHA256_Init`) and `libc++abi`
  (`std::terminate`), all byte-exact.
* **Every** constant-materialization flag that exists in clang 7.0.2 is neutral or
  worse. `-arm-promote-constant` is catastrophically wrong (byte 1386);
  `-mno-movt` is a no-op on the app TUs; `-arm-constant-island-max-iteration` needs >=4 and is
  neutral for 4..32; `-arm-atomic-cfg-tidy=false`, `-arm-use-mulops=false`,
  `-arm-global-merge=false`, `-arm-load-store-opt=false`,
  `-arm-adjust-jump-tables=false` are all real regressions.
* Several knobs named in the task **do not exist in LLVM 7.0.2**:
  `-arm-use-movt`, `-constpool-min-size`, `-arm-enable-swp`, `-arm-long-calls`,
  `-arm-constant-island` (boolean), `-arm-constant-island-max-ldr`,
  `-arm-constant-island-max-size`, `-arm-constant-island-ldr-distance`.
* The residual diffs in `Globals::Globals` (pool order/alignment) and `Layout`
  (missing/extra float constants) are **source-shape**, not flag-driven. Each is
  dissected below with the exact pool words.
* Verdict: **the current global flags are correct for constant materialization**;
  the remaining work is source/TU shape.

## 1. Baseline and gate

Commands (canonical build, untouched):

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j64
cmake --build cmake-build-match --target check   -j64
```

Before and after this note:

```text
verify: byte 4499  linked 4499  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Section sizes: `.text 0x1337c8 @ 0x69440`, `.rodata 0x41b67 @ 0x1c9e10`,
`.ARM.exidx 0x6f70`, `.ARM.extab 0x26298`, `.data 0x296c`, `.bss 0x69ac`.
`size-exact 4524/4524`, lint `CLEAN`, drift 0.

The ground-truth binary is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(sha256 `d4c17be6…bed9483`). Every number below is from the official
`tools/verify/verify.py` run against it on a fresh link of this tree, not a proxy.

## 2. Positive evidence: the original's materialization fingerprint

Classification commands (scripts in `/dev/shm/w4303`, logic described inline):

```sh
# whole-binary / per-group counts (uses verify.disassemble, same pool masking as the verifier)
python3 /dev/shm/w4303/global_count.py
# per-target movw/movt/ldr-pc/pool-word counts for the 25 non-exact functions
python3 /dev/shm/w4303/classify.py
# dump every pool word of one function with its PIC/const/address classification
python3 /dev/shm/w4303/pool_dump.py _ZN6LayoutC1Ev
# multiset difference of pool words (orig vs ours) for all non-exact targets
python3 /dev/shm/w4303/poolset.py
# first normalized-instruction difference per target, with context
python3 /dev/shm/w4303/firstdiff.py
# list every movt in the original
python3 /dev/shm/w4303/movt_list.py
```

All of them reuse `tools/verify/verify.py`'s `disassemble()`, `Resolver` and
`PCREL_LOAD`/pool logic, so their instruction and pool-word definitions are the
official verifier's, not an independent approximation.

### 2.1 Whole-binary instruction counts

Method: disassemble every one of the 4,524 common functions from both `.so` files
with the same `verify.disassemble()` the official verifier uses (correct ARM/Thumb
mode per symbol, literal pools and `tbb/tbh` tables masked), then count mnemonics.
`poolword` is the number of distinct 4-byte literal-pool words inside the
function; `ldrpc` is the number of `ldr/ldrd/vldr [pc, …]` loads.

| group | functions | insns | movw | movt | ldr [pc] | pool words |
|---|---:|---:|---:|---:|---:|---:|
| ALL orig | 4524 | 447,833 | 9,197 | 18 | 15,621 | 15,387 |
| ALL ours | 4524 | 447,680 | 9,200 | 18 | 15,609 | 15,375 |
| **delta** | | **-153** | **+3** | **0** | **-12** | **-12** |
| EXACT orig | 4499 | 385,100 | 8,681 | 18 | 13,524 | 13,375 |
| EXACT ours | 4499 | 385,100 | 8,681 | 18 | 13,524 | 13,375 |
| **delta** | | **0** | **0** | **0** | **0** | **0** |
| NONEXACT orig | 25 | 62,733 | 516 | 0 | 2,097 | 2,012 |
| NONEXACT ours | 25 | 62,580 | 519 | 0 | 2,085 | 2,000 |
| **delta** | | **-153** | **+3** | **0** | **-12** | **-12** |

This is the strongest single piece of evidence: **99.45 % of the binary already
materializes constants with exactly the original's instruction mix and exactly the
original's literal-pool word count.** A wrong global materialization flag would
show up on thousands of functions, not on 25.

### 2.2 The 25 non-exact functions

`movw`/`movt`/`ldr [pc]`/pool-word counts, original/ours (a single number means the
counts are equal). `const` = non-PIC pool words (float/integer constants);
`pic` = pool words consumed by the `ldr rX,[pc] / add rX,pc` PIC idiom (GOT/data
address deltas).

| function | size | movw | movt | ldr [pc] | pool W | pic | const |
|---|---:|---:|---:|---:|---:|---:|---:|
| `Layout::Layout` C1/C2 | 7072 | 12/13 | 0 | 39/32 | 38/32 | 22/23 | 16/9 |
| `LevelScript::process` | 57232 | 209 | 0 | 755 | 706/704 | 274/273 | 432/431 |
| `Status::missionCompleted` | 1432 | 3 | 0 | 4 | 4 | 4 | 0 |
| `Radar::draw` | 9908 | 2/3 | 0 | 45 | 45 | 37 | 8 |
| `Level::createShip` | 2764 | 10 | 0 | 126 | 121 | 6 | 115 |
| `Generator::createMission` | 1996 | 2 | 0 | 17/18 | 17/18 | 10 | 7/8 |
| `MGame::OnTouchEnd` | 8080 | 1 | 0 | 91 | 91 | 87 | 4 |
| `HangarWindow::render` | 9700 | 3/4 | 0 | 112/111 | 112/111 | 100/99 | 12 |
| `Globals::Globals` C1/C2 | 612 | 3 | 0 | 31 | 31 | 31 | 0 |
| `RocketGun::update` | 1908 | 0 | 0 | 12 | 10 | 4 | 6 |
| `PlayerEgo::calcCollision` | 968 | 3 | 0 | 7 | 7 | 5 | 2 |
| `Level::createScene` | 2252 | 4 | 0 | 26 | 26 | 24 | 2 |
| `Hud::draw` | 9332 | 3 | 0 | 80 | 77 | 67 | 10 |
| `Gun::calcCharacterCollision` | 1984 | 3 | 0 | 11 | 11 | 5 | 6 |
| `Level::createMission` | 17172 | 96/95 | 0 | 183/185 | 176/178 | 71/73 | 105 |
| `MenuTouchWindow` C1/C2 | 9600 | 43 | 0 | 81 | 79 | 69 | 10 |
| `SpaceLounge::drawLounge` | 1948 | 3 | 0 | 26 | 25 | 21 | 4 |
| `MenuTouchWindow::OnTouchBegin` | 1896 | 0 | 0 | 20 | 20 | 20 | 0 |
| `MenuTouchWindow::OnTouchEnd` | 8132 | 11 | 0 | 107 | 106 | 98 | 8 |
| `ModStation::ModStation` C1/C2 | 1008 | 1 | 0 | 10 | 10 | 10 | 0 |
| `MGame::OnUpdate` | 11440 | 45 | 0 | 153 | 142 | 110 | 32 |

**18 of the 25 have identical materialization counts** and differ only in
regalloc/scheduling/block order. The 7 that differ do so by at most 7 words, and
each is explained in §4.

### 2.3 Where the 18 `movt` are

`movt` appears only in three vendored functions, all byte-exact:

```text
SHA224_Init       8 movt   (libcrypto)
SHA256_Init       8 movt   (libcrypto)
std::terminate    2 movt   (libc++abi)
```

**No app function uses `movt` on either side.** The vendored archives are built by
`third_party/build_match_lib.cmake` with their own fixed flags, so an app-TU
`-mno-movt` cannot touch these three functions anyway. On the app TUs the flag is
unobservable (measured: 0 rows changed, §3).

### 2.4 What the first difference in each target is

For each target, the first normalized-instruction difference (official verifier
streams) was classified. None is a different materialization *policy*; the pool
offsets that differ are a consequence of the code before the pool being a different
length.

| class | targets |
|---|---|
| callee-saved register set (prologue) | `Layout` C1/C2 (`{r6..}` vs `{r5..}`), `Radar::draw` (`vpush d8-d13` vs `d8-d11`), `Hud::draw` (`vpush d8-d11` vs `d8-d12`) |
| pool offset only (code length differs) | `LevelScript::process`, `Generator::createMission`, `HangarWindow::render`, `Gun::calcCharacterCollision`, `RocketGun::update`, `ModStation` C1/C2 |
| register allocation | `PlayerEgo::calcCollision` (`r9` vs `fp`), `Level::createScene` (`r9` vs `r8`), `SpaceLounge::drawLounge` (`r9` vs `fp`), `MenuTouchWindow::OnTouchBegin` (`ip` vs `r4`), `ModStation` (`r3/r4` swap) |
| stack slot | `Level::createShip` (`[sp,#40]` vs `#32`), `Level::createMission` (`[sp,#88]` vs `#84`), `MenuTouchWindow` C1/C2 (`[sp,#60]` vs `#64`) |
| branch immediate / block order | `Status::missionCompleted`, `MGame::OnTouchEnd`, `MenuTouchWindow::OnTouchEnd`, `MGame::OnUpdate`, `Globals::Globals` |

## 3. Flag sweep (official verifier)

### 3.1 Method

A scratch wrapper toolchain includes the real `cmake/toolchain.cmake` and appends
exactly one candidate to `CMAKE_CXX_FLAGS`/`CMAKE_C_FLAGS`:

```cmake
# /dev/shm/w4303/toolchain_extra.cmake
include("/home/debian/GalaxyOnFire2/wt/w4303/cmake/toolchain.cmake")
if(GOF2_EXTRA_CXX_FLAGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GOF2_EXTRA_CXX_FLAGS}" CACHE STRING "" FORCE)
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${GOF2_EXTRA_CXX_FLAGS}" CACHE STRING "" FORCE)
endif()
```

Each variant gets its own build directory `/dev/shm/w4303/build-<tag>` and verify
directory `/dev/shm/w4303/verify-<tag>`. The vendored-archive build writes into the
shared source tree, so builds are serialized with `flock`; verifies run in parallel.
Measurement is the official verifier:

```sh
python3 tools/verify/verify.py --build-dir /dev/shm/w4303/verify-<tag> \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /dev/shm/w4303/build-<tag>/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
```

The `base` variant (no extra flag) reproduces the canonical gate exactly
(byte 4499 / linked 4499 / size 4524 / avg 99.90). Its `.so` sha differs from
`cmake-build-match/libgof2hdaa.so` only because the build-id hashes DWARF paths
that embed the build-directory name; the gate counts are identical.

### 3.2 Knob availability in clang 7.0.2

`clang -mllvm -help-hidden` on the NDK r18b clang 7.0.2 lists only these
constant-materialization knobs:

```text
-arm-promote-constant
-arm-promote-constant-max-size=<uint>
-arm-promote-constant-max-total=<uint>
-arm-constant-island-max-iteration=<uint>
-align-neon-spills
```

The other names in the task are **not present** (verified by passing them to
clang; each prints `Unknown command line argument`):

```text
-arm-use-movt=0/1            -constpool-min-size=N
-arm-enable-swp              -arm-long-calls
-arm-constant-island         -arm-constant-island-max-ldr=N
-arm-constant-island-max-size=N   -arm-constant-island-ldr-distance=N
```

`-mno-movt` and `-mlong-calls` exist as **driver** flags, not `-mllvm` flags.

### 3.3 Results

All variants are one flag on top of the canonical `-Oz` build. `size` is
size-exact / 4524. `text`/`rodata` are the linked section sizes (base:
`0x1337c8` / `0x41b67`).

| variant | flag | byte | linked | size | avg | .text | .rodata |
|---|---|---:|---:|---:|---:|---:|---:|
| `base` | — | **4499** | **4499** | **4524** | **99.90** | `0x1337c8` | `0x41b67` |
| `mno_movt` | `-mno-movt` | 4499 | 4499 | 4524 | 99.90 | `0x1337c8` | `0x41b67` |
| `promote_off` | `-mllvm -arm-promote-constant=false` | 4499 | 4499 | 4524 | 99.90 | `0x1337c8` | `0x41b67` |
| `promote_on` | `-mllvm -arm-promote-constant` | **1386** | **4192** | 4221 | 98.18 | `0x134c48` | `0x401e7` |
| `promote_size_0` | `-mllvm -arm-promote-constant-max-size=0` | 4499 | 4499 | 4524 | 99.90 | `0x1337c8` | `0x41b67` |
| `promote_size_4` | `-mllvm -arm-promote-constant-max-size=4` | 4499 | 4499 | 4524 | 99.90 | `0x1337c8` | `0x41b67` |
| `promote_size_8` | `-mllvm -arm-promote-constant-max-size=8` | 4499 | 4499 | 4524 | 99.90 | `0x1337c8` | `0x41b67` |
| `promote_size_16` | `-mllvm -arm-promote-constant-max-size=16` | 4499 | 4499 | 4524 | 99.90 | `0x1337c8` | `0x41b67` |
| `promote_size_32` | `-mllvm -arm-promote-constant-max-size=32` | 4499 | 4499 | 4524 | 99.90 | `0x1337c8` | `0x41b67` |
| `promote_size_64` | `-mllvm -arm-promote-constant-max-size=64` | 4499 | 4499 | 4524 | 99.90 | `0x1337c8` | `0x41b67` |
| `promote_size_128` | `-mllvm -arm-promote-constant-max-size=128` | 4499 | 4499 | 4524 | 99.90 | `0x1337c8` | `0x41b67` |
| `promote_size_256` | `-mllvm -arm-promote-constant-max-size=256` | 4499 | 4499 | 4524 | 99.90 | `0x1337c8` | `0x41b67` |
| `promote_total_0` | `-mllvm -arm-promote-constant-max-total=0` | 4499 | 4499 | 4524 | 99.90 | `0x1337c8` | `0x41b67` |
| `promote_total_4` | `-mllvm -arm-promote-constant-max-total=4` | 4499 | 4499 | 4524 | 99.90 | `0x1337c8` | `0x41b67` |
| `promote_total_8` | `-mllvm -arm-promote-constant-max-total=8` | 4499 | 4499 | 4524 | 99.90 | `0x1337c8` | `0x41b67` |
| `promote_total_16` | `-mllvm -arm-promote-constant-max-total=16` | 4499 | 4499 | 4524 | 99.90 | `0x1337c8` | `0x41b67` |
| `promote_total_32` | `-mllvm -arm-promote-constant-max-total=32` | 4499 | 4499 | 4524 | 99.90 | `0x1337c8` | `0x41b67` |
| `promote_total_64` | `-mllvm -arm-promote-constant-max-total=64` | 4499 | 4499 | 4524 | 99.90 | `0x1337c8` | `0x41b67` |
| `promote_total_128` | `-mllvm -arm-promote-constant-max-total=128` | 4499 | 4499 | 4524 | 99.90 | `0x1337c8` | `0x41b67` |
| `promote_total_256` | `-mllvm -arm-promote-constant-max-total=256` | 4499 | 4499 | 4524 | 99.90 | `0x1337c8` | `0x41b67` |
| `promote_on_size_4` | promote + max-size 4 | 1414 | 4394 | 4438 | 99.48 | `0x133308` | `0x41a87` |
| `promote_on_size_8` | promote + max-size 8 | 1371 | 4352 | 4418 | 99.28 | `0x1331a8` | `0x418f7` |
| `promote_on_size_16` | promote + max-size 16 | 1436 | 4258 | 4298 | 98.61 | `0x134168` | `0x410a7` |
| `promote_on_size_32` | promote + max-size 32 | 1370 | 4205 | 4236 | 98.27 | `0x134888` | `0x40727` |
| `promote_on_total_64` | promote + max-total 64 | 1370 | 4192 | 4221 | 98.29 | `0x134348` | `0x406b7` |
| `promote_on_total_256` | promote + max-total 256 | 1379 | 4192 | 4221 | 98.12 | `0x135448` | `0x3fce7` |
| `promote_on_total_1024` | promote + max-total 1024 | 1370 | 4191 | 4221 | 98.11 | `0x135f14` | `0x3f107` |
| `island_iter_0` | `-mllvm -arm-constant-island-max-iteration=0` | — | — | — | — | — | — |
| `island_iter_1` | `-mllvm -arm-constant-island-max-iteration=1` | — | — | — | — | — | — |
| `island_iter_2` | `-mllvm -arm-constant-island-max-iteration=2` | 4444 | 4497 | 4522 | 99.90 | `0x1337c8` | `0x41b67` |
| `island_iter_3` | `-mllvm -arm-constant-island-max-iteration=3` | 4444 | 4497 | 4522 | 99.90 | `0x1337c8` | `0x41b67` |
| `island_iter_4` | `-mllvm -arm-constant-island-max-iteration=4` | 4499 | 4499 | 4524 | 99.90 | `0x1337c8` | `0x41b67` |
| `island_iter_8` | `-mllvm -arm-constant-island-max-iteration=8` | 4499 | 4499 | 4524 | 99.90 | `0x1337c8` | `0x41b67` |
| `island_iter_16` | `-mllvm -arm-constant-island-max-iteration=16` | 4499 | 4499 | 4524 | 99.90 | `0x1337c8` | `0x41b67` |
| `island_iter_32` | `-mllvm -arm-constant-island-max-iteration=32` | 4499 | 4499 | 4524 | 99.90 | `0x1337c8` | `0x41b67` |
| `atomic_cfg_tidy` | `-mllvm -arm-atomic-cfg-tidy` | 4499 | 4499 | 4524 | 99.90 | `0x1337c8` | `0x41b67` |
| `atomic_cfg_tidy_false` | `-mllvm -arm-atomic-cfg-tidy=false` | 1409 | 4498 | 4523 | 99.90 | `0x1337e8` | `0x41b67` |
| `use_mulops` | `-mllvm -arm-use-mulops` | 4499 | 4499 | 4524 | 99.90 | `0x1337c8` | `0x41b67` |
| `use_mulops_false` | `-mllvm -arm-use-mulops=false` | 1545 | 4461 | 4503 | 99.79 | `0x133588` | `0x41b67` |
| `global_merge` | `-mllvm -arm-global-merge` | 4499 | 4499 | 4524 | 99.90 | `0x1337c8` | `0x41b67` |
| `global_merge_false` | `-mllvm -arm-global-merge=false` | 1397 | 4489 | 4517 | 99.82 | `0x133928` | `0x41b67` |
| `load_store_opt_false` | `-mllvm -arm-load-store-opt=false` | 1225 | 3313 | 3906 | 94.71 | `0x137628` | `0x41b67` |
| `adjust_jump_tables_false` | `-mllvm -arm-adjust-jump-tables=false` | 1571 | 4493 | 4517 | 99.87 | `0x1339a8` | `0x41b67` |
| `assume_misaligned` | `-mllvm -arm-assume-misaligned-load-store` | 1225 | 3404 | 3976 | 95.14 | `0x136b48` | `0x41b67` |
| `implicit_it_always` | `-mllvm -arm-implicit-it=always` | 4499 | 4499 | 4524 | 99.90 | `0x1337c8` | `0x41b67` |
| `implicit_it_never` | `-mllvm -arm-implicit-it=never` | 4499 | 4499 | 4524 | 99.90 | `0x1337c8` | `0x41b67` |
| `arm_restrict_it` | `-mllvm -arm-restrict-it` | 1278 | 3325 | 4192 | 96.42 | `0x1334a8` | `0x41b67` |
| `arm_no_restrict_it` | `-mllvm -arm-no-restrict-it` | 4499 | 4499 | 4524 | 99.90 | `0x1337c8` | `0x41b67` |
| `synth_thumb1_tbb` | `-mllvm -arm-synthesize-thumb-1-tbb` | 4499 | 4499 | 4524 | 99.90 | `0x1337c8` | `0x41b67` |
| `enable_arm_3addr_false` | `-mllvm -enable-arm-3-addr-conv=false` | 4499 | 4499 | 4524 | 99.90 | `0x1337c8` | `0x41b67` |
| `arm_interworking_false` | `-mllvm -arm-interworking=false` | 4499 | 4499 | 4524 | 99.90 | `0x1337c8` | `0x41b67` |
| `align_neon_spills` | `-mllvm -align-neon-spills` | 4499 | 4499 | 4524 | 99.90 | `0x1337c8` | `0x41b67` |
| `align_neon_spills_false` | `-mllvm -align-neon-spills=false` | 4499 | 4499 | 4524 | 99.90 | `0x1337c8` | `0x41b67` |
| `force_fast_isel` | `-mllvm -arm-force-fast-isel` | 4498 | 4499 | 4524 | 99.90 | `0x1337c8` | `0x41b67` |
| `pipeliner_opt_size` | `-mllvm -enable-pipeliner-opt-size` | 4499 | 4499 | 4524 | 99.90 | `0x1337c8` | `0x41b67` |
| `arm_add_build_attributes` | `-mllvm -arm-add-build-attributes` | 4499 | 4499 | 4524 | 99.90 | `0x1337c8` | `0x41b67` |
| `mlong_calls` | `-mlong-calls` | 1345 | 1664 | 1673 | 75.45 | `0x191278` | `0x41b67` |
| `fno_merge_constants` | `-fno-merge-constants` | 4499 | 4499 | 4524 | 99.90 | `0x1337c8` | `0x41b67` |
| `fmerge_all_constants` | `-fmerge-all-constants` | 4499 | 4499 | 4524 | 99.90 | `0x1337c8` | `0x41b67` |
| `fno_merge_all_constants` | `-fno-merge-all-constants` | 4499 | 4499 | 4524 | 99.90 | `0x1337c8` | `0x41b67` |
| `align_all_blocks_2` | `-mllvm -align-all-blocks=2` | 1190 | 2467 | 2540 | 92.95 | `0x143a88` | `0x41b67` |
| `align_all_blocks_4` | `-mllvm -align-all-blocks=4` | 1173 | 2347 | 2350 | 83.80 | `0x1b6708` | `0x41b67` |
| `align_all_nofallthru_2` | `-mllvm -align-all-nofallthru-blocks=2` | 1289 | 3185 | 3313 | 96.32 | `0x1399c8` | `0x41b67` |
| `align_all_nofallthru_4` | `-mllvm -align-all-nofallthru-blocks=4` | 1251 | 2802 | 2818 | 92.90 | `0x1655c8` | `0x41b67` |

`island_iter_0`/`island_iter_1` fail to build:
`fatal error: error in backend: Constant Island pass failed to converge!`
(ImageFont, Image, Mesh, VirtualInput, …).

### 3.4 Per-target deltas

For every neutral variant the 25 target percentages are **exactly** at baseline
(max Δ = +0.0). No variant raises any target. The variants that move targets:

| variant | targets moved | Δ range | example |
|---|---:|---:|---|
| `force_fast_isel` | 0 (pct) / 1 byte | 0 | `FileInterfaceAndroid::~FileInterfaceAndroid` byte-exact -> 100 % non-byte (same size) |
| `island_iter_2`/`_3` | 2 | -8.4 … -5.4 | `Status::nextCampaignMission` 100.0 -> 91.6 |
| `atomic_cfg_tidy_false` | 1 | -13.5 | `MGame::OnTouchEnd` 100.0 -> 86.5 |
| `global_merge_false` | 10 | -64.5 … -14.2 | `GameText::getText` 100.0 -> 35.5 |
| `use_mulops_false` | 47 | -51.6 … -0.1 | `Radar::calcDistance` 100.0 -> 48.4 |
| `adjust_jump_tables_false` | 7 | -47.3 … -3.4 | several 100.0 -> 52.7 |
| `promote_on` | 319 | -85.6 … -0.2 | `getConsumableSKU` 100.0 -> 14.4 |
| `load_store_opt_false` | 1211 | -95.0 … -0.1 | broad |
| `assume_misaligned` | 1117 | -95.0 … -0.1 | broad |
| `mlong_calls` | 2856 | -100.0 … -1.5 | broad |

## 4. Case studies

### 4.1 `Globals::Globals` (612 B, 80.5 %) — pool alignment is not a flag

The function has a **140-byte literal pool in both binaries**: 31 GOT words + four
float words (`0.6f, 0.6f, 0.5f, 0.5f`) loaded by `vld1.64 {d16-d17}`. The pool is
8-byte aligned; ours is 2 instructions (4 bytes) longer, so the assembler inserts
two `nop`s to reach the 8-byte boundary. The real code is 468 B (ours) vs 472 B
(original); the pool is 140 B in both.

The pool **contents** are the same 31 symbols, but `status` is at pool position 6
in ours and position 25 in the original. That is the order of first GOT load, i.e.
the source order of the member initializations. The original also has the
`movw r6,#583` hoist that our current source does not produce. Both are
source-shape (see `globals-globals-c1-c2-w4259.md`), not materialization flags.

There is no const-pool alignment knob in LLVM 7.0.2 (`-align-all-const-pools` does
not exist); the 8-byte padding is forced by the NEON `vld1.64` alignment
requirement. `-align-neon-spills` (the only NEON-alignment knob) is neutral.

### 4.2 `Layout::Layout` (7072 B, 47.7 %) — missing/extra float constants are source

The original has 16 non-PIC pool words, ours 9. The multiset difference is:

| pool word | value | original | ours |
|---|---:|---:|---:|
| `0x41e10000` | 28.125 | 1 | 0 |
| `0x41610000` | 14.0625 | 1 | 0 |
| `0x40b40000` | 5.625 | 1 | 0 |
| `0x42960000` | 75.0 | 1 | 0 |
| `0x41700000` | 15.0 | 1 | 0 |
| `0x41400000` | 12.0 | 1 | 0 |
| `0x42160000` | 37.5 | 1 | 0 |
| `0x42480000` | 50.0 | 1 | 0 |
| `0x430ca000` | 140.625 | 0 | 1 |

`28.125 = 20 * 1.40625`, `14.0625 = 10 * 1.40625`, `5.625 = 4 * 1.40625`,
`140.625 = 100 * 1.40625`. Our `Layout.cpp` has no `20 * 1.40625f`,
`10 * 1.40625f` or `4 * 1.40625f` term, and has a `100 * 1.40625f` term the
original does not. The other five (75, 15, 12, 37.5, 50) are fields our source
stores as integers (or with a different branch) where the original stores floats.
There is also one extra `Globals::iPadLarge` GOT load in ours (5 vs 4).

No compiler flag can create source constants that are not in the source. The
`Layout::Layout` residual is the known missing/extra `vstr` opaque-field and
HD-block-order problem (`layout-layout-c1-c2-hd-block-order-w4221.md`), not a
materialization flag.

### 4.3 `Radar::draw` and `HangarWindow::render` — the extra `movw` is if-conversion

`Radar::draw` has 3 `movw` in ours vs 2 in the original. The extra one is the same
constant `2242`:

```text
original:   mov   r1, r0
            cmp   r1, #106
            mov.w r1, #0
            str   r1, [sp, #0]
            ite   lt
            movwlt r1, #2241
            movwge r1, #2242          <- conditional movw

ours:       movs  r1, #0
            cmp   r0, #106
            str   r1, [sp, #0]
            movw  r1, #2242           <- unconditional movw
            it    lt
            movwlt r1, #2241
```

Both materialize the same two constants. The difference is whether the `2242`
arm is if-converted into the `ite` or kept as an unconditional store; that is a
block-layout/if-conversion difference. `HangarWindow::render` is the same pattern
with `movw r1,#3400` (`it eq; movweq r1,#3400` in the original vs an unconditional
`movw` after an `it ne; addne.w` in ours).

## 5. Verdicts per hypothesis

| hypothesis | verdict | evidence |
|---|---|---|
| `-mllvm -arm-use-movt=0/1` | **does not exist**; driver `-mno-movt` is **disproven as a cause** | no app `movt` on either side; `-mno-movt` changes 0 rows |
| `-mllvm -arm-promote-constant` (on) | **disproven** | byte 4499 -> 1386, linked -> 4192, `.text` +0x1480, `.rodata` -0x1980; pool loads -1365, insns +5109 |
| `-mllvm -arm-promote-constant=false` | **proven default off** | 0 rows changed |
| `-mllvm -arm-promote-constant-max-size/total` | **no-op at default** | all 16 values neutral; with promotion on, all worse (1370-1436) |
| `-mllvm -arm-constant-island` (boolean / max-ldr / max-size / ldr-distance) | **does not exist in LLVM 7** | `Unknown command line argument` |
| `-mllvm -arm-constant-island-max-iteration` | **default (>=4) proven**; no custom value better | 0/1 fail to converge; 2/3 -> 4444; 4/8/16/32 neutral |
| `-mllvm -constpool-min-size` | **does not exist** | `Unknown command line argument` |
| `-mllvm -arm-enable-swp` | **does not exist**; `-enable-pipeliner-opt-size` neutral | `Unknown command line argument`; 4499 |
| `-mllvm -arm-long-calls` | **does not exist**; driver `-mlong-calls` **disproven** | byte 1345, linked 1664, `.text 0x191278` |
| `-mllvm -arm-atomic-cfg-tidy` | **default true proven** | flag neutral; `=false` -> 1409 |
| `-mllvm -arm-use-mulops` | **default true proven** | flag neutral; `=false` -> 1545 |
| `-mllvm -arm-global-merge` | **default true proven** | flag neutral; `=false` -> 1397 |
| `-mllvm -arm-load-store-opt` | **default true proven** | `=false` -> 1225 |
| `-mllvm -arm-adjust-jump-tables` | **default true proven** | `=false` -> 1571 |
| `-mllvm -arm-assume-misaligned-load-store` | **default false proven** | flag -> 1225 |
| `-mllvm -arm-implicit-it=always/never` | neutral | 4499 |
| `-mllvm -arm-restrict-it` | **default false proven** | flag -> 1278; `-arm-no-restrict-it` neutral |
| `-mllvm -arm-synthesize-thumb-1-tbb` | neutral | 4499 |
| `-mllvm -enable-arm-3-addr-conv=false` | neutral | 4499 |
| `-mllvm -arm-interworking=false` | neutral | 4499 |
| `-mllvm -align-neon-spills` / `=false` | neutral | 4499 |
| `-mllvm -arm-force-fast-isel` | **worse** | byte 4498 (one function) |
| `-fno-merge-constants` / `-fmerge-all-constants` / `-fno-merge-all-constants` | neutral | 4499 |
| `-mllvm -align-all-blocks=2/4`, `-align-all-nofallthru-blocks=2/4` | **worse** | 1190-1289 |
| Globals pool alignment/order | **source, not flag** | 31/31 same symbols; `status` first-use order differs; 4 B nop pad |
| Layout missing/extra constants | **source, not flag** | 8 missing + 1 extra float constants; `20/10/4 * 1.40625f` absent from source |
| Radar/HangarWindow extra `movw` | **source (if-conversion)**, not flag | same constants, `ite` vs unconditional |

## 6. What remains

* The global constant-materialization flags are not the residual cause. The
  remaining 25 functions are source/TU shape: expression order, local lifetimes,
  if-conversion shape, struct field sets and missing constants.
* The most tractable materialization-adjacent source fixes are:
  * `Layout::Layout`: add the missing `20 * 1.40625f`, `10 * 1.40625f`,
    `4 * 1.40625f` HD fields and remove the `100 * 1.40625f` one; the 8 missing
    float constants are a concrete, checkable target.
  * `Globals::Globals`: reproduce the `movw r6,#583` hoist and the original
    `status` first-use position; that is worth ~4 B of real code and would remove
    the 4-byte nop pad.
  * `Radar::draw` / `HangarWindow::render`: source shape that lets the `2242` /
    `3400` arms if-convert into the `ite` block.
* No further global `-mllvm` sweep is warranted for this question: every knob that
  exists in clang 7.0.2 has been measured, and all are neutral or worse.

## 7. Reproduction

```sh
# baseline
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j64

# one variant (see §3.1 for the wrapper)
cmake -S . -B /dev/shm/w4303/build-<tag> -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/dev/shm/w4303/toolchain_extra.cmake \
  -DGOF2_MATCH_OPT=-Oz -DGOF2_EXTRA_CXX_FLAGS='<flag>' \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build /dev/shm/w4303/build-<tag> --target link_so -j16
python3 tools/verify/verify.py --build-dir /dev/shm/w4303/verify-<tag> \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /dev/shm/w4303/build-<tag>/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
```

Scratch harness (not tracked): `/dev/shm/w4303/{toolchain_extra.cmake,sweep.py,
sweep2.py,classify.py,pool_dump.py,poolset.py,firstdiff.py,global_count.py,
movt_list.py}`, results in `/dev/shm/w4303/sweep_results_run1.json` and
`/dev/shm/w4303/sweep_results.json`.
