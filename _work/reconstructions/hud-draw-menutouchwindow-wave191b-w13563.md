# Hud::draw / MenuTouchWindow::OnTouchBegin — wave 191b (w13563)

Worker `w13563`, branch `wave191b-w13563`, base master `a7c4defa9`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch `/home/debian/GalaxyOnFire2/scratch/w13563`.

**No code change landed.** Both targets are size-exact plateaus; the tree is
clean at HEAD shape and the gate is unchanged (`check` **RATCHET PASS** —
byte_exact 4507/4524, linked_exact 4507/4524, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, parity 5370/5370).

| target | symbol | size | raw/aligned | insn |
|---|---|---:|---:|---:|
| `Hud::draw` | `_ZN3Hud4drawExxP9PlayerEgobjj` | 9,332 | 19 / 6 | 3226/3226 |
| `MenuTouchWindow::OnTouchBegin` | `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 1,896 | 28 / 16 | 754/754 |

## 1. Hud::draw — the anchor is a dead-value store; the frame delta is a spill consequence

New evidence this wave (from the final MIR, `-print-after-all`,
`scratch/w13563/filt_head.txt` and `filt_noanchor.txt`):

* The anchor `Status **volatile p = &Globals::status;` is a **volatile store
  of an undef value**: `%ir.11` is never read, so the RA is free to pick any
  register.  It picks `r8`, which is `[sp,#0x6c]` = `&Globals::status`
  (`%stack.48`), loaded at `0x164176`.  The stored value is therefore the
  *address* `&Globals::status`, not `Globals::status`.
* In the **original**, the same `ldr.w r8,[sp,#0x6c]` at `0x164176` exists
  but is **dead** (`r8` is clobbered at `0x1641e4` by `Globals::w` and the
  next use is `0x164208`).  The original also has a second dead reload at
  `0x164210`.  Both are RA reload artifacts, not source statements.
* The frame delta is **not** a missing source object.  HEAD and the original
  use *identical* direct `[sp,#...]` offsets and identical spill counts
  (`str/ldr rX,[sp,#0x3c..0x78]`).  The no-anchor build is 8 bytes smaller
  because its allocation spills one fewer value (15 int slots vs 16) and the
  String slots shift 0x74..0xc8 -> 0x7c..0xd0.  So the extra 8 bytes are a
  *consequence* of the allocation, not a source-level frame consumer.
* The anchor's alloca is stack-colored into the `String time` slot at
  `[sp,#0xd0]` (it adds no slot of its own); removing it removes a *spill*
  slot, not the alloca.

### Measured this wave (Hud; all 9,332 B / 3,226 insn unless noted)

| variant (inside `if (hitFlags & 0x24)`, store unconditional) | raw | aligned |
|---|---:|---:|
| HEAD `Status **volatile p = &Globals::status;` | **19** | **6** |
| `(void)*(Status *volatile *)&Globals::status;` (volatile value read) | 25 | 8 |
| `(void)*(volatile int *)&Globals::status;` | 25 | 8 |
| `__sync_synchronize();` | 7,330 | 920 (9,328 B) |
| `volatile int pad = 300;` | 3,043 | 889 |
| `volatile int pad = this->field_0x4b8;` | 7,469 | 597 (9,352 B) |
| `int *volatile pad = &this->field_0x4b8;` | 7,370 | 548 (9,348 B) |
| `Status *volatile pad = Globals::status;` | 898 | 72 |
| `int volatile pad = hitFlags;` | 7,355 | 917 (9,328 B) |
| `unsigned int volatile pad = sideImage;` | 7,355 | 917 (9,328 B) |
| `unsigned int volatile pad = frontImage;` | 6,720 | 1,082 (9,328 B) |
| `long long volatile pad = frameTime;` | 8,731 | 2,068 (9,248 B) |
| function-scope `Status **volatile pad;` stored in the if | 369 | 363 |
| function-scope `volatile int pad;` stored in the if | 7,341 | 903 (9,328 B) |
| function-scope `volatile int pad; pad = 300;` | 3,027 | 875 |
| `(void)*(volatile int *)&this->field_0x4b8;` | 7,335 | 919 (9,328 B) |
| `(void)*(volatile int *)&hitFlags;` | 7,362 | 915 (9,328 B) |
| `(void)*(volatile int *)&Globals::w;` | 6,691 | 933 (9,336 B) |
| `(void)*(PaintCanvas *volatile *)&Globals::Canvas;` | 3,076 | 901 |
| unconditional `volatile int pad = 300;` between the two ifs | 3,023 | 874 |
| unconditional `Status **volatile pad = &Globals::status;` between the ifs | 7,125 | 809 (9,336 B) |
| `const int damage = 300;` shared by both ifs (plain block) | 7,386 | 905 (9,328 B) |
| `ego->field_0x20` direct in all four ifs (plain block) | 7,386 | 905 (9,328 B) |
| braced `{ volatile pad; field = 300; }` | 7,141 | 425 (9,336 B) |
| braced `{ field = 300; volatile pad; }` | 7,139 | 427 (9,336 B) |
| anchor moved into the `& 0x18` block (plain `& 0x24`) | 7,125-7,127 | 425-427 (9,336 B) |

The shipped anchor remains the unique 19/6 shape.  The original's damage
block is exactly the *plain* source (`tst / itt ne / movne / strne` twice);
the anchor buys the original's global allocation by adding one live range
(`&Globals::status` in `r8`) across the block, and pays for the second
`movne` with its own store.  No source spelling found that keeps the
allocation and drops the store.

## 2. MenuTouchWindow::OnTouchBegin — original is all-direct with `fp = &Globals::Canvas`

New evidence this wave:

* The original's case-15 block is the **all-direct** source shape (no
  `Layout &layout` reference): it reloads `&Globals::layout` from `[sp,#0x10]`
  at `0x12a1bc`, `0x12a212` and `0x12a23c`.
* The original's `r11` (`fp`) is **`&Globals::Canvas`** (loaded once at
  `0x12a1ec`, reused at `0x12a1f8`, `0x12a21a`, `0x12a242`); `&Globals::layout`
  is rematerialised from `[sp,#0x10]` after each call.
* The 4-byte size difference vs the all-direct build is exactly the
  high-register encoding: the original's `ldr.w r0,[r11]` (4 B) vs the
  all-direct build's `ldr r0,[r5]` (2 B) in the second and third conditions.
  So 1,896 B requires `fp = &Globals::Canvas`.
* HEAD already has `fp = &Globals::Canvas`; its only residual is the cached
  `Layout &layout` value kept in `r6` across the first call and used for
  `field_0xc_leftMargin` in the second condition.  The original reloads
  `Globals::layout` there and reads both fields from it.

### Measured this wave (MTW; all 1,896 B / 754 insn unless noted)

| variant | raw | aligned |
|---|---:|---:|
| HEAD (`layout.field_0xc_leftMargin`, fresh top) | **28** | **16** |
| `layout.field_0x20_top` cached, fresh leftMargin (`GLG`) | 29 | 15 |
| `layout.buttonInsetX` cached in c1, fresh c2/c3 (`LGG`) | 362 | 76 (1,892 B) |
| both c2 fields cached (`GLL`) | 308 | 73 (1,892 B) |
| c3 `layout.buttonInsetX` cached | 322 | 74 (1,888 B) |
| all 16 cached/direct combinations of c1/c2-top/c2-lm/c3 | best 28/16, next 29/15 | |
| `PaintCanvas *canvas = Globals::Canvas;` used in all three calls | 359 | 75 (1,892 B) |
| `PaintCanvas &canvas = *Globals::Canvas;` used in all three calls | 359 | 75 (1,892 B) |
| `Layout *lp` + `canvas` locals, all accesses via them | 348 | 82 (1,884 B) |
| `Layout *lp` used in c1/c3 only | 322 | 74 (1,888 B) |
| `(void)Globals::Canvas;` extra use | 362 | 76 (1,892 B) |
| `PaintCanvas *canvas` used in c2/c3 only | 359 | 75 (1,892 B) |
| `this->scrollbarHit = (x > ... && y < ...);` bool assignment | 363 | 84 |
| nested `if (x > ...) { if (y < ...) ... }` | 363 | 78 (1,892 B) |
| `(*(Layout *volatile *)&Globals::layout)->...` in c2+c3 | 128 | 78 (1,896 B) |
| same, c2 only | 127 | 76 (1,896 B) |
| `Layout *lp = *(Layout *volatile *)&Globals::layout;` in c2 | 376 | 82 (1,888 B) |
| volatile reload in c3 only | 364 | 78 (1,892 B) |
| `int tp = Globals::layout->field_0x20_top;` before the if | 320 | 33 (1,892 B) |
| `int lm = Globals::layout->field_0xc_leftMargin;` before the if | 320 | 33 (1,892 B) |
| both `tp` and `lm` before the if | 355 | 42 (1,888 B) |
| `tp` + `canvas` locals | 345 | 77 (1,888 B) |
| `PaintCanvas *canvas` hoisted before the scrollSlots loop | 373 | 88 (1,888 B) |
| `Layout *lp` hoisted before the scrollSlots loop | 413 | 91 (1,888 B) |

`GLG` is the closest non-HEAD shape: same size, same instruction sequence as
the original in c2, only a register permutation in c1 (`r2/r3/r4/r6` vs the
original's `r3/r6/r5/r2`).  It is 29 raw / 15 aligned, i.e. one byte worse
raw and one better aligned than HEAD, so it is not a gate win.

### Dead end

The residual is a `RegAllocGreedy` remat-vs-keep tie-break: to get
`fp = &Globals::Canvas` the allocator needs a long-lived layout value in
`r6` (HEAD / `GLG`), which is exactly the value the original does not have.
Every source shape that frees `r6` (all-direct, `LGG`, `GLL`, `lp` locals,
volatile reloads) either drops `fp` to `&Globals::layout` (1,892 B) or keeps
the cached field load.

## 3. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13563
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
    _ZN3Hud4drawExxP9PlayerEgobjj _ZN15MenuTouchWindow12OnTouchBeginEiiPv
# raw=19 aligned=6 size=9332/9332 insn=3226/3226
# raw=28 aligned=16 size=1896/1896 insn=754/754
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Note: the worktree's CMake cache defaulted to
`wt/w13563/../_work/bins/...` (i.e. `wt/_work/...`), which does not exist;
`check` fails until `-DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
is re-passed.

Scratch harness: `scratch/w13563/` — `sweep.py` (Hud), `mtw.py` (MTW),
`fd.py` (aligned diff with disasm context), `cc.sh` (single-TU compile with
the project flags), `filt_head.txt` / `filt_noanchor.txt` (Hud final MIR,
`-print-after-all`), `b8`–`b14.py` (Hud sweeps), `m7`–`m15.py` (MTW sweeps).

---

## 4. Follow-up (same wave, second pass): the frame object must be *code-free*

New evidence, from a fast single-TU harness (`scratch/w13563/fast.py`: compile
`Hud.cpp` with the exact project flags, `objdump -j .text.<sym>`, report
size / `sub sp` frame / `frontImage` spill offset (`fi`) / damage block).

**The original's extra 4 bytes of code is a single dead instruction, and the
extra 4 bytes of frame is a *separate*, code-free object.**  `orig_hud.txt`
vs `noanchor_hud.txt` differ by exactly one instruction in the damage region:

```
orig  164174: 0788        lsls r0, r1, #0x1e
orig  164176: f8dd 806c   ldr.w r8, [sp, #0x6c]   <- dead (r8 clobbered at 0x1641e4)
orig  16417a: bf44        itt mi
plain 164174: 0788        lsls r0, r1, #0x1e
plain 164176: bf44        itt mi
```

`orig` is otherwise **byte-identical** to the plain build (`noanchor`), and the
two differ by 9332 - 9328 = 4 bytes.  So the target is:

* damage block = the plain 28-byte shape (`tst / itt ne / movne / strne` twice),
* frame 224 with `frontImage` at `[sp,#0x60]` (`fi=96`) — i.e. the *exact*
  HEAD frame layout,
* plus one code-free 4-byte stack object.

**Any block whose size is not exactly 28 bytes is fatal.**  The extra
instruction shifts every following branch/PC-relative immediate, and raw
explodes.  `p_stat` below (32-byte block, otherwise the exact original frame
layout) measures raw **7092** even though its damage block is byte-identical
except for one extra `strne.w r8,[sp,#0xd0]`.

### Measured (fast harness; `fi` = `frontImage` spill offset; original = 96)

| variant | size | frame | fi | damage block |
|---|---:|---:|---:|---|
| HEAD (anchor) | 9332 | 224 | 96 | `tst / mov / it ne / strne r8,[sp,#0xd0] / str` |
| plain | 9328 | 216 | 88 | plain |
| `p_stat` = `if (h&0x24){ Status**volatile pad=&Globals::status; f=300; }` | 9336 | 224 | **96** | `ittt ne / strne.w r8,[sp,#0xd0] / movne / strne` |
| `i24_pad_first` = `{ volatile int pad=hitFlags; f=300; }` | 9332 | 216 | 88 | `ittt ne / strne r1,[sp,#0xc8] / movne / strne` |
| `i24_vpp_first` (same as `p_stat`) | 9336 | 224 | 96 | as `p_stat` |
| `vpp_unread_b1` = `{ Status**volatile pad; (void)pad; }` before `&1` | 9332 | 216 | 88 | plain + `ldr r0,[sp,#0xc8]` |
| `vpp_initread_b1` = `{ Status**volatile pad=&Globals::status; (void)pad; }` | 9336 | 224 | 96 | plain + `str`/`ldr` |
| `fs_vintinit` = fn-scope `volatile int pad=hitFlags;` | 9332 | 224 | 92 | plain |
| `fsv_store_i24` = fn-scope `volatile int pad;` + store in the if | 9332 | 224 | 92 | plain |
| `vintinit_before1` | 9332 | 224 | 92 | plain |
| `__builtin_prefetch(&hitFlags)` | 9336 | 224 | – | plain |
| `volatile int pad = hitFlags;` before `&1` | 9332 | 224 | 92 | plain |

Key facts:

1. **Unused locals are eliminated** (`volatile int pad;`, `int pad;`,
   `char pad[4];`, `long long pad;`, `String pad;` at fn scope is the exception:
   String's ctor/dtor add 220 B).  `volatile int pad; (void)pad;` emits a load
   but the slot is **stack-coloured into the last `String` slot** → frame stays
   216.
2. **Function-scope volatile locals do get a real slot** (frame 224), but the
   layout is `fi=92` (`frontImage` at `[sp,#0x5c]`) — the new slot lands in the
   middle of the frame, not at the top.
3. **Only a block-scope `Status **volatile` store inside the `&0x24` if
   reproduces the exact original frame layout (`fi=96`)** — because the value is
   the GOT address `&Globals::status`, which is not rematerialisable, so the
   slot stays live inside the `if` and is placed at `[sp,#0xd0]`.  But the store
   itself is 4 bytes, so the block becomes 32 B → 9336.
4. **`volatile int` inside the if is stack-coloured** (`strne r1,[sp,#0xc8]`,
   frame 216) because `hitFlags` is rematerialisable.
5. The original's extra frame object is a **register spill slot**, not an
   alloca: HEAD's final frame has 16 four-byte spill slots, plain has 15
   (`scratch/w13563/fa.txt` vs `fb.txt`).  The anchor's own alloca is
   stack-coloured (`fi#1: dead`).

So the remaining gap is exactly: a source shape that makes the allocator spill
one extra value (giving the 16th spill slot and the 224 frame) **and** emits the
dead `ldr.w r8,[sp,#0x6c]`, **without** adding a 29th byte to the damage block.
The anchor achieves the first two but pays a 4-byte store for them.

### MTW re-check

Re-dumped pristine HEAD (`B_c2lm` = HEAD source) at `0x12a1b0`:
`fp = Globals::Canvas` (`ldr.w fp,[r1]` at `0x12a1ec`), `&Globals::layout` in
`[sp,#0x10]`, and c2 = `ldr r2,[sp,#0x10] / ldr r4,[r6,#0xc] (cached lm) /
ldr r2,[r2] / ldr r5,[r2,#0x20] (fresh top)`.  The original reads **both** c2
fields through the freshly reloaded `Globals::layout`.  Confirmed: all 16
cached/direct combinations of c1 / c2-lm / c2-top / c3 were already covered;
best is HEAD 28/16, next `GLG` 29/15.

Scratch additions: `fast.py` (single-TU size/frame/fi probe, ~1.3 s per
variant), `hb5`–`hb20.py` (Hud frame-object sweeps), `mtb5.py` (MTW combos).
