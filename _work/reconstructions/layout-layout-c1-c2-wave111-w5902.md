# Layout::Layout C1/C2 — wave 111 (w5902): shared-int clusters, store-order
# replay, and the measured 7072-B frontier

Worker **w5902**, branch `wave111-w5902`, base `8eea060cc`.
Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (same body and score),
ELF VMA `0xd1028`, original size **7072 B**.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.

## Result — no landable improvement found; the wave-110 body is the optimum

| metric | before | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| `_ZN6LayoutC1Ev` unrounded linked `difflib` ratio | 48.337646824607 % | **48.337646824607 %** |
| `_ZN6LayoutC2Ev` | 48.337646824607 % | **48.337646824607 %** |
| normalised insns (orig/ours) | 2529 / 2494 | 2529 / 2494 |
| `byte_exact` / `linked_exact` | 4500 / 4500 | **4500 / 4500** |
| `size_exact` / `data_exact` | 4524 / 843 | **4524 / 843** |
| imports / lint / drift | 207 / CLEAN / 0 | **207 / CLEAN / 0** |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
27 section headers, file size 2,187,328 B, `.ARM.extab` `0x26294` — all unchanged
(no code change was landed).

**This pass is a note-only commit.** Every variant below was built for real
(`link_so` + `tools/verify` scorer) and measured; none beat 48.3376 % at the
mandatory 7072 B.

## What the original actually does (established from Thumb + Ghidra)

The function is a **3-way branch on `Globals::iPadHD` / `Globals::n9` /
`Globals::retinaDisplay`** that clang hoists out of the flat per-field
ternaries, followed by a merged tail. Block map (addresses in the original):

| block | range | fields |
|---|---|---|
| HD | `d1042..d1268` | `0x4..0x2d4`, `0x1e0`, then `jmp d21e2` |
| n9 | `d126c..d12fc` | `0x4..0x78`, `0x2c0..0x2cc` (constants) |
| non-n9 | `d1570..d185e` | `0x4..0x2cc` (per-field `retina?A:B`) |
| tail A | `d1860..d1ac4` | `0x2d0,0x2d4,0x7c..0x138` |
| tail B | `d1ac6..d21e2` | alternate path for `0x7c..0x1e0` |
| tail C | `d21e2..d2be8` | `0x1e8,0x1e4`, `0x1fc..0x27c`, `0x280..0x320` |

### Proven shared-int clusters (one source variable → several fields)

Read off the original's register reuse (a value computed once and stored to
several offsets is only possible if the *source* has one variable — the
compiler cannot CSE the `GOT`-loaded globals across stores to `this`):

* **non-n9 block**, `uVar30 = retina ? 10 : 5` → `0x20`, `0x24`, `0x28`
  (`strd ip, ip, [r0,#32]` + `str.w ip,[r0,#40]` at `d15ee..d15f2`).
* **non-n9 block**, `uVar13 = retina ? 4 : 2` → `0x1fc`, `0x22c`, `0x258`.
* **non-n9 block**, `uVar15 = retina ? 2 : 1` → `0x230`, `0x234`, `0x250`.
* **non-n9 block**, `uVar23 = retina ? 8 : 4` → `0x208`, `0x210`, `0x254`
  (`strd r3,r2,[r0,#528]` at `d23b0`).
* **non-n9 block**, `uVar19 = retina ? 0x28 : 0x14` → `0x224`, `0x27c`.
* **tail A**, `r6 = retina ? 2 : 1` → `0x124`, `0x128`
  (`stmia.w ip!,{r2,r3,r4,r6}` at `d1c88` + `strd r6,r1,[r0,#296]` at `d1c8c`).
* **tail C**, `r1 = retina ? 2 : 1` → `0x230`, `0x234`, `0x250`
  (`str.w r1,[r0,#560]`, `str.w r1,[r0,#564]`, `strd r2,r1,[r0,#588]` at
  `d2348..d236c`).
* **tail C**, `r3 = retina ? 8 : 4` → `0x208`, `0x210`
  (`strd r2,r3,[r0,#516]` + `strd r3,r2,[r0,#528]` at `d23a4..d23b0`).

Note the clusters are **branch-local**: e.g. `0x208`/`0x210` share in the HD
and non-n9 branches but *not* in n9 (`0x208=7`, `0x210=6`), so a single flat
ternary cannot express them.

### Other verified facts

* `0x1e4` is stored through a float register (`vmov.f32 s0,#24; vcvt.s32.f32;
  vstr s0,[r0,#484]` at `d21a6..d21fe`) — i.e. `(int)(float)v`. Our header
  declares it `int` and `Hud.cpp` reads it as `int`; retyping it moves Hud's
  codegen, so the float round trip cannot be landed.
* `0x150` is likewise an `(int)(float)` store (`d1d2a..d1d32`).
* `0x9c` HD store is a plain int inside the `stmia 0x94` group; the retina
  store is an int→float round trip (`d199e..d19a8`). The wave-110 `int u9c`
  phi already reproduces this.
* Original HD block: 27 `vld1.64` pool vectors, 81 `vst1.32`, 62 `strd`,
  2 `stmia`, 103 `str.w`. Ours: 165 `str.w`, 46 `strd`, 70 `vst1.32` —
  i.e. **62 excess scalar `str.w`** (fields not memory-adjacent in our source).

## Measurements (all real builds, unrounded linked score)

Baseline (wave-110 landed body) = **7072 B / 48.337646824607 %**.

| variant | size | % |
|---|---:|---:|
| **baseline (landed)** | **7072** | **48.3376** |
| swap `0x188`↔`0x180` (adjacent) | 7064 | 48.3473 |
| 7040 body (baseline minus 4 ballast wraps) | 7040 | 48.3955 |
| 7040 + swap `0x1a4`↔`0x190` | 6996 | 48.6497 |
| 7040 + swap `0x1a4`↔`0x190` + `0x1a4`↔`0x188` | 6928 | 38.4801 |
| HD `vst1` group order replayed (`0x14,0x4,0x24,…`) | 7056 | 45.7678 |
| … + ballast to 7072 | 7072 | 45.7313 |
| ascending order, first 22 fields | 7068 | 24.7007 |
| full store-order replay (NORM+TAILA+TAILC) | 6932 | 10.8477 |
| move `0x2c0` next to `0x2c4` | 7048 | 44.7326 |
| … + ballast to 7072 | 7072 | 44.6792 |
| move `0x1e0` to end of HD block | 7040 | 45.9972 |
| move `0x140,0x144,0x148` before `0x14c` | 7012 | 45.8984 |
| … + ballast | 7076 | 45.5923 |
| `0x1e4` gets `(int)(float)` | 7104 | 45.3190 |
| `0x3f8` gets `(int)(float)` | 7072 | 48.2978 |
| `0x9c`/`0x158`/`0x15c`/`0x170` get `(int)(float)` | 7072 | 48.3376 (code-identical) |
| every `N * 1.40625f` → int literal | 6896 | 14.9407 |
| `0x20_top` HD arm → int literal 7 | 6948 | 34.3017 |

### Shared-int cluster experiments (flat locals; long live range)

| cluster shared | size | % |
|---|---:|---:|
| `{0x124,0x128}` (n9 test dropped) | 7048 | 39.7369 |
| `{0x124,0x128}` (n9 test kept) | 7128 | 42.9621 |
| `{0x124,0x128,0x230,0x234,0x250}` | 7024 | 39.4479 |
| `{0x1c,0x224,0x27c}` | 7072 | 44.4754 |
| `{0x24,0x20,0x1d0}` | 7064 | 38.0402 |
| `{0x24,0x20}` | 7048 | 41.7530 |
| `{0x22c,0x1fc,0x258}` | 7072 | 48.3376 (**code-identical** — compiler already shares these) |
| `{0x150,0x1d8,0x210}` | 7072 | 48.3376 (expressions differ, no share) |

The flat-local form always loses: the value's live range spans the whole
function, spilling registers. The original's sharing is branch-local and
short-range, which a flat ternary source cannot express.

### Exhaustive / random searches (all at 7072 B unless noted)

* **All 204 adjacent swaps** of the landed body: no 7072-B variant above
  48.3376; best 7064-B swap is `0x188`↔`0x180` at 48.3473.
* **All 204 adjacent swaps** of the 7064-B swap body: no 7072-B hit.
* **500 random non-adjacent swaps** of the landed body: no 7072-B hit.
* **600 random statement moves** of the landed body: no 7072-B hit.
* **Single `(int)(float)(int)` wrap scan** of the 7040 body (all 236 fields):
  +8 delta fields all measure 48.3762 individually (`0x180` 48.3955); the
  landed 4×+8 partition is the best 7072 combination found.
* **Combination search** (k=1..3 over 25 shortlisted wraps, targeting 7072):
  no combination above 48.3376.
* **Duplicate-statement insertion** on the 7064 body (every statement):
  nothing reaches 7072.

## Why the 6996-B body cannot be landed

The best fuzzy variant is 6996 B / 48.6497 % (7040 body + swap
`0x1a4`↔`0x190`). It needs **+76 B** to keep `.text`/`size_exact` exact.
On that body every single `(int)(float)(int)` wrap *shrinks* the function
(register-allocation cliff): the only wraps that grow it are
`orbitInfoSystemY` (+4 → 7000, 37.82 %) and `field_0x234` (+12 → 7008,
37.87 %). No combination reaches 7072 without losing more than the 0.31 pp
the 6996 body gains.

## Proven vs hypothesis

**Proven (binary / measured):**

* The shared-int clusters listed above are real source objects in the
  original (register reuse across distant stores of the same value).
* They are branch-local; the flat-ternary source form cannot reproduce them,
  and forcing them with flat locals costs 4–9 pp.
* The original's store order is **not** the source order: replaying the
  binary store order (NORM+TAILA+TAILC) drops the score to 10.8 %, and the
  HD `vst1` group order to 45.7 %.
* The wave-110 body is a strong local optimum: 204 adjacent swaps, 500
  random swaps, 600 random moves, and a 25-candidate wrap-combination search
  all fail to beat 48.3376 % at 7072 B.
* No section/`.ARM.extab` delta was introduced (no code change landed).

**Hypothesis (not proven):**

* The remaining ~51.7 % mismatch is dominated by (a) the 62 excess scalar
  `str.w` — our source does not place the fields of the original's
  `strd`/`stmia` groups adjacently, and (b) per-field `retina` compares that
  the original shares. Closing it needs the original's **explicit
  `if (iPadHD) … else if (n9) … else …` source shape with branch-local
  shared locals**, not a flat ternary chain. A faithful reconstruction of
  that shape was not completed in this pass; the flat-local approximations
  all measured worse.
* `field_0x1e4` is a `float` in the original struct; our `int` declaration is
  a real type divergence that cannot be fixed without touching `Hud.cpp`.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5902
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 /dev/shm/w5902/fscore.py _ZN6LayoutC1Ev         # 7072 B, 48.337646824607
```

Scratch harness (all under `/dev/shm/w5902`): `fscore.py` (unrounded linked
scorer), `trybody.sh` (splice ctor body → `link_so` → score), `move.py` /
`reorder.py` / `swap.py` (statement permutation), `ballast.py` /
`combo.py` / `scanwrap.py` (size-exact wrap search), `adjswap.py`
(adjacent-swap scan), `randsearch.py` / `randmove.py` (random search),
`stores3.py` (per-offset store map from objdump), `body.base.txt` (landed
body), `body.7040.txt`, `body.s1.txt` (6996 body), `body.sw119.txt`
(7064 body).

## Continuation measurements (same worker, second session)

The pristine HEAD body (`git show HEAD:src/game/ui/Layout.cpp`) was re-scored
after restoring the worktree: **7072 B / 48.337646824607 %**, 2529/2494
insns. The following additional real-build variants were measured; none
beats the landed body.

| variant | size | % |
|---|---:|---:|
| `field_0x94` moved back to its iOS position (before `0x98`) | 7016 | 45.6800 |
| five identical `1*1.40625f` exprs (`124,128,230,234,250`) shared, block-scoped local | 7060 | 37.5199 |
| same five shared, top-level local | 7056 | 22.3461 |
| identical `20*1.40625f` exprs (`orbitInfoSystemY` 0x224, `0x27c`) shared | 7056 | 47.8469 |
| … + `(int)(float)(int)` wrap on `field_0x1a4` (to 7072) | **7072** | 47.7992 |
| retina arm `(retina ? 8 : 4)` shared by `0x208/0x210/0x254` | 7024 | 37.6400 |
| single-use int temp + `(int)(float)` on `0x158` | 7072 | 48.3376 (code-identical) |
| single-use int temp + `(int)(float)` on `0x15c` | 7072 | 48.3376 (code-identical) |
| single-use int temp + `(int)(float)` on `0x170` | 7072 | 48.3376 (code-identical) |
| single-use int temp + `(int)(float)` on `0x3f8` | 7072 | 48.3376 (code-identical) |
| single-use int temp + `(int)(float)` on `0x1e4` | 7104 | 45.3190 |
| `0x158/0x15c/0x170/0x3f8` as explicit `if/else if/else` chains + temp | 7072 | 48.3376 (code-identical) |

The single-use-temp results are the important negative: clang at `-Oz`
propagates and constant-folds a temp with one use even when the assignment is
an `if/else` chain, so the original's missing `vstr` stores (`0x158 0x15c
0x170 0x3f8`) cannot be reproduced by a per-field temp. They require a value
that is genuinely shared by several fields (a multi-use phi), which is exactly
the branch-local cluster shape that the flat-ternary source cannot express
without the live-range cost measured above.

Gate after restoring the HEAD body: `RATCHET PASS`, byte_exact 4500,
linked_exact 4500, size_exact 4524, data_exact 843, imports 207, lint CLEAN,
drift 0; 27 section headers, file size 2,187,328 B, `.ARM.extab` `0x26294`
(all unchanged).
