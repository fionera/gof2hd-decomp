# LevelScript::process — wave 190 / w13442: mission-4-ish state exit `m_bCinematicBreak` sink

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13442`, branch
`wave190-w13442`, base master `f0f4d7918` (raw **51,714**, `.text` 72,849,
`.rel.plt` md5 `289b2a069f8504d1e8a3919e43915e04`).

A fresh full `d=1` sweep (1,530 unique pairs) on the current body found only the
five already-documented `-1` cells (four read-after-write and the `.rel.plt`-fatal
mission-0 state-9 family), so a fresh full `d=2` rotation sweep (2,055 candidates,
exact NDK r18b `-Oz` compile + gold link, 4 workers) was run.  Four size-exact
`-1` cells were found; three are read-after-write `m_tempVec` rotations.  The one
safe cell is landed here.

## Landed change

Source lines 3536/3538 (mission-4-ish state exit, `setFreeze` block):

```cpp
-                        m_bCinematicBreak = 1;
                         m_pHud->visible = 0;
                         player->setFreeLookMode(false);
+                        m_bCinematicBreak = 1;
```

The moved statement is a `this` flag store; its neighbours are a `Hud` member
store and a `PlayerEgo::setFreeLookMode` call on a disjoint object, so the
rotation is semantics-preserving.  The original emits `m_bCinematicBreak` after
the player setters (`0x143a82`, after `setFreeLookMode`), so the direction is
also faithful.

Measured alone on the real build:

```
raw        51,714 -> 51,713  (-1)
size       57,232 / 57,232   (exact)
.text      72,849 -> 72,848  (-1)
.ARM.extab 597 (unchanged; size/base exact 156,312 B @ 0x1a3b78)
.rel.plt   md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical, 29,392 B)
```

`check` -> RATCHET PASS (byte_exact 4507, linked 4507, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370).

## Rejected cells (same sweep)

| move (0-based) | source lines | reason |
|---|---|---|
| `up 1727,1729` (raw 51,712) | 1730 before 1728 | `setPosition(camPos)` before the `camPos += …` producer — wrong position |
| `up 827,829` (raw 51,713) | 830 before 828 | moves `particleRenderBoolPtr->reset()` across the first `switchPlanetForIntro` reference — `.rel.plt` order |
| `up 888,890` (raw 51,713) | 891 before 889 | `m_tempVec += …` before `m_tempVec = player->getPosition()` — RAW |
| `up 4541,4543` (raw 51,713) | 4544 before 4542 | `setPosition(m_tempVec)` before the `m_tempVec = …` producer — RAW |
| `up 4603,4605` (raw 51,713) | 4606 before 4604 | `MatrixSetTranslation(..., m_tempVec)` before the `m_tempVec += …` producer — RAW |
| `down 4963,4965` (raw 51,713) | 4964 after 4966 | `m_tempVec += …` after the `setPosition(m_tempVec)` reader — RAW |

## Other levers tried (no size-exact win)

* Flipping every one of the 68 `__builtin_expect` probabilities in `process`
  (single flip each): no flip beats 51,714; the smallest non-neutral is 51,716
  (line 2240).
* `__builtin_expect` on the `m_bStartSequence != 0` guard (0/1, `==`/`!=`):
  all change the function size (+160..+284).
* A `LevelScript *lsp = this;` alias used for every member access compiles to
  byte-identical code (raw 51,714).
* Changing the pre-switch `else if (… == 5)` to a separate `if` (which makes the
  first literal pool match the original's 27 words, including `5000.0`/`30000.0`,
  and puts the wingman-reset loop body after the pool) is semantically faithful
  but grows the function to 57,396 B (+164), so it is not gate-safe yet.
