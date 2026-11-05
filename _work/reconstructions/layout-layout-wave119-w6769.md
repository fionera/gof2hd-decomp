# Layout::Layout C1/C2 — wave 119 / worker w6769

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (identical bodies, aliased at `0xd1028`),
original size **7072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave119-w6769`, base wave-119 harvest (`415673c43`).
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.

## Result — no landable source change; note-only commit

| metric | before (HEAD) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| unrounded `difflib` ratio | 48.624950 % | **48.624950 %** |
| `byte_exact` / `linked_exact` | 4505 / 4505 | **4505 / 4505** |
| `size_exact` / `data_exact` | 4524 / 843 | **4524 / 843** |
| imports / lint / drift | 207 / CLEAN / 0 | **207 / CLEAN / 0** |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
No source change landed, so the 27 section headers, file size 2,187,328 B,
`.text`, `.rodata`, `.ARM.extab` and `.ARM.exidx` are unchanged.

The current body is a strong local optimum. The best *fuzzy* variants found this
wave are real but land at 7056/7064 B, and any shrink of the ctor shifts every
downstream function and collapses `byte_exact` (measured 4505 → 3221 for −16 B),
so they cannot land without also recovering the missing bytes inside the ctor.

## 1. Current gap re-measured (post-w6750)

`pop {r4,r5,r6,r7,pc}` is the last hot instruction:

| body | `pop` | region after `pop` |
|---|---:|---:|
| original | `0xd2800` | **966 B** |
| HEAD | `0xd28bc` | **778 B** |
| delta | | **188 B** |

So the hot code is 188 B longer in HEAD and the pool/cold region is 188 B
shorter. The w6720 census (46 words) is stale: w6750's `field_0x9c` float arms
already added the second `100.0f`/`50.0f` pair. Current word census of the
function body:

* original has **50** unique 16-byte SLP constant blocks, HEAD **42** → the
  remaining 8 four-int SLP groups (32 pool words = 128 B) are still missing.
* HEAD has **two extra scalar float words** `82.0f`/`41.0f` (0x42a40000 /
  0x42240000) that the original does not have. They come from the local
  `int v` in the `field_0xa8` initialisation, whose ternary is float-typed
  (`61.0f * 1.40625f` HD arm) so clang materialises every int arm as a float
  constant. The original's `v` is int-typed (its `0xa8` store is a single
  `vcvt.f32.s32`/`vcvt.s32.f32` round-trip of an int value).
* HEAD has **one extra GOT-offset literal** (23 vs 22).
* original outlines **four** cold `else` tails (38+14+14+14 = 80 B:
  `0x130/0x134/v138`, `0x2c8`, `0x2d0`, `0x60`); HEAD outlines **one**
  different 28 B tail (`0x1ac`). Delta +52 B.

128 (SLP) + 8 (extra floats) + 4 (GOT) − 52 (cold) = 188 B, matching the
measured hot/pool split.

## 2. Best fuzzy variants (real `link_so` builds)

`score.py` = official `tools/verify` internals, unrounded `difflib`,
`autojunk=False`.

| variant | size | ratio % |
|---|---:|---:|
| HEAD (baseline) | 7072 | 48.6250 |
| **A** `field_0x138 = (int)(float)v138` (drop inner `(int)(float)`) | 7064 | **48.6443** |
| **B** A + `field_0x160` inner `(int)` removed (`(int)(float)(int_ternary)`) | 7056 | **48.6637** |
| A + `0x150` cast removal | 7056 | 48.6540 |
| A + `0x2c8` double round-trip (restores size) | 7072 | 48.6250 |
| A + `0x2d0` double round-trip (restores size) | 7072 | 48.6250 |
| A + `0x150` double round-trip (restores size) | 7072 | 48.6250 |
| A + `0x2c8` double + `0x160` inner removal | 7048 | 46.6174 |

The `0x138` change is the known w6750-family win: the original stores `0x138`
with a *single* `(int)(float)` round-trip, HEAD has a double one. Removing the
inner cast saves 8 B (one `vcvt` pair) and raises the ratio by +0.019.
`0x160` is the same story: HEAD has three conversions
(`(int)(float)(int)(float_ternary)`), the original two.

Every attempt to restore the missing 8/16 B with a redundant round-trip
(`(int)(float)((int)(float)x)` on `0x2c8`, `0x2d0`, `0x150`, `0xa8`) lands at
exactly 7072 B but the added `vcvt` pair is unmatched, so the ratio falls back
to the baseline 48.6250.

## 3. Why 7072 B is a hard wall

`byte_exact` compares raw bytes of every function; the ctor is followed in
`.text` by ~2000 functions whose addresses shift if its size changes.
Measured: variant A (7064 B) → `byte_exact` **4505 → 3221**, `linked_exact`
4505 → 4505, `RATCHET FAIL`. A 7076-B variant behaves the same. The ratchet
requires `byte_exact` to never decrease, so the ctor must stay exactly 7072 B.

The only non-byte-exact functions in the whole image are the 19 in
`report.json`; the ones before `Layout` are far away (`Generator::createMission`
at `0x971b8` … `Level::createShip` at `0xc0214`) with thousands of byte-exact
functions between, so growing one of them to compensate is not viable either.

## 4. Searches run this wave (all real builds)

* **Random single-statement moves seeded from variant A** (60 builds, seed 11):
  no 7072-B result above 48.6443; the only 7072 hit scored 41.2.
* **All 210 float-field → float-field moves** (`0x8c,0x90,0xdc,
  animSpeedScale,driftSpeedDivisor,ringRadiusScale,0x188,0x18c,0x1a8,0x174,
  0x168,0x164,0x1d4,0x2f8,eventBannerScale`): only six keep 7072 B, all below
  baseline (best `field_0x18c` before `field_0x1a8` = 47.6987).
* **The same six moves re-run on top of variant A**: sizes 6960–7144, none
  above 48.6250.
* **`(int)(float)` wraps on `0x158,0x15c,0x170,0x1e8,0x13c,0x14c,0x178,
  0x16c,0x17c,0xac`**: clang 7.0.2 folds all of them back to the int ternary —
  byte-identical code (7072 / 48.6250).
* **Local-variable rewrites** of `0x158/0x15c/0x170` (if/else chain + `int v`,
  or float temp + `(int)(float)v`): also folded back, identical code. This
  reproduces the w4221 §4.2 finding: the conversion only survives when the
  converted value is **opaque** (a memory load), not a constant select/phi.
* **`rewardBoxY2` `(int)(float)((rewardBoxY + 17) << retina)`** (the one proven
  opaque round-trip): 7088 / 47.2416; combined with A → 7080 / 47.2604; with A
  plus one extra-`vstr` field made int-typed (`0x148`, `0x1f4`, `0x204`) →
  7072 / 47.11–47.35. All below baseline.
* **`0x1e8` boolean forms** (`||`, `!(...)`, `(int)(...)`): identical code or
  7076 / 40.2.
* **`v` / `field_0xac` int-typed arms** (removes the extra `82.0f`/`41.0f`
  words): 7032–7072, ratio 48.41–48.63.

## 5. Conclusion / next step

The remaining 188 B is structural: 8 SLP groups + 4 outlined cold tails +
int-typed ternaries. The w4221 ranked plan still holds — recover the HD-block
source order and reproduce ≥3 of the missing `vstr` round-trips on **opaque**
values (member loads), so that the body grows back to 7072 B *inside the ctor*
while the fuzzy alignment improves. A pure cast/order edit cannot both improve
the ratio and preserve the size: every 7072-B variant measured this wave is
≤ 48.6250.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6769
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6     # RATCHET PASS, byte 4505

OBJ=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OBJ -d --start-address=0xd1028 --stop-address=0xd2bc8 \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so > /dev/shm/w6769/orig.dis
$OBJ -d --start-address=0xd1028 --stop-address=0xd2bc8 \
  cmake-build-match/libgof2hdaa.so > /dev/shm/w6769/ours.dis
```

Scratch harnesses (not committed): `/dev/shm/w6769/{score.py,seq.py,regions.py,
trace2.py,vcvtmap.py,cmp_pool.py,try.py,wrap.py,localize.py,move.py,
movebefore.py,movetop.py,reorder_units.py,search138.py,sweepfloat.py}` plus
`Layout.base.cpp` (pristine HEAD) and `orig.dis` / `ours.base.dis`.
