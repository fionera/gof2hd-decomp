# `Gun::calcCharacterCollision` wave 119 (w6713): stack-slot frame order is
# the missing half of the scatter-block transpose; the RA register choice is
# tied to the frame index, not the vreg

Worker: w6713. Branch: `wave119-w6713`, base master `47a1396bd`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`,
650 instructions. Scratch: `/dev/shm/w6713/` (not tracked).

## 0. Result

| build | gate byte/linked | target pct | matched | size |
|---|---:|---:|---:|---:|
| before (HEAD `47a1396bd`) | **4503 / 4503** | **93.3846** (report 93.4) | 607/650 | 1984 |
| after this pass | **4503 / 4503** | **93.3846** | 607/650 | 1984 |

No code change: analysis only. `check` after: byte 4503/4524, linked 4503/4524,
size-exact 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0,
`RATCHET PASS`. Structural state unchanged: 27 ELF sections, file size
2,187,328 B.

This wave found the *exact* source-level cause of the scatter-block transpose
and proved it is separable from the register allocation, but no single source
shape reaches the shipped map. The remaining delta is now fully characterised:
`(playerPosition, delta)` must be `(r4, sp+44)` and `(r5, sp+104)`; HEAD gives
`(r4, sp+104)` / `(r5, sp+44)`; the expression form gives `(r5, sp+44)` /
`(r4, sp+104)`.

## 1. The shipped map, measured

The shipped function has all of:

| property | shipped | HEAD |
|---|---|---|
| `this` | r9 | r9 |
| `enemy` | r8 | r8 |
| `td` (`&this->targetDir`) | r6 | r6 |
| `i` | fp | fp |
| `i*3` | **r4** | **r5** |
| `playerPosition` | **r4 / sp+44** | r4 / sp+104 |
| `delta` (`enemyPosition - playerPosition`) | **r5 / sp+104** | r5 / sp+44 |
| k-loop layout | **rotated** `[body,then,latch,header]` | not rotated |
| k-loop temp `&(enemy->getPosition())` | recomputed at `0x153444` | **hoisted to i-loop preheader `0x153260`** |

The scatter block is the only place where the *slot* and the *register* are
coupled, and they are coupled in opposite directions in the two known shapes:

```
HEAD     : add r4, sp, #104   (playerPosition)   add r5, sp, #44   (delta)
shipped  : add r4, sp, #44    (playerPosition)   add r5, sp, #104  (delta)
expr form: add r5, sp, #44    (playerPosition)   add r4, sp, #104  (delta)
```

## 2. Root cause: alloca order selects the merge target

`StackColoring` sorts stack objects by size descending (stable, so equal sizes
keep frame-index order) and greedily merges every disjoint later slot into the
first one. The two 60-byte survivors are

```
fi#10 size=60 align=8   view  (memcpy dest, sp+104)
fi#11 size=60 align=4   MatrixSetTranslation sret (sp+44)
```

`fi#10` is processed first, so it absorbs the *first* disjoint 12-byte slot.
The frame-index order of the two scatter temporaries therefore decides the
final offsets:

* HEAD source `Vector playerPosition = ...; int distance = VectorLength(enemyPosition - playerPosition);`
  allocas are `playerPosition`=fi#4, `delta`=fi#5 → fi#4 merges into fi#10
  (sp+104), fi#5 into fi#11 (sp+44). **Slots transposed.**
* Expression form `VectorLength(enemyPosition - getPosition())` allocas are
  `delta`=fi#4 (the binary-op result temp is created first), `getPosition()`
  temp=fi#5 → fi#4 merges into fi#10 (sp+104), fi#5 into fi#11 (sp+44).
  **Slots match the shipped binary.**

This is confirmed by the `Stack Slot Coloring` MIR dump: HEAD remaps
`playerPosition`→`%stack.10`, `delta`→`%stack.11`; the expression form remaps
`playerPosition`→`%stack.11`, `delta`→`%stack.10`.

`pp_anon` (expression form) therefore scores **92.6154** with the correct
slots but the transposed registers:

```
pp_anon : add r5, sp, #44 (playerPosition)   add r4, sp, #104 (delta)
```

## 3. The register choice follows the frame index, not the vreg

Both shapes emit the identical MIR before RA except the `t2ADDri` stack-slot
operand. The two address vregs are `%114` (first, getPosition arg) and `%115`
(second, operator- arg), with identical slot indices `3604B` / `3684B` and
identical live intervals. Yet:

| shape | `%114` | `%115` |
|---|---|---|
| HEAD (fi#4 first) | **r4** | r5 |
| expression form (fi#5 first) | **r5** | r4 |

So the greedy allocator's choice for these two rematerialisable `t2ADDri`
vregs is a function of the *frame index* (or of the slot's lifetime markers,
which are the only other difference), not of the vreg number or interval.
The shipped binary wants `%114`→r4 with the expression form's frame order —
i.e. HEAD's register choice *and* the expression form's alloca order. No
source shape tried reaches that combination.

The `Matrix &view = *CameraGetLocal(...)` variant (no copy, so only the sret
60-byte slot survives) does reach `playerPosition`=r4/sp+44,
`delta`=r5/sp+104 — the exact shipped map — but scores **80.9266** because it
drops the `__aeabi_memcpy` that the shipped binary has.

## 4. `i*3 = r4` is the same problem seen from LICM

`i*3` is a global range (`%98`) allocated after `this` (`%56`) and `td`
(`%11`); `this` is evicted from r4 to r9 by a late local spill child, and
`%98` is re-queued to r5. In the shipped binary `i*3` is r4, so `r4` is
occupied across the i-loop and `MachineLICM` cannot hoist the k-loop temp
address into it: the shipped binary recomputes `add r4, sp, #104` at
`0x153444` inside the k-loop. In HEAD `i*3` is r5, r4 is free, and the address
is hoisted to `0x153260` in the i-loop preheader. Fixing `i*3` would therefore
also fix the hoisting/scheduling delta at `0x153260`/`0x15327a`.

The only shapes that move `i*3` to r4 are (a) making the
`enemy->egoFlag && ki->field_0x8c` check cold (`__builtin_expect(..., 0)`),
which destroys `this`/`enemy` (best 59.40), or (b) adding an extra 12-byte
stack slot (see §6), which grows the frame.

## 5. k-loop rotation still needs a hot `distance < magnitude` edge

MBP's `rotateLoop` only rotates when the loop chain contains the `then` block,
i.e. `selectBestSuccessor(body)` must pick `then` over `latch`. With
`__builtin_expect(distance < magnitude, 0)` the latch wins (99.95%) and the
chain is `[header, body, latch]` → no rotation. A plain `if`, `expect(...,1)`
or `__builtin_unpredictable` makes the edge 50/50 or hot and rotates the loop,
but changes the RA map to `this=r4, enemy=r6, td=r9` (64.5412). The shipped
binary has the *cold* RA map (`this=r9, enemy=r8, td=r6`) **and** the rotated
layout, so its branch weights must differ from both.

`__builtin_expect_with_probability` is not available in clang 7.0.2
(`use of undeclared identifier`), so a fractional weight cannot be expressed
from source.

## 6. New measurements this wave (official verifier path)

All numbers are `pct / size`; `slots` is the first `add rX, sp, #44|104`
pair (playerPosition, delta).

### 6.1 Expression / declaration forms

| variant | pct | size | slots | i3 |
|---|---:|---:|---|---|
| HEAD (`Vector playerPosition = ...; VectorLength(ep - pp)`) | **93.3846** | 1984 | r4/104, r5/44 | r5 |
| `pp_anon` (`VectorLength(ep - getPosition())`) | 92.6154 | 1984 | **r5/44, r4/104** | r5 |
| `Vector delta = ep - getPosition(); VectorLength(delta)` | 92.6154 | 1984 | r5/44, r4/104 | r5 |
| `VectorLength(getPosition() - ep)` | 92.6154 | 1984 | r5/44, r4/104 | r5 |
| `VectorLength(ep - Vector(getPosition()))` | 92.6154 | 1984 | r5/44, r4/104 | r5 |
| nested scope around `playerPosition` | 93.3846 | 1984 | r4/104, r5/44 | r5 |
| `const Vector &playerPosition = ...` | 93.3846 | 1984 | r4/104, r5/44 | r5 |
| `Vector delta;` before `playerPosition` | 63.3359 | 1964 | **r4/44, r4/104** | r5 |
| `Vector delta = enemyPosition; delta -= pp;` | 77.8802 | 1984 | r5/44, r4/104 | r5 |
| `Vector playerPosition;` at i-loop top (HEAD form) | 64.2583 | 1960 | r4/104 ×4 | **r4** |
| `Vector playerPosition;` at function top | 67.4365 | 1984 | r4/104 ×4 | **r4** |
| `int distance;` hoisted to i-loop top (HEAD / pp form) | 93.3846 / 92.6154 | 1984 | unchanged | r5 |
| `KIPlayer *ki;` hoisted to i-loop top | 93.3846 | 1984 | r4/104, r5/44 | r5 |
| `Vector position(...)` copy-ctor form | 93.3846 | 1984 | r4/104, r5/44 | r5 |
| `int radius` before `enemyPosition` | 92.7692 | 1984 | r4/104, r5/44 | r5 |
| `positions[i]` / `velocities[i]` (operator[]) | 93.3846 | 1984 | r4/104, r5/44 | r5 |

### 6.2 Matrix / view forms

| variant | pct | size | slots |
|---|---:|---:|---|
| `Matrix view = *CameraGetLocal(); MatrixSetTranslation(view,...)` (HEAD) | **93.3846** | 1984 | r4/104, r5/44 |
| `Matrix view(*(Matrix *)...)` copy ctor | 93.3846 | 1984 | r4/104, r5/44 |
| `Matrix &view = *CameraGetLocal()` (no memcpy) | 80.9266 | 1984 | **r4/44, r5/104** |
| `Matrix view = MatrixSetTranslation(*CameraGetLocal(),...)` | 81.3900 | 1984 | **r4/44, r5/104** |
| `view = MatrixSetTranslation(view,...)` | 87.0299 | 1984 | r4/104, r5/44 |
| `Matrix view; view = *CameraGetLocal()` | 67.8733 | 1984 | — |

### 6.3 `__builtin_expect` sweeps

* 36 single-expect variants on the `pp_anon` body (18 sites × {0,1}): best
  **92.6154** (many sites); no variant reaches HEAD.
* 36 single-expect variants on the hot body + `expect(mine,0)` (18 sites ×
  {0,1}): best **83.9107** (`empDamage > 0` expect 0); all keep `i3=r4` but
  `this=r9, enemy=r6`.
* Partial cold-body pair sweep (first 34 of 684 pairs): best **93.3846**.
* Loop-condition expects: `e<size` 0/1 = 40.79 / 85.14; `i<count` 0/1 = 43.05
  / 78.96; `k<size` 0/1 = 62.11 / 54.91; `positions.size()` = 93.3846.
* `__builtin_unpredictable(distance < magnitude)` = 64.5412 (same as plain
  `if`); `__builtin_expect_with_probability` = build error in clang 7.

### 6.4 Source-shape variants (all ≤ 93.3846)

`positions.data()[i]`→`positions[i]`; `Player *enemy = enemies->data()[e]`;
`mine != 0`; `isActive() == false`; `getHitpoints() <= 0`; `!ki`; `ki &&`;
`float r = radius`; `damage(9999, 0, ...)`; `++field_12c; count = ...`;
`179 == itemIndex`; `!owner`; `getPlayer() &&`; `impact &&`; `geometries &&`;
`enemies->data()[k]`; `(float)(enemy->radius)`; `(int)kNoAttribute`:
all **93.3846** (codegen identical). Named `dir` in the mine bounce 87.0769,
`velocities[i] * (pitchRate * 100.0f)` 89.2830, `*td = getPosition() - position`
61.2182, `radius = radius * 3` 84.7458.

## 7. Conclusion

The shipped function requires four independent properties simultaneously:

1. `this=r9, enemy=r8, td=r6` — cold `distance` edge (HEAD's RA map).
2. `i*3=r4` — the early eviction of `this` from r4 (needs a cold branch / an
   extra stack slot).
3. `playerPosition=r4/sp+44`, `delta=r5/sp+104` — the expression form's alloca
   order (delta fi#4 before playerPosition fi#5) **and** HEAD's register
   choice.
4. the k-loop rotated — a hot/tied `distance < magnitude` edge.

(1) and (4) are mutually exclusive through the branch weights: the cold edge
gives the shipped RA map but no rotation; the hot edge rotates but gives
`this=r4, enemy=r6, td=r9`. (2) and (3) are mutually exclusive through the
frame index: the expression form fixes (3)'s slots but flips the registers;
the statement form fixes the registers but transposes the slots. No source
shape tried (≈60 this wave on top of ≈1100 from waves 106–118) reaches all
four.

Best measured variant remains the HEAD body at **93.3846 % / 1984 B**.

## 8. Reproduction

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

MIR / stack-colouring evidence:

```
<clang++> -mllvm -print-after-all \
  -mllvm -filter-print-funcs=_ZN3Gun22calcCharacterCollisionEv -c src/game/weapons/Gun.cpp
# "Stack Slot Coloring" -> %stack.10 / %stack.11 remap
# "Prologue/Epilogue Insertion & Frame Finalization" -> fi#10/fi#11 offsets
# "Virtual Register Rewriter" -> %114/%115 physical registers
```

Register probe: `arm-linux-androideabi-objdump -d cmake-build-match/libgof2hdaa.so`
then read `mov r9, r0` (`this`), `add.w rX, fp, fp, lsl #1` (`i*3`),
`add rX, sp, #44|104` (scatter temporaries).
