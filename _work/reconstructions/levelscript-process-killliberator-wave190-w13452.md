# LevelScript::process — wave 190 / w13452: killLiberator before setRocketControl

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13452`, branch
`wave190-w13452`, base master `ec8838768` (raw **51,714**, aligned **21,911**,
`.text` 72,385, `.rel.plt` md5 `289b2a069f8504d1e8a3919e43915e04`).

Lens: literal-pool ordering / branch-target-cold-block layout / prologue
register-allocation lever.  The prologue and first-island levers were
re-measured on the current body (all size-changing or worse; see below).  A
fresh full adjacent-statement (`d=1`) sweep on the current 51,714 body found
one new size-exact, `.rel.plt`-clean raw win.

## Landed change

Source lines 3526-3527 (rocket-control teardown):

```cpp
-                            player->setRocketControl(nullptr, nullptr);
-                            player->killLiberator();
+                            player->killLiberator();
+                            player->setRocketControl(nullptr, nullptr);
```

`killLiberator` reads `player->player->guns` and `currentSecondaryWeaponIndex`
and writes gun lifetimes/positions/active flags; `setRocketControl(nullptr, …)`
writes `rocketControlGun`, calls `ParticleSystemManager::systemSetMatrix` on
the player transform and clears `rocketBanking`.  The two touch disjoint state,
so the reorder is semantics-preserving.

Measured alone on the real build:

```
raw        51,714 -> 51,712  (-2)
aligned    21,911 -> 21,911
size       57,232 / 57,232   (exact)
.text      72,385 -> 72,383  (-2)
.ARM.extab 584 (unchanged; size/base exact 156,312 B @ 0x1a3b78)
.rel.plt   md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical)
```

`check` -> RATCHET PASS (byte_exact 4507, linked 4507, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370).

## Fresh d=1 sweep

Full adjacent simple-statement sweep over the current body (1,722 candidates,
exact NDK r18b `-Oz` compile + gold link, 4 workers): three size-exact cells
beat the base.  Two are read-after-write rotations and were rejected:

* source 5230 `m_nScriptTimerA += delta;` across `float approach = (float)
  m_nScriptTimerA / -34000.0f + 1.0f;` (approach reads the timer) — raw 51,711.
* source 3736 `*tmp = ref2->getUpVector();` across `*camPos += *tmp * 400.0f;`
  (`*tmp` read-after-write) — raw 51,713.

The landed cell is the only semantically independent one; it is `-2` raw
(51,712), not the `-1` the raw cell list printed for the other two.

## Prologue / first-island re-measurements (all non-wins)

* `Status *status` local removed (`Globals::status->` everywhere): size exact,
  raw **51,715** (+1); prologue still `this=fp, canary=sl, &status=sb`.
* `Level *level = m_pLevel` (all 296 uses) or `Level &level = *m_pLevel`:
  `this=sl` (matches the original) but the frame grows 0x1b0 -> 0x1b8, size
  **57,216** (-16) and raw **55,190** (+3,476) — the +8 frame shift moves every
  `[sp,#…]` access.
* `Status *volatile status`: size **57,240** (+8), raw 55,063.
* `else if (mission == 5)` -> separate `if`: size **57,396** (+164), pool moves
  to 0x1380a0, `.rel.plt` changes to `9568f1e6…`.
* separate `if` + `__builtin_expect(m_bStartSequence != 0, 1)`: size
  **57,336** (+104), raw 55,130, `.rel.plt` still changed.
* first wingman loop `__builtin_expect(isWingMan(), 0)` -> `1`: size exact,
  raw **51,804** (+90) — confirms the w13408 finding.

## First literal island

Ours at `0x1380c0` (25 words), original at `0x1380a4` (27 words), +0x1c.  Words
0..21 are identical; the differences are the 4-byte `double 0.1` alignment pad
(`bf00bf00`, present only when the island starts 4 mod 8) and the last word
(original `30000.0` from the mission-5 block, ours `10000.0` from the
mission-81 rumble divide).  The island boundary follows the post-pool block
order (original lays the mission-5 block before the intro mission-81 block;
ours the reverse), not a wrong constant.
