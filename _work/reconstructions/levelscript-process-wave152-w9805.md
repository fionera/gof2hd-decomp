# LevelScript::process — wave 152 / w9805: c154->strong adjacency without
# reordering the mission case entries (inner-condition weight pair)

Target `_ZN11LevelScript7processEi` (`LevelScript::process`), original
`0x137d18`, **57,232 B (0xdf90)**, size-exact.  Owned TU
`src/game/world/LevelScript.cpp`.  Base master `9d491d46a` (wave 151).
Scratch in `/dev/shm/w9805/`.

## Result

```
                         HEAD (wave 151)   landed (this pass)
process raw diffbytes    55,320            55,347            (+27)
process align_diff       17,244            17,223            (-21)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
RATCHET                  PASS              PASS
```

## 1. The finding: an inner-condition weight pair keeps the case order

w9705's pair (`expect(m_nState == 9, 1)` + `expect(m_nState == 4, 0)`) is a
pair of **state-switch guards**.  Any weight on a state guard changes the
case-154 / case-157 state-switch lowering (tbh -> compare chain), which
changes the block order and therefore reorders the *mission* case entries
(aligned 17,244 -> 22,486 on this base; raw 55,320 -> 55,435).

The reorder is **not** caused by the adjacency itself.  The adjacency can be
reproduced with a pair of **inner-condition** weights that never touch a
state-switch guard, and that pair leaves the mission case order untouched:

```cpp
// case 154 state 9 completion (line 4669): make the completion block cold
if (__builtin_expect(m_nScriptTimerA >= 2001, 0)) { // lint: arm_builtin
// case 157 state 5 strong arm (line 4855): make the else (non-weak) arm cold
if (__builtin_expect(Globals::isRunningHDonWeakDevice, 1)) { // lint: arm_builtin
```

Measured (`c154done0 + weak1`):

```
size 57,188 (-44)   raw 55,471   aligned 17,541   gap(marker->strong) 0xea
```

The c154 s9 completion block is **immediately before** the c157 s5 strong
arm (`movs r5,#11`), exactly as in the original (orig gap 0x108; ours 0xea =
0x108 - 0x1e, the residual being the `this`-in-`r4` vs `sl` register
allocation inside the completion tail, not a missing block).  The mission
case order is **identical to the original** (see §2).

The size is -44 B, so this pair cannot be landed as-is: a -44 shift makes
every following function non-byte-exact (byte_exact 4507 -> 2741).  A +44
companion is the remaining work.

## 2. Case-entry table (the deliverable)

Baseline (wave 151) and `c154done0 + weak1` — identical, and identical to
the original's order:

```
158,131,64,135,145,142,144,139,69,73,67,70,29,41,80,56,125,92,157,40,
102,154,24,14,21,42,91,94,105,114,16
```

w9705's state-guard pair reorders it:

```
158,131,64,145,135,144,142,139,69,73,67,70,29,41,80,125,56,92,157,40,
154,102,24,14,114,42,105,21,91,94,16
```

Swapped/moved: (135,145), (142,144), (56,125), (102,154), and
114/42/105/21.  This is the whole source of w9705's aligned regression.

`c154done0 + weak1` case-entry table (ours) vs original — the entry order is
identical; the only large `dSpan` rows are the same cold-block placement
differences that already exist at HEAD (case 64 +130, case 70 -136, case
125 -924, case 16 +224).  No new reorder.

## 3. The landed size-exact variant

Because the adjacency pair is -44 B, the landed change is its size-exact
sibling: keep the completion **hot** and add the state-5 timer weight:

```cpp
if (__builtin_expect(m_nScriptTimerA >= 2001, 1)) { ... }   // case 154 s9
if (__builtin_expect(Globals::isRunningHDonWeakDevice, 1)) { ... } // c157 s5
if (__builtin_expect(m_nScriptTimerB >= 25001, 1)) { ... }  // c157 s5 timer
```

Measured: size 57,232 (exact), raw 55,347, aligned 17,223 (better than HEAD
17,244), mission case order preserved.  It does **not** move the completion
block adjacent to the strong arm (gap 0x11d2); it is landed only because it
is size-exact and gate-green, and it locks the order-preserving
inner-condition lever.

## 4. Negative results (all measured, not landed)

| variant | size | raw | aligned | gap | order |
|---|---:|---:|---:|---:|---|
| HEAD | 57232 | 55320 | 17244 | -0x6da | ok |
| w9705 (`e9`+`s4e0`) | 57232 | 55435 | 22486 | 0x108 | **reordered** |
| `e9` alone | 57268 | 55384 | 21596 | 0x4a4 | reordered |
| `s4e0` alone | 57220 | 55333 | 19833 | -0xa5e | reordered |
| `c154done0` alone | 57196 | 55401 | 17528 | -0x179a | ok |
| `c154done1` alone | 57232 | 55320 | 17245 | -0x6da | ok |
| **`c154done0`+`weak1`** | **57188** | **55471** | **17541** | **0xea** | **ok** |
| `c154done1`+`weak1` | 57224 | 55521 | 17221 | 0x11d2 | ok |
| `c154done1`+`weak1`+`s5timer1` | 57232 | 55347 | 17223 | 0x11d2 | ok |
| `c154done0`+`weak1`+`s5timer1` | 57196 | 55400 | 17543 | 0xea | ok |

Every `__builtin_expect` on a case-157 state guard (`s1e0..s9e0`, `s4_1`,
`s4_ne0`) reorders the mission cases.  Every inner-condition weight
(`c154done*`, `weak*`, `s5timer1`, `s5big0`, `msg7over1`) preserves them.
`switch (__builtin_expect(m_nState, 9))` over the case-154 state chain is
+76 B and aligned 34,075 (worse), confirming the state-guard route is the
wrong lever.

## 5. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9805/fdiff.py _ZN11LevelScript7processEi
python3 /dev/shm/w9805/align_diff.py _ZN11LevelScript7processEi
python3 /dev/shm/w9805/table.py        # case-entry table
python3 /dev/shm/w9805/probe.py        # marker -> strong-arm gap
cmake --build cmake-build-match --target check -j6
```

## 6. Open questions

1. Find a +44 B companion for `c154done0 + weak1` (adjacency + order at
   exact size).  Reverting the four w9707 faithfulness fixes is net 0; the
   mission-50/51 pre-switch specials are the largest unmodelled block left.
2. The residual gap 0xea vs 0x108 is the `this`-in-`r4` vs `sl` register
   allocation in the completion tail (the same allocator difference w9302 /
   w9603 documented).  Fixing it would make the adjacency byte-identical.
