# SpaceLounge::drawLounge + Generator::createMission — wave 132 (w7718): RA eviction-cost model

Status: research note (2026-11-07). Worktree `wt/w7718`, base `70e451ffe`.
**No `src/` change landed**: both targets keep their committed shapes.
* `SpaceLounge::drawLounge` raw **95.676047 %**, 1948 B (unchanged)
* `Generator::createMission` raw **97.200000 %**, 1996 B (unchanged)
* gate: `RATCHET PASS` — byte 4506 / linked 4506, avg 99.94 %, size 4524/4524,
  data 843/843, imports 207, stub/missing/extra 0, lint CLEAN, drift 0

Prior notes: `spacelounge-drawlounge-wave130-w7682.md`,
`generator-createmission-wave127-w7652.md`, `spacelounge-drawlounge-wave129-w7657.md`.
This note supersedes nothing.

## 0. Method: instrumented clang 7.0.2

`/tmp/llvm_android_702b/build2/bin/clang++` (patched `CalcSpillWeights.cpp` /
`RegAllocGreedy.cpp`) prints `FINALDBG` per live interval and honours `VRW`,
`NOHINT`, `NOEVICT`, `HINTVREG`.  This wave extended the `FINALDBG` filter to
`Generator::createMission` too, so both targets can be inspected directly.
`RATRACE` prints the full greedy `select` / `try` / `evict` decision for every
vreg (`RA_TRACE=1`).

Compile scripts and variant generators are in `/dev/shm/w7718/`
(`compile_sl.sh`, `compile_gen.sh`, `score_one.py`, `slvariants.py`,
`genvariants.py`).  `score_one.py` reproduces the official `verify.py` score for
one symbol from the linked `.so`.

## 1. SpaceLounge — why pad can never evict x from r9

Live intervals at the clamp (HEAD, `short pad3` shape):

```text
%1   agent      size 8784  w 2.263368e-04  [464,6064)[6752,9936)
%2   pad        size 6384  w 2.159249e-04  [496,6064)[6752,7568)
%44  lineHeight size  384  w 5.978897e-04  [7312,7696)
%608 x          size 4220  w 2.946140e-04  [6784,6896)[6912,6976)[6976,7168)[7216,7264)[7264,11072)
```

Greedy order is `(hint bit, size)`: hinted intervals first (x has the `$r3`
copy hint from `mov r3, x` at the join), then unhinted by descending size
(pad 6384 before lineHeight 384).  The crucial detail is `tryEvict`
(`RegAllocGreedy.cpp`): the cost of evicting a candidate register is
`(BrokenHints, MaxWeight-of-victim)`, **not** the evictor's weight.  At pad's
allocation the candidates are

```text
r8  agent        cost (0, 2.263368e-04)
r9  x            cost (0, 2.946140e-04)
r11 Canvas %365  cost (0, 2.145276e-04)   <- cheapest
```

so pad always evicts **Canvas from r11**, never x from r9, whatever pad's own
weight is (confirmed by `VRW=pad:1.0..3.0`: pad lands in r11 or splits, never
r9).  lineHeight (`w 5.98e-4`) then evicts pad from r11 because agent
(2.263e-4) is more expensive than pad (2.159e-4).  Hence the residual
`pad=fp, x=r9`.

For pad to land in r9 there are exactly three possibilities:
1. **r9 is free at pad's allocation** — i.e. x must still be in r5.  In the
   trace x is first assigned r5, then `%46 = x - pad` (hinted `$r3`, size 736,
   w 6.25e-4) evicts it from r5 (r5 is a cost-0 register, r9 is cost-1, so the
   cheap-register heuristic prefers r5 over the free r9).  x then takes r9.
   `%46` takes r5 only because `%47 = pad*2 + width` (size 768 > 736) was
   allocated first and occupies r6; `%47` later moves to r10.  If `%46` were
   allocated before `%47` it would take r6 and x would stay in r5.
2. **r11 is occupied by a hinted/heavier interval** at pad's allocation, so
   r9 becomes the cheapest victim.
3. **agent > pad > x** (the wave-130 conclusion): pad can then evict x from
   r9 (r8 agent is too heavy).  But agent (2.263e-4) < x (2.946e-4) today, so
   this needs both agent and pad raised above x.

`NOEVICT=2147484256` (x) makes `%46` take the free r9 instead of evicting x,
and pad then takes r10 — not the target.  `HINTVREG=pad:r9` (raw 75) makes pad
take r9 first, but x is then re-selected to r10 and lineHeight evicts agent
from r8; agent later evicts pad from r9.  `VRW` sweeps over agent×pad
(1.0–2.5 × 1.0–3.0) never produce `FINAL %2 -> $r9`.

### Source shapes measured this wave (all 1948 B unless noted)

| variant | raw % | note |
|---|---:|---|
| HEAD (`short pad3`) | **95.676** | pad=fp, x=r9 |
| faithful `width + pad*2` | 94.0 | right arm exact, pad=fp |
| `width + pad + pad` | 95.5 | pad=fp, same allocation |
| `width + 2*pad` | 94.0 | |
| named `inset` / `right` / `left` | 94.0 | |
| `x = w - inset - width - pad*2` | 94.0 | regrouped |
| `if (inset <= x-pad) { if (...) ... }` | 93.8 | |
| `int boxW = pad*2+width;` before clamp | 89.8 | 1940 B |
| `lineHeight` declared before clamp | 91.6 | 1952 B |
| `lineHeight` declared at top | 72.2 | 1968 B |
| `DrawRectangle(..., lineHeight)` reuse | 87.9 | lineHeight size 4864 |
| `lineHeight` at top + reuse | 82.4 | 1960 B |

**Conclusion (new):** the residual is an eviction-cost fact, not a weight fact.
pad picks r11 because Canvas is the cheapest victim; raising pad's weight alone
cannot move it to r9.  The target needs either x to stay in r5 (so r9 is free)
or r11 to be more expensive than r9 at pad's allocation.  `%46`/`%47` ordering
(736 vs 768) is the only source-adjacent lever found that could keep x in r5,
but every spelling tried leaves `%47` in r6 first.

## 2. Generator — the literal subtraction is 2000 B because rem lands in r9

`FINALDBG` on the literal shape (`bonus = rawBonus - rem`) gives:

```text
%151 agent     size 18832  w 8.679897e-04  [32,19696)
%506 rawBonus  size   272  w 1.563988e-03  [18432,18704)
%509 rem       size   160  w 1.407590e-03  [18544,18704)
%718 bonus     size   148  w 2.379982e-03  (FINAL $r5)
```

At rem's allocation the candidates are `r8 agent (8.68e-4)` (cheapest) and
`r9`; rem's weight (1.41e-3) is above both, so it evicts agent.  Codegen:

```text
orig 9770c  adds r5,r1,r6        ; bonus = rem + rawBonus
     9770e  str  r1,[sp,#32]     ; spill rem
     97718  ldr  r0,[sp,#32]
     9771e  subne r5,r6,r0       ; 16-bit encoding
ours 554    adds r5,r1,r6
     556    mov  r9,r1           ; rem -> r9 (high reg)
     564    subne.w r5,r6,r9     ; 32-bit encoding -> +4 B, frame 104
```

The `subne.w` (high register) is exactly the 4-byte size difference
(2000 vs 1996).  If rem is spilled it is reloaded into `r0` and the encoding is
the 16-bit `subne`, matching the original.

`VRW=2147484157:0.5` (rem weight → 7.04e-4 < agent 8.68e-4) makes
`FINAL %509 -> $noreg` (spilled), confirming the threshold: **rem must weigh
less than agent (8.68e-4)**.  `NOEVICT=2147484157` gives 2004 B with a rotated
allocation, not the target.

### Source spellings measured this wave (all `Generator.cpp` only)

Every literal spelling below canonicalises to the same IR and scores
**83.8 %, 2000 B**:

`bonus = rawBonus - rem`; `bonus = bonus - rem - rem`; `bonus -= rem; bonus -= rem;`;
`bonus = rawBonus + rem - rem - rem`; `bonus = rawBonus + (-rem)`;
`int sum = rawBonus + rem; bonus = sum; ...`; `if (plus % 50 == 0) bonus = plus; else ...`;
`bool round = ...`; `(long)rawBonus - (long)rem`; `unsigned` rem;
`const int rawBonus` / `const int rem`; `int bonus = 0; if ((type|4) != 12)`;
`float bonusFactor`; `Standing *standing`; `int bonusRace`; extra `if (rem == 999999)`.

The current `bonus -= rem * 2` form is the only one that keeps rem in r5
(bonus takes rawBonus's dead r6) and therefore 1996 B.  No spelling found that
both keeps `rawBonus` live across the second `__aeabi_idivmod` (forcing rem to
spill) and keeps the frame at 96 B.

## 3. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7718
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
# instrumented weights / allocation:
CLANG=/tmp/llvm_android_702b/build2/bin/clang++ RA_TRACE=1 \
  /dev/shm/w7718/compile_sl.sh src/game/menu/SpaceLounge.cpp /dev/shm/w7718/sl.o
CLANG=/tmp/llvm_android_702b/build2/bin/clang++ RA_TRACE=1 \
  /dev/shm/w7718/compile_gen.sh src/game/mission/Generator.cpp /dev/shm/w7718/gen.o
# single-symbol score from the linked .so:
python3 /dev/shm/w7718/score_one.py _ZN11SpaceLounge10drawLoungeEv
python3 /dev/shm/w7718/score_one.py _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE
```
