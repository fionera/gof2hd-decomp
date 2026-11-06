# Layout::Layout C1/C2 — wave 121 / worker w7101
## 241-word pool re-derivation, round-trip typing audit, and the 7072-B local optimum

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (identical bodies, aliased at
`0xd1028`), original size **7072 B**, source `src/game/ui/Layout.cpp`.
Worktree `/home/debian/GalaxyOnFire2/wt/w7101`, base = wave-120 master
`bd061a489`.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(sha256 `d4c17be6…bed9483`).

## Result — note only, no source change landed

| metric | before (base) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| unrounded `difflib` ratio | 48.963317384 % | **48.963317384 %** |
| `report.json` pct (1 dp) | 49.0 | 49.0 |
| `byte_exact` / `linked_exact` | 4505 / 4505 | **4505 / 4505** |
| `size_exact` / `data_exact` | 4524 / 843 | 4524 / 843 |
| imports / lint / drift | 207 / CLEAN / 0 | 207 / CLEAN / 0 |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`
(`verify: byte 4505 linked 4505 stubs 0 missing 0 extra 0 avg 99.94`,
`data 843/843`, `sodiff extra 0 missing 0`, `imports 207`).
`--target drift` → `0 field(s) across 0 class(es)`.
No source change landed, so the 27 section headers, `.text` (`0x1337c8`),
`.ARM.exidx` (`0x6f70`), `.ARM.extab` (`0x26298`) and `.rodata` are unchanged.
`tools/verify_baseline.json` was **not** touched.

## 1. Method / exact commands

```sh
cd /home/debian/GalaxyOnFire2/wt/w7101
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
cmake --build cmake-build-match --target drift -j6      # 0 drifted fields

OBJ=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OBJ -d --start-address=0xd1028 --stop-address=0xd2bd0 \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so > /dev/shm/w7101/orig.dis
$OBJ -d --start-address=0xd1028 --stop-address=0xd2bd0 \
  cmake-build-match/libgof2hdaa.so > /dev/shm/w7101/ours.dis
```

Scratch harnesses (not committed, `/dev/shm/w7101/`): `score1.py`
(full-link exact scorer, reproduces `7072 / 48.963317` bit-for-bit),
`fast.sh` + `fastscore.py` (single-TU compile + gold-link + strip, 0.3 s,
also reproduces `7072 / 48.963317`), `climb.py` / `climb2.py` / `climb3.py`
(random swap/move hill-climbs), `sweep_move.py` (exhaustive single-line
moves), `combo.py` / `combo2.py` (typed-edit combinations), `stores.py`
(store-order extractor), `split.py` (hot/tail difflib split), `v178.cpp`,
`v184.cpp`, `Layout.base.cpp`.

## 2. The 241-word post-`pop` region — re-verified

The last hot instruction is `pop {r4,r5,r6,r7,pc}`.  Everything after it is
the cold region (outlined tails + alignment pad + literal pool):

| | `pop` | region after `pop` | cold blocks | pad | end pool | pool words |
|---|---:|---:|---:|---:|---:|---:|
| **original** | `0xd2800` | **966 B** | `0xd2802..0xd2852` = **80 B** | 2 B | `0xd2854..0xd2bc8` = **884 B** | **221** |
| HEAD (base) | `0xd28bc` | **778 B** | `0xd28be..0xd28ce` = **16 B** | 2 B | `0xd28d0..0xd2bc8` = **760 B** | **190** |
| delta | | **+188 B** | **+64 B** | 0 | **+124 B** | **+31** |

`(0xd2bc8 - 0xd2854)/4 = 221`; `(0xd2bc8 - 0xd28d0)/4 = 190`.  The brief's
"241 words" is the whole 966-B post-`pop` region / 4 = 241.5.

End-pool composition (re-derived by tracking `add/addw/subw rX,pc,#imm`
and `ldr rX,[pc,#N]; add rX,pc`):

| | original | base |
|---|---:|---:|
| end-pool `vld1.64` groups | **50** (200 words) | **42** (168 words) |
| end-pool scalar floats | **14** | **14** |
| end-pool GOT deltas | **7** | **8** |
| total | **221** | **190** |

The 4-word group multiset diff is 31 original-only vs 23 base-only; 16
window-shift pairs (the seven HD and seven n9 `0x204..0x26c` runs plus two)
cancel, leaving **15 original-only / 7 base-only = net 8 groups = 32 words**.
The 31-word deficit decomposes exactly as
`+32 groups − 1 GOT + 2 (82f,41f) − 2 (0.325f,0.65f moved to a .text island)`.
The 15/7 unpaired groups are listed in
`layout-literal-pool-241-words-wave119-w6964.md` §3a/§3b and
`…w6974.md` §4a; I re-derived the same sets from the disassembly.

**New measurement this pass:** the raw end-pool word sequence
(`0xd2854..0xd2bc8` vs `0xd28d0..0xd2bc8`, 221 vs 190 words) has
`difflib` ratio **0.725061** and a multiset diff of 60 original-only /
29 base-only words.  A perfect pool match would therefore add only
≈0.03 to the whole-function ratio — the pool is *not* the dominant lever.

## 3. Hot body vs tail split

Splitting the reconciled instruction stream at `pop`:

| | original | base | ratio |
|---|---:|---:|---:|
| hot instructions (before `pop`) | **2069** | **2121** | **0.438663** |
| tail instructions (after `pop`) | **460** | **366** | **0.748184** |
| whole function | 2529 | 2487 | **0.489828** |

The base has **52 more hot instructions** and **94 fewer tail instructions**
than the original.  The original outlines four `iPad ? A : B` tails (80 B,
20 instructions) where the base outlines one (16 B, 4 instructions); the
remaining ~36 hot instructions are the extra scalar `str`/`strd` stores the
base emits because it vectorises 8 fewer SLP windows.  The fuzzy metric is
dominated by the hot-body alignment, not by the pool.

## 4. Round-trip (`vcvt.s32.f32` → `vstr`) typing audit — new

The original's int→float→int round-trip sites (`vmov s0,rN` /
`vcvt.f32.s32` / `vcvt.s32.f32` / `vstr`) are at:
`0x60, 0x68, 0x6c, 0x70, 0x2c8, 0x9c, 0xa8, 0x150, 0x158, 0x15c, 0x160,
0x168, 0x170, 0x1a8, 0x280, 0x3f4, 0x31c`.

The base's sites are:
`0x60, 0x68, 0x6c, 0x70, 0x2c8, 0x2d0, 0x9c, 0xa8, 0x150, 0x148, 0x160,
0x168, 0x1a8, 0x3e8, 0x1f4, 0x31c`.

Differences: the original round-trips **0x158, 0x15c, 0x170, 0x280, 0x3f4**
where the base does not; the base round-trips **0x148, 0x1f4** where the
original does not.  Every attempt to force the missing round-trips is a
codegen no-op because clang constant-folds `(int)(float)` when all ternary
arms are integer constants:

| variant | source change | size | ratio % |
|---|---|---:|---:|
| `rt_158/15c/170` | wrap the whole ternary in `(int)(float)(…)` | 7072 | 48.963317 (no-op) |
| `lrt_158/15c/170` | `int t = <ternary>; field = (int)(float)t;` | 7072 | 48.963317 (no-op) |
| `fi3_158/15c/170` | float arms + `(int)(float)(int)(…)` | 7072 | 48.963317 (no-op) |
| `fi_148` | `2 * 1.40625f` → `2` (drop a round-trip) | **7064** | 48.773679 |
| `fi_1f4` | `16 * 1.40625f` → `22` (drop a round-trip) | **7064** | 48.853440 |

The working round-trip shape is the one already used for `0x2c8`:
`int t = <int ternary>; field = (int)(float)t;` compiles to
`vmov s0,rN / vcvt.f32.s32 / vcvt.s32.f32 / vstr` only when the selected
value is already an int in a GPR (the `0x2c8` HD arm is
`(int)(45 * 1.40625f)`, which keeps the phi in a GPR).  With literal int
arms clang folds the conversion away.  The original's `0x158/0x15c/0x170`
round-trips therefore imply a source expression that is not a plain
constant ternary — most plausibly a shared float temporary or a
`(int)(float)` around a value clang cannot fold.  No shape found reproduces
it at 7072 B.

## 5. Measured variants (all real single-TU link+strip builds)

Scorer `fastscore.py` reproduces the full-link number bit-for-bit
(base `7072 / 48.963317`).

### 5a. Expression / type edits

| variant | source change | size | ratio % |
|---|---|---:|---:|
| **base** | — | **7072** | **48.963317** |
| `v124int` | `0x124` HD `1 * 1.40625f` → `1` | **7064** | **48.982848** |
| `fi_033` | `0x29c` HD `12 * 1.40625f` → `16` | 7080 | 48.934051 |
| `fi_054` | `0x204` HD `9 * 1.40625f` → `12` | 7080 | 48.635730 |
| `fi_059` | `0x1ac` HD `173 * 1.40625f` → `243` | 7088 | 47.859845 |
| `if_012` | `0x2d8` HD `141` → `141.0f` | 7088 | 48.914559 |
| `if_039` | `0x194` HD `8` → `8.0f` | 7088 | 48.914559 |
| `if_042` | `0x180` HD `23` → `23.0f` | 7088 | 48.565737 |
| `v138rt` | `0x138 = (int)(float)v138` | 7088 | 48.645418 |
| `bx_60/130/2c8` | `__builtin_expect(Globals::iPad,0)` on a cold arm | 7072 | 48.963317 (no-op) |
| `v178` | `0x178` retina/n9 arm swap | **7024** | **49.080000** |
| `v178+184` | `v178` + `0x184` iPad-before-n9 | 7024 | 48.820472 |
| base+`184` | `0x184` iPad-before-n9 (faithful) | 7072 | 48.704663 |

`v178` is the best raw score found (49.080 %), but it is **48 B short** of
7072.  A size change is not free: `docs/IDENTICALITY.md` states the
`Layout::Layout` body is size-exact 7072 B and `.text` is size-exact
(`0x1337c8`); shrinking the body would shift every later function and
break those invariants, so it is not landable under the "edit only the
owned TU" constraint.

### 5b. Combinations (all keep 7072 B unless noted)

| variant | size | ratio % |
|---|---:|---:|
| `v124int + fi_054` | **7072** | 48.655111 |
| `v124int + if_012` / `+ if_039` / `+ if_042` | 7080 | 48.93 / 48.93 / 48.90 |
| `v124int + v138rt` | 7080 | 48.983659 |
| `v124int + fi_033` | 7080 | 48.605578 |
| `v178 + if_012 + v138rt + fi_033 + fi_059` | **7072** | 47.377866 |
| `v178 + if_012 + v138rt + fi_033 + fi_059 + fi_054` | **7072** | 45.844130 |

No 7072-B combination beats the base.

### 5c. SLP re-grouping

The 8 missing groups are the retina/default arms of `0xb0..0xbc`,
`0x108..0x114`, `0xec..0xf8`, `0x13c..0x148`, `0x1d0..0x1dc`, `0x2a0..0x2ac`
(plus the n9 `0xec`/`0x140` and iPadLarge `0x180`).  In the base these
fields are scattered (e.g. `0xb0` line 71, `0xb8` 77, `0xb4` 78, `0xbc` 83);
in the original's iOS source order they are adjacent, which is why clang
vectorises them there.

| variant | source change | size | ratio % |
|---|---|---:|---:|
| `g_b0_before` | move `0xb8,0xb4,0xbc` next to `0xb0` | **7072** | 44.032676 |
| `g_b0_atb8` | move `0xb0` next to `0xb8` | 7080 | 44.869496 |
| `g_b0_before + fi_148` | grouping + drop a round-trip | 7056 | 43.917032 |

Grouping the windows reproduces the missing SLP groups but destroys the
hot-body alignment; the score falls by 5–9 points.

### 5d. Searches (all real builds)

| search | variants | best 7072-B result |
|---|---:|---:|
| random swap hill-climb from the base (seed 1) | 1076 | **48.963317** (no hit) |
| random move hill-climb from the base (seed 3) | 674 | **48.963317** (no hit) |
| random swap hill-climb from `v178` (seed 7) | 676 | **48.963317** (no hit) |
| random swap hill-climb from base+`184` (seed 11) | 675 | **48.963317** (no hit) |
| exhaustive single-line move of `0x90` to all 192 top-level positions | 192 | **48.963317** (base position) |

This confirms the base is a local optimum for single swaps and single
moves, and that `v178`'s 7024-B basin does not contain a 7072-B point above
the base.

## 6. Root cause

The base's source order is a **hill-climbed permutation** tuned for the
`difflib` instruction metric (git history: 20 passes from 37.7 % to
48.96 %).  It reproduces the original's HD-arm SLP group order
(`0x14,0x4,0x24,0x34,0x44,0x54,0x64,0x2c8,0x7c`) but not the original's
`else` arm, which is in **ascending-offset (iOS source) order**:

```
orig else: 4 8 c 10 14 18 1c 20 24 28 2c 30 34 38 3c 40 44 48 4c 50 54 58 5c 60 64 68 6c 2c0 70 74 2c4 2c8 2cc
base else: 24 4 8 c 10 14 1c 28 20 30 34 3c 38 44 40 4c 48 54 50 58 5c 5c 60 64 64 64 68 6c 70 70 74 2c4 1e0 …
```

The original's true source order is the iOS order
(`_work/reconstructions/layout_ctor/ios_source_order.md`), but that order
compiles to **6864 B / 35.69 %** (and the compiled-store order
`order_default.txt` to **6740 B / 21.72 %**): it is 208–336 B *smaller*
than the original because the original's expressions (four cold `iPad`
tails, eight extra SLP groups, and the `0x158/0x15c/0x170` round-trips)
add code that the base's scrambled order replaces with extra scalar stores.
The base trades store-order fidelity for hot-body instruction alignment,
which is what `difflib` rewards.

The remaining 51 % is therefore not a single wrong constant or callee: it
is the **source-order + expression-typing** difference between the
hill-climbed base and the original's ascending-offset source.  Closing it
requires reproducing both the iOS order *and* the original's non-foldable
float round-trips at exactly 7072 B; every 7072-B variant measured here
(and in waves 115–119) ties or loses to the base.

## 7. Proven vs hypothesis

**Proven (measured):**

* Original post-`pop` region = 966 B = 80 B cold + 2 B pad + 221 pool
  words; base = 778 B = 16 B cold + 2 B pad + 190 pool words.
* Original end pool = 50 groups + 14 scalars + 7 GOT = 221; base =
  42 + 14 + 8 = 190; net 8-group (32-word) deficit.
* Raw end-pool `difflib` ratio = 0.725061 (221 vs 190 words).
* Hot/tail split: original 2069/460, base 2121/366; hot ratio 0.438663,
  tail ratio 0.748184.
* The original's extra round-trips are at `0x158, 0x15c, 0x170, 0x280,
  0x3f4`; the base's extra ones at `0x148, 0x1f4`.
* Explicit `(int)(float)` on `0x158/0x15c/0x170` is a codegen no-op
  (clang folds the conversion when all ternary arms are integer constants).
* `v178` (0x178 retina/n9 arm swap) is the best raw score, 7024 B /
  49.080 %, 48 B short.
* The base is a local optimum for 1076 random swaps, 674 random moves, all
  192 single-line moves of `0x90`, and 676/675 swaps from `v178`/`v184`.
* Grouping the `0xb0..0xbc` SLP window keeps 7072 B but scores 44.03 %.

**Hypothesis (not proven):**

* The Android 2.0.16 source is the iOS ascending-offset order, and the
  208–336 B size gap is caused by the original's non-foldable float
  round-trips (`0x158/0x15c/0x170` and the `0x280/0x3f4` int conversions),
  not by a different statement set.
* The original's `0x158/0x15c/0x170` round-trips come from a shared float
  temporary (or a `(int)(float)` around a value clang cannot fold), which
  the base's literal-int ternary arms do not reproduce.

## 8. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7101
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4505
cmake --build cmake-build-match --target drift -j6      # 0 drifted fields

OBJ=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OBJ -d --start-address=0xd1028 --stop-address=0xd2bd0 \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so > /dev/shm/w7101/orig.dis
$OBJ -d --start-address=0xd1028 --stop-address=0xd2bd0 \
  cmake-build-match/libgof2hdaa.so > /dev/shm/w7101/ours.dis

python3 /dev/shm/w7101/score1.py                 # 7072 / 48.963317
bash    /dev/shm/w7101/fast.sh && python3 /dev/shm/w7101/fastscore.py
python3 /dev/shm/w7101/split.py                  # hot/tail ratios
python3 /dev/shm/w7101/stores.py /dev/shm/w7101/orig.dis 0xd1042 0xd1268
python3 /dev/shm/w7101/climb.py 1 480            # random swap hill-climb
```
