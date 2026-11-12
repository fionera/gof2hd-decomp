# HangarWindow::render — free-credits `textY` volatile-read re-seat (wave 187, w13117)

Worktree `wt/w13117`, branch `wave187-w13117`, base master `8b5d4281c`
(wave-186 harvest 6).  Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B, 3,734 insns,
`.ARM.extab` record 948 B / 156 rows, row 39 `(2508, 150, 9664)`.

Entry state (wave-186 w13017 body): raw **1,512**, aligned **643**, size
**9,700/9,700**, insns **3,730/3,734**, `.text` **74,274**,
`.ARM.extab` **647**, build-id **20**, total **74,941**.

## Result

One source change in the free-credits loop: make `textY` an ordinary
`int` and take a `volatile int &` alias for the first `DrawString`
argument.

```diff
-                volatile int textY = (int) ((float) y + (float) height * 0.2f);
+                int textY = (int) ((float) y + (float) height * 0.2f);
+                volatile int &textYv = textY;
                 canvas->SetColor(0xffffffffu);
                 canvas->DrawString(hangarFont(), *Globals::gameText->getText(i != 0 ? i + 112 : 3400), textX,
-                                            textY, false);
+                                            textYv, false);
```

The volatile read is only needed on the first `DrawString`; the two
`textY + Globals::layout->field_0x4` uses stay plain.  All three
`textY` reads are semantically identical (the value is never written),
and the alias is a normal C++ reference.

| metric | before | after |
|---|---:|---:|
| raw same-address diff | 1,512 | **1,510** |
| instruction-index alignment | 643 | **643** |
| function size | 9,700 / 9,700 | **9,700 / 9,700** |
| instructions | 3,730 / 3,734 | **3,730 / 3,734** |
| its `.ARM.extab` record diff | 81 | **81** |
| row 39 (orig / ours) | (2508,150,9664) / (2508,152,9664) | **(2508,150,9664) / (2508,152,9664)** |
| `.text` section diff | 74,274 | **74,272** |
| `.ARM.extab` section diff | 647 | **647** |
| `.note.gnu.build-id` | 20 | 20 |
| total differing bytes | 74,941 | **74,939** |
| file size | 2,187,328 | **2,187,328** |

The win is two bytes in the free-credits `else` branch: the `String(")")`
temporary is built directly in `r0` (`add r0,sp,#216`) instead of first
being copied through `r5` (`add r5,sp,#216; mov r0,r5`).  The volatile
read on the first `DrawString` changes the allocator's register pressure
at exactly that point.  `.ARM.extab` content is unchanged (647) and its
size/base stay exact (`0x1a3b78`, 156,312 B).

## Gate

`check` green (RATCHET PASS): byte_exact 4507/4524, linked_exact
4507/4524, size-exact 4524/4524, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370.  `.rel.plt` byte-identical, 27/27 section
headers, file size exact.

## Variants measured on the 1,512 base

`textY` forms (all size 9,700 unless noted):

| form | raw | aligned | size |
|---|---:|---:|---:|
| **plain `int` + `volatile int &` on first DrawString (landed)** | **1,510** | **643** | 9,700 |
| plain `int` + `*(volatile int *)&textY` on first DrawString | 1,510 | 643 | 9,700 |
| plain `int` + volatile read on the `i == 0` DrawString | 1,510 | 643 | 9,700 |
| plain `int` + volatile read on the `else` DrawString | 1,512 | 643 | 9,700 |
| plain `int` (no volatile read) | 3,375 | 664 | 9,716 |
| `volatile int` (base) | 1,512 | 643 | 9,700 |
| `volatile float textYf` + `int textY` | 3,282 | 649 | 9,708 |
| `volatile int *textYp = &textY` and `*textYp` | 1,512 | 643 | 9,700 |
| `int textYv = *(volatile int *)&textY` | 3,281 | 556 | 9,712 |

Free-credits `rh`/`canvas`/`textX` permutations re-swept on the same
base with the landed `textY` form; the base `rh`-before-`width`,
`PaintCanvas *volatile canvas` and `width + baseX + rh` remain the best
size-exact combination (1,510).  `rh` before `width` is 1,519; `rh`
after `height` 1,522; no `rh` 1,522; plain canvas 9,708.

## Residual

Unchanged in kind: the buy-list `lay` clobber (`ldr.w sl,[sl]` at
`0x14a960` + `ldr.w sl,[sp,#128]` at `0x14ac64`) keeps the 528-byte
buy-list run shifted +4, the ship branch keeps the non-ship `switch`
inline (308-byte run), and the free-credits `width`/`rh` and
`sl`/`r9` roles remain swapped.  `Globals::layout` direct use (no `lay`
local) is 9,696 B and raw 3,532; the local `lay` is still the best
size-exact shape.

## Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN12HangarWindow6renderEv
python3 _work/reconstructions/extab_lsda_decode.py \
    /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
    cmake-build-match/libgof2hdaa.so
cmake --build cmake-build-match --target check -j2
```
