# `Layout::Layout` C1/C2 — wave 191 / worker w13513

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, linked symbol size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave191b-w13513`, base master `566374cfd`.

**Result: no size-exact raw win landed.** Baseline unchanged at raw **5,243**,
aligned **2,728**, size **7,072/7,072**, insn 2,521/2,552. Tree left clean.

This pass adds three new pieces of evidence over `Layout-LayoutC1-wave191-w13492.md`
and `...-w13504.md`: a value-equivalence oracle for the whole ctor, a
per-field flag-test order audit against the recovered original chain order, and
a region-level byte census. It also records the full variant matrix from this
session.

## 1. Value-equivalence harness (new, reusable)

`scratch/w13513/cmp.cpp` (built as `cmp`) contains the current ctor body and
the `gen.py`-emulated body, both rewritten to `*(int*)(s->data+off)` over a flat
0x420-byte buffer, and runs **all 32 flag combinations** with a memcmp. Result:
**`bad 0`**. So

```
_work/reconstructions/layout_ctor/gen.py lay_orig.json <names> w4e/b3.ord chain
```

reproduces the current source's stored values exactly. `lay_orig.json` +
`w4e/b3.cho`/`b3.ord` are therefore a trustworthy oracle for further
order/chain experiments (not just a single flag path).

Names file built with:

```
python3 tools/layout.py Layout | awk '/^  0x/{print $1, "x", $2}' > layout_dump3.txt
```

(gen.py needs 3 columns). The emulator body in `b3.ord` order compiles to raw
6,465 / size 6,352 — that order is **not** the current source order.

## 2. Per-field flag-test order audit

`w4e/b3.cho` vs the current source: only **3 mismatches** with the original's
recovered per-field chain order:

| field | source order | original |
|---|---|---|
| `field_0xac` | HLNPR | HLPNR |
| `helpWindowX` | HLRNP | HLPNR |
| `field_0x314` | LPNR | LPRNH |

Fixing `0xac` and `helpWindowX` is **codegen-identical** (5,243). The `0x314`
reorder is size-fatal (7,080).

## 3. `__builtin_expect` sweep

Wrapped every one of the 178 `Globals::retinaDisplay ?` occurrences with
`__builtin_expect(...,0)` and `(...,1)`: all identical (5,243 / 7,072) except
site 168 with expect 1 → 5,400 / 7,048. No win.

## 4. Variant matrix (real `link_so` + `fn_diff`, all size-exact unless noted)

Harness `scratch/w13513/variants.py` (keys A–T, A2, AC, HX, F314, U,V,W,X,Y,
I,J,G,H,K,L,M,N,O,P,Q,R,S,T). Baseline `[]` = (5243, 2728, 7072).

| key | change | raw / size |
|---|---|---|
| A | `0x130` block → 3 flat ternaries | 5401 / 7080 |
| A2 | `0x130` split `else if (iPad)` | 5397 / 7064 |
| B | `0x2cc` → if/else chain | 5335 / 7092 |
| C | `0x2c8` store moved before `0x2cc` | 6248 / 6968 |
| D | `0x2d8` moved after `0x2d4` block | 6266 / 6888 |
| E | `helpWindowY` uses `*rd` | 5276 / 7076 |
| G/H | `0x8c` via `(int)(float)` / float local | neutral |
| I | `0x8c` moved before `0x9c` | 5982 / 7000 |
| J | `0x8c` after `eventBannerScale` | neutral |
| K | `field_0xa8` `int v` HD arm → `85` | 5496 / 7032 |
| M | inline the `v` expression | 5378 / 7064 |
| U/V/W | move `0x1d0..0x1dc` block to `0x27c` / `scrollBarInset` / `0x234` | 6012/6920, 5983/7016, 6061/6864 |
| X/Y | move `0x2c4` after `0x2c0` / `0x2c0` after `0x2c4` | 6060/6936, 5985/6872 |
| S/T | `__builtin_expect(iPad,1)` / `expect(retina,1)` on `0x18c` | 5827/7040, neutral |

## 5. Region byte-diff census (orig vs ours, same addresses)

| range | bytes differing |
|---|---|
| `d1042–d126c` (HD arm) | 329 |
| `d126c–d1570` (n9 arm) | 346 |
| `d1570–d21e2` (retina/default arm) | **2,669** |
| `d21e2–d2854` (late block) | 1,508 |

## 6. Structural facts (re-confirmed this session)

- Original HD arm: **27** `vld1/vst1` groups at offsets
  `0x14,4,0x24,0x34,0x44,0x54,0x64,0x2c8,0x7c`, then
  `0xc0,0xb0,0xec,0x294,0x2a4,0x108,0x118`; ours groups at
  `0x14,4,0x38,0x24,0x58,0x48,0x68,0x2c8,0x7c`, then `0xb0,0xc0,0x294,0x2a4,0xdc`.
- **Non-HD arm (d1570–d21e2) is the dominant gap**: orig **34 vld1 / 27 vst1 /
  33 strd / 97 it**; ours **21 / 15 / 15 / 106**. The original SLP-vectorises
  and `strd`-fuses far more of the default path.
- Original `0x2d4`+`0x2d8` are one `strd r4,r6,[r0,#724]` at `d18b6` (both
  computed first); ours stores `0x2d8` early (`d1134`) and `0x2d4` separately
  (`d18d2`).
- Original `0x138` is `strd r1,r2,[r0,#312]` in the HD arm (`d1244`) and
  default arm (`d1abe`).
- Cold blocks: orig 4 (`d2802` 0x130-default 38 B, `d2828` 0x2c8, `d2836`
  0x2d0, `d2844` 0x60); ours 2 (`0x18c`, `0x60`).
- Pool: orig starts `d2854` (221 words), ours `d28d8` (188 words); code
  6,188 vs 6,320 B.
- Callee-saved: orig HD=`sl`, n9=`fp`, retina ptr=`r8`, `strd fp,sl,[sp]`;
  ours HD=`lr`, retina=`ip`, `str.w lr,[sp]`.

## 7. Conclusion

The remaining delta is dominated by the retina/default arm. A win needs the
original's default-arm SLP/`strd` grouping (≈ −132 B code, +33 pool words) plus
the four cold tails and the `sl`/`fp`/`r8` callee-saved rotation. The
`0x2d4/0x2d8` `strd` (variant D) is −184 B by itself and needs an exactly
compensating +184 B elsewhere; measured combinations (D+A+B 6,274/6,896,
D+A2 6,246/6,880) are still far short.

## 8. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13513
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5243 aligned=2728 size=7072/7072 insn=2521/2552
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13513/`: `Layout.base.cpp`
(pristine source), `orig.dis`/`ours.dis`, `align.py`/`align.txt`,
`variants.py`, `expect_sweep.py`/`.log`, `layout_dump3.txt`,
`gen_b3.cpp`, `cmp.cpp`/`cmp`.
