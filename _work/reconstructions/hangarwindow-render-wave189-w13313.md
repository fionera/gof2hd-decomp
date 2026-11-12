# HangarWindow::render — wave 189 (w13313): EXTAB-aware refinement of the 1,244 ship-guard shape

Worktree `wt/w13313`, branch `wave189-w13313`, base master `51d865ebe`
(wave-188 notes).  Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B, 3,734 insns.
Task: reproduce the w13302 candidate codegen (raw 1,244 / aligned 481) but
restore the original LSDA so `.ARM.extab` content stays at 639.

## Result: no net win — `src/` left at the wave-188 base

The w13302 candidate is reproduced exactly, and the mechanism behind the
`.ARM.extab` growth is now fully explained, but **no source shape was found
that keeps the 1,244 codegen while restoring the original landing pads**.
The two are coupled through register allocation (see below).  Best total
remains the wave-188 base at **74,698**.

| variant | raw | aligned | size | `.text` | `.ARM.extab` | build-id | total |
|---|---:|---:|---:|---:|---:|---:|---:|
| **base (wave-188, kept)** | **1,510** | **643** | 9,700 | **74,039** | **639** | 20 | **74,698** |
| cand (w13302 shape) | 1,244 | 481 | 9,700 | 73,773 | 1,234 | 20 | 75,027 |
| cand + inner `expect` (`ce01`) | 1,235 | 492 | 9,700 | 73,764 | 1,234 | 20 | 75,018 |
| base + upgrade-block `spdd` order | 1,513 | 643 | 9,700 | 74,042 | 639 | 20 | 74,700 |
| base + upgrade-block `dspv` order | 1,512 | 643 | 9,700 | 74,041 | 639 | 20 | 74,700 |
| cin_true (decl+upgrade-if in body) | 6,641 | 630 | **9,704** | size+32 | 1,495 | 20 | — |
| cin_true + all expects (`ci111`) | 1,521 | 654 | 9,700 | 74,050 | 639 | 20 | 74,709 |

All numbers are the ELF-section diff (`secdiff`), not `fn_diff` raw:
`.text` + `.ARM.extab` + `.note.gnu.build-id` (20).  `.rel.plt` untouched,
27/27 headers, file size 2,187,328 exact, `.ARM.extab` size/base exact
(156,312 B at 0x1a3b78) for every 9,700 variant above.

## The w13302 candidate (reproduced)

Inside `if (item->isShip())` of the `item == this->selectedItem` branch:

```cpp
int upgTab = tab;
if (tab != 0 && (tab != 3 || Globals::status->getShip() != this->selectedItem->ship)) {
    (*this->buttons)[1]->setPosition(
        Globals::layout->buttonInsetX + this->hintOffsetX + listWidth,
        this->field_0x114 + y, 18);
    (*this->buttons)[1]->setVisible(true);
    (*this->buttons)[1]->draw();
    buttonOffset = (*this->buttons)[1]->getWidth() + Globals::layout->field_0x2c_rowHeight;
}
if (this->upgradeMode && upgTab == 1) {
    (*this->buttons)[2]->setVisible(true);
    (*this->buttons)[10]->setPosition(Globals::layout->buttonInsetX + this->hintOffsetX,
                                      this->field_0x114 + y, 17);
    (*this->buttons)[10]->setVisible(true);
    (*this->buttons)[10]->draw();
}
```

Machine code (ours) vs the original ship guard — the body and upgrade block
now match the original instruction-for-instruction; only the `tab == 0` edge
differs:

```
orig  149b1e ldr  r0,[sp,#108]        cand  149b1e ldr  r0,[sp,#108]
      149b20 ldrd r5,r4,[sp,#132]           149b20 ldrd r5,r4,[sp,#132]
      149b24 cbz  r0,149b96   (2 B)         149b24 cmp  r0,#0
      149b26 cmp  r0,#3                     149b26 beq.w 14a6d0   (6 B)
      149b28 bne.n 149b44                   149b2a cmp  r0,#3
      ...                                   149b2c bne.n 149b44
      149b3c ldrd r5,r4,[sp,#132]           ...
      149b40 beq.w 14a6d0                   149b40 beq.w 14a6d0
```

Both are 44 bytes (`0x149b1e..0x149b44`); the original spends 4 B on the
second `ldrd` and 2 B on `cbz`, cand spends 0 B on the reload and 6 B on
`cmp/beq.w`.  Size stays 9,700 either way.

## Root cause of the `.ARM.extab` growth

`.ARM.extab` total diff 1,234 = 676 (HangarWindow record) + 558 (nine other
records, constant across all variants).  Base's HangarWindow record diff is
81, so the candidate adds 595.

The LSDA record is 948 B / 156 rows in both.  Decoded rows (orig vs cand):

```
row 39  orig (2508, 150, 9664)   cand (2508, 118, 9664)
row 40  orig (2696, 140, 9562)   cand (2654, 182, 9514)
```

Row 39 is the call-site range that ends at the `ListItem::isShip()` call
(`0x149b16..0x149b1a`).  In the original, `Status::getShip()`
(`0x149b36`) shares that range's landing pad (9664, the loop-body cleanup
that destroys the `name` String), so the range runs to `0x149b3a`
(length 150).  In cand, `getShip()` unwinds to a *different* pad (9514),
so row 39 stops at 118 and `getShip` is merged with the body into row 40.

118 is a 1-byte ULEB128, 150 is 2 bytes, so the whole 948-byte record shifts
by one byte from row 39 onward — that is why 676 of 948 bytes differ
(the `recdiff` metric) even though only rows 39/40 actually changed.

### Why the pad changes: any local declaration in the ship block creates an EH cleanup scope

Verified on the IR (`ircheck`/`irpads`: compile the TU to LLVM IR and read
the `unwind label` of the `getShip` invoke):

| ship-block source | `getShip` unwind pad |
|---|---|
| `int upgTab = tab;` before the guard (cand) | **new ship-block pad** (`%1765`) |
| `const int upgTab`, `unsigned`, `long long`, `float`, `double`, `void*`, `bool`, `char`, `int upgTab; upgTab = tab;` | new ship-block pad |
| no declaration | enclosing pad (`%452`) |
| declaration inside the guard body (cin_true / base) | enclosing pad (`%452`) |
| declaration in the enclosing `if (item == selectedItem)` block | enclosing pad, but regalloc changes |
| `static int upgTab;` (no lifetime markers) | enclosing pad, but +16 B |

So clang emits `llvm.lifetime.start/end` + an `EHCleanup` for *any* local in
the ship block; the block's landing pad then covers every invoke in it,
including the `getShip` call in the guard condition.  The pad itself is
empty (`br` to the loop-body pad), exactly like the original's row-40 pad —
but it is a *different* address, so the two rows split.

### Why the codegen cannot simply be decoupled

The 1,244 codegen (the `ldrd r5,r4,[sp,#132]` early load and the r4/r5 body
allocation) only appears in the **pad-1765** family.  Removing the ship-block
declaration (`nodecl`, `decl_end`, `scoped`, `preship`, `static`,
`decl_inside`) flips `getShip` back to `%452` but changes the body to
r2/r3/r6 stack reloads, which lengthens the guard by 10 B and shifts the
whole non-ship switch block (`.text` diff 79,399 instead of 73,773).
The IR in the two cases is otherwise identical apart from the extra
landing-pad block and one `llvm.dbg.value`; the divergence is pure
register-allocation/block-order fallout from the extra pad block.

`cin_true` (decl + upgrade-if both inside the guard body) is the only shape
that reproduces the original's *row structure* (row 39 length 150,
`getShip` → `%452`), but it is **9,704 B**: because the upgrade check lives
inside the guarded body, clang must emit `cmp r0,#0; beq.w <far merge>`
(6 B) instead of `cbz r0,<nearby check>` (2 B), and no body/upgrade-block
ordering or `__builtin_expect` combination tried recovers the 4 bytes.
Its `.ARM.extab` is also 1,495 (the +4 shift moves every landing pad).

## Variants measured this wave (all size 9,700 unless stated)

Sweeps: guard form (3) x decl form (4) x check form (3) x body order (2) x
upgrade-block order (2) x upgrade-if placement (2) — 96 builds; plus
`__builtin_expect` grids, hoisted/scoped/static declarations, `bool`
snapshot, uninitialised snapshot, check-before-body, and `Layout *lay`
placement.

| variant | raw | size | `.text` | `.ARM.extab` | total |
|---|---:|---:|---:|---:|---:|
| base | 1,510 | 9,700 | 74,039 | 639 | **74,698** |
| cand | 1,244 | 9,700 | 73,773 | 1,234 | 75,027 |
| cand + inner expect | 1,235 | 9,700 | 73,764 | 1,234 | 75,018 |
| cand, upgradeMode-first check | 1,249 | 9,700 | 73,778 | 1,234 | 75,032 |
| cand, draw-before-setVisible | 1,258 | 9,700 | 73,787 | 1,234 | 75,041 |
| cand, upgrade block `sspd` | — | 9,700 | 73,815 | 1,234 | 75,069 |
| cand + outer expect only | 1,539 | 9,700 | 74,068 | 1,230* | 74,886* |
| cand + both expects | 7,295 | 9,652 | — | — | — |
| cin_true | 6,641 | **9,704** | — | 1,495 | — |
| cin_true + all expects (`ci111`) | 1,521 | 9,700 | 74,050 | 639 | 74,709 |
| base + `spdd` upgrade order | 1,513 | 9,700 | 74,042 | 639 | 74,700 |
| base + `dspv` upgrade order | 1,512 | 9,700 | 74,041 | 639 | 74,700 |
| `Layout *lay` moved after the switch | 3,563 | 9,692 | — | 787 | — |
| `Layout *lay` removed (use `Globals::layout`) | 3,532 | 9,696 | — | 780 | — |
| `HangarAppData *data` moved after the loop | 9,001 | 9,704 | — | 1,827 | — |

\* `.ARM.extab` section size 156,308 (4 rows fewer), so not size-exact.

## Next target: the +4 tail shift (shared with base)

Independent of the ship guard, the tail is 4 B out of phase from
`0x14a960` onward: ours emits an extra `ldr.w sl, [sl]` (4 B) that the
original does not, right before the `switch (count)` jump table in the
`buyCreditsActive` / `listModeFlag` loop.  That shift makes the 574-byte
`tbb` jump table differ and moves every landing pad in rows 99-131.  It is
worth roughly 1,134 `.text` bytes and ~558 `.ARM.extab` bytes if fixed, and
would make *either* the base or the cand shape a large win.  Related
observations: at `0x14a8ec` the original stores only `r9` to `[sp,#132]`
while ours stores `strd sl, r9, [sp,#128]`; the extra `sl` spill/reload is
the whole difference.

## Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN12HangarWindow6renderEv
python3 _work/reconstructions/extab_record_map.py
python3 _work/reconstructions/extab_lsda_decode.py   # (via dumprec.py)
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch (not committed): `/home/debian/GalaxyOnFire2/scratch/w13313/`
(`HangarWindow.base.cpp`, `HangarWindow.cand.cpp`, `measurefull.py`,
`mytotal.py`, `secdiff.py`, `regdiff.py`, `firstdiff.py`, `dumprec.py`,
`ircheck.py`, `irpads.py`, `sweepA.py`, `sweepB.py`, `sweepC.py`,
`batch.py`, `gen.py`, `x1.py`..`x4.py`).

## Harvest on wave-189 master (`75e111e90`)

Note-only: the branch landed no source change (raw stays 1,510 / 643).  On
current master `.text` is 73,999 and total 74,658; the w13302 1,244 candidate
would still cost the documented `.ARM.extab` 639 -> 1,234, so it remains a
net loss.  The next target is the independent +4 tail shift at `0x14a960`
(extra `ldr.w sl,[sl]` before the `switch (count)` jump table), worth
~1,134 `.text` + ~558 `.ARM.extab` bytes.
