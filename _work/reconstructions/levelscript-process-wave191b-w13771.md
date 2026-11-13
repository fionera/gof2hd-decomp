# LevelScript::process — wave 191b / w13771: mission-26/5 order + case-102 s2 loop

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13771`, branch
`wave191b-w13771`, base master `403b587d1`, session start HEAD `a6acec168`.

Session start (HEAD): raw **52,845**, aligned **20,180**, size 57,232,
`.text` diffbytes **74,465**, `.ARM.extab` 561, `.rodata` 0, `.data` 0,
`.rel.plt` md5 `289b2a06…`.  `check` green (byte 4508/4524, linked 4508,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0, parity
5370/5370).

Session end: raw **52,811**, aligned **20,082**, `.text` **74,431**,
`.ARM.extab` 561, `.rel.plt` md5 unchanged.

## Landed

| change | raw | aligned | .text | .extab |
|---|---:|---:|---:|---:|
| mission-26 block before mission-5 (src ~1513) | 52,845 → 52,814 (-31) | 20,180 → 20,080 (-100) | 74,465 → 74,434 | 561 |
| + case-102 s2 loop → do-while, `awake` before `setVisible` | 52,814 → **52,811** (-3) | 20,080 → **20,082** (+2) | 74,434 → **74,431** | 561 |

Both are size-exact.  The mission-26/5 swap is semantically neutral (the two
mission ids are mutually exclusive and `inAlienOrbit()` short-circuits on the
`mission == 26` test); it reproduces the original's emitted block layout.
`rangediff.py 13812a 1383f0` drops from 192 to 146 non-equal instructions
(the 55- and 77-instruction clusters at orig `0x1381b8` / `0x138274` vanish).

The case-102 state-2 fighter-staging loop (orig `0x13e030..0x13e088`) is a
do-while: body order `getPosition → setPosition(pl) → awake (virtual +12) →
setVisible(true) → x -= 3000 → i++`, bottom test `cmp r6,#4; bne`.  Our
emitted loop at `0x13eabc` now matches the body order and bottom test; only
the `adds r6,#1` / `subw r4,#4,#3000` schedule order still differs (clang
sinks the increment ahead of the subtract).

## Measured / rejected this session

All size-exact unless noted.  Base 52,845 / 20,180.

| candidate | raw | aligned | size | .text | .extab |
|---|---:|---:|---:|---:|---:|
| e8 alone (case-102 s2 do-while) | 52,842 | 20,182 | 57,232 | 74,462 | 561 |
| e13/e14/e15 (wingman loop: drop `__builtin_expect(...,0)` / `expect(...,1)` / `continue` form) | 52,914 | 20,168 | 57,232 | 74,534 | 561 |
| e16 alone (mission-26 before 5) | 52,814 | 20,080 | 57,232 | 74,434 | 561 |
| e16 + e8 | **52,811** | 20,082 | 57,232 | **74,431** | 561 |
| e16 + e13 | 52,883 | 20,068 | 57,232 | 74,503 | 561 |
| e16 + e8 + e13 | 52,880 | 20,070 | 57,232 | 74,500 | 561 |
| e1 e2 e4 (case-91 s3/s4, mission-106 s2, case-92 s2) | 54,575 | 20,133 | 57,232 | 76,195 | 566 |
| e10 (mission-5 `else if`→`if`) | 55,323 | 20,825 | 57,216 (-16) | 357,903 | 561 |
| e9 (move mission-5 after mission-81) | 55,615 | 27,903 | 57,280 (+48) | 410,488 | 1,329 |
| e9a (same, cleaner) | 55,660 | 24,049 | 57,404 (+172) | 415,517 | 1,635 |
| e3 (case-102 s3 loop body order) | 53,902 | 20,211 | 57,224 (-8) | 352,953 | 561 |
| e6 (mission-89 `awake` before `getPosition`) | 55,170 | 20,177 | 57,240 (+8) | 401,645 | 1,246 |
| e4 (case-92 `m_nState++`) | 54,392 | 20,160 | 57,240 (+8) | 400,867 | 1,246 |

`e13` is a real structural fix (the wingman loop at orig `0x137fae..0x137fc8`
then matches instruction-for-instruction, including the inline `cbz` store
skip), but it shortens the loop by 2 B, shifting the whole tail by -2 and
costing +69 raw.  `e8` is the better raw trade; `e16+e8` is the best raw
combination found.  `e1 e2 e4` remains the best aligned-only candidate
(20,133) but costs +1,730 raw and +5 `.ARM.extab`.

## Findings carried forward

* Original pre-switch mission chain (process, `0x137d5e..0x138c7a`): emitted
  compare order `0 → 1 → 5 → 26 → 81 → 78 → 50 → 89 → 106 → 42`.  Our
  process source order is `0,1,81,78,89,106,{95/99/…},5,26,50,51,42`.
  Mission-26/5 are now swapped (landed); the remaining large reorders
  (5/26 before 81, 50 before 89) are size-fragile.
* The constructor `LevelScript::LevelScript` (`0x135adc`, 8,332 B) carries a
  separate mission-intro chain (`0x135adc..0x137b68`) that is *not* part of
  `process`; the mission-106 intro at src line 202 belongs to it.
* Top remaining aligned clusters in `process` (difflib, HEAD): orig
  `0x13e0e6` (614 insns) vs ours (1); `0x13e962` (9) vs ours (423);
  `0x13d74a` (413) vs ours (1); `0x13dbd8` (354) vs ours (1);
  `0x141028` (324) vs ours (1); `0x13a1ae` (1) vs ours (270).  These are
  whole-block layout mismatches, not local statement order.
* case-157 `>=16001` (`0x13ce0e..0x13ce24`): ours hoists the `m_pGeometry4`
  load (`[r4,#60]`) above the `adds/adc/strd` timer store; the original
  loads it between `adds.w` and `vmov r3,s0`.  Not controllable from a
  local statement reorder (register scheduling); left for a later pass.
* The wingman loop's 2-byte shift comes from the `getWingmen` call site:
  original `ldr r0,[sp,#92]; ldr r0,[r0]; blx` (10 B) vs ours
  `ldr.w r0,[r9]; blx` (8 B) — the `&Globals::status` address is kept in a
  register instead of the stack slot.  Function-wide regalloc, not local.
