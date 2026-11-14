# MGame::OnTouchEnd — wave 192, worker w14008

Target: `_ZN5MGame10OnTouchEndEiiPv` @ 0x17a144, size 0x1f90 = 8,080.
Baseline: raw 3,544 / aligned 697 / size 8,080 / insn 2865 vs 2864.
Entry block 0x000..0x486 is byte-identical.

First divergence at +0x486 is the `if (count > 0)` at MGame.cpp:1394 (mining-plant
refine loop), NOT the `while (ship->hasCargo(...))` above it (identical):

    orig 0x17a5ca: f1b9 0f00  cmp.w r9,#0        ; amount != 0
    orig 0x17a5ce: d040       beq.n 0x17a652
    ours 0x17a5ca: f1b9 3fff  cmp.w r9,#0xffffffff ; amount > 0 (count > 0)
    ours 0x17a5ce: dc40       bgt.n 0x17a652

## Cluster table (aligned blocks; n = orig/ours)

| orig off | ours off | n | note |
|---|---|---|---|
| +0x0486 | +0x0486 | 2/2 | `cmp.w r9,#0; beq.n` vs `cmp #-1; bgt.n` — `count > 0` vs `count != 0` (line 1394) |
| +0x04a4 | +0x04a4 | 0/1 | ours extra `add.w r8,sp,#80` (scheduling) |
| +0x0e3e | +0x0e3e | 2/10 | `nextCampaignMission` call shape (~line 1205) |
| +0x0e60 | +0x0e78 | 10/1 | paired with above |
| +0x0f7e | +0x0f82 | 1/16 | insert block |
| +0x0f9a | +0x0fc6 | 0/6 | |
| +0x109e | +0x10da | 21/1 | |
| +0x122e | +0x1234 | 1/64 | literal pool + 95 body before cold block |
| +0x1270 | +0x1312 | 22/1 | |
| +0x15a8 | +0x166a | 46/2 | `ldr r1,[pc,#240]`/`ldr r2,[pc,#244]` vs `ldr r1,[pc,#920]`/`ldr r2,[pc,#920]` |
| +0x166c | +0x16b4 | 35/3 | |
| +0x17cc | +0x17cc | 14/0 | ours missing 14 |
| +0x1894 | +0x1894 | 25/9 | |
| +0x19f4 | +0x19f4 | 1/40 | |
| +0x1a28 | +0x1a28 | 32/0 | ours missing 32 |
| +0x1ade | +0x1ade | 8/1 | |

Raw longest differing runs: +0x1489 n=173, +0x1c63 n=114, +0x16c6 n=92,
+0x1d33 n=87, +0x1798 n=86, +0x1537 n=75, +0x1174 n=71, +0x1bfc n=71,
+0x1338 n=68, +0x0fe6 n=66, +0x0f72 n=65, +0x195c n=64, +0x1449 n=63,
+0x1d9c n=63, +0x10e5 n=62, +0x1041 n=59.

Also: +0x1560 orig `cmp r0,#95; bne.n` vs ours `cmp r0,#96; beq.n` (campaign
clause, tied to #95 ordering); +0x6b4 `revsh` vs `hlt` is literal-pool data.

## Rejected / prior findings (size 8,080 unless noted)
- 95 after 74: 4028/824/8076 (size-fatal).
- 95 after 65: 3551/731; 95 after 81: 3554/887.
- `if (amount != 0) { String(-amount) }` shape: 8,084 (size-fatal).
- cargo `if (count != 0)`: 6644/1325/8084.
- 95 body `initStreamOutPosition` before field_68: 3725/680/8084; after
  departStation: 4011/703/8096.
- else-block setFreelanceMission first: 3546/697.
- weapon-slot `hudMenuOpen=0` before `pauseOpen=0`: 3544/763.

## Plan
1. Try `if (count != 0)` at line 1394 (size-neutral candidate).
2. Then largest clusters: +0x1489 (173 raw), +0x15a8 (46/2 aligned), +0x1560
   (#95 vs #96), +0x0e3e (nextCampaignMission shape), +0x122e (95-body /
   literal-pool placement).
