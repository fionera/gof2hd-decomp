# LevelScript::process — wave 166 / w11101: PLT call histogram + 0.8f hoist sizing

Target `_ZN11LevelScript7processEi` (original `0x137d18`, 57,232 B = 0xdf90),
size-exact.  Worktree `wt/w11101`, branch `wave166-w11101`, base master
`85c1f88df`.  Scratch `/dev/shm/w11101/`.

## Baseline (HEAD, unchanged)

```
process raw diffbytes    54,247
process aligned insns    17,532
orig / ours size         57232 / 57232   (size-exact)
.rel.plt                 0        (BYTE-IDENTICAL)
byte_exact 4507 / linked_exact 4507 / size 4524 / data 843 / imports 207
```

## Call histogram — pinned with a script

`/dev/shm/w11101/callseq.py` extracts every `bl`/`blx` (direct + register)
from both ELFs, resolves the target name, and diffs the two sequences.
Totals: **original 2,900 calls, ours 2,906** (+6 net).

`/dev/shm/w11101/ctx.py` then builds, for every call of a given callee, the
multiset of `(previous call, next call)` pairs on each side and diffs the
multisets.  This is robust to block reordering (which destroys a plain
sequence diff in this 2,900-call function).  `/dev/shm/w11101/window.py`
does the same with a ±5-instruction normalised window; it is noisier because
register allocation differs almost everywhere.

The per-callee deltas reproduce the wave-164 w11003 numbers exactly:

| callee | orig | ours | delta | kind |
|---|---:|---:|---:|---|
| `AEGeometry::moveForward(float)` | 40 | 41 | +1 | source shape |
| `AEGeometry::setPosition(const Vector&)` | 23 | 24 | +1 | source shape |
| `TargetFollowCamera::setPosition(float,float,float)` | 8 | 9 | +1 | source shape |
| `Level::getStarSystem()` | 15 | 16 | +1 | PLT anchor (see below) |
| `Level::getPlayer()` | 23 | 24 | +1 | source shape |
| `PlayerEgo::getPosition()` | 41 | 42 | +1 | source shape |
| `Status::getStanding()` | 1 | 2 | +1 | deliberate PLT anchor (:2023) |
| `Standing::setStanding(int,int)` | 1 | 2 | +1 | deliberate PLT anchor (:2023) |
| `PlayerEgo::setAutoTurret(bool)` | 2 | 3 | +1 | deliberate PLT anchor (:1818) |
| `PlayerEgo::autoTurretIsEnabled()` | 1 | 2 | +1 | deliberate PLT anchor (:1818) |
| `LODManager::forceUpdate(int,bool)` | 66 | 65 | -1 | source shape |
| `TargetFollowCamera::setPosition(const Vector&)` | 59 | 58 | -1 | source shape |

The three "PLT anchor" pairs are load-bearing: removing them rotates
`.rel.plt` (18 B for the auto-turret pair, 26 B for the Standing pair), so
they cannot be removed without a `.rel.plt`-order compensation.

Note that the `getStarSystem` +1 is **not** a plain duplicate either: the
original's single `switchSunForSupernovaIntro` call lives in the mission-89
case-3 arm, while ours is emitted in the mission-125 state-6 wormhole block
(`LevelScript.cpp:2139`, comment at :2133).  The total `getStarSystem` count
is +1 because the re-sited call is an extra fetch; moving it back is size
+8 and raw +145 (wave-165 w11105).

## The six "genuine duplicate" sites are not literal double calls

A basic-block census (`/dev/shm/w11101/blocks.py`) shows that in every block
where ours has two calls of one of the six callees, the original also has
two; and every high-similarity block pair (difflib ratio > 0.9 on the call
name sequence) has equal counts.  The +1 deltas therefore come from
*different source shapes* in reordered blocks, not from a call written twice
in one place.  The only concrete redundant pair found in the source is:

* `m_pLevel->getPlayer()->getPosition()` at `LevelScript.cpp:2877` and
  `:4496` — a `getPlayer` immediately followed by `getPosition`, where the
  cached `player` local is available.

Measured (full link, `tools/fn_diff.py`):

| change | raw | aligned | size |
|---|---:|---:|---:|
| HEAD | **54,247** | 17,532 | 57,232 |
| `:2877` → `player->getPosition()` | 54,256 (+9) | 17,531 | **57,232** (unchanged) |
| `:4496` → `player->getPosition()` | 55,549 (+1,302) | 18,501 | 57,368 (+136) |
| both `m_pLevel->getPlayer()->setCollide(false)` → `player->` | 54,871 (+624) | 17,525 | 57,224 (-8) |
| both `m_pLevel->getPlayer()->setCollide(true)` → `player->` | 54,529 (+282) | 17,511 | 57,224 (-8) |

Each removed `getPlayer` call is exactly -4 B (the `bl`), confirming the
24 B accounting for six duplicates.  But none of these replacements improves
raw: the original really does re-fetch `m_pLevel->getPlayer()` at those
sites, so the "duplicate" is elsewhere and was not pinned this pass.

## 0.8f hoist sizing on the current base (wave-166)

The wave-164 w11003 hoist (`int st78 = m_nState; if (st78 >= 5 && st78 <= 8)
m_pCamera->translate(-delta * 0.8f, ...)` at the top of the mission-0x4e
arm, deleting the unreachable post-switch copy) is **larger on this base**
than the +48 B recorded on `b3d3a1f68`, because wave-164's w11002
`__builtin_expect` sweep changed the block layout:

| shape | raw | aligned | size |
|---|---:|---:|---:|
| HEAD | **54,247** | 17,532 | 57,232 |
| hoist, `st78`, state-3 kept | 55,342 | 17,551 | 57,272 (+40) |
| hoist, no `st78`, state-3 kept | (57,280 by arithmetic) | — | 57,280 (+48) |
| state-3 translate removed only | 54,997 | 17,559 | 57,184 (-48) |
| hoist `st78` + state-3 removed | 54,830 | 17,567 | 57,224 (-8) |
| **hoist no `st78` + state-3 removed (after gate)** | 54,598 | 17,581 | **57,232 exact** |
| **hoist no `st78` + state-3 removed (before gate)** | 54,594 | 17,579 | **57,232 exact** |

So the `no st78` hoist is +48 B (not +40), which is exactly the size the
task's accounting assumes: six duplicates (-24 B) plus ~24 B of other extra
code would make `hoist + state-3 kept` size-exact.  On this base the hoist
alone is a **raw regression of ~1,095 bytes**, and even the size-exact
`hoist + state-3 removed` pair is raw +347/+351 — so neither was landed.

## Landed

Nothing (source reverted; tree clean).  The base shape (raw 54,247) is still
the best measured shape, and every hoist variant measured this pass is a
raw regression.  The remaining lever is unchanged: pin the six +1 call sites
by matching the original's block call sequences, remove 24 B, find a further
24 B, and only then re-apply the `no st78` hoist with the state-3 translate
kept.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
python3 /dev/shm/w11101/callseq.py > callseq.txt   # 2,900 vs 2,906 calls
python3 /dev/shm/w11101/ctx.py 'AEGeometry11moveForwardEf' ...  # context multiset diff
```
