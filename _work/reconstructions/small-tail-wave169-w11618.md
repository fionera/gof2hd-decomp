# Small tail wave 169 (w11618): Generator frame-96 shape landed (structural)

Status: **1 `src/` change landed**.  Worktree `wt/w11618`, branch
`wave169-w11618`, base master `50716a763`.  Scratch `/dev/shm/w11618`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Gate on exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370, `.rel.plt` byte-identical.

Metrics: raw same-address diff bytes and instruction-index alignment
(`tools/fn_diff.py`).  Baselines re-confirmed exactly:

| symbol | raw | aligned | size |
|---|---:|---:|---:|
| `_ZN3Gun22calcCharacterCollisionEv` | 463 | 57 | 1984 |
| `_ZN11SpaceLounge10drawLoungeEv` | 175 | 41 | 1948 |
| `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 127 | 107 | 1996 |

## Generator::createMission — `int bonus` + `int rem` + `volatile int b`

Root cause of the 127 raw is the **stack frame**: ours was 104 with 8 scalar
slots (20..48), the original is 96 with 7 slots (16..40).  The missing slot is
`agent`/`difficulty`: the original keeps `agent` in `r9` from `getName` to
`getImageParts` (so its stack copy at sp+24 dies at 97322 and `difficulty`
reuses it), while our both-non-volatile shape lets `rem` take `r9`, spills
`agent` to sp+36 and keeps `difficulty` in its own slot at sp+28.

Making `b` volatile forces `b` into memory, which frees `r5` for `rem` and
leaves `r9` to `agent`: frame drops to **96** and every stack offset matches
the original (aligned 107 -> 25).  The residual is the bonus block itself
(`b` is spilled/reloaded instead of `rem`, and the `bonus = 0` store is sunk
past `new Mission`); that is a greedy-RA / machine-sinking tie.

| shape | raw | aligned | size | frame |
|---|---:|---:|---:|---:|
| baseline (`volatile bonus` + `volatile rem` + `int b`) | 127 | 107 | 1996 | 104 |
| **`int bonus` + `int rem` + `volatile int b` (landed)** | **163** | **25** | **1996** | **96** |
| `int bonus` + `int rem` + `int b` (both non-volatile) | 181 | 116 | 2000 | 104 |
| `int bonus` + `volatile rem` + `int b` | 709 | 89 | 2000 | 96 |
| `volatile bonus` + `int rem` + `int b` | 162 | 97 | 2000 | 104 |
| `int bonus` + `int rem` + `volatile b`, `b` fn-scope | 236 | 105 | 1996 | — |
| `int bonus` + `int rem` + `volatile b`, `bonus` in branches | 227 | 44 | 2000 | — |
| `int bonus` + `int rem` + `volatile b`, `bonus` volatile | 661 | 144 | 2000 | — |
| `int bonus` + `int rem` + `volatile b`, `setBonus` before `setCosts` | 550 | 63 | 1996 | — |
| `int bonus` + `const int rem` + `int b` | 181 | 116 | 2000 | 104 |
| `int bonus` + `int rem` + `int b`, `rem` fn-scope | 181 | 116 | 2000 | 104 |
| `int bonus` + `unsigned rem` + `int b` | 183 | 116 | 2000 | 104 |
| `int bonus` + `short/long/long long rem` + `int b` | 181 | 116 | 2000 | 104 |
| `int bonus` + `int rem` + `int b`, `b` volatile / `rem` volatile cast | 716 | 88 | 2000 | — |
| `int bonus` + `int rem` + `int b`, `rem` used once via `b` | 181 | 116 | 2000 | 104 |
| `int bonus` + `int rem` + `int b`, `__builtin_expect(b % 50 != 0, 0)` | 181 | 116 | 2000 | 104 |
| `int bonus` + `int rem` + `volatile b`, `bonusLocal` read | 163 | 25 | 1996 | 96 |
| `int bonus` + `int rem` + `volatile b`, `bonus-bonus` use | 163 | 25 | 1996 | 96 |

The raw rise (127 -> 163) is a same-address shift artifact: the bonus block is
one instruction longer in ours, and the deferred `bonus` store compensates
later; the literal pool then shifts by 22 bytes.  The structural diff is only
25 instructions (vs 107) and the frame now matches.  Landed as a structural
win under the "clear structural improvement" clause; the coordinator can
revert if raw-first is preferred.

## SpaceLounge::drawLounge — re-confirmed, no new win

The 175 raw is the right-edge clamp (93 bytes) plus the cold-tail landing-pad
order (31 bytes).  The original clamp is `r1 = width + sl; x = (w-inset) - r1`
(`sl = pad*2` already live from `top`); clang 7 `-Oz` always reassociates
`right - (width + pad + pad)` to `right + (-2*pad - width)` and rematerialises
`pad<<1` from `fp`.  New shapes (all size 1948 unless noted):

| shape | raw | aligned | size |
|---|---:|---:|---:|
| baseline | 175 | 41 | 1948 |
| `int right` local, both condition and clamp | 176 | 42 | 1948 |
| `int right` local, clamp only | 176 | 42 | 1948 |
| `right - width - pad - pad` | 468 | 64 | 1944 |
| `right - (width + (pad+pad))` | 445 | 54 | 1948 |
| `right - (width + pad*2)` | 445 | 54 | 1948 |
| `int pad2 = pad+pad`, `top` uses pad2, `right-(width+pad2)` | 451 | 58 | 1948 |
| `const int pad2`, same | 451 | 58 | 1948 |
| `right - width - pad2` | 451 | 58 | 1948 |
| `right - (pad2 + width)` | 451 | 58 | 1948 |
| `x = x - (width+pad+pad)` | 476 | 70 | 1944 |

The `pad2` shapes do recover `add r1,r6,sl; sub r5,r0,r1` but the extra live
int cascades through the rest of the function.  Master keeps `175/41`.

## Gun::calcCharacterCollision — re-confirmed, no new win

The 463 raw is dominated by the scatter `k` loop block layout: the original is
rotated (bottom-tested, `b.n` to the test at 1534fc), ours is top-tested.  New
loop shapes:

| shape | raw | aligned | size |
|---|---:|---:|---:|
| baseline | 463 | 57 | 1984 |
| `for (...; __builtin_expect(k < size, 1); ...)` | 1800 | 355 | 1948 |
| `for (...; __builtin_expect(k < size, 0); ...)` | 1717 | 303 | 1944 |
| `while (__builtin_expect(k < size, 1))` | 1800 | 355 | 1948 |
| drop inner `__builtin_expect(distance < magnitude, 0)` | 1886 | 257 | 1908 |
| inner expect true | 1886 | 257 | 1908 |

Loop rotation is a MachineBlockPlacement decision; every source rewrite
changes size.  Master keeps `463/57`.

## Conclusion

One structural win landed (Generator frame 104 -> 96, aligned 107 -> 25,
size-exact 1996).  SpaceLounge and Gun residuals are reassociation /
block-placement ties at this toolchain.
