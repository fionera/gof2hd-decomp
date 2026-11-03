# PIC / visibility / relocation model and PLT/GOT shapes (w4308, wave 93)

Status: research note, 2026-11-03. Worktree `wt/w4308`, branch `wave93-w4308`, base
`eea2e3c1`. Scratch: `/dev/shm/w4308`. **No `src/` or build-flag change is landed**: the
current flags are positively confirmed, not merely "the gate did not improve". Baseline and
final gate are identical (`byte 4499`, `linked 4499`, `size 4524/4524`, `data 843/843`,
`imports 207`, lint `CLEAN`, drift 0).

The question was: *"Regarding the flags, we have no clue if they are correct. Feel free to
start research on how things could have been built."* This note treats every PIC/visibility/
relocation flag as a hypothesis and looks for **positive evidence in the original binary**, not
just for a gate delta. The short answer: the original's PIC and default-visibility model is
**proven** by its own ELF attributes, relocation tables and byte-identical GOT/PLT; no TU shows
a different model.

## 0. Bottom line

| hypothesis | verdict | positive evidence |
|---|---|---|
| global PIC (`-fPIC` or `-fpic`) | **proven** | `.ARM.attributes` `Tag_ABI_PCS_GOT_use: GOT-indirect` (byte-identical to ours); 0 dynamic relocations in `.text`; relocation type counts exactly `ABS32 1817 / GLOB_DAT 614 / JUMP_SLOT 3674 / RELATIVE 1162`; `.got`/`.plt`/`.rel.dyn`/`.rel.plt` byte-identical |
| `-fPIC` vs `-fpic` | **unknown (indistinguishable)** | clang 7.0.2 ARM emits identical objects for both; whole `.so` identical except the 20-byte build-id |
| global `-fno-pic` / `-fno-PIC` | **disproven** | 7,723 text relocations; `.ARM.attributes` flips to `GOT_use: direct`; `.text 0x129e48` (smaller), `.got 0x3a50`, `.plt 0xad3c`; gate 1369/2956 |
| **any** TU compiled `-fno-pic` | **disproven** | one `-fno-pic` TU flips the linked `.ARM.attributes` to `GOT_use: direct` and adds 10-297 text relocations; the original is `GOT-indirect` with 0 text relocations |
| default visibility | **proven** | original exports 5,373 defined dynsym symbols (5,370 compared by the symbol-parity gate); `-fvisibility=hidden` loses 4,280 compared functions; all app calls to exported functions go through PLT (49,717 PLT calls, 0 direct calls in app code) |
| `-fvisibility=hidden` | **disproven** | 57/243, missing 4,280 |
| `-fvisibility-inlines-hidden` | **disproven** | 1,278/4,203, missing 141 |
| `-fvisibility=protected` | **disproven** | 1,345/2,382; `.got 0xb78`, `.plt 0xfb0` (vs original `0x430c`/`0xac4c`) |
| `-fno-plt` | **unknown (codegen no-op)** | with `-fPIC`, clang 7.0.2's ARM backend emits byte-identical objects with and without `-fno-plt` |
| `-mllvm -relocation-model=pic/static/dynamic-no-pic` | **disproven (flag does not exist)** | clang 7.0.2: `Unknown command line argument '-relocation-model=...'` |
| `-mllvm -arm-dynamic-no-pic` | **disproven (flag does not exist)** | clang 7.0.2: unknown argument |
| `-fno-semantic-interposition` | **disproven (flag does not exist)** | clang 7.0.2: `unknown argument: '-fno-semantic-interposition'` |
| `-mpie-copy-relocations` | **unknown (codegen no-op)** | whole `.so` byte-identical except build-id |
| `-mllvm -remat-pic-stub-load=true/false` | **unknown (codegen no-op)** | `.text` byte-identical to base |
| `-mllvm -arm-promote-constant=false` | **unknown (codegen no-op)** | `.text` byte-identical to base |
| `-mllvm -arm-add-build-attributes=true/false` | **unknown (codegen no-op)** | `.text` and `.ARM.attributes` identical to base |
| `-mllvm -old-thumb2-ifcvt`, `-enable-arm-3-addr-conv=false`, `-arm-synthesize-thumb-1-tbb` | **unknown (codegen no-op)** | `.text` byte-identical to base |

No tested flag raises `byte_exact` or any of the 25 target fuzzy values. The remaining 25
functions are source-shape, not relocation-model, work.

## 1. Method

The canonical build was left untouched. All A/B builds used a scratch wrapper that includes
the real toolchain and appends candidate flags:

```cmake
# /dev/shm/w4308/toolchain_extra.cmake
include("/home/debian/GalaxyOnFire2/wt/w4308/cmake/toolchain.cmake")
if(GOF2_EXTRA_FLAGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GOF2_EXTRA_FLAGS}" CACHE STRING "" FORCE)
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${GOF2_EXTRA_FLAGS}" CACHE STRING "" FORCE)
endif()
```

Per-TU variants used `CMAKE_PROJECT_INCLUDE` with:

```cmake
# /dev/shm/w4308/per_tu.cmake
set_source_files_properties("${CMAKE_SOURCE_DIR}/${GOF2_AB_TU}"
    PROPERTIES COMPILE_FLAGS "${GOF2_AB_FLAGS}")
```

Each variant was configured/built with its own scratch build dir:

```sh
cmake -S /home/debian/GalaxyOnFire2/wt/w4308 -B /dev/shm/w4308/build-<tag> -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/dev/shm/w4308/toolchain_extra.cmake \
  -DGOF2_MATCH_OPT=-Oz -DGOF2_EXTRA_FLAGS='<flag>' \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build /dev/shm/w4308/build-<tag> --target link_so -j8
```

Every variant was measured with the official verifier:

```sh
python3 tools/verify/verify.py \
  --build-dir /dev/shm/w4308/verify-<tag> \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /dev/shm/w4308/build-<tag>/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
```

`size-exact` is `sum(osize == usize)` over the report rows. `.text`/`.rodata`/`.got`/`.plt`
are `readelf -SW` sizes. `text relocs` is the number of `.rel.dyn`/`.rel.plt` entries whose
`r_offset` lies in `.text` (`0x69440..0x19cc08`).

Baseline (`g-base`, canonical flags, fresh scratch link):

```text
verify: byte 4499  linked 4499  stubs 0  missing 0  extra 0  avg 99.9
size-exact 4524/4524
.text 1259464 (0x1337c8)  .rodata 269159 (0x41b67)  .got 17164 (0x430c)  .plt 44108 (0xac4c)
text relocs 0
```

`cmake --build cmake-build-match --target check -j8` before and after the research:
`RATCHET PASS` at `byte 4499 / linked 4499`, `data 843/843`, `imports 207`, lint `CLEAN`,
drift 0. `report.json` was not committed.

## 2. Positive evidence from the original binary

### 2.1 The original's own `.ARM.attributes` says GOT-indirect

`readelf -A` on the ground truth and on our link:

```text
Tag_ABI_PCS_GOT_use: GOT-indirect
```

The whole `.ARM.attributes` section is **byte-identical** between the original and our
`-fPIC` build (`objcopy --dump-section .ARM.attributes` + `cmp`). This is compiler-emitted
evidence, not a gate inference.

A probe compiled with `-fno-pic` instead emits `Tag_ABI_PCS_GOT_use: direct` and drops
`Tag_ABI_PCS_RW_data`/`RO_data`. Crucially, **one** `-fno-pic` TU is enough to flip the linked
`.so`'s attribute:

```text
g-base (all -fPIC)          GOT-indirect
g-fnopic (all -fno-pic)     direct
pt-Gun-fnopic (one TU)      direct
pt-HangarWindow-fnopic      direct
pt-Level-fnopic             direct
pt-Globals-fnopic           direct
pt-Radar-fnopic             direct
```

The original is `GOT-indirect`, therefore **no TU in the original was compiled
`-fno-pic`/`-fno-PIC`**. This is the strongest single piece of evidence in this note.

### 2.2 Zero text relocations; relocation counts exact

The original has **no dynamic relocation whose `r_offset` is in `.text`** (and no
`DT_TEXTREL`). Any non-PIC absolute address in code would need one. The relocation types are
exactly reproduced:

| relocation | original | ours (`g-base`) | `-fno-pic` |
|---|---:|---:|---:|
| `R_ARM_ABS32` | 1817 | 1817 | 5901 |
| `R_ARM_GLOB_DAT` | 614 | 614 | 37 |
| `R_ARM_JUMP_SLOT` | 3674 | 3674 | 3692 |
| `R_ARM_RELATIVE` | 1162 | 1162 | 4801 |
| relocations in `.text` | **0** | **0** | **7723** |

(Historical note: `_work/reconstructions/fpic_vs_fPIC_got.md` recorded a 1-off
`ABS32 1818 / RELATIVE 1161` at the older tree state. At `eea2e3c1` the counts are exact.)

### 2.3 GOT/PLT and symbol tables are byte-identical

`objcopy --dump-section` + `cmp` on the canonical link:

```text
.rel.dyn   SAME
.rel.plt   SAME
.got       SAME
.plt       SAME
.dynsym    SAME
.dynstr    SAME
.hash      SAME
.rodata    SAME
.data      SAME
.data.rel.ro SAME
.ARM.attributes SAME
```

Only `.text` (106,772 differing bytes), `.ARM.exidx` (839), `.ARM.extab` (108,462) and the
20-byte `.note.gnu.build-id` differ. The relocation/GOT/PLT model is therefore not merely
size-exact: it is **byte-for-byte the same**.

### 2.4 How the original accesses globals and functions

A scanner over the linked `.so` resolves every `ldr rX,[pc,#imm]; add rX,pc` sequence to its
literal and classifies the result as a `.got` slot (GOT-indirect) or a `.rodata`/`.data`/
`.text` address (PC-relative literal). Totals over all functions:

| access class | original | ours |
|---|---:|---:|
| PLT calls (`blx ...@plt`) | 49,717 | 49,731 |
| GOT-indirect loads | 6,503 | 6,504 |
| PC-relative `.rodata` literals | 4,508 | 4,508 |
| PC-relative `.data/.bss` literals | 210 | 209 |
| PC-relative `.text` literals | 44 | 42 |
| other PC-relative | 60 | 63 |
| register calls (`blx rN`, virtual/function-pointer) | 6,666 | 6,662 |

Two facts matter:

* **No exported global is accessed PC-relative.** All 210 `.data/.bss` PC-relative literals
  resolve to non-exported (static) objects. Every access to a default-visibility global
  (`Globals::layout`, `Globals::Canvas`, `gEngine`, …) is GOT-indirect. That is the PIC model
  with semantic interposition on.
* **No call to an exported function is direct.** The only two `bl` instructions with a symbol
  name outside `@plt` are in `crtbegin`-style startup code at `0x694ec`/`0x6950c` (objdump
  labels them `_end+...`); app code has 0 direct calls to exported functions. All inter-TU and
  intra-TU calls to default-visibility functions go through PLT. `-fvisibility=protected`,
  `-fvisibility=hidden` and `-fno-semantic-interposition` would all produce direct calls.

### 2.5 Per-function GOT/PLT shape of the 25 non-exact functions

`orig/ours` counts. `plt` = `@plt` calls, `got` = GOT-indirect loads, `ro` = PC-relative
`.rodata`, `data` = PC-relative `.data/.bss`, `text` = PC-relative `.text`, `other` =
unclassified PC-relative, `reg` = `blx rN`. `C1`/`C2` are aliases at the same address.

| function | plt | got | ro | data | text | other | reg |
|---|---:|---:|---:|---:|---:|---:|---:|
| `Layout::Layout` C1/C2 | 0/0 | 22/23 | 0/0 | 0/0 | 0/0 | 1/2 | 0/0 |
| `LevelScript::process` | 2356/2361 | 267/266 | 7/7 | 1/0 | 3/1 | 17/19 | 544/538 |
| `Status::missionCompleted` | 70/70 | 4/4 | 0/0 | 0/0 | 0/0 | 0/0 | 0/0 |
| `Radar::draw` | 344/351 | 35/35 | 2/2 | 0/0 | 0/0 | 0/0 | 10/10 |
| `Level::createShip` | 81/83 | 6/6 | 0/0 | 0/0 | 0/0 | 0/0 | 6/6 |
| `Generator::createMission` | 84/84 | 10/10 | 0/0 | 0/0 | 0/0 | 0/0 | 0/0 |
| `MGame::OnTouchEnd` | 465/464 | 82/82 | 5/5 | 0/0 | 0/0 | 0/0 | 17/16 |
| `HangarWindow::render` | 418/421 | 80/79 | 20/20 | 0/0 | 0/0 | 0/0 | 22/22 |
| `Globals::Globals` C1/C2 | 0/0 | 31/31 | 0/0 | 0/0 | 0/0 | 0/0 | 0/0 |
| `RocketGun::update` | 52/52 | 4/4 | 0/0 | 0/0 | 0/0 | 0/0 | 8/8 |
| `PlayerEgo::calcCollision` | 42/42 | 5/5 | 0/0 | 0/0 | 0/0 | 0/0 | 5/5 |
| `Level::createScene` | 107/107 | 12/12 | 11/11 | 1/1 | 0/0 | 0/0 | 2/2 |
| `Hud::draw` | 378/378 | 57/57 | 10/10 | 0/0 | 0/0 | 0/0 | 42/42 |
| `Gun::calcCharacterCollision` | 59/59 | 5/5 | 0/0 | 0/0 | 0/0 | 0/0 | 1/1 |
| `Level::createMission` | 676/674 | 55/57 | 15/15 | 1/1 | 2/2 | 0/0 | 133/136 |
| `MenuTouchWindow::MenuTouchWindow` C1/C2 | 358/358 | 51/51 | 18/18 | 0/0 | 0/0 | 0/0 | 42/42 |
| `SpaceLounge::drawLounge` | 102/102 | 13/13 | 8/8 | 0/0 | 0/0 | 0/0 | 0/0 |
| `MenuTouchWindow::OnTouchBegin` | 37/37 | 20/20 | 0/0 | 0/0 | 0/0 | 0/0 | 8/8 |
| `MenuTouchWindow::OnTouchEnd` | 312/312 | 86/86 | 12/12 | 0/0 | 0/0 | 0/0 | 35/35 |
| `ModStation::ModStation` C1/C2 | 25/25 | 5/5 | 5/5 | 0/0 | 0/0 | 0/0 | 0/0 |
| `MGame::OnUpdate` | 566/566 | 93/93 | 12/12 | 5/5 | 0/0 | 0/0 | 28/28 |

**15 of the 25 have identical GOT/PLT/PC-relative counts.** The 10 that differ do so by small
amounts explained by source shape (extra/missing calls or assignments), not by a different
relocation model. The GOT-symbol multisets differ in only four functions:

| function | symbol | orig → ours |
|---|---|---:|
| `Layout::Layout` C2 | `Globals::iPadLarge` | 4 → 5 |
| `LevelScript::process` | `Level::getLandmarks` | 4 → 3 |
| `HangarWindow::render` | `Globals::layout` | 2 → 1 |
| `Level::createMission` | `AEMath::Vector::operator=` | 0 → 1 |
| `Level::createMission` | `Globals::rnd` | 3 → 4 |

`Gun::calcCharacterCollision` has an **identical** GOT-symbol multiset and identical PLT-call
multiset. Its 90.0% mismatch is register allocation/scheduling, not relocation.

## 3. Global flag matrix

All variants are one flag on top of the canonical `-Oz`/`-fPIC` build. `text==base` means the
`.text` section is byte-identical to `g-base` (the `.so` may still differ in the build-id).

| tag | flag | byte | linked | size | avg | missing | .text | .rodata | .got | .plt | text relocs | text==base |
|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| `g-base` | — | 4499 | 4499 | 4524 | 99.90 | 0 | 1259464 | 269159 | 17164 | 44108 | 0 | — |
| `g-fpic` | `-fpic` | 4499 | 4499 | 4524 | 99.90 | 0 | 1259464 | 269159 | 17164 | 44108 | 0 | yes (whole `.so`) |
| `g-fnoplt` | `-fno-plt` | 4499 | 4499 | 4524 | 99.90 | 0 | 1259464 | 269159 | 17164 | 44108 | 0 | yes (whole `.so`) |
| `g-mpie` | `-mpie-copy-relocations` | 4499 | 4499 | 4524 | 99.90 | 0 | 1259464 | 269159 | 17164 | 44108 | 0 | yes (whole `.so`) |
| `g-visdefault` | `-fvisibility=default` | 4499 | 4499 | 4524 | 99.90 | 0 | 1259464 | 269159 | 17164 | 44108 | 0 | yes (whole `.so`) |
| `g-fnopic` | `-fno-pic` | 1369 | 2956 | 3078 | 89.65 | 0 | 1220168 | 275911 | 14928 | 44324 | 7723 | no |
| `g-fnoPIC` | `-fno-PIC` | 1369 | 2956 | 3078 | 89.65 | 0 | 1220168 | 275911 | 14928 | 44324 | 7723 | no |
| `g-visprot` | `-fvisibility=protected` | 1345 | 2382 | 4170 | 92.59 | 0 | 1249080 | 269159 | 2936 | 4016 | 0 | no |
| `g-vishidden` | `-fvisibility=hidden` | 57 | 243 | 244 | 99.92 | 4280 | 1249080 | 269159 | 2936 | 4016 | 0 | no |
| `g-visinlhidden` | `-fvisibility-inlines-hidden` | 1278 | 4203 | 4381 | 99.84 | 141 | 1259432 | 269159 | 16592 | 42428 | 0 | no |

Reading:

* `-fpic`, `-fno-plt` and `-mpie-copy-relocations` are **whole-`.so` identical** to the
  canonical build (only the 20-byte build-id differs). They cannot be distinguished from the
  binary; `-fPIC` is a valid, conventional choice.
* `-fno-pic` is dramatically wrong: the smaller `.text` is the non-PIC address model, the
  larger `.rodata` is the absolute literal pool, and the 7,723 text relocations are the direct
  fingerprint.
* `-fvisibility=protected` and `-fvisibility=hidden` collapse `.got` from `0x430c` to `0xb78`
  and `.plt` from `0xac4c` to `0xfb0` (direct calls instead of PLT). The original's exact
  `0x430c`/`0xac4c` therefore rules both out globally.

### 3.1 The 25 targets under the global variants

No variant raises any target. Values are the official verifier `pct`; only the extremes are
shown for the two visibility variants (all 25 go to 0.0 under `-fvisibility=hidden`).

| target | base | `-fno-pic` | `-fvisibility-inlines-hidden` |
|---|---:|---:|---:|
| `Layout::Layout` | 47.7 | 22.7 | 47.7 |
| `LevelScript::process` | 52.0 | 34.7 | 52.0 |
| `Status::missionCompleted` | 57.7 | 55.9 | 57.7 |
| `Radar::draw` | 73.9 | 59.1 | 73.9 |
| `Level::createShip` | 74.1 | 60.4 | 74.1 |
| `Generator::createMission` | 74.3 | 56.9 | 74.3 |
| `MGame::OnTouchEnd` | 78.4 | 60.9 | 78.3 |
| `HangarWindow::render` | 78.8 | 44.0 | 78.2 |
| `Globals::Globals` | 80.5 | 13.2 | 80.5 |
| `RocketGun::update` | 86.9 | 73.5 | 86.9 |
| `PlayerEgo::calcCollision` | 87.3 | 66.1 | 87.3 |
| `Level::createScene` | 88.3 | 61.3 | 88.3 |
| `Hud::draw` | 89.1 | 58.0 | 88.8 |
| `Gun::calcCharacterCollision` | 90.0 | 74.7 | 90.0 |
| `Level::createMission` | 90.7 | 60.7 | 90.7 |
| `MenuTouchWindow::MenuTouchWindow` | 94.2 | 69.6 | 94.2 |
| `SpaceLounge::drawLounge` | 95.5 | 61.3 | 95.5 |
| `MenuTouchWindow::OnTouchBegin` | 97.7 | 69.3 | 97.7 |
| `MenuTouchWindow::OnTouchEnd` | 97.9 | 44.5 | 97.8 |
| `ModStation::ModStation` | 98.4 | 57.0 | 98.4 |
| `MGame::OnUpdate` | 99.9 | 51.9 | 99.9 |

`-fpic`, `-fno-plt`, `-mpie-copy-relocations` and `-fvisibility=default` leave every target
exactly at base.

## 4. Per-TU flag matrix

TUs chosen: the 16 TUs that own the 25 non-exact functions. Every per-TU variant was built
with `COMPILE_FLAGS` (appended after the canonical flags and after the scoped `-mllvm`
options), then measured with the official verifier.

### 4.1 `-fno-pic` per TU

Every TU introduces text relocations and lowers its own targets. Because a single `-fno-pic`
TU flips the linked `.ARM.attributes` to `GOT_use: direct`, and the original is
`GOT-indirect`, **no TU is `-fno-pic`**.

| TU | byte | linked | size | .text | text relocs | target base → `-fno-pic` |
|---|---:|---:|---:|---:|---:|---|
| `Layout.cpp` | 2037 | 4467 | 4492 | 1258824 | 113 | `Layout` 47.7 → 22.7 |
| `LevelScript.cpp` | 1378 | 4490 | 4515 | 1257768 | 184 | `process` 52.0 → 34.7 |
| `Status.cpp` | 1378 | 4482 | 4507 | 1258952 | 63 | `missionCompleted` 57.7 → 55.9 |
| `Radar.cpp` | 1378 | 4491 | 4515 | 1259080 | 51 | `draw` 73.9 → 59.1 |
| `Level.cpp` | 1861 | 4459 | 4484 | 1256968 | 297 | `createShip` 74.1 → 60.4, `createScene` 88.3 → 61.3, `createMission` 90.7 → 60.7 |
| `Generator.cpp` | 1668 | 4491 | 4515 | 1259176 | 50 | `createMission` 74.3 → 56.9 |
| `MGame.cpp` | 2239 | 4468 | 4491 | 1257896 | 196 | `OnTouchEnd` 78.4 → 60.9, `OnUpdate` 99.9 → 51.9 |
| `HangarWindow.cpp` | 1378 | 4481 | 4505 | 1258312 | 169 | `render` 78.8 → 44.0 |
| `Globals.cpp` | 1378 | 4468 | 4492 | 1257064 | 111 | `Globals` 80.5 → 13.2 |
| `RocketGun.cpp` | 1378 | 4493 | 4519 | 1259432 | 10 | `update` 86.9 → 73.5 |
| `PlayerEgo.cpp` | 1378 | 4451 | 4478 | 1258696 | 142 | `calcCollision` 87.3 → 66.1 |
| `Hud.cpp` | 1428 | 4482 | 4506 | 1258408 | 141 | `draw` 89.1 → 58.0 |
| `Gun.cpp` | 2161 | 4488 | 4512 | 1259272 | 30 | `calcCharacterCollision` 90.0 → 74.7 |
| `MenuTouchWindow.cpp` | 1378 | 4484 | 4506 | 1257640 | 278 | `MenuTouchWindow` 94.2 → 69.6, `OnTouchBegin` 97.7 → 69.3, `OnTouchEnd` 97.9 → 44.5 |
| `SpaceLounge.cpp` | 1378 | 4487 | 4511 | 1257960 | 169 | `drawLounge` 95.5 → 61.3 |
| `ModStation.cpp` | 1378 | 4475 | 4498 | 1257864 | 198 | `ModStation` 98.4 → 57.0 |

### 4.2 `-fvisibility=protected` per TU

Protected keeps the exports but makes intra-TU calls direct, shrinking `.got`/`.plt`. Every
TU lowers the gate and (except for three unchanged targets) lowers its own target. The
original's `.got`/`.plt` are exactly the default-visibility values, so no TU is protected.

| TU | byte | linked | size | .text | .got | .plt | target base → protected |
|---|---:|---:|---:|---:|---:|---:|---|
| `Layout.cpp` | 1535 | 4339 | 4524 | 1259400 | 16972 | 43532 | 47.7 → 47.7 |
| `LevelScript.cpp` | 1457 | 4336 | 4524 | 1259432 | 17084 | 43868 | 52.0 → 51.8 |
| `Status.cpp` | 1457 | 4151 | 4524 | 1259352 | 16616 | 42464 | 57.7 → 57.7 |
| `Radar.cpp` | 1657 | 4360 | 4523 | 1259464 | 17076 | 43844 | 73.9 → 73.4 |
| `Level.cpp` | 1356 | 4253 | 4517 | 1259112 | 16640 | 42692 | 74.1 → 73.4, 88.3 → 86.4, 90.7 → 79.4 |
| `Generator.cpp` | 1547 | 4366 | 4524 | 1259400 | 17068 | 43820 | 74.3 → 73.9 |
| `MGame.cpp` | 1680 | 4396 | 4520 | 1259416 | 16988 | 43592 | 78.4 → 77.6, 99.9 → 98.5 |
| `HangarWindow.cpp` | 1581 | 4398 | 4520 | 1259416 | 17024 | 43712 | 78.8 → 78.1 |
| `Globals.cpp` | 1356 | 4256 | 4502 | 1257688 | 16944 | 43568 | 80.5 → 19.1 |
| `RocketGun.cpp` | 1457 | 4360 | 4520 | 1259432 | 17080 | 43868 | 86.9 → 86.7 |
| `PlayerEgo.cpp` | 1356 | 4278 | 4522 | 1259224 | 16524 | 42224 | 87.3 → 86.7 |
| `Hud.cpp` | 1654 | 4361 | 4523 | 1259368 | 16968 | 43544 | 89.1 → 88.7 |
| `Gun.cpp` | 1696 | 4450 | 4524 | 1259400 | 17040 | 43748 | 90.0 → 89.8 |
| `MenuTouchWindow.cpp` | 1616 | 4377 | 4524 | 1259432 | 16984 | 43568 | 94.2 → 92.7, 97.7 → 97.7, 97.9 → 97.4 |
| `SpaceLounge.cpp` | 1750 | 4329 | 4524 | 1259464 | 17000 | 43616 | 95.5 → 95.5 |
| `ModStation.cpp` | 1592 | 4409 | 4520 | 1259400 | 16996 | 43616 | 98.4 → 90.1 |

### 4.3 `-fvisibility=hidden` / `-fvisibility-inlines-hidden` per TU

Both hide exported symbols, so they are disproven for any TU that owns exported symbols. Four
representative TUs:

| TU | flag | byte | linked | size | missing | .text | .got | .plt |
|---|---|---:|---:|---:|---:|---:|---:|---:|
| `HangarWindow.cpp` | `-fvisibility=hidden` | 1575 | 4372 | 4484 | 40 | 1259416 | 17024 | 43712 |
| `HangarWindow.cpp` | `-fvisibility-inlines-hidden` | 1750 | 4450 | 4523 | 1 | 1259464 | 17160 | 44096 |
| `Gun.cpp` | `-fvisibility=hidden` | 1639 | 4421 | 4486 | 38 | 1259400 | 17040 | 43748 |
| `Gun.cpp` | `-fvisibility-inlines-hidden` | **4499** | **4499** | **4524** | **0** | 1259464 | 17164 | 44108 |
| `Level.cpp` | `-fvisibility=hidden` | 1320 | 4153 | 4393 | 131 | 1259112 | 16640 | 42692 |
| `Level.cpp` | `-fvisibility-inlines-hidden` | 1612 | 4453 | 4521 | 1 | 1259464 | 17156 | 44096 |
| `Globals.cpp` | `-fvisibility=hidden` | 1354 | 4160 | 4469 | 55 | 1257688 | 16944 | 43568 |
| `Globals.cpp` | `-fvisibility-inlines-hidden` | 1750 | 4448 | 4522 | 2 | 1259464 | 17156 | 44084 |

Note the `Gun.cpp` + `-fvisibility-inlines-hidden` cell: it is **gate-neutral and
`.so`-identical**. That is a genuine "unobservable" case: `Gun.cpp` has no exported inline
function, so the flag cannot be excluded for that TU. It is also irrelevant to codegen. The
same caveat applies to any TU with no exported inline functions.

### 4.4 Per-TU no-op flags

`-fno-plt` and `-fpic` per TU are whole-`.so` identical to base (only the build-id differs):

| TU | flag | byte | linked | size | sha == base |
|---|---|---:|---:|---:|---|
| `HangarWindow.cpp` | `-fno-plt` | 4499 | 4499 | 4524 | yes |
| `HangarWindow.cpp` | `-fpic` | 4499 | 4499 | 4524 | yes |
| `Gun.cpp` | `-fno-plt` | 4499 | 4499 | 4524 | yes |
| `Gun.cpp` | `-fpic` | 4499 | 4499 | 4524 | yes |

## 5. LLVM option matrix

clang 7.0.2's `-mllvm -help-hidden` was searched for PIC/relocation/GOT/PLT options. The
relevant ones were measured globally:

| option | byte | linked | size | .text | text relocs | .text == base |
|---|---:|---:|---:|---:|---:|---|
| `-mllvm -remat-pic-stub-load=true` | 4499 | 4499 | 4524 | 1259464 | 0 | yes |
| `-mllvm -remat-pic-stub-load=false` | 4499 | 4499 | 4524 | 1259464 | 0 | yes |
| `-mllvm -arm-promote-constant=false` | 4499 | 4499 | 4524 | 1259464 | 0 | yes |
| `-mllvm -arm-promote-constant=true` | 1386 | 4192 | 4221 | 1264712 | 0 | no |
| `-mllvm -arm-add-build-attributes=true` | 4499 | 4499 | 4524 | 1259464 | 0 | yes |
| `-mllvm -arm-add-build-attributes=false` | 4499 | 4499 | 4524 | 1259464 | 0 | yes |
| `-mllvm -old-thumb2-ifcvt=true` | 4499 | 4499 | 4524 | 1259464 | 0 | yes |
| `-mllvm -enable-arm-3-addr-conv=false` | 4499 | 4499 | 4524 | 1259464 | 0 | yes |
| `-mllvm -arm-synthesize-thumb-1-tbb=true` | 4499 | 4499 | 4524 | 1259464 | 0 | yes |
| `-mllvm -arm-load-store-opt=false` | 1225 | 3313 | 3906 | 1275432 | 0 | no |
| `-mllvm -arm-atomic-cfg-tidy=false` | 1409 | 4498 | 4523 | 1259496 | 0 | no |
| `-mllvm -arm-adjust-jump-tables=false` | 1571 | 4493 | 4517 | 1259944 | 0 | no |
| `-mllvm -arm-global-merge=false` | 1397 | 4489 | 4517 | 1259816 | 0 | no |

The neutral options are useful negatives: `-remat-pic-stub-load` (the option most directly
aimed at the lazy GOT-load pattern) does **not** change this tree's `.text` at all. The
original's lazy GOT load in `HangarWindow::render` is therefore not a missing LLVM option.

## 6. The two "known cases"

### 6.1 `HangarWindow::render` lazy GOT load at `0x149136`

Original:

```text
149132: ldr  r0, [pc, #748]   ; literal at 149420
149134: add  r0, pc
149136: ldr.w sl, [r0]        ; GOT slot for Globals::layout
14913a: ldr.w r0, [sl]        ; Globals::layout pointer
14913e: blx  61d40 <Layout::drawBG@plt>
```

Ours:

```text
1490f0: add  r1, pc           ; GOT slot for Globals::layout (hoisted to prologue)
1490f6: ldr.w sl, [r1]
149104: ldr.w r4, [sl]        ; value cached in r4
...
14913e: mov  r0, r4
149140: blx  61d40 <Layout::drawBG@plt>
```

Both are GOT-indirect. The difference is **when** the GOT slot and the pointer value are
loaded: the original reloads at the use site (`0x149134` and again at `0x14a85c`); ours hoists
the GOT load to the prologue and caches the pointer value in `r4` across the whole function.
That is a source-shape/lifetime difference (a local `Layout*` in our source vs a direct
`Globals::layout` access in the original), not a relocation-model difference. The GOT-symbol
multiset confirms it: `Globals::layout` 2 → 1. `-mllvm -remat-pic-stub-load` does not change
it (`.text` identical).

### 6.2 `Gun::calcCharacterCollision`

The function is fully PIC:

```text
152fa0: ldr  r0, [pc, #848]   ; GOT slot for __stack_chk_guard
152fa2: add  r0, pc
152fa4: ldr  r0, [r0]
...
152fac: ...                    ; Globals::Canvas is also GOT-indirect
...
15302c: blx  65850 <Player::getKIPlayer@plt>
```

Its only caller is `Gun::update` via PLT:

```text
1537e6: mov  r0, r5
1537e8: blx  673e0 <Gun::calcCharacterCollision@plt>
```

The GOT-symbol multiset and PLT-call multiset are **identical** to ours (5 GOT loads total,
including `__stack_chk_guard` 1 and `Globals::Canvas` 1; 59 PLT calls; 1 register call). The "non-PIC reachable" part of the
hypothesis is not supported: the function is reached through the PLT, and every global access
is GOT-indirect. Its 90.0% mismatch is register allocation/scheduling.

## 7. Verdicts and what remains

**Proven (positive evidence):**

1. The original is PIC with default visibility and semantic interposition on. Its own
   `.ARM.attributes` says `GOT_use: GOT-indirect`; it has zero text relocations; its
   relocation-type counts are exactly reproduced; `.got`, `.plt`, `.rel.dyn`, `.rel.plt`,
   `.dynsym`, `.dynstr`, `.hash` are byte-identical.
2. No TU was compiled `-fno-pic`/`-fno-PIC`: one such TU flips the linked attribute to
   `GOT_use: direct` and adds text relocations; the original has neither.
3. No TU was compiled with `-fvisibility=hidden` or `-fvisibility=protected` (for the 16 TUs
   that own the 25 targets): hidden loses exports, protected collapses `.got`/`.plt`; the
   original matches the default-visibility build exactly.
4. `-fpic`, `-fno-plt` and `-mpie-copy-relocations` are codegen-identical to `-fPIC` on this
   target; the binary cannot distinguish them. The repo's `-fPIC` is valid and conventional.
5. `-mllvm -relocation-model=...`, `-mllvm -arm-dynamic-no-pic` and
   `-fno-semantic-interposition` do not exist in clang 7.0.2, so they cannot be the original
   build's flags.
6. No PIC/visibility/relocation flag raises `byte_exact` or any of the 25 target fuzzy values.
   The remaining 25 functions are source-shape work.

**Not excluded / unobservable:**

1. `-fpic` vs `-fPIC`: identical codegen on this target.
2. `-fno-plt` under `-fPIC`: identical codegen.
3. `-fvisibility-inlines-hidden` on a TU with no exported inline functions (e.g. `Gun.cpp`):
   gate-neutral and `.so`-identical.
4. `-fvisibility=protected` on a leaf TU with no intra-TU calls to its own exported functions:
   could leave `.got`/`.plt` unchanged. No positive evidence for it, and the default model
   explains everything.
5. `-mllvm -remat-pic-stub-load`, `-arm-promote-constant=false`,
   `-arm-add-build-attributes`, `-old-thumb2-ifcvt`, `-enable-arm-3-addr-conv=false`,
   `-arm-synthesize-thumb-1-tbb`: codegen-neutral on this tree, so they cannot be excluded,
   but they also cannot be detected or used to improve the gate.

**Recommendation:** keep the current `-fPIC` and default visibility. Do not spend another wave
on PIC/visibility/relocation flags. The next work on the 25 functions is source-shape:
`HangarWindow::render`'s `Globals::layout` lifetime, `Gun::calcCharacterCollision`'s register
allocation, and the already-known TU/expression-order queue.

## 8. Reproduction artifacts

Scratch (not tracked):

```text
/dev/shm/w4308/toolchain_extra.cmake       global A/B wrapper
/dev/shm/w4308/per_tu.cmake                per-TU COMPILE_FLAGS wrapper
/dev/shm/w4308/build_seq.py                sequential build driver
/dev/shm/w4308/build_seq/                  shared sequential build dir
/dev/shm/w4308/verify_all.py               parallel official-verifier driver
/dev/shm/w4308/verify_results.json         all official verifier summaries + 25 target pcts
/dev/shm/w4308/res-*.json                  per-variant build metadata (sections, text relocs, sha)
/dev/shm/w4308/sos/*.so                    every variant's linked .so
/dev/shm/w4308/got_scan2.py, got_scan3.py  GOT/PLT classifier
/dev/shm/w4308/o3d.json, u3d.json          per-function GOT/PLT detail (orig / ours)
```

The repo `check` target was re-run after the research: `RATCHET PASS` at
`byte 4499 / linked 4499`, `size 4524/4524`, `data 843/843`, `imports 207`, lint `CLEAN`,
drift 0. `report.json` was not committed.
