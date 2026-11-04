# `RocketGun::update(int)` — pass 15 (wf210, wave 102)

**Status:** research note, 2026-11-04. Worktree `wt/wf210`, branch
`wave102-wf210`, base `53d9ee4d5`. **No source change landed**: every source
shape tried this pass scores <= 86.9 %, and — the new result of this pass —
the ~120 expression-level variants that do reach 86.9 % compile to
**byte-identical machine code** (same md5), so the residual is a hard
fixed point of the front end + register allocator, not a source-expression
choice.

Target `_ZN9RocketGun6updateEi`: original **1908 B / 635 insns**, ours
**1908 B / 636 insns**, official verify score **86.9**, `linked=False`.

Earlier notes: `rocketgun-update-wf109.md` (pass 14, NDK machine-IR + RA
trace), `rocketgun-update-w5010.md` (pass 13, slot weight model),
`rocketgun-update-w4812.md` (pass 11+12), `rocketgun-update-w4710.md`,
`rocketgun-update-w4512.md`, `rocketgun-update-w4412.md` (Ghidra decomp
appendix), and the earlier `-w4265`, `-w4248`, `-w4216`, `-w4111`, `-w4011`,
`-w3831`, `-w3811` notes.

---

## 1. Baseline and gate (this worktree)

```
cmake --preset default
cmake -S . -B cmake-build-match \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
```
```
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.91
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`_ZN9RocketGun6updateEi`: 1908 B both sides, **635 vs 636** instructions,
**86.9 %**. Every other `RocketGun` function is byte-exact.

Fast oracle used this pass (scratch only, `/dev/shm/wf210`):
`try.sh <variant.cpp>` = copy into `src/`, `ninja link_so`, then `score.py`
(imports `tools/verify/verify.py` and scores just this symbol with the
official `disassemble`/`reconcile`/`score` machinery, ~0.7 s/variant).
`h.py` prints the md5 of the function bytes.

---

## 2. New result: the 86.9 % plateau is a byte-identical fixed point

Every variant below scores **86.9 % / 1908 B / 636 insns** *and* produces the
**same 1908 function bytes**, md5 `ead3570211ebfdf4df8e3b2341c326b6`:

| variant | change |
|---|---|
| `base` | current checked-in source |
| `cl_base` | cluster block re-emitted verbatim |
| `cl_nfirst` | `n` declared before `total` |
| `cl_snc` | `s`/`c` non-`const` |
| `cl_stop` | `float s, c;` declared at loop top, assigned in the block |
| `cl_2s` / `cl_s2` | `(2.0f * s)` / `(s * 2.0f)` instead of `(s + s)` |
| `cl_gun` | `gun->…` instead of `this->gun->…` in both `+=` statements |
| `cl_nc` | `total`/`n`/`life` non-`const` |
| `cl_phase_i` | `total * i` instead of `total * (int) i` |
| `geom_bool` | `__builtin_expect(this->hasGeometry && this->gun->delayActive, 1)` |
| `geom_expect2` | extra parentheses inside `__builtin_expect` |
| `wasfiring` | `this->gun->delayActive` for `wasFiring` |
| `nog` | `this->gun->update(elapsed)` instead of `Gun &g` |
| `nog_noreload` | additionally drops every `gun = this->gun;` |
| `ship2` | named `const Matrix &ship2` for the `MatrixGetDir` argument |
| `upctor` | `Vector up = {0,1,0}` re-emitted |
| `scale_int` | explicit `int iscale` before the float `scale` |
| `c_gun` | `Cosf` argument uses `gun->…` instead of `this->gun->…` |
| `cross_gun` | `VectorCross(gun->velocities…, gun->upVectors…)` |
| `st_gun` | both `+=` statements use `gun->…` |
| `u_uint` / `u_long` / `u_arr` / `u_after_dt` | `unused` anchor type/placement variants |
| `ptrref` / `auto` | `Array<Matrix> *const` / `auto` for the trail local |
| `sarr2` | `float s[1]` with `s[0]` uses |
| `lifebase` | `const int *lifetimes = gun->lifetimes;` (base variant) |
| `a_thisgun` | `this->gun->…` in the `ahead` expression |
| `a_noreload1` / `a_noreload2` / `a_both_noreload` | drop the loop-top / second `gun = this->gun;` |
| `a_gunref` | named `Gun *g2` for the `ahead` expression |
| `a_split` | named `const Vector &p` / `const Vector &v` for the `ahead` operands |

Because the front end canonicalises all of these to the same IR, no
expression-level rewrite can move the allocator. The remaining difference
must come from an IR *shape* difference (a different set of stack objects,
or a different live range), not from how the expressions are spelled.

---

## 3. Anchor decomposition (re-measured)

The four documented anchors in the checked-in source:

| expect | unused | padx | trail local | pct | insns |
|---|---|---|---|---|---|
| 1 | 1 | 1 | 1 | **86.9** | 636 |
| 1 | 1 | 0 | 1 | 86.7 | 636 |
| 1 | 0 | 1 | 1 | 85.8 | 636 |
| 0 | 1 | 1 | 1 | 84.6 | 635 |
| 1 | 1 | 1 | 0 | 57.6 | 626 |
| 1 | 0 | 0 | 0 | 57.4 | 622 |
| 0 | 1 | 1 | 0 | 61.5 | 657 |
| 0 | 0 | 0 | 0 | 61.6 | 657 |

Removing `__builtin_expect` alone changes the allocation to `elapsed=r6`
(635 insns, 84.6 %); it does **not** change the two-`beq` branch structure
(both forms emit `cmp/beq; ldr/cmp/beq`), so the anchor is a *register
pressure* knob, not a branch-shape knob.

Removing the trail local (the faithful form per Ghidra: six `ldr [sl,#216]`
loads, no `[sp,#16]` slot) gives `elapsed@16` — the original's slot — but
moves `this` from `sl` to `fp` and drops to 626 insns / 57.6 %.

---

## 4. The `s` spill (the one structural difference left)

Original spills **both** trig results:

```
15f274:  str r0, [sp, #8]     ; s = Sinf(...)
15f29c:  str r0, [sp, #12]    ; c = Cosf(...)
```

Ours keeps `s` in `r6` and spills only `c` (`str r0,[sp,#8]`).  With `s`
spilled, `c` moves to `[sp,#12]`, `elapsed` to `[sp,#16]` next to `this` on
`[sp,#20]`, and the post-RA load/store optimiser merges the two stores into
`strd r4, sl, [sp,#16]` (635 insns).  That is the whole remaining delta.

Shapes tried this pass to force the `s` spill (all <= 86.9):

| variant | pct | insns |
|---|---|---|
| `volatile float s` | 76.1 | 637 |
| `volatile float s` + `volatile float c` | 76.4 | 638 |
| `float s[1]` + `s[0]` uses | 86.9 | 636 |
| `const int *lifetimes` used for both trig args (`lifebase3`) | 75.0 | 635 |
| `const int *lifetimes` used only for `Cosf` (`lifebase2`) | 84.1 | 635 |
| `total - lifetimes[i]` for both (`lifebase3`) | 75.0 | 635 |
| `const Vector keep = velocities[i]` live across `Sinf` | 86.9 | 636 |
| `Vector scaled = velocities[i] * dt` named temp | 61.1 | 635 |
| `Vector scaled` + notrail | 52.9 | 625 |
| `const Vector &vel = velocities[i]` | 85.4 | 637 |
| `Vector vel = velocities[i]` (copy) | 56.7 | 645 |
| `const Vector &pos = positions[i]` | 86.9 | 636 |
| `dt * 0.5f` parenthesised (one `operator*` instead of two) | 53.7 | 619 |
| `Vector cross; cross = VectorCross(...)` | 51.6 | 652 |
| inline `VectorNormalize(VectorCross(...))` | 60.0 | 628 |
| `const Vector norm = VectorNormalize(cross)` | 85.1 | 636 |
| `cross` / `c` / `s` reordered before the trig calls | 57.1 / 45.2 / 56.9 | 630-640 |
| `+=` statements swapped | 56.6 | 640 |
| named `v1`/`v2` temporaries for the two `+=` | 70.4 | 634 |

Distinct outputs (md5) reached this pass, for reference:

```
base     ead3570211ebfdf4df8e3b2341c326b6  1908 B
noun     a09084bc12c4ba5a87cd3be2727df570  1908 B
nopad    f795c096fcdbabcbefa497c2bb8b8661  1908 B
noexp    02bc4d8b73c7d28834941f6e4c953d41  1908 B
notrail  dc7dceb5359eeed3af525df7090e153f  1880 B
scaled   188c80f920adf6231d75617a476176ec  1900 B
lifebase2 705fd49420c0442df7ac64fa04584e55 1904 B
opidx    81120b2a34b428059dacef6a2ac32ad8  1908 B
velref   453762550e8c0c4755f767ebd71a6639  1908 B
```

---

## 5. Conclusion and remaining hypothesis

The pre-RA machine-IR instruction order already matches the original
one-for-one (same 60 `blx` sites, same constants, same literal pool).  The
residual is exactly one greedy-allocator decision:

```
%29  (elapsed)  weight 9.40e-4   -> r4 at entry
%251 (t2ADDri %stack.4.ref.tmp, 0, hoisted to the preheader by
      MachineLICM, weight 8.57e-3) evicts %29 from r4
=> elapsed ends in r5/r6, cursor i*60 in r9, positions.data_ in r4,
   temp1 in r6, s in r6, no strd
```

The original instead keeps `elapsed` in `r4` through the preheader, gives
`sl` to the hoisted temp address (spilling `this` in the loop), and spills
`s`.  The two levers pull in opposite directions, and the front end
canonicalises every expression-level shape tried (this pass and passes
2–14, ~500 variants total) to the same IR, so the plateau is a genuine
fixed point.

What would break it: an IR shape with a *different set of stack objects*
(so `%stack.4.ref.tmp` is not the single shared address), or a different
`elapsed` live range.  The one shape that does change the slot map is the
faithful no-trail-local form (`elapsed@16`, 626 insns), but it moves `this`
to `fp` and loses 29 points.  A source that keeps `this` in `sl` *and*
spills `s` while leaving `elapsed@16` has not been found.

---

## 6. Reproduction

```
# gate
cmake --build cmake-build-match --target check -j8

# side-by-side
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN9RocketGun6updateEi

# fast score + function md5 (scratch)
python3 /dev/shm/wf210/score.py _ZN9RocketGun6updateEi
python3 /dev/shm/wf210/h.py
```

Scratch lives in `/dev/shm/wf210`: `base.cpp`, `cc.sh`, `cc70.sh`,
`try.sh`, `score.py`, `h.py`, `udiff.py`, `sdiff.py`, `mdiff.py`,
`batch*.py`, `gen.py`, `rand.py`, `v_*.cpp`, `orig.asm`, `ours.asm`,
`notrail.asm`, `noexp.asm`, `scaled.asm`, `lb2.asm`; none of it is in the
repo.
