# LevelScript::process — wave 179 / w12302: fresh alias/hoist sweep

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`,
original `0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w12302`,
branch `wave179-w12302`, base master `64014ce6f` (wave-178 merge).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w12302`.

Follow-up to the wave-177 alias sweep (`levelscript-process-alias-greedy-
wave177-w12102.md`) and the wave-178 statement reorder (w12203), which took the
function to **53,082** raw.  This pass re-enumerated every brace block in
`process` and re-tested block-scope aliases from scratch on that base.

## Result (code win landed)

```
                         base (HEAD)      this pass
process raw diffbytes    53,082           53,060     (-22)
process aligned insns    20,149           20,191     (+42)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,728  20,830 / 20,729
.text diffbytes          77,936           77,914     (-22)
.ARM.extab diffbytes     700              700        (unchanged)
.rel.plt diffbytes       0                0          (byte-identical)
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
is `289b2a069f8504d1e8a3919e43915e04` before and after.

## The four landed aliases

All four are block-scope receiver aliases.  They are ordinary human C++ and
each replaces two or three repeated loads of a stable pointer inside one
block.  The deltas are exactly additive: -8, -7, -4 and -3 bytes.

1. **Mission-7 docking block** (`m_nState == 7`, file lines 3103-3113), the
   inner `if (player->isDockedToDockingPoint())` body:

   ```cpp
   Radar *radar = m_pRadar;
   ...
   radar->lockedEnemy = nullptr;
   radar->dockNavPtr = nullptr;
   ...
   self->getPlayer()->dockToDockingPoint(nullptr, radar);
   ```

   Alone: raw **53,074** (-8), size exact.

2. **Post-sequence block** (file lines 1482-1512), the
   `field_0x24 >= 7001` transition:

   ```cpp
   Status *st = Globals::status;
   ...
   if (!Level::programmedStation->equals(st->getStation())) { ... }
   ...
   if (st->getWingmen() != 0) { ... }
   ```

   Alone: raw **53,075** (-7), size exact.  Reusing the existing function-scope
   `Status *status` local instead of a new block-scope `st` is much worse
   (55,114), so the original evidently reloaded `Globals::status` into a fresh
   local here.

3. **Mission-6 camera block** (`m_nState == 6`, file lines 2660-2676):

   ```cpp
   AbyssEngine::PaintCanvas *canvas = Globals::Canvas;
   ...
   AEGeometry *frame = new AEGeometry(canvas);
   ...
   canvas->CameraSetLocal(canvas->CameraGetCurrent(), frame->getMatrix());
   ```

   Alone: raw **53,078** (-4), size exact.

4. **Mission-3 drift block** (`m_nState == 3`, file lines 3623-3635):

   ```cpp
   AEGeometry *geom = player->geometry;
   ...
   Vector drift = (geom->getDirection() * 6.08f
                   + geom->getRightVector() * 0.2f) * 0.4f * (float) delta;
   ```

   Alone: raw **53,079** (-3), size exact.

The other two size-exact single-candidate wins found by the sweep were
`m_pRadar` over the whole `m_nState == 7` body (53,075, -7; it overlaps alias 1
and is 1 byte worse) and `m_pLevel->getEnemies()` in the mission-0
`field_0x12d` loop (53,081, -1; adding it to the four-edit set breaks size,
57,224).  The best four-edit set is the one above at **53,060**.

## Method

### Block enumeration

`scratch/w12302/blocks.py` parses the `process` body, ignoring comments and
string/char literals, and records every `{ ... }` block with line numbers.  It
then counts exact textual occurrences of the candidate expressions in each
block body (word-boundary checked):

```
m_pLevel, m_pCamera, m_pHud, m_pRadar, Globals::Canvas, Globals::status,
Globals::sound, Globals::rnd, Globals::galaxy, Globals::layout,
Globals::appManager, m_pLevel->getEnemies(), m_pLevel->getPlayer(),
m_pLevel->getMessages(), player->player, player->geometry
```

862 blocks; 670 block/expression candidates with at least two uses.  Blocks
that are a `switch` body are skipped for declaration insertion (a declaration
before the first `case` label is a "jump bypasses variable initialization"
error in C++); their case bodies are enumerated separately and were tested.

### Sweep

`scratch/w12302/sweep.py` inserts `Type *name = expr;` at the block head,
rewrites every occurrence in the block body, rebuilds the real gold link
(`ninja -C cmake-build-match libgof2hdaa.so -j2`) and measures with
`scratch/w12302/measure.py` (same-address `.text` byte diff of the function,
size, `.rel.plt` md5).

* 253 candidates with >=5 uses: no size-exact win (best 53,084).
* 402 candidates with 2-4 uses: six size-exact wins, listed above.

### Combination search

`scratch/w12302/combo.py` applies subsets of the six wins.  All 63 non-
overlapping subsets were measured; the best is
`{rad_in, stat, canv, geom}` = **53,060** (size 57,232 exact, `.rel.plt`
identical).  `rad_out` instead of `rad_in` is 53,061; the other three-way
subsets are 53,063-53,068.

`scratch/w12302/addone.py` then added each of the 67 other size-exact
candidates (raw <= 53,200 alone) on top of the four-edit base.  None improved
on 53,060; the neutral ones stayed at 53,060 and the rest were 53,061-53,129.
`scratch/w12302/multiextra.py` tested all 120 pairs of the 16 best extras
(raw <= 53,062); no pair improved on 53,060.  The four-edit set is a local
optimum for this alias family.

### Other hoist/order cells (all rejected)

`scratch/w12302/toplevel.py` permuted the four function-scope declarations
(`messages, player, enemies, status`).  The current order is the best
(53,082); the next best permutation is 53,086.

`scratch/w12302/hoist.py full` hoisted each candidate expression to function
scope and rewrote every use.  All 16 changed the function size (none
57,232 B), so none can be landed.

`scratch/w12302/declorder.py` swapped adjacent alias declarations at every
block head with two independent declarations.  All swaps were neutral
(53,082) or worse (53,353).

`scratch/w12302/variants.py` tested receiver re-use inside the winning blocks
(`self->getPlayer()` -> `player`, `lvl->getPlayer()` -> `player`,
`m_pLevel->getPlayer()` -> `player`, `m_pLevel` -> `lvl`, adding a camera
alias in the mission-6 block).  All were worse or size-changing; the best
variant stayed at 53,060.

## Reproduction

```sh
ulimit -v 12000000
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 53060 aligned= 20191 size= 57232/57232 insn= 20729/20830
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
```

The four source edits are the aliases listed above.  The sweep harnesses are
`blocks.py`, `sweep.py`, `measure.py`, `combo.py`, `addone.py`,
`multiextra.py`, `toplevel.py`, `hoist.py`, `declorder.py` and `variants.py`
in `/home/debian/GalaxyOnFire2/scratch/w12302`.
