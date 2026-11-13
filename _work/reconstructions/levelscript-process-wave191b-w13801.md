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
