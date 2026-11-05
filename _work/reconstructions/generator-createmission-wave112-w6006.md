# `Generator::createMission` — wave 112 (w6006): MBP / slot-order analysis

Target: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(original `0x971b8`, 1996 B, `.ARM.extab` LSDA record 172 B).

Status: **research note, no source change landed.**  The landed wave-110 shape
(`volatile double padA` declared next to `amount` and assigned before the
reward, `__builtin_expect(pick == 10, 1)`, `__builtin_expect(pick == 12, 0)`)
remains the best measured variant at **82.6 % / 1996 B** (raw 82.620690,
normalised length 727 vs 723).  This pass measured ~70 more source variants
and decoded *why* the remaining block layout differs, using the LLVM 7.0
`MachineBlockPlacement.cpp` source.

Gate at the start and end of this pass (full `check`):

```
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`.ARM.extab` is **0x26298** (exact), 27 sections, file size **2,187,328 B**,
`.text` 0x1337c8 — all unchanged from wave 111.  `git status --short` is
empty after the note commit (the source is byte-identical to wave 110).

## 1. What the remaining 17.4 % actually is

The diff is 83 `SequenceMatcher` chunks over 727/723 normalised instructions
(599 matched).  Chunk histogram:

| shape | count |
|---|---:|
| `replace` 1:1 | **50** |
| `replace` 2:2 | 5 |
| `replace` 1:2 | 3 |
| `replace` 3:2 | 3 |
| `delete` 1:0 | 5 |
| everything else (1–11 instructions) | 17 |

Of the **50** 1:1 replaces, **25 are branch-target-only** (same mnemonic,
`loc_XXX` differs) and 25 are real operand differences.  So the single
largest available win is *block layout*: if the layout matched, ~25 branch
targets would match for free (≈ +3.4 pts).  The other 25 are:

```
str r0,[sp,#92]   vs [sp,#108]     (frame/canary)
ldr r0,[pc,#796]  vs #812          (literal pool, ×4)
str r0,[sp,#28]   vs [sp,#24]      (targetStation/agent slots)
str r5,[sp,#28]   vs [sp,#24]
str.w r9,[sp,#24] vs [sp,#28]
ldrd r9,r4,[sp,#24] vs ldrd r4,r9  (×2)
mov r1,r0         vs mov r5,r0     (difficulty reg in type==8 arm)
str r6,[sp,#24]   vs str r5,[sp,#24]
vmov s2,r6        vs vmov s2,r5
movge r6,#10      vs movge r5,#10
vmov s2,r5        vs vmov s2,r0
str r1,[sp,#32]   vs mov r9,r1     (rem spill)
subne r5,r6,r0    vs subne.w r5,r6,r9
mov r0,r9         vs ldrd r5,r0,[sp,#24]
ldr r2,[sp,#28]   vs [sp,#92]      (amount)
ldr r1,[sp,#28]   vs [sp,#92]
ldr r0,[sp,#92]   vs [sp,#108]
addeq sp,#96      vs #112
vmov s2,r0        vs vldr s2,[sp,#92]
```

Two independent classes: **(a)** the `amount`/`agent`/`targetStation` stack
slot order, **(b)** the `rem` spill, plus the type==8-arm schedule.  The
branch-target-only 25 are downstream of the block layout.

## 2. Block layout: the null block and the shared type store

Both binaries have the *same* block inventory; only the order differs.

Original block starts (function-relative, from branch targets + fallthroughs):

```
0x000 0x1dc(getSystem) 0x1ee(getRoutes) 0x1f4(found=false) 0x23a(loop hdr)
0x23e(movs r0,#13) 0x240(str r0,[sp,#36]) 0x242(getName) ... 0x2f6(else)
0x3c8(case15 tail) 0x3ce(NULL BLOCK) 0x3d4(case0) ...
```

Ours:

```
0x000 0x1f6(getRoutes) 0x1f8(found=false) 0x238(loop hdr) 0x23c(movs r0,#13)
0x23e(b.n 0x19c) 0x240(NULL BLOCK) 0x254(getName) ... 0x19c(shared type store)
```

So **two** blocks are misplaced relative to the original:

1. the `getRoutes()==nullptr` block (ours 0x240, original 0x3ce — after the
   `switch (type)` arms), and
2. the shared `type = pick` store (`str r0,[sp,#36]`): ours is placed as the
   fallthrough of the `type = 8` arm at 0x19c; the original places it as the
   fallthrough of the `type = 13` block at 0x240, right before `getName`.

Both are `MachineBlockPlacement` decisions, not source order.  The original
therefore emits the `lp=0` gap entry (`cs7 @0x3c8 len 0x20`) and
`cmp r0,#0; beq.w` (6 B) instead of our `cbz` (2 B) — the LSDA 172 vs 168
delta and 4 of the 16 B size delta.

### 2.1 The LLVM 7.0 placement rules (read from `release_70`)

`buildChain` grows one chain from the entry.  When a block has no viable
successor it calls `selectBestCandidateBlock(Chain, BlockWorkList)`, which
**scans the whole worklist and keeps the highest block frequency** (first
max wins).  If the worklist is empty it falls back to
`getFirstUnplacedBlock` (first unplaced block in the *current* function
order).  A successor is skipped when
`hasBetterLayoutPredecessor` reports a CFG conflict:

```
if (SuccChain.UnscheduledPredecessors == 0) return false;   // no conflict
...
if (PredEdgeFreq * HotProb >= CandidateEdgeFreq * HotProb.getCompl())
    BadCFGConflict = true;                                  // HotProb = 0.8
```

i.e. the edge to a merge block is rejected when another unplaced predecessor
has frequency `>= 1/4` of the current block's edge frequency.

This explains both misplacements:

* **Shared type store.**  Its predecessors are the `type = 8` arm and the
  `type = 13` block.  In our build `freq(type13) < freq(type8)/4`, so the
  store is merged into the `type = 8` chain (fallthrough at 0x19c).  The
  original must have `freq(type13) >= freq(type8)/4` (or the `type = 13`
  block placed first), so the store is merged into the `type = 13` chain.
* **Null block.**  It is a separate chain (its only successor, `getName`,
  has many predecessors).  Ours is picked by `selectBestCandidateBlock`
  immediately after the loop chain ends, i.e. its frequency is the highest
  remaining.  The original places it after the `switch` arms, i.e. its
  frequency is *lower* than the arm chains'.

Measured block frequencies (propagated from the MIR `successors:` lines;
loops make the absolute values approximate, the ratios are what matter):

```
bb.41 getRoutes     6.1e-05     bb.51 null block   2.3e-05
bb.42 found=false   3.8e-05     bb.52 getName      6.5e-04
bb.49 loop test     3.8e-05     bb.72 switch disp ~1e-05
bb.50 type=13       1.0e-06
```

`getName` is 28× the null block, yet is placed *after* it, because it is
reached through the main chain (`bb.50 -> bb.34 -> bb.52`), not by
`selectBestCandidateBlock`.

### 2.2 What was tried to move the null block

| variant | pct | usize | extab | null @ |
|---|---:|---:|---:|---:|
| baseline | **82.6** | **1996** | 0x26294 | 0x240 |
| `expect(routes==nullptr, 0)` | 62.4 | 2000 | 0x26294 | 0x246 |
| `expect(routes==nullptr, 1)` | 48.2 | 2040 | 0x26294 | 0x726 |
| `!expect(routes!=nullptr, 1)` | 62.4 | 2000 | 0x26294 | 0x246 |
| inverted `if (routes != nullptr) {loop} else {null}` | 82.6 | 1996 | 0x26294 | 0x240 |
| `goto noRoutes` / `goto afterRoutes` | 82.6 | 1996 | 0x26294 | 0x240 |
| `type = 13` hoisted before the `if` | 82.6 | 1996 | 0x26294 | 0x240 |
| `bool noRoutes` temp | 82.6 | 1996 | 0x26294 | 0x240 |
| `expect(agent->getOffer()==5, 0)` | 63.4 | 1972 | 0x26294 | 0x21a |
| `expect(agent->getOffer()==5, 1)` | 38.8 | 2052 | 0x26294 | 0x790 |
| `expect(race<4 && type==13, 1)` | 64.0 | 1968 | 0x26294 | 0x224 |
| `expect(race<4 && type==13, 0)` | 42.2 | 2028 | 0x26294 | 0x766 |

`expect(routes==nullptr,1)` *does* move the null block to 0x726 (past the
switch arms) but the code shape collapses (2040 B, 48.2 %).  No hint puts it
at the original 0x3ce while keeping the rest of the shape.

### 2.3 The pick==12 / loop-rotation coupling

The original's `pick == 12` test is `cmp r6,#12; bne <pick!=12 path>`, i.e.
the `pick == 12` body is the fallthrough (likely).  Our landed
`__builtin_expect(pick == 12, 0)` produces `cmp r6,#12; beq <pick==12 body>`
(inverted).  Removing the hint restores the original branch direction but
rotates the `while (!selected)` loop the other way (condition block first):

```
original / landed:  0x248 body ... 0x2d4 movs.w r0,fp,lsl#31; beq 0x248
plain pick==12:     0x246 movs.w r0,fp,lsl#31; bne exit; 0x24c body ...; b 0x246
```

`plain pick==12` = 79.9 % / 1996 B; `-mllvm -rotation-max-header-size=100`
reproduces the original rotation on that shape (still 79.9 %).  The rotation
is a `LoopRotation` decision, not source.

## 3. Frame and stack-slot map

Original frame **96 B**:

```
[16] bonus (galaxy->visited early)   [44]/[56]/[68] String temps
[20] stack-chk copy                  [80] agentName
[24] agent  -> difficulty            [92] canary
[28] targetStation -> amount
[32] status -> rem -> mission
[36] race -> type
[40] &rnd
```

Ours (baseline) frame **112 B**:

```
[16] fromSystemPtr   [20] stack-chk   [24] targetStation   [28] agent
[32] status          [36] race/type   [40] &rnd            [80] agentName
[92] amount          [96] padA (volatile double)           [108] canary
```

Two independent differences:

* `padA` (8 B) + a dedicated `amount` slot (4 B) + alignment.
* **`agent` and `targetStation` are swapped** relative to the original:
  original `[24]=agent, [28]=targetStation`; ours `[24]=targetStation,
  [28]=agent`.  This is directly visible as `ldrd r9,r4,[sp,#24]` (orig) vs
  `ldrd r4,r9,[sp,#24]` (ours) at 0x14e and 0x16a, and as
  `str.w r9,[sp,#24]` vs `[sp,#28]` at 0x68.

The original reuses `[24]` for `difficulty` and `[28]` for `amount`; ours
reuses them the other way round (when the frame is 96, see §4).

`volatile int amount` is required for the 1996 B size; plain `int amount`
gives 1992 B / 70.5 %.  `volatile double padA` is the only pad type that
keeps both 1996 B and the score (wave-110 sweep).

## 4. Frame-96 family (plain `amount`, no `padA`)

`plain int amount` + no `padA` + `volatile int rem` gives frame **96** with
the *same slot map as the original* (`[16]=galaxy [20]=chk [24]=agent
[28]=targetStation [32]=status [36]=race/type [40]=rnd`), but the code is
16 B short (1980 B) and scores **78.1 %** (raw 78.092605, lenU 720).

Moving the `amount` declaration after `difficulty` gives frame 96 / 1984 B /
**78.4 %**, but the slot *reuse* is still swapped: `[24]` becomes `amount`
and `[28]` becomes `difficulty` (original: `[24]=difficulty`,
`[28]=amount`).

The 16 missing bytes are exactly the four items in §1:

| missing | bytes |
|---|---:|
| `cmp; beq.w` null block instead of `cbz` | +4 |
| type==8 arm stores `amount` in-arm (`str; mov r6,r1`) | +4 |
| `rem` spill (`str r1,[sp,#32]` + reload) instead of `mov r9,r1` | +4 |
| `ldr r0,[sp,#28]` before `vmov s2,r0` (amount reload) | +4 |

All four are RA/MBP decisions.  Adding a `tec` temp to force the type==8
schedule gives the exact original sequence

```
blx getTecLevel; mov r1,r0; adds r0,r6,#5; str r0,[sp,#28]; mov r6,r1
```

but splits the `getTecLevel` try-range into two LSDA entries (`cs3`/`cs4`,
lp `0x754`/`0x752`) so the record grows to 176 B and the score drops to
80.3 % (2000 B).  Declaring `tec` at function scope instead of inside the
`if` keeps extab at 168 B and ties at 82.6 % (but does not change the
schedule — the tail-merge still hoists the store).

## 5. Measured variants (this pass)

All rows are the official scorer after one `link_so`, starting from the
landed wave-110 shape unless noted.  `pct / usize / extab / frame`.

### `rem` and bonus block

| variant | pct | usize | frame |
|---|---:|---:|---:|
| baseline (`int rem`) | **82.6** | **1996** | 112 |
| `volatile int rem` | 78.6 | 1992 | 112 |
| `rem` declared before `rawBonus` | 82.6 | 1996 | 112 |
| `bonus2` temp then `bonus = bonus2` | 82.6 | 1996 | 112 |
| `volatile int bonus` | 71.9 | 1992 | 112 |
| `volatile int difficulty` | 68.6 | 1992 | 112 |
| `volatile int level` | 78.5 | 2004 | 112 |
| `volatile int count` | 82.3 | 1996 | 112 |
| `volatile int itemId` | 46.9 | 2004 | 112 |

### Declaration-position sweep (all tie at raw 82.620690)

`agentName` after `race` (45.0, extab 224 — loses); `mission` at top;
`costs` at top; `level` at top; `bonus` at top; `padA`/`amount` after
`used`; after `selected`; before `race`; `padA`-only at top with `amount`
late.  `itemId` at top = 81.4.  `galaxy` at top = 62.6 / frame 120.

### type==8 arm / amount expression

| variant | pct | usize | extab |
|---|---:|---:|---:|
| baseline | **82.6** | **1996** | 168 |
| `amount = 5 + count` | 82.6 | 1996 | 168 |
| `const int count` | 82.6 | 1996 | 168 |
| `amount = count + 5` before `getTecLevel()` | 82.3 | 1996 | 168 |
| `amount = count; amount += 5` | 81.5 | 1996 | 168 |
| `int tec` temp (in-arm) | 80.3 | 2000 | **176** |
| `int tec` temp (function scope) | 82.6 | 1996 | 168 |
| `Array<Item*> *items` hoisted out of the `if` | 69.9 | 1988 | 168 |
| `items` hoisted to function top | 65.2 | 2000 | 168 |

### pick==12 / loop / getRoutes (re-checked)

| variant | pct | usize |
|---|---:|---:|
| baseline | **82.6** | **1996** |
| `expect` on the inner `getIndex()==25` (0 or 1) | 82.6 | 1996 |
| `expect(pick != 12, 0)` (inverted) | 69.9 | 1992 |
| plain `pick == 12` | 79.9 | 1996 |
| plain + `if (guard < 1) selected = true` | 79.9 | 1996 |
| plain + `expect(guard < 1, 0)` | 79.9 | 1996 |
| plain + `expect(guard < 1, 1)` | 79.9 | 1996 |
| plain + `expect(!selected, 1)` | 42.9 | 2032 |
| plain + `expect(!selected, 0)` | 54.7 | 1968 |

### Switch-case reordering

Reordering `case 15` first or `case 3/5` first is a **no-op** (82.6 /
1996 / null 0x240): MBP re-orders the arms by frequency, so source order of
the cases does not reach the layout.

### `offer == 5` inversion

`if (agent->getOffer() != 5) { ... } else { targetStation=...; type=8; }`
ties at 82.6 / 1996 / null 0x240 — the compiler canonicalises it.

## 6. Proven vs hypothesis

Proven:

* `.ARM.extab` is exact (0x26298) only because the Hud `drawExx` record is
  +4 (ours 480 B / 78 entries vs orig 476 / 77) and cancels the −4 here.
  Any variant that moves this record to 176/164 breaks the budget.
* The LSDA delta is exactly (a) the missing `lp=0` gap entry for the
  out-of-line `getRoutes()==nullptr` block (`cs7 @0x3c8 len 0x20`) and
  (b) the `cs9`/`cs10` split of the reward block (`distance` vs
  `Status::getLevel`, different landing pads).
* The null block and the shared `type = pick` store are placed by
  `MachineBlockPlacement::selectBestCandidateBlock` (highest frequency) and
  `hasBetterLayoutPredecessor` (HotProb 0.8, reject if the other
  predecessor is ≥ 1/4 of the edge frequency).  Source order and `goto`
  forms do not reach either decision.
* `agent`/`targetStation` stack slots are swapped relative to the original
  (`[24]`/`[28]`), independent of `padA`.
* The 16 B size gap of the frame-96 shape is exactly the four missing
  instructions listed in §4.
* The `int tec` temp is the only shape found that reproduces the original
  type==8 schedule, and it costs one extra LSDA call-site entry
  (extab 176).

Hypothesis (not closed):

* A source shape that raises `freq(type13)` to ≥ 1/4 of `freq(type8)` would
  move the shared store to the `type = 13` block (fixing 3 instructions and
  the `b.n 0x19c` target), and a shape that lowers the null block's
  frequency below the switch arms' would move it to ~0x3ce (fixing the
  `lp=0` entry and ~20 instructions).  No plain C++ spelling tried so far
  changes either frequency enough; only `__builtin_expect` does, and it
  costs more than it gains.

## 7. Commands

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
```

Single-function score / side-by-side:

```
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE
```

Scratch files from this pass live under `/dev/shm/w6006/` (`score.py`,
`rawscore.py`, `batch3.py`, `nullpos.py`, `freq.py`, `blocks2.py`,
`lsda2.py`, `dumpdiff.py`, `v1.py`..`v19.py`, `sweep.py`, `o.asm`,
`u.asm`, `u2..u7.asm`, `mi.txt`, `gen.ll`).  They are not part of the repo.
