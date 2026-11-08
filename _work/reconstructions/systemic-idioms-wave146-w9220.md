# Systemic idioms across the 17 residual functions — wave 146 / w9220

Status: **analysis + two source changes landed**, 2026-11-08. Worktree `wt/w9220`,
based on `master` `31297fadc` (wave 145).

Targets touched: `src/game/menu/MGame.cpp` (`MGame::OnUpdate`),
`src/game/menu/ModStation.cpp` (`ModStation::ModStation` C1/C2).

## 1. Method

For each of the 17 non-byte-exact functions I disassembled the original and ours
(`arm-linux-androideabi-objdump -d`), normalised every branch target to the
**target instruction index** (so a uniformly shifted but otherwise identical
branch still matches), aligned the two instruction streams with
`difflib.SequenceMatcher`, and classified every non-matching instruction.
Scratch: `/dev/shm/w9220/classify6.py` (alignment), `classify5.py`
(same-address byte runs), `fdiff.py`, `secdiff.py`, `examples.py`.

The same-address byte-run classifier (`classify5.py`) is dominated by cascade:
83% of differing bytes sit on instructions whose opcode differs at the same
address, because an earlier shift moves instruction boundaries. The
instruction-index alignment (`classify6.py`) removes that cascade and is what
the table below uses.

## 2. Ranked pattern table (long tail: all 17 minus Layout C1/C2 and LevelScript)

`operand:ldr` = same `ldr` opcode, memory operand differs (stack slot / literal
pool offset / base register). `extra-*` = instruction present on one side only.

| rank | pattern | funcs | count | candidate source idiom |
|---:|---|---:|---:|---|
| 1 | `operand:ldr` (`[sp,#N]` / `[pc,#N]`) | 11 | 520 | **downstream**: spill-slot or literal-pool layout differs; fix the root, not the load |
| 2 | `operand:b.n` | 11 | 513 | **downstream**: block layout / branch relaxation; fix the block |
| 3 | `extra-orig:ldr` | 10 | 271 | load not hoisted in original — remove the local alias that keeps it alive |
| 4 | `extra-ours:ldr` | 11 | 267 | load hoisted in ours — a local/`const` pointer is cached across a call |
| 5 | `extra-ours:mov` | 11 | 157 | extra copy from a register-allocation tie |
| 6 | `extra-orig/ours:blx` | 7 | 152/149 | call order / call CSE |
| 7 | `extra-orig/ours:ldr.w` | 8-9 | 149/145 | same as 3/4 but 32-bit encoding |
| 8 | `extra-orig/ours:mov` | 11 | 144 | constant materialisation moved by block layout |
| 9 | `extra-orig/ours:movs` | 6 | 130/126 | `movs r0,#0/#1` (bool result) moved |
| 10 | `regalloc:mov` | 10 | 103 | pure register-allocation tie |
| 11 | `operand:cbz` | 9 | 99 | `cbz` target differs (cascade) |
| 12 | `regalloc:ldr.w` | 7 | 86 | register tie on a wide load |
| 13 | `width:ldr/ldr.w` | 5 | 35 | 16- vs 32-bit load: pointer type / offset range / `volatile` |
| 14 | `imm:ldr` | 6 | 25 | literal-pool word differs |

**Root-cause families** (what the downstream rows collapse to):

1. **Global pointer cached in a callee-saved register across a call.**
   The original reloads `Globals::x` (or `&Globals::x`) after every call; ours
   keeps it in `r4/r5/r6` because a local alias (`Status *st`, `Layout &layout`,
   `const float *table`) is live. Candidate idiom: use `Globals::x->` directly
   in each expression. Confirmed on `MGame::OnUpdate` (§3).
2. **Wrong source variable in a call/expression.** The register that survives
   the call identifies which source variable the original passed. Confirmed on
   `ModStation::ModStation` (§4).
3. **Register-allocation ties with no source difference.** The allocator picks
   a different callee-saved register for a value that is dead at the same
   point. These are the `regalloc:*` and `extra-ours:mov` rows; no source
   lever was found for them in this pass.
4. **Literal-pool / spill-slot layout.** `operand:ldr [pc,#N]` and
   `[sp,#N]` are consequences of (1)-(3) and of the number of live locals.
5. **`ldr` vs `ldr.w` width.** Only 35 instructions in the long tail; each is
   a 16-bit offset that crossed the 12-bit unsigned range (or a pointer
   alignment/type difference), not a systematic idiom.

## 3. `MGame::OnUpdate` — landed (raw 41 -> 37, faithful)

`_ZN5MGame8OnUpdateEv`, 11440 B, size-exact. Baseline raw same-address diff
**41** (one run at `+0x17bb`).

The original at `0x17e0b8` is `ldr.w r0,[r9]; blx Status::getPlayingTime`
(i.e. it **reloads** `Globals::status`), and at `0x17e0c0` reloads it again for
`field_0x100`. Ours had `ldr.w r5,[r9]` once and `mov r0,r5` twice, because of
the local `Status *st = Globals::status;` added by wave 9de4c1fb9.

Wave 9de4c1fb9 had **moved** `st` from the mission-122 radio block to the
mission-23 idle block. Restoring the pre-9de4c1fb9 placement (i.e. `st` in the
**mission-122** block, mission-23 using `Globals::status->getPlayingTime()`)
gives:

| variant | size | raw |
|---|---:|---:|
| wave-145 HEAD (`st` in mission-23) | 11440 | 41 |
| **`st` in mission-122 (landed)** | **11440** | **37** |
| no `st` at all | 11392 | 5148 (size-negative) |

The mission-23 block is now instruction-identical to the original. Remaining
37 B: (a) mission-24 `getPlayingTime` pointer lands in `r5` vs original `r2`
(2 B regalloc tie); (b) mission-122 `radioBase` still uses `mov r0,r5`
(cached `st`) where the original reloads `[r9]` (34 B); (c) 1 B branch offset.
Removing `st` entirely fixes (b) but costs 48 B of code (the local is
load-bearing for the whole function's register pressure), so it cannot be
landed without a 48 B companion.

Faithfulness: the original has no `st` local anywhere (it reloads at both
`0x17e0b8` and `0x17e0c0`), so the landed variant is closer but still not the
true source. The true source is variant "no `st`" plus a 48 B companion.

## 4. `ModStation::ModStation` C1/C2 — landed (raw 11 -> 43, **faithful**; 11 is METRIC-ONLY)

`_ZN10ModStationC1Ev`/`C2Ev`, 1008 B, size-exact, aliased. Baseline raw **11**.

The original's first `MatrixSetRotation(nearKey, pitch, yaw, 0, 2)` call
preserves **`r2` (pitch)** in `r4` across the call (`0xd546e mov r4,r2`) and
the second call consumes `r4` as its first float argument
(`0xd54f0 mov r2,r4`). `r2` is the first volatile table load
(`gHangarCamPitch[race]`); `r3` is the second (`gHangarCamYaw[race]`). So the
original's second call is

```cpp
MatrixSetRotation(farKey, pitch, (Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw)[race], 0.0f, 2);
```

not `... farKey, yaw, ...`. Our source had `yaw`. Independent confirmation:
`ModStation::OnInitialize` is **byte-exact** and contains exactly this idiom
(`MatrixSetRotation(camera, pitch, yawTable[race], -0.03f, 2)`), so `pitch`
is the original's variable.

| variant | size | raw |
|---|---:|---:|
| wave-145 HEAD (`yaw`) | 1008 | **11** (METRIC-ONLY, wrong variable) |
| **`pitch` (landed)** | **1008** | **43** |

The 43 B is a pure scheduling/register tie: the allocator puts the
`gHangarCamYawIPad` base in `r4` and `gHangarCamYaw` in `r3` (original: the
opposite), which moves `mov r4,r2` before the second table load. Tried and
measured (all 1008 B / 43): `!Globals::iPad ? yaw : yawIPad`, an `if/else`
assignment to `yawTable`, swapping the `float pitch`/`float yaw` declaration
order, and inlining the ternary in `yawBits`. Inlining the ternary into
`yawBits` as well (dropping the `yawTable` local) is 1000 B; using
`yawTable[race]` in the second call is 984 B. The base-register tie is not
reachable from source in this pass.

## 5. `MenuTouchWindow::OnTouchBegin` — analysed, not landed (28 B)

`_ZN15MenuTouchWindow12OnTouchBeginEiiPv`, 1896 B, size-exact. Raw **28**.
Two clusters, both one root: the scrollbar block at source lines 2347-2352.

The original reloads `Globals::layout` after the `GetImage2DHeight` call
(`0x12a218 ldr r2,[r2,#0]`) and uses that one pointer for both
`field_0xc_leftMargin` and `field_0x20_top`. Ours caches `Globals::layout` in
`r6` (from the local `Layout &layout = *Globals::layout;`) and loads
`field_0xc_leftMargin` before the call; it also uses `ip` instead of `r4` for
`Globals::w` at `0x129fd2` because `r4` is live in ours.

Tested variants (all keep 1896 B except where noted):

| variant | size | raw |
|---|---:|---:|
| HEAD | 1896 | **28** |
| `layout.field_0xc_leftMargin` -> `Globals::layout->...` | 1892 | 358 |
| all `layout.` -> `Globals::layout->` (drop local ref) | 1892 | 358 |
| first `layout.buttonInsetX` -> global only | 1896 | 28 |

So the local reference is load-bearing for the 4 B (the reference spill), and
changing the second condition alone is size-negative. Not landed.

## 6. Gate (after)

```
compared 4524  avg 99.93%  byte_exact 4507  linked_exact 4507  fuzzy_perfect 4507
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Section deltas vs the original: `.text` **85,679** differing bytes
(baseline 85,651; +28 = ModStation C1/C2 +64, MGame -4, C1/C2 aliased so
counted once in `.text`), `.ARM.extab` **1,370** (unchanged), `.rel.plt` **0**,
`.note.gnu.build-id` 20. All 27 section sizes exact; file size exact.

17-function raw same-address diff total: **91,426** (baseline 91,366; +60 =
ModStation +64, MGame -4).

## 7. Open questions

1. `ModStation` C1/C2: break the `gHangarCamYawIPad`/`gHangarCamYaw` base-register
   tie (r3/r4 swap) to get the faithful `pitch` source down to ~11 B. The
   literal order is fixed by `.data` (swapping the arrays breaks data
   byte-exactness), so this must come from a source form that changes the
   allocator's preference.
2. `MGame::OnUpdate`: find the 48 B companion that lets the `st` local be
   removed entirely (true source), or make the mission-122 `radioBase` reload
   `Globals::status` while keeping the 11440 B size.
3. `MenuTouchWindow`: find a source form that keeps the `layout` reference
   spill (1896 B) but makes the second condition reload `Globals::layout`.
4. The `operand:ldr [pc,#N]` / `[sp,#N]` rows are cascade; they should be
   re-measured after each root fix, not swept directly.
