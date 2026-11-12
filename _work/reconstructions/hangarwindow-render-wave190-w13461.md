# HangarWindow::render — wave 190 (w13461): free-credits `baseY` addition order

Worktree `wt/w13461`, branch `wave190-w13461`, base master `83d52fe52`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B, 3,734 insns.

## Result (landed)

Reorder the free-credits `baseY` addition so the `rowHeight * 2` term is
evaluated first:

```cpp
int baseY = Globals::layout->field_0x2c_rowHeight * 2 + this->dialog->y +
            Globals::layout->windowTopInset;
```

| metric | base | landed |
|---|---:|---:|
| raw same-address diff | 1,077 | **1,073** |
| instruction-index alignment | 604 | 613 |
| function size | 9,700 / 9,700 | 9,700 / 9,700 |
| `.text` section diff | 72,257 | **72,253** |
| `.ARM.extab` section diff | 584 | **584** |
| build-id | 20 | 20 |
| total (text+extab+build-id) | 72,861 | **72,857** |

Gate: `check` RATCHET PASS — byte_exact 4507/4524, linked_exact 4507/4524,
size-exact 4524/4524, stub_zero_size 0, missing 0, wrong_type 0, extra 0,
data_exact 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
`.rel.plt` byte-identical.

The win is a pure scheduling change in the free-credits preheader: the
`rowHeight * 2` load/add is hoisted ahead of the `dialog->y`/`windowTopInset`
adds, matching the original's `add.w r0,r0,r1,lsl #1` placement. No
`.ARM.extab` growth.

## Negative results measured this wave (all size 9,700 B unless stated)

Entry state was the wave-190 base (raw 1,077, `.text` 72,257, extab 584).
The worktree base was re-measured first; all numbers below are on it.

### Free-credits declaration/expression forms

| variant | raw | `.text` | extab |
|---|---:|---:|---:|
| **landed `rowHeight*2` first** | **1,073** | **72,253** | **584** |
| base (`dialog->y + top + rh*2`) | 1,077 | 72,257 | 584 |
| `baseY` declared before `baseX` | 1,077 | 72,257 | 584 |
| `2 * rowHeight` | 1,077 | 72,257 | 584 |
| `buttonInsetX + dialog->x` | 1,077 | 72,257 | 584 |
| `windowTopInset + dialog->y` | 1,077 | 72,257 | 584 |
| `height` before `width` | 1,079 | 72,259 | 584 |
| `width` after `textX` | 1,079 | 72,259 | 584 |
| `textX` = `rh + width + baseX` / `baseX + width + rh` / `baseX + rh + width` | 1,077 | 72,257 | 584 |
| `canvas` before `textY` | 1,079 | 72,259 | 584 |
| `textY` before `textX` | 1,077 | 72,257 | 584 |
| `textY` operand swap / `f4` first | 1,077 | 72,257 | 584 |
| `textYv` = `int&` / `volatile int` / `volatile int*` | 8,768 / 2,892 / 1,077 | — | 1,706 / 1,426 / 584 |
| no `textYv` / `int textYv` / `const int&` | 8,768 | 392,825 | 1,706 |
| `canvas` volatile | 2,799 | 331,628 | 724 |
| `Layout **layp` / `Layout *&lay` / `layp` rh-only | 1,077 | 72,257 | 584 |
| `f4` local after `rh` / after `textX` | 2,714 / 1,132 | — | 717 / 585 |
| `baseX`/`baseY` `Layout *lay` local | 3,475 | 379,935 | 1,427 |
| `i == 0` -> `!i`, else-first | 3,507 | 335,961 | 836 (9,676 B) |
| `String("")` -> `String("", false)` | 1,077 | 72,257 | 584 |
| named `String empty` / default ctor | 2,799 / 2,752 | 331,628 / 331,581 | 729 / 723 (9,692 B) |
| free-credits statement order (24 perms of SetColor/DrawString/SetColor/if) | 1,077–3,532 | — | 584–1,440 |
| landed + `canvas` before `textY` | 1,075 | 72,255 | 584 |
| landed + `height` first | 1,075 | 72,255 | 584 |
| landed + `baseY` declared before `baseX` | 1,073 | 72,253 | 584 (tie) |

### Ship-guard body (all 120 permutations re-measured on this base)

Only the following are size-exact; `VUDPB` is the base.

| order | raw | `.text` | extab |
|---|---:|---:|---:|
| `VUDPB` (base) | 1,077 | 72,257 | 584 |
| `VUPDB` | 1,075 | 72,255 | 1,217 |
| `DUPVB` | 1,067 | 72,247 | 1,217 |
| `DUVPB` | 1,078 | 72,258 | 585 |
| `VDUPB` / `DVUPB` | 1,109 / 1,110 | 72,289 / 72,290 | 1,217 |
| `UVDPB`/`UVPDB`/`UDVPB`/`UDPVB`/`UPVDB`/`UPDVB` | 1,225–1,230 | 72,405–72,410 | 593–594 |

`DUPVB`/`VUPDB` remain the best raw but their LSDA row-40 length (126/…) is a
1-byte ULEB, so `.ARM.extab` jumps to 1,217 — a net loss.  The landed
free-credits change is independent of the ship body.

### Other regions re-checked

| variant | raw | `.text` | extab |
|---|---:|---:|---:|
| buy-list `switch (count)` plain / `(int)` | 3,441 | 379,901 | 1,432 (9,704 B) |
| buy-list `(short) count` | 1,077 | 72,257 | 584 |
| buy-list String decl/assign swaps (4 forms) | 1,079–1,092 | — | 584 |
| buy-list `setVisible` before/mid Strings | 1,157 / 1,077 | — | 26,886 / 586 |
| buy-list `lay` outside loop / hoist `top`+`rh` | 3,364–3,492 | — | 761–1,436 |
| buy-list `x`/`y` operand reorders (7 forms) | 1,077 (one 3,057) | — | 584 |
| blueprint `bx`/`by` locals | 7,547 | 384,219 | 28,328 (9,704 B) |
| blueprint `Layout *lay` at function top | 8,929–9,298 | — | 898–1,641 |
| buttons[17] `draw/setPosition/replace` order (6 perms) | 1,077–8,625 | — | 584–1,556 |
| buttons[17] `rh`/`top`/`gsy` decl order (6 perms) | 1,077 | 72,257 | 584 |
| ship guard comparison swap | 1,082 | 72,262 | 584 |
| ship guard sibling shape | 4,561 | 75,741 | 1,223 |

## Residual after this wave

The base remains a strong local optimum.  The live leads are unchanged:
the blueprint `ldrd sl,r9` RA tie at `0x602`, the buy-list `sl`/String-temp
tie (ours spills `&Globals::layout` and uses `sl` for the second String,
so the free-credits block starts 2 B early and the LSDA rows 39/40 differ),
and the free-credits `width`-in-`r6`-vs-`sl` allocation.

## Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN12HangarWindow6renderEv
python3 _work/reconstructions/extab_lsda_decode.py \
        /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
        cmake-build-match/libgof2hdaa.so
cmake --build cmake-build-match --target check -j2
```

Scratch (not committed): `/home/debian/GalaxyOnFire2/scratch/w13461/`
(`HangarWindow.base.cpp`, `sweep.py`, `v*.py`, `gen_*.py`, `side.py`,
`align.py`, `secdiff.py`).

## Wave-190 second pass (same worker): free-credits button call order — LANDED

After the first pass (`bf0560523`), the free-credits loop's three button
calls were re-swept in all six orders on the landed base (raw 1,073).

| order | raw | aligned | `.text` | extab |
|---|---:|---:|---:|---:|
| **`DVP` (landed: draw, setVisible, setPosition)** | **1,039** | **620** | **72,219** | **584** |
| `VDP` (setVisible, draw, setPosition) | 1,045 | 620 | 72,225 | 584 |
| `DPV` (draw, setPosition, setVisible) | 1,072 | 615 | 72,252 | 584 |
| `VPD` (faithful: setVisible, setPosition, draw) | 1,073 | 613 | 72,253 | 584 |
| `PDV` | 1,076 | 624 | 72,256 | 584 |
| `PVD` | 1,077 | 624 | 72,257 | 584 |

The landed order is `button->draw(); button->setVisible(visible); int y = ...;
button->setPosition(baseX, y);`.  The `int y` must stay between `setVisible`
and `setPosition`: `y` before `draw` or before `setVisible` is 3,700 raw
(9,716 B); `y` inline in `setPosition` fails to build; moving `draw()` after
the `if (visible)` block is 8,767 raw (9,752 B).

Mechanism: the landed setVisible call lands at `0x1c22` and the setPosition
call at `0x1c3e`, exactly matching the original, so the whole
`0x1c1e..0x1c3e` block (34 bytes) becomes byte-identical.  The original
source order is `V,P,D` (its `draw` is at `0x1c44`), so this is a
phase-alignment reorder: `draw()` moves from after `setPosition` to before
`setVisible`, which is length-neutral (4 B either way) and keeps the function
at 9,700 B with `.ARM.extab` unchanged at 584.

Gate after the pass: `check` RATCHET PASS — byte_exact 4507/4524,
linked_exact 4507/4524, size-exact 4524/4524, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact 843/843, imports 207, lint CLEAN, drift 0,
parity 5370/5370.  `.rel.plt` byte-identical.

## Wave-190 third pass (same worker): free-credits `baseX` operand order — LANDED

The free-credits preheader's `baseX` expression was written
`this->dialog->x + Globals::layout->buttonInsetX`; the original computes the
layout inset first (`add.w r8,r6,r2` with `r6 = layout->field_0x28`,
`r2 = dialog->x`).  Swapping the operands to
`Globals::layout->buttonInsetX + this->dialog->x` is semantics-preserving.

| metric | before | after |
|---|---:|---:|
| raw same-address diff | 1,039 | **1,038** |
| instruction-index alignment | 620 | 620 |
| function size | 9,700 / 9,700 | 9,700 / 9,700 |
| `.text` section diff | 72,219 | **72,218** |
| `.ARM.extab` section diff | 584 | **584** |
| build-id | 20 | 20 |
| total (text+extab+build-id) | 72,823 | **72,822** |

Gate: `check` RATCHET PASS.  `.rel.plt` byte-identical, 27/27 headers,
file size 2,187,328 exact.

Negative results measured on the same base (all size 9,700 unless noted):

| variant | raw | `.text` | extab |
|---|---:|---:|---:|
| **`baseX` inset-first (landed)** | **1,038** | **72,218** | 584 |
| base (`dialog->x + inset`) | 1,039 | 72,219 | 584 |
| `baseY` faithful (`dialog->y + top + rh*2`) | 1,043 | 72,223 | 584 |
| `baseY` `top + dialog->y + rh*2` | 1,043 | 72,223 | 584 |
| `baseY` `dialog->y + rh*2 + top` | 1,039 | 72,219 | 584 |
| `baseY` `rh + rh + ...` | 1,039 | 72,219 | 584 |
| free-credits `width` before `rh` | 1,088 | 72,268 | 585 |
| free-credits `height` before `rh` | 1,041 | 72,221 | 584 |
| free-credits `width`,`height`,`rh` | 1,088 | 72,268 | 585 |

On the `D,U,P,V,B` ship body (`DUPVB`, raw 1,029 / extab 1,217) the
free-credits declaration/statement permutations re-measured neutral
(1,029 / 72,209) except `canvas` first (9,608 B, row-40 length 128) and
`width`-before-`rh` (1,078).  Row 40 stays 126 for every size-exact variant,
so the DUPVB ULEB is still unfixed.
