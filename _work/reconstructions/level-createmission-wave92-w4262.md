# w4262: `Level::createMission` (90.7 %, 17172 B) — the raid/guard spill swap is a
# *weight-order adjacency* problem: the guard must be processed before raid

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w4262`, branch `wave92-w4262`,
base `b379d00d`. Ground truth: `../_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; real address = Ghidra address − 0x10000).
No source change is committed: every size-preserving variant below is neutral.

## Baseline (unchanged, gate green)

```text
_ZN5Level13createMissionEv  fuzzy 90.7 %  size 17172 B  @0xaffd8
gate: byte 4499 / linked 4499, size 4524/4524, stubs 0, missing 0, wrong_type 0,
      extra 0, data 843/843, imports 207, .text 0x1337c8, .rodata 0x41b67,
      .ARM.exidx 0x6f70, .ARM.extab 0x26290 (-8), lint CLEAN, drift 0,
      RATCHET PASS
```

`cmake --build cmake-build-match --target check -j8` passes before and after this
pass. `report.json` is not committed. (`tools/verify_baseline.json` in this
worktree already holds `byte_exact`/`linked_exact` = **4499**, not the 4498
quoted in the wave-92 handoff; the wave-92 handoff's 4498 is stale.)

## Result in one line

The raid/guard spill swap is **not** a "raise the guard weight above the raid
weight by 0.108" problem in the sense of needing a different body: the two
intervals are **adjacent in the StackSlotColoring weight order** (raid at index
25, guard at index 26, nothing between them), so *any* source shape that moves
the guard's weight into `(1.301489, 1.429714)` flips the two colours and leaves
every other colour untouched. The flip therefore needs a **branch-frequency
(BFI) change**, not a different spill set: the guard's weight is
`freq(bb.1) + freq(bb.519) = 0.625000 + 0.568760` and the epilogue block
`bb.519` would have to rise from 0.568760 to **> 0.676489** (or raid's mass fall
by the same amount). No size-preserving source shape tried this pass moves the
BFI that far.

## 1. Exact StackSlotColoring weights and the adjacency theorem

Method (reproducible, see commands at the end): `-mllvm -print-machineinstrs`
gives the `After Virtual Register Rewriter` machine function (pre-colouring frame
indices) and `-mllvm -print-machine-bfi -mllvm
-print-bfi-func-name=_ZN5Level13createMissionEv` gives the machine BFI. The
second/third BFI sections are identical and are the ones SSC sees. A slot's
weight is `LiveIntervals::getSpillWeight(false, true, MBFI, MI)` summed over the
FI operands, i.e. the sum of the frequencies of the referencing blocks (one per
`%stack.N` operand; the `:: (store 4 into %stack.N)` memoperand comment must be
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

The **full weight order around the pair** (stable sort by weight descending, ties
by FI ascending — `std::stable_sort(SSIntervals, IntervalSorter())`):

```text
  index 23  fi#87  1.523901
  index 24  fi#100 1.429714
  index 25  fi#86  1.301489   <- raid
  index 26  fi#74  1.193760   <- guard
  index 27  fi#110 0.930655
  index 28  fi#93  0.922129
```

There is **nothing between raid and guard**. This is the key new fact: the
wave-90/91 notes treated the swap as a weight-margin problem, but the pair is
adjacent, so a weight crossing in the open interval
`(1.301489, 1.429714)` swaps exactly these two colours and nothing else.

### Why adjacency is sufficient (not just necessary)

`StackSlotColoring::ColorSlot` (`llvm/lib/CodeGen/StackSlotColoring.cpp`,
LLVM 7) walks `UsedColors[StackID].find_first()` upward and takes the first
colour whose `Assignments[Color]` do not `overlaps()` the interval; if none, it
takes `NextColors[StackID]` (the lowest never-used FI).

* The guard's live interval is a **single continuous range** from its store
  (bb.1, position 31) to its load (bb.519, position 8924): the value is the
  GOT address of `__stack_chk_guard`, stored once at `b0024` and reloaded once
  at `b02ca`. It therefore overlaps every spill slot that has any reference in
  the body.
* The slots that own colours 74..82 (the colours already taken when index 25 is
  processed) are fi#75/132/140 (74), 93/96/103/105/134/139 (75),
  99/106/108/109/110/114/117/118/119/131/135/137/141 (76),
  83/97/104/107/121/142/144 (77), 76/130/138/143/145/147 (78),
  111/112/120/123/126/129/133/136/146 (79), 77/115/116/124 (80),
  87/122/125 (81), 100/113/127/128 (82). Every one of them has a reference in
  `[29, 8856]`, so every one overlaps the guard's `[31, 8924]`.
* Hence the guard can never share colours 74..82; whichever of raid/guard is
  processed first at index 25 takes `NextColors` = **83**, and the other takes
  **84**. Ours: raid→83 (sp+88), guard→84 (sp+84). Original: guard→83
  (sp+88), raid→84 (sp+84) — i.e. **the original's guard weight is greater than
  the original's raid weight**.

The full pre→post colour map extracted from the `After Stack Slot Coloring`
section (85 entries; colour = post-colouring FI index; the colour→sp mapping is
`colour 83 → sp+88`, `84 → sp+84`, `85 → sp+80`, …, i.e. increasing colour =
decreasing offset):

```text
  74: [75,132,140]          75: [93,96,103,105,134,139]
  76: [99,106,108,109,110,114,117,118,119,131,135,137,141]
  77: [83,97,104,107,121,142,144]      78: [76,130,138,143,145,147]
  79: [111,112,120,123,126,129,133,136,146]
  80: [77,115,116,124]      81: [87,122,125]      82: [100,113,127,128]
  83: [86] raid             84: [74] guard          85: [95]
  86: [94]   87: [92]   88: [84]   89: [102]   90: [98]   91: [101]
  92: [88]   93: [89]   94: [80]   95: [82]   96: [79]   97: [78]
  98: [81]   99: [85]  100: [91]  101: [90]
```

(Colours 74..82 are heavily shared; 83..101 are single-owner long-lived
intervals. `fi#75` has 54 references and spans `[29, 8856]`, yet shares colour
74 with fi#132/140 — the real live intervals have holes, which is why a
`[first,last]` approximation cannot reproduce the colouring; see §3.)

## 2. The epilogue block frequency and the exact threshold

The guard slot's store is in `bb.1` (the block after the `mission == nullptr`
branch; `freq = 0.625 = 5/8`, the pointer heuristic `p == 0 → 12/32` taken /
`20/32` not-taken, `PH_TAKEN_WEIGHT=20`, `PH_NONTAKEN_WEIGHT=12`). Its load is
in `bb.519` (the shared early-return epilogue at `b02ca`:
`ldr.w fp,[sp,#88]; b.n b0194`).

`bb.519` frequency = 0.568760, decomposed over its 17 predecessors:

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
  total = 0.568759  (matches freq(bb.519) = 0.56876)
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
(`X != 0`, `X > 0`), which gives `f(bb.519) = 0.647` and a guard weight of
`1.272` — still below raid's 1.301489. So even a 62.5 % orbit branch is not
enough; the loop-exit contribution alone cannot close the gap.

## 3. Exact model of the colouring (validated 57/57 on an LLVM 7 debug build)

`/home/debian/llvm7build/build/bin/llc` is an assertions+DEBUG build
(`-debug-only=stack-slot-coloring` works). Running it on the NDK clang's
`-emit-llvm -g0` IR for `Level.cpp` gives, for `_ZN5Level13createMissionEv`, the
`Spill slot intervals:` dump (exact `[start,end)` ranges and weights), the
`Assigning fi#X to fi#Y` trace, and the post-colouring dump. A Python
re-implementation of `ColorSlot` (stable weight-desc sort, `UsedColors`
lowest-first share check against **all** slots already assigned to that colour,
else `NextColors`) reproduces the llc trace **exactly, 57/57 assignments**.
This validates the algorithm model used in §1. (The llc codegen is LLVM 7.0.0
and differs from the NDK's 7.0.2 — e.g. its guard slot is `fi#123` and its
frame is larger — so the llc FI numbers are not the NDK's; only the algorithm
behaviour is transferable.)

The llc trace also shows the sharing behaviour directly, e.g.:

```text
  Assigning fi#99 to fi#74      (highest weight takes the lowest FI)
  Assigning fi#95 to fi#75
  Assigning fi#74 to fi#76
  Assigning fi#129 to fi#74     (shares: disjoint ranges)
  ...
```

## 4. Variants measured this pass (all reverted)

Object sizes are a direct `Level.cpp` compile (NDK r18b clang 7.0.2, `-Oz`);
`guard_sp` is the `[sp,#N]` of the stack-protector-address spill (`88` = matches
the original, `84` = ours). Baseline: `guard_sp 84`, obj **17172**, 14732 insns.

| variant | guard_sp | obj size | note |
|---|---:|---:|---|
| **baseline** | **84** | **17172** | |
| `int introSystem` (working-tree variant) | **88** | **17160** | flips, but −12 B and a global RA change (r4/r9 instead of r0/r6) |
| `char`/`uchar`/`short`/`uint`/`long introSystem` | **88** | **17160** | all identical to `int`; the `bool` type is what produces the 16-bit `movs` and 17172 |
| `int raid` | 84 | 17172 | neutral; weights unchanged (1.301489 / 1.193760) |
| `unsigned raid`, `char raid` | 84 | 17172 | neutral |
| `bool raid = false;` + `if (!(...))` | 84 | 17172 | neutral |
| `raid` nested `if/else` | **88** | **17192** | flips, +20 B |
| `!hardCoreMode() && introSystem` | **88** | **17168** | flips, −4 B, global RA change (checks hardCore first, unlike original `b04a0`) |
| `campaign>=32 && raid && ...` | **88** | **17180** | flips, +8 B |
| `(security==3) && raid` (wave-91) | **88** | **17196** | flips, +24 B |
| `raidChance` if-chain / switch / temp | 84 | 17172 | neutral (size-preserving) |
| `raiders` via `if (raid)` | 84 | 17172 | neutral |
| `bool alienWave` | 84 | 17156 | b2e04 materialised `(i!=X)\|!alienWave`, −16 B |
| `bool bigAlienWave` / `bool pirateFortress` | 84 | 17156 / 17176 | neutral |
| `bool` all three | 84 | 17184 | neutral |
| decl order `alienWave,bigAlienWave,pirateFortress` | 84 | 17172 | neutral |
| `alienWave,pirateFortress,bigAlienWave` | 84 | 17164 | neutral |
| `mission_null_braces`, `mission_notnull`, `isempty_not`, `isempty_eq0/true`, `orbit_eq_true`, `orbit_ne_false`, `orbit_int_eq1/ne0`, `hardcore_eq_true/false`, `raid_eq_true`, `raid_sec_eq`, `supernova_eq` | 84 | 17172 | all neutral (IR canonicalises) |
| `playerPos` after the orbit check | 72 | 17296 | frame change |

No **size-preserving** variant flips the guard slot. Every variant that flips
(`int introSystem`, `!hardCore && introSystem`, `campaign>=32 && raid`,
`(security==3) && raid`, `raid` nested) either changes the size or changes the
register allocation globally (r4/r9 instead of r0/r6), so it is not a targeted
epilogue-lifetime fix.

### The `bool` type is load-bearing

`bool introSystem = false; ... introSystem = true;` compiles to the 16-bit
`movs r6,#0` / `movlt r6,#1`, giving 17172 B. Every other integer type compiles
to the 32-bit `mov.w r9,#0` / `movlt.w r9,#1`, which changes the register
allocation (r9 instead of r6), shrinks the function to 17160 B and flips the
guard slot as a side effect. The original is 17172 B and has the 16-bit form
(`b01f2: it lt; b01f4: movlt r6,#1`), so the original's `introSystem` is a
`bool` — the `int` variant is a coincidence, not the original shape.

## 5. What remains

1. **raid/guard swap** (~19 instructions, ~1.1 pp). The required change is a
   BFI change that raises `freq(bb.519)` above 0.676489 (or lowers raid's
   1.301489 by the same amount) while keeping the function at 17172 B. The only
   large lever is the alien-orbit loop exit (`0.3125`, 55 % of the epilogue
   frequency); the branch is a direct `i1` branch (50/50) and the loop heuristic
   is fixed at 4/128. A source shape that makes the loop exit more likely (or
   that changes the switch/return inputs to `bb.519` by ~0.11) is still missing.
2. **alienGuns `Vector` lifetime** (~50 instructions): faithful direct-init of
   `turretPos` after `setRotation` shares `turretDir`'s slot, but shrinks the
   function to 17144 B (wave-92 w4235 note).
3. **b2e04** (`bool alienWave`): correct shape, 16 B too small.
4. **convoy switch `b1408`**: needs unequal `!prof` weights below the 66 % peel
   threshold.
5. **EH landing pads** (`b41f0`–`b42d0`): block order only (`.ARM.extab`
   identical, 148 call sites).
6. **sp+56/60/64 permutation** (alienWave/bigAlienWave/pirateFortress): ours
   alienWave→89, pirateFortress→90, bigAlienWave→91; original
   bigAlienWave→89, alienWave→90, pirateFortress→91. This is the same class of
   BFI-driven colouring difference as the raid/guard swap (the original's
   `bigAlienWave` weight is greater than its `alienWave` weight, the reverse of
   ours), so both will likely fall together once the BFI matches.

## Method / commands

```sh
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
# exact Level.cpp compile (NDK r18b clang 7.0.2, -Oz), see /dev/shm/w4262/cc.sh
clang++ -target armv7-none-linux-androideabi16 -march=armv7-a -mthumb -mfpu=neon \
  -mfloat-abi=softfp -std=c++11 -fPIC -frtti -fstack-protector-strong -Oz -g \
  ... -ffunction-sections -funwind-tables -c src/game/world/Level.cpp
# spill weights
clang++ ... -mllvm -print-machine-bfi -mllvm -print-bfi-func-name=_ZN5Level13createMissionEv
# pre-colouring frame indices / CFG edges
clang++ ... -mllvm -print-machineinstrs -mllvm -filter-print-funcs=_ZN5Level13createMissionEv
# exact SSC trace (LLVM 7 debug build)
clang++ ... -S -emit-llvm -g0 -o Level.ll src/game/world/Level.cpp
/home/debian/llvm7build/build/bin/llc -mtriple=thumbv7-none-linux-android16 \
  -mcpu=generic -mattr=+neon,+vfp3,-d16,-fp16,-vfp4,-fp-armv8,+soft-float-abi \
  -float-abi=soft -O2 -debug-only=stack-slot-coloring -filetype=obj -o /dev/null Level.ll
# side-by-side disasm
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so --objdump <ndk-objdump> \
  --show _ZN5Level13createMissionEv
```

Scratch (not committed): `/dev/shm/w4262/` (`cc.sh`, `measure.py`, `tryvar.py`,
`variants1..8.py`, `mi3.txt`, `bfi.txt`, `ssc_dbg.txt`, `Level.ll`).
