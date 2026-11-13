# LevelScript::process — wave 191b / w13769: mission-0x4e state order

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13769`, branch
`wave191b-w13769`, base master `a4a497605`.

## Session baseline

| metric | value |
|---|---:|
| raw | 53,419 |
| aligned | 20,308 |
| size | 57,232 / 57,232 |
| `.text` diff bytes | 74,990 |
| `.ARM.extab` diff bytes | 561 |
| `.ARM.exidx` / `.rodata` / `.rel.plt` | 0 / 0 / 0 |

## Landed: mission-0x4e substate statement order (three edits)

All three are verified directly against the Thumb disassembly of the original.

1. **s12 @0x13b7f2** — the original stores `m_nState = 13`
   (`str r0,[sl,#28]`, `r0=13`, `0x13b8b8`) *before* branching to the shared
   epilogue (`b.w 0x144f5a`).  Source had `return m_bCinematicBreak != 0;`
   ahead of the store.
2. **s10 @0x13b948** — the original emits the camera `translate`
   (`0x13b984`) before `m_nScriptTimerA += delta` (`0x13b990`).
3. **s1 @0x1383a0** — the original emits `m_pCamera->setTarget(...)`
   (`0x1383b2`) before the `TransformGetTransform` / `SetAnimationState`
   pair (`0x1383d2`).

Measured, each step separately:

| step | raw | aligned | `.text` | `.ARM.extab` | size |
|---|---:|---:|---:|---:|---:|
| baseline | 53,419 | 20,308 | 74,990 | 561 | 57,232 |
| s12 store before return | 53,057 | 20,292 | 74,628 | 561 | 57,232 |
| s10 translate before timer | 53,056 | 20,292 | 74,627 | 561 | 57,232 |
| s1 setTarget before transform | 53,421 | 20,293 | 74,992 | 561 | 57,232 |
| **final (all three)** | **53,056** | **20,292** | **74,627** | **561** | **57,232** |

Net: raw **-363**, `.text` **-363**, aligned **-16**, size exact,
`.rel.plt` still byte-identical, `.ARM.extab` unchanged at 561.

## Verified-but-already-matching mission-0x4e substates

s2 `0x13ba5c`, s3 `0x13b8dc`, s4 `0x13baba`, s5 `0x13b8c0`, s6 `0x13bb7c`,
s7 `0x13bc18`, s8 `0x13baf2`, s9 `0x13bc34`, s11 `0x13bd2c` were re-derived
from the Thumb this session and already match the source statement-for-
statement (including the inline `m_tempVec.x/.y/.z +=` adds at
`0x13be3c..0x13be6e`, which the compiler emits as three `vadd.f32` +
`vstr`, not a `Vector::operator+=` call).

## Not landed (no measured win)

* s2 `camera2->setPosition(20000,5000,30000)` shape: the original loads the
  three literal-pool floats and tail-branches to the shared
  `setPosition(float,float,float)` call site (`b.w 0x13c888`); the current
  source already produces that shape.
* s6/s8 `(float) delta * 0.3f + (float) m_nScriptTimerA`: the original
  computes the float delta term first, then `__aeabi_l2f`, then `vadd.f32`;
  the source order already matches.
