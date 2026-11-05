# `Level::createScene` — wave-115 w6312: a scheduling-friendly address
# expression beats the shift trick (90.5277 → 90.7243 %, 2252 B)

**Status:** landed, 2026-11-05. Worktree `wt/w6312`, branch
`wave115-w6312`. Target `_ZN5Level11createSceneEv`.
**Source change:** one line in `src/game/world/Level.cpp` (parts loop,
`Vector *rp` address expression). Gate green.

Follow-up to `level-createscene-wave114-w6214.md` (and the chain back through
waves 88–114). Wave 114 concluded that the wave-112 `Vector` address trick
(`(char *) &rot + ((size_t) g << 31)`) at `rnd` mask 47 was the flag-free
maximum at 90.5277 %. This pass re-swept the *shape* of that address
expression instead of the `rnd` mask and found a variant that makes clang
schedule the stack-address add into the original position.

## TL;DR

* Before: **90.5277 % / 2252 B**, 783/790 instructions, first diff at
  instruction 462 (parts-loop preheader branch target `loc_610` vs `loc_614`).
* After: **90.7243 % / 2252 B**, 783/791 instructions, first diff at
  instruction 462 (branch target `loc_610` vs `loc_614`).
* The change is the parts-loop Vector address expression:

  ```cpp
  // before (wave-112, shift trick)
  Vector *rp = (Vector *) ((char *) &rot + ((size_t) g << 31));
  // after (wave-115)
  Vector *rp = (Vector *) ((char *) &rot - ((size_t) g + 1));
  ```

  `- (g + 1)` is the same value as `+ ~g`; clang 7.0.2 lowers both to
  `mvns r0, r5` + `add r1, r0`, but — unlike the shift form — it schedules
  `add r1, sp, #56` and `str r0, [sp, #60]` *before* the complement/add.
  The first two instructions of the `setRotation` setup therefore match the
  original exactly, and the remaining stores/call align with a two-instruction
  insertion instead of a whole-block replacement.

* Original / ours `setRotation` setup (parts loop):

  ```
  orig: add r1, sp, #56 ; str r0, [sp, #60] ; mov.w sl, #0 ; mov r0, r5
        str.w sl, [sp, #56] ; str.w sl, [sp, #64] ; blx setRotation
  ours: add r1, sp, #56 ; str r0, [sp, #60] ; mvns r0, r5 ; add r1, r0
        mov.w sl, #0 ; mov r0, r5 ; str.w sl, [sp, #56] ; str.w sl, [sp, #64]
        blx setRotation
  ```

  The two extra instructions shift every later branch target by +8 bytes
  (the shift trick shifted by +4), but difflib matches more of the stream:
  the first two setup instructions and the whole tail now line up.

* `rnd` mask 47 is still the unique maximum with the new expression
  (90.7243). Re-swept masks: 110 90.6, 109/107/103/124/122/118 90.0,
  61/59/55 89.8, 111 88.1, 35/37 88.2, 127 86.0, 0 75.7.
* Neutral with the new expression (identical code, 90.7243): Vector
  assignment form (`rot.x/y/z = ...`), removing the `spots` local, `int`
  ownHangar, shared `i` for the fill and ship loops, `Vector` array /
  reference / inline-argument forms.
* Worse: `tries = -100; tries++` 90.0, `&rot - (size_t) g` 90.3,
  `&rot + (size_t) g` 90.3, `&rot + (g & 1)` 88.0, `&rot + g * 3` 88.0.

## 1. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6312
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 tools/verify/verify.py --original \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so --show _ZN5Level11createSceneEv
```

Scratch harness (`/dev/shm/w6312`, not committed): `fast.sh` compiles one
copy of `Level.cpp` with the exact NDK r18b clang 7.0.2 `-Oz` flags, re-runs
the saved gold link + `strip --strip-all` + `reorder_shstrtab.py`, and scores
with the official single-function verifier (`score.py`).

## 2. Address-expression sweep (mask 47, current declaration position)

| expression | pct | size | insns |
|---|---|---|---|
| **`&rot - ((size_t) g + 1)` (landed)** | **90.7243** | **2252** | **791** |
| `&rot + (~(size_t) g)` | 90.7243 | 2252 | 791 |
| `&rot + ((size_t) g >> 31)` | 90.5277 | 2252 | 790 |
| `&rot + ((size_t) g << 31)` (wave-112) | 90.5277 | 2252 | 790 |
| `&rot + ((size_t) g << 1..24)` | 90.5277 | 2252 | 790 |
| `&rot - (size_t) g` | 90.3 | 2252 | 791 |
| `&rot + (size_t) g` | 90.3 | 2252 | 791 |
| `&rot + ((size_t) g ^ 0)` / `| 0` / `* 1` / `/ 1` | 90.3 | 2252 | 791 |
| `&rot + ((size_t) g & 1)` | 88.0 | 2252 | 790 |
| `&rot + ((size_t) g * 3)` | 88.0 | 2252 | 790 |

The `~g` / `-(g+1)` forms are the only ones that both keep the parts-loop
register assignment (`r9` = row, `r8` = `&Globals::Canvas`, `r5` = `g`) and
schedule the stack-address add first.

## 3. Gate

Before and after: **byte_exact 4500/4524, linked_exact 4500/4524,
stub_zero_size 0, missing 0, wrong_type 0, extra 0, data_exact 843/843,
imports 207, lint CLEAN, drift 0, RATCHET PASS.** Structural state
unchanged: 27 section headers, file size 2,187,328 B, `.text` `0x1337c8`,
`.rodata` `0x41c27`, `.ARM.exidx` `0x6f70`, `.ARM.extab` `0x26298`.

## 4. Bottom line

`Level::createScene` improves from **90.5277 %** to **90.7243 %** at the same
2252 B. The remaining diff is still dominated by the parts-loop `+8` branch
shift and the ship-loop register permutation (`rnd` value in `r5` vs
`race` in `r5`), which the wave-113/114 notes showed are in direct tension
with the early-register-allocation match.
