# MGame::OnTouchEnd — wave 169 (w11413): epilogue/chain-placement sweep

Status: analysis only, no source change landed, 2026-11-11. Worktree
`/home/debian/GalaxyOnFire2/wt/w11413`, branch `wave169-w11413`, base
`ec3ceb07c`. Scratch `/dev/shm/w11413`. Target
`_ZN5MGame10OnTouchEndEiiPv`, original 8,080 B @ `0x17a144`.

Entry/exit state (unchanged): raw **3,775**, aligned **1,454**, size
**8,080/8,080**, `.rel.plt` byte-identical (0), gate byte **4507**
linked **4507**, RATCHET PASS.

## Where the residual actually is (byte profile)

Per-256-byte differing-byte profile of the base:

```
17a144..17ad44  0/6/9/0/25/5/10/3/0/3/10/8      (hot prologue: ~matches)
17ad44          164
17ae44..17af44  246/240
17b044..17b144  248/194
17b244          8
17b344..17bd44  206/243/251/239/228/242/239/211/221/239/220
17be44          4
17bf44..       45/8
```

So the first ~3.4 KB already matches the original almost byte-for-byte;
the residual is entirely the `won`/reward-chain half.  The first real
divergence is at `0x17adae`: ours `beq.w 17aef8` (4 B) vs original
`beq.n 17aeac` (2 B), because our `maneuverTouchEnd` block sits 76 bytes
later (the hot `hudTouchFlags == 0` body is in front of it).  Then
`0x17adbe` `b.w` vs `b.n` (+2) and the shared `initHudMenu` tail (+4),
giving a persistent +8 by `0x17adc8`, and +76 after the hot `flags==0`
body at `0x17aeb2`.

## The real structural delta

Original hot region ends at the epilogue `0x17b004` (3,776 B); ours at
`0x17ba22` (6,366 B).  The whole reward chain (2,572 B) is *cold* in the
original (after the epilogue, from `0x17b106`) but *hot* in ours
(`0x17b016`..`0x17ba22`).  The `hudTouchFlags == 0` body is also cold in
the original (`0x17b01a`) but hot inline in ours (`0x17aeb2`).

`b.w` vs `b.n` at `0x17a7f2` is the pivot: the original epilogue at
`0x17b004` is just out of `b.n` range (offset 0x80e > 0x7fe), ours at
`0x17ba22` is far out of range, so both use `b.w`.  Any shape that pulls
the epilogue below ~`0x17b004` flips it to `b.n`, shifting the literal
pool by -2/-4 and making the whole tail differ.

## Measured shapes (all relplt 0 unless noted)

| shape | raw | aligned | size |
|---|---:|---:|---:|
| base | **3,775** | 1,454 | 8,080 |
| `expect(flags==0,0)` | 5,922 | 1,603 | 8,076 |
| `expect(won,0)` | 5,384 | 2,209 | 7,828 (relplt 24) |
| `expect(won,1)` | 4,746 | 1,366 | 8,084 |
| `expect(isInstantActionMission(),0)` | 5,963 | 1,885 | 8,084 (relplt 8) |
| `expect(hudMenuOpen,1)` | 5,963 | 1,808 | 8,068 (relplt 26) |
| `expect(hudMenuOpen,0)` | 6,252 | 1,799 | 8,048 (relplt 22) |
| `expect(hudTouchFlags!=0,1)` | 7,423 | 1,891 | 8,152 |
| `expect(mission->isCampaignMission()&&…==15,0)` | 4,684 | 1,525 | 8,092 |
| `!hudTouchFlags` | 3,775 | 1,454 | 8,080 (identical) |
| `count != 0` in ore-refine loop | 6,635 | 2,257 | 8,064 |
| `count < 0` | 6,071 | 1,636 | 8,064 |
| `amount < 0` | 3,775 | 1,454 | 8,080 (identical) |
| `won` second term truthy (`hasWon()`, no `== true`) | 6,102 | 2,276 | 8,068 |
| `bool Mission::hasWon` (cross-TU) | 6,042 | 1,492 | 8,076 |
| bool `hasWon` + chain in original address order | 6,062 | 1,103 | 8,080 |
| chain in original address order (base types) | 4,696 | 943 | 8,084 |
| `flags==0` arm moved to front of weaponSlot/wingman chain | 5,498 | 1,697 | 8,056 (relplt 22) |
| 40 random 4-cycles of the chain (15 pinned first) | none < 3,775 | | |

### `bool hasWon` is the only shape that puts the epilogue in the right place

Changing `Mission::hasWon` to return `bool` (and dropping the redundant
`== true` / `!= false`) makes the `won` computation byte-identical to the
original and moves the epilogue to `0x17afec` (canary check; stack-chk
`0x17affe`), i.e. only **0x18 (24 B) short** of `0x17b004`.  In that
shape the chain is cold, and the hot region matches the original
instruction-for-instruction from `0x17a7f2` to `0x17af7e` at a constant
-4 offset (the -4 is the `b.n` at `0x17a7f2` + literal pool).

The missing 24 bytes are exactly the `else if (mission->isCampaignMission())`
preamble: original keeps it hot at `0x17afa6` (right after the
instant-action body), ours places it cold at `0x17b05a`.  If that
preamble were hot, the epilogue would land at `0x17b004` and the
`0x17a7f2` branch would become `b.w`, potentially making the whole hot
region byte-identical.  I could not find a source shape that keeps the
preamble hot while the chain is cold: `expect(won,0)` and
`expect(chain-first-arm,0)` both pull the preamble cold too (epilogue
`0x17ab9e`/`0x17af86`).  This is the next lever.

## `.rel.plt`

Still byte-identical (0).  First-use order of OnTouchEnd-only symbols is
`dockToStream, useCloak, maneuverTouchEnd, isInstantActionMission,
resetStartSequenceOver`; all call sites are in the hot region before the
reward chain, so chain permutations keep it intact.  Shapes that rotate
it: `expect(won,0)` (24 B), `expect(hudMenuOpen,*)` (22-26 B),
`flags==0`-arm-first (22 B), `expect(isInstantActionMission(),0)` (8 B).

## Reproduction

```
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5MGame10OnTouchEndEiiPv
cmake --build cmake-build-match --target check -j2
```

Scratch: `/dev/shm/w11413/{cmp.py,cmp2.py,bag.py,prof.py,relplt.py,
chain.py,exp.py,batch2.py,rand4.py,MGame.base.cpp,Mission.h.base,
Mission.cpp.base,orig.dis,ours*.dis}`.
