# LevelScript::process — wave 190 / w13455: animation-state call swap

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13455`, branch
`wave190-w13455`.  Base after the d6 first-person flag sink: raw **51,697**,
`.text` 72,253.

A targeted 5!-permutation search of the mission-0 state-1 camera-frame block
(source 1271-1278) found one size-exact, `.rel.plt`-clean permutation well
below the current order: swapping the two `SetAnimationState` calls.

## Landed change

Source lines 1272-1275:

```cpp
                     ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(m_pGeometry6->transform))
-                            ->SetAnimationState((AbyssEngine::AnimationMode) 3, nullptr);
+                            ->SetAnimationState((AbyssEngine::AnimationMode) 1, nullptr);
                     ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(m_pGeometry6->transform))
-                            ->SetAnimationState((AbyssEngine::AnimationMode) 1, nullptr);
+                            ->SetAnimationState((AbyssEngine::AnimationMode) 3, nullptr);
```

`Transform::SetAnimationState` writes disjoint fields per mode: mode 3 sets
`animating = true` and `currentTime = rangeStart`; mode 1 writes only `flags`
(mode 0 is the only one that also writes `animating`).  Both calls operate on
the same transform but touch disjoint state, so the swap is
semantics-preserving and the final field values are identical.

Measured alone on the real build:

```
raw        51,697 -> 51,690  (-7)
aligned    21,955 -> 21,964
size       57,232 / 57,232   (exact)
.text      72,253 -> 72,246  (-7)
.ARM.extab 584 (unchanged; size/base exact 156,312 B @ 0x1a3b78)
.rel.plt   md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical, 29,392 B)
```

`check` -> RATCHET PASS (byte_exact 4507, linked 4507, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370).

## Block-permutation census (for future passes)

The full 120-permutation census of the block
`{A=setPosition(enemyPos), B=anim3, C=anim1, D=moveForward, E=setDirection}`
on the current body: `ACBDE` is the unique size-exact winner at 51,690;
`ABCDE` (current) 51,697; `ECABD` 51,699; `ECDAB` 51,700.  The original emits
`ABCDE` (`anim3` at `0x1386f2`, `anim1` at `0x138706`), so this is another
raw-metric-over-faithfulness reorder, like the w13408 pass-5 cell.
