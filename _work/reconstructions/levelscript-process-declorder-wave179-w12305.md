# LevelScript::process — wave 179 / w12305: declaration-order sweep (raw 53,082 -> 53,061)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w12305`, branch
`wave179-w12305`, base master `64014ce6f` (wave-178 merge).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w12305`.

## Result (landed)

```
                         before (base)     after (landed)
process raw diffbytes    53,082            53,061            (-21)
process aligned insns    20,149            20,165            (+16)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
orig / ours insns        20,830 / 20,728   20,830 / 20,728
.text section diff       77,936            77,915            (-21)
.ARM.extab diff          700               700               (size/base exact)
.rel.plt                 29,392 B ident    29,392 B ident    (md5 289b2a069f8504d1e8a3919e43915e04)
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
symbol parity            5370/5370         5370/5370
RATCHET                  PASS              PASS
```

`raw` is the same-address byte diff over the function body (literal pools
included), from `tools/fn_diff.py`; it is the primary metric.  Total differing
section bytes fall 78,656 -> 78,634 (`.text` -21, `.note.gnu.build-id` -1).

## The landed source change

Ten semantics-preserving declaration moves (9 line swaps) inside
`LevelScript::process`.  Every moved declaration is a pure constant, a pure
getter/array load, or a `m_pLevel` alias; no RNG call order and no mutating
call order changes.  The moves are:

1. **~1596** — `Vector zero{0,0,0}` moves *after* `player->setTurretMode(false)`.
2. **~1880** — `Vector tmp{0,0,0}` moves *before* the two `*frame` assignments
   (`*frame = ...getDirection()`, `*frame *= 25000.0f`).
3. **~2265** — `Vector camPos = (*enemies)[0]->getPosition()` moves *before*
   `m_pCamera->setActive(true)`.
4. **~3157** — `Level *lvl = m_pLevel` moves *after* `Vector pos` and
   `Vector zero` (it is only used much later in `resetCamera(lvl)`).
5. **~3486** — `int dx = rnd->nextInt(10000)+35000` moves *before*
   `(*enemies)[i]->awake()`, and `KIPlayer *fighter = (*enemies)[i]` moves
   *after* `int dz = rnd->nextInt(10000)+35000`.  RNG order stays
   `10000, 2, 10000, 10000, 2`.
6. **~3799** — `int rx` and `int rz` move *before*
   `(*enemies)[i]->revive()`, and `revive()` moves *before* `int ry`.
   RNG order stays `1000, 1000, 1000`.
7. **~3869** — `Vector camPos = *wcamera->getPosition()` moves *before*
   `wcamera->setTarget(marker)`.
8. **~3988** — `Vector playerPos = player->getPosition()` moves *before*
   `(*enemies)[i]->awake()`.

The changes were found by exhaustive one-line declaration-move sweeps (see
below), not by analogy.  Each move was measured individually and in
combination; only the combined size-exact, `.rel.plt`-identical tree is landed.

## Method

A local harness (`scratch/w12305/drive.py`, `drive_multi.py`) applies exact
string replacements to a saved base file, rebuilds the single changed TU and
the real gold link (`cmake --build cmake-build-match --target link_so -- -j2`,
`ulimit -v 12000000`), then measures `raw` with `tools/fn_diff.py`, the function
size from `.dynsym`, and the `.rel.plt` md5.  A full build of the changed TU
plus link is ~1.5 s when the box is idle.

### Sweep 1 — top-level declaration permutations (base 53,082)

All 23 non-base permutations of `messages, player, enemies, status`:
base is best; next best `enemies, messages, player, status` = 53,086.  `const`
on any of the four (and on all four) is neutral at 53,082.  Dropping the local
`status` is size-exact but +1 raw on the wave-178 base (the wave-177 result
reproduces).

### Sweep 2 — adjacent declaration swaps and one-line moves (base 53,082)

- 70 adjacent same-indent declaration swaps (dependency-checked): no win; the
  best is neutral at 53,082.
- 224 one-line declaration moves (down past the next same-indent line / up past
  the previous same-indent line): six wins, all size-exact:
  `up_1880` -3, `down_1596` -2, `up_842` -1 (rejected: `rumble` depends on the
  preceding `m_nScriptTimerA += delta`), `up_2266` -1, `up_3870` -1,
  `up_3989` -1.  The best 7-change combination is **53,071**.
- Full permutations of every 3–6 declaration block whose RNG call order can be
  preserved: `b3157_120` -1 and `b3486_023145` -2 are the only wins.  The
  `b2406` pirate-position permutations are all >= 53,083.

### Sweep 3 — repeat on the 53,071 tree

294 one-line moves and swaps on the post-win tree: three more wins, all
size-exact: `up_1879` -2, `up_3486` -2, `up_3799` -2.  Combined tree
**53,065**.

### Sweep 4 — repeat on the 53,065 tree

127 up-moves: one more win, `up_3800` -4 (`int rz` before `revive()`), giving
the landed **53,061**.  A second variant that also moves `ry` before `revive()`
is size-inexact (57,240) and was rejected.

## Negative results (all size-exact and `.rel.plt`-identical unless noted)

- Inlining the one-use `AbyssEngine::PaintCanvas *canvas = Globals::Canvas;`
  temporary in all 17 `Matrix local` / `Vector dir` blocks: neutral at 53,082
  (or +3/+7), no win.
- `Vector`/`Matrix` adjacent swaps: `camPos`/`dir` swaps in the case-40 blocks
  are size-inexact (57,240); `pos`/`zero` swaps are neutral.
- `b3157_120` is a pure alias move; `b3157` permutations that put `lvl` first
  are worse (53,123..53,125).
- `b3486` permutations that move `fighter` elsewhere are 53,083..53,099.
- `up_842` (-1) is rejected on semantics: the original computes
  `rumble = (float)(m_nScriptTimerA + delta) / 2000.0f` and stores the
  incremented timer (`0x138f08`: `ldrd` -> `adds/adc` -> `__aeabi_l2f` ->
  `vdiv` -> `strd`), so the declaration must stay after the increment.

## Residual

The dominant residual is unchanged from w12206: the whole-function
callee-saved swap (`this` in `fp` vs the original `sl`, `&__stack_chk_guard`
in `sl` vs the original `fp`) and the first literal-pool offset (+28 B).
Declaration-order moves are now exhausted at the one-line-neighbour level;
further gains need a block-layout/register-assignment change, not a local
declaration move.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12305
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw=53061 aligned=20165 size=57232/57232 insn=20728/20830
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=53061 size=57232/57232 text=77915 relplt=289b2a069f8504d1e8a3919e43915e04
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
cmake --build cmake-build-match --target drift -- -j2   # 0 fields
cmake --build cmake-build-match --target lint -- -j2    # CLEAN
```

The sweep harnesses and result tables live in
`/home/debian/GalaxyOnFire2/scratch/w12305/` (`drive.py`, `drive_multi.py`,
`gen_moves.py`, `spec_*.json`, `out_*.json`, `base*.cpp`); they are scratch,
not tracked.
