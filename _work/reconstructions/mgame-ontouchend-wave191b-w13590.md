# MGame::OnTouchEnd — wave191b w13590: block-placement forensics (note only, no raw win)

Target: `_ZN5MGame10OnTouchEndEiiPv` in `src/game/menu/MGame.cpp` (original 8,080 B
@ `0x17a144`, Thumb, NDK r18b clang 7.0.2 `-Oz`). Worktree `wt/w13590`, branch
`wave191b-w13590`, base master `b060a2c04`. Scratch `/home/debian/GalaxyOnFire2/scratch/w13590`.

Prior notes: `mgame-ontouchend-wave191b-w13577.md` (case-100 order, landed),
`mgame-ontouchend-wave191-w13571.md` (pre-chain/block order), `-w13539.md` (95/96/127).

## Result

Base unchanged. `check` stays green (byte_exact 4508/4524, linked_exact 4508/4524,
size-exact 4524/4524, data_exact 843/843, imports 207, lint CLEAN, drift 0,
parity 5370/5370). `MGame::OnTouchEnd` raw **2,878**, aligned **1,661**,
size **8,080/8,080**, insn 2,862/2,865.

## New evidence: the residual is *one* misplaced cold block

Diffing the ordered `blx` call sequences of the original and ours (addresses
rebased to `0x10000`) shows they are **identical** from the function entry all
the way to `0x10f6a`, and identical again from `0x110d6` onward. The **only**
difference is the position of the `else if (soundsMuted)` **PlayEngineSound**
cold block:

| | original | ours |
|---|---|---|
| `PlayEngineSound` block | `0x110a4` (after the 110-arm body, between the 43 and 42 arms) | `0x10f72` (right after the `resetStartSequenceOver` block, before the non-campaign block) |
| `StopEngineSound` block | `0x1145e` | `0x11478` (+0x1a, knock-on of the shift) |
| `ResumeEngineSound` block | `0x10dc2` | `0x10dc2` (identical) |

The sound branch structure itself is byte-identical up to `0x10db8`
(`cmp r1,#0; beq Play_check` / `cmp r2,#0; bne Play`), so the CFG of the
`if (!IsCategoryEnabled(2) && !soundsMuted) Stop; else if (soundsMuted) Play;
else Resume` chain already matches. Only the MachineBlockPlacement decision for
the Play block differs, and it shifts every byte after `0x10f72`
(the `0x10f72..0x1156d` run alone is 1,531 of the 2,878 raw bytes).

Everything that is *not* knock-on shift is accounted for by three local diffs:

1. `0x1048b` (1 B): mining-plant `count` test. Original emits
   `cmp r9,#0; beq skip` (skip when `amount == 0`), ours emits `bgt skip`
   (`count >= 0` -> `amount <= 0`). The original source is therefore
   `if (count != 0)` (equivalently `if (amount != 0)`), **not** `count >= 0`.
   `count != 0` is size 8,088 / raw 6,638 (layout explodes); `count > 0`
   (behaviourally identical because `amount <= 0` always) is size 8,080 / raw 2,880.
2. `0x104a4..0x104ba` (22 B): the `add.w r8,sp,#80` (address of the
   `String("t ")` temporary) is hoisted before the `operator+` call in ours and
   sunk after it in the original. Expression-form variants all lose (see below).
3. `0x1149a`/`0x114cc`: original emits `getStarSystem()->initLight()` **before**
   `enableParticleEffects(...)`; ours emits them in the opposite order. Matching
   the original order (`initLight` first, `particleQuality` after it) is
   size-exact but raw 2,885 (+7).

## Swept and reverted (all measured on this base, 2,878 / 1,661 / 8,080)

- **Pre-chain** (ia-first, all body/nesting permutations incl. `switch=2` before
  `active=0`, `setFreelanceMission` before `showReward`, nested forms):
  4,467..4,497. `nested_camp` (campaign-first with the instant action nested in
  the `else`) is byte-identical to base.
- **Chain-arm order**: all 16 adjacent arm swaps (best tie: swap 2/3, raw 2,878
  / aligned 1,693), 18 rotations and 80 random permutations (w13571) all worse.
- **Statement order**: all 72 single-statement moves in arms 95 and 96; all
  adjacent simple-statement swaps in every one of the 96 `else if` arms in the
  TU; 120 random two-swap combos; 150 random single moves — no win.
- **Pause-menu skip switch**: `case 1` before `case 0` (byte-identical);
  case-0 exact-original order 2,881..2,888; case-1 exact order 8,076 (fatal).
- **Sound block**: arm reorder (`Stop/Resume/Play`, `Stop/Play/Resume`) is
  byte-identical; play-first 8,076; resume-first 8,096; `__builtin_expect` on
  Stop/Play/menu-window/won/pauseOpen/menuTouchOpen all worse or fatal.
  Moving the whole sound if/else to 11 other positions in the `==1` block:
  8,048..8,148.
- **`soundsMuted` declaration**: `bool`/value 8,084; `const&` identical.
- **`initLight`/`enableParticleEffects` order + store order**: 2,885 / 8,140.
- **Mining report expression**: one-line identical; `String t` named 2,916;
  `paren_left` 8,072; split `+=` 8,016; `tmp_assign` 8,092.
- **`__builtin_expect(mission->isCampaignMission(),1)`** added to each of the 18
  chain arms: 3,772..4,754 (arm 162 already has it; adding it there is identical).
- **Block order**: all 6 permutations of the `starMapOpen` / `cutsceneActive` /
  `menuTouchOpen` blocks — only the base order is good; the other five are
  8,060..8,148. `else if` chaining of the same blocks is byte-identical.
- **Declaration moves**: hoisting `FileRead *reader`, `const` on the locals,
  `!= 0` forms of every bool test — all byte-identical.

## Conclusion

The base is a strong local optimum. Every remaining difference is either the
single Play-block placement decision (which no source-order/expect/probability
perturbation tried so far moves to `0x110a4`) or the two local codegen diffs
above, each of which is individually layout-fatal. A future pass should look at
the *literal-pool/`Globals` access* shape or at a different formulation of the
`if (OnTouchEnd(...) == 1)` region, not at statement order.

## Reproduction

Scratch `/home/debian/GalaxyOnFire2/scratch/w13590`: `measure.sh` (build +
`tools/fn_diff.py`), `dump.py`/`align.py`/`summary.py` (disassembly diff),
`mysweep.py` (candidate harness), `armperm.py`, `adj2.py`, `singlemove.py`,
`rand1.py`/`rand2.py`, `armorder2.py`, `blkperm.py`, `pre2.py`/`pre3.py`/`pre4.py`,
`snd2.py`/`snd3.py`/`sndpos.py`, `pq.py`, `stores.py`, `mine2.py`/`mine3.py`,
`expect.py`, `armexp.py`, `swapcase.py`, `combo.py`, `comp8.py`.
