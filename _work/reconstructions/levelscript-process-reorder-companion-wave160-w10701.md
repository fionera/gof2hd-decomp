# LevelScript::process — wave 160 / w10701: the -60 B companion for the
# declaration reorder (found, but PLT-unsafe) + a gate-green reorder form

Target `_ZN11LevelScript7processEi` (`LevelScript::process`), original
`0x137d18`, **57,232 B (0xdf90)**, size-exact.  Owned TU
`src/game/world/LevelScript.cpp`.  Worktree `wt/w10701`, base master
`01c7f92d6` (wave 160).  Scratch `/dev/shm/w10701/`.

## Result

```
                         HEAD (base)      landed
process raw diffbytes    55,048           55,061            (+13)
process aligned insns    17,049           17,051            (+2)
orig / ours size         57232 / 57232    57232 / 57232     (size-exact)
byte_exact               4507             4507
linked_exact             4507             4507
size-exact               4524/4524        4524/4524
data_exact               843/843          843/843
imports                  207              207
lint                     CLEAN ✓          CLEAN ✓
RATCHET                  PASS             PASS
```

Landed change (2 lines):

```cpp
Array<RadioMessage *> *messages = m_pLevel->getMessages();
Status *&status = Globals::status;          // was: PlayerEgo *player = ...;
Array<KIPlayer *> *enemies = m_pLevel->getEnemies();
PlayerEgo *player = m_pLevel->getPlayer();  // was: Status *status = ...;
```

`Status *&status` aliases the global pointer, so every `status->` use
re-reads `Globals::status` — matching the original's per-use
`ldr r0,[r4]` reload (w10601 §1).  This is the only gate-green way found to
land the `messages,status,enemies,player` declaration order.

## 1. The plain reorder and its +60 B

`messages,status,enemies,player` with a plain `Status *status` local:

| shape | raw | aligned | size |
|---|---:|---:|---:|
| HEAD base `messages,player,enemies,status` | 55,048 | 17,049 | 57,232 |
| **plain reorder** | **54,929** | **16,920** | **57,292 (+60)** |
| reorder + `Status *&status` (landed) | 55,061 | 17,051 | 57,232 |
| reorder + no `Status` local at all | 55,061 | 17,051 | 57,232 |

The plain reorder's `this=r6, &m_pLevel=r5, messages=sl, delta=r8->r5,
enemies=r8, &Globals::status=r9, Status*=r4, canary=fp, frame 432`
(w10601 §4).  The +60 is NOT a single construct; the reorder-vs-no-local
call-stream diff localises it as:

* **+10 B** in the prologue/epilogue: the reorder spills `fp` (the canary
  base) and reloads it (`ldr.w fp,[sp,#68]`) on the mission-95/99 `return 1`
  path; the no-local shape has `canary=sl` and needs no reload.
* **+44 B** of block placement / literal pool after the mission-95/99
  `Mission::setStatusValue` call (`0x13cb06` reorder vs `0x13cad6`
  no-local): an extra `b.n` + `movs r5,#1` + `ldr.w fp,[sp,#68]` + `b.w`
  plus ~34 B more pool words.
* **+4 B** one extra `Layout::startFade` call instruction (10 vs the
  original's 9): the original tail-merges its two `startFade(true,-1,500)`
  sites onto one call (13b3a6 -> `b.w 139154`), ours does not.

## 2. The -60 B companion (found, size-exact, but PLT-unsafe)

Two genuine source fixes from `levelscript-process-literal-pools-w10603.md`
combine to exactly -60 B on top of the plain reorder:

**F1** — move the dead `if (m_nState >= 5 && m_nState <= 8)` block out of
the mission-106 switch (it sits after the last `case` and every case ends
with `break`, so clang deletes it).  The original's block is the switch's
break target.  This restores the second `0.8f` load.

```cpp
// inside the mission-106 arm, AFTER the switch's closing brace:
        }
        if (m_nState >= 5 && m_nState <= 8) {
            m_pCamera->translate((float) -delta * 0.8f, 0.0f, 0.0f);
        }
    } else if (Globals::status->getCurrentCampaignMission() == 89) {
```

**F2** — mission-89 case 4: drop the no-op `m_nState = 4;` in the fade
block and add the original's 8000 transition.

```cpp
            case 4: {
                StarSystem *stars4 = m_pLevel->getStarSystem();
                stars4->scaleSunDuringSupernovaIntro(delta);
                stars4->updateSupernova(delta);
                if (m_nScriptTimerA <= 7000 && m_nScriptTimerA + delta > 7000) {
                    Globals::layout->startFade(true, 255, delta + 1000);
                }
                m_nScriptTimerA += delta;
                if (m_nScriptTimerA >= 8000) {
                    m_nState = 5;
                }
                break;
            }
```

Measured (plain reorder + F1 + F2):

| shape | raw | aligned | size |
|---|---:|---:|---:|
| plain reorder | 54,929 | 16,920 | 57,292 |
| reorder + F1 | 55,469 | 20,109 | 57,224 |
| **reorder + F1 + F2** | **55,489** | **20,341** | **57,232 (exact)** |
| base + F1 | 55,463 | 19,754 | 57,208 |
| base + F1 + F2 | 55,453 | 19,976 | 57,224 |

So F1+F2 is the exact -60 companion, but it **reorders 30 PLT entries**
(F1 makes the previously-dead block's calls reachable, changing the
first-reference order of `RadioMessage::isOver`, `PlayerEgo::setFreeLookMode`,
`TargetFollowCamera::enableFirstPersonCam`, …).  `byte_exact` falls
4507 -> 4499 (the 8 functions whose `blx` PLT targets moved), so it is
gate-fatal even though every function stays size-exact.  `.rel.plt`
content is unchanged; only the `.plt` stub order moves.

The plain reorder alone does **not** move any PLT entry (0 changed), so a
-60 companion that preserves first-reference order would land.  F1 is the
blocker.

## 3. Other measured (not landed)

* 24 declaration permutations on the current base: 12 are size-exact; the
  best is the base order (55,048).  Next best `messages,enemies,status,player`
  = 55,052.  None beats the base.
* `Status *&status` with the base order: 55,056 / 17,068.
* Removing the c106 state-6 `Status *st` local: size 57,232, raw 55,058.
* 256-subset sweep of the 8 c106 state-6 `Globals::status->` uses through
  `st` under the reorder: sizes {57,268, 57,276, 57,284, 57,292}; none is
  57,232.
* Deleting one `startFade(true,-1,500)` site: 57,268 / 54,885; deleting
  both: 57,244 / 54,940.
* Sequential `Globals::status->` -> `status->` replacement (0..102 uses):
  no N gives 57,232.

## 4. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10701/fdiff.py _ZN11LevelScript7processEi        # 55,061 / 57,232
python3 /dev/shm/w10701/align_diff.py _ZN11LevelScript7processEi  # 17,051
cmake --build cmake-build-match --target check -j6                 # RATCHET PASS
```

## 5. Open questions

1. Land F1+F2 (the exact -60 companion) once the 30-entry PLT reorder is
   neutralised — e.g. by forcing the moved block's first references back to
   their old positions, or by a source-order tweak that restores the `.plt`
   slot order.  Then the plain reorder (54,929 / 16,920) becomes landable.
2. The mission-95/99 `return 1` path spills/reloads `fp` in the reorder
   shape; a source form that keeps `fp` live would remove ~10 B.
3. The original tail-merges its two `startFade(true,-1,500)` sites onto one
   call instruction; reproducing that merge removes ~24 B.
