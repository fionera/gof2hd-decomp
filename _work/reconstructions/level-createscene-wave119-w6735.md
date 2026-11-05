# `Level::createScene` — wave-119 w6735: no source win over 92.7573 %

**Status:** dead-end sweep, 2026-11-05. Worktree `wt/w6735`, branch
`wave119-w6735`. Target `_ZN5Level11createSceneEv`. No source change landed;
this note records the measurements so the next pass does not repeat them.

Continuation of `level-createscene-wave119-w6709.md` (which landed the `-g`
Vector-address trick, the y/z/x store order and the 78 `rnd` mask, 90.7243 ->
92.7573 %). The wave-119 landed shape is still the unique maximum found.

## TL;DR

* Baseline and best: **92.7573 % / 2252 B**, 783/791 instructions, first
  diff at instruction 478 (parts-loop store schedule).
* The function is instruction-exact up to instruction 477. The only two
  differing regions are the parts-loop store schedule (478-483) and the
  ship-loop register allocation (548 onward).
* ~90 source variants were measured this session (Vector construction and
  store order, address-trick spellings, `rnd` declaration position, `rnd`
  local-vs-global masks, variable types, branch order, statement order,
  preamble order, volatile/static/reference locals). **None beat
  92.7573 %**; the best non-landed variant is the constructor form at
  88.3100 % (it fixes the store order but adds one address instruction).
* Gate unchanged: **byte_exact 4505/4524, linked_exact 4505/4524,
  stub_zero_size 0, missing 0, wrong_type 0, extra 0, data_exact 843/843,
  imports 207, lint CLEAN, drift 0, RATCHET PASS.** 27 sections, file size
  2,187,328 B, `.ARM.extab` 0x26290 (pre-existing createShip -8).

## 1. The two remaining diff regions

### 1.1 Parts loop (instructions 478-483)

Original (`g->setRotation(rot)` — one-instruction address, three separate
stores, zero materialised after the y store):

```
ldr   r0, [pc, #808]      ; pi
add   r1, sp, #56         ; &rot
str   r0, [sp, #60]       ; rot.y = pi
mov.w sl, #0              ; zero
mov   r0, r5              ; g
str.w sl, [sp, #56]       ; rot.x = 0
str.w sl, [sp, #64]       ; rot.z = 0
blx   setRotation
```

Ours (landed `-g` trick — two-instruction address, `strd` merge):

```
ldr   r0, [pc, #808]      ; pi
mov.w sl, #0              ; zero
str.w sl, [sp, #64]       ; rot.z = 0
strd  sl, r0, [sp, #56]   ; rot.x = 0, rot.y = pi (merged)
add   r0, sp, #56
subs  r1, r0, r5          ; rp = &rot - g
mov   r0, r5
blx   setRotation
```

Both regions are 26 bytes. The original's three stores are not merged
because the y store is separated from the x store by `mov.w sl,#0` and
`mov r0,r5`. The landed trick's `subs` both forces the zero into `sl`
(Basin B) and creates the adjacency that lets the ARM load/store optimizer
form `strd`.

The plain `g->setRotation(rot)` shape produces the original's
one-instruction `add r1, sp, #56` and the three separate stores, but the
allocator moves the zero to `r4`, swaps Canvas/table-base (`r9`/`r8`) and
spills the parts-loop counter `k` (`str r6,[sp,#44]`). Score 84.88-84.95 %
depending on the Vector construction. The `-g` trick is what buys the
Basin-B allocation, at the cost of the extra `subs`.

### 1.2 Ship loop (instructions 548 onward)

Original:

```
ldr   r0, [pc, #632]      ; GOT
ldr   r1, [pc, #636]
add   r0, pc
add   r1, pc
ldr.w r8, [r0]            ; r8 = &Globals::rnd
ldr   r0, [sp, #52]       ; sceneRace
ldr.w r6, [r1, r0, lsl#2] ; capacity -> r6
ldr.w r0, [r8]            ; Globals::rnd
adds  r1, r6, #1
blx   nextInt
```

Ours:

```
ldr   r0, [pc, #632]
ldr   r1, [pc, #636]
add   r0, pc
add   r1, pc
ldr   r2, [r0, #0]        ; r2 = &Globals::rnd
ldr   r0, [sp, #52]
str   r2, [sp, #44]       ; spill &Globals::rnd
ldr.w fp, [r1, r0, lsl#2] ; capacity -> fp
ldr   r0, [r2, #0]        ; Globals::rnd
add.w r1, fp, #1
blx   nextInt
```

Register map in the ship loop:

| value | original | ours |
|---|---|---|
| `spotUsed` | r4 | r4 |
| `race` | r5 | r6 |
| `capacity` | r6 | fp |
| `&Globals::rnd` | **r8** | spilled `[sp,#44]` |
| `i` | r9 | r9 |
| `ship` / `status` | sl | sl |
| `type` | fp | r8 |
| `Globals::rnd` value | reloaded `[r8]` at every site | cached in r5 |

The original uses `Globals::rnd->` at **all eight** sites (the GOT-derived
address is loop-invariant and hoisted into r8; the pointer value is
reloaded at each use). Our source uses a local
`AERandom *rnd = Globals::rnd;` for four sites and `Globals::rnd->` for
the other four, so the compiler caches the pointer value in r5 and spills
the address. That is one extra live value, and the allocator resolves it by
spilling the address and giving r8 to `type`.

This is the central basin conflict:

* **Local-value basin (landed, 92.7573 %):** `Globals::status` in `sl`,
  Canvas in `r8`, table base in `r9`, parts-loop zero in `sl`; ship loop
  caches `rnd` in r5 and spills the address.
* **All-global basin (75.4286 %, 2232 B):** removing the local `rnd`
  entirely and using `Globals::rnd->` at every site gives the original's
  `ldr rX,[addr]` ship-loop shape, but the whole function collapses to a
  different allocation: `Globals::status` in `r4`, `this` spilled,
  Canvas/table base swapped in the parts loop, and the ship loop's
  `&Globals::rnd` lands in r9 (not r8) with `capacity` in r8.

No source shape found this session bridges the two: the local value must
exist to pin the prologue/parts-loop basin, but its existence is exactly
what pushes the address out of r8.

## 2. Measurements (all 2252 B unless noted)

Scored with the official single-function verifier (unrounded difflib
ratio). `pct` is the `--show` structural ratio.

### 2.1 Vector construction with the landed `-g` trick

| variant | pct |
|---|---|
| **y, z, x field assignment (landed)** | **92.7573** |
| x, z, y | 92.6302 |
| z, x, y | 92.6302 |
| y, x, z | 92.6302 |
| x, y, z | 88.3100 |
| z, y, x | 88.1830 |
| `Vector rot(0, pi, 0)` constructor | 88.3100 |
| constructor + redundant `rot.x = 0` | 92.7573 (redundant store eliminated) |
| constructor + redundant `rot.z = 0` | 88.3100 |
| constructor + redundant `rot.y = pi` | 87.9288 |
| `Vector(pi, pi, 0)` + `rot.x = 0` | 92.7573 |
| `Vector(0, pi, pi)` + `rot.z = 0` | 88.3100 |
| `float rot[3]` / anonymous struct / `float*` aliasing | 92.7573 (identical code) |
| `rot` declared before the `for` loop | 84.2839 (2264 B) |
| `rot` declared at function scope | 78.9677 (2180 B) |
| `rot.x = rot.y - pi`, `rot.z = rot.y - pi` | 92.7573 (folded) |
| `rot.x = rot.z = 0`, `rot.z = rot.x = 0` | 92.6302 |
| `rot.z = rot.x`, `rot.x = rot.z` | 92.7573 |
| `-0.0f` zeros | 88.0000 |
| address computed before the stores | 92.7573 (scheduler still sinks it) |
| address before stores, x/y/z order | 88.3100 |
| address before stores, x/z/y or z/x/y | 92.6302 |
| fold-back `rp + g` | 84.9524 (compiler folds to plain) |

### 2.2 Vector construction with plain `g->setRotation(rot)`

| variant | pct |
|---|---|
| y/z/x, x/z/y, z/x/y, y/x/z field assignment | 84.8793 (2248 B) |
| x/y/z, z/y/x | 84.9524 (2248 B) |
| constructor / aggregate / `= {}` then y | 84.9524 (2248 B) |
| `-0.0f` zeros | 88.0000 (2252 B) |
| `rot` before the `for` loop | 84.8985 (2260 B) |

Every plain form moves the zero to `r4`, swaps Canvas (`r9`) and table
base (`r8`), and spills the parts-loop counter `k`. The `-g` trick is
required for the Basin-B allocation.

### 2.3 Ship-loop `rnd` local-vs-global

The eight sites are:

| # | site |
|---|---|
| 0 | `shipCount = ...nextInt(capacity + 1)` |
| 1 | `if (...nextInt(100) <= 29)` |
| 2 | `race = ...nextInt(4)` |
| 3 | `if (...nextInt(100) < 30)` |
| 4 | `int pick = ...nextInt(3)` |
| 5 | `spot = ...nextInt(capacity)` |
| 6 | `spot = ...nextInt(capacity)` (retry) |
| 7 | `...nextInt(300)` |

The landed source has sites 1, 2, 6 local and 0, 3, 4, 5, 7 global
(the wave-119 mask-78 shape). Measured variants:

| variant | pct |
|---|---|
| **landed mask** | **92.7573** |
| all global (no local `rnd`) | 75.4286 (2232 B) |
| `AERandom *&rnd = Globals::rnd` (pointer reference) | 75.4286 (2232 B) |
| `AERandom *const &rnd` | 75.4286 (2232 B) |
| all-global + `Status *status` local in branch 23 | 72.4028 (2220 B) |
| all-global + function-scope `Status *status` | 69.5652 (2208 B) |
| all-global + local `rnd4` in branch 4 | 75.4286 |
| all-global, trick `-g` / `-(g+1)` / `~g` / `g<<31` / `-g*2` / `-(g|1)` | 75.43 / 75.05 / 75.05 / 75.10 / 75.10 / 75.22 |
| all-global, branch order 23,4,2 / 23,2,4 / 4,23,2 / 4,2,23 / 2,23,4 / 2,4,23 | 75.43 / 75.43 / 66.37 / 66.37 / 74.41 / 74.41 |
| `volatile AERandom *rnd` | 77.5949 (2240 B) |
| `static AERandom *rnd = Globals::rnd` | 72.1908 (2272 B, guard overhead) |
| `rnd` declared before `capacity` | 70.7595 (2236 B) |
| `rnd` declared after `shipCount` | 70.7595 (2236 B) |
| `rnd` declared after the ownHangar block | 72.8771 (2224 B) |
| `rnd` declared after `new unsigned char[capacity]` | 72.8771 (2224 B) |
| `rnd` declared after the fill loop (landed) | 92.7573 |
| `rnd` declared after `spots` | 92.7573 |
| `rnd` declared immediately before the `for` | 91.4867 |
| decl-before-capacity, shipCount local only | 75.4286 |
| decl-before-capacity, spot local only | 75.4286 |
| decl-before-capacity, retry local only | 64.4388 (2244 B) |

### 2.4 Variable types

| variant | pct |
|---|---|
| `short capacity` | 86.7089 (2268 B) |
| `unsigned short capacity` | 92.6302 |
| `long capacity` | 92.7573 |
| `size_t capacity` | 92.6302 |
| `intptr_t capacity` | 92.7573 |
| `unsigned int capacity` | 92.6302 |
| `const int &capacity` | 92.7573 |
| `unsigned` / `short` / `unsigned short race` | 92.7573 |
| `unsigned type` | 92.7573 |
| `short` / `unsigned short type` | 84.5178 (2264 B) |
| `volatile int type` | 81.0640 (2256 B) |
| `volatile int race` | 80.5556 (2264 B) |
| `volatile int capacity` | 75.8402 (2272 B) |
| `int cap = capacity` copy in the loop | 92.7573 (folded) |

### 2.5 Control flow / statement order

| variant | pct |
|---|---|
| branch order 23,4,2 (source) | 92.7573 |
| branch order 23,2,4 | 92.7573 |
| branch order 2,4,23 | 90.3431 |
| branch order 2,23,4 | 70.6776 (2248 B) |
| branch order 4,23,2 / 4,2,23 | 61.8413 (2256 B) |
| `ownHangar` block before the pick block | 91.2325 |
| `createShip` before pick/ownHangar | 75.2538 (2228 B) |
| `ownHangar` block after `capacity` | 82.1226 (2276 B) |
| `spots` before `spotUsed` | 88.1830 |
| `spots` before the fill loop | 88.1830 |
| `spots` before `capacity` | 90.7243 |

## 3. MIR evidence

`-mllvm -print-machineinstrs` on the landed source (dump in
`/dev/shm/w6735/mir.txt`, function section 1108473-1209800):

* Instruction selection already orders the parts-loop stores **z, y, x**
  (source order is y, z, x): `%544 = t2STRi12 ... stack.1, 8` (z),
  `%543 = t2STRi12 ... stack.1, 4` (y), `%544 = t2STRi12 ... stack.1, 0`
  (x). The y/x merge into `strd` happens later in the ARM load/store
  optimizer, after the zero (`%370`) is materialised early.
* The pre-scheduler address computation is `%545 = t2ADDri %stack.1, 0`
  followed by `%546 = t2SUBrr %545, %10` (the `-g` trick). The post-RA
  sink moves the `add` down to just before the `subs`.
* The greedy allocator assigns `capacity -> r11`, `rnd -> r5`,
  `race -> r6`, `type -> r8` in the landed source. The original's
  `capacity -> r6`, `&rnd -> r8`, `race -> r5`, `type -> r11` is a
  cyclic permutation of the same four registers.
* `-mllvm -sink-insts-to-avoid-spills` changes the plain
  `setRotation(rot)` parts loop to use `sl` for the zero (instead of r4)
  and removes the `k` spill, but it still emits the `strd` merge and
  still scores 84.88 %; it does not reproduce the original's three
  separate stores.

## 4. Conclusion

The landed wave-119 shape is a genuine local maximum for this compiler at
`-Oz`: the `-g` trick is the only lever found that forces the original's
Basin-B prologue/parts-loop allocation, and the local `rnd` value is the
only lever found that keeps it. The original's ship loop needs
`&Globals::rnd` in r8, which the compiler only produces in the all-global
basin (75.43 %). A source shape that simultaneously pins `Globals::status`
in `sl` and `&Globals::rnd` in r8 was not found in ~90 variants.

The next pass should look for the source feature that pins the prologue
basin independently of the `rnd` local (e.g. a witnessed original local
that keeps `this`/`status` live in the right registers), rather than
continuing to sweep `rnd` masks and Vector spellings.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6735
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 tools/verify/verify.py --original \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so --show _ZN5Level11createSceneEv
```

Scratch harness (not committed): `/dev/shm/w6735/` — `probe.py`
(build + official score + resolved disassembly + `nextInt` fingerprint),
`Level.base.cpp` (the landed source), `exp*.py` (this session's variants),
`mir.txt` (full `-print-machineinstrs` dump).
