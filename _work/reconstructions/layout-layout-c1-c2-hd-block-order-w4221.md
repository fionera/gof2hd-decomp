# Layout::Layout (`_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev`) — HD-block field set, store-group order, and the `(int)(float)` round-trip family

Worker **w4221** (wave 92), branch `wave92-w4221`, base `b379d00d`.
Target: `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` at `0xd1028`, original size **7072 B**.
Baseline match: **47.7 %**, `usize` 7072. No code change landed (see §9 for why a
strictly-better *landable* variant was not found); this note is the deliverable.

All addresses below are **original-image VMAs** (image base `0x10000`). The function is
`0xd1028 .. 0xd2bc8`; its literal pools are inside that span (`.text` is
`0x69440 .. 0x1cb808`, so the pools are *not* `.rodata`).

---

## 1. What is already known (pointer to prior notes)

Prior passes established:

* the original HD (`Globals::iPadHD != 0`) branch is **SLP-vectorized**: runs of four
  consecutive `int` fields become `vld1.64 {dN-dN+1}` + `vst1.32 {dN-dN+1}` groups;
* our first eight vector groups are identical to the original's;
* the residual is scheduling/register allocation plus a small set of fields that the
  original stores with `vstr` (float) instead of `str`/`vst1.32` (int);
* the current body is a strong local optimum (exhaustive ±2/±3 moves, 1200 random
  moves, and every single ternary↔if/else conversion all fail to beat 47.7 at 7072 B).

Read alongside:
`layout-layout-c1-c2-hd-vector-w4101.md`,
`layout-layout-c1-c2-sweeps-w4001b.md`,
`layout-layout-c1-c2-source-shape-w3905.md`,
`layout-layout-c1-c2-midtail-vstr-w4001.md`.

---

## 2. New result: the HD block has exactly 137 fields, ours 138 — the difference is `0x1e8`

Running the original under the small Thumb/VFP emulator
(`_work/reconstructions/layout_ctor/emu.py`, copy at `/dev/shm/w4221/emu2.py`) over
the HD flag combination and comparing every word stored before the HD block's tail
(`b.w 0xd21e2`) gives:

* original HD block stores **137** distinct fields;
* our current body stores **138**;
* the symmetric difference is exactly one field: **`0x1e8`**.

`0x1e8` is written by the original in the **mid block**, not the HD block. Its source
is `field_0x1e8 = retinaDisplay ? 1 : (iPadHD ? 1 : (n9 ? 1 : 0))`. Because the
`retinaDisplay` test is first, clang sinks the whole assignment out of the HD region
(where it is provably `1`) into the common mid path. Our source has the same line in
the same place, but our codegen emits it inside the HD region.

Moving the line in source order was measured and always loses (see §5); the sink is a
scheduling/RA consequence of the surrounding HD block, not of the line's position.

---

## 3. Exact original HD store-group sequence (ground truth)

A "group" is one store instruction: `offset(count)`. Extracted from the original
disassembly of `0xd1042 .. 0xd1268`:

```
14(4) 4(4) 24(4) 34(4) 44(4) 54(4) 64(4) 2c8(4) 7c(4) 74(2) 2c0(2) 2d8(1) ac(1)
8c(2) 94(4) a4(2) c0(4) b0(4) ec(4) 294(4) 2a4(4) 108(4) 118(4) d0(3) dc(2)
e4(2) fc(2) 104(1) 2b4(2) 128(4) 140(4) 150(4) 160(4) 170(4) 180(4) 190(4)
1a0(4) 1b0(4) 1c0(4) 1d0(4) 138(2) 1e0(1)
```

Ours (current source):

```
14(4) 4(4) 24(4) 34(4) 44(4) 54(4) 64(4) 2c8(4) 7c(4) 74(2) 2c4(1) 1e0(1)
2d8(1) 8c(2) 9c(1) ac(1) 98(1) b4(4) c4(4) 294(4) 2a4(4) dc(4) b0(1) a8(1)
a0(1) d4(2) a4(1) ec(2) f4(2) fc(3) 2b4(1) 118(4) 108(4) 128(4) 148(4)
158(4) 168(4) 180(4) 190(4) 1a0(4) 1b0(4) 23c(1) 2b8(1) 138(2) 204(1)
2bc(1) 178(2) 2c0(1) 1c0(4) 1d0(4)
```

The **first ten groups match exactly**. Divergence starts at `2c4/1e0`. The original
HD block is 164 instructions and ends `d1268 b.w d21e2`; ours ends
`d1274 b.w d2274`.

The original's HD-block field order is the iOS source order **restricted to HD
fields**; the 4-word vector groups are contiguous iOS-order runs. The mid/tail block
is *not* in iOS order (the pure iOS-order body scores 37.5 % / 6864 B; its n9 region
is 97.4 %, its HD region 3.1 %).

---

## 4. New result: the exact `vstr` (float-store) sets

`vstr sN, [r0, #imm]` is a 4-byte float store. The original uses it for 27 offsets;
ours for 26. Both sets are listed in decimal byte offsets:

| original `vstr` | ours `vstr` |
|---|---|
| 96 `0x60`, 104 `0x68`, 108 `0x6c`, 112 `0x70` | 32 `0x20`, 96, 104, 108, 112 |
| 140 `0x8c`, 144 `0x90`, **156 `0x9c`**, 168 `0xa8` | 140, 144, 168 |
| 220 `0xdc`, 224 `0xe0`, 228 `0xe4`, 232 `0xe8` | 220, 224, 228, 232 |
| 312 `0x138`, **336 `0x150`**, **344 `0x158`**, **348 `0x15c`** | 292 `0x124`, 312, 328 `0x148` |
| **352 `0x160`**, 360 `0x168`, **368 `0x170`**, 424 `0x1a8` | 352, 360, 424 |
| 480 `0x1e0`, **484 `0x1e4`**, 712 `0x2c8`, 720 `0x2d0` | 480, 500 `0x1f4`, 516 `0x204` |
| 760 `0x2f8`, 992 `0x3e0`, **1016 `0x3f8`** | 712, 720, 760, 992, 1000 `0x3e8` |

**7 missing** (original `vstr`, ours plain int store):
`0x9c 0x150 0x158 0x15c 0x170 0x1e4 0x3f8`.

**6 extra** (ours `vstr`, original plain int store):
`0x20 0x124 0x148 0x1f4 0x204 0x3e8`.

### 4.1 What the `vstr` actually is

The original's `vstr` stores are **int fields written through a float round-trip**.
Example, original `0x3f8` (`rewardBoxY2`):

```
d2514: add.w r2, r1, #17
d2518: lsl.w r2, r2, r8            ; (value + 17) << retinaDisplay
d251c: vmov  s0, r2
d2520: vcvt.f32.s32 s0, s0
d2524: vcvt.s32.f32 s0, s0
d2528: str.w r1, [r0, #1012]       ; 0x3f4 = rewardBoxY
d252c: vstr  s0, [r0, #1016]       ; 0x3f8 = rewardBoxY2
```

The emulator reads `0x3f8` back as the **int** `132/111/127/...`, not as float bits,
so the field is `int` and the source contains an `(int)(float)` conversion. The same
shape appears at `0x9c`, `0x150`, `0x158`, `0x15c`, `0x170`, `0x1e4`.

### 4.2 Why our `(int)(float)(...)` folds but the original's does not

clang 7.0.2 (same toolchain, verified from `.comment`) folds
`(int)(float)(cond ? 100.0f : 50.0f)` to `select i32 100, 50` **at the front end**
when the value is a compile-time-bounded select/phi of constants. Measured with the
exact project flags (`-target armv7-none-linux-androideabi -march=armv7-a -mthumb
-mfpu=neon -mfloat-abi=softfp -Oz`):

| source shape | codegen |
|---|---|
| `s->x = (int)(cond ? 100.0f : 50.0f);` | `movs #100; moveq #50; str` |
| `float f = cond ? 100.0f : 50.0f; s->x=(int)f;` | folds |
| `int v; if(...) v=70; ... ; s->x=(int)(float)v;` | folds |
| `s->x = (int)(float)(v * 1.0f);` where `v` is a local int | folds |
| `s->x = (int)(float)v;` where `v` comes from an **opaque load/member** | **keeps `vmov/vcvt.f32.s32/vcvt.s32.f32/vstr`** |
| `-O0` (any of the above) | keeps the round-trip |

So the original's conversion survives because the converted value is **opaque at the
point of conversion** — in the `0x3f8` case the source reads the member
`this->rewardBoxY` (a memory load) before the shift, so the front end cannot bound the
result and emits `fptosi(sitofp(...))`; InstCombine does not re-fold it after the
store-to-load forwarding. This is the key to reproducing the 7 missing `vstr`s: the
conversion must be applied to a value that is not a constant select/phi.

### 4.3 The 6 extra `vstr`s: our ternaries are float-typed, the original's are int-typed

For these 6 fields our source has `iPadHD ? N * 1.40625f : ...`, which makes the
conditional expression `float`-typed; the non-HD path then does the int→float→int
round-trip. The original's non-HD path uses plain `str`, so its conditional is
`int`-typed and its HD branch is a plain integer. The HD values are the truncations:

| field | our HD expr | HD value (emulator) | original HD branch |
|---|---|---|---|
| `0x20`  | `5 * 1.40625f`   | 7   | `7`   |
| `0x124` | `1 * 1.40625f`   | 1   | `1`   |
| `0x148` | `2 * 1.40625f`   | 2   | `2`   |
| `0x1f4` | `16 * 1.40625f`  | 22  | `22`  |
| `0x204` | `9 * 1.40625f`   | 12  | `12`  |
| `0x3e8` | `100 * 1.40625f` | 140 | `140` |

Rewriting those six HD branches as plain integers was measured (see §5) and every one
*reduces* the match, because the change also perturbs the HD SLP grouping and the
literal pool; it is not a local edit.

---

## 5. Measurements (official verifier, `autojunk=False`)

Baseline: **7072 / 47.7** (`score.py`). All numbers below are `usize / pct`.

### 5.1 Single-field source-form changes (current body)

| change | result |
|---|---|
| `0x98` if/else (instead of ternary) | **7016 / 47.8** |
| `0x9c` if/else → ternary | 7064 / 47.6 |
| `0x9c` ternary + `(int)(float)` | 7064 / 47.6 |
| `0x150` `(int)(float)(...)` | 7080 / 47.7 |
| `0x158` `(int)(float)(...)` | 7072 / 47.7 |
| `0x15c` `(int)(float)(...)` | 7072 / 47.7 |
| `0x170` `(int)(float)(...)` | 7072 / 47.7 |
| `0x1e4` `(int)(float)(...)` | 7056 / 42.5 |
| `0x158` all-float branches | 7072 / 47.7 |
| `0x15c` all-float branches | 7072 / 47.7 |
| `0x170` all-float branches | 7072 / 47.7 |
| `0x150` all-float branches | 7080 / 47.4 |
| `0x1e4` all-float branches | 7072 / 41.0 |
| `0x3f8` float-typed ternary (`(rewardBoxY+17)*1.40625f`) | 7080 / 47.6 |

### 5.2 The 6 extra-`vstr` fields rewritten to int HD branches

| change | result |
|---|---|
| `0x20` → `7` | 6968 / 20.9 |
| `0x124` → `1` | 7008 / 42.6 |
| `0x148` → `2` | 7056 / 47.5 |
| `0x1f4` → `22` | 7056 / 47.6 |
| `0x204` → `12` | 7064 / 47.4 |
| `0x3e8` → `140` | 7076 / 42.4 |

### 5.3 The `0x3f8` restructure (the one real structural finding)

The original computes `rewardBoxY2` from `rewardBoxY` for **all** non-HD/non-n9
branches. Verified against the emulator table (`lay_orig.json`):

| config | `rewardBoxY` `0x3f4` | `rewardBoxY2` `0x3f8` | `(y+17)<<retina` |
|---|---|---|---|
| HD | 77 | 132 | (special) |
| n9 | 57 | 111 | (special) |
| iPadLarge | 110 | 127 / 254 | 127<<0 / 127<<1 ✔ |
| iPad | 55 | 72 / 144 | 72<<0 / 72<<1 ✔ |
| retina | 76 | 186 | 93<<1 ✔ |
| else | 38 | 55 | 55<<0 ✔ |

So the faithful source is:

```cpp
if (Globals::iPadHD)
    this->rewardBoxY2 = 132;
else if (Globals::n9)
    this->rewardBoxY2 = 111;
else
    this->rewardBoxY2 = (this->rewardBoxY + 17) << Globals::retinaDisplay;
```

(and with the `(int)(float)` of §4.1 if the conversion is wanted).

Measured:

| variant | result |
|---|---|
| restructure, no `(int)(float)` | **7024 / 47.8** |
| restructure with `(int)(float)` | 7040 / 46.7 |
| restructure (no float) + `0x9c` ternary | **7024 / 48.0** |
| restructure (no float) + `0x9c` ternary + any single `(int)(float)` wrap | no variant reaches 7072 with pct > 47.7 (best 7040 / 47.7) |

Region breakdown (`regions.py`, original regions):

| region | baseline | restructure+`0x9c` |
|---|---|---|
| hd (`0xd1042..0xd1268`) | 25.8 % | 25.8 % |
| n9 (`0xd127e..0xd12fc`) | 55.3 % | 55.3 % |
| def (`0xd1570..0xd1830`) | 60.2 % | 60.2 % |
| mid (`0xd1860..0xd215c`) | 28.9 % | 28.6 % |
| tail (`0xd21e2..0xd2bd0`) | 49.4 % | **49.9 %** |
| overall | 47.66 % | **47.96 %** |

### 5.4 Order experiments (all at 7072 unless stated)

| order | result |
|---|---|
| current source order | 7072 / 47.7 |
| iOS order | 6864 / 37.5 |
| iOS order + `94..ac` ascending | 6864 / 33.8 |
| iOS order + `d0..e8` ascending | 6864 / 37.5 |
| HD-store order as source order | 6824 / 36.8 |
| `order_default.txt` | 6864 / 28.5 |
| `order_hd.txt` | 6880 / 11.2 |
| `order_rest.txt` | 6896 / 15.5 |
| reverse whole body | 6760 / 8.9 |
| reverse first 150 | 6556 / 16.4 |
| reverse tail | 6912 / 33.4 |
| `0x94` before `0x98` | 6984 / 45.3 |
| `0x144` before `0x148` | 6984 / 45.0 |
| `0x140` before `0x148` | 6972 / 45.3 |
| `0x1d0` before `0x138` | 7032 / 41.1 |
| `0x1c0` before `0x138` | 6992 / 40.0 |
| `0x1dc` before `0x138` | 6968 / 41.3 |
| `0x2c4` before `0x78` | 7104 / 23.4 |
| `0x1c0` before `0x1cc` | 7088 / 47.4 |
| `0x1c0` before `0x1c8` | 7056 / 41.8 |
| `0x1e8` before `0x1e4` | 6944 / 34.7 |
| `0x1e8` before `0x204` | 6876 / 29.4 |
| `0x1e8` to end | 6944 / 31.9 |

Exhaustive ±2/±3 position moves (806 evals): no 7072 result above 47.7.
1200 random moves on the `0x98` if/else body (7016 base): no 7072 result ≥ 47.8.
`-O1/-O2/-O3/-Os` instead of `-Oz`: function identical (7072 / 47.7).

---

## 6. Literal-pool order (new evidence)

The original's first literal pool starts at `0xd1308`; ours at `0xd1318` (16 bytes
later). The pool order is the order of first use of each constant, so it is a direct
read of the original source order. First entries:

```
orig 0xd1308: 9 11 5 253 | 0x13f38c bf00bf00 1 21 78 151 15 18 2 189 5 70 70 206
              68 0 35 184 174 84 84 217 23 163 23 244 -6 8 16 90 18 22 22 243 30 12
              28 7 21 42 40 52 7 7 4 42 8 9 82 140 7 15 7 125 487 341 63 90 112
              566 191 63 63 126 63 112 12 14 112 42 18 42 46 1 0.462222 1.40625 4
              20 25 11 56 112 281 562 281 118 16 16 154 140 147 42 -2 1 2 1 9 35 21
              1 92 26 40 50 28.125 14.0625 ...
ours 0xd1318: 20 25 11 9 9 35 21 1 1 21 78 151 2 189 5 70 70 206 68 0 35 184 174
              84 23 163 23 244 -6 8 16 90 0x13f356 1.40625 18 22 22 243 30 12 28 7
              21 42 40 52 7 7 4 42 8 9 82 140 7 15 7 125 487 341 63 90 112 566 191
              63 63 126 63 112 12 14 112 42 18 42 46 1 0.462222 bf00bf00 11 5 253
              21 281 118 16 16 154 140 147 42 14.0625 1.40625 28.125 1.40625 92 26
              40 50 0x13f240 bf00bf00 -2 1 2 1 7 17 5 14 0x13f16a bf00bf00 37 11 ...
```

Observations:

* the original's first four words are `9 11 5 253`; ours starts `20 25 11 9`.
  The original's `0x1c`/`0x20`/`0x24`-family constants are emitted first, which is
  consistent with the HD block's first vector group being `0x14/0x18/0x1c/0x20`
  (original group `14(4)`) — i.e. the original's source order starts with the
  `field_0x14/footerTextInset/field_0x1c/field_0x20` run, not with our
  `field_0x4/footerTextInset/field_0x1c/...` run.
* the original emits `0.462222` immediately followed by `1.40625` (`0xd1440/0xd1444`);
  ours emits `0.462222` then `bf00bf00` (`0xd1450/0xd1454`). `0.462222` is the HD
  `field_0x90` value and `1.40625` is the HD scale; the original's adjacency means the
  `field_0x90` assignment is immediately followed (in first-use order) by a
  `1.40625f`-scaled assignment.
* the original's `0xd14b8/0xd14bc` are `28.125`/`14.0625` (i.e. `20*1.40625`,
  `10*1.40625`); ours has the same two values but at `0xd1488/0xd148c` in the other
  order, and repeats `1.40625` at `0xd1490/0xd1494`. This is the `0x2f8`/`0x2f4`
  family (`281.25f`/`112`) and is a second independent order signal.

The pool is therefore a usable oracle for the remaining HD-block order work: a
candidate source order can be rejected before building if its first-use sequence does
not reproduce `9 11 5 253 ...` and the `0.462222, 1.40625` adjacency.

---

## 7. Why the function must stay exactly 7072 bytes

`byte_exact` compares raw bytes, so every function after `Layout::Layout` in `.text`
inherits its size. Measured: the `0x3f8` restructure (`7024 B`, pct 48.0) drops
`byte_exact` from **4498 to 2454** (`linked_exact` stays 4498). The 48-byte shrink
shifts ~2044 downstream functions and destroys their byte identity.

Consequence: any landable edit must keep `usize == 7072`. The `0x3f8` restructure is
the right *shape* (tail 49.4 → 49.9 %) but is 48 bytes short; the missing 48 bytes are
exactly the code that the 7 missing `vstr` round-trips would add (≈4 instructions /
16 B each). Landing it requires reproducing at least three of those conversions in the
same pass.

Also note `docs/IDENTICALITY.md` states inline that the `Layout::Layout` body is
size-exact 7072 B; if a future pass changes it, that doc (and
`docs/VALIDATION.md`) must be updated in the same commit.

---

## 8. Reproduction

Scratch (all under `/dev/shm/w4221`):

* `score.py` — official-verifier score of `_ZN6LayoutC1Ev`/`C2Ev` from
  `cmake-build-match/libgof2hdaa.so` (`tools/verify`, `autojunk=False`).
* `harness.py` — rewrites only the `Layout::Layout()` body from `units.json` in a
  given unit order, rebuilds (`link_so -j8`, ~0.5 s), scores, restores.
* `split_body.py` — regenerates `units.json` (204 units, one per assignment /
  if-else chain) from the pristine `Layout.cpp`. **Re-run after any
  `git checkout src/game/ui/Layout.cpp`.**
* `emu2.py` — Thumb/VFP emulator over the original (`EMU_SO` env var), prints every
  store to `this` as `offset size value int float`.
* `trace.py` — per-store-group trace of a `.so`.
* `regions.py` — per-region fuzzy match (hd/n9/def/mid/tail).
* `floatify.py`, `conv.py` — ternary/if-else and float-literal source transforms.
* `buildvar.py`, `scanA.py`, `scanAE.py`, `try*.py` — the variant sweeps in §5.

Commands:

```sh
# gate
cmake --build cmake-build-match --target check -j8

# score the current body
python3 /dev/shm/w4221/score.py

# original store table for one flag combination
python3 /dev/shm/w4221/emu2.py _ZN6LayoutC1Ev iPadHD=1 n9=0 iPad=0 iPadLarge=0 retinaDisplay=0

# store-group sequence
python3 /dev/shm/w4221/trace.py <so>

# region match
python3 /dev/shm/w4221/regions.py <so>
```

Note: this worktree's `cmake-build-match` was configured with
`-DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
and `-DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs` because the
preset's default relative path does not exist from this worktree.

---

## 9. Remaining work (ranked)

1. **Recover the HD-block source order that (a) sinks `0x1e8` into the mid block and
   (b) reproduces the original's `2c0/2c4`, `94/a4`, `c0/b0`, `d0/dc/e4/fc/104` group
   split.** The literal-pool first-use sequence of §6 is the cheapest oracle; the
   original's first group is `14(4)` and its first pool words are `9 11 5 253`.
2. **Reproduce the 7 missing `vstr` round-trips.** The conversion must be applied to
   an opaque value (member load or non-constant phi), not a constant select; see §4.2.
   `0x3f8` is the proven case (`(int)(float)((rewardBoxY + 17) << retina)`).
   Note that `A + E + wrap(0x158/0x15c/0x170)` folds (7024 / 48.0) and
   `A + E + wrap(0x150)` is 7040 / 47.7 — the conversion only survives when the
   converted value is opaque, which none of those three ternaries are.
3. **Land the `0x3f8` restructure together with ≥3 recovered `vstr`s** so the body is
   exactly 7072 B; alone it is 7024 B and drops `byte_exact` to 2454.
4. The 6 extra `vstr`s need their HD branches as plain integers, but that edit is not
   local: it perturbs the HD SLP grouping and the pool (§5.2). Do it as part of the
   full HD-block order recovery, not alone.

---

## 10. Gate state

Before and after this pass (no code change): `RATCHET PASS` —
`byte_exact 4498/4524`, `linked_exact 4498/4524`, `size-exact 4524/4524`,
`stub_zero_size 0`, `missing 0`, `wrong_type 0`, `extra 0`, `data_exact 843/843`,
`imports 207`, `lint CLEAN`, `drift 0`.
Target function: `osize 7072`, `usize 7072`, `pct 47.7`.
