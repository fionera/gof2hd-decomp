# Hud::draw — wave 160 (w10617): the "marker loop" hint does not apply; `short barH` is a size anchor

**Status:** analysis-only, 2026-11-08 (wave 160). Worktree `wt/w10617`,
base `master 08af4633b` (wave 159). Target `_ZN3Hud4drawExxP9PlayerEgobjj`
(`src/game/ui/Hud.cpp`, `Hud::draw`), original **9332 B** (VMA `0x163b90`).

| metric | before | after |
|---|---:|---:|
| raw same-address diff | **1325** | **1325** |
| aligned (instruction-index) diff | **249** | **249** |
| size | 9332 | 9332 |
| frame | 224 | 224 |

Gate on exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size-exact 4524/4524, `data_exact 843/843`, imports
**207**, lint **CLEAN**, drift **0**, parity 5370/5370.

---

## 0. TL;DR

1. The wave-160 mission said "attack the marker loop", with hints about
   `markers->count`, `data()` vs `operator[]`, marker-field access order and
   `float` vs `double` screen projection. **`Hud::draw` contains no loop at
   all** (verified by scanning every backward branch in the original
   disassembly: the only backward branches are the `b.n 163dac` block-ordering
   jump at `0x164500` and cold cleanup blocks at `0x1657xx`; there is no
   counter/back-edge in the function). The hints match `Radar::draw`, not
   `Hud::draw`. They were nevertheless replayed in the closest analogue (the
   jump-drive/cloak bar block) and none improved the metrics.
2. **New finding: the original's `barH` is `int`, not `short`.** The original
   at `0x16594c` does `mov fp,r0` after `GetImage2DHeight` and later
   `vmov s0,fp` at `0x165a24` with **no `sxth`**. Our source's
   `short barH = ...` emits `sxth.w r4,r8` at `0x1659f2`. Changing it to
   `int barH` removes the `sxth` but drops the function to **9324 B**
   (raw 1956 / aligned 257), i.e. `short barH` is a deliberate size anchor,
   not the original type.
3. The 8-byte delta of the `int barH` form is in the **exception cleanup
   block** (String destructor landing pads), not in the jump-drive body: the
   body is 424 instructions / 642 bytes in both forms (original: 426 / 642).
   `short barH` changes the number of live `String` temporaries seen by the
   landing-pad emitter.
4. The residual is concentrated in two regions: the damage-indicator block
   (`0x164000-0x164400`, 486 raw bytes) and the jump-drive/cloak bar block
   (`0x165800-0x166000`, 830 raw bytes). The mission panel
   (`0x164400-0x165800`) is already **byte-identical except 8 bytes**.

---

## 1. Measured variants (full build; base = 1325 / 249 / 9332 / 224)

All variants are applied to the wave-159 source; only the listed change is
made. `size` is `orig/ours`; a non-9332 ours is gate-fatal.

### 1.1 `barH` type / declaration order (jump-drive docking-transfer block)

| variant | size | raw |
|---|---:|---:|
| **base (`short barH = ...`)** | **9332/9332** | **1325** |
| `int barH = ...` | 9332/9324 | 1956 |
| `unsigned int barH` | 9332/9324 | 1957 |
| `long barH` | 9332/9324 | 1956 |
| `int barH = (int) ...` | 9332/9324 | 1956 |
| `int barH = ... & 0xffff` | 9332/9332 | 1342 |
| `int barH = ... & 0xff` | 9332/9332 | 1342 |
| `short barH = (short)(... & 0xffff)` | 9332/9332 | 1325 |
| `int barH;` declared before `String text`, assigned later | 9332/9320 | 1926 |
| `short barH;` declared before `String text`, assigned later | 9332/9328 | 1572 |
| `int barH, barW;` both declared before `String text` | 9332/9320 | 1926 |
| `int barW;` declared before `String text` (barH unchanged) | 9332/9332 | 1325 |
| `barH` computed before `String text` | 9332/9344 | 3785 |
| `int barH` computed before `String text` | 9332/9340 | 3741 |
| `String text` moved after `barW`/`barH` | 9332/9332 | 1990 |

### 1.2 Hoisting / locals (jump-drive block)

| variant | size | raw |
|---|---:|---:|
| **base** | **9332/9332** | **1325** |
| `int screenW = Globals::w;` + `screenW >> 1` | 9332/9308 | 8822 |
| `int halfScreenW = Globals::w >> 1;` | 9332/9276 | 8462 |
| `Globals::w / 2` instead of `>> 1` | 9332/9360 | 2099 |
| `int barYOff = Globals::layout->field_0x218;` | 9332/9328 | 3723 |
| local `PaintCanvas *canvas = Globals::Canvas` for the whole block | 9332/9288 | 8739 |
| local `unsigned int fontId = (unsigned int)(uintptr_t) Globals::font` | 9332/9304 | 2132 |
| `unsigned int barYOffset` -> `int barYOffset` | 9332/9332 | 1325 |
| `int top = (int) this->field_0x3e2` | 9332/9332 | 1325 |

### 1.3 `float` vs `double` (jump-drive block)

| variant | size | raw |
|---|---:|---:|
| **base** | **9332/9332** | **1325** |
| `2.5f` -> `2.5` | 9332/9364 | 2242 |
| `255.0f` -> `255.0` | 9332/9344 | 2091 |
| `0.5f` -> `0.5` | 9332/9336 | 2045 |
| `1.05f` -> `1.05` | 9332/9352 | 2026 |
| `double progress` in the docking-transfer block | 9332/9340 | 2177 |
| `1000.0f` -> `1000.0` | 9332/9332 | 1325 |
| y-expression paren/association variants | 9332/9332 | 1325 |
| `(float) barH * 2.5f` -> `barH * 2.5f` | 9332/9372 | 2145 |

### 1.4 `__builtin_expect` on hot/cold guards

| variant | size | raw |
|---|---:|---:|
| **base** | **9332/9332** | **1325** |
| `__builtin_expect(field_0x4b0 >= 1, 1)` | 9332/9332 | 1325 |
| `__builtin_expect(field_0x277 || field_0x276 || weaponSelectState, 1)` | 9332/9332 | 1325 |
| `__builtin_expect(field_0x4ac >= 1, 1)` | 9332/9336 | 7217 |
| `__builtin_expect(field_0x4b4 >= 1, 1)` | 9332/9472 | 7136 |
| `__builtin_expect(field_0x4b8 >= 1, 1)` | 9332/9336 | 7126 |
| `__builtin_expect(hasArmorRegen, 0)` | 9332/9336 | 8331 |
| `__builtin_expect(hasShieldBar, 1)` | 9332/9352 | 7699 |
| `__builtin_expect(isInRocketControl(), 0)` | 9332/9304 | 8939 |
| `__builtin_expect(mission type == 12, 0)` | 9332/9332 | 7005 |

### 1.5 Damage-block `Globals::status` forms

| variant | size | raw |
|---|---:|---:|
| **base** | **9332/9332** | **1325** |
| `Status *st = Globals::status;` before `hitFlags` | 9332/9332 | 1325 |
| `st` before the first damage block, used in the last condition | 9332/9352 | 7485 |
| `st` immediately before the last condition | 9332/9332 | 1325 |
| `Globals::status && Globals::status && ...` | 9332/9332 | 1325 |
| add `Globals::status &&` to `field_0x4ac` | 9332/9344 | 7329 |
| add to `field_0x4b0` | 9332/9340 | 7113 |
| add to `field_0x4b4` | 9332/9348 | 7070 |
| move `&&` from `field_0x4b8` to `field_0x4ac` | 9332/9336 | 7154 |

### 1.6 Gamma-block reorder (the 5-byte `fp`/`r9` swap)

| variant | size | raw |
|---|---:|---:|
| **base** | **9332/9332** | **1325** |
| swap `gammaY`/`gammaBarY` declaration order | 9332/9332 | 1325 |
| `gammaBarY` declaration first | 9332/9332 | 1325 |
| `0x448 + 0x448 - 0x44a` | 9332/9328 | 7021 |
| `0x448 * 2 - 0x44a` | 9332/9328 | 7021 |

---

## 2. Why the `int barH` form is the right lead but not yet a win

The original jump-drive body has 426 instructions / 642 bytes; both our
`short barH` and `int barH` bodies have 424 / 642. The original's extra two
instructions are the `mov r5,fp` at `0x16594c` and the `ldr r3,[sp,#96];
ldr r3,[r3]` pair at `0x1659d8/0x1659e0`, both consequences of the original
allocating `barH` to **`fp`** and spilling `Globals::Canvas` to `sp+96`.
Our allocator instead keeps `Globals::Canvas` in `fp` and puts `barH` in
`r8`, so it needs neither the move nor the reload. The `int barH` form
removes the `sxth` but does not change that allocation, and its 8-byte loss
is entirely in the landing-pad block (the body is still 642 bytes).

The next productive attack is therefore to force `barH` into `fp` (or
equivalently force `Globals::Canvas` to be spilled to `sp+96`) while
preserving the 9332-byte landing-pad count. The `short barH` anchor is
currently doing that size preservation indirectly; a source-level
`String`-lifetime change that adds the same landing-pad entry without the
`sxth` would let the `int barH` form land.

## 3. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10617
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10617/fdiff_w.py _ZN3Hud4drawExxP9PlayerEgobjj
python3 /dev/shm/w10617/align_diff_w.py _ZN3Hud4drawExxP9PlayerEgobjj
cmake --build cmake-build-match --target check -j6
```

Scratch harness in `/dev/shm/w10617`: `runvar.py`, `sweep1.py`..`sweep13.py`,
`fdiff_w.py`, `align_diff_w.py`, `adiff_w.py`, `rawruns_w.py`,
`Hud.base.cpp` (baseline source).
