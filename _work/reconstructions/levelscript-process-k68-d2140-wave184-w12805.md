# LevelScript::process — wave 184 / w12805: K=6..8 window, d21..40 and 2-opt sweeps

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree
`/home/debian/GalaxyOnFire2/wt/w12805`, branch `wave184-w12805`, base master
`4c06c2662` (wave-183 harvest, raw **52,281**, aligned 20,700).
Scratch `/home/debian/GalaxyOnFire2/scratch/w12805`.

Continuation of the wave-183 w12705 K=4..5 / distance-13..20 statement-move
sweeps.  This pass runs **K=6..8** non-adjacent statement moves, a
**distance-21..40** pass, and **2-opt / greedy pair/triple/quad combinations**
of the best single moves on the wave-183 body.

## Result (code win landed)

```
                         base (HEAD)      this pass
process raw diffbytes    52,281           52,257     (-24)
process aligned insns    20,700           20,685     (-15)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,725  20,830 / 20,722
.text diffbytes          74,842           74,818     (-24)
.ARM.extab diffbytes     646              646        (unchanged; size/base exact)
.note.gnu.build-id       20               20         (unchanged)
total differing bytes    75,508           75,484     (-24)
.rel.plt                 byte-identical   byte-identical (diff 0, md5 289b2a06…)
.rodata                  byte-identical   byte-identical
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

## Method

The w12705 harness was not preserved in scratch, so it was rebuilt from the
note's description (`scratch/w12805/stmts.py`, `gen.py`, `eval.py`, `sweep.py`,
`apply.py`, `secdiff.py`):

* `stmts.py` parses the `process` body (lines 605-5367), tracks brace depth and
  keeps single-line statements (`;`-terminated, brace-free, balanced,
  not `case`/`default`/`break`/`continue`/`return`/control header).  **2,729
  statements** in the current body.
* Candidate generation keeps a pair `(i,j)` only when every statement between
  them has the same brace depth, every non-blank/non-comment source line
  between them is itself a recognized statement line, and the moved statement
  is prefix-aware independent of every crossed statement including the far
  endpoint.  Both directions are generated.
* `eval.py` compiles one candidate with the exact `compile_commands.json`
  command, then runs the exact `build.ninja` gold link command (stopping
  before strip), and reports the same-address raw diff, function size and
  `.rel.plt` md5 with a minimal ELF32 reader.  Four workers, `ulimit -v
  12000000`, `-j2` builds only.

Candidate counts: **2,465** for K=6..8 (947/823/695 for distance 6/7/8) and
**22** for distance 21..40.  The long-distance pass is small because straight
runs of >20 statements that cross no control-flow line are rare in this body.

### Single-move winners

K=6..8 found **20** size-exact, `.rel.plt`-identical raw winners (best -3);
1,821 of 2,465 candidates were size-exact and rel-ident.  The distance-21..40
pass found **no** winner (22 candidates, 11 size-exact).

### 2-opt / greedy combination

The best single move per distinct primary statement was combined greedily
(pair, triple, quad, then adding the remaining winners one at a time), keeping
only size-exact, `.rel.plt`-identical, raw-lowering combinations.  The best
combination is **9 moves** and reaches raw **52,267** (-14).

## Landed moves

Line numbers are from the wave-183 base (`4c06c2662`); "down"/"up" is relative
to the base position.  All moves are pure field stores, pure declarations, or
calls whose receiver is untouched by the crossed statements.  The
mission-24 camera block only swaps `setTarget` and `translate` (both
independent camera state; `setPosition` stays before `translate`, matching the
original order at `0x13e9e0`/`0x13e9ec`).

| # | base line | dir | moved statement |
|---:|---:|---|---|
| 1 | 691 | down | `player->setTurretMode(false);` (after `self->getPlayer()->setCollide(false)`) |
| 2 | 747 | down | `m_nScriptTimerA = 0;` (after `m_bStartSequence = 0;`) |
| 3 | 1816 | up | `m_pCamera->setTarget(player->geometry);` (before `m_pCamera->translate(10500.0f, ...)`) |
| 4 | 1877 | up | `m_bCinematicBreak = 1;` (before `resetCamera(m_pLevel)`) |
| 5 | 1941 | down | `(*enemies)[0]->awake();` (after `player->player->setVulnerable(false)`) |
| 6 | 3189 | up | `m_bCinematicBreak = 0;` (before `cam->setLookAtCam(false)`) |
| 7 | 3570 | down | `m_pCamera->setTarget(player->geometry);` (after `m_bCinematicBreak = 0;`) |
| 8 | 3719 | down | `m_pCamera->setLookAtCam(true);` (after `m_pHud->visible = 0;`) |
| 9 | 5209 | down | `m_nState++;` (after `Globals::switch_to_target_setting = 1;`) |

The alternative `m_pCamera->setPosition(camPos)` down-move also reaches 52,267
when combined, but it reorders `setPosition` past `translate` (observable
camera state), so the semantically clean `setTarget`/`translate` swap was
landed instead at the same raw count.

## Second pass on the improved tree (landed)

The same K=6..8 generator/sweep was re-run on the 52,267 tree (2,467
candidates, 1,836 size-exact, 2,441 rel-ident).  It found 16 winners below
52,267 (best -3).  Greedy combination of the best distinct moves (including
`setLookAtCam`/`setVulnerable`/`hideShip` field-store reorders and the
`switch_to_target_setting` store) reached raw **52,257** (-10 more).

```
                         after pass 1     after pass 2
process raw diffbytes    52,267           52,257     (-10)
process aligned insns    20,666           20,685     (+19)
.text diffbytes          74,828           74,818     (-10)
.ARM.extab diffbytes     646              646        (unchanged)
total differing bytes    75,494           75,484     (-10)
.rel.plt                 byte-identical   byte-identical
RATCHET                  PASS             PASS
```

Line numbers are from the pass-1 tree (`9980db7c8`).

| # | base line | dir | moved statement |
|---:|---:|---|---|
| 1 | 1869 | up | `player->hideShipForFirstPersonCameraView(false);` (before `player->setFreeLookMode(false)`) |
| 2 | 3182 | down | `cam->setLookAtCam(false);` (after `player->player->setVulnerable(true)`) |
| 3 | 3568 | down | `player->player->setVulnerable(true);` (after `resetCamera(m_pLevel)`) |
| 4 | 3726 | up | `m_pCamera->setLookAtCam(true);` (before `player->setFreeLookMode(false)`) |
| 5 | 5214 | up | `Globals::switch_to_target_setting = 1;` (before `m_nState++`) |

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12805
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 52257 aligned= 20685 size= 57232/57232 insn= 20722/20830
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
```

Harnesses and result tables (`stmts.py`, `gen.py`, `eval.py`, `sweep.py`,
`apply.py`, `secdiff.py`, `res_k68.jsonl`, `res_d2140.jsonl`, `pairs.json`,
`triples.json`) live in `/home/debian/GalaxyOnFire2/scratch/w12805/` (scratch,
not tracked).
