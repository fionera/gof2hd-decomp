# LevelScript::process — wave 136 / w8208 (kill the `.rel.plt` anchor by source shape)

Status: **code change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**. Owned TU:
`src/game/world/LevelScript.cpp`. Base master `655e3bcc5` (wave-136 docs head, 58.9%).

## Result

```
                         before (master)   after (landed)
official verify pct      58.9              58.9
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
process .text diffbytes  54327             54326            (-1)
whole .text diffbytes    84709             84708            (-1)
.ARM.extab size          156312            156312           (unchanged)
.rodata/.data/.bss        exact             exact
.rel.plt                 3674 entries      3674 entries, byte-identical (3674/3674 names)
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

## The hypothesis (new this wave)

The `if (delta < 0)` "first-reference anchor" in mission-125 case 3 was a
duplicate of real calls and existed only to rotate `.rel.plt`.  On the current
base it had already gone stale: **removing the whole block changed nothing in
`.rel.plt`** except the one `PlayerStation::setVisible` / `PlayerEgo::deleteHackingGame`
swap (measured: 57232 -> 57200, names 3672/3674).  So the anchor's three calls
(`switchPlanetForIntro`, `PlayEngineSound`, `StopEngineSound`) were pure waste.

The real remaining `.rel.plt` problem was:

```
orig 2835 PlayerStation::setVisible      ours 2835 PlayerEgo::deleteHackingGame
orig 2836 PlayerEgo::deleteHackingGame   ours 2836 PlayerStation::setVisible
```

`PlayerStation::setVisible` is first referenced exactly once in the original,
by the **real** mission-42 state-7 statement (`0x13da1e`, inside
`Transform::currentTime >= 4001`).  In the original that block is laid out at
`0x13d8fc`, *before* the mission-125 hacking default block (`0x13de44`); in our
build it landed at `0x13e56c`, *after* it (`0x13dcda`).

## The fix: reorder the mission-42 state tests

The mission-42 state chain was `state 0 / state 6 / state 7`.  Reordering the
three mutually-exclusive `else if` tests to **`state 6 / state 0 / state 7`**
changes clang's block placement so the state-7 block is emitted before the
mission-125 default block.  Measured on the pristine base:

| mission-42 state order | size | `.rel.plt` names matching |
|---|---:|---:|
| 0, 6, 7 (base) | 57232 | 3672 |
| **6, 0, 7 (landed)** | **57248** | **3674** |
| 0, 7, 6 | 57232 | 3665 |
| 6, 7, 0 | 57228 | 3672 |
| 7, 0, 6 | 57244 | 3662 |
| 7, 6, 0 | 57240 | 3665 |

The branches are mutually exclusive, so the reorder is semantics-preserving.
With `6,0,7` the real `setVisible` is first referenced at `.rel.plt` 2835 and
`deleteHackingGame` at 2836 — **identical to the original with zero duplicate
calls**.  Removing the whole `delta < 0` anchor then costs -32 B (57248 ->
57216) and leaves `.rel.plt` identical.

## Where the freed bytes went: case-24 state-1 landmark un-cache (+16 B)

The original calls `Level::getLandmarks()` **three times** in mission-24
state 1 (`0x13a96a`, `0x13a978`, `0x13a988` — once each for `setVisible`,
`reset(false)` and `open`).  Our source cached it in a local:

```cpp
Array<KIPlayer *> *landmarks = m_pLevel->getLandmarks();
(*landmarks)[3]->setVisible(true);
((PlayerWormHole *) (*landmarks)[3])->reset(false);
player->setAutoTurret(player->autoTurretIsEnabled());
((PlayerWormHole *) (*landmarks)[3])->open();
```

Un-caching it (`(*m_pLevel->getLandmarks())[3]->...` at each site) is faithful
and measures **+16 B** in isolation (57232 -> 57248, `.rel.plt` unchanged).
It exactly closes the 57216 -> 57232 gap.

## Landed combination

```
reorder mission-42 state tests to 6,0,7     +16
remove the delta<0 anchor block             -32
un-cache case-24 state-1 getLandmarks()     +16
                                            ----
net                                           0   -> 57232
```

Verified: `.rel.plt` readelf name list is 3674/3674 identical (all `r_offset`
and `r_info` also identical, `same-name/info-diff 0`).

## Call-count / faithfulness notes

* The mission-125 case-3 block no longer contains any call; the stale
  "first-reference anchor" comment was deleted.
* `Level::getLandmarks` call count moves closer to the original (the original
  really does re-fetch it at the three state-1 sites).
* `PlayerStation::setVisible` is now first referenced by its one real call.

## Negative results (do not re-run)

* Removing the anchor **alone** does not fix `.rel.plt` (still 3672/3674).
* The case-24 state-1 un-cache **alone** does not fix `.rel.plt` (3672/3674).
* The `6,0,7` reorder is the only one of the six permutations that yields
  3674/3674 matching names.
* Adding the w8106 case-102 state-0/1 `setTarget`/`translate` move on top of
  this landing re-rolls the whole body: size 57224, process diffbytes 54918
  (much worse).  It needs its own companion.
* Swapping the `if (m_bRenderParticles)` arms (negated condition) is a no-op
  for `.rel.plt` and size (57232, 3672/3674).

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
cmake --build cmake-build-match --target drift -j6      # 0 fields
```

Scratch helpers in `/dev/shm/w8208/` (not tracked): `sz.sh` (build + symbol
size + `.rel.plt` compare), `relcmp.py` (readelf `.rel.plt` name diff),
`diffbytes.py` (raw `LevelScript::process` byte diff), `perm.py` (mission-42
state-order permutation generator), `LevelScript.base.cpp` (pristine snapshot).

## Open leads

1. The mission-42 case body is still missing states 1–4 (the original has
   `0x13bf86` / `0x13cf28` blocks: `RadioMessage[4]` trigger → `m_nState = 4`,
   then `m_nState == 4 && RadioMessage[4]->isOver()` → `setFixed(true)`,
   `setAIDisabled(false)`, `setHitpoints(0)`, `m_nState = 5`).  Adding them is
   the biggest remaining semantic gap in this case, but it is >16 B.
2. The w8106 case-102 state-0/1 `setTarget`/`translate` move is still correct
   but needs a fresh block-placement companion on this base.
