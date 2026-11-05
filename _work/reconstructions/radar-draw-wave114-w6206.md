# Radar::draw pass (wave114, w6206) — first-chain / 4-term-OR / block-layout sweep

**Status:** research note, 2026-11-05. Worktree `wt/w6206`, branch
`wave114-w6206`, based on current `master`. No source change landed; this commit
contains this note only. The target remains the pristine baseline.

Target `_ZN5Radar4drawEP6PlayerP3Hudi` in `src/game/weapons/Radar.cpp`
(the prompt's `src/game/ui/Radar.cpp` path is stale). Original 9908 B.

## 0. Baseline and gate

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
```

```
_ZN5Radar4drawEP6PlayerP3Hudi: osize 9908 usize 9908 raw 74.029509 %
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS   (lint CLEAN, drift 0)
```

Scores below are the official verifier API
(`difflib.SequenceMatcher(None, on, un, autojunk=False).ratio()` at six
decimals). Baseline instruction count: **3492 original / 3489 ours**.
Structural state re-measured this wave and unchanged: 27 sections, exact file
size **2,187,328 B**, `.text` **1,259,464 B** on both sides, `.ARM.extab`
**156,312 B (0x26298)** on both sides.

## 1. The first `otherLock` chain — original shape re-confirmed

Original `0x2fc..0x312` (function-relative):

```
2fc mov  r1, r4
2fe ldr.w r0, [r1, #12]!      ; &lockedAsteroid, load lockedAsteroid
302 str  r1, [sp, #124]
304 cbnz r0, 312              ; a != null -> set
306 ldr  r0, [r4, #56]        ; lockedGasCloud
308 cbnz r0, 312              ; b != null -> set
30a ldr  r0, [r4, #28]        ; field_0x1c
30c cmp  r0, #0
30e beq.w ebc2                ; c == 0 -> cold: otherLock = field_0x14 != 0
312 movs r5, #1               ; set
314 ldr  r0, [r4, #312]       ; landmark guard
```

Cold block `0xebc2`:

```
ebc2 ldr  r5, [r4, #20]       ; field_0x14
ebc4 cmp  r5, #0
ebc6 it   ne
ebc8 movne r5, #1
ebca b.w  2c4                 ; landmark guard
```

Ours (`0x2fc..0x338`):

```
2fc mov  r1, r4
2fe ldr.w r0, [r1, #12]!
302 str  r1, [sp, #124]
304 cbz  r0, 324              ; a == null -> test b
306 mov.w r9, #1              ; set copy 1
30a ldr  r5, [sp, #156]
30c b.n  338                  ; guard
324 ldr  r0, [r4, #56]
326 ldr  r5, [sp, #156]
328 cmp  r0, #0
32a itt  eq
32c ldreq r0, [r4, #28]
32e cmpeq r0, #0
330 beq.w 1305fa              ; cold (r9)
334 mov.w r9, #1              ; set copy 2
338 ldr  r0, [r4, #312]
```

The original keeps **one** set block (`movs r5,#1`) reached by taken branches
from both the `a` and `b` tests; ours duplicates it and merges the `b`/`c`
tests into an `itt eq` pair. `otherLock` is `r5` in the original and `r9` in
ours; the guard's `Globals::status` is `r6` in the original and `r5` in ours.
The second `otherLock` chain at `0x376` (`a||b||c||lockedStation`) already
matches instruction-for-instruction (`cbnz` chain + far cold block), so the
difference is specific to the first (phi-valued) chain.

## 2. First-chain spelling sweep this wave (all dead ends)

Negative-AND hint sweep, outer × `lockedAsteroid` × `lockedGasCloud`
(`n` = no hint, `0/1` = `__builtin_expect(..., 0/1)`; `field_0x1c` unhinted):

| hints | usize | raw % |
|---|---:|---:|
| `0n1` (= landed baseline placement) | 9908 | **74.029509** |
| `0nn` | 9908 | 74.000859 |
| `n11` | 9908 | 72.836676 |
| `nnn` | 9908 | 72.797593 |
| `nn0` | 9908 | 71.985105 |
| `n0n` / `n01` | 9908 | 71.956459 |
| `001` | 9908 | 71.891117 |
| `01n` / `011` | 9916 | 71.888873 |
| `n10` / `010` | 9916 | 71.870524 |
| `0n0` | 9908 | 71.692705 |
| `n1n` | 9908 | 71.680275 |
| `00n` | 9908 | 71.635373 |
| `nn1` | 9892 | 71.625107 |
| `1nn`..`111` (outer likely) | 9912 | 70.915877 (all) |
| `n00` | 9912 | 60.481100 |
| `000` | 9912 | 60.789926 |

**No negative-AND spelling produced a `cbnz` chain.** The outer-`1` forms
produce `cmp/itt eq` + far `bne.w` (the set block moves out of line and the
cold `field_0x14` block becomes the fallthrough), not the original's shape.

Other first-chain source forms (each replaces only the chain on the baseline):

| spelling | usize | raw % | cbnz in chain |
|---|---:|---:|---:|
| `goto` neg-AND | 9908 | 72.797593 | 0 |
| `int x = 1; if (AND) x = d!=0;` | 9908 | 72.797593 | 0 |
| `int x = d!=0; if (!AND) x = 1;` | 9908 | 69.754768 | 0 |
| same + `expect(!AND,1)` | 9908 | 72.925326 | 0 |
| `bool any = !AND; if (any)...` | 9908 | 72.797593 | 0 |
| ternary | 9908 | 72.797593 | 0 |
| nested ifs | 9908 | 72.797593 | 0 |
| `int x=1; if (expect(AND,0)) ...` | 9908 | 74.000859 | 0 |
| `int x=1; if (expect(AND,1)) ...` | 9912 | 70.915877 | 0 |
| goto + `expect(AND,0)` | 9908 | 74.000859 | 0 |
| bitwise `&` AND | 9952 | 63.837109 | 1 (wrong block) |
| three pointer locals | 9952 | 63.837109 | 1 (wrong block) |
| three pointer locals, OR form | 9952 | 63.837109 | 1 (wrong block) |
| `init d!=0; if (a) x=1; else if (b)...` | 9908 | 72.925326 | 0 |
| `init 1; if (AND) x=d!=0;` | 9908 | 72.797593 | 0 |
| `a && (b & c)` | 9908 | 72.159335 | 0 |
| `a && !(b!=0 || c!=0)` | 9908 | 74.000859 | 0 |
| `a && (b && c)` | 9908 | 74.000859 | 0 |
| separate `expect` on a,b,c + `expect(AND,0)` | 9908 | 71.635373 | 0 |
| separate `expect` on a,b,c + `expect(AND,1)` | 9912 | 70.915877 | 0 |

Term permutations (hint kept on the gasCloud term, outer 0): `abc` 74.029509,
`acb` 74.000859, `bac` 70.976064, `bca` 71.004730, `cab` 66.418445,
`cba` 66.418445. All still `cbz`/`itt eq`.

`otherLock` type: `int` 74.029509, `unsigned char` 74.029509, `bool`
67.853042, `int x = 0;` declaration 74.029509.

### 2a. The 4-term-OR equivalence (new idea this wave)

`(a||b||c) ? 1 : (d!=0)` is logically identical to `a||b||c||(d!=0)`, and the
second chain (`a||b||c||lockedStation`) compiles to the desired `cbnz` chain.
Writing the first chain as a single 4-term OR therefore looked promising:

```cpp
otherLock = this->lockedAsteroid != nullptr || this->lockedGasCloud != nullptr ||
            this->field_0x1c != nullptr || this->field_0x14 != 0;
```

Measured: **9904 B / 70.549419 %** (all groupings `left`, `(a||b)||(c||d)`,
`a||(b||c)||d`, `a||b||(c||d)`, `(a||b||c)||d`, `bool any = ...` are
byte-identical). `__builtin_expect(whole, 0)` = 71.362984; `1` = 70.549419.
The compiler still emits `cmp r0,#0; itt eq; ldreq b; cmpeq c; beq cold;
movs r5,#1` — the `b`/`c` merge and the missing cold-block placement are not
caused by the phi, so the 4-term OR does not reproduce the original chain.

## 3. Guard / planet-condition hints (all dead ends)

| hint | raw % |
|---|---:|
| baseline | **74.029509** |
| `expect(landmarks != nullptr, 1)` | 64.647625 |
| `expect(landmarks != nullptr, 0)` | 56.455769 |
| `expect(campaign >= 2, 1)` | 69.255014 |
| `expect(campaign >= 2, 0)` | 42.186606 |
| `expect(planetTargets != nullptr, 1)` | 74.029509 |
| `expect(planetTargets != nullptr, 0)` | 43.832882 |
| `expect(!alienOrbit, 1)` | 74.029509 |
| `expect(!alienOrbit, 0)` | 43.832882 |
| `expect(!isDockingToPlanet(), 1)` | 69.340974 |
| `expect(!isDockingToPlanet(), 0)` | 43.987432 |
| guard + campaign both `1` | 68.510821 |
| guard + campaign both `0` | 42.052744 |

## 4. Landmark `i <= 1` block (32-instruction mismatch)

Original layout (`0x9ba..0xaec`): the **generic `else` branch** (calcDistance +
DrawString) is emitted first at `0x9ba..0x9e8` and ends with `b.n 0xaec`; then
the `else if ((i&0x7fffffff)==1||==3)` DrawImage2D at `0x9ea..0xa18`; then the
`i==0 && !alienOrbit` branch at `0xa2a..0xacc`. Ours emits the `i==0` branch
first. The original's else-if is the `bic.w r0,sl,#0x80000000; cmp #1; beq;
cmp #3; bne` form (two compares), ours is the one-instruction `orr sl,#2`
form.

Reordering the `i<=1` block (then/else swap, nested forms, `i < 2`):

| spelling | usize | raw % |
|---|---:|---:|
| baseline | 9908 | **74.029509** |
| `if (!(i==0 && !alienOrbit)) {generic} else {i0}` | 9908 | 72.289847 |
| `if (i != 0 || alienOrbit) {generic} else {i0}` | 9908 | 72.289847 |
| `if (alienOrbit || i != 0) {generic} else {i0}` | 9908 | 72.206304 |
| `if (!alienOrbit && i == 0) {i0} else {generic}` | 9908 | 72.144289 |
| `if (i==0) { if (!alienOrbit) i0 else generic } else generic` | 9908 | 71.064254 |
| `if (i!=0) generic else if (alienOrbit) generic else i0` | 9908 | 71.306087 |
| `if (i==0 && !alienOrbit) i0 else generic` (same as baseline) | 9908 | 74.029509 |
| `i < 2` instead of `i <= 1` | 9908 | 74.029509 |

Two separate `else if` (to get the original `bic` form):

| spelling | usize | raw % |
|---|---:|---:|
| two plain `else if` | 9916 | 72.318488 |
| two `else if`, both `expect(...,0)` | 9916 | 72.472071 |
| first hinted only | 9916 | 72.318488 |
| second hinted only | 9916 | 71.086925 |
| no mask (`i==1`/`i==3`) | 9916 | 72.318488 |
| no mask + both hinted | 9916 | 72.472071 |
| reversed (`==3` first) | 9916 | 71.144207 |

Hoisting the shared `landmark = data()[i]` + `calcDistance` out of both
branches: 9816 B / 69.506404 % (the original really does have two separate
`calcDistance` calls).

## 5. Planet tail / recompute placement (all dead ends)

Original planet tail is at `0x6f8..0x748` (before the landmark loop) and ends
with `b.n 0xbd4` (enemies). Ours planet tail is at `0xeb46..0xeb88` (after the
landmark loop) and ends with `b.w 0x378` (second chain). The baseline's
semantically-wrong post-planet-loop `onPlanetCourse` recompute (33 extra
instructions, `insert: orig[1077] (0) ours[1051:1084] (33)`) is what keeps the
second chain at `0x378` and the planet block in the original's slot.

| variant | usize | raw % |
|---|---:|---:|
| baseline (recompute kept) | 9908 | **74.029509** |
| recompute deleted (`nodup`) | 9756 | 62.314026 |
| recompute before the planet loop | 9868 | 63.977028 |
| recompute after the planet tail | 9936 | 62.067976 |
| early block trivialised, recompute kept | 9756 | 60.709096 |
| early block reduced to alienOrbit-only, recompute kept | 9756 | 61.500579 |
| both simplified | 9756 | 60.709096 |

## 6. Enemy in-sight block — exact schedule reached, but asteroid section re-colours

The original keeps `crosshairPos.x/.y` in `s26`/`s24` across
`AEMath::operator-`, converts both with `vcvt.s32.f32` **before**
`AEMath::VectorLength`, and then does the four bounds tests. That is exactly
the distance-first `int cx/cy` source:

```cpp
enemy->field_0x73 = 0;
int cx = (int) PlayerEgo::crosshairPos.x;
int cy = (int) PlayerEgo::crosshairPos.y;
bool inSight = false;
if (AbyssEngine::AEMath::VectorLength(this->egoPos - this->targetPos) < 60000.0f &&
    this->screenX > cx - this->field_0x124 && this->screenX < cx + this->field_0x124 &&
    this->screenY > cy - this->field_0x124 && this->screenY < cy + this->field_0x124) {
    inSight = true;
}
enemy->field_0x73 = inSight;
```

This variant (`V_insf4`) reproduces the original's `vpush {d8-d13}` prologue,
the `vldr s26/s24` before the call, the `vcvt.s32.f32 s24/s26` before
`VectorLength`, and the bounds body instruction-for-instruction (only the
`r5`/`r6`/`r1`/`r2` colouring differs). But it scores **9896 B / 67.517202 %**:
the `d8-d13` live range re-colours the asteroid section, producing
90-instruction and 79-instruction mismatches at `orig[2717:2807]` and
`orig[2829:2908]` that the baseline does not have. The `float cx/cy` form
(`V_f5`) is 9896 B / 67.459862 %.

## 7. Misc hints (all neutral or worse)

| hint | raw % |
|---|---:|
| baseline | **74.029509** |
| `expect(route != nullptr, 1)` | 74.029509 (neutral) |
| `expect(route != nullptr, 0)` | 69.969927 |
| `expect(i != 3, 1)` | 72.714265 |
| `expect(i != 3, 0)` | 72.185620 |
| `expect(i <= 1, 1)` | 66.103636 |
| `expect(i <= 1, 0)` | 61.485419 |
| `expect(i == 0 && !alienOrbit, 1)` | 62.177650 |
| `expect(i == 0 && !alienOrbit, 0)` | 63.627240 |
| `expect(!turretMode, 1)` | 70.591606 |
| `expect(!turretMode, 0)` | 61.334095 |
| `expect(landmarkHit, 1)` | 70.869752 |
| `expect(landmarkHit, 0)` | 63.547926 |

## 8. Re-measurement of every existing variant in `/dev/shm/w6206`

All `*.cpp` variants from waves 110–113 were rebuilt and scored. The maximum is
**74.029509 % / 9908 B**, reached by `N5_tern`, `N3_1minus`, `N2_eq0`,
`N1_xor`, `K3_expect_sub`, `AC_lh0` and the pristine baseline. Nothing
exceeds it. Best non-baseline: `S1_second_field14` / `M1_outer_only` /
`E4_pos_or_expect` / `E3_neg_expect0` at 74.000859.

## 9. Conclusion

No variant measured this wave beats the pristine baseline
**9908 B / 74.029509 %**. The remaining gap is the same self-consistent
compiler artefact documented in waves 112/113, now narrowed to:

1. the first chain's `cbnz`-chain + single inline set block + far cold
   `field_0x14` block, with `otherLock` in `r5` and `Globals::status` in `r6`
   (ours: `cbz` + duplicated set block + `itt eq` merge, `otherLock` in `r9`,
   status in `r5`, cold block at `0x25aa` instead of `0xb72`);
2. the planet tail before the landmark loop (original `0x6f8`, ours `0xeb46`);
3. the landmark `i<=1` block rotation and the un-merged `DrawString` pair;
4. the `d8-d13` float allocation, which is reachable only by re-colouring the
   asteroid section.

The baseline's extra (semantically wrong) post-planet-loop recompute is what
keeps (2) aligned; deleting it collapses the score to 62.31 %. The baseline is
a tuned local optimum of the fuzzy metric.

The worktree was restored to the pristine `Radar.cpp` before the final gate.
`check` is green: byte 4500, linked 4500, lint CLEAN, drift 0, data 843/843,
imports 207, `RATCHET PASS`. Structural state unchanged: 27 sections, file
size 2,187,328 B, `.text` 1,259,464 B, `.ARM.extab` 156,312 B (0x26298).

## 10. Scratch

All scratch under `/dev/shm/w6206` (not committed): `Radar.base.cpp`,
`measure.py`, `rebuild.sh`, `dumpvar.py`, `gap.py`, `coldaddr.py`,
`sweep_neg.py`, `sweep_forms.py`..`sweep_forms4.py`, `sweep_perm.py`,
`sweep_4or.py`, `sweep_guard.py`, `sweep_guard2.py`, `sweep_i1.py`,
`sweep_elseif.py`, `sweep_recompute.py`, `sweep_misc.py`, `sweep_otype.py`,
`runall.py`, `V_*.cpp` (the variants tabulated above), `rea.json`/`rea.c`
(Ghidra), `orig.dis`/`ours.dis`.
