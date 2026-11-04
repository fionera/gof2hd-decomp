# Layout::Layout C1/C2 — the `(int)(float)` round-trips come from a *shared int
# variable used for two fields*, and the 6800-byte body is 3.0 pp better than the
# landed 7072-byte body (w5301, wave 105)

Worker **w5301**, branch `wave105-w5301`, base `835748c18`.
Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` at ELF VMA `0xd1028`, original size
**7072 B**. Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`.comment` confirms clang 7.0.2 r328903, same as the build cache).

Continues `layout-layout-c1-c2-rea-block-tree-w5301.md` (the immediately
preceding note), `layout-layout-c1-c2-literal-pool-constants-wf102.md` (the
landed 47.7724741448 state) and `layout-layout-c1-c2-ios-order-wf101.md`.

## Result

**No landable improvement found.** The ctor stays at **7072 B /
47.77247414478918 %** (official unrounded `fscore.py`, `autojunk=False`).
`src/game/ui/Layout.cpp` is byte-identical to the base (`md5
8014b6fc9f2ce06fe63de151f501bf50`). Gate before == after (`check` on the
pristine tree): `byte_exact` 4500/4524, `linked_exact` 4500/4524,
`size_exact` 4524/4524, `data_exact` 843/843, imports 207, lint CLEAN,
drift 0, `RATCHET PASS`.

This pass contributes three new, measured facts:

1. **The mechanism of the `(int)(float)` round-trips is now proven**: the
   original source uses **one int variable for two fields**, storing it
   directly to one and through a `float` temporary to the other.  That is
   why clang 7.0.2 cannot fold `(int)(float)v` — `v` has a second use.
   §1.
2. **The 6800-byte all-ternary body scores 50.79 %** (vs 47.77 % for the
   landed 7072-byte body).  The 272-byte difference is exactly the missing
   round-trips + extra branches.  §2.
3. **The identical-value field pairs are enumerated** (the candidates for
   the shared variable).  §1.3.

## 1. The round-trip mechanism (new)

### 1.1 The smoking gun

`0xa8` and `0xac` have bit-identical values for all 32 flag combinations
(`_work/reconstructions/layout_ctor/lay_orig.json`).  The original code at
`d19c0`:

```asm
d19c0: movs   r2, #82            ; 0x52
d19c2: cmp.w  r8, #0             ; retinaDisplay
d19c6: it     eq
d19c8: moveq  r2, #41            ; 0x29
d19ca: vmov   s0, r2             ; int -> float register
d19ce: str.w  r2, [r0, #172]     ; 0xac = r2            (plain int store)
d19d2: cmp.w  fp, #0             ; n9
d19d6: vcvt.f32.s32 s0, s0
d19da: vcvt.s32.f32 s0, s0
d19de: vstr   s0, [r0, #168]     ; 0xa8 = (int)(float)r2
```

The same `r2` value is stored **twice**: once as an int (`0xac`) and once
through `(int)(float)` (`0xa8`).  This is the source shape

```cpp
int v = Globals::iPadLarge ? 122 : (Globals::n9 ? 61 : ...);
this->field_0xac = v;
this->field_0xa8 = (int)(float)v;
```

Clang 7.0.2 `-Oz` keeps the round-trip because `v` is live across both
stores.  A single-use `float f = v; field = (int)f;` is folded to an int
select (verified with a standalone TU and in the real ctor: code-identical,
7072 B / 47.7724741448).

The same pattern explains every int-field `vstr` in the original:
`0x60`, `0x68`, `0x6c`, `0x70`, `0x2c8`, `0x2d0`, `0x9c`, `0xa8`, `0x138`,
`0x150`, `0x158`, `0x15c`, `0x160`, `0x170`, `0x1e4`, `0x3e0`, `0x3f8`
(17 fields).  The genuine float fields are `0x90`, `0xdc`, `0xe0`, `0xe4`,
`0xe8`, `0x1e0`, `0x2f8`.

### 1.2 The current build's vstr set is wrong in 14 fields

| | offsets |
|---|---|
| original-only (missing) | `0x9c 0x138 0x150 0x158 0x15c 0x160 0x170 0x1e4 0x3f8` |
| current-only (extra) | `0x20 0x10c 0x148 0x204 0x3e8 0x1f4` |

The six "extra" are fields whose HD arm is written as `N * 1.40625f` but
whose true HD value is an int (`0x20`→7, `0x10c`→1, `0x148`→2, `0x204`→12,
`0x3e8`→140, `0x1f4`→22).  Writing the int literal instead of the float
product removes the `vstr`, but every single edit was measured by wf101 and
lowers the fuzzy score (e.g. `0x204`→12 stays at 7072 B but drops to
47.4354; `0x1f4`→22 gives 7056 B / 47.7404).

### 1.3 Identical-value field pairs (shared-variable candidates)

From `lay_orig.json` over the six flag combinations
(`iPadHD,n9,iPad,iPadLarge,retinaDisplay,default`), the round-trip fields
that share a value tuple with another field are:

| round-trip field | identical partner(s) |
|---|---|
| `0x70` | `0x5c` |
| `0xa8` | `0xac` |
| `0x150`, `0x1e4` | `0x1d8`, `0x210` |
| `0x3e0` | `0x10c`, `0x124`, `0x128`, `0x1bc`, `0x230`, `0x234`, `0x250` |

`0x5c/0x70`, `0xa8/0xac`, `0x20/0x24`, `0x128/0x124/0x10c`,
`0x180/0x188`, `0x20c/0x218/0x220`, `0x230/0x234` are adjacent in the
6800-byte source order.

## 2. The 6800-byte body is 3.0 pp better (new)

The wave-89 all-ternary body (`git show 4ce92d572:src/game/ui/Layout.cpp`,
ctor only) compiles to **6800 B / 50.788515972503035 %** — 3.02 pp above
the landed 7072-byte body.  Instruction counts: original 2529 (2491 code +
38 pool), 6800 body ≈ 2430.  The 272-byte gap is ~100 instructions, i.e.
the 17 round-trips (≈12 B each) plus the extra branch structure.

The 6800 body's `vstr` set is also 27 words but at
`0x28 0x60 0x68 0x6c 0x70 0x1e0 0x2c8 0x2d0 0x8c 0x90 0x9c 0xa8 0xe4 0xdc
0xe0 0xe8 0x124 0x10c 0x138 0x148 0x15c 0x168 0x1a8 0x3e0 0x3f4 0x1f4
0x2f8` — i.e. the same six "wrong" round-trips plus `0x28 0x124 0x3f4`.

## 3. What was tried this pass (all measured)

All sizes are real `link_so` sizes; ratios are the official unrounded
`fscore.py` unless marked *fast* (object-level).

### 3.1 Exhaustive single-unit moves of the 14 disputed fields

Every one of the 14 fields (`0x1e0 0x2c0 0xac 0xa8 0x9c 0x138 0x150 0x158
0x15c 0x160 0x170 0x1e4 0x3e0 0x3f8`) moved to every one of the 204
positions: **2856 builds, no size-7072 position beats 47.7724**.  (The
previous note covered `0x94 0x140 0x144 0x204 0x23c 0x2bc 0x90 0x98`; the
two sets are disjoint except `0x204`.)

Notable individual results: `0x1e0`→136 gives 6984 B / 40.63,
`0x1e0`→27 gives 7116 B / 23.40, `0x2c0`→27 gives 7052 B / 21.12,
`0x2c0`→136 gives 7056 B / 41.13.

### 3.2 Pairwise swaps among the 14 disputed fields

All `C(14,2) = 91` swaps: no size-7072 swap beats 47.7724.

### 3.3 Float-literal forms (`(int)(float-literal-ternary)`)

Rewriting a field as `field = (int)(cond ? 100.0f : 50.0f)` is folded by
clang 7.0.2 to an int select.  Measured: `0x9c`, `0x158`, `0x15c`, `0x170`,
`0x3f8` are code-identical (7072 / 47.7725); `0x160` gives 7088 / 47.6437;
`0x1e4` gives 7072 / 41.0817.

### 3.4 Localized shared-int-variable experiments (the §1 shape)

Applying the real `int v; fieldA = v; fieldB = (int)(float)v;` shape to the
landed body:

| pair | `usize` | raw % |
|---|---:|---:|
| `0x5c`/`0x70` (4 statements apart) | 7036 | 16.7698 |
| `0xac`/`0xa8` (adjacent) | 7100 | 18.6282 |

and to the 6800-byte body:

| pair | `usize` | raw % |
|---|---:|---:|
| `0x5c`/`0x70` | 6804 | 31.0401 |
| `0xac`/`0xa8` | 6920 | 28.2989 |

The shared int variable forces a callee-saved register and changes the
register allocation of the *whole* function, so a partial application is
much worse than the flat body.  This is the strongest evidence that the
original source must be rewritten **as a whole**, not patched.

### 3.5 The all-ternary + all-if/else variants

* landed body with all 14 if/else blocks converted to ternaries:
  7056 B / 45.0945;
* 6800 body with all 14 if/else blocks restored: 6904 B / 42.7711;
* 6800 body + `0x1f4` int + `0x160` float-literal: 7080 B / 47.5736;
* 6800 body + `0x148` int + `0x160` float-literal: 7080 B / 47.4145.

## 4. Why the residue is (still) irreducible by local edits

The landed body is a **flat ternary chain**, so clang 7.0.2 `-Oz`:

1. hoists the `iPadHD` arm into one straight-line HD block (matches the
   original's HD block shape);
2. if-converts each non-HD per-field chain to a select;
3. folds `(int)(float)(select)` to an int select because the value has a
   single use.

The original is a **block tree with shared int variables**; the round-trips
survive because each int is used by two fields.  Every local edit either
(a) is folded away, or (b) changes the function size away from 7072, or
(c) changes the global register allocation and loses far more than it
gains.

## 5. Next hypothesis

A from-scratch rewrite is required.  Recommended shape (from the Android
REA `/dev/shm/w5301/rea_layout_c1.c` and the block map in
`layout-layout-c1-c2-oracle-block-structure-wf301.md`):

```cpp
if (Globals::iPadHD) {
    // 137 straight-line int/float assignments (HD block, iOS order)
} else {
    if (Globals::n9) {
        // 34 straight-line assignments
    } else {
        // 34 retina/default assignments
    }
    // 101 mid assignments (flat ternaries with n9 tests)
}
// 67 tail assignments
```

and, for each of the 17 round-trip fields, declare the int value **once**
and use it for the paired field as well:

```cpp
int v = ...;                 // e.g. 0xa8/0xac
this->field_0xac = v;        // int store
this->field_0xa8 = (int)(float)v;   // round-trip store
```

The pairs to use are the identical-value pairs in §1.3.  The expected size
is ≈7072 B (6800 + 17×12 + branch structure).  Do **not** land the 6800
body as-is: it breaks `size_exact` (6800 != 7072).

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5301
cmake --build cmake-build-match --target check -j6          # RATCHET PASS
python3 /dev/shm/w5301/fscore.py                            # 47.7724741448, 7072 B
python3 /dev/shm/w5301/search_new.py                        # 14-field × 204 moves
python3 /dev/shm/w5301/search_pair2.py                      # 91 disputed pairs
```

Scratch (`/dev/shm/w5301`): `fscore.py`, `fastscore.py`, `trybody.sh`,
`moveunit.py`, `floatlit.py`, `ifelseize.py`, `ternarize.py`,
`search_new.py`, `search_pair2.py`, `Layout.cur.cpp` (pristine ctor),
`Layout.6800.cpp` (wave-89 all-ternary body), `rea_layout_c1.c` (REA
pseudocode), `orig.asm` / `ours.asm` / `cur6800.asm` (disassembly).
