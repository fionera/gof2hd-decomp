# MGame::OnTouchEnd — wave 171 (w11613): epilogue / reward-chain re-sweep

Status: analysis only, no source change landed, 2026-11-11. Worktree
`/home/debian/GalaxyOnFire2/wt/w11613`, branch `wave169-w11613`, base
`50716a763`. Scratch `/dev/shm/w11613`. Target `_ZN5MGame10OnTouchEndEiiPv`,
original 8,080 B @ `0x17a144`.

Entry/exit state (unchanged): raw **3,775**, aligned **1,454**, size
**8,080/8,080**, `.rel.plt` byte-identical (0), gate byte **4507** linked
**4507**, data 843/843, imports 207, lint CLEAN, drift 0, RATCHET PASS.

## What was measured

### Reward-chain order (18 blocks, base `15,74,65,42,43,81,22,95,161,110,100,120,126,134,96,155,144,162`)

| sweep | candidates | result |
|---|---:|---|
| adjacent swaps | 17 | base is optimum (all >= 3,780) |
| full pairwise swaps | 153 | base optimum (ties at 3,775: `15<->43`, `126<->144`); no strict win |
| full 3-cycles (re-confirm wave-168) | 816 | base optimum |
| random 4-cycles of the 8 cold blocks (`100..162`) | 200 | no hit |
| random 4-cycles of the 10 hot blocks (`15..110`) | 90 | no hit |
| random full permutations | 120 | no hit |

The base order is a strong local optimum for every neighbourhood tried.

### `96/127` split (original has two separate checks sharing one body)

Splitting the combined `(96 || 127)` arm into two `else if` blocks (identical
bodies, tail-merged by clang) and inserting the `127` check at every one of
19 positions: best is raw **4,334** / size **8,088** (127 right after 96).
No split shape is size-exact or beats 3,775.

### `bool Mission::hasWon` route (epilogue at 0x17b004)

Re-confirmed on this base: `bool hasWon` alone is raw **6,042** / size
**8,076**; the hot region matches the original byte-for-byte up to the
`b` at `0x17a7f2`, then the epilogue lands at `0x17afe8` (28 B short of
`0x17b004`) and the `b.n`/literal-pool shift makes the whole tail differ.
The missing hot code is the `else if (mission->isCampaignMission())`
preamble (24 B), which clang keeps cold at `0x17b056`.

Shapes tried on top of `bool hasWon` (all worse than 3,775):

| shape | raw | aligned | size |
|---|---:|---:|---:|
| bool `hasWon` | 6,042 | 1,492 | 8,076 |
| + `expect(isCampaignMission(),1)` | 5,946 | 1,582 | 8,080 |
| + `expect(isInstantActionMission(),0)` | 4,277 | 2,210 | 8,088 |
| + both expects | 4,242 | 2,200 | 8,088 |
| + `expect(isInstantActionMission(),1)` | 5,410 | 2,036 | 7,856 |
| + `expect(won,1)` | 4,748 | 1,387 | 8,088 |
| + `expect(won,0)` | 5,385 | 2,202 | 7,828 |
| campaign-check-first | 3,898 | 1,351 | 8,092 |
| split `if` / inverted `if` | 6,042 | 1,492 | 8,076 |

No source shape moves the `isCampaignMission` preamble hot while keeping the
epilogue at `0x17b004` and the size at 8,080.

### Hot-region residual (0x17a144..0x17ad54, ~79 B)

All but one are branch-offset bytes to the shared epilogue (`0x17b004` orig
vs `0x17ba22` ours) and to forward cold blocks; they are a consequence of the
epilogue placement, not independent source deltas. The one real code delta is
the ore-refine loop at `0x17a5ca`: orig `cmp r9,#0; beq` (`count != 0`) vs
ours `cmp r9,#-1; bgt` (`count > 0`). Writing `count != 0` in the source
re-lays the tail (raw 6,635 / size 8,064, wave-169), so it is not a win.

### `.rel.plt`

Still byte-identical (0). The OnTouchEnd-only first-use order
`dockToStream, useCloak, maneuverTouchEnd, isInstantActionMission,
resetStartSequenceOver` is entirely in the hot region before the reward
chain; every chain permutation above keeps it intact.

## Conclusion

The reward-chain order, the `96/127` split, the `bool hasWon` epilogue route
and the cold-block order are all exhausted on this base. The only remaining
lever is making the `isCampaignMission` preamble hot in the `bool hasWon`
shape (to push the epilogue from `0x17afe8` to `0x17b004`); no source shape
found does it. No change landed.

## Reproduction
```
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5MGame10OnTouchEndEiiPv
cmake --build cmake-build-match --target check -j2
```
Scratch: `/dev/shm/w11613/{perm.py,measure.py,measure2.py,sweep.py,split.py,
trytext.py,trystruct.py,randcold.py,randall.py,prof.py,runs.py,hotdiff.py,
MGame.base.cpp,MGame.split.cpp}`.
