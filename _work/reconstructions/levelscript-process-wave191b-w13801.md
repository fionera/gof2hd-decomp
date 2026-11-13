# LevelScript::process — wave 191b / w13801: case-144 escort cloak condition order

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13801`, branch
`wave191b-w13801`, base master `e03264c21`.

Session start: raw **53,037**, aligned **19,875**, `.text` diffbytes 75,211,
`.ARM.extab` 587, `.rel.plt` md5 `289b2a069f8504d1e8a3919e43915e04`.
Session end: raw **53,030**, aligned **19,820**, `.text` diffbytes 75,204,
`.ARM.extab` 587, `.rel.plt` md5 unchanged.

## Landed

Case 144 state 3, the cloaked-escort loop (`src/game/world/LevelScript.cpp`
line ~4339).  The original evaluates the random roll first and only then the
`field_0x13c` flag:

```
13d5ca: ldr   r0, [r6, #0]          ; Globals::rnd
13d5cc: movs  r1, #100
13d5ce: blx   AERandom::nextInt(int)
13d5d2: cmp   r0, #9
13d5d4: bgt.n 13d5f8                ; -> moveForward
13d5d6: ldr   r0, [r4, #4]
13d5d8: ldr.w r0, [r0, r5, lsl #2]
13d5dc: ldrb.w r1, [r0, #316]       ; PlayerFighter::field_0x13c
13d5e0: cbnz  r1, 13d5f8
13d5e2: movs  r1, #1
13d5e4: blx   PlayerFighter::setCloakingPossible(bool)
...
13d5f8: ... moveForward
```

Our previous source had the operands the other way round and wrapped them in a
nested `__builtin_expect`, which also made clang lay the `moveForward` path out
as fall-through and the cloak body out of line (extra `cbz`/`b` pair):

```cpp
// before
if (__builtin_expect(__builtin_expect(!((PlayerFighter *) (*enemies)[i])->field_0x13c, 0) && Globals::rnd->nextInt(100) <= 9, 0)) { // lint: arm_builtin
// after
if (Globals::rnd->nextInt(100) <= 9 && !((PlayerFighter *) (*enemies)[i])->field_0x13c) {
```

After the change our loop body `0x13d7e2..0x13d842` is instruction-for-instruction
identical to the original `0x13d5be..0x13d61e` (same `bgt`/`cbnz`/`bcc` layout,
same literal-pool slot for `0.05f`).

| metric | before | after |
|---|---:|---:|
| raw | 53,037 | **53,030** (-7) |
| aligned | 19,875 | **19,820** (-55) |
| size | 57,232 | 57,232 (exact) |
| `.text` diffbytes | 75,211 | **75,204** (-7) |
| `.ARM.extab` diffbytes | 587 | 587 |
| `.rel.plt` md5 | 289b2a06 | 289b2a06 |

`cmake --build cmake-build-match --target check` = RATCHET PASS
(byte_exact 4508, linked_exact 4508, size 4524/4524, data 843/843, imports 207,
lint CLEAN).

## Investigated / rejected (all size-fatal or worse)

- Case 142 state 1 (`m_pLevel->getGasClouds()` loop): dropping the
  `Level *lvl4280 = m_pLevel;` local so the member is re-read each iteration
  (matching the original's `ldr r0,[r6,#0]`) makes the function **57,240 B**
  (+8) and raw 55,246 / aligned 20,380.  Reverted.
- Case 144 state 2 (`messages[3]->isOver()`): ours loads the message array from
  the stack slot (`ldr r0,[sp,#72]; ldr r0,[r0,#4]`) where the original keeps it
  in `fp` (`ldr r0,[fp,#4]`); the tail branch also targets a different shared
  `m_nState = 3` block (orig `0x1438d6`, ours `0x13f29e`).  Base-register /
  block-layout residual, not a statement-order fix.
- Case 144 state 1 (`0x139532`) is already structurally faithful; residual is
  base-register allocation only.
- Case 102 s2 escort (line ~3612) d/r declaration swap: raw 53,037 -> 53,040
  (+3), aligned 19,875 -> 19,877 (+2).  Not a win.

## Landed (2): case-92 state-4 tail test — plain `if`, not `__builtin_expect`

`src/game/world/LevelScript.cpp` line ~3064 (case 92, `m_nState == 4` branch):

```cpp
// before
if (__builtin_expect(m_nScriptTimerA > 2000, 1)) { // lint: arm_builtin
    m_nState++;
    m_nScriptTimerA = 0;
}
// after
if (m_nScriptTimerA > 2000) {
    m_nState++;
    m_nScriptTimerA = 0;
}
```

The original `0x140104` emits a real branch (`bge.w 144e36`, i.e. skip the body
when `2000 - timerA >= 0`) followed by the straight-line body and a second
`b.w 144e36`.  Our `__builtin_expect(..., 1)` made clang if-convert the body to
`itttt lt` / `itt lt` predicated stores.  Dropping the hint restores the
branchy layout; our body `0x13fa96..0x13faa8` is now instruction-for-instruction
identical to the original `0x140104..0x140116` (only the `strd` base register
and the shared-return branch target differ).

| metric | before | after |
|---|---:|---:|
| raw | 53,030 | **53,024** (-6) |
| aligned | 19,820 | **19,816** (-4) |
| size | 57,232 | 57,232 (exact) |
| `.text` diffbytes | 75,204 | **75,198** (-6) |
| `.ARM.extab` | 587 | 587 |
| `.rel.plt` md5 | 289b2a06 | 289b2a06 |

`check` = RATCHET PASS (byte_exact 4508, linked 4508, size 4524/4524, data
843/843, imports 207, lint CLEAN).

## Rejected experiments (this session, all reverted)

- **case-142 state-1 gas-cloud loop**: dropping the `Level *lvl4280 = m_pLevel;`
  local so the member is re-read each iteration makes the loop body
  instruction-identical to the original `0x13fac0..0x13fae8`, but the function
  grows to **57,240 (+8)** (raw 55,246 / aligned 20,380).  The original spills
  `this` into *two* stack slots (`sp+80` for the loop-condition re-read,
  `sp+88` for the `m_nState = 2` store) and the loop is rotated so the preheader
  jumps into the condition half; clang cannot be coaxed into that layout from
  source without the size regression.
- **case-144 state-2** `messages[3]->isOver()`: our body is instruction-identical
  to the original `0x13d6c0..0x13d6da`; the residual is only the `fp` vs
  `sp+72` base-register choice and the shared `m_nState = 3` block target
  (`0x1438d6` vs `0x13f29e`).  Block layout, not statement order.

## Landed (3): case-11 state-12 order + visibility, case-6 s6 plain if, two expect drops

All four edits are in `src/game/world/LevelScript.cpp`:

1. **case 11 state 12** (line ~864): `m_nState = 12;` moved to the head of the
   `m_nScriptTimerA >= 2501` body, before `StopEngineSound()`.  Original
   `0x13d37c`: `m_nState=12` -> `StopEngineSound` -> `field_0x1c=156` ->
   `PlayEngineSound` -> `startSmokeEmission` -> `setVisible(true)` ->
   `setExhaustVisible(false)` -> `setSpeed(2.0f)`.  Our body
   `0x13d3ac..0x13d3e4` is now instruction-identical apart from base registers
   and the shared-return branch.
2. **case 11 visibility swap**: `setVisible(true)` before
   `setExhaustVisible(false)` (same original order as above).
3. **case 6 s6** (line ~1112): `__builtin_expect((m_nScriptTimerA >= 1100 - delta), 0)`
   -> plain `if`.  Original `0x13bb7c` emits `ittt ge` predicated stores; the
   plain form reproduces `bfa2 ittt ge; movge r0,#7; strge.w r0,[fp,#28];
   strdge r8,r8,[r4]` exactly.
4. **two `__builtin_expect(...)` drops** found by a 66-site sweep (each site
   replaced one at a time, size-exact only):
   - line ~2513, case-100 s2 `((RadioMessage *)((*messages)[1]))->isOver()`:
     drop `__builtin_expect(..., 0)`.
   - line ~2844, case-100 s2 `player->isDockedToDockingPoint()`:
     drop `__builtin_expect(..., 1)` (single-site raw -30, aligned unchanged).

| metric | HEAD | after |
|---|---:|---:|
| raw | 53,024 | **52,993** (-31) |
| aligned | 19,816 | **19,776** (-40) |
| size | 57,232 | 57,232 (exact) |
| `.text` diffbytes | 75,198 | **75,167** (-31) |
| `.ARM.extab` diffbytes | 587 | 587 |
| `.rel.plt` md5 | 289b2a06 | 289b2a06 |

`check` = RATCHET PASS (byte_exact 4508, linked 4508, size 4524/4524, data
843/843, imports 207), lint CLEAN, drift 0.

## Rejected experiments (this session, all reverted)

- Sweep of all 66 `__builtin_expect` sites on top of the landed base, then a
  second sweep of the 30 size-exact variants on top of the landed combo.  No
  further site improves raw below 52,993 while staying size-exact:
  - line 4114 `(*enemies)[i]->shipGroup == 8`: raw 53,279 / aligned 19,732
    (aligned -44, raw +286) -- best aligned but raw-fatal.
  - line 1467 `wingman->isWingMan()`: 53,062 / 19,764 (+69 / -12).
  - line 4476 `m_nScriptTimerA > 2000 && < 5000`: 52,995 / 19,766 (+2 / -10).
  - line 4871 `m_nScriptTimerB <= 6999`: 53,001 / 19,772 (+8 / -4).
  - line 2233 `isAlwaysFriend()`: 52,995 / 19,773 (+2 / -3).
  - line 1033 `!Globals::layout->isFading()`: 52,994 / 19,774 (+1 / -2).
  - line 5126 `messages[15]->isOver()`: 53,020 / 19,763 (+27 / -13).
  - lines 1150/2288/3826/4739/5176 and others are metric-neutral.
- HEAD + the two expect drops (without the case-11/case-6 edits) measures
  52,992 / 19,785: raw -1 but aligned +9 vs the landed combo.  The landed
  combo is preferred because the case-11/case-6 edits are instruction-verified
  structural matches (see above), not metric-only trades.
- case-12 state-1 `m_nScriptTimerA = 0;` moved after
  `m_pCamera->setPosition(m_tempVec)` (original `0x13d3b8` stores the timer
  after `setPosition`): on top of the landed combo it measures
  52,994 / 19,772 (+1 raw / -4 aligned).  Not a raw win; reverted.
