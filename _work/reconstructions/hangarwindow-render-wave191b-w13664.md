# HangarWindow::render — wave 191b (w13664): faithful ship/upgrade/free-credits
# statement orders, no net win

Worktree `wt/w13664`, branch `wave191b-w13664`, base master `a3f6a5b88`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B.

## Result: no net-positive win landed; `src/` untouched

Baseline re-measured and left pristine:

| metric | value |
|---|---:|
| raw same-address diff | **1,038** |
| instruction-index alignment | 620 |
| function size | 9,700 / 9,700 |
| instructions | 3,726 / 3,734 |
| `.text` section diff | 71,210 |
| `.ARM.extab` section diff | 577 |
| `.ARM.exidx` / `.rodata` / `.rel.plt` | 0 / 0 / 0 |
| `.note.gnu.build-id` | 20 |
| total differing bytes | **71,807** |

`check` RATCHET PASS: byte_exact 4508/4524, linked_exact 4508/4524,
size-exact 4524/4524, stub/missing/wrong_type/extra 0, data_exact 843/843,
imports 207, lint CLEAN, drift 0, parity 5370/5370.

## The residual is one greedy-queue ordering, and it is not size-driven

The blueprint block is byte-identical from `0x1496d8` to the reload at
`0x1496da`, then:

```
orig 1496da  e9dd a921  ldrd sl, r9, [sp,#132]   ; sl=&Globals::layout, r9=this
ours 1496da  e9dd 9a21  ldrd r9, sl, [sp,#132]   ; r9=&Globals::layout, sl=this
```

The prologue stores are identical (`[sp+132]=&Globals::layout`,
`[sp+136]=this`), so this is purely a physical-register choice.  Every
later cluster (buy-list preheader `strd r9,sl,[sp,#128]` vs `str r9,[sp,#132]`,
the free-credits `rh`/`width`/`height` roles, `buttons[17]`) is downstream
of it.

New evidence from a fresh `-mllvm -print-after-all
-mllvm -filter-print-funcs=_ZN12HangarWindow6renderEv` dump (80 MB, scratch):

* MIR bb.288 reloads, in order, `%3331 = LDR %stack.75` (this, 47212B)
  then `%3356 = LDR %stack.78` (layout, 47220B).  VirtRegRewriter gives
  `%3331 -> r10`, `%3356 -> r9` (ours); the original is the reverse.
* Both reload intervals are contiguous (no holes).  `this` spans
  47212B..50336B = **222 instructions**; layout spans 47220B..50288B =
  **218 instructions**.  So `this` has the larger `LiveInterval::getSize()`.
* LLVM 7 `RAGreedy::enqueue` gives RS_Assign global ranges
  `Prio = (1<<31)|(1<<29)+Size` (plus `1<<30` for a known preference) and
  RS_Split ranges `Prio = Size`.  ARM `GPR` allocation order is
  `LR, r0..r12, sp, pc`, i.e. **r9 is handed out before r10** to the first
  range processed.
* Therefore ours processes *layout* before *this* even though *this* is
  larger.  The only consistent explanation is that `layout`'s reload is
  RS_Assign (or has a known preference) while `this`'s is RS_Split — the
  same conclusion w13613 reached.  The stage/hint is not observable in the
  r18b release-clang dump (`-debug-only=regalloc` is compiled out and
  `-print-after=greedy` is not a valid pass name in this build).

## Faithful statement orders read out of the Thumb disassembly

The original's ship/upgrade/free-credits blocks are *not* in the order the
current source uses.  Reconstructed from `0x149b20..0x149bf0` and
`0x14acf4..0x14ad1c`:

```
ship swap body:   buttons[1]->setPosition(...); setVisible(true); draw();
                  buttonOffset = getWidth() + layout->field_0x2c_rowHeight;
upgrade body:     buttons[2]->setVisible(true);            // literal true
                  buttons[10]->setPosition(...);
                  buttons[10]->setVisible(true); draw();
free-credits:     button->setVisible(visible); y=...; button->setPosition(); draw();
```

(also: the ship guard is two sequential `if`s — `if (tab != 0 && (tab != 3 || ...))`
then `if (tab == 1 && upgradeMode)` — and the upgrade body's argument is the
literal `1`, not `this->upgradeMode`.)

## Measured candidates (all size 9,700 unless noted)

| variant | raw | aligned | `.text` | extab | note |
|---|---:|---:|---:|---:|---|
| base | **1,038** | 620 | 71,210 | 577 | best raw |
| upgrade body V(2),P(10),V(10),D + `setVisible(true)` | 1,187 | **497** | 71,359 | 581 | faithful upgrade body; body now instruction-identical to orig (modulo a 2 B `ldr` hoist) |
| `setVisible(true)` only (P,V,V,D kept) | 1,040 | 516 | 71,212 | 577 | faithful arg; +2 raw |
| free-credits V,P,D order (H1) | 1,072 | 613 | 71,244 | 577 | faithful order; +34 raw |
| V2 + free-credits V,P,D | 1,221 | 490 | 71,393 | 581 | |
| `setVisible(true)` + free-credits V,P,D | 1,074 | 509 | 71,246 | 577 | |
| two sequential `if`s, faithful bodies (V1) | 6,881 | 847 | 77,053 | 19,007 | size 9,700 but literal pool +4 / extab −12 B |
| ship body P,V,D order | 6,657 | 578 | — | 1,484 | **size 9,704** (not exact) |
| `rate > 0` -> `rate != 0` | 1,317 | 762 | 71,489 | 620 | |
| y-expression `this->field_0x114` before `field_0x260` | 1,040 | 622 | 71,212 | 577 | |
| x-expression `hintOffsetX` before `buttonInsetX` | 1,040 | 622 | 71,212 | 577 | |
| both x and y reorders | 1,042 | 624 | 71,214 | 577 | |
| `const float rate` / `const int borderX` | 1,038 | 620 | 71,210 | 577 | byte-identical |
| hoist `int bgX` before first DrawImage2D | 7,548 | 815 | — | 28,325 | **size 9,704** |
| block-scope `Layout *layout = Globals::layout;` | 7,548 | 815 | — | 28,325 | **size 9,704** |

`__builtin_expect` additions on the loop continue, `isShip()`, `isItem()`,
`item == selectedItem`, `isBluePrint()` and the `rate > 0` test all change
the function size (9,528–9,696) and are unusable.

## Why the faithful orders lose

Adopting the original statement order does **not** flip the `this`/`layout`
tie, so the register roles in the surrounding blocks stay wrong; the
reorder then only shifts addresses and adds raw diff.  Conversely the
committed base uses the *unfaithful* orders (`draw` before `setVisible`,
`setPosition(10)` before `setVisible(2)`) precisely because those are the
size-exact shapes that keep the whole function at 9,700 B and the LSDA
rows at their current lengths.

The one structural prize left is the `ldrd r5,r4,[sp,#132]` early reload in
the original's ship body (both `this` and `&layout` are hoisted above the
`cbz` because the upgrade check sits *after* the guard).  Producing it
requires the two-sequential-`if` shape, which is size 9,696 and shifts the
literal pool by +4, blowing `.text` to 77,053 and `.ARM.extab` to 19,007
bytes of diff — a large net loss.

## Conclusion

The base remains the unique size-exact raw minimum.  The irreducible
residual is the greedy queue order of the two post-`getCompletionRate`
reloads; every source shape that changes it also changes the function size
or the LSDA row layout.  Next angle would be a clang 7.0.2 build with
asserts/`-debug-only=regalloc` to read the actual `RS_*` stage and `Prio`
of `%3331`/`%3356`.

## Scratch

`/home/debian/GalaxyOnFire2/scratch/w13664/` — `base.cpp` (pristine),
`measure.py` (section diffs), `region.py`, `clusters.py`, `buckets.py`,
`aligndiff.py`, `run.py` + `batch1..10.py` (variant harness),
`allpass.err` (80 MB `-print-after-all` dump for the single function),
`base.so`/`h1.so`, `rag.cpp`/`armri.td`/`vrm.cpp`/`lre.cpp` (LLVM 7.0
sources for the queue analysis), `check.log`.
