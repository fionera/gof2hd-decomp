# w4417: `Level::createMission` (90.721649 %, 17172 B) — wave-94 re-measurement of the
# raid/guard spill margin, the `nameLeader` return shape, and the remaining diff regions

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w4417`, branch `wave94-w4417`,
base `f262bd4b`. Ground truth: `../_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; real address = Ghidra address − 0x10000).
**No source change is committed**: every variant below is neutral or a regression.
The deliverable is this note.

## Baseline (unchanged, gate green)

```text
_ZN5Level13createMissionEv   fuzzy 90.721649 %   size 17172 B   @0xaffd8
gate: byte 4499 / linked 4499, size 4524/4524, stubs 0, missing 0, wrong_type 0,
      extra 0, data 843/843, imports 207, .text 0x1337c8, .rodata 0x41b67,
      .ARM.exidx 0x6f70, .ARM.extab 0x26290 (-8), lint CLEAN, drift 0,
      RATCHET PASS
```

`cmake --build cmake-build-match --target check -j8` passes before and after this
pass. `report.json` is not modified/committed.

The official verifier (autojunk=False, normalised instruction stream) reports
**90.72164948453609 %**, orig 6055 insns vs ours 6070 insns, both functions
17172 B. (`verify-fn` is broken in this worktree — `--show "$ENV{FN}"` is
expanded at configure time — so the raw command is used; see §7.)

## Result in one line

The wave-94 handoff's **0.004193 / 0.004199 SSC margin is stale**: it is the
wave-90 base (`360c7467`) number. On this base (`f262bd4b`) the raid slot weighs
**1.301489** and the guard slot **1.193760**, a margin of **0.107729**. The two
slots are still **adjacent** in the StackSlotColoring weight order (indices 23
and 24), so the flip is a BFI problem, not a spill-set problem: the guard would
have to gain **+0.107729** (epilogue `freq(bb.519)` from 0.568760 to
`> 0.676489`). No size-preserving source shape found this pass moves the BFI
that far. The other residuals are re-localised and quantified below.

## 1. Exact StackSlotColoring weights on this base (re-measured)

Method: `-mllvm -print-machine-bfi -mllvm
-print-bfi-func-name=_ZN5Level13createMissionEv` gives the machine BFI; the
second and third emitted sections are identical and are the ones SSC sees.
`-mllvm -print-machineinstrs -mllvm
-filter-print-funcs=_ZN5Level13createMissionEv` gives the `After Virtual
Register Rewriter` machine function (pre-colouring frame indices). A slot's
weight is the sum of the frequencies of the blocks referencing it (each FI
operand once; the `:: (store 4 into %stack.N)` memoperand comment must be
excluded or every weight doubles).

```text
  slot (pre-color fi)   weight      final sp   refs
  raid        fi#86     1.301489    sp+88      19 distinct blocks
  guard       fi#74     1.193760    sp+84       2 (bb.1 store, bb.519 load)
  localRace   fi#83     5.488776    (other)
  alienWave   fi#102    0.527731    sp+64
  bigAlienWave fi#101   0.353174    sp+56
  pirateFortress fi#98  0.325035    sp+60
```

The full stable-sorted weight order around the pair (descending weight, ties by
FI ascending — `std::stable_sort(SSIntervals, IntervalSorter())`):

```text
  index 21  fi#87   1.523901
  index 22  fi#100  1.429714
  index 23  fi#86   1.301489   <- raid
  index 24  fi#74   1.193760   <- guard
  index 25  fi#110  0.930655
  index 26  fi#93   0.922129
```

There is **nothing between raid and guard**, exactly as wave-92 w4262 found
(that note saw the same pair at indices 25/26; two extra >1.9 slots, fi#137 /
fi#138 / fi#0, sit above them on this base and shift the index, not the
adjacency). The pre→post colour map extracted by aligning the `After Virtual
Register Rewriter` and `After Stack Slot Coloring` instruction streams is:

```text
  fi#86 (raid)   -> fi#83   (sp+88)      fi#74 (guard) -> fi#84 (sp+84)
  fi#102 (alienWave)    -> fi#89 (sp+64)
  fi#101 (bigAlienWave) -> fi#91 (sp+56)
  fi#98  (pirateFortress) -> fi#90 (sp+60)
```

`ColorSlot` walks `UsedColors` lowest-first and takes the first colour whose
`Assignments` do not overlap the interval, else `NextColors`. The guard's live
interval is the single continuous range `[bb.1 store, bb.519 load]`, so it
overlaps every body slot; whichever of raid/guard is processed first at index 23
takes `NextColors` = 83 (sp+88) and the other takes 84 (sp+84). Ours:
raid→83, guard→84. Original: guard→83, raid→84 (the original's guard weight is
greater than its raid weight). The flip interval is open:
`(1.193760, 1.429714)` — i.e. any guard weight in that band flips exactly these
two colours and nothing else.

## 2. The epilogue block frequency and the exact threshold

The guard slot's store is in `bb.1` (`freq = 0.625`, the `mission == nullptr`
pointer heuristic: `p == 0 → 12/32` taken / `20/32` not-taken, `PH_TAKEN_WEIGHT
= 20`, `PH_NONTAKEN_WEIGHT = 12`). Its load is in `bb.519`, the shared
early-return epilogue at `b02ca` (`ldr.w fp,[sp,#88]; b.n b0194`).

`freq(bb.519) = 0.568760`, decomposed over its predecessors (re-parsed on this
base; the numbers match the wave-92 note exactly):

```text
  bb.6    freq 10.000000  edge 3.125 %  contrib 0.312500   (alien-orbit loop exit)
  bb.133  freq  0.029297  edge 100 %    contrib 0.029297
  bb.307  freq  0.067096  edge 100 %    contrib 0.067096   (merge of 2 return paths)
  bb.318  freq  0.014850  edge 100 %    contrib 0.014850
  bb.325  freq  0.128350  edge 3.75 %   contrib 0.004813   (switch dispatch default)
  bb.326  freq  0.018601  edge 50 %     contrib 0.009300
  bb.357  freq  0.009300  edge 100 %    contrib 0.009300
  bb.368  freq  0.022461  edge 100 %    contrib 0.022461
  bb.382/396/402/417/440/454/465/488 (0.011230 each, 100 %) = 0.089840
  bb.517  freq  0.009300  edge 100 %    contrib 0.009300
  total = 0.568757  (matches freq(bb.519) = 0.568760)
```

The alien-orbit loop header `bb.6` (freq 10.0) is `0.3125 / (4/128)`:
`LBH_TAKEN_WEIGHT=124`, `LBH_NONTAKEN_WEIGHT=4` give a fixed 3.125 % exit
probability, and its entry edge frequency is `freq(bb.2) = 0.625 * 0.5 = 0.3125`
(the `inAlienOrbit()` call returns `i1` and is branched on directly, so BPI has
no heuristic and uses 50/50).

**Threshold.** To be processed before raid the guard needs
`0.625 + f(bb.519) > 1.301489`, i.e. `f(bb.519) > 0.676489` — a **+0.107729**
increase (19 %). Equivalently, the `inAlienOrbit()` branch would have to be
≥ 67.2 % in favour of the alien-orbit path (currently 50 %), because the loop
exit is `freq(bb.2) * 4/128` and `0.625 * p * 4/128 + 0.25626 > 0.676489`
⇒ `p > 0.672`. The `calcZeroHeuristics` alternatives only reach 62.5 %
(`X != 0`, `X > 0`), giving `f(bb.519) = 0.647` and a guard weight of 1.272 —
still below raid's 1.301489. The loop-exit contribution alone cannot close the
gap. (The wave-90 note's `epilogue > 0.572958` follows from its stale
`raid = 1.197958`; on this base the number is `0.676489`.)

## 3. `nameLeader`: the original really does `return`, but the faithful shape is 12 B short

Source (lines 2106–2111):

```cpp
    if (nameLeader) {
        (*this->enemies)[0]->name = *Globals::gameText->getText(1663);
    } else {
        return;
    }
    }
case4Else:
```

The original at `b408a`–`b40b2` is:

```text
  b408a  ldr r0,[sp,#28]
  b408c  cmp r0,#0
  b408e  beq.w b02ca          ; if (!nameLeader) return;
  b4092..b40ae                ; name = *getText(1663)
  b40b2  b.w b02ca            ; return;   <-- unconditional
  b40b6  ldr.w r8,[sp,#124]   ; (next block, reached from b19cc)
```

i.e. the name-assignment path **returns**; it does not fall through to
`case4Else`. Our baseline instead falls through from `b40aa` to the `case4Else`
code at `b40ac` (`vldr s0,[sl,#200]` … `new Route(coords,3)`), so the source is
missing a `return;` after the if/else. The faithful shape

```cpp
    if (nameLeader) {
        (*this->enemies)[0]->name = *Globals::gameText->getText(1663);
    }
    return;
```

reproduces the original's `beq.w` + `b.w` pair exactly (verified in the `nl`
build: `b41c2 beq.w b02ca` / `b41e6 b.w b02ca`), but compiles to **17160 B**
and scores **89.288661 %** — 12 B smaller than the original. The 12 B are a
global branch-relaxation/block-layout effect (the extra return block changes
`b.n`↔`b.w` selections elsewhere), not a missing statement at this site.
`return;` is the only legal form: a bare `break;` is a compile error there
(the enclosing switch has already closed), and `else { return; } return;`
compiles identically to the plain `return;`.

## 4. The three-slot permutation (`alienWave` / `bigAlienWave` / `pirateFortress`)

The original and ours assign the three `int` slots in a 3-cycle:

```text
                 original          ours
  bigAlienWave   sp+64 (fi#89)     sp+56 (fi#91)
  alienWave      sp+60 (fi#90)     sp+64 (fi#89)
  pirateFortress sp+56 (fi#91)     sp+60 (fi#90)
```

Evidence (original `b0ed2`–`b0f3c`):

```text
  b0ed8  cmp r0,#103 ; it gt ; movgt r0,#1
  b0ee2  str r0,[sp,#64]     ; bigAlienWave = mission > 103
  b0ee4  movs r0,#1
  b0ee6  str r0,[sp,#60]     ; alienWave = 1
  ...
  b0eec  movs r1,#0 ; str r1,[sp,#56]   ; pirateFortress = 0
  b0f00  str r0,[sp,#60]                ; alienWave = 0
  b0f06  str r0,[sp,#64]                ; bigAlienWave = 0
  ...
  b0f32  cmp r0,#140 ; it gt ; movgt r0,#1 ; b.n b14a2  ; pirateFortress = mission > 140
```

Ours (`b0ee2 str r0,[sp,#56]`, `b0ee6 str r0,[sp,#64]`, `b0ef2 str r1,[sp,#60]`,
…) is the same code with the three offsets permuted. This is the same class of
BFI/colour-order difference as the raid/guard swap: the original's
`bigAlienWave` is processed before its `alienWave`, the reverse of ours
(ours: alienWave 0.527731 > bigAlienWave 0.353174 > pirateFortress 0.325035).
The `bool alienWave` variant (which reproduces the original's materialised
`(i != X) | !alienWave` at `b2e04`) leaves this permutation unchanged
(fi#102→89, fi#101→91, fi#98→90), so it is not the lever either.

## 5. Other residuals re-localised on this base

Region-gain table (hybrid: replace the ours-region with the orig-region and
re-score with the official normalisation; base ≈ 92.3 in this metric):

```text
  region           gain    orig insns / ours insns
  cmp0ble          +0.10    2 / 2
  convoy_switch    +0.20   27 / 27
  b2e04            +0.10    8 / 8
  alienGuns        +0.50   42 / 40
  b3dda            +0.10   17 / 17
  EH pads          +1.40  108 / 107
  b19c0            +0.10   20 / 20
  b1d70            +0.10   11 / 11
  b2a9a            +0.10   13 / 13
  b3580            +0.10   38 / 39
  b3690            +0.10   18 / 17
  b3750            +0.30   58 / 61
```

* **EH pads (`b41f0`–`b42d2`)** — block order only; `.ARM.extab` is identical.
  The original pad region is 108 insns (`b41f0`–`b42d2`), ours 107
  (`b41ea`–`b42cc`) plus a trailing `nop` at `b42ce`; the original's
  `_Unwind_Resume` is at `b42d0`, ours at `b42ca`. The `b.n` filler run-lengths
  per pad differ (orig 3,5,2,5,3,10,12,17,4,3,2,1,4,1 vs ours
  2,6,2,5,3,9,11,19,4,3,2,1,4,1). The pad order follows the order of throwing
  invokes in the IR and is not locally controllable.
* **alienGuns (`b32c6`–`b332c`)** — the original keeps `turretDir` and
  `turretPos` in the **same** slot `sp+200..208`, temp at `sp+212`, `t[2]`
  staged in `sp+116`, `t[1]` in `r8`, and calls `Vector::operator=` via the
  PLT (`blx 5eb3c`). Ours puts `turretDir` at `sp+200`, `turretPos` at
  `sp+212`, temp at `sp+188`, and calls `Vector::operator=` through a
  GOT-loaded function pointer (`ldr r6,[got]; blx r6`). The faithful
  direct-init (`Vector turretPos = {…}` after `setRotation`) shares the slot
  but shrinks the function to 17144 B / 88.406276 %.
* **b3750 (`raid`/stream-out)** — the original starts the `if (raid)` block at
  `b3752` with `ldr.w r8,[sp,#108]` (raidRace) and `cmp r3,#0` (raid), and
  computes the hostile-loop preheader (`add r5,sl,fp`) *after* the stream-out
  check. Ours hoists the loop start (`ldr r4,[sp,#48]; add r4,r6; str
  r4,[sp,#112]`) before the `if (raid)` check, spills `raidType` to
  `[sp,#104]` and keeps `hard` in `r9`; the original keeps `raidType` in `r9`
  and `hard` in `r8`. This is a register-colouring/hoisting outcome.
* **convoy switch (`b1408`)** — source order is `if (k == 2) … else if (k == 1)
  … else if (k == 0) … else …`. The original lowers it as `cmp #2; beq;
  cmp #1; beq; cbnz r1, default` (order 2,1,0), ours as `cmp #0; beq;
  cmp #1; beq; cmp #2; bne` (order 0,1,2). This is branch-weight driven
  (`!prof`/loop-trip-count); no source spelling tried so far changes it.
* **b3690 (`case 40` else-branch tail)** — source lines 2369–2370:

  ```cpp
  (*this->enemies)[i]->cargo = nullptr;
  ((PlayerFighter *) (*this->enemies)[i])->crateCaptured() = 0;
  ```

  The original loads `(*this->enemies)[i]` **once** and stores both fields
  (`b3696 ldr.w r0,[r0,r6,lsl #2]; b369a str r1,[r0,#80]; b369c strb.w
  r1,[r0,#76]`). Ours reloads it for the second store (`b3696 ldr.w
  r1,[…]; b369a str r2,[r1,#80]; b369c ldr.w r0,[…]; b36a0 strb.w
  r2,[r0,#76]`) — a missed CSE across the `cargo` store. A local
  `KIPlayer *e = (*this->enemies)[i];` CSEs it but shrinks the function to
  17168 B / 90.712636 % (the original's extra 4 B are elsewhere).

## 6. Variants measured this pass (all reverted)

Object sizes are a direct `Level.cpp` compile (NDK r18b clang 7.0.2, `-Oz`);
`guard_sp` is the `[sp,#N]` of the stack-protector-address spill (`88` =
matches the original, `84` = ours). Baseline: 17172 B, guard_sp 84,
90.721649 %.

| variant | size | guard_sp | raw score | note |
|---|---:|---:|---:|---|
| **baseline** | **17172** | **84** | **90.721649** | |
| `bool alienWave` | 17156 | 84 | 89.215282 | materialises b2e04; −16 B |
| `bool alienWave`+`bool bigAlienWave` | 17156 | 84 | 89.215282 | |
| `bool bigAlienWave`+`bool pirateFortress` | 17176 | 84 | 89.896907 | |
| `bool alienWave`+`bool pirateFortress` | 17180 | 84 | 67.111771 | |
| `nameLeader` plain `return;` (drop `else`) | 17160 | 84 | 89.288661 | shape right, −12 B |
| `nameLeader` `else { return; } return;` | 17160 | 84 | 89.288661 | identical to plain return |
| alienGuns `turretPos` direct-init after `setRotation` | 17144 | 84 | 88.406276 | slot merge, −28 B |
| alienGuns direct-init + nameLeader return | 17132 | 84 | 88.804429 | |
| `bool alienWave` + nameLeader return | 17144 | 84 | 87.945839 | |
| `case 4` dispatch restructured to `if (nextInt(2)==0) goto case4Else; else {…goto case4Tail;}` | 17172 | 84 | 90.721649 | neutral |
| `&& alienWave != 0` / `bool first = …` / `alienWave && i == …` / `& (alienWave != 0)` | 17172 | 84 | 90.721649 | neutral (IR canonicalises) |
| `int hard = hardCoreMode();` | 17172 | 84 | 90.721649 | neutral |
| swap `raidType`/`hard` declaration order | 17172 | 84 | 90.705154 | −0.016 |
| `20 + (hard ? 20 : 0) + getLevel()` / `20 + 20*hard + getLevel()` | 17172 | 84 | 90.721649 | neutral |
| move `raidType` after the stream-out `if` | 17180 | 84 | 90.640719 | +8 B |
| `int raidStart = guns + …; for (i = raidStart; i < raidStart + hostileCount; i++)` | 17180 | 84 | 90.625773 | +8 B |
| `KIPlayer *e = (*this->enemies)[i]; e->cargo = …; e->crateCaptured() = 0;` | 17168 | 84 | 90.712636 | CSEs b3690, −4 B |
| swap the `cargo`/`crateCaptured` statements | 17176 | 84 | 90.598714 | +4 B |
| `cargo = 0` (int) instead of `nullptr` | 17172 | 84 | 90.721649 | neutral |
| `Player *p2 = …->player;` before the two stores | 17176 | 84 | 90.598714 | +4 B |

(`break;` in place of `return;` at the nameLeader site does not compile: the
enclosing `switch` has already closed — `error: 'break' statement not in loop
or switch statement`.)

## 7. Method / commands

```sh
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
# exact Level.cpp compile (NDK r18b clang 7.0.2, -Oz), see /dev/shm/w4417/cc.sh
clang++ -target armv7-none-linux-androideabi16 -march=armv7-a -mthumb -mfpu=neon \
  -mfloat-abi=softfp -std=c++11 -fPIC -frtti -fstack-protector-strong -Oz -g \
  ... -ffunction-sections -funwind-tables -c src/game/world/Level.cpp
# spill weights / pre-colouring frame indices
clang++ ... -mllvm -print-machine-bfi -mllvm -print-bfi-func-name=_ZN5Level13createMissionEv \
  -mllvm -print-machineinstrs -mllvm -filter-print-funcs=_ZN5Level13createMissionEv
# official side-by-side (verify-fn is broken in this worktree)
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5Level13createMissionEv
# raw score (same normalisation as verify.py, autojunk=False)
python3 /dev/shm/w4417/rawscore.py cmake-build-match/libgof2hdaa.so
```

Scratch (not committed): `/dev/shm/w4417/` (`cc.sh`, `measure.py`,
`rawscore.py`, `regiongain.py`, `diffregions.py`, `dumpfn.py`, `tryvar.py`,
`batch1..9.py`, `Level.cpp.orig`, `orig_fn.s`, `ours_fn.s`, `mi_new.err`,
`mi_bw.txt`, `mi_nl.txt`, `mi_disp.txt`).

## 8. What remains

1. **raid/guard swap** (~19 instructions, ~1.1 pp). The pair is adjacent in the
   SSC weight order on this base (indices 23/24). The required change is a BFI
   change that raises `freq(bb.519)` above **0.676489** (or lowers raid's
   1.301489 by the same amount) while keeping the function at 17172 B. The only
   large lever is the alien-orbit loop exit (`0.3125`, 55 % of the epilogue
   frequency); the branch is a direct `i1` branch (50/50) and the loop
   heuristic is fixed at 4/128. A source shape that makes the loop exit more
   likely (or changes the switch/return inputs to `bb.519` by ~0.11) is still
   missing. **Use 0.676489, not 0.572958.**
2. **nameLeader return** (12 B): the faithful `return;` shape is proven by the
   original's `b.w b02ca` at `b40b2`, but it compiles to 17160 B. The missing
   12 B are global branch-relaxation, not a local statement.
3. **alienWave/bigAlienWave/pirateFortress 3-cycle** (`sp+64/60/56` vs
   `sp+56/64/60`): a colouring outcome; `bool alienWave` does not move it.
4. **alienGuns `Vector` lifetime / call-through-GOT** (~0.5 pp): the original
   shares `turretDir`/`turretPos` in one slot and calls `Vector::operator=`
   via the PLT; ours uses three slots and a GOT-loaded call.
5. **convoy switch `b1408`** (+0.20): needs the original's 2,1,0 compare order,
   i.e. unequal `!prof` weights below the 66 % peel threshold.
6. **b3690 CSE** (+0.10): the original CSEs the `(*this->enemies)[i]` load
   across the `cargo` store; a local variable CSEs it but costs 4 B.
7. **EH landing pads** (+1.40): block order only (`.ARM.extab` identical,
   108 vs 107 insns + trailing `nop`); invoke-order-driven.
