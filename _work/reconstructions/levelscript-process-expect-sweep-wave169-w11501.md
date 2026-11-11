# LevelScript::process — wave 169 / w11501: expect + operand-order sweep on the wave-169 base

Target `_ZN11LevelScript7processEi` (original `0x137d18`, **57,232 B = 0xdf90**),
size-exact.  Owned TU `src/game/world/LevelScript.cpp`.  Worktree `wt/w11501`,
base master `00c403d5a` (wave 169).  Scratch `/dev/shm/w11501/`.

Continues `levelscript-process-expect-sweep-wave169-w11401.md` (cells 1-2 +
one stale-expect removal landed there) and the wave-168 w11301 table.

## Baseline (wave-169 master, gate green)

```
process raw diffbytes    53,970
process aligned insns    17,818
orig / ours size         57232 / 57232   (size-exact)
.text diffbytes          81,642
.rel.plt                 0        (BYTE-IDENTICAL, md5 289b2a069f8504d1e8a3919e43915e04)
byte_exact 4507 / linked_exact 4507 / size 4524 / data 843 / imports 207
```

## Method

Three independent sweeps, each candidate compiled with the exact NDK r18b
clang 7.0.2 `-Oz` command (from `compile_commands.json`), size-checked against
57,232 B, then **linked with the real gold command** (`/dev/shm/w11501/link.sh`,
the build.ninja link line with the candidate object substituted) and measured
with a direct ELF reader (`raw` = same-address differing bytes vs the original
`.so`; `.text` = whole-section differing bytes; `.rel.plt` md5 compared
byte-for-byte).  The link harness reproduces the build `.so` md5 exactly.

1. **Expect sweep** — all 830 `if`/`while`/`for` conditions plus every nested
   `&&`/`||`/`!` operand in the `process` body, each with weight 0 and 1
   (1,660 compiles).  675 stay size-exact; all 675 were linked and measured.
2. **Existing-expect sweep** — the 56 `__builtin_expect` sites in `process`
   were both flipped (0<->1) and stripped (112 variants).
3. **Operand-order sweep** — every adjacent pair of `&&`/`||` operands at any
   nesting depth, swapped (133 variants; assignment/inc/dec operands skipped).

## Landed (five commits)

| # | commit | line | change | raw | aligned |
|---|---|---|---:|---:|---:|
| 1 | `530c23b88` | 2854 | `__builtin_expect(m_nScriptTimerA >= 3001, 0)` (mission-42 state-5 timer) | 53,970 -> 53,964 | 17,818 -> 17,821 |
| 2 | `9078677d1` | 3566 | `__builtin_expect(messages[1]->isOver(), 1)` (mission-2 state-2) | 53,964 -> 53,962 | 17,821 -> 17,823 |
| 3 | `81ed4f12b` | 2917 | **remove** stale `expect(field_178 == 0, 1)` (mission-92) | 53,962 -> 53,959 | 17,823 |
| 4 | `ea1de9662` | 1536, 1526 | swap `&&` operands (wormhole gate; mission-50 dispatch) | 53,959 -> 53,955 | 17,823 -> 17,826 |
| 5 | `8525dd391` | 4199 | swap `&&` operands (hacking gate) | 53,955 -> 53,954 | 17,826 |
| 6 | `794cd8b7d` | 3075 | `__builtin_expect(player->isDockedToDockingPoint(), 1)` (mission-42 state-7) | 53,954 -> 53,953 | 17,826 -> 17,871 |

(Commit 4 is one commit with two swaps; the table lists them as one row.)

## Final (all six commits)

```
process raw diffbytes    53,970 -> 53,953   (-17)
process aligned insns    17,818 -> 17,871   (+53, dominated by commit 6)
orig / ours size         57232 / 57232      (size-exact)
.text diffbytes          81,642 -> 81,625   (-17)
.rel.plt                 0                  (BYTE-IDENTICAL)
RATCHET PASS (byte 4507 / linked 4507 / size 4524 / data 843 / imports 207 / lint CLEAN)
```

Commit 6 is a raw win (-1) but costs +45 aligned instructions; the original
branch at `0x140268` is `cbz r0, skip` (fall through when docked), so the
`expect(...,1)` hint is faithful even though the aligned stream shifts.  It is
the only single cell below the 53,954 base in the third sweep; every other
size-exact cell is neutral or worse.

## Expect-cell table (675 size-exact cells that change object bytes)

Base for this table: raw 53,962 / `.text` 81,634 (after commits 1-2, before
commit 3).  `rel.plt` is identical unless marked `ROTATED` (17 cells rotate
`.rel.plt` and are NOT landable).  Cells not listed were size-changing and
rejected.  The minimum is line 3075 at 53,961.

| raw | line | e | kind | cell | .text | rel.plt | size |
|---:|---:|---:|---|---|---:|---|---:|
| 53961 | 3071 | 1 | if | `m_nScriptTimerA >= 3001` | 81633 | same | 57232 |
| 53962 | 649 | 1 | if | `m_nState == 1` | 81634 | same | 57232 |
| 53962 | 652 | 1 | if | `msg6->isOver()` | 81634 | same | 57232 |
| 53962 | 658 | 1 | if | `m_nState == 2` | 81634 | same | 57232 |
| 53962 | 664 | 0 | if | `Globals::sound->isPlaying(142)` | 81634 | same | 57232 |
| 53962 | 664 | 1 | if | `!Globals::sound->isPlaying(142)` | 81634 | same | 57232 |
| 53962 | 669 | 1 | if | `((RadioMessage *) ((*messages)[7]))->isOver()` | 81634 | same | 57232 |
| 53962 | 686 | 0 | if | `((RadioMessage *) ((*messages)[10]))->isOver()` | 81634 | same | 57232 |
| 53962 | 728 | 1 | if | `((RadioMessage *) ((*messages)[13]))->isOver()` | 81634 | same | 57232 |
| 53962 | 731 | 0 | if | `((RadioMessage *) ((*messages)[14]))->isTriggered()` | 81634 | same | 57232 |
| 53962 | 740 | 0 | if | `((RadioMessage *) ((*messages)[8]))->isOver()` | 81634 | same | 57232 |
| 53962 | 740 | 1 | if | `((RadioMessage *) ((*messages)[8]))->isOver()` | 81634 | same | 57232 |
| 53962 | 764 | 0 | if | `!(rumble < 1.0f)` | 81634 | same | 57232 |
| 53962 | 764 | 0 | if | `(rumble < 1.0f)` | 81634 | same | 57232 |
| 53962 | 764 | 1 | if | `!(rumble < 1.0f)` | 81634 | same | 57232 |
| 53962 | 764 | 1 | if | `(rumble < 1.0f)` | 81634 | same | 57232 |
| 53962 | 790 | 1 | if | `m_nScriptTimerA >= 2001` | 81634 | same | 57232 |
| 53962 | 806 | 0 | if | `rumble < 0.0f` | 81634 | same | 57232 |
| 53962 | 806 | 1 | if | `rumble < 0.0f` | 81634 | same | 57232 |
| 53962 | 810 | 0 | if | `m_nScriptTimerA >= 4001` | 81634 | same | 57232 |
| 53962 | 810 | 1 | if | `m_nScriptTimerA >= 4001` | 81634 | same | 57232 |
| 53962 | 839 | 0 | if | `!(rumble < 1.0f)` | 81634 | same | 57232 |
| 53962 | 839 | 0 | if | `(rumble < 1.0f)` | 81634 | same | 57232 |
| 53962 | 839 | 1 | if | `!(rumble < 1.0f)` | 81634 | same | 57232 |
| 53962 | 839 | 1 | if | `(rumble < 1.0f)` | 81634 | same | 57232 |
| 53962 | 843 | 0 | if | `m_nScriptTimerA >= 2001` | 81634 | same | 57232 |
| 53962 | 843 | 1 | if | `m_nScriptTimerA >= 2001` | 81634 | same | 57232 |
| 53962 | 856 | 0 | if | `rumble < 0.0f` | 81634 | same | 57232 |
| 53962 | 856 | 1 | if | `rumble < 0.0f` | 81634 | same | 57232 |
| 53962 | 866 | 0 | if | `m_nScriptTimerA >= 2501` | 81634 | same | 57232 |
| 53962 | 866 | 1 | if | `m_nScriptTimerA >= 2501` | 81634 | same | 57232 |
| 53962 | 955 | 0 | if | `!(depth < 1.0f)` | 81634 | same | 57232 |
| 53962 | 955 | 0 | if | `(depth < 1.0f)` | 81634 | same | 57232 |
| 53962 | 955 | 1 | if | `!(depth < 1.0f)` | 81634 | same | 57232 |
| 53962 | 955 | 1 | if | `(depth < 1.0f)` | 81634 | same | 57232 |
| 53962 | 962 | 0 | if | `m_nState == 0` | 81634 | same | 57232 |
| 53962 | 962 | 0 | if | `msg2->isOver() && m_nState == 0` | 81634 | same | 57232 |
| 53962 | 962 | 1 | if | `m_nState == 0` | 81634 | same | 57232 |
| 53962 | 962 | 1 | if | `msg2->isOver()` | 81634 | same | 57232 |
| 53962 | 962 | 1 | if | `msg2->isOver() && m_nState == 0` | 81634 | same | 57232 |
| 53962 | 983 | 0 | if | `!(rumble < 1.0f)` | 81634 | same | 57232 |
| 53962 | 983 | 0 | if | `(rumble < 1.0f)` | 81634 | same | 57232 |
| 53962 | 983 | 1 | if | `!(rumble < 1.0f)` | 81634 | same | 57232 |
| 53962 | 983 | 1 | if | `(rumble < 1.0f)` | 81634 | same | 57232 |
| 53962 | 987 | 0 | if | `m_nScriptTime >= 10001` | 81634 | same | 57232 |
| 53962 | 987 | 1 | if | `m_nScriptTime >= 10001` | 81634 | same | 57232 |
| 53962 | 1004 | 0 | if | `!(rumble < 1.0f)` | 81634 | same | 57232 |
| 53962 | 1004 | 0 | if | `(rumble < 1.0f)` | 81634 | same | 57232 |
| 53962 | 1004 | 1 | if | `!(rumble < 1.0f)` | 81634 | same | 57232 |
| 53962 | 1004 | 1 | if | `(rumble < 1.0f)` | 81634 | same | 57232 |
| 53962 | 1023 | 1 | if | `m_nState == 2` | 81634 | same | 57232 |
| 53962 | 1051 | 1 | if | `m_nScriptTime >= 7901` | 81634 | same | 57232 |
| 53962 | 1073 | 0 | if | `m_nScriptTime >= 7001` | 81634 | same | 57232 |
| 53962 | 1073 | 1 | if | `m_nScriptTime >= 7001` | 81634 | same | 57232 |
| 53962 | 1080 | 1 | if | `m_nScriptTime >= 7901` | 81634 | same | 57232 |
| 53962 | 1081 | 0 | if | `Globals::sound->isPlaying(1122)` | 81634 | same | 57232 |
| 53962 | 1081 | 1 | if | `!Globals::sound->isPlaying(1122)` | 81634 | same | 57232 |
| 53962 | 1091 | 0 | if | `m_nScriptTime >= 18001` | 81634 | same | 57232 |
| 53962 | 1091 | 1 | if | `m_nScriptTime >= 18001` | 81634 | same | 57232 |
| 53962 | 1119 | 1 | if | `m_nScriptTime >= 27001` | 81634 | same | 57232 |
| 53962 | 1131 | 1 | if | `m_nScriptTimerA >= 2001` | 81634 | same | 57232 |
| 53962 | 1143 | 0 | if | `!(rumble < 1.0f)` | 81634 | same | 57232 |
| 53962 | 1143 | 0 | if | `(rumble < 1.0f)` | 81634 | same | 57232 |
| 53962 | 1143 | 1 | if | `!(rumble < 1.0f)` | 81634 | same | 57232 |
| 53962 | 1143 | 1 | if | `(rumble < 1.0f)` | 81634 | same | 57232 |
| 53962 | 1167 | 0 | if | `!(rumble < 1.0f)` | 81634 | same | 57232 |
| 53962 | 1167 | 0 | if | `(rumble < 1.0f)` | 81634 | same | 57232 |
| 53962 | 1167 | 1 | if | `!(rumble < 1.0f)` | 81634 | same | 57232 |
| 53962 | 1167 | 1 | if | `(rumble < 1.0f)` | 81634 | same | 57232 |
| 53962 | 1182 | 0 | if | `m_nScriptTimerA >= 2151` | 81634 | same | 57232 |
| 53962 | 1182 | 1 | if | `m_nScriptTimerA >= 2151` | 81634 | same | 57232 |
| 53962 | 1240 | 0 | if | `m_nState <= 8` | 81634 | same | 57232 |
| 53962 | 1240 | 0 | if | `m_nState >= 5` | 81634 | same | 57232 |
| 53962 | 1240 | 0 | if | `m_nState >= 5 && m_nState <= 8` | 81634 | same | 57232 |
| 53962 | 1240 | 1 | if | `m_nState <= 8` | 81634 | same | 57232 |
| 53962 | 1240 | 1 | if | `m_nState >= 5` | 81634 | same | 57232 |
| 53962 | 1240 | 1 | if | `m_nState >= 5 && m_nState <= 8` | 81634 | same | 57232 |
| 53962 | 1259 | 0 | if | `m_nScriptTime >= 30001` | 81634 | same | 57232 |
| 53962 | 1279 | 0 | if | `!(rumble < 1.0f)` | 81634 | same | 57232 |
| 53962 | 1279 | 0 | if | `(rumble < 1.0f)` | 81634 | same | 57232 |
| 53962 | 1279 | 1 | if | `!(rumble < 1.0f)` | 81634 | same | 57232 |
| 53962 | 1279 | 1 | if | `(rumble < 1.0f)` | 81634 | same | 57232 |
| 53962 | 1283 | 1 | if | `distance < 200000.0f` | 81634 | same | 57232 |
| 53962 | 1290 | 1 | if | `m_nScriptTime - delta <= 38000` | 81634 | same | 57232 |
| 53962 | 1295 | 0 | if | `m_nScriptTime >= 39001` | 81634 | same | 57232 |
| 53962 | 1308 | 1 | if | `planet->getScaling().x < 10.0f` | 81634 | same | 57232 |
| 53962 | 1326 | 1 | if | `m_nScriptTimerA + delta > 7000` | 81634 | same | 57232 |
| 53962 | 1326 | 1 | if | `m_nScriptTimerA <= 7000` | 81634 | same | 57232 |
| 53962 | 1330 | 1 | if | `m_nScriptTimerA >= 8000` | 81634 | same | 57232 |
| 53962 | 1386 | 1 | if | `m_nState == 4` | 81634 | same | 57232 |
| 53962 | 1394 | 1 | if | `m_nScriptTimerA >= 3001` | 81634 | same | 57232 |
| 53962 | 1398 | 0 | if | `m_nState == 6` | 81634 | same | 57232 |
| 53962 | 1398 | 1 | if | `((RadioMessage *) ((*messages)[5]))->isOver()` | 81634 | same | 57232 |
| 53962 | 1398 | 1 | if | `m_nState == 6` | 81634 | same | 57232 |
| 53962 | 1398 | 1 | if | `m_nState == 6 && ((RadioMessage *) ((*messages)[5]))->isOver()` | 81634 | same | 57232 |
| 53962 | 1413 | 1 | if | `m_nState <= 4` | 81634 | same | 57232 |
| 53962 | 1413 | 1 | if | `m_nState >= 2` | 81634 | same | 57232 |
| 53962 | 1429 | 0 | if | `m_nScriptTimerA >= 2000` | 81634 | same | 57232 |
| 53962 | 1429 | 1 | if | `m_nScriptTimerA >= 2000` | 81634 | same | 57232 |
| 53962 | 1438 | 0 | if | `m_nScriptTimerA >= 2000` | 81634 | same | 57232 |
| 53962 | 1438 | 1 | if | `m_nScriptTimerA >= 2000` | 81634 | same | 57232 |
| 53962 | 1461 | 1 | if | `Globals::status->getWingmen() != 0` | 81634 | same | 57232 |
| 53962 | 1463 | 1 | for | `i < wingmen->count` | 81634 | same | 57232 |
| 53962 | 1475 | 1 | if | `field_0x24 >= 7001` | 81634 | same | 57232 |
| 53962 | 1484 | 1 | if | `Level::programmedStation != 0` | 81634 | same | 57232 |
| 53962 | 1485 | 0 | if | `Level::programmedStation->equals(Globals::status->getStation())` | 81634 | same | 57232 |
| 53962 | 1485 | 1 | if | `!Level::programmedStation->equals(Globals::status->getStation())` | 81634 | same | 57232 |
| 53962 | 1491 | 1 | if | `Level::doInstantJump == 0` | 81634 | same | 57232 |
| 53962 | 1495 | 1 | if | `Globals::status->getWingmen() != 0` | 81634 | same | 57232 |
| 53962 | 1497 | 1 | for | `i < wingmen->count` | 81634 | same | 57232 |
| 53962 | 1499 | 1 | if | `wingman->isWingMan()` | 81634 | same | 57232 |
| 53962 | 1511 | 1 | if | `m_nState == 0` | 81634 | same | 57232 |
| 53962 | 1521 | 0 | if | `((RadioMessage *) ((*messages)[1]))->isTriggered()` | 81634 | same | 57232 |
| 53962 | 1521 | 0 | if | `((RadioMessage *) ((*messages)[1]))->isTriggered() && m_nState == 0` | 81634 | same | 57232 |
| 53962 | 1521 | 0 | if | `m_nState == 0` | 81634 | same | 57232 |
| 53962 | 1521 | 1 | if | `((RadioMessage *) ((*messages)[1]))->isTriggered()` | 81634 | same | 57232 |
| 53962 | 1521 | 1 | if | `((RadioMessage *) ((*messages)[1]))->isTriggered() && m_nState == 0` | 81634 | same | 57232 |
| 53962 | 1521 | 1 | if | `m_nState == 0` | 81634 | same | 57232 |
| 53962 | 1526 | 0 | if | `messages != nullptr` | 81634 | same | 57232 |
| 53962 | 1526 | 1 | if | `Globals::status->getCurrentCampaignMission() == 50` | 81634 | same | 57232 |
| 53962 | 1526 | 1 | if | `messages != nullptr` | 81634 | same | 57232 |
| 53962 | 1538 | 0 | if | `!(*enemies)[i]->isWingMan()` | 81634 | same | 57232 |
| 53962 | 1538 | 0 | if | `(*enemies)[i]->isWingMan()` | 81634 | same | 57232 |
| 53962 | 1538 | 1 | if | `!(*enemies)[i]->isWingMan()` | 81634 | same | 57232 |
| 53962 | 1538 | 1 | if | `(*enemies)[i]->isWingMan()` | 81634 | same | 57232 |
| 53962 | 1608 | 1 | if | `(*m_pLevel->getEnemies())[i]->shipGroup == 8` | 81634 | same | 57232 |
| 53962 | 1628 | 0 | if | `((RadioMessage *) ((*messages)[3]))->isOver()` | 81634 | same | 57232 |
| 53962 | 1628 | 1 | if | `((RadioMessage *) ((*messages)[3]))->isOver()` | 81634 | same | 57232 |
| 53962 | 1648 | 0 | if | `((RadioMessage *) ((*messages)[4]))->isOver()` | 81634 | same | 57232 |
| 53962 | 1648 | 0 | if | `((RadioMessage *) ((*messages)[4]))->isOver() && m_nState == 3` | 81634 | same | 57232 |
| 53962 | 1648 | 0 | if | `m_nState == 3` | 81634 | same | 57232 |
| 53962 | 1648 | 1 | if | `((RadioMessage *) ((*messages)[4]))->isOver()` | 81634 | same | 57232 |
| 53962 | 1648 | 1 | if | `((RadioMessage *) ((*messages)[4]))->isOver() && m_nState == 3` | 81634 | same | 57232 |
| 53962 | 1648 | 1 | if | `m_nState == 3` | 81634 | same | 57232 |
| 53962 | 1679 | 1 | if | `((RadioMessage *) ((*messages)[1]))->isOver()` | 81634 | same | 57232 |
| 53962 | 1720 | 1 | if | `ship != nullptr` | 81634 | same | 57232 |
| 53962 | 1720 | 1 | if | `ship != nullptr && ship->shipGroup == 9` | 81634 | same | 57232 |
| 53962 | 1720 | 1 | if | `ship->shipGroup == 9` | 81634 | same | 57232 |
| 53962 | 1734 | 1 | if | `((RadioMessage *) ((*messages)[3]))->isOver()` | 81634 | same | 57232 |
| 53962 | 1736 | 1 | for | `i < m_pLevel->getEnemies()->count` | 81634 | same | 57232 |
| 53962 | 1738 | 1 | if | `ship != nullptr` | 81634 | same | 57232 |
| 53962 | 1738 | 1 | if | `ship != nullptr && ship->shipGroup == 9` | 81634 | same | 57232 |
| 53962 | 1738 | 1 | if | `ship->shipGroup == 9` | 81634 | same | 57232 |
| 53962 | 1746 | 1 | if | `((RadioMessage *) ((*messages)[4]))->isOver()` | 81634 | same | 57232 |
| 53962 | 1746 | 1 | if | `((RadioMessage *) ((*messages)[4]))->isOver() && m_nState == 5` | 81634 | same | 57232 |
| 53962 | 1746 | 1 | if | `m_nState == 5` | 81634 | same | 57232 |
| 53962 | 1763 | 1 | if | `m_nState == 0` | 81634 | same | 57232 |
| 53962 | 1764 | 1 | for | `i <= 3` | 81634 | same | 57232 |
| 53962 | 1766 | 1 | if | `ship->enemyFlagsLo != 0` | 81634 | same | 57232 |
| 53962 | 1775 | 1 | if | `((RadioMessage *) ((*messages)[2]))->isTriggered()` | 81634 | same | 57232 |
| 53962 | 1826 | 0 | if | `m_nState == 2` | 81634 | same | 57232 |
| 53962 | 1826 | 1 | if | `m_nState == 2` | 81634 | same | 57232 |
| 53962 | 1897 | 1 | if | `m_pGeometry6 != nullptr` | 81634 | same | 57232 |
| 53962 | 1906 | 0 | if | `m_pGeometry6 != nullptr` | 81634 | same | 57232 |
| 53962 | 1906 | 1 | if | `m_pGeometry6 != nullptr` | 81634 | same | 57232 |
| 53962 | 1914 | 0 | if | `((RadioMessage *) ((*messages)[3]))->isTriggered()` | 81634 | same | 57232 |
| 53962 | 1914 | 1 | if | `((RadioMessage *) ((*messages)[3]))->isTriggered()` | 81634 | same | 57232 |
| 53962 | 1943 | 0 | if | `((RadioMessage *) ((*messages)[4]))->isOver()` | 81634 | same | 57232 |
| 53962 | 1943 | 1 | if | `((RadioMessage *) ((*messages)[4]))->isOver()` | 81634 | same | 57232 |
| 53962 | 1953 | 0 | if | `(*enemies)[0]->getPosition().z >= 90000.0f` | 81634 | same | 57232 |
| 53962 | 1953 | 0 | if | `m_nState == 2 && (*enemies)[0]->getPosition().z >= 90000.0f` | 81634 | same | 57232 |
| 53962 | 1953 | 1 | if | `(*enemies)[0]->getPosition().z >= 90000.0f` | 81634 | same | 57232 |
| 53962 | 1967 | 0 | if | `m_nState == 3                            && (*enemies)[0]->getPosition().z >= (*` | 81634 | same | 57232 |
| 53962 | 1967 | 1 | if | `m_nState == 3` | 81634 | same | 57232 |
| 53962 | 1967 | 1 | if | `m_nState == 3                            && (*enemies)[0]->getPosition().z >= (*` | 81634 | same | 57232 |
| 53962 | 1968 | 0 | if | `(*enemies)[0]->getPosition().z >= (*m_pLevel->getLandmarks())[3]->getPosition().` | 81634 | same | 57232 |
| 53962 | 1968 | 1 | if | `(*enemies)[0]->getPosition().z >= (*m_pLevel->getLandmarks())[3]->getPosition().` | 81634 | same | 57232 |
| 53962 | 1970 | 0 | if | `m_nState == 4` | 81634 | same | 57232 |
| 53962 | 1970 | 1 | if | `m_nState == 4` | 81634 | same | 57232 |
| 53962 | 1974 | 0 | if | `(*enemies)[0]->getPosition().z > 500000.0f` | 81634 | same | 57232 |
| 53962 | 1974 | 1 | if | `(*enemies)[0]->getPosition().z > 500000.0f` | 81634 | same | 57232 |
| 53962 | 2025 | 1 | if | `delta == 0x7d` | 81634 | same | 57232 |
| 53962 | 2169 | 1 | if | `((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(m_pParticleGe` | 81634 | same | 57232 |
| 53962 | 2174 | 1 | if | `m_nScriptTimerA >= 15001` | 81634 | same | 57232 |
| 53962 | 2181 | 0 | if | `!Globals::layout->isFading()` | 81634 | same | 57232 |
| 53962 | 2181 | 0 | if | `!Globals::layout->isFading() \|\| m_nScriptTimerA >= 10001` | 81634 | same | 57232 |
| 53962 | 2181 | 0 | if | `m_nScriptTimerA >= 10001` | 81634 | same | 57232 |
| 53962 | 2181 | 1 | if | `!Globals::layout->isFading() \|\| m_nScriptTimerA >= 10001` | 81634 | same | 57232 |
| 53962 | 2181 | 1 | if | `Globals::layout->isFading()` | 81634 | same | 57232 |
| 53962 | 2181 | 1 | if | `m_nScriptTimerA >= 10001` | 81634 | same | 57232 |
| 53962 | 2206 | 1 | if | `enemies != nullptr` | 81634 | same | 57232 |
| 53962 | 2207 | 1 | for | `i < enemies->count` | 81634 | same | 57232 |
| 53962 | 2208 | 1 | if | `(*enemies)[i]->player->isAlwaysFriend()` | 81634 | same | 57232 |
| 53962 | 2215 | 1 | if | `player->player->getArmorHP() <= 0` | 81634 | same | 57232 |
| 53962 | 2216 | 1 | if | `enemies != nullptr` | 81634 | same | 57232 |
| 53962 | 2217 | 1 | for | `i < enemies->count` | 81634 | same | 57232 |
| 53962 | 2258 | 1 | if | `((RadioMessage *) ((*messages)[2]))->isTriggered()` | 81634 | same | 57232 |
| 53962 | 2268 | 1 | for | `i < enemies->count` | 81634 | same | 57232 |
| 53962 | 2278 | 0 | if | `(*enemies)[i]->isWingMan()` | 81634 | same | 57232 |
| 53962 | 2278 | 1 | if | `!(*enemies)[i]->isWingMan()` | 81634 | same | 57232 |
| 53962 | 2285 | 1 | if | `((RadioMessage *) ((*messages)[6]))->isOver()` | 81634 | same | 57232 |
| 53962 | 2285 | 1 | if | `m_nState == 3` | 81634 | same | 57232 |
| 53962 | 2285 | 1 | if | `m_nState == 3 && ((RadioMessage *) ((*messages)[6]))->isOver()` | 81634 | same | 57232 |
| 53962 | 2286 | 1 | for | `i < enemies->count` | 81634 | same | 57232 |
| 53962 | 2301 | 1 | if | `((RadioMessage *) ((*messages)[3]))->isOver()` | 81634 | same | 57232 |
| 53962 | 2301 | 1 | if | `m_nState == 1` | 81634 | same | 57232 |
| 53962 | 2322 | 0 | if | `((RadioMessage *) ((*messages)[4]))->isOver()` | 81634 | same | 57232 |
| 53962 | 2322 | 1 | if | `((RadioMessage *) ((*messages)[4]))->isOver()` | 81634 | same | 57232 |
| 53962 | 2339 | 1 | if | `((RadioMessage *) ((*messages)[9]))->isTriggered()` | 81634 | same | 57232 |
| 53962 | 2339 | 1 | if | `m_nState == 5` | 81634 | same | 57232 |
| 53962 | 2407 | 1 | if | `((RadioMessage *) ((*messages)[0]))->isTriggered()` | 81634 | same | 57232 |
| 53962 | 2407 | 1 | if | `m_nState == 0` | 81634 | same | 57232 |
| 53962 | 2430 | 0 | if | `m_nState == 1` | 81634 | same | 57232 |
| 53962 | 2430 | 1 | if | `m_nState == 1` | 81634 | same | 57232 |
| 53962 | 2436 | 0 | if | `((RadioMessage *) ((*messages)[1]))->isOver()` | 81634 | same | 57232 |
| 53962 | 2436 | 1 | if | `((RadioMessage *) ((*messages)[1]))->isOver()` | 81634 | same | 57232 |
| 53962 | 2522 | 0 | if | `!((RadioMessage *) ((*messages)[2]))->isOver()` | 81634 | same | 57232 |
| 53962 | 2522 | 0 | if | `((RadioMessage *) ((*messages)[2]))->isOver()` | 81634 | same | 57232 |
| 53962 | 2522 | 1 | if | `!((RadioMessage *) ((*messages)[2]))->isOver()` | 81634 | same | 57232 |
| 53962 | 2522 | 1 | if | `((RadioMessage *) ((*messages)[2]))->isOver()` | 81634 | same | 57232 |
| 53962 | 2547 | 0 | if | `(int) AbyssEngine::AEMath::VectorLength(playerPos - enemyPos) <= 49999` | 81634 | same | 57232 |
| 53962 | 2547 | 1 | if | `(int) AbyssEngine::AEMath::VectorLength(playerPos - enemyPos) <= 49999` | 81634 | same | 57232 |
| 53962 | 2566 | 1 | for | `j < enemies->count` | 81634 | same | 57232 |
| 53962 | 2567 | 0 | if | `(*enemies)[j]->field_0x40 != 0` | 81634 | same | 57232 |
| 53962 | 2567 | 1 | if | `(*enemies)[j]->field_0x40 != 0` | 81634 | same | 57232 |
| 53962 | 2580 | 1 | if | `((RadioMessage *) ((*messages)[7]))->isTriggered()` | 81634 | same | 57232 |
| 53962 | 2623 | 0 | if | `m_nScriptTimerA >= 3001` | 81634 | same | 57232 |
| 53962 | 2632 | 0 | if | `m_nScriptTimerA >= 2001` | 81634 | same | 57232 |
| 53962 | 2632 | 1 | if | `m_nScriptTimerA >= 2001` | 81634 | same | 57232 |
| 53962 | 2655 | 0 | if | `m_nScriptTimerA >= 801` | 81634 | same | 57232 |
| 53962 | 2655 | 1 | if | `m_nScriptTimerA >= 801` | 81634 | same | 57232 |
| 53962 | 2661 | 0 | if | `m_nScriptTimerA >= 801` | 81634 | same | 57232 |
| 53962 | 2661 | 1 | if | `m_nScriptTimerA >= 801` | 81634 | same | 57232 |
| 53962 | 2680 | 1 | if | `m_nScriptTimerA >= 8001` | 81634 | same | 57232 |
| 53962 | 2730 | 0 | if | `m_nScriptTimerA >= 4001` | 81634 | same | 57232 |
| 53962 | 2730 | 1 | if | `m_nScriptTimerA >= 4001` | 81634 | same | 57232 |
| 53962 | 2742 | 0 | if | `!(rumble < 1.0f)` | 81634 | same | 57232 |
| 53962 | 2742 | 0 | if | `(rumble < 1.0f)` | 81634 | same | 57232 |
| 53962 | 2742 | 1 | if | `!(rumble < 1.0f)` | 81634 | same | 57232 |
| 53962 | 2742 | 1 | if | `(rumble < 1.0f)` | 81634 | same | 57232 |
| 53962 | 2746 | 1 | if | `m_nScriptTimerA >= 8001` | 81634 | same | 57232 |
| 53962 | 2766 | 0 | if | `!(rumble < 1.0f)` | 81634 | same | 57232 |
| 53962 | 2766 | 0 | if | `(rumble < 1.0f)` | 81634 | same | 57232 |
| 53962 | 2766 | 1 | if | `!(rumble < 1.0f)` | 81634 | same | 57232 |
| 53962 | 2766 | 1 | if | `(rumble < 1.0f)` | 81634 | same | 57232 |
| 53962 | 2770 | 0 | if | `m_nScriptTimerA >= 2151` | 81634 | same | 57232 |
| 53962 | 2770 | 1 | if | `m_nScriptTimerA >= 2151` | 81634 | same | 57232 |
| 53962 | 2773 | 0 | if | `m_nScriptTimerA >= 10001` | 81634 | same | 57232 |
| 53962 | 2773 | 1 | if | `m_nScriptTimerA >= 10001` | 81634 | same | 57232 |
| 53962 | 2789 | 0 | if | `m_nState <= 9` | 81634 | same | 57232 |
| 53962 | 2789 | 0 | if | `m_nState >= 7` | 81634 | same | 57232 |
| 53962 | 2789 | 0 | if | `m_nState >= 7 && m_nState <= 9` | 81634 | same | 57232 |
| 53962 | 2789 | 1 | if | `m_nState <= 9` | 81634 | same | 57232 |
| 53962 | 2789 | 1 | if | `m_nState >= 7` | 81634 | same | 57232 |
| 53962 | 2789 | 1 | if | `m_nState >= 7 && m_nState <= 9` | 81634 | same | 57232 |
| 53962 | 2791 | 0 | if | `m_nState >= 8` | 81634 | same | 57232 |
| 53962 | 2791 | 1 | if | `m_nState >= 8` | 81634 | same | 57232 |
| 53962 | 2833 | 1 | if | `m_nScriptTimerA >= 2001` | 81634 | same | 57232 |
| 53962 | 2838 | 0 | if | `m_nScriptTimerA >= 8001` | 81634 | same | 57232 |
| 53962 | 2860 | 1 | if | `player->isInRocketControl()` | 81634 | same | 57232 |
| 53962 | 2897 | 1 | if | `Globals::status->field_178 > 7` | 81634 | same | 57232 |
| 53962 | 2897 | 1 | if | `m_nScriptTime <= 179999` | 81634 | same | 57232 |
| 53962 | 2913 | 1 | if | `((RadioMessage *) ((*messages)[2]))->isTriggered()` | 81634 | same | 57232 |
| 53962 | 2913 | 1 | if | `m_nState == 0` | 81634 | same | 57232 |
| 53962 | 2913 | 1 | if | `m_nState == 0 && ((RadioMessage *) ((*messages)[2]))->isTriggered()` | 81634 | same | 57232 |
| 53962 | 2924 | 1 | for | `i < 3` | 81634 | same | 57232 |
| 53962 | 2963 | 0 | for | `i < 3` | 81634 | same | 57232 |
| 53962 | 2963 | 1 | for | `i < 3` | 81634 | same | 57232 |
| 53962 | 2983 | 1 | if | `m_nState == 3` | 81634 | same | 57232 |
| 53962 | 2999 | 1 | for | `i < 3` | 81634 | same | 57232 |
| 53962 | 3003 | 1 | if | `((RadioMessage *) ((*messages)[5]))->isOver()` | 81634 | same | 57232 |
| 53962 | 3011 | 1 | if | `m_nState == 4` | 81634 | same | 57232 |
| 53962 | 3028 | 0 | for | `i < 3` | 81634 | same | 57232 |
| 53962 | 3028 | 1 | for | `i < 3` | 81634 | same | 57232 |
| 53962 | 3036 | 1 | if | `m_nState == 5` | 81634 | same | 57232 |
| 53962 | 3042 | 1 | if | `m_nScriptTimerA >= 2001` | 81634 | same | 57232 |
| 53962 | 3052 | 0 | if | `docked` | 81634 | same | 57232 |
| 53962 | 3052 | 1 | if | `docked` | 81634 | same | 57232 |
| 53962 | 3069 | 0 | if | `m_nState == 6` | 81634 | same | 57232 |
| 53962 | 3075 | 1 | if | `player->isDockedToDockingPoint()` | 81634 | same | 57232 |
| 53962 | 3087 | 1 | if | `player->isInRocketControl()` | 81634 | same | 57232 |
| 53962 | 3134 | 1 | if | `m_nScriptTimerA >= 2001` | 81634 | same | 57232 |
| 53962 | 3135 | 1 | if | `m_nState == 9` | 81634 | same | 57232 |
| 53962 | 3163 | 1 | switch | `m_nState` | 81634 | same | 57232 |
| 53962 | 3165 | 0 | if | `Globals::status->field_178 > 6 \|\| m_nScriptTime >= 60001` | 81634 | same | 57232 |
| 53962 | 3165 | 1 | if | `Globals::status->field_178 > 6` | 81634 | same | 57232 |
| 53962 | 3167 | 1 | for | `i != 2` | 81634 | same | 57232 |
| 53962 | 3176 | 1 | if | `player->isInRocketControl()` | 81634 | same | 57232 |
| 53962 | 3208 | 1 | if | `m_nScriptTimerA >= 6001` | 81634 | same | 57232 |
| 53962 | 3217 | 0 | if | `docked` | 81634 | same | 57232 |
| 53962 | 3217 | 1 | if | `docked` | 81634 | same | 57232 |
| 53962 | 3220 | 1 | if | `field_0xab` | 81634 | same | 57232 |
| 53962 | 3242 | 1 | for | `i != 4` | 81634 | same | 57232 |
| 53962 | 3271 | 1 | if | `((RadioMessage *) ((*messages)[0]))->isTriggered()` | 81634 | same | 57232 |
| 53962 | 3298 | 0 | if | `((RadioMessage *) ((*messages)[2]))->isOver()` | 81634 | same | 57232 |
| 53962 | 3298 | 1 | if | `((RadioMessage *) ((*messages)[2]))->isOver()` | 81634 | same | 57232 |
| 53962 | 3327 | 0 | if | `ship->shipGroup == 10` | 81634 | same | 57232 |
| 53962 | 3327 | 1 | if | `ship->shipGroup == 10` | 81634 | same | 57232 |
| 53962 | 3355 | 1 | if | `ship->shipGroupFlag == 51` | 81634 | same | 57232 |
| 53962 | 3392 | 1 | if | `m_nState == 3` | 81634 | same | 57232 |
| 53962 | 3401 | 1 | for | `i < enemies->count` | 81634 | same | 57232 |
| 53962 | 3402 | 0 | if | `ship->shipGroup == 10` | 81634 | same | 57232 |
| 53962 | 3402 | 1 | if | `ship->shipGroup == 10` | 81634 | same | 57232 |
| 53962 | 3406 | 1 | if | `i == (unsigned int) leader` | 81634 | same | 57232 |
| 53962 | 3426 | 1 | if | `m_nScriptTimerA >= 10001` | 81634 | same | 57232 |
| 53962 | 3428 | 1 | for | `i < enemies->count` | 81634 | same | 57232 |
| 53962 | 3451 | 1 | for | `i < enemies->count` | 81634 | same | 57232 |
| 53962 | 3451 | 1 | if | `(*enemies)[i]->shipGroup == 10` | 81634 | same | 57232 |
| 53962 | 3451 | 1 | if | `(*enemies)[i]->shipGroup == 10 && (*enemies)[i]->isDead()` | 81634 | same | 57232 |
| 53962 | 3466 | 1 | if | `(*enemies)[i]->shipGroup == 10` | 81634 | same | 57232 |
| 53962 | 3466 | 1 | if | `(*enemies)[i]->shipGroup == 10 && (*enemies)[i]->player->getEnemy(0)->isDead()` | 81634 | same | 57232 |
| 53962 | 3467 | 1 | if | `(*enemies)[i]->player->getEnemy(0)->isDead()` | 81634 | same | 57232 |
| 53962 | 3469 | 0 | for | `k <= 5 && target == -1` | 81634 | same | 57232 |
| 53962 | 3469 | 0 | for | `target == -1` | 81634 | same | 57232 |
| 53962 | 3469 | 0 | if | `!(*enemies)[k]->isDead()` | 81634 | same | 57232 |
| 53962 | 3469 | 1 | for | `k <= 5 && target == -1` | 81634 | same | 57232 |
| 53962 | 3469 | 1 | for | `target == -1` | 81634 | same | 57232 |
| 53962 | 3469 | 1 | if | `!(*enemies)[k]->isDead()` | 81634 | same | 57232 |
| 53962 | 3469 | 1 | if | `(*enemies)[k]->isDead()` | 81634 | same | 57232 |
| 53962 | 3476 | 1 | if | `((Mission *) (intptr_t) Globals::status->getCampaignMission())->getStatusValue()` | 81634 | same | 57232 |
| 53962 | 3506 | 1 | if | `m_nState == 5` | 81634 | same | 57232 |
| 53962 | 3517 | 1 | if | `m_nState == 6` | 81634 | same | 57232 |
| 53962 | 3523 | 1 | if | `m_nScriptTimerA >= 4001` | 81634 | same | 57232 |
| 53962 | 3546 | 0 | for | `i != 2` | 81634 | same | 57232 |
| 53962 | 3546 | 1 | for | `i != 2` | 81634 | same | 57232 |
| 53962 | 3552 | 0 | if | `m_nState == 1` | 81634 | same | 57232 |
| 53962 | 3562 | 1 | if | `m_nState == 2` | 81634 | same | 57232 |
| 53962 | 3590 | 1 | if | `m_nState == 3` | 81634 | same | 57232 |
| 53962 | 3595 | 0 | for | `i < 2` | 81634 | same | 57232 |
| 53962 | 3595 | 1 | for | `i < 2` | 81634 | same | 57232 |
| 53962 | 3602 | 1 | if | `m_nState == 4` | 81634 | same | 57232 |
| 53962 | 3610 | 1 | if | `m_nScriptTimerA > 3000` | 81634 | same | 57232 |
| 53962 | 3625 | 1 | if | `m_nState == 5` | 81634 | same | 57232 |
| 53962 | 3627 | 1 | if | `m_nScriptTimerA >= 10001` | 81634 | same | 57232 |
| 53962 | 3637 | 1 | if | `m_nScriptTimerA >= 24001` | 81634 | same | 57232 |
| 53962 | 3660 | 1 | for | `i < 5` | 81634 | same | 57232 |
| 53962 | 3697 | 1 | if | `m_nState == 7` | 81634 | same | 57232 |
| 53962 | 3705 | 1 | if | `i == 2` | 81634 | same | 57232 |
| 53962 | 3730 | 1 | if | `m_nScriptTimerA >= 10001` | 81634 | same | 57232 |
| 53962 | 3749 | 1 | if | `m_nState == 8` | 81634 | same | 57232 |
| 53962 | 3789 | 1 | if | `((RadioMessage *) ((*messages)[7]))->isOver()` | 81634 | same | 57232 |
| 53962 | 3825 | 1 | if | `m_nState == 11` | 81634 | same | 57232 |
| 53962 | 3827 | 1 | if | `m_nScriptTimerA >= 2501` | 81634 | same | 57232 |
| 53962 | 3828 | 1 | if | `m_pGeometry6 == nullptr` | 81634 | same | 57232 |
| 53962 | 3840 | 1 | if | `m_nScriptTimerA >= 9001` | 81634 | same | 57232 |
| 53962 | 3841 | 0 | if | `m_nScriptTimerA - delta <= 9000` | 81634 | same | 57232 |
| 53962 | 3841 | 1 | if | `m_nScriptTimerA - delta <= 9000` | 81634 | same | 57232 |
| 53962 | 3862 | 1 | for | `i < enemies->count` | 81634 | same | 57232 |
| 53962 | 3863 | 1 | if | `mover != nullptr` | 81634 | same | 57232 |
| 53962 | 3863 | 1 | if | `mover != nullptr && mover->shipGroup == 10` | 81634 | same | 57232 |
| 53962 | 3864 | 1 | if | `mover->shipGroup == 10` | 81634 | same | 57232 |
| 53962 | 3867 | 0 | if | `!Globals::layout->isFading()` | 81634 | same | 57232 |
| 53962 | 3867 | 1 | if | `Globals::layout->isFading()` | 81634 | same | 57232 |
| 53962 | 3881 | 0 | if | `m_nState == 13` | 81634 | same | 57232 |
| 53962 | 3881 | 1 | if | `m_nState == 13` | 81634 | same | 57232 |
| 53962 | 3887 | 1 | if | `m_nScriptTimerA >= 6001` | 81634 | same | 57232 |
| 53962 | 3913 | 1 | if | `e->player->isActive()` | 81634 | same | 57232 |
| 53962 | 3914 | 0 | for | `k < enemies->count` | 81634 | same | 57232 |
| 53962 | 3914 | 1 | for | `k < enemies->count` | 81634 | same | 57232 |
| 53962 | 3952 | 1 | for | `i < 6` | 81634 | same | 57232 |
| 53962 | 3972 | 1 | if | `player->isInRocketControl()` | 81634 | same | 57232 |
| 53962 | 4000 | 1 | for | `i < 6` | 81634 | same | 57232 |
| 53962 | 4004 | 1 | if | `m_nScriptTimerA >= 7001` | 81634 | same | 57232 |
| 53962 | 4005 | 1 | for | `i < 6` | 81634 | same | 57232 |
| 53962 | 4012 | 0 | if | `!m_pLevel->getPlayer()->isDockedToDockingPoint()` | 81634 | same | 57232 |
| 53962 | 4031 | 1 | if | `m_nState >= 4` | 81634 | same | 57232 |
| 53962 | 4031 | 1 | if | `m_nState >= 4 && m_nState <= 6` | 81634 | same | 57232 |
| 53962 | 4034 | 1 | if | `player->getHackingGameDockIndex() >= 0` | 81634 | same | 57232 |
| 53962 | 4058 | 1 | if | `m_nState == 0` | 81634 | same | 57232 |
| 53962 | 4063 | 0 | if | `player->isDockedToDockingPoint() \|\|                         m_nScriptTime >= 6` | 81634 | same | 57232 |
| 53962 | 4063 | 1 | if | `player->isDockedToDockingPoint()` | 81634 | same | 57232 |
| 53962 | 4068 | 0 | if | `(*enemies)[i]->shipGroup == 8` | 81634 | same | 57232 |
| 53962 | 4068 | 1 | for | `i < enemies->count` | 81634 | same | 57232 |
| 53962 | 4088 | 1 | if | `m_nState == 1` | 81634 | same | 57232 |
| 53962 | 4095 | 1 | if | `m_nScriptTimerA < 75001` | 81634 | same | 57232 |
| 53962 | 4189 | 0 | for | `i < 2` | 81634 | same | 57232 |
| 53962 | 4189 | 1 | for | `i < 2` | 81634 | same | 57232 |
| 53962 | 4211 | 1 | if | `(*enemies)[0]->player->isAlwaysEnemy()` | 81634 | same | 57232 |
| 53962 | 4215 | 0 | if | `!(*enemies)[i]->isWingMan()` | 81634 | same | 57232 |
| 53962 | 4215 | 1 | for | `i < m_pLevel->getEnemies()->count` | 81634 | same | 57232 |
| 53962 | 4215 | 1 | if | `!(*enemies)[i]->isWingMan() && (*enemies)[i]->field_0x3f_b == 0` | 81634 | same | 57232 |
| 53962 | 4215 | 1 | if | `(*enemies)[i]->isWingMan()` | 81634 | same | 57232 |
| 53962 | 4216 | 0 | if | `(*enemies)[i]->field_0x3f_b == 0` | 81634 | same | 57232 |
| 53962 | 4220 | 0 | if | `player->isDockedToDockingPoint()` | 81634 | same | 57232 |
| 53962 | 4220 | 0 | if | `player->isDockedToDockingPoint() && player->hackingWon() &&                     ` | 81634 | same | 57232 |
| 53962 | 4220 | 1 | if | `player->isDockedToDockingPoint()` | 81634 | same | 57232 |
| 53962 | 4221 | 0 | if | `player->getHackingGameDockIndex() >= 0` | 81634 | same | 57232 |
| 53962 | 4221 | 0 | if | `player->hackingWon()` | 81634 | same | 57232 |
| 53962 | 4221 | 1 | if | `player->getHackingGameDockIndex() >= 0` | 81634 | same | 57232 |
| 53962 | 4221 | 1 | if | `player->hackingWon()` | 81634 | same | 57232 |
| 53962 | 4224 | 0 | if | `target->getDockingType() == 3` | 81634 | same | 57232 |
| 53962 | 4224 | 0 | if | `target->getDockingType() == 3 && dockingCount == 1` | 81634 | same | 57232 |
| 53962 | 4224 | 1 | if | `target->getDockingType() == 3` | 81634 | same | 57232 |
| 53962 | 4224 | 1 | if | `target->getDockingType() == 3 && dockingCount == 1` | 81634 | same | 57232 |
| 53962 | 4253 | 1 | for | `i < m_pLevel->getGasClouds()->count` | 81634 | same | 57232 |
| 53962 | 4253 | 1 | if | `((PlayerGasCloud *) (*m_pLevel->getGasClouds())[i])->getSparks() != nullptr` | 81634 | same | 57232 |
| 53962 | 4260 | 1 | if | `m_nState == 2` | 81634 | same | 57232 |
| 53962 | 4262 | 0 | if | `Globals::status->getShip()->hasCargo(201, 1)` | 81634 | same | 57232 |
| 53962 | 4262 | 0 | if | `Globals::status->getShip()->hasCargo(201, 1)                         \|\| Global` | 81634 | same | 57232 |
| 53962 | 4263 | 0 | if | `Globals::status->getShip()->hasCargo(202, 1)` | 81634 | same | 57232 |
| 53962 | 4264 | 0 | if | `Globals::status->getShip()->hasCargo(203, 1)` | 81634 | same | 57232 |
| 53962 | 4265 | 1 | if | `Globals::status->getShip()->hasCargo(204, 1)` | 81634 | same | 57232 |
| 53962 | 4282 | 0 | if | `m_nState == 2` | 81634 | same | 57232 |
| 53962 | 4282 | 1 | if | `m_nState == 2` | 81634 | same | 57232 |
| 53962 | 4290 | 1 | for | `i < enemies->count` | 81634 | same | 57232 |
| 53962 | 4298 | 1 | if | `m_nScriptTimerA > 4000` | 81634 | same | 57232 |
| 53962 | 4300 | 1 | if | `m_nScriptTimerA >= 7001` | 81634 | same | 57232 |
| 53962 | 4320 | 1 | if | `m_nState >= 2` | 81634 | same | 57232 |
| 53962 | 4321 | 0 | if | `m_nState != 4` | 81634 | same | 57232 |
| 53962 | 4321 | 1 | if | `m_nState != 4` | 81634 | same | 57232 |
| 53962 | 4326 | 1 | switch | `m_nState` | 81634 | same | 57232 |
| 53962 | 4328 | 0 | for | `i < 13` | 81634 | same | 57232 |
| 53962 | 4340 | 1 | if | `!((RadioMessage *) ((*messages)[0]))->isOver()` | 81634 | same | 57232 |
| 53962 | 4340 | 1 | if | `((RadioMessage *) ((*messages)[0]))->isOver()` | 81634 | same | 57232 |
| 53962 | 4344 | 0 | if | `player->isInRocketControl()` | 81634 | same | 57232 |
| 53962 | 4344 | 1 | if | `player->isInRocketControl()` | 81634 | same | 57232 |
| 53962 | 4428 | 1 | if | `m_nScriptTimerA > 2000` | 81634 | same | 57232 |
| 53962 | 4428 | 1 | if | `m_nScriptTimerA > 2000 && m_nScriptTimerA < 5000` | 81634 | same | 57232 |
| 53962 | 4429 | 1 | if | `m_nScriptTimerA < 5000` | 81634 | same | 57232 |
| 53962 | 4438 | 1 | if | `m_nScriptTimerA > 10000` | 81634 | same | 57232 |
| 53962 | 4469 | 1 | if | `((RadioMessage *) ((*messages)[0]))->isTriggered()` | 81634 | same | 57232 |
| 53962 | 4470 | 1 | if | `player->isInRocketControl()` | 81634 | same | 57232 |
| 53962 | 4511 | 1 | if | `m_nState == 1` | 81634 | same | 57232 |
| 53962 | 4516 | 1 | for | `i < enemies->count` | 81634 | same | 57232 |
| 53962 | 4523 | 1 | if | `m_nScriptTimerA >= 1501` | 81634 | same | 57232 |
| 53962 | 4571 | 0 | if | `((RadioMessage *) ((*messages)[3]))->isOver()` | 81634 | same | 57232 |
| 53962 | 4571 | 1 | if | `((RadioMessage *) ((*messages)[3]))->isOver()` | 81634 | same | 57232 |
| 53962 | 4668 | 0 | if | `m_nState == 7` | 81634 | same | 57232 |
| 53962 | 4668 | 1 | if | `m_nState == 7` | 81634 | same | 57232 |
| 53962 | 4673 | 0 | if | `m_nState == 8` | 81634 | same | 57232 |
| 53962 | 4673 | 1 | if | `m_nState == 8` | 81634 | same | 57232 |
| 53962 | 4680 | 1 | if | `((RadioMessage *) ((*messages)[7]))->isOver()` | 81634 | same | 57232 |
| 53962 | 4685 | 1 | for | `i < enemies->count` | 81634 | same | 57232 |
| 53962 | 4686 | 1 | if | `ship->shipGroup == 9` | 81634 | same | 57232 |
| 53962 | 4721 | 1 | if | `((RadioMessage *) ((*messages)[8]))->isOver()` | 81634 | same | 57232 |
| 53962 | 4722 | 1 | if | `m_nTimeLimit <= 0` | 81634 | same | 57232 |
| 53962 | 4726 | 1 | if | `player->isDockedToDockingPoint()` | 81634 | same | 57232 |
| 53962 | 4726 | 1 | if | `player->isDockedToDockingPoint() && player->hackingWon()` | 81634 | same | 57232 |
| 53962 | 4727 | 1 | if | `player->hackingWon()` | 81634 | same | 57232 |
| 53962 | 4730 | 0 | if | `(*enemies)[i]->shipGroup == 9` | 81634 | same | 57232 |
| 53962 | 4730 | 1 | for | `i < enemies->count` | 81634 | same | 57232 |
| 53962 | 4730 | 1 | if | `(*enemies)[i]->shipGroup == 9` | 81634 | same | 57232 |
| 53962 | 4745 | 0 | if | `!Globals::isRunningHDonWeakDevice` | 81634 | same | 57232 |
| 53962 | 4745 | 1 | if | `!Globals::isRunningHDonWeakDevice` | 81634 | same | 57232 |
| 53962 | 4766 | 1 | if | `((RadioMessage *) ((*messages)[0]))->isOver()` | 81634 | same | 57232 |
| 53962 | 4784 | 1 | if | `m_nState == 2` | 81634 | same | 57232 |
| 53962 | 4788 | 1 | if | `m_nScriptTimerA >= 16001` | 81634 | same | 57232 |
| 53962 | 4791 | 0 | for | `i < enemies->count` | 81634 | same | 57232 |
| 53962 | 4791 | 1 | for | `i < enemies->count` | 81634 | same | 57232 |
| 53962 | 4792 | 0 | if | `ship->shipGroupFlag == 49` | 81634 | same | 57232 |
| 53962 | 4792 | 0 | if | `ship->shipGroupFlag == 49 \|\| ship->shipGroupFlag == 44` | 81634 | same | 57232 |
| 53962 | 4792 | 1 | if | `ship->shipGroupFlag == 49` | 81634 | same | 57232 |
| 53962 | 4792 | 1 | if | `ship->shipGroupFlag == 49 \|\| ship->shipGroupFlag == 44` | 81634 | same | 57232 |
| 53962 | 4803 | 0 | if | `m_nState == 3` | 81634 | same | 57232 |
| 53962 | 4803 | 1 | if | `m_nState == 3` | 81634 | same | 57232 |
| 53962 | 4806 | 0 | if | `lift < 0.0f` | 81634 | same | 57232 |
| 53962 | 4814 | 0 | if | `m_nState == 4` | 81634 | same | 57232 |
| 53962 | 4814 | 1 | if | `m_nState == 4` | 81634 | same | 57232 |
| 53962 | 4825 | 0 | for | `i < enemies->count` | 81634 | same | 57232 |
| 53962 | 4825 | 1 | for | `i < enemies->count` | 81634 | same | 57232 |
| 53962 | 4826 | 0 | if | `ship->shipGroupFlag == 49` | 81634 | same | 57232 |
| 53962 | 4826 | 0 | if | `ship->shipGroupFlag == 49 \|\| ship->shipGroupFlag == 44` | 81634 | same | 57232 |
| 53962 | 4826 | 1 | if | `ship->shipGroupFlag == 49` | 81634 | same | 57232 |
| 53962 | 4826 | 1 | if | `ship->shipGroupFlag == 49 \|\| ship->shipGroupFlag == 44` | 81634 | same | 57232 |
| 53962 | 4827 | 1 | if | `ship->shipGroupFlag == 44` | 81634 | same | 57232 |
| 53962 | 4830 | 1 | if | `((RadioMessage *) ((*messages)[3]))->isOver()` | 81634 | same | 57232 |
| 53962 | 4830 | 1 | if | `((RadioMessage *) ((*messages)[3]))->isOver() && m_nScriptTimerA >= 18001` | 81634 | same | 57232 |
| 53962 | 4844 | 0 | if | `(*enemies)[i]->shipGroupFlag == 49` | 81634 | same | 57232 |
| 53962 | 4844 | 0 | if | `(*enemies)[i]->shipGroupFlag == 49 \|\| (*enemies)[i]->shipGroupFlag == 44` | 81634 | same | 57232 |
| 53962 | 4867 | 1 | if | `m_nState == 5` | 81634 | same | 57232 |
| 53962 | 4871 | 1 | for | `i < 12` | 81634 | same | 57232 |
| 53962 | 4886 | 1 | if | `(destroyed >= 9 && m_nScriptTime > 200000)` | 81634 | same | 57232 |
| 53962 | 4887 | 1 | if | `(*enemies)[fighterIndex]->player->getHitpoints()                            < (*` | 81634 | same | 57232 |
| 53962 | 4921 | 1 | if | `m_nState == 6` | 81634 | same | 57232 |
| 53962 | 4988 | 1 | if | `m_nState == 9` | 81634 | same | 57232 |
| 53962 | 4998 | 1 | if | `m_nScriptTimerB < 9000` | 81634 | same | 57232 |
| 53962 | 4998 | 1 | if | `m_nScriptTimerB < 9000 && m_nScriptTimerB + delta >= 9000` | 81634 | same | 57232 |
| 53962 | 5007 | 1 | if | `m_pExplosion == nullptr` | 81634 | same | 57232 |
| 53962 | 5016 | 1 | if | `m_nFlashCountdown > 0` | 81634 | same | 57232 |
| 53962 | 5054 | 1 | if | `m_nState == 10` | 81634 | same | 57232 |
| 53962 | 5061 | 0 | if | `m_nScriptTimerB + delta >= 9000` | 81634 | same | 57232 |
| 53962 | 5061 | 1 | if | `m_nScriptTimerB + delta >= 9000` | 81634 | same | 57232 |
| 53962 | 5061 | 1 | if | `m_nScriptTimerB < 9000` | 81634 | same | 57232 |
| 53962 | 5085 | 0 | if | `m_nState == 11` | 81634 | same | 57232 |
| 53962 | 5085 | 1 | if | `m_nState == 11` | 81634 | same | 57232 |
| 53962 | 5088 | 1 | if | `m_nScriptTimerA >= 6001` | 81634 | same | 57232 |
| 53962 | 5089 | 0 | if | `m_nScriptTimerA - delta <= 6000` | 81634 | same | 57232 |
| 53962 | 5089 | 1 | if | `m_nScriptTimerA - delta <= 6000` | 81634 | same | 57232 |
| 53962 | 5111 | 1 | if | `m_nScriptTimerA < 7000` | 81634 | same | 57232 |
| 53962 | 5114 | 1 | if | `m_nScriptTimerA < 5000` | 81634 | same | 57232 |
| 53962 | 5122 | 1 | if | `m_nScriptTimerA >= 5000` | 81634 | same | 57232 |
| 53962 | 5122 | 1 | if | `m_nScriptTimerA >= 5000 && m_nScriptTimerA - delta < 5000` | 81634 | same | 57232 |
| 53962 | 5125 | 1 | if | `m_nScriptTimerA >= 5500` | 81634 | same | 57232 |
| 53962 | 5125 | 1 | if | `m_nScriptTimerA >= 5500 && m_nScriptTimerA - delta < 5500` | 81634 | same | 57232 |
| 53962 | 5131 | 1 | if | `m_nScriptTimerA >= 5700` | 81634 | same | 57232 |
| 53962 | 5131 | 1 | if | `m_nScriptTimerA >= 5700 && m_nScriptTimerA - delta < 5700` | 81634 | same | 57232 |
| 53962 | 5140 | 1 | if | `m_nScriptTimerA > 5000` | 81634 | same | 57232 |
| 53962 | 5140 | 1 | if | `m_nScriptTimerA > 5000 && m_nScriptTimerA < 6000` | 81634 | same | 57232 |
| 53962 | 5142 | 0 | if | `m_nScriptTimerA < 5500` | 81634 | same | 57232 |
| 53962 | 5180 | 1 | if | `m_nScriptTimerA > 25000` | 81634 | same | 57232 |
| 53962 | 5181 | 0 | if | `!(*enemies)[0]->isVisible()` | 81634 | same | 57232 |
| 53962 | 5200 | 0 | if | `m_nScriptTimerA <= 2999` | 81634 | same | 57232 |
| 53962 | 5205 | 0 | if | `m_nScriptTimerB >= 12001` | 81634 | same | 57232 |
| 53962 | 5205 | 0 | if | `m_nScriptTimerB >= 12001 && ((RadioMessage *) ((*messages)[2]))->isOver()` | 81634 | same | 57232 |
| 53962 | 5205 | 1 | if | `m_nScriptTimerB >= 12001` | 81634 | same | 57232 |
| 53962 | 5205 | 1 | if | `m_nScriptTimerB >= 12001 && ((RadioMessage *) ((*messages)[2]))->isOver()` | 81634 | same | 57232 |
| 53962 | 5240 | 0 | if | `(*enemies)[i]->player->isActive()` | 81634 | same | 57232 |
| 53962 | 5272 | 1 | if | `distance <= 49999` | 81634 | same | 57232 |
| 53962 | 5273 | 0 | for | `i < enemies->count` | 81634 | same | 57232 |
| 53962 | 5275 | 0 | if | `i != 0` | 81634 | same | 57232 |
| 53962 | 5275 | 1 | if | `i != 0 && (*enemies)[i - 1]->field_0x3e != 0` | 81634 | same | 57232 |
| 53962 | 5287 | 1 | for | `i < enemies->count` | 81634 | same | 57232 |
| 53962 | 5293 | 1 | for | `i < Globals::status->field_90->count` | 81634 | same | 57232 |
| 53963 | 1030 | 1 | if | `m_nState == 3` | 81635 | same | 57232 |
| 53963 | 1775 | 1 | if | `((RadioMessage *) ((*messages)[2]))->isTriggered() && m_nState == 0` | 81635 | same | 57232 |
| 53963 | 1775 | 1 | if | `m_nState == 0` | 81635 | same | 57232 |
| 53963 | 2174 | 0 | if | `m_nScriptTimerA >= 15001` | 81635 | same | 57232 |
| 53963 | 2808 | 1 | if | `((RadioMessage *) ((*messages)[3]))->isTriggered()` | 81635 | same | 57232 |
| 53963 | 4425 | 1 | for | `i < 13` | 81635 | same | 57232 |
| 53963 | 5122 | 0 | if | `m_nScriptTimerA >= 5000` | 81637 | ROTATED | 57232 |
| 53963 | 5122 | 0 | if | `m_nScriptTimerA >= 5000 && m_nScriptTimerA - delta < 5000` | 81637 | ROTATED | 57232 |
| 53964 | 1119 | 0 | if | `m_nScriptTime >= 27001` | 81636 | same | 57232 |
| 53964 | 4215 | 0 | for | `i < m_pLevel->getEnemies()->count` | 81636 | same | 57232 |
| 53964 | 4215 | 0 | if | `(*enemies)[i]->isWingMan()` | 81636 | same | 57232 |
| 53964 | 4290 | 1 | if | `(*enemies)[i]->shipGroupFlag == 44` | 81636 | same | 57232 |
| 53964 | 4616 | 0 | if | `((RadioMessage *) ((*messages)[5]))->isOver()` | 81636 | same | 57232 |
| 53964 | 5123 | 1 | if | `m_nScriptTimerA - delta < 5000` | 81636 | same | 57232 |
| 53965 | 5114 | 0 | if | `m_nScriptTimerA < 5000` | 81639 | ROTATED | 57232 |
| 53966 | 4033 | 1 | if | `player->isDockedToDockingPoint()` | 81638 | same | 57232 |
| 53966 | 4034 | 1 | if | `player->hackingWon()` | 81638 | same | 57232 |
| 53967 | 1289 | 1 | if | `m_nScriptTime >= 38001` | 81639 | same | 57232 |
| 53967 | 1295 | 1 | if | `m_nScriptTime >= 39001` | 81639 | same | 57232 |
| 53967 | 2021 | 1 | if | `((RadioMessage *) ((*messages)[5]))->isTriggered()` | 81639 | same | 57232 |
| 53967 | 2021 | 1 | if | `m_nState == 1` | 81639 | same | 57232 |
| 53967 | 2459 | 1 | if | `((RadioMessage *) ((*messages)[0]))->isTriggered()` | 81639 | same | 57232 |
| 53967 | 2459 | 1 | if | `m_nState == 0` | 81639 | same | 57232 |
| 53967 | 3517 | 0 | if | `m_nState == 6` | 81639 | same | 57232 |
| 53967 | 3749 | 0 | if | `m_nState == 8` | 81639 | same | 57232 |
| 53967 | 4012 | 1 | if | `m_pLevel->getPlayer()->isDockedToDockingPoint()` | 81639 | same | 57232 |
| 53967 | 4032 | 1 | if | `m_nState <= 6` | 81639 | same | 57232 |
| 53967 | 4033 | 1 | if | `player->isDockedToDockingPoint() && player->hackingWon()                        ` | 81639 | same | 57232 |
| 53967 | 5141 | 0 | if | `m_nScriptTimerA < 6000` | 81639 | same | 57232 |
| 53967 | 5141 | 1 | if | `m_nScriptTimerA < 6000` | 81639 | same | 57232 |
| 53968 | 1657 | 1 | if | `((RadioMessage *) ((*messages)[0]))->isTriggered()` | 81640 | same | 57232 |
| 53968 | 1657 | 1 | if | `m_nState == 0` | 81640 | same | 57232 |
| 53968 | 2819 | 1 | if | `player->isDockedToDockingPoint()` | 81640 | same | 57232 |
| 53969 | 1330 | 0 | if | `m_nScriptTimerA >= 8000` | 81641 | same | 57232 |
| 53969 | 1586 | 1 | if | `((RadioMessage *) ((*messages)[1]))->isOver()` | 81641 | same | 57232 |
| 53969 | 1586 | 1 | if | `m_nState == 1` | 81641 | same | 57232 |
| 53969 | 4144 | 1 | if | `m_nState == 2` | 81641 | same | 57232 |
| 53969 | 4253 | 0 | if | `((PlayerGasCloud *) (*m_pLevel->getGasClouds())[i])->getSparks() != nullptr` | 81641 | same | 57232 |
| 53969 | 4260 | 0 | if | `m_nState == 2` | 81641 | same | 57232 |
| 53970 | 2835 | 1 | if | `reinterpret_cast<float &>(speedBits) < 100.0f` | 81642 | same | 57232 |
| 53971 | 1080 | 0 | if | `m_nScriptTime >= 7901` | 81643 | same | 57232 |
| 53974 | 3607 | 1 | for | `i < 2` | 81646 | same | 57232 |
| 53975 | 5181 | 1 | if | `(*enemies)[0]->isVisible()` | 81647 | same | 57232 |
| 53975 | 5189 | 1 | if | `m_nScriptTimerA >= 20000` | 81647 | same | 57232 |
| 53976 | 3552 | 1 | if | `m_nState == 1` | 81648 | same | 57232 |
| 53977 | 4601 | 0 | if | `((RadioMessage *) ((*messages)[4]))->isOver()` | 81649 | same | 57232 |
| 53978 | 1017 | 1 | if | `m_nScriptTime >= 12001` | 81650 | same | 57232 |
| 53987 | 2353 | 1 | if | `((RadioMessage *) ((*messages)[7]))->isTriggered()` | 81659 | same | 57232 |
| 53988 | 4938 | 1 | if | `((RadioMessage *) ((*messages)[10]))->isOver()` | 81660 | same | 57232 |
| 53990 | 2838 | 1 | if | `m_nScriptTimerA >= 8001` | 81662 | same | 57232 |
| 54010 | 4806 | 1 | if | `lift < 0.0f` | 81682 | same | 57232 |
| 54011 | 4290 | 0 | for | `i < enemies->count` | 81683 | same | 57232 |
| 54023 | 3912 | 1 | if | `e->shipGroup == 8 && e->player->isActive()` | 81695 | same | 57232 |
| 54025 | 4586 | 0 | if | `m_nState == 4` | 81697 | same | 57232 |
| 54030 | 4586 | 1 | if | `m_nState == 4` | 81702 | same | 57232 |
| 54032 | 2827 | 1 | if | `((RadioMessage *) ((*messages)[6]))->isOver()` | 81704 | same | 57232 |
| 54035 | 768 | 1 | if | `((RadioMessage *) ((*messages)[15]))->isOver()` | 81707 | same | 57232 |
| 54044 | 4679 | 1 | if | `m_nState == 9` | 81716 | same | 57232 |
| 54046 | 4679 | 0 | if | `m_nState == 9` | 81718 | same | 57232 |
| 54053 | 4278 | 0 | if | `m_nState == 1` | 81725 | same | 57232 |
| 54055 | 4198 | 1 | if | `player->hackingWon() && player->isDockedToDockingPoint() &&                     ` | 81728 | ROTATED | 57232 |
| 54057 | 4107 | 0 | if | `enemy->shipGroup == 8 && enemy->isDead()` | 81729 | same | 57232 |
| 54057 | 4199 | 1 | if | `player->getHackingGameDockIndex() >= 0` | 81729 | same | 57232 |
| 54057 | 4220 | 1 | if | `player->isDockedToDockingPoint() && player->hackingWon() &&                     ` | 81729 | same | 57232 |
| 54058 | 1131 | 0 | if | `m_nScriptTimerA >= 2001` | 81730 | same | 57232 |
| 54061 | 2118 | 1 | if | `player->isInWormhole()` | 81733 | same | 57232 |
| 54062 | 4092 | 0 | if | `statusValue >= amount / 2` | 81734 | same | 57232 |
| 54062 | 4107 | 1 | if | `enemy->shipGroup == 8` | 81734 | same | 57232 |
| 54062 | 4107 | 1 | if | `enemy->shipGroup == 8 && enemy->isDead()` | 81734 | same | 57232 |
| 54063 | 4844 | 1 | if | `(*enemies)[i]->shipGroupFlag == 49 \|\| (*enemies)[i]->shipGroupFlag == 44` | 81735 | same | 57232 |
| 54067 | 1432 | 1 | if | `m_nState == 1` | 81739 | same | 57232 |
| 54069 | 1814 | 1 | if | `((RadioMessage *) ((*messages)[4]))->isTriggered()` | 81741 | same | 57232 |
| 54069 | 1814 | 1 | if | `m_nState == 1` | 81741 | same | 57232 |
| 54080 | 1679 | 0 | if | `((RadioMessage *) ((*messages)[1]))->isOver()` | 81752 | same | 57232 |
| 54080 | 4105 | 1 | for | `i < enemies->count` | 81752 | same | 57232 |
| 54083 | 2150 | 1 | if | `m_nState == 0` | 81756 | ROTATED | 57232 |
| 54090 | 4190 | 1 | if | `((PlayerFixedObject *) (*enemies)[i])->getDockingType() != 3` | 81765 | ROTATED | 57232 |
| 54099 | 1641 | 0 | if | `m_nScriptTime >= m_nScriptTimerA + 4000` | 81771 | same | 57232 |
| 54099 | 1641 | 0 | if | `m_nState == 4 && m_nScriptTime >= m_nScriptTimerA + 4000` | 81771 | same | 57232 |
| 54116 | 2156 | 1 | if | `m_bRenderParticles` | 81792 | ROTATED | 57232 |
| 54116 | 4105 | 0 | for | `i < enemies->count` | 81788 | same | 57232 |
| 54116 | 4107 | 0 | if | `enemy->shipGroup == 8` | 81788 | same | 57232 |
| 54122 | 4266 | 0 | if | `Globals::status->getShip()->hasCargo(205, 1)` | 81794 | same | 57232 |
| 54150 | 3337 | 0 | if | `i == (unsigned int) (leader + 1)` | 81822 | same | 57232 |
| 54155 | 3513 | 1 | if | `m_nScriptTimerA >= 8001` | 81827 | same | 57232 |
| 54157 | 2566 | 0 | for | `j < enemies->count` | 81829 | same | 57232 |
| 54157 | 3403 | 0 | if | `leader == -1` | 81829 | same | 57232 |
| 54158 | 3403 | 1 | if | `leader == -1` | 81830 | same | 57232 |
| 54159 | 2815 | 0 | if | `m_nState == 1 && player->isDockedToDockingPoint()` | 81831 | same | 57232 |
| 54159 | 2815 | 0 | if | `player->isDockedToDockingPoint()` | 81831 | same | 57232 |
| 54161 | 3912 | 1 | if | `e->shipGroup == 8` | 81833 | same | 57232 |
| 54167 | 4745 | 1 | if | `Globals::isRunningHDonWeakDevice` | 81839 | same | 57232 |
| 54167 | 4749 | 1 | if | `m_pGeometry2->isVisible()` | 81839 | same | 57232 |
| 54167 | 4886 | 0 | if | `(destroyed >= 9 && m_nScriptTime > 200000)                         \|\| (*enemie` | 81839 | same | 57232 |
| 54185 | 3906 | 1 | if | `m_nState != 0` | 81857 | same | 57232 |
| 54187 | 4845 | 1 | if | `(*enemies)[i]->shipGroupFlag == 44` | 81859 | same | 57232 |
| 54219 | 2565 | 0 | if | `(*enemies)[i]->field_0x40 != 0 && (*enemies)[i]->player->empDisabledByte != 0` | 81891 | same | 57232 |
| 54219 | 2565 | 0 | if | `(*enemies)[i]->player->empDisabledByte != 0` | 81891 | same | 57232 |
| 54294 | 1418 | 1 | if | `Globals::status->getCurrentCampaignMission() == 95 \|\| Globals::status->getCurr` | 81966 | same | 57232 |
| 54294 | 1424 | 1 | if | `Globals::status->getCurrentCampaignMission() == 161` | 81966 | same | 57232 |
| 54375 | 1511 | 0 | if | `m_nState == 0` | 82065 | ROTATED | 57232 |
| 54375 | 1536 | 1 | if | `m_nState == 0` | 82065 | ROTATED | 57232 |
| 54375 | 1536 | 1 | if | `messages->count >= 3` | 82065 | ROTATED | 57232 |
| 54432 | 4469 | 1 | if | `m_nState == 0` | 82104 | same | 57232 |
| 54445 | 3642 | 0 | for | `i < 5` | 82117 | same | 57232 |
| 54455 | 5198 | 1 | if | `m_nState == 2` | 82127 | same | 57232 |
| 54456 | 3627 | 0 | if | `m_nScriptTimerA >= 10001` | 82128 | same | 57232 |
| 54476 | 4290 | 0 | if | `(*enemies)[i]->shipGroupFlag == 44` | 82148 | same | 57232 |
| 54476 | 4298 | 0 | if | `m_nScriptTimerA > 4000` | 82148 | same | 57232 |
| 54476 | 5142 | 1 | if | `m_nScriptTimerA < 5500` | 82148 | same | 57232 |
| 54548 | 3848 | 0 | if | `m_nScriptTimerA >= 10001` | 82222 | ROTATED | 57232 |
| 54618 | 1746 | 0 | if | `((RadioMessage *) ((*messages)[4]))->isOver()` | 82290 | same | 57232 |
| 54618 | 1746 | 0 | if | `((RadioMessage *) ((*messages)[4]))->isOver() && m_nState == 5` | 82290 | same | 57232 |
| 54618 | 1746 | 0 | if | `m_nState == 5` | 82290 | same | 57232 |
| 54618 | 2071 | 1 | if | `m_nScriptTimerA >= 15001` | 82290 | same | 57232 |
| 54670 | 2071 | 0 | if | `m_nScriptTimerA >= 15001` | 82342 | same | 57232 |
| 54673 | 1775 | 0 | if | `((RadioMessage *) ((*messages)[2]))->isTriggered()` | 82345 | same | 57232 |
| 54680 | 2565 | 1 | if | `(*enemies)[i]->field_0x40 != 0` | 82352 | same | 57232 |
| 54702 | 1732 | 1 | if | `m_nState == 4` | 82374 | same | 57232 |
| 54706 | 3205 | 1 | for | `i != 2` | 82378 | same | 57232 |
| 54720 | 2564 | 1 | for | `i < enemies->count` | 82392 | same | 57232 |
| 54733 | 2565 | 1 | if | `(*enemies)[i]->player->empDisabledByte != 0` | 82405 | same | 57232 |
| 54763 | 2268 | 0 | for | `i < enemies->count` | 82435 | same | 57232 |
| 54764 | 3446 | 1 | if | `m_nState == 4` | 82436 | same | 57232 |
| 54771 | 4520 | 1 | if | `((RadioMessage *) ((*messages)[1]))->isOver()` | 82443 | same | 57232 |
| 54781 | 1178 | 1 | if | `m_nScriptTimerA >= 4001` | 82454 | same | 57232 |
| 54790 | 2554 | 0 | for | `i < enemies->count` | 82462 | same | 57232 |
| 54792 | 3763 | 0 | if | `(*enemies)[i]->isDead()` | 82464 | same | 57232 |
| 54792 | 3788 | 0 | if | `m_nState == 10` | 82464 | same | 57232 |
| 54793 | 3762 | 0 | for | `i < 5` | 82465 | same | 57232 |
| 54798 | 3220 | 0 | if | `field_0xab` | 82470 | same | 57232 |
| 54799 | 3242 | 0 | for | `i != 4` | 82471 | same | 57232 |
| 54807 | 3910 | 0 | for | `i < enemies->count` | 82479 | same | 57232 |
| 54807 | 3912 | 0 | if | `e->shipGroup == 8` | 82479 | same | 57232 |
| 54817 | 714 | 1 | if | `reinterpret_cast<float &>(speedBits) == 2.0f` | 82489 | same | 57232 |
| 54832 | 1432 | 0 | if | `m_nState == 1` | 82504 | same | 57232 |
| 54834 | 1984 | 1 | if | `((RadioMessage *) ((*messages)[4]))->isTriggered()` | 82512 | ROTATED | 57232 |
| 54838 | 5231 | 0 | if | `m_nState == 3` | 82510 | same | 57232 |
| 54843 | 2150 | 0 | if | `!Globals::status->inAlienOrbit()` | 82520 | ROTATED | 57232 |
| 54843 | 2150 | 0 | if | `m_nState == 0 && !Globals::status->inAlienOrbit()` | 82520 | ROTATED | 57232 |
| 54843 | 2150 | 1 | if | `Globals::status->inAlienOrbit()` | 82520 | ROTATED | 57232 |
| 54843 | 4238 | 0 | if | `AbyssEngine::AEMath::VectorLength(                                 (*enemies)[0]` | 82515 | same | 57232 |
| 54851 | 5231 | 1 | if | `m_nState == 3` | 82523 | same | 57232 |
| 54853 | 3271 | 1 | if | `m_nState == 0` | 82525 | same | 57232 |
| 54854 | 2564 | 0 | for | `i < enemies->count` | 82526 | same | 57232 |
| 54856 | 1306 | 1 | if | `fading` | 82528 | same | 57232 |
| 54868 | 5096 | 0 | if | `m_nScriptTimerA >= 10001` | 82540 | same | 57232 |
| 54872 | 3406 | 0 | if | `i == (unsigned int) leader` | 82544 | same | 57232 |
| 54875 | 1428 | 0 | if | `m_nState == 0` | 82547 | same | 57232 |
| 54896 | 2962 | 0 | if | `m_nState == 2` | 82568 | same | 57232 |
| 54897 | 1128 | 1 | if | `m_nScriptTimerA < 1100 - delta` | 82569 | same | 57232 |
| 54901 | 3428 | 1 | if | `(*enemies)[i]->shipGroup == 10` | 82573 | same | 57232 |
| 54928 | 1955 | 0 | for | `i < enemies->count` | 82600 | same | 57232 |
| 54935 | 1030 | 0 | if | `m_nState == 3` | 82607 | same | 57232 |
| 54936 | 2580 | 0 | if | `((RadioMessage *) ((*messages)[7]))->isTriggered()` | 82608 | same | 57232 |
| 54952 | 3036 | 0 | if | `m_nState == 5` | 82624 | same | 57232 |
| 54960 | 4767 | 0 | if | `player->isInRocketControl()` | 82632 | same | 57232 |
| 54972 | 3032 | 0 | if | `m_nScriptTimerA > 2000` | 82644 | same | 57232 |
| 54977 | 2542 | 1 | switch | `m_nState` | 82649 | same | 57232 |
| 54997 | 3500 | 1 | for | `k != 10` | 82669 | same | 57232 |
| 55013 | 2050 | 1 | if | `m_nState == 3` | 82685 | same | 57232 |
| 55046 | 3134 | 0 | if | `m_nScriptTimerA >= 2001` | 82718 | same | 57232 |
| 55061 | 4721 | 0 | if | `((RadioMessage *) ((*messages)[8]))->isOver() && m_nTimeLimit <= 0` | 82733 | same | 57232 |
| 55075 | 4714 | 0 | if | `field_0xaa` | 82747 | same | 57232 |
| 55101 | 1351 | 1 | if | `Globals::status->getCurrentCampaignMission() == 106` | 82773 | same | 57232 |
| 55109 | 1383 | 1 | if | `m_nState == 3` | 82781 | same | 57232 |
| 55147 | 4098 | 0 | if | `m_nState == 2` | 82819 | same | 57232 |
| 55292 | 4485 | 1 | if | `field_0xaa` | 82964 | same | 57232 |
| 55311 | 1383 | 0 | if | `m_nState == 3` | 82983 | same | 57232 |
| 55403 | 1061 | 0 | switch | `m_nState` | 83107 | ROTATED | 57232 |
| 55418 | 1657 | 1 | if | `((RadioMessage *) ((*messages)[0]))->isTriggered() && m_nState == 0` | 83090 | same | 57232 |
| 55459 | 643 | 0 | if | `m_nState <= 2` | 83164 | ROTATED | 57232 |

## Existing-expect flip / removal table (112 variants, 59 size-exact)

Base raw 53,962.  `flip` = invert the weight, `rm` = strip the wrapper.
Winner: `rm ln2917 field_178 == 0` at 53,959 (landed as commit 3).

| raw | .text | action | rel.plt |
|---:|---:|---|---|
| 53959 | 81631 | rm ln2917 'Globals::status->field_178 == 0' 1 | same |
| 53962 | 81634 | flip ln1031 '!Globals::layout->isFading()' 1 | same |
| 53962 | 81634 | flip ln2852 'm_nState == 5' 1 | same |
| 53962 | 81634 | flip ln3781 'player->getHitpoints() >= 1 && // lint: arm_builti' 0 | same |
| 53962 | 81634 | rm ln1031 '!Globals::layout->isFading()' 1 | same |
| 53962 | 81634 | rm ln3781 'player->getHitpoints() >= 1 && // lint: arm_builti' 0 | same |
| 53962 | 81634 | rm ln4633 '(*enemies)[i]->shipGroup == 9' 1 | same |
| 53962 | 81634 | rm ln4684 'm_nScriptTimerA >= 2001' 1 | same |
| 53963 | 81635 | flip ln1336 '!Globals::layout->isFading()' 1 | same |
| 53963 | 81635 | flip ln1438 'm_nState == 2' 0 | same |
| 53963 | 81635 | flip ln2047 'm_nScriptTimerA >= 15001' 0 | same |
| 53963 | 81635 | flip ln2218 '(*enemies)[i]->player->isAlwaysFriend()' 0 | same |
| 53963 | 81635 | flip ln3708 'i == 3' 1 | same |
| 53963 | 81635 | flip ln4692 'i % 3 == 0' 1 | same |
| 53963 | 81635 | rm ln1112 'm_nScriptTimerA >= 1100 - delta' 0 | same |
| 53963 | 81635 | rm ln1336 '!Globals::layout->isFading()' 1 | same |
| 53963 | 81635 | rm ln1438 'm_nState == 2' 0 | same |
| 53963 | 81635 | rm ln2047 'm_nScriptTimerA >= 15001' 0 | same |
| 53963 | 81635 | rm ln2218 '(*enemies)[i]->player->isAlwaysFriend()' 0 | same |
| 53963 | 81635 | rm ln3708 'i == 3' 1 | same |
| 53963 | 81635 | rm ln4692 'i % 3 == 0' 1 | same |
| 53964 | 81636 | rm ln1886 'm_nState == 1' 1 | same |
| 53964 | 81636 | rm ln3567 '(((RadioMessage *) ((*messages)[1]))->isOver())' 1 | same |
| 53964 | 81636 | rm ln4292 '!((PlayerFighter *) (*enemies)[i])->field_0x13c &&' 1 | same |
| 53964 | 81636 | rm ln4927 'chaser->getPosition().x > (*enemies)[stationIndex]' 1 | same |
| 53965 | 81637 | rm ln2273 'm_nState == 2' 1 | same |
| 53965 | 81637 | rm ln3474 'target == -1' 1 | same |
| 53965 | 81637 | rm ln5040 '((RadioMessage *) ((*messages)[14]))->isOver()' 1 | same |
| 53966 | 81638 | rm ln1693 '((RadioMessage *) ((*messages)[2]))->isTriggered()' 1 | same |
| 53966 | 81638 | rm ln4375 'm_nScriptTimerA >= 1001' 1 | same |
| 53968 | 81640 | flip ln2854 '(m_nScriptTimerA >= 3001)' 0 | same |
| 53968 | 81640 | rm ln2854 '(m_nScriptTimerA >= 3001)' 0 | same |
| 53969 | 81641 | flip ln5071 '(((RadioMessage *) ((*messages)[15]))->isOver())' 0 | same |
| 53969 | 81641 | rm ln4816 'm_nScriptTimerB <= 6999' 1 | same |
| 53969 | 81641 | rm ln5071 '(((RadioMessage *) ((*messages)[15]))->isOver())' 0 | same |
| 53970 | 81642 | rm ln3322 'm_nScriptTimerA >= 30001' 1 | same |
| 53972 | 81644 | flip ln4406 'm_nScriptTimerA >= 324' 0 | same |
| 53972 | 81644 | rm ln4406 'm_nScriptTimerA >= 324' 0 | same |
| 53977 | 81649 | flip ln1465 'wingman->isWingMan()' 0 | same |
| 53977 | 81649 | rm ln1465 'wingman->isWingMan()' 0 | same |
| 53983 | 81655 | rm ln3156 '(m_nState == 11)' 1 | same |
| 53998 | 81670 | flip ln3128 'm_nState == 10' 0 | same |
| 53998 | 81670 | flip ln3128 'm_nState == 9' 0 | same |
| 54001 | 81673 | rm ln3113 'm_nState == 8' 0 | same |
| 54004 | 81676 | rm ln3128 'm_nState == 10' 0 | same |
| 54004 | 81676 | rm ln3128 'm_nState == 9' 0 | same |
| 54005 | 81677 | flip ln4877 'i < 22' 1 | same |
| 54012 | 81684 | flip ln4292 '!((PlayerFighter *) (*enemies)[i])->field_0x13c &&' 1 | same |
| 54012 | 81684 | rm ln1530 'Globals::status->getCurrentCampaignMission() == 51' 1 | same |
| 54019 | 81691 | flip ln4141 '*counter >= 3001' 1 | same |
| 54047 | 81719 | rm ln5238 'AbyssEngine::AEMath::VectorLength((*enemies)[0]->g' 1 | same |
| 54098 | 81770 | flip ln1112 'm_nScriptTimerA >= 1100 - delta' 0 | same |
| 54127 | 81799 | flip ln2061 'ship != nullptr && ship->shipGroup == 9' 0 | same |
| 54153 | 81825 | flip ln5040 '((RadioMessage *) ((*messages)[14]))->isOver()' 1 | same |
| 54869 | 82541 | flip ln3074 'm_nState == 7 && ((RadioMessage *) ((*messages)[8]' 1 | same |
| 54883 | 82555 | rm ln1433 '((RadioMessage *) ((*messages)[0]))->isOver()' 1 | same |
| 54930 | 82602 | flip ln1433 '((RadioMessage *) ((*messages)[0]))->isOver()' 1 | same |
| 54999 | 82671 | flip ln1473 'Globals::status->getCurrentCampaignMission() >= 2' 1 | same |
| 55044 | 82716 | flip ln1886 'm_nState == 1' 1 | same |
| size-change | - | flip ln1084 'm_nScriptTime >= 9001' 0 ['size', 57240] | - |
| size-change | - | flip ln1366 'm_nState == 2' 0 ['size', 57276] | - |
| size-change | - | flip ln1369 'm_nScriptTimerA >= 3001' 0 ['size', 57280] | - |
| size-change | - | flip ln1392 'm_nState == 5' 1 ['size', 57248] | - |
| size-change | - | flip ln1530 'Globals::status->getCurrentCampaignMission() == 51' 1 ['size', 57256] | - |
| size-change | - | flip ln1693 '((RadioMessage *) ((*messages)[2]))->isTriggered()' 1 ['size', 57240] | - |
| size-change | - | flip ln2273 'm_nState == 2' 1 ['size', 57240] | - |
| size-change | - | flip ln2395 '((RadioMessage *) ((*messages)[11]))->isOver() && ' 1 ['size', 57224] | - |
| size-change | - | flip ln2496 '((RadioMessage *) ((*messages)[1]))->isOver()' 0 ['size', 57240] | - |
| size-change | - | flip ln2917 'Globals::status->field_178 == 0' 1 ['size', 57236] | - |
| size-change | - | flip ln3113 'm_nState == 8' 0 ['size', 57224] | - |
| size-change | - | flip ln3116 'm_nScriptTimerA >= 8001' 1 ['size', 57228] | - |
| size-change | - | flip ln3156 '(m_nState == 11)' 1 ['size', 57236] | - |
| size-change | - | flip ln3322 'm_nScriptTimerA >= 30001' 1 ['size', 57236] | - |
| size-change | - | flip ln3474 'target == -1' 1 ['size', 57228] | - |
| size-change | - | flip ln3567 '(((RadioMessage *) ((*messages)[1]))->isOver())' 1 ['size', 57240] | - |
| size-change | - | flip ln3757 'm_nScriptTimerA >= 110001' 1 ['size', 57248] | - |
| size-change | - | flip ln3999 'm_nState == 3' 1 ['size', 57236] | - |
| size-change | - | flip ln4375 'm_nScriptTimerA >= 1001' 1 ['size', 57228] | - |
| size-change | - | flip ln4557 '((RadioMessage *) ((*messages)[2]))->isOver()' 0 ['size', 57248] | - |
| size-change | - | flip ln4620 'm_nScriptTimerA >= 1001' 0 ['size', 57228] | - |
| size-change | - | flip ln4633 '(*enemies)[i]->shipGroup == 9' 1 ['size', 57236] | - |
| size-change | - | flip ln4684 'm_nScriptTimerA >= 2001' 1 ['size', 57200] | - |
| size-change | - | flip ln4816 'm_nScriptTimerB <= 6999' 1 ['size', 57240] | - |
| size-change | - | flip ln4870 'Globals::isRunningHDonWeakDevice' 1 ['size', 57236] | - |
| size-change | - | flip ln4882 'm_nScriptTimerB >= 25001' 1 ['size', 57236] | - |
| size-change | - | flip ln4927 'chaser->getPosition().x > (*enemies)[stationIndex]' 1 ['size', 57236] | - |
| size-change | - | flip ln4943 'm_nScriptTimerA <= 9999' 1 ['size', 57248] | - |
| size-change | - | flip ln5118 'm_nScriptTimerA < 6000' 1 ['size', 57236] | - |
| size-change | - | flip ln5138 'm_nScriptTimerA >= 5500' 0 ['size', 57236] | - |
| size-change | - | flip ln5238 'AbyssEngine::AEMath::VectorLength((*enemies)[0]->g' 1 ['size', 57228] | - |
| size-change | - | rm ln1084 'm_nScriptTime >= 9001' 0 ['size', 57240] | - |
| size-change | - | rm ln1366 'm_nState == 2' 0 ['size', 57244] | - |
| size-change | - | rm ln1369 'm_nScriptTimerA >= 3001' 0 ['size', 57280] | - |
| size-change | - | rm ln1392 'm_nState == 5' 1 ['size', 57240] | - |
| size-change | - | rm ln1473 'Globals::status->getCurrentCampaignMission() >= 2' 1 ['size', 57240] | - |
| size-change | - | rm ln2061 'ship != nullptr && ship->shipGroup == 9' 0 ['size', 57240] | - |
| size-change | - | rm ln2395 '((RadioMessage *) ((*messages)[11]))->isOver() && ' 1 ['size', 57224] | - |
| size-change | - | rm ln2496 '((RadioMessage *) ((*messages)[1]))->isOver()' 0 ['size', 57240] | - |
| size-change | - | rm ln2852 'm_nState == 5' 1 ['size', 57224] | - |
| size-change | - | rm ln3074 'm_nState == 7 && ((RadioMessage *) ((*messages)[8]' 1 ['size', 57240] | - |
| size-change | - | rm ln3116 'm_nScriptTimerA >= 8001' 1 ['size', 57228] | - |
| size-change | - | rm ln3757 'm_nScriptTimerA >= 110001' 1 ['size', 57228] | - |
| size-change | - | rm ln3999 'm_nState == 3' 1 ['size', 57224] | - |
| size-change | - | rm ln4141 '*counter >= 3001' 1 ['size', 57240] | - |
| size-change | - | rm ln4557 '((RadioMessage *) ((*messages)[2]))->isOver()' 0 ['size', 57240] | - |
| size-change | - | rm ln4620 'm_nScriptTimerA >= 1001' 0 ['size', 57228] | - |
| size-change | - | rm ln4870 'Globals::isRunningHDonWeakDevice' 1 ['size', 57224] | - |
| size-change | - | rm ln4877 'i < 22' 1 ['size', 57236] | - |
| size-change | - | rm ln4882 'm_nScriptTimerB >= 25001' 1 ['size', 57224] | - |
| size-change | - | rm ln4943 'm_nScriptTimerA <= 9999' 1 ['size', 57240] | - |
| size-change | - | rm ln5118 'm_nScriptTimerA < 6000' 1 ['size', 57236] | - |
| size-change | - | rm ln5138 'm_nScriptTimerA >= 5500' 0 ['size', 57236] | - |

## Operand-order (swap) table (133 variants, 66 size-exact)

Base raw 53,959 (after commit 3).  `swap lnA <-> lnB` means the adjacent
`&&`/`||` operands were exchanged.  Winners: `ln1536` (53,956) and
`ln1526` (53,958), landed together as commit 4; `ln4199` (53,954 on the
post-commit-4 base) landed as commit 5.

| raw | .text | swap | rel.plt |
|---:|---:|---|---|
| 53954 | 81626 | swap ln4199 && 'player->isDockedToDockingPoint()' <-> 'player->hackingWon()' | same |
| 53955 | 81627 | swap ln1240 && 'm_nState >= 5' <-> 'm_nState <= 8' | same |
| 53955 | 81627 | swap ln1413 && 'm_nState >= 2' <-> 'm_nState <= 4' | same |
| 53955 | 81627 | swap ln1418 \|\| 'Globals::status->getCurrentCampaign' <-> 'Globals::status->getCurrentCampaign' | same |
| 53955 | 81627 | swap ln1418 \|\| 'Globals::status->getCurrentCampaign' <-> 'Globals::status->getCurrentCampaign' | same |
| 53955 | 81627 | swap ln1419 \|\| 'Globals::status->getCurrentCampaign' <-> 'Globals::status->getCurrentCampaign' | same |
| 53955 | 81627 | swap ln1420 \|\| 'Globals::status->getCurrentCampaign' <-> 'Globals::status->getCurrentCampaign' | same |
| 53955 | 81627 | swap ln1421 \|\| 'Globals::status->getCurrentCampaign' <-> 'Globals::status->getCurrentCampaign' | same |
| 53955 | 81627 | swap ln1422 \|\| 'Globals::status->getCurrentCampaign' <-> 'Globals::status->getCurrentCampaign' | same |
| 53955 | 81627 | swap ln1423 \|\| 'Globals::status->getCurrentCampaign' <-> 'Globals::status->getCurrentCampaign' | same |
| 53955 | 81627 | swap ln1519 && 'Globals::status->getCurrentCampaign' <-> 'Globals::status->inAlienOrbit()' | same |
| 53955 | 81627 | swap ln1720 && 'ship != nullptr' <-> 'ship->shipGroup == 9' | same |
| 53955 | 81627 | swap ln1738 && 'ship != nullptr' <-> 'ship->shipGroup == 9' | same |
| 53955 | 81627 | swap ln2383 && '((RadioMessage *) ((*messages)[5]))' <-> 'm_nState == 3' | same |
| 53955 | 81627 | swap ln2565 && '(*enemies)[i]->field_0x40 != 0' <-> '(*enemies)[i]->player->empDisabledB' | same |
| 53955 | 81627 | swap ln2789 && 'm_nState >= 7' <-> 'm_nState <= 9' | same |
| 53955 | 81627 | swap ln3128 \|\| '__builtin_expect(m_nState == 9, 0)' <-> '__builtin_expect(m_nState == 10, 0)' | same |
| 53955 | 81627 | swap ln3864 && 'mover != nullptr' <-> 'mover->shipGroup == 10' | same |
| 53955 | 81627 | swap ln4033 && 'm_nState >= 4' <-> 'm_nState <= 6' | same |
| 53955 | 81627 | swap ln4034 && 'player->hackingWon()' <-> 'player->getHackingGameDockIndex() >' | same |
| 53955 | 81627 | swap ln4034 && 'player->isDockedToDockingPoint()' <-> 'player->hackingWon()' | same |
| 53955 | 81627 | swap ln4199 && 'player->hackingWon()' <-> 'player->getHackingGameDockIndex() >' | ROTATED |
| 53955 | 81627 | swap ln4216 && '!(*enemies)[i]->isWingMan()' <-> '(*enemies)[i]->field_0x3f_b == 0' | same |
| 53955 | 81627 | swap ln4221 && 'player->hackingWon()' <-> 'player->getHackingGameDockIndex() >' | same |
| 53955 | 81627 | swap ln4263 \|\| 'Globals::status->getShip()->hasCarg' <-> 'Globals::status->getShip()->hasCarg' | same |
| 53955 | 81627 | swap ln4264 \|\| 'Globals::status->getShip()->hasCarg' <-> 'Globals::status->getShip()->hasCarg' | same |
| 53955 | 81627 | swap ln4265 \|\| 'Globals::status->getShip()->hasCarg' <-> 'Globals::status->getShip()->hasCarg' | same |
| 53955 | 81627 | swap ln4266 \|\| 'Globals::status->getShip()->hasCarg' <-> 'Globals::status->getShip()->hasCarg' | same |
| 53955 | 81627 | swap ln4429 && 'm_nScriptTimerA > 2000' <-> 'm_nScriptTimerA < 5000' | same |
| 53955 | 81627 | swap ln4727 && 'player->isDockedToDockingPoint()' <-> 'player->hackingWon()' | same |
| 53955 | 81627 | swap ln4793 \|\| 'ship->shipGroupFlag == 49' <-> 'ship->shipGroupFlag == 44' | same |
| 53955 | 81627 | swap ln4827 \|\| 'ship->shipGroupFlag == 49' <-> 'ship->shipGroupFlag == 44' | same |
| 53955 | 81627 | swap ln4845 \|\| '(*enemies)[i]->shipGroupFlag == 49' <-> '(*enemies)[i]->shipGroupFlag == 44' | same |
| 53955 | 81627 | swap ln4887 && 'destroyed >= 9' <-> 'm_nScriptTime > 200000' | same |
| 53955 | 81627 | swap ln4999 && 'm_nScriptTimerB < 9000' <-> 'm_nScriptTimerB + delta >= 9000' | same |
| 53955 | 81627 | swap ln5061 && 'm_nScriptTimerB < 9000' <-> 'm_nScriptTimerB + delta >= 9000' | same |
| 53955 | 81627 | swap ln5061 && 'm_nScriptTimerB > 2000' <-> 'm_nScriptTimerB < 9000' | same |
| 53955 | 81627 | swap ln5123 && 'm_nScriptTimerA >= 5000' <-> 'm_nScriptTimerA - delta < 5000' | same |
| 53955 | 81627 | swap ln5126 && 'm_nScriptTimerA >= 5500' <-> 'm_nScriptTimerA - delta < 5500' | same |
| 53955 | 81627 | swap ln5132 && 'm_nScriptTimerA >= 5700' <-> 'm_nScriptTimerA - delta < 5700' | same |
| 53955 | 81627 | swap ln5141 && 'm_nScriptTimerA > 5000' <-> 'm_nScriptTimerA < 6000' | same |
| 53955 | 81627 | swap ln973 && 'Globals::status->getCurrentCampaign' <-> 'Globals::status->inAlienOrbit()' | same |
| 53956 | 81628 | swap ln1526 && 'messages != nullptr' <-> 'Globals::status->getCurrentCampaign' | same |
| 53956 | 81628 | swap ln4221 && 'player->isDockedToDockingPoint()' <-> 'player->hackingWon()' | same |
| 53956 | 81628 | swap ln4225 && 'target->getDockingType() == 3' <-> 'dockingCount == 1' | same |
| 53956 | 81628 | swap ln4245 && 'player->getRoute() != 0' <-> '(uint8_t) ((Route *) (intptr_t) pla' | same |
| 53957 | 81629 | swap ln1560 && 'Globals::status->getMission()->isCa' <-> '!Globals::status->getMission()->isE' | same |
| 53957 | 81629 | swap ln3452 && '(*enemies)[i]->shipGroup == 10' <-> '(*enemies)[i]->isDead()' | same |
| 53958 | 81630 | swap ln1536 && '((RadioMessage *) ((*messages)[2]))' <-> 'messages->count >= 3' | same |
| 53958 | 81630 | swap ln2273 && '__builtin_expect(m_nState == 2, 1)' <-> '((RadioMessage *) ((*messages)[5]))' | same |
| 53959 | 81631 | swap ln4108 && 'enemy->shipGroup == 8' <-> 'enemy->isDead()' | same |
| 53959 | 81631 | swap ln4212 && '!(*enemies)[0]->player->isAlwaysEne' <-> '((RadioMessage *) ((*messages)[1]))' | same |
| 53961 | 81633 | swap ln4831 && '((RadioMessage *) ((*messages)[3]))' <-> 'm_nScriptTimerA >= 18001' | same |
| 53962 | 81634 | swap ln3781 && 'player->getHitpoints() >= 1' <-> '// lint: arm_builtin\n              ' | same |
| 53964 | 81636 | swap ln4064 \|\| 'player->isDockedToDockingPoint()' <-> 'm_nScriptTime >= 60001' | same |
| 53983 | 81655 | swap ln2395 && '((RadioMessage *) ((*messages)[11])' <-> 'm_nState == 6, 1' | same |
| 54020 | 81692 | swap ln3929 && 'm_nState == 0' <-> '((RadioMessage *) ((*messages)[1]))' | same |
| 54075 | 81747 | swap ln3074 && 'm_nState == 7' <-> '((RadioMessage *) ((*messages)[8]))' | same |
| 54119 | 81791 | swap ln2061 && 'ship != nullptr' <-> 'ship->shipGroup == 9, 0' | same |
| 54119 | 81791 | swap ln2296 && '((RadioMessage *) ((*messages)[1]))' <-> 'm_nState == 0' | same |
| 54157 | 81830 | swap ln1984 && '((RadioMessage *) ((*messages)[4]))' <-> 'm_nState == 0' | same |
| 54163 | 81836 | swap ln1967 && 'm_nState == 3' <-> '(*enemies)[0]->getPosition().z >= (' | same |
| 54259 | 81938 | swap ln1814 && '((RadioMessage *) ((*messages)[4]))' <-> 'm_nState == 1' | ROTATED |
| 54388 | 82078 | swap ln1536 && 'messages->count >= 3' <-> 'm_nState == 0' | ROTATED |
| 54947 | 82619 | swap ln2815 && 'm_nState == 1' <-> 'player->isDockedToDockingPoint()' | same |
| 55095 | 82767 | swap ln4938 && 'm_nState == 7' <-> '((RadioMessage *) ((*messages)[10])' | same |
| size-change | - | swap ln1326 && 'm_nScriptTimerA <= 7000' <-> 'm_nScriptTimerA + delta > 7000' ['size', 57216] | - |
| size-change | - | swap ln1383 && 'm_nState == 3' <-> '((RadioMessage *) ((*messages)[4]))' ['size', 57244] | - |
| size-change | - | swap ln1386 && 'm_nState == 4' <-> '((RadioMessage *) ((*messages)[4]))' ['size', 57244] | - |
| size-change | - | swap ln1398 && 'm_nState == 6' <-> '((RadioMessage *) ((*messages)[5]))' ['size', 57252] | - |
| size-change | - | swap ln1438 && '__builtin_expect(m_nState == 2, 0)' <-> 'm_nScriptTimerA >= 2000' ['size', 57224] | - |
| size-change | - | swap ln1472 && '__builtin_expect(Globals::status->g' <-> 'field_0x24 >= 7001' ['compile-error', "ror: expected unqualified-id\n    return m_bCinematicBreak != 0;\n    ^\n/dev/shm/w11501/sw_1472_&&_1/LevelScript.cpp:5309:1: error: extraneous closing brace ('}')\n}\n^\n9 warnings and 9 errors generated.\n"] | - |
| size-change | - | swap ln1472 && 'm_pLevel->getPlayer() != 0' <-> '__builtin_expect(Globals::status->g' ['compile-error', "o match this '('\n        if (__builtin_expect(Globals::status->getCurrentCampaignMission() >= 2, 1) // lint: arm_builtin && m_pLevel->getPlayer() != 0&&\n           ^\n9 warnings and 1 error generated.\n"] | - |
| size-change | - | swap ln1521 && '((RadioMessage *) ((*messages)[1]))' <-> 'm_nState == 0' ['size', 57296] | - |
| size-change | - | swap ln1530 && 'Globals::status->getCurrentCampaign' <-> 'messages != nullptr, 1' ['size', 57224] | - |
| size-change | - | swap ln1560 || '(Globals::status->getMission()->isC' <-> 'Globals::status->getCurrentCampaign' ['size', 57248] | - |
| size-change | - | swap ln1569 && '((RadioMessage *) ((*messages)[1]))' <-> 'm_nState == 0' ['size', 57224] | - |
| size-change | - | swap ln1586 && '((RadioMessage *) ((*messages)[1]))' <-> 'm_nState == 1' ['size', 57308] | - |
| size-change | - | swap ln1641 && 'm_nState == 4' <-> 'm_nScriptTime >= m_nScriptTimerA + ' ['size', 57252] | - |
| size-change | - | swap ln1648 && '((RadioMessage *) ((*messages)[4]))' <-> 'm_nState == 3' ['size', 57252] | - |
| size-change | - | swap ln1657 && '((RadioMessage *) ((*messages)[0]))' <-> 'm_nState == 0' ['size', 57248] | - |
| size-change | - | swap ln1690 && 'm_nState == 2' <-> 'm_nScriptTime >= 20001' ['size', 57240] | - |
| size-change | - | swap ln1693 && '__builtin_expect(((RadioMessage *) ' <-> 'm_nState == 3' ['size', 57240] | - |
| size-change | - | swap ln1746 && '((RadioMessage *) ((*messages)[4]))' <-> 'm_nState == 5' ['size', 57228] | - |
| size-change | - | swap ln1775 && '((RadioMessage *) ((*messages)[2]))' <-> 'm_nState == 0' ['size', 57284] | - |
| size-change | - | swap ln1784 && '((RadioMessage *) ((*messages)[3]))' <-> 'm_nState == 0' ['size', 57240] | - |
| size-change | - | swap ln1841 && '((RadioMessage *) ((*messages)[0]))' <-> 'm_nState == 0' ['size', 57240] | - |
| size-change | - | swap ln1886 && '((RadioMessage *) ((*messages)[2]))' <-> '__builtin_expect(m_nState == 1, 1)' ['size', 57248] | - |
| size-change | - | swap ln1953 && 'm_nState == 2' <-> '(*enemies)[0]->getPosition().z >= 9' ['size', 57248] | - |
| size-change | - | swap ln2021 && '((RadioMessage *) ((*messages)[5]))' <-> 'm_nState == 1' ['size', 57252] | - |
| size-change | - | swap ln2092 && 'm_nState == 5' <-> '((RadioMessage *) ((*messages)[7]))' ['size', 57228] | - |
| size-change | - | swap ln2092 && 'messages != nullptr' <-> 'm_nState == 5' ['size', 57256] | - |
| size-change | - | swap ln2150 && 'm_nState == 0' <-> '!Globals::status->inAlienOrbit()' ['size', 57240] | - |
| size-change | - | swap ln2181 || '!Globals::layout->isFading()' <-> 'm_nScriptTimerA >= 10001' ['size', 57224] | - |
| size-change | - | swap ln2233 && 'm_nState == 0' <-> '((RadioMessage *) ((*messages)[0]))' ['size', 57216] | - |
| size-change | - | swap ln2285 && 'm_nState == 3' <-> '((RadioMessage *) ((*messages)[6]))' ['size', 57240] | - |
| size-change | - | swap ln2301 && '((RadioMessage *) ((*messages)[3]))' <-> 'm_nState == 1' ['size', 57248] | - |
| size-change | - | swap ln2339 && '((RadioMessage *) ((*messages)[9]))' <-> 'm_nState == 5' ['size', 57248] | - |
| size-change | - | swap ln2353 && '((RadioMessage *) ((*messages)[7]))' <-> 'm_nState == 4' ['size', 57300] | - |
| size-change | - | swap ln2407 && '((RadioMessage *) ((*messages)[0]))' <-> 'm_nState == 0' ['size', 57280] | - |
| size-change | - | swap ln2459 && '((RadioMessage *) ((*messages)[0]))' <-> 'm_nState == 0' ['size', 57240] | - |
| size-change | - | swap ln2555 && '(*enemies)[i]->field_0x40 != 0' <-> '(*enemies)[i]->player->getEmpPoints' ['size', 57240] | - |
| size-change | - | swap ln2600 && 'm_nState == 0' <-> '((RadioMessage *) ((*messages)[0]))' ['size', 57228] | - |
| size-change | - | swap ln2708 && 'm_nState == 10' <-> '((RadioMessage *) ((*messages)[9]))' ['size', 57244] | - |
| size-change | - | swap ln2817 && 'm_nState == 2' <-> '((RadioMessage *) ((*messages)[5]))' ['size', 57224] | - |
| size-change | - | swap ln2823 && 'm_nState == 3' <-> 'Globals::status->field_178 >= 10' ['size', 57236] | - |
| size-change | - | swap ln2827 && '((RadioMessage *) ((*messages)[6]))' <-> '!m_pLevel->getPlayer()->isDockedToD' ['size', 57224] | - |
| size-change | - | swap ln2827 && 'm_nState == 4' <-> '((RadioMessage *) ((*messages)[6]))' ['size', 57236] | - |
| size-change | - | swap ln2897 && 'Globals::status->field_178 > 7' <-> 'm_nScriptTime <= 179999' ['size', 57312] | - |
| size-change | - | swap ln2913 && 'm_nState == 0' <-> '((RadioMessage *) ((*messages)[2]))' ['size', 57276] | - |
| size-change | - | swap ln3166 || 'Globals::status->field_178 > 6' <-> 'm_nScriptTime >= 60001' ['size', 57240] | - |
| size-change | - | swap ln3272 && 'm_nState == 0' <-> '((RadioMessage *) ((*messages)[0]))' ['size', 57220] | - |
| size-change | - | swap ln3467 && '(*enemies)[i]->shipGroup == 10' <-> '(*enemies)[i]->player->getEnemy(0)-' ['size', 57236] | - |
| size-change | - | swap ln3781 && '// lint: arm_builtin\n              ' <-> 'reinterpret_cast<Route *>(player->g' ['size', 57220] | - |
| size-change | - | swap ln3789 && 'm_nState == 10' <-> '((RadioMessage *) ((*messages)[7]))' ['size', 57256] | - |
| size-change | - | swap ln3913 && 'e->shipGroup == 8' <-> 'e->player->isActive()' ['size', 57248] | - |
| size-change | - | swap ln4292 && '!((PlayerFighter *) (*enemies)[i])-' <-> 'Globals::rnd->nextInt(100) <= 9, 1' ['size', 57224] | - |
| size-change | - | swap ln4321 && 'i != 0' <-> 'm_nState >= 2' ['size', 57224] | - |
| size-change | - | swap ln4322 || 'm_nState != 4' <-> 'm_nScriptTimerA <= 1000' ['size', 57228] | - |
| size-change | - | swap ln4470 && 'm_nState == 0' <-> '((RadioMessage *) ((*messages)[0]))' ['size', 57344] | - |
| size-change | - | swap ln4722 && '((RadioMessage *) ((*messages)[8]))' <-> 'm_nTimeLimit <= 0' ['size', 57228] | - |
| size-change | - | swap ln4767 && 'm_nState == 1' <-> '((RadioMessage *) ((*messages)[0]))' ['size', 57220] | - |
| size-change | - | swap ln4887 || '(destroyed >= 9 && m_nScriptTime > ' <-> '(*enemies)[fighterIndex]->player->g' ['size', 57240] | - |
| size-change | - | swap ln4976 && 'm_nScriptTimerA >= 12001' <-> '((RadioMessage *) ((*messages)[12])' ['size', 57236] | - |
| size-change | - | swap ln5007 && '((RadioMessage *) ((*messages)[13])' <-> 'm_pExplosion == nullptr' ['size', 57236] | - |
| size-change | - | swap ln5206 && 'm_nScriptTimerB >= 12001' <-> '((RadioMessage *) ((*messages)[2]))' ['size', 57268] | - |
| size-change | - | swap ln5238 && 'AbyssEngine::AEMath::VectorLength((' <-> 'Globals::rnd->nextInt(100) >= 61, 1' ['size', 57212] | - |
| size-change | - | swap ln5273 && 'distance <= 49999' <-> 'player->player->field_5e == 0' ['size', 57264] | - |
| size-change | - | swap ln5276 && 'i != 0' <-> '(*enemies)[i - 1]->field_0x3e != 0' ['size', 57236] | - |
| size-change | - | swap ln620 && 'introMsg->isOver()' <-> 'm_nState == 0' ['size', 57460] | - |
| size-change | - | swap ln709 && 'm_nState == 5' <-> '((RadioMessage *) ((*messages)[12])' ['size', 57204] | - |
| size-change | - | swap ln962 && 'msg2->isOver()' <-> 'm_nState == 0' ['size', 57436] | - |
| size-change | - | swap ln966 && 'm_nState == 1' <-> '!Globals::layout->isFading()' ['size', 57244] | - |

## Combinations tested (not landed)

On the post-commit-3 base, adding any of the neutral swaps (ln1240, ln1413,
ln1519, ln1720, ln2383, ln2789) on top of ln4199 stays at 53,954.  The
ln1536+ln1526+ln4199 triple is 53,955 (same as the pair; ln4199 only pays on
the later base).  On the post-commit-5 base the only improving expect cell is
ln3075:1 (53,953).

## Reproduction

```sh
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
cmake --build cmake-build-match --target check -j2
```
