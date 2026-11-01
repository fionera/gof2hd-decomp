# Debug-info flags (`-g` variants): codegen effect and original level

Status: research note, worktree `wt/w3564`, branch `wave87-w3564`, base `113737ee`.
Every number below is from a command run in this worktree against
`../_work/bins/android_2.0.16_libgof2hdaa.so`, or from a controlled rebuild of this tree.
No source or flag change was landed: the current `-g` is already the codegen-correct choice.

## TL;DR

`-g` is **not** metadata-only at clang 7.0.2 `-Oz` on this tree. The codegen threshold is
**`-g2` (`-debug-info-kind=limited`)**, not line tables:

| variant | byte_exact | linked_exact | size-exact | avg | verdict |
|---|---:|---:|---:|---:|---|
| `-g` (current) | **4496** | **4496** | **4524/4524** | 99.9 | correct |
| `-g2` | **4496** | **4496** | **4524/4524** | 99.9 | identical to `-g` |
| `-g3` | **4496** | **4496** | **4524/4524** | 99.9 | identical to `-g2` in clang 7.0.2 |
| `-g2 -gdwarf-2` | **4496** | **4496** | **4524/4524** | 99.9 | codegen identical |
| `-g2 -gdwarf-3` | **4496** | **4496** | **4524/4524** | 99.9 | codegen identical |
| `-g2 -gdwarf-4` | **4496** | **4496** | **4524/4524** | 99.9 | identical to `-g2` |
| `-g2 -fno-standalone-debug` | **4496** | **4496** | **4524/4524** | 99.9 | identical to `-g2` |
| `-g2 -gno-column-info` | **4496** | **4496** | **4524/4524** | 99.9 | codegen identical |
| `-g2 -fdebug-info-for-profiling` | **4496** | **4496** | **4524/4524** | 99.9 | codegen identical |
| `-g0` | 2069 | 4245 | 4485/4524 | 99.55 | **wrong** |
| `-g1` | 2069 | 4245 | 4485/4524 | 99.55 | **wrong** |
| `-gline-tables-only` | 2069 | 4245 | 4485/4524 | 99.55 | **wrong** |
| `-gmlt` | 2069 | 4245 | 4485/4524 | 99.55 | **wrong** |
| `-g0 -mllvm -disable-postra-machine-sink` | 1573 | 4384 | 4473/4524 | 99.74 | mechanism probe, not a candidate |

All rows had `stub_zero_size 0`, `missing 0`, `wrong_type 0`, `extra 0`, exported data
`843/843`, sodiff extra/missing `0/0`, imports `207`, lint `CLEAN ✓`, and drift `0`.
The `-g0`/`-g1`/`-gline`/`-gmlt` rows fail the ratchet only on `byte_exact` and
`linked_exact` (expected).

**Conclusion:** the original was compiled with at least `-g2`-level variable debug info.
The ndk-build r18b default is exactly `GLOBAL_CFLAGS += -g`, and clang 7.0.2 maps `-g` to
`-debug-info-kind=limited` (the same cc1 setting as `-g2`). Keep `-g`; do not replace it with
`-g0`, `-g1`, `-gline-tables-only`, or `-gmlt`. `-g3` is indistinguishable from `-g2` here,
so there is no positive evidence for `-g3`.

## 1. Baseline

Commands:

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
```

Result:

```
verify: byte 4496  linked 4496  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
=== DRIFT: 0 field(s) across 0 class(es) ===
```

`size-exact` is `4524/4524` (computed from `report.json` rows as `osize == usize`).
`.text` is `0x1337c8` at `0x69440`; `.rodata` is `0x41b67` at `0x1c9e10`. All ELF section
sizes and bases are unchanged across the `-g`-level variants below (verified with
`readelf -SW`). The `-g0 -mllvm -disable-postra-machine-sink` probe is the exception: its
`.text` grows to `0x134d68`, shifting the downstream section bases.

## 2. Controlled A/B method

The tracked toolchain hard-codes `-g` after `${GOF2_MATCH_OPT}`, so a scratch wrapper
toolchain replaced that one token and left every other flag untouched. `/tmp` was inode-full
on this host, so the scratch build dirs live under the (untracked, removed-at-end)
`.w3564-scratch/` directory in the worktree.

Wrapper (`/tmp/w3564-dbg/toolchain_dbg.cmake`):

```cmake
set(GOF2_DBG_FLAG "-g" CACHE STRING "debug-info flag under test")
include("/home/debian/GalaxyOnFire2/wt/w3564/cmake/toolchain.cmake")
string(REPLACE " -g " " ${GOF2_DBG_FLAG} " _w3564_cxx "${CMAKE_CXX_FLAGS}")
string(REPLACE " -g " " ${GOF2_DBG_FLAG} " _w3564_c   "${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${_w3564_cxx}" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS   "${_w3564_c}"   CACHE STRING "" FORCE)
```

Per variant:

```sh
cmake -S . -B .w3564-scratch/build-<tag> -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/tmp/w3564-dbg/toolchain_dbg.cmake \
  -DGOF2_DBG_FLAG='<flag>' -DGOF2_MATCH_OPT=-Oz
cmake --build .w3564-scratch/build-<tag> --target check -j16
cmake --build .w3564-scratch/build-<tag> --target drift -j8
cmake --build .w3564-scratch/build-<tag> --target lint -j8
```

The vendored zlib/libzip build script hard-codes `-g` independently of the wrapper, so the
A/B differences below are entirely in the 203 app TUs.

## 3. What clang 7.0.2 actually does

Driver mapping (`clang++ -### ... | grep -E 'debug-info-kind|dwarf-version'`):

| flag | cc1 debug kind | DWARF |
|---|---|---|
| `-g` | `-debug-info-kind=limited` | 4 |
| `-g2` | `-debug-info-kind=limited` | 4 |
| `-g3` | `-debug-info-kind=limited` | 4 |
| `-g1` | `-debug-info-kind=line-tables-only` | 4 |
| `-gline-tables-only` | `-debug-info-kind=line-tables-only` | 4 |
| `-gmlt` | `-debug-info-kind=line-tables-only` | 4 |
| `-g0` | (none) | — |

So `-g` and `-g2` are the same compiler setting; `-g1`, `-gline-tables-only`, and `-gmlt`
are the same compiler setting. `-g3` also lowers to `limited` in this clang build (no macro
debug-info difference was observable). `-g2 -gdwarf-5` is not usable with this clang:
it segfaults in codegen on `PaintCanvas.cpp` and `PlayerEgo.cpp`.

Same-directory object checks (one C TU and one C++ TU, exact `compile_commands.json`
command line, only the `-g` token replaced, same output directory) gave:

* `-g` == `-g2` == `-g3` == `-g2 -gdwarf-4` == `-g2 -fno-standalone-debug`:
  byte-identical full objects.
* `-g1` == `-gline-tables-only` == `-gmlt`: byte-identical full objects.
* `-g0`: distinct full objects.
* `-g2 -gdwarf-2`, `-g2 -gdwarf-3`, `-g2 -gno-column-info`, and
  `-g2 -fdebug-info-for-profiling`: different debug sections, but after
  `objcopy --strip-debug` all 203 app objects are byte-identical to the `-g2` objects.
* After zeroing the 20-byte `.note.gnu.build-id` payload, the linked `.so` for every
  `-g2`-class variant is byte-identical to the baseline `.so`.

## 4. How large the `-g0` codegen change is

`-g0` vs `-g2` (both `.text` size `0x1337c8`; build-id zeroed before comparison):

| section | differing bytes |
|---|---:|
| `.text` | 803,145 / 1,259,464 |
| `.dynsym` | 3,683 / 89,248 |
| `.rel.plt` | 11 / 29,392 |
| `.ARM.exidx` | 2,668 / 28,528 |
| `.ARM.extab` | 313 / 156,304 |
| `.data.rel.ro` | 10 / 12,264 |

At object level, **74 of 203** app TUs differ after `objcopy --strip-debug`; 129 are
identical. On the common dynsym function set, **39 functions change size** (net +24 bytes
over that set); the total `.text` size nevertheless remains `0x1337c8` because local
functions/alignment absorb the difference.

Concrete original-vs-variant checks:

| function | original size | `-g2` | `-g0` |
|---|---:|---|---|
| `InitializeBlowfish` | 416 | 416, **0 differing bytes** | 416, 4 differing bytes |
| `_Z10keyPressedPN11AbyssEngine6EngineEi` | 708 | 708, **0 differing bytes** | 704, 621 differing bytes |
| `_ZN10ModStation10OnTouchEndEiiPv` | 5404 | 5404, **0 differing bytes** | 5416, size mismatch |
| `_ZN11AbyssEngine11PaintCanvas11DrawImage2DEjiih` | 340 | 340, **0 differing bytes** | 348, size mismatch |

The `InitializeBlowfish` difference is the classic entry-block copy sink:

```
# original and -g2
blx   opensubkeyfile@plt
mov   r6, r0
cbz   r0, error

# -g0
blx   opensubkeyfile@plt
cbz   r0, error
mov   r6, r0          @ sunk into the error branch
```

The machine-MIR dump after `PostRA Machine Sink` (`-mllvm -print-machineinstrs`) shows
why: `-g2` has a `DBG_VALUE ... !"error"` for the copy in the entry block, which pins it
there; `-g0` has no `DBG_VALUE`, and the `$r6 = COPY $r0` is sunk into the error branch.
This is a proven mechanism at this site, not a claim that every one of the 2,427 lost
byte-exact functions has the same single cause. A global
`-g0 -mllvm -disable-postra-machine-sink` probe does **not** recover the gate
(1573/4384, `.text 0x134d68`), so the full `-g` effect is not just "turn off PostRA sinking".

## 5. Original-build evidence

The NDK r18b build system still on this host says:

```sh
grep -n -B2 -A2 'Always enable debug info\|GLOBAL_CFLAGS += -g' \
  .cache/ndk/android-ndk-r18b/build/core/default-build-commands.mk
# 143:# Always enable debug info. We strip binaries when needed.
# 144:GLOBAL_CFLAGS += -g
```

That is the same `-g` the repo already uses. The codegen A/B above proves the original's
final stripped `.so` is consistent with `-g2`-level debug info, not with `-g0`/line-tables.
The original `.so` itself is stripped, so the DWARF version cannot be read out of it; all
tested DWARF versions (`2`, `3`, `4`) produce identical code on this tree, and clang's
default (and ndk-build's) is DWARF 4.

## 6. Reproduction commands for the key evidence

```sh
# baseline
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8

# representative function bytes (original vs -g0 vs -g2)
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -D -S --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so | grep ' InitializeBlowfish$'
$NM -D -S --defined-only .w3564-scratch/build-g0/libgof2hdaa.so | grep ' InitializeBlowfish$'
$NM -D -S --defined-only .w3564-scratch/build-g2/libgof2hdaa.so | grep ' InitializeBlowfish$'

# disassembly around the copy/compare
OBJ=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OBJ -d --start-address=0x6b3b0 --stop-address=0x6b3e0 ../_work/bins/android_2.0.16_libgof2hdaa.so
llvm-objdump -d --disassemble-symbols=InitializeBlowfish \
  .w3564-scratch/build-g0/CMakeFiles/gof2.dir/src/engine/file/BlowfishSubkeyFile.c.obj
llvm-objdump -d --disassemble-symbols=InitializeBlowfish \
  .w3564-scratch/build-g2/CMakeFiles/gof2.dir/src/engine/file/BlowfishSubkeyFile.c.obj

# machine MIR after PostRA Machine Sink
# (insert `-mllvm -print-machineinstrs` into the exact compile command from
#  cmake-build-match/compile_commands.json, then grep the InitializeBlowfish block)
```

No `--update-baseline` was run. The scratch build directory was removed before the final
commit; the worktree contains only this note.
