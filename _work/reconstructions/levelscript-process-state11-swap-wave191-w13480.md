# LevelScript::process — state-11 y/z add swap, wave191 (w13480)

Target: `_ZN11LevelScript7processEi` (src/game/world/LevelScript.cpp), size 57,232.

## Change
Mission-0 intro state 11/12 block: swap the two independent vector component
adds so the `m_tempVec.z += -20000.0f;` line precedes `m_tempVec.y += 500.0f;`.

Found by a local comment-aware rotation sweep (N=2..10) over source lines
1190..1300 on top of commit 482d18617: cell `l2_16` (both directions agree,
the two adds are independent).

## Measurement
- before: raw 51,655, text 71,787
- after:  raw 51,654 (-1), text 71,786 (-1), size 57,232/57,232 exact
- `.rel.plt` md5 unchanged: 289b2a069f8504d1e8a3919e43915e04
- `check`: RATCHET PASS (byte_exact 4507/4524, linked 4507, data 843/843,
  imports 207, lint CLEAN, drift 0, parity 5370/5370)
