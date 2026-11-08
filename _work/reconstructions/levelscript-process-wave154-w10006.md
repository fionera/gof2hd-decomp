# LevelScript::process — wave 154 / w10006: c154 s9 completion immediately
# before the c157 s5 strong arm at 57,232 B, aligned 17,401 -> 17,363

Target `_ZN11LevelScript7processEi` (`LevelScript::process`), original
`0x137d18`, **57,232 B (0xdf90)**, size-exact.  Owned TU
`src/game/world/LevelScript.cpp`.  Worktree `wt/w10006`, base master
`4417db0b4` (wave 153).  Scratch in `/dev/shm/w10006/`.

## Result (landed)

```
                         before (HEAD)     after (landed)
process raw diffbytes    55,328            55,380            (+52)
process align_diff       17,185            17,363            (+178)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
RATCHET                  PASS              PASS
```

The mission target is met: the case-154 state-9 completion tail (`strd;
b.w`) is **immediately before** the c157 state-5 strong arm (`movs r5,#11;
movs r4,#0`), gap **0x4** (the `b.w`; original gap 0x4), at exactly
57,232 B, with the original mission case-entry order.  Aligned improves
from w9907's 17,401 to **17,363** (-38).

## 1. The landed companion (5 levers, all order-preserving)

The -44 pair from w9907 is `expect(m_nScriptTimerA >= 2001, 0)` (c154 s9)
+ `expect(isRunningHDonWeakDevice, 1)` (c157 s5).  On the wave-153 w9804
base the second weight is already present, so the pair alone is **-36 B**
(57,196).  The +36 companion found by a full subset sweep of the w9907
levers is:

| # | line | edit | region |
|---|------|------|--------|
| 1 | 1816 | `m_nState = 2` -> `m_nState++` (state is 1) | wormhole state-1 arm |
| 2 | 4642 | `messages[6]->isOver()` -> `__builtin_expect(..., 1)` | c154 s6 |
| 3 | 4669 | `expect(m_nScriptTimerA >= 2001, 0)` (the pair) | c154 s9 |
| 4 | 4869 | `expect(m_nScriptTimerB >= 25001, 1)` | c157 s5 |
| 5 | 4930 | `m_nScriptTimerA <= 9999` -> `expect(..., 1)` | c157 s7 |
| 6 | 4974 | `m_nState = 9` -> `m_nState++` (state is 8) | c157 s8 |

All are semantically identical (`m_nState++` on a known state; weights are
hints only).  Note the w9907 `s3inc` (c157 s3 `m_nState = 4` -> `++`) is
**not** in this companion: it is +16 alone but the composition
s1+msg6+s3+s7+s8 is +8, not +44 (strong non-additivity).

## 2. Case-entry table (deliverable)

Original and landed — identical (31 real cases; trailing 132.. are the
tbh default-to-end entries):

```
158,131,64,135,145,142,144,139,69,73,67,70,29,41,80,56,125,92,157,40,
102,154,24,14,21,42,91,94,105,114,16
```

The table is extracted by parsing the mission dispatch compare chain plus
the two `tbh` tables (`sub.w rX,r0,#131; cmp rX,#14` -> 0x138a8a and
`sub.w rX,r0,#64; cmp rX,#9` -> 0x138c3e) and sorting the resolved case-body
addresses.  Script: `/dev/shm/w10006/table.py`.

## 3. Measured sweep (full build + link, size / raw / aligned)

Base = wave-153 HEAD (w9804 variant, 57,232 / 55,328 / 17,185).
`pair` = c154 s9 `expect(...,0)` alone (weak1 already present) = 57,196
(-36), raw 55,382, aligned 17,506, gap 0x4, order ok.

Individual levers on `pair` (size delta): msg6 +24, s1 +8, s3 +16,
s5t1 +8, s5t0 +8, s7 -8, s8 +4, s0 +32, msg7 0, tail +12,
c154_10 (`m_nState = 10`) +4, c154_10r +8.

Size-exact combos (all gap 0x4, order ok):

| combo | raw | aligned |
|---|---:|---:|
| **s1,msg6,s5t1,s7,s8 (landed)** | **55,380** | **17,363** |
| s1,s3,tail | 55,375 | 17,365 |
| s1,msg6,s7,s8,tail | 55,369 | 17,369 |
| s1,s3,s5t1,tail | 55,398 | 17,372 |
| msg6,s3,s7,s8 | 55,406 | 17,511 |
| msg6,s8,s5t1 | 55,429 | 17,562 |

Near-exact (better aligned but not size-exact, not landable):
`s1,s3,s5t1,s7,c154_10` = 57,216 (-16), raw 55,431, aligned **17,316**;
`s1,s3,s5t1,s7,c154_10r` = 57,220 (-12), raw 55,421, aligned 17,318;
`s1,s3,s5t1,s7,tail` = 57,224 (-8), raw 55,428, aligned 17,320.

## 4. Negative results (measured, not landed)

| variant | size | raw | aligned | gap | order |
|---|---:|---:|---:|---:|---|
| pair alone | 57,196 | 55,382 | 17,506 | 0x4 | ok |
| c154g (`expect(m_nState==9,1)`) | 57,248 | 55,382 | 22,199 | 0x4 | **reordered** |
| + weak0 (strong arm cold) | 57,232 | 55,480 | 17,454 | 0x10 | ok |
| + dead0/dead1/ship/i3 weights | 57,232 | 55,380 | 17,363 | 0x4 | ok (no-ops) |
| + destr (destroyed=0 then +5) | 57,232 | 55,386 | 17,454 | 0x4 | ok |
| + msg7 weight | 57,232 | 55,380 | 17,363 | 0x4 | ok (no-op) |

`expect(m_nState == 9, 1)` on the c154 state guard reorders the mission
cases (aligned 22,199) — confirming w9805's rule that state-guard weights
are the wrong lever.

## 5. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10006/fdiff.py _ZN11LevelScript7processEi       # 55,380 / 57,232
python3 /dev/shm/w10006/align_diff.py _ZN11LevelScript7processEi # 17,363
python3 /dev/shm/w10006/gap.py                                   # ours strong 0x145bd4 gap 0x4
python3 /dev/shm/w10006/table.py                                 # order matches original
cmake --build cmake-build-match --target check -j6               # RATCHET PASS
```

## 6. Open questions

1. The aligned metric still costs +178 vs the no-adjacency baseline
   (17,185).  The near-exact `s1,s3,s5t1,s7,c154_10` shape reaches 17,316
   but is -16 B; a +16 B order-preserving companion for it is still open.
2. The residual completion-tail gap is now exactly the `b.w` (0x4), but
   ours tail-merges `m_nState++` into the shared increment at 0x13f3a0
   while the original inlines `ldr/adds/str`; the original also keeps
   `this` in `sl` (4-byte accesses) vs our `r4` (2-byte).  A prologue shape
   that puts `this` in `sl` would add the 36 B inside the block.
