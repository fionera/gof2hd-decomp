# `Layout::Layout` C1/C2 — wave 171 / worker w11610

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Baseline (master `50716a763`): raw **5,640**, aligned **2,282**, 7,072 B.
Exit: **no code change landed** (nothing beat raw 5,640 at size 7,072).

## What the original tail actually is (objdump, `-M force-thumb`)

`objdump` of the stripped original needs `-M force-thumb` (no `$t` mapping
symbols survive `--strip-all`); without it the whole dump is garbage.

The tail `d21e2..d2548` is a genuine three-way device dispatch, and the
iPadHD branch really does cover **all** of `0x204..0x278` with seven
16-byte `vst1.32 {d16-d17}..{d28-d29}` stores (each is 4 words, not 2):
`0x204,0x214,0x224,0x23c,0x24c,0x25c,0x26c`.  The literal pool words at
`d2a90/d2aa0/d2ab0/d2ac0/d2ad0/d2ae0/d2af0` are exactly the iPadHD
values, e.g. `12,5,7,5` = `0x204,0x208,0x20c,0x210`.

Register map in the original: `sl=iPadHD`, `r8=retinaDisplay`,
`r9=!n9`, `fp=iPadLarge`, `lr`/`ip` are derived booleans
(`lr=!(ip||sl)` on the iPadHD path).  The reward-box tail is shared:
`n9` and the retina/else path both fall into `d2498` (0x280,
scrollBarHandle, scrollBarInset, rewardBoxWidth/Y/Y2) and `d2530`
(rewardBoxHeight/X); the iPadHD branch jumps into the same height/X
block at `d22b2`.

Reconstructed shape (this is what was compiled and measured):

```cpp
this->field_0x1e8 = ...; this->field_0x1e4 = ...;
if (Globals::iPadHD) {
    this->field_0x1fc = 2; this->field_0x1ec = 26;
    this->field_0x234 = 1 * 1.40625f; this->field_0x238_blk16.a = 50 * 1.40625f;
    this->field_0x27c = 20 * 1.40625f;
    this->field_0x280 = 0; this->scrollBarInset = 1 * 1.40625f;
    this->scrollBarHandle = 0.0f; this->rewardBoxWidth = 100 * 1.40625f;
    this->rewardBoxY = 55 * 1.40625f; this->rewardBoxY2 = 132;
    /* 0x204..0x278 ascending, iPadHD constants */
    this->rewardBoxHeight = Globals::n9 ? 352 : (Globals::retinaDisplay ? 392 : 196);
    this->rewardBoxX = Globals::n9 ? 75 : (Globals::retinaDisplay ? 100 : 50);
} else if (Globals::n9) {
    /* 0x1fc,0x1ec,0x234,0x238.a,0x27c constants + 0x204..0x278 constants */
    this->field_0x280 = 4; this->scrollBarHandle = 1; this->scrollBarInset = 2;
    this->rewardBoxWidth = Globals::iPadLarge ? 200 : 112;
    this->rewardBoxY = Globals::iPadLarge ? 110 : 57; this->rewardBoxY2 = 111;
    this->rewardBoxHeight = 352; this->rewardBoxX = 75;
} else {
    /* 0x1fc..0x278 retina?A:B selects */
    this->field_0x280 = Globals::retinaDisplay ? 5 : 0;
    this->scrollBarHandle = Globals::retinaDisplay;
    this->scrollBarInset = Globals::retinaDisplay ? 2 : 1;
    this->rewardBoxWidth = Globals::iPad ? 100 : (Globals::retinaDisplay ? 150 : 75);
    this->rewardBoxY = Globals::iPad ? 55 : (Globals::retinaDisplay ? 76 : 38);
    this->rewardBoxY2 = (this->rewardBoxY + 17) << Globals::retinaDisplay;
    this->rewardBoxHeight = Globals::retinaDisplay ? 392 : 196;
    this->rewardBoxX = Globals::retinaDisplay ? 100 : 50;
}
```

Everything after `rewardBoxX` (`0x2bc, footerButtonOffset, 0x200, 0x1f0,
0x1f4, 0x1f8, 0x284, 0x285, 0x286, 0x288, 0x28c, 0x290, 0x2f4..`) is a
per-field ternary cascade in the original too (`cmp sl` / `cmp r8` /
`cmp r9` per field), not part of the dispatch.

## Measured shapes (real `link_so` + `fn_diff`, size must stay 7,072)

| shape | raw | aligned | size |
|---|---:|---:|---:|
| baseline (HEAD `50716a763`) | **5,640** | **2,282** | **7,072** |
| full 3-way dispatch, ascending fields, shared reward box | 5,953 | 2,968 | 6,896 |
| per-field tail reordered ascending (no dispatch) | 5,897 | 2,719 | 6,960 |
| rewardBoxWidth/Y converted to in-place if/else | 5,951 | 2,458 | 7,088 |
| reward box block moved after `field_0x274` (width,Y,Y2,height,X) | 5,712 | 2,457 | 7,060 |
| reward box block moved together after scrollBarHandle | 5,806 | 2,474 | 7,144 |
| reward box as if/else + plain ints 140/77 | 5,748 | 2,462 | 7,104 |
| whole first block sorted ascending by offset | 6,123 | 2,927 | 6,800 |
| `field_0x1e8` before `field_0x1e4` | 5,945 | 2,372 | 6,984 |
| `field_0x1e8 = (retina||iPadHD||n9) ? 1 : 0` | 5,640 | 2,282 | 7,072 |
| `field_0x10c_rowBaseY` without `(int)(float)` | 5,744 | 2,328 | 7,064 |

Local search: all 51 adjacent swaps of the 52 tail statements, and ~250
random moves over the tail plus ~130 random moves over the 134 first-block
units were evaluated; **none** kept size 7,072 and lowered raw.  The
dispatch is consistently 100-250 B *smaller* than the per-field source
(clang tail-merges the shared blocks and drops literal-pool words), so
the faithful dispatch cannot keep the 7,072 B size without an unrelated
compensating change.

Conclusion: the remaining raw residue is global register allocation
(original `sl=iPadHD`, ours `lr=iPadHD`) plus literal-pool layout, not
missing tail stores.  A win needs the whole function's source order
reconstructed, not just the tail.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11610
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
```

Scratch `/dev/shm/w11610/`: `Layout.base.cpp` (baseline source),
`search.py` / `scan.py` (tail adjacent-swap search), `headrand.py`
(first-block random-move search), `aldiff.py` (per-region aligned diff),
`ctor.asm` (original `-M force-thumb` disassembly).
