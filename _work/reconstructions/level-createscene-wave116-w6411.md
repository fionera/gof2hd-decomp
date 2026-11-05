# `Level::createScene` — wave-116 w6411: faithful all-global ship loop
# re-sweep; 90.7243 % / 2252 B remains the maximum

**Status:** research note, 2026-11-05. Worktree `wt/w6411`, branch
`wave116-w6411`. Target `_ZN5Level11createSceneEv`.
**No source change landed** — the shipped wave-115 source is still the best
variant found. This commit contains only this note; `src/` is untouched.

Follow-up to `level-createscene-wave115-w6312.md` (and the chain back through
waves 88–115). Wave 115 landed the `&rot - ((size_t) g + 1)` address
expression (90.5277 → 90.7243 %). This pass attacked the two remaining diff
clusters from the opposite direction: (1) reproduce the original's
*all-global* ship loop (`r8 = &Globals::rnd`, `ldr.w r0,[r8]` at every
`nextInt` site) while keeping the wave-115 parts-loop register assignment,
and (2) find a source shape that fixes the ship-loop register permutation
without the fake rnd pointer.

## TL;DR

* Baseline (shipped source): **90.7243 % / 2252 B**, 783/791 instructions,
  first diff at instruction 462 (parts-loop preheader branch target
  `loc_610` vs `loc_614`, the known +4 shift from the trick's two 16-bit
  instructions).
* The function is **instruction-exact for instructions 0–461** (missionPtr
  dispatch, missionPtr==2 shield/beam branch, missionPtr==4 bar branch) and
  the parts loop is exact except for the +4 shift.  All remaining diffs are
  in the ship loop (register permutation + the `&Globals::rnd` spill).
* Ghidra confirms the original ship loop is the codegen of the **all-global**
  source: `AERandom::nextInt(Globals::rnd, ...)` at all 8 sites, and the
  retry loop is `iVar21 = -100; while (iVar21 != 0 && ...) iVar21++` (count
  up, not down).  There is no local rnd pointer in the original.
* But the local rnd pointer is exactly what forces the original's
  `__stack_chk_guard` spill (`str r0,[sp,#36]`) and the parts-loop
  `r9 = row / r8 = Canvas` assignment.  Removing it drops the score to
  **75.03 % / 2228 B** (plain) or **75.68 % / 2232 B** (trick).
* New measurements this pass (exact NDK r18b clang 7.0.2 `-Oz` flags, gold
  link + strip + `reorder_shstrtab`, official single-function scorer):
  - all-global, plain `setRotation(rot)`: **75.0318 % / 2228 B**
  - all-global, trick: **75.6825 % / 2232 B**
  - all-global, plain + `int n = shipCount` loop bound: **80.0769 % / 2212 B**
    (this variant *does* get `r8 = &Globals::rnd`, `r9 = i`, `r4 = spotUsed`
    right, but its prologue still keeps the guard in `r5` and the parts loop
    is `r8 = row / sl = Canvas / r9 = zero`)
  - local rnd at all 8 sites: **85.9873 % / 2244 B**
  - local rnd at 5 sites + `int n`: **86.9732 % / 2236 B**
  - local rnd at 5 sites, plain: **85.3147 % / 2244 B**
  - local rnd at 5 sites, 1-instruction trick (`&rot - (size_t)g`):
    **90.3431 % / 2252 B** (correct parts-loop registers `r9=row`,
    `r8=Canvas`, `sl=zero`, but only 1 extra instruction and a different
    scheduling: `mov.w sl,#0` is hoisted above `add r1,sp,#56`)
  - local rnd at 5 sites, 2-instruction trick (shipped): **90.7243 %**
* Full 128-combination sweep of which of the 8 `nextInt` sites use the local
  rnd vs `Globals::rnd`: the shipped mask (sites a–e = the 3 `nextInt(100)`
  calls, `nextInt(4)`, and the retry `nextInt(capacity)`; sites f–g =
  `shipCount` and the `nextInt(300)` rotation) is the unique maximum.
* Retry-loop direction, loop-variable type/scope, `capacity`/`shipCount`/
  `race`/`type`/`spot` types, `spotUsed` element type, shared `i`, `spots`
  declaration order, `table` switch layout (default-first, no-default,
  if/else chain, ternary), `hangarPart` declaration form, `Vector`
  construction form, and 16 `rnd` declaration forms are all neutral or worse.

## 1. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6411
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 tools/verify/verify.py --original \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so --show _ZN5Level11createSceneEv
```

Scratch harness (`/dev/shm/w6411`, not committed): `score.py` imports the
official verifier and prints the unrounded difflib ratio, sizes and the
first differing instruction; `score.py ... diff` prints the opcode hunks.
`bs.sh` recompiles `Level.cpp` with the exact NDK r18b clang 7.0.2 `-Oz`
flags and re-runs the saved gold link + `strip --strip-all` +
`reorder_shstrtab.py`, then scores.

## 2. Why the parts loop and the ship loop are in tension

The original parts loop (`0xb4860`):

```
add  r1, sp, #56      ; &rot
str  r0, [sp, #60]    ; rot.y = pi
mov.w sl, #0
mov  r0, r5           ; g
str.w sl, [sp, #56]   ; rot.x = 0
str.w sl, [sp, #64]   ; rot.z = 0
blx  setRotation
```

This is exactly `Vector rot(0, pi, 0); g->setRotation(rot);` — no address
computation.  clang 7.0.2 only produces it if the allocator picks `sl` as
the zero register and reserves `r4` for `hangarPart`; that choice is driven
by the register pressure of the *whole function*, and the only source knob
found that reproduces it is the fake rnd pointer in the ship loop (live
across the loop's calls).  The fake pointer then occupies `r5` in the ship
loop and cascades:

| value | original | shipped |
|---|---|---|
| `&Globals::rnd` | `r8` | spilled `[sp,#44]` |
| `Globals::rnd` value | reload `[r8]` | cached `r5` |
| `capacity` | `r6` | `fp` |
| `race` / `tries` | `r5` | `r6` |
| `type` | `fp` | `r8` |
| `i` | `r9` | `r9` |
| `ship` | `sl` | `sl` |
| `spotUsed` | `r4` | `r4` |

The shipped build also spills `&Globals::rnd` to `[sp,#44]`, which shifts
`spots`/`globals`/`ownHangar`/`bitmask` stack slots down by one word
(original `spots` = `[sp,#44]`, shipped `spots` = `[sp,#24]`).

## 3. The all-global ship loop is not reachable with the parts loop

The all-global source (Ghidra-faithful, no local rnd) compiles the ship loop
with a different 5-cycle permutation (`&rnd → r9`, `capacity → r8`,
`i → sl`, `ship → r5`, `race → r6`) because the prologue keeps
`__stack_chk_guard` in `r5` instead of spilling it to `[sp,#36]`.  Adding
`int n = shipCount;` and using `n` as the loop bound changes the ship-loop
allocation to `&rnd → r8`, `i → r9`, `spotUsed → r4` (three of the original's
registers) but still keeps the guard in `r5` and `capacity` in `sl`.

Every attempt to force the guard spill without the local rnd (top-of-function
local, `Status *status` alias, `Level *self` alias, `Array *enemies` alias,
`int n`, `int c`, guard-use after the loop, `volatile`, function-local
`static`) either is optimised away or changes the early allocation for the
worse.  The local rnd pointer is the only mechanism found.

## 4. Bottom line

`Level::createScene` remains **90.7243 % / 2252 B** (783/791, first diff at
instruction 462).  The 0–461 prefix and the parts loop are exact; the
remaining ~60-instruction diff is the ship-loop register permutation caused
by the fake rnd pointer required to reproduce the prologue and parts-loop
allocation.  Gate green before and after: **byte_exact 4501/4524,
linked_exact 4501/4524, stub_zero_size 0, missing 0, wrong_type 0, extra 0,
data_exact 843/843, imports 207, lint CLEAN, drift 0, RATCHET PASS.**
Structural state unchanged: 27 section headers, file size 2,187,328 B,
`.text` `0x1337c8`, `.rodata` `0x41c27`, `.ARM.exidx` `0x6f70`,
`.ARM.extab` `0x26298`.
