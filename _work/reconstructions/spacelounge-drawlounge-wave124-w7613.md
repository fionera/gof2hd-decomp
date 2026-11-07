# SpaceLounge::drawLounge — wave-124 MIR priority / hint re-check (w7613)

Status: research note (2026-11-07). Worktree `wt/w7613`, base `bf365a964`.
**No `src/` change landed**: the committed HEAD is still the best measured shape.
The tree is clean apart from this note.

Target: `_ZN11SpaceLounge10drawLoungeEv` (1948/1948 B, `byte = linked = false`).

* before: raw **95.676047 %**, official **95.7 %**, 1948 B, 729/728 insns
* after:  unchanged (note only)
* gate: `RATCHET PASS` — byte 4506 / linked 4506, avg 99.94 %, size 4524/4524,
  data 843/843, imports 207, stub/missing/extra 0, lint CLEAN, drift 0

Prior work: `spacelounge-drawlounge-wave122-w7217.md` (pad weight x1.5),
`SpaceLounge-drawLounge-wave123-w7315.md` (weight-order impossibility),
`-wave121-w7117.md`, `-wave119-w6953.md`, `-wave113-w6117.md`.
This note supersedes nothing.

## 1. Residual (unchanged, re-verified)

Only the clamp / join `0x1712b2..0x171330` differs, plus the tail EH selector
offsets that follow from the 2-byte-longer `short pad3` arm:

```text
value        original        HEAD
layout       r4              r4
x            r5 -> r3 -> r9  r9 (whole)
width        r6              r6
agent        r8 (spilled)    r8 (spilled)
top          r2 -> r8        r2 -> r8
pad          r9              fp (r11)
pad*2 (sl)   r10             r10
lineHeight   fp (r11)        fp (r11)
```

## 2. New: exact MIR priority/live-range evidence (instrumented LLVM 7.0.2)

This wave dumped the *pre-RA* machine IR (`MBBDBG`/`MIDBG`/`FINALDBG` from
`/tmp/llvm_android_702b/build2/bin/clang++`, same revision as NDK r18b). The
decisive facts:

* `%2` (pad) is a plain `t2LDRi12 %372:gpr, 148` in **MBB2** (the `mode==0`
  entry block, freq `16777559/71584254`). It has **no COPY anywhere**, so it
  is unhinted.
* `%608` (x) is defined by `%608:rgpr = COPY %386:spr` at the start of
  **MBB39** (the first clamp block, freq `16777408`). `%386` is the `VTOSIZS`
  (fptosi) result of `centerX`; the COPY is the int move out of the VFP
  register. `x` also gets a physical hint `$r3` from
  `$r3 = COPY %608` at the `DrawString` call.
* Live ranges / weights (`FINALDBG`):

  ```text
  %2   pad  size=6384  w=2.159249e-04  li=[496r,6064B)[6752B,7568r)
  %608 x    size=4220  w=2.946140e-04  li=[6784r,6896B)[6912r,6976B)
                                          [6976B,7168B)[7216r,7264B)[7264B,11072r)
  %1   agent size=8784 w=2.263368e-04
  ```

* Greedy priority (`RegAllocGreedy::enqueue`, `RegAllocGreedy.cpp:753-760`):

  ```text
  unhinted: Prio = (1u<<31) | ((1u<<29) + Size)
  hinted:   Prio = (1u<<31) | (1u<<30) | ((1u<<29) + Size)
  ```

  So `x` is selected first *because of the `$r3` hint*, not because of its
  weight. Among unhinted ranges pad's Size (6384) is larger than x's (4220),
  so **if x had no physical hint pad would be selected first** — the target
  order. The hint is therefore the single structural blocker.

* Per-use block frequencies for pad (from `WDBG`):

  ```text
  MBB2  load (def)      0.234375
  MBB2  pad<<1          0.234375   (top)
  MBB39 x-pad           0.234373
  MBB40 inset+pad       0.117186
  MBB41 pad+x           0.117186
  MBB42 pad<<16 (pad3)  0.058593   (only in the HEAD `short pad3` form)
  MBB44 top-pad, x-pad  0.234373 each
  ```

  pad's `UseDefFreq` ≈ 1.40; x's ≈ 1.27. pad already has the *higher* use
  density; it loses only on `Size` (6384 vs 4220). This is the arithmetic form
  of the wave-123 "weight-order" tension: `Size_pad` is dominated by the
  `[496,6064)` stretch because `top` consumes pad in the entry block and the
  clamp consumes it after the whole name/role construction, while `x`'s
  interval starts only at the clamp (`COPY %386:spr`).

## 3. Source shapes measured this wave (all <= 95.676047)

Harness: `/dev/shm/w7613/dl/drive.py` (patch the `mode==0` block, full
`link_so`, official `verify.py` internals, `autojunk=False`).

| variant | raw % | size | note |
|---|---:|---:|---|
| **HEAD (`short pad3`)** | **95.676047** | **1948** | 729/728 |
| nested `else { if (...) ... }` | 95.676047 | 1948 | same allocation |
| nested + faithful `pad*2` | 93.964335 | 1948 | 729/729 |
| ternary clamp | 95.676047 | 1948 | same allocation |
| `int right = w - inset` local | 95.676047 | 1948 | rematerialised |
| `int inset = buttonInsetX` local | 95.676047 | 1948 | rematerialised |
| `x` is the fptosi result directly (no `centerX` local) | 95.676047 | 1948 | MIR identical (`x` still starts at MBB39 COPY) |
| `int textX = centerX` used in first DrawString | 79.890561 | 1952 | extra `vmov`, pad still fp |
| same, both DrawStrings | 80.300958 | 1956 | worse |

The faithful clamp (`width + pad * 2`, the original's exact right arm) was
also swept over `-likely-branch-weight`/`-unlikely-branch-weight` ratios
{7:1, 2000:1, 20:12, 3:1, 5:3, 1:1, 2:1, 4:1, 6:1, 8:1, 10:1, 16:1, 31:1,
50:1, 100:1}: **every ratio gives `pad=fp pad2=sl x=r9 lh=fp arm=itt`**
(729 insns). The branch weight is not the lever either.

`x_direct` is the informative one: removing the `int centerX` local does not
move the `COPY %386:spr` out of MBB39, so `x`'s Size stays 4220. The fptosi
result is kept in a VFP register (`VTOSIZS`) until the clamp regardless of
whether the source names it `centerX` or `x`.

## 4. Why the hint cannot be dropped in source

`NOHINT=%608` (wave-113) removes x's hint and does make pad win the *priority*
comparison, but the greedy allocator then packs pad into `r10` (not `r9`) and
the whole 7-value packing rotates (69.1 %). The target needs the hint present
for the *other* values' selection order while pad still wins `r9`; no source
form that keeps the `$r3 = COPY %608` hint and also removes it from the
priority exists. The `textX` copy variants either coalesce back into `%608`
(no change) or add a real `vmov` (1952/1956 B).

## 5. Conclusion

The residual is a single greedy-RA ordering fact, now pinned to the exact MIR:
`x`'s `$r3` physical hint (from the `DrawString` argument COPY) outranks
`pad`'s larger unhinted interval, so `x` takes `r9` before `pad` is selected.
pad's `Size` is inherently 6384 because `top` uses it in the entry block and
the clamp uses it after the name construction; no source-neutral expression
changes that. The committed HEAD (`short pad3`, **95.67604667124228 %**,
1948 B) remains the best measured shape.

## 6. Structural state (re-verified)

`check` = RATCHET PASS: byte 4506 / linked 4506, avg 99.94 %, size 4524/4524,
data 843/843, imports 207, stub/missing/extra 0, lint CLEAN, drift 0,
27 sections, file size 2,187,328 B exact.

## 7. Reproduction

```bash
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
# per-variant raw score
python3 /dev/shm/w7613/dl/score.py
# MIR weights / live ranges (instrumented LLVM 7.0.2)
WDBG=1 /tmp/llvm_android_702b/build2/bin/clang++ <TU flags> -c SpaceLounge.cpp
```
