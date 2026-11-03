# Layout::Layout C1/C2 — the 27 `vstr` stores classified by conversion shape,
# and which of them the current body can reproduce (w4401, wave 94)

Status: analysis-only pass (no code change). Worktree `wt/w4401`, branch
`wave94-w4401`, base `f262bd4b`. Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` at ELF VMA `0xd1028`, size **7072 B**,
image base `0x10000`).

Continues `layout-layout-c1-c2-hd-block-order-w4221.md` (wave 92),
`layout-layout-c1-c2-hd-vector-w4101.md`, `layout-layout-c1-c2-midtail-vstr-w4001.md`.

## Result

`_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` stay at **47.655 % / 7072 B** (the
verifier prints `47.7`). No landable improvement was found; the gate is
unchanged:

| metric | before | after |
|---|---:|---:|
| `byte_exact` | 4499 / 4524 | **4499 / 4524** |
| `linked_exact` | 4499 / 4524 | **4499 / 4524** |
| `size_exact` | 4524 / 4524 | **4524 / 4524** |
| `stub_zero_size` / `missing` / `wrong_type` / `extra` | 0 | **0** |
| `data_exact` | 843 / 843 | **843 / 843** |
| imports | 207 | **207** |
| lint / drift | CLEAN / 0 | **CLEAN / 0** |

`RATCHET PASS`.

Four new results:

1. **The 27 `vstr` stores split into exactly three conversion shapes**, not
   two: 14 × `(int)(float)int` (`vmov sX,rN; vcvt.f32.s32; vcvt.s32.f32;
   vstr`), 6 × `(int)float` (`vcvt.s32.f32; vstr`), 7 × real `float` members
   (§2). The current body has the same 7 real-float stores but **17**
   `(int)(float)int` and only **2** `(int)float`.
2. **One of the 20 "common" `vstr` fields is a shape mismatch, not a match**:
   `0x168` is `(int)(float)int` in our body and `(int)float` in the original
   (§2.3). The earlier notes' "20 common" is really 19 common + 1 shape-swapped.
3. **Only 3 of the 7 missing `vstr` stores are reproducible from source**:
   `0x150` and `0x1e4` with all-float ternary branches, `0x3f8` with the
   w4221 restructure plus `(int)(float)` (§3). `0x9c`, `0x158`, `0x15c`,
   `0x170` are **unreachable from any source form tried** — clang folds their
   `fptosi` because our statement order if-converts the whole chain into a
   pure constant `select` (§4). The original's block structure keeps a
   `phi float` with a `select` incoming, which InstCombine cannot fold.
4. **The best 7072-byte variants found are all ≤ 47.655 %** (§5). The best
   *overall* variant is `3f8 restructure + 0x9c ternary` at **7024 B /
   47.965 %**, 48 B short.

## 1. Tooling used

All measurements use the official verifier (`tools/verify/verify.py`,
`autojunk=False`) on the linked `cmake-build-match/libgof2hdaa.so`; the
per-function score is re-computed unrounded for tie-breaking. Scratch:
`/dev/shm/w4401` (`fscore.py`, `vstrset.py`, `seq3.py`, `stores.py`,
`edits.py`, `conv.py`, `sweep.py`, `sweep2.py`, `rsearch.py`).

A full `link_so` after editing only `src/game/ui/Layout.cpp` takes **0.85 s**,
so a variant can be built + scored in ~1 s.

## 2. The 27 `vstr` stores, classified by conversion shape

`seq3.py` walks the original's disassembly, tracks which conversion last
defined each `s` register, and classifies the store. Result (original, address
order):

| shape | offsets |
|---|---|
| `(int)(float)int` — `vmov sX,rN; vcvt.f32.s32; vcvt.s32.f32; vstr` | `60 68 6c 70 2c8 2d0 a8 138 158 15c 160 170 3e0 3f8` (14) |
| `(int)float` — `vcvt.s32.f32; vstr` | `8c 9c 150 168 1a8 1e4` (6) |
| real `float` member — no `vcvt` | `90 dc e0 e4 e8 1e0 2f8` (7) |

The current body (`seq3.py` on our `.so`):

| shape | offsets |
|---|---|
| `(int)(float)int` | `20 60 68 6c 70 2d0 2c8 a8 124 138 148 204 160 168 3e0 3e8 1f4` (17) |
| `(int)float` | `8c 1a8` (2) |
| real `float` | `1e0 90 e4 dc e0 e8 2f8` (7) |

Set difference (26 vs 27):

* **original-only (7)**: `9c 150 158 15c 170 1e4 3f8`
* **ours-only (6)**: `20 124 148 204 3e8 1f4`
* **common but shape-swapped (1)**: `168` — ours `(int)(float)int`,
  original `(int)float`.

### 2.1 The six "extra" `vstr` fields

Our source writes their HD branch as a float product (`N * 1.40625f`), which
makes the whole ternary `float`-typed and forces the non-HD path through
`int→float→int`. The original's HD branch is the plain truncated integer, so
its ternary is `int`-typed and the store is a plain `str`/`vst1`:

| offset | our HD expr | HD value | original HD literal |
|---|---|---:|---:|
| `0x20`  | `5 * 1.40625f`   | 7   | `7`   |
| `0x124` | `1 * 1.40625f`   | 1   | `1`   |
| `0x148` | `2 * 1.40625f`   | 2   | `2`   |
| `0x1f4` | `16 * 1.40625f`  | 22  | `22`  |
| `0x204` | `9 * 1.40625f`   | 12  | `12`  |
| `0x3e8` | `100 * 1.40625f` | 140 | `140` |

Measured individually on the pristine 7072 B body (exact pct):

| change | size | pct | `vstr` count |
|---|---:|---:|---:|
| `0x20` → `7` | 6968 | 20.888 | 20 |
| `0x124` → `1` | 7008 | 42.562 | 25 |
| `0x148` → `2` | 7056 | 47.504 | 25 |
| `0x1f4` → `22` | 7056 | 47.623 | 25 |
| `0x204` → `12` | 7064 | 47.396 | 25 |
| `0x3e8` → `140` | 7076 | 42.354 | 25 |

(The `0x20` edit also drops `0x1e0`, `0x1e4`, `0x2f8`, `0x2c8`, `0x2d0`, `0x3e0`
out of the `vstr` set — it perturbs the whole HD SLP grouping, which is why
its size delta is −104 B rather than −8 B. This is the same non-locality
w4221 §5.2 reported.)

### 2.2 Where the six extra `vstr` stores land in the original

`seq_orig.txt`/`seq2.py` recovers all 177 store groups of the original in
address order (appendix A). The six fields are all inside integer vector /
`strd` groups there:

```
0xd15ee  strd 0x20  (8)      ; 0x20 + 0x24
0xd227e  vst1 0x204 (16)     ; 0x204..0x213
0xd2464  vst1 0x204 (16)
0xd23a4  strd 0x204 (8)
```

so they are unambiguously `int`-typed in the source.

### 2.3 `0x168` is a shape mismatch

`0x168` is the only offset present in both `vstr` sets with a *different*
conversion shape:

* original `d1e56: vcvt.s32.f32 s0,s0` → `d1e5e: vstr s0,[r0,#360]` — a single
  `fptosi`, i.e. the source ternary is already `float`-typed
  (`(int)(H ? 25*1.40625f : (N ? 37 : (R ? 50 : 25)))`).
* ours emits `vmov s0,rX; vcvt.f32.s32; vcvt.s32.f32; vstr` — the ternary was
  constant-folded to `int` first and then round-tripped.

So the correct source for `0x168` is the *current* expression, but the current
statement order still produces the wrong shape. This is a second, independent
instance of the block-structure problem in §4 (the shape, not the value,
differs).

## 3. The 7 missing `vstr` stores: exact original codegen and reproducibility

### 3.1 Ground-truth codegen (original)

```
; 0x9c  — (int)float
d1972: mov.w  r1,#840                      ; 0x98 default
d1976: addw   r3,pc,#3812                  ; r3 = &100.0f (pool d285c)
d197a: cmp.w  r8,#0                        ; retinaDisplay
d197e: it eq / d1980: moveq.w r1,#420
d1984: it eq / d1986: addeq  r3,#4         ; r3 = &50.0f (pool d2860)
d198a: vldr   s0,[r3]
d199e: vcvt.s32.f32 s0,s0
d19a8: vstr   s0,[r0,#156]                 ; 0x9c
; (n9 path: d1964 vldr s0,[pc,#-104] -> pool d1900 = 75.0f, shared fptosi at d199e)

; 0x150 — (int)float
d1cd4: vmov.f32 s4,#16                     ; 4.0
d1cec: vmov.f32 s0,#32                     ; 8.0
d1d1e: vmoveq.f32 s0,s4
d1d2a: vcvt.s32.f32 s0,s0
d1d32: vstr   s0,[r0,#336]                 ; 0x150

; 0x158 — (int)(float)int
d1da6: vmov   s0,r4                        ; r4 = int phi (50 / 28 / 14 / 13 / ...)
d1db0: vcvt.f32.s32 s0,s0
d1db4: vcvt.s32.f32 s0,s0
d1db8: vstr   s0,[r0,#344]                 ; 0x158

; 0x15c — (int)(float)int
d1dd8: vmov   s0,r4
d1dde: vcvt.f32.s32 s0,s0
d1de2: vcvt.s32.f32 s0,s0
d1de6: vstr   s0,[r0,#348]                 ; 0x15c

; 0x170 — (int)(float)int
d1f24: vmov   s0,r1
d1f2c: vcvt.f32.s32 s0,s0
d1f30: vcvt.s32.f32 s0,s0
d1f34: vstr   s0,[r0,#368]                 ; 0x170

; 0x1e4 — (int)float
d20e8: vmov.f32 s0,#32                     ; 8.0
d20f6: vmoveq.f32 s0,s4                    ; 4.0
d21a6: vmov.f32 s0,#24                     ; 6.0  (other path)
d21e2: vcvt.s32.f32 s0,s0
d21fe: vstr   s0,[r0,#484]                 ; 0x1e4

; 0x3f8 — (int)(float)int
d2514: add.w  r2,r1,#17                    ; r1 = this->rewardBoxY (member load)
d2518: lsl.w  r2,r2,r8                     ; << retinaDisplay
d251c: vmov   s0,r2
d2520: vcvt.f32.s32 s0,s0
d2524: vcvt.s32.f32 s0,s0
d252c: vstr   s0,[r0,#1016]                ; 0x3f8
```

### 3.2 Reproduction in our body

| field | faithful source form | emitted? | size | pct |
|---|---|---|---:|---:|
| `0x150` | all-float branches `5.0f : 6.0f : 8.0f : 4.0f` | **yes** (`vmov.f32 s0,#32; vmoveq.f32 s0,s4; vcvt.s32.f32; vstr` — instruction-for-instruction identical to the original) | 7080 | 47.377 |
| `0x1e4` | all-float branches | **yes** | 7072 | 41.009 |
| `0x3f8` | w4221 restructure + `(int)(float)((rewardBoxY+17)<<retina)` | **yes**, but as `vmov r4,s0; str` (4 B larger than the original's `vstr`) | 7040 | 46.740 |
| `0x9c` | int/float mixed ternary, all-float ternary, if/else, float local, `(int)(float)` wrap | **no** | 7064–7072 | 47.564–47.655 |
| `0x158` | `(int)(float)(ternary)`, float local, if/else, all-float | **no** | 7072 | 47.655 |
| `0x15c` | same | **no** | 7072 | 47.655 |
| `0x170` | same | **no** | 7072 | 47.655 |

The `0x150` result is the strongest evidence that the source forms are right:
our emitted sequence for that field is bit-for-bit the original's (only the
addresses differ), and the +8 B is exactly the cost of the round-trip.

## 4. Why `0x9c`/`0x158`/`0x15c`/`0x170` fold, and `0x150` does not

The fold is `InstCombine`'s `fptosi(sitofp(x)) -> x` /
`fptosi(select c, f1, f2) -> select c, fptosi f1, fptosi f2`. It fires when
the `fptosi` operand is a constant select/phi of constants. It does **not**
fire when the operand is a `phi` with a non-constant incoming.

Emitted LLVM IR (`-emit-llvm -S`, same flags):

* `0x150` (float branches) — survives:
  ```
  %476 = select i1 %473, float 4.0, float 8.0
  %478 = phi float [ 6.0, %430 ], [ %476, %465 ]
  %479 = fptosi float %478 to i32
  store i32 %479, i32* %480     ; 0x150
  ```
  The n9 test became a *branch* (phi incoming `6.0`); the retina test became a
  *select*, so the phi has a non-constant incoming and the `fptosi` is kept.
* `0x9c` (any form tried) — folds:
  ```
  %271 = select i1 %270, i32 50, i32 100
  %272 = select i1 %6,   i32 %271, i32 75
  store i32 %272, ...           ; 0x9c
  ```
  Here clang if-converted **both** the n9 and the retina tests into `select`s,
  so the value is a pure constant `select` and the `fptosi` is folded away.

The original's `0x9c` block structure is the one that survives: the n9 test is
a real branch (`d1960: beq.n d1972`) and the retina test is an `it eq`
select (`d197e/d1986`), i.e. `phi float [75.0, n9], [select(retina,100.0,50.0),
retina]` then one `fptosi` at the merge (`d199e`).

The same applies to `0x158`/`0x15c`/`0x170`: the original keeps the value in a
register `r4`/`r1` across a *branchy* chain (`d1d4e..d1da6`, `d1efc..d1f24`)
and converts once at the merge. Our hill-climbed statement order if-converts
those chains into `select`s, so `fptosi(sitofp(select))` folds to the int
select and the `vstr` disappears. **The conversion is therefore a function of
the statement order, not of the expression text** — which is why every
expression-only rewrite of those four fields produces byte-identical code
(`7072 / 47.655`, `vstr` count 26, for all of `(int)(float)`, float-local,
if/else and all-float forms).

## 5. Measured variants (official verifier)

Baseline: **7072 B / 47.655 %**, 26 `vstr`.

| variant | size | pct | `vstr` |
|---|---:|---:|---:|
| current body | 7072 | 47.655 | 26 |
| `3f8` restructure (w4221 §5.3, no float) | 7024 | 47.845 | 26 |
| `3f8` restructure + `0x9c` ternary | **7024** | **47.965** | 26 |
| `3f8` restructure + `(int)(float)` | 7040 | 46.740 | 26 |
| `3f8` restructure + `0x150` float | 7032 | 47.606 | 27 |
| `3f8` restructure + `0x9c` ternary + `0x150` float | 7032 | 47.446 | 27 |
| `3f8` restructure + `0x1e4` float | 7024 | 40.455 | 27 |
| `0x150` float alone | 7080 | 47.377 | 27 |
| `0x1e4` float alone | 7072 | 41.009 | 27 |
| `0x9c` if/else → ternary | 7064 | 47.564 | 26 |
| `0x9c` faithful mixed (`70 : (75.0f : (R ? 100.0f : 50.0f))`) | 7064 | 47.564 | 26 |
| `0x158`/`0x15c`/`0x170` any float form | 7072 | 47.655 | 26 |
| `field_0x168` ternary → if/else | 7056 | 47.702 | 26 |
| `rewardBoxWidth` ternary → if/else | 7048 | 47.682 | 26 |
| `field_0x168` + `field_0x29c_buttonRowGap` if/else | 7072 | 47.615 | 26 |

Exhaustive single ternary→if/else sweep over all 186 convertible assignments,
on the pristine body and on the `3f8+0x9c` body: **no 7072 B result above
47.655 %**. 160 random 1–4-way combinations of the 37 size-changing
conversions on the `3f8+0x9c` body: no 7072 B hit at all.

So the residual is *not* reachable by expression edits or by local statement
reordering — it needs the original's branch/select split, which is a
whole-function property of the statement order.

## 6. Exact remaining target

For a future pass to land, in priority order:

1. **Recover the statement order that makes `0x9c`/`0x158`/`0x15c`/`0x170`
   keep their `fptosi`.** The necessary condition is proven: the `fptosi`
   operand must be a `phi float` with a `select` incoming. In the original,
   for `0x9c` that is `phi [75.0, n9], [select(retina,100.0,50.0), retina]`;
   for `0x158`/`0x15c`/`0x170` it is `phi [const, n9], [select(retina,hi,lo),
   default]` with the value carried in `r4`/`r1` across the branchy chain.
   Any source order that if-converts *both* tests into selects loses the
   store (measured on every form tried).
2. **Land `3f8` restructure + `0x9c` ternary (7024 B / 47.965 %) together with
   ≥ 3 recovered `vstr` round-trips** so the body is exactly 7072 B. The
   48 B gap is 3 × 16 B (a `(int)(float)int` round-trip in the original is
   `vmov + vcvt.f32.s32 + vcvt.s32.f32 + vstr` = 16 B).
3. **Fix the `0x168` shape** (`(int)(float)int` → `(int)float`); it is
   independent of the seven missing stores and follows from the same
   block-structure fix.
4. **Convert the six "extra" `vstr` HD branches to plain integers**
   (`0x20→7, 0x124→1, 0x148→2, 0x1f4→22, 0x204→12, 0x3e8→140`) only as part
   of the full HD-block order recovery — individually they perturb the SLP
   grouping and lose 5–27 points (§2.1).

## Appendix A — the original's 177 store groups in address order

Format `offset:kind<bytes>`; `V`=`vst1.32` (4×i32), `d`=`strd` (2 words),
`s`=`str` (1 word), `v`=`vstr` (1 word). Recovered by `seq2.py`
(`vst1` base registers are tracked through `add.w rX,r0,#off`).

```
14:V16 4:V16 24:V16 34:V16 44:V16 54:V16 64:V16 2c8:V16 7c:V16 74:d8 2c0:d8 8c:d8
a4:d8 c0:V16 b0:V16 ec:V16 294:V16 2a4:V16 108:V16 118:V16 dc:d8 e4:d8 fc:d8 2b4:d8
128:V16 140:V16 150:V16 160:V16 170:V16 180:V16 190:V16 1a0:V16 1b0:V16 1c0:V16 1d0:V16 138:d8
74:d8 14:V16 4:V16 24:V16 34:V16 44:V16 54:V16 64:V16 4:d8 18:d8 20:d8 2c:s4
34:s4 3c:s4 44:s4 40:s4 48:s4 54:s4 50:s4 58:s4 5c:V16 5c:s4 5c:s4 60:v4
64:d8 64:s4 64:s4 68:v4 6c:v4 70:s4 70:v4 74:d8 2c8:v4 2d0:v4 2d4:d8 7c:V16
7c:d8 84:d8 8c:v4 90:v4 9c:v4 a8:v4 a0:d8 d0:d8 d8:d8 e0:d8 fc:d8 2b4:d8
b0:V16 c0:V16 ec:V16 294:V16 2a4:V16 118:V16 108:V16 128:V16 140:V16 138:d8 a0:d8 fc:d8
dc:v4 e0:v4 e4:v4 e8:v4 b0:V16 c0:V16 ec:V16 2a0:V16 108:V16 128:d8 130:d8 130:d8
138:v4 13c:V16 150:v4 154:d8 158:v4 15c:v4 160:v4 168:v4 16c:V16 18c:d8 190:d8 180:V16
198:d8 170:v4 178:d8 178:d8 178:d8 18c:d8 18c:d8 18c:d8 198:d8 1a8:v4 1e0:v4 1c0:V16
1b0:V16 1d0:V16 198:d8 1c0:V16 1b0:V16 1d0:V16 1e4:v4 234:d8 27c:d8 3e0:d8 3f4:d8 204:V16
214:V16 224:V16 23c:V16 24c:V16 25c:V16 26c:V16 24c:d8 204:d8 210:d8 23c:V16 264:V16 254:d8
234:d8 204:V16 214:V16 224:V16 23c:V16 24c:V16 25c:V16 26c:V16 3e0:v4 3f4:d8 3f8:v4 1f0:d8
288:d8 2f4:d8 2fc:d8 2f8:v4 30c:d8 30c:d8 30c:d8 30c:d8 130:d8
```

The first 12 groups are the HD block's SLP runs (identical to w4221 §3); the
`74:d8 14:V16 4:V16 ...` run is the HD block's second half.

## Appendix B — reproduction

```sh
cmake --build cmake-build-match --target check -j8          # RATCHET PASS

# per-function score (unrounded), ~1 s per variant
python3 /dev/shm/w4401/fscore.py <Layout.cpp>

# vstr set of the current build
python3 /dev/shm/w4401/vstrset.py

# classify every vstr store of a .so by conversion shape
python3 /dev/shm/w4401/seq3.py <so>

# full store-group sequence
python3 /dev/shm/w4401/seq2.py <so> > seq.txt

# store context for specific offsets
python3 /dev/shm/w4401/stores.py <so> 0x9c 0x150 0x158 0x15c 0x170 0x1e4 0x3f8
```

`fscore.py` builds `link_so` only when a source path is given, then scores
`_ZN6LayoutC1Ev` with `tools/verify/verify.py`'s own `disassemble`,
`reconcile` and `SequenceMatcher(autojunk=False)`.
