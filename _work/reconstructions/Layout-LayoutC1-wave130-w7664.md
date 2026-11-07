# `Layout::Layout` C1/C2 — wave 130 / worker w7664
## SLP store-group census + spill-liveness trace, with a clean statement-chunk reorder harness

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, size **7072 B**, source `src/game/ui/Layout.cpp`.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
Baseline = wave-129 master `003a9623a` (48.943802311678 % unrounded).

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

## 1. SLP census re-confirmed on the current link

Linear disassembly over `0xd1028..0xd2bc8` (pool bytes included, so the
`vst1.32`/`vld1.64`/`vbsl` counts are exact):

| | original | HEAD |
|---|---:|---:|
| instructions in range | 2552 | 2513 |
| `vst1.32` | **81** | 70 |
| `vld1.64` | **86** | 77 |
| `vbsl` | **11** | 7 |

So the original has 11 more vector stores, 9 more vector loads and 4 more
vector selects.  The extra `vbsl` groups are the concrete form of the
"common block" SLP deficit: the original's non-HD path is a vectorised
select chain (`0x294`, `0x2a0`, `0xdc..0xe8`, `0x2b0`, `0x13c..0x148`,
`0x1c0..0x1d0`, `0x20c..0x218`), while HEAD emits scalar
`movs`/`moveq`/`str` for most of the same fields.

## 2. Spill/liveness trace re-confirmed

HEAD's prologue/epilogue save one extra local-stack word
(`stmdb sp!,{r5,r6,r7,r8,r9,sl,fp}` vs the original's
`{r6,r7,r8,r9,sl,fp}`; `ldmia` similarly one register wider).  The extra
slot is `[sp,#8]`, written at `d198c: str r3,[sp,#8]` and reloaded at
`d211e/d2182/d218c`; it holds `n9 | retinaDisplay`.  The original computes
the same value into **r5** (`d1898: orrs.w r5, r8, fp`) and keeps it in
that callee-saved register across the same range.

The reason HEAD spills is the `0.325f/0.65f` `.text` island: HEAD's
`field_0x90` code is

```
d1974: orr.w  r1, lr, r8
d1978: orrs.w r1, r1, fp
d197c: subw   r1, pc, #88      ; r1 = d1928 (island)
d1980: it eq
d1982: addeq  r1, #4           ; r1 = d192c (0.65f)
d1984: cmp.w  fp, #0
d1988: vldr   s2, [r1]
d198c: str    r3, [sp, #8]     ; <- the extra spill
```

while the original uses the end pool:

```
d1940: orr.w  r2, fp, r3
d1944: orrs.w r1, r2, r8
d1948: addw   r1, pc, #3848    ; r1 = d2854 (end pool, 0.325f)
d194c: it eq
d194e: addeq  r1, #4           ; r1 = d2858 (0.65f)
d1950: cmp.w  fp, #0
d1954: vldr   s2, [r1]
```

Both are 4-byte Thumb-2 address materialisations, so the pool-address
difference itself is masked by the verifier; only the one extra `str`
and the prologue/epilogue word are real score differences (≈2 of 2529
instructions).

## 3. NEW clean statement-chunk harness (the earlier sweeps were partly contaminated)

The previous waves generated variants by text substitution on
`src/game/ui/Layout.cpp` in a loop.  Because `gen.py` re-read the file at
import time, a variant built from a *previous variant* silently inherited
its edits.  This pass rebuilt the harness to parse the constructor once
into 205 top-level chunks (one per field, with `if/else` chains and the
`int t_field_0x2c8` / `int v` / `volatile rd` declarations kept as units),
map each chunk to its struct offset via the header, and emit any
permutation from the pristine source.  Every variant below is generated
from the pristine file and is a real `link_so` build scored with the
`verify.py` `Resolver`+`reconcile`+`difflib` pipeline (`/dev/shm/w7664/score.py`).

## 4. Variants measured this wave (all clean)

| variant | change | size | unrounded % |
|---|---|---:|---:|
| **HEAD baseline** | — | **7072** | **48.943802** |
| `cI` | `field_0x90 = (retina\|\|iPad\|\|n9) ? 0.325f : 0.65f` | 7072 | 48.943802 (byte-identical) |
| `cL` | `field_0x90 = (retina\|\|iPad) ? 0.325f : (n9 ? 0.325f : 0.65f)` | 7072 | 48.943802 (byte-identical) |
| `cJ` | `field_0x8c` HD arm written `45.0f` instead of `32.0f*1.40625f` | 7072 | 48.943802 (byte-identical) |
| `cK` | `field_0x9c` int constants (no `float` arms) | 7048 | 48.782435 |
| `cO` | `0xdc,0xe0,0xe4,0xe8` contiguous ascending | 7108 | 44.263597 |
| `cP` | `0x13c,0x140,0x144,0x148,0x14c` contiguous ascending | 7004 | 46.203165 |
| `cQ` | `0x1c0,0x1c4,0x1c8,0x1cc,0x1d0` contiguous ascending | 7056 | 47.685555 |
| `cR` | `0x20c,0x210,0x214,0x218` contiguous ascending | 7100 | 47.903001 |
| `vA` | sort `0x13c..0x1dc` (incl. `0x204/0x2bc/0x2c0`) ascending | 6904 | 40.964824 |
| `vB` | as `vA`, but `0x13c` after `0x1dc` (original HD order) | 6904 | 41.045226 |
| `vX` | `__builtin_expect(Globals::iPad,0)` on `field_0x60` final arm | 6984 | 43.492255 |
| `vY` | `__builtin_expect(Globals::iPad,0)` on `t_field_0x2c8` final arm | 7072 | 48.943802 (no-op) |
| `vZ` | `__builtin_expect(Globals::iPad,0)` on `field_0x2d0` final arm | 7072 | 48.943802 (no-op) |

## 5. What this proves

**Proven (measured):**

* The original has **81/86/11** `vst1.32`/`vld1.64`/`vbsl` vs HEAD **70/77/7**.
  The 4 missing `vbsl` are the common-block select chains.
* The extra `[sp,#8]` spill is `n9|retinaDisplay`; the original keeps it in
  r5, HEAD's r5 is taken by the `0.325f/0.65f` `.text` island.
* The `field_0x90` source form is irrelevant: both the explicit
  `(retina||iPad||n9)` OR and the `(retina||iPad)` split compile to
  **byte-identical** code to the current nested ternary.  The island
  placement is chosen by `ARMConstantIslandPass`, not by the source shape.
* The `field_0x8c` HD arm is already folded (`32.0f*1.40625f` == `45.0f`);
  writing `45.0f` is byte-identical.
* `__builtin_expect(Globals::iPad,0)` on the `t_field_0x2c8` and
  `field_0x2d0` final arms is a codegen no-op; on `field_0x60` it changes
  the body (6984 B) and loses score.
* Every clean grouping reorder that restores an original SLP window
  changes the body size (6904–7108 B) and loses `difflib` alignment.  The
  current field-major order is a strong local optimum at 7072 B.

**Hypothesis (not proven):**

* The original's extra `vbsl`/`vst1` groups are a consequence of the
  *non-HD* source being config-major (or at least having the retina/iPad
  select chains adjacent), while HEAD's hill-climbed field-major order
  scatters them.  Reproducing them needs a source that both groups the
  selects and keeps the total at 7072 B; every grouping measured here
  lands below (or above) that wall.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7664
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6     # RATCHET PASS, byte 4506

OBJ=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OBJ -d /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so > /dev/shm/w7664/orig_all.dis
$OBJ -d cmake-build-match/libgof2hdaa.so > /dev/shm/w7664/ours_all.dis
python3 /dev/shm/w7664/score.py     # 7072 / 48.943802311678
```

Scratch harnesses (not committed, `/dev/shm/w7664/`): `gen.py` (chunk
parser/emitter, 205 chunks + offset map), `parse3.py` (header offset map),
`score.py` (single-function exact scorer), `tryv.py` (build+score),
`mkvar.py` (reorder generator), `Layout.orig.cpp` (pristine source),
`orig_all.dis`, `ours_all.dis`.
