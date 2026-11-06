# `Layout::Layout` C1/C2 — wave 124 / worker w7402

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7072 B (0x1ba0)**, end `0xd2bc8`.
Source `src/game/ui/Layout.cpp` (constructor only).
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(sha256 `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`).
Baseline = wave-123 master `6f8ee2320`.

Assignment focus: SLP/vectorisation and literal-pool placement — make clang 7
group adjacent stores into the original's 4-word `vst1.32` windows and put the
literal pools in the original order, **without changing the 7072-byte size**.

## Result — no source win

| metric | before | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| unrounded `difflib` ratio | 48.894202 % | **48.894202 %** |
| `report.json` pct (1 dp) | 48.9 | 48.9 |
| `byte_exact` / `linked_exact` | 4506 / 4506 | **4506 / 4506** |
| `size_exact` / `data_exact` | 4524 / 843 | 4524 / 843 |
| imports / lint / drift | 207 / CLEAN / 0 | 207 / CLEAN / 0 |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
No `src/` change landed; the worktree is clean apart from this note.

## 1. The residual is an SLP/code-vs-pool split, not a wrong value

Both bodies are exactly 7072 B, but the split between code and pool differs:

| | original | baseline (HEAD) |
|---|---:|---:|
| code region (`0x1028` → pool) | **6188 B** | **6312 B** |
| literal-pool words | **221** | **190** |
| instructions | **2529** | **2490** |
| `vld1.64` | **86** | **77** |
| `vst1.32` | **81** | **70** |
| `stmia` | **8** | **3** |
| `strd` | **62** | **51** |
| `vmov` | **53** | **51** |
| `vcvt` | **37** | **41** |

The original is 124 B *smaller* in code and 124 B *larger* in pool: it
vectorises eleven more 4-word store windows (and five more `stmia`) that the
current field-major body materialises with `movw`/`strd`.  The 39-instruction
deficit is mostly the missing `vld1`/`vst1` pairs plus the `(int)(float)`
round-trips documented in wave-119/121.

Pool alignment is almost total mismatch:

* aligned-from-end (both pools end at `0xd2bc8`): **10 / 190** words equal;
* `difflib` over the whole pool: **149 / 411** words match (ratio 0.725);
* code match: **929 / 4608** instructions.

The original's 15 original-only / 7 HEAD-only SLP groups are still exactly the
wave-119 §4a list (n9 `0xec/0x140`, retina `0x2a0/0x108/0xec/0xb0/0x13c/0x1d0`,
iPadLarge `0x180`, default `0x2a0/0xb0/0x108/0xec/0x13c/0x1d0` are missing;
n9 `0xdc/0x16c`, retina `0x294/0x16c`, iPad `0x16c`, default `0x294/0x16c`
are extra).

## 2. New concrete finding: moving `0xec..0xf8` contiguous fixes the pool head

The original's pool head is `0.325f, 0.65f, 100.0f, 50.0f` at
`0xd2854..0xd2860`; the baseline's head is only `100.0f, 50.0f` at
`0xd28d0` because `field_0x90`'s two constants live in a `.text` island at
`0xd2540` (`32.0f, 140.625f, 0.325f, 0.65f`).

Making the four `ringNearMidThreshold/field_0xf0/ringMidFarThreshold/field_0xf8`
statements contiguous (source order `0xec,0xf0,0xf4,0xf8`) makes clang 7 form
the missing `0xec` group and move the `0.325f/0.65f` pair into the end pool:

```
d2890: 3ea66666 3f266666 42c80000 42480000   ; 0.325f 0.65f 100.0f 50.0f
```

and `field_0x90` becomes `addw r1,pc,#0xf24; addeq r1,#4; vldr s2,[r1]`
loading from `d2890/d2894` — structurally identical to the original's
`d1948..d1954` (`addw r1,pc,#0xf08; addeq r1,#4; vldr s2,[r1]` from
`d2854/d2858`).  The pool grows 190 → **206** words.

But the body shrinks to **6952 B** (code 6128 B, 60 B short of the original;
pool 824 B, 60 B short) and the score drops to **46.876883 %** (2450 insns).
The rest of the pool order also changes for the worse: pool `difflib` match
falls 149 → 137.  Adding back individual missing windows on top of this
variant (`0x2a0..0x2ac`, `0xb0..0xbc`, `0x140..0x14c`, `0x1d0..0x1dc`,
`0x180..0x18c`, `0x13c..0x148`, `0x108..0x114`) gives 6936–7072 B but every
one scores below 48.894 (best 48.32 at `ec f8`, 7032 B).

## 3. Variants measured (real `link_so` builds, unrounded ratio)

| variant | size | ratio % |
|---|---:|---:|
| **baseline (HEAD)** | **7072** | **48.894202** |
| `0xec,0xf0,0xf4,0xf8` contiguous | 6952 | 46.876883 |
| `0xdc..0xe8` + `0xec..0xf8` contiguous | 6992 | 43.959126 |
| `0xec..0xf8` + `0x108..0x114` contiguous | 7072 | 42.210253 |
| `0xec..0xf8` + `0x294..0x29c` contiguous | 7048 | 45.383848 |
| `0xec,0xf8` only | 7032 | 48.321343 |
| `0xec,0xf4` only | 7040 | 46.840741 |
| `0x294,0x298,0x29c` contiguous | 7064 | 48.725100 |
| `0x2a0..0x2ac` contiguous | 7056 | 44.360303 |
| `0xb0..0xbc` contiguous | 7072 | 43.334660 |
| `0xc0..0xcc` contiguous | 7032 | 42.723098 |
| `0x13c..0x148` contiguous | 7004 | 46.434295 |
| `0x1d0..0x1dc` contiguous | 7072 | 47.161920 |
| `0x180..0x18c` contiguous | 7144 | 43.154349 |
| `0x16c..0x178` contiguous | 7072 | 48.894202 (no change) |
| remove `volatile rd` local (direct `Globals::retinaDisplay`) | 7064 | 48.843700 |
| `field_0x90` as `(retina||iPad||n9) ? 0.325f : 0.65f` | 7072 | 48.894202 (identical) |
| `(int)(float)` wraps on `0x9c/0x150/0x158/0x15c/0x168` | 7072 | 48.894202 (folded) |
| `(int)(float)(int)` on `0x168` | 7080 | 48.874726 |
| `(int)(float)(int)` on `0x160` | 7064 | 48.794100 |
| `(int)(float)(int)` on `0x10c` | 7048 | 45.024925 |
| 200 random permutations of the first 12 statements | — | none beat 48.894202 |
| 400 random single-statement moves | — | none beat 48.894202 |
| 300 random moves on top of the `0xec..0xf8` body | — | no 7072 hit above 48.894202 |

The `(int)(float)` wraps are all folded: clang 7.0.2 constant-folds a select of
constants through any number of int↔float casts, so the original's 19
phi-crossing round-trips cannot be reproduced from the field-major body — they
only survive in the config-major body, where each branch sees a specialised
expression (wave-123 §3).

## 4. Why the window reorders fail

The original's SLP windows are 16-byte aligned (`0xb0..0xbc`, `0xc0..0xcc`,
`0xec..0xf8`, `0x108..0x114`, `0x13c..0x148`, `0x2a0..0x2ac`, ...).  The
baseline's are shifted (`0xb4..0xc0`, `0xc4..0xd0`, `0xdc..0xe8`,
`0x148..0x154`, `0x294..0x2a0`, ...).  Making a target window contiguous does
change the SLP grouping, but it always also changes the *code* size by
8–136 B, and the 7072-B wall is a knife edge: only two of the tested windows
(`0xb0..0xbc`, `0x16c..0x178`, `0x1d0..0x1dc`) keep 7072 and both score below
the baseline.  The current field-major statement order is a strong local
optimum of the fuzzy metric; ~900 additional random orderings (permutations,
single moves, moves from the `0xec..0xf8` body) found nothing better.

## 5. Remaining root cause

The original is a config-major constructor whose common tail is a tree of
per-group blocks (wave-123 w7302 §1), so it vectorises 16-byte-aligned windows
and keeps the `(int)(float)` phis.  The current body is field-major and
hill-climbed: it matches the first nine HD `vst1` groups (same offsets, same
order) but then diverges in register allocation (`d0-d1` vs the original's
`d2-d3` for the nine store groups), SLP window alignment, and the pool order.
Reaching 7072 B with the original's grouping needs the common-tail tree
reconstruction (wave-122 §3): a faithful per-field body is ~400 B short, and
every partial reorder measured here either misses 7072 or loses more code
matches than it gains in the pool.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7402
cmake --build cmake-build-match --target check -j6     # RATCHET PASS, byte 4506
python3 /dev/shm/w7402/score.py                         # 7072 / 48.894202
```

Scratch harnesses (not committed, `/dev/shm/w7402/`): `score.py` (exact
single-function ratio), `groups.py` (maps every `vld1`/`vst1` pair to its
store offset + 4 pool words), `islands3.py` (dumps all `.text` constant
islands), `mkwin.py` (make a field window contiguous ascending), `batch.py`
(expression-variant runner), `move_search.py` / `perm_first.py` /
`ec_search.py` (order searches).
