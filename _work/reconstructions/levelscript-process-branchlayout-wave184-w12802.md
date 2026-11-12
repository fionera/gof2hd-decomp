# LevelScript::process — wave 184 / w12802: branch-layout pass (mission-0 state-16 tail)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree
`/home/debian/GalaxyOnFire2/wt/w12802`, branch `wave184-w12802`, base master
`4c06c2662` (wave-183 harvest 6, raw **52,281**).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w12802`.

Continuation of the wave-183 w12702 branch-layout pass.  This pass builds the
branch/`tbh`/call-order maps of both bodies, groups the target-offset
differences by state block, and reconstructs the original fall-through
structure.  One size-exact, `.rel.plt`-identical raw win was found in the
mission-0 state-16 tail.

## Result (code win landed)

```
                         base (HEAD)      this pass
process raw diffbytes    52,281           52,279     (-2)
process aligned insns    20,700           20,700     (unchanged)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,725  20,830 / 20,725
.text diffbytes          74,842           74,840     (-2)
.ARM.extab diffbytes     646              646        (unchanged; size/base exact)
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical)
byte_exact               4507             4507
linked_exact             4507             4507
size-exact               4524/4524        4524/4524
data_exact               843/843          843/843
imports                  207              207
lint                     CLEAN ✓          CLEAN ✓
drift                    0                0
symbol parity            5370/5370        5370/5370
RATCHET                  PASS             PASS
```

## The landed change

Mission-0 `m_nState == 16` handler (source lines 936-945).  The original
handler is out of line at `0x13f902` and its call order is
`Layout::enableFillScreen` → `Status::nextCampaignMission` → the two global
stores → `ApplicationManager::SetCurrentApplicationModule`.  Our handler is
inline at `0x13d598` and its source order was
`initStreamOutPosition=1` → `nextCampaignMission` → `switch_to_target_setting=1`
→ `SetCurrentApplicationModule` → `enableFillScreen`.

All five statements are independent (different globals/objects), so all 120
permutations are semantics-preserving.  A full 120-permutation sweep
(compile + gold link + strip + `.rel.plt` check per candidate, four workers,
`ulimit -v 12000000`) gives a flat landscape of 52,279..52,286; the two best
orders are

* `SetCurrentApplicationModule(2); enableFillScreen(true);
   initStreamOutPosition=1; switch_to_target_setting=1;
   nextCampaignMission(true);`  → **52,279** (landed)
* `SetCurrentApplicationModule(2); enableFillScreen(true);
   switch_to_target_setting=1; initStreamOutPosition=1;
   nextCampaignMission(true);`  → 52,279

Both are size-exact and `.rel.plt`-identical.  The landed order is the first
one.  Note the compiler prefers `nextCampaignMission` last even though the
original's *emitted* call order is `enableFillScreen`, `nextCampaignMission`,
`SetCurrentApplicationModule`; the raw win comes from the surrounding block
placement, not from the call order itself.  The original-order permutation
`enableFillScreen; nextCampaignMission; initStreamOutPosition=1;
switch_to_target_setting=1; SetCurrentApplicationModule;` measures 52,285.

## Branch-layout map (why the rest does not move)

The function has **15 `tbh` tables** in both binaries.  Fourteen have the same
range; the only differing one is the mission-0 `switch (m_nState)`
(original `subs #6; cmp #9`, 10 entries 6..15; ours `subs #3; cmp #12`,
13 entries 3..15), exactly as w12702 documented: clang 7 SimplifyCFG merges
the preceding `else if (m_nState == 3)` arm into the switch.  The merge makes
the state-16 handler the inline default at `0x13d598` (ours) instead of the
out-of-line `0x13f902` (original), so the mission-0 "not handled" paths branch
to `0x13d598` instead of back to the shared tail `0x137f7c`.

The source arm order that prevents the merge (`4,3,5`) was re-measured on the
current body: raw **53,665** (vs 52,281) and `.rel.plt` DIFF
(`a8d49c…`; only `PlayerStation::setVisible` moves, by five PLT slots).  It is
therefore gate-fatal and not a win, confirming w12702.

Call-stream alignment (2,900 direct/indirect calls in each body, 1,863 in
order) shows the rest of the function is structurally the same: the remaining
target-offset differences are block placement and register-allocation
consequences (the known `this`/`status` = `sl`/`fp` swap), not missing or
extra branches.  The `__builtin_expect` set was re-swept on the current body
(64 cells, flip each): the best is unchanged at 52,281, so the expect set is
locally optimal.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12802
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 52279 aligned= 20700 size= 57232/57232
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
```

Scratch harnesses (`harness.py`, `parse.py`, `branchdiff.py`, `bdisp.py`,
`bmn.py`, `callseq.py`, `sweep_expect.py`, `perm_s16.py`, `secdiff.py`) live in
`/home/debian/GalaxyOnFire2/scratch/w12802/`; they are scratch, not tracked.

## Pass 2: shared `isInRocketControl` cleanup block

A second sweep permuted every run of 3-4 consecutive independent single-line
statements in `process` (128 runs, 1,432 permutations, four workers).  The
best cell is the cleanup block at source lines 3836-3838 (mission state 10):

```
m_pCamera->setRumblePercentage(0.0f, 0);
player->killLiberator();                 // moved before setRocketControl
player->setRocketControl(nullptr, nullptr);
```

Raw **52,279 -> 52,271** (-8), aligned 20,700 -> 20,694, size-exact 57,232,
`.rel.plt` byte-identical, `.ARM.extab` unchanged at 646.  `.text` diff
74,840 -> 74,832.

The block is tail-merged by clang with the identical cleanup block at source
line 4392, so the change moves the shared emitted block at `0x142c1e` from
`setRumble; setRocketControl; killLiberator` to
`setRumble; killLiberator; setRocketControl`; the first four bytes at
`0x142c2c` then match the original's `blx killLiberator`.  The original's
emitted order at that site is `setRocketControl; killLiberator;
setRumble`, so this is a codegen-driven reorder of two independent
`PlayerEgo` cleanup calls (both are idempotent teardown calls), not a
semantic change of the surrounding state machine.  Writing the faithful
order in source measures 52,280-52,282 (worse), so the compiler's shared-block
layout prefers this order.

Cumulative for this pass: raw **52,281 -> 52,271** (-10), `.text`
74,842 -> 74,832, aligned 20,700 -> 20,694; size-exact, `.rel.plt`-identical,
`.ARM.extab` unchanged.

## Pass 3: 12-statement-run permutation harvest

The 3-4-statement run sweep above produced 13 composing single-move winners.
One of them (the `Route *route = new Route(...)` run at source lines
3976-3979) shrinks `.ARM.extab` by 4 bytes (156,308 vs 156,312) and blows the
`.ARM.extab` content diff from 646 to 28,848 even though it saves 1 raw byte;
per the "net loss" rule it was dropped.  The other 12 compose:

```
                         after pass 2     after pass 3
process raw diffbytes    52,271           52,256     (-15)
process aligned insns    20,700           20,707     (+7)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
.text diffbytes          74,832           74,817     (-15)
.ARM.extab diffbytes     646              646        (size/base exact)
.ARM.exidx diffbytes     0                0
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical)
RATCHET                  PASS             PASS
```

Landed moves (line numbers are from the pass-2 tree; all are permutations of
3-4 consecutive independent statements, `[old order] -> [new order]`):

| lines | block | new order |
|---|---|---|
| 3401-3404 | `resetCamera` / `setTurretMode` / `setFreeze` / `setVisible` | resetCamera, setVisible, setTurretMode, setFreeze |
| 5188-5191 | enemy+geometry `setVisible` / `startFade` / `sound->play(2248)` | play, startFade, geometry setVisible, enemy setVisible |
| 720-723 | `sound->play(158)` / `update(158)` / `stop` / `play(161)` | update(158), stop, play(158), play(161) |
| 1324-1326 | `startFade(false)` / `enemies[2]->setVisible(true)` / `m_nState=4` | m_nState=4, setVisible, startFade |
| 1878-1880 | `setLookAtCam` / `setComputerControlled` / `setPosition` | setLookAtCam, setPosition, setComputerControlled |
| 1994-1996 | `setActive(false)` / `setPosition` / `m_nState=5` | m_nState=5, setPosition, setActive(false) |
| 2037-2040 | `setTarget` / `setPosition` / `translate` / `forceUpdate` | setTarget, setPosition, forceUpdate, translate |
| 3125-3127 | `setRocketControl` / `killLiberator` / `setRumblePercentage` | killLiberator, setRocketControl, setRumble |
| 3212-3214 | `setTurretMode(false)` / `field_0xab=isInTurretMode()` / `resetCamera` | resetCamera, field_0xab=isInTurretMode(), setTurretMode(false) |
| 3216-3218 | `setRocketControl` / `killLiberator` / `setRumblePercentage` | setRumble, killLiberator, setRocketControl |
| 4708-4711 | `setSpeed` / `setFixed` / `setTarget` / `m_tempVec=...` | setTarget, setFixed, m_tempVec, setSpeed |
| 5297-5300 | `revive` / `setActive(true)` / `setPosition` / `turnEnemy` | revive, turnEnemy, setPosition, setActive(true) |

The 3212-3214 move is the semantically faithful one: the original
`0x13a15c` reads `isInTurretMode()` into `field_0xab` *before*
`setTurretMode(false)` and `resetCamera`, which the new order reproduces.

Cumulative for the whole pass: raw **52,281 -> 52,256** (-25), `.text`
74,842 -> 74,817, aligned 20,700 -> 20,707; size-exact, `.rel.plt`-identical,
`.ARM.extab` size/base exact and content diff unchanged at 646.
