# MGame::OnTouchEnd — wave 164 (w11011): `bool Mission::hasFailed`

Status: one source change landed, 2026-11-08. Worktree
`/home/debian/GalaxyOnFire2/wt/w11011`, branch `wave164-w11011`, base
`b3d3a1f68` (wave-163 docs). Scratch `/dev/shm/w11011`. Target
`_ZN5MGame10OnTouchEndEiiPv`, original 8,080 B @ `0x17a144`.

## Result

| | base | this wave |
|---|---:|---:|
| size | 8,080 | **8,080** (exact) |
| raw same-address diff | 4,127 | **4,125** (-2) |
| aligned (`align_diff.py`) | 906 | **905** (-1) |
| `.rel.plt` diff | 0 | **0** (29,392 B byte-identical) |
| `.text` diff | 84,652 | **84,650** (-2) |
| `.ARM.extab` diff | 725 | **725** (unchanged) |

`check` green: byte **4507/4524**, linked **4507/4524**, size **4524/4524**,
data **843/843**, imports **207**, lint CLEAN, drift 0, RATCHET PASS.
Symbol parity 5370/5370, file size exact 2,187,328 B.

## The landed shape

`Mission::failed` is a `bool` (was `uint8_t`) and `hasFailed()` returns `bool`
(was `unsigned char`):

```cpp
// Mission.h
    bool failed;
    ...
    bool hasFailed();

// Mission.cpp
void Mission::setFailed(bool failed) {
    this->failed = failed;          // was: failed ? 1 : 0
}
bool Mission::hasFailed() { return this->failed; }
```

`_ZN7Mission9hasFailedEv` is 4 B (`ldrb r0,[r0,#4]; bx lr`) either way, so the
function itself stays byte-exact.  The win is at the MGame call site:

```
// original 0x17ab36
  17ab36: 2c00       cmp   r4, #0
  17ab38: f000 8146  beq.w 17adc8
// base (unsigned char hasFailed, `int failed = ...; if (failed == 1)`)
  17ab36: 2c01       cmp   r4, #1
  17ab38: f040 814a  bne.w 17add0
// landed (bool hasFailed)
  17ab36: 2c00       cmp   r4, #0
  17ab38: f000 814a  beq.w 17add0
```

With a `bool` return clang knows the value is 0/1, so `failed == 1` is folded
to `failed != 0` -> `cmp #0; beq`, which is the original's shape (the branch
target still differs: `17add0` vs `17adc8`, a block-placement residual).
The source can stay `int failed = ...; if (failed == 1)`; `bool failed`,
`if (failed)`, `if (failed != 0)` and a direct `if (hasFailed())` all measure
identically (4,125 / 905 / 8,080 / relplt 0).

## Measured alternatives (all `bool hasFailed` unless noted)

| variant | raw | aligned | size | relplt |
|---|---:|---:|---:|---:|
| base | 4,127 | 906 | 8,080 | 0 |
| **landed** | **4,125** | **905** | **8,080** | **0** |
| `bool won` + `bool hasWon` (w10911 canonical) | 6,053 | 1,084 | 8,080 | 0 |
| `bool won` + truthy `hasWon` | 6,053 | 1,084 | 8,080 | 0 |
| drop `== true` on the campaign `hasWon` | 6,082 | 1,907 | 8,064 | 0 |
| `B != 0` / `(bool)B` / `!!B` / `B > 0` | 6,082 | 1,907 | 8,064 | 0 |
| `B == 1` (same as `== true`) | 4,125 | 905 | 8,080 | 0 |
| `int count = -amount; if (amount != 0)` | 6,599 | 1,840 | 8,060 | 0 |
| `if (amount < 0)` (same as `count > 0`) | 4,125 | 905 | 8,080 | 0 |
| `expect(hudTouchFlags == 0, 0)` | 5,728 | 1,048 | 8,068 | 0 |
| `expect(hudTouchFlags == 0, 1)` | 4,125 | 905 | 8,080 | 0 |
| `expect(hudTouchFlags & 0x400, 0)` (w10910 shape) | 4,669 | 1,123 | 8,080 | 0 |
| `expect(mission->isCampaignMission(), 0)` (reward arm) | 4,689 | 929 | 8,084 | 0 |
| `expect(mission->isInstantActionMission(), 0)` | 6,049 | 1,434 | 8,084 | 8 |
| `weaponSlot > -1` / `!= -1`, `wingmanCommand > -1` | 4,125 | 905 | 8,080 | 0 |

## The 23rd `isCampaignMission` call (w10810 source) revisited

The original has **23** `isCampaignMission` calls; the current base has **22**
(the `cm == 96 || cm == 127` arm is merged).  The w10810 split
(`ea0498d1e`, not an ancestor of master) restores the split + the original
chain order but measured 5,816 / 1,379 at 8,080 B.

Combining the w10810 source with `bool hasFailed` gives a much better aligned
shape — **4,677 / 893** — but **8,092 B** (+12).  All formulations of the
`failed` guard (`int`/`bool` local, `if (failed)`, `== 1`, direct call) give
the identical 4,677 / 893 / 8,092.  The bool return changes the call-site
branch width, which re-lays the whole tail and overshoots by 12 B; no -12 B
companion was found.  This is the best aligned number seen for the function
and the top lead for the next wave.

The shared return epilogue is still at `0x17ba18` (original `0x17b004`); the
`hudTouchFlags == 0` arm is still emitted inline at `0x17aeb2` (original keeps
it cold at `0x17b01a`, after the epilogue).  Every size-exact shape that moves
either of those measured worse on aligned.

## Reproduction

```
cmake --build cmake-build-match --target link_so -j8
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5MGame10OnTouchEndEiiPv   # 4125 / 8080
python3 /dev/shm/w11011/ad.py _ZN5MGame10OnTouchEndEiiPv                                        # 905
python3 /dev/shm/w11011/relplt.py                                                               # diff 0
cmake --build cmake-build-match --target check -j8                                               # RATCHET PASS
```

Scratch: `/dev/shm/w11011/{m.sh,batch.py,ad.py,relplt.py,regen.sh,realhunks.py,gaps.py,
t_failed.py,t_matrix.py,t_w10810*.py,t_won.py,t_count.py,t_expect2.py}`.
