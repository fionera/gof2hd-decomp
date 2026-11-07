# `.ARM.extab` LSDA scopes: `Radar::Radar` placement tie-break analysis (w7742)

Status: research note, 2026-11-07. Worktree
`/home/debian/GalaxyOnFire2/wt/w7742`. Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000). No source change landed this pass; the gate is the
w7720c baseline.

## Result

| metric | value |
|---|---:|
| `.ARM.extab` differing bytes | **1416** |
| `.ARM.extab` differing records | **12** |
| `.text` differing bytes | 85,244 |
| `.ARM.exidx` differing bytes | 0 |
| `.rel.plt` differing bytes | 0 |
| byte_exact / linked_exact | 4506 / 4506 |
| size-exact / data / imports | 4524, 843, 207 |
| lint / drift | CLEAN / 0 |

`check` is green (`RATCHET PASS`). Section sizes are unchanged.

## Correction to the w7720c frequency claim

The w7720c note said the two `Radar::Radar` pads had placement frequencies
21 (enemyGuns) and 107 (outer), and concluded the original must have reversed
them.  That is **not** the placement-time frequency.  `-mllvm -print-bfi`
prints the *IR-level* BFI; the value used by `MachineBlockPlacement` is printed
by `-mllvm -print-machine-bfi`.  The machine BFI immediately before
`Branch Probability Basic Block Placement` gives:

```
BB60 (ir-block.195, outer scope)      float = 0.000011563, int = 519
BB87 (ir-block.423, enemyGuns scope)  float = 0.000011568, int = 519
```

The two pads have **equal integer frequencies**.  The underlying float sums are
outer `12.12489` and enemyGuns `12.13031` (sum of predecessor block
frequencies; each pad edge has the invoke unwind probability 1/1048576), i.e.
the enemyGuns pad is in fact *slightly hotter*, but both truncate to 519 under
the BFI integer scale.

## Why the tie puts the enemyGuns pad first (ours)

`MachineBlockPlacement::selectBestCandidateBlock` (LLVM 7.0.2,
`lib/CodeGen/MachineBlockPlacement.cpp:1623`) keeps the **last** block of the
minimum frequency for EH pads:

```cpp
if (BestBlock && (IsEHPad ^ (BestFreq >= CandidateFreq)))
  continue;
```

With equal frequencies `BestFreq >= CandidateFreq` is true, so the candidate
replaces `BestBlock`; the last minimum in `EHPadWorkList` wins.

The NDK clang is a release build (`NDEBUG`), so the
`assert(Chain.UnscheduledPredecessors == 0)` in `fillWorkLists` is compiled
out.  `buildCFGChains` therefore seeds `EHPadWorkList` with **every** chain head
in `MachineFunction` order:

```
outer (bb.60) ... playerGuns loop (bb.78) ... enemyGuns (bb.87) ... enemyGuns loop (bb.88)
```

so the enemyGuns pad is later in the worklist and is placed first (offset
2462); the outer pad follows at 2464.  The original has the outer pad at 2462,
i.e. its enemyGuns pad must be *later* in the worklist (or its integer
frequency must be strictly greater).

The remaining pads are ordered by frequency and match in both builds:
enemyGuns-loop (7,837,338) then playerGuns-loop (23,645,858).

## Why a source shape is hard to find

The BFI integer scale is `8 / Min` (`convertFloatingToInteger`,
`lib/Analysis/BlockFrequencyInfoImpl.cpp:462`), where `Min` is the smallest
block frequency in the function.  Here `Min = BB195 = 1.7815e-7`, giving the
observed scale `44,907,140`.  The two pads sit at 519.3 and 519.5; a **~0.1 %**
change in the scale (i.e. a ~0.1 % change in the smallest pad's frequency) or
a ~0.0005 increase in the enemyGuns float sum would make them 519 vs 520 and
flip the order.  The smallest pad is a temporary-String scope in the
`lockFrames`/`blipImage`/`qualityImage` region, so the tie is sensitive to
almost any IR-level frequency perturbation — yet the final machine code must
stay byte-identical.

## Variants tested this pass (all compiled with the exact TU flags)

Body comparison is against the baseline object's `.text._ZN5RadarC2EP5Level`
(all relocations identical), so `diffbytes 0` means the linked body is still
byte-exact.  Row comparison is against the original `.so`'s record.

| variant | body diff | row diffs |
|---|---:|---:|
| baseline | 0 | 7 (early rows 2464, rows 19/21 2462) |
| `if (enemyGuns)` / `!= 0` / `!= NULL` / `!(== nullptr)` | 0 | 7 |
| `if ((int)enemyGuns != 0)`, `(unsigned)enemyGuns != 0` | 0 | 7 |
| `if (__builtin_expect(...,1/0))` | 43 / 2496 B | 24 / 38 |
| `auto *`, `*const`, direct-init `(expr)`, parenthesised init | 0 | 7 |
| `for` -> `while` (either/both loops), `i += 1`, `i++`, `i != n` | 0 (1 B for `!=`) | 7 |
| `unsigned int i`, `int i`, `i` declared before loop, `for(;;){...break;}` | 0 | 7 |
| `do { } while (0)` around the enemyGuns block | 0 | 7 |
| braces around labelStrings / playerGuns / enemyGuns / image group / whole ctor | 0 | 7 |
| braces around enemyGuns block *ending before* the image group | **14** | 11 (early rows correct, row 19 -> 2368) |
| `if (Array<...> *enemyGuns = getEnemyGuns())` | **14** | 11 (same as above) |
| compound block around decl+if+image group (closes after group) | 0 | 7 |
| declaration outside, assignment inside a block | 0 | 7 |
| `enemyGuns` declared at ctor top (init or assign later) | -1 / 24 | 38-39 |
| `isRocketGun() != 0` / `== 1` / `!!` (all loops) | 0 | 7 |
| `getIndex() == 101` -> `101 == getIndex()` | 0 | 7 |
| `&&` chain -> `!(!a || !b || c)` / `== true` / `== false` | 0 | 7 |
| `const`/`u` suffix on lockFrames/blipImage/qualityImage | 0 | 7 |
| braces around lockFrames / qualityImage block | 0 | 7 |

The only variants that flip the early rows are the two that **end the
enemyGuns lifetime scope before the image group** (14-byte cleanup-chain
delta, row 19 unwinds to the String-temporary pad 2368 instead of the
enemyGuns pad).  Every body-exact variant leaves the rows at 7 diffs.

### Root cause of the 14-byte variant

The 14 differing bytes are all in the cleanup trampolines at function offsets
2358..2390 (`b.n` targets), not in the body proper:

```
2358 0x0b->0x0c  2362 0x09->0x0a  2368 0x1f->0x30  2370-2374 ...
2384-2390 ...
```

Ending the enemyGuns scope before the image group moves `getEnemyGuns`'s
unwind edge to the String-temporary cleanup pad, so the cleanup chain order
changes.  The original keeps `getEnemyGuns` and the image group in the same
scope (both 2464), so that shape cannot be the original source.

## Remaining 12 records (owners from the original `.dynsym`/exidx)

| owner (fn address) | diff bytes | body byte-exact? |
|---|---:|---|
| `Generator::createMission` (0x971b8) | 24 | no |
| `Level::createMission` (0xaffd8) | 138 | no |
| `Level::createScene` (0xb42ec) | 9 | no |
| `Level::createShip` (0xc0214) | 70 | no |
| `ModStation::ModStation` (0xd52d0) | 2 | no |
| `Radar::Radar` (0x12d484) | 7 | **yes** |
| `Radar::draw` (0x12e050) | 29 | no |
| `LevelScript::process` (0x137d18) | 291 | no |
| `HangarWindow::render` (0x1490d8) | 762 | no |
| `Hud::draw` (0x163b90) | 40 | no |
| `SpaceLounge::drawLounge` (0x170d38) | 14 | no |
| `MGame::OnTouchEnd` (0x17a144) | 30 | no |

`Radar::Radar` is still the only record owned by a byte-exact body.  The
`ModStation::ModStation` record is not a pad swap either: the row boundary
differs (`(390,30)` vs `(398,22)`), so it will close with its body.
`Level::createScene` is the systematic +2 shift documented in w7720c.

## Next hypotheses (not yet tried)

1. Find a source shape that changes the **smallest pad's frequency** (the
   `lockFrames`/`blipImage`/`qualityImage` scopes) by ~0.1 % without changing
   the body, e.g. a declaration/scope form that moves one invoke between the
   `lockFrames` and `blipImage` scopes but still emits the same cleanup code.
2. Find a shape that raises the enemyGuns float sum by >= 0.0005 (one extra
   image-group invoke would be far too much; a different branch-weight form
   that survives canonicalisation is needed).
3. Check whether the original was built with a slightly different
   `MachineBlockFrequencyInfo` scale because of a different *smallest* pad in
   an unrelated part of the ctor (the scale is global to the function).

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7742
cmake --build cmake-build-match --target check -j6   # RATCHET PASS, byte 4506
python3 _work/reconstructions/extab_lsda_decode.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
# extab differing bytes 1416, records 12
```

Harness used for the variant sweep (scratch, `/dev/shm/w7742`):
`cc_radar.sh` (exact TU compile flags), `radtest.py` (compiles a variant,
extracts `.text._ZN5RadarC2EP5Level` + `.ARM.extab.text._ZN5RadarC2EP5Level`
from the object, compares body to the baseline object and rows to the original
`.so`).  `-mllvm -print-machine-bfi -mllvm -print-machineinstrs` gives the
placement-time frequencies and the pre/post-placement block order.
