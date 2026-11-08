# LevelScript::process — wave 160 / w10605: `Globals::status->field_90`
# loop-bound reload landed (raw -8 / aligned -9, size-exact)

Target `_ZN11LevelScript7processEi` (`LevelScript::process`), original
`0x137d18`, **57,232 B (0xdf90)**, size-exact.  Owned TU
`src/game/world/LevelScript.cpp`.  Worktree `wt/w10605`, base master
`08af4633b` (wave 159).  Scratch `/dev/shm/w10605/`.

## Result (landed)

```
                         before (HEAD)     after (landed)
process raw diffbytes    55,055            55,047            (-8)
process aligned insns    17,059            17,050            (-9)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
.text diffbytes          85,722            85,714            (-8)
.ARM.extab diffbytes     1,256             1,256             (neutral)
.ARM.extab size          156,312           156,312           (exact)
.ARM.exidx               0                 0                 **BYTE-IDENTICAL**
.rel.plt                 0                 0                 **BYTE-IDENTICAL**
.rodata / .data / .bss   exact             exact
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN             CLEAN
drift                    0                 0
RATCHET                  PASS              PASS
```

## The change (1 line)

Mission-fail tail (`m_nState <= 1`, source ~line 5284).  The loop condition
used a cached local:

```cpp
Array<int> *stationList = Globals::status->field_90;
for (unsigned int i = 0; i < stationList->count; ++i) {
    if ((*stationList)[i] == Globals::status->getStation()->getIndex()) {
        (*stationList)[i] = -1;
    }
}
```

The original re-reads `Globals::status->field_90` on **every** loop test
(`0x144f14 ldr r0,[r5,#0]` / `0x144f16 ldr.w r1,[r0,#144]` /
`0x144f1a ldr r2,[r1,#0]`), so the bound is written inline:

```cpp
Array<int> *stationList = Globals::status->field_90;
for (unsigned int i = 0; i < Globals::status->field_90->count; ++i) {
    if ((*stationList)[i] == Globals::status->getStation()->getIndex()) {
        (*stationList)[i] = -1;
    }
}
```

After the edit our loop tail is instruction-for-instruction the original's:

```
orig 0x144f14  ldr   r0,[r5,#0]        ours 0x144f8c  ldr  r0,[sp,#92]
orig 0x144f16  ldr.w r1,[r0,#144]      ours 0x144f8e  ldr.w r1,[r0,#144]
orig 0x144f1a  ldr   r2,[r1,#0]        ours 0x144f92  ldr  r1,[r1,#0]
orig 0x144f1c  cmp   r4,r2             ours 0x144f94  cmp  r5,r1
orig 0x144f1e  bcc   0x144ef4          ours 0x144f96  bcc  0x144f70
```

(`r5`/`sl` and the stack slot differ only by the function-wide register
allocation; the reload shape is now identical.)

## Measured variants (all full link)

| variant | raw | aligned | size | note |
|---|---:|---:|---:|---|
| baseline | 55,055 | 17,059 | 57,232 | |
| **C: inline condition, keep body local** | **55,047** | **17,050** | **57,232** | landed |
| A: inline condition + inline body | 55,371 | 17,318* | 57,408 | +176 B, gate-fatal |
| B: local condition + inline body | 55,394 | 17,637* | 57,204 | -28 B, gate-fatal |
| C + `enemies->data_[i]` at case-145 loop | 55,047 | 17,050 | 57,232 | identical (inlined `operator[]`) |
| C + `Array<KIPlayer*>* en = enemies` at case-145 | 55,047 | 17,050 | 57,232 | identical (CSE) |
| C + `en = m_pLevel->getEnemies()` at case-145 | 55,203 | 17,054 | 57,240 | +8 B, gate-fatal |
| C + ternary -> if/else at line 3467 | 55,111 | 17,063 | 57,228 | -4 B, gate-fatal |
| C + drop `pirate` local at line 3946 | 55,238 | 17,071 | 57,240 | +8 B, gate-fatal |
| C + drop `enemy`/`ship` locals at line 4490 | 55,309 | 18,387* | 57,384 | +152 B, gate-fatal |

\* aligned numbers for non-size-exact variants are not comparable (the
function length changes, so the alignment includes the extra/missing
instructions).

## What did NOT move

* `(*arr)[i]` vs `arr->data()[i]` vs `arr->data_[i]` is exactly the same
  AST after inlining `Array::operator[]`; no codegen change anywhere.
* Hoisting `Array<T>*` into a fresh local (`Array<KIPlayer*>* en = enemies;`)
  is CSE'd away; the allocator decision is unchanged.
* Replacing `m_pLevel->getEnemies()` by a hoisted local in the loops that
  call it inline (lines 1603/1714/1732/1802) is already what the source
  does and already matches the original (the original also re-calls
  `getEnemies()` per access).
* The original's `ldr.w r0,[r8]` loops (r8 = `&array->data_`) are a
  register-allocation artefact, not a source shape: rewriting the loop body
  to force reloads changes the whole function's allocation and the size.

## Open questions

1. Variant A reproduces the original's body exactly (`ldr r0,[status]` /
   `ldr.w r0,[r0,#144]` / `ldr r0,[r0,#4]` / `str.w`) but grows the
   function by 176 B because the function-wide register allocation changes
   (`fp`/`sl` swap, many narrow `ldr` become `ldr.w`).  A -176 B companion
   elsewhere would let the exact body land.
2. The `case 145` loops at lines 4312/4322/4433 and the `isDead` count loop
   at line 4869 reload the `enemies` local from the stack once per
   iteration; the original keeps it in a callee-saved register.  No source
   spelling tried so far changes the allocator's choice.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10605/fdiff_local.py _ZN11LevelScript7processEi        # 55,047 / 57,232
python3 /dev/shm/w10605/align_local.py _ZN11LevelScript7processEi        # 17,050
cmake --build cmake-build-match --target check -j6                       # RATCHET PASS
```

Note: the shared `/dev/shm/fdiff.py` and `_work/reconstructions/align_diff.py`
hard-code `gof2-decomp/cmake-build-match`; local copies with the worktree
path were used for every measurement above.
