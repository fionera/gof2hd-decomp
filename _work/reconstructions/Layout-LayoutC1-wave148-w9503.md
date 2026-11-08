# `Layout::Layout` C1/C2 — wave 148 / worker w9503
## The full ascending (iOS-order) tail puts the first `0x164` store at exactly
## `0xdf4`, but the function collapses to 6,804 B: the missing 268 B is the
## original's 242-word literal pool (ours 194) — i.e. the duplicated device
## arms, not a source-order problem.  No 7,072 B variant beats the baseline.

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Baseline = wave-148 HEAD `fece77a64`: raw same-address diff **5,743**,
aligned **2,213**, size 7,072.  Gate on entry and exit: byte_exact 4507/4524,
linked_exact 4507/4524, size 4524/4524, data 843/843, imports 207, lint CLEAN,
`RATCHET PASS`.

## 1. Method

The whole constructor body was parsed into 205 named statements
(`/dev/shm/w9503/gen2.py` + `Layout.base.cpp`) so any permutation can be
regenerated and rebuilt (`eval.sh` = `link_so` + `fdiff.py` + `align_diff.py`,
~0.8 s per variant).  Store offsets were extracted with `stores2.py`, which
resolves `vst1`/`stmia` bases from the preceding `add.w rX, r0, #imm` (the
plain `str`-only table misses every SLP group — that is why the earlier notes
under-counted the `0x140..0x14c` group).

## 2. Original store-offset table (tail, function offsets)

`ORIG` (`stores2.py orig.dis`, `0xce0..0x1100`):

```
332@cec 312@cfa 316@cfe 336@d0a 340@d16 348@d1c 340@d2e 340@d48 340@d78
344@d90 348@dbe 352@dea 356@df4 356@e0c 356@e2a 360@e36 380@e4a 364@e52
384@e72 388@e7a 392@e80 396@e88 404@e94 416@e98 364@ea2 400@eba 384@ec4
408@eca 416@ece 364@ee4 364@ef6 368@f0c 372@f22 376@f36 372@f46 376@f56
376@f72 384@f7c 388@f86 388@f9a 388@fae 392@fb8 396@fc8 396@ff8 396@1012
404@101c 408@1032 416@1036 420@1058 420@1064 424@1072 428@1086 480@1100
```

The `0x13c..0x148` SLP group is `vst1 316 @ cfe`; `0x14c` is the scalar
`str.w 332 @ cec` immediately before it; `0x138` is `vstr 312 @ cfa`.

## 3. Baseline store-offset table (same window)

`BASE` (HEAD, 7,072 / raw 5,743 / aligned 2,213):

```
288@ce0 276@cf0 572@cf6 264@d04 296@d08 268@d0c 304@d16 172@d36 304@d46
304@d80 312@d98 332@da8 336@dc4 340@dda 328@dec 340@df2 340@e02 328@e20
344@e3c 344@e5a 700@e7a 516@e7e 352@ec4 356@ed0 356@ee8 356@ef8 360@f16
380@f3a 364@f46 364@f62 404@f68 380@f6e 364@fac 380@fb8 404@fca 388@fd8
400@fe2 420@fe8 420@ff8 388@ffe 400@1006 400@100e 420@101c 384@1028
392@1030 420@104a 388@1050 344@106a 420@1078 388@1088 400@1096 384@10a8
392@10ac 396@10bc 412@10c0 396@10d8 412@10de 424@10e8 396@10fe
```

Differences vs ORIG: the `0x13c..0x148` group does not exist (0x148 is a
separate `str.w 328` pair at `dec`/`e20`), `0x140` is not stored in this
region at all, and the first `0x164` store is at `ed0` (ORIG `df4`, +0xdc).

## 4. Full ascending tail — the `0x164`-at-`0xdf4` variant

Reordering statements `0x13c..0x1e8` into ascending offset order (moving
`0x140`/`0x144` up from their late positions at source lines 271/283 and
`eventBannerScale`/`0x1e4`/`0x1e8` after `0x1dc`):

```
ASC (6,804 B / raw 5,653 / aligned 2,679)
172@cfa 304@d2a 304@d48 312@d86 320@d8a 328@d90 336@dac 340@dcc 344@dd0
348@dd4 352@de8 356@df4 356@e0c 340@e1e 348@e26 356@e36 360@e54 380@e78
364@e84 364@ea2 380@ea8 340@ed8 364@f16 380@f22 384@f34 388@f42 392@f52
400@f58 388@f66 392@f7c 400@f82 404@f8e 388@fb0 392@fc0 396@fe4 400@fee
400@1010 404@1022 408@1032 416@1036 408@1050 416@1054 408@1078 416@107c
420@1098 424@10a4 428@10bc 480@10e0 448@10e8 432@10f0
```

**The first `0x164` store lands at exactly `0xdf4`** (`356@df4`), and
`320@d8a` is the `0x140..0x14c` SLP group — the two structural targets of the
mission.  But the function is **6,804 B**, 268 B short, so it cannot land
(size-exact is a hard gate: a 268 B shrink shifts every later function).

Where the 268 B goes: instruction counts up to the end of code (`0x182a`) are
essentially identical (ORIG 2,116 vs ASC 2,120 instructions), but the literal
pool is **242 words (968 B) in the original vs 194 words (776 B) in ours**.
The original spends the budget on duplicated device-arm constants
(`vld1.64 {d16-d17}, [r1]` constant vectors per arm); the ascending source
lets clang share/if-convert them and the pool collapses.

## 5. `0x148` joining the `0x140..0x14c` group — measured

Moving only `0x140`, `0x144`, `0x148` up next to `0x13c` so the source order is
`13c, 140, 144, 148, 14c, 150, ...` (variant **A**):

| variant | size | raw | aligned | `0x164` @ |
|---|---:|---:|---:|---:|
| HEAD baseline | **7,072** | **5,743** | **2,213** | `ed0` |
| A: `0x148` group only | 7,008 | 5,897 | 2,241 | `ed8` |
| B: A + `0x180..0x1ac` ascending | 7,016 | 5,879 | 2,274 | `ed0` |
| B + explicit `0x2c4` if/else | 7,064 | 5,980 | 2,315 | — |
| C: ascending `0x13c..0x1ac` only | 6,992 | 5,795 | 2,298 | `e72` |
| ASC: full ascending tail | 6,804 | **5,653** | 2,679 | **`df4`** |

The group is worth **64 B** (7,072 → 7,008), not the ~104 B estimated in
w9408, and it does **not** move `0x164` to `0xdf4` (only `ed0`→`ed8`).  It
cannot be landed alone: it is size-negative and worsens both metrics.

## 6. Adjacent-swap search at exact 7,072 B (confirms the local optimum)

All 50 adjacent swaps of statements 96..145 were rebuilt and measured.  The
ones that keep size 7,072:

```
swap 136 -> raw 5,739 aligned 2,236      swap 123 -> raw 5,742 aligned 2,239
swap 100 -> raw 5,764 aligned 2,215      swap 119 -> raw 5,787 aligned 2,223
swap 117 -> raw 5,772 aligned 2,307      swap 131/132 -> raw 5,746/5,747 aligned 2,213
```
No single adjacent swap improves both raw and aligned; the current HEAD
(5,743 / 2,213) is a genuine local optimum for this move set.

## 7. Conclusion / open questions

* The iOS-order tail is **structurally right** (first `0x164` store exactly at
  `0xdf4`, `0x140..0x14c` SLP group, `0x138`/`0x13c` pair) but is **268 B of
  literal pool short**.  The blocker is the duplicated device-arm constants
  (242 vs 194 pool words), not statement order.
* Next lever: reproduce the device-major arms so clang re-materialises the
  per-device constant vectors.  The original's out-of-line cold blocks are
  `d2802` (iPad/default `0x130..0x138`), `d2828`/`d2836` (iPad/retina
  `0x2c4`), `d2844` (iPad `0x74`).  Explicit source if/else for `0x2c4` alone
  adds 40 B (6,804 → 6,844) — a plausible direction, but 6 such blocks would
  be needed and each one tested so far also raises raw/aligned.
* `0x148` group is worth 64 B and must be paired with a +64 B tail
  reconstruction to stay at 7,072; no such +64 B source change was found in
  this wave.

## 8. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9503
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9503/fdiff.py _ZN6LayoutC1Ev        # 7072/7072, diff 5743
python3 /dev/shm/w9503/align_diff.py _ZN6LayoutC1Ev   # 2213
cmake --build cmake-build-match --target check -j6    # RATCHET PASS
```

Scratch (`/dev/shm/w9503/`): `Layout.base.cpp`, `gen2.py` (statement-order
generator), `eval.sh`, `fdiff.py`, `align_diff.py`, `stores2.py` (SLP-aware
store table), `order_asc.txt`, `order_A.txt`, `order_AB.txt`, `order_AB2.txt`,
`order_C.txt`, `order_decl.txt`, `search.py`, `orig.dis`, `asc.dis`, `A.dis`,
`AB.dis`, `C.dis`, `orig.stores2`, `ours.stores2`, `asc.stores2`.

## 9. Addendum — explicit device if/else on the ascending tail

Converting the `0x2c4`, `0x74`, `0x5c`, `0x60`, `0x64`, `0x68` ternaries to
explicit `if/else if` chains on top of the full ascending tail gives
**6,828 B / raw 5,779 / aligned 2,845** — only +24 B for six diamonds, so the
literal-pool budget is not recoverable this way either.  No source change was
landed; `src/game/ui/Layout.cpp` is unchanged and the gate is green at
raw **5,743** / aligned **2,213** / size **7,072**.
