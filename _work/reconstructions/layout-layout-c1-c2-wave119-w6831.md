# Layout::Layout C1/C2 — wave 119 / worker w6831 (second pass)

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (identical bodies), ELF VMA
`0xd1028`, original size **7072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave119-w6831`, base = current wave-119 master (`fdb68f8ee`).
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.

This pass continues from the earlier `layout-layout-wave119-w6811.md`
(48.833964521 %, 7072 B) and re-applies two statement-order edits that had
been lost in the wave-119 merge.

## Result — two real 7072-B improvements landed

| metric | before (base) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| unrounded `difflib` ratio (full link) | 48.833964521 % | **48.923444976 %** |
| `report.json` pct (1 dp) | 48.8 | **48.9** |
| `byte_exact` / `linked_exact` | 4505 / 4505 | **4505 / 4505** |
| `size_exact` / `data_exact` | 4524 / 843 | **4524 / 843** |
| imports / lint / drift | 207 / CLEAN / 0 | **207 / CLEAN / 0** |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`
(verify: byte 4505, linked 4505, stubs 0, missing 0, extra 0, avg 99.94;
data 843/843; sodiff imports 207).  No baseline update.  File size stays
exactly **2,187,328 B**; 27 section headers unchanged; `.text` = `0x1337c8`,
`.ARM.exidx` = `0x6f70`, `.ARM.extab` = `0x26290` (still 8 B short of the
original `0x26298`), `.rodata` = `0x41c27`, `.data` = `0x296c` — all
unchanged, because the ctor body is still exactly 7072 B.

## The two source edits

Both are pure statement relocations of independent stores (no value change),
i.e. the same kind of edit the earlier w6801 pass had landed but which did not
survive the later wave-119 merges.

### 1. `field_0x94` / `field_0x1f4` swapped (+0.0797 pp)

```cpp
// before
this->field_0x94  = Globals::iPadHD ? 14 : (Globals::n9 ? 15 : (Globals::retinaDisplay ? 20 : 10));
this->field_0x1f4 = Globals::iPadHD ? 16 * 1.40625f : (Globals::retinaDisplay ? (Globals::iPad ? 32 : 4) : (Globals::iPad ? 16 : 2));
// after
this->field_0x1f4 = Globals::iPadHD ? 16 * 1.40625f : (Globals::retinaDisplay ? (Globals::iPad ? 32 : 4) : (Globals::iPad ? 16 : 2));
this->field_0x94  = Globals::iPadHD ? 14 : (Globals::n9 ? 15 : (Globals::retinaDisplay ? 20 : 10));
```

Both sit between `rewardBoxHeight` and `rewardBoxY`; they are independent
stores.  Alone: **7072 B / 48.913693442 %** (+0.0797 pp).

### 2. `field_0xb4` moved before `hudInset` (+0.0098 pp)

```cpp
// before: field_0xb8, hudInset, field_0xa4, oreTextYOffset, field_0xc4,
//         field_0xbc, field_0xc0, field_0xb4, field_0xf0, field_0xc8
// after:  field_0xb8, field_0xb4, hudInset, field_0xa4, oreTextYOffset,
//         field_0xc4, field_0xbc, field_0xc0, field_0xf0, field_0xc8
```

This is one of the four-int SLP-adjacency windows: it changes the store
schedule around the `0xb0..0xcc` run without moving the body off 7072 B.
Alone on top of edit 1: **7072 B / 48.923444976 %** (+0.0098 pp).

## Measurement method

Exact single-symbol scorer: compile only `Layout.cpp` with the NDK r18b
clang 7.0.2 flags, gold-link that one object into a throwaway `.so`, strip it
with the same `arm-linux-androideabi-strip --strip-all` as the real link, then
run the `tools/verify/verify.py` `Resolver` + `reconcile` +
`difflib(autojunk=False)` pipeline on `_ZN6LayoutC1Ev`
(`/dev/shm/w6831/fast2.sh` + `score2.py`, ~0.5 s/variant).  It reproduces the
full-link score bit-for-bit (48.833964521 on the base, 48.923444976 on the
landed variant).  The full `check` target was re-run on the landed tree.

## Searches run this pass (all real builds, 7072 B required)

| search | builds | best 7072 result |
|---|---:|---:|
| exhaustive adjacent swaps on the new base | 302 | 48.923444976 (tie; all swaps neutral) |
| exhaustive single-statement moves ±8, two rounds | 8,948 | **48.923444976** (only (0xb4 → before hudInset) won) |
| full-range move of `field_0x90` | 301 | 42.159793 (only 1 hit) |
| random contiguous block moves (len 2–8, 1–3 per build) | 600 | no 7072 hit |
| `v138` if/else block moved to 14 positions | 14 | 41.50 (all worse) |
| `quickMenuHeaderLift` moved to 30 positions | 30 | no win |

## Variants measured but not landable (wrong size)

| variant | size | ratio % |
|---|---:|---:|
| drop inner `(int)` of `field_0x160` | 7064 | **48.933200399** |
| `field_0x1a0`/`field_0x1a8` swap | 7076 | 48.894202032 |
| drop `(int)(float)` of `field_0x150` | 7064 | 48.813559322 |
| drop `(int)(float)(int)` of `field_0x160` (both) | 7064 | 48.903071400 |
| drop 0x160 cast + append `field_0x134 = iPad ? 108 : 80` | 7088 | 48.756218905 |

The `field_0x160` inner-cast drop is the best raw score seen (48.9332) but is
8 B short; every 8-B compensation tried (duplicate `0x134`/`0x130`/`0x94`/
`0x28c`/`0x285`/`0x140` stores at the ctor tail) overshot or undershot and
scored lower at 7072 B.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6831
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6     # RATCHET PASS, byte 4505
```

Scratch harnesses (not committed): `/dev/shm/w6831/{fast2.sh,score2.py,
tester.py,movesearch.py,swapsearch.py,moves.py,rblock.py}`.
