# Radar::draw wave 190 (w13493) — asteroid-lock block: unconditional
# `lockedAsteroid = nullptr` (not `asteroidLockTimer = 0`); raw 6,064 -> 6,058

**Status:** landed, 2026-11-12. Worktree `wt/w13493`, branch `wave190-w13493`,
base `master` `6d390af5f`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave190-w13474.md` (statement-order/regalloc sweep, base
6,065), `-w13476.md` (landmark store order), `-w13456-pass3.md`, `-w13465.md`.

## 0. The change

In the asteroid-lock block (`if (this->lockedAsteroid != nullptr) { ... }`,
`Radar.cpp:1195`) the tail store was reconstructed as
`this->asteroidLockTimer = 0;`.  The original stores through the
`&lockedAsteroid` pointer (sp+124) instead:

```
12fdd0: mov   r6, r4
12fdd2: ldr.w r0, [r6, #16]!   ; r6 = &field_0x10, r0 = field_0x10
12fdd6: cmp   r1, r0           ; r1 = lockedAsteroid
12fdd8: itt   eq
12fdda: moveq r1, #1
12fddc: strbeq.w r1, [r4, #536] ; lockConfirmed = 1
12fde0: ldr   r2, [sp, #124]    ; &lockedAsteroid (offset 0xc)
12fde2: movs  r1, #0
12fde4: str   r1, [r2, #0]      ; lockedAsteroid = nullptr
```

`asteroidLockTimer` (0x19c) is zeroed later, in the `!asteroidHit ||
isDockingToAsteroid()` block (12fe24/12fe2c), which the source already has.
So the source must be:

```cpp
                     if (this->lockedAsteroid == this->field_0x10) {
                         this->lockConfirmed = 1;
                     }
-                    this->asteroidLockTimer = 0;
+                    this->lockedAsteroid = nullptr;
                 }
```

This is a faithfulness fix (the original releases the lock here; it does not
touch the timer) and it removes the 6-byte `ldr/movs/str` to `[r4,#412]`
in favour of the original's 6-byte store through the cached pointer.

| metric | base | after |
|---|---:|---:|
| raw same-address diff | 6,064 | **6,058 (-6)** |
| instruction-index alignment | 2,043 | 2,051 |
| function size | 9,908 / 9,908 | 9,908 / 9,908 |
| instruction count | 3,541 / 3,532 | 3,542 / 3,532 |
| `.text` differing bytes (whole `.so`) | 71,819 | **71,813 (-6)** |
| `.ARM.extab` differing bytes | 575 | 575 (no growth) |
| `.note.gnu.build-id` | 20 | 20 |
| total (text+extab+build-id) | 72,414 | **72,408 (-6)** |
| `.rel.plt` | byte-identical | **byte-identical** |

`RATCHET PASS`: byte_exact 4,507 / linked_exact 4,507 / size 4,524 / data 843 /
imports 207 / lint CLEAN / drift 0 / parity 5,370.

## 1. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6058 aligned= 2051 size=9908/9908 insn=3542/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```
