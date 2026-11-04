# `Level::createScene` — wave-98 w4814: the r5 value cache is the whole residue

**Status:** research note, 2026-11-04. Worktree `wt/w4814`, branch `wave98-w4814`,
base `fe4f5e7cf`. **No source change landed** — no source shape found that raises
`byte_exact` or the function score. This commit contains only this note.

Follow-up to `level-createscene-wave95-w4514.md` (which followed
`level-createscene-wave94-w4414.md`, `level-createscene-wave92-w4249.md`,
`level-createscene-wave92-w4213.md`, `level-createscene-wave91-w4113.md`,
`level-createscene-phi-use-w4013.md`, `level-createscene-licm-hoist-w3833.md`,
`Level-createScene-flag-free.md`, `level-createscene-88-3-2252-b_w3813.md`).

## TL;DR

* Target `_ZN5Level11createSceneEv`: **88.3 %**, orig/ours both **2252 B**,
  783/791 instructions, `byte=False`, `linked=False`.
* Gate re-measured this pass, unchanged and green: **byte_exact 4500/4524,
  linked_exact 4500/4524, size-exact 4524/4524, stub_zero_size 0, missing 0,
  wrong_type 0, extra 0, data_exact 843/843, imports 207, lint CLEAN ✓,
  drift 0, RATCHET PASS.**
* **New root-cause result: the entire remaining diff is one register, `r5`.**
  The shipped source's `AbyssEngine::AERandom *rnd = Globals::rnd;` is
  rematerialised by the allocator as a *value cache in `r5`* (one
  `ldr r5,[&Globals::rnd]` in the ship-loop preheader). That single live value
  occupies `r5` for the whole ship loop and forces the rest of the loop into a
  one-step rotation: `race` cannot take `r5` and goes to `r6`, `capacity`
  cannot take `r6` and goes to `fp`, and `&Globals::rnd` is spilled to
  `[sp,#44]` instead of living in `r8`. The original has `r5 = race`,
  `r6 = capacity`, `r8 = &Globals::rnd`, and **no value cache**.
* The shipped source is nevertheless the unique *early-codegen-preserving*
  shape: removing the local `rnd` (the all-globals shape) frees `r5`, which
  the allocator then uses for the `&Globals::status` GOT address in the
  function prologue, so the first 459 instructions stop matching (first diff at
  instruction 8). All-globals is 75.0 % / 2228 B.
* The full 2^7 local-vs-`Globals::rnd` site matrix was re-run on the current
  base (128 builds). The maximum is **88.3 %**, attained only by the shipped
  set `{0,3,5}` and by its `{0,3,4}` variant (swap the two `spot` sites).
  Every other subset is lower. §4.
* ~150 further source shapes were measured this pass (parts-loop statement
  order, Vector construction/assignment order, `spots` form, `spotUsed` type,
  loop forms, types, casts, array indexing, `rnd` declaration placement,
  `ownHangar` forms, `shipCount` placement). **None beats 88.3 %.** §5.
* The parts loop itself is a hard local optimum: its pre-RA machine code is
  unchanged by every semantically-neutral variant tried, and its allocation is
  decided by the *ship*-loop `r5` interval (wave-95 §3). §6.

## 1. What the diff is (current base)

`tools/verify/verify.py --show _ZN5Level11createSceneEv`: the first 459
instructions are byte-identical. The first diff is instruction 460, the
parts-loop preheader. The instruction *stream* is identical apart from one
spill (`str r6,[sp,#44]`) and the `ldr`/`ldrd` pair after `ArrayAdd`.

Parts loop (`for (int k = 0; k < 4; k++)`):

| value | original | shipped |
|---|---|---|
| row `kHangarMeshes[sceneRace]` | **`r9`** | `r8` |
| `Globals::Canvas` value | **`r8`** | `r9` |
| zero of `Vector{0,pi,0}` | **`sl`** | `r4` |
| `k` | **`r6`** | `r6` (spilled `sp#44` around `setRotation`) |
| `hangarPart` | **`r4`** (spilled `sp#44` after the ctor) | `r6` |
| `g` | `r5` | `r5` |
| `j` | **`sl`** | `sl` |
| child count `kBarChildCounts[sceneRace]` | **`fp`** | `r8` |

Ship loop:

| value | original | shipped |
|---|---|---|
| `&Globals::rnd` | **`r8`** (value reloaded at every site) | `[sp,#44]` (value cached in `r5`) |
| `capacity` | **`r6`** | `fp` |
| `race` | **`r5`** | `r6` |
| `type` | **`fp`** | `r8` |
| `i` | **`r9`** | `r9` |
| `&Globals::status` | **`sl`** | `sl` |
| `spotUsed` | `r4` | `r4` |

`i`, `&Globals::status`, `spotUsed`, `g`, `j`, `k` are already right. The
wrong ones are exactly the `r5`-cache cascade.

## 2. The `r5` value cache is the cause

Shipped ship-loop preheader (ours, disassembly):

```
543 movs  r4,#0
544 ldr   r0,[pc,#640]      ; &Globals::rnd GOT
545 ldr   r1,[pc,#640]      ; kHangarCapacity GOT
546 add   r0,pc
547 add   r1,pc
548 ldr   r2,[r0,#0]        ; &Globals::rnd -> r2
549 ldr   r0,[sp,#52]       ; sceneRace
550 str   r2,[sp,#44]       ; spill &Globals::rnd
551 ldr.w fp,[r1,r0,lsl#2]  ; capacity -> fp
552 ldr   r0,[r2,#0]        ; *rnd
553 add.w r1,fp,#1
554 blx   nextInt
...
597 ldr   r0,[sp,#44]       ; &Globals::rnd
598 ldr   r5,[r0,#0]        ; rnd value -> r5   <-- the cache
```

Original:

```
543 movs  r4,#0
544 ldr   r0,[pc,#632]
545 ldr   r1,[pc,#636]
546 add   r0,pc
547 add   r1,pc
548 ldr.w r8,[r0]           ; &Globals::rnd -> r8
549 ldr   r0,[sp,#52]
550 ldr.w r6,[r1,r0,lsl#2]  ; capacity -> r6
551 ldr.w r0,[r8]           ; *rnd
552 adds  r1,r6,#1
553 blx   nextInt
```

The `ldr r5` at our 598 is the only extra live value. Because `r5` is
callee-saved and live for the whole ship loop, the greedy allocator rotates
`race→r6`, `capacity→fp`, `type→r8`, `&rnd→sp#44`. The original has no such
cache (`r5 = race`).

## 3. Why the local `rnd` cannot simply be removed

The all-globals shape (`Globals::rnd->` at all seven sites, no local `rnd`) is
75.0 % / 2228 B. It fixes the ship loop (`&rnd` in a callee-saved register,
`race→r5`) but changes the function prologue: with `r5` free, the allocator
uses it for the `&Globals::status` GOT address, so instruction 8 becomes
`ldr r5,[r0,#0]` instead of `ldr r0,[r0,#0]`, and every later instruction
shifts. The first diff moves from 460 to 8.

So the original must both (a) keep `r5` reserved in the prologue and (b) not
cache the `rnd` value in the ship loop. The shipped local `rnd` does (a) but
not (b); the all-globals shape does (b) but not (a). Every source form tried
that separates the two (declare `rnd` early and use it only for `shipCount`,
`AERandom *&rnd = Globals::rnd`, `volatile`, `const`, `auto`, use it at 1..7
sites) lands between 63.6 % and 88.3 %.

## 4. The full local-vs-`Globals::rnd` matrix (128 builds)

Sites, in source order:

```
0  if (rnd->nextInt(100) <= 29)                 [shipped: local]
1  race = Globals::rnd->nextInt(4);             [global]
2  if (Globals::rnd->nextInt(100) < 30)         [global]
3  int pick = rnd->nextInt(3);                  [local]
4  int spot = Globals::rnd->nextInt(capacity);  [global]
5  spot = rnd->nextInt(capacity);               [local]
6  (float) Globals::rnd->nextInt(300)           [global]
```

All 128 subsets of "use the local `rnd` at site i" were built and scored.
Maximum **88.3 %**, at:

* `{0,3,5}` (the shipped source), and
* `{0,3,4}` (swap sites 4 and 5; same 88.3 %, same 2252 B, different code).

Everything else: `{0,3,5,6}` 87.7, `{0,3}` 88.2, `{0,3,4,6}` 88.2, `{0,3,5}`
minus any one 83.6–87.7, all-global 75.0, all-local 83.7 / 2236.

## 5. Other source shapes measured this pass (all ≤ 88.3 %)

Parts loop: `k != 4 / ++k / k += 1`; `mesh == -1`; `if (mesh >= 0) { … }`;
`while`/`do-while` forms; `g`/`hangarPart`/`mesh`/`j`/`child`/`count` hoisted
out of the loop; `int n = kBarChildCounts[sceneRace]`; `ArrayAdd<KIPlayer*>`;
`ArrayAdd((KIPlayer*) …)`; `plainHangar` as `int`/negated/inlined; flat array
indexing `((const int*)kHangarMeshes)[sceneRace*4+k]`; `row` pointer hoist;
`Vector` brace/paren/default-then-assign; all six assignment orders of
`rot.x/y/z`; `rot` named inside/outside the loop; `PlayerStatic` args as
`0` vs `0.0f`; `-0.0f`; `3.14159265f` vs `3.1415927f`.

Ship loop: explicit `if`/`phi`/`else` for `spots` (86.5); `&kHangarSpots[0][0]`
and `(const int*)` casts (86.5); `switch` → if-chain (74.3, 2260 B); `table`
hoisted with `default: break`; `pick` as if-chain; `<= 29` vs `< 30`;
`tries > 0`; `spotUsed[spot]` vs `!= 0`; `spotUsed = true`; `bool`/`char`/
`uint8_t` spotUsed; `unsigned` for `k/j/i/race/type/spot/tries/pick/capacity/
shipCount`; `const`/`auto`/`volatile`/reference forms of `rnd`; `rnd`
declared before `spotUsed`/before `shipCount`/after `spots`/inside the loop;
`spots` inside the loop (LICM-hoisted, 88.3); `setPosition` split into locals;
rotation angle split; `setAlwaysFriend`/`setToSleep`/`setExhaustVisible`
swapped; `100.0f` vs `100.0`; casts removed.

Prologue: `capacity` before `ownHangar` (79.8, 2276 B); `spotUsed` before
`ownHangar`/before `shipCount`/after `ownHangar`; `ownHangar` as one
expression/`int`/swapped conditions; `ships` `const`; `shipCount` before
`ownHangar` (82.8); `sceneRace` block braces removed.

None moves the parts-loop signature from `(r8, r9)` to the original
`(r9, r8)`.

## 6. The allocator mechanism (for the next pass)

The greedy allocator's queue is ordered by `Prio` (LLVM 7
`RegAllocGreedy::enqueue`): multi-MBB ("global") ranges get
`(1<<31)|(1<<29)|Size`, boosted by `(1<<30)` when the range has a physical
hint; one-MBB ranges get `InstrDistance(begin, LastIndex)` instead of `Size`.
Larger globals are allocated first.

In the shipped source the parts-loop values are all multi-MBB and their
priorities are:

```
%15  capacity       size 5150  hinted  0xE000141E   -> r6 (later evicted to r11/r8)
%471 k (PHI)        size 4810  hinted  0xE00012CA
%440 Vector address size 1886  hinted  0xE000075E   -> r4
%7   g              size 1390  hinted  0xE000056E   -> r5
%8   hangarPart     size 1056  hinted  0xE0000420   -> r6
%270 zero           size 3134  no hint 0xA0000C3E   -> r9 then r4
%431 row            size 1934  no hint 0xA000078E   -> r8
%9   child count    size  718  no hint 0xA00002CE   -> r8
%6   mesh           size  224  no hint 0xA00000E0   -> r4
```

`%440` is the `t2ADDri %stack.1, 0` for the `Vector` temporary. It is hinted
(`$r1 = COPY %440` before `setRotation`) and, because it is hoisted to the
parts-loop preheader, its interval spans the whole loop (size 1886). It is
therefore allocated **before** `hangarPart` (size 1056) and takes `r4`. The
original evidently has `hangarPart` taking `r4` first (it is then spilled to
`[sp,#44]` right after the `PlayerStatic` ctor), which frees `r6` for `k` and
lets `capacity` keep `r6`.

The same `r5` cascade is visible in the ship loop: `%270`-style value caches
and the `%440`-style hinted hoisted address are the two levers. The concrete
remaining target is a source shape where **`hangarPart` outranks `%440`** (or
`%440` is not a global hinted range), while `r5` stays reserved in the
prologue.

## 7. Scratch artefacts (not committed)

`/dev/shm/w4814/`:

* `Level.cpp.base` — pristine `src/game/world/Level.cpp`.
* `build.sh`, `score.py` — link + single-function score.
* `sig.py` — parts-loop / ship-loop register signature.
* `dual.py A B [o|u]` — aligned original/ours disassembly slice.
* `matrix.py`, `matrix_results.txt` — the 128-build local/global matrix.
* `sweepA..Z.py` — the ~150 source-shape measurements of §5.
* `ghidra_cs.c` — Ghidra decompile of `Level::createScene` (`0xc42ec`).
* `dumps_base.txt` — clang `-mllvm -print-after-all` for the shipped source;
  pre-RA machine code at line 118843, post-LICM at 133179, post-RA at 173709.
* `regalloc_base_g0.log` — `llc -O2` regalloc debug log (note: this llc model
  is *not* faithful to the real `-Oz` build; it spills `row`/`hangarPart` and
  is only used for the priority arithmetic above).
