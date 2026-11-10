# `Layout::Layout` C1/C2 — wave 165 / worker w11109

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Baseline (wave-164 HEAD `3633505f8`): raw **5,688**, aligned **2,108**,
7,072 B, 2516/2552 instructions.

Scope: the tail `d21e2..d2800` (fields `0x1e4` onward), per the wave-164
w11006 hand-off (`Layout-LayoutC1-wave164-w11006.md`).

## Result

**No measured win landed.**  Baseline unchanged: raw 5,688 / aligned 2,108 /
7,072 B.  Tree left clean.

## Original tail structure (confirmed from objdump)

Registers: `sl = iPadHD`, `ip = n9`, `r8 = retinaDisplay`, `r4 = iPad`,
`r9 = !n9` (a local bool computed at `d20d8`/`d21b2`; it overwrites the
`iPadLarge` value that `r9` held earlier in the function).

```
d21e2  field_0x1e4 = (int)(float)s0 ; field_0x1e8 = (lr ? r8 : 1)
d2202  if (sl==0) goto d22b8                     ; !iPadHD
       BLOCK A (iPadHD) d2204..d22b2:
         0x1fc=2, 0x1ec=26, 0x234=1, 0x238.a=70, 0x27c=28, 0x280=0,
         0x3e0=1, 0x3e4=0, 0x3e8=140, 0x3f4=77, 0x3f8=132,
         vst1 groups 0x204,0x214,0x224,0x23c,0x24c,0x25c,0x26c (4 words each)
         if (r9==0 /*n9*/) goto d24de else goto d2530
d22b8  if (r9==0 /*n9*/) goto d240e
       BLOCK B (!n9, retina/else) d22c0..d240c:
         r8-based selects for 0x1fc,0x1ec,0x224,0x228,0x22c,0x230,0x234,
         0x238,0x24c,0x250,0x20c,0x218,0x220,0x204,0x208,0x210,0x214,
         0x21c,0x23c,0x278,0x264,0x25c,0x260,0x274,0x27c
         (vectorised with vdup.16/vmovl.u16/vshl/vbsl and vst1 lane stores)
d240e  BLOCK C (n9) d240e..d2494:
         0x1fc=4, 0x1ec=34, 0x234=2, 0x238.a=90, 0x27c=30,
         vst1 groups 0x204,0x214,0x224,0x23c,0x24c,0x25c,0x26c
d2498  shared (non-iPadHD): 0x280, 0x3e4, 0x3e0, rewardBoxWidth(0x3e8),
       rewardBoxY(0x3f4), rewardBoxY2(0x3f8, from NEW Y)
       n9 -> 112/57/111; iPadLarge -> 200/110/(Y+17)<<retina;
       iPad -> 100/55/(Y+17)<<retina; else retina?150:75 / retina?76:38
d24de  n9 tail: rewardBoxHeight(0x3ec)=352, rewardBoxX(0x3f0)=75
d2530  else tail: 0x3ec = retina?392:196, 0x3f0 = retina?100:50
d2548  shared: 0x2bc, 0x3fc(footerButtonOffset), 0x200, 0x1f0, 0x1f4,
       0x1f8, 0x284, 0x285, 0x286, 0x288, 0x28c, 0x290
d2654  shared: 0x2f4, 0x2f8, 0x2fc, 0x300, 0x304, 0x308
d2704  shared: 0x30c, 0x310, 0x314, 0x318
d2774  shared: 0x31c, 0x320
```

Literal pools (original): block A constants at `d2a90..d2af0`, block C at
`d2b00..d2b60`; block B's retina selects are assembled from `d2b70..d2ba0`
plus vector ops.

## Shapes measured (all real `link_so`, `python3 tools/fn_diff.py`)

| shape | raw | aligned | size |
|---|---:|---:|---:|
| baseline HEAD | **5,688** | **2,108** | **7,072** |
| explicit if/else blocks for whole tail (iPadHD/n9/else, incl. 0x204, 0x2bc, shared) | 6,059 | 2,699 | 6,880 |
| explicit blocks, 0x204 left in first half | 5,903 | 2,551 | 7,016 |
| reward-box width/Y/Y2 as explicit if/else chain (iPadHD outer) | 6,040 | 2,300 | 7,168 |
| reward-box Y/Y2 as explicit if/else chain (width ternary) | 6,049 | 2,338 | 7,176 |
| width,Y,Y2,height,X reordered to original order | 6,052 | 2,317 | 7,168 |
| move `field_0x204` next to `0x208` | 5,966 | 2,359 | 7,024 |
| move `0x204` next to `0x208` and `0x27c` out | 5,966 | 2,363 | 7,024 |
| move `field_0x200` to after `rewardBoxX` | 5,755 | 2,205 | 7,040 |
| move `field_0x234` before `0x238.a` | 5,689 | 2,118 | 7,072 |
| move `field_0x27c` before `0x280` | 5,744 | 2,120 | 7,080 |
| move `field_0x1fc` before `0x1ec` | 5,727 | 2,112 | 7,080 |
| scrollBarHandle before scrollBarInset | 5,719 | 2,249 | 7,072 |
| move `field_0x285` after scrollBarHandle | 5,716 | 2,114 | 7,080 |
| move rewardBoxWidth before scrollBarInset | 5,728 | 2,253 | 7,088 |
| `(int)(16 * 1.40625f)` for `field_0x1f4` | 5,753 | 2,106 | 7,064 |
| `(int)(100 * 1.40625f)` for `rewardBoxWidth` | 6,011 | 2,489 | 7,120 |
| all tail `N * 1.40625f` -> `(int)(N * 1.40625f)` | 5,827 | 2,371 | 7,048 |
| scrollBarHandle `0.0f` -> `0` | 5,688 | 2,108 | 7,072 (neutral) |
| `(int)(1 * 1.40625f)` for `field_0x234` | 5,688 | 2,108 | 7,072 (neutral) |

The current per-field-ternary source is a strong local optimum for the tail:
every explicit-dispatch or reorder shape tried is raw-worse, and most are not
size-exact.  The residual is dominated by the fact that the original's
iPadHD block owns `0x234, 0x27c, 0x3e8, 0x3f4, 0x3f8` and the `0x204..0x278`
vector groups, while ours owns `0x200, 0x285` and the `0x208..0x27c` groups
(shifted +4).  Moving `0x204` into the tail is the natural fix but it changes
the first-half dispatch and loses 48 B of size (7,024).

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11109
cmake --build cmake-build-match --target link_so -j8
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
```

Scratch `/dev/shm/w11109/`: `Layout.cpp.orig` (baseline source),
`newtail.txt`/`tail2.txt` (explicit-block rewrites), `measure.sh`
(build + fn_diff), `regions.py`, `adiff2.py` (raw/aligned diff dumps),
`orig_full.txt`/`ours_full.txt` (objdump).
