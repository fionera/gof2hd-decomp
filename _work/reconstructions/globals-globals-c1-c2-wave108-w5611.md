# Globals::Globals C1/C2 (80.5%, 612 B) — w5611 (wave 108)

Status: research note, 2026-11-04. Worktree `wt/w5611`, branch `wave108-w5611`,
base `8f98ec4d6`. **No source change landed** — the checked-in constructor is
still the best fuzzy shape (80.5%). This commit contains only this note.

## TL;DR

* Baseline and final: **`_ZN7GlobalsC1Ev` / `_ZN7GlobalsC2Ev` = 80.5%, 612 B**
  (size-exact; 225 vs 227 normalised entries, M = 182,
  `2·182/452 = 0.8053097345132744`). Pool LCS **30/31** (`status` at slot 6).
  `movw #583` at normalised index 139. Two trailing `nop`s (code is 4 B short).
* This pass re-confirmed the baseline is a local optimum and added two new
  exhaustive searches (imageFactory×lcmfc position grid, quality-block position
  grid). **Nothing beat 80.5.**
* New: `movw #583` at index **34** *is* reachable, but only with a worse pool:
  - Q-block at baseline late position 29: score **73.0**, pool 24, movw@34.
  - Q-block at B6 late position 15: score **58.2**, pool 20, movw@34, **nops 0**.
  The `movw@34` and pool-31/31 goals remain mutually exclusive in every order
  tried (now ~5 000 additional compiles this wave).
* New: `nops 0` (i.e. 472 B of real code, matching the original's size) is
  reachable, but only together with a bad pool (≤25/31). The 4-byte shortfall
  is the `imageFactory` 16-bit `ldr r0`/`str r2` pair; `lcmfc` already gets the
  32-bit `ip` pair in the baseline.

## 1. Commands

```bash
cd /home/debian/GalaxyOnFire2/wt/w5611
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
```

Gate at baseline (re-run this pass):

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.91
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Fast per-variant scorer (scratch, `/dev/shm/w5611`, not tracked): `build_one.sh`
is the exact repo compile line for `Globals.cpp` plus a plain gold link into
`mini.so`; `harness.py` reproduces the official `verify.py` fuzzy number, M,
pool LCS, `movw` index and `nop` count from `mini.so`. `evalorder.py` rebuilds
the constructor body from an ordered token list (tokens: `settings`, the 14
early pointer stores, `tintR`, `this0`, `SETTINGS` (the whole settings block),
the 19 late stores, `Q` (the 9-line resolution block)); `search.py` is the same
evaluator parallelised over 6 processes.

## 2. Ground truth (re-derived from the shipped `.so` this pass)

`_ZN7GlobalsC1Ev` @ `0xe31e0`, 612 B = 472 B code + 16 B float pool
(`0x3f000000, 0x3f000000, 0x3f19999a, 0x3f19999a`) + 124 B (31-word) GOT pool
@ `0xe33c8`.

### 2.1 Original GOT pool order (31 words)

```text
Canvas, lastSpace, lastStation, lcmf, lcmfc, options, recordHandler,
shipTemplate, gameText, font, fontAlien, fontLangSelect, rnd, globals,
layout, appManager, generator, hints, gameLoaded, gameSaving, initMem,
imageFactory, achievements, galaxy, ias, iaw, status, iat, iapn, items, ships
```

The first 7 are the prologue hoist (`ldr rX,[pc,#N]` + `add rX,pc` at
`e31e8..e3202`); the remaining 24 are materialised inline, in store order.

### 2.2 Original machine store order (56 stores)

```text
Canvas, shipTemplate, gameText, font, fontAlien, fontLangSelect, rnd,
globals, layout, appManager, recordHandler, generator, this+0,
opt:0x14 (vst1: 0.5,0.5,0.6,0.6), opt:0x0c (0x0101),
opt:0x00/0x04 (strd 0.5,0.5), opt:0x08 (0.5),
opt:0x24/0x28 (strd 0.5,1.0), opt:0x2c (0.5), opt:0x0e (1), opt:0x30 (1),
opt:0x10, opt:0x40, opt:0x50, opt:0x39, opt:0x35, opt:0x31,
hints:0x04, hints:0x13, opt:0x3f, opt:0x3b, opt:0x4e, opt:0x4c, opt:0x48,
hints:0x00,
gameLoaded, gameSaving, lastSpace(-1), initMem, lastStation(-1),
lcmfc(0), lcmf(-1), opt:0x54 (resWidth), opt:0x58 (resHeight),
ias, iaw, iat(-1), iapn, items, status, opt:0x60 (0x0100),
galaxy, achievements, imageFactory, ships, this+4 (soundResources)
```

The original uses `ip` for **two** long live ranges: `&lcmfc` from index 33 to
122 (`ldr.w ip,[r6]` / `str.w r2,[ip]`, 4+4 B), then reloads `ip` with
`&imageFactory` at index 128 and stores at 181 (`ldr.w ip,[r0]` /
`str.w r2,[ip]`, 4+4 B). The baseline reproduces the `lcmfc` `ip` live range
but not the `imageFactory` one, hence the 4-byte shortfall and two `nop`s.

### 2.3 Baseline (HEAD) machine store order

```text
Canvas, recordHandler, shipTemplate, gameText, font, fontAlien,
fontLangSelect, rnd, globals, layout, appManager, status, generator,
this+0, [settings block], lastSpace, gameLoaded, gameSaving, initMem,
imageFactory, achievements, galaxy, lastStation, ias, [Q], iat, iapn,
items, iaw, lcmf, ships, sound, short60, lcmfc
```

`movw #583` is at index 139; `imageFactory` uses the 16-bit
`ldr r0`/`str r2` pair; `lcmfc` uses `ip` and is the last store.

## 3. Measured variants this pass (official fuzzy %)

| variant | score | M | pool | movw@ | nops | notes |
|---|---:|---:|---:|---:|---:|---|
| baseline HEAD | **80.5** | 182 | 30/31 | 139 | 2 | best |
| `imageFactory` position sweep (36 positions) | 80.5 | 182 | 30 | 139 | 2 | best at pos 22 = baseline; no position gives nops 0 |
| `imageFactory`×`lcmfc` 2-D sweep (1 296 compiles) | 80.5 | 182 | 30 | 139 | 2 | best is baseline; no pair gives nops 0 |
| `status` position sweep (36 positions) | 80.5 | 182 | 30 | 139 | 2 | only baseline position (s14) |
| `recordHandler` position sweep (36 positions) | 80.5 | 182 | 30 | 139 | 2 | only baseline position (r2) |
| Q-block position sweep on baseline (36 positions) | 80.5 | 182 | 30 | 139 | 2 | q17–q20 identical to baseline |
| Q at baseline late pos 29 | 73.0 | 165 | 24 | **34** | 2 | movw hoisted, pool wrecked |
| Q at B6 late pos 15 | 58.2 | 131 | 20 | **34** | **0** | movw hoisted *and* 472 B real code |
| B6 early + original late order, Q sweep (20 positions) | 66.8 | 151 | 25 | 158 | 2 | scheduler moves ias/iaw/iat before Q |
| random late-block shuffles, Q fixed @15 (1 200) | 73.5 | 166 | 23 | 135 | 2 | B6 early |
| random late-block shuffles, Q fixed @29 (1 500) | 75.7 | 171 | 21 | 139 | 2 | baseline early |
| original settings-tail order | 79.2 | 179 | 30 | 137 | 2 | |
| `secondary->slot1`/`flagAt13` before `intAt3b` | 79.2 | 179 | 30 | 137 | 2 | |
| `else if` quality form | 37.6 | 85 | 21 | none | 1 | clang branches |
| quality block computed early, stores late | 52.3 | 118 | 22 | 143 | 1 | |
| `int v54/v58` declared at function top | 80.5 | 182 | 30 | 139 | 2 | clang still sinks `movw` |
| `int v54/v58` declared at top + baseline | 80.5 | 182 | 30 | 139 | 2 | identical bytes |

Previous-wave results (wave 106/107) still hold: full single-token move sweep
over all 66 statements, settings-tail hill climb, late-block swap+insert hill
climb, 6 000 late-block and 3 000 early-block random permutations, `-mllvm`
scheduler toggles, `-O1/-O2/-O3/-Os`, and all `status`/`imageFactory` init
forms — none beat 80.5.

## 4. Proven vs hypothesis

Proven (measured, reproducible):

1. The baseline source order is a local optimum of the statement-order space:
   every single-token move of `status`, `recordHandler`, `imageFactory`,
   `tintR`, `lastSpace`, `flagAt3f` and the settings tail is ≤ 80.5.
2. `movw #583` at index 34 is reachable, but every shape that reaches it has
   pool LCS ≤ 25/31 (best score 73.0). It is not reachable together with
   pool 31/31 in any of the ~5 000 orders tried.
3. `nops 0` (472 B real code) is reachable, but only with pool LCS ≤ 25/31.
   The 4-byte shortfall is the `imageFactory` `ldr`/`str` pair, not the `movw`.
4. The baseline already gets the 32-bit `ip` pair for `lcmfc` (live range
   34→183); the original additionally gets a second `ip` live range for
   `imageFactory` (128→181). No source form tried gives two long `ip` ranges.
5. `int v54 = 0x247;` is always sunk by clang's `-Oz` scheduler to the
   quality block (ISel emits `movw` at IR position ~117–126), regardless of
   declaration position. The original's early `movw r6,#583` is a scheduler
   hoist that the current source shape does not reproduce.

Hypothesis (not yet proven):

* The original source order is likely the *pool order* (Canvas, lastSpace,
  lastStation, lcmf, lcmfc, options, recordHandler, shipTemplate, …), and the
  original scheduler both hoisted those 7 loads and sank their stores. Writing
  that order in the current TU does not reproduce the hoist/sink pattern
  (score 50–53), so a different source *structure* (e.g. the settings block
  written through a different object/helper, or a different inlining context)
  is probably required, not another permutation.
* Because the original is an Android-2.0.x TU with no dump witness for the
  exact constructor body, the remaining gap may be irreducible from this TU
  alone without a source-level witness.

## 5. Reproduction scripts (scratch, not tracked)

`/dev/shm/w5611/build_one.sh`:

```bash
ROOT=/home/debian/GalaxyOnFire2/wt/w5611
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

`harness.py` imports `tools/verify/verify.py`, normalises both functions with
the same resolver, and returns
`{pct, M, pool, movw, nops, ln}`; it reproduces the official verify number
exactly on every variant above.
