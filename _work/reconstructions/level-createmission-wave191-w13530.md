# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 191 / w13530

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave191b-w13530`, base master `a303f439e`. Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base `0x10000`).

Continues `level-createmission-wave191-w13526.md` / `...w13512.md`.

```text
                                     size            raw    aligned  insn
base a303f439e                       17172/17172     2659    871     6206/6197
landed                               17172/17172     2657    871     6206/6197
```

## 1. Result — landed

| metric | before | after |
|---|---:|---:|
| size | 17,172 | 17,172 (exact) |
| raw | 2,659 | **2,657** (-2) |
| aligned | 871 | 871 |
| insn | 6206/6197 | 6206/6197 |
| `.text` diff | 71,251 | **71,249** (-2) |
| `.ARM.extab` diff | 1,216 | 1,216 |
| `.rodata` diff | 485 | 485 |
| `.rel.plt` diff | 0 | 0 |
| `.ARM.exidx` diff | 0 | 0 |

`check`: **RATCHET PASS** (byte_exact 4507/4524, linked_exact 4507/4524,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0).

## 2. The change: case-2 loot `crateCaptured`/`cargo` store order

The case-2 second (loot) loop had

```cpp
Array<KIPlayer *> *enemies = this->enemies;
(*enemies)[convoyEnd + j]->cargo = nullptr;
((PlayerFighter *) (*enemies)[convoyEnd + j])->crateCaptured() = 0;
(*enemies)[convoyEnd + j]->setRotationSpeed(0.0f);
(*enemies)[convoyEnd + j]->setSpeed(0.0f);
```

Swapping the two independent stores (crate byte store first, then the cargo
pointer store) is semantically identical and moves two more `.text` bytes into
agreement.  In the emitted block clang already schedules the byte store first
(`strb r9,[r0,#76]`); writing the source in that order lets the following
`ldr r0,[r4,#4]` / `add r0,r6` / `ldr r1,[r0,r8,lsl#2]` / `str.w r9,[r1,#80]`
pair land on the original's bytes.

## 3. Sweep results (all size 17,172 unless noted)

Full 2x2x2x2 sweep over {ship local} x {setSpeed-first} x {crate-first} x
{`enemies` local} in the loot block:

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base (`cargo` then `crate`, local enemies, rot-first) | 2,659 | 871 | 17,172 |
| **crate then cargo (landed)** | **2,657** | **871** | **17,172** |
| crate then cargo, setSpeed-first | 2,664 | 864 | 17,172 |
| ship local for cargo/crate (local enemies) | 10,323 | 870 | 17,164 |
| ship local + `this->enemies` for the rest | 9,964 | 865 | 17,180 |
| no local enemies (any order) | 9,991+ | 887+ | 17,180 |

The `ship` local and dropping the `enemies` local are size-fatal in every
combination; the store-order swap is the only size-exact improvement.

## 4. Residual / neutral cells (do not repeat)

* Case-2 prologue: the original order is `coords[0]`, `coords[1]=0`, `coords[2]`,
  `coords[3..5]=0`, then `Vector asteroidPos = {0,0,0}`, then
  `new Route(coords,6)`, then `getProductionGoodAmount()`, then `getDifficulty()`.
  Reordering the source to that order (`asteroidPos` before the Route, `goods`
  after it, interleaved coords zero-stores) reproduces the prologue almost
  exactly and keeps the case-2 block size identical to the original, but the
  whole block still starts 2 B early because of the case-1 `enemyRace` shift
  below, so raw rises to 3,254.  Not landable until case 1 is fixed.
* Case-1 `enemyRace` loop-head reload: the original reloads
  `ldr r5,[sp,#116]` inside the attacker loop (`0xb1608`); ours hoists it into
  the preheader (`0xb158c`) and drops the 2-byte `nop` before the literal pool,
  shifting the loop latch and the whole case-1 tail by -2 B.  `int race =
  enemyRace;` in the body is LICM-hoisted (neutral); `*(volatile int *)&enemyRace`
  forces the reload but makes the address escape, growing the frame to 17,232 B.
  A dead `if (enemyRace == 99) return;` after the loop also re-allocates the
  frame (+28 B).  This -2 B is the main remaining case-1 residual.
* Tail (`0x3200..0x42ff`, ~2.2 KB of the residual) is dominated by `this`
  living in `r4` (original) vs `r8` (ours) from `0xb322e` (the `alienGuns`
  block) onward; ours also keeps an extra live value from `[sp,#100]`.  The
  case-10 `objectivesA = new Objective(7,...); return;` tail is outlined to the
  shared cold block at `0xb41e8` in ours but duplicated inline at `0xb20f6` in
  the original, precisely because the original's shared block uses `r8` for
  `this` while case 10 uses `r4`.

## 5. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13530
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)
```

Expected: `raw=2657 aligned=871 size=17172/17172`, `.text` diff 71,249,
`.ARM.extab` diff 1,216, `.rel.plt` diff 0, `RATCHET PASS`.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13530/`
(`diffmap.py`, `sbs.py`, `cmp2.py`, `run.py`, `sweep1.py`, `sweep2.py`).
