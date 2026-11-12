# HangarWindow::render — wave 189 (w13333): the 0x14a960 tail shift

Worktree `wt/w13333`, branch `wave189-w13333`, base master `374b49e55`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B, 3,734 insns.
Task: remove the extra `ldr.w sl,[sl]` at `0x14a960` (the `switch (count)`
anchor) without disturbing the 1,244 ship-guard family.

## Result: no size-exact raw win — `src/` left at the base

The tail shift was re-derived from Ghidra/objdump and a large set of source
shapes was measured.  Every shape that removes the extra load either loses
bytes (9,696/9,692) or introduces a 2-byte phase shift at the loop setup that
costs more `.text` than the tail fix saves.  The best total remains the base
at **raw 1,510 / aligned 643 / `.text` 73,999 / `.ARM.extab` 639 / total
74,658**.

| variant | raw | aligned | size | `.text` | `.ARM.extab` |
|---|---:|---:|---:|---:|---:|
| **base (kept)** | **1,510** | **643** | **9,700** | **73,999** | **639** |
| nolay (use `Globals::layout` directly) | 3,532 | 657 | 9,696 | 119,785 | 780 |
| nolay + exp2b | 3,556 | 642 | 9,696 | 119,809 | 780 |
| nolay + f4a (size-exact) | 1,703 | 528 | 9,700 | 74,192 | 641 |
| nolay + exp2b + f4a (size-exact) | 1,727 | 513 | 9,700 | 74,216 | 641 |
| nolay + exp2b + f4a + swap Strings | 1,730 | 502 | 9,700 | 74,219 | 641 |
| `lay` after the switch (value form) | 3,563 | 681 | 9,692 | — | — |
| `lay` at y site, `Globals::layout` at DI | 1,852 | 593 | 9,700 | 74,341 | 646 |
| `lay` at DI site, `Globals::layout` at y | 8,721 | 1,440 | 9,704 | — | — |
| `Layout *const` / `auto` / `const Layout *` | 1,510 | 643 | 9,700 | 73,999 | 639 |
| `unsigned int count` | 1,510 | 643 | 9,700 | 73,999 | 639 |

All numbers are `tools/fn_diff.py` raw/aligned/size plus the ELF-section diff
(`.text`/`.ARM.extab`); `.rel.plt` is untouched and `.ARM.extab` stays
156,312 B at `0x1a3b78` for every 9,700 variant.

## Anatomy of the shift (re-confirmed)

Base vs original, `0x14a960`:

```
orig  14a960  bic.w r0, r9, #0x80000000        ours  14a960  ldr.w sl, [sl]
      14a964  cmp   r0, #4                            14a964  bic.w r0, r9, #0x80000000
      14a966  bhi.n 14a9aa                            14a968  cmp   r0, #4
      14a968  tbb   [pc, r0]                          14a96a  bhi.n 14a9ae
                                                      14a96c  tbb   [pc, r0]
```

Base also has an extra `ldr.w sl,[sp,#128]` at the loop tail `0x14ac64`
(the original has none).  The two extra loads are exactly the two address-form
derefs the original performs inside the loop body:

```
orig  14ab02  ldr.w r1, [sl]        ; r1 = Globals::layout
      14ab24  ldr   r1, [r1, #32]   ; layout->field_0x20_top
...
orig  14abc2  ldr.w r2, [sl]        ; r2 = Globals::layout
      14abc8  ldr   r2, [r2, #44]   ; layout->field_0x2c_rowHeight
```

Base instead keeps `Globals::layout`'s **value** in `sl` (loaded once at the
loop head) and uses `ldr.w r1,[sl,#32]` / `ldr.w r2,[sl,#44]`.  So the original
keeps `sl = &Globals::layout` (the *address*) across the loop; base keeps the
*value*.

The source difference is exactly the loop-local `Layout *lay = Globals::layout;`
in the buy-list loop.  Removing it (use `Globals::layout` directly, "nolay")
produces the original's two address-form derefs, but clang then spills
`&Globals::layout` to a stack slot (`stack.98`) and gives `sl` to one of the
`String` temporaries.  The IR difference is one extra
`%x = load %class.Layout*, @_ZN7Globals6layoutE` in the loop body.

## Why nolay is not size-exact

The original's loop setup uses two low registers for the two `String`
temporaries:

```
orig  14a912  add r6, sp, #312      ; String temp
      14a916  add r5, sp, #300      ; String temp
```

nolay uses `sl` for the second one, and `add.w sl, sp, #300` is 4 B instead of
2 B, so the whole loop body starts 2 B late (`0x14a93c` vs `0x14a93a`).  That
2-byte phase shift is what makes nolay's `.text` diff explode to 119,785.

The missing bytes relative to the original are the freeCredits spill
`str.w sl,[sp,#128]` at `0x14ac9c` (the original spills `&Globals::layout`
there before reusing `sl` for the button width; nolay already has
`&Globals::layout` at `sp+132` from `0x14a8ec` and so emits no spill).

## The f4a compensation (size-exact, tail fixed, but 2-byte shift)

Adding a `Layout *flay = Globals::layout;` at the top of the `else if
(this->freeCreditsActive)` block and using it for the first (empty-string)
`DrawString` adds the missing 4 B and makes nolay size-exact:

```cpp
} else if (this->freeCreditsActive) {
    Layout *flay = Globals::layout;
    ...
    canvas->DrawString(hangarFont(), String(""), textX, textY + flay->field_0x4, false);
```

`nolay + exp2b + f4a` is 9,700 B, 3,734 insns (exact), and has **no**
`ldr.w sl,[sl]` at the loop head and **no** loop-tail restore.  But the
`add.w sl, sp, #300` 2-byte shift remains, so raw is 1,727 and `.text` 74,216
— a net loss vs base.  The freeCredits block also re-allocates (`r6` holds the
layout value instead of `r1`, stack slots `sp+108/sp+116` instead of
`sp+108/sp+120`), so the compensation does not reproduce the original's
freeCredits shape.

## What was tried (all measured, all size-exact unless noted)

- `Layout *lay` declaration forms: `Layout *const`, `const Layout *`, `auto`,
  `Layout *&`, `Layout **`, `Layout *volatile`, `Layout *const volatile` —
  `const`/`auto`/`const Layout *` are identical to base (1,510); `Layout *&`
  canonicalises to nolay (9,696); `volatile` variants lose size.
- `lay` placement: at loop head, before the `for`, before the switch, after
  the switch, right before the y computation, right before the `DrawImage2D`.
- Mixed sites: `lay` at y + `Globals::layout` at DI (1,852 / 9,700),
  `lay` at DI + `Globals::layout` at y (8,721 / 9,704).
- Third uses of `lay` / `Globals::layout` in the `count==4` block and in the
  x computation.
- `x` form: `(1-col)` vs expanded `mla` form (`exp2b`); original uses the
  expanded form, but it is 3 B worse on base and does not change the sl issue.
- `String` declaration order (description/price swapped).
- `count` type: `unsigned int` (identical), `unsigned char` (3,441).
- Loop forms: `__builtin_expect` vs plain `<=4` vs `<5` vs `while`; the
  expect anchor is required for 9,700.
- freeCredits compensation targets: `baseX`, `baseY`, `y`, `rh`, `f4a`,
  `f4b`, `formatCredits`, and all 1/2/3-combinations of them.

The blocker is the register allocator's choice to spill `&Globals::layout`
(rematerialisable) and keep a `String` temporary in `sl`.  Any shape that
keeps `sl = &Globals::layout` (base) also keeps the value form and therefore
the loop-head/tail loads.  The two are coupled through the same allocation
decision.

## Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN12HangarWindow6renderEv
cmake --build cmake-build-match --target check -j2
```

Scratch (not committed): `/home/debian/GalaxyOnFire2/scratch/w13333/`
(`HangarWindow.base.cpp`, `v_nolay.cpp`, `v_nl_exp2b_f4a.cpp`,
`v_nl_f4a.cpp`, `v_lay_after_switch.cpp`, `sweepx*.py`, `secdiff.py`,
`regiondiff.py`, `sizediff.py`, `side.py`).
