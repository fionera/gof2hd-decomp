# `Generator::createMission` — wave 119 (w6760): block-placement / RA sweep, no source win

Target: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(original `0x971b8`, 1996 B, `.ARM.extab` LSDA at `0x1a78c4` identical to ours).

Status: **no source change landed.**  The wave-119/w6752 shape (plain `int amount`,
`*(volatile int *) &amount = 0` in the else arm, `volatile double padA` store+load just
before `fromSystemPtr`) remains the best measured variant at **83.6 % (raw 83.6088)**,
1996 B, frame 112.  Every variant tried this pass scored `<= 83.6088`; the remaining
delta is register allocation and block placement, not source shape.

```
before (HEAD):  83.6 %  1996 B  frame 112  .ARM.extab 0x2628c
after:          83.6 %  1996 B  frame 112  .ARM.extab 0x2628c (no source change)
```

Full `check` (with the pristine HEAD source):

```
verify: byte 4505  linked 4505  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

27 sections, file size 2,187,328 B, `.text` `0x1337c8`, `.rodata` `0x41c27`,
`.ARM.extab` `0x2628c` — unchanged from HEAD (this pass only adds this note).

## 1. Where the remaining 16.4 % is

`difflib` over the normalized streams: 727 (orig) vs 725 (ours) instructions, 607 equal.
Masking only intra-function branch targets raises the ratio to **89.1 %**; masking branch
targets + literal-pool/pc-relative operands raises it to **90.4 %**.  So ~5.5 points are
block placement (branch-target offsets) and ~1.2 points are pool/pc offsets; the rest is
real instruction differences (stack slots, registers, merged stores).

The original stack layout (from the Thumb disassembly) is:

```
sp+16 galaxy        sp+20 &stack_chk_guard   sp+24 agent -> difficulty
sp+28 targetStation -> amount               sp+32 status -> mission
sp+36 race -> type                          sp+40 rnd
sp+44..91 four AbyssEngine::String temporaries   sp+92 canary   frame 96
```

Our landed layout is:

```
sp+24 galaxy -> bonus   sp+28 targetStation -> difficulty   sp+32 mission
sp+36 type -> race      sp+92 amount (alloca)   sp+96 padA (alloca)
sp+108 canary           frame 112
```

The original's `amount` is a **spill slot** (`str r0,[sp,#28]`, 16-bit); ours is an
address-taken alloca (`str r0,[sp,#92]`, and `str.w sl,[sp,#92]` because `sl` is a high
register).  The original's `difficulty` lives in **r6**; ours in **r5**.  The original
does not merge the `amount` stores: case 0 has its own `str r0,[sp,#28]`, cases 2/11 share
one, and the `type == 8` arm stores `count+5` before jumping to the join.  Ours sinks all
of them to a single store at the join.

Block-placement deltas (each one shifts many branch-target offsets):

* `getRoutes() == nullptr` block: original is **cold at `0x97586`** (between the `type==15`
  and `type==0` switch arms), reached by `cmp/beq.w`; ours is inline at `0x973f8`, reached
  by `cbz`, and forces an extra `b` after the `type = 13` store.
* `pick == 12` block: original is inline (the `pick != 12` common path branches *over* it,
  `cmp r6,#12; bne`); ours is out-of-line (`cmp r6,#12; beq`), an extra `b`.
* mask check: original threads `type > 14` to the `race < 4` check (`bhi 0x97388`); ours
  threads it directly to `getName` (`bhi 0x9740c`).

## 2. Measurements (all `usize == 1996` unless noted; raw = unrounded ratio)

| variant | raw % | note |
|---|---:|---|
| **HEAD (landed)** | **83.6088** | `volatile double padA` store+load, `*(volatile int*)&amount = 0` |
| plain (no padA, plain `amount = 0`) | 79.1 | 1980 B, frame 96, amount `sp+24`, difficulty `sp+28` |
| plain + amount decl after difficulty | 79.4 | 1984 B |
| plain + all amount stores via `volatile int*` | 71.9 | 1984 B, amount `sp+88` |
| `volatile int amount` + padA | 83.5 | |
| `int amount` + padA, plain `amount = 0` | 70.2 | 1992 B, amount `sp+20` |
| padA `char`/`short`/`int`/`long long`/`float` | 79.6/79.6/79.4/79.5/79.8 | 1996/1996/1992/1996/1992 B |
| padA store without `(void) padA` load | 79.8 | 1992 B |
| padA `= padA` | 80.1 | 1988 B |
| padA `= 1e-9` / `-0.0` | 82.4 | 2004 B |
| padA `= (double) level` | 83.6088 | byte-identical to HEAD |
| padA store/load split across anchors (galaxy/fromSystem/fromSystemPtr/reward/level/clamp) | 82.1–83.6088 | adjacent store+load before `fromSystemPtr` is best |
| padA store twice (adjacent / fromSystem+level / fromSystem+reward) | 79.6/80.0/79.5 | 2004/2008/2008 B |
| `__atomic_signal_fence` at galaxy/fromSystem/fromSystemPtr/level/clamp/reward (landed) | 83.6088 | no code change |
| `__atomic_signal_fence` on plain | 71.9–72.0 | 1984 B |
| padB int (non-volatile) used in level/costs/tail | 71.9 | 1984 B |
| `getRoutes` hint `expect(...,0)` | 63.8 | 1980 B, RA collapses |
| `getRoutes` hint `expect(...,1)` / inverted `!= nullptr` hint | 48.0 | 2036 B |
| inverted `getRoutes() != nullptr` if/else (no hint) | 83.6088 | byte-identical |
| `goto` to end-of-block for the null case | 83.6088 | byte-identical |
| `pick == 12` hint 1 / no hint | 80.9 | loop rotation changes |
| `pick == 12` direct `type = pick` in else | 52.7 | 1988 B |
| `pick == 12` `type = 12` before the index test | 83.6088 | byte-identical |
| `type == 8` arm: `amount` before `difficulty` | 83.3 | amount store moves into the arm but RA shifts |
| `type == 8` arm: volatile amount store | 83.6088 | byte-identical |
| `type == 8` arm: `goto afterAmount` | 83.6088 | byte-identical |
| invert `type == 8` if/else | 74.7 | 1988 B |
| `items` hoisted out of the `type == 8` arm | 78.5 | 1988 B |
| `items` at top / before itemId | 66.5 / 78.5 | |
| `itemId` uninitialised | 78.4 | 1988 B |
| `itemId` declared at top | 57.0 | 2004 B |
| `amount` declared after difficulty / after itemId | 83.1 | 1996 B |
| `amount` before padA declaration | 75.5 | 1996 B |
| `difficulty` declared at top / before agentName | 83.6088 | byte-identical |
| `agentName` moved after difficulty / after itemId / to top | 70.4/70.4/44.4 | |
| `int`/`uint32_t` type, `type < 15`, `0x4801U != 0U` | 83.6088 | byte-identical |
| `race <= 3`, `type == 13 && race < 4` | 83.6088 | byte-identical |
| `(type | 4) == 12` / `type & ~4 == 8` | 83.6088 / 83.3 | |
| `type == 8 || type == 4` | 76.7 | |
| `expect(type == 12, 0/1)`, `expect(difficulty >= 10, 0/1)` | 83.6088 | byte-identical |
| `expect(type == 8, 0/1)` | 47.8 / 45.9 | 2032 / 1996 B |
| `expect(offer == 5, 0/1)` | 63.9 / 40.0 | 1956 / 2020 B |
| `expect(type == 7/9/11, 0)` | 78.7 / 78.4 / 83.2 | |
| `expect(used != nullptr, 0/1)` | 57.2 / 56.2 | |
| `expect(campaign <= 15, 0/1)` | 78.3 | |
| reward distance temp / base temp | 82.0 / 77.7 | |
| reward `1.0f + dist/1200` (commuted) | 83.6088 | byte-identical |
| bonus raw temp / costs temp / rem temp | 83.6088 | byte-identical |
| `used` volatile pointer / inside loop | 47.3 / 46.1 | |
| `sum`/`pick`/`guard` `int32_t`, `pick` unsigned | 83.6088 | byte-identical |
| `i` as `int` in the used-sum loop | 83.3 | |
| `found` as `int` / `char` | 83.3 | |
| `found` do-while / `for(;;)` / `while(true)` | 43.7 | 2032 B |
| `found` loop without `break` | 64.5 | 1988 B |
| `difficulty = rnd->nextInt(campaign <= 15 ? 2 : 9)` | 81.2 | |
| clamp `> 10` / ternary / `10 <= difficulty` | 83.6088 | byte-identical |
| `amount`/`difficulty`/`itemId`/`level`/`guard`/`selected` volatile | 68.6/83.5/47.5/79.5/52.9/59.7 | |
| `galaxy` const/auto/ref, `fromSystemPtr` const/auto | 83.6088 | byte-identical |
| `items` const | 83.6088 | byte-identical |
| reorder `galaxy`/`fromSystem`/`padA`/`fromSystemPtr`/`reward` (all 15 valid orders) | 83.1–83.6088 | `G F P S R` (landed) is best |
| combine neutral forms (`int type`, `race<=3`, `(type|4)==12`, `pad=(double)level`, `t8vol`, `case3/5 swap`, `elseswap`, `pick12first`) | <=83.6088 | none beat HEAD |

The plain (faithful) shape is **79.1 % / 1980 B / frame 96**, with `amount` at `sp+24`
and `difficulty` at `sp+28` — the original has them swapped (`amount` `sp+28`,
`difficulty` `sp+24`).  All declaration/statement permutations tried on the plain shape
leave `amount` at `sp+24`; the spill-slot order cannot be flipped from source.

## 3. Conclusion

The wave-119/w6752 landed shape is a strong local optimum.  The residual gap is:

1. frame 112 vs 96 and the `amount` alloca at `sp+92` vs the original spill slot at `sp+28`
   (the `padA` anchor is what buys the schedule match, at the cost of the frame);
2. `difficulty` in `r5` vs `r6`;
3. three block-placement choices (`getRoutes` cold block, `pick == 12` inline block,
   mask-check threading) that all shift branch-target offsets;
4. store merging of the `amount` assignments (ours single sink store, original per-arm
   stores).

Any source-level fix for (1) would need `amount` to remain a spill slot while the
`difficulty`/`amount` colouring flips; every permutation tried leaves the plain shape at
79.1 % and the `padA` shape at 83.6 %.  No variant measured this pass exceeded
**83.6088 %**.
