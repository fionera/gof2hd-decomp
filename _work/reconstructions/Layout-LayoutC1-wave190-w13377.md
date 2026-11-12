# `Layout::Layout` C1/C2 — wave 190 / worker w13377

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave190-w13377`, base master `1904beaf7` (raw **5,256**, aligned
**2,726**, whole-`.text` diff **73,505**).

**Result: no size-exact raw win found.  `src/` left at the base.**

| metric | base |
|---|---:|
| raw same-address bytes | **5,256** |
| aligned (difflib) | **2,726** |
| size | **7,072/7,072** |
| instructions | 2,520/2,552 |
| whole-`.text` diff | 73,505 |
| `.ARM.extab` diff | 610 (size/base exact `0x1a3b78`, 156,312 B) |

Gate after the (empty) pass: `RATCHET PASS` — byte_exact 4507/4524,
linked_exact 4507/4524, size-exact 4524/4524, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact 843/843, imports 207, lint CLEAN, drift 0,
parity 5370/5370.

## 1. Independent semantic oracle: the constructor is value-perfect

The wave-188/189 value checks were re-done from scratch with a different
mechanism: the original ctor is executed under `qemu-arm -cpu cortex-a15`, and
the result is compared against a host-compiled copy of the current C++ ctor for
all 32 combinations of the five `Globals` flags.

Harness (scratch, not tracked): the original `.so` is mapped at its ELF vaddrs,
the function blob `[0xd1028, 0xd1028+7072)` is copied to `0x1000000`, the
`.data` segment is copied to `0x113bb78` (the blob-relative offset of
`0x20cba0`), the `R_ARM_GLOB_DAT` slots for the five flags are patched to the
exported `Globals` addresses, and the flags are set in a page at `0x218000`.
The host side is a minimal `Layout` struct generated from `Layout.h` (pointers
widened to 4-byte `unsigned`) with the ctor body copied verbatim; the struct is
zeroed before the ctor, exactly like the emulator's destination.

Result: **0 differing words across all 0x414 bytes × 32 flag combinations.**
This independently confirms that the landed `0x190` shape
(`HD -6 / iPadLarge -8 / n9 -4 / retina ? (iPad ? -4 : -2) : (iPad ? -4 : -1)`)
is value-identical to the original, including the iPad-over-retina priority.
No semantic fix is available; the whole residual is codegen.

## 2. Codegen searches on the current order

All searches used the current `src/game/ui/Layout.cpp` statement order (the
wave-189 w13317 order).  Two proxies were used:

* full-TU proxy: compile `Layout.cpp` with the checked-in command and compare
  `.text._ZN6LayoutC2Ev` to the original (base proxy raw **5,241**; linked
  raw **5,256**);
* minimal-TU proxy: a standalone TU with only the `Layout` member list and the
  ctor body (base proxy raw **5,251**; ~0.02 s/compile).  It reproduces the
  full-TU body byte-for-byte up to the literal pool, but its *size* prediction
  is unreliable for far moves (e.g. swapping units 143/207 predicts 7,072 but
  links at 7,080).

Searches run (all on the current order):

* exhaustive single-unit moves, units 0..30 × positions 0..30 (930):
  best proxy 5,242, no improvement;
* exhaustive 2-opt, units 0..30 (465): best proxy 5,242, no improvement;
* exhaustive 2-opt, all 227 units (25,651, minimal proxy): best
  `a_143_207` proxy 5,245, `a_146_207` 5,248, `a_8_13` 5,249.  Real build:
  `143↔207` → size 7,080 raw 5,901; `146↔207` → size 7,080 raw 5,616;
  `8↔13` → size 7,072 raw 5,257.  None is a win;
* exhaustive K=5, windows 80..135 (6,664, minimal proxy): best
  `117 (4,2,0,1,3)` proxy 5,249.  Real build: size 7,072, raw **5,338**
  (worse); the other two 5,250 candidates are size 6,948/7,064.

The current order is therefore a local optimum for single moves and 2-opt, and
the minimal-proxy K=5 improvements do not survive the real TU.

## 3. Structural shapes tried (all size-fatal or raw-worse)

* split the initial `iPadHD/n9/else` block into two ternary assignments
  (`field_0x4`, `footerTextInset`): proxy 5,250, real **5,264** (worse);
* `float f150 = ...; field_0x150 = (int)f150;`: real 5,256 (neutral);
* `float f160 = ...; field_0x160 = (int)f160;`: real 5,256 (neutral);
* `field_0x158`/`field_0x170`/`field_0x1e4`/`field_0x1a8` as
  `(int)(float)(...)`: size changes or raw ≥ 5,242 proxy;
* rewrite `0x30c..0x318` as one `iPadLarge/iPad/n9/retina/else` chain:
  size 7,092 (fatal);
* exact original `!iPad ? (retina ? -2 : -1) : -4` nested form of `0x190`:
  size 6,960 (fatal);
* move `0x158` away from the `0x150..0x15c` SLP group: size 7,064 (fatal);
* reorder the first block to the original's store-group order
  (`0x24..0x30, 0x34..0x40, ...`): size 7,040 (fatal); partial variants
  5,299 / 5,727 / 5,827.

The original is more SLP/`strd`/`stmia` heavy (62 `strd`, 81 `vst1`) than the
current body; the residual is a register-allocation/scheduling difference, not
a wrong field order or value.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13377
ulimit -v 12000000
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5256 aligned=2726 size=7072/7072 insn=2520/2552
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13377/`
(`emu2.py`, `gen_host.py`, `compare.py`, `gen_harness.py`, `harness.s`,
`harness.ld`, `score2.py`, `sweep_moves.py`, `sweep_swap.py`, `sweep_min.py`,
`sweep_k5b.py`, `applyperm.py`, `tryshape.py`, `tryfloat.py`).
