# `Gun::calcCharacterCollision` wave 119 (w6940): the `i*3` register is the
# highest-value residual; it is pinned to `r5` by the MachineLICM-hoisted
# scatter-block stack address, and no source shape found separates them

Worker: w6940. Branch: `wave119-w6940`, base master `be3034dc1`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`,
650 instructions. Scratch: `/dev/shm/w6940/` (not tracked).

## 0. Result

| build | gate byte/linked | target pct | matched insns | size |
|---|---:|---:|---:|---:|
| before (HEAD) | **4505 / 4505** | **93.4000** | **607 / 650** | 1984 |
| after this pass | **4505 / 4505** | **93.4000** | **607 / 650** | 1984 |

No code change: analysis only. `check` after: `RATCHET PASS`, byte 4505/4524,
linked 4505/4524, size-exact 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0. Structural state unchanged: 27 ELF sections, file size 2,187,328 B.
No section/extab change.

## 1. Where the 43 unmatched instructions live

An LCS diff of the two 650-instruction streams gives exactly 43 unmatched on
each side. They fall into five groups:

| group | unmatched (each side) | fixable by |
|---|---:|---|
| `i*3` computed into `r5` (shipped: `r4`) | 9 | RA/priority, see §3 |
| `pp`/`delta` stack slots transposed | 2 | StackColoring order, see §2 |
| 2 extra `add r4, sp, #104` in the i-loop body (shipped: `ldr.w fp,[sp,#40]`) | 2 + 14 branch-target shifts | RA (same cause as `i*3`) |
| view block rematerialises `sp+104` 3x (shipped: 1 `add r5` + 3 `mov`s) | 4 | RA |
| k-loop not rotated (`[header, body, latch]` vs `[body, then, latch, header]`) | 12 | MBP, see §4 |

Simulating a fix directly on the disassembly (register/slot substitution, then
the official `reconcile`+`score`) quantifies each:

| simulated fix | pct |
|---|---:|
| none (HEAD) | 93.4 |
| `i*3` register `r5 -> r4` only | **94.8** |
| `pp`/`delta` slots swapped only | 93.7 |
| both | **95.1** |

So the `i*3` register is the single highest-value residual (+1.4 points), and
it is also the one that removes the two extra body instructions (they are the
same root cause: the RA keeping the scatter block's `sp+104` address in `r4`).

## 2. The frame layout is fixed; only the merge order can move `pp`

`PEI::calculateFrameObjectOffsets` builds `ObjectsToAllocate` in **frame-index
order** (ARM does not override `orderFrameObjects`) and `AdjustStackOffset`
places each object *below* the previous one (`Offset += Size; Offset =
alignTo(Offset, Align); setObjectOffset(i, -Offset)`). So for the two 60-byte
slots:

```
fi#10 (60 B, align 8)  -> SP-208 -> sp+104   (the `Matrix view` memcpy dest)
fi#11 (60 B, align 4)  -> SP-268 -> sp+44    (the MatrixSetTranslation sret)
```

`fi#10` is therefore *always* above `fi#11`. `StackColoring` then merges the
12-byte allocas in size-descending, index-ascending order: `fi#4` merges into
`fi#10` (sp+104) and `fi#5` into `fi#11` (sp+44).

Consequences (all confirmed against MIR):

* statement form (`Vector playerPosition = ...; VectorLength(enemyPosition -
  playerPosition)`) -> `pp = fi#4 = stack.10 = sp+104`, `delta = fi#5 =
  stack.11 = sp+44`. **Slots transposed** vs the shipped binary.
* expression form (`VectorLength(enemyPosition - getPlayer()->getPosition())`)
  -> the `operator-` sret alloca is created before the `getPosition()` temp, so
  `delta = fi#4 = stack.10 = sp+104`, `pp = fi#5 = stack.11 = sp+44`.
  **Slots match the shipped binary.**

The shipped binary has `pp` at sp+44 and `delta` at sp+104, i.e. the expression
form's slot assignment. To get `pp` at sp+44 *and* `pp = r4` one would need
`fi#10` to be the sret alloca (so `pp = stack.10 -> r4` while still merging into
the sp+44 slot), which requires the `MatrixSetTranslation` sret temp to be
created *before* the `view` alloca. `Matrix view = MatrixSetTranslation(
*CameraGetLocal(), x, y, z)` does exactly that but elides the 60-byte
`__aeabi_memcpy`, giving 1972 B / 81.4 % (recorded in the w6904 note as V5).

## 3. Root cause of the `i*3` register: MachineLICM hoists the scatter-block
## stack address and greedy RA colours it `r4`

`t2ADDri %stack.N` is trivially rematerialisable, so `MachineLICM` always hoists
it (`IsProfitableToHoist`: "Rematerializable instructions should always be
hoisted"). In the *Early Machine LICM* dump the preheader receives, in this
order:

```
%108 = t2ADDri %stack.3    (enemyPosition)
%152 = t2ADDri %stack.1    (position)
%291 = t2ADDri %stack.2    (velocity)
%308 = t2ADDri %stack.10   (view / delta / k-loop getPosition)
%282 = t2ADDri %stack.11   (MatrixSetTranslation sret / pp)
```

Greedy RA colours the first hoisted stack address with `r4` and the second with
`r5`, and `i*3` (`add.w rX, fp, fp, lsl #1`) then lands in `r5`. That is HEAD:

```
i3 = r5, pp = r4 / sp+104, delta = r5 / sp+44
```

The shipped binary instead has `i3 = r4`, `pp = r4 / sp+44`, `delta = r5 /
sp+104`, with `mov r6, r4` / `mov r4, r6` saving/restoring `i*3` around the
scatter block (0x1530cc / 0x153153). I.e. in the original the `sp+104` address
is rematerialised into `r5` in the scatter/view blocks and into `r4` in the
k-loop, and `i*3` keeps `r4`.

Decisive experiment: deleting the scatter block's `playerPosition`/`delta`
computation (`int distance = 0;`) makes `i*3` become `r4` (75.4 %, 1880 B),
while deleting the whole `Matrix view` block leaves `i*3 = r5` (55.5 %, 1896 B).
So the scatter block's hoisted `stack.10` address is what evicts `i*3`.

## 4. Why the k-loop rotation is coupled to the RA map

MBP runs after RA, but branch weights are also read by the pre-RA MachineLICM.
With `__builtin_expect(distance < magnitude, 0)` the if-body edge is 0.05 %,
`selectBestSuccessor` picks the latch, the chain is `[header, body, latch]`,
`findBestLoopExit` returns null (header not at the end of its chain) and
`rotateLoop` never fires: HEAD's layout. With a plain `if` the chain is
`[header, body, then, latch]` and `rotateLoop` produces the shipped
`[body, then, latch, header]`, but the whole RA map shifts
(`this=r4, enemy=r6, td=r9`, 1908 B, 64.5 %).

`__builtin_expect_with_probability` is not available in clang 7.0.2.

## 5. Measurements this wave (official verifier path)

Format: `pct / size / i3 / pp / delta`. All builds are 650 instructions unless
noted.

### 5.1 Scatter-block forms (HEAD body, cold `distance`)

| variant | pct | size | i3 | pp | delta |
|---|---:|---:|---|---|---|
| HEAD statement form | **93.4** | 1984 | r5 | r4/104 | r5/44 |
| expression form | 92.6 | 1984 | r5 | r5/44 | r4/104 |
| `Vector delta = enemyPosition - getPlayer()->getPosition();` | 92.6 | 1984 | r5 | r5/44 | r4/104 |
| both named (`pp` then `delta`) | 93.4 | 1984 | r5 | r4/104 | r5/44 |
| `PlayerEgo *player = ...getPlayer();` then `player->getPosition()` | 93.4 | 1984 | r5 | r4/104 | r5/44 |
| `Vector playerPosition(...)` direct-init | 93.4 | 1984 | r5 | r4/104 | r5/44 |
| `Vector playerPosition = Vector(...)` copy-ctor | 93.4 | 1984 | r5 | r4/104 | r5/44 |
| `{}`-init / `const` / `const&` alias / `&` alias / nested scope | 93.4 | 1984 | r5 | r4/104 | r5/44 |
| `Vector playerPosition; playerPosition = ...` | 82.8 | 2000 | r5 | r5/44 | r4/104 |
| `*td = enemyPosition - playerPosition; VectorLength(*td)` | 83.3 | 1992 | r5 | r4/104 | r5/44 |
| `Vector delta; delta = enemyPosition - playerPosition;` (loop-scope) | 66.4 | 1964 | ? | r4/104 | r5/44 |
| `Vector playerPosition;` declared in loop body, assigned in branch | 67.3 | 1964 | **r4** | r4/104 | r4/104 |
| `int distance = 0;` (no pp/delta at all) | 75.4 | 1880 | **r4** | r4/104 | r4/104 |

### 5.2 `mine` radius branch x scatter form x k-loop

| variant | pct | size | i3 | pp | delta |
|---|---:|---:|---|---|---|
| HEAD (stmt, plain `if (mine)`, `expect(distance<magnitude,0)`) | **93.4** | 1984 | r5 | r4/104 | r5/44 |
| stmt + `expect(mine,0)` + k0 | 88.8 | 1984 | r6 | r5/104 | r4/44 |
| stmt + `expect(mine,0)` + plain k-if | 82.6 | 1980 | **r4** | r5/104 | r4/44 |
| expr + `expect(mine,0)` + k0 | 89.9 | 1984 | r6 | **r4/44** | **r5/104** |
| expr + `expect(mine,0)` + plain k-if | 83.7 | 1980 | **r4** | **r4/44** | **r5/104** |
| expr + `expect(mine,1)` + k0 | 73.4 | 1996 | r4 | r4/104 | r4/104 |
| stmt + `expect(mine,1)` + k0 | 73.4 | 1996 | r4 | r4/104 | r4/104 |
| stmt + plain k-if (no expect) | 64.5 | 1908 | r0 | r5/104 | r0/104 |
| expr + plain k-if | 65.0 | 1908 | r0 | r5/104 | r0/104 |
| outer `if (mine)` (enemyFlagsLo) `expect(...,1)` | 89.4 | 1984 | r5 | r4/104 | r5/44 |
| outer `if (mine)` `expect(...,0)` | 75.5 | 1984 | r5 | r4/104 | r5/44 |

`expr + expect(mine,0) + plain k-if` is the only shape found that gets three of
the four shipped properties (`i3 = r4`, `pp = r4/sp+44`, `delta = r5/sp+104`);
it scores 83.7 % because the cold `mine` edge also moves the `mine`/`scatter`
block layout (`enemy` becomes `r6` instead of `r8`).

### 5.3 k-loop forms (HEAD body)

| variant | pct | size |
|---|---:|---:|
| `for (unsigned k = 0; k < size(); ++k)` + `expect(...,0)` (HEAD) | **93.4** | 1984 |
| `for` + `expect(...,1)` | 64.5 | 1908 |
| `for` + plain `if` | 64.5 | 1908 |
| `unsigned k = 0; while (k < size()) { ...; ++k; }` | 93.4 | 1984 |
| `do { ...; ++k; } while (k < size())` | 83.9 | 1988 |
| `Vector ep = enemy->getPosition(); *td = ep; *td -= position;` | 93.4 | 1984 |
| `Vector &ep = *td; ep = ...; ep -= position;` | 93.4 | 1984 |
| `*td = enemy->getPosition() - position;` | 61.2 | 1956 |

### 5.4 `__builtin_expect` sweep on top of a plain k-loop

12 sites x {0,1}: best 64.5 (`enemyFlagsLo` 0, `playerGun` 1, `impact` 1,
`emp` 1). None beats 93.4.

### 5.5 View-block forms

| variant | pct | size | i3 | pp | delta |
|---|---:|---:|---|---|---|
| HEAD | **93.4** | 1984 | r5 | r4/104 | r5/44 |
| `float *local = CameraGetLocal(...); Matrix view = *(Matrix*)local;` | 93.4 | 1984 | r5 | r4/104 | r5/44 |
| nested scope around the view block | 93.4 | 1984 | r5 | r4/104 | r5/44 |
| `Matrix res = MatrixSetTranslation(view,...); TransformSetLocal(...,res)` | 83.7 | 1988 | r6 | r4/104 | r5/44 |
| `TransformSetLocal(..., MatrixSetTranslation(view,...))` | 61.5 | 1976 | r6 | ? | ? |
| `Matrix view` hoisted to the top of the i-loop body | 48.8 | 1904 | ? | r6/104 | r5/44 |

### 5.6 Misc

* `while`/`for`/`for_noinc`/`k != size`/`data()[k]`/`(unsigned) size()`: 93.4.
* `int`/`unsigned`/`unsigned long` loop indices: 93.4.
* `radius` as `float`: 41.8. `radius` via if/else instead of `?:`: 93.4.
* `int magnitude = this->magnitude;` in the k-loop: 93.4.
* `int distance;` declared before `playerPosition`: 93.4.
* extra dead `Vector` local in the i-loop body: 93.4 (frame unchanged).
* `Vector dummy` live across the view block: 76.5 / 2024.

### 5.7 `expect` on top of `expr + expect(mine,0) + plain k-if` (83.7%)

12 sites x {0,1}: best 83.7 (`playerGun` 1, `mine2` 0, `impact` 1,
`enemyFlagsLo` 0 -- all codegen-identical to the 83.7 % base). Every one keeps
`i3 = r4, pp = r4/sp+44, delta = r5/sp+104` but none recovers the `mine`-branch
layout or the k-loop rotation.

~130 variants were measured this wave on top of ~1500 from waves 106-119.

## 6. Conclusion

The shipped function needs four coupled properties:

1. `this=r9, enemy=r8, td=r6` (HEAD already has these),
2. `i*3 = r4`,
3. `pp = r4 / sp+44`, `delta = r5 / sp+104`,
4. the k-loop rotated.

(2) needs the scatter block's hoisted `stack.10` address to *not* take `r4`.
(3) needs `pp = stack.11` (expression form) *and* `stack.11 -> r4`, i.e. the
opposite of the greedy-RA rule that the first-hoisted stack address gets `r4`.
(4) needs a hot `distance` edge, which changes the pre-RA LICM/RA map wholesale.

Every shape tried this wave that reaches one of these loses at least one other.
Best measured variant remains the HEAD body at **93.4000 % / 1984 B**
(607 / 650 instructions matched).

## 7. Reproduction

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

MIR evidence (hoisting order / stack slots / frame offsets):

```
<clang++> -mllvm -print-after-all \
  -mllvm -filter-print-funcs=_ZN3Gun22calcCharacterCollisionEv -c src/game/weapons/Gun.cpp
# "IR Dump After Early Machine Loop Invariant Code Motion"
#   %308 = t2ADDri %stack.10 ; %282 = t2ADDri %stack.11   (both forms)
# "IR Dump After Merge disjoint stack slots"
#   fi#4 -> %stack.10, fi#5 -> %stack.11
# "IR Dump After Prologue/Epilogue Insertion & Frame Finalization"
#   fi#10: size=60, align=8, at location [SP-208]   -> sp+104
#   fi#11: size=60, align=4, at location [SP-268]   -> sp+44
```

Register probe: `arm-linux-androideabi-objdump -d cmake-build-match/libgof2hdaa.so`
then read `mov r9, r0` (`this`), `add.w rX, fp, fp, lsl #1` (`i*3`),
`ldr.w rY, [r0, r2, lsl #2]` (`enemy`), `str rZ, [sp, #24]` (`td`), and the
first two `add rX, sp, #44|104` in `0x1530c0..0x153160` (`pp`, then `delta`).
