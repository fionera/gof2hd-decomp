# Layout::Layout C1/C2 — Android mid/tail expression trees, the float-store (vstr) set,
# and the 272-byte code/pool split (w4001)

Status: analysis-only pass (no code change). Worktree `wt/w4001`, branch `wave90-w4001`,
base `360c7467`. Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.

## Result

`_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` stay at **47.7% / 7072 B**. The gate is unchanged:

| metric | before | after |
|---|---:|---:|
| `byte_exact` | 4498 / 4524 | **4498 / 4524** |
| `linked_exact` | 4498 / 4524 | **4498 / 4524** |
| `size_exact` | 4524 / 4524 | **4524 / 4524** |
| `stub_zero_size` / `missing` / `wrong_type` / `extra` | 0 | **0** |
| `data_exact` | 843 / 843 | **843 / 843** |
| imports | 207 | **207** |
| `.text` | `0x1337c8` | **`0x1337c8`** |
| `.rodata` | `0x41b67` | **`0x41b67`** |
| lint / drift | CLEAN / 0 | **CLEAN / 0** |

`RATCHET PASS`.

Three concrete new results:

1. **The 272-byte gap is a code/pool split, not missing code.** The original
   `Layout::Layout` is 6188 B of code + 884 B of inline literal pool. The
   all-ternary body (w3801/w3821, 6800 B) is 6092 B code + 708 B pool. The
   current if/else-mixed body (7072 B) is 6296 B code + 776 B pool. The
   original is *smaller in code* than the current body by 108 B and *larger in
   pool* by 108 B; versus the all-ternary body it is +96 B code and +176 B
   pool. The "272-byte construct" is therefore not an assignment or a helper:
   it is 96 B of extra instruction selection plus 176 B of extra literal-pool
   words, both produced by the compiler from the same 204 assignments.
2. **The scalar float-store (`vstr`) set identifies the float-typed ternaries.**
   The original has exactly 27 `vstr sX,[r0,#off]` stores. The current source
   matches 20 of them, is missing 7, and emits 6 extra. This is the first
   precise, field-level codegen target recovered for this function.
3. **The Android mid/tail flag order is recoverable** from the `cmp/it/moveq`
   chains and from Ghidra's decompilation (address `0xe1028` = ELF `0xd1028`
   plus image base `0x10000`). The recovered order for the 28 fields that
   differ from iOS is unchanged from w3905; the mid/tail block uses the same
   `iPadHD → n9 → retinaDisplay → iPadLarge → iPad` skeleton, with `||`
   conditions for the `(retinaDisplay || n9)` and
   `(retinaDisplay || n9 || iPad)` cases.

None of the form/order fixes below improves the size-7072 fuzzy score, so no
code was landed.

## 1. Code / literal-pool split (the 272-byte question)

The function contains two inline literal pools (the first between the HD/n9
blocks and the retina block, the second after the final `pop`). The split is
measured from the last code instruction to the function end:

| body | size | code | code end | pool | pool start |
|---|---:|---:|---|---:|---|
| original `0xd1028` | 7072 | **6188** | `d2854` | **884** | `d2854` |
| current if/else-mixed | 7072 | 6296 | `d28c0` | 776 | `d28c0` |
| all-ternary (w3821) | 6800 | 6092 | `d27f4` | 708 | `d27f4` |

* original: `0xd2854 - 0xd1028 = 0x182c = 6188`; `0xd2bc8 - 0xd2854 = 0x374 = 884`.
* current: `0xd28c0 - 0xd1028 = 0x1898 = 6296`; `0xd2bc8 - 0xd28c0 = 0x308 = 776`.
* all-ternary: `0xd27f4 - 0xd1028 = 0x17cc = 6092`; `0xd2ab8 - 0xd27f4 = 0x2c4 = 708`.

The original's first pool word at `d2854` is `0x3ea66666` (`0.325f`); the
current body's pool starts at `d28c0` with a GOT-relative word (`0x0013debc`).
The original's code is 108 B shorter than the current body's, and its pool is
108 B longer. The all-ternary body is 96 B shorter in code and 176 B shorter
in pool than the original. So the total 272 B is *entirely* a code/pool
repartition; there is no missing assignment, helper, or cold block.

This also explains why the fuzzy score and the size trade off: the current
body buys 47.7% by using more 32-bit `str.w`/immediate forms (larger code,
smaller pool); the original uses more 16-bit forms and more pool loads (smaller
code, larger pool). The all-ternary body is the opposite extreme (50.8% at
6800 B).

## 2. The float-typed (`vstr`) field set

A field is *float-typed* when its ternary's common type is `float` (one branch
is a float expression), so the compiler converts the selected int to float and
back and stores the converted int with `vstr`. The original's 27 scalar
`vstr` stores are the exact fingerprint of this set:

```
orig  (27): 60 68 6c 70 2c8 2d0 8c 90 9c a8 dc e0 e4 e8 138 150 158 15c
            160 168 170 1a8 1e0 1e4 3e0 3f8 2f8
```

(`0x90 dc e0 e4 e8 1e0 2f8` are the real `float` members; the rest are `int`
members whose source ternary is float-typed.)

The current body emits 26 `vstr` stores:

```
ours  (26): 20 60 68 6c 70 1e0 2d0 2c8 8c 90 a8 e4 dc e0 e8 124 138 148
            204 160 168 1a8 3e0 3e8 1f4 2f8
```

Common (20): `60 68 6c 70 2c8 2d0 8c 90 a8 dc e0 e4 e8 138 160 168 1a8
1e0 3e0 2f8`.

| | offsets |
|---|---|
| original only (7) | `9c 150 158 15c 170 1e4 3f8` |
| current only (6) | `20 124 148 204 3e8 1f4` |

The all-ternary body emits a third set (27 stores):

```
alltern (27): 28 60 68 6c 70 1e0 2c8 2d0 8c 90 9c a8 e4 dc e0 e8 124
              10c 138 148 15c 168 1a8 3e0 3f4 1f4 2f8
```

Common with the original (21): `60 68 6c 70 2c8 2d0 8c 90 9c a8 dc e0 e4
e8 138 15c 168 1a8 1e0 3e0 2f8`; original-only (6): `150 158 160 170 1e4
3f8`; all-ternary-only (6): `28 124 10c 148 3f4 1f4`.

### Why the source form cannot simply be flipped

`(int)(float)v` is folded to `v` by clang at `-Oz` whenever `v` is a phi of
constants (verified for `-O1/-O2/-O3/-Os/-Oz`; the round-trip survives only
when the int comes from an opaque source such as a call). The original's
round-trip therefore implies its selected int is not a foldable constant phi;
the exact source shape that produces that is not yet identified. Measured on
the current body (all at size 7072 unless noted):

| change | size | pct |
|---|---:|---:|
| `0x20` `5*1.40625f` -> `7` | 6968 | 20.9 |
| `0x2d0` `45*1.40625f` -> `63` | 7056 | 47.3 |
| `0x148` `2*1.40625f` -> `2` | 7056 | 47.5 |
| `0x204` `9*1.40625f` -> `12` | 7064 | 47.4 |
| `0x1f4` `16*1.40625f` -> `22` | 7056 | 47.6 |
| `0x3e8` `100*1.40625f` -> `140` | 7076 | 42.4 |
| `0x150` `5` -> `5.0f` | 7080 | 47.7 |
| `0x158` `70` -> `70.0f` | 7072 | 47.7 |
| `0x170` `174` -> `174.0f` | 7072 | 47.7 |
| all 13 vstr-set fixes at once (6 removed + 7 added) | 6976 | 27.5 |
| all-ternary body + vstr-set fixes | 6916 | 42.6 |

So the current body is a local optimum in this direction. Forcing the vstr
set to match the original moves the code/pool split but destroys the
instruction stream.

## 3. Android mid/tail expression trees

The original is a tail-merged branch tree. Ghidra (project copy at
`/dev/shm/w4001`, `Layout::Layout` at `0xe1028`) decompiles it as:

```
if (iPadHD == 0) {
    if (n9 == 0) { ... retina/default ... } else { ... n9 ... }
    ... mid ...
} else { ... HD ... }
... tail ...
```

The first two flag tests are `cVar5 = **Globals::iPadHD` and
`cVar43 = **Globals::n9`; the decompiler then shows the mid block as a chain
of `if (cVar59 == 0)` / `if (*pcVar46 == 0)` tests with `moveq` selects. The
recovered per-field trees are the same as the w3905 table for the 28 fields
that differ from iOS (e.g. `2c0` = `H ? -2 : (N ? -3 : (R ? -4 : -2))`,
`2d8` = `H ? 141 : ((R || N) ? (iPad ? 200 : 125) : (iPad ? 125 : 75))`,
`3f8` = `H ? 132.0f : (N ? 111 : (L ? (R ? 254 : 127) : (R ? (iPad ? 144 :
186) : (rewardBoxY + 17) << R)))`).

The Android-specific `||` forms are visible in the binary as `orr`/`clz`
sequences, e.g. `d1248 orr.w r1,ip,sl; d124c clz r1,r1; d1258 mov.w lr,r1,lsr
#5` (the `(retinaDisplay || n9)` test) and `d1940 orr.w r2,fp,r3; d1944
orrs.w r1,r2,r8; d194c it eq; d194e addeq r1,#4` (the `retina || n9 || iPad`
test). These are the only places where the source is not a simple nested
ternary.

The float-typed fields are exactly the ones whose value is computed as
`vmov s0,rX; vcvt.f32.s32 s0,s0; vcvt.s32.f32 s0,s0; vstr s0,[r0,#off]`.
Examples from the original:

```
d1da6 vmov s0,r4          ; 0x158 value selected in r4
d1db0 vcvt.f32.s32 s0,s0
d1db4 vcvt.s32.f32 s0,s0
d1db8 vstr s0,[r0,#344]   ; 0x158

d1dd8 vmov s0,r4          ; 0x15c value selected in r4
d1dde vcvt.f32.s32 s0,s0
d1de2 vcvt.s32.f32 s0,s0
d1de6 vstr s0,[r0,#348]   ; 0x15c

d2514 add.w r2,r1,#17     ; rewardBoxY2 = (rewardBoxY + 17) << retinaDisplay
d2518 lsl.w r2,r2,r8
d251c vmov s0,r2
d2520 vcvt.f32.s32 s0,s0
d2524 vcvt.s32.f32 s0,s0
d252c vstr s0,[r0,#1016]  ; 0x3f8
```

The current source already has the `(rewardBoxY + 17) << retinaDisplay` form
but as an `int` if/else branch, so it does not emit the round-trip.

## 4. Sweep results (official verifier, `autojunk=False`)

All variants keep the 204 assignments and the 32-combination value table
(`_work/reconstructions/layout_ctor/lay_orig.json`, 0 bad combos).

| variant | size | pct |
|---|---:|---:|
| current `src/game/ui/Layout.cpp` | **7072** | **47.7** |
| current expressions, iOS source order | 6880 | 37.4 |
| current expressions, default-path store order | 6824 | 28.7 |
| all-ternary body (w3821) | 6800 | 50.8 |
| all-ternary body + vstr-set fixes | 6916 | 42.6 |
| current body + all vstr-set fixes | 6976 | 27.5 |
| iOS order, iOS trees refined with Android values | 6868 | 15.1 |
| iOS order, iOS trees + `*1.40625f` for every HD leaf | 4060 | 9.1 |
| iOS order, all-if/else chains | 8640 | 15.6 |

The w3905 exhaustive sub-sweeps (203 adjacent swaps, all 13 if/else->ternary
conversions, all 78 pairs and 715 four-way combinations, `field_0x98` block
moved to all 203 positions, all 190 single ternary->if/else conversions) stand:
the best size-7072 result is 47.7.

## 5. Reproduction

```sh
cmake --preset default
cmake -S . -B cmake-build-match \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8        # RATCHET PASS

# variant scorer (w4001 scratch)
python3 /dev/shm/w4001/score.py <Layout.cpp> <tag>
python3 /dev/shm/w4001/diffblocks.py /dev/shm/w4001/layout_<tag>.so _ZN6LayoutC1Ev

# vstr set
<ndk-objdump> -d --start-address=0xd1028 --stop-address=0xd2bc8 <so> |
  grep 'vstr'
```

Ghidra: the project's image base is `0x10000`; `Layout::Layout` is at
`0xe1028` (ELF `0xd1028`). A headless decompile script is
`/dev/shm/w4001/ghidra_scripts/W4001Decomp.java`.

## 6. Remaining work

* Recover the exact source form that makes clang keep the int->float->int
  round-trip for the 7 missing `vstr` fields (`9c 150 158 15c 170 1e4 3f8`)
  without changing the other 20. Clang folds `(int)(float)phi-of-constants` at
  every optimization level tested; the original's round-trip needs a
  non-foldable (opaque) int source, which has not been identified yet.
* Recover the exact statement order of the mid/tail blocks. The default-path
  store trace is the iOS order modulo `strd` pair swaps, but compiling the iOS
  order gives 6880 B / 37.4%, so the Android 2.0.16 source order is not the
  1.1.4 order (or the expressions differ enough to change the schedule).
* The all-ternary body is the highest fuzzy score (50.8) but is 272 B short;
  any landable candidate must be exactly 7072 B and must beat 47.7 at that
  size.
