# Wave 148 (w9515): `Status::missionCompleted` — per-TU profile derived from
# the ORIGINAL binary's TBH block order

Status: **tool + analysis, no `src/`/`CMakeLists.txt` change**.  Worktree
`wt/w9515`, base master `fece77a64` (wave 148).  Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507/4524`,
`linked_exact 4507/4524`, size 4524/4524, data 843/843, imports 207, lint
CLEAN, drift 0, symbol parity 5370/5370.

Target: `_ZN6Status16missionCompletedEbbx` — original `0xabf38`, **1432 B**.
Baseline: raw same-address diff **722**, instruction-index aligned diff **320**.

## 0. New result

1. **The harness now derives the case weights from the original's block
   order.**  `pgo_derive_w9515.py` reads the original's `TBH`/`TBB` tables
   (both the `150..189` and the `8..14` switch), excludes the values that map
   to the shared default target, sorts the remaining targets by address, and
   assigns `weight = rank` (first physical block = highest).  The derived
   order is exactly the source order
   `167,174,8,11,13,14,150,151,152,153,154,155,158,162,166,170,184,189,156,157,160,163,164,165,168,169,171,172`.
2. **A profile with `base=12`, `c28=28`, latch counters `6,7,12 = 0`
   reproduces BOTH original switch tables exactly** (the `150..189` table and
   the `8..14` table, default block last), at **1428 B** (4 B short of 1432).
   Raw **1366**, aligned **657**.
3. **`base=2`, `c28=0` gives exactly 1432 B**, and the `150..189` real cases
   are in source order — but the default block moves to the *front* and the
   `8..14` table is `9,10,12,8,13,14,11`.  Raw **1385**, aligned **863**.
4. **The rest of the TU is byte-exact**: 145/145 other `.text.*` sections of
   `Status.cpp.obj` are identical to the canonical object (same as w9415).
   Only the target section is renamed `.text.hot.` (the profile marks the
   function hot).
5. **The remaining residual is the entry block, not the switch.**  The
   canonical/original entry computes `shortMission`/`longMission` eagerly
   (`rsbs/sbcs/it lt` at `abf4c..abf80`); every profiled build sinks them
   (`movw sl,#2001` / `movw r9,#10001` and a deferred compare).  This is why
   the aligned diff *rises* (657/863 vs 320) even when the case order is
   exact: the profile attaches `!prof` to the entry `extra > 2000`/`> 10000`
   branches too, and clang 7 places them differently.  A full per-TU profile
   therefore cannot be a source/build win here; the w9305 switch-only
   `!prof` injection remains the only 1432 B shape.

## 1. Exact profile

`FuncHash 2003737435339834133`, `NumCounters 116`, `base=12`, `c28=28`,
latch `6,7,12 = 0` (both tables exact, 1428 B):

```
12,12,12,12,12,12,0,0,12,12,12,12,0,27,12,12,25,12,12,12,12,12,12,12,12,12,12,12,28,12,12,12,23,12,12,22,12,12,12,21,12,20,12,19,12,12,12,18,12,17,12,16,12,15,12,12,12,12,12,12,12,14,12,12,12,12,12,13,12,12,12,12,12,12,11,12,12,12,12,10,12,12,12,12,9,12,12,12,8,12,12,12,7,12,12,12,6,12,12,12,5,12,4,12,12,3,12,2,12,1,12,12,0,12,12,12
```

`base=2`, `c28=0` (1432 B, default first):

```
2,2,2,2,2,2,0,0,2,2,2,2,0,27,2,2,25,2,2,2,2,2,2,2,2,2,2,2,0,2,2,2,23,2,2,22,2,2,2,21,2,20,2,19,2,2,2,18,2,17,2,16,2,15,2,2,2,2,2,2,2,14,2,2,2,2,2,13,2,2,2,2,12,2,11,2,2,2,2,10,2,2,2,2,9,2,2,2,8,2,2,2,7,2,2,2,6,2,2,2,5,2,4,2,2,3,2,2,2,1,2,2,0,2,2,2
```

Case counters (from the instrumented IR): `13`=167/174, `16`=8, `28`=0/11,
`32`=13, `35`=14, `39`=150, `41`=151, `43`=152, `47`=153, `49`=154, `51`=155,
`53`=158, `61`=162, `67`=166, `72`=170, `74`=184, `79`=189, `84`=156,
`88`=157, `92`=160, `96`=163, `100`=164, `102`=165, `105`=168, `107`=169,
`109`=171, `112`=172.  Counters `6,7,12` are the loop-latch phi
(`%695`); setting them to 0 is what moves the default block to the end.

## 2. Measured variants

| variant | raw | size | 150 table | 8 table | aligned |
|---|---:|---:|---|---|---:|
| master (no profile) | **722** | **1432** | numeric | **exact** | **320** |
| `base=2, c28=0` | 1385 | **1432** | source, default first | 9,10,12,8,13,14,11 | 863 |
| `base=12, c28=28` | 1366 | 1428 | **exact** | **exact** | 657 |
| `base=11, c28=28` | 1366 | 1428 | **exact** | **exact** | ~657 |
| `base=16, c28=32` | 1367 | 1424 | **exact** | **exact** | 669 |

The master's `8..14` table is already exact; the only mismatching table is
`150..189` (numeric vs source).  The profile fixes it but changes the entry
codegen, so neither metric improves.

## 3. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9515
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6

# both TBH tables exact, 1428 B:
python3 _work/reconstructions/pgo_derive_w9515.py --base 12 --c28 28
# 1432 B, 150-table source order (default first):
python3 _work/reconstructions/pgo_derive_w9515.py --base 2 --c28 0
# dump the exact counter vector:
python3 _work/reconstructions/pgo_derive_w9515.py --base 12 --c28 28 --dump
```

The harness uses `ninja -t commands` for the exact TU compile line, the NDK
r18b `llvm-profdata`, the checked-in gold link command, and reports raw diff,
both TBH tables, and the other-`.text`-section identity.

## 4. Open items

1. The entry-block codegen (eager `shortMission`/`longMission`) is the
   remaining gap.  It is lost as soon as `!prof` is attached to the entry
   comparisons.  Recovering it needs a profile that also carries the inlined
   `Array::size()` counters (the loop header block `%33` increments
   `@__profc__ZNK5ArrayIP7MissionE4sizeEv`, not a `missionCompleted` counter),
   i.e. a whole-program profile rather than a per-TU one.
2. `base=12, c28=28` is 4 B short of 1432; no `base`/`c28`/latch combination
   in the searched range gave 1432 with both tables exact.
