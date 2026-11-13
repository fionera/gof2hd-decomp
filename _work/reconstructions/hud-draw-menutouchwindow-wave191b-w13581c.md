# Hud::draw / MenuTouchWindow::OnTouchBegin — wave 191b (w13581c): fresh-angle sweeps

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

## 1. MenuTouchWindow — the 16-way case-15 matrix

The case-15 block has four independent source choices:

* `f1` = first-condition `buttonInsetX` spelling
  (`Globals::layout->buttonInsetX` vs `layout.buttonInsetX`)
* `lm` = second-condition `field_0xc_leftMargin` source (cached `layout.` vs
  fresh `Globals::layout->`)
* `tp` = second-condition `field_0x20_top` source (cached vs fresh)
* `th` = third-if `buttonInsetX` source (cached vs fresh)

All 16 combinations were built and measured. **`f1` is completely inert**:
`layout.buttonInsetX` and `Globals::layout->buttonInsetX` produce
bit-identical output (the reference `layout` is `*Globals::layout`, so the
compiler emits the same `ldr r2,[r4,#0]; ldr r2,[r2,#40]` either way).

Result matrix (raw / size), identical for both `f1` values:

| lm | tp | th | raw | size |
|---|---|---|---|---:|
| cached | fresh | fresh | **28** | **1896** |
| fresh  | cached | fresh | 29 | 1896 |
| cached | cached | fresh | 308 | 1892 |
| cached | fresh | cached | 291 | 1892 |
| fresh  | fresh | fresh | 362 | 1892 |
| fresh  | cached | cached | 292 | 1892 |
| fresh  | fresh | cached | 322 | 1888 |
| cached | cached | cached | 316 | 1888 |

So **1896 is reachable only when exactly one of `{lm,tp}` is cached**
(i.e. some `Globals::layout` value is live across the first call). The
original's instruction stream at `0x12a212-0x12a220` is the all-fresh shape
(`ldr r2,[sp,#16]; ldr r2,[r2,#0]; ldr r5,[r2,#12]; ldr r6,[r2,#32]`), yet
the original is 1896, not 1892. That is a `RegAllocGreedy` remat-vs-keep
tie-break: the original spills `&Globals::layout` to `sp+16` and reloads it
after the call even though the source value is dead, while our all-fresh
build keeps it in `fp`. No source spelling of the four choices above
reproduces "all-fresh *and* spilled".

Other new measurements this wave (all 1,896 B unless noted):

| variant | raw | aligned |
|---|---:|---:|
| HEAD (cached `leftMargin`, fresh `top`) | **28** | **16** |
| `(layout.field_0xc_leftMargin, Globals::layout->field_0xc_leftMargin)` comma | 362 | 76 (1892) |
| `layout.… ^ Globals::layout->… ^ Globals::layout->…` | 28 | 16 |
| `int &lm = layout.field_0xc_leftMargin;` used in c2 | 28 | 16 |
| `int top = Globals::layout->field_0x20_top;` before if, cached lm | 75 | 21 |
| `int top`/`int lm` locals before the if | 320 | 33 (1892) |
| `int top` + `int lm` locals before the if | 355 | 42 (1888) |
| `unsigned int img = heapBufB[field_0x1e0];` before the if | 351 | 76 (1888) |
| `int h = Canvas->GetImage2DHeight(...)` before the if | 617 | 155 |
| `Layout &layout2 = layout;` + `(void) layout2;` | 362 | 76 (1892) |
| `(void) layout;` after the third if | 362 | 76 (1892) |

Site A (`0x129fd2`, `ip` vs `r4` for `listEntryWidth`) is unchanged and
independent; even a perfect case-15 fix caps at raw 2.

## 2. Hud::draw — the anchor store is exactly the second `movne`

Decoding the two 28-byte damage blocks instruction-for-instruction shows the
anchor is not additive, it is a *substitution*:

```
orig  164184 tst.w  r1,#36        f011 0f24
      164188 itt    ne            bf1c
      16418a movne.w r0,#300      f44f 7096
      16418e strne.w r0,[sl,#1208] f8ca 04b8
      164192 tst.w  r1,#24        f011 0f18
      164196 itt    ne            bf1c
      164198 movne.w r0,#300      f44f 7096
      16419c strne.w r0,[sl,#1204] f8ca 04b4

ours  164184 tst.w  r1,#36        f011 0f24
      164188 mov.w  r0,#300       f44f 7096
      16418c it     ne            bf18
      16418e strne.w r8,[sp,#208] f8cd 80d0   <- anchor
      164192 str.w  r0,[sl,#1208] f8ca 04b8
      164196 tst.w  r1,#24        f011 0f18
      16419a it     ne            bf18
      16419c strne.w r0,[sl,#1204] f8ca 04b4
```

`mov.w r0,#300` and `movne.w r0,#300` share the encoding `f44f 7096`; the
anchor's `strne` occupies exactly the slot the original uses for the second
`movne`. So the block is 28 B either way and the anchor is "free" in size —
it is only the *bytes* that differ.

The anchor is also load-bearing for the frame: it forces `&Globals::status`
into `r8` at `0x164176`, which is the same reload the original has (dead
there, then re-materialised at `0x164210` into `r8` for the mission panel at
`0x164558`). That r8 occupancy is what makes `Globals::w` spill at `sp+60`
(`0x164336`/`0x16436a`) and gives the 224-byte frame. Removing the anchor
gives the exact damage block but drops the frame to 216 and puts
`Globals::w` in `r8` / `&Globals::status` in `fp` (raw 7,340, 9,328 B).

New measurements this wave (all 9,332 B / 3,226 insn unless noted):

| variant | raw | aligned |
|---|---:|---:|
| HEAD (`Status **volatile p = &Globals::status;` unbraced) | **19** | **6** |
| `void *volatile p = &Globals::status;` | 19 | 6 |
| `Status *volatile p = Globals::status;` (value) | 898 | 72 |
| `volatile int pad[2]; pad[0] = (int)(uintptr_t)&Globals::status;` | 19 | 6 |
| `(void) *(Status *volatile *)&Globals::status;` in the 0x24 if | 474 | 88 |
| `(void) *(Status *volatile *)&Globals::status;` after the if | 7,134 | 426 (9,336) |
| `(void) Globals::status;` before the block | 7,386 | 905 (9,328) |
| `Status *st = Globals::status;` before block, `st->` at panel | 8,045 | 1,150 (9,340) |
| `Status *st = Globals::status;` just before the panel | 7,386 | 903 (9,328) |
| `Status **stp = &Globals::status;` just before the panel | 7,386 | 905 (9,328) |
| `Status *st = Globals::status;` at function top, `st->` at panel | 9,253 | 1,708 (9,620) |
| `int w2 = Globals::w >> 1;` before `SetColor` in block 4 | 7,383 | 918 (9,328) |
| `int w2` + `int h2` before `SetColor` in block 4 | 7,616 | 552 (9,308) |
| `unsigned int hitFlags` | 468 | 86 |
| `if ((hitFlags & 0x24) != 0)` + `if ((hitFlags & 0x18) != 0)` | 468 | 86 |
| anchor in the 0x18 block (unbraced) | 7,125 | 427 (9,336) |
| anchor in the 0x18 block (braced) | 7,127 | 425 (9,336) |
| anchor + separate `if (hitFlags & 0x24)` store | 7,141 | 425 (9,336) |
| anchor + `else` store | 7,139 | 429 (9,336) |

Every attempt to keep the anchor's r8 occupancy while emitting the original
`itt ne / movne / strne` pair either grows the function to 9,336 B (anchor
outside the block) or collapses to the no-anchor allocation (9,328 B). The
shipped `Status **volatile p = &Globals::status;` unbraced form is still the
unique 19/6 shape.

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

Scratch harness: `scratch/w13581/` — `sweep_fresh.py` / `spec_fresh*.json`
(Hud), `mtw_sweep_fresh.py` / `spec_mtw*.json` (MTW, incl. the 16-way
`spec_mtw_all.json`), `dumpsym.sh` + `cur.dis` (symbol disassembly).
