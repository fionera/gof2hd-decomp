# LevelScript::process — wave 152 / w9804: the w9704 `m_nState++` + else-arm
# `destroyed = 0` removal composes with w9707 at exactly 57,232 B

Status: **code change landed (byte-identical .text)**, 2026-11-08.  Target
`_ZN11LevelScript7processEi` (`LevelScript::process`), original `0x137d18`,
**57,232 B (0xdf90)**, size-exact.  Owned TU `src/game/world/LevelScript.cpp`.
Worktree `wt/w9804`, base master `9d491d46a` (wave 151).  Scratch in
`/dev/shm/w9804/`.

## Result (landed)

```
                         before (HEAD)     after (landed)
process raw diffbytes    55,320            55,320            (0)
process aligned insns    17,244            17,244            (0)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
whole .text diffbytes    87,271            87,271            (0)
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

`.text` is byte-identical to master (only `.note.android.ident` /
`.note.gnu.build-id` differ between two builds).  The landing is a
**source-faithfulness** change: the w9704 edit (drop the redundant else-arm
`destroyed = 0` store) is landed, but the zero-init is kept as a declaration
initializer so the code is byte-identical to the original's `movs r4,#0`.

## 1. The w9704 / w9707 composition is 57,228 B; the +4 companion is the
##    declaration initializer

w9704 (`385ad08c4`, wave 150) landed two edits on the wave-150 base:

1. case-154 state-9 completion `m_nState = 6` -> `m_nState++` (+4 B), and
2. case-157 state-5 else arm: drop the redundant `destroyed = 0;` (-4 B).

w9707 (`60dd96ab2`, wave 150) independently landed the *same* `m_nState++`
edit plus the faithful `messages, player, enemies` prologue (+4), the case-92
3-call `getPosition()` shape (+8) and the case-144 `setStation(getStation())`
removal (-16).  Composing w9704 with w9707 (i.e. master + the else-arm store
removal) is therefore

```
prologue +4, case-92 +8, case-144 setStation -16,
m_nState++ +4, destroyed-removal -4   =  -4   ->  57,228 B
```

The mission's +4 companion is the zero-initializer itself.  Keeping it as a
declaration initializer (`int destroyed = 0;`) instead of the else-arm store
is +4 B **and compiles to exactly the same code as the original's else-arm
`movs r4,#0`** (clang hoists it before the branch either way):

```cpp
int destroyed = 0;
if (Globals::isRunningHDonWeakDevice) {
    destroyed = 5;
    for (int i = 6; i < 12; i++) destroyed += (*enemies)[i]->isDead();
} else {
    for (int i = 11; i < 22; i++) destroyed += (*enemies)[i]->isDead();
}
```

Measured variants on the union base (all built + linked + measured):

| variant | size | raw | aligned |
|---|---:|---:|---:|
| union (else-arm store removed, no init) | 57,228 | 55,102 | 17,237 |
| **union + `int destroyed = 0;` decl-init (LANDED)** | **57,232** | **55,320** | **17,244** |
| union + case-157 state-9 `m_nState++` | 57,232 | 55,329 | 17,244 |
| union + case-157 state-6 `m_nState++` | 57,232 | 55,328 | 17,244 |
| union + case-158 state-1 `m_nState++` (0x1438d2) | 57,232 | 55,322 | 17,242 |
| union + case-158 `m_nState = 2` (0x1438d2 block) | 57,232 | 55,317 | 17,243 |
| union + case-144 `m_nState = 4` -> `++` | 57,236 | 55,292 | 17,262 |
| union + restore case-144 setStation, revert c92 + m_nState++ | 57,232 | 55,324 | 17,220 |

The decl-init is the only variant that is both **faithful** (the original does
zero `destroyed` on the non-weak path) and leaves `.text` byte-identical to
master.  The `m_nState++` alternatives are all unfaithful: the original uses
`movs r0,#N; str` at 0x1438d2 (case 158 s1->2), 0x13c872 (case 157 s8->9) and
0x144c06 (case 157 s5->6).  The `setStation`-restore variant has the best
aligned metric (17,220) but reverts the faithful case-92 3-call shape and the
faithful `m_nState++`.

## 2. Case-entry table (landed build)

Mission-switch dispatch on `getCurrentCampaignMission()`; entries are the
`cmp r0,#N; beq.w` / TBH targets.  `span` is the distance to the next entry in
address order (the compiler scatters cold blocks, so span is only indicative).

```
case      orig     spanO     ours     spanU   dSpan
 158   138862      586   138856      606     +20
 131   138aac      426   138ab4      428      +2
  64   138c56     1386   138c60     1256    -130
 135   1391c0      310   139148      310      +0
 145   1392f6      590   13927e      534     -56
 142   139544      156   139494      188     +32
 144   1395e0      124   139550      124      +0
 139   13965c      196   1395cc      174     -22
  69   139720      276   13967a      276      +0
  73   139834       94   13978e       94      +0
  67   139892      272   1397ec      258     -14
  70   1399a2      288   1398ee      346     +58
   0   139ac2      480        -        -       -    (mission 50/51 shared body)
  29   139ca2      180   139a48      206     +26
  41   139d56      222   139b16      254     +32
  80   139e34      224   139c14      228      +4
  56   139f14      136   139cf8       70     -66
 125   139f9c       98   139d3e     1022    +924
  92   139ffe      778   13a13c      748     -30
 157   13a308      464   13a428      524     +60
  40   13a4d8      384   13a634      348     -36
 102   13a658      278   13a790      134    -144
 154   13a76e      450   13a816      160    -290
  24   13a930      130   13a8b6      182     +52
  14   13a9b2      370   13a96c      342     -28
  21   13ab24      134   13aac2      134      +0
  42   13abaa      152   13ab48      156      +4
  91   13ac42      106   13abe4      114      +8
  94   13acac      504   13ac56      530     +26
 105   13aea4      314   13ae68      264     -50
 114   13afde    40536   13af70    40748    +212
```

The case-144 entry span is **exact** (124 B).  The case-144 state-3 body is
16 B short (the original's `setStation(getStation())` at 0x13d684 is still
removed by w9707); that is compensated inside case 114's scattered body.

## 3. Why the raw metric does not improve

The union (57,228) has raw 55,102 because the -4 B shift happens to re-align
the whole post-0x144400 tail.  Any +4 B companion restores the 57,232 layout
and the raw metric returns to ~55,320.  The decl-init form is byte-identical
to master, so both metrics are unchanged; the win is the landed source edit
and the documented case-entry table.

## 4. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9804/fdiff.py _ZN11LevelScript7processEi        # 55,320 / 57,232
python3 /dev/shm/w9804/align_diff.py _ZN11LevelScript7processEi  # 17,244
cmake --build cmake-build-match --target check -j6               # RATCHET PASS
```

## 5. Open questions

1. The 4-byte residual is **not** an EH-cleanup instruction (corrected in
   the wave-152 continuation below): both the original and master have the
   same `b.n 145c64; mov sl/r9, r0` landing-pad pair at 0x145c60/0x145c62.
   The union's -4 is a *uniform cold-region shift* starting at the destroyed
   block: its landing pads are master's shifted by -4
   (0x145bf0..0x145c68 vs 0x145bf0..0x145c6c) and its literal pool reorders
   the five float constants (`0.8f, 3000.0f, 6500.0f, 25.0f, 1.7f` ->
   `3000.0f, 6500.0f, 25.0f, 1.7f, 0.8f`) and changes the last PC-relative
   literal by 0xc.  The union's lower raw (55,102) is that re-layout
   accidentally matching more of the original's cold tail; it is not a
   missing source instruction.
2. The case-144 state-3 body is still 16 B short (w9707's `setStation`
   removal).  Restoring it (+16 B) needs a -12 B companion elsewhere; the
   case-92 3-call revert (-8) plus the case-154 `m_nState++` revert (-4) is
   one such pair and lands at 57,232 with aligned 17,220 (see table above).
3. Case 125 is still 1022 B inline vs the original's 98 B outlined entry
   (w9602/w9706); this is the largest single span delta.

## 6. Wave-152 continuation: `__builtin_expect` sweep on the destroyed block

New hypothesis tried after the landing: the original is PGO-weighted, so a
source-level `__builtin_expect` on the destroyed-block guards may reproduce
the original's hot/cold layout.  All rows below are built, linked and
measured from the landed source (`int destroyed = 0;`, no else-arm store);
`weak` = `Globals::isRunningHDonWeakDevice`, `timer` =
`m_nScriptTimerB >= 25001`, `bigcond` = the `(destroyed >= 9 && ...) ||
...getHitpoints() < getMaxHitpoints()/4` condition.

| variant | size | raw | aligned |
|---|---:|---:|---:|
| **landed (no expect)** | **57,232** | **55,320** | **17,244** |
| `expect(weak, 0)` | 57,224 | 55,520 | 17,311 |
| `expect(weak, 1)` | 57,224 | 55,521 | 17,220 |
| `expect(timer, 0)` | 57,240 | 55,423 | 17,245 |
| `expect(timer, 1)` | 57,236 | 55,285 | 17,246 |
| `expect(rocket, 0)` | 57,244 | 55,434 | 17,251 |
| `expect(rocket, 1)` | 57,232 | 55,320 | 17,244 (no-op) |
| `expect(bigcond, 0)` | 57,228 | 54,793 | 17,384 |
| `expect(bigcond, 1)` | 57,232 | 55,320 | 17,244 (no-op) |
| `expect(weak,1) + expect(timer,0)` | 57,232 | 55,336 | 17,232 |
| `expect(weak,1) + expect(timer,1)` | 57,232 | 55,347 | **17,222** |
| `expect(bigcond,0) + expect(timer,1)` | 57,232 | 55,215 | 17,393 |
| union (store removed, no init) + `expect(timer,1)` | 57,232 | 55,326 | 17,244 |
| union + `expect(weak,1)` | 57,224 | 55,502 | 17,220 |
| `expect(weak,1)` + restore c144 setStation + revert c92 3-call | 57,232 | 55,328 | **17,185** |

The best aligned size-exact variant is `expect(weak,1)` + restore the
case-144 `setStation(getStation())` + revert the case-92 3-call shape
(17,185, -59 vs landed) at raw 55,328 (+8); it trades two of w9707's
faithfulness fixes for alignment.  Among variants that keep every w9707
fix, the best is `expect(weak,1) + expect(timer,1)` (17,222, -22) at raw
55,347 (+27).  The best raw size-exact
variant is `expect(bigcond,0) + expect(timer,1)` (55,215, -105) at aligned
17,393 (+149).  Neither dominates: the aligned win comes from speculative
branch weights (`expect(weak,1)` says "weak device is likely";
`expect(timer,1)` says the 25 s timer has expired, which is false ~99.9% of
frames), so the conservative byte-identical landing was kept.  This sweep is
the new lead for the next wave: the weights are free parameters, and the
size responds in 4/8-byte quanta.

### Corrected EH-tail analysis (replaces old open question 1)

`master.so` and the original both place the landing pads at
0x145bf0..0x145c6c; `master` 0x145c34 is `add r0, sp, #124` while the
original is `mov r0, r6` (same 4 B), and the only systematic difference is
the exception-pointer register (`r9` vs `sl`) — a register colouring, not a
size difference.  The union's landing pads are master's shifted by -4
(0x145bf0..0x145c68), and its literal pool reorders the five float constants
and changes the final PC-relative literal by 0xc.  So the union's 55,102 raw
is a cold-tail re-layout coincidence, not evidence of a missing instruction.
