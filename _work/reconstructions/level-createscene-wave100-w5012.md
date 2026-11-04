# `Level::createScene` — wave-100 w5012: from-scratch parts-loop reconstruction, and the
# capacity-before-ownHangar lever that reproduces the original parts loop exactly

**Status:** research note, 2026-11-04. Worktree `wt/w5012`, branch `wave100-w5012`,
base `44225192c`. **No source change landed** — no source shape found that raises
`byte_exact`. This commit contains only this note.

Follow-up to `level-createscene-wave97-w4712.md` (which followed
`level-createscene-wave95-w4514.md`, `level-createscene-wave94-w4414.md`,
`level-createscene-wave92-w4249.md`, `level-createscene-wave92-w4213.md`,
`level-createscene-wave91-w4113.md`, `level-createscene-phi-use-w4013.md`,
`level-createscene-licm-hoist-w3833.md`, `Level-createScene-flag-free.md`,
`level-createscene-88-3-2252-b_w3813.md`).

## TL;DR

* Target `_ZN5Level11createSceneEv`: **88.3 %**, orig/ours both **2252 B**,
  783/791 instructions, `byte=False`, `linked=False`.
* Gate re-measured before and after this pass, unchanged: **byte_exact
  4500/4524, linked_exact 4500/4524, size-exact 4524/4524, stub_zero_size 0,
  missing 0, wrong_type 0, extra 0, data_exact 843/843, imports 207, lint
  CLEAN ✓, drift 0, RATCHET PASS.**
* **A from-scratch parts-loop reconstruction confirms the parts-loop source is
  already right.** The parts loop is 72 instructions on both sides; the
  instruction *stream* is identical modulo register names and one spill pair
  (`str r6,[sp,#44]` + `ldrd` vs `str r4,[sp,#44]` + two `ldr`s). No rewrite
  of the loop body (canvas local in/out of the loop, `KIPlayer*` hangarPart,
  child-count/child-mesh locals, named `Vector`, int zeros, …) moves the
  allocation. §4.
* **The original's parts-loop register assignment *is* reachable, and this
  pass pins down the exact condition: `capacity` must be loaded before
  `ownHangar`.** In the all-globals shape, every prelude order with
  `B(capacity) < A(ownHangar)` reproduces all eight parts-loop values
  (`row→r9, Canvas→r8, zero→sl, k→r6, hangarPart→r4, g→r5, j→sl,
  childCount→fp, child-mesh temp→r9`); every order with `A < B` does not.
  Best such order `B A C D E F` = **77.2 % / 2264 B / 806 insns**. §5.
* **But the original's instruction order is `A` then `B`** (ownHangar at
  `0xb4900..0xb491e`, the `kHangarCapacity` load at `0xb4922..0xb4932`), so
  the lever that fixes the parts loop also breaks the ship-loop prologue
  (`this` spills to `sp#48` and is reloaded twice, the canary moves to
  `sp#40`, `&Globals::status` is spilled to `sp#44`). It therefore cannot be
  the original source as written. §5.2.
* **The local-`rnd` mask sweep is now exhaustive and correct** (the wave-97
  handoff's sweep had a scoping bug: `if (rnd->nextInt(100) <= 29)` also
  occurs in `createMission` at line 1838). Result: all 128 masks were built;
  the decisive site is **bit 5, the `tries` loop `spot = rnd->nextInt(capacity)`**.
  Every mask without bit 5 gives the all-globals parts loop; every mask with
  bit 5 plus at least one other site gives the shipped parts loop; bit 5 alone
  gives a third, larger-frame shape. The shipped `{0,3,5}` (`0x29`) is the
  best of all 128 at 88.3, and **no mask produces the original's parts loop**. §6.
* The iOS 1.1.4 oracle (`_work/bins/ios_1.1.4_HD`) has no `Level` symbols
  (1528 syms, game code stripped) — it is **not** usable for this TU. §8.

## 1. Baseline diff

`python3 /dev/shm/w5012/fscore.py --diff` at the baseline: 276 `|` lines, all
inside the `missionPtr == 23` branch. The first 459 instructions are
byte-identical; the first diff is instruction 460, the parts-loop preheader.

Normalising registers (`r0..r12`, `sl`, `fp`, `sp`, `lr`, `pc` → `R`) and
branch/literal addresses, the shipped source differs from the original in
**only** these places:

1. the parts loop: `zero` register, the `k`/`hangarPart` spill choice, and the
   `ldrd` vs two `ldr`s at the latch;
2. the ship loop: the local `rnd` value and the `&Globals::rnd` spill;
3. a `+2`/`-1` group in the `switch (sceneRace)` table selection
   (`b.n 0x79c; ldr r1,[sp,#24]` inserted, `ldr r1,[sp,#44]` deleted).

Everything else is pure register renaming. The 8-instruction gap (791 vs 783)
is 7 instructions of ship-loop `rnd` handling plus the net +1 of the switch
group; the parts loop itself is instruction-count-neutral (its extra `str
r6,[sp,#44]` is offset by the `ldrd` merge at the latch). The `rnd` sites:

| site | original | shipped |
|---|---|---|
| prelude | `ldr.w r8,[r0]` (`&Globals::rnd`) | `ldr r2,[r0,#0]` + `str r2,[sp,#44]` |
| shipCount | `ldr.w r0,[r8]` | `ldr r0,[r2,#0]` |
| local rnd load | — | `ldr r0,[sp,#44]` + `ldr r5,[r0,#0]` |
| site 1 (`nextInt(4)`) | `ldr.w r0,[r8]` | `mov r0,r5` |
| site 3 (`pick`) | `ldr.w r0,[r8]` | `mov r0,r5` |
| site 4 (`spot`) | `ldr.w r0,[r8]` | `ldr r0,[sp,#44]` + `ldr r0,[r0,#0]` |
| site 5 (`tries`) | `ldr.w r0,[r8]` | `mov r0,r5` |
| site 6 (`rotation`) | `ldr.w r0,[r8]` | `ldr r0,[sp,#44]` + `ldr r0,[r0,#0]` |

The original therefore has **no cached `rnd` value** — it reloads
`Globals::rnd` at all eight sites (shipCount + seven in-loop). This confirms
wave-94/95: the original source uses `Globals::rnd` directly.

## 2. The parts-loop signature

A small extractor (`/dev/shm/w5012/sig.py`) reads the eight values out of the
linked disassembly. The original and the two known shapes:

| value | original | shipped | all-globals |
|---|---|---|---|
| `row` (`kHangarMeshes[sceneRace]`) | **r9** | r8 | r8 |
| `Globals::Canvas` value | **r8** | r9 | sl |
| zero (`Vector{0,pi,0}` / `PlayerStatic` args) | **sl** | r4 | r9 |
| `k` | **r6** | r6 (spilled `sp#44`) | r6 |
| `hangarPart` | **r4** (spilled `sp#44`) | r6 | r4 |
| `g` | **r5** | r5 | r5 |
| `j` | **sl** | sl | fp |
| child count `kBarChildCounts[sceneRace]` | **fp** | r8 | r9 |
| child-mesh temp (`kBarChildMeshes[sceneRace]+j`) | **r9** | fp | r8 |

The original's parts loop is 72 instructions; so is the shipped one. The
shipped body pays one extra `str r6,[sp,#44]` (spilling `k`) but merges the
latch's two loads into one `ldrd sl,r6,[sp,#40]`, so the loop length is
identical. The whole parts-loop residue is therefore a *register permutation*,
not an instruction-stream difference.

## 3. The greedy-queue model (from wave-95, re-confirmed)

`RAGreedy::enqueue` (LLVM 7.0.2 `RegAllocGreedy.cpp`):

* global ranges: `Prio = (1u<<31) + Size` (+`1u<<30` if hinted); ties broken
  by `~Reg` (lower vreg first);
* local (one-MBB) ranges: `Prio = beginIndex.getInstrDistance(lastIndex) |
  RC.AllocationPriority<<24`, i.e. **allocated in linear instruction order**,
  after every global range.

Measured live sizes for the shipped shape (slot units, 16 per instruction,
`liveness2.py`): `%368(zero)=3088`, `%529(row)=1856`, `%6(k)=1840`,
`%533(&Canvas)=1456`, `%10(g)=1392`, `%11(hangarPart)=1056`,
`%12(childCount)=688`, `%13(j)=672`, `%18(child)=432`, `%9(mesh)=368`,
`%534(Canvas value)=96`.

`%534` is a one-MBB local range, so it is coloured by linear order at its
position in the parts loop. That is why the parts-loop permutation is
reachable only by changing the *global* queue (the ship loop), never by
rewriting the loop body.

## 4. From-scratch parts-loop reconstruction (all no change)

The current loop body is already the right source. Every semantically
equivalent rewrite measured this pass reproduces the shipped parts loop
(88.3 / 2252 / 791) unless noted:

| variant | pct | size | insns | parts sig (row/Canvas/zero/k/hp/j/cc/cm) |
|---|---|---|---|---|
| shipped (base) | **88.3** | 2252 | 791 | r8/r9/r4/r6/r6/sl/r8/fp |
| `KIPlayer *hangarPart` + no cast in `ArrayAdd` | 88.3 | 2252 | 791 | shipped |
| `int count = kBarChildCounts[sceneRace];` before the `j` loop | 88.3 | 2252 | 791 | shipped |
| `int childMesh = kBarChildMeshes[sceneRace] + j;` | 88.3 | 2252 | 791 | shipped |
| `Vector{0, 3.1415927f, 0}` (int zeros) | 88.3 | 2252 | 791 | shipped |
| `PlayerStatic(-1, g, 0, 0, 0)` (int zeros) | 88.3 | 2252 | 791 | shipped |
| both int-zero forms | 88.3 | 2252 | 791 | shipped |
| `PaintCanvas *canvas = Globals::Canvas;` **inside** the loop | 82.2 | 2248 | 791 | r8/—/r4/r5/r5/sl/r8/r9 |
| `PaintCanvas *canvas = Globals::Canvas;` **before** the loop | 82.8 | 2252 | 792 | r8/—/r5/—/r4/sl/r8/r9 |
| `const int *row = kHangarMeshes[sceneRace];` + canvas before | 82.8 | 2252 | 792 | r8/—/r5/—/r4/sl/r8/r9 |
| `const int *row` + canvas inside | 82.2 | 2248 | 791 | r8/—/r4/r5/r5/sl/r8/r9 |
| named `Vector rot{…}` before the loop | 83.9 | 2260 | 791 | r8/fp/—/r6/—/sl/r8/r9 |
| named `Vector rot{…}` inside the loop | 85.4 | 2256 | 790 | r6/r9/—/r0/r6/sl/r8/fp |

Note that hoisting `Globals::Canvas` into a local moves `hangarPart` to `r4`
and the child-mesh temp to `r9` — two of the original's values — but it
*costs* `row` (still `r8`), `zero` (`r5`) and the child count (`r8`), and it
adds an instruction (the extra `Canvas` load), so it drops to 82.2–82.8.

**Conclusion of §4: the parts loop cannot be moved from inside the parts
loop.** Its pre-RA machine code is identical to the original's, so the
decision is made elsewhere (the ship-loop global intervals), exactly as
wave-95 proved.

## 5. The `capacity`-before-`ownHangar` lever reproduces the original parts loop

### 5.1 The all-globals prelude-order sweep

The ship-loop prelude statements are:

| tag | statement |
|---|---|
| A | `bool ownHangar = false; if (getStation()->getIndex()==108) ownHangar = field_114==3;` |
| B | `int capacity = kHangarCapacity[sceneRace];` |
| C | `int shipCount = Globals::rnd->nextInt(capacity + 1);` |
| D | `if (ownHangar) { … ships->size() … }` |
| E | `unsigned char *spotUsed = new unsigned char[capacity]; for(…) spotUsed[i]=0;` |
| F | `const int *spots = sceneRace == 7 ? kHangarSpotsKhador[0] : kHangarSpotsDefault[0];` |

All 66 orders satisfying `A<D, B<C, B<D, C<D, B<E` were built on the
**all-globals** shape (no local `rnd`) and scored. Full table in
`/dev/shm/w5012/prelude_sweep2.json`. The orders that reproduce the
original's **complete** parts-loop signature are exactly the ones with
`B < A`:

| order | pct | size | insns | parts sig |
|---|---|---|---|---|
| **B A C D E F** | **77.2** | 2264 | 806 | **orig (r9/r8/sl/r6/r4/sl/fp/r9)** |
| B A C D F E | 75.7 | 2268 | 808 | orig |
| B E A C D F | 71.9 | 2268 | 805 | orig |
| B E C A D F | 73.0 | 2220 | 787 | orig |
| B E C F A D | 72.3 | 2224 | 789 | orig |
| B E F A C D | 70.4 | 2268 | 805 | orig |
| B E F C A D | 72.3 | 2224 | 789 | orig |
| B F A C D E | 75.6 | 2264 | 806 | orig |
| B F E A C D | 70.3 | 2268 | 805 | orig |
| A B C D E F (shipped order) | 75.0 | 2228 | 787 | all-globals (r8/sl/r9/r6/r4/fp/r9/r8) |

Every `A<B` order gives the all-globals parts loop; every `B<A` order gives
the original's. The mechanism is the one wave-95 §5.3 described: moving the
`kHangarCapacity` load in front of the `getStation()`/`getIndex()` calls
lengthens `capacity`'s interval across two calls, which reorders the
whole-function greedy queue and colours the parts loop exactly as in the
original.

### 5.2 Why it still is not the original source

The original's instruction order is unambiguously **A then B**:

```
b4900: ldr.w r0,[sl]        ; Globals::status
b4904: blx   Status::getStation
b4908: blx   Station::getIndex
b490c: cmp   r0,#108
b490e: bne.n 0xb4922
b4910: ldr.w r0,[sl]
b4914: ldr.w r0,[r0,#276]   ; field_114
b4918: subs  r0,#3
b491a: clz   r0,r0
b491c: lsrs  r4,r0,#5       ; ownHangar
b491e: b.n   0xb4922
b4920: movs  r4,#0
b4922: ldr   r0,[pc,#632]   ; kHangarCapacity
b4926: ldr   r1,[pc,#636]   ; &Globals::rnd
b492a: add   r0,pc
b492c: add   r1,pc
b492e: ldr.w r8,[r0]        ; &Globals::rnd
b4930: ldr   r0,[sp,#52]    ; sceneRace
b4932: ldr.w r6,[r1,r0,lsl #2] ; capacity
b4936: ldr.w r0,[r8]        ; rnd value
b493a: adds  r1,r6,#1
b493c: blx   AERandom::nextInt
```

So `B A C D E F` cannot be the original source: it emits the capacity load
*before* `getStation`/`getIndex`. Measured, it also breaks the rest of the
function:

* `this` is spilled to `sp#48` at index 23 instead of 25 and reloaded twice
  (`ldr r0,[sp,#48]` at 77 and 97) — `this` no longer stays in `r6`;
* `&__stack_chk_guard` moves from `sp#36` to `sp#40`;
* `&Globals::status` is spilled to `sp#44` from the prologue instead of
  living in `sl` until the parts loop;
* total 806 instructions vs the original's 783.

So the original has the **all-globals ship loop** (confirmed by the 8
`ldr.w r0,[r8]` reloads) but the **shipped prologue/status allocation**. The
one source shape that satisfies both halves has not been found.

## 6. The local-`rnd` mask sweep (corrected, exhaustive)

Wave-97's 7-bit sweep had a scoping bug: the literal
`if (rnd->nextInt(100) <= 29)` also appears in `createMission` (line 1838 of
`Level.cpp`), so replacing it globally broke that function for every mask with
bit 0 set. The corrected sweep (`/dev/shm/w5012/sweep_mask4.py`, results in
`mask_sweep4.json`) does all replacements inside the `createScene` body only
and uses the indented `tries`-loop line for site 5 (the unindented
`spot = Globals::rnd->nextInt(capacity);` is a substring of the site-4 line,
which corrupted the first corrected sweep).

Sites (bit order): 0 `if (rnd->nextInt(100) <= 29)`, 1 `race = rnd->nextInt(4)`,
2 `if (rnd->nextInt(100) < 30)`, 3 `pick = rnd->nextInt(3)`,
4 `int spot = rnd->nextInt(capacity)`, 5 `spot = rnd->nextInt(capacity)` in the
`tries` loop, 6 `(float) rnd->nextInt(300)`.

**Result: bit 5 (the `tries` loop) is the flip.**

* every mask **without** bit 5 (64 masks) gives the **all-globals** parts loop
  (`row=r8, Canvas=sl, zero=r9, hangarPart=r4, j=fp, childCount=r9, cm=r8`);
* every mask **with** bit 5 and at least one other site (63 masks) gives the
  **shipped** parts loop (`row=r8, Canvas=r9, zero=r4, k spilled, hangarPart=r6,
  j=sl, childCount=r8, cm=fp`);
* mask `0x20` (bit 5 alone) gives a **third** shape with a larger frame
  (`Vector` at `sp#64`, `hangarPart` at `sp#52`): 66.0 / 2236 / 801.

The shipped source is `0x29` = sites {0,3,5}; the sweep reproduces it exactly
(88.3 / 2252 / 791). Top of the 128-mask table:

| mask | sites (bits) | pct | size | insns | parts sig |
|---|---|---|---|---|---|
| **0x29** (shipped) | 0,3,5 | **88.3** | 2252 | 791 | shipped |
| 0x61 | 0,5,6 | 88.2 | 2252 | 791 | shipped |
| 0x31 | 0,4,5 | 88.1 | 2252 | 791 | shipped |
| 0x68 | 3,5,6 | 87.7 | 2252 | 791 | shipped |
| 0x69 | 0,3,5,6 | 87.7 | 2252 | 791 | shipped |
| 0x70 | 4,5,6 | 87.7 | 2252 | 791 | shipped |
| 0x38 | 3,4,5 | 87.5 | 2252 | 791 | shipped |
| 0x39 | 0,3,4,5 | 87.4 | 2252 | 791 | shipped |
| 0x00 | none (all-globals) | 75.0 | 2228 | 787 | all-globals |

Single-site locals:

| local only at | pct | size | insns | parts sig |
|---|---|---|---|---|
| site 0 | 75.1 | 2232 | 789 | all-globals |
| site 1 | 74.6 | 2232 | 789 | all-globals |
| site 2 | 74.6 | 2232 | 789 | all-globals |
| site 3 | 74.6 | 2232 | 789 | all-globals |
| site 4 | 75.1 | 2232 | 789 | all-globals |
| **site 5** | **66.0** | 2236 | 801 | third shape |
| site 6 | 74.9 | 2232 | 789 | all-globals |

So the wave-97 conclusion (shipped `{0,3,5}` is the best local set) stands, but
the mechanism is now clear: the local's last use at the `tries` site extends
its live interval to the bottom of the ship loop, which is what keeps the
prologue/parts-loop allocation matching the original. No mask produces the
original's parts loop.

## 7. What this pass adds

1. **The parts-loop source is settled.** A from-scratch reconstruction with
   every plausible local/caching/type/`Vector` variation reproduces the same
   72-instruction body; the residue is register allocation only.
2. **The exact RA lever for the original parts loop is identified:** in the
   all-globals shape, `capacity` must be loaded before `ownHangar`. This is
   the first shape since wave-95 §5.3 that reproduces all eight parts-loop
   values, and the sweep shows it is the *only* class of prelude order that
   does.
3. **The contradiction is now explicit:** the original emits `A` before `B`
   (ownHangar then capacity) yet has the parts loop that only `B` before `A`
   produces. The remaining unknown is a ship-loop shape that (a) keeps the
   original's `A`-then-`B` load order, (b) keeps `&Globals::status` in `sl`
   and `this` in `r6` through the prologue, and (c) produces the queue
   reordering that `capacity`-first produces. That is the concrete target for
   the next pass.
4. **The mask sweep is now correct and exhaustive.** The wave-97 conclusion
   (shipped `{0,3,5}` is the best *local* set) stands, and the reason is now
   understood: the local's last use at the `tries` site (bit 5) is what makes
   its live interval reach the bottom of the ship loop and keeps the
   prologue/parts-loop allocation matching. Using the local only at sites
   before the `tries` loop collapses to the all-globals allocation.

## 8. Oracle notes

* The iOS 1.1.4 HD Mach-O (`_work/bins/ios_1.1.4_HD`) has only 1528 symbols
  and **no `Level` symbols** (game code is stripped); it is useless for this
  TU. Its `__const` copies of `kHangarMeshes` / `kHangarCapacity` /
  `kBarChildMeshes` live at file offsets `0x3ebca0` / `0x3ebd40` / `0x3ec180`
  (vmaddr `+0x1000`), which only confirms the table contents.
* The Ghidra decompile (wave-95 `ghidra_cs.c`) confirms the source shape:
  `kHangarMeshes[sceneRace][k]`, `if (-1 < mesh)`, `AEGeometry(mesh,
  Globals::Canvas, false)`, `setRotation(Vector{0, pi, 0})`,
  `PlayerStatic(-1, g, 0, 0, 0)`, the `!plainHangar` child loop with
  `kBarChildCounts[sceneRace]` / `kBarChildMeshes[sceneRace] + j`, and
  `ArrayAdd<KIPlayer*>((KIPlayer *)hangarPart, *this->enemies)`. Every
  `nextInt` site reads `Globals::rnd` directly.

## 9. Reproduction

Scratch is `/dev/shm/w5012`:

* `Level.cpp.base` — pristine `src/game/world/Level.cpp`;
* `fscore.py` — official single-function score (`--diff` for the raw diff);
* `sig.py` — parts-loop register-signature extractor (row / Canvas / zero / k
  / hangarPart / j / child count / child-mesh temp);
* `v.py` + `variants*.py` — the variant harness used in §4/§6/§7;
* `sweep_mask4.py` + `mask_sweep4.json` — the corrected 128-mask `rnd` sweep;
* `sweep_prelude.py` + `prelude_sweep2.json` — the 66-order all-globals
  prelude sweep;
* `ndiff.py` — register/address-normalised instruction-stream diff;
* `liveness2.py` — CFG liveness / live-interval-size approximation over the
  `Expand ISel Pseudo-instructions` machine-IR dump;
* `ra.txt` / `preRA.txt` / `greedy.txt` / `vrr.txt` — clang
  `-mllvm -print-after-all` dumps for the shipped source.

Build recipe:

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
```
