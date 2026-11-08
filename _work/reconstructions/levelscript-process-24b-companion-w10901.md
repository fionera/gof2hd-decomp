# LevelScript::process — wave 163 / w10901: the +24 B companion for L1364:0 + L1389:1

Target `_ZN11LevelScript7processEi` (`LevelScript::process`), original `0x137d18`,
**57,232 B (0xdf90)**, size-exact.  Owned TU `src/game/world/LevelScript.cpp`.
Worktree `wt/w10901`, base master `e51db2fc7` (wave 162).  Scratch `/dev/shm/w10901/`.

## Baseline (wave-162 tree, unchanged)

```
raw diffbytes   54,698
aligned insns   17,456
orig/ours size  57,232 / 57,232
.text diff      85,042
.ARM.extab      725
.rel.plt        0 (BYTE-IDENTICAL)
byte_exact      4507/4524   linked_exact 4507/4524   size 4524/4524
data_exact      843/843     imports 207              lint CLEAN   drift 0
```

## The w10803 open question

w10803's PLT-safe expect cells (L1030:1, L1367:0, L1430:1, L1526:1) land
54,698 / 17,456 / 57,232.  Adding `L1364:0` (`m_nState == 2` cold) and
`L1389:1` (`m_nState == 5` hot) reaches **54,546 raw but 57,208 B**, i.e.
24 B short of size-exact.  The mission was to find a source change that adds
exactly 24 B to that shape without undoing the expect cells.

## Landed shape

Source change (one hunk, `src/game/world/LevelScript.cpp` ~1361-1380):

```cpp
} else if (__builtin_expect(m_nState == 2, 0)) { // lint: arm_builtin
    m_nScriptTimerA += delta;
    m_pLevel->lodManager->forceUpdate(0, false);
    if (__builtin_expect(m_nScriptTimerA >= 3001, 0)) { // lint: arm_builtin
        m_bCinematicBreak = 0;                       // 4 B dead store (overwritten below)
        AEGeometry *ship = (*enemies)[0]->geometry;
        m_pCamera->setTarget(ship);                  // original calls setTarget first
        m_tempVec = ((AEGeometry *) (*enemies)[0]->geometry)->getPosition();
        m_tempVec += ((AEGeometry *) (*enemies)[0]->geometry)->getDirection() * 8000.0f;
        m_tempVec += ((AEGeometry *) (*enemies)[0]->geometry)->getUpVector() * 500.0f;
        m_tempVec += ((AEGeometry *) (*enemies)[0]->geometry)->getRightVector() * 1000.0f;
        m_pCamera->setPosition(m_tempVec);           // original calls setPosition last
        m_nState = 3;
        m_nScriptTimerA = 0;
    } else {
        ...
} else if (__builtin_expect(m_nState == 5, 1)) { // lint: arm_builtin
```

The `m_bCinematicBreak = 0;` store is dead: mission 106 unconditionally sets
`m_bCinematicBreak = 1;` at the end of the block before `return 1;`.

## Exact 24-byte accounting

| step | fn size | delta | raw |
|---|---:|---:|---:|
| hint pair L1364:0 + L1389:1, old source order | 57,208 | — | 54,546 |
| swap `setTarget` before the position math, `setPosition` last | 57,208 | **0** | 54,545 |
| inline `(*enemies)[0]->geometry` at the four getter sites (4 reloads) | 57,228 | **+20** | 54,546 |
| `m_bCinematicBreak = 0;` at the top of the 3001 block | **57,232** | **+4** | **54,541** |

The four inlined geometry reloads each emit `ldr.w r8,[sp,#96];
ldr r0,[r8,#4]; ldr r0,[r0,#0]; ldr r1,[r0,#8]` (12 B) instead of reusing the
`ship` local, but the allocator reuses part of the sequence; net **+20 B**.
The dead store emits `strb.w r6,[fp,#17]` (4 B).  Total **+24 B**, size-exact.

The `setTarget`/`setPosition` reorder is the original's source order (original
`TargetFollowCamera::setTarget` at `0x13bed6` precedes `getPosition` at
`0x13bee2`; `setPosition` is at `0x13bf72`).  It is size-neutral and raw -1.

## Measured (full link)

| | raw | aligned | fn size | .text | .rel.plt |
|---|---:|---:|---:|---:|---:|
| baseline | 54,698 | 17,456 | 57,232 | 85,042 | 0 |
| **landed** | **54,541** | **17,427** | **57,232** | **84,885** | **0** |

Raw **-157**, aligned **-29**.  Every section size is exact; `.rel.plt` is
byte-identical; `.ARM.extab` stays 725; `.rodata/.data/.bss/.ARM.exidx` are
byte-identical.  `check` is green: byte_exact 4507/4524, linked_exact
4507/4524, size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0,
symbol parity 5370/5370.

## Rejected variants (all measured, full link)

| variant | raw | size | why not |
|---|---:|---:|---|
| make state 4 an `else if` of the state-3 chain (semantically closer to the original's skip of the state-4 check) | 54,941 | 57,232 | adds exactly +24 B but raw +395; the old separate-`if` layout is byte-closer |
| `m_pLevel->lodManager->forceUpdate(0,false)` duplicated | 55,035 | 57,224 | +16 B, raw +490 |
| `m_nScriptTimerA = 0;` duplicated at block top | 54,547 | 57,216 | +8 B only |
| `if (Globals::status == nullptr) m_nState = 3;` at block end | 54,538 | 57,216 | +8 B only |
| dead store at block end (`v15i`) | 54,544 | 57,232 | size-exact but raw 3 worse than block-top placement |
| dead store after `getPosition` | 54,543 | 57,232 | size-exact, raw 2 worse |
| `m_bCinematicBreak = 1;` / `m_bStartSequence = 0;` at block top | 54,549 | 57,232 | size-exact, raw 8 worse |

## Reproduction

```sh
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10901/fdiff.py _ZN11LevelScript7processEi        # 54,541 / 57,232
python3 /dev/shm/w10901/align_diff.py _ZN11LevelScript7processEi  # 17,427
cmake --build cmake-build-match --target check -j6                 # RATCHET PASS
```

## Open questions

1. The 3001 block is still 184 B vs the original's 190 B (6 B short); the
   original has `movs r0,#0` before `strd r0,r0,[r5]` and one more wide
   `add.w` than the current shape.  A source shape that forces the zero
   materialisation (instead of reusing the live r6=0) would recover 2 B and
   could replace the dead store.
2. The else-if control-flow fix is semantically correct but raw-worse; the
   original's PGO layout evidently pairs the separate-`if` source with the
   L1364/L1389 block order.
