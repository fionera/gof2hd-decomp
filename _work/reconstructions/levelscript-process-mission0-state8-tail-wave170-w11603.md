# LevelScript::process — wave 170 / w11603: mission-0 state 8 + shared state tail

Target `_ZN11LevelScript7processEi` (original `0x137d18`, 57,232 B = 0xdf90).
Worktree `wt/w11603`, branch `wave169-w11603`, base master `50716a763`.
Scratch `/dev/shm/w11603/`.

## Baseline (rebuilt from the clean tree)

```
process raw diffbytes    53,932
process aligned insns    17,900
orig / ours size         57232 / 57232   (size-exact)
gate byte 4507 / linked 4507 / size 4524 / data 843 / imports 207
```

## Original blocks (objdump, image base 0x10000)

State 8 `>= 4001` arm, `0x13d194..0x13d1da`:

```
movs r0,#9; movs r1,#0
str.w r0,[sl,#28]          ; m_nState = 9
movs r0,#0
strd r0,r0,[r6]            ; m_nScriptTimerA = 0
movs r2,#0; ldr r0,[sl,#20]; blx setRumblePercentage
... TransformGetTransform(m_pGeometry5->transform)->SetAnimationState(0)
... TransformGetTransform(m_pGeometry5->transform)->SetAnimationState(3)
b.w 0x137f7c
```

There is **no** `m_pGeometry5->setPosition(player->getPosition())` in this arm.
The duplicate is a real extra call (it also accounts for the +1
`AEGeometry::setPosition(Vector)` / +1 `PlayerEgo::getPosition()` call deltas).

State 15 / default tail, `0x13d4c6..0x13d4f0`:

```
ldr r0,[sp,#72]; ldr r0,[r0,#4]; ldr r0,[r0,#88]; blx RadioMessage::isOver
cmp r0,#0
beq.w 0x13f902             ; not over -> switch default (state-16 check)
... startFade(true,255,5000)
movs r0,#16
str.w r0,[sl,#28]          ; m_nState = 16
b.w 0x137f7c               ; over -> shared active tail
```

The switch default at `0x13f902` is `ldr r1,[sl,#28]; cmp r1,#16; bne 0x137f7c`,
then the state-16 body.  So the over path must leave the switch (to the shared
active tail); the not-over path falls into the default handler, which re-checks
`m_nState == 16` and skips.

## Landed shape

* Remove the state-8 duplicate `m_pGeometry5->setPosition(player->getPosition())`.
* Make state 15 fall through to a `case 16:` handler only on the over path;
  the not-over path `break`s to the shared tail.  The `case 16:` body keeps the
  `m_nState == 16` guard equivalent to the original default check (written as
  `if (m_nState != 16) break;`).

This is size-exact and semantically equivalent to the original: the over path
sets `m_nState = 16` and runs the state-16 handler; the not-over path exits to
the shared active tail.  The handler's `isFading()` check is true immediately
after `startFade`, so the same-frame handler is a no-op in the over case, as in
the original's next-frame default.

Measured (full link, `tools/fn_diff.py`):

| shape | raw | aligned | size |
|---|---:|---:|---:|
| baseline (HEAD) | **53,932** | **17,900** | 57,232 |
| state-8 duplicate removed only (A) | 54,661 | 17,894 | 57,208 |
| A + `case 16:` without `m_nState` guard | 54,498 | 18,221 | 57,232 |
| **landed (A + guarded `case 16:` fallthrough)** | **54,462** | **18,219** | **57,232** |
| A + `default:` instead of `case 16:` | 55,409 | 20,231 | 57,276 |
| A + `goto` to after-switch label | 55,286 | 21,265 | 57,180 |
| A + state-16 check moved to top of mission-0 | 55,778 | 23,147 | 57,428 |

`check` green: byte_exact 4507 / linked_exact 4507 / size 4524 / data 843 /
imports 207, lint CLEAN, RATCHET PASS.

## `.rel.plt` caveat (important)

The landed shape rotates six `.rel.plt` entries (`PlayerStation::setVisible`
moves from the last slot of the `0x2132d0..0x2132e4` group to second).  The
first `PlayerStation::setVisible` reference moves from `0x13da1e` (original) to
`0x13cf98` because the `case 16:` block changes MachineBlockPlacement of the
mission-125 state-7 block.  The gate's `sodiff` only checks the import set, so
`check` still passes; a byte-identical `.rel.plt` requires either:

* a source shape that keeps the mission-125 state-7 block after mission-0
  state 9 (not found in the variants measured here), or
* a duplicate PLT anchor for the earlier symbols (would change code size).

Variants measured with `.rel.plt` SAME but wrong size:

| shape | raw | size | `.rel.plt` |
|---|---:|---:|---|
| A + `m_pParticleGeom1->setPosition` before the `currentTime >= 4001` check | 54,625 | 57,216 | SAME |
| A + that move + setRumble last | 54,623 | 57,216 | SAME |

The `movepg1` order is a real faithful fix (original `0x13d9ec` calls
`m_pParticleGeom1->setPosition(dir * 10000.0f)` before the `currentTime` test),
but it is -16 B and cannot land alone (size/`byte_exact` gate drops).

## Measured non-wins (all on the landed base unless noted)

| change | raw | size |
|---|---:|---:|
| state-8 duplicate removed only | 54,661 | 57,208 |
| `case 16` without `m_nState` guard | 54,498 | 57,232 |
| `default:` handler | 55,409 | 57,276 |
| `goto` after-switch label | 55,286 | 57,180 |
| state-16 check at top of mission-0 | 55,778 | 57,428 |
| state-8 statement permutations (R/A0/A3) | 54,462..55,215 | 57,216..57,232 |
| state-16 store permutations | 54,498..54,500 | 57,232 |
| prologue declaration permutations | 54,498..55,349 | 57,232..57,236 |
| `__builtin_expect` sweep (state 4/5/6/7/8/12/13/14/15/16) | 54,494..55,556 | 57,192..57,356 |
| move `case 16` before cases 6..15 | 55,283..55,440 | 57,204..57,252 |
| case-6 translate before timer add | 54,494 | 57,232 |
| `initStreamOutPosition = 0` / no store | 54,999 / 55,202 | 57,224 / 57,216 |

The raw metric is layout-dominated: the state-8 arm itself is now
instruction-for-instruction identical to the original, but the literal pool
that clang places at `0x13d13c` in ours (the original has no pool there) shifts
the whole mission-0 tail by +0x5c.  The landed shape is the best size-exact,
semantically faithful shape found; it is 336 B better raw than the unguarded
`case 16` shape and 385 B better than the state-8-only shape.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
cmake --build cmake-build-match --target check -j2
```
