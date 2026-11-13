# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 191b / w13545

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave191b-w13545`, base master `8cffca5f4`. Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base 0x10000).

Continues `level-createmission-wave191b-w13538.md` (case-183 convoy switch
order + case-4 dispatch expect).

```text
                                     size            raw    aligned  insn
base 8cffca5f4                       17172/17172     2628    844     6206/6197
this pass (no new win)               17172/17172     2628    844     6206/6197
```

No size-exact raw win was found; the tree is left at `8cffca5f4`
(`git status --porcelain` empty).  `check` is green: byte_exact 4507/4524,
linked_exact 4507/4524, size 4524/4524, data 843/843, imports 207, lint
CLEAN, drift 0, parity 5370/5370.  Section deltas unchanged: `.text` 71,260,
`.ARM.extab` **576**, `.rodata` 485, `.bss` 2, build-id 20; `.rel.plt`,
`.ARM.exidx`, `.dynsym`, `.dynstr`, `.hash`, `.got` byte-identical.

## 1. Why the case-1 `enemyRace` reload is the only lever that can move raw

The function is byte-identical up to `0xb1608` except two bytes that are
*branch targets* into the shifted region (`0xb02ae` tbh entry, `0xb10c4`
`b.n`).  Everything from `0xb1608` to `0xb178e` is shifted by **-2 B**
because post-RA MachineLICM hoists `ldr r5,[sp,#116]` out of the case-1
attacker loop into the preheader (`0xb158c`), and the assembler then drops
the `nop` at `0xb159a` that the original needs to align the literal pool.
The shift is *compensated* by the case-2 prologue, which is 2 B longer in
ours, so the streams realign at `0xb178e` and the function size stays exact.

Consequence: the ~330 raw bytes in `0xb1608..0xb178e` cannot be reduced by
any local edit — only by moving the load back into the loop body.  All
"case-2 prologue order" measurements in earlier notes are confounded by this
shift (raw is dominated by it); they must be re-measured after the hoist is
fixed.

## 2. Post-RA MachineLICM candidate filter (re-derived, LLVM 7)

`MachineLICMBase::HoistRegionPostRA()` hoists a reload only if
`TII->isLoadFromStackSlot(MI, FI) && MFI->isSpillSlotObjectIndex(FI)`
(the `!HasNonInvariantUse && IsLICMCandidate` branch is dead for FI loads,
because a FI operand sets `HasNonInvariantUse = true`), and then only if:

1. `StoredFIs` does not contain the load's FI (i.e. no store to that FI
   anywhere in the loop), and
2. `!PhysRegClobbers.test(Def) && !TermRegs.test(Def)`, and
3. no register used by the load is in `PhysRegDefs`/`PhysRegClobbers`.

For our MIR the load is `renamable $r5 = t2LDRi12 %stack.75, 0`, `%stack.75`
is an RA spill slot at `[SP-380]` (`[sp,#116]` after the 496-byte frame),
`r5` is defined exactly once in `bb.372`+`bb.376`, the preheader terminator
is `t2B` (no regs) and the load has no register operands.  So every check
passes and it is hoisted.

For the original *not* to hoist, one of the following must hold; none is
reachable from the source shapes tried so far:

* `%stack.75` is not a spill-slot object (front-end stack object), or
* the load's FI is at a non-zero offset (`isLoadFromStackSlot` requires
  `imm == 0`), or
* the loop body stores to the same FI, or
* `r5` is defined twice in the loop / is a live-in of a loop block.

Note `[sp,#116]` is a heavily stack-slot-coloured spill slot (it also holds
the case-183 loop index at `0xb0d34`, the case-2 `enemyRace` at `0xb1794`,
`fp` at `0xb2062`, …), so the "front-end object" hypothesis requires the
object to land at the same offset.

## 3. Case-1 loop-shape sweep (all size 17,172 unless noted)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | 2,628 | 844 | 17,172 |
| `goto`-based do-while (entry jumps to cond) | 2,628 | 844 | 17,172 |
| `while (i < n) { …; i++; }` | 2,628 | 844 | 17,172 |
| `do { …; i++; } while (i < n)` | 10,748 | 1,044 | **17,180** |
| `int race = enemyRace;` / `int type = …` / both | 2,628 | 844 | 17,172 |
| `enemyRace &= 0x7fffffff;` at loop top | 4,915 | **749** | 17,172 |
| `*(volatile int *)&enemyRace = enemyRace;` | 16,487 | 2,360 | **17,240** |
| `int race = *(volatile int *)&enemyRace;` | 16,404 | 2,369 | **17,232** |
| `volatile int *erp = &enemyRace; int race = *erp;` | 16,404 | 2,369 | **17,232** |
| `int enemyRaceArr[1]; *(volatile int*)&arr[0] = …` | 16,404 | 2,369 | **17,232** |
| `int enemyRace;` declared before `localRace` | 2,628 | 844 | 17,172 |
| `unsigned`, `if/else` instead of ternary, unused `volatile int*` | 2,628 | 844 | 17,172 |

The `&=` variant is interesting: it is the only one that gets the *aligned*
count down to 749 (-95) while staying size-exact, but raw explodes to 4,915
because `enemyRace` stops being a pure phi and the pre-switch register
allocation changes (several tbh entries change).  It is a register op, not a
store, so it does **not** stop the hoist (the `ldr` is still at `0xb158c`;
the body gains a 4-byte `bic.w r5,r5,#0x80000000` at `0xb1608`).

The address-taken / volatile forms all grow the frame by **exactly 60 B**
(`17,232`): the front-end object is laid out in the local-object area at a
high `[sp,#…]` offset, so every access needs a 4-byte `ldr.w` (instruction
count becomes 6197/6197 — exactly the original's — but 60 B wider).  Any
future attempt along this line must first find where the original keeps the
object at `[sp,#116]`.

## 4. Tail `this` register: root cause identified (new)

From `0xb322e` the original uses `this = r4`, `alienGuns = sl`; ours uses
`this = r8`, `alienGuns = r6` and additionally loads `[sp,#100]` into `r4`
at `0xb3234`.  `[sp,#100]` is `Globals::rnd` (stored at `0xb01c2`; also
`0xb1e1a` in case 10).  New evidence: in ours that `0xb3234` load is
**dead on both the alienGuns and the pirateFortress paths** (r4 is
overwritten at `0xb32a4` / `0xb3410` before any read), yet it keeps `rnd`
live in `r4` all the way to the raid block, which forces `this` into `r8`.
The original does **not** keep `rnd` in a register: it reloads
`[sp,#100]` at each use (`0xb3788`, `0xb35da`, …).  So the whole ~2.2 KB
tail residual is a single RA live-range decision for the spilled
`Globals::rnd` value, not a `this`-colouring coin flip.

Things that did not flip it (all size 17,172, raw 2,628 unless noted):
`AERandom *rnd = Globals::rnd;` in the raid block; `const int guns`;
`guns` declared first; `hunters`/`opt44` reorder (2,631/838);
`opt44` inlined into `hunterCount` (2,630); `opt44` after `new Array`
(2,630); `int i` in the case-4 loop; `Array<KIPlayer*> *enemies` local in
the alienGuns block (2,393/938, **17,176**); `Level *self`-style locals.

## 5. Alien-gun block: the `(i < 0)` hack is a size crutch, not the original

Original `0xb32c6..0xb32d4` is a plain `ldr r0,[sp,#64]; cbz r0,…; ldr
r1,[pc]; add r1,pc; b …; ldr r1,[pc]; add r1,pc; add r0,r1,r9`, i.e. a
straight `bigAlienWave ? kAlienStationTurrets : kAlienOutpostTurrets`.
Writing the plain ternary in our tree gives **17,168 B** (raw 4,381) and a
completely different (shorter) block; the current
`(bigAlienWave + (i < 0) + (i < 0))` is only there to hold the size at
17,172.  So somewhere else in our tail is 4 B *larger* than the original;
finding that would let the plain ternary (and the `cbz` shape) land.

The binary also interleaves `gun->geometry->setRotation(v)` between the two
`MatrixRotateVector` calls (semantic divergence).  Writing it in binary
order is size-exact at raw **2,638** / aligned **840** (raw loss).

## 6. Case-2 loot block: 4! re-swept with a block-local metric

The real field order in the original is `cargo = nullptr` (`str.w
r9,[r1,#80]`), `crateCaptured() = 0` (`strb.w r9,[r1,#76]`), `setSpeed(0)`
(virtual call, vtable+28), `setRotationSpeed(0)` (PLT call) — i.e. `G C S
R`.  Re-running all 24 permutations and measuring the *loot block only*
(`0xb1930..0xb1970`, which is **not** shifted) gives:

| order | loot-block diff | raw | aligned |
|---|---:|---:|---:|
| **CSGR (current)** | **31** | 2,628 | 844 |
| CGRS / GCRS / GRCS | 32–34 | 2,629–2,633 | 848 |
| GCSR (binary order) | 36 | 2,633 | 840 |
| GSCR / GSRC | 39–40 | 2,632 | 840 |

So the current `CSGR` source is still the best byte shape even though it is
not the original's semantic order.  Do not "fix" the order.

## 7. Sweeps that produced no raw win

Adjacent-statement swaps over `1958–2010`, `2002–2012`, `2010–2070`,
`2120–2160`, `2395–2442`, `1855–1900`, plus stride-2 block moves over
`1958–2010` and `2395–2442`.  Best aligned-only results:

| variant | raw | aligned | size |
|---|---:|---:|---:|
| swap 2019/2020 (`setPosition(spawnPosition)` before `spawnPosition.z +=`) | 2,629 | **814** | 17,172 |
| swap 2127/2128 (`hunterCount` before `ArraySetLength`) | 2,629 | **835** | 17,172 |
| swap 2018/2019 | 2,635 | 828 | 17,172 |
| `enemyRace &= 0x7fffffff` | 4,915 | **749** | 17,172 |

All are raw-neutral-or-worse, so none is landable under the raw gate.

## 8. Next steps

1. Fix the case-1 hoist (section 2) — worth ~330 raw bytes and it unlocks
   every case-2 prologue measurement.
2. Kill the dead `[sp,#100]` (`Globals::rnd`) reload at `0xb3234` — if `rnd`
   stops being live across the alienGuns/pirateFortress blocks, `this` gets
   `r4` and the ~2.2 KB tail collapses.
3. Find the 4 B that our tail is larger than the original; then the plain
   `bigAlienWave ? … : …` ternary (and the `cbz` shape) becomes landable.

## 9. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13545
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)
```

Expected: `raw=2628 aligned=844 size=17172/17172`, `RATCHET PASS`.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13545/`
(`run.sh`, `try.py`, `sbs.py`, `asbs.py`, `regions.py`, `gen_swap*.py`,
`sweep_loot.py`, `v_*.py`, MIR dumps `licm3.txt` / `mir.txt`).
