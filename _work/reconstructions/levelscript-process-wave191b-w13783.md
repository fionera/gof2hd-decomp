# LevelScript::process — wave 191b / w13783: Chain-A integer `-2*delta`

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13783`, branch
`wave191b-w13783`, base master `5968bc1ac`.

Session start: raw **52,886**, aligned **20,148**, `.text` diffbytes 74,651,
`.ARM.extab` 562, `.rel.plt` md5 `289b2a069f8504d1e8a3919e43915e04`.
Session end: raw **52,883**, aligned **20,141**, `.text` diffbytes 74,648,
`.ARM.extab` 562, `.rel.plt` md5 unchanged (`289b2a06…`).

## Landed

Chain-A short station-departure intro, shared by missions
95/99/109/119/126/133/160/161 (source line ~1430):

```cpp
// before
m_pCamera->translate((float) delta, 0.0f, (float) -delta * 2.0f);
// after
m_pCamera->translate((float) delta, 0.0f, (float) (-2 * delta));
```

Original `0x138eaa` computes the z argument in integer before conversion:

```
138ea6: 2000        movs  r0, #0
138eaa: eba0 0045   sub.w r0, r0, r5, lsl #1   ; -2 * delta
138eb2: ee01 0a10   vmov  s2, r0
138eba: eeb8 1ac1   vcvt.f32.s32 s2, s2
```

The previous `(float) -delta * 2.0f` emitted `negs` + `vmov`/`vcvt` + a
`vadd.f32 s0,s0,s0` (multiply by 2.0f).  The integer form matches the original
instruction-for-instruction through the call.  This is the same change as
`9863bbb01` (w13746), which the coordinator rejected on the pre-`5968bc1ac`
base as +73 aligned for -3 raw; on the current base it is a clean win on every
metric, so it is landed here.

| metric | before | after |
|---|---:|---:|
| raw | 52,886 | **52,883** (-3) |
| aligned | 20,148 | **20,141** (-7) |
| size | 57,232 | 57,232 (exact) |
| `.text` diffbytes | 74,651 | **74,648** (-3) |
| `.ARM.extab` diffbytes | 562 | 562 |
| `.rel.plt` md5 | 289b2a06 | 289b2a06 |

`cmake --build cmake-build-match --target check` = RATCHET PASS
(byte_exact 4508, linked_exact 4508, size 4524/4524, data 843/843, imports 207,
lint CLEAN).

## Rejected / not pursued

- Prologue callee-saved register permutation (`this` fp vs sl, `&status`
  r9 vs r4) is worth at most ~6 bytes and cannot be changed without a size
  change; see `levelscript-process-callee-saved-wave168-w11304.md`.
- EH tail: ours has an extra `String` temporary cleanup (two `~String` calls
  before `_Unwind_Resume` at orig `0x145c60` vs ours).  Touching it risks
  `.ARM.extab`; not attempted.
- Mission-0x51 state 1 (0x138f08) is already structurally faithful; residual
  is base-register allocation only.
- Dispatch chain at `0x138820` (missions 80/91/92/94/102/105/114/125/154/157/158)
  is heavily-worked; not reordered blindly.

## Investigated but rejected (size-fatal)

State 11 (source line 1219) originally read `(float) delta * 3.0f`; the
original at `0x13bd3c` computes the z argument in integer:

```
13bd3c: eb04 0044  add.w r0, r4, r4, lsl #1   ; 3 * delta
13bd44: ee03 0a10  vmov  s6, r0
13bd4c: eeb8 3ac3  vcvt.f32.s32 s6, s6
```

Changing the line to `(float) (delta * 3)` makes the whole 25-instruction
state-11 translate block (`0x13ba8e..0x13bac6` in our build vs
`0x13bd2c..0x13bd64` in the original) instruction-identical modulo the
`fp`/`sl` base register.  But the function then grows to **57,240 B**
(+8; `raw` 52,883 -> 54,832, `aligned` 20,141 -> 20,130, `.text` +32,
`.ARM.extab` 562 -> 1,247, `.dynsym`/`.data.rel.ro`/`.data`/`.ARM.exidx`
all shift).  `(float) (3 * delta)` produces the same +8.  The growth is a
literal-pool/register-allocation cascade elsewhere in the function, not at
the edited site (the edited block itself shrinks by 4 B).  Size is sacred,
so reverted.  Worth revisiting only together with a compensating -8 B
change.

`3.0f` is still used elsewhere (lines 469, 2317), so the growth is not a
dropped literal-pool entry.
