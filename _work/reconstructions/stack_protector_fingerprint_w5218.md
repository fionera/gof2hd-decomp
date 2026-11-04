# Stack-protector fingerprint (w5218)

Status: research note, 2026-11-04. Worktree `/home/debian/GalaxyOnFire2/wt/w5218`,
branch `wave104-w5218`, base `8c5fd3646`. Scratch: `/dev/shm/w5218`.
No `src/`, build-file, tool, baseline or allowlist change landed: no strict gate
improvement exists. This commit contains only this note.

## 0. Bottom line

The original `libgof2hdaa.so` was compiled with **`-fstack-protector-strong`** for the
app C and C++ TUs.  The shipped binary has **678 canary-bearing functions / 787
`__stack_chk_fail` call sites**.  A controlled full rebuild with
`-fstack-protector-strong` reproduces that set **exactly** (same 678 function names,
same per-function call counts, same guard-load/save registers for every byte-exact
function).  The alternatives are decisively wrong:

| flag | canary functions | call sites | official `byte_exact` | official `linked_exact` |
|---|---:|---:|---:|---:|
| `-fstack-protector-strong` (current) | **678** | **787** | **4500** | **4500** |
| `-fstack-protector` (basic) | 29 | 88 | 1369 | 3785 |
| `-fstack-protector-all` | 3960 | 4421 | 91 | 964 |
| `-fno-stack-protector` | 25 | 84 | 1369 | 3780 |
| `-fstack-check` (added) | 678 | 787 | 4500 (fast counter) | 4500 (fast counter) |

`-fstack-check` is **neutral/unobservable**: the whole `.so` is byte-identical to the
current build except `.note.gnu.build-id`.  `-fstack-clash-protection` and every
`-mstack-protector-guard=...` spelling are **rejected by clang 7.0.2** on this target.
There are **no per-TU exceptions** among the 156 app TUs that contain at least one
canary-eligible function; the other 36 app TUs have no canary-eligible function, so a
per-TU flag difference there is unobservable (but no positive evidence for one).

## 1. Baseline gate

Configured for this nested worktree (the preset's default `../_work` path resolves to
`wt/_work`, not the shared `_work`):

```sh
cd /home/debian/GalaxyOnFire2/wt/w5218
export TMPDIR=/dev/shm/w5218/tmp
cmake --preset default
cmake -S . -B cmake-build-match \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
cmake --build cmake-build-match --target lint -j6
cmake --build cmake-build-match --target drift -j6
```

Raw result before and after the experiments:

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
size-exact 4524 / 4524
lint CLEAN ✓ (no new soft violations; hard invariants 0)
=== DRIFT: 0 field(s) across 0 class(es) ===
```

The 24 non-byte-exact functions are unchanged by this research.  Fifteen of them are
canary-bearing; their canary call counts still match the original exactly (only their
source-shape bytes differ).

## 2. Direct evidence in the shipped binary

### 2.1 Imports and relocation kind

```sh
RE=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-readelf
$RE --dyn-syms -W /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so | grep stack_chk
$RE -rW /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so | grep stack_chk
```

```text
36: 00000000 0 FUNC   GLOBAL DEFAULT UND __stack_chk_fail@LIBC (2)
37: 00000000 0 OBJECT GLOBAL DEFAULT UND __stack_chk_guard@LIBC (2)
0020fd04  00002515 R_ARM_GLOB_DAT  00000000 __stack_chk_guard@LIBC
002106a4  00002416 R_ARM_JUMP_SLOT 00000000 __stack_chk_fail@LIBC
```

There is **no TLS relocation** for `__stack_chk_guard` (the original has no
`R_ARM_TLS_*` relocations at all).  The guard is the default **global GOT** form, not
the TLS form.  The code loads the GOT slot and then dereferences it twice:

```text
6b3bc: 485f        ldr   r0, [pc, #380]   ; literal -> GOT slot for __stack_chk_guard
6b3c0: 4478        add   r0, pc
6b3c2: f8d0 8000   ldr.w r8, [r0]         ; r8 = &__stack_chk_guard
6b3c6: f8d8 0000   ldr.w r0, [r8]         ; r0 = guard value
6b3ca: 9008        str   r0, [sp, #32]     ; save canary
```

Epilogue (`InitializeBlowfish`, original `0x6b3b1`, size 416):

```text
ldr   r0, [sp, #32]
ldr   r1, [r8]
subs  r0, r1, r0
ittt  eq
ldmiaeq.w sp!, {r1, r2, r3, r4, r5, r7, lr}
addeq sp, #4
bxeq  lr
blx   __stack_chk_fail@plt
```

The same pattern is present in the current build byte-for-byte for every canary-bearing
function that is already byte-exact.

### 2.2 Exact canary counts

A scanner over the NDK `objdump -d` output (reproduced in §7) gives:

| binary | canary functions | call sites | app-TU functions | library functions | zero-size alias |
|---|---:|---:|---:|---:|---:|
| original | **678** | **787** | **652** | **25** | 1 (`_ZN11AbyssEngine13getAppVersionEv`) |
| current `-strong` build | **678** | **787** | **652** | **25** | 1 |
| `-fstack-protector` basic | 29 | 88 | 4 | 25 | 0 |
| `-fstack-protector-all` | 3960 | 4421 | 3934 | 25 | 1 |
| `-fno-stack-protector` | 25 | 84 | 0 | 25 | 0 |
| `-fstack-check` | 678 | 787 | 652 | 25 | 1 |

The 25 library canaries are the zlib/libzip/libc++abi functions that the original
statically linked (`_tr_flush_block`, `inflate*`, `_zip_*`, `zip_*`, `__cxa_*`,
`__dynamic_cast`, `__gxx_personality_v0`, `_ZnwjSt11align_val_t`).  The zero-size
`getAppVersion` alias shares `Engine::InitGL`'s body and is not an independent
canary function.

The current build's canary function set is **set-equal** to the original's:

```text
orig canary 678  ours 678  common 678  orig-only 0  ours-only 0
per-function call-count mismatches: 0
per-function guard-register/save-pattern mismatches: 1
  (_ZN9Generator13createMissionE...: save slot sp#92 vs sp#108; this function is
   one of the known 24 non-byte-exact source-shape functions)
```

For the other 663 canary functions the raw function bytes are identical; the 15
canary-bearing functions whose bytes differ are exactly the known non-byte-exact set:

```text
1008  _ZN10ModStationC1Ev
57232 _ZN11LevelScript7processEi
1948  _ZN11SpaceLounge10drawLoungeEv
9700  _ZN12HangarWindow6renderEv
9600  _ZN15MenuTouchWindowC1Ei
1984  _ZN3Gun22calcCharacterCollisionEv
9332  _ZN3Hud4drawExxP9PlayerEgobjj
2252  _ZN5Level11createSceneEv
17172 _ZN5Level13createMissionEv
8080  _ZN5MGame10OnTouchEndEiiPv
11440 _ZN5MGame8OnUpdateEv
9908  _ZN4Radar4drawEP6PlayerP3Hudi
1996  _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE
968   _ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE
1908  _ZN9RocketGun6updateEi
```

### 2.3 Positive discriminators in the original itself

Both are exported, so their sizes are directly in the original `.dynsym`:

```sh
$RE --dyn-syms -W "$O" | grep -E 'InitializeBlowfish|opensubkeyfile'
# 153: 0006b3b1 416 FUNC GLOBAL DEFAULT 12 InitializeBlowfish
# 160: 0006b2a1  52 FUNC GLOBAL DEFAULT 12 opensubkeyfile
```

Object-level controlled compiles of the same TU
(`src/engine/file/BlowfishSubkeyFile.c`) give:

| function | `-strong` | `-fstack-protector` (basic) | `-all` | `-fno-stack-protector` |
|---|---:|---:|---:|---:|
| `InitializeBlowfish` | **416 B, canary** | 372 B, no canary | 416 B, canary | 372 B, no canary |
| `opensubkeyfile` | **52 B, no canary** | 52 B, no canary | 80 B, canary | 52 B, no canary |

`InitializeBlowfish` therefore **disproves basic**: the original is 416 B and
canary-bearing, basic is 372 B and canary-free.  `opensubkeyfile` **disproves all**:
the original is 52 B and canary-free, all is 80 B and canary-bearing.  Together they
leave strong as the only candidate.

The same result holds for the vendored C libraries:

```text
zlib trees.c / _tr_flush_block:  strong 1 canary, basic 0, none 0
libzip zip_dirent.c:             strong 3 canaries, basic 1, none 0
```

The original's zlib/libzip canary functions (`_tr_flush_block`, `_zip_dirent_read`,
`_zip_dirent_torrent_normalize`, `_zip_dirent_write`, ...) match the strong counts, not
the basic counts.  libcrypto (GCC 4.9) has no canary-eligible function in the linked
subset, so its stack-protector flag is unobservable.

### 2.4 Per-TU distribution

The 192 app TUs map to 652 canary functions.  **156 TUs contain at least one canary
function and every one of them matches the original exactly.**  The 36 TUs with zero
canary functions (where strong/basic/none are indistinguishable on this source) are:

```text
engine/core/AERandom.cpp
engine/core/ArrayInstantiations.cpp
engine/core/IApplicationModule.cpp
engine/input/VirtualInput.cpp
engine/math/AEMath.cpp
engine/math/EaseInOut.cpp
engine/render/AMeshMerger.cpp
engine/render/Curve.cpp
engine/render/FBOContainer.cpp
engine/render/GLES1Compat.cpp
engine/render/Image2D.cpp
engine/render/ImagePart.cpp
engine/render/LensFlare.cpp
engine/render/MarqueeImage.cpp
engine/render/Sprite.cpp
game/core/CheatCode.cpp
game/menu/MTitle.cpp
game/mission/Achievements.cpp
game/mission/Item.cpp
game/mission/Objective.cpp
game/ship/PlayerJumpgate.cpp
game/ship/PlayerStatic.cpp
game/ship/Ship.cpp
game/ui/ListItem.cpp
game/ui/TouchSlider.cpp
game/weapons/SentryGun.cpp
game/weapons/SpriteGun.cpp
game/world/NewsItem.cpp
game/world/SpacePoint.cpp
game/world/Standing.cpp
game/world/SystemPathFinder.cpp
game/world/Wanted.cpp
game/world/Waypoint.cpp
platform/android/BackButton.c
platform/android/GOF2HD2012.c
platform/android/NdkAutosave.cpp
```

For the 156 canary-bearing TUs, basic is disproven (basic protects only 4 app
functions total: `RecordHandler::{addHashToOptions,addHash,checkHash}` and
`FModSound::init`), and all is disproven (all protects every function).  No TU has a
canary set that is smaller or larger than strong's.

## 3. Controlled rebuilds

### 3.1 Harness

`/dev/shm/w5218/toolchain_extra.cmake` includes the real toolchain and removes/adds
flags:

```cmake
include("/home/debian/GalaxyOnFire2/wt/w5218/cmake/toolchain.cmake")
set(GOF2_EXTRA_FLAGS "" CACHE STRING "")
set(GOF2_REMOVE_FLAGS "" CACHE STRING "")
foreach(_var CMAKE_CXX_FLAGS CMAKE_C_FLAGS)
  foreach(_rm ${GOF2_REMOVE_FLAGS})
    string(REPLACE " ${_rm} " " " _tmp "${${_var}}")
    set(${_var} "${_tmp}")
  endforeach()
endforeach()
if(GOF2_EXTRA_FLAGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GOF2_EXTRA_FLAGS}" CACHE STRING "" FORCE)
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${GOF2_EXTRA_FLAGS}" CACHE STRING "" FORCE)
endif()
```

Each variant:

```sh
cmake -S . -B /dev/shm/w5218/build-<name> -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/dev/shm/w5218/toolchain_extra.cmake \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs \
  -DGOF2_MATCH_OPT=-Oz \
  -DGOF2_EXTRA_FLAGS='<flag>' -DGOF2_REMOVE_FLAGS='<flag to remove>'
cmake --build /dev/shm/w5218/build-<name> --target link_so -j6
```

The official verifier was run for the key variants:

```sh
python3 tools/verify/verify.py \
  --build-dir /dev/shm/w5218/verify-<name> \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /dev/shm/w5218/build-<name>/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
```

Raw official results:

```text
basic:      compared 4524  avg 93.5%   byte_exact 1369  linked_exact 3785
all:        compared 4524  avg 45.97%  byte_exact 91    linked_exact 964
none:       compared 4524  avg 93.47%  byte_exact 1369  linked_exact 3780
base:       compared 4524  avg 99.9%   byte_exact 4500  linked_exact 4500
```

`stackcheck` was measured with the exact-byte/size counter (§7.3), which reproduces
the official baseline (4500/4524) and gives `byte 4500, size 4524` for stackcheck.

### 3.2 Section sizes and hashes

```sh
python3 /dev/shm/w5218/section_hashes.py <so>   # readelf -SW + sha256 of each section
```

| build | `.text` | `.ARM.exidx` | `.ARM.extab` | `.rodata` | `.got` | `.data` |
|---|---:|---:|---:|---:|---:|---:|
| original | `0x1337c8` | `0x6f70` | `0x26298` | `0x41b67` | `0x430c` | `0x296c` |
| current strong | `0x1337c8` | `0x6f70` | `0x26298` | `0x41b67` | `0x430c` | `0x296c` |
| basic | `0x12e188` | `0x6f18` | `0x2607c` | `0x41b67` | `0x430c` | `0x296c` |
| all | `0x14f728` | `0x7230` | `0x270bc` | `0x41b67` | `0x430c` | `0x296c` |
| none | `0x12e108` | `0x6f18` | `0x2607c` | `0x41b67` | `0x430c` | `0x296c` |
| stackcheck | `0x1337c8` | `0x6f70` | `0x26298` | `0x41b67` | `0x430c` | `0x296c` |

Full sha256 of `.text` / `.ARM.extab`:

```text
original   .text d49e32c1330e5d94e7628b9a40e05500af963a22816a93f2c738dbd366f813ed
           .extab 1880b0ef21ad149ef0af8eceb25b62a8a14f11568023d0e6e3a32e3fa0fe53e1
base       .text 144a3c3799cddbe2a02e3eef72cf37b213fd24b9a2eccaac866e75640d0a1a2f
           .extab 6b66adedd2edc4901a27ad6e5a5cf84681b2f8f587cda635eb36d98f2ddb0959
stackcheck .text 144a3c3799cddbe2a02e3eef72cf37b213fd24b9a2eccaac866e75640d0a1a2f
           .extab 6b66adedd2edc4901a27ad6e5a5cf84681b2f8f587cda635eb36d98f2ddb0959
```

A full section-by-section diff of base vs stackcheck shows **only**
`.note.gnu.build-id` differs.  `-fstack-check` is therefore byte-neutral on this
source.

### 3.3 Flags clang 7.0.2 rejects on this target

```sh
CC=.cache/ndk/android-ndk-r18b/toolchains/llvm/prebuilt/linux-x86_64/bin/clang
$CC -target armv7-none-linux-androideabi16 -march=armv7-a -mthumb -mfpu=neon \
    -mfloat-abi=softfp -Oz -fstack-protector-strong -x c -c /dev/null -o /tmp/t.o
```

```text
-fstack-clash-protection          -> clang: error: unknown argument
-mstack-protector-guard=tls       -> clang: error: unknown argument
-mstack-protector-guard=global    -> clang: error: unknown argument
-mstack-protector-guard-offset=16 -> clang: error: unknown argument
-mllvm -stack-protector-guard=... -> clang (LLVM option parsing): Unknown command line argument
```

`clang --help` for this 7.0.2 has no `-mstack-protector-guard` option at all.  The
only guard form available is the default **global GOT** form, which is exactly what
the original uses.

## 4. Proven / disproven / unobservable

### Positively proven

* The original app C and C++ TUs use **`-fstack-protector-strong`**.  The full
  canary function set (678 functions, 787 call sites) is reproduced exactly; basic,
  all and none each produce a different set and a much worse gate.
* The guard is the **global GOT** form (`R_ARM_GLOB_DAT`, no TLS relocations), not
  TLS.
* There are **no per-TU exceptions** among the 156 app TUs that have canary-eligible
  functions; every one matches strong exactly.
* zlib and libzip are also built with **`-fstack-protector-strong`** (object-level
  canary counts: zlib 1/0/0, libzip 3/1/0 for strong/basic/none).
* The current build's `-fstack-protector-strong` is correct and already matches the
  original's canary fingerprint.

### Disproven

* `-fstack-protector` (basic): 29 functions / 88 calls vs 678 / 787; official
  `byte_exact` 1369 vs 4500.  `InitializeBlowfish` is 372 B vs the original's 416 B.
* `-fstack-protector-all`: 3960 functions / 4421 calls; official `byte_exact` 91.
  `opensubkeyfile` is 80 B and canary-bearing vs the original's 52 B and canary-free.
* `-fno-stack-protector`: 0 app-TU canaries vs 652; official `byte_exact` 1369.
* `-fstack-clash-protection`: clang 7.0.2 rejects the flag on this target.
* Every `-mstack-protector-guard=...` spelling: clang 7.0.2 rejects it; the binary's
  GOT relocation independently proves the global form.

### Unobservable / unknown

* `-fstack-check`: accepted, but byte-neutral on this source (whole `.so` identical
  except build-id; clang 7 ARM emits no probes even for a 64 KiB test frame).  It
  could have been present or absent; there is no evidence either way.
* Per-TU stack-protector flag on the 36 TUs with zero canary-eligible functions:
  strong/basic/none are indistinguishable there.  No positive evidence for an
  exception, and the simplest uniform hypothesis is strong everywhere.
* libcrypto's stack-protector flag: its linked subset has no canary-eligible function
  (GCC 4.9 build), so the flag is invisible.
* The exact build-system spelling (`-fstack-protector-strong` vs an equivalent
  ndk-build default) is not in the APK; the codegen proves the strong policy, not the
  command-line token.

## 5. Confidence table

| claim | confidence | hard evidence |
|---|---|---|
| Original app C/C++ uses `-fstack-protector-strong` | **very high** | 678/787 exact set match; basic 29/88, all 3960/4421, none 25/84; official byte 4500 vs 1369/91/1369 |
| `-fstack-protector` basic is not the original flag | **very high** | 156/156 canary-bearing TUs mismatch basic; `InitializeBlowfish` 416 B vs 372 B; basic protects only 4 app functions |
| `-fstack-protector-all` is not the original flag | **very high** | all protects 3960 functions vs 678; `opensubkeyfile` 80 B vs 52 B; official byte 91 |
| `-fno-stack-protector` is not the original flag | **very high** | 0 app canaries vs 652; official byte 1369 |
| Guard is global GOT, not TLS | **very high** | `R_ARM_GLOB_DAT __stack_chk_guard`; no `R_ARM_TLS_*`; code loads GOT slot then dereferences |
| No per-TU exceptions among canary-bearing TUs | **very high** | 156 TUs with canaries, all set-equal; 0 TU mismatches |
| zlib/libzip use strong | **high** | object-level canary counts strong/basic/none = 1/0/0 and 3/1/0; original library canary set matches strong |
| `-fstack-check` was/was not used | **unobservable** | full build identical except build-id; no probes even for 64 KiB test |
| `-fstack-clash-protection` / `-mstack-protector-guard` | **high (absent)** | clang 7.0.2 driver rejects every spelling on this target |
| libcrypto stack-protector flag | **unobservable** | no canary-eligible function in the linked subset |

## 6. Strict improvement

**None.**  The current build already uses `-fstack-protector-strong`, and the full
gate is already 4500/4500 byte/linked with size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0.  Every alternative is worse; `-fstack-check` is neutral (only
build-id changes) and has no positive binary evidence, so adding it would not be a
strict improvement and would not be justified as a matching change.  No `src/`,
`cmake/`, `tools/`, baseline or allowlist change was made; this commit is the note
only.

## 7. Reproduction harness

### 7.1 Canary scanner

```python
#!/usr/bin/env python3
# canary_scan.py <so> <text_start_hex> [--json out]
import subprocess, re, sys, json, collections
OD = '/home/debian/GalaxyOnFire2/wt/w5218/.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump'

def norm(n):
    return n.split('@@', 1)[0].split('@', 1)[0]

def scan(path, text_start=0):
    out = subprocess.check_output([OD, '-d', path], text=True, errors='replace')
    funcs, cur = [], None
    for ln in out.splitlines():
        m = re.match(r'^([0-9a-f]+) <([^>]+)>:$', ln)
        if m:
            cur = {'addr': int(m.group(1), 16), 'label': m.group(2), 'ins': []}
            funcs.append(cur); continue
        m = re.match(r'^\s*([0-9a-f]+):\s+(?:[0-9a-f]{4}\s+){1,2}\t(.+)$', ln)
        if m and cur is not None:
            cur['ins'].append((int(m.group(1), 16), m.group(2).split(';', 1)[0].strip()))
    funcs = [f for f in funcs if f['addr'] >= text_start and not f['label'].endswith('@plt')]
    for f in funcs:
        f['name'] = norm(f['label'])
        f['canary_calls'] = sum(1 for _, t in f['ins'] if '__stack_chk_fail@plt' in t)
        f['canary'] = f['canary_calls'] > 0
        # guard chain: ldr rX,[pc]; add rX,pc; ldr rY,[rX]; ldr rZ,[rY]; str rZ,[sp]
        pc = got = gp = can = save = None
        for _, t in f['ins'][:60]:
            m = re.match(r'ldr(?:\.w)?\s+(\w+),\s*\[pc(?:,\s*#-?\d+)?\]$', t)
            if m and pc is None: pc = m.group(1); continue
            m = re.match(r'add(?:\.w)?\s+(\w+),\s*pc$', t)
            if m and pc is not None and m.group(1) == pc and got is None: got = pc; continue
            m = re.match(r'ldr(?:\.w)?\s+(\w+),\s*\[(\w+)(?:,\s*#0)?\]$', t)
            if m:
                d, b = m.group(1), m.group(2)
                if got is not None and b == got and gp is None: gp = d; continue
                if gp is not None and b == gp and can is None: can = d; continue
            if can is not None and save is None:
                m = re.match(rf'str(?:\.w)?\s+{can},\s*\[(?:sp|r7)', t)
                if m: save = t
        f.update(guard_ptr_reg=gp, canary_reg=can, canary_save=save)
        f['push'] = next((t for _, t in f['ins'][:5] if re.match(r'push\b', t)), None)
    return funcs

if __name__ == '__main__':
    path = sys.argv[1]
    start = int(sys.argv[2], 16) if len(sys.argv) > 2 and sys.argv[2] != '--json' else 0
    fs = scan(path, start)
    can = [f for f in fs if f['canary']]
    print('labels', len(fs), 'canary_functions', len(can),
          'canary_call_sites', sum(f['canary_calls'] for f in fs))
    print('guard_ptr_reg', collections.Counter(f['guard_ptr_reg'] for f in can))
    print('canary_reg', collections.Counter(f['canary_reg'] for f in can))
    print('canary_save', collections.Counter(f['canary_save'] for f in can))
    if '--json' in sys.argv:
        json.dump([{k: v for k, v in f.items() if k != 'ins'} for f in fs],
                  open(sys.argv[sys.argv.index('--json') + 1], 'w'), indent=1)
```

Run with `0x69440` (the original `.text` start) to skip the PLT stubs.

### 7.2 Object-level compile

```python
#!/usr/bin/env python3
# compile_tu.py <src> <out.o> <extra flags> <flags to remove>
import json, shlex, subprocess, sys, os
src, out, extra, remove = sys.argv[1:5]
cc = json.load(open('/home/debian/GalaxyOnFire2/wt/w5218/cmake-build-match/compile_commands.json'))
ent = next(x for x in cc if os.path.realpath(x['file']) == os.path.realpath(src))
args = shlex.split(ent['command'])
rm = set(remove.split()) if remove else set()
args = [a for a in args if a not in rm]
i = args.index('-o'); args[i + 1] = out
args += (extra.split() if extra else [])
os.makedirs(os.path.dirname(out), exist_ok=True)
subprocess.run(args, check=True)
```

Example:

```sh
python3 compile_tu.py src/engine/file/BlowfishSubkeyFile.c /dev/shm/w5218/obj/Blowfish.basic.o \
  '-fstack-protector' '-fstack-protector-strong'
```

### 7.3 Exact-byte/size counter

```python
#!/usr/bin/env python3
# fast_verify.py <so>...
import sys
sys.path.insert(0, '/home/debian/GalaxyOnFire2/wt/w5218/tools/verify')
from elf import Elf32
orig = Elf32('/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so')
o = orig.functions()
for path in sys.argv[1:]:
    e = Elf32(path); f = e.functions(); common = set(o) & set(f)
    byte = size = 0
    for n in common:
        a, b = o[n], f[n]
        if a['size'] == b['size']:
            size += 1
        if a['size'] == b['size'] and orig.bytes_at(a['value'], a['size']) == e.bytes_at(b['value'], b['size']):
            byte += 1
    print(f'{path}: compared {len(common)} byte {byte} size {size} '
          f'missing {len(set(o) - set(f))} extra {len(set(f) - set(o))}')
```

### 7.4 Section hashes

```python
#!/usr/bin/env python3
# section_hashes.py <so>
import subprocess, re, hashlib, sys
RE = '/home/debian/GalaxyOnFire2/wt/w5218/.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-readelf'
p = sys.argv[1]; data = open(p, 'rb').read()
for ln in subprocess.check_output([RE, '-SW', p], text=True).splitlines():
    m = re.match(r'\s*\[\s*(\d+)\]\s+(\S+)\s+(\S+)\s+([0-9a-f]+)\s+([0-9a-f]+)\s+([0-9a-f]+)', ln)
    if m:
        n, t, off, size = m.group(2), m.group(3), int(m.group(5), 16), int(m.group(6), 16)
        h = 'NOBITS' if t == 'NOBITS' else hashlib.sha256(data[off:off + size]).hexdigest()
        print(f'{n:20s} size {size:8d} {h}')
```

## 8. Files touched

* `_work/reconstructions/stack_protector_fingerprint_w5218.md` (this note).

No `src/`, `cmake/`, `tools/`, baseline or allowlist change.  `git status` is clean
after the commit.
