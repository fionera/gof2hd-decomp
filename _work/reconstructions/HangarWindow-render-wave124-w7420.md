# `HangarWindow::render` — wave 124 (w7420): RA tie-break sweep, no gain

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`), original
`0x1490d8`, size `0x25e4` = 9700 B. Worktree `/home/debian/GalaxyOnFire2/wt/w7420`,
base master `6f8ee2320` (wave-123 final; `render` 94.057631 %).

This pass: **94.057631 -> 94.057631** (raw difflib over the reconciled
instruction stream). No source change landed; the deliverable is this
measurement record.

## Result

| metric | before (HEAD 6f8ee2320) | landed |
|---|---:|---:|
| `render` fuzzy (raw difflib) | 94.057631 | **94.057631** |
| `render` size | 9700 | 9700 |
| normalized insns | 3626 / 3627 | 3626 / 3627 |
| diff ops / matched | 142 / 3411 | 142 / 3411 |
| `.ARM.extab` record | 948 B (exact) | 948 B (exact) |
| `.ARM.exidx` | byte-identical | byte-identical |
| byte_exact / linked_exact | 4506 / 4506 | 4506 / 4506 |
| size-exact / data / imports | 4524, 843, 207 | same |
| lint / drift | CLEAN / 0 | CLEAN / 0 |

`check`: **RATCHET PASS** — `verify: byte 4506 linked 4506 stubs 0 missing 0
extra 0 avg 99.94`; `data: exact 843 of 843`; `sodiff: imports 207`; lint
`CLEAN ✓`; drift `0 field(s)`. `exidx_table.py --diff` reports no differing
extab record (total delta +0).

## Residual map (re-derived this wave)

Everything before `0x1496da` is instruction-identical. 142 diff ops, all
register-allocation / block-placement ties:

| region | diff ops | what |
|---|---:|---|
| `0x1496da`-`0x149840` | ~25 | blueprint branch: orig `ldrd sl,r9,[sp,#132]` (`sl`=&Globals::layout, `r9`=this) vs ours `ldrd r9,sl`. Every subsequent use of the pair is swapped; the whole branch is instruction-identical modulo the r9/sl rename. |
| `0x149ada`-`0x149b74` | ~12 | ship branch: orig `ldrd r5,r4` + `cbz r0,<tab==1 test>`; ours `ldrd r6,r5` + `cmp r0,#0; beq.w <join>`. The `tab==0` edge is jump-threaded past the `tab==1 && upgradeMode` test in ours (wide branch, +4 B) but not in orig (narrow `cbz`, and orig has an extra `ldrd` reload at `0x149b3c` that ours lacks, net 0 B). |
| `0x14a8ec`-`0x14abf2` | ~60 | buy-credits: orig `str.w r9,[sp,#132]` (only `&this->buttons` spilled; `&Globals::layout` stays in `sl` from `0x14a85e`); ours `strd sl,r9,[sp,#128]` (layout spilled to `sp+128`), then `add.w sl,sp,#312` (String temp in `sl`) vs orig `add r6,sp,#312` (String temp in `r6`). |
| `0x14ad2c`-`0x14ae90` | ~40 | free-credits: orig `mov r1,sl; mov sl,r0` (width -> `sl`, rowHeight -> `r9`, layout read via `r1`); ours `str r0,[sp,#116]; ldr.w r0,[sl]; ldr.w sl,[r0,#44]` (width spilled, rowHeight -> `sl`). Plus the `visible` loop-tail shape: orig predicated `ldrb/subs/it ne/movne/clz/lsrs/orr` at `0x14aeb4`; ours branchy `cmp/bne/ldrb/clz/lsrs` at `0x14ae80`. |
| tail | ~5 | cleanup landing pads and the `tbb` jump-table entries shifted by the above (`.short 0x00a6` vs `0x00a7`). |

## Root cause (confirmed)

The blueprint `ldrd` is a genuine Greedy tie-break, not a source-order effect:

* The two reload vregs are created by the spiller at the end of bb.288 in the
  order Canvas (`%3281`->`r8`), this (`%3337`->`r10`), layout (`%3360`->`r9`).
* The post-RA `ARM load/store optimization pass` forms the `ldrd` from the two
  adjacent stack slots, so the *operand order is fixed by stack offset*
  (`[sp,#132]` = layout, `[sp,#136]` = this) -- it is not the MIR order.
* Only the physical-register assignment differs: ours gives this->`r10`,
  layout->`r9`; the original gives this->`r9`, layout->`r10`.
* `this` has 12 uses in the bb.289-291 live range, layout has 4, yet layout
  (the higher-numbered, lower-weight vreg) ends up with the lower register in
  ours. Everywhere else in the function (e.g. ship branch, `%188`/`%189`,
  `%785`/`%786`, `%818`/`%819`) the lower vreg takes the lower register, so
  the blueprint pair is the one place the allocation order inverts.

The buy-credits and free-credits halves are the same phenomenon one level
down: `&Globals::layout` is spilled in ours but kept in `sl` in the original,
which frees `sl` for the `sp+312` String temp and pushes the width spill onto
the free-credits block.

## Variants measured this wave (all on the wave-123 base)

Base = 94.057631, 9700 B, 3626/3627, unless noted.

### Blueprint branch (`ldrd` tie)

| variant | pct | size |
|---|---:|---:|
| landed | **94.057631** | 9700 |
| x-expr `this`-first (`textX + 2 + this->hintOffsetX + Globals::layout->buttonInsetX`) | 92.914254 | 9700 |
| x-expr `this`-first, `hintOffsetX + textX + 2 + ...` | 92.914254 | 9700 |
| grouped `(textX + 2) + (buttonInsetX + hintOffsetX)` | 94.057631 | 9700 |
| y-expr `1 + y + rowHeight/2` | 94.057631 | 9700 |
| `(*this).progressBarBgImage` | 94.057631 | 9700 |
| condition flipped `0.0f < rate` | 94.057631 | 9700 |
| condition inverted (`if (rate <= 0) {} else {...}`) | 94.030057 | 9700 |
| `float rate` before the `if` | 88.395198 | 9692 |
| `float rate` before `name =` | 88.395198 | 9692 |
| `Layout &lay = *Globals::layout;` (unused) | 85.887541 | 9708 |
| `Layout *const lay = Globals::layout;` (unused) | 85.887541 | 9708 |
| `PaintCanvas &cv = *Globals::Canvas;` (unused) | 85.887541 | 9708 |
| `int hintX = this->hintOffsetX;` local | 87.856651 | 9704 |
| `int pw = this->progressBarWidth;` local | 85.887541 | 9708 |
| `int bg = this->progressBarBgImage;` local | 85.887541 | 9708 |
| `int inset = Globals::layout->buttonInsetX;` local | 85.887541 | 9708 |
| `int rh = Globals::layout->field_0x70_rowHeight;` local | 85.887541 | 9708 |
| `Layout *lay` + all `Globals::layout->` -> `lay->` in branch | 87.258687 | 9704 |
| `Layout &lay` + all -> `lay.` in branch | 87.258687 | 9704 |
| `String progress` split into `+` statements | 72.004417 | 9688 |
| `String progress` parenthesised `(a+b)+(c+d)` | 90.404529 | 9676 |
| `String pct` temp + concat | 90.841379 | 9692 |
| `BluePrint *bp = item->bluePrint;` local | 57.012951 | 9636 |

None of the local forms keeps 9700 B: binding a `Layout*`/`Layout&`/`Canvas&`
forces an extra GOT load (+8 B) because the pointer can be re-stored by the
calls in between, and `int hintX` costs +4 B.

### Ship branch (`cbz` vs `cmp/beq.w`)

| variant | pct | size |
|---|---:|---:|
| landed `if (tab != 0) {...} if (upgradeMode && tab == 1)` | **94.057631** | 9700 |
| `if (tab == 1 && this->upgradeMode)` | 94.002482 | 9700 |
| `if (tab > 0)` | 94.057631 | 9700 |
| `if (this->upgradeMode) { if (tab == 1) {...} }` | 94.057631 | 9700 |
| `if (tab == 1) { if (this->upgradeMode) {...} }` | 94.002482 | 9700 |
| upgrade block moved *inside* `if (tab != 0)` | 94.057631 | 9700 |
| `volatile int *padX` moved after the `tab != 0` block | 86.698828 | 9700 |

The `tab==0` edge is threaded to the join in every source shape tried; the
original's narrow `cbz` to the `tab==1` test is not reachable from the current
expression ordering (clang folds `tab==0` through `tab==1` in all forms).

### Free-credits (`textY`, width/rowHeight)

| variant | pct | size |
|---|---:|---:|
| landed `volatile int textY` | **94.057631** | 9700 |
| non-volatile `int textY` | 75.374673 | 9672 |
| `const int textY` | 75.374673 | 9672 |
| `unsigned int textY` | 75.374673 | 9672 |
| `volatile unsigned int textY` | 94.057631 | 9700 |
| `volatile const int textY` | 94.057631 | 9700 |
| `volatile int textY = ... + 0` | 94.057631 | 9700 |
| `textY` computed before `SetColor` | 93.737931 | 9700 |
| `volatile float textYf` + `int textY` | 91.637967 | 9720 |
| `volatile int textYv` + `int textY = textYv` | 91.608103 | 9708 |
| `volatile float` + `volatile int` | 91.412819 | 9708 |
| `const int width` | 94.057631 | 9700 |
| `const int width/textX/height` | 94.057631 | 9700 |
| `const int &width = button->getWidth()` | 94.057631 | 9700 |
| `const int &textX = ...` | 94.057631 | 9700 |
| `PaintCanvas *const canvas` | 94.057631 | 9700 |
| `height` declared before `width` | 73.798706 | 9668 |
| `int rowHeight` local before `textX` | 94.057631 | 9700 |
| `textX` split into `textX = width+baseX; textX += rowHeight;` | 94.057631 | 9700 |

Dropping `volatile` lets clang keep the *float* `y + height*0.2f` in `s18`
across `getText()` and re-issue `vcvt.s32.f32` at each use (9672 B, 604 diff
ops). The `volatile` is what forces the single conversion + 376-byte frame.

### Buy-credits / globals

| variant | pct | size |
|---|---:|---:|
| landed | **94.057631** | 9700 |
| `int count` before `HangarAppData *data` | 94.057631 | 9700 |
| `String price` before `String description` | 93.947332 | 9700 |
| `const int tab` | 94.057631 | 9700 |
| `Array<ListItem *> *const items` | 94.057631 | 9700 |
| `const int textX` (list) | 94.057631 | 9700 |
| `int i` instead of `unsigned int i` | 94.030057 | 9700 |
| `Layout *layout = Globals::layout;` at function top (all uses) | 39.020998 | 9584 |
| `Layout &layout = *Globals::layout;` at function top (all uses) | 39.020998 | 9584 |

### `visible` (free-credits loop tail)

| variant | pct | size |
|---|---:|---:|
| landed single expression | **94.057631** | 9700 |
| `visible = visible && (i != 4 || !options[76]);` | 94.057631 | 9700 |
| two statements + `if (i == 4 && options[76]) visible = false;` | 90.691270 | 9720 |

## Conclusion

The landed wave-123 source is still a strong local optimum at 94.057631 %.
The remaining 142 diff ops are four independent Greedy/CFG tie-breaks
(blueprint `ldrd` pair order, ship `cbz` threading, buy-credits layout spill,
free-credits width/rowHeight roles) plus their knock-on into the cleanup
landing pads and `tbb` entries. Every source shape that makes one block more
faithful (the `Layout*`/`Layout&` locals, the non-volatile `textY`, the
`tab == 1 && upgradeMode` order, the explicit `if (tab == 0) {} else`) either
changes the function size or re-allocates the whole function and loses more
than it gains.

The one structural observation worth carrying forward: the blueprint pair is
the only place in the function where the lower-numbered vreg (`this`) takes
the *higher* register, i.e. the allocator processed `layout` first there. A
future pass that can perturb the spill-weight ordering of the two bb.288
reload vregs (without adding a live `Layout*` local) is the most promising
route.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
cmake --build cmake-build-match --target drift -j6      # 0 fields
cmake --build cmake-build-match --target lint -j6       # CLEAN
python3 tools/verify/verify.py --show _ZN12HangarWindow6renderEv
```

Scratch (not committed): `/dev/shm/w7420/{score.py,run.py,batch.py,dump.py,
diffaddr2.py,w1..w21.py}`, `/dev/shm/w7420/{orig,ours}.asm`, and the
`-mllvm -print-machineinstrs` dump `/dev/shm/w7420/mi.txt` (the Greedy and
post-RA MIR sections used above are at lines 500749 and 510294).
