# Hud::draw / MenuTouchWindow::OnTouchBegin — wave 191b (w13581), fresh-angle sweep

Worker `w13581`, branch `wave191b-w13581`, base master `b263b6071`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch `/home/debian/GalaxyOnFire2/scratch/w13581`.

**No code change landed.** Both targets remain size-exact plateaus; tree left
clean at HEAD shape. Gate unchanged: byte_exact 4507/4524, linked_exact
4507/4524, size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0,
parity 5370/5370.

| target | symbol | size | raw/aligned | insn |
|---|---|---:|---:|---:|
| `Hud::draw` | `_ZN3Hud4drawExxP9PlayerEgobjj` | 9,332 | 19 / 6 | 3226/3226 |
| `MenuTouchWindow::OnTouchBegin` | `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 1,896 | 28 / 16 | 754/754 |

## 1. Hud::draw — the anchor's 8-byte frame is a spill consequence, not an alloca

This wave confirms and sharpens the w13581/w13563 notes with the frame-object
lists from the final MIR (`-print-after-all`):

* `%stack.1` (the anchor alloca, 4 bytes) is **stack-colored into the
  `String time` slot at `sp+0xd0`** and is listed as `fi#1: dead` in the
  final frame objects.  It adds **no** frame slot of its own.
* The 224 vs 216 frame delta is entirely the **extra 4-byte spill slot**
  (`Globals::w` in the third damage-indicator block, `%stack.60` =
  `str r0,[sp,#60]` at `0x164336`, reloaded at `0x16436a`) plus 8-byte frame
  rounding.  The no-anchor build keeps `Globals::w` in `r8` and has 15 spill
  slots; HEAD has 16.

New measurements (all 9,332 B / 3,226 insn unless noted), on top of HEAD:

| variant | raw | aligned |
|---|---:|---:|
| HEAD `Status **volatile p = &Globals::status;` (unbraced) | **19** | **6** |
| `volatile int pad[2]; pad[0] = (int)(intptr_t)&Globals::status;` braced, store outside | 19 | 6 |
| `void *volatile pad[2]; pad[0] = (void*)&Globals::status;` braced, store outside | 19 | 6 |
| `volatile long long pad = (long long)(intptr_t)&Globals::status;` unbraced | 7,121 | 417 (9,336 B) |
| `volatile double pad = 0.0;` unbraced | 2,924 | 869 (9,332 B) |
| `Status ** volatile p = (Status **)&Globals::status;` (explicit cast) | 468 | 86 |
| `void *volatile p = (void*)&Globals::status;` (explicit cast) | 468 | 86 |
| `intptr_t volatile p = (intptr_t)&Globals::status;` | 468 | 86 |
| `Status **const volatile p = &Globals::status;` | 468 | 86 |
| `auto volatile p = &Globals::status;` / `decltype(...) volatile p` | 19 | 6 |
| `Status *volatile p = (Status*)&Globals::status;` | 19 | 6 |
| `*(void *volatile *)&this->field_0x4b8 = (void*)&Globals::status;` | 466 | 86 |
| `String pad;` (unused, unbraced) | 3,322 | 964 |
| `volatile unsigned int savedColor` | 8,900 | 1,646 (9,204 B) |
| `volatile int dividerOffset` | 8,925 | 1,305 (9,224 B) |
| `volatile int armorBarY` / `volatile unsigned short armorY` | 8,853 / 8,822 | (9,228 / 9,232 B) |
| `volatile int pad = 0;` at function top | 8,877 | 1,001 (9,292 B) |
| `*(volatile int *)&this->field_0x4b8 = 300;` + `*(volatile int *)&this->field_0x4b4 = 300;` | 2,928 | 865 (9,332 B, duplicate `strne`!) |
| `int w = Globals::w;` in the third damage block, use `w` | 7,332 | 972 (9,328 B) |
| `long long tl = timeLeft;` / `long long ft = frameTime;` (uses replaced) | 19 | 6 |

The anchor's *exact* source spelling matters: an explicit cast on the value
(`(void*)`, `(intptr_t)`, `(Status**)`) changes the IR enough to lose the
19/6 shape (raw 468).  `auto`/`decltype`/`Status *volatile` with a cast keep
it.  An 8-byte volatile anchor (`volatile int pad[2]`, `volatile void *pad[2]`)
also keeps raw 19 — the frame is 224 either way because the alloca is colored
into the String slot; the extra frame is the spill, not the alloca.

Dead end (unchanged): no source shape found that emits the shipped plain
damage block (`itt ne / movne / strne` twice) **and** keeps the anchor
allocation.  The anchor's volatile store (4 B) exactly pays for the shared
`mov r0,#300` (4 B), and its live range is what forces the `Globals::w` spill.

## 2. MenuTouchWindow::OnTouchBegin — second-condition associativity matters

The case-15 second condition's **add association** is load-bearing:

* `top + (leftMargin + height)` (HEAD, parenthesised) → raw **28**, 1,896 B.
* `top + leftMargin + height` (left-assoc, no inner parens) → raw **45**,
  1,896 B (HEAD and GLG both).
* `(top + leftMargin) + height` (explicit left-assoc) → raw 45.

The original's disassembly at `0x12a226` is `adds r1,r6,r5` (top + leftMargin)
then `add r0,r1` (+ height), i.e. left-assoc — but the left-assoc source
gives raw 45 because the register allocation then differs.  The parenthesised
HEAD form is the only 28/16 shape.

New measurements (all 1,896 B / 754 insn unless noted):

| variant | raw | aligned |
|---|---:|---:|
| HEAD (`Layout &layout`, cached `field_0xc_leftMargin`, fresh `top`) | **28** | **16** |
| `layout.field_0x20_top` cached, fresh `leftMargin` (GLG) | 29 | 15 |
| all-direct (no `Layout &layout`) | 362 | 76 (1,892 B) |
| `layout` used for c1 `buttonInsetX`, c2 fresh | 362 | 76 (1,892 B) |
| `int lm = layout.field_0xc_leftMargin;` before the if | 320 | 33 (1,892 B) |
| `int top = Globals::layout->field_0x20_top;` before the if | 75 | 21 |
| `PaintCanvas *canvas = Globals::Canvas;` (all 3 calls) | 316 | 40 (1,892 B) |
| `int idx = this->field_0x1e0;` before the if | 317 | 75 (1,892 B) |
| `int w = Globals::w;` before the if | 28 | 16 |
| `(void) layout;` dead use before/after | 362 | 76 (1,892 B) |
| `layout` used in c3 only (c2 all-direct) | 322 | 74 (1,888 B) |
| split first call: `int bw = Globals::Canvas->GetImage2DWidth(...)` | 362 | 97 (1,888 B) |
| split first call + fresh c2 | 354 | 97 (1,884 B) |
| c2 operand orders (`lm+top+h`, `h+lm+top`, `h+top+lm`, `lm+h+top`) | 362–380 | 76–111 (1,892 B) |

Dead end (unchanged): the original needs `fp = &Globals::Canvas` **and** the
cached layout value dead.  Every source shape that frees the cached value
(all-direct, `LGG`, `lp` locals, volatile reloads) drops `fp` to
`&Globals::layout` (1,892 B); every shape that keeps it gives 28/16.  The
residual is a `RegAllocGreedy` remat-vs-keep tie-break plus the case-8
`ip`-vs-`r4` pick (site A alone caps a case-15-only win at raw 2).

## 3. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13581
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
    _ZN3Hud4drawExxP9PlayerEgobjj _ZN15MenuTouchWindow12OnTouchBeginEiiPv
# raw=19 aligned=6  size=9332/9332 insn=3226/3226
# raw=28 aligned=16 size=1896/1896 insn=754/754
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch harness: `scratch/w13581/` — `sweep3.py`/`sweep_hud2.py` (Hud),
`mtw_sweep.py`/`mtw_sweep2.py` (MTW), `Hud.cpp.base`,
`MenuTouchWindow.cpp.base`, `paa_head.txt` / `paa_noanchor2.txt` (Hud MIR),
`mtw_paa_alldirect.txt` (MTW MIR).
