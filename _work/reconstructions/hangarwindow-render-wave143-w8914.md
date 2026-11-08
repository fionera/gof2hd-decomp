# `HangarWindow::render` — wave 143 (w8914): r5/r6 flip + credit-loop LSDA

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`), original
`0x1490d8`, size `0x25e4` = 9700 B. Worktree `/home/debian/GalaxyOnFire2/wt/w8914`,
base `f5093474f` (wave 143; `render` gate byte 4507 / linked 4507).

Metric: **raw same-address diff bytes** in the 9700-byte body
(`/dev/shm/w8914/fdiff.py`). Baseline **1731**; this pass **1731** (no source change
landed — see "Why nothing landed"). `check` is green on the base tree:
byte 4507, linked 4507, size 4524, data 843, imports 207.

## Ground truth re-derived this wave

Original credit loop (`0x14a912..0x14ac5c`):

| value | stack slot | original reg | ours (base) |
|---|---|---|---|
| `description` (ctor first, dtor second) | `sp+312` | **r6** | r5 |
| `price` (ctor second, dtor first) | `sp+300` | **r5** | r6 |
| `&Globals::layout` | — | **sl** (loaded `0x14a85e`, kept through the loop, spilled `0x14ac9c`) | `lay` value in sl (`ldr.w sl,[sl]` at `0x14a960`, restore `ldr.w sl,[sp,#128]` at `0x14ac64`) |

Verified by relocation: the case-0 literal at `0x14aa98` resolves through GOT
`0x2101a8` to `_ZN7Globals23cItemListDescription_01E`; the first assignment in
every case writes r6/sp+312, the second r5/sp+300.  `replaceTextKeepSize` takes
sp+312, `setSplitText` takes sp+300.  So description = sp+312, price = sp+300.

The original's dtors (`0x14ac52` `mov r0,r5`; `0x14ac58` `add r6,sp,#312`) destroy
**price first, description second** — same order as our base.

## Why the r5/r6 flip is coupled to the slot order (new analysis)

The allocator's choice is not "longer interval wins" in isolation; it is
**first-declared wins r5** in every shape we can build:

| shape | description slot/reg | price slot/reg | dtor order | raw diff |
|---|---|---|---|---|
| base `String description(""); String price("");` | sp+312 / **r5** | sp+300 / **r6** | price, description | 1731 |
| price first | sp+300 / r6 | sp+312 / **r5** | description, price | 1731 |
| `String description(""), price("");` | sp+312 / r5 | sp+300 / r6 | price, description | 1731 |
| `String description = String("");` (copy-init) | sp+312 / r5 | sp+300 / r6 | price, description | 1731 |
| `String price = String("");` (copy-init) | sp+312 / r5 | sp+300 / r6 | price, description | 1731 |
| both copy-init | sp+312 / r5 | sp+300 / r6 | price, description | 1731 |
| scopeA `String price(""); { String description(""); … }` | sp+300 / r6 | sp+312 / **r5** | description, price | 1731 |
| `String description(""); { String price(""); … }` (desc outer) | sp+312 / r5 | sp+300 / r6 | price, description | 1731 |
| both Strings in an inner scope | sp+312 / r5 | sp+300 / r6 | price, description | 1731 |
| assignments swapped in every case (price first) | sp+312 / r5 | sp+300 / r6 | price, description | 1731 |
| `Layout **lay = &Globals::layout; (*lay)->…` | sp+312 / **sl** | sp+300 / r5 | price, description | 2062 |
| nolay (direct `Globals::layout`) | sp+312 / **sl** | sp+300 / r5 | price, description | 2062 |
| `if (Globals::showBestDeal && count == 4)` | sp+312 / r5 | sp+300 / r6 | price, description | 1731 |
| nested `if (count==4) { if (showBestDeal) … }` | sp+312 / r5 | sp+300 / r6 | price, description | 1731 |

Conclusion: the frame-index order (which sets the *slot*) and the vreg order
(which sets *r5*) both follow the declaration/ctor order, and the dtor order
follows the reverse declaration order.  The original is the only observed shape
where the **second-declared** variable (price) gets r5 while the first-declared
(description) keeps the high slot.  That requires price's live interval to end
*after* description's, i.e. description must be rematerialised at its dtor
(`add r6,sp,#312`) while price is kept in r5.  In our base it is the other way:
description is rematerialised into r5 at `0x14ab8c` and kept to the dtor, price
is rematerialised inline at `0x14abac` and again at the dtor.  The lever is the
`sl` role: the original keeps `&Globals::layout` in sl, so description cannot
live there and is forced into r6.

No source shape tried this wave produced `sl = &Globals::layout` together with
`description = r6`.  `Globals::layout` direct / `Layout**` / `Layout *const&`
all rematerialise or spill the GOT address and give `description = sl` (2062).

## Credit-loop LSDA: borderX fixes rows 19/20 but breaks byte_exact

`int borderX = this->hintOffsetX + (textX + 5 + Globals::layout->buttonInsetX +
(int)(rate * (float)(this->progressBarWidth - 4)));` before the 4th
`DrawImage2D` gives **raw diff 1698** (9700 B) and makes LSDA rows 19/20 exactly
match the original.  But it adds one call-site row in the credit loop:

```
orig 125 (6678,146,0)  126 (6824,210,9654)  127 (7034,390,0)
ours 125 (6682,144,0)  126 (6826,96,9656)   127 (7004,30,9602)  128 (7034,392,0)
```

The extra row is the `if (count == 4 && Globals::showBestDeal)` body
`[0x14abf2, 0x14ac52)`: in ours it is **not** covered by a landing pad (cleanup
0), while the original covers `[0x14ab90, 0x14ac58)` with the single landing pad
9654.  `.ARM.extab` grows 156,312 -> 156,316 (157 rows).

**Important correction to the w8818 handoff:** the `byte_exact 4044` seen with
borderX is **not** a verify.py nondeterminism.  Measured this wave:
base -> `byte_exact 4507`, borderX -> `byte_exact 4044`, deterministically.
The +4 `.ARM.extab` shifts `.rodata`/`.data`, which changes address-dependent
literal-pool words in 463 other functions.  Because `byte_exact` must not
decrease, borderX is **not committable** until the row merge restores the extab
size to 156,312.

Variants tried to merge the row (all still 157 rows / 952 B extab):
condition order swap, nested if, `int borderX` const / nested block / `if(true)`,
hoisting `iconX`/`iconY`, hoisting `showBestDeal`, moving the if before
`draw()` (9796 B), before `setSplitText`/`replaceText`/`DrawImage2D` (9712 B),
and wrapping the Strings in an inner scope so the if follows the dtors
(9692 B, 155 rows — wrong direction).

## Why nothing landed

The base tree is already the best **gate-green** variant: raw diff 1731,
extab 156,312, byte_exact 4507.  borderX is 33 raw bytes better but costs
463 byte_exact functions, so it is left as a candidate for the coordinator to
pick up once the credit-loop row merge is found.

## Open questions

* Find a shape that keeps `sl = &Globals::layout` (no `ldr.w sl,[sl]` /
  `ldr.w sl,[sp,#128]`) while description lives in r6.  Every direct
  `Globals::layout` shape puts description in sl.
* Merge the credit-loop if-body call-site row into the cleanup row (landing pad
  9654) so borderX keeps `.ARM.extab` at 156,312.  The if body's only call is
  `DrawImage2D`; the compiler currently emits cleanup 0 for that range.
* Free-credits `textX` is still scheduled after `getText` in ours, before it in
  the original; free-credits width-first hunk (`0x14b22e`) and tail (`0x14b562`)
  remain independent.

## Reproduction

```sh
# base (landed, gate-green)
python3 /dev/shm/w8914/fdiff.py _ZN12HangarWindow6renderEv   # 1731, 9700 B
# borderX candidate (raw 1698, extab +4, byte_exact 4044)
cp /dev/shm/w8914/b1_borderx.cpp src/game/ui/HangarWindow.cpp
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8914/fdiff.py _ZN12HangarWindow6renderEv   # 1698, 9700 B
```

Scratch (not committed): `/dev/shm/w8914/{fdiff.py,probe.py,probe2.py,batch.py,
HangarWindow.base.cpp,b1_borderx.cpp,z_*.cpp,q_*.cpp,y_*.cpp,w_*.cpp,
allpasses.txt,lia_render.txt,greedy_base.txt,orig_full.txt,ours_full.txt}`.
