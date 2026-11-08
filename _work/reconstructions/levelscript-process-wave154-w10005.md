# LevelScript::process — wave 154 / w10005: case-125 else-if + case-42
# reorder + case-142 companion landed at 57,232 B; the span-96
# `expect(m_nState == 0, 1)` entry is blocked by a +4 B `.ARM.extab` growth

Status: **code change landed**, 2026-11-08.  Target
`_ZN11LevelScript7processEi` (`LevelScript::process`), original `0x137d18`,
**57,232 B (0xdf90)**, size-exact.  Owned TU `src/game/world/LevelScript.cpp`.
Worktree `wt/w10005`, base master `4417db0b4` (wave 153, w9904 landed).
Scratch in `/dev/shm/w10005/`.

## Result (landed)

```
                         before (HEAD)     after (landed)
process raw diffbytes    55,328            55,314            (-14)
process aligned insns    17,185            17,304            (+119)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
.ARM.extab size          0x26298           0x26298           (exact)
.rel.plt                 0                 0                 **BYTE-IDENTICAL**
.rodata / .data / .bss   exact             exact
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
RATCHET                  PASS              PASS
```

## The landed change (5 source edits)

1. **case 125, state-0/1 `else-if`** (line ~3924).  The original entry at
   `0x139f9c` is `cmp state,#0; bne.w 13b43e` where `0x13b43e` is the
   state-1 test, and the state-0 body ends with `b.w 13b518` (the state-2
   test).  That is `if (s==0 && trig) {...} else if (s==1) {...}` followed
   by the independent `if (s==2)`.  Making only the state-0/1 pair an
   `else-if` shrinks the case-125 entry span from **1022 B to 88 B**
   (original 98 B).
2. **case 42 arm order `6,0,7` -> `0,6,7`** (state-0 block moved first).
   The original's entry test is state 0 first (`0x13c1a4: cbnz r0; ...
   inAlienOrbit`).  Semantics-preserving (mutually exclusive arms).
3. **case 42 state-0 `m_nState = 1` -> `m_nState++`** (state is 0).
4. **case 142 state-0 `m_nState = 1` -> `m_nState++`** (state is 0).
5. **case 157 state-7 timer `if (m_nScriptTimerA <= 9999)` ->
   `__builtin_expect(..., 1)`** (line 4929).  This is the -8 B size
   companion that closes the +8 B left by edits 1-4 on the w9904 base.

Edits 2-5 are the w9906 composition (case-42 reorder + case-142 companion)
plus the -8 B companion; edit 1 is the case-125 faithfulness fix.

## Case-entry table (landed)

```
case      orig     ours  dEntry   spanO   spanU    dSpan
158     138862   13884a     -24     586     608       +22
131     138aac   138ab0      +4     426     428        +2
64      138c56   138c5e      +8    1386    1250      -136
135     1391c0   139140    -128     310     310         0
145     1392f6   139276    -128     590     534       -56
142     139544   139490    -180     156     192       +36
144     1395e0   139554    -140     124     124         0
139     13965c   1395d0    -140     196     174       -22
69      139720   13967c    -164     276     276         0
73      139834   139790    -164      94      94         0
67      139892   1397ee    -164     272     258       -14
70      1399a2   13992c    -118     768     346      -422
0       139ac2   139ac2       0     480     480         0
29      139ca2   139a4c    -598     180     206       +26
41      139d56   139b1a    -572     222     254       +32
80      139e34   139c18    -540     224     228        +4
56      139f14   139cfc    -536     136      70       -66
125     139f9c   139d42    -602      98      88       -10
92      139ffe   139d9a    -612     778     770        -8
157     13a308   13a09c    -620     464     524       +60
40      13a4d8   13a2a8    -560     384     348       -36
102     13a658   13a404    -596     278     134      -144
154     13a76e   13a48a    -740     450     160      -290
24      13a930   13a52a   -1030     130     182       +52
14      13a9b2   13a5e0    -978     370     342       -28
21      13ab24   13a736   -1006     134     134         0
42      13abaa   13a7bc   -1006     152     154        +2
91      13ac42   13a856   -1004     106     114        +8
94      13acac   13a8c8    -996     504     532       +28
105     13aea4   13aadc    -968     314     264       -50
114     13afde   13abe4   -1018      54      54         0
16      13b014   13ac1a   -1018   40482   41540     +1058
order: 158,131,64,135,145,142,144,139,69,73,67,70,0,29,41,80,56,125,92,157,
       40,102,154,24,14,21,42,91,94,105,114,16
```

The case-entry order is identical to the original.  Case 42 is 154 B vs the
original 152 B (was 156 B before the reorder).

## The span-96 faithful entry is blocked by `.ARM.extab`

`w9906`'s `expect(m_nState == 0, 1)` + `else-if` shape (case-125 span **96**,
state-0 body inline) is the faithful entry: the original's entry is
`cmp state,#0; bne.w state1; isTriggered; beq.w state1; <state-0 body
inline>; b.w state2`.  Measured on the current w9904 base it is
**57,344 B (+112)**, aligned **17,417**, span 96.

The blocker is not the code size but the **exception table**: `E+I` grows
`.ARM.extab` from `0x26298` to `0x2629c` (+4 B).  `.rodata` is 16-aligned,
so the +4 B shifts `.rodata` from `0x1c9e10` to `0x1c9e20` (+0x10), which
moves every literal pool in the image and drops `byte_exact` **4507 ->
4044** (measured).  `E` alone and `I` alone each keep `.ARM.extab` at
`0x26298`; only the combination grows it.

Measured `.ARM.extab` sizes (all built + linked):

| case-125 shape | size | extab | span | aligned |
|---|---:|---:|---:|---:|
| base (independent ifs) | 57,232 | 0x26298 | 1022 | 17,185 |
| `E` alone (`expect(state==0,1)`) | 57,248 | 0x26298 | 1022 | 17,342 |
| `I` alone (`else-if`) | 57,256 | 0x26298 | 88 | 17,396 |
| **`E+I` (faithful, span 96)** | **57,344** | **0x2629c** | **96** | **17,417** |
| `E` on trig (`expect(trig,1)`) + `I` | 57,396 | 0x26298 | 92 | 16,385 |
| `E` on whole cond + `I` | 57,340 | 0x2629c | 778 | 17,363 |
| `E` with 0 (`expect(state==0,0)`) + `I` | 57,264 | 0x26298 | 940 | 19,989 |

The `expect(trig,1)` shape keeps `.ARM.extab` exact and has the best aligned
metric (16,385) but is +164 B and span 92.

## Size-companion sweep for `E+I` (all built + linked)

`E+I` is 57,344 (+112).  Single-statement removals on the w9904 base:

| removal | size | delta |
|---|---:|---:|
| state-0 `forceUpdate(30,false)` | 57,292 | -52 |
| `MatrixSetTranslation(*local,...)` block | 57,180 | -164 |
| `(*enemies)[i]->geometry->translate(0,wobble,0)` | 57,268 | -76 |
| `player->rotate(spin,spin,spin)` | 57,312 | -32 |
| `m_pCamera->setRumblePercentage(rumble,20)` | 57,288 | -56 |

The faithful mission-0 loop `(*enemies)[i]->geometry->setVisible(true)` is
**-100 B** on `E+I` (57,244) but rotates the whole-function register colour
and costs aligned (~21,800).  `E+I + loop + case-144 setStation revert +
m_nState++` reaches exactly 57,232 B with span 96 and `.rel.plt` 0, but
`.ARM.extab` is still `0x2629c`, so `byte_exact` drops to 4044.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10005/fdiff.py _ZN11LevelScript7processEi        # 55,314 / 57,232
python3 /dev/shm/w10005/align_diff.py _ZN11LevelScript7processEi  # 17,304
python3 /dev/shm/w10005/relplt.py                                 # 0
python3 /dev/shm/w10005/casetable.py                              # order matches
cmake --build cmake-build-match --target check -j6                # RATCHET PASS
```

## Open questions

1. **The span-96 entry needs a -4 B LSDA fix.**  `E+I` grows the process
   LSDA call-site table by 5 bytes (`c4 02` -> `c9 02` table length at
   `0x1c2af8`).  If a source shape keeps the state-0 body inline *and*
   keeps the LSDA at `0x26298`, `E+I` + the -112 companion lands the
   faithful entry.  The `expect(trig,1)` shape (span 92, extab exact,
   aligned 16,385) is the closest measured alternative and needs -164 B.
2. The aligned metric is dominated by the whole-function register colour.
   `E+I` alone is 17,417 on w9904 but **16,608 on the w9805 base** (the
   case-144 `setStation` is the switch).  Any size-fixing edit moves the
   colour and loses that.
3. `I+O+N+P` on the w9805 base (w9906's landed composition) is 57,232 B,
   aligned 17,365, `.ARM.extab` exact.  It is the same source as the landed
   set minus the s7-timer expect, but measured on the w9805 base (case-144
   `setStation` reverted, case-92 3-call restored).
