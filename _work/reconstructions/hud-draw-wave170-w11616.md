# Hud::draw — wave 170 (w11616): transfer-block stack-slot permutation analysis

Target: `_ZN3Hud4drawExxP9PlayerEgobjj` (`src/game/ui/Hud.cpp`, `Hud::draw`),
original **9,332 B**, VMA `0x163b90`.  Entry state (wave-169 / w11516):
raw **548**, aligned **220**, size 9,332, `.ARM.extab` 0x26298 (77 LSDA entries),
`.rel.plt` byte-identical.

No code change landed this pass: every measured variant was neutral or worse than
the wave-169 baseline.  The tree is left at the 548-raw shape.  The measurements
below are the durable result.

## Raw-diff map of the 548-byte residual

The residual is concentrated in three regions (raw differing-byte spans, merged
with <=8-byte gaps):

| span | bytes | region |
|---|---:|---|
| 164188–16419c | 20 | damage block, `hitFlags & 0x24` |
| 16588c–165895 | 9 | transfer block, `frameH`/`font` stack slots |
| 16594c–1659a9 | 93 | transfer block, `barW`/`barH` regalloc |
| 1659b4–165a06 | 71 | transfer block, `&Globals::status` regalloc |
| 165a40–165a83 | 68 | transfer block, `font`/`status` reloads |
| 165ae6–165af3 | 14 | transfer block tail |
| 165ba6–165ca4 | 196 | show block, `barH`/`text`/`halfW` order + Canvas reg |
| 165cb6–165d48 | 146 | show block, DrawRegion2D/DrawString |
| 164052–164081 | 13 | shield/gamma register swap |
| 165fbc–165ff1 | 12 | landing-pad tail |

The show-block differences (`ldr r0,[fp]` vs `ldr r0,[r5]` for `Globals::Canvas`)
are a **downstream symptom** of the transfer block's register allocation, not an
independent declaration-order problem.

## Root cause: stack-slot permutation in the transfer block

The original and our build assign the same long-lived values to a permuted set
of frame slots.  Original (creation order):

| value | slot |
|---|---:|
| `&Globals::iPad` | 92 (reused later for `Globals::font`) |
| `Globals::Canvas` | 108 |
| `GetImage2DWidth(image_0x31c)` | 104 |
| `GetImage2DWidth(image_0x310)` | 100 |
| `Globals::layout` | **100** |
| `this` | 104 |
| `dividerOffset` | **96** |
| `&Globals::status` | 108 |
| transfer `frameH` | **96** |
| transfer `Globals::font` | **92** |

Ours: `layout`→104, `this`→108, `dividerOffset`→100, `status`→96,
transfer `frameH`→92, `font`→96.  Everything is shifted by one slot because the
allocator picks the *oldest* free slot where the original picks the *newest*
(original: `layout` reuses width2=100 and `frameH` reuses `dividerOffset`=96;
ours reuses width1=104 and `&iPad`=92).  Fixing the first choice (`layout`=100)
cascades the whole damage block into agreement.

## Measured variants (all size 9,332 unless noted)

| variant | raw | aligned | notes |
|---|---:|---:|---|
| wave-169 baseline (`H T W D Y`, anchor 0x218) | **548** | **220** | landed |
| move `dividerOffset` init after `SetColor` | 582 | 235 | fixes layout=100/status=108, but adds 5-instruction reorder |
| same + `textH` declared before `frameH` | 605 | 233 | |
| same + `int textH;` split declaration | 548 | 220 | no effect |
| move `armorY`/`armorBarY` before `dividerOffset` | 548 | 220 | no effect |
| anchor `layout->field_0x1e8` instead of 0x218 | 588 | 221 | |
| `T W H D Y` + `float filled;` declared before SetColor | 1180 | 191 | keeps 77 LSDA entries, but show block shrinks 4 insns and shifts the tail |
| `0x24` volatile read moved to end of first damage block | 7453 | 631 | size 9,272 |
| `0x24` volatile read moved to start of first damage block | 7358 | 629 | size 9,272 |
| `0x24` read after the store (braced block) | 7117 | 515 | size 9,336 |
| `0x24` read unconditionally between 0x24 and 0x18 blocks | 7112 | 514 | size 9,336 |
| `0x24` read moved to the `field_0x4b4` block | 800 | 227 | size 9,332 |

The `hitFlags & 0x24` volatile read is a genuine **size anchor**: removing it
drops the function to 9,272 B (60 bytes short), so it cannot simply be deleted to
match the original's `itt ne; movne; strne` shape.

## Next step

Find the source shape that makes the original pick `layout`=100 *without*
moving the `Globals::layout` load after `SetColor`.  Candidate: an extra
4-byte stack object created between `&Globals::iPad` and `Globals::layout`, or a
different declaration order of the setCoordsSteer argument temporaries.  Fixing
that one slot should cascade through `dividerOffset`/`status`/`frameH`/`font`
and remove most of the transfer and show block residual.

## Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN3Hud4drawExxP9PlayerEgobjj
cmake --build cmake-build-match --target check -j2
```
