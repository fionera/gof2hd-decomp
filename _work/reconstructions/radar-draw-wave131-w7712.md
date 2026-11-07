# Radar::draw wave 131 (w7712) — loop-block nesting / draw-block order / literal-pool sweep (no source win)

**Status:** research note, 2026-11-07. Worktree `wt/w7712`, based on `master`
(`70e451ffe`). **No source change landed**; this commit contains this note only.
The target remains the pristine baseline `src/game/weapons/Radar.cpp`
(`sha256 9b315de8bf294d1f5bbc303ef37551b6d828598d57ea6c4a121882bba684fbd3`).

Target `_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).
Original 9908 B / 3492 normalised instructions; ours 9908 B / 3483.
Baseline unrounded `difflib.SequenceMatcher(None, on, un, autojunk=False).ratio()`
= **75.354839 %** (official display 75.4). Whole-`.text` differing bytes **85,379**
(`.text` size/base exact). Gate at start and end: `RATCHET PASS`, byte **4506**,
linked **4506**, size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0.

Read with `radar-draw-wave131-w7697.md`, `radar-draw-wave131-w7696.md`,
`radar-draw-wave129-w7674.md`, `radar-draw-body-wave122-w7210.md`.

This wave attacked the assignment's hypothesis directly: **planet/ship/asteroid
loop-block nesting, draw-block order, and literal-pool order**, using a fresh
Ghidra 12.1.2 decompile of `FUN_0013e050` (image base 0x10000) and the original
Thumb disassembly as ground truth. It adds evidence the earlier notes did not
have: a **direct block-by-block store/compare-order audit** of the four loops and
the landmark fallback, plus a **call-sequence delta** of the `bool otherLock`
variant against the original. Every variant below was built and scored; all land
at or below the baseline. The conclusion is unchanged: the baseline is a strong
local optimum and the residual is the coupled register-allocation fixed point.

## 1. Method / scratch

```sh
cd /home/debian/GalaxyOnFire2/wt/w7712
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6     # ~0.9 s per variant
cmake --build cmake-build-match --target check -j6        # RATCHET PASS
```

Ghidra decompile (new this wave; the provider is reachable with
`GHIDRA_INSTALL_DIR=/tmp/ghidra_12.1.2_PUBLIC`):

```sh
rea decompile /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --procedure 0x13e050 --format md
```

Scratch in `/dev/shm/w7712` (not committed): `Radar.base.cpp` (pristine source),
`score2.py` (single-symbol difflib harness reproducing 75.354839 exactly),
`textdiff.py` (whole-`.text` differing-byte count), `align.py` (normalised
instruction alignment), `calldelta.py` (call-sequence address delta),
`ghidra_code.c` (1815-line Ghidra decompile), `orig.dis`/`ours*.dis`.

## 2. The original's exact loop-block order (Ghidra + disasm)

The original's layout order is **landmark guard -> planet section -> planet loop
-> cold initial recompute -> planet-lock tail -> landmark setup -> landmark loop
-> enemies -> gas clouds -> asteroids -> music tail**. The landmark loop is
*outlined* after the planet section: the guard at `12e354` jumps forward to the
landmark setup at `12e7a8`, and the loop exit at `12eb42` branches **back** to the
planet section at `12e376`. Ours reproduces this arrangement; the only layout
difference is the landmark loop header placement (original top-placed at
`12e7d4`, ours bottom-placed at `12eb48`, `bcc.w` back-edge).

## 3. New measurements this wave (all single edits on the pristine baseline)

### 3.1 Landmark fallback draw-block order (`i==1` / `i==3`)

The original's fallback (`12e9ea..12ea14`) is:

```
12e9ea  bic.w   r0, sl, #0x80000000
12e9ee  cmp     r0, #1
12e9f0  beq.n   12e9fc          ; -> load image_0xbc
12e9f2  cmp     r0, #3
12e9f4  bne.n   12ea18          ; -> skip
12e9f6  ldr.w   r1, [r4, #184]  ; image_0xb8
12e9fa  b.n     12ea00
12e9fc  ldr.w   r1, [r4, #188]  ; image_0xbc
```

i.e. **`cmp #1` first**, `cmp #3` second. The baseline source writes
`else if ((i & 0x7fffffff) == 3) ... else if ((i & 0x7fffffff) == 1) ...`
and **already emits exactly this order** (`cmp #1` then `cmp #3`). The mask is
irrelevant: `(i & 0x7fffffff) == 3` and `i == 3` both emit the same `bic`/`cmp`
pair, and the emitted compare order is driven by the **source order** (writing
`==1` first flips the emitted order to `cmp #3` first and costs 0.057 points).
So the fallback is *not* a residual; the baseline is already correct here.

| variant | usize | raw % | `.text` diff |
|---|---:|---:|---:|
| **baseline** (`==3` then `==1`, mask) | 9908 | **75.354839** | 85,379 |
| `==1` then `==3`, mask (`mask_1_3`) | 9908 | 75.297491 | 85,379 |
| `i==3` then `i==1`, no mask | 9908 | 75.354839 | 85,379 |
| `i==1` then `i==3`, no mask | 9908 | 75.297491 | 85,379 |
| `switch (i & 0x7fffffff)` | 9908 | 75.297491 | 85,379 |

### 3.2 Planet loop nesting: two `continue`s vs one combined `if`

Ghidra renders the planet loop as
`if ((planetTargets->data()[i] != 0) && (update(...), onScreen != 0)) { body }`.
The baseline writes two early `continue`s (null check, then `!onScreen`). Wrapping
the body in a single `if (data[i] != nullptr) { update(); if (onScreen) { ... } }`
is byte-identical in size and score. It only re-shuffles the landmark fallback
compare order (see 3.1) and does not move the planet-loop register split.

| variant | usize | raw % | `.text` diff |
|---|---:|---:|---:|
| **baseline** | 9908 | **75.354839** | 85,379 |
| planet loop combined `if` | 9908 | 75.354839 | 85,379 |
| planet loop combined `if` + no-mask fallback | 9908 | 75.354839 | 85,379 |

### 3.3 Tractor-block store order (`dockNavPtr` vs `field_0x1c`)

The original stores **`dockNavPtr` (offset 8) before `field_0x1c` (offset 0x1c)**
in all three tractor blocks:

```
12f17a  str.w r8, [r4, #8]    ; ship tractorMode2:  dockNavPtr
12f17e  str.w r8, [r4, #28]   ;                     field_0x1c
12fce2  str   r6, [r4, #8]    ; asteroid minable:   dockNavPtr
12fce6  str   r6, [r4, #28]   ;                     field_0x1c
12fcc4  eors  r0, sl, #1
12fcca  streq r6, [r4, #8]    ; asteroid tractorMode2: dockNavPtr
12fccc  streq r6, [r4, #28]   ;                        field_0x1c
```

The baseline source writes `field_0x1c` first in those three places. Swapping the
source order to match the disassembly **loses 0.057 points** (clang schedules the
two stores independently of source order; the swap re-rolls a nearby block). The
original's store order is therefore not recoverable from the source here.

| variant | usize | raw % |
|---|---:|---:|
| **baseline** (`field_0x1c` then `dockNavPtr`) | 9908 | **75.354839** |
| ship tractorMode2 swapped | 9908 | 75.297491 |
| asteroid minable swapped | 9908 | 75.297491 |
| asteroid tractorMode2 swapped | 9908 | 75.297491 |
| all three swapped | 9908 | 75.297491 |

### 3.4 Loop forms / iterator reuse / block hoisting

Every loop-form spelling is a tie or a loss; none moves the landmark loop header
or the planet-loop register split.

| variant | usize | raw % |
|---|---:|---:|
| **baseline** | 9908 | **75.354839** |
| landmark `for (uint32_t i...)` | 9908 | 75.354839 (tie) |
| landmark `if (i < 2)` instead of `i <= 1` | 9908 | 75.354839 (tie) |
| planet `for (int i...)` | 9908 | 75.354839 (tie) |
| ship `for (int i...)` | 9908 | 75.354839 (tie) |
| asteroid `for (int i...)` | 9908 | 75.354839 (tie) |
| gas loop `for (uint32_t i = 0; ...)` (no pre-decl) | 9908 | 75.297491 |
| asteroid slot loop `k` -> `i` (iterator reuse) | 9908 | 75.354839 (tie) |
| asteroid `best`/`bestDistance` declaration swap | 9908 | 75.354839 (tie) |
| hoist `bool autoPilot = isAutoPilot()` (asteroid) | 9908 | 75.297491 |
| negate ship guard `!(a||b||c)` -> `!a&&!b&&!c` | 9908 | 75.354839 (tie) |
| negate gas guard | 9908 | 75.354839 (tie) |
| negate asteroid guard | 9908 | 75.354839 (tie) |
| remove unused `bool dead` in ship loop | 9892 | 74.2 |

### 3.5 First `otherLock` guard: `bool` vs `int` (re-confirmed with call delta)

Ghidra independently types `otherLock` as `bool` (`bVar46`), and the original
materialises **both** `notTurretMode` (`eor r0,r0,#1`) and `notOtherLock`
(`eor r0,r5,#1`) at the landmark setup `12e7c2/12e7c8`. The baseline folds
`!otherLock` into `cmp r9,#0` and emits only the `notTurretMode` `eor`. Declaring
`otherLock` `bool` reproduces the original's `cbnz` chain, the landmark-setup
`eor`, and the first literal pool at `12e340` — but the whole `.text` becomes
**96 B smaller** (9812 B function) and the ratio drops to 69.5.

A call-sequence delta (`calldelta.py`) shows the 96 B is **distributed**, not one
block: the bool variant is +20 B at the route block, -8 B at the landmark guard,
-12 B at the planet loop, -10 B in the planet body, and reaches -96 B only in the
music tail and the `String::~String` landing pads. So the bool type is almost
certainly the original's, but it enables ~96 B of compaction somewhere the
original does not have it (most likely the later `otherLock = ...` OR-assignments
and the `!otherLock` uses). Every bool combination tried this wave reproduces the
same 9812 B floor:

| variant | usize | raw % |
|---|---:|---:|
| **baseline** (`int`, negated `&&`, expect 0) | 9908 | **75.354839** |
| `bool otherLock`, positive `||` | 9812 | 69.500072 |
| `bool otherLock`, positive `||` + expect(1) | 9812 | 69.1 |
| `bool otherLock` + negated `&&` | 9812 | 69.1 |
| `bool otherLock` + `!a&&!b&&!c` guards | 9812 | 69.500072 |
| `bool otherLock` + remove post-loop recompute | 9792 | 66.8 |
| `bool otherLock` + `camp_reload` | 9808 | 68.4 |
| `bool otherLock` + remove recompute + `camp_reload` | 9776 | 66.9 |
| `int otherLock = 1; if (all null) ...` + expect(0) | 9908 | 75.326165 |
| `int otherLock = 1; if (all null) ...` (no expect) | 9908 | 74.179211 |
| `int otherLock = field_0x14 != 0; if (any) otherLock = 1` | 9856 | 73.511648 |

### 3.6 `notOtherLock` / `notTurretMode` spellings

`int notOtherLock = otherLock ^ 1;` and `int notOtherLock = (otherLock == 0);`
are byte-identical to the baseline (`!otherLock`) and tie at 75.354839. `bool`
locals land at 9920 B / 71.963287.

## 4. Root cause (unchanged)

The residual is the coupled fixed point already described in waves 122/127/128/129:

1. the first `otherLock` guard's `cbz`+`itt` shape and the first pool at
   `12e310` (original `cbnz` chain, pool at `12e340`);
2. the invented post-loop recompute at `12e72c` (original has only the cold
   initial recompute at `12e6ee`), which displaces the planet-lock tail;
3. `stations` in `r6` instead of `sl`+spill, and the `turretMode` slot swap;
4. the bottom-placed landmark loop header (`12eb48` vs `12e7d4`);
5. the ship `inSight` `s24`/`s26` live range (`vpush {d8-d13}` vs `{d8-d11}`).

The new evidence this wave rules out the two cheapest remaining levers: the
landmark fallback compare order is already correct in the baseline, and the
tractor store order cannot be recovered from source (clang schedules it). The
`bool otherLock` type is confirmed but is a 96 B *loss*, not a win, because the
later `otherLock` uses compact too aggressively.

## 5. Gate / section deltas (baseline, unchanged)

```
compared 4524  avg 99.94%  byte_exact 4506  linked_exact 4506  fuzzy_perfect 4506
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Section deltas vs the original: `.text` 85,379 differing bytes (size/base
exact), `.ARM.extab` 1,420, `.note.gnu.build-id` 20. `.rodata`, `.data`,
`.bss`, `.got`, `.ARM.exidx`, `.rel.plt`, `.rel.dyn`, `.dynsym`, `.dynstr`,
`.hash` are byte-identical.
