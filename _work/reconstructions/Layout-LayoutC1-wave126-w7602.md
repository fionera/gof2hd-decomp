# `Layout::Layout` C1/C2 — wave 126 / worker w7602

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7072 B (0x1ba0)**, source `src/game/ui/Layout.cpp`.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(sha256 `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`).
Base = `bf365a964` (wave-125 final).

## Result — no source win

| metric | before (base) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| unrounded `difflib` ratio | 48.943802 % | **48.943802 %** |
| `report.json` pct (1 dp) | 48.9 | 48.9 |
| `byte_exact` / `linked_exact` | 4506 / 4506 | **4506 / 4506** |
| `size_exact` / `data_exact` | 4524 / 843 | 4524 / 843 |
| imports / lint / drift | 207 / CLEAN / 0 | 207 / CLEAN / 0 |

`cmake --build cmake-build-match --target check -j8` → `RATCHET PASS`.
No `src/` change landed; the worktree is clean apart from this note.

## 1. Ghidra reconstruction (ground truth)

The function is at **Ghidra address `0xe1028`**, not `0xd1028`: the program is
loaded with image base `0x10000`, so `ghidra_addr = ELF_VMA + 0x10000`.
`rea decompile ... 0xe1028` returns the constructor (1551 lines).  The
decompiler confirms the original is **config-major**, with this shape:

```
if (iPadHD) {                          // prefix HD block, ~137 stores
    /* fields 0x4..0x1e0, mostly address order, SLP-grouped */
} else {
    if (n9 == 0) { /* default/retina/iPad prefix */ }
    else         { /* n9 prefix */ }
    /* common1: fields 0x2d0..0x1e0, a tree of if/else groups */
}
/* common2: fields 0x1e8..0x320, a SECOND `if (iPadHD)` block + else tree */
```

Two separate `iPadHD` blocks (prefix and common2) are visible in the machine
code (`0xd1042` and `0xd2202`); the prefix HD block branches to `0xd21e2`
(common2), while the n9/else prefix blocks branch to `0xd1860` (common1).
Ghidra assigns several fields more than once (e.g. `0x5c`, `0x2c4`, `0x130`,
`0x198` are each written 4–5 times in different branches), which is exactly
why a per-field ternary body is ~400 B shorter than the original.

## 2. Store-trace comparison (all 32 flag combinations)

A Thumb/VFP emulator (`emu.py` from `_work/reconstructions/layout_ctor/`,
retargeted at our linked `.so`) reproduces the first-write order for every
configuration.  Values are **identical for all 32 combinations** (0
differences), so the remaining gap is pure codegen.

The HD trace order (first write to each word) agrees for the first 10 stores
and then diverges:

```
orig: 14 4 24 34 44 54 64 2c8 7c 74 2c0 2d8 ac 8c 94 a4 c0 b0 ec 294 2a4 ...
ours: 14 4 24 34 44 54 64 2c8 7c 74 2c4 1e0 2d8 8c 98 9c ac b4 c4 294 ...
```

The n9 trace diverges immediately after `0x74`:

```
orig: 74 2c0 14 4 24 34 44 54 64 2d0 2d4 7c 8c 90 94 98 9c ac a8 a0 ...
ours: 74 2c4 1e0 2cc 14 4 24 34 44 54 64 2c8 2d8 2d4 7c 8c 90 98 9c ac ...
```

The concrete differences are:

* the original groups `0x2c0..0x2cc` in one `stmia`/`vst1` in the n9 arm;
  ours stores `0x2c4` alone and puts `0x2c0` in common1;
* the original stores `eventBannerScale` (`0x1e0`) in common2 (trace position
  42, between `0x1d0` and `0x1e8`); ours stores it in the prefix HD/n9 arms;
* the original groups `{0x94,0x98,0x9c,0xa0}`; ours stores `0x94` near the
  end of the function and `0x98/0x9c` early.

## 3. Variants measured (real `link_so` builds, exact unrounded ratio)

Baseline is 7072 B / **48.943802 %**.

| variant | size | ratio % |
|---|---:|---:|
| move `eventBannerScale` after `0x1e4` | 6976 | 39.711481 |
| move `eventBannerScale` after `0x1dc` | 6960 | 43.980738 |
| move `eventBannerScale` before `0x1e8` | 6976 | 39.711481 |
| move `0x2c0` after `0x2c4` | 7032 | 45.181927 |
| reorder `0x2c0,0x2c4,0x2c8,0x2cc` (keep rest) | 7056 | 42.999601 |
| reorder `0x2c0..0x2cc` + move `eventBannerScale` after `0x1e4` | 7052 | 40.636816 |
| reorder `0x2c0..0x2cc` + `eventBannerScale` after the block | 7044 | 44.488818 |
| move `0x94` before `0x98` | 7040 | 46.498599 |
| move `0x94` after `0x98` | 7040 | 46.049210 |
| move `0x8c` before `0x94` | 6992 | 45.478243 |
| move `0xa4` before `0xa0` | 7080 | 48.258013 |
| move `0xb0` before `0xac` | 7040 | 43.575419 |
| move `0xc0` before `0xb0` | 7088 | 45.076586 |
| `0x190` if/else → ternary | 7052 | 48.603352 |
| `0x1cc` if/else → ternary | 7072 | 48.864089 |
| `0x20c` if/else → ternary | 7108 | 47.864945 |
| `0x218` if/else → ternary | 7088 | 47.710076 |
| `orbitInfoStationY` if/else → ternary | 7088 | 47.710076 |
| `0x260` if/else → ternary | 7152 | 45.995242 |
| `0x280` if/else → ternary | 7072 | 48.864089 |
| `0x2d8` ternary → if/else | 7072 | 48.943802 (identical) |
| remove `volatile` on `rd` | 7064 | 48.853440 |
| `rd` → `const volatile` | 7072 | 48.943802 (identical) |
| remove `t_field_0x2c8` temp | 7072 | 48.943802 (identical) |
| `int v` → `float v` | 7064 | 48.923445 |
| remove `v`, direct `(int)(float)(...)` | 7064 | 48.923445 |
| remove `v`, int-typed `0xa8` arms | **7016** | **48.980408** |
| `0x1e8` → `(!n9 && !iPadHD) ? retina : 1` | 7000 | 42.069517 |
| `0x1e8` → `(n9 || iPadHD) ? 1 : ...` | 7072 | 48.943802 (identical) |

Also exhausted: 400 random **pairwise swaps** of single-line field
assignments in the constructor (all size-7072 swaps scored ≤ baseline).

The only variant above baseline is the int-typed `0xa8` form (48.980408 %),
but it compiles to **7016 B**; a non-7072 size shifts `.text` and drops
`byte_exact` 4506 → 2455 (measured), so it is not landable without a size
anchor, and the original *does* keep the `0xa8` int→float→int round-trip
(`vcvt.f32.s32` + `vcvt.s32.f32` at `0xd19da`), so the int form is also less
faithful.

## 4. Callee-saved-register difference

The original prologue is
`push {r4,r5,r6,r7,lr}; add r7,sp,#12; stmdb sp!,{r6,r7,r8,r9,sl,fp}`
(6 registers in the second push).  Ours is
`push {r4,r5,r6,r7,lr}; add r7,sp,#12; stmdb sp!,{r5,r6,r7,r8,r9,sl,fp}`
(7 registers).  The union is identical; only the second `stmdb` range starts
at `r5` instead of `r6` (one extra 4-byte slot, i.e. 4 bytes fewer body bytes
elsewhere).  This is a clang 7.0.2 prologue-emission choice, not a source
construct; no source variant tested moved it.

## 5. Conclusion

The current field-major body is a strong local optimum for the fuzzy metric.
The true source is config-major with two `iPadHD` blocks and duplicated
stores across branches (Ghidra shows 4–5 assignments for several offsets).
Reproducing it byte-for-byte requires transcribing that branch tree exactly
(including the duplicates), which is the remaining work; every local reorder
or expression rewrite measured here changes the 7072-B size or lowers the
ratio.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7602
cmake --build cmake-build-match --target check -j8      # RATCHET PASS, byte 4506
python3 /dev/shm/w7602/score.py                          # 7072 B, 48.943802 %
GHIDRA_INSTALL_DIR=/tmp/ghidra_12.1.2_PUBLIC \
  rea decompile /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so 0xe1028
```

Scratch harnesses (not committed, `/dev/shm/w7602/`): `score.py` (exact
single-function ratio), `emu2.py`/`emu3.py` (store/PC emulator, `EMU_SO` and
`EMU_TRACE` env vars), `cmp32.py` (all-32-combination value check),
`swapsearch.py` (pairwise swap search), `run_trials.py`/`run2.py`..`run6.py`
(variant batches), `Layout.base.cpp` (the untouched source).
