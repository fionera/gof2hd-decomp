# MGame::OnTouchEnd (8080 B) — w6005 (wave 112): chain-level analysis;
# no gate-safe gain

Target: `_ZN5MGame10OnTouchEndEiiPv` (file VMA 0x17a144, 8080 B,
`src/game/menu/MGame.cpp`).  Base: current master, branch `wave112-w6005`.

## Result

**No gate-safe improvement found.**  HEAD stays at **8080 B /
78.9189 % / `.rel.plt` diff 0 / RATCHET PASS** (byte 4500, linked 4500,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0,
27 sections, `.ARM.extab` 0x26298 = original, `.rodata` 0x41c27 =
original 0x41b67 + the 192-byte `Status::missionCompleted` rank table).
This note records the measurements plus one new, precise finding about
*which* MachineBlockPlacement chain the tail cluster belongs to and why
the source cannot move it without a size change.

## 1. New finding: the tail cluster's chain, read from the MIR

`-mllvm -print-machineinstrs` (scratch `mir.txt`) around
`# After Branch Probability Basic Block Placement` for this function gives
the chain (a run of blocks with fall-through) that ends in the epilogue:

```
CHAIN: bb.350 (weaponSlot = 3 arm, `$r6 = t2MOVi 3`)
       bb.351 (if (weaponSlot >= 0) test)
       bb.352 (weaponSlot body, part 1)
       bb.353 (weaponSlot body tail: resumeSounds/closeHudMenu)
       bb.354 (if (hudTouchFlags == 0) test)
       bb.355 (tail: if (player->isAutoPilot() && !wasAutoPilot))
       bb.356 (epilogue / stack cookie)
       bb.357 (__stack_chk_fail)
       bb.358..360 (hudTouchFlags == 0 body)
```

The original's equivalent chain is

```
[0x400 arm] -> [shared `blx Hud::initHudMenu`] -> [hudTouchFlags test]
            -> [tail] -> [epilogue] -> [stack_chk_fail] -> [hud0 body]
```

i.e. in the original the tail cluster hangs off the **`initHudMenu(2)` arm**,
not off the **`weaponSlot = 3` arm**.  Evidence from the original binary:

* `0x17afd8` `ldrd r0,r2,[fp,#116]; movs r1,#2; blx initHudMenu` (the
  `hudTouchFlags & 0x400` arm) falls straight through into
  `0x17afe2 ldr.w r0,[fp,#248]` (the `hudTouchFlags == 0` test), then
  `0x17afe8` (tail), `0x17b004` (epilogue), `0x17b01a` (hud0 body).
* `0x17adc0` (the `hudTouchFlags & 0x200` arm) is a one-block chain ending
  in `b.n 17afde` (the shared `blx`).  In HEAD the roles are swapped: the
  shared `blx` is at `0x17adcc`, chained after the `0x200` arm, and the
  tail cluster is at the very end (`0x17b934`).
* The original's `weaponSlot` body (`0x17b9cc`, `blx Ship::getEquipment` at
  `0x17b9da`) ends in `b.w 17afe8` — it jumps **straight to the tail,
  skipping the `hudTouchFlags == 0` test**.  HEAD's weaponSlot body
  (`0x17b8da`, `getEquipment` at `0x17b8ea`) falls through into the test
  (`0x17b934`).  So the original's CFG has the test's predecessors =
  {no-flag-matched path, wingman body} only; HEAD adds the weaponSlot body.

`hasBetterLayoutPredecessor()` (LLVM 7 `MachineBlockPlacement.cpp`) is what
stops HEAD's `blx` chain at bb.343: the test block has another predecessor
(the weaponSlot body) whose edge frequency is comparable, so the edge
`blx -> test` is rejected and the tail is claimed later by the
`weaponSlot = 3` chain.

## 2. Reproducing the original CFG costs +4 B and scores lower

Spellings that make the weaponSlot body jump straight to the tail (i.e.
reproduce the original CFG) are semantically free — if `weaponSlot >= 0`
then `hudTouchFlags` has one of 0x2000/0x4000/0x8000/0x10000 set, so
`hudTouchFlags == 0` is statically false:

```cpp
if (this->hudTouchFlags == 0 && weaponSlot < 0) { ... }   // `hud0_ws`
```
or the faithful wingman nesting (`nested`, hud0 check inside the `else`).

Both give the original's `weaponSlot body -> tail` edge, but the chain that
results is `[weaponSlot = 3 arm][body][tail][epilogue][hud0]` with the test
block hoisted to `0x17aeb2` and the tail at `0x17b970` — the tail is still
at the end, and the function grows to 8084 B.  The +4 B is not the mining
`String` hoist; it is one extra 32-bit branch plus a `nop` from the new
block order.

## 3. Measurements (all on top of HEAD unless noted)

| variant | size | fuzzy (4 dp) | .rel.plt | note |
|---|---:|---:|---:|---|
| **HEAD** | **8080** | **78.9189** | **0** | gate-safe best |
| `s127` (faithful separate `== 127` case) | 8100 | 75.1620 | 0 | size fail |
| `disp` (campaign-first won dispatch) | 8088 | 79.0848 | 4 | size + plt fail |
| `s127+disp` | 8080 | **79.9063** | 4 | byte 4499 → RATCHET FAIL (verified) |
| `disp + cnt_ne` | 8076 | 75.0405 | 4 | |
| `nested` (hud0 check inside wingman `else`) | 8084 | 76.6847 | 0 | original CFG, +4 B |
| `nested + bool refinedAny` | 8080 | 74.6936 | 0 | |
| `hud0_ws` (`hudTouchFlags == 0 && weaponSlot < 0`) | 8084 | 76.6847 | 0 | same code as `nested` |
| `hud0_wc` (`... && wingmanCommand < 0`) | 8080 | 78.9189 | 0 | code-identical to HEAD |
| `hud0_ws_wc` | 8084 | 72.3443 | 0 | |
| `else if (hudTouchFlags == 0)` (chained) | 8080 | 76.0497 | 0 | |
| hud0 block moved before the weaponSlot chain | 8048 | 72.9666 | 11 | |
| hud0 block moved before the dispatch | 8080 | 76.0000 | 0 | |
| hud0 block moved outside the `if (hudMenuOpen)` | 8080 | 78.9189 | 0 | code-identical |
| `swap200_400` (0x400 arm first) | 8080 | 78.8108 | 0 | |
| `200last` (0x200 arm after 0x1000) | 8064 | 74.0580 | 13 | |
| `swap800_400` | 8064 | 71.9798 | 2 | |
| `s127 + 400first` | 8076 | 70.2703 | 2 | |
| `s127 + hud0_ws` | 8104 | 74.2621 | 0 | |
| `s127 + swap200_400` | 8100 | 75.0540 | 0 | |

Code-identical (8080 / 78.9189 / plt 0), i.e. no source lever at all:
`if (!this->hudTouchFlags)`, `keepObjectives` as `&&`,
`hasFailed() != 0`, `hasWon() != 0`, `mission` as if/else, swapping the
`case 0` / `case 1` order in the cinematicSteer switch, `(x & 0x40) != 0`,
`pauseOpen != 0`, `if (this->hudTouchFlags)`, `hasCargo(...) != 0`,
`ore <= 165`, `refinedAny != 0`, `isAutoPilot() != 0`, `_bc8 == 0`.

## 4. Instruction-level residual is now down to three instructions

A difflib pass over the normalised disassembly (2865 lines each) shows the
*only* non-branch, non-literal-pool differences left are:

1. mining loop: original `cmp.w r9,#0; beq` (0x17a5ca) vs HEAD
   `cmp.w r9,#-1; bgt` — the original tests `amount != 0`, HEAD is
   strength-reduced to `count > 0`.  Every `cmp #0` spelling switches the
   down-counter to an up-counter and loses 8 B (wave-110/111 result).
2. `add.w r8, sp, #80` (the `String("t ")` temporary) is scheduled before
   the `String::operator+` call in HEAD (0x17a5e8) and after it in the
   original (0x17a5f6); the two `mov` operands also swap.  Same instruction
   count, scheduler-only.
3. `lsls r0,r0,#31` (original, `bool refinedAny`) vs `#24` (HEAD,
   `char refinedAny`); `bool` is faithful but -4 B (wave-110).

Everything else is block placement / literal-pool offsets.

## 5. Why `s127+disp` (79.9063 %) cannot be landed

`s127+disp` is 8080 B and 79.9063 %, but `disp` moves the cold
`resetStartSequenceOver` block to `0x17b49a`, after
`Level::removeObjectives` (`0x17b2dc`), `goingToWaypoint` (`0x17b2fe`) and
`removeRoute` (`0x17b312`).  `.rel.plt` order is the relocation order inside
`MGame::OnTouchEnd`'s own section, so the four entries rotate and
`MGame::successCheck`'s `blx` immediates move.  Verified with the real gate:

```
$ cmake --build cmake-build-match --target check -j6
verify: byte 4499  linked 4500  ...
  FAIL  byte_exact 4500 -> 4499
RATCHET FAIL
```

`linked_exact` stays 4500 (branch targets are compared by symbol name), so
only the raw-byte count fails.  There is no other caller of
`resetStartSequenceOver` in the tree (`grep -rn` → only `MGame.cpp:1777`),
so the order cannot be pinned from another TU either.

## 6. Gate / section state (unchanged)

27 sections (exact), file size 2,187,328 B, `.text` 0x1337c8 (size and base
exact), `.ARM.extab` 0x26298 (**exact** — the wave-110 "4 B short" note is
stale), `.rodata` 0x41c27 (= original 0x41b67 + the 192-byte
`Status::missionCompleted` rank table), `.rel.plt` diff 0, imports 207,
lint CLEAN, drift 0, `RATCHET PASS` (byte 4500 / linked 4500).

```bash
cd /home/debian/GalaxyOnFire2/wt/w6005
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
python3 /dev/shm/w6005/score.py                      # 8080 / 78.9189
python3 /dev/shm/w6005/plt.py                        # diff 0
```

Scratch (not tracked): `/dev/shm/w6005/` (`score.py`, `plt.py`,
`try.sh`, `batch1..5.py`, `MGame.base.cpp`, `orig.asm`, `ours.asm`,
`nested.asm`, `hud0first.asm`, `v200last.asm`, `mir.txt`, `mbp.cpp`
= LLVM 7.0.1 `MachineBlockPlacement.cpp`).
