# LevelScript::process — d5 5-statement rotation, wave191 (w13480)

Target: `_ZN11LevelScript7processEi` (src/game/world/LevelScript.cpp), size 57,232.

## Change
Mission-0 intro state 6 (`m_nScriptTimerA >= 1001`) block: rotate the 5-statement
`m_tempVec`/camera group up by one, moving
`m_tempVec += Vector{200.0f, 300.0f, 1800.0f};` to the front of the group
(before `m_pCamera->setTarget(...)`).

Found by the full d5 (5-statement window) rotation sweep over the committed
51,665-byte body: 1,008 windows x up/down. Best cell with size exact and
`.rel.plt` unchanged: `up 890`.

## Measurement
- before: raw 51,665, text 71,797
- after:  raw 51,660 (-5), text 71,792 (-5), size 57,232/57,232 exact
- `.rel.plt` md5 unchanged: 289b2a069f8504d1e8a3919e43915e04
- `check`: RATCHET PASS (byte_exact 4507/4524, linked 4507, data 843/843,
  imports 207, lint CLEAN, drift 0, parity 5370/5370)

Codegen-coincidence reorder (original disasm order of this block is not yet
reconstructed); kept as a measured, size-exact, rel.plt-identical improvement.
