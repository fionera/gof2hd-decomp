# `Level::createScene` — wave-121 w7114: live-interval census, no source win

**Status:** dead-end sweep, 2026-11-06. Worktree `wt/w7114`. Target
`_ZN5Level11createSceneEv`, `src/game/world/Level.cpp`. No source change
landed. This note records the measurements, a **new** exact live-interval
dump taken from the NDK clang itself (`-mllvm -print-machineinstrs`), and
the register-map census, so the next pass does not repeat them.

Continuation of `level-createscene-wave119-w6972.md` (parts-loop store
order + `unsigned short cap` split), `…-w6939.md` (the most complete
prior note), `…-w6961.md`, `…-w6934.md`, `…-w6903.md`, `…-w6820.md`.

## TL;DR

* Baseline and best: **92.75730622617534 % / 2252 B**, 791/783
  instructions, first diff at normalized instruction 478 (parts-loop
  store schedule). Gate re-measured green: **byte 4505 / linked 4505**.
* The two structural basins are now characterised exactly:
  * **landed** (`AERandom *rnd` local + `- (size_t)g` trick + `y,z,x`):
    `capacity=fp`, `ownHangar=r4`, `spotUsed=r4` — the **ownHangar /
    spotUsed pair already matches the original**; only `capacity`
    (`fp` vs `r6`) and the `rnd` value (`[sp,#44]`-spilled vs `r8`)
    differ.
  * **pac** (all-global `Globals::rnd` + natural `x,y,z` + `unsigned
    short cap`): `capacity=r4`, `ownHangar=r5`, `spotUsed=r6` — the
    **parts loop and the ship-loop head are byte-exact**, but
    `capacity`/`spotUsed` are transposed.
* **New this session:** the NDK clang's own pre-RA slot indices were
  extracted with `-mllvm -print-machineinstrs` (the `After Greedy
  Register Allocator` MIR still prints vregs, the `After Virtual
  Register Rewriter` MIR prints physical registers). For `pac`:
  * `capacity` `%27` = `[6816,7840]`, size 1024 → **r4**
  * `spotUsed` `%32` = `[7520,12064]`, size 4544 → **r6**
  * `cap` `%35` = `[7840,10704]`, size 2864 → **r4** (shares with capacity)
  * `ownHangar` `%649` = `[3952,6968]`, size 3016 → **r5**
  * `race` `%593` = `[8416,8864]`, size 448 → r5 (local, after globals)
  * `type` `%595` = `[8912,9984]`, size 1072 → r11 (local)
* ~110 new variants measured (160 + 96 + 28 + 24 + 10 + 6 + 7 + 7).
  **None produced `capacity=r6`.** The register map is extremely robust:
  the only two basins reachable from the pac shape are `r4/r5/r6`
  (with the `cap` split) and `r8/r4/r4` (without it).

## 1. The two basins, exactly

`verify.score` (unrounded difflib ratio), `fd` = first differing
normalized instruction.

| variant | pct | size | insns | fd | capacity / ownHangar / spotUsed |
|---|---|---|---|---|---|
| **landed** (rnd local, trick, y,z,x) | **92.75730622617534** | 2252 | 791/783 | 478 | `fp` / `r4` / `r4` |
| pac (all-global, natural x,y,z, ushort cap) | 89.9235 | 2256 | 785/783 | 9 | `r4` / `r5` / `r6` |
| plain (all-global, natural x,y,z, no cap) | 75.0318 | 2228 | 787/783 | 6 | `r8` / `r4` / `r4` |
| landed + natural parts (y,z,x) | 84.8793 | 2248 | 791/783 | 28 | `fp` / `r4` / `r4` |
| landed + natural parts (x,y,z) | 84.9524 | 2248 | 792/783 | 28 | `fp` / `r4` / `r4` |
| pac + trick parts (y,z,x) | 89.4133 | 2256 | 785/783 | 9 | `r4` / `r5` / `r6` |
| pac + trick parts (x,y,z) | 87.7707 | 2260 | 787/783 | 9 | `r4` / `r5` / `r6` |
| pac, no cap + trick | 75.3016 | 2232 | 792/783 | 6 | `r8` / `r4` / `r4` |

Key observation: the **landed** variant already has the original's
`ownHangar=r4` / `spotUsed=r4` pair. Its residual is `capacity=fp` and
the `rnd` value being spilled to `[sp,#44]` instead of living in `r8`.
The **pac** variant has the original's `rnd=r8` and byte-exact parts
loop, but `capacity`/`spotUsed` transposed.

Original target map (from the disassembly):

| value | original |
|---|---|
| `spotUsed` | r4 |
| `race` | r5 |
| `capacity` | r6 |
| `&Globals::rnd` | r8 |
| `i` | r9 |
| `status` / `ship` | r10 (sl) |
| `type` | r11 (fp) |
| `ownHangar` | r4 (briefly, then `[sp,#24]`) |

## 2. Parts loop: three forms

Original (`b4860`):

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

* **pac** (natural `g->setRotation(rot)`, source order `x,y,z`) is
  **byte-exact** here — 8 instructions, `add r1,sp,#56`, three separate
  stores in `y,x,z` order, zero in `sl`.
* **landed** (`- (size_t)g` trick, source order `y,z,x`) emits
  `mov.w sl,#0; str.w sl,[sp,#64]; strd sl,r0,[sp,#56]; add r0,sp,#56;
  subs r1,r0,r5; mov r0,r5` — same instruction count, but the `strd`
  merge and the `subs` address differ.
* **landed + natural `x,y,z`** (no trick) matches the original's store
  order and `add r1,sp,#56`, but adds a `str r6,[sp,#44]` spill and
  puts the zero in `r4` instead of `sl`; it also shifts the parts-loop
  base from `r9` to `r8` and moves `fd` to 28 (2-byte shift).

All six store-order permutations were re-measured with the trick:
`y,z,x` **92.7573** > `x,z,y` = `y,x,z` = `z,x,y` **92.6302** >
`x,y,z` **88.3100** > `z,y,x` **88.1830**. `y,z,x` is optimal.

## 3. The `cap` split is the only lever that changes the basin

`unsigned short cap = (unsigned short) capacity;` used at the two
`nextInt` sites is the only source change found that moves the map from
`r8/r4/r4` to `r4/r5/r6`. The `uxth` splits `capacity`'s live range
into a short pre-loop part (`%27`, [6816,7840]) and a loop-carried part
(`%35`, [7840,10704]); without it, `capacity` is one long range and the
map collapses to `r8/r4/r4`.

Placement of the `cap` declaration is irrelevant (the compiler hoists
it): `after capacity`, `after spots`, `before loop`, `before shipcount`
all give the identical 89.9235 % / `r4/r5/r6`.

Cap type sweep (`short`, `unsigned short`, `unsigned char`, `int`,
`unsigned`, `long`, `signed char`) × use site (`retry`, `both`,
`first`, `new[]`, `zero loop`, `min`) — every combination lands in one
of the two basins. `capacity` type (`int`, `unsigned`, `long`,
`size_t`) and `ownHangar` form (`bool`, `int`, `&&`, ternary) do not
change the map either.

## 4. Why `capacity=r6` is hard

Greedy RA (LLVM 7 `RegAllocGreedy::enqueue`) orders global intervals by
`(1u<<31) | (1u<<29) | Size` (largest first) and local (single-MBB)
intervals by `(1u<<31) | (AllocationPriority<<24) | distance-to-end`;
all globals are dequeued before all locals. `tryAssign` then takes the
first free register in `GPR` allocation order `r4, r5, r6, r8, r9, r10,
r11` (r7 is the frame pointer), evicting only when no register is free.

For `pac` the global sizes are `spotUsed` 4544 > `ownHangar` 3016 >
`cap` 2864 > `capacity` 1024, so the size order predicts
`spotUsed=r4, ownHangar=r5, cap=r6, capacity=r6`. The observed map is
`capacity=r4, ownHangar=r5, spotUsed=r6, cap=r4`: `capacity`/`cap` are
coloured first (they are adjacent, non-overlapping, and share r4) and
`spotUsed` is pushed to r6. The eviction cascade, not the size order,
decides the result, and no source perturbation tried moves
`capacity` off r4.

To reach the original map, `spotUsed` must be coloured r4 **before**
`capacity`/`cap`, and `capacity` must then skip r5 (taken by `race`)
to r6. The original reaches this because `ownHangar` occupies r4 while
`capacity` is coloured, then is spilled and `spotUsed` reuses r4. In
`pac`, `capacity` takes r4 first and `ownHangar` takes r5.

## 5. Variants tried this session (all ≤ 92.7573 %)

* 160 cap-type × use-site × placement combinations (`gA`), 96 more
  (`gB`), 28 hoisted (`gC`).
* 24 `ownHangar` form × `min` form × cap combinations (`gE`).
* 10 split variants (`gD`): `spotUsed` copy, two-cap chains, cap at
  `new[]`/zero-loop only, cap kept live past the loop.
* 7 loop-variable hoists (`gF`): `race`, `type`, `spot`, `ship` moved
  outside the loop.
* 6 extra splits (`gG`): `shipCount` as `unsigned short`, `spots`
  copy, `cap` at `min`/`new[]`/zero-loop.
* 7 `rnd`-local site subsets (`gH`): A/B/R combinations. Only `ABR`,
  `AR`, `BR`, `R` give `fp/r4/r4`; `A`/`AB` give `sl/r5/r4`; `B` gives
  `r8/r5/r4`.
* Table type changes (`unsigned short[]`, `unsigned char[]`) with
  `capacity` as `int`/`unsigned short` — all collapse to the no-cap
  basin (`r8/r4/r4`, 74.9 %).
* `rot` initialisation forms (`{0,pi,0}`, `rot.x=rot.z=0`,
  `rot.y=pi; rot.x=rot.z=0`) — `rot.y=pi; rot.x=rot.z=0` reproduces
  the landed 92.7573 %, all others ≤ 92.63 %.

Register-map census over ~110 new variants: `r4/r5/r6` (dominant with
cap), `r8/r4/r4` (dominant without cap), `r5/r6/r4`, `r5/r6/r6`,
`sl/r4/r4`, `fp/r4/r4`, `None/r6/r4`, `r8/r5/r4`. **No `r6/*/*`.**

## 6. Reproduction

```sh
# baseline
cmake --build cmake-build-match --target check -j6   # byte 4505 linked 4505

# exact live intervals from the NDK clang itself
clang++ <exact flags> -mllvm -print-machineinstrs -c pac.cpp -o pm.o > pm.txt
#   "After Greedy Register Allocator"   -> vregs + slot indices
#   "After Virtual Register Rewriter"   -> physical registers
```

## 7. Remaining root cause

One greedy-RA transposition: `capacity` takes r4 where the original has
r6, and `spotUsed` takes r6 where the original has r4 (`ownHangar`
r5 vs r4 follows). The `uxth` that creates the split is load-bearing and
costs one extra instruction (2256 vs 2252 B). No source shape found in
eight sessions bridges the transposition; the landed 92.7573 % remains
the best.
