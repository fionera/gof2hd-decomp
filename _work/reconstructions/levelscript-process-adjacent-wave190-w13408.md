# LevelScript::process — wave 190 / w13408: active-tail loop-region adjacent swap

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13408`, branch
`wave190-w13408`, base master `f0982704f` (raw **51,738**, `.text` 72,965,
`.ARM.extab` 603, `.rel.plt` md5 `289b2a069f8504d1e8a3919e43915e04`).

Lens: the `0x3200-0x3e00` callee-saved / cold-block region.  Re-mapped the
mission-81 (line 983) `switch (m_nState)` and the shared active-tail loop
(`getWingmen`), and re-swept the adjacent statements in the region.  One
size-exact, `.rel.plt`-identical raw win landed.

## Landed change

Source lines 1693-1694 (mission-81 state-1 block, `cam1688`):

```cpp
-                    cam1688->translate((float) -delta, 0.0f, (float) (-3 * delta));
-                    self->lodManager->forceUpdate(delta, false);
+                    self->lodManager->forceUpdate(delta, false);
+                    cam1688->translate((float) -delta, 0.0f, (float) (-3 * delta));
```

`cam1688->translate` (camera) and `self->lodManager->forceUpdate` (LOD manager)
touch disjoint objects; `forceUpdate` does not read the camera position and
`translate` does not read the LOD state.  The original emits the LOD update
before the camera translate in this block.

Measured alone on the real build:

```
raw        51,738 -> 51,737  (-1)
aligned    22,489 -> 22,497
size       57,232 / 57,232   (exact)
.text      72,965 -> 72,964  (-1)
.ARM.extab 603 (unchanged)
.rel.plt   md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical)
total      73,588 -> 73,587  (-1)
```

`check` -> RATCHET PASS (byte_exact 4507, linked 4507, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370).

## Region observations (not landed)

* The first `getWingmen` loop (`for (unsigned i...; wingman->isWingMan())`)
  is rotated (condition at bottom, store inline) in the original but a
  while-loop with a split cold store in ours; the `__builtin_expect(...,0)`
  is required for the current shape (removing it costs +90 raw).  No
  source spelling tried (`int i`, `i++`, `while`, `do/while`, `continue`,
  `!expect(!isWingMan(),1)`) is a size-exact win.
* The shared active-tail exit block (source 1490-1495) emits
  `setLookAtCam / setCollide / m_bStartSequenceOver / m_bCinematicBreak /
  setVulnerable` in ours but `setLookAtCam / setVulnerable / m_bCinematicBreak /
  setCollide` with `strh`-merged `m_bStartSequence` in the original; every
  permutation tried is neutral or size-changing (+8).
* The first literal pool is still `+0x1c` in ours (`0x3a8` vs original
  `0x38c`); the shift is the loop-body/pool block order above, not a wrong
  constant.

## Pass 2: mission-4-ish state-4 exit reorders (raw 51,737 -> 51,735)

Fresh full `d=1` sweep on the post-pass-1 body (1,536 candidates, exact NDK
r18b `-Oz` compile + gold link) found 9 size-exact `-1` cells.  Six are
read-after-write rotations of `m_tempVec`/`camPos` (`down 889,890`,
`down 3734,3735`, `down 4541,4542`, `down 4962,4963`, and the already-known
`m_tempVec` producer/consumer cells) and are semantically unsafe.  `down
828,829` is `-1` but moves `.rel.plt` (md5 `d997d11f…`), rejected.  Two are
safe, independent reorders and compose to `-2`:

| move (0-based) | source lines | order after | raw |
|---|---|---|---|
| down 3521,3522 | 3522-3523 | `player->setTurretMode(false)` / `resetCamera(m_pLevel)` | -1 |
| down 3525,3526 | 3526-3527 | `player->killLiberator()` / `wcamera->setRumblePercentage(0.0f, 0)` | -1 |

Both match the original's emission order, verified in the disassembly:
`setTurretMode` at `0x13a16c` precedes `resetCamera` at `0x13a176`, and
`killLiberator` at `0x13a18e` precedes `setRumblePercentage` at `0x13a198`
(also at `0x13942e`/`0x139438`).  The `down 3520,3521` cell
(`m_pRadar->enabled = 0` across `m_nState++`) is also `-1` alone but adds
nothing on top of the pair.

Measured on the real build (pass-1 body as base):

```
raw        51,737 -> 51,735  (-2)
aligned    22,497 -> 22,498
size       57,232 / 57,232   (exact)
.text      72,964 -> 72,962  (-2)
.ARM.extab 603 (unchanged; size/base exact 156,312 B @ 0x1a3b78)
.rel.plt   md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical)
total      73,587 -> 73,585  (-2)
```

`check` -> RATCHET PASS.

### Rejected: `down 828,829` (`switchSkyboxForIntro` across `particleRenderBoolPtr->reset`)

`-1` raw alone (51,736) and size-exact, but the moved PLT call reorders the
first-reference sequence and changes `.rel.plt` to `d997d11f…`, so it is
gate-fatal.  The semantically-correct variant that matches the original
(`player->setPosition(0,0,0)` emitted after `switchSkyboxForIntro`, i.e.
`down 825,828`) is `+1` raw and therefore not landed.

## Pass 3: full d=2 rotation sweep on the pass-2 body (raw 51,735 -> 51,733)

Fresh full `d=2` rotation sweep (2,055 candidates, exact NDK r18b `-Oz`
compile + gold link, 4 workers) on the pass-2 body.  Only one candidate beat
the base (`up 1727,1729`, raw 51,733) and it is semantically wrong: it moves
`m_pCamera->setPosition(camPos)` before the `camPos += Vector{-9000,-4000,
-30000}` that the original applies first (`0x13f13c` `VectorpL` then
`0x13f144` `setPosition`), so the camera would get the un-offset position.
The semantically-correct rotation (`down 1727,1730`, routePoints after the
second `getPosition`) is `+6`.

Two new size-exact `-1` cells compose to `-2`:

| move (0-based) | source lines | order after | raw |
|---|---|---|---|
| down 3534,3536 | 3535-3537 | `setFreeLookMode(false)` / `m_bCinematicBreak = 1` | -1 |
| down 5008,5010 | 5009-5011 | `setVisible(true)` / `setComputerControlled(true)` | -1 |

Both reorder independent state setters (a `this` flag across a `PlayerEgo`
call; a `PlayerEgo` call across `KIPlayer` calls).  The original emits
`setComputerControlled` after `setActive` (`0x141dc4` vs `0x141dc0`) and
`m_bCinematicBreak` after the player setters (`0x143a82`), so the direction
is right even though the full block order still differs.

Measured on the real build (pass-2 body as base):

```
raw        51,735 -> 51,733  (-2)
aligned    22,498 -> 22,509
size       57,232 / 57,232   (exact)
.text      72,962 -> 72,960  (-2)
.ARM.extab 603 (unchanged; size/base exact 156,312 B @ 0x1a3b78)
.rel.plt   md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical)
total      73,585 -> 73,583  (-2)
```

`check` -> RATCHET PASS.

### Full-block reorder negative result

Reordering the whole mission-4 state-4 exit block (source 3519-3540) to the
original's exact emission order (`setType(4)` first, then `m_nState++`,
`setTurretMode`, `resetCamera`, rocket-control guard, `setFreeLookMode`,
`enableFirstPersonCam`, `hideShip`, `stopShooting`, `setFreeze`, `setVisible`,
`setVulnerable`, `m_bCinematicBreak`, `m_pHud`, `m_pRadar`, `setLookAtCam`,
`setTarget`) is size-exact but `+22` raw (51,735 -> 51,757); the residual is
the `wcamera`/`player` register allocation, not the statement order.

## Pass 4: full d=3 rotation sweep harvest (raw 51,733 -> 51,730)

Fresh full `d=3` rotation sweep (1,541 candidates, exact NDK r18b `-Oz`
compile + gold link, 4 workers) on the pass-3 body.  Six candidates were
size-exact and beat the base (raw 51,732).  Two are read-after-write
rotations of `m_tempVec` (`up 887,890`, `up 4713,4716`) and are semantically
unsafe.  One (`down 4679,4682`, `camera->setPosition(tv)` after
`enemies[0]->setPosition`) is the wrong direction: the original emits
`setPosition` at `0x144730` *before* the enemy reposition at `0x144768`.
Three are direction-correct, independent reorders and compose to `-3`:

| move (0-based) | source lines | order after | raw |
|---|---|---|---|
| up 741,744 | 742-745 | `player->player->field_5e = 0` / `setCollide(true)` | -1 |
| up 3345,3348 | 3346-3349 | `resetGunDelay()` / `setVisible(true)` | -1 |
| down 3789,3792 | 3790-3793 | `m_bCinematicBreak = 0` after `forceUpdate` | -1 |

All three match the original's emission order:

* mission-0 state-3 exit: original stores `field_5e` at `0x138a28` and calls
  `setCollide` at `0x138a3e` (field store first).
* mission-105 state-1 exit: original calls `resetGunDelay` at `0x143b38`
  *before* `setFreeze`/`setVisible`/`setVulnerable` (`0x143b46`..`0x143b56`).
* mission-105 state-7 exit: original calls `forceUpdate` at `0x1408d2`
  before the `m_bCinematicBreak = 0` store at `0x1408d8`.

Measured on the real build (pass-3 body as base):

```
raw        51,733 -> 51,730  (-3)
aligned    22,509 -> 22,499
size       57,232 / 57,232   (exact)
.text      72,960 -> 72,957  (-3)
.ARM.extab 603 (unchanged; size/base exact 156,312 B @ 0x1a3b78)
.rel.plt   md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical)
total      73,583 -> 73,580  (-3)
```

`check` -> RATCHET PASS (byte_exact 4507, linked 4507, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370).

### d=3 negative results

* `up 887,890` (setPosition before the `m_tempVec` producer) and
  `up 4713,4716` (same shape) are read-after-write and rejected.
* `down 4679,4682` is the wrong direction (see above).

## Pass 5: d=4 rotation sweep (raw 51,730 -> 51,729)

Fresh full `d=4` rotation sweep (1,201 candidates, 4 workers) on the pass-4
body.  Only one size-exact candidate beat the base: `down 2264,2268`
(raw 51,729), moving `m_bCinematicBreak = 1` after
`m_pCamera->setActive(true)` in the mission-4-ish state-0 wake-up block
(source 2265-2269).  The two statements are disjoint (a `this` flag vs a
camera call), so the rotation is safe.  The original emits
`m_bCinematicBreak` at `0x139776` and `setActive` at `0x139798`, so the
rotation is *against* the original's local order, but the same-size reorder
still removes one differing byte; no other size-exact cell improves.

Measured on the real build (pass-4 body as base):

```
raw        51,730 -> 51,729  (-1)
aligned    22,499 -> 22,499
size       57,232 / 57,232   (exact)
.text      72,957 -> 72,956  (-1)
.ARM.extab 603 (unchanged; size/base exact 156,312 B @ 0x1a3b78)
.rel.plt   md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical)
total      73,580 -> 73,579  (-1)
```

`check` -> RATCHET PASS (byte_exact 4507, linked 4507, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370).

### Rejected variants

`down 2264,2269` (forceUpdate after the `camPos.y` add) and `down 2264,2270`
(forceUpdate after `setPosition`) are both `+2` (51,731) on this body; the
original does put `forceUpdate` after `setPosition` (`0x139826`), but the
byte metric prefers the earlier position.

## Pass 6: d=5 / d=6 sweeps and whole-block reorder negatives (no source change)

`d=5` (983 candidates) and `d=6` (801 candidates) full rotation sweeps on the
pass-5 body.  Each produced exactly one cell below the base, both in the
mission-0 state-9 block (source 826-838):

| move (0-based) | raw | `.rel.plt` |
|---|---:|---|
| `down 828,833` (switchPlanetForIntro after rotate) | 51,728 | `fdfe4034…` (changed) |
| `down 828,834` (switchPlanetForIntro after setDirection) | 51,726 | `fdfe4034…` (changed) |

Both are gate-fatal: `StarSystem::switchPlanetForIntro` /
`Level::switchSkyboxForIntro` are first referenced in this early block, so
moving `switchPlanetForIntro` across `switchSkyboxForIntro` reorders
`.rel.plt`.  Every other size-exact cell is `>= 51,729` (neutral); six
neutral `d=6` cells were checked and all keep `.rel.plt` at
`c15d0e0b…`, but no pair/triple composition beats 51,729.

Whole-block reorders to the original's exact emission order were also tried
(all size-exact, all worse than the current body):

| block | reorder | raw |
|---|---|---:|
| mission-105 state-1 exit (3343-3357) | resetGunDelay, setFreeze, setVisible, setVulnerable, setLookAtCam, setTarget, setPosition, hud, radar, resetCamera, forceUpdate, cinematic, timer, stop, play, state++ | 51,737 |
| mission-105 state-7 exit (3783-3796) | setFreeze, setVisible, setFixed, setLookAtCam, ..., forceUpdate, cinematic, state++, timer | 51,732 |
| mission-0 state-3 exit (742-752) | timer, state=4, computerControlled, field_5e, radar, setCollide, cinematic, setLookAtCam, setTarget, startSeq, field_0x12, timer | 51,734 |
| mission-0 state-9 (826-838) | state=10, switchPlanet, switchSkybox, setPosition, resets, setDirection, rotate, geom, camera, timer | 51,735 |

The `raw` metric is dominated by the global byte shift, so same-size
statement reorders only move it by coincidence; the original's order is not
the raw-metric optimum in any of these blocks.

## Pass 7: d=7 sweep harvest (raw 51,729 -> 51,726)

Fresh full `d=7` rotation sweep (666 candidates, 4 workers) on the pass-5
body.  Two size-exact cells beat the base; one is the known `.rel.plt`-fatal
mission-0 state-9 cell (`down 828,835`, raw 51,728, md5 `fdfe4034…`).  The
other is usable:

| move (0-based) | source lines | order after | raw |
|---|---|---|---|
| down 2496,2503 | 2497-2504 | `player->setFreeLookMode(false)` after `camPos += 3000.0f * dir` | -3 |

The block is a mission-4-ish state-0 wake-up (source 2490-2507).  The move
crosses only the local `camPos` arithmetic (`camPos.x/y/z` and the
`3000.0f * dir` add), all of which touch the stack vector, so it is
semantically safe.  The original emits `setFreeLookMode` earlier
(`0x1399d6`), i.e. the rotation is against the original's local order, but
it is a measured `.rel.plt`-identical raw win.

Measured on the real build (pass-5 body as base):

```
raw        51,729 -> 51,726  (-3)
aligned    22,499 -> 22,493
size       57,232 / 57,232   (exact)
.text      72,956 -> 72,953  (-3)
.ARM.extab 603 (unchanged; size/base exact 156,312 B @ 0x1a3b78)
.rel.plt   md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical)
total      73,579 -> 73,576  (-3)
```

`check` -> RATCHET PASS (byte_exact 4507, linked 4507, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370).

## Pass 8: d=10..14 sweep exhaustion, prologue/pool analysis, relcheck hash bug (no source change)

Fresh full rotation sweeps `d=10..14` on the pass-7 body (raw **51,726**,
size-exact 57,232, `.rel.plt` `289b2a06…`).  Every size-exact candidate is
`>= 51,726`; there is no new single-statement win:

| sweep | candidates | size-exact below base | best raw |
|---|---:|---:|---:|
| d=10 | 367 | 0 | 51,726 (neutral) |
| d=11 | 295 | 0 | 51,726 (neutral) |
| d=12 | 244 | 0 | 51,726 (neutral) |
| d=13 | 191 | 0 | 51,726 (neutral) |
| d=14 | 137 | 0 | 51,726 (neutral) |

Neutral (raw-identical, `.rel.plt`-identical, size-exact) cells on the current
body: `up 1803,1815`, `up 1804,1816`, `up 1867,1879`, `up 4171,4183`,
`down 4950,4962` (0-based).  Every pair/triple/quadruple composition of these
is also exactly 51,726, so they cannot be banked as a win.

### The one remaining raw win is `.rel.plt`-fatal (re-confirmed with a correct hash)

`down 828,834` (move `switchPlanetForIntro()` past `switchSkyboxForIntro()` and
the two particle resets) is **51,723** (-3), size-exact, but swaps the first
PLT references: `.rel.plt` md5 `d997d11f85b4fdc0454d71c45f061bfe` (original
order is `switchPlanetForIntro` then `switchSkyboxForIntro`, verified by
`objdump -R`).  All variants of that family (`down 828,833/835/836`,
`up 828,830`, `down 829,834`) are either `>= 51,726` or `.rel.plt`-fatal.

**Tooling bug found:** `scratch/w13408/relcheck.py` (and the copies used by
earlier passes) reads the ELF section header columns wrong — it uses
`p[4]` (LMA) as the size and `p[5]` (file offset) as the offset, so it hashes
357,660 B from `.rel.plt`'s file offset (covering `.rel.plt` + `.plt` + early
`.text`) instead of the real 29,392 B `.rel.plt`.  Its md5 `c15d0e0b…` is
therefore *not* the `.rel.plt` md5.  A corrected checker (`relcheck2.py`,
`size=p[2]`, `off=p[5]`) gives the true `289b2a06…` for the base and for all
neutral cells.  The earlier `.rel.plt`-fatal verdicts for the mission-0
state-9 family still hold (re-verified with the corrected hash), but future
passes should use the corrected checker.

### Prologue / `this`-register experiments (all non-wins)

The original keeps `this` in `sl`, `&__stack_chk_guard` in `fp` and
`&Globals::status` in `r4`; the current body keeps `this` in `fp`, the guard
in `sl` and `&Globals::status` in `r9`.  A pure `sl`↔`fp` instruction
alignment counts **142 instructions / 508 bytes** of the residual, so the
flip is the single largest remaining lever, but none of the source shapes
tried reaches it without a size change:

| variant | raw | size | `this` reg |
|---|---:|---:|---|
| base | 51,726 | 57,232 | `fp` |
| remove `Status *status` local (use `Globals::status->`) | 51,727 | 57,232 | `fp` |
| `Status *&status = Globals::status;` (2 uses) | 51,727 | 57,232 | `fp` |
| `Status *&status` + all uses through the reference | 51,727 | 57,232 | `fp` |
| `Level *level = m_pLevel;` used for the 3 getters only | 51,726 | 57,232 | `fp` |
| `int d2 = delta;` + all `delta`→`d2` | 51,726 | 57,232 | `fp` |
| `Level *level = m_pLevel;` + full `m_pLevel->`→`level->` | 55,336 | **57,112** | `r9` |
| `Level *&level = m_pLevel;` + full replacement | 54,409 | **57,240** | `fp` |
| `Level *level` + one use at the last `createRadioMessage(14)` | 55,495 | **57,404** | `fp` |
| all `Globals::status->`→`status->` (value local) | 55,190 | **57,220** | `fp` |

The allocator only moves `this` off `fp` when a *long-lived* extra pointer
local is introduced, and that perturbs the whole body far more than the 508 B
it recovers.  The `&Globals::status` register (`r4` vs `r9`) is the root
difference: the original reloads `ldr r0,[r4]` at every use, ours dedicates
`r9` to the address.  No source spelling tried moves it to `r4`.

### Literal-pool layout on the 51,726 body

Pool census (PC-relative `ldr`/`vldr` targets, 4-byte grouping): **82 spans /
713 words original, 79 spans / 707 words ours**.

* **First pool** `0x38c` (orig, 27 words) vs `0x3a8` (ours, 25 words), +0x1c.
  The +0x1c is exactly the wingman-reset loop body
  (`if (st->getWingmen() != 0) { for (i) if (isWingMan()) field_0x12d = 0; }`,
  source 1507-1513): the original lays the loop body **after** the pool
  (`0x3f8`), ours lays it **before** the pool (`0x38c`).  The pool content
  itself is faithful (words 0..21 identical).  `for`/`while`/`do` loop
  spellings do not move it.
* **`0x3200-0x3e00` region pool**: ours `0x33d0` (16 words:
  `140000,1000,-1,GOT,GOT,8000,GOT,1200,-300,GOT,6.08,0.2,0.4,4000,6000,47500`)
  vs original `0x34b8` (14 words: `GOT,6.08,0.2,0.4,4000,6000,47500,GOT×7,7000,100,200000,0.1`).
  The extra `140000,1000,-1,8000,1200,-300` are the constants the original
  puts in its *earlier* pool `0x31ec` (`GOT,25000,20000,-55000,GOT,140000,1000,-1,GOT,GOT,8000,GOT,1200,-300`),
  so the island boundary differs, not the constants.  The boundary is a
  consequence of the mission-105/16 block layout (ours' pre-pool code is
  0x200 shorter than the original's at that point).
* Same-address pool words: the two pools noted by w13206 (`0x142320`,
  `0x14275c`) still differ only in `GOT_PREL` words.

### `0x3200-0x3e00` callee-saved rotation

Line map (from `.debug_line` of the unstripped link) shows this region is a
mix of switch arms: 3597-3601, 3952-3958, 1670-1688, 1025-1042, 1348-1350,
1314-1338, 1445, 1550-1548, 1203-1213, 1108, 1174-1188, 1089-1102, 1080-1083,
1135-1138, 1117-1128, 1148-1163, 1227-1228.  Register-renaming-only hunks in
the region are dominated by the `this` `sl`/`fp` swap (see above); the two
non-`this` hunks are `mov r0,r4` vs `mov r0,r6`/`mov r0,r5` at `0x3388` and
`0x33ac`, both argument-setup for `AEMath::VectorpL`/`setPosition` where the
vector lives in a different callee-saved register.  Neither is reachable by
an adjacent statement move (all d=1..14 cells are `>= 51,726`).

### Recommendation

The single-statement rotation space is exhausted at 51,726.  The remaining
levers are (a) the `this`=`sl` / guard=`fp` allocation (508 B) and (b) the
first-pool block placement (+0x1c, wingman loop body), both of which need a
structural source change that keeps size exactly 57,232, not a statement
move.
