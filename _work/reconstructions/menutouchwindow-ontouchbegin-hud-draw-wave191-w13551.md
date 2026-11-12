# MenuTouchWindow::OnTouchBegin / Hud::draw wave 191 (w13551): both residuals are allocator anchors

Date: 2026-11-12. Worktree `wt/w13551`, base `e79990318`. Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Both targets are size-exact and unchanged by this wave.  No source win was
found; the tree is clean and the gate is unchanged
(byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, parity 5370/5370).

## 1. `MenuTouchWindow::OnTouchBegin` (`_ZN15MenuTouchWindow12OnTouchBeginEiiPv`)

Baseline: **raw 28 / aligned 16**, 1896 B exact, 754/754 insns.

The 18 structural diff lines are exactly the four regions in
`menutouchwindow-ontouchbegin-wave132-w7719.md`:

* Region A (`0x129fd2`): `this->listEntryWidth` gets `ip` in the original,
  `r4` in ours (case 8, first hit-test, fourth term).
* Region B (`0x12a1e0-0x12a204`): register permutation in the case-15 first
  condition; original `r3=heapBufB`, `r6=field_0x1e0`, `r5=Globals::w`,
  ours `r2=heapBufB`, `r3=field_0x1e0`, `r4=Globals::w`.
* Region C (`0x12a214-0x12a226`): original reloads `Globals::layout` after the
  first call and reads **both** `field_0xc_leftMargin` and `field_0x20_top`
  fresh; HEAD reads `leftMargin` from the cached `Layout &layout` (`%29`) and
  only `top` fresh.
* Region D (`0x12a246`): downstream `r5` vs `r4` for `buttonInsetX`.

### New measurements this wave (all 1896 B / 754 insns unless noted)

| variant | raw | aligned |
|---|---:|---:|
| HEAD (cached `leftMargin`, fresh `top`) | **28** | **16** |
| all-direct case 15 (`top + leftMargin + height`, no `Layout&`) | 362 | 76 (1892 B) |
| HEAD + `int w = Globals::w;` used in c1 | 28 | 16 |
| HEAD + `int right = Globals::w - layout->buttonInsetX;` | 28 | 16 |
| HEAD + `(layout.field_0xc_leftMargin - layout.field_0xc_leftMargin)` in c3 | 28 | 16 |
| HEAD + `(layout.field_0xc_leftMargin ? 0 : 0)` in c3 | 28 | 16 |
| HEAD + `this->listEntryWidth | 0` / `+ 0` / `?:` | 28 | 16 |
| HEAD + `(unsigned)/(int)/(+)/` cast on `listEntryWidth` | 28-30 | 16-17 |
| case-8 first-if term permutations | 28-33 | 16-17 |
| local `listEntryWidth` before the first if | 794 | 107 (1892 B) |
| local `listEntryWidth` after the first if | 34 | 18 |
| swap `listTopY` / `buttonInsetX` declarations | 30 | 18 |
| `PaintCanvas *canvas` local in case 15 | 360 | 80 (1892 B) |
| `uint32_t *hb` + `unsigned idx` locals | 349 | 79 (1884 B) |
| `layout.buttonInsetX` in c1+c3, fresh leftMargin/top | 322 | 74 (1888 B) |
| `layout.field_0xc_leftMargin == Globals::layout->field_0xc_leftMargin` extra `&&` | 398 | 92 (1912 B) |

### Dead end

The exact original instruction stream is the **all-direct** shape, but clang
then keeps `&Globals::layout` in `fp` and moves the `Globals::Canvas` value to
`r5`, shrinking the function to 1892 B.  HEAD keeps `fp = Canvas` and remats
the GOT address (matching the original), but only because `%29` (the cached
layout value) is live across the first call; the original has `%29` dead
**and** `fp = Canvas`.  Every source shape tried (this wave plus the ~770 in
waves 118-132) either keeps `%29` live (28 diffs) or lets it die (1892 B,
hundreds of diffs).  The residual is a Greedy-RA remat-vs-keep tie-break
coupled to `%29` liveness, not a reachable source-shape difference.

## 2. `Hud::draw` (`_ZN3Hud4drawExxP9PlayerEgobjj`)

Baseline: **raw 19 / aligned 6**, 9332 B exact, 3226/3226 insns.
The only differing bytes are the damage-direction block
`0x164184-0x16419f`:

```
orig  164184  tst.w  r1, #36
      164188  itt    ne
      16418a  movne.w r0, #300
      16418e  strne.w r0, [sl, #1208]   ; field_0x4b8
      164192  tst.w  r1, #24
      164196  itt    ne
      164198  movne.w r0, #300
      16419c  strne.w r0, [sl, #1204]   ; field_0x4b4
ours  164184  tst.w  r1, #36
      164188  mov.w  r0, #300
      16418c  it     ne
      16418e  strne.w r8, [sp, #208]    ; anchor store
      164192  str.w  r0, [sl, #1208]    ; unconditional
      164196  tst.w  r1, #24
      16419a  it     ne
      16419c  strne.w r0, [sl, #1204]
```

The anchor is the unbraced

```cpp
    if (hitFlags & 0x24)
        Status **volatile p = &Globals::status;
        this->field_0x4b8 = 300;
```

which makes the `field_0x4b8` store **unconditional** in C++ semantics (the
original store is `strne`).  Removing the anchor gives the exact shipped
damage block but the function becomes 9328 B / raw 7340 (frame 216 vs 224,
all stack offsets shift by -4/-8).

### New measurements this wave (all 9332 B unless noted)

| anchor variant | raw | aligned |
|---|---:|---:|
| current `Status **volatile p = &Globals::status;` | **19** | **6** |
| `void *volatile p = &Globals::status;` | 19 | 6 |
| `volatile char/short/int/unsigned p = (T)(uintptr_t)&Globals::status;` | 19 | 6 |
| `void *volatile p = &Globals::Canvas;` | 3070 | 900 |
| `void *volatile p = &hitFlags;` | 2950 | 893 |
| `void *volatile p = &this->field_0x4ac;` | 3043 | 890 |
| `void *volatile p = &Globals::w;` | 6691 | 933 (9336 B) |
| `void *volatile p = &Globals::layout;` | 7401 | 903 (9324 B) |
| `void *volatile p = this;` / `ego` | 7330 | 920 (9328 B) |
| `volatile int dummy = 0;` | 7335 | 914 (9328 B) |
| `volatile int dummy = 300;` | 3043 | 889 |
| `volatile int dummy = hitFlags;` | 7355 | 917 (9328 B) |
| `volatile int dummy = Globals::w;` | 7402 | 842 (9340 B) |
| `Status *volatile dummy = Globals::status;` | 898 | 72 |
| `volatile long long p = ...` | 7093 | 425 (9336 B) |
| anchor in `if (hitFlags & 1)` | 8900 | 1568 (9456 B) |
| anchor in `if (hitFlags & 2)` | 8742 | 1896 (9636 B) |
| anchor in `if (hitFlags & 0x18)` | 7076 | 410 (9336 B) |
| anchor before all four ifs | 8070 | 1066 (9360 B) |
| anchor after all four ifs | 7112 | 809 (9336 B) |
| braced `{ anchor; store; }` | 7092 | 408 (9336 B) |
| braced `{ store; anchor; }` | 7090 | 410 (9336 B) |
| `(void) Globals::status;` inside braced if | 7340 | 914 (9328 B) |
| `Status *p = Globals::status; (void)p;` inside braced if | 7340 | 914 (9328 B) |
| `(void) *(volatile int *)&Globals::status;` inside braced if | 7093 | 410 (9336 B) |
| unused `char/int/volatile char pad[8]` at function top | 7340-8842 | 914-1041 (9296-9328 B) |

### Dead end

The anchor is load-bearing for the whole function: it keeps the
`&Globals::status` GOT address live in `r8` across the damage block and
reproduces the shipped 224-byte frame / stack offsets / register allocation.
Without it the damage block is exactly right but the frame drops to 216 and
the allocation cascades (raw 7340).  Every alternative anchor value, type,
position, brace form and no-op statement measured this wave is worse; the
shipped `&Globals::status` address store is the unique 19/6 shape.  The real
source almost certainly had a non-anchor live range or 8 bytes of unused
frame padding that the anchor is proxying; no such source shape was found in
waves 127-190 or this wave.

## 3. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13551
cmake --preset default -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
    _ZN15MenuTouchWindow12OnTouchBeginEiiPv _ZN3Hud4drawExxP9PlayerEgobjj
# raw=28 aligned=16 size=1896/1896 insn=754/754
# raw=19 aligned=6  size=9332/9332 insn=3226/3226
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w13551` (measure harness,
`mtw.base.cpp`, `hud.base.cpp`, MIR dumps, sweep scripts).
