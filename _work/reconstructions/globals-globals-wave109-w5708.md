# Globals::Globals C1/C2 (80.5%, 612 B) — w5708 (wave 109)

Status: research note, 2026-11-04. Worktree `wt/w5708`, branch `wave109-w5708`,
base `7c34c4098`. **No source change landed** — the checked-in constructor is still the
best fuzzy shape (80.5%). This commit contains only this note.

## TL;DR

* Baseline and final: **`_ZN7GlobalsC1Ev` / `_ZN7GlobalsC2Ev` = 80.5%, 612 B**
  (size-exact; M = 182, `2·182/452 = 0.8053097345132744`). Pool LCS **30/31**
  (`status` at slot 6, `recordHandler` at slot 7); `movw #583` at normalised index 139
  (original: 34); two trailing `nop`s (real code 468 B vs the original's 472 B).
* This pass tested the wave-108 hint ("try a different declaration/initialization
  mechanism: helper function, static initializer order, different literal form") and
  re-ran the order searches with a fresh scorer. **Nothing beat 80.5.**
* New this pass: a **full single-swap hill climb over all 33 non-settings statements**
  (early pointer block + late block) and 1 500-2 000-sample random permutations of the
  settings block, the settings prefix, the late block, and the Q-at-29 late block.
  Every search converges back to the checked-in order.
* New this pass: `nops 0` (i.e. the original's 472 B of real code, with the
  `imageFactory` 32-bit `ldr.w ip` / `str.w ip` pair) is reachable in **93 of 1 500**
  random late-block orders, but the best such order scores only **66.2%** (pool 20/31).
  The 4-byte widening and the 30/31 pool remain mutually exclusive.

## 1. Commands

```bash
cd /home/debian/GalaxyOnFire2/wt/w5708
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
```

Gate at baseline (re-run this pass, unchanged tree):

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Fast per-variant scorer (scratch, `/dev/shm/w5708`, not tracked): `build_one.sh` is the
exact repo compile line for `Globals.cpp` plus a plain gold link into `mini.so`;
`score.py` reproduces the official `verify.py` fuzzy number, M, pool LCS, `movw` index
and `size` from `mini.so`. `gen2.py` rebuilds the constructor body from an ordered token
list (37 tokens: 14 early pointer stores + `tintR` + `this+0` + `SETTINGS` (25 stores) +
19 late stores + `Q` (9 lines)); `tryorder2.py "<comma,order>"` builds and scores one
order. `gen3..gen16.py` hold the alternative Q-block / helper / type / member-init forms.

## 2. Ground truth (unchanged from wave 108)

`_ZN7GlobalsC1Ev` @ `0xe31e0`, 612 B = 472 B code + 16 B float pool
(`0x3f000000, 0x3f000000, 0x3f19999a, 0x3f19999a`) + 124 B (31-word) GOT pool @ `0xe33c8`.

Original GOT pool order (31 words):

```text
Canvas, lastSpace, lastStation, lcmf, lcmfc, options, recordHandler,
shipTemplate, gameText, font, fontAlien, fontLangSelect, rnd, globals,
layout, appManager, generator, hints, gameLoaded, gameSaving, initMem,
imageFactory, achievements, galaxy, ias, iaw, status, iat, iapn, items, ships
```

Baseline pool is identical except `status` at slot 6 (original: `recordHandler`).
The original keeps `&imageFactory` in `ip` from index 128 to 181 (32-bit `ldr.w ip` /
`str.w ip`, 8 B); ours uses the 16-bit `ldr r0` / `str r2` pair (4 B), hence 468 B + 2
`nop`s. That 4-byte shift changes every `[pc,#N]` immediate in the second half and is
the dominant residual mismatch.

## 3. Measured variants this pass (official fuzzy %)

| variant | score | M | pool | movw@ | notes |
|---|---:|---:|---:|---:|---|
| baseline HEAD | **80.5** | 182 | 30/31 | 139 | best |
| `QDIRECT` (direct `settings->resWidth = …` stores) | 80.5 | 182 | 30/31 | 139 | byte-identical to baseline |
| `QDIRECT2` (decimal literals, `>=` form) | 80.5 | 182 | 30/31 | 139 | byte-identical |
| `QHELP` (`static inline int qualityWidth/Height(float)`) | 79.2 | 179 | 30/31 | 141 | |
| `QHELPVOID` (`static void setResolution(GameSettingsRecord*)`) | 80.5 | 182 | 30/31 | 139 | inlined, byte-identical |
| `HELPEARLY` (`static void resetGlobalsPointers()`) | 80.5 | 182 | 30/31 | 139 | inlined, byte-identical |
| `QTERN` (`?:` nested) | 72.1 | 163 | 30/31 | 139 | |
| `QTERN2`/`QTERN3` (mixed `?:` + `if`) | 80.5 | 182 | 30/31 | 139 | byte-identical |
| `QMINMAX` (`if/else if`) | 37.6 | 85 | 21/31 | 116 | branches |
| `QSWAP` (resHeight block first) | 79.6 | 180 | 30/31 | 148 | |
| `int`/`unsigned`/`short`/`long` v54/v58 | 80.5 | 182 | 30/31 | 139 | all byte-identical |
| member-init list `: field_0x0(0), soundResources(0)` | 42.9 | 97 | 24/31 | 162 | |
| `bool options[100]` → `unsigned char options[100]` | 80.5 | 182 | 30/31 | 139 | byte-identical |
| `status = nullptr` / `(Status*)0` / `0L` | 80.5 | 182 | 30/31 | 139 | byte-identical |
| `fv` load hoisted before the settings block | 72.6 | 164 | 30/31 | 138 | |
| `fv` load at function top | 53.2 | 120 | 24/31 | 143 | |
| `lcmfc` position sweep (36 positions) | 80.5 | 182 | 30/31 | 139 | only baseline (last) |
| `recordHandler` position sweep (36 positions) | 80.5 | 182 | 30/31 | 139 | only baseline (2nd) |
| `status` position sweep (36 positions) | 80.5 | 182 | 30/31 | 139 | only baseline (13th) |
| `Q` position sweep (36 positions) | 80.5 | 182 | 30/31 | 139 | positions 17–20 identical |
| `Q` at late pos 29–31 | 71.7–73.0 | 162–165 | 23–24/31 | **34** | movw hoisted, pool wrecked |
| single-swap hill climb, all 33 non-settings statements | 80.5 | 182 | 30/31 | 139 | no improving swap |
| 2 000 random settings-tail permutations | 80.5 | 182 | 30/31 | 139 | best = baseline |
| 1 500 random settings-prefix permutations | 80.5 | 182 | 30/31 | 139 | best = baseline |
| 1 500 random late-block permutations | 75.7 | 171 | 23/31 | 139 | |
| 1 500 random Q-at-29 late-block permutations | 73.0 | 165 | 24/31 | **34** | |
| 1 500 random late-block, filter `nops 0` | 66.2 | 149 | 20/31 | 90 | 93/1500 have `nops 0` |
| compiler flags: `-Os` | 80.5 | 182 | 30/31 | 139 | identical to `-Oz` |
| compiler flags: `-O2` / `-O1` | 71.1 / 40.0 | 162 / 89 | 30 / 23 | 139 / 128 | |

## 4. Proven vs hypothesis

Proven (measured, reproducible):

1. The checked-in source order is a **local optimum under single swaps of every
   non-settings statement** and under 1 500–2 000-sample random permutations of the
   settings prefix, the settings tail, and the late block. No order tried beats 80.5.
2. The quality block's **source form is irrelevant**: local `int`/`unsigned`/`short`/
   `long` variables, direct field stores, decimal literals, `?:` chains, and a
   `static inline`/`static void` helper all compile to the **same bytes** as the
   checked-in form (80.5). Only the `if/else if` form (branches) and the fully nested
   `?:` form change the code, and both are worse.
3. `movw #583` at index 34 is reachable only by moving the whole Q block into the late
   block (positions 29–31, i.e. after `items`), and every such order drops the pool to
   ≤ 24/31 (best 73.0). It is not reachable together with pool 30/31.
4. `nops 0` (the original's 472 B of real code, `imageFactory` in `ip`) is reachable in
   93/1 500 random late-block orders, but the best of those scores 66.2% (pool 20/31).
   The 4-byte widening and the pool are mutually exclusive in every order tried.

Hypothesis (not yet proven):

* The remaining gap is a **SelectionDAG `list-hybrid` scheduling difference** that the
  source order cannot reach. The original's `movw r6,#583` is emitted at index 34
  because the pre-RA scheduler hoisted the constant there and the register allocator
  then picked `r6` (free after `ldr.w ip,[r6]`); in every source shape we can write,
  `list-hybrid` sinks the constant to the Q block and the allocator picks `r0`.
  The same scheduler difference sinks `lcmfc`'s store to the end in our build (keeping
  `ip` busy) while the original stores it at index 122 (freeing `ip` for
  `imageFactory`).
* Because the original `Globals.cpp` is an Android-2.0.x TU with no dump witness for the
  constructor body, the remaining gap may be irreducible from this TU alone without a
  source-level witness (the iOS 1.1.4/1.0.5 binaries are stripped and use a different
  version/compiler; the Windows `GOF2.exe` is stripped too).

## 5. Reproduction scripts (scratch, not tracked)

`/dev/shm/w5708/build_one.sh`:

```bash
ROOT=/home/debian/GalaxyOnFire2/wt/w5708
NDK=$ROOT/.cache/ndk/android-ndk-r18b
$NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++ \
  -target armv7-none-linux-androideabi16 -march=armv7-a -mthumb -mfpu=neon \
  -mfloat-abi=softfp -std=c++11 -fPIC -frtti -fstack-protector-strong -Oz -g \
  -stdlib=libc++ -isystem $NDK/sources/cxx-stl/llvm-libc++/include \
  -isystem $NDK/sources/cxx-stl/llvm-libc++abi/include \
  -isystem $NDK/sources/android/support/include --sysroot=$NDK/sysroot \
  -isystem $NDK/sysroot/usr/include \
  -isystem $NDK/sysroot/usr/include/arm-linux-androideabi \
  -D__ANDROID_API__=16 -DANDROID -DNDEBUG \
  -Wno-int-to-pointer-cast -Wno-int-to-void-pointer-cast \
  -I$ROOT/src -I$ROOT/third_party/fmod/inc -I$ROOT/third_party/gl \
  -I$ROOT/third_party/jni -I$ROOT/third_party/libzip -I$ROOT/third_party/crypto \
  -ffunction-sections -funwind-tables -c "$1" -o "$2/Globals.o"
$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-ld \
  -shared -soname libgof2hdaa.so --build-id -z now --exclude-libs=libgcc.a \
  -o "$2/mini.so" "$2/Globals.o"
```

`score.py` imports `tools/verify/verify.py`, normalises both functions with the same
resolver, and returns `{pct, M, total, pool, movw, movw_ours, size}`; it reproduces the
official verify number exactly on every variant above.
