# Layout::Layout C1/C2 (`_ZN6LayoutC1Ev`) — wave 118 / worker w6602

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (identical bodies), original size
**7072 B**, source `src/game/ui/Layout.cpp`. Branch `wave118-w6602`, base
`3d4a98a3e` (wave-117 harvest).

## Result — one landable improvement

| metric | before (HEAD) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| unrounded `difflib` ratio | 48.554913295 % | **48.594777756 %** |
| `byte_exact` / `linked_exact` | 4501 / 4501 | **4501 / 4501** |
| `size_exact` / `data_exact` | 4524 / 843 | **4524 / 843** |
| imports / lint / drift | 207 / CLEAN / 0 | **207 / CLEAN / 0** |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
27 section headers, file size 2,187,328 B. `.text` 0x69440/1259464 and
`.ARM.exidx` 0x19cc08/28528 are unchanged. `.ARM.extab` is 0x2628C (156300 B)
vs the original 0x26298 (156312 B), i.e. 12 B short — this is **pre-existing at
HEAD~1** (verified by rebuilding HEAD~1: same 156300 B), not caused by this pass.

## Landed edits (two, both real source shape)

### 1. `rewardBoxHeight` if/else chain -> nested ternary

```cpp
// before (HEAD)
if (Globals::n9) {
    this->rewardBoxHeight = 352;
} else if (Globals::retinaDisplay) {
    this->rewardBoxHeight = 392;
} else {
    this->rewardBoxHeight = 196;
}

// after
this->rewardBoxHeight = Globals::n9 ? 352 : (Globals::retinaDisplay ? 392 : 196);
```

Original asm (rewardBoxHeight is offset `0x3ec`):

```
d24de:  f44f 71b0   mov.w  r1, #352      ; 0x160   (n9 branch)
d24e2:  f8c0 13ec   str.w  r1, [r0,#1004] ; 0x3ec
...
d2530:  f44f 71c4   mov.w  r1, #392      ; 0x188   (non-n9, retina)
d2534:  f1b8 0f00   cmp.w  r8, #0
d2538:  bf08        it     eq
d253a:  21c4        moveq  r1, #196      ; 0xc4
d253c:  f8c0 13ec   str.w  r1, [r0,#1004] ; 0x3ec
```

The original materialises the `n9` arm as a direct store and the remaining
`retinaDisplay ? 392 : 196` as one conditional move — the nested-ternary shape.
The if/else spelling made clang emit an extra 8 B of branch scaffolding.
Alone this variant is 7064 B / 48.574277 %; the 8 B is restored by edit 2.

### 2. `field_0x138` double round trip

```cpp
// before
this->field_0x138 = (int)(float)v138;
// after
this->field_0x138 = (int)(float)((int)(float)v138);
```

Original asm:

```
d1d22:  ed80 1a4e   vstr  s2, [r0,#312]   ; 0x138
```

The original stores `0x138` with a **float** store (`vstr`), i.e. the value is
rounded through float twice before being written. The extra `(int)(float)`
round trip reproduces the `vstr` on the non-HD paths. Alone on HEAD this edit
is score-neutral (7072 B / 48.554913 %); combined with edit 1 it recovers the
8 B and lifts the score.

## Best measured variants (all real `link_so` builds, unrounded ratio)

| variant | size | score % |
|---|---:|---:|
| HEAD (baseline) | 7072 | 48.554913 |
| **both landed edits** | **7072** | **48.594778** |
| `rewardBoxHeight` ternary only | 7064 | 48.574277 |
| `0x138` double round trip only | 7072 | 48.554913 |
| `rewardBoxHeight` + `field_0x184` ternary | 7072 | 48.116404 |
| `rewardBoxHeight` + `orbitInfoStationY` ternary | 7072 | 47.408293 |
| `rewardBoxHeight` + `field_0x2d4` ternary | 7052 | 48.284118 |
| `rewardBoxHeight` + `0x1cc`/`0x280` ternary | 7064 | 48.494516 |
| `float v138` variant | 7092 | 48.347272 |
| wave-89 6800-B all-ternary body | 6800 | 50.788516 (wrong size) |

Any ctor size other than 7072 B collapses `byte_exact` (7076 -> 2454,
7068 -> 4458, 7064 -> 3221), so the 8-B trade between the two edits is what
makes the win landable.

## Negative search (recorded, not landed)

- Single `(int)(float)(...)` wraps on the best body: no better hits.
- Single `(int)(float)(int)(...)` wraps on the best body: all neutral.
- `N * 1.40625f` -> `N.0f * 1.40625f` on every occurrence: neutral (clang folds).
- 400 random 1-3 edit move/wrap fuzz on the best body (seed 7): no improvement.
- Moving `field_0x140`/`0x144` next to `0x148` (the structural SLP fix) gives
  7000-7072 B but 44-46 % — the alignment gain destroys difflib alignment.

## Pool-word context

Original: 6106 B code + 241 literal-pool words. HEAD: 6290 B + 195 words.
The remaining pool deficit is SLP grouping, not wrong constants: all per-field
values agree with the original across all six flag configurations
(`hd/n9/ipad/ipadlarge/retina/ipadhd` store traces identical). The original HD
path aligns SLP groups at `0xb0, 0xc0, 0x140, 0x150, 0x160, 0x170`; HEAD groups
at `0xb4, 0xc4, 0x148, 0x158, 0x168` because `0x140`/`0x144` are assigned late.
That structural fix shrinks the ctor below 7072 B, so it cannot land alone.
