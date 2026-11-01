# Alternate NDK/clang versions vs the 28 mismatched functions (w3563)

Status: research note (2026-11-01). Worktree `wt/w3563`, branch `wave87-w3563`, based on
master `113737ee`. Every number below was measured against
`../_work/bins/android_2.0.16_libgof2hdaa.so` by rebuilding this tree in a scratch build dir
(`/tmp/build-*`) with only the clang binary changed.

## TL;DR

**The 28 mismatched functions are not a compiler-version artifact.**

* The original `.comment` is byte-identical to NDK r18b's clang 7.0.2 (same Android build
  `4751641`, same LLVM git hash `1d739ffb...`). NDK r18 and r18b ship a **byte-identical
  clang binary** (sha256 `af826867...`) and identical libc++/libc++abi/support headers.
* Rebuilding the whole tree with clang 6.0.2 (r17c), 8.0.2 (r19), 8.0.7 (r20), 9.0.8 (r21)
  and 14.0.7 (r25c) makes the overall match **much worse** (byte_exact 1270 / 1391 / 1345 /
  1087 / 1041 vs 4496) and **fixes none of the 28**: all 28 remain non-byte under every
  compiler, with 0/28 becoming byte-exact.
* The residual is source shape / TU membership, not the toolchain. Do not chase alternate
  NDK versions for these functions.

## 1. Baseline (r18b, unchanged tree)

```
$ cmake --preset default
$ cmake --build cmake-build-match --target check -j8
...
verify: byte 4496  linked 4496  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`.text` is `0x1337c8` at `0x69440` (the original's exact size and base); `lint` prints
`CLEAN ✓`; `drift` prints `=== DRIFT: 0 field(s) across 0 class(es) ===`.

The 28 non-byte functions are exactly the 28 rows with `"byte": false` in `report.json`
(4524 compared, 4496 byte-exact, all 28 size-exact; C1/C2 pairs grouped):

```
Layout::Layout (C1/C2)                         46.6%
LevelScript::process(int)                      52.8%
Radar::draw(Player*, Hud*, int)                71.6%
Level::createShip(...)                         73.6%
Generator::createMission(...)                  73.6%
MGame::OnTouchEnd(int,int,void*)               78.4%
HangarWindow::render()                         78.7%
Globals::Globals (C1/C2)                       80.5%
Gun::calcCharacterCollision()                  82.7%
Level::createMission()                         83.8%
Status::missionCompleted(bool,bool,long long)  86.7%
RocketGun::update(int)                         86.9%
PlayerEgo::calcCollision(...)                  87.3%
Level::createScene()                           88.2%
Hud::draw(...)                                 88.7%
Hud::drawChallengeModeScore(int)               89.1%
MenuTouchWindow::OnTouchEnd(...)               89.5%
MenuTouchWindow::MenuTouchWindow(int) (C1/C2)  91.2%
StarMap::draw()                                94.8%
SpaceLounge::drawLounge()                      95.5%
MenuTouchWindow::OnTouchBegin(...)             97.2%
ModStation::ModStation (C1/C2)                 98.6%
MGame::OnUpdate()                              99.9%
StarMap::drawOnScreenInfo(int,bool)            99.9%
```

They live in 17 game TUs: `Layout.cpp`, `LevelScript.cpp`, `Radar.cpp`, `Level.cpp`,
`Generator.cpp`, `MGame.cpp`, `HangarWindow.cpp`, `Globals.cpp`, `Gun.cpp`, `Status.cpp`,
`RocketGun.cpp`, `PlayerEgo.cpp`, `Hud.cpp`, `MenuTouchWindow.cpp`, `StarMap.cpp`,
`SpaceLounge.cpp`, `ModStation.cpp`.

## 2. Compiler identity (hard evidence)

`readelf -p .comment` on the original and on a trivial object compiled by each NDK's clang:

| NDK | clang `.comment` |
|---|---|
| **r18b (baseline)** | `Android (4751641 based on r328903) clang version 7.0.2 (.../clang 003100370607242ddd5815e4a043907ea9004281) (.../llvm 1d739ffb0366421d383e04ff80ec2ee591315116) (based on LLVM 7.0.2svn)` |
| r18 | **same clang binary as r18b** (sha256 `af8268675ca343698d3fae4bca21b0563762442204cbbc34555c8ddcf362376a`); libc++/libc++abi/support headers `diff -rq` identical |
| r17c | `Android (4691093 based on r316199) clang version 6.0.2 ... (based on LLVM 6.0.2svn)` |
| r19 | `Android (5058415 based on r339409) clang version 8.0.2 ... (based on LLVM 8.0.2svn)` |
| r20 | `Android (5220042 based on r346389c) clang version 8.0.7 ... (based on LLVM 8.0.7svn)` |
| r21 | `Android (5900059 based on r365631c) clang version 9.0.8 ... (based on LLVM 9.0.8svn)` |
| r25c | `Android (9352603, based on r450784d1) clang version 14.0.7 ...` |

The original's `.comment` is exactly the r18b line. The original's `.note.android.ident` is
`r18b` (build `5063045`), so r18-vs-r18b is a no-op: same compiler binary, same headers.

Commands:

```
readelf -p .comment ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -n ../_work/bins/android_2.0.16_libgof2hdaa.so
sha256sum .cache/ndk/android-ndk-r18b/toolchains/llvm/prebuilt/linux-x86_64/bin/clang \
          /tmp/ndks/android-ndk-r18/toolchains/llvm/prebuilt/linux-x86_64/bin/clang
diff -rq .cache/ndk/android-ndk-r18b/sources/cxx-stl/llvm-libc++/include \
         /tmp/ndks/android-ndk-r18/sources/cxx-stl/llvm-libc++/include
```

## 3. Controlled A/B method

Each alternate is a full rebuild in a fresh scratch dir. The toolchain file includes the repo
toolchain (so **all flags, the r18b libc++ headers, the r18b sysroot, the r18b static libs and
the r18b gold linker are unchanged**) and then overrides only the compiler binary:

```cmake
# /tmp/toolchain-rXX.cmake
include("/home/debian/GalaxyOnFire2/wt/w3563/cmake/toolchain.cmake")
set(CMAKE_CXX_COMPILER "/tmp/ndks/android-ndk-rXX/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++" CACHE FILEPATH "" FORCE)
set(CMAKE_C_COMPILER   "/tmp/ndks/android-ndk-rXX/toolchains/llvm/prebuilt/linux-x86_64/bin/clang"   CACHE FILEPATH "" FORCE)
set(CMAKE_CXX_COMPILER_ID Clang)
set(CMAKE_C_COMPILER_ID Clang)
```

Build:

```
cmake -S . -B /tmp/build-rXX -G Ninja \
      -DCMAKE_TOOLCHAIN_FILE=/tmp/toolchain-rXX.cmake -DGOF2_MATCH_OPT=-Oz
cmake --build /tmp/build-rXX --target link_so -j8
```

Two version-specific details:

* r17c's clang needs `libtinfo.so.5`; it was run with
  `LD_LIBRARY_PATH=/tmp/libcompat`, where `libtinfo.so.5 -> /usr/lib/x86_64-linux-gnu/libtinfo.so.6`.
* r25c (clang 14) defaults to `-fno-common`; r18b defaults to `-fcommon`. To keep the C TU
  semantics comparable it was configured with `-DGOF2_MATCH_OPT='-Oz -fcommon'`. Without it
  the link fails on duplicate tentative definitions (`g_android_back_button_pressed`).

## 4. Gate counts

Full `verify.py` was run for r19/r20. For r17c/r21/r25c the byte/linked counts were produced
by a fast counter that reuses `verify.py`'s `Elf32`, `Resolver`, `disassemble` and
`reconcile` but skips the difflib ratio (the ratio is only needed for the fuzzy `pct`; it is
what makes a full run over ~3000 broken functions take minutes). The per-function `pct` for
the 28 was produced by the same `verify.py` primitives plus `verify.score`.

| build | clang | `.text` addr / size | compared | byte_exact | linked_exact | missing | 28 byte-exact |
|---|---|---:|---:|---:|---:|---:|---:|
| **r18b (baseline)** | 7.0.2 | `0x69440 / 0x1337c8` | 4524 | **4496** | **4496** | 0 | 0/28 |
| r17c | 6.0.2 | `0x69420 / 0x145078` | 4523 | 1270 | 2549 | 0 | 0/28 |
| r19 | 8.0.2 | `0x69440 / 0x133748` | 4524 | 1391 | 4065 | 0 | 0/28 |
| r20 | 8.0.7 | `0x69420 / 0x132348` | 4523 | 1345 | 3710 | 1 | 0/28 |
| r21 | 9.0.8 | `0x69420 / 0x12fb28` | 4523 | 1087 | 2135 | 1 | 0/28 |
| r25c | 14.0.7 | `0x694e0 / 0x12ff38` | 4524 | 1041 | 1992 | 0 | 0/28 |

Raw summary lines:

```
r19  (full verify): compared 4524  avg 98.55%  byte_exact 1391  linked_exact 4065  fuzzy_perfect 4066
                    stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
r20  (full verify): compared 4523  avg 97.39%  byte_exact 1345  linked_exact 3710  fuzzy_perfect 3710
                    stub_zero_size 0  orig_zero_size 0  missing 1 (wrong_type 0  absent 0)  extra 0
r17c (fast counter): compared 4523  byte_exact 1270  linked_exact 2549  orig_zero_size 1
r21  (fast counter): compared 4523  byte_exact 1087  linked_exact 2135  missing 1
r25c (fast counter): compared 4524  byte_exact 1041  linked_exact 1992
```

`missing 1` for r20/r21 is `_ZN5ArrayIPN11AbyssEngine7KeyCodeEED2Ev`
(`Array<AbyssEngine::KeyCode*>::~Array()`), which newer clang no longer emits; it is not one
of the 28. r17c has `orig_zero_size 1` (a size-0 original symbol), also unrelated.

The `.text` size is a useful sanity check: r19's `.text` is only 0x80 smaller than the
original's, yet 3105 functions stop being byte-exact. A compiler-version change is therefore
a broad codegen change, not a small local fix.

## 5. Per-function result for the 28

`pct` is `verify.py`'s normalized-instruction ratio (100.0 would mean linked-exact; byte-exact
is stronger). None of the alternates reaches 100.0 on any of the 28.

| function | base (r18b) | r17c (6.0.2) | r19 (8.0.2) | r20 (8.0.7) | r21 (9.0.8) | r25c (14.0.7) |
|---|---:|---:|---:|---:|---:|---:|
| `Layout::Layout()` (C1) | 46.6 | 22.2 | 32.1 | 32.0 | 17.7 | 9.2 |
| `Layout::Layout()` (C2) | 46.6 | 22.2 | 32.1 | 32.0 | 17.7 | 9.2 |
| `LevelScript::process(int)` | 52.8 | 38.8 | 50.7 | 44.7 | 25.9 | 22.8 |
| `Radar::draw(Player*, Hud*, int)` | 71.6 | 49.2 | 39.9 | 39.9 | 37.7 | 33.7 |
| `Level::createShip(int,int,int,Waypoint*,bool,bool)` | 73.6 | 57.9 | 73.6 | 72.6 | 27.3 | 26.5 |
| `Generator::createMission(Agent*, Array<SolarSystem*>*)` | 73.6 | 50.1 | 45.2 | 41.2 | 38.1 | 31.0 |
| `MGame::OnTouchEnd(int,int,void*)` | 78.4 | 38.6 | 58.1 | 57.8 | 45.8 | 28.1 |
| `HangarWindow::render()` | 78.7 | 36.4 | 53.1 | 54.8 | 34.2 | 29.2 |
| `Globals::Globals()` (C1) | 80.5 | 41.2 | 80.5 | 78.8 | 19.2 | 16.1 |
| `Globals::Globals()` (C2) | 80.5 | 41.2 | 80.5 | 78.8 | 19.2 | 16.1 |
| `Gun::calcCharacterCollision()` | 82.7 | 65.1 | 75.0 | 74.4 | 67.3 | 47.2 |
| `Level::createMission()` | 83.8 | 53.8 | 65.1 | 64.9 | 36.8 | 24.3 |
| `Status::missionCompleted(bool,bool,long long)` | 86.7 | 61.6 | 86.7 | 86.7 | 57.7 | 45.1 |
| `RocketGun::update(int)` | 86.9 | 74.0 | 86.9 | 86.9 | 68.9 | 62.4 |
| `PlayerEgo::calcCollision(Array<KIPlayer*>*)` | 87.3 | 44.1 | 87.0 | 87.0 | 68.4 | 60.4 |
| `Level::createScene()` | 88.2 | 61.1 | 70.3 | 70.3 | 57.4 | 48.2 |
| `Hud::draw(long long,long long,PlayerEgo*,bool,unsigned,unsigned)` | 88.7 | 44.1 | 84.7 | 81.1 | 46.5 | 33.4 |
| `Hud::drawChallengeModeScore(int)` | 89.1 | 50.6 | 80.3 | 82.4 | 64.7 | 53.7 |
| `MenuTouchWindow::OnTouchEnd(int,int,void*)` | 89.5 | 21.2 | 39.4 | 39.8 | 28.3 | 14.2 |
| `MenuTouchWindow::MenuTouchWindow(int)` (C1) | 91.2 | 50.6 | 68.3 | 63.8 | 40.5 | 34.0 |
| `MenuTouchWindow::MenuTouchWindow(int)` (C2) | 91.2 | 50.6 | 68.3 | 63.8 | 40.5 | 34.0 |
| `StarMap::draw()` | 94.8 | 50.9 | 94.5 | 94.5 | 57.5 | 44.1 |
| `SpaceLounge::drawLounge()` | 95.5 | 57.8 | 86.8 | 86.8 | 57.3 | 37.0 |
| `MenuTouchWindow::OnTouchBegin(int,int,void*)` | 97.2 | 58.6 | 73.8 | 73.8 | 42.6 | 40.6 |
| `ModStation::ModStation()` (C1) | 98.6 | 57.0 | 91.3 | 91.3 | 50.6 | 38.5 |
| `ModStation::ModStation()` (C2) | 98.6 | 57.0 | 91.3 | 91.3 | 50.6 | 38.5 |
| `MGame::OnUpdate()` | 99.9 | 58.7 | 79.2 | 56.9 | 42.8 | 35.2 |
| `StarMap::drawOnScreenInfo(int,bool)` | 99.9 | 44.2 | 99.8 | 99.8 | 54.6 | 38.7 |

Set-level result (byte-exactness over all 4524 functions):

| build | fixed (base-nonbyte -> byte) | newly broken (base-byte -> nonbyte) | 28 still nonbyte |
|---|---:|---:|---:|
| r17c | 0 | 3225 | 28/28 |
| r19 | 0 | 3105 | 28/28 |
| r20 | 0 | 3150 | 28/28 |
| r21 | 0 | 3408 | 28/28 |
| r25c | 0 | 3455 | 28/28 |

## 6. Conclusion

* **Proven:** the original was compiled by NDK r18b clang 7.0.2; r18 is byte-identical to
  r18b for the compiler and headers. No alternate NDK version is the missing piece.
* **Proven:** across clang 6.0.2, 8.0.2, 8.0.7, 9.0.8 and 14.0.7, none of the 28 becomes
  byte-exact and all 28 remain non-byte. The alternates break 3105-3455 other functions.
* **Still hypothesis:** the 28 are source-shape/TU-membership mismatches (expression trees,
  local lifetimes, statement order, register pressure), exactly as the existing
  `build_flags_research.md` concluded for the global flag set. The near-misses
  (`MGame::OnUpdate` 99.9%, `StarMap::drawOnScreenInfo` 99.9%) are the best places to look
  for a one-instruction/one-register source difference; the large misses (`Layout::Layout`
  46.6%, `LevelScript::process` 52.8%) need body reconstruction, not a compiler swap.

## 6b. Caveats / not tested

* The alternate builds deliberately keep **r18b's** libc++ headers, sysroot, static libs and
  gold linker; only the clang binary changes. This isolates compiler codegen. Using an
  alternate NDK's own headers would add a header-version variable and cannot be the original
  (the original's `.note.android.ident` is r18b).
* The darwin-x86_64 r18b clang package was not run (this host is Linux). Its `.comment` is the
  same and LLVM codegen is host-independent; it is not a plausible explanation for the 28.
* r17c/r21/r25c `linked_exact` comes from the fast counter, not a full `verify.py` run (the
  difflib ratio over ~3000 broken functions takes minutes). `byte_exact` and the 28-function
  `pct` use the same `disassemble`/`reconcile`/`score` primitives as `verify.py`.
* The 28-function scorer is the fast counter's loop restricted to the 28 names from
  `report.json`, with `verify.score(on, un)` used for `pct`.

## 7. Reproduction

```sh
# baseline
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint -j8
cmake --build cmake-build-match --target drift -j8

# alternate compiler (repeat for r17c/r19/r20/r21/r25c)
cmake -S . -B /tmp/build-r19 -G Ninja \
      -DCMAKE_TOOLCHAIN_FILE=/tmp/toolchain-r19.cmake -DGOF2_MATCH_OPT=-Oz
cmake --build /tmp/build-r19 --target link_so -j8

# full verify (r19/r20)
python3 tools/verify/verify.py \
  --build-dir /tmp/build-r19/verify \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/build-r19/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
```

The fast counter used for r17c/r21/r25c (byte/linked only, no difflib) is:

```python
import sys, json, tempfile
sys.path.insert(0, 'tools/verify')
import verify
from elf import Elf32

orig_path = '../_work/bins/android_2.0.16_libgof2hdaa.so'
objdump = '.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump'
ours_path = sys.argv[1]
tempfile.tempdir = '/tmp'
of, oe = Elf32(orig_path), Elf32(ours_path)
orig, ours = of.functions(), oe.functions()
common = set(orig) & set(ours)

def addr_range(elf):
    mapped = [s for s in elf.sections if s['addr']]
    lo = next((s['addr'] for s in mapped if s['name'] == '.plt'), min(s['addr'] for s in mapped))
    return lo, max(s['addr'] + s['size'] for s in mapped)

ores = verify.Resolver(orig, common, of.plt_map(), addr_range(of))
ures = verify.Resolver(ours, common, oe.plt_map(), addr_range(oe))
common_objects = {o[2] for o in of.objects()} & {o[2] for o in oe.objects()}
for res, e in ((ores, of), (ures, oe)):
    res.attach_data(e, common_objects)
imports = ({v for k, v in of.got_map().values() if k == 'sym'}
           & {v for k, v in oe.got_map().values() if k == 'sym'})
ores.imports = ures.imports = imports

be = le = 0
for name in sorted(common):
    o, u = orig[name], ours[name]
    if o['size'] > 0 and u['size'] == 0:
        continue
    if o['size'] == 0 and u['size'] > 0:
        continue
    ob = of.bytes_at(o['value'], o['size'])
    ub = oe.bytes_at(u['value'], u['size'])
    if ob == ub:
        be += 1; le += 1; continue
    _, on = verify.disassemble(objdump, ob, o['thumb'], o['value'], ores)
    _, un = verify.disassemble(objdump, ub, u['thumb'], u['value'], ures)
    on, un = verify.reconcile(on, un)
    if on == un and o['size'] == u['size']:
        le += 1
print(dict(byte_exact=be, linked_exact=le))
```

The 28-function `pct` table was produced with the same setup, calling
`verify.disassemble`/`verify.reconcile`/`verify.score` for each name in `report.json` with
`"byte": false`.
