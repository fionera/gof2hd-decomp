# Radar::draw pass (wave 131, w7696) — loop-block / draw-order / literal-pool sweep, no source win

**Status:** research note, 2026-11-07. Worktree `wt/w7696`, based on current
`master` (`7c6933c2d`). **No source change landed**; this commit contains this
note only. The target remains the pristine baseline
`src/game/weapons/Radar.cpp`
(`sha256 6a97cabddbd9abc603e47c5ddc2e57af292e55732e71ecb6798b248d147102e4`).

Target `_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).
Original 9908 B / 3492 normalised instructions; ours 9908 B / 3483.
Baseline unrounded `difflib.SequenceMatcher(None, on, un, autojunk=False).ratio()`
= **75.354839 %** (official display 75.4). Gate at start and end:
`RATCHET PASS`, byte **4506**, linked **4506**, data 843/843, imports 207,
lint CLEAN, drift 0, avg 99.94.

Read with `radar-draw-wave129-w7674.md`, `radar-draw-wave129-w7673.md`,
`radar-draw-wave128-w7650.md`, `radar-draw-wave127-w7628.md`.

This wave attacked the assignment's hypothesis directly: **planet/ship loop
block layout, order of the draw blocks, and literal-pool/register
allocation**. It adds two pieces of evidence the earlier notes did not have:
(a) a full literal-pool word/position diff of the whole function, and
(b) a 24-way declaration-order sweep of the planet-loop locals plus a
`camp_reload` family sweep. The conclusion is unchanged: the baseline is a
strong local optimum, and the residual is the coupled `stations`/`turretMode`
callee-saved-register swap.

## 1. Commands

```sh
cd /home/debian/GalaxyOnFire2/wt/w7696
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6     # ~0.9 s per variant
cmake --build cmake-build-match --target check -j6        # RATCHET PASS
```

Scratch in `/dev/shm/w7696` (not committed): `Radar.base.cpp` (pristine
source), `score.py` (single-symbol difflib harness reproducing 75.354839
exactly), `try.sh` (copy + link + score), `gen_batch.py`/`gen_insight.py`
(variant generators), `pools.py` (literal-pool extractor), `orig.dis`/
`ours.dis`/`dump.json`/`diffaddr.txt` (full function disassembly and
address-annotated diff).

## 2. Literal pools — words are identical, only positions differ

`pools.py` extracts every `ldr/ldrd/vldr [pc,#N]` target word in the function
from both binaries. The pool **word count, order and meaning are identical**;
the only differences are the PC-relative deltas (which encode the code layout
distance, so they are expected to differ when the layout differs).

| pool | original | ours | words (meaning) |
|---|---:|---:|---|
| first | `0x12e340` (fn+`0x2f0`) | `0x12e310` (fn+`0x2c0`) | 5: `__stack_chk_guard`, `Globals::Canvas`, `Globals::status`, `PaintCanvas::DrawImage2D`, `Globals::font` |
| second | `0x12e79c` (fn+`0x74c`) | `0x12e7f4` (fn+`0x7a4`) | 3: `StarSystem::orbitPlanetIndex`, `PlayerEgo::crosshairPos`, `Globals::font` |
| ship | `0x12f200` (fn+`0x11b0`) | `0x12f21c` (fn+`0x11cc`) | 4 pointers + `24000.0f`, `-24000.0f` |
| asteroid | `0x12fca0` (fn+`0xba0`) | `0x12fca4` (fn+`0xba4`) | 7 pointers + `24000.0f`, `60000.0f`, `-24000.0f`, `100.0f` |

The float constants are byte-identical (`0x46bb8000`, `0x476a6000`,
`0xc6bb8000`, `0x42c80000`). So the literal-pool **contents** are not a lever;
only the block layout that positions the pools is.

## 3. The real residual: `stations` ↔ `turretMode` callee-saved swap

Address-annotated diff of the planet loop (`12e376`–`12e7d4`):

| value | original | baseline ours | `camp_reload` (faithful) |
|---|---|---|---|
| `i` | `fp` | `fp` | `fp` |
| `stations` | `sl`, spilled `[sp,#116]` | `r6` (and `sl`), not spilled | spilled `[sp,#148]` |
| `stationId` | `r8` | `r8` | `r9` |
| `currentStation` | `r9` | `r9` | `r6` |
| `turretMode` | `[sp,#148]` | `[sp,#152]` | `sl` |
| campaign `isTarget` | reloads `stations->data()[i]` into `sl`, restores `sl` | compares `stationId` (`r8`) directly | reloads into `r8` |
| `vpush` | `{d8-d13}` | `{d8-d11}` | `{d8-d11}` |

The faithful source change is one line:

```cpp
- bool isTarget = stationId == campaignMission->getTargetStation();
+ bool isTarget = stations->data()[i] == campaignMission->getTargetStation();
```

It reproduces the original's `ldr.w r0,[sl,#4]; ldr.w sl,[r0,fp,lsl#2]`
reload and the `ldr.w sl,[sp,#116]` restore, but clang then chooses
`turretMode` for `sl` and spills `stations` to `[sp,#148]` (the slot the
original uses for `turretMode`). The two assignments are mutually dependent:
the reload needs a scratch register, and the allocator either gives it `sl`
(original) or `r8`/`r9` (variant). The variant is **9900 B / 73.24 %**.

## 4. Variants measured this wave

All rows are single edits on the pristine baseline unless noted; `usize` is the
emitted size; `raw %` is the unrounded ratio.

### 4.1 `camp_reload` family (planet campaign `isTarget`)

| variant | usize | raw % |
|---|---:|---:|
| **baseline** | 9908 | **75.354839** |
| `camp_reload` (`stations->data()[i]`) | 9900 | 73.235800 |
| `camp_reload` + `currentStation` declared first | 9900 | 73.3 |
| `camp_reload` + `textOffset` declared first | 9900 | 73.2 |
| `camp_reload` + freelance block before campaign | 9900 | 73.2 |
| `camp_reload` + local `targetStation` | 9900 | 73.2 |
| `camp_reload` + `unsigned stationId`/`currentStation` | 9900 | 73.2 |
| `camp_reload` + inlined comparison into the `if` | 9896 | 73.5 |
| `camp_reload` + `const`/`const`-pointee `stations` | 9900 | 73.2 |
| `camp_reload` + local `SolarSystem *sys` | 9900 | 73.2 |
| `camp_reload` + `const`/`volatile`/`int turretMode` | 9900 | 73.2 / 41.3 / 67.2 |

### 4.2 24-way declaration-order sweep (planet-loop locals)

The four declarations `stationId`, `currentStation`, `warpGate`,
`textOffset` were permuted exhaustively. The baseline order
(`stationId, currentStation, warpGate, textOffset`) is best at **75.354839**;
`stationId, currentStation, textOffset, warpGate` and
`stationId, textOffset, currentStation, warpGate` tie exactly. Every order
with `currentStation` first is 74.85–75.33 (the three orders that keep `stationId`
before `warpGate` reach 75.326165); every order with `warpGate` first is
70.4–71.0; `stationId, currentStation, warpGate, textOffset` is the only
family that reaches 75.35.

### 4.3 Ship `inSight` block (the `vpush {d8-d13}` live range)

The original loads `crosshairPos.x/y` into `s26`/`s24`, converts them in place
**before** the `VectorLength` call, calls `VectorLength` **before** the integer
bounds test, and keeps the converted ints in `s26`/`s24` across the call (hence
`vpush {d8-d13}`). Ours converts first, does the bounds test, and calls
`VectorLength` last (`vpush {d8-d11}`). Source-order attempts:

| variant | usize | raw % |
|---|---:|---:|
| **baseline** | 9908 | **75.354839** |
| `float dist = VectorLength(...)` before `if` | 9904 | 64.1 |
| `VectorLength(...) < 60000.0f` first in the `if` | 9904 | 65.4 |
| `bool near = VectorLength(...) < 60000.0f` then `near && ...` | 9904 | 65.5 |
| distance local before `cx`/`cy` | 9904 | 64.0 |
| assign directly to `enemy->field_0x73` | 9864 | 63.7 |
| direct assign, distance first | 9904 | 65.5 |
| `bool inSight = <whole condition>` | 9864 | 73.3 |

None reproduces the original's `s24`/`s26` live range; all re-roll the rest of
the function.

### 4.4 First `otherLock` guard (the `cbnz` chain)

The original is a contiguous `cbnz` chain at `12e304`–`12e312`; ours is a
`cbz`+`itt` shape with the first literal pool (`12e310`) in the gap. Every
positive/bitwise/ternary spelling canonicalises back to the `cbz` shape:

| variant | usize | raw % |
|---|---:|---:|
| **baseline** (negated `&&`, expect 0) | 9908 | **75.354839** |
| positive `||` (`!= nullptr`) | 9908 | 74.2 |
| positive `||`, implicit pointer bool | 9908 | 74.2 |
| `int otherLock = field_0x14 != 0; if (A||B||C) otherLock = 1;` | 9856 | 73.3 |
| positive `||` ternary | 9908 | 74.2 |
| bitwise `|` | 9908 | 67.3 |
| negated `&&` without expect | 9908 | 74.2 |

### 4.5 Landmark body / draw-block order

| variant | usize | raw % |
|---|---:|---:|
| **baseline** | 9908 | **75.354839** |
| `if (i < 2)` instead of `i <= 1` | 9908 | 75.354839 (tie) |
| `if (i == 0 || i == 1)` | 9908 | 75.354839 (tie) |
| `if (i < 3)` for the `i != 3` draw guard | 9908 | 75.326165 |
| `if (i >= 0 && i != 3)` | 9908 | 75.354839 (tie) |
| `if (!alienOrbit && i == 0)` | 9908 | 71.9 |
| guard inverted (`i != 2 && ptr && visible`) | 9908 | 40.9 |

### 4.6 Declaration order of the campaign/freelance block

| variant | usize | raw % |
|---|---:|---:|
| **baseline** (`campaignMission`, `freelance`, `marked`) | 9908 | **75.354839** |
| `freelance` first | 9908 | 71.98 |
| `marked` first | 9908 | 75.354839 (tie) |
| `marked` between the two missions | 9908 | 75.354839 (tie) |

## 5. Root cause (unchanged)

The residual is the coupled fixed point described in waves 122/127/128/129:

1. the first `otherLock` guard's `cbz`+`itt` shape and the first pool at
   `12e310` (original `cbnz` chain, pool at `12e340`);
2. the invented post-loop recompute at `12e72c` (original single recompute at
   `12e6ee`), which displaces the planet-lock tail to `12e776` and the
   landmark setup to `12e7c4`;
3. `stations` in `r6` instead of `sl`+spill, and the corresponding
   `turretMode` slot swap (`[sp,#152]` vs `[sp,#148]`);
4. the bottom-tested landmark loop (pool at `12e7f4` instead of `12e79c`);
5. the ship `inSight` `s24`/`s26` live range (`vpush {d8-d13}` vs `{d8-d11}`).

The faithful single-recompute + `camp_reload` shape fixes (2)–(4) but breaks
(3) and scores 72.9–73.2 %. A real win still needs a source shape that keeps
the single recompute **and** the `fp`/`sl`+spill planet-loop split **and** the
`vpush {d8-d13}` live range simultaneously.

## 6. Gate / section deltas (baseline, unchanged)

```
compared 4524  avg 99.94%  byte_exact 4506  linked_exact 4506  fuzzy_perfect 4506
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Section deltas vs the original: `.text` 85,379 differing bytes (size/base
exact), `.ARM.extab` 1,435, `.note.gnu.build-id` 20. `.rodata`, `.data`,
`.bss`, `.got`, `.ARM.exidx`, `.rel.plt`, `.rel.dyn`, `.dynsym`, `.dynstr`,
`.hash` are byte-identical.
