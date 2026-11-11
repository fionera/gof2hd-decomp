# MGame::OnTouchEnd — wave 170 (w11513): epilogue/preamble + tail 4-cycle sweep

Status: analysis + one raw-neutral cleanup landed, 2026-11-11. Worktree `/home/debian/GalaxyOnFire2/wt/w11513`,
branch `wave170-w11513`, base `00c403d5a`. Scratch `/dev/shm/w11513`. Target
`_ZN5MGame10OnTouchEndEiiPv`, original 8,080 B @ `0x17a144`.

Entry state: raw **3,775**, aligned **1,454**, size **8,080/8,080**, `.rel.plt`
byte-identical (0), gate byte **4507** linked **4507**, RATCHET PASS.

## What was measured

### Epilogue is the whole residual
The first byte diff is at `0x17a2b4` (five branch targets to the shared epilogue;
orig `0x17b004`, ours `0x17ba22`). The only other prefix diff is the ore-refine
loop scheduling at `0x17a5e8` (`add r0,sp,#116` vs `add.w r8,sp,#80`). Everything
else in `0x17a144..0x17ad44` is byte-identical. The tail (reward chain) is hot in
ours / cold in the original.

### `bool Mission::hasWon` family (size/instruction-exact but raw worse)
Changing `Mission::hasWon` to return `bool` (body stays `ldrb r0,[r0,#5]; bx lr`)
makes the `won` computation match orig (`cmp r0,#0` not `cmp r0,#1`) and puts the
epilogue at `0x17afec` (start) / popeq `0x17affc`, i.e. **24 B short** of
`0x17b004`, size 8,076. Adding `__builtin_expect(hudTouchFlags == 0, 0)` gives
epilogue `0x17afec`/popeq `0x17affc`, size 8,084, raw 5,803. The missing 24 B is
exactly the `else if (mission->isCampaignMission())` preamble (26 B) minus the
`cbz`→`cmp/beq` +2. But the preamble stays cold at `0x17b024`; no source shape
tried moves it hot (`expect(campaign,1)`, local bool, branch swap, switch,
`else`-split all codegen-identical). Because the epilogue is short, the `b` at
`0x17a7f2` stays `b.n`, shifting the literal pool -4 and making the whole hot
tail differ (raw ~5.8-6.1k).

### Shapes measured (all relplt 0 unless noted)
| shape | raw | aligned | size | epilogue popeq |
|---|---:|---:|---:|---:|
| base | **3,775** | 1,454 | 8,080 | 0x17ba32 |
| bool hasWon | 6,042 | 1,492 | 8,076 | 0x17aff8 |
| bool + expect(flags==0,0) | 5,803 | 1,568 | 8,084 | 0x17affc |
| bool + expect(instant,0) | 4,277 | 2,210 | 8,088 (relplt 8) | 0x17b142 |
| bool + expect(instant,0)+campaign,1 | 4,104 | 2,251 | 8,100 (relplt 8) | 0x17b11a |
| bool + expect(won,1) | 4,748 | 1,387 | 8,088 (relplt 18) | 0x17ba3c |
| bool + expect(won,0) | 5,385 | 2,202 | 7,828 (relplt 24) | — |
| base + expect(flags==0,0) | 5,922 | 1,603 | 8,076 | 0x17af90 |
| base + expect(flags==0,0)+campaign,1 | 5,891 | 1,694 | 8,080 | 0x17af90 |
| base + campaign-check-first | 6,071 | 2,293 | 8,084 | 0x17af92 |
| base + swap 0x800/0x400 arms | 3,780 | 1,567 | 8,080 (relplt 2) | 0x17ba32 |
| base + swap 0x200/0x400 arms | 3,782 | 1,569 | 8,080 (relplt 2) | 0x17ba32 |
| base + `won != 0` / truthy | 6,102 | 2,276 | 8,068 | 0x17af92 |
| base + `keepObjectives` truthy | 3,775 | 1,454 | 8,080 | 0x17ba32 (identical) |
| base + `count >= 1` / `amount < 0` | 3,775 | 1,454 | 8,080 | identical |
| base + `refinedAny != false` | 3,775 | 1,454 | 8,080 | identical |

`__builtin_expect_with_probability` is not available in clang 7.0.2.

### Tail 4-cycle sweep
Exhaustive 4-cycles of the last six reward-chain blocks (`126,134,96/127,155,144,162`)
on the base order: **345 candidates, none beat 3,775**. The base order is also a
4-cycle local optimum for the tail.

### `won` branch inversion
Writing `if (!won) { resetStartSequenceOver(); } else { ... }` is codegen-identical
to `if (won) { ... } else { reset; }` (3,775 / 1,454 / 8,080, epilogue 0x17ba32).

## Landed
One raw-neutral cleanup: drop the redundant `!= false` on the `keepObjectives`
`hasWon()` test (3,775 -> 3,775 raw, 8,080 B, `.rel.plt` 0). No epilogue shape
found that both lands the shared epilogue at `0x17b004` and keeps the matched
prefix; the campaign preamble placement is the unresolved lever.

## Reproduction
```
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5MGame10OnTouchEndEiiPv
python3 /dev/shm/w11513/relplt.py
cmake --build cmake-build-match --target check -j2
```
Scratch: `/dev/shm/w11513/{measure2.sh,relplt.py,epi.py,firstdiff.py,firstins.py,
firstlen.py,prefixdiff.py,chainlib.py,search4.py,batch*.py,MGame.base.cpp}`.
