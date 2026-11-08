# LevelScript::process — wave 160 / w10603: literal-pool / rodata audit

Target `_ZN11LevelScript7processEi` (`LevelScript::process`), original
`0x137d18`, **57,232 B (0xdf90)**, size-exact.  Owned TU
`src/game/world/LevelScript.cpp`.  Worktree `wt/w10603`, base master
`08af4633b` (wave 159).  Scratch `/dev/shm/w10603/`.

## Result (landed)

```
                         before (HEAD)     after (landed)
process raw diffbytes    55,055            55,060            (+5)
process aligned insns    17,059            17,055            (-4)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
.text diffbytes          85,722            85,727            (+5)
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
RATCHET                  PASS              PASS
```

Landed source change: mission-89 case 4 now calls
`m_pLevel->getStarSystem()` **twice**, matching the original's two calls at
`0x13b3ba` and `0x13b3c8` (the old local `StarSystem *stars4` let clang CSE
them into one call and reuse `r4`).  Size-exact; aligned -4, raw +5.

## Literal-pool / rodata audit (the mission)

Method: disassemble both functions over the same 57,232-byte range, extract
every PC-relative `ldr`/`vldr`/`ldrd`, resolve `ldr rX,[pc]; add rX,pc`
sequences to their final `.data`/`.got` address, and compare the loaded
*values* (not pool slots) as multisets.  Pool slots themselves are shifted
between the two builds, so only values are meaningful.  `.rodata`, `.data`,
`.bss` and `.ARM.exidx` are byte-identical at section level, so any wrong
constant must be an instruction immediate or a `.text` literal-pool word.

### Float constants

All float values match except one **extra `0.8f` in the original**:

| build | `0x3f4ccccd` (0.8f) loads |
|---|---:|
| original | 2 |
| ours (base) | 1 |

The missing use is `m_pCamera->translate(-(float) delta * 0.8f, 0.0f, 0.0f);`
in the original's block at `0x13c88c`:

```
13c88c: ldr.w r0,[sl,#28]      ; m_nState
13c890: subs  r0,#5
13c892: cmp   r0,#3
13c894: bhi   13c8ba
13c896: ldr   r0,[sp,#76]      ; delta
13c89a: vldr  s2, 0.8f
...
13c8ae: vmul.f32 s0,s0,s2
13c8b6: blx   TargetFollowCamera::translate
```

Our source *contains* the block (line 1239) but it is **inside the switch**:
the `if (m_nState >= 5 && m_nState <= 8)` sits after the last `case` and
before the switch's closing brace.  Every case ends with `break`, so the
compiler correctly proves the block unreachable and deletes it.  The
original's block is the switch's break target (i.e. it is *after* the
switch).  Moving the `if` outside the closing brace restores the second
0.8f load but changes the function size to **57,212 (-20)** and aligned to
**19,754** (raw 55,463), so it is gate-fatal on this tree.

### Global (`ldr+add pc`) values

Resolved targets are identical except for two counts:

| global | original | ours |
|---|---:|---:|
| `Globals::sound` (`0x210168`) | n | n+1 |
| `Globals::layout` (`0x210224`) | n | n+1 |

i.e. our source loads each of those two globals one extra time.  No wrong
global is referenced.

### String / vtable / function pointers

`.rodata` is byte-identical and the function is at the same address, so
every absolute pointer value matches.  The residual `abs` multiset
differences (`0x0a10ee00`, `0x1a43ed8d`, …) are literal-pool words that
objdump disassembles as instructions, not real loads.

### Mission thresholds / timers / immediates

Large-immediate multiset comparison found **no wrong threshold value**.
Every common timer (`1100`, `2001`, `2151`, `2501`, `3001`, `4001`,
`5001`, `6001`, `7001`, `9001`, `12001`, `15001`, `18001`, `27001`,
`30001`, `38001`, `39001`, `60001`, …) occurs the same number of times in
both builds.  The only large-immediate differences are code that is
missing/extra, not a changed value:

* original-only `#8000` (`0x13df66`, `subs.w r0,r0,#8000`): mission-89
  case 4's shared tail is
  `m_nScriptTimerA += delta; if (m_nScriptTimerA >= 8000) m_nState = 5;`.
  Our source has the increment but **not the 8000 transition**, and instead
  has a no-op `m_nState = 4` in the fade block.  Adding the transition and
  dropping the no-op gives size **57,240 (+8)**, raw 55,245, aligned 17,293.
  (Both fixes together: size 57,228 (-4), raw 55,394, aligned 19,997.)
* `#35000` count 4 (orig) vs 3 (ours): the `35000 + rnd->nextInt(10000)`
  spawn offsets; one occurrence is CSE'd/absent, values are the same.
* `#9175040` (`0x8c0000`) is pool data at `0x140eb4`, not an immediate.

### Switch tables

Both builds have 15 `tbh` tables.  Decoded with the `cmp` before each
`tbh`:

| original (cmp, entries) | ours (cmp, entries) |
|---|---|
| 11,12 | 11,12 |
| 4,5 | 4,5 |
| (r1 dispatch) | (r1 dispatch) |
| (r1 dispatch) | (r1 dispatch) |
| 3,4 | 12,13 (one duplicate pair) |
| 9,10 | 5,6 |
| 6,7 | 3,4 |
| 3,4 | 6,7 |
| 9,10 | 5,6 |
| 3,4 | 9,10 |
| 5,6 | 3,4 |
| 9,10 (entries 2,3,4 equal) | 9,10 |
| 5,6 | 5,6 |
| 9,10 | 9,10 (entries 2,3,4 equal) |
| 4,5 | 4,5 |

The corresponding 10-entry mission switch (`0x13aeec` orig / `0x13aeb0`
ours) has the same case order (case 1 lowest, then cases 2..10 in
ascending address order).  The duplicate-target pattern in the
`0x13e638`/`0x13ed56` table matches.  The extra 13-entry table in ours
(`0x139068`, entries 1 and 2 both `0x13b2dc`) has no counterpart in the
original and is the largest remaining switch-structure difference.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10603/fdiff.py _ZN11LevelScript7processEi        # 55,060 / 57,232
python3 /dev/shm/w10603/align_diff.py _ZN11LevelScript7processEi  # 17,055
cmake --build cmake-build-match --target check -j6                 # RATCHET PASS
```

## Open questions

1. Move the `if (m_nState >= 5 && m_nState <= 8)` block outside the switch
   and then find the 20 bytes the compiler now removes elsewhere, so the
   size returns to 57,232.  This is the only way to recover the second
   0.8f load.
2. Add the mission-89 `if (m_nScriptTimerA >= 8000) m_nState = 5;` and find
   an 8-byte companion so the size stays exact.
3. The extra `Globals::sound` / `Globals::layout` loads (one each) are a
   structural difference worth locating; they may explain part of the
   8-byte size gap above.
4. The extra 13-entry `tbh` table in ours (`0x139068`) has no original
   counterpart; verify the source switch that produced it.
