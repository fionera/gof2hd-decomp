# LevelScript::process — wave 174 / w11904: remaining block-scope aliases

Target `_ZN11LevelScript7processEi` (original `0x137d18`, **57,232 B = 0xdf90**),
owned TU `src/game/world/LevelScript.cpp`.  Worktree `wt/w11904`, branch
`wave174-w11904`, base master `be9dd429b` (wave 173).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w11904`.

Follow-up to `levelscript-process-alias-wave173-w11804.md` (w11804's four
aliases) and `levelscript-process-tail-inline-wave173-w11805.md` (w11805's five
aliases).  Both sets are in the wave-174 base; this pass re-swept the alias
families on the combined base, where additive interactions can flip candidates
that were neutral or negative in wave 173.

## Baseline (rebuilt from the clean tree)

```
process raw diffbytes    53,711
process aligned insns    18,228
orig / ours size         57232 / 57232   (size-exact)
orig / ours insns        20830 / 20728
.text diffbytes          80,897
.rel.plt                 byte-identical
gate: byte 4507 / linked 4507 / size 4524 / data 843 / imports 207 / lint CLEAN / drift 0
```

## Method

The wave-170/171/173 brace-block parser enumerates every statement block whose
`{` is the last non-space token on its line (709 blocks in the function).  For
each block with >= 2 uses of a target expression, insert `Type *alias =
expr;` at the block head and rewrite every use inside the block, rebuild
(`link_so -j2`, ccache incremental ~1.5 s) and measure with `tools/fn_diff.py`.
Each family was swept one candidate at a time on the wave-174 base; every
landed change was then re-swept on the new base to catch additive interactions.

Swept families (candidate blocks with >= 2 uses):

| family | candidates | best raw | result |
|---|---:|---:|---|
| `m_pCamera` | 170 | 53,707 | neutral, no win |
| `m_pRadar` | 30 | 53,707 | neutral, no win |
| `m_pLevel` | 144 | **53,707** | **win: state-5 `>= 3001` block** |
| `m_pGeometry5` | 28 | 53,707 | neutral, no win |
| `Globals::status` -> `Status *status` | 43 | 53,706 | neutral, no win |
| `Globals::status` -> `Status *st` | 32 | 53,706 | neutral, no win |
| `m_pLevel->getPlayer()` -> `PlayerEgo *player` | 9 | 53,707 | neutral, no win |
| `player->player` -> `Player *pp` | 26 | 53,707 | neutral, no win |
| `PlayerEgo *pl = player` | 215 | 53,707 | all exactly neutral |
| `m_pHud` / `m_pExplosion` / `m_pGeometry0..6` / `m_pParticleGeom0..2` | 119 | 53,707 | no win |
| `Globals::layout` / `sound` / `Canvas` / `galaxy` / `appManager` | 108 | **53,706** | **win: state-6 `Galaxy *galaxy`** |

## Landed wins (each on top of the previous)

| # | site | change | raw | delta | aligned | size |
|---|---|---|---:|---:|---:|---:|
| 1 | state-5 `m_nState == 5` / `>= 3001` block (base line 2881) | use the existing block-scope `Level *self` alias in the second `resetCamera` call (`resetCamera(m_pLevel)` -> `resetCamera(self)`) | 53,711 -> 53,707 | -4 | 18,228 -> 18,223 | 57,232 |
| 2 | state-6 `m_nState == 6` block (base line 2851) | `Galaxy *galaxy = Globals::galaxy;` (2 uses, `getStation(113)` twice) | 53,707 -> 53,706 | -1 | 18,223 -> 18,224 | 57,232 |

The deltas are exactly additive (-4, -1).  Win #1 is the same block-scope alias
shape as the wave-173 `Level *lvl = m_pLevel;` sweep result: the alias-sweep
form measures **identical** codegen (raw 53,707 / aligned 18,223 / 57,232 B),
but the original block already declares `Level *self = m_pLevel;` and the
second teardown call is the only remaining member use, so using `self`
directly is the faithful, local-free form.  Win #2 is standalone: on the clean
base the `Galaxy *galaxy` alias alone gives raw 53,710 (-1) / aligned 18,229
(+1) / 57,232 B, so it is not an additive-only win.

The mission-42 state-7 `Player *pp = player->player;` alias is untouched and
remains **block-scoped** to the inner `if (!Globals::layout->isFading() ||
m_nScriptTimerA >= 10001)` block; hoisting it still changes the function size
(57,224 B at the enclosing state-7 block).

## Negative results (re-measured on the final 53,706 base)

* `m_pCamera`: 170 blocks; the four size-exact neutrals (body lines 757, 1203,
  2656, 2793) all stay at raw 53,706.
* `m_pRadar`: 30 blocks; only the size-exact neutral at body line 4038
  (raw 53,706).
* `m_pLevel`: 142 blocks after win #1; the only size-exact neutral is body line
  1329 (raw 53,706).  The state-5 block that won is no longer a candidate
  (its remaining member use is gone).
* `m_pGeometry5`: 28 blocks; no size-exact win.
* `Globals::status` (both `status` and `st` names): 43/32 blocks; the state-5
  teardown block is a size-exact neutral (53,706), all others are worse or
  size-changing.
* `m_pLevel->getPlayer()`: 9 blocks; no size-exact win.
* `player->player` -> `Player *pp`: 26 blocks; the best is body line 1794,
  raw 53,707 (one worse than the base).
* `PlayerEgo *pl = player`: 215 blocks; every result is exactly neutral
  (53,706 / 18,224 / 57,232 on the final base).
* Extra member families (`m_pHud`, `m_pExplosion`, `m_pGeometry0..6`,
  `m_pParticleGeom0..2`): 119 blocks; no win.
* `Globals::layout` / `sound` / `Canvas` / `appManager`: no win.  The best
  `Globals::sound` blocks (body lines 3852, 5017) are one worse (53,707), and
  the best `Globals::Canvas` block (body line 2656) is one worse (53,707).
* Additive groups: the seven size-exact neutral candidates (`m_pCamera` x4,
  `m_pLevel` x1, `m_pRadar` x1, `Globals::status` x1) combined give raw
  53,706, and every pair among them is also 53,706.  No additive-only win
  remains in this alias family.

## Final

```
process raw diffbytes    53,706   (was 53,711, -5)
process aligned insns    18,224   (was 18,228, -4)
orig / ours size         57232 / 57232   (size-exact)
orig / ours insns        20830 / 20728
.text diffbytes          80,892   (was 80,897, -5)
.rel.plt                 0 differing bytes (byte-identical)
.ARM.extab               675 differing bytes (unchanged)
gate                     byte 4507 / linked 4507 / size 4524 / data 843 /
                         imports 207 / lint CLEAN / drift 0 / parity 5370/5370
```

`.text` falls by exactly the function's raw delta (-5); `.ARM.extab`,
`.rel.plt`, `.rodata`, `.data`, `.bss` and `.ARM.exidx` are unchanged.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 53706 aligned= 18224 size= 57232/57232 insn= 20728/20830
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

The two source edits are:

```c++
// state-6 arm (base line 2851)
} else if (m_nState == 6) {
    Galaxy *galaxy = Globals::galaxy;
    ...
    Globals::status->setStation((Station *) (intptr_t) galaxy->getStation(113));
    ...
    Globals::status->departStation((Station *) (intptr_t) galaxy->getStation(113));

// state-5 arm (base line 2881)
if (player->isInRocketControl()) {
    resetCamera(self);              // was resetCamera(m_pLevel)
```
