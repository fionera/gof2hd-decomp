# LevelScript::process — wave 187 / w13105: K=4..8 + d9..12 + d13..40 re-sweep and greedy 2/3-opt

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13105`, branch
`wave187-w13105`, base master `8b5d4281c` (wave-186 harvest 6, raw **51,968**,
aligned 20,856).  Scratch `/home/debian/GalaxyOnFire2/scratch/w13105`.

The w13005 harness was not preserved in scratch, so it was rebuilt from the
w13005 note (`stmts.py`, `model.py`, `gen.py`, `eval.py`, `sweep.py`,
`combine.py`, `greedy.py`, `greedy_loop.py`, `elfread.py`).  The sweep was
re-run on the current 51,968 body.

## Result (code win landed)

```
                         base (HEAD)      this pass
process raw diffbytes    51,968           51,956     (-12)
process aligned insns    20,856           20,868     (+12)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
.text diffbytes          74,274           74,262     (-12)
.ARM.extab diffbytes     647              646        (-1; size/base exact @0x1a3b78, 156,312 B)
.ARM.exidx diffbytes     0                0
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical)
file size                2,187,328        2,187,328  (exact)
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

* `stmts.py` is a character-level scanner that splits the `process` body
  (source lines 605-5370) into logical units at top-level `;`, `{` and `}`,
  tracking brace/paren depth and braced initialisers (`Vector{...}`,
  `int a[3] = {...}`).  A unit is a movable statement iff it occupies exactly
  one source line, ends with `;`, has balanced braces, and is not a control
  keyword/label.  **2,788 statements in 750 runs** (max run 29).
* `model.py` computes read/write access-chain sets per statement: assignment
  LHS (plus base prefixes) written, RHS read; calls treated as read+write of
  the receiver chain and its base prefixes; braced-initialiser declarations
  write the declared name; chains normalised so `(*x)[i]->y` == `x[i]->y`.
  Two statements conflict if a write set prefix-overlaps the other's read or
  write set (prefix-aware: `m_tempVec` conflicts with `m_tempVec.x`), or if
  both are calls on the same receiver base.
* `gen.py` keeps a pair only when every statement between them is in the same
  contiguous run and the moved statement is independent of every crossed
  statement including the far endpoint.  Both directions are generated.
* `eval.py` compiles one candidate with the exact `compile_commands.json`
  command, runs the exact gold link command (stopping before strip), and
  reports the same-address raw diff, function size and `.rel.plt` md5 with a
  minimal ELF32 reader (`elfread.py`).  Four workers, `ulimit -v 12000000`.
* `combine.py` evaluates all near-range pairs/triples of winners;
  `greedy_loop.py` adds the best improving winner one at a time.

Candidate counts: **3,146** for d4..8, **1,015** for d9..12, **658** for
d13..40.  2,990 are size-exact and `.rel.plt`-identical.

### Single-move results

* d13..40: one winner, `(*enemies)[10]->setPosition(...)` down 16 (51,967).
* d9..12: best `(*enemies)[10]->setSpeed(2.0f);` down 10/12 (51,965).
* d4..8: best `(*enemies)[10]->setSpeed(2.0f);` down 5 (51,966) and
  `m_nState++;` up 7 (51,966); ten further -1 moves.

### 2-opt / 3-opt / greedy result

The best 2-opt pair is `setPosition` down 16 + `routePoints` declaration up 4
(51,965).  Greedy addition of independent -1 moves then composes monotonically
down to **51,956** with 10 moves; no remaining winner improves the tree.

## Landed moves

Line numbers are from the base (`8b5d4281c`); "up"/"down" is relative to the
base position.

| # | base line | dir | moved statement | across |
|---:|---:|---|---|---|
| 1 | 2379 | down | `(*enemies)[10]->setPosition(origin.x, origin.y, origin.z + 12000.0f);` | … `player->stopShooting(0);` (2395) |
| 2 | 2384 | up | `int routePoints[3] = {0, 0, 800000};` | `(*enemies)[10]->setSpeed(2.0f);` (2380) … `delete m_pLevel->objectivesB;` (2383) |
| 3 | 4634 | up | `m_nState++;` | `tvA = player->getPosition();` (4627) … `forceUpdate(delta, false);` (4633) |
| 4 | 827 | down | `m_nState = 10;` | `switchPlanetForIntro();` (828) … `m_pCamera->setPosition(...)` (835) |
| 5 | 1237 | down | `m_pCamera->setTarget((*enemies)[enemies->count - 10]->geometry);` | `m_tempVec = …getPosition();` (1238) … `m_nScriptTimerA = 0;` (1244) |
| 6 | 2952 | down | `m_nScriptTimerA = 0;` | `setType(4);` (2953) … `(*enemies)[1]->setPosition(...)` (2956) |
| 7 | 3102 | up | `m_nState++;` | `forceUpdate(delta, false);` (3098) … `m_bCinematicBreak = 0;` (3101) |
| 8 | 3301 | up | `(*enemies)[i]->setVisible(true);` | `x -= 3000;` (3297) … `(*enemies)[i]->awake();` (3300) |
| 9 | 4679 | down | `camera->setFixed(false);` | `forceUpdate(delta, false);` (4680) … `setAIDisabled(false);` (4683) |
| 10 | 5103 | up | `m_pGeometry3->setDirection(Vector{0,0,-1}, Vector{0,1,0});` | `fighter->setVisible(false);` (5099) … `m_nScriptTimerB = 0;` (5102) |

Every move is independent of every crossed statement under the prefix-aware
model and was re-checked by hand against the source: the moved statement
touches only its own object/field (`enemy[10]` transform, `m_nState`,
`m_nScriptTimerA`, camera mode, `m_pGeometry3` orientation, enemy visibility),
and every crossed statement touches a different object/field.  The combination
is worth **-12 raw bytes** and also removes one `.ARM.extab` byte.

## Parser bug found and fixed mid-pass

The first scanner treated every top-level `{` as a block brace, so braced
initialisers (`Vector zero{...};`, `int routePoints[3] = {...};`) split into a
bogus `;` statement plus a barrier.  Moving the physical line then bypassed
the independence check (the harness moved whole lines but tested the text
`;`), producing a spurious -5 "winner" that crossed a write to `m_tempVec`.
The scanner now tracks initialiser braces (a `{` preceded by an identifier,
`]`, `>`, `=` or `:` is an initialiser; `else`/`do`/`)`/`;`/start is a block),
and `model.py` treats a braced declaration as a write of the declared name.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13105
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 51956 aligned= 20868 size= 57232/57232 insn= 20728/20830
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
cmake --build cmake-build-match --target lint  -- -j2   # CLEAN
cmake --build cmake-build-match --target drift -- -j2   # 0 fields
```

The harnesses (`stmts.py`, `model.py`, `gen.py`, `eval.py`, `sweep.py`,
`combine.py`, `greedy.py`, `greedy_loop.py`, `elfread.py`) and result tables
(`cands_*.json`, `res2_*.jsonl`, `res_greedy*.jsonl`) live in
`/home/debian/GalaxyOnFire2/scratch/w13105/`; they are scratch, not tracked.
