# Near-exact residual bundle — wave 191 (w13500): source-shape sweep, no win landed

Worker `w13500`, branch `wave191-w13500`, base master `f325e2d87`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch `/home/debian/GalaxyOnFire2/scratch/w13500`.

**Status: no `src/` change landed.**  The committed bodies remain the best
measured shapes.  Gate re-confirmed at HEAD: `RATCHET PASS` — byte_exact
4507/4524, linked_exact 4507/4524, size 4524/4524, data 843/843, imports
207, stub/missing/extra 0, lint CLEAN, drift 0, parity 5370/5370.
Baseline re-measured exactly:

| function | raw | aligned | size |
|---|---:|---:|---:|
| `Generator::createMission` | 3 | 3 | 1996/1996 |
| `Hud::draw` | 19 | 6 | 9332/9332 |
| `ModStation::ModStation` (C1/C2) | 11 | 5 | 1008/1008 |
| `MGame::OnUpdate` | 10 | 112 | 11440/11440 |
| `MenuTouchWindow::OnTouchBegin` | 28 | 16 | 1896/1896 |
| `Status::missionCompleted` | 97 | 54 | 1432/1432 |
| `Gun::calcCharacterCollision` | 455 | 93 | 1984/1984 |
| `SpaceLounge::drawLounge` | 138 | 87 | 1948/1948 |

Every cell below is a full `link_so` + `tools/fn_diff.py` measurement
(`-j2`, `ulimit -v 12000000`).  All variants not listed were size-exact
unless noted; none beat the committed raw count.

## 1. `Generator::createMission` (raw 3)

Two independent bytes: the `bhi.n` at +0x1a2 (mask-check false edge targets
the race-check block in the original, the merge block in ours) and the
`rem` volatile alloca at sp+44 vs sp+32.

### 1.1 `rem` slot

The IR `Merge disjoint stack slots` dump shows fi#2 (the 4-byte `volatile
int rem`) merged into fi#5, the 12-byte `setTargetSystemName` String
temporary.  Extending the alloca's lifetime (function-top or a scope from
the bonus block to the end of the loop) prevents the merge but grows the
frame to 104 and shifts every sp offset:

| variant | raw | size |
|---|---:|---:|
| base (`volatile int rem = remVal`) | **3** | 1996 |
| `volatile int rem; rem = remVal;` | 3 | 1996 |
| `volatile int *remp = &remVal; int remCopy = *remp;` | 3 | 1996 |
| `volatile int &rem = remVal;` | 3 | 1996 |
| `volatile int rem[1]` / `rem[2..4]` | 3 | 1996 |
| named `String sysName` before the loop / `const String &` | 3 | 1996 |
| `String sysName` before the bonus block | 763 | 2028 |
| `volatile int rem` at function top | 598 | 2000 |
| scope from bonus block to end of loop | 85 | 1996 (frame 104) |
| `(void) rem` after the bonus block | 593 | 2000 |
| plain `int rem` | 97 | 1996 (frame 104) |

### 1.2 `bhi` fold

The mask-check false edge is retargeted by the second JumpThreading pass
because LVI knows `type >= 15` implies `type != 13`.  Every form that keeps
the 1996 B size still folds (raw 3); every form that defeats the fold
changes the size:

| variant | raw | size |
|---|---:|---:|
| base `__builtin_expect(race < 4 && type == 13, 1)` | **3** | 1996 |
| `__builtin_expect(type == 13 && race < 4, 1)` | 3 | 1996 |
| `__builtin_expect((race < 4) & (type == 13), 1)` | 3 | 1996 |
| `__builtin_expect(!!(...), 1)` / `... != 0` / ternary | 3 | 1996 |
| `__builtin_expect(race < 4, 1) && __builtin_expect(type == 13, 1)` | 3 | 1996 |
| `(type + 1) == 14` / `(type - 1) == 12` / `(type + 0) == 13` | 3 | 1996 |
| `uint32_t typeCheck = type;` (non-volatile copy) | 3 | 1996 |
| `__builtin_memcpy(&typeCheck, &type, 4)` / union / struct box | 3 | 1996 |
| `*(uint32_t *) &type == 13` | 3 | 1996 |
| `(type & 0x1f) == 13` / `(type << 1) == 26` / `(type \| 13) == 13` | 1773 | 1964 |
| `volatile uint32_t typeCheck = type;` | 1774 | 1964 |
| `*(volatile uint32_t *) &type == 13` | 1913 | 2012 |
| `volatile uint32_t type` | (w190) | 2000 |
| no `__builtin_expect` | 1707 | 1984 |

## 2. `Hud::draw` (raw 19)

The residual is the damage-direction block; the committed anchor
(`if (hitFlags & 0x24) Status **volatile p = &Globals::status;` with the
field store unconditional) is load-bearing for the whole function's
allocation.  Removing it gives the exact original block but 9328 B / raw
7340.  Every attempt to restore the frame with a pad elsewhere re-colours
the function:

| variant | raw | size |
|---|---:|---:|
| base (anchor, unbraced) | **19** | 9332 |
| `if (hitFlags & 0x24) { anchor }` + unconditional field store | 19 | 9332 |
| braces + anchor inside | 7092 | 9336 |
| braces, no anchor | 7340 | 9328 |
| anchor before / after / between the two ifs | 7125–7135 | 9336 |
| anchor in the `& 0x18` block | 7076 | 9336 |
| `volatile int pad = 0;` after the damage block | 2916 | 9332 |
| `volatile int pad = hitFlags;` after the block | 3027 | 9332 |
| `volatile int pad[2]` before the block | 2892 | 9332 |
| ternary `field_0x4b8 = (hitFlags & 0x24) ? 300 : field_0x4b8` | 2918 | 9332 |
| 4-byte pad at 100+ positions in the function | best 3194 | 9332 |

## 3. `ModStation::ModStation` (raw 11)

The original's farKey `MatrixSetRotation` x argument is `pitchBits`
(the same value as the nearKey x), while the committed source passes
`pitch` (= `yawBits`).  The original's `mov r4,r2` is scheduled immediately
before the nearKey `blx`; every faithful `farKey = yaw` spelling schedules
it early and re-colours the table pointers:

| variant | raw | size |
|---|---:|---:|
| base (farKey uses `pitch` = yawBits) | **11** | 1008 |
| farKey uses `yaw` / `*(float *) &pitchBits` (faithful) | 43 | 1008 |
| same with `const float`, reversed decls, one-line decls | 43 | 1008 |
| `float farYaw = yaw;` after the nearKey call | 43 | 1008 |
| `int farBits = pitchBits;` | 43 | 1008 |
| `volatile float farYaw = yaw;` | 619 | 1016 |

## 4. `MGame::OnUpdate` (raw 10)

The 10 bytes are 4 B in the m122 64-bit compare (`ldrd r2,r3,[r2,#256]`
vs `[r4,#256]`) and 6 B in the m122 body (`ldr.w r0,[r9]` vs
`mov r0,r4; str r4,[sp,#120]`).

| variant | raw | size |
|---|---:|---:|
| base (`Status *st` + `Status *volatile dummy = st` + `Level *level`) | **10** | 11440 |
| `Status *volatile st = Globals::status` | 15 | 11440 |
| `Level *level` before the ccm (wave-163 shape) | 16 | 11440 |
| no `st` / no dummy | 5196 | 11392 |
| `Status *st` without dummy | 918 | 11436 |
| `Status *st` used in the tail too | 918 | 11436 |
| `volatile int dummy = 0` / `Status **volatile dummy = &Globals::status` | 355 | 11440 |
| m122 compare `3600000 < ...` / `*(long long *)` cast | 10 | 11440 |
| m122 compare `... > field + 3600000` | 4471 | 11436 |
| m122 compare `... - 3600000 > field` | 349 | 11440 |

## 5. `MenuTouchWindow::OnTouchBegin` (raw 28)

The residual is the Greedy-RA live-range/rematerialisation tie for
`&Globals::layout` (`%99`) coupled to the cached layout value (`%29`).
All 16 ref/direct combinations of the four case-15 field uses give the
same 28/16/1896; the current source is a local optimum.

| variant | raw | size |
|---|---:|---:|
| base (case-15 `Layout &layout`, mixed ref/dir) | **28** | 1896 |
| all 16 ref/dir combinations | 28 | 1896 |
| case-15 `Layout *lp = Globals::layout` | 316 | 1888 |
| all-direct (`Globals::layout->` everywhere) | 362 | 1892 |
| function-top `layout` pointer only | 1776 | 1908 |

## 6. `Status::missionCompleted` (raw 97)

The original's case-10 body calls `getProductionGoodIndex`,
`getProductionGoodAmount`, `getCargo` (in that order) and re-queries
`getProductionGoodIndex` in the `hasEquipment` argument.  The committed
source calls index/cargo/amount, caches the index, and inserts a dead
`(void) m->getStatusValue()` to hold the 1432 B size.  Every faithful
re-query spelling either grows the body by 4 B or re-colours the
registers:

| variant | raw | size |
|---|---:|---:|
| base (index/cargo/amount, cached index, dead getStatusValue) | **97** | 1432 |
| index/amount/cargo + re-query, no dead call | 696 | 1436 |
| index/cargo/amount + re-query, no dead call | 402 | 1432 |
| index/amount/cargo + re-query + dead getStatusValue | 682 | 1440 |
| index/cargo/amount + re-query + dead getStatusValue | 675 | 1440 |
| amount/index/cargo + re-query | 700 | 1436 |
| cargo/index/amount + re-query | 408 | 1432 |
| `Ship *s = ship` / `this->ship` / `unsigned index` / decl reorder | 696–708 | 1436 |

The case-6 tail (`mov r8,r4; lsls; ldr r6` vs `ldr r6; mov r8,r4; lsls`)
is a BranchFolder tail-merge, already documented in
`status-missioncompleted-wave190-w13423.md`; no source shape breaks it at
1432 B.

## 7. `Gun::calcCharacterCollision` (455) and `SpaceLounge::drawLounge` (138)

Re-confirmed the wave-190 plateaus.  For `drawLounge`, swapping the
`topLeft`/`bottomRight` loads, moving the `pad` load first, moving
`SetColor` first, using float temporaries, and moving the `agent` load
after the Vector loads all stay at 138/87/1948 (moving `SetColor` first
gives 1545/1952).  For `Gun`, the committed body remains the unique 455
among the wave-190 permutation spaces.

## 8. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE \
  _ZN3Hud4drawExxP9PlayerEgobjj _ZN10ModStationC1Ev \
  _ZN5MGame8OnUpdateEv _ZN15MenuTouchWindow12OnTouchBeginEiiPv \
  _ZN6Status16missionCompletedEbbx _ZN3Gun22calcCharacterCollisionEv \
  _ZN11SpaceLounge10drawLoungeEv
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w13500/` (`sweep_*.py`,
`*.base.cpp`, `gen_ir.txt`).

## 9. Files changed

This note only.  `git status --porcelain` is empty apart from it.
