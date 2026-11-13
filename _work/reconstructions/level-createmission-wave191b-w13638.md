# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 191b / w13638

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave191b-w13638`, base master `57ce1a4c8`. Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base 0x10000).

Continues `level-createmission-wave191b-w13625.md` (post-RA MachineLICM
diagnosis), `...w13615.md`, `...w13601.md`, `...w13589.md`, `...w13578.md`.

```text
                                     size            raw    aligned  insn
base 57ce1a4c8                       17172/17172     2626    845     6206/6197
this pass (no new win)               17172/17172     2626    845     6206/6197
```

No size-exact raw win was found; the tree is left at `57ce1a4c8`
(`git status --porcelain` empty). `.ARM.extab` unchanged (577 differing
bytes, size/base exact at `0x1a3b78`); `.text` diff 71,197, `.ARM.exidx` 0,
`.rodata` 0. All 27 section headers still byte-identical.

## 1. NEW: the exact hoist gate is `isLoadFromStackSlot`, and it requires a
##    **zero FI offset** — a concrete second lever besides `isSpillSlotObjectIndex`

`MachineLICMBase::ProcessMI` (LLVM 7.0.2 `lib/CodeGen/MachineLICM.cpp`) makes a
reload a candidate only through the second disjunct:

```cpp
if ((!HasNonInvariantUse && IsLICMCandidate(*MI)) ||
    (TII->isLoadFromStackSlot(*MI, FI) && MFI->isSpillSlotObjectIndex(FI)))
  Candidates.push_back(CandidateInfo(MI, Def, FI));
```

An FI operand sets `HasNonInvariantUse = true`, so for a reload the first
disjunct is dead. The second disjunct has **two** independently breakable
terms:

1. `MFI->isSpillSlotObjectIndex(FI)` — w13625's conclusion: the slot must be a
   front-end stack object, not an RA spill slot.
2. `TII->isLoadFromStackSlot(*MI, FI)` — for ARM (`ARMBaseInstrInfo.cpp:1338`)
   this additionally requires `MI.getOperand(2).getImm() == 0`, i.e. the load
   must be at **offset 0 of its frame object**.

So a reload at a nonzero FI offset (`t2LDRi12 %stack.X, 4`) is *not* a
candidate even when it is an ordinary spill slot. The emitted instruction is
still the same 16-bit `ldr r5,[sp,#116]`, so this lever can in principle move
the load into the loop body without changing the emitted bytes.

Confirmed empirically: putting `enemyRace` at offset 36 of the case-1
`coords` array (`int coords[10]`, `coords[9] = enemyRace`) leaves the load in
the loop body (`ldr r4,[sp,#348]` at `0xb1614`, no preheader load), i.e. the
hoist is blocked. The cost is that the array grows and the whole function
re-allocates (see §3).

## 2. Baseline region table (unchanged, `regionraw.py`)

| region | diff bytes | mechanism |
|---|---:|---|
| `0x0..0x1500` | 2 | branch-target indices |
| `0x1500..0x178e` (case-1 `enemyRace` reload) | 300 | **post-RA MachineLICM hoist** |
| `0x178e..0x1930` (case-2 prologue) | 39 | store scheduling of the same stores |
| `0x1930..0x1980` (case-2 loot) | 17 | source order (CSGR best, w13567) |
| `0x1980..0x322e` | 66 | `r1` vs `r6` regalloc + a tail-merged `Objective` epilogue |
| `0x322e..0x4314` (alienGuns/fortress/raid tail) | 2202 | **greedy-RA live range** |
| total | 2626 | |

The case-1 preheader is the *same length* in both binaries (20 B):
ours has the extra 2-B `ldr r5,[sp,#116]` and no `nop`; the original has no
load and a 2-B `nop` at `0xb159a`. The body is 2 B shorter in ours, so the
`0xb1608..0xb178e` region shifts −2 and the case-2 prologue re-syncs at
`0xb178e`. Fixing the hoist should therefore be a pure raw −341 win at size
17,172 (w13601 §1).

## 3. Measurements (all from base `2626 / 845 / 17172`)

### 3a. Offset-trick / front-end-object shapes (target `isLoadFromStackSlot`)

| variant | raw | aligned | size | insn | hoist blocked? |
|---|---:|---:|---:|---:|---|
| base | **2,626** | **845** | 17,172 | 6206/6197 | no |
| `int coords[10]` + `coords[9]=enemyRace`, loop uses `coords[9]` | 12,061 | 1,541 | **17,180** | 6211/6197 | **yes** (`ldr r4,[sp,#348]` in body) |
| same, but `coords[9]=enemyRace` after `new Route` | 12,045 | 1,517 | **17,180** | 6209/6197 | yes |
| `Vector raceVec={0,0,0}; raceVec.y=(float)enemyRace`, loop uses `(int)raceVec.y` | 16,079 | 1,639 | **17,140** | 6220/6197 | yes (offset 4) |
| `Vector raceVec` + `.z` | 16,079 | 1,639 | **17,140** | 6220/6197 | yes (offset 8) |

Blocking the hoist re-allocates the whole function (the case-1 `this` moves
from `sl` to `r5`, etc.) and changes the size, so none is a landing candidate.

### 3b. Front-end-object shapes (target `isSpillSlotObjectIndex`)

| variant | raw | aligned | size | insn |
|---|---:|---:|---:|---:|
| `volatile int enemyRace` | 16,402 | 2,309 | **17,232** | 6195/6197 |
| `*(volatile int *)&enemyRace = expr;` (volatile store only) | 16,406 | 2,369 | **17,232** | 6197/6197 |
| same, via an `int erTmp` | 16,406 | 2,369 | **17,232** | 6197/6197 |
| `int *volatile p = &enemyRace; (void)p;` | 16,517 | 2,342 | **17,240** | 6201/6197 |
| `*(volatile int *)&enemyRace = enemyRace;` (self store) | 16,540 | 2,348 | **17,240** | 6201/6197 |
| `__builtin_prefetch(&enemyRace);` | 16,659 | 2,400 | **17,268** | 6199/6197 |
| `volatile int erTmp = expr; int enemyRace = erTmp;` | 16,667 | 2,442 | **17,268** | 6203/6197 |

The volatile-store-only form keeps the *original instruction count*
(6197/6197) but is +60 B: the object is forced into memory, the whole
function re-allocates and ~30 instructions widen to 4-B encodings (e.g.
`str.w r8,[sp,#8]` for a high register). The frame stays `sub sp,#400`
(496 B) in every case, so the +60 is not frame growth.

### 3c. Neutral (SROA folds them back to the same SSA value)

All **2,626 / 845 / 17,172**:

* `(void) &enemyRace;`
* `int *const p = &enemyRace; (void)p;`
* `volatile int *p = &enemyRace; (void)p;`
* `int enemyRace[1]` / `int enemyRace[2]` (constant indices)
* `struct RaceBox { int race; }; RaceBox er; er.race = …`
* `int &er = enemyRace; (void)er;`
* `int missionType = mission->getType(); switch (missionType) {`
* `int type = mission->getType(); switch (type) {`
* alienGuns setup reorder `Vector base` before `int k` / `int k` first
* `int goods` after `new Route` → **2,645 / 837** (aligned win, raw loss)

### 3d. Case-2 loot store order (`GCSR` vs binary `GCSR`)

w13567/w13615 identified the binary order as `cargo`, `crateCaptured`,
`setSpeed`, `setRotationSpeed`. Reordering the source to that order:

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base (`CSGR`) | **2,626** | 845 | 17,172 |
| `cargo; crateCaptured; setSpeed; setRotationSpeed` | 2,631 | **841** | 17,172 |
| `crateCaptured; cargo; setSpeed; setRotationSpeed` | 2,634 | 842 | 17,172 |
| `cargo; setSpeed; crateCaptured; setRotationSpeed` | 2,635 | 841 | 17,172 |

The reorder now emits the two stores in the right order but still re-loads the
array element between them (orig reuses `r1`), so the region goes 17 → 21
bytes and raw rises. A `KIPlayer *ship` local CSEs the load but changes the
size (17,148 / 17,164), so it is not landable.

### 3e. Case-1 loop-body locals

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | **2,626** | **845** | 17,172 |
| `KIPlayer *ship = createShip(...); enemies[i]=ship; ship->...` | 10,722 | 944 | **17,164** |
| `PlayerFighter *ship = (PlayerFighter*)createShip(...)` | 10,722 | 944 | **17,164** |


## 3f. Follow-up sweep (same wave): alienGuns/pirateFortress source order

The binary's `alienGuns` loop interleaves the two rotations with the
`setRotation` call, and puts `cargo = nullptr` *after* `setScaling`:

```text
0xb32f6  blx MatrixRotateVector     (t[3..5])
0xb3300  blx Vector::operator=      (v = result)
0xb3308  blx setRotation(v)
0xb3320  blx MatrixRotateVector     (t[0..2])
0xb3328  blx Vector::operator=      (v = result)
...      setPosition, setScaling
0xb3372  str r0,[r6,#80]            (cargo = nullptr)
0xb337e  str r0,[r6,#40]            (shipGroup)
```

The checked-in source instead has both rotations before `setRotation`, so the
first rotation is dead and `setRotation` receives the *second* vector — a
semantic divergence from the binary, not just a scheduling one.  Likewise
`pirateFortress` has `cargo = nullptr` before `shipGroup` in the binary but
after it in the source.

Reordering to the binary's order was measured; it fixes the call order but
re-allocates the tail:

| variant | raw | aligned | size | insn |
|---|---:|---:|---:|---:|
| base | **2,626** | **845** | 17,172 | 6206/6197 |
| alienGuns order fixed | 2,642 | 1,022 | 17,172 | 6206/6197 |
| alienGuns order + `gun` after store | 2,643 | 1,018 | 17,172 | 6206/6197 |
| alienGuns order + both `gun`-after + pirateFortress order | 2,655 | 1,018 | 17,172 | 6206/6197 |
| alienGuns `cargo` moved only (rotations unchanged) | 2,632 | 844 | 17,172 | 6206/6197 |

## 3g. `KIPlayer *gun` must be assigned *after* `createStaticObject`

The binary reloads the array element after the store in both turret loops
(`str r0,[r1,fp,lsl#2]` then `ldr r0,[r4,#248] ... ldr r6,[r0,fp,lsl#2]`), i.e.
the source reads `(*this->enemies)[i]` after the assignment.  The checked-in
source reads it before the store, so `gun` is the *replaced* object.  Moving
the declaration after the store reproduces the binary's shape exactly in the
loop body but does not move the gate:

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | **2,626** | **845** | 17,172 |
| alienGuns `gun` after store | 2,627 | **841** | 17,172 |
| pirateFortress `gun` after store | 2,632 | 842 | 17,172 |
| both | 2,633 | **838** | 17,172 |
| both + GCSR loot order | 2,638 | **834** | 17,172 |

## 3h. The `(i < 0) + (i < 0)` crutch is exactly the `ldrd`/`strd` width

Dropping the crutch for the plain `bigAlienWave ? ... : ...` select gives
**17,168** (confirms w13578 §3): the crutch's four extra instructions are
exactly the 4 B that the `ldrd`/`strd` pairing saves.  A named `Vector` local
plus the plain select gives the first **exact instruction count**
(`6197/6197`) but 17,152 B — the shape is right and only the 16-vs-32-bit
encodings differ:

| variant | raw | aligned | size | insn |
|---|---:|---:|---:|---:|
| plain select | 4,380 | 883 | 17,168 | 6203/6197 |
| plain + alienGuns order | 4,374 | 878 | 17,168 | 6203/6197 |
| plain + alienGuns order + `gun`-after | 4,425 | 1,010 | 17,160 | 6203/6197 |
| plain + `Vector` locals | 4,439 | **813** | 17,152 | **6197/6197** |

## 3i. Other size-breaking shapes (this pass)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| case-2 loot without the `enemies` local alias (CSGR) | 9,973 | 869 | 17,180 |
| case-2 loot without the local alias (GCSR) | 9,967 | 865 | 17,180 |
| raid `int guns` moved after the inner `if` | 4,184 | 849 | 17,164 |
| raid `int guns` hoisted before `if (raid)` | 3,560 | 916 | 17,168 |
| `-mllvm -enable-earlycse-memssa=true` (per-TU flag toggle) | 2,626 | 845 | 17,172 |

The case-2 `enemies` alias is load-bearing: removing it re-schedules the
whole `0x1980..0x322e` region (66 -> 5,921 differing bytes), so the binary
cannot simply be using `this->enemies` there even though the loot block
reloads it twice.

## 4. Why every front-end/offset shape re-allocates

The `enemyRace` value is a plain SSA vreg at ISel (`%398:gpr`, DBG_VALUE
`!"enemyRace"`, ISel dump line 9789 of `mir.txt`), so SROA promotes it and the
RA spills it into `%stack.109` → fi#75 after StackSlotColoring. Forcing it
into a front-end object (volatile/escape/array member) changes its live range
from "one value live across the whole switch" to "one store + per-use
reloads", which changes the global greedy-RA solution — not just this load.
The `this` register moves (`sl`→`r5`), the case-2 prologue re-schedules, and
the size leaves 17,172. The case-1 hoist and the 2.2 KB tail are therefore
still the same single global-RA coupling described in w13625 §2.

## 5. Next steps

1. The offset trick (§1) is the only new structural lever found. To land it
   the front-end object must be introduced **without changing the frame
   layout or the live range**: an object that is still written once before
   the switch and read once per loop iteration, placed at a nonzero offset,
   with the same 4-byte slot as fi#75. Candidate shapes not yet tried: a
   2-word struct in the `else` scope whose address escapes through an
   already-present opaque call (no new call), or reusing the 12-byte
   `playerPos` front-end object's tail slot.
2. The tail (`%698` / `&Globals::rnd`, 2,202 B) is unchanged: greedy-RA keeps
   it in `r4` across alienGuns/pirateFortress; the original reloads
   `[sp,#100]` only at `0xb3788`. Adjacent-statement swaps over 1500–2535 are
   exhausted (w13578, ~450 builds).
3. `GCSR` (§3d) is a measured aligned-only trade (845 → 841, raw +5); it is
   not landed because raw is the primary metric.

## 6. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13638
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
```

Expected: `raw=2626 aligned=845 size=17172/17172 insn=6206/6197`.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13638/`
(`measure.sh`, `try.py`, `regionraw.py`, `v1.py`..`v11.py`, `mir.txt` 58 MB
`-mllvm -print-machineinstrs` dump, `MachineLICM.cpp`,
`ARMBaseInstrInfo.cpp`, `orig.dis`, `ours.dis`, `Level.base.cpp`,
`CMakeLists.base.txt`).
