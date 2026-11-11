# `Layout::Layout` C1/C2 — wave 172 / worker w11710

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Baseline (master `41ef4c8b6`): raw **5,640**, aligned **2,282**, 7,072 B.
Exit: **no code change landed** (best faithful-dispatch shape: 7,016 B,
raw 6,009, aligned 2,610).

Scope: the tail `0x1e4..0x280` + shared reward-box block
(`d21e2..d2548`).  The first half (`d1042..d21e2`) and the T block
(`d2548..d2800`) were left as-is.

## Exact shared-block source shape (reconstructed from objdump)

The original tail is **not** an explicit 3-way `if/else if/else` over the
whole field set.  It is:

```cpp
field_0x1e4 = iPadHD ? 5 : (n9 ? 6 : (retina ? 8 : 4));
field_0x1e8 = retina ? 1 : (iPadHD ? 1 : (n9 ? 1 : 0));
if (iPadHD) {
    field_0x1fc = 2;  field_0x1ec = 26;
    field_0x234 = 1 * 1.40625f;  field_0x238_blk16.a = 50 * 1.40625f;
    field_0x27c = 20 * 1.40625f; field_0x280 = 0;
    scrollBarInset = 1 * 1.40625f; scrollBarHandle = 0.0f;
    rewardBoxWidth = 100 * 1.40625f; rewardBoxY = 55 * 1.40625f;
    rewardBoxY2 = 132;
    /* 0x204..0x278 ascending, 7 groups of 4 words */
    rewardBoxHeight = n9 ? 352 : (retina ? 392 : 196);
    rewardBoxX      = n9 ? 75  : (retina ? 100 : 50);
} else {
    if (n9) {
        field_0x1fc = 4; field_0x1ec = 34;
        field_0x234 = 2; field_0x238_blk16.a = 90; field_0x27c = 30;
        /* 0x204..0x278 ascending constants */
    } else {
        field_0x1fc = retina ? 4 : 2;  /* .. same 0x1ec..0x278, retina selects */
    }
    field_0x280      = n9 ? 4 : (retina ? 5 : 0);
    scrollBarHandle  = n9 ? 1 : retina;
    scrollBarInset   = n9 ? 2.0f : (retina ? 2.0f : 1.0f);   /* float expr! */
    if (n9)            { rewardBoxWidth = 112; rewardBoxY = 57;  rewardBoxY2 = 111; }
    else if (iPadLarge){ rewardBoxWidth = 200; rewardBoxY = 110; rewardBoxY2 = (Y+17)<<retina; }
    else if (iPad)     { rewardBoxWidth = 100; rewardBoxY = 55;  rewardBoxY2 = (Y+17)<<retina; }
    else               { rewardBoxWidth = retina ? 150 : 75;
                         rewardBoxY     = retina ? 76  : 38;
                         rewardBoxY2    = (Y+17)<<retina; }
    rewardBoxHeight = retina ? 392 : 196;
    rewardBoxX      = retina ? 100 : 50;
}
```

The A branch's store order is a clean linear witness: `0x1fc, 0x1ec,
0x234, 0x238, 0x27c, 0x280, 0x3e0, 0x3e4, 0x3e8, 0x3f4, 0x3f8`, then the
seven `vst1.32` groups `0x204, 0x214, 0x224, 0x23c, 0x24c, 0x25c, 0x26c`
(each 4 words), then `0x3ec` (height) and `0x3f0` (x).

The S block is shared by the `n9` and `!n9` inner paths; `clang` sinks
the `0x280`/`scrollBarHandle`/`scrollBarInset` stores to the merge point.
`scrollBarInset` must be a **float** expression (the original does
`vmov s0,r1; vcvt.f32.s32; vcvt.s32.f32; vstr`); an int expression
produces a plain `str`.

## Measured shapes (real `link_so` + `fn_diff`; size must stay 7,072)

| shape | raw | aligned | size |
|---|---:|---:|---:|
| baseline HEAD | **5,640** | **2,282** | **7,072** |
| exact dispatch above (0x1e4..0x140 replaced) | 6,009 | 2,610 | 7,016 |
| + `field_0x214`/`field_0x24c` written as `N * 1.40625f` | 6,009 | 2,610 | 7,016 |
| + move `0x140,0x144` into the first-half `0x140..0x14c` group | 5,982 | 2,592 | 6,944 |
| + re-apply w11609 ascending `0x34,0x38,0x3c,0x44` | 6,079 | 2,582 | 7,016 |

So the faithful dispatch is only **56 B smaller** than the baseline
per-field cascade (not the 176 B of the earlier w11610 shape).  It is a
size regression, and the raw metric worsens because the whole tail shifts
by ~96 B.

## Where the 56 B go

The original's first half (`d1042..0x1e4`) is **4,536 B / 1,586 insns**;
ours is **4,628 B / 1,591 insns** — 92 B wider for only 5 extra insns
(wider Thumb encodings, i.e. different register allocation / instruction
selection).  The tail (`0x1e4..end`) is correspondingly 152 B smaller in
ours.  The S block alone is ~126 B in ours vs 176 B in the original:
`clang` folds `Y+17` into `orr r1,r1,#17` (valid for 110/55/76/38) and
uses a more compact 3-arm chain, saving ~50 B.

The A-branch SLP grouping also differs.  Original: 7 `vst1` groups
`0x204,0x214,0x224,0x23c,0x24c,0x25c,0x26c`.  Ours: 5 groups
`0x204,0x218,0x23c,0x24c,0x260` plus scalar stores for
`0x214,0x228..0x238,0x25c,0x270..0x278`.  The `0x214` store refuses to
join the `0x214..0x220` group even when written as `1 * 1.40625f`
(the value `1` is rematerialised and shared with `0x230/0x234/0x250/0x260`).

## Conclusion

The exact shared-block shape is now pinned down (above), but landing it
requires reconstructing the **first half's** source order (92 B too wide)
and the **T block** order at the same time: the tail alone cannot keep
7,072 B.  The remaining raw residue is global register allocation plus
literal-pool layout, not missing tail stores.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11710
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
cmake --build cmake-build-match --target check -j2
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w11710/`: `Layout.base.cpp`
(baseline source), `newtail.cpp` (the exact dispatch block above),
`side.py` (per-region aligned diff), `ctor_orig.txt` and `tail_orig.txt`
(original `-M force-thumb` disassembly).
