# LevelScript::process — wave 173 / w11804: remaining block-scope aliases

Target `_ZN11LevelScript7processEi` (original `0x137d18`, **57,232 B = 0xdf90**),
owned TU `src/game/world/LevelScript.cpp`.  Worktree `wt/w11804`, branch
`wave173-w11804`, base master `3fafa5b03` (wave 172).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w11804`.

Follow-up to `levelscript-process-alias-wave171-w11704.md` (w11704's alias set
on the wave-171 base) and the wave-172 LevelScript wins in `b8e7610f1`.

## Baseline (wave-172 master, gate green)

```
process raw diffbytes    53,750
process aligned insns    18,049
orig / ours size         57232 / 57232   (size-exact)
orig / ours insns        20830 / 20729
.text diffbytes          81,503
```

## Method

A brace-block parser (same as wave 170/171) enumerates every statement block
whose `{` is the last non-space token on its line.  For each block with >=2
uses of a member, insert `Type *alias = m_pX;` at the block head and rewrite
every use inside the block to the alias, rebuild (`link_so -j2`, ccache
incremental ~3.5 s) and measure with `tools/fn_diff.py`.  Each target was
swept one at a time on the then-current base, then re-swept on the new base to
catch additive interactions.

Swept targets: `m_pCamera`, `m_pRadar`, `m_pLevel`, `m_pGeometry5`, plus the
local/expression aliases `Globals::status` -> local `status`,
`m_pLevel->getPlayer()` -> local `player`, `PlayerEgo *pl = player`,
`Player *pp = player->player`, and `Status *st = Globals::status`.

## Landed wins (each on top of the previous)

| # | site | alias | raw | aligned | size |
|---|---|---|---:|---:|---:|
| 1 | `case 70` block (file 2464) | `Radar *radar = m_pRadar;` (2 uses) | 53,750 -> 53,741 | 18,049 -> 18,279 | 57,232 |
| 2 | mission-67 `m_nState == 5` block (file 2349) | `Level *lvl = m_pLevel;` (3 uses) | 53,741 -> 53,737 | 18,279 -> 18,275 | 57,232 |
| 3 | mission-42 state-7 `!isFading()` block (file 2191) | `Player *pp = player->player;` (4 uses) | 53,737 -> 53,734 | 18,275 -> 18,281 | 57,232 |
| 4 | mission-? `m_nState == 10` `m_nTimeLimit` block (file 4744) | `Level *lvl = m_pLevel;` (2 uses) | 53,734 -> 53,728 | 18,281 -> 18,281 | 57,232 |

The four deltas are exactly additive (-9, -4, -3, -6).  Win #4 is an
**additive-only** win: on the clean baseline that block's alias gives raw
53,869 and size 57,228; it becomes size-exact and raw-negative (-6) only on
top of wins #1-#3.  This is the kind of interaction the wave-171 note warned
about, and the reason every target was re-swept on the new base.

Win #3 is the mission-42 state-7 alias.  It is kept **block-scoped** to the
inner `if (!Globals::layout->isFading() || m_nScriptTimerA >= 10001)` block:
hoisting it to the enclosing state-7 block changes the size to 57,228, and to
the function scope to 57,264.  The pre-existing `m_pParticleGeom0` alias in
the same state-7 region is untouched (wave 171: hoisting it shrinks the
function to 57,224 B).

## Negative results (re-measured on the final base)

* `m_pCamera`: all 149 statement blocks with >=2 uses; no raw win.  Best
  size-exact results are raw-neutral (53,728) at body lines 153, 599, 2050
  and 2187; line 2187 is aligned 18,278 (-3) but was not landed (raw-neutral,
  adds a local for a 3-instruction alignment change).
* `m_pGeometry5`: all 27 blocks; no raw win.
* `m_pRadar`: all 31 blocks; only the landed `case 70` block wins.
* `m_pLevel`: all 138 blocks on the final base; only the two landed blocks
  win.  The next best is raw-neutral (body line 725).
* `Globals::status` -> local `status`: all 43 blocks; no size-exact raw win.
* `m_pLevel->getPlayer()` -> local `player`: all 9 blocks; no size-exact win.
* `PlayerEgo *pl = player`: all 213 blocks; every result is exactly neutral
  (raw 53,750 / aligned 18,049 / size 57,232 on the clean base).
* `Player *pp = player->player`: 32 blocks; only the landed mission-42
  state-7 block wins.  The same alias at the enclosing `else` block is also
  -3 raw but aligned 18,284 (slightly worse), so the inner block was kept.
* `Status *st = Globals::status`: all 32 blocks; no size-exact raw win.  The
  mission-42 state-7 block gives raw 53,931 / size 57,216, not size-exact.

## Final

```
process raw diffbytes    53,728   (was 53,750, -22)
process aligned insns    18,281   (was 18,049; raw is the ground truth)
orig / ours size         57232 / 57232   (size-exact)
orig / ours insns        20830 / 20727
.text diffbytes          81,481   (was 81,503, -22)
.rel.plt                 0 differing bytes
.ARM.extab               711 differing bytes (unchanged)
gate                     byte 4507 / linked 4507 / size 4524 / data 843 /
                         imports 207 / lint CLEAN / drift 0 / parity 5370/5370
```

Aligned rises while raw falls: the aliases change register allocation and
branch/instruction alignment, but the same-address byte diff — the
byte-identicality ground truth — drops monotonically.  This matches the
wave-170/171 convention.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 53728 aligned= 18281 size= 57232/57232 insn= 20727/20830
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```
