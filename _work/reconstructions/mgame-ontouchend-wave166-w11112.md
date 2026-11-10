# MGame::OnTouchEnd — wave 166 (w11112): reward-chain 96/127 <-> 144 swap

Status: one source change landed, 2026-11-10. Worktree
`/home/debian/GalaxyOnFire2/wt/w11112`, branch `wave166-w11112`, base
`85c1f88df` (wave-164 final). Scratch `/dev/shm/w11112`. Target
`_ZN5MGame10OnTouchEndEiiPv`, original 8,080 B @ `0x17a144`.

## Result

| | base | this wave |
|---|---:|---:|
| size | 8,080 | **8,080** (exact) |
| raw same-address diff | 4,087 | **4,082** (-5) |
| aligned (`fn_diff.py`) | 917 | **1,018** (+101) |
| `.rel.plt` diff | 0 | **0** (29,392 B byte-identical) |
| `.text` diff | 83,147 | **83,142** (-5) |
| `.ARM.extab` diff | 659 | **659** (unchanged) |

`check` green: byte **4507/4524**, linked **4507/4524**, size **4524/4524**,
data **843/843**, imports **207**, lint CLEAN, drift 0, RATCHET PASS.
Symbol parity 5370/5370.

## The landed shape

Swap the combined `96 || 127` reward-chain block with the `144` block.  The
current chain order is

```
15,22,43,42,65,81,74,95,161,110,100,120,126,134,144,155,96/127,162
```

and the landed order is

```
15,22,43,42,65,81,74,95,161,110,100,120,126,134,96/127,155,144,162
```

i.e. the `96/127` body moves from position 16 to position 14 and the `144`
body moves from position 14 to position 16 (`155` stays put).  This is a
layout tuning, not the original source order (binary: 96,100,110,120,126,
127,134,144,155,161,162).  It is landed because it is a measured -5 raw win
at exact size with `.rel.plt` byte-identical.

## What was measured (all size 8,080 / relplt 0 unless noted)

### Reward-chain permutations (18 blocks)

* **all 17 adjacent swaps**: none beat 4,087.  Best neutral: `120/126`
  (4,087 / 921).  `110/100` (original order) = 4,125 / 905; `134/144` =
  4,088 / 1,047; `155/96` = 4,093 / 943; `96/162` = 4,206 / 928.
* **pairwise swaps**: none below 4,087.  The first full run used a stale
  block-index parser (the live file was left permuted); a re-run with the
  fixed parser covered ~140 of the 153 pairs before the timebox and found
  no improvement.
* **exhaustive permutation of the last five blocks** (`134,144,155,96,162`):
  best is `134,96,155,144,162` = **4,082 / 1,018** (landed).  Next:
  `134,144,96,155,162` = 4,093 / 943; `134,144,155,96,162` (base) =
  4,087 / 917.
* **all contiguous reversals**: `14..16` (= the landed swap) 4,082;
  `2..4` (43/42/65 -> 65/42/43) 4,086 / 1,311; both together 4,081 / 1,412.
* **single-block moves** (tail blocks 8..17 to every position): none below
  4,087.

The aligned score rises by 101; raw is the byte-identicality ground truth
(wave-164 merge lesson), so the raw win is kept.

### Epilogue / `hudTouchFlags == 0` arm

The shared return epilogue is at `0x17ba22` in ours vs `0x17b004` in the
original (21 branch sites each).  Making the `else if (hudTouchFlags == 0)`
arm cold with `__builtin_expect(..., 0)` does move the body out of line but
re-lays the whole function: 8,068 B, raw 5,720, aligned 1,060.  `!flags`,
`== 0u` and a nested `else { if (...) }` are codegen-identical to the base.
A separate `if (hudTouchFlags == 0)` after the chain is 4,146 / 937 / 8,080.

`expect(failed, 0)` on the current (wave-164) shape is 8,064 B / 5,318 raw /
2,110 aligned / relplt 18; `expect(failed, 1)` is 7,824 B / 5,435 / 2,367 /
relplt 19 — both far worse than the wave-153 variant-C measurement,
so the epilogue-at-`0x17b004` lead does not reproduce on this shape.

### Reward-path predicates

`won` with `!= false`, `!= 0` or truthy: 5,866 / 1,907 / 8,064 (all three
identical).  `keepObjectives` with `== true`, truthy or `&&` form: 4,087 /
917 (neutral).  The `mission = hasWon() ? getMission() : getCampaignMission()`
written as an `if` assignment: 4,673 / 929 / 8,084.

## Reproduction

```
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5MGame10OnTouchEndEiiPv
python3 /dev/shm/w11112/relplt.py
cmake --build cmake-build-match --target check -j2
```

Scratch: `/dev/shm/w11112/{sweep.py,chain.py,regions.py,alignmap.py,
relplt.py,try.sh,MGame.base.cpp}`.  Note: `chain.py` reads `MGame.base.cpp`
(fixed) and writes the live `MGame.cpp`; the first sweep run imported a live
file left in a permuted order, so the pairwise/reversal/single-move numbers
above were re-derived with the fixed parser (the exhaustive tail permutation
is order-independent and was re-run too).
