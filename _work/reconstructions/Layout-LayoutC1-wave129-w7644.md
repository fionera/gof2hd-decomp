# `Layout::Layout` C1/C2 — wave 129 / worker w7644
## SLP store-group census + register-liveness trace: the block structure is already right; the residual is RA/scheduling

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr `0xd1028`,
size **7072 B**, source `src/game/ui/Layout.cpp`.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
Baseline = wave-128 master `fee528cbe` (48.943802311678 % unrounded).

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

## 1. NEW: the four-block structure is identical — the source *shape* is already correct

The earlier notes compared per-block difflib ratios but never checked whether the
original and HEAD agree on the **block decomposition itself**.  They do, exactly:

| block | original | HEAD | test |
|---|---|---|---|
| prologue (iPadHD load/test) | `d1028..d1042`, 9 insns, 1 `cmp` | same, 9 insns, 1 `cmp` | identical |
| HD arm | `d1042..d1268` (164 insns) | `d1042..d1274` (169) | **`cmp` count 0 in both** |
| n9 test | `d126c..d127e`, 6 insns | `d1278..d128a`, 6 insns | identical |
| n9 arm | `d127e..d12fc` (346) | `d128a..d1568` (335) | **`cmp` count 0 in both** |
| retina/iPad/default arm | `d1570..d21e2` (1063) | `d1568..d229e` (1106) | 98 vs 108 `cmp` |
| tail (2d0..3ac) | `d21e2..d2800` (493) | `d229e..d28b8` (509) | 47 vs 46 `cmp` |

Both binaries therefore have **exactly one** `Globals::iPadHD` test and **exactly one**
`Globals::n9` test for the whole ctor.  That is only possible if the source is the
current field-major ternary form (`f = iPadHD ? A : (n9 ? B : (retina ? C : ...))`):
clang's block placement chains the HD arms into one fall-through block and deletes the
redundant re-tests on both arms.  A config-major `if/else if` source would need a
different n9-arm field set (see §2), so the "config-major source" hypothesis of
`layout-literal-pool-241-words-wave119-w6974.md` §7 is **disproved as a shape change**:
the current shape already produces the observed single-test structure.

The n9 arm covers only `0x4..0x78` + `0x2c0..0x2cc` in *both* binaries (HEAD also sinks
`0x1e0` and `0x2d0` there); it then joins the common arm at `d1860` / `d1860`.  Same
split, same join point.

## 2. NEW: SLP group census per block — the original vectorises 11 more groups

`vst1.32` groups / scalar `str*` per block (linear disassembly, base register tracked
through `add rX, r0, #imm`):

| block | orig vst1 | HEAD vst1 | orig str* | HEAD str* |
|---|---:|---:|---:|---:|
| HD | **27** | 26 | **13** | 23 |
| n9 | 7 | 7 | 1 | 4 |
| common | **27** | 20 | **103** | 141 |
| tail | **20** | 17 | **66** | 79 |
| **total** | **81** | **70** | **183** | **247** |

The original vectorises 11 more 4-int store windows and emits 64 fewer scalar stores.
The extra windows are exactly the ones wave-119 listed (0xb0, 0xc0, 0xec, 0x140, 0x150,
0x160, 0x170, 0x1d0, 0x2a0, 0x108 …), i.e. **16-byte-aligned windows**; HEAD's windows
are shifted by one field (`0xb4`, `0xc4`, `0x148`, `0x158`, `0x168`) because the
corresponding `0xb0`/`0xc0`/`0x144`/`0x14c` stores are scheduled outside the run.

New store-order extraction (base-register-tracked, both arms) shows the **HD arm store
order is identical for the first nine groups** in both binaries
(`0x14,0x4,0x24,0x34,0x44,0x54,0x64,0x2c8,0x7c`) and then diverges exactly at
`0x74/0x78` — the point where HEAD sinks `0x1e0` and `0x2c0` into the HD arm while the
original keeps them in the common arm (`0x1e0` is the *last* HD store in the original,
`0x2c0` is an early `strd` there).  In the common arm the two store orders agree again
from `0x30` onward but differ over `0x4..0x2c` and `0x68..0x84`.

## 3. NEW: the extra `[sp,#8]` spill is the `retinaDisplay || n9` value, not a callee-saved register

Wave 126 established the prologue difference is a folded 12-byte vs 8-byte local frame.
This pass traced *what* is spilled:

```
HEAD  d18b6: orrs.w r3, fp, r8        ; r3 = n9 | retinaDisplay
      d198c: str   r3, [sp, #8]       ; spilled
      d211e/d2182/d218c: ldr.w r9,[sp,#8]
      d2178: cmp.w r9, #0             ; field_0x1a8  (n9||retina -> 24/32)
      d229e: orrs.w r1, r9, sl        ; field_0x1e8  (n9||retina||HD -> 1/0)
```

The original computes the same value into **r5** (`d1898: orrs.w r5, r8, fp`) and keeps
it in that callee-saved register across the same ~800-instruction range
(`d1e06 orrs.w r1,r4,r9`, `d27da orrs.w r1,r1,r8`).  HEAD's r5 is occupied at that point
by `subw r5, pc, #36` (the `0.325f/0.65f` island), so Greedy-RA spills r3.  This is a
pure allocation outcome: the two live ranges are disjoint in the original and overlap in
HEAD only because HEAD materialises `0.325f/0.65f` in a `.text` island instead of the end
pool.  Fixing the island placement (wave 119's `field_0x90` observation) is therefore the
*precondition* for losing the spill, not an independent defect.

## 4. NEW variants measured (all real `link_so` builds, exact unrounded ratio)

Scorer: `/dev/shm/w7644/score.py` (verify.py `Resolver`+`reconcile`+`difflib`, reproduces
the full-link number bit-for-bit).

| variant | size | ratio % |
|---|---:|---:|
| **HEAD baseline** | **7072** | **48.943802** |
| 0x13c..0x17c fully address-ordered (moves 0x140/0x144/0x14c/0x150 into place) | 7004 | 38.068523 |
| 0x94 moved before 0x98 | 7040 | 46.049210 |
| 0xa4 moved before 0x9c (0x98..0xa4 contiguous) | 7048 | 45.570630 |
| both of the above | 7040 | 42.814311 |
| 0x204 moved before 0x208 | 7032 | 45.636509 |
| eventBannerScale (0x1e0) moved next to 0x1dc | 6960 | 43.980738 |
| field_0x2d8 moved next to 0x1a4 (shorten the OR live range) | 7020 | 46.120000 |
| field_0x1e8 moved next to 0x2d8 | 6680 | 21.721478 |
| field_0x1e8 + 0x1ac moved next to 0x2d8 | 6700 | 15.119852 |

Every statement-order change that restores an original SLP window shrinks the body and
loses more `difflib` alignment than the window gains.  The 7072-B wall remains hard:
the original's extra 11 SLP groups are paid for by its +52 B of outlined `iPad` tails
(80 B vs 28 B) and its +124 B literal pool, and no measured source permutation
reproduces both sides of that trade at once.

## 5. Proven vs hypothesis

**Proven (measured):**

* The original and HEAD have the same four-block decomposition and the same single
  `iPadHD` / single `n9` test for the whole ctor — the field-major ternary source shape
  is correct; a config-major rewrite is not needed.
* The original emits **81** `vst1.32` groups vs HEAD **70** (HD 27/26, common 27/20,
  tail 20/17) and **183** scalar stores vs HEAD **247**.
* The HD-arm store order matches for the first nine SLP groups and diverges at
  `0x74/0x78`; HEAD sinks `0x1e0`/`0x2c0` into the HD arm, the original does not.
* The extra `[sp,#8]` spill is `n9|retinaDisplay`; the original keeps it in r5.  HEAD's
  r5 is taken by the `0.325f/0.65f` `.text` island, so the island placement (wave 119)
  is the root cause of the spill.

**Hypothesis (not proven):**

* The original statement order differs from HEAD only in the `0x4..0x2c`,
  `0x68..0x84` and `0x74/0x2c0/0x1e0` regions.  Reproducing it needs the exact original
  order *and* the `field_0x90` island in the end pool; every partial reorder measured here
  shrinks the body below 7072 B.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7644
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6     # RATCHET PASS, byte 4506

OBJ=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OBJ -d --start-address=0xd1028 --stop-address=0xd2bc8 \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so > /dev/shm/w7644/orig.dis
$OBJ -d --start-address=0xd1028 --stop-address=0xd2bc8 \
  cmake-build-match/libgof2hdaa.so > /dev/shm/w7644/ours.dis
python3 /dev/shm/w7644/score.py        # 7072 / 48.943802311678
```

Scratch harnesses (not committed, `/dev/shm/w7644/`): `score.py` (exact single-function
scorer), `mkvar.py` (statement reorder generator), `try.sh` (rebuild+score), `orig.dis`,
`ours.dis`, `Layout.base.cpp`.
