# ARM scheduling / CPU tuning model (w4302, wave 93)

Status: research note, 2026-11-03. Worktree `wt/w4302`, branch `wave93-w4302`,
base `eea2e3c1`. Scratch is `/dev/shm/w4302`. **No build-flag or source change is
landed**: no variant strictly raises the gate (best is 4499/4499, equal to the
baseline). This commit contains this note only.

## 0. Bottom line

The original's ARM scheduling model is **clang 7.0.2's generic ARMv7-A model**,
with the **default pre-RA `list-hybrid` scheduler** and the **default post-RA
pipeline** (old `PostRAScheduler` + post-RA machine sinking and LICM). This is
positive evidence from the binary, not just "the gate did not improve":

* The original's `.ARM.attributes` is byte-identical to our generic build
  (`0x38` bytes; no `Tag_MPextension_use`, no `Tag_DIV_use`, no
  `Tag_Virtualization_use`). **Every real ARMv7 CPU tested adds at least one of
  those tags to the linked `.so`** (Cortex-A8 adds `Tag_Virtualization_use:
  TrustZone`; Cortex-A5/A9 add `Tag_MPextension_use` + TrustZone; Cortex-A7/A12/
  A15/A17 add MPextension + DIV + TrustZone/Virtualization). The original is not
  any of them.
* `.text` is exactly `0x1337c8` for generic and for **no** other CPU model
  (A8 `0x133fc8`, A9 `0x1336c8`, A15 `0x133168`, A7 `0x133da8`, A5 `0x133f08`,
  A12/A17 `0x132f28`).
* 4499/4524 functions are byte-exact under generic; every other CPU model loses
  at least 3131 byte-exact functions.
* Explicit `-mllvm -pre-RA-sched=list-hybrid` is byte-identical to the default;
  `list-burr`, `source` and `list-ilp` lose at least 3238 byte-exact functions.
* `-mtune` is a **no-op in clang 7.0.2**. The driver accepts it but cc1 has no
  `-tune-cpu` option; `-mtune=cortex-a8` still passes `-target-cpu generic`;
  `-mtune=bogus-cpu` is silently accepted; both the linux and darwin r18b clang
  binaries contain no `-tune-cpu` string. All four `-mtune` variants produce
  byte-identical code sections. A non-default `-mtune` therefore cannot explain
  any byte in the original, whether or not it was on the command line.
* 74 builds measured (73 candidate flags + the baseline): **43 candidate flags
  are exact no-ops** (byte-identical `.text`/`.rodata`/`.data`/`.ARM.exidx`/
  `.ARM.extab`/dyn tables), **30 degrade**, and **none beats 4499/4499/4524**. No scheduling knob improves
  any of the 25 non-exact functions by more than `+0.5` (Hud::draw with
  `-mllvm -disable-ifcvt-simple`), and that variant costs 2980 byte-exact
  functions globally.
* The remaining 25 functions are **not scheduling-model-driven**: 23 of 25 have
  a different normalized opcode multiset from the original (instruction
  selection / register allocation / source shape), and no CPU model or
  scheduler choice moves them up.

## 1. Method

Each variant gets its own scratch build directory under `/dev/shm/w4302`. A
wrapper toolchain includes the real `cmake/toolchain.cmake` and appends exactly
one candidate flag to both `CMAKE_CXX_FLAGS` and `CMAKE_C_FLAGS`:

```cmake
# /dev/shm/w4302/toolchain_extra.cmake
include("/home/debian/GalaxyOnFire2/wt/w4302/cmake/toolchain.cmake")
set(GOF2_EXTRA_FLAGS "" CACHE STRING "extra matching flags")
if(GOF2_EXTRA_FLAGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GOF2_EXTRA_FLAGS}" CACHE STRING "matching ARM compile flags" FORCE)
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${GOF2_EXTRA_FLAGS}" CACHE STRING "matching ARM C compile flags" FORCE)
endif()
```

Per variant:

```sh
cmake -S /home/debian/GalaxyOnFire2/wt/w4302 -B /dev/shm/w4302/build-<tag> -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/dev/shm/w4302/toolchain_extra.cmake \
  -DGOF2_MATCH_OPT=-Oz -DGOF2_EXTRA_FLAGS='<flag>' \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build /dev/shm/w4302/build-<tag> --target link_so -j64
python3 tools/verify/verify.py --build-dir /dev/shm/w4302/verify-<tag> \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /dev/shm/w4302/build-<tag>/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
```

`size-exact` is `sum(osize == usize)` over the verifier rows. `.text`/`.rodata`
are `readelf -SW` sizes of the linked `.so`. "Sections identical" means
`.text`, `.rodata`, `.data`, `.ARM.exidx`, `.ARM.extab`, `.dynsym`, `.dynstr`,
`.rel.plt` and `.rel.dyn` are byte-identical to the baseline build (compared
with the NDK `objcopy -O binary --only-section=...`). The `.so` sha256 is **not**
a useful identity here: `-g` embeds the build directory, so the build-id and
therefore the whole-file hash differ between scratch directories even when every
code section is identical.

The canonical `cmake-build-match` was not disturbed. Its baseline and final
`check` are both green (see §2).

## 2. Baseline

```text
verify: byte 4499  linked 4499  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
=== DRIFT: 0 field(s) across 0 class(es) ===
```

`size-exact` 4524/4524. `.text 0x1337c8 @ 0x69440`, `.rodata 0x41b67 @
0x1c9e10`, `.ARM.exidx 0x6f70`, `.ARM.extab 0x26298`. The scratch baseline
(`build-base`) reproduces 4499/4499/4524 with byte-identical code sections; only
the build-id differs from `cmake-build-match` (build-directory path in `-g`).

The 25 non-exact functions are the current `report.json` set (same list as
`docs/IDENTICALITY.md`, with Hud::draw at 89.1 and LevelScript::process at 52.0
in this report).

## 3. `-mtune` is a no-op in clang 7.0.2

### 3.1 Compiler evidence

```sh
CL=.cache/ndk/android-ndk-r18b/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++
$CL -target armv7-none-linux-androideabi16 -march=armv7-a -mthumb -mfpu=neon \
    -mfloat-abi=softfp -Oz -mtune=cortex-a8 -### -c t.cpp -o t.o 2>&1 | tail -1
#   ... "-target-cpu" "generic" ...          <- no -tune-cpu, CPU unchanged
$CL ... -mcpu=cortex-a8 -### ...
#   ... "-target-cpu" "cortex-a8" ...
$CL -target armv7-none-linux-androideabi16 -mtune=bogus-cpu -c t.cpp -o /dev/null
#   rc=0, no diagnostic                     <- value not even validated
$CL -cc1 --help | grep -c -- '-tune-cpu'
#   0
strings -a .../prebuilt/linux-x86_64/bin/clang  | grep -c -- '-tune-cpu'   # 0
strings -a .../prebuilt/darwin-x86_64/bin/clang | grep -c -- '-tune-cpu'   # 0
```

Clang 7.0.2 predates the `-tune-cpu` cc1 option (added in later LLVM). On ARM
the driver's `-mtune` is accepted and dropped. The darwin-host clang binary has
no `-tune-cpu` string either, so the original's (darwin) build host behaves the
same.

### 3.2 A/B measurement

| variant | byte | linked | size-exact | avg | `.text` | `.rodata` | sections identical |
|---|---:|---:|---:|---:|---:|---:|---|
| base (`-march=armv7-a`, no `-mtune`) | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | `0x41b67` | — |
| `-mtune=cortex-a8` | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | `0x41b67` | **yes** |
| `-mtune=cortex-a9` | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | `0x41b67` | **yes** |
| `-mtune=cortex-a15` | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | `0x41b67` | **yes** |
| `-mtune=cortex-a7` | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | `0x41b67` | **yes** |

**Verdict:** `-mtune=<cpu>` is *disproven as a mechanism*. It cannot have
changed the original's codegen. Whether the original command line literally
contained one is unknowable and irrelevant.

## 4. `-mcpu` and `.ARM.attributes`: the original is generic ARMv7-A

### 4.1 The attribute witness

The task suggested checking `.ARM.attributes` for a `Tag_CPU_name` change. That
specific check is **misleading**: gold's attribute merge keeps `Tag_CPU_name:
"ARM v7"` at `.so` level even when every app TU was compiled `-mcpu=cortex-a8`
(the app object says `"cortex-a8"`; the linked `.so` still says `"ARM v7"`).
The discriminating attributes are the *extra* tags, which do survive the merge:

| build | `.ARM.attributes` size | extra tags vs original |
|---|---:|---|
| **original** | `0x38` | — |
| base / `-mcpu=generic` | `0x38` | **none (byte-identical)** |
| `-mcpu=cortex-a8` | `0x3a` | `Tag_Virtualization_use: TrustZone` |
| `-mcpu=cortex-a5` | `0x3c` | `Tag_MPextension_use: Allowed`, `Tag_Virtualization_use: TrustZone` |
| `-mcpu=cortex-a9` | `0x3c` | same as A5 |
| `-mcpu=cortex-a7` | `0x3e` | `Tag_MPextension_use`, `Tag_DIV_use: Allowed in v7-A with integer division extension`, `Tag_Virtualization_use: TrustZone and Virtualization Extensions` |
| `-mcpu=cortex-a12` | `0x3e` | same as A7 |
| `-mcpu=cortex-a15` | `0x3e` | same as A7 |
| `-mcpu=cortex-a17` | `0x3e` | same as A7 |

(`Tag_CPU_name` remains `"ARM v7"` in every row; the extra tags are the
witness. The original's `.ARM.attributes` bytes are identical to our base
build's.)

### 4.2 Gate measurement

| variant | byte | linked | size-exact | avg | `.text` | `.rodata` | sections identical |
|---|---:|---:|---:|---:|---:|---:|---|
| base | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | `0x41b67` | — |
| `-mcpu=generic` | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | `0x41b67` | **yes** |
| `-mcpu=cortex-a5` | 1368 | 4383 | 4403/4524 | 99.27 | `0x133f08` | `0x41b67` | no |
| `-mcpu=cortex-a7` | 1330 | 4181 | 4330/4524 | 98.41 | `0x133da8` | `0x41b67` | no |
| `-mcpu=cortex-a8` | 1330 | 4225 | 4366/4524 | 98.82 | `0x133fc8` | `0x41b67` | no |
| `-mcpu=cortex-a9` | 1219 | 2944 | 4052/4524 | 92.76 | `0x1336c8` | `0x41b67` | no |
| `-mcpu=cortex-a12` | 1212 | 2937 | 4091/4524 | 92.73 | `0x132f28` | `0x41b67` | no |
| `-mcpu=cortex-a15` | 1201 | 2927 | 4068/4524 | 92.6 | `0x133168` | `0x41b67` | no |
| `-mcpu=cortex-a17` | 1212 | 2937 | 4091/4524 | 92.73 | `0x132f28` | `0x41b67` | no |

**Verdict:** the original used the **generic ARMv7-A** CPU model
(`-march=armv7-a`, no `-mcpu`, or equivalently `-mcpu=generic`). Every real
ARMv7 CPU is disproven by both `.ARM.attributes` and the gate.

## 5. Scheduler selection

Clang 7.0.2's ARM backend uses the **old pre-RA list scheduler**, not the
MachineScheduler: `-mllvm -enable-misched=false` is an exact no-op, and every
`-mllvm -misched=...` choice (converge/ilpmax/ilpmin) and
`-misched-bottomup`/`-topdown` is an exact no-op. (Either the pre-RA
MachineScheduler is not run for this target at `-Oz`, or its output is
identical; in either case it is not a codegen lever here.) The default pre-RA scheduler
is `list-hybrid`: selecting it explicitly is byte-identical, while the other
three schedulers are far worse.

| variant | byte | linked | size-exact | avg | `.text` | sections identical | 25-target movement |
|---|---:|---:|---:|---:|---:|---|---|
| base | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | — | — |
| `-mllvm -pre-RA-sched=list-hybrid` | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | **yes** | none |
| `-mllvm -pre-RA-sched=list-burr` | 1261 | 2744 | 4088/4524 | 90.69 | `0x132f68` | no | 25 moved, −55.8..−1.8 |
| `-mllvm -pre-RA-sched=source` | 1216 | 2576 | 3987/4524 | 89.42 | `0x132a68` | no | 25 moved, −60.7..−3.8 |
| `-mllvm -pre-RA-sched=list-ilp` | 1301 | 2825 | 3937/4524 | 90.7 | `0x1337c8` | no | 25 moved, −62.7..−2.4 |
| `-mllvm -misched=converge` | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | **yes** | none |
| `-mllvm -misched=ilpmax` | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | **yes** | none |
| `-mllvm -misched=ilpmin` | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | **yes** | none |
| `-mllvm -misched-bottomup` | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | **yes** | none |
| `-mllvm -misched-topdown` | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | **yes** | none |
| `-mllvm -enable-misched=false` | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | **yes** | none |
| `-mllvm -misched-postra` | 1575 | 3982 | 4505/4524 | 99.51 | `0x1337c8` | no | 23 moved, −1.8..−0.2 |
| `-mllvm -enable-post-misched` | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | **yes** | none |
| `-mllvm -enable-post-misched=false` | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | **yes** | none |
| `-mllvm -disable-post-ra` | 1216 | 2331 | 4310/4524 | 94.15 | `0x133868` | no | 25 moved, −23.0..−1.6 |

**Verdicts:** default pre-RA scheduler = `list-hybrid` (**proven**);
pre-RA MachineScheduler is unused (**proven** by the neutral `-enable-misched=false`);
post-RA MachineScheduler (`-misched-postra`) is off in the original
(**disproven** as an original flag: enabling it is worse); the old
`PostRAScheduler` is on and load-bearing (**proven** by `-disable-post-ra`).

## 6. Scheduling heuristics and post-RA passes

| variant | byte | linked | size-exact | avg | `.text` | sections identical | 25-target movement |
|---|---:|---:|---:|---:|---:|---|---|
| base | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | — | — |
| `-mllvm -break-anti-dependencies=none` | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | **yes** | none |
| `-mllvm -break-anti-dependencies=all` | 1502 | 3570 | 4500/4524 | 98.2 | `0x133828` | no | 23 moved, −10.0..−0.4 |
| `-mllvm -disable-2addr-hack` | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | **yes** | none |
| `-mllvm -twoaddr-reschedule=false` | 1608 | 4490 | 4515/4524 | 99.88 | `0x1337e8` | no | 1 moved, −0.2 |
| `-mllvm -disable-sched-hazard` | 1383 | 3378 | 4250/4524 | 95.76 | `0x1331e8` | no | 24 moved, −49.0..−0.6 |
| `-mllvm -disable-sched-cycles` | 1379 | 3397 | 4269/4524 | 95.9 | `0x13352c` | no | 24 moved, −48.1..−0.6 |
| `-mllvm -disable-sched-physreg-join` | 1633 | 4481 | 4515/4524 | 99.86 | `0x1337a8` | no | 2 moved, −7.7..−0.3 |
| `-mllvm -disable-sched-reg-pressure` | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | **yes** | none |
| `-mllvm -disable-sched-critical-path` | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | **yes** | none |
| `-mllvm -disable-sched-height` | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | **yes** | none |
| `-mllvm -disable-sched-live-uses` | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | **yes** | none |
| `-mllvm -disable-sched-stalls` | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | **yes** | none |
| `-mllvm -disable-sched-vrcycle` | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | **yes** | none |
| `-mllvm -max-sched-reorder=0` | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | **yes** | none |
| `-mllvm -disable-dfa-sched` | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | **yes** | none |
| `-mllvm -scheditins` | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | **yes** | none |
| `-mllvm -schedmodel` | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | **yes** | none |
| `-mllvm -misched-cluster=false` | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | **yes** | none |
| `-mllvm -misched-regpressure=false` | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | **yes** | none |
| `-mllvm -misched-cyclicpath=false` | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | **yes** | none |
| `-mllvm -misched-fusion=true` | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | **yes** | none |
| `-mllvm -use-tbaa-in-sched-mi` | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | **yes** | none |
| `-mllvm -enable-aa-sched-mi` | 1408 | 4021 | 4498/4524 | 99.44 | `0x1337e8` | no | 16 moved, −6.0..**+0.1** |
| `-mllvm -disable-machine-sink` | 1235 | 3734 | 4019/4524 | 95.02 | `0x134788` | no | 22 moved, −49.6..−0.2 |
| `-mllvm -disable-postra-machine-sink` | 1573 | 4394 | 4473/4524 | 99.75 | `0x134d68` | no | 8 moved, −4.4..−0.2 |
| `-mllvm -disable-postra-machine-licm` | 2292 | 4480 | 4520/4524 | 99.89 | `0x1337e8` | no | 2 moved, −3.8..−0.1 |
| `-mllvm -machine-sink-bfi` | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | **yes** | none |
| `-mllvm -enable-andcmp-sinking` | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | **yes** | none |
| `-mllvm -disable-ifcvt-diamond` | 1428 | 4400 | 4459/4524 | 99.23 | `0x1338c8` | no | 7 moved, −5.8..−0.1 |
| `-mllvm -disable-ifcvt-simple` | 1519 | 4366 | 4456/4524 | 99.32 | `0x133728` | no | 1 moved, **+0.5** |
| `-mllvm -disable-ifcvt-triangle` | 4220 | 4494 | 4521/4524 | 99.89 | `0x1337c8` | no | 1 moved, −0.1 |
| `-mllvm -disable-early-ifcvt` | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | **yes** | none |
| `-mllvm -old-thumb2-ifcvt` | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | **yes** | none |

**Verdicts:** the default heuristics are the original's: `break-anti-dependencies
= none` (**proven**: `none` is byte-identical, `all` is much worse),
`twoaddr-reschedule = true` (**proven**), hazard detection / cycle precision /
physreg join on (**proven**), machine sinking / post-RA machine sinking /
post-RA machine LICM on (**proven**), if-conversion (diamond/simple/triangle) on
(**proven**). Register-pressure / critical-path / height / live-use / stall /
vrcycle priorities, `max-sched-reorder`, DFA scheduling, `scheditins` vs
`schedmodel`, and the MachineScheduler heuristics are **codegen-neutral** on this
tree at `-Oz` (they cannot be distinguished, but none is needed). `-enable-aa-sched-mi`
is the only variant with a positive target delta (LevelScript::process +0.1) and
it costs 3091 byte-exact.

## 7. ARM backend (`-mllvm -arm-*`) knobs

Enumerated with `clang++ -mllvm -help-hidden` (the full list is in
`/dev/shm/w4302/mllvm-help.txt`). Tested one at a time:

| variant | byte | linked | size-exact | avg | `.text` | sections identical | 25-target movement |
|---|---:|---:|---:|---:|---:|---|---|
| base | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | — | — |
| `-mllvm -arm-load-store-opt=false` | 1225 | 3313 | 3906/4524 | 94.71 | `0x137628` | no | 25 moved, −44.1..−0.7 |
| `-mllvm -arm-adjust-jump-tables=false` | 1571 | 4493 | 4517/4524 | 99.87 | `0x1339a8` | no | 1 moved, −3.4 |
| `-mllvm -arm-global-merge=false` | 1397 | 4489 | 4517/4524 | 99.82 | `0x133928` | no | none of the 25 |
| `-mllvm -arm-use-mulops=false` | 1545 | 4461 | 4503/4524 | 99.79 | `0x133588` | no | 9 moved, −16.9..−0.1 |
| `-mllvm -arm-assume-misaligned-load-store` | 1225 | 3404 | 3976/4524 | 95.14 | `0x136b48` | no | 22 moved, −41.5..−0.3 |
| `-mllvm -arm-constant-island-max-iteration=2` | 4444 | 4497 | 4522/4524 | 99.9 | `0x1337c8` | no | none of the 25 |
| `-mllvm -arm-constant-island-max-iteration=0` | — | — | — | — | — | — | build failure: "Constant Island pass failed to converge!" |
| `-mllvm -arm-atomic-cfg-tidy` | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | **yes** | none |
| `-mllvm -arm-promote-constant=false` | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | **yes** | none |
| `-mllvm -enable-arm-3-addr-conv=false` | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | **yes** | none |
| `-mllvm -arm-add-build-attributes` | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | **yes** | none |
| `-mllvm -arm-synthesize-thumb-1-tbb` | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | **yes** | none |
| `-mllvm -arm-implicit-it=never` | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | **yes** | none |
| `-mllvm -arm-implicit-it=always` | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | **yes** | none |
| `-mllvm -arm-interworking=false` | 4499 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | **yes** | none |

**Verdicts:** ARM load/store optimization, jump-table layout adjustment,
global merge and `mul`-ops are on in the original (**proven** by their
disabling); `arm-assume-misaligned-load-store` is off by default and enabling
it is worse; constant-island iteration default is 1 (0 crashes, 2 is worse);
the remaining `-arm-*` knobs are codegen-neutral.

## 8. Per-TU scheduling probes

A per-TU harness (`COMPILE_FLAGS` on one source file, appended after the
repo's per-file `COMPILE_OPTIONS`) tested the five flags that moved the 25
targets globally on the eight TUs that own the near-miss functions. **No cell
improved a target except two +0.1/+0.5 artifacts**; every other cell is ≤ 0.

| TU | flag | target | base % | per-TU % | Δ |
|---|---|---|---:|---:|---:|
| `LevelScript.cpp` | `-mllvm -enable-aa-sched-mi` | `LevelScript::process` | 52.0 | 52.1 | **+0.1** |
| `Hud.cpp` | `-mllvm -disable-ifcvt-simple` | `Hud::draw` | 89.1 | 89.6 | **+0.5** |
| `Status.cpp` | `-mllvm -misched-postra` | `Status::missionCompleted` | 57.7 | 57.7 | 0.0 |
| `Status.cpp` | `-mllvm -disable-postra-machine-sink` | `Status::missionCompleted` | 57.7 | 57.7 | 0.0 |
| `MGame.cpp` | `-mllvm -disable-postra-machine-sink` | `MGame::OnUpdate` | 99.9 | 99.9 | 0.0 |
| `Gun.cpp` | `-mllvm -disable-postra-machine-sink` | `Gun::calcCharacterCollision` | 90.0 | 90.0 | 0.0 |
| `MenuTouchWindow.cpp` | `-mllvm -disable-postra-machine-sink` | `OnTouchBegin` / `OnTouchEnd` | 97.7 / 97.9 | 97.7 / 97.9 | 0.0 |
| `Globals.cpp` | `-mllvm -disable-postra-machine-sink` | `Globals::Globals` C1/C2 | 80.5 | 80.5 | 0.0 |
| `MenuTouchWindow.cpp` | `-mllvm -enable-aa-sched-mi` | `MenuTouchWindow` C1/C2 | 94.2 | 94.2 | 0.0 |
| all other 41 cells (40 in the first matrix + 10 in the second, minus the 9 shown) | — | — | — | ≤ base | ≤ 0 |

The Hud result is the only clean per-TU win: `-mllvm -disable-ifcvt-simple` on
`Hud.cpp` changes **only** `Hud::draw` (89.1 → 89.6) and keeps the full gate at
4499/4499/4524. It is a fuzzy-only improvement (the function is still not
byte-exact), so it is **not landed** under this wave's "strictly improves the
gate" rule. It is a useful source-shape lead: Hud's remaining diff includes an
if-conversion-shape difference, not a global scheduling model.

## 9. What the 25 remaining functions look like

For each of the 25, the original and our function were disassembled with the
official `verify.disassemble` and their **normalized opcode multisets** compared
(strip `.w`/`.n`, unify `movs`/`mov`, `adds`/`add`, `itt*`→`it`, etc.):

* **23 of 25 differ** in opcode mix; only `ModStation::ModStation` C1/C2 have an
  identical normalized multiset (and they are the two closest at 98.4%).
* The differences are instruction-selection / encoding / register-allocation
  differences, e.g. `vldr` vs `vmov` (float constant materialization),
  `ldr.w` vs `ldr`, `movs` vs `mov`, `itt` vs `itttt`, `nop` padding.
* This is consistent with the w3751 result: the 25 are size-exact and their
  first differences are callee-saved register sets, table-base registers,
  instruction scheduling around a table load, and basic-block order — all
  source-shape/regalloc effects, not a different machine model.

The full mnemonic-multiset table is reproducible with
`/dev/shm/w4302/mnemonic_norm.py <ours.so>` (scratch, not tracked).

## 10. Verdicts per hypothesis

| hypothesis | verdict | evidence |
|---|---|---|
| Original used `-mtune=cortex-a8/a9/a15/a7` | **disproven as a mechanism** | clang 7.0.2 has no `-tune-cpu`; `-###` keeps `-target-cpu generic`; bogus CPU accepted; all four variants byte-identical |
| Original used `-mcpu=cortex-a8` | **disproven** | `.ARM.attributes` +2 B (`Tag_Virtualization_use: TrustZone`); byte 1330; `.text 0x133fc8` |
| Original used `-mcpu=cortex-a5/a7/a9/a12/a15/a17` | **disproven** | extra MPextension/DIV/Virtualization tags; byte 1201–1368; `.text` ≠ `0x1337c8` |
| Original used the generic ARMv7-A model | **proven** | `.ARM.attributes` byte-identical; `.text 0x1337c8`; 4499 byte-exact; `-mcpu=generic` byte-identical |
| Original used a non-default pre-RA scheduler | **disproven** | `list-hybrid` byte-identical; burr/source/ilp lose ≥3238 |
| Original used the MachineScheduler pre-RA | **disproven/irrelevant** | `-enable-misched=false` and every `-misched=...` choice are exact no-ops |
| Original used `-misched-postra` | **disproven** | enabling it drops byte 4499 → 1575 |
| Original used `-enable-post-misched` | **unknown but codegen-neutral** | default true; `false` is an exact no-op |
| Original disabled a scheduling heuristic/pass | **disproven** for hazard, cycles, physreg-join, twoaddr-reschedule, machine-sink, post-RA sink, post-RA LICM, ifcvt diamond/simple/triangle, ARM load/store opt, jump-table adjust, global merge, mulops | each disable is worse |
| Original used a non-default `-arm-*` knob | **unknown but codegen-neutral** for atomic-cfg-tidy, promote-constant, 3-addr-conv, add-build-attributes, synth-thumb1-tbb, implicit-it, interworking | exact no-ops |
| The 25 remaining functions are scheduling-model-driven | **disproven** | no CPU/scheduler knob improves them; opcode multisets differ in 23/25 |
| The 25 remaining functions are source-shape/regalloc-driven | **likely** | consistent with w3751 and the opcode-multiset analysis |

## 11. Reproduction

```sh
# baseline
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8

# one variant (wrapper + verifier commands in §1)
python3 /dev/shm/w4302/run_variant.py mtune-cortex-a8 "-mtune=cortex-a8"

# attribute witness
RE=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-readelf
$RE -A /dev/shm/w4302/build-mcpu-cortex-a8/libgof2hdaa.so | tail -3
$RE -A /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so | tail -3

# per-TU probe
/dev/shm/w4302/run_pt.sh src/game/ui/Hud.cpp "-mllvm -disable-ifcvt-simple" \
  _ZN3Hud4drawExxP9PlayerEgobjj
```

Scratch artifacts (not tracked): `/dev/shm/w4302/toolchain_extra.cmake`,
`run_variant.py`, `results.jsonl`, `results_full.json`, `run_pt.sh`,
`per_tu_flags.cmake`, `score_targets.py`, `mnemonic_norm.py`, `pt_results*.txt`,
`mllvm-help.txt`, and the per-variant `build-*`/`verify-*` directories.

## 12. What remains

* No build-flag change is justified. The original's scheduling model is the
  clang 7.0.2 generic ARMv7-A default; the current tree already uses it.
* The 25 non-exact functions need source-shape / register-allocation work, not
  a scheduling flag. The two per-TU fuzzy leads (§8) are Hud::draw
  (`-disable-ifcvt-simple`, +0.5) and LevelScript::process
  (`-enable-aa-sched-mi`, +0.1); both are compensations for our source shape,
  not original build flags, and neither raises the gate.
* `-mtune` can be ignored permanently for this compiler: it is not a
  codegen-relevant flag in clang 7.0.2. `-mcpu` is the only CPU-model lever and
  it is already pinned to generic by `.ARM.attributes` and `.text` size.
