# LevelScript::process — comment-aware rotation win, wave191 (w13480)

Target: `_ZN11LevelScript7processEi` (src/game/world/LevelScript.cpp), size 57,232.

## Motivation
Earlier d1..d8 rotation sweeps skipped any statement line ending in a trailing
`// ...` comment (the `simple()` filter required the line to end with `;`).
Re-running the rotation sweeps over the *comment-bearing* windows only
(N=2..6, 700 new windows x up/down) found new wins.

## Change
Mission-0 intro state 11/12 block (`m_nState = 12`): rotate the 4-statement
`m_tempVec` group down by one (move
`m_tempVec = (*enemies)[enemies->count - 10]->geometry->getPosition();` to the
end of the group, after `m_pLevel->lodManager->forceUpdate(30, false);`).

Found as `c4_24_down` in the comment-window sweep.

## Measurement
- before: raw 51,660, text 71,792
- after:  raw 51,655 (-5), text 71,787 (-5), size 57,232/57,232 exact
- `.rel.plt` md5 unchanged: 289b2a069f8504d1e8a3919e43915e04
- `check`: RATCHET PASS (byte_exact 4507/4524, linked 4507, data 843/843,
  imports 207, lint CLEAN, drift 0, parity 5370/5370)

Runner-up cells from the same sweep (all size-exact, rel.plt unchanged):
c3_32_down 51,658; c2_119_down/up 51,659.
