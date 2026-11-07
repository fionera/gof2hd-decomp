# `Layout::Layout` C1/C2 — wave 131 / worker w7686

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7072 B (0x1ba0)**, source `src/game/ui/Layout.cpp`.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(sha256 `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`).
Baseline = wave-130 final `7c6933c2d` (7072 B / 48.943802311678 % unrounded).

## Result — no landable source win (note + new oracle artifact)

| metric | before (HEAD) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| unrounded `difflib` ratio | 48.943802311678 % | **48.943802311678 %** |
| `byte_exact` / `linked_exact` | 4506 / 4506 | **4506 / 4506** |
| `size_exact` / `data_exact` | 4524 / 843 | 4524 / 843 |
| imports / lint / drift | 207 / CLEAN / 0 | 207 / CLEAN / 0 |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
No `src/` change landed; the worktree is clean apart from this note and the
new decompile artifact.

## 1. NEW artifact: the *original* binary's Ghidra decompile

The previous waves used the KiritoJPK Unity-remake oracle (a different,
clang-3.8 `-O2` build).  This pass finally dumped the **shipped 2.0.16
binary's own** `Layout::Layout` from the local Ghidra project
(`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so_ghidra`,
program `android_2.0.16_libgof2hdaa.so`, image base 0x10000 → Ghidra address
`0xe1028`).  The full 1739-line decompile is committed alongside this note as
`_work/reconstructions/layout-orig-ghidra-decompile.txt`.

It is the first source-shaped view of the *exact* target.  Key facts:

* The function is one outer `if (!iPadHD) { ... } else { ... }` with the
  non-HD arm split into `if (!n9) { ... } else { ... }`, then a shared
  `common1`, then a `common2` reached by all paths — exactly the wave-121/122
  skeleton, now confirmed from the real binary rather than the oracle.
* The HD arm is a long run of paired `*(undefined8 *)(this+N)` stores in the
  order `14 1c 4 c 24 2c 34 3c 44 4c 54 5c 64 6c 2c8 2d0 7c 84 74 78 2c0
  2c4 2d8 ac 8c 90 94 98 9c a0 a4 a8 c0 c8 b0 b8 ec f4 294 29c 2a4 2ac
  108 110 118 120 d0 d4 d8 dc e0 e4 e8 fc 100 104 2b4 2b8 128 130 140 148
  150 158 160 168 170 178 180 188 190 198 1a0 1a8 1b0 1b8 1c0 1c8 1d0 1d8
  138 13c 1e0`.  This is the oracle's ascending source order with the first
  four pairs (`14/18, 1c/20, 4/8, c/10`) moved to the front by the scheduler.
* The common1 tree is a chain of `if (cVar62 == 0)` (iPadLarge),
  `if (cVar43 == 0)` (iPad), `if (cVar59 == 0)` (retina) / `if (uVar61 == 0)`
  (n9) blocks with **shared `bVar42`/`bVar9`/`bVar10` state carried across
  fields** and `goto LAB_...` joins.  That shared state is what produces the
  duplicated stores the wave-130 store census counted (`0x154` 5 vs 4,
  `0x16c` 5 vs 4, `0x178` 5 vs 4, `0x184` 6 vs 5, `0x18c` 6 vs 4, ...).
* The `0x1b0..0x1dc` block is a genuine SLP `vbsl` select chain: the
  decompile shows `VectorBitwiseSelect` over `DAT_000e2a30..0x2a88` pairs,
  then `*(longlong *)(this+0x1b0)` etc.  The constants are the
  n9/default/retina value pairs, not a single scalar expression.

## 2. Why the wave-130 "landed" body is actually 7024 B

The wave-130 w7663 commit message claims `7072 B / 48.864089 %`, but the
committed file `git show dfb3bfd04:src/game/ui/Layout.cpp` builds to
**7024 B / 49.050570 %** on the current tree (verified this pass with a
clean `link_so`).  That is why the next commit reverted it ("7024 B broke
size gate").  The note's variant table mixed measurements from different
base revisions; the real w7663 body is 48 B short.

## 3. Variants measured this pass (real `link_so` builds, exact unrounded)

All variants were generated from the pristine wave-130 source and scored with
the `verify.py` `Resolver`+`reconcile`+`difflib` pipeline
(`/dev/shm/w7686/score.py`).

| variant | size | ratio % |
|---|---:|---:|
| **HEAD baseline** | **7072** | **48.943802** |
| w7663 committed body (all five trees) | 7024 | **49.050570** |
| w7663 + `0x7c/0x80/0x84/0x88` if/else tree | 7040 | 47.833899 |
| w7663 + `0x7c` tree + `0x140` duplicated in all five `0x174` arms | **7072** | 45.849170 |
| exact `0x16c..0x1ac` tree transcribed from the original decompile (on w7663) | 6852 | 46.051303 |
| exact `0x16c..0x1ac` tree (on the 7024 `0x174` variant) | 6880 | 44.080808 |
| simplified `0x184..0x1ac` branch tree | 6816 | 42.952748 |
| `0x16c..0x1ac` reordered ascending (baseline) | 7080 | 45.899682 |
| `0x16c..0x1ac` reordered ascending (w7663 body) | 7048 | 48.325359 |
| first group `0x4..0x78` reordered ascending | 7068 | 43.139605 |

The exact decompile tree **shrinks** the body (6852–6880 B) rather than
growing it: clang 7 `-Oz` merges the transcribed `if/else` arms more
aggressively than the original's clang did, so the duplicated stores
disappear.  Reordering the block to the original's ascending source order
also loses the fuzzy alignment (the current scrambled order is a strong
hill-climbed local optimum of the *compiled* stream).

## 4. Best size-exact body found

`w7663 + 0x7c tree + 0x140 duplicated in all five 0x174 arms` is exactly
**7072 B** and gate-green, but scores **45.849170 %** — a 3.1-point
regression, so it was not landed.  Its value is structural: it proves the
`0x7c..0x88` tree (+16 B) and duplicated `0x140` stores (+16 B) are real
code-size anchors.  The missing 48 B of the 49.05 % w7663 body are
duplicated common1 stores, not padding.

## 5. Open questions for the next wave

* The original's common1 shared state (`bVar42`/`bVar9`/`bVar10`) must be
  modelled as a single source-level variable that is assigned in one field's
  arm and read in the next field's condition.  A direct transcription of the
  decompile's `goto` graph compiles to 6852 B because clang re-merges it;
  the source likely has a `bool`/`int` local threaded through the block.
* The `0x1b0..0x1dc` `vbsl` chain needs the exact `VectorBitwiseSelect`
  source shape (two constant vectors selected by `retina`), not the current
  scalar ternaries.  The decompile's `DAT_000e2a30..0x2a88` words are the
  four 32-bit pairs; reading them from the original binary gives the exact
  per-arm values.
* The `0x3e8` reward-box store count (original 5, HEAD 1) remains the
  clearest config-major tail; the decompile shows it in the
  `cVar5 == 0` / `cVar43 == 0` / `iPad` / `iPadLarge` arms around
  `0xe24b2..0xe2548`.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7686
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
python3 /dev/shm/w7686/score.py                          # 7072 / 48.943802
```

Ghidra decompile reproduction (local Ghidra 12.1.2 at
`/tmp/ghidra_12.1.2_PUBLIC`, pyghidra venv `/tmp/ghvenv`):

```sh
/tmp/ghvenv/bin/python /home/debian/GalaxyOnFire2/.tmp/ghidra_decomp2.py \
  /home/debian/GalaxyOnFire2/_work/bins \
  android_2.0.16_libgof2hdaa.so_ghidra / android_2.0.16_libgof2hdaa.so 0xe1028
```

Scratch harnesses (not committed, `/dev/shm/w7686/`): `score.py` (exact
single-function scorer), `chunks.py` / `reorder.py` (constructor chunk
parser), `convert.py` (decompile → C++ store converter), `Layout.w7663.cpp`,
`Layout.7016.cpp`, `Layout.w7663.7c.140.cpp`, `orig_decomp.txt`.
