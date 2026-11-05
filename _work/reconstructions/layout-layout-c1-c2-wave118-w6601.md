# Layout::Layout C1/C2 (`_ZN6LayoutC1Ev`) — wave 118 / worker w6601

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (identical bodies), original size
**7072 B**, source `src/game/ui/Layout.cpp`. Branch `wave118-w6601`, base
`3d4a98a3e` (wave-117 harvest).

## Result — no landable source win; note-only commit

| metric | before (HEAD) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| unrounded `difflib` ratio | 48.594778 % | **48.594778 %** |
| `byte_exact` / `linked_exact` | 4501 / 4501 | **4501 / 4501** |
| `size_exact` / `data_exact` | 4524 / 843 | **4524 / 843** |
| imports / lint / drift | 207 / CLEAN / 0 | **207 / CLEAN / 0** |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
No section changed: 27 section headers, file size 2,187,328 B,
`.ARM.extab 0x26298` all unchanged.  No source change landed.

## Task hypothesis checked: "the 46 extra pool words are GOT-slot addresses"

**Disproved.**  The pool was decoded word-by-word from the original
(`objdump -d 0xd1028..0xd2bc8`, literal targets resolved through
`ldr rX,[pc,#imm]` + `add rX,pc` and `addw rX,pc,#imm`):

| pool region (original) | words | kind |
|---|---:|---|
| `0xd1300`..`0xd1570` (HD block) | 156 | SLP int/float constant groups + 2 GOT offsets |
| `0xd17d8` | 1 | literal |
| `0xd18d4`..`0xd1900` (n9 block) | 11 | 10 GOT offsets + `57.6f` |
| `0xd1c20` | 1 | literal |
| `0xd1d7c`..`0xd1d94` | 6 | ints + `5.625f` |
| `0xd2854`..`0xd2bc8` (end pool) | 221 | SLP int/float constant groups + 8 GOT offsets |

GOT-offset words (the `ldr rX,[pc]; add rX,pc; ldr rX,[rX]` idiom):
**original 8, ours 9** — we have *more*, not fewer.  The extra words are
**SLP int-constant groups**: the original has **93** `add/addw rX,pc,#imm`
literal-pool address instructions, ours has **85**, and its end pool is
**221 words vs our 188** (32 words = 128 B).  Each extra group is four int
constants consumed by `vld1.64 {dX-dY}` + `vst1.32 {dX-dY}` (four adjacent
`int` field stores), not a global access.

## Real body split (re-derived)

| body | code | cold blocks after `pop` | end pool |
|---|---:|---:|---:|
| original | 6106 B (`0xd1028..0xd2802`) | 80 B (`0xd2802..0xd2852`, 4 blocks) | 884 B / 221 w (`0xd2854..`) |
| HEAD | 6288 B (`0xd1028..0xd28b8`) | 28 B (`0xd28ba..0xd28d6`, 1 block) | 752 B / 188 w (`0xd28d8..`) |

The original's HD block stores (in order) the SLP groups
`0x14, 0x4, 0x24, 0x34, 0x44, 0x54, 0x64, 0x2c8, 0x7c`, then
`0xc0, 0xb0, 0xec, 0x294, 0x2a4, 0x108, 0x118, 0x128, 0x140, 0x150, 0x160,
0x170, 0x180, 0x190, 0x1a0, 0x1b0, 0x1c0, 0x1d0`.
HEAD reproduces the **first nine groups exactly** and then emits
`0xc4, 0x294, 0x2a4, 0xdc, 0x108, 0x128, 0x148, 0x158, 0x168, 0x180, ...`
— i.e. from 0x140 on our groups are offset by 8 because `field_0x140` /
`field_0x144` are assigned near the end of the ctor (wave-117 finding,
re-confirmed here).  The original's `0x140..0x1dc` groups are strict
address-ordered four-field groups; ours are not.

The original's 4 cold blocks are the outlined `else` tails of the
`field_0x130/0x134/v138`, `field_0x2c4`, `field_0x180` and `field_0x174`
chains (`ldrb rX,[rd]; cmp; it eq; moveq; b.w ...`).  HEAD inlines them.

## Every 7072-B / near-miss variant measured this wave (real `link_so` builds)

| variant | size | unrounded % |
|---|---:|---:|
| HEAD (baseline) | **7072** | **48.594778** |
| `u9c2` = `field_0x9c` float ternary + `t_field_0x2c8` HD arm `(int)(45*1.40625f)` | 7076 | **48.725100** |
| `u9c2` minus the `field_0x150` `(int)(float)` round trip | 7060 | **48.733799** |
| `u9c2` minus the `field_0x138` cast | 7060 | 48.724083 |
| `u9c2` minus the `field_0x160` `(int)(float)(int)` | 7068 | 48.704663 |
| `u9c2` minus the `field_0xa8` cast | 7068 | 48.664807 |
| `u9c2` + `field_0x2d8` moved before `rd` | 7076 | 48.725100 |
| `u9c2` + `rd` moved before `field_0x2d8` | 7068 | 48.684211 |
| `field_0x184` if/else → ternary, order iPadHD/iPadLarge/iPad/n9/retina | 7072 | 48.554913 |
| `field_0x184` if/else → ternary, order iPadHD/iPadLarge/retina/iPad/n9 | 7080 | 47.937836 |
| sort groups 95..135 (0x140..0x1dc) by address | 6896 | 42.557483 |
| sort groups 47..70 (0xb0..0xf8) by address | 7064 | 41.140124 |
| move the `0xc0` group (0xc0/0xc4/0xc8/0xcc) to the ctor front | 6944 | 41.795853 |
| 120 random single-group moves of the 7060 body | — | one 7072 hit (46.027944) |

Searches run this wave (all real builds, unrounded `verify.py` ratio):

* all 8 `(int)` / `(int)(float)` **removals** on the `u9c2` (7076) body —
  deltas −16, −8; **no 7072 hit**;
* all 182 single-line int-ternary assignments wrapped `(int)(float)(...)`
  on the 7060 body — size distribution
  {6992×1, 6996×1, 7008×2, 7020×2, 7028×2, 7040×1, 7044×1, 7048×4,
  7052×5, **7060×154**, 7068×2, 7076×5, 7100×1, 7116×1}; **no 7072**;
* the same 182 wrapped `(float)(...)` — same distribution, **no 7072**;
* the same 182 wrapped `(int)(float)(int)(...)` on the 7060 body — **no 7072**;
* the same 182 wrapped `(int)(float)(...)` on the 7068 body — **no 7072**.

So the +/−4-byte gap that separates the best-scoring bodies from the 7072-B
wall could not be closed: every cast/fold edit lands on 0, ±8, ±16, never ±4,
and the only 7072 hits found score below the baseline.

## Why the remaining gap cannot be closed from the source

The original's extra 184 B relative to HEAD is **128 B of end-pool SLP
constants + 52 B of outlined cold blocks**, matched by 184 B less hot code.
Reproducing it needs the original's `0x140..0x1dc` statement order (strictly
address-ordered four-field runs) *and* the compiler choosing to outline the
four `else` tails.  Sorting those statements by address reproduces the right
SLP groups but shrinks the body to 6896 B (−176) and drops the ratio to
42.6 %; every compensation tried costs more score than it recovers.
The 7072-B wall is mandatory: a 7076-B ctor still collapses `byte_exact`
4501 → 2454.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6601
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6     # RATCHET PASS
```

Scratch harnesses (not committed): `/dev/shm/w6601/{try.sh,score.py,
poolclass3.py,blocks.py,showidx.py,emit.py,mv2.py,sortall2.py,variant.py,
edits.py,edits2.py,edits3.py,edits4.py,scan12.py,scan12b.py,scan68.py,
randsearch.py}`, `Layout.base.cpp`, `Layout.u9c2.cpp`, `Layout.v7060.cpp`,
`Layout.v7068c.cpp`, `orig.dis`, `ours.dis`.
