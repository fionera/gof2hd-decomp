# `LevelScript::process` body audit (w13954, wave 192)

Target: `_ZN11LevelScript7processEi` in `src/game/world/LevelScript.cpp`.
Original and ours both place the symbol at `0x137d19`, size **57,232 B**
(`0x137d18..0x145b68`, 20,830 instructions in the original).  Current residual:
raw **53,209**, aligned **18,127**, size **57,232/57,232 exact**
(`docs/IDENTICALITY.md`, wave-192 row).

Question: is the body structurally complete but shifted/reordered, or are whole
mission cases absent?

**Verdict: body complete; no whole cases missing.**  The residual is
instruction reordering, register allocation/stack-slot choice, and the known
prologue register swap -- not missing switch arms.  The 18k aligned delta is
*not* explained by the prologue swap alone (see "Prior prologue experiment").

## Method

1. Disassembled exactly the function's 57,232 bytes from both
   `cmake-build-match/libgof2hdaa.so` and the original with the NDK r18b
   `arm-linux-androideabi-objdump -d`.
2. Normalized branch/call targets and absolute addresses, then compared
   instruction skeletons (mnemonic + operand shape) from the **middle outward**
   at ten 4 KiB probe windows: `0x139000`, `0x13a000`, ..., `0x144000`.
   Starting at the prologue is misleading because the first divergence is a
   two-register prologue swap.
3. Counted and diffed every `bl`/`blx` with a resolved symbol (call sequence and
   per-callee multiplicity).
4. Decoded every `tbh` jump table in the function and compared the union of arm
   targets with the 46 unique source `case` labels.

Scratch scripts: `/home/debian/GalaxyOnFire2/scratch/w13954/{probe.py,tbh.py}`.

## Probe-window table

"Matched" means the normalized instruction sequence at that window is identical;
the block size is the number of consecutive matched instructions found there.
For differing windows, the first differing instruction is shown.

| Probe | Result | First differing instruction (original vs ours) | What it does |
|---|---|---|---|
| `0x139000` | **DIFF**, 19-insn region at orig `0x138fda` / ours `0x13904c` | `bne.w 0x139b1a` vs `bhi.w 0x13d598` | Conditional branch into a state-machine arm; different condition/target, same dispatch family. |
| `0x13a000` | MATCHED (2 insns from `0x13a1ae`) | -- | `ldr r1,[r0,#28]` |
| `0x13b000` | MATCHED (20 insns from `0x13aff0`) | -- | `cmp r0,#0` |
| `0x13c000` | MATCHED (30 insns from `0x13bc70`) | -- | `ldr r0,[pc,#384]` (literal pool load) |
| `0x13d000` | **DIFF**, 7-insn region at orig `0x13cff0` / ours `0x13cdf2` | `ldr.w r4,[sl,#216]` vs `ldr r0,[r0,#4]` | Load a `this`/global field vs load a field through a pointer; register-allocation/field-access ordering difference. |
| `0x13e000` | MATCHED (17 insns from `0x13ea3c`) | -- | `adds r6,r1,r5` |
| `0x13f000` | **DIFF**, 112-insn region at orig `0x13efb0` / ours `0x13ee72` | `mov.w r8,#1` vs `movs r1,#1` | Constant materialization into a different register; same value/role. |
| `0x140000` | **DIFF**, 173-insn region at orig `0x13fe9a` / ours `0x13fa40` | `ldr.w r0,[r8,#4]` vs `ldr r0,[sp,#368]` | Field load vs reload of a stack-spilled value; spill/register allocation difference. |
| `0x142000` | **DIFF**, 2-insn region at orig `0x142000` / ours `0x14202c` | `movs r0,r0` vs `ands r0,r0` | Flag-setting no-op vs flag-setting mask; codegen artifact, value unchanged. |
| `0x144000` | MATCHED (55 insns from `0x143eb6`) | -- | `blx FModSound::play` |

5 of 10 probe windows are exact matches.  The five differences are local
reorder/register-allocation regions of 2--173 instructions, not missing code
blocks.  In particular the 173-insn region at `0x140000` is a spill-slot
difference (`[r8,#4]` vs `[sp,#368]`), not a branch over absent case code.

## Call-sequence evidence

`bl`/`blx` with a resolved symbol:

| | Original | Ours |
|---|---:|---:|
| Calls | **2,340** | **2,352** |
| Distinct callees | **244** | **245** |

Ours has **12 more** calls, not fewer.  A missing case would normally remove
calls; the only new callee is `Player::setGammaHP` (0 -> 1), consistent with a
source-level ordering/state update difference rather than absent code.  Other
per-callee deltas are +/-1--2 and include reordered calls, e.g.
`getPosition`/`Vector=`/`rotate`/`getCurrentCampaignMission` moves from original
`0x138150` to ours `0x138374`; `__stack_chk_fail` moves from `0x144f7c` to
`0x144c8c`; `Explosion::start`/`Explosion::update` are reordered around
original `0x145376`/`0x145ad0`.  No callee class present in the original is
absent from ours.

## Jump-table decode

15 `tbh` sites, **118 arms total**.  These are nested state-machine switches
inside mission bodies, not the top-level mission dispatch.

| Site | Selector | Arms |
|---|---|---:|
| `0x138384` | `r0`, `cmp #11` | 12 |
| `0x13866a` | `r0`, `cmp #4` | 5 |
| `0x138a8a` | `r1 = r0-131`, `cmp #14` | 15 (missions 131..145) |
| `0x138c3e` | `r1 = r0-64`, `cmp #9` | 10 (missions 64..73) |
| `0x13983e` | `r0`, `cmp #3` | 4 |
| `0x139b22` | `r0`, `cmp #9` | 10 |
| `0x13a024` | `r0`, `cmp #6` | 7 |
| `0x13acb6` | `r0`, `cmp #3` | 4 |
| `0x13aeec` | `r1`, `cmp #9` | 10 |
| `0x13cad4` | `r0`, `cmp #3` | 4 |
| `0x13cdf0` | `r1`, `cmp #5` | 6 |
| `0x13e638` | `r1`, `cmp #9` | 10 |
| `0x13e81e` | `r0`, `cmp #5` | 6 |
| `0x13e8f0` | `r0`, `cmp #9` | 10 |
| `0x143d28` | `r0`, `cmp #4` | 5 |

`tbh.py` prints `cmp#None` for the two range-gated tables because they compute
`r1 = r0 - base` before the compare; manual decode gives 15 and 10 arms
respectively.

## Mission-dispatch coverage

The top-level dispatch at `0x138792` first tests campaign mission 42, then the
two range tables (64..73 and 131..145), then the single mission values:

`14, 16, 21, 24, 29, 40, 41, 42, 56, 80, 91, 92, 94, 102, 105, 114, 125, 154,
157, 158`, with 158 falling into its own state machine at `0x138862`.

The source has **46 unique `case` labels**:

`0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 21 24 29 40 41 42 56 64 67 69 70 73
80 91 92 94 102 105 114 125 131 135 139 142 144 145 154 157 158`

The range tables cover 131/135/139/142/144/145 and 64/67/69/70/73; every binary
dispatch target has a source case.  There is **no missing case number**.

## Prior prologue experiment

`levelscript-prologue-wave192.md` showed the first divergence is prologue-only:
`this` and `Globals::globals` are swapped between `sl` and `fp`.  Removing the
local `Level *level` matched the prologue but changed raw 53,203 -> 53,322
(aligned unchanged), so it was not landed.  Therefore the prologue swap is real
but does **not** account for the 18,127-byte aligned residual; the bulk is the
reorder/spill differences above.

## Verdict

**Body complete, prologue-shifted.**  No whole mission case is missing.  The
remaining 18,127 aligned bytes are attributable to local instruction ordering,
register allocation/spill-slot choice, and the prologue register swap.  No
source change was made: the audit found no small missing case to implement.
