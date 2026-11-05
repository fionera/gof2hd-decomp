# `Level::createScene` — wave-117 w6511: ship-loop RA re-sweep and
# all-global/`enemies`-alias probes; 90.7243 % / 2252 B remains the maximum

**Status:** research note, 2026-11-05. Worktree `wt/w6511`, branch
`wave117-w6511`. Target `_ZN5Level11createSceneEv`.
**No source change landed** — the shipped wave-115 source is still the best
variant found. This commit contains only this note; `src/` is untouched.

Follow-up to `level-createscene-wave116-w6411.md` (and the chain back through
waves 88–116). Wave 116 re-swept the all-global ship loop and concluded the
local `rnd` pointer is the only mechanism that reproduces the original's
prologue / parts-loop allocation. This pass attacked the same tension from
two new directions: (1) a fresh sweep of the `Vector` address-trick
expression, and (2) source shapes that keep the local `rnd` pointer but
perturb the ship-loop live ranges (aliases, declaration hoisting, reloads,
two pointers, statement order).

## TL;DR

* Baseline (shipped wave-115 source): **90.7243 % / 2252 B**, 783/791
  instructions, first diff at instruction 462 (parts-loop preheader branch
  target `loc_610` vs `loc_614`, the known +4 shift from the trick's two
  16-bit instructions).
* The function is instruction-exact for instructions 0–461 and the parts
  loop is exact except for the +4 shift. All remaining diffs are the
  ship-loop register permutation (`rnd` value cached in `r5` vs the
  original's `&Globals::rnd` in `r8` and `ldr.w r0,[r8]` reloads) plus the
  resulting stack-slot and literal-pool shifts.
* No variant beat **90.7243 %**. The best new probes were:
  - all-global ship loop + a local `Array<KIPlayer *> *enemies = this->enemies`
    used at `ArrayAdd`: **76.2087 % / 2232 B** (this *does* fix the
    instruction-23 scheduling: `str r6,[sp,#48]` now sits after `cmp r0,#4`
    exactly like the original, but `Globals::status` still lands in `r4`
    instead of `sl`, and the ship loop stays permuted);
  - all-global ship loop alone: **75.6825 % / 2232 B**;
  - all-global + plain `setRotation(rot)`: **75.0318 % / 2228 B**;
  - plain `setRotation(rot)` with the local `rnd`: **85.3147 % / 2244 B**.
* The `-(g+1)` / `~g` trick remains the unique maximum. Re-swept expressions
  (all with the shipped local-`rnd` source): `-g` 90.3431, `+g` 90.3431,
  `g>>31` 90.5277, `g&1` 87.9847, `g>>32` 87.1338, `g+2` 87.9847,
  `g-1` 87.9847, `mesh` 89.4670, `k` 89.0724, `this` 87.7616, `race`
  84.5374.  `-(g+1)`, `~g`, `-1-g`, `(g+1)`-with-`size_t`/`uintptr_t`/
  `reinterpret_cast` casts all produce byte-identical 90.7243 % code.
* Ship-loop live-range probes that were neutral (identical 90.7243 % code):
  `int n = shipCount`, `race`/`type`/`spot`/`ship` declared outside the loop,
  `unsigned`/`bool`/`char`/`uint8_t` types for `capacity`/`shipCount`/
  `spotUsed`/`type`/`spot`/`race`/`i`/`k`, `const int *table` hoisted,
  `spots` declaration moved (before `rnd`, before `spotUsed`, inside the
  loop), `Level *self = this`, `restrict`/`const` on the `rnd` pointer,
  `Vector` brace/aggregate/field-assignment forms, `const Vector rot`,
  `Vector rp = rot` copies, a non-capturing lambda wrapper, and the
  `setPosition`/`setRotation` temporary splits.
* Worse: `Status *status` local (whole function 71.36 %, branch-local
  74.94 %), `Array<KIPlayer*> *enemies` alias on the shipped source
  (75.46 %), `volatile` `rnd` (78.10 %), `AERandom **rndp` two-pointer
  (68.43 %), mid-loop `rnd = Globals::rnd` reloads (88.18 %), swapping the
  station-100/`ownHangar` `if`s (89.20 %), `spot` before `createShip`
  (62.22 %), table selection before the retry loop (75.29 %), and
  `volatile int capacity` / `volatile int shipCount` on the
  all-global+`self`+`enemies` source (68.78 % / 72.01 %).

## 1. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6511
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 tools/verify/verify.py --original \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so --show _ZN5Level11createSceneEv
```

Scratch harness (`/dev/shm/w6511`, not committed): `harness.py` applies a
list of textual replacements to `src/game/world/Level.cpp`, rebuilds the
`link_so` target (≈2 s), and scores with `score.py` (imports the official
verifier and prints the unrounded difflib ratio, sizes and per-opcode diff).
`dump.py` prints the full normalised instruction stream of one symbol.

## 2. Why the all-global ship loop is still not reachable

Ghidra and the original Thumb code agree that the ship loop is the codegen of
the all-global source: `r8 = &Globals::rnd` (GOT slot `0x2101f8`) and
`ldr.w r0,[r8]` at all 8 `nextInt` sites.  A local
`AERandom *rnd = Globals::rnd` caches the value in `r5` instead.

Wave 116 established that the local pointer is what reproduces the original
prologue (`__stack_chk_guard` spilled to `[sp,#36]`/`[sp,#68]`) and the
parts-loop allocation (`r8 = Canvas`, `sl = 0`, `r4 = hangarPart` spilled to
`[sp,#44]`).  This pass confirmed that with the wave-115 trick the guard is
spilled even in the all-global source; the remaining early-code divergence
is the `Globals::status` register (`r4` vs the original's `sl`) and the
instruction-23 scheduling of `str r6,[sp,#48]`.

The `enemies`-alias probe is the first source-only change found that fixes
the instruction-23 scheduling in the all-global source (the spill moves
after `cmp r0,#4`).  Adding a `Level *self = this` alias as well makes the
all-global ship loop reload `this` from `[sp,#48]` at `createShip` (matching
the original) and frees `sl` from `this` -- but `sl` is then claimed by
`capacity`, and `Globals::status` still lands in `r4`/`r6` rather than the
original's `sl`.  The all-global + `self` + `enemies` variant is identical
to all-global + `enemies` (**76.2087 % / 2232 B / 789 insns**): ship loop
`r9=&rnd`, `sl=capacity`, `r5=race/ship`, `r8=type/spot`, `fp=i`,
`r4=spotUsed`, `r6=status/tries`, `this` reloaded from `[sp,#48]`; the
original has `r8=&rnd`, `r6=capacity`, `r5=race/ship`, `fp=type/spot`,
`r9=i`, `r4=spotUsed`, `sl=status/ship`.  Every attempt to free `sl` for
status (a `Status *status` local, `int n = shipCount`, `const capacity`,
a `cap2` alias, an `enemies` alias on the shipped source) either is CSE'd
back or moves the allocation further away.

## 3. Measurements (official single-function scorer, exact NDK r18b
clang 7.0.2 `-Oz` flags, gold link + strip + `reorder_shstrtab`)

| variant | pct | size | insns |
|---|---|---|---|
| **shipped (local `rnd`, `-(g+1)` trick)** | **90.7243** | **2252** | **791** |
| `~g` / `-1-g` / cast spellings of the trick | 90.7243 | 2252 | 791 |
| trick `g>>31` | 90.5277 | 2252 | 790 |
| trick `-g` / `+g` | 90.3431 | 2252 | 791 |
| trick `mesh` | 89.4670 | 2252 | 793 |
| trick `k` | 89.0724 | 2252 | 791 |
| trick `g&1` | 87.9847 | 2252 | 790 |
| trick `g>>32` | 87.1338 | 2244 | 787 |
| plain `setRotation(rot)`, local `rnd` | 85.3147 | 2244 | 790 |
| all-global + `enemies` alias | 76.2087 | 2232 | 789 |
| all-global ship loop | 75.6825 | 2232 | 792 |
| all-global + plain | 75.0318 | 2228 | 787 |
| all-global + plain + `int n` | 75.0318 | 2228 | 787 |
| `Status *status` (whole function) | 71.3555 | 2208 | 781 |
| `Status *status` (missionPtr==23 branch) | 74.9364 | 2228 | 789 |
| `volatile` `rnd` | 78.1013 | 2240 | 797 |
| `AERandom **rndp` (two pointers) | 68.4310 | 2240 | 804 |
| mid-loop `rnd = Globals::rnd` | 88.1830 | 2252 | 791 |
| swap station-100 / `ownHangar` `if`s | 89.1995 | 2252 | 791 |
| `spot` before `createShip` | 62.2166 | 2244 | 805 |
| table selection before retry loop | 75.2852 | 2272 | 795 |

Neutral (identical 90.7243 % / 2252 B / 791 insns): `int n = shipCount`;
`race`/`type`/`spot`/`ship` hoisted; `unsigned`/`bool`/`char`/`uint8_t`
types; `const int *table` hoisted; `spots` moved before `rnd`, before
`spotUsed`, or inside the loop; `Level *self = this`; `restrict`/`const`
`rnd`; `Vector` brace/aggregate/field forms; `const Vector rot`;
`Vector rp = rot`; lambda wrapper; `setPosition`/`setRotation` temporary
splits; `raceCopy`/`capCopy` aliases; `sceneRace` at function scope.

## 4. Bottom line

`Level::createScene` remains **90.7243 % / 2252 B** (783/791, first diff at
instruction 462).  The 0–461 prefix and the parts loop are exact; the
remaining ~60-instruction diff is the ship-loop register permutation caused
by the local `rnd` pointer required to reproduce the prologue and
parts-loop allocation.  Gate green before and after: **byte_exact 4501/4524,
linked_exact 4501/4524, stub_zero_size 0, missing 0, wrong_type 0, extra 0,
data_exact 843/843, imports 207, lint CLEAN, drift 0, RATCHET PASS.**
Structural state unchanged: 27 section headers, file size 2,187,328 B,
`.text` `0x1337c8`, `.rodata` `0x41c27`, `.ARM.exidx` `0x6f70`,
`.ARM.extab` `0x26298`.
