# Per-TU compiler-flag inference from codegen fingerprints (w3415)

Status: research note (2026-11-01). Worktree `wt/w3415`, branch `wave87-w3415`,
base `a782e382`. No `src/` or compile-flag change landed: every tested per-TU
flag was neutral or worse, so this commit contains only this note.

## TL;DR

The original's exported app code is consistent with **one uniform compiler-flag
set across all 195 app TUs**. No per-TU flag is supported by codegen
fingerprints or by controlled per-TU A/B:

* 178/195 app TUs are fully `linked_exact`; 99/195 are fully `byte_exact`
  under the single global flag set.
* The 17 TUs with non-linked functions are exactly the known source-shape
  queue (Layout, LevelScript, Radar, Generator, MenuTouchWindow, ...).
* Per-TU A/B on those TUs: 12 neutral, 8 worse, 1 link failure; **no strict
  improvement**.
* Fingerprints that a per-TU flag would change (stack canary, frame pointer,
  NEON, jump tables, movw, strd, sized new/delete, division helpers, guard
  variables) are identical function-for-function in every app TU.

Conclusion: the remaining non-linked functions are source/TU shape, not a
missing per-TU compiler flag. Keep the global flag set.

## 1. Baseline

Commands:

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
```

Measured at `a782e382` before and after the experiments:

```
verify: byte 4115  linked 4495  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
size-exact 4524 / 4524
DRIFT: 0 field(s) across 0 class(es)
lint CLEAN
```

`size-exact` is `sum(osize == usize)` over `report.json`; `check` does not
print it. All experiments below were compared against this exact baseline.

## 2. Per-TU function map

The repo has 204 app objects; 195 of them define at least one exported function
in the comparison set. Mapping was done with the NDK `nm` on the built objects:

```sh
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -A -S --defined-only -g cmake-build-match/CMakeFiles/gof2.dir/src/game/world/Level.cpp.obj
```

Each `report.json` row name was looked up in those symbol tables. 4,281 exported
functions map to app TUs; the remaining 243 are archive/runtime functions
(libcrypto, libzip, zlib, libc++abi, libgcc, ...).

Distribution:

| item | count |
|---|---:|
| app TUs with exported functions | 195 |
| app-TU functions in `report.json` | 4,281 |
| app-TU `byte_exact` | 4,058 |
| app-TU `linked_exact` | 4,252 |
| TUs fully `linked_exact` | 178 / 195 |
| TUs fully `byte_exact` | 99 / 195 |

The 17 TUs with at least one non-linked function are:

| TU | n | byte | linked |
|---|---:|---:|---:|
| `game/ui/MenuTouchWindow.cpp` | 42 | 37 | 37 |
| `game/world/Level.cpp` | 107 | 95 | 104 |
| `game/core/Globals.cpp` | 41 | 35 | 39 |
| `game/menu/MGame.cpp` | 45 | 38 | 43 |
| `game/menu/ModStation.cpp` | 39 | 31 | 37 |
| `game/ui/Hud.cpp` | 54 | 50 | 52 |
| `game/ui/Layout.cpp` | 50 | 48 | 48 |
| `game/world/StarMap.cpp` | 33 | 28 | 31 |
| `game/menu/SpaceLounge.cpp` | 32 | 26 | 31 |
| `game/mission/Generator.cpp` | 15 | 8 | 14 |
| `game/mission/Status.cpp` | 131 | 127 | 130 |
| `game/ship/PlayerEgo.cpp` | 174 | 171 | 173 |
| `game/ui/HangarWindow.cpp` | 32 | 30 | 31 |
| `game/weapons/Gun.cpp` | 28 | 25 | 27 |
| `game/weapons/Radar.cpp` | 19 | 18 | 18 |
| `game/weapons/RocketGun.cpp` | 13 | 12 | 12 |
| `game/world/LevelScript.cpp` | 17 | 16 | 16 |

These are the same functions listed in `report.json`'s worst-first table and in
the existing reconstruction notes; none is a whole-TU codegen shift.

## 3. Codegen fingerprints

A per-TU flag would normally leave a visible fingerprint across that TU. The
following scan disassembles both binaries once with the NDK objdump, attributes
each instruction to its function, and compares the set of app-TU functions that
carry each fingerprint. Counts are exported app-TU functions only; the decisive
column is **TUs with mismatch**.

| fingerprint | orig | ours | both | orig-only | ours-only | TUs with mismatch |
|---|---:|---:|---:|---:|---:|---:|
| stack canary (`__stack_chk_*`) | 652 | 652 | 652 | 0 | 0 | **0** |
| guard call (`__cxa_guard_*`) | 1 | 1 | 1 | 0 | 0 | **0** |
| `__cxa_atexit` | 1 | 1 | 1 | 0 | 0 | **0** |
| frame pointer (prologue `push {...,r7}` + `mov/add r7,sp`) | 2288 | 2288 | 2288 | 0 | 0 | **0** |
| NEON instruction | 640 | 640 | 640 | 0 | 0 | **0** |
| `tbb`/`tbh` jump table | 84 | 84 | 84 | 0 | 0 | **0** |
| `movw` | 407 | 407 | 407 | 0 | 0 | **0** |
| `movt` | 3 | 3 | 2 | 1 | 1 | 2 |
| `ldrd` | 482 | 481 | 480 | 2 | 1 | 3 |
| `strd` | 775 | 775 | 775 | 0 | 0 | **0** |
| sized new/delete (`_Znwj`/`_Znaj`/`_ZdlPv*`) | 573 | 573 | 573 | 0 | 0 | **0** |
| division helper (`__aeabi_*div*`) | 54 | 54 | 54 | 0 | 0 | **0** |
| EH throw/catch (`__cxa_throw`/`catch`) | 1 | 2 | 1 | 0 | 1 | 1 |

The `movt`, `ldrd` and EH-throw deltas are the known source-shape deltas, not
flag shifts: the EH extra is `Level::createShip` (already documented as a
landing-pad split), and the `movt`/`ldrd` differences are isolated instruction
selection in functions whose bodies already differ. Every fingerprint that a
per-TU `-fno-stack-protector`, `-fomit-frame-pointer`, `-mfpu=vfpv3-d16`,
`-fno-vectorize`, `-fno-jump-tables`, `-mno-movt`, `-fno-sized-deallocation`,
`-fno-threadsafe-statics`, or `-march` change would move is exact across all
195 app TUs.

ISA mode is also uniform: all 195 app TUs contain only Thumb functions in both
binaries. The only 23 ARM functions are the libcrypto SHA/atomic/probe block
(`SHA224_*`, `SHA256_*`, `sha256_block_data_order*`, `OPENSSL_*`,
`CRYPTO_memcmp`, `_armv7_*`), identical in both.

Reproduction script (self-contained; run from repo root after a baseline build):

```python
import subprocess, re, collections
OD = '.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump'
NM = '.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm'
ORIG = '../_work/bins/android_2.0.16_libgof2hdaa.so'
OURS = 'cmake-build-match/libgof2hdaa.so'

patterns = {
 'stack-canary': re.compile(r'__stack_chk_(?:fail|guard)'),
 'guard-call': re.compile(r'__cxa_guard_(?:acquire|release|abort)'),
 'cxa-atexit': re.compile(r'__cxa_atexit|\batexit\b'),
 'frame-pointer': None,
 'neon': re.compile(r'^v(?:ld|st|add|sub|mul|mov|dup|zip|uzp|trn|ext|rev|padd|pmin|pmax|and|orr|eor|bic|mvn|abs|neg|clz|cnt|shl|shr|sli|sri|qadd|qsub|hadd|hsub|rhadd|mull|mla|mls|tst|ceq|cge|cgt|cle|clt|max|min|recpe|rsqrte|sqrt|div|fma|fms|fadd|fsub|fmul|fdiv|fcmp|fabs|fneg|fmsr|fmrs|fmdhr|fmdlr|fmdrr|fmsrr|fmstat|push|pop)\b'),
 'jump-table': re.compile(r'^tb[bh]\b'),
 'movw': re.compile(r'^movw\b'), 'movt': re.compile(r'^movt\b'),
 'ldrd': re.compile(r'^ldrd\b'), 'strd': re.compile(r'^strd\b'),
 'sized-new-delete': re.compile(r'_ZdlPvj|_ZdlPv|_Znwj|_Znaj'),
 'div-helper': re.compile(r'__aeabi_(?:u|s)?ldiv|__aeabi_[ui]div'),
 'eh-throw': re.compile(r'__cxa_(?:throw|begin_catch|end_catch|allocate_exception)'),
}

def norm(n):
    return n.split('@@', 1)[0].split('@', 1)[0]

def scan(path):
    out = subprocess.check_output([OD, '-d', '-M', 'force-thumb', path], text=True, errors='replace')
    cur, ins, blocks = None, [], {}
    def finish():
        if cur is not None: blocks[cur] = ins[:]
    for ln in out.splitlines():
        m = re.match(r'^([0-9a-f]+) <([^>]+)>:$', ln)
        if m:
            finish(); cur = norm(m.group(2)); ins = []; continue
        m = re.match(r'^\s*[0-9a-f]+:\s+(?:[0-9a-f]{4}\s+){1,2}\t(.+)$', ln)
        if m and cur: ins.append(m.group(1))
    finish()
    sets = {}
    for name, pat in patterns.items():
        s = set()
        for fn, body in blocks.items():
            if name == 'frame-pointer':
                pro = ' '.join(body[:12])
                hit = bool(re.search(r'push\b[^\n]*\br7\b', pro) and
                           re.search(r'\b(?:mov|add)\s+r7,\s*sp\b', pro))
            else:
                hit = any(pat.search(x) for x in body)
            if hit: s.add(fn)
        sets[name] = s
    return sets

o, u = scan(ORIG), scan(OURS)
objs = subprocess.check_output(['find', 'cmake-build-match/CMakeFiles/gof2.dir/src', '-name', '*.obj']).decode().split()
sym2tu = {}
for obj in objs:
    out = subprocess.check_output([NM, '-S', '--defined-only', '-g', obj], text=True, errors='replace')
    for ln in out.splitlines():
        p = ln.split()
        if len(p) >= 4 and p[2] in 'TtWwVv':
            sym2tu.setdefault(p[3], obj.split('/src/', 1)[1].rsplit('.obj', 1)[0])
for name in patterns:
    a = {n for n in o[name] if n in sym2tu}
    b = {n for n in u[name] if n in sym2tu}
    by = collections.defaultdict(lambda: [0, 0])
    for n in a | b:
        tu = sym2tu[n]; by[tu][0] += n in a; by[tu][1] += n in b
    bad = sum(1 for v in by.values() if v[0] != v[1])
    print(f'{name:20s} {len(a):5d} {len(b):5d} {len(a & b):5d} '
          f'{len(a - b):6d} {len(b - a):6d} {bad:11d}')
```

## 4. Per-TU flag A/B

Harness: a temporary cache-variable block was inserted immediately after
`add_library(gof2 OBJECT ${_srcs})` in `CMakeLists.txt`:

```cmake
set(GOF2_AB_TU "" CACHE STRING "TU (path under src/) for per-TU flag A/B")
set(GOF2_AB_FLAGS "" CACHE STRING "compile flags for GOF2_AB_TU")
if(GOF2_AB_TU)
    set_source_files_properties("${CMAKE_SOURCE_DIR}/src/${GOF2_AB_TU}"
        PROPERTIES COMPILE_OPTIONS "${GOF2_AB_FLAGS}")
endif()
```

Each variant was built and verified with:

```sh
cmake -S . -B cmake-build-match \
  -DGOF2_AB_TU='<tu>' -DGOF2_AB_FLAGS='<flags>'
cmake --build cmake-build-match --target link_so -j8
cmake --build cmake-build-match --target verify -j8
```

`GOF2_AB_FLAGS` is a CMake list; multi-token LLVM options were passed as
`-mllvm;-sink-insts-to-avoid-spills`. The baseline row is the same build with
`GOF2_AB_TU=''`.

| # | TU | per-TU flags | byte | linked | size | fuzzy | avg | verdict |
|---:|---|---|---:|---:|---:|---:|---:|---|
| 0 | *(baseline)* | — | **4115** | **4495** | **4524/4524** | **4496** | **99.89** | — |
| 1 | `game/ui/Layout.cpp` | `-O2` | 1348 | 4467 | 4498/4524 | 4468 | 99.68 | worse |
| 2 | `game/ui/Layout.cpp` | `-fno-inline-functions-called-once` | 4115 | 4495 | 4524/4524 | 4496 | 99.89 | neutral |
| 3 | `game/ui/Layout.cpp` | `-mllvm -disable-postra-machine-sink` | 4115 | 4495 | 4524/4524 | 4496 | 99.89 | neutral |
| 4 | `game/ui/Layout.cpp` | `-mllvm -pre-RA-sched=source` | 3040 | 4461 | 4510/4524 | 4462 | 99.64 | worse |
| 5 | `game/world/LevelScript.cpp` | `-mllvm -condsider-local-interval-cost` | 3462 | 4495 | 4523/4524 | 4496 | 99.89 | worse |
| 6 | `game/world/LevelScript.cpp` | `-fno-inline-functions-called-once` | 4115 | 4495 | 4524/4524 | 4496 | 99.89 | neutral |
| 7 | `game/weapons/Radar.cpp` | `-mllvm -likely-branch-weight=80 -mllvm -unlikely-branch-weight=1` | 4115 | 4495 | 4524/4524 | 4496 | 99.89 | neutral |
| 8 | `game/mission/Generator.cpp` | `-fno-inline-functions-called-once` | 4115 | 4495 | 4524/4524 | 4496 | 99.89 | neutral |
| 9 | `game/mission/Generator.cpp` | `-fno-delete-null-pointer-checks` | 4115 | 4495 | 4524/4524 | 4496 | 99.89 | neutral |
| 10 | `game/ui/MenuTouchWindow.cpp` | `-fno-inline-functions-called-once` | 4115 | 4495 | 4524/4524 | 4496 | 99.89 | neutral |
| 11 | `game/menu/MGame.cpp` | `-fno-inline-functions-called-once` | 4115 | 4495 | 4524/4524 | 4496 | 99.89 | neutral |
| 12 | `game/core/Globals.cpp` | `-fno-inline-functions-called-once` | 4115 | 4495 | 4524/4524 | 4496 | 99.89 | neutral |
| 13 | `game/core/Globals.cpp` | `-fno-delete-null-pointer-checks` | 4115 | 4495 | 4524/4524 | 4496 | 99.89 | neutral |
| 14 | `game/world/Level.cpp` | `-mllvm -disable-postra-machine-sink` | 4115 | 4494 | 4524/4524 | 4495 | 99.89 | worse (linked -1) |
| 15 | `game/world/Level.cpp` | `-mllvm -sink-insts-to-avoid-spills` | 2313 | 4481 | 4511/4524 | 4482 | 99.80 | worse |
| 16 | `game/weapons/RocketGun.cpp` | `-mllvm -stress-regalloc=12` | 2874 | 4493 | 4523/4524 | 4494 | 99.87 | worse |
| 17 | `engine/render/ParticleSettings.cpp` | `-fno-threadsafe-statics` | 4115 | 4495 | 4524/4524 | 4496 | 99.89 | neutral |
| 18 | `game/world/Level.cpp` | `-fno-stack-protector` | 2312 | 4477 | 4504/4524 | 4478 | 99.78 | worse |
| 19 | `game/ship/Player.cpp` | `-fomit-frame-pointer` | 2222 | 4452 | 4483/4524 | 4453 | 99.59 | worse |
| 20 | `engine/render/PaintCanvas.cpp` | `-fno-inline-functions-called-once` | 4115 | 4495 | 4524/4524 | 4496 | 99.89 | neutral |
| 21 | `engine/render/PaintCanvas.cpp` | `-O2` | — | — | — | — | — | **link fail** (`load segment overlap [0x0 -> 0x20cc97] and [0x20cc40 -> 0x21d31c]`) |

The `-O2` link failure is consistent with the fixed `-Tdata 0x20cb80` anchor in
`CMakeLists.txt`: a per-TU size increase pushes the RW segment into the fixed
`.data` address. It is not a codegen result; `Layout -O2` did link and was much
worse.

No row is a strict improvement. The neutral rows are useful negatives: the
flag is accepted and changes the object's compile command, but does not move
the gate. (Whether it changes individual TU functions is not claimed; only the
gate counts were measured.)

## 5. What is proven vs still hypothesis

**Proven by measurement**

* Baseline is byte 4115 / linked 4495 / size 4524/4524 / lint CLEAN / drift 0.
* 178/195 app TUs are fully linked-exact and 99/195 fully byte-exact under the
  single global flag set.
* All 195 app TUs are Thumb-only in both binaries; the 23 ARM functions are the
  libcrypto block.
* The stack-canary, frame-pointer, NEON, jump-table, `movw`, `strd`,
  sized-new/delete, and division-helper function sets are identical in every
  app TU (0 TU mismatches). The few `movt`/`ldrd`/EH-throw deltas are known
  source-shape deltas, not TU-wide flag shifts.
* Every tested per-TU flag is neutral or worse; no strict improvement exists
  among the candidates.

**Hypothesis / limits**

* A per-TU flag that is codegen-neutral for the tested TUs (for example
  `-fno-inline-functions-called-once` on a TU with no called-once functions)
  cannot be excluded by A/B. There is simply no positive fingerprint or gate
  gain for it.
* Fingerprint equality is strong evidence but not a formal proof of the
  original command line; the original build script is not in the APK.
* The 17 non-linked TUs are source-shape work. The per-TU flag lever is
  exhausted for the candidates tested; the next useful work is the existing
  per-function reconstruction queue (`Layout::Layout`, `LevelScript::process`,
  `Radar::draw`, `Generator::createMission`, ...).

## 6. Restore procedure used

After the last variant, `CMakeLists.txt` was restored with
`git checkout -- CMakeLists.txt`. Removing the harness does not invalidate the
last TU's object, so the last variant's TU was touched and rebuilt:

```sh
touch src/game/world/Level.cpp
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target verify -j8   # sync report.json
cmake --build cmake-build-match --target drift -j8
```

Final state: byte 4115 / linked 4495 / size 4524/4524 / lint CLEAN / drift 0;
worktree clean after this note is committed.
