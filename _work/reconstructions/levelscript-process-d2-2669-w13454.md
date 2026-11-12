# LevelScript::process — wave 190 / w13454: state-6 setPosition sink

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13454`, branch
`wave190-w13454`, base master `83d52fe52` (raw **51,701**, aligned **21,920**,
`.text` 72,257, `.rel.plt` md5 `289b2a069f8504d1e8a3919e43915e04`).

Task lens: literal-pool ordering / branch-target-cold-block layout / prologue
register-allocation lever.  A fresh full `d=1` sweep on the current body found
only the two already-documented read-after-write cells (`m_nScriptTimerA`
across `approach`, `*tmp = getUpVector()` across `*camPos += *tmp * 400`).
A fresh full `d=2` rotation sweep (1,274 candidates, exact NDK r18b `-Oz`
compile + gold link, 4 workers) found the same unsafe timer cell plus one new
size-exact, `.rel.plt`-clean raw win.

## Landed change

Source lines 2669-2671 (mission-4-ish state-6 camera setup):

```cpp
-                    m_pCamera->setPosition(m_tempVec);
                     Vector facing = m_pGeometry6->getPosition() - m_tempVec;
                     m_pHud->visible = 0;
+                    m_pCamera->setPosition(m_tempVec);
```

`facing` is a local subtraction that only *reads* `m_tempVec`;
`m_pHud->visible = 0` is a byte store on a disjoint object; and
`setPosition(m_tempVec)` only reads `m_tempVec`.  Rotating the camera
`setPosition` below both is therefore semantics-preserving.  (The original
emits `setPosition` before `facing`, but the raw metric prefers the sink.)

Measured alone on the real build:

```
raw        51,701 -> 51,699  (-2)
aligned    21,920 -> 21,925
size       57,232 / 57,232   (exact)
.text      72,257 -> 72,255  (-2)
.ARM.extab 584 (unchanged; size/base exact 156,312 B @ 0x1a3b78)
.rel.plt   md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical)
total      72,861 -> 72,859  (-2)
```

`check` -> RATCHET PASS (byte_exact 4507, linked 4507, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370).

## Rejected cells (same sweeps)

| move (0-based) | source lines | reason |
|---|---|---|
| `d=1` line 5230 | 5230/5231 | `m_nScriptTimerA += delta` across `approach = timer/...` — read-after-write |
| `d=1` line 3736 | 3736/3737 | `*tmp = getUpVector()` across `*camPos += *tmp * 400` — read-after-write |
| `d=2` line 5230 | 5230-5232 | same timer rotation — read-after-write |

## Block-layout observation (not landed)

The original lays the mission-5 test+body immediately after the shared-tail
body (`0x13812a`/`0x13813c`) and the mission-81 test+body after it
(`0x1381b2`/`0x1381be`).  Ours lays mission-81 first (`0x13812c`/`0x13813a`)
and mission-5 second (`0x138256`/`0x138264`), so the shared-tail `getWingmen`
branch is a wide `beq.w` in ours vs the original's narrow `beq.n`.  Changing
the `else if (mission == 5)` to a separate `if` is documented (w13442) to
grow the function by +164 B, so the block order is not reachable by a
size-exact source edit yet.
