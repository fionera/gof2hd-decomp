# w12113 (wave 177): `Level::createMission` — inner `switch (k)` store-order
# permutation, size-exact 17,172 B and raw 5,257 -> 5,246 (-11)

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w12113`, branch
`wave177-w12113`, base master `9445ac5fb` (wave 176).  Target
`_ZN5Level13createMissionEv` (`src/game/world/Level.cpp`, 17,172 B, VMA
`0xaffd8`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2 `-Oz`).

## 1. Result

```text
                                     size            raw    aligned  insn
HEAD (base 9445ac5fb)                17172/17172     5257   963      6212/6197
landed (this pass)                   17172/17172     5246   970      6212/6197
```

Section bytes, measured in this worktree: `.text` differing bytes
**79,591 -> 79,580** (-11); `.ARM.extab` **1,321 -> 1,322** (+1, size- and
base-exact at 156,312 B @ `0x1a3b78`); `.ARM.exidx` **0**; `.rel.plt`
byte-identical (0); `.rodata`/`.data`/`.bss`/`.got`/`.rel.dyn`/`.dynsym`/
`.dynstr`/`.hash` exact.  Total differing bytes **80,932 -> 80,921**
(-11; `.note.gnu.build-id` measured 19 bytes).  File size exact
(2,187,328 B).

Gate after the change: byte_exact **4507/4524**, linked_exact
**4507/4524**, size-exact **4524/4524**, stub_zero_size **0**, missing
**0**, wrong_type **0**, extra **0**, data_exact **843/843**, imports
**207**, lint **CLEAN ✓**, drift **0**, symbol parity **5370/5370**.
`RATCHET PASS`; no baseline update was needed because no ratcheted count
changed.

## 2. Landed source shape

Only the store order of the four bodies of the inner `switch (k)` in the
`case 183` convoy block (`src/game/world/Level.cpp:2195`) changed.  The
values are unchanged; the assignments are a permutation of the same three
independent `float` stores to the local `offset[3]`:

```cpp
                        case 0:
                            offset[1] = -3000.0f;
                            offset[0] = 20000.0f;
                            offset[2] = -50000.0f;
                            break;
                        case 1:
                            offset[2] = -30000.0f;
                            offset[0] = 10000.0f;
                            offset[1] = -7000.0f;
                            break;
                        case 2:
                            offset[2] = -36000.0f;
                            offset[1] = 0.0f;
                            offset[0] = 25000.0f;
                            break;
                        default:
                            offset[2] = -45000.0f;
                            offset[0] = 12000.0f;
                            offset[1] = 6000.0f;
                            break;
```

This is semantics-preserving: the three components are independent
non-volatile stores to a local array that is fully overwritten before the
`setPosition(*reinterpret_cast<Vector *>(offset))` read.

## 3. Mechanism: switch-body block placement

The original and our base both lower the 4-way `switch (k)` to the same
compare tree at `0xb1418`:

```text
cmp r1,#2 ; beq case2
cmp r1,#1 ; beq case1
cbnz r1, default
case0:
```

but the block placement order of the case bodies differs:

```text
original:  case0, case1, case2, default   (0xb1420, 0xb142c, 0xb1438, 0xb1444)
base:      case0, case2, case1, default   (0xb1420, 0xb142c, 0xb1438, 0xb1446)
```

Because each body is a short straight-line sequence of literal-pool loads
and stack stores, permuting the store order changes the body sizes and the
literal-pool/stack-slot scheduling, which in turn moves the block
boundaries by a few bytes.  The landed permutation keeps the function
exactly 17,172 B but moves the `case1`/`case2`/`default` stores onto the
original's addresses for more bytes, dropping the same-address raw diff by
11.  The aligned instruction-index diff rises 963 -> 970 (the block order
itself is still 0,2,1,default); raw is the primary metric and the function
is not byte-exact either way.

## 4. Measured variants

All rows are size-exact 17,172 B unless noted.  `caseN` is the order of the
three `offset[]` assignments in that body (`012` = offset[0], offset[1],
offset[2]).  The base is `case0=012, case1=012, case2=012, default=201`.

```text
case0 case1 case2 default   raw    aligned
012   012   012   201       5257   963     (base)
102   201   210   201       5246   970     LANDED
102   201   210   102       5252   972
102   102   210   201       5247   976
102   102   210   102       5267   974
102   102   102   201       10278  940     size 17164 (-8, not landable)
102   102   102   102       10736  1008    size 17164 (-8, not landable)
120   120   120   120       10737  1006    size 17164 (-8, not landable)
```

The full 6^2 sweep of `case1`/`case2` orders (with `case0=012`,
`default=201`) and the full 6^2 sweep of `case0`/`default` orders (with
`case1=201`, `case2=210`) were run; 5246 is the best size-exact raw result
found.  The all-`102` "natural" order is 8 B short, so it is not landable
without a compensating +8 B elsewhere.

## 5. Other switch opportunities checked

* The `switch (Globals::status->getStation()->getIndex())` at
  `src/game/world/Level.cpp:1668` is already byte-identical in the
  `0xb08e2`..`0xb092e` region (both cases store in the original order).
* The `switch (good)` at `src/game/world/Level.cpp:1790` is lowered to a
  literal-pool lookup table at `0xb0c14` and is byte-identical.
* The outer mission-type switch dispatch at `0xb02a8` (`tbh`) is identical;
  only the case-body offsets differ by the accumulated 2/4-byte shifts
  documented in the wave-176 notes.

## 6. Verification commands

```sh
cd /home/debian/GalaxyOnFire2/wt/w12113
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
# -> raw=5246 aligned=970 size=17172/17172 insn=6212/6197
cmake --build cmake-build-match --target check -j2
# -> byte 4507 linked 4507 stubs 0 missing 0 extra 0 avg 99.93
#    data 843/843  imports 207  RATCHET PASS
cmake --build cmake-build-match --target drift -j2   # -> DRIFT: 0
cmake --build cmake-build-match --target diff -j2    # -> SYMBOL PARITY
```

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w12113/`
(`Level.base.cpp`, `sweep_cases.py`, `sweep_c0def.py`, `sweep_one.py`,
`secdiff.py`, `adiff.py`, `extras.py`, `shift2.py`, `base.so`, `single.so`).
