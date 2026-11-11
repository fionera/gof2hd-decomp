# MGame::OnTouchEnd — wave 174 (w11919): epilogue placement + a size-exact raw win

Status: one small size-exact code win landed, 2026-11-11. Worktree
`/home/debian/GalaxyOnFire2/wt/w11919`, branch `wave174-w11919`, base `be9dd429b`.
Target `_ZN5MGame10OnTouchEndEiiPv`, original 8,080 B @ `0x17a144`.
Scratch `/home/debian/GalaxyOnFire2/scratch/w11919`.

## Result

| | raw | aligned | size | epilogue `popeq` | `.rel.plt` |
|---|---:|---:|---:|---:|---:|
| base | 3,773 | 1,453 | 8,080/8,080 | `0x17ba32` | 0 |
| **landed** | **3,771** | **1,453** | **8,080/8,080** | `0x17ba32` | **0** |

The landed change is a single statement move inside the `hudTouchFlags == 0`
body: `this->pauseOpen = 0;` is emitted **after** `resumeSounds()` instead of
before it.

```cpp
} else if (this->hudTouchFlags == 0) {
    this->hud->closeHudMenu();
    this->hudMenuOpen = 0;
    this->touch0Id = nullptr;
    this->touch1Id = nullptr;
    resumeSounds();          // <- moved above pauseOpen
    this->pauseOpen = 0;
    ...
```

`resumeSounds()` does not read `pauseOpen` (`MGame::resumeSounds` only calls
`Sound::resumeAll`, `PlayerEgo::ResumeEngineSound` and the enemy loop), so the
reorder is semantically neutral. The compiler keeps the same instruction count
and the same block layout; only the store is scheduled after the call. The
2-byte gain is three bytes that now coincide with the original at
`0x17aed5/0x17aed8/0x17aeda` against one that stops coinciding at `0x17aedb`
(the original's bytes there belong to the `menuTouchOpen` block, which sits at
`0x17aed4` in the original but at `0x17aebc` in ours). It is a real same-address
byte win, not a size or alignment change: aligned stays 1,453.

Gate after the change (full `check`, `RATCHET PASS`): byte_exact **4507**,
linked_exact **4507**, size-exact 4524/4524, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact 843/843, imports 207, lint CLEAN, drift 0,
symbol parity 5370/5370, `.rel.plt` byte-identical (0).

## Why the epilogue cannot be moved cheaply (refined MBP analysis)

The epilogue address is a downstream symptom of two MachineBlockPlacement
(MBP) chain decisions in the tail. Both were traced against the pre-MBP MIR
(`-mllvm -print-machineinstrs -mllvm -filter-print-funcs=_ZN5MGame10OnTouchEndEiiPv`).

### 1. The shared `Hud::initHudMenu` tail

`BranchFolder` merges the trailing `blx Hud::initHudMenu` of the `0x200` and
`0x400` dispatcher arms into one block (bb.391). Its predecessors are bb.390
(the `0x200` arm, block freq 44.95) and bb.421 (the `0x400` arm, freq 11.24).
`hasBetterLayoutPredecessor` rejects an edge when

```
freq(other pred) * P(other->succ) * 0.8 >= freq(BB) * P(BB->succ) * 0.2
```

i.e. when the other predecessor's edge is at least 4x hotter. With the 50/50
bit-test dispatch, `freq(390) == 4 * freq(421)` **exactly**, so the check is on
the knife edge: 390 processed first claims bb.391 (no conflict), while
421 -> 391 is rejected. Ours therefore puts the `blx` inside the `0x200` arm
(`17adc4 ldrd; movs r1,#1; 17adca blx`), making that arm 12 B instead of the
original's 8 B and shifting the whole tail by +4. The original has the `0x400`
arm falling through into the `blx` (`17afd8 ldrd; movs r1,#2; 17afde blx`).

### 2. The `hudTouchFlags == 0` test fallthrough

bb.392 is the `hudTouchFlags == 0` test. Its successors are bb.393 (the body,
37.5% — `calcZeroHeuristics` gives `X == 0` the 20:12 unlikely weight) and
bb.458 (the final `isAutoPilot` join, 62.5%). MBP prefers bb.458, but
`hasBetterLayoutPredecessor` rejects it: bb.455 (the second `cameraMode`
switch compare, freq 35.96, edge probability 0.5) has an edge frequency of
17.98, above the threshold `freq(392)*0.625/4 = 12.29`. MBP falls back to
bb.393, so the chain becomes `392 -> 393 -> 394 -> 458 -> 459` and the epilogue
lands at the end of the function (`0x17ba22`). The original's chain is
`421 -> 391 -> 392 -> 458 -> 459`, with the body demoted to a later chain after
the epilogue (`17b01a`).

`freq(455)` is `freq(388) * 0.375 * 0.667`; removing the conflict needs either
`hudMenuOpen` to be much more likely true (so the `cameraMode` else-arm is
cold) or the `cameraMode == 3` branch to be strongly biased. Both are
reachable with `__builtin_expect`, but every such shape changes the branch
metadata globally and re-lays the whole tail (see the table), losing far more
raw than the epilogue move gains.

## Shapes measured this wave

All shapes are applied to `src/game/menu/MGame.cpp` on top of the base source.
`size` is 8,080 unless noted; the epilogue column is the address of the last
`popeq`.

| shape | raw | aligned | size | epi | relplt |
|---|---:|---:|---:|---:|---:|
| base | 3,773 | 1,453 | 8,080 | `0x17ba32` | 0 |
| `switch` case 0 first | 3,764 | 1,463 | 8,080 | `0x17ba32` | **5** |
| case 0 first + `expect(0x200,1)` | 3,764 | 1,463 | 8,080 | `0x17ba32` | 5 |
| case 0 first + `count > 0` | 3,766 | 1,464 | 8,080 | `0x17ba32` | 5 |
| `if (cameraMode == 0)` else-if form | 3,764 | 1,463 | 8,080 | `0x17ba32` | 5 |
| `if (cameraMode == 3)` first | 3,773 | 1,453 | 8,080 | `0x17ba32` | 0 |
| invert `if (hudMenuOpen)` / `else` | 6,126 | 1,637 | 8,068 | `0x17af9c` | 6 |
| invert + case 0 first | 5,604 | 1,620 | 8,072 | `0x17af9c` | 0 |
| `hudTouchFlags == 0` as a separate `if` | 4,751 | 1,459 | 8,084 | `0x17ba00` | 0 |
| weaponSlot body closeHudMenu first (DACB) | 3,772 | 1,521 | 8,080 | `0x17ba32` | 0 |
| weaponSlot body DCBA | 3,772 | 1,521 | 8,080 | `0x17ba32` | 0 |
| htf body closeHudMenu<->pauseOpen (CD4) | 3,772 | 1,453 | 8,080 | `0x17ba32` | 0 |
| **htf body resumeSounds before pauseOpen (RS4)** | **3,771** | **1,453** | **8,080** | `0x17ba32` | **0** |
| RS4 + weaponSlot DACB | 3,770 | 1,521 | 8,080 | `0x17ba32` | 0 |
| RS4 + case 0 first | 3,764 | 1,463 | 8,080 | `0x17ba32` | 5 |
| RS4 + `expect(0x200,1)` | 3,771 | 1,453 | 8,080 | `0x17ba32` | 0 |
| htf body resumeSounds first | 4,729 | 1,451 | 8,052 | `0x17ba16` | 0 |
| htf body closeHudMenu last | 4,769 | 1,462 | 8,040 | `0x17ba0a` | 0 |
| htf `_bc8` before resumeSounds | 4,709 | 1,482 | 8,076 | `0x17ba2e` | 0 |
| `won != 0` instead of `== true` | 6,100 | 2,275 | 8,068 | `0x17af92` | 0 |
| campaign mission-id chain "original order" | 4,694 | **942** | 8,084 | `0x17b9a6` | 0 |
| `expect(hudTouchFlags == 0, 0)` | 5,920 | 1,602 | 8,076 | `0x17af90` | 0 |
| `expect(hudMenuOpen, 1)` | 5,961 | 1,807 | 8,068 | `0x17afa0` | 26 |
| `expect(0x400, 0)` | 4,696 | 1,654 | 8,072 | `0x17b81a` | 0 |
| `expect(0x200, 0)` | 4,688 | 1,658 | 8,072 | `0x17b81a` | 4 |
| `expect(autopilot, 1)` | 4,377 | 1,486 | 8,076 | `0x17ba30` | 0 |
| `expect(autopilot, 0)` | 4,063 | 1,464 | 8,084 | `0x17ba28` | 0 |

Sweeps (each measured independently, all size 8,080 unless noted):

- all 24 permutations of the `weaponSlot >= 0` body statements: best 3,772
  (closeHudMenu first, four orderings), aligned 1,521;
- all 24 permutations of the `wingmanCommand >= 0` body statements: base order
  is best (3,773);
- all 120 permutations of the `hudTouchFlags == 0` body with closeHudMenu
  first: best 3,771, six orderings, all with `pauseOpen` after `resumeSounds`;
- all 120 permutations with `pauseOpen` last: best 3,771;
- 24 RS4 x weaponSlot permutations: best 3,770 (RS4 + closeHudMenu first),
  aligned 1,521;
- 24 RS4 x wingman permutations: base order best (3,771);
- cameraMode switch/if-else forms, `expect` on every dispatcher arm and on the
  `hudMenuOpen`/`hudTouchFlags`/`autopilot` tests, `won` spelling variants,
  literal spellings (`= false`, `!flags`, `0 <= slot`, combined declarations),
  `default:` case, and the separate-`if` forms. Nothing else reached 8,080 with
  `.rel.plt` 0 below 3,771.

### The `.rel.plt` trap

Every shape that moves the `maneuverTouchEnd` block earlier than
`0x17ae24`/`0x17aeb4` (the first `PlayerEgo::dockToStream` and `MGame::useCloak`
references) rotates three `.rel.plt` entries (`dockToStream`, `useCloak`,
`maneuverTouchEnd` -> `maneuverTouchEnd`, `dockToStream`, `useCloak`). The
cameraMode case-0-first shapes all do this and therefore cost 5 `.rel.plt`
bytes even though they are the lowest raw shapes found (3,764). `.rel.plt`
order is the order of first relocation in the object, so it cannot be repaired
without moving the starMap/`0x800` blocks ahead of the maneuver block.

## Reproduction

```
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5MGame10OnTouchEndEiiPv
cmake --build cmake-build-match --target check -j2
```

Scratch helpers (all under `/home/debian/GalaxyOnFire2/scratch/w11919`):
`measure.py` (link + fn_diff + epilogue + relplt), `runvar.py <json>` (batch
source-variant runner: a list of `{name, reps:[[old,new],...]}` applied to
`MGame.base.cpp`, builds, prints raw/aligned/size/epi/relplt, restores base),
`ranges2.py <so>` (same-address diff run map), `relplt_names.py` (decodes
`.rel.plt` symbol order), `mbpsim.py` (MBP frequency simulator over `mir.txt`),
`dumpmir.sh` (LLVM `-print-machineinstrs` dump), `mbpfull.py` (full MBP chain
simulation, partial), `bpi.cpp`/`mbp.cpp` (LLVM 7.0.1 sources used for the
conflict-threshold analysis), plus the `w*.json`/`w*.out` experiment batches.
