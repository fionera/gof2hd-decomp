# LevelScript::process — wave 191 / w13466: mission-81 arrival animation-state order (d1)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13466`, branch
`wave191-w13466`, base master `f325e2d87` (raw **51,679**, aligned **22,003**,
`.text` 71,811, `.ARM.extab` content diff 575, `.rel.plt` byte-identical).

Task: fresh `d=1`/`d=2` adjacent-statement rotation sweep over the whole
function body on current master, excluding the cells already documented in the
wave-189/190 notes.

## Landed change

Mission-81 alien-orbit arrival, `case 0` (`m_nScriptTime >= 10001` block),
source lines 993-997:

```cpp
-                    m_pGeometry5->setPosition(Vector{0.0f, 0.0f, 10000.0f});
                     ((AbyssEngine::Transform *) canvas983->TransformGetTransform(m_pGeometry5->transform))
                             ->SetAnimationState((AbyssEngine::AnimationMode) 3, nullptr);
+                    m_pGeometry5->setPosition(Vector{0.0f, 0.0f, 10000.0f});
                     ((AbyssEngine::Transform *) canvas983->TransformGetTransform(m_pGeometry5->transform))
                             ->SetAnimationState((AbyssEngine::AnimationMode) 1, nullptr);
```

`Transform::SetAnimationState(mode 3)` only sets `animating = true` and
`currentTime = rangeStart`; it reads no position.  `AEGeometry::setPosition`
writes the geometry transform's translation (a disjoint field), so the two
statements commute.  The following `SetAnimationState(mode 1)` is unchanged.

Measured alone on the real build:

```
raw        51,679 -> 51,675  (-4)
aligned    22,003 -> 22,003  (unchanged)
size       57,232 / 57,232   (exact)
.text      71,811 -> 71,807  (-4)
.ARM.extab 156,312 B, content diff 575 (unchanged; size/base exact @0x1a3b78)
.rel.plt   29,392 B, byte-identical
.ARM.exidx byte-identical
.rodata 0 diff / .data identical / .bss 2
check      RATCHET PASS (byte 4507, linked 4507, size 4524/4524,
           data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370)
```

## Sweep harness

Scratch `/home/debian/GalaxyOnFire2/scratch/w13466` (`stmts.py`, `harness.py`,
`sweep.py`): parse the function body into same-indent straight-line runs of
single statements (barriers: braces, control headers, labels, `return`/
`break`/`continue`), then for each run compile+gold-link every `d=1` swap and
both `d=2` rotations (`[B,C,A]` and `[C,A,B]`) with the exact NDK r18b `-Oz`
flags, 4 workers.  Candidate line numbers are 1-based source lines.
