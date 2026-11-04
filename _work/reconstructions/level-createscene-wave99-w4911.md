# `Level::createScene` — wave-99 w4911: the iOS source-shape oracle, and why the
# all-globals shape still cannot be coloured

**Status:** research note, 2026-11-04. Worktree `wt/w4911`, branch `wave99-w4911`,
base `44225192c`. **No source change landed** — no source shape found that raises
`byte_exact`. This commit contains only this note.

Follow-up to `level-createscene-wave97-w4712.md` (which followed
`level-createscene-wave95-w4514.md`, `…-wave94-w4414.md`, `…-wave92-w4249.md`,
`…-wave92-w4213.md`, `…-wave91-w4113.md`, `level-createscene-phi-use-w4013.md`,
`level-createscene-licm-hoist-w3833.md`, `Level-createScene-flag-free.md`,
`level-createscene-88-3-2252-b_w3813.md`).

## TL;DR

* Target `_ZN5Level11createSceneEv`: **88.3 %**, orig/ours both **2252 B**,
  783/791 instructions, `byte=False`, `linked=False`.
* Gate re-measured before and after this pass, unchanged: **byte_exact
  4500/4524, linked_exact 4500/4524, size-exact 4524/4524, stub_zero_size 0,
  missing 0, wrong_type 0, extra 0, data_exact 843/843, imports 207, lint
  CLEAN ✓, drift 0, RATCHET PASS.**
* **New evidence: the iOS 1.1.4 `Level::createScene` has been decompiled**
  (`FUN_00110b48` @ `0x110b48` in `ios_1.1.4_HD`). It confirms the *source
  shape* of every construct the Android residue depends on — the parts loop,
  the `spots`/`table` selection, the `pick` nested-if, the `-100` retry
  counter, and the `Globals::rnd` direct-access form. No shape in the iOS
  source differs from the shipped `Level.cpp` in a way that moves the Android
  allocation. §3.
* **The all-globals ship-loop prelude has now been swept exhaustively** (all
  66 valid topological orders of `A=ownHangar, B=capacity, C=shipCount,
  D=if(ownHangar), E=spotUsed, G=spots`, constraints `B<C`, `A<D`, `B<E`,
  `C<D`). The best is `B A C D E G` (`capacity` first) at
  **77.3 % / 2264 B**, confirming wave-95 §5.3; the shipped order
  `A B C D E G` is 75.0 %. **No all-globals order reaches 88.3 %.** §5.
* The shipped (local-`rnd`) shape remains a very sharp local maximum: the
  full 2^8 local-vs-`Globals::rnd` matrix re-measured on the current base
  peaks at **88.3 % at mask 82/83**; every other mask is ≤ 88.1 %. §6.
* The remaining lever is unchanged and now precisely stated: the original
  keeps `&Globals::rnd` in **`r8`** and reloads the value at all seven sites,
  while the shipped shape spends `r5` on the cached value and spills the
  address to `sp#44`. The all-globals shape has the right *shape* but the
  greedy allocator gives it `&rnd → r9`, `&status → r6`, `capacity → r8`,
  `i → sl` — a one-step rotation of the original's
  `{&rnd → r8, &status → sl, capacity → r6, i → r9}`. §4, §7.

## 1. Baseline diff

`tools/verify/verify.py --show _ZN5Level11createSceneEv` at the baseline:
290 `|` lines, all inside the `missionPtr == 23` branch (68 in the parts
loop, 222 in the ship loop). The first 459 instructions are byte-identical;
the first diff is instruction 460, the parts-loop preheader.

The instruction *stream* is the same on both sides — only register names,
spill-slot numbers and encoding widths differ (plus 8 extra spill/reload
instructions on our side).

### 1.1 Register maps (recovered from the disassembly)

Parts loop (`for (int k = 0; k < 4; k++)`, `kHangarMeshes[sceneRace][k]`):

| value | original | shipped source | all-globals source |
|---|---|---|---|
| row `kHangarMeshes[sceneRace]` | **`r9`** (spill `sp#24`) | `r8` (spill `sp#24`) | `r8` (spill `sp#28`) |
| `Globals::Canvas` value | **`r8`** | `r9` | `sl` |
| zero of `Vector{0,pi,0}` | **`sl`** | `r4` | `r9` |
| `k` | **`r6`** | spilled `sp#44` | `r6` |
| `hangarPart` | **`r4`** (spill `sp#44`) | `r6` | `r4` (spill `sp#36`) |
| `g` | `r5` | `r5` | `r5` |
| `j` | **`sl`** | `sl` | `fp` |
| child count `kBarChildCounts[sceneRace]` | **`fp`** | `r8` | `r9` |
| `&Globals::status` | spill `sp#40` | spill `sp#40` | spill `sp#44` |

Ship loop (`for (int i = 0; i < shipCount; i++)`):

| value | original | shipped source | all-globals source |
|---|---|---|---|
| `&Globals::rnd` | **`r8`** (value reloaded at every site) | spill `sp#44` | `r9` |
| `rnd` value | never cached | `r5` | never cached |
| `capacity` | **`r6`** | `fp` | `r8` |
| `race` | **`r5`** | `r6` | `r5` |
| `type` | **`fp`** | `r8` | `fp` |
| `i` | **`r9`** | `r9` | `sl` |
| `&Globals::status` | **`sl`** | `sl` | `r6` |
| `spotUsed` | `r4` | `r4` | `r4` |

The original's ship-loop assignment is a 4-cycle of the shipped one:
`rnd: r5→r8`, `type: r8→fp`, `capacity: fp→r6`, `race: r6→r5` (with the
shipped `rnd` *value* replaced by the `&rnd` *address*). The parts loop is a
separate permutation (`row r8→r9`, `Canvas r9→r8`, `zero r4→sl`,
`k spill→r6`, `hangarPart r6→r4`, `cc r8→fp`).

### 1.2 Where the 8 extra instructions are

* **Parts loop.** Shipped: `k` is spilled to `sp#44` (`str r6,[sp,#44]`) and
  restored at the latch (`ldrd sl,r6,[sp,#40]`), because `hangarPart` takes
  `r6`. Original: `k` stays in `r6`, `hangarPart` takes `r4` and is spilled
  to `sp#44` for the child loop only.
* **Ship loop.** Shipped: `&Globals::rnd` is spilled to `sp#44` and both the
  address and the value are reloaded at the five `Globals::rnd` sites
  (`ldr rX,[sp,#44]; ldr r0,[rX,#0]`). Original: `&Globals::rnd` stays in
  `r8` and only the value is reloaded (`ldr.w r0,[r8]`).

## 2. The original source uses `Globals::rnd` and `Globals::status` directly

Re-confirmed from the Ghidra decompile of the target
(`/dev/shm/w4911/decomp.txt`, `FN_ADDR=0xc42ec`; image base `0x10000`):

```c
puVar24 = *(undefined4 **)(DAT_000c4ba0 + 0xc492c);   /* &Globals::rnd */
uVar23  = *(uint *)(DAT_000c4ba4 + 0xc492e + uVar8 * 4); /* capacity */
local_74 = AbyssEngine::AERandom::nextInt((AERandom *)*puVar24, uVar23 + 1);
...
iVar18 = AbyssEngine::AERandom::nextInt((AERandom *)*puVar24, 100);
```

`puVar24` is the **address** of the global pointer (the GOT entry holds
`&Globals::rnd`), and `*puVar24` is the value, reloaded at every one of the
seven `nextInt` sites (`ldr.w r8,[r0]` at `b492c`, `ldr.w r0,[r8]` at
`b4936/b49ae/b49be/b49ca/b49f6/b4a46/b4a56`). Every `Globals::status` site
likewise reloads through `sl` (`ldr.w r0,[sl]`). There is no local
`AERandom *rnd` and no local `Status *status` in the original source.

## 3. New: the iOS 1.1.4 `Level::createScene` decompile

`ios_1.1.4_HD` (Mach-O armv7) was imported into a Ghidra project
(`/dev/shm/w4911/iosproj6`) and `FUN_00110b48` @ `0x110b48` decompiled. It is
`Level::createScene`: it references the same tables as the Android binary —

| iOS vaddr | Android table |
|---|---|
| `0x3ecca0` (`__const`) | `kHangarMeshes[10][4]` |
| `0x3ecba0` | `kShipHangarHeights[64]` |
| `0x3ecd40` | `kHangarCapacity[10]` |
| `0x3ed170` / `0x3ed180` | `kBarChildCounts[4]` / `kBarChildMeshes[4]` |
| `0x3ece1c` / `0x3ece40` | `kHangarSpotsKhador` / `kHangarSpotsDefault` |

The decompiled ship loop is (abridged, variable names added):

```c
spots = &kHangarSpotsDefault;
if (sceneRace == 7) spots = &kHangarSpotsKhador;
i = 0;
do {
    r = nextInt(Globals::rnd, 100);
    race = sceneRace;
    if (r < 30) {
        race = nextInt(Globals::rnd, 4);
        if (nextInt(Globals::rnd, 100) < 30) race = 8;
    }
    type = Globals::globals->getRandomEnemyFighter(race);
    if (Globals::status->getStation()->getIndex() == 100) {
        pick = nextInt(Globals::rnd, 3);
        type = 37;                          /* nested ifs, not a ternary */
        if (pick != 0) { type = 40; if (pick == 1) type = 38; }
    }
    if (ownHangar) type = (*Globals::status->voidStation->getShips())[i]->getIndex();
    ship = createShip(this, 0, 0, type, 0, 0, 0);
    spot = nextInt(Globals::rnd, capacity);
    tries = 99;                             /* count-up retry */
    if (spotUsed[spot] & 1) {
        do {
            spot = nextInt(Globals::rnd, capacity);
            if (!(spotUsed[spot] & 1)) break;
        } while (tries-- > 0);
    }
    spotUsed[spot] = 1;
    table = &kHangarSpotsVossk;             /* switch(0,1,3,8) lowering */
    if (sceneRace == 0) table = &kHangarSpotsTerran;
    if (sceneRace > 1) table = &kHangarSpotsNivelian;
    if ((sceneRace > 3 || sceneRace == 2) && (table = spots, sceneRace == 8))
        table = &kHangarSpotsVoid;
    ship->setPosition(table[spot*3], table[spot*3+1] + kShipHangarHeights[type],
                      table[spot*3+2]);
    ship->player->setAlwaysFriend(true);
    ship->setToSleep();
    ship->geometry->setRotation(0, nextInt(Globals::rnd, 300) / 100.0f, 0);
    ship->setExhaustVisible(false);
    ArrayAdd(ship, enemies);
    i++;
} while (i != shipCount);
```

Every construct matches the shipped `Level.cpp`:

* the `spots` ternary is hoisted out of the loop (the iOS `local_100` is
  computed before the `do` loop);
* the `table` selection is the same `switch (sceneRace)` over
  `{0,1,3,8}` with `default: table = spots` — the Android `b4a6e..b4a88`
  bitmask `(0x10b >> race) & 1` + value-table lookup is exactly clang's
  lowering of that switch;
* the `pick` selection is `type = 37; if (pick != 0) { type = 40; if
  (pick == 1) type = 38; }` — the Android `b49f6..b4a10`
  (`mov fp,#40; cmp r0,#1; moveq fp,#38; cmp r0,#0; moveq fp,#37`) is the
  same nested-if, and the shipped ternary `pick == 0 ? 37 : pick == 1 ? 38 :
  40` compiles to exactly this stream;
* the retry loop counts **up** from `-100` (iOS `tries = 99`), which is what
  clang already emits for the shipped `for (int tries = 100; tries != 0; tries--)`
  (`mvn r5,#99; adds r5,#1` at `b4a50/b4a60`);
* every `nextInt` site reads `Globals::rnd` directly.

The iOS parts loop is also the shipped shape: `g = new AEGeometry(mesh,
Globals::Canvas, false); Vector v = {0, pi, 0}; g->setRotation(v);
hangarPart = new PlayerStatic(-1, g, 0,0,0); if (!plainHangar) { … child
loop … } ArrayAdd(hangarPart, enemies);` with the row pointer
`&kHangarMeshes[sceneRace]` hoisted before the loop.

**Conclusion:** the iOS oracle does not reveal any source-shape difference
from the shipped file. The remaining gap is purely the Android greedy
allocator's colouring, not a missing statement.

## 4. The greedy-allocator model (recap and refinement)

`RAGreedy::enqueue` (LLVM 7.0.1, `lib/CodeGen/RegAllocGreedy.cpp:680`):

* one-MBB (local) intervals: `Prio = beginIndex().getInstrDistance(getLastIndex())
  | RC.AllocationPriority << 24`;
* global/split intervals: `Prio = (1u << 29) + Size`;
* both `|= 1u << 31`; a physical-register hint adds `1u << 30`;
* tie-break is `~Reg` (lower vreg first).

For GPR, `AllocationPriority == 0`, so **all global intervals are dequeued
before any local interval**, in decreasing `Size` order. The parts-loop
`%541 = t2ADDri %stack.1, 0` is local (one MBB) and is therefore allocated
*after* every global interval in the function — including the ship loop's
`&Globals::rnd`, `capacity`, `&status` and `i`. This is why the parts loop is
not controllable from the parts loop: its registers are whatever the ship
loop's global intervals left free.

The original's assignment is consistent across both loops: `r8` is the
"globals address" register (`Globals::Canvas` in the parts loop,
`&Globals::rnd` in the ship loop), `r9` is the "row / loop counter"
register, `r6` is "k / capacity", `r4` is the "object pointer", `r5` is the
"temp", `fp` is "child count / type", `sl` is "j / &status". The shipped
shape gets only `{g, j, i, &status, spotUsed}` right; the all-globals shape
gets only `{k, hangarPart, g, race, type, spotUsed}` right.

## 5. New: exhaustive all-globals ship-loop prelude sweep

The seven prelude statements were reduced to six for the all-globals shape
(no local `rnd`):

| tag | statement |
|---|---|
| A | `bool ownHangar = false; if (getStation()->getIndex()==108) ownHangar = field_114==3;` |
| B | `int capacity = kHangarCapacity[sceneRace];` |
| C | `int shipCount = Globals::rnd->nextInt(capacity + 1);` |
| D | `if (ownHangar) { … ships->size() … }` |
| E | `unsigned char *spotUsed = new unsigned char[capacity]; for (…) spotUsed[i]=0;` |
| G | `const int *spots = sceneRace == 7 ? kHangarSpotsKhador[0] : kHangarSpotsDefault[0];` |

All 66 orders satisfying `B<C`, `A<D`, `B<E`, `C<D` were built with the
official `link_so` target and scored. Top of the table:

| order | pct | size | insns |
|---|---|---|---|
| **B A C D E G** (capacity first) | **77.3** | 2264 | 806 |
| B G A C D E | 76.0 | 2264 | 806 |
| G B A C D E | 76.0 | 2264 | 806 |
| B A C D G E | 75.8 | 2268 | 808 |
| **A B C D E G** (shipped order, all-globals) | 75.0 | 2228 | 787 |
| G A B C D E | 74.9 | 2228 | 787 |
| B A C G D E | 74.5 | 2228 | 787 |
| B A G C D E | 74.5 | 2228 | 787 |
| A B C D G E | 74.3 | 2228 | 787 |
| A B C G D E | 74.1 | 2228 | 787 |

**No all-globals order reaches the shipped shape's 88.3 %.** The
`capacity`-first order is the only one that reproduces the original's
parts-loop registers (wave-95 §5.3), but it moves the `capacity` load in
front of the `ownHangar` computation, which the original does not do
(`b4900..b4922` ownHangar, `b4924..b4932` capacity), and it duplicates the
`nextInt(capacity+1)` call into both arms of the `ownHangar` branch (806
instructions vs 783).

## 6. The 2^8 local-vs-`Globals::rnd` matrix re-measured

Bit order `shipCount, top, nextInt(4), inner, pick, spot, tries, rotation`.
The shipped source is mask 82 (`top, pick, tries`).

| mask | sites using the local | pct | size | insns |
|---|---|---|---|---|
| **82** | top, pick, tries (shipped) | **88.3** | 2252 | 791 |
| 98 | top, spot, tries | 88.1 | 2252 | 791 |
| 210 | top, pick, tries, rotation | 87.7 | 2252 | 791 |
| 86 | top, nextInt(4), tries | 85.3 | 2244 | 790 |
| 214 | top, nextInt(4), pick, tries | 85.2 | 2244 | 790 |
| 118 | top, nextInt(4), spot, tries | 85.1 | 2244 | 790 |
| 70 | top, nextInt(4), tries | 85.0 | 2248 | 792 |
| 254 | all except shipCount | 83.7 | 2236 | 786 |
| 0 | none (all-globals) | 75.0 | 2228 | 787 |

Masks with bit 0 (`shipCount`) set require moving the `rnd` declaration
before `shipCount`; that position alone costs ~25 points (63.6 % at mask
83), so masks 83/99/211/… are not viable.

## 7. Other source-shape measurements this pass (all ≤ 88.3 %)

Baseline: **88.3 % / 2252 B / 791 insns**. Every row is a full
`cmake --build cmake-build-match --target link_so` + a single-function verify.

| variant | pct | size | insns |
|---|---|---|---|
| retry loop `for (tries = -100; tries != 0; tries++)` | 87.6 | 2252 | 792 |
| retry loop `while (tries != 0 && …) { …; tries++; }` | 87.6 | 2252 | 792 |
| retry loop `spotUsed[spot]` (no `!= 0`) | 88.3 | 2252 | 791 |
| retry loop `tries > 0` | 88.3 | 2252 | 791 |
| retry loop `do { … } while` (iOS shape) | 72.0 | 2224 | 793 |
| ship loop `while (i < shipCount) { …; i++; }` | 88.3 | 2252 | 791 |
| ship loop `do { … } while` | 85.5 | 2248 | 789 |
| parts loop `while (k < 4)` | 67.8 | 2292 | 799 |
| parts loop `do { … } while (k < 4)` | 87.8 | 2252 | 791 |
| `if (mesh == -1) continue;` | 88.2 | 2252 | 791 |
| `if (mesh >= 0) { … }` (block form) | 88.3 | 2252 | 791 |
| `spots` computed inside the loop | 88.3 | 2252 | 791 |
| `spots` inside `if (shipCount > 0)` | 76.0 | 2264 | 802 |
| `const int *table = spots;` before the switch | 88.3 | 2252 | 791 |
| `const int *table = nullptr;` + `if (table == nullptr)` | 73.7 | 2224 | 785 |
| `if/else` chain instead of the switch | 71.8 | 2260 | 803 |
| `const int *caps = kHangarCapacity;` local | 88.3 | 2252 | 791 |
| `const int *heights = kShipHangarHeights;` local | 88.3 | 2252 | 791 |
| `int sr = sceneRace;` used in the loop | 88.3 | 2252 | 791 |
| `Status *st = Globals::status;` local | 71.6 | 2244 | 792 |
| `for (int i = 0; i != shipCount; i++)` | 88.3 | 2252 | 791 |
| `int race;` declared outside the loop | 88.3 | 2252 | 791 |
| `int type;` declared outside the loop | 88.3 | 2252 | 791 |
| `unsigned int race` | 88.3 | 2252 | 791 |
| `unsigned int k` | 88.3 | 2252 | 791 |
| `unsigned int j` | 88.3 | 2252 | 791 |
| `unsigned int mesh` | 76.2 | 2244 | 795 |
| `short mesh` | 78.6 | 2260 | 792 |
| `short j` | 84.7 | 2256 | 792 |
| `int capacity;` declared before `ownHangar`, assigned after | 88.3 | 2252 | 791 |
| `capacity` before `ownHangar` (shipped shape) | 80.0 | 2276 | 800 |
| `capacity` + `shipCount` before `ownHangar` | 83.1 | 2244 | 789 |
| `bool ownHangar = … && …` (one expression) | 88.3 | 2252 | 791 |
| `int ownHangar = … && …` | 88.3 | 2252 | 791 |
| `plainHangar = sceneRace > 3 \|\| sceneRace == 1` | 88.3 | 2252 | 791 |
| `rnd` declared before `shipCount` | 63.6 | 2224 | 799 |
| `rnd` declared before `ownHangar` | 72.0 | 2232 | 800 |
| `rnd` used at all sites except `shipCount` | 83.7 | 2236 | 786 |

The shipped shape is a very sharp local maximum: ~40 semantically neutral
rewrites all reproduce exactly 88.3 % / 2252 B / 791 insns.

## 8. What this proves / what remains

**Proved:**

1. The whole `createScene` residue is register allocation; the instruction
   stream is identical and the first 459 instructions are byte-identical.
2. The original source uses `Globals::rnd` and `Globals::status` **directly**
   (address in a callee-saved register, value reloaded at every use); it has
   no `AERandom *rnd` and no `Status *status` local. The iOS 1.1.4 decompile
   independently confirms every source shape in the ship loop and parts loop.
3. The all-globals ship-loop prelude is exhaustively swept: the best order
   is `capacity`-first at 77.3 %, and no order reaches 88.3 %.
4. The 2^8 local-vs-`Globals::rnd` matrix peaks at 88.3 % (mask 82/83).
5. The original's ship-loop assignment is a 4-cycle of the shipped one
   (`rnd: r5→r8`, `type: r8→fp`, `capacity: fp→r6`, `race: r6→r5`) with the
   cached value replaced by the address.

**Remaining lever** (none landed): a source shape that makes the greedy
allocator colour `&Globals::rnd → r8`, `&Globals::status → sl`,
`capacity → r6`, `i → r9` in the ship loop **while keeping the shipped
shape's prologue** (the shipped prologue is byte-identical to the original;
the all-globals prologue moves the `__stack_chk_guard` address from `sp#36`
into `r5`). The all-globals shape is the right *kind* of shape but shifts
the whole-function queue by one position; the shipped shape has the right
prologue but caches the `rnd` value and spills the address. No source
spelling tried so far (≈ 1050 variants cumulative across waves 91–99)
produces the original's colouring.

## 9. Reproduction

Scratch is `/dev/shm/w4911`:

* `Level.cpp.pristine` — pristine `src/game/world/Level.cpp`;
* `fast.py` — single-function score (size, pct, orig/ours insn counts);
* `batch2.py <json>` — apply `[name, [[old,new],…]]` replacements to the
  pristine base, build `link_so`, score;
* `sweep_ag.py` — the all-globals 6-statement prelude sweep (§5);
* `gen_mask.py` — the 2^8 matrix generator (§6);
* `dump.sh <name>` — full disassembly of the current `.so` to
  `/dev/shm/w4911/<name>.txt`;
* `opseq.py <orig.txt> <ours.txt>` — opcode-sequence diff (used to locate
  the extra instructions);
* `decomp.txt` — Ghidra decompile of the Android target (`FN_ADDR=0xc42ec`);
* `ios_decomp.txt` — Ghidra decompile of the iOS `FUN_00110b48`
  (`iosproj6`, `ios_1.1.4_HD`, full analysis);
* `orig.txt` / `ours.txt` / `allg.txt` / `alllocal.txt` / `agcap.txt` —
  annotated disassemblies of the baseline and the measured shapes.

Build recipe used:

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
```
