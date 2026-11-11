# MGame::OnTouchEnd — wave 167 (w11212): reward-chain 22<->74 and 43<->65 swaps

Status: one source change landed, 2026-11-10. Worktree
`/home/debian/GalaxyOnFire2/wt/w11212`, branch `wave167-w11212`, base
`d00057e6d` (wave-166 docs). Scratch `/dev/shm/w11212`. Target
`_ZN5MGame10OnTouchEndEiiPv`, original 8,080 B @ `0x17a144`.

## Result

| | base (wave 166) | this wave |
|---|---:|---:|
| size | 8,080 | **8,080** (exact) |
| raw same-address diff | 4,082 | **3,775** (-307) |
| aligned (`fn_diff.py`) | 1,018 | 1,454 (+436) |
| `.rel.plt` diff | 0 | **0** (29,392 B byte-identical) |
| `.text` diff | 82,898 | **82,591** (-307) |

`check` green: byte **4507/4524**, linked **4507/4524**, size **4524/4524**,
data **843/843**, imports **207**, lint CLEAN, drift 0, RATCHET PASS.
Symbol parity 5370/5370.

## The landed shape

Two reward-chain block swaps on top of the wave-166 order:

```
wave-166: 15,22,43,42,65,81,74,95,161,110,100,120,126,134,96/127,155,144,162
landed:   15,74,65,42,43,81,22,95,161,110,100,120,126,134,96/127,155,144,162
```

i.e. `22 <-> 74` (block indices 1 and 6) and `43 <-> 65` (indices 2 and 4).
Individually: `22<->74` = 4,079 (-3), `43<->65` = 4,081 (-1); together
4,082 -> **3,775** (-307).  The combination is strongly non-additive: it
re-lays the whole reward-path tail so the `.text` stream realigns.

## What was measured (all size 8,080 / relplt 0 unless noted)

* **Full pairwise swap sweep of the wave-166 order** (153 pairs): the three
  wins above; every other pair is >= 4,082.
* **All 40 random permutations** of the 18 chain blocks: 4,627..4,907, all
  worse than the wave-166 order.
* **Pairwise sweep on top of the landed order** (153 pairs): no pair below
  3,775.
* **Single-block moves on top of the landed order** (306 moves): none below
  3,775.  The landed order is a local optimum for both pairwise swaps and
  single-block moves.

## Epilogue / `.rel.plt` analysis (not landed)

The residual raw is dominated by the shared return epilogue: original
`0x17b004`, ours `0x17ba22` (wave-166 shape).  The `hudTouchFlags == 0` arm
is inline in ours (`0x17aeb2`) but cold in the original (`0x17b01a`), and the
reward chain's first half is hot in ours but cold in the original.  The
4-byte shift starts at the `b.n`/`b.w` at `0x17a7f2` (original `b.w 17b004`,
ours `b.n` to the nearer epilogue), which is why the whole tail differs.

`__builtin_expect(hudTouchFlags == 0, 0)` moves the epilogue to `0x17af80`
(too early by 0x84) and the tail cold: 8,068 B, raw 5,746.  `expect(won, 0)`
is 7,828 B / raw 5,415 and rotates 13 `.rel.plt` slots.  `bool hasWon` is
8,080 B / raw 6,050 / relplt 0 (instruction count exact 2865/2865 but the
epilogue lands at `0x17affe`).  The `-mllvm -block-placement-exit-block-bias`
flag (0..1000) is neutral except 100 (8,072 B / raw 4,682 / relplt 4).

The `.rel.plt` first-use order of the OnTouchEnd-only symbols is
`dockToStream, useCloak, maneuverTouchEnd, isInstantActionMission,
resetStartSequenceOver` (indices 3320..3324); all five call sites are in the
hot region before the reward chain, so chain permutations keep it intact.

## Reproduction

```
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5MGame10OnTouchEndEiiPv
python3 /dev/shm/w11212/relplt.py
cmake --build cmake-build-match --target check -j2
```

Scratch: `/dev/shm/w11212/{chainlib.py,sweep2.py,pairs.py,pairs2.py,
combo.py,moves.py,realdiff.py,regions.py,relplt.py,try.sh,MGame.base.cpp}`.
