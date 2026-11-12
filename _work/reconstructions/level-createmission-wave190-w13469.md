# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 190 / w13469

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave190-w13469`, base master `8e3604cca`. Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base `0x10000`).

Continues `level-createmission-wave190-w13459.md`. One size-exact raw win landed.

## Result — landed

```text
before (8e3604cca)                   17172/17172     2982   1123     6210/6197
after                                17172/17172     2966   1125     6210/6197
```

| metric | before | after |
|---|---:|---:|
| size | 17,172 | 17,172 (exact) |
| raw | 2,982 | **2,966** (-16) |
| aligned | 1,123 | 1,125 (+2) |
| insn | 6210/6197 | 6210/6197 |
| `.text` diff | 72,049 | **72,033** (-16) |
| `.ARM.extab` diff | 578 | 578 (unchanged, size/base exact) |
| build-id diff | 20 | 20 |
| total (text+extab+build-id) | 72,647 | **72,631** (-16) |

Gate: `check` **RATCHET PASS** — byte_exact 4507/4524, linked_exact 4507/4524,
size-exact 4524/4524, stub_zero_size 0, missing 0, wrong_type 0, extra 0,
data_exact 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
`.rel.plt` byte-identical, `.ARM.exidx` 0, `.rodata` content exact over the
original 269,159 B, 27/27 headers, file size 2,187,328 exact.

## The change

`case 2`'s `coords[6]` initializer is split so the zero components are stored
**before** the two `Globals::rnd->nextInt(20000)` calls:

```cpp
int coords[6];
coords[1] = 0;
coords[2] = 0;
coords[3] = 0;
coords[5] = 0;
coords[0] = (int) (float) (-20000 - Globals::rnd->nextInt(20000));
coords[4] = (int) (float) (-20000 - Globals::rnd->nextInt(20000));
```

Semantics are identical (same two `nextInt(20000)` calls, same order, same
array contents). The original emits the zero stores before the first call
(`0xb16d4..0xb16e2`: `movs r4,#0; movw r1,#20000; strd r4,r4,[sp,#272];
ldr.w r0,[fp]; str r4,[sp,#280]; blx nextInt`), while the aggregate
initializer let the scheduler sink them after the call. The explicit stores
reproduce the original schedule and shrink the case-2 prologue diff.

## Angles tried and rejected (all size 17,172 unless noted)

* `case 1` first loop `enemyRace` reload: the original reloads
  `ldr r5,[sp,#116]` at the loop head (`0xb1608`); ours hoists it into the
  preheader (`0xb158c`), which is the root of the `-2` shift that misaligns
  `0xb160a..0xb19d8`. Tried `int race = enemyRace` (in/out of loop),
  `const int race`, `int *racep = &enemyRace`, `volatile int race` (size-fatal
  17,228), `int n = attackerCount` loop bound, `int shipType` hoist,
  `Array<KIPlayer*> *enemies` local (raw 6,087), `Globals *globals` local
  (17,188) — the greedy RA keeps the value in `r5` for all non-volatile forms.
* `nameLeader` tail: the original returns on **both** paths
  (`0xb408e beq b02ca` then `0xb40b2 b.w b02ca`); our `else { return; }`
  falls through to `case4Else`. `if (...) {...} return;` is the faithful
  shape but is +4 B (17,176, raw 3,998) and shifts the whole tail.
* `case 183` switch arm order (`s1`/`s3` 2,983; `s2`/`s4` 2,982): the
  compiler canonicalises the body order, so the literal-pool block
  `0xb15c8..0xb15d8` (`-7000,10000,-30000,25000,-36000`) never flips.
* Long-sum term reorder (`miningPlantIndex` 2,077, `ArraySetLength` 1,861,
  blueprint `slot` 2,083): neutral except `guns` moved to the end (17,316)
  and a full reverse (2,986). The `guns` spill slot differs (`[sp,#92]` orig
  vs `[sp,#104]` ours) but is RA-chosen; declaration-order moves are neutral.
* Adjacent-line swap sweeps over source lines 2010–2123 (stationPirates /
  extraAliens / convoy) and 2148–2275 (case 183): no win (best 2,982).
* `case 2` `coords[6]` as `{0,...}` then two assignments (raw 9,380) or
  fully statement-by-statement in source order (2,982).

## Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13469
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
```

Expected: `raw=2966 aligned=1125 size=17172/17172`.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13469/`
(`batch6.py`, `Level.base.cpp`, `orig.txt`, `ours.txt`, `obj.dis`).

## Additional angles tried after the landed win (raw 2966 base)

All measured on top of the landed `case-2` coords change (raw **2,966**,
size 17,172). None beat 2,966; recorded so the next pass does not repeat them.

### `case 2` goods / Route / `new Array` / `total` orders

| variant | shape | raw | size |
|---|---|---:|---:|
| `g_a` | Route, escorts, goods, convoyEnd, new Array, total | 4,836 | 17172 |
| `g_b` | goods, Route, escorts, convoyEnd, total, new Array | 4,887 | 17172 |
| `g_c` | goods, Route, escorts, new Array, total=escorts+2+goods | build fail | — |
| `g_d` | goods, escorts, Route, convoyEnd, new Array, total | 10,432 | 17180 |
| `h1` | Route, goods, escorts, convoyEnd, new Array, total | 2,983 | 17172 |
| `h2` | Route, goods, escorts, convoyEnd, total, new Array | 4,904 | 17172 |
| `h3` | Route, goods, escorts, total, new Array, convoyEnd | 4,904 | 17172 |

`h1` is the faithful "goods after Route" order and improves *aligned*
(1,125 -> 1,118) but loses raw; the `int total` placement relative to
`new Array` is a large RA lever (+1,900 raw when moved before the allocation).

### `case 2` coords interleavings (`i1`-`i6`)

Tried every plausible interleaving of the two `nextInt` stores and the four
zero stores (including the original's own schedule: `coords[1]` after the
first call, `coords[2,3,5]` after the second): 2,985 / 2,992 / 2,985 / 2,992
/ 2,988; the `{0,0,0,0,0,0}` aggregate form is 9,380. The landed
"all zeros first" form remains best.

### `case 2` `asteroidPos` / `offset` declarations

Full placement sweep of `Vector asteroidPos = {0,0,0}` among the seven
statements (`p0`-`p7`): 3,015 / 2,989 / 2,992 / 4,820 / 4,814 / 4,886 /
4,886 / **2,966** (`p7` = current, after `ArraySetLength`, before `offset`).
`offset` before `asteroidPos`, split declarations, and `Vector asteroidPos;`
are all neutral (2,966).

`volatile` on `offset` is load-bearing for **size**: `int offset = 0` is
17,164 (-8 B) and raw 10,711 (aligned 936). `volatile int offset; offset=0;`
is identical to the base. Capturing the volatile read once per iteration
(`int off = offset;` before `setPosition`) reproduces the original's
`ldr.w r9,[sp,#120]` shape: size-exact 17,172, aligned **950**, but raw
4,900 (the loop is 8 B short of the original and shifts the tail).

### Root cause of the `case 2` first-loop 8 B gap

The original keeps `convoyEnd` in `fp` across the loop and spills `ship` to
`[sp,#108]` (`str r1,[sp,#108]` at `0xb17d6`, plus `mov r4,fp` /
`mov fp,r4` around the `x` component). Our build keeps `ship` in `r4` and
spills `convoyEnd`, saving exactly 8 B. Forcing the spill with
`ship->getRoute()->reachWaypoint(0)` costs +8 B (17,180, raw 10,372);
combining it with the non-volatile `offset` (-8 B) is nonlinear
(17,156 / 17,140 / 17,196). The `ship`-spill + `convoyEnd`-in-`fp` shape is
the remaining faithful form if a future pass can pin the 8 B.

### `nameLeader` tail

`if (!nameLeader) return;` compiles identically to the current
`if (...) {...} else { return; }` (2,966). `if (...) { ...; return; }` is
2,982 at 17,172 (raw 3,109); the faithful unconditional `return;` after the
`if` is 17,176 (raw 3,982) and needs a -4 B compensation.

### Tail and misc

- `case 4` convoy condition forms: swapping `hasWanted && i == 0` is
  17,164 (-8 B, raw 12,669); `else if` operand swap 15,466; `*bigShipsCount`
  operand swap 17,184. `localRace == 1 && *bigShipsCount >= 1` reorder is
  neutral.
- `case 4` tail `Globals::options[44]` load: moving the read after
  `getDifficulty`/`new Array` is 2,967-2,968 (aligned 1,116-1,125).
- `case 4` `case4Else` coords: statement-by-statement, `Vector pos` copy,
  and y/z swap are all neutral 2,966.
- `case 183` switch source order `0,1,2,default` is 2,967; explicit
  `case 3` is 5,342.
- `case 2` loop-bound spellings (`convoyEnd > i`, `i != convoyEnd`,
  `escorts + 2`, `unsigned`) are 2,966-2,967.
- `localRace` as `const` / `unsigned` / split declaration: neutral.
- `int total = goods + convoyEnd` / `const int total`: neutral.
- Second convoy loop: dropping the `enemies` local is 10,732 (17,188);
  using `convoyShip` for all accesses is 6,186 (size-exact) / 10,660.
- Adjacent-line swap sweeps over source lines 1838-1900, 1900-1970 and
  1970-2030: no size-exact win (best 2,966).

### Reproduce the residual

```sh
cd /home/debian/GalaxyOnFire2/wt/w13469
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
```

Expected: `raw=2966 aligned=1125 size=17172/17172`.
