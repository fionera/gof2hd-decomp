# LevelScript::process — wave 185 / w12905: K=6..8 window, d21..40 and 2-opt sweeps

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w12905`, branch
`wave185-w12905`, base master `f390b0e68` (wave-184 harvest 10, raw **52,132**,
aligned 20,658).  Scratch `/home/debian/GalaxyOnFire2/scratch/w12905`.

Continuation of the wave-184 w12805 K=6..8 / distance-21..40 / 2-opt sweeps on
the current wave-184 body.  The w12705/w12805 harness was not preserved in
scratch, so it was rebuilt from the note descriptions (`stmts.py`, `gen.py`,
`eval.py`, `sweep.py`, `apply.py`).

## Result (code win landed)

```
                         base (HEAD)      this pass
process raw diffbytes    52,132           52,129     (-3)
process aligned insns    20,658           20,653     (-5)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,725  20,830 / 20,725
.text diffbytes          74,591           74,588     (-3)
.ARM.extab diffbytes     647              647        (unchanged; size/base exact)
.note.gnu.build-id       20               20         (unchanged)
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical)
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
included), from `tools/fn_diff.py`.  `.rel.plt` md5 is
`289b2a069f8504d1e8a3919e43915e04` before and after.

## Method

* `stmts.py` parses the `process` body (source lines 605-5365), tracks brace
  depth and keeps single-line statements (`;`-terminated, brace-free,
  balanced, not `case`/`default`/`break`/`continue`/`return`/control header).
  **2,819 statements** in the current body.
* `gen.py` keeps a pair `(i,j)` only when every statement between them has the
  same brace depth, every non-blank/non-comment source line between them is
  itself a recognized statement line, and the moved statement is prefix-aware
  independent of every crossed statement including the far endpoint.  Both
  directions are generated.  Access chains are parsed with
  `(*x)[i]->y` normalised to `x[]->y`; prefix aliasing (`m_tempVec` vs
  `m_tempVec.x`) blocks a move, and calls are treated as read+write of their
  receiver chain.
* `eval.py` compiles one candidate with the exact `compile_commands.json`
  command, runs the exact gold link command (stopping before strip), and
  reports the same-address raw diff, function size and `.rel.plt` md5 with a
  minimal ELF32 reader.  Four workers, `ulimit -v 12000000`.
* `apply.py` applies a sequence of base-statement moves, re-checking
  independence in the current order, then measures the combined tree.

Candidate counts: **2,473** for K=6..8 (d6/d7/d8) and **29** for d21..40.
1,822 of the K=6..8 candidates are size-exact and `.rel.plt`-identical.

### Single-move winners

K=6..8 found **7** size-exact, `.rel.plt`-identical raw winners, all -1
(three are alternate destinations for the same statement and two more are
alternates in the same block).  The distance-21..40 pass found **no** winner
(best 52,148).

### 2-opt / greedy combination

The three distinct moves were combined greedily; the best combination is
**3 moves** and reaches raw **52,129** (-3).  Adding the fourth candidate
(`m_pCamera->setTarget` down in the same block) is worse (52,161) because the
two down-moves interact.

## Landed moves

Line numbers are from the base (`f390b0e68`, 0-based statement ids in the
harness).  "up"/"down" is relative to the base position.

| # | base line (1-based) | dir | moved statement |
|---:|---:|---|---|
| 1 | 2496 | up | `m_pLevel->lodManager->forceUpdate(delta, false);` (before `m_pCamera->setTarget(...)`) |
| 2 | 2903 | up | `(*enemies)[0]->player->setHitpoints(0);` (before `player->stopShooting(0)`) |
| 3 | 4752 | down | `m_pCamera->setTarget(player->geometry);` (after `resetCamera(m_pLevel)`) |

All three are calls whose receiver/state is untouched by the crossed
statements: `forceUpdate` does not touch the `camPos`/`dir` locals it crosses;
the enemy hitpoint store is independent of the player's camera/freeze state;
and the camera target store is independent of the player movement/hud/radar
stores it crosses (`resetCamera` sets the same target, so the reorder is
behaviour-preserving).

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12905
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 52129 aligned= 20653 size= 57232/57232 insn= 20725/20830
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
cmake --build cmake-build-match --target lint  -- -j2   # CLEAN
cmake --build cmake-build-match --target drift -- -j2   # 0 fields
```

The harnesses (`stmts.py`, `gen.py`, `eval.py`, `sweep.py`, `apply.py`) and
result tables (`cands_8_6_8.json`, `res_k68.jsonl`, `res_d2140.jsonl`) live in
`/home/debian/GalaxyOnFire2/scratch/w12905/`; they are scratch, not tracked.

## Pass 2 (distance-13..20 on the improved tree, landed)

The distance-13..20 sweep had not been run on the wave-184 body, so it was
re-run here on the 52,129 tree (894 candidates, 417 size-exact/rel-ident).
It found one size-exact, `.rel.plt`-identical winner: moving
`m_pCamera->setLookAtCam(true);` (base line 3717) **down** past the whole
`camPos`/`tmp` vector computation to just before `m_nScriptTimerB = 0;`
(base line 3735) gives raw **52,124** (-5).

```
                         after pass 1     after pass 2
process raw diffbytes    52,129           52,124     (-5)
process aligned insns    20,653           20,674     (+21)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
.text diffbytes          74,588           74,583     (-5)
.ARM.extab diffbytes     647              647        (unchanged)
total differing bytes    75,255           75,250     (-5)
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical)
RATCHET                  PASS             PASS
```

The move is independent of every crossed statement (`hideShip`, radar enable,
free-look mode, the `ref2`/`camPos`/`tmp` locals and their `m_tempVec` /
`m_dirVec` arithmetic, the hud store, and the two timer stores); the only
other camera calls crossed are `setTarget`/`enableFirstPersonCam`, and the
measured ordering places `setLookAtCam` after them.

## Pass 3 (K=4..5 on the improved tree + greedy 9-move combination, landed)

The K=4..5 sweep had last run on the wave-182 body, so it was re-run here on
the 52,124 tree (2,430 candidates, 1,871 size-exact/rel-ident).  It found
**16** raw winners; the best single move is `player->setFreeLookMode(false)`
down past the `ref2`/`camPos` setup (base line 3719 -> after line 3724),
worth -9.

A greedy combination of the distinct winners reached raw **52,096** (-28) with
**9 moves** (each measured alone first, then added one at a time, keeping only
size-exact, `.rel.plt`-identical, raw-lowering combinations and re-checking
independence in the current order):

| # | base line (1-based) | dir | moved statement | across |
|---:|---:|---|---|---|
| 1 | 3719 | down | `player->setFreeLookMode(false);` | `m_pHud->visible = 0;` (3724) |
| 2 | 4752 | down | `player->setSpeed(2.0f);` | `lodManager->forceUpdate(delta, false);` (4757) |
| 3 | 3722 | up | `m_pCamera->enableFirstPersonCam(false);` | `player->hideShipForFirstPersonCameraView(false);` (3717) |
| 4 | 2903 | down | `(*enemies)[0]->player->setHitpoints(0);` | `self->getPlayer()->setCollide(false);` (2907) |
| 5 | 3567 | down | `m_pLevel->getPlayer()->resetGunDelay();` | `m_pRadar->enabled = 1;` (3571) |
| 6 | 1907 | down | `m_nState = 2;` | `player->setComputerControlled(false);` (1911) |
| 7 | 2011 | up | `player->stopShooting(0);` | `player->freeze = 1;` (2006) |
| 8 | 3737 | up | `m_pCamera->setPosition(*camPos);` | `m_nScriptTimerA = 0;` (3733) |
| 9 | 3937 | up | `Globals::status->setStation(...getStation(0x6f));` | `Globals::status->field_64 = ...` (3932) |

```
                         after pass 2     after pass 3
process raw diffbytes    52,124           52,096     (-28)
process aligned insns    20,674           20,708     (+34)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
.text diffbytes          74,583           74,555     (-28)
.ARM.extab diffbytes     647              647        (unchanged)
total differing bytes    75,250           75,222     (-28)
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical)
RATCHET                  PASS             PASS
```

All nine moves are pure field stores, pure declarations, or calls on a
receiver untouched by the crossed statements (player state vs camera/hud/radar,
enemy hitpoints vs player state, `Globals::status` field store vs `setStation`).
