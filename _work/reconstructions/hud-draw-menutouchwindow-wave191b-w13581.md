# Hud::draw / MenuTouchWindow::OnTouchBegin — wave 191b (w13581): no win landed

Worker `w13581`, branch `wave191b-w13581`, base master `b263b6071`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch `/home/debian/GalaxyOnFire2/scratch/w13581`.

**No code change landed.** Tree left clean at HEAD shape; `check` RATCHET PASS
(byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, parity 5370/5370).

| target | symbol | size | raw/aligned | insn |
|---|---|---:|---:|---:|
| `Hud::draw` | `_ZN3Hud4drawExxP9PlayerEgobjj` | 9,332 | 19 / 6 | 3226/3226 |
| `MenuTouchWindow::OnTouchBegin` | `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 1,896 | 28 / 16 | 754/754 |

## 1. Hud::draw — the anchor's real mechanism is the Globals::w spill

Corrects the wave-191b w13552 note: the original's `[sp,#0xd0]` **is** live
(it is a `String` temporary).  The real story:

* `sp+108` is written twice.  At `0x163c2c` it holds `Globals::globals`;
  at `0x164022` it is **overwritten with `&Globals::status`**
  (`ldr r0,[pc,#276]` -> GOT `_ZN7Globals6statusE`, `ldr r6,[r0]`).
* `0x164176 ldr.w r8,[sp,#108]` therefore loads `&Globals::status` into `r8`
  in **both** the original and HEAD.  The anchor's store
  `strne r8,[sp,#208]` writes `&Globals::status` into a dead `String`-temp
  slot at `sp+208`; it does not add a frame slot.
* The frame difference is elsewhere: HEAD has one extra live 4-byte frame
  object at `sp+60`, which is the **`Globals::w` spill**
  (`str r0,[sp,#60]` at `0x164336`, reloaded at `0x16436a`).  Without the
  anchor, clang keeps `Globals::w` in `r8` (`ldr.w r8,[r1]` at `0x16432e`)
  and puts `&Globals::status` in `fp`; the frame drops 224 -> 216 and every
  spill offset shifts.

So the anchor is not "8 bytes of padding": it is what keeps `r8` occupied by
`&Globals::status` across the first damage-indicator `GetImage2DWidth/Height`
pair, which forces `Globals::w` to spill.  HEAD's whole-function allocation is
byte-identical to the original's except the 8-instruction damage block.

### Measured this wave (all 9,332 B / 3,226 insn unless noted)

| variant | raw | aligned |
|---|---:|---:|
| HEAD (`Status **volatile p` unbraced, store unconditional) | **19** | **6** |
| remove anchor (exact original damage block) | 7,340 | 914 (9,328 B) |
| `volatile int pad;` unused | 7,340 | 914 (9,328 B) |
| `volatile int pad; (void)&pad;` | 7,340 | 914 (9,328 B) |
| `volatile int pad; (void)pad;` | 3,027 | 872 (9,332 B) |
| `volatile int pad = 0;` before the if | 2,892 | 875 (9,332 B) |
| `volatile int pad[2/3/4]; pad[0]=0;` before the if | 2,892 | 875 (9,332 B) |
| `volatile long long / double pad` before the if | 6,796 / 6,558 | (9,336 B) |
| `volatile int pad = 0;` inside braced if | 2,910 | 890 (9,332 B) |
| `volatile int pad; if(..) pad=0;` | 2,894 | 876 (9,332 B) |
| `volatile int pad = 0;` at function top / end | 8,877 / 8,872 | (9,292 / 9,296 B) |
| `volatile char/short/unsigned p = (T)(intptr_t)&Globals::status;` in if | 468 | 86 (9,332 B) |
| `Status **volatile p;` declared outside, assigned in if | 369 | 363 (9,332 B) |
| `volatile short/char/uint` anchor (16-bit store) | 468 | 86 (9,332 B) |
| `volatile Status **p` / `Status *volatile *p` / `Status *volatile &p` | 7,494 | 906 (9,320 B) |
| `volatile int p = 300;` in if | 3,043 | 889 (9,332 B) |
| `volatile unsigned p = sideImage;` / `= hitFlags;` | 7,355 | 917 (9,328 B) |
| `volatile Status *p = Globals::status;` (value) | 898 | 72 (9,332 B) |
| `volatile void *p = Globals::Canvas;` | 3,076 | 895 (9,332 B) |
| `volatile unsigned int frontImage` | 8,745 | 1,357 (9,296 B) |
| `volatile unsigned int sideImage` | 8,653 | 1,032 (9,320 B) |
| `if (__builtin_expect(hitFlags & 0x24, 0/1))` | 7,340 | 914 (9,328 B) |
| `(hitFlags & 0x24) != 0` / `||` of 4|0x20 / braced / switch / while | 7,340 | 914 (9,328 B) |
| split `4` + `0x20` into two ifs | 7,376 | 936 (9,340 B) |
| split `8` + `0x10` into two ifs | 7,571 | 889 (9,352 B) |
| `int w = Globals::w;` before/inside block 1, use `w` | 7,340 | 914 (9,328 B) |
| `volatile int w = Globals::w;` inside block 1 | 6,767 | 908 (9,336 B) |
| `*(volatile int *)&Globals::w >> 1` | 3,037 | 867 (9,332 B) |
| `*(volatile int *)&Globals::h >> 1` | 3,038 | 867 (9,332 B) |
| swap sideImage/frontImage ternaries, `int` types, `(int)` casts | 7,340-7,394 | (9,328 B) |
| move the four damage ifs before sideImage/frontImage | 7,988 | 702 (9,348 B) |
| `int hitFlags` first, then sideImage/frontImage | 3,082 | 888 (9,332 B) |
| `float fade` clamp reverted to `if` form (wave-190 change) | 32 | 21 |

Dead end: the anchor's 4-byte store exactly pays for the shared `mov r0,#300`
between the `0x24` and `0x18` blocks, so the block is 28 B either way.  Any
source shape that keeps a 4-byte stack object alive with **zero code** would
win, but clang 7 `-Oz` deletes every unused/address-only alloca tried, and
every *used* pad either shifts the whole stack by 4 (raw ~2.9k) or changes the
allocation completely.

## 2. MenuTouchWindow::OnTouchBegin — two independent regalloc ties

`fn_diff` gives 28 raw / 16 aligned.  `mdiff` (instruction-index diff) shows
exactly two sites:

**Site A** (`0x129fd2`, case 8, last hit-test term):
```
orig  ldr.w ip, [sl, #340]   ; this->listEntryWidth (0x154)
ours  ldr.w r4, [sl, #340]
```
`ip` vs `r4` for a single-use value; 2 bytes.

**Site B** (`0x12a1e0-0x12a254`, case 15).  Original vs HEAD:

| value | original | HEAD |
|---|---|---|
| `this->heapBufB` | r3 | r2 |
| `this->field_0x1e0` | r6 | r3 |
| `Globals::layout` value | r2 (reloaded after 1st call) | r6 (live across 1st call) |
| `Globals::w` | r5 | r4 |
| `buttonInsetX` | r2 | r5 |
| `&Globals::Canvas` | fp | fp |
| `&Globals::layout` | sp+16 | sp+16 |

Both spill `&Globals::layout` to `sp+16`; the original reloads `Globals::layout`
through it after the first `GetImage2DWidth` (`ldr r2,[sp,#16]; ldr r2,[r2]`),
HEAD keeps it in callee-saved `r6`.  Because HEAD keeps it in `r6`, the
original's `fp = &Globals::Canvas` is already reproduced — the wave-191 note's
"HEAD keeps fp = layout" no longer matches the current build.

The all-direct shape (`layout` reference removed) is **1892 B**, not 1896: with
`fp = Globals::layout` the reload is one instruction instead of two, so the
4-byte difference is code, not frame (the prologue/epilogue are identical,
`stmdb sp!,{r3,r4,r5,r6,r7,r8,r9,sl,fp}` + `add sp,#20`).

### Measured this wave (all 1,896 B / 754 insn unless noted)

| variant | raw | aligned |
|---|---:|---:|
| HEAD (`Layout &layout` used for leftMargin) | **28** | **16** |
| all-direct (no `Layout&`) | 362 | 76 (1,892 B) |
| `Layout *layout` / `Layout *const layout` / `Layout &layout` used for c1 buttonInsetX | 28 | 16 |
| `Layout &layout` used for `field_0x20_top` | 29 | 15 |
| `Layout &layout` used for c3 buttonInsetX only | 28 | 16 |
| `*(volatile int *)&layout.field_0xc_leftMargin` | 35 | 21 |
| `volatile Layout *layout` for leftMargin | 35 | 21 |
| `Layout *volatile layout` for leftMargin | 1,759 | 258 (1,924 B) |
| `Layout *&layout` | 362 | 76 (1,892 B) |
| swap `&&` operands | 375 | 103 (1,892 B) |
| nested `if` instead of `&&` | 39 | 21 |
| `this->scrollbarHit = cond ? 1 : 0` / `bool hit = cond` | 335 | 83 (1,892 B) |
| second if before first | 358 | 91 (1,892 B) |
| `x < Canvas->GetImage2DWidth(...) + layout->buttonInsetX` | 42 | 22 |
| `unsigned int heapImage = this->heapBufB[...]` local | 351 | 76 (1,888 B) |
| `int leftMargin = layout.field_0xc_leftMargin` local | 320 | 33 (1,892 B) |
| `int top` / `int bi` locals | 75 / 329 | 21 / 75 |
| `layout` used only for c1 buttonInsetX (dead before call) | 362 | 76 (1,892 B) |
| `layout` used only in the second if | 322 | 74 (1,888 B) |
| case-8 sum reorders / parens / `(int)` cast | 28-33 | 16-17 |

Dead end: the original needs `Globals::layout` reloaded after the first call
(so it must not live in a callee-saved register) **and** `fp = &Globals::Canvas`
(which requires the reference to be live across the call so `fp` is not reused
for `Globals::layout`).  Every source shape tried satisfies one and not the
other; the tie is a `RegAllocGreedy` remat-vs-keep decision plus the case-8
`ip`-vs-`r4` pick, and site A alone caps any case-15-only win at raw 2.

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

Scratch harness: `scratch/w13581/` — `Hud.cpp.base`, `MenuTouchWindow.cpp.base`,
`measure.sh`, `sweep_hud2.py`, `sweep_gen.py`, `sweep_mtwg.py`, `mdiff.py`,
`dumpsym.sh`, `hud_orig.dis`/`hud_noanchor.dis`/`mtw_orig.dis`/`mtw_alldirect.dis`,
`paa_head.txt`/`paa_noanchor2.txt` (`-print-after-all` dumps).
