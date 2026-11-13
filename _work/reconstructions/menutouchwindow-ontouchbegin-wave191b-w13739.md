# MenuTouchWindow::OnTouchBegin wave 191b (w13739): RA keep-vs-remat recheck, no win

Date: 2026-11-13. Worktree `wt/w13739`, branch `wave191b-w13739`, base
`22c4b25f5`. Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target `_ZN15MenuTouchWindow12OnTouchBeginEiiPv`
(`src/game/ui/MenuTouchWindow.cpp`, function at `0x129bd4`, 1,896 B).
Baseline and best measured this wave: **raw 28 / aligned 16 / size
1896/1896 / 754/754 insns**. `src/` is unchanged; the gate is green.

Gate at HEAD and after this wave (`check`): `RATCHET PASS` — byte_exact
**4508/4524**, linked_exact 4508/4524, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, symbol parity 5370/5370 (0 extra / 0
missing), stub_zero_size 0. Section deltas unchanged: `.text` 72,646,
`.ARM.extab` 564, `.ARM.exidx` 0, `.rodata` 0; `.rel.plt` byte-identical.

This wave re-read the five wave-191b notes
(`menutouchwindow-ontouchbegin-wave191b-w13712/-w13666/-w13652/-w13612/-w13603.md`)
and re-ran the established levers plus a few new ones (16-way local/direct
grid with forced rebuilds, volatile-reference variants, Canvas-local
variants, `__builtin_expect`, case-8 condition rewrites, line-layout
perturbations). Nothing beat 28/16.

## 1. Residual map at HEAD (28 bytes, 15 runs)

`tools/fn_diff.py --ours` reports 754/754 instructions; every difference is
a register name or a `[sp,#16]` reload. `bytediff.py` byte map:

| offset | original | ours | region |
|---|---|---|---|
| `0x401` | `c1` | `41` | A (case 8: `ldr.w ip,[sl,#340]` vs `r4`) |
| `0x404` | `63` | `23` | A (`add r3,ip` vs `add r3,r4`) |
| `0x60f` | `31` | `21` | B (c1: `r3=heapBufB` vs `r2`) |
| `0x615` | `61` | `31` | B (c1: `r6=field_0x1e0` vs `r3`) |
| `0x61c` | `22` | `26` | B (c1: `r2=layout value` vs `r6`) |
| `0x61e` | `53` | `52` | B (c1: `[r3,r6]` vs `[r2,r3]`) |
| `0x620` | `26` | `23` | B (c1 index reg) |
| `0x622` | `05` | `04` | B (c1: `r5=w` vs `r4`) |
| `0x628` | `92` | `b5` | B (c1: `ldr r2,[r2,#40]` vs `ldr r5,[r6,#40]`) |
| `0x62a..0x62c` | `ad1a` | `641b` | B (c1: `subs r5,r5,r2` vs `subs r4,r4,r5`) |
| `0x630` | `28` | `20` | B (c1: `subs r0,r5,r0` vs `subs r0,r4,r0`) |
| `0x640..0x64d` | `50f821101268dbf80000d56816` | `f46850f821101268dbf8000015` | C (c2: fresh reload vs cached `lm`) |
| `0x652` | `71` | `61` | C (c2: `adds r1,r6,r5` vs `adds r1,r4,r5`) |
| `0x672` | `95` | `94` | D (c3: `r5=buttonInsetX` vs `r4`) |
| `0x678` | `28` | `20` | D (c3: `add r0,r5` vs `add r0,r4`) |

Regions B/C/D are one coupled register-allocation difference: the original
reloads `Globals::layout` after the first call and keeps nothing live across
it, so the layout value lands in call-clobbered `r2` and `r5` is free for
`w`; HEAD keeps the pre-call layout value in callee-saved `r6`, which shifts
the whole c1 register assignment. Region A (case 8) is independent.

## 2. Root cause: Greedy-RA keep-vs-remat tie-break

Post-RA MIR (`-mllvm -print-machineinstrs`, "After Virtual Register
Rewriter") confirms the two reachable states:

* **HEAD** (`Layout &layout = *Globals::layout;` used for c2 `lm`):
  `%29 = [%508]` is live across `GetImage2DWidth` and kept in `r6`;
  the `&Globals::layout` address is rematerialised from `%stack.0` at
  bb.42/44/47 (`$r4`, `$r2`, `$r0`), and `fp = &Globals::Canvas`.
  7 address loads, 754 insns, 1896 B, raw 28.
* **D** (all-direct `Globals::layout->`): `%29` is dead after c1; the RA
  keeps `%508` (`&Globals::layout`) in `r11`/`fp` from the case-15 start
  (`$r11 = [%stack.0]`), and `&Globals::Canvas` goes to `r5`. 5 address
  loads, 752 insns, 1892 B, raw 362.

The original is the third state: **`%29` dead after c1, address still
rematerialised from `[sp,#16]`, `fp = &Globals::Canvas`** (3 address loads
at `0x12a1bc`/`0x12a212`/`0x12a23c`, 754 insns, 1896 B). The pre-RA IR of
HEAD and D is otherwise identical (`%99 = tLDRLIT @Globals::layout` at
entry, `%29/%301 = [%99]` in bb.42, `%319 = [%99]` in bb.44); the
`%508 -> %511/%510` split is created by the Greedy RA, not by the front end.
So the residual is a pure RA keep-vs-remat tie-break, not a source-shape
difference in the instruction stream.

## 3. Measurements this wave (all reverted; 1896 B unless noted)

Every candidate was a full gold link measured with `tools/fn_diff.py`.
Scratch harness: `/home/debian/GalaxyOnFire2/scratch/w13739/grid2.py`
(forced rebuild: `os.utime(SRC, now+2)` before every build, because a
same-second write can leave a stale object).

### 3.1 16-way `{c1, c2lm, c2top, c3}` local/direct grid

`L` = via `Layout &layout = *Globals::layout;`, `G` = direct
`Globals::layout->`. HEAD is `GLGG` (c1 direct, lm local, top direct, c3
direct).

| cell | raw | aligned | size |
|---|---:|---:|---:|
| GGGG (all-direct D) | 362 | 76 | 1892 |
| GGGL | 322 | 74 | 1888 |
| GLLG | 308 | 73 | 1892 |
| GLLL | 316 | 76 | 1888 |
| **GLGG (HEAD)** | **28** | **16** | **1896** |
| GLGL | 292 | 73 | 1892 |
| GGLG | 28 | 16 | 1896 |
| GGLL | 291 | 74 | 1892 |
| LGGG | 362 | 76 | 1892 |
| LGGL | 322 | 74 | 1888 |
| LLLG | 308 | 73 | 1892 |
| LLLL | 316 | 76 | 1888 |
| LLGG | 29 | 15 | 1896 |
| LLGL | 292 | 73 | 1892 |
| LGLG | 28 | 16 | 1896 |
| LGLL | 291 | 74 | 1892 |

Only the four cells with a value live across the first call (`GLGG`,
`GGLG`, `LLGG`, `LGLG`) stay at 1896 B; every cell with both c2 fields
fresh is 1892 B or smaller. There is no cell with both c2 fields fresh and
1896 B. (`LGLG` is the alternative 28/16 tie: c1 local, lm direct, top
local, c3 direct.)

### 3.2 Volatile-reference variants (force a fresh c2 load)

| variant | raw | aligned | size | note |
|---|---:|---:|---:|---|
| `volatile Layout &vlayout` for lm | 35 | 21 | 1896 | c2 fresh, but `fp = &Globals::layout` |
| `volatile Layout &` for top | 39 | 15 | 1896 | c2 fresh, address still in `fp` |
| `volatile Layout &` for lm+top | 314 | 74 | 1892 | all-direct D behaviour |
| `(*(Layout *volatile *)&Globals::layout)->` for lm+top | 127 | 76 | 1896 | two separate volatile pointer loads, 753 insns |
| `(*(Layout *volatile *)&Globals::layout)->` for lm only | 127 | 76 | 1896 | same |
| `(*(Layout *volatile *)&Globals::layout)->` for top only | 365 | 80 | 1892 | |

The volatile reference does produce a fresh c2 load, but the address stays
in `fp`; it never combines with the HEAD address-spill state.

### 3.3 Canvas-local / texture-local / expect variants

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `PaintCanvas *canvas = Globals::Canvas` for c2 | 316 | 40 | 1892 |
| `PaintCanvas *canvas` for c1+c2 | 316 | 40 | 1892 |
| `PaintCanvas *canvas` for all three | 359 | 75 | 1892 |
| `PaintCanvas *volatile vcanvas` for c2 | 1749 | 255 | 1920 |
| `unsigned int texB = this->heapBufB[...]` | 374 | 86 | 1892 |
| `int idx = this->field_0x1e0` | 378 | 78 | 1888 |
| `int w = Globals::w` local | 362 | 76 | 1892 |
| `__builtin_expect(c1,1)` (HEAD shape) | 105 | 58 | 1896 |
| `__builtin_expect(c2,1)` (HEAD shape) | 105 | 58 | 1896 |
| `__builtin_expect(c1&&c2,1)` (HEAD shape) | 306 | 74 | 1896 |
| `__builtin_expect(c3,1)` (HEAD shape) | 247 | 68 | 1896 |
| `__builtin_expect` on c1/c2/c12 (D shape) | 361-371 | 79-106 | 1892 |

### 3.4 Nested-`if` shape (reference declared after the first call)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| nested `if (c1) { Layout &layout = ...; if (c2) ... }` | 363 | 84 | 1896 |
| nested, all-direct c2 | 363 | 78 | 1892 |

### 3.5 Case-8 first-`if` 4th-condition rewrites (region A)

`y < lm + top + listEntryWidth` and ten rewrites (operand order,
association, `(int)` cast, `*1`, `( )`, `-` form, `>` form) all keep region
A at `r4`: raw 28-33, except the `-`/`>` forms which go to 832/47
(1896 B, 753 insns). None flips `r4 -> ip`.

### 3.6 Line-layout / formatting perturbations

`-g` debug values can influence the RA, so the block's line layout was
perturbed:

* one blank line inserted at each of the 23 line positions of the HEAD
  case-15 block: all 28/16/1896;
* `if` conditions on one line, `if` body on one line, whole block on one
  line, declaration on the same line as the `if`: all 28/16/1896;
* blank lines before/mid/after the all-direct D block: all 362/76/1892.

Line layout is not the lever.

## 4. Conclusion

HEAD is unchanged at **28/16/1896/754**. The residual is the Greedy-RA
keep-vs-remat tie-break on `%29` (layout value) vs `%508`
(`&Globals::layout`): the original keeps neither across the first call
(rematerialises both), which is the one combination the RA does not
produce from any source shape tested across w13603/w13612/w13652/w13666/
w13712 and this wave. Region A (`ip` vs `r4` in case 8) is an independent
tie-break; every spelling that flips it also permutes the `tbh` jump table
or the literal pool (`0x744..0x766`), so it is not a free win.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13739
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN15MenuTouchWindow12OnTouchBeginEiiPv
# scratch harness (outside the worktree):
#   /home/debian/GalaxyOnFire2/scratch/w13739/grid2.py
#   /home/debian/GalaxyOnFire2/scratch/w13739/bytediff.py
#   /home/debian/GalaxyOnFire2/scratch/w13739/lines2.py
```
