# Layout::Layout C1/C2 — wave 109 (w5701): the `(int)(float)(int)` round-trip
# on `0x10c`/`0x138`/`0x160` and the `0x1a4` cast strip

Worker **w5701**, branch `wave109-w5701`, base `7c34c4098`.
Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (same body and score),
ELF VMA `0xd1028`, original size **7072 B**.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.

## Result — landed a real, size-exact fuzzy improvement

| metric | before | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** |
| `_ZN6LayoutC1Ev` unrounded linked `difflib` ratio | 48.049363057325 % | **48.269001193792 %** |
| `_ZN6LayoutC2Ev` | 48.049363057325 % | **48.269001193792 %** |
| normalised insns (orig/ours) | 2529 / 2495 | 2529 / **2497** |
| equal normalised insns | 1207 | **1213** |
| `byte_exact` / `linked_exact` | 4500 / 4500 | **4500 / 4500** |
| `size_exact` / `data_exact` | 4524 / 843 | **4524 / 843** |
| imports / lint / drift | 207 / CLEAN / 0 | **207 / CLEAN / 0** |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
No section change: still 29 section headers (the pre-existing wave-108 192-byte
`.jcr` is the only extra vs the original's 28), `.ARM.extab` still
`0x26294` (4 B short, unchanged), `.text` size/base unchanged, so
`byte_exact` stays 4500.

## The four edits (ctor only)

1. `field_0x1a4` is a **plain int select** in the original (Thumb
   `str.w r1,[r0,#420]` at `d2080`/`d208c`, no `vcvt`). The landed body had a
   redundant `(int)(float)((int)(float)(...))` double cast; stripping it is
   the correct source shape and removes 24 B.

```cpp
this->field_0x1a4 = Globals::iPadHD ? 22 : (Globals::n9 ? 28 : (Globals::retinaDisplay ? 32 : 16));
```

2–4. `field_0x10c_rowBaseY`, `field_0x138` and `field_0x160` are stored
through the original's **int→float→int round trip** (`vmov s0,rN;
vcvt.f32.s32; vcvt.s32.f32; vstr s0,[r0,#off]`):

```
d1d22: vmov s0,r1 ; vcvt.f32.s32 s2,s0 ; vcvt.s32.f32 s2,s2 ; vstr s2,[r0,#312] ; 0x138
d1e12: ... vstr s0,[r0,#352] ; 0x160
d1???: ... (0x10c is in the mid-block SLP group)
```

The source spelling that reproduces it in the flat body is the redundant
inner `(int)` cast:

```cpp
this->field_0x10c_rowBaseY = (int)(float)(int)(Globals::iPadHD ? 1 * 1.40625f : (Globals::n9 ? 2 : (Globals::retinaDisplay ? 2 : 1)));
this->field_0x138 = (int)(float)(int)(Globals::iPadHD ? 47 * 1.40625f : (Globals::n9 ? 52 : (Globals::iPadLarge ? 94 : (Globals::retinaDisplay ? 70 : (Globals::iPad ? 47 : 35)))));
this->field_0x160 = (int)(float)(int)(Globals::iPadHD ? 49.0f * 1.40625f : (Globals::iPadLarge ? 98.0f : (Globals::iPad ? 49.0f : (Globals::n9 ? 13.0f : (Globals::retinaDisplay ? 26.0f : 13.0f)))));
```

`(int)(float)(expr)` alone constant-folds (`(int)(float)52` → `52`); the
redundant `(int)` on the operand keeps the float round trip in the IR, so the
`vmov`/two `vcvt`/`vstr` fingerprint survives. `(int)(float)(int)(int)` is
code-identical; `(short)`/`(char)` forms shrink and lose the match.

## Why this closes the 24-byte gap

The wave-108 body was 7072 B but had 24 B of *wrong* code (the `0x1a4`
double cast). Stripping it gives the correct 7048 B body, which is 24 B short.
The original's mid block carries the intphi round trip on several fields; the
three chosen round trips add exactly the missing 24 B (the individual deltas
overlap: `0x10c` +16 B, `0x138` +8 B, `0x160` +8 B, but the three together
add 24 B) and raise the fuzzy match by 6 normalised instructions.

## Measured table (real `link_so` builds, unrounded linked score)

| variant | size | % |
|---|---:|---:|
| wave-108 baseline | 7072 | 48.049363057325 |
| `0x1a4` strip only | 7048 | 48.167330677291 |
| strip + `0x10c` intphi | 7064 | 48.238805970149 |
| strip + `0x138` intphi | 7056 | 48.058928926936 |
| strip + `0x160` intphi | 7056 | 48.108323377141 |
| strip + `0x10c`+`0x138` | 7064 | 48.248407643312 |
| strip + `0x10c`+`0x160` | 7072 | 48.259399244082 |
| **strip + `0x10c`+`0x138`+`0x160` (landed)** | **7072** | **48.269001193792** |
| strip + `0x10c`+`0x138`+`0x160`+ any 0-B intphi | 7072 | 48.269001193792 (code-identical) |
| strip + `0x10c`+`0x138`+`0x160`+ another +8 intphi | 7080 | 48.2498 / 48.2100 |
| `0x1a4` strip + `0x310` 5-arm if/else | 7076 | 48.062015503876 |
| `0x1a4` strip + `0x310` 5-arm if/else + `0x10c` intphi | 7092 | 48.2484 |
| all 120 `0x310` if/else arm permutations on the strip body | 7044–7180 | none 7072 |
| all 205 ternary→if/else conversions on the strip body | — | only unit 52 gives 7072 (43.44 %) |
| all 204 adjacent swaps of the landed body | — | none above 48.2690 |

## Proven vs hypothesis

**Proven (binary / measured):**

* The original's `0x1a4` store is a plain int store (no `vcvt`), so the
  double cast is wrong; stripping it is a genuine source fix.
* The original's `0x138` and `0x160` stores are the intphi round trip
  (`vmov`/`vcvt.f32.s32`/`vcvt.s32.f32`/`vstr`), and `0x10c` is in the same
  mid-block intphi group.
* The landed body is exactly 7072 B, both C1/C2 score 48.269001193792 %,
  and the full gate is `RATCHET PASS` with `byte_exact = linked_exact = 4500`.
* No section / `.ARM.extab` delta is introduced (29 headers, `.ARM.extab`
  `0x26294` — both unchanged from wave 108).

**Hypothesis (not proven):**

* The redundant inner `(int)` is a source-level device to keep clang from
  constant-folding the original `(int)(float)` round trip; the true original
  spelling may be a shared local or a differently-typed intermediate.
* The remaining ~51.7 % mismatch is still dominated by the original's
  block-tree/SLP layout (group A duplicated three ways, mid-block
  `vdup`/`vshl`/`vbsl` groups), not by these four statements.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5701
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 /dev/shm/w5701/score1u.py _ZN6LayoutC1Ev        # 7072 B, 48.269001193792
```

Scratch harness (all under `/dev/shm/w5701`): `vtest.py` (pristine-body
substitution + `link_so` + unrounded score), `score1u.py` (unrounded linked
scorer), `scan_triple.py` (all 205 `(int)(float)(int)` single edits),
`scan7048_ifelse.py` (all ternary→if/else on the strip body),
`scan_best_swap.py` (all adjacent swaps of the landed body), `t17.py`/`t20.py`
(3-combination searches), `apply_best.py` (re-applies the landed body).
