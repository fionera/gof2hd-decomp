# `Level::createScene` — wave-118 w6611: exhaustive mask / plain-`setRotation`
# / pointer-spelling sweeps; 90.7243 % / 2252 B remains the maximum

**Status:** research note, 2026-11-05. Worktree `wt/w6611`, branch
`wave118-w6611`. Target `_ZN5Level11createSceneEv`.
**No source change landed** — the shipped wave-115 source is still the best
variant found. This commit contains only this note; `src/` is untouched.

Follow-up to `level-createscene-wave117-w6511.md` (and the chain back through
waves 88–117). Wave 117 re-swept the ship-loop register allocation and the
`Vector` address trick. This pass re-ran the *complete* 8-site `rnd`
local/global mask sweep twice (once with the local declaration in its shipped
position, once moved before `shipCount` so the `shipCount` site can be local),
swept the `plain setRotation(rot)` form against all 128 masks, and tested
pointer spellings (`&`, `*const`, `*volatile`, `**`, `volatile **`, reference
to the pointer, reference to the object) plus a broad set of type / scope /
statement-order perturbations. Nothing beat **90.7243 %**.

## TL;DR

* Baseline (shipped wave-115 source): **90.7243 % / 2252 B**, 783/791
  instructions, first diff at instruction 462 (parts-loop preheader branch
  target `loc_610` vs `loc_614`, the known +4 shift from the trick's two
  16-bit instructions).
* The function is instruction-exact for instructions 0–461 and the parts
  loop is exact except for the +4 shift. All remaining diffs are the
  ship-loop register permutation plus the resulting stack-slot and
  literal-pool shifts.
* **Mask sweep, A = global (shipped declaration position), 128 masks:**
  mask 94 (`0b01011110`; A, F, H global — exactly the shipped source) is the
  unique maximum at 90.7243. Runner-up mask 220 (`0b11011100`) 90.5972;
  next group (206/214/218/236/244/248) 89.9619.
* **Mask sweep, A = local (declaration hoisted before `shipCount`), 128
  masks:** best is 77.3045 (masks 11/13/19/21/25/26/28). Making the
  `shipCount` call use the local value costs ~13 points.
* **`plain setRotation(rot)` × 128 masks:** best is 88.3100 (mask 82).
  Plain never reproduces the original's parts-loop allocation: it always
  picks `r4` as the zero register and spills `r6` (the parts-loop `k`),
  whereas the original uses `sl` as zero and keeps `r6`.
* Pointer spellings: `AERandom *const rnd` and `AERandom &rnd = *Globals::rnd`
  are byte-identical to the shipped 90.7243; `AERandom *&rnd = Globals::rnd`,
  `AERandom **rndp = &Globals::rnd`, and
  `AERandom *volatile *rndp = &Globals::rnd` all collapse to the *all-global*
  codegen (75.6825) because clang treats the reference/address as the global
  itself. `AERandom *volatile rnd` is 78.1013.
* Every type / scope / statement-order perturbation tested is neutral
  (identical 90.7243 % / 2252 B / 791 insns) or worse. The only worse
  non-neutral ones worth recording: `mesh == -1` 90.5972, `tries = -100;
  tries++` 90.0317, `plainHangar == false` 89.3968, shared fill/ship `i`
  84.8601, `spot` before `createShip` 62.2166, `do { } while` 87.6590.

## 1. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6611
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 tools/verify/verify.py --original \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so --show _ZN5Level11createSceneEv
```

Scratch harness (`/dev/shm/w6611`, not committed): `harness.py` applies a
list of textual replacements to `src/game/world/Level.cpp`, rebuilds the
`link_so` target (≈2 s), and scores with `score.py` (imports the official
verifier and prints the unrounded difflib ratio, sizes and per-opcode diff).
`dump2.py` prints the full raw instruction stream (with byte offsets) of one
symbol. Each sweep restores the pristine source afterwards.

## 2. The two allocation basins

The original ship loop is unambiguously the codegen of the **all-global**
source (`Globals::rnd->nextInt(...)` at all 8 sites): `r8 = &Globals::rnd`
(GOT slot) and `ldr.w r0,[r8]` at every site. But its *early* allocation
(instructions 0–461 and the parts loop) is the one produced by the shipped
**local-value** source (`AERandom *rnd = Globals::rnd;` at 5 sites):

| value | original | shipped (local value) | all-global |
|---|---|---|---|
| `Globals::status` | `sl` | `sl` | `r4` (spilled to `[sp,#40]`) |
| `this` spill `str r6,[sp,#48]` | after `cmp r0,#4` (insn 25) | same | after `cmp r0,#2` (insn 23) |
| parts-loop table base | `r9` | `r9` | `r8` |
| parts-loop `Globals::Canvas` | `r8` | `r8` | `r9` |
| parts-loop zero register | `sl` | `sl` | `sl` |
| ship-loop `&Globals::rnd` | `r8` | `[sp,#44]` (value in `r5`) | `r9` |
| ship-loop `capacity` | `r6` | `fp` | `r8` |
| ship-loop `race` | `r5` | `r6` | `r5` |
| ship-loop `type` | `fp` | `r8` | `fp` |

So the original is a **mix**: the parts loop / prologue of the local-value
variant and the ship loop of the all-global variant. Every source change
tried moves the whole function into one basin or the other:

* local-value spellings (`AERandom *rnd`, `*const`, reference-to-object)
  → Basin B (early allocation right, ship loop `r5`-cached) = 90.7243;
* address spellings (`*&`, `**`, `volatile **`, `Globals::rnd` directly)
  → Basin A (ship loop `r8`/`r9` address + reloads, early allocation wrong)
  = 75.6825.

The trick in the parts loop (`Vector *rp = (Vector*)((char*)&rot -
((size_t)g + 1)); g->setRotation(*rp);`) is what forces Basin B's parts-loop
allocation while keeping the ship loop's local-value form. It costs two
16-bit instructions (`mvns r0,r5; add r1,r0`) and therefore shifts every
later branch target by +4 bytes. Replacing it with the semantically valid
`g->setRotation(rot)` drops to 85.3147 because the allocator then picks `r4`
as the zero register and spills `r6`.

## 3. Measurements (official single-function scorer, exact NDK r18b
clang 7.0.2 `-Oz` flags, gold link + strip + `reorder_shstrtab`)

| variant | pct | size | insns |
|---|---|---|---|
| **shipped (local `rnd`, `-(g+1)` trick)** | **90.7243** | **2252** | **791** |
| mask 220 (`0b11011100`) | 90.5972 | 2252 | 791 |
| `mesh == -1` instead of `mesh < 0` | 90.5972 | 2252 | 791 |
| `g >> 31` address trick | 90.5277 | 2252 | 790 |
| `- (size_t) g` (1-instruction trick) | 90.3431 | 2252 | 791 |
| `tries = -100; tries++` | 90.0317 | 2252 | 792 |
| masks 206/214/218/236/244/248 | 89.9619 | 2252 | 791 |
| `plainHangar == false` | 89.3968 | 2252 | 792 |
| plain `setRotation(rot)` + mask 82 | 88.3100 | 2252 | 791 |
| `do { } while (i < shipCount)` | 87.6590 | 2248 | 789 |
| shared fill/ship `i` | 84.8601 | 2252 | 789 |
| plain `setRotation(rot)` (shipped mask) | 85.3147 | 2244 | 790 |
| `AERandom *volatile rnd` | 78.1013 | 2240 | 797 |
| mask A=local (best 11/13/19/21/25/26/28) | 77.3045 | 2232 | 790 |
| all-global (`Globals::rnd` at all 8 sites) | 75.6825 | 2232 | 792 |
| all-global + plain `setRotation` | 75.0318 | 2228 | 787 |

Neutral (identical 90.7243 % / 2252 B / 791 insns): `int`→`unsigned` for
`capacity`/`shipCount`/`race`/`type`/`spot`/`i`; `size_t capacity`;
`int ownHangar`; `bool`/`char`/`unsigned char *spotUsed`; `spotUsed[spot]`
instead of `!= 0`; `++i`/`++k`/`++j`; `while` loop; `int i` at function or
pre-parts scope; `const int capacity`; `AERandom *const rnd`;
`AERandom &rnd = *Globals::rnd`; `const Vector rot`; `Vector` brace /
field-assignment / temporary forms; `Vector &rr = rot`; `AEGeometry *gp = g`
alias; `plainHangar` if/else form; `spots` moved before `spotUsed` or before
`rnd`.

Worse: `Status *status = Globals::status` local (whole branch) 75.68 with
`status` still in `r4`; `Level *self = this` / `Array *enemies` aliases
(75.68); `int n = shipCount` loop bound (75.68 with the trick, 75.03 with
plain); `int cap2 = capacity` / `race2` aliases (75.68/75.97); comma /
ternary spellings that route the call through `Globals::rnd` (75.68).

## 4. Bottom line

`Level::createScene` remains **90.7243 % / 2252 B** (783/791, first diff at
instruction 462). The 0–461 prefix and the parts loop are exact; the
remaining ~60-instruction diff is the ship-loop register permutation caused
by the local-value `rnd` pointer required to reproduce the prologue and
parts-loop allocation. The original's all-global ship loop and its
local-value-style early allocation could not be combined by any source
spelling tried. Gate green before and after: **byte_exact 4501/4524,
linked_exact 4501/4524, stub_zero_size 0, missing 0, wrong_type 0, extra 0,
data_exact 843/843, imports 207, lint CLEAN, drift 0, RATCHET PASS.**
Structural state unchanged: 27 section headers, file size 2,187,328 B,
`.text` `0x1337c8`, `.rodata` `0x41c27`, `.ARM.exidx` `0x6f70`,
`.ARM.extab` `0x26298`.
