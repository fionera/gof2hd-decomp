# LevelScript::process — wave 180 / w12406: greedy statement reorder, two passes

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w12406`, branch
`wave180-w12406`, base master `99e341e9b` (wave-179 harvest 3, raw **52,926**).
Scratch `/home/debian/GalaxyOnFire2/scratch/w12406`.

This pass re-ran the adjacent-statement swap sweep on the wave-179 base and then
re-ran it a second time on the improved tree.  All landed edits are
semantics-preserving statement reorders inside one basic block; no
`__builtin_expect` cell was touched (w12401 owns the expect sweep).

## Result (code win landed)

```
                         base (HEAD)      this pass
process raw diffbytes    52,926           52,875            (-51)
process aligned insns    20,010           20,076            (+66)
orig / ours size         57232 / 57232    57232 / 57232     (size-exact)
orig / ours insns        20,830 / 20,731  20,830 / 20,729
.text diffbytes          76,870           76,819            (-51)
.ARM.extab diffbytes     709              709               (unchanged)
.note.gnu.build-id diff  20               19                (-1)
total differing bytes    77,599           77,547            (-52)
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
included), from `tools/fn_diff.py`; it is the primary metric.  `.rel.plt` md5
is `289b2a069f8504d1e8a3919e43915e04` before and after.  The measured base
total is 77,599 (76,870 + 709 + 20); the wave-179 doc quotes 77,598, a
one-byte bookkeeping difference in the build-id count.

## Method

A local harness (`scratch/w12406/drive.py`, `manual.py`) applies a text edit to
a saved base source, recompiles only `LevelScript.cpp` with the exact
`compile_commands.json` command, runs the real gold link + strip +
`reorder_shstrtab.py`, and reads raw/size/`.rel.plt` from the candidate ELF.
A full compile+link is ~1.5 s.

### Sweep 1 — adjacent statement swaps on the 52,926 base

`gen_swaps.py` enumerates every pair of adjacent same-indent, single-line
statements ending in `;` inside `process`, excluding declarations, `case`
labels, control-flow lines, and any line containing an RNG call
(`nextInt`/`nextFloat`/`getPulseValue`), so no RNG order can change.
**1,313 candidates**, each measured independently on the base.  22 were
size-exact, `.rel.plt`-identical wins.  A greedy combination
(`greedy.py`: at each step try every remaining win on the current tree and
keep the best) landed 20 of them, raw **52,885**.  The three-line block at
source line 2299 was then permuted exhaustively: the best order is
`setRoute / setEnemies / setSpeed`, raw **52,882**.

### Sweep 2 — repeat on the 52,882 tree

The same generator and sweep were re-run on the improved tree (another 1,313
cells).  Six more size-exact, `.rel.plt`-identical wins appeared; all six are
additive on the improved tree and take it to **52,875**.

## The landed swaps

All are adjacent-statement exchanges; every statement is an independent
store/call in the same block.  Line numbers are from the base source.

| # | source line | swapped pair (A <-> B) |
|---|---|---|
| 1 | 1369 | `(*enemies)[0]->awake()` <-> `setCloakingPossible(false)` (mission 106) |
| 2 | 1676 | `player->setComputerControlled(true)` <-> `m_bCinematicBreak = 1` |
| 3 | 1936 | `(*enemies)[0]->setVisible(true)` <-> `player->setTurretMode(false)` |
| 4 | 2008 | `player->stopShooting(0)` <-> `m_pHud->visible = 0` |
| 5 | 2010 | `m_pRadar->enabled = 0` <-> `camera41->setLookAtCam(true)` |
| 6 | 2299 | three-line rotation to `setRoute / setEnemies / setSpeed` |
| 7 | 2345 | `forceUpdate(delta,false)` <-> `m_nState = 7` |
| 8 | 2530 | `camPos += dir * 2000.0f` <-> `camPos.y += 800.0f` |
| 9 | 2531 | `camPos += dir * 2000.0f` <-> `setPosition(camPos)` |
| 10 | 2671 | `frame->setPosition(m_tempVec)` <-> `CameraSetLocal(...)` |
| 11 | 2696 | `enableSystemEmit(...)` <-> `m_nState = 9` |
| 12 | 3117 | `player->setTurretMode(false)` <-> `resetCamera(m_pLevel)` |
| 13 | 3120 | `setRocketControl(nullptr,nullptr)` <-> `killLiberator()` |
| 14 | 3124 | `enableFirstPersonCam(false)` <-> `setFreeLookMode(false)` |
| 15 | 3127 | `player->stopShooting(0)` <-> `m_bCinematicBreak = 1` |
| 16 | 3134 | `setLookAtCam(true)` <-> `setTarget(player->geometry)` |
| 17 | 3182/3183 | three-line rotation to `setActive / setVisible / m_bCinematicBreak = 0` |
| 18 | 3396-3398 | four-line rotation to `resetCamera / setTurretMode / setVisible / setFreeze` |
| 19 | 3779 | `m_pRadar->enabled = 1` <-> `m_pHud->visible = 1` |
| 20 | 4027 | `setTarget(leader)` <-> `m_tempVec = leader->getPosition()` |
| 21 | 4031 | `m_tempVec += leader->getRightVector() * -500` <-> `setPosition(m_tempVec)` |
| 22 | 4060 | `setTarget(player->geometry)` <-> `setLookAtCam(false)` |
| 23 | 4589 | `m_tempVec += m_dirVec * (step * 0.008f)` <-> `m_dirVec = getRightVector()` |

The four multi-line rotations (#6, #17, #18) were found by exhaustively
permuting the involved lines; the listed order is the measured best.  The
individual deltas are not additive (the 20-edit combination without the
line-2299 permutation is 53,153 / size +4), so the greedy order matters.

## What was tried and rejected

### Data-flow rewrites (all neutral at 52,882 unless noted)

* `bool fading` -> `int`, `bool docked` -> `int` (both sites): neutral.
* `int distance` -> `unsigned int distance` in the post-switch fail check:
  neutral.
* `int speedBits2` -> `unsigned int speedBits2`: neutral.
* `Vector offset; offset.y/x/z` assignment orders: base `y,x,z` is best;
  `x,y,z` is **53,152**, `y,z,x` is 52,883, `z,y,x` (RNG order swap) 53,143.
* `Vector offset(...)` -> `Vector offset{...}` in the case-157 else arm:
  neutral.

### Post-switch tail condition forms (all neutral at 52,882)

`distance <= 49999 && field_5e == 0` was rewritten as `&& !field_5e`,
`distance < 50000 && ...`, `!(distance > 49999 || field_5e != 0)`, and
`!(distance >= 50000 || field_5e != 0)`: all neutral.  Reordering the two
operands is size-changing (57,264, +32 B).  `if (m_nState == 0)` ->
`if (!m_nState)`, `if (m_nState <= 1)` -> `if (m_nState < 2)`, and
`if (field_0xa9 != 0)` -> `if (field_0xa9)` are all neutral.

### camPos block (source line 2529)

The original disassembly at `0x13c0e4` orders the arithmetic as
`camPos += dir*2000; camPos.x += -600; camPos.y += 800; camPos.z += -1000;
setPosition(camPos); forceUpdate; m_nState = 2; player->freeze = 0`.  Rewriting
the source to that exact order gives **52,887** (worse); only moving
`camPos.z += -1000` before `setPosition` gives **52,885** (also worse).  The
landed order (with `setPosition` before the `dir*2000` and `z` additions) is
the measured best; clang hoists the pure vector arithmetic across the call, so
the runtime camera position is unchanged.

### Other

* Adding the first-pass `m_tempVec = MatrixGetPosition(local)` /
  `m_dirVec = GetDirVector()` swap (#4587) to the 52,882 tree is
  **53,150 / size +4**; it is the one first-pass win that does not compose.
* The `Vector offset` RNG order is load-bearing: swapping the two `nextInt`
  calls changes the values, not just the code shape.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12406
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw=52875 aligned=20076 size=57232/57232 insn=20729/20830
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=52875 size=57232/57232 text=76819 relplt=289b2a069f8504d1e8a3919e43915e04
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
cmake --build cmake-build-match --target drift -- -j2   # 0 fields
cmake --build cmake-build-match --target lint  -- -j2   # CLEAN
```

The sweep harnesses (`gen_swaps.py`, `drive.py`, `sweep_swaps.py`,
`sweep_swaps2.py`, `greedy.py`, `greedy2.py`, `manual.py`, `perm3.py`,
`expdf.py`, `exptail*.py`, `expvec.py`, `expcam.py`) and the raw result tables
(`swaps_out.jsonl`, `swaps2_out.jsonl`, `wins.json`, `wins2.json`) live in
`/home/debian/GalaxyOnFire2/scratch/w12406/`; they are scratch, not tracked.
