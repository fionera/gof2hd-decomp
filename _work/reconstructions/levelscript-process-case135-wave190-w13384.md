# LevelScript::process — wave 190 / w13384: case-135 setVisible/setPosition order

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13384`, branch
`wave190-w13384`.  Base before this pass: raw **51,774**, `.text` 73,271,
`.ARM.extab` 603.

## Win: case 135 state-0 loop body — `setVisible` before `setPosition`

`case 135` state 0 (disasm `0x13c50e`) re-homes every `shipGroup == 8`
enemy.  The source had

```cpp
(*enemies)[i]->setPosition(playerPos + Vector{(float) xOff, 5000.0f, 25000.0f});
(*enemies)[i]->setVisible(true);
```

and the two are independent (visibility does not read the transform).  Moving
the `setVisible(true)` ahead of the `setPosition(...)` call is
semantics-preserving and is a measured win:

* raw **51,774 -> 51,772** (-2), aligned 22,414 -> 22,413,
* `.text` 73,271 -> 73,269 (-2), `.ARM.extab` 603 unchanged,
  build-id 20, total 73,894 -> 73,892,
* size 57,232/57,232 exact, `.rel.plt` md5
  `289b2a069f8504d1e8a3919e43915e04` unchanged, `.ARM.exidx` 0.

The original's emitted order in this loop is actually `setPosition` ->
`awake` -> `setVisible` -> `setEnemy`; a full reorder to that order measured
raw 51,775 (+1) and was not landed.  The simple two-statement swap is the
local optimum.

## Method

The d1/d2/d3 sweeps only considered pairs the access-chain model accepted.
The model under-approximates call-argument reads (single-argument call parens
are stripped by `norm()`), so it generated many read-after-write false
positives, and it over-approximates by treating any two calls on the same
receiver as conflicting.  A full adjacent-pair sweep over *all* rejected
pairs (`scratch/w13384/h/sweepall.py`, 1,054 candidates, 4 workers) found the
clean win above.  The other `-1/-2` cells are read-after-write swaps
(`m_tempVec = getPosition(); m_tempVec.y += ...`, `camPos += ...;
routePoints = ...`, `offset *= 1000; *camPos += offset`) or a dependent
`frame->setPosition` / `CameraSetLocal(frame->getMatrix())` swap, so they
were not landed.
