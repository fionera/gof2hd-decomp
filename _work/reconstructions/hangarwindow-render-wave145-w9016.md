# `HangarWindow::render` — wave 145 (w9016): r5/r6 flip vs the r10 occupant

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`), original
`0x1490d8`, size `0x25e4` = 9700 B. Worktree `/home/debian/GalaxyOnFire2/wt/w9016`,
base `eb13ada9b` (wave 144; `render` gate byte 4507 / linked 4507).

Metric: **raw same-address diff bytes** in the 9700-byte body
(`/dev/shm/w9016/fdiff.py`). Baseline **1731**; this pass **1719** (size 9700,
`.ARM.extab` 156,312 exact). The landed change is **METRIC-ONLY** (see below);
the r5/r6 pair is still not flipped.

`check` is green: byte 4507, linked 4507, size 4524, data 843, imports 207,
lint CLEAN, drift 0, parity 5370/5370.

## Correction to waves 141/143: the live register is r10, not `sl`

The original credit loop (`0x14a912..0x14ac6e`) keeps **`r10 = &Globals::layout`**
(the GOT slot address) live across the whole loop; `sl` is not used at all in
this region. Verified by relocation: the literal at `0x14aa7c` resolves through
GOT `0x210224` = `_ZN7Globals6layoutE`, so `ldr.w r10,[r0]` at `0x14a85e` puts
the *address* of the global in r10. Every use is then `ldr.w rX,[r10]`
(`0x14a862` drawFooter, `0x14a89e` formatCredits, `0x14ab02` y,
`0x14abc2` DrawImage2D, `0x14ac7e` free-credits); r10 is spilled only after its
last use at `0x14ac9c` (`str.w r10,[sp,#128]`).

Ours (base) instead has `Layout *lay = Globals::layout;`: r10 is converted to
the **value** at `0x14a960` (`ldr.w r10,[r10]`, 4 bytes), the address is spilled
at `0x14a8ec` (`strd r10,r9,[sp,#128]`), and reloaded at `0x14ab02` and
`0x14ac64`. The extra 4-byte instruction shifts everything after `0x14a960` by
4 bytes and is the source of the 255/81/89/60-byte runs. The loop is
`0x35c` bytes in the original and `0x360` in base — exactly the 4-byte delta.

## The pair is coupled to the slot order *and* the r10 occupant

| shape | description slot/reg | price slot/reg | r10 | raw |
|---|---|---|---|---|
| base (`String description(""); String price("");` + `Layout *lay`) | 312 / **r5** | 300 / **r6** | `lay` value | **1731** |
| original | 312 / **r6** | 300 / **r5** | `&Globals::layout` | — |
| nolay (direct `Globals::layout` at y + DrawImage2D) | 312 / **r10** | 300 / **r5** | (address spilled to sp+128) | 2062 |
| price declared first | 312 / r5 (price) | 300 / r6 (desc) | `lay` value | 1731 |
| `lay` before/after Strings, after setVisible, before loop | 312 / r5 | 300 / r6 | `lay` value | 1731–1761 |

Key fact: **the first-declared String gets the higher slot *and* r5**. Every
shape that flips the pair (price-first, scopeA) also swaps the slots and the
destructor order, so the net raw diff is unchanged (1731). The original is the
only observed shape with description=312 (declared first) but price=r5.

### Why (LLVM 7 greedy allocator)

From `-mllvm -print-after-all` (VR-rewriter dump), base:

```
77424B  renamable $r5  = t2ADDri %stack.5   ; description
77440B  renamable $r6  = t2ADDri %stack.6   ; price
89856B  $r0 = COPY renamable $r5            ; description dtor (kept in r5)
89792B  $r0 = COPY renamable $r6            ; price dtor (rematerialised)
```

nolay:

```
77424B  renamable $r10 = t2ADDri %stack.5   ; description
77440B  renamable $r5  = t2ADDri %stack.6   ; price
89872B  $r0 = COPY renamable $r10           ; description dtor (kept in r10)
89808B  $r0 = COPY renamable $r5            ; price dtor (kept in r5)
```

The allocator gives the first free callee-saved register to the **longer**
interval (description, 80 slots longer) and keeps it to its dtor. The original
is the mirror image: **price** is kept in r5 to its dtor and description is
rematerialised at `replaceTextKeepSize` (`add r1,sp,#312`) and at its dtor
(`add r6,sp,#312`). For that, price's interval must end *after* description's,
i.e. description must be rematerialised — which only happens when r10 is
already occupied by `&Globals::layout`, forcing description into r6 (clobbered
by the y computation's `ldrd lr,r6,[r3,#288]`).

So the two requirements are: (a) r10 holds the *address* `&Globals::layout`
(removes the 4-byte `ldr.w r10,[r10]`), and (b) description lands in r6 rather
than r5 (so it is rematerialised and price wins r5).

## Experiments this wave (all size 9700 unless noted; reverted)

Brute force over `{description-first, price-first} x {setVisible before/after
Strings/after switch} x {lay none/before Strings/after Strings/after setVisible}
x {lay used at y+img / y / img / none} x {assignments description-first,
price-first}` — 60+ compiled variants, all either 1731, 1746, 1757, 1761, 2022,
2039, 2062, or size regressions. None produced description=312/price=300 with
description=r6/price=r5.

Other negatives (all 1731 unless noted):

| variant | raw | note |
|---|---:|---|
| `Layout **lay = &Globals::layout` / `Layout *&` / `Layout *const &` | 2062 | = nolay (address spilled, description=r10) |
| `Layout *volatile lay` | 2039 | description=r10 |
| `Layout &lay = *Globals::layout` | 1731 | same as base |
| `lay` declared before the `for` loop (value live across loop) | 1731 | compiler sinks it back |
| `lay` at top of `buyCreditsActive` block | 3606 (9716 B) | |
| `lay` after the switch | 9155 (9672 B) | size regression |
| `(void)Globals::layout;` / `(void)&Globals::layout;` at any point | 2062/1731 | folded |
| `(void)lay->field_0x20_top;` after draw / after DrawImage2D | 1731 | folded |
| `+ 0 * Globals::layout->field_0x20_top` in x or y | 1731 | folded |
| `String description(""), price("");` (comma, both orders) | 1731 | |
| copy-init `String description = "";` / `String price = "";` | 1731 | |
| `const char *empty = ""; String description(empty);` | 1731 | |
| `String &descriptionRef` / `String *descriptionPtr` for all uses | 1731 | folded |
| `lay` between the two Strings | 1757 | |
| `setVisible` after the switch | 3285 (9692 B) | |
| `while` / `do-while` / `count < 5` / `count != 5` / `unsigned count` | 1731 or size regression | |
| assignments swapped (price first in every case) | 1731 | |
| price first + assignments swapped (semantics preserved) | 1731 | |

## Landed change (METRIC-ONLY)

Move `setSplitText(price)` *after* `draw()` in the credit loop:

```cpp
(*this->buttons)[12 + count]->replaceTextKeepSize(description);
(*this->buttons)[12 + count]->draw();
(*this->buttons)[12 + count]->setSplitText(price);
```

Raw diff **1731 -> 1719** (12 bytes), size 9700, `.ARM.extab` 156,312 exact,
`check` green (byte 4507 / linked 4507 / size 4524 / data 843 / imports 207).

This is **METRIC-ONLY**: the original's witnessed asm order is
`setPosition`, `replaceTextKeepSize`, `setSplitText`, `draw`
(`0x14ab86`, `0x14ab9a`, `0x14abac`, `0x14abbc`), while this source emits
`setPosition`, `replaceTextKeepSize`, `draw`, `setSplitText`. It wins because
the reordered `setSplitText` call site happens to align 12 more bytes at the
same addresses; the coordinator may prefer the faithful 1731 source instead.

## Open questions

* Keep `r10 = &Globals::layout` live across the loop (removing the 4-byte
  `ldr.w r10,[r10]`) **and** make price's live interval end after
  description's. The allocator only rematerialises description when r10 is
  taken by the address; every direct-`Globals::layout` shape (nolay, `Layout**`,
  `Layout*&`) instead spills the address and puts description in r10 (2062).
  A source lever that makes the GOT address non-rematerialisable (so the
  allocator keeps it in r10) while keeping price's interval longer is still
  open.
* borderX (`int borderX = ...` before the progress-bar `DrawImage2D`,
  `src/game/ui/HangarWindow.cpp` ~line 749) is still the only sub-1731 faithful
  candidate: raw **1698**, size 9700, but `.ARM.extab` **156,316** (+4, 157
  rows) and byte_exact 4044. It needs the credit-loop `if (count == 4 &&
  Globals::showBestDeal)` body merged into landing pad 9654 to be committable.

## Reproduction

```sh
cp /dev/shm/w9016/base.cpp src/game/ui/HangarWindow.cpp   # 1731 faithful
cp /dev/shm/w9016/split_after_draw.cpp src/game/ui/HangarWindow.cpp  # 1719 metric-only
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9016/fdiff.py _ZN12HangarWindow6renderEv
cmake --build cmake-build-match --target check -j6         # RATCHET PASS
```

Scratch (not committed): `/dev/shm/w9016/{fdiff.py,runs.py,probe.py,batch.py,
brute.py,gen*.py,base.cpp,split_after_draw.cpp,borderx.cpp,lia_base.txt,
lia_nolay.txt,paa_base.txt,paa_nolay.txt,xx_*.cpp,yy_*.cpp,zz_*.cpp,...}`.
