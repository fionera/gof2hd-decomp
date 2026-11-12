# Hud::draw / MenuTouchWindow::OnTouchBegin — wave 183 (w12719)

Worker `w12719`, branch `wave183-w12719`, base master `9555474d8`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch `/home/debian/GalaxyOnFire2/scratch/w12719`.

Status: **no `src/` change landed for these two functions** (the ModStation
constructor win is in `modstation-ctor-wave183-w12719.md`).  Both targets were
re-swept and remain at their wave-182 raw counts.

| target | VMA | size | raw/aligned |
|---|---|---:|---:|
| `_ZN3Hud4drawExxP9PlayerEgobjj` | `0x163b90` | 9332 | **32 / 21** |
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | `0x129bd4` | 1896 | **28 / 16** |

## 1. Hud::draw — residual 32 = damage 20 + dock 12

### Damage block (`hitFlags & 0x24`)

The wave-176 address-store anchor is still the unique size-exact shape:

```cpp
if (hitFlags & 0x24)
    Status **volatile p = &Globals::status;
    this->field_0x4b8 = 300;
```

Original vs ours (`0x164184`):

```
O: tst r1,#36 ; itt ne ; movne r0,#300 ; strne r0,[sl,#0x4b8]
U: tst r1,#36 ; mov.w r0,#300 ; it ne ; strne r8,[sp,#0xd0] ; str.w r0,[sl,#0x4b8]
U: tst r1,#24 ; it ne ; strne r0,[sl,#0x4b4]
```

The original has a genuinely dead `ldr.w r8,[sp,#0x6c]` at `0x164176`
(reload of the spilled `&Globals::status`), then two plain conditional
`movne`/`strne` pairs.  The anchor reproduces the reload but pays for it with
a conditional store that also lets the compiler hoist the shared
`mov r0,#300` out of the `0x24` block.  Size is exact (24 B region either
way); the 20 differing bytes are the store + hoisted mov.

Measured this wave (all size 9332 unless noted):

| damage shape | raw |
|---|---:|
| landed address-store anchor | **32** |
| `(void) *(Status * volatile *) &Globals::status;` braced + store after | 38 |
| `(void) *(Status * volatile *) &Globals::status;` unbraced (wave-175 deref) | 156 |
| plain `if` (no anchor) | 7339 (size 9328) |
| plain + `__atomic_signal_fence` between the `&1`/`&2` ifs | 7339 (size 9328) |
| plain + `(void) *(volatile int *) &hitFlags;` | 2942 (size 9332) |
| plain + `(void) *(volatile int *) &Globals::iPad;` | 2921 (size 9332) |
| plain + volatile read of `Globals::w`/`h`/`layout`/`Canvas`/`status` | 7339-8349 |
| anchor type `volatile int/long/short/char/Status**` | 32 |
| anchor `volatile long long` / `float` / `double` | 7092-8706 |
| `if (hitFlags & 0x24) { ... }` braced anchor | 9336 |

The missing 4 B in the plain shape is exactly the dead reload.  A bare
4-byte reload with no accompanying store/load was not source-expressible:
every non-DCE'd use of `&Globals::status` costs a store (4 B) or a value load
(4 B) in addition to the reload.  The anchor's store is what keeps the
reload alive, and the store replaces the second block's `movne`, keeping the
size exact.

### Dock-transfer block

Residual is the `&Globals::Canvas` / `top` register pair only:
original `r5 = &Globals::Canvas`, `r6 = top`; ours `r6 = &Globals::Canvas`,
`r5 = top`.  Every other instruction (barW `r8`, barH `fp`, y `r9`,
barYOffset `r4`) is identical.  Swept this wave (all size 9332, 3226 insns):

| shape | raw |
|---|---:|
| landed (`int top = field_0x3e2; int y = top*2;`) | **32** |
| `top` before the two image calls | 110 |
| `top` between the two image calls | 96 |
| `top` after the fade block | 50 |
| `top` after `SetColor` | 1492 (size 9328) |
| `no top` (use `field_0x3e2*2` directly) | 32 |
| `unsigned`/`long`/`const` top or y | 32-33 |
| `short` top / `short` y | 33 / 626 |
| `PaintCanvas *canvas` / `*const` / `&canvas` local | 32-48 |
| `PaintCanvas **canvasPtr` / `PaintCanvas *&canvasRef` | 32 |
| `barH` declared before `barW` | 33 |
| `frameH`/`textH` `short/short` | 930 |

The pair is a greedy-colouring tie: at `0x16594c` the compiler copies
`fp` (`&Globals::Canvas`) into a callee-saved register before overwriting
`fp` with `barH`.  It picks `r6` in our build and `r5` in the original.
No declaration/type/order spelling tested changes that choice without
changing the size or the rest of the block.

## 2. MenuTouchWindow::OnTouchBegin — residual 28

Two independent groups:

* **case 3/8 (2 B)**: `ldr.w ip,[sl,#0x154]` + `add r3,ip` (original) vs
  `ldr.w r4,[sl,#0x154]` + `add r3,r4` (ours).  `this->listEntryWidth` is
  short-lived; `r4` is dead in both.  Pure allocator tie-break.
* **case 15 (26 B)**: the scrollbar block.  The landed wave-162 shape keeps
  `fp = &Globals::Canvas` and rematerialises `&Globals::layout` from
  `sp+16`, but caches the `Globals::layout` value in `r6` and uses it for
  `field_0xc_leftMargin`; the original reloads the value for both
  `field_0xc_leftMargin` and `field_0x20_top`.  This is the wave-112
  "local optimum" still in force.

Swept this wave (all size 1896 unless noted):

| case-15 shape | raw |
|---|---:|
| landed `Layout &layout` used for block-2 leftMargin | **28** |
| all-direct (no local) | 362 (size 1892) |
| local used for block-1 inset only | 362 (size 1892) |
| local used for block-3 inset only | 362 (size 1892) |
| local used for block-2 top only | 29 |
| local used for block-2 top+leftMargin | 308 (size 1892) |
| `Layout *lp = Globals::layout` for block-2 leftMargin | 28 |
| `Layout *lp` for all three blocks | 316 (size 1888) |
| `Layout **lp = &Globals::layout` | 362 (size 1892) |
| `Layout *volatile *lp` | 127 |
| `volatile Layout &layout` | 35 |
| `Layout *volatile lp` | 1752 (size 1916) |
| case-3 `listEntryWidth` local before the second if | 35 |
| case-3 `int lw` local for both ifs | 1417 (size 1892) |
| case-3 expression reorder (`lw` first) | 533 |

The all-direct shape has the original's exact pre-RA data flow but greedy
assigns `&Globals::layout` to `fp`, saving 4 B; the local-reference shape
raises register pressure enough to spill the address but caches the value.
The two effects remain coupled, as documented in
`menutouchwindow-ontouchbegin-wave112-w6017.md`.

## 3. Gate

No `src/` change for these two functions.  Full `check` after the ModStation
commit: **RATCHET PASS** — byte_exact 4507/4524, linked_exact 4507/4524,
size 4524/4524, stub_zero_size 0, missing 0, wrong_type 0, extra 0,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370,
`.rel.plt` byte-identical.

## 4. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
    _ZN3Hud4drawExxP9PlayerEgobjj _ZN15MenuTouchWindow12OnTouchBeginEiiPv
# Hud raw=32 aligned=21 size=9332/9332
# MTW raw=28 aligned=16 size=1896/1896
```
