# `Layout::Layout` C1/C2 — wave 131 / worker w7685
## SLP store-group census + spill liveness: the 4-byte window shift is systematic; every lever that fixes it changes the 7072-B size

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr `0xd1028`,
size **7072 B**, source `src/game/ui/Layout.cpp`.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
Baseline = wave-130 master `7c6933c2d` (48.943802311678 % unrounded).

## Result — no source win (note only)

| metric | before (HEAD) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| unrounded `difflib` ratio | 48.943802311678 % | **48.943802311678 %** |
| `byte_exact` / `linked_exact` | 4506 / 4506 | **4506 / 4506** |
| `size_exact` / `data_exact` | 4524 / 843 | 4524 / 843 |
| imports / lint / drift | 207 / CLEAN / 0 | 207 / CLEAN / 0 |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
No `src/` change landed; the worktree is clean apart from this note.

## 1. NEW: full store census with addresses — the window shift is systematic

`/dev/shm/w7685/stores.py` tracks `add rX, r0, #imm` base registers and lists every
`vst1.32` / `strd` / `str(.w)` destination offset **with its instruction address**, so the
groups can be assigned to the HD / n9 / common / tail arms.

Whole-function totals (raw objdump counts):

| form | original | HEAD |
|---|---:|---:|
| `vst1.32` | **81** | **70** |
| `vld1.64` | **86** | **77** |
| `strd` | **62** | **51** |
| scalar `str` | 18 | 20 |
| `str.w` | 103 | 172 |

Per arm (vst1 group destinations):

| arm | original groups | HEAD groups |
|---|---|---|
| HD (`d1042..`) | 27 | 27 |
| n9 | 9 | 9 |
| common | **17** | **10** |
| tail | **28** | **25** |

So the HD and n9 arms already have the *same group count*; the deficit is common (17 vs 10)
and tail (28 vs 25). The earlier "HD 27 vs 26" figure was a base-register-tracking artefact.

### 1a. The shift is always one field (4 B) earlier in the original

| run | original group starts | HEAD group starts |
|---|---|---|
| `0xb0..0xcc` | `0xb0`, `0xc0` | `0xb4`, `0xc4` |
| `0x140..0x17c` | `0x140`,`0x150`,`0x160`,`0x170` | `0x148`,`0x158`,`0x168` |
| `0x204..0x278` | `0x204`,`0x214`,`0x224`,`0x23c`,`0x24c`,`0x25c`,`0x26c` | `0x208`,`0x218`,`0x228`,`0x240`,`0x250`,`0x260`,`0x270` |
| `0xec` run | `0xec` | `0xdc` (float group) |

Every original window starts at the **first** field of the run; HEAD's starts one field
later because that first field's store is scheduled *outside* the run. This is the same
"window shift" w126/w129 saw on `0xb0..0xcc`; this pass shows it is the dominant, systematic
difference across all four runs, not an isolated `0xb0` quirk.

### 1b. The HD-arm store order differs by *which* stores are sunk into it

Full HD-arm store order (base-register tracked):

```
orig: 0x14 0x4 0x24 0x34 0x44 0x54 0x64 0x2c8 0x7c | 0x74 0x2c0 0x2d8 0xac 0x8c 0xa4 |
      0xc0 0xb0 0xec 0x294 0x2a4 0x108 0x118 | 0xdc 0xe4 0xfc 0x104 0x2b4 |
      0x128 0x140 0x150 0x160 0x170 0x180 0x190 0x1a0 0x1b0 0x1c0 0x1d0 | 0x138 0x1e0
ours: 0x14 0x4 0x24 0x34 0x44 0x54 0x64 0x2c8 0x7c | 0x74 0x2c4 0x1e0 0x2d8 0x8c 0x98 0x9c 0xac |
      0xb4 0xc4 0x294 0x2a4 0xdc 0xb0 0xa8 0xa0 0xd4 0xa4 0xec 0xf4 0x2b4 |
      0x118 0x108 0x128 0x148 0x158 0x168 0x180 0x190 0x1a0 0x1b0 0x23c 0x2b8 0x138 0x204 0x2bc 0x178 0x2c0 0x1c0 0x1d0
```

HEAD sinks `0x23c`, `0x2b8`, `0x204`, `0x2bc`, `0x178`, `0x2c0` into the HD arm and stores
`0x2c4`/`0x2c0` separately, while the original keeps them in the common arm and pairs
`0x2c0`/`0x2c4` with one `strd` at `d1122`. This is the block-placement half of the residual.

## 2. NEW: `0.325f`/`0.65f` are unique to the end pool in the original

A whole-file word search shows `0x3ea66666` (0.325f) and `0x3f266666` (0.65f) occur **exactly
once each** in the original, at the end pool `0xd2854`/`0xd2858`. In HEAD they are in a
`.text` island at `0xd1928`/`0xd192c` (loaded with `subw r1, pc, #88`). The original's
`field_0x8c` island (`64.0f`/`32.0f`, `0xd1d94`/`0xd1d98`) is still a `.text` island, so the
difference is *only* the `0.325/0.65` pair. This confirms the w129 claim: the pool placement
is a downstream effect of the SLP-group count (more vector constants fill the early islands),
not an independent source lever.

## 3. NEW variants measured (all real `link_so` builds, exact unrounded ratio)

Scorer `/dev/shm/w7685/score.py` (verify.py `Resolver`+`reconcile`+`difflib`, reproduces the
full-link number bit-for-bit). ~0.7 s/variant.

### 3a. Int-literal forms for the float-expression `int` fields (removes the vcvt)

| variant | size | ratio % |
|---|---:|---:|
| **HEAD baseline** | **7072** | **48.943802** |
| `field_0xb0` HD `3*1.40625f` → `4` | 7116 | 18.242448 |
| `field_0xb8` HD `18*1.40625f` → `25` | 7060 | 19.554504 |
| `field_0xc4` HD `8*1.40625f` → `11` | 7040 | 20.834157 |
| `field_0xcc` HD `180*1.40625f` → `253` | 7068 | 45.434653 |
| `field_0x140` (already int) | 7072 | 48.943802 (no-op) |

Removing the conversion is a large lever: `field_0xb0` alone drops the prologue to a
5-word `stmdb sp!, {r7,r8,r9,sl,fp}` and re-rolls the whole schedule. No int-literal form
keeps 7072.

### 3b. Shared `retina||n9` local (the spill-liveness lever)

| variant | size | ratio % |
|---|---:|---:|
| **HEAD baseline** | **7072** | **48.943802** |
| `int rn = retina||n9;` then both ternaries | 6772 | 21.342444 |
| `bool rn = ...` | 6772 | 21.342444 |
| `unsigned char rn = ...` | 6772 | 21.342444 |
| `field_0x2d4` as ternary (same `||` expr as `0x2d8`) | 7056 | 48.633553 |
| `0x2d4` ternary before `0x2d8` | 7056 | 48.633553 |
| drop `volatile` on `rd` | 7064 | 48.853440 |
| `*rd` → `Globals::retinaDisplay` (keep if/else) | 7064 | 48.853440 |
| `unsigned char rn = *rd || n9;` used in if/else | 6860 | 34.620024 |

The base's `volatile unsigned char *rd` forces a second `retinaDisplay` load
(`d1890`/`d18b2`) and therefore a second `n9|retina` OR (`d1898`/`d18b6`); the original
computes it once in `r5` (`d1898`) and reuses it. But every shape that CSEs it shrinks the
body by 8–16 B (7064 / 7056), i.e. the `volatile` reload is exactly what holds HEAD at 7072.

### 3c. Moving the first field of each shifted run into its run

| variant | size | ratio % |
|---|---:|---:|
| **HEAD baseline** | **7072** | **48.943802** |
| `0x140`+`0x144` moved into the `0x140..0x17c` block | 7060 | 37.205589 |
| `0x13c..0x17c` fully ascending | 6980 | 37.334404 |
| `0x204` before `0x208` | 7032 | 45.636509 |
| `0x204` after `0x208` | 7032 | 45.636509 |
| `0x214` before `0x208` | 7064 | 45.157433 |
| `0x24c` before `0x250` | 7064 | 47.400916 |
| `0x26c` before `0x270` | 7072 | 48.943802 (no-op) |
| `0x2c0` after `0x2c4` | 7032 | 44.297719 |
| `0x2c0` before `0x2c4` | 7056 | 44.346784 |
| `0x2c4` moved next to `0x2c0` | 7008 | 44.030449 |

Moving the first field into its run *does* restore the aligned SLP windows in the HD arm
(verified: `0x140`,`0x150`,`0x160`,`0x170` appear after moving `0x140/0x144` in), but it
saves 12–92 B and the fuzzy score falls because the rest of the function no longer lines up.

### 3d. `field_0x1e8` / `field_0x1ac` live-range moves

| variant | size | ratio % |
|---|---:|---:|
| **HEAD baseline** | **7072** | **48.943802** |
| `0x1e8` after `0x1ac` | 6912 | 37.520194 |
| `0x1e8` after `0x1a8` | 6872 | 33.663366 |
| `0x1e8` before `0x1a8` | 6896 | 38.545455 |
| `0x1e8` after `0x2d8` | 6680 | 21.721478 |
| `0x1ac` after `0x1e8` | 6968 | 40.900865 |
| `0x2d8` after `0x1e8` | 6944 | 21.268882 |
| `0x1e8` as `(retina||iPadHD||n9) ? 1 : 0` | 7072 | 48.943802 (no-op) |
| `0x1a8` parenthesised | 7072 | 48.943802 (no-op) |

### 3e. Random single-line move search

148 random moves of one constructor statement to another position (255 candidate lines,
seed 7685): **no** variant both stayed at 7072 B and beat 48.943802. This extends the
w126/w129 adjacent-swap sweeps to non-adjacent moves.

## 4. Proven vs hypothesis

**Proven (measured):**

* Raw store forms: original 81 `vst1` / 62 `strd` / 86 `vld1` vs HEAD 70 / 51 / 77.
  Per-arm vst1 groups: HD 27/27, n9 9/9, common **17/10**, tail **28/25**.
* The original's SLP windows start one field (4 B) earlier than HEAD's in every shifted run:
  `0xb0` vs `0xb4`, `0x140` vs `0x148`, `0x204` vs `0x208`, `0xec` vs `0xdc`.
* HEAD sinks `0x23c`,`0x2b8`,`0x204`,`0x2bc`,`0x178`,`0x2c0` into the HD arm and stores
  `0x2c4`/`0x2c0` separately; the original keeps them in the common arm and pairs
  `0x2c0`/`0x2c4` with one `strd` at `d1122`.
* `0.325f`/`0.65f` occur exactly once each in the original, at the end pool
  `0xd2854`/`0xd2858`; HEAD materialises them in a `.text` island. `field_0x8c`'s
  `64.0f`/`32.0f` island is `.text` in both.
* Every source shape that CSEs the `n9|retina` value (the `[sp,#8]` spill) shrinks the body
  to 7056–7064 B; the `volatile` reload in the current source is what keeps 7072 B.
* 148 random non-adjacent single-statement moves: no 7072-B improvement.

**Hypothesis (not proven):**

* The original source order makes the *first* field of each run available to the scheduler,
  so the SLP vectorizer picks the aligned `0xb0/0x140/0x204/...` windows. Reproducing it
  from HEAD requires the same order *and* the original's +64 B of outlined cold tails (and
  the +124 B end pool) to pay for the ~50–90 B of scalar stores the extra groups remove.
  Every partial reorder measured here lands below 7072 B.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7685
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6     # RATCHET PASS, byte 4506

OBJ=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OBJ -d --start-address=0xd1028 --stop-address=0xd2bc8 \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so > /dev/shm/w7685/orig.dis
$OBJ -d --start-address=0xd1028 --stop-address=0xd2bc8 \
  cmake-build-match/libgof2hdaa.so > /dev/shm/w7685/ours.dis
python3 /dev/shm/w7685/stores.py /dev/shm/w7685/orig.dis /dev/shm/w7685/ours.dis
python3 /dev/shm/w7685/score.py        # 7072 / 48.943802311678
```

Scratch harnesses (not committed, `/dev/shm/w7685/`): `score.py` (exact single-function
scorer), `stores.py` (base-register-tracked store census), `try.sh`, `mkvar.py`,
`batch*.py`, `search.py`, `Layout.base.cpp`, `orig.dis`, `ours.dis`.
