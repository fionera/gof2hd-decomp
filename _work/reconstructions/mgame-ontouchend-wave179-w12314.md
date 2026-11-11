# MGame::OnTouchEnd — chain-unit reorder + campaign expect (wave 179, w12314)

Status: **landed**. Worktree `wt/w12314` (recovered after the 90-minute timeout),
base master `64014ce6f`. Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.

## Result

| | raw | aligned | size | `.ARM.extab` |
|---|---:|---:|---:|---:|
| base (wave 178) | 3,261 | 1,185 | 8,080 | 700 diff |
| w12315 alone | 3,259 | 1,183 | 8,080 | 700 diff |
| **w12314 candidate on the merged tree** | **3,049** | **1,407** | **8,080** | **709 diff** |

The `.ARM.extab` section stays size- and base-exact (156,312 B @ `0x1a3b78`);
its content diff rises 694 -> 709, which is a net -196 total-byte win
(`.text` 77,080 -> 76,870).

## Shape

The candidate is a **permutation of the 18 top-level chain units** of
`MGame::OnTouchEnd` plus one branch-weight annotation. The unit order is:

```
74,162,43,15,42,22,65,120,96,100,95,110,81,134,144,126,155,161
```

The expect is applied to unit **162** (the `if (mission->isCampaignMission())`
chain block):

```cpp
if (__builtin_expect(mission->isCampaignMission(), 1)) { // lint: arm_builtin
```

Every other unit is emitted with the plain `mission->isCampaignMission()`
expression (the generator strips the expect from all units first, then
re-applies it to the chosen unit).

## Reproduction

The generator is in `scratch/w12314/mkexp.py` / `chainlib.py` (worker scratch,
not tracked). It parses the top-level units of the function, rebuilds them in
the given order, and applies the expect to the chosen unit id:

```sh
cd /home/debian/GalaxyOnFire2/wt/w12314
python3 - <<'PY'
import sys
sys.path.insert(0, '/home/debian/GalaxyOnFire2/scratch/w12314')
import mkexp, lib
order = [74,162,43,15,42,22,65,120,96,100,95,110,81,134,144,126,155,161]
open(lib.MGAME, 'w').write(mkexp.make(expect_id=162, order=order))
PY
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5MGame10OnTouchEndEiiPv
```

The candidate was found by the worker's `sweep_trans_extab.py 162 <order>`
single-transposition sweep over the previous 3,163-raw order; the winning
swap was positions 2/3 (`15` and `43`). The worker timed out before committing;
the source was recovered from `wt/w12314/src/game/menu/MGame.cpp` and applied on
top of the merged wave-179 tree (which also carries w12315's
`MenuTouchWindow::OnTouchEnd` bool change).
