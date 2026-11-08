# LevelScript::process — wave 152 / w9906: case-125 composition on top of w9805

Target `_ZN11LevelScript7processEi` (`LevelScript::process`), original
`0x137d18`, **57,232 B (0xdf90)**, size-exact.  Owned TU
`src/game/world/LevelScript.cpp`.  Worktree `wt/w9906`, base master
`4a6104a43` (wave 152, w9805 landed).  Scratch in `/dev/shm/w9906/`.

## Result (landed)

```
                         HEAD (w9805)      landed (this pass)
process raw diffbytes    55,347            55,343            (-4)
process aligned insns    17,223            17,365            (+142)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
.ARM.extab diffbytes     1,324             1,428             (+104)
.text diffbytes          87,131            87,127            (-4)
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

The landed change is the w9806 composition (w9806's note was committed as a
note only; its source was never on master):

1. **case 125** state-0/1 `else-if`: `if (m_nState == 1)` ->
   `else if (m_nState == 1)` (line ~3924).  Case-125 entry span 1032 -> 88 B.
2. **case 42** arm order `6,0,7` -> `0,6,7` (state-0 block moved first).
3. **case 42** state-0 `m_nState = 1` -> `m_nState++`.
4. **case 142** state-0 `m_nState = 1` -> `m_nState++` (the -4 B companion
   that closes the +4 B left by edits 1-3).

## Case-entry table (landed)

```
case      orig     ours  dEntry   spanO   spanU    dSpan
158     138862   138862      +0     586     594       -8
131     138aac   138ab4      -8     426     428       -2
64      138c56   138c60     -10    1386    1256     +130
135     1391c0   139148    +120     310     310       +0
145     1392f6   13927e    +120     590     534      +56
142     139544   139494    +176     156     192      -36
144     1395e0   139554    +140     124     124       +0
139     13965c   1395d0    +140     196     174      +22
69      139720   13967e    +162     276     276       +0
73      139834   139792    +162      94      94       +0
67      139892   1397f0    +162     272     258      +14
70      1399a2   1398f2    +176   49926   50102     -176
0       139ac2   13bd22   -8800     480   -8918    +9398
29      139ca2   139a4c    +598     180     206      -26
41      139d56   139b1a    +572     222     254      -32
80      139e34   139c18    +540     224     228       -4
56      139f14   139cfc    +536     136      70      +66
125     139f9c   139d42    +602      98      88      +10
92      139ffe   139d9a    +612     778     774       +4
157     13a308   13a0a0    +616     464     524      -60
40      13a4d8   13a2ac    +556     384     348      +36
102     13a658   13a408    +592     278     134     +144
154     13a76e   13a48e    +736     450     160     +290
24      13a930   13a52e   +1026     130     182      -52
14      13a9b2   13a5e4    +974     370     342      +28
21      13ab24   13a73a   +1002     134     134       +0
42      13abaa   13a7c0   +1002     152     154       -2
91      13ac42   13a85a   +1000     106     114       -8
94      13acac   13a8cc    +992     504     532      -28
105     13aea4   13aae0    +964     314     264      +50
114     13afde   13abe8   +1014      54      54       +0
16      13b014   13ac1e   +1014   44180   45194    -1014
order: 158,131,64,135,145,142,144,139,69,73,67,70,0,29,41,80,56,125,92,157,40,102,154,24,14,21,42,91,94,105,114,16
```

The case-entry order is identical to the original.  Case 125 is 88 B vs the
original 98 B (the 10 B gap is the outlined state-0 body: the original keeps
the state-0 body inline at the case entry and outlines the state-1 test; the
`if (s==0 && trig) ... else if (s==1)` form outlines the state-0 body).

## The near-miss: `__builtin_expect(m_nState == 0, 1)` keeps state-0 inline

The original entry is `cmp state,#0; bne.w state1; ... isTriggered; beq.w
state1; <state-0 body inline>; b.w state2`.  Adding
`__builtin_expect(m_nState == 0, 1)` to the state-0 guard reproduces that
shape almost exactly:

```
ours e125x0 entry 0x139d88:
  ldr.w r1,[r9,#28]; cmp r1,#0; bne.w 13c1a8
  ... isTriggered ...; cmp r0,#0; beq.w 13c1a8
  <state-0 body inline>; b.w 13c272
```

Measured (all built/linked, `.rel.plt` checked):

| variant | size | raw | aligned | relplt | c125 span |
|---|---:|---:|---:|---:|---:|
| **landed full w9806 set** | **57232** | **55343** | **17365** | **0** | **88** |
| e125 only | 57256 | 55429 | 17383 | 0 | 88 |
| e125+ord067 | 57236 | 55419 | 17346 | 0 | 88 |
| e125+ord067+inc42 | 57236 | 55422 | 17343 | 0 | 88 |
| e125+ord067+inc42+e144 | 57224 | 55492 | 17346 | 0 | 88 |
| e125+ord067+inc42+e144+aa2 | 57232 | 55417 | 17352 | 0 | 88 |
| full+e144+aa+aa2 | 57232 | 55389 | 17350 | 0 | 88 |
| **e125x0** (`expect(state==0,1)`) | **57360** | **55488** | **16608** | **0** | **96** |
| e125xt (`expect(trig,1)`) | 57384 | 55321 | 16271 | 0 | 92 |
| e125n2 (nested `if(s==0){if(trig)}` + else-if) | 57384 | 55393 | 16334 | 0 | 102 |
| e125x0+ord607 | 57360 | 55488 | 16608 | 0 | 96 |
| e125x0+ord607+e142 | 57344 | 55464 | 16648 | 0 | 96 |
| e125x0+ord067 | 57280 | 55459 | 17622 | 4 | 94 |
| e125x0+s2 (`expect(state==2,1)`) | 57168 | 55373 | **17201** | 13 | 96 |
| e125n2+s2 | 57156 | 55408 | 17048 | 12 | 102 |

The `e125x0` shape is the faithful one (span 96 vs 98, state-0 body inline)
and improves aligned by 615, but is **+128 B**.  No size-exact companion was
found in this pass.  `e125x0+s2` even reaches aligned **17,201** (better than
the 17,223 baseline) but is **-64 B** and rotates 13 bytes of `.rel.plt`
(the state-2 weight reorders the mission cases, the same effect w9805
documented for state-guard weights).

## Negative results

* `ord076` (the original's literal 0,7,6 test order) gives aligned 17,310 but
  rotates 18 bytes of `.rel.plt`; `ord670` gives 17,264 but rotates 4.
  Only `ord067` leaves `.rel.plt` byte-identical.
* `e125x0` composes badly with the case-42 reorder: `x0+ord067` rotates 4
  bytes of `.rel.plt` and worsens aligned to 17,622.
* Flipping the w9805 inner-condition weights (`c154done0`, `weak0`,
  `s5timer0`) on top of `e125x0` changes size by 0 (the levers interact with
  the case-125 block layout).
* `expect(field_0xaa,0)` on the case-154 state-0/9 auto-turret guards is +4 B
  each in this shape; used as the +4 companion in the `e144+aa2` variant.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9906/fdiff.py _ZN11LevelScript7processEi        # 55343 / 57232
python3 /dev/shm/w9906/align_diff.py _ZN11LevelScript7processEi  # 17365
python3 /dev/shm/w9906/relplt.py                                 # .rel.plt 0
python3 /dev/shm/w9906/table.py                                  # case table
cmake --build cmake-build-match --target check -j6               # RATCHET PASS
```

## Open questions

1. The `e125x0` shape (span 96, aligned 16,608) needs a **-128 B** companion
   to land at 57,232.  The `e125x0+s2` shape (aligned 17,201, -64 B) needs a
   +64 B companion **and** a `.rel.plt` fix (13 bytes rotated).
2. The +128 B in `e125x0` is not in the state-0/1 bodies (they are -2/-16 vs
   the original); it is in the state-2/tail block placement.  The case-0
   body is placed at `0x13bd22` vs the original `0x139ac2` in this shape.
3. `ord076` is the original's literal test order but rotates nine `.rel.plt`
   entries; a statement-level reorder in the state-6/7 bodies that restores
   the original first-reference order would let it land.
