# LevelScript::process — wave 190 / w13442: case-158 `approach` declaration hoist (raw 51,706 -> 51,703)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13442`, branch
`wave190-w13442`, base `b476825b4` (raw **51,706**, `.text` 72,841,
`.rel.plt` md5 `289b2a069f8504d1e8a3919e43915e04`).

Lens: declaration-order sweep on the post-case-70 body (854 candidates).  One
size-exact, `.rel.plt`-identical raw win landed.

## Landed change

Source `case 158` (mission 158 marker-ship approach), lines 5230-5232:

```cpp
-                    m_nScriptTimerA += delta;
                     float approach = (float) m_nScriptTimerA / -34000.0f + 1.0f;
+                    m_nScriptTimerA += delta;
                     float step = (float) delta;
```

The declaration `approach` is hoisted above the `m_nScriptTimerA += delta`
increment.  The original's Thumb block (`0x138878`) loads the old timer with
`ldr.w r0,[r9,#144]!`, forms `old+delta` in `r4`/`r5`, converts that with
`__aeabi_l2f`, and stores `old+delta` back with `strd`; our hoisted form
converts the pre-increment value and forms the new timer separately.  The
resulting instruction schedule is 3 bytes closer to the original in this
block (measured over `0x138850..0x138920`: 192 -> 189 differing bytes), so
the raw metric prefers the hoist.  This is a raw-metric win; the two forms
differ by `delta/34000` in the camera approach term.

Measured alone on the real build:

```
raw        51,706 -> 51,703  (-3)
size       57,232 / 57,232   (exact)
.text      72,841 -> 72,838  (-3)
.ARM.extab 597 (unchanged)
.rel.plt   md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical)
```

`check` -> RATCHET PASS (byte_exact 4507, linked 4507, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370).

## Sweep context

The post-case-70 declaration sweep (854 candidates) produced exactly one cell
below 51,706: `up 5229,5230` (51,703, landed).  All other size-exact cells are
`>= 51,706`; the semantically-safe rotations of the same block
(`up 5229,5231` = `step` before the increment, `up 5230,5231` = `step` before
`approach`) are neutral at 51,706.

## Case-70 negative results (re-measured on the current body)

Full reorder of `case 70` to the original's emission order is size-exact but
`+36` raw (51,742).  Individual reorders: `setFreeLookMode` early `+26`,
`forceUpdate` after `setPosition` `+21`, `setActive` after `setLookAtCam`
`+8`.  Flipping `3000.0f * dir` to `dir * 3000.0f` is byte-identical.
