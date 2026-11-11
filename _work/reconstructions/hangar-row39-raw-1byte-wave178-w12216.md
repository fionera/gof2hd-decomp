# HangarWindow::render — 1-byte raw recovery with row 39 exact (wave 178, w12216)

Worktree `wt/w12216`, branch `wave178-w12216`, base master `1b48f990f`
(wave-177 merge).  Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B, 3,734 insns,
`.ARM.extab` record 948 B / 156 rows.

Entry state (wave-177 landing, `1b48f990f`): raw **2,244**, aligned **382**,
size **9,700/9,700**, insns **3,734/3,734**, LSDA record diff **90**,
row 39 **`(2508, 150, 9664)`** exact, `.text` **79,251**, `.ARM.extab`
**731**, total **80,002**.

## Result

One source change in the tail of `render()`:

```cpp
(*this->buttons)[11]->setAlwaysPressed(!*(volatile bool *)&Globals::options[78]);
```

instead of

```cpp
(*this->buttons)[11]->setAlwaysPressed(!Globals::options[78]);
```

| metric | before | after |
|---|---:|---:|
| raw same-address diff | 2,244 | **2,243** |
| instruction-index alignment | 382 | 384 |
| function size | 9,700 / 9,700 | **9,700 / 9,700** |
| instructions | 3,734 / 3,734 | **3,734 / 3,734** |
| its `.ARM.extab` record diff | 90 | **90** |
| row 39 (orig / ours) | (2508,150,9664) | **(2508,150,9664)** |
| `.text` section diff | 79,251 | **79,250** |
| `.ARM.extab` section diff | 731 | **731** |
| total differing bytes | 80,002 | **80,001** |

`check` green: byte_exact 4507/4524, linked_exact 4507/4524, size-exact
4524/4524, data_exact 843/843, imports 207, lint CLEAN, drift 0, parity
5370/5370.  `.rel.plt` byte-identical (0), `.ARM.exidx` 0, `.rodata` 0,
`.data` 0, `.bss` 0.  `.ARM.extab` size/base exact (`0x26298` @ `0x1a3b78`),
file size exact (2,187,328 B).

The aligned metric regresses 382 -> 384 (the volatile read changes the
instruction schedule around `0x14a880`); the raw same-address metric — the
primary residual metric for this function — improves by one byte.  The
`.ARM.extab` record is untouched because the row-39 range and the call-site
table are unchanged.

## Why the volatile read wins one byte

The original spills `Globals::options` to `[sp,#124]` before the credit
dialog block:

```
14a882 ldr  r2,[r0,#0]     ; r2 = Globals::options
14a884 ldr  r0,[r1,#4]
14a886 str  r2,[sp,#124]   ; spill
14a888 ldrb r1,[r2,#78]    ; options[78]
```

Our wave-177 shape keeps `Globals::options` in `r9` instead:

```
14a880 ldr.w r9,[r0]
14a886 ldrb  r1,[r9,#78]
```

The volatile read forces the compiler to re-materialise the load at the
`setAlwaysPressed` call site; the resulting schedule happens to match three
of the original bytes at `0x14a889..0x14a88b` while differing in two bytes at
`0x14a884..0x14a885`, for a net -1 byte.  It is a codegen coincidence, not a
structural recovery: the tail register allocation (`r9` vs `r4` for
`&this->buttons`, `Globals::options` spilled vs kept) is otherwise unchanged.

## What was tried (all size-exact unless noted)

The wave-177 note documents the row-39/raw tradeoff: the original block order
(ship body inline, non-ship `switch` out of line) is reachable only with
`getShip()` in the outer cleanup scope, and every such shape either loses the
2-byte `cbz` (9,704 B) or sinks `getShip()` into the buttons-body landing pad
(row 39 = 118, record 676).  This pass re-tested the most promising
untried shapes:

| shape | raw | aligned | size | row39 |
|---|---:|---:|---:|---|
| wave-177 base | **2,244** | 382 | 9,700 | exact |
| **landed: volatile `options[78]`** | **2,243** | 384 | **9,700** | **exact** |
| volatile `options[78]` + operand flip | 2,241 | 623 | 9,700 | (2508,152,9664) |
| `int upgTab` after the guard body | 6,609 | 586 | 9,708 | (2508,154,9672) |
| `int upgTab` after the guard body, no hints | 6,619 | 593 | 9,708 | — |
| near-miss (`tab != 0 && !(tab==3 && ship==getShip)`, upg inside) | 6,511 | 611 | **9,704** | (2508,154,9668) |
| near-miss + both guard hints + upg hint | 6,310 | 625 | 9,696 | (2508,150,9660) |
| near-miss + outer hint only | 6,310 | 625 | 9,696 | (2508,150,9660) |
| nested `if (tab != 0) { if (...) {...} }`, upg outside | 6,727 | 733 | 9,700 | (2508,362,9666) |
| nested + outer hint | 6,748 | 807 | 9,700 | (2508,154,9666) |
| `goto upgcheck` before the guard | 6,773 | 737 | 9,700 | (2508,362,9666) |
| `goto` + `int upgTab` inside the body | 6,556 | 621 | 9,704 | (2508,154,9668) |
| `if (__builtin_expect(item->isShip(), 1))` | 8,692 | 2,327 | 9,696 | (2528,154,9650) |
| `bool ship = item->isShip(); if (ship)` | 2,402 | 810 | 9,700 | (2508,100,9656) |
| `if (this->selectedItem == item)` | 2,245 | 383 | 9,700 | exact |
| `bool *options = Globals::options;` local | 2,244 | 382 | 9,700 | exact |
| `static_cast<volatile bool &>` read | 2,244 | 382 | 9,700 | exact |
| `options[78] ^ 1` / `1 - options[78]` | 2,244 | 526 | 9,700 | exact |
| `volatile bool` local then `!v` | 2,246 | 387 | 9,700 | exact |
| `!*(volatile bool **)&Globals::options)[78]` | 3,616 | 586 | 9,704 | (2508,150,9668) |

The near-miss is the closest structural shape: it reproduces the original
ship head and body byte-for-byte except for the `tab == 0` edge, which is
`cmp r0,#0; beq.w <merge>` (6 B) instead of `cbz r0,<near upg check>` (2 B),
making the function 9,704 B.  Its `str buttonOffset` / `ldr tab` order is
also swapped relative to the original (`ldr` before `str`), so the `cbz`
cannot target the `cmp` directly.  Every attempt to reorder the snapshot
(`int upgTab` at each body position), add hints, or use `goto` either keeps
the 6-byte edge or changes the body landing pad.

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

Shape generators used this pass live in
`/home/debian/GalaxyOnFire2/scratch/w12216/` (`h.py`, `sweep_after.py`,
`sweep_guard.py`, `sweep_nested.py`, `sweep_nm.py`, `sweep_nm2.py`,
`sweep_sink.py`, `sweep_sink2.py`, `sweep_snapos.py`, `sweep_goto.py`,
`sweep_vol2.py`, `sweep_bool.py`, `sweep_opt78.py`, `sweep_op.py`,
`sweep_isship2.py`, `sweep_selitem.py`, `sweep_reg.py`, `sweep_shipblk.py`).
