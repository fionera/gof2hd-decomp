# Generator::createMission — exact StackSlotColoring weights, and why the 74.3 shape is a
# local optimum (w4408)

Status: **research note, no source change landed** (wave 94, 2026-11-03, worktree `w4408`,
branch `wave94-w4408`, base `f262bd4b`). The landed `short pick` / `goto assignType` shape
from w4232/w4257 stays at **74.3 %** (`osize 1996  usize 1996`, 727 instructions).

This pass replaces the previous *inferred* SSC weight model with a **measured** one. The
NDK clang 7.0.2 release build accepts `-mllvm -print-machine-bfi`, which prints the
`MachineBlockFrequencyInfo` block frequencies, and `-mllvm -print-machineinstrs`, which prints
the machine code at every pass (including `After Greedy Register Allocator` and
`After Prologue/Epilogue Insertion & Frame Finalization`, the latter with the final frame
offsets). Combining the two reproduces LLVM 7's `StackSlotColoring` weight exactly and explains
every slot in both the original and our build.

With that tool the remaining gap is now a **pure weight-ordering** statement, not a mystery:

```
original : rnd > race > status > targetStation > agent > &__stack_chk_guard > galaxy
ours(HEAD): rnd > race > used > status > type > agent > targetStation > mission > amount > ...
```

`used` sits in `sl` in the original, so it never enters the ordering; `galaxy` is spilled in the
original (lowest weight, `[sp,#16]`) and kept in `r6` in ours. Two independent flips are needed
(`used -> sl`, `targetStation > agent`), and every source spelling that buys one loses the other.

Gate at HEAD (unchanged by this note):

```
verify: byte 4499  linked 4499  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
lint CLEAN  drift 0   RATCHET PASS
```

Target row: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(original @ `0x971b8`, size 1996, image base 0x10000).

## 1. The measurement method (new, reusable)

```sh
CXX=$NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++
$CXX <the exact compile_commands.json flags for Generator.cpp> \
     -c src/game/mission/Generator.cpp -o /tmp/t.o \
     -mllvm -print-machineinstrs -mllvm -print-machine-bfi > mi.txt 2>&1
python3 weights.py mi.txt     # per-FI weight + referencing blocks
python3 fimap.py  mi.txt      # per-FI final [sp,#N] offset + DBG_VALUE variable names
```

* `-mllvm -print-machine-bfi` prints one `block-frequency-info: <mangled>` block per pass that
  computes MBFI. The one printed immediately **before** `# After Greedy Register Allocator:` is
  the CFG the register allocator produced and therefore the one SSC sees (183 blocks for this
  function; the later printouts have different block numbering and must not be used).
* `weights.py` sums, for every non-`DBG_VALUE` `%stack.N` operand in the
  `After Greedy Register Allocator` section, the frequency of the containing block. That is
  exactly `LiveIntervals::getSpillWeight(false, /*isUse=*/true, MBFI, MI)` summed by
  `StackSlotColoring::ScanForSpillSlotRefs` (LLVM 7.0.0 `lib/CodeGen/StackSlotColoring.cpp`).
* `fimap.py` reads the `After Prologue/Epilogue Insertion & Frame Finalization` section: its
  `$sp, N` operands are the final stack offsets and its `DBG_VALUE ... DW_OP_plus_uconst, N`
  records name the source variable that lives there.

The absolute weights printed below are 2x the LLVM value (each `%stack.N` operand is counted
twice by the dump — once as the operand and once in the `:: (load/store ...)` annotation).
The factor is uniform, so the **ordering** is exact.

### 1.1 The SSC assignment rule (LLVM 7.0.0)

* `InitializeSlots` collects the LiveStacks intervals, sorts them **stably by weight descending**
  and sets `NextColors[0] = AllColors[0].find_first()`, i.e. the **lowest FI index that has a
  live interval**.
* `ColorSlot` first tries the **lowest already-used colour** whose intervals do not overlap the
  new interval (sharing), otherwise takes the next new colour.
* PEI then lays the surviving FIs out in index order, so **colour index order == stack offset
  order**: the first-processed (highest-weight) interval gets the lowest FI index, which PEI
  places at the **highest** local offset (`[sp,#92]` here), and each subsequent new colour steps
  down by 4 bytes.

For this function the canary value (`fi#0`, `[sp,#92]`) and the four 12-byte
`AbyssEngine::String` temporaries (`fi#1..fi#4`) have no LiveStacks interval, so the first
colour is **`fi#5` = `[sp,#40]`** and the coloured slots are `fi#5..fi#11` = `[sp,#40..#16]`.

## 2. Measured weights — HEAD (74.3 %)

`-print-machine-bfi` + `-print-machineinstrs` on the landed source:

```
FI   weight    DBG name        referencing blocks
8    80.2319                   [3, 5, 28, 46, 57, 65, 78, 87, 90, 94, 105, 140]   <- rnd
9    34.9840   race            [3, 7, 29, 33]
16   17.5078   used            [3, 9]
10   11.4336                   [3, 21, 25, 95, 107, 128, 131]                     <- status
11   11.0769   type            [35, 36, 37, 43, 46, 48, 59, 62, 92, 112, 129, 139, 141]
7     6.0812                   [3, 29, 33, 111, 137]                              <- agent
6     4.0000   targetStation   [1, 2, 29, 33]
14    3.1218   mission         [136, 138, 146, 153]
12    3.0574   amount          [80, 86, 88, 91, 92, 93, 95, 121, 125, 154, 180]
0     2.6938                   [0, 168]                                            <- canary value (fi#0)
5     2.6938                   [0, 168]                                            <- &__stack_chk_guard
1     2.5203   agentName       [56, 137, 168, 171]
17    2.0812   fromSystemPtr   [109, 111]
15    2.0812   bonus           [130, 134, 155]
2     1.5609                   [137, 139, 140, 145]
13    1.2192   difficulty      [95, 97, 106, 131, 134]
4     0.5039                   [162, 163, 164, 166]
3     0.3902                   [149, 150, 151, 152]
```

Applying the rule above reproduces the observed frame **exactly**:

| order | FI | weight | colour | final slot | contents |
|---:|---:|---:|---:|---|---|
| 1 | 8  | 80.23 | 5  | `[sp,#40]` | `rnd` |
| 2 | 9  | 34.98 | 6  | `[sp,#36]` | `race` |
| 3 | 16 | 17.51 | 7  | `[sp,#32]` | `used` |
| 4 | 10 | 11.43 | 8  | `[sp,#28]` | `status` |
| 5 | 11 | 11.08 | 6 (share) | `[sp,#36]` | `type` |
| 6 | 7  | 6.08  | 9  | `[sp,#24]` | `agent` |
| 7 | 6  | 4.00  | 10 | `[sp,#20]` | `targetStation` |
| 8 | 14 | 3.12  | 7 (share) | `[sp,#32]` | `mission` |
| 9 | 12 | 3.06  | 10 (share) | `[sp,#20]` | `amount` |
| 10 | 5 | 2.69  | 11 | `[sp,#16]` | `&__stack_chk_guard` |
| 13 | 17 | 2.08 | 7 (share) | `[sp,#32]` | `fromSystemPtr` |
| 14 | 15 | 2.08 | 8 (share) | `[sp,#28]` | `bonus` |
| 16 | 13 | 1.22 | 9 (share) | `[sp,#24]` | `difficulty` |
| –  | 0  | 2.69 | (no interval) | `[sp,#92]` | canary value |
| –  | 1  | 2.52 | (no interval) | `[sp,#80]` | `agentName` (12 B) |

This is the w4260 slot map, now derived rather than guessed:
`16=&guard, 20=targetStation/amount, 24=agent/difficulty, 28=status/bonus, 32=used/mission,
36=race/type, 40=rnd`.

## 3. Measured weights — the original (from its disassembly)

The original's `[sp,#N]` references (full listing, `orig.asm` = objdump of
`0x971b8..0x979a4`) give the contents of each slot:

```
[sp,#16]  97614 str galaxy        97644 ldr galaxy (distance)   976d8 str 0 / 97720 str bonus   9783a ldr bonus (setBonus)
[sp,#20]  971f6 str &__stack_chk_guard                              978b2 ldr  (epilogue compare)
[sp,#24]  97220 str agent         97306/97322 ldrd agent        9752a/9761c str difficulty       9764c/97722/97908 ldr difficulty
[sp,#28]  97218/9721c str targetStation   974a8/974b4/9753e/975ca/97604 str amount   97830/97878/978ec ldr amount
[sp,#32]  97228 str status  972b4/972da/97536/9761a/976ac/976dc ldr status  9770e str rem  97718 ldr rem
          9775e str mission  9776c/97824/9793c ldr mission
[sp,#36]  9726e ldr race   9730c/97326 ldr race   97340 str type   97356/9738e/97420/974ec/9768e/976c8/97782/977c4 ldr type
[sp,#40]  97244 str rnd    then 11 loads
```

So the original's colour order is:

```
[sp,#40] rnd   >  [sp,#36] race  >  [sp,#32] status  >  [sp,#28] targetStation
          >  [sp,#24] agent  >  [sp,#20] &__stack_chk_guard  >  [sp,#16] galaxy
```

The two structural facts this proves:

1. **`used` is not a stack slot in the original** — it is `sl` (`9723a: ldr.w sl,[r0,#80]`),
   so it never enters the SSC ordering at all.
2. **`galaxy` *is* a stack slot in the original**, and it is the *lowest*-weight interval
   (`[sp,#16]`, the last colour), sharing with `bonus`. It is loaded from the GOT, spilled once
   (`97610..97614: ldr r5,[r0]; ldr r0,[r5]; str r0,[sp,#16]`) and reloaded for
   `Galaxy::distance` (`97644`), while `Galaxy::getStation` re-loads `Globals::galaxy` from the
   GOT (`97630: ldr r0,[r5,#0]`) instead of using the spilled copy. That is a
   rematerialisation of a spilled global, i.e. the source is a plain
   `Galaxy *galaxy = Globals::galaxy;` local that the allocator chose to spill.

## 4. Measured weights — the `int pick` family (`int5`, 68.7 %)

`int5` is the w4257 §5 shape: `int pick`, `while (!selected)`, `--guard` after `nextInt`,
`__builtin_expect(pick == 12, 0)`, and `Globals::galaxy` used directly in the tail.

```
FI   weight    name            blocks
8    80.2319                   [3, 5, 25, 43, 54, 62, 75, 84, 87, 91, 102, 137]   <- rnd
9    36.5449   race            [3, 7, 26, 30, 108, 131]
11   11.0769   type            [32, 33, 34, 40, 43, 45, 56, 59, 89, 109, 126, 136, 138]
10    5.0453                   [3, 20, 22, 108]                                    <- status
6     4.0000   targetStation   [1, 2, 26, 30]
7     4.0000                   [3, 26, 30]                                         <- agent
13    3.3004   difficulty      [92, 94, 103, 104, 109, 136]
...
```

Resulting frame: `[sp,#40] rnd, [sp,#36] race, [sp,#32] targetStation/type,
[sp,#28] status/bonus, [sp,#24] agent/difficulty, [sp,#20] ?, [sp,#16] amount`.

`used` is in `sl` here (the w4257 chain), so `status` moves **up** one colour to 7 — exactly the
original's rank for it — and `agent/difficulty` lands on `[sp,#24]`, also exactly right. The two
remaining mismatches are:

* `type` (11.08) is processed **before** `status` (5.05), so it takes a fresh colour 7 instead of
  sharing `race`'s colour 6. In the original `type` shares `race`'s `[sp,#36]`.
* `targetStation` (4.00) ties `agent` (4.00) and, being FI 6 < FI 7, is processed first and takes
  the fresh colour that `status` would otherwise have had; the original needs
  `targetStation > agent`.

## 5. Region-by-region mismatch accounting (official verifier, `autojunk=False`)

`SequenceMatcher` opcodes bucketed by the original's address
(`prologue+loop < 0x972e0`, `postloop < 0x97340`, `mid < 0x97600`, `reward < 0x97700`,
`mission < 0x97800`, `epilogue`):

| shape | instrs (orig/ours) | score | mismatch | loop | postloop | mid | reward | mission | epilogue |
|---|---|---:|---:|---:|---:|---:|---:|---:|---:|
| **HEAD (`short pick`)** | 727 / 727 | **74.3** | 220 | 49 | 3 | 85 | 37 | 13 | 33 |
| `int5` (`int pick`) | 727 / 726 | 68.7 | 284 | **20** | 15 | 107 | 41 | 30 | 71 |
| `int5` + local `galaxy` | 727 / 725 | 70.9 | 247 | 25 | 6 | 98 | 40 | 25 | 53 |

The `int pick` loop is worth **29 instructions** (49 -> 20) and is essentially the original's
loop encoding (`mov r6,r0; cmp r0,#8; cmp r6,#10; cmp r6,#12`, non-folded
`movs r2,#12; it ne; movne r2,r6`). It pays for that with **+64** instructions in the tail,
because the tail's register/slot allocation changes once `used` leaves the stack.

## 6. New measurements this pass

All scores are the official verifier (`tools/verify/verify.py` scoring path,
`autojunk=False`) after a full `link_so`.

### 6.1 `0.5f` instead of the `0.5000001f` anchor: 74.5, but the size drops to 1992

The original really is `vmov.f32 s2,#96` (`0x3f000000`, exact 0.5) at `977ce`. Replacing the
documented anchor with `0.5f` on the landed shape gives

```
HEAD + 0.5f : pct 74.5  osize 1996  usize 1992  n_orig 727  n_ours 726
```

i.e. **+0.2 points and one instruction closer**, but the function shrinks by 4 bytes. Because
`createMission` is followed by ~2600 functions in `.text`, the size change shifts every later
address and the gate collapses:

```
RATCHET FAIL   byte_exact 4499 -> 2615   (linked_exact stays 4499)
```

So the anchor is not decorative: it is the only thing holding the function at 1996 B while the
`short pick` family cannot produce the original's non-folded `pick == 12` select (which is the
real 4 bytes). Recovering those 4 bytes elsewhere while keeping `0.5f` was tried and failed —
every candidate that adds bytes also changes the layout:

| variant (on `HEAD + 0.5f`) | pct | size |
|---|---:|---:|
| `HEAD + 0.5f` (baseline) | **74.5** | 1992 |
| `+ int amt` temp in the `type == 8` block | 74.5 | 1992 |
| `+ __builtin_expect(sum == 14, 1)` | 74.5 | 1992 |
| `+ __builtin_expect(type == 12, 1)` | 74.5 | 1992 |
| `+ __builtin_expect(pick == 12, 0)` | 73.5 | 1992 |
| `+ __builtin_expect(campaign < 16, 1)` | 72.8 | 1992 |
| `+ difficulty = getTecLevel()` before `amount` | 72.0 | 1988 |
| `+ __builtin_expect(getRoutes()==nullptr, 0)` | 56.9 | 1968 |
| `+ __builtin_expect(pick == 12, 1)` | 66.9 | 1988 |
| `int pick` + `0.5f` (`int5_half`) | 68.9 | 1992 |

### 6.2 Tail-order / initialiser sweep on the `int pick` family

24 tail orders x {`Globals::galaxy` direct, `Galaxy *galaxy` local} x {`0.5f`, anchor} x
{`amount`/`itemId` initialised, `amount` uninitialised, both uninitialised} x {clamp before the
tail, clamp after `fromSystemPtr`} (72 variants, all with the exact `int5` loop). Best:

| tail order | galaxy | 0.5f | init | pct | size |
|---|---|---|---:|---:|---:|
| `toSystem, fromSystem, fromSystemPtr` | global | yes | both uninit | **71.8** | 1968 |
| `toSystem, fromSystem, fromSystemPtr` | global | no | both uninit | 71.5 | 1972 |
| `fromSystem, toSystem, fromSystemPtr` | local | yes | both uninit | 71.3 | 1992 |
| `fromSystem, fromSystemPtr, toSystem` | local | yes | both uninit | 71.2 | 1976 |
| `fromSystem, fromSystemPtr, toSystem` | local | no | initialised | 70.9 | 1996 |

Nothing in this family reaches 74.3.

### 6.3 Single-knob sweep on the landed shape

All 1996 B, all 74.3 unless noted: `int16_t`/`signed char` pick, `int type`, `int race`,
`int level = 0`, `int rem` instead of `float rem`, `int bonus = 0`, `int difficulty = 0`,
`(short)` casts on the three `pick` comparisons, `pick == 8 && pick != 9`,
`int p2 = pick` alias, `Galaxy *galaxy` moved before the clamp, `(void) 0` after
`fromSystemPtr`. Regressions: `int selected` 74.1, `short guard` 73.3, `unsigned guard` 61.6,
`amount` uninitialised 72.6, `itemId` uninitialised 66.4, `both uninitialised` 73.7,
`used` declared before `race` 71.1, `guard` declared first 71.3, `toSystem` computed first 72.4,
`used` replaced by inline `Globals::status->field_50` 57.0 (frame grows to 108).

## 7. What this proves / what remains

**Proved.**

* The exact LLVM 7 SSC weights for this function are now measurable with the shipped NDK clang
  (`-mllvm -print-machine-bfi`), and the model reproduces the landed frame slot-for-slot and the
  original's slot contents slot-for-slot from the disassembly.
* The original's ordering is `rnd > race > status > targetStation > agent > &guard > galaxy`,
  with `used` in `sl` and `galaxy` spilled to the last colour.
* The landed shape's ordering is `rnd > race > used > status > type > agent > targetStation >
  mission > amount > &guard`; the two flips needed are `used -> sl` and
  `targetStation > agent`.
* `0.5f` is worth +0.2 (74.5) but is unusable at 1992 B: the size change alone costs 1884
  `byte_exact` functions because every later `.text` address shifts.

**Remains.**

* `short pick` cannot produce the original's non-folded `pick == 12` select, so the 4 bytes that
  the `0.5000001f` anchor currently holds must come from a *different* real source difference
  before the anchor can be dropped.
* `int pick` gives the original's loop (49 -> 20 mismatches) and the original's `status` and
  `agent/difficulty` colours, but then `used` moves to `sl` and the tail loses 64 instructions.
  The `int pick` tail needs `type` to share `race`'s colour and `targetStation` to outrank
  `agent`; neither was reachable with 72 tail orderings or the single-knob sweep.
* A cheap way to shift one interval's weight by ~1.0 (the `status` vs `targetStation` gap in
  `int5` is only 5.05 vs 4.00) would flip the `int5` frame to the original's. No faithful source
  spelling tried so far does it.

## 8. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w4408
cmake --build cmake-build-match --target check -j8        # RATCHET PASS, byte 4499

# exact SSC weights + final slot map for the current source
$NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++ <Generator.cpp compile flags> \
  -c src/game/mission/Generator.cpp -o /dev/shm/w4408/t.o \
  -mllvm -print-machineinstrs -mllvm -print-machine-bfi > /dev/shm/w4408/mi.txt 2>&1
python3 /dev/shm/w4408/weights.py /dev/shm/w4408/mi.txt
python3 /dev/shm/w4408/fimap.py  /dev/shm/w4408/mi.txt
```

Scratch harness (not committed, `/dev/shm/w4408`): `apply.sh` (replace the whole
`Generator::createMission` body), `score.sh` + `score1.py` (official single-symbol score),
`analyze.sh` (apply + build + weights + slot map + score), `weights.py`, `fimap.py`,
`diffstat2.py` (official-normalised per-region mismatch buckets), `sweep.py` / `sweep2.py` /
`sweep3.py` / `sweep4.py` (the sweeps tabulated above), `head_func.cpp` (landed body),
`int5.cpp` / `int5_gal.cpp` / `int5_half.cpp` (the `int pick` family), `orig.asm` / `head.asm`.
