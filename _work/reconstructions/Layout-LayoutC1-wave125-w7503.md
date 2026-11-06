# `Layout::Layout` C1/C2 — wave 125 / worker w7503

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (identical bodies, aliased at
`0xd1028`), original size **7072 B**, source `src/game/ui/Layout.cpp`
(constructor only).  Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(sha256 `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`).
Base = wave-124 final master `02513de33`.

## Result — source win landed

| metric | before (base) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| unrounded `difflib` ratio | 48.934051 % | **48.943802 %** |
| `report.json` pct (1 dp) | 48.9 | 48.9 |
| `byte_exact` / `linked_exact` | 4506 / 4506 | **4506 / 4506** |
| `size_exact` / `data_exact` | 4524 / 843 | 4524 / 843 |
| imports / lint / drift | 207 / CLEAN / 0 | **207 / CLEAN / 0** |
| symbol parity | 5370/5370 | 5370/5370 |
| `.text` differing bytes | 85,975 | **85,953** (−22) |
| `.ARM.exidx` differing bytes | 0 | **0** (byte-identical) |
| `.ARM.extab` | 156,312 B @ 0x1a3b78 | **156,312 B @ 0x1a3b78**, 1,474 diff |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`
(`verify: byte 4506 linked 4506 stubs 0 missing 0 extra 0 avg 99.93`,
`data 843/843`, `sodiff extra 0 missing 0`, `imports 207`).
`--target drift` → `0 field(s) across 0 class(es)`.  `--target lint` → `CLEAN ✓`.
The function keeps its 7072-B symbol size, so no downstream `.text` address
moves and `byte_exact` is unchanged.  Only the `.text` content-delta count
moves 85,975 → 85,953 (docs updated in the same commit).

## The change (one expression)

```cpp
// before (wave-123 n9-first spelling)
this->field_0x184 = Globals::iPadHD ? 163 : (Globals::iPadLarge ? 346
    : (Globals::n9 ? (Globals::iPad ? 173 : (Globals::retinaDisplay ? 173 : 210))
                   : (Globals::retinaDisplay ? 232 : (Globals::iPad ? 173 : 116))));
// after
this->field_0x184 = Globals::iPadHD ? 163 : (Globals::iPadLarge ? 346
    : (Globals::n9 ? (Globals::iPad ? 173 : 210)
                   : (Globals::iPad ? 173 : (Globals::retinaDisplay ? 232 : 116))));
```

This is a **semantic bug fix** as well as a code-shape win.  The correct
priority order is `iPadHD > iPadLarge > iPad > n9 > retina`; the wave-123
spelling gave `n9+retina -> 173` (original 210) and `iPad+retina -> 232`
(original 173).  The new spelling reproduces the original store value for
**all 32 flag combinations** (verified with the repo's Thumb/VFP store
emulator, `_work/reconstructions/layout_ctor/emu.py`, comparing our linked
`.so` against the original: zero differing words for all 32 combos).

## Evidence

Original `field_0x184` value table (emulated from the original ctor, `emu.py`):

| flags | value |
|---|---:|
| iPadHD (any) | 163 |
| iPadLarge | 346 |
| n9 only | 210 |
| n9+retina | 210 |
| n9+iPad | 173 |
| iPad only | 173 |
| iPad+retina | 173 |
| retina only | 232 |
| default | 116 |

The old chain returned 173 for `n9+retina` and 232 for `iPad+retina`; the
new chain returns 210 and 173.  The corrected chain also removes one
compare/branch from the n9 arm and reorders the non-n9 arm, which is why the
linear instruction stream aligns 22 `.text` bytes better.

## Round-trip investigation (assignment focus)

The assignment asked to preserve the original's `(int)(float)` round-trips
(the original keeps 21 `vcvt` sites; ours folds six and adds six).  I mapped
every round-trip store offset in both binaries:

* original: `0x60 0x68 0x6c 0x70 0x2c8 0x2d0 0x8c 0x9c 0xa8 0x138 0x150
  0x158 0x15c 0x160 0x168 0x170 0x1a8 0x1e4 0x3e0 0x3f8 0x1f4 0x320`
* ours: `0x20 0x60 0x68 0x6c 0x70 0x2c8 0x2d0 0x8c 0x9c 0xa8 0x10c 0x148
  0x150 0x160 0x168 0x1a8 0x204 0x3e0 0x3e8 0x1f4 0x320`

Missing in ours: `0x138 0x158 0x15c 0x170 0x1e4 0x3f8`.
Extra in ours: `0x20 0x10c 0x148 0x204 0x3e8`.

The original's round-trips survive because the value is an **int phi across
store-bearing basic blocks** (e.g. `0xd1da6: vmov s0,r4; vcvt.f32.s32;
vcvt.s32.f32; vstr s0,[r0,#0x158]`, with `r4 = 13/14` set in branches that
also store `0x154`).  Every source form that keeps the field-major body and
adds a cast folds (`(int)(float)(select-of-constants)` is constant-folded by
clang 7.0.2 `-Oz`), and every form that introduces store-bearing branches
changes the size away from 7072 and/or loses more instruction alignment than
it gains.  Measured (real `link_so` builds, unrounded ratio):

| variant | size | ratio % |
|---|---:|---:|
| **base** | **7072** | **48.934051** |
| `0x158` default `(int)(float)(field_0x154-1)` | 7100 | 47.920398 |
| `0x158` whole ternary `(int)(float)` | 7096 | 48.477612 |
| `0x138 = (int)(float)v138` | 7088 | 48.616365 |
| `0x3f8 = (int)(float)((rewardBoxY+17)<<retina)` | 7088 | 48.426922 |
| `0x1e4 = (int)(float)(...)` | 7016 | 42.275449 |
| `0x15c` / `0x170` whole `(int)(float)` | 7072 | 48.934051 (folded no-op) |
| `0x158` volatile local | 7148 | 22.776787 |
| `0x158` stack array/struct temp | 7096 | 48.477612 |
| `0x158` static local temp | 7096 | 48.477612 |
| `0x154+0x158` store-bearing chain | 7072 | 48.416019 |
| `0x154+0x158+0x15c` store-bearing chain | 7020 | 48.760000 |
| `0x154+0x158+0x15c+0x160` chain | 7028 | 18.696772 |
| `0x138` cast + `0xa8`/`0x160` direct (size-neutral) | 7072 | 48.854353 |
| `0x138` cast + `0x150`/`0x160` direct | 7072 | 48.884462 |
| `0x184` old spelling + `0x158` win (base) | 7072 | 48.934051 |
| **`0x184` corrected chain (landed)** | **7072** | **48.943802** |

Also exhausted: all 85 HD-arm `(int)(N*1.40625f)` casts (all 48.934051),
all 191 adjacent statement swaps (none above base), 300 random single-
statement moves (none above base), all default-arm anchors for
`0x148/0x15c/0x170/0x1e4/0x204/0x3e8` (none above base), and the
`0xec..0xf8` SLP-group variant combined with round-trip casts (all below
base).  The field-major base remains a strong local optimum for the fuzzy
metric; the only landable win found is the semantically-exact `0x184` chain.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7503
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
cmake --build cmake-build-match --target drift -j6      # 0 drifted fields
cmake --build cmake-build-match --target lint           # CLEAN
python3 /dev/shm/w7503/cmp32.py                         # empty: all 32 combos exact
```

Scratch harnesses (not committed, `/dev/shm/w7503/`): `try.py` (single-TU
compile + gold link + exact unrounded ratio), `cmp32.py` + `emu2.py`
(32-combination store-value comparison), `batch*.py` / `search1.py` /
`climb.py` / `adjswap.py` / `anchor*.py` (variant and order searches).
