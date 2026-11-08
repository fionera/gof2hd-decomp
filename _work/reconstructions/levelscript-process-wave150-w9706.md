# LevelScript::process — wave 150 / w9706: case-125 state-0/1 else-if composes
# with w9602 at exactly 57,232 B (`.rel.plt` 0), case-125 entry span 90 B

Status: **code change landed**, 2026-11-08.  Target
`_ZN11LevelScript7processEi` (`LevelScript::process`), original `0x137d18`,
**57,232 B (0xdf90)**, size-exact.  Owned TU `src/game/world/LevelScript.cpp`.
Worktree `wt/w9706`, base master `db9658a51` (wave 150).  Scratch in
`/dev/shm/w9706/`.

## Result (landed)

```
                         before (HEAD)     after (landed)
process raw diffbytes    54,792            54,918            (+126)
process align_diff       17,605            17,638            (+33)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
.ARM.extab               exact             exact
.ARM.exidx               exact             exact
.rel.plt                 0 bytes           0 bytes           **BYTE-IDENTICAL**
.rodata / .data / .bss   exact             exact
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
RATCHET                  PASS              PASS
```

The mission target is met: the case-125 `else-if` composition lands at
**exactly 57,232 B** with `.rel.plt` still byte-identical (0 differing
bytes), and the case-125 entry span drops from **1032 B to 90 B** (original
98 B).  The raw and aligned metrics are essentially flat (+126 / +33); the
win is the case-entry-table faithfulness, not the byte count.

## The landed change (4 source edits)

1. **case 125, state-0/state-1 `else-if`** (line ~3924).  The original entry
   at `0x139f9c` is `cmp state,#0; bne.w 13b43e` where `0x13b43e` is the
   state-1 test, and the state-0 body ends with `b.w 13b518` (the state-2
   test).  That is exactly `if (s==0 && trig) {...} else if (s==1) {...}`
   followed by the independent `if (s==2)`.  Making only the state-0/1 pair
   an `else-if` (states 2, 3, default stay independent, preserving w9602's
   `.rel.plt` fix) shrinks the case-125 entry span to 90 B.

2. **case 42 arm order 0, 6, 7** (was 6, 0, 7).  The original's entry test is
   state 0 first (`0x13c1a4: cbnz r0; ... inAlienOrbit`).  Moving the state-0
   arm to the front is worth -20 B in the composed shape and keeps every
   `.rel.plt` entry in place.

3. **case 42 state-0 `m_nState = 1` -> `m_nState++`** (state is 0, so `++`
   is semantically identical).  -8 B in the composed shape.

4. **case 154 state-9 completion `m_nState = 6` -> `m_nState++`** (the
   original at `0x144a08` is `ldr.w r0,[sl,#28]; adds r0,#1; str.w
   r0,[sl,#28]`).  +4 B in this shape; it is the faithful instruction and is
   the "+4 companion" that closes the last 4 bytes.

Only the first edit is a faithfulness fix; edits 2-4 are the size-neutral
composition levers required to keep the function exactly 57,232 B.

## Case-entry table (landed)

```
case      orig     ours  dEntry   spanO   spanU    dSpan
158     138862   138862      +0     586     602      -16
131     138aac   138abc     -16     426     428       -2
64      138c56   138c68     -18    1386    1248     +138
135     1391c0   139148    +120     310     310       +0
145     1392f6   13927e    +120     590     534      +56
142     139544   139494    +176     156     196      -40
144     1395e0   139558    +136     124     124       +0
139     13965c   1395d4    +136     196     170      +26
69      139720   13967e    +162     276     276       +0
73      139834   139792    +162      94      94       +0
67      139892   1397f0    +162     272     308      -36
70      1399a2   139924    +126   49926   50052     -126
0       139ac2   13bd32   -8816     480   -8942    +9422
29      139ca2   139a44    +606     180     208      -28
41      139d56   139b14    +578     222     254      -32
80      139e34   139c12    +546     224     228       -4
56      139f14   139cf6    +542     136     130       +6
125     139f9c   139d78    +548      98      90       +8
92      139ffe   139dd2    +556     778     762      +16
157     13a308   13a0cc    +572     464     466       -2
40      13a4d8   13a29e    +570     384     346      +38
102     13a658   13a3f8    +608     278     134     +144
154     13a76e   13a47e    +752     450     228     +222
24      13a930   13a562    +974     130     148      -18
14      13a9b2   13a5f6    +956     370     344      +26
21      13ab24   13a74e    +982     134     134       +0
42      13abaa   13a7d4    +982     152     158       -6
91      13ac42   13a872    +976     106     114       -8
94      13acac   13a8e4    +968     504     540      -36
105     13aea4   13ab00    +932     314     256      +58
114     13afde   13ac00    +990   40536   41638    -1102
```

Case 125 is now 90 B (was 1032 B), matching the original's 98 B to within
the 8-byte state-0 body codegen difference.  Case 42 is 158 B vs the
original 152 B.

## Swept variants (all built + linked + `.rel.plt`-checked)

Baseline composition (with w9602's independent-if 2,3,default already in
HEAD) is raw 54,792 / align 17,605 / 57,232 / relplt 0.

| variant | size | raw | align | relplt |
|---|---:|---:|---:|---:|
| **landed: ei01 + ord067 + inc42 + inc** | **57232** | **54918** | **17638** | **0** |
| ei01 (else-if 0/1 only) | 57256 | 55095 | 17675 | 0 |
| ei01 + inc154 | 57260 | 55115 | 17672 | 0 |
| ei01 + ord067 | 57236 | 55075 | 17647 | 0 |
| ei01 + ord067 + inc154 | 57240 | 54906 | 17654 | 0 |
| ei01 + ord067 + inc42 | 57228 | 54577 | 17635 | 0 |
| ei01 + ord067 + inc42 + inc67 | 57228 | 54576 | 17635 | 0 |
| ei01 + ord067 + inc42 + inc154 | **57232** | 54918 | 17638 | 0 |
| ei01 + ord067 + inc42 + inc67 + inc154 | **57232** | 54917 | 17638 | 0 |
| ei01 + ord076 (original test order 0,7,6) | 57240 | 54794 | 17621 | 18 |
| ei01 + ord076 + inc154 | 57244 | 54814 | 17608 | 18 |
| ei01 + ord670 (6,7,0) | 57236 | 55090 | 17680 | 4 |
| ei01 + ord706 | 57252 | 55044 | 18078 | 22 |
| ei01 + ord760 | 57256 | 55031 | 17801 | 18 |
| ei01 + ord607 (baseline order) | 57256 | 55095 | 17675 | 0 |
| ei01n (nested `if (s==0) { if (trig) }` else-if) | 57428 | 55051 | 16817 | 0 |
| ei12 (else-if 1/2) | 57324 | 55204 | 19101 | 12 |
| ei23 (else-if 2/3) | 57264 | 54838 | 16468 | 13 |
| ei34 (else-if 3/hacking) | 57212 | 55135 | 20469 | 14 |
| whole-chain else-if (0/1/2/3/default) | 57256 | 55395 | 19300 | 14 |
| inc154 alone (`m_nState++`) | 57236 | 54880 | 17602 | 0 |
| inc42 alone (case-42 `m_nState++`) | 57232 | 54799 | 17603 | 0 |
| inc67 alone (case-42 state-6 `m_nState++`) | 57232 | 54802 | 17605 | 0 |

The exhaustive 3x32 sweep over {ord067, ord076, ord607} x subsets of
{inc154, inc2, inc4, inc42, inc67} on top of `ei01` found exactly two
size-57,232 / relplt-0 combinations: `ei01+ord067+inc154+inc42` and
`ei01+ord067+inc154+inc42+inc67`.  The landed one is the first.

`ord076` is the original's literal test order (0, 7, 6) but it rotates nine
case-42 `.rel.plt` entries (18 bytes) because the state-6/7/0 block layout
changes; `ord670` flips only the target `setVisible`/`deleteHackingGame`
pair (4 bytes).  Only `ord067` leaves `.rel.plt` untouched.

## Why this is not a byte-count win

The case-125 entry span is now faithful (90 vs 98 B), but the state-0 body
is sunk to `0x13c1fe` while the original keeps it inline at `0x139f9c`; the
state-1 body takes its place inline.  That block swap costs the +126 raw /
+33 aligned.  A source shape that keeps the state-0 body inline (the nested
`ei01n` form) has a much better aligned metric (16,817) but is +196 B.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9706/fdiff.py _ZN11LevelScript7processEi        # 54918 / 57232
python3 /dev/shm/w9706/align_diff.py _ZN11LevelScript7processEi  # 17638
python3 /dev/shm/w9706/relplt.py                                 # 0
python3 /dev/shm/w9706/table.py                                  # case-entry table
cmake --build cmake-build-match --target check -j6               # RATCHET PASS
```

## Open questions

1. The state-0 body is sunk to `0x13c1fe` instead of staying inline at the
   case entry (`0x139f9c`).  A shape that keeps it inline while preserving
   the 90 B entry span would likely recover the +126 raw.
2. `ei01n` (nested form) has aligned 16,817 (vs 17,605 baseline) but is
   +196 B; finding the 196 B is the highest-value remaining lever.
3. `ord076` (original test order 0,7,6) rotates nine `.rel.plt` entries.
   A statement-level reorder in the state-6/7 bodies that restores the
   original first-reference order would let the faithful arm order land.
