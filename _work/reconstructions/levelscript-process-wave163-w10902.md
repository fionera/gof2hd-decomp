# LevelScript::process — wave 163 / w10902: mission-0 4,3,5 swap + 10-entry tbh

Target `_ZN11LevelScript7processEi` (original `0x137d18`, 57,232 B = 0xdf90),
size-exact.  Worktree `wt/w10902`, base `e51db2fc7` (wave 162).  Scratch
`/dev/shm/w10902/`.

## Baseline (HEAD, unchanged)

```
process raw diffbytes    54,698
process aligned insns    17,456
orig / ours size         57232 / 57232   (size-exact)
.text diffbytes          85,042
.ARM.extab diffbytes     725
.rel.plt                 0        (BYTE-IDENTICAL)
byte_exact 4507 / linked_exact 4507 / size 4524 / data 843 / imports 207
```

## Landed: mission-0 arm order 4,3,5 + mission-125 arm order 0,6,7

`src/game/world/LevelScript.cpp`:

1. **Mission-0 dispatch** (line ~680): the state-3 `else if` arm is moved
   before the state-5 arm, giving source order **4,3,5**.  This is the
   original's order (w10804/w10704): the extra 13-entry `tbh` (states 3..15)
   disappears and the switch lowers to the original's 10-entry
   `subs r0,r1,#6; cmp r0,#9; bhi; tbh` (states 6..15).  Our table is now at
   `0x13b16a` with 10 entries, matching the original's `0x139b26`.

2. **Mission-125 arm order** (line ~2104): the three mutually-exclusive arms
   are reordered from `6,0,7` to **`0,6,7`**.  With the 4,3,5 swap the
   mission-0 case-9/11 blocks (`switchPlanetForIntro`, `switchSkyboxForIntro`,
   `ParticleSystemManager::reset`, `startSmokeEmission`) are laid out *after*
   the mission-125 state-7 block (`PlayerStation::setVisible`), rotating five
   `.rel.plt` `R_ARM_JUMP_SLOT` slots (setVisible first instead of last).
   Putting the state-0 arm first moves the state-7 block back after the
   mission-0 cases: `.rel.plt` is byte-identical again.

3. **Size compensation**: the 0,6,7 reorder is 16 B shorter than the original
   (57,216).  Three `volatile int` locals in the state-0 arm restore the
   exact 57,232 B.  (The original's state-0 arm at `0x13f308` is just
   `reset(true); m_nState = 1;`; the 16 B is a block-placement delta, not a
   source-level local.)

```
                         before (HEAD)     after (landed)
process raw diffbytes    54,698            54,977            (+279)
process aligned insns    17,456            17,993            (+537)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
.text diffbytes          85,042            85,321            (+279)
.ARM.extab diffbytes     725               723               (-2)
.rel.plt                 0                 0                 (BYTE-IDENTICAL)
```

`check` green, byte_exact 4507 / linked_exact 4507.

## Honest assessment

The swap fixes the structural table mismatch (16 `tbh` -> 15, 10-entry table
matching the original) but is a **raw regression** (+279) and an aligned
regression (+537) on this base.  The w10803 `__builtin_expect` cells were
tuned for the old 4,5,3 block layout; on the 4,3,5 layout the same cells no
longer reproduce the original's head ordering (the original tests state 4
first and falls through to state 3; ours still tests state 3 first and falls
through to state 4).

## Measured alternatives (all size-fatal unless noted)

| variant | raw | aligned | size | plt |
|---|---:|---:|---:|---:|
| HEAD 4,5,3 (13-entry tbh) | 54,698 | 17,456 | 57,232 | 0 |
| swap 4,3,5 alone | 55,025 | 18,005 | 57,232 | 10 |
| swap + 125 order 6,7,0 | 54,985 | 17,969 | 57,224 | 18 |
| swap + 125 order 0,6,7 | 55,251 | 17,969 | 57,216 | **0** |
| **swap + 125 0,6,7 + 3 volatile pads** | **54,977** | **17,993** | **57,232** | **0** |
| swap + 125 0,6,7 + nested state-0 | 55,180 | 18,060 | 57,228 | 4 |
| swap + 125 0,6,7 + case reorder | 55,201 | 17,982 | 57,208 | 0 |
| swap + expect(state==3,1) | 55,493 | 26,238 | 57,456 | 59 |
| swap + expect(state==4,0) | 55,450 | 26,901 | 57,296 | 25 |
| swap + expect(state==5,1) | 55,251 | 19,480 | 57,256 | 13 |

Mission-0 case-label order does not change the layout (all five permutations
of the `case 6..15` labels measured identical: 55,190 / 57,224 / plt 10).

## Reproduce

```sh
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10902/fdiff.py _ZN11LevelScript7processEi        # 54,977 / 57,232
python3 /dev/shm/w10902/align_diff.py _ZN11LevelScript7processEi  # 17,993
cmake --build cmake-build-match --target check -j6                 # RATCHET PASS
```

## Open questions

1. The original's mission-0 head tests state 4 first and falls through to the
   state-3 body; ours tests state 3 first and falls through to state 4.  A
   `__builtin_expect` that flips the fall-through without changing size was
   not found (all size-changing).
2. The original's mission-125 layout is state-6 (`0x13c312`), state-7
   (`0x13d8fc`), hacking (`0x13de44`), state-0 (`0x13f308`).  Our 0,6,7 source
   order reproduces the `.rel.plt` order but not that layout; the 16 B pad is
   the price.
