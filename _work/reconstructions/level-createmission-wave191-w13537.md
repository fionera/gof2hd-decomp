# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 191 / w13537

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave191b-w13537`, base master `23a6ebf8b`. Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base `0x10000`).

Continues `level-createmission-wave191-w13531.md` (case-2 loot store order)
and `...w13530b.md` (post-RA MachineLICM evidence for the case-1 reload).

```text
                                     size            raw    aligned  insn
base 23a6ebf8b                       17172/17172     2656    867     6206/6197
this pass (no new win)               17172/17172     2656    867     6206/6197
```

No size-exact raw win was found.  The tree is left at `23a6ebf8b`.  This note
records the mechanisms that were *ruled out* so the next worker does not have
to re-run them.  Scratch: `/home/debian/GalaxyOnFire2/scratch/w13537/`
(`batch.py`, `probe.py`, `chk.py`, `dis.py`, `v1.py`..`v20.py`,
`decl_sweep.py`, `mixed_sweep.py`).

## 1. Case-1 `enemyRace` reload — the mechanism is now fully pinned down

Original loop body (`0xb1608..0xb1648`, 24 insns):

```
0b1608 ldr  r5,[sp,#116]      <-- reload of enemyRace, first insn of the body
0b160a ldr.w r0,[r8]          ; Globals::globals
0b160e mov  r1,r5
0b1610 blx  Globals::getRandomEnemyFighter(int)
...
0b1642 adds r6,#1             ; i++
0b1644 ldr  r0,[sp,#108]      ; attackerCount
0b1646 cmp  r6,r0
0b1648 blt.n b1608
```

Ours is the same 23 insns with the `ldr r5,[sp,#116]` hoisted into the
preheader (`0xb158c`).  Because the preheader then holds 6 real insns instead
of 5 it needs no alignment `nop` before the literal pool at `0xb159c`, so the
whole case-1 tail is shifted **-2 B** while the total function size is
unchanged (the original spends the 2 bytes on the `nop`).  Fixing this is
worth ~325 raw bytes in `0xb1608..0xb178e`.

### Why it is hoisted (LLVM 7 `MachineLICM::HoistRegionPostRA`)

`ProcessMI` only makes a load a candidate via one of

```cpp
(!HasNonInvariantUse && IsLICMCandidate(*MI)) ||
(TII->isLoadFromStackSlot(*MI, FI) && MFI->isSpillSlotObjectIndex(FI))
```

Our load has an FI operand, so `HasNonInvariantUse` is always true and only the
second arm can apply.  The candidate is then dropped when

* `StoredFIs.count(FI)` — i.e. some instruction in the loop region has an FI
  operand with `mayStore()` (and either no memoperands or a matching
  `FixedStackPseudoSourceValue`), **or**
* `PhysRegClobbers.test(Def)` — `r5` is defined more than once in the loop
  (including via a block **live-in**, which `ProcessMI` treats as an external
  def), **or**
* `TermRegs.test(Def)` — the preheader's first terminator reads/clobbers `r5`,
  **or**
* `Safe == false` — a register used by the load is defined in the loop.

Measured facts for the case-1 loop: `bb.372` (cond) and `bb.376` (body) have
liveins `$r4,$r6,$r8,$r9,$r10` (no `$r5`), `r5` is defined exactly once in the
loop (the load itself), the preheader terminator is a plain `t2B` with no
registers, and no instruction in the loop region has an FI operand with
`mayStore()`.  So every check passes and the load is hoisted.

**Contrast — the case-2 convoy loop is *not* hoisted in either binary.**  Its
body (`bb.389`) loads `%stack.75` (`enemyRace`) at the top too, but `$r4` is
defined twice in that body (`ldr r4,%stack.75` and later `mov r4,r11`), so
`PhysRegClobbers.test(r4)` is set and the load is not a candidate.  That is why
`0xb178e..0xb1933` is byte-identical.  The case-1 difference is therefore a
pure "r5 happens to be defined once" accident, not a source-shape difference.

### Attempts that did NOT move the load (all size 17,172, raw 2,656)

`int race = enemyRace;` at the loop top; `int &race = enemyRace;`;
`int *pRace = &enemyRace;` + `*pRace`; `int race` in the `for`-init;
`for (; ; ++i)`, `i += 1`, `i != attackerCount`, `attackerCount > i`, `while`,
`do/while`, `int i` hoisted out of the `for`; `int type =
getRandomEnemyFighter(enemyRace)` hoisted into a local; `KIPlayer *ship =
createShip(...)`; `Globals *g` / `Array<KIPlayer *> *en` cached locals
(the last two are size-fatal, 17,188 / 17,180).

`enemyRace` wrapped in a local `struct {int pad, race;}` / `int rb[2]` /
`int &rb` all get SROA-promoted back to a scalar (neutral).

`enemyRace += (i >> 31)` (semantically dead, `i >= 0`) is **neutral**, not a
win: clang proves `i >= 0` inside the loop and deletes the update.

### Attempts that force the reload but are size-fatal

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `volatile int enemyRace` | 16,401 | — | 17,232 |
| `int race = *(volatile int *)&enemyRace;` in body | 16,404 | 2,379 | 17,232 |
| `volatile int er = enemyRace;` + `int race = er;` | 16,100 | 1,639 | 17,204 |
| `int er = enemyRace; volatile int *pe = &er;` + `*pe` | 16,100 | 1,639 | 17,204 |
| `volatile int era[1]; era[0]=enemyRace;` + `race = era[0]` | 16,100 | 1,639 | 17,204 |

All five force the reload (and `volatile int enemyRace` even reaches the
original's 6,197 instruction count), but the escaping address turns
`enemyRace` into a real stack object, which perturbs the whole frame and the
global register allocation: the prologue `push` set changes and the function
grows 32–60 B.  Note that a *non-spill* frame object is exactly what would
stop the hoist (`isSpillSlotObjectIndex(FI)` false), so the mechanism is right
— the problem is purely that the frame perturbation costs more than it saves.

**The frame layout is otherwise identical to the original** (prologue
`push {r4,r5,r6,r7,lr}` / `stmdb sp!,{r7,r8,r9,sl,fp}` / `vpush {d8-d14}` /
`sub sp,#400` is byte-identical, and `enemyRace` is `[sp,#116]` in both), so
the original's `enemyRace` is a spill slot at the same offset and one of the
`HoistRegionPostRA` predicates above must have failed in the original build.

## 2. Case-2 loot tail — the binary's store order is `cargo, crate, spd, rot`

Original `0xb1930..0xb1962`:

```
str.w r9,[r1,#80]   ; cargo = nullptr
strb.w r9,[r1,#76]  ; crateCaptured() = 0
... blx setSpeed(0)
... blx setRotationSpeed(0)
```

i.e. the *source* order `cargo, crate, setSpeed, setRotationSpeed`.  The
w13531 landing (`crate, setSpeed, cargo, setRotationSpeed`) is still the best
size-exact raw cell, so the binary's order is not directly reproducible:

| order | raw | aligned |
|---|---:|---:|
| `crate,spd,cargo,rot` (landed) | **2,656** | 867 |
| `crate,cargo,rot,spd` | 2,657 | 871 |
| `cargo,crate,rot,spd` | 2,659 | 871 |
| `cargo,rot,crate,spd` | 2,659 | 871 |
| `crate,spd,rot,cargo` | 2,659 | 864 |
| `cargo,crate,spd,rot` (faithful) | 2,661 | 863 |

The remaining diff in that block is that the original caches
`this->enemies` once (`ldr.w r0,[r5,#248]` … `ldr r0,[r0,#4]` … `add r0,r6`)
and reuses the ship pointer in `r1` for both the `cargo` and `crate` stores,
while ours reloads `[r4,#4]` for every statement.

## 3. Alien-gun block (`0xb322e`) — `this` is spilled into two slots

Byte-identical up to `0xb322e`.  Then:

```
orig: ldr.w sl,[sp,#52]   ; alienGuns
      ldr   r4,[sp,#124]  ; this
      cmp.w sl,#0
ours: ldr   r6,[sp,#52]   ; alienGuns
      ldr.w r8,[sp,#124]  ; this
      ldr   r4,[sp,#100]  ; <-- extra, dead on this path
      cmp   r6,#0
```

The extra load is `bb.226`'s `$r4 = t2LDRi12 %stack.79` (MIR), a redundant
reload of `this` that is also present in `bb.249` (`0xb3398`).  `this` is
spilled into **two** frame objects (`%stack.73`, stored in `bb.1`, and
`%stack.79`, stored in `bb.10`), so the allocator picks `r8` for the
alienGuns path and `r4` for the fortress path.  The original keeps `this` in
`r4` for both and needs only one load.  Everything from `0xb322e` to the
epilogue then uses a permuted register set (`this` r8 vs r4, `alienGuns` r6 vs
sl, `t` r9 vs r6/sl, `gun` r4 vs r6, `i` sl vs fp) — that is where ~2.2 KB of
the residual lives.  This is the single highest-value remaining target.

### Block-order sweeps that were re-measured on this base (all size 17,172)

* alienGuns body `{setRotation, cargo, setPosition, setScaling, shipGroup}`:
  full permutation sweep; best is the current `R,C,P,S,G` = 2,656.  The
  binary's `R,P,S,C,G` is 2,662.  `P` first is size-fatal (17,164).
* fortress body `{setPosition, setScaling, shipGroup, cargo, t+=6}`: current
  `S,C,G,K,T` = 2,656 is best; `S,C,K,G,T` = 2,662, `S,K,C,G,T` = 2,671.
* raid loot body `{setRotate, setSpeed, hitpoints}`: binary order is
  `hitpoints, setSpeed, setRotate`, but `H,S,R` = 2,664 (aligned 857) while the
  current `R,S,H` = 2,656.
* alienGuns `MatrixRotateVector` pair: swapping the two rotates to
  `R(t[0..2]); R(t[3..5]); setRotation(v); cargo` gives raw **2,656** /
  aligned **852** (insn 6,208) — a 15-instruction aligned improvement with
  *zero* raw change.  It is not landed because it is raw-neutral (and it moves
  the divergence from `setRotation` to `setPosition`).  Recorded here in case a
  later pass needs the aligned win.
* `int k` moved into the `for`-init: neutral.  `Vector base` before
  `Matrix rotation`: 2,731.  `k = i - base` recomputed: size-fatal.

## 4. Case-183 `offset` switch

Re-confirmed: emitted block order is `case0, case2, case1, default` while the
literal pool in the binary is `-7000,10000,-30000, 25000,-36000, 6000,12000,
-45000` (= `case1, case2, default`).  Source order `0,1,D,2` (current) and
`0,D,1,2` both give 2,656; `0,1,2,D` / `0,D,2,1` / `0,2,1,D` / `0,2,D,1` give
2,657; anything not starting with `case 0` is ~5,367.  This is
MachineBlockPlacement, not source order.

## 5. Exhaustive adjacent-statement sweeps (no raw < 2,656)

* **402** adjacent same-indent single-line pairs over the whole body
  (lines 1523–2584), including declaration↔statement pairs that the earlier
  `swap_sweep.py` filter excluded: no cell below 2,656.  Best aligned-only
  cell: swapping lines 1969/1970 (the two `MatrixRotateVector` calls) →
  2,656 / **852**.
* **54** adjacent declaration↔declaration pairs: all neutral (2,656) or
  build-fatal.
* Case-2 loot 4! sweep with the `enemies` cache local: `KSCR` (current) is
  best at 2,656.
* `int fc = this->friendCount; for (...)`: 13,190 / 17,180.  `unsigned i` in
  the friendCount loop: 2,658.  `int i` hoisted: neutral.
* Reordering the two case-1 loops (friends loop first): 16,164 / 17,200.
* `Objective *obj = new Objective(7, stations, this); this->objectivesA = obj;`
  and `Level *self = this;` for the case-10 tail: neutral (the `0xb20f6`
  `b.w`+`nop` vs `str`+`b.w` tail-merge is driven by the `this`-in-`r4`-vs-`r8`
  issue of §3, not by the source spelling).

## 6. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13537
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)
```

Expected: `raw=2656 aligned=867 size=17172/17172`, `.rel.plt` diff 0,
`RATCHET PASS` (byte_exact 4507/4524, linked 4507/4524, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0).
