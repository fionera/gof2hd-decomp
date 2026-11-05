# Layout::Layout C1/C2 — wave 119 / worker w6771
## Angle J: type/order/pool sweep at the hard 7072-byte wall — no landable win

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (identical bodies), ELF VMA `0xd1028`,
original size **7072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave119-w6771`, base `026aadb4e` (wave-119 harvest).
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.

## Result — no source change landed; note-only commit

| metric | before (HEAD) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| unrounded `difflib` ratio | 48.624950179 % | **48.624950179 %** |
| `byte_exact` / `linked_exact` | 4505 / 4505 | **4505 / 4505** |
| `size_exact` / `data_exact` | 4524 / 843 | **4524 / 843** |
| imports / lint / drift | 207 / CLEAN / 0 | **207 / CLEAN / 0** |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
27 section headers, file size 2,187,328 B, `.rodata` and `.ARM.extab` untouched.

## The hard size wall (re-measured, and the earlier claim corrected)

The wave-119 handoff stated "any 7076 B build collapses `byte_exact` 4505 → 2454".
That is only true for **+4**; a **−4** build behaves very differently.  Measured
with the real `check` target:

| `_ZN6LayoutC1Ev` size | `byte_exact` | `linked_exact` | ratchet |
|---:|---:|---:|---|
| **7072** (HEAD) | **4505** | **4505** | PASS |
| 7068 (`N_swap` + `M_0x138`) | 4462 | 4505 | FAIL (−43) |
| 7076 (`N_swap` only) | 2454 | 4505 | FAIL (−2051) |

So a 4-byte shrink only costs 43 byte-exact functions (all downstream functions
whose raw bytes contain a shifted address), while a 4-byte growth costs 2051.
`linked_exact` stays 4505 in both cases (branch targets are function-relative).
The gate still requires a **no-decrease** `byte_exact`, so only exactly-7072
variants are landable.

## Best measured variant overall (unlandable, 7068 B)

`N_swap` + `M_0x138` = **48.704663212 %** at **7068 B**:

```cpp
// N_swap: 0x1a8 before 0x1a0 (revert of the wave-119 landed order)
this->field_0x1a8 = Globals::iPadHD ? 16.0f * 1.40625f : Globals::n9 ? 24.0f : Globals::retinaDisplay ? 32.0f : 16.0f;
this->field_0x1a0 = Globals::iPadHD ? 18 : (...);
// M_0x138: drop the outer float round-trip
this->field_0x138 = (int)((int)(float)v138);
```

Individual components (all real `link_so`/fast-.o builds, unrounded):

| variant | size | ratio % |
|---|---:|---:|
| HEAD | 7072 | 48.624950179 |
| `N_swap` only | 7076 | 48.685258964 |
| `M_0x138` only | 7064 | 48.644338118 |
| `N_swap` + `M_0x138` | 7068 | **48.704663212** |
| `N_swap` + `M_0x138` + `orbitInfoSecurityY` round-trip | 7068 | 48.704663212 |

Every 7072-B variant found in this wave scored **≤ 48.624950179**.

## What was swept (all real builds; 2,915 object builds + 817 full scores)

Scoring harness: compile `Layout.cpp` alone with the exact NDK r18b clang 7.0.2
`-Oz` flags, read `_ZN6LayoutC1Ev` size from the `.o`, then score the `.o`
disassembly with the `tools/verify/verify.py` normalizer (validated to reproduce
`score.py`'s linked-`.so` ratio bit-for-bit on the baseline: 48.62495017935433).

### 1. Store-type corrections from the original's store mnemonics

The original's per-offset store kinds were extracted from the disassembly
(`str`/`strd` = int, `vstr` after `vcvt.s32.f32` = int round-trip, `vstr`
without `vcvt` = float).  Fields where HEAD and the original disagree:

| offset | field | original | HEAD | edit | Δsize | ratio |
|---|---|---|---:|---|---:|---:|
| 0x020 | `field_0x20_top` | `strd` (int) | `vstr` | `(int)(5 * 1.40625f)` arm | **−224** | 20.44 |
| 0x10c | `field_0x10c_rowBaseY` | int (SLP) | `vstr` | drop `(float)` | −8 | 48.39 |
| 0x148 | `field_0x148` | int | `vstr` | `(int)(2 * 1.40625f)` arm | −8 | 46.5 |
| 0x1f4 | `field_0x1f4` | int | `vstr` | `(int)(16 * 1.40625f)` arm | −8 | 48.54 |
| 0x204 | `field_0x204` | `strd` (int) | `vstr` | `(int)(9 * 1.40625f)` arm | 0 | 48.14 |
| 0x3e0 | `scrollBarInset` | `strd` (int) | `vstr` | `(int)(1 * 1.40625f)` arm | −16 | 47.17 |
| 0x3e8 | `rewardBoxWidth` | int | `vstr` | `(int)(100 * 1.40625f)` arm | **+48** | 40.18 |
| 0x158 | `field_0x158` | `vstr` round-trip | int | wrap `(int)(float)(...)` | 0 | 48.625 |
| 0x15c | `field_0x15c` | `vstr` round-trip | int | wrap `(int)(float)(...)` | 0 | 48.625 |
| 0x170 | `field_0x170` | `vstr` round-trip | int | wrap `(int)(float)(...)` | 0 | 48.625 |
| 0x1e4 | `field_0x1e4` | `vstr` round-trip | int | wrap `(int)(float)(...)` | −40 | 45.64 |
| 0x3f8 | `rewardBoxY2` | `vstr` round-trip | int | wrap `(int)(float)(...)` | +16 | 47.24 |

The `(int)(float)(...)` wraps on 0x158/0x15c/0x170 are **codegen-neutral** (the
constants fold), so those round-trips are already implicit in HEAD.

### 2. Exhaustive pair/triple combinations of the type edits

144 subsets with predicted Δsize 0 were built; 96 landed at exactly 7072 B and
were scored.  Best = 48.624950179 (baseline).  Notable: every combination
containing `G_0x3e8` (+48) scores ≈ 40 %, because it breaks the
`scrollBarInset`/`scrollBarHandle`/`rewardBoxWidth` store run.

### 3. `(int)(float)(...)` round-trip sweep

All 172 int-typed ternary assignments wrapped in `(int)(float)(...)`:
147 are codegen-neutral, 25 change size (deltas −72 … +60), **no +4**.

### 4. `Globals::iPadHD ? N * 1.40625f` → `(int)(N * 1.40625f)` sweep

86 sites: deltas −288 … +48, **no +4**.

### 5. Per-arm `(int)` casts on `n9` / `retinaDisplay` / `iPadLarge` / `iPad` arms

385 sites on the 7068 base: **all codegen-neutral**.

### 6. Adjacent-statement swaps

All 185 adjacent simple-statement swaps on HEAD: deltas −116 … +96; only
`s014` (0x4↔0x8), `s024` (0x20↔0x34), `s025` (0x34↔0x30) are +4.  All 15 swaps
that land at 7072 on the 7076 base score ≤ 48.625.

### 7. Random statement moves

2,000 random 1–3 statement relocations on HEAD → 23 at 7072 B, best 48.625.
2,000 on the 7068 base → 10 at 7072 B, best 48.536.

### 8. Random multi-edit combinations

800 random 2–4-edit subsets of a 120-edit pool (type edits + ±8/±16 arm casts +
small adjacent swaps), predicted Δsize 0 → 385 at 7072 B; best 48.625.

### 9. SLP-group adjacency (the 8 missing `vld1.64` groups)

Making each original SLP run adjacent in address order in the source:

| group | fields | size | pool start | ratio |
|---|---|---:|---:|---:|
| g1 | 0xb0,b4,b8,bc | 7088 | 0x2f0 | — |
| g2 | 0xec,f0,f4,f8 | 7008 | 0x2f0 | — |
| g3 | 0x13c,140,144,148,14c | 7012 | 0x2f8 | — |
| g4 | 0x16c,170,174,178 | **7072** | 0x2f0 | 48.624950 |
| g5 | 0x180,188,18c | **7072** | 0x2f0 | 48.207171 |
| g6 | 0x2a0,a4,a8,ac | 7048 | 0x2f0 | — |
| all | — | 6972 | 0x2f8 | — |

Only g4/g5 (and g4+g5) land at 7072; neither improves the ratio.  Notably the
adjacency does **not** add the missing SLP group in the emitted code — the
`vld1.64`/`vst1.32` count is unchanged (77/70) — because the four stores still
come from different branch arms.

### 10. if/else-chain → ternary conversions

All 10 simple `if (Globals::…) … else if …` chains converted to nested
ternaries: sizes 7072–7144; the three that land at 7072 (ch203, ch280, ch300)
score 48.545 / 48.545 / 48.625.

## Structural findings

* **The original's first literal island is at `0xd1308` = function+`0x2e0`;
  HEAD's is at `0xd1318` = function+`0x2f0`.**  The original island is
  `(9,11,5,253)` = `field_0xc0/c4/c8/cc` (iPadHD arms); HEAD's is
  `(20,25,11,9)`.  Everything from the *second* island
  `(1,21,78,151)` onward is the same data in the same order.  Aligning the pool
  needs the code before the first island to shrink by 16 B while the pool grows
  by 4 words — no source shape found in this wave achieves it (the closest
  observed pool starts are `0x2e8` and `0x2f8`).
* **Prologue:** original `stmdb sp!, {r6,r7,r8,r9,sl,fp}`; HEAD
  `stmdb sp!, {r5,r6,r7,r8,r9,sl,fp}` — HEAD redundantly re-saves `r5`
  (+4 B).  This is register allocation, not source-controllable.
* The original outlines **four** cold `else` tails after `pop` (80 B:
  0x130/0x134/v138, `t_field_0x2c8`, `field_0x2d0`, `field_0x60`); HEAD
  outlines **one** (28 B: `field_0x1ac`).  The 52 B cold-block delta is the
  other half of the pool-start mismatch.
* The 8 missing SLP groups, the +2 `100.0f`/`50.0f` words and the −1 GOT word
  from `layout-literal-pool-got-wave119-w6720.md` were all reconfirmed, but no
  source shape that restores them keeps the function at exactly 7072 B.

## Conclusion

At the mandatory 7072-byte size the wave-119 HEAD is a strong local optimum for
the fuzzy metric: **817 distinct 7072-B variants** (from ten independent edit
families) were scored and none beat **48.624950179 %**.  The best code shape
found (`N_swap` + `M_0x138`, **48.704663212 %**) is 7068 B and therefore
unlandable (ratchet `byte_exact` 4462 < 4505).  No source change is committed.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6771
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6     # RATCHET PASS, byte 4505

OBJ=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OBJ -d --start-address=0xd1028 --stop-address=0xd2bc8 \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
$OBJ -d --start-address=0xd1028 --stop-address=0xd2bc8 \
  cmake-build-match/libgof2hdaa.so
```

Scratch harnesses (not committed): `/dev/shm/w6771/{score.py,fastscore.py,
parsize.py,partext.py,fastall.py,poolstart.py,scanall.py,genmove.py,genadj.py,
chains.py}` plus `Layout.base.cpp`, `Layout.7068b.cpp`, `Layout.7076.cpp`.
