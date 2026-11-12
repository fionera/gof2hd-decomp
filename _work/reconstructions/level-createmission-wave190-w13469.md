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
