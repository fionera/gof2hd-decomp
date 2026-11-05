# w6764: `Level::createMission` (92.293729 -> 92.293729 %, 17172 B) — wave-119:
# Stack-Slot-Coloring weight audit (raid vs `&__stack_chk_guard`), case-183/`guns`/
# turret/`&&` re-sweeps; no source win

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w6764`, branch `wave119-w6764`,
based on current master (`c8a402ec2`).  Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; function `0xaffd8`-`0xb42ec`, 17172 B).

## Baseline (pristine HEAD, wave-118 `volatile unsigned n` latch)

```text
_ZN5Level13createMissionEv   official 92.293729 %   size 17172/17172 B
                             6065 ours / 6055 orig insns
gate: byte 4505/4524, linked 4505/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
ELF: 27 sections, file 2,187,328 B, .ARM.extab 0x26290 (8 B short, createShip only).
```

## 1. Result

No source variant beat the baseline.  Best measured score remains
**92.293729 % / 17172 B / 6065 insns**.  `src/game/world/Level.cpp` is unchanged
(this note is the only commit).

## 2. New result: the swapped spill slots are `raid` and `&__stack_chk_guard`

The w6737 note called the second slot `&Globals::rnd`; that is wrong.  The MIR
prologue (`mir_vrr.txt`, bb.0) is unambiguous:

```text
 32B  $r0 = tLDRLIT_ga_pcrel @__stack_chk_guard
 48B  $r1 = tLDRLIT_ga_pcrel @_ZN7Globals6statusE
 64B  $r11 = ldr [$r0]                 ; r11 = &__stack_chk_guard  (fp)
 96B  $r1  = ldr [$r11]                ; __stack_chk_guard
128B  str   $r1, %stack.0              ; canary slot (sp+396)
...
532B  str   $r11, %stack.74            ; spill &__stack_chk_guard
170920B $r11 = ldr %stack.74           ; reload in the shared return block
```

So the two 4-byte slots that swap are:

| slot | original | ours |
|---|---|---|
| `raid` (VRR `%stack.86`, 13 stores + 4 loads) | sp+84 | sp+88 |
| `&__stack_chk_guard` (VRR `%stack.74`, 2 refs) | sp+88 | sp+84 |

`Globals::rnd`'s address is a *different* slot (`%stack.138`, sp+120 in both).

## 3. Stack-Slot-Coloring weights: the two slots are within 0.35 %

`StackSlotColoring::ScanForSpillSlotRefs` sets
`li.weight += LiveIntervals::getSpillWeight(false, true, MBFI, MI)`, i.e. the
sum of the *relative block frequencies* of every non-debug FI reference;
`InitializeSlots` then `stable_sort`s by weight (descending) and `ColorSlot`
hands out colours in that order.  So the colour (and hence the final sp offset)
of two slots is decided purely by that weight.

I solved the MIR CFG frequencies exactly (537 machine blocks, successor
probabilities from the `successors:` comments, Tarjan SCCs + per-SCC linear
solve — the plain forward iteration diverges on the probability-1.0 back
edges).  The result reproduces the IR `print<block-freq>` values for the early
blocks (bb.0 = 1.0, bb.1 = 0.625, bb.6 = 10.0, ...), so the model is sound.

Weights of the two slots in the current build:

```text
%stack.86  raid                  weight 1.197954   (19 refs, 19 blocks)
%stack.74  &__stack_chk_guard    weight 1.193761   (2 refs: bb.1 0.625 + bb.519 0.56876)
                                          delta 0.004193  (0.35 %)
```

`raid` therefore wins by a hair and takes the lower colour.  The original must
have `guard.weight > raid.weight`; because the gap is only 0.35 %, *any*
small difference in the branch-probability/loop-scale estimate flips it.  The
`raid` weight is spread over 19 cold blocks (largest single contribution
0.156), while the guard weight is `freq(bb.1) + freq(shared return block)`.

This also explains why declaration/type/`volatile` changes on `raid` never
moved the slot: they change the *code*, not the frequency distribution.

Measured value of the swap (simulated by rewriting the offsets in the
normalised instruction streams, so it is an upper bound for any real fix):

| offset remap applied to ours | official % |
|---|---|
| none (baseline) | 92.293729 |
| 84 <-> 88 | **92.607261** |
| 84 <-> 88 plus 44->40, 40->48, 48->44 | 92.623762 |
| the above plus 104 <-> 92 | 92.277228 (over-applied) |

## 4. Diff census (baseline)

```text
difflib ratio 92.293729 %, 620 opcodes, 484 diff elements
  97 / 484 diff elements are *pure register* differences (same opcode, same
  operands except rN)
  31 single-instruction diffs are *pure sp-offset* differences
  region buckets (orig insn index -> diff elements):
    4500-4749: 111   (alien-turret loop)
    5000-5249: 110   (raid loop)
    5250-5499:  54   (raid loop tail)
    5750-5999:  45   (mission-type tail)
    4250-4499:  32
    4750-4999:  28
```

The dominant residual is still the register-allocator permutation that starts
at the alien-turret preheader (`this` r4 orig / r5 ours, `gun` r6 / r4,
`&return-slot` r5 / r8, `k*24` r9 / fp, `i` fp / r9, `t` sl / r6), plus the
`setScaling`-through-fresh-`(*enemies)[i]` hack that the current best shape
needs (the original really uses the cached `gun`: `ldr r1,[pc]; mov r0,r6;
blx setScaling` at orig 4649-4651).

## 5. Variants measured this wave (official verifier)

All are `official % / size / insns`; baseline `92.293729 / 17172 / 6065`.
Every variant keeps 27 sections, file size 2,187,328 B and `.ARM.extab`
0x26290.

### 5.1 `guns` (raid-loop start) shapes

| variant | official % | size | insns |
|---|---|---|---|
| **baseline `int guns = alienGuns + fortressGuns;`** | **92.293729** | 17172 | 6065 |
| `int guns = alienGuns; guns += fortressGuns;` | 92.293729 | 17172 | 6065 |
| `int guns = fortressGuns + alienGuns;` | 92.293729 | 17172 | 6065 |
| inline `(alienGuns + fortressGuns)` at every use (no `guns` local) | 88.822899 | 17200 | 6068 |

### 5.2 case-183 convoy `offset[]` chain

| variant | official % | size | insns |
|---|---|---|---|
| **baseline `if (k==2) / else if (k==1) / else if (k==0) / else`** | **92.293729** | 17172 | 6065 |
| nested negated `if (k!=2) { if (k!=1) { if (k!=0) ... } }` | 92.293729 | 17172 | 6065 |
| `switch (k)` with `default:` first, cases 2,1,0 | 92.293729 | 17172 | 6065 |
| `switch (k)` with cases 0,1,2, default last | 92.145215 | 17172 | 6065 |
| `if (k==0) / else if (k==1) / else if (k==2) / else` | 92.145215 | 17172 | 6065 |
| three nested ternaries (one per component) | 89.281003 | 17188 | 6073 |

### 5.3 alien-turret `base` / preheader shapes

| variant | official % | size | insns |
|---|---|---|---|
| **baseline** | **92.293729** | 17172 | 6065 |
| `int baseIndex = this->friendCount + this->field_164;` hoisted above the `if` | 92.293729 | 17172 | 6065 |
| `Array<KIPlayer*> *en = this->enemies;` local for `base` | 92.293729 | 17172 | 6065 |
| `Vector base` computed before `Matrix rotation` | 92.062706 | 17172 | 6065 |
| `Vector alienBase` hoisted above the `if (alienGuns != 0)` | 81.501650 | 17180 | 6065 |

### 5.4 `setScaling` placement (turret loop)

| variant | official % | size | insns |
|---|---|---|---|
| **baseline fresh `(*enemies)[i]`, between `setRotation` and 2nd rotate** | **92.293729** | 17172 | 6065 |
| fresh, before the first rotate | 92.277228 | 17172 | 6065 |
| fresh, at the end of the body | 92.244224 | 17172 | 6065 |
| cached `gun`, at the end (faithful) | 92.176927 | 17172 | 6063 |
| cached `gun`, between `setRotation` and 2nd rotate | 90.922594 | 17164 | 6063 |

### 5.5 raid block (`hard`, `raidType`, `raid`)

| variant | official % | size | insns |
|---|---|---|---|
| **baseline** | **92.293729** | 17172 | 6065 |
| `Level::initStreamOutPosition && raidRace == 8` (swapped) | 92.293729 | 17172 | 6065 |
| `const bool hard` | 92.293729 | 17172 | 6065 |
| `raid` declared `int` | 92.128713 | 17172 | 6065 |
| `bool raid = false;` + negated `if` | 92.293729 | 17172 | 6065 |
| `volatile bool raid` | 82.483077 | 17196 | 6059 |
| `hardCoreMode()` inlined into the condition | 91.354562 | 17192 | 6067 |
| `bool hard` declared before `int raidType` | 92.277228 | 17172 | 6065 |
| pirate-base `raid = false;` moved to the top of the block | 92.293729 | 17172 | 6065 |
| supernova `raid = false;` moved to the top of the block | 92.293729 | 17172 | 6065 |

### 5.6 `&&` at line 1904 (`i == friendCount + field_164 && alienWave`)

| variant | official % | size | insns |
|---|---|---|---|
| **baseline** | **92.293729** | 17172 | 6065 |
| `(i == X) & (alienWave != 0)` | 92.293729 | 17172 | 6065 |
| `alienWave && i == X` (swapped) | 92.293729 | 17172 | 6065 |
| `!(i != X \| alienWave == 0)` | 92.062706 | 17172 | 6065 |
| `!(i != X) & !(alienWave == 0)` | 92.062706 | 17172 | 6065 |
| `(i == X) * alienWave` | 92.062706 | 17172 | 6065 |
| `!(i != X \| !alienWave)` | 82.981182 | 17168 | 6061 |
| `(i == X) & alienWave` (int operand) | 75.689058 | 17172 | 6063 |

### 5.7 early mission-setup spellings (`security`, `stationPirates`, ...)

| variant | official % | size | insns |
|---|---|---|---|
| **baseline** | **92.293729** | 17172 | 6065 |
| `if (raid & (security == 3))` | 92.293729 | 17172 | 6065 |
| `if ((security == 3) & (int) raid)` | 92.293729 | 17172 | 6065 |
| `int stationPirates` instead of `unsigned` | 92.293729 | 17172 | 6065 |
| parenthesised `raid && (...) && (...)` | 92.293729 | 17172 | 6065 |
| `if (security == 3 && raid)` | 87.262828 | 17196 | 6067 |
| `pirateEscorts` as a conditional `int` | 87.430882 | 17180 | 6062 |

### 5.8 loop-trip-count probes (branch-probability perturbation)

`for (i = 1; i < 6; i++)` -> `i <= 5`, `for (i = 0; i < 8; i++)` -> `i <= 7`,
`for (k = 0; k < 4; k++)` -> `k <= 3` all produce byte-identical output
(92.293729 / 17172 / 6065), i.e. the loop-scale estimates are unchanged.

### 5.9 frequency probe

Deleting the supernova `raid = false;` (semantically wrong, probe only)
changes the whole allocation: 84.629354 % / 17164 / 6059, and `raid` leaves
sp+84 entirely.  This confirms the SSC weight is the lever, but the store is
present in the original (`str r1,[sp,#84]` at orig insn 850), so it cannot be
dropped.

## 6. Residual analysis (unchanged)

* `this` r4 (orig) vs r5 (ours) from the alien-turret preheader onward;
  `gun` r6 vs r4; `&return-slot` r5 vs r8; `k*24` r9 vs fp; `i` fp vs r9;
  `t` sl vs r6.  Dominant residual (~2.7 points).
* Stack-slot swap `raid` (orig sp+84 / ours sp+88) vs `&__stack_chk_guard`
  (orig sp+88 / ours sp+84); worth +0.3135 pp if fixed.
* Secondary slot cycle 40/44/48 (`stationPirates`, `blueprint`, ...) worth
  a further +0.0165 pp.
* Turret selector branch (orig `ldr r0,[sp,#64]; cbz`) vs if-converted
  select (ours).
* Case-183 default body inline+shared store (orig) vs out-of-line (ours);
  first size divergence at `0x144e`.
* `&&` at 1904: orig materialises `(i != X) | !alienWave` (`subs/mov/it/movne/
  eor/orrs/bne`), ours short-circuits.
* `*bigShipsCount > 0`: orig `cmp #0; ble`, ours `cmp #1; blt`.
* `.ARM.extab` still 8 B short (`0x26290` vs `0x26298`, createShip only);
  27 sections and `2,187,328 B` file size unchanged by every variant above.

## 7. Next step

The only mechanical lever left is the 0.35 % weight gap between
`%stack.86` (`raid`) and `%stack.74` (`&__stack_chk_guard`).  Because the gap
is tiny, the fix is not "make raid colder" but "reproduce the original's
branch-probability/loop-scale estimate".  A useful experiment for the next
worker: dump `MachineBlockFrequencyInfo` from an assertions-enabled LLVM 7
build (`-debug-only=stack-slot-coloring` prints the weight of every interval)
and compare the sorted order against the target; then find which IR block's
frequency differs.  The current NDK clang 7.0.2 is a release build, so
`LLVM_DEBUG` is compiled out.

## 8. Commands / scratch

```text
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w6764/score.py            # official % / size / insns
cmake --build cmake-build-match --target check -j6
```

Scratch (not tracked): `/dev/shm/w6764/{score,bs.sh,dumpinsn,weights,mirfreq,
irblocks,irfreq_map,batch1..10,probe}.py`, `Level.base.cpp` (== HEAD),
`insns.json`, `mirfreq.json`, `mir_vrr.txt`, `mir_ssc.txt`, `orig.dis`,
`ours.dis`, `ops_summary.txt`.
