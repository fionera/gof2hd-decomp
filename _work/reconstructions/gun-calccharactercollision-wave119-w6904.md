# `Gun::calcCharacterCollision` wave 119 (w6904): the pp/delta register is
# tied to the MachineLICM hoisting order of the two loop-invariant stack
# addresses, not to the vreg number; no source shape reaches all four shipped
# properties

Worker: w6904. Branch: `wave119-w6904`, base master `be3034dc1`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`,
650 instructions. Scratch: `/dev/shm/w6904/` (not tracked).

## 0. Result

| build | gate byte/linked | target pct | size |
|---|---:|---:|---:|
| before (HEAD `50d2df436`) | **4505 / 4505** | **93.4000** | 1984 |
| after this pass | **4505 / 4505** | **93.4000** | 1984 |

No code change: analysis only. `check` after: `RATCHET PASS`, byte 4505/4524,
linked 4505/4524, size-exact 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0. Structural state unchanged: 27 ELF sections, file size 2,187,328 B.

Section sizes measured this wave (ours vs original): `.text` 1259464 ==,
`.ARM.exidx` 28528 ==, `.data` 10604 ==, `.bss` 27052 ==, file size 2187328 ==;
`.ARM.extab` **156304 vs 156312 (8 B short)**, `.rodata` 269351 vs 269159
(known `Status::missionCompleted` table input). The `Generator::createMission`
LSDA is not touched by this pass.

## 1. The shipped map, measured

| property | shipped | HEAD |
|---|---|---|
| `this` | r9 | r9 |
| `enemy` | r8 | r8 |
| `td` (`&this->targetDir`) | r6 | r6 |
| `i` | fp | fp |
| `i*3` | **r4** | **r5** |
| `playerPosition` (`pp`) | **r4 / sp+44** | r4 / sp+104 |
| `delta` (`enemyPosition - pp`) | **r5 / sp+104** | r5 / sp+44 |
| k-loop layout | **rotated** `[body,then,latch,header]` | not rotated |
| k-loop temp `&(enemy->getPosition())` | recomputed `0x153444` | hoisted `0x153260` |

Raw objdump anchors: `add.w rX, fp, fp, lsl #1` (`i*3`), the first two
`add rX, sp, #44|104` in `0x1530c0..0x153160` (`pp`, then `delta`), and
`mov r6, rX` at `0x1530cc` (the `i*3` save across the scatter block).

## 2. Root cause: MachineLICM hoists both stack addresses, and the *hoisting
## order* picks the registers

Both `&playerPosition` and `&delta` are loop-invariant (`sp+const`) and are
hoisted out of the i-loop by **Early Machine Loop Invariant Code Motion**
(MIR dump "IR Dump After Early Machine Loop Invariant Code Motion"). The
hoisted `t2ADDri %stack.N` vregs are renamed to `%308` / `%282`, and the
greedy allocator deterministically gives

```
%308 -> r4        %282 -> r5
```

In HEAD the prologue of the LICM dump contains, in this order:

```
%308 = t2ADDri %stack.10     ; pp  (sp+104)
...
%282 = t2ADDri %stack.11     ; delta (sp+44)
```

and in the `pp_anon` variant (expression form, `pp = %stack.11`):

```
%308 = t2ADDri %stack.10     ; delta (sp+104)
...
%282 = t2ADDri %stack.11     ; pp  (sp+44)
```

So the hoisting order is always `stack.10` first, `stack.11` second, and the
register follows the *stack slot*: `stack.10 -> r4`, `stack.11 -> r5`. That is
why HEAD gets `pp=r4/sp+104`, `delta=r5/sp+44` and `pp_anon` gets the exact
transpose `pp=r5/sp+44`, `delta=r4/sp+104`.

The frame-slot assignment is `StackColoring` greedy merge, which is also
deterministic (stable sort by size, then n^2 disjoint scan): the first 12-byte
slot (`fi#4`, `pp` in the statement form) merges into the first 60-byte slot
(`fi#10`, the `view` memcpy dest at `sp+104`), the second (`fi#5`, `delta`)
into `fi#11` (the `MatrixSetTranslation` sret at `sp+44`). Hence

```
HEAD     : pp=fi#4->stack.10=sp+104   delta=fi#5->stack.11=sp+44
pp_anon  : pp=fi#5->stack.11=sp+44    delta=fi#4->stack.10=sp+104
```

The shipped binary needs `pp=sp+44` (i.e. `pp=stack.11`) **and** `pp=r4`
(i.e. `stack.10 -> r4`). The two rules are mutually exclusive for every
alloca order that keeps the statement/expression forms above.

## 3. New measurements this wave (official verifier path)

All numbers are `pct / size`; `pp`/`delta` are `(reg, sp offset)` from the
first two `add rX, sp, #44|104` in the scatter block.

### 3.1 Matrix / view forms (the 60-byte frame objects)

| variant | pct | size | pp | delta |
|---|---:|---:|---|---|
| HEAD `Matrix view = *CameraGetLocal(); MatrixSetTranslation(view,...)` | **93.4000** | 1984 | r4/104 | r5/44 |
| `Matrix view(*(Matrix*)...)` copy ctor | 93.4000 | 1984 | r4/104 | r5/44 |
| `const Matrix view = *...` | build error | — | — | — |
| `(void)view;` after decl / after call | 93.4000 | 1984 | r4/104 | r5/44 |
| `Matrix *vp = &view; MatrixSetTranslation(*vp,...)` | 93.4000 | 1984 | r4/104 | r5/44 |
| `Matrix view = MatrixSetTranslation(*CameraGetLocal(),...)` (V5) | **81.4000** | **1972** | **r4/44** | **r5/104** |
| `Matrix view = Matrix(*CameraGetLocal())` | 93.4000 | 1984 | r4/104 | r5/44 |
| nested scope around the view block | 93.4000 | 1984 | r4/104 | r5/44 |
| `Matrix result = MatrixSetTranslation(view,...)` (result unused) | 93.4000 | 1984 | r4/104 | r5/44 |
| `view = MatrixSetTranslation(view,...)` | 87.0000 | 1992 | r4/104 | r5/44 |
| `Matrix view; view = *CameraGetLocal()` | 67.9000 | 2056 | — | — |

**V5 is the key new datapoint**: `Matrix view = MatrixSetTranslation(
*CameraGetLocal(), x, y, z)` creates the sret alloca *before* the view alloca,
so the sret becomes `fi#10` and the view `fi#11`; the physical layout still
puts the sret at `sp+44` and the view at `sp+104`, and the merge then produces
the exact shipped `pp=r4/sp+44`, `delta=r5/sp+104`. But V5 also elides the
60-byte `__aeabi_memcpy` (the view is the sret), so the build is 1972 B and
scores 81.4 %. Adding the copy back (`Matrix view = Matrix(...)`, `(void)view`,
nested scopes, `Matrix result = ...`) restores HEAD's `fi#10=view` order and
the transpose.

### 3.2 `pp`/`delta` forms (HEAD body, cold `distance`)

| variant | pct | size | pp | delta | i3 |
|---|---:|---:|---|---|---|
| HEAD statement form | **93.4000** | 1984 | r4/104 | r5/44 | r5 |
| `pp_anon` expression form | 92.6000 | 1984 | r5/44 | r4/104 | r5 |
| `Vector delta = enemyPosition - getPosition(); VectorLength(delta)` | 92.6000 | 1984 | r5/44 | r4/104 | r5 |
| `Vector delta = enemyPosition - playerPosition; VectorLength(delta)` (both named) | 93.4000 | 1984 | r4/104 | r5/44 | r5 |
| `VectorLength(playerPosition - enemyPosition)` | 93.1000 | 1984 | r4/104 | r5/44 | r5 |
| `float distance = ...; int idistance = (int) distance` | 56.5000 | 1964 | r5/104 | r4/44 | — |
| `pp_anon + mine-radius cold` (`expect(mine,0)` on `radius*=5`) | **89.9000** | 1984 | **r4/44** | **r5/104** | **r6** |
| `pp_anon + mine-radius hot` (`expect(mine,1)`) | 73.4000 | 1996 | moved | moved | **r4** |
| `pp_anon + mine-radius cold + empDamage>0 cold` | 89.9000 | 1984 | r4/44 | r5/104 | r6 |

`pp_anon + mine-radius cold` is the only shape found that gets **three** of
the four shipped properties simultaneously (`this=r9`, `td=r6`, `enemy=r8`,
`pp=r4/sp+44`, `delta=r5/sp+104`) — only `i*3` is wrong (`r6` instead of
`r4`). It scores 89.9 % because the cold `mine` radius branch also moves the
`mine`/`scatter` block layout.

### 3.3 `__builtin_expect` sweeps on top of `pp_anon + mine-radius cold`

11 sites x {0,1}: `isActive` 82.7/54.9, `hitpoints` 83.1/54.9, `egoFlag`
57.7/49.6, `empDamage != kNoAttribute` 84.9/89.5, `empDamage > 0`
89.9/84.2, `owner` 84.8/87.6, `playerGun` 89.9/89.9, `impact` 84.5/89.9,
`scatter` **72.3 (i3=r4, layout moved, 2000 B)** / 62.1, `isAsteroid`
56.0/42.5, `geometries` 57.4/72.2. None beats 93.4.

### 3.4 Statement/loop/misc sweeps (all codegen-identical to HEAD, 93.4000)

* `int`/`unsigned long` for `e`, `i`, `k` and all combinations (10 builds).
* `const Vector *pos = this->positions.data(); pos[i]` for positions and/or
  velocities (P1 93.1, P2/P3 93.4).
* extra dead `Vector` locals in the i-loop (`Vector extra = position;`,
  `extra.x = 0`, `= enemyPosition`, `= velocity`) — all merged, 93.4.
* `this->lifetimes[i] = -1000 * 1000` / `-1000000.0f`, `(int) enemy->getKIPlayer()`,
  `this->playerGun != 0`, `hitFlags[i] = true`, extra parens around
  `VectorLength` — all 93.4 except `enemy->field_54 = 1` (93.2) and
  `0.0f - velocity.x` (86.3, +20 B).
* k-loop rewrites `if (distance >= magnitude) continue;`,
  `if (!(distance < magnitude)) continue;`, `bool inRange = ...` — all
  64.5 / 1908 B (same as plain `if`, because the branch weight is what
  matters, not the spelling).

### 3.5 Expect-combination sweeps

* `{pp form} x {distance plain/0/1} x {mine-filter plain/0/1} x {egoFlag
  plain/0/1}` (54 builds): best 93.4 (HEAD); 92.6 (`pp_anon`); everything
  else <= 89.9.
* `{pp form} x {distance} x {mine-radius plain/0/1} x {empDamage2
  plain/0/1}` (54 builds): best 93.4; `PA D0 R0` 89.9.
* `{distance} x {mine-filter} x {empDamage2}` (27 builds): best 93.4
  (`D0 M1 Ep`); `D0 Mp Ep` 85.1; `Dp *` 48.3-64.5.

## 4. Why the k-loop rotation is coupled to the RA map

MBP runs *after* RA (MIR order: Greedy RA -> ... -> Branch Probability Basic
Block Placement), but the branch weights are also read by the pre-RA
**MachineLICM**. With `__builtin_expect(distance < magnitude, 0)` the if-body
edge is 0.05 %, `selectBestSuccessor(bb.90)` picks the latch (99.95 %), the
chain is `[header, body, latch]` and `findBestLoopExit` returns null (the
header is not at the end of its chain), so `rotateLoop` never fires: HEAD's
layout. With a plain `if` (50/50) the if-body is the first successor and wins
the tie, the chain is `[header, body, then, latch]` and `rotateLoop` produces
the shipped `[body, then, latch, header]` — but the pre-RA LICM no longer
hoists the k-loop temp address, the `%306` spill / `%455` local child that
evicts `this` from r4 disappears, and the whole map shifts
(`this=r4, enemy=r6, td=r9`, 1908 B, 64.5 %).

## 5. Conclusion

The shipped function needs four coupled properties: (1) `this=r9, enemy=r8,
td=r6`, (2) `i*3=r4`, (3) `pp=r4/sp+44, delta=r5/sp+104`, (4) the k-loop
rotated. (1)+(2) need the cold `distance` edge (the `%455` spill); (4) needs
a hot `distance` edge. (3) needs `pp=stack.11` (so `pp=fi#5`, the expression
form) **and** `stack.10->r4` (the LICM hoisting order), which no source shape
reached: the expression form transposes the registers, the statement form
transposes the slots, and the only form that fixes the slots (V5) loses the
60-byte memcpy. `pp_anon + mine-radius cold` reaches (1)+(3) but not (2).

Best measured variant remains the HEAD body at **93.4000 % / 1984 B**.
~200 variants were measured this wave on top of ~1300 from waves 106-119.

## 6. Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6        # RATCHET PASS
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Gun22calcCharacterCollisionEv
```

MIR evidence (hoisting order / stack slots):

```
<clang++> -mllvm -print-after-all \
  -mllvm -filter-print-funcs=_ZN3Gun22calcCharacterCollisionEv -c src/game/weapons/Gun.cpp
# "IR Dump After Early Machine Loop Invariant Code Motion"
#   %308 = t2ADDri %stack.10 ; %282 = t2ADDri %stack.11   (both forms)
# "IR Dump After Merge disjoint stack slots"
#   fi#4 -> %stack.10, fi#5 -> %stack.11
# "IR Dump After Virtual Register Rewriter"
#   %308 -> $r4, %282 -> $r5
```

Register probe: `arm-linux-androideabi-objdump -d cmake-build-match/libgof2hdaa.so`
then read `mov r9, r0` (`this`), `add.w rX, fp, fp, lsl #1` (`i*3`),
`ldr.w rY, [r0, r2, lsl #2]` (`enemy`), `str rZ, [sp, #24]` (`td`), and the
first two `add rX, sp, #44|104` in `0x1530c0..0x153160` (`pp`, `delta`).
