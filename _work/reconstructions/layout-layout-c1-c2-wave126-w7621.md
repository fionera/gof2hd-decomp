# `Layout::Layout` C1/C2 — wave 126 / worker w7621

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7072 B (0x1ba0)**, end `0xd2bc8`.
Source `src/game/ui/Layout.cpp` (constructor only).
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(sha256 `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`).
Baseline = wave-125 final master `4bbe88776` (48.943802 %, 7072 B).

Assignment focus: independent approach #1 — reconstruct the exact original
statement/expression order from Ghidra, then test source shapes that change
SLP vectorisation and live ranges.  The body is size-exact; the residual is
SLP vectorisation, literal-pool ordering and Greedy register allocation.
The original saves one fewer callee-saved register.

## Result — no source win (note only)

| metric | before (HEAD) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| unrounded `difflib` ratio | 48.943802 % | **48.943802 %** |
| instructions | 2529 / 2489 | 2529 / 2489 |
| `byte_exact` / `linked_exact` | 4506 / 4506 | **4506 / 4506** |
| `size_exact` / `data_exact` | 4524 / 843 | 4524 / 843 |
| imports / lint / drift | 207 / CLEAN / 0 | 207 / CLEAN / 0 |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`
(`verify: byte 4506 linked 4506 stubs 0 missing 0 extra 0 avg 99.94`,
`data 843/843`, `sodiff extra 0 missing 0`, `imports 207`).
No `src/` change landed; the worktree is clean apart from this note.

## 1. Per-region instruction match (new measurement)

The whole-function 48.9 % hides a very uneven match.  Splitting both bodies at
the original block boundaries (HD / n9 / else / common1 / common2) and running
`difflib` over the normalised instruction sequences gives:

| region | original insns | ours | ratio |
|---|---:|---:|---:|
| HD branch (`0x1a..0x240` / `0x1a..0x24c`) | 164 | 169 | **51.1 %** |
| n9 branch (`0x244..0x2d8` / `0x250..0x314`) | 46 | 68 | **52.6 %** |
| else branch (`0x548..0x838` / `0x540..0x84a`) | 278 | 292 | **60.0 %** |
| common1 (`0x838..0x11ba` / `0x84a..0x1276`) | 785 | 814 | **32.6 %** |
| common2 (`0x11ba..0x1828` / `0x1276..0x18a8`) | 528 | 518 | **32.1 %** |

So the HD/n9/else prefixes already match ~51–60 %; essentially all of the
remaining loss is in **common1 + common2** (the per-field non-HD / common
chains).  Any future pass should target those two blocks, not the HD prefix.

## 2. Field order: common2 is already exact, common1 is not

Recovering the per-field store order from the original with a recursive
capstone CFG traversal (skipping the inline literal-pool islands) gives:

* **common2** (first store of each field):
  `0x1e8 0x1e4 0x1fc 0x1ec 0x234 0x27c 0x3e0 0x3e8 0x3f4 0x204 0x214
  0x224 0x23c 0x24c 0x25c 0x26c 0x228 0x22c 0x230 0x238 0x210 0x21c
  0x264 0x254 0x260 0x274 0x280 0x3e4 0x3ec 0x3f8 0x3f0 0x2bc 0x3fc
  0x200 0x1f0 0x1f8 0x284 0x285 0x286 0x288 0x290 0x2f4 0x2fc 0x304
  0x2f8 0x308 0x30c 0x314 0x318 0x31c 0x320 0x130`.
  Our linked body emits **exactly this order** (verified by the same
  traversal on `cmake-build-match/libgof2hdaa.so`).  The common-field
  statement order is therefore already correct; the 32 % is pure
  register-allocation / if-conversion / round-trip difference.

* **common1** (first store of each field):
  `0x2d0 0x2d4 0x2d8 0x7c 0x80 0x84 0x88 0x8c 0x90 0x94 0x98 0x9c 0xac
  0xa8 0xa0 0xd0 0xd8 0xe0 0xe8 0xfc 0x104 0x2b4 0xb0 0xc0 0xec 0x294
  0x2a4 0x118 0x108 0x128 0x140 0x138 0xd4 0x298 0xdc 0xe4 0x2a0 0x130
  0x14c 0x13c 0x150 0x154 0x15c 0x158 0x160 0x164 0x168 0x17c 0x16c
  0x180 0x184 0x188 0x18c 0x1a0 0x190 0x198 0x170 0x174 0x178 0x194
  0x1a4 0x1a8 0x1ac 0x1e0 0x1c0 0x1b0 0x1d0`.
  Ours is `0x2c8 0x2d8 0x2d4 0x7c 0x84 0x8c 0x90 0x98 0x9c 0xac 0xa8
  0xa0 0xec 0xf0 0xf4 0xf8 0xfc 0x100 0x2b4 0xb0 0xc0 0x294 0x2a4 0xdc
  0x118 0x108 0x128 0x23c 0x138 0x14c …` — i.e. the same *set* but a
  different order (notably `0x2d0` first in the original vs `0x2c8` first
  in ours, and the `0xd0/0xd8/0xe0/0xe8` quartet early in the original).

## 3. Variants measured (real `link_so` builds, exact unrounded ratio)

All reorders were done by a statement-level rewriter that keeps `if/else`
chains and local-declaration/use pairs together (`/dev/shm/w7621/reorder*.py`,
`move.py`).

| variant | size | ratio % |
|---|---:|---:|
| **baseline (HEAD)** | **7072** | **48.943802** |
| all statements sorted by common1 order, then common2 | 6528 | 8.551666 |
| common1-order reorder preserving non-common1 slots | 6624 | 24.730737 |
| config-dependent statements reordered by HD-branch order | 6896 | 15.727604 |
| `eventBannerScale` (`0x1e0`) moved to the end | 6996 | 37.201844 |
| `0x2c8` (`t_field_0x2c8` unit) moved after `0x2d8` | 7104 | 47.355865 |

Every attempt to impose the recovered original order **lowers** the fuzzy
ratio and usually changes the 7072-B size.  The current field-major statement
order is a strong local optimum of the fuzzy metric even though it is not the
original order.

## 4. Callee-saved register difference (the "one fewer" hint)

| | original | ours |
|---|---|---|
| first push | `{r4,r5,r6,r7,lr}` | `{r4,r5,r6,r7,lr}` |
| second spill | `{r6,r7,r8,r9,sl,fp}` (6) | `{r5,r6,r7,r8,r9,sl,fp}` (7) |
| epilogue | `ldmia {r2,r3,r8,r9,sl,fp}` + `pop {r4,r5,r6,r7,pc}` | `ldmia {r1,r2,r3,r8,r9,sl,fp}` + `pop {r4,r5,r6,r7,pc}` |

Both functions use r4–r11; the second spill is the high-CSR save with padding
words chosen for stack alignment.  The original pads with 2 words, ours with 3
(48-byte vs 44-byte frame).  The extra word is **not** a real extra live
value: r5 is used in both bodies for the same `field_0x74` HD store
(`movw r5,#369; strd r6,r5,[r0,#0x74]`).  It is a downstream allocation /
frame-size artefact, so it cannot be fixed by deleting a variable; it should
fall out once common1/common2 match.

## 5. Round-trip map (unchanged from wave 125)

Original `(int)(float)` sites: `0x60 0x68 0x6c 0x70 0x2c8 0x2d0 0x8c 0x9c
0xa8 0x138 0x150 0x158 0x15c 0x160 0x168 0x170 0x1a8 0x1e4 0x3e0 0x3f8
0x1f4 0x320`.
Ours: `0x20 0x60 0x68 0x6c 0x70 0x2c8 0x2d0 0x8c 0x9c 0xa8 0x10c 0x148
0x150 0x160 0x168 0x1a8 0x204 0x3e0 0x3e8 0x1f4 0x320`.
Original-only: `0x138 0x158 0x15c 0x170 0x1e4 0x3f8`;
ours-only: `0x20 0x10c 0x148 0x204 0x3e8`.
The original common2 begins with a round-trip on `0x1e4`
(`vcvt.f32.s32; vcvt.s32.f32; vstr s0,[r0,#484]`) that ours does not emit;
wave-125 measured `0x1e4 = (int)(float)(…)` at 7016 B / 42.28 %, so the cast
alone is not the answer.

## 6. Remaining root cause

The original source is field-major (our field-major body reproduces the same
hoisted HD/n9/else block structure), and the *common2* statement order is
already identical.  The residual is entirely in common1/common2 codegen:
if-conversion (`it eq` / `moveq`) vs our `it ne` forms, the `(int)(float)`
round-trip set, and Greedy register allocation.  Recovering the original
common1 order from the binary is unreliable because the `-Oz` block layout is
driven by branch targets and SLP grouping, not source order; every recovered
order tested this wave scored below the current body.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7621
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
python3 /dev/shm/w7621/score.py                          # 7072 / 48.943802
python3 /dev/shm/w7621/region.py                         # per-region ratios
```

Scratch harnesses (not committed, `/dev/shm/w7621/`): `score.py` (exact
unrounded score), `region.py` (per-block difflib), `cfg.py` (recursive
capstone traversal that skips inline literal pools), `order2.py` (first-store
per field), `reorder.py` / `reorder2.py` (whole-body / slot-preserving
statement reorders), `move.py` (single-statement move), `parse.py`
(statement → field-offset map using `fieldmap.json`).
