# Globals::Globals C1/C2 (80.5%, 612 B) — wf208 (wave 102)

Status: research note, 2026-11-04. Worktree `wt/wf208`, branch `wave102-wf208`,
base `53d9ee4d5`. **No source change landed** — the w3808..w5106 baseline source
order is still the best measured shape (80.5%). This note is self-contained.

## TL;DR

* Baseline and final: **`_ZN7GlobalsC1Ev` / `_ZN7GlobalsC2Ev` = 80.5%, 612 B**
  (size-exact; 225 vs 227 normalised entries, M = 182,
  `2·182/452 = 0.8053097345132744`).
* Gate unchanged: **byte_exact 4500 / linked_exact 4500 / size-exact 4524,
  stubs 0, missing 0, wrong_type 0, extra 0, data 843/843, imports 207,
  lint CLEAN, drift 0, `RATCHET PASS`**.
* The remaining deltas are the same three w5106 identified: the
  `movw r6,#583` hoist, the two Thumb-16→Thumb-32 promotions for
  `&Globals::imageFactory`, and the late-block store schedule.
* This pass adds a **compiler-pipeline proof** of *why* the hoist is not a
  source-order property: the `MOVi16 583` is emitted early by ISel in a
  source shape that declares `v54` early, but the **Greedy register allocator
  rematerialises it at the store**; disabling the pre-RA scheduler
  (`-mllvm -enable-misched=false`) does not change this. The hoist basin is
  therefore an RA/remat outcome, not a source-order outcome.
* A 191-evaluation greedy swap/move hill-climb of the 19-token late block
  starting from the hoist basin returns to the baseline order (80.5) and
  nothing above it. Combined with w5106's SA and w5008's 20 000 three-move
  permutations, 80.5 is a basin optimum of the fuzzy metric.

## 1. Baseline and gate

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.91
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

```text
Globals::Globals()  orig 612B @0xe31e0 | ours 612B @0xe31e0
(length differs: orig 225 vs ours 227 insns)
```

The two extra normalised entries are the two `nop`s that pad our 4-byte-shorter
code body up to the 4-byte-aligned literal pool.

## 2. Ground truth re-derived this pass

`_ZN7GlobalsC1Ev` @ `0xe31e0`, 612 B. 186 code instructions, then 4 float
words at `0xe33b8` (`0.5, 0.5, 0.6, 0.6`), then the 31-word GOT pool at
`0xe33c8`.

**Original GOT-load order** (resolved through the linked `.so`'s `.rel.dyn`,
not the object's relocation order):

```text
Canvas, lastSpace, lastStation, lcmf, lcmfc, options, recordHandler,
shipTemplate, gameText, font, fontAlien, fontLangSelect, rnd, globals,
layout, appManager, generator, hints, gameLoaded, gameSaving,
initMemoryWarning, imageFactory, achievements, galaxy, instantActionScore,
instantActionWave, status, instantActionType, instantActionPlayerName,
items, ships
```

**Original store order** (same resolution):

```text
Canvas, shipTemplate, gameText, font, fontAlien, fontLangSelect, rnd,
globals, layout, appManager, recordHandler, generator, this+0,
settings block (incl. hints:0x4, hints:0x13, hints:0x0),
gameLoaded, gameSaving, lastSpace, initMem, lastStation, lcmfc, lcmf,
resWidth, resHeight, ias, iaw, iat, iapn, items, status, short60,
galaxy, achievements, imageFactory, ships, soundRes
```

**Our baseline GOT-load order** differs in exactly two places: slot 6 is
`status` (orig `recordHandler`), slot 16 is `recordHandler` (orig `generator`),
and `status` is at slot 29 (orig slot 26). The baseline pool matches the
original 38/39 entries; the one miss is the moved `status`.

**Original quality block** (indices 121–146) is two independent if-converted
`if`s in source order (ge then ls), with `v54` in `r6` and `v58` in `r0`:

```text
134 vcmpe.f32 s2,s0       ; fv vs 1.0
140 vcmpe.f32 s2,#0.0     ; fv vs 0.0
148 movw r0,#513          ; v58 default
14e it ge / 150 movwge r6,#830
158 vcmpe.f32 s2,s0
15c it ls / 15e movwls r6,#415
166 vcmpe.f32 s2,#0.0
16a str r6,[r1,#84]       ; resWidth
16c it ge / 16e movwge r0,#730
176 it ls / 178 movwls r0,#365
17e str r0,[r1,#88]       ; resHeight
```

Our baseline quality block is `ls` then `ge`, both values in `r0`, defaults
materialised at the use. The original's default `movw r6,#583` is at index 34
(offset `0x56`), immediately after `ldr.w ip,[r6]` (the last use of `r6`).

## 3. New measurements this pass

All variants compiled with the exact repo compile line (NDK r18b clang 7.0.2,
`-target armv7-none-linux-androideabi16 -march=armv7-a -mthumb -mfpu=neon
-mfloat-abi=softfp -std=c++11 -fPIC -frtti -fstack-protector-strong -Oz -g
-stdlib=libc++`) and scored with the official `tools/verify/verify.py`
normalisation. `movw` offset is the linked `.so` offset of `movw r?, #583`.

### 3.1 Declaration/expression position does not move the `MOVi16`

| variant | pct | `movw #583` |
|---|---:|---:|
| baseline | **80.5** | `0x152` |
| `int v54 = 0x247;` moved to top of body | 80.5 | `0x152` |
| both defaults moved to top | 80.5 | `0x152` |
| `fv` + `v54` computation moved to top | 52.3 | `0x15c` |
| `fv` + both computations moved to top | 52.3 | `0x15c` |
| direct `settings->resWidth = ...` stores | 80.5 | `0x152` |
| `unsigned`/`long`/`short`/`int32_t`/`int16_t` | 80.5 | `0x152` |
| `const float fv` / `double fv` | 80.5 | `0x152` |
| `static const int` named constants | 80.5 | `0x152` |
| `__builtin_expect` (both directions) | 80.5 | `0x152` |
| `bool high/low` + two ifs | 80.5 | `0x152` |
| `bool high/low` + ternary | 72.1 | `0x152` |
| ternary (high-first or low-first) | 72.1 / 80.5 | `0x152` |
| `if/else if/else` | 72.1 | `0x152` |
| helper `qualityRes(fv, lo, mid, hi)` | 72.1 | `0x152` |
| direct `settings->qualityLevel` in conditions | 79.2 | `0x152` |

### 3.2 Compiler-pipeline proof

Compiling the baseline with `-mllvm -print-after-all`:

* the optimised IR has `store i32 %7, ... offset 84` (resWidth) at the source
  position, after `gameSaving`;
* the ISel/`Expand ISel Pseudo-instructions` order is the same;
* the `Greedy Register Allocator` output has `%66 = t2MOVi16 583` immediately
  before the `MOVCC` chain, i.e. at the store;
* `-mllvm -enable-misched=false` leaves the `movw` at `0x152`.

In the early-declaration variant (`int v54` before `shipTemplate`), the
optimised IR has the `select` **before** the `shipTemplate` store, and the
ISel order has `t2MOVi16 583` before the store too; the Greedy RA still sinks
it to the store. So the hoist is a register-allocator rematerialisation
decision, not a source-order/ISel decision.

### 3.3 Hoist basin

Moving the whole quality block to late positions (the w5106 `q` sweep) does
reproduce the `movw` at `0x56`:

| quality block inserted before | pct | `movw #583` |
|---|---:|---:|
| `ships` | 73.0 | **`0x56`** |
| `instantActionWave` | 71.7 | **`0x56`** |
| `lastCampaignMissionFailed` | 72.1 | **`0x56`** |
| `items` | 77.9 | `0x162` |
| `initMemoryWarning` | 79.2 | `0x162` |
| baseline (after `gameSaving`) | **80.5** | `0x152` |

The hoist basin's quality block uses `r6` for **both** `v54` and `v58`; the
original uses `r6` for `v54` and `r0` for `v58` (because the original's
`galaxy` address load reuses `r6` right after `resWidth`). Moving `galaxy`
after the quality block raises the basin to 77.4 but loses the `0x56` hoist
(`movw` at `0x162`).

### 3.4 Late-block hill-climb

A 191-evaluation greedy first-improvement hill-climb over the 19-token late
block (swap moves + move-`q` moves), starting from the hoist basin
(`q` before `ships`), immediately returns to the baseline order:

```text
q, lastSpace, gameLoaded, gameSaving, initMem, imageFactory, achievements,
galaxy, ias, lastStation, iat, iapn, items, ships, iaw, lcmf, soundRes,
short60, lcmfc
```

= **80.5**, `movw` at `0x152`. No swap/move above 80.5 was found.

### 3.5 Early-block reorderings

| variant | pct | size |
|---|---:|---:|
| baseline | **80.5** | 612 |
| `recordHandler` after `appManager` | 79.2 | 612 |
| `recordHandler` after `generator` | 79.2 | 612 |
| `status` after `items` | 77.0 | 612 |
| `status` after `items` + `recordHandler` after `appManager` | 77.4 | 612 |
| `status` after `items` + `recordHandler` after `generator` | 78.3 | 612 |
| original early store order, `status` late | 76.5 | 612 |
| original early store order, `status` early | 71.1 | 600 |
| `secondary->slot0` after `flagAt3f` | 79.2 | 612 |
| `secondary->slot0` after `intAt48` | 78.8 | 612 |
| `secondary->slot0` after `resHeight` | 74.8 | 612 |

## 4. What is proven vs still hypothesis

**Proven (measured or read off the binary):**

1. The original's exact GOT-load order, store order and quality-block schedule
   (section 2), re-derived from the linked `.so`'s `.rel.dyn`.
2. The baseline scores 80.5 with early 95/96, late 49/90, pool 38/39; the
   single early miss is the `movw #583` position.
3. The `movw #583` hoist is reproducible from source only in the late-block
   basin at 71.7–73.0 (`q` before `ships`/`iaw`/`lcmf`); that basin uses `r6`
   for both resolution values and does not match the original's `r6`/`r0`
   split.
4. The hoist is an RA rematerialisation outcome: the optimised IR and ISel
   order can have the `MOVi16` early, but the Greedy RA sinks it to the store;
   disabling the pre-RA scheduler does not change the result.
5. No declaration/expression/type/`__builtin_expect`/helper variant tried here
   moves the `movw` off `0x152`.
6. The 191-eval late-block hill-climb from the hoist basin returns to the
   baseline order and 80.5; nothing above 80.5 was found.

**Hypothesis (not proven):**

1. The original's `movw r6,#583` hoist and the `r6`/`r0` split are one
   register-allocation outcome of a source shape not yet found; the two
   Thumb-16→Thumb-32 `imageFactory` promotions follow from the same split
   (`r0` occupied by the `v58` default forces `ip` for the imageFactory
   address).
2. A source shape that keeps the `v54` default live across the early block
   *and* lets the `v58` default reuse `r0` would recover both the hoist and
   the `ip` promotions; none of the ~40 shapes tried here does.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/wf208
cmake --build cmake-build-match --target check -j8        # RATCHET PASS
python3 tools/verify/verify.py \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN7GlobalsC1Ev
```

Scratch harness (not tracked, `/dev/shm/wf208`): `harness.py` (splice a ctor
body into the real source + official score), `harness2.py` (same + `movw`
offset), `annot4.py` (annotated disassembly with GOT/store resolution),
`gen2.py`/`late_search.py` (token-order body generator), `hc_late.py`
(191-eval late-block swap/move hill-climb), `allir.txt`/`allirD.txt`
(`-mllvm -print-after-all` dumps for the baseline and the early-declaration
variant).
