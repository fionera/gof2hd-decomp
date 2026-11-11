# `Layout::Layout` C1/C2 — wave 178 / worker w12211

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Baseline: branch `wave178-w12211`, base master `1b48f990f` (includes w12110's
unit-72/77 swap): raw **5,604**, aligned **2,334**, size **7,072/7,072**,
2,519 / 2,552 instructions.

**Landed: a size-exact raw win.** Tail-region store-order search:

| metric | before | after |
|---|---:|---:|
| raw same-address bytes | **5,604** | **5,576** (-28) |
| aligned (difflib) | 2,334 | 2,410 (+76) |
| size | 7,072/7,072 | **7,072/7,072** |
| instructions | 2,519/2,552 | 2,519/2,552 |

The raw metric is the worker objective; aligned is reported honestly and moved
the other way.  `reload()` is untouched.

## 1. Winning statement moves

Seven moves inside the constructor, all pure reorderings of existing
field assignments (no value, type or control-flow change).  In source order:

1. `field_0x2b8` before `field_0x110_strip58` (was the reverse).
2. `field_0x188`, `field_0x180`, then the `field_0x190` if/else chain
   (was: `field_0x190` chain, then `0x188`, `0x180`).
3. `field_0x118`, `field_0x12c`, `field_0x11c` (was `0x11c`, `0x12c`, `0x118`).
4. `field_0x188`, `field_0x194`, `field_0x1a4` (was `0x194`, `0x1a4`, `0x188`).
5. `field_0x150`, `field_0x148`, `field_0x154`, `field_0x13c`
   (was `0x13c`, `0x150`, `0x154`, `0x148`).
6. `field_0xc8`, `field_0xc4`, `field_0xcc`, `ringNearMidThreshold`
   (was `0xcc`, `ringNearMidThreshold`, `0xc4`, `0xc8`).
7. `ringNearMidThreshold`, `field_0xc4`, `field_0xcc`
   (was `0xcc`, `ringNearMidThreshold`, `0xc4`).

## 2. Method (and why it is not w12210's search)

`Layout::Layout` is a 204-store straight-line initializer.  A unit parser
splits the ctor body into top-level statements / if-else chains; a window
search permutes `W` consecutive units (all `W!` orders) and recompiles.
Candidates are scored with a compile-only proxy: the `Layout.cpp.obj`
`.text._ZN6LayoutC2Ev` bytes are compared to the original's linked bytes with
the 23 `R_ARM_GOT_PREL` pool words masked.  The proxy is exact for ranking:
base `raw_est` 5,539 vs linked raw 5,604 (constant +65), and every measured
candidate tracked the linked raw one-for-one.  Winners were re-linked and
measured with `tools/fn_diff.py`; only size-exact winners were kept.

Search history (each row is a landed, re-linked win):

| window | region | move | raw |
|---|---|---|---:|
| W=2 | 38..228 | swap 82/83 | 5,603 |
| W=3 | 38..228 | `(115,116,114)` | 5,595 |
| W=3 | 38..228 | `(86,85,84)` | 5,590 |
| W=3 | 38..228 | `(114,112,113)` | 5,586 |
| W=4 | 38..228 | `(97,99,98,96)` | 5,583 |
| W=4 | 38..130 | `(62,60,59,61)` | 5,580 |
| W=3 | 38..228 | `(59,60,58)` | **5,576** |

Further W=3 (whole tail), W=4 (130..228) and W=5 (50..70) rounds found no
further size-exact raw improvement; the landed order is a local optimum for
these neighbourhoods.

## 3. Store order is already exact — only `rewardBoxY2`'s value is not

The `layout_ctor/emu.py` Thumb emulator was run on the original and on our
build for all six flag configurations (HD, n9, retina, iPadLarge, iPad,
default).  **The store sequence is identical in every configuration: 202/202
stores, same offsets, same order.**  The reorder above therefore cannot change
semantics, and indeed the traces are unchanged after the win.

One pre-existing value difference remains, and it is a real source bug:

- `0x3f8` (`rewardBoxY2`), non-n9 configs:
  original `186 / 127 / 72 / 55` (retina / iPadLarge / iPad / default),
  ours `34 / 17 / 17 / 17`.
- Cause: the source computes `rewardBoxY2 = (rewardBoxY + 17) << retina`
  **before** assigning `rewardBoxY`, so it reads the uninitialised member.
  The original source assigns `rewardBoxY` first; its `y2` is
  `(y+17)<<retina` (e.g. retina `(76+17)<<1 = 186`).
- Fixing the order (`y` then `y2`, or a local `ry`) gives the correct values
  and a size-exact build, but **loses raw**:
  `raw 5,576 -> 5,691` (local `ry` form, size 7,072) or `5,690` (store-y-
  first form, size 7,080).  Not landed under the size-exact-raw-win rule;
  recorded here for a future pass that can also fix the surrounding
  scheduling.

## 4. `volatile unsigned char *rd` block

Original (`d1870..d18ba`): one shared `retina|n9` computation, `0x2d4` and
`0x2d8` both branchless, merged into one `strd r4,r6,[r0,#724]`.
Ours: `0x2d8` branchless, then a real-branch `if/else` for `0x2d4` through
`volatile unsigned char *rd`, plus a second `ldrb` of `retinaDisplay`.
Replacing the if/else with the shared-condition ternary
(`Globals::iPadHD ? 112 : ((retina||n9) ? (iPad?160:100) : (iPad?100:60))`)
matches the original's shape but shrinks the body 16 B (size 7,056) and
**raw rises to 5,710**; keeping `volatile` in the ternary gives size 7,072
but raw 5,710.  Dropping `volatile` alone gives size 7,064 / raw 5,727.  The
current if/else form is the raw-best local choice on this base.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12211
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5576 aligned=2410 size=7072/7072 insn=2519/2552
cmake --build cmake-build-match --target check -j2
# RATCHET PASS: byte_exact 4507, linked_exact 4507, size-exact 4524/4524,
# data_exact 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w12211/`: `Layout.orig.cpp`
(pristine ctor), `Layout.win.cpp` (landed ctor), `reapply.py` (replays the
seven moves), `search.py` (unit parser), `winsearch.py` (window permutation),
`rawest.py` (compile-only proxy), `emu.py` / `emu_ours.py` / `cmptrace.py`
(store-trace comparison).

Gate at exit: byte_exact **4507/4524**, linked_exact **4507/4524**,
size-exact **4524/4524**, stub_zero_size **0**, missing **0**, wrong_type
**0**, extra **0**, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift **0**, parity **5370/5370**, `.rel.plt` byte-identical.
