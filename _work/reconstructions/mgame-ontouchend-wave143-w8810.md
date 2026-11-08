# MGame::OnTouchEnd — wave 143 (w8810): flat `hudTouchFlags == 0` shape flips the join test to `cbz`

Status: source change landed, 2026-11-08. Worktree `/home/debian/GalaxyOnFire2/wt/w8810`,
base `bf083ab41` (wave 142). Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only).
Scratch `/dev/shm/w8810`. Full `check` green before and after:
byte **4507**, linked **4507**, size **4524/4524**, data **843/843**, imports **207**,
lint **CLEAN**, drift 0.

Target `_ZN5MGame10OnTouchEndEiiPv`, original 8080 B @ `0x17a144`.

| | base | this wave |
|---|---:|---:|
| size | 8080 | **8080** |
| raw same-address diff | 4464 | **4399** |
| join test | `ldr [fp,#248]; cmp #0; bne.w epilogue` | **`ldr [fp,#248]; cbz flags_body`** |
| join test address | `0x17aeb2` | `0x17b93c` |

## 1. Hypothesis tested

Wave 142 asked for a source shape that flips the join-test fall-through
(`hudTouchFlags == 0`) to the original `cbz flags_body` (epilogue fall-through)
without `__builtin_expect(failed, 0)` re-laying `0x17ab00..0x17ad00`.

The new shape is to take the `hudTouchFlags == 0` test **out of the `else`** of the
weapon-slot test.  The base (wave 140 nested shape) is:

```cpp
if (weaponSlot >= 0) { ... }
else {
    if (wingmanCommand >= 0) { ... }
    if (this->hudTouchFlags == 0) { ... }
}
```

The landed shape is:

```cpp
if (weaponSlot >= 0) { ... }
else if (wingmanCommand >= 0) { ... }
if (this->hudTouchFlags == 0) { ... }
```

This is semantically equivalent: `weaponSlot >= 0` and `wingmanCommand >= 0` both
imply `hudTouchFlags != 0` (the corresponding bit was set), so moving the test out
of the `else` only adds a provably-false check on those paths.

## 2. Result

Clang now emits the original's join-test polarity exactly:

```
ours 17b93c: ldr.w r0,[fp,#248]   ; hudTouchFlags
ours 17b940: cbz r0, 17b974       ; -> flags body (cold)
ours 17b942: ... epilogue (isAutoPilot) ...
ours 17b974: flags body
```

The original is the same instruction sequence at `0x17afe2` / `0x17afe8` / `0x17b01a`.
The base instead emitted `cmp r0,#0; bne.w 17b962` with the flags body inline.

Raw diff drops 4464 -> **4399** (-65 B).  The win is spread over the tail
(`0x17bd00` 228 -> 191, `0x17bf00` 59 -> 36, `0x17c000` 21 -> 17); the
`0x17ae44` dockToStream window is **not** fixed: it moves from +8 (base) to +12
(flat), because the tail-merge survivor is still the `0x200` body and the epilogue
lands at `0x17b942`, not `0x17afe8`.  So this is a **join-test polarity fix**, not
the full `0x17ae44` fix.

## 3. Variants measured (all reverted unless noted)

| variant | size | raw diff | note |
|---|---:|---:|---|
| base (nested) | 8080 | 4464 | `cmp; bne` join test |
| **flat (landed)** | **8080** | **4399** | `cbz` join test |
| flat + `cutsceneActive == true` | 8080 | 4475 | |
| flat + `touchesStream == true` | 8080 | 4400 | |
| flat + swap 0x200/0x400 | 8080 | 4402 | survivor 0x400, test order wrong |
| flat + swap 0x800/0x400 | 8048 | 4685 | |
| `__builtin_expect(flags == 0, 0)` | 8076 | 6058 | join test unchanged |
| `__builtin_expect(flags == 0, 1)` | 8080 | 4464 | byte-identical |
| `__builtin_expect(failed, 0)` | 8064 | 5115 | exact join block, re-lays `0x17ab00..` |
| `hasWon() == true` -> bare `hasWon()` | 8068 | 6023 | matches orig `cmp #0; beq`, -12 B |
| bare `hasWon()` + `amount < 0` + `hasFailed() == true` | 8072 | 4270 | best sub-8080 shape |
| bare `hasWon()` + `amount < 0` + `hasFailed() == true` + `goingToWaypoint() == true` | 8076 | 4369 | |
| bare `hasWon()` + `amount < 0` + `hasFailed() == true` + `freeCamMode == true` | 8080 | 4943 | |

The `hasWon()` bare variant still matches the original's `cmp r0,#0; beq` but is
12 B short and re-lays the whole function; no +12 faithful companion was found
that keeps the raw diff below the flat shape.  The best 8080-B variant combining
the bare `hasWon()` is 4943, worse than flat.

## 4. Caveat / faithfulness

The flat shape matches the original join-test **instruction polarity** and keeps
the call order, but the original's join test is the layout successor of the
`0x400` body (inside the `else`), which suggests the original source may have been
the nested shape.  The flat shape is therefore a structural source change whose
win is partly block-placement; it is not a claim that the original source was
flat.  The `0x17ae44` (+8) survivor/branch-width problem remains open.

## 5. Reproduction

```
cp /dev/shm/w8810/MGame.cpp.base src/game/menu/MGame.cpp   # pristine base
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8810/fdiff.py _ZN5MGame10OnTouchEndEiiPv  # 4464 / 8080
# apply the flat shape from section 1
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8810/fdiff.py _ZN5MGame10OnTouchEndEiiPv  # 4399 / 8080
```

Scratch: `/dev/shm/w8810/{fdiff.py,windows.py,shapes.py,expect.py,expect2.py,
wonvariants.py,mining2.py,combo3.py,combo4.py,base.dis,flat.dis,orig.dis}`.
The authoritative binary is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
