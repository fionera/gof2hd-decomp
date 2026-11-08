# LevelScript::process — wave 154 / w10004: the faithful mission-0
# `(*enemies)[i]` loop lands at 57,232 B with a -112 B companion

Target `_ZN11LevelScript7processEi` (`LevelScript::process`), original `0x137d18`,
**57,232 B (0xdf90)**, size-exact.  Owned TU `src/game/world/LevelScript.cpp`.
Worktree `wt/w10004`, base master `4417db0b4` (wave 153).  Scratch in
`/dev/shm/w10004/`.

## Result (landed)

```
                         before (HEAD)     after (landed)
process raw diffbytes    55,328            55,298            (-30)
process aligned insns    17,185            21,871            (+4,686)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
whole .text diffbytes    86,623            86,593            (-30)
.ARM.extab diffbytes     1,265             1,269             (+4)
.ARM.exidx               0                 0
.rel.plt                 0                 0                 (byte-identical)
.rodata / .data / .bss   exact             exact
.note.gnu.build-id       20                20
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

All 27 section sizes exact; every section except `.text` / `.ARM.extab` /
`.note.gnu.build-id` is byte-identical.

## 1. The landed source change

Three genuine mission-0 statement removals, measured together at **-112**:

1. state-3 `m_pCamera->translate((float) delta * 0.2f, 0.0f, (float) delta * 2.2f);`
2. state-5 `int speedBits2 = player->getSpeed(); player->setSpeed(... * 0.98f);`
3. state-12 `float spin = (float) delta / 3000.0f; player->rotate(spin, spin, spin);`

plus the w9905 faithful loop shape (`(*enemies)[i]` for both `setDirection` and
`setVisible`, replacing the `KIPlayer **enemyData` local).

The deltas are **not additive**: the individual removals on the faithful base
measure -48 / -24 / -40 (= -112), but the combined build is exactly 57,232 B
because the whole-function re-colour moves in 8-byte steps.  Many other
-112 combinations were measured (see §3); this one is the only one that keeps
**all 27 section sizes exact** *and* improves raw `.text`.

## 2. Prologue register map (deliverable)

The landed build reproduces the w9905 "faithful ours" map exactly:

| value | original | landed |
|---|---|---|
| `this` | `sl` | **`r9`** |
| `&__stack_chk_guard` | `fp` | **`sl`** |
| `&m_pLevel` (temp) | `r4` | `r4` |
| `messages` | `r5` | `r5` |
| `delta` | `r6` | `r6` |
| `enemies` | `r8` | `r8` |
| `player` | spilled from `r0` to `[sp+84]` | **`fp`**, then `stmia` to `[sp+76]` |
| `&Globals::status` | **`r4`** | **`r4`** (match) |
| frame size | `0x1b0` (432) | **`0x1a8`** (424) |
| canary slot | `[sp+428]` | **`[sp+420]`** |
| stack slots | msg/delta `[sp+72]`, &level `[sp+80]`, player `[sp+84]`, this `[sp+88]`, status `[sp+92]`, enemies `[sp+96]` | msg/delta/player `[sp+68/72/76]`, this/status `[sp+80/84]`, enemies `[sp+88]`, &level `[sp+64]` |

The key mission lever is confirmed: the faithful loop puts `&Globals::status`
in **`r4`** (`ldr r4,[r0,#0]; ldr r0,[r4,#0]`, both 16-bit) exactly like the
original.  The residual `this`/canary rotation (`r9`/`sl` vs `sl`/`fp`) and the
8-byte-smaller frame are unchanged from w9905.

## 3. The -112 companion sweep (all built + linked)

Base = wave-153 HEAD + faithful loop = **57,344 B**, raw 55,091, aligned 21,843.
Every row below is a source removal (or two/three) that lands on 57,232 B.

| companion | size | sections | raw | aligned |
|---|---:|---|---:|---:|
| **state-3 translate + state-5 setSpeed + state-12 rotate** | **57,232** | **all exact** | **55,298** | **21,871** |
| state-1 block + state-0 `player->setPosition` | 57,232 | all exact | 55,384 | 21,190 |
| state-1 block + state-12 rotate | 57,232 | all exact | 55,336 | 21,500 |
| state-0 matrix block + state-12 rotate | 57,232 | `.ARM.extab` **-4** | 55,291 | 19,218 |
| state-6 rumble + state-3 translate + dead `m_nState=13` | 57,232 | `.text` **+12** | 55,304 | 21,905 |
| state-0 forceUpdate (redundant) alone | 57,236 | (not size-exact) | 55,196 | 21,034 |
| state-0 forceUpdate + `rumble >= 1.0f` | 57,228 | (not size-exact) | — | — |

Notes:

* The **state-0 `forceUpdate(30, false)`** removal is the semantically safe
  "genuine" companion (the original emits it at `0x137f72` and the shared tail
  emits it again at `0x137f90`), but on the wave-153 base it is **-108**, not
  the wave-152 note's -124.  No 4-byte lever was found to close the gap
  (`m_nState = N -> ++` are 0/+8/+40; adding `m_bStartSequenceOver = 0;` is +8;
  `s10` clamp is -16).  It was left out because it is not size-exact.
* Removing the **state-0 matrix block** (`CameraGetCurrent/CameraGetLocal/
  MatrixSetTranslation`) drops `.ARM.extab` from 156,312 to 156,308 (-4): the
  block owns an exception cleanup region.  That violates the section-size
  invariant, so the matrix-block pair was rejected even though it has the best
  aligned count (19,218).
* Removing the dead `m_nState = 13;` after `return m_bCinematicBreak != 0;`
  grows whole `.text` by 12 bytes (literal-pool/alignment), so it was rejected.
* `dir_then_vis_sep` (`AEGeometry *g = (*enemies)[i]->geometry; g->setDirection;
  g->setVisible`) is a size-exact 57,232 B alternative with raw **55,124** and
  aligned **17,198**, but it is *not* the faithful loop (one `data_` load per
  iteration instead of the original's two) and leaves `status` in `r9`, so it
  was not landed.

## 4. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10004/fdiff.py _ZN11LevelScript7processEi        # 55,298 / 57,232
python3 /dev/shm/w10004/align_diff.py _ZN11LevelScript7processEi  # 21,871
cmake --build cmake-build-match --target check -j6                 # RATCHET PASS
```

Scratch scripts: `/dev/shm/w10004/{fdiff.py,align_diff.py,sect.py,
variants.py,matrix.py,loopsyn.py,perturb.py,combos*.py,removals.py}`.

## 5. Open questions

1. The aligned count regresses 17,185 -> 21,871 because the faithful loop's
   double `enemies->data_` reload re-colours the whole function.  A source
   shape that keeps the double reload *and* the HEAD allocation is still open.
2. `nofu` is 4 B over target on this base; a genuine 4-byte shrink would let the
   semantically-safe redundant-forceUpdate removal land instead of the three
   statement removals used here.
3. The `this`/canary rotation (`r9`/`sl` vs `sl`/`fp`) and the 8-byte frame
   shrink remain; spilling `player` straight from `r0` (as the original does)
   is the most promising route (F_local shape: 57,224 B, original stack layout,
   but `status` in `r9`).
