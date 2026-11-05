# Layout::Layout C1/C2 — wave 119 / worker w6789

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (identical bodies), ELF VMA `0xd1028`,
original size **7072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave119-w6789`, base = current wave-119 master (`32e535a5e`).
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.

## Result — three source edits, 7072 B preserved

| metric | before (base) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| unrounded `difflib` ratio (full link) | 48.655110580 % | **48.824232762 %** |
| `byte_exact` / `linked_exact` | 4505 / 4505 | **4505 / 4505** |
| `size_exact` / `data_exact` | 4524 / 843 | **4524 / 843** |
| imports / lint / drift | 207 / CLEAN / 0 | **207 / CLEAN / 0** |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
No baseline update.  File size stays exactly **2,187,328 B**; 27 section
headers unchanged.  `.text` and `.ARM.exidx` are byte-size identical to the
base.  `.ARM.extab` is **0x26294** in both base and after (the wave-118 note's
`0x26290` is stale; the current wave-119 base links `0x26294`, 4 B short of
the original `0x26298`).  This change does not move it.

## The three changes

### 1. `field_0x184` if/else chain → single ternary, `n9` arm moved ahead of `iPad`

```cpp
// before: if/else chain (iPadHD / iPadLarge / retinaDisplay / iPad / n9 / else)
// after
this->field_0x184 = Globals::iPadHD ? 163 : (Globals::iPadLarge ? 346
    : (Globals::n9 ? 210 : (Globals::iPad ? 173 : (Globals::retinaDisplay ? 232 : 116))));
```

Alone this is **+8 B** (7080) and raises the ratio to 48.834893 %.

### 2. `field_0x160` — drop both round-trip casts

```cpp
// before
this->field_0x160 = (int)(float)(int)(Globals::iPadHD ? 49.0f * 1.40625f : ...);
// after
this->field_0x160 = (int)(Globals::iPadHD ? 49.0f * 1.40625f : ...);
```

Every arm is integral, so this is value-identical.  (`(int)(float)(...)` is
codegen-identical here — both forms measure 48.824232762 %.)  Combined with
(1) it is **−8 B** → back to 7072, ratio 48.814505 %.

### 3. `field_0x128` moved before `field_0x124`

Independent store; +0 B, +0.0097 ratio.

Combined: `+8 −8 +0 = 0` → exactly **7072 B**, ratio **48.824232762 %**.

### Caveat: this duplicates worker w6792

This is byte-for-byte the landed commit `e42ff60c5` on branch `wave119-w6792`
(not in this branch's HEAD), so the coordinator will see it twice and the
merge is a no-op.  It is a genuine improvement over this branch's base.  No
*different* 7072-B win was found in the time available (see the dead-end
table below).

## Measurement method

Two independent scorers were used and cross-checked:

* **full link** — `cmake --build cmake-build-match --target link_so` then
  `tools/verify/verify.py`'s exact pipeline (`Resolver` + `reconcile` +
  `difflib`, autojunk OFF) on `libgof2hdaa.so`;
* **partial link** — compile only `Layout.cpp.obj`, gold-link it alone into a
  throwaway `.so`, same pipeline (`/dev/shm/w6789/score.py`).

Both give **48.655110580** on the base and **48.824232762** on the landed
variant, so the fast partial-link scorer is trustworthy for this function.
(Watch out: `cmake --build --target link_so` can race with a concurrent build
in the same tree; always re-check the `.obj`/`.so` mtimes before trusting a
number.)

## Structural findings (this session)

* **HD-branch SLP store order.**  The first nine `vst1.32` groups are
  *identical* in original and ours:
  `0x14, 0x4, 0x24, 0x34, 0x44, 0x54, 0x64, 0x2c8, 0x7c`
  (`add r1,r0,#20/4/36/52/68/84/100/712/124`).  So the header block
  (`footerTextInset, headerTitleY, 0x1c, 0x20_top` then `0x4, 0x8, 0xc, 0x10`
  then `0x24, 0x28, 0x2c, 0x30`, …) already matches.
* **Second HD batch differs.**
  original: `0x128, 0x140, 0x150, 0x160, 0x170, 0x180, 0x190, 0x1a0, 0x1b0, 0x1c0, 0x1d0`
  ours:     `0x118, 0x108, 0x128, 0x148, 0x158, 0x168, 0x180, 0x190, 0x1a0, 0x1b0, 0x1d0`
  i.e. ours has two extra groups (`0x108`, `0x118`), groups the
  `0x148/0x158/0x168` windows instead of `0x140/0x150/0x160/0x170`, and misses
  `0x1c0`.  The `0x140` group needs `0x140`/`0x144` (currently far below at
  source lines 297/287) moved next to `0x148`/`0x14c`; the `0x1c0` group needs
  the `field_0x1cc` if/else converted to a ternary and placed after
  `field_0x1c0`.
* **Cold outlined tails.**  Original outlines four `else` tails after the
  `pop` (`0xd2802` 38 B = `0x130/0x134/v138`; `0xd2828` 14 B = `t_0x2c8`;
  `0xd2836` 14 B = `0x2d0`; `0xd2844` 14 B = `0x60`, all ending in
  `Globals::iPad ? A : B`).  Ours outlines one different tail (`0xd28be`,
  28 B = `field_0x1ac` `retinaDisplay ? 232 : 116`).  That is the 52 B
  cold-block component of the 46-word pool deficit.
* **`field_0x2d0` / `field_0x2c8` shape is already faithful.**  Original and
  ours both emit `vmov s0,r6; vcvt.f32.s32; vcvt.s32.f32; vstr s0,[r0,#720]`
  for `0x2d0` and the same sequence to `#712` for `0x2c8`; the `t_field_0x2c8`
  local + `(int)(float)` is correct.
* **`volatile unsigned char *rd`** is required: dropping `volatile` gives
  7064 B / 48.574277 % (w6761's result, re-confirmed).

## Dead ends measured this session (real linked scores)

All on top of the landed three-edit variant unless stated.  `size == 7072`
required; none beat 48.824233 %.

| variant | size | ratio % |
|---|---:|---:|
| `field_0x1cc` if/else → ternary (alone) | 7072 | 48.744520 |
| `field_0x1cc` ternary + moved after `field_0x1c0` | 7072 | 48.186528 |
| `field_0x1c0` moved after `field_0x1c8` | 7048 | 47.199522 |
| `field_0x1cc` moved after `field_0x1c4` | 7088 | 47.909200 |
| reorder `0x140..0x17c` into address order | 7004 | 36.746143 |
| `field_0x2d4` if/else → ternary | 7072 | 48.285486 |
| `field_0x20c` if/else → ternary | 7108 | 47.199046 |
| `orbitInfoTextX` if/else → ternary | 7108 | 47.199046 |
| `orbitInfoStationY` if/else → ternary | 7080 | 47.619048 |
| `field_0x218` if/else → ternary | 7080 | 47.619048 |
| `field_0x260` if/else → ternary | 7144 | 45.497818 |
| `field_0x280` if/else → ternary | 7072 | 48.744520 |
| `field_0x190` if/else → ternary | 7060 | 48.494516 |
| `rewardBoxY2` if/else → ternary | 7072 | 48.824233 (neutral) |
| `field_0x9c` → `(int)(float)(...)` | 7072 | 48.824233 (neutral) |
| `field_0xa8 = (int)v` | 7064 | 48.684211 |
| `v` arms → int (`82`/`41` not `82.0f`/`41.0f`) | 7032 | 48.421894 |
| `v` int arms + `field_0xa8 = v` | 7024 | 48.730762 |
| `v` → `(int)(float)(...)` round-trip | 7072 | 48.824233 (neutral) |
| `field_0x8c` arms → int | 7064 | 48.295115 |
| `field_0x168` arms → int | 7056 | 48.733799 |
| `field_0x2d8` moved before `field_0x84` | 7072 | 48.704663 |
| `field_0x2d0` moved after `field_0x2c8` | 7048 | 48.512677 |
| `field_0x2c4` moved before `field_0x2c8` | 6960 | 45.919390 |

Also re-ran on the landed variant: exhaustive adjacent swaps (all 202 pairs)
and single-statement moves within ±2 (~1020 variants, 4 workers, 4 m 10 s) —
**no 7072-B variant above 48.824233**.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6789
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6     # RATCHET PASS, byte 4505
```

Scratch harnesses (not committed): `/dev/shm/w6789/{score.py,fastscore.py,
fullscore.py,harness.py,search_par.py,exp_chains.py,exp190.py,exp_float.py,
exp_v.py,exp_2d4.py,exp_2d4b.py,reorder.py,stores.py}`, plus `best.cpp`
(the landed variant) and `base.cpp`.

## Coordinator merge note (wave 119 harvest)

Identical change to w6792 (already landed): 0x184 ternary + 0x160 unwrap +
0x128 move. Note only.
