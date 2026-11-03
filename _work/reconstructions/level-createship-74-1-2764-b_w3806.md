# Level::createShip — 74.1%, 2764 B (wave88-w3806)

**Status:** research note, 2026-11-03. Worktree `wt/w3806`, branch
`wave88-w3806`, base `41804126`. No source change landed: no tested source
shape beats the current 74.1% fuzzy without a gate regression. This commit
contains only this note.

Target: `_ZN5Level10createShipEiiiP8Waypointbb` (original 2764 B, `0xc0214`).

## Baseline

`cmake --build cmake-build-match --target check -j8`:

```text
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`createShip`: **74.1%**, 2764 B both sides, not byte/linked exact. `.text`
`0x1337c8`, lint `CLEAN`, drift 0.

## Root cause: spawn `x` is sunk past `setEmpData`

The original computes the three spawn integers **before** `Player::setEmpData`:

```text
c03c8  ldr  r2,[sp,#48]   ; wy
c03ca  ldr  r3,[sp,#32]   ; jitterY
c03cc  ldr  r0,[sp,#44]   ; wx
c03ce  ldr  r1,[sp,#36]   ; jitterX
c03d0  add  r2,r3
c03d2  ldr  r3,[sp,#40]   ; wz
c03d4  ldr  r4,[sp,#28]   ; jitterZ
c03d6  add  r0,r1
c03d8  movw r1,#20000
c03dc  sub.w fp,r0,r1    ; x -> fp
c03e0  add  r3,r4
c03e2  subs r4,r2,r1     ; y -> r4
c03e4  sub.w r8,r3,r1    ; z -> r8
c03e8  mov  r0,r5
c03ea  mov  r1,sl
c03ec  mov  r2,r9
c03ee  blx  Player::setEmpData
```

Our build declares `int x`, `float z`, `float y` before the `try`, but clang
sinks the `x` computation **after** the call:

```text
c03c8  ldr  r0,[sp,#44]   ; wz
c03ca  movw r4,#20000
c03ce  ldr  r1,[sp,#36]   ; jitterZ
...
c03dc  vmov s0,r1
c03e0  vcvt s18,s0        ; y
c03e4  vmov s0,r0
c03e8  vcvt s16,s0        ; z
c03ec  mov  r0,r5
c03f2  blx  Player::setEmpData
c03f6  ldrd r1,r0,[sp,#28] ; x computed only now
c03fc  ldr.w sl,[sp,#56]   ; this
c0402  sub.w r4,r0,r4      ; x -> r4
```

This one scheduling difference cascades into the whole fixed-ship branch:

| value | original | ours |
|---|---|---|
| `x` across call | `fp` | computed after call, `r4` |
| `ship` | `r9` | `fp` |
| `volumes` | `fp` | `r9` |
| `race` | `r6` | `r4` |
| `type` | `r4` (live across ctor) | `r2`, reloaded from `[sp,#52]` |
| `this` | `r6` | `sl` |
| hull-box `x` | `sl` | `r6` |
| hull-box `y` | `r8` | `r8` |
| hull-box `z` | `r4` | `sl` |
| `0.0f` stack arg | `r6` (loop-invariant) | `r0` (rematerialised) |

The hull-box sequence is the visible symptom: the original has
`mov r1,sl / mov r2,r8 / mov r3,r4` and `strd r6,r0,[sp]`, ours has
`mov r1,r6 / mov r2,r8 / mov r3,sl` and `movs r0,#0 / str r0,[sp]`.

The previous note's `NOHINT` experiment (89.0%, +4 B) is consistent with this:
suppressing the hint on the `x` copy makes the allocator choose `sl` for `x`
and `r4` for `z`, exactly the original loop allocation.

## Source-shape experiments (all measured with the official verifier)

| variant | size | fuzzy |
|---|---|---|
| current (`int x`, `float z`, `float y`, try/catch trap) | 2764 | **74.1** |
| `int x, y, z` | 2756 | 61.4 |
| `float x, y, z` | 2748 | 64.4 |
| `float x`, `int y, z` | 2784 | 67.0 |
| `int x`, `float y`, `int z` | 2764 | 72.3 |
| `int x`, `int y`, `float z` | 2764 | 72.6 |
| remove try/catch | 2776 | 70.3 |
| empty catch | 2772 | 65.5 |
| `catch (...) { throw; }` | 2740 | 49.5 |
| `catch (int)` | 2772 | 66.1 |
| `catch (...) { __builtin_unreachable(); }` | 2760 | 66.2 |
| `int mp = missionPtr` (mixed test) | 2764 | 67.3 |
| `int mp = missionPtr` (both compares) | 2760 | 67.0 |
| `int mp = missionPtr` inside fighter branch | 2764 | 74.1 |
| jitter declaration order `YZX` | 2764 | 74.1 |
| jitter declaration order `XZY/ZXY/ZYX` | 2764 | 73.9–74.0 |
| waypoint declaration `wx,wz,wy` | 2764 | **74.1** |
| waypoint declaration `wx,wy,wz` | 2764 | 73.9 |
| waypoint declaration `wz,...` | 2764 | 72.6–72.9 |
| `switch (shipClass)` | 2760 | 38.8 |
| two separate `if (shipClass)` | 2712 | 34.2 |
| fixed branch first | 2760 | 33.6 |
| `PlayerFixedObject(type, race, ...)` | 2764 | 74.0 |
| explicit `float fx = x` / `float zero = 0.0f` | 2764 | 74.1 |
| `BoundingAAB *box` local | 2764 | 74.1 |
| `Level *self = this` alias | 2764 | 74.1 |
| remove `kiFighter` local | 2764 | 74.1 |
| use `ship` directly in fixed branch | 2764 | 74.1 |
| per-TU `-mllvm -disable-vreg-hints` | 2748 | 64.3 |
| per-TU `-mllvm -enable-machine-sink=false` | 2748 | 64.3 |
| per-TU `-mllvm -disable-sink` | 2748 | 64.3 |

The current source is a local optimum. The `try/catch { __builtin_trap(); }`
anchor is what keeps the size at 2764 and the early allocation matching; it is
not the original EH shape (the original landing pad is cleanup-only:
`_ZdlPv` + `_Unwind_Resume`, no `__cxa_begin_catch`), but removing it costs 12 B
and 3.8 fuzzy points.

## Semantic bug left in place

The current source calls

```cpp
new (fixed) PlayerFixedObject(race, type, player, nullptr, x, y, z);
```

but the original passes `type` first and `race` second:

```text
c0426  ldr  r4,[sp,#52]   ; type
c042a  ldr  r6,[sp,#60]   ; race
c0430  mov  r1,r4         ; arg1 = type
c0432  mov  r2,r6         ; arg2 = race
```

`PlayerFixedObject` stores its second parameter into `faction()` (offset 0x28),
so the original source must be `PlayerFixedObject(type, race, ...)`. Fixing the
argument order is semantically correct but scores **74.0%** (2764 B), a 0.1
fuzzy regression, so it was not landed under the "real improvement" rule.

## Remaining work

The single remaining root cause is the `x` live-range start: the original keeps
the spawn `x` integer live across `Player::setEmpData` in `fp`; clang 7 `-Oz`
sinks it after the call from the current source. A source shape that pins that
integer across the call (without adding instructions) should recover the
original `r9=ship, fp=volumes, r6=race, r4=type, sl=x, r8=y, r4=z` allocation
and the ~89% `NOHINT` result.
