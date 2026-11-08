# Small tail B wave 154 (w10018): MenuTouchWindow / ModStation / SpaceLounge

Status: **analysis only, no landed `src/` change**. Worktree `wt/w10018`,
base `4417db0b4` (wave 153). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Read with `small-tail-b-wave153-w9918.md`, `small-tail-b-wave152-w9818.md`,
`small-tail-b-wave151-w9718.md`.

Gate on entry and exit (no src edit): `byte_exact 4507`, `linked_exact 4507`,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0, parity
5370/5370.  Body sizes unchanged: `OnTouchBegin` 1896 B, `ModStation` 1008 B,
`drawLounge` 1948 B.

Metrics: raw same-address diff bytes (`/dev/shm/w10018/fdiff.py`) and
instruction-index alignment (`/dev/shm/w10018/align_diff.py`, path-adjusted
copy of the committed tool).

| fn | raw before | raw after | aligned before | aligned after |
|---|---:|---:|---:|---:|
| `MenuTouchWindow::OnTouchBegin` (`0x129bd4`) | 29 | 29 | 15 | 15 |
| `ModStation::ModStation` C1 (`0xd52d0`) | 43 | 43 | 12 | 12 |
| `SpaceLounge::drawLounge` (`0x170d38`) | 175 | 175 | 41 | 41 |

## 1. MenuTouchWindow: the original case-15 block is ALL-DIRECT; the
## `Layout &layout` reference is a register-allocation crutch

New evidence this wave.  The original's case-15 scrollbar test is

```
12a1f0: ldr r2,[r4,#0]        ; Globals::layout   (r4 = &Globals::layout)
12a1fc: ldr r2,[r2,#40]       ; -> buttonInsetX
...
12a212: ldr r2,[sp,#16]       ; &Globals::layout  (spill)
12a218: ldr r2,[r2,#0]        ; Globals::layout   <- RELOAD after the call
12a21e: ldr r5,[r2,#12]       ; -> field_0xc_leftMargin
12a220: ldr r6,[r2,#32]       ; -> field_0x20_top
```

i.e. **both** the `leftMargin` (B) and `top` (C) operands come from the
post-call reload of `Globals::layout`, not from a cached reference.  That is
exactly the all-direct spelling

```
if (x > Globals::w - Globals::layout->buttonInsetX - Canvas->GetImage2DWidth(...) &&
    y < Globals::layout->field_0x20_top +
        (Globals::layout->field_0xc_leftMargin + Canvas->GetImage2DHeight(...)))
```

**But** all-direct removes the only live-across-call use of the inner
`Layout &layout = *Globals::layout;`, which changes the function's *global*
register colouring: `&Globals::Canvas` moves out of `fp` into `r5`, the
`&Globals::layout` address lives in `fp` instead of `r4`+`[sp,#16]`, the body
shrinks 1896 -> 1892 B and every branch to the tail shifts by 4 bytes
(`12a2b6` -> `12a2b2`).  Net: **358 raw / 76 aligned**.

So the reference is load-bearing for the frame/colouring, and while it is
live across the call any post-call use of it is served from the cached value
(`r6`).  The two requirements are mutually exclusive in clang 7.0.2:

* reference live across call (frame preserved) -> B or C cached
  (HEAD 29/15, w9918 28/16);
* reference not live across call (B and C both reload) -> frame changes
  (358/76).

New spellings tried this wave (all reverted):

| shape | raw | aligned | size |
|---|---:|---:|---:|
| HEAD (A,B direct; C ref; D direct) | **29** | **15** | 1896 |
| w9918 (A direct; B ref; C direct; D direct) | 28 | 16 | 1896 |
| B ref + `leftMargin + (top + H)` (operand swap) | 28 | 16 | 1896 |
| all four direct | 358 | 76 | 1892 |
| A ref; B,C,D direct | 358 | 76 | 1892 |
| B,C direct; D ref | 314 | 74 | 1896 |
| B ref; C direct; D ref | 287 | 74 | 1896 |
| B direct; C ref; D ref | 288 | 73 | 1896 |
| B,C ref; D direct | 304 | 73 | 1896 |
| `(leftMargin + top) + H` grouping | 45 | 23 | 1896 |
| `top + (H + leftMargin)` | 292 | 35 | 1896 |
| `top + (leftMargin + H)` (HEAD grouping) | 29 | 15 | 1896 |
| `leftMargin + (top + H)` (B ref) | 28 | 16 | 1896 |
| `ListEntryWidth + (leftMargin + top)` (case 8) | 34 | 16 | 1896 |
| `x > w - W - inset` (A subtraction swap) | 305 | 45 | 1896 |
| `x > w - (inset + W)` | 90 | 26 | 1896 |
| ref hoisted to top of case 15 (`Layout &lay2`) | 1761 | 822 | 1896 |
| same, B ref / C direct | 1761 | 823 | 1896 |
| same, all direct | 358 | 76 | 1896 |

The `ip`/`r4` tie at `0x129fd2` (case-8 `listEntryWidth` scratch) is
unaffected by every shape above and is confirmed a pure Greedy-RA tie.

**Conclusion.** No source shape improves both metrics; HEAD (29/15) is kept
(wave 149 already preferred aligned 15 over raw 28).  The remaining 15
aligned mismatches are 10 register-colouring ties in the case-15 cluster
plus the independent `ip`/`r4` tie.

## 2. ModStation: the volatile-yaw family is saturated at 43/12

Re-confirmed HEAD 43/12/1008.  Raw byte runs (8 runs): the two `ldr`
register swaps (2 B), 16 B + 23 B of shifted instructions, and the two
literal-pool words.  New spellings tried this wave (all reverted; 1008 B
unless noted):

| shape | raw | aligned |
|---|---:|---:|
| HEAD (both volatile) | **43** | **12** |
| yaw plain (`*(const int *)`) | 44 | 18 |
| `yawTable = YawIPad; if (!iPad) yawTable = Yaw;` (plain) | 44 | 18 |
| `yawTable = Yaw; if (iPad) yawTable = YawIPad;` (plain) | 44 | 18 |
| `if/else` both orders (plain) | 44 | 18 |
| `yawTable = YawIPad; if (!iPad) ...` (volatile) | 43 | 12 |
| `yawTable = Yaw; if (iPad) ...` (volatile) | 43 | 12 |
| `yawTable = YawIPad; yawTable = iPad ? yawTable : Yaw;` | 43 | 12 |
| `yawTable = Yaw; yawTable = iPad ? YawIPad : yawTable;` | 43 | 12 |
| `!iPad ? Yaw : YawIPad` / `iPad == 0 ? ...` / `!= 0` / `> 0` | 43 | 12 |
| inline select `&(iPad ? YawIPad : Yaw)[race]` | 690 | 52 |
| pointer arithmetic `((const volatile int*)yawTable)[race]` | 43 | 12 |
| `*(const volatile int *)(yawTable + race)` | 43 | 12 |
| `__builtin_memcpy` bit-cast | 43 | 12 |
| `volatile int yawBits` local | 563 | 124 |
| `volatile int pitchBits` local | 607 | 177 |
| yaw load before pitch load | 744 | 40 |
| pitch load between select and yaw load | 690 | 52 |
| select after pitch load | 690 | 52 |
| select after pitch load, yaw plain | 689 | 61 |

The `volatile` on the yaw *element* load is still the only lever that flips
the select operand order and therefore the literal-pool order
(`YawIPad,Yaw` vs `Yaw,YawIPad`) and the `mov r0,r3`/`cmp` order.  Dropping
it fixes the pool order but the PostRA list scheduler then sinks the `Yaw`
LDRLIT below the pitch load, hoists `mov r4,r2` (the pitch callee-save copy)
above the yaw load and swaps `movs r0,#2`/`str r8` -> 44/18.  No source form
keeps the volatile schedule *and* the non-volatile pool order.

## 3. SpaceLounge: unchanged

Not swept this wave (budget); HEAD 175/41/1948 stands.  The open question is
still the w130 `agent > pad > x` colouring tie that keeps `pad` in `fp` and
`x` in `r9` (original: `pad` in `r9`, `x` in `r5`) and forces the
4-instruction right-edge clamp.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10018
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10018/fdiff.py <mangled>
python3 /dev/shm/w10018/align_diff.py <mangled>
```

Scratch (not committed), `/dev/shm/w10018/`: `fdiff.py`, `align_diff.py`,
`rdiff.py`, `m.sh`, `sweep_ms*.py`, `sweep_mtw*.py`, `base/` (pristine
sources), `ms_mir_*.txt` (MIR dumps of ModStation C1 for HEAD and the
yaw-plain variant).

## 5. Files changed

* `_work/reconstructions/small-tail-b-wave154-w10018.md` (this note).
