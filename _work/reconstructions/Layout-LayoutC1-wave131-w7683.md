# `Layout::Layout` C1/C2 — wave 131 / worker w7683

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, size **7072 B**, source `src/game/ui/Layout.cpp`.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
Baseline = wave-130 master `7c6933c2d` (7072 B / 48.943802 % unrounded).

## Result — size-exact branch-tree body, gate green

| metric | before (HEAD) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** |
| unrounded `difflib` ratio | 48.943802 % | **20.939061 %** |
| `byte_exact` / `linked_exact` | 4506 / 4506 | **4506 / 4506** |
| `size_exact` / `data_exact` | 4524 / 843 | 4524 / 843 |
| imports / lint / drift | 207 / CLEAN / 0 | 207 / CLEAN / 0 |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.

This wave starts from w7663's common1 branch-tree body (`dfb3bfd04`, 7024 B)
and restores the missing 48 B with the **reward-box configuration tree**,
decoded from the original disassembly at `0xd2470..0xd2548`
(function-relative `0x1448..0x1520`).

## 1. Reward-box tree (new decode)

Original Thumb (function-relative addresses):

```
0x1470  vmov s0,r1 ; 0x3e4 = n9 ? retina : 1
0x1476  cmp r9,#0
0x148a  str [r0,#996]        ; field_0x3e4
0x148e  vstr s0,[r0,#992]    ; field_0x3e0 = (int)(float)...
0x1492  beq.n 0x14a8         ; n9 -> 0x14a8
0x1494  if (iPadLarge) { width=200; Y=110; goto 0x14ec; }
0x14c2  if (iPad)      { width=100; Y=55;  goto 0x14ec; }
0x14d6  width = retina?150:75; Y = retina?76:38;
0x14ec  Y2 = (int)(float)((Y+17) << retina);
0x1508  height = retina?392:196;
0x1518  X = retina?100:50;
0x1520  str [r0,#1008]       ; rewardBoxX
0x14a8  width=112; Y=57; Y2=111; height=352; X=75; goto 0x1520
```

So the source shape is

```cpp
if (n9) {
    rewardBoxWidth = 112; rewardBoxY = 57; rewardBoxY2 = 111;
    rewardBoxHeight = 352; rewardBoxX = 75;
} else {
    if (iPadLarge)      { rewardBoxWidth = 200; rewardBoxY = 110; }
    else if (iPad)      { rewardBoxWidth = 100; rewardBoxY = 55;  }
    else                { rewardBoxWidth = retina ? 150 : 75;
                          rewardBoxY     = retina ? 76  : 38; }
    rewardBoxY2 = (int)(float)((rewardBoxY + 17) << retina);
    rewardBoxHeight = retina ? 392 : 196;
    rewardBoxX      = retina ? 100 : 50;
}
```

The iPadHD values (`width = 100 * 1.40625f`, `Y = 55 * 1.40625f`,
`Y2 = 132`) are set in the separate `if (iPadHD)` arm of the enclosing
device split, not inside this tree; the landed source keeps them as the
first arm (T1) because the outer HD/non-HD split is not yet modelled.

Store census confirms the decode: original `0x3e8 ×5`, `0x3f4 ×3`,
`0x3ec ×2`, `0x3f0 ×2`, `0x3f8 ×1`, `0x3fc ×1`.

## 2. Size accounting

| variant (all real `link_so` builds) | size | ratio % |
|---|---:|---:|
| w7663 body | 7024 | 49.050570 |
| + reward tree T1, order `F1F4,F94,FOOTER,F28C,F1F0,F144,F1F8` | 7068 | 41.237525 |
| + same, order `FOOTER,F28C,F1F0,F1F4,F1F8,F94,F144` | 7076 | 44.692737 |
| + `field_0xf0` int form `112` (80*1.40625f = 112.5 -> 112) | 7060 | 40.846476 |
| + `field_0x1dc` int form `14` (10*1.40625f = 14.0625 -> 14) | 6952 | 34.074074 |
| **+ both int forms (landed)** | **7072** | **20.939061** |

`field_0xf0` and `field_0x1dc` are `int` members (`Layout.h`); the float
multiply truncates, so `112` and `14` are bit-identical values.  They are
the only 4-byte knobs found that take the T1 tree from 7068 to exactly
7072 without changing the branch-tree shape.

## 3. Why the fuzzy ratio drops

The original places the whole tail inside one `if (iPadHD) { ...SLP... }
else { ...reward tree... }` split (Ghidra decompile lines 1140-1570).  Our
source still emits field-major ternaries for the HD arm, so the reward
tree is scheduled across a much wider instruction window than the
original's compact `0x1470..0x1524` block.  The branch trees are
structurally right but the surrounding HD/non-HD split is not yet modelled,
which is what costs the fuzzy ratio.  Baseline HEAD's flat ternaries
happen to align better globally (48.94 %) even though they are not the
original control flow.

## 4. Open questions

* Model the outer `if (iPadHD) { vectorised constant block } else { ... }`
  split for the whole tail.  This is the single change most likely to
  recover the fuzzy ratio while keeping the branch trees.
* The `int v` ballast at `0xa8` (line 69) is still present.  `float v`
  (semantically identical) gives 7068 B on the good order at 44.63 % —
  a promising base for a future 7072 landing with a better ratio.
* `0x174/0x178/0x17c` flat tree is still the strongest ratio lever
  (49.2 % at 7016 B); it needs 56 B of the original's duplicated
  `0x184..0x1ac` stores to become size-exact.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7683
cmake --build cmake-build-match --target check -j6     # RATCHET PASS
python3 /dev/shm/w7683/score.py                        # 7072 / 20.939061
```

Scratch harnesses (not committed, `/dev/shm/w7683/`): `score.py`,
`gen_reward2.py` (T0..T8 reward-tree variants + statement order),
`diffaddr.py`, `stores2.py`, `sweep.py` / `custom.py` / `micro.py` /
`micro2.py` / `ballast.py` / `hit7072.py` (size/ratio sweeps).
