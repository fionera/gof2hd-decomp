# LevelScript::process — wave 177 / w12103: branch-layout / state-machine statement order
# (landed: 15 independent statement reorders; raw 53,568 -> 53,545)

Target `_ZN11LevelScript7processEi` (original `0x137d18`, **57,232 B = 0xdf90**),
size-exact.  Owned TU `src/game/world/LevelScript.cpp`.  Worktree `wt/w12103`,
branch `wave177-w12103`, base master `9445ac5fb` (wave 176).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w12103/`.

## Result (landed)

```
                         before (base)     after (landed)
process raw diffbytes    53,568            53,545            (-23)
process aligned insns    18,498            18,514            (+16)
orig / ours size         57232 / 57232     57232 / 57232    (size-exact)
orig / ours insns        20830 / 20726     20830 / 20726
.rel.plt                 29392 B ident     29392 B ident    (byte-identical)
.text section diff       79,591            79,568            (-23)
.rodata / .data          SAME / SAME       SAME / SAME
.ARM.extab diff          1,321             1,321            (unchanged)
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
symbol parity            5370/5370         5370/5370
RATCHET                  PASS              PASS
```

`raw` is the same-address byte diff of the function body (literal pools
included), from `tools/fn_diff.py`.  The landed shape keeps the function at
exactly 57,232 B and `.rel.plt` byte-identical.

## 1. Cluster map — where the residual lives

The function is size-exact but **97.8 % of its bytes differ** (only 1,285 of
57,232 bytes are equal at the same address).  The residual is dominated by two
whole-function outcomes, not by local code:

1. **Prologue register permutation (+6 B shift).**  Original: `this -> sl`,
   `&__stack_chk_guard -> fp`, `&Globals::status -> r4` (16-bit `ldr`), status
   value reloaded from `[r4]`.  Ours: `this -> fp`, `&guard -> sl`,
   `&Globals::status -> r9` (32-bit `ldr.w`), status value cached in `r4`, plus
   an extra `mov r0,r4` before each `getCurrentCampaignMission()` call.
   Everything after the prologue is positionally shifted by +6 B until the
   3-ship `enemies` loop absorbs 4 B (`ldr.w r9,[sp,#96]` is missing in ours),
   so the shift becomes +2 B from `0x137f36` on.  This is the single largest
   raw-diff contributor.
2. **Mission-0 state dispatch is a merged jump table.**  Original: compare
   chain `cmp r1,#4 / beq 138d2a`; `cmp r1,#3 / bne 138fd8`; state-3 body at
   `0x1389d4`; `0x138fd8: cmp r1,#5 / bne 139b1a`; state-5 body at `0x138fde`;
   `0x139b16: ldr r1,[sl,#28]; subs r0,r1,#6; cmp r0,#9; bhi 13f906; tbh` ->
   **states 6..15 only (10 entries)**.  Ours: `cmp r1,#5 / beq 138d88`;
   `cmp r1,#4 / bne 139048`; state-4 body at `0x1389bc`; `0x139048:
   subs r0,r1,#3; cmp r0,#12; bhi 13d598; tbh` -> **states 3..15 merged
   (13 entries, entries 4 and 5 dead -> default)**.

Per-region byte diff (base, before this pass):

```
region              diff/size
0x137d18-0x138518   1849/2048   (prologue + mission-0 state 0/1/2)
0x138518-0x1389c8   1148/1200   (mission-0 state 2 tail + literal pool)
0x1389c8-0x139120   1465/1880   (mission-0 dispatch + states 3/4/5)
0x139120-0x13d4ec  16825/17356  (mission-0 switch states 6..15)
0x13d4ec-0x13f906   9009/9242   (mission-0 state 14/15 + state-16 handler)
0x13f906-0x145ca8  24736/25506  (mission 5/26/50/51/78/81/89/106/95..)
```

### Bodies match when the layout matches

The state-3, state-4 and state-5 bodies are **instruction-for-instruction
identical** once the `sl`/`fp` swap is discounted.  Example (state 3,
`0x1389d4` orig vs `0x13906e` ours): same `vmov/vldr/vmul/blx translate`,
same `strd r4,r4,[sl,#144]`, same `strb` store order
(`m_bStartSequence` @0x20 before `field_0x12` @0x12).  The only body-local
difference is that ours reloads `delta` from `[sp,#76]` where the original
keeps it in `r6` (a spill caused by `r9` holding `&Globals::status`).

The mission-0 `case 14` body is at `0x13d4f4` in **both** binaries and is
byte-identical from `0x13d500` (84-byte equal run).  The `case 6` body is
likewise identical apart from `mov r4,sp,#280` being hoisted one slot in ours.
So the switch bodies are correct; only their **block order** differs
(orig layout order `6,10,7,8,9,11,12,13,15,14`; ours
`9,15,6,7,8,10,11,12,13,14`).

## 2. What was tried and rejected (all full link, size + `.rel.plt` checked)

| variant | size | raw | rel.plt |
|---|---:|---:|---|
| **base** | **57232** | **53568** | ident |
| split `if (state==4)` out of the chain, arms `4,3,5` | 57284 | 55561 | DIFF |
| split `if (state==4)`, arms `4,5,3` | 57332 | 55750 | DIFF |
| split `if (state==4)`, arms `3,4,5` | 57276 | 55519 | DIFF |
| switch moved to the front of the `else`, arms `4,5,3` after | 57276 | 55351 | DIFF |
| `switch (m_nState - 6)` + cases renumbered 0..9 | 57232 | 53568 | ident (identical codegen) |
| `switch ((unsigned char)m_nState)` | 57256 | 54811 | DIFF |
| `else if (state >= 6 && state <= 15) { switch }` | 57268 | 55408 | DIFF |
| `if (m_nState >= 16)` instead of `== 16` | 57232 | 53568 | ident (folded) |
| `case 16` moved into the switch | 57256 | 55090 | DIFF |
| switch cases reordered to orig layout `6,10,7,8,9,11,12,13,15,14` | 57240 | 54990 | DIFF |
| switch cases reversed `15..6` | 57240 | 54990 | DIFF |
| `__builtin_expect(state==4,1)` on the dispatch | 57476 | 55816 | DIFF |
| `__builtin_expect(state==3,1)` on the dispatch | 57488 | 55659 | DIFF |
| `__builtin_expect(state==5,1)` on the dispatch | 57220 | 55209 | DIFF |
| `__builtin_expect(state==16,0)` on the post-switch handler | 57252 | 55014 | ident |
| 3-ship loop without the `KIPlayer *e` local | 57336 | 55590 | ident |
| `Level *self` removed from the state-4 body (loads at use sites) | 57232 | 53568 | ident (neutral) |
| `m_nState = 5` moved to the top of the state-4 body | 57232 | 53580 | ident |
| all 24 declaration permutations of `messages/player/enemies/status` | 57232..57316 | 53568..55577 | ident/DIFF |

The state-3/4/5 jump-table merge is not source-controllable: every shape that
breaks it changes the function size, and `switch (m_nState - 6)` is folded back
to the merged 13-entry table.  The prologue permutation is a whole-function
greedy-allocator outcome (waves 157/158/159/173/176 all tried it).

## 3. The landed change: 15 independent statement reorders

All edits are in `src/game/world/LevelScript.cpp`, inside `process()`.  Each
pair is two independent statements (no data dependence; where a declaration is
involved it is still declared before its first use).  The reorder changes the
`-Oz` scheduler's block layout / register pressure and matches the original's
emission order more closely.

| # | body | before | after |
|---:|---|---|---|
| 1 | mission-0 state 1 (`msg6` arm) | `m_nState = 2;` then `wcamera->setTarget(...)` | `setTarget` then `m_nState = 2` |
| 2 | mission-0 state 4 | `Level *self = m_pLevel;` then `sound->stop(...)` | `stop` then `Level *self` |
| 3 | mission-0 state 4 | `player->setComputerControlled(true);` then `player->player->removeAllGuns();` | `removeAllGuns` then `setComputerControlled` |
| 4 | mission-0 state 4 | `m_pCamera->setTarget(...)` then `player->geometry->setRotation(...)` | `setRotation` then `setTarget` |
| 5 | mission-0 state 5 | `sound->play(157,...)` then `Vector zero157{}` | `zero157` then `play(157)` |
| 6 | mission-0 state 5 | `sound->play(158,...)` then `Vector zero158{}` | `zero158` then `play(158)` |
| 7 | mission-0 state 3 | `m_pCamera->setLookAtCam(false);` then `m_pCamera->setTarget(...)` | `setTarget` then `setLookAtCam` |
| 8 | mission-0 case 6 | `updateEvent3DAttributes(161,...)` then `m_nScriptTimerA += delta;` | `+= delta` then `updateEvent3D` |
| 9 | mission-0 case 9 | `m_nScriptTimerA = 0;` then `wcamera->setRumblePercentage(0.0f, 0);` | `setRumble` then `= 0` |
| 10 | mission-0 case 9 | `geometry->setDirection(...)` then `player->rotate(...)` | `rotate` then `setDirection` |
| 11 | mission-0 case 9 | `m_pGeometry5->setPosition(...)` then `m_pCamera->setPosition(...)` | camera `setPosition` then geometry5 `setPosition` |
| 12 | mission-0 case 11 | `m_nState = 12;` then `player->StopEngineSound();` | `StopEngineSound` then `m_nState = 12` |
| 13 | mission-0 case 11 | `player->setVisible(true);` then `player->setExhaustVisible(false);` | `setExhaustVisible` then `setVisible` |
| 14 | mission-0 case 11 | `m_nState = 12;` then `player->field_0x1c = 156;` | `field_0x1c = 156` then `m_nState = 12` |
| 15 | mission-26 pre-switch | `m_nState = 1;` then `Array<KIPlayer*> *landmarks = ...` | `landmarks` then `m_nState = 1` |

Rows 12-14 compose to the final order
`StopEngineSound; field_0x1c = 156; m_nState = 12; PlayEngineSound; ...`
in the `m_nScriptTimerA >= 2501` arm of `case 11`.

Measured incrementally (greedy hill-climb, each accepted only if it was
size-exact + `.rel.plt`-identical + raw-lowering):

```
base                 53568
+ rows 2,3,4,5,6,7   53556   (state-3/4/5 body reorders)
+ row 1              53555
+ row 8              53554
+ row 9              53553
+ row 10             53551
+ row 11             53550
+ row 12             53549
+ row 13             53547
+ row 15             53545   (row 14 is the same edit as 12 on a later line)
```

## 4. Rejected near-wins (size-exact but `.rel.plt` DIFF)

Two further adjacent swaps lower raw but rotate `.rel.plt` (the two PLT
relocations for `PlayerEgo::setFreeLookMode` / `TargetFollowCamera::
enableFirstPersonCam` swap slots at `.rel.plt+0x57c0` / `+0x57c8`), so they are
not landable under the byte-identical-`.rel.plt` gate:

| swap | raw | rel.plt |
|---|---:|---|
| state-4 `setFreeLookMode` <-> `enableFirstPersonCam` | 53541 | DIFF (2 entries) |
| state-4 `enableFirstPersonCam` <-> `hideShipForFirstPersonCameraView` | 53544 | DIFF (2 entries) |

Also rejected as semantically risky despite a 1-byte raw win:
`float rumble = m_nScriptTimerA / 2000.0f;` before `m_nScriptTimerA += delta;`
in `case 10` (the original computes `(timer + delta) / 2000`; the swap reads
the pre-increment value), and `m_tempVec = player->getPosition();` before
`m_tempVec += Vector{...}` (data dependence).

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12103
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# raw= 53545 aligned=18514 size=57232/57232 insn=20726/20830
cmake --build cmake-build-match --target check -j2    # RATCHET PASS
cmake --build cmake-build-match --target lint  -j2    # CLEAN
cmake --build cmake-build-match --target drift -j2    # 0 fields
```

Scratch harnesses (all under `/home/debian/GalaxyOnFire2/scratch/w12103/`):
`measure.sh` (link + fn_diff + per-section SAME/DIFF), `exp.py` (build+measure
one transform), `region.py` (per-region byte diff), `winshift.py` (per-1 KiB
best-shift), `shape.py` (register-normalised instruction diff),
`pairdiff.py` (aligned instruction pairs for an address range),
`sweep_swap.py` / `sweep3.py` (adjacent-statement swap sweeps), `greedy.py`
(greedy hill-climb), `best.py`/`best3.cpp`/`best4.cpp` (the landed shape).

## 6. Open questions

1. **Prologue `&Globals::status` -> r4.**  Getting `this -> sl`, `&guard -> fp`
   and `&Globals::status -> r4` (so `r9` is free for the 3-ship loop's
   `enemies` hoist) would align the mission-0 body and the whole post-tail
   stream.  Every shape measured so far either changes the size (body-local
   `Status *status`, +20 B) or leaves `&status` in `r9`.
2. **Break the state-3/4/5 jump-table merge.**  The original keeps a compare
   chain for 3/4/5 and a 10-entry `tbh` for 6..15.  `switch (m_nState - 6)` is
   folded back to the merged 13-entry table; every shape that does break it is
   size-inexact.
3. **Mission-0 switch block order.**  The bodies are byte-identical when they
   land at the same address (case 14 at `0x13d4f4`), but MachineBlockPlacement
   orders them differently (`9,15,6,7,8,10,...` vs `6,10,7,8,9,...`).
