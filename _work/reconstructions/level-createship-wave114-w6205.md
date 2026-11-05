# Level::createShip — wave 114 (w6205): hitpoint chain order

Status: **source change landed** (wave-114, 2026-11-05).
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`, original
2764 B @ `0xc0214`). Builds on `level-createship-wave113-w6104.md` and
`level-createship-wave111-realnew.md`.

## Result

```
                                   before (HEAD d8557f7e2)   after
createShip fuzzy (report, 1 dp)    67.3                      67.4
createShip size                    2764 B                    2764 B
.text size / addr                  0x1337c8 @0x69440         0x1337c8 @0x69440
.ARM.extab size / addr             0x26298 @0x1a3b78         0x26298 @0x1a3b78
.ARM.exidx size / addr             0x6f70 @0x19cc08          0x6f70 @0x19cc08
.rodata size                       0x41c27 (269351)          0x41c27 (269351)
file size                          2,187,328 (exact)         2,187,328 (exact)
section headers                    27 (exact)                27 (exact)
byte_exact / linked_exact          4500 / 4500               4500 / 4500
imports / data / lint / drift      207 / 843 / CLEAN / 0     207 / 843 / CLEAN / 0
```

The verify fuzzy is `round(difflib.ratio*100, 1)`, so 67.3 -> 67.4 is the
whole measured delta; the function is still not byte- or linked-exact.

## Landed diff (`src/game/world/Level.cpp`)

The `hitpoints` type multiplier is written as an `if`/`else if` chain in the
order **`0x2c`, `0x31`, `0x33`** (bodies unchanged: `*2.25f`, `*17.0f`,
`*1.7f`). Clang 7.0.2 at `-Oz` emits the *reverse* order, so this source order
is what produces the original's emitted comparison order:

```
orig c02b0  cmp.w r11, #51   ; 0x33
     c02c0  cmp.w r11, #49   ; 0x31
     c02c6  cmp.w r11, #44   ; 0x2c
```

HEAD's source order `0x33/0x31/0x2c` emitted `0x2c/0x31/0x33`, which is a
3-instruction block mismatch in the middle of the prologue. This is ordinary
C++; no attributes, asm, manual data or shims.

## Measurement table (official scorer, full link, one build each)

`pct` is the report's 1-dp fuzzy; `size` is `usize` (our function size).

| variant | pct | size |
|---|---|---|
| HEAD baseline (chain `33/31/2c`) | 67.3 | 2764 |
| **chain `2c/31/33` (LANDED)** | **67.4** | 2764 |
| `setEmpData` after `KIPlayer *ship = nullptr;` | 67.4 | 2764 |
| `ship` declared at top of function | 67.4 | 2764 |
| `empTime` declared before `empPoints` | 67.4 | 2764 |
| `int empTime = 15000;` + conditional `= 45000` | 67.4 | 2764 |
| `empTime = 45000;` first in the `shipClass == 1` block | 67.4 | 2764 |
| `if (!shipClass)` | 67.4 | 2764 |
| `ship` uninitialised + `else { ship = nullptr; }` | 67.4 | 2764 |
| `hitpoints *= type == 14 ? 25 : 5;` -> if/else | 67.4 | 2764 |
| `hitpoints = level*14 + 20 + (...)` reassociation | 67.4 | 2764 |
| swap `x`/`y` declaration order | 67.4 | 2764 |
| named `float zero = 0.0f;` used by every BoundingAAB | 67.4 | 2764 |
| swap `y`/`z` declaration order | 67.2 | 2764 |
| `x,y,z` computed in `z,y,x` order | 67.2 | 2764 |
| `empTime` ternary (`shipClass == 1 ? 45000 : 15000`) | 66.4 | 2764 |
| `AEGeometry *geom = ...` local before `addObject` | 65.3 | 2760 |
| coords `i/i/f` | 64.0 | 2780 |
| `armor` before the type chain | 63.3 | 2784 |
| coords declared `int x,y,z` at top + casts at ctors | 63.1 | 2792 |
| coords `int/int/int` + casts at ctors | 63.1 | 2792 |
| coords `i/f/i` | 63.0 | 2780 |
| `armor` before the float hitpoint scaling | 62.5 | 2784 |
| coords `f/i/i` | 61.5 | 2784 |
| `armor` at top of function | 60.9 | 2780 |
| all-float coords (`f/f/f`) | 56.9 | 2784 |
| `Player` ctor before the float hitpoint scaling | 53.8 | 2736 |
| `if (shipClass > 0)` | 36.5 | 2760 |
| two independent `if (shipClass == ...)` blocks | 25.1 | 2692 |

(Also measured in wave 113 and unchanged: `switch(shipClass)` 63.2,
`switch(type)` hitpoints 67.2, cargo if-chain `2c/33/31` 67.2, `wp` decl
permutations — YZX best, `wpX=0,wpY=0,wpZ=0` init form 67.1.)

## Structural findings (why the rest does not move)

The residual 32.6 % is register allocation, not source shape. Measured from
`-mllvm -print-machineinstrs` MIR (slot-index dump after the machine
scheduler) for the current source:

* Live intervals (slot index, size): `this` 64-26496 (26432), `type`
  16-26160 (26144), `race` 48-26000 (25952), `player` 21648, the
  `new BoundingAAB` result vreg 20592, `volumes` 18320, `z` float 17728,
  `y` float 17664, `x` float 15168, **`shipClass` 32-6960 (6928)**.
  `shipClass` is the 10th largest interval, so by the time greedy RA reaches
  it every callee-saved register is taken and it is spilled to `sp60`.
* Original keeps `shipClass` in `r5`/`r10` and copies it to `r6`
  (`mov r6, r10` at `c02b4`) instead of spilling; it spills `race` to `sp60`
  at entry (`str r1,[sp,#60]`). Ours spills both with one `strd r1,r2,[sp,#56]`.
  This single allocator choice cascades: original `type` -> `r11` (fp),
  `hitpoints` -> `r5`, `empPoints` -> `r10`, `this` -> `r9` then `sp56`;
  ours `type` -> `r10` (sl), `this` -> `r9` then `sp52`, `race` -> `sp56`.
* Capital-ship branch: the four long-lived values are permuted between
  `{r6, r9, r10, r11}`. Original `fixed`=r9, `x`-bits=r10, `volumes`=r11,
  zero=r6; ours `x`-bits=r6, `volumes`=r9, zero=r10, `fixed`=r11. The
  S-register assignment already matches exactly (`x`=s20, `y`=s18, `z`=s16).
* `setEmpData` ordering: the MIR computes `x`/`y`/`z` *before* the call
  (slots 4624-4704 vs the call at 5104) but the PostRA machine sink moves
  them after it in our build; the original keeps them before
  (`c03c8`-`c03e4` then `c03ee blx setEmpData`). Source reordering does not
  change this.
* The `missionPtr != 1 && != 23` `addObject` block is inline (fallthrough
  into the cargo check) in the original and out-of-line (with a `b.n` back)
  in ours. Inverting/nesting the condition, or hoisting the geometry
  ternary into a local, does not move it.
* Ours materialises one extra dead `mov.w r8,#0` next to
  `mov.w r11,#0` before the `shipClass` branch; the original has only
  `mov.w r9,#0`. Restructuring `ship` (`else` arm, uninitialised, top of
  function) does not remove it.

## Gate

`cmake --build cmake-build-match --target check -j6`:
`RATCHET PASS`, byte_exact 4500, linked_exact 4500, stubs 0, missing 0,
extra 0, avg 99.92, data 843/843, imports 207, lint CLEAN, drift 0,
27 section headers, file size 2,187,328, `.ARM.extab` 0x26298 @0x1a3b78,
`.text` 0x1337c8 @0x69440.
