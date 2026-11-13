# Generator::createMission — wave 191b / w13557: 3-byte residual, no win landed

Status: **superseded by `generator-createmission-wave191b-w13557-win.md`**
(raw 3 -> 1 landed; this note records the dead ends explored first).  Worktree
`wt/w13557`, branch `wave191b-w13557`, base master `4c5d0b229`.  Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

`_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` (1996 B,
744 insn) is still **raw 3 / aligned 3 / size 1996/1996**.  A full
byte-diff of the function body (`bdiff.py`, ELF `.text` slice) shows
exactly three differing bytes and nothing else:

| offset | original | ours (HEAD) |
|---|---|---|
| `0x9735a` (`bhi.n`) | `d815` -> `0x97388` (race-check block) | `d84e` -> `0x973fa` (merge block) |
| `0x9770e` | `9108` `str r1,[sp,#32]` | `910b` `str r1,[sp,#44]` |
| `0x97718` | `9808` `ldr r0,[sp,#32]` | `980b` `ldr r0,[sp,#44]` |

Every other instruction, the frame (96 B), the spill-slot map and the
alloca layout (agentName `sp+80`, Strings `sp+68/56/44`, canary
`sp+92`) are identical.  The two diffs are independent; fixing either
alone is a raw-1/raw-2 win, but neither reaches raw 0.

## 1. The `rem` slot is an RA spill slot in the original, not an alloca

Evidence that `sp+32` cannot be an alloca in this function:

* frame = 96; the four `AbyssEngine::String` objects (12 B each,
  `sp+44..91`) plus the stack-protector canary (`sp+92`) fill the
  *local* area exactly.  PEI assigns `ObjectsToAllocate` in frame-index
  order from high to low (`PrologEpilogInserter.cpp`
  `calculateFrameObjectOffsets`), so any extra alloca would land at
  `sp+40` or higher, never `sp+32`.
* `sp+32` is shared by three values in the original: `status`
  (`0x976dc`), `rem` (`0x9770e`/`0x97718`) and `mission` (`0x9775e`) —
  i.e. it is a *spill slot*, re-coloured by machine-level
  `StackSlotColoring`.
* `LiveStacks` intervals are only created for RA spill slots
  (`InlineSpiller.cpp:1033` is the sole `getOrCreateInterval(Slot,...)`
  caller), and `StackSlotColoring::InitializeSlots` only colours
  `LS->getNumIntervals()`.  A `volatile` alloca therefore can *never*
  be moved by SSC; it keeps its PEI offset.

Consequence: the original's `rem` is a plain `int` that the register
allocator spilled.  Our HEAD uses `volatile int rem = remVal;` whose
alloca is IR-level `StackColoring`-merged into the `%stack.5`
`setTargetSystemName` String slot (`sp+44`) — the only shape that
reproduces the original's *instruction stream* byte-for-byte.

### Measured shapes (all size 1996 unless noted)

| variant | raw | frame | rem |
|---|---:|---:|---|
| HEAD `volatile int rem` + `int remCopy = rem` | **3** | 96 | `sp+44` (alloca) |
| any plain-int form (`int rem = rawBonus % 50; int b = rawBonus + rem; if (b % 50 != 0) b = rawBonus - rem; bonus = b;`) | 97 | 104 | `r9`; agent spilled to a new slot |
| plain + `if (agent == nullptr) { bonus = 0; }` after `bonus = b;` | 571 | 96 | **`sp+32`** |
| plain + `bonus = agent ? b : 0;` / `if (!agent)` | 571 | 96 | `sp+32` |
| plain + `__builtin_expect((type\|4)==12, 1)` | 1826 | 96 | `sp+32` (size **1992**, else block moved out of line) |
| `volatile int rem` hoisted to function top | 85 | 104 | `sp+96` |
| `alignas(8) volatile int rem` | 85 | 104 | `sp+48` |
| `volatile int rem[1..4]`, `volatile int *rem`, `volatile short/long long/float/double rem` | 3..689 | 96..104 | still merged or worse |
| `volatile int b` + plain `rem` | 137 | 96 | `b` at `sp+44`, rem in `r5` |

The `if (agent == nullptr) bonus = 0;` trick makes the whole bonus
block instruction-identical to the original (`adds r5,r1,r6;
str r1,[sp,#32]; movs r1,#50; mov r0,r5; blx idivmod; ldr r0,[sp,#32];
cmp r1,#0; it ne; subne r5,r6,r0`) — it keeps `agent` in `r9` so `rem`
is the value that gets spilled.  Cost: `cmp.w r9,#0; it eq; moveq
r5,r9` = +8 B (size 2004).  No zero-cost agent use was found
(`if (agent) bonus = b;` does *not* flip the allocation; `(void)agent`
is a no-op).

## 2. The `bhi` fold is a JumpThreading/LVI decision

On the `type >= 15` edge (`0x97356` `cmp r1,#14; bhi`), LVI knows
`type u>= 15`, hence `type != 13`, so JumpThreading retargets the edge
from the race-check block (`0x97388`) to the merge block (`0x973fa`).
The original keeps the redundant `type == 13` test.

Measured (HEAD `rem` shape):

| variant | raw | size |
|---|---:|---:|
| `(type & 0xffffU) == 13` (also `&0xffU`, `(int)(type&0xffffU)`) | 366 | 1996 (inserts `uxth r0,r0` at `0x97390`, shifting everything +2) |
| `(type & 0xffffU) == 13` + `__builtin_expect(...,1)` | 1761 | 1964 |
| `volatile uint32_t type` | 1674 | 2000 (frame 104) |
| `*(volatile uint32_t *)&type == 13` | 1871 | 2012 |
| `type % 16 == 13` | 1408 | 2000 |
| `uint16_t type` | 1683 | 1976 |
| `__builtin_expect((uint32_t)type < 15 && ..., 1)` | 1832 | 1944 |
| `if (type == 13 && race < 4)` | 3 | 1996 (compiler reorders back) |

No form was found that defeats LVI without adding at least a 2-byte
instruction, so the branch byte cannot be fixed size-neutrally today.

## 3. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE
# raw=3 aligned=3 size=1996/1996 insn=744/744
```

Scratch harness (not in the repo): `/home/debian/GalaxyOnFire2/scratch/w13557/`
— `Generator.base.cpp` (pristine HEAD copy), `try.sh` (build+score),
`bdiff.py` (exact byte diff of the function's `.text` slice),
`sweep3.py`/`batch3.py` (variant sweeps), `probe.py` (frame + rem
store/load offsets), `one.sh` (single-TU `-mllvm` compile), plus
LLVM 7.0.0 sources (`StackColoring.cpp`, `StackSlotColoring.cpp`,
`LiveStacks.cpp`, `RegAllocGreedy.cpp`, `CalcSpillWeights.cpp`,
`PrologEpilogInserter.cpp`).

## 4. Suggested next angles

* Find a **zero-instruction** way to keep `agent` live past the bonus
  block (the null-check proves that this alone flips `rem` onto
  `sp+32` and reproduces the block exactly).  Any extra use costs >= 2 B
  and breaks the 1996-byte frame.
* Or make `rem` a plain int that the RA spills: `rem.weight >
  agent.weight` in `RAGreedy::shouldEvict` is the current outcome
  (`rem` is short-lived, `agent` spans ~1100 B).  Lowering `rem`'s
  weight without changing the instruction stream has not been found.
* Or make `type` opaque to LVI on the `type>=15` edge for free (the
  original's `type` alloca was probably not SROA-promoted).
