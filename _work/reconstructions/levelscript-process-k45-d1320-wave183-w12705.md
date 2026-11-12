# LevelScript::process — wave 183 / w12705: K=4..5 window + distance-13..20 sweeps

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree
`/home/debian/GalaxyOnFire2/wt/w12705`, branch `wave183-w12705`, base master
`9555474d8` (wave-182 harvest, raw **52,412**, aligned 22,050).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w12705`.

Continuation of the wave-182 w12605 K=1..3 + distance-9..12 sweeps.  This pass
runs **K=4 and K=5** non-adjacent statement moves and a **distance-13..20**
pass on the wave-182 body.

## Result (code win landed)

```
                         base (HEAD)      this pass
process raw diffbytes    52,412           52,398     (-14)
process aligned insns    22,050           20,391     (-1,659)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,728  20,830 / 20,730
.text diffbytes          75,015           75,001     (-14)
.ARM.extab diffbytes     646              646        (unchanged; size/base exact)
.note.gnu.build-id       20               20         (unchanged)
total differing bytes    75,681           75,667     (-14)
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
included), from `tools/fn_diff.py`; `.rel.plt` md5 is
`289b2a069f8504d1e8a3919e43915e04` before and after (byte-identical to the
original).  `.ARM.extab` content diff is unchanged at 646 bytes and the section
stays size- and base-exact (156,312 B @ `0x1a3b78`).  The large aligned-insn
drop is a block-placement/register-allocation side effect of the reordered
statements; size, `.rel.plt` and `.ARM.extab` are unchanged.

## Method

### Candidate generation (`scratch/w12705/stmts.py`, `sweep.py`)

`stmts.py` parses the `process` body (lines 605-5357), tracks brace depth and
keeps single-line statements (trimmed line ends with `;`, no braces, balanced
parens/brackets, not `case`/`default`/`break`/`continue`/`return` and not a
control-flow header).  This yields **2,672 statements** in the current body.

For every statement pair `(i, j)` with `i < j` and `j-i <= K` the move is kept
only when:

* every statement between `i` and `j` has the same brace depth (no block
  boundary is crossed);
* every non-blank/non-comment source line between the two statement lines is
  itself a recognized statement line (no `if`/`for`/`while`/`switch`/brace
  line is crossed);
* the **moved** statement is independent of every statement it crosses,
  **including the far endpoint** (moving `i` down crosses `j`; moving `j` up
  crosses `i`).  The w12503/w12605 generator checked the earlier statement `i`
  against the in-between statements, which is correct only for the "move `i`
  down" direction.
* independence is **prefix-aware on access chains**: `m_tempVec` aliases
  `m_tempVec.x` (a write to the whole vector overwrites a component write),
  while `m_tempVec.x` and `m_tempVec.y` do not alias.  Write/write,
  write/read and read/write overlaps all block the move, and two calls on the
  same receiver base keep their order.

Both directions are generated per pair (`i` moved down past `j`, and `j` moved
up before `i`); they are distinct permutations for distance >= 2.

Candidate counts: **1,743** for the K=4..5 window (distance 4..5, both
directions) and **427** for distance 13..20.

### Evaluation (`scratch/w12705/eval.py`, `sweep.py`)

`eval.py` compiles one candidate `LevelScript.cpp` with the exact
`compile_commands.json` command, then runs the exact `build.ninja` gold link
command (stopping before `strip`; `.text`/`.rel.plt` bytes are identical in the
unstripped output).  A minimal ELF32 reader (`elfraw.py`) reports the
same-address raw diff, function size and `.rel.plt` md5.  `sweep.py` runs four
workers (`ulimit -v 12000000`); ~0.36 s/candidate at low load.

### Greedy combination (`scratch/w12705/apply.py`)

Moves are tracked by original source line id, so a move can be applied to an
already-permuted tree.  Winners are sorted by single-move raw and added one at
a time; a move is kept only when the combined tree is still size-exact
(57,232), `.rel.plt`-identical and raw-lowering, and the move is re-checked for
depth/prefix-aware independence in the current order.

## Landed moves

Line numbers are from the wave-182 base (`9555474d8`).  "down"/"up" is relative
to the base position.  All are independent field stores, pure declarations, or
calls on different objects.

| # | base line | dir | moved statement |
|---:|---:|---|---|
| 1 | 3713 | down | `m_pCamera->enableFirstPersonCam(false);` (after `player->stopShooting(0)`) |
| 2 | 3715 | up | `m_bCinematicBreak = 1;` (before `player->setFreeLookMode(false)`) |
| 3 | 2907 | up | `self->getPlayer()->setCollide(false);` (before `m_pRadar->enabled = 0;`) |
| 4 | 1238 | down | `m_tempVec.x += 2000.0f;` (after `m_nState = 12;`) |
| 5 | 2205 | up | `Globals::status->field_f4 = player->getCurrentSecondaryWeaponIndex();` (before `Player *pp = player->player;`) |
| 6 | 2432 | down | `m_pCamera->enableFirstPersonCam(false);` (after `m_pHud->visible = 0;`) |
| 7 | 3932 | up | `Level::initStreamOutPosition = 1;` (before `Globals::status->field_64 = ...`) |
| 8 | 742 | down | `m_nScriptTimerA = 0;` (after `m_pRadar->enabled = 1;`) |

The K=4..5 sweep found 10 single-move raw winners (best -4).  Two were
alternate destinations for the same statement (`3713d3717` / `3713d3718`) and
one (`747u742`) was an alternate for the `m_nScriptTimerA` region; the greedy
kept the better of each and reached raw **52,398**.

## Prefix-aware independence: a semantic bug avoided

An earlier version of this pass used exact-string write sets.  It moved
`m_tempVec.z += -20000.0f;` above `m_tempVec = ...->getPosition();` (raw
52,397) because `m_tempVec.z` and `m_tempVec` were treated as distinct.  The
whole-vector assignment overwrites the component add, so the reorder is not
semantics-preserving.  The original disassembly at `0x13be2c-0x13be72` is
unambiguous: `AEGeometry::getPosition`, then `AEMath::Vector::operator=`, then
the x/y/z adds, then `TargetFollowCamera::setPosition`.  The landed tree keeps
that order (the x add is only moved below the y/z adds and the unrelated
`forceUpdate`/`m_nState` stores; it stays after the assignment).

## Distance-13..20 pass (no win)

432 candidates (both directions, prefix-aware), 205 size-exact and
`.rel.plt`-identical; **none** lowered raw below 52,398.  The earlier
exact-string pass on the base body also found no winner (390 candidates).

## Second K=4..5 pass on the improved tree (landed)

The same generator/sweep was re-run on the 52,398 tree (1,741 candidates).
It found **4** size-exact, `.rel.plt`-identical raw winners (best -3).  Greedy
accepted 3 (the fourth, `3716d3721`, was an alternate destination for
`player->stopShooting(0)` and did not compose) and reached raw **52,393**.

```
                         after pass 1     after pass 2
process raw diffbytes    52,398           52,393     (-5)
process aligned insns    20,391           20,391     (0)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
.text diffbytes          75,001           74,996     (-5)
.ARM.extab diffbytes     646              646        (unchanged)
total differing bytes    75,667           75,662     (-5)
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical)
RATCHET                  PASS             PASS
```

Line numbers are from the pass-1 tree (`e7238a50d`).

| # | base line | dir | moved statement |
|---:|---:|---|---|
| 1 | 2435 | up | `m_pHud->visible = 0;` (before `player->setTurretMode(false)`) |
| 2 | 2904 | down | `m_pRadar->enabled = 0;` (after `(*enemies)[0]->player->setHitpoints(0)`) |
| 3 | 3716 | down | `player->stopShooting(0);` (after `m_pCamera->setTarget((*enemies)[2]->geometry)`) |

## Third K=4..5 pass on the improved tree (landed)

A third sweep on the 52,393 tree (1,741 candidates) found **4** winners
(best -11).  Greedy accepted 2 (the other two were alternate destinations for
the same statements) and reached raw **52,381**.

```
                         after pass 2     after pass 3
process raw diffbytes    52,393           52,381     (-12)
process aligned insns    20,391           20,402     (+11)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
.text diffbytes          74,996           74,984     (-12)
.ARM.extab diffbytes     646              646        (unchanged)
total differing bytes    75,662           75,650     (-12)
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical)
RATCHET                  PASS             PASS
```

Line numbers are from the pass-2 tree (`238c5f7cf`).

| # | base line | dir | moved statement |
|---:|---:|---|---|
| 1 | 3720 | down | `player->stopShooting(0);` (after `m_nScriptTimerB = 0;`) |
| 2 | 3717 | down | `m_pHud->visible = 0;` (after `Vector *camPos = &m_tempVec;`) |

### Final cumulative result (wave 183 / w12705)

`process` raw **52,412 -> 52,381** (-31) over the three K=4..5 passes;
aligned **22,050 -> 20,402**; `.text` **75,015 -> 74,984**; `.ARM.extab`
unchanged at 646; `.rel.plt` byte-identical; size 57,232 exact; all gate counts
unchanged (`byte_exact` 4507, `linked_exact` 4507, size-exact 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370).

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12705
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 52381 aligned= 20402 size= 57232/57232 insn= 20730/20830
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
cmake --build cmake-build-match --target lint  -- -j2   # CLEAN
cmake --build cmake-build-match --target drift -- -j2   # 0 fields
```

The sweep harnesses (`stmts.py`, `sweep.py`, `eval.py`, `elfraw.py`,
`apply.py`) and result tables (`res3_k45.jsonl`, `res4_k45.jsonl`, `res5_k45.jsonl`, `res3_d1320.jsonl`, `applied3_k45.json`, `applied4_k45.json`, `applied5_k45.json`) live in
`/home/debian/GalaxyOnFire2/scratch/w12705/`; they are scratch, not tracked.
