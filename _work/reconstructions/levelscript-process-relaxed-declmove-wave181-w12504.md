<<<<<<< HEAD
# LevelScript::process — wave 181 / w12504: relaxed declaration moves (raw 52,795 -> 52,787)
=======
# LevelScript::process — wave 181 / w12504: relaxed declaration moves (raw 52,795 -> 52,788)
>>>>>>> 7f8ed3075 (LevelScript::process wave 181: five relaxed declaration moves (raw 52795 -> 52788))

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree
`/home/debian/GalaxyOnFire2/wt/w12504`, branch `wave181-w12504`, base master
`1e270aa0e` (wave-180 harvest).  Scratch `/home/debian/GalaxyOnFire2/scratch/w12504`.

Follow-up to the same-wave declaration-order pass
(`levelscript-process-declorder-wave181-w12504.md`, raw 52,818 -> 52,799) and
the three one-line operand/one-use wins that followed (52,799 -> 52,795).
This pass re-runs the block-scoped declaration-move search with the
"declaration-with-a-call may not cross another call" filter relaxed, and lands
<<<<<<< HEAD
six further moves (five in the first commit, one in the immediate
follow-up commit after a re-sweep on the new tree).
=======
five further moves.
>>>>>>> 7f8ed3075 (LevelScript::process wave 181: five relaxed declaration moves (raw 52795 -> 52788))

## Result (code win landed)

```
                         base (HEAD)      this pass
<<<<<<< HEAD
process raw diffbytes    52,795           52,787            (-8)
process aligned insns    20,204           20,243            (+39)
orig / ours size         57232 / 57232    57232 / 57232     (size-exact)
orig / ours insns        20,830 / 20,731  20,830 / 20,731
.text diffbytes          76,177           76,169            (-8)
.ARM.extab diffbytes     713              713               (size/base exact)
.ARM.exidx diffbytes     0                0
.note.gnu.build-id       20               20
total differing bytes    76,910           76,902            (-8)
=======
process raw diffbytes    52,795           52,788            (-7)
process aligned insns    20,204           20,243            (+39)
orig / ours size         57232 / 57232    57232 / 57232     (size-exact)
orig / ours insns        20,830 / 20,731  20,830 / 20,731
.text diffbytes          76,177           76,170            (-7)
.ARM.extab diffbytes     713              713               (size/base exact)
.ARM.exidx diffbytes     0                0
.note.gnu.build-id       20               20
total differing bytes    76,910           76,903            (-7)
>>>>>>> 7f8ed3075 (LevelScript::process wave 181: five relaxed declaration moves (raw 52795 -> 52788))
.rel.plt                 md5 289b2a06…    md5 289b2a06…     (byte-identical)
byte_exact               4507             4507
linked_exact             4507             4507
size-exact               4524/4524        4524/4524
data_exact               843/843          843/843
imports                  207              207
lint                     CLEAN ✓          CLEAN ✓
drift                    0                0
symbol parity            5370/5370        5370/5370
stub_zero_size           0                0
missing / wrong_type     0 / 0            0 / 0
extra                    0                0
RATCHET                  PASS             PASS
```

`raw` is the same-address byte diff over the function body (literal pools
included), from `tools/fn_diff.py`; it is the primary metric.  Size,
`.rel.plt`, `.ARM.extab` and `.ARM.exidx` are unchanged, so the -7 `.text`
bytes are a net win.

## The landed source change

<<<<<<< HEAD
Six semantics-preserving declaration moves inside `LevelScript::process`.
=======
Five semantics-preserving declaration moves inside `LevelScript::process`.
>>>>>>> 7f8ed3075 (LevelScript::process wave 181: five relaxed declaration moves (raw 52795 -> 52788))
Every moved declaration is a pure getter / field load; the crossed statements
are calls/field writes that do not mutate the object being read, so the value
read is unchanged.  No RNG call order changes.  Line numbers are the
pre-change tree (HEAD `20eeb6046`).

| # | base line | moved declaration | new position | raw |
|---:|---:|---|---|---:|
| 1 | 727 | `int speedBits2 = player->getSpeed();` | before `Vector pos = player->getPosition();` (line 713) | 52,794 |
| 2 | 1898 | `Vector camPos = *m_pCamera->getPosition();` | before `m_pCamera->setTarget(m_pGeometry6);` (line 1896) | 52,794 |
| 3 | 2049 | `KIPlayer *freighter = (*m_pLevel->getEnemies())[0];` | before `(*enemies)[0]->geometry->updateReferenceMatrix();` (line 2046) | 52,792 |
| 4 | 2497 | `Vector dir = (*m_pLevel->getEnemies())[0]->geometry->getDirection();` | before `radar->enabled = 0;` (line 2491) | 52,794 |
| 5 | 2526 | `Vector camPos = (*m_pLevel->getEnemies())[0]->getPosition();` | before `field_0x12d = 0;` (line 2523) | 52,794 |
<<<<<<< HEAD
| 6 | 1899* | `Vector zero{0.0f, 0.0f, 0.0f};` | before `m_pCamera->setTarget(m_pGeometry6);` (line 1897*) | 52,787 |
| all six | | | | **52,787** |

\* line numbers on the post-move-2 tree (the first five moves are commit
`7f8ed3075`; move 6 is the follow-up commit).

Each move was measured individually and in combination; the six are strictly
additive (-3, -1, -1, -1, -1, -1).  Move 6 is a pure constant local, so its
placement has no observable effect.
=======
| all five | | | | **52,788** |

Each move was measured individually and in combination; the five are strictly
additive (-3, -1, -1, -1, -1).
>>>>>>> 7f8ed3075 (LevelScript::process wave 181: five relaxed declaration moves (raw 52795 -> 52788))

### Why the moves are semantics-preserving

1. `player->getSpeed()` is a pure getter; the crossed statements are
   `getPosition()` and `FModSound::play/updateEvent3DAttributes/stop`, none of
   which changes the player's speed.  The original also reads the speed twice
   (`0x13c…`); both reads observe the same value.
2. `m_pCamera->getPosition()` is read before `TargetFollowCamera::setTarget`
   and `FModSound::play(14,…)`.  `setTarget` stores the target geometry only
   and does not move the camera, so the position read is identical.
3. `m_pLevel->getEnemies()` is a pure getter and `(*…)[0]` is a stable array
   slot; `AEGeometry::updateReferenceMatrix` and
   `ParticleSystemManager::enableSystemEmit` do not touch the Level enemy
   array or its element 0.
4. `AEGeometry::getDirection()` is a pure getter; the crossed statements are
   radar/HUD writes and `TargetFollowCamera::setLookAtCam/setActive/setTarget`,
   none of which changes the enemy geometry's direction.
5. `KIPlayer::getPosition()` (vtable+0x28) reads the geometry position; the
   crossed writes are `PlayerFighter::field_0x12d/0x38/0x12c`, unrelated
   fields, and the move stays inside the same `if` body (still conditional).

## Method

The earlier wave-181 sweep (`scratch/w12504/sweep.py`) rejects moving a
declaration whose initializer contains a call across any statement that also
contains a call.  `gen_relaxed.py` drops only that rule (keeping the
name-mention, earlier-declaration and RNG-order filters), producing 1,456
candidates instead of 993.  `run_relaxed.py` measures them with the same
harness: exact `compile_commands.json` clang command, exact gold link from
`build.ninja`, `tools/fn_diff.py`-equivalent same-address raw diff, function
size from `.dynsym` and `.rel.plt` md5.  `combo_relaxed.py` then measures all
<<<<<<< HEAD
31 non-empty subsets of the first five winners; the full set is the best.
The sweep was then re-run on the resulting tree (AST regenerated): the only
new win was the `zero` constant move (move 6, -1).
=======
31 non-empty subsets of the five winners; the full set is the best.
>>>>>>> 7f8ed3075 (LevelScript::process wave 181: five relaxed declaration moves (raw 52795 -> 52788))

The `lift` candidate at line 4863 (`float lift = (float) m_nScriptTimerA /
-15000.0f + 1.0f`) also measures 52,792 in this sweep but is **not** landed:
the original (`0x141932`-`0x141976`) converts `m_nScriptTimerA + delta` and
stores the incremented timer, so the declaration must stay after
`m_nScriptTimerA += delta`.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12504
ulimit -v 12000000
ninja -C cmake-build-match libgof2hdaa.so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
<<<<<<< HEAD
# -> raw=52787 aligned=20243 size=57232/57232 insn=20731/20830
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=52787 size=57232/57232 text=76169 relplt=289b2a069f8504d1e8a3919e43915e04
=======
# -> raw=52788 aligned=20243 size=57232/57232 insn=20731/20830
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=52788 size=57232/57232 text=76170 relplt=289b2a069f8504d1e8a3919e43915e04
>>>>>>> 7f8ed3075 (LevelScript::process wave 181: five relaxed declaration moves (raw 52795 -> 52788))
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
cmake --build cmake-build-match --target lint  -- -j2   # CLEAN
cmake --build cmake-build-match --target drift -- -j2   # 0 fields
```

Harnesses (`gen_relaxed.py`, `run_relaxed.py`, `combo_relaxed.py`,
`apply_relaxed.py`, `relaxed.json`, `relaxed_results.tsv`) are scratch, not
tracked.
