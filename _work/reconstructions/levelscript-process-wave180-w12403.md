# LevelScript::process — wave 180 / w12403: greedy adjacent-statement reorder

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`,
original `0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree
`/home/debian/GalaxyOnFire2/wt/w12403`, branch `wave180-w12403`, base master
`99e341e9b` (wave-179 merge).  Scratch `/home/debian/GalaxyOnFire2/scratch/w12403`.

Follow-up to the wave-179 greedy statement reorder (w12303, raw 53,039 ->
52,926) and the wave-179 alias/declaration sweeps (w12302/w12305).  This pass
re-runs the adjacent-statement search on the wave-179 base and extends it.

## Result (code win landed)

```
                         base (HEAD)      this pass
process raw diffbytes    52,926           52,819     (-107)
process aligned insns    20,010           20,109     (+99)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,731  20,830 / 20,731
.text diffbytes          76,870           76,763     (-107)
.ARM.extab diffbytes     709              709        (unchanged; size/base exact)
.note.gnu.build-id       19               19         (unchanged)
total differing bytes    77,598           77,491     (-107)
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
included), from `tools/fn_diff.py`; it is the primary metric.  The +99 aligned
instructions are a block-placement/register-allocation side effect; size and
`.rel.plt` are unchanged, and `.ARM.extab` content does not grow.

## Method

### Statement enumeration and independence filter

`scratch/w12403/stmts.py` parses the `process` body (lines 605-5357), ignores
comments/string literals, tracks brace and paren depth, and keeps only
single-line statements (trimmed line ends with `;`, no braces, balanced
parens, paren depth 0 at line start).  On the wave-179 base this gives 4,753
body lines, 2,826 single-line statements, 456 same-depth groups and 1,910
adjacent pairs.

`scratch/w12403/sweep_pairs.py` keeps an adjacent pair only when it is
independent: the write set of each statement (LHS identifiers of `=`, `+=`,
`-=`, `*=`, `/=`, `|=`, `&=`, `^=`, `<<=`, `>>=`, and `++`/`--` operands) does
not intersect the read set of the other, and two calls on the same receiver
base keep their order.  This leaves 1,427-1,434 pairs per pass.

### Evaluation harness

`scratch/w12403/eval.py` compiles one TU with the exact
`compile_commands.json` command and links it with the exact `build.ninja`
gold command (stopping before `strip`; `.text`/`.rel.plt` bytes are
identical in the unstripped output).  It reports the same-address raw diff,
function size and `.rel.plt` md5.  A candidate takes ~1.5 s; four workers
(`ulimit -v 12000000`) evaluate a full 1,430-pair sweep in ~9 min.

### Greedy search

`scratch/w12403/run_pairs.py` evaluates every independent pair in parallel,
then `apply_wins.py` applies the non-overlapping size-exact,
`.rel.plt`-identical winners and repeats.  Five full sweeps were run:

| sweep | base raw | size-exact relplt-clean wins | applied | new raw |
|---|---:|---:|---:|---:|
| 1 | 52,926 | 28 | 25 | 52,846 |
| 2 | 52,846 | 13 | 10 | 52,826 |
| 3 | 52,826 | 4 | 3 | 52,821 |
| 4 | 52,821 | 2 | 1 | 52,819 |
| 5 | 52,819 | 1 (rejected) | 0 | 52,819 |

The final diff is 28 moved statements (git diff: 28 insertions / 28
deletions).  Several are 3-5 line rotations; the moved statement is the one
that changes position.

## Landed moves

All moves are semantics-preserving: independent field stores and/or calls on
different objects.  Line numbers are the wave-179 base (`99e341e9b`).  No RNG call order changes, no condition/branch changes, no
read-after-write reordering.

| # | base line | moved statement | new position |
|---:|---:|---|---|
| 1 | 623 | `m_nState = 1;` | after the two `setPosition` calls |
| 2 | 1369 | `(*enemies)[0]->awake();` | after `setCloakingPossible(false)` |
| 3 | 1487 | `m_bStartSequenceOver = 1;` | after `setVulnerable(true)` |
| 4 | 1676 | `player->setComputerControlled(true);` | after `m_bCinematicBreak = 1;` |
| 5 | 1805 | `player->geometry->setRotation(0, pi/2, 0);` | before `setLookAtCam(true)` |
| 6 | 1936 | `(*enemies)[0]->setVisible(true);` | after `player->setTurretMode(false)` |
| 7 | 2007 | `m_pHud->visible = 0;` | before `hideShipForFirstPersonCameraView(false)` |
| 8 | 2008 | `camera41->setLookAtCam(true);` | before `player->stopShooting(0)` |
| 9 | 2299 | `(*enemies)[i]->player->setEnemies(nullptr);` | after `setRoute(route->clone())` |
| 10 | 2345 | `m_pLevel->lodManager->forceUpdate(delta, false);` | after `m_nState = 7;` |
| 11 | 2465 | `m_nState = 2;` | after `m_bCinematicBreak = 0;` |
| 12 | 2526 | `(*m_pLevel->getEnemies())[0]->setRoute(nullptr);` | after `Vector dir = ...getDirection()` |
| 13 | 2555 | `m_bCinematicBreak = 0;` | after `Player *lead = ...` |
| 14 | 2696 | `m_pLevel->field_74->enableSystemEmit(...);` | after `m_nState = 9;` |
| 15 | 3117 | `player->setTurretMode(false);` | after `resetCamera(m_pLevel);` |
| 16 | 3124 | `m_pCamera->enableFirstPersonCam(false);` | after `player->setFreeLookMode(false);` |
| 17 | 3127 | `player->stopShooting(0);` | after `m_bCinematicBreak = 1;` |
| 18 | 3182 | `m_bCinematicBreak = 0;` | after `setVisible(false)` |
| 19 | 3396 | `resetCamera(m_pLevel);` | before `player->setTurretMode(false);` |
| 20 | 3667 | `(*enemies)[i]->setActive(false);` | after `setSpeed(0.0f)` |
| 21 | 3715 | `player->stopShooting(0);` | after `m_bCinematicBreak = 1;` |
| 22 | 3779 | `m_pRadar->enabled = 1;` | after `m_pHud->visible = 1;` |
| 23 | 3837 | `m_pHud->visible = 0;` | after `hideShipForFirstPersonCameraView(false)` |
| 24 | 3840 | `m_pCamera->setLookAtCam(true);` | before `player->stopShooting(0)` |
| 25 | 3843 | `m_pCamera->setPosition(player->geometry->getPosition());` | after `player->setComputerControlled(true)` |
| 26 | 4027 | `m_pCamera->setTarget(leader);` | after `m_tempVec = leader->getPosition();` |
| 27 | 4587 | `m_tempVec = MatrixGetPosition(local);` | after `m_dirVec = player->GetDirVector();` |
| 28 | 5205 | `Globals::status->field_68 = ...getGammaHP();` | after `Globals::switch_to_target_setting = 1;` |

(The table lists the moved statement; the diff also shifts the statements it
moves past, hence the 3-5 line rotations.)

## Rejected

* **`frame->setPosition` / `CameraSetLocal` (line 2671).**  Swapping
  `frame->setPosition(m_tempVec);` and
  `canvas->CameraSetLocal(canvas->CameraGetCurrent(), frame->getMatrix());`
  is a size-exact, `.rel.plt`-clean raw win of **-1**, but it is a
  read-after-write reorder: `CameraSetLocal` consumes the matrix written by
  `setPosition`.  The original disassembly has `AEGeometry::setPosition` at
  `0x141608` and `PaintCanvas::CameraSetLocal` at `0x141626`, i.e. the base
  order.  Rejected for semantics; the landed tree is raw **52,819** (the swap
  would give 52,818).
* After sweep 4 the only remaining size-exact relplt-clean cell was the
  rejected pair above; the adjacent-statement search is exhausted at this
  point.
* A 3-line rotation of the post-sequence block (`m_bStartSequence` /
  `m_bStartSequenceOver` / `setLookAtCam`) was not tested as a separate cell:
  the landed move 3 is the raw winner, but it breaks the original's adjacent
  `strh.w` merge of the two byte stores.  The move is still a net raw win and
  leaves size/`.rel.plt`/`.ARM.extab` unchanged.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12403
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 52819 aligned= 20109 size= 57232/57232 insn= 20731/20830
python3 /home/debian/GalaxyOnFire2/scratch/w12403/measure.py
# -> raw=52819 size=57232/57232 relplt=289b2a069f8504d1e8a3919e43915e04
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
cmake --build cmake-build-match --target drift -- -j2   # 0 fields
```

The sweep harnesses are `stmts.py`, `sweep_pairs.py`, `eval.py`,
`run_pairs.py`, `apply_wins.py`, `aligned.py`, `secdiff.py` and the
`pairs*.json` / `pairs_results.json` tables in
`/home/debian/GalaxyOnFire2/scratch/w12403` (scratch, not tracked).
