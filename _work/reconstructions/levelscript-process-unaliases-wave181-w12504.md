# LevelScript::process — wave 181 / w12504: four alias removals (raw 52,787 -> 52,776)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree
`/home/debian/GalaxyOnFire2/wt/w12504`, branch `wave181-w12504`, base master
`1e270aa0e` (wave-180 harvest).  Scratch `/home/debian/GalaxyOnFire2/scratch/w12504`.

Follow-up to the same-wave relaxed declaration-move pass
(`levelscript-process-relaxed-declmove-wave181-w12504.md`, raw 52,795 ->
52,787).  The alias sweeps of waves 167-179 only ever *added* block-scope
receiver aliases; this pass measures the inverse on the current tree: remove an
existing alias, replace every use in its block with the aliased expression, and
delete the declaration.

## Result (code win landed)

```
                         base (HEAD)      this pass
process raw diffbytes    52,787           52,776            (-11)
process aligned insns    20,243           20,124            (-119)
orig / ours size         57232 / 57232    57232 / 57232     (size-exact)
orig / ours insns        20,830 / 20,731  20,830 / 20,728
.text diffbytes          76,169           76,158            (-11)
.ARM.extab diffbytes     713              713               (size/base exact)
.ARM.exidx diffbytes     0                0
.note.gnu.build-id       20               20
total differing bytes    76,902           76,891            (-11)
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
included), from `tools/fn_diff.py`; it is the primary metric.  Size,
`.rel.plt`, `.ARM.extab` and `.ARM.exidx` are unchanged, so the -11 `.text`
bytes are a net win.

## The landed source change

Four receiver aliases removed; every use now names the aliased expression
directly.  All four expressions are pure member / getter loads of a value that
cannot change inside the block, so the generated value is identical.

| # | base line | removed alias | uses rewritten | raw |
|---:|---:|---|---:|---:|
| 1 | 806 | `TargetFollowCamera *wcamera = m_pCamera;` (case 8) | `wcamera->setRumblePercentage` ×2 | 52,783 |
| 2 | 5130 | `TargetFollowCamera *wcamera = m_pCamera;` (`m_nState == 11`) | `wcamera->translate`, `wcamera->setTarget` | 52,784 |
| 3 | 3909 | `AEGeometry *geo = player->geometry;` | `geo->getDirection`, `geo->setDirection` | 52,785 |
| 4 | 4378 | `Level *self = m_pLevel;` (case 1) | `resetCamera(self)`, `self->getEnemies()` | 52,785 |
| all four | | | | **52,776** |

Each removal was measured individually and in combination; the four are
strictly additive (-4, -3, -2, -2).

### Why the removals are semantics-preserving

1-2. `m_pCamera` is a `LevelScript` member pointer, not modified between the
declaration and its uses; `wcamera` was a pure copy of it.
3. `player->geometry` is a stable `PlayerEgo` member pointer; `geo` was a pure
copy.  `getDirection()` is a pure getter and `setDirection()` is called on the
same object.
4. `m_pLevel` is a `LevelScript` member pointer, not modified in the block;
`self` was a pure copy.  `getEnemies()` is a pure getter.

## Method

`unalias.py` walks the clang `-ast-dump` of `process`, collects every
pointer-typed `VarDecl` with a single declarator and a non-`new` initializer
(178 candidates), records its enclosing `CompoundStmt` line range, and
`run_unalias.py` rewrites every word-boundary use of the alias inside that
range to the initializer expression, deletes the declaration, rebuilds the TU
with the exact `compile_commands.json` clang command, gold-links with the exact
`build.ninja` command, and measures the same-address raw diff, function size and
`.rel.plt` md5.  `combo_unalias.py` then measures all 15 non-empty subsets of
the four winners; the full set is the best.  Extra parentheses produced by the
mechanical substitution were removed by hand (parentheses do not affect
codegen); the cleaned tree was re-measured at the same 52,776.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12504
ulimit -v 12000000
ninja -C cmake-build-match libgof2hdaa.so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw=52776 aligned=20124 size=57232/57232 insn=20728/20830
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=52776 size=57232/57232 text=76158 relplt=289b2a069f8504d1e8a3919e43915e04
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
cmake --build cmake-build-match --target lint  -- -j2   # CLEAN
cmake --build cmake-build-match --target drift -- -j2   # 0 fields
```

Harnesses (`unalias.py`, `run_unalias.py`, `combo_unalias.py`,
`apply_unalias.py`, `unalias.json`, `unalias_results.tsv`) are scratch, not
tracked.
