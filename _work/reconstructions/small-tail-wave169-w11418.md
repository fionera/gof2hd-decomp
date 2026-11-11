# Small tail wave 169 (w11418): Gun / SpaceLounge / Generator re-sweep, no new source win

Status: **0 `src/` changes landed**.  Worktree `wt/w11418`, branch
`wave169-w11418`, base master `ec3ceb07c` (wave 168).  Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).  Scratch
`/dev/shm/w11418`.

Gate on entry and exit: **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, `data_exact 843/843`, imports 207,
lint CLEAN, drift 0, parity 5370/5370, `.rel.plt` byte-identical.

Metrics: raw same-address diff bytes and instruction-index alignment
(`tools/fn_diff.py`).  Baseline residuals re-confirmed exactly:

| symbol | raw | aligned | size |
|---|---:|---:|---:|
| `_ZN3Gun22calcCharacterCollisionEv` | 463 | 57 | 1984 |
| `_ZN11SpaceLounge10drawLoungeEv` | 175 | 41 | 1948 |
| `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 127 | 107 | 1996 |

This note extends `small-tail-wave168-w11318.md`,
`small-tail-wave166-w11117.md` and `small-tail-wave164-w11017.md`; only
new shapes are recorded.

## Generator::createMission — frame 104 vs 96 is a stack-colouring fixed point

Baseline (`volatile int bonus` + `volatile int rem` + local `b`) is
`127/107/1996`, frame **104**.  The original is frame **96** with seven
scalar slots (`galaxy/bonus` share `sp+16`, `&status/rem` share `sp+32`,
`agent/difficulty` share `sp+24`, `&rnd/mission` share `sp+40`, Strings
at 44/56/68/80, canary at 92).  Ours has eight scalar slots
(20/24/28/32/36/40/44/48), `bonus` at `sp+84`, Strings at 48/60/72/88,
canary at 100.  The extra 8 bytes are exactly the dedicated `bonus` and
`rem` slots.

The non-volatile-bonus shape (`int bonus`) recovers the original frame
**96** and the `galaxy/bonus` sharing, but the block-placement pass then
moves the `type == 8` costs check ahead of the `float rem`/`new Mission`
sequence, so raw explodes to 709.  The volatile bonus keeps the block
order but costs the frame.  Every combination tried below is a trade
between those two.

| shape | raw | aligned | size | frame |
|---|---:|---:|---:|---:|
| baseline (`volatile bonus` + `volatile rem` + local `b`) | 127 | 107 | 1996 | 104 |
| `int rem` (non-volatile) | 162 | 97 | 2000 | 104 |
| `int bonus` (non-volatile) | 709 | 89 | 2000 | **96** |
| both non-volatile | 181 | 116 | 2000 | 104 |
| `volatile int bonus = 0` + `if (!= 12)` | 686 | 171 | 1996 | 104 |
| `int rem` + `volatile int *remp = &rem`, reads via `*remp` | 127 | 107 | 1996 | 104 |
| `int rem` + `int *remp = &rem`, reads via `*remp` | 162 | 97 | 2000 | 104 |
| `volatile int rem` + `int *remp = (int*)&rem`, reads via `*remp` | 649 | 119 | 2000 | 104 |
| `const volatile int rem` | 127 | 107 | 1996 | 104 |
| `volatile int b` | 618 | 74 | 2004 | 104 |
| `int bonus` + `volatile int *bonusPtr = &bonus` | 127 | 107 | 1996 | 104 |
| `int bonus` + `*(volatile int*)&bonus` stores/read | 127 | 107 | 1996 | 104 |
| `int bonus` + `__builtin_expect(type == 8, 0)` | 704 | 107 | 2000 | 96 |
| `int bonus` + `__builtin_expect(type == 8, 1)` | 1769 | 187 | 1996 | 96 |
| drop the `race/type` `__builtin_expect` | 1567 | 342 | 1956 | — |
| `__builtin_expect(race < 4 && type == 13, 0)` | 1771 | 596 | 1988 | — |
| `0x4801U & (1U << type)` operand order | 127 | 107 | 1996 | 104 |
| `(uint32_t) type <= 14` | 127 | 107 | 1996 | 104 |
| `bool isRace13 = ...` local | 127 | 107 | 1996 | 104 |
| `type == 13 && race < 4` order | 127 | 107 | 1996 | 104 |

The `bhi.n 0x1d0` (orig) vs `0x242` (ours) residual is a value-range
fold: when `type > 14` the original still branches to the `race < 4 &&
type == 13` test, ours folds it away.  None of the condition spellings
above changes the fold.

## SpaceLounge::drawLounge — `pad2` fixes the clamp but shifts global RA

The original right-edge clamp is `addgt.w r1,r6,sl; subgt r5,r0,r1`
(`x = (w-inset) - (width + 2*pad)`, reusing the `sl = pad*2` already
live for `top`).  Introducing an explicit `pad2 = pad + pad` local does
recover that exact clamp (`addgt.w r1,r6,sl; subgt.w r9,r0,r1`), but the
extra live value moves `pad` from `r9` to `fp` and cascades through the
whole function: 179 structural diff lines instead of 41.

| shape | raw | aligned | size |
|---|---:|---:|---:|
| baseline (`+ (-width - pad - pad)`) | 175 | 41 | 1948 |
| `(w - inset) - (width + pad + pad)` | 176 | 42 | 1948 |
| `int pad2 = pad + pad` early, `top` uses `pad2` | 445 | 54 | 1948 |
| `int pad2 = pad + pad` late (inside the clamp arm) | 445 | 54 | 1948 |
| `const int pad2` early | 445 | 54 | 1948 |
| `pad2` early, `top` still uses `pad + pad` | 445 | 54 | 1948 |

The clamp is correct in the `pad2` shapes but the `pad` register tie is
the dominant cost.  Baseline `175/41` remains the raw optimum.

## Gun::calcCharacterCollision — vector-slot / `i*3` RA tie

The residual is the `i*3` index in `r4` (orig) / `r5` (ours) plus the
`position`/`enemyPosition` address materialisation registers being
swapped (`add r4,sp,#44` + `add r5,sp,#104` vs `add r4,sp,#104` +
`add r5,sp,#44`).  The stack slots themselves are identical
(`position` at `sp+44`, `enemyPosition` at `sp+104`, frame exact).

| shape | raw | aligned | size |
|---|---:|---:|---:|
| baseline | 463 | 57 | 1984 |
| `enemyPosition` declared first | 546 | 101 | 1984 |
| `velocity` declared first | 478 | 80 | 1984 |
| `position`, `enemyPosition`, `velocity` order | 508 | 81 | 1984 |
| `int i` loop variable | 463 | 57 | 1984 |

Pure greedy-RA tie; no source order swaps the `r4`/`r5` pair without
also permuting the vector address materialisations.

## Conclusion

All three residuals are stack-colouring / block-placement / greedy-RA
ties at this toolchain.  No size-exact raw win was found; the tree is
left clean at the wave-168 baseline (`byte 4507`, `linked 4507`).
