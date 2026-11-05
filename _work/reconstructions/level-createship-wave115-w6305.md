# Level::createShip — wave 115 (w6305): empPoints position reaches the target prologue

Status: **analysis only, no source change landed** (2026-11-05).
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`,
original 2764 B @ `0xc0214`, Thumb entry `0xc0215`). Branch
`wave115-w6305`, base `b847d5d46`.

## Result

```
                                   before (HEAD b847d5d46)   after
createShip fuzzy (raw)             67.366317                 67.366317
createShip fuzzy (report, 1 dp)    67.4                      67.4
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

No variant beat the baseline. The gate was re-run on the pristine source
after the sweep: `RATCHET PASS`, byte 4500 / linked 4500, avg 99.92,
data 843/843, imports 207, lint CLEAN, drift 0.

The scorer is `tools/verify/verify.py`'s `score()`, which is
`round(difflib.SequenceMatcher(None, on, un, autojunk=False).ratio()*100, 1)`.
The raw ratio is quoted below because several variants round to the same
1-dp value.

## What is new in this wave

The target prologue **is reachable** with ordinary C++, but only in a
source shape whose emitted statement order is wrong:

```
target (original)                    best new shape (62.6 %)
str  r1, [sp, #60]   ; race          str  r1, [sp, #56]   ; race
mov  r9, r0          ; this          mov  r9, r0
ldr  r0, [pc, #572]                  ldr  r0, [pc, #588]
mov  fp, r3          ; type -> fp    mov  fp, r3          ; type -> fp
mov  r5, r2          ; shipClass     mov  sl, r2          ; shipClass -> sl
add  r0, pc                          add  r0, pc
ldr  r4, [r0, #0]    ; status -> r4  ldr  r4, [r0, #0]    ; status -> r4
ldr  r0, [r4, #0]                    ldr  r0, [r4, #0]
blx  getCurrentCampaignMission       blx  getCurrentCampaignMission
mov  r8, r0                          mov  r8, r0
ldr  r0, [r7, #8]                    ldr  r0, [r7, #8]
mov  sl, r5                          cmp  r0, #0
cbz  r0, ...
```

The shape that produces it:

```cpp
int empPoints = (Globals::status->getLevel() > 20 ? 20 : Globals::status->getLevel()) * 5 + 40;
int level = Globals::status->getLevel() > 20 ? 20 : Globals::status->getLevel();
int hitpoints = (Globals::status->gameWon() ? 180 : mission * 4) + level * 14 + 20;
...
int empTime = shipClass == 1 ? 45000 : 15000;   // ternary, not if/else
if (shipClass == 1) { ... }
...
int x = wpX + jitterX - 20000;
int y = wpY + jitterY - 20000;
int z = wpZ + jitterZ - 20000;
```

Two independent ingredients:

1. **`empPoints` computed before `level`/`hitpoints`** (anywhere before
   the `level` line) moves `status` from `r6` to `r4` in every
   coordinate pattern swept. `empPoints` declared/assigned after
   `hitpoints` (the current source) always gives `status -> r6`.
2. **`empTime` as a ternary** then moves `type` from `sl` to `fp` for
   the two-int coordinate patterns (`i/i/i`, `i/i/f`, `i/f/i`). With
   the `if/else` form `type` stays in `sl`.

In this family the `Player` ctor and `setEmpData` argument registers
also match the original exactly:

```
mov r2, fp   ; hitpoints -> fp   (original 0xc03c0)
mov r1, sl   ; empPoints -> sl   (original 0xc03ea)
mov r2, r9   ; empTime   -> r9   (original 0xc03ec)
```

The remaining prologue deltas are (a) `shipClass` in `sl` directly
instead of the original's split `mov r5,r2` / `mov sl,r5`, and (b) the
race spill slot (`56` vs `60`, a consequence of (a)).

The price is the emitted order of the `getLevel` calls: the
`empPoints` pair is emitted *before* the `hitpoints` type chain,
whereas the original emits it *after* the `mission == 49..52/56`
block (`0xc0314`). That single reorder costs ~5 points of sequence
alignment, more than the prologue gain.

## Measurement table (full link, official scorer)

`raw` is the unrounded `difflib` ratio; `pct` is the report's 1-dp
value; `size` is `usize`; `n` is orig/ours instruction count.

| variant | raw % | pct | size | n |
|---|---|---|---|---|
| **HEAD baseline** (`i/f/f`, `empPoints` after mission, if/else `empTime`) | **67.366317** | **67.4** | 2764 | 1007/994 |
| `empPoints` after type chain | 66.066967 | 66.1 | 2764 | 1007/994 |
| `empPoints` after type chain + ternary | 63.972056 | 64.0 | 2776 | 1007/997 |
| `empPoints` before `level` (`i/f/f`) | 63.387978 | 63.4 | 2760 | 1007/1006 |
| `empPoints` before `level` + ternary (`i/f/f`) | 61.058882 | 61.1 | 2784 | 1007/1014 |
| `i/i/i` coords (baseline order) | 63.126551 | 63.1 | 2792 | 1007/1008 |
| `i/i/f` coords (baseline order) | 64.015905 | 64.0 | 2780 | 1007/1005 |
| `i/f/i` coords (baseline order) | 63.021869 | 63.0 | 2780 | 1007/1005 |
| `f/f/f` coords (baseline order) | 56.858847 | 56.9 | 2784 | 1007/1005 |
| **`i/i/i` + `empPoints` before `level` + ternary** | **62.562066** | **62.6** | 2800 | 1007/1007 |
| `i/i/f` + `empPoints` before `level` + ternary | 62.195728 | 62.2 | 2792 | 1007/1006 |
| `i/f/i` + `empPoints` before `level` + ternary | 61.102832 | 61.1 | 2792 | 1007/1006 |
| `level` as `getLevel(); if (>20) level=20;` | 63.963964 | 64.0 | 2756 | 1007/991 |
| `mission` block as `switch` | 67.366317 | 67.4 | 2764 | 1007/994 |
| `mission == 154` instead of `0x9a` | 67.366317 | 67.4 | 2764 | 1007/994 |
| `hitpoints` declared before `level` | 67.366317 | 67.4 | 2764 | 1007/994 |
| `empPoints` before the coordinate block | 67.366317 | 67.4 | 2764 | 1007/994 |
| `ship` declaration after `setEmpData` | 67.366317 | 67.4 | 2764 | 1007/994 |
| `armor` before the alien block | 62.809100 | 62.8 | 2784 | 1007/1015 |
| coords before `new Player` | 62.151394 | 62.2 | 2788 | 1007/1001 |

Also swept (prologue only, 576 variants, full `Level.cpp` compile each):
8 coordinate type patterns x 6 `ship` declaration modes (`= nullptr`,
`= 0`, top, before `setEmpData`, before `Player`, `else { ship = nullptr; }`)
x 2 `armor` forms (variable / inlined into `new Player`) x 3 `empTime`
forms (if/else, ternary, `= 15000` + conditional) x 2 `level` forms
(variable / inlined) x 2 `empPoints` positions. Every `type -> fp` hit
requires `empPoints` before `level`; every `status -> r4` hit requires
`empPoints` before `hitpoints`; no hit in the family reaches
`shipClass -> r5` or the `mov sl,r5` split.

`int cls = shipClass;` copies (placed after the mission call, before
the early check, before the late checks, or after the `shipClass == 1`
block; used in the early check, the late checks, or both) are all
coalesced away by clang 7.0.2 and change nothing.

## Register-allocation evidence

Live-interval sizes were recomputed from a fresh
`-mllvm -print-machineinstrs` dump of the committed build (own
backward-liveness over the post-scheduler MIR; size in MIR slot units):

```
%128 this      18160     %480 VMOVRS x-bits   4768
%131 type      17616     %478 VMOVRS y-bits   4768
%129 race      16800     %479 VMOVRS z-bits   4768
%46  player    14768     %130 shipClass       3408
%48  volumes   12352     %671/672/673 wp*     3072/2896/2880
                         %139 mission         2240
```

The three 4768-slot `VMOVRS` intervals are the `vmov rN, sM` moves of
the x/y/z float bits used by every `BoundingAAB` call in the capital
branch. They are live across the whole unrolled block and are all
hinted (`COPY $r0`-class), so the greedy allocator places them before
`shipClass` (3408) and `shipClass` ends up with no free callee-saved
register in the current shape.

The original avoids this: its x/y/z are ints held in `fp`/`r4`/`r8`
(`0xc03dc`/`0xc03e2`/`0xc03e4`) and converted with `vcvt` only inside
the two ctor branches. The `i/i/i` shape reproduces the ints and the
spawn slot map (`{A:44,B:48,C:40}` / rnd calls `[36,32,28]`) and even
the instruction count (1008 vs 1007), but its allocator output is worse
everywhere else (63.1 %).

The original's `shipClass` is split into three register segments
(`r5` entry->`0xc0238`, `sl` ->`0xc02b4`, `r6` ->`0xc03fc`). The split
copies (`mov sl,r5` at `0xc0238`, `mov r6,sl` at `0xc02b4`) are not
reproducible from any faithful source shape swept: clang either keeps
the value in one register or spills it.

## Conclusion

The target prologue is not mutually exclusive with the target spawn
slot map (the wave-92 pass-6 result), but it *is* mutually exclusive
with the original's emitted `getLevel` order in every family swept.
The best target-prologue shape is 62.562066 % (62.6, 2800 B, 1007
instructions), below the 67.366317 % baseline. No faithful C++ change
beats the baseline; the residual is register allocation driven by the
`empPoints`/`level` statement order and the `shipClass` live-range
split.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6305
cmake --build cmake-build-match --target check -j6
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5Level10createShipEiiiP8Waypointbb
```

Scratch helpers (all under `/dev/shm/w6305`): `Level.cpp.base` (pristine
source), `searchlib.py` (parallel `Level.cpp` compile + prologue
extractor), `gen.py` / `gen2.py` (variant families), `live.py`
(post-scheduler MIR liveness / interval sizes), `score3.py` (unrounded
official ratio), `mir2.txt` (committed-build `-print-machineinstrs`
dump), `mir_iii.txt` (`i/i/i` dump).

## Wave-115 continuation (second pass, same worker)

The pristine `HEAD` source was re-measured after the sweep above:
`raw=67.366317  pct=67.4  osize=2764  usize=2764  n=1007/994`.
Gate re-run: `RATCHET PASS`, byte 4500 / linked 4500, data 843/843,
imports 207, lint CLEAN, drift 0; file size 2,187,328; 27 sections;
`.text` 0x1337c8 @0x69440; `.ARM.extab` 0x26298 @0x1a3b78;
`.ARM.exidx` 0x6f70 @0x19cc08; `.rodata` 0x41c27.

### Additional measurements (official scorer, full link)

| variant | raw % | pct | size | n |
|---|---|---|---|---|
| **HEAD baseline** | **67.366317** | **67.4** | 2764 | 1007/994 |
| best target-prologue shape (`i/i/i` + empPoints before `level` + ternary) | 62.462761 | 62.5 | 2800 | 1007/1007 |

(The earlier table's 62.562066 for this shape was measured before the
wave-114 `0x2c/0x31/0x33` chain-order commit; re-measured on the current
base it is 62.462761. Both are below the 67.366317 baseline.)
| `i/i/i` + `ship` at top + ternary empTime | 62.879920 | 62.9 | 2780 | 1007/1000 |
| `i/i/i` + inline `level` | 62.034739 | 62.0 | 2792 | 1007/1008 |
| wave-92 placement-new body (spliced, `try`/`catch` anchor) | 74.379345 | 74.4 | 2764 | 1007/1007 |
| wave-92 body + `try`/`catch` on the real-`new` shape | 63.500000 | 63.5 | 2768 | 1007/993 |

Also measured, all exactly `67.366317` (no codegen change): `int cls =
shipClass` at four placements; `this->missionPtr`; `Level *self = this`;
`int mp = missionPtr`; `KIPlayer *kiFighter = fighter`; `AEGeometry *geom`
local; `if`/`else` geometry select; `Status *status = Globals::status`
alias (61.7, worse); hoisted `int empPoints;` / `int hitpoints;`;
`empPoints` expression `40 + ...*5`; `if (waypoint)`; `wpX = wpY = wpZ = 0`;
`const int` jitters; jitter declarations at the top; `wpX += rnd(...)`
instead of separate jitter locals; `float opt = options[0x2c]`;
`const int armor`; `empPoints *= 3` before/after the hitpoint multiply;
`hitpoints` if/else instead of ternary; `hp scaling` before `empPoints`.

The `try`/`catch` anchor is not usable: it grows `.text` to 0x1337e8 and
`.ARM.extab` to 0x262a8, breaking the section budget.

### Local proxy sweep (3072 variants)

A local `Level.o` scorer (same NDK clang flags, `objcopy` the
`.text._ZN5Level10createShip...` section, `verify.disassemble` at base 0,
pool words included, external `bl`/`blx` unified) ranks variants with the
same ordering as the official scorer on the six calibration points
(base 58.17 -> 67.37, ternary 57.34 -> 66.4, `i/i/f` 54.47 -> 64.0,
`i/i/i` 54.29 -> 63.1, `i/f/i` 53.58 -> 63.0, `f/f/f` 48.11 -> 56.9).

Cross product of 8 coordinate type patterns x 2 waypoint declaration
orders x 2 jitter declaration orders x 6 `ship` declaration modes x 2
`armor` forms x 3 `empTime` forms x 2 `level` forms x 3 `empPoints`
positions (3072 variants) found **no variant above the baseline proxy
58.1709**; the next-best `iff`/`iif` shapes reach 58.00 / 57.91.

### Why the wave-92 body scores 74.4

Splicing the wave-92 `createShip` body (placement `new` + `try`/`catch`
around `setEmpData` + `switch (type)` hitpoint chain) into the current
tree reproduces `74.379345 %`. Its capital-ship block keeps the
`BoundingAAB` zero argument in `r0` (`movs r0,#0` / `str r0,[sp,#0]`),
which is what the original does, whereas the real-`new` shape hoists the
zero into `sl` (`mov.w sl,#0` / `strd sl,r0,[sp]`). The placement-new
body's `.ARM.extab` record is 332 B, so it cannot be landed without
breaking the 336 B / 0x26298 budget.

### Conclusion (unchanged)

No faithful real-`new` C++ variant beats `67.366317 %`. The residual is
the `empPoints`/`level` statement order (which decides whether
`empPoints` lives in `sl` or `fp`) plus the `shipClass` live-range split;
both are allocator decisions that no source shape in the swept families
reproduces together with the original's emitted `getLevel` order.
