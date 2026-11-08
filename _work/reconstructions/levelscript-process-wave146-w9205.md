# LevelScript::process — wave 146 / w9205: same-address diff-run census + largest-run attack

Status: **tool + census landed; largest-run reconstruction is a layout artifact
(no source win found)**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, size-exact.
Owned TU `src/game/world/LevelScript.cpp`. Base `31297fadc` (wave-145 docs head).
Scratch in `/dev/shm/w9205/` (not tracked).

## Result

```
                         baseline (HEAD)   this pass
process raw diffbytes    54,895            54,895   (source pristine)
orig / ours size         57232 / 57232     57232 / 57232
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
RATCHET                  PASS              PASS
```

## The tool

`_work/reconstructions/levelscript_process_cluster_census.py` (self-contained;
run `python3 _work/reconstructions/levelscript_process_cluster_census.py --top N`).

It:
1. reads both `.so`s, locates `_ZN11LevelScript7processEi` in each (both are at
   `0x137d18`, size `0xdf90`), and computes every maximal run of byte positions
   where the two functions differ **at the same function-relative address**;
2. maps each run start to our source file:line using the per-TU object file's
   DWARF line table (`readelf --debug-dump=decodedline` — note: `objdump -dl`
   mis-decodes this object's line table, reporting e.g. line 3592 where readelf
   correctly reports 4882);
3. maps that line to the innermost enclosing `case N:` with a brace/switch-aware,
   string-safe source scan (nested switches included);
4. prints the original's first three instructions at the run start as a semantic
   hint, plus a per-source-case diff-byte summary.

The `?` bucket in the summary is lines whose DWARF row points into an inlined
header (`Array.h`, `Vector.h`) or into a line before the first `case` of the
function; those runs are still listed individually.

## Census (top 45 of 1640 runs)

```
orig 137d18 size 57232  ours 137d18 size 57232  diff bytes 54895  runs 1640

    # addr range             len  line   case   orig hint
    1 00144b16..00144c9e    393  L5131  c157   144b14:ldr.w r0,[r8,#4]
    2 001396e9..00139860    376  L2405  c69    1396e6:blx getHackingGameDockIndex
    3 0013fa34..0013fb50    285  L2977  c92    13fa32:vmov r3,s4
    4 00142741..00142854    276  L3710  c105   14273e:bic.w r0,r5,#0x80000000
    5 0013c439..0013c53f    263  L4905  c157   13c438:movs r1,#0
    6 0014431e..00144415    248  L2785  c80    14431c:blx Transform::SetAnimationState
    7 0013e856..0013e946    241  L2556  c2     13e856:stmia r6!,{...}
    8 00139de4..00139ecc    233  L3922  c125   139de2:blx TargetFollowCamera::setTarget
    9 0014200d..001420f3    231  L3587  c105   14200c:ands r0,r0
   10 00141438..00141510    217  L2711  c80    141438:strb.w r1,[r0,#72]
   11 001405e2..001406b8    215  L3384  c102   1405e0:str.w r8,[r0,#296]
   12 0013baa9..0013bb7e    214  L1378  -      13baa8:movs r6,#3
   13 00138d81..00138e4f    207  L241   -      138d80:movs r1,#0
   14 0013b7c0..0013b888    201  L1227  c11    13b7be:blx AEGeometry::getRightVector
   15 001447e6..001448ac    199  L4979  c157   1447e6:ldr r0,[r0,#0]
   16 0013a0ce..0013a18c    191  L2947  c92    13a0ce:ldr r2,[r0,#68]
   17 0013b547..0013b605    191  L1107  c6     13b546:str r0,[sp,#64]
   18 00141f4d..0014200b    191  L3572  c105   141f4a:vmul.f32 s2,s2,s0
   19 0013a4a8..0013a563    188  L3286  c102   13a4a8:mov r0,r4
   20 0013fdf3..0013fead    187  L3019  c92    13fdf0:vmov r2,s0
   21 001425a8..00142662    187  L3679  c105   1425a8:blx resetCamera
   22 00141a8b..00141b41    183  L4669  c154   141a8a:movs r1,#1
   23 0014161a..001416cb    178  L4531  c154   14161a:mov r0,r6
   24 0013e1ca..0013e27a    177  L1855  c29    13e1ca:ldr r2,[r0,#68]
   25 0013dddc..0013de8a    175  L3948  c2     13dddc:ldr.w fp,[r5,#20]
   26 0013a7f5..0013a8a2    174  L1766  c21    13a7f4:movs r1,#0
   27 00138783..0013882d    171  L473   -      138780:ldr.w r0,[fp]
   28 0013da2e..0013dad8    171  L1962  c40    13da2e:adds r0,r0,r2
   29 0013dd30..0013ddda    171  L4015  c3     13dd30:movs r1,#0
   30 00143044..001430ec    169  L4970  c157   143044:ldr r4,[sp,#88]
   31 00137f72..00138018    167  L640   -      137f72:blx forceUpdate
   32 0013f781..0013f824    164  L2275  c64    13f780:vmov s0,r0
   33 00141280..00141322    163  L2643  c80    141280:<UNDEFINED 0xf250000c>
   34 00142d0b..00142dad    163  L4261  c142   142d0a:strd r0,r0,[sp,#368]
   35 0013cea6..0013cf47    162  L803   c8     13cea6:blx AEGeometry::setPosition
   36 00141072..00141112    161  L2682  c80    141072:blx nextCampaignMission
   37 00143ef8..00143f96    159  L3824  c105   143ef8:adds r0,r0,r2
   38 00144d9e..00144e36    153  L3053  c92    144d9c:blx Layout::isFading
   39 0013d6fa..0013d791    152  L2345  c67    13d6fa:add r7,r5
   40 0014519c..00145233    152  L4727  c154   14519c:mov.w r1,#0x3f800000
   41 0013d51b..0013d5b1    151  L4290  c144   13d51a:vmov r1,s0
   42 0013e29a..0013e32d    148  L1869  c29    13e298:ldr.w fp,[r0]
   43 0013d640..0013d6d1    146  L1376  -      13d640:ldr r0,[sp,#96]
   44 00143de0..00143e71    146  L3134  c92    143dde:ldr.w r0,[r9,#4]
   45 0014596a..001459fb    146  L4842  c157   145968:vmov.f32 s2,#62
```

Per-source-case diff bytes (top):
```
?       6934  254
157     5686  214
105     4549  122
92      3625   87
102     3580  122
80      2812   61
2       1651   30
14      1446   52
3       1350   28
16      1264   41
41      1248   33
42      1198   35
67      1100   33
40      1086   28
158     1020   31
91       926   34
1        881   39
5        808   27
0        803   29
70       746   23
...
```

## Largest run: `0x144b16..0x144c9e` (393 B) — a layout artifact

The run is **not** a semantic mismatch. It compares two different blocks that
happen to sit at the same address:

* **original** `0x144b16`: inside the case-157 state-5 "destroyed" block
  (`0x144a14..0x144c0e`), source lines ~4898-4910:
  `m_pCamera->setTarget(chaser->geometry); m_tempVec += Vector{45000,2000,11000};
   m_tempVec = stationShip->getPosition(); m_pCamera->setPosition(m_tempVec);
   chaser->setPosition(m_tempVec + Vector{15000,-200,1000}); ...`
* **ours** `0x144b16`: inside the case-157 state-12 block (source line 5131,
  `if (m_nScriptTimerA > 5000 && m_nScriptTimerA < 6000)`), because ours'
  state-5 block is at `0x144366..0x14455a` and the original's is at
  `0x144a14..0x144c0e`.

The two state-5 blocks are nearly instruction-for-instruction equivalent
(verified with a normalized `difflib` sequence alignment of
`orig 0x144a14..0x144c10` against `ours 0x144374..0x144560`): the differences
are `r4/r5/r6/r8` register choices, branch targets, and one moved
`m_pHud->visible = 0` store (`ours 0x1443be`, `orig 0x144ad4`).
The original's block is `0x6ae` bytes later than ours. The split of state 5
itself is identical in both: the `isRunningHDonWeakDevice` weak arm (loop
`i=6..12`) is inline/hot (`orig 0x141b7a`, `ours 0x1418fc`) and the strong arm
(`m_bCinematicBreak=1; loop i=11..22`) is outlined/cold (`orig 0x144a2a`,
`ours 0x144384`). So the shift is upstream of state 5.

A call-symbol delta plot (`orig addr - ours addr` over the aligned call
sequence) shows the layout delta is not a constant: it wanders from +18 at the
prologue to -850 around `0x138d30`, +10318 around `0x13b528`, -1200 around
`0x13fb68`, +696 around `0x1444a2`, and -202 around `0x144c66`. The function's
block order is broadly different, so the largest run cannot be fixed by
reconstructing that one block; it is a downstream symptom.

## Experiments measured (all size-exact unless noted)

| variant | edit | size | raw |
|---|---|---:|---:|
| baseline | — | 57232 | **54,895** |
| e1 | `__builtin_expect(m_nState == 5, 0)` on case-157 state 5 | 57204 | 55,350 |
| e2 | swap the `case 154:` and `case 157:` bodies in the switch | 57232 | **54,895** |

* **e1** makes the state-5 branch cold; the function shrinks by 28 B and the raw
  metric gets 455 B worse. The layout does not move toward the original.
* **e2** is byte-identical to baseline: LLVM normalizes the sparse switch case
  order (the dispatch compare chain is sorted by case value), so source case
  order is not a layout lever.

## Faithfulness notes

* The census is read-only; no source change landed. The baseline is unchanged.
* `objdump -dl` line info for this object is unreliable; `readelf
  --debug-dump=decodedline` is authoritative and is what the tool uses.
* The run at `0x144b16` must **not** be treated as a missing/incorrect source
  block. Reconstructing case-157 state 5 in place would not change the raw
  metric unless the block also moves to `0x144a14`.

## Open questions

1. Find the upstream block whose size/placement differs and moves the whole
   case-157 tail by `0x6ae`. The call-delta plot says the first large divergence
   is around `0x138d30` (-850) and again at `0x13b528` (+10318); a per-case size
   comparison (orig vs ours) would localize it.
2. A source shape that makes LLVM place the case-154 body before the case-157
   state-5 block (the original's order) would collapse this run and several
   neighbours. `__builtin_expect` on the case-154/case-157 mission tests is the
   next lever.
3. The `?` bucket (6,934 B / 254 runs) is mostly inlined-header line rows;
   mapping those to the enclosing statement would need the inline stack, not
   just the top line-table row.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 _work/reconstructions/levelscript_process_cluster_census.py --top 45
cmake --build cmake-build-match --target check -j6
```
