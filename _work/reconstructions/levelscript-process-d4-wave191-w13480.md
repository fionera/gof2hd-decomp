# LevelScript::process — d4 rotation, wave191 (w13480)

Target: `_ZN11LevelScript7processEi` (src/game/world/LevelScript.cpp), size 57,232.

## Change
Mission-89 `case 1` (m_nScriptTime >= 30001) block: rotate the 4-statement
`m_tempVec` / enemy-park / geometry-follow group up by one, i.e. move
`m_pGeometry6->setPosition(enemies[0]->geometry->getPosition())` to the front of
the group (before `m_tempVec = *m_pCamera->getPosition()`).

Found by the full d4 (4-statement window) rotation sweep over the committed
51,679-byte body: 1,216 quads x up/down, best measured cell `up 60`.

## Measurement (on top of landed d1 swap b9ee2b44d)
- before: raw 51,679, text 71,810
- after:  raw 51,665 (-14), text 71,797 (-13), size 57,232/57,232 exact
- aligned: 22,003 -> 22,015 (worse; raw is the reported metric)
- `.rel.plt` md5 unchanged: 289b2a069f8504d1e8a3919e43915e04
- `check`: RATCHET PASS (byte_exact 4507/4524, linked 4507, data 843/843,
  imports 207, lint CLEAN, drift 0, parity 5370/5370)

Note: Ghidra disasm of the original block orders `m_pGeometry6->setPosition`
last; this rotation is a codegen-coincidence win (the residual is not yet zero),
kept because it is a measured, size-exact, rel.plt-identical improvement.
