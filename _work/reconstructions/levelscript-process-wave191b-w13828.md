# LevelScript::process — wave 191b / w13828: case-154 state-4 moveForward order

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13828`, branch
`wave191b-w13828`, base master `da998189d`.

Session start: raw **52,997**, aligned **19,531**, `.text` diffbytes 76,629,
`.ARM.extab` 623, `.rodata`/`.ARM.exidx`/`.rel.plt` diff 0, size 57,232/57,232,
insn 20,728 / 20,830.

## Landed

### case 154 (mission 0x9a) state 4 — `moveForward` before the camera-local block

Original `0x143138`: `(*enemies)[0]->geometry->moveForward(step)` is emitted
**before** `CameraGetCurrent` / `CameraGetLocal`, not after.  Our source built the
`Matrix local` first.  Reordering:

```c++
 } else if (m_nState == 4) {
     float step = (float) delta * 0.25f;
+    (*enemies)[0]->geometry->moveForward(step);
     AbyssEngine::PaintCanvas *canvas = Globals::Canvas;
     Matrix local = *reinterpret_cast<Matrix *>(canvas->CameraGetLocal(canvas->CameraGetCurrent()));
-    (*enemies)[0]->geometry->moveForward(step);
     m_tempVec = AbyssEngine::AEMath::MatrixGetPosition(local);
```

Result: raw **52,997 -> 52,999** (+2), aligned **19,531 -> 19,497** (-34),
`.text` **76,629 -> 76,631** (+2), `.ARM.extab` 623 unchanged, size exact.
Landed: the aligned instruction match is the better signal in this region and the
aligned win (-34) far outweighs the +2 raw / +2 `.text` cost (cf. w13814, which
landed raw +1 for aligned -4).

## Measured variants (all size 57,232 exact unless noted)

| variant | raw | aligned | .text | .ARM.extab |
|---|---:|---:|---:|---:|
| session start (HEAD) | 52,997 | 19,531 | 76,629 | 623 |
| **case-154 state-4 moveForward first (landed)** | 52,999 | **19,497** | 76,631 | 623 |
| prologue: drop `Level *level` local, call `m_pLevel->` directly | 53,005 | 19,538 | — | — |
| radio-14 moved into state-0 success block | 53,003 | 19,531 | — | — |
| case-154 state-0 full reorder (prior, A.patch) | 53,044 | 19,540 | — | — |
| case-94 lookBehind order (prior) | 54,056 | 19,531 | — | — |

## Residual / next angles (not landed)

- **Mission-1 tail** `0x138a66`: original emits `enableFillScreen(true)` /
  `switch_to_target_setting = 0` / `SetCurrentApplicationModule(5)` *before* the
  camera-local matrix block (branch to `0x144f60`); ours emits them after
  (`0x138c84..`).  src ~lines 960-980.  Not yet tested.
- **Prologue register plan** `0x137d18`: original keeps `this` in `sl` and
  `&this->m_pLevel` in `r4` with one `strd`; ours keeps `Globals::status` in `sl`
  and `this` in `fp`.  The shared tail (`0x144e36..`) and mission dispatch
  (`0x13879e`) reload `[sp,#92]` where the original keeps it in `r6`, and the
  original loads `sl = delta` at `0x1387ee` vs ours at `0x138830`.  These are all
  symptoms of the same prologue allocation; no source shape found yet.
- **Mission-94 state-1** `0x13e0e6` is the largest single residual block
  (n=614); next blocks `0x13e962` (423), `0x13d74a` (402), `0x13dbd8` (354),
  `0x141028` (mission-106, 324), `0x13a1ae` (270).

## Tools

`tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi`
for raw/aligned/size; scratch helpers `dumpdiff.py <minN>` (aligned diff blocks
with orig/ours disasm + our source line), `deltas.py`, `blocklines.py`,
`calls2.py`, `secdiff.sh`.

## Second landed pass: mission-158 (case 158) state-5 store order

Inside the `for (int i = 0; i < 3; i++)` loop that re-enables the three enemies
(src ~L3078), the original emits `str.w sl,[r0,#296]` (`field_0x128 = 50000`)
**before** `str r5,[r0,#56]` (`field_0x38 = 4`).  Our source had the two stores
swapped.  Fix:

```c++
 for (int i = 0; i < 3; i++) {
     ((PlayerFighter *) (*enemies)[i])->setAIDisabled(false);
-    (*enemies)[i]->field_0x38 = 4;
     ((PlayerFighter *) (*enemies)[i])->field_0x128 = 50000;
+    (*enemies)[i]->field_0x38 = 4;
 }
```

Result: raw **52,999 -> 52,994** (-5), aligned **19,497 unchanged**, size
57,232/57,232, `.text` **76,631 -> 76,626** (-5), `.ARM.extab` 623 unchanged,
`.rodata`/`.ARM.exidx`/`.rel.plt` diff 0.  Landed (raw win, aligned neutral).

| variant | raw | aligned | .text | .ARM.extab |
|---|---:|---:|---:|---:|
| HEAD after case-154 pass | 52,999 | 19,497 | 76,631 | 623 |
| **mission-158 state-5 store order (landed)** | **52,994** | **19,497** | **76,626** | 623 |
| prologue: `Level *&level = m_pLevel;` | 53,007 | 19,504 | — | — |
| mission-1 state-3: drop duplicate `m_nScriptTimerA = 0` | 53,006 | 19,500 | — | — |
| mission-1 `else { translate }` | 52,999 | 19,497 | — | — |
| mission-1 `translate` before state-1 if | 53,680 | 19,525 | — | — |
| mission-94 state-1 freelook move only | 54,056 | 19,497 | — | — |
| mission-94 lookBehind order swap | 53,001 | 19,497 | — | — |
| mission-94 2nd-site dock/freelook swap | 53,001 | 19,497 | — | — |
| case-154 state-0 split nested ifs | 55,210 | 19,956 | — | **size 57,236 fatal** |
| mission-94 full state-5 reorder | — | — | — | **size 57,236 fatal** |
| mission-94 dock/freelook reorder alone | — | — | — | **size 57,236 fatal** |
| mission-158 state-5 `setFixed`/freelook reorder | 52,997 | 19,498 | — | — |
