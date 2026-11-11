# LevelScript::process — wave 173 / w11805: mission-78 tail, resetCamera, ArrayAdd, and a block-alias win

Target `_ZN11LevelScript7processEi` (original `0x137d18`, **57,232 B = 0xdf90**),
owned TU `src/game/world/LevelScript.cpp`.  Worktree `wt/w11805`, branch
`wave173-w11805`, base master `3fafa5b03` (wave 172).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w11805/`.

Builds on `levelscript-process-tail-inline-wave171-w11705.md`,
`levelscript-process-alias-wave171-w11704.md` and
`levelscript-process-mission0-state8-tail-wave172-w11703.md`.

## Baseline (rebuilt from the clean tree)

```
process raw diffbytes    53,750
process aligned insns    18,049
orig / ours size         57232 / 57232   (size-exact)
orig / ours insns        20830 / 20729
.text diffbytes          81,503
.rel.plt                 byte-identical
gate: byte 4507 / linked 4507 / size 4524 / data 843 / imports 207 / lint CLEAN / drift 0
```

## Landed: five block-scoped member aliases (raw 53,750 -> 53,722)

The wave-171 alias sweep was re-run on this base over every brace block with
>= 2 uses of `m_pCamera/m_pRadar/m_pLevel/m_pHud/m_pExplosion/m_pGeometry0..6/
m_pParticleGeom0..2` (433 candidates).  Six were raw wins; five are disjoint and
combine exactly additively, and they are the ones landed.  Each is a
semantics-preserving `Type *x = m_pX;` copy of a member that is only assigned in
the constructor, scoped to the block that uses it (the same shape as the
wave-171/172 aliases).

| case | site | alias | raw | delta | aligned |
|---|---|---|---:|---:|---:|
| 16 | wormhole tutorial `messages[0]->isTriggered() && m_nState == 0` | `Level *self = m_pLevel` | 53,739 | -11 | 18,042 |
| 29 | cutscene-over `messages[2]->isOver() && expect(m_nState == 1, 1)` | `Level *self = m_pLevel` | 53,749 | -1 | 18,056 |
| 42 | mission-89 `m_nState == 6` / `player->isInWormhole()` | `Level *self = m_pLevel` | 53,745 | -5 | 18,000 |
| 67 | `messages[9]->isTriggered() && m_nState == 5` | `Level *self = m_pLevel` | 53,748 | -2 | 18,059 |
| **70** | cutscene-exit teardown (whole case) | `Radar *radar = m_pRadar` | 53,741 | -9 | **18,279** |

Combined (all five): **raw 53,722 (-28)**, aligned **18,222 (+173)**, size
**57,232/57,232**, `.rel.plt` byte-identical.  The case-70 `m_pRadar` alias is
also the largest aligned win (+230 on its own) because it removes the
`m_pRadar` reload in the state-2 arm, matching the original's `radar` register
liveness across the `hud`/`resetCamera` run.

The sixth win, `(2121,2160) m_pLevel` (raw 53,748), overlaps the landed
`(2128,2159)` block and cannot be combined with it; the inner block was kept
because it is 3 raw better.

### Second sweep (no further wins)

The same 433-candidate sweep was re-run on the 53,722 base (421 candidates
after the landed aliases are skipped).  No candidate beat 53,722; the best were
raw-neutral (`m_pCamera` at `2791..2804`, `m_pLevel` at `5394..5428`, etc.).
53,722 is a local optimum for this alias family.

## Mission-78 escort fade order — already faithful, raw-neutral

The wave-172 landing put the case-10 `m_nScriptTimerA >= 2151` fade in the
original's order:

```
(*enemies)[0]->setVisible(false);
(*enemies)[1]->setVisible(false);
(*m_pLevel->getLandmarks())[0]->setVisible(false);
```

Disassembly confirms the cold block is instruction-for-instruction identical to
the original's `0x13c82e` block (`ldr r6,[sp,#80] / movw r2,#2151 / subs /
sbcs / blt / ... / blx setVisible / blx setVisible / blx getLandmarks / blx
setVisible`), differing only in register naming and the branch target.  All
permutations of the three fades are raw-neutral (53,750); the current order is
the faithful one and is kept.

## Mission-78 tail — re-measured, still not landable

The original's mission-78 switch has a live shared tail at `0x13c88c`:

```
13c88c  ldr  r0,[sl,#28]     ; m_nState
13c890  subs r0,#5
13c892  cmp  r0,#3
13c894  bhi  13c8ba
13c896  ... translate(-delta*0.8f, 0, 0) ...
13c8ba  ldrb r5,[sl,#17]     ; m_bCinematicBreak
13c8be  b.w  144f5a          ; shared bool epilogue
```

Our source keeps the tail inside the switch braces after `case 11`'s `break`,
so it is unreachable and clang drops it; the dispatch `bhi` goes to the shared
active tail (`0x137f76`) instead of `0x13c88c`.  Re-measured shapes on this
base:

| shape | raw | aligned | size | `.rel.plt` |
|---|---:|---:|---:|---|
| HEAD (tail dead) | **53,750** | 18,049 | 57,232 | identical |
| A: tail after switch, no return | 55,530 | 20,961 | 57,260 (+28) | DIFF |
| A + `expect(state>=5 && state<=8, 0)` | 55,111 | 20,468 | 57,260 (+28) | DIFF |
| A + remove both PLT anchors | 55,419 | 20,302 | 57,236 (+4) | DIFF |
| B: tail after switch + `return m_bCinematicBreak != 0` | 55,410 | 23,716 | 57,352 (+120) | DIFF |
| tail before the switch | 55,426 | 18,060 | 57,280 (+48) | DIFF |
| A + case-12 `return`/`m_nState=13` swap | 55,510 | 21,188 | 57,284 (+52) | DIFF |

Root cause is block placement, not call shape: the original's mission-106
case-2 block (`0x13be96`, 0xec B) is interleaved between mission-78 case 11's
`break` (`0x13be92`) and the mission-78 tail (`0x13c88c`), while ours places the
whole mission-106 region ~0x1dd8 earlier.  Our case-10 cold block lands at
`0x13cdce` (original `0x13c82e`), so a live tail would sit at `0x13ce08`, not
`0x13c88c`.  Every live shape costs +28 B or more and rotates `.rel.plt`; none
is size-exact, so the dead-tail approximation stays.

### Switch case order is not a lever

The original's physical case order is `1,12,5,3,10,2,4,8,6,7,9,11`; ours is
`1,12,5,10,3,4,2,8,6,7,9,11`.  Reordering the source case bodies to the
original's order produces a **byte-identical** function (raw 53,750): clang's
MachineBlockPlacement here is driven by block frequency/fallthrough, not source
order, so the two swapped pairs (`{3,10}`, `{2,4}`) cannot be fixed by source
reordering.

## Inlined-vs-called split — re-confirmed

* `LevelScript::resetCamera` is a PLT call **51x** on both sides — never
  inlined.  The call contexts differ at ~30 sites only because the surrounding
  blocks are placed differently.
* `ArrayAdd<Player*>` is a PLT call **2x** on both sides; `Array<Player>::
  Array()` is 1x on both.  Not inlined.
* The 245-name PLT callee **set** is identical.  Direct-call **counts** differ
  by 5 (ours 2,906 / orig 2,901): ours has +1 each of `moveForward`,
  `setPosition(Vector const&)`, `getStarSystem`, `getPlayer`, `getStanding`,
  `setStanding`, `getPosition`, `setAutoTurret`, `autoTurretIsEnabled`, and
  -1 `forceUpdate`.  The extra `getStanding`/`setStanding` and
  `setAutoTurret`/`autoTurretIsEnabled` pairs are the two artificial
  PLT-order anchors:
  * `if (delta == 0x7d) status->getStanding()->setStanding(0, 100);`
  * `player->setAutoTurret(player->autoTurretIsEnabled());`
  Removing both is size-changing (57,208 B, -24) and rotates `.rel.plt`
  (26 B / 18 B respectively), so they stay.
* The third `ArrayAdd` site is the one real source difference in that block:
  the original reloads `(*enemies)[i]->player->setEnemies(targets)`
  (`0x140bc0..0x140bca`), ours uses the cached loop local
  `enemy->player->setEnemies(targets)`.  Rewriting it to `(*enemies)[i]` makes
  the two `ArrayAdd` blocks instruction-identical but is **raw 53,931 (+181)**
  (size-exact, `.rel.plt` identical) because the extra live `enemies` reload
  re-colours the whole loop.  Not landed.

## Final

```
process raw diffbytes    53,722   (was 53,750, -28)
process aligned insns    18,222   (was 18,049, +173)
orig / ours size         57232 / 57232
orig / ours insns        20830 / 20728
.text diffbytes          81,475   (was 81,503, -28)
.rel.plt                 byte-identical
gate: byte_exact 4507 / linked_exact 4507 / size 4524 / data 843 / imports 207
      lint CLEAN / drift 0 / RATCHET PASS
```

## Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 53722 aligned= 18222 size= 57232/57232 insn= 20728/20830
readelf -rW cmake-build-match/libgof2hdaa.so | awk '/\.rel\.plt/{f=1;next} f&&/^$/{exit} f{print $1, $NF}'
cmake --build cmake-build-match --target check -j2
```
