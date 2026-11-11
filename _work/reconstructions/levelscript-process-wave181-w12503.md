# LevelScript::process — wave 181 / w12503: non-adjacent statement-move sweep

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`,
original `0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree
`/home/debian/GalaxyOnFire2/wt/w12503`, branch `wave181-w12503`, base master
`1e270aa0e` (wave-180 harvest 4, raw **52,818**).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w12503`.

Follow-up to wave 180 / w12403 (28 adjacent-statement moves, raw 52,926 ->
52,819) and w12404 (case-102 expect, -> 52,818).  The adjacent-pair search was
exhausted there; this pass searches **non-adjacent** moves (a statement moved
across 2 neighbours, `K=2`) inside the same basic block.

## Result (code win landed)

```
                         base (HEAD)      this pass
process raw diffbytes    52,818           52,683     (-135)
process aligned insns    20,109           21,609     (+1,500)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,731  20,830 / 20,734
.text diffbytes          76,200           76,065     (-135)
.ARM.extab diffbytes     713              713        (unchanged; size/base exact)
.note.gnu.build-id       20               20         (unchanged)
total differing bytes    76,933           76,798     (-135)
.rel.plt                 md5 903755c8…    md5 903755c8…   (byte-identical)
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
included), from `tools/fn_diff.py`; it is the primary metric.  The +1,500
aligned instructions are a block-placement/register-allocation side effect of
the reordered statements; size, `.rel.plt`, and `.ARM.extab` are unchanged.

## Method

### Candidate generation

`scratch/w12503/stmts.py` parses the `process` body (lines 605-5357), tracks
brace depth, and keeps single-line statements (trimmed line ends with `;`, no
braces, balanced parens/brackets, depth 0 at line start), excluding `case`,
`default`, `break`, `continue`, `return`, and control-flow lines.  This gives
**2,613 statements** in 9 depth groups.

For every statement `i` and every target `j` with `0 < |i-j| <= 2`, the move is
kept only when:

* every statement between `i` and `j` has the same brace depth (so the move
  never crosses a block boundary);
* the source lines strictly between the two statements contain no other code
  (only blank/comment lines), so no `if`/`for`/`while`/`switch` line is
  crossed;
* the moved statement is independent of every crossed statement: the write
  set (LHS of `=`, `+=`, ... and `++`/`--` operands) of each does not intersect
  the read set of the other, and two calls on the same receiver base keep their
  order.

This yields **3,870 candidates** (`K=2`; `K=1` gives 2,410 and is already
exhausted by w12403).

### Evaluation harness

`scratch/w12503/evalmod.py` compiles one TU with the exact
`compile_commands.json` command, then runs the exact `build.ninja` gold link
command (stopping before `strip`; `.text`/`.rel.plt` bytes are identical in
the unstripped output).  It reports the same-address raw diff, function size
and `.rel.plt` md5.  Four workers (`ulimit -v 12000000`) evaluate the full
3,870-candidate sweep in **~25 min** (~0.38 s/candidate).

### Greedy combination

`scratch/w12503/apply.py` sorts the 95 individual size-exact,
`.rel.plt`-identical raw winners by raw and adds them one at a time, keeping a
move only when the combined tree is still size-exact, `.rel.plt`-identical and
raw-lowering.  It accepted **71** moves and reached raw **52,682**.  A
leave-one-out over the 71 (`scratch/w12503/loo.py`) showed every move is
load-bearing (removing any one raises raw by 1-9).

The one semantically unsafe winner was then dropped: moving
`frame->setPosition(m_tempVec);` after
`canvas->CameraSetLocal(canvas->getCurrent(), frame->getMatrix());` is a
read-after-write reorder (the original has `AEGeometry::setPosition` at
`0x141608` before `PaintCanvas::CameraSetLocal` at `0x141626`; w12403 rejected
the same pair).  The landed tree is the remaining **70 moves**, raw **52,683**
(the dropped swap would give 52,682).

## Landed moves

All moves are semantics-preserving: independent field stores and/or calls on
different objects, or a `Vector`/pointer declaration moved across independent
stores.  Line numbers are from the wave-180 base (`1e270aa0e`).  "down" means
the statement moved later; "up" means earlier.

| # | base line | dir | new line | moved statement |
|---:|---:|---|---:|---|
| 1 | 2491 | down | 2493 | `radar->enabled = 0;` |
| 2 | 1601 | up | 1599 | `player->setFreeLookMode(false);` |
| 3 | 2632 | down | 2634 | `m_pRadar->enabled = 0;` |
| 4 | 2766 | up | 2764 | `m_pCamera->translate((float) delta * 0.5f, (float) delta * 0.2f, (float) -delta * 4.0f);` |
| 5 | 1609 | down | 1611 | `player->player->removeAllGuns();` |
| 6 | 2627 | up | 2625 | `m_pCamera->enableFirstPersonCam(false);` |
| 7 | 2987 | down | 2989 | `((PlayerFixedObject *) m_pLevel->getDockingTarget(0))->setName(String("", false));` |
| 8 | 3730 | down | 3732 | `m_pCamera->setPosition(*camPos);` |
| 9 | 1367 | down | 1369 | `m_nScriptTimerA = 0;` |
| 10 | 1608 | up | 1606 | `m_pLevel->flashScreen(3);` |
| 11 | 1954 | up | 1952 | `m_nState = 1;` |
| 12 | 2258 | up | 2256 | `m_pCamera->enableFirstPersonCam(false);` |
| 13 | 4315 | down | 4317 | `m_pGeometry4->moveForward((float) (delta * 5));` |
| 14 | 4521 | down | 4523 | `m_pCamera->enableFirstPersonCam(false);` |
| 15 | 5091 | up | 5089 | `m_pGeometry3->setPosition(((PlayerStation *) (*enemies)[stationIndex])->rootGeometry->getPosition());` |
| 16 | 948 | down | 952 | `m_pLevel->lodManager->forceUpdate(delta, true);` |
| 17 | 1345 | down | 1347 | `Globals::layout->enableFillScreen(true);` |
| 18 | 1627 | down | 1629 | `player->setFreeLookMode(false);` |
| 19 | 1671 | up | 1669 | `m_pCamera->enableFirstPersonCam(false);` |
| 20 | 1948 | down | 1950 | `m_bCinematicBreak = 1;` |
| 21 | 2052 | up | 2050 | `m_pCamera->setTarget(freighter->geometry);` |
| 22 | 2742 | up | 2740 | `m_pHud->visible = 0;` |
| 23 | 2811 | down | 2813 | `m_bCinematicBreak = 0;` |
| 24 | 2962 | up | 2960 | `resetCamera(m_pLevel);` |
| 25 | 3130 | down | 3132 | `m_pRadar->enabled = 0;` |
| 26 | 3150 | up | 3148 | `m_dirVec = (*enemies)[3]->geometry->getDirection();` |
| 27 | 3718 | up | 3716 | `m_pRadar->enabled = 0;` |
| 28 | 3832 | up | 3830 | `m_pCamera->setRumblePercentage(0.0f, 0);` |
| 29 | 4213 | down | 4215 | `m_pHud->visible = 1;` |
| 30 | 4400 | up | 4398 | `m_pCamera->setLookAtCam(true);` |
| 31 | 4629 | down | 4631 | `(*enemies)[0]->geometry->moveForward(step);` |
| 32 | 4741 | up | 4739 | `m_pCamera->setFixed(false);` |
| 33 | 4945 | down | 4947 | `m_pCamera->setLookAtCam(true);` |
| 34 | 5059 | up | 5057 | `m_nFlashCountdown = 300;` |
| 35 | 5196 | down | 5198 | `m_nState++;` |
| 36 | 692 | down | 694 | `resetCamera(self);` |
| 37 | 957 | up | 955 | `(*enemies)[0]->setToSleep();` |
| 38 | 998 | down | 1000 | `Globals::sound->play(160, nullptr, nullptr, 0.0f);` |
| 39 | 1210 | down | 1212 | `m_pCamera->setLookAtCam(false);` |
| 40 | 1387 | up | 1385 | `m_nScriptTimerA = 0;` |
| 41 | 1411 | up | 1409 | `st->field_68 = pp->getGammaHP();` |
| 42 | 1483 | down | 1485 | `Status *st = Globals::status;` |
| 43 | 1714 | up | 1712 | `m_pCamera->enableFirstPersonCam(false);` |
| 44 | 1832 | down | 1837 | `(*m_pLevel->getLandmarks())[3]->setVisible(true);` |
| 45 | 1869 | down | 1871 | `player->hideShipForFirstPersonCameraView(false);` |
| 46 | 1898 | up | 1896 | `Vector camPos = *m_pCamera->getPosition();` |
| 47 | 1960 | down | 1962 | `m_pCamera->setTarget(player->geometry);` |
| 48 | 2012 | down | 2014 | `m_bCinematicBreak = 1;` |
| 49 | 2091 | up | 2089 | `(*enemies)[0]->player->setHitpoints(100);` |
| 50 | 2116 | down | 2118 | `(*m_pLevel->getLandmarks())[3]->setPosition(25000.0f, 20000.0f, -55000.0f);` |
| 51 | 2136 | down | 2138 | `Level *self = m_pLevel;` |
| 52 | 2265 | up | 2263 | `m_pCamera->setLookAtCam(true);` |
| 53 | 2391 | down | 2393 | `player->stopShooting(0);` |
| 54 | 2643 | up | 2641 | `Globals::sound->play(1121, nullptr, nullptr, 0.0f);` |
| 55 | 2730 | up | 2728 | `m_pGeometry0->setVisible(false);` |
| 56 | 3045 | down | 3047 | `wcamera->setTarget(leader);` |
| 57 | 3092 | down | 3094 | `m_pHud->visible = 1;` |
| 58 | 3219 | up | 3217 | `m_bCinematicBreak = 1;` |
| 59 | 3345 | up | 3343 | `resetCamera(m_pLevel);` |
| 60 | 3410 | down | 3412 | `m_pRadar->enabled = 0;` |
| 61 | 3973 | up | 3971 | `m_nScriptTimerA = 0;` |
| 62 | 4062 | up | 4060 | `player->setComputerControlled(false);` |
| 63 | 4066 | up | 4064 | `resetCamera(m_pLevel);` |
| 64 | 4387 | up | 4385 | `m_pCamera->setRumblePercentage(0.0f, 0);` |
| 65 | 4493 | up | 4491 | `m_pHud->visible = 1;` |
| 66 | 4619 | up | 4617 | `m_tempVec = player->getPosition();` |
| 67 | 4662 | down | 4664 | `player->setSpeed(0.02f);` |
| 68 | 4892 | up | 4890 | `((PlayerFighter *) (*enemies)[i])->setAIDisabled(false);` |
| 69 | 4927 | down | 4929 | `m_nScriptTimerB = 0;` |
| 70 | 5028 | down | 5030 | `player->setSpeed(10.0f);` |

The moves are not the adjacent-pair winners of w12403; they are all
distance-2 moves (each crosses exactly one intervening statement), which the
wave-180 sweep could not reach.

## Rejected

* `frame->setPosition(m_tempVec)` <-> `CameraSetLocal(...)` (base line 2671):
  size-exact, `.rel.plt`-clean, raw **-1**, but a read-after-write reorder
  (`CameraSetLocal` consumes the matrix written by `setPosition`; the original
  emits `setPosition` first).  Dropped for semantics; raw 52,683.
* The `K=1` adjacent-pair search is exhausted at the wave-180 base; this sweep
  re-confirmed that none of the 2,410 adjacent candidates is a size-exact,
  `.rel.plt`-identical raw winner on the landed tree.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12503
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 52683 aligned= 21609 size= 57232/57232 insn= 20734/20830
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
cmake --build cmake-build-match --target lint  -- -j2   # CLEAN
cmake --build cmake-build-match --target drift -- -j2   # 0 fields
```

The sweep harnesses (`stmts.py`, `evalmod.py`, `gen_moves.py`, `sweep.py`,
`apply.py`, `loo.py`, `secdiff.py`) and the result table (`res_k2.jsonl`,
`cands_k2.json`, `movetable.txt`) live in
`/home/debian/GalaxyOnFire2/scratch/w12503/`; they are scratch, not tracked.

## Second sweep on the improved tree (landed)

The same `K=2` generator/sweep was re-run on the 52,683 tree (3,911
candidates, ~25 min).  It found **45** size-exact, `.rel.plt`-identical raw
winners.  Greedy combination accepted 23 and reached raw **52,642**; the
read-after-write `frame->setPosition` / `CameraSetLocal` swap was again
dropped, leaving **22 moves** and raw **52,643**.

```
                         after sweep 1    after sweep 2
process raw diffbytes    52,683           52,643     (-40)
process aligned insns    21,609           21,563
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
.text diffbytes          76,065           76,025     (-40)
.ARM.extab diffbytes     713              713        (unchanged)
.note.gnu.build-id       20               20         (unchanged)
total differing bytes    76,798           76,758     (-40)
.rel.plt                 md5 903755c8…    md5 903755c8…   (byte-identical)
RATCHET                  PASS             PASS
```

Line numbers are from the sweep-1 tree (`55a288e92`); all are distance-2
moves and all were individually size-exact + `.rel.plt`-identical.

| # | line | dir | new line | moved statement |
|---:|---:|---|---:|---|
| 1 | 1602 | up | 1600 | `player->hideShipForFirstPersonCameraView(false);` |
| 2 | 2630 | down | 2632 | `m_bCinematicBreak = 1;` |
| 3 | 1606 | up | 1604 | `m_pLevel->flashScreen(3);` |
| 4 | 1611 | down | 1612 | `player->player->removeAllGuns();` |
| 5 | 1213 | down | 1214 | `player->setVisible(true);` |
| 6 | 3344 | down | 3346 | `m_pHud->visible = 1;` |
| 7 | 952 | down | 956 | `m_pLevel->lodManager->forceUpdate(delta, true);` |
| 8 | 1607 | down | 1609 | `m_pHud->visible = 0;` |
| 9 | 1713 | down | 1714 | `resetCamera(m_pLevel);` |
| 10 | 1897 | down | 1898 | `m_pCamera->setTarget(m_pGeometry6);` |
| 11 | 1947 | down | 1948 | `player->stopShooting(0);` |
| 12 | 2051 | down | 2052 | `freighter->player->setHitpoints(9999999);` |
| 13 | 2090 | down | 2091 | `Globals::sound->stop(156);` |
| 14 | 2261 | down | 2263 | `m_bCinematicBreak = 1;` |
| 15 | 2483 | down | 2484 | `player->setTurretMode(false);` |
| 16 | 2985 | down | 2986 | `m_pCamera->setPosition(m_tempVec);` |
| 17 | 3096 | up | 3094 | `m_nScriptTimerA = 0;` |
| 18 | 3129 | down | 3130 | `m_pHud->visible = 0;` |
| 19 | 3221 | up | 3219 | `m_pRadar->enabled = 0;` |
| 20 | 3343 | up | 3341 | `resetCamera(m_pLevel);` |
| 21 | 4399 | down | 4400 | `m_pHud->visible = 0;` |
| 22 | 4633 | up | 4631 | `m_dirVec = player->GetDirVector();` |

A leave-one-out check on the sweep-1 combination had shown every one of those
70 moves load-bearing; the sweep-2 combination was accepted greedily with the
same size/`.rel.plt`/raw checks at every step.

## Third sweep on the improved tree (landed)

Re-ran the `K=2` sweep on the 52,643 tree (3,919 candidates).  It found **11**
size-exact, `.rel.plt`-identical raw winners.  Greedy accepted 4 (raw 52,634);
the `frame->setPosition` / `CameraSetLocal` read-after-write swap was again
dropped, leaving **3 moves** and raw **52,635**.

```
                         after sweep 2    after sweep 3
process raw diffbytes    52,643           52,635     (-8)
process aligned insns    21,563           21,571
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
.text diffbytes          76,025           76,017     (-8)
.ARM.extab diffbytes     713              713        (unchanged)
.note.gnu.build-id       20               20         (unchanged)
total differing bytes    76,758           76,750     (-8)
.rel.plt                 md5 903755c8…    md5 903755c8…   (byte-identical)
RATCHET                  PASS             PASS
```

Line numbers are from the sweep-2 tree (`3d37f6670`).

| # | line | dir | new line | moved statement |
|---:|---:|---|---:|---|
| 1 | 1604 | up | 1602 | `m_pLevel->flashScreen(3);` |
| 2 | 3220 | down | 3222 | `player->stopShooting(0);` |
| 3 | 3347 | down | 3348 | `m_bCinematicBreak = 0;` |

### Cumulative result

`process` raw **52,818 -> 52,635** (-183) over the three sweeps; `.text`
**76,200 -> 76,017**; `.ARM.extab` unchanged at 713; `.rel.plt` byte-identical;
size 57,232 exact; all gate counts unchanged.

## Distance-3 sweep (landed)

The `K=2` space was exhausted after the third sweep, so the next sweep kept only
distance-3 moves (`|i-j| == 3`, i.e. crossing two intervening statements):
1,048 candidates, **30** size-exact, `.rel.plt`-identical raw winners.  Greedy
accepted all 28 that compose (raw **52,595**; no semantically unsafe pair in
this set, so no rejection was needed).

```
                         after sweep 3    after distance-3
process raw diffbytes    52,635           52,595     (-40)
process aligned insns    21,571           21,770
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
.text diffbytes          76,017           75,977     (-40)
.ARM.extab diffbytes     713              713        (unchanged)
.note.gnu.build-id       20               20         (unchanged)
total differing bytes    76,750           76,710     (-40)
.rel.plt                 md5 903755c8…    md5 903755c8…   (byte-identical)
RATCHET                  PASS             PASS
```

Line numbers are from the sweep-3 tree (`8fba3fc2c`).

| # | line | dir | new line | moved statement |
|---:|---:|---|---:|---|
| 1 | 3024 | up | 3021 | `m_pCamera->setFixed(true);` |
| 2 | 2005 | up | 2002 | `camera41->enableFirstPersonCam(false);` |
| 3 | 3563 | up | 3560 | `m_pCamera->setLookAtCam(false);` |
| 4 | 700 | down | 703 | `player->setComputerControlled(true);` |
| 5 | 1267 | down | 1270 | `m_nState = 2;` |
| 6 | 2890 | down | 2893 | `resetCamera(self);` |
| 7 | 5264 | down | 5267 | `m_pLevel->lodManager->forceUpdate(delta, false);` |
| 8 | 939 | up | 936 | `Level::initStreamOutPosition = 1;` |
| 9 | 1241 | down | 1244 | `m_pCamera->setPosition(m_tempVec);` |
| 10 | 1369 | down | 1372 | `m_nScriptTimerA = 0;` |
| 11 | 1702 | up | 1699 | `m_bCinematicBreak = 0;` |
| 12 | 1837 | down | 1840 | `(*m_pLevel->getLandmarks())[3]->setVisible(true);` |
| 13 | 1965 | up | 1962 | `player->setComputerControlled(false);` |
| 14 | 2343 | down | 2346 | `player->player->setVulnerable(true);` |
| 15 | 2349 | down | 2352 | `m_pCamera->setTarget(player->geometry);` |
| 16 | 2386 | up | 2383 | `player->setTurretMode(false);` |
| 17 | 2640 | down | 2643 | `m_pCamera->setPosition(m_tempVec);` |
| 18 | 2694 | up | 2691 | `m_pLevel->field_74->systemSetMatrix(m_pLevel->field_5c, &m_matrix);` |
| 19 | 2905 | down | 2908 | `self->getPlayer()->setCollide(false);` |
| 20 | 3181 | up | 3178 | `player->setComputerControlled(false);` |
| 21 | 3221 | up | 3218 | `m_pCamera->setLookAtCam(true);` |
| 22 | 3265 | up | 3262 | `m_nScriptTimerA = 0;` |
| 23 | 3424 | up | 3421 | `m_nState = 3;` |
| 24 | 4394 | down | 4397 | `player->player->setVulnerable(false);` |
| 25 | 4489 | down | 4492 | `m_pCamera->setTarget(player->geometry);` |
| 26 | 4528 | down | 4531 | `m_bCinematicBreak = 1;` |
| 27 | 4669 | up | 4666 | `m_nState++;` |
| 28 | 4876 | down | 4879 | `m_pLevel->getPlayer()->resetGunDelay();` |

### Final cumulative result

`process` raw **52,818 -> 52,595** (-223) over the four sweeps; `.text`
**76,200 -> 75,977**; `.ARM.extab` unchanged at 713; `.rel.plt` byte-identical;
size 57,232 exact; all gate counts unchanged (`byte_exact` 4507,
`linked_exact` 4507, size-exact 4524/4524, data 843/843, imports 207, lint
CLEAN, drift 0, parity 5370/5370).

## Distance-4 sweep (landed)

Next: only distance-4 moves (`|i-j| == 4`, crossing three intervening
statements): 808 candidates, **14** size-exact, `.rel.plt`-identical raw
winners.  Greedy accepted all 10 that compose (raw **52,570**).

```
                         after d3         after d4
process raw diffbytes    52,595           52,570     (-25)
process aligned insns    21,770           21,834
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
.text diffbytes          75,977           75,952     (-25)
.ARM.extab diffbytes     713              713        (unchanged)
.note.gnu.build-id       20               20         (unchanged)
total differing bytes    76,710           76,685     (-25)
.rel.plt                 md5 903755c8…    md5 903755c8…   (byte-identical)
RATCHET                  PASS             PASS
```

Line numbers are from the distance-3 tree (`59e50ccc8`).

| # | line | dir | new line | moved statement |
|---:|---:|---|---:|---|
| 1 | 2117 | up | 2113 | `m_nScriptTimerA = 0;` |
| 2 | 2388 | down | 2392 | `resetCamera(m_pLevel);` |
| 3 | 2987 | up | 2983 | `m_nScriptTimerA = 0;` |
| 4 | 3561 | down | 3565 | `m_pLevel->getPlayer()->resetGunDelay();` |
| 5 | 4879 | down | 4883 | `m_pLevel->getPlayer()->resetGunDelay();` |
| 6 | 1908 | up | 1902 | `m_pRadar->enabled = 1;` |
| 7 | 2004 | down | 2008 | `resetCamera(m_pLevel);` |
| 8 | 2550 | down | 2554 | `player->setComputerControlled(false);` |
| 9 | 2919 | up | 2915 | `m_nState = 6;` |
| 10 | 4493 | down | 4497 | `player->setVisible(true);` |

### Final cumulative result (wave 181 / w12503)

`process` raw **52,818 -> 52,570** (-248) over the five sweeps; `.text`
**76,200 -> 75,952**; `.ARM.extab` unchanged at 713; `.rel.plt` byte-identical;
size 57,232 exact; all gate counts unchanged (`byte_exact` 4507,
`linked_exact` 4507, size-exact 4524/4524, data 843/843, imports 207, lint
CLEAN, drift 0, parity 5370/5370).
