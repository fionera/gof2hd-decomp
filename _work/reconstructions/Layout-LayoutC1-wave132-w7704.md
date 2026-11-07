# `Layout::Layout` C1/C2 — wave 132 / worker w7704
## EH/cleanup scope, member-init order and literal-pool order: the ctor has **no** unwind table at all

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr `0xd1028`,
size **7072 B**, source `src/game/ui/Layout.cpp`.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
Baseline = wave-131 master `70e451ffe` (48.943802311678 % unrounded, 7072 B).

## Result — no source win (note only)

| metric | before (HEAD) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| unrounded `difflib` ratio | 48.943802311678 % | **48.943802311678 %** |
| `byte_exact` / `linked_exact` | 4506 / 4506 | **4506 / 4506** |
| `size_exact` / `data_exact` | 4524 / 843 | 4524 / 843 |
| imports / lint / drift | 207 / CLEAN / 0 | 207 / CLEAN / 0 |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
No `src/` change landed; the worktree is clean apart from this note.

## 1. NEW: the `.ARM.exidx` entry is `EXIDX_CANTUNWIND` — there is no `.ARM.extab` record

Decoding the exidx table (prel31 word 0 relative to its own address, word 1 = inline
compact-entry / prel31 to `.ARM.extab` / `1` = CANTUNWIND):

| binary | covering exidx entry | function | word 1 |
|---|---|---|---|
| original | `0x1a0300` | `0xd1028` | **`0x00000001` (CANTUNWIND)** |
| ours | `0x1a0300` | `0xd1028` | **`0x00000001` (CANTUNWIND)** |

So `Layout::Layout` has **no unwind table entry and no landing pads at all** in the original.
Consequences (all now *proven*, not hypotheses):

* No `try`/`catch` and no cleanup-only scope is possible in the original ctor body — any of
  them would require an extab record (and would grow the size-exact `.ARM.extab` section).
* The `new TouchButton(...)` expressions in the ctor must be in a context where clang needs no
  `operator delete` landing pad. That is already true of the current source (same CANTUNWIND
  entry), so the button-construction tail is not a source lever.
* "EH/cleanup scope" is therefore **ruled out** for this function: there is nothing to decode
  and nothing to match. The 1,420 differing `.ARM.extab` bytes belong to other functions.

Reproduce with the parser in §5.

## 2. NEW: member-initializer-list order — measured, no 7072-B hit

A member-initializer list is emitted in **declaration order**, not list order. The first 27
`Layout` members are declared in ascending offset order (`0x4,0x8,0xc,...,0x6c`), which is
exactly the iOS/true source order for that prefix (`layout_ctor/ios_source_order.md`), while the
current hill order is `0x4,0x14,0x1c,0x10,0xc,0x18,0x8,0x24,...`. Converting the first *K*
fields to a member-init list therefore forces the true order on that prefix, a genuinely new
ordering lever (not another statement move).

| K (prefix in declaration order) | size | ratio % |
|---:|---:|---:|
| **0 (HEAD)** | **7072** | **48.943802** |
| 2 | 7024 | 40.9 |
| 4 | 7060 | 41.1 |
| 6 | 7016 | 41.0 |
| 8 | 7032 | 45.8 |
| 10 | 7032 | 45.8 |
| 12 | 7048 | 45.0 |
| 14 | 7040 | 44.8 |
| 16 | 7040 | 44.8 |
| 20 | 6960 | 38.3 |
| 24 | 7048 | 44.1 |
| 27 | 7048 | 44.5 |

Every K shrinks the body by 12–112 B (the init-list form also loses the body's block-placement
freedom) and lowers the fuzzy score. No K keeps 7072 B.

## 3. NEW: literal-pool word order — the original's pool confirms the iOS order, not the hill order

The original end pool `0xd2854..0xd2bc8` (221 words) starts

```
0.325f  0.65f  100.0f  50.0f  <GOT> <GOT> <GOT> <GOT> 200.0f 100.0f ...
3ea66666 3f266666 42c80000 42480000 ...
```

i.e. the four constants of `field_0x90` (0.325/0.65) and `field_0x9c` (100/50) are the **first
four pool words**. In HEAD the pool starts with GOT offsets and the `0.325/0.65` pair is not in
the end pool at all — it sits in a `.text` constant island at `0xd1928` (loaded with
`subw r1, pc, #88`). This is the same pool-order evidence wf101 used: the iOS-order build's
integer run matches the original's first 44 words exactly, while the hill order's does not.

So the literal-pool word order is a **second independent confirmation** that the true source
order is the iOS order, and that the current hill order is a fuzzy-metric local optimum, not
the original.

## 4. NEW: re-running the iOS order on the *current* expressions is still ~216 B short

The current `Layout.cpp` body was mechanically reordered to the 204-offset iOS order
(`layout_ctor/ios_source_order.md`), keeping every current expression verbatim and inserting the
`t_field_0x2c8` / `v` / `rd` declarations before their uses:

| body | size | ratio % |
|---|---:|---:|
| original | 7072 | 100 |
| HEAD hill order | **7072** | **48.9** |
| iOS order, current expressions | **6856** | **36.2** |

That is 216 B short (wf101 measured 6936 B on the wave-101 expressions; the current expressions
are 80 B leaner). The pop is at `0xd277c` vs the original's `0xd2800` — the deficit is code
(–132 B) plus cold-tail/pool placement, exactly the wf101/w131 diagnosis. Landing the true
order therefore still needs +216 B of expression/type detail, not a reorder.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7704
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6     # RATCHET PASS, byte 4506
```

exidx decode (self-contained, ~30 lines): read `.ARM.exidx`/`.ARM.extab` section headers from
the ELF section table, then for each 8-byte exidx entry compute
`prel31(word0, entry_addr)`; the entry covering `0xd1028` is at `0x1a0300` with `word1 == 1`
(`EXIDX_CANTUNWIND`). `prel31` is the 31-bit signed offset with bit 30 as the sign bit.

Scratch harnesses (not committed, `/dev/shm/w7704/`): `extab.py` (exidx/extab decoder),
`score.py` (exact single-function unrounded scorer built on `tools/verify`'s
`Resolver`/`disassemble`/`reconcile`), `mkios.py` (iOS-order rewriter), `tryinit.py`
(member-init-list sweep), `Layout.base.cpp`, `Layout.ios.cpp`, `Layout.initlist.cpp`,
`orig.dis`, `ours.dis`, `ios.dis`.
