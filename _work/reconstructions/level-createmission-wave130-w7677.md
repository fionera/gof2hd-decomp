# w7677: `Level::createMission` (92.326733 -> 92.333086 %, 17172 B) — wave-130
# faithful `size()` re-read + missing 8-byte tail frame slot

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w7677`, based on current master.
Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; Ghidra address `0xbffd8`, linked `.text` address `0xaffd8`,
17172 B).  Continues `level-createmission-wave121-w7112.md` …
`…-wave128-w7653.md`.

## Baseline (pristine HEAD, re-measured this session)

```text
_ZN5Level13createMissionEv   92.326733 %   size 17172/17172 B
                             6055 orig / 6065 ours insns
gate: byte 4506/4524, linked 4506/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
```

## 1. New hypothesis: the `case4Tail` loop is faithful, and the frame is 8 B short

Wave-127/128 established that the original re-reads `this->enemies->size()` on every
iteration of the case-4 tail loop:

```text
b41c4  add.w r9, r9, #1
b41c8  ldr.w r0, [r8, #248]   ; this->enemies
b41cc  ldr   r0, [r0, #0]     ; ->size()
b41ce  cmp   r9, r0
b41d0  bcc.n b416e
```

The checked-in source instead cached the size in a `volatile unsigned n` latch
(`sp+312`).  The latch is a workaround: the faithful re-read alone is **8 B short**
(17164 vs 17172), because clang shrinks the frame from 400 B to 392 B.

This wave tested the hypothesis that the original has an otherwise-dead 8-byte
stack slot live across the case-4 tail block.  A `volatile double` local in that
block forces exactly that slot:

```cpp
case4Tail:
    {
    // Codegen anchor: the original keeps an otherwise-dead 8-byte stack slot
    // live across this block (frame 400 B).  Without it clang shrinks the
    // frame to 392 B and the function comes out 8 B short.
    volatile double tailFrameSlot = 0.0;
    ...
    for (unsigned i = 0; i < this->enemies->size(); i++) {
```

The double is zeroed with `vmov.i32 d16,#0; vstr d16,[sp,#312]` (2 insns), but
removing the 4-byte latch and restoring the faithful re-read removes 5 insns
elsewhere, so the function ends at **6062** insns (down from 6065) and the
fuzzy ratio rises from **92.326733 %** to **92.333086 %** (one more matching
instruction than baseline).

The original's frame is 400 B and its `case4Else` coords array occupies
`sp+312..sp+320`; the tail block does not use that slot, but the original
keeps the 400 B frame.  The anchor is a reconstruction of that missing
8-byte slot, not a behaviour change.

## 2. Variants measured this session

Each row is a fresh `cmake --build cmake-build-match --target link_so -j6`
followed by the official verifier's normalized stream.  Baseline is
**92.326733 / 17172 / 6065**.

### 2.1 Faithful re-read / frame pad

| variant | % | size | insns |
|---|---:|---:|---:|
| **baseline `volatile unsigned n` latch** | **92.326733** | 17172 | 6065 |
| faithful `this->enemies->size()` re-read only | 91.341312 | 17164 | 6060 |
| faithful + `volatile double pad = 0.0` | **92.333086** | 17172 | 6062 |
| faithful + `volatile unsigned long long pad = 0` | 92.317848 | 17172 | 6064 |
| faithful + `volatile int pad = 0` | 91.212146 | 17172 | 6064 |
| faithful + `volatile int pad[2] = {0,0}` | 92.333086 | 17172 | 6062 |
| faithful + `volatile float pad[2] = {0,0}` | 92.333086 | 17172 | 6062 |
| faithful + `volatile double pad = 1.0` | 92.333086 | 17172 | 6062 |
| faithful + `volatile double pad = *(double *)&this->stationPosition` | 92.333086 | 17172 | 6062 |
| faithful + `volatile double pad` (uninitialised) | 91.341312 | 17164 | 6060 |
| faithful + `double pad = 0.0` (non-volatile) | 91.341312 | 17164 | 6060 |
| faithful + `volatile int pad[3] = {0,0,0}` | 91.296098 | 17180 | 6066 |
| pad at top of `createMission` | 83.761248 | 17180 | 6058 |
| pad at start of `isEmpty` block | 79.319739 | 17180 | 6058 |
| pad in `case4Else` / before `case4Tail` / after `hunterCount` | 92.333086 | 17172 | 6062 |

### 2.2 Other source hypotheses re-tested with the pad

| variant | % | size | insns |
|---|---:|---:|---:|
| `bool raid` -> `int raid` | 92.168028 | 17172 | 6062 |
| `bool raid` moved to function top | 91.491293 | 17172 | 6062 |
| `fortressGuns` declared early | 92.333086 | 17172 | 6062 |
| `extraAliens` declared early | 92.333086 | 17172 | 6062 |
| both declared early | 92.333086 | 17172 | 6062 |
| `bool hostile = true; bool group = false;` in tail loop | 92.333086 | 17172 | 6062 |
| `int hostile = 1; int group = 0;` in tail loop | 92.333086 | 17172 | 6062 |
| `coords` moved to outer scope (no pad) | 87.742468 | 17164 | 6060 |
| `coords` moved to outer scope + pad | 88.734835 | 17172 | 6062 |
| `volatile int coords[3]` | 91.341312 | 17164 | 6060 |
| `double hunters` | 91.170160 | 17172 | 6063 |
| `double threshold = 0.7` for the two `> 0.7` comparisons | 91.341312 | 17164 | 6060 |

The pad's exact type/value/placement inside the `case 4` block is neutral as long
as it is a live 8-byte volatile slot; the score is set by the faithful re-read.

## 3. Residual (unchanged)

The remaining ~7.7 pp is still backend:

1. alien-turret table select: original `cbz` diamond vs our if-converted
   `cmp.w r1, r9, asr #31; it ne; movne` (whole cyclic register permutation);
2. `case4Tail`: original `fp = 1` (hostile) and `&Globals::rnd` reloaded from
   `sp+120`; ours keeps `&Globals::rnd` in `fp` and rematerialises `1`;
3. `raid`/`Globals::globals` spill-slot swap (`sp+84`/`sp+88`);
4. `stationPirates`/`extraAliens`/`fortressGuns` slot permutation
   (`sp+48/44/40` vs `sp+44/40/48`);
5. case-183 test/body order; branchless `&&` sites; pirate-fortress
   `ldmia`-vs-`ldrd` SLP pair.

## 4. Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w7677/score.py
cmake --build cmake-build-match --target check -j6
```

Ghidra headless (project copy, live project locked):

```text
cp -r ../_work/bins/android_2.0.16_libgof2hdaa.so_ghidra.{gpr,rep} /dev/shm/w7677/gh/
/tmp/ghidra_12.1.2_PUBLIC/support/analyzeHeadless /dev/shm/w7677/gh \
  android_2.0.16_libgof2hdaa.so_ghidra -process android_2.0.16_libgof2hdaa.so \
  -scriptPath /dev/shm/w7677 -postScript Decomp.java 0xbffd8 -noanalysis
```

Scratch under `/dev/shm/w7677` (`Level.base.cpp` = pristine HEAD copy;
`score.py`, `batch.py`, `v1.py` … `v19.py` = variant specs;
`orig.asm`, `ours.asm`, `ours_dpad.asm` = disassembly dumps;
`gh_decomp.c` = full Ghidra decompile).

## 5. Gate counts (before -> after)

```text
byte_exact      4506 -> 4506   (4506/4524)
linked_exact    4506 -> 4506   (4506/4524)
size_exact      4524 -> 4524
stub_zero_size     0 -> 0
missing            0 -> 0
wrong_type         0 -> 0
extra              0 -> 0
data_exact       843 -> 843
imports          207 -> 207
lint            CLEAN   -> CLEAN
drift           0       -> 0
RATCHET         PASS    -> PASS
```
