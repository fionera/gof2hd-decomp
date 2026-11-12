# MGame::OnTouchEnd — wave 190 (w13425): campaign-chain condition operand swaps

Target: `_ZN5MGame10OnTouchEndEiiPv` in `src/game/menu/MGame.cpp` (original 8,080 B
@ `0x17a144`, Thumb, NDK r18b clang 7.0.2 `-Oz`). Worktree `wt/w13425`, branch
`wave190-w13425`, base master `32b826e31`. Scratch `/home/debian/GalaxyOnFire2/scratch/w13425`.

Prior notes: `mgame-ontouchend-wave190-w13374.md`, `-w13378.md`, `-w13392.md`,
`-w13401.md`, `-w13405.md`, `-w13421.md` (chain order, getter permutations,
`__builtin_expect` sweeps, cold-block shapes, string-schedule probes).

## Result — first raw win on this function in wave 190

| metric | base | after |
|---|---:|---:|
| raw same-address diff bytes | 2,913 | **2,898** |
| aligned (instruction) diff | 1,706 | 1,714 |
| function size | 8,080 / 8,080 | 8,080 / 8,080 |
| `.text` differing bytes | 72,922 | **72,907** |
| `.ARM.extab` size / diff | 156,312 / 603 | 156,312 / 603 |
| `.rel.plt` diff | 0 | 0 (byte-identical) |
| file size | 2,187,328 | 2,187,328 |
| total (text+extab+build-id) | 73,545 | **73,530** |

Gate (`check`, `RATCHET PASS`): byte_exact **4507/4524**, linked_exact
**4507/4524**, size-exact **4524/4524**, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift **0**, parity **5370/5370**, 27/27 headers.

## Win — swap `isCampaignMission()` behind the mission-id test (raw 2,913 -> 2,898)

The campaign `else if` chain tested `mission->isCampaignMission()` *first* and the
`Globals::status->getCurrentCampaignMission() == N` literal *second*.  Swapping the
two operands of `&&` (behaviour-preserving: both are side-effect-free predicates,
and `&&` short-circuits either way) for eight chain arms lets the scheduler hoist
the mission-id compare/cold branch and re-uses the already-loaded id register,
removing 15 bytes of same-address diff.  Swapped arms:

* `== 81`, `== 74`, `== 15`, `== 43`, `== 134`, `== 65`;
* the `(== 96 || == 127)` arm (whole parenthesised id test first);
* `== 162`, keeping `__builtin_expect(mission->isCampaignMission(), 1)` on the
  second operand (`// lint: arm_builtin` waiver retained).

Marginal values measured from the plain base (each row adds one more swap):

| swap added | raw |
|---|---:|
| 81 | 2,909 |
| +74 | 2,907 |
| +15 | 2,905 |
| +43 | 2,903 |
| +134 | 2,901 |
| +65 | 2,900 |
| +96/127 | 2,899 |
| +162 | **2,898** |

No further swap in this family helps: 42, 110, 120, 126, 155, 95, 144, 161 are
neutral or worse, and 100 is +102 (raw 3,000).  This is the local optimum of the
operand-swap family; the aligned metric rises 1,706 -> 1,714, which is irrelevant
to the raw gate.

Reproduce with `scratch/w13425/apply_win.py` (applies the eight replacements to
`MGame.cpp.base`) then `lib.build()` + `measure.py`.
