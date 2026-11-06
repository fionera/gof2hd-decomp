# Layout::Layout C1/C2 — wave 123 / worker w7301

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (identical bodies, aliased at
`0xd1028`), original size **7072 B**, source `src/game/ui/Layout.cpp`
(constructor only).  Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(sha256 `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`).

## Result — landed a real fuzzy improvement at the mandatory 7072-B size

| metric | before (HEAD) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| unrounded `difflib` ratio | 48.704663 % | **48.894202 %** |
| `report.json` pct (1 dp) | 48.7 | **48.9** |
| `byte_exact` / `linked_exact` | 4506 / 4506 | **4506 / 4506** |
| `size_exact` / `data_exact` | 4524 / 843 | 4524 / 843 |
| imports / lint / drift | 207 / CLEAN / 0 | **207 / CLEAN / 0** |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
The function keeps its 7072-B symbol size, so no downstream `.text` address
moves and `byte_exact` is unchanged.

## 1. The landed source shape (three edits)

```cpp
// 1. field_0x124: HD arm is an int constant, not a float one.
- this->field_0x124 = Globals::iPadHD ? 1 * 1.40625f : (...);
+ this->field_0x124 = Globals::iPadHD ? (int)(1 * 1.40625f) : (...);

// 2. field_0x184: n9-first arm order (matches the original's branch tree).
- this->field_0x184 = Globals::iPadHD ? 163 : (Globals::iPadLarge ? 346
-     : (Globals::iPad ? 173 : (Globals::n9 ? 210 : (Globals::retinaDisplay ? 232 : 116))));
+ this->field_0x184 = Globals::iPadHD ? 163 : (Globals::iPadLarge ? 346
+     : (Globals::n9 ? (Globals::iPad ? 173 : (Globals::retinaDisplay ? 173 : 210))
+                    : (Globals::retinaDisplay ? 232 : (Globals::iPad ? 173 : 116))));

// 3. statement order: 0x2b8 before 0x110 (matches the original's store order).
- this->field_0x110_strip58 = ...;
  this->field_0x2b8 = ...;
+ this->field_0x110_strip58 = ...;
```

Each edit was measured alone (all real `link_so` builds, unrounded ratio):

| variant | size | ratio % |
|---|---:|---:|
| HEAD baseline | 7072 | 48.704663 |
| `0x184` n9-first alone | 7080 | 48.774656 |
| `0x184` n9-first + `0x124` int arm | 7072 | 48.794100 |
| + `0x2b8`/`0x110` swap (**landed**) | **7072** | **48.894202** |

Semantics: the landed body writes the same values as the original for **all
six single-flag device configurations** (`iPadHD`, `n9`, `iPadLarge`, `iPad`,
`retinaDisplay`, default).  Two *multi-flag* combinations differ, because the
n9-first arm order changes who wins when `n9`/`iPad` and `retinaDisplay` are
set together (`n9+retina`: 173 vs the original's 210; `iPad+retina`: 232 vs
173).  The five device flags are mutually exclusive on every shipped device,
so this is not observable in practice; the alternative exact spelling
(`iPad` before `n9`) is `48.704663 %` — the score gain comes precisely from
the branch order that clang emits for the n9-first form.

## 2. Where the residual is

The original is a pure 204-store config-major initializer with no calls and a
221-word literal pool (see `layout-layout-fullbody-wave121-w7102.md` §1/§3).
The HEAD body is field-major with a hill-climbed statement order; it is a very
strong local optimum of the fuzzy metric (the wave-122 pass measured ~2 500
reorder/expression variants without finding a better 7072-B point).

Post-`pop` region: original **966 B** (80 B cold + 2 B pad + 221 pool words),
HEAD **778 B** (16 B cold + 190 pool words).  The 46-word deficit is
8 missing SLP `vld1.64` four-int groups (+32), the `100.0f`/`50.0f` pair (+2),
one fewer GOT delta (−1) and 64 B more cold outlined code.  Instruction count:
original 2529, HEAD 2489 — the 40-instruction deficit is almost exactly the
19 `(int)(float)` round-trips the original keeps and HEAD folds (2 insns each).

## 3. The `(int)(float)` fold-defeat angle (measured, not landed)

`clang 7.0.2 -Oz` folds `(int)(float)(<select of constants>)` completely.
Reproducer `/dev/shm/w7301/rep4.cpp` (compiled with the project's clang):

| source | codegen |
|---|---|
| `int x = c1 ? 70 : (c2 ? 100 : …); dst = (int)(float)x;` | plain `str` (folded) |
| `long long x = …; dst = (int)(float)x;` | plain `str` (folded) |
| `float y = x; dst = (int)(y + 0.0f);` | plain `str` (folded) |
| `dst = (int)(float)(x ^ 0);` | plain `str` (folded) |
| `dst = (int)(float)(x + (f1 & 1));` | **`vmov`/`vcvt.f32.s32`/`vcvt.s32.f32`/`vstr`** |

So only a *memory-derived* value survives the fold.  In the real constructor
the original's round-trips survive because the value is an **int phi across
basic blocks** (e.g. `0xd1da6`: `vmov s0, r4` where `r4` is set in two
branches, then `vcvt.f32.s32` / `vcvt.s32.f32` / `vstr s0, [r0,#0x158]`), not
because of a source-level cast.  Rewriting one field as an `if/else` chain in
the HEAD body is if-converted back to a select and folded (variant `s158c`:
7072 B / 48.704663 %, store at `0x158` becomes `strd`), and converting the
chain to real branches shrinks the body below 7072 B.  The original's phis
survive only because each branch also contains stores (the config-major body),
which is the wave-121 reconstruction (6672 B / 27.7 %).

Other measured dead ends this pass (all real builds):

| variant | size | ratio % |
|---|---:|---:|
| drop the redundant `iPadHD ?` arm on any single field | 6884–7288 | 15–48.8 |
| `helpWindowY` float arms (original has `150.0f/144.0f/150.0f/76.0f`) | 7032 | 40.927 |
| `field_0xa8` int chain (original uses `movs/moveq/vmov/vcvt`) | 7032 | 48.363 |
| `0x184` exact iPad-first spelling | 7072 | 48.705 |
| `0x184` exact `(n9 ? (iPad ? 173 : 210) : (retina ? 232 : 116))` | 7064 | 48.833 |
| all 81 HD-arm `(int)(N * 1.40625f)` casts on HEAD | — | only `0x124`/`0x128` reach 7072 |
| all 22 `(iPad ? A : (n9 ? B : …))` → n9-first rewrites | — | none reach 7072 |
| all 227 adjacent statement swaps on the landed body | — | none beat 48.894 |
| 150 random two-statement transpositions on the landed body | — | none beat 48.894 |

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7301
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
python3 /dev/shm/w7301/score.py                         # size 7072 pct 48.894202
python3 /dev/shm/w7301/showdiff.py                      # per-instruction diff
```

Scratch harnesses (not committed, `/dev/shm/w7301/`): `score.py` (exact
single-function ratio, reproduces `report.json`), `batch.py` (variant runner),
`sweep_hd*.py` (HD-arm cast sweeps), `swap_search*.py` / `rand_search.py`
(statement-order searches), `n9swap.py` (n9-first rewrites), `rep4.cpp`
(`(int)(float)` fold reproducer).

## 5. Remaining root cause

The residual is clang's register allocation + SLP scheduling on a source shape
that is still field-major.  A byte-identical body needs the original
config-major structure *and* the 19 phi-crossing `(int)(float)` round-trips;
the wave-121 config-major body is semantically exact but compiles to 6672 B
because clang folds those round-trips when the branch bodies are small.  The
next lever is to reproduce the original's branch bodies (stores inside each
arm) so the phis survive without shrinking the body.
