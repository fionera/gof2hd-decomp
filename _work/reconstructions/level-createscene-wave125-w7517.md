# `Level::createScene` — wave-125 w7517: cap/spot/retry + `&rnd`/`&status` form sweep, no source win

**Status:** dead-end sweep, 2026-11-06. Worktree `wt/w7517`. Target
`_ZN5Level11createSceneEv` (`src/game/world/Level.cpp`, `createScene` only).
Baseline and best remains the wave-124 landed source: **93.554563 % / 2252 B**,
784 normalized instructions (original 785 before the literal pool), first
structural diff at instruction 9 (`str r0,[sp,#36]` vs `[sp,#40]`).
Full gate re-measured green: **byte_exact 4506 / linked_exact 4506**, avg
**99.93 %**, data **843/843**, imports **207**, lint CLEAN, drift 0,
symbol parity 5370/5370, `.ARM.exidx` byte-identical, `.ARM.rel.plt` exact,
`.ARM.extab` size/base exact.

## TL;DR

The wave-124 source is a very strong local optimum. The residual is exactly
the four-part RA puzzle the wave-124 note described:

1. `&Globals::status` spill slot is `[sp,#40]` (original `[sp,#36]`); the
   whole 4-slot group `{spots/hangarPart, parts-status, &status,
   kShipHangarHeights}` is rotated by one physical slot.
2. `capacity` is in `r4` (original `r6`) and `spotUsed` in `r6` (original
   `r4`); `ownHangar` is in `r5` (original `r4`).
3. One extra 16-bit `uxth r4,r4` from the `unsigned short cap` split.
4. `ldrd r0,fp,[sl,#4]` (player+geometry fused) instead of the original's
   two separate `ldr.w` loads, plus an extra `b.n` in the switch default
   block and one extra `ldr r1,[sp,#spots]` (machine block placement).

~120 new source variants were compiled with the repo's exact NDK r18b
`-Oz` flags and their register maps / stack slots / sizes read from the
object disassembly. **None beat 93.554563 % at 2252 B.** The register
basin is extremely robust: only the `cap` split vs no-`cap` choice moves
it, and every no-`cap` form lands in a 2228 B basin with `capacity=sl/r8`
and `&status` in a low register.

## 1. Exact residual (base, from `verify --show`)

```
  str  r0, [sp, #36]      |   str  r0, [sp, #40]      ; &Globals::status
  ...
  ldr.w r6, [r1, r0, lsl #2] | ldr.w r4, [r1, r0, lsl #2]  ; capacity
  mov  r4, r0             |   mov  r6, r0             ; spotUsed = new[]
  lsrs r4, r0, #5         |   lsrs r5, r0, #5         ; ownHangar
  ...                     |   uxth r4, r4             ; extra cap truncation
  ldr.w r0, [sl, #4]      |   ldrd r0, fp, [sl, #4]   ; player(+geometry)
  ...                     |   b.n 0x7a0               ; extra switch branch
```

Original target map (unchanged from wave-121/122/123/124):

| value | original | wave-124 landed |
|---|---|---|
| `spotUsed` | r4 | r6 |
| `race` | r5 | r5 |
| `capacity` | r6 | r4 |
| `&Globals::rnd` | r8 | r8 |
| `i` | r9 | r9 |
| `&Globals::status` | sl (+`[sp,#36]`) | sl (+`[sp,#40]`) |
| `type` | fp | fp |
| `ownHangar` | r4 | r5 |
| `spots` / `hangarPart` | `[sp,#44]` | `[sp,#32]` |

## 2. Variants measured (all no-win)

### 2.1 `cap` / `spot` / retry forms (all 2252 B, `cap=r4 spot=r6 own=r5`)

Every one of these produced **byte-identical** code to the landed source:

* `short cap`, `unsigned char cap`, `signed char cap`, `const unsigned short cap`
* separate declaration + assignment (`unsigned short cap; cap = ...;`)
* `cap` declared after `shipCount`, after `ownHangar`, or inside the ship loop
* `unsigned short cap = (unsigned short) kHangarCapacity[sceneRace];` (recompute)
* `unsigned short cap = (unsigned short) (capacity + 0 / ^ 0 / % 65536 / <<16>>16)`
* `int cap = (int) (unsigned short) capacity;`
* `cap` used for `new[]`/zero loop as well as the nextInts
* `spot = rnd->nextInt(capacity)` for the first call, `cap` for the retry (2236 B, worse basin)
* `spot = rnd->nextInt(cap)` first, `capacity` for the retry (2236 B, worse basin)
* `while`/`do` retry loop (`do` collapses to 2212 B)
* `unsigned`/`bool`/`char` `spotUsed`, `int`/`unsigned` `spot`/`tries`/`i`/`shipCount`

Forms that remove the `uxth` (`int cap = capacity`, `capacity & 0xFFFF`,
`capacity & 0xFFFFu`, in-place `capacity &= 0xffff`,
`unsigned short capacity = ...`) all collapse to the **2228 B** basin
(`capacity=sl`, `spotUsed=r4`, `ownHangar=r4`, `&status` in r4/r6, `&rnd`
r8/r9). None emit a free split: `int cap = capacity` is coalesced away,
and `% 65536` re-emits the same `uxth`.

### 2.2 `&Globals::rnd` / `&Globals::status` caching (all 2252 B, identical code)

* `AERandom *&rnd = Globals::rnd` (landed) vs direct `Globals::rnd` — identical
* `AERandom **rnd = &Globals::rnd; (*rnd)->nextInt(...)` — identical
* `rnd` declared before/after `capacity`, before `ownHangar`, at the top of
  the `missionPtr==23` block — identical
* `Status *&status = Globals::status` (block-local or whole-function) — identical
* `Status *status = Globals::status` (value) — 2204 B, different basin
* `AERandom *rnd = Globals::rnd` (value) — 2244 B, different basin

### 2.3 parts loop / switch / first-ship forms (all 2252 B, identical code)

* `Vector rot = {0, pi, 0}` aggregate, `Vector rot(0, pi, 0)` ctor
* `switch` case reorder, `default` first, `table = spots` initialiser
* `if/else` chain instead of `switch` — 2280 B
* `kShipHangarHeights[shipIndex]` cached in `int`/`float`
* first-ship call reorder, `enemies[0]` cached in a local — 2220 B
* `plainHangar` spellings (`>= 4`, swapped `||`, `|`, ternary, `int`,
  `unsigned char`, `const`) — all identical clz form
* `ownHangar` as `int`, `== true`, `!= 0`, `&&` single expression — identical
* `shipCount` if/else, reversed min, `unsigned` — identical

### 2.4 forms that DO move the stack slot (but lose elsewhere)

| variant | pct | size | slot | cap/spot/own |
|---|---|---|---|---|
| **landed** | **93.554563** | **2252** | 40 | r4/r6/r5 |
| `plainHangar = sceneRace==1 \|\| sceneRace==7 \|\| sceneRace==8` | 85.6 | 2264 | **36** | r4/r6/r5 |
| same + no `cap` + `geo` after `setToSleep` | 76.7 | 2252 | 68 | fp/r4/r4 |
| `Vector rot; rot = Vector(0,pi,0);` | — | 2264 | **36** | r4/r6/r5 |
| `geo` after `setToSleep` (cap kept) | 89.9 | 2256 | 40 | r4/r6/r5 |
| no `cap`, `geo` after `setToSleep` | — | 2228 | 68 | r8/r4/r4 |

The 3-compare `plainHangar` and the `Vector` assignment both re-seat
`&Globals::status` to `[sp,#36]`, proving the slot is reachable — but both
change the parts-loop / `plainHangar` code enough to drop the score by
8-17 points. The original's `plainHangar` is the clz form
(`(sceneRace==1) | (sceneRace>3)`), so neither is the original source.

## 3. Why the register basin cannot be bridged from source

Greedy RA processes `capacity` and `spotUsed` by spill weight. In the
landed `cap` form `spotUsed` is processed first and takes `r6`
(`&Globals::rnd` is evicted to `r8`), so `capacity` falls to `r4` and
`ownHangar` to `r5`. In the original `capacity` is processed first and
takes `r6`, leaving `r4` for `ownHangar`/`spotUsed`. The `cap` split is
what raises the pressure enough to push `&status` into `sl` (matching the
original), but it simultaneously gives `spotUsed` the `r6` it wants.
Removing the split restores `capacity`'s weight but then `&status` drops
out of `sl` (it lands in r4/r6) and the whole function shrinks to 2228 B.
Every no-instruction split tried (`int cap`, `&`, `%`, in-place truncate)
is either coalesced or re-emits the `uxth`; no form keeps `capacity` in
`r6` **and** `&status` in `sl`.

## 4. Remaining root cause

The residual is entirely Greedy-RA / MachineBlockPlacement ordering:

* the 4-slot frame rotation `{spots, parts-status, &status, kShipHangarHeights}`
  (physical-slot colouring order, not a value difference),
* `capacity`/`spotUsed`/`ownHangar` register transposition,
* the 16-bit `uxth` from the `cap` split,
* the fused `ldrd` (source `geo` hoist) and the switch-default `b.n`.

All constants, calls and call order already match the binary. Closing the
function needs the original source's exact live ranges (or a way to make
the greedy allocator process `capacity` before `spotUsed` without the
`cap` truncation), which no source perturbation found in this wave.

## 5. Reproduction

```sh
OBJDUMP=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
python3 tools/verify/verify.py --show _ZN5Level11createSceneEv ...
cmake --build cmake-build-match --target check -j6   # byte 4506 linked 4506
```
