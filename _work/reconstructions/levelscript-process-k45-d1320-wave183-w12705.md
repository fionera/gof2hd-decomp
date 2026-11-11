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
process raw diffbytes    52,412           52,397     (-15)
process aligned insns    22,050           22,076     (+26)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,728  20,830 / 20,730
.text diffbytes          75,015           75,000     (-15)
.ARM.extab diffbytes     646              646        (unchanged; size/base exact)
.note.gnu.build-id       20               20         (unchanged)
total differing bytes    75,681           75,666     (-15)
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
stays size- and base-exact (156,312 B @ `0x1a3b78`).

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
  *including the far endpoint*.  (The w12503/w12605 generator checked the
  earlier statement `i` against the in-between statements; that is correct only
  for the "move `i` down" direction.  For "move `j` up" the moved statement is
  `j`, and the endpoint `i` is also crossed.)  Independence = write set of one
  disjoint from read set of the other (both directions), no write/write
  overlap, and no two calls on the same receiver base.

Both directions are generated per pair (`i` moved down past `j`, and `j` moved
up before `i`); they are distinct permutations for distance >= 2.

Candidate counts: **1,936** for the K=4..5 window (922 pairs x 2 directions)
and **390** for distance 13..20.

### Evaluation (`scratch/w12705/eval.py`, `sweep.py`)

`eval.py` compiles one candidate `LevelScript.cpp` with the exact
`compile_commands.json` command, then runs the exact `build.ninja` gold link
command (stopping before `strip`; `.text`/`.rel.plt` bytes are identical in the
unstripped output).  A minimal ELF32 reader (`elfraw.py`) reports the
same-address raw diff, function size and `.rel.plt` md5.  `sweep.py` runs four
workers (`ulimit -v 12000000`); ~0.36 s/candidate.

### Greedy combination (`scratch/w12705/apply.py`)

Moves are tracked by original source line id, so a move can be applied to an
already-permuted tree.  Winners are sorted by single-move raw and added one at
a time; a move is kept only when the combined tree is still size-exact
(57,232), `.rel.plt`-identical and raw-lowering, and the move is re-checked for
depth/independence in the current order.

## Landed moves

Line numbers are from the wave-182 base (`9555474d8`).  "down"/"up" is relative
to the base position.  All are independent field stores, pure declarations, or
calls on different objects.

| # | base line | dir | moved statement |
|---:|---:|---|---|
| 1 | 1240 | up | `m_tempVec.z += -20000.0f;` (before `m_pCamera->setTarget(...)`) |
| 2 | 3713 | down | `m_pCamera->enableFirstPersonCam(false);` (after `player->stopShooting(0)`) |
| 3 | 2907 | up | `self->getPlayer()->setCollide(false);` (before `m_pRadar->enabled = 0;`) |
| 4 | 1238 | down | `m_tempVec.x += 2000.0f;` (after `m_nState = 12;`) |
| 5 | 2432 | down | `m_pCamera->enableFirstPersonCam(false);` (after `m_pHud->visible = 0;`) |
| 6 | 742 | down | `m_nScriptTimerA = 0;` (after `m_pRadar->enabled = 1;`) |

The K=4..5 sweep found 12 single-move raw winners (best -11).  Four were
rejected by the corrected independence check because the moved call crosses a
call on the same receiver base:

* `1614u1609` — `m_pCamera->setPosition(...)` moved above
  `m_pCamera->setLookAtCam(true)` (same `m_pCamera` receiver).
* `1912u1908` — `player->player->setVulnerable(true)` moved above
  `player->setComputerControlled(false)` (same `player` base).
* `2010u2005` — `player->stopShooting(0)` moved above
  `player->hideShipForFirstPersonCameraView(false)` (same `player` base).
* `2205u2200` — `Globals::status->field_f4 = ...` moved above the
  `Player *pp = player->player;` declaration; legal in isolation, but after
  the other moves it did not compose (it is a pure store/load reorder).

The remaining 6 composed greedily to raw **52,397**.  Two of the 12 winners
were alternate destinations for the same `enableFirstPersonCam` statement
(`3713d3717` / `3713d3718`); the greedy kept `3713d3717` and the other no
longer lowered raw.

## Distance-13..20 pass (no win)

390 candidates (195 pairs x 2 directions), 174 size-exact and
`.rel.plt`-identical; **none** lowered raw below 52,412.  The 64 compile
failures are declarations moved below their first use (the independence model
does not track declaration liveness).

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12705
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 52397 aligned= 22076 size= 57232/57232 insn= 20730/20830
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
cmake --build cmake-build-match --target lint  -- -j2   # CLEAN
cmake --build cmake-build-match --target drift -- -j2   # 0 fields
```

The sweep harnesses (`stmts.py`, `sweep.py`, `eval.py`, `elfraw.py`,
`apply.py`) and result tables (`res_k45.jsonl`, `res_d1320.jsonl`,
`applied_k45.json`) live in `/home/debian/GalaxyOnFire2/scratch/w12705/`; they
are scratch, not tracked.
