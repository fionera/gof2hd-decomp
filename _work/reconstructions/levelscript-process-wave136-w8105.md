# LevelScript::process — wave 136 / w8105 (4 getPlayer receiver fixes + faithful size companion)

Status: **code change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**. Owned TU:
`src/game/world/LevelScript.cpp`. Base master `2e51d765d` (wave-135 docs sync).

## Result

```
                         before (master)   after (landed)
official verify pct      58.8              57.1
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
process .text diffbytes  54316             55276            (+960)
.ARM.extab size          156312            156312           (unchanged)
.rodata/.data/.bss/.got  exact             exact
.rel.plt                 3674 entries      3674 entries, identical
byte_exact               4506              4506
linked_exact             4506              4506
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

The four `getPlayer` receiver fixes are semantically correct (verified against the
original disassembly) but re-roll clang's `MachineBlockPlacement`/register
colouring for the whole 57 KB body. They cost **+88 B** on the current base
(57320 B) and +960 differing bytes. This landing pairs them with a faithful
size companion that restores exactly 57232 B and keeps `.rel.plt` byte-identical.

## 1. The four receiver fixes (all re-verified against objdump)

| source line | before | after | original evidence |
|---|---|---|---|
| 947 | `m_pLevel->getPlayer()->rotate(0,angle,angle)` | `player->rotate(...)` | `0x137da6 ldr r0,[sp,#84]` |
| 1786 | `m_pLevel->getPlayer()->geometry->getPosition()` | `player->geometry->getPosition()` | preceded by `setTarget(player->geometry)` |
| 3247 | `if (m_pLevel->getPlayer()->isInRocketControl())` | `if (player->isInRocketControl())` | after `resetCamera(m_pLevel)` |
| 4336 | `if (m_pLevel->getPlayer()->isInRocketControl())` | `if (player->isInRocketControl())` | case-145 state 1 |

After the fixes the `Level::getPlayer` call count is **23/23** (was 27/23), i.e.
these four sites are exactly the extra calls w7749/w7766 identified.

## 2. The size budget on the *current* base (the numbers have drifted)

w7766's measurements were taken on the wave-134 base. On `2e51d765d` the same
levers measure differently. Measured on the pristine master (`57232`):

| edit | Δ size (current base) |
|---|---:|
| 4 getPlayer fixes | **+88** (57320) |
| rotate fix alone | +104 |
| campos / rocket1 / rocket2 alone | -8 each |
| remove whole `delta < 0` block | -64 |
| trim `delta < 0` to 1 statement | -40 |
| trim to 2 statements (keep setVisible + one more) | -40 |
| case-4 `StarSystem*` hoist | -8 |
| case-24 loop-2 un-cache | -8 |
| case-135 `(*enemies)[i]->setVisible` | -8 |
| case-16 state-3 landmarks cache (`lm4`) | -24 |
| case-16 loop un-cache (`enemies1718`) | -28 |
| `resetGunDelay` site-1 -> `player` | -16 |
| routePoints fold/unfold | 0 |

Note the anchor is **not** -80 B on this base: the whole block is -64 and the
1-statement trim is -40. The w7766 note's "-80/-88" is stale.

## 3. What the `delta < 0` anchor actually is

The anchor is a **duplicate** of five calls that already exist elsewhere in the
source. Call counts on master:

```
                 orig  ours
switchPlanetForIntro  1    2
switchSkyboxForIntro  1    2
PlayEngineSound       1    2
StopEngineSound       1    2
PlayerStation::setVisible 1 2
```

The real calls live at lines 823/824 (mission-0 case 3) and 867/869 (mission-0
case 9/11) and 2153 (mission 42). The anchor exists only to force their *first
reference* before `PlayerEgo::deleteHackingGame` in `.rel.plt` order. Trimming
the anchor to its single `PlayerStation::setVisible(false)` statement removes the
four redundant calls and keeps `.rel.plt` identical (the other four symbols are
first-referenced early by the real calls). Removing the block *entirely* moves
`PlayerStation::setVisible` past `deleteHackingGame` and breaks `.rel.plt` at
index 2835.

## 4. The landed combination

```
4 getPlayer fixes                          +88
delta<0 anchor trimmed to 1 statement      -40
case-4 StarSystem* hoist                    -8
case-16 state-3 landmarks cache (lm4)      -24
                                          ----
net                                        +16  ... measured 57232 (block-placement)
```

The exact landed size is **57232 B** and `.rel.plt` is byte-identical (3674
entries). The companion is deliberately *different* from w7766's: it replaces
w7766's case-24/135/resetGunDelay/loop-1 set with a single case-16 landmarks
cache (`lm4`, the mission-50/51 exit-wormhole block at lines 1680-1720), where
four `m_pLevel->getLandmarks()` calls in one block become one.

## 5. Why not a better pct?

The `rotate` fix is the expensive one: alone it is +104 B and drops the fuzzy
pct from 58.8 to 57.6. The other three fixes are -8 B each and only drop to
58.6. Every 57232-byte combination found re-rolls the block layout; the best
process diff at exactly 57232 B is 55276 (this landing). The best diff seen at
all was **54887 at 57228 B** (`anchor_trim2 + case4_hoist + case24_loop2 +
case135 + enemies1718`), i.e. 4 B short of the target; no faithful +4 B lever was
found to close it.

Full subset search over `{case4_hoist, case24_loop2, case135, enemies1718, lm4,
landmarks24, case16_loop2, rgd1, routepoints}` on the four-fix + anchor-trim
base produced exactly three 57232 B hits, all `.rel.plt`-exact:

| combo | pct | diffbytes |
|---|---:|---:|
| anchor_trim + case4_hoist + lm4 (landed) | 57.1 | 55276 |
| anchor_trim + case24_loop2 + lm4 | 57.0 | 55311 |
| anchor_trim + case135 + lm4 | 57.1 | 55299 |

## 6. Negative results (do not re-run)

* `anchor_remove` (whole block) always breaks `.rel.plt` at index 2835.
* `autoturret_remove` breaks `.rel.plt` at index 2821; `standing_remove` at 2816.
* `rgd_all` (all 10 `resetGunDelay` sites -> `player`) is **unfaithful**: the
  original calls `getPlayer()` at every `resetGunDelay` site
  (`ldr r0,[rX,#24]; blx getPlayer; blx resetGunDelay`), so it drops the
  `getPlayer` count to 13/23.
* `lm4` does not change the `Level::getLandmarks` call count (still 17/15) —
  clang re-materialises calls; it is a pure size/layout lever.
* `routePoints` fold <-> unfold is 0 B on this base (the literal pool has slack).

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
cmake --build cmake-build-match --target drift -j6      # 0 fields
```

Scratch helpers in `/dev/shm/w8105/` (not tracked): `score2.py` (pct + size +
diffbytes), `variant.py` (edit harness), `callcount.py`, `callseq.py`,
`relplt.py` (readelf `.rel.plt` comparator), `search.py`/`search2.py` (subset
sweeps), `LevelScript.base.cpp` (pristine snapshot).

## Open leads

1. The 57228 B / 54887-diff combination is 4 B short; a faithful +4 B lever
   would beat this landing by ~390 diff bytes.
2. The anchor is a duplicate of five real calls. If the mission-0 case-3/9/11
   blocks could be made to be first-referenced before `deleteHackingGame` without
   the anchor (source block ordering), the anchor could be removed entirely
   (-40 B) and the call counts would become 1/1.
