# HangarWindow::render — wave 191b (w13823): free-credits `textY`/`textYv` arm swap

Worktree `wt/w13823`, branch `wave191b-w13823`, base master `6b680e716`
(includes w13799: `buttons[17]` row-height inline, free-credits `rh`-first,
unsigned-char switch).  Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B.

## Landed win

In the free-credits `if (i == 0) { ... } else { ... }` block, swap which
variable feeds each `DrawString` y argument:

```cpp
if (i == 0) {
    canvas->DrawString(hangarFont(), String(""), textX, textY + Globals::layout->field_0x4, false);
} else {
    canvas->DrawString(
        hangarFont(),
        String("(+") + Globals::layout->formatCredits(FREE_CREDIT_AMOUNTS[i]) + String(")"), textX,
        textYv + Globals::layout->field_0x4, false);
}
```

(The first `DrawString` above the branch still uses `textYv`; `int textY` and
`volatile int textYv = textY;` are unchanged from w13767/w13799.)

The original keeps the computed `textY` in the callee-saved `r6`
(`0x14ad8c vmov r6,s0`) and reuses it for both branch arms
(`0x14ae0a` and `0x14ae52`: `add r0,r6`).  The w13799 arm assignment forced an
extra reload of the volatile slot; the swap recovers part of that.

### Measured (full `link_so`, `-j2`)

| metric | base (w13799) | landed |
|---|---:|---:|
| raw same-address diff | 2,050 | **2,018** |
| instruction-index alignment | 317 | 317 |
| function size | 9,700 | 9,700 |
| instructions | 3,734 / 3,734 | 3,734 / 3,734 |
| `.text` diff | 76,743 | **76,711** |
| `.ARM.extab` diff | 625 | **623** |
| `.ARM.exidx` / `.rodata` / `.rel.plt` | 0 / 0 / 0 | 0 / 0 / 0 |

Net: raw −32, `.text` −32, `.ARM.extab` −2, size exact, `.rel.plt`
byte-identical.  The w13799 shape (rh-first, unsigned-char switch,
`buttons[17]` row-height inline) is preserved.

## Variants measured this session (no further raw win)

All full `link_so` builds, size 9,700 unless noted.  Base = landed above.

| variant | raw | aligned | `.text` | extab |
|---|---:|---:|---:|---:|
| **landed (`textYv,textY,textYv`)** | **2,018** | **317** | **76,711** | **623** |
| free-credits `width,rh,height` (original statement order) | 2,016 | 412 | 76,709 | 623 |
| buy-list `String description` before `price` | 2,021 | **306** | 76,714 | 623 |
| free-credits `textY,textYv,textYv` | 2,038 | 328 | 76,731 | 621 |
| free-credits `textYv,textYv,textY` (w13799 base) | 2,050 | 317 | 76,743 | 625 |
| free-credits `textYv,textYv,textYv` | 3,519 | 344 | — | 759 (size **9,696**) |
| free-credits `textY,textYv,textY` | 3,567 | 487 | — | 1,442 (size **9,704**) |
| free-credits `textY,textY,textYv` | 3,556 | 486 | — | 1,442 (size **9,704**) |
| free-credits `textY,textY,textY` | 3,606 | 493 | — | 1,451 (size **9,712**) |
| free-credits `textYv,textY,textY` | 3,607 | 493 | — | 1,445 (size **9,708**) |
| volatile after first `DrawString` | 8,783 | 1,382 | — | 1,624 (size **9,728**) |
| volatile after `SetColor` | 8,785 | 1,382 | — | 1,624 (size **9,728**) |
| volatile only inside `i == 0` arm | 3,529 | 499 | — | 1,439 (size **9,708**) |

`width,rh,height` (the original's `getWidth` → `rowHeight` → `getHeight`
schedule at `0x14ad26..0x14ad38`) is a 2-byte raw win but drops the
instruction count to 3,733/3,734 and raises alignment to 412; the w13799
`rh`-first shape is the size-exact instruction-count anchor, so it is kept.
Buy-list `description`-first is the best aligned shape (306) but costs +3 raw.

## Remaining aligned clusters (landed)

- `0x149b20`/`0x149b3c`/`0x149b6e` — ship guard: original hoists
  `ldrd r5,r4,[sp,#132]` before `cbz`, emits `cbz 149b96` (upgrade check
  outside the guard body) and the body order `setPosition → setVisible →
  draw`; ours keeps the upgrade check nested and `setVisible → draw →
  setPosition`.  Moving the upgrade check out (faithful shape) is size-exact
  but 3,736 instructions / raw 6,767–6,813 and `.ARM.extab` diff 19,047.
- `0x14ad22..0x14ae52` — free-credits `rh` spill (`str [sp,#116]`) and
  `textY` volatile spill (`vstr s0,[sp,#204]`) vs the original's `r9`/`r6`
  allocation; downstream of the known `this`↔`&Globals::layout` RAGreedy tie
  at `0x1496da` (`ldrd r9,sl` vs `ldrd sl,r9`), documented in
  `hangarwindow-render-wave191b-w13580.md`.
- `0x14a938`/`0x14a94e`/`0x14ac54` — buy-list `String` temp registers
  (`r6`/`r5` vs `r5`/`sl`) and the `sp+312` `StringD1Ev` slot, same RA tie.
- `0x14b4be..0x14b688` — LSDA landing pads / jump-table `b.n` offsets
  (ours +2 B), not source-orderable without changing size.

## Gate

`check` green after the landing: byte_exact 4508/4524, linked_exact 4508/4524,
size-exact 4524/4524, stub_zero_size 0, missing 0, wrong_type 0, extra 0,
data_exact 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.

## Checkpoint 3 (w13823 continuation): RA-tie probes, still no further win

Baseline re-confirmed after every experiment: raw **2,018**, aligned **317**,
size **9,700/9,700**, insn **3,734/3,734**, `.text` **76,711**,
`.ARM.extab` **623**, `.ARM.exidx`/`.rodata`/`.rel.plt` 0.  The landed win
above is unchanged; `src/` is clean.

The remaining aligned clusters are all downstream of the single
`this` ↔ `&Globals::layout` RAGreedy tie at `0x1496da`
(`ldrd r9, sl` vs the original `ldrd sl, r9`; `[sp+132]` = `&Globals::layout`,
`[sp+136]` = `this`, so ours gives layout→`r9`/this→`sl`, the original the
reverse).  The whole pre-block `0x1496b0..0x1496de` is byte-identical, so the
tie is pure post-RA register choice, not statement order.

### New probes this checkpoint (full `link_so`, `-j2`)

All size 9,700 unless noted.  Base = landed (`raw 2,018 / aligned 317 /
.text 76,711 / extab 623`).

| variant | raw | aligned | `.text` | extab | notes |
|---|---:|---:|---:|---:|---|
| **landed (base)** | **2,018** | **317** | **76,711** | **623** | insn 3,734 |
| progress `DrawImage2D` x-term `layout` first | 2,018 | 317 | 76,711 | 623 | canonicalised |
| progress `DrawImage2D` y-term `layout` first | 2,018 | 317 | 76,711 | 623 | canonicalised |
| progress `DrawImage2D` x/y args swapped | 2,027 | 341 | 76,720 | 623 | ldrd **not** flipped |
| progress `DrawImage2D` image/y args swapped | 2,036 | 342 | 76,729 | 623 | ldrd not flipped |
| `DrawRegion2D` x-term `layout` first | 2,689 | 548 | 77,382 | 640 | insn 3,733 |
| progress `int hintX = this->hintOffsetX` local | 8,511 | 898 | — | 19,457 | size **9,704** |
| `buttons[17]` y `- top * 3` / `-(top*3)` / `h/2-3*top-gbh/2` | 2,018 | 317 | 76,711 | 623 | canonicalised |
| `buttons[17]` y split `(h/2-gbh/2) - (3*top)` | 2,621 | 490 | — | 1,423 | size **9,708** |
| ship body `P,V,D` (`upgTab` mid) | 6,740 | 655 | — | 1,942 | size **9,708**, insn 3,736 |
| ship body `P,V,D` (`upgTab` before `P`) | 6,557 | 637 | — | 1,498 | size **9,704**, insn 3,736 |
| ship body `P,D,V` (`upgTab` mid) | 6,738 | 657 | — | 1,942 | size **9,708**, insn 3,736 |
| ship body + hoisted `px`/`py` locals | 8,075 | 687 | — | 28,064 | size **9,712**, insn 3,736 |

Conclusions:

- **The `ldrd` tie is not statement-orderable.**  Reordering the
  `DrawImage2D`/`DrawRegion2D` operands (evaluation order) leaves the
  `ldrd r9, sl` unchanged; the allocator's choice follows the live-range
  spill weights (`this` ~12 uses / `layout` ~4 uses), not the block order.
  Every shape that perturbs the weights (a `this`- or `layout`-caching local)
  changes the function size.
- **The ship guard body order is size-locked.**  The original's
  `setPosition → setVisible → draw` order (which would also hoist the
  `ldrd r5,r4,[sp,#132]` above the `cbz`) is **not** size-exact at this base:
  every `{P,V,D}` permutation is 9,704–9,712 B / 3,736 insns.  The landed
  `V,D,P` with `upgTab` in the middle is the unique size-exact anchor.
- **`buttons[17]` y scheduling** (`sub.w r1,r6,r6,lsl#2` hoisted before
  `vmov s0,r5` vs after) is a machine-scheduler artifact; `- top * 3`,
  `-(top * 3)` and `Globals::h/2 - 3*top - gbh/2` all canonicalise back to
  the landed stream.  Splitting the int/float casts changes size (9,708).
- The `0x14961c`/`0x149812`/`0x14a85a`/`0x149c0a` single-insn diffs are
  literal-pool address shifts (code-size phase), and `0x14b614..0x14b698`
  are LSDA landing pads / jump-table `b.n` offsets (+2 B) — neither is
  source-orderable without changing size.

No further measured win; `src/` left clean.  The landed commit
`0a4ad9abd` stands as the net minimum at this base.
