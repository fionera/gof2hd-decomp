# `Layout::Layout` C1/C2 — wave 127 / worker w7643

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7072 B (0x1ba0)**, source `src/game/ui/Layout.cpp`.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(sha256 `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`).
Baseline = wave-126 final `fee528cbe` (7072 B / 48.943802312 % unrounded, 2529/2489 insns).

## Result — no landable source win (note only)

| metric | before (HEAD) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| unrounded `difflib` ratio | 48.943802 % | **48.943802 %** |
| `byte_exact` / `linked_exact` | 4506 / 4506 | **4506 / 4506** |
| `size_exact` / `data_exact` | 4524 / 843 | 4524 / 843 |
| imports / lint / drift | 207 / CLEAN / 0 | 207 / CLEAN / 0 |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
No `src/` change landed; the worktree is clean apart from this note.

## 1. The `int v` ballast hypothesis is CONFIRMED (but not landable)

The w7602 hypothesis is exactly right about the *codegen*: the current `int v` is a
float-typed ternary, so `int v = <float expr>` emits an extra `vcvt.s32.f32`
(float→int) before the `(int)(float)v` round-trip.  The original emits only the
two-conversion round-trip:

```
orig  d19ca: vmov s0, r2          ; r2 = int value (82/41/61/122/85)
      d19ce: str.w r2,[r0,#0xac]  ; field_0xac = v   (int store)
      d19d6: vcvt.f32.s32 s0,s0   ; int -> float
      d19da: vcvt.s32.f32 s0,s0   ; float -> int
      d19de: vstr s0,[r0,#0xa8]   ; field_0xa8 = (int)(float)v
```
HEAD instead emits `vcvt.s32.f32` **before** `vcvt.f32.s32` (3 conversions).
The faithful int-typed form is

```cpp
this->field_0xac = Globals::iPadHD ? 61 * 1.40625f : (Globals::iPadLarge ? 122 : (Globals::n9 ? 61 : (Globals::iPad ? 61 : (Globals::retinaDisplay ? 82 : 41))));
this->field_0xa8 = (int)(float)(Globals::iPadHD ? 85 : (Globals::iPadLarge ? 122 : (Globals::n9 ? 61 : (Globals::iPad ? 61 : (Globals::retinaDisplay ? 82 : 41)))));
```

Measured (real `link_so` builds, exact unrounded ratio):

| variant | size | ratio % |
|---|---:|---:|
| HEAD baseline (`int v` float ternary) | **7072** | **48.943802** |
| int-typed `0xa8` (above) | 7032 | **49.011384** |
| int `v`, `ac = v`, `a8 = (int)(float)v` | 7024 | 48.701558 |
| `float v`, `a8 = (int)v` | 7064 | 48.923445 |
| `int v`, `float f = v`, `a8 = (int)f` | 7024 | 48.701558 |
| `int v`, `ac = (int)(float)v`, `a8 = (int)(float)v` | 7024 | 48.701558 |

So the int form is **more faithful and scores +0.068 %**, but it is exactly
**40 B short** (7032 vs 7072).  A size change is *not* landable: `-ffunction-sections`
+ the fixed link order shifts every later function by 40 B, and the PC-relative
GOT/pool deltas change, so `byte_exact` collapses 4506 → **2455** (measured, same
as w7602's note).  The 40 B must be recovered as *real* code.

## 2. Where the 40 B must come from: the duplicated common1 stores

Recovered the original's **per-offset store counts** in common1 (`d1860..d21e2`,
2434 B) with a capstone/objdump sweep that resolves `add rX,r0,#imm` and
`strd`/`vst1` base registers.  The original stores several fields **more than
once** (each branch arm writes its own value); HEAD's field-major ternaries write
each field once (plus the outlined cold tails).  Duplicate counts in the original:

```
0x7c x2   0x94 x2   0x154 x4  0x164 x3  0x16c x4  0x180 x3
0x184 x4  0x18c x4  0x198 x3  0x1a0 x3  0x1a4 x3  0x1ac x3
```

The clearest example is the `0x154/0x158/0x15c` group (`d1d2a..d1da6`):

```
d1d2e cmp r9,#0 ; beq d1d4e
d1d38 0x154=100 ; 0x158=100 ; 0x15c=294          <- iPadLarge arm
d1d4e cbz r2,d1d5c
d1d52 0x154=50                                   <- iPad arm
d1d5c cmp lr,#0 ; beq d1d9c
d1d62 0x154 = retina?30:15 ; r4 = retina?28:14    <- !n9 arm
d1d9c 0x154=14 ; r4=13                            <- n9 arm
d1da6 0x158 = (int)(float)r4
```
i.e. the source is a nested `if (iPadLarge) / else if (iPad) / else if (!n9) /
else` tree that writes `0x154` in **four** arms and shares the `r4` value between
`0x154` and `0x158`.

## 3. Variants measured (real `link_so` builds, exact unrounded ratio)

All built on top of the faithful int-typed `0xa8` form (7032 / 49.011 %):

| variant | size | ratio % |
|---|---:|---:|
| int-typed `0xa8` (base for this table) | 7032 | 49.011384 |
| `0x154/0x158/0x15c` nested branch tree (iPadLarge/iPad/!n9/n9) | 7032 | 48.971440 |
| flat `if/else` tree for `0x154/0x158/0x15c` | 6976 | 48.866600 |
| `0x154` alone → flat if/else | 7032 | 49.011384 (no change) |
| `0x16c` alone → flat if/else | 7032 | 49.011384 (no change) |
| `0x18c` alone → flat if/else | 7032 | 49.011384 (no change) |
| `0x198` alone → flat if/else | 7032 | 49.011384 (no change) |
| `0x180` alone → flat if/else | 7000 | 48.909346 |
| `0x184` alone → flat if/else | 7016 | 48.780488 |
| `0x164` alone → flat if/else | 6972 | 38.221154 |
| `0x1a4` alone → flat if/else | 7020 | 48.610834 |
| `0x1ac` alone → flat if/else | 7016 | 48.129626 |
| `(int)(float)` on `0x158` | 7048 | 48.592533 |
| `(int)(float)` on `0x1e4` | 6976 | 42.905743 |
| `(int)(float)` on `0x158+0x15c+0x170+0x1e4` | 7000 | 42.734359 |

On the **HEAD (float-`v`) baseline** (7072), adding the original-only round-trips
(`0x158`, `0x15c`, `0x170`, `0x1e4`) also never improves at 7072 B:

| variant | size | ratio % |
|---|---:|---:|
| HEAD | **7072** | **48.943802** |
| `(int)(float)` on `0x15c` | 7072 | 48.943802 (folded) |
| `(int)(float)` on `0x170` | 7072 | 48.943802 (folded) |
| `(int)(float)` on `0x158` | 7096 | 48.447452 |
| `(int)(float)` on `0x1e4` | 7016 | 42.372204 |

Converting a ternary to a flat `if/else` almost always **shrinks** the body
(LLVM folds/merges the arms), so it cannot serve as a 40 B anchor.  The original's
duplication comes from *nested* arms that share a value (`r4`) across two fields;
reproducing it exactly requires transcribing the whole common1 tree, not a few
per-field `if/else`s.

## 4. Conclusion / open questions

* The `int v` is confirmed ballast: the int-typed `0xa8` form is the faithful one
  (2 conversions) and is exactly 40 B shorter.
* It cannot be landed alone because the 40 B size change collapses `byte_exact`
  4506 → 2455.  A size anchor of **real duplicated common1 stores** is required.
* The anchor is not any single per-field `if/else` tested here; it is the nested
  common1 branch tree that shares temporaries between adjacent fields
  (`0x154`/`0x158`, `0x164`/`0x168`, `0x16c`/`0x170`, `0x180`/`0x184`/`0x188`,
  `0x18c`/`0x190`, `0x198`/`0x19c`/`0x1a0`, `0x1a4`/`0x1a8`, `0x1ac`).
* Next step: transcribe the whole common1 tree in original order (iPadLarge,
  iPad, !n9, n9) with shared locals, then re-check that the body lands at exactly
  7072 B; if it does, the int-typed `0xa8` can be dropped in and the ratio should
  rise above 49.0 %.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7643
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
python3 /dev/shm/w7643/score.py                          # 7072 / 48.943802312
```

Scratch harnesses (not committed, `/dev/shm/w7643/`): `score.py` (exact
single-function scorer using `tools/verify/verify.py`), `stores2.py`
(store-offset tracer with `add rX,r0,#imm` / `strd` / `vst1` base resolution),
`batch.py`..`batch7.py` (variant batches), `Layout.base.cpp` (HEAD snapshot),
`orig_decomp.c` (Ghidra decompile at `0xe1028`), `orig.dis` / `ours.dis`.
