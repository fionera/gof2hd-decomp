# Layout::Layout C1/C2 — wave-103 oracle: the source order is the iOS order,
# but the fuzzy optimum is still the hill order (wf301)

Worker **wf301**, branch `wave103-wf301`, base `1d2b4398e`.
Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` at ELF VMA `0xd1028`, original size
**7072 B**. Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(Ghidra image base `0x10000`, so the ctor is `0xe1028` in the Ghidra project).

Continues `layout-layout-c1-c2-literal-pool-constants-wf102.md` (wave 102,
the landed 47.7724741448 state), `layout-layout-c1-c2-ios-order-wf101.md`
(wave 101) and `layout_ctor/ios_source_order.md`.

## Result

**No landable improvement found.** The ctor stays at **7072 B /
47.7724741448 %** (unrounded `fscore.py`, `autojunk=False`).  This pass adds
one new, independent confirmation of the source order from the wave-103
third-party Ghidra oracle, plus a measured table of the block-structured and
expression variants it suggested.  None beats the hill order.

Gate before == after (`check` on the pristine tree):

| metric | value |
|---|---:|
| `byte_exact` | 4500 / 4524 |
| `linked_exact` | 4500 / 4524 |
| `size_exact` | 4524 / 4524 |
| `stub_zero_size` / `missing` / `wrong_type` / `extra` | 0 |
| `data_exact` | 843 / 843 |
| imports | 207 |
| lint / drift | CLEAN / 0 |

`RATCHET PASS`.

## 1. New oracle and why it is useful

Wave 103 adds a full Ghidra decompilation of a KiritoJPK NDK-r14/clang-3.8
`-O2` rebuild of the same game source family:

```
/home/debian/GalaxyOnFire2/_work/oracles/Galaxy-on-Fire-2-Unity-Remake/Reference/decompiled/native/
  game/Layout.c        # Layout::Layout @0x000e2a00 (8140 bytes)
  _FUNCTIONS.tsv
```

It is a *different* compiler and a slightly different source revision
(localization/DeviceInfo differ, e.g. the non-HD `field_0x4` is `22.5` there
vs the Android `22`), so it is a source-shape oracle only.  But its control
flow is the original source's control flow.

## 2. The oracle's constructor structure

`Layout.c` lines 8..2026 (the ctor) have this shape:

```
if (Globals::iPadHD == 0) {
    /* 0x4 .. 0xa4, one per-field n9/retina chain each */
} else {
    /* 0x4 .. 0xa4, straight-line int stores */
}
field_0xb0 = ...;      /* fVar28 shared out of both arms */
field_0xb8 = ...;      /* (n9 || iPadHD) ? 25 : retina ? 36 : 18 */
if (Globals::iPadHD) {
    /* 0xb4 .. 0x1e0, straight-line int stores */
    goto done;
}
if (n9) { /* 0xb4 .. 0x14c */ } else { /* 0xb4 .. 0x14c */ }
field_0x150 = ...;     /* fVar28 shared out of both arms */
/* 0x154 .. 0x320, per-field chains */
done:
field_0x1e4 = ...; field_0x1e8 = ...;
...
```

Two things are directly recoverable from it:

1. **The source order.**  Extracting the `*(...(this + 0xNN)) = ...` offsets in
   decompile order (expanding `undefined8` stores into two words) gives, for the
   non-HD arm, exactly the iOS order of `layout_ctor/ios_source_order.md`:

   ```
   4 8 c 10 14 18 1c 20 24 28 2c 30 34 38 3c 40 44 48 4c 50 54 58 5c 60 64
   68 6c 2c0 70 74 78 2c4 2c8 2cc 2d0 2d4 2d8 7c 80 84 88 8c 90 94 98 9c
   ac a8 a0 a4
   ```

   and for the `0xb4..0x1e0` region:

   ```
   b4 bc c0 c4 c8 cc d0 d4 d8 dc e0 e4 e8 ec f0 f4 f8 fc 100 104
   294 298 29c 2a0 2a4 2a8 2ac 2b0 2b4 2b8
   108 10c 110 114 118 11c 120 124 128 12c 130 134 138 13c 140 144 148 14c
   ```

   i.e. the iOS order with `b0`/`b8` hoisted before the block and `0x150` after
   it.  This is a **second, compiler-independent** confirmation of wf101's
   conclusion.

2. **The per-field flag order** matches the current `Layout.cpp` expressions.
   Example (`field_0x5c`): oracle order `n9, iPadLarge, retina, iPad`, same as
   our `iPadHD ? ... : (n9 ? 51 : (iPadLarge ? 90 : (retina ? 60 : (iPad ? 45
   : 30))))`.

3. **The HD arm is a straight-line int block.**  The original Android binary
   agrees: after the single `beq.w` on `iPadHD` at `0xd103e`, `0xd1042..0xd1268`
   is one run of `vld1.64 {d2-d3}` / `vst1.32 {d2-d3}` 16-byte SLP groups
   (`0x14,18,1c,20`, `0x4,8,c,10`, `0x24,28,2c,30`, ...), then `b.w 0xd21e2`.
   There is exactly **one** `iPadHD` test for the whole block, not one per
   field, so the compiler has hoisted the flat ternaries into the oracle's
   block shape.  The block structure is therefore a *compiler artifact of the
   flat-ternary source*, not necessarily explicit `if` statements.

## 3. Measured variants (official linked scorer, `autojunk=False`)

All sizes are real `link_so` sizes; all ratios are the unrounded
`tools/verify/verify.py` ratio for `_ZN6LayoutC1Ev`.

| variant | `usize` | raw % |
|---|---:|---:|
| HEAD (hill order, landed) | **7072** | **47.7724741448** |
| iOS order, current expressions (flat) | 6936 | 38.00723182 |
| iOS order + `gen.py` regenerated ternaries | 6740 | 26.797 |
| iOS order, HD arms as plain int literals | 6780 | 14.046 |
| iOS order, explicit `if (iPadHD) {0x4..0xa4} else {...}` block (block1 only) | 6680 | 33.3469 |
| iOS order, block1 + `if (iPadHD) {..} else if (n9) {..} else {..}` for `0xb4..0x1e0` | 6352 | 31.7362 |
| iOS order, all arms as literal constants | 5624 | 25.9660 |
| original default-path **store order** as source order | 6804 | 27.5512 |
| whole HEAD body duplicated in `if (iPadHD) {..} else {..}` | 7132 | 22.55 |
| HEAD + `field_0x1e8` as `(n9 || iPadHD) ? 1 : retina` | 7056 | 43.2863 |
| HEAD + `field_0xb8` as `(n9 || iPadHD) ? 25 : (retina ? 36 : 18)` | 7048 | 18.1389 |
| HEAD + `field_0xb0` non-HD arms as `4.5f/6.0f/3.0f` | 7072 | 47.7724741448 (code-identical) |
| HEAD + `field_0x2d4` as one 2-variable ternary | 7064 | 47.4333 |

The block-structured variants are systematically *smaller* than the original:
literal arms let clang pick better immediates and drop the per-field
`it eq` predication.  The flat iOS order (6936 B) is the closest faithful
shape, 136 B short.

## 4. Where the iOS-order build actually diverges from the original

Running the w4301 store emulator (`layout_ctor/emu.py`, path-adapted) over both
binaries gives the word-level store order:

* `iPadHD=1`: the iOS-order build matches the original's HD store sequence for
  the **first 41 words** (through `0x70`), then diverges: the original stores
  `ac 8c 90 94 98 9c a0 a4 a8 c0 c4 c8 cc b0 b4 b8 bc`, ours stores
  `8c 90 94 98 9c b0 b4 b8 bc c0 c4 c8 cc ... a8 ac a0 a4`.
* default: matches for the first 10 words, then the original stores
  `30 2c 38 34 44 40 4c 48 54 50` where ours stores
  `2c 30 34 38 40 44 48 4c 50 54` (a consistent even/odd scheduler swap).

So the source order is right but the *scheduling* differs.  Instruction-mix
residue (code only, original vs iOS order): original has +52 `movs`, +18 `cbz`,
+14 `cmp`, +5 `clz`, +5 `vorr`, +6 `vmov.f32`, +5 `vstr`, +6 `vld1.64`,
+6 `vst1.32`; ours has +23 `cmp.w`, +12 `strd`, +11 `str.w`, +16 `beq.n`,
+9 `mov.w`, -11 `addw`.  Code bytes: original 6106, iOS order 6036; pool:
original 966, iOS order 900.

## 5. Conclusion

* **Proven (new):** the wave-103 oracle's source order is the iOS order, for
  both the `0x4..0xa4` head and the `0xb4..0x1e0` mid-block, and its per-field
  flag order matches the current `Layout.cpp`.  Its top-level structure is
  `if (iPadHD) { head } else { head }`, then `b0`/`b8`, then
  `if (iPadHD) { mid } else if (n9) { mid } else { mid }`, then the shared
  tail — which is exactly what clang hoists out of the current flat ternaries.
* **Proven (measured):** every block-structured variant is smaller than 7072 B
  (5624..6680 B) and scores far below 47.77 %.  The store order is not the
  source order (it scores 27.55 %).
* **Hypothesis:** the remaining 136 B is a scheduling/register-allocation
  difference driven by exact expression types (the original keeps the flag
  bytes in low registers, uses more `cbz`/`cmp`/`clz`, and pairs more stores
  into `strd`).  Recovering it needs the exact original expression forms, which
  the oracle cannot pin down because its revision and compiler differ.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/wf301
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8     # RATCHET PASS
python3 /dev/shm/wf301/fscore.py                       # 7072 / 47.7724741448
python3 /dev/shm/wf301/apply.py /dev/shm/wf301/ios_cur_expr.cpp
cmake --build cmake-build-match --target link_so -j8
python3 /dev/shm/wf301/fscore.py                       # 6936 / 38.00723182
python3 /dev/shm/wf301/genblock5.py                    # block1-only body
python3 /dev/shm/wf301/apply.py /dev/shm/wf301/block5_body.cpp
cmake --build cmake-build-match --target link_so -j8
python3 /dev/shm/wf301/fscore.py                       # 6680 / 33.3469
```

Scratch (`/dev/shm/wf301`): `fscore.py` (official unrounded scorer),
`apply.py` (swap a ctor body into `Layout.cpp`), `reorder2.py` (statement
splitter + reorder), `genblock5.py`/`genblock6.py` (block-structured body
generators), `tern.py` (ternary splitter), `emu.py`/`wordorder.py` (store
emulator), `Layout.head.cpp` (pristine HEAD ctor), `ios_cur_expr.cpp`,
`block5_body.cpp`, `block6_body.cpp`, `block_body.cpp`, `ios_order.txt`,
`store_order.txt`.
