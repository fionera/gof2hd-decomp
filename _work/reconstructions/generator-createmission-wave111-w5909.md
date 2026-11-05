# `Generator::createMission` — wave 111 (w5909): LSDA gap + frame-96 analysis

Target: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(original `0x971b8`, 1996 B, `.ARM.extab` LSDA record 172 B).

Status: **research note, no source change landed.**  The landed wave-110 shape
(`volatile double padA` declared next to `amount` and assigned before the
reward, `__builtin_expect(pick == 10, 1)`, `__builtin_expect(pick == 12, 0)`)
remains the best measured variant at **82.6 % / 1996 B**.  This pass decoded
the LSDA record exactly, mapped the frame-slot difference, and measured ~55
additional source variants; nothing beat 82.6 %.

Gate at the start and end of this pass (full `check`):

```
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`git status --short` is empty after the note commit (the source is unchanged
from wave 110).  `.ARM.extab` remains **0x26294** (4 B short of the original
0x26298); 27 sections, file size 2,187,328 B, all unchanged.

## 1. The LSDA record, decoded

`/dev/shm/w5909/lsda.py` (scratch) decodes the `.ARM.exidx`/`.ARM.extab`
entry for the function.  Both binaries place the extab entry at the same
`.ARM.extab` offset `0x3d4c`; the LSDA proper starts at `extab+16` and both
headers are `lpenc=ff ttenc=ff csenc=01`.

| | original | ours (baseline) |
|---|---|---|
| call-site entries | **26** | **25** |
| call-site table length | **151** (0x97) | **147** |
| extab record length | **172 B** | **168 B** |
| `.ARM.extab` size | 0x26298 | 0x26294 |

Original table (function-relative offsets):

```
cs0  0x000 len 0x24c lp 0x000   cs13 0x5a0 len 0x008 lp 0x776
cs1  0x24c len 0x01c lp 0x78e   cs14 0x5a8 len 0x00a lp 0x772
cs2  0x27c len 0x032 lp 0x796   cs15 0x5b2 len 0x02a lp 0x77c
cs3  0x2da len 0x014 lp 0x754   cs16 0x5dc len 0x018 lp 0x000
cs4  0x368 len 0x006 lp 0x78e   cs17 0x5f4 len 0x04e lp 0x78c
cs5  0x39a len 0x006 lp 0x792   cs18 0x642 len 0x008 lp 0x75a
cs6  0x3b0 len 0x018 lp 0x756   cs19 0x64a len 0x022 lp 0x000
cs7  0x3c8 len 0x020 lp 0x000   cs20 0x66c len 0x01e lp 0x78c
cs8  0x3e8 len 0x038 lp 0x78e   cs21 0x696 len 0x006 lp 0x794
cs9  0x468 len 0x02c lp 0x790   cs22 0x6bc len 0x004 lp 0x758
cs10 0x4f8 len 0x004 lp 0x778   cs23 0x6de len 0x006 lp 0x770
cs11 0x528 len 0x014 lp 0x77a   cs24 0x6e4 len 0x008 lp 0x760
cs12 0x53c len 0x064 lp 0x000   cs25 0x6ec len 0x0e0 lp 0x000
```

Ours (baseline):

```
cs0  0x000 len 0x25e lp 0x000   cs13 0x5a6 len 0x00a lp 0x76a
cs1  0x25e len 0x01c lp 0x78e   cs14 0x5b4 len 0x028 lp 0x77a
cs2  0x28e len 0x032 lp 0x796   cs15 0x5dc len 0x018 lp 0x000
cs3  0x2ec len 0x012 lp 0x752   cs16 0x5f4 len 0x04e lp 0x770
cs4  0x378 len 0x006 lp 0x768   cs17 0x642 len 0x008 lp 0x756
cs5  0x3aa len 0x006 lp 0x792   cs18 0x64a len 0x022 lp 0x000
cs6  0x3c0 len 0x018 lp 0x754   cs19 0x66c len 0x01e lp 0x78c
cs7  0x3e2 len 0x036 lp 0x768   cs20 0x696 len 0x006 lp 0x794
cs8  0x45e len 0x008 lp 0x78a   cs21 0x6bc len 0x004 lp 0x790
cs9  0x47c len 0x080 lp 0x790   cs22 0x6de len 0x006 lp 0x766
cs10 0x528 len 0x014 lp 0x778   cs23 0x6e4 len 0x008 lp 0x75c
cs11 0x53c len 0x062 lp 0x000   cs24 0x6ec len 0x0e0 lp 0x000
cs12 0x59e len 0x008 lp 0x76e
```

Two independent differences produce the 4-byte record delta:

1. **The missing `lp=0` gap entry.**  The original's `cs7` covers
   `0x3c8..0x3e8` with no landing pad.  That range is the out-of-line
   `getRoutes()==nullptr` block (`0x3ce..0x3e4`, the `nextInt(2)` call) plus
   the tail of the `case 15` arm.  In the original the null block is placed
   **after the `switch (type)` arms** (at `0x97586` = offset 0x3ce), i.e.
   between the `cs6` try-range (`lp 0x756`) and the `cs8` try-range
   (`lp 0x78e`), so `EHStreamer::computeCallSiteTable` emits a
   "potentially-throwing call between try-ranges" entry with `lp=0`.
   In our build the null block is placed at offset **0x240**, *before*
   `agent->getName()` and therefore inside the initial `cs0` (`lp=0`) range,
   so no extra entry is emitted.

2. **The `cs9`/`cs10` split.**  The original has two entries for the reward
   block: `cs9` (`getStation`/`getSystem`/`getStation`/`getSystem`/`distance`,
   `lp 0x790`, len 0x2c) and `cs10` (`Status::getLevel`, `lp 0x778`, len 4).
   The two calls have **different landing-pad labels** (both are `b 0x9794e`,
   the `agentName` destructor stub, but distinct labels), so they do not
   merge.  Our build gives both calls the same landing pad (`lp 0x790`) and
   merges them into one entry of length `0x80` (which also costs a 2-byte
   uleb length instead of two 1-byte lengths).

The `lp=0` entry is 5 encoded bytes; the merged `cs9` is 1 byte longer than
the original's split pair.  Net: 151 - 147 = 4.

The landing pads themselves are all tiny `b 0x9794e` stubs (destroy
`agentName` at `[sp,#80]`); the original has 12 such stubs, ours 13.  The
distinct labels are per-`landingpad`-instruction in the IR, so matching them
requires matching clang's cleanup-block structure, not just the C++ scopes.

## 2. Why the null block is not out of line

The original's block order around the routes check is:

```
0x1dc Agent::getSystem
0x1ea SolarSystem::getRoutes
0x1ee cmp r0,#0
0x1f0 beq.w 0x97586        ; null block, far away
0x1f4 movs r0,#0           ; found = false
0x1f6 b.n 0x973f2          ; routes-exist loop
...
0x23e movs r0,#13
0x240 str r0,[sp,#36]      ; type = 13 (shared with type = 8)
0x242 Agent::getName
...
0x2f6 switch arms (cold)
0x3ce null block: rnd->nextInt(2) ... b 0x973fa
```

Our baseline:

```
0x1ec SolarSystem::getRoutes
0x1f0 cbz r0, 0x973f8      ; null block at 0x240
0x1f2 movs r0,#0
0x1f4 b.n 0x973f0
...
0x23c movs r0,#13
0x23e b.n 0x97354          ; type store shared at 0x19c
0x240 null block: rnd->nextInt(2)
0x254 Agent::getName
```

The null block is *cold* in both (it is the taken branch), but MBP places it
immediately after the routes-exist block in ours and after the switch arms in
the original.  `__builtin_expect(routes == nullptr, 0)` makes the block cold
but does **not** move it past `getName()`: the measured build puts it at
0x246, still inside `cs0` (0..0x264), so the LSDA record stays 168 B (and the
score drops to 62.4 % / 2000 B).  Inverting the if/else, adding
`__builtin_expect(routes != nullptr, 1)`, using a local `SolarSystem *`, or
hoisting `nextInt(2)` all leave the record at 168 B.

## 3. Frame and stack-slot map

Original frame **96 B** (`.ARM.extab`-independent):

```
[sp,16] galaxy            [sp,44] String temp (getRandomName)
[sp,20] stack-chk copy    [sp,56] String temp (target name)
[sp,24] agent / difficulty[sp,68] String temp (Mission ctor copy)
[sp,28] targetStation / amount   [sp,80] agentName
[sp,32] &status / rem / mission  [sp,92] canary
[sp,36] race / type
[sp,40] &rnd
```

Ours (baseline) frame **112 B**:

```
[sp,16] fromSystemPtr     [sp,80] agentName
[sp,20] stack-chk copy    [sp,92] amount
[sp,24] targetStation / difficulty  [sp,96] padA (volatile double)
[sp,28] agent             [sp,108] canary
[sp,32] &status / rem / mission
[sp,36] race / type
[sp,40] &rnd
[sp,44]/[56]/[68] String temps
```

The original keeps `targetStation` in `r4` for the whole function, so its
spill slot `[sp,28]` is dead after the `ldrd r9,r4,[sp,24]` at 0x14e/0x16a;
`StackSlotColoring` then reuses `[sp,28]` for `amount`.  In our build
`difficulty` takes `r4`, `targetStation` is reloaded into `r5` just before
the `Mission` constructor (`ldrd r5,r0,[sp,24]` at 0x5b0), and `amount`
gets a dedicated slot at `[sp,92]`.

The root of the register-allocation cascade is in the bonus block.  The
original spills `rem` to `[sp,32]`:

```
0550 blx __aeabi_idivmod
0554 adds r5,r1,r6        ; bonus = rawBonus + rem
0556 str  r1,[sp,32]      ; spill rem
0558 movs r1,#50
055a mov  r0,r5
055c blx __aeabi_idivmod
0560 ldr  r0,[sp,32]      ; reload rem
0562 cmp  r1,#0
0564 it   ne
0566 subne r5,r6,r0
```

Our build keeps `rem` in `r9` (a callee-saved register):

```
0554 adds r5,r1,r6
0556 mov  r9,r1           ; rem -> r9
0558 movs r1,#50
055a mov  r0,r5
055c blx __aeabi_idivmod
0560 cmp  r1,#0
0562 it   ne
0564 subne.w r5,r6,r9
```

Because `r9` is now `rem`, `agent` is reloaded from the stack at
`agent->getImageParts()` (`ldrd r5,r0,[sp,24]` at 0x5b0) instead of the
original's `mov r0,r9`.  Making `rem` `volatile` forces the spill and flips
the agent/targetStation slot order to the original's
(agent `[24]`, targetStation `[28]`, status `[32]`, race `[36]`, rnd `[40]`),
but it also changes the codegen to `cbz; ldr; sub` and drops the score to
78.6 % / 1992 B.  A non-volatile `rem` is always kept in `r9` regardless of
type (`int`, `short`, `unsigned`, `long long`), declaration position, or
bonus-expression rewrite.

## 4. Measured variants (this pass)

All rows are the official scorer (`tools/verify` normalisation +
`difflib.SequenceMatcher` ratio, rounded to 0.1) after one `link_so`, starting
from the landed wave-110 shape unless noted.  `pct / usize`.

### LSDA / routes block

| variant | pct | usize | extab |
|---|---:|---:|---:|
| baseline | **82.6** | **1996** | 0x26294 |
| `__builtin_expect(routes==nullptr, 0)` | 62.4 | 2000 | 0x26294 |
| inverted if/else + `expect(routes!=nullptr, 1)` | 62.4 | 2000 | 0x26294 |
| inverted if/else + `expect(routes!=nullptr, 1)` (plain) | 48.2 | 2040 | 0x26294 |
| `expect(race<4 && type==13, 0)` | 42.2 | 2028 | 0x26294 |
| `expect(type==13, 0)` inside the `&&` | 42.1 | 2032 | 0x26294 |
| `!routes` pointer-truthy | 82.6 | 1996 | 0x26294 |
| local `SolarSystem *routeSys` | 82.6 | 1996 | 0x26294 |
| `nextInt(2)` hoisted before the check | 76.6 | 2012 | 0x26294 |

No variant moved the null block past `getName()`, so none produced the
`lp=0` entry; every one stayed at 0x26294.

### Frame / slot order

| variant | pct | usize | frame | slots (guard/agent/ts/status/race/rnd) |
|---|---:|---:|---:|---|
| baseline | **82.6** | **1996** | 112 | 20 / 28 / 24 / 32 / 36 / 40 |
| no `padA` | 71.5 | 1984 | 104 | 24 / 32 / 28 / 36 / 40 / 44 |
| `volatile int rem` (padA kept) | 78.6 | 1992 | 112 | 20 / 24 / 28 / 32 / 36 / 40 |
| `volatile int rem` + no padA | 71.1 | 1984 | **96** | 16 / 20 / 24 / 28 / 32 / 36 |
| `volatile int rem` + `volatile int padA` | 78.3 | 1988 | 104 | 20 / 24 / 28 / 32 / 36 / 40 |
| `volatile int rem` + no padA + `volatile int padB` | 78.3 | 1988 | 104 | 20 / 24 / 28 / 32 / 36 / 40 |
| non-volatile `amount` (padA kept) | 70.5 | 1992 | 112 | — |
| non-volatile `amount` + no padA | 70.5 | 1992 | 112 | — |
| `volatile int amount = 0` | 79.9 | 1996 | 112 | — |
| `amount = count+5` before `getTecLevel()` | 82.3 | 1996 | 112 | — |
| same + no padA | 71.2 | 1984 | 104 | — |
| same + `volatile rem` + no padA | 70.8 | 1984 | 96 | — |
| `padA` after `amount` | 75.4 | 1996 | 112 | — |
| `volatile long long padA` | 78.5 | 1996 | 112 | — |

The `volatile int rem` + no-padA row is the only one that reaches frame 96,
but its slots are all shifted by -4 because `galaxy` stays in `r6` and
`fromSystemPtr` takes `[sp,16]` instead of `galaxy`.  Adding a 4-byte pad
(`padB`) restores the `[20..40]` slot order but not the frame (104) and does
not recover the score.

### Register-allocation / declaration sweeps (all 82.6 unless noted)

`race` as `int` (82.5); `targetStation` as `uint32_t`; `level` declared after
`agentName`; `Mission *mission` declared at the top; `int bonus` at the top;
`float reward` at the top; `int rawBonus` at the top; `int bonusRem` at the
top; `bonus` if/else rewrites; `register` on `targetStation`/`amount`;
`Agent *a = agent` alias; `targetStation` declaration split; `ptr_truthy`.

Losing variants: `itemId` at the top (81.4); `level` declared at first use
(80.7); `rawBonus` as `long long` (80.7); `rawBonus` volatile (78.3);
`volatile int rem` (78.6); `volatile int bonus` (71.9); `difficulty`
volatile (68.6); `race` first (63.4); `race` volatile (56.1);
`targetStation` volatile (49.9); `galaxy` first / no local / mixed GOT use
(76.9 / 82.5 / 68.8 / 71.8).

## 5. Proven vs hypothesis

Proven:

* The LSDA delta is exactly the missing `lp=0` gap entry for the out-of-line
  `getRoutes()==nullptr` block, plus the `cs9`/`cs10` landing-pad split.
  The record is 168 B in ours and 172 B in the original; `.ARM.extab` is
  0x26294 vs 0x26298.
* The null block is at offset 0x240 in our build (before `getName`) and at
  0x3ce in the original (after the switch arms).  `__builtin_expect` makes
  it cold but does not move it past `getName()`, so it stays inside `cs0`.
* The original frame is 96 B; `amount` reuses `targetStation`'s `[sp,28]`
  because the original keeps `targetStation` in `r4` and the slot is dead
  after 0x16a.  Ours keeps `difficulty` in `r4` and reloads `targetStation`
  into `r5`.
* The `rem` spill is the RA cascade root: the original spills `rem` to
  `[sp,32]`; ours keeps it in `r9`, forcing `agent` to be reloaded at
  `getImageParts()`.
* `volatile int rem` flips the agent/targetStation slot order to match the
  original but changes the bonus codegen (`cbz; ldr; sub` vs
  `ldr; cmp; it ne; subne`) and loses 4 points.

Hypothesis (not closed):

* A source shape that places the null-routes block after `getName()` (and
  after the switch arms) would both add the `lp=0` entry (making
  `.ARM.extab` exact) and fix the `type = 13` / `getName` block order.  MBP
  block placement, not source order, controls this; no C++ spelling tried
  this pass moves it.
* The `rem` spill and the `r4`/`r5` assignment are downstream of the same
  RA decision.  A source shape that keeps `agent` live in `r9` across the
  bonus block (or raises register pressure there without changing the code
  shape) would collapse the frame to 96 B and likely jump the score
  substantially.

## 6. Commands

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
```

LSDA decode (scratch, not part of the repo):

```
python3 /dev/shm/w5909/lsda.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
```

Single-function score / side-by-side:

```
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE
```

Scratch files from this pass live under `/dev/shm/w5909/` (`lsda.py`,
`batch.py`, `run1.py`..`run15.py`, `o.asm`, `u.asm`, `align2.py`).  They are
not part of the repo.
