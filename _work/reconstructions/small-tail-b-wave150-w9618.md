# Small tail B wave 150 (w9618): MenuTouchWindow / ModStation / SpaceLounge —
# new hypotheses on the three residuals (analysis only, no source change)

Status: **analysis only, no landed `src/` change**. Worktree `wt/w9618`, base
`7eed2966e` (wave 149). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Read with `small-tail-wave149-w9516.md`, `modstation-extab-lp-wave149-w9520.md`,
`spacelounge-drawlounge-wave130-w7682.md`.

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0. Body sizes unchanged: `OnTouchBegin` 1896 B, `ModStation` 1008 B,
`drawLounge` 1948 B.

Metrics: raw same-address diff bytes (`/dev/shm/fdiff.py`) and
instruction-index alignment (`_work/reconstructions/align_diff.py`).

| fn | raw | aligned |
|---|---:|---:|
| `MenuTouchWindow::OnTouchBegin` (`0x129bd4`) | 29 | 15 |
| `ModStation::ModStation` C1 (`0xd52d0`) | 43 | 12 |
| `SpaceLounge::drawLounge` (`0x170d38`) | 176 | 42 |

## 1. MenuTouchWindow: the residual is a single Greedy-RA remat tie-break

The 15 mismatched instructions are exactly two clusters:

* `0x129fd2`/`0x129fd8` — case-8 fourth hit-test: original `ldr.w ip,[sl,#340]`
  (`this->listEntryWidth`), ours `ldr.w r4,[sl,#340]`.  Pure register name;
  `r4` is dead in both at that point.
* `0x12a1e0..0x12a24c` — case-15 scrollbar hit test.  Original: `layout`
  value in `r2` (dead at the first `GetImage2DWidth` call), `w` in `r5`,
  `heapBufB` in `r3`, index in `r6`, `Globals::Canvas` value in `fp`.  Ours:
  `layout` value in `r6` (live across the first call, used for the
  `field_0x20_top` load), `w` in `r4`, `heapBufB` in `r2`, index in `r3`,
  `Globals::Canvas` value in `fp`.

The root is the `Layout &layout` reference: it keeps the `Globals::layout`
value live across the first call (`%29`), so Greedy-RA puts it in a
callee-saved register (`r6`).  The original reloads `Globals::layout` for
both `field_0xc_leftMargin` and `field_0x20_top` (`%29` dead at the call),
but still rematerialises `&Globals::layout` (`%508`) from `[sp,#16]` and
keeps `Globals::Canvas` in `fp`.

### New probes this wave (all reverted; size 1896 B unless noted)

Confirmed the wave-129/132 two-bucket result is exhaustive over the
direct/reference choice for the four `Globals::layout` accesses (A =
`buttonInsetX` in the first clause, B = `field_0xc_leftMargin`, C =
`field_0x20_top`, D = `buttonInsetX` in the third `if`), 16 combinations:

| shape | raw | aligned | size |
|---|---:|---:|---:|
| **HEAD (C via `Layout &`; A,B,D direct)** | **29** | **15** | 1896 |
| C via `Layout *` (pointer) | 29 | 15 | 1896 |
| C via `Layout *` + A via pointer | 29 | 15 | 1896 |
| A via `Layout &`, C direct | 358 | 76 | 1892 |
| A via `Layout *`, C direct | 358 | 76 | 1892 |
| A,B via `Layout &`, C direct | 28 | 16 | 1896 |
| A,B via `Layout *`, C direct | 358 | 76 | 1892 |
| B via `Layout &`, C direct | 28 | 16 | 1896 |
| all direct | 358 | 76 | 1892 |
| all direct + `int right = w - inset` (wave-132 D) | 358 | 76 | 1892 |
| C direct, D via `Layout &` | 314 | 74 | 1888 |
| all direct, D via `Layout &` | 314 | 74 | 1888 |
| `int top = layout.field_0x20_top` before the call | 1892 B / 78 structural | | |
| both `top` and `lm` cached before the call | 1888 B / 96 structural | | |

New spellings tried this wave (all reverted):

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `__builtin_expect(whole &&, 0)` (all-direct) | 363 | 84 | 1896 |
| `__builtin_expect(whole &&, 0)` (wave-132 D) | 363 | 84 | 1896 |
| `__builtin_expect(second clause, 0)` | 275 | 66 | 1896 |
| `__builtin_expect(second clause, 1)` | 106 | 57 | 1896 |
| nested `if (c1) { if (c2) ... }` | 359 | 78 | 1892 |
| `*(volatile int *)&Globals::layout->field_0x20_top` | 363 | 93 | 1892 |
| `*(volatile int *)&Globals::layout->field_0xc_leftMargin` | 364 | 93 | 1892 |
| case-8: `this->listEntryWidth + leftMargin + top` | 34 | 16 | 1896 |
| case-8: `leftMargin + this->listEntryWidth + top` | 34 | 16 | 1896 |
| case-8: `top + leftMargin + this->listEntryWidth` | 29 | 15 | 1896 |

**Conclusion.** Any shape that kills `%29` at the first call also makes
Greedy-RA keep `%508` (`&Globals::layout`) in `fp`, which costs the
`Globals::Canvas` colouring and 4 bytes.  The two properties the original
has simultaneously (`%29` dead, `%508` remat'd) are not reachable by the
direct/reference/volatile/expect spellings tested (now >800 cumulative
across waves 118-150).  The case-8 `ip`/`r4` tie is a separate Greedy-RA
order artefact and does not move with any case-8 operand permutation.

## 2. ModStation: the CMOV copy is a scheduler order, not source order

The 12 mismatched instructions are three artefacts (unchanged from w9520):

1. literal-pool order: original `Pitch, YawIPad, Yaw`, ours `Pitch, Yaw,
   YawIPad` (the `select` materialisation order);
2. `mov r0,r3` (the CMOV false-arm init, `%121 = COPY %165`) is before
   `cmp r1,#0` in the original, after it in ours;
3. `str.w r8,[sp,#184]` (race spill) and `mov r4,r2` (pitch save for the
   farKey rotation) are sunk after the yaw load in the original, hoisted
   before it in ours.

New shapes tried this wave (all reverted; all 1008 B unless noted):

| shape | raw | aligned | size |
|---|---:|---:|---:|
| HEAD | **43** | **12** | 1008 |
| `yawTable = YawIPad; if (!iPad) yawTable = Yaw;` | 43 | 12 | 1008 |
| `if (iPad) yawTable = YawIPad; else yawTable = Yaw;` | 43 | 12 | 1008 |
| `iPad != 0` / `!!iPad` / `iPad > 0` / `iPad >= 1` / `iPad == 0` | 43 | 12 | 1008 |
| `(unsigned char)iPad` | 43 | 12 | 1008 |
| `(iPad & 1)` | 572 | 24 | 1008 |
| `iPad == 1` | 688 | 55 | 1000 |
| `(signed char)iPad > 0 ? YawIPad : Yaw` | 688 | 54 | 1000 |
| farKey rotation uses `*(const float *)&pitchBits` | 43 | 12 | 1008 |
| farKey rotation re-reads `pitchTable[race]` | 43 | 12 | 1008 |
| `yawTable` declared before `pitchTable` | 46 | 13 | 1008 |
| `yawBits` before `pitchBits` | 744 | 40 | 1000 |
| swap both bit and float copies | 744 | 40 | 1000 |
| drop `volatile` on yaw load | 44 | 18 | 1008 |
| drop `volatile` on pitch load | 666 | 60 | 1008 |

All non-HEAD spellings canonicalise to the same `select (icmp eq 0), Yaw,
YawIPad` IR.  The three artefacts are instruction-selection/scheduling
choices (`DoInstructionSelection` walks the DAG backwards; the machine
scheduler puts the CMP first because it is on the branch's critical path).
No source shape tested moves the `COPY` before the `CMP` while keeping
`it eq` and the roles.  Note that `(signed char)iPad > 0` flips the pool
order but changes the condition to `it gt` and shrinks the body to 1000 B.

## 3. SpaceLounge: faithful clamp gets the right arm but swaps pad/x

The 42 mismatched instructions are the pad/x colouring plus the right-edge
clamp arm:

* original: `pad` in `r9`, `x` in `r5`, right arm `itt gt; addgt.w r1,r6,sl;
  subgt r5,r0,r1` (2 instructions, reuses `sl = pad*2` from `top`);
* HEAD (`pad + pad`): `pad` in `fp`, `x` in `r9`, right arm
  `itttt gt; movgt r1,#0; subgt.w r1,r1,fp,lsl#1; subgt r1,r1,r6;
  addgt.w r9,r1,r0` (4 instructions).

New shapes tried this wave (all reverted; 1948 B unless noted):

| shape | raw | aligned | size |
|---|---:|---:|---:|
| **HEAD `pad + pad`** | **176** | **42** | 1948 |
| faithful `width + pad * 2` | 445 | 54 | 1948 |
| `w - inset - width - pad * 2` | 445 | 54 | 1948 |
| `w - inset - (pad * 2 + width)` | 445 | 54 | 1948 |
| `w - inset - width - pad - pad` | 464 | 64 | 1944 |
| `w - (inset + width + pad * 2)` | 196 | 47 | 1948 |
| `int x = centerX;` hoisted before the strings (HEAD clamp) | 176 | 42 | 1948 |
| `int x = centerX;` hoisted (faithful clamp) | 445 | 54 | 1948 |
| `centerX`/`top` moved down to the clamp | 1134 | 118 | 1948 |

The faithful `pad * 2` clamp is the only spelling that reuses `sl` and
emits the 2-instruction arm, but it re-colours `pad` to `fp` and `x` to
`r9` and adds two `mov r5,fp` in the two clamp arms (dropping the join
`mov r5,fp`), shifting every address after the join by 2 bytes.  Hoisting
`x`'s live range does not change the colouring (the compiler sinks the
declaration).  The wave-130 weight-window analysis (`agent > pad > x`) is
still the only known lever and remains unreachable from source.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9618
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w9618/fdiff.py <mangled>
python3 /dev/shm/w9618/align_diff.py <mangled>
```

Scratch (not committed), `/dev/shm/w9618/`: `fdiff.py`, `align_diff.py`,
`showdiff.py`, `batch.py`, `batch2.py`, `batch3.py`, `batch_ms*.py`,
`batch_sl*.py`, `run*.py`, `base/` (pristine sources).

## 5. Files changed

* `_work/reconstructions/small-tail-b-wave150-w9618.md` (this note).

No `src/` change: byte 4507 / linked 4507, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, RATCHET PASS.
