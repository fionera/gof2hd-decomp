# LevelScript::process — wave 190 / w13408: active-tail loop-region adjacent swap

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13408`, branch
`wave190-w13408`, base master `f0982704f` (raw **51,738**, `.text` 72,965,
`.ARM.extab` 603, `.rel.plt` md5 `289b2a069f8504d1e8a3919e43915e04`).

Lens: the `0x3200-0x3e00` callee-saved / cold-block region.  Re-mapped the
mission-81 (line 983) `switch (m_nState)` and the shared active-tail loop
(`getWingmen`), and re-swept the adjacent statements in the region.  One
size-exact, `.rel.plt`-identical raw win landed.

## Landed change

Source lines 1693-1694 (mission-81 state-1 block, `cam1688`):

```cpp
-                    cam1688->translate((float) -delta, 0.0f, (float) (-3 * delta));
-                    self->lodManager->forceUpdate(delta, false);
+                    self->lodManager->forceUpdate(delta, false);
+                    cam1688->translate((float) -delta, 0.0f, (float) (-3 * delta));
```

`cam1688->translate` (camera) and `self->lodManager->forceUpdate` (LOD manager)
touch disjoint objects; `forceUpdate` does not read the camera position and
`translate` does not read the LOD state.  The original emits the LOD update
before the camera translate in this block.

Measured alone on the real build:

```
raw        51,738 -> 51,737  (-1)
aligned    22,489 -> 22,497
size       57,232 / 57,232   (exact)
.text      72,965 -> 72,964  (-1)
.ARM.extab 603 (unchanged)
.rel.plt   md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical)
total      73,588 -> 73,587  (-1)
```

`check` -> RATCHET PASS (byte_exact 4507, linked 4507, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370).

## Region observations (not landed)

* The first `getWingmen` loop (`for (unsigned i...; wingman->isWingMan())`)
  is rotated (condition at bottom, store inline) in the original but a
  while-loop with a split cold store in ours; the `__builtin_expect(...,0)`
  is required for the current shape (removing it costs +90 raw).  No
  source spelling tried (`int i`, `i++`, `while`, `do/while`, `continue`,
  `!expect(!isWingMan(),1)`) is a size-exact win.
* The shared active-tail exit block (source 1490-1495) emits
  `setLookAtCam / setCollide / m_bStartSequenceOver / m_bCinematicBreak /
  setVulnerable` in ours but `setLookAtCam / setVulnerable / m_bCinematicBreak /
  setCollide` with `strh`-merged `m_bStartSequence` in the original; every
  permutation tried is neutral or size-changing (+8).
* The first literal pool is still `+0x1c` in ours (`0x3a8` vs original
  `0x38c`); the shift is the loop-body/pool block order above, not a wrong
  constant.
