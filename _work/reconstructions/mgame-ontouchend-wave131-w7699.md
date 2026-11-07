# MGame::OnTouchEnd — wave 131 (w7699): separate-`if` test shape vs `.rel.plt` order

Status: analysis-only, 2026-11-07. Worktree `/home/debian/GalaxyOnFire2/wt/w7699`,
base `7c6933c2d`. Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only). No source change
landed: every 8080 B shape that improves `.text` flips `.rel.plt` (3-slot rotation); the one
shape that keeps `.rel.plt` exact is 4 B worse than the base. Scratch `/dev/shm/w7699*`.

Target `_ZN5MGame10OnTouchEndEiiPv` (`MGame::OnTouchEnd`), original 8080 B @ `0x17a144`.
Baseline (verified): fuzzy **81.9 %**, 8080/8080 B, `.text` **85,379**, `.rel.plt` **0**,
`.ARM.exidx` **0**, `.ARM.extab` **1,435**, `.rodata`/`.data`/`.bss` exact.

## 1. The hud test block is the whole `[test][final tail][canary]` cluster

Original hud cluster (VMA):

```
17ad4c  dispatch (0x200, 0x800, 0x400, 0x2000..0x1000)
17adc0  0x200 body: ldrd; movs r1,#1; b 17afde
17adc8  won body ...
17ae8c  0x800 body -> b 17afe2
17afd8  0x400 body: ldrd; movs r1,#2; fallthrough
17afde  blx Hud::initHudMenu          ; shared tail owned by the 0x400 body
17afe2  ldr flags; cbz r0, 17b01a     ; test: cold body is the branch target
17afe8  final tail (isAutoPilot)      ; fallthrough
17b004  shared canary return
17b01a  hudTouchFlags == 0 body -> b 17afe8
```

Our base emits the *inverted* test: `ldr; cmp r0,#0; bne.w final_tail`, so the final tail is a
branch target and MBP places the whole `[test][final tail][canary]` cluster at the very end
(`0x184e`), not after the shared `blx`. This is the source of the 20 `b.w 0xec0` vs
`b.w 0x184e` structural diffs (one per early-return site).

## 2. A separate `if (hudTouchFlags == 0)` reproduces the original test polarity

Changing the trailing `} else if (this->hudTouchFlags == 0) {` to a separate
`}\n if (this->hudTouchFlags == 0) {` emits exactly the original test block:

```
17b936  ldr flags; cbz r0, 17b96e     ; cold body is the branch target
17b93c  final tail                    ; fallthrough
17b958  shared canary return
17b96e  hudTouchFlags == 0 body -> b 17b93c
```

i.e. the `[test][final tail][canary]` adjacency is reproduced, but the cluster is still placed
at the end of the function rather than after the shared `blx`. The separate-`if` shape is
**8076 B (-4)**, `.rel.plt` 0. The -4 B is exactly the test block: `ldr; cmp; bne.w` (10 B)
becomes `ldr; cbz` (6 B). The original also uses `ldr; cbz`, so the base is size-exact only
because its 4 B-too-big test compensates for a 4 B deficit elsewhere in the function.

## 3. Every 8080 B shape that improves `.text` flips `.rel.plt`

The best `.text` shape found is separate-`if` + bare `hasFailed()`:

| variant | pct | size | `.text` | `.rel.plt` | `.ARM.extab` |
|---|---:|---:|---:|---:|---:|
| base | 81.9 | 8080 | 85,379 | **0** | 1,435 |
| separate-`if` | 79.1 | 8076 | (shifted) | 0 | 1,473 |
| separate-`if` + bare `hasFailed()` | 77.6 | 8080 | **85,311** | **6** | 1,440 |
| separate-`if` + bare `hasFailed()` + bare `goingToWaypoint()` | 77.6 | 8080 | **85,304** | **6** | 1,440 |
| separate-`if` + no `campaign` temp | 79.3 | 8080 | 85,713 | 0 | 1,472 |
| separate-`if` + `goto afterHudMenu` | 81.9 | 8080 | 85,383 | 0 | 1,435 |
| base + bare `menuWindow->OnTouchEnd` | 80.5 | 8080 | 85,398 | 0 | 1,436 |
| base + bare `goingToWaypoint()` | 81.9 | 8080 | 85,395 | 0 | 1,435 |

The `.rel.plt` delta is a **3-slot rotation**, not a new/removed import:

```
orig slots 3329-3331: inCinematicMode, setCinematicMode, resetAnalogStick
ours (failed bare):   resetAnalogStick, inCinematicMode, setCinematicMode
```

Root cause (measured, not inferred): bare `hasFailed()` moves the cutscene tail
(`frameTime=0; hud->resetAnalogStick(); ...`) *before* the `menuTouchOpen` block, so the first
`resetAnalogStick` reference (0x17b63e) precedes the first `inCinematicMode` reference
(0x17b6f4). In the original (and in the base) the order is reversed: `inCinematicMode` at
0x17b816, `resetAnalogStick` at 0x17b8ec. The cutscene tail is the shared fallthrough of the
failed-body end; the +8 B of bare `hasFailed()` re-ranks that edge in MBP and lands the tail
inside the campaign body instead of after the `menuTouchOpen` block.

## 4. New hypotheses tried and rejected

| hypothesis | result |
|---|---|
| `goto afterHudMenu` at the end of the weaponSlot body (wave-4007 shape) | folded away; byte-identical to base (81.9 / 8080 / `.text` 85,379) |
| separate-`if` + `goto afterHudMenu` | 81.9 / 8080 / `.text` 85,383 / PLT 0 — the `goto` adds the missing 4 B back but does **not** flip the 0x200/0x400 tail merge in the current tree |
| move `if (hudTouchFlags == 0)` block *before* the `weaponSlot` chain | 75.3 / 8052 / `.text` shifted |
| swap `if (cutsceneActive)` and `if (menuTouchOpen)` blocks | 69.9 / 8092 / PLT 16 |
| swap blocks + separate-`if` + bare `hasFailed()` | 59.9 / 8100 / PLT 16 |
| `!hudTouchFlags` spelling | byte-identical to base |
| `pauseOpen == true`, `cutsceneActive != 0`, `keepObjectives == true`, `hudTouchFlags` `!`/`!=0` | all 8080/PLT 0 but `.text` 85,379-85,395 (no win) |

## 5. Remaining attack

1. Find the 4 B deficit that the base hides behind its 4 B-too-big test. The base has 1 fewer
   instruction than the original (2774 vs 2775) and the test is 1 instruction bigger, so there
   are two missing Thumb16 instructions somewhere in the campaign body. Adding them + the
   separate-`if` test shape would give the original `ldr; cbz` polarity at 8080 B.
2. Re-place the cutscene tail after the `menuTouchOpen` block in the bare-`hasFailed()` shape.
   The tail is a shared fallthrough of the failed-body end; moving it requires changing which
   block ends the failed body (e.g. the `setPlayerRoute(nullptr)` / `removeRoute` order), not
   the `hudTouchFlags == 0` test.
3. The `separate-`if` + `goto` shape is the only 8080 B / PLT 0 shape with the correct test
   polarity; it is 4 B worse than base only because the `goto` is a real branch. If (1) is
   found, drop the `goto`.

## 6. Reproduction

```
# per-function score (0.16 s on the linked .so)
python3 /dev/shm/w7699/fastscore.py
# section byte deltas
python3 /dev/shm/w7699/secdiff.py
# rebuild only MGame.cpp + relink (1.2 s)
cmake --build cmake-build-match --target link_so -j6
```
