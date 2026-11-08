# LevelScript::process — wave 153 / w9904: the w9804 best-aligned variant
# composes onto the w9805 base at exactly 57,232 B (aligned 17,185)

Target `_ZN11LevelScript7processEi` (`LevelScript::process`), original
`0x137d18`, **57,232 B (0xdf90)**, size-exact.  Owned TU
`src/game/world/LevelScript.cpp`.  Worktree `wt/w9904`, base master
`4a6104a43` (wave 152, w9805 landed).  Scratch in `/dev/shm/w9904/`.

## Result (landed)

```
                         before (HEAD)     after (landed)
process raw diffbytes    55,347            55,328            (-19)
process aligned insns    17,223            17,185            (-38)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
whole .text diffbytes    87,131            87,112            (-19)
.ARM.extab diffbytes     1,324             1,324             (0)
.ARM.exidx               0                 0
.rel.plt                 0                 0                 (byte-identical)
.rodata / .data / .bss   exact             exact
.note.gnu.build-id       20                20
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

All 27 section sizes exact; every section except `.text` / `.ARM.extab` /
`.note.gnu.build-id` is byte-identical.

## 1. The mission's w9804 variant is size-exact on the w9805 base

w9804 (wave 152) measured its best-aligned variant on the *pre-w9805* base:
`expect(weak,1)` + restore case-144 `setStation(getStation())` + revert the
case-92 3-call `getPosition()` shape = 57,232 B, raw 55,328, aligned 17,185
(-59 aligned / +8 raw vs that base's 55,320 / 17,244).

w9805 then landed `c154done1 + weak1 + s5timer1`, which is size-neutral on
its own base but not additive with the w9804 variant.  Applying only the
w9804 edits (keep `weak1`, add the case-144 restore, revert the case-92
3-call, and drop w9805's `c154done1` and `s5timer1`) reproduces the w9804
numbers **exactly** on the current base:

```
raw 55,328   aligned 17,185   size 57,232 (exact)
```

## 2. Full composition sweep (all built + linked + measured)

Base = current HEAD + w9804 edits (case-144 restore, case-92 revert).
`c154` = `expect(m_nScriptTimerA >= 2001, X)` at case-154 s9 completion;
`weak` = `expect(Globals::isRunningHDonWeakDevice, X)` at case-157 s5;
`s5timer` = `expect(m_nScriptTimerB >= 25001, X)` at case-157 s5;
`bigcond` = `expect((destroyed >= 9 && ...) || ... , X)`; `None` = plain if.

| c154 | weak | s5timer | size | raw | aligned |
|---|---:|---:|---:|---:|---:|
| None | None | None | 57,240 | 54,918 | 17,229 |
| None | 0 | None | **57,232** | 55,328 | 17,276 |
| **None** | **1** | **None** | **57,232** | **55,328** | **17,185** |
| None | 1 | 0 | 57,240 | 55,472 | 17,181 |
| None | 1 | 1 | 57,240 | 55,455 | 17,182 |
| 0 | 1 | None | 57,196 | 55,382 | 17,506 |
| 1 | 0 | None | 57,232 | 55,328 | 17,277 |
| **1** | **1** | **None** | **57,232** | **55,328** | **17,186** |
| 1 | 1 | 0 | 57,240 | 55,472 | 17,182 |
| 1 | 1 | 1 | 57,240 | 55,455 | 17,183 |

`bigcond=0` is the only other size-exact lever (-8 B) but it wrecks the
alignment (17,339..17,349); `msg7over` is a no-op once `c154` is unweighted.
The best size-exact pair is therefore **w9804's variant with no extra
w9805 weight** (17,185); keeping `c154done1` is size-exact too but +1
aligned (17,186).  Both beat the landed w9805 baseline (17,223 / 55,347).

The w9804 variant trades two w9707 faithfulness fixes for alignment: the
case-92 3-call `getPosition()` (original calls it three times) and the
case-144 `setStation` (original `0x141490` has no setStation).  It also
drops w9805's speculative `c154done1` / `s5timer1` branch weights, which
the measurements show are not in the original for this layout.

## 3. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9904/fdiff.py _ZN11LevelScript7processEi        # 55,328 / 57,232
python3 /dev/shm/w9904/align_diff.py _ZN11LevelScript7processEi  # 17,185
cmake --build cmake-build-match --target check -j6               # RATCHET PASS
```

## 4. Open questions

1. The case-125 inline body (1022 B vs the original's 98 B outlined entry)
   is still the largest structural delta; the two missing `blx` calls in the
   aligned diff are likely its outlined callee.
2. The residual 0xea vs 0x108 completion-tail gap (w9805) is unchanged; it
   is the `this`-in-`r4` vs `sl` register allocation, not a missing block.
3. Re-test the w9805 adjacency pair (`c154done0 + weak1`, -44 B) once a +44 B
   companion is found; it is still the only route to the original's
   c154->strong-arm adjacency.
