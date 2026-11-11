# Hud::draw — wave 169 (w11416): show-block order + LSDA trap re-confirmed

Target: `_ZN3Hud4drawExxP9PlayerEgobjj` (`src/game/ui/Hud.cpp`, `Hud::draw`),
original **9,332 B**, VMA `0x163b90`.

Entry state = wave-168 landed shape (`YHTDW` show order + dock
`unsigned int barW; short barH`): raw **582**, aligned **228**, size 9,332,
`.ARM.extab 0x26298`, gate green (byte_exact 4507).

**No source change landed** (the best raw win is LSDA-fatal; see below).
The tree is left at the wave-168 state.

## Key result: the faithful String-first show order is 572 raw but gate-fatal

The original show block order is `String text; halfW=GetImage2DWidth*0.5;
barH=GetImage2DHeight; field_0x464+=dt; fade; top; barYOffset; fadeColor; y`
(disasm `165ba6..165c1c`).  A `T W D Y H F C P y` order (String first,
`dummy` after `halfW`) measures:

| shape | raw | aligned | size | `.ARM.extab` |
|---|---:|---:|---:|---:|
| wave-168 landed `Y H T D W F C P y` | **582** | 228 | 9,332 | **0x26298** |
| `T W D Y H F C P y` (T-first) | **572** | **212** | 9,332 | **0x2629c** |
| `T W Y D H F C P y` | 572 | 214 | 9,332 | 0x2629c |
| `T W D H Y F C P y` | 574 | 206 | 9,332 | 0x2629c |
| `T W Y H F C P y` (no dummy) | 1,190 | 210 | 9,328 | 0x2629c |

`T W D Y H` is the best raw shape measured (572, aligned 212), but it grows
Hud::draw's LSDA call-site table by **one entry (77 -> 78 / 453 -> 459 B)**,
which pushes `.ARM.extab` from `0x26298` to `0x2629c`, shifts `.rodata` by
`0x10` (16-byte alignment), and drops `byte_exact 4507 -> 4044` (the
known `.ARM.extab`/`.rodata` trap, `hud-draw-extab-lsda-w4273.md`).
**RATCHET FAIL** as-is.

### Why the extra LSDA entry

Parsing Hud::draw's LSDA (`/dev/shm` helper, same format as the w4273 note):

```
orig / wave-168: (8108,180,0,0) (8288,4,..) (8420,42,..) (8496,126,..)   77 entries
T-first        : (8108,152,0,0) (8260,4,..) (8306,4,..) (8420,42,..) (8496,126,..)  78
```

The String-first order puts `GetImage2DHeight` *after* the `String`
construction, so that call needs its own cleanup entry.  The original has the
same call order but only 77 entries, so the original's IR merges one of the
two show-block landing pads; clang 7.0.2 on our source does not.

### Measured LSDA compensations (all gate-green, all raw-worse)

Moving `float filled` before `SetColor` merges the two show-block landing
pads back to 77 entries, but changes the allocation:

| shape | raw | aligned | size | extab |
|---|---:|---:|---:|---:|
| faithful `T W H F top Y C y`, `filled` before `SetColor` | 1,089 | 208 | 9,332 | 0x26298 |
| `T W Y H ...`, `filled` before `SetColor` (no dummy) | 1,196 | 212 | 9,332 | 0x26298 |
| `T W D Y H ...`, `filled` before `SetColor` | 1,169 | 227 | 9,336 | 0x26298 |
| `T W D Y H ...`, `float filled;` decl before, assign after | 1,172 | 203 | 9,336 | 0x26298 |
| `T W D Y H ...`, `filled` inlined in DrawRegion2D | 1,172 | 203 | 9,336 | 0x26298 |

So the 572 shape cannot be landed without paying >400 raw bytes.

## Dock block: types and declaration orders are at a local optimum

Full type sweep with the wave-168 show order (call order W-then-H unless
noted), all size 9,332 / extab 0x26298 unless stated:

| dock types | raw | aligned | size |
|---|---:|---:|---:|
| **`unsigned int barW; short barH`** (landed) | **582** | **228** | 9,332 |
| `unsigned int; unsigned short` | 583 | 229 | 9,332 |
| `short barW; int barH` | 644 | 226 | 9,332 |
| `int; int` | 769 | 230 | 9,332 |
| `int; unsigned int` | 770 | 231 | 9,332 |
| `unsigned int; int` | 1,563 | 226 | 9,328 |
| `int; short` | 1,597 | 237 | 9,336 |

`frameH`/`textH` declaration/assignment forms (`int textH;` first,
`int frameH, textH;`, separate assign) all give 582/228; swapping the call
order to H-then-W gives 639/241.  The `[sp,#92]`/`[sp,#96]` spill-slot swap
(`frameH` vs `font`) is not moved by any of these.

The dock register swap is unchanged: original `barW=r8, barH=fp, Canvas=r5`;
ours `barW=r5, barH=r8, Canvas=fp`.  With `int/int` the allocator picks
`r6/r8` instead — none of the tested shapes reproduces `r8/fp`.

## Damage block: no new win

- `brace_read` (current: volatile read in the `if`, unconditional store) 582/228.
- Braces around both read+store, two separate `if`s, read-then-brace:
  **7,132 raw / 523 aligned / 9,336 B** (the `field_0x4b8` store becomes
  conditional and the function grows 4 B).
- Unconditional volatile read before the `if`: 7,138/522/9,336.
- `(void) &Globals::status;` before the `if`: 7,519/641/9,268 (size collapses).
- The original's dead `ldr r8,[sp,#108]` + four `itt ne / movne / strne`
  stores is still not reproducible: our shape keeps the conditional
  `ldrne r0,[r8]` and the unconditional store.

## Other measured non-wins

- Armor regen `DrawRegion2D` pivotX `0 -> dividerOffset` (to match the
  original's `strd r4,r4`): **7,955 raw / 584 aligned / 9,352 B** — large
  allocation cascade; reverted.
- `armorY`/`armorBarY` declaration swap: 582/228 (no change).
- `gammaBarY` operand/declaration orders: no change to the `fp`/`r9` swap.

## Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN3Hud4drawExxP9PlayerEgobjj
```
