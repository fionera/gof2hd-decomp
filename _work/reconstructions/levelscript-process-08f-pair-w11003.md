# LevelScript::process — wave 164 / w11003: land the 0.8f hoist + size companion

Target `_ZN11LevelScript7processEi` (`LevelScript::process`), original
`0x137d18`, **57,232 B (0xdf90)**.  Owned TU `src/game/world/LevelScript.cpp`.
Worktree `wt/w11003`, branch `wave164-w11003`, base master `b3d3a1f68`
(wave 163).  Scratch `/dev/shm/w11003/`.

Follow-up to `levelscript-process-08f-plt-safe-w10903.md` and
`levelscript-process-08f-strd-w10903.md`.  Those notes measured the hoist on
`e51db2fc7` (wave 162), where it is **+32 B**.  Master already contains
w10901's +24 B companion (`levelscript-process-24b-companion-w10901.md`), so
on `b3d3a1f68` the same hoist is **+48 B** (57,280).  The companion therefore
has to be -48 B, not -32 B.

## Baseline (master b3d3a1f68, gate green)

```
process raw diffbytes    54,541
process aligned insns    17,427
orig / ours size         57232 / 57232
0.8f pool words          2 referenced (original: 3)
.rel.plt                 byte-identical (29,392 B, cmp clean)
byte_exact               4507 / 4524
```

## Landed source change (two hunks)

1. Hoist the missing post-switch block to the top of the mission-0x4e arm,
   after the `m_nScriptTime >= 7901` gate, sharing one `m_nState` load with
   the switch:

```cpp
        int st78 = m_nState;
        if (st78 >= 5 && st78 <= 8) {
            m_pCamera->translate((float) -delta * 0.8f, 0.0f, 0.0f);
        }
        switch (st78) {
```

   and delete the old in-switch copy (which clang proved unreachable because
   every `case` `break`s).

2. Size companion: remove the mission-0x4e state-3 camera drift

```cpp
            m_pCamera->translate((float) delta * 0.2f, 0.0f, (float) delta * 2.2f);
```

   (the original emits it at `0x1389fc`; this is the same -48 B companion
   w10004 used).  The `// FP immediates @0x1389c0 = 0.2f, @0x1389c4 = 2.2f.`
   comment is kept so the pool constants stay documented.

## Result (landed)

```
                         before (master)   after (landed)
process raw diffbytes    54,541            54,844            (+303)
process aligned insns    17,427            17,437            (+10)
orig / ours size         57232 / 57232     57232 / 57232     EXACT
0.8f pool words          2                 3                 (block restored)
.rel.plt                 29392 B           29392 B           BYTE-IDENTICAL (cmp)
```

Gate (`check`): byte 4507, linked 4507, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, parity 5370/5370.  **RATCHET PASS.**

The pair is not a raw/aligned win; it is a structural-faithfulness landing
that restores the original's third `0.8f` literal-pool reference while
keeping the function and `.rel.plt` byte-exact.  The aligned delta is +10
(the hoisted block is emitted, the state-3 block is dropped); the raw delta
is +303 because the two blocks are shaped differently in clang's global
linearisation.

## Placement sweep (all measured, size-exact with the state-3 companion)

| placement | raw | aligned | size |
|---|---:|---:|---:|
| **after gate, `st78` (landed)** | **54,844** | **17,437** | 57,232 |
| after gate, no `st78` | 54,868 | 17,440 | 57,232 |
| before gate, `st78` | 54,783 | 17,667 | 57,232 |
| before gate, no `st78` | 54,864 | 17,438 | 57,232 |

`before gate, st78` has the best raw (+242) but a +240 aligned regression;
the landed after-gate form is near-neutral on aligned (+10) and is the
better balanced shape.  All four keep `.rel.plt` byte-identical.

## Single-companion sweep (hoist + one removal, no state-3 companion)

| removal | raw | aligned | size |
|---|---:|---:|---:|
| state-3 translate | 54,844 | 17,437 | **57,232** |
| state-5 `getSpeed`/`setSpeed(0.98f)` | 55,335 | 17,402 | 57,248 |
| state-5 messages[13] translate | 55,136 | 17,400 | 57,192 |
| state-12 `spin/4000` rotate | 55,418 | 17,421 | 57,240 |
| state-13 `spin/5000` rotate | 55,321 | 17,426 | 57,248 |
| state-1 `setPosition(-5000,300,-5000)` | 55,052 | 17,427 | 57,256 |
| dead `m_nState = 13` after `return` | 55,467 | 17,418 | 57,280 |

Only the state-3 translate removal lands exactly 57,232.

## Remaining lever (not landed)

The baseline still carries ~8 extra PLT calls vs the original (histogram:
+1 each `AEGeometry::moveForward`, `AEGeometry::setPosition(Vector)`,
`TargetFollowCamera::setPosition(float,float,float)`, `Level::getStarSystem`,
`Level::getPlayer`, `PlayerEgo::getPosition`, plus the two deliberate
PLT-order anchors `getStanding`/`setStanding` and
`setAutoTurret`/`autoTurretIsEnabled`; -1 each `LODManager::forceUpdate` and
`TargetFollowCamera::setPosition(Vector)`).  Removing the six genuine
duplicates (24 B) plus ~24 B of other extra code would let the hoist be
size-exact *without* dropping the state-3 translate, which should recover
raw/aligned.  The call-sequence alignment is too noisy in this 2,900-call
function to pin the duplicate sites by inspection; that is the next pass.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j8
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# raw= 54844 aligned=17437 size=57232/57232
cmake --build cmake-build-match --target check -j8
```
