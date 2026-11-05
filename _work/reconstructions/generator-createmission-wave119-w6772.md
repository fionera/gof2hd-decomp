# `Generator::createMission` — wave 119 (w6772): block-placement / RA sweep, no source win

Target: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(original `0x971b8`, 1996 B, original `.ARM.extab` LSDA record 172 B; landed shape 168 B).

Status: **research note, no source change landed.**  The wave-119/w6752 shape
(`int amount` + `*(volatile int *) &amount = 0`, `volatile double padA` store before
`fromSystemPtr`, the three branch hints) stays at **83.6 % / 1996 B / frame 112 /
`.ARM.extab 0x2628c` (168 B record)**.  Every variant tried this pass is equal or worse.

```
before (HEAD):  83.6 %  1996 B  frame 112  .ARM.extab 0x2628c
after:          83.6 %  1996 B  frame 112  .ARM.extab 0x2628c   (no change)
```

Full `check` at HEAD (unchanged, re-run this pass):

```
verify: byte 4505  linked 4505  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓   drift: 0 field(s) across 0 class(es)
```

27 sections, file size 2,187,328 B, `.text` `0x1337c8` (exact), `.ARM.exidx` `0x6f70`
(exact), `.rodata` `0x41c27`, `.ARM.extab` `0x2628c` (12 B short of the original
`0x26298`; `createShip` 8 B + `createMission` 4 B).

## 1. Exact remaining deltas (base vs original, 75 normalised diff hunks)

The landed shape is instruction-for-instruction identical to the original in the whole
prologue, the destination loop (`0x971b8..0x97340`), the campaign/choice block
(`0x97340..0x97420`), the `type == 8` item-selection loop and the reward chain.  The
residual hunks are only:

1. **frame 112 vs 96** — `padA` (`sp+96`) and the volatile `amount` alloca (`sp+92`)
   replace the original's spill slots; every `[sp,#N]` in the tail is +4/+16.
2. **`pick == 12` block order** — original `cmp r6,#12; bne 0x116` (then-block
   fall-through); ours `beq 0x104` (else-block fall-through).  The
   `__builtin_expect(pick == 12, 0)` hint is what keeps the loop bottom-tested
   (`+0x90`); removing it matches this block but rotates the loop (`+0x8e`, 80.9 %).
3. **shared `type` store block** — original `type = 8` (offer arm) and `type = 13`
   (race arm) share one `str r0,[sp,#36]` block; ours inlines it in the offer arm and
   branches back from the race arm (`str r0,[sp,#36]; b` vs `b 0x240`).
4. **cold `getRoutes() == nullptr` block** — original at `+0x3ce`; ours inline
   (`cbz r0, 0x240`).  This is exactly the 4 B LSDA-record shortfall (168 vs 172).
5. **tail RA** — original keeps `difficulty` in `r6` and spills it to `sp+24`, and
   spills `amount` to `sp+28`; ours keeps `difficulty` in `r5` (spilled to `sp+28`)
   and spills `amount` to `sp+92` (alloca).  The two spill slots are **swapped**.

## 2. Root cause of delta 5: the `amount` store is sunk to the join

The original stores `amount` *inside* each switch case / the `type == 8` block
(`0x974a8`, `0x974b4`, `0x975ca`, `0x97604`, `0x9753e`).  In our build LLVM's
`machine-sink` moves the stores to the common join block (`0x97600`), so `r0` stays
live across the branch and the register allocator gives `difficulty` `r5` (the
just-dead `items` pointer) instead of `r6` (the just-dead `count`).  The original's
inline store frees `r0`, and `difficulty` is copied `r0 -> r1 -> r6` around the
`adds r0,r6,#5`.

Confirmed on the faithful no-`padA` shape (`int amount`, plain `amount = 0`,
frame 96, 1980 B): the store is at the join (`b 0x43e`), `difficulty` is `r5`, and
`amount` spills to `sp+24` while `difficulty` spills to `sp+28` — exactly the
original's two slots **swapped**.

The `volatile` initial store used by the landed shape does not prevent the sinking;
it only forces `amount` to be an alloca at `sp+92`, which is why the frame is 112.

### 2.1 The only known way to get `difficulty` into `r6`

Hoisting `Array<Item *> *items = Globals::items;` out of the `type == 8` branch and
using it in the `type == 8` reward keeps `r5` live across the `else`, so the allocator
gives `difficulty` `r6` (`mov r6,r0`) and stores `amount` inline.  It costs a spill
slot for `items` and shifts every scalar (`targetStation` `sp+28 -> sp+32`, etc.):

| shape | pct | usize | frame | note |
|---|---:|---:|---:|---|
| landed (padA, volatile initial store) | **83.6** | 1996 | 112 | `difficulty` r5, `amount` sp+92 |
| landed + hoisted `items` | 66.9 | 2004 | 112 | `difficulty` r6, but all scalars shift |
| no-padA + hoisted `items` | 63.2 | 1996 | 104 | `difficulty` r6 |
| no-padA + hoisted `items` + no `pick==12` hint | 62.8 | 1992 | 104 | |

This reproduces the w4004 finding: no shape with both `difficulty` in `r6` and the
original's frame/stack layout was found.

## 3. Measurements (all official-size `usize` unless noted; base = 83.6/1996)

### 3.1 Frame / `padA` anchor

| variant | pct | usize | frame | note |
|---|---:|---:|---:|---|
| **landed (`padA` store before `fromSystemPtr`)** | **83.6** | **1996** | **112** | |
| `padA` before `fromSystemPtr`, `(void) padA; padA = 0;` | 83.6 | 1996 | 112 | byte-identical |
| `padA = 0; if (padA == 0) { }` | 83.6 | 1996 | 112 | byte-identical |
| `padA = 0; padA = 0;` | 83.6 | 1996 | 112 | byte-identical |
| `padA = level; (void) padA;` | 83.6 | 1996 | 112 | byte-identical |
| `padA` after `fromSystemPtr` | 83.6 | 1996 | 112 | byte-identical |
| `padA` before reward expression | 83.5 | 1996 | 112 | |
| `padA` after `int fromSystem` | 83.3 | 1996 | 112 | |
| `padA` after `level` | 82.2 | 1996 | 112 | |
| `padA` before `new Mission` | 82.0 | 1996 | 112 | |
| `padA` after `agentName` / in offer else | 81.1 | 1996 | 112 | |
| `padA` at `return mission` | 80.6 | 1996 | 112 | |
| `padA` before loop | 78.2 | 1996 | 112 | |
| `padA` store only (no load) | 79.8 | 1992 | 112 | |
| `padA` load only | 80.1 | 1988 | 112 | |
| `volatile int padA` | 79.4 | 1992 | 104 | |
| `volatile float padA` | 79.5 | 1992 | 104 | |
| `volatile long long padA` | 79.2 | 1996 | 120 | |
| two volatile locals (`double`+`int`) | 70.9–72.7 | 1996 | 112 | |

### 3.2 `amount` / tail shape

| variant | pct | usize | frame | note |
|---|---:|---:|---:|---|
| **landed** | **83.6** | **1996** | **112** | |
| plain `amount = 0` (no volatile store) | 70.2 | 1992 | 112 | stores sunk |
| no `padA`, plain `amount = 0` | 79.1 | 1980 | 96 | stores sunk, slots swapped |
| no `padA`, `volatile int amount` | 71.9 | 1984 | 96 | alloca at sp+88 |
| `volatile` case-8 store only | 83.6 | 1996 | 112 | store still at join |
| `volatile` case-0/3/5, case-2/11 stores | 83.6 | 1996 | 112 | |
| all `amount` stores volatile | 83.5 | 1996 | 112 | |
| `amount` declared late (before `itemId`) | 79.4 | 1984 | 96 | |
| `amount` declared before `targetStation` | 79.1 | 1980 | 96 | |
| `amount` declared before `targetStation` + `padA` | 75.5 | 1996 | 112 | |
| `difficulty` declared early / before `amount` | 83.6 | 1996 | 112 | byte-identical |
| `volatile int difficulty` | 68.6 | 1992 | 112 | |
| `difficulty`/`itemId`/`bonus`/`costs` as `int32_t` | 83.6 | 1996 | 112 | byte-identical |
| `int race` / `int32_t race` | 83.5 | 1996 | 112 | |
| `int selected` | 83.5 | 1996 | 112 | |
| `uint32_t guard` | 56.6 | 1988 | 112 | |
| `int guard = 998 / 1000` | 83.5 | 1996 | 112 | |

### 3.3 Loop / hints

| variant | pct | usize | note |
|---|---:|---:|---|
| **landed (`expect(pick==12,0)`)** | **83.6** | **1996** | loop bottom-tested |
| no `pick==12` hint | 80.9 | 1996 | then-block fall-through, loop rotated (+0x8e) |
| `expect(pick==12,1)` | 80.9 | 1996 | same as no hint |
| `expect(pick-12==0,0)` / `expect(12==pick,0)` | 83.6 | 1996 | byte-identical |
| `if/else` restructure of the pick-12 block | 83.6 | 1996 | byte-identical |
| `selected = true;` before the pick-12 check | 83.6 | 1996 | byte-identical |
| no `pick==10` hint | 53.6 | 1996 | |
| no mask hint | 82.1 | 1996 | |
| mask hint `((1U<<type)&0x4801U)!=0` (other spellings) | 83.6 | 1996 | byte-identical |
| `type==0||type==11||type==14` hint | 82.1 | 1996 | |
| hint `pick==8` (0/1) | 57.5 / 54.4 | 1952 / 2008 | |
| hint `used != nullptr` (0/1) | 56.3 / 55.8 | 2016 / 1996 | |
| hint `used->data()[pick]` (0/1) | 58.5 / 80.9 | 1980 / 1996 | |
| hint `sum == 14` (0/1) | 59.4 / 80.9 | 1968 / 1996 | |
| hint `race > 3` | 54.2 | 2004 | |
| hint `guard < 1` (0/1) | 80.9 / 80.9 | 1996 | |
| hint `!selected` / `selected==0` | 47.2 / 38.1 | 2032 / 1532 | |
| `for (; !selected;)` / `guard--` / `--guard` before `nextInt` | 83.6 | 1996 | byte-identical |

### 3.4 Switch body / tail spellings

| variant | pct | usize | note |
|---|---:|---:|---|
| all 120 source permutations of the 5 switch-case groups | 83.6 | 1996 | byte-identical (LLVM canonicalises) |
| `int tec` temp in the `type == 8` branch | 81.3 | 2000 | |
| `amount` before `getTecLevel` (swap) | 83.3 | 1996 | store inline, `difficulty` still r5 |
| `bonus`/`reward`/`costs` rounding as ternaries | 83.6 | 1996 | byte-identical |
| `if (race < 4) if (type == 13)` | 83.6 | 1996 | byte-identical |
| `if (!getRoutes())` / one-line `type == 12` | 83.6 | 1996 | byte-identical |
| `const String &agentName` | 83.6 | 1996 | byte-identical |
| `Galaxy *galaxy` removed / moved after `fromSystemPtr` | 83.3 | 1996 | |
| `padA` before `galaxy` | 83.5 | 1996 | |

### 3.5 Frame-96 + 1996 B via a global volatile load (no `padA`)

A `volatile` read of `Globals::options[0x37]` adds 16 B without a stack slot, so the
frame stays 96 and the body reaches 1996 B.  The `sp` offsets then match the original,
but the extra load is 4 non-original instructions:

| variant | pct | usize | frame |
|---|---:|---:|---:|
| no-padA + `(void)*(volatile bool*)&Globals::options[0x37]` before clamp | 80.1 | 1996 | 96 |
| same, before reward / at `return` | 80.1 | 1996 | 96 |
| same + no `pick==12` hint | 76.7 | 2000 | 96 |
| same + `volatile int amount` | 70.7 | 2000 | 96 |

## 4. Conclusion

The landed 83.6 % shape is a wide local optimum.  The remaining gap is not a source
spelling: it is the `machine-sink` placement of the `amount` stores (which decides
`difficulty`'s register) and the resulting spill-slot colouring (`amount`/`difficulty`
slots swapped).  The only source lever found for the register is hoisting `items`,
which costs a spill slot and shifts the whole scalar frame (66.9 %).  A byte-exact
shape needs a source form in which the `amount` store stays in each case block
**without** keeping `items` live and **without** forcing `amount` to an alloca.
`volatile` on the store is not enough: LLVM still places the store at the join.

## 5. Reproduction

```sh
git checkout -- src/game/mission/Generator.cpp
cmake --build cmake-build-match --target check -j6     # RATCHET PASS, byte 4505
# fast single-function scorer used for the sweep:
python3 /dev/shm/w6772/fastscore.py                    # pct 83.6, usize 1996
```

Scratch: `/dev/shm/w6772/` (`Generator.base.cpp`, `Generator.nopad.cpp`,
`fastscore.py`, `udiff.py`, `runfile.py`, `sweep_cases.py`, `v_*.cpp`).  Not part of
the repo.
