# Level::createShip — pass 10 (wave 99, w4906)

**Status:** analysis only, no source change landed. Target
`_ZN5Level10createShipEiiiP8Waypointbb` (original 2764 B @ `0xc0214`,
Thumb entry `0xc0215`, 1007 insns). Verifier score **74.379345 %**
(report rounds 74.4), `osize == usize == 2764`, 1007 insns on both
sides, `byte False`. This pass continues `level-createship-pass9-w4807.md`.

Gate before and after (unchanged, no code change):

```text
byte_exact 4500/4524  linked_exact 4500/4524  size-exact 4524/4524
stub_zero_size 0  missing 0  wrong_type 0  extra 0  data_exact 843/843
imports 207  .text 0x1337c8  .rodata 0x41b67  .ARM.exidx 0x6f70
.ARM.extab 0x26294 (-4)  lint CLEAN  drift 0  RATCHET PASS
```

## 0. Result

The committed source is confirmed to be a **strict local optimum** of a
much larger measured space than pass 9 covered. 61 builds were made
and scored this pass (all with the official verifier; several are
instruction-identical to each other); **none beats 74.379345 %**. The three residual deltas of pass 9 are unchanged:

1. the `this` spill (`str.w r9,[sp,#56]`) is one basic block early
   (`0xc031c` vs `0xc0332`);
2. the spawn block is the mixed `int x / float y / float z` semantic
   hack, not the original's three-waypoint-plus-jitter `int` form;
3. the downstream register colouring (`volumes`, `x`, `y`, `z`, `type`,
   `race`) differs.

The prologue and the frame-index (slot) map are **already byte-exact**
and were re-confirmed this pass; "fix the prologue/FI order" is done.
What remains is driven entirely by the spawn's `int`/`float` mix.

## 1. Byte-identical regions (fresh objdump, committed build)

```text
orig c0214..c031c  ==  ours c0214..c031c   (100 insns: prologue, waypoint
                                            loads, 3x rnd, level ternary,
                                            hitpoints + type switch,
                                            mission==49..56 mask)
orig c0336..c03c8  ==  ours c0336..c03c8   (empPoints tail, shipClass
                                            branch, options adjust,
                                            mission==0x9a, hardCoreMode,
                                            operator new, Player ctor)
```

Slot map (re-verified from the current build, `objdump`):

```text
waypoint->x -> [sp,#44]   waypoint->y -> [sp,#48]   waypoint->z -> [sp,#40]
1st rnd     -> [sp,#36]   2nd rnd     -> [sp,#32]   3rd rnd     -> [sp,#28]
```

This is exactly the original's map. The committed declaration order
`int wx, wz, wy;` + `wz=wp->x, wy=wp->y, wx=wp->z` and the jitter order
`jitterY, jitterZ, jitterX` are the unique optimum (pass 7 §4, pass 9 §3,
re-measured here).

## 2. The original spawn, decoded exactly

```text
c03c8: ldr r2,[sp,#48]   ; wy
c03ca: ldr r3,[sp,#32]   ; j2
c03cc: ldr r0,[sp,#44]   ; wz
c03ce: ldr r1,[sp,#36]   ; j1
c03d0: add r2,r3         ; wy + j2
c03d2: ldr r3,[sp,#40]   ; wx
c03d4: ldr r4,[sp,#28]   ; j3
c03d6: add r0,r1         ; wz + j1
c03d8: movw r1,#20000
c03dc: sub.w fp,r0,r1    ; x = wz + j1 - 20000
c03e0: add r3,r4         ; wx + j3
c03e2: subs r4,r2,r1     ; y = wy + j2 - 20000
c03e4: sub.w r8,r3,r1    ; z = wx + j3 - 20000
c03e8: mov r0,r5         ; player
c03ea: mov r1,sl         ; empPoints
c03ec: mov r2,r9         ; empTime
c03ee: blx Player::setEmpData(int,int)
```

i.e. the faithful all-`int` source

```cpp
int x = wz + jitterY - 20000;   // 44 + 36
int y = wy + jitterZ - 20000;   // 48 + 32
int z = wx + jitterX - 20000;   // 40 + 28
```

with all three `vcvt.f32.s32` *after* the call, duplicated in both
`shipClass` branches (`0xc040c..0xc0422` and `0xc06d0..0xc06d8`).
The committed source instead computes

```cpp
int x = jitterZ + jitterX - 20000;          // 32 + 28, sunk past the call
float y = (float)(wy + jitterY - 20000);    // 48 + 36, vcvt before the call
float z = (float)(wz + wx - 20000);         // 44 + 40, vcvt before the call
```

which is a *semantic hack*: `y`/`z` are swapped at the ctor call and `z`
uses no jitter at all.

## 3. Why the all-`int` spawn loses the exact prologue

The original's entry allocation is

```text
str r1,[sp,#60]   ; race -> stack
mov r9,r0         ; this -> r9
ldr r0,[pc,#572]  ; Globals::status
mov fp,r3         ; type -> fp
mov r5,r2         ; shipClass -> r5
```

The faithful all-`int` build (with or without the `try`, with or without
`ship` hoisted, with `Level *self`, with explicit `(float)` casts, with
`int x,y,z;` declared separately) *always* emits

```text
str r2,[sp,#60]   ; shipClass -> stack
mov r9,r3         ; type -> r9
str r0,[sp,#56]   ; this -> stack
mov sl,r1         ; race -> sl
```

The mechanism is visible in the rnd block. In the original the 3rd
`nextInt` result lands in `r5` and is spilled immediately:

```text
c0282: mov r5,r0
c028c: str r5,[sp,#28]     ; j3 -> [sp,#28]
```

In the all-`int` build the 3rd result lands in `r6` and stays there
across the whole function (used at `c03da`):

```text
c0280: mov r6,r0           ; j3 -> r6, never spilled
```

One extra callee-saved GPR (`r6`) held across the `Player` ctor and the
`setEmpData` call raises the entry pressure by one and forces `this`
out of `r9` into `[sp,#56]` and `type` from `fp` into `r9`. That is a
whole-function Greedy-RA fixed point: it is not localised to the spawn
block and cannot be undone by any of the source permutations below.

## 4. Residual #1 — the `this` spill is one block early

```text
original                              committed
c0314  ldr   r0,[r4,#0]               c0314  ldr   r0,[r4,#0]
c0316  blx   Status::getLevel         c0316  blx   Status::getLevel
c031a  cmp   r0,#20                   c031a  cmp   r0,#20
c031c  ble.n c0322                    c031c  str.w r9,[sp,#56]   <- here
c031e  movs  r0,#20                   c0320  ble.n c0326
c0320  b.n   c0328                    c0322  movs  r0,#20
c0322  ldr   r0,[r4,#0]               c0324  b.n   c032c
c0324  blx   Status::getLevel         c0326  ldr   r0,[r4,#0]
c0328  add.w r0,r0,r0,lsl#2           c0328  blx   Status::getLevel
c032c  cmp   r6,#1                    c032c  add.w r0,r0,r0,lsl#2
c032e  add.w sl,r0,#40                c0330  cmp   r6,#1
c0332  str.w r9,[sp,#56]   <- here    c0332  add.w sl,r0,#40
c0336  str.w fp,[sp,#52]              c0336  str.w fp,[sp,#52]
c033a  bne.n c0352                    c033a  bne.n c0352
```

Both forms are 4 bytes, so the block shift cancels and the stream
re-syncs exactly at `0xc0336`. This is the Greedy-RA live-range split of
`%124` (`this`, live `64B..26224B`): pass 9 proved from the
`-print-after-all` dumps that the store is already in its final block in
the Virtual Register Rewriter dump (right after Greedy RA) and that the
PostRA Machine Sink does not touch it. The original splits the same
interval at the end of `bb.17`; ours splits it at the end of `bb.14`.

## 5. Residual #2 — downstream register colouring

Everything from `0xc03f6` to the epilogue is the same instruction
sequence with a different colouring:

```text
original : volumes = fp   z = r4   x = sl   y = r8   type = r4   race = r6
committed: volumes = r9   z = sl   x = r4   y = r6   type = r6   race = r4
```

The 24 `BoundingAAB` constructions dominate the diff: each
`ldr.w r0,[fp,#4]` vs `ldr.w r0,[r9,#4]` and each
`movs r6,#0 / str r6,[sp,#0]` vs `movs r0,#0 / str r0,[sp,#0]` is a
pure colouring delta. It is a consequence of the spawn's `int`/`float`
mix (the two `float` coordinates occupy `s16`/`s18`, freeing two GPRs
and changing the Greedy-RA colouring). It cannot be fixed independently
of the spawn.

## 6. Variants measured this pass (official verifier)

All variants replace only the spawn block (and, where stated, one
structural line) of the committed body.

| variant | raw % | usize |
|---|---|---|
| **committed baseline** | **74.379345** | **2764** |
| all-`int` faithful (`wz+jY, wy+jZ, wx+jX`) + `try` | 61.561412 | 2752 |
| all-`int` faithful, no `try` | 61.691542 | 2748 |
| all-`float` faithful | 64.964965 | 2748 |
| faithful pairing, `int x / float y / float z` | 73.647643 | 2764 |
| faithful pairing, `float x / int y / float z` | 72.555831 | 2764 |
| faithful pairing, `float x / float y / int z` | 72.492552 | 2764 |
| faithful pairing, `int x / int y / float z` | 68.394816 | 2780 |
| faithful pairing, `int x / float y / int z` | 67.397807 | 2780 |
| faithful pairing, `float x / int y / int z` | 67.298106 | 2780 |
| all-`int` + `KIPlayer *ship` at function top | 61.561412 | 2752 |
| all-`int` + `Level *self = this` | 61.561412 | 2752 |
| all-`int` + `PlayerFixedObject *ship` | 42.878412 | 2736 |
| all-`int` + explicit `(float)` casts at ctor | 61.561412 | 2752 |
| all-`int` + `int x, y, z;` then assignments | 61.561412 | 2752 |
| mixed + `Level *self = this` | 74.379345 | 2764 |
| mixed + `Level *self` at top | 74.379345 | 2764 |
| mixed + `KIPlayer *ship` at top | 74.379345 | 2764 |
| mixed + `PlayerFixedObject *ship` | 35.100000 | 2700 |
| mixed + `volumes` declared before `fixed` | 59.333001 | 2760 |
| mixed + `volumes` declared at top | 74.379345 | 2764 |
| all-`int` + `volumes` before `fixed` | 59.513164 | 2756 |
| mixed + `float zero = 0.0f` local | 74.379345 | 2764 |
| mixed + separate `empLevel` variable | 74.379345 | 2764 |
| all-`int` + separate `empLevel` | 61.561412 | 2752 |
| mixed + `40 + (...) * 5` | 74.379345 | 2764 |
| mixed + `empPoints` if-form | 74.379345 | 2764 |
| mixed + `level` if-form | 74.379345 | 2764 |
| mixed + `empTime` ternary | 72.293942 | 2764 |
| all-`int` + `empTime` ternary | 61.064147 | 2752 |
| mixed + `empTime = 15000` default + override | 74.379345 | 2764 |
| spawn moved inside the `try` (all-`int` / mixed) | 26.659697 | 2580 |
| spawn before the `try` (all-`int`) | 61.561412 | 2752 |
| spawn before the `try` (mixed) | 74.379345 | 2764 |
| spawn hoisted to just after the jitters (all-`int`) | 61.033797 | 2752 |
| spawn hoisted to just after the jitters (mixed) | 33.988906 | 2716 |
| operand-order swaps (x / y / z / all) | 74.379345 | 2764 |
| `int x = jZ + jX; x -= 20000;` | 74.379345 | 2764 |
| float subtraction for `y` | 70.833333 | 2772 |
| float subtraction for `z` | 70.173697 | 2772 |
| float subtraction for both | 71.130952 | 2776 |
| two independent `if`s instead of `else if` | 34.444998 | 2712 |
| explicit empty `else` | 74.379345 | 2764 |
| `KIPlayer *ship = 0;` | 74.379345 | 2764 |

Every all-`int` variant above produced the identical entry allocation
(`str r2,[sp,#60]; mov r9,r3; str r0,[sp,#56]; mov sl,r1`). Every
mixed variant that keeps `int x / float y / float z` produced the
byte-exact prologue and the `this` spill at `0xc031c`.

## 7. Conclusion / next lever

The committed source is a strict local optimum at **74.379345 %**.
The prologue, slot map, FI order, hitpoints/empPoints/empTime arithmetic
and `Player` ctor are already byte-exact. The remaining 457-instruction
diff is entirely a whole-function Greedy-RA colouring difference driven
by the spawn's `int`/`float` mix:

* the faithful all-`int` spawn is what the binary actually computes, but
  our clang 7.0.2 `-Oz` build assigns the 3rd `nextInt` result to `r6`
  (kept live across the whole function) instead of `r5` (spilled at
  `0xc028c`), which raises entry pressure and forces `this` out of `r9`;
* the committed mixed hack keeps the original's entry allocation (and
  therefore the exact prologue) precisely because it drops two GPRs
  from the spawn, so it is *closer* to the original overall even though
  its spawn block is semantically wrong.

A shape that keeps the mixed-type entry allocation **and** the all-`int`
spawn scheduling was not found in this pass (61 builds) nor in pass 9
(216 + 54 + 48 variants). The next real lever is a source construct that
makes the 3rd `nextInt` result spill to `[sp,#28]` early in the all-`int`
shape; that single register-assignment change should recover the entry
allocation and, with it, most of the 457 diffs.

## 8. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w4906
cmake --build cmake-build-match --target check -j8
python3 /dev/shm/w4906/score2.py        # official ratio + prologue + spill
python3 /dev/shm/w4906/opcodes.py       # opcode-aligned diff
python3 /dev/shm/w4906/batchB.py vX.json   # sweep a JSON variant set
```

Scratch helpers under `/dev/shm/w4906`: `score2.py` (official ratio,
this-spill offset, first 8 prologue insns), `opcodes.py`
(difflib-aligned opcode diff), `batchB.py` (JSON-driven variant sweep),
`run2.py` (single variant), plus the pass-9 LLVM dumps `gra.txt`,
`li.txt`, `sink_before.txt`, `sink_after.txt`.
