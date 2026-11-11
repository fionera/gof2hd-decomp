# LevelScript::process — wave 173 / w11803: mission-0 state-0 block order

Target `_ZN11LevelScript7processEi` (original `0x137d18`, **57,232 B = 0xdf90**).
Worktree `wt/w11803`, branch `wave173-w11803`, base master `3fafa5b03`.
Follow-up to `levelscript-process-blockorder-wave171-w11706.md`.

## Baseline (HEAD, tree left clean)

```
process raw diffbytes    53,750
process aligned insns    18,049
orig / ours size         57232 / 57232   (size-exact)
orig / ours insns        20830 / 20729
gate                     byte 4507 / linked 4507 / size 4524 / data 843 /
                         imports 207 / lint CLEAN / drift 0 / parity 5370
```

The first literal pool sits at `0x1380a4` in the original and `0x1380c0` in ours
(**+28 B**); every later pool inherits the shift.

## Concrete byte accounting of the +28

Region sizes (original vs ours):

| region | original | ours | delta |
|---|---:|---:|---:|
| `0x137d18..` prologue + mission-1 body | 364 B | 370 B | **+6** |
| mission-0 state-0 body up to tail tag | 248 B | 236 B | **-12** |
| tail preamble | 6 B | 6 B | 0 |
| `0x137f7c..` tail body up to first pool | 296 B | 330 B | **+34** |
| **total to first pool** | **908 B** | **936 B** | **+28** |

The `+6` prologue is `&Globals::status` living in `r9` (a high register: `ldr.w r9,[r0]`
+ `ldr.w r4,[r9]` + `mov r0,r4`) instead of a low register (`ldr r4,[r0]` + `ldr r0,[r4]`).
The `-12` is the mission-0 3-ship loop (see below).  The `+34` tail is
`+4` (extra `str.w sl,[sp,#68]` spill of `this` in the tail preamble),
`+2` (first wingman loop layout), `-2` (status reload), `-2` (`ittt ge` vs two `blt.w`),
`+2` (`beq.w` vs `beq.n`), and `+26` (second wingman loop body placed **before** the pool).

## The two wingman loops

Original, first wingman loop (`L1`) `0x137fb0..0x137fc8` (bottom-tested, body inline):

```
137fae: b.n  137fc4          ; test first (count==0 handled)
137fb0: ldr  r0,[r4,#4]      ; enemies->data_
137fb2: ldr.w r5,[r0,r6,lsl#2]
137fb6: mov  r0,r5
137fb8: blx  KIPlayer::isWingMan
137fbc: cbz  r0,137fc2
137fbe: strb.w r8,[r5,#301]  ; field_0x12d = 1
137fc2: adds r6,#1
137fc4: ldr  r0,[r4,#0]
137fc6: cmp  r6,r0
137fc8: bcc.n 137fb0
```

Ours, `L1` `0x137fac..0x137fc8` (top-tested, body out of line because of
`__builtin_expect(wingman->isWingMan(), 0)`):

```
137fac: ldr  r0,[r4,#0]
137fae: cmp  r6,r0
137fb0: bcs.n 137fca
137fb2: ldr  r0,[r4,#4]
...
137fbe: cbnz r0,137fc4
137fc0: adds r6,#1
137fc2: b.n  137fac
137fc4: strb.w r8,[r5,#301]
137fc8: b.n  137fc0
```

Removing the `expect` makes the body instruction-for-instruction faithful but moves
it 2 B earlier and does not change the function size (raw 53,765).

Second wingman loop (`L2`): original places the loop body **after** the first pool
(`0x138110`), ours places it **before** the pool (`0x1380a4`):

```
original:  1380a0 b.n 138124 | 1380a2 nop | 1380a4 pool (108 B) | 138110 L2 body | 138124 L2 test | 13812a mission-5 check
ours:      1380a2 b.n 1380b8 | 1380a4 L2 body | 1380b8 L2 test | 1380be b.n exit | 1380c0 pool (100 B) | 138124 mission-81 check
```

The pool placement is a post-RA `arm-cp-islands` decision.  The pass walks the
`WaterList` (blocks without fallthrough) backwards and picks the highest-numbered
water block in range.  In our IR both `bb.30` (L2 loop entry, ends `tB bb.34`) and
`bb.34` (L2 test, ends `tB bb.85`) are water, so the pool goes after `bb.34`.
In the original `bb.34` falls through to the mission-5 check, so it is **not** water
and the pool goes after `bb.30`.  That is exactly the layout we want.

`bb.34` only has an unconditional `tB` because the current source uses
`else if (mission == 5)` for the mission chain: when `m_bStartSequence != 0` the
whole mission chain must be skipped, so `bb.34`'s exit is `bb.85` (the general
state machine), not the next block.

## Oracle: the mission chain is a separate `if`

`_work/oracles/Galaxy-on-Fire-2-Unity-Remake/Reference/decompiled/native/game/LevelScript.c`
(`LevelScript::process`, `0x160d50`, 57,020 B) shows:

```c
if (this[0x20] != 0) {                      /* m_bStartSequence */
    ...
    if (Status::getWingmen(Globals::status) != 0) {
        puVar8 = Level::getEnemies(...);
        if (*puVar8 != 0) { do { ... } while (i < *puVar8); }
    }
    ...
    if (Status::getWingmen(...) != 0) { ... same do-while ... }
}
iVar7 = Status::getCurrentCampaignMission(Globals::status);
if (iVar7 == 5) { ... }
else { if (iVar7 == 0x1a && inAlienOrbit()) { ... }
       else { if (iVar7 == 0x32) { ... }
              else { if (messages != 0 && iVar7 == 0x33) goto wormhole; } } }
```

So the mission chain is a **separate `if`**, not an `else if` of `m_bStartSequence`.
Converting our source to that shape is faithful but changes the function size
(see table) and the deactivation body moves out of line, so the pool does not land
where expected.

## Measurements (all size-checked; no source change landed)

Current base: raw **53,750**, aligned **18,049**, size **57,232**.

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | **53,750** | **18,049** | **57,232** |
| first-loop expect removed | 53,765 | 18,045 | 57,232 |
| `int i` in either/both wingman loops | 53,750 | 18,049 | 57,232 |
| `PlayerFighter *` cast in loops | 53,750 | 18,049 | 57,232 |
| L1 hoist `count` | 55,226 | 18,070 | 57,240 |
| L2 hoist `count` | 55,225 | 18,916 | 57,252 |
| both hoist | 55,221 | 18,780 | 57,252 |
| mission-0 3-ship loop double-deref `(*enemies)[i]` twice | 55,607 | 25,396 | 57,336 |
| mission-5 `expect(...,1)` | 55,535 | 21,817 | 57,432 |
| mission-5 `expect(...,0)` | 55,404 | 18,675 | 57,236 |
| mission-26 `expect(...,0)` | 55,166 | 18,436 | 57,236 |
| mission-81 `expect(...,0)` | 55,483 | 18,707 | 57,232 |
| m5 e1 + m81 e0 | 55,620 | 21,134 | 57,388 |
| m5 e0 + m81 e0 | 55,556 | 19,711 | 57,248 |
| whole deactivation condition one expect | 53,775 | 18,196 | 57,232 |
| separate `if` mission 5 | 55,440 | 18,707 | 57,200 |
| sepif + whole-condition expect | 55,431 | 18,577 | 57,200 |
| sepif + L1 expect removed | 55,436 | 18,700 | 57,200 |
| sepif + whole + L1 removed | 55,447 | 18,570 | 57,200 |
| `act e1` (`expect(m_bStartSequence != 0,1)`) | 55,705 | 22,201 | 57,412 |
| sepif + act e1 | 55,334 | 18,782 | 57,216 |
| sepif + m5 e0 + act e1 | 54,964 | 18,835 | 57,276 |
| sepif + whole + act e1 | 55,282 | 18,573 | 57,200 |
| sepif + whole + m5 e0 + act e1 | 55,367 | 18,779 | 57,268 |
| W1 `getWingmen` expect0 | 55,318 | 18,166 | 57,248 |
| W2 `getWingmen` expect0 | 55,326 | 18,153 | 57,268 |
| both W expect0 | 55,430 | 18,225 | 57,176 |
| no deactivation expect | 55,092 | 18,045 | 57,240 |
| L2 moved outside inner deactivation `if` (still inside outer) | 54,148 | 18,262 | 57,232 |
| L2 moved before `field_0x24 += delta` | 55,484 | 18,397 | 57,296 |
| L2 moved after full else-if chain before switch guard | 55,599 | 18,651 | 57,460 |
| L2 into mission-5 block | 55,309 | 18,094 | 57,240 |
| L2 `expect(...,0)` | 55,206 | 18,049 | 57,240 |
| L2 `expect(...,1)` | 53,750 | 18,049 | 57,232 (neutral) |
| mission-81 block moved last | 55,699 | 20,681 | 57,400 |

New in this pass (base 53,750):

| variant | raw | aligned | size |
|---|---:|---:|---:|
| all `Globals::status` -> local `status` | 55,575 | 23,536 | 55,572 |
| `Status *volatile status` | 55,104 | 21,734 | 57,240 |
| `Status *&status` (reference) | 53,751 | 18,049 | 57,232 |
| `status->nextCampaignMission` at state-16 | 55,251 | 19,086 | 57,472 |
| 3-ship loop `enemies->data_` hoist | 55,230 | 18,062 | 57,240 |
| 3-ship loop `Array&` hoist | 55,607 | 25,396 | 57,336 |
| `L1 count!=0` wrapper | 55,230 | 18,056 | 57,240 |
| `L2 count!=0` wrapper | 55,200 | 18,054 | 57,240 |
| both `count!=0` | 55,265 | 18,049 | 57,248 |
| L2 `while` loop | 53,750 | 18,049 | 57,232 (neutral) |
| L2 `do-while` | 55,010 | 18,087 | 57,184 |
| L2 `do-while` + `count!=0` | 54,996 | 18,054 | 57,192 |
| L1 `do-while` + `count!=0` | 55,422 | 20,568 | 57,248 |
| **L2 loop-condition `expect(i < count, 0)`** | 55,112 | 20,574 | 57,192 |
| L2 cond e0 + L1 noexp | 55,122 | 20,568 | 57,184 |
| L2 cond e0 + L2 body e0 | 55,058 | 18,064 | 57,184 |
| L2 cond e0 + act e1 | 55,369 | 21,708 | 57,336 |
| L2 cond e0 + m5 e0 | 55,321 | 20,575 | 57,208 |
| L2 cond e0 + m5 e0 + m51 noexp | 55,428 | 18,558 | 57,208 |
| L2 cond e0 + m5 e0 + dact noexp | 55,327 | 20,571 | 57,208 |
| mission chain reordered (5 last) | 55,376 | 18,385 | 57,232 |
| M50 condition order swapped (mission first) | 53,751 | 18,049 | 57,232 |
| M51 expect removed | 53,879 | 18,007 | 57,232 |
| M50 swap + M51 noexp | 53,876 | 18,007 | 57,232 |
| M5 e1 + M51 noexp | 55,535 | 21,817 | 57,432 |

A 64-cell grid over the `L2 expect(...,0)` family (L1 expect, L2 body expect, M5
expect, M51 expect, `act` expect, deactivation expect) produced **no size-exact
member**; the closest sizes were 57,184 / 57,192 / 57,196 / 57,200 / 57,208.

## Machine-IR / pass findings

Standalone compile with the exact `compile_commands.json` flags plus
`-mllvm -print-after=arm-cp-islands` (pass name is **`arm-cp-islands`**; a full
`-print-after-all` is ~517 MB, redirect to disk).

Base IR after constant islands (relevant blocks):

```
bb.15  tail tag (ldrb m_bStartSequence; str sl,[sp,#68])
bb.16  forceUpdate; status->getWingmen
bb.17  getEnemies -> r4
bb.18..21  L1 loop (top-tested: bb.18 test, bb.19 body, bb.20 incr, bb.21 strb)
bb.22  field_0x24 += delta; getPlayer
bb.23  mission>=2 && passedTime>=7001   (ittt ge)
bb.24..29  deactivation body (inline)
bb.30  getEnemies -> r4; i=0; tB bb.34
bb.31  L2 body
bb.32  strb
bb.33  i++
bb.34  ldr count; cmp; bcc bb.31; tB bb.85
bb.35..58  CONSTPOOL_ENTRYs 479..502 placed after bb.34
bb.85  general state machine (getMission()->isCampaignMission ...)
```

`bb.30` and `bb.34` are both water (`tB` terminators).  `arm-cp-islands`
`findAvailableWater` searches `WaterList` backwards and prefers the highest-numbered
water block in range, so the first CPE lands after `bb.34`.  Making `bb.34` fall
through (i.e. making the mission-5 check the next block) makes it non-water and the
first CPE lands after `bb.30` — the original layout.

The separate-`if` shape does make `bb.23` fall through to the mission-5 check, but
`MachineBlockPlacement` then moves the deactivation body out of line (`bge.w` to a
far block) and the pool lands even later (`0x1380c8`), so the size shrinks and raw
rises.

The `L2 expect(i < count, 0)` shape *does* move the first pool to `0x1380a4`
(verified: pool bytes `7fd2 000d 8400 000d ...` land at exactly the original
address, only 3 GOT-offset entries differ) and keeps the deactivation body inline,
but the cold loop body is then placed far out of line (`0x13a2a6`) and the function
shrinks to 57,192 B, so the post-pool code is shifted and raw rises to 55,112.

## Conclusion

No source shape found in this pass beats the base raw 53,750 while remaining
size-exact.  The remaining structural blockers are:

1. `&Globals::status` occupies `r9` for the whole function, so the mission-0 3-ship
   loop cannot hoist `enemies` into `r9` and stays 12 B short.
2. The `else if (mission == 5)` chain forces `bb.34` to end in `tB bb.85`, making it
   water, so the first constant pool is placed after the second wingman loop instead
   of before it.
3. The faithful separate-`if` shape (oracle-confirmed) changes the function size by
   -32 B because `MachineBlockPlacement` moves the deactivation body out of line; no
   compensating +32 B has been found.

Tree left at base; only this note is committed.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11803
ulimit -v 12000000
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
```

IR:

```sh
cd cmake-build-match
<clang++ from compile_commands.json> -mllvm -print-after=arm-cp-islands \
  -o /home/debian/GalaxyOnFire2/scratch/w11803/ls.o -c ../src/game/world/LevelScript.cpp \
  > /home/debian/GalaxyOnFire2/scratch/w11803/mi.log 2>&1
```
