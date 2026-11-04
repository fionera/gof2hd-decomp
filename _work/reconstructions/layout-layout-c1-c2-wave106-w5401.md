# Layout::Layout C1/C2 — wave 106 (w5401): round-trip mechanism, literal-pool
# deficit, from-scratch block tree, and exhaustive 7072 B searches

Worker **w5401**, branch `wave106-w5401`, base `486f8a119`.
Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (both share the same body and score),
ELF VMA `0xd1028`, original size **7072 B**.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.

## Result

**No landable improvement found.** The ctor stays at **7072 B /
47.84860557768924 %** (official unrounded linked `fscore.py`, `autojunk=False`).
`src/game/ui/Layout.cpp` is byte-identical to the base. Gate before == after:
`byte_exact` 4500/4524, `linked_exact` 4500/4524, `size_exact` 4524/4524,
`data_exact` 843/843, imports 207, lint CLEAN, drift 0, `RATCHET PASS`.

This pass contributes five measured facts:

1. The `(int)(float)` round-trip comes from a **float local assigned from an
   int expression and then stored to an int field** (`float f = <int ternary>;
   this->X = f;`) — exactly two `vcvt`s; the `int v; ...; (int)(float)v`
   form is codegen-equivalent. §1.
2. The original has **27 `vstr` int/float stores**; the current body has 28,
   of which 6 are the wrong fields. §2.
3. The original's **code/pool split is 6188 B code + 884 B (221 words) pool**;
   ours is 6296 B code + 776 B (194 words) pool. The 48-word pool deficit is
   the single largest structural difference. §3.
4. A **from-scratch block tree** (HD / n9 / retina / mid / tail, shared `int v`
   + `float f` locals, values from `lay_orig.json`) compiles to **6560 B /
   30.42 %** — the flat-ternary body (6800 B / 50.79 %) is the better source
   shape on the fuzzy metric. §4.
5. Exhaustive 7072 B reorder/edit searches find **nothing above 47.8486**;
   the best variant overall is the adjacent swap `0xac`↔`0x98` at
   **6984 B / 48.2607 %**, which is not landable (it shifts `.text` and drops
   `byte_exact`). §5–6.

## 1. The two round-trip mechanisms (proven, standalone TU + real ctor)

Disassembly of the original at `0xd998` (offset `0x998` in the function):

```
movs   r2, #82
cmp.w  r8, #0            ; retinaDisplay
it     eq
moveq  r2, #41
vmov   s0, r2            ; int -> float register
str.w  r2, [r0, #172]    ; 0xac = r2            (plain int store)
cmp.w  fp, #0            ; n9
vcvt.f32.s32 s0, s0
vcvt.s32.f32 s0, s0
vstr   s0, [r0, #168]    ; 0xa8 = (int)(float)r2
```

Two distinct source shapes produce the two `vstr` fingerprints:

* **intphi** (two conversions): `int v = <int ternary>; this->X = (int)(float)v;`
  or equivalently `float f = <int ternary>; this->X = f;` where `X` is an int
  field. Emits `vmov sN,rM; vcvt.f32.s32; vcvt.s32.f32; vstr`.
* **floatphi** (one conversion): `this->X = (int)(<float ternary>);` i.e. the
  arms are float literals and the destination is an int field. Emits
  `vldr/vmov.f32; vcvt.s32.f32; vstr`.

`int v = cond ? A : B;` (ternary initialiser) keeps the round-trip; a
single-use `this->X = (int)(float)(cond ? A : B);` is folded to an int select
by clang 7.0.2 `-Oz`.

## 2. The `vstr` set (exact, from the original disassembly)

Genuine `vstr` stores in the original (27, one each):

```
0x60 0x68 0x6c 0x70 0x8c 0x90 0x9c 0xa8 0xdc 0xe0 0xe4 0xe8 0x138 0x150
0x158 0x15c 0x160 0x168 0x170 0x1a8 0x1e0 0x1e4 0x2c8 0x2d0 0x2f8 0x3e0 0x3f8
```

Mechanism per field (window of 4 preceding instructions contains `vcvt`):

| mechanism | fields |
|---|---|
| intphi (2 converts) | `0x60 0x68 0x6c 0x70 0x9c? 0xa8 0x158 0x15c 0x160 0x170 0x3f8` |
| floatphi (1 convert) | `0x9c 0x150 0x168 0x1a8 0x1e4` |
| genuine float field | `0x8c 0x90 0xdc 0xe0 0xe4 0xe8 0x138 0x1e0 0x2f8 0x2d0 0x3e0` |

Current body's `vstr` set (28): `0x20 0x60 0x68 0x6c 0x70 0x8c 0x90 0xa8
0xdc 0xe0 0xe4 0xe8 0x10c 0x148 0x150 0x160 0x168 0x194 0x1a4 0x1a8 0x1e0
0x1f4 0x204 0x2c8 0x2d0 0x2f8 0x3e0 0x3e8`.

* missing vs original: `0x9c 0x138 0x158 0x15c 0x170 0x1e4 0x3f8`
* extra vs original: `0x20 0x10c 0x148 0x194 0x1a4 0x1f4 0x204 0x3e8`

The six "extra" fields (`0x20 0x10c 0x148 0x1f4 0x204 0x3e8`) have HD arms
written as `N * 1.40625f` although the original stores the truncated int
(`0x20`→7, `0x10c`→1, `0x148`→2, `0x1f4`→22, `0x204`→12, `0x3e8`→140).
Rewriting them to int literals removes the `vstr` but shrinks the function
(7064 B) and lowers the fuzzy score (measured: `0x20` 46.59, `0x148` 47.58,
`0x1f4` 47.78, `0x204` 46.63, all 7064 B; `0x10c` 7008 B / 42.18).

## 3. Literal-pool / code split (new, measured from the linked `.so`)

| | original | current 7072 body |
|---|---:|---:|
| code bytes | 6188 (`0x182c`) | 6296 (`0x1898`) |
| pool bytes | 884 (221 words) | 776 (194 words) |
| normalised insn entries | 2529 | 2491 |

The original's pool has **48 more words** than ours. The extra words are small
int constants (`4 0x16 0x19 0xc 0xb 0xa 0x10e 0x168 0x92 ...`) that the
original loads with `ldr`/`vld1.64` while ours materialises with
`movs`/`movw`. In the HD block alone the original has 11 `ldr` + 9 `strd` +
2 `stmia` + 13 `movs` + 4 `str.w`; ours has 6 `ldr` + 7 `strd` + 1 `stmia` +
18 `movs` + 17 `str.w`. This is a source-shape difference (pooled constants +
pair stores), not a local edit.

The original also has **two** literal pools: `0x2d8..0x544` (620 B, HD + n9
constants, placed after the n9 block's `b.n`) and `0x182c..0x1ba0` (884 B,
the rest). Ours has one trailing pool.

## 4. From-scratch block tree (new negative result)

Generated from the REA `/dev/shm/w5301/rea_layout_c1.c` control flow:

```cpp
int v; float f;
if (Globals::iPadHD) {
    // 137 straight-line assignments (HD block, values from lay_orig.json)
} else {
    if (Globals::n9) {
        // 34 straight-line assignments
    } else {
        // 34 retina/default assignments:  v = <ternary>; this->X = v;
    }
    // 101 mid assignments:               v = <ternary>; this->X = v;
}
// 67 tail assignments:                  v = <ternary>; this->X = v;
```

with `intphi` for `0x60 0x68 0x6c 0x70 0x2c8 0x2d0 0xa8 0x158 0x15c 0x160
0x170 0x3e0 0x3f8`, `floatphi` for `0x8c 0x9c 0x138 0x150 0x168 0x1a8
0x1e4`, and `genfloat` for the genuine float fields.

| body | size | % |
|---|---:|---:|
| current landed body | 7072 | **47.8486** |
| wave-89 all-ternary body (`4ce92d572`) | 6800 | **50.7885** |
| block tree, per-field `int v` (gen5) | 6560 | 30.4187 |
| block tree, shared `int v; float f;` (gen6) | 6560 | 30.4249 |
| block tree, HD block in iOS order (gen6c) | 6560 | 30.1190 |
| shared `int v` over the whole flat body | 6760 | 37.5051 |
| shared `float v` over the whole flat body | 6744 | 38.9478 |

The block tree is 512 B short and its HD block register allocation
(`d30-d31` groups) does not match the original's (`d2-d3` groups), so the
fuzzy score collapses. The flat-ternary body is the better source shape on
this metric even though the original is a block tree.

## 5. Exhaustive 7072 B searches (all real `link_so` builds, linked scorer)

* **All 203 adjacent swaps** of the 204-unit body: best is `k=44`
  (`0xac`↔`0x98`) at **6984 B / 48.2607 %**; every other 7072 B swap is
  either exactly 47.8486 (a code no-op) or lower.
* **swap44 + one further adjacent swap** (203 builds): only one 7072 B
  candidate, `k=117`, at 46.7620 %.
* **700 random arbitrary pair swaps**: best 7072 B results are 47.8486
  (code no-ops); next 47.8088.
* **500 random 1–3 edits** (adjacent swap / intphi / floatphi / int-arm) on
  the current body: best 7072 B is 47.8486.
* **swap44 + single-field round-trip edits** (measured, linked):
  `0x3f8:intphi` 7000/46.99, `0x150:floatphi` 7068/18.17,
  `0xe0:floatphi` 7004/38.12, `0x6c/0x70:intphi` 6968/48.30,
  `0xa8:intphi` 6968/48.31. None reaches 7072 above 47.8486.
* **Full iOS reorder** of the current body (same units, iOS field order):
  6848 B / 41.7609.
* **All 14 `if/else` blocks → flat ternaries**: 7056 B / 45.1176.
* **6800 body + 13 intphi round-trips**: 6912 B / 46.5154.

## 6. Best measured variant (not landable)

`swap 44/45` (`this->field_0xac` before `this->field_0x98`):

```
before: 7072 B, 47.84860557768924 %
after:  6984 B, 48.260695721711315 %
```

It is a real +0.41 pp fuzzy gain, but 6984 != 7072: shrinking the ctor shifts
every following `.text` symbol, so `byte_exact` falls from 4500 to ~2454
(measured with the 6800 body: `FAIL byte_exact 4500 -> 2454`). It cannot be
landed without a compensating +88 B of real code, which no measured edit
provides at that score.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5401
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6     # RATCHET PASS
python3 /dev/shm/w5401/fscore.py                       # [7072, 7072, 47.84860557768924]
```

Scratch (`/dev/shm/w5401`): `fscore.py` (official unrounded linked scorer),
`harness.py` (swap ctor body → `link_so` → score), `matchmap2.py`
(difflib opcode map with original addresses), `dump3.py` (objdump with pool
markers), `exp2.py`/`expflat.py`/`exp6800.py` (per-field mechanism rewrites),
`gen5.py`/`gen6.py` (block-tree generators), `search_adj_full.py`,
`search2.py`, `randswap.py`, `randedit.py`, `reorder_ios.py`,
`body_gen6.txt` (the block-tree body), `body6800_plain.txt` (wave-89 body).
