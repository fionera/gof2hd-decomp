# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 191b / w13625

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave191b-w13625`, base master `de5b169db`. Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base 0x10000).

Continues `level-createmission-wave191b-w13601.md` (SpillPlacement diagnosis,
2626 raw), `...w13589.md`, `...w13560.md`, `...w13567.md`, `...w13545.md`.

```text
                                     size            raw    aligned  insn
base de5b169db                       17172/17172     2626    845     6206/6197
this pass (no new win)               17172/17172     2626    845     6206/6197
```

No size-exact raw win was found; the tree is left at `de5b169db`
(`git status --porcelain` empty). `.ARM.extab` unchanged (577 differing bytes,
size/base exact at `0x26298`).

## 1. CORRECTION: the case-1 `enemyRace` reload is hoisted by **post-RA
##    MachineLICM**, not by greedy-RA SpillPlacement

w13601 concluded "the reload is already in the preheader after greedy RA; the
decision is SpillPlacement". That is wrong: it read the *post-LICM* dump.
Walk the pass dumps in `-mllvm -print-machineinstrs` order for the case-1
first loop (header `bb.372` = condition at `0xb1644`, body `bb.376` = `0xb1608`):

```text
After Virtual Register Rewriter (334940)   bb.376: renamable $r5 = t2LDRi12 %stack.109  (body)
After Stack Slot Coloring        (347087)   bb.376: renamable $r5 = t2LDRi12 %stack.75   (body)
After Machine Copy Propagation   (359234)   bb.376: renamable $r5 = t2LDRi12 %stack.75   (body)
After Machine Loop Invariant CM  (371320)   bb.371: renamable $r5 = t2LDRi12 %stack.75   (PREHEADER)
After PostRA Machine Sink        (383406)   bb.371: renamable $r5 = t2LDRi12 %stack.75   (PREHEADER)
```

So greedy RA puts the reload **in the loop body** (as the original does) and
`PostRAMachineLICM` moves it to the preheader. Confirmed by a flag build:
`-mllvm -disable-postra-machine-licm` puts the load at `0xb1610` (body) instead
of `0xb158c` (preheader).

`MachineLICMBase::HoistRegionPostRA` (LLVM 7.0.2 `lib/CodeGen/MachineLICM.cpp`)
is **purely structural** — it is *not* a register-pressure heuristic:

1. the frame index must not be stored anywhere in the loop (`StoredFIs`),
2. the def physreg must not be defined/clobbered elsewhere in the loop
   (`PhysRegClobbers`) and must not be read by the preheader terminator
   (`TermRegs`),
3. every use of the candidate must be loop-invariant,
4. the load must be `TII->isLoadFromStackSlot` **and**
   `MFI->isSpillSlotObjectIndex(FI)` (the `HasNonInvariantUse` flag is always
   set for an FI operand, so the stack-slot disjunct is the only one that can
   fire for a reload).

In our build all four hold (body liveins are `$r4,$r6,$r8,$r9,$r10` — r5 is
*not* live-in, so the `ldr r5` def is not a clobber), hence the hoist. For the
original not to hoist, one of (1)-(4) must have been false in its IR. Since the
loop body is byte-identical from `0xb160a` on, the only plausible lever is the
frame-index classification (a front-end stack object is not
`isSpillSlotObjectIndex`) or r5 being clobbered in the loop. The `[sp,#116]`
slot is reused for many unrelated values in both binaries, which argues it is a
plain RA spill slot in the original too — so this remains unexplained.

## 2. `%698` (`&Globals::rnd`) and the whole tail are one greedy-RA decision

`%698` is the single CSE'd `tLDRLIT_ga_pcrel @_ZN7Globals3rndE` + GOT load,
live from source line 2146 to 2539 (every `Globals::rnd` use in the `else`
branch). Ours keeps it in `r4` across the alienGuns / pirateFortress / raid
tail; the original spills it and reloads `[sp,#100]` only at `0xb3788`, which
is why `this` gets `r4` there. Every tail diff is a downstream consequence:

* block order differs at `0xb322e` (orig `ldr.w sl,[sp,#52]` = a block that
  ours lays out elsewhere — `sl` is not even used for that value in ours);
* the remaining tail diffs are pure register renames, e.g.
  `0xb2ce8..0xb2d04` orig `ldr r1,[sp,#104] / adds r1,#1 / cmp r1,r0` vs ours
  `ldr r6,[sp,#104] / adds r6,#1 / cmp r6,r0`, and the `str r1,[sp,#104]` /
  `str r6,[sp,#104]` position swap at `0xb2aa6`/`0xb2ab0`;
* `0xb20f6` orig stores `objectivesA` inline (`str r5,[r4,#40]`) and branches
  to `0xb02ca`; ours branches to a shared block at `0xb41e8` with a `nop`.

Diff-run totals: case-1 region `0xb158c..0xb1790` = 341 raw bytes; tail
`0xb322e..0xb42ec` = 2,204 raw bytes; everything else = 81 bytes.

## 3. Measurements (all from base `2626 / 845 / 17172`)

### 3a. Per-TU `-mllvm` flags (Level.cpp already carries
###     `-enable-earlycse-memssa=false`)

| flag added | raw | aligned | size | insn |
|---|---:|---:|---:|---:|
| (none) | **2,626** | **845** | 17,172 | 6206/6197 |
| `-disable-postra-machine-licm` | 15,797 | 1,165 | **17,188** | 6212/6197 |
| `-disable-postra-machine-sink` | 2,634 | 849 | 17,172 | 6206/6197 |
| `-hoist-cheap-insts` | 3,310 | 897 | 17,172 | 6206/6197 |
| `-avoid-speculation=false` | 2,626 | 845 | 17,172 | 6206/6197 |
| `-hoist-const-stores=false` | 2,626 | 845 | 17,172 | 6206/6197 |
| `-disable-machine-licm` (pre-RA) | 16,121 | 4,485 | 17,100 | 6159/6197 |
| `-sink-insts-to-avoid-spills=true` | 16,440 | 2,878 | 17,204 | 6205/6197 |

Disabling post-RA LICM moves the load into the body (as the original) but
re-allocates the entire function, so it is a large net loss.

### 3b. `__builtin_expect` on the loop conditions (new this pass)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| case-1 loop `expect(i < attackerCount, 1)` | 16,500 | 2,483 | 17,304 |
| case-1 loop `expect(i < attackerCount, 0)` | 16,458 | 1,749 | 17,204 |
| case-1 second loop `expect(..., 1)` | 15,433 | 1,343 | 17,124 |
| case-2 loop `expect(i < convoyEnd, 1)` | 16,811 | 3,039 | 17,364 |
| case-2 loop `expect(i < convoyEnd, 0)` | 15,893 | 1,946 | 17,124 |

The expect metadata re-runs block placement for the whole function; all
size-breaking.

### 3c. Source shapes (all size-exact `17,172` unless noted)

| variant | raw | aligned |
|---|---:|---:|
| base | **2,626** | **845** |
| `ArraySetLength(friends + 3 + attackerCount, ...)` | 2,626 | 845 |
| `int n = attackerCount; for (i < n)` | 2,626 | 845 |
| `for (attackerCount > i)` | 2,626 | 845 |
| `++i` / `i != attackerCount` | 2,626 | 845 / 847 |
| fighter hoisted to a local before `createShip` | 2,626 | 845 |
| `(int) enemyRace` on both uses | 2,626 | 845 |
| `float attackers` declared before `int friends` | 2,626 | 845 |
| `Array *en = new ...; this->enemies = en;` | 2,626 | 845 |
| `int &er = enemyRace;` in the loop | 2,626 | 845 |
| `int *erp = &enemyRace;` + `*erp` in the loop | 2,626 | 845 |
| `const int er = enemyRace;` in the loop | 2,626 | 845 |
| `int er = enemyRace;` in the preheader | 2,626 | 845 |
| `AERandom *rnd = Globals::rnd;` before `friends` | 2,626 | 845 |
| loop cond `&& enemyRace != -12345` | 15,523 | 1,514 (size 17,140) |
| loop bound `attackerCount + (enemyRace >> 31)` | 15,428 | 1,161 (size 17,188) |

SROA folds every reference/pointer/copy spelling back to the same SSA value, so
none of them changes the LICM input.

## 4. Next steps

1. The case-1 lever is `MachineLICMBase::HoistRegionPostRA`, not
   SpillPlacement. Something must make `isSpillSlotObjectIndex(FI)` false or
   make `PhysRegClobbers.test(r5)` true in the original. A front-end stack
   object for `enemyRace` (address-taken in a way SROA cannot promote) is the
   only untested shape that would do the first without changing the emitted
   `str/ldr [sp,#116]` pair. `volatile` does move the load into the body and
   restores the original instruction count (6197/6197) but costs +60 B
   (size 17,232) and re-allocates the whole function (raw 16,406).
2. The tail (`%698` spill) is a single greedy-RA live-range decision over the
   whole `else` branch (source lines 2146-2539); declaration reorders and
   reference forms are all neutral (w13560/67/78/89/601).
3. A size-exact case-2/CSGR reorder is still open; w13601 measured
   `asteroidPos`-first at 3,230 and `goods`-after-`new Route` at 3,228/2,645.

## 5. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13625
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
```

Expected: `raw=2626 aligned=845 size=17172/17172 insn=6206/6197`.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13625/`
(`mir0.txt` 58 MB `-print-machineinstrs` dump, `orig.dis`, `ours.dis`,
`harness.py`, `measure.sh`, `flagsweep.py`, `b1.py`..`b4.py`, `regions.py`).
