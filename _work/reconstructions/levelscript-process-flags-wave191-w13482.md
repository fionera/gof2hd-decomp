# LevelScript::process — build-flag / codegen research (wave 191, w13482)

Status: **research note, no landed code or CMake change** (2026-11-12).
Worktree `wt/w13482`, branch `wave191-w13482`, base master `f325e2d87`.
Scratch `/home/debian/GalaxyOnFire2/scratch/w13482`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Gate on entry and exit: **RATCHET PASS**, `byte_exact 4507/4524`,
`linked_exact 4507/4524`, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, parity 5370/5370. `.text` **71,811** differing bytes,
`.ARM.extab` **575**, `.rel.plt` **0** (byte-identical, md5
`289b2a06…`), build-id 20.

## 0. Bottom line

1. **No size-exact flag is a raw win on `LevelScript::process`.** 2,525
   candidate cells (single flags, boolean bare/`=true`/`=false` forms,
   value sweeps, ARM subtarget features, and 246 two/three-flag
   combinations) were compiled; every cell that kept the function at
   57,232 B and left every other `.text.*` function byte-identical was
   link-measured. The best non-baseline raw is **51,682** (`+hwdiv`, +3);
   the best neutral cells reproduce the baseline exactly.
2. **The checked-in `LevelScript.cpp -mllvm -disable-postra-machine-sink`
   knob is now dead** on this base: removing it produces a byte-identical
   linked `.so` (raw 51,679, aligned 22,003, `.text` 71,811, `.ARM.extab`
   575). It can be removed as a cleanup, but it is not a measured win.
3. The closest **aligned** improvement is `-mllvm
   -available-load-scan-limit=16` (aligned 22,003 -> **21,628**, -375)
   but it costs +199 raw and +2 `.ARM.extab`, so it is not a
   byte-identicality win.
4. The residual is a body-shape difference, not a missing flag: the
   original has 20,830 instructions vs our 20,726, and the diff is
   dominated by `extra-orig:ldr` (1,972), `extra-orig:blx` (1,208),
   `extra-ours:mov` (1,174), `extra-ours:movs` (907) — i.e. a shifted
   instruction stream, not a single scheduling/regalloc decision.

## 1. Harness

Single-TU compile + exact gold link. The canonical compile command is
taken from `ninja -C cmake-build-match -t commands libgof2hdaa.so`; the
object is written to scratch and the canonical link command is re-run
with that object substituted (same gold line, same strip +
`reorder_shstrtab.py` post-processing). The original and candidate `.so`
are compared with `tools/fn_diff.py` (raw/aligned) and by section bytes
(whole `.text`, `.ARM.extab`, `.rel.plt`).

Baseline command (the `-mllvm -disable-postra-machine-sink` is the
checked-in per-TU knob):

```sh
$NDK/clang++ -target armv7-none-linux-androideabi16 -march=armv7-a -mthumb \
  -mfpu=neon -mfloat-abi=softfp -std=c++11 -fPIC -frtti \
  -fstack-protector-strong -Oz -g -stdlib=libc++ \
  <NDK libc++/libc++abi/android-support include paths> \
  --sysroot=$NDK/sysroot \
  <NDK sysroot include paths> \
  -D__ANDROID_API__=16 -DANDROID -DNDEBUG \
  -Wno-int-to-pointer-cast -Wno-int-to-void-pointer-cast \
  -I$SRC/src -I$SRC/third_party/fmod/inc -I$SRC/third_party/gl \
  -I$SRC/third_party/jni -I$SRC/third_party/libzip -I$SRC/third_party/crypto \
  -ffunction-sections -funwind-tables -mllvm -disable-postra-machine-sink \
  -o /home/debian/GalaxyOnFire2/scratch/w13482/base.o \
  -c $SRC/src/game/world/LevelScript.cpp
```

The exact `+hwdiv` candidate command is the same line with
`-Xclang -target-feature -Xclang +hwdiv` appended after the source file.
The link line is the canonical `ld.gold -shared -soname libgof2hdaa.so
--build-id -z now --exclude-libs=libgcc.a …` line from
`cmake-build-match/build.ninja` with the scratch object substituted for
`CMakeFiles/gof2.dir/./src/game/world/LevelScript.cpp.obj`, followed by
the same `arm-linux-androideabi-strip --strip-all` and
`tools/reorder_shstrtab.py`.

The object-level filter is strict: the candidate must keep
`.text._ZN11LevelScript7processEi` at exactly 57,232 B and leave every
other `.text.*` section byte-identical (debug sections ignored). Only
those cells are link-measured; a cell that moves another function would
drop `byte_exact` even if the target improves.

## 2. Coverage

| family | cells |
|---|---:|
| driver/language + `-fno-*` + FP/vectorizer/unroll toggles | 885 |
| ARM constant islands, literal pools, alignment, `-f*` driver forms | 332 |
| combinations of the closest knobs and ARM subtarget features | 246 |
| bare `-mllvm` boolean spellings + value sweeps (thresholds 0..1024) | 804 |
| two/three-flag combinations of size-changing knobs | 246 |
| canonical-flag removal / post-RA knob combinations | 12 |
| **total** | **2,525** |

Families covered include: `-misched` / `-enable-misched` /
`-enable-post-misched` / `-misched-postra` / `-post-RA-scheduler` /
`-pre-RA-sched`, all `-disable-sched-*` knobs, `-regalloc=basic|greedy|
fast|pbqp`, spill/coalescing knobs, `-enable-gvn-hoist/sink/newgvn`,
`-enable-load-pre`, `-enable-tail-merge`, `-enable-shrink-wrap`,
`-enable-machine-outliner`, `-enable-mergeicmps`, if-conversion knobs
(`-disable-ifcvt-*`, `-ifcvt-*`), tail-duplication knobs, loop/vectorizer
knobs, `-arm-constant-island-max-iteration`, `-arm-promote-constant*`,
`-arm-*` target features, and the `-Xclang -target-feature` ARM feature
set.

## 3. Link-measured size-exact cells

All rows keep `LevelScript::process` at 57,232 B and every other
`.text.*` function byte-identical. `.rel.plt` is byte-identical (md5
`289b2a06…`, 29,392 B) in every row.

| extra flags | raw | aligned | `.text` | `.ARM.extab` |
|---|---:|---:|---:|---:|
| **baseline** | **51,679** | **22,003** | **71,811** | **575** |
| `-mllvm -enable-earlycse-memssa=false` | 51,679 | 22,003 | 71,811 | 575 |
| `-mllvm -disable-ifcvt-triangle-rev` | 51,679 | 22,007 | 71,811 | 575 |
| `-Xclang -target-feature -Xclang +hwdiv` | 51,682 | 22,032 | 71,814 | 575 |
| `-mllvm -alias-set-saturation-threshold=1` | 51,743 | 22,003 | 71,875 | 575 |
| `-mllvm -disable-constant-hoisting=true -mllvm -memdep-block-scan-limit=12` | 51,816 | 22,021 | 71,948 | 574 |
| `-mllvm -available-load-scan-limit=16` | 51,878 | **21,628** | 72,010 | 577 |
| `-mllvm -jump-threading-threshold=8` | 52,329 | 21,931 | 72,461 | 575 |

The best raw is the baseline itself. `+hwdiv` is the only single flag
within 3 bytes; it does not change any other function, but +3 raw is not
a win.

## 4. Size-changing cells (not landable)

For reference, the closest size-changing cells (the function itself is
not 57,232 B, so the whole `.text` diff is dominated by the downstream
address shift and is not comparable):

| extra flags | raw | aligned | function size | `.text` | `.ARM.extab` |
|---|---:|---:|---:|---:|---:|
| `-mllvm -available-load-scan-limit=4` | 51,840 | 21,942 | 57,256 (+24) | 401,836 | 1,260 |
| `-mllvm -tail-dup-size=2` | 51,881 | 22,006 | 57,236 (+4) | 145,697 | 740 |
| `-mllvm -twoaddr-reschedule=false` | 52,128 | 22,008 | 57,228 (-4) | 136,523 | 574 |
| `-mllvm -disable-constant-hoisting=true` | 53,093 | 22,016 | 57,224 (-8) | 351,501 | 575 |
| `-fno-tree-slp-vectorize` / `-mllvm -slp-threshold=8` | 53,301 | 21,795 | 57,196 (-36) | 404,719 | 1,243 |

Even the best of these is +161 raw, so no compensation shape was pursued
further.

## 5. Negative results worth keeping

* `-mllvm -enable-earlycse-memssa=false` is exactly neutral on this TU
  (identical linked output). It is already landed for `Level.cpp`, but it
  buys nothing for `LevelScript.cpp`.
* `-mllvm -disable-ifcvt-triangle-rev` is raw-neutral (51,679) but moves
  aligned 22,003 -> 22,007; not a win.
* `-mllvm -disable-postra-machine-sink` removal is exactly neutral
  (identical linked output), so the current per-TU CMake knob is dead on
  the wave-191 base. Removing it is a cleanup, not a gate win.
* `-mllvm -disable-postra-machine-licm` changes the function to 57,236 B
  (+4) and raw 52,422; not size-exact.
* `-mllvm -available-load-scan-limit=*` is the only family that
  materially improves the aligned instruction stream (-375 at 16), but it
  costs +199 raw and +2 `.ARM.extab`.
* `-mllvm -jump-threading-threshold=*` (8..1024) is size-exact but raw
  +650; `-mllvm -alias-set-saturation-threshold=0|1` is size-exact but
  raw +64.
* `-mllvm -misched-postra`, `-mllvm -post-RA-scheduler=false`,
  `-mllvm -enable-aa-sched-mi`, `-mllvm -disable-post-ra` and
  `-mllvm -enable-selectiondag-sp=false` all change the byte-exact
  `LevelScript` constructor (`C2`), so they would drop `byte_exact` by 1
  even when the target improves.
* `-mllvm -enable-machine-outliner`, `-mllvm -enable-mergeicmps`,
  `-mllvm -enable-early-ifcvt` (with `=true/false`) and several bare
  `-fno-*` GCC spellings are rejected by clang 7.0.2.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13482
ulimit -v 12000000
python3 /home/debian/GalaxyOnFire2/scratch/w13482/harness.py base
python3 /home/debian/GalaxyOnFire2/scratch/w13482/sweep.py \
  /home/debian/GalaxyOnFire2/scratch/w13482/flags.json
python3 /home/debian/GalaxyOnFire2/scratch/w13482/link_cells.py
```

Scratch artifacts: `harness.py`, `sweep.py`, `gen_flags*.py`,
`flags*.json`, `obj_results_batch*.json`, `link_*.json` under
`/home/debian/GalaxyOnFire2/scratch/w13482`.

## 7. Files changed

* `_work/reconstructions/levelscript-process-flags-wave191-w13482.md` —
  this note. No `src/` or `CMakeLists.txt` change; tree clean.
