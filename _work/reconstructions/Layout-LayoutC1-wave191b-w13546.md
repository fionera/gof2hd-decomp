# `Layout::Layout` C1/C2 — wave 191b / worker w13546

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, linked symbol size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave191b-w13546`, base master `6e55ef9d6` (w13519 field_0x7c win).

**Result: one size-exact raw win landed.**

| | before | after |
|---|---:|---:|
| raw | 5,241 | **5,234** |
| aligned | 2,726 | **2,709** |
| size | 7,072/7,072 | 7,072/7,072 |
| insn | 2,521/2,552 | **2,519**/2,552 |

`check` = RATCHET PASS: byte_exact 4507, linked_exact 4507, data 843/843,
imports 207, lint CLEAN, drift 0, `.rel.plt` byte-identical.

## The win

The ctor's opening block assigns `field_0x4` and `footerTextInset` in each
`iPadHD / n9 / else` arm.  Re-stating `footerTextInset` as its own ternary
chain immediately after the block (the same value, so semantically identical)
makes the default (retina) arm's store schedule match the original:

```cpp
    if (Globals::iPadHD) {
        this->field_0x4 = 15 * 1.40625f;
        this->footerTextInset = 22 * 1.40625f;
    } else if (Globals::n9) {
        this->field_0x4 = 22;
        this->footerTextInset = 37;
    } else {
        this->field_0x4 = (Globals::retinaDisplay ? 30 : 15);
        this->footerTextInset = (Globals::retinaDisplay ? 50 : 22);
    }
    this->footerTextInset = Globals::iPadHD ? 22 * 1.40625f : (Globals::n9 ? 37 : (Globals::retinaDisplay ? 50 : 22));
```

The redundant store is CSE'd away, but the value is now materialised in the
ternary's form, which lets the backend merge the default-arm stores:

* before: `strd r6,lr,[r0,#12]` (0xc/0x10) + `strd sl,r1,[r0,#20]` (0x14/0x18)
  + `str r5,[r0,#28]` (0x1c) — five separate stores.
* after: `stmia.w r2,{r1,r6,sl}` (0xc/0x10/0x14) + `strd r5,lr,[r0,#24]`
  (0x18/0x1c) — exactly the original's `stmia r1!,{r2,r5,r6}` at `d15e2` and
  `strd r3,lr,[r0,#24]` at `d15ea`.

Net: two instructions fewer, same 7,072 B (the freed bytes are absorbed by the
literal pool), and 7 more bytes matching at the same address.

## How it was found

`scratch/w13546/` harness: `measure.py` writes a candidate ctor, runs
`link_so -j2` + `tools/fn_diff.py`, and restores the base; `parse.py` splits the
ctor into 204 top-level statement units (if/else chains kept whole);
`iosorder.py`/`ioswin.py` reorder units by the iOS source order
(`layout_ctor/ios_source_order.md`); `adj.py`/`rnd.py` sweep adjacent swaps and
random moves.  ~1 s per measured variant.

## Negative results this session (do not redo)

All measured with `link_so -j2` + `fn_diff`; baseline 5,241 / 2,726 / 7,072.

* **iOS source order** (204 units sorted by `ios_source_order.md`): 5,965 /
  6,920 (−152).  Windowed iOS sorts (0x80..0x2b8, 0x108..0x2b8, 0x108..0x14c,
  0x154..0x2b8, 0x204..0x280, 0x248..0x320, 0x2bc..0x320, 0x284..0x3fc, …):
  every one shrinks the code; only `0x284..0x320` is size-exact (7,072) but
  raw 5,949.  The iOS order is *not* a raw win on the Android build.
* **Original Android default-path store order** (`layout_ctor/order.py`, now
  runnable again — see below): reordering to it was already measured by
  w13513 as 6,465 / 6,352.
* **SLP group reorders** (make 4 adjacent fields contiguous so the backend
  forms a `vld1/vst1` bundle): 0xb0..0xbc (−40), 0xc0..0xcc (−160),
  0xec..0xf8 (−72), 0x294..0x2a0 (+28), 0x2a4..0x2b0 (−24), 0x108..0x114
  (size-exact, raw 5,271), 0x118..0x124 (−80), 0x128..0x12c (size-exact,
  raw 5,247), 0x140..0x14c (−88); all nine together 5,980 / 6,952.  All 45
  single/pair combinations were measured: no size-exact raw win.
* **Exhaustive adjacent swaps of all 204 units** (203 builds): best 5,241
  (many are codegen-identical); no win.
* **400 random single moves/pairs**: only 10 size-exact, best 5,241.
* **Tail reorders**: 0x248..0x27c ascending (−128), 0x238.a-d + ascending
  (−196), 0x248..0x320 iOS (+28), 0x284..0x320 iOS (size-exact, raw 5,949).
* **`field_0x20_top` int-typed if/else chain** (the original stores ints at
  0x20/0x24 via one `strd`): 5,809 / 7,052; +0x294 group 7,084.  The float
  round-trip (`vmov/vcvt/vcvt/vstr`) is 20 B of the residual code-length
  excess, but removing it is size-fatal on its own.
* **`field_0x2d4` if/else → ternary**: 5,416 / 7,064 (the original's shared
  tail merges 0x2d4+0x2d8 into one `strd`; not reachable this way).
* **`field_0x2d8` moved next to 0x2d4** (w13513 variant D): 6,265 / 6,888 —
  the `strd` lands in the HD arm, not the shared tail.
* **`field_0x2c8`/`field_0x2d0` `else if` splits**: 2c8 codegen-identical;
  2d0 5,739 / 7,048.
* **`field_0x8c`/`field_0x90`/`field_0x9c` permutations**: only the base order
  is 5,241; (0x8c,0x90,0x9c) is size-exact at 5,335.
* **`rd` forms** (`const`, `*volatile`, `char*`, no local): neutral or
  size-fatal (`no_rd` 7,064, `rd_ptrvol` 6,896).
* **Remaining if/else → ternary**: 0x20c (+16), orbitInfoTextX (+16),
  orbitInfoStationY (−24), 0x218 (−24), 0x260 (size-exact, 5,847),
  rewardBoxY2 (−144).
* **`helpWindowX` moved before `helpWindowY`** (−156), `rewardBoxY2` before
  0x300 (−128), `0x94` after 0x304 (+20, raw 5,272); 0x94-move + 0x20 chain
  = 7,068 (4 short).
* **`field_0x2c0` moved next to 0x2c4/0x98/0x2cc/0x2d0**: all shrink;
  after 0x1d0 is codegen-identical.
* **Early-block groups** (0x34/0x44/0x54/0x64 etc.): all shrink except
  0x64/0x68/0x6c/0x70, which is codegen-identical.

## Tool repair

`_work/reconstructions/layout_ctor/emu.py` hard-coded
`/home/debian/GalaxyOnFire2/wt/w4e/tools/verify` (worktree no longer exists),
so `order.py`/`gen.py` were unusable.  It now resolves `tools/verify` relative
to the file.  `python3 order.py _ZN6LayoutC1Ev` prints the original's
default-path store order again.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13546
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5234 aligned=2709 size=7072/7072 insn=2519/2552
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13546/`: `measure.py`, `parse.py`,
`iosorder.py`, `ioswin.py`, `adj.py`, `rnd.py`, `v1.py`..`v5.py`, `combo20.py`,
`counts.py`, `sizediff.py`, `regionmatch.py`, `base.dis`, `win1.dis`.

## Win 2 — re-state `field_0x184` after itself

Duplicating the `field_0x184` ternary immediately after itself is CSE-neutral
but changes materialisation: the `n9/iPad` arm's separate `movs r1,#173` +
`b.n` pair folds into the shared 232/116 path (`d214c`), and the merged block
gains a `str.w r1,[r0,#388]`.  Net −1 raw / −2 insns.

* before: raw 5,234 / aligned 2,709 / insn 2,519
* after:  raw 5,233 / aligned 2,712 / insn 2,517 / size 7,072:7,072
* `.text` delta 71,259 → 71,251; `.ARM.extab` 576 (unchanged);
  `.rel.plt` byte-identical; `check` RATCHET PASS.
