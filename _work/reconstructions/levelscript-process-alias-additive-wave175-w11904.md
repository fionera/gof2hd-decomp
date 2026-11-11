# LevelScript::process — wave 175 / w11904: additive block-scope aliases

Target `_ZN11LevelScript7processEi` (original `0x137d18`, **57,232 B = 0xdf90**),
owned TU `src/game/world/LevelScript.cpp`.  Worktree `wt/w11904`, branch
`wave175-w11904`, base master `5f599a137` (wave 174).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w11904`.

Follow-up to `levelscript-process-alias-wave173-w11804.md`,
`levelscript-process-blockorder-wave173-w11806.md` and
`levelscript-process-deactivation-wave174-w11906.md`.

## Result (code win landed)

```
                         base (HEAD)      this wave
process raw diffbytes    53,673           53,651     (-22)
process aligned insns    18,226           18,216     (-10)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,729  20,830 / 20,729
.text diffbytes          80,439           80,417     (-22)
.ARM.extab diffbytes     750              750        (unchanged)
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
included), from `tools/fn_diff.py`.  Raw is the ground truth; the aligned
count is secondary.

## Method

The remaining block-scope aliases were re-swept one at a time on the wave-174
base, then in additive groups:

* `m_pCamera` — all 171 statement blocks with >=2 uses; no size-exact raw win.
  Best size-exact results are raw-neutral (53,673) at body lines 757, 2656,
  2793.
* `m_pRadar` — all 37 blocks; no raw win.  The only neutral block is
  `L4038-4041` (raw 53,673).
* `m_pLevel` — all 147 blocks; no individual raw win.  The only neutral block
  is `L1329-1340` (raw 53,673).
* `m_pGeometry5` — all 28 blocks; no individual raw win.
* `Player *pp = player->player` — all 30 blocks; no individual raw win.
* `Status *status` / `Globals::status` — all 44 blocks, both a fresh
  `Status *statusA = Globals::status;` and substitution with the existing
  function-scope `status`; no size-exact raw win.
* `m_pLevel->getPlayer()` — existing `player` and a fresh
  `PlayerEgo *playerA`; no size-exact raw win.

The individual sweeps are therefore all negative on this base.  The win is
**additive**: two size-compensating alias pairs, each pair a `-4 B` alias and
a `+4 B` alias, combine to size-exact and raw-negative.

## The change

Four block-scoped aliases, all in `LevelScript::process`:

1. `AEGeometry *geom5 = m_pGeometry5;` in the `m_nState == 13` block
   (`L2776-2808`), replacing the two `m_pGeometry5` uses in that block.
2. `Level *lvl = m_pLevel;` in the `m_nScriptTimerA >= 1001` block
   (`L4630-4639`), replacing the two `m_pLevel->getEnemies()` uses.
3. `TargetFollowCamera *cam = m_pCamera;` in the `m_nScriptTimerA >= 7001`
   block (`L3157-3177`), replacing the four `m_pCamera` uses.
4. `Level *lvl = m_pLevel;` in the enclosing `m_nScriptTimerA >= 2001` block
   (`L3150-3178`), replacing the two `m_pLevel` uses (`resetCamera` and
   `lodManager->forceUpdate`).

`m_pCamera`, `m_pRadar`, `m_pLevel` and `m_pGeometry5` are assigned only in
the constructor (`LevelScript.cpp:55-57`, `:112/:143/:153/:476`), never in
`process`, so every alias is semantics-preserving.  The aliases stay
block-scoped; hoisting any of them changes the size or regresses raw.

### Additive measurements (clean wave-174 base)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | 53,673 | 18,226 | 57,232 |
| `geom5` alone (`L2776-2808`) | 53,782 | 18,236 | 57,228 (-4) |
| `lvl` alone (`L4630-4639`) | 53,793 | 18,207 | 57,236 (+4) |
| `cam` alone (`L3157-3177`) | 53,708 | 18,225 | 57,228 (-4) |
| `lvl` alone (`L3150-3178`) | 53,712 | 18,226 | 57,236 (+4) |
| pair A: `geom5` + `lvl(L4630)` | 53,653 | 18,217 | 57,232 |
| pair B: `cam` + `lvl(L3150)` | 53,671 | 18,225 | 57,232 |
| **pair A + pair B (landed)** | **53,651** | **18,216** | **57,232** |

The two pairs are exactly additive on raw (-20 and -2) and both are
size-exact.  The individual aliases are not wins on their own; only the
size-compensating pairs are.  This is the same interaction class as
w11804's win #4 and w11906's deactivation alias.

## Re-checked / rejected

* w11906's deactivation `Level *lvl = m_pLevel;` (`L1480-1510`) is still a
  large win: removing it gives raw 55,085 and size 57,240.  Adding further
  aliases in that block regresses: `m_pLevel` alias for all three uses
  54,876; `Status *statusA` 54,873; `PlayerEgo *pl` 53,688 (w11906).
* The mission-42 state-7 `Player *pp = player->player;` alias
  (`L2192-2213`) is left block-scoped.  A `Status *statusA` alias in that
  block is size -12 (57,220) and raw 53,819; adding size-compensating
  aliases (`pp:682`, `m_pLevel:448`, `m_pCamera:121`, ...) did not beat
  53,651.
* `m_pLevel:381 + m_pLevel:590` is a size-exact raw 53,658 pair, but adding
  the landed pairs gives 53,656 (worse than 53,651).
* `m_pLevel:302 + m_pLevel:590 + cam + lvl(L3150)` is 53,652; the landed
  `geom5` variant is 53,651.
* Adding neutral aliases (`m_pCamera` L757/2656/2793, `m_pRadar` L4038,
  `m_pLevel` L1329, `Status` L2937, ...) to the landed pair leaves raw at
  53,651; none improves it.
* Adding a third size-compensating pair (`m_pCamera:302+m_pCamera:593`,
  `m_pLevel:381+m_pCamera:587`, `m_pLevel:609+m_pCamera:587`,
  `m_pCamera:465+m_pCamera:670`, `status_local:535+m_pCamera:591`) regresses
  raw to 53,658-53,683.

## Reproduction

```sh
ulimit -v 12000000
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 53651 aligned= 18216 size= 57232/57232 insn= 20729/20830
cmake --build cmake-build-match --target check -- -j2    # RATCHET PASS
cmake --build cmake-build-match --target drift -- -j2    # 0 fields
cmake --build cmake-build-match --target lint -- -j2     # CLEAN
```

Scratch harnesses: `parse_blocks.py`, `sweep.py`, `combo.py`,
`batch_combos.py`, `remove_alias.py`, `measure.py`; sweep data in
`sweep_*.json`, pair search in `combos_pairs4.log`, additive search in
`combos_clean*.log`.
