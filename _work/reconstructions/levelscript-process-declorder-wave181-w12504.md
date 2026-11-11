# LevelScript::process — wave 181 / w12504: declaration-order sweep (raw 52,818 -> 52,799)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree
`/home/debian/GalaxyOnFire2/wt/w12504`, branch `wave181-w12504`, base master
`1e270aa0e` (wave-180 harvest).  Scratch `/home/debian/GalaxyOnFire2/scratch/w12504`.

Follow-up to the wave-179 declaration-order sweep (w12305) and the wave-180
adjacent-statement reorder (w12403) / case-102 expect (w12404).  This pass
re-runs a block-scoped declaration-move search on the wave-180 base and lands
seven independent moves.

## Result (code win landed)

```
                         base (HEAD)      this pass
process raw diffbytes    52,818           52,799            (-19)
process aligned insns    20,109           20,178            (+69)
orig / ours size         57232 / 57232    57232 / 57232     (size-exact)
orig / ours insns        20,830 / 20,731  20,830 / 20,731
.text diffbytes          76,200           76,181            (-19)
.ARM.extab diffbytes     713              713               (size/base exact)
.note.gnu.build-id       20               20
total differing bytes    76,933           76,914            (-19)
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
included), from `tools/fn_diff.py`; it is the primary metric.  The aligned
instruction count rises by 69 as a block-placement/register-allocation side
effect, but size, `.rel.plt` and `.ARM.extab` are unchanged, so the -19
`.text` bytes are a net win.

## The landed source change

Seven semantics-preserving declaration moves inside `LevelScript::process`.
Every moved declaration is a pure constant, a pure getter/field load, or a
`m_pLevel` / `Globals::Canvas` alias.  No RNG call order and no mutating call
order changes.  Line numbers are the wave-180 base (`1e270aa0e`).

| # | base line | moved declaration | new position |
|---:|---:|---|---|
| 1 | 1483 | `Status *st = Globals::status;` | after `m_bStartSequence = 0;` (before `setLookAtCam(false)`) |
| 2 | 1728 | `int routePoints[3] = {...camPos...};` | before `camPos += Vector{-9000,-4000,-30000};` |
| 3 | 2136 | `Level *self = m_pLevel;` | after `resetCamera(m_pLevel);` (before `setFreeLookMode(false)`) |
| 4 | 2382 | `int routePoints[3] = {0, 0, 800000};` | before `(*enemies)[10]->setVisible(true);` |
| 5 | 2975 | `AEGeometry *leader = (*enemies)[0]->geometry;` | before `player->stopShooting(0);` |
| 6 | 3046 | `AbyssEngine::PaintCanvas *canvas = Globals::Canvas;` | after `m_nScriptTimerA += delta;` (before `AEGeometry *leader = ...`) |
| 7 | 3606-3614 | `int route0[3]` / `int route1[3]` | swap: `route1` before `route0` |

Each move was measured individually on the wave-180 base and in combination;
all seven are additive.  The individual raws are:

| move | raw | size |
|---|---:|---:|
| 1 (`st`) | 52,815 | 57,232 |
| 2 (`routePoints` 1728) | 52,815 | 57,232 |
| 3 (`self`) | 52,817 | 57,232 |
| 4 (`routePoints` 2382) | 52,817 | 57,232 |
| 5 (`leader`) | 52,815 | 57,232 |
| 6 (`canvas`) | 52,817 | 57,232 |
| 7 (`route0`/`route1` swap) | 52,811 | 57,232 |
| all seven | **52,799** | 57,232 |

### Why move 2 is semantics-preserving

The original Thumb at `0x13f11a`-`0x13f190` computes the landmark position into
`camPos` (`sp+368`), adds `{-9000,-4000,-30000}`, calls
`TargetFollowCamera::setPosition(camPos)`, then reloads `camPos` with a second
`getPosition()` and only afterwards converts `{camPos.x, camPos.y, camPos.z +
30000}` into `routePoints` (`0x13f162` loads `30000.0f`, then
`vcvt.s32`/`vstr` into `sp+280..288`, then `operator new` + `Route`).  The
source move computes `routePoints` from the first `getPosition()` value, which
is the same value the original reads after the second `getPosition()`; the
intervening `camPos +=` only mutates `camPos`, not the landmark.  The route
integers are therefore unchanged.

Move 3 (`self`) crosses only `player->setTurretMode(false)` and
`resetCamera(m_pLevel)`; `m_pLevel` is not modified.  Move 5 (`leader`) crosses
only `stopShooting`, `m_bCinematicBreak = 1` and `m_pHud->visible = 0`; none
touch `(*enemies)[0]->geometry`.  Move 6 (`canvas`) crosses only the timer
increment; `Globals::Canvas` is a global pointer load.  Move 1 (`st`) crosses
only `m_pLevel`/`field_0x24`/`m_bStartSequence`/`setLookAtCam`; `Globals::status`
is not modified.  Move 4 is a constant array.  Move 7 swaps two independent
constant-initialised arrays that both depend only on the already-loaded `d` and
`r` vectors.

## Method

`scratch/w12504/astparse.py` parses the clang `-ast-dump` of `process`
(`-Xclang -ast-dump -Xclang -ast-dump-filter -Xclang process`) into a tree and
recovers every `DeclStmt`'s exact source line range and enclosing
`CompoundStmt`.  `gen_moves.py` then enumerates, for every declaration in every
block, a move to every sibling statement position (993 candidates), with a
dependency filter:

* moving a declaration later is rejected if any crossed statement mentions the
  declared name;
* moving it earlier is rejected if its initializer mentions a name declared by
  a crossed statement;
* a declaration with a call is not moved past another statement with a call;
* a declaration containing `rnd`/`nextInt`/`nextFloat` is not moved past any
  statement containing the same (RNG order preserved).

`runner.py` compiles the candidate TU with the exact `compile_commands.json`
command and links it with the exact gold command from `build.ninja`, then
measures the same-address raw diff, the function size and the `.rel.plt` md5.
`run_all.py` evaluates the 993 candidates with four workers (`ulimit -v
12000000`); a candidate takes ~1.4 s.  661 candidates were size-exact; eleven
were raw wins.  The seven landed moves are the non-overlapping additive subset
(each of the four near-miss alternatives `st`/`routePoints`/`leader`/`canvas`
was measured at its other sibling positions; the landed positions are the
best).

## Rejected

* **`float lift` (line 4864) moved before the `m_nScriptTimerB` guard
  (52,815, -3, size-exact, `.rel.plt`-clean).**  Rejected on semantics: the
  original at `0x141932`-`0x141976` computes `lift` from
  `m_nScriptTimerA + delta` (the incremented value) and stores the sum
  afterwards; moving the declaration above `m_nScriptTimerA += delta` would
  read the old timer.
* Alternative positions of the same declarations: `st` before
  `setVulnerable` (52,817), `routePoints` before `setPosition` (52,817),
  `leader` before `m_bCinematicBreak` (52,817), `canvas` before `wcamera`
  (52,817), `routePoints` after the second `getPosition` (neutral 52,818).
* All other 650 size-exact cells are neutral or worse.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12504
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 52799 aligned= 20178 size= 57232/57232 insn= 20731/20830
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=52799 size=57232/57232 text=76181 relplt=289b2a069f8504d1e8a3919e43915e04
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
cmake --build cmake-build-match --target lint  -- -j2   # CLEAN
cmake --build cmake-build-match --target drift -- -j2   # 0 fields
```

Scratch harnesses (not tracked): `astparse.py`, `gen_moves.py`, `runner.py`,
`run_all.py`, `multi.py`, `pairs.py`, `hotlines.py`, `moves.json`,
`moves_results.tsv` in `/home/debian/GalaxyOnFire2/scratch/w12504/`.
