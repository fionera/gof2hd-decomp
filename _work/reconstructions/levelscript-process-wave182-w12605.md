# LevelScript::process — wave 182 / w12605: K=1..3 window + distance-9..12 sweep

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree
`/home/debian/GalaxyOnFire2/wt/w12605`, branch `wave182-w12605`, base master
`57e085059` (wave-181 harvest 5, raw **52,523**).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w12605`.

Continuation of the wave-181 w12503 statement-permutation sweeps.  That pass
exhausted the K=2 space and ran distance-3..8; this pass re-runs the full
**K=1..3 window** against the landed body and adds a **distance-9..12** pass.
The w12503 harness scripts were not preserved in scratch, so the generator and
evaluator were reconstructed from the method section of
`levelscript-process-wave181-w12503.md` (same candidate rules, same
compile+gold-link measurement).

## Result (code win landed)

```
                         base (HEAD)      this pass
process raw diffbytes    52,523           52,413     (-110)
process aligned insns    22,148           22,055     (-93)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,730  20,830 / 20,728
.text diffbytes          75,365           75,255     (-110)
.ARM.extab diffbytes     655              656        (+1; size/base exact)
.note.gnu.build-id       20               20         (unchanged)
total differing bytes    76,040           75,931     (-109)
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
`289b2a069f8504d1e8a3919e43915e04` before and after (byte-identical to the
original).  The `.ARM.extab` content diff grows by one byte because the
reordered call sites shift the LevelScript LSDA call-site ranges; the section
stays size- and base-exact (156,312 B @ `0x1a3b78`) and the net total still
drops by 76 bytes.

## Method

### Candidate generation (`scratch/w12605/stmts.py`, `gen.py`)

`stmts.py` parses the `process` body (lines 605-5350), tracks brace depth and
keeps single-line statements (trimmed line ends with `;`, no braces, balanced
parens/brackets, not `case`/`default`/`break`/`continue`/`return` and not a
control-flow header).  This yields **2,568 statements** in 9 depth groups.

For every statement pair `(i, j)` with `0 < |i-j| <= K` the move is kept only
when:

* every statement between `i` and `j` has the same brace depth (no block
  boundary is crossed);
* every non-blank/non-comment source line between the two statement lines is
  itself a recognized statement line (no `if`/`for`/`while`/`switch`/brace
  line is crossed);
* the moved statement is independent of every crossed statement: the write
  set (LHS of `=`, `+=`, ... and `++`/`--` operands) of each does not
  intersect the read set of the other (prefix-aware on access chains), and
  two calls on the same receiver base keep their order.

Candidate counts: **5,161** for `K=1..3` and **826** for distance 9..12.

### Evaluation (`scratch/w12605/eval.py`, `sweep.py`)

`eval.py` compiles one candidate `LevelScript.cpp` with the exact
`compile_commands.json` command and runs the exact `build.ninja` gold link
command (stopping before `strip`; `.text`/`.rel.plt` bytes are identical in
the unstripped output).  It reports the same-address raw diff, function size
and `.rel.plt` md5.  `sweep.py` runs four workers (`ulimit -v 12000000`)
through the candidate list; ~0.36 s/candidate.

### Greedy combination (`scratch/w12605/apply.py`)

Moves are tracked by original source line id, so a move can be applied to an
already-permuted tree.  Winners are sorted by single-move raw and added one at
a time; a move is kept only when the combined tree is still size-exact
(57,232), `.rel.plt`-identical and raw-lowering.  The K=1..3 sweep produced
121 winners (best single move -8); the distance-9..12 sweep produced 2 (both
-3).  Greedy accepted **36** moves and reached raw **52,446**.

One variant was rejected: the two moves that swap the `dy`/`dz`
`Globals::rnd->nextInt(10000)` calls (`2409 -> 2410` and `2411 -> 2410`)
combine to raw 52,445 (one byte better) but reorder two side-effecting random
draws, so the landed tree keeps the original `dy` then `dz` order and instead
only hoists `KIPlayer *pirate = (*enemies)[i];` above `dy` (`2410 -> 2409`,
semantically a pure load).  The original disassembly at `0x13daa2-0x13db36`
issues `nextInt(10000)` for `dy` before `dz`.

## Landed moves

Line numbers are from the wave-181 base (`57e085059`).  Moves are listed in
the order the greedy applied them; "down"/"up" is relative to the base
position.  All are independent field stores, pure declarations, or calls on
different objects.

| # | base line | dir | moved statement |
|---:|---:|---|---|
| 1 | 1836 | down | `((PlayerWormHole *) (*m_pLevel->getLandmarks())[3])->open();` |
| 2 | 4498 | down | `player->setVisible(true);` |
| 3 | 2349 | up | `m_pRadar->enabled = 1;` |
| 4 | 4878 | down | `player->setComputerControlled(false);` |
| 5 | 4885 | up | `m_pLevel->lodManager->forceUpdate(delta, false);` |
| 6 | 937 | down | `Globals::layout->enableFillScreen(true);` |
| 7 | 2053 | down | `Globals::sound->play(155, nullptr, nullptr, 0.0f);` |
| 8 | 2055 | down | `(*enemies)[0]->player->StopEngineSound();` |
| 9 | 2402 | up | `m_pLevel->lodManager->forceUpdate(delta, false);` |
| 10 | 2986 | down | `((PlayerFixedObject *) m_pLevel->getDockingTarget(0))->setDockingType(0);` |
| 11 | 4593 | up | `m_pCamera->setFixed(true);` |
| 12 | 700 | down | `m_pCamera->setLookAtCam(true);` |
| 13 | 703 | down | `player->setComputerControlled(true);` |
| 14 | 1418 | up | `Globals::appManager->SetCurrentApplicationModule(2);` |
| 15 | 2056 | up | `freighter->player->setHitpoints(9999999);` |
| 16 | 2264 | up | `resetCamera(m_pLevel);` |
| 17 | 2332 | up | `m_pCamera->setLookAtCam(true);` |
| 18 | 2388 | down | `m_pCamera->enableFirstPersonCam(false);` |
| 19 | 2410 | up | `KIPlayer *pirate = (*enemies)[i];` |
| 20 | 2907 | down | `(*enemies)[0]->player->setHitpoints(0);` |
| 21 | 3232 | down | `m_pCamera->setPosition(m_tempVec);` |
| 22 | 3410 | up | `m_pRadar->enabled = 0;` |
| 23 | 1764 | down | `m_pCamera->setTarget(player->geometry);` |
| 24 | 1944 | up | `m_pHud->visible = 0;` |
| 25 | 2092 | down | `m_pHud->visible = 1;` |
| 26 | 2210 | down | `Level::programmedStation = nullptr;` |
| 27 | 2354 | up | `(*enemies)[10]->setActive(false);` |
| 28 | 2386 | down | `(*enemies)[10]->setRoute(route);` |
| 29 | 2458 | down | `m_pCamera->setLookAtCam(false);` |
| 30 | 2548 | down | `resetCamera(m_pLevel);` |
| 31 | 3219 | down | `player->hideShipForFirstPersonCameraView(false);` |
| 32 | 3563 | down | `m_pCamera->setTarget(player->geometry);` |
| 33 | 4395 | down | `resetCamera(self);` |
| 34 | 4633 | down | `m_tempVec = AbyssEngine::AEMath::MatrixGetPosition(local);` |
| 35 | 4825 | down | `m_pCamera->translate(-10000.0f, 5000.0f, 25000.0f);` |
| 36 | 5261 | down | `player->player->setVulnerable(true);` |

The distance-9..12 winner that landed is #11 (`m_pCamera->setFixed(true)`
hoisted to the top of the case-158 state-2 block, base line 4593 -> before
4581); the other distance-9..12 winner (`m_nState++`, 2988 -> 2979) did not
compose with the K=1..3 body and was dropped by the greedy re-measurement.

## Second pass on the improved tree (landed)

The same K=1..3 generator/sweep was re-run on the 52,446 tree (5,184
candidates, ~31 min).  It found **20** size-exact, `.rel.plt`-identical raw
winners (best single move -14).  Greedy accepted **10** and reached raw
**52,413**.

```
                         after pass 1     after pass 2
process raw diffbytes    52,446           52,413     (-33)
process aligned insns    22,057           22,055     (-2)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
.text diffbytes          75,288           75,255     (-33)
.ARM.extab diffbytes     656              656        (unchanged)
.note.gnu.build-id       20               20         (unchanged)
total differing bytes    75,964           75,931     (-33)
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical)
RATCHET                  PASS             PASS
```

Line numbers are from the pass-1 tree (`ba2415432`); all are K=1..3 moves
and all were individually size-exact + `.rel.plt`-identical.

| # | base line | dir | moved statement |
|---:|---:|---|---|
| 1 | 2112 | down | `delete m_pLevel->objectivesB;` |
| 2 | 2837 | down | `m_nScriptTimerB = 0;` |
| 3 | 2357 | up | `m_bCinematicBreak = 0;` |
| 4 | 2331 | down | `m_pHud->visible = 0;` |
| 5 | 2461 | up | `player->setComputerControlled(false);` |
| 6 | 3407 | up | `m_pRadar->enabled = 0;` |
| 7 | 4580 | down | `Array<KIPlayer *> *enemies4568 = m_pLevel->getEnemies();` |
| 8 | 4588 | down | `m_dirVec = player->GetDirVector();` |
| 9 | 4886 | up | `resetCamera(m_pLevel);` |
| 10 | 5263 | up | `m_bCinematicBreak = 0;` |

Move 8 is the semantics-preserving `MatrixGetPosition`-before-`GetDirVector`
order that w12506 identified as the original order at `0x142f9a`/`0x142fb6`
(the `0.008f` term must use the direction vector).

### Cumulative result (wave 182 / w12605)

`process` raw **52,523 -> 52,413** (-110) over the two passes; `.text`
**75,365 -> 75,255**; `.ARM.extab` unchanged at 656; `.rel.plt` byte-identical;
size 57,232 exact; all gate counts unchanged (`byte_exact` 4507,
`linked_exact` 4507, size-exact 4524/4524, data 843/843, imports 207, lint
CLEAN, drift 0, parity 5370/5370).

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12605
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 52446 aligned= 22057 size= 57232/57232 insn= 20728/20830
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
cmake --build cmake-build-match --target lint  -- -j2   # CLEAN
cmake --build cmake-build-match --target drift -- -j2   # 0 fields
```

The sweep harnesses (`measure.py`, `stmts.py`, `gen.py`, `eval.py`,
`sweep.py`, `apply.py`) and the result tables (`cand_k123.json`,
`cand_d912.json`, `res_k123.jsonl`, `res_d912.jsonl`, `wins_*.json`,
`apply_*.json`) live in `/home/debian/GalaxyOnFire2/scratch/w12605/`; they are
scratch, not tracked.
