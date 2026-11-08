# LevelScript::process — wave 150 / w9705: c154 s9 completion immediately
# before the c157 s5 strong arm, landed at exactly 57,232 B

Status: **code change landed**, 2026-11-08.  Target
`_ZN11LevelScript7processEi` (`LevelScript::process`), original `0x137d18`,
**57,232 B (0xdf90)**, size-exact.  Owned TU `src/game/world/LevelScript.cpp`.
Worktree `wt/w9705`, base master `db9658a51` (wave 150).  Scratch in
`/dev/shm/w9705/`.

```
                         before (HEAD)     after (landed)
process raw diffbytes    54,792            55,142            (+350)
process align_diff       17,605            22,383            (+4,778)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
RATCHET                  PASS              PASS
```

The mission target is met: the case-154 state-9 completion block is now
**immediately before** the case-157 state-5 strong arm, exactly as in the
original, and the function is still exactly 57,232 B.

## 1. The landed change

The w9601 two-sided weight pair, unchanged:

```cpp
} else if (__builtin_expect(m_nState == 9, 1)) { // case 154 state 9
...
} else if (__builtin_expect(m_nState == 4, 0)) { // case 157 state 4
```

Both lines carry a same-line `// lint: arm_builtin` waiver (the lint scanner
counts every non-`offsetof` `__builtin_*` as a SOFT `arm_builtin` hit).

Resulting layout (built object):

```
143fa4  movs r0,#6          ; c154 s9 completion tail
143fa6  strd r4,r4,[r9]
143faa  str.w r0,[sl,#28]
143fae  b.n 1446ac          ; break out of the mission switch
143fb0  ldr r6,[sp,#96]     ; <- c157 s5 strong arm begins (no block between)
143fb2  movs r5,#11
143fb4  movs r4,#0
143fb6  b.n 143fc6
```

Original: `0x144a12 b.n 144e36` then `0x144a14 ldr r6,[sp,#96]; 0x144a16
movs r5,#11`.  Marker-to-strong gap: ours `0x104`, original `0x108` (the
residual 4 B is the `m_nState = 6` / `m_nState++` tail difference, see §3).

## 2. Where the +40 B went (it is already gone on this base)

w9601 measured `e9 + s4e0` at **57,272 B (+40)** on base `7eed2966e` (wave
149, before w9602).  On the current base it is **57,232 B (exact)**.  The
difference is w9602's landing (`2b1a77728`, case-125 independent-`if` shape):
w9602 is size-neutral on the pristine baseline but removes exactly the 40 B
that the `e9` weight pair added to the case-125 region.  No further +40
companion is needed on `db9658a51`.

Measured size deltas around the shape (all on the current base):

| variant | size | delta |
|---|---:|---:|
| baseline | 57232 | 0 |
| `e9 + s4e0` (landed) | 57232 | **0** |
| `e9` alone | 57248 | +16 |
| `s4e0` alone | 57208 | -24 |
| `e9 + s4e0` on w9601 base | 57272 | +40 |

## 3. New hypotheses tried (the +40 hunt)

**(a) switch-scrutinee form is a real graded-weight lever, but not here.**
`__builtin_expect_with_probability` does not exist in clang 7.0.2 (verified:
`use of undeclared identifier`).  `switch (__builtin_expect(x, C))` *does*
produce graded `!prof` weights (`case C` = 2000, all others = 1), unlike
`expect(x == C, 1)` on an `if` guard which makes the whole cumulative chain
hot.  Rewriting the case-154 `if/else if` chain as
`switch (__builtin_expect(m_nState, 9)) { case 0..10 }` is **-40 B**
(57,192 B) and reorders the mission cases; the plain `switch (m_nState)` form
is **-52 B** (57,180 B) but also diverges (the original's case-154 dispatch is
a compare chain starting `cmp r0,#0; bne`).  Neither is landable.

**(b) the `expect(x,C)==C` / `C==expect(x,C)` forms fold to the same code**
as `expect(x==C,1)`; `expect(x,C)!=C-1` does not (it gives a different size).
`expect(field_0xaa,0)` vs `expect(field_0xaa!=0,0)` also differ by 4 B even
though they are semantically identical — a useful +4/-4 companion knob.

**(c) order-preserving alternative (measured, not landed).**
`expect(m_nState == 10, 1)` (case-154 state 10) + `expect(m_nScriptTimerB >=
25001, 1)` (case-157 state 5 timer) is also size-exact 57,232 B, keeps the
original mission case order, puts c154 before the strong arm (marker gap
`0x48c`, not immediate), and gives raw 54,813 (+21) / aligned 17,481
(**-124**).  It was not landed because the target is the *immediate*
adjacency; it is the best fallback if the case-order reorder is unacceptable.

**(d) mission-switch scrutinee.** `switch (__builtin_expect(
getCurrentCampaignMission(), 158))` restores the original mission case order
with immediate adjacency when added to `e9+s4e0`, but costs +492 B (57,724 B)
because it turns the mission compare chain into a jump table.

## 4. Known cost: the mission case entries reorder

As w9601 found, the `e9` weight turns the case-154 state chain into a
weighted compare chain and the mission-case block placement follows.  Landed
order:

```
ours: 158,131,64,145,135,144,142,139,69,73,67,70,29,41,80,125,56,92,157,40,154,102,24,14,114,42,105,21,91,94
orig: 158,131,64,135,145,142,144,139,69,73,67,70,29,41,80,56,125,92,157,40,102,154,24,14,21,42,91,94,105,114
```

Adjacent pairs (135/145, 142/144, 56/125, 102/154) swap; 114/42 and 105/21
move.  This is the source of the raw/align regression; the c154→strong region
itself now matches the original.  The source `case` order is untouched.

## 5. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9705/fdiff.py _ZN11LevelScript7processEi       # 55142 / 57232
python3 /dev/shm/w9705/align_diff.py _ZN11LevelScript7processEi # 22383
cmake --build cmake-build-match --target check -j6              # RATCHET PASS
```

`/dev/shm/w9705/probe.py` locates the c154 marker and the strong arm;
`sweep.py` / `batch.py` are the patch+build+measure harnesses; `mkswitch.py`
generates the case-154 switch form; `order.py` prints the mission case order.

## 6. Open questions

1. Restore the original mission case order while keeping the immediate
   adjacency and 57,232 B.  The only shape found that does all three is
   `e9+s4e0+expect(mission,158)`, which is +492 B.
2. The c154 completion tail still uses the wave-148 `m_nState = 6` hack
   (`movs r0,#6; str.w r0,[sl,#28]`, 6 B) instead of the original's
   `m_nState++` (`ldr; adds; str`, 10 B).  `m_nState++` is +4 B but changes
   the raw metric to 54,828 and was not combined here (it would need a new
   -4 B companion).
