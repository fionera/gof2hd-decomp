# LevelScript::process — camera-offset swap, wave191 (w13480)

Target: `_ZN11LevelScript7processEi` (src/game/world/LevelScript.cpp), size 57,232.

## Change
Mission-89 supernova camera setup (~source line 3861): move
`*camPos += offset;` before `offset *= 1000.0f;` for the right-vector term.

Found by the comment-aware rotation sweep (N=2..6) on top of commit 817a41453:
cell `c2_119` (both directions agree).

## Measurement
- before: raw 51,654, text 71,786
- after:  raw 51,653 (-1), text 71,785 (-1), size 57,232/57,232 exact
- `.rel.plt` md5 unchanged: 289b2a069f8504d1e8a3919e43915e04
- `check`: RATCHET PASS (byte_exact 4507/4524, linked 4507, data 843/843,
  imports 207, lint CLEAN, drift 0, parity 5370/5370)
