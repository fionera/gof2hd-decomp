# LevelScript::process — wave 130 / w7667 (switch-shape audit: the mission-0 switch is 4..15 in ours, 6..15 in the original)

Status: **no source change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, 20169 normalised
instructions. Owned TU: `src/game/world/LevelScript.cpp`. Base master `003a9623a`.
Scratch helpers in `/dev/shm/w7667/` (not tracked).

## Result

```
                         before (master)   after (no code change)
raw ratio (difflib)      60.102467171      60.102467171
official verify pct      60.1              60.1
orig / ours insns        20169 / 20039     20169 / 20039
orig / ours size         57232 / 57232     57232 / 57232    (size-exact)
byte_exact               4506              4506
linked_exact             4506              4506
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

Metric = `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s reconciled streams (`/dev/shm/w7667/score.py`).

## New hypothesis tested: the emitted switch *shape*, not the switch-case order

Wave 128 decoded the two main-switch `tbh` tables and the linear compare chain and
found the **body order already matches**.  This pass decoded **all 15 `tbh` tables**
in the function on both sides (not just the two main ones) and compared the switch
shapes.  Decoder: `/dev/shm/w7667/tbh2.py` (base of a `tbh` is `Align(insn+4, 2)`,
target = base + 2*halfword; entry count taken from the guarding `cmp #N`).

Original `tbh` tables (guard -> case range):

```
0x138384 n=12  m_nState-1, cmp #11   mission 0x4e states 1..12
0x13866a n=5   m_nState-1, cmp #4    mission 89 states 1..5
0x138a8a n=15  main switch 131..145
0x138c3e n=10  main switch 64..73
0x13983e n=4   m_nState,   cmp #3    case 73 states 0..3
0x139b22 n=10  m_nState-6, cmp #9    mission 0 states 6..15      <-- key
0x13a024 n=7   m_nState-1, cmp #6    case 92 states 1..7
0x13acb6 n=4   m_nState,   cmp #3    case 94 states 0..3
0x13aeec n=10  m_nState-1, cmp #9
0x13cad4 n=4   m_nState-2, cmp #3
0x13cdf0 n=6   m_nState-2, cmp #5
0x13e638 n=10  m_nState-1, cmp #9
0x13e81e n=6   m_nState-1, cmp #5
0x13e8f0 n=10  m_nState-1, cmp #9
0x143d28 n=5   m_nState-8, cmp #4
```

Our baseline `tbh` tables: same multiset **except one table**:

```
0x139038 n=12  m_nState-4, cmp #11   mission 0 states 4..15      <-- ours
```

i.e. clang folds the plain `else if (m_nState == 4)` into the following
`switch (m_nState)` (states 6..15) and emits a **12-entry `tbh` over 4..15**,
while the original keeps the switch at **10 entries over 6..15**.  This is a real
structural difference in the mission-0 state machine (the source is
`... else if (m_nState == 3) ... else if (m_nState == 5 && msg->isOver()) ...
else if (m_nState == 4) ... else switch (m_nState) { case 6..15 }`).

Root cause: LLVM's switch formation folds the immediately-preceding
`icmp eq` into the switch when it is the direct predecessor of the switch's
default.  In our source the last `else if` before the switch is the plain
`m_nState == 4`, so it is folded.  In the original the last test before the
switch is the **compound** `m_nState == 5 && msg12->isOver()`, which cannot be
folded, so the switch stays 6..15.

### Original mission-0 compare order (disassembly evidence)

```
1389c8  cmp r1,#4
1389ca  beq.w 138d2a          ; state 4 body (out of line)
1389ce  cmp r1,#3
1389d0  bne.w 138fd8          ; state 3 body is the fall-through (0x1389d4)
...
138fd8  cmp r1,#5
138fda  bne.w 139b1a          ; -> the 6..15 tbh at 0x139b22
138fde  ... msg12->isOver() ...
139b16  ldr r1,[sl,#28]; subs r0,r1,#6; cmp r0,#9; bhi 13f906; tbh
```

So the original source order is **4, 3, 5, switch** and the state-3 body is the
fall-through while state 4 is out of line.

## Permutation sweep of the mission-0 `else if` chain (all measured)

Reordered the three blocks (state 3 / state 5 / state 4) in
`src/game/world/LevelScript.cpp` and rebuilt (`/dev/shm/w7667/reord.py`):

| source order | size | insns | raw % | switch shape |
|---|---:|---:|---:|---|
| **3,5,4 (baseline)** | **57232** | **20039** | **60.102467** | 4..15 (n=12) |
| 3,4,5 | 57264 | 20049 | 57.019245 | 6..15 (n=10) |
| 5,3,4 | 57236 | 20044 | 54.947405 | 4..15 (n=12) |
| 5,4,3 | 57236 | 20044 | 54.947405 | 3..15 (n=13) |
| 4,3,5 (faithful compare order) | 57256 | 20047 | 57.041973 | 6..15 (n=10) |
| 4,5,3 | 57248 | 20048 | 59.129224 | 4..15 (n=12) |

The `4,3,5` order reproduces the original's compare order **and** the 6..15
switch shape, but costs **+24 B** and the fuzzy score drops 3.06 points because
the state-4 body then lands *inline* (ours `0x1389ca`) whereas the original places
it *out of line* at `0x138d2a` with state 3 inline.  Every non-baseline order is
size-changing, so none can land without a compensating -N edit; the baseline is
a strict local optimum on the fuzzy metric.

## `tbh` body order for the two switches whose content differs

Body order by address (case values after the inline entry):

```
mission 0x4e (0x138384 / 0x138302):
  orig  12,5,3,10,2,4,8,6,7,9,11
  ours  12,5,10,3,4,2,8,6,7,9,11      (cases 2,3,4,10 permuted)

mission 0 (0x139b22 / 0x139038):
  orig  10,7,8,9,11,12,13,15,14
  ours  9,15,6,7,8,10,11,12,13,14     (baseline 4..15 table; case 4/5 prepended)
```

The high tables (131..145 and 64..73) are byte-order identical, confirming wave
128.  The remaining permutation is MachineBlockPlacement over the case-body CFG,
not source order (wave 128's dense-`tbb` probe showed source order does not move
the body order).

## Other checks this pass

* Declaration-order sweep of the three prologue locals (`enemies`, `player`,
  `messages`): the baseline `enemies, player, messages` is the only size-57232
  order and the best score (60.102467); the other five orders are 57316/57416 B
  and 58.6–59.2 %.
* `m_pLevel->getPlayer()->rotate(...)` at the mission-1 body (`0x137da6`): the
  original uses the cached `player` at `[sp,#84]`; changing our source to
  `player->rotate(...)` is faithful but grows the function to 57340 B and drops
  the score to 57.39 (global register-allocation perturbation), so it was
  reverted.  This is the same "un-refetch is size-changing" wall as wave 124.

## Open lead for the next wave

Land `4,3,5` **together with a -24 B companion** so the function stays 57232 B.
The wave-129 note lists two faithful negative edits (`case 40 state 4`
`m_nState = 5` is -4 B; `case 157 state 9` `m_pGeometry2->setVisible(false)` is
-4 B) and the wave-124/126 "un-refetch a `Level::getPlayer()` call" family is
-4 B each; four of those plus the two case edits would exactly pay back the +24.
The state-4 body placement (inline vs out-of-line) also has to be checked: the
original's state-4 body is out of line at `0x138d2a`, which may require the
state-4 test to be the first comparison (as in `4,3,5`) plus a branch-weight
difference that C++11 source alone may not express.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
python3 /dev/shm/w7667/score.py                         # 60.102467171
python3 /dev/shm/w7667/tbh2.py <so> <objdump> 0x137d18 0x145ca8
```

Scratch helpers: `score.py` (raw fuzzy ratio), `tbh2.py` (guarded `tbh`
decoder), `tbhctx.py` (8-instruction context before each `tbh`), `opcodes.py`
(difflib replace-run report), `reord.py`/`perm.py` (source-order sweeps),
`try.sh` (build + score), `LevelScript.base.cpp` (pristine baseline copy).
