# Debug info and build-id archaeology (w5211)

Status: research note, 2026-11-04. Worktree `/home/debian/GalaxyOnFire2/wt/w5211`,
branch `wave104-w5211`, base `8c5fd3646`. Scratch: `/dev/shm/w5211`.
**No source or build-flag change is landed.** No experiment produced a strict gate
improvement; the note is the deliverable. The canonical build is untouched.

This pass re-tests the full debug-info flag space (`-g` levels, DWARF versions, debug-info
tuning flags, prefix-map/compilation-dir), proves gold's build-id algorithm on this tree,
and searches the shipped Android/iOS/Windows/Steam artifacts for an unstripped original or
debug symbols. It also records the current, independently re-measured `.text`/`.ARM.exidx`/
`.ARM.extab` content deltas, which are smaller than the stale numbers in
`docs/IDENTICALITY.md`.

## 0. Bottom line

* `-g` is **codegen-relevant** and the current `-g` is correct. The original's final
  stripped `.so` is consistent with **`-g2`-level (`-debug-info-kind=limited`)** debug
  info, not `-g0`, `-g1`, `-gline-tables-only`, or `-gmlt`. `-g`, `-g2`, `-g3`,
  `-gdwarf-4`, and `-fno-standalone-debug` are byte-identical at the object level; the
  linked stripped `.so` is identical apart from the build-id.
* `-g0`/`-g1`/`-gline-tables-only`/`-gmlt` all produce the same codegen (identical stripped
  `.so` apart from the build-id): **byte_exact 2069, linked_exact 4247**, vs the baseline
  **4500/4500**. They lose 2,431 byte-exact functions and 253 linked-exact functions.
* `-gdwarf-2`, `-gdwarf-3`, `-gdwarf-4`, `-fno-standalone-debug`, `-gno-column-info`,
  `-fdebug-info-for-profiling`, `-fdebug-prefix-map`, and `-fdebug-compilation-dir` are
  **codegen-neutral**: the stripped `.so` is byte-identical to the baseline apart from the
  20-byte build-id. They are therefore unobservable from the shipped file.
* `-gdwarf-5` **crashes clang 7.0.2** (segfault) on 45 of the app TUs; it cannot be the
  original DWARF version with this compiler/source.
* Gold's `--build-id` is **SHA-1 over the complete unstripped output file with the 20-byte
  build-id descriptor zeroed** (proved on our link). The original build-id
  `c011810da45777b83ed721b2a87d517efed0d310` is the hash of the **original unstripped
  link output**, which is not shipped. The shipped stripped file's own SHA-1 (descriptor
  zeroed) is `d7c05dc5e9804023d8786147addc48537b0bee6d` (ground-truth copy) or
  `ca7d79401291dbc354dd5e1f4fc394397fa4f9e1` (APK copy), not `c011810d...`.
* The original unstripped file would be ~19 MB, of which **57.1% is `.debug_*` and 31.6% is
  `.symtab`+`.strtab`** on our build. Matching the build-id therefore requires matching
  ~17 MB of debug/symbol data, not just the 2.19 MB shipped content. It also requires the
  exact original build directory/source paths (`DW_AT_comp_dir`, `DW_AT_name`), DWARF
  version, and debug-info flags. None of these survive in the stripped `.so`.
* No unstripped original, `.o`, `.a`, `.dSYM`, `.pdb`, `.map`, or debug section exists in
  the APK, IPA, Windows/Steam artifacts, or the local oracle repositories. The only
  surviving path-like strings are the NDK libc++abi buildbot path and the `main.c` log
  strings. The Crashlytics symbol server that might have held a `.cSYM` is dead
  (`cm.crashlytics.com` no longer resolves), and a `.cSYM` would not contain the full
  unstripped `.so` anyway.
* **No strict improvement exists.** Every `-g2`-class variant has the same gate counts
  (4500/4500, size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0) and the same
  stripped bytes apart from the build-id. `-g0`/`-g1` regress; `-gdwarf-5` does not build.
  The note is committed alone; no baseline update.

## 1. Baseline gate and current content deltas

Commands:

```sh
cd /home/debian/GalaxyOnFire2/wt/w5211
mkdir -p /dev/shm/w5211/tmp
TMPDIR=/dev/shm/w5211/tmp cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
TMPDIR=/dev/shm/w5211/tmp cmake --build cmake-build-match --target check -j6
TMPDIR=/dev/shm/w5211/tmp cmake --build cmake-build-match --target drift -j6
```

Result:

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
=== DRIFT: 0 field(s) across 0 class(es) ===
```

`size-exact` is `4524/4524` (from `report.json` rows as `osize == usize`). The 24
non-byte-exact functions are all size-exact:

| pct | size | function |
|---:|---:|---|
| 47.8 | 7072 | `_ZN6LayoutC1Ev` |
| 47.8 | 7072 | `_ZN6LayoutC2Ev` |
| 53.4 | 57232 | `_ZN11LevelScript7processEi` |
| 60.4 | 1432 | `_ZN6Status16missionCompletedEbbx` |
| 64.4 | 1996 | `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` |
| 66.0 | 2764 | `_ZN5Level10createShipEiiiP8Waypointbb` |
| 73.9 | 9908 | `_ZN5Radar4drawEP6PlayerP3Hudi` |
| 74.8 | 8080 | `_ZN5MGame10OnTouchEndEiiPv` |
| 80.2 | 9700 | `_ZN12HangarWindow6renderEv` |
| 80.5 | 612 | `_ZN7GlobalsC1Ev` |
| 80.5 | 612 | `_ZN7GlobalsC2Ev` |
| 86.9 | 1908 | `_ZN9RocketGun6updateEi` |
| 87.6 | 968 | `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` |
| 88.3 | 2252 | `_ZN5Level11createSceneEv` |
| 90.0 | 1984 | `_ZN3Gun22calcCharacterCollisionEv` |
| 92.0 | 17172 | `_ZN5Level13createMissionEv` |
| 92.1 | 9332 | `_ZN3Hud4drawExxP9PlayerEgobjj` |
| 95.1 | 9600 | `_ZN15MenuTouchWindowC1Ei` |
| 95.1 | 9600 | `_ZN15MenuTouchWindowC2Ei` |
| 95.5 | 1948 | `_ZN11SpaceLounge10drawLoungeEv` |
| 97.7 | 1896 | `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` |
| 98.4 | 1008 | `_ZN10ModStationC1Ev` |
| 98.4 | 1008 | `_ZN10ModStationC2Ev` |
| 99.9 | 11440 | `_ZN5MGame8OnUpdateEv` |

Current section content deltas (measured with `objcopy -O binary --only-section=...` and a
bytewise compare; both files are 2,187,328 bytes):

| section | orig size | ours size | differing bytes |
|---|---:|---:|---:|
| `.text` | 1,259,464 | 1,259,464 | **100,113** |
| `.ARM.exidx` | 28,528 | 28,528 | **829** |
| `.ARM.extab` | 156,312 | 156,312 | **108,295** |
| `.rel.plt` | 29,392 | 29,392 | 0 |
| `.rodata` | 269,159 | 269,159 | 0 |
| `.data` | 10,604 | 10,604 | 0 |
| `.dynsym` | 89,248 | 89,248 | 0 |
| `.dynstr` | 189,208 | 189,208 | 0 |
| `.hash` | 38,716 | 38,716 | 0 |
| `.note.gnu.build-id` | 36 | 36 | 20 |

`.text` SHA-256: original `d49e32c1330e5d94e7628b9a40e05500af963a22816a93f2c738dbd366f813ed`,
ours `144a3c3799cddbe2a02e3eef72cf37b213fd24b9a2eccaac866e75640d0a1a2f`.
The `.text`/`.ARM.exidx` numbers are **smaller than the stale `docs/IDENTICALITY.md`
numbers (102,777 / 842)**; the doc also says only `.text`/`.ARM.exidx`/build-id differ,
but `.ARM.extab` content differs by 108,295 bytes (its size and base are exact, and the
differences follow the shifted `.text`/LSDA addresses). This is a doc-accuracy finding, not
a build change.

## 2. Debug-info flag A/B matrix

### 2.1 Method

The tracked toolchain hard-codes `-g` after `${GOF2_MATCH_OPT}`. A scratch wrapper
toolchain replaced that one token and left every other flag untouched:

```cmake
# /dev/shm/w5211/toolchain_dbg.cmake
set(GOF2_DBG_FLAG "-g" CACHE STRING "debug-info flag under test")
include("/home/debian/GalaxyOnFire2/wt/w5211/cmake/toolchain.cmake")
string(REPLACE " -g " " ${GOF2_DBG_FLAG} " _w5211_cxx "${CMAKE_CXX_FLAGS}")
string(REPLACE " -g " " ${GOF2_DBG_FLAG} " _w5211_c   "${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${_w5211_cxx}" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS   "${_w5211_c}"   CACHE STRING "" FORCE)
```

Per variant (scratch build dir under `/dev/shm/w5211`):

```sh
cmake -S . -B /dev/shm/w5211/build-<tag> -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/dev/shm/w5211/toolchain_dbg.cmake \
  -DGOF2_DBG_FLAG='<flag>' -DGOF2_MATCH_OPT=-Oz \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build /dev/shm/w5211/build-<tag> --target check -j6
```

The vendored zlib/libzip/libcrypto build scripts hard-code `-g` independently, so the A/B
differences are entirely in the app TUs. `build-id` is read from the stripped `.so`'s
`.note.gnu.build-id`; `zeroed==base` means the stripped file is byte-identical to the
baseline after zeroing the 20-byte build-id descriptor.

### 2.2 Linked-level results

| tag | flag | byte | linked | size-exact | `.text` | `.rodata` | `.ARM.exidx` | `.ARM.extab` | build-id | zeroed==base | unstripped size |
|---|---|---:|---:|---:|---:|---:|---:|---:|---|---|---:|
| g | `-g` (baseline) | 4500 | 4500 | 4524/4524 | 1,259,464 | 269,159 | 28,528 | 156,312 | `084f1d293f47628a4d553b79d619581196442bc8` | yes | 19,353,700 |
| g0 | `-g0` | 2069 | 4247 | 4484/4524 | 1,259,464 | 269,159 | 28,528 | 156,312 | `690c398e70ff7a44f1f9a637a73faf4c0fd69c84` | no | 3,660,252 |
| g1 | `-g1` | 2069 | 4247 | 4484/4524 | 1,259,464 | 269,159 | 28,528 | 156,312 | `6e4cefa3f396ee161b6cf145a27850df2d90b00a` | no | 5,183,484 |
| g2 | `-g2` | 4500 | 4500 | 4524/4524 | 1,259,464 | 269,159 | 28,528 | 156,312 | `910d07b5d8d2bac97e6ee962dee22edd9d675d6e` | yes | 19,353,676 |
| g3 | `-g3` | 4500 | 4500 | 4524/4524 | 1,259,464 | 269,159 | 28,528 | 156,312 | `33c42f114e613c77b8a1597a56a0d6d2bb40c5b9` | yes | 19,353,676 |
| gline | `-gline-tables-only` | 2069 | 4247 | 4484/4524 | 1,259,464 | 269,159 | 28,528 | 156,312 | `4d636e03a92555965c1b1354f1df18d3f17c2825` | no | 5,183,492 |
| gmlt | `-gmlt` | 2069 | 4247 | 4484/4524 | 1,259,464 | 269,159 | 28,528 | 156,312 | `5a6efd4ab75a117fbec490dfbaf1b8e1ad33ee8d` | no | 5,183,484 |
| gdwarf2 | `-g2 -gdwarf-2` | 4500 | 4500 | 4524/4524 | 1,259,464 | 269,159 | 28,528 | 156,312 | `1a23f1b9d6dc1cf49ae0f146c4e064071d6ba9a0` | yes | 19,964,060 |
| gdwarf3 | `-g2 -gdwarf-3` | 4500 | 4500 | 4524/4524 | 1,259,464 | 269,159 | 28,528 | 156,312 | `e0fc1bae5bd36d706c88178e1b7f8ac8ca40412c` | yes | 19,770,580 |
| gdwarf4 | `-g2 -gdwarf-4` | 4500 | 4500 | 4524/4524 | 1,259,464 | 269,159 | 28,528 | 156,312 | `be4f6b82e20caedcbeee982dd48db4181863738d` | yes | 19,353,676 |
| gdwarf5 | `-g2 -gdwarf-5` | **build fails** | | | | | | | | | |
| fnostandalone | `-g2 -fno-standalone-debug` | 4500 | 4500 | 4524/4524 | 1,259,464 | 269,159 | 28,528 | 156,312 | `70f00cae921be528b0bf424ffaea4fc08fc6bb2e` | yes | 19,353,684 |
| gnocolumn | `-g2 -gno-column-info` | 4500 | 4500 | 4524/4524 | 1,259,464 | 269,159 | 28,528 | 156,312 | `7e75b465793ad2b9ddff24e9772ef4b66042c866` | yes | 18,821,148 |
| fprofiling | `-g2 -fdebug-info-for-profiling` | 4500 | 4500 | 4524/4524 | 1,259,464 | 269,159 | 28,528 | 156,312 | `1c2ed6a873f501b54a9576b5954dd9a05fa54194` | yes | 19,560,612 |
| fprefixmap | `-g2 -fdebug-prefix-map=/home/debian/GalaxyOnFire2/wt/w5211=/src` | 4500 | 4500 | 4524/4524 | 1,259,464 | 269,159 | 28,528 | 156,312 | `79d9bb79f215732b6a6f618fadb6fb2e47d7cee4` | yes | 18,350,492 |
| fcompdir2 | `-g2 -Xclang -fdebug-compilation-dir -Xclang /src` | 4500 | 4500 | 4524/4524 | 1,259,464 | 269,159 | 28,528 | 156,312 | `fb958d859e6e99a9bdcceea941b10c17c0af1766` | yes | 19,353,652 |
| fprefixmap_compdir2 | `-g2 -fdebug-prefix-map=... -Xclang -fdebug-compilation-dir -Xclang /src` | 4500 | 4500 | 4524/4524 | 1,259,464 | 269,159 | 28,528 | 156,312 | `06249fff55c9c5cf05663a3eb4fc3c273e7c9c0a` | yes | 18,350,460 |

All rows had `stub_zero_size 0`, `missing 0`, `wrong_type 0`, `extra 0`, exported data
`843/843`, sodiff extra/missing `0/0`, imports `207`, lint `CLEAN ✓`, and drift `0`.
`-g0`/`-g1`/`-gline`/`-gmlt` fail the ratchet only on `byte_exact`/`linked_exact` (as
expected). The `-g2`-class rows pass the ratchet unchanged.

The different build-ids in the table are **not** evidence of codegen differences: most
variants were built in different scratch directories, and `DW_AT_comp_dir` is part of the
debug info. The same-directory experiment in §2.4 isolates the flag effects.

### 2.3 Object-level results (same directory, exact compile line)

Using the exact `compile_commands.json` line for `src/game/world/Level.cpp` and
`src/platform/android/main.c`, replacing only the `-g` token and the output path, in the
same directory (`/dev/shm/w5211/objtest`), then `objcopy --strip-debug`:

| variant | `Level.cpp` full object SHA-256 (first 16) | `Level.cpp` stripped SHA-256 (first 16) | `main.c` full object SHA-256 (first 16) | `main.c` stripped SHA-256 (first 16) |
|---|---|---|---|---|
| `-g` | `058e6bc37dab5464` | `1d76d1edf6fe62fd` | `8d131ac2b16c0c74` | `164f540f5afd2ff5` |
| `-g0` | `3551dc710442f4e2` | `5e9740cc8af23a32` | `f54d56bcf8b3ad28` | `164f540f5afd2ff5` |
| `-g1` | `76a5abd116830478` | `5e9740cc8af23a32` | `01edd04dedfc686e` | `164f540f5afd2ff5` |
| `-g2` | `058e6bc37dab5464` | `1d76d1edf6fe62fd` | `8d131ac2b16c0c74` | `164f540f5afd2ff5` |
| `-g3` | `058e6bc37dab5464` | `1d76d1edf6fe62fd` | `8d131ac2b16c0c74` | `164f540f5afd2ff5` |
| `-gline-tables-only` | `76a5abd116830478` | `5e9740cc8af23a32` | — | — |
| `-gmlt` | `76a5abd116830478` | `5e9740cc8af23a32` | — | — |
| `-g2 -gdwarf-2` | `8b77e30930e13bb9` | `1d76d1edf6fe62fd` | `26069d732b2f1582` | `164f540f5afd2ff5` |
| `-g2 -gdwarf-3` | `fd1f859128b7c273` | `1d76d1edf6fe62fd` | — | — |
| `-g2 -gdwarf-4` | `058e6bc37dab5464` | `1d76d1edf6fe62fd` | — | — |
| `-g2 -gdwarf-5` | `f4f046e4df7b2474` | `1d76d1edf6fe62fd` | — | — |
| `-g2 -fno-standalone-debug` | `058e6bc37dab5464` | `1d76d1edf6fe62fd` | — | — |
| `-g2 -gno-column-info` | `d90545f847a6780b` | `1d76d1edf6fe62fd` | `579c40291250f1ce` | `164f540f5afd2ff5` |
| `-g2 -fdebug-info-for-profiling` | `7c77276abcb1f50f` | `1d76d1edf6fe62fd` | — | — |
| `-g2 -fdebug-prefix-map=...` | `780ca52c86402884` | `1d76d1edf6fe62fd` | — | — |
| `-g2 -Xclang -fdebug-compilation-dir -Xclang /src` | `1f2e63a4eb4b2704` | `1d76d1edf6fe62fd` | — | — |

Key facts:

* `-g` == `-g2` == `-g3` == `-gdwarf-4` == `-fno-standalone-debug`: **byte-identical full
  objects**.
* `-g1` == `-gline-tables-only` == `-gmlt`: byte-identical full objects; their
  stripped-debug object equals `-g0`'s, not `-g`'s.
* `-gdwarf-2`, `-gdwarf-3`, `-gno-column-info`, `-fdebug-info-for-profiling`,
  `-fdebug-prefix-map`, and `-fdebug-compilation-dir` change only debug sections; after
  `objcopy --strip-debug` the object is byte-identical to `-g`'s.
* `main.c` is codegen-identical for `-g0`/`-g1`/`-g2`/`-g3`; the `-g` codegen effect is a
  C++/PostRA-sinking effect, not a C effect.

Driver mapping (`clang++ -### ... | grep`):

| flag | cc1 setting |
|---|---|
| `-g` | `-debug-info-kind=limited -dwarf-version=4 -dwarf-column-info` |
| `-g2` | `-debug-info-kind=limited -dwarf-version=4 -dwarf-column-info` |
| `-g3` | `-debug-info-kind=limited -dwarf-version=4 -dwarf-column-info` |
| `-g1` | `-debug-info-kind=line-tables-only -dwarf-version=4 -dwarf-column-info` |
| `-gline-tables-only` | `-debug-info-kind=line-tables-only -dwarf-version=4 -dwarf-column-info` |
| `-gmlt` | `-debug-info-kind=line-tables-only -dwarf-version=4 -dwarf-column-info` |
| `-g0` | no `-debug-info-kind`; `-dwarf-column-info` only |
| `-g2 -gdwarf-2/3/4/5` | `-debug-info-kind=limited -dwarf-version=2/3/4/5` |
| `-g2 -gno-column-info` | `-debug-info-kind=limited -dwarf-version=4` (no `-dwarf-column-info`) |
| `-g2 -fdebug-info-for-profiling` | adds `-fdebug-info-for-profiling` |

`-fdebug-compilation-dir` is **not a clang-7.0.2 driver flag**: `clang++: error: unknown
argument: '-fdebug-compilation-dir=/src'`. It must be passed through cc1:
`-Xclang -fdebug-compilation-dir -Xclang /src`. `-fdebug-prefix-map` is a normal driver
flag and also passes `-fdebug-compilation-dir` to cc1.

### 2.4 Same-directory build-id experiment

To isolate debug-info effects on the build-id, all variants below were built in the **same
build directory** (`/dev/shm/w5211/build-same`, removed between runs), so the compile
command, `DW_AT_comp_dir`, vendored libs, and object order are identical. Only the debug
flag differs.

| variant | stripped zeroed SHA-256 (first 16) | build-id | unstripped size | unstripped zeroed SHA-256 (first 16) |
|---|---|---|---|---|
| `-g` | `bf30731a74bdbcac` | `21d1d3236c33a656b1ba9e9355d61491e022faee` | 19,353,676 | `6e2cb23cdea3c7ce` |
| `-g2` | `bf30731a74bdbcac` | `21d1d3236c33a656b1ba9e9355d61491e022faee` | 19,353,676 | `6e2cb23cdea3c7ce` |
| `-g3` | `bf30731a74bdbcac` | `21d1d3236c33a656b1ba9e9355d61491e022faee` | 19,353,676 | `6e2cb23cdea3c7ce` |
| `-g2 -gdwarf-4` | `bf30731a74bdbcac` | `21d1d3236c33a656b1ba9e9355d61491e022faee` | 19,353,676 | `6e2cb23cdea3c7ce` |
| `-g2 -gdwarf-2` | `bf30731a74bdbcac` | `b5989a0a1914eb508601d854b6bd0dda89b78c2f` | 19,964,052 | `c55d72ee0d5c618a` |
| `-g2 -gno-column-info` | `bf30731a74bdbcac` | `ac5c7050a552c9293e89e026ece18fe78125020d` | 18,821,140 | `4ca530121c9bee46` |
| `-g2 -fdebug-info-for-profiling` | `bf30731a74bdbcac` | `e5bbf23ce2aa643b48d7d6adc12f8bcfc0f0172b` | 19,560,612 | `c960c359030e0722` |
| `-g2 -fdebug-prefix-map=...` | `bf30731a74bdbcac` | `780c459856111d5415c7e3aa6462f34cbd052b6a` | 18,350,484 | `54c95c2542086552` |

**This is the key build-id result:** every row has the **same stripped `.so` bytes** apart
from the 20-byte build-id, yet `-gdwarf-2`, `-gno-column-info`, `-fdebug-info-for-profiling`,
and `-fdebug-prefix-map` produce **different build-ids**. The build-id therefore encodes
debug information that is not present in the shipped file. `-g`, `-g2`, `-g3`, and
`-gdwarf-4` produce the same build-id when built in the same directory.

### 2.5 What `-g` does at `-Oz`

The mechanism is the same one recorded in `debug_info_flags_w3564.md`: `-g2` has
`DBG_VALUE` users that pin entry-block argument copies against PostRA machine sinking;
`-g0`/`-g1` have no such users and sink the copy into the error branch. Example
(`InitializeBlowfish`, 416 B in both):

```text
# -g / -g2 (and original)
blx   opensubkeyfile@plt
mov   r6, r0
cbz   r0, error

# -g0 / -g1 / line-tables-only
blx   opensubkeyfile@plt
cbz   r0, error
mov   r6, r0
```

`-g0`/`-g1`/`-gline`/`-gmlt` lose **2,431 byte-exact** and **253 linked-exact** functions.
40 functions change size (net -20 bytes over the common set); the total `.text` size stays
1,259,464 because local functions/alignment absorb the difference. The full list is in
Appendix A. The `-g0`/`-g1`/`-gline`/`-gmlt` stripped `.so` is byte-identical among
themselves (zeroed SHA-256 `9149b1833de8e767...`), and the `-g2`-class stripped `.so` is
byte-identical to the baseline (zeroed SHA-256 `bf30731a74bdbcac...`).

### 2.6 `-gdwarf-5` failure

`-g2 -gdwarf-5` compiles `Level.cpp` fine (full object `f4f046e4df7b2474...`, stripped-debug
identical to `-g`), but the full build fails with a clang frontend **segfault** on 45 TUs:

```text
src/engine/audio/AESoundRessource.cpp
src/engine/core/ApplicationManager.cpp
src/engine/file/AEFile.cpp
src/engine/file/ConfigReader.cpp
src/engine/render/Image.cpp
src/engine/render/PaintCanvas.cpp
src/engine/render/shaders/{BumpMapping,BumpRimCubeShader,BumpShader,ColorMixAdd,
  CubeMapping,CubeNormalMapping,DNSShader,EnergyShield,GenericShader,GenericShader1,
  GenericShader2,GlowShader,GreenShader,MaskShader,NoTexShader,NoTexVtxColorShader,
  PulseShader,SandboxShader,SimpleRefractionShader,SimpleShader,SpecCubeAlphaMapping,
  SpecCubeMapping,TexOnlyShader,TextureAlphaTestShader,TextureLightShader,TextureShader,
  TextureVtxColorShader,VertexColorAlphaTextureShader,VertexColorShader}.cpp
src/game/core/{BumpRimCubeShader_new,BumpShaderCloak,BumpShaderParticle,BumpShaderRefract,
  BumpShaderV2,BumpShaderV3,BumpShaderV4,GameInit,TextureConference}.cpp
src/game/ship/PlayerEgo.cpp
```

`-fno-standalone-debug` does not avoid the crash. Since the original is the same clang
7.0.2 r328903, `-gdwarf-5` is effectively disproven for this source/compiler.

## 3. Gold's build-id algorithm

### 3.1 Proof on our link

The tracked link is a plain gold link with `--build-id`, followed by GNU `strip --strip-all`.
The unstripped output is deleted by the build; re-running the gold command with a different
`-o` reproduces it. On `/dev/shm/w5211/samedir/unstripped-g2.so`:

```python
import hashlib, struct
p = '/dev/shm/w5211/samedir/unstripped-g2.so'
d = bytearray(open(p, 'rb').read())
# .note.gnu.build-id at file offset 0x1cc, size 0x24; descriptor at 0x1cc+16
off = 0x1cc
stored = bytes(d[off+16:off+36]).hex()
d[off+16:off+36] = b'\0' * 20
print(stored)                     # 21d1d3236c33a656b1ba9e9355d61491e022faee
print(hashlib.sha1(d).hexdigest()) # 21d1d3236c33a656b1ba9e9355d61491e022faee
```

Result: **match**. `--build-id=sha1` is identical to the default `--build-id`
(`21d1d323...` both). The note header is the standard 36-byte SHA-1 note:
`namesz=4 descsz=20 type=3 name="GNU\0" desc=20 bytes`.

The hash is over the **whole file**, including the section header table: hashing only bytes
before `e_shoff` gives `1807248e668a03e9b249facc7d9d37f6e023e686`, zeroing the section
headers gives `3fc7f9810ab977dda19cc646ada9800667023c26`, and neither matches. The 20-byte descriptor is zeroed; the note header and the rest of the file
are hashed as written.

### 3.2 The original build-id is not the stripped file's hash

Original file: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(2,187,328 bytes, 27 sections). Stored build-id: `c011810da45777b83ed721b2a87d517efed0d310`.

| computation | SHA-1 |
|---|---|
| whole file as-is | `39528e8a46f5d75adfd851f71bdeb97c02b060f0` |
| whole file, 20-byte descriptor zeroed | `d7c05dc5e9804023d8786147addc48537b0bee6d` |
| before section header table | `da6747226ca7108bf98a72e43e30392022e1f0f3` |
| section headers zeroed | `5ac27826ab76e6e1d93a516f470df6e1aed18b12` |
| APK copy, descriptor zeroed | `ca7d79401291dbc354dd5e1f4fc394397fa4f9e1` |

None equals `c011810d...`. The stored build-id is therefore the SHA-1 of a **larger,
unstripped** file. This independently confirms the original was linked **without `-s`** and
then GNU-stripped: if gold `-s` had produced the shipped file, its build-id would be the
hash of the stripped file (`d7c05d...`/`ca7d79...`), not `c011810d...`.

### 3.3 What unstripped input would be needed

To reproduce `c011810d...`, the original **unstripped gold output** must be byte-identical
to ours. On our same-directory `-g2` link the unstripped file is 19,353,676 bytes in 40
sections (27 shipped + `.debug_str`, `.debug_loc`, `.debug_abbrev`, `.debug_info`,
`.debug_ranges`, `.debug_macinfo`, `.debug_pubnames`, `.debug_pubtypes`, `.debug_frame`,
`.debug_line`, `.debug_aranges`, `.symtab`, `.strtab`). Its composition:

| part | bytes | share |
|---|---:|---:|
| `.debug_*` (11 sections) | 11,045,425 | 57.1% |
| `.symtab` + `.strtab` | 6,120,236 | 31.6% |
| everything else (headers + 27 shipped sections + `.shstrtab`) | 2,188,015 | 11.3% |
| **total** | **19,353,676** | 100% |

The build-id is dominated by data that is **not shipped**. Matching it requires:

1. byte-identical `.text`, `.rodata`, `.data`, `.ARM.exidx`, `.ARM.extab`, `.rel.plt`,
   `.dynsym`, `.dynstr`, `.hash`, `.rel.dyn`, `.got`, `.plt`, `.dynamic`, `.init_array`,
   `.fini_array`, `.comment`, `.note.android.ident`, `.ARM.attributes` — currently
   `.text` differs in 100,113 bytes, `.ARM.exidx` in 829, `.ARM.extab` in 108,295;
2. byte-identical `.symtab`/`.strtab`: every local symbol, file symbol, section symbol,
   symbol name, and symbol order;
3. byte-identical `.debug_*`: DWARF version, abbreviation table, line table, variable
   locations, type info, macro info, pubnames/pubtypes, aranges, frame, ranges, str;
4. the exact original build directory and source paths (`DW_AT_comp_dir`, `DW_AT_name`),
   because `-fdebug-prefix-map`/`-fdebug-compilation-dir` change the build-id while leaving
   the stripped `.so` identical;
5. byte-identical section header table (40 sections) and `.shstrtab` (gold's Stringpool
   order);
6. byte-identical ELF header and program headers.

The original unstripped size is unknown but would be of the same order (~19 MB). The
build-id is a 160-bit one-way hash: there is no partial credit and no way to invert it. It
is not a usable independent oracle until every other byte of the unstripped link matches;
at that point it would confirm the debug-info flags/paths. It cannot guide the 24
non-byte-exact functions.

## 4. Artifact search: unstripped binaries, symbols, source paths

### 4.1 Original APK

```sh
APK="/home/debian/GalaxyOnFire2/Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk"
unzip -l "$APK"
unzip -Z1 "$APK" | grep -Ei '\.(o|a|d|mk|cmake|gradle|log|pdb|dSYM|map|sym|debug|dwarf)$'
```

143 entries; no `.o`, `.a`, `.d`, `.mk`, `.cmake`, `.gradle`, `.log`, `.pdb`, `.dSYM`,
`.map`, `.sym`, or debug file. The only build-system strings are
`Created-By: Android Gradle 3.1.2` (MANIFEST.MF) and the Crashlytics timestamp
`#Mon Oct 22 10:18:17 CEST 2018` (`assets/crashlytics-build.properties`). The native
library is stripped; no `.symtab`/`.debug_*`.

The only path-like string in the original `.so` is the NDK libc++abi buildbot path:

```text
/Volumes/Android/buildbot/src/android/ndk-release-r18/external/libcxx/../../external/libcxxabi/src/abort_message.cpp
```

The only app source-file-name witness is `main.c`, in three log strings
(`dataDirectory in main.c: %s`, `rootPath in main.c: %s`, `zipDirectory in main.c: %s`),
referenced from `Java_net_fishlabs_gof2hdallandroid2012_ToJNI_SetDirectories`. No Android
app source path survives.

### 4.2 iOS IPA

```sh
IPA="/home/debian/GalaxyOnFire2/Original IPA/Galaxy_on_Fire_2__HD_1.1.4_ios_4.3.ipa"
unzip -l "$IPA" | grep -Ei '\.(dSYM|o|a|d|pdb|map|sym|debug|dwarf)$'
llvm-objdump --macho --section-headers _work/bins/ios_1.1.4_HD
llvm-nm -a _work/bins/ios_1.1.4_HD | wc -l
```

No dSYM, no `.o`/`.a`, no DWARF sections (the binary is stripped of DWARF). The 1.1.4 binary
has 1,528 symbols (713 external defined, 1 local) and the 1.0.5 armv7 binary has 828
symbols (172 external defined, 1 local); these are Mach-O symbol tables, not DWARF. The only
Fishlabs source paths are FMOD/assert strings such as
`/Users/hck/Documents/Dev/GalaxyOnFire2_merged/Classes/Game/ParticleSystems/*.cpp` and the
FMOD build paths. The iOS build is a different source version/compiler (Xcode 4.5 / Apple
clang 1.0 for 1.1.4; Xcode 4.0 / GCC 4.2 for 1.0.5), so it is not an Android debug-info
oracle.

### 4.3 Windows / Steam

```sh
llvm-readobj --file-headers --coff-debug-directory --symbols \
  "/home/debian/GalaxyOnFire2/_work/bins/win_GOF2.exe"
find "/home/debian/GalaxyOnFire2/Original Windows" -type f
```

`win_GOF2.exe` has `PointerToSymbolTable: 0x0`, `SymbolCount: 0`, and no COFF symbol table.
The only debug information is a CodeView entry:

```text
PDBSignature: 0x53445352
PDBGUID: {9A525EDE-5753-47D1-B793-9D334ED401D4}
PDBAge: 2
PDBFileName: C:\Users\s.lohse\Desktop\GoF2_PC_REP\GOF2.pdb
```

No `.pdb`, `.lib`, `.exp`, `.map`, `.o`, or `.ilk` is shipped in `Original Windows/Steam`.
The Steam `installscript.vdf` contains only DirectX/VC++ redistributable commands and a
signature; no build id. The Windows build is VS2010/x86 and cannot validate Android flags.

### 4.4 Unity Remake LFS reference (different Android build)

The oracle repo `_work/oracles/Galaxy-on-Fire-2-Unity-Remake` has Git-LFS pointers for
`Reference/binaries/libgof2hdaa.so` (2,469,656 bytes) and
`Reference/binaries/GOF2_2.0.16_Android_Price.apk` (7,767,689 bytes). Fetching both from
the LFS server gives a **different** Android build:

| property | value |
|---|---|
| size | 2,469,656 bytes |
| sections | 26 (no `.note.android.ident`) |
| compiler | `Android clang version 3.8.275480 (based on LLVM 3.8.275480)` |
| linker | `gold 1.11` |
| `.text` | 0x1682c8 |
| build-id | `1dbb7d451b7ca0ff0cfccf2f2d2fae0e8f9c5a1c` |
| debug info | none (stripped) |
| path string | `/Volumes/Android/buildbot/src/android/ndk-r14-release/.../abort_message.cpp` |

This is an NDK r14 build of 2.0.16 (the "Price" modded APK), not the ground truth
(NDK r18b / clang 7.0.2 / gold 1.12). It is stripped, so it adds no debug info. Its
build-id is also a hash of its own unstripped output and is not reproducible from the
shipped file.

### 4.5 Source-layout dumps

`../source file list dump/` contains iOS/mac/windows/MeeGo source→object→symbol dumps
(`gof2hd_1.1.19_ios.cpp`, `gof2sd_1.0.1_ios.cpp`, `gof3d_1.1.4_ios.cpp`,
`source file lists dump.txt`). They give original TU membership and source roots such as
`/Users/n.tran/projects/gof2/`, `/Users/mhehmeyer/Documents/Dev/GalaxyOnFire2/`, and
`/Users/hck/Documents/Dev/GalaxyOnFire2_merged/`, but **no Android build directory or
Android build flags**. They are not debug-info evidence for the Android `.so`.

### 4.6 Crashlytics lead (server dead)

The APK's `AndroidManifest.xml` contains the Fabric API key
`d5485de3d8f8426719f8b164856b4a3ede0c606d`; `assets/crashlytics-build.properties` contains
the Crashlytics build id `f527605b-309f-404d-8afc-b68027edc49e`. The Crashlytics NDK
plugin uploads a `.cSYM` symbol file (symbol/line mapping) to the old endpoint
`https://cm.crashlytics.com/api/v2/platforms/android/apps/<key>/builds/<build_id>/native`.
`cm.crashlytics.com` **no longer resolves** (NXDOMAIN from the local resolver and public
DNS), so the old symbol archive is not reachable. The modern endpoint
`https://firebasecrashlyticssymbols.googleapis.com/v1/project/-/app/<app>/upload/native/<uuid>`
is an upload-only GCS endpoint (GET returns 405). Even if the old `.cSYM` were retrieved, it
would contain symbol/line mappings, not the full unstripped `.so`, so it would not by itself
reproduce the build-id.

## 5. Proven, disproven, unobservable

### Positively proven

* The original is NDK r18b / clang 7.0.2 r328903 / gold 1.12, API 16, ARMv7-A, Thumb-2,
  NEON, softfp (`.comment`, `.note.android.ident`, `.note.gnu.gold-version`,
  `.ARM.attributes`).
* The original was compiled with at least **`-g2`-level (`-debug-info-kind=limited`)**
  debug info: `-g`/`-g2`/`-g3` reproduce 4500/4500; `-g0`/`-g1`/line-tables-only reproduce
  2069/4247 and change 40 function sizes.
* `-g` is codegen-relevant at `-Oz` via `DBG_VALUE` pinning of entry-block copies; the
  `InitializeBlowfish` copy/sink example is reproduced.
* Gold's `--build-id` is SHA-1 over the complete unstripped output with the 20-byte
  descriptor zeroed; `--build-id=sha1` is the same as the default; the hash includes the
  section header table.
* The original build-id `c011810d...` is the hash of the original unstripped link output,
  not of the shipped stripped file (`d7c05d...`/`ca7d79...`).
* The original was linked without `-s` and then GNU-stripped (build-id + bfd-normalized
  section headers).
* `-g` == `-g2` == `-g3` == `-gdwarf-4` == `-fno-standalone-debug` at the object level;
  `-g1` == `-gline-tables-only` == `-gmlt`; `-g0` and `-g1` are codegen-identical.
* `-gdwarf-2/3/4`, `-gno-column-info`, `-fdebug-info-for-profiling`, `-fdebug-prefix-map`,
  and `-fdebug-compilation-dir` are codegen-neutral (stripped `.so` identical apart from
  build-id).
* `-gdwarf-5` crashes clang 7.0.2 on 45 TUs.
* `-fdebug-compilation-dir` requires `-Xclang -fdebug-compilation-dir -Xclang <dir>` in
  clang 7.0.2.
* The build-id is sensitive to `DW_AT_comp_dir` and to debug-info flags even when the
  stripped `.so` is identical (same-directory experiment).
* No unstripped original, `.o`, `.a`, `.dSYM`, `.pdb`, `.map`, or debug section exists in
  the APK/IPA/Windows/Steam artifacts; the Unity Remake reference is a different NDK r14
  build.

### Disproven

* `-g0`, `-g1`, `-gline-tables-only`, `-gmlt` as the original debug level (codegen mismatch:
  2069/4247 vs 4500/4500).
* `-gdwarf-5` as the original DWARF version (full build segfaults on clang 7.0.2).
* `-g3` as distinguishable from `-g2` (identical full objects, same-directory build-id).
* `-fdebug-prefix-map`/`-fdebug-compilation-dir` as codegen-affecting flags (neutral).
* The build-id being a hash of the stripped file (hash mismatch).
* The existence of an unstripped original in the shipped artifacts (none found).

### Unobservable / unknown

* Exact debug-info level between `-g2` and `-g3` (identical in clang 7.0.2).
* Exact DWARF version (2/3/4 are codegen-neutral; clang 7.0.2 default is 4; `-gdwarf-5`
  crashes).
* `-fno-standalone-debug`, `-gno-column-info`, `-fdebug-info-for-profiling` (codegen-neutral).
* `-fdebug-prefix-map` / `-fdebug-compilation-dir` values, the original build directory,
  and the original source paths.
* The exact original `.symtab`, `.strtab`, `.debug_*` contents and unstripped size.
* The exact original CMake/ndk-build script and CMake version.
* Whether the original used `-g` or `-g2` (same compiler setting).
* Whether the original used `-gdwarf-2/3/4` (same codegen; default 4).
* The original build-id cannot be reproduced without the original unstripped link output.

## 6. Confidence table

| claim | confidence | hard evidence |
|---|---|---|
| Original compile line includes `-g` at `-g2`-level | **High** | `-g`/`-g2`/`-g3` = 4500/4500; `-g0`/`-g1`/line = 2069/4247; 40 size changes; `InitializeBlowfish` copy/sink |
| `-g` == `-g2` == `-g3` in clang 7.0.2 | **High** | byte-identical full objects; same-directory build-id `21d1d323...` |
| `-g1` == `-gline-tables-only` == `-gmlt` | **High** | byte-identical full objects; same stripped codegen as `-g0` |
| `-gdwarf-2/3/4` codegen-neutral | **High** | stripped-debug objects identical for `Level.cpp`; stripped `.so` identical apart from build-id |
| `-gdwarf-5` incompatible with clang 7.0.2 here | **High** | 45-TU segfault, reproduced with and without `-fno-standalone-debug` |
| `-fdebug-compilation-dir` is cc1-only in clang 7.0.2 | **High** | driver `unknown argument`; `-Xclang` form compiles |
| `-fdebug-prefix-map`/`-fdebug-compilation-dir` change build-id only | **High** | same-directory stripped `.so` identical; build-id differs |
| Gold build-id = SHA-1(whole unstripped file, descriptor zeroed) | **High** | recomputation matches stored `21d1d323...`; `--build-id=sha1` same; truncated/zeroed variants don't match |
| Original build-id is unstripped-output hash | **High** | stored `c011810d...` ≠ stripped hashes `d7c05d...`/`ca7d79...`; gold 1.12 same algorithm |
| Original was linked without `-s` then stripped | **High** | build-id + bfd-normalized headers + `.shstrtab` reorder |
| No unstripped original in shipped artifacts | **High** | APK/IPA/Windows/Steam inventories; no `.o`/`.a`/`.dSYM`/`.pdb`/DWARF |
| Unity Remake reference is a different NDK r14 build | **High** | clang 3.8.275480, gold 1.11, 26 sections, build-id `1dbb7d45...`, no `.note.android.ident` |
| Crashlytics old symbol server is dead | **Medium-High** | `cm.crashlytics.com` NXDOMAIN; modern endpoint 405 GET; `.cSYM` is not the full `.so` |
| Exact original DWARF version / debug flags | **Unobservable** | stripped `.so`; codegen-neutral variants |
| Exact original build directory/source paths | **Unobservable** | no app path in binary; build-id is one-way |
| Original unstripped size | **Unknown** | not shipped; our build is 19,353,676 B |

## 7. Strict improvement?

**No.** Every `-g2`-class variant has the same gate counts and the same stripped bytes
apart from the build-id:

```text
byte_exact 4500/4524, linked_exact 4500/4524, size-exact 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0
```

`-g0`/`-g1`/`-gline`/`-gmlt` regress to 2069/4247; `-gdwarf-5` does not build.
`-fdebug-prefix-map`/`-fdebug-compilation-dir` are gate-neutral and have no evidence for a
particular original path. The current `-g` is already the codegen-correct choice, and
`-gdwarf-4` is the clang default. No source or build change is made; no baseline update is
required. The note is committed alone.

## 8. Reproduction commands

```sh
# baseline
cd /home/debian/GalaxyOnFire2/wt/w5211
mkdir -p /dev/shm/w5211/tmp
TMPDIR=/dev/shm/w5211/tmp cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
TMPDIR=/dev/shm/w5211/tmp cmake --build cmake-build-match --target check -j6
TMPDIR=/dev/shm/w5211/tmp cmake --build cmake-build-match --target drift -j6

# debug-info A/B (wrapper toolchain in /dev/shm/w5211/toolchain_dbg.cmake)
cmake -S . -B /dev/shm/w5211/build-<tag> -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/dev/shm/w5211/toolchain_dbg.cmake \
  -DGOF2_DBG_FLAG='<flag>' -DGOF2_MATCH_OPT=-Oz \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build /dev/shm/w5211/build-<tag> --target check -j6

# regenerate the unstripped gold output from a build dir
python3 - <<'PY'
import re, os, subprocess, sys
build, out = sys.argv[1], sys.argv[2]
txt = open(os.path.join(build, 'build.ninja')).read()
for line in txt.splitlines():
    if line.startswith('  COMMAND = ') and 'ld.gold' in line:
        cmd = line[len('  COMMAND = '):]; break
if cmd.startswith('cd '): cmd = cmd.split(' && ', 1)[1]
gold = cmd.split(' && ')[0]
gold = re.sub(r'-o \S+', '-o ' + out, gold, count=1)
subprocess.run(gold, shell=True, cwd=build, check=True)
PY

# build-id proof
python3 - <<'PY'
import hashlib, struct
p = '/dev/shm/w5211/samedir/unstripped-g2.so'
d = bytearray(open(p, 'rb').read())
off = 0x1cc
stored = bytes(d[off+16:off+36]).hex()
d[off+16:off+36] = b'\0' * 20
print('stored  ', stored)
print('computed', hashlib.sha1(d).hexdigest())
PY

# original stripped-file hashes
python3 - <<'PY'
import hashlib, struct
p = '/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so'
d = bytearray(open(p, 'rb').read())
shoff = struct.unpack_from('<I', d, 32)[0]
shent = struct.unpack_from('<H', d, 46)[0]
shnum = struct.unpack_from('<H', d, 48)[0]
shstr = struct.unpack_from('<H', d, 50)[0]
S = [struct.unpack_from('<IIIIIIIIII', d, shoff+i*shent) for i in range(shnum)]
sh = S[shstr]
def name(n):
    e = d.index(b'\0', sh[4]+n); return d[sh[4]+n:e].decode()
off = [s[4] for s in S if name(s[0]) == '.note.gnu.build-id'][0]
print('stored', bytes(d[off+16:off+36]).hex())
d[off+16:off+36] = b'\0'*20
print('zeroed', hashlib.sha1(d).hexdigest())
PY

# artifact inventories
APK="/home/debian/GalaxyOnFire2/Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk"
unzip -l "$APK" | grep -Ei '\.(o|a|d|mk|cmake|gradle|log|pdb|dSYM|map|sym|debug|dwarf)$'
IPA="/home/debian/GalaxyOnFire2/Original IPA/Galaxy_on_Fire_2__HD_1.1.4_ios_4.3.ipa"
unzip -l "$IPA" | grep -Ei '\.(dSYM|o|a|d|pdb|map|sym|debug|dwarf)$'
llvm-readobj --file-headers --coff-debug-directory --symbols \
  /home/debian/GalaxyOnFire2/_work/bins/win_GOF2.exe
strings -a -n 6 /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  | grep -E '\.(c|cc|cpp|cxx|h|hpp|mm)$|^/[A-Za-z0-9_./-]+$'
```

## Appendix A: functions whose size changes between `-g2` and `-g0`

`-g0`, `-g1`, `-gline-tables-only`, and `-gmlt` are identical in codegen; this is the
`-g2` -> `-g0` size delta list (40 functions, net -20 bytes over the common set; `.text`
total unchanged at 1,259,464):

| function | `-g2` size | `-g0` size | delta |
|---|---:|---:|---:|
| `_ZN5MGame10OnTouchEndEiiPv` | 8080 | 8084 | +4 |
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 1896 | 1892 | -4 |
| `_Z10keyPressedPN11AbyssEngine6EngineEi` | 708 | 704 | -4 |
| `_ZN10AEGeometry9updateLodERKN11AbyssEngine6AEMath6VectorEf` | 532 | 528 | -4 |
| `_ZN10GameRecord4loadEv` | 2696 | 2692 | -4 |
| `_ZN10ModStation10OnTouchEndEiiPv` | 5404 | 5416 | +12 |
| `_ZN10ModStation11OnTouchMoveEiiPv` | 384 | 388 | +4 |
| `_ZN10RepairBeam6updateEiP5RadarP5LevelP3Hud` | 1872 | 1868 | -4 |
| `_ZN11AbyssEngine11PaintCanvas11DrawImage2DEjiih` | 340 | 348 | +8 |
| `_ZN11AbyssEngine11PaintCanvas13Image2DCreateEtRj` | 192 | 188 | -4 |
| `_ZN11AbyssEngine11PaintCanvas20TransformRemoveChildEjj` | 60 | 62 | +2 |
| `_ZN11AbyssEngine11PaintCanvas24MeshChangeMaterialInternEPNS_9TransformEPNS_8MaterialE` | 80 | 76 | -4 |
| `_ZN11AbyssEngine11PaintCanvas29MaterialResourceChangeTextureEjji` | 60 | 58 | -2 |
| `_ZN11AbyssEngine12CurveReleaseEPPNS_5CurveE` | 86 | 90 | +4 |
| `_ZN11AbyssEngine16ShaderBaseStruct14ES2LoadProgramEPKcS2_` | 240 | 236 | -4 |
| `_ZN11AbyssEngine18ApplicationManager25RegisterApplicationModuleEjPNS_18IApplicationModuleE` | 58 | 54 | -4 |
| `_ZN11AbyssEngine18ApplicationManager8SoundSetEPKNS_11AESoundInfoEi` | 52 | 50 | -2 |
| `_ZN11AbyssEngine6Engine11SetTexturesEjj` | 312 | 316 | +4 |
| `_ZN11AbyssEngine6Engine14ShaderRegisterEPNS_16ShaderBaseStructE` | 128 | 124 | -4 |
| `_ZN11AbyssEngine6String5SplitES0_` | 196 | 194 | -2 |
| `_ZN11AbyssEngine6String9SplitTagsES0_` | 456 | 452 | -4 |
| `_ZN12Achievements28initCheckEquipmentAndWeaponsEv` | 164 | 172 | +8 |
| `_ZN12HangarWindow6updateEi` | 488 | 486 | -2 |
| `_ZN14DialogueWindow3setEP7Missionii` | 120 | 116 | -4 |
| `_ZN16SystemPathFinder15getJumpDistanceEP5ArrayIP11SolarSystemEii` | 38 | 36 | -2 |
| `_ZN4Item17combineDuplicatesEP5ArrayIPS_E` | 130 | 128 | -2 |
| `_ZN4Ship10getSlotPosEP4Item` | 84 | 82 | -2 |
| `_ZN4Ship13getCargoValueEv` | 52 | 56 | +4 |
| `_ZN5Level16almostKillWantedEi` | 252 | 248 | -4 |
| `_ZN5MGame12OnInitializeEv` | 2080 | 2076 | -4 |
| `_ZN6Player15StopEngineSoundEv` | 40 | 44 | +4 |
| `_ZN6Player16PauseEngineSoundEv` | 36 | 40 | +4 |
| `_ZN6Player4healEf` | 94 | 100 | +6 |
| `_ZN6Player6addGunEP5ArrayIP3GunEi` | 118 | 116 | -2 |
| `_ZN6Player6damageEibi` | 1236 | 1232 | -4 |
| `_ZN8CutScene14checkForTurretEv` | 1048 | 1044 | -4 |
| `_ZN9BluePrint19getIngredientsValueEv` | 88 | 92 | +4 |
| `_ZN9FModSound23playMusicFadeOutCurrentEi` | 44 | 48 | +4 |
| `ndk_setNativeItemInformationList` | 1504 | 1496 | -8 |
| `pConstToNonConst` | 40 | 36 | -4 |

## Appendix B: raw build-id / hash values

```text
original stored build-id                 c011810da45777b83ed721b2a87d517efed0d310
original stripped SHA-1 (as-is)          39528e8a46f5d75adfd851f71bdeb97c02b060f0
original stripped SHA-1 (desc zeroed)    d7c05dc5e9804023d8786147addc48537b0bee6d
APK copy stripped SHA-1 (desc zeroed)    ca7d79401291dbc354dd5e1f4fc394397fa4f9e1
original .text SHA-256                   d49e32c1330e5d94e7628b9a40e05500af963a22816a93f2c738dbd366f813ed
ours .text SHA-256                       144a3c3799cddbe2a02e3eef72cf37b213fd24b9a2eccaac866e75640d0a1a2f
baseline stripped .so SHA-256            cf090f33cd9f2e577fab3e4dc91a612dd37c4d6b7067050175a43a3bd10c544b
baseline unstripped build-id             084f1d293f47628a4d553b79d619581196442bc8
same-dir -g/-g2/-g3/-gdwarf-4 build-id   21d1d3236c33a656b1ba9e9355d61491e022faee
same-dir -gdwarf-2 build-id              b5989a0a1914eb508601d854b6bd0dda89b78c2f
same-dir -gno-column-info build-id       ac5c7050a552c9293e89e026ece18fe78125020d
same-dir -fdebug-info-for-profiling      e5bbf23ce2aa643b48d7d6adc12f8bcfc0f0172b
same-dir -fdebug-prefix-map             780c459856111d5415c7e3aa6462f34cbd052b6a
Unity Remake NDK r14 build-id            1dbb7d451b7ca0ff0cfccf2f2d2fae0e8f9c5a1c
```

## Appendix C: files touched

* `_work/reconstructions/debug_info_build_id_w5211.md` (this note).
* Scratch only: `/dev/shm/w5211/` (wrapper toolchain, build dirs, unstripped links,
  object A/B files, logs). The worktree contains only the note.
