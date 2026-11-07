# LevelScript::process — wave 140 / w8503 (re-land w8408 two-chain; .rel.plt rotation
# investigated: moving the mission-125 hacking block earlier restores .rel.plt only at +200 B)

Status: **code change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, size-exact.
Owned TU: `src/game/world/LevelScript.cpp`. Base master `d4857f174` (wave-139 head).
Scratch helpers in `/dev/shm/w8503/` (not tracked): `fdiff.py`, `sections.py`,
`relplt.py`, `gen.py`, `gen2.py`, `sweep.sh`, `measure.sh`, `insndiff.py`.

## Result

```
                         before (HEAD)     after (landed)
process raw diffbytes    54,262            54,595            (+333)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
whole .text diffbytes    84,651            84,984            (+333)
.text size               1,259,464         1,259,464         (exact)
.ARM.extab size          156,312           156,312           (exact; table 0x146)
.ARM.exidx               exact             exact
.rodata / .data / .bss   exact             exact
.rel.plt                 0 (byte-identical) 12 bytes (ROTATED)
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
RATCHET                  PASS              PASS
```

The source is exactly the w8408 landing (`f5f6b5d20`): the faithful case-125
two-chain (`if/else-if` state 0/1, then `if/else-if` state 2/3/4..6) plus the
case-91 nested `if (m_nState == 0) { if (isTriggered()) ... }` test that keeps
the LSDA call-site table at 0x146 and `.ARM.extab` at 156,312 B.

## The .rel.plt rotation (the new w8503 question)

`.rel.plt` differs in exactly one rotated group of six entries:

```
entry  original                       w8408 / landed
2830   PlayerEgo::setDockingState     PlayerEgo::setDockingState
2831   StarSystem::switchPlanetForIntro  PlayerEgo::deleteHackingGame   <-- rotated
2832   Level::switchSkyboxForIntro    StarSystem::switchPlanetForIntro
2833   ParticleSystemManager::reset   Level::switchSkyboxForIntro
2834   PlayerEgo::startSmokeEmission  ParticleSystemManager::reset
2835   PlayerStation::setVisible      PlayerEgo::startSmokeEmission
2836   PlayerEgo::deleteHackingGame   PlayerStation::setVisible
```

`.rel.plt` is emitted in first-call-site order, and the call sites confirm it:

| symbol | original | w8408 |
|---|---|---|
| `setDockingState` | 0x13cd8e | 0x13cd08 |
| `deleteHackingGame` | **0x13ded4** | **0x13d114** |
| `switchPlanetForIntro` | 0x13d1ec | 0x13d1a8 |
| `switchSkyboxForIntro` | 0x13d1f4 | 0x13d1b0 |
| `ParticleSystemManager::reset` | 0x13d212 | 0x13d1ce |
| `startSmokeEmission` | 0x13d396 | 0x13d6f2 |
| `PlayerStation::setVisible` | 0x13da1e | 0x13daa4 |

The two-chain makes MachineBlockPlacement lay the case-125 `state 3` + mission-125
hacking (`default`) pair **before** the case-42 cold blocks, so
`deleteHackingGame` (0x13d114) precedes `switchPlanetForIntro` (0x13d1a8).
In the original the pair is placed **after** case-42 state 7 (`setVisible`
0x13da1e), at 0x13dcb6 / 0x13de44.

## The hypothesis tested: move the hacking block earlier in the source

Moving the mission-125 hacking block **earlier in the source** does move it
**later in `.text`** (MBP chain-order effect), exactly as the handoff predicted:

```cpp
// landed (w8408): hacking block is the else-if tail of state 3
if (m_nState == 2) { ... }
else if (m_nState == 3) { ... }
else if (m_nState >= 4 && ...) { hacking }        // -> 0x13d084, .rel.plt rotated

// tested: hacking block hoisted before state 3 (separate chain)
if (m_nState == 2) { ... }
if (m_nState >= 4 && ...) { hacking }             // -> 0x13e530, .rel.plt EXACT
else if (m_nState == 3) { ... }
```

The hoisted form gives `.rel.plt` diff **0** (deleteHackingGame moves to entry
2836, after `setVisible`), but the function grows to **57432 B** and `.text` to
1,259,688 B (+224).  That shifts every later function and drops byte_exact to
**2741** (RATCHET FAIL), so it is not landable while the size must stay 57232.

The +200 is not extra instructions: the hoisted build has 20,674 instructions vs
20,697 for the landed build, but ~100 short `b.n` become 4-byte `b.w`/literal-pool
padding because the cold blocks are spread further apart.

## Variants measured (all with the case-91 nested test)

`spec` = chain layout of the case-125 second chain; `|` starts a new chain,
`:0` = `if`, `:1` = `else if`.

| spec | process size | raw diff | .text size | .rel.plt diff |
|---|---:|---:|---:|---:|
| `s2:0,s3:1,s4:1` (**landed**) | **57232** | **54595** | 1259464 | **12** |
| `s3:0,s2:1,s4:1` | 57232 | 54588 | 1259464 | 12 |
| `s3:0,s4:1,s2:1` | 57224 | 55156 | 1259464 | 13 |
| `s4:0,s2:1,s3:1` | 57244 | 55208 | 1259496 | 13 |
| `s4:0,s3:1,s2:1` | 57244 | 55206 | 1259496 | 13 |
| `s2:0,s4:1,s3:1` | 57328 | 55414 | 1259560 | 13 |
| `s2:0\|s4:0,s3:1` | 57432 | 55299 | 1259688 | **0** |
| `s2:0\|s3:0,s4:1` | 57436 | 55273 | 1259688 | **0** |
| `s2:0\|s3:0\|s4:0` | 57432 | 55287 | 1259688 | **0** |
| `s0:0,s1:1,s4:1\|s2:0,s3:1` | 57468 | 54783 | 1259720 | 13 |
| `s0:0,s1:1,s4:1\|s2:0\|s3:0` | 57292 | 55072 | 1259528 | 13 |
| `s4:0,s0:1,s1:1\|s2:0,s3:1` | 57520 | 54507 | 1259752 | 34 |
| hybrid `if/else-if(1)` + `switch(2,3,default)` | 57188 | 55172 | 1259432 | 12 |
| case-125 moved before case 42 | 57232 | 54598 | 1259464 | 12 |
| case-125 moved before case 92 | 57232 | 54597 | 1259464 | 12 |
| case-42 moved before case 40 | 57232 | 54595 | 1259464 | 12 |
| case-42 states reordered 7,6,0 | 57232 | 54582 | 1259464 | 18 |

Every size-exact variant keeps the rotation; every `.rel.plt`-exact variant is
+200 B (or more).  The case-42 reorder `7,6,0` moves `setVisible` too early
(before `isInWormhole`/`freeMissionLock`/`switchSun`), so it breaks three more
entries.

## Conclusion / open question

The w8408 source is re-landed faithfully at exactly 57,232 B and the full gate is
green, but `.rel.plt` is rotated by 12 bytes.  Restoring it needs the mission-125
hacking block placed after case-42 state 7, which costs +200 B on this base.
The next lever is a faithful **-200 B companion** (or a different MBP nudge that
moves the `state 3` + hacking pair after case-42 without lengthening branches).
The `s3:0,s2:1,s4:1` reorder is a size-neutral raw-diff improvement (54595 ->
54588) but does not touch `.rel.plt`.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w8503/fdiff.py _ZN11LevelScript7processEi
python3 /dev/shm/w8503/relplt.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
```
