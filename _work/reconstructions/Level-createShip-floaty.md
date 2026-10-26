# Level::createShip — float-y shape (wave29-w655)

Target: `_ZN5Level10createShipEiiiP8Waypointbb` (original 2764 B, `0xc0214`).

## Starting point

Master source had `float z`, `int y`, `int x` and compiled to 2776 B / 63.6 %
(verify).  The 12-byte excess was not in the hull-box literal pool: the code
was 16 B long and the tail 4 B short.  The excess came from register
allocation, not from an extra semantic block.

## Evidence from the original

Original disassembly around the `Player` construction:

```
c03c8  ldr  r2,[sp,#48]   ; wy
c03ca  ldr  r3,[sp,#32]   ; jitterY
c03cc  ldr  r0,[sp,#44]   ; wx
c03ce  ldr  r1,[sp,#36]   ; jitterX
c03d0  add  r2,r3
c03d2  ldr  r3,[sp,#40]   ; wz
c03d4  ldr  r4,[sp,#28]   ; jitterZ
c03d6  add  r0,r1
c03d8  movw r1,#20000
c03dc  sub.w fp,r0,r1    ; x (int)
c03e0  add  r3,r4
c03e2  subs r4,r2,r1     ; y (int)
c03e4  sub.w r8,r3,r1    ; z (int)
...
c0412  vcvt.f32.s32 s20,s0   ; x -> float at the fixed-object call
c041a  vcvt.f32.s32 s18,s0   ; y -> float
c0422  vcvt.f32.s32 s16,s0   ; z -> float
```

The fixed-object and fighter constructors take `float` positions, so the
source can declare the locals as floats even though clang keeps the integer
value in a core register until the call.  The master `int y` shape made clang
spill/reload differently and produced the 2776-byte body.

## Change

`int y = wy + jitterY - 20000;` -> `float y = (float) (wy + jitterY - 20000);`

This is the only semantic/source-shape change in `createShip`.  It compiles
to exactly 2764 B and raises verify to **65.9 %** (from 63.6 %).

## Layout anchor

The 12-byte body reduction moved `.text` from `0x1333c0` to `0x1333b0` and
dropped `byte_exact` 1352 -> 1342 through the page-quantized RW/GOT segment.
Two volatile double stores were added to the documented
`createCampaignMission` layout anchor (`layoutPad` 22/23), restoring
`.text = 0x1333c0`, `byte_exact = 1352`, `linked_exact = 4454`.
`createCampaignMission` itself moves 83.1 % -> 83.0 %; it is not linked-exact.

## Branch order experiments (no gain)

The original type-scaling block tests `0x33`, then `0x31`, then `0x2c`
(`c02b0`/`c02c0`/`c02c6`).  All six `if/else` permutations and all six
`switch` permutations were compiled.  Clang normalises the block to
`0x2c, 0x31, 0x33` (or `0x33, 0x2c, 0x31`) regardless of source order; the
best full-function result remained the float-y shape.  The hull-box race
switch already emits race 2 before race 3 in code order, matching the
original `tbh` table (`c094e` race 2, `c09fa` race 3); the `case 3`/`case 2`
source order only affects the literal-pool order and was left alone.

## Remaining differences (register allocation)

The original keeps `type` in `fp`, `status` in `r4`, `ship` in `r9`, and uses
`r6` as the hull-box zero constant.  The current clang allocation uses
`status` in `r6`, spills `type`, keeps `ship` in `fp`, and uses `sl` as the
zero constant.  This is the dominant remaining source of mismatch; the
float-y change fixes the size and the x/y/z region but does not change that
allocation.  `setEmpData` is still scheduled before x/y/z in our build while
the original computes x/y/z first.

## Gate

`cmake --build cmake-build-match --target check -j4`:
`RATCHET PASS`, byte_exact 1352, linked_exact 4454, avg 99.71,
`.text 0x1333c0`, imports 207, data 843/843.
